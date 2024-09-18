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
		MessageBoxA(NULL, "M2TWEOP cannot find medieval2.exe or kingdoms.exe. You probably installed the mod in the wrong folder. The file structure should look something 'Medieval II Total War/mods/<mod_folder>'", "Error!", NULL);
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
			MessageBoxA(NULL, "Failed to start M2TWEOP. Please try running again with Adminstrator rights.", "ERROR", MB_OK);
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
void gameStarter::ensureFiles(std::vector<std::wstring>& newFiles, std::vector<std::wstring>& oldFiles)
{
	using namespace boost::filesystem;
	for (size_t i = 0; i < newFiles.size(); ++i)
	{
		// Get the full paths
		path newFilePath = system_complete(newFiles[i]);
		path oldFilePath = system_complete(oldFiles[i]);

		std::wstring newFileStr = newFilePath.wstring();
		std::wstring oldFileStr = oldFilePath.wstring();

		if (helpers::compareFiles(oldFileStr, newFileStr) == false)
		{
			if (CopyFileW(newFileStr.c_str(), oldFileStr.c_str(), FALSE) == false)
			{
				DWORD ERR = GetLastError();
				std::wstring errorMsg = L"Cannot run M2TWEOP, error replacing " + newFiles[i] + L"! Try deleting it in the game folder or copying it manually from the M2TWEOP archive AND START M2TWEOP WITH ADMIN RIGHTS IF IT STILL DOESN'T WORK.";
				MessageBoxW(NULL, errorMsg.c_str(), L"ERROR", MB_OK);
				exit(0);
			}
		}
	}

	return;
}

bool gameStarter::initM2TWEOP()
{
	using namespace boost::filesystem;
	if (dataG::data.modData.useM2TWEOP == false)
	{
		return true;
	}

	std::vector<std::wstring> newFiles = {
	L"d3d9_vk.dll",
	L"d3d9.dll",
	L"libfbxsdk.dll",
	L"lua5.1.dll",
	L"openal32.dll",
	L"discord_game_sdk.dll",
	L"avcodec-61.dll",
	L"avfilter-10.dll",
	L"avformat-61.dll",
	L"avutil-59.dll",
	L"swresample-5.dll",
	L"swscale-8.dll",
	};

	std::vector<std::wstring> oldFiles = {
		L"..\\..\\d3d9_vk.dll",
		L"..\\..\\d3d9.dll",
		L"..\\..\\libfbxsdk.dll",
		L"..\\..\\lua5.1.dll",
		L"..\\..\\openal32.dll",
		L"..\\..\\discord_game_sdk.dll",
		L"..\\..\\avcodec-61.dll",
		L"..\\..\\avfilter-10.dll",
		L"..\\..\\avformat-61.dll",
		L"..\\..\\avutil-59.dll",
		L"..\\..\\swresample-5.dll",
		L"..\\..\\swscale-8.dll",
	};
	ensureFiles(newFiles, oldFiles);

	return true;
}
