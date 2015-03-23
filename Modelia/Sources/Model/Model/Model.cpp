#include "Conf.h"

#include "Model.h"

#include "Components/Variable.h"
#include "Components/Function.h"
#include "Components/Composition.h"
#include "Components/Link.h"

namespace Model
{

class Model::Impl
{
	Model* owner;
public:
	Composition root;

	Impl(Model* owner_, PropertyTree & ptree) : owner(owner_), root(nullptr, ptree)
	{

	}

	~Impl();

};
Model::Impl::~Impl() {}

Model::Model(PropertyTree & ptree) : m(new Impl(this, ptree))
{

}

Model::~Model()
{

}

Composition & Model::root()
{
	return m->root;
}

Composition & Model::goTo(QList<QString> path)
{
	Composition * current = &m->root;
	for (QString & name : path)
		current = dynamic_cast<Composition *>(current->find(name));

	return *current;
}

}
