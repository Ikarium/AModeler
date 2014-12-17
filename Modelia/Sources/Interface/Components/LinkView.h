#pragma once

#include "Conf.h"

#include <QGraphicsItem>

namespace Interface
{

class LinkView : public QObject, public QGraphicsLineItem
{
	Q_OBJECT

	Model::Link * model_;

public:

	LinkView(Model::Link *, PropertyTree &);
	~LinkView();

/**************************
PropertyTrees
***************************/
	PropertyTree get() const;
	
	void set(PropertyTree &);

	Model::Link * model();

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
	virtual QRectF boundingRect() const override;

};

}