#pragma once

#include "Interface/Components/FunctionView.h"

#include "Process.h"

namespace Model
{

class Function : public Process
{

public:

	Function(Composition *, PropertyTree &);

	PropertyTree export() const;

	void import(PropertyTree &);

	QString & code();
	void setCode(QString const &);

	virtual Interface::FunctionView* view() override;

private:
	class Impl;
	UniquePtr<Impl> m;

};

}