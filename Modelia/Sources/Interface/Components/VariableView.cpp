#include "VariableView.h"

#include "Model/Model/Components/Variable.h"

namespace Interface
{

VariableView::VariableView(Model::Variable * _model, PropertyTree & ptree)
	: ComponentView(dynamic_cast<Model::Component*>(_model), ptree),
	model_(_model)
{
	set(ptree);
}

PropertyTree VariableView::get() const
{
	PropertyTree ptree = ComponentView::get();

	return ptree;
}

void VariableView::set(PropertyTree & ptree)
{
	if (ptree.get_value<std::string>() == "View")
	{

	}
}

Model::Variable * VariableView::model()
{
	return model_;
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
	QLinearGradient linearGrad(QPointF(0, 0), QPointF(0, size_.width()));
	linearGrad.setColorAt(0, fillColor.lighter(120));
	linearGrad.setColorAt(1, fillColor);
	painter->setBrush(linearGrad);
	painter->drawEllipse(QRectF(QPointF(0, 0), size_));
}

}