#include "Conf.h"

#include "Process.h"

namespace Model
{

Process::Process(Composition * owner, PropertyTree & ptree)
	: Component(owner, ptree)
{ set(ptree); }

/**************************
PropertyTrees
***************************/
PropertyTree Process::get() const
{
	PropertyTree ptree = Component::get();

	ptree.put_value("Process");

	for (Slot const & input : inputs_)
		ptree.add_child("Inputs.Slot", input.get());
	for (Slot const & output : outputs_)
		ptree.add_child("Outputs.Slot", output.get());

	return ptree;
}

void Process::set(PropertyTree & ptree)
{
	checkHierarchy("Process", QString::fromStdString(ptree.get_value<std::string>()));

	if (ptree.count("Inputs") != 0)
		for (auto input : ptree.get_child("Inputs"))
			addInput(input.second);
	if (ptree.count("Outputs") != 0)
		for (auto output : ptree.get_child("Outputs"))
			addOutput(output.second);
}

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