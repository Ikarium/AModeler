#pragma once

#include "../Class.h"
#include "Slot.h"

namespace Model
{

void checkHierarchy(QString const &, QString const &);

class Component
{

protected:

	Component(Composition *, PropertyTree &);

	virtual ~Component() = 0;

public:

/**************************
PropertyTrees
***************************/
	PropertyTree get() const;
	void set(PropertyTree &);

/**************************
	accessors		 
***************************/
	QString const & name() const;
	void setName(QString const &);
	List<Slot> const & inputs() const;
	List<Slot> const & outputs() const;
	Composition* owner();

/**************************
	inputs accessors
***************************/
	List<Slot> & inputs();
	Slot * addInput(PropertyTree &);
	void removeInput(Slot *);
	List<Slot>::const_iterator findInputSlot(Slot *);
	Slot * findInputSlot(QString);

/**************************
	outputs accessors
***************************/
	List<Slot> & outputs();
	Slot * addOutput(PropertyTree &);
	void removeOutput(Slot *);
	List<Slot>::const_iterator findOutputSlot(Slot *);
	Slot * findOutputSlot(QString);

	virtual Slot * findSlot(QString);
	void removeSlot(Slot *);

	virtual Interface::ComponentView* view() = 0;

	operator QString () const;

	bool operator == (Component const &) const;

	bool operator != (Component const &) const;

private:
	class Impl;
	UniquePtr<Impl> m;

};


}
