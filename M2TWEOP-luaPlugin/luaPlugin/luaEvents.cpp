///
//![Lua logo](_static/Lua.png)
//M2TWEOP structures and functions. There are not many examples and descriptions here. Also note that the examples do not include many of the checks that would be required when creating modifications.
//@module LuaPlugin
//@author youneuoy
//@license GPL-3.0
#include "luaP.h"
#include "plugData.h"
void luaP::onPluginLoadF()
{
	///EOP Events
	//@section eventsList

	/***
	Events functions list.
	Just list, use it without EventsFunctionsList.!!!

	@tfield draw draw
	@tfield onReadGameDbsAtStart onReadGameDbsAtStart
	@tfield onClickAtTile onClickAtTile
	@tfield onNewGameStart onNewGameStart
	@tfield onCampaignMapLoaded onCampaignMapLoaded
	@tfield onCreateSaveFile onCreateSaveFile
	@tfield onLoadSaveFile onLoadSaveFile
	@tfield onPluginLoad onPluginLoad
	@tfield onEndSiege onEndSiege
	@tfield onStartSiege onStartSiege
	@tfield onSelectWorldpkgdesc onSelectWorldpkgdesc
	@tfield onChangeTurnNum onChangeTurnNum
	@tfield onCharacterSelected onCharacterSelected
	@tfield onPreFactionTurnStart onPreFactionTurnStart
	@tfield onFactionTurnStart onFactionTurnStart
	@tfield onFactionTurnEnd onFactionTurnEnd
	@tfield onFactionNewCapital onFactionNewCapital
	@tfield onFactionWarDeclared onFactionWarDeclared
	@tfield onFactionAllianceDeclared onFactionAllianceDeclared
	@tfield onFactionTradeAgreementMade onFactionTradeAgreementMade
	@tfield onFactionBreakAlliance onFactionBreakAlliance
	@tfield onGiveMoney onGiveMoney
	@tfield onUpdateAttitude onUpdateAttitude
	@tfield onDemeanour onDemeanour
	@tfield onGeneralAssaultsGeneral onGeneralAssaultsGeneral
	@tfield onGeneralAssaultsResidence onGeneralAssaultsResidence
	@tfield onGeneralCaptureSettlement onGeneralCaptureSettlement
	@tfield onGeneralCaptureResidence onGeneralCaptureResidence
	@tfield onSiegeEquipmentCompleted onSiegeEquipmentCompleted
	@tfield onPostBattle onPostBattle
	@tfield onMultiTurnMove onMultiTurnMove
	@tfield onSettlementSelected onSettlementSelected
	@tfield onSettlementUpgraded onSettlementUpgraded
	@tfield onSettlementConverted onSettlementConverted
	@tfield onCityRiots onCityRiots
	@tfield onUngarrisonedSettlement onUngarrisonedSettlement
	@tfield onUngarrisonedFort onUngarrisonedFort
	@tfield onGiveSettlement onGiveSettlement
	@tfield onOccupySettlement onOccupySettlement
	@tfield onExterminatePopulation onExterminatePopulation
	@tfield onSackSettlement onSackSettlement
	@tfield onAddedToBuildingQueue onAddedToBuildingQueue
	@tfield onBuildingDestroyed onBuildingDestroyed
	@tfield onBuildingCompleted onBuildingCompleted
	@tfield onEventCounter onEventCounter
	@tfield onFactionExcommunicated onFactionExcommunicated
	@tfield onDisaster onDisaster
	@tfield onHordeFormed onHordeFormed
	@tfield onAddedToTrainingQueue onAddedToTrainingQueue
	@tfield onUnitDisbanded onUnitDisbanded
	@tfield onUnitTrained onUnitTrained
	@tfield onAgentCreated onAgentCreated
	@tfield onObjSeen onObjSeen
	@tfield onTileSeen onTileSeen
	@tfield onGameReloaded onGameReloaded
	@tfield onTransgression onTransgression
	@tfield onPopeAcceptsCrusadeTarget onPopeAcceptsCrusadeTarget
	@tfield onCrusadeCalled onCrusadeCalled
	@tfield onCrusadeEnds onCrusadeEnds
	@tfield onPopeRejectsCrusadeTarget onPopeRejectsCrusadeTarget
	@tfield onArmyTakesCrusadeTarget onArmyTakesCrusadeTarget
	@tfield onUnitsDesertCrusade onUnitsDesertCrusade
	@tfield onPopeElected onPopeElected
	@tfield onVotedForPope onVotedForPope
	@tfield onAssassinCaughtAttackingPope onAssassinCaughtAttackingPope
	@tfield onInquisitorAppointed onInquisitorAppointed
	@tfield onSettlementPanelOpen onSettlementPanelOpen
	@tfield onFinancesPanelOpen onFinancesPanelOpen
	@tfield onFactionSummaryPanelOpen onFactionSummaryPanelOpen
	@tfield onFamilyTreePanelOpenFunc onFamilyTreePanelOpenFunc
	@tfield onDiplomaticStandingPanelOpen onDiplomaticStandingPanelOpen
	@tfield onDiplomacyPanelOpen onDiplomacyPanelOpen
	@tfield onPreBattlePanelOpen onPreBattlePanelOpen
	@tfield onNavalAutoResolvePanelOpen onNavalAutoResolvePanelOpen
	@tfield onCharacterPanelOpen onCharacterPanelOpen
	@tfield onTradePanelOpen onTradePanelOpen
	@tfield onRequestBuildingAdvice onRequestBuildingAdvice
	@tfield onRequestTrainingAdvice onRequestTrainingAdvice
	@tfield onMessageOpen onMessageOpen
	@tfield onIncomingMessage onIncomingMessage
	@tfield onMessageClosed onMessageClosed
	@tfield onButtonPressed onButtonPressed
	@tfield onScrollClosed onScrollClosed
	@tfield onScrollOpened onScrollOpened
	@tfield onUIElementVisible onUIElementVisible
	@tfield onScrollAdviceRequested onScrollAdviceRequested
	@tfield onSettlementScrollAdviceRequested onSettlementScrollAdviceRequested
	@tfield onPreBattleScrollAdviceRequested onPreBattleScrollAdviceRequested
	@tfield onNavalPreBattleScrollAdviceRequested onNavalPreBattleScrollAdviceRequested
	@tfield onCollegeOfCardinalsPanelOpen onCollegeOfCardinalsPanelOpen
	@tfield onGuildUpgraded onGuildUpgraded
	@tfield onGuildDestroyed onGuildDestroyed
	@tfield onBrotherAdopted onBrotherAdopted
	@tfield onBirth onBirth
	@tfield onCharacterComesOfAge onCharacterComesOfAge
	@tfield onCharacterMarries onCharacterMarries
	@tfield onCharacterMarriesPrincess onCharacterMarriesPrincess
	@tfield onCharacterBecomesAFather onCharacterBecomesAFather
	@tfield onNewAdmiralCreated onNewAdmiralCreated
	@tfield onShortcutTriggered onShortcutTriggered
	@tfield onBecomesFactionLeader onBecomesFactionLeader
	@tfield onBecomesFactionHeir onBecomesFactionHeir




	@table EventsFunctionsList
	*/




	/***
	Called every time an image is rendered for display

	@function draw
	@tparam LPDIRECT3DDEVICE9 pDevice

	@usage
	function draw(device)
		ImGui.Begin("test");

		ImGui.Text("Some test text")

		ImGui.End();
	end
	*/
	drawLuaFunc = new sol::function(luaState["draw"]);
	checkLuaFunc(&drawLuaFunc);

	resetDXFunc = new sol::function(luaState["deinitDX"]);
	checkLuaFunc(&resetDXFunc);

	initDXFunc = new sol::function(luaState["initDX"]);
	checkLuaFunc(&initDXFunc);

	/***
	Called when a new campaign is started from the menu.

	@function onNewGameStart

	@usage
	function onNewGameStart()
	--something here
	end
	*/



	onNewGameStart = new sol::function(luaState["onNewGameStart"]);
	checkLuaFunc(&onNewGameStart);

	/***
	Called after the game loads various db`s (edu, etc) at startup.

	@function onReadGameDbsAtStart

	@usage
	function onReadGameDbsAtStart()
	--something here
	end
	*/



	onReadGameDbsAtStart = new sol::function(luaState["onReadGameDbsAtStart"]);
	checkLuaFunc(&onReadGameDbsAtStart);

	/***
	Called on clicking the stratmap.

	@function onClickAtTile
	@tparam int x
	@tparam int y
	@usage
	clickAtMap={["isClicked"]=false,["xTile"]=0,["yTile"]=0}
	function draw(device)
		if(clickAtMap.isClicked==true)
		then
			print(tostring(ImGui.IsMouseDown(ImGuiMouseButton.ImGuiMouseButton_Right)).."\n");
			print(tostring(ImGui.IsMouseDown(ImGuiMouseButton.ImGuiMouseButton_Left)).."\n");
			print(tostring(clickAtMap.xTile).." "..tostring(clickAtMap.yTile));

			clickAtMap.isClicked=false;
		end
	end

	function onClickAtTile(x,y)
		clickAtMap.isClicked=true;
		clickAtMap.xTile=x;
		clickAtMap.yTile=y;
	end
	*/



	onClickAtTile = new sol::function(luaState["onClickAtTile"]);
	checkLuaFunc(&onClickAtTile);

	/***
	Called after loading the campaign map

	@function onCampaignMapLoaded

	@usage
	function onCampaignMapLoaded()
	--something here
	end
	*/



	onCampaignMapLoaded = new sol::function(luaState["onCampaignMapLoaded"]);
	checkLuaFunc(&onCampaignMapLoaded);

	/***
	Called on plugin load (at game start).

	@function onPluginLoad

	@usage
	--An example of using this event to perform actions at the beginning of a campaign once:

	isCampaignLoadedFirstTime=false;
	function onNewGameStart()
	  isCampaignLoadedFirstTime=true;
	end

	function onCampaignMapLoaded()
	  if(isCampaignLoadedFirstTime==false)
		then
		  do return end
		end


	  --here we do our stuff, what works only one time for each campaign
	  isCampaignLoadedFirstTime=false;
	end
	*/



	onPluginLoad = new sol::function(luaState["onPluginLoad"]);
	checkLuaFunc(&onPluginLoad);


	/***
	Called on creating a new save file.

	@function onCreateSaveFile

	@usage
		function onCreateSaveFile()
			local savefiles, currentPath = {}, M2TWEOP.getPluginPath()
			savefiles[1] = currentPath.."\\testSaveFile1.ourSave"
			savefiles[2] = currentPath.."\\testSaveFile2.ourSave"

			file1 = io.open(savefiles[1], "w"); file1:write("This is save file 1!!!"); file1:close()
			file2 = io.open(savefiles[2], "w"); file2:write("This is save file 2!!!"); file2:close()

			return savefiles
		end
	*/
	onCreateSaveFile = new sol::function(luaState["onCreateSaveFile"]);
	checkLuaFunc(&onCreateSaveFile);

	/***
	Called on loading a save file.

	@function onLoadSaveFile

	@usage
	function onLoadSaveFile(paths)
		lastSaveFiles={};
		for key, value in pairs(paths) do

		print(key, " -- ", value);

		lastSaveFiles[key]={};
		lastSaveFiles[key][1]=value;
		lastSaveFiles[key][2]=readFile(value);

		end
	end


	--read file and return it`s content
	local function readFile(path)
		local file = io.open(path, "r") ;
		if (not file) then return nil end
		local content = file:read "*a";
		file:close();
		return content;
	end
	*/
	onLoadSaveFile = new sol::function(luaState["onLoadSaveFile"]);
	checkLuaFunc(&onLoadSaveFile);


	/***
	Called at the start of a new turn.

	@function onChangeTurnNum
	@tparam int turnNumber

	@usage
	function onChangeTurnNum(turnNum)
	--something here
	end
	*/
	onChangeTurnNumFunc = new sol::function(luaState["onChangeTurnNum"]);
	checkLuaFunc(&onChangeTurnNumFunc);

	/***
	Called on select worldpkgdesc for battlemap. See https://wiki.twcenter.net/index.php?title=.worldpkgdesc-M2TW. M2TWEOP will ignore the return value if its group does not match the required group!

	@function onSelectWorldpkgdesc
	@tparam string selectedRecordName
	@tparam string selectedRecordGroup
	@treturn string newSelectedRecordName

	@usage
	function onSelectWorldpkgdesc(selectedRecord,selectedRecordGroup)
	--something here

		return selectedRecord;
	end
	*/
	onSelectWorldpkgdesc = new sol::function(luaState["onSelectWorldpkgdesc"]);
	checkLuaFunc(&onSelectWorldpkgdesc);
	/***
	Called on specified fortificationlevel in a siege of a settlement.

	@function onfortificationlevelS
	@tparam settlementStruct siegedSettlement
	@treturn int overridedFortificationlevel
	@treturn bool isCastle override settlement type (siege equipment is slightly different between cities and castles of the same level)

	@usage
	function onfortificationlevelS(settlement)
		if settlement.xCoord == 10 and settlement.yCoord == 25 then
			return 3, false --override settlement under siege at these coordinates to level 3 city
		end
		return nil --else, do not override
	end
	*/
	onfortificationlevelS = new sol::function(luaState["onfortificationlevelS"]);
	checkLuaFunc(&onfortificationlevelS);

	/***
	Called on the completion of the siege (in any way, with any outcome).

	@function onEndSiege
	@tparam int xCoord x coordinate of siege(settlement or fort)
	@tparam int yCoord y coordinate of siege(settlement or fort)

	@usage
	function onEndSiege(x,y)
	--something here
	end
	*/
	onEndSiege = new sol::function(luaState["onEndSiege"]);
	checkLuaFunc(&onEndSiege);
	/***
	Called on the starting of the siege (in any way, with any outcome).

	@function onStartSiege
	@tparam int xCoord x coordinate of siege(settlement or fort)
	@tparam int yCoord y coordinate of siege(settlement or fort)

	@usage
	function onStartSiege(x,y)
	--something here
	end
	*/
	onStartSiege = new sol::function(luaState["onStartSiege"]);
	checkLuaFunc(&onStartSiege);
	/***
	A character has been selected by any means (including through the UI).

	@function onCharacterSelected
	@tparam namedCharacter selectedChar

	@usage
	function onCharacterSelected(selectedChar)
	--something here
	end
	*/
	onCharacterSelectedFunc = new sol::function(luaState["onCharacterSelected"]);
	checkLuaFunc(&onCharacterSelectedFunc);

	/***
	Called before the faction's turn starts.

	@function onPreFactionTurnStart
	@tparam factionStruct faction

	@usage
	function onPreFactionTurnStart(fac)
	--something here
	end
	*/
	onPreFactionTurnStartFunc = new sol::function(luaState["onPreFactionTurnStart"]);
	checkLuaFunc(&onPreFactionTurnStartFunc);

	/***
	Called at a faction's turn start.

	@function onFactionTurnStart
	@tparam factionStruct faction

	@usage
	function onFactionTurnStart(fac)
	--something here
	end
	*/
	onFactionTurnStartFunc = new sol::function(luaState["onFactionTurnStart"]);
	checkLuaFunc(&onFactionTurnStartFunc);

	/***
	Called at a faction's turn end.

	@function onFactionTurnEnd
	@tparam factionStruct faction

	@usage
	function onFactionTurnEnd(fac)
	--something here
	end
	*/
	onFactionTurnEndFunc = new sol::function(luaState["onFactionTurnEnd"]);
	checkLuaFunc(&onFactionTurnEndFunc);

	/***
	Called after faction changes to a new capital.

	@function onFactionNewCapital
	@tparam factionStruct faction

	@usage
	function onFactionNewCapital(fac)
	newCapital=fac.capital();
	end
	*/
	onFactionNewCapitalFunc = new sol::function(luaState["onFactionNewCapital"]);
	checkLuaFunc(&onFactionNewCapitalFunc);

	/***
	Called after a faction declares a war.

	@function onFactionWarDeclared
	@tparam factionStruct faction
	@tparam factionStruct targetFaction

	@usage
	function onFactionWarDeclared(faction,targetFaction)
	--something
	end
	*/
	onFactionWarDeclaredFunc = new sol::function(luaState["onFactionWarDeclared"]);
	checkLuaFunc(&onFactionWarDeclaredFunc);

	/***
	Called after a faction declares an alliance.

	@function onFactionAllianceDeclared
	@tparam factionStruct faction
	@tparam factionStruct targetFaction

	@usage
	function onFactionAllianceDeclared(faction,targetFaction)
	--something
	end
	*/

	onFactionAllianceDeclaredFunc = new sol::function(luaState["onFactionAllianceDeclared"]);
	checkLuaFunc(&onFactionAllianceDeclaredFunc);

	/***
	Called after a faction makes a trade agreement.

	@function onFactionTradeAgreementMade
	@tparam factionStruct faction
	@tparam factionStruct targetFaction

	@usage
	function onFactionTradeAgreementMade(faction,targetFaction)
	--something
	end
	*/

	onFactionTradeAgreementMadeFunc = new sol::function(luaState["onFactionTradeAgreementMade"]);
	checkLuaFunc(&onFactionTradeAgreementMadeFunc);

	/***
	Called after a faction breaks an alliance.

	@function onFactionBreakAlliance
	@tparam factionStruct faction
	@tparam factionStruct targetFaction

	@usage
	function onFactionBreakAlliance(faction,targetFaction)
	--something
	end
	*/


	onFactionBreakAllianceFunc = new sol::function(luaState["onFactionBreakAlliance"]);
	checkLuaFunc(&onFactionBreakAllianceFunc);


	/***
	Called after a faction gives money to another faction.

	@function onGiveMoney
	@tparam factionStruct faction
	@tparam factionStruct targetFaction
	@tparam int amount

	@usage
	function onGiveMoney(faction,targetFaction,amount)
	--something
	end
	*/


	onGiveMoneyFunc = new sol::function(luaState["onGiveMoney"]);
	checkLuaFunc(&onGiveMoneyFunc);


	/***
	Called after a faction updates it's attitude towards another faction.

	@function onUpdateAttitude
	@tparam factionStruct faction
	@tparam factionStruct targetFaction

	@usage
	function onUpdateAttitude(faction,targetFaction)
	--something
	end
	*/

	onUpdateAttitudeFunc = new sol::function(luaState["onUpdateAttitude"]);
	checkLuaFunc(&onUpdateAttitudeFunc);

	/***
	A demeanour response has occured in diplomacy talks.

	@function onDemeanour
	@tparam factionStruct faction
	@tparam factionStruct targetFaction
	@tparam float amount

	@usage
	function onDemeanour(faction,targetFaction,amount)
	--something
	end
	*/

	onDemeanourFunc = new sol::function(luaState["onDemeanour"]);
	checkLuaFunc(&onDemeanourFunc);

	/***
	A general has attacked another general.

	@function onGeneralAssaultsGeneral
	@tparam namedCharacter attacker
	@tparam namedCharacter defender

	@usage
	function onGeneralAssaultsGeneral(attacker,defender)
	--something
	end
	*/

	onGeneralAssaultsGeneralFunc = new sol::function(luaState["onGeneralAssaultsGeneral"]);
	checkLuaFunc(&onGeneralAssaultsGeneralFunc);

	/***
	An assault has taken place.

	@function onGeneralAssaultsResidence
	@tparam namedCharacter attacker
	@tparam settlementStruct settlement nil if it is fort
	@tparam fortStruct fort nil if it is settlement

	@usage
	function onGeneralAssaultsResidence(attacker,settlement,fort)
		if(settlement~=nil)
		then
			-settlement
		else
			-fort
		end

	end
	*/

	onGeneralAssaultsResidenceFunc = new sol::function(luaState["onGeneralAssaultsResidence"]);
	checkLuaFunc(&onGeneralAssaultsResidenceFunc);

	/***
	A General has captured a settlement.

	@function onGeneralCaptureSettlement
	@tparam namedCharacter attacker
	@tparam settlementStruct settlement

	@usage
	function onGeneralCaptureSettlement(attacker,settlement)
	--something
	end
	*/

	onGeneralCaptureSettlementFunc = new sol::function(luaState["onGeneralCaptureSettlement"]);
	checkLuaFunc(&onGeneralCaptureSettlementFunc);

	/***
	A General has captured a residence such as a fort or watchtower.

	@function onGeneralCaptureResidence
	@tparam namedCharacter attacker

	@usage
	function onGeneralCaptureResidence(attacker)
	--something
	end
	*/

	onGeneralCaptureResidenceFunc = new sol::function(luaState["onGeneralCaptureResidence"]);
	checkLuaFunc(&onGeneralCaptureResidenceFunc);

	/***
	Siege equipment has been completed by one of the sieging armies at this residence.

	@function onSiegeEquipmentCompleted
	@tparam settlementStruct settlement nil if it is fort
	@tparam fortStruct fort nil if it is settlement

	@usage
	--something
	*/

	onSiegeEquipmentCompletedFunc = new sol::function(luaState["onSiegeEquipmentCompleted"]);
	checkLuaFunc(&onSiegeEquipmentCompletedFunc);

	/***
	When a battle has finished.

	@function onPostBattle
	@tparam namedCharacter character

	@usage
	--something
	*/

	onPostBattleFunc = new sol::function(luaState["onPostBattle"]);
	checkLuaFunc(&onPostBattleFunc);

	/***
	The player has attempted to move a character beyond that character's given movement range for that turn.

	@function onMultiTurnMove
	@tparam namedCharacter character

	@usage
	--something
	*/

	onMultiTurnMoveFunc = new sol::function(luaState["onMultiTurnMove"]);
	checkLuaFunc(&onMultiTurnMoveFunc);

	/***
	A settlement has been selected by any means (including through the UI).

	@function onSettlementSelected
	@tparam settlementStruct settlement

	@usage
	function onSettlementSelected(settlement)
		print("Function: onSettlementSelected()\n\tSettlement: "..settlement.name)
	end
	*/

	onSettlementSelectedFunc = new sol::function(luaState["onSettlementSelected"]);
	checkLuaFunc(&onSettlementSelectedFunc);

	/***
	A settlement's level has been increased.

	@function onSettlementUpgraded
	@tparam settlementStruct settlement

	@usage
	function onSettlementUpgraded(settlement)
		print("Function: onSettlementUpgraded()\n\tSettlement Level: "..settlement.level)
	end
	*/

	onSettlementUpgradedFunc = new sol::function(luaState["onSettlementUpgraded"]);
	checkLuaFunc(&onSettlementUpgradedFunc);

	/***
	A settlement converted between city and castle and vice-versa.

	@function onSettlementConverted
	@tparam settlementStruct settlement

	@usage
	function onSettlementConverted(settlement)
		if settlement.isCastle == 1 then
			--do stuff
		end
	end
	*/

	onSettlementConvertedFunc = new sol::function(luaState["onSettlementConverted"]);
	checkLuaFunc(&onSettlementConvertedFunc);

	/***
	A settlement has rioted.

	@function onCityRiots
	@tparam settlementStruct settlement

	@usage
	--something
	*/

	onCityRiotsFunc = new sol::function(luaState["onCityRiots"]);
	checkLuaFunc(&onCityRiotsFunc);

	/***
	The last unit has been removed from a settlement, agents do not count.

	@function onUngarrisonedSettlement
	@tparam settlementStruct settlement

	@usage
	--something
	*/

	onUngarrisonedSettlementFunc = new sol::function(luaState["onUngarrisonedSettlement"]);
	checkLuaFunc(&onUngarrisonedSettlementFunc);

	/***
	The last unit has been removed from a fort, agents do not count.

	@function onUngarrisonedFort
	@tparam fortStruct fort

	@usage
	--something
	*/

	onUngarrisonedFortFunc = new sol::function(luaState["onUngarrisonedFort"]);
	checkLuaFunc(&onUngarrisonedFortFunc);

	/***
	A settlement has been given to another faction.

	@function onGiveSettlement
	@tparam settlementStruct settlement
	@tparam factionStruct fac1
	@tparam factionStruct fac2


	@usage
	--something
	*/

	onGiveSettlementFunc = new sol::function(luaState["onGiveSettlement"]);
	checkLuaFunc(&onGiveSettlementFunc);

	/***
	A general has captured a settlement and the occupy option has been chosen

	@function onOccupySettlement
	@tparam namedCharacter character only generals, event does not fire for captains
	@tparam factionStruct targetFaction

	@usage
	--something
	*/

	onOccupySettlementFunc = new sol::function(luaState["onOccupySettlement"]);
	checkLuaFunc(&onOccupySettlementFunc);

	/***
	A general has captured a settlement and the exterminate option has been chosen.

	@function onExterminatePopulation
	@tparam namedCharacter character only generals, event does not fire for captains
	@tparam factionStruct targetFaction


	@usage
	--something
	*/

	onExterminatePopulationFunc = new sol::function(luaState["onExterminatePopulation"]);
	checkLuaFunc(&onExterminatePopulationFunc);

	/***
	A general has captured a settlement and the sack option has been chosen.

	@function onSackSettlement
	@tparam namedCharacter character only generals, event does not fire for captains
	@tparam factionStruct targetFaction


	@usage
	--something
	*/

	onSackSettlementFunc = new sol::function(luaState["onSackSettlement"]);
	checkLuaFunc(&onSackSettlementFunc);

	/***
	A building has been added to the construction queue.

	@function onAddedToBuildingQueue
	@tparam settlementStruct settlement
	@tparam string buildNme level name


	@usage
	--something
	*/

	onAddedToBuildingQueueFunc = new sol::function(luaState["onAddedToBuildingQueue"]);
	checkLuaFunc(&onAddedToBuildingQueueFunc);

	/***
	A building has been destroyed.

	@function onBuildingDestroyed
	@tparam settlementStruct settlement
	@tparam string buildNme level name


	@usage
	--something
	*/

	onBuildingDestroyedFunc = new sol::function(luaState["onBuildingDestroyed"]);
	checkLuaFunc(&onBuildingDestroyedFunc);

	/***
	A building has been completed.

	@function onBuildingCompleted
	@tparam factionStruct fac
	@tparam settlementStruct settlement

	@usage
	--something
	*/

	onBuildingCompletedFunc = new sol::function(luaState["onBuildingCompleted"]);
	checkLuaFunc(&onBuildingCompletedFunc);

	/***
	An event counter has changed it's value.

	@function onEventCounter
	@tparam string counter

	@usage
	--something
	*/

	onEventCounterFunc = new sol::function(luaState["onEventCounter"]);
	checkLuaFunc(&onEventCounterFunc);

	/***
	When a faction is excommunicated by the Pope.

	@function onFactionExcommunicated
	@tparam factionStruct fac

	@usage
	--something
	*/

	onFactionExcommunicatedFunc = new sol::function(luaState["onFactionExcommunicated"]);
	checkLuaFunc(&onFactionExcommunicatedFunc);

	/***
	When a particular disaster has just happened.

	@function onDisaster
	@tparam int eventType (earthquake, flood, horde, fire, riot, storm, volcano), Don't know enum order!

	@usage
	--something
	*/

	onDisasterFunc = new sol::function(luaState["onDisaster"]);
	checkLuaFunc(&onDisasterFunc);

	/***
	A Faction has formed a horde.

	@function onHordeFormed
	@tparam factionStruct fac

	@usage
	--something
	*/

	onHordeFormedFunc = new sol::function(luaState["onHordeFormed"]);
	checkLuaFunc(&onHordeFormedFunc);

	/***
	A unit has been added to the training queue.

	@function onAddedToTrainingQueue
	@tparam settlementStruct sett
	@tparam string unitName

	@usage
	--something
	*/

	onAddedToTrainingQueueFunc = new sol::function(luaState["onAddedToTrainingQueue"]);
	checkLuaFunc(&onAddedToTrainingQueueFunc);

	/***
	A unit has been disbanded.

	@function onUnitDisbanded
	@tparam factionStruct fac
	@tparam unit unit

	@usage
	--something
	*/

	onUnitDisbandedFunc = new sol::function(luaState["onUnitDisbanded"]);
	checkLuaFunc(&onUnitDisbandedFunc);

	/***
	A unit has been trained.

	@function onUnitTrained
	@tparam factionStruct fac
	@tparam settlementStruct sett
	@tparam unit unit

	@usage
	--something
	*/

	onUnitTrainedFunc = new sol::function(luaState["onUnitTrained"]);
	checkLuaFunc(&onUnitTrainedFunc);

	/***
	An agent has been trained.

	@function onAgentCreated
	@tparam namedCharacter character
	@tparam int agentType see @{character:setTypeID} for list of type integers
	@tparam settlementStruct sett

	@usage
	--something
	*/

	onAgentCreatedFunc = new sol::function(luaState["onAgentCreated"]);
	checkLuaFunc(&onAgentCreatedFunc);

	/***
	An object of the target faction has been seen by the faction.

	@function onObjSeen
	@tparam factionStruct fac Faction that saw the object.
	@tparam factionStruct targetFac Faction that owns the object.
	@tparam int xCoord
	@tparam int yCoord

	@usage
	--something
	*/

	onObjSeenFunc = new sol::function(luaState["onObjSeen"]);
	checkLuaFunc(&onObjSeenFunc);

	/***
	The tile has been seen by the faction.

	@function onTileSeen
	@tparam factionStruct fac
	@tparam int xCoord
	@tparam int yCoord

	@usage
	--something
	*/

	onTileSeenFunc = new sol::function(luaState["onTileSeen"]);
	checkLuaFunc(&onTileSeenFunc);

	/***
	The game has been reloaded while on the strategy map.

	@function onGameReloaded
	@tparam int something

	@usage
	--something
	*/

	onGameReloadedFunc = new sol::function(luaState["onGameReloaded"]);
	checkLuaFunc(&onGameReloadedFunc);

	/***
	A faction to faction transgression has occurred.

	@function onTransgression
	@tparam factionStruct fac
	@tparam string description
	@tparam factionStruct targetFac

	@usage
	--something
	*/

	onTransgressionFunc = new sol::function(luaState["onTransgression"]);
	checkLuaFunc(&onTransgressionFunc);

	/***
	The Pope has accepted the player's crusade target. Deus Vult!

	@function onPopeAcceptsCrusadeTarget
	@tparam crusadeStruct crusade
	@tparam settlementStruct target

	@usage
	--something
	*/

	onPopeAcceptsCrusadeTargetFunc = new sol::function(luaState["onPopeAcceptsCrusadeTarget"]);
	checkLuaFunc(&onPopeAcceptsCrusadeTargetFunc);

	/***
	A Crusade or Jihad has been called.

	@function onCrusadeCalled
	@tparam crusadeStruct crusade
	@tparam settlementStruct target

	@usage
	--something
	*/

	onCrusadeCalledFunc = new sol::function(luaState["onCrusadeCalled"]);
	checkLuaFunc(&onCrusadeCalledFunc);

	/***
	A Crusade or Jihad has ended.

	@function onCrusadeEnds
	@tparam crusadeStruct crusade
	@tparam settlementStruct target

	@usage
	--something
	*/

	onCrusadeEndsFunc = new sol::function(luaState["onCrusadeEnds"]);
	checkLuaFunc(&onCrusadeEndsFunc);

	/***
	The Pope has rejected the player's crusade target.

	@function onPopeRejectsCrusadeTarget
	@tparam crusadeStruct crusade
	@tparam settlementStruct target

	@usage
	--something
	*/

	onPopeRejectsCrusadeTargetFunc = new sol::function(luaState["onPopeRejectsCrusadeTarget"]);
	checkLuaFunc(&onPopeRejectsCrusadeTargetFunc);

	/***
	An army has taken a crusade or jihad target settlement.

	@function onArmyTakesCrusadeTarget
	@tparam crusadeStruct crusade
	@tparam settlementStruct target
	@tparam stackStruct army

	@usage
	--something
	*/

	onArmyTakesCrusadeTargetFunc = new sol::function(luaState["onArmyTakesCrusadeTarget"]);
	checkLuaFunc(&onArmyTakesCrusadeTargetFunc);

	/***
	Units have deserted a crusade or jihad.

	@function onUnitsDesertCrusade
	@tparam crusadeStruct crusade
	@tparam settlementStruct target
	@tparam factionStruct fac

	@usage
	--something
	*/

	onUnitsDesertCrusadeFunc = new sol::function(luaState["onUnitsDesertCrusade"]);
	checkLuaFunc(&onUnitsDesertCrusadeFunc);

	/***
	A new pope has been elected.

	@function onPopeElected
	@tparam factionStruct fac
	@tparam factionStruct fac2

	@usage
	--something
	*/

	onPopeElectedFunc = new sol::function(luaState["onPopeElected"]);
	checkLuaFunc(&onPopeElectedFunc);

	/***
	A Faction has voted for the new pope.

	@function onVotedForPope
	@tparam factionStruct fac

	@usage
	--something
	*/

	onVotedForPopeFunc = new sol::function(luaState["onVotedForPope"]);
	checkLuaFunc(&onVotedForPopeFunc);

	/***
	An assassination mission against the pope has failed and the assassin is executed.

	@function onAssassinCaughtAttackingPope
	@tparam factionStruct fac

	@usage
	--something
	*/

	onAssassinCaughtAttackingPopeFunc = new sol::function(luaState["onAssassinCaughtAttackingPope"]);
	checkLuaFunc(&onAssassinCaughtAttackingPopeFunc);

	/***
	An inquisitor has been dispatched to a region.

	@function onInquisitorAppointed
	@tparam factionStruct fac
	@tparam settlementStruct sett

	@usage
	--something
	*/

	onInquisitorAppointedFunc = new sol::function(luaState["onInquisitorAppointed"]);
	checkLuaFunc(&onInquisitorAppointedFunc);

	/***
	The player has opened the panel for the selected settlement.

	@function onSettlementPanelOpen
	@tparam settlementStruct sett

	@usage
	--something
	*/

	onSettlementPanelOpenFunc = new sol::function(luaState["onSettlementPanelOpen"]);
	checkLuaFunc(&onSettlementPanelOpenFunc);

	/***
	The player has opened the finances panel.

	@function onFinancesPanelOpen
	@tparam factionStruct fac

	@usage
	--something
	*/

	onFinancesPanelOpenFunc = new sol::function(luaState["onFinancesPanelOpen"]);
	checkLuaFunc(&onFinancesPanelOpenFunc);

	/***
	The player has opened the faction summary panel.

	@function onFactionSummaryPanelOpen
	@tparam factionStruct fac

	@usage
	--something
	*/

	onFactionSummaryPanelOpenFunc = new sol::function(luaState["onFactionSummaryPanelOpen"]);
	checkLuaFunc(&onFactionSummaryPanelOpenFunc);

	/***
	The player has opened the family tree panel.

	@function onFamilyTreePanelOpenFunc
	@tparam factionStruct fac

	@usage
	--something
	*/

	onFamilyTreePanelOpenFunc = new sol::function(luaState["onFamilyTreePanelOpenFunc"]);
	checkLuaFunc(&onFamilyTreePanelOpenFunc);

	/***
	The player has opened the diplomatic standing panel.

	@function onDiplomaticStandingPanelOpen
	@tparam factionStruct fac

	@usage
	--something
	*/

	onDiplomaticStandingPanelOpenFunc = new sol::function(luaState["onDiplomaticStandingPanelOpen"]);
	checkLuaFunc(&onDiplomaticStandingPanelOpenFunc);

	/***
	The player has opened a diplomacy panel.

	@function onDiplomacyPanelOpen
	@tparam factionStruct fac

	@usage
	--something
	*/

	onDiplomacyPanelOpenFunc = new sol::function(luaState["onDiplomacyPanelOpen"]);
	checkLuaFunc(&onDiplomacyPanelOpenFunc);

	/***
	The player has attacked an army or settlement. The decision panel is now open.

	@function onPreBattlePanelOpen
	@tparam factionStruct fac

	@usage
	--something
	*/

	onPreBattlePanelOpenFunc = new sol::function(luaState["onPreBattlePanelOpen"]);
	checkLuaFunc(&onPreBattlePanelOpenFunc);

	/***
	The naval auto resolve panel has opened.

	@function onNavalAutoResolvePanelOpen

	@usage
	--something
	*/

	onNavalAutoResolvePanelOpenFunc = new sol::function(luaState["onNavalAutoResolvePanelOpen"]);
	checkLuaFunc(&onNavalAutoResolvePanelOpenFunc);

	/***
	The player has opened the panel for the selected character.

	@function onCharacterPanelOpen
	@tparam namedCharacter selectedChar

	@usage
	--something
	*/

	onCharacterPanelOpenFunc = new sol::function(luaState["onCharacterPanelOpen"]);
	checkLuaFunc(&onCharacterPanelOpenFunc);

	/***
	The player has opened a trade panel.

	@function onTradePanelOpen
	@tparam settlementStruct sett

	@usage
	--something
	*/

	onTradePanelOpenFunc = new sol::function(luaState["onTradePanelOpen"]);
	checkLuaFunc(&onTradePanelOpenFunc);

	/***
	The player has requested building advice.

	@function onRequestBuildingAdvice
	@tparam settlementStruct sett

	@usage
	--something
	*/

	onRequestBuildingAdviceFunc = new sol::function(luaState["onRequestBuildingAdvice"]);
	checkLuaFunc(&onRequestBuildingAdviceFunc);

	/***
	The player has requested training advice.

	@function onRequestTrainingAdvice
	@tparam settlementStruct sett
	@tparam eduEntry recommendedUnitEntry

	@usage
	--something
	*/

	onRequestTrainingAdviceFunc = new sol::function(luaState["onRequestTrainingAdvice"]);
	checkLuaFunc(&onRequestTrainingAdviceFunc);

	/***
	The player has opened a message.

	@function onMessageOpen
	@tparam factionStruct fac
	@tparam int msgType

	@usage
	--something
	*/

	onMessageOpenFunc = new sol::function(luaState["onMessageOpen"]);
	checkLuaFunc(&onMessageOpenFunc);

	/***
	The player has recieved a message.

	@function onIncomingMessage
	@tparam factionStruct fac
	@tparam int msgType

	@usage
	--something
	*/

	onIncomingMessageFunc = new sol::function(luaState["onIncomingMessage"]);
	checkLuaFunc(&onIncomingMessageFunc);

	/***
	The player has closed a message.

	@function onMessageClosed
	@tparam int msgType

	@usage
	--something
	*/

	onMessageClosedFunc = new sol::function(luaState["onMessageClosed"]);
	checkLuaFunc(&onMessageClosedFunc);

	/***
	The player has clicked on a button.

	@function onButtonPressed
	@tparam string buttonName

	@usage
	--something
	*/

	onButtonPressedFunc = new sol::function(luaState["onButtonPressed"]);
	checkLuaFunc(&onButtonPressedFunc);

	/***
	The player closed a scroll.

	@function onScrollClosed
	@tparam string scrollName

	@usage
	--something
	*/

	onScrollClosedFunc = new sol::function(luaState["onScrollClosed"]);
	checkLuaFunc(&onScrollClosedFunc);

	/***
	The player opened a scroll.

	@function onScrollOpened
	@tparam string scrollName

	@usage
	--something
	*/

	onScrollOpenedFunc = new sol::function(luaState["onScrollOpened"]);
	checkLuaFunc(&onScrollOpenedFunc);

	/***
	A special UI Element is visible.

	@function onUIElementVisible
	@tparam string elementName

	@usage
	--something
	*/

	onUIElementVisibleFunc = new sol::function(luaState["onUIElementVisible"]);
	checkLuaFunc(&onUIElementVisibleFunc);

	/***
	The player has requested advice on a scroll.

	@function onScrollAdviceRequested
	@tparam string scrollName

	@usage
	--something
	*/

	onScrollAdviceRequestedFunc = new sol::function(luaState["onScrollAdviceRequested"]);
	checkLuaFunc(&onScrollAdviceRequestedFunc);

	/***
	The player has requested advice on the settlement scroll.

	@function onSettlementScrollAdviceRequested
	@tparam settlementStruct sett
	@tparam string scrollName

	@usage
	--something
	*/

	onSettlementScrollAdviceRequestedFunc = new sol::function(luaState["onSettlementScrollAdviceRequested"]);
	checkLuaFunc(&onSettlementScrollAdviceRequestedFunc);

	/***
	The player has requested advice on the prebattle scroll.

	@function onPreBattleScrollAdviceRequested

	@usage
	--something
	*/

	onPreBattleScrollAdviceRequestedFunc = new sol::function(luaState["onPreBattleScrollAdviceRequested"]);
	checkLuaFunc(&onPreBattleScrollAdviceRequestedFunc);

	/***
	The player has requested advice on the naval prebattle scroll.

	@function onNavalPreBattleScrollAdviceRequested

	@usage
	--something
	*/

	onNavalPreBattleScrollAdviceRequestedFunc = new sol::function(luaState["onNavalPreBattleScrollAdviceRequested"]);
	checkLuaFunc(&onNavalPreBattleScrollAdviceRequestedFunc);

	/***
	The player has opened the college of cardinals panel.

	@function onCollegeOfCardinalsPanelOpen
	@tparam collegeCardinalsStruct college

	@usage
	--something
	*/

	onCollegeOfCardinalsPanelOpenFunc = new sol::function(luaState["onCollegeOfCardinalsPanelOpen"]);
	checkLuaFunc(&onCollegeOfCardinalsPanelOpenFunc);

	/***
	A guild has been created or upgraded.

	@function onGuildUpgraded
	@tparam settlementStruct sett
	@tparam string guildName

	@usage
	function onGuildUpgraded(sett, guildName)
		print("Function: onGuildUpgraded()\n\tSettlement: "..sett.name.."\n\tGuild: "..guildName)
	end
	*/

	onGuildUpgradedFunc = new sol::function(luaState["onGuildUpgraded"]);
	checkLuaFunc(&onGuildUpgradedFunc);

	/***
	A guild has been destroyed.

	@function onGuildDestroyed
	@tparam settlementStruct sett
	@tparam int guildID

	@usage
	function onGuildDestroyed(sett, guildID)
		print("Function: onGuildDestroyed()\n\tSettlement: "..sett.name.."\n\tID: "..guildID)
	end
	*/

	onGuildDestroyedFunc = new sol::function(luaState["onGuildDestroyed"]);
	checkLuaFunc(&onGuildDestroyedFunc);

	/***
	A character in the family tree has received a brother by adoption.

	@function onBrotherAdopted
	@tparam namedCharacter character This is the original child, not the newly adopted character.

	@usage
	function onBrotherAdopted(character)
		print("Function: onBrotherAdopted()\n\tName: "..character.fullName.."\n\tParent: "..character.parent.fullName.."\n\tFaction: "..character.faction:getFactionName())
	end
	*/

	onBrotherAdoptedFunc = new sol::function(luaState["onBrotherAdopted"]);
	checkLuaFunc(&onBrotherAdoptedFunc);

	/***
	A child has been born.

	@function onBirth
	@tparam namedCharacter child

	@usage
	function onBirth(child)
		print("Function: onBirth()\n\tChild Name: "..child.fullName.."\n\tParent: "..child.parent.fullName.."\n\tFaction: "..child.faction:getFactionName())
	end
	*/

	onBirthFunc = new sol::function(luaState["onBirth"]);
	checkLuaFunc(&onBirthFunc);

	/***
	A character has come of age.

	@function onCharacterComesOfAge
	@tparam namedCharacter character Character's age is equal to age\_of\_manhood in descr\_campaign\_db.xml.

	@usage
	function onCharacterComesOfAge(character)
		print("Function: onCharacterComesOfAge()\n\tName: "..character.fullName.."\n\tParent: "..character.parent.fullName.."\n\tFaction: "..character.faction:getFactionName())
	end
	*/

	onCharacterComesOfAgeFunc = new sol::function(luaState["onCharacterComesOfAge"]);
	checkLuaFunc(&onCharacterComesOfAgeFunc);

	/***
	A character has married (someone who isn't a princess).

	@function onCharacterMarries
	@tparam namedCharacter husband

	@usage
	function onCharacterMarries(husband)
		print("Function: onCharacterMarries()\n\tName: "..husband.fullName.."\n\tSpouse: "..husband.spouse.fullName.."\n\tFaction: "..husband.faction:getFactionName())
	end
	*/

	onCharacterMarriesFunc = new sol::function(luaState["onCharacterMarries"]);
	checkLuaFunc(&onCharacterMarriesFunc);

	/***
	A character has married a princess agent.

	@function onCharacterMarriesPrincess
	@tparam namedCharacter husband

	@usage
	function onCharacterMarriesPrincess(husband)
		print("Function: onCharacterMarriesPrincess()\n\tName: "..husband.fullName.."\n\tSpouse: "..husband.spouse.fullName.."\n\tFaction: "..husband.faction:getFactionName())
	end
	*/

	onCharacterMarriesPrincessFunc = new sol::function(luaState["onCharacterMarriesPrincess"]);
	checkLuaFunc(&onCharacterMarriesPrincessFunc);

	/***
	A child has been born.

	@function onCharacterBecomesAFather
	@tparam namedCharacter father Note: this event fires for all child births for this father, not just the first one.

	@usage
	function onCharacterBecomesAFather(father)
		local i, children = 1, ""
		while father.childs[i] ~= nil do
			children, i = children.." "..father.childs[i].fullName..",", i + 1
		end
		children = children:gsub(",$", "")
		print("Function: onCharacterBecomesAFather()\n\tName: "..father.fullName.."\n\tChildren:"..children.."\n\tFaction: "..father.faction:getFactionName())
	end
	*/

	onCharacterBecomesAFatherFunc = new sol::function(luaState["onCharacterBecomesAFather"]);
	checkLuaFunc(&onCharacterBecomesAFatherFunc);

	/***
	A new admiral has been created for a new ship.

	@function onNewAdmiralCreated
	@tparam namedCharacter admiral
	@tparam settlementStruct sett

	@usage
	function onNewAdmiralCreated(admiral, sett)
		print("Function: onNewAdmiralCreated()\n\tAdmiral: "..admiral.fullName.."\n\tSettlement: "..sett.name)
	end
	*/

	onNewAdmiralCreatedFunc = new sol::function(luaState["onNewAdmiralCreated"]);
	checkLuaFunc(&onNewAdmiralCreatedFunc);

	/***
	The player triggered a keyboard shortcut.

	@function onShortcutTriggered
	@tparam string shortcut

	@usage
	function onShortcutTriggered(shortcut)
		if shortcut == "strat_ui::mission_button" then
			--do stuff
		end
	end
	*/

	onShortcutTriggeredFunc = new sol::function(luaState["onShortcutTriggered"]);
	checkLuaFunc(&onShortcutTriggeredFunc);

	/***
	The character has been made the faction leader.

	@function onBecomesFactionLeader
	@tparam namedCharacter leader

	@usage
	function onBecomesFactionLeader(leader)
		print("Function: onBecomesFactionLeader()\n\tLeader: "..leader.fullName)
	end
	*/

	onBecomesFactionLeaderFunc = new sol::function(luaState["onBecomesFactionLeader"]);
	checkLuaFunc(&onBecomesFactionLeaderFunc);

	/***
	The character has been made a faction heir.

	@function onBecomesFactionHeir
	@tparam namedCharacter heir

	@usage
	function onBecomesFactionHeir(heir)
		print("Function: onBecomesFactionHeir()\n\tHeir: "..heir.fullName)
	end
	*/

	onBecomesFactionHeirFunc = new sol::function(luaState["onBecomesFactionHeir"]);
	checkLuaFunc(&onBecomesFactionHeirFunc);


	if (onPluginLoad != nullptr)
	{
		tryLua((*onPluginLoad)());
	}
}