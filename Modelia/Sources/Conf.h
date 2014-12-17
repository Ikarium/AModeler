#pragma once;

#define DEVELOPPEMENT

#define DEBUG
#define ASSERT
#define EXEPTION
#define GAME_LOOP_EXEPTION
#define EXEPTION_LEVEL -1

#define LOG_LEVEL -1
#define LOG_DEFAULT_LEVEL 3
#define LOG_DEFAULT_CONSOLE_TYPE notice

#define SYSTEM_TARGET x64

#define NOMINMAX

#include <QDebug>
#include <Karma>
using namespace Karma;

#include "Declarator.h"

/***************************************************************************
ModeliaExection
**************************************************************************/

struct ModeliaExection : public KExeption
{
	ModeliaExection(QString	const & file = "",
		int		const & line = 0,
		QString	const & message = "",
		int		const & code = 0)
		: KExeption("\nIn: " + file + "\t" + QString::number(line)
		+ "\n   Modelia exception: " + message + "\n", code){}
};


inline PropertyTree simpleType() 
{ 
	PropertyTree simpleType;
	infoToPree("SimpleType.ATypes", simpleType);
	return simpleType;
}
