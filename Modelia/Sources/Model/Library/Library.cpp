#include "Library.h"

#include<QMimeData>

#include "Model/Model/Components/Variable.h"
#include "Model/Model/Components/PureFunction.h"
#include "Model/Model/Components/CodeFunction.h"
#include "Model/Model/Components/Composition.h"
#include "Model/Model/Components/Link.h"

namespace Model
{
	
LibraryItem::LibraryItem(	LibraryItem* parent, 
							uint16 row, 
							PropertyTree & ptree)
	: parent_(parent), row_(row)
{

	if (QString::fromStdString(ptree.get_value<std::string>()) != "LibraryItem")
		KThrow(ModeliaExection, lvl6, "Bad library ptree");

	name_ = QString::fromStdString(ptree.get<std::string>("Name"));
	isElement_ = ptree.get<bool>("isElement");
	if (isElement_)
	{
		type_ = QString::fromStdString(ptree.get<std::string>("Element"));
		PropertyTree viewPtree = (ptree.count("Element.View") != 0) 
									? ptree.get_child("Element.View") 
									: PropertyTree();
		if (type_ == "Variable")
			pixmap_ = Interface::VariableView(nullptr, viewPtree).getIllustration();
		else if (type_ == "PureFunction")
			pixmap_ = Interface::PureFunctionView(nullptr, viewPtree).getIllustration();
		else if (type_ == "CodeFunction")
			pixmap_ = Interface::CodeFunctionView(nullptr, viewPtree).getIllustration();
		else if (type_ == "Composition")
			pixmap_ = Interface::CompositionView(nullptr, viewPtree).getIllustration();

		elem_ = savePtree(ptree.get_child("Element"));
	}

	if (ptree.count("Items") != 0)
		for (auto item : ptree.get_child("Items"))
			addChild(item.second);
}

void LibraryItem::addChild(PropertyTree & ptree)
{
	children_.emplace_back(this, children_.size(), ptree);
}

PropertyTree LibraryItem::save()
{
	PropertyTree ptree;

	ptree.put_value("LibraryItem");
	ptree.put("Name", name_.toStdString());
	ptree.put("isElement", isElement_);
	for (auto & child : children_)
		ptree.add_child("Items.Item", child.save());
	if (isElement_)
	{
		PropertyTree tmpTree = loadPtree(elem_);
		tmpTree.put_value(type_.toStdString());
		ptree.put_child("Element", tmpTree);
	}

	return ptree;
}



Library::Library(QString const & path)
{
	populate(path);
}

QVariant Library::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	LibraryItem *item = static_cast<LibraryItem*>(index.internalPointer());

	if (role == Qt::DisplayRole)
	{
		return item->name();
	}
	else if (!item->isElement())
		return QVariant();
	else if (role == Qt::DecorationRole)
	{
		return QIcon(item->pixmap());
	}
	else if (role == Qt::UserRole)
	{
		return item->pixmap();
	}
	else if (role == Qt::ToolTipRole)
	{
		return item->type();
	}
	else
		return QVariant();
}

QModelIndex Library::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	if (parent.internalPointer() == &root->children()[1])
		row = row;
	LibraryItem *parentItem;

	if (!parent.isValid())
		parentItem = root;
	else
		parentItem = static_cast<LibraryItem*>(parent.internalPointer());

	LibraryItem *childItem = &parentItem->children()[row];
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex Library::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	if (index.internalPointer() == &root->children()[1])
		int row = 100;
	LibraryItem *childItem = static_cast<LibraryItem*>(index.internalPointer());
	LibraryItem *parentItem = childItem->parent();

	if (parentItem == root)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int Library::rowCount(const QModelIndex &parent) const
{
	LibraryItem *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = root;
	else
		parentItem = static_cast<LibraryItem*>(parent.internalPointer());

	return parentItem->children().size();
}

int Library::columnCount(const QModelIndex &parent) const
{
	return 1;
}

void Library::populate(QString const & path)
{
	PropertyTree ptree;
	infoToPree(path.toStdString(), ptree);

	if (QString::fromStdString(ptree.get<std::string>("FileType")) != "Library")
		KThrow(ModeliaExection, lvl6, "Bad library ptree");
	beginResetModel();
	root = new LibraryItem(nullptr, 0, ptree.get_child("Root"));
	endResetModel();
}

void Library::save(QString const & path)
{
	PropertyTree ptree;

	ptree.put("FileType", "Library");
	ptree.put_child("Root", root->save());
	ptreeToInfo(path.toStdString(), ptree);
}

Library::~Library()
{
	delete root;
}

Qt::ItemFlags Library::flags(const QModelIndex &index) const
{

	if (!index.isValid())
		return 0;

	Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);
	LibraryItem *childItem = static_cast<LibraryItem*>(index.internalPointer());

	if (childItem->isElement())
		return Qt::ItemIsDragEnabled | defaultFlags;
	else
		return defaultFlags;
}

Qt::DropActions Library::supportedDropActions() const
{
	return Qt::CopyAction | Qt::MoveAction;
}

QMimeData * Library::mimeData(const QModelIndexList & indexes) const
{
	QMimeData *mimeData = new QMimeData();
	QByteArray encodedData;

	QDataStream stream(&encodedData, QIODevice::WriteOnly);

	foreach(QModelIndex index, indexes)
		if (index.isValid())
		{
			LibraryItem* item = static_cast<LibraryItem*>(index.internalPointer());
			stream << item->type();
			stream << item->elem();
		}

	mimeData->setData("application/modelia/library/component", encodedData);
	return mimeData;
}

void Library::addUserElem(QString elem)
{
	PropertyTree ptree;
	PropertyTree elemPtree = loadPtree(elem);
	ptree.put_value("LibraryItem");
	ptree.put("Name", elemPtree.get<std::string>("Name"));
	ptree.put("isElement", true);
	elemPtree.put_value("Composition");
	ptree.put_child("Element", elemPtree);

	beginResetModel();
	root->addChild(ptree);
	endResetModel();
}


}