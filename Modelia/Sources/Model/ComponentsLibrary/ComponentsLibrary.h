#pragma once

#include "Conf.h"

#include <QTreeWidget>

namespace Model
{

class ComponentsLibraryItem
{
	friend ComponentsLibrary;

public:
	ComponentsLibraryItem(ComponentsLibraryItem *, uint16, PropertyTree &);
	ComponentsLibraryItem(ComponentsLibraryItem &&);
	~ComponentsLibraryItem();

	void addChild(PropertyTree &);
	PropertyTree save();

private:
	class Impl;
	UniquePtr<Impl> m;
};

class ComponentsLibrary : public QAbstractItemModel
{
	Q_OBJECT

private:
	ComponentsLibraryItem * root;

public:

	ComponentsLibrary(QString const & = "Default.library");
	~ComponentsLibrary();

	QVariant data(const QModelIndex &index, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	Qt::DropActions supportedDropActions() const;
	QMimeData * mimeData(const QModelIndexList & indexes) const;

	void addUserElem(QString);

	void populate(QString const &);
	void save(QString const &);

};

}