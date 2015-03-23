#pragma once

#include "Interface/Components/SlotView.h"

#include "Model/TypesManager/Type.h"

namespace Model
{

enum class SlotType{ input, output };

class Slot
{

public:
	Slot(Component*, PropertyTree &);
	~Slot();

	/**************************
	PropertyTrees
	***************************/
	PropertyTree export() const;
	void import(PropertyTree & ptree);

	void setLinks();

	/**************************
	Getter
	***************************/

	QString const & name() const;
	Type const & type() const;
	int const pos() const;
	Component const * owner() const;
	Component* owner();
	SlotType slotType();
	List<Link *> & links();
	Link * uniqueLink();
	Vector<Slot*> linkedSlots();
	Slot* linkedUniqueSlot();
	Interface::SlotView * view();
	Interface::SlotView * const view() const;

	/**************************
	Setter
	***************************/
	void setPos(int pos);
	void setName(QString &);
	void setViewDeleted();

	bool canLink();

	void connectLink(Link *);
	void connectUniqueLink(Link *);
	void unlink(Link *);


	Slot & operator=(Slot const &);

	bool operator ==(Slot const &) const;

	bool operator !=(Slot const &) const; 

private:
	class Impl;
	UniquePtr<Impl> m;
};

}