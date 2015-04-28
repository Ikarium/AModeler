#include "Conf.h"

#include "ProcessView.h"

#include "ui_ProcessProperties.h"

#include "KExpandableWidget.h"

#include "Model/Model/Components/Process.h"
#include "Model/TypesManager/TypesLibrary.h"

#include <QBitMap>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QGroupBox>
#include <QPushButton>

namespace Interface
{

class ProcessView::Impl
{
	ProcessView* w;

public:

	Model::Process * model;

	KExpandableWidget* processPropertiesWidget = nullptr;
	bool processPropertiesWidgetOpen = true;

	Impl(ProcessView *, Model::Process *);
	~Impl();

};

ProcessView::Impl::Impl(ProcessView* owner, Model::Process * model_)
	: w(owner),
	model(model_)
{

}

ProcessView::Impl::~Impl()
{

}

ProcessView::ProcessView(Model::Process * model, PropertyTree & ptree)
	: m(new Impl(this, model)), ComponentView(static_cast<Model::Process*>(model), ptree)
{
	import(ptree);
}

ProcessView::~ProcessView()
{

}

/**************************
PropertyTrees
***************************/
PropertyTree ProcessView::export() const
{
	PropertyTree ptree = ComponentView::export();

	return ptree;
}

void ProcessView::import(PropertyTree & ptree)
{
	if (ptree.get_value<std::string>() == "View")
	{

	}
}

Model::Process * ProcessView::model()
{
	return m->model;
}

void ProcessView::paint(QPainter *painter, const QStyleOptionGraphicsItem *options, QWidget *widget)
{
	drawIllustration(painter, options);
	drawName(painter, options);
}

void ProcessView::drawIllustration(QPainter *painter, const QStyleOptionGraphicsItem *options) const
{
	QColor fillColor("#6DAB83");
	if (options->state & QStyle::State_HasFocus)
		fillColor = fillColor.lighter(140);
	else if (options->state & QStyle::State_Selected)
		fillColor = fillColor.lighter(130);
	else if (options->state & QStyle::State_MouseOver)
		fillColor = fillColor.lighter(120);

	QPen pen;
	pen.setWidth(2);
	QLinearGradient linearGrad(QPointF(0, 0), QPointF(0, size().width()));
	linearGrad.setColorAt(0, fillColor.lighter(120));
	linearGrad.setColorAt(1, fillColor);
	painter->setBrush(linearGrad);
	painter->drawRoundedRect(QRectF(QPointF(0, 0), size()),
		size().width() / 10, size().height() / 10);
}

void ProcessView::fillPropertiesWidget()
{
	ComponentView::fillPropertiesWidget();

	Ui::ProcessProperties ui;
	m->processPropertiesWidget = new KExpandableWidget("Process", m->processPropertiesWidgetOpen);
	ui.setupUi(m->processPropertiesWidget->content());

	QPixmap tmp = QPixmap("Sources/Ressources/Icons/draft.png");
	QPixmap deletePixmap(tmp.size());
	deletePixmap.fill("#916063");
	deletePixmap.setMask(tmp.createMaskFromColor(Qt::transparent));
	deletePixmap = deletePixmap.scaledToHeight(13, Qt::SmoothTransformation);


	QGridLayout* layout = new QGridLayout();
	int i = 0;
	for (Model::Slot & input : m->model->inputs())
	{
		QLineEdit * inputNameLineEdit = new QLineEdit(input.name());
		inputNameLineEdit->setObjectName(input.name());
		inputNameLineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
		connect(inputNameLineEdit, SIGNAL(editingFinished()), this, SLOT(slotRename()));
		layout->addWidget(inputNameLineEdit, i, 0, 1, 2, Qt::AlignTop);

		QLineEdit * inputTypeLineEdit = new QLineEdit(input.type()->getPath() + (input.vectorized() ? "[]" : ""));
		inputTypeLineEdit->setObjectName(input.name());
		inputTypeLineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
		connect(inputTypeLineEdit, SIGNAL(editingFinished()), this, SLOT(slotReType()));
		layout->addWidget(inputTypeLineEdit, i, 2, 1, 2, Qt::AlignTop);

		QPushButton * deleteInputBtn = new QPushButton(QIcon(deletePixmap), "");
		deleteInputBtn->setObjectName(input.name());
		deleteInputBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
		connect(deleteInputBtn, SIGNAL(clicked()), this, SLOT(deleteSlot()));
		layout->addWidget(deleteInputBtn, i, 4, 1, 1, Qt::AlignTop);
		i++;
	}
	layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Preferred, QSizePolicy::Expanding), i, 0);
	ui.inputs->setLayout(layout);

	i = 0;
	layout = new QGridLayout();
	for (Model::Slot & output : m->model->outputs())
	{
		QLineEdit * outputLineEdit = new QLineEdit(output.name());
		outputLineEdit->setObjectName(output.name());
		outputLineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
		connect(outputLineEdit, SIGNAL(editingFinished()), this, SLOT(slotRename()));
		layout->addWidget(outputLineEdit, i, 0, 1, 2, Qt::AlignTop);

		QLineEdit * outputTypeLineEdit = new QLineEdit(output.type()->getPath() + (output.vectorized() ? "[]" : ""));
		outputTypeLineEdit->setObjectName(output.name());
		outputTypeLineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
		connect(outputTypeLineEdit, SIGNAL(editingFinished()), this, SLOT(slotReType()));
		layout->addWidget(outputTypeLineEdit, i, 2, 1, 2, Qt::AlignTop);

		QPushButton * deleteOutputBtn = new QPushButton(QIcon(deletePixmap), "");
		deleteOutputBtn->setObjectName(output.name());
		deleteOutputBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
		connect(deleteOutputBtn, SIGNAL(clicked()), this, SLOT(deleteSlot()));		
		layout->addWidget(deleteOutputBtn, i, 4, 1, 1, Qt::AlignTop);

		i++;
	}

	layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Preferred, QSizePolicy::Expanding), i, 0);

	QPushButton * addNewOutputBtn = new QPushButton("Add");
	connect(addNewOutputBtn, SIGNAL(clicked()), this, SLOT(addNewOutput()));

	layout->addWidget(addNewOutputBtn, i+1, 0, 1, 4, Qt::AlignTop);
	ui.outputs->setLayout(layout);

	propertiesWidget()->layout()->addWidget(m->processPropertiesWidget);

}

void ProcessView::savePropertyWidget()
{
	ComponentView::savePropertyWidget();

	if (m->processPropertiesWidget)
		m->processPropertiesWidgetOpen = m->processPropertiesWidget->open();
	m->processPropertiesWidget = nullptr;
}

void ProcessView::slotRename()
{
	Model::Slot * slot = m->model->findSlot(sender()->objectName());
	if (!slot)
		KThrow(ModeliaExection, lvl5, "Slot not found.");

	QLineEdit * outputLineEdit = dynamic_cast<QLineEdit *>(sender());
	if (slot->slotType() == Model::SlotType::output)
	{
		if (!m->model->findOutputSlot(outputLineEdit->text()))
			slot->setName(outputLineEdit->text());
	}
	else
		if (!m->model->findSlot(outputLineEdit->text()))
			slot->setName(outputLineEdit->text());

	outputLineEdit->setText(slot->name());

	updateSlots();

	QList<QWidget *> widgets = m->processPropertiesWidget->layout()
		->parentWidget()->findChildren<QWidget *>(sender()->objectName());
	for (QWidget * widget : widgets)
		widget->setObjectName(slot->name());

	QGraphicsItem::update();
}

void ProcessView::slotReType()
{
	Model::Slot * slot = m->model->findSlot(sender()->objectName());
	if (slot)
	{
		QLineEdit * typeLineEdit = dynamic_cast<QLineEdit *>(sender());
		QString path = typeLineEdit->text();

		bool vectorized = path.endsWith("[]");
		if (vectorized)
			path.chop(2);

		Model::Type* newAliasType = App::typesLibrary->usePath(path);
		if (newAliasType)
		{
			slot->setType(newAliasType);
			slot->setVectorized(vectorized);
		}

		typeLineEdit->setText(slot->type()->getPath() + (slot->vectorized() ? "[]" : ""));
	}

	updateSlots();

	QGraphicsItem::update();
}

void ProcessView::deleteSlot()
{
	m->model->removeSlot(m->model->findSlot(sender()->objectName()));

	updatePropertiesWidget();

	updateSlots();

	QGraphicsItem::update();
}

Model::Slot * ProcessView::addNewOutput(Model::Type* type, bool vectorized)
{
	PropertyTree ptree;
	ptree.put_value("Slot");

	QString name = "y";
	for (int i = 2; m->model->findSlot(name); i++)
		name = "y" + QString::number(i);

	ptree.put("Name", name.toStdString());
	ptree.put("UniqueLink", false);
	ptree.put("SlotType", "Output");

	if (type)
		ptree.put("Type", type->getPath().toStdString());
	else
		ptree.put("Type", App::typesLibrary->defaultType()->getPath().toStdString());

	ptree.put("Vectorized", vectorized);

	Model::Slot * slot = m->model->addOutput(ptree);
	slot->view()->setParentItem(this);

	updatePropertiesWidget();

	updateSlots();

	QGraphicsItem::update();
	return slot;
}

Model::Slot * ProcessView::addNewInput(Model::Type* type, bool vectorized, QString originalName)
{
	PropertyTree ptree;
	ptree.put_value("Slot");

	QString name = originalName;
	for (int i = 2; m->model->findSlot(name); i++)
		name = originalName + QString::number(i);

	ptree.put("Name", name.toStdString());
	ptree.put("UniqueLink", true);
	ptree.put("SlotType", "Input");

	if (type)
		ptree.put("Type", type->getPath().toStdString());
	else
		ptree.put("Type", App::typesLibrary->defaultType()->getPath().toStdString());

	ptree.put("Vectorized", vectorized);

	Model::Slot * slot = m->model->addInput(ptree);
	slot->view()->setParentItem(this);
	updatePropertiesWidget();

	updateSlots();

	QGraphicsItem::update();
	return slot;
}


}