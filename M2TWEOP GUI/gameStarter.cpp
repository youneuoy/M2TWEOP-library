#include "gameStarter.h"



bool gameStarter::startGame()
{
	string gameStartArgs;

	if (dataG::data.gameData.gameMode == 0)
	{
			return false;
	}
	
	if (helpers::selectGameExe(dataG::data.gameData.gameMode)==false)
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

	}
	return true;
}

bool gameStarter::runGameExe()
{
	return helpers::runGame(dataG::data.gameData.gamePath.c_str(), dataG::data.gameData.gameArgs.c_str());
}
