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
	void setTypeEditor(TypeEditor *);

private slots:
	void selectionChanged(const QItemSelection &, const QItemSelection &);

signals: void selectionChanged(Model::TypesLibraryItem *, Model::TypesLibraryItem *);

};
}


