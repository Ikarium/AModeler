#pragma once

#include "Slot.h"

#include "Component.h"
#include "Variable.h"
#include "Link.h"
#include "Composition.h"
#include "Model/TypesManager/TypesLibrary.h"

namespace Model
{

class Slot::Impl
{

	Slot* w;

public:

	Interface::SlotView * view = nullptr;
	SlotType slotType;
	Type* type = nullptr;
	uint pos;
	Component* owner;
	QString name;
	List<Link*> links;
	Link* uniqueLink = nullptr;

	Impl(Slot*, Component*, PropertyTree &);
	~Impl();

};

Slot::Impl::Impl(Slot* pimplOwner, Component* owner_, PropertyTree & ptree)
	: w(pimplOwner),
	owner(owner_),
	view(new Interface::SlotView(pimplOwner, ptree))
{

}

Slot::Impl::~Impl()
{

}

Slot::Slot(Component * owner, PropertyTree & ptree)
	: m(new Impl(this, owner, ptree))
{
	import(ptree);
}

Slot::~Slot()
{
	if (m->view)
		delete m->view;
	m->view = nullptr;

	for (Link* link : m->links)
		link->owner()->removeLink(link); 

	if (m->uniqueLink && m->uniqueLink->slot1())
		m->uniqueLink->owner()->removeLink(m->uniqueLink);

}

/**************************
PropertyTrees
***************************/
PropertyTree Slot::export() const
{
	PropertyTree ptree;

	ptree.put_value("Slot");

	ptree.put("Name", m->name.toStdString());
	ptree.put("Pos", m->pos);
	ptree.put("SlotType", (m->slotType == SlotType::input) ? "Input" : "Output");

	ptree.put("Type", m->type->getPath().toStdString());

	ptree.put_child("View", m->view->export());

	return ptree;
}

void Slot::import(PropertyTree & ptree)
{
	checkHierarchy("Slot",
		QString::fromStdString(ptree.get_value<std::string>()));

	m->name = QString::fromStdString(ptree.get<std::string>("Name"));

	m->pos = (ptree.count("Pos") == 0) ? 0 : ptree.get<uint>("Pos");
	m->slotType = QString::fromStdString(ptree.get<std::string>("SlotType")) == "Input"
					? SlotType::input 
					: SlotType::output;
	m->type = App::typesLibrary->usePath(QString::fromStdString(ptree.get<std::string>("Type")));

}

/**************************
Getter
***************************/

QString const & Slot::name() const { return m->name; }
Type const & Slot::type() const { return *m->type; }
int const Slot::pos() const { return m->pos; }
Component const * Slot::owner() const { return m->owner; }
Component* Slot::owner() { return m->owner; }
SlotType Slot::slotType() { return m->slotType; }
List<Link*> & Slot::links() { return m->links; }
Link * Slot::uniqueLink() { return m->uniqueLink; }
Interface::SlotView * Slot::view() { return m->view; }
Interface::SlotView * const Slot::view() const { return m->view; }

Vector<Slot*> Slot::linkedSlots()
{
	Vector<Slot*> linkedSlots;
	for (Link* link : m->links)
		if (link->slot1() == this)
			linkedSlots.push_back(link->slot2());
		else
			linkedSlots.push_back(link->slot1());

	return linkedSlots;
}

Slot* Slot::linkedUniqueSlot()
{
	if (m->uniqueLink->slot1() == this)
		return m->uniqueLink->slot2();
	else
		return m->uniqueLink->slot1();
}

/**************************
Setter
***************************/

void Slot::setPos(int pos) { m->pos = pos; }
void Slot::setName(QString & name) { m->name = name; }
void Slot::setViewDeleted() { m->view = nullptr; }

bool Slot::canLink() { return (!m->uniqueLink || m->links.size() == 0); }

void Slot::connectLink(Link * link)
{
	m->links.emplace_back(link);
}

void Slot::connectUniqueLink(Link * link)
{
	m->uniqueLink = link;
}

void Slot::unlink(Link * linkToDelete)
{
	m->links.remove_if([linkToDelete](Link * t){ return t == linkToDelete; });

	if (linkToDelete == m->uniqueLink)
	{
		m->uniqueLink = nullptr;
		if (m->slotType == SlotType::input)
			m->owner->removeSlot(this);
	}
}

Slot & Slot::operator=(Slot const & slot)
{
	KThrow(ModeliaExection, lvl6, "No assignement for Slots !");
	return *this;
}

bool Slot::operator ==(Slot const & slot) const
{
	if (slot.owner() == m->owner && slot.name() == m->name)
		return true;
	return false;
}

bool Slot::operator !=(Slot const & slot) const
{
	if (slot.owner() != m->owner || slot.pos() != m->pos);
	return true;
	return false;
}

}