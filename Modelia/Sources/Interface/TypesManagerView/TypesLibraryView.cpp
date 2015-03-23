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

}


TypesLibraryView::~TypesLibraryView()
{

}

void TypesLibraryView::selectionChanged(const QItemSelection & selected, const QItemSelection & deselected)
{
	selectionChanged((selected.indexes().empty()) ? nullptr : App::typesLibrary->getItem(selected.indexes().first()),
		(deselected.indexes().empty()) ? nullptr : App::typesLibrary->getItem(deselected.indexes().first()));
}

void TypesLibraryView::setTypeEditor(TypeEditor * typeEditor_)
{
	typeEditor = typeEditor_;

	connect(this, SIGNAL(selectionChanged(Model::TypesLibraryItem *, Model::TypesLibraryItem *)),
		typeEditor, SLOT(selectionChanged(Model::TypesLibraryItem *, Model::TypesLibraryItem *)));
}

}
