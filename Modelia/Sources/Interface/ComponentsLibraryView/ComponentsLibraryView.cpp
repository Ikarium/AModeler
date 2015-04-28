#include "ComponentsLibraryView.h"

#include<QDropEvent>
#include<QDrag>
#include<QMimeData>

#include "Model/ComponentsLibrary/ComponentsLibrary.h"

namespace Interface
{

ComponentsLibraryView::ComponentsLibraryView(QWidget * parent) : QTreeView(parent)
{
	setSelectionMode(QAbstractItemView::SingleSelection);
	setDragEnabled(true);
	setAcceptDrops(false);
	setDropIndicatorShown(true);
	setDragDropMode(QAbstractItemView::DragDrop);

	App::componentsLibraryView = this;
}


ComponentsLibraryView::~ComponentsLibraryView()
{

}

void ComponentsLibraryView::init(Model::ComponentsLibraryItem* item)
{
	if (!item) item = App::componentsLibrary->root();
	if (item->childrenCount() != 0)
	{
		setExpanded(App::componentsLibrary->index(item), item->expanded());
		for (Model::ComponentsLibraryItem* child : item->children())
			init(child);
	}
}

void ComponentsLibraryView::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat("application/modelia/component"))
		event->accept();
	else
		event->ignore();
}

void ComponentsLibraryView::dragMoveEvent(QDragMoveEvent *event)
{
	if (event->mimeData()->hasFormat("application/modelia/component")) {
		event->setDropAction(Qt::CopyAction);
		event->accept();
	}
	else {
		event->ignore();
	}
}

void ComponentsLibraryView::dragLeaveEvent(QDragLeaveEvent *event)
{
	event->ignore();
}

void ComponentsLibraryView::dropEvent(QDropEvent *event)
{
	event->ignore();
}

void ComponentsLibraryView::startDrag(Qt::DropActions supportedActions)
{
	QMimeData* mimeData = model()->mimeData(selectedIndexes());

	QPixmap pixmap = qvariant_cast<QPixmap>(
								model()->data(selectedIndexes().first(), 
								Qt::UserRole));

	QDrag *drag = new QDrag(this);
	drag->setMimeData(mimeData);
	drag->setHotSpot(QPoint(pixmap.width() / 2, pixmap.height() / 2));
	drag->setPixmap(pixmap);
	drag->exec(Qt::CopyAction);
}

void ComponentsLibraryView::setModel(QAbstractItemModel * model)
{
	QTreeView::setModel(model);

	init();
}

void ComponentsLibraryView::onExpanded(const QModelIndex & index)
{
	App::componentsLibrary->item(index)->setExpanded(true);
}

void ComponentsLibraryView::onCollapsed(const QModelIndex & index)
{
	App::componentsLibrary->item(index)->setExpanded(false);
}

}
