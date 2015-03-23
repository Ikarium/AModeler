#pragma once

#include "ComponentView.h"

namespace Interface
{

class ProcessView : public ComponentView
{
	Q_OBJECT
public:

	ProcessView(Model::Process*, PropertyTree &);
	~ProcessView();

/**************************
PropertyTrees
***************************/
	PropertyTree export() const;
	void import(PropertyTree & ptree);

	Model::Process * model();

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

private:
	virtual void drawIllustration(QPainter *, const QStyleOptionGraphicsItem *) const override;

public:
	virtual void fillPropertiesWidget() override;
	virtual void savePropertyWidget() override;

public slots:
	void slotRename();
	void deleteSlot();
	Model::Slot * addNewOutput();
	Model::Slot * addNewInput();

private:
	class Impl;
	UniquePtr<Impl> m;

};

}
