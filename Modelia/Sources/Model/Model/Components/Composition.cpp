#include "Conf.h"

#include "Composition.h"
#include "Variable.h"
#include "PureFunction.h"
#include "CodeFunction.h"
#include "Link.h"

namespace Model
{

Composition::Composition(Composition * owner, PropertyTree & ptree)
	: Process(owner, ptree),
	view_(this, ptree.count("View") != 0 ? ptree.get_child("View") : PropertyTree())
{
	set(ptree);
	view_.init();
}

/**************************
PropertyTrees
***************************/
PropertyTree Composition::get() const
{
	PropertyTree ptree = Process::get();

	ptree.put_value("Composition");
	ptree.put("IterationCount", iterationCount_);

	for (auto & variable : variables_)
		ptree.add_child("Elements.Element", variable.get());
	for (auto & pureFunction : pureFunctions_)
		ptree.add_child("Elements.Element", pureFunction.get());
	for (auto & codeFunction : codeFunctions_)
		ptree.add_child("Elements.Element", codeFunction.get());
	for (auto & composition : compositions_)
		ptree.add_child("Elements.Element", composition.get());
	for (auto & link : links_)
		ptree.add_child("Links.Link", link.get());

	ptree.put_child("View", view_.get());

	return ptree;
}
void Composition::set(PropertyTree & ptree)
{
	checkHierarchy("Composition", QString::fromStdString(ptree.get_value<std::string>()));

	iterationCount_ = 1;
	if (ptree.count("IterationCount") != 0)
		iterationCount_ = ptree.get<uint8>("IterationCount");

	if (ptree.count("Elements") != 0)
		for (auto element : ptree.get_child("Elements"))
			addElement(element.second);

	if (ptree.count("Links") != 0)
		for (auto link : ptree.get_child("Links"))
			addLink(link.second);

}

/**************************
Getters
***************************/
List<Variable>		&	Composition::variables() { return variables_; }
List<PureFunction>	&	Composition::pureFunctions() { return pureFunctions_; }
List<CodeFunction>	&	Composition::codeFunctions() { return codeFunctions_; }
List<Composition>	&	Composition::compositions() { return compositions_; }
List<Link>			&	Composition::links() { return links_; }
uint8					Composition::iterationCount() { return iterationCount_; }

/**************************
Inserter
***************************/
Component* Composition::addElement(PropertyTree & ptree)
{
	if (QString::fromStdString(ptree.get_value<std::string>()) == "Variable")
	{
		variables_.emplace_back(this, ptree);
		return &variables_.back();
	}
	else if (QString::fromStdString(ptree.get_value<std::string>()) == "PureFunction")
	{
		pureFunctions_.emplace_back(this, ptree);
		return &pureFunctions_.back();
	}
	else if (QString::fromStdString(ptree.get_value<std::string>()) == "CodeFunction")
	{
		codeFunctions_.emplace_back(this, ptree);
		return &codeFunctions_.back();
	}
	else if (QString::fromStdString(ptree.get_value<std::string>()) == "Composition")
	{
		compositions_.emplace_back(this, ptree);
		return &compositions_.back();
	}

	return nullptr;
}

Link* Composition::addLink(PropertyTree & ptree)
{
	links_.emplace_back(this, ptree);
	return &links_.back();
}

/**************************
Setter
***************************/
void Composition::setIterationCount(uint8 _iterationCount) 
	{ iterationCount_ = _iterationCount; }

/**************************
Deleter
***************************/

void Composition::removeComponent(QString const & name)
{
	auto check = [&name](Component & t){ return t.name() == name; };
	pureFunctions_.remove_if(check);
	codeFunctions_.remove_if(check);
	variables_.remove_if(check);
	compositions_.remove_if(check);

}
void Composition::removeLink(Link * link)
{
	links_.remove(*link);
}

/**************************
Find
***************************/
Component * Composition::find(QString const & name)
{
	for (Component & current : variables_)
		if (current.name() == name) return &current;
	for (Component & current : codeFunctions_)
		if (current.name() == name) return &current;
	for (Component & current : pureFunctions_)
		if (current.name() == name) return &current;
	for (Component & current : compositions_)
		if (current.name() == name) return &current;

	return nullptr;
}

Interface::CompositionView * Composition::view()
{
	return &view_;
}

}