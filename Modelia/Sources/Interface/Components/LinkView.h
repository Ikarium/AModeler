#pragma once

#include "Conf.h"

#include <QGraphicsItem>

namespace Interface
{

class LinkView : public QObject, public QGraphicsLineItem
{
	Q_OBJECT

public:

	LinkView(Model::Link *, PropertyTree &);
	~LinkView();

/**************************
PropertyTrees
***************************/
	PropertyTree export() const;
	void import(PropertyTree & ptree);

	Model::Link * model();

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
	virtual QRectF boundingRect() const override;

private:
	class Impl;
	UniquePtr<Impl> m;

};

}