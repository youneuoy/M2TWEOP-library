///
//![Lua logo](../Lua.png)
//M2TWEOP structures and functions. There are not many examples and descriptions here. Also note that the examples do not include many of the checks that would be required when creating modifications.
//@module LuaPlugin
//@author youneuoy
//@license GPL-3.0
#include "luaP.h"
#include "gameHelpers.h"
#include "characterRecord.h"
#include "faction.h"
#include "unit.h"
#include "army.h"
#include "strategyMap.h"
#include "campaign.h"
#include "gameUi.h"
#include "m2tweopMapManager.h"


void luaP::initCampaign()
{

	stratMapHelpers::addToLua(luaState);
	campaignHelpers::addToLua(luaState);
	buildingHelpers::addToLua(luaState);
	battleHelpers::addToLua(luaState);
	gameUiHelpers::addToLua(luaState);
	
	struct
	{
		sol::usertype<options1> options1;
		sol::usertype<options2> options2;
		sol::usertype<campaignDifficulty1> campaignDifficulty1;
		sol::usertype<campaignDifficulty2> campaignDifficulty2;
		sol::usertype<selectionInfo> selectionInfo;
		sol::usertype<mapImage> mapImageStruct;
	}typeAll;

	///Game Options
	//@section options1

	/***
	Basic options1 table

	@tfield int widescreen
	@tfield int antiAliasMode
	@tfield int subtitles
	@tfield int english
	@tfield int noBattleTimeLimit
	@tfield int useNewCursorActions
	@tfield int campaignNumTimesPlay
	@tfield int uiWinConditions
	@tfield int isScenario
	@tfield int isHotseatEnabled
	@tfield int hotseatAutosave
	@tfield int email
	@tfield int saveConfig
	@tfield int closeAfterSave
	@tfield int validateData
	@tfield int campaignMapSpeedUp
	@tfield int skipAiFactions
	@tfield int labelCharacters
	@tfield int noBackGroundFmv
	@tfield int disableArrowMarkers
	@tfield int arcadeBattles
	@tfield int disableEvents
	@tfield int isPrologue
	@tfield int updateAiCamera
	@tfield int hideCampaign
	@tfield int unlimitedMenOnBattlefield
	@tfield int tgaReserveSpace
	@tfield int keysetUsed
	@tfield int muteAdvisor
	@tfield int advancedStatsAlways
	@tfield int microManageAllSettlements
	@tfield int blindAdvisor
	@tfield int terrainQuality
	@tfield int vegetationQuality
	@tfield int useQuickChat
	@tfield int graphicsAdaptor
	@tfield int showDemeanour
	@tfield int radar
	@tfield int unitCards
	@tfield int sa_cards
	@tfield int buttons
	@tfield int tutorialBattlePlayed
	@tfield int disableVnVs
	@tfield int allUsers

	@table options1
	*/
	typeAll.options1 = luaState.new_usertype<options1>("options1");
	typeAll.options1.set("widescreen", &options1::widescreen);
	typeAll.options1.set("antiAliasMode", &options1::antiAliasMode);
	typeAll.options1.set("subtitles", &options1::subtitles);
	typeAll.options1.set("english", &options1::english);
	typeAll.options1.set("noBattleTimeLimit", &options1::noBattleTimeLimit);
	typeAll.options1.set("useNewCursorActions", &options1::useNewCursorActions);
	typeAll.options1.set("campaignNumTimesPlay", &options1::campaignNumTimesPlay);
	typeAll.options1.set("uiWinConditions", &options1::uiWinConditions);
	typeAll.options1.set("isScenario", &options1::isScenario);
	typeAll.options1.set("isHotseatEnabled", &options1::isHotseatEnabled);
	typeAll.options1.set("hotseatAutosave", &options1::hotseatAutosave);
	typeAll.options1.set("email", &options1::email);
	typeAll.options1.set("saveConfig", &options1::saveConfig);
	typeAll.options1.set("closeAfterSave", &options1::closeAfterSave);
	typeAll.options1.set("validateData", &options1::validateData);
	typeAll.options1.set("campaignMapSpeedUp", &options1::campaignMapSpeedUp);
	typeAll.options1.set("skipAiFactions", &options1::skipAiFactions);
	typeAll.options1.set("labelCharacters", &options1::labelCharacters);
	typeAll.options1.set("noBackGroundFmv", &options1::noBackGroundFmv);
	typeAll.options1.set("disableArrowMarkers", &options1::disableArrowMarkers);
	typeAll.options1.set("arcadeBattles", &options1::arcadeBattles);
	typeAll.options1.set("disableEvents", &options1::disableEvents);
	typeAll.options1.set("isPrologue", &options1::isPrologue);
	typeAll.options1.set("updateAiCamera", &options1::updateAiCamera);
	typeAll.options1.set("hideCampaign", &options1::hideCampaign);
	typeAll.options1.set("unlimitedMenOnBattlefield", &options1::unlimitedMenOnBattlefield);
	typeAll.options1.set("tgaReserveSpace", &options1::tgaReserveSpace);
	typeAll.options1.set("keysetUsed", &options1::keysetUsed);
	typeAll.options1.set("muteAdvisor", &options1::muteAdvisor);
	typeAll.options1.set("advancedStatsAlways", &options1::advancedStatsAlways);
	typeAll.options1.set("microManageAllSettlements", &options1::microManageAllSettlements);
	typeAll.options1.set("blindAdvisor", &options1::blindAdvisor);
	typeAll.options1.set("terrainQuality", &options1::terrainQuality);
	typeAll.options1.set("vegetationQuality", &options1::vegetationQuality);
	typeAll.options1.set("useQuickChat", &options1::useQuickChat);
	typeAll.options1.set("graphicsAdaptor", &options1::graphicsAdaptor);
	typeAll.options1.set("showDemeanour", &options1::showDemeanour);
	typeAll.options1.set("radar", &options1::radar);
	typeAll.options1.set("unitCards", &options1::unitCards);
	typeAll.options1.set("sa_cards", &options1::sa_cards);
	typeAll.options1.set("buttons", &options1::buttons);
	typeAll.options1.set("tutorialBattlePlayed", &options1::tutorialBattlePlayed);
	typeAll.options1.set("disableVnVs", &options1::disableVnVs);
	typeAll.options1.set("allUsers", &options1::allUsers);


	/***
	Basic options2 table
	
	@tfield int campaignResolutionX
	@tfield int campaignResolutionY
	@tfield int battleResolutionX
	@tfield int battleResolutionY
	@tfield int vSync
	@tfield int uiIconBarCheck
	@tfield int uiRadarCheck
	@tfield int useMorale
	@tfield int uiAmmoCheck
	@tfield int useFatigue
	@tfield int uiSupplyCheck
	@tfield int toggleFowState this does not toggle fow just remembers if it was on or off
	@tfield int cameraRestrict
	@tfield int eventCutscenes
	@tfield int defaultCameraInBattle
	@tfield int splashes
	@tfield int stencilShadows
	@tfield int audioEnable
	@tfield int speechEnable
	@tfield int firstTimePlay
	@tfield int toggleAutoSave
	@tfield int showBanners
	@tfield int passwords
	@tfield int hotseatTurns
	@tfield int hotseatScroll
	@tfield int allowValidationFeatures
	@tfield int campaignSpeed
	@tfield int labelSettlements
	@tfield int disablePapalElections
	@tfield int autoresolveAllBattles
	@tfield int savePrefs
	@tfield int disableConsole
	@tfield int validateDiplomacy
	@tfield int unitDetail
	@tfield int buildingDetail
	@tfield int maxSoldiersOnBattlefield if limited
	@tfield int unitSize
	@tfield int cameraRotateSpeed
	@tfield int cameraMoveSpeed
	@tfield float cameraSmoothing
	@tfield int masterVolume
	@tfield int musicVolume
	@tfield int speechVolume
	@tfield int sfxVolume
	@tfield int subFactionAccents
	@tfield int tgaWidth
	@tfield float tgaAspect
	@tfield int tgaInputScale
	@tfield int scrollMinZoom
	@tfield int scrollMaxZoom
	@tfield int advisorVerbosity
	@tfield int effectQuality
	@tfield int EnableCameraCampaignSmoothing
	@tfield int chatMsgDuration
	@tfield int saveGameSpyPassword
	@tfield int addDateToLogs
	@tfield int showToolTips
	@tfield int isNormalHud
	@tfield int showPackageLitter
	@tfield float unitSizeMultiplierLow
	@tfield float unitSizeMultiplierMedium
	@tfield float unitSizeMultiplierLarge

	@table options2
	*/
	typeAll.options2 = luaState.new_usertype<options2>("options2");
	typeAll.options2.set("campaignResolutionX", &options2::campaignResolutionX);
	typeAll.options2.set("campaignResolutionY", &options2::campaignResolutionY);
	typeAll.options2.set("battleResolutionX", &options2::battleResolutionX);
	typeAll.options2.set("battleResolutionY", &options2::battleResolutionY);
	typeAll.options2.set("vSync", &options2::vSync);
	typeAll.options2.set("uiIconBarCheck", &options2::uiIconBarCheck);
	typeAll.options2.set("uiRadarCheck", &options2::uiRadarCheck);
	typeAll.options2.set("useMorale", &options2::useMorale);
	typeAll.options2.set("uiAmmoCheck", &options2::uiAmmoCheck);
	typeAll.options2.set("useFatigue", &options2::useFatigue);
	typeAll.options2.set("uiSupplyCheck", &options2::uiSupplyCheck);
	typeAll.options2.set("toggleFowState", &options2::toggleFowState);
	typeAll.options2.set("cameraRestrict", &options2::cameraRestrict);
	typeAll.options2.set("eventCutscenes", &options2::eventCutscenes);
	typeAll.options2.set("defaultCameraInBattle", &options2::defaultCameraInBattle);
	typeAll.options2.set("splashes", &options2::splashes);
	typeAll.options2.set("stencilShadows", &options2::stencilShadows);
	typeAll.options2.set("audioEnable", &options2::audioEnable);
	typeAll.options2.set("speechEnable", &options2::speechEnable);
	typeAll.options2.set("firstTimePlay", &options2::firstTimePlay);
	typeAll.options2.set("toggleAutoSave", &options2::toggleAutoSave);
	typeAll.options2.set("showBanners", &options2::showBanners);
	typeAll.options2.set("passwords", &options2::passwords);
	typeAll.options2.set("hotseatTurns", &options2::hotseatTurns);
	typeAll.options2.set("hotseatScroll", &options2::hotseatScroll);
	typeAll.options2.set("allowValidationFeatures", &options2::allowValidationFeatures);
	typeAll.options2.set("campaignSpeed", &options2::campaignSpeed);
	typeAll.options2.set("labelSettlements", &options2::labelSettlements);
	typeAll.options2.set("disablePapalElections", &options2::disablePapalElections);
	typeAll.options2.set("autoresolveAllBattles", &options2::autoresolveAllBattles);
	typeAll.options2.set("savePrefs", &options2::savePrefs);
	typeAll.options2.set("disableConsole", &options2::disableConsole);
	typeAll.options2.set("validateDiplomacy", &options2::validateDiplomacy);
	typeAll.options2.set("unitDetail", &options2::unitDetail);
	typeAll.options2.set("buildingDetail", &options2::buildingDetail);
	typeAll.options2.set("maxSoldiersOnBattlefield", &options2::maxSoldiersOnBattlefield);
	typeAll.options2.set("unitSize", &options2::unitSize);
	typeAll.options2.set("cameraRotateSpeed", &options2::cameraRotateSpeed);
	typeAll.options2.set("cameraMoveSpeed", &options2::cameraMoveSpeed);
	typeAll.options2.set("cameraSmoothing", &options2::cameraSmoothing);
	typeAll.options2.set("masterVolume", &options2::masterVolume);
	typeAll.options2.set("musicVolume", &options2::musicVolume);
	typeAll.options2.set("speechVolume", &options2::speechVolume);
	typeAll.options2.set("sfxVolume", &options2::sfxVolume);
	typeAll.options2.set("subFactionAccents", &options2::subFactionAccents);
	typeAll.options2.set("tgaWidth", &options2::tgaWidth);
	typeAll.options2.set("tgaAspect", &options2::tgaAspect);
	typeAll.options2.set("tgaInputScale", &options2::tgaInputScale);
	typeAll.options2.set("scrollMinZoom", &options2::scrollMinZoom);
	typeAll.options2.set("scrollMaxZoom", &options2::scrollMaxZoom);
	typeAll.options2.set("advisorVerbosity", &options2::advisorVerbosity);
	typeAll.options2.set("effectQuality", &options2::effectQuality);
	typeAll.options2.set("EnableCameraCampaignSmoothing", &options2::EnableCameraCampaignSmoothing);
	typeAll.options2.set("chatMsgDuration", &options2::chatMsgDuration);
	typeAll.options2.set("saveGameSpyPassword", &options2::saveGameSpyPassword);
	typeAll.options2.set("addDateToLogs", &options2::addDateToLogs);
	typeAll.options2.set("showToolTips", &options2::showToolTips);
	typeAll.options2.set("isNormalHud", &options2::isNormalHud);
	typeAll.options2.set("showPackageLitter", &options2::showPackageLitter);
	typeAll.options2.set("unitSizeMultiplierLow", &options2::unitSizeMultiplierLow);
	typeAll.options2.set("unitSizeMultiplierMedium", &options2::unitSizeMultiplierMedium);
	typeAll.options2.set("unitSizeMultiplierLarge", &options2::unitSizeMultiplierLarge);


	///Campaign Difficulty Modifiers
	//@section campaignDifficulty1

	/***
	Basic campaignDifficulty1 table.

	@tfield int orderFromGrowth
	@tfield int considerWarWithPlayer
	@tfield float brigandChanceAi
	@tfield float brigandChancePlayer
	@tfield int forceAttackDelay
	@tfield float taxIncomeModifierPlayer
	@tfield float farmingIncomeModifierPlayer
	@tfield float incomeModifierAi
	@tfield float playerRegionValueModifier

	@table campaignDifficulty1
	*/

	typeAll.campaignDifficulty1 = luaState.new_usertype<campaignDifficulty1>("campaignDifficulty1");
	typeAll.campaignDifficulty1.set("orderFromGrowth", &campaignDifficulty1::orderFromGrowth);
	typeAll.campaignDifficulty1.set("considerWarWithPlayer", &campaignDifficulty1::considerWarWithPlayer);
	typeAll.campaignDifficulty1.set("brigandChanceAi", &campaignDifficulty1::brigandChanceAi);
	typeAll.campaignDifficulty1.set("brigandChancePlayer", &campaignDifficulty1::brigandChancePlayer);
	typeAll.campaignDifficulty1.set("forceAttackDelay", &campaignDifficulty1::forceAttackDelay);
	typeAll.campaignDifficulty1.set("taxIncomeModifierPlayer", &campaignDifficulty1::taxIncomeModifierPlayer);
	typeAll.campaignDifficulty1.set("farmingIncomeModifierPlayer", &campaignDifficulty1::farmingIncomeModifierPlayer);
	typeAll.campaignDifficulty1.set("incomeModifierAi", &campaignDifficulty1::incomeModifierAi);
	typeAll.campaignDifficulty1.set("playerRegionValueModifier", &campaignDifficulty1::playerRegionValueModifier);

	/***
	Basic campaignDifficulty2 table.

	@tfield int popGrowthBonusAi
	@tfield int publicOrderBonusAi
	@tfield int experienceBonusAi
	@tfield int incomeBonusAi
	@tfield int wantsTargetPlayer
	@tfield int wantsTargetPlayerNaval
	@tfield int autoAttackPlayerIfCrusadeTarget

	@table campaignDifficulty2
	*/

	typeAll.campaignDifficulty2 = luaState.new_usertype<campaignDifficulty2>("campaignDifficulty2");
	typeAll.campaignDifficulty2.set("popGrowthBonusAi", &campaignDifficulty2::popGrowthBonusAi);
	typeAll.campaignDifficulty2.set("publicOrderBonusAi", &campaignDifficulty2::publicOrderBonusAi);
	typeAll.campaignDifficulty2.set("experienceBonusAi", &campaignDifficulty2::experienceBonusAi);
	typeAll.campaignDifficulty2.set("incomeBonusAi", &campaignDifficulty2::incomeBonusAi);
	typeAll.campaignDifficulty2.set("wantsTargetPlayer", &campaignDifficulty2::wantsTargetPlayer);
	typeAll.campaignDifficulty2.set("wantsTargetPlayerNaval", &campaignDifficulty2::wantsTargetPlayerNaval);
	typeAll.campaignDifficulty2.set("autoAttackPlayerIfCrusadeTarget", &campaignDifficulty2::autoAttackPlayerIfCrusadeTarget);

	///mapImage
	//@section mapImageStruct

	/***
	@tfield float blurStrength
	@tfield bool useBlur
	@tfield bool adaptiveBlur Can be slow on large or frequently updated images! needs use blur also true.
	@tfield makeMapImage makeMapImage
	@tfield clearMapImage clearMapImage
	@tfield loadMapTexture loadMapTexture
	@tfield fillRegionColor fillRegionColor
	@tfield addRegionColor addRegionColor
	@tfield fillTileColor fillTileColor
	@tfield addTileColor addTileColor
	@table mapImageStruct
	*/
	typeAll.mapImageStruct = luaState.new_usertype<mapImage>("mapImageStruct");
	typeAll.mapImageStruct.set("useBlur", &mapImage::useBlur);
	typeAll.mapImageStruct.set("adaptiveBlur", &mapImage::adaptiveBlur);
	typeAll.mapImageStruct.set("blurStrength", &mapImage::blurStrength);
	
	/***
	Create a new image you want to determine region colors.
	@function mapImageStruct.makeMapImage
	@treturn mapImageStruct mapImage
	@usage
	local mapImage = mapImageStruct.makeMapImage();
	*/
	typeAll.mapImageStruct.set_function("makeMapImage", &m2tweopMapManager::makeMapImage);
	
	/***
	Reset image state.
	@function mapImageStruct:clearMapImage
	@usage
	local mapImage = mapImageStruct.makeMapImage();
	mapImage:clearMapImage();
	*/
	typeAll.mapImageStruct.set_function("clearMapImage", &m2tweopMapManager::clearMapImage);
	
	/***
	Create a new map texture. Use an uncompressed(!) dds image that is some multiple of your map_regions size. The background must line up with map_regions for whatever scale you chose for it to display correctly.
	@function mapImageStruct:loadMapTexture
	@tparam string path full path to texture
	@treturn int x size of the image
	@treturn int y size of the image
	@treturn int id of the image
	@usage
	local mapImage = mapImageStruct.makeMapImage();
	local x, y, id = mapImage:loadMapTexture(M2TWEOP.getModPath() .. "map_regions.dds");
	ImGui.Image(id, x, y);
	*/
	typeAll.mapImageStruct.set_function("loadMapTexture", &m2tweopMapManager::loadMapTexture);
	
	/***
	Fill a region with a color.
	@function mapImageStruct:fillRegionColor
	@tparam int id region ID
	@tparam int r red
	@tparam int g green
	@tparam int b blue
	@tparam int a alpha
	@usage
	local mapImage = mapImageStruct.makeMapImage();
	mapImage:fillRegionColor(50, 0, 255, 0, 255);
	*/
	typeAll.mapImageStruct.set_function("fillRegionColor", &m2tweopMapManager::fillRegionColor);
	
	/***
	Add a color to already filled region.
	@function mapImageStruct:addRegionColor
	@tparam int id region ID
	@tparam int r red
	@tparam int g green
	@tparam int b blue
	@tparam int a alpha
	@usage
	local mapImage = mapImageStruct.makeMapImage();
	mapImage:fillRegionColor(50, 0, 255, 0, 255);
	mapImage:addRegionColor(50, 10, -10, 10, 0);
	*/
	typeAll.mapImageStruct.set_function("addRegionColor", &m2tweopMapManager::addRegionColor);
	
	/***
	Fill a tile with a color.
	@function mapImageStruct:fillTileColor
	@tparam int x x coordinate
	@tparam int y y coordinate
	@tparam int r red
	@tparam int g green
	@tparam int b blue
	@tparam int a alpha
	@treturn mapImageStruct mapImage
	@usage
	local mapImage = mapImageStruct.makeMapImage();
	mapImage:fillTileColor(153, 210, 0, 255, 0, 255);
	*/
	typeAll.mapImageStruct.set_function("fillTileColor", &m2tweopMapManager::fillTileColor);
	/***
	Add a color to an already set tile.
	@function mapImageStruct:addTileColor
	@tparam int x x coordinate
	@tparam int y y coordinate
	@tparam int r red
	@tparam int g green
	@tparam int b blue
	@tparam int a alpha
	@treturn mapImageStruct mapImage
	@usage
	local mapImage = mapImageStruct.makeMapImage();
	mapImage:fillTileColor(153, 210, 0, 255, 0, 255);
	mapImage:addTileColor(153, 210, 20, -10, 20, 0);
	*/
	typeAll.mapImageStruct.set_function("addTileColor", &m2tweopMapManager::addTileColor);

	///selectionInfo
	//@section Selection Info

	/***
	Basic selectionInfo table

	@tfield character selectedCharacter (Get only)
	@tfield character hoveredCharacter (Get only)
	@tfield character selectedEnemyCharacter You can only select non-player characters with zoom to location button (Get only)
	@tfield settlementStruct selectedSettlement (Get only)
	@tfield settlementStruct hoveredSettlement (Get only)
	@tfield settlementStruct selectedEnemySettlement You can only select non-player settlements with zoom to location button (Get only)
	@tfield fortStruct selectedFort (Get only)
	@tfield fortStruct hoveredFort (Get only)
	@tfield fortStruct selectedEnemyFort You can only select non-player forts with zoom to location button (Get only)

	@table selectionInfo
	*/
	typeAll.selectionInfo = luaState.new_usertype<selectionInfo>("selectionInfo");
	typeAll.selectionInfo.set("selectedCharacter", sol::property(&selectionInfo::getSelectedCharacter));
	typeAll.selectionInfo.set("hoveredCharacter", sol::property(&selectionInfo::getHoveredCharacter));
	typeAll.selectionInfo.set("selectedEnemyCharacter", sol::property(&selectionInfo::getSelectedEnemyCharacter));
	typeAll.selectionInfo.set("selectedSettlement", sol::property(&selectionInfo::getSelectedSettlement));
	typeAll.selectionInfo.set("hoveredSettlement", sol::property(&selectionInfo::getHoveredSettlement));
	typeAll.selectionInfo.set("selectedEnemySettlement", sol::property(&selectionInfo::getSelectedEnemySettlement));
	typeAll.selectionInfo.set("selectedFort", sol::property(&selectionInfo::getSelectedFort));
	typeAll.selectionInfo.set("hoveredFort", sol::property(&selectionInfo::getHoveredFort));
	typeAll.selectionInfo.set("selectedEnemyFort", sol::property(&selectionInfo::getSelectedEnemyFort));

}
void luaP::initP2()
{
	struct
	{
		//global game table
		sol::usertype<gameDataAllStruct> gameDataAllTable;

	}typeAll;
	///GameDataAll
	//@section gameDataAll

	/***
	Basic gameDataAll table

	@tfield get get
	@tfield battleStruct battleStruct battle data
	@tfield campaignStruct campaignStruct campaign data
	@tfield uiCardManager uiCardManager ui and selected objects data
	@tfield stratMap stratMap data
	@tfield selectionInfo selectionInfo data

	@table gameDataAll
	*/
	typeAll.gameDataAllTable = luaState.new_usertype<gameDataAllStruct>("gameDataAll");
	/***
	Call at the start of the script, this is a static object and the pointer to it doesn't change.
	@function gameDataAll.get
	@treturn gameDataAll gameDataAll
	@usage
	gameData=gameDataAll.get();
	battleXCoord=gameData.battleStruct.xCoord;
	*/
	typeAll.gameDataAllTable.set_function("get", &gameHelpers::getGameDataAll);
	typeAll.gameDataAllTable.set("battleStruct", &gameDataAllStruct::battleHandler);
	typeAll.gameDataAllTable.set("campaignStruct", &gameDataAllStruct::campaignStruct);
	typeAll.gameDataAllTable.set("uiCardManager", &gameDataAllStruct::uiCardManager);
	typeAll.gameDataAllTable.set("stratMap", &gameDataAllStruct::stratMap);
	typeAll.gameDataAllTable.set("selectionInfo", &gameDataAllStruct::selectInfo);
	using namespace campaignEnums;

	///EOP Enums
	//@section dipRelType

	/***
	Enum with a list of types of diplomatic relations.

	@tfield int war
	@tfield int peace
	@tfield int alliance
	@tfield int suzerain
	@tfield int trade (Doesn't work with trade rights agreements set at game start)

	@usage
	local campaign=gameDataAll.get().campaignStruct;
	local fac1=campaign.factionsSortedByDescrStrat[1];
	local fac2=campaign.factionsSortedByDescrStrat[2];
	local isInWar=campaign:checkDipStance(dipRelType.war,fac1,fac2);

	@table dipRelType
	*/
	luaState.new_enum(
		"dipRelType",
		"war", dipRelEnum::war,
		"peace", dipRelEnum::peace,
		"alliance", dipRelEnum::alliance,
		"suzerain", dipRelEnum::suzerain,
		"trade", dipRelEnum::trade
	);

	
	/***
	Enum with a list of types of unit battle properties.

	@tfield int guardMode
	@tfield int fireAtWill
	@tfield int skirmish

	@table unitBattleProperties
	*/
	luaState.new_enum(
		"unitBattleProperties",
		"guardMode", guardMode,
		"fireAtWill", fireAtWill,
		"skirmish", skirmish
	);

	
	/***
	Enum with a list of types of building capabilities.

	@tfield int population_growth_bonus
	@tfield int population_loyalty_bonus
	@tfield int population_health_bonus
	@tfield int trade_base_income_bonus
	@tfield int trade_level_bonus
	@tfield int trade_fleet
	@tfield int taxable_income_bonus
	@tfield int mine_resource
	@tfield int farming_level
	@tfield int road_level
	@tfield int gate_strength
	@tfield int gate_defences
	@tfield int wall_level
	@tfield int tower_level
	@tfield int armour
	@tfield int stage_games
	@tfield int stage_races
	@tfield int fire_risk
	@tfield int weapon_melee_simple
	@tfield int weapon_melee_blade
	@tfield int weapon_missile_mechanical
	@tfield int weapon_missile_gunpowder
	@tfield int weapon_artillery_mechanical
	@tfield int weapon_artillery_gunpowder
	@tfield int weapon_naval_gunpowder
	@tfield int upgrade_bodyguard
	@tfield int recruits_morale_bonus
	@tfield int recruits_exp_bonus
	@tfield int happiness_bonus
	@tfield int law_bonus
	@tfield int construction_cost_bonus_military
	@tfield int construction_cost_bonus_religious
	@tfield int construction_cost_bonus_defensive
	@tfield int construction_cost_bonus_other
	@tfield int construction_time_bonus_military
	@tfield int construction_time_bonus_religious
	@tfield int construction_time_bonus_defensive
	@tfield int construction_time_bonus_other
	@tfield int construction_cost_bonus_wooden
	@tfield int construction_cost_bonus_stone
	@tfield int construction_time_bonus_wooden
	@tfield int construction_time_bonus_stone
	@tfield int free_upkeep
	@tfield int pope_approval
	@tfield int pope_disapproval
	@tfield int religion_level
	@tfield int amplify_religion_level
	@tfield int archer_bonus
	@tfield int cavalry_bonus
	@tfield int heavy_cavalry_bonus
	@tfield int gun_bonus
	@tfield int navy_bonus
	@tfield int recruitment_cost_bonus_naval
	@tfield int retrain_cost_bonus
	@tfield int weapon_projectile
	@tfield int income_bonus
	@tfield int recruitment_slots

	@table buildingCapability
	*/
	luaState.new_enum(
		"buildingCapability",
		"population_growth_bonus", 0,
		"population_loyalty_bonus", 1,
		"population_health_bonus", 2,
		"trade_base_income_bonus", 3,
		"trade_level_bonus", 4,
		"trade_fleet", 5,
		"taxable_income_bonus", 6,
		"mine_resource", 7,
		"farming_level", 8,
		"road_level", 9,
		"gate_strength", 10,
		"gate_defences", 11,
		"wall_level", 12,
		"tower_level", 13,
		"armour", 14,
		"stage_games", 15,
		"stage_races", 16,
		"fire_risk", 17,
		"weapon_melee_simple", 18,
		"weapon_melee_blade", 19,
		"weapon_missile_mechanical", 20,
		"weapon_missile_gunpowder", 21,
		"weapon_artillery_mechanical", 22,
		"weapon_artillery_gunpowder", 23,
		"weapon_naval_gunpowder", 24,
		"upgrade_bodyguard", 25,
		"recruits_morale_bonus", 26,
		"recruits_exp_bonus", 27,
		"happiness_bonus", 28,
		"law_bonus", 29,
		"construction_cost_bonus_military", 30,
		"construction_cost_bonus_religious", 31,
		"construction_cost_bonus_defensive", 32,
		"construction_cost_bonus_other", 33,
		"construction_time_bonus_military", 34,
		"construction_time_bonus_religious", 35,
		"construction_time_bonus_defensive", 36,
		"construction_time_bonus_other", 37,
		"construction_cost_bonus_wooden", 38,
		"construction_cost_bonus_stone", 39,
		"construction_time_bonus_wooden", 40,
		"construction_time_bonus_stone", 41,
		"free_upkeep", 42,
		"pope_approval", 43,
		"pope_disapproval", 44,
		"religion_level", 45,
		"amplify_religion_level", 46,
		"archer_bonus", 47,
		"cavalry_bonus", 48,
		"heavy_cavalry_bonus", 49,
		"gun_bonus", 50,
		"navy_bonus", 51,
		"recruitment_cost_bonus_naval", 52,
		"retrain_cost_bonus", 53,
		"weapon_projectile", 54,
		"income_bonus", 55,
		"recruitment_slots", 56
	);

	
	/***
	Enum with a list of character types.

	@tfield int spy
	@tfield int assassin
	@tfield int diplomat
	@tfield int admiral
	@tfield int merchant
	@tfield int priest
	@tfield int general
	@tfield int named_character
	@tfield int princess
	@tfield int heretic
	@tfield int witch
	@tfield int inquisitor
	@tfield int pope

	@table characterType
	*/
	luaState.new_enum(
		"characterType",
		"spy", 0,
		"assassin", 1,
		"diplomat", 2,
		"admiral", 3,
		"merchant", 4,
		"priest", 5,
		"general", 6,
		"named_character", 7,
		"princess", 8,
		"heretic", 9,
		"witch", 10,
		"inquisitor", 11,
		"pope", 13
	);

	
	/***
	Enum with a list of army sort types.

	@tfield int eduType
	@tfield int category
	@tfield int class
	@tfield int soldierCount
	@tfield int experience
	@tfield int categoryClass
	@tfield int aiUnitValue

	@table sortType
	*/
	luaState.new_enum(
		"sortType",
		"eduType", 1,
		"category", 2,
		"class", 3,
		"soldierCount", 4,
		"experience", 5,
		"categoryClass", 6,
		"aiUnitValue", 7
	);

	
	/***
	Enum with a list of GTA AI plans.

	@tfield int doNothing
	@tfield int attackAll
	@tfield int defend
	@tfield int defendFeature
	@tfield int hide
	@tfield int ambush
	@tfield int scout
	@tfield int withdraw
	@tfield int attackSettlement
	@tfield int defendSettlement
	@tfield int sallyOut

	@table aiPlan
	*/
	luaState.new_enum(
		"aiPlan",
		"doNothing", 0,
		"attackAll", 1,
		"defend", 2,
		"defendFeature", 3,
		"hide", 4,
		"ambush", 5,
		"scout", 6,
		"withdraw", 7,
		"attackSettlement", 8,
		"defendSettlement", 9,
		"sallyOut", 10
	);

	
	/***
	Enum with a list of GTA AI objectives.

	@tfield int invalid
	@tfield int moveToPoint
	@tfield int attackEnemyBattleGroup
	@tfield int defendTerrainHill
	@tfield int defendTerrainForest
	@tfield int defendTerrainArea
	@tfield int defendCrossing
	@tfield int assaultCrossing
	@tfield int defendLine
	@tfield int scout
	@tfield int withdraw
	@tfield int defendSettlement
	@tfield int supportDefendSettlement
	@tfield int attackSettlement
	@tfield int skirmish
	@tfield int bombard
	@tfield int attackModel
	@tfield int sallyOut
	@tfield int ambush

	@table aiObjective
	*/
	luaState.new_enum(
		"aiObjective",
		"invalid", 0,
		"moveToPoint", 1,
		"attackEnemyBattleGroup", 2,
		"defendTerrainHill", 3,
		"defendTerrainForest", 4,
		"defendTerrainArea", 5,
		"defendCrossing", 6,
		"assaultCrossing", 7,
		"defendLine", 8,
		"scout", 9,
		"withdraw", 10,
		"defendSettlement", 11,
		"supportDefendSettlement", 12,
		"attackSettlement", 13,
		"skirmish", 14,
		"bombard", 15,
		"attackModel", 16,
		"sallyOut", 17,
		"ambush", 18
	);

	
	/***
	Enum with a list of resources.

	@tfield int gold
	@tfield int silver
	@tfield int fish
	@tfield int furs
	@tfield int grain
	@tfield int timber
	@tfield int iron
	@tfield int ivory
	@tfield int wine
	@tfield int slaves
	@tfield int chocolate
	@tfield int marble
	@tfield int textiles
	@tfield int dyes
	@tfield int tobacco
	@tfield int silk
	@tfield int sugar
	@tfield int sulfur
	@tfield int tin
	@tfield int spices
	@tfield int cotton
	@tfield int amber
	@tfield int coal
	@tfield int wool
	@tfield int elephants
	@tfield int camels
	@tfield int dogs
	@tfield int generic

	@table resourceType
	*/
	luaState.new_enum(
		"resourceType",
		"gold", 0,
		"silver", 1,
		"fish", 2,
		"furs", 3,
		"grain", 4,
		"timber", 5,
		"iron", 6,
		"ivory", 7,
		"wine", 8,
		"slaves", 9,
		"chocolate", 10,
		"marble", 11,
		"textiles", 12,
		"dyes", 13,
		"tobacco", 14,
		"silk", 15,
		"sugar", 16,
		"sulfur", 17,
		"tin", 18,
		"spices", 19,
		"cotton", 20,
		"amber", 21,
		"coal", 22,
		"wool", 23,
		"elephants", 24,
		"camels", 25,
		"dogs", 26,
		"generic", 27
	);

	
	/***
	Enum of unit morale status.

	@tfield int berserk
	@tfield int impetuous
	@tfield int high
	@tfield int firm
	@tfield int shaken
	@tfield int wavering
	@tfield int routing

	@table moraleStatus
	*/
	luaState.new_enum(
		"moraleStatus",
		"berserk", 0,
		"impetuous", 1,
		"high", 2,
		"firm", 3,
		"shaken", 4,
		"wavering", 5,
		"routing", 6
	);

	
	/***
	Enum of unit discipline.

	@tfield int berserker
	@tfield int impetuous
	@tfield int low
	@tfield int normal
	@tfield int disciplined

	@table unitDiscipline
	*/
	luaState.new_enum(
		"unitDiscipline",
		"berserker", 0,
		"impetuous", 1,
		"low", 2,
		"normal", 3,
		"disciplined", 4
	);

	
	/***
	Enum of unit training.

	@tfield int untrained
	@tfield int trained
	@tfield int highly_trained

	@table unitTraining
	*/
	luaState.new_enum(
		"unitTraining",
		"untrained", 0,
		"trained", 1,
		"highly_trained", 2
	);

	
	/***
	Enum of unit combat status.

	@tfield int notInCombat
	@tfield int victoryCertain
	@tfield int victoryAlmostCertain
	@tfield int victoryDistinct
	@tfield int balanced
	@tfield int defeatDistinct
	@tfield int defeatAlmostCertain
	@tfield int defeatCertain

	@table combatStatus
	*/
	luaState.new_enum(
		"combatStatus",
		"notInCombat", 0,
		"victoryCertain", 1,
		"victoryAlmostCertain", 2,
		"victoryDistinct", 3,
		"balanced", 4,
		"defeatDistinct", 5,
		"defeatAlmostCertain", 6,
		"defeatCertain", 7
	);

	
	/***
	Enum of battle success types.

	@tfield int close
	@tfield int average
	@tfield int clear
	@tfield int crushing

	@table battleSuccess
	*/
	luaState.new_enum(
		"battleSuccess",
		"close", 0,
		"average", 1,
		"clear", 2,
		"crushing", 3
	);

	
	/***
	Enum of unit mount class.

	@tfield int horse
	@tfield int camel
	@tfield int elephant
	@tfield int infantry

	@table mountClass
	*/
	luaState.new_enum(
		"mountClass",
		"horse", 0,
		"camel", 1,
		"elephant", 2,
		"infantry", 3
	);
	
	/***
	Enum of tile visibility status.

	@tfield int notVisibleInitial
	@tfield int notVisible
	@tfield int wasVisible
	@tfield int visible

	@table tileVisibility
	*/
	luaState.new_enum(
		"tileVisibility",
		"notVisibleInitial", 0,
		"notVisible", 1,
		"wasVisible", 2,
		"visible", 3
	);
	
	/***
	Enum of construction types.

	@tfield int upgrade
	@tfield int construction
	@tfield int repair
	@tfield int demolition
	@tfield int convert
	@tfield int none

	@table constructionType
	*/
	luaState.new_enum(
		"constructionType",
		"upgrade", 0,
		"construction", 1,
		"repair", 2,
		"demolition", 3,
		"convert", 4,
		"none", 5
	);

	
	/***
	Enum of battle types.

	@tfield int ambushSuccess
	@tfield int ambushFail
	@tfield int open
	@tfield int siege
	@tfield int sally
	@tfield int naval
	@tfield int withdrawal
	@tfield int meetEnemy

	@table battleType
	*/
	luaState.new_enum(
		"battleType",
		"ambushSuccess", 0,
		"ambushFail", 1,
		"open", 2,
		"siege", 3,
		"sally", 4,
		"naval", 5,
		"withdrawal", 6,
		"meetEnemy", 7
	);

	
	/***
	Enum of battle states.

	@tfield int notInBattle
	@tfield int preBattle
	@tfield int delay
	@tfield int deployment
	@tfield int deploymentPlayer2
	@tfield int conflict
	@tfield int victoryScroll
	@tfield int pursuit

	@table battleState
	*/
	luaState.new_enum(
		"battleState",
		"notInBattle", 0,
		"preBattle", 1,
		"delay", 2,
		"deployment", 3,
		"deploymentPlayer2", 4,
		"conflict", 5,
		"victoryScroll", 6,
		"pursuit", 7
	);

	
	/***
	Enum of unit classes

	@tfield int heavy
	@tfield int light
	@tfield int skirmish
	@tfield int spearmen
	@tfield int missile

	@table unitClass
	*/
	luaState.new_enum(
		"unitClass",
		"heavy",0,
		"light",1,
		"skirmish",2,
		"spearmen",3,
		"missile",4
	);
	
	/***
	Enum of unit categories

	@tfield int infantry
	@tfield int cavalry
	@tfield int siege
	@tfield int non_combatant
	@tfield int ship
	@tfield int handler

	@table unitCategory
	*/
	luaState.new_enum(
		"unitCategory",
		"infantry", 0,
		"cavalry", 1,
		"siege", 2,
		"non_combatant", 3,
		"ship", 4,
		"handler", 5
	);

	
	/***
	Enum of unit categories and class combo

	@tfield int nonCombatant
	@tfield int lightInfantry
	@tfield int heavyInfantry
	@tfield int spearmenInfantry
	@tfield int missileInfantry
	@tfield int lightCavalry
	@tfield int heavyCavalry
	@tfield int missileCavalry
	@tfield int siegeWeapon
	@tfield int animalHandler
	@tfield int battleShip

	@table unitCategoryClass
	*/
	luaState.new_enum(
		"unitCategoryClass",
		"nonCombatant", 0,
		"lightInfantry", 1,
		"heavyInfantry", 2,
		"spearmenInfantry", 3,
		"missileInfantry", 4,
		"lightCavalry", 5,
		"heavyCavalry", 6,
		"missileCavalry", 7,
		"siegeWeapon", 8,
		"animalHandler", 9,
		"battleShip", 10
	);

	
	/***
	Enum of automated settlement policy types

	@tfield int balanced
	@tfield int financial
	@tfield int military
	@tfield int growth
	@tfield int cultural
	@tfield int noPolicy

	@table managePolicy
	*/
	luaState.new_enum(
		"managePolicy",
		"balanced", 0,
		"financial", 1,
		"military", 2,
		"growth", 3,
		"cultural", 4,
		"noPolicy", 5
	);
	
	/***
	Enum of production controller extra bias

	@tfield int frontier
	@tfield int conflict
	@tfield int frontline
	@tfield int abandon
	@tfield int increaseGarrison
	@tfield int trade
	@tfield int tax
	@tfield int spies
	@tfield int assassins
	@tfield int merchants
	@tfield int priests
	@tfield int diplomats
	@tfield int ships
	@tfield int qualityUnits
	@tfield int buildStrengthLimit

	@table productionBias
	*/
	luaState.new_enum(
		"productionBias",
		"frontier", 0,
		"conflict", 1,
		"frontline", 2,
		"abandon", 3,
		"increaseGarrison", 4,
		"trade", 5,
		"tax", 6,
		"spies", 7,
		"assassins", 8,
		"merchants", 9,
		"priests", 10,
		"diplomats", 11,
		"ships", 12,
		"qualityUnits", 13,
		"buildStrengthLimit", 14
	);

	
	/***
	Enum of formation types

	@tfield int horde
	@tfield int column
	@tfield int square
	@tfield int wedge
	@tfield int squareHollow
	@tfield int phalanx
	@tfield int schiltrom
	@tfield int shieldWall
	@tfield int wall
	@tfield int movingThrough

	@table formationType
	*/
	luaState.new_enum(
		"formationType",
		"horde", 0,
		"column", 1,
		"square", 2,
		"wedge", 3,
		"squareHollow", 4,
		"phalanx", 5,
		"schiltrom", 6,
		"shieldWall", 7,
		"wall", 8,
		"movingThrough", 9
	);

	
	/***
	Enum of battle building types

	@tfield int ambient
	@tfield int gate
	@tfield int tower
	@tfield int wall

	@table battleBuildingType
	*/
	luaState.new_enum(
		"battleBuildingType",
		"ambient", 1,
		"gate", 3,
		"tower", 9,
		"wall", 10
	);
	
	/***
	Enum of battle ground types

	@tfield int grassShort
	@tfield int grassLong
	@tfield int sand
	@tfield int rock
	@tfield int forestDense
	@tfield int scrubDense
	@tfield int swamp
	@tfield int mud
	@tfield int mudRoad
	@tfield int stoneRoad
	@tfield int water
	@tfield int ice
	@tfield int snow
	@tfield int wood
	@tfield int dirt
	@tfield int unknown

	@table physicalGroundType
	*/
	luaState.new_enum(
		"physicalGroundType",
		"grassShort", 0,
		"grassLong", 1,
		"sand", 2,
		"rock", 3,
		"forestDense", 4,
		"scrubDense", 5,
		"swamp", 6,
		"mud", 7,
		"mudRoad", 8,
		"stoneRoad", 9,
		"water", 10,
		"ice", 11,
		"snow", 12,
		"wood", 13,
		"dirt", 14,
		"unknown", 15
	);
	
	/***
	Enum of engine types

	@tfield int catapult
	@tfield int trebuchet
	@tfield int ballista
	@tfield int bombard
	@tfield int grandBombard
	@tfield int hugeBombard
	@tfield int culverin
	@tfield int basilisk
	@tfield int cannon
	@tfield int mortar
	@tfield int scorpion
	@tfield int serpentine
	@tfield int rocketLauncher
	@tfield int ribault
	@tfield int monsterRibault
	@tfield int mangonel
	@tfield int tower
	@tfield int ram
	@tfield int ladder
	@tfield int holy_cart

	@table engineType
	*/
	luaState.new_enum(
		"engineType",
		"catapult", 0,
		"trebuchet", 1,
		"ballista", 2,
		"bombard", 3,
		"grandBombard", 4,
		"hugeBombard", 5,
		"culverin", 6,
		"basilisk", 7,
		"cannon", 8,
		"mortar", 9,
		"scorpion", 10,
		"serpentine", 11,
		"rocketLauncher", 12,
		"ribault", 13,
		"monsterRibault", 14,
		"mangonel", 15,
		"tower", 17,
		"ram", 18,
		"ladder", 19,
		"holy_cart", 20
	);
}

