#pragma once

#include "Conf.h"

#include "ProcessView.h"

namespace Interface
{

class CodeFunctionView : public ProcessView
{
	Q_OBJECT

public:

	CodeFunctionView(Model::CodeFunction *, PropertyTree &);
	~CodeFunctionView();
	
/**************************
PropertyTrees
***************************/
	PropertyTree get() const;

	void set(PropertyTree &);

	Model::CodeFunction * model();

	void fillPropertiesWidget();

public slots:
	void codeEdited();

private:
	class Impl;
	UniquePtr<Impl> m;

};

}