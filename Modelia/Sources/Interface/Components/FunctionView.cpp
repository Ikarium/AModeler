#include "FunctionView.h"

#include "Model/Model/Components/Function.h"

#include "ui_FunctionProperties.h"

#include "KExpandableWidget.h"

#include <QFormLayout>
#include <QPlainTextEdit>
#include <QGridLayout>
#include <QLineEdit>
#include <QGroupBox>
#include <QPushButton>
#include <QTextEdit>

namespace Interface
{

class FunctionView::Impl
{
	FunctionView* w;

public:

	Model::Function * model;
		
	KExpandableWidget* functionPropertiesWidget = nullptr;
	bool functionPropertiesWidgetOpen = true;

	Impl(FunctionView *, Model::Function *);
	~Impl();

};

FunctionView::Impl::Impl(FunctionView* owner, Model::Function * model_)
	: w(owner),
	model(model_)
{

}

FunctionView::Impl::~Impl()
{

}

FunctionView::FunctionView(Model::Function * model, PropertyTree & ptree)
	: m(new Impl(this, model)), ProcessView(static_cast<Model::Process*>(model), ptree)
{
	import(ptree);
}

FunctionView::~FunctionView()
{

}

/**************************
PropertyTrees
***************************/
PropertyTree FunctionView::export() const
{
	PropertyTree ptree = ProcessView::export();

	return ptree;
}

void FunctionView::import(PropertyTree & ptree)
{
	if (ptree.get_value<std::string>() == "View")
	{

	}

}

Model::Function * FunctionView::model()
{
	return m->model;
}

void FunctionView::fillPropertiesWidget()
{
	ProcessView::fillPropertiesWidget();

	Ui::FunctionProperties ui;
	m->functionPropertiesWidget = new KExpandableWidget("Function", m->functionPropertiesWidgetOpen);
	ui.setupUi(m->functionPropertiesWidget->content());

	ui.code->setPlainText(m->model->code());
	connect(ui.code, SIGNAL(textChanged()), this, SLOT(codeEdited()));

	propertiesWidget()->layout()->addWidget(m->functionPropertiesWidget);
}

void FunctionView::savePropertyWidget()
{
	ProcessView::savePropertyWidget();

	if (m->functionPropertiesWidget)
		m->functionPropertiesWidgetOpen = m->functionPropertiesWidget->open();
	m->functionPropertiesWidget = nullptr;
}

void FunctionView::codeEdited()
{
	QPlainTextEdit * newCode = dynamic_cast<QPlainTextEdit *>(sender());

	m->model->setCode(newCode->toPlainText());
}

}