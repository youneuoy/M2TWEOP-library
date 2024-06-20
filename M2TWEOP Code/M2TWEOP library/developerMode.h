#pragma once
#include "headersSTD.h"
#include "battleMapOverrider.h"

class DeveloperMode
{
public:
	void Update();
	bool isDeveloperModeNeeded = true;
	void toggleDeveloperModeBase();

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
void toggleDeveloperMode();

