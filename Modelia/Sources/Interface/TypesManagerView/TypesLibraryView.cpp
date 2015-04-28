#include "TypesLibraryView.h"

#include<QDropEvent>
#include<QDrag>
#include<QMimeData>

#include "Model/TypesManager/TypesLibrary.h"

namespace Interface
{

TypesLibraryView::TypesLibraryView(QWidget * parent) : QTreeView(parent)
{
	setSelectionMode(QAbstractItemView::SingleSelection);
	App::typesLibraryView = this;

	connect(this, SIGNAL(expanded(const QModelIndex &)), this, SLOT(onExpanded(const QModelIndex &)), Qt::UniqueConnection);
	connect(this, SIGNAL(collapsed(const QModelIndex &)), this, SLOT(onCollapsed(const QModelIndex &)), Qt::UniqueConnection);
}

void TypesLibraryView::init(Model::TypesLibraryItem* item)
{
	if (!item) item = App::typesLibrary->root();
	if (item->childrenCount() != 0)
	{
		setExpanded(App::typesLibrary->index(item), item->expanded());
		for (Model::TypesLibraryItem* child : item->children())
			init(child);
	}
}

TypesLibraryView::~TypesLibraryView()
{

}

void TypesLibraryView::setTypeEditor(TypeEditor * typeEditor_)
{
	typeEditor = typeEditor_;

	connect(this, SIGNAL(selectionChanged(Model::TypesLibraryItem *, Model::TypesLibraryItem *)),
		typeEditor, SLOT(selectionChanged(Model::TypesLibraryItem *, Model::TypesLibraryItem *)));
}

void TypesLibraryView::setModel(QAbstractItemModel * model)
{
	QTreeView::setModel(model);

	init();
}

void TypesLibraryView::selectionChanged(const QItemSelection & selected, const QItemSelection & deselected)
{
	selectionChanged((selected.indexes().empty()) ? nullptr : App::typesLibrary->item(selected.indexes().first()),
		(deselected.indexes().empty()) ? nullptr : App::typesLibrary->item(deselected.indexes().first()));
}

void TypesLibraryView::onExpanded(const QModelIndex & index)
{
	App::typesLibrary->item(index)->setExpanded(true);
}

void TypesLibraryView::onCollapsed(const QModelIndex & index)
{
	App::typesLibrary->item(index)->setExpanded(false);
}

}
