#include "VariableView.h"

#include "Model/Model/Components/Variable.h"
#include <QLineEdit>

#include "ui_VariableProperties.h"

#include "KExpandableWidget.h"

namespace Interface
{

	class VariableView::Impl
{
	VariableView* w;

public:

	Model::Variable * model;

	KExpandableWidget* variablePropertiesWidget = nullptr;
	bool variablePropertiesWidgetOpen = true;

	Impl(VariableView *, Model::Variable *);
	~Impl();

};

VariableView::Impl::Impl(VariableView* owner, Model::Variable * model_)
	: w(owner),
	model(model_)
{

}

VariableView::Impl::~Impl()
{

}

VariableView::VariableView(Model::Variable * model, PropertyTree & ptree)
	: m(new Impl(this, model)), ComponentView(static_cast<Model::Component*>(model), ptree)
{
	import(ptree);
}

VariableView::~VariableView()
{

}

PropertyTree VariableView::export() const
{
	PropertyTree ptree = ComponentView::export();

	return ptree;
}

void VariableView::import(PropertyTree & ptree)
{
	if (ptree.get_value<std::string>() == "View")
	{

	}
}

Model::Variable * VariableView::model()
{
	return m->model;
}

void VariableView::fillPropertiesWidget()
{
	ComponentView::fillPropertiesWidget();

	Ui::VariableProperties ui;
	m->variablePropertiesWidget = new KExpandableWidget("Variable", m->variablePropertiesWidgetOpen);
	ui.setupUi(m->variablePropertiesWidget->content());
	propertiesWidget()->layout()->addWidget(m->variablePropertiesWidget);

	ui.initialValue->setText(m->model->initialValue());
	connect(ui.initialValue, SIGNAL(editingFinished()), this, SLOT(updateInitialValue()));
}

void VariableView::savePropertyWidget()
{
	ComponentView::savePropertyWidget();

	if (m->variablePropertiesWidget)
		m->variablePropertiesWidgetOpen = m->variablePropertiesWidget->open();
	m->variablePropertiesWidget = nullptr;
}

void VariableView::setInitialValue(QString newValue)
{
	m->model->setInitialValue(newValue);
}
void VariableView::updateInitialValue()
{
	setInitialValue(dynamic_cast<QLineEdit *>(sender())->text());
}

void VariableView::paint(QPainter *painter, const QStyleOptionGraphicsItem *options, QWidget *widget)
{
	drawIllustration(painter, options);
	drawName(painter, options);
}

void VariableView::drawIllustration(QPainter *painter, const QStyleOptionGraphicsItem *options) const
{
	QColor fillColor(95, 165, 170);
	if (options->state & QStyle::State_HasFocus)
		fillColor = fillColor.lighter(140);
	else if (options->state & QStyle::State_Selected)
		fillColor = fillColor.lighter(130);
	else if (options->state & QStyle::State_MouseOver)
		fillColor = fillColor.lighter(120);

	QPen pen;
	pen.setWidth(2);
	QLinearGradient linearGrad(QPointF(0, 0), QPointF(0, size().width()));
	linearGrad.setColorAt(0, fillColor.lighter(120));
	linearGrad.setColorAt(1, fillColor);
	painter->setBrush(linearGrad);
	painter->drawEllipse(QRectF(QPointF(0, 0), size()));
}

}