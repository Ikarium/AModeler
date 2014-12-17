#pragma once

namespace Model
{

class Compiler
{
public:
	Compiler();

	~Compiler();

	QString aModelToPhython(Composition *);
	QVector<Process*> topSort(Composition *, bool &);

private:
	class Impl;
	UniquePtr<Impl> m;
};

}
