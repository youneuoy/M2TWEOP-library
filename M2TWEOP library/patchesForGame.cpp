#include "pch.h"
#include "patchesForGame.h"
#include "tilesChange.h"
#include "graphicsD3D.h"
void __stdcall patchesForGame::afterEDUread()
{
	if (!graphicsD3D::InitS())
		{
			MessageBoxA(NULL, "Cant run M2TWEOP, graphics error", "ERROR", NULL);
		}
}

void __stdcall patchesForGame::onChangeTurnNum()
{
}

void __stdcall patchesForGame::onGiveTrait()
{
}

void __stdcall patchesForGame::afterLoadGameData()
{
}

void __stdcall patchesForGame::afterI_CompareCounter()
{
}

int __stdcall patchesForGame::checkFort()
{
	return 0;
}

int __stdcall patchesForGame::checkDipString()
{
	return 0;
}

int __stdcall patchesForGame::checkFacDip()
{
	return 0;
}

int __stdcall patchesForGame::spawnArmyCoords()
{
	return 0;
}

void __stdcall patchesForGame::spawnEndArmyCoords()
{
}

int __stdcall patchesForGame::spawnCharacterCoords()
{
	return 0;
}

void __stdcall patchesForGame::checkAndChangeCharacterBattleModels()
{
}

void __fastcall patchesForGame::onEvent(DWORD** vTab)
{
	plugins::onEvent(vTab);
}

void __fastcall patchesForGame::onTileCheck(int* coords)
{
	if (coords == nullptr)
	{
		MessageBoxA(NULL, "Error in onTileCheck", "Attention", NULL);
		return;
	}

	tilesChange::onTilesCheck(coords);
}

void __stdcall patchesForGame::readTilesFile()
{
}

void __stdcall patchesForGame::unicalStratModsRead()
{
	stratModelsChange::loadModels();
}


void __stdcall patchesForGame::checkAndChangeModels()
{
	stratModelsChange::checkAndChangeStratModels();
}
