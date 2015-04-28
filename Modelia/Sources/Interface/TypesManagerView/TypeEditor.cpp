#include "TypeEditor.h"

#include "Model/TypesManager/TypesLibrary.h"
#include "Model/TypesManager/Type.h"

#include "TypesLibraryView.h"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>

namespace Interface
{

class TypeEditor::Impl
{

	TypeEditor* w;

public:

	Impl(TypeEditor *);
	~Impl();

	QGridLayout * layout = nullptr;
	Model::TypesLibraryItem* currentItem = nullptr;

	QString findName(QString &&, Model::TypesLibraryItem* = nullptr);

};

TypeEditor::Impl::Impl(TypeEditor* owner)
	: w(owner)
{

}

TypeEditor::Impl::~Impl()
{

}

TypeEditor::TypeEditor()
	: m(new Impl(this))
{
	m->layout = new QGridLayout();
	setLayout(m->layout);
}

TypeEditor::~TypeEditor()
{

}

void TypeEditor::fill()
{
	if (!m->currentItem) return;

	QLineEdit* name = new QLineEdit();
	m->layout->addWidget(new QLabel("Name: "), 1, 0);
	m->layout->addWidget(name, 1, 1);
	name->setText(m->currentItem->name());
	connect(name, SIGNAL(editingFinished()), this, SLOT(nameValidator()), Qt::UniqueConnection);

	if (!m->currentItem->editSelfPerm())
		name->setDisabled(true);

	if (!m->currentItem->type())
	{
		m->layout->addWidget(new QLabel("Names space"),0, 0);

		if (m->currentItem->editContentPerm())
		{

			QPushButton* addStructure = new QPushButton("Add structure");
			connect(addStructure, SIGNAL(clicked()), this, SLOT(addStructure()), Qt::UniqueConnection);
			m->layout->addWidget(addStructure, m->layout->rowCount(), 0);

			QPushButton* addAlias = new QPushButton("Add alias");
			connect(addAlias, SIGNAL(clicked()), this, SLOT(addAlias()), Qt::UniqueConnection);
			m->layout->addWidget(addAlias, m->layout->rowCount()-1, 1);

			QPushButton* addNamesSpace = new QPushButton("Add names space");
			connect(addNamesSpace, SIGNAL(clicked()), this, SLOT(addNamesSpace()), Qt::UniqueConnection);
			m->layout->addWidget(addNamesSpace, m->layout->rowCount()-1, 2);
		}
	}
	else
	{

		if (m->currentItem->type()->isBaseType())
			m->layout->addWidget(new QLabel("Base Type"), 0, 0);
		else
		{

			m->layout->addWidget(new QLabel("Name"), 1, 0);

			m->layout->addWidget(new QLabel("Name"), 1, 0);
			if (m->currentItem->type()->isAlias())
			{
				m->layout->addWidget(new QLabel("Alias"), 0, 0);

				QLineEdit* aliasType = new QLineEdit();
				m->layout->addWidget(new QLabel("Alias: "), m->layout->rowCount(), 0);
				m->layout->addWidget(aliasType, m->layout->rowCount()-1, 1);
				aliasType->setText(App::typesLibrary->getPath(m->currentItem->type()->alias()->type,
					m->currentItem->type()));
				connect(aliasType, SIGNAL(editingFinished()), 
					this, SLOT(aliasTypeValidator()), Qt::UniqueConnection);

				QCheckBox* aliasVectorized = new QCheckBox();
				m->layout->addWidget(new QLabel("Vector: "), m->layout->rowCount(), 0);
				m->layout->addWidget(aliasVectorized, m->layout->rowCount()-1, 1);
				if (m->currentItem->type()->alias()->vectorized)
					aliasVectorized->setChecked(true);
				connect(aliasVectorized, SIGNAL(stateChanged(int)), 
					this, SLOT(aliasVectorizeValidator(int)), Qt::UniqueConnection);

				if (!m->currentItem->editSelfPerm())
				{
					aliasType->setDisabled(true);
					aliasVectorized->setDisabled(true);
				}

			}
			else
			{
				m->layout->addWidget(new QLabel("Structure"), 0, 0);

				m->layout->addWidget(new QLabel("Fields: "), m->layout->rowCount(), 0);
				m->layout->addWidget(new QLabel("Name"), m->layout->rowCount(), 0);
				m->layout->addWidget(new QLabel("Type"), m->layout->rowCount() - 1, 1);
				m->layout->addWidget(new QLabel("Vector"), m->layout->rowCount() - 1, 2);

				QMapIterator<QString, Model::Field*> iter(m->currentItem->type()->fields());

				while (iter.hasNext())
				{
					iter.next();

					QLineEdit* fieldName = new QLineEdit();
					m->layout->addWidget(fieldName, m->layout->rowCount(), 0);
					fieldName->setText(iter.key());
					fieldName->setObjectName(iter.key());
					connect(fieldName, SIGNAL(editingFinished()),
						this, SLOT(fieldNameValidator()), Qt::UniqueConnection);

					QLineEdit* fieldType = new QLineEdit();
					m->layout->addWidget(fieldType, m->layout->rowCount() - 1, 1);
					fieldType->setText(App::typesLibrary->getPath(iter.value()->type,
						m->currentItem->type()));
					fieldType->setObjectName(iter.key());
					connect(fieldType, SIGNAL(editingFinished()),
						this, SLOT(fieldTypeValidator()), Qt::UniqueConnection);

					QCheckBox* fieldVector = new QCheckBox();
					m->layout->addWidget(fieldVector, m->layout->rowCount() - 1, 2);
					if (iter.value()->vectorized)
						fieldVector->setChecked(true);
					fieldVector->setObjectName(iter.key());
					connect(fieldVector, SIGNAL(stateChanged(int)),
						this, SLOT(fieldVectorValidator(int)), Qt::UniqueConnection);

					if (!m->currentItem->editContentPerm())
					{
						fieldName->setDisabled(true);
						fieldType->setDisabled(true);
						fieldVector->setDisabled(true);
					}

				}

				if (m->currentItem->editSelfPerm())
				{
					QPushButton* addField = new QPushButton("Add field");
					connect(addField, SIGNAL(clicked()), this, SLOT(addField()), Qt::UniqueConnection);
					m->layout->addWidget(addField, m->layout->rowCount(), 0, 1, 3);
				}
				else
					name->setDisabled(true);
			}
		}
	}

	m->layout->addItem(new QSpacerItem(0, 9999,
		QSizePolicy::Ignored, QSizePolicy::Maximum), m->layout->rowCount(), 0);
}

void TypeEditor::selectionChanged
(	
	Model::TypesLibraryItem * selected,
	Model::TypesLibraryItem * deselected)
{
	clearQLayout(m->layout);
	m->currentItem = selected;
	fill();
}

void TypeEditor::nameValidator()
{
	QLineEdit* name = dynamic_cast<QLineEdit*>(sender());
	if (!m->currentItem->parent()->hasChild(name->text()))
	{
		m->currentItem->setName(name->text());
	}
	else
		name->setText(m->currentItem->name());
}

void TypeEditor::aliasTypeValidator()
{
	QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(sender());
	Model::Type* newAliasType = App::typesLibrary->find(m->currentItem->type(), lineEdit->text());
	if (newAliasType)
		m->currentItem->type()->alias()->type = newAliasType;

	lineEdit->setText(App::typesLibrary->getPath(m->currentItem->type()->alias()->type,
	m->currentItem->type()));

}

void TypeEditor::aliasVectorizeValidator(int state)
{
	if (state == Qt::Checked)
		m->currentItem->type()->alias()->vectorized = true;
	else
		m->currentItem->type()->alias()->vectorized = false;
}

void TypeEditor::addStructure()
{
	PropertyTree ptree;

	ptree.put_value("TypesLibraryItem");
	ptree.put("EditSelfPerm", true);
	ptree.put("EditContentPerm", true);
	PropertyTree typePtree;
	typePtree.put_value("Type");
	typePtree.put("Name", m->findName("New_Structure").toStdString());
	typePtree.put("Fields", "");
	ptree.put_child("Type", typePtree);

	App::typesLibrary->insertItem(m->currentItem, ptree);
	App::typesLibraryView->expand(App::typesLibrary->index(m->currentItem));
}

void TypeEditor::addAlias()
{
	PropertyTree ptree;

	ptree.put_value("TypesLibraryItem");
	ptree.put("EditSelfPerm", true);
	ptree.put("EditContentPerm", true);
	PropertyTree typePtree;
	typePtree.put_value("Type");
	typePtree.put("Name", m->findName("New_Alias").toStdString());
	typePtree.put("Alias", "Core.Integer");
	typePtree.put("Vectorized", false);
	ptree.put_child("Type", typePtree);

	App::typesLibrary->insertItem(m->currentItem, ptree);
	App::typesLibraryView->expand(App::typesLibrary->index(m->currentItem));
}

void TypeEditor::addNamesSpace()
{
	PropertyTree ptree;

	ptree.put_value("TypesLibraryItem");
	ptree.put("EditSelfPerm", true);
	ptree.put("EditContentPerm", true);
	ptree.put("Name", m->findName("New_Namespace").toStdString());

	App::typesLibrary->insertItem(m->currentItem, ptree);
	App::typesLibraryView->expand(App::typesLibrary->index(m->currentItem));
}

QString TypeEditor::Impl::findName(QString && name, Model::TypesLibraryItem* parent)
{
	if (!parent) parent = currentItem;
	if (!parent->hasChild(name))
		return name;

	QString result;
	for (int i = 2;; i++)
	{
		result = name + "_" + QString::number(i);
		if (!parent->hasChild(result))
			return result;
	}
}

void TypeEditor::addField()
{
	m->currentItem->type()->addField(m->findName("new_field", m->currentItem), 
		new Model::Field(App::typesLibrary->defaultType()));

	selectionChanged(m->currentItem, m->currentItem);
}

void TypeEditor::fieldNameValidator()
{
	QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(sender());
	if (!m->currentItem->hasChild(lineEdit->text()))
	{
		QMap<QString, Model::Field*>& fields = m->currentItem->type()->fields();
		Model::Field* field = fields[sender()->objectName()];

		fields.remove(sender()->objectName());
		fields.insert(lineEdit->text(), field);

		QList<QWidget *> widgets = m->layout->parentWidget()->findChildren<QWidget *>(sender()->objectName());
		for (QWidget * widget : widgets)
				widget->setObjectName(lineEdit->text());

	}
	else
		lineEdit->setText(sender()->objectName());


}

void TypeEditor::fieldTypeValidator()
{
	QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(sender());
	Model::Type* newType = App::typesLibrary->find(m->currentItem->type(), lineEdit->text());
	Model::Field * test = m->currentItem->type()->field(sender()->objectName());
	if (newType)
		m->currentItem->type()->field(sender()->objectName())->type = newType;

	lineEdit->setText(App::typesLibrary->getPath(m->currentItem->type()->field(sender()->objectName())->type,
		m->currentItem->type()));
}

void TypeEditor::fieldVectorValidator(int state)
{
	Model::Field* field = m->currentItem->type()->field(sender()->objectName());
	if (state == Qt::Checked)
		field->vectorized = true;
	else
		field->vectorized = false;

}


}