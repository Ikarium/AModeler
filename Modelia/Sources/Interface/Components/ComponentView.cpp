#include "ComponentView.h"

#include "ui_ComponentProperties.h"

#include "KExpandableWidget.h"

#include "Model/Model/Components/Component.h"
#include "Model/Model/Components/Composition.h"

#include <QFormLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QGroupBox>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>

namespace Interface
{

class ComponentView::Impl
{
	ComponentView* w;

public:

	Model::Component * model;

	QSizeF size;

	QWidget* propertiesWidget = nullptr;
	KExpandableWidget* componentPropertiesWidget = nullptr;
	bool componentPropertiesWidgetOpen = true;

	Impl(ComponentView *, Model::Component *);
	~Impl();

};

ComponentView::Impl::Impl(ComponentView* owner, Model::Component * model_)
	: w(owner),
	model(model_)
{

}

ComponentView::Impl::~Impl()
{

}

ComponentView::ComponentView(Model::Component * model, PropertyTree & ptree)
	: m(new Impl(this, model))
{
	import(ptree);

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
	removePropertiesWidget();

	if (m->model)
	{
		for (Model::Slot & input : m->model->inputs())
		{
			input.setViewDeleted();
		}
		for (Model::Slot & output : m->model->outputs())
		{
			output.setViewDeleted();
		}
	}
}

void ComponentView::init()
{
	for (Model::Slot & input : m->model->inputs())
		input.view()->setParentItem(this);
	for (Model::Slot & output : m->model->outputs())
		output.view()->setParentItem(this);

	updateSlots();
}

void ComponentView::updateSlots()
{
	for (Model::Slot & input : m->model->inputs())
		input.view()->update();
	for (Model::Slot & output : m->model->outputs())
		output.view()->update();
}


/**************************
PropertyTrees
***************************/
PropertyTree ComponentView::export() const
{
	PropertyTree ptree;

	ptree.put_value("View");
	ptree.put("Pos.x", pos().x());
	ptree.put("Pos.y", pos().y());
	ptree.put("Size.width", m->size.width());
	ptree.put("Size.height", m->size.height());

	return ptree;
}
void ComponentView::import(PropertyTree & ptree)
{
	setPos(QPoint(0, 0));
	m->size = QSize(100, 100);

	if (ptree.get_value<std::string>() == "View")
	{
		if (ptree.count("Pos") != 0)
			setPos(QPoint(ptree.get<float>("Pos.x"), ptree.get<float>("Pos.y")));
		if (ptree.count("Size") != 0)
			m->size = QSize(ptree.get<float>("Size.width"), ptree.get<float>("Size.height"));
	}

}

Model::Component * ComponentView::model() { return m->model; }
QSizeF const & ComponentView::size() const { return m->size; }
void ComponentView::setSize(QSizeF const & _size) { m->size = _size; }
QIcon ComponentView::getIcon() { return getIllustration(); }

void ComponentView::paint(QPainter *painter, const QStyleOptionGraphicsItem *options, QWidget *widget)
{
	drawIllustration(painter, options);
	drawName(painter, options);
}

void ComponentView::drawName(QPainter *painter, const QStyleOptionGraphicsItem *options) const
{
	painter->drawText(QRectF(QPointF(0, 0), m->size), Qt::AlignCenter, m->model->name());
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
	painter->drawRect(QRectF(QPointF(0, 0), m->size));
}

QPixmap const ComponentView::getIllustration() const
{
	QPixmap pixmap(m->size.toSize());

	QPainter painter(&pixmap);
	drawIllustration(&painter, &QStyleOptionGraphicsItem());
	return pixmap;
}

QRectF ComponentView::boundingRect() const
{
	return QRectF(m->size.height() * -0.1, m->size.width() * -0.1, m->size.height() * 1.2, m->size.width() * 1.2);
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

QWidget* ComponentView::propertiesWidget() const
{ 
	return m->propertiesWidget; 
}

void ComponentView::setPropertiesWidget(QWidget* propertiesWidget_)
{
	m->propertiesWidget = propertiesWidget_;
}

void ComponentView::fillPropertiesWidget()
{
	m->componentPropertiesWidget = new KExpandableWidget("Component", m->componentPropertiesWidgetOpen);
	Ui::ComponentProperties ui;
	ui.setupUi(m->componentPropertiesWidget->content());

	ui.name->setText(m->model->name());
	ui.deleteComponent->setObjectName(m->model->name());

	connect(ui.name, SIGNAL(editingFinished()), this, SLOT(nameValidator()));
	connect(ui.deleteComponent, SIGNAL(clicked()), m->model->owner()->view(), SLOT(deleteComponent()));

	m->propertiesWidget->layout()->addWidget(m->componentPropertiesWidget);
}

void ComponentView::savePropertyWidget()
{
	if (m->componentPropertiesWidget)
		m->componentPropertiesWidgetOpen = m->componentPropertiesWidget->open();
	m->componentPropertiesWidget = nullptr;
}

void ComponentView::nameValidator()
{
	QLineEdit * outputLineEdit = dynamic_cast<QLineEdit *>(sender());
	if (m->model->owner()->find(outputLineEdit->text()))
		outputLineEdit->setText(m->model->name());
	else
		m->model->setName(outputLineEdit->text());
	update();
	
}

void ComponentView::deleteComponent()
{
	dynamic_cast<CompositionView*>(this)->model()->removeComponent(sender()->objectName()); //Hardcoooore
}

}

