#pragma once

#include "Interface/Components/VariableView.h"

#include "Component.h"

namespace Model
{

class Variable : public Component
{

public:
	Variable(Composition *, PropertyTree &);

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

	void removeInput();
	void removeOutput();

	QString initialValue();
	void setInitialValue(QString);

	virtual Interface::VariableView * view() override;

private:
	class Impl;
	UniquePtr<Impl> m;
};
}
