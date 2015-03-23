#include "Conf.h"

#include "LinkView.h"

#include <QPen>
#include "Model/Model/Components/Link.h"

namespace Interface
{

class LinkView::Impl
{

	LinkView* w;

public:

	Model::Link * model;

	Impl(LinkView *, Model::Link *);
	~Impl();

};

LinkView::Impl::Impl(LinkView* owner, Model::Link * model_)
	: w(owner),
	model(model_)
{

}

LinkView::Impl::~Impl()
{

}

LinkView::LinkView(Model::Link * model, PropertyTree & ptree)
	: m(new Impl(this, model))
{
	import(ptree);
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
PropertyTree LinkView::export() const
{
	PropertyTree ptree;

	return ptree;
}

void LinkView::import(PropertyTree & ptree)
{
	if (ptree.get_value<std::string>() == "View")
	{

	}

}

Model::Link * LinkView::model()
{
	return m->model;
}


void LinkView::paint(QPainter *painter, const QStyleOptionGraphicsItem * options, QWidget *)
{
	if (m->model)
	{
		QPointF from = m->model->slot1()->view()->linkConnectionPos();
		QPointF to = m->model->slot2()->view()->linkConnectionPos();
		setLine(QLineF(from, to));
	}

	QGraphicsLineItem::paint(painter, options);
}

QRectF LinkView::boundingRect() const
{
	return QGraphicsLineItem::boundingRect();
}

}