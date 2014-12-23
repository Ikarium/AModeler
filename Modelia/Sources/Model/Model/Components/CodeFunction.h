#pragma once

#include "Interface/Components/CodeFunctionView.h"

#include "Process.h"

namespace Model
{

class CodeFunction : public Process
{

public:

	CodeFunction(Composition *, PropertyTree &);

	PropertyTree get() const;

	void set(PropertyTree &);

	QString & code();
	void setCode(QString const &);

	virtual Interface::CodeFunctionView* view() override;

private:
	class Impl;
	UniquePtr<Impl> m;

};

}