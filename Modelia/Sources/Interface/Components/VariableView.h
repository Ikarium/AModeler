#pragma once

#include "Conf.h"

#include "ComponentView.h"

namespace Interface
{

class VariableView : public ComponentView
{

public:

	VariableView(Model::Variable *, PropertyTree &);
	~VariableView();

/**************************
PropertyTrees
***************************/
	PropertyTree get() const;

	void set(PropertyTree &);

	Model::Variable * model();

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
private:
	virtual void drawIllustration(QPainter *, const QStyleOptionGraphicsItem *) const override;

private:
	class Impl;
	UniquePtr<Impl> m;

};

}
