#pragma once

#include "Conf.h"
#include<QWidget>

namespace Interface
{

class TypeEditor : public QWidget
{
	Q_OBJECT

public:
	TypeEditor();
	~TypeEditor();

	void fill();

public slots:
	void selectionChanged(Model::TypesLibraryItem *, Model::TypesLibraryItem *);
	void nameValidator();
	void aliasTypeValidator();
	void aliasVectorizeValidator(int);
	void addStructure();
	void addAlias();
	void addNamesSpace();
	void addField();
	void fieldNameValidator();
	void fieldTypeValidator();
	void fieldVectorValidator(int);

private:
	class Impl;
	UniquePtr<Impl> m;
};

}
