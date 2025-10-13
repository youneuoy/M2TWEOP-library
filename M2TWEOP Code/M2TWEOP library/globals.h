#pragma once
#include "headersSTD.h"

#include "battlemapWorker.h"
#include "developerMode.h"
#include "mmbMover.h"
#include "tacticalMapViewer.h"
#include "contextMenuStrat.h"
class globals
{
public:
	static struct dataGlob
	{
		struct developerS
		{

		};

		struct
		{
			bool isBlockLaunchWithoutEop = false;
			bool isDiscordRichPresenceEnabled = true;
			string launcherTheme = "default";
			string modVersion = "";
			bool isSaveBackupEnabled = true;
		}gameCfg;
		struct modulesS
		{
			tacticalMapViewer tacticalMapViewer;
			developerMode developerMode;
			battlemapWorker battleMapWorker;
			MmbMover mmbMover;
			ContextMenuStrat contextMenuStrat;
		}Modules;

		//steam =2, kingdoms=1
		UINT32 gameVersion;
		string modPath;
		string modFolderName{};

		struct
		{
			//enable full logging(file all.youneuoylog)
			int fullLogging = 0;

			//unlock console command change faction in singleplayer game
			int unlockChangeFaction = 0;

			//berserkers crach fix
			int bersPatch = 1;

			//online history battles fix
			int histBattleFix = 0;

			//creatures fix
			int DCI = 0;

			//allow spawn_army changes
			int spawnArmyCoords = 0;

			//character ui functions
			int characterUIMenus = 1;

			//unlock console commands
			int unlockConsoleCommands = 0;

			//bodyguard replenish max count
			int maxBodSize = 31;

			//language path
			string langP;

			//legio string for soldiers
			char* legioString=nullptr;
		}modConfig;


		struct
		{
			//religions limit
			int religionLimit = 10;
			//ancillaries limit
			int ancillariesLimit = 8;
			//disable unit size settings in game
			int lockSizeOfUnit = 0;
			//set size of unit
			int sizeOfUnit = 0;
			//minimal soldiers number in edu
			INT16 minSoldiersEdu = 4;
			//maximal soldiers number in edu
			INT16 maxSoldiersEdu = 100;
			//battlefield map size x axis
			float battlefieldMapSizeX = 1728;
			//battlefield map size y axis
			float battlefieldMapSizeY = 1728;
			//siege ram cost
			UINT32 siegeRamCost = 50;
			//siege ladder cost
			UINT32 siegeLadderCost = 15;
			//siege tower cost
			UINT32 siegeTowerCost = 65;
		}limits;

		struct
		{
			int32_t bLimitRecruitmentToSlotsAndByMoney = 0;
		}eopGameplaySettings;

	}dataS;
};

