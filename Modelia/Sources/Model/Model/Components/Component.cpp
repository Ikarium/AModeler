#include "Conf.h"

#include "Component.h"

namespace Model
{


/**************************
PropertyTrees
***************************/

Component::Component(Composition * owner, PropertyTree & ptree) 
	: owner_(owner)
{ 
	set(ptree);
}

PropertyTree Component::get() const
{
	PropertyTree ptree;

	ptree.put_value("Component");
	ptree.put("Name", name_.toStdString());

	return ptree;
}
void Component::set(PropertyTree & ptree)
{
	checkHierarchy("Component", QString::fromStdString(ptree.get_value<std::string>()));

	name_ = QString::fromStdString(ptree.get<std::string>("Name"));

}

/**************************
accessors		 
***************************/
QString const & Component::name() const { return name_; }
Composition* Component::owner() { return owner_; }
void Component::setName(QString const & name)
{ 
	name_ = name; 
}

/**************************
inputs accessors
***************************/
List<Slot> & Component::inputs()
{
	return inputs_;
}

Slot * Component::addInput(PropertyTree & ptree)
{
	inputs_.emplace_back(this, ptree);
	recalcInputPos();
	return &inputs_.back();
}

void Component::removeInput(Slot * slot)
{
	inputs_.erase(findInputSlot(slot));
	recalcInputPos();
}

List<Slot>::const_iterator
Component::findInputSlot(Slot * slot)
{
	if (slot->owner() != this) KThrow(ModeliaExection, lvl6, "Slot is not owned by this component");
	for (List<Slot>::const_iterator iter = inputs_.cbegin();
		iter != inputs_.cend(); iter++)
		if (*slot == *iter) return iter;

	KThrow(ModeliaExection, lvl7, "Slot not found");

}

Slot * Component::findInputSlot(QString name)
{
	for (Slot & current : inputs_)
		if (current.name() == name) return &current;

	return nullptr;

}

void Component::recalcInputPos()
{
	int i = 0;
	for (List<Slot>::iterator iter = inputs_.begin();
		iter != inputs_.cend(); iter++)
		iter->pos(i++);
}

/**************************
outputs accessors
***************************/

List<Slot> & Component::outputs()
{
	return outputs_;
}

Slot * Component::addOutput(PropertyTree & ptree)
{
	outputs_.emplace_back(this, ptree);
	recalcOutputPos();
	return &outputs_.back();
}

void Component::removeOutput(Slot * elem)
{
	outputs_.erase(findOutputSlot(elem));
	recalcOutputPos();
}

List<Slot>::const_iterator
Component::findOutputSlot(Slot * slot)
{
	if (slot->owner() != this) KThrow(ModeliaExection, lvl6, "Slot is not owned by this component");
	for (List<Slot>::const_iterator iter = outputs_.cbegin();
		iter != outputs_.cend(); iter++)
		if (*slot == *iter) return iter;

	KThrow(ModeliaExection, lvl7, "Slot not found");
}

Slot * Component::findOutputSlot(QString name)
{
	for (Slot & current : outputs_)
		if (current.name() == name) return &current;

	return nullptr;
}

void Component::recalcOutputPos()
{
	int i = 0;
	for (List<Slot>::iterator iter = outputs_.begin();
		iter != outputs_.cend(); iter++)
		iter->pos(i++);
}

Slot * Component::findSlot(QString name)
{
	for (Slot & current : inputs_)
		if (current.name() == name) return &current;

	for (Slot & current : outputs_)
		if (current.name() == name) return &current;

	qDebug() << "Slot " + name + " not found";

	return nullptr;
}

void Component::removeSlot(Slot * slot)
{
	outputs_.remove(*slot);
	inputs_.remove(*slot);
	recalcOutputPos();
	recalcInputPos();
}


Component::operator QString() const
{
	return name_;
}

bool Component::operator == (Component const & comp) const
{
	return comp.name_ == name_;
}

bool Component::operator != (Component const & comp) const
{
	return comp.name_ != name_;
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
