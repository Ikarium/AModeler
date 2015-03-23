#include "PureFunctionView.h"

#include "Model/Model/Components/PureFunction.h"

#include "KExpandableWidget.h"

namespace Interface
{

class PureFunctionView::Impl
{
	PureFunctionView* w;

public:

	Model::PureFunction * model;

	Impl(PureFunctionView *, Model::PureFunction *);
	~Impl();

};

PureFunctionView::Impl::Impl(PureFunctionView* owner, Model::PureFunction * model_)
	: w(owner),
	model(model_)
{

}

PureFunctionView::Impl::~Impl()
{

}

PureFunctionView::PureFunctionView(Model::PureFunction * model, PropertyTree & ptree)
	: m(new Impl(this, model)), ProcessView(static_cast<Model::Process*>(model), ptree)
{
	import(ptree);
}

PureFunctionView::~PureFunctionView()
{

}

/**************************
PropertyTrees
***************************/
PropertyTree PureFunctionView::export() const
{
	PropertyTree ptree = ProcessView::export();

	return ptree;
}

void PureFunctionView::import(PropertyTree & ptree)
{
	if (ptree.get_value<std::string>() == "View")
	{

	}

}

Model::PureFunction * PureFunctionView::model()
{
	return m->model;
}

}