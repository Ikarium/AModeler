#pragma once

#include "Slot.h"

#include "Component.h"
#include "Variable.h"
#include "Link.h"
#include "Composition.h"

namespace Model
{

Slot::Slot(Component* owner, PropertyTree & ptree) 
	: view_(new Interface::SlotView(this, ptree)), 
	owner_(owner)
{
	set(ptree);
}

Slot::~Slot()
{
	if (view_)
		delete view_;
	view_ = nullptr;

	for (Link* link : links_)
		link->owner()->removeLink(link); 

	if (uniqueLink_ && uniqueLink_->slot1())
		uniqueLink_->owner()->removeLink(uniqueLink_);

}

/**************************
PropertyTrees
***************************/
PropertyTree Slot::get() const
{
	PropertyTree ptree;

	ptree.put_value("Slot");

	ptree.put("Name", name_.toStdString());
	ptree.put("Pos", pos_);
	ptree.put("SlotType", (slotType_ == SlotType::input) ? "Input" : "Output");
	ptree.put_child("Type", type_.get());

	ptree.put_child("View", view_->get());

	return ptree;
}

void Slot::set(PropertyTree & ptree)
{
	checkHierarchy("Slot",
		QString::fromStdString(ptree.get_value<std::string>()));

	name_ = QString::fromStdString(ptree.get<std::string>("Name"));

	pos_ = (ptree.count("Pos") == 0) ? 0 : ptree.get<uint>("Pos");
	slotType_ = QString::fromStdString(ptree.get<std::string>("SlotType")) == "Input" 
					? SlotType::input 
					: SlotType::output;
	type_ = Type(ptree.get_child("Type"));

}

/**************************
Getter
***************************/

QString const & Slot::name() const { return name_; }
Type const & Slot::type() const { return type_; }
int const Slot::pos() const { return pos_; }
Component const * Slot::owner() const { return owner_; }
Component* Slot::owner() { return owner_; }
SlotType Slot::slotType() { return slotType_; }
List<Link*> & Slot::links() { return links_; }
Link * Slot::uniqueLink() { return uniqueLink_; }
Interface::SlotView * Slot::view() { return view_; }
Interface::SlotView * const Slot::view() const { return view_; }

Vector<Slot*> Slot::linkedSlots()
{
	Vector<Slot*> linkedSlots;
	for (Link* link : links_)
		if (link->slot1() == this)
			linkedSlots.push_back(link->slot2());
		else
			linkedSlots.push_back(link->slot1());

	return linkedSlots;
}

Slot* Slot::linkedUniqueSlot()
{
	if (uniqueLink_->slot1() == this)
		return uniqueLink_->slot2();
	else
		return uniqueLink_->slot1();
}

/**************************
Setter
***************************/

void Slot::pos(int pos) { pos_ = pos; }
void Slot::setName(QString & name) { name_ = name; }
void Slot::setViewDeleted() { view_ = nullptr; }

bool Slot::canLink() { return (!uniqueLink_ || links_.size() == 0); }

void Slot::connectLink(Link * link)
{
	links_.emplace_back(link);
}

void Slot::connectUniqueLink(Link * link)
{
	uniqueLink_ = link;
}

void Slot::unlink(Link * linkToDelete)
{
	links_.remove_if([linkToDelete](Link * t){ return t == linkToDelete; });

	if (linkToDelete == uniqueLink_)
	{
		uniqueLink_ = nullptr;
		if (slotType_ == SlotType::input)
				owner_->removeSlot(this);
	}
}

Slot & Slot::operator=(Slot const & slot)
{
	KThrow(ModeliaExection, lvl6, "No assignement for Slots !");
	return *this;
}

bool Slot::operator ==(Slot const & slot) const
{
	if (slot.owner_ == owner_ && slot.name_ == name_)
		return true;
	return false;
}

bool Slot::operator !=(Slot const & slot) const
{
	if (slot.owner_ != owner_ || slot.pos_ != pos_);
	return true;
	return false;
}

}