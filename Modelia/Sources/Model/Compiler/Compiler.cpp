#include "Conf.h"

#include "Compiler.h"
#include "Model/Model/Components/Variable.h"
#include "Model/Model/Components/Composition.h"
#include "Model/Model/Components/Function.h"
#include "Model/Model/Components/Slot.h"
#include "Model/Model/Components/Link.h"

#include "Model/TypesManager/TypesLibrary.h"
#include "Model/TypesManager/Type.h"

#include <QFile>

namespace Model
{

class Compiler::Impl
{
	Compiler * owner;

public:

	Impl(Compiler *);

	~Impl();

	QString compileLibraryItem(TypesLibraryItem *, uint8 = 0);
	QString compileNamespace(TypesLibraryItem *, uint8 = 0);
	QString compileType(Type *, uint8 = 0);

	QString compileFunction(Function *, uint8 = 0);
	QString compileComposition(Composition *, uint8 = 0);

};

Compiler::Compiler() : m(new Impl(this))
{

}

Compiler::~Compiler()
{

}

Compiler::Impl::Impl(Compiler * owner_) : owner(owner_)
{

}

Compiler::Impl::~Impl()
{

}

QString Compiler::aModelToPhython(Composition * root)
{
	QFile file("Python/model.py");

	file.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream stream(&file);
	stream << "import os" << endl << endl;

	stream << m->compileLibraryItem(App::typesLibrary->root()->child("Model"));
	stream << endl;
	stream << m->compileComposition(root);

	file.close();

	system("Python\\Python32\\python.exe Python/model.py & pause");
	return "";

}

QString Compiler::Impl::compileLibraryItem(TypesLibraryItem * item, uint8 tabCount)
{
	QString result;
	QTextStream stream(&result);

	if (item->type())
		stream << compileType(item->type(), tabCount);
	else
		stream << compileNamespace(item, tabCount);

	return result;
}

QString Compiler::Impl::compileNamespace(TypesLibraryItem * item, uint8 tabCount)
{
	QString result;
	QTextStream stream(&result);

	QString tab = "";
	for (int i = 0; i < tabCount; i++)
		tab += "\t";

	stream << tab << "class " << item->name() << ":" << endl;

	bool empty = true;
	for (TypesLibraryItem * subItem : item->children())
		if (subItem->type())
		{
			if (subItem->type()->isStructure())
			{
				empty = false;
				stream << compileType(subItem->type(), tabCount + 1);
			}
		}
		else
		{
			empty = false;
			stream << compileNamespace(subItem, tabCount + 1);
		}

	if (empty)
		stream << tab << "\t" << "None" << endl;

	return result;
}

QString Compiler::Impl::compileType(Type * type, uint8 tabCount)
{
	QString result;
	QTextStream stream(&result);

	QString tab = "";
	for (int i = 0; i < tabCount; i++)
		tab += "\t";

	if (type->isStructure())
	{
		stream << tab << "class " << type->name() << ":" << endl;

		if (type->fields().count() == 0)
			stream << tab << "\t" << "None" << endl;

		for (auto field = type->fields().cbegin(); field != type->fields().cend(); ++field)
			if (field.value()->type->isBaseType())
				if (field.value()->vectorized)
					stream << tab << "\t" << field.key() << " = []" << endl;
				else stream << tab << "\t" << field.key() << " = None" << endl;
			else if (field.value()->type->isStructure())
				if (field.value()->vectorized)
					stream << tab << "\t" << field.key() << " = [" << field.value()->type->name() << "()" << "]" << endl;
				else stream << tab << "\t" << field.key() << " = " << field.value()->type->name() << "()" << endl;
	}

	return result;
}

QString Compiler::Impl::compileComposition(Composition * composition, uint8 tabCount)
{

	QString result;
	QTextStream stream(&result);

	QString tab = "";
	for (int i = 0; i < tabCount; i++)
		tab += "\t";

	QVector<Variable*> variablesList;
	for (Variable & variable : composition->variables())
		variablesList << &variable;
	bool cycle;
	QVector<Process*> processList = owner->topSort(composition, cycle);

	if (cycle) return "Cycle !";

	for (Variable* variable : variablesList)
		if (variable->initialValue() != "")
			stream << tab << variable->name() + " = " + variable->initialValue() << endl;
		else if (variable->type()->isBaseType() || variable->type()->isAlias() && variable->type()->alias()->type->isBaseType())
			if (variable->vectorized())
				stream << tab << variable->name() + " = []" << endl;
			else
				stream << tab << variable->name() + " = None" << endl;
		else if (variable->type()->isStructure())
			if (variable->vectorized())
				stream << tab << variable->name() + " = [" << variable->type()->getPath() << "()" << "]" << endl;
			else
				stream << tab << variable->name() + " = " << variable->type()->getPath() << "()" << endl;
		else if (variable->type()->isAlias())
			if (variable->type()->alias()->vectorized)
				if (variable->vectorized())
					stream << tab << variable->name() + " = [" << variable->type()->alias()->type->getPath() << "()" << "][]" << endl;
				else
					stream << tab << variable->name() + " = [" << variable->type()->alias()->type->getPath() << "()" << "]" << endl;
			else
				if (variable->vectorized())
					stream << tab << variable->name() + " = [" << variable->type()->alias()->type->getPath() << "()" << "]" << endl;
				else
					stream << tab << variable->name() + " = " << variable->type()->alias()->type->getPath() << "()" << endl;


	for (Process* process : processList)
	{
		stream << tab << "def " << process->name() << "(";

		stream << "iteration";
		for (Slot& slot : process->inputs())
		{
			stream << ", ";
			stream << slot.name();
		}

		stream << "):" << endl;


		for (Slot& slot : process->outputs())
			if (process->findInputSlot(slot.name()) == nullptr)
				if (slot.type()->isBaseType() || slot.type()->isAlias() && slot.type()->alias()->type->isBaseType())
					if (slot.vectorized())
						stream << tab << "\t" << slot.name() << " = []" << endl;
					else
						stream << tab << "\t" << slot.name() << " = None" << endl;
				else if (slot.type()->isStructure())
					if (slot.vectorized())
						stream << tab << "\t" << slot.name() << " = [" << slot.type()->getPath() << "()" << "]" << endl;
					else
						stream << tab << "\t" << slot.name() << " = " << slot.type()->getPath() << "()" << endl;		
				else if (slot.type()->isAlias())
					if (slot.type()->alias()->vectorized)
						if (slot.vectorized())
							stream << tab << "\t" << slot.name() + " = [" << slot.type()->alias()->type->getPath() << "()" << "][]" << endl;
						else
							stream << tab << "\t" << slot.name() + " = [" << slot.type()->alias()->type->getPath() << "()" << "]" << endl;
					else
						if (slot.vectorized())
							stream << tab << "\t" << slot.name() + " = [" << slot.type()->alias()->type->getPath() << "()" << "]" << endl;
						else
							stream << tab << "\t" << slot.name() + " = " << slot.type()->alias()->type->getPath() << "()" << endl;


		if (dynamic_cast<Composition*>(process))
			stream	<< compileComposition(dynamic_cast<Composition*>(process), tabCount + 1)
					<< endl;
		else if (dynamic_cast<Function*>(process))
			stream	<< compileFunction(dynamic_cast<Function*>(process), tabCount + 1)
					<< endl;
		else if (dynamic_cast<Function*>(process))
			stream	<< compileFunction(dynamic_cast<Function*>(process), tabCount + 1)
					<< endl;

		stream << tab << "\t" << "return (";
		bool first = true;
		for (Slot& slot : process->outputs())
		{
			if (!first) stream << ", ";
			first = false;
			stream << slot.name();
		}
		stream << ")" << endl;

		stream << endl;

	}

	stream	<< tab << "for iteration in range("
			<< QString::number(composition->iterationCount()) << "):"
			<< endl;
	tab += "\t";
	stream << tab << "None" << endl;
	for (Process* process : processList)
	{
		stream << tab;
		if (process->outputs().size() != 0)
		{
			bool first = true;
			for (Slot& slot : process->outputs())
			{
				if (!first) stream << ", ";
				first = false;
				stream	<< "var_"
						<< process->name() 
						<< "_" 
						<< slot.name();
			}
			stream << " = ";
		}
		stream << process->name() << "(";

		stream << "iteration";
		for (Slot& slot : process->inputs())
		{
			stream << ", ";
			if (dynamic_cast<Variable*>(slot.linkedUniqueSlot()->owner()))
				stream << slot.linkedUniqueSlot()->owner()->name();
			else if (slot.linkedUniqueSlot()->owner() == slot.owner()->owner())
				stream << slot.linkedUniqueSlot()->name();
			else
				stream	<< "var_"
						<< slot.linkedUniqueSlot()->owner()->name()
						<< "_" 
						<< slot.linkedUniqueSlot()->name();
		}

		stream << ")" << endl;

		for (Slot& slot : process->outputs())
		{
			for (auto& linkedSlot : slot.linkedSlots())
				if (dynamic_cast<Variable*>(linkedSlot->owner()))
					stream << tab << linkedSlot->owner()->name()
					<< " = "
					<< "var_"
					<< process->name()
					<< "_"
					<< slot.name()
					<< endl;
		}
	}

	for (Slot& slot : composition->outputs())
		if (dynamic_cast<Variable*>(slot.linkedUniqueSlot()->owner()))
			stream << tab << slot.name()
			<< " = "
			<< slot.linkedUniqueSlot()->owner()->name()
			<< endl;
		else
			stream	<< tab << slot.name()
					<< " = "
					<< "var_"
					<< slot.linkedUniqueSlot()->owner()->name()
					<< "_"
					<< slot.linkedUniqueSlot()->name()
					<< endl;

	return result;
}

QString Compiler::Impl::compileFunction(Function * codeFunction, uint8 tabCount)
{
	QString result;
	QTextStream stream(&result);

	QString tab = "";
	for (int i = 0; i < tabCount; i++)
		tab += "\t";

	QString code = codeFunction->code();

	for (QString line : code.split(QRegExp("\n|\r\n|\r")))
		stream << tab << line << endl;

	return result;
}

QVector<Process*> Compiler::topSort(Composition * root, bool &cycle)
{
	QVector<Process*> result;

	for (Process & current : root->functions())
		result << &current;
	for (Process & current : root->compositions())
		result << &current;

	int sortedCount = 0;
	for (int i = 0; i < result.size(); i++)
	{
		bool free = true;
		for (Slot & slot : result[i]->inputs())
		{
			if (slot.linkedUniqueSlot()->owner() != result[i]->owner()
				&& !dynamic_cast<Variable*>(slot.linkedUniqueSlot()->owner()))
			{
				free = false;
				for (int j = 0; j < sortedCount; j++)
					if (slot.linkedUniqueSlot()->owner() == result[j])
					{
						free = true;
						break;
					}
				if (!free) break;
			}
		}

		if (!free) continue;

		std::swap(result[i], result[sortedCount]);
		i = sortedCount++;
	}

	cycle = false;
	if (sortedCount != result.size())
		cycle = true;

	return result;
}

}
