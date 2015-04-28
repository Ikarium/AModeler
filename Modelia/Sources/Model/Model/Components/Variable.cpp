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
	bool vectorized;
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
	ptree.put("Vectorized", m->vectorized);
	ptree.put("InitialValue", m->initialValue.toStdString());
	ptree.put_child("View", m->view.export());

	return ptree;
}

void Variable::import(PropertyTree & ptree)
{
	checkHierarchy("Variable", QString::fromStdString(ptree.get_value<std::string>()));
	
	if (ptree.count("Type") != 0)
	{
		m->type = App::typesLibrary->usePath(QString::fromStdString(ptree.get<std::string>("Type")));
		m->vectorized = ptree.get<bool>("Vectorized");

		PropertyTree slot;
		slot.put_value("Slot");
		slot.put("Type", m->type->getPath().toStdString());
		slot.put("Vectorized", m->vectorized);
		slot.put("UniqueLink", false);
		slot.put("SlotType", "Input");
		slot.put("Name", "Input");
		Component::addInput(slot);
		slot.put("SlotType", "Output");
		slot.put("Name", "Output");
		Component::addOutput(slot);
	}
	else KThrow(ModeliaExection, lvl7, "Variable ptree must have Type.");

	if (ptree.count("InitialValue") != 0)
		m->initialValue = QString::fromStdString(ptree.get<std::string>("InitialValue"));
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
Type * Variable::type() const { return m->type; }
void Variable::setType(Type* type) { m->type = type; }
bool Variable::vectorized() const { return m->vectorized; }
void Variable::setVectorized(bool vectorized) { m->vectorized = vectorized; }
QString Variable::initialValue() const { return m->initialValue; }
void Variable::setInitialValue(QString newValue) { m->initialValue = newValue; }

void Variable::updateSlots()
{
	inputs().front().setType(m->type);
	inputs().front().setVectorized(m->vectorized);
	outputs().front().setType(m->type);
	outputs().front().setVectorized(m->vectorized);
}

Interface::VariableView * Variable::view()
{
	return &m->view;
}

}
