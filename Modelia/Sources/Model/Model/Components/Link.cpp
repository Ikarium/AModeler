#include "Conf.h"

#include "Link.h"

#include "Composition.h"
#include "Variable.h"

namespace Model
{

class Link::Impl
{
	Link* w;

public:

	Interface::LinkView view;

	Composition * parrent;

	Slot * slot1 = nullptr;
	Slot * slot2 = nullptr;

	Impl(Link*, Composition *, PropertyTree &);
	~Impl();

};

Link::Impl::Impl(Link* owner, Composition * parrent_, PropertyTree & ptree)
	: w(owner),
	parrent(parrent_),
	view(owner, ptree.count("View") != 0 ? ptree.get_child("View") : PropertyTree())
{

}

Link::Impl::~Impl()
{

}

Link::Link(Composition * parrent, PropertyTree & ptree)
	: m(new Impl(this, parrent, ptree))
{
	import(ptree);
}

Link::~Link()
{
	if (m->slot1->view())
		m->slot1->unlink(this);
	if (m->slot2->view())
		m->slot2->unlink(this);
}

PropertyTree Link::export() const
{
	PropertyTree ptree;

	ptree.put_value("Link");

	if (m->slot1->owner() == m->parrent)
	{
		ptree.put("Slot1.ComponentName", "__Parent");
		ptree.put("Slot1.SlotName", m->slot1->name().toStdString());
		ptree.put("Slot1.SlotType", 
			(m->slot1->slotType() == SlotType::input) ? "Input" : "Output");
	}
	else
	{
		ptree.put("Slot1.ComponentName", m->slot1->owner()->name().toStdString());
		ptree.put("Slot1.SlotName", m->slot1->name().toStdString());
		ptree.put("Slot1.SlotType",
			(m->slot1->slotType() == SlotType::input) ? "Input" : "Output");
	}
	if (m->slot2->owner() == m->parrent)
	{
		ptree.put("Slot2.ComponentName", "__Parent");
		ptree.put("Slot2.SlotName", m->slot2->name().toStdString());
		ptree.put("Slot2.SlotType",
			(m->slot2->slotType() == SlotType::input) ? "Input" : "Output");
	}
	else
	{
		ptree.put("Slot2.ComponentName", m->slot2->owner()->name().toStdString());
		ptree.put("Slot2.SlotName", m->slot2->name().toStdString());
		ptree.put("Slot2.SlotType",
			(m->slot2->slotType() == SlotType::input) ? "Input" : "Output");
	}

	ptree.put_child("View", m->view.export());

	return ptree;
}

void Link::import(PropertyTree & ptree)
{
	checkHierarchy("Link", QString::fromStdString(ptree.get_value<std::string>()));

	Slot * slot1;
	QString slot1Component = QString::fromStdString(ptree.get<std::string>("Slot1.ComponentName"));
	QString slot1Name = QString::fromStdString(ptree.get<std::string>("Slot1.SlotName"));
	SlotType slot1Type = QString::fromStdString(ptree.get<std::string>("Slot1.SlotType")) == "Input"
		? SlotType::input
		: SlotType::output;

	if (slot1Component == "__Parent")
		if (slot1Type == SlotType::input)
			slot1 = m->parrent->findInputSlot(slot1Name);
		else
			slot1 = m->parrent->findOutputSlot(slot1Name);
	else
		if (slot1Type == SlotType::input)
			slot1 = m->parrent->find(slot1Component)->findInputSlot(slot1Name);
		else
			slot1 = m->parrent->find(slot1Component)->findOutputSlot(slot1Name);


	Slot * slot2;
	QString slot2Component = QString::fromStdString(ptree.get<std::string>("Slot2.ComponentName"));
	QString slot2Name = QString::fromStdString(ptree.get<std::string>("Slot2.SlotName"));
	SlotType slot2Type = QString::fromStdString(ptree.get<std::string>("Slot2.SlotType")) == "Input"
		? SlotType::input
		: SlotType::output;

	if (slot2Component == "__Parent")
		if (slot2Type == SlotType::input)
			slot2 = m->parrent->findInputSlot(slot2Name);
		else
			slot2 = m->parrent->findOutputSlot(slot2Name);
	else
		if (slot2Type == SlotType::input)
			slot2 = m->parrent->find(slot2Component)->findInputSlot(slot2Name);
		else
			slot2 = m->parrent->find(slot2Component)->findOutputSlot(slot2Name);

	connectSlots(slot1, slot2);
}

Composition * Link::owner() { return m->parrent; }
Slot * Link::slot1() { return m->slot1; }
Slot * Link::slot2() { return m->slot2; }

void Link::connectSlots(Slot * slot1, Slot * slot2)
{
	if (slot1->vectorized() != slot2->vectorized())
		KThrow(ModeliaExection, lvl7, "Cannot link a vectorized and a unvectorized type, input type: '" + *slot1->type() + "', output type: '" + *slot2->type() + "'");
	if (slot1->type() != slot2->type())
		KThrow(ModeliaExection, lvl7, "Cannot link 2 slot of different type, input type: '" + *slot1->type() + "', output type: '" + *slot2->type() + "'");
	if (slot1->owner() == slot2->owner())
		KThrow(ModeliaExection, lvl7, "Can't link element with itself.");

	if (slot1->slotType() != slot2->slotType())
	{
		if (slot2->owner() == m->parrent || slot1->owner() == m->parrent)
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
	else if (slot1->owner() == m->parrent ^ slot2->owner() == m->parrent)
	{
		Slot * parentSlot = slot1;
		Slot * nonParentSlot = slot2;
		if (slot1->owner() != m->parrent)
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

	m->slot1 = slot1;
	m->slot2 = slot2;
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
	return &m->view;
}

}