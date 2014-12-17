#pragma once

#include "ComponentView.h"

namespace Interface
{

class ProcessView : public ComponentView
{

	Q_OBJECT

	Model::Process* model_;

public:

	ProcessView(Model::Process*, PropertyTree &);

/**************************
PropertyTrees
***************************/
	PropertyTree get() const;

	void set(PropertyTree &);

	Model::Process * model();

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

private:
	virtual void drawIllustration(QPainter *, const QStyleOptionGraphicsItem *) const override;

public:
	virtual void fillPropertiesWidget() override;

public slots:
	void slotRename();
	void deleteSlot();
	Model::Slot * addNewOutput();
	Model::Slot * addNewInput();

};
}
