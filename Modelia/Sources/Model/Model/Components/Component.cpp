#include "Conf.h"

#include "Component.h"

namespace Model
{

class Component::Impl
{
	Component* w;

public:

	Composition*	owner;
	QString			name;
	List<Slot>		inputs;
	List<Slot>		outputs;

	Impl(Component*, Composition *, PropertyTree &);
	~Impl();

	void recalcOutputPos();
	void recalcInputPos();

};

Component::Impl::Impl(Component* pimplOwner, Composition * owner_, PropertyTree & ptree)
	: w(pimplOwner), owner(owner_)
{

}

Component::Impl::~Impl()
{

}

Component::Component(Composition * owner, PropertyTree & ptree)
	: m(new Impl(this, owner, ptree))
{
	set(ptree);
}

Component::~Component() {}

/**************************
PropertyTrees
***************************/

PropertyTree Component::get() const
{
	PropertyTree ptree;

	ptree.put_value("Component");
	ptree.put("Name", m->name.toStdString());

	return ptree;
}
void Component::set(PropertyTree & ptree)
{
	checkHierarchy("Component", QString::fromStdString(ptree.get_value<std::string>()));

	m->name = QString::fromStdString(ptree.get<std::string>("Name"));

}

/**************************
accessors		 
***************************/
QString const & Component::name() const			{ return m->name; }
Composition* Component::owner()					{ return m->owner; }
void Component::setName(QString	const & name)	{ m->name = name; }
List<Slot> const &  Component::inputs()	const	{ return m->inputs; }
List<Slot> const &  Component::outputs() const	{ return m->outputs; }

/**************************
inputs accessors
***************************/
List<Slot> & Component::inputs()
{
	return m->inputs;
}

Slot * Component::addInput(PropertyTree & ptree)
{
	m->inputs.emplace_back(this, ptree);
	m->recalcInputPos();
	return &m->inputs.back();
}

void Component::removeInput(Slot * slot)
{
	m->inputs.erase(findInputSlot(slot));
	m->recalcInputPos();
}

List<Slot>::const_iterator
Component::findInputSlot(Slot * slot)
{
	if (slot->owner() != this) KThrow(ModeliaExection, lvl6, "Slot is not owned by this component");
	for (List<Slot>::const_iterator iter = m->inputs.cbegin();
		iter != m->inputs.cend(); iter++)
		if (*slot == *iter) return iter;

	KThrow(ModeliaExection, lvl7, "Slot not found");

}

Slot * Component::findInputSlot(QString name)
{
	for (Slot & current : m->inputs)
		if (current.name() == name) return &current;

	return nullptr;

}

void Component::Impl::recalcInputPos()
{
	int i = 0;
	for (List<Slot>::iterator iter = inputs.begin();
		iter != inputs.cend(); iter++)
		iter->pos(i++);
}

/**************************
outputs accessors
***************************/

List<Slot> & Component::outputs()
{
	return m->outputs;
}

Slot * Component::addOutput(PropertyTree & ptree)
{
	m->outputs.emplace_back(this, ptree);
	m->recalcOutputPos();
	return &m->outputs.back();
}

void Component::removeOutput(Slot * elem)
{
	m->outputs.erase(findOutputSlot(elem));
	m->recalcOutputPos();
}

List<Slot>::const_iterator
Component::findOutputSlot(Slot * slot)
{
	if (slot->owner() != this) KThrow(ModeliaExection, lvl6, "Slot is not owned by this component");
	for (List<Slot>::const_iterator iter = m->outputs.cbegin();
		iter != m->outputs.cend(); iter++)
		if (*slot == *iter) return iter;

	KThrow(ModeliaExection, lvl7, "Slot not found");
}

Slot * Component::findOutputSlot(QString name)
{
	for (Slot & current : m->outputs)
		if (current.name() == name) return &current;

	return nullptr;
}

void Component::Impl::recalcOutputPos()
{
	int i = 0;
	for (List<Slot>::iterator iter = outputs.begin();
		iter != outputs.cend(); iter++)
		iter->pos(i++);
}

Slot * Component::findSlot(QString name)
{
	for (Slot & current : m->inputs)
		if (current.name() == name) return &current;

	for (Slot & current : m->outputs)
		if (current.name() == name) return &current;

	qDebug() << "Slot " + name + " not found";

	return nullptr;
}

void Component::removeSlot(Slot * slot)
{
	m->outputs.remove(*slot);
	m->inputs.remove(*slot);
	m->recalcOutputPos();
	m->recalcInputPos();
}


Component::operator QString() const
{
	return m->name;
}

bool Component::operator == (Component const & comp) const
{
	return comp.name() == m->name;
}

bool Component::operator != (Component const & comp) const
{
	return comp.name() != m->name;
}

void checkHierarchy(QString const & base, QString const & given)
{
	if (base == given) return;
	if (base == "Component" &&
		(given == "Variable" ||
		given == "Process" ||
		given == "PureFunction" ||
		given == "CodeFunction" ||
		given == "Composition")) return;
	if (base == "Process" &&
		(given == "PureFunction" ||
		given == "CodeFunction" ||
		given == "Composition")) return;

	QString error = "Bad property tree, needed '" + base + "', given '" + given + "'";
	KThrow(PropertyTreeExeption, lvl6, error);
}

}
