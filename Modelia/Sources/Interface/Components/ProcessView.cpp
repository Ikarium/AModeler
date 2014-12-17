#include "Conf.h"

#include "ProcessView.h"

#include "Model/Model/Components/Process.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QGroupBox>
#include <QPushButton>

namespace Interface
{

ProcessView::ProcessView(Model::Process* _model, PropertyTree & ptree) 
	: ComponentView(static_cast<Model::Component*>(_model), ptree),
	model_(_model)
{ 
	set(ptree);
}

/**************************
PropertyTrees
***************************/
PropertyTree ProcessView::get() const
{
	PropertyTree ptree = ComponentView::get();

	return ptree;
}

void ProcessView::set(PropertyTree & ptree)
{
	if (ptree.get_value<std::string>() == "View")
	{

	}
}

Model::Process * ProcessView::model()
{
	return model_;
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
	QLinearGradient linearGrad(QPointF(0, 0), QPointF(0, size_.width()));
	linearGrad.setColorAt(0, fillColor.lighter(120));
	linearGrad.setColorAt(1, fillColor);
	painter->setBrush(linearGrad);
	painter->drawRoundedRect(	QRectF(QPointF(0, 0), size_), 
								size_.width() / 10, size_.height()/10);
}

void ProcessView::fillPropertiesWidget()
{
	ComponentView::fillPropertiesWidget();

	QGroupBox * inputs = new QGroupBox("Inputs");
	QGridLayout * inputsLayout = new QGridLayout();
	int i = 0;
	for (Model::Slot & input : model_->inputs())
	{
		QLineEdit * inputLineEdit = new QLineEdit(input.name(), propertiesWidget);
		inputLineEdit->setObjectName(input.name());
		inputsLayout->addWidget(inputLineEdit, i, 1);
		connect(inputLineEdit, SIGNAL(editingFinished()), this, SLOT(slotRename()));
		QPushButton * deleteInputBtn = new QPushButton("delete");
		deleteInputBtn->setObjectName(input.name());
		inputsLayout->addWidget(deleteInputBtn, i, 2);
		connect(deleteInputBtn, SIGNAL(clicked()), this, SLOT(deleteSlot()));
		i++;
	}
	inputs->setLayout(inputsLayout);
	dynamic_cast<QGridLayout*>(propertiesWidget->layout())->addWidget(inputs, 0, 1);

	QGroupBox * outputs = new QGroupBox("Outputs");
	QGridLayout * outputsLayout = new QGridLayout();
	i = 0;
	for (Model::Slot & output : model_->outputs())
	{
		QLineEdit * outputLineEdit = new QLineEdit(output.name(), propertiesWidget);
		outputLineEdit->setObjectName(output.name());
		outputsLayout->addWidget(outputLineEdit, i, 1);
		connect(outputLineEdit, SIGNAL(editingFinished()), this, SLOT(slotRename()));
		QPushButton * deleteOutputBtn = new QPushButton("delete");
		deleteOutputBtn->setObjectName(output.name());
		outputsLayout->addWidget(deleteOutputBtn, i, 2);
		connect(deleteOutputBtn, SIGNAL(clicked()), this, SLOT(deleteSlot()));
		i++;
	}
	QPushButton * addNewOutputBtn = new QPushButton("Add new output");
	outputsLayout->addWidget(addNewOutputBtn, outputsLayout->rowCount(), 1, 2, 1);
	outputs->setLayout(outputsLayout);
	connect(addNewOutputBtn, SIGNAL(clicked()), this, SLOT(addNewOutput()));
	dynamic_cast<QGridLayout*>(propertiesWidget->layout())->addWidget(outputs, 0, 2);

}

void ProcessView::slotRename()
{
	Model::Slot * slot = model_->findSlot(sender()->objectName());
	if (slot)
	{
		QLineEdit * outputLineEdit = dynamic_cast<QLineEdit *>(sender());
		if (model_->findSlot(outputLineEdit->text()))
			outputLineEdit->setText(slot->name());
		else
			slot->setName(outputLineEdit->text());
	}

	updateSlots();

	QGraphicsItem::update();
}

void ProcessView::deleteSlot()
{
	Model::Slot * slot = model_->findSlot(sender()->objectName());

	if (slot->slotType() == Model::SlotType::input)
		model_->removeInput(slot);
	else model_->removeOutput(slot);
	clearLayout(propertiesWidget->layout());
	delete propertiesWidget->layout();
	fillPropertiesWidget();

	updateSlots();

	QGraphicsItem::update();
}

Model::Slot * ProcessView::addNewOutput()
{
	PropertyTree ptree;
	ptree.put_value("Slot");

	QString name = "y";
	for (int i = 2; model_->findSlot(name); i++)
		name = "y" + QString::number(i);

	ptree.put("Name", name.toStdString());
	ptree.put("UniqueLink", false);
	ptree.put("SlotType", "Output");
	ptree.put_child("Type", simpleType());

	Model::Slot * slot = model_->addOutput(ptree);
	slot->view()->setParentItem(this);
	clearLayout(propertiesWidget->layout());
	delete propertiesWidget->layout();
	fillPropertiesWidget();

	updateSlots();

	QGraphicsItem::update();
	return slot;
}

Model::Slot * ProcessView::addNewInput()
{
	PropertyTree ptree;
	ptree.put_value("Slot");

	QString name = "x";
	for (int i = 2; model_->findSlot(name); i++)
		name = "x" + QString::number(i);

	ptree.put("Name", name.toStdString());
	ptree.put("UniqueLink", true);
	ptree.put("SlotType", "Input");
	ptree.put_child("Type", simpleType());

	Model::Slot * slot = model_->addInput(ptree);
	slot->view()->setParentItem(this);
	if (propertiesWidget)
	{
		if (propertiesWidget->layout())
		{
			clearLayout(propertiesWidget->layout());
			delete propertiesWidget->layout();
		}
		fillPropertiesWidget();
	}

	updateSlots();

	QGraphicsItem::update();
	return slot;
}


}