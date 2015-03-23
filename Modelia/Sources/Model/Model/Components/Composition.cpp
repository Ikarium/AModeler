#include "Conf.h"

#include "Composition.h"
#include "Variable.h"
#include "Function.h"
#include "Link.h"

namespace Model
{

class Composition::Impl
{
	Composition* w;

public:

	Interface::CompositionView view;

	List<Link> links;
	List<Variable> variables;
	List<Function> functions;
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
	import(ptree);
	m->view.init();
}

Composition::~Composition()
{

}

/**************************
PropertyTrees
***************************/
PropertyTree Composition::export() const
{
	PropertyTree ptree = Process::export();

	ptree.put_value("Composition");
	ptree.put("IterationCount", m->iterationCount);

	for (auto & variable : m->variables)
		ptree.add_child("Elements.Element", variable.export());
	for (auto & function : m->functions)
		ptree.add_child("Elements.Element", function.export());
	for (auto & composition : m->compositions)
		ptree.add_child("Elements.Element", composition.export());
	for (auto & link : m->links)
		ptree.add_child("Links.Link", link.export());

	ptree.put_child("View", m->view.export());

	return ptree;
}
void Composition::import(PropertyTree & ptree)
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
List<Component*>	Composition::components()
{ 
	List<Component*> result;

	for (auto & variable : m->variables)
		result.push_back(&variable);
	for (auto & function : m->functions)
		result.push_back(&function);
	for (auto & composition : m->compositions)
		result.push_back(&composition);

	return result;
}

List<Variable>		&	Composition::variables() { return m->variables; }
List<Function>		&	Composition::functions() { return m->functions; }
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
	else if (QString::fromStdString(ptree.get_value<std::string>()) == "Function")
	{
		m->functions.emplace_back(this, ptree);
		return &m->functions.back();
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
	m->functions.remove_if(check);
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
	for (Component * current : components())
		if (current->name() == name) return current;

	return nullptr;
}

Interface::CompositionView * Composition::view()
{
	return &m->view;
}

}