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

	void init(Model::ComponentsLibraryItem* = nullptr);

	void dragEnterEvent(QDragEnterEvent *);
	void dragMoveEvent(QDragMoveEvent *);
	void dragLeaveEvent(QDragLeaveEvent *);
	void dropEvent(QDropEvent *);
	void startDrag(Qt::DropActions);

	virtual void setModel(QAbstractItemModel *) override;

private slots:
	void onExpanded(const QModelIndex &);
	void onCollapsed(const QModelIndex &);

};
}

