// pluginM2TWEOP.cpp : Определяет экспортируемые функции для DLL.
//

#include "pluginM2TWEOP.h"

#include "lua/sol.hpp"

#include "imgui/sol_ImGui.h"

int initPlugin(std::string* modPath)
{
	plugData::data.modFolder = *modPath;

	std::string plPath = plugData::data.modFolder;
	plPath = plPath + "\\M2TWEOPLibrary.dll";


	//onChangeTurnNum function
   /* std::string fName = "getYear";

	plugData::data.funcs.getYear.Load(&plPath, &fName);

	fName = "getBattleStacksListP";
	plugData::data.funcs.getBattleArmies.Load(&plPath, &fName);
	*/
	std::string fName = "isTileFree";
	plugData::data.funcs.isTileFree.Load(&plPath, &fName);


	fName = "setModel";
	plugData::data.funcs.setModel.Load(&plPath, &fName);

	fName = "MakeTextFont";
	plugData::data.funcs.MakeTextFont.Load(&plPath, &fName);
	fName = "DeleteTextFont";
	plugData::data.funcs.DeleteTextFont.Load(&plPath, &fName);
	fName = "MakeText";
	plugData::data.funcs.MakeText.Load(&plPath, &fName);
	fName = "Delete3dText";
	plugData::data.funcs.Delete3dText.Load(&plPath, &fName);
	fName = "ScaleText";
	plugData::data.funcs.ScaleText.Load(&plPath, &fName);
	fName = "ChangeTextColor";
	plugData::data.funcs.ChangeTextColor.Load(&plPath, &fName);
	fName = "SetTextDrawingCoords";
	plugData::data.funcs.SetTextDrawingCoords.Load(&plPath, &fName);
	fName = "StartDrawingText";
	plugData::data.funcs.StartDrawingText.Load(&plPath, &fName);
	fName = "StopDrawingText";
	plugData::data.funcs.StopDrawingText.Load(&plPath, &fName);
	fName = "DrawingTextOnce";
	plugData::data.funcs.DrawingTextOnce.Load(&plPath, &fName);
	fName = "SetDimensionsTextXYZ";
	plugData::data.funcs.SetDimensionsTextXYZ.Load(&plPath, &fName);
	fName = "SetRotationTextXYZ";
	plugData::data.funcs.SetRotationTextXYZ.Load(&plPath, &fName);

	fName = "addModelToGame";
	plugData::data.funcs.addModelToGame.Load(&plPath, &fName);
	fName = "addCasModelToDrawList";
	plugData::data.funcs.addCasModelToDrawList.Load(&plPath, &fName);
	fName = "removeCasModelFromDrawList";
	plugData::data.funcs.removeCasModelFromDrawList.Load(&plPath, &fName);
	fName = "setCharacterModel";
	plugData::data.funcs.setCharacterModel.Load(&plPath, &fName);
	fName = "addCharacterCas";
	plugData::data.funcs.addCharacterCas.Load(&plPath, &fName);




	fName = "GetGameTileCoordsWithCursor";
	plugData::data.funcs.getGameTileCoordsWithCursor.Load(&plPath, &fName);

	fName = "getTileRegionID";
	plugData::data.funcs.getTileRegionID.Load(&plPath, &fName);

	fName = "getRegionByID";
	plugData::data.funcs.getRegionByID.Load(&plPath, &fName);

	fName = "findFort";
	plugData::data.funcs.findFort.Load(&plPath, &fName);

	fName = "findSettlement";
	plugData::data.funcs.findSettlement.Load(&plPath, &fName);

	fName = "deleteFort";
	plugData::data.funcs.deleteFort.Load(&plPath, &fName);

	fName = "createFortXY";
	plugData::data.funcs.createFortXY.Load(&plPath, &fName);

	fName = "createFort";
	plugData::data.funcs.createFort.Load(&plPath, &fName);

	fName = "getRegionOwner";
	plugData::data.funcs.getRegionOwner.Load(&plPath, &fName);

	fName = "getGameDataAll";
	plugData::data.funcs.getGameDataAll.Load(&plPath, &fName);

	fName = "setEDUUnitsSize";
	plugData::data.funcs.setEDUUnitsSize.Load(&plPath, &fName);

	fName = "setHeir";
	plugData::data.funcs.setHeir.Load(&plPath, &fName);

	fName = "callGameConsoleCommand";
	plugData::data.funcs.callGameConsoleCommand.Load(&plPath, &fName);

	fName = "scriptCommand";
	plugData::data.funcs.scriptCommand.Load(&plPath, &fName);

	fName = "getScriptCounter";
	plugData::data.funcs.getScriptCounter.Load(&plPath, &fName);

	fName = "setScriptCounter";
	plugData::data.funcs.setScriptCounter.Load(&plPath, &fName);

	fName = "changeSettlementName";
	plugData::data.funcs.changeSettlementName.Load(&plPath, &fName);

	fName = "getFactionName";
	plugData::data.funcs.getFactionName.Load(&plPath, &fName);

	fName = "historicEvent";
	plugData::data.funcs.historicEvent.Load(&plPath, &fName);

	fName = "changeFactionName";
	plugData::data.funcs.changeFactionName.Load(&plPath, &fName);

	fName = "changeRegionName";
	plugData::data.funcs.changeRegionName.Load(&plPath, &fName);

	fName = "GetMovepointsForReachNearTile";
	plugData::data.funcs.GetMovepointsForReachNearTile.Load(&plPath, &fName);

	fName = "condition";
	plugData::data.funcs.condition.Load(&plPath, &fName);

	fName = "allocateGameMem";
	plugData::data.funcs.allocateGameMem.Load(&plPath, &fName);

	fName = "changeRebelsName";
	plugData::data.funcs.changeRebelsName.Load(&plPath, &fName);

	fName = "getGameVersion";
	plugData::data.funcs.getGameVersion.Load(&plPath, &fName);

	fName = "saveGame";
	plugData::data.funcs.saveGame.Load(&plPath, &fName);

	fName = "mergeArmies";
	plugData::data.funcs.mergeArmies.Load(&plPath, &fName);

	fName = "spawnArmy";
	plugData::data.funcs.spawnArmy.Load(&plPath, &fName);

	fName = "createUniString";
	plugData::data.funcsTech.createUniString.Load(&plPath, &fName);

	fName = "toggleDeveloperMode";
	plugData::data.funcs.toggleDeveloperMode.Load(&plPath, &fName);


	fName = "setCryptedString";
	plugData::data.funcs.setCryptedString.Load(&plPath, &fName);


	fName = "setReligionsLimit";
	plugData::data.funcs.setReligionsLimit.Load(&plPath, &fName);

	fName = "getFactionsCount";
	plugData::data.funcs.getFactionsCount.Load(&plPath, &fName);

	fName = "getFactionsList";
	plugData::data.funcs.getFactionsList.Load(&plPath, &fName);

    fName = "removeTrait";
    plugData::data.funcs.removeTrait.Load(&plPath, &fName);

	fName = "moveNormal";
	plugData::data.funcs.moveNormal.Load(&plPath, &fName);

	fName = "siegeSettlement";
	plugData::data.funcs.siegeSettlement.Load(&plPath, &fName);

	fName = "siegeFort";
	plugData::data.funcs.siegeFort.Load(&plPath, &fName);

	fName = "addAncillary";
	plugData::data.funcs.addAncillary.Load(&plPath, &fName);

	fName = "removeAncillary";
	plugData::data.funcs.removeAncillary.Load(&plPath, &fName);

	fName = "findAncillary";
	plugData::data.funcs.findAncillary.Load(&plPath, &fName);

	fName = "addTrait";
	plugData::data.funcs.addTrait.Load(&plPath, &fName);

	fName = "replaceTile";
	plugData::data.funcs.replaceTile.Load(&plPath, &fName);

	fName = "moveStratCameraSlow";
	plugData::data.funcs.moveStratCameraSlow.Load(&plPath, &fName);

	fName = "moveStratCameraFast";
	plugData::data.funcs.moveStratCameraFast.Load(&plPath, &fName);

	fName = "zoomStratCamera";
	plugData::data.funcs.zoomStratCamera.Load(&plPath, &fName);

	fName = "teleportCharacter";
	plugData::data.funcs.teleportCharacter.Load(&plPath, &fName);

	fName = "createCharacter";
	plugData::data.funcs.createCharacter.Load(&plPath, &fName);

	fName = "setSoldiersCount";
	plugData::data.funcs.setSoldiersCount.Load(&plPath, &fName);

	fName = "setUnitParams";
	plugData::data.funcs.setUnitParams.Load(&plPath, &fName);


	fName = "setUnitMovepoints";
	plugData::data.funcs.setUnitMovepoints.Load(&plPath, &fName);

	fName = "killUnit";
	plugData::data.funcs.killUnit.Load(&plPath, &fName);

	fName = "killCharacter";
	plugData::data.funcs.killCharacter.Load(&plPath, &fName);


	fName = "createArmy";
	plugData::data.funcs.createArmy.Load(&plPath, &fName);

	fName = "createArmyInSettlement";
	plugData::data.funcs.createArmyInSettlement.Load(&plPath, &fName);


	fName = "createUnitN";
	plugData::data.funcs.createUnitN.Load(&plPath, &fName);

	fName = "createUnitIdx";
	plugData::data.funcs.createUnitIdx.Load(&plPath, &fName);

	fName = "createUnitEDB";
	plugData::data.funcs.createUnitEDB.Load(&plPath, &fName);

	fName = "addUnitToArmy";
	plugData::data.funcs.addUnitToArmy.Load(&plPath, &fName);

	fName = "setBodyguard";
	plugData::data.funcs.setBodyguard.Load(&plPath, &fName);

	fName = "attackCharacter";
	plugData::data.funcs.attackCharacter.Load(&plPath, &fName);

	fName = "updateRegionColors";
	plugData::data.funcs.updateRegionColors.Load(&plPath, &fName);

	fName = "loadTexture";
	plugData::data.funcs.loadTexture.Load(&plPath, &fName);

	fName = "unloadTexture";
	plugData::data.funcs.unloadTexture.Load(&plPath, &fName);

	fName = "setAncLimit";
	plugData::data.funcs.setAncLimit.Load(&plPath, &fName);

    fName = "getBattleCamCoords";
    plugData::data.funcsBattle.getBattleCamCoords.Load(&plPath, &fName);

	fName = "unlockConsoleCommands";
	plugData::data.funcs.unlockConsoleCommands.Load(&plPath, &fName);

	fName = "setMaxBgSize";
	plugData::data.funcs.setMaxBgSize.Load(&plPath, &fName);

	fName = "setConversionLvlFromCastle";
	plugData::data.funcs.setConversionLvlFromCastle.Load(&plPath, &fName);

	fName = "setConversionLvlFromCity";
	plugData::data.funcs.setConversionLvlFromCity.Load(&plPath, &fName);


	fName = "setSettlementOwner";
	plugData::data.funcs.setSettlementOwner.Load(&plPath, &fName);

	fName = "destroyBuilding";
	plugData::data.funcs.destroyBuilding.Load(&plPath, &fName);

	fName = "createBuilding";
	plugData::data.funcs.createBuilding.Load(&plPath, &fName);

	fName = "setCharacterType";
	plugData::data.funcs.setCharacterType.Load(&plPath, &fName);

	//battle functions
	fName = "getBattleCondCode";
	plugData::data.funcsBattle.getBattleCondCode.Load(&plPath, &fName);

	fName = "swUnBMapHighlight";
	plugData::data.funcsBattle.swUnBMapHighlight.Load(&plPath, &fName);


	//eop edu functions
	fName = "addEopEduEntry";
	plugData::data.funcsEopEdu.addEopEduEntry.Load(&plPath, &fName);

	fName = "addEopEduEntryFromFile";
	plugData::data.funcsEopEdu.addEopEduEntryFromFile.Load(&plPath, &fName);

	fName = "getEopEduEntry";
	plugData::data.funcsEopEdu.getEopEduEntry.Load(&plPath, &fName);

	fName = "getEduEntry";
	plugData::data.funcsEopEdu.getEduEntry.Load(&plPath, &fName);

	fName = "getEduEntryByType";
	plugData::data.funcsEopEdu.getEduEntryByType.Load(&plPath, &fName);

	fName = "getEduIndexByType";
	plugData::data.funcsEopEdu.getEduIndexByType.Load(&plPath, &fName);

	fName = "getDataEopEdu";
	plugData::data.funcsEopEdu.getDataEopEdu.Load(&plPath, &fName);


	fName = "setEntryUnitCardTga";
	plugData::data.funcsEopEdu.setEntryUnitCardTga.Load(&plPath, &fName);

	fName = "setEntryInfoCardTga";
	plugData::data.funcsEopEdu.setEntryInfoCardTga.Load(&plPath, &fName);

	fName = "setEntrySoldierModel";
	plugData::data.funcsEopEdu.setEntrySoldierModel.Load(&plPath, &fName);

	fName = "setEntryLocalizedName";
	plugData::data.funcsEopEdu.setEntryLocalizedName.Load(&plPath, &fName);

	fName = "setEntryLocalizedDescr";
	plugData::data.funcsEopEdu.setEntryLocalizedDescr.Load(&plPath, &fName);

	fName = "setEntryLocalizedShortDescr";
	plugData::data.funcsEopEdu.setEntryLocalizedShortDescr.Load(&plPath, &fName);

	fName = "setBuildingChainLimit";
	plugData::data.funcs.setBuildingChainLimit.Load(&plPath, &fName);

	fName = "setGuildCooldown";
	plugData::data.funcs.setGuildCooldown.Load(&plPath, &fName);

	fName = "getGuild";
	plugData::data.funcs.getGuild.Load(&plPath, &fName);

	fName = "GetUnitSize";
	plugData::data.funcs.GetUnitSize.Load(&plPath, &fName);

	//eop build functions

	fName = "addEopBuildEntry";
	plugData::data.funcs.addEopBuildEntry.Load(&plPath, &fName);

	fName = "getEopBuildEntry";
	plugData::data.funcs.getEopBuildEntry.Load(&plPath, &fName);

	fName = "setBuildingPic";
	plugData::data.funcs.setBuildingPic.Load(&plPath, &fName);

	fName = "setBuildingPicConstructed";
	plugData::data.funcs.setBuildingPicConstructed.Load(&plPath, &fName);

	fName = "setBuildingPicConstruction";
	plugData::data.funcs.setBuildingPicConstruction.Load(&plPath, &fName);

	fName = "setBuildingLocalizedName";
	plugData::data.funcs.setBuildingLocalizedName.Load(&plPath, &fName);

	fName = "setBuildingLocalizedDescr";
	plugData::data.funcs.setBuildingLocalizedDescr.Load(&plPath, &fName);

	fName = "setBuildingLocalizedDescrShort";
	plugData::data.funcs.setBuildingLocalizedDescrShort.Load(&plPath, &fName);

	fName = "addBuildingCapability";
	plugData::data.funcs.addBuildingCapability.Load(&plPath, &fName);

	fName = "removeBuildingCapability";
	plugData::data.funcs.removeBuildingCapability.Load(&plPath, &fName);

	fName = "getBuildingCapability";
	plugData::data.funcs.getBuildingCapability.Load(&plPath, &fName);

	fName = "getBuildingCapabilityNum";
	plugData::data.funcs.getBuildingCapabilityNum.Load(&plPath, &fName);

	fName = "addBuildingPool";
	plugData::data.funcs.addBuildingPool.Load(&plPath, &fName);

	fName = "removeBuildingPool";
	plugData::data.funcs.removeBuildingPool.Load(&plPath, &fName);

	fName = "getBuildingPool";
	plugData::data.funcs.getBuildingPool.Load(&plPath, &fName);

	fName = "getBuildingPoolNum";
	plugData::data.funcs.getBuildingPoolNum.Load(&plPath, &fName);

	fName = "createEOPBuilding";
	plugData::data.funcs.createEOPBuilding.Load(&plPath, &fName);

	fName = "getBuildingByName";
	plugData::data.funcs.getBuildingByName.Load(&plPath, &fName);

	//fbx
	fName = "addFbxModel";
	plugData::data.functsEopFbx.addFbxModel.Load(&plPath, &fName);

	fName = "deleteFbxModel";
	plugData::data.functsEopFbx.deleteFbxModel.Load(&plPath, &fName);

	fName = "getFbxModel";
	plugData::data.functsEopFbx.getFbxModel.Load(&plPath, &fName);

	fName = "addFbxObject";
	plugData::data.functsEopFbx.addFbxObject.Load(&plPath, &fName);

	fName = "deleteFbxObject";
	plugData::data.functsEopFbx.deleteFbxObject.Load(&plPath, &fName);

	fName = "getFbxObject";
	plugData::data.functsEopFbx.getFbxObject.Load(&plPath, &fName);

	fName = "setFbxObjectCoords";
	plugData::data.functsEopFbx.setFbxObjectCoords.Load(&plPath, &fName);

	fName = "getFbxObjectCoords";
	plugData::data.functsEopFbx.getFbxObjectCoords.Load(&plPath, &fName);

	fName = "setFbxObjectSize";
	plugData::data.functsEopFbx.setFbxObjectSize.Load(&plPath, &fName);

	fName = "getFbxObjectSize";
	plugData::data.functsEopFbx.getFbxObjectSize.Load(&plPath, &fName);

	fName = "setFbxObjectDrawState";
	plugData::data.functsEopFbx.setFbxObjectDrawState.Load(&plPath, &fName);

	fName = "getFbxObjectDrawState";
	plugData::data.functsEopFbx.getFbxObjectDrawState.Load(&plPath, &fName);

	fName = "setFbxObjectAnimState";
	plugData::data.functsEopFbx.setFbxObjectAnimState.Load(&plPath, &fName);

	fName = "getFbxObjectAnimState";
	plugData::data.functsEopFbx.getFbxObjectAnimState.Load(&plPath, &fName);

	fName = "getFbxObjectDrawType";
	plugData::data.functsEopFbx.getFbxObjectDrawType.Load(&plPath, &fName);

	fName = "setFbxObjectDrawType";
	plugData::data.functsEopFbx.setFbxObjectDrawType.Load(&plPath, &fName);



	fName = "createEOPSound";
	plugData::data.functsEopSounds.createEOPSound.Load(&plPath, &fName);

	fName = "deleteEOPSound";
	plugData::data.functsEopSounds.deleteEOPSound.Load(&plPath, &fName);

	fName = "playEOPSound";
	plugData::data.functsEopSounds.playEOPSound.Load(&plPath, &fName);

	fName = "setEOPSoundOffset";
	plugData::data.functsEopSounds.setEOPSoundOffset.Load(&plPath, &fName);

	fName = "getEOPSoundOffset";
	plugData::data.functsEopSounds.getEOPSoundOffset.Load(&plPath, &fName);

	fName = "pauseEOPSound";
	plugData::data.functsEopSounds.pauseEOPSound.Load(&plPath, &fName);

	fName = "stopEOPSound";
	plugData::data.functsEopSounds.stopEOPSound.Load(&plPath, &fName);



	fName = "getUiElement";
	plugData::data.functsGameUI.getUiElement.Load(&plPath, &fName);

	fName = "useUiElement";
	plugData::data.functsGameUI.useUiElement.Load(&plPath, &fName);

	initLua();
	return 1;
}

void initLua()
{
	std::string luaFile = plugData::data.modFolder + R"(\youneuoy_Data\plugins\lua\luaPluginScript.lua)";

	sol::state* luaState = plugData::data.luaAll.init(luaFile, plugData::data.modFolder);
	plugData::data.luaAll.initCampaign();
	plugData::data.luaAll.initP2();
	plugData::data.luaAll.initEopEdu();
	plugData::data.luaAll.initEopFbx();
	plugData::data.luaAll.initEop3dObjects();
	plugData::data.luaAll.initEopSounds();
	plugData::data.luaAll.initTech();
	sol_ImGui::Init(*luaState);

	gameEvents::initEvents();
	plugData::data.luaAll.onPluginLoadF();
	if (luaState == nullptr)
	{
		MessageBoxA(nullptr, "LUA loading error!", "Error!", NULL);
		exit(0);
	}
}

void reloadLua()
{
	std::string luaFile = plugData::data.modFolder + R"(\youneuoy_Data\plugins\lua\luaPluginScript.lua)";
	auto script = plugData::data.luaAll.luaState.load_file(luaFile);
	script();
}

