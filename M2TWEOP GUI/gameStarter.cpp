#include "gameStarter.h"

#include "pipe.h"
bool gameStarter::startGame()
{
//SetCurrentDirectoryA("C:\\Program Files (x86)\\Steam\\steamapps\\common\\Medieval II Total War\\mods\\teutonic");
	string gameStartArgs;

	if (dataG::data.gameData.gameMode == 0)
	{
		return false;
	}

	if (helpers::selectGameExe(dataG::data.gameData.gameMode) == false)
	{
		MessageBoxA(NULL, "Cannot select exe file! Need correct medieval2 or kingdoms", "Error!", NULL);
		return false;
	}
	gameStartArgs += dataG::data.gameData.exeName + ' ';

	// vanilla
	if (dataG::data.gameData.gameMode == 1)
	{
		if (!runGameExe())
		{
			return false;
		}
	}
	//dlc
	else if (dataG::data.gameData.gameMode < 6)
	{
		helpers::addModPathArg(gameStartArgs, dataG::data.gameData.gameMode);
		dataG::data.gameData.gameArgs = gameStartArgs;
		if (!runGameExe())
		{
			return false;
		}
	}
	//mod
	else
	{
		if (initM2TWEOP() == false)
		{
			MessageBoxA(NULL, "Cannot init M2TWEOP.", "ERROR", MB_OK);
			exit(0);
		}
		if (dataG::data.modData.useVanillaConfig == true)
		{
			dataG::data.gameData.gameArgs += " --features.mod=mods/";
			dataG::data.gameData.gameArgs += dataG::data.gameData.modFolder;
			if (!runGameExe())
			{
				return false;
			}
		}
		else
		{
			dataG::data.gameData.gameArgs += " @mods\\";
			dataG::data.gameData.gameArgs += dataG::data.gameData.modFolder;
			dataG::data.gameData.gameArgs += '\\';
			dataG::data.gameData.gameArgs += dataG::data.modData.configName;


			if (!runGameExe())
			{
				return false;
			}
		}

	}
	return true;
}

bool gameStarter::runGameExe()
{
	if (dataG::data.modData.useM2TWEOP == true)
	{
		string currentPath;
		helpers::getCurrentPath(currentPath);

		dataG::data.gameData.gameArgs += "m2tweopStartCommand";
		dataG::data.gameData.gameArgs += to_string(dataG::data.gameData.gameVer);

		dataG::data.gameData.gameArgs += "eopModFolder=*";
		dataG::data.gameData.gameArgs += currentPath;
		dataG::data.gameData.gameArgs += "*";

		dataG::data.gameData.gameArgs += "eopGameVer=*";
		dataG::data.gameData.gameArgs += to_string(dataG::data.gameData.gameVer);
		dataG::data.gameData.gameArgs += "*";
	}


	helpers::runGame(dataG::data.gameData.gamePath.c_str(), dataG::data.gameData.gameArgs.c_str());

	exit(0);

	return false;
}

bool gameStarter::initM2TWEOP()
{
	if (dataG::data.modData.useM2TWEOP == false)
	{
		return true;
	}

	string wrapd3dS = "d3d9.dll";
	string d3dS = "..\\..\\d3d9.dll";
	if (helpers::compareFiles(d3dS, wrapd3dS)==false)
	{
		if (CopyFileA(wrapd3dS.c_str(), d3dS.c_str(), FALSE) == false)
		{
			MessageBoxA(NULL, "Cannot run M2TWEOP, d3d9.dll replasing error! Try to delete d3d9.dll in game folder or copy d3d.dll from M2TWEOP archive. ", "ERROR", MB_OK);
			exit(0);
		}
	}

	return true;
}
