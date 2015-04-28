#pragma once

#include "Conf.h"
#include "TypeEditor.h"

#include <QTreeView>

namespace Interface
{
class TypesLibraryView : public QTreeView
{
	Q_OBJECT

	TypeEditor* typeEditor = nullptr;

public:
	TypesLibraryView(QWidget * = nullptr);
	~TypesLibraryView();
	void init(Model::TypesLibraryItem* = nullptr);

	void setTypeEditor(TypeEditor *);

	virtual void setModel(QAbstractItemModel *) override;

private slots:
	void selectionChanged(const QItemSelection &, const QItemSelection &);
	void onExpanded(const QModelIndex &);
	void onCollapsed(const QModelIndex &);

signals: void selectionChanged(Model::TypesLibraryItem *, Model::TypesLibraryItem *);

};

}
