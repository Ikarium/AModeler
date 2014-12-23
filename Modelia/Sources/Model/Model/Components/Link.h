#pragma once

#include "Interface/Components/LinkView.h"

#include "Slot.h"

namespace Model
{

class Link
{

public:
	Link(Composition *, PropertyTree &);
	
	~Link();

	PropertyTree get() const;
	void set(PropertyTree & ptree);

	Composition * owner();
	Slot * slot1();
	Slot * slot2();

	void connectSlots(Slot *, Slot *);

	Link & operator=(Link const &);

	bool operator == (Link const &) const;

	bool operator != (Link const &) const;

	Interface::LinkView * Link::view();

private:
	class Impl;
	UniquePtr<Impl> m;

};

}