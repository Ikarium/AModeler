#pragma once

#include "Process.h"

#include "Interface/Components/PureFunctionView.h"

namespace Model
{

class PureFunction : public Process
{
	QString code_;

	Interface::PureFunctionView view_;

public:
	PureFunction(Composition *, PropertyTree &);

	PropertyTree get() const;

	void set(PropertyTree &);

	QString & code();
	void code(QString const &);

	virtual Interface::PureFunctionView * view() override;
};

}