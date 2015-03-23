#pragma once

#include "Conf.h"

#include <QTreeView>

namespace Interface
{
class ComponentsLibraryView : public QTreeView
{
	Q_OBJECT

public:
	ComponentsLibraryView(QWidget * = nullptr);
	~ComponentsLibraryView();

	void dragEnterEvent(QDragEnterEvent *);
	void dragMoveEvent(QDragMoveEvent *);
	void dragLeaveEvent(QDragLeaveEvent *);
	void dropEvent(QDropEvent *);
	void startDrag(Qt::DropActions);
};
}

