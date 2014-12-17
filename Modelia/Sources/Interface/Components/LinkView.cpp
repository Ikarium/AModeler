#include "Conf.h"

#include "LinkView.h"

#include <QPen>
#include "Model/Model/Components/Link.h"

namespace Interface
{

LinkView::LinkView(Model::Link * _model, PropertyTree & ptree)
	: model_(_model)
{
	set(ptree);
	QPen pen;
	pen.setWidth(4);
	pen.setBrush(Qt::green);
	pen.setCapStyle(Qt::RoundCap);
	pen.setJoinStyle(Qt::RoundJoin);

	setPen(pen);

	setZValue(-10);

}
LinkView::~LinkView()
{

}

/**************************
PropertyTrees
***************************/
PropertyTree LinkView::get() const
{
	PropertyTree ptree;

	return ptree;
}

void LinkView::set(PropertyTree & ptree)
{
	if (ptree.get_value<std::string>() == "View")
	{

	}

}

Model::Link * LinkView::model()
{
	return model_;
}


void LinkView::paint(QPainter *painter, const QStyleOptionGraphicsItem * options, QWidget *)
{
	if (model_)
	{
		QPointF from = model_->slot1()->view()->linkConnectionPos();
		QPointF to = model_->slot2()->view()->linkConnectionPos();
		setLine(QLineF(from, to));
	}

	QGraphicsLineItem::paint(painter, options);
}

QRectF LinkView::boundingRect() const
{
	return QGraphicsLineItem::boundingRect();
}

}