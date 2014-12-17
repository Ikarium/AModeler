#pragma once

#include "Conf.h"

#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <QIcon>
#include <QPainter>

namespace Interface
{

void clearLayout(QLayout *layout);

class ComponentView : public QObject, public QGraphicsItem
{
	Q_OBJECT

protected:

	QSizeF size_;

	Model::Component * model_;

	QWidget* propertiesWidget = nullptr;

public:

	ComponentView(Model::Component *, PropertyTree &);
	~ComponentView();
	virtual void init();
	virtual void updateSlots();

/**************************
PropertyTrees
***************************/
	PropertyTree get() const;

	void set(PropertyTree &);

	Model::Component * model();
	QSizeF & size();
	void setSize(QSizeF const &);

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

	struct PropertyWidgetElements
	{
		QLineEdit* name;
	};
	PropertyWidgetElements propertyWidgetElements;


public:
	void setPropertiesWidget(QWidget*);
	virtual void fillPropertiesWidget();

protected slots:
	void nameValidator();
	void deleteComponent();
};

}


