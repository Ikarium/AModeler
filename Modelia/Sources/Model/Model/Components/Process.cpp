#include "Conf.h"

#include "Process.h"

namespace Model
{

class Process::Impl
{
	Process* w;

public:

	QString code;


	Impl(Process*, PropertyTree &);
	~Impl();

};

Process::Impl::Impl(Process* owner, PropertyTree & ptree)
	: w(owner)
{

}

Process::Impl::~Impl()
{

}

Process::Process(Composition * owner, PropertyTree & ptree)
	: m(new Impl(this, ptree)), Component(owner, ptree)
{
	import(ptree);
}

Process::~Process() {}

/**************************
PropertyTrees
***************************/
PropertyTree Process::export() const
{
	PropertyTree ptree = Component::export();

	ptree.put_value("Process");

	for (Slot const & input : inputs())
		ptree.add_child("Inputs.Slot", input.export());
	for (Slot const & output : outputs())
		ptree.add_child("Outputs.Slot", output.export());

	return ptree;
}

void Process::import(PropertyTree & ptree)
{
	checkHierarchy("Process", QString::fromStdString(ptree.get_value<std::string>()));

	if (ptree.count("Inputs") != 0)
		for (auto input : ptree.get_child("Inputs"))
			addInput(input.second);
	if (ptree.count("Outputs") != 0)
		for (auto output : ptree.get_child("Outputs"))
			addOutput(output.second);
}

/**************************
Slot accessors
***************************/
Slot * Process::addInput(PropertyTree & ptree)
{
	ptree.put("UniqueLink", true);
	return Component::addInput(ptree);
}

Slot * Process::addOutput(PropertyTree & ptree)
{
	ptree.put("UniqueLink", false);
	return Component::addOutput(ptree);
}


}