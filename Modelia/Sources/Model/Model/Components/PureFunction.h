#pragma once

#include "Process.h"

#include "Interface/Components/PureFunctionView.h"

namespace Model
{

class PureFunction : public Process
{

public:
	PureFunction(Composition *, PropertyTree &);

	PropertyTree export() const;
	void import(PropertyTree & ptree);

	QString & code();
	void code(QString const &);

	virtual Interface::PureFunctionView * view() override;

private:
	class Impl;
	UniquePtr<Impl> m;

};

}