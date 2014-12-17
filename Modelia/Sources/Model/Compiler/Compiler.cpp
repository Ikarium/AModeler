#include "Conf.h"

#include "Compiler.h"
#include "Model/Model/Components/Variable.h"
#include "Model/Model/Components/Composition.h"
#include "Model/Model/Components/CodeFunction.h"
#include "Model/Model/Components/PureFunction.h"
#include "Model/Model/Components/Slot.h"
#include "Model/Model/Components/Link.h"

#include <QFile>

namespace Model
{

class Compiler::Impl
{
	Compiler * owner;

public:

	Impl(Compiler *);

	~Impl();

	QString compileCodeFunction(CodeFunction *, uint8 = 0);
	QString compileComposition(Composition *, uint8 = 0);
	QString compilePureFunction(PureFunction *, uint8 = 0);

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
	stream << "import os" << endl;

	stream << m->compileComposition(root);

	file.close();

	system("Python\\Python34\\python.exe Python/model.py & pause");
	return "";

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
		stream << tab << variable->name() + " = None" << endl;

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
			stream << tab << "\t" << slot.name() << " = None" << endl;

		if (dynamic_cast<Composition*>(process))
			stream	<< compileComposition(dynamic_cast<Composition*>(process), tabCount + 1)
					<< endl;
		else if (dynamic_cast<CodeFunction*>(process))
			stream	<< compileCodeFunction(dynamic_cast<CodeFunction*>(process), tabCount + 1)
					<< endl;
		else if (dynamic_cast<PureFunction*>(process))
			stream	<< compilePureFunction(dynamic_cast<PureFunction*>(process), tabCount + 1)
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
	{
		stream	<< tab << slot.name()
				<< " = "
				<< "var_"
				<< slot.linkedUniqueSlot()->owner()->name()
				<< "_"
				<< slot.linkedUniqueSlot()->name();
	}
	return result;
}

QString Compiler::Impl::compileCodeFunction(CodeFunction * codeFunction, uint8 tabCount)
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

QString Compiler::Impl::compilePureFunction(PureFunction * pureFunction, uint8 tabCount)
{
	QString result;
	QTextStream stream(&result);

	QString tab = "";
	for (int i = 0; i < tabCount; i++)
		tab += "\t";

	return result;
}


QVector<Process*> Compiler::topSort(Composition * root, bool &cycle)
{
	QVector<Process*> result;

	for (Process & current : root->codeFunctions())
		result << &current;
	for (Process & current : root->pureFunctions())
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
