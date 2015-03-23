
#include<QDrag>
#include<QGraphicsSceneDragDropEvent>

#include "SceneManager.h"

#include <QGraphicsView>
#include <QLayout>
#include <QLabel>
#include <QAction>
#include <QSpinBox>
#include <QSpacerItem>

#include "Model/Model/Components/Variable.h"
#include "Model/Model/Components/Function.h"
#include "Model/Model/Components/Process.h"
#include "Model/Model/Components/Composition.h"
#include "Model/Model/Components/Component.h"
#include "Model/Model/Components/Link.h"

namespace Interface
{

class SceneManager::Impl
{
	SceneManager* owner;

public:

	QWidget* propertiesWidget = nullptr;

	QList<QString> currentPath;

	Model::Composition * currentComposition = nullptr;

	ComponentView * currentPropertiesWidgetComponent = nullptr;
	QToolBar * toolBar = nullptr;
	QToolBar * navBar = nullptr;
	QMap<QAction*, Model::Composition*> navActionsMap;

	bool linkDragMode = false;
	LinkView * tempLink = nullptr;
	SlotView * tempLinkSlot = nullptr;

	Impl(SceneManager* owner_, Model::Composition * composition) :
		owner(owner_), 
		currentComposition(composition)
	{

	}

	void setPropertyWidget(ComponentView *);
	void unsetPropertyWidget();

};

SceneManager::SceneManager(Model::Composition * composition)
	: m(new Impl(this, composition))
{
	update();

	connect(this, SIGNAL(focusItemChanged(QGraphicsItem *, QGraphicsItem *, Qt::FocusReason))
		  , this, SLOT(focusItemChanged(QGraphicsItem *, QGraphicsItem *, Qt::FocusReason)));

	
}

SceneManager::~SceneManager()
{

}

void SceneManager::init(QToolBar* toolBar, QToolBar* navBar, QWidget* propertiesWidget)
{
	m->toolBar = toolBar;
	m->navBar = navBar;
	m->propertiesWidget = propertiesWidget;
}

Model::Composition & SceneManager::currentComposition()
{
	return *m->currentComposition;
}

void SceneManager::setCurrentComposition(Model::Composition * composition)
{
	clean();

	m->currentComposition = composition;
	m->toolBar->clear();
	m->toolBar->addWidget(new QLabel("Iteration count: "));
	QSpinBox* iterCountSpinBox = new QSpinBox();
	iterCountSpinBox->setMinimum(1);
	iterCountSpinBox->setValue(m->currentComposition->iterationCount());
	connect(iterCountSpinBox, SIGNAL(valueChanged(int)), 
			m->currentComposition->view(), SLOT(iterationCountUpdate(int)));
	m->toolBar->addWidget(iterCountSpinBox);

	for (QAction * action : m->navBar->actions())
		m->navBar->removeAction(action);

	QList<Model::Composition *> path;
	for (Model::Composition * comp = composition;
		comp != nullptr;
		comp = comp->owner())
		path.push_front(comp);

	m->navActionsMap.clear();
	int i = 0;
	for (Model::Composition * elem : path)
	{
		if (i != 0) m->navBar->addWidget(new QLabel("<"));
		QAction * newAction = m->navBar->addAction(elem->name());
		connect(newAction, SIGNAL(triggered()), this, SLOT(navBarElemTriggered()));
		m->navActionsMap[newAction] = elem;
		i++;
	}

	m->currentPropertiesWidgetComponent = nullptr;

	if (m->propertiesWidget)
		focusItemChanged(nullptr, nullptr, Qt::FocusReason::OtherFocusReason);


	populate();

}

void SceneManager::clean()
{

	if (m->currentComposition)
	{
		for (auto component : m->currentComposition->components())
			removeItem(component->view());
		for (auto & link : m->currentComposition->links())
			removeItem(link.view());

		for (auto & slot : m->currentComposition->inputs())
			removeItem(slot.view());
		for (auto & slot : m->currentComposition->outputs())
			removeItem(slot.view());
	}

	QGraphicsScene::clear();

	m->currentComposition = nullptr;
}

void SceneManager::populate()
{
	if (m->currentComposition)
	{
		for (auto & component : m->currentComposition->components())
			addItem(component->view());
		for (auto & component : m->currentComposition->compositions())
			component.view()->updateSlots();
		for (auto & link : m->currentComposition->links())
			addItem(link.view());

		int i = 0;
		for (auto & slot : m->currentComposition->inputs())
		{
			slot.view()->setPos(QPointF(-500, i++ * 20));
			addItem(slot.view());
		}
		i = 0;
		for (auto & slot : m->currentComposition->outputs())
		{
			slot.view()->setPos(QPointF(500, i++ * 20));
			addItem(slot.view());
		}
	}

}

void SceneManager::navBarElemTriggered()
{
	Model::Composition * composition = m->navActionsMap[qobject_cast<QAction*>(sender())];

	setCurrentComposition(composition);
}

void SceneManager::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	if (event->mimeData()->hasFormat("application/modelia/library/component"))
		event->accept();
	else
		event->ignore();
}

void SceneManager::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
	if (event->mimeData()->hasFormat("application/modelia/library/component"))
	{
		event->setDropAction(Qt::MoveAction);
		event->accept();
	}
	else
		event->ignore();
}

void SceneManager::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
	event->ignore();
}

void SceneManager::dropEvent(QGraphicsSceneDragDropEvent *event)
{
	if (m->currentComposition 
		&& event->mimeData()->hasFormat("application/modelia/library/component"))
	{
		addElem(event->mimeData(), event->scenePos());

		event->accept();
	}
	else
		event->ignore();
}

void SceneManager::addElem(QMimeData const * mimeData, QPointF const & pos)
{
	QByteArray encodedData = mimeData->data("application/modelia/library/component");
	QDataStream stream(&encodedData, QIODevice::ReadOnly);
	QString elem;
	QString type;
	stream >> type;
	stream >> elem;
	PropertyTree ptree = loadPtree(elem);
	ptree.put_value<std::string>(type.toStdString());

	QString name = QString::fromStdString(ptree.get<std::string>("Name"));
	if (m->currentComposition->find(name))
	{
		int i = 2;
		while (m->currentComposition->find(name + QString::number(i)))
			i++;
		name = name + QString::number(i);
	}

	ptree.put("Name", name.toStdString());

	Model::Component * newComponent 
		= reinterpret_cast<Model::Component*>(m->currentComposition->addElement(ptree));
	newComponent->view()->setPos(QPoint(pos.x() - newComponent->view()->size().width() / 2,
										pos.y() - newComponent->view()->size().height() / 2));
	addItem(newComponent->view());
}

void SceneManager::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

	QGraphicsItem * item = itemAt(event->scenePos(), views()[0]->transform());
	SlotView * slot = dynamic_cast<SlotView*>(item);
	if (slot && (slot->model()->slotType() == Model::SlotType::output
		|| slot->model()->owner() == m->currentComposition))
	{
		m->tempLinkSlot = slot;
		m->linkDragMode = true;
		views().first()->setDragMode(QGraphicsView::NoDrag);
		m->tempLink = new LinkView(nullptr, PropertyTree());
		m->tempLink->setLine(QLineF(slot->linkConnectionPos(), slot->linkConnectionPos()));

		addItem(m->tempLink);
	}
	else
		QGraphicsScene::mousePressEvent(event);
}

void SceneManager::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem * item = itemAt(event->scenePos(), views()[0]->transform());
	CompositionView * composition = dynamic_cast<CompositionView*>(item);
	if (composition)
		setCurrentComposition(composition->model());
	else
		QGraphicsScene::mouseDoubleClickEvent(event);
}

void SceneManager::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (m->linkDragMode)
	{
		m->tempLink->setLine(QLineF(m->tempLink->line().p1(), event->scenePos()));
	}
	else 
		QGraphicsScene::mouseMoveEvent(event);
}

void SceneManager::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if (m->linkDragMode)
	{
		QList<QGraphicsItem *> allItems = items(event->scenePos());
		ComponentView * component;
		SlotView * slot;
		for (auto item : allItems)
		{
			component = dynamic_cast<ComponentView*>(item);
			slot = dynamic_cast<SlotView*>(item);
			if (slot
				&& m->tempLinkSlot->model()->owner() != m->currentComposition
				&& slot->model()->owner() == m->currentComposition) break;
			else if (component) break;
		}

		/*Detecting connection type*/

		/*If it is slot with component, check if we dont 
		link the parent output with some component*/
		if (component && 
			!(m->tempLinkSlot->model()->owner() == m->currentComposition &&
				m->tempLinkSlot->model()->slotType() == Model::SlotType::output)) 
				addLink(m->tempLinkSlot, component);
		/*Slot with slot, check we dont try to link output with parent input
		or input with parent output*/
		else if (slot && m->tempLinkSlot->model()->slotType() == slot->model()->slotType())
			addLink(m->tempLinkSlot, slot);
			
		removeItem(m->tempLink);
		m->tempLinkSlot = nullptr;
		m->tempLink = nullptr;
		m->linkDragMode = false;
		views().first()->setDragMode(QGraphicsView::RubberBandDrag);
	}
	else 
		QGraphicsScene::mouseReleaseEvent(event);
}


void SceneManager::addLink(SlotView * src, ComponentView * dest)
{

	bool go = false;
	Model::Slot * input = nullptr;
	if (dynamic_cast<VariableView*>(dest)
		&& !dynamic_cast<VariableView*>(src->model()->owner()->view())
		&& src->model()->owner() != dest->model())
	{
		input = &dest->model()->inputs().front();
		go = true;
	}
	else if (dynamic_cast<ProcessView*>(dest)
		&& src->model()->owner() != dest->model())
	{
		input = dynamic_cast<ProcessView*>(dest)->addNewInput();
		go = true;
	}

	if (go)
		addLink(src, input->view());
}

void SceneManager::addLink(SlotView * src, SlotView * dest)
{
	PropertyTree ptree;
	ptree.put_value("Link");
	if (src->model()->owner() == m->currentComposition)
	{
		ptree.put("Slot1.ComponentName", "__Parent");
		ptree.put("Slot1.SlotName", src->model()->name().toStdString());
	}
	else
	{
		ptree.put("Slot1.ComponentName", src->model()->owner()->name().toStdString());
		ptree.put("Slot1.SlotName", src->model()->name().toStdString());
	}
	if (dest->model()->owner() == m->currentComposition)
	{
		ptree.put("Slot2.ComponentName", "__Parent");
		ptree.put("Slot2.SlotName", dest->model()->name().toStdString());
	}
	else
	{
		ptree.put("Slot2.ComponentName", dest->model()->owner()->name().toStdString());
		ptree.put("Slot2.SlotName", dest->model()->name().toStdString());
	}
	Model::Link * link = m->currentComposition->addLink(ptree);
	addItem(link->view());
}


void SceneManager::focusItemChanged(QGraphicsItem * newFocusItem, 
									QGraphicsItem * oldFocusItem, 
									Qt::FocusReason reason)
{

	if (m->propertiesWidget && reason == Qt::MouseFocusReason)
	{
		ComponentView * newComponent = dynamic_cast<ComponentView*>(newFocusItem);
		ComponentView * oldComponent = dynamic_cast<ComponentView*>(oldFocusItem);

		if (m->currentPropertiesWidgetComponent != newFocusItem 
			&& newFocusItem && m->currentPropertiesWidgetComponent)
		{
			m->unsetPropertyWidget();
		}

		if (m->currentPropertiesWidgetComponent != newFocusItem 
			&& !m->currentPropertiesWidgetComponent)
		{
			m->setPropertyWidget(newComponent);
		}
	}
	else if (reason == Qt::FocusReason::OtherFocusReason)
	{
		m->unsetPropertyWidget();
	}
}

void SceneManager::Impl::setPropertyWidget(ComponentView * newComponent)
{
	if (newComponent)
	{
		currentPropertiesWidgetComponent = newComponent;
		propertiesWidget->setDisabled(false);
		currentPropertiesWidgetComponent->setPropertiesWidget(propertiesWidget);
		connect(newComponent, SIGNAL(updatePropertiesWidget()),
			owner, SLOT(updatePropertiesWidget()),
			Qt::UniqueConnection);
		connect(newComponent, SIGNAL(removePropertiesWidget()),
			owner, SLOT(removePropertiesWidget()),
			Qt::UniqueConnection);
		currentPropertiesWidgetComponent->fillPropertiesWidget();
		propertiesWidget->layout()->addItem(new QSpacerItem(0, 9999,
			QSizePolicy::Ignored, QSizePolicy::Maximum));
	}
}

void SceneManager::updatePropertiesWidget()
{
	ComponentView* tmp = m->currentPropertiesWidgetComponent;
	m->unsetPropertyWidget();
	m->setPropertyWidget(tmp);
}

void SceneManager::removePropertiesWidget()
{
	m->currentPropertiesWidgetComponent = nullptr;
	m->unsetPropertyWidget();
}

void SceneManager::Impl::unsetPropertyWidget()
{
	if (currentPropertiesWidgetComponent)
		currentPropertiesWidgetComponent->savePropertyWidget();
	propertiesWidget->setDisabled(true);
	if (propertiesWidget->layout())
		clearQLayout(propertiesWidget->layout());

	currentPropertiesWidgetComponent = nullptr;
}



}