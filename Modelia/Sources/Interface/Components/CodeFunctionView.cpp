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

class CodeFunctionView::Impl
{
	CodeFunctionView* w;

public:

	Model::CodeFunction * model;

	Impl(CodeFunctionView *, Model::CodeFunction *);
	~Impl();

};

CodeFunctionView::Impl::Impl(CodeFunctionView* owner, Model::CodeFunction * model_)
	: w(owner),
	model(model_)
{

}

CodeFunctionView::Impl::~Impl()
{

}

CodeFunctionView::CodeFunctionView(Model::CodeFunction * model, PropertyTree & ptree)
	: m(new Impl(this, model)), ProcessView(static_cast<Model::Process*>(model), ptree)
{
	set(ptree);
}

CodeFunctionView::~CodeFunctionView()
{

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
	return m->model;
}

void CodeFunctionView::fillPropertiesWidget()
{
	ProcessView::fillPropertiesWidget();

	QGroupBox * code = new QGroupBox("Code");
	QGridLayout *codeLayout = new QGridLayout();
	code->setLayout(codeLayout);
	QTextEdit* codeText = new QTextEdit();
	codeText->setPlainText(m->model->code());
	codeText->resize(QSize(200, 50));
	codeLayout->addWidget(codeText);
	connect(codeText, SIGNAL(textChanged()), this, SLOT(codeEdited()));

	dynamic_cast<QGridLayout*>(propertiesWidget()->layout())->addWidget(code, 1, 0, 2, 1);
}

void CodeFunctionView::codeEdited()
{
	QTextEdit * newCode = dynamic_cast<QTextEdit *>(sender());

	m->model->setCode(newCode->toPlainText());
}

}