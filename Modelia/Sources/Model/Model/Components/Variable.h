#pragma once

#include "Interface/Components/VariableView.h"

#include "Component.h"

namespace Model
{

class Variable : public Component
{
	Type type_;

	Interface::VariableView view_;

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

};
}
