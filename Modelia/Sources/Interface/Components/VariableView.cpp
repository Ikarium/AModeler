#include "VariableView.h"

#include "Model/Model/Components/Variable.h"

namespace Interface
{

	class VariableView::Impl
{
	VariableView* w;

public:

	Model::Variable * model;

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
	set(ptree);
}

VariableView::~VariableView()
{

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
	return m->model;
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