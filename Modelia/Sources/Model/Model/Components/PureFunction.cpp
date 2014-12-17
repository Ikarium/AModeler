#include "Conf.h"

#include "PureFunction.h"

namespace Model
{

PureFunction::PureFunction(Composition * owner, PropertyTree & ptree)
	: Process(owner, ptree),
	view_(this, ptree.count("View") != 0 ? ptree.get_child("View") : PropertyTree())
{
	set(ptree);
	view_.init();
}

PropertyTree PureFunction::get() const
{
	PropertyTree ptree = Process::get();

	ptree.put_value("PureFunction");
	ptree.put("Code", code_.toStdString());
	ptree.put_child("View", view_.get());

	return ptree;
}
void PureFunction::set(PropertyTree & ptree)
{
	checkHierarchy("PureFunction", QString::fromStdString(ptree.get_value<std::string>()));

	if (ptree.count("Code") != 0)
		code_ = QString::fromStdString(ptree.get<std::string>("Code"));

}

QString & PureFunction::code() { return code_; }
void PureFunction::code(QString const & code) { code_ = code; }

Interface::PureFunctionView * PureFunction::view() 
{ 
	return &view_; 
}

}