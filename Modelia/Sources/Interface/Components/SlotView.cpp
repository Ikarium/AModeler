#pragma once

#include "SlotView.h"
#include "Model/Model/Components/Slot.h"
#include "Model/Model/Components/Component.h"
#include "ComponentView.h"

#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <iostream>

namespace Interface
{

class SlotView::Impl
{
		SlotView* w;

public:

	Model::Slot * model;

	Impl(SlotView *, Model::Slot *);
	~Impl();

};

SlotView::Impl::Impl(SlotView* owner, Model::Slot * model_)
	: w(owner),
	model(model_)
{

}

SlotView::Impl::~Impl()
{

}

SlotView::SlotView(Model::Slot * model, PropertyTree & ptree)
	: m(new Impl(this, model))
{
	set(ptree);

	setAcceptHoverEvents(true);
}

SlotView::~SlotView()
{
	m->model->owner()->view()->update();
}

Model::Slot * SlotView::model()
{
	return m->model;
}

/**************************
PropertyTrees
***************************/
PropertyTree SlotView::get() const
{
	PropertyTree ptree;

	return ptree;
}

void SlotView::set(PropertyTree & ptree)
{
	if (ptree.get_value<std::string>() == "View")
	{

	}

}

void SlotView::ownerChange()
{
	ComponentView * componentView = m->model->owner()->view();
	setParentItem(componentView);
	setToolTip(m->model->name());

	int slotCount;
	if (m->model->slotType() == Model::SlotType::input)
		slotCount = m->model->owner()->inputs().size();
	else
		slotCount = m->model->owner()->outputs().size();
	int slotPos = m->model->pos();

	int vPos = componentView->size().height() / (slotCount + 1) * (slotPos + 1) - 10;
	if (m->model->slotType() == Model::SlotType::input)
		setPos(QPointF(-8, vPos));
	else
		setPos(QPointF(componentView->size().width() - 8, vPos));
}

void SlotView::paint(QPainter *painter, const QStyleOptionGraphicsItem * options, QWidget *)
{
	QPen pen;
	pen.setWidth(1);
	QBrush b = painter->brush();
	QVector<QPointF> arrowPoints;
	arrowPoints << 
		QPointF(0, 0)	<< 
		QPointF(20, 10) <<
		QPointF(0, 20)	<<
		QPointF(8, 10);

	QColor fillColor = QColor(115, 205, 175);

	if (options->state & QStyle::State_MouseOver)
		fillColor = fillColor.lighter(140);

	static const QPolygonF arrow = QPolygonF(arrowPoints);
	painter->setBrush(fillColor);
	painter->drawPolygon(arrow);
}

QRectF SlotView::boundingRect() const
{
	return QRectF(QPointF(0, 0), QSizeF(20, 20));
}
QPointF SlotView::linkConnectionPos() const
{
	if (m->model->slotType() == Model::SlotType::input)
		return QPointF(scenePos().x() + 8, scenePos().y() + 10);
	else
		return QPointF(scenePos().x() + 15, scenePos().y() + 10);
}

}