#pragma once
#ifndef plagDataH_DEF
#define plagDataH_DEF
#include "framework.h"
#include "basicFuncs.h"
#include "luaP.h"
#define basicFunc plugData::data.funcs
class plugData
{
public:
	static struct pDat
	{
		std::string modFolder;
		basicFuncs funcs;
		technicalFuncs funcsTech;
		battleFuncs funcsBattle;
		eopEduFuncs funcsEopEdu;
		eopFbxFuncs functsEopFbx;
		eopSoundsFuncs functsEopSounds;


		gameUIFuncs functsGameUI;


		luaP luaAll;
	}
	data;
};


#endif