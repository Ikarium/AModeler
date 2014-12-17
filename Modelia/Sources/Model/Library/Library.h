#pragma once

#include "Conf.h"

#include <QTreeWidget>

namespace Model
{

class LibraryItem
{
	Vector<LibraryItem> children_;
	LibraryItem* parent_;
	uint16 row_;
	QString elem_;
	QString name_;
	bool isElement_;
	QString type_;
	QPixmap pixmap_;
	Component* component_;

public:
	LibraryItem(LibraryItem *, uint16, PropertyTree &);

	LibraryItem* parent()	const { return parent_; }
	uint16 row()			const { return row_; }
	QString const & elem()	const { return elem_; }
	QPixmap const & pixmap()const { return pixmap_; }
	QString const & type()	const { return type_; }
	QString const & name()	const { return name_; }
	bool isElement()		const { return isElement_; }
	Vector<LibraryItem> & children()  { return children_; }

	void addChild(PropertyTree &);
	PropertyTree save();
};

class Library : public QAbstractItemModel
{
	Q_OBJECT
private:
	LibraryItem * root;

public:

	Library(QString const & = "Default.library");
	~Library();

	QVariant data(const QModelIndex &index, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	Qt::DropActions supportedDropActions() const;
	QMimeData * Library::mimeData(const QModelIndexList & indexes) const;

	void addUserElem(QString);

	void populate(QString const &);
	void save(QString const &);

};

}