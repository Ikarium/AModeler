#include "Conf.h"

#include "CompositionView.h"

#include "ui_CompositionProperties.h"

#include "KExpandableWidget.h"

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

	KExpandableWidget* compositionPropertiesWidget = nullptr;
	bool compositionPropertiesWidgetOpen = true;

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
	import(ptree);
}

CompositionView::~CompositionView()
{

}

/**************************
PropertyTrees
***************************/
PropertyTree CompositionView::export() const
{
	PropertyTree ptree = ProcessView::export();

	return ptree;
}

void CompositionView::import(PropertyTree & ptree)
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

	Ui::CompositionProperties ui;
	m->compositionPropertiesWidget = new KExpandableWidget("Composition", m->compositionPropertiesWidgetOpen);
	ui.setupUi(m->compositionPropertiesWidget->content());

	ui.addToLibrary->setObjectName(savePtree(model()->export()));
	connect(ui.addToLibrary, SIGNAL(clicked()), scene()->views().first()->window(), SLOT(addToLibrary()));

	propertiesWidget()->layout()->addWidget(m->compositionPropertiesWidget);
}

void CompositionView::savePropertyWidget()
{
	ProcessView::savePropertyWidget();

	if (m->compositionPropertiesWidget)
		m->compositionPropertiesWidgetOpen = m->compositionPropertiesWidget->open();
	m->compositionPropertiesWidget = nullptr;
}


void CompositionView::iterationCountUpdate(int iterationCount)
{
	m->model->setIterationCount(iterationCount);
}

}