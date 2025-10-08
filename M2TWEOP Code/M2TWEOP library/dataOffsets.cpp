#include "pch.h"
#include "dataOffsets.h"

#include "faction.h"
struct consoleCommands;
dataOffsets::offsetsT dataOffsets::offsets;
void dataOffsets::initDataOffsets(int gameVer)
{
	if (gameVer == 1) //kingdoms
	{
		offsets.factionOffsetsStart = 0x02c00140;
		
		offsets.battleMapData = 0x02c00130;
		
		offsets.statStratModelsListOffset = 0x01b5e708;

		offsets.unitTypesStart = 0x0190bd0c;

		offsets.stratMapAllOffsetStart = 0x02c74ff4;

		offsets.stringTable = 0x02CB97E8;
		
		offsets.globalSett = 0x016F10D0;

		offsets.someStratMapPointer = 0x019b3134;

		offsets.skeletonDatabase = 0x01B59B08;

		offsets.getMovePath = 0x004D8390;

		offsets.tgaDataBase = 0x019875E8;

		offsets.stratModelArray = 0x01B617D8;

		offsets.gameDataAllOffset = reinterpret_cast<gameDataAllStruct*>(0x02c74f90);

		offsets.consoleCommands = reinterpret_cast<consoleCommands*>(0x02cbce50);

		offsets.guildDataStart = 0x018FD958;
		offsets.battleCamera = 0x0198848c;

		offsets.edbDataStart = 0x018AA5A8;
		offsets.selectedEra = 0x2CB9210;
		offsets.campaignDb = 0x16666BC;
		offsets.campaignDbExtra = 0x18AA87C;
		
		offsets.scriptCommandVFT = 0x0134E3A0;
		
		offsets.religionDatabase = 0x016E9DC0;
		offsets.perfectSpy = 0x016F0E5C;
		
		offsets.portraitDatabase = 0x18B0340;

		offsets.cultureDatabase = 0x01666FC8;
		offsets.loadGameHandler = 0x02CB6D78;
		offsets.currentGameHandler = 0x016EC1C4;
		offsets.logFlushRate = 0x16E445C;
		
		offsets.audioEnable = reinterpret_cast<bool*>(0x016818cd);
		offsets.audioMaster_vol = reinterpret_cast<int*>(0x01681910);
		offsets.gameUnit_size = reinterpret_cast<int*>(0x0016818fc);
		offsets.globalCadClass = 0x1674570;
		offsets.constructionItems = 0x016F4CAC;
		offsets.recruitmentItems = 0x16F4E00;
		offsets.ltgdGlobals = 0x016F115C;
		offsets.groupLabels = 0x001BA96A0;
		offsets.stratCursorCoords = 0x02c86c28;
		offsets.battleCursorCoords = 0x02C86C10;
		offsets.modelsDb = 0x016e9dc8;
		offsets.customTiles = 0x02C3BF50;
		offsets.battlePerimeters = 0x16F0600;
		offsets.battlefieldEngines = 0x02C8329C;
		offsets.battleAreas = 0x2C83290;
		offsets.options1 = 0x02CB693C;
		offsets.options2 = 0x016818A0;
		offsets.campaignDiff1 = 0x016F4760;
		offsets.campaignDiff2 = 0x0164BF18;
		offsets.equipmentCosts = 0x01655BB0;
		offsets.saveGameHandler = 0x02C649BC;
		offsets.ancLimit = 0x005A57CD;
		offsets.maxUnitSize = 0x008ef381;
		offsets.maxBgSize1 = 0x008ebd95;
		offsets.maxBgSize2 = 0x008ebd9a;
		offsets.unlockConsoleCommands1 = 0x00d2d763;
		offsets.unlockConsoleCommands2 = 0x00d329aa;
		offsets.highlightUnits = 0x009e0ec4;
		offsets.religionLimit = 0x0411C0E;
		offsets.scriptCounters = 0x016F0BF0;
		offsets.scriptCountersSet = 0x16F0BC8;
		offsets.buildingChainLimit = 0x008AB794;
		offsets.guildCooldown = 0x004F57AB;
		offsets.bugReport = 0x16EBB30;
		offsets.uiNotify = 0x2C75000;
		offsets.mapTilesDb = 0x019B3134;
		offsets.campaignControllerVtbl = 0x13380AC;
		offsets.ltgdConfig = 0x0164D04C;
		offsets.descrRebels = 0x16673F0;
		offsets.eventManager = 0x16F0BC8;
		offsets.uiStratUiV2 = 0x2CB62FC;
		offsets.currentRegion = 0x18FD970;
		offsets.traitDb = 0x1666F90;
		offsets.ancillaryDb = 0x1666F40;
		offsets.modelRigidCounts = 0x1B5EFE4;
		offsets.groundMoveCosts = 0x164A0A0;
		offsets.expandedBinTable = 0x2CB97E8;
		offsets.descrCharacter = 0x18AA8F8;
		offsets.currentCharacterActionType = 0x16F0E58;
		offsets.aStarTilesGlobal = 0x16F0E60;
		offsets.religionTriggerBug = 0x892F7C;
		
		offsets.fortVtbl = 0x13362F4;
		offsets.fortSpyingVtbl = 0x13362D0;
		offsets.settlementVtbl = 0x1341F54;
		offsets.settlementSpyingVtbl = 0x1341F34;
		offsets.gameScriptLogger = 0x2CBD7F8;
		offsets.sineValues = 0x2CE67A0;
		offsets.descr_sm_factionslist = reinterpret_cast<smFactionsDb*>(0x01667288);

		offsets.mapCacheSize1 = 0x00B6CDA9;
		offsets.mapCacheSize2 = 0x00B6C968;
		offsets.selectedFactionSymbol = 0x01687e4c;
		offsets.weaponLimit1 = 0x0073BA72;
		offsets.weaponLimit2 = 0x0073BA76;
		offsets.weaponLimit3 = 0x0073BB05;
		offsets.weaponLimit4 = 0x0073BB0F;
		offsets.weaponLimit5 = 0x005F76DF;
		offsets.weaponLimit6 = 0x00475B60;
		offsets.weaponLimit7 = 0x00475B64;
		offsets.weaponLimit8 = 0x00632B12;
		offsets.weaponLimit9 = 0x00632B05;
	}
	else if (gameVer == 2) //steam
	{
		offsets.factionOffsetsStart = 0x02bb7088;
		
		offsets.battleMapData = 0x02BB7078;
		offsets.customTiles = 0x02BF2E98;
		
		offsets.statStratModelsListOffset = 0x01b155d8;

		offsets.unitTypesStart = 0x18C2BAC;

		offsets.stratMapAllOffsetStart = 0x02c2bf3c;

		offsets.stringTable = 0x02C70760;

		offsets.someStratMapPointer = 0x0196a004;

		offsets.skeletonDatabase = 0x01B109D8;

		offsets.tgaDataBase = 0x0193E498;

		offsets.stratModelArray = 0x01B186A8;
		offsets.battleCamera = 0x0193f34c;
		offsets.battlePerimeters = 0x016A7428;

		offsets.getMovePath = 0x004D8390;
		
		offsets.globalSett = 0x016A7F38;

		offsets.gameDataAllOffset = reinterpret_cast<gameDataAllStruct*>(0x02c2bed8);

		offsets.consoleCommands = reinterpret_cast<consoleCommands*>(0x02c73df8);

		offsets.guildDataStart = 0x18B47E8;
		
		offsets.scriptCommandVFT = 0x013933D0;
		
		offsets.religionDatabase = 0x016A0B90;
		offsets.perfectSpy = 0x016A7CC4;
		
		offsets.portraitDatabase = 0x018671D0;
		offsets.traitDb = 0x161F0B8;
		offsets.ancillaryDb = 0x161F068;

		offsets.cultureDatabase = 0x0161F0F0;
		offsets.loadGameHandler = 0x02C6DC60;
		offsets.currentGameHandler = 0x016A2FA4;
		offsets.logFlushRate = 0x169CF94;

		offsets.edbDataStart = 0x01861438;
		offsets.selectedEra = 0x02C70178;
		offsets.campaignDb = 0x0161E7E4;
		offsets.campaignDbExtra = 0x0186170C;
		offsets.globalCadClass = 0x0162C740;
		offsets.constructionItems = 0x016ABB14;
		offsets.recruitmentItems = 0x0016ABC68;
		offsets.ltgdGlobals = 0x016A7Fc4;
		offsets.groupLabels = 0x01B60580;
		offsets.stratCursorCoords = 0x02c3da48;
		offsets.battleCursorCoords = 0x02c3da30;
		offsets.modelsDb = 0x016a0b98;
		offsets.battlefieldEngines = 0x02C3A254;
		offsets.battleAreas = 0x02C3A248;
		offsets.options1 = 0x02C6D804;
		offsets.options2 = 0x01639EF0;
		offsets.campaignDiff1 = 0x01604000;
		offsets.campaignDiff2 = 0x016AB5C8;
		offsets.equipmentCosts = 0x0160DCC8;
		offsets.saveGameHandler = 0x02C1B904;
		offsets.ancLimit = 0x005A5CAD;
		offsets.maxUnitSize = 0x008efe01;
		offsets.maxBgSize1 = 0x008ec815;
		offsets.maxBgSize2 = 0x008ec81a;
		offsets.unlockConsoleCommands1 = 0x00d27ae3;
		offsets.unlockConsoleCommands2 = 0x00d2cd2a;
		offsets.highlightUnits = 0x009e1a64;
		offsets.religionLimit = 0x0411E0E;
		offsets.scriptCounters = 0x016A7A58;
		offsets.scriptCountersSet = 0x016A7A30;
		offsets.buildingChainLimit = 0x008AC174;
		offsets.guildCooldown = 0x004F5D7B;
		offsets.bugReport = 0x16A2900;
		
		offsets.fortVtbl = 0x12F12D4;
		offsets.fortSpyingVtbl = 0x012F12B0;
		offsets.settlementVtbl = 0x012FCF34;
		offsets.settlementSpyingVtbl = 0x012FCF14;
		offsets.uiNotify = 0x2C2BF48;
		offsets.mapTilesDb = 0x0196A004;
		offsets.campaignControllerVtbl = 0x12F308C;
		offsets.ltgdConfig = 0x01605134;
		offsets.descrRebels = 0x161F518;
		offsets.eventManager = 0x16A7A30;
		offsets.currentRegion = 0x18B4800;
		offsets.uiStratUiV2 = 0x2C6D1B0;
		offsets.isDLC = 0x016A284C;
		offsets.modelRigidCounts = 0x1B15EB4;
		offsets.groundMoveCosts = 0x01602188;
		offsets.expandedBinTable = 0x2C70760;
		offsets.descrCharacter = 0x01861788;
		offsets.gameScriptLogger = 0x02C747A0;
		offsets.sineValues = 0x2C9D800;
		offsets.currentCharacterActionType = 0x16A7CC0;
		offsets.aStarTilesGlobal = 0x16A7CC8;
		offsets.religionTriggerBug = 0x892FBC;

		offsets.audioEnable = reinterpret_cast<bool*>(0x01639f1d);
		offsets.audioMaster_vol = reinterpret_cast<int*>(0x01639f60);
		offsets.gameUnit_size = reinterpret_cast<int*>(0x01639f4c);
		offsets.descr_sm_factionslist = reinterpret_cast<smFactionsDb*>(0x0161f3b0);

		offsets.mapCacheSize1 = 0x00B66CF8;
		offsets.mapCacheSize2 = 0x00B67139;
		offsets.selectedFactionSymbol = 0x01640574;
		offsets.weaponLimit1 = 0x0073C202;
		offsets.weaponLimit2 = 0x0073C206;
		offsets.weaponLimit3 = 0x0073C295;
		offsets.weaponLimit4 = 0x0073C29F;
		offsets.weaponLimit5 = 0x005F7ACF;
		offsets.weaponLimit6 = 0x00475F50;
		offsets.weaponLimit7 = 0x00475F54;
		offsets.weaponLimit8 = 0x00632EB2;
		offsets.weaponLimit9 = 0x00632EA5;
	}
}
