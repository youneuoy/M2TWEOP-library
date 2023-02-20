#pragma once
#include "realGameTypes.h"
#include "headersSTD.h"
#include "BattleMapOverrider.h"
class DeveloperMode
{
public:
	void Update();
	bool isDeveloperModeNeeded = true;

	string SelectWorldpkgdesc(const std::string& selectedRec, const std::string& selectedGroup);
private:
	bool isWork = false;

	struct
	{
		bool isMapManager = false;
		bool isBattleMapOverrider = false;
	}subsState;

	struct
	{
		BattleMapOverrider battleMapOverrider;
	}subs;
};

