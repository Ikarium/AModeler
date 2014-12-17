#include "Conf.h"

#include "CodeFunction.h"

namespace Model
{

CodeFunction::CodeFunction(Composition * owner, PropertyTree & ptree) 
	: Process(owner, ptree),
	view_(this, ptree.count("View") != 0 ? ptree.get_child("View") : PropertyTree())
{
		set(ptree);
		view_.init();
}

PropertyTree CodeFunction::get() const
{
	PropertyTree ptree = Process::get();

	ptree.put_value("CodeFunction");
	ptree.put("Code", code_.toStdString());
	ptree.put_child("View", view_.get());

	return ptree;
}
void CodeFunction::set(PropertyTree & ptree)
{
	checkHierarchy("CodeFunction", QString::fromStdString(ptree.get_value<std::string>()));

	if (ptree.count("Code") != 0)
		code_ = QString::fromStdString(ptree.get<std::string>("Code"));

}

QString & CodeFunction::code() { return code_; }
void CodeFunction::setCode(QString const & code) { code_ = code; }

Interface::CodeFunctionView * CodeFunction::view()
{
	return &view_;
}

}