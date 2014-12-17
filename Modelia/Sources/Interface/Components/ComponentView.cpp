#include "ComponentView.h"

#include "Model/Model/Components/Component.h"
#include "Model/Model/Components/Composition.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QGroupBox>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>

namespace Interface
{

void clearLayout(QLayout *layout)
{
	QLayoutItem *child;
	while ((child = layout->takeAt(0)) != 0) {
		if (child->layout() != 0)
			clearLayout(child->layout());
		else if (child->widget() != 0)
			delete child->widget();

		delete child;
	}
}

ComponentView::ComponentView(Model::Component * _model, PropertyTree & ptree) : model_(_model)
{
	set(ptree);

	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
	setAcceptHoverEvents(true);

	QGraphicsDropShadowEffect * shadow = new QGraphicsDropShadowEffect();
	shadow->setBlurRadius(10);
	shadow->setOffset(QPointF(0, 0));
	setGraphicsEffect(shadow);

}

ComponentView::~ComponentView()
{
	if (model_)
	{
		for (Model::Slot & input : model_->inputs())
		{
			input.setViewDeleted();
		}
		for (Model::Slot & output : model_->outputs())
		{
			output.setViewDeleted();
		}
	}
}

void ComponentView::init()
{
	for (Model::Slot & input : model_->inputs())
		input.view()->setParentItem(this);
	for (Model::Slot & output : model_->outputs())
		output.view()->setParentItem(this);

	updateSlots();
}

void ComponentView::updateSlots()
{
	for (Model::Slot & input : model_->inputs())
		input.view()->ownerChange();
	for (Model::Slot & output : model_->outputs())
		output.view()->ownerChange();
}


/**************************
PropertyTrees
***************************/
PropertyTree ComponentView::get() const
{
	PropertyTree ptree;

	ptree.put_value("View");
	ptree.put("Pos.x", pos().x());
	ptree.put("Pos.y", pos().y());
	ptree.put("Size.width", size_.width());
	ptree.put("Size.height", size_.height());

	return ptree;
}
void ComponentView::set(PropertyTree & ptree)
{
	setPos(QPoint(0, 0));
	size_ = QSize(100, 100);

	if (ptree.get_value<std::string>() == "View")
	{
		if (ptree.count("Pos") != 0)
			setPos(QPoint(ptree.get<float>("Pos.x"), ptree.get<float>("Pos.y")));
		if (ptree.count("Size") != 0)
			size_ = QSize(ptree.get<float>("Size.width"), ptree.get<float>("Size.height"));
	}

}

Model::Component * ComponentView::model()
{
	return model_;
}

QSizeF & ComponentView::size()
{
	return size_;
}

void ComponentView::setSize(QSizeF const & _size)
{
	size_ = _size;
}

QIcon ComponentView::getIcon()
{
	QIcon icon(getIllustration());

	return icon;
}

void ComponentView::paint(QPainter *painter, const QStyleOptionGraphicsItem *options, QWidget *widget)
{
	drawIllustration(painter, options);
	drawName(painter, options);
}

void ComponentView::drawName(QPainter *painter, const QStyleOptionGraphicsItem *options) const
{
	painter->drawText(QRectF(QPointF(0, 0), size_), Qt::AlignCenter, model_->name());
}

void ComponentView::drawIllustration(QPainter *painter, const QStyleOptionGraphicsItem *options) const
{
	QColor fillColor(0, 100, 0);
	if (options->state & QStyle::State_Selected)
		fillColor = QColor(100, 0, 0);
	if (options->state & QStyle::State_MouseOver)
		fillColor = QColor(50, 50, 0);
	if (options->state & QStyle::State_HasFocus)
		fillColor = QColor(200, 0, 0);
	QPen pen;
	pen.setWidth(2);
	QBrush b = painter->brush();
	painter->setBrush(fillColor);
	painter->drawRect(QRectF(QPointF(0, 0), size_));
}

QPixmap const ComponentView::getIllustration() const
{
	QPixmap pixmap(size_.toSize());

	QPainter painter(&pixmap);
	drawIllustration(&painter, &QStyleOptionGraphicsItem());
	return pixmap;
}

QRectF ComponentView::boundingRect() const
{
	return QRectF(size_.height() * -0.1, size_.width() * -0.1, size_.height() * 1.2, size_.width() * 1.2);
}


void ComponentView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mousePressEvent(event);
}

void ComponentView::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mouseDoubleClickEvent(event);
}

void ComponentView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mouseMoveEvent(event);
}

void ComponentView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mouseReleaseEvent(event);
}

QVariant ComponentView::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemPositionChange)
	{

	}

	return QGraphicsItem::itemChange(change, value);
}

void ComponentView::setPropertiesWidget(QWidget* propertiesWidget_)
{
	propertiesWidget = propertiesWidget_;
}

void ComponentView::fillPropertiesWidget()
{
	QGridLayout *mainLayout = new QGridLayout;
	propertiesWidget->setLayout(mainLayout);
	mainLayout->setSizeConstraint(QLayout::SetMaximumSize);

	QGroupBox * fields = new QGroupBox("Properties");
	QFormLayout *fieldsLayout = new QFormLayout();
	propertyWidgetElements.name = new QLineEdit(model_->name(), propertiesWidget);
	fieldsLayout->addRow("Name:", propertyWidgetElements.name);
	connect(propertyWidgetElements.name, SIGNAL(editingFinished()), this, SLOT(nameValidator()));
	QPushButton * deleteComponentBtn = new QPushButton("Delete component");
	deleteComponentBtn->setObjectName(model_->name());
	fieldsLayout->addRow("", deleteComponentBtn);
	connect(deleteComponentBtn, SIGNAL(clicked()), model_->owner()->view(), SLOT(deleteComponent()));
	fields->setLayout(fieldsLayout);
	mainLayout->addWidget(fields, 0, 0);

}

void ComponentView::nameValidator()
{
	QLineEdit * outputLineEdit = dynamic_cast<QLineEdit *>(sender());
	if (model_->owner()->find(outputLineEdit->text()))
		outputLineEdit->setText(model_->name());
	else
		model_->setName(outputLineEdit->text());
	update();
	
}

void ComponentView::deleteComponent()
{
	dynamic_cast<CompositionView*>(this)->model()->removeComponent(sender()->objectName()); //Hardcoooore
}

}

