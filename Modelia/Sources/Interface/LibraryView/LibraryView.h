#pragma once

#include "Conf.h"

#include <QTreeView>

namespace Interface
{
class LibraryView : public QTreeView
{
	Q_OBJECT

public:
	LibraryView(QWidget * = nullptr);
	~LibraryView();

public:
	void dragEnterEvent(QDragEnterEvent *);
	void dragMoveEvent(QDragMoveEvent *);
	void dragLeaveEvent(QDragLeaveEvent *);
	void dropEvent(QDropEvent *);
	void startDrag(Qt::DropActions);
};
}

