#include "Conf.h"

#include "PureFunction.h"

namespace Model
{

class PureFunction::Impl
{
	PureFunction* w;

public:
	Type type;
	Interface::PureFunctionView view;
	QString code;

	Impl(PureFunction*, PropertyTree &);


};

PureFunction::Impl::Impl(PureFunction* owner, PropertyTree & ptree)
	: w(owner),
	view(owner, ptree.count("View") != 0 ? ptree.get_child("View") : PropertyTree())
{

}

PureFunction::PureFunction(Composition * owner, PropertyTree & ptree)
	: m(new Impl(this, ptree)), Process(owner, ptree)
{
	import(ptree);
	m->view.init();
}

PropertyTree PureFunction::export() const
{
	PropertyTree ptree = Process::export();

	ptree.put_value("PureFunction");
	ptree.put("Code", m->code.toStdString());
	ptree.put_child("View", m->view.export());

	return ptree;
}

void PureFunction::import(PropertyTree & ptree)
{
	checkHierarchy("PureFunction", QString::fromStdString(ptree.get_value<std::string>()));

	if (ptree.count("Code") != 0)
		m->code = QString::fromStdString(ptree.get<std::string>("Code"));

}

QString & PureFunction::code() { return m->code; }
void PureFunction::code(QString const & code) { m->code = code; }

Interface::PureFunctionView * PureFunction::view() 
{ 
	return &m->view;
}

}