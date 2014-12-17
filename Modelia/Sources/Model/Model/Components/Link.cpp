#include "Conf.h"

#include "Link.h"

#include "Composition.h"
#include "Variable.h"

namespace Model
{

Link::Link(Composition* _owner, PropertyTree & ptree)
	: owner_(_owner),
	view_(this, ptree.count("View") != 0 ? ptree.get_child("View") : PropertyTree())
{ set(ptree); }

Link::~Link()
{
	if (slot1_->view())
		slot1_->unlink(this);
	if (slot2_->view())
		slot2_->unlink(this);
}

PropertyTree Link::get() const
{
	PropertyTree ptree;

	ptree.put_value("Link");

	if (slot1_->owner() == owner_)
	{
		ptree.put("Slot1.ComponentName", "__Parent");
		ptree.put("Slot1.SlotName", slot1_->name().toStdString());
	}
	else
	{
		ptree.put("Slot1.ComponentName", slot1_->owner()->name().toStdString());
		ptree.put("Slot1.SlotName", slot1_->name().toStdString());
	}
	if (slot2_->owner() == owner_)
	{
		ptree.put("Slot2.ComponentName", "__Parent");
		ptree.put("Slot2.SlotName", slot2_->name().toStdString());
	}
	else
	{
		ptree.put("Slot2.ComponentName", slot2_->owner()->name().toStdString());
		ptree.put("Slot2.SlotName", slot2_->name().toStdString());
	}

	ptree.put_child("View", view_.get());

	return ptree;
}

void Link::set(PropertyTree & ptree)
{
	checkHierarchy("Link", QString::fromStdString(ptree.get_value<std::string>()));

	Slot * slot1;
	Slot * slot2;
	qDebug() << QString::fromStdString(ptree.get<std::string>("Slot1.ComponentName"));
	qDebug() << QString::fromStdString(ptree.get<std::string>("Slot1.SlotName"));
	qDebug() << QString::fromStdString(ptree.get<std::string>("Slot2.ComponentName"));
	qDebug() << QString::fromStdString(ptree.get<std::string>("Slot2.SlotName"));

	if (QString::fromStdString(ptree.get<std::string>("Slot1.ComponentName")) == "__Parent")
		slot1 = owner_->findSlot(
		QString::fromStdString(ptree.get<std::string>("Slot1.SlotName")));
	else
		slot1 = owner_->find(
					QString::fromStdString(ptree.get<std::string>("Slot1.ComponentName")))->
					findSlot(
					QString::fromStdString(ptree.get<std::string>("Slot1.SlotName")));
	
	if (QString::fromStdString(ptree.get<std::string>("Slot2.ComponentName")) == "__Parent")
		slot2 = owner_->findSlot(
		QString::fromStdString(ptree.get<std::string>("Slot2.SlotName")));
	else
		slot2 = owner_->find(
					QString::fromStdString(ptree.get<std::string>("Slot2.ComponentName")))->
					findSlot(
					QString::fromStdString(ptree.get<std::string>("Slot2.SlotName")));

	connectSlots(slot1, slot2);
}

Composition * Link::owner() { return owner_; }
Slot * Link::slot1() { return slot1_; }
Slot * Link::slot2() { return slot2_; }

void Link::connectSlots(Slot * slot1, Slot * slot2)
{

	if (slot1->type() != slot2->type())
		KThrow(ModeliaExection, lvl7, "Cannot link 2 slot of different type, input type: '" + slot1->type() + "', output type: '" + slot2->type() + "'");
	if (slot1->owner() == slot2->owner())
		KThrow(ModeliaExection, lvl7, "Can't link element with itself.");

	if (slot1->slotType() != slot2->slotType())
	{
		if (slot2->owner() == owner_ || slot1->owner() == owner_)
			KThrow(ModeliaExection, lvl7, "Connection with parent must be between two outputs or inputs.");
		if (dynamic_cast<Variable*>(slot1->owner())
			&& dynamic_cast<Variable*>(slot2->owner()))
		KThrow(ModeliaExection, lvl7, "Can't link two Variables.");

		if (slot1->slotType() == SlotType::input && !dynamic_cast<Variable*>(slot1->owner()))
			slot1->connectUniqueLink(this);
		else
			slot1->connectLink(this);
		if (slot2->slotType() == SlotType::input && !dynamic_cast<Variable*>(slot2->owner()))
			slot2->connectUniqueLink(this);
		else
			slot2->connectLink(this);
	}
	else if (slot1->owner() == owner_ ^ slot2->owner() == owner_)
	{
		Slot * parentSlot = slot1;
		Slot * nonParentSlot = slot2;
		if (slot1->owner() != owner_)
			std::swap(parentSlot, nonParentSlot);

		if (parentSlot->slotType() == SlotType::input)
			parentSlot->connectLink(this);
		else
			parentSlot->connectUniqueLink(this);

		if (nonParentSlot->slotType() == SlotType::input
			&& !dynamic_cast<Variable*>(nonParentSlot->owner()))
			nonParentSlot->connectUniqueLink(this);
		else
			nonParentSlot->connectLink(this);
	}
	else
		KThrow(ModeliaExection, lvl7, "Link have to connect output and input (exept of parent connection).");

	slot1_ = slot1;
	slot2_ = slot2;
}

Link & Link::operator=(Link const & link)
{
	KThrow(ModeliaExection, lvl6, "No assignement for Links !");
	return *this;
}

bool Link::operator == (Link const & link) const
{
	return (&link == this);
}

bool Link::operator != (Link const & link) const
{
	return (&link != this);
}

Interface::LinkView * Link::view()
{
	return &view_;
}

}