#pragma once
#include "realGameTypes.h"
#include "headersSTD.h"
#include "BattleMapOverrider.h"
#include "RecruitPlace.h"
#include <mutex>
class RecruitSystemConfig
{
public:
	void StartLoad(const std::wstring& path,function<void(int)> callback);
private:
	wstring ourPath;
	void LoadJsons();
};

