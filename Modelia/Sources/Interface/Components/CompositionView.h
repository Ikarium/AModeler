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
	PropertyTree export() const;
	void import(PropertyTree & ptree);

	Model::Composition * model();

	virtual void fillPropertiesWidget() override;
	virtual void savePropertyWidget() override;

public slots:
	void iterationCountUpdate(int);

private:
	class Impl;
	UniquePtr<Impl> m;

};

}