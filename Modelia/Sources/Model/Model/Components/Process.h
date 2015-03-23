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
	PropertyTree export() const;
	void import(PropertyTree & ptree);

/**************************
Slot accessors
***************************/
	virtual Slot * addInput(PropertyTree &) override;
	virtual Slot * addOutput(PropertyTree &) override;

/**************************
Accessors
***************************/
	virtual Interface::ComponentView * view() override = 0;

private:
	class Impl;
	UniquePtr<Impl> m;

};

}