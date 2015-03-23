#pragma once

#include "Conf.h"
#include <QGraphicsItem>

namespace Interface
{

class SlotView : public QObject, public QGraphicsItem
{
	Q_OBJECT

public:

	SlotView(Model::Slot *, PropertyTree &);
	~SlotView();

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
	virtual QRectF boundingRect() const override;

/**************************
PropertyTrees
***************************/
	PropertyTree export() const;
	void import(PropertyTree & ptree);

	void update();
	QPointF linkConnectionPos() const;

	Model::Slot * model();

private:
	class Impl;
	UniquePtr<Impl> m;

};

}