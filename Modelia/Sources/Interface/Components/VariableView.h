#pragma once

#include "Conf.h"

#include "ComponentView.h"

namespace Interface
{

class VariableView : public ComponentView
{

	Q_OBJECT

public:

	VariableView(Model::Variable *, PropertyTree &);
	~VariableView();

/**************************
PropertyTrees
***************************/
	PropertyTree export() const;
	void import(PropertyTree & ptree);

	Model::Variable * model();

	virtual void fillPropertiesWidget() override;
	virtual void savePropertyWidget() override;
	void setInitialValue(QString);

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
private:
	virtual void drawIllustration(QPainter *, const QStyleOptionGraphicsItem *) const override;

public slots:
	void reType();
	void updateInitialValue();

private:
	class Impl;
	UniquePtr<Impl> m;

};

}
