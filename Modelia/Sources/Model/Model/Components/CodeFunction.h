#pragma once

#include "Interface/Components/CodeFunctionView.h"

#include "Process.h"

namespace Model
{

class CodeFunction : public Process
{

private:

	QString code_;

	Interface::CodeFunctionView view_;

public:

	CodeFunction(Composition *, PropertyTree &);

	PropertyTree get() const;

	void set(PropertyTree &);

	QString & code();
	void setCode(QString const &);

	virtual Interface::CodeFunctionView* view() override;

};

}