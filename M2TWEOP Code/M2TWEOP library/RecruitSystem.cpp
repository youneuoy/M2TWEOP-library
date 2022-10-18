#include "RecruitSystem.h"
#include "globals.h"
#include "fastFunctsHelpers.h"

void RecruitSystem::Init()
{
	scoped_lock placLock{places.placesMutex};

	using namespace std::placeholders; //
	config.StartLoad(fastFunctsHelpers::ANSIStrToWstr(globals::dataS.modPatch)+L"\\eopData\\RecruitSystem", std::bind(&RecruitSystem::AtInit, this, _1));
}


void RecruitSystem::AtInit(int status)
{
}
