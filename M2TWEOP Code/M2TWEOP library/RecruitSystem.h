#pragma once
#include "realGameTypes.h"
#include "headersSTD.h"
#include "BattleMapOverrider.h"
#include "RecruitPlace.h"
#include <mutex>
#include "RecruitSystemConfig.h"
class RecruitSystem
{
public:
	void Init();
private:
	//callback
	void AtInit(int status);
	typedef vector<shared_ptr<RecruitPlace>>recruitPlaces;

	struct
	{
		//use it always where acess anything from this struct
		mutex placesMutex;

		recruitPlaces data;
	}places;

	RecruitSystemConfig config;
};

