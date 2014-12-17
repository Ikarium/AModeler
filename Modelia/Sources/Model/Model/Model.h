#pragma once

#include "Components\Composition.h"

namespace Model
{

class Model
{
public:
	Model(PropertyTree &);

	Composition & goTo(QList<QString>);
	Composition & root();
	~Model();

private:
    class Impl;
	UniquePtr<Impl> m;
};

}
