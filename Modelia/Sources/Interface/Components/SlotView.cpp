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

SlotView::SlotView(Model::Slot * _model, PropertyTree & ptree) 
	: model_(_model)
{
	set(ptree);

	setAcceptHoverEvents(true);
}

SlotView::~SlotView()
{
		model_->owner()->view()->update();
}

Model::Slot * SlotView::model()
{
	return model_;
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
	ComponentView * componentView = model_->owner()->view();
	setParentItem(componentView);
	setToolTip(model_->name());

	int slotCount;
	if (model_->slotType() == Model::SlotType::input)
		slotCount = model_->owner()->inputs().size();
	else
		slotCount = model_->owner()->outputs().size();
	int slotPos = model_->pos();

	int vPos = componentView->size().height() / (slotCount + 1) * (slotPos + 1) - 10;
	if (model_->slotType() == Model::SlotType::input)
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
	if (model_->slotType() == Model::SlotType::input)
		return QPointF(scenePos().x() + 8, scenePos().y() + 10);
	else
		return QPointF(scenePos().x() + 15, scenePos().y() + 10);
}

}