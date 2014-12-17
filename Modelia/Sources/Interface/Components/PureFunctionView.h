#pragma once

#include "ProcessView.h"

namespace Interface
{

class PureFunctionView : public ProcessView
{

	Model::PureFunction * model_;

public:

	PureFunctionView(Model::PureFunction *, PropertyTree &);

	/**************************
	PropertyTrees
	***************************/
	PropertyTree get() const;

	void set(PropertyTree &);

	Model::PureFunction * model();
};

}