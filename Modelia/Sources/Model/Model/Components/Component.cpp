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

	void recalcSlotsPos();

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
	import(ptree);
}

Component::~Component() {}

/**************************
PropertyTrees
***************************/

PropertyTree Component::export() const
{
	PropertyTree ptree;

	ptree.put_value("Component");
	ptree.put("Name", m->name.toStdString());

	return ptree;
}
void Component::import(PropertyTree & ptree)
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
slots accessors
***************************/
Slot * Component::addOutput(PropertyTree & ptree)
{
	m->outputs.emplace_back(this, ptree);
	m->recalcSlotsPos();
	return &m->outputs.back();
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

List<Slot> & Component::inputs()
{
	return m->inputs;
}

Slot * Component::addInput(PropertyTree & ptree)
{
	m->inputs.emplace_back(this, ptree);
	m->recalcSlotsPos();
	return &m->inputs.back();
}

List<Slot> & Component::outputs()
{
	return m->outputs;
}

void Component::removeSlot(Slot * slot)
{
	m->outputs.remove_if([slot](Slot & t){ return &t == slot; });
	m->inputs.remove_if([slot](Slot & t){ return &t == slot; });
	m->recalcSlotsPos();
}

void Component::Impl::recalcSlotsPos()
{
	int i = 0;
	for (List<Slot>::iterator iter = outputs.begin();
		iter != outputs.cend(); iter++)
		iter->setPos(i++);
	i = 0;
	for (List<Slot>::iterator iter = inputs.begin();
		iter != inputs.cend(); iter++)
		iter->setPos(i++);
}

/**************************
operators accessors
***************************/
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
		given == "Function" ||
		given == "Composition")) return;
	if (base == "Process" &&
		(given == "Function" ||
		given == "Composition")) return;

	QString error = "Bad property tree, needed '" + base + "', given '" + given + "'";
	KThrow(PropertyTreeExeption, lvl6, error);
}

}
