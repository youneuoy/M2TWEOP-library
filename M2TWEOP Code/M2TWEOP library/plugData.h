#pragma once
#ifndef plagDataH_DEF
#define plagDataH_DEF
#include "luaP.h"
class plugData
{
public:
	static struct pDat
	{
		std::string modFolder;
		luaP luaAll;
	}
	data;
};


#endif