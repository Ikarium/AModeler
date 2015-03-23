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
}


ComponentsLibraryView::~ComponentsLibraryView()
{

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

}
