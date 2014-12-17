#pragma once

#include "Component.h"

namespace Model
{

class Process : public Component
{

public:

	Process(Composition *, PropertyTree &);
	virtual ~Process() = 0 {}

/**************************
PropertyTrees
***************************/
	PropertyTree get() const;

	void set(PropertyTree &);

	Slot * addInput(PropertyTree &);

	Slot * addOutput(PropertyTree &);

	virtual Interface::ComponentView * view() override = 0;
};

}