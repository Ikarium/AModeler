#pragma once

#include "ProcessView.h"

namespace Interface
{

class PureFunctionView : public ProcessView
{

public:

	PureFunctionView(Model::PureFunction *, PropertyTree &);
	~PureFunctionView();

	/**************************
	PropertyTrees
	***************************/
	PropertyTree get() const;

	void set(PropertyTree &);

	Model::PureFunction * model();

private:
	class Impl;
	UniquePtr<Impl> m;

};

}