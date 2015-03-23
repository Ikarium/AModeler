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
		ptree.put_value("Composition");
		if (model_) delete model_;
		model_ = new Model::Model(ptree);
	}

	void saveModel(QString path)
	{
		boost::property_tree::info_parser::write_info(path.toStdString(), model_->root().export());
	}

	Model::Model * model() { return model_; }

};

}
