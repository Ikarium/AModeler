#pragma once

#include "Component.h"

namespace Model
{

class Process : public Component
{

protected:

	Process(Composition *, PropertyTree &);
	virtual ~Process() = 0;

public:

/**************************
PropertyTrees
***************************/
	PropertyTree get() const;

	void set(PropertyTree &);

	Slot * addInput(PropertyTree &);

	Slot * addOutput(PropertyTree &);

	virtual Interface::ComponentView * view() override = 0;

private:
	class Impl;
	UniquePtr<Impl> m;

};

}