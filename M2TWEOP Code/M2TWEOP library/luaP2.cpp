///
//![Lua logo](../Lua.png)
//M2TWEOP structures and functions. There are not many examples and descriptions here. Also note that the examples do not include many of the checks that would be required when creating modifications.
//@module LuaPlugin
//@author youneuoy
//@license GPL-3.0
#include "luaP.h"
#include "gameDataAllHelper.h"
#include "battleHandlerHelpers.h"
#include "gameHelpers.h"
#include "gameSTDUIHelpers.h"
#include "m2tweopHelpers.h"
#include "smallFuncs.h"
#include "unitActions.h"
#include "unitHelpers.h"


void luaP::initCampaign()
{
	struct
	{
		sol::usertype<campaign> campaignTable;
		sol::usertype<stratMap> stratMap;
		sol::usertype<oneTile> tileStruct;
		sol::usertype<regionStruct> regionStruct;
		sol::usertype<mercPool> mercPool;
		sol::usertype<mercPoolUnit> mercPoolUnit;
		sol::usertype<collegeOfCardinals> collegeOfCardinals;
		sol::usertype<crusade> crusadeStruct;
		sol::usertype<jihad> jihadStruct;
		sol::usertype<options1> options1;
		sol::usertype<options2> options2;
		sol::usertype<campaignDifficulty1> campaignDifficulty1;
		sol::usertype<campaignDifficulty2> campaignDifficulty2;
		sol::usertype<uiCardManager> uiCardManager;
		sol::usertype<coordPair> coordPair;
		sol::usertype<landMass> landMass;
		sol::usertype<roadStruct> roadStruct;
		sol::usertype<seaConnectedRegion> seaConnectedRegion;
		sol::usertype<neighbourRegion> neighbourRegion;
		sol::usertype<eventTrigger> eventTrigger;
		sol::usertype<selectionInfo> selectionInfo;
		sol::usertype<mapImage> mapImageStruct;
		sol::usertype<buildingInfoScroll> buildingInfoScroll;
		sol::usertype<unitInfoScroll> unitInfoScroll;
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

	///eventTrigger
	//@section eventTrigger

	/***
	This is data that comes with game events. You need to check the event documentation to see what data is available under "Exports". The rest of the fields not stated inside "Exports" will return nil! 
	
	@tfield unit attackingUnit
	@tfield unit defendingUnit
	@tfield character stratCharacter
	@tfield namedCharacter character - Note it is namedCharacter, not character
	@tfield namedCharacter targetCharacter - Note it is namedCharacter, not character
	@tfield settlementStruct settlement
	@tfield settlementStruct targetSettlement
	@tfield fortStruct fort
	@tfield factionStruct faction
	@tfield factionStruct targetFaction
	@tfield stackStruct army
	@tfield int regionID
	@tfield int targetRegionID
	@tfield unit playerUnit
	@tfield unit enemyUnit
	@tfield buildingBattle battleBuilding
	@tfield buildingInQueue priorBuild
	@tfield string resourceDescription
	@tfield eduEntry eduEntry
	@tfield int characterType
	@tfield int targetCharacterType
	@tfield string disasterType
	@tfield string missionSuccessLevel
	@tfield int missionProbability
	@tfield string missionDetails
	@tfield int eventID
	@tfield int guildID
	@tfield string eventCounter
	@tfield coordPair coords
	@tfield int religion
	@tfield int targetReligion
	@tfield float amount
	@tfield crusadeStruct crusade
	@tfield capturedFactionInfo captureInfo
	@tfield string ransomType
	@tfield unit unit
	
	
	@table eventTrigger
	*/
	typeAll.eventTrigger = luaState.new_usertype<eventTrigger>("eventTrigger");
	typeAll.eventTrigger.set("attackingUnit", sol::property(gameHelpers::getEventAttackingUnit));
	typeAll.eventTrigger.set("defendingUnit", sol::property(gameHelpers::getEventDefendingUnit));
	typeAll.eventTrigger.set("stratCharacter", sol::property(gameHelpers::getEventCharacter));
	typeAll.eventTrigger.set("character", sol::property(gameHelpers::getEventNamedCharacter));
	typeAll.eventTrigger.set("targetCharacter", sol::property(gameHelpers::getEventTargetNamedCharacter));
	typeAll.eventTrigger.set("settlement", sol::property(gameHelpers::getEventSettlement));
	typeAll.eventTrigger.set("targetSettlement", sol::property(gameHelpers::getEventTargetSettlement));
	typeAll.eventTrigger.set("fort", sol::property(gameHelpers::getEventFort));
	typeAll.eventTrigger.set("faction", sol::property(gameHelpers::getEventFaction));
	typeAll.eventTrigger.set("targetFaction", sol::property(gameHelpers::getEventTargetFaction));
	typeAll.eventTrigger.set("army", sol::property(gameHelpers::getEventArmy));
	typeAll.eventTrigger.set("regionID", sol::property(gameHelpers::getEventRegion));
	typeAll.eventTrigger.set("targetRegionID", sol::property(gameHelpers::getEventTargetRegion));
	typeAll.eventTrigger.set("playerUnit", sol::property(gameHelpers::getEventPlayerUnit));
	typeAll.eventTrigger.set("enemyUnit", sol::property(gameHelpers::getEventEnemyUnit));
	typeAll.eventTrigger.set("battleBuilding", sol::property(gameHelpers::getBuildingBattle));
	typeAll.eventTrigger.set("priorBuild", sol::property(gameHelpers::getPriorBuild));
	typeAll.eventTrigger.set("resourceDescription", sol::property(gameHelpers::getResourceDescription));
	typeAll.eventTrigger.set("eduEntry", sol::property(gameHelpers::getEduEntry));
	typeAll.eventTrigger.set("characterType", sol::property(gameHelpers::getEventCharacterType));
	typeAll.eventTrigger.set("targetCharacterType", sol::property(gameHelpers::getEventTargetCharacterType));
	typeAll.eventTrigger.set("disasterType", sol::property(gameHelpers::getEventType));
	typeAll.eventTrigger.set("missionSuccessLevel", sol::property(gameHelpers::getMissionSuccessLevel));
	typeAll.eventTrigger.set("missionProbability", sol::property(gameHelpers::getMissionProbability));
	typeAll.eventTrigger.set("missionDetails", sol::property(gameHelpers::getMissionDetails));
	typeAll.eventTrigger.set("eventID", sol::property(gameHelpers::getEventID));
	typeAll.eventTrigger.set("guildID", sol::property(gameHelpers::getEventGuild));
	typeAll.eventTrigger.set("eventCounter", sol::property(gameHelpers::getEventCounter));
	typeAll.eventTrigger.set("coords", sol::property(gameHelpers::getPosition));
	typeAll.eventTrigger.set("religion", sol::property(gameHelpers::getReligion));
	typeAll.eventTrigger.set("targetReligion", sol::property(gameHelpers::getTargetReligion));
	typeAll.eventTrigger.set("amount", sol::property(gameHelpers::getAmount));
	typeAll.eventTrigger.set("crusade", sol::property(gameHelpers::getCrusade));
	typeAll.eventTrigger.set("captureInfo", sol::property(gameHelpers::getCapturedFactionInfo));
	typeAll.eventTrigger.set("ransomType", sol::property(gameHelpers::getRansomType));
	typeAll.eventTrigger.set("unit", sol::property(gameHelpers::getUnit));
	
	
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
	typeAll.mapImageStruct.set_function("makeMapImage", &m2tweopHelpers::makeMapImage);
	
	/***
	Reset image state.
	@function mapImageStruct:clearMapImage
	@usage
	local mapImage = mapImageStruct.makeMapImage();
	mapImage:clearMapImage();
	*/
	typeAll.mapImageStruct.set_function("clearMapImage", &m2tweopHelpers::clearMapImage);
	
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
	typeAll.mapImageStruct.set_function("loadMapTexture", &m2tweopHelpers::loadMapTexture);
	
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
	typeAll.mapImageStruct.set_function("fillRegionColor", &m2tweopHelpers::fillRegionColor);
	
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
	typeAll.mapImageStruct.set_function("addRegionColor", &m2tweopHelpers::addRegionColor);
	
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
	typeAll.mapImageStruct.set_function("fillTileColor", &m2tweopHelpers::fillTileColor);
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
	typeAll.mapImageStruct.set_function("addTileColor", &m2tweopHelpers::addTileColor);

	///unitInfoScroll
	//@section Info scrolls

	/***
	Basic unitInfoScroll table

	@tfield unit unit If the scroll is about existing unit, this is set and eduEntry empty.
	@tfield eduEntry eduEntry only for non-recruited units.

	@table unitInfoScroll
	*/
	typeAll.unitInfoScroll = luaState.new_usertype<unitInfoScroll>("unitInfoScroll");
	typeAll.unitInfoScroll.set("unit", &unitInfoScroll::unit);
	typeAll.unitInfoScroll.set("eduEntry", &unitInfoScroll::entry);

	/***
	Basic buildingInfoScroll table

	@tfield settlementStruct settlement
	@tfield building building If the scroll is about existing building, this is set and edbEntry empty.
	@tfield edbEntry edbEntry only for non-constructed buildings.

	@table buildingInfoScroll
	*/
	typeAll.buildingInfoScroll = luaState.new_usertype<buildingInfoScroll>("buildingInfoScroll");
	typeAll.buildingInfoScroll.set("settlement", &buildingInfoScroll::settlement);
	typeAll.buildingInfoScroll.set("building", &buildingInfoScroll::building);
	typeAll.buildingInfoScroll.set("edbEntry", &buildingInfoScroll::entry);

	///uiCardManager
	//@section uiCardManager

	/***
	Basic uiCardManager table

	@tfield int selectedUnitCardsCount
	@tfield int unitCardsCount
	@tfield settlementStruct selectedSettlement
	@tfield character selectedCharacter
	@tfield fortStruct selectedFort
	@tfield getSelectedUnitCard getSelectedUnitCard
	@tfield getUnitCard getUnitCard
	@tfield getBuildingInfoScroll getBuildingInfoScroll
	@tfield getUnitInfoScroll getUnitInfoScroll

	@table uiCardManager
	*/
	typeAll.uiCardManager = luaState.new_usertype<uiCardManager>("uiCardManager");
	typeAll.uiCardManager.set("selectedUnitCardsCount", &uiCardManager::selectedUnitCardsCount);
	typeAll.uiCardManager.set("unitCardsCount", &uiCardManager::unitCardsCount);
	typeAll.uiCardManager.set("selectedSettlement", &uiCardManager::selectedSettlement);
	typeAll.uiCardManager.set("selectedCharacter", &uiCardManager::selectedCharacter);
	typeAll.uiCardManager.set("selectedFort", &uiCardManager::selectedFort);
	/***
	Get selected unit card by index.
	@function uiCardManager:getSelectedUnitCard
	@tparam int index
	@treturn unit selectedUnit
	@usage
	local cardManager=gameDataAll.get().uiCardManager;
	local selectedUnit=cardManager:getSelectedUnitCard(0);
	*/
	typeAll.uiCardManager.set_function("getSelectedUnitCard", &gameHelpers::getSelectedUnitCard);
	/***
	Get unit card by index (battle or strat).
	@function uiCardManager:getUnitCard
	@tparam int index
	@treturn unit unit
	@usage
	local cardManager=gameDataAll.get().uiCardManager;
	local unit=cardManager:getUnitCard(0);
	*/
	typeAll.uiCardManager.set_function("getUnitCard", &gameHelpers::getUnitCard);
	/***
	Get building info scroll.
	@function uiCardManager.getBuildingInfoScroll
	@treturn buildingInfoScroll scroll
	@usage
	local infoScroll = cardManager.getBuildingInfoScroll();
	*/
	typeAll.uiCardManager.set_function("getBuildingInfoScroll", &gameSTDUIHelpers::getBuildingInfoScroll);
	/***
	Get unit info scroll.
	@function uiCardManager.getUnitInfoScroll
	@treturn unitInfoScroll scroll
	@usage
	local infoScroll = cardManager.getUnitInfoScroll();
	*/
	typeAll.uiCardManager.set_function("getUnitInfoScroll", &gameSTDUIHelpers::getUnitInfoScroll);
	
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
	typeAll.selectionInfo.set("selectedCharacter", sol::property(&gameDataAllHelper::getSelectedCharacter));
	typeAll.selectionInfo.set("hoveredCharacter", sol::property(&gameDataAllHelper::getHoveredCharacter));
	typeAll.selectionInfo.set("selectedEnemyCharacter", sol::property(&gameDataAllHelper::getSelectedEnemyCharacter));
	typeAll.selectionInfo.set("selectedSettlement", sol::property(&gameDataAllHelper::getSelectedSettlement));
	typeAll.selectionInfo.set("hoveredSettlement", sol::property(&gameDataAllHelper::getHoveredSettlement));
	typeAll.selectionInfo.set("selectedEnemySettlement", sol::property(&gameDataAllHelper::getSelectedEnemySettlement));
	typeAll.selectionInfo.set("selectedFort", sol::property(&gameDataAllHelper::getSelectedFort));
	typeAll.selectionInfo.set("hoveredFort", sol::property(&gameDataAllHelper::getHoveredFort));
	typeAll.selectionInfo.set("selectedEnemyFort", sol::property(&gameDataAllHelper::getSelectedEnemyFort));

	///Campaign
	//@section campaignStruct

	/***
	Basic campaign table.

	@tfield int playerFactionId
	@tfield int[31] campaignDifficultyFaction Indexing starts at 1, so add 1 to faction ID. Maximum 31.
	@tfield int[31] battleDifficultyFaction Indexing starts at 1, so add 1 to faction ID. Maximum 31.
	@tfield factionStruct[31] factionsSortedByDescrStrat Table of factionStruct[31], indexing starts at 1. Maximum 31. Slightly misleading name, sorted by the turn order of the factions. Player controlled faction is at index 0 in single player.
	@tfield factionStruct[31] factionsSortedByID Table of factionStruct[31], indexing starts at 1, so add 1 to faction ID. Maximum 31.
	@tfield int numberOfFactions
	@tfield int numberHumanFactions Number of player-controlled factions.
	@tfield factionStruct currentFaction Faction whose turn it is at the moment, can be set.
	@tfield collegeOfCardinals collegeOfCardinals
	@tfield factionStruct papalFaction
	@tfield int fogOfWar
	@tfield int factionTurnID
	@tfield int tickCount
	@tfield int millisecondCount
	@tfield float secondCount
	@tfield int turnNumber
	@tfield float timescale Factor for number of turns per year, see descr\_strat.txt
	@tfield settlementStruct romeSettlement
	@tfield settlementStruct constantinopleSettlement
	@tfield crusadeStruct crusade
	@tfield jihadStruct jihad
	@tfield float BrigandSpawnValue Lower values increase spawn rate.
	@tfield float PirateSpawnValue Lower values increase spawn rate.
	@tfield int restrictAutoResolve
	@tfield int saveEnabled
	@tfield int FreeUpkeepForts Number of units who get free upkeep in forts.
	@tfield float currentDate
	@tfield int currentSeason season (0=summer, 1=winter)
	@tfield float startDate
	@tfield int startSeason season (0=summer, 1=winter)
	@tfield float endDate
	@tfield int endSeason season (0=summer, 1=winter)
	@tfield int daysInBattle
	@tfield float currentTimeInBattle 24 max, so calculate as daysInBattle*24+currentTimeInBattle.
	@tfield int fortsNum
	@tfield int portsBuildingsNum
	@tfield int watchTowerNum
	@tfield int slaveFactionID
	@tfield int roadsNum
	@tfield int nightBattlesEnabled
	@tfield int rebellingCharactersActive
	@tfield checkDipStance checkDipStance
	@tfield setDipStance setDipStance
	@tfield GetUnitSize GetUnitSize
	@tfield getCampaignPath getCampaignPath
	@tfield getFort getFort
	@tfield getPort getPort
	@tfield getWatchTower getWatchTower
	@tfield getFaction getFaction
	@tfield getSettlementByName getSettlementByName
	@tfield getMercPool getMercPool
	@tfield getRoad getRoad

	@table campaignStruct
	*/
	typeAll.campaignTable = luaState.new_usertype<campaign>("campaignStruct");
	typeAll.campaignTable.set("playerFactionId", &campaign::playerFactionId);
	typeAll.campaignTable.set("campaignDifficultyFaction", sol::property([](campaign& self) { return std::ref(self.campaignDifficultyFaction); }));
	typeAll.campaignTable.set("battleDifficultyFaction", sol::property([](campaign& self) { return std::ref(self.battleDifficultyFaction); }));
	typeAll.campaignTable.set("factionsSortedByDescrStrat", sol::property([](campaign& self) { return std::ref(self.factionsSortedByDescrStrat); }));
	typeAll.campaignTable.set("factionsSortedByID", sol::property([](campaign& self) { return std::ref(self.factionsSortedByID); }));
	typeAll.campaignTable.set("numberOfFactions", &campaign::numberOfFactionsWithSlave);
	typeAll.campaignTable.set("papalFaction", &campaign::papalFaction);
	typeAll.campaignTable.set("numberHumanFactions", &campaign::humanPlayers);
	typeAll.campaignTable.set("collegeOfCardinals", &campaign::collegeOfCardinals);
	typeAll.campaignTable.set("currentFaction", &campaign::currentFactionTurn);
	typeAll.campaignTable.set("factionTurnID", &campaign::factionTurnID);
	typeAll.campaignTable.set("tickCount", &campaign::tickCount);
	typeAll.campaignTable.set("millisecondCount", &campaign::millisecondCount);
	typeAll.campaignTable.set("secondCount", &campaign::secondCount);
	typeAll.campaignTable.set("fogOfWar", &campaign::fogOfWar);
	typeAll.campaignTable.set("passedTurnsNum", &campaign::TurnNumber);
	typeAll.campaignTable.set("turnNumber", &campaign::TurnNumber);
	typeAll.campaignTable.set("crusade", &campaign::crusade);
	typeAll.campaignTable.set("jihad", &campaign::jihad);

	typeAll.campaignTable.set("timescale", &campaign::TimeScale);
	typeAll.campaignTable.set("slaveFactionID", &campaign::slaveFactionID);
	typeAll.campaignTable.set("romeSettlement", &campaign::rome);
	typeAll.campaignTable.set("constantinopleSettlement", &campaign::constantinople);

	typeAll.campaignTable.set("BrigandSpawnValue", &campaign::BrigandSpawnValue);
	typeAll.campaignTable.set("PirateSpawnValue", &campaign::PirateSpawnValue);
	typeAll.campaignTable.set("restrictAutoResolve", &campaign::restrictAutoResolve);
	typeAll.campaignTable.set("saveEnabled", &campaign::saveEnabled);
	typeAll.campaignTable.set("FreeUpkeepForts", &campaign::FreeUpkeepForts);
	typeAll.campaignTable.set("rebellingCharactersActive", &campaign::rebellingCharactersActive);
	typeAll.campaignTable.set("nightBattlesEnabled", &campaign::nightBattlesEnabled);

	typeAll.campaignTable.set("currentDate", &campaign::currentDate);
	typeAll.campaignTable.set("currentSeason", &campaign::season);

	typeAll.campaignTable.set("startDate", &campaign::startDate);
	typeAll.campaignTable.set("startSeason", &campaign::startSeason);

	typeAll.campaignTable.set("endDate", &campaign::endDate);
	typeAll.campaignTable.set("endSeason", &campaign::endSeason);

	typeAll.campaignTable.set("daysInBattle", &campaign::daysInBattle);
	typeAll.campaignTable.set("currentTimeInBattle", &campaign::currentTimeInBattle);
	typeAll.campaignTable.set("fortsNum", &campaign::fortsNum);
	typeAll.campaignTable.set("roadsNum", &campaign::roadsNum);
	typeAll.campaignTable.set("portsBuildingsNum", &campaign::portsBuildingsNum);
	typeAll.campaignTable.set("watchTowerNum", &campaign::watchtowersNum);
	/***
	Check if a diplomatic relation between two factions.
	@function campaignStruct:checkDipStance
	@tparam dipRelType checkType Example: dipRelType.war
	@tparam factionStruct fac1 A faction.
	@tparam factionStruct fac2 Another faction.
	@treturn bool checkResult
	@usage
	local campaign=gameDataAll.get().campaignStruct;
	local fac1=campaign.factionsSortedByDescrStrat[1];
	local fac2=campaign.factionsSortedByDescrStrat[2];
	local isInWar=campaign:checkDipStance(dipRelType.war,fac1,fac2);
	*/
	typeAll.campaignTable.set_function("checkDipStance", &m2tweopHelpers::checkDipStance);
	/***
	Set a diplomatic relation between two factions.
	@function campaignStruct:setDipStance
	@tparam dipRelType relType Example: dipRelType.war
	@tparam factionStruct fac1 A faction.
	@tparam factionStruct fac2 Another faction.
	@usage
	local campaign=gameDataAll.get().campaignStruct;
	local fac1=campaign.factionsSortedByDescrStrat[1];
	local fac2=campaign.factionsSortedByDescrStrat[2];
	campaign:setDipStance(dipRelType.war,fac1,fac2);
	*/
	typeAll.campaignTable.set_function("setDipStance", &m2tweopHelpers::setDipStance);
	/***
	Get size of unit(i.e. small or medium, etc). Numbers from 0 to 3
	@function campaignStruct:GetUnitSize
	@treturn int unitSize
	@usage
	local campaign=gameDataAll.get().campaignStruct;
	local unitSize=campaign:GetUnitSize();
	*/
	typeAll.campaignTable.set_function("GetUnitSize", &smallFuncs::GetUnitSize);
	/***
	Get fort by index.
	@function campaignStruct:getFort
	@tparam int index
	@treturn fortStruct fort
	@usage
	local campaign=gameDataAll.get().campaignStruct;
	local fort=campaign:getFort(0);
	*/
	typeAll.campaignTable.set_function("getFort", &gameHelpers::getFortAll);
	/***
	Get port by index.
	@function campaignStruct:getPort
	@tparam int index
	@treturn portStruct port
	@usage
	local campaign=gameDataAll.get().campaignStruct;
	local port=campaign:getPort(0);
	*/
	typeAll.campaignTable.set_function("getPort", &gameHelpers::getPortAll);
	/***
	Get watchtower by index.
	@function campaignStruct:getWatchTower
	@tparam int index
	@treturn watchtowerStruct watchtower
	@usage
	local campaign=gameDataAll.get().campaignStruct;
	local watchtower=campaign:getWatchTower(0);
	*/
	typeAll.campaignTable.set_function("getWatchTower", &gameHelpers::getWatchTowerAll);
	/***
	Get settlement by internal name.
	@function campaignStruct:getSettlementByName
	@tparam string name
	@treturn settlementStruct settlement
	@usage
	local campaign=gameDataAll.get().campaignStruct;
	local sett=campaign:getSettlementByName("London");
	*/
	typeAll.campaignTable.set_function("getSettlementByName", &gameHelpers::getSettlementByName);
	/***
	Get a faction by it's internal name.
	@function campaignStruct:getFaction
	@tparam string name
	@treturn factionStruct faction
	@usage
	local CAMPAIGN = gameDataAll.get().campaignStruct;
	local fac = CAMPAIGN:getFaction("england");
	*/
	typeAll.campaignTable.set_function("getFaction", &gameHelpers::getFactionHashed);
	/***
	Get path to the current descr\_strat file used.
	@function campaignStruct:getCampaignPath
	@treturn string path
	@usage
	local campaign = gameDataAll.get().campaignStruct;
	local path = campaign:getCampaignPath();
	*/
	typeAll.campaignTable.set_function("getCampaignPath", &gameHelpers::getCampaignPath);
	/***
	Get a mercenary pool by name.
	@function campaignStruct:getMercPool
	@tparam string name
	@treturn mercPool pool
	@usage
	local campaign = gameDataAll.get().campaignStruct;
	local pool = campaign:getMercPool("mercpool1");
	*/
	typeAll.campaignTable.set_function("getMercPool", &gameHelpers::getMercPool);
	/***
	Get a road by index.
	@function campaignStruct:getRoad
	@tparam int index
	@treturn roadStruct road
	@usage
	local campaign = gameDataAll.get().campaignStruct;
	local pool = campaign:getRoad(0);
	*/
	typeAll.campaignTable.set_function("getRoad", &gameHelpers::getRoad);

	///CollegeofCardinals
	//@section collegeOfCardinals

	/***
	Basic College of Cardinals table.
	@tfield namedCharacter pope
	@tfield int cardinalNum
	@tfield getCardinal getCardinal

	@table collegeOfCardinals
	*/
	typeAll.collegeOfCardinals = luaState.new_usertype<collegeOfCardinals>("collegeOfCardinals");
	typeAll.collegeOfCardinals.set("pope", &collegeOfCardinals::pope);
	typeAll.collegeOfCardinals.set("cardinalNum", &collegeOfCardinals::cardinalNum);

	/***
	Get a specific cardinal by index.
	@function collegeOfCardinals:getCardinal
	@tparam int index
	@treturn character cardinal
	@usage
	local college = campaign.collegeOfCardinals;
	local cardinal = college:getCardinal(2);
	*/
	typeAll.collegeOfCardinals.set_function("getCardinal", &gameHelpers::getCardinal);

	/// Crusade
	//@section crusade

	/***
	Basic crusade table.

	@tfield int startTurn
	@tfield int endTurn
	@tfield settlementStruct targetSettlement
	@tfield int length
	@tfield int outcome

	@table crusadeStruct
	*/
	typeAll.crusadeStruct = luaState.new_usertype<crusade>("crusadeStruct");
	typeAll.crusadeStruct.set("startTurn", &crusade::startTurn);
	typeAll.crusadeStruct.set("endTurn", &crusade::endTurn);
	typeAll.crusadeStruct.set("targetSettlement", &crusade::targetSettlement);
	typeAll.crusadeStruct.set("length", &crusade::length);
	typeAll.crusadeStruct.set("outcome", &crusade::outcome);

	/// Jihad
	//@section jihad

	/***
	Basic jihad table.

	@tfield int startTurn
	@tfield int endTurn
	@tfield settlementStruct targetSettlement
	@tfield int length
	@tfield int outcome

	@table jihadStruct
	*/
	typeAll.jihadStruct = luaState.new_usertype<jihad>("jihadStruct");
	typeAll.jihadStruct.set("startTurn", &jihad::startTurn);
	typeAll.jihadStruct.set("endTurn", &jihad::endTurn);
	typeAll.jihadStruct.set("targetSettlement", &jihad::targetSettlement);
	typeAll.jihadStruct.set("length", &jihad::length);
	typeAll.jihadStruct.set("outcome", &jihad::outcome);

	/// coords
	//@section coordPair

	/***
	Basic coordPair table.

	@tfield int xCoord
	@tfield int yCoord

	@table coordPair
	*/
	typeAll.coordPair = luaState.new_usertype<coordPair>("coordPair");
	typeAll.coordPair.set("xCoord", &coordPair::xCoord);
	typeAll.coordPair.set("yCoord", &coordPair::yCoord);

	/// Strat Map
	//@section stratMap

	/***
	Basic strat map table.

	@tfield int mapWidth
	@tfield int mapHeight
	@tfield int regionsNum
	@tfield int volcanoesNum
	@tfield int landConnectionsNum
	@tfield int landMassNum
	@tfield getRegion getRegion
	@tfield getTile getTile
	@tfield getVolcanoCoords getVolcanoCoords
	@tfield getLandConnection getLandConnection
	@tfield getLandMass getLandMass
	@tfield getSettlement getSettlement
	@tfield getRegionByName getRegionByName

	@table stratMap
	*/
	typeAll.stratMap = luaState.new_usertype<stratMap>("stratMap");
	typeAll.stratMap.set("mapWidth", &stratMap::mapWidth);
	typeAll.stratMap.set("mapHeight", &stratMap::mapHeight);
	typeAll.stratMap.set("regionsNum", &stratMap::regionsNum);
	typeAll.stratMap.set("volcanoesNum", &stratMap::volcanoesNum);
	typeAll.stratMap.set("landConnectionsNum", &stratMap::landMassConnectionsNum);
	typeAll.stratMap.set("landMassNum", &stratMap::landMassNum);

	/***
	Get a specific region by index.
	@function stratMap.getRegion
	@tparam int index
	@treturn regionStruct region
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	*/
	typeAll.stratMap.set_function("getRegion", &gameHelpers::getRegion);

	/***
	Get a specific tile by it's coordinates.
	@function stratMap.getTile
	@tparam int x
	@tparam int y
	@treturn tileStruct tile
	@usage
	local sMap = gameDataAll.get().stratMap;
	local tile = sMap.getTile(182, 243);
	*/
	typeAll.stratMap.set_function("getTile", &gameHelpers::getTile);

	/***
	Get a volcano's coordinates.
	@function stratMap:getVolcanoCoords
	@tparam int index
	@treturn coordPair tile
	@usage
	local sMap = gameDataAll.get().stratMap;
	local volcano = sMap:getVolcanoCoords(0);
	*/
	typeAll.stratMap.set_function("getVolcanoCoords", &gameHelpers::getVolcanoCoords);

	/***
	Get a land connection's coordinates (the green arrows on the map that allow you to cross bodies of water).
	@function stratMap:getLandConnection
	@tparam int index
	@treturn coordPair tile
	@usage
	local sMap = gameDataAll.get().stratMap;
	local connection = sMap:getLandConnection(0);
	*/
	typeAll.stratMap.set_function("getLandConnection", &gameHelpers::getLandConnection);

	/***
	Get a landmass (collection of regions reachable by land, like a continent or island).
	@function stratMap:getLandMass
	@tparam int index
	@treturn landMass landMass
	@usage
	local sMap = gameDataAll.get().stratMap;
	local tile = sMap:getLandMass(0);
	*/
	typeAll.stratMap.set_function("getLandMass", &gameHelpers::getLandMass);

	/***
	Get a settlement by its internal name. This is the intended way to get a settlement quickly by it's name.
	@function stratMap:getSettlement
	@tparam string name
	@treturn settlementStruct settlement
	@usage
	local STRAT_MAP = gameDataAll.get().stratMap;
	local london = STRAT_MAP:getSettlement("London");
	*/
	typeAll.stratMap.set_function("getSettlement", &gameHelpers::getSettlement);

	/***
	Get a region by its internal name.
	@function stratMap:getRegionByName
	@tparam string name
	@treturn regionStruct region
	@usage
	local STRAT_MAP = gameDataAll.get().stratMap;
	local londonRegion = STRAT_MAP:getRegionByName("London_Province");
	*/
	typeAll.stratMap.set_function("getRegionByName", &gameHelpers::getRegionByName);

	/// landMass
	//@section landMass

	/***
	Basic landMass table.

	@tfield int index
	@tfield int regionsNum
	@tfield getRegionID getRegionID

	@table landMass
	*/
	typeAll.landMass = luaState.new_usertype<landMass>("landMass");
	typeAll.landMass.set("index", &landMass::index);
	typeAll.landMass.set("regionsNum", &landMass::regionsNum);

	/***
	Get a region ID by index.
	@function landMass:getRegionID
	@tparam int index
	@treturn int regionID
	@usage
	local region = landMass:getRegionID(0);
	*/
	typeAll.landMass.set_function("getRegionID", &gameHelpers::getRegionID);

	/// roadStruct
	//@section roadStruct

	/***
	Basic roadStruct table.

	@tfield int coordsNum
	@tfield int regionIdStart
	@tfield int regionIdEnd
	@tfield getCoord getCoord
	@tfield int tradeValue --only counts from 1 side (importer)

	@table roadStruct
	*/
	typeAll.roadStruct = luaState.new_usertype<roadStruct>("roadStruct");
	typeAll.roadStruct.set("coordsNum", &roadStruct::coordsNum);
	typeAll.roadStruct.set("regionIdStart", &roadStruct::regionIdStart);
	typeAll.roadStruct.set("regionIdEnd", &roadStruct::regionIdEnd);
	typeAll.roadStruct.set("tradeValue", &roadStruct::tradeValue);

	/***
	Get a road coord by index.
	@function roadStruct:getCoord
	@tparam int index
	@treturn coordPair coords
	@usage
	local coords = road:getCoord(0);
	*/
	typeAll.roadStruct.set_function("getCoord", &gameHelpers::getRoadCoord);

	///Tile
	//@section tileStruct

	/***
	Basic tile table.

	@tfield roadStruct road
	@tfield int isLand (1 = land, 0 = sea)
	@tfield int groundType
	0 low fertility
	1 medium fertility
	2 high fertility
	3 wilderness
	4 high moutains
	5 low moutains
	6 hills
	7 dense forest
	8 woodland
	9 swamp
	10 ocean
	11 deep sea
	12 shallow sea
	13 coast (beach)
	14 impassable_land
	15 impassable_sea
	@tfield int regionID
	@tfield tradeResource|nil resource
	@tfield character|nil character
	@tfield settlementStruct|nil settlement
	@tfield fortStruct|nil fort
	@tfield portStruct|nil port
	@tfield watchtowerStruct|nil watchtower
	@tfield int height
	@tfield int climate
	@tfield int heatValue
	@tfield int factionID
	@tfield int xCoord
	@tfield int yCoord
	@tfield int objectTypes bitfield, from left to right: unknown, character, ship, watchtower, port, ship, fort, settlement.
	@tfield bool hasRiver
	@tfield bool hasRiverSource
	@tfield bool hasCrossing
	@tfield bool hasCharacter
	@tfield bool hasShip
	@tfield bool hasWatchtower
	@tfield bool hasPort
	@tfield bool hasFort
	@tfield bool hasCliff
	@tfield bool hasSettlement
	@tfield bool isDevastated
	@tfield bool isCoastalWater
	@tfield int roadLevel Settlement tiles return 3.
	@tfield bool isLandConnection Crossing created by green arrows.
	@tfield bool isSeaCrossing Crossing created by close proximity, not green arrows.
	@tfield int border 1 = border, 2 = seaBorder, 3 = sea edge border (point where the region border both another land region and sea).
	@tfield int armiesNearTile bitfield of faction id's (counts both tile and the 8 tiles around it, if you want only on tile combine with charactersOnTile).
	@tfield int charactersOnTile bitfield of faction id's
	@tfield float mpModifier
	@tfield factionHasArmyNearTile factionHasArmyNearTile
	@tfield factionHasCharacterOnTile factionHasCharacterOnTile
	@tfield getTileCharacterCount getTileCharacterCount
	@tfield getTileCharacterAtIndex getTileCharacterAtIndex
	@tfield int hasRoad
	@tfield int borderField
	@tfield int otherField
	@tfield int choke
	@tfield int ModelIsHills
	
	@table tileStruct
	*/
	typeAll.tileStruct = luaState.new_usertype<oneTile>("tileStruct");
	typeAll.tileStruct.set("road", &oneTile::road);
	typeAll.tileStruct.set("isLand", &oneTile::isLand);
	typeAll.tileStruct.set("groundType", sol::property(&gameHelpers::getTileGroundType, &gameHelpers::setTileGroundType));
	typeAll.tileStruct.set("regionID", &oneTile::regionId);
	typeAll.tileStruct.set("objectTypes", &oneTile::objectTypes);
	typeAll.tileStruct.set("otherField", &oneTile::otherField);
	typeAll.tileStruct.set("borderField", &oneTile::border);
	typeAll.tileStruct.set("hasRoad", &oneTile::hasRoad);
	typeAll.tileStruct.set("choke", &oneTile::choke);
	typeAll.tileStruct.set("ModelIsHills", &oneTile::ModelIsHills);
	typeAll.tileStruct.set("xCoord", sol::property(gameHelpers::getTileX));
	typeAll.tileStruct.set("yCoord", sol::property(gameHelpers::getTileY));
	typeAll.tileStruct.set("factionID", sol::property(gameHelpers::getTileFactionID));
	typeAll.tileStruct.set("hasRiver", sol::property(gameHelpers::tileHasRiver));
	typeAll.tileStruct.set("hasRiverSource", sol::property(gameHelpers::tileHasRiverSource));
	typeAll.tileStruct.set("hasCliff", sol::property(gameHelpers::tileHasCliff));
	typeAll.tileStruct.set("isLandConnection", sol::property(gameHelpers::isLandConnection));
	typeAll.tileStruct.set("isCoastalWater", sol::property(gameHelpers::isCoastalWater));
	typeAll.tileStruct.set("isSeaCrossing", sol::property(gameHelpers::isSeaCrossing));
	typeAll.tileStruct.set("hasCrossing", sol::property(gameHelpers::tileHasCrossing));
	typeAll.tileStruct.set("roadLevel", sol::property(gameHelpers::tileRoadLevel));
	typeAll.tileStruct.set("border", sol::property(gameHelpers::tileBorderType));
	typeAll.tileStruct.set("hasCharacter", sol::property(gameHelpers::tileHasCharacter));
	typeAll.tileStruct.set("hasShip", sol::property(gameHelpers::tileHasShip));
	typeAll.tileStruct.set("hasWatchtower", sol::property(gameHelpers::tileHasWatchtower));
	typeAll.tileStruct.set("hasPort", sol::property(gameHelpers::tileHasPort));
	typeAll.tileStruct.set("hasFort", sol::property(gameHelpers::tileHasFort));
	typeAll.tileStruct.set("hasSettlement", sol::property(gameHelpers::tileHasSettlement));
	typeAll.tileStruct.set("isDevastated", sol::property(gameHelpers::isDevastated));
	typeAll.tileStruct.set("height", sol::property(&gameHelpers::getTileHeight, &gameHelpers::setTileHeight));
	typeAll.tileStruct.set("climate", sol::property(&gameHelpers::getTileClimate, &gameHelpers::setTileClimate));
	typeAll.tileStruct.set("heatValue", sol::property(gameHelpers::getTileHeatValue));
	typeAll.tileStruct.set("resource", sol::property(gameHelpers::getTileResource));
	typeAll.tileStruct.set("character", sol::property(gameHelpers::getTileCharacter));
	typeAll.tileStruct.set("settlement", sol::property(gameHelpers::getTileSettlement));
	typeAll.tileStruct.set("fort", sol::property(gameHelpers::getTileFort));
	typeAll.tileStruct.set("port", sol::property(gameHelpers::getTilePort));
	typeAll.tileStruct.set("watchtower", sol::property(gameHelpers::getTileWatchtower));
	typeAll.tileStruct.set("armiesNearTile", &oneTile::armiesNearTile);
	typeAll.tileStruct.set("charactersOnTile", &oneTile::charactersOnTile);
	typeAll.tileStruct.set("mpModifier", &oneTile::mpModifier);

	/***
	Check if a faction has an army near a tile.
	@function tileStruct:factionHasArmyNearTile
	@tparam int factionID
	@treturn bool hasArmyNearTile
	@usage
	local hasArmyNearTile = tile:factionHasArmyNearTile(3);
	*/
	typeAll.tileStruct.set_function("factionHasArmyNearTile", &gameHelpers::factionHasArmyNearTile);

	/***
	Check if a faction has a character on a tile.
	@function tileStruct:factionHasCharacterOnTile
	@tparam int factionID
	@treturn bool hasCharacterOnTile
	@usage
	local hasCharacterOnTile = tile:factionHasCharacterOnTile(3);
	*/
	typeAll.tileStruct.set_function("factionHasCharacterOnTile", &gameHelpers::factionHasCharacterOnTile);

	/***
	Get amount of characters on a tile.
	@function tileStruct:getTileCharacterCount
	@treturn int characterCount
	@usage
	local tileCharacterCount = tile:getTileCharacterCount();
	*/
	typeAll.tileStruct.set_function("getTileCharacterCount", &gameHelpers::getTileCharacterCount);

	/***
	Get a character on a tile.
	@function tileStruct:getTileCharacterAtIndex
	@tparam int index
	@treturn character char
	@usage
	local tileCharacterCount = tile:getTileCharacterCount();
	for i=0,tileCharacterCount -1 do
		local character = tile:getTileCharacterAtIndex(i);
	end
	*/
	typeAll.tileStruct.set_function("getTileCharacterAtIndex", &gameHelpers::getTileCharacterAtIndex);

	///RegionStruct
	//@section RegionStruct

	/***
	Basic regionStruct table.

	@tfield string regionName
	@tfield string localizedName
	@tfield string settlementName
	@tfield string legioName
	@tfield int regionID
	@tfield int roadLevel as set in descr_strat
	@tfield int farmingLevel as set in descr_strat
	@tfield int famineThreat
	@tfield int harvestSuccess
	@tfield int totalSeaTradeValue
	@tfield int stacksNum
	@tfield int fortsNum
	@tfield int colorRed
	@tfield int colorGreen
	@tfield int colorBlue
	@tfield int watchtowersNum
	@tfield int isSea
	@tfield int hasLake (fully enclosed by region)
	@tfield landMass landMass
	@tfield roadStruct roadToPort
	@tfield int seaConnectedRegionsCount
	@tfield int loyaltyFactionID
	@tfield seaConnectedRegion seaExportRegion
	@tfield int seaImportRegionsCount
	@tfield int regionSeaEdgesCount (point where the region border both another land region and sea).
	@tfield int tilesBorderingEdgeOfMapCount
	@tfield int devastatedTilesCount
	@tfield settlementStruct settlement
	@tfield int tileCount
	@tfield int fertileTilesCount
	@tfield int neighbourRegionsNum
	@tfield int resourcesNum
	@tfield int resourceTypesBitMap
	@tfield int hiddenResources1 (bitmap with 32 first hidden resources), needs to be converted to binary and then use bitwise operators from lua.
	@tfield int hiddenResources2 (bitmap last 32 first hidden resources), needs to be converted to binary and then use bitwise operators from lua.
	@tfield int settlementXCoord
	@tfield int settlementYCoord
	@tfield int portEntranceXCoord
	@tfield int portEntranceYCoord
	@tfield factionStruct faction
	@tfield mercPool mercPool
	@tfield string rebelType
	@tfield string localizedRebelsName
	@tfield int triumphValue
	@tfield getStack getStack
	@tfield getFort getFort
	@tfield getWatchtower getWatchtower
	@tfield getResource getResource
	@tfield getNeighbour getNeighbour
	@tfield getHiddenResource getHiddenResource
	@tfield setHiddenResource setHiddenResource
	@tfield getSeaConnectedRegion getSeaConnectedRegion
	@tfield getSeaImportRegion getSeaImportRegion
	@tfield getRegionSeaEdge getRegionSeaEdge
	@tfield getReligionHistory getReligionHistory
	@tfield getTileBorderingEdgeOfMap getTileBorderingEdgeOfMap
	@tfield getTile getTile
	@tfield getFertileTile getFertileTile
	@tfield getDevastatedTile getDevastatedTile
	@tfield getHostileArmiesStrength getHostileArmiesStrength
	@tfield hasResourceType hasResourceType

	@table regionStruct
	*/
	typeAll.regionStruct = luaState.new_usertype<regionStruct>("regionStruct");
	typeAll.regionStruct.set("regionName", &regionStruct::regionName);
	typeAll.regionStruct.set("settlementName", &regionStruct::settlementName);
	typeAll.regionStruct.set("localizedName", sol::property(
		&gameHelpers::getRegionName, &smallFuncs::changeRegionName
		));
	typeAll.regionStruct.set("legioName", &regionStruct::legioName);
	typeAll.regionStruct.set("colorRed", &regionStruct::colorRed);
	typeAll.regionStruct.set("colorGreen", &regionStruct::colorGreen);
	typeAll.regionStruct.set("colorBlue", &regionStruct::colorBlue);
	typeAll.regionStruct.set("regionID", &regionStruct::regionID);
	typeAll.regionStruct.set("loyaltyFactionID", &regionStruct::loyaltyFactionID);
	typeAll.regionStruct.set("roadLevel", &regionStruct::roadLevel);
	typeAll.regionStruct.set("farmingLevel", &regionStruct::farmingLevel);
	typeAll.regionStruct.set("famineThreat", &regionStruct::famineThreat);
	typeAll.regionStruct.set("harvestSuccess", &regionStruct::harvestSuccess);
	typeAll.regionStruct.set("totalSeaTradeValue", &regionStruct::totalSeaTradeValue);
	typeAll.regionStruct.set("seaConnectedRegionsCount", &regionStruct::seaConnectedRegionsCount);
	typeAll.regionStruct.set("seaImportRegionsCount", &regionStruct::seaImportRegionsCount);
	typeAll.regionStruct.set("landMass", &regionStruct::landMass);
	typeAll.regionStruct.set("roadToPort", &regionStruct::roadToPort);
	typeAll.regionStruct.set("seaExportRegion", &regionStruct::seaExportRegion);
	typeAll.regionStruct.set("regionSeaEdgesCount", &regionStruct::regionSeaEdgesCount);
	typeAll.regionStruct.set("devastatedTilesCount", &regionStruct::devastatedTilesCount);
	typeAll.regionStruct.set("tilesBorderingEdgeOfMapCount", &regionStruct::tilesBorderingEdgeOfMapCount);
	typeAll.regionStruct.set("fertileTilesCount", &regionStruct::fertileTilesCount);
	typeAll.regionStruct.set("resourceTypesBitMap", &regionStruct::resourceTypesBitMap);
	typeAll.regionStruct.set("stacksNum", &regionStruct::stacksNum);
	typeAll.regionStruct.set("fortsNum", &regionStruct::fortsNum);
	typeAll.regionStruct.set("watchtowersNum", &regionStruct::watchtowersNum);
	typeAll.regionStruct.set("isSea", &regionStruct::isSea);
	typeAll.regionStruct.set("hasLake", &regionStruct::hasLake);
	typeAll.regionStruct.set("mercPool", &regionStruct::mercPool);
	typeAll.regionStruct.set("settlement", &regionStruct::settlement);
	typeAll.regionStruct.set("tileCount", &regionStruct::tileCount);
	typeAll.regionStruct.set("neighbourRegionsNum", &regionStruct::neighbourRegionsNum);
	typeAll.regionStruct.set("resourcesNum", &regionStruct::resourcesNum);
	typeAll.regionStruct.set("hiddenResources1", &regionStruct::hiddenResources1);
	typeAll.regionStruct.set("hiddenResources2", &regionStruct::hiddenResources2);
	typeAll.regionStruct.set("settlementXCoord", &regionStruct::settlementXCoord);
	typeAll.regionStruct.set("settlementYCoord", &regionStruct::settlementYCoord);
	typeAll.regionStruct.set("portEntranceXCoord", &regionStruct::portEntranceXCoord);
	typeAll.regionStruct.set("portEntranceYCoord", &regionStruct::portEntranceYCoord);
	typeAll.regionStruct.set("faction", &regionStruct::factionOwner);
	typeAll.regionStruct.set("rebelType", &regionStruct::rebelType);
	typeAll.regionStruct.set("localizedRebelsName", sol::property(
		&gameHelpers::getRebelsName, &smallFuncs::changeRebelsName
		));
	typeAll.regionStruct.set("triumphValue", &regionStruct::triumphValue);

	/***
	Get an army by it's index.
	@function regionStruct:getStack
	@tparam int index
	@treturn stackStruct army
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	local army = region:getStack(0)
	*/
	typeAll.regionStruct.set_function("getStack", &gameHelpers::getStack);

	/***
	Get a fort by it's index.
	@function regionStruct:getFort
	@tparam int index
	@treturn fortStruct fort
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	local fort = region:getFort(0)
	*/
	typeAll.regionStruct.set_function("getFort", &gameHelpers::getFort);

	/***
	Get a watchtower by it's index.
	@function regionStruct:getWatchtower
	@tparam int index
	@treturn watchtowerStruct watchtower
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	local watch = region:getWatchtower(0)
	*/
	typeAll.regionStruct.set_function("getWatchtower", &gameHelpers::getWatchtower);

	/***
	Get a resource by it's index.
	@function regionStruct:getResource
	@tparam int index
	@treturn tradeResource resource
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	local res = region:getResource(0)
	*/
	typeAll.regionStruct.set_function("getResource", &gameHelpers::getResource);

	/***
	Get a neighbour region by it's index.
	@function regionStruct:getNeighbour
	@tparam int index
	@treturn neighbourRegion nRegion
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	local nRegion = region:getNeighbour(0)
	*/
	typeAll.regionStruct.set_function("getNeighbour", &gameHelpers::getNeighbour);

	/***
	Check if a region has a hidden resource.
	@function regionStruct:getHiddenResource
	@tparam int index
	@treturn bool hr
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	localhr = region:getHiddenResource(0)
	*/
	typeAll.regionStruct.set_function("getHiddenResource", &gameHelpers::getHiddenResource);

	/***
	Set a region's hidden resource (reset on game restart).
	@function regionStruct:setHiddenResource
	@tparam int index
	@tparam bool enable
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	region:setHiddenResource(0, false)
	*/
	typeAll.regionStruct.set_function("setHiddenResource", &gameHelpers::setHiddenResource);

	/***
	Get a region that is reachable from this region.
	@function regionStruct:getSeaConnectedRegion
	@tparam int index
	@treturn seaConnectedRegion connectedRegion
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	local connectedRegion = region:getSeaConnectedRegion(0)
	*/
	typeAll.regionStruct.set_function("getSeaConnectedRegion", &gameHelpers::getSeaConnectedRegion);

	/***
	Get a region this region is importing trade goods from.
	@function regionStruct:getSeaImportRegion
	@tparam int index
	@treturn seaConnectedRegion seaImportRegion
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	local seaImportRegion = region:getSeaImportRegion(0)
	*/
	typeAll.regionStruct.set_function("getSeaImportRegion", &gameHelpers::getSeaImportRegion);

	/***
	Get a region sea edge (point where it borders both sea and another land region).
	@function regionStruct:getRegionSeaEdge
	@tparam int index
	@treturn tileStruct edge
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	local edge = region:getRegionSeaEdge(0)
	*/
	typeAll.regionStruct.set_function("getRegionSeaEdge", &gameHelpers::getRegionSeaEdge);

	/***
	Get a devastated tile.
	@function regionStruct:getDevastatedTile
	@tparam int index
	@treturn tileStruct tile
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	local tile = region:getDevastatedTile(0)
	*/
	typeAll.regionStruct.set_function("getDevastatedTile", &gameHelpers::getDevastatedTile);

	/***
	Get a tile that borders the edge of the map.
	@function regionStruct:getTileBorderingEdgeOfMap
	@tparam int index
	@treturn tileStruct edge
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	local edge = region:getTileBorderingEdgeOfMap(0)
	*/
	typeAll.regionStruct.set_function("getTileBorderingEdgeOfMap", &gameHelpers::getTileBorderingEdgeOfMap);

	/***
	Get a tile by index.
	@function regionStruct:getTile
	@tparam int index
	@treturn tileStruct tile
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	local tile = region:getTile(0)
	*/
	typeAll.regionStruct.set_function("getTile", &gameHelpers::getTileRegion);

	/***
	Get a fertile tile by index.
	@function regionStruct:getFertileTile
	@tparam int index
	@treturn tileStruct tile
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	local tile = region:getFertileTile(0)
	*/
	typeAll.regionStruct.set_function("getFertileTile", &gameHelpers::getFertileTile);

	/***
	Get religion amount from a set number of turns ago.
	@function regionStruct:getReligionHistory
	@tparam int religionID
	@tparam int turnsAgo (max 19)
	@treturn float religionAmount
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	local catholicThreeTurnsAgo = region:getReligionHistory(0, 3)
	*/
	typeAll.regionStruct.set_function("getReligionHistory", &gameHelpers::getReligionHistory);

	/***
	Check if region has a resource type.
	@function regionStruct:hasResourceType
	@tparam int resourceID
	@treturn bool hasResource
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	local hasResource = region:hasResourceType(16)
	*/
	typeAll.regionStruct.set_function("hasResourceType", &gameHelpers::hasResourceType);

	/***
	Get the strength total of all armies in this region that are hostile to a specific faction.
	@function regionStruct:getHostileArmiesStrength
	@tparam int factionID
	@treturn int totalStrength
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	local totalStrength = region:getHostileArmiesStrength(myFac.factionID)
	*/
	typeAll.regionStruct.set_function("getHostileArmiesStrength", &gameHelpers::getHostileArmiesStrength);

	///neighbourRegion
	//@section neighbourRegion

	/***
	Basic neighbourRegion table.

	@tfield int regionID
	@tfield regionStruct region
	@tfield int tradeValue
	@tfield int notReachable
	@tfield float moveCost
	@tfield int borderTilesCount
	@tfield roadStruct connectingRoad
	@tfield getBorderTile getBorderTile


	@table neighbourRegion
	*/
	typeAll.neighbourRegion = luaState.new_usertype<neighbourRegion>("neighbourRegion");
	typeAll.neighbourRegion.set("regionID", &neighbourRegion::regionID);
	typeAll.neighbourRegion.set("tradeValue", &neighbourRegion::tradeValue);
	typeAll.neighbourRegion.set("region", &neighbourRegion::region);
	typeAll.neighbourRegion.set("borderTilesCount", &neighbourRegion::borderTilesCount);
	typeAll.neighbourRegion.set("connectingRoad", &neighbourRegion::connectingRoad);
	typeAll.neighbourRegion.set("notReachable", &neighbourRegion::alliedRegion);
	typeAll.neighbourRegion.set("moveCost", &neighbourRegion::value);

	/***
	Get a border tile by index.
	@function neighbourRegion:getBorderTile
	@tparam int index
	@treturn tileStruct tile
	@usage
	local tile = nRegion:getBorderTile(0)
	*/
	typeAll.neighbourRegion.set_function("getBorderTile", &gameHelpers::getBorderTile);



	///seaConnectedRegion
	//@section seaConnectedRegion

	/***
	Basic seaConnectedRegion table.

	@tfield int regionID
	@tfield int seaExportValue
	@tfield int seaImportValue
	@tfield int tilesReachableCount
	@tfield int seaTradeLanePathCount
	@tfield getReachableTile getReachableTile
	@tfield getTradeLaneCoord getTradeLaneCoord
	

	@table seaConnectedRegion
	*/
	typeAll.seaConnectedRegion = luaState.new_usertype<seaConnectedRegion>("seaConnectedRegion");
	typeAll.seaConnectedRegion.set("regionID", &seaConnectedRegion::regionID);
	typeAll.seaConnectedRegion.set("seaExportValue", &seaConnectedRegion::seaExportValue);
	typeAll.seaConnectedRegion.set("seaImportValue", &seaConnectedRegion::seaImportValue);
	typeAll.seaConnectedRegion.set("tilesReachableCount", &seaConnectedRegion::tilesReachableCount);
	typeAll.seaConnectedRegion.set("seaTradeLanePathCount", &seaConnectedRegion::seaTradeLanePathCount);

	/***
	Get a reachable tile by index.
	@function seaConnectedRegion:getReachableTile
	@tparam int index
	@treturn tileStruct tile
	@usage
	local tile = seaRegion:getReachableTile(0)
	*/
	typeAll.seaConnectedRegion.set_function("getReachableTile", &gameHelpers::getReachableTile);

	/***
	Get trade lane coords by index.
	@function seaConnectedRegion:getTradeLaneCoord
	@tparam int index
	@treturn coordPair coords
	@usage
	local coords = seaRegion:getTradeLaneCoord(0)
	*/
	typeAll.seaConnectedRegion.set_function("getTradeLaneCoord", &gameHelpers::getTradeLaneCoord);

	///MercPools
	//@section mercPool

	/***
	Basic mercenary pool table.

	@tfield string name
	@tfield getMercUnitNum getMercUnitNum
	@tfield addMercUnit addMercUnit
	@tfield getMercUnit getMercUnit

	@table mercPool
	*/
	typeAll.mercPool = luaState.new_usertype<mercPool>("mercPool");
	typeAll.mercPool.set("name", &mercPool::name);

	/***
	Get amount of mercenary units a region has.
	@function mercPool:getMercUnitNum
	@treturn int mercUnitNum
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	local mercpool = region.mercPool;
	local mercNum = mercpool:getMercUnitNum();
	*/
	typeAll.mercPool.set_function("getMercUnitNum", &gameHelpers::getMercUnitNum);

	/***
	Add a new mercenary unit to a pool.
	@function mercPool:addMercUnit
	@tparam int idx EDU index, supports EOP units.
	@tparam int exp Starting experience.
	@tparam int cost
	@tparam float repmin Minimum replenishment rate.
	@tparam float repmax Maximum replenishment rate.
	@tparam int maxunits Maximum Pool.
	@tparam float startpool Starting pool.
	@tparam int startyear (0 to disable) Use 0 if the startyear is before the year you introduce the merc, not an earlier startyear!
	@tparam int endyear (0 to disable)
	@tparam int crusading
	@treturn mercPoolUnit mercunit
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	local mercpool = region.mercPool;
	local mercUnit = mercpool:addMercUnit(1907, 0, 570, 0.15, 0.35, 3, 3, 0, 0, 0);
	*/
	typeAll.mercPool.set_function("addMercUnit", &gameHelpers::addMercUnit);

	/***
	Get a mercenary unit from a pool by index.
	@function mercPool:getMercUnit
	@tparam int idx
	@treturn mercPoolUnit mercUnit
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	local mercpool = region.mercPool;
	local mercNum = mercPool:getMercUnitNum();
	for i = 0, mercNum-1 do
		local mercUnit = mercpool:getMercUnit(i);
	end
	*/
	typeAll.mercPool.set_function("getMercUnit", &gameHelpers::getMercUnit);


	///MercenaryUnits
	//@section mercPoolUnit

	/***
	Basic mercenary unit table.

	@tfield eduEntry eduEntry
	@tfield int experience
	@tfield int cost
	@tfield float replenishMin
	@tfield float replenishMax
	@tfield int maxUnits
	@tfield float currentPool
	@tfield int startYear
	@tfield int endYear
	@tfield int crusading
	@tfield int poolIndex
	@tfield int mercPoolUnitIndex
	@tfield mercPool mercPool
	@tfield setMercReligion setMercReligion

	@table mercPoolUnit
	*/
	typeAll.mercPoolUnit = luaState.new_usertype<mercPoolUnit>("mercPoolUnit");
	typeAll.mercPoolUnit.set("eduEntry", &mercPoolUnit::eduEntry);
	typeAll.mercPoolUnit.set("experience", &mercPoolUnit::experience);
	typeAll.mercPoolUnit.set("cost", &mercPoolUnit::cost);
	typeAll.mercPoolUnit.set("replenishMin", &mercPoolUnit::replenishMin);
	typeAll.mercPoolUnit.set("replenishMax", &mercPoolUnit::replenishMax);
	typeAll.mercPoolUnit.set("maxUnits", &mercPoolUnit::maxUnits);
	typeAll.mercPoolUnit.set("currentPool", &mercPoolUnit::currentPool);
	typeAll.mercPoolUnit.set("startYear", &mercPoolUnit::startYear);
	typeAll.mercPoolUnit.set("endYear", &mercPoolUnit::endYear);
	typeAll.mercPoolUnit.set("crusading", &mercPoolUnit::crusading);
	typeAll.mercPoolUnit.set("poolIndex", &mercPoolUnit::poolIndex);
	typeAll.mercPoolUnit.set("mercPoolUnitIndex", &mercPoolUnit::mercPoolUnitIndex);
	typeAll.mercPoolUnit.set("mercPool", &mercPoolUnit::mercPool);

	/***
	Set or remove a religion requirement for a mercenary unit.
	@function mercPoolUnit:setMercReligion
	@tparam int religion
	@tparam bool set True means enable this religion requirement, False means disable.
	@usage
	local stratmap = gameDataAll.get().stratMap;
	local region = stratMap.getRegion(2);
	local mercpool = region.mercPool;
	local mercNum = mercPool:getMercUnitNum();
	for i = 0, mercNum-1 do
		local mercUnit = mercPool:getMercUnit(i);
		mercUnit:setMercReligion(3, true)
	end
	*/
	typeAll.mercPoolUnit.set_function("setMercReligion", &gameHelpers::setMercReligion);


}
void luaP::initP2()
{
	struct
	{
		//global game table
		sol::usertype<gameDataAllStruct> gameDataAllTable;
		//this inside gameDataAll table
		sol::usertype<battleDataS> battleTable;
		sol::usertype<battleSide> battleSideTable;
		sol::usertype<battleSideArmy> battleSideArmyTable;
		sol::usertype<deploymentAreaS> deploymentAreaTable;
		sol::usertype<battlePos> battlePos;
		sol::usertype<battleAI> battleAI;
		sol::usertype<armyAndCharacter> battleArmy;
		sol::usertype<battleUnit> battleUnit;
		sol::usertype<battleResidence> battleResidence;
		sol::usertype<AIBattleObjectiveBase> battleObjective;
		sol::usertype<buildingBattle> buildingBattle;
		sol::usertype<battleBuildings> battleBuildings;
		sol::usertype<siegeEngine> siegeEngineStruct;
		sol::usertype<battleTile> battleTile;
		sol::usertype<battlefieldEngines> battlefieldEngines;
		sol::usertype<terrainLineSegment> terrainLine;
		sol::usertype<terrainSegmentVector> terrainLines;
		sol::usertype<terrainFeatures> terrainFeatures;
		sol::usertype<terrainFeatureHill> terrainFeatureHill;
		sol::usertype<hillVector> terrainHills;
		sol::usertype<plazaStuff> plazaData;
		sol::usertype<fortBattleInfo> fortBattleInfo;
		sol::usertype<battleStreets> battleStreets;
		sol::usertype<roadNode> roadNode;
		sol::usertype<perimeterBuildings> perimeterBuildings;
		sol::usertype<towerEntry> battleBuildingStats;
		sol::usertype<towerStats> towerStats;
		sol::usertype<fireRate> fireRate;

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
	typeAll.gameDataAllTable.set_function("get", &gameDataAllHelper::get);
	typeAll.gameDataAllTable.set("battleStruct", &gameDataAllStruct::battleHandler);
	typeAll.gameDataAllTable.set("campaignStruct", &gameDataAllStruct::campaignData);
	typeAll.gameDataAllTable.set("uiCardManager", &gameDataAllStruct::uiCardManager);
	typeAll.gameDataAllTable.set("stratMap", &gameDataAllStruct::stratMap);
	typeAll.gameDataAllTable.set("selectionInfo", &gameDataAllStruct::selectInfo);



	///BattleStruct
	//@section battleStruct

	/***
	basic battleStruct table

	@tfield int battleState
		0 not in battle
		1 prebattle scroll
		2 delay (also for preconflict phase of successful ambushes)
		3 deployment
		4
		5 conflict (also for pause)
		6 victory scroll
		7 pursuit
		8
		9 postbattle scroll (not for autoresolved battles)
	@tfield int battleType
		0 succesful ambush
		1 failed ambush
		2 normal
		3 siege
		4 sally besieger
		5 naval
		6 withdrawal?
	@tfield int isNightBattle
	@tfield int xCoord
	@tfield int yCoord
	@tfield int attackerXCoord
	@tfield int attackerYCoord
	@tfield int defenderXCoord
	@tfield int defenderYCoord
	@tfield int paused
	@tfield int inBattle
	@tfield float battleSpeed
	@tfield float secondsPassed
	@tfield int secondsSinceBattleLoaded
	@tfield int hidingEnabledSet
	@tfield fortBattleInfo fortInfo
	@tfield float mapWidth
	@tfield float mapHeight
	@tfield terrainFeatures terrainFeatures
	@tfield int sidesNum
	@tfield int playerArmyNum
	@tfield battleSide[8] sides Returns a battleSide[8]. Maximum: 8.
	@tfield int[31] factionSide faction alliance array, -1 if not in battle, start at 1 so faction ID + 1 Maximum 31.
	@tfield getPlayerArmy getPlayerArmy
	@tfield getBattleResidence getBattleResidence
	@tfield getUnitByLabel getUnitByLabel
	@tfield getGroupByLabel getGroupByLabel
	@tfield getBattleMapHeight getBattleMapHeight
	@tfield getBattlefieldEngines getBattlefieldEngines
	@tfield getBattleTile getBattleTile
	@tfield getZoneID getZoneID
	@tfield getPosPerimeter getPosPerimeter
	@tfield isZoneValid isZoneValid

	@table battleStruct
	*/
	typeAll.battleTable = luaState.new_usertype<battleDataS>("battleStruct");
	typeAll.battleTable.set("battleState", &battleDataS::battleState);
	typeAll.battleTable.set("battleType", &battleDataS::battleType);
	typeAll.battleTable.set("isNightBattle", &battleDataS::isNightBattle);
	typeAll.battleTable.set("xCoord", &battleDataS::xCoord);
	typeAll.battleTable.set("fortInfo", &battleDataS::fortInfo);
	typeAll.battleTable.set("yCoord", &battleDataS::yCoord);
	typeAll.battleTable.set("playerArmyNum", &battleDataS::playerArmyNum);
	typeAll.battleTable.set("attackerXCoord", &battleDataS::attackerXCoord);
	typeAll.battleTable.set("attackerYCoord", &battleDataS::attackerYCoord);
	typeAll.battleTable.set("defenderXCoord", &battleDataS::defenderXCoord);
	typeAll.battleTable.set("defenderYCoord", &battleDataS::defenderYCoord);
	typeAll.battleTable.set("paused", &battleDataS::paused);
	typeAll.battleTable.set("inBattle", &battleDataS::inBattle);
	typeAll.battleTable.set("battleSpeed", &battleDataS::speed);
	typeAll.battleTable.set("secondsPassed", &battleDataS::secondsPassed);
	typeAll.battleTable.set("secondsSinceBattleLoaded", &battleDataS::secondsSinceBattleLoaded);
	typeAll.battleTable.set("hidingEnabledSet", &battleDataS::hidingEnabledSet);
	typeAll.battleTable.set("mapWidthDoubled", &battleDataS::mapWidthDoubled);
	typeAll.battleTable.set("mapHeightDoubled", &battleDataS::mapHeightDoubled);
	typeAll.battleTable.set("mapHeight", &battleDataS::mapHeightDoubled);
	typeAll.battleTable.set("mapWidth", &battleDataS::mapHeightDoubled);
	typeAll.battleTable.set("sidesNum", &battleDataS::sidesNum);
	typeAll.battleTable.set("terrainFeatures", &battleDataS::terrainFeatures);
	typeAll.battleTable.set("sides", sol::property([](battleDataS& self) { return std::ref(self.sides); }));
	typeAll.battleTable.set("factionSide", sol::property([](battleDataS& self) { return std::ref(self.factionSide); }));
	/***
	Get a players army.
	@function battleStruct:getPlayerArmy
	@tparam int index
	@treturn stackStruct army
	@usage

		local playerArmy = battle:getPlayerArmy(0)

	*/
	typeAll.battleTable.set_function("getPlayerArmy", &battleHandlerHelpers::getPlayerArmy);
	/***
	Get table with certain info about the battle residence.
	@function battleStruct.getBattleResidence
	@treturn battleResidence battleResidence
	@usage

		local battleResidence = battle.getBattleResidence()

	*/
	typeAll.battleTable.set_function("getBattleResidence", &battleHandlerHelpers::getBattleResidence);
	/***
	Get a unit by it's label.
	@function battleStruct.getUnitByLabel
	@tparam string label
	@treturn unit foundUnit
	@usage
	local unit = BATTLE.getUnitByLabel("a1u1");
	*/
	typeAll.battleTable.set_function("getUnitByLabel", &unitActions::getUnitByLabel);
	/***
	Get a group by it's label.
	@function battleStruct.getGroupByLabel
	@tparam string label
	@treturn unitGroup foundGroup
	@usage
	      local group = BATTLE.getGroupByLabel("group1");
	*/
	typeAll.battleTable.set_function("getGroupByLabel", &unitHelpers::getGroupByLabel);
	
	/***
	Get battlemap height at position.
	@function battleStruct.getBattleMapHeight
	@tparam float xCoord
	@tparam float yCoord
	@treturn float zCoord
	@usage
	local unit = BATTLE.getBattleMapHeight(10,20);
	*/
	typeAll.battleTable.set_function("getBattleMapHeight", &unitActions::getBattleMapHeight);
	
	/***
	Get battlefield engines.
	@function battleStruct.getBattlefieldEngines
	@treturn battlefieldEngines engines
	@usage
	     local engines = BATTLE.getBattlefieldEngines();
	*/
	typeAll.battleTable.set_function("getBattlefieldEngines", &battleHandlerHelpers::getBattlefieldEngines);
	
	/***
	Get battlefield tile.
	@function battleStruct.getBattleTile
	@tparam float xCoord
	@tparam float yCoord
	@treturn battleTile tile
	@usage
	     local tile = BATTLE.getBattleTile(103.65, 385.54);
	*/
	typeAll.battleTable.set_function("getBattleTile", &battleHandlerHelpers::getBattleTile);
	
	/***
	Get zone ID.
	@function battleStruct.getZoneID
	@tparam float xCoord
	@tparam float yCoord
	@treturn int zoneID
	@usage
	     local id = BATTLE.getZoneID(103.65, 385.54);
	*/
	typeAll.battleTable.set_function("getZoneID", &battleHandlerHelpers::getZoneID);
	
	/***
	Get position perimeter.
	@function battleStruct.getPosPerimeter
	@tparam float xCoord
	@tparam float yCoord
	@treturn int perimeter
	@usage
	     local perimeter = BATTLE.getPosPerimeter(103.65, 385.54);
	*/
	typeAll.battleTable.set_function("getPosPerimeter", &battleHandlerHelpers::getZonePerimeter);
	
	/***
	Is zone valid.
	@function battleStruct.isZoneValid
	@tparam int zoneID
	@treturn bool valid
	@usage
	     local valid = BATTLE.isZoneValid(3);
	*/
	typeAll.battleTable.set_function("isZoneValid", &battleHandlerHelpers::isZoneValid);

	///BattleSide
	//@section battleSide

	/***
	Basic battleSide table

	@tfield bool isDefender
	@tfield bool isCanDeploy
	@tfield int wonBattle 0 = lose, 1 = draw, 2 = win
	@tfield int battleSuccess 0 = close, 1 = average, 2 = clear, 3 = crushing
	@tfield int[4] winConditions Returns an int index of a wincondition. Maximum 4.
	@tfield getWinConditionString getWinConditionString
	@tfield int armiesNum
	@tfield int battleArmyNum
	@tfield int alliance
	@tfield int soldierCountStart
	@tfield int factionCount
	@tfield int totalStrength
	@tfield int reinforceArmyCount
	@tfield float battleOdds
	@tfield int activeArmyStrength
	@tfield battleAI battleAIPlan
	@tfield getBattleArmy getBattleArmy
	@tfield getFaction getFaction
	@tfield getReinforcementArmy getReinforcementArmy
	@tfield battleSideArmy[64] armies Returns a table of battleSideArmy. Maximum: 64.
	
	@table battleSide
	*/
	typeAll.battleSideTable = luaState.new_usertype<battleSide>("battleSide");
	typeAll.battleSideTable.set("isDefender", &battleSide::isDefender);
	typeAll.battleSideTable.set("isCanDeploy", &battleSide::isCanDeploy);
	typeAll.battleSideTable.set("wonBattle", &battleSide::wonBattle);
	typeAll.battleSideTable.set("battleSuccess", &battleSide::battleSuccess);
	typeAll.battleSideTable.set("battleArmyNum", &battleSide::armiesNum);
	typeAll.battleSideTable.set("reinforceArmyCount", &battleSide::reinforceArmyCount);
	typeAll.battleSideTable.set("alliance", &battleSide::alliance);
	typeAll.battleSideTable.set("soldierCountStart", &battleSide::soldierCount);
	typeAll.battleSideTable.set("totalStrength", &battleSide::totalStrength);
	typeAll.battleSideTable.set("activeArmyStrength", &battleSide::activeArmyStrength);
	typeAll.battleSideTable.set("battleOdds", &battleSide::battleOdds);
	typeAll.battleSideTable.set("factionCount", &battleSide::factionCount);
	typeAll.battleSideTable.set("battleAIPlan", &battleSide::battleAIPlan);
	typeAll.battleSideTable.set("winConditions", sol::property([](battleSide& self) { return std::ref(self.winConditions); }));
	/***
	Get win condition string, for example: destroy\_or\_rout_enemy
	@function battleSide.getWinConditionString
	@tparam int condition
	@treturn string winCondition destroy\_or\_rout\_enemy, balance\_of\_strength\_percent, destroy\_enemy\_strength\_percent, capture\_location, destroy\_character, capture\_major\_settlement, capture\_army\_settlement, unknown\_condition
	@usage
	function onPostBattle(eventData)
		getBattleData()
	end

	function getBattleData()
		local thisBattle, battleList = gameData.battleStruct, "Function: getBattleData()"
		for i = 1, thisBattle.sidesNum, 1 do
			local thisSide = thisBattle.sides[i]
			battleList = battleList.."\n\tSide "..i.."\n\t\tisDefender: "..tostring(thisSide.isDefender).."\n\t\tisCanDeploy: "..tostring(thisSide.isCanDeploy).."\n\t\tarmiesNum: "..thisSide.armiesNum.."\n\t\twinConditions:"
			for j = 1, 4, 1 do
				local thisWinCond = thisSide.winConditions[j]
				if thisWinCond ~= 0 then
					battleList = battleList.."\n\t\t\t"..thisSide.getWinConditionString(thisWinCond)
				end
			end
			if thisSide.armies[1] ~= nil then
				local k = 1
				repeat
					local thisArmy = thisSide.armies[k].army
					if thisArmy ~= nil then
						battleList = battleList.."\n\t\tArmy "..k.."\n\t\t\tFaction: "..thisArmy.faction.name
						if thisArmy.leader ~= nil then
							battleList = battleList.."\n\t\t\tLeader: "..thisArmy.leader.namedCharacter.fullName
						end
					end
					k = k + 1
				until thisSide.armies[k] == nil
			end
		end
		print(battleList)
	end
	*/
	typeAll.battleSideTable.set_function("getWinConditionString", &battleHandlerHelpers::getWinConditionS);
	typeAll.battleSideTable.set("armiesNum", &battleSide::battleSideArmyNum);
	typeAll.battleSideTable.set("armies", sol::property([](battleSide& self) { return std::ref(self.armies); }));
	/***
	Get a battle army by it's index.
	@function battleSide:getBattleArmy
	@tparam int index
	@treturn battleArmy army
	@usage

		battleArmy = side:getBattleArmy(0)

	*/
	typeAll.battleSideTable.set_function("getBattleArmy", &battleHandlerHelpers::getBattleArmy);
	/***
	Get a faction in this side by it's index.
	@function battleSide:getFaction
	@tparam int index
	@treturn factionStruct faction
	@usage

		faction = side:getFaction(0)

	*/
	typeAll.battleSideTable.set_function("getFaction", &battleHandlerHelpers::getFaction);
	/***
	Get a reinforcement army in this side by it's index.
	@function battleSide:getReinforcementArmy
	@tparam int index
	@treturn stackStruct army
	@usage

		army = side:getReinforcementArmy(0)

	*/
	typeAll.battleSideTable.set_function("getReinforcementArmy", &battleHandlerHelpers::getReinforcementArmy);


	///battleSideArmy
	//@section battleSideArmy

	/***
	Basic battleSideArmy table

	@tfield stackStruct army
	@tfield int isReinforcement
	@tfield deploymentAreaS deploymentArea

	@table battleSideArmy
	*/
	typeAll.battleSideArmyTable = luaState.new_usertype<battleSideArmy>("battleSideArmy");
	typeAll.battleSideArmyTable.set("army", &battleSideArmy::stack);
	typeAll.battleSideArmyTable.set("isReinforcement", &battleSideArmy::isReinforcement);
	typeAll.battleSideArmyTable.set("deploymentArea", &battleSideArmy::deploymentArea);
	
	///battlePos
	//@section battlePos

	/***
	Basic battlePos table

	@tfield float xCoord
	@tfield float yCoord


	@table battlePos
	*/
	typeAll.battlePos = luaState.new_usertype<battlePos>("battlePos");
	typeAll.battlePos.set("xCoord", &battlePos::xCoord);
	typeAll.battlePos.set("yCoord", &battlePos::yCoord);

	///DeploymentAreaS
	//@section deploymentAreaS

	/***
	Basic DeploymentAreaS table

	@tfield int coordsNum
	@tfield getCoordPair getCoordPair


	@table deploymentAreaS
	*/
	typeAll.deploymentAreaTable = luaState.new_usertype<deploymentAreaS>("deploymentAreaS");
	typeAll.deploymentAreaTable.set("coordsNum", &deploymentAreaS::coordsNum);
	/***
	Get pair of coords with index.
	@function deploymentAreaS:getCoordPair
	@tparam int index
	@treturn battlePos position
	@usage
	gameData=gameDataAll.get();
	battleS=gameData.battleStruct;
	side1=gameData.battleStruct.sides[1];
	deployArea1=side1.armies[1].deploymentArea;

	for i = 0, deployArea1.coordsNum-1 do
		xCoord,yCoord=side1.armies[1].deploymentArea:getCoordPair(i);
		print(xCoord,yCoord);
	end
	*/
	typeAll.deploymentAreaTable.set("getCoordPair", &battleHandlerHelpers::getCoordPair);

	///BattleAI
	//@section battleAI

	/***
	Basic Battle AI table

	@tfield int gtaPlan
		0 = "DO_NOTHING"
		1 = "ATTACK_ALL"
		2 = "DEFEND"
		3 = "DEFEND_FEATURE"
		4 = "HIDE"
		5 = "AMBUSH"
		6 = "SCOUT"
		7 = "WITHDRAW"
		8 = "ATTACK_SETTLEMENT"
		9 = "DEFEND_SETTLEMENT"
		10 = "SALLY_OUT"
	@tfield int unitCount
	@tfield int enemyUnitCount
	@tfield int addedObjectivesCount
	@tfield getObjective getObjective
	

	@table battleAI
	*/

	typeAll.battleAI = luaState.new_usertype<battleAI>("battleAI");
	typeAll.battleAI.set("gtaPlan", &battleAI::currentAIPlan);
	typeAll.battleAI.set("unitCount", &battleAI::unitCount);
	typeAll.battleAI.set("enemyUnitCount", &battleAI::enemyUnitCount);
	typeAll.battleAI.set("addedObjectivesCount", &battleAI::addedObjectivesCount);
	/***
	Get a battle objective by it's index.
	@function battleAI:getObjective
	@tparam int index
	@treturn battleObjective objective
	@usage

		local objective = battleAI:getObjective(0)

	*/
	typeAll.battleAI.set_function("getObjective", &battleHandlerHelpers::getObjective);


	///BattleObjective
	//@section battleObjective

	/***
	Basic battleObjective table

	@tfield int priority
	@tfield int unitCount
	@tfield getUnit getUnit
	@tfield getType getType


	@table battleObjective
	*/

	typeAll.battleObjective = luaState.new_usertype<AIBattleObjectiveBase>("battleObjective");
	typeAll.battleObjective.set("priority", &AIBattleObjectiveBase::priority);
	typeAll.battleObjective.set("unitCount", &AIBattleObjectiveBase::aiUnitsCount);
	/***
	Get a unit by it's index.
	@function battleObjective:getUnit
	@tparam int index
	@treturn unit unit
	@usage

		local unit = battleObjective:getUnit(0)

	*/
	typeAll.battleObjective.set_function("getUnit", &battleHandlerHelpers::getUnit);
	/***
	Get the type of objective.
	@function battleObjective:getType
	@treturn int objectiveType
	@usage

		local objective = battleObjective:getType()

	*/
	typeAll.battleObjective.set_function("getType", &battleHandlerHelpers::getObjectiveType);

	///BattleArmy
	//@section battleArmy

	/***
	Basic battleArmy table

	@tfield stackStruct army
	@tfield character character
	@tfield int generalNumKillsBattle
	@tfield float totalValue
	@tfield float generalHPRatioLost
	@tfield int numKilledGenerals
	@tfield int unitCount
	@tfield getBattleUnit getBattleUnit

	@table battleArmy
	*/
	typeAll.battleArmy = luaState.new_usertype<armyAndCharacter>("battleArmy");
	typeAll.battleArmy.set("army", &armyAndCharacter::army);
	typeAll.battleArmy.set("character", &armyAndCharacter::character);
	typeAll.battleArmy.set("generalNumKillsBattle", &armyAndCharacter::generalNumKillsBattle);
	typeAll.battleArmy.set("generalHPRatioLost", &armyAndCharacter::generalHPRatioLost);
	typeAll.battleArmy.set("numKilledGenerals", &armyAndCharacter::numKilledGenerals);
	typeAll.battleArmy.set("totalValue", &armyAndCharacter::totalValue);
	typeAll.battleArmy.set("unitCount", &armyAndCharacter::unitCount);
	/***
	Get a battle unit by it's index.
	@function battleArmy:getBattleUnit
	@tparam int index
	@treturn battleUnit unit
	@usage

		unit = battleArmy:getBattleUnit(0)

	*/
	typeAll.battleArmy.set_function("getBattleUnit", &battleHandlerHelpers::getBattleUnit);
	///BattleUnit
	//@section battleUnit

	/***
	Basic battleUnit table

	@tfield unit unit
	@tfield float valuePerSoldier
	@tfield int soldiersLost
	@tfield int soldiersStart
	@tfield int unitsRouted
	@tfield int soldiersKilled
	@tfield int takenPrisoner
	@tfield int prisonersCaught
	@tfield int soldiersHealed
	@tfield int unitsRoutedEnd
	@tfield int soldiersEnd
	@tfield int friendlyFireCasualties
	@tfield int expStart
	@tfield int expGained
	@tfield int isGeneral
	@tfield int hasWithdrawn

	@table battleUnit
	*/
	typeAll.battleUnit = luaState.new_usertype<battleUnit>("battleUnit");
	typeAll.battleUnit.set("unit", &battleUnit::unit);
	typeAll.battleUnit.set("valuePerSoldier", &battleUnit::valuePerSoldier);
	typeAll.battleUnit.set("soldiersLost", &battleUnit::soldiersLost);
	typeAll.battleUnit.set("soldiersStart", &battleUnit::soldiersStart);
	typeAll.battleUnit.set("unitsRouted", &battleUnit::unitsRouted);
	typeAll.battleUnit.set("soldiersKilled", &battleUnit::soldiersKilled);
	typeAll.battleUnit.set("takenPrisoner", &battleUnit::takenPrisoner);
	typeAll.battleUnit.set("prisonersCaught", &battleUnit::prisonersCaught);
	typeAll.battleUnit.set("soldiersHealed", &battleUnit::soldiersHealed);
	typeAll.battleUnit.set("unitsRoutedEnd", &battleUnit::unitsRoutedEnd);
	typeAll.battleUnit.set("soldiersEnd", &battleUnit::soldiersEnd);
	typeAll.battleUnit.set("friendlyFireCasualties", &battleUnit::friendlyFireCasualties);
	typeAll.battleUnit.set("expStart", &battleUnit::expStart);
	typeAll.battleUnit.set("expGained", &battleUnit::expGained);
	typeAll.battleUnit.set("isGeneral", &battleUnit::isGeneral);
	typeAll.battleUnit.set("hasWithdrawn", &battleUnit::isGeneral);

	/// battleResidence
	//@section battleResidence

	/***
	Basic battleResidence table

	@tfield settlementStruct settlement
	@tfield battleBuildings battleBuildings
	@tfield factionStruct faction
	@tfield plazaData plazaData
	@tfield fortBattleInfo fortInfo
	@tfield int settlementWallsBreached
	@tfield int settlementGateDestroyed
	@tfield getBattleStreets getBattleStreets

	@table battleResidence
	*/
	typeAll.battleResidence = luaState.new_usertype<battleResidence>("battleResidence");
	typeAll.battleResidence.set("settlement", &battleResidence::settlement);
	typeAll.battleResidence.set("faction", &battleResidence::faction);
	typeAll.battleResidence.set("settlementWallsBreached", &battleResidence::settlementWallsBreached);
	typeAll.battleResidence.set("settlementGateDestroyed", &battleResidence::settlementGateDestroyed);
	typeAll.battleResidence.set("battleBuildings", &battleResidence::battleBuildings);
	typeAll.battleResidence.set("fortInfo", &battleResidence::fort);
	typeAll.battleResidence.set("plazaData", &battleResidence::plaza);
	/***
	Get battle streets.
	@function battleResidence.getBattleStreets
	@treturn battleStreets streets
	@usage

		local streets = battleResidence.getBattleStreets()

	*/
	typeAll.battleResidence.set_function("getBattleStreets", &battleHandlerHelpers::getBattleStreets);

	/// plazaData
	//@section Plaza Data

	/***
	Basic plazaData table

	@tfield int soldiersAlliance0
	@tfield int soldiersAlliance1
	@tfield float xCoord
	@tfield float yCoord
	@tfield float sizeX
	@tfield float sizeY
	@tfield int alliance
	@tfield float plazaMaxTime
	@tfield float plazaControlPerSecond
	@tfield float plazaControl

	@table plazaData
	*/
	typeAll.plazaData = luaState.new_usertype<plazaStuff>("plazaData");
	typeAll.plazaData.set("soldiersAlliance0", &plazaStuff::soldiersAlliance0);
	typeAll.plazaData.set("soldiersAlliance1", &plazaStuff::soldiersAlliance1);
	typeAll.plazaData.set("xCoord", &plazaStuff::plazaXcoord);
	typeAll.plazaData.set("yCoord", &plazaStuff::plazaYCoord);
	typeAll.plazaData.set("sizeX", &plazaStuff::sizeX);
	typeAll.plazaData.set("sizeY", &plazaStuff::sizeY);
	typeAll.plazaData.set("alliance", &plazaStuff::alliancePlaza);
	typeAll.plazaData.set("plazaMaxTime", &plazaStuff::plazaMaxTime);
	typeAll.plazaData.set("plazaControlPerSecond", &plazaStuff::plazaControlPerSecond);
	typeAll.plazaData.set("plazaControl", &plazaStuff::plazaTimer);

	/// battleBuildingStats
	//@section Battle Building Stats

	/***
	Basic battleBuildingStats table

	@tfield string name
	@tfield int flammability
	@tfield int impactDamage
	@tfield int health (descr_walls, not currently battle)
	@tfield float controlAreaRadius
	@tfield int manned
	@tfield int isSelectable
	@tfield int healthExcluded
	@tfield getTowerStats getTowerStats
	@tfield int towerStatsCount
	

	@table battleBuildingStats
	*/
	typeAll.battleBuildingStats = luaState.new_usertype<towerEntry>("battleBuildingStats");
	typeAll.battleBuildingStats.set("name", &towerEntry::name);
	typeAll.battleBuildingStats.set("flammability", &towerEntry::flammability);
	typeAll.battleBuildingStats.set("impactDamage", &towerEntry::impactDamage);
	typeAll.battleBuildingStats.set("health", &towerEntry::health);
	typeAll.battleBuildingStats.set("controlAreaRadius", &towerEntry::controlAreaRadius);
	typeAll.battleBuildingStats.set("manned", &towerEntry::manned);
	typeAll.battleBuildingStats.set("healthExcluded", &towerEntry::healthExcluded);
	typeAll.battleBuildingStats.set("isSelectable", &towerEntry::isSelectable);
	typeAll.battleBuildingStats.set("towerStatsCount", sol::property(battleHandlerHelpers::getTowerStatCount));

	/***
	Get tower stats by index.
	@function battleBuildingStats:getTowerStats
	@tparam int index
	@treturn towerStats stats
	@usage
		local stats = battleBuildingStats:getTowerStats(0)
	*/
	typeAll.battleBuildingStats.set_function("getTowerStats", &battleHandlerHelpers::getTowerStats);

	/// towerStats
	//@section Tower Stats

	/***
	Basic towerStats table

	@tfield int range
	@tfield projectileStruct projectile
	@tfield float fireAngle
	@tfield float slotYawX
	@tfield float slotYawY
	@tfield float slotPitchX
	@tfield float slotPitchY
	@tfield fireRate[4] fireRates
	

	@table towerStats
	*/
	typeAll.towerStats = luaState.new_usertype<towerStats>("towerStats");
	typeAll.towerStats.set("range", &towerStats::missileRange);
	typeAll.towerStats.set("projectile", &towerStats::missile);
	typeAll.towerStats.set("fireAngle", &towerStats::fireAngle);
	typeAll.towerStats.set("slotYawX", &towerStats::slotYawX);
	typeAll.towerStats.set("slotYawY", &towerStats::slotYawY);
	typeAll.towerStats.set("slotPitchX", &towerStats::slotPitchX);
	typeAll.towerStats.set("slotPitchY", &towerStats::slotPitchY);
	typeAll.towerStats.set("fireRates", sol::property([](towerStats& self) { return std::ref(self.fireRatesNormal); }));

	/***
	Basic fireRate table

	@tfield int normal
	@tfield int flaming
	

	@table fireRate
	*/
	typeAll.fireRate = luaState.new_usertype<fireRate>("fireRate");
	typeAll.fireRate.set("normal", &fireRate::normal);
	typeAll.fireRate.set("flaming", &fireRate::flaming);


	/// terrainLine
	//@section Terrain Line

	/***
	Basic terrainLine table

	@tfield float startX
	@tfield float startZ
	@tfield float startY
	@tfield float endX
	@tfield float endZ
	@tfield float endY
	@tfield terrainLine previousSegment
	@tfield terrainLine nextSegment
	

	@table terrainLine
	*/
	typeAll.terrainLine = luaState.new_usertype<terrainLineSegment>("terrainLine");
	typeAll.terrainLine.set("startX", &terrainLineSegment::startX);
	typeAll.terrainLine.set("startZ", &terrainLineSegment::startZ);
	typeAll.terrainLine.set("startY", &terrainLineSegment::startY);
	typeAll.terrainLine.set("endX", &terrainLineSegment::endX);
	typeAll.terrainLine.set("endZ", &terrainLineSegment::endZ);
	typeAll.terrainLine.set("endY", &terrainLineSegment::endY);
	typeAll.terrainLine.set("previousSegment", &terrainLineSegment::previousSegment);
	typeAll.terrainLine.set("nextSegment", &terrainLineSegment::nextSegment);

	/// terrainFeatureHill
	//@section Terrain Hill

	/***
	Basic terrainFeatureHill table

	@tfield float xCoord
	@tfield float zCoord
	@tfield float yCoord
	@tfield float radius
	@tfield terrainLine terrainLinesStart
	@tfield float area
	

	@table terrainFeatureHill
	*/
	typeAll.terrainFeatureHill = luaState.new_usertype<terrainFeatureHill>("terrainFeatureHill");
	typeAll.terrainFeatureHill.set("xCoord", &terrainFeatureHill::xCoord);
	typeAll.terrainFeatureHill.set("zCoord", &terrainFeatureHill::zCoord);
	typeAll.terrainFeatureHill.set("yCoord", &terrainFeatureHill::yCoord);
	typeAll.terrainFeatureHill.set("radius", &terrainFeatureHill::radius);
	typeAll.terrainFeatureHill.set("terrainLinesStart", &terrainFeatureHill::terrainLineSegmentStart);
	typeAll.terrainFeatureHill.set("area", &terrainFeatureHill::area);

	/// terrainHills
	//@section Terrain Hills

	/***
	Basic terrainHills table

	@tfield int hillsNum
	@tfield terrainHills nextHills
	@tfield terrainHills previousHills
	@tfield getHill getHill
	

	@table terrainHills
	*/
	typeAll.terrainHills = luaState.new_usertype<hillVector>("terrainHills");
	typeAll.terrainHills.set("hillsNum", &hillVector::hillsNum);
	typeAll.terrainHills.set("nextHills", &hillVector::nextHills);
	typeAll.terrainHills.set("previousHills", &hillVector::previousHills);

	/***
	Get a hill by index.
	@function terrainHills:getHill
	@tparam int index
	@treturn terrainFeatureHill hill
	@usage
		local hill = terrainHills:getHill(0)
	*/
	typeAll.terrainHills.set_function("getHill", &battleHandlerHelpers::getHill);

	/// terrainLines
	//@section Terrain Lines

	/***
	Basic terrainLines table

	@tfield int segmentNum
	@tfield terrainLines nextLines
	@tfield terrainLines previousLines
	@tfield getTerrainLine getTerrainLine
	

	@table terrainLines
	*/
	typeAll.terrainLines = luaState.new_usertype<terrainSegmentVector>("terrainLines");
	typeAll.terrainLines.set("segmentNum", &terrainSegmentVector::lineSegmentsNum);
	typeAll.terrainLines.set("nextLines", &terrainSegmentVector::nextSegments);
	typeAll.terrainLines.set("previousLines", &terrainSegmentVector::previousSegments);

	/***
	Get a terrain line segment by index.
	@function terrainLines:getTerrainLine
	@tparam int index
	@treturn terrainLine line
	@usage
		local lineSegment = terrainLines:getTerrainLine(0)
	*/
	typeAll.terrainLines.set_function("getTerrainLine", &battleHandlerHelpers::getTerrainLine);

	/// terrainFeatures
	//@section Terrain Features

	/***
	Basic terrainFeatures table

	@tfield terrainLines lines
	@tfield terrainHills hills
	@tfield float width
	@tfield float widthHalf
	@tfield float length
	@tfield float lengthHalf
	@tfield float widthOnePercent
	@tfield float lengthOnePercent
	

	@table terrainFeatures
	*/
	typeAll.terrainFeatures = luaState.new_usertype<terrainFeatures>("terrainFeatures");
	typeAll.terrainFeatures.set("lines", &terrainFeatures::terrainLines);
	typeAll.terrainFeatures.set("hills", &terrainFeatures::hills);
	typeAll.terrainFeatures.set("width", &terrainFeatures::width);
	typeAll.terrainFeatures.set("widthHalf", &terrainFeatures::widthHalf);
	typeAll.terrainFeatures.set("length", &terrainFeatures::height);
	typeAll.terrainFeatures.set("lengthHalf", &terrainFeatures::heightHalf);
	typeAll.terrainFeatures.set("widthOnePercent", &terrainFeatures::widthOnePercent);
	typeAll.terrainFeatures.set("lengthOnePercent", &terrainFeatures::heightOnePercent);

	/// roadNode
	//@section roadNode

	/***
	Basic roadNode table

	@tfield float xCoord
	@tfield float yCoord
	

	@table roadNode
	*/
	typeAll.roadNode = luaState.new_usertype<roadNode>("roadNode");
	typeAll.roadNode.set("xCoord", &roadNode::xCoord);
	typeAll.roadNode.set("yCoord", &roadNode::yCoord);

	/// battleStreets
	//@section Battle Streets

	/***
	Basic battleStreets table

	@tfield getStreetNode getStreetNode
	@tfield nodeNum nodeNum
	

	@table battleStreets
	*/
	typeAll.battleStreets = luaState.new_usertype<battleStreets>("battleStreets");
	typeAll.battleStreets.set("nodeNum", sol::property(&battleHandlerHelpers::getStreetNum));

	/***
	Get a street point.
	@function battleStreets:getStreetNode
	@tparam int index
	@treturn roadNode node
	@usage
		local node = battleStreets:getStreetNode(0)
	*/
	typeAll.battleStreets.set_function("getStreetNode", &battleHandlerHelpers::getStreetNode);
	

	/// buildingBattle
	//@section buildingBattle

	/***
	Basic buildingBattle table

	@tfield int type
	@tfield int endHealth
	@tfield int currentHealth
	@tfield int startHealth
	@tfield int alliance
	@tfield int factionId
	@tfield factionStruct faction
	@tfield battleResidence battleResidence
	@tfield float posX
	@tfield float posZ
	@tfield float posY
	@tfield battleBuildingStats battleBuildingStats
	@tfield int index

	@table buildingBattle
	*/
	typeAll.buildingBattle = luaState.new_usertype<buildingBattle>("buildingBattle");
	typeAll.buildingBattle.set("type", &buildingBattle::type);
	typeAll.buildingBattle.set("factionId", &buildingBattle::factionId);
	typeAll.buildingBattle.set("posX", &buildingBattle::xCoord);
	typeAll.buildingBattle.set("posZ", &buildingBattle::zCoord);
	typeAll.buildingBattle.set("posY", &buildingBattle::yCoord);
	typeAll.buildingBattle.set("faction", &buildingBattle::faction);
	typeAll.buildingBattle.set("endHealth", &buildingBattle::endHealth);
	typeAll.buildingBattle.set("currentHealth", &buildingBattle::currentHealth);
	typeAll.buildingBattle.set("startHealth", &buildingBattle::startHealth);
	typeAll.buildingBattle.set("battleResidence", &buildingBattle::battleResidence);
	typeAll.buildingBattle.set("alliance", &buildingBattle::alliance);
	typeAll.buildingBattle.set("index", &buildingBattle::index);
	typeAll.buildingBattle.set("battleBuildingStats", &buildingBattle::descrWallsEntry);

	///Siege Engine
	//@section Siege Engine

	/***
	Basic siegeEngineStruct table

	@tfield float xCoord
	@tfield float zCoord
	@tfield float yCoord
	@tfield float mass
	@tfield int angle
	@tfield int engineID
	@tfield unit currentUnit
	@tfield unit lastUnit
	@tfield getType getType
	@tfield int range
	@tfield projectileStruct projectile

	@table siegeEngineStruct
	*/
	typeAll.siegeEngineStruct = luaState.new_usertype<siegeEngine>("siegeEngineStruct");
	typeAll.siegeEngineStruct.set("xCoord", &siegeEngine::posX);
	typeAll.siegeEngineStruct.set("zCoord", &siegeEngine::posZ);
	typeAll.siegeEngineStruct.set("yCoord", &siegeEngine::posY);
	typeAll.siegeEngineStruct.set("mass", &siegeEngine::mass);
	typeAll.siegeEngineStruct.set("angle", sol::property(&unitHelpers::getEngineAngle, &unitHelpers::setEngineAngle));
	typeAll.siegeEngineStruct.set("currentUnit", &siegeEngine::currentUnit);
	typeAll.siegeEngineStruct.set("lastUnit", &siegeEngine::lastUnit);
	typeAll.siegeEngineStruct.set("engineID", &siegeEngine::engineID);
	typeAll.siegeEngineStruct.set("range", &siegeEngine::range);
	typeAll.siegeEngineStruct.set("projectile", &siegeEngine::statPriMissle);

	/***
	Get the type of the engine (use the enum).
	@function siegeEngineStruct:getType
	@treturn int type
	@usage
		local type = siegeEngineStruct:getType()
	*/
	typeAll.siegeEngineStruct.set_function("getType", &battleHandlerHelpers::getEngineType);

	///Battle Tile
	//@section Battle Tile

	/***
	Basic battleTile table

	@tfield int physicalGroundType
	@tfield float height
	@tfield float waterHeight

	@table battleTile
	*/
	typeAll.battleTile = luaState.new_usertype<battleTile>("battleTile");
	typeAll.battleTile.set("physicalGroundType", sol::property(&battleHandlerHelpers::getGroundType));
	typeAll.battleTile.set("height", sol::property(&battleHandlerHelpers::getGroundHeight));
	typeAll.battleTile.set("waterHeight", sol::property(&battleHandlerHelpers::getWaterHeight));

	///Battlefield Engines
	//@section Battlefield Engines

	/***
	Basic battlefieldEngines table

	@tfield int engineNum
	@tfield getEngine getEngine

	@table battlefieldEngines
	*/
	typeAll.battlefieldEngines = luaState.new_usertype<battlefieldEngines>("battlefieldEngines");
	typeAll.battlefieldEngines.set("engineNum", &battlefieldEngines::enginesNum);

	/***
	Get an engine from the battlefield.
	@function battlefieldEngines:getEngine
	@tparam int index
	@treturn siegeEngineStruct engine
	@usage
		local engine = battlefieldEngines:getEngine(0)
	*/
	typeAll.battlefieldEngines.set_function("getEngine", &battleHandlerHelpers::getSiegeEngine);


	/// battleBuildings
	//@section battleBuildings

	/***
	Basic battleBuildings table

	@tfield int buildingCount
	@tfield int perimeterCount
	@tfield getBuilding getBuilding
	@tfield getPerimeter getPerimeter

	@table battleBuildings
	*/
	typeAll.battleBuildings = luaState.new_usertype<battleBuildings>("battleBuildings");
	typeAll.battleBuildings.set("buildingCount", &battleBuildings::allBuildingsNum);
	typeAll.battleBuildings.set("perimeterCount", &battleBuildings::perimitersNum);

	/***
	Get a battle building by it's index.
	@function battleBuildings:getBuilding
	@tparam int index
	@treturn buildingBattle building
	@usage

		local building = battleBuildings:getBuilding(0)

	*/
	typeAll.battleBuildings.set_function("getBuilding", &battleHandlerHelpers::getBattleBuilding);

	/***
	Get a perimeter by it's index.
	@function battleBuildings:getPerimeter
	@tparam int index
	@treturn perimeterBuildings perimeter
	@usage

		local perimeter = battleBuildings:getPerimeter(0)

	*/
	typeAll.battleBuildings.set_function("getPerimeter", &battleHandlerHelpers::getPerimeter);
	
	/// perimeterBuildings
	//@section Perimeter Buildings

	/***
	Basic perimeterBuildings table

	@tfield int buildingCount
	@tfield getBuilding getBuilding

	@table perimeterBuildings
	*/
	typeAll.perimeterBuildings = luaState.new_usertype<perimeterBuildings>("perimeterBuildings");
	typeAll.perimeterBuildings.set("buildingCount", &perimeterBuildings::buildingNum);

	/***
	Get a battle building in a perimiter by it's index.
	@function perimeterBuildings:getBuilding
	@tparam int index
	@treturn buildingBattle building
	@usage

		local building = perimeterBuildings:getBuilding(0)

	*/
	typeAll.perimeterBuildings.set_function("getBuilding", &battleHandlerHelpers::getPerimeterBuilding);


	/// fortBattleInfo
	//@section Fort battle info

	/***
	Basic fortBattleInfo table

	@tfield fortStruct fort
	@tfield stackStruct garrison
	@tfield factionStruct faction
	@tfield int ownerFactionID
	@tfield int creatorFactionID
	@tfield int fortFortificationLevel

	@table fortBattleInfo
	*/
	typeAll.fortBattleInfo = luaState.new_usertype<fortBattleInfo>("fortBattleInfo");
	typeAll.fortBattleInfo.set("fort", &fortBattleInfo::fort);
	typeAll.fortBattleInfo.set("garrison", &fortBattleInfo::garrison);
	typeAll.fortBattleInfo.set("faction", &fortBattleInfo::faction);
	typeAll.fortBattleInfo.set("ownerFactionID", &fortBattleInfo::ownerFactionID);
	typeAll.fortBattleInfo.set("creatorFactionID", &fortBattleInfo::creatorFactionID);
	typeAll.fortBattleInfo.set("fortFortificationLevel", &fortBattleInfo::fortFortificationLevel);

	
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
		"guardMode", unitHelpers::guardMode,
		"fireAtWill", unitHelpers::fireAtWill,
		"skirmish", unitHelpers::skirmish
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


void luaP::initP3()
{
	struct
	{
		sol::usertype<campaignDbRecruitment> campaignDbRecruitment;
		sol::usertype<campaignDbReligion> campaignDbReligion;
		sol::usertype<campaignDbBribery> campaignDbBribery;
		sol::usertype<campaignDbFamilyTree> campaignDbFamilyTree;
		sol::usertype<campaignDbDiplomacy> campaignDbDiplomacy;
		sol::usertype<campaignDbDisplay> campaignDbDisplay;
		sol::usertype<campaignDbRansom> campaignDbRansom;
		sol::usertype<campaignDbAutoresolve> campaignDbAutoresolve;
		sol::usertype<campaignDbSettlement> campaignDbSettlement;
		sol::usertype<campaignDbRevolt> campaignDbRevolt;
		sol::usertype<campaignDbHorde> campaignDbHorde;
		sol::usertype<campaignDbMerchants> campaignDbMerchants;
		sol::usertype<campaignDbAgents> campaignDbAgents;
		sol::usertype<campaignDbCrusades> campaignDbCrusades;
		sol::usertype<campaignDbAi> campaignDbAi;
		sol::usertype<campaignDbMisc> campaignDbMisc;
		sol::usertype<campaignDb> campaignDb;
		sol::usertype<campaignDbExtra> campaignDbExtra;
	}typeAll;



		///Campaign DB
		//@section Campaign DB

		/***
		Basic campaignDb Recruitment table

		@tfield int recruitmentSlots
		@tfield int retrainingSlots
		@tfield bool deplenishPoolsWithCaps
		@tfield float deplenishMultiplier
		@tfield float deplenishOffset
		@tfield bool addDisbandNoCaps
		@tfield int percentagePoolReductionLost
		@tfield int percentagePoolReductionOccupy
		@tfield int percentagePoolReductionSack
		@tfield int percentagePoolReductionExterminate
		@tfield int maxAgentsPerTurn

		@table campaignDbRecruitment
		*/
	typeAll.campaignDbRecruitment = luaState.new_usertype<campaignDbRecruitment>("campaignDbRecruitment");
	typeAll.campaignDbRecruitment.set("recruitmentSlots", &campaignDbRecruitment::recruitmentSlots);
	typeAll.campaignDbRecruitment.set("retrainingSlots", &campaignDbRecruitment::retrainingSlots);
	typeAll.campaignDbRecruitment.set("deplenishPoolsWithCaps", &campaignDbRecruitment::deplenishPoolsWithCaps);
	typeAll.campaignDbRecruitment.set("deplenishMultiplier", &campaignDbRecruitment::deplenishMultiplier);
	typeAll.campaignDbRecruitment.set("deplenishOffset", &campaignDbRecruitment::deplenishOffset);
	typeAll.campaignDbRecruitment.set("addDisbandNoCaps", &campaignDbRecruitment::addDisbandNoCaps);
	typeAll.campaignDbRecruitment.set("percentagePoolReductionLost", &campaignDbRecruitment::percentagePoolReductionLost);
	typeAll.campaignDbRecruitment.set("percentagePoolReductionOccupy", &campaignDbRecruitment::percentagePoolReductionOccupy);
	typeAll.campaignDbRecruitment.set("percentagePoolReductionSack", &campaignDbRecruitment::percentagePoolReductionSack);
	typeAll.campaignDbRecruitment.set("percentagePoolReductionExterminate", &campaignDbRecruitment::percentagePoolReductionExterminate);
	typeAll.campaignDbRecruitment.set("maxAgentsPerTurn", &campaignDbRecruitment::maxAgentsPerTurn);

	/***
	Basic campaignDb Religion table

	@tfield int maxWitchesPerRegion
	@tfield int maxWitches
	@tfield int maxHereticsPerRegion
	@tfield int maxHeretics
	@tfield int maxInquisitorsPerRegion
	@tfield int maxInquisitors
	@tfield float maxHereticsConversionModifier
	@tfield float hereticConversionRateModifier
	@tfield float hereticConversionRateOffset
	@tfield float witchConversionRateOffset
	@tfield float inquisitorConversionRateModifier
	@tfield float inquisitorConversionRateOffset
	@tfield float priestConversionRateModifier
	@tfield float priestConversionRateOffset
	@tfield float witchChanceModifier
	@tfield float hereticChanceModifier
	@tfield float inquisitorChanceModifier
	@tfield int minCardinalPiety
	@tfield float convertToHereticBaseModifier
	@tfield float convertToHereticUnorthodoxModifier
	@tfield int inquisitorTurnStart

	@table campaignDbReligion
	*/
	typeAll.campaignDbReligion = luaState.new_usertype<campaignDbReligion>("campaignDbReligion");
	typeAll.campaignDbReligion.set("maxWitchesPerRegion", &campaignDbReligion::maxWitchesPerRegion);
	typeAll.campaignDbReligion.set("maxWitches", &campaignDbReligion::maxWitches);
	typeAll.campaignDbReligion.set("maxHereticsPerRegion", &campaignDbReligion::maxHereticsPerRegion);
	typeAll.campaignDbReligion.set("maxHeretics", &campaignDbReligion::maxHeretics);
	typeAll.campaignDbReligion.set("maxInquisitorsPerRegion", &campaignDbReligion::maxInquisitorsPerRegion);
	typeAll.campaignDbReligion.set("maxInquisitors", &campaignDbReligion::maxInquisitors);
	typeAll.campaignDbReligion.set("maxHereticsConversionModifier", &campaignDbReligion::maxHereticsConversionModifier);
	typeAll.campaignDbReligion.set("hereticConversionRateModifier", &campaignDbReligion::hereticConversionRateModifier);
	typeAll.campaignDbReligion.set("hereticConversionRateOffset", &campaignDbReligion::hereticConversionRateOffset);
	typeAll.campaignDbReligion.set("witchConversionRateOffset", &campaignDbReligion::witchConversionRateOffset);
	typeAll.campaignDbReligion.set("inquisitorConversionRateModifier", &campaignDbReligion::inquisitorConversionRateModifier);
	typeAll.campaignDbReligion.set("inquisitorConversionRateOffset", &campaignDbReligion::inquisitorConversionRateOffset);
	typeAll.campaignDbReligion.set("priestConversionRateModifier", &campaignDbReligion::priestConversionRateModifier);
	typeAll.campaignDbReligion.set("priestConversionRateOffset", &campaignDbReligion::priestConversionRateOffset);
	typeAll.campaignDbReligion.set("witchChanceModifier", &campaignDbReligion::witchChanceModifier);
	typeAll.campaignDbReligion.set("hereticChanceModifier", &campaignDbReligion::hereticChanceModifier);
	typeAll.campaignDbReligion.set("inquisitorChanceModifier", &campaignDbReligion::inquisitorChanceModifier);
	typeAll.campaignDbReligion.set("minCardinalPiety", &campaignDbReligion::minCardinalPiety);
	typeAll.campaignDbReligion.set("convertToHereticBaseModifier", &campaignDbReligion::convertToHereticBaseModifier);
	typeAll.campaignDbReligion.set("convertToHereticUnorthodoxModifier", &campaignDbReligion::convertToHereticUnorthodoxModifier);
	typeAll.campaignDbReligion.set("inquisitorTurnStart", &campaignDbReligion::inquisitorTurnStart);
	typeAll.campaignDbReligion.set("inquisitorTurnStart", &campaignDbReligion::inquisitorTurnStart);

	/***
	Basic campaignDb Bribery table

	@tfield float baseCharacterChance
	@tfield float religionModifier
	@tfield float combinedAttributeModifier
	@tfield float briberAttributeDivisor
	@tfield float bribeeAttributeDivisor
	@tfield float armySizeModifier
	@tfield float baseSettlementChance
	@tfield float settlementLoyaltyModifier
	@tfield float settlementPopulationModifier
	@tfield float factionStandingDivisor
	@tfield float maxBribeChance
	@tfield float minBribeChance
	@tfield float bribeChanceModifier

	@table campaignDbBribery
	*/
	typeAll.campaignDbBribery = luaState.new_usertype<campaignDbBribery>("campaignDbBribery");
	typeAll.campaignDbBribery.set("baseCharacterChance", &campaignDbBribery::baseCharacterChance);
	typeAll.campaignDbBribery.set("religionModifier", &campaignDbBribery::religionModifier);
	typeAll.campaignDbBribery.set("combinedAttributeModifier", &campaignDbBribery::combinedAttributeModifier);
	typeAll.campaignDbBribery.set("briberAttributeDivisor", &campaignDbBribery::briberAttributeDivisor);
	typeAll.campaignDbBribery.set("bribeeAttributeDivisor", &campaignDbBribery::bribeeAttributeDivisor);
	typeAll.campaignDbBribery.set("armySizeModifier", &campaignDbBribery::armySizeModifier);
	typeAll.campaignDbBribery.set("baseSettlementChance", &campaignDbBribery::baseSettlementChance);
	typeAll.campaignDbBribery.set("settlementLoyaltyModifier", &campaignDbBribery::settlementLoyaltyModifier);
	typeAll.campaignDbBribery.set("settlementPopulationModifier", &campaignDbBribery::settlementPopulationModifier);
	typeAll.campaignDbBribery.set("factionStandingDivisor", &campaignDbBribery::factionStandingDivisor);
	typeAll.campaignDbBribery.set("maxBribeChance", &campaignDbBribery::maxBribeChance);
	typeAll.campaignDbBribery.set("minBribeChance", &campaignDbBribery::minBribeChance);
	typeAll.campaignDbBribery.set("bribeChanceModifier", &campaignDbBribery::bribeChanceModifier);

	/***
	Basic campaignDb Family Tree table

	@tfield int maxAge
	@tfield int maxAgeForMarriageMale
	@tfield int maxAgeForMarriageForFemale
	@tfield int maxAgeBeforeDeath
	@tfield int maxAgeOfChild
	@tfield int oldAge
	@tfield int ageOfManhood
	@tfield int daughtersAgeOfConsent
	@tfield int daughtersRetirementAge
	@tfield int ageDifferenceMin
	@tfield int ageDifferenceMax
	@tfield int parentToChildMinAgeDiff
	@tfield int minAdoptionAge
	@tfield int maxAdoptionAge
	@tfield int maxAgeForConception
	@tfield int ageOfManhoodClose
	@tfield int maxNumberOfChildren

	@table campaignDbFamilyTree
	*/
	typeAll.campaignDbFamilyTree = luaState.new_usertype<campaignDbFamilyTree>("campaignDbFamilyTree");
	typeAll.campaignDbFamilyTree.set("maxAge", &campaignDbFamilyTree::maxAge);
	typeAll.campaignDbFamilyTree.set("maxAgeForMarriageMale", &campaignDbFamilyTree::maxAgeForMarriageMale);
	typeAll.campaignDbFamilyTree.set("maxAgeForMarriageForFemale", &campaignDbFamilyTree::maxAgeForMarriageForFemale);
	typeAll.campaignDbFamilyTree.set("maxAgeBeforeDeath", &campaignDbFamilyTree::maxAgeBeforeDeath);
	typeAll.campaignDbFamilyTree.set("maxAgeOfChild", &campaignDbFamilyTree::maxAgeOfChild);
	typeAll.campaignDbFamilyTree.set("oldAge", &campaignDbFamilyTree::oldAge);
	typeAll.campaignDbFamilyTree.set("ageOfManhood", &campaignDbFamilyTree::ageOfManhood);
	typeAll.campaignDbFamilyTree.set("daughtersAgeOfConsent", &campaignDbFamilyTree::daughtersAgeOfConsent);
	typeAll.campaignDbFamilyTree.set("daughtersRetirementAge", &campaignDbFamilyTree::daughtersRetirementAge);
	typeAll.campaignDbFamilyTree.set("ageDifferenceMin", &campaignDbFamilyTree::ageDifferenceMin);
	typeAll.campaignDbFamilyTree.set("ageDifferenceMax", &campaignDbFamilyTree::ageDifferenceMax);
	typeAll.campaignDbFamilyTree.set("parentToChildMinAgeDiff", &campaignDbFamilyTree::parentToChildMinAgeDiff);
	typeAll.campaignDbFamilyTree.set("minAdoptionAge", &campaignDbFamilyTree::minAdoptionAge);
	typeAll.campaignDbFamilyTree.set("maxAdoptionAge", &campaignDbFamilyTree::maxAdoptionAge);
	typeAll.campaignDbFamilyTree.set("maxAgeForConception", &campaignDbFamilyTree::maxAgeForConception);
	typeAll.campaignDbFamilyTree.set("ageOfManhoodClose", &campaignDbFamilyTree::ageOfManhoodClose);
	typeAll.campaignDbFamilyTree.set("maxNumberOfChildren", &campaignDbFamilyTree::maxNumberOfChildren);

	/***
	Basic campaignDb Diplomacy table

	@tfield int maxDiplomacyItems
	@tfield int nullMissionScore

	@table campaignDbDiplomacy
	*/
	typeAll.campaignDbDiplomacy = luaState.new_usertype<campaignDbDiplomacy>("campaignDbDiplomacy");
	typeAll.campaignDbDiplomacy.set("maxDiplomacyItems", &campaignDbDiplomacy::maxDiplomacyItems);
	typeAll.campaignDbDiplomacy.set("nullMissionScore", &campaignDbDiplomacy::nullMissionScore);

	/***
	Basic campaignDb Display table

	@tfield float characterSelectionRadius
	@tfield float characterSelectionHeight
	@tfield float characterSelectionHeightCrouching
	@tfield float diplomacyScrollHeight
	@tfield float factionStandingMin
	@tfield float factionStandingMax
	@tfield bool UseOrigRebelFactionModels
	@tfield int hudTabTextOffset
	@tfield bool useFactionCreatorSettModels
	@tfield int standardSoldierLimit
	@tfield int standardSoldierLevelScale
	@tfield bool clearBattleModelsOnNewFaction

	@table campaignDbDisplay
	*/
	typeAll.campaignDbDisplay = luaState.new_usertype<campaignDbDisplay>("campaignDbDisplay");
	typeAll.campaignDbDisplay.set("characterSelectionRadius", &campaignDbDisplay::characterSelectionRadius);
	typeAll.campaignDbDisplay.set("characterSelectionHeight", &campaignDbDisplay::characterSelectionHeight);
	typeAll.campaignDbDisplay.set("characterSelectionHeightCrouching", &campaignDbDisplay::characterSelectionHeightCrouching);
	typeAll.campaignDbDisplay.set("diplomacyScrollHeight", &campaignDbDisplay::diplomacyScrollHeight);
	typeAll.campaignDbDisplay.set("factionStandingMin", &campaignDbDisplay::factionStandingMin);
	typeAll.campaignDbDisplay.set("factionStandingMax", &campaignDbDisplay::factionStandingMax);
	typeAll.campaignDbDisplay.set("UseOrigRebelFactionModels", &campaignDbDisplay::UseOrigRebelFactionModels);
	typeAll.campaignDbDisplay.set("hudTabTextOffset", &campaignDbDisplay::hudTabTextOffset);
	typeAll.campaignDbDisplay.set("useFactionCreatorSettModels", &campaignDbDisplay::useFactionCreatorSettModels);
	typeAll.campaignDbDisplay.set("standardSoldierLimit", &campaignDbDisplay::standardSoldierLimit);
	typeAll.campaignDbDisplay.set("standardSoldierLevelScale", &campaignDbDisplay::standardSoldierLevelScale);
	typeAll.campaignDbDisplay.set("clearBattleModelsOnNewFaction", &campaignDbDisplay::clearBattleModelsOnNewFaction);

	/***
	Basic campaignDb Ransom table

	@tfield float captorReleaseChanceBase
	@tfield float captorReleaseChanceChivMod
	@tfield float captorRansomChanceBase
	@tfield float captorRansomChanceChivMod
	@tfield float captorRansomChanceTmMod
	@tfield float captiveRansomChanceBase
	@tfield float captiveRansomChanceChivMod
	@tfield float captiveRansomChanceTmMod
	@tfield float captiveRansomChanceMsmMod

	@table campaignDbRansom
	*/
	typeAll.campaignDbRansom = luaState.new_usertype<campaignDbRansom>("campaignDbRansom");
	typeAll.campaignDbRansom.set("captorReleaseChanceBase", &campaignDbRansom::captorReleaseChanceBase);
	typeAll.campaignDbRansom.set("captorReleaseChanceChivMod", &campaignDbRansom::captorReleaseChanceChivMod);
	typeAll.campaignDbRansom.set("captorRansomChanceBase", &campaignDbRansom::captorRansomChanceBase);
	typeAll.campaignDbRansom.set("captorRansomChanceChivMod", &campaignDbRansom::captorRansomChanceChivMod);
	typeAll.campaignDbRansom.set("captorRansomChanceTmMod", &campaignDbRansom::captorRansomChanceTmMod);
	typeAll.campaignDbRansom.set("captiveRansomChanceBase", &campaignDbRansom::captiveRansomChanceBase);
	typeAll.campaignDbRansom.set("captiveRansomChanceChivMod", &campaignDbRansom::captiveRansomChanceChivMod);
	typeAll.campaignDbRansom.set("captiveRansomChanceTmMod", &campaignDbRansom::captiveRansomChanceTmMod);
	typeAll.campaignDbRansom.set("captiveRansomChanceMsmMod", &campaignDbRansom::captiveRansomChanceMsmMod);

	/***
	Basic campaignDb Autoresolve table

	@tfield float minCapturePercent
	@tfield float maxCapturePercent
	@tfield float lopsidedThresh
	@tfield float lopsidedHnMod
	@tfield int separationMissileAdd
	@tfield float navalSinkModifier
	@tfield float navalSinkOffset
	@tfield float navalSinkMax
	@tfield float sallyAtDefDrawDivisor
	@tfield bool useNewSettAutoResolve
	@tfield int gateDefenceNumOilAttacks
	@tfield float gateDefenceStrengthOilBase
	@tfield int gateDefenceNumArrowAttacks
	@tfield float gateDefenceStrengthArrowBase
	@tfield float gateDefenceStrengthArrowLevelModifier
	@tfield int gateDefenceNumDefaultAttacks
	@tfield float gateDefenceStrengthDefaultBase
	@tfield float gateDefenceStrengthDefaultLevelModifier
	@tfield int settDefenceNumArrowAttacks
	@tfield float settDefenceStrengthArrowBase
	@tfield float settDefenceStrengthArrowModifier
	@tfield float settDefenceStrengthDefaultBase
	@tfield float settDefenceStrengthDefaultModifier
	@tfield float displayStrengthOil
	@tfield float displayStrengthArrow
	@tfield float displayStrengthDefault

	@table campaignDbAutoresolve
	*/
	typeAll.campaignDbAutoresolve = luaState.new_usertype<campaignDbAutoresolve>("campaignDbAutoresolve");
	typeAll.campaignDbAutoresolve.set("minCapturePercent", &campaignDbAutoresolve::minCapturePercent);
	typeAll.campaignDbAutoresolve.set("maxCapturePercent", &campaignDbAutoresolve::maxCapturePercent);
	typeAll.campaignDbAutoresolve.set("lopsidedThresh", &campaignDbAutoresolve::lopsidedThresh);
	typeAll.campaignDbAutoresolve.set("lopsidedHnMod", &campaignDbAutoresolve::lopsidedHnMod);
	typeAll.campaignDbAutoresolve.set("separationMissileAdd", &campaignDbAutoresolve::separationMissileAdd);
	typeAll.campaignDbAutoresolve.set("navalSinkModifier", &campaignDbAutoresolve::navalSinkModifier);
	typeAll.campaignDbAutoresolve.set("navalSinkOffset", &campaignDbAutoresolve::navalSinkOffset);
	typeAll.campaignDbAutoresolve.set("navalSinkMax", &campaignDbAutoresolve::navalSinkMax);
	typeAll.campaignDbAutoresolve.set("sallyAtDefDrawDivisor", &campaignDbAutoresolve::sallyAtDefDrawDivisor);
	typeAll.campaignDbAutoresolve.set("useNewSettAutoResolve", &campaignDbAutoresolve::useNewSettAutoResolve);
	typeAll.campaignDbAutoresolve.set("gateDefenceNumOilAttacks", &campaignDbAutoresolve::gateDefenceNumOilAttacks);
	typeAll.campaignDbAutoresolve.set("gateDefenceStrengthOilBase", &campaignDbAutoresolve::gateDefenceStrengthOilBase);
	typeAll.campaignDbAutoresolve.set("gateDefenceNumArrowAttacks", &campaignDbAutoresolve::gateDefenceNumArrowAttacks);
	typeAll.campaignDbAutoresolve.set("gateDefenceStrengthArrowBase", &campaignDbAutoresolve::gateDefenceStrengthArrowBase);
	typeAll.campaignDbAutoresolve.set("gateDefenceStrengthArrowLevelModifier", &campaignDbAutoresolve::gateDefenceStrengthArrowLevelModifier);
	typeAll.campaignDbAutoresolve.set("gateDefenceNumDefaultAttacks", &campaignDbAutoresolve::gateDefenceNumDefaultAttacks);
	typeAll.campaignDbAutoresolve.set("gateDefenceStrengthDefaultBase", &campaignDbAutoresolve::gateDefenceStrengthDefaultBase);
	typeAll.campaignDbAutoresolve.set("gateDefenceStrengthDefaultLevelModifier", &campaignDbAutoresolve::gateDefenceStrengthDefaultLevelModifier);
	typeAll.campaignDbAutoresolve.set("settDefenceNumArrowAttacks", &campaignDbAutoresolve::settDefenceNumArrowAttacks);
	typeAll.campaignDbAutoresolve.set("settDefenceStrengthArrowBase", &campaignDbAutoresolve::settDefenceStrengthArrowBase);
	typeAll.campaignDbAutoresolve.set("settDefenceStrengthArrowModifier", &campaignDbAutoresolve::settDefenceStrengthArrowModifier);
	typeAll.campaignDbAutoresolve.set("settDefenceStrengthDefaultBase", &campaignDbAutoresolve::settDefenceStrengthDefaultBase);
	typeAll.campaignDbAutoresolve.set("settDefenceStrengthDefaultModifier", &campaignDbAutoresolve::settDefenceStrengthDefaultModifier);
	typeAll.campaignDbAutoresolve.set("displayStrengthOil", &campaignDbAutoresolve::displayStrengthOil);
	typeAll.campaignDbAutoresolve.set("displayStrengthArrow", &campaignDbAutoresolve::displayStrengthArrow);
	typeAll.campaignDbAutoresolve.set("displayStrengthDefault", &campaignDbAutoresolve::displayStrengthDefault);

	/***
	Basic campaignDb Settlement table

	@tfield float sackMoneyModifier
	@tfield float exterminateMoneyModifier
	@tfield float chivSpfModifier
	@tfield float chivSofModifier
	@tfield float dreadSofModifier
	@tfield float pietyCorruptionModifier
	@tfield float pietyAdminSifModifier
	@tfield float portToPortMpMin
	@tfield float heresyUnrestNodifier
	@tfield float religionUnrestModifier
	@tfield int siegeGearRequiredForCityLevel
	@tfield int noTowersOnlyForCityLevel
	@tfield int minTurnKeepRebelGarrison
	@tfield bool destroyEmptyForts
	@tfield bool canBuildForts
	@tfield float raceGameCostsModifier
	@tfield float altRelAlliedModifier
	@tfield float altRelGovModifierBase
	@tfield float altRelGovCoefficient

	@table campaignDbSettlement
	*/
	typeAll.campaignDbSettlement = luaState.new_usertype<campaignDbSettlement>("campaignDbSettlement");
	typeAll.campaignDbSettlement.set("sackMoneyModifier", &campaignDbSettlement::sackMoneyModifier);
	typeAll.campaignDbSettlement.set("exterminateMoneyModifier", &campaignDbSettlement::exterminateMoneyModifier);
	typeAll.campaignDbSettlement.set("chivSpfModifier", &campaignDbSettlement::chivSpfModifier);
	typeAll.campaignDbSettlement.set("chivSofModifier", &campaignDbSettlement::chivSofModifier);
	typeAll.campaignDbSettlement.set("dreadSofModifier", &campaignDbSettlement::dreadSofModifier);
	typeAll.campaignDbSettlement.set("pietyCorruptionModifier", &campaignDbSettlement::pietyCorruptionModifier);
	typeAll.campaignDbSettlement.set("pietyAdminSifModifier", &campaignDbSettlement::pietyAdminSifModifier);
	typeAll.campaignDbSettlement.set("portToPortMpMin", &campaignDbSettlement::portToPortMpMin);
	typeAll.campaignDbSettlement.set("heresyUnrestNodifier", &campaignDbSettlement::heresyUnrestNodifier);
	typeAll.campaignDbSettlement.set("religionUnrestModifier", &campaignDbSettlement::religionUnrestModifier);
	typeAll.campaignDbSettlement.set("siegeGearRequiredForCityLevel", &campaignDbSettlement::siegeGearRequiredForCityLevel);
	typeAll.campaignDbSettlement.set("noTowersOnlyForCityLevel", &campaignDbSettlement::noTowersOnlyForCityLevel);
	typeAll.campaignDbSettlement.set("minTurnKeepRebelGarrison", &campaignDbSettlement::minTurnKeepRebelGarrison);
	typeAll.campaignDbSettlement.set("destroyEmptyForts", &campaignDbSettlement::destroyEmptyForts);
	typeAll.campaignDbSettlement.set("canBuildForts", &campaignDbSettlement::canBuildForts);
	typeAll.campaignDbSettlement.set("raceGameCostsModifier", &campaignDbSettlement::raceGameCostsModifier);
	typeAll.campaignDbSettlement.set("altRelAlliedModifier", &campaignDbSettlement::altRelAlliedModifier);
	typeAll.campaignDbSettlement.set("altRelGovModifierBase", &campaignDbSettlement::altRelGovModifierBase);
	typeAll.campaignDbSettlement.set("altRelGovCoefficient", &campaignDbSettlement::altRelGovCoefficient);

	/***
	Basic campaignDb Revolt table

	@tfield float endTurnModifier
	@tfield float excommunicatedModifier
	@tfield float newLeaderModifier
	@tfield float maxEffectiveLoyalty
	@tfield float rebelRegionModifier
	@tfield float shadowRegionModifier
	@tfield float rebelBorderModifier
	@tfield float shadowBorderModifier
	@tfield float numUnitsModifier
	@tfield float captainModifier
	@tfield float minRevoltChance
	@tfield float maxRevoltChance
	@tfield float aiRevoltModifier
	@tfield float shadowAuthorityModifier
	@tfield float shadowAuthorityModifierSett

	@table campaignDbRevolt
	*/
	typeAll.campaignDbRevolt = luaState.new_usertype<campaignDbRevolt>("campaignDbRevolt");
	typeAll.campaignDbRevolt.set("endTurnModifier", &campaignDbRevolt::endTurnModifier);
	typeAll.campaignDbRevolt.set("excommunicatedModifier", &campaignDbRevolt::excommunicatedModifier);
	typeAll.campaignDbRevolt.set("newLeaderModifier", &campaignDbRevolt::newLeaderModifier);
	typeAll.campaignDbRevolt.set("maxEffectiveLoyalty", &campaignDbRevolt::maxEffectiveLoyalty);
	typeAll.campaignDbRevolt.set("rebelRegionModifier", &campaignDbRevolt::rebelRegionModifier);
	typeAll.campaignDbRevolt.set("shadowRegionModifier", &campaignDbRevolt::shadowRegionModifier);
	typeAll.campaignDbRevolt.set("rebelBorderModifier", &campaignDbRevolt::rebelBorderModifier);
	typeAll.campaignDbRevolt.set("shadowBorderModifier", &campaignDbRevolt::shadowBorderModifier);
	typeAll.campaignDbRevolt.set("numUnitsModifier", &campaignDbRevolt::numUnitsModifier);
	typeAll.campaignDbRevolt.set("captainModifier", &campaignDbRevolt::captainModifier);
	typeAll.campaignDbRevolt.set("minRevoltChance", &campaignDbRevolt::minRevoltChance);
	typeAll.campaignDbRevolt.set("maxRevoltChance", &campaignDbRevolt::maxRevoltChance);
	typeAll.campaignDbRevolt.set("aiRevoltModifier", &campaignDbRevolt::aiRevoltModifier);
	typeAll.campaignDbRevolt.set("shadowAuthorityModifier", &campaignDbRevolt::shadowAuthorityModifier);
	typeAll.campaignDbRevolt.set("shadowAuthorityModifierSett", &campaignDbRevolt::shadowAuthorityModifierSett);

	/***
	Basic campaignDb Horde table

	@tfield int endTargetFactionBonus
	@tfield int startTargetFactionBonus
	@tfield int farmingLevelBonus
	@tfield int sharedTargetBonus
	@tfield int disbandingHordeBonus
	@tfield int hordeStartingRegionBonus
	@tfield int hordeTargetResourceBonus

	@table campaignDbHorde
	*/
	typeAll.campaignDbHorde = luaState.new_usertype<campaignDbHorde>("campaignDbHorde");
	typeAll.campaignDbHorde.set("endTargetFactionBonus", &campaignDbHorde::endTargetFactionBonus);
	typeAll.campaignDbHorde.set("startTargetFactionBonus", &campaignDbHorde::startTargetFactionBonus);
	typeAll.campaignDbHorde.set("farmingLevelBonus", &campaignDbHorde::farmingLevelBonus);
	typeAll.campaignDbHorde.set("sharedTargetBonus", &campaignDbHorde::sharedTargetBonus);
	typeAll.campaignDbHorde.set("disbandingHordeBonus", &campaignDbHorde::disbandingHordeBonus);
	typeAll.campaignDbHorde.set("hordeStartingRegionBonus", &campaignDbHorde::hordeStartingRegionBonus);
	typeAll.campaignDbHorde.set("hordeTargetResourceBonus", &campaignDbHorde::hordeTargetResourceBonus);

	/***
	Basic campaignDb Merchants table

	@tfield float baseIncomeModifier
	@tfield float tradeBonusOffset

	@table campaignDbMerchants
	*/
	typeAll.campaignDbMerchants = luaState.new_usertype<campaignDbMerchants>("campaignDbMerchants");
	typeAll.campaignDbMerchants.set("baseIncomeModifier", &campaignDbMerchants::baseIncomeModifier);
	typeAll.campaignDbMerchants.set("tradeBonusOffset", &campaignDbMerchants::tradeBonusOffset);

	/***
	Basic campaignDb Agents table

	@tfield float denounceInquisitorBaseChance
	@tfield float DenouncePriestBaseChance
	@tfield float denounceAttackModifier
	@tfield float denounceDefenceModifier
	@tfield int denounceChanceMax
	@tfield float assassinateBaseChance
	@tfield float assassinateAttackModifier
	@tfield float assassinateDefenceModifier
	@tfield float assassinatePublicModifier
	@tfield float assassinatePersonalModifier
	@tfield float assassinateCounterSpyModifier
	@tfield float assassinateAgentModifier
	@tfield float assassinateOwnRegionModifier
	@tfield float assassinateAssassinateAttrModifier
	@tfield int assassinateChanceMin
	@tfield int assassinateChanceMax
	@tfield float denounceHereticAttemptModifier
	@tfield float denounceCharacterAttemptModifier
	@tfield float acquisitionBaseChance
	@tfield float acquisitionLevelModifier
	@tfield float acquisitionAttackTradeRightsModifier
	@tfield float acquisitionDefenceTradeRightsModifier
	@tfield int acquisitionChanceMin
	@tfield int acquisitionChanceMax
	@tfield float inquisitorCrtHeresyDivisor
	@tfield float inquisitorCrtPfpModifier
	@tfield float inquisitorCrtPfpModifierMin
	@tfield float inquisitorCrtPfpModifierMax
	@tfield float inquisitorCrtChanceMax
	@tfield float spyBaseChance
	@tfield float spyLevelModifier
	@tfield float notSpyLevelModifier
	@tfield float spyPublicModifier
	@tfield float spyCounterSpyModifier
	@tfield float spyDistanceModifier
	@tfield float spySecretAgentTargetModifier
	@tfield float spySedentaryTurnsModifier
	@tfield float spyAllianceModifier
	@tfield float spyTargetEngagedModifier
	@tfield float spyInSettlementModifier
	@tfield float spyWatchtowerModifier
	@tfield float spyInOwnRegionModifier
	@tfield int spyChanceMin
	@tfield int spyChanceMax

	@table campaignDbAgents
	*/
	typeAll.campaignDbAgents = luaState.new_usertype<campaignDbAgents>("campaignDbAgents");
	typeAll.campaignDbAgents.set("denounceInquisitorBaseChance", &campaignDbAgents::denounceInquisitorBaseChance);
	typeAll.campaignDbAgents.set("DenouncePriestBaseChance", &campaignDbAgents::DenouncePriestBaseChance);
	typeAll.campaignDbAgents.set("denounceAttackModifier", &campaignDbAgents::denounceAttackModifier);
	typeAll.campaignDbAgents.set("denounceDefenceModifier", &campaignDbAgents::denounceDefenceModifier);
	typeAll.campaignDbAgents.set("denounceChanceMax", &campaignDbAgents::denounceChanceMax);
	typeAll.campaignDbAgents.set("assassinateBaseChance", &campaignDbAgents::assassinateBaseChance);
	typeAll.campaignDbAgents.set("assassinateAttackModifier", &campaignDbAgents::assassinateAttackModifier);
	typeAll.campaignDbAgents.set("assassinateDefenceModifier", &campaignDbAgents::assassinateDefenceModifier);
	typeAll.campaignDbAgents.set("assassinatePublicModifier", &campaignDbAgents::assassinatePublicModifier);
	typeAll.campaignDbAgents.set("assassinatePersonalModifier", &campaignDbAgents::assassinatePersonalModifier);
	typeAll.campaignDbAgents.set("assassinateCounterSpyModifier", &campaignDbAgents::assassinateCounterSpyModifier);
	typeAll.campaignDbAgents.set("assassinateAgentModifier", &campaignDbAgents::assassinateAgentModifier);
	typeAll.campaignDbAgents.set("assassinateOwnRegionModifier", &campaignDbAgents::assassinateOwnRegionModifier);
	typeAll.campaignDbAgents.set("assassinateAssassinateAttrModifier", &campaignDbAgents::assassinateAssassinateAttrModifier);
	typeAll.campaignDbAgents.set("assassinateChanceMin", &campaignDbAgents::assassinateChanceMin);
	typeAll.campaignDbAgents.set("assassinateChanceMax", &campaignDbAgents::assassinateChanceMax);
	typeAll.campaignDbAgents.set("denounceHereticAttemptModifier", &campaignDbAgents::denounceHereticAttemptModifier);
	typeAll.campaignDbAgents.set("denounceCharacterAttemptModifier", &campaignDbAgents::denounceCharacterAttemptModifier);
	typeAll.campaignDbAgents.set("acquisitionBaseChance", &campaignDbAgents::acquisitionBaseChance);
	typeAll.campaignDbAgents.set("acquisitionLevelModifier", &campaignDbAgents::acquisitionLevelModifier);
	typeAll.campaignDbAgents.set("acquisitionAttackTradeRightsModifier", &campaignDbAgents::acquisitionAttackTradeRightsModifier);
	typeAll.campaignDbAgents.set("acquisitionDefenceTradeRightsModifier", &campaignDbAgents::acquisitionDefenceTradeRightsModifier);
	typeAll.campaignDbAgents.set("acquisitionChanceMin", &campaignDbAgents::acquisitionChanceMin);
	typeAll.campaignDbAgents.set("acquisitionChanceMax", &campaignDbAgents::acquisitionChanceMax);
	typeAll.campaignDbAgents.set("inquisitorCrtHeresyDivisor", &campaignDbAgents::inquisitorCrtHeresyDivisor);
	typeAll.campaignDbAgents.set("inquisitorCrtPfpModifier", &campaignDbAgents::inquisitorCrtPfpModifier);
	typeAll.campaignDbAgents.set("inquisitorCrtPfpModifierMin", &campaignDbAgents::inquisitorCrtPfpModifierMin);
	typeAll.campaignDbAgents.set("inquisitorCrtPfpModifierMax", &campaignDbAgents::inquisitorCrtPfpModifierMax);
	typeAll.campaignDbAgents.set("inquisitorCrtChanceMax", &campaignDbAgents::inquisitorCrtChanceMax);
	typeAll.campaignDbAgents.set("spyBaseChance", &campaignDbAgents::spyBaseChance);
	typeAll.campaignDbAgents.set("spyLevelModifier", &campaignDbAgents::spyLevelModifier);
	typeAll.campaignDbAgents.set("notSpyLevelModifier", &campaignDbAgents::notSpyLevelModifier);
	typeAll.campaignDbAgents.set("spyPublicModifier", &campaignDbAgents::spyPublicModifier);
	typeAll.campaignDbAgents.set("spyCounterSpyModifier", &campaignDbAgents::spyCounterSpyModifier);
	typeAll.campaignDbAgents.set("spyDistanceModifier", &campaignDbAgents::spyDistanceModifier);
	typeAll.campaignDbAgents.set("spySecretAgentTargetModifier", &campaignDbAgents::spySecretAgentTargetModifier);
	typeAll.campaignDbAgents.set("spySedentaryTurnsModifier", &campaignDbAgents::spySedentaryTurnsModifier);
	typeAll.campaignDbAgents.set("spyAllianceModifier", &campaignDbAgents::spyAllianceModifier);
	typeAll.campaignDbAgents.set("spyTargetEngagedModifier", &campaignDbAgents::spyTargetEngagedModifier);
	typeAll.campaignDbAgents.set("spyInSettlementModifier", &campaignDbAgents::spyInSettlementModifier);
	typeAll.campaignDbAgents.set("spyWatchtowerModifier", &campaignDbAgents::spyWatchtowerModifier);
	typeAll.campaignDbAgents.set("spyInOwnRegionModifier", &campaignDbAgents::spyInOwnRegionModifier);
	typeAll.campaignDbAgents.set("spyChanceMin", &campaignDbAgents::spyChanceMin);
	typeAll.campaignDbAgents.set("spyChanceMax", &campaignDbAgents::spyChanceMax);

	/***
	Basic campaignDb Crusades table

	@tfield int requiredJihadPiety
	@tfield float maxDisbandProgress
	@tfield float nearTargetNoDisbandDistance
	@tfield int disbandProgressWindow
	@tfield int crusadeCalledStartTurn
	@tfield int jihadCalledStartTurn
	@tfield float movementPointsModifier

	@table campaignDbCrusades
	*/
	typeAll.campaignDbCrusades = luaState.new_usertype<campaignDbCrusades>("campaignDbCrusades");
	typeAll.campaignDbCrusades.set("requiredJihadPiety", &campaignDbCrusades::requiredJihadPiety);
	typeAll.campaignDbCrusades.set("maxDisbandProgress", &campaignDbCrusades::maxDisbandProgress);
	typeAll.campaignDbCrusades.set("nearTargetNoDisbandDistance", &campaignDbCrusades::nearTargetNoDisbandDistance);
	typeAll.campaignDbCrusades.set("disbandProgressWindow", &campaignDbCrusades::disbandProgressWindow);
	typeAll.campaignDbCrusades.set("crusadeCalledStartTurn", &campaignDbCrusades::crusadeCalledStartTurn);
	typeAll.campaignDbCrusades.set("jihadCalledStartTurn", &campaignDbCrusades::jihadCalledStartTurn);
	typeAll.campaignDbCrusades.set("movementPointsModifier", &campaignDbCrusades::movementPointsModifier);

	/***
	Basic campaignDb Ai table

	@tfield float priestReligionMin
	@tfield float priestReligionMax
	@tfield float priestHeresyMin
	@tfield float priestHeresyMax
	@tfield float priestReligionExport
	@tfield float priestMaxProdTurns
	@tfield int merchantMinSurvivalAcquire
	@tfield float attStrModifier
	@tfield float siegeAttStrModifier
	@tfield float crusadeAttStrModifier
	@tfield float sallyAttStrModifier
	@tfield float ambushAttStrModifier
	@tfield float strLimitWeak
	@tfield float strLimitStrong

	@table campaignDbAi
	*/
	typeAll.campaignDbAi = luaState.new_usertype<campaignDbAi>("campaignDbAi");
	typeAll.campaignDbAi.set("priestReligionMin", &campaignDbAi::priestReligionMin);
	typeAll.campaignDbAi.set("priestReligionMax", &campaignDbAi::priestReligionMax);
	typeAll.campaignDbAi.set("priestHeresyMin", &campaignDbAi::priestHeresyMin);
	typeAll.campaignDbAi.set("priestHeresyMax", &campaignDbAi::priestHeresyMax);
	typeAll.campaignDbAi.set("priestReligionExport", &campaignDbAi::priestReligionExport);
	typeAll.campaignDbAi.set("priestMaxProdTurns", &campaignDbAi::priestMaxProdTurns);
	typeAll.campaignDbAi.set("merchantMinSurvivalAcquire", &campaignDbAi::merchantMinSurvivalAcquire);
	typeAll.campaignDbAi.set("attStrModifier", &campaignDbAi::attStrModifier);
	typeAll.campaignDbAi.set("siegeAttStrModifier", &campaignDbAi::siegeAttStrModifier);
	typeAll.campaignDbAi.set("crusadeAttStrModifier", &campaignDbAi::crusadeAttStrModifier);
	typeAll.campaignDbAi.set("sallyAttStrModifier", &campaignDbAi::sallyAttStrModifier);
	typeAll.campaignDbAi.set("ambushAttStrModifier", &campaignDbAi::ambushAttStrModifier);
	typeAll.campaignDbAi.set("strLimitWeak", &campaignDbAi::strLimitWeak);
	typeAll.campaignDbAi.set("strLimitStrong", &campaignDbAi::strLimitStrong);

	/***
	Basic campaignDb Misc table

	@tfield int fortDevastationDistance
	@tfield int armyDevastationDistance
	@tfield int fortDevastationModifier
	@tfield int armyDevastationModifier
	@tfield bool allowEnemyForts
	@tfield int siegeMovementPointsModifier
	@tfield int cavalryMovementPointsModifier

	@table campaignDbMisc
	*/
	typeAll.campaignDbMisc = luaState.new_usertype<campaignDbMisc>("campaignDbMisc");
	typeAll.campaignDbMisc.set("fortDevastationDistance", &campaignDbMisc::fortDevastationDistance);
	typeAll.campaignDbMisc.set("armyDevastationDistance", &campaignDbMisc::armyDevastationDistance);
	typeAll.campaignDbMisc.set("fortDevastationModifier", &campaignDbMisc::fortDevastationModifier);
	typeAll.campaignDbMisc.set("armyDevastationModifier", &campaignDbMisc::armyDevastationModifier);
	typeAll.campaignDbMisc.set("allowEnemyForts", &campaignDbMisc::allowEnemyForts);
	typeAll.campaignDbMisc.set("siegeMovementPointsModifier", &campaignDbMisc::siegeMovementPointsModifier);
	typeAll.campaignDbMisc.set("cavalryMovementPointsModifier", &campaignDbMisc::cavalryMovementPointsModifier);

	/***
	Basic campaignDb table

	@tfield campaignDbRecruitment recruitment
	@tfield campaignDbReligion religion
	@tfield campaignDbBribery bribery
	@tfield campaignDbFamilyTree familyTree
	@tfield campaignDbDiplomacy diplomacy
	@tfield campaignDbDisplay display
	@tfield campaignDbRansom ransom
	@tfield campaignDbAutoresolve autoResolve
	@tfield campaignDbSettlement settlement
	@tfield campaignDbRevolt revolt
	@tfield campaignDbHorde horde
	@tfield campaignDbMerchants merchants
	@tfield campaignDbAgents agents
	@tfield campaignDbCrusades crusades
	@tfield campaignDbAi ai
	@tfield campaignDbMisc misc

	@table campaignDb
	*/
	typeAll.campaignDb = luaState.new_usertype<campaignDb>("campaignDb");
	typeAll.campaignDb.set("recruitment", &campaignDb::campaignDbRecruitment);
	typeAll.campaignDb.set("religion", &campaignDb::campaignDbReligion);
	typeAll.campaignDb.set("bribery", &campaignDb::campaignDbBribery);
	typeAll.campaignDb.set("familyTree", &campaignDb::campaignDbFamilyTree);
	typeAll.campaignDb.set("diplomacy", &campaignDb::campaignDbDiplomacy);
	typeAll.campaignDb.set("display", &campaignDb::campaignDbDisplay);
	typeAll.campaignDb.set("ransom", &campaignDb::campaignDbRansom);
	typeAll.campaignDb.set("autoResolve", &campaignDb::campaignDbAutoresolve);
	typeAll.campaignDb.set("settlement", &campaignDb::campaignDbSettlement);
	typeAll.campaignDb.set("revolt", &campaignDb::campaignDbRevolt);
	typeAll.campaignDb.set("horde", &campaignDb::campaignDbHorde);
	typeAll.campaignDb.set("merchants", &campaignDb::campaignDbMerchants);
	typeAll.campaignDb.set("agents", &campaignDb::campaignDbAgents);
	typeAll.campaignDb.set("crusades", &campaignDb::campaignDbCrusades);
	typeAll.campaignDb.set("ai", &campaignDb::campaignDbAi);
	typeAll.campaignDb.set("misc", &campaignDb::campaignDbMisc);

	/***
	Basic campaignDb Extra table (these options are just in another place in memory)

	@tfield bool clearPoolsWithCaps
	@tfield bool addInitialWithCaps
	@tfield bool forceClampToMax
	@tfield float witchConversionRateModifier
	@tfield bool inquisitorTargetCrusades
	@tfield float foundingConversionDefaultRate
	@tfield float ownerConversionDefaultRate
	@tfield float neighbourNormaliseWeight
	@tfield float governorConversionRateOffset
	@tfield float governorConversionRateModifier
	@tfield float spyConversionRateOffset
	@tfield float spyConversionRateModifier
	@tfield float spyConversionRateForeignModifier
	@tfield bool bribeToFamilyTree
	@tfield bool enemiesRejectGifts
	@tfield bool useBalanceOwed
	@tfield bool recruitmentSortSimple
	@tfield bool keepOriginalHereticPortraits
	@tfield bool altSettOrderColors
	@tfield bool separateGamesRaces
	@tfield int chivalryDisplayThreshold
	@tfield bool captiveRansomForSlave
	@tfield bool switchableDefenceExposed
	@tfield float gateDefenceStrengthOilLevelModifier
	@tfield int settDefenceStrengthNumDefaultAttacks
	@tfield int siegeGearRequiredForCastleLevel
	@tfield int noTowersOnlyForCastleLevel
	@tfield int fortFortificationLevel
	@tfield bool alternativeReligiousUnrest
	@tfield bool revoltAdditionalArmies
	@tfield bool revoltCrusadingArmies
	@tfield bool agentsCanHide
	@tfield int denounceChanceMin
	@tfield float inquisitorCrtChanceMin
	@tfield bool inquisitorTargetLeaders
	@tfield bool inquisitorTargetHeirs
	@tfield bool spyRescaleChance
	@tfield bool allowResourceForts
	@tfield bool enableHotseatMessages
	@tfield bool enableBananaRepublicCheat
	@tfield bool enableUnitAccentOverrides

	@table campaignDbExtra
	*/

	typeAll.campaignDbExtra = luaState.new_usertype<campaignDbExtra>("campaignDbExtra");
	typeAll.campaignDbExtra.set("clearPoolsWithCaps", &campaignDbExtra::clearPoolsWithCaps);
	typeAll.campaignDbExtra.set("addInitialWithCaps", &campaignDbExtra::addInitialWithCaps);
	typeAll.campaignDbExtra.set("forceClampToMax", &campaignDbExtra::forceClampToMax);
	typeAll.campaignDbExtra.set("witchConversionRateModifier", &campaignDbExtra::witchConversionRateModifier);
	typeAll.campaignDbExtra.set("inquisitorTargetCrusades", &campaignDbExtra::inquisitorTargetCrusades);
	typeAll.campaignDbExtra.set("foundingConversionDefaultRate", &campaignDbExtra::foundingConversionDefaultRate);
	typeAll.campaignDbExtra.set("ownerConversionDefaultRate", &campaignDbExtra::ownerConversionDefaultRate);
	typeAll.campaignDbExtra.set("neighbourNormaliseWeight", &campaignDbExtra::neighbourNormaliseWeight);
	typeAll.campaignDbExtra.set("governorConversionRateOffset", &campaignDbExtra::governorConversionRateOffset);
	typeAll.campaignDbExtra.set("governorConversionRateModifier", &campaignDbExtra::governorConversionRateModifier);
	typeAll.campaignDbExtra.set("spyConversionRateOffset", &campaignDbExtra::spyConversionRateOffset);
	typeAll.campaignDbExtra.set("spyConversionRateModifier", &campaignDbExtra::spyConversionRateModifier);
	typeAll.campaignDbExtra.set("spyConversionRateForeignModifier", &campaignDbExtra::spyConversionRateForeignModifier);
	typeAll.campaignDbExtra.set("bribeToFamilyTree", &campaignDbExtra::bribeToFamilyTree);
	typeAll.campaignDbExtra.set("enemiesRejectGifts", &campaignDbExtra::enemiesRejectGifts);
	typeAll.campaignDbExtra.set("useBalanceOwed", &campaignDbExtra::useBalanceOwed);
	typeAll.campaignDbExtra.set("recruitmentSortSimple", &campaignDbExtra::recruitmentSortSimple);
	typeAll.campaignDbExtra.set("keepOriginalHereticPortraits", &campaignDbExtra::keepOriginalHereticPortraits);
	typeAll.campaignDbExtra.set("altSettOrderColors", &campaignDbExtra::altSettOrderColors);
	typeAll.campaignDbExtra.set("separateGamesRaces", &campaignDbExtra::separateGamesRaces);
	typeAll.campaignDbExtra.set("chivalryDisplayThreshold", &campaignDbExtra::chivalryDisplayThreshold);
	typeAll.campaignDbExtra.set("captiveRansomForSlave", &campaignDbExtra::captiveRansomForSlave);
	typeAll.campaignDbExtra.set("switchableDefenceExposed", &campaignDbExtra::switchableDefenceExposed);
	typeAll.campaignDbExtra.set("gateDefenceStrengthOilLevelModifier", &campaignDbExtra::gateDefenceStrengthOilLevelModifier);
	typeAll.campaignDbExtra.set("settDefenceStrengthNumDefaultAttacks", &campaignDbExtra::settDefenceStrengthNumDefaultAttacks);
	typeAll.campaignDbExtra.set("siegeGearRequiredForCastleLevel", &campaignDbExtra::siegeGearRequiredForCastleLevel);
	typeAll.campaignDbExtra.set("noTowersOnlyForCastleLevel", &campaignDbExtra::noTowersOnlyForCastleLevel);
	typeAll.campaignDbExtra.set("fortFortificationLevel", &campaignDbExtra::fortFortificationLevel);
	typeAll.campaignDbExtra.set("alternativeReligiousUnrest", &campaignDbExtra::alternativeReligiousUnrest);
	typeAll.campaignDbExtra.set("revoltAdditionalArmies", &campaignDbExtra::revoltAdditionalArmies);
	typeAll.campaignDbExtra.set("revoltCrusadingArmies", &campaignDbExtra::revoltCrusadingArmies);
	typeAll.campaignDbExtra.set("agentsCanHide", &campaignDbExtra::agentsCanHide);
	typeAll.campaignDbExtra.set("denounceChanceMin", &campaignDbExtra::denounceChanceMin);
	typeAll.campaignDbExtra.set("inquisitorCrtChanceMin", &campaignDbExtra::inquisitorCrtChanceMin);
	typeAll.campaignDbExtra.set("inquisitorTargetLeaders", &campaignDbExtra::inquisitorTargetLeaders);
	typeAll.campaignDbExtra.set("inquisitorTargetHeirs", &campaignDbExtra::inquisitorTargetHeirs);
	typeAll.campaignDbExtra.set("spyRescaleChance", &campaignDbExtra::spyRescaleChance);
	typeAll.campaignDbExtra.set("allowResourceForts", &campaignDbExtra::allowResourceForts);
	typeAll.campaignDbExtra.set("enableHotseatMessages", &campaignDbExtra::enableHotseatMessages);
	typeAll.campaignDbExtra.set("enableBananaRepublicCheat", &campaignDbExtra::enableBananaRepublicCheat);
	typeAll.campaignDbExtra.set("enableUnitAccentOverrides", &campaignDbExtra::enableUnitAccentOverrides);
}