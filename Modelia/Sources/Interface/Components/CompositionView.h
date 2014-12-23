#pragma once

#include "Conf.h"

#include "ProcessView.h"

namespace Interface
{

class CompositionView : public ProcessView
{
	Q_OBJECT

public:

	CompositionView(Model::Composition *, PropertyTree &);
	~CompositionView();

	/**************************
	PropertyTrees
	***************************/
	PropertyTree get() const;

	void set(PropertyTree &);

	Model::Composition * model();

	void fillPropertiesWidget();

public slots:
	void iterationCountUpdate(int);

private:
	class Impl;
	UniquePtr<Impl> m;

};

}