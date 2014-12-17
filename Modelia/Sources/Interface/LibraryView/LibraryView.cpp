#include "LibraryView.h"

#include<QDropEvent>
#include<QDrag>
#include<QMimeData>

#include "Model/Library/Library.h"

namespace Interface
{

LibraryView::LibraryView(QWidget * parent) : QTreeView(parent)
{
	setSelectionMode(QAbstractItemView::SingleSelection);
	setDragEnabled(true);
	setAcceptDrops(false);
	setDropIndicatorShown(true);
	setDragDropMode(QAbstractItemView::DragDrop);
}


LibraryView::~LibraryView()
{

}


void LibraryView::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat("application/modelia/component"))
		event->accept();
	else
		event->ignore();
}

void LibraryView::dragMoveEvent(QDragMoveEvent *event)
{
	if (event->mimeData()->hasFormat("application/modelia/component")) {
		event->setDropAction(Qt::CopyAction);
		event->accept();
	}
	else {
		event->ignore();
	}
}

void LibraryView::dragLeaveEvent(QDragLeaveEvent *event)
{
	event->ignore();
}

void LibraryView::dropEvent(QDropEvent *event)
{
	event->ignore();
}

void LibraryView::startDrag(Qt::DropActions supportedActions)
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
