#include "Conf.h"

#include "CompositionView.h"

#include "Model/Model/Components/Composition.h"

#include <QFormLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QGroupBox>
#include <QPushButton>
#include <QTextEdit>

#include <QGraphicsView>

namespace Interface
{

class CompositionView::Impl
{

	CompositionView* w;

public:

	Model::Composition * model;

	Impl(CompositionView *, Model::Composition *);
	~Impl();

};

CompositionView::Impl::Impl(CompositionView* owner, Model::Composition * model_)
	: w(owner),
	model(model_)
{

}

CompositionView::Impl::~Impl()
{

}

CompositionView::CompositionView(Model::Composition * model, PropertyTree & ptree)
	: m(new Impl(this, model)), ProcessView(static_cast<Model::Process*>(model), ptree)
{
	set(ptree);
}

CompositionView::~CompositionView()
{

}

/**************************
PropertyTrees
***************************/
PropertyTree CompositionView::get() const
{
	PropertyTree ptree = ProcessView::get();

	return ptree;
}

void CompositionView::set(PropertyTree & ptree)
{
	if (ptree.get_value<std::string>() == "View")
	{

	}

}

Model::Composition * CompositionView::model()
{
	return m->model;
}

void CompositionView::fillPropertiesWidget()
{
	ProcessView::fillPropertiesWidget();

	QPushButton * addToLibrary = new QPushButton("Add to Library");
	addToLibrary->setObjectName(savePtree(model()->get()));
	connect(addToLibrary, SIGNAL(clicked()), scene()->views().first()->window(), SLOT(addToLibrary()));
	dynamic_cast<QGridLayout*>(propertiesWidget()->layout())->addWidget(addToLibrary, 1, 0, 2, 1);
}

void CompositionView::iterationCountUpdate(int iterationCount)
{
	m->model->setIterationCount(iterationCount);
}

}