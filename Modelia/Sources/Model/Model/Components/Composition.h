#pragma once

#include "Process.h"

#include "Interface/Components/CompositionView.h"

namespace Model
{

class Composition : public Process
{

public:
	Composition(Composition *, PropertyTree &);
	~Composition();

/**************************
PropertyTrees
***************************/
	PropertyTree export() const;
	void import(PropertyTree & ptree);

/**************************
Getters
***************************/
	List<Component*>		components();
	List<Variable>		&	variables();
	List<Function>		&	functions();
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

private:
	class Impl;
	UniquePtr<Impl> m;

};

}