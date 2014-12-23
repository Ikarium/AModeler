#include "Conf.h"

#include "Composition.h"
#include "Variable.h"
#include "PureFunction.h"
#include "CodeFunction.h"
#include "Link.h"

namespace Model
{

class Composition::Impl
{
	Composition* w;

public:

	Type type;
	Interface::CompositionView view;

	List<Link> links;
	List<Variable> variables;
	List<PureFunction> pureFunctions;
	List<CodeFunction> codeFunctions;
	List<Composition> compositions;

	uint8 iterationCount;


	Impl(Composition*, PropertyTree &);
	~Impl();

};

Composition::Impl::Impl(Composition* owner, PropertyTree & ptree)
	: w(owner),
	view(owner, ptree.count("View") != 0 ? ptree.get_child("View") : PropertyTree())
{

}

Composition::Impl::~Impl()
{

}

Composition::Composition(Composition * owner, PropertyTree & ptree)
	: m(new Impl(this, ptree)), Process(owner, ptree)
{
	set(ptree);
	m->view.init();
}

Composition::~Composition()
{

}

/**************************
PropertyTrees
***************************/
PropertyTree Composition::get() const
{
	PropertyTree ptree = Process::get();

	ptree.put_value("Composition");
	ptree.put("IterationCount", m->iterationCount);

	for (auto & variable : m->variables)
		ptree.add_child("Elements.Element", variable.get());
	for (auto & pureFunction : m->pureFunctions)
		ptree.add_child("Elements.Element", pureFunction.get());
	for (auto & codeFunction : m->codeFunctions)
		ptree.add_child("Elements.Element", codeFunction.get());
	for (auto & composition : m->compositions)
		ptree.add_child("Elements.Element", composition.get());
	for (auto & link : m->links)
		ptree.add_child("Links.Link", link.get());

	ptree.put_child("View", m->view.get());

	return ptree;
}
void Composition::set(PropertyTree & ptree)
{
	checkHierarchy("Composition", QString::fromStdString(ptree.get_value<std::string>()));

	m->iterationCount = 1;
	if (ptree.count("IterationCount") != 0)
		m->iterationCount = ptree.get<uint8>("IterationCount");

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
List<Variable>		&	Composition::variables() { return m->variables; }
List<PureFunction>	&	Composition::pureFunctions() { return m->pureFunctions; }
List<CodeFunction>	&	Composition::codeFunctions() { return m->codeFunctions; }
List<Composition>	&	Composition::compositions() { return m->compositions; }
List<Link>			&	Composition::links() { return m->links; }
uint8					Composition::iterationCount() { return m->iterationCount; }

/**************************
Inserter
***************************/
Component* Composition::addElement(PropertyTree & ptree)
{
	if (QString::fromStdString(ptree.get_value<std::string>()) == "Variable")
	{
		m->variables.emplace_back(this, ptree);
		return &m->variables.back();
	}
	else if (QString::fromStdString(ptree.get_value<std::string>()) == "PureFunction")
	{
		m->pureFunctions.emplace_back(this, ptree);
		return &m->pureFunctions.back();
	}
	else if (QString::fromStdString(ptree.get_value<std::string>()) == "CodeFunction")
	{
		m->codeFunctions.emplace_back(this, ptree);
		return &m->codeFunctions.back();
	}
	else if (QString::fromStdString(ptree.get_value<std::string>()) == "Composition")
	{
		m->compositions.emplace_back(this, ptree);
		return &m->compositions.back();
	}

	return nullptr;
}

Link* Composition::addLink(PropertyTree & ptree)
{
	m->links.emplace_back(this, ptree);
	return &m->links.back();
}

/**************************
Setter
***************************/
void Composition::setIterationCount(uint8 _iterationCount) 
{
	m->iterationCount = _iterationCount;
}

/**************************
Deleter
***************************/

void Composition::removeComponent(QString const & name)
{
	auto check = [&name](Component & t){ return t.name() == name; };
	m->pureFunctions.remove_if(check);
	m->codeFunctions.remove_if(check);
	m->variables.remove_if(check);
	m->compositions.remove_if(check);

}
void Composition::removeLink(Link * link)
{
	m->links.remove(*link);
}

/**************************
Find
***************************/
Component * Composition::find(QString const & name)
{
	for (Component & current : m->variables)
		if (current.name() == name) return &current;
	for (Component & current : m->codeFunctions)
		if (current.name() == name) return &current;
	for (Component & current : m->pureFunctions)
		if (current.name() == name) return &current;
	for (Component & current : m->compositions)
		if (current.name() == name) return &current;

	return nullptr;
}

Interface::CompositionView * Composition::view()
{
	return &m->view;
}

}