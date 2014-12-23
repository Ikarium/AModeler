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
	PropertyTree get() const;
	void set(PropertyTree &);

	/**************************
	inputs accessors
	***************************/
	void addInput(PropertyTree &);
	void removeInput();


	/**************************
	outputs accessors
	***************************/
	void addOutput(PropertyTree &);
	void removeOutput();

	virtual Interface::VariableView * view() override;

private:
	class Impl;
	UniquePtr<Impl> m;
};
}
