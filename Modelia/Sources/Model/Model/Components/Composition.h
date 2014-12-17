#pragma once

#include "Process.h"

#include "Interface/Components/CompositionView.h"

namespace Model
{

class Composition : public Process
{

	Interface::CompositionView view_;

	List<Link> links_;
	List<Variable> variables_;
	List<PureFunction> pureFunctions_;
	List<CodeFunction> codeFunctions_;
	List<Composition> compositions_;

	uint8 iterationCount_;

public:
	Composition(Composition *, PropertyTree &);

/**************************
PropertyTrees
***************************/
	PropertyTree get() const;

	void set(PropertyTree & ptree);

/**************************
Getters
***************************/
	List<Variable>		&	variables();
	List<PureFunction>	&	pureFunctions();
	List<CodeFunction>	&	codeFunctions();
	List<Composition>	&	compositions();
	List<Link>			&	links();
	uint8					iterationCount();

/**************************
Inserter
***************************/
	Component* addElement(PropertyTree &);
	Link* addLink(PropertyTree &);

/**************************
Setter
***************************/
	void setIterationCount(uint8);

/**************************
Deleter
***************************/
	void removeComponent(QString const &);
	void removeLink(Link *);

/**************************
Find
***************************/
	Component * find(QString const &);

	virtual Interface::CompositionView * view() override;

};

}