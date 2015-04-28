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
	ComponentsLibraryItem & operator=(ComponentsLibraryItem const &) = delete;
	ComponentsLibraryItem(ComponentsLibraryItem const &) = delete;
	~ComponentsLibraryItem();

	PropertyTree save();

	QString name() const;
	void setName(QString &&);

	ComponentsLibraryItem* parent() const;

	int childrenCount();
	bool hasChild(QString &&);
	ComponentsLibraryItem* child(QString &&) const;
	Vector<ComponentsLibraryItem*> & children();
	void addChild(PropertyTree &);
	void removeChild(ComponentsLibraryItem*);
	void removeChild(QString const &&);


	bool expanded() const;
	void setExpanded(bool);

private:
	class Impl;
	UniquePtr<Impl> m;
};

class ComponentsLibrary : public QAbstractItemModel
{
	Q_OBJECT

public:

	ComponentsLibrary(QString const & = "Default.library");
	~ComponentsLibrary();

	void populate(QString const &);
	void save(QString const &);

	ComponentsLibraryItem* root() const;

	QVariant data(const QModelIndex &index, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	QModelIndex index(ComponentsLibraryItem*) const;
	ComponentsLibraryItem* item(QModelIndex const &) const;
	QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	Qt::DropActions supportedDropActions() const;
	QMimeData * mimeData(const QModelIndexList & indexes) const;

	void insertItem(ComponentsLibraryItem*, PropertyTree &);
	void removeItem(ComponentsLibraryItem*);

public slots:
	void addModelElement();


private:
	class Impl;
	UniquePtr<Impl> m;

};

}