#include "PureFunctionView.h"

#include "Model/Model/Components/PureFunction.h"

namespace Interface
{

PureFunctionView::PureFunctionView(Model::PureFunction * _model, PropertyTree & ptree)
	: ProcessView(static_cast<Model::Process*>(_model), ptree),
	model_(_model)
{
	set(ptree);
}

/**************************
PropertyTrees
***************************/
PropertyTree PureFunctionView::get() const
{
	PropertyTree ptree = ProcessView::get();

	return ptree;
}

void PureFunctionView::set(PropertyTree & ptree)
{
	if (ptree.get_value<std::string>() == "View")
	{

	}

}

Model::PureFunction * PureFunctionView::model()
{
	return model_;
}

}