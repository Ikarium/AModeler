#pragma once

#include "Model/Model/Model.h"

#include <boost/property_tree/info_parser.hpp>
namespace Interface
{

class ModelInterface
{
	Model::Model * model_ = nullptr;

public:

	ModelInterface()
	{

	}

	void loadModel(QString path)
	{
		PropertyTree ptree;
		boost::property_tree::info_parser::read_info(path.toStdString(), ptree);

		if (QString::fromStdString(ptree.get<std::string>("FileType")) != "Model")
			KThrow(ModeliaExection, lvl6, "Bad library ptree");

		App::typesLibrary->removeItem(App::typesLibrary->root(), "Model");
		ptree.put("TypesLibrary", "TypesLibraryItem");
		ptree.put("TypesLibrary.Name", "Model");
		ptree.put("TypesLibrary.EditSelfPerm", false);
		ptree.put("TypesLibrary.EditContentPerm", true);
		ptree.put("TypesLibrary.Expanded", true);
		App::typesLibrary->insertItem(App::typesLibrary->root(), ptree.get_child("TypesLibrary"));

		App::componentsLibrary->removeItem(App::componentsLibrary->root()->child("Model"));
		ptree.put("ComponentLibrary", "ComponentLibraryItem");
		ptree.put("ComponentLibrary.Name", "Model");
		ptree.put("ComponentLibrary.Expanded", true);
		App::componentsLibrary->insertItem(App::componentsLibrary->root(), ptree.get_child("ComponentLibrary"));

		if (model_) delete model_;
		model_ = new Model::Model(ptree.get_child("Model"));
	}

	void saveModel(QString path)
	{
		PropertyTree ptree;

		ptree.put("FileType", "Model");

		ptree.put_child("TypesLibrary", App::typesLibrary->root()->child("Model")->export());
		ptree.get_child("TypesLibrary").erase("Name");
		ptree.get_child("TypesLibrary").erase("EditSelfPerm");
		ptree.get_child("TypesLibrary").erase("EditContentPerm");
		ptree.get_child("TypesLibrary").erase("Expanded");

		ptree.put_child("ComponentLibrary", App::componentsLibrary->root()->child("Model")->save());
		ptree.get_child("ComponentLibrary").erase("Name");
		ptree.get_child("ComponentLibrary").erase("Expanded");

		ptree.put_child("Model", model_->root().export());

		boost::property_tree::info_parser::write_info(path.toStdString(), ptree);
	}

	Model::Model * model() { return model_; }

};

}
