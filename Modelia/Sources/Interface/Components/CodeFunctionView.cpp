#include "CodeFunctionView.h"

#include "Model/Model/Components/CodeFunction.h"

#include <QFormLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QGroupBox>
#include <QPushButton>
#include <QTextEdit>

namespace Interface
{

CodeFunctionView::CodeFunctionView(Model::CodeFunction * _model, PropertyTree & ptree) 
	: ProcessView(static_cast<Model::Process*>(_model), ptree),
	model_(_model)
{
	set(ptree);
}

/**************************
PropertyTrees
***************************/
PropertyTree CodeFunctionView::get() const
{
	PropertyTree ptree = ProcessView::get();

	return ptree;
}

void CodeFunctionView::set(PropertyTree & ptree)
{
	if (ptree.get_value<std::string>() == "View")
	{

	}

}

Model::CodeFunction * CodeFunctionView::model()
{
	return model_;
}

void CodeFunctionView::fillPropertiesWidget()
{
	ProcessView::fillPropertiesWidget();

	QGroupBox * code = new QGroupBox("Code");
	QGridLayout *codeLayout = new QGridLayout();
	code->setLayout(codeLayout);
	QTextEdit* codeText = new QTextEdit();
	codeText->setPlainText(model_->code());
	codeText->resize(QSize(200, 50));
	codeLayout->addWidget(codeText);
	connect(codeText, SIGNAL(textChanged()), this, SLOT(codeEdited()));

	dynamic_cast<QGridLayout*>(propertiesWidget->layout())->addWidget(code, 1, 0, 2, 1);
}

void CodeFunctionView::codeEdited()
{
	QTextEdit * newCode = dynamic_cast<QTextEdit *>(sender());

	model_->setCode(newCode->toPlainText());
}

}