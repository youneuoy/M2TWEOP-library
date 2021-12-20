#include "gameStarter.h"

#include "gameRunnerUI.h"
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
			MessageBoxA(NULL, "Cannot init M2TWEOP. START M2TWEOP WITH ADMIN RIGHTS IF IT NOT WORK.", "ERROR", MB_OK);
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
	string eopArgs;
	bool isEopNeeded = dataG::data.modData.useM2TWEOP;
	if (isEopNeeded == true)
	{

		string currentPath;
		helpers::getCurrentPath(currentPath);

		eopArgs = helpers::makeFString(
			"%s\n%s\n%s\n%s\n%s",
			"m2tweopStartCommand",
			"eopModFolder:",
			currentPath.c_str(),
			"GameVer:",
			to_string(dataG::data.gameData.gameVer).c_str()
		);

	}

	//mod
	if (dataG::data.gameData.gameMode == 7 && isEopNeeded)
	{
		gameRunnerUI::setRunParams(dataG::data.gameData.gamePath, dataG::data.gameData.gameArgs, eopArgs, isEopNeeded);
	}
	else
	{
		helpers::runGame(dataG::data.gameData.gamePath.c_str(), dataG::data.gameData.gameArgs.c_str());
		exit(0);
	}

	return true;
}

bool gameStarter::initM2TWEOP()
{
	using namespace boost::filesystem;
	if (dataG::data.modData.useM2TWEOP == false)
	{
		return true;
	}

	path wrapd3dS = system_complete("d3d9.dll");

	path d3dS = system_complete("..\\..\\d3d9.dll");
	string wrapd3dStr = wrapd3dS.string();
	string d3dStr = d3dS.string();

	if (helpers::compareFiles(d3dStr, wrapd3dStr) == false)
	{

		if (CopyFileA(wrapd3dStr.c_str(), d3dStr.c_str(), FALSE) == false)
		{
			DWORD ERR = GetLastError();
			MessageBoxA(NULL, "Cannot run M2TWEOP, d3d9.dll replacing error! Try to delete d3d9.dll in game folder or copy d3d.dll from M2TWEOP archive AND START M2TWEOP WITH ADMIN RIGHTS IF IT STILL NOT WORK AFTER THIS. ", "ERROR", MB_OK);
			exit(0);
		}
	}

	path newFbxS = system_complete("libfbxsdk.dll");
	path fbxS = system_complete("..\\..\\libfbxsdk.dll");

	string newFbxStr = newFbxS.string();
	string fbxStr = fbxS.string();
	if (helpers::compareFiles(fbxStr, newFbxStr) == false)
	{

		if (CopyFileA(newFbxStr.c_str(), fbxStr.c_str(), FALSE) == false)
		{
			DWORD ERR = GetLastError();
			MessageBoxA(NULL, "Cannot run M2TWEOP, libfbxsdk.dll replacing error! Try to delete libfbxsdk.dll in game folder or copy libfbxsdk.dll from M2TWEOP archive AND START M2TWEOP WITH ADMIN RIGHTS IF IT STILL NOT WORK AFTER THIS. ", "ERROR", MB_OK);
			exit(0);
		}
	}

	return true;
}
