#include "Conf.h"

#include "Function.h"

namespace Model
{

class Function::Impl
{

Function* w;

public:

	Interface::FunctionView view;
	QString code;


	Impl(Function*, PropertyTree &);
	~Impl();

};

Function::Impl::Impl(Function* owner, PropertyTree & ptree)
	: w(owner),
	view(owner, ptree.count("View") != 0 ? ptree.get_child("View") : PropertyTree())
{

}

Function::Impl::~Impl()
{

}

Function::Function(Composition * owner, PropertyTree & ptree)
	: m(new Impl(this, ptree)), Process(owner, ptree)
{

	import(ptree);
	m->view.init();
}

PropertyTree Function::export() const
{
	PropertyTree ptree = Process::export();

	ptree.put_value("Function");
	ptree.put("Code", m->code.toStdString());
	ptree.put_child("View", m->view.export());

	return ptree;
}
void Function::import(PropertyTree & ptree)
{
	checkHierarchy("Function", QString::fromStdString(ptree.get_value<std::string>()));

	if (ptree.count("Code") != 0)
		m->code = QString::fromStdString(ptree.get<std::string>("Code"));

}

QString & Function::code() { return m->code; }
void Function::setCode(QString const & code) { m->code = code; }

Interface::FunctionView * Function::view()
{
	return &m->view;
}

}