#include "pch.h"
#include "luaPlugin.h"
#include "army.h"
#include "campaign.h"
#include "casModelsDrawer.h"
#include "gameHelpers.h"
#include "realGameTypes.h"
#include "strategyMap.h"
#include "character.h"
#include "stratModelsChange.h"
#include "tilesChange.h"
#include "gameUi.h"
#include "battle.h"

void luaPlugin::addLegacy()
{
    struct
    {
        sol::table stratmap;
        sol::table objects;
        sol::table camera;
        sol::table game;
        sol::usertype<gameDataAllStruct> gameDataAllStruct;
    }tables;
	
	///////////////////////////////////Legacy stratmap.x tables///////////////////////////////////
	tables.objects = luaState.create_table();
	tables.objects.set_function("startDrawModelAt", &casModelsDrawer::addCasModelToDrawList);
	tables.objects.set_function("stopDrawModel", &casModelsDrawer::removeCasModelFromDrawList);
	tables.objects.set_function("addCharacterCas", &stratModelsChange::addCharacterCas);
	tables.objects.set_function("setModel", sol::overload(&stratModelsChange::setModel, &stratModelsChange::setModelOneVar));
	tables.objects.set_function("replaceTile", &tilesChange::replaceTile);
	tables.camera = luaState.create_table();
	tables.camera.set_function("move", &stratMapHelpers::moveStratCameraSlow);
	tables.camera.set_function("jump", &stratMapHelpers::moveStratCameraFast);
	tables.camera.set_function("zoom", &stratMapHelpers::zoomStratCamera);
	tables.game = luaState.create_table();
	tables.game.set_function("getFactionsCount", &campaignHelpers::getFactionsCount);
	tables.game.set_function("getFaction", &campaignHelpers::getFaction);
	tables.game.set_function("callConsole", &gameHelpers::callConsole);
	tables.game.set_function("createCharacterByString", &characterHelpers::createCharacter);
	tables.game.set_function("spawnArmy", sol::overload(
				sol::resolve<armyStruct*(
					factionStruct*,
					const char*,
					const char*,
					int,
					const char*,
					const char*,
					int,
					int,
					int,
					bool,
					int,
					int,
					int,
					uint8_t,
					uint8_t)>(&armyHelpers::spawnArmy),
					sol::resolve<armyStruct*(
						factionStruct*,
						const char*,
						const char*,
						int,
						const char*,
						const char*,
						int,
						int,
						int,
						bool,
						int,
						int,
						int,
						uint8_t,
						uint8_t,
						int)>(&armyHelpers::spawnArmy)
			));
	tables.game.set_function("createArmy", &armyHelpers::createArmy);
	tables.game.set_function("createArmyInSettlement", &armyHelpers::createArmyInSettlement);
	tables.game.set_function("historicEvent", sol::overload(
			sol::resolve<void(const std::string&, const std::string&, const std::string&)>(gameHelpers::historicEvent),
			sol::resolve<void(const std::string&, const std::string&, const std::string&, bool)>(gameHelpers::historicEvent),
			sol::resolve<void(const std::string&, const std::string&, const std::string&, bool, int, int)>(gameHelpers::historicEvent),
			sol::resolve<void(const std::string&, const std::string&, const std::string&, bool, int, int, const sol::table&)>(gameHelpers::historicEvent)
		));
	tables.game.set_function("setScriptCounter", &gameHelpers::setScriptCounter);
	tables.game.set_function("getScriptCounter", &gameHelpers::getScriptCounterLua);
	tables.game.set_function("scriptCommand", &gameHelpers::scriptCommandLua);
	tables.stratmap = luaState.create_table("stratmap");
	tables.stratmap.set("objects", tables.objects);
	tables.stratmap.set("camera", tables.camera);
	tables.stratmap.set("game", tables.game);
	tables.gameDataAllStruct = luaState.new_usertype<gameDataAllStruct>("gameDataAll");
	tables.gameDataAllStruct.set_function("get", &gameHelpers::getGameDataAll);
	tables.gameDataAllStruct.set("battleStruct", &gameDataAllStruct::battleHandler);
	tables.gameDataAllStruct.set("campaignStruct", &gameDataAllStruct::campaignStruct);
	tables.gameDataAllStruct.set("uiCardManager", &gameDataAllStruct::uiCardManager);
	tables.gameDataAllStruct.set("stratMap", &gameDataAllStruct::stratMap);
	tables.gameDataAllStruct.set("selectionInfo", &gameDataAllStruct::selectInfo);
}
