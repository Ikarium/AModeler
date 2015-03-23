#pragma once

#include "Conf.h"

#include "ProcessView.h"

namespace Interface
{

class FunctionView : public ProcessView
{
	Q_OBJECT

public:

	FunctionView(Model::Function *, PropertyTree &);
	~FunctionView();
	
/**************************
PropertyTrees
***************************/
	PropertyTree export() const;
	void import(PropertyTree & ptree);

	Model::Function * model();

	virtual void fillPropertiesWidget() override;
	virtual void savePropertyWidget() override;

public slots:
	void codeEdited();

private:
	class Impl;
	UniquePtr<Impl> m;

};

}