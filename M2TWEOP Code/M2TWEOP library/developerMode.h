#pragma once
#include "headersSTD.h"
#include "battleMapOverrider.h"

class developerMode
{
public:
	void update();
	bool isDeveloperModeNeeded = true;
	void toggleDeveloperModeBase();
	static void toggleDeveloperMode();

	string selectWorldpkgdesc(const std::string& selectedRec, const std::string& selectedGroup);
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

	static bool draw();
};

