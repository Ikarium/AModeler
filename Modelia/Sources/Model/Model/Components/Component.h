#pragma once

#include "Model/TypesManager/Type.h"
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
	PropertyTree export() const;
	void import(PropertyTree &);

/**************************
	accessors		 
***************************/
	QString const & name() const;
	void setName(QString const &);
	List<Slot> const & inputs() const;
	List<Slot> const & outputs() const;
	Composition* owner();
	virtual Interface::ComponentView* view() = 0;

/**************************
slots accessors
***************************/
	virtual Slot * findSlot(QString);
	virtual Slot * findInputSlot(QString);
	virtual Slot * findOutputSlot(QString);
	void removeSlot(Slot *);

	List<Slot> & inputs();
	virtual Slot * addInput(PropertyTree &);

	List<Slot> & outputs();
	virtual Slot * addOutput(PropertyTree &);

/**************************
operators accessors
***************************/
	operator QString () const;
	bool operator == (Component const &) const;
	bool operator != (Component const &) const;

/**************************
PIMPL
***************************/
private:
	class Impl;
	UniquePtr<Impl> m;

};


}
