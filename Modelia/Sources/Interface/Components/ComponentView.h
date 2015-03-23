#pragma once

#include "Conf.h"

#include <QScrollArea>
#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <QIcon>
#include <QPainter>
#include <QToolBox>

namespace Interface
{

class ComponentView : public QObject, public QGraphicsItem
{
	Q_OBJECT

public:

	ComponentView(Model::Component *, PropertyTree &);
	~ComponentView();

	virtual void init();
	virtual void updateSlots();

/**************************
PropertyTrees
***************************/
	PropertyTree export() const;
	void import(PropertyTree & ptree);

	Model::Component * model();
	QSizeF const & size() const;
	void setSize(QSizeF const &);

	void setPropertiesWidget(QWidget*);
	QWidget* propertiesWidget() const;
	virtual void fillPropertiesWidget();
	virtual void savePropertyWidget();

	QIcon getIcon();

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
	virtual QPixmap const getIllustration() const;
	virtual QRectF boundingRect() const override;

protected:
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *) override; 
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *) override;
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *) override;
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;
	virtual QVariant itemChange(GraphicsItemChange, const QVariant &) override;

	virtual void drawIllustration(QPainter *, const QStyleOptionGraphicsItem *) const;
	virtual void drawName(QPainter *, const QStyleOptionGraphicsItem *) const;

protected slots:
	void nameValidator();
	void deleteComponent();

signals:
	void updatePropertiesWidget();
	void removePropertiesWidget();

private:
	class Impl;
	UniquePtr<Impl> m;
};

}


