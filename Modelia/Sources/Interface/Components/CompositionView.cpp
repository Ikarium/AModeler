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

CompositionView::CompositionView(Model::Composition * _model, PropertyTree & ptree) 
	: ProcessView(static_cast<Model::Process*>(_model), ptree),
	model_(_model)
{
	set(ptree);
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
	return model_;
}

void CompositionView::fillPropertiesWidget()
{
	ProcessView::fillPropertiesWidget();

	QPushButton * addToLibrary = new QPushButton("Add to Library");
	addToLibrary->setObjectName(savePtree(model()->get()));
	connect(addToLibrary, SIGNAL(clicked()), scene()->views().first()->window(), SLOT(addToLibrary()));
	dynamic_cast<QGridLayout*>(propertiesWidget->layout())->addWidget(addToLibrary, 1, 0, 2, 1);
}

void CompositionView::iterationCountUpdate(int iterationCount)
{
	model_->setIterationCount(iterationCount);
}

}