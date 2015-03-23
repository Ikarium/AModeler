#include "Conf.h"

#include "Variable.h"
#include "Model/TypesManager/TypesLibrary.h"

namespace Model
{

class Variable::Impl
{
	Variable* w;

public:

	Type* type;
	Interface::VariableView view;
	QString initialValue;

	Impl(Variable*, PropertyTree &);
	~Impl();

};

Variable::Impl::Impl(Variable* owner, PropertyTree & ptree)
	: w(owner),
	view(owner, ptree.count("View") != 0 ? ptree.get_child("View") : PropertyTree())
{

}

Variable::Impl::~Impl()
{

}

Variable::Variable(Composition * owner, PropertyTree & ptree)
	: m(new Impl(this, ptree)), Component(owner, ptree)
{

	import(ptree);
	m->view.init();
}

/**************************
PropertyTrees
***************************/
PropertyTree Variable::export() const
{
	PropertyTree ptree = Component::export();

	ptree.put_value("Variable");
	ptree.put("Type", m->type->getPath().toStdString());
	ptree.put_child("View", m->view.export());

	return ptree;
}

void Variable::import(PropertyTree & ptree)
{
	checkHierarchy("Variable", QString::fromStdString(ptree.get_value<std::string>()));
	
	if (ptree.count("Type") != 0)
	{
		m->type = App::typesLibrary->usePath(QString::fromStdString(ptree.get<std::string>("Type")));

		PropertyTree slot;
		slot.put_value("Slot");
		slot.put("Type", m->type->getPath().toStdString());
		slot.put("UniqueLink", false);
		slot.put("SlotType", "Input");
		slot.put("Name", name().toStdString() + "_Input");
		Component::addInput(slot);
		slot.put("SlotType", "Output");
		slot.put("Name", name().toStdString() + "_Output");
		Component::addOutput(slot);
	}
	else KThrow(ModeliaExection, lvl7, "Variable ptree must have Type.");

}

/**************************
Slot accessors
***************************/
Slot * Variable::addInput(PropertyTree & ptree)
{
	KThrow(ModeliaExection, lvl5, "Variable inputs are fixed.");

	return nullptr;
}

void Variable::removeInput()
{
	KThrow(ModeliaExection, lvl5, "Variable inputs are fixed.");

}

Slot * Variable::addOutput(PropertyTree & ptree)
{
	KThrow(ModeliaExection, lvl5, "Variable outputs are fixed.");

	return nullptr;
}

void Variable::removeOutput()
{
	KThrow(ModeliaExection, lvl5, "Variable outputs are fixed.");
}
QString Variable::initialValue() { return m->initialValue; }
void Variable::setInitialValue(QString newValue) { m->initialValue = newValue; }
Interface::VariableView * Variable::view()
{
	return &m->view;
}

}
