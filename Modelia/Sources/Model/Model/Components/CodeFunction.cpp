#include "Conf.h"

#include "CodeFunction.h"

namespace Model
{

class CodeFunction::Impl
{
CodeFunction* w;

public:

	Type type;
	Interface::CodeFunctionView view;
	QString code;


	Impl(CodeFunction*, PropertyTree &);
	~Impl();

};

CodeFunction::Impl::Impl(CodeFunction* owner, PropertyTree & ptree)
	: w(owner),
	view(owner, ptree.count("View") != 0 ? ptree.get_child("View") : PropertyTree())
{

}

CodeFunction::Impl::~Impl()
{

}

CodeFunction::CodeFunction(Composition * owner, PropertyTree & ptree)
	: m(new Impl(this, ptree)), Process(owner, ptree)
{

	set(ptree);
	m->view.init();
}

PropertyTree CodeFunction::get() const
{
	PropertyTree ptree = Process::get();

	ptree.put_value("CodeFunction");
	ptree.put("Code", m->code.toStdString());
	ptree.put_child("View", m->view.get());

	return ptree;
}
void CodeFunction::set(PropertyTree & ptree)
{
	checkHierarchy("CodeFunction", QString::fromStdString(ptree.get_value<std::string>()));

	if (ptree.count("Code") != 0)
		m->code = QString::fromStdString(ptree.get<std::string>("Code"));

}

QString & CodeFunction::code() { return m->code; }
void CodeFunction::setCode(QString const & code) { m->code = code; }

Interface::CodeFunctionView * CodeFunction::view()
{
	return &m->view;
}

}