#pragma once

#include "ProcessView.h"

namespace Interface
{

class PureFunctionView : public ProcessView
{

public:

	FunctionView(Model::PureFunction *, PropertyTree &);
	~FunctionView();

	/**************************
	PropertyTrees
	***************************/
	PropertyTree export() const;
	void import(PropertyTree & ptree);

	Model::Function * model();

private:
	class Impl;
	UniquePtr<Impl> m;

};

}