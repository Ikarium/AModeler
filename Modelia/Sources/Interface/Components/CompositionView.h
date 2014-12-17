#pragma once

#include "Conf.h"

#include "ProcessView.h"

namespace Interface
{

class CompositionView : public ProcessView
{
	Q_OBJECT

	Model::Composition * model_;

public:

	CompositionView(Model::Composition *, PropertyTree &);

	/**************************
	PropertyTrees
	***************************/
	PropertyTree get() const;

	void set(PropertyTree &);

	Model::Composition * model();

	void fillPropertiesWidget();

public slots:
	void iterationCountUpdate(int);

};

}