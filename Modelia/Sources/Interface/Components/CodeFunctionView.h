#pragma once

#include "Conf.h"

#include "ProcessView.h"

namespace Interface
{

class CodeFunctionView : public ProcessView
{
	Q_OBJECT

	Model::CodeFunction * model_;

public:

	CodeFunctionView(Model::CodeFunction *, PropertyTree &);
	
/**************************
PropertyTrees
***************************/
	PropertyTree get() const;

	void set(PropertyTree &);

	Model::CodeFunction * model();

	void fillPropertiesWidget();

public slots:
	void codeEdited();

};

}