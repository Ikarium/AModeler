#include "Conf.h"

#include "Variable.h"

namespace Model
{

class Variable::Impl
{
	Variable* w;

public:

	Type type;
	Interface::VariableView view;

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

	set(ptree);
	m->view.init();
}

/**************************
PropertyTrees
***************************/
PropertyTree Variable::get() const
{
	PropertyTree ptree = Component::get();

	ptree.put_value("Variable");
	ptree.put_child("Type", m->type.get());
	ptree.put_child("View", m->view.get());

	return ptree;
}

void Variable::set(PropertyTree & ptree)
{
	checkHierarchy("Variable", QString::fromStdString(ptree.get_value<std::string>()));
	
	if (ptree.count("Type") != 0)
	{
		m->type = Type(ptree.get_child("Type"));

		PropertyTree slot;
		slot.put_value("Slot");
		slot.put_child("Type", m->type.get());
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
inputs accessors
***************************/
void Variable::addInput(PropertyTree & ptree)
{
	KThrow(ModeliaExection, lvl5, "Variable inputs are fixed.");

}

void Variable::removeInput()
{
	KThrow(ModeliaExection, lvl5, "Variable inputs are fixed.");
}


/**************************
outputs accessors
***************************/
void Variable::addOutput(PropertyTree & ptree)
{
	KThrow(ModeliaExection, lvl5, "Variable outputs are fixed.");
}

void Variable::removeOutput()
{
	KThrow(ModeliaExection, lvl5, "Variable outputs are fixed.");
}

Interface::VariableView * Variable::view()
{
	return &m->view;
}

}
