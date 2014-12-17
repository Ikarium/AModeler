#pragma once

#include "Conf.h"
#include <QGraphicsItem>

namespace Interface
{

class SlotView : public QObject, public QGraphicsItem
{
	Q_OBJECT

	Model::Slot * model_;

public:

	SlotView(Model::Slot *, PropertyTree &);
	~SlotView();

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
	virtual QRectF boundingRect() const override;

/**************************
PropertyTrees
***************************/
	PropertyTree get() const;
	
	void set(PropertyTree &);

	void ownerChange();
	QPointF linkConnectionPos() const;

	Model::Slot * model();

};

}