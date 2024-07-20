///
//![Lua logo](../Lua.png)
//@module LuaPlugin
//@author Fynn
//@license GPL-3.0
#include "pch.h"
#include "campaignAi.h"

#include "faction.h"
#include "campaignDb.h"
#include "dataOffsets.h"
#include "gameHelpers.h"
#include "fort.h"
#include "unit.h"
#include "army.h"
#include "campaign.h"
#include "luaPlugin.h"
#include "strategyMap.h"
#include "techFuncs.h"

std::shared_ptr<globalEopAiConfig> globalEopAiConfig::m_Instance = std::make_shared<globalEopAiConfig>();

bool TURN_HAD_ACTION = false;
bool PLAYER_ASSAULTED = false;

namespace campaignAi
{

	ltgdGlobals* getLtgdGlobals()
	{
		return reinterpret_cast<ltgdGlobals*>(dataOffsets::offsets.ltgdGlobals);
	}

	ltgdConfig* getLtgdConfig()
	{
		return reinterpret_cast<ltgdConfig*>(dataOffsets::offsets.ltgdConfig);
	}

	militaryValuesLTGD* getAiFactionValues(const factionStruct* fac)
	{
		ltgdGlobals* ltgd = getLtgdGlobals();
		if (!ltgd)
			return nullptr;
		return &ltgd->ltgdFactionValues[fac->factionID];
	}

	interFactionLTGD* getInterFactionLTGD(const factionStruct* fac, const factionStruct* targetFac)
	{
		ltgdGlobals* ltgd = getLtgdGlobals();
		if (!ltgd)
			return nullptr;
		return &ltgd->interFactionLTGD[fac->factionID][targetFac->factionID];
	}

	int assessGarrisonStrength(const aiRegionData* gsdData, const settlementStruct* settlement, const factionStruct* faction)
	{
		int requiredGarrisonStrength = 0;
		int ownStr = settlement->army ? settlement->army->totalStrength : 0;
		int enemyStr = 0;
		const auto strModifier = campaignHelpers::getCampaignDb()->campaignDbAi.siegeAttStrModifier;
		const auto reg = stratMapHelpers::getRegion(settlement->regionID);
		for (int i = 0; i < reg->stacksNum; i++)
		{
			if (const auto army = reg->getArmy(i);
				army->faction->factionID == faction->factionID && army != settlement->army)
				ownStr += army->totalStrength / 2;
			else if (army->isEnemyToFaction(faction))
				enemyStr += army->settlement ? army->totalStrength / 2 : army->totalStrength;
			else if (!army->isAllyToFaction(faction))
				enemyStr += army->settlement ? army->totalStrength / 4 : army->totalStrength / 2;
		}
		for (int i = 0; i < reg->neighbourRegionsNum; i++)
		{
			const auto nRegion = reg->neighbourRegions[i];
			if (nRegion.isBlocked || nRegion.region->isSea || !nRegion.region->settlement)
				continue;
			for (int j = 0; j < nRegion.region->stacksNum; j++)
			{
				if (const auto army = nRegion.region->getArmy(j);
					army->faction->factionID == faction->factionID && army != settlement->army)
					ownStr += army->settlement ? army->totalStrength / 8 : army->totalStrength / 4;
				else if (army->isEnemyToFaction(faction))
					enemyStr += army->settlement ? army->totalStrength / 4 : army->totalStrength / 2;
				else if (!army->isAllyToFaction(faction))
					enemyStr += army->settlement ? army->totalStrength / 8 : army->totalStrength / 4;
			}
		}
		int balance = enemyStr - (ownStr * strModifier);
		if (const int po = settlement->stats.settlementStats.PublicOrder; po < 80)
			balance += 1000 - po * 10;
		if (balance < -100)
		{
			if (!settlement->army || (settlement->army && settlement->army->numOfUnits == 0))
				return 1000;
			return 0;
		}
		balance += clamp(settlement->stats.settlementStats.TotalIncomeWithoutAdmin / 10, 0, 5000);
		balance += clamp(gsdData->regionValue / 5, 0, 5000);
		requiredGarrisonStrength = clamp(balance, 0, 50000);
		return requiredGarrisonStrength;
	}
	

	void addToLua(sol::state& luaState)
	{
		struct
		{
			sol::usertype<globalEopAiConfig> eopAiConfig;
			sol::usertype<aiFactionData> aiFactionData;
		}typeAll;
		
		///EOP AI
		//@section EOP AI

		/***
		Basic eopAiConfig table
		@tfield bool enabled
		@tfield bool enableLogging
		@tfield float aggressionFactor
		@tfield float defenseFactor
		@tfield float residenceFactor
		@tfield float aidFactor
		@tfield float moveCostFactor
		@tfield float powerFactor
		@tfield float nonBorderFactor
		@tfield float invadePriorityFactor
		@tfield int maxTurnSearchCount
		@tfield getFactionData getFactionData

		@table eopAiConfig
		*/
		typeAll.eopAiConfig = luaState.new_usertype<globalEopAiConfig>("eopAiConfig");
		typeAll.eopAiConfig.set("enabled", sol::property(&globalEopAiConfig::getEnabled, &globalEopAiConfig::setEnabled));
		typeAll.eopAiConfig.set("enableLogging", sol::property(&globalEopAiConfig::getLoggingEnabled, &globalEopAiConfig::setLoggingEnabled));
		typeAll.eopAiConfig.set("aggressionFactor", sol::property(&globalEopAiConfig::getAggressionFactor, &globalEopAiConfig::setAggressionFactor));
		typeAll.eopAiConfig.set("defenseFactor",sol::property(&globalEopAiConfig::getDefenseFactor, &globalEopAiConfig::setDefenseFactor));
		typeAll.eopAiConfig.set("residenceFactor",sol::property(&globalEopAiConfig::getResidenceFactor, &globalEopAiConfig::setResidenceFactor));
		typeAll.eopAiConfig.set("aidFactor", sol::property(&globalEopAiConfig::getAidFactor, &globalEopAiConfig::setAidFactor));
		typeAll.eopAiConfig.set("moveCostFactor", sol::property(&globalEopAiConfig::getMoveCostFactor, &globalEopAiConfig::setMoveCostFactor));
		typeAll.eopAiConfig.set("powerFactor", sol::property(&globalEopAiConfig::getPowerFactor, &globalEopAiConfig::setPowerFactor));
		typeAll.eopAiConfig.set("nonBorderFactor", sol::property(&globalEopAiConfig::getNonBorderFactor, &globalEopAiConfig::setNonBorderFactor));
		typeAll.eopAiConfig.set("invadePriorityFactor", sol::property(&globalEopAiConfig::getInvadePriorityFactor, &globalEopAiConfig::setInvadePriorityFactor));
		typeAll.eopAiConfig.set("maxTurnSearchCount", sol::property(&globalEopAiConfig::getMaxTurnSearchCount, &globalEopAiConfig::setMaxTurnSearchCount));
		
		/***
		Get faction specific data.
		@function eopAiConfig.getFactionData
		@tparam factionStruct fac
		@treturn aiFactionData data
		@usage
			  local data = eopAiConfig.getFactionData(fac)
		*/
		typeAll.eopAiConfig.set_function("getFactionData", &globalEopAiConfig::getFactionDataLua);
		
		/***
		Basic aiFactionData table
		@tfield setTargetFactionFactor setTargetFactionFactor
		@tfield setAidFactionFactor setAidFactionFactor
		@tfield setSettlementFactor setSettlementFactor
		@tfield setTargetReligionFactor setTargetReligionFactor
		@tfield setAidReligionFactor setAidReligionFactor

		@table aiFactionData
		*/
		typeAll.aiFactionData = luaState.new_usertype<aiFactionData>("aiFactionData");
		
		/***
		Set factor for targetting a faction.
		@function aiFactionData:setTargetFactionFactor
		@tparam factionStruct fac
		@tparam float factor
		@usage
		      facData:setTargetFactionFactor(otherFac, 1.5)
		*/
		typeAll.aiFactionData.set_function("setTargetFactionFactor", &aiFactionData::setTargetFactionFactor);
		
		/***
		Set factor for aiding a faction.
		@function aiFactionData:setAidFactionFactor
		@tparam factionStruct fac
		@tparam float factor
		@usage
		      facData:setAidFactionFactor(otherFac, 1.5)
		*/
		typeAll.aiFactionData.set_function("setAidFactionFactor", &aiFactionData::setAidFactionFactor);
		
		/***
		Set factor for a specific settlement (defense or attack).
		@function aiFactionData:setSettlementFactor
		@tparam settlementStruct sett
		@tparam float factor
		@usage
		      facData:setSettlementFactor(someSett)
		*/
		typeAll.aiFactionData.set_function("setSettlementFactor", &aiFactionData::setSettlementFactor);
		
		/***
		Set factor for targetting a religion.
		@function aiFactionData:setTargetReligionFactor
		@tparam int religionID
		@tparam float factor
		@usage
			  facData:setTargetReligionFactor(3, 1.5)
		*/
		typeAll.aiFactionData.set_function("setTargetReligionFactor", &aiFactionData::setTargetReligionFactor);
		
		/***
		Set factor for aiding a religion.
		@function aiFactionData:setAidReligionFactor
		@tparam int religionID
		@tparam float factor
		@usage
			  facData:setAidReligionFactor(3, 1.5)
		*/
		typeAll.aiFactionData.set_function("setAidReligionFactor", &aiFactionData::setAidReligionFactor);
	}
}

bool armyResource::validate()
{
	if (used || !own || !army || army->inBattle || !army->gen || !army->gen->characterRecord || army->gen->markedForDeath)
		return false;
	return true;
}

void armyResource::calculatePositionPower()
{
	positionPower = army->calculatePositionPower();
}

void settlementResource::calculatePositionPower()
{
	positionPower = settlement->army ? settlement->army->totalStrength : 0;
	auto neighbourTiles = stratMapHelpers::getNeighbourTiles(settlement->xCoord, settlement->yCoord);
	while (true)
	{
		if (neighbourTiles.empty())
			break;
		const auto [checkX, checkY] = neighbourTiles.front();
		neighbourTiles.pop();
		const auto tile = stratMapHelpers::getTile(checkX, checkY);
		if (!tile)
			continue;
		const auto tileArmy = tile->getArmy(false);
		if (!tileArmy || tileArmy->isAdmiral)
			continue;
		if (tileArmy->isAllyToFaction(settlement->faction))
			positionPower += tileArmy->totalStrength;
		if (tileArmy->isEnemyToFaction(settlement->faction))
			positionPower -= tileArmy->totalStrength;
	}
}


float globalEopAiConfig::calculateSettPriority(const std::shared_ptr<settlementResource>& settRes, priorityType priType)
{
	const auto campaignDbAi = campaignHelpers::getCampaignDb()->campaignDbAi;
	const bool empty = !settRes->settlement->army || settRes->settlement->army->numOfUnits == 0;
	float balance;
	if (priType == priType_target)
		balance = (settRes->totalThreatReceiving * campaignDbAi.siegeAttStrModifier) / (max(settRes->positionPower, 0) + 1);
	else
		balance = settRes->totalThreatReceiving / ((max(settRes->positionPower, 0) * campaignDbAi.siegeAttStrModifier) + 1);
	if (balance < 1.f && !empty)
		return 0.f;
	const auto facData = getCurrentFactionData();
	float priority = clamp(settRes->settlement->stats.settlementStats.TotalIncomeWithoutAdmin * 2, 0, 50000);
	const auto religion = settlementHelpers::getReligion(settRes->settlement, m_Faction->religion);
	priority *= 1 + (religion * religionFactor);
	priority += settRes->settlement->getSettlementValue() * 10;
	priority *= facData->getSettlementFactor(settRes->settlement);
	if (settRes->settlement->governor && settRes->settlement->governor->getTypeID() == characterTypeStrat::namedCharacter)
	{
		priority *= 1.1f;
		if (settRes->settlement->governor->characterRecord->isHeir())
			priority *= 1.2f;
		if (settRes->settlement->governor->characterRecord->isLeader())
			priority *= 1.5f;
		if (settRes->settlement->governor->characterRecord->isFamily)
		{
			priority *= 1.25f;
			if (settRes->settlement->faction->getAliveCharacterNumOfType(characterTypeStrat::namedCharacter) < 3)
				priority *= 1.5f;
		}
	}
	if (priType == priType_target)
		priority -= clamp(settRes->totalSupportReceiving * 0.5f, 0.f, 50000.f);
	if (priority < 1)
		return 0.f;
	if (empty)
		priority *= 2.f;
	priority *= (clamp(balance * powerFactor, 0.f, 3.f));
	priority *= residenceFactor;
	const auto config = campaignAi::getLtgdConfig();
	switch(priType)
	{
	case priType_own:
		{
			if (settRes->settlement->siegeNum > 0)
				priority *= 2.5f;
			if (m_Faction->settlementsNum < 4)
				priority *= 6 - m_Faction->settlementsNum;
			priority *= defenseFactor;
			break;
		}
	case priType_target:
		{
			if (m_Faction->settlementsNum < 4)
				priority *= 4 - m_Faction->settlementsNum;
			if (settRes->settlement->siegeNum > 0)
			{
				if (settRes->settlement->getSiege(0)->army && settRes->settlement->getSiege(0)->army->isAllyToFaction(m_Faction))
					priority *= 5;
				else
					priority /= 4;
			}
			priority *= (config->getPriorityMod(m_Faction, settRes->settlement->faction) * invadePriorityFactor);
			priority *= facData->getTargetFactionFactor(settRes->settlement->faction);
			priority *= facData->getTargetReligionFactor(settRes->settlement->faction->religion);
			priority *= aggressionFactor;
			break;
		}
	case priType_aid:
		{
			if (m_Faction->settlementsNum < 4)
				priority /= 4 - m_Faction->settlementsNum;
			if (settRes->settlement->siegeNum > 0)
			{
				if (settRes->settlement->getSiege(0)->army && settRes->settlement->getSiege(0)->army->isEnemyToFaction(m_Faction))
					priority *= 3;
				else
					priority /= 3;
			}
			priority *= facData->getAidFactionFactor(settRes->settlement->faction);
			priority *= facData->getAidReligionFactor(settRes->settlement->faction->religion);
			const auto fs = factionHelpers::getFactionStanding(m_Faction, settRes->settlement->faction);
			priority *= (aidFactor * fs);
		}
		break;
	}
	if (!m_Faction->isInNeighbourArray(settRes->settlement->regionID))
		priority *= nonBorderFactor;
	return priority;
}

float globalEopAiConfig::calculateArmyPriority(const std::shared_ptr<armyResource>& armyRes, priorityType priType)
{
	const auto campaignDbAi = campaignHelpers::getCampaignDb()->campaignDbAi;
	float balance;
	if (priType == priType_target)
		balance = (armyRes->totalThreatReceiving * campaignDbAi.attStrModifier) / (max(armyRes->positionPower, 0) + 1);
	else
		balance = armyRes->totalThreatReceiving / ((max(armyRes->positionPower, 0) * campaignDbAi.attStrModifier) + 1);
	if (balance < 1.f)
		return 0.f;
	const auto facData = getCurrentFactionData();
	float priority = clamp(armyRes->army->totalStrength * 1.f, 0.f, 20000.f);
	priority += clamp(armyRes->totalThreatGiving * (priType == priType_target ? 3.5f : 1.f), 0.f, 30000.f);
	if (armyRes->army->gen && armyRes->army->gen->getTypeID() == characterTypeStrat::namedCharacter)
	{
		priority *= 1.25f;
		if (armyRes->army->gen->characterRecord->isLeader())
			priority *= 1.2f;
		if (armyRes->army->gen->characterRecord->isLeader())
			priority *= 1.5f;
		if (armyRes->army->gen->characterRecord->isFamily)
		{
			priority *= 1.5f;
			if (armyRes->army->faction->getAliveCharacterNumOfType(characterTypeStrat::namedCharacter) < 3)
				priority *= 2.0f;
		}
	}
	priority *= clamp(balance * powerFactor, 0.f, 3.f);
	if (priType == priType_target)
		priority -= clamp(armyRes->totalSupportReceiving, 0, 30000) ;
	if (priority < 1)
		return 0.f;
	if (armyRes->army->siege)
	{
		if (const auto targetSett = armyRes->army->siege->getSiegedSettlement(); targetSett)
		{
			if (targetSett->isAllyToFaction(m_Faction))
				priority *= 2.5f;
			else if (!targetSett->isEnemyToFaction(m_Faction))
				priority *= 1.25f;
			else
				priority /= 2.f;
		}
	}
	if (armyRes->army->faction->factionRecord->slave)
		priority *= 0.50f;
	const auto config = campaignAi::getLtgdConfig();
	switch(priType)
	{
	case priType_own:
		{
			if (!stratMapHelpers::getRegion(armyRes->army->gen->regionID)->hasAlliesToFaction(m_Faction->factionID, false))
				priority *= 0.66f;
			priority *= defenseFactor;
			break;
		}
	case priType_target:
		{
			if (!stratMapHelpers::getRegion(armyRes->army->gen->regionID)->hasAlliesToFaction(m_Faction->factionID, false))
				priority *= 0.50f;
			else
				priority += stratMapHelpers::getRegion(armyRes->army->gen->regionID)->devastatedTilesCount * 200;
			priority *= (config->getPriorityMod(m_Faction, armyRes->army->faction) * invadePriorityFactor);
			priority *= facData->getTargetFactionFactor(armyRes->army->faction);
			priority *= facData->getTargetReligionFactor(armyRes->army->faction->religion);
			priority *= aggressionFactor;
			break;
		}
	case priType_aid:
		{
			if (!stratMapHelpers::getRegion(armyRes->army->gen->regionID)->hasAlliesToFaction(m_Faction->factionID, false))
				priority *= 0.33f;
			priority *= facData->getAidFactionFactor(armyRes->army->faction);
			priority *= facData->getAidReligionFactor(armyRes->army->faction->religion);
			const auto fs = factionHelpers::getFactionStanding(m_Faction, armyRes->army->faction);
			priority *= (aidFactor * fs);
		}
		break;
	}
	return priority;
}

void aiOrder::setTiles(const int x, const int y)
{
	if (assignedArmies.empty())
		return;
	if (validTiles.empty())
	{
		auto neighbours = stratMapHelpers::getNeighbourTiles(x, y);
		while (true)
		{
			if (neighbours.empty())
				break;
			const auto [checkX, checkY] = neighbours.front();
			neighbours.pop();
			if (stratMapHelpers::isTileValidForCharacterType(7, checkX, checkY))
			{
				const auto tile = stratMapHelpers::getTile(checkX, checkY);
				const auto army = tile->getArmy();
				if (army && army->faction->isPlayerControlled == 1)
					playerInvolved = true;
				if ((army && army->faction->factionID == globalEopAiConfig::getCurrentFaction()->factionID) || !army)
				{
					validTiles.emplace_back(checkX, checkY);
					tileNums.emplace_back(army ? army->numOfUnits : 0);
				}
			}
		}
		if (validTiles.empty())
			return;
		const auto armySort = [](const std::shared_ptr<armyResource>& a, const std::shared_ptr<armyResource>& b)
		{
			return a->moveCost < b->moveCost;
		};
		std::stable_sort(assignedArmies.begin(), assignedArmies.end(), armySort);
		const auto armyPos = assignedArmies.front()->army->getCoords();
		const auto sortDistance = [&armyPos](const std::pair<int, int>a, const std::pair<int, int>b)
		{
			return stratMapHelpers::getDistanceInTiles(a.first, a.second, armyPos.first, armyPos.second) < stratMapHelpers::getDistanceInTiles(b.first, b.second, armyPos.first, armyPos.second);
		};
		std::sort(validTiles.begin(), validTiles.end(), sortDistance);
	}
}

void aiOrder::sortArmies()
{
	const auto armySort = [](const std::shared_ptr<armyResource>& a, const std::shared_ptr<armyResource>& b)
	{
		return a->army->totalStrength > b->army->totalStrength;
	};
	std::stable_sort(assignedArmies.begin(), assignedArmies.end(), armySort);
}

void attackSettlementOrder::sortAssigned()
{
	const auto armySort = [](const std::shared_ptr<armyResource>& a, const std::shared_ptr<armyResource>& b)
	{
		if (FLOAT_EQUAL(a->moveCost, b->moveCost))
			return a->army->totalStrength > b->army->totalStrength;
		return a->moveCost < b->moveCost;
	};
	std::stable_sort(assignedArmies.begin(), assignedArmies.end(), armySort);
}

void aiOrder::removeUsedResources()
{
	assignedArmies.erase(std::remove_if(assignedArmies.begin(), assignedArmies.end(), [](const std::shared_ptr<armyResource>& army)
	{
		return army->used;
	}), assignedArmies.end());
}

bool attackSettlementOrder::evaluate()
{
	removeUsedResources();
	if (assignedArmies.empty())
		return false;
	if (!targetSettlement->settlement)
		return false;
	const auto fac = globalEopAiConfig::getCurrentFaction();
	const auto attModifier = campaignHelpers::getCampaignDb()->campaignDbAi.siegeAttStrModifier;
	if (!targetSettlement->settlement->isEnemyToFaction(fac))
		return false;
	int totalStrength = 0;
	for (const auto& assignedArmy : assignedArmies)
	{
		if (assignedArmy->used || !assignedArmy->own)
			continue;
		//if (assignedArmy->army->gen->inEnemyZOC)
		//	continue;
		totalStrength += assignedArmy->army->totalStrength;
	}
	if (totalStrength * attModifier < targetSettlement->positionPower)
		return false;
	setTiles(targetSettlement->settlement->xCoord, targetSettlement->settlement->yCoord);
	if (validTiles.empty())
		return false;
	return true;
}

bool attackSettlementOrder::evaluateAttack()
{
	const auto settlement = targetSettlement->settlement;
	if (assignedArmies.front()->moveCost >= assignedArmies.front()->army->gen->movePointsArmy || assignedArmies.front()->turns > 1)
		return false;
	if (!settlement->army)
		return true;
	int power = settlement->army ? settlement->army->totalStrength : 0;
	const auto attModifier = campaignHelpers::getCampaignDb()->campaignDbAi.siegeAttStrModifier;
	const auto fac = globalEopAiConfig::getCurrentFaction();
	if (assignedArmies.size() == 1 && assignedArmies.front()->moveCost < assignedArmies.front()->army->gen->movePointsArmy && assignedArmies.front()->turns < 2)
	{
		return targetSettlement->positionPower < assignedArmies.front()->army->totalStrength * attModifier;
	}
	auto neighbours = stratMapHelpers::getNeighbourTiles(settlement->xCoord, settlement->yCoord);
	while (true)
	{
		if (neighbours.empty())
			break;
		const auto [checkX, checkY] = neighbours.front();
		neighbours.pop();
		const auto tile = stratMapHelpers::getTile(checkX, checkY);
		if (!tile)
			continue;
		const auto tileArmy = tile->getArmy(true);
		if (!tileArmy || tileArmy->isAdmiral)
			continue;
		if (tileArmy->isAllyToFaction(settlement->faction) && tileArmy->isEnemyToFaction(fac))
			power += static_cast<int>(tileArmy->totalStrength * attModifier);
		if (tileArmy->isEnemyToFaction(settlement->faction) && tileArmy->isAllyToFaction(fac))
			power -= tileArmy->totalStrength;
	}
	targetSettlement->positionPower = power;
	if (power < 1)
		return true;
	return false;
}

bool attackSettlementOrder::execute()
{
	gameHelpers::logStringGame("Try attack settlement order for faction: " + std::to_string(globalEopAiConfig::getCurrentFaction()->factionID));
	if (!evaluate())
		return false;
	sortAssigned();
	if (evaluateAttack())
	{
		bool hasAssaulted = false;
		for (const auto& assignedArmy : assignedArmies)
		{
			if (!assignedArmy->validate() || !targetSettlement->settlement
				|| !targetSettlement->settlement->isEnemyToFaction(assignedArmy->army->faction)
				|| assignedArmy->army->gen->visitingArmy
				|| !assignedArmy->army->canStartSiege(targetSettlement->settlement))
				continue;
			if (assignedArmy->army->siege && globalEopAiConfig::getInstance()->isEndTurn)
				continue;
			const std::string logString2 = "General: " + string(assignedArmy->army->gen->characterRecord->fullName);
			gameHelpers::logStringGame(logString2);
			const std::string logString = "Settlement: " + string(targetSettlement->settlement->name);
			gameHelpers::logStringGame(logString);
			if (targetSettlement->settlement->siegeNum > 0 && !assignedArmy->army->siege)
			{
				for (int i = 0; i < targetSettlement->settlement->siegeNum; i++)
				{
					const auto siege = targetSettlement->settlement->getSiege(i);
					if (siege->army->faction->factionID == assignedArmy->army->faction->factionID
						&& siege->army->canReceiveMerge(assignedArmy->army))
					{
						assignedArmy->army->gen->hasEopOrders = true;
						const auto [xCoord, yCoord] = siege->army->getCoords();
						if (assignedArmy->army->moveTactical(xCoord, yCoord, true))
						{
							if (const auto& armyRes = globalEopAiConfig::getInstance()->findArmyResource(siege->army))
								armyRes->unitCount += assignedArmy->army->numOfUnits;
						}
						assignedArmy->used = true;
						TURN_HAD_ACTION = true;
						hasAssaulted = true;
						break;
					}
				}
			}
			if (assignedArmy->used)
				continue;
			assignedArmy->used = true;
			TURN_HAD_ACTION = true;
			if (!hasAssaulted)
			{
				if (assignedArmy->army->canStartAssault(targetSettlement->settlement)
					&& assignedArmy->moveCost < assignedArmy->army->gen->movePointsArmy
					&& assignedArmy->turns < 2
					&& !PLAYER_ASSAULTED
					&& !globalEopAiConfig::getInstance()->isEndTurn)
				{
					gameHelpers::logStringGame("Assaulted");
					assignedArmy->army->gen->hasEopOrders = true;
					if (!targetSettlement->settlement->siegeNum && targetSettlement->settlement->army)
						targetSettlement->settlement->siegeNum = 1;
					assignedArmy->army->siegeSettlement(targetSettlement->settlement, true);
					if (playerInvolved || targetSettlement->settlement->isPlayerControlled())
						PLAYER_ASSAULTED = true;
					break;
				}
				if (
					(targetSettlement->settlement->siegeNum < 8 || assignedArmy->army->siege)
					&& ((assignedArmy->moveCost < assignedArmy->army->gen->movePointsArmy && assignedArmy->turns < 2)
						||
						(assignedArmy->army->isBorderingSettlement(targetSettlement->settlement)))
						)
				{
					gameHelpers::logStringGame("Maintained siege");
					assignedArmy->army->gen->hasEopOrders = true;
					assignedArmy->army->siegeSettlement(targetSettlement->settlement, false);
					hasAssaulted = true;
					assignedArmy->army->gen->movePointsArmy = 0;
				}
			}
			else if (assignedArmy->army->isBorderingSettlement(targetSettlement->settlement))
			{
				gameHelpers::logStringGame("Supported siege");
				assignedArmy->army->gen->hasEopOrders = true;
				assignedArmy->army->gen->movePointsArmy = 10;
			}
		}
		gameHelpers::logStringGame("Executed attack settlement order for faction: " + std::to_string(globalEopAiConfig::getCurrentFaction()->factionID));
	}
	else
	{
		int totalCommitted = 0;
		for (const auto& assignedArmy : assignedArmies)
		{
			int tileIndex = 0;
			const int unitNum = assignedArmy->army->numOfUnits;
			while(true)
			{
				if (tileIndex >= static_cast<int>(validTiles.size()))
					break;
				if (tileNums[tileIndex] + unitNum <= 20)
					break;
				tileIndex++;
			}
			if (!assignedArmy->validate() || tileIndex >= static_cast<int>(validTiles.size()))
				continue;
			assignedArmy->used = true;
			TURN_HAD_ACTION = true;
			tileNums[tileIndex] += assignedArmy->army->numOfUnits;
			totalCommitted += assignedArmy->army->totalStrength;
			if (assignedArmy->moveCost > assignedArmy->army->gen->movePointsArmy)
			{
				assignedArmy->army->gen->hasEopOrders = true;
				auto [newX, newY] = stratMapHelpers::findValidTileNearTile(validTiles[tileIndex].first, validTiles[tileIndex].second, 7);
				characterHelpers::moveNormal(assignedArmy->army->gen, newX, newY);
			}
			else
			{
				assignedArmy->army->gen->hasEopOrders = true;
				assignedArmy->army->moveTactical(validTiles[tileIndex].first, validTiles[tileIndex].second);
			}
			if ((totalCommitted >> 1) > targetSettlement->positionPower)
				break;
		}
		gameHelpers::logStringGame("Executed move to attack settlement order for faction: " + std::to_string(globalEopAiConfig::getCurrentFaction()->factionID));
		const std::string logString = "totalCommitted: " + std::to_string(totalCommitted);
		gameHelpers::logStringGame(logString);
	}
	return true;
}

std::string attackArmyOrder::toString()
{
	std::string logString = "Attack army order: ";
	logString += "\npriority: " + std::to_string(priority);
	logString += "\nfaction: " + factionHelpers::getLocalizedFactionName(globalEopAiConfig::getCurrentFaction());
	logString += "\ntarget faction: " + factionHelpers::getLocalizedFactionName(targetArmy->army->faction);
	logString += "\nposition power: " + std::to_string(targetArmy->positionPower);
	return logString;
}

std::string mergeArmiesOrder::toString()
{
	std::string logString = "Merge armies order: ";
	logString += "\npriority: " + std::to_string(priority);
	logString += "\nfaction: " + factionHelpers::getLocalizedFactionName(globalEopAiConfig::getCurrentFaction());
	logString += "\ntarget faction: " + factionHelpers::getLocalizedFactionName(targetRes->army->faction);
	return logString;
}

std::string defendArmyOrder::toString()
{
	std::string logString = "Defend army order: ";
	logString += "\npriority: " + std::to_string(priority);
	logString += "\nfaction: " + factionHelpers::getLocalizedFactionName(globalEopAiConfig::getCurrentFaction());
	logString += "\ntarget faction: " + factionHelpers::getLocalizedFactionName(targetArmy->army->faction);
	logString += "\nposition power: " + std::to_string(targetArmy->positionPower);
	return logString;
}

std::string attackSettlementOrder::toString()
{
	std::string logString = "Attack Settlement order: ";
	logString += "\nSettlement: " + string(targetSettlement->settlement->name);
	logString += "\npriority: " + std::to_string(priority);
	logString += "\nfaction: " + factionHelpers::getLocalizedFactionName(globalEopAiConfig::getCurrentFaction());
	logString += "\ntarget faction: " + factionHelpers::getLocalizedFactionName(targetSettlement->settlement->faction);
	logString += "\nposition power: " + std::to_string(targetSettlement->positionPower);
	return logString;
}


std::string defendSettlementOrder::toString()
{
	std::string logString = "Defend Settlement order: ";
	logString += "\nSettlement: " + string(targetSettlement->settlement->name);
	logString += "\npriority: " + std::to_string(priority);
	logString += "\nfaction: " + factionHelpers::getLocalizedFactionName(globalEopAiConfig::getCurrentFaction());
	logString += "\ntarget faction: " + factionHelpers::getLocalizedFactionName(targetSettlement->settlement->faction);
	logString += "\nposition power: " + std::to_string(targetSettlement->positionPower);
	return logString;
}

bool attackArmyOrder::evaluate()
{
	removeUsedResources();
	if (assignedArmies.empty())
		return false;
	const auto fac = globalEopAiConfig::getCurrentFaction();
	const auto attModifier = campaignHelpers::getCampaignDb()->campaignDbAi.attStrModifier;
	if (!targetArmy->army->isEnemyToFaction(fac))
		return false;
	int totalStrength = 0;
	for (const auto& assignedArmy : assignedArmies)
	{
		if (assignedArmy->used || !assignedArmy->own)
			continue;
		totalStrength += assignedArmy->army->totalStrength;
	}
	if (totalStrength * attModifier < targetArmy->positionPower)
		return false;
	setTiles(targetArmy->army->gen->xCoord, targetArmy->army->gen->yCoord);
	if (validTiles.empty())
		return false;
	return true;
}

bool attackArmyOrder::evaluateAttack()
{
	if (PLAYER_ASSAULTED)
		return false;
	const auto attModifier = campaignHelpers::getCampaignDb()->campaignDbAi.attStrModifier;
	if (assignedArmies.size() == 1)
	{
		return targetArmy->positionPower < assignedArmies.front()->army->totalStrength * attModifier;
	}
	int power = targetArmy->army->totalStrength;
	const auto fac = globalEopAiConfig::getCurrentFaction();
	auto neighbours = stratMapHelpers::getNeighbourTiles(targetArmy->army->gen->xCoord, targetArmy->army->gen->yCoord);
	while (true)
	{
		if (neighbours.empty())
			break;
		const auto [checkX, checkY] = neighbours.front();
		neighbours.pop();
		const auto tile = stratMapHelpers::getTile(checkX, checkY);
		if (!tile)
			continue;
		const auto tileArmy = tile->getArmy(true);
		if (!tileArmy || tileArmy->isAdmiral)
			continue;
		if (tileArmy->isAllyToFaction(targetArmy->army->faction) && tileArmy->isEnemyToFaction(fac))
			power += static_cast<int>(tileArmy->totalStrength * attModifier);
		if (tileArmy->isEnemyToFaction(targetArmy->army->faction) && tileArmy->isAllyToFaction(fac))
			power -= tileArmy->totalStrength;
	}
	targetArmy->positionPower = power;
	if (power < 1)
		return true;
	return false;
}

bool attackArmyOrder::execute()
{
	gameHelpers::logStringGame("Try attack army order for faction: " + std::to_string(globalEopAiConfig::getCurrentFaction()->factionID));
	if (!evaluate())
		return false;
	sortArmies();
	if (evaluateAttack())
	{
		for (const auto& assignedArmy : assignedArmies)
		{
			if (!assignedArmy->validate() || !targetArmy->army || !targetArmy->army->gen || targetArmy->army->gen->markedForDeath)
				continue;
			const std::string logString2 = "General: " + string(assignedArmy->army->gen->characterRecord->fullName);
			gameHelpers::logStringGame(logString2);
			if (assignedArmy->army->attackArmy(targetArmy->army))
			{
				if (playerInvolved || targetArmy->army->faction->isPlayerControlled == 1)
					PLAYER_ASSAULTED = true;
				assignedArmy->army->gen->hasEopOrders = true;
				assignedArmy->used = true;
				TURN_HAD_ACTION = true;
				break;
			}
		}
		gameHelpers::logStringGame("Executed attack army order for faction: " + std::to_string(globalEopAiConfig::getCurrentFaction()->factionID));
		gameHelpers::logStringGame("x: " + std::to_string(targetArmy->army->gen->xCoord) + " y: " + std::to_string(targetArmy->army->gen->yCoord));
	}
	else
	{
		int totalCommitted = 0;
		for (const auto& assignedArmy : assignedArmies)
		{
			int tileIndex = 0;
			const int unitNum = assignedArmy->army->numOfUnits;
			while(true)
			{
				if (tileIndex >= static_cast<int>(validTiles.size()))
					break;
				if (tileNums[tileIndex] + unitNum <= 20)
					break;
				tileIndex++;
			}
			if (!assignedArmy->validate() || tileIndex >= static_cast<int>(validTiles.size()))
				continue;
			assignedArmy->army->gen->hasEopOrders = true;
			assignedArmy->used = true;
			const std::string logString2 = "General: " + string(assignedArmy->army->gen->characterRecord->fullName);
			gameHelpers::logStringGame(logString2);
			TURN_HAD_ACTION = true;
			tileNums[tileIndex] += assignedArmy->army->numOfUnits;
			totalCommitted += assignedArmy->army->totalStrength;
			if (assignedArmy->moveCost > assignedArmy->army->gen->movePointsArmy)
			{
				auto [newX, newY] = stratMapHelpers::findValidTileNearTile(validTiles[tileIndex].first, validTiles[tileIndex].second, 7);
				characterHelpers::moveNormal(assignedArmy->army->gen, newX, newY);
			}
			else
				assignedArmy->army->moveTactical(validTiles[tileIndex].first, validTiles[tileIndex].second);
			if ((totalCommitted >> 1) > targetArmy->positionPower)
				break;
		}
		gameHelpers::logStringGame("Executed move to attack army order for faction: " + std::to_string(globalEopAiConfig::getCurrentFaction()->factionID));
		gameHelpers::logStringGame("x: " + std::to_string(targetArmy->army->gen->xCoord) + " y: " + std::to_string(targetArmy->army->gen->yCoord));
		const std::string logString = "totalCommitted: " + std::to_string(totalCommitted);
		gameHelpers::logStringGame(logString);
	}
	return true;
}

bool defendSettlementOrder::evaluate()
{
	removeUsedResources();
	if (assignedArmies.empty())
		return false;
	const auto fac = globalEopAiConfig::getCurrentFaction();
	if (!targetSettlement->settlement->isAllyToFaction(fac))
		return false;
	int totalStrength = 0;
	for (const auto& assignedArmy : assignedArmies)
	{
		if (assignedArmy->used || !assignedArmy->own)
			continue;
		totalStrength += assignedArmy->army->totalStrength;
	}
	if (totalStrength == 0)
		return false;
	return true;
}

bool defendSettlementOrder::execute()
{
	gameHelpers::logStringGame("Try defend settlement order for faction: " + std::to_string(globalEopAiConfig::getCurrentFaction()->factionID));
	if (!evaluate())
		return false;
	sortArmies();
	int totalCommitted = 0;
	for (const auto& assignedArmy : assignedArmies)
	{
		if (!assignedArmy->validate())
			continue;
		gameHelpers::logStringGame("character: " + string(assignedArmy->army->gen->characterRecord->fullName));
		assignedArmy->army->gen->hasEopOrders = true;
		assignedArmy->used = true;
		totalCommitted += assignedArmy->army->totalStrength;
		if (assignedArmy->moveCost > assignedArmy->army->gen->movePointsArmy || !targetSettlement->own || targetSettlement->settlement->siegeNum > 0)
		{
			const auto [newX, newY] = stratMapHelpers::findValidTileNearTile(static_cast<int>(targetSettlement->settlement->xCoord), static_cast<int>(targetSettlement->settlement->yCoord), 7);
			characterHelpers::moveNormal(assignedArmy->army->gen, newX, newY);
		}
		else
		{
			if (targetSettlement->settlement->army->canReceiveMerge(assignedArmy->army))
				assignedArmy->army->moveTactical(targetSettlement->settlement->xCoord, targetSettlement->settlement->yCoord, true);
			else
			{
				const auto [newX, newY] = stratMapHelpers::findValidTileNearTile(static_cast<int>(targetSettlement->settlement->xCoord), static_cast<int>(targetSettlement->settlement->yCoord), 7);
				assignedArmy->army->moveTactical(newX, newY);
			}
		}
		if ((totalCommitted >> 1) > targetSettlement->totalThreatReceiving)
			break;
	}
	gameHelpers::logStringGame("Executed defend settlement order for faction: " + std::to_string(globalEopAiConfig::getCurrentFaction()->factionID));
	const std::string logString = "totalCommitted: " + std::to_string(totalCommitted);
	gameHelpers::logStringGame(logString);
	gameHelpers::logStringGame("Settlement: " + string(targetSettlement->settlement->name));
	return true;
}

bool defendArmyOrder::evaluate()
{
	removeUsedResources();
	if (assignedArmies.empty())
		return false;
	if (targetArmy->used || !targetArmy->army)
		return false;
	const auto fac = globalEopAiConfig::getCurrentFaction();
	if (!targetArmy->army->isAllyToFaction(fac))
		return false;
	int totalStrength = 0;
	for (const auto& assignedArmy : assignedArmies)
	{
		if (assignedArmy->used || !assignedArmy->own)
			continue;
		totalStrength += assignedArmy->army->totalStrength;
	}
	if (totalStrength == 0)
		return false;
	return true;
}

bool defendArmyOrder::execute()
{
	gameHelpers::logStringGame("Try defend army order for faction: " + std::to_string(globalEopAiConfig::getCurrentFaction()->factionID));
	if (targetArmy->army && targetArmy->army->gen && targetArmy->army->gen->characterRecord)
	{
		const std::string logString = "target: " + string(targetArmy->army->gen->characterRecord->shortName);
		gameHelpers::logStringGame(logString);
	}
	else
		return false;
	if (!evaluate())
		return false;
	sortArmies();
	int totalCommitted = 0;
	for (const auto& assignedArmy : assignedArmies)
	{
		if (!assignedArmy->validate())
			continue;
		/*
		* 
		gameHelpers::logStringGame("character: " + string(assignedArmy->army->gen->characterRecord->fullName));
		gameHelpers::logStringGame("target character: " + string(targetArmy->army->gen->characterRecord->fullName));
		gameHelpers::logStringGame("startX: " + std::to_string(assignedArmy->army->gen->xCoord) + " startY: " + std::to_string(assignedArmy->army->gen->yCoord));
		gameHelpers::logStringGame("endX: " + std::to_string(targetArmy->army->gen->xCoord) + " endY: " + std::to_string(targetArmy->army->gen->yCoord));
		 */
		assignedArmy->army->gen->hasEopOrders = true;
		assignedArmy->used = true;
		totalCommitted += assignedArmy->army->totalStrength;
		if (assignedArmy->moveCost > assignedArmy->army->gen->movePointsArmy)
		{
			auto [newX, newY] = stratMapHelpers::findValidTileNearTile(targetArmy->army->gen->xCoord, targetArmy->army->gen->yCoord, 7);
			characterHelpers::moveNormal(assignedArmy->army->gen, newX, newY);
		}
		else
		{
			if (targetArmy->army->canReceiveMerge(assignedArmy->army) && targetArmy->unitCount + assignedArmy->unitCount <= 20)
			{
				targetArmy->unitCount += assignedArmy->army->numOfUnits;
				assignedArmy->army->moveTactical(targetArmy->army->gen->xCoord, targetArmy->army->gen->yCoord, true);
			}
			else
			{
				auto [newX, newY] = stratMapHelpers::findValidTileNearTile(targetArmy->army->gen->xCoord + 1, targetArmy->army->gen->yCoord + 1, 7);
				assignedArmy->army->moveTactical(newX, newY);
			}
		}
		if ((totalCommitted >> 1) > targetArmy->totalThreatReceiving)
			break;
	}
	gameHelpers::logStringGame("Executed defend army order for faction: " + std::to_string(globalEopAiConfig::getCurrentFaction()->factionID));
	const std::string logString = "totalCommitted: " + std::to_string(totalCommitted);
	gameHelpers::logStringGame(logString);
	return true;
}

bool mergeArmiesOrder::evaluate()
{
	removeUsedResources();
	if (assignedArmies.empty())
		return false;
	if (targetRes && targetRes->used)
		return false;
	if (assignedArmies.front()->army->numOfUnits + targetArmy->numOfUnits > 20)
		return false;
	return true;
}

bool mergeArmiesOrder::execute()
{
	gameHelpers::logStringGame("Try merge army order for faction: " + std::to_string(globalEopAiConfig::getCurrentFaction()->factionID));
	if (!evaluate())
		return false;
	const auto [xCoord, yCoord] = targetArmy->getCoords();
	assignedArmies.front()->army->moveTactical(xCoord, yCoord, true);
	assignedArmies.front()->used = true;
	assignedArmies.front()->army->gen->hasEopOrders = true;
	gameHelpers::logStringGame("Executed merge army order for faction: " + std::to_string(globalEopAiConfig::getCurrentFaction()->factionID));
	return true;
}

void globalEopAiConfig::checkRegion(int regionId)
{
	const auto region = stratMapHelpers::getRegion(regionId);
	m_CheckedRegions.emplace_back(regionId);
	for (int j = 0; j < region->stacksNum; j++)
	{
		const auto regionArmy = region->armies[j];
		if ( !regionArmy->gen || regionArmy->isAdmiral || regionArmy->settlement || !regionArmy->isEnemyToFaction(m_Faction) || !m_Faction->canSeeCharacter(regionArmy->gen))
			continue;
		auto regionArmyRes = findArmyResource(regionArmy);
		const auto moveData = regionArmy->gen->createMoveData(static_cast<int>(searchType::avoidZoc), 1);
		for (auto& nearSettlement : moveData->settlements)
		{
			if (nearSettlement.settlement->faction->factionID != m_Faction->factionID
				&& nearSettlement.settlement->faction->factionID != regionArmy->faction->factionID)
				continue;
			if (nearSettlement.turns == 0)
				nearSettlement.turns = 1;
			if (const auto settRes = findSettResource(nearSettlement.settlement); settRes)
			{
				settRes->nearResources.emplace_back(regionArmyRes);
				regionArmyRes->value += regionArmy->totalStrength - settRes->positionPower;
				if (nearSettlement.settlement->faction->factionID == m_Faction->factionID)
				{
					const auto threatGiving = regionArmy->totalStrength - settRes->positionPower;
					if (threatGiving > 0)
						regionArmyRes->totalThreatGiving += threatGiving / nearSettlement.turns;
					settRes->totalThreatReceiving += regionArmy->totalStrength / nearSettlement.turns;
				}
				else if (nearSettlement.settlement->faction->factionID == regionArmy->faction->factionID)
				{
					regionArmyRes->totalSupportGiving += regionArmy->totalStrength / nearSettlement.turns;
					settRes->totalSupportReceiving += regionArmy->totalStrength / nearSettlement.turns;
				}
				settRes->nearResources.back()->moveCost = nearSettlement.moveCost;
				settRes->nearResources.back()->turns = nearSettlement.turns;
			}
		}
		for (auto& nearArmy : moveData->armies)
		{
			if (nearArmy.army->faction->factionID != m_Faction->factionID
				&& nearArmy.army->faction->factionID != regionArmy->faction->factionID
				|| !nearArmy.army->gen
				|| !m_Faction->canSeeCharacter(nearArmy.army->gen)
				)
				continue;
			if (nearArmy.turns == 0)
				nearArmy.turns = 1;
			if (const auto armyRes = findArmyResource(nearArmy.army); armyRes)
			{
				armyRes->nearResources.emplace_back(regionArmyRes);
				regionArmyRes->value += regionArmy->totalStrength - armyRes->positionPower;
				if (nearArmy.army->faction->factionID == m_Faction->factionID)
				{
					const auto threatGiving = regionArmy->totalStrength - armyRes->positionPower;
					if (threatGiving > 0)
						regionArmyRes->totalThreatGiving += threatGiving / nearArmy.turns;
					armyRes->totalThreatReceiving += regionArmy->totalStrength / nearArmy.turns;
				}
				else if (nearArmy.army->faction->factionID == regionArmy->faction->factionID)
				{
					regionArmyRes->totalSupportGiving += regionArmy->totalStrength / nearArmy.turns;
					armyRes->totalSupportReceiving += regionArmy->totalStrength / nearArmy.turns;
				}
				armyRes->nearResources.back()->moveCost = nearArmy.moveCost;
				armyRes->nearResources.back()->turns = nearArmy.turns;
			}
		}
	}
}

void globalEopAiConfig::characterTurnStart(character* currentChar)
{
	m_Faction = currentChar->characterRecord->faction;
	if (!m_Faction || m_Faction->factionRecord->slave || m_Faction->isHorde || !currentChar->army || !currentChar->isGeneral() || currentChar->markedForDeath)
		return; 
	const auto army = currentChar->army;
	if (army->numOfUnits > 20)
	{
		gameHelpers::logStringGame("Character: " + string(currentChar->characterRecord->fullName) + " has more than 20 units!");
		MessageBoxA(nullptr, "Army has more than 20 units!", "Error", MB_OK);
		const auto [newX, newY] = stratMapHelpers::findValidTileNearTile(army->getX() + 1, army->getY() + 1, 7);
		sol::table unitTable = plugData::data.luaAll.luaState.create_table();
		for (int i = army->numOfUnits - 1; i > 19; i--)
		{
			const auto unit = army->getUnit(i);
			unitTable.add(unit);
		}
		factionHelpers::splitArmy(currentChar->characterRecord->faction, unitTable, newX, newY);
	}
	if (army->siege)
		return;
	bool used = false;
	const auto moveData = currentChar->createMoveData(static_cast<int>(searchType::avoidZoc), 1);
	for (const auto& nearSett : moveData->settlements)
	{
		if (!nearSett.settlement->faction)
			continue;
		if (nearSett.settlement->faction->factionID == m_Faction->factionID)
		{
			if ((!nearSett.settlement->army || nearSett.settlement->army->numOfUnits == 0) && nearSett.settlement->siegeNum == 0)
			{
				armyHelpers::createArmyInSettlement(nearSett.settlement);
				if (army->moveTactical(nearSett.settlement->xCoord, nearSett.settlement->yCoord, true))
					gameHelpers::logStringGame("Character: " + string(currentChar->characterRecord->fullName) + " Garrisoned empty settlement!");
				used = true;
				break;
			}
		}
	}
	if (used)
		return;
	for (const auto& nearArmy : moveData->armies)
	{
		if (!nearArmy.army->gen
			|| nearArmy.army->isAdmiral
			|| nearArmy.army->faction->factionID != m_Faction->factionID
			|| nearArmy.army->gen->markedForDeath)
			continue;
		if (nearArmy.moveCost < 25.f && nearArmy.army->canReceiveMerge(army))
		{
			const auto [xCoord, yCoord] = nearArmy.army->getCoords();
			if (xCoord == -1)
				continue;
			if (army->moveTactical(xCoord, yCoord, true))
				gameHelpers::logStringGame("Character: " + string(currentChar->characterRecord->fullName) + " merged armies!");
			break;
		}
	}
}

std::shared_ptr<armyResource> globalEopAiConfig::findArmyResource(armyStruct* army)
{
	if (!army->gen)
		return nullptr;
	if (army->faction->factionID == m_Faction->factionID)
	{
		for (auto& armyRes : m_Armies)
		{
			if (armyRes->army == army)
				return armyRes;
		}
		m_Armies.emplace_back(make_shared<armyResource>(army));
		m_Armies.back()->own = true;
		m_Armies.back()->enemy = false;
		m_Armies.back()->calculatePositionPower();
		return m_Armies.back();
	}
	if (army->isEnemyToFaction(m_Faction))
	{
		for (auto& armyRes : m_TargetArmies)
		{
			if (armyRes->army == army)
				return armyRes;
		}
		m_TargetArmies.emplace_back(make_shared<armyResource>(army));
		m_TargetArmies.back()->own = false;
		m_TargetArmies.back()->enemy = true;
		m_TargetArmies.back()->calculatePositionPower();
		return m_TargetArmies.back();
	}
	if (army->isAllyToFaction(m_Faction))
	{
		for (auto& armyRes : m_AllyArmies)
		{
			if (armyRes->army == army)
				return armyRes;
		}
		m_AllyArmies.emplace_back(make_shared<armyResource>(army));
		m_AllyArmies.back()->own = false;
		m_AllyArmies.back()->enemy = false;
		m_AllyArmies.back()->calculatePositionPower();
		return m_AllyArmies.back();
	}
	return nullptr;
}

std::shared_ptr<settlementResource> globalEopAiConfig::findSettResource(settlementStruct* sett)
{
	if (sett->faction->factionID == m_Faction->factionID)
	{
		for (auto& settRes : m_Settlements)
			{
			if (settRes->settlement->regionID == sett->regionID && settRes->settlement->minorSettlementIndex == sett->minorSettlementIndex)
				return settRes;
		}
		m_Settlements.emplace_back(make_shared<settlementResource>(sett));
		m_Settlements.back()->own = true;
		m_Settlements.back()->enemy = false;
		m_Settlements.back()->calculatePositionPower();
		return m_Settlements.back();
	}
	if (sett->isEnemyToFaction(m_Faction))
	{
		for (auto& settRes : m_TargetSettlements)
		{
			if (settRes->settlement->regionID == sett->regionID && settRes->settlement->minorSettlementIndex == sett->minorSettlementIndex)
				return settRes;
		}
		m_TargetSettlements.emplace_back(make_shared<settlementResource>(sett));
		m_TargetSettlements.back()->own = false;
		m_TargetSettlements.back()->enemy = true;
		m_TargetSettlements.back()->calculatePositionPower();
		return m_TargetSettlements.back();
	}
	if (sett->isAllyToFaction(m_Faction))
	{
		for (auto& settRes : m_AllySettlements)
		{
			if (settRes->settlement->regionID == sett->regionID && settRes->settlement->minorSettlementIndex == sett->minorSettlementIndex)
				return settRes;
		}
		m_AllySettlements.emplace_back(make_shared<settlementResource>(sett));
		m_AllySettlements.back()->own = false;
		m_AllySettlements.back()->enemy = false;
		m_AllySettlements.back()->calculatePositionPower();
		return m_AllySettlements.back();
	}
	return nullptr;
}

const auto C_ORDER_SORT = [](const std::shared_ptr<aiOrder>& a, const std::shared_ptr<aiOrder>& b)
{
	return a->priority > b->priority;
};
const auto threatSort = [](const std::shared_ptr<settlementResource>& a, const std::shared_ptr<settlementResource>& b)
{
	return (a->totalThreatReceiving - a->positionPower) > (b->totalThreatReceiving- b->positionPower);
};
const auto threatSort2 = [](const std::shared_ptr<armyResource>& a, const std::shared_ptr<armyResource>& b)
{
	return (a->totalThreatReceiving - a->positionPower) > (b->totalThreatReceiving- b->positionPower);
};

void globalEopAiConfig::assignOrders(factionStruct* fac)
{
	const auto campaignData = campaignHelpers::getCampaignData();
	for (const auto& settRes : m_Settlements)
	{
		const float priority = calculateSettPriority(settRes, priType_own);
		if (priority < 1.f)
			continue;
		m_Orders.emplace_back(make_shared<defendSettlementOrder>(settRes));
		m_Orders.back()->priority = priority;
		for (const auto& nearArmy : settRes->nearResources)
		{
			if (!nearArmy || !nearArmy->own || nearArmy->used || !nearArmy->army || nearArmy->army->siege)
				continue;
			m_Orders.back()->assignedArmies.emplace_back(nearArmy);
			m_Orders.back()->priority -= nearArmy->moveCost * (moveCostFactor * nearArmy->turns);
		}
		if (m_Orders.back()->priority < 0)
			m_Orders.erase(m_Orders.end() - 1);
	}
	for (const auto& armyRes : m_Armies)
	{
		const float priority = calculateArmyPriority(armyRes, priType_own);
		if (priority < 1.f)
			continue;
		m_Orders.emplace_back(make_shared<defendArmyOrder>(armyRes));
		m_Orders.back()->priority = priority;
		for (const auto& army : armyRes->nearResources)
		{
			if (!army->own || army->used || !army->army || army->army->siege)
				continue;
			m_Orders.back()->assignedArmies.emplace_back(army);
			m_Orders.back()->priority -= army->moveCost * (moveCostFactor * army->turns);
		}
		if (m_Orders.back()->priority < 0)
			m_Orders.erase(m_Orders.end() - 1);
	}
	for (const auto& targetSett : m_TargetSettlements)
	{
		if (targetSett->settlement->siegeNum > 0 && !targetSett->settlement->getSiege(0)->army->isAllyToFaction(m_Faction))
			continue;
		const float priority = calculateSettPriority(targetSett, priType_target);
		if (priority < 1.f)
			continue;
		m_Orders.emplace_back(make_shared<attackSettlementOrder>(targetSett));
		m_Orders.back()->priority = priority;
		for (const auto& army : targetSett->nearResources)
		{
			if (!army->own || army->used || !army->army)
				continue;
			if (army->army->siege)
			{
				if (army->army->siege->getSiegedSettlement() != targetSett->settlement)
					continue;
			}
			m_Orders.back()->assignedArmies.emplace_back(army);
			m_Orders.back()->priority -= (army->moveCost * (moveCostFactor * army->turns));
		}
		if (m_Orders.back()->priority < 0)
			m_Orders.erase(m_Orders.end() - 1);
	}
	for (const auto& targetArmy : m_TargetArmies)
	{
		const float priority = calculateArmyPriority(targetArmy, priType_target);
		if (priority < 1.f)
			continue;
		m_Orders.emplace_back(make_shared<defendArmyOrder>(targetArmy));
		m_Orders.back()->priority = priority;
		for (const auto& army : targetArmy->nearResources)
		{
			if (!army->own || army->used || !army->army || army->army->siege)
				continue;
			m_Orders.back()->assignedArmies.emplace_back(army);
			m_Orders.back()->priority -= army->moveCost * (moveCostFactor * army->turns);
		}
		if (m_Orders.back()->priority < 0)
			m_Orders.erase(m_Orders.end() - 1);
	}
	for (const auto& settRes : m_AllySettlements)
	{
		const float priority = calculateSettPriority(settRes, priType_aid);
		if (priority < 1.f)
			continue;
		m_Orders.emplace_back(make_shared<defendSettlementOrder>(settRes));
		m_Orders.back()->priority = priority;
		for (const auto& army : settRes->nearResources)
		{
			if (!army->own || army->used || !army->army || army->army->siege)
				continue;
			m_Orders.back()->assignedArmies.emplace_back(army);
			m_Orders.back()->priority -= army->moveCost * (moveCostFactor * army->turns);
		}
		if (m_Orders.back()->priority < 0)
			m_Orders.erase(m_Orders.end() - 1);
	}
	for (const auto& armyRes : m_AllyArmies)
	{
		const float priority = calculateArmyPriority(armyRes, priType_aid);
		if (priority < 1.f)
			continue;
		m_Orders.emplace_back(make_shared<defendArmyOrder>(armyRes));
		m_Orders.back()->priority = priority;
		for (const auto& army : armyRes->nearResources)
		{
			if (!army->own || army->used || !army->army || army->army->siege)
				continue;
			m_Orders.back()->assignedArmies.emplace_back(army);
			m_Orders.back()->priority -= army->moveCost * (moveCostFactor * army->turns);
		}
		if (m_Orders.back()->priority < 0)
			m_Orders.erase(m_Orders.end() - 1);
	}
	std::stable_sort(m_Orders.begin(), m_Orders.end(), C_ORDER_SORT);
	for (const auto& order : m_Orders)
	{
		if (order->executed || order->priority < 1)
			continue;
		order->execute();
		order->executed = true;
		campaignData->speedUp = true;
		campaignData->ignoreSpeedUp = false;
		campaignData->followMovement = false;
		if (enableLogging)
			gameHelpers::logStringGame(order->toString());
	}
	m_Armies.erase(std::remove_if(m_Armies.begin(), m_Armies.end(), [](const std::shared_ptr<armyResource> army)
	{
		return army->used || army->army->siege;
	}), m_Armies.end());
}

float aiFactionData::getTargetFactionFactor(const factionStruct* faction)
{
	return targetFactionFactors.at(faction->factionID);
}

float aiFactionData::getAidFactionFactor(const factionStruct* faction)
{
	return aidFactionFactors.at(faction->factionID);
}

float aiFactionData::getSettlementFactor(const settlementStruct* settlement)
{
	for (const auto& [indexes, value] : settlementFactors)
	{
		if (const auto [regionId, settIndex] = indexes; regionId == settlement->regionID && settIndex == settlement->minorSettlementIndex)
			return value;
	}
	return 1.f;
}

float aiFactionData::getTargetReligionFactor(const int religion)
{
	if (const auto it = targetReligionFactors.find(religion); it != targetReligionFactors.end())
		return it->second;
	return 1.f;
}

float aiFactionData::getAidReligionFactor(const int religion)
{
	if (const auto it = aidReligionFactors.find(religion); it != aidReligionFactors.end())
		return it->second;
	return 1.f;
}

void aiFactionData::setTargetFactionFactor(const factionStruct* faction, const float factor)
{
	targetFactionFactors[faction->factionID] = factor;
}

void aiFactionData::setAidFactionFactor(const factionStruct* faction, const float factor)
{
	aidFactionFactors[faction->factionID] = factor;
}

void aiFactionData::setSettlementFactor(const settlementStruct* settlement, float factor)
{
	if (!settlement)
	{
		gameHelpers::logStringGame("Settlement is null!");
		return;
	}
	for (auto& [indexes, value] : settlementFactors)
	{
		if (const auto [regionId, settIndex] = indexes; regionId == settlement->regionID && settIndex == settlement->minorSettlementIndex)
		{
			value = factor;
			return;
		}
	}
	settlementFactors.emplace_back(std::make_pair(settlement->regionID, settlement->minorSettlementIndex), factor);
}

void aiFactionData::setTargetReligionFactor(const int religion, float factor)
{
	targetReligionFactors.insert_or_assign(religion, factor);
}

void aiFactionData::setAidReligionFactor(const int religion, float factor)
{
	aidReligionFactors.insert_or_assign(religion, factor);
}

globalEopAiConfig::globalEopAiConfig()
{
	m_FactionData.fill(std::make_shared<aiFactionData>());
}

aiFactionData* globalEopAiConfig::getFactionDataLua(const factionStruct* fac)
{
	if (!fac)
		return nullptr;
	const auto instance = getInstance();
	if (!instance)
		return nullptr;
	const auto factionData = instance->m_FactionData.at(fac->factionID);
	if (!factionData)
		return nullptr;
	return factionData.get();
}

float distanceRaw(int x1, int y1, int x2, int y2)
{
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

float distance(const character* charOne, const character* charTwo)
{
	return distanceRaw(charOne->xCoord, charOne->yCoord, charTwo->xCoord, charTwo->yCoord);
}

void globalEopAiConfig::turnStartMove(factionStruct* fac, const bool isEnd)
{
	if (!enabled || fac->isPlayerControlled > 0 || fac->factionRecord->slave || fac->isHorde)
		return;
	const auto campaignData = campaignHelpers::getCampaignData();
	campaignData->speedUp = true;
	m_Faction = fac;
	m_CurrentFacData = getFactionData(m_Faction);
	TURN_HAD_ACTION = false;
	PLAYER_ASSAULTED = false;
	isEndTurn = isEnd;
	getData(fac);
	assignOrders(fac);
	campaignData->speedUp = true;
	if (isEnd)
		return;
	const auto strengthSort = [](const std::shared_ptr<armyResource>& a, const std::shared_ptr<armyResource>& b)
	{
		return a->army->totalStrength < b->army->totalStrength;
	};
	std::stable_sort(m_Armies.begin(), m_Armies.end(), strengthSort);
	for (auto& armyRes : m_Armies)
	{
		if (!armyRes->validate() || armyRes->army->siege)
			continue;
		const auto distanceSort = [armyRes](const std::shared_ptr<armyResource>& a, const std::shared_ptr<armyResource>& b)
		{
			return distance(armyRes->army->gen, a->army->gen) < distance(armyRes->army->gen, b->army->gen);
		};
		const auto distanceSort2 = [armyRes](const std::shared_ptr<settlementResource>& a, const std::shared_ptr<settlementResource>& b)
		{
			return distanceRaw(armyRes->army->gen->xCoord, armyRes->army->gen->yCoord, a->settlement->xCoord, a->settlement->yCoord)
				< distanceRaw(armyRes->army->gen->xCoord, armyRes->army->gen->yCoord, b->settlement->xCoord, b->settlement->yCoord);
		};
		std::stable_sort(armyRes->nearResources.begin(), armyRes->nearResources.end(), distanceSort);
		for (const auto& nearRes : armyRes->nearResources)
		{
			if (nearRes->used || nearRes->army->isAdmiral || nearRes->army->faction->factionID != m_Faction->factionID || nearRes->unitCount >= 20)
				continue;
			if (nearRes->army->canReceiveMerge(armyRes->army) && nearRes->unitCount + armyRes->unitCount <= 20)
			{
				const auto [xCoord, yCoord] = nearRes->army->getCoords();
				if (xCoord == -1)
					continue;
				string charName;
				if (armyRes->army->gen->characterRecord->shortName)
					charName = armyRes->army->gen->characterRecord->shortName;
				if (armyRes->army->moveTactical(xCoord, yCoord, true))
					gameHelpers::logStringGame("Character: " + charName + " merged armies!");
				armyRes->used = true;
				nearRes->used = true;
				break;
			}
		}
		if (!armyRes->used && armyRes->totalThreatReceiving > 0)
		{
			armyRes->calculatePositionPower();
			if (armyRes->totalThreatReceiving > armyRes->positionPower && !armyRes->nearSetts.empty())
			{
				std::stable_sort(armyRes->nearSetts.begin(), armyRes->nearSetts.end(), distanceSort2);
				string charName;
				if (armyRes->army->gen->characterRecord->shortName)
					charName = armyRes->army->gen->characterRecord->shortName;
				if (armyRes->army->moveTactical(armyRes->nearSetts.front()->settlement->xCoord, armyRes->nearSetts.front()->settlement->yCoord, true))
					gameHelpers::logStringGame("Character: " + charName + " moved to safety!");
			}
		}
	}
	if (!isEnd)
	{
		if (TURN_HAD_ACTION)
		{
			if (m_CurrentFacData->noActionTurns > 0)
				m_CurrentFacData->noActionTurns--;
		}
		else if (m_CurrentFacData->noActionTurns < maxTurnSearchCount)
			m_CurrentFacData->noActionTurns++;
	}
	campaignData->speedUp = true;
}
	

void globalEopAiConfig::turnEndMerge(factionStruct* fac)
{
	if (!enabled || fac->isPlayerControlled > 0 || fac->factionRecord->slave)
		return;
	m_Faction = fac;
	m_CurrentFacData = getFactionData(m_Faction);
	getData(fac);
	const auto strengthSort = [](const std::shared_ptr<armyResource>& a, const std::shared_ptr<armyResource>& b)
	{
		return a->army->totalStrength < b->army->totalStrength;
	};
	std::stable_sort(m_Armies.begin(), m_Armies.end(), strengthSort);
	for (auto& armyRes : m_Armies)
	{
		if (armyRes->used || armyRes->army->siege || !armyRes->army|| !armyRes->army->gen)
			continue;
		const auto distanceSort = [armyRes](const std::shared_ptr<armyResource>& a, const std::shared_ptr<armyResource>& b)
		{
			return distance(armyRes->army->gen, a->army->gen) < distance(armyRes->army->gen, b->army->gen);
		};
		std::stable_sort(armyRes->nearResources.begin(), armyRes->nearResources.end(), distanceSort);
		for (const auto& nearRes : armyRes->nearResources)
		{
			if (nearRes->used || !nearRes->own || nearRes->army->isAdmiral || nearRes->army->faction->factionID != m_Faction->factionID)
				continue;
			if (nearRes->army->numOfUnits + armyRes->army->numOfUnits <= 20 && nearRes->unitCount + armyRes->unitCount <= 20)
			{
				const auto [xCoord, yCoord] = nearRes->army->getCoords();
				if (xCoord == -1)
					continue;
				nearRes->unitCount += armyRes->unitCount;
				armyRes->army->moveTactical(xCoord, yCoord, false);
				armyRes->used = true;
				break;
			}
		}
	}
}

void globalEopAiConfig::getData(factionStruct* fac)
{
	clearData();
	for (int i = 0; i < fac->settlementsNum; i++)
	{
		const auto settlement = fac->settlements[i];
		const auto res = findSettResource(settlement);
		if (!res || res->searched)
			continue;
		res->searched = true;
		if (m_SearchedRegions.end() != std::find(m_SearchedRegions.begin(), m_SearchedRegions.end(), settlement->regionID))
			continue;
		m_SearchedRegions.emplace_back(settlement->regionID);
		const auto region = stratMapHelpers::getRegion(settlement->regionID);
		if (m_CheckedRegions.end() == std::find(m_CheckedRegions.begin(), m_CheckedRegions.end(), settlement->regionID))
			checkRegion(settlement->regionID);
		for (int j = 0; j < region->neighbourRegionsNum; j++)
		{
			const auto nRegion = region->neighbourRegions[j];
			if (nRegion.region->isSea || nRegion.isBlocked)
				continue;
			if (m_CheckedRegions.end() == std::find(m_CheckedRegions.begin(), m_CheckedRegions.end(), nRegion.region->regionID))
				checkRegion(nRegion.region->regionID);
		}
	}
	for (int i = 0; i < fac->numOfCharacters; i++)
	{
		const auto currentChar = fac->characters[i];
		currentChar->hasEopOrders = false;
		if (currentChar->getTypeID() != characterTypeStrat::general
			&& currentChar->getTypeID() != characterTypeStrat::namedCharacter)
			continue;
		if (!currentChar->army)
			continue;
		const auto res = findArmyResource(currentChar->army);
		if (!res || res->searched || !res->army || !res->army->gen || res->used || res->army->gen->currentCrusade)
			continue;
		//gameHelpers::logStringGame("Found army resource: " + std::string(res->army->gen->characterRecord->fullName));
		res->searched = true;
		if (m_SearchedRegions.end() == std::find(m_SearchedRegions.begin(), m_SearchedRegions.end(), currentChar->regionID))
		{
			m_SearchedRegions.emplace_back(currentChar->regionID);
			const auto region = stratMapHelpers::getRegion(currentChar->regionID);
			if (m_CheckedRegions.end() == std::find(m_CheckedRegions.begin(), m_CheckedRegions.end(), currentChar->regionID))
				checkRegion(currentChar->regionID);
			for (int j = 0; j < region->neighbourRegionsNum; j++)
			{
				const auto nRegion = region->neighbourRegions[j];
				if (nRegion.region->isSea || nRegion.isBlocked)
					continue;
				if (m_CheckedRegions.end() == std::find(m_CheckedRegions.begin(), m_CheckedRegions.end(), nRegion.region->regionID))
					checkRegion(nRegion.region->regionID);
			}
		}
		const auto moveData = currentChar->createMoveData(static_cast<int>(searchType::avoidZoc), 1 + (isEndTurn ? 0 : m_CurrentFacData->noActionTurns));
		for (auto& nearSettlement : moveData->settlements)
		{
			if (auto settRes = findSettResource(nearSettlement.settlement); settRes)
			{
				settRes->nearResources.emplace_back(res);
				res->value += res->army->totalStrength - settRes->positionPower;
				settRes->nearResources.back()->moveCost = nearSettlement.moveCost;
				if (nearSettlement.turns == 0)
					nearSettlement.turns = 1;
				settRes->nearResources.back()->turns = nearSettlement.turns;
				if (nearSettlement.settlement->isEnemyToFaction(m_Faction))
				{
					const auto threatGiving = res->army->totalStrength - settRes->positionPower;
					if (threatGiving > 0)
						res->totalThreatGiving += threatGiving / nearSettlement.turns;
					settRes->totalThreatReceiving += res->army->totalStrength;
				}
				else
				{
					res->totalSupportGiving += res->army->totalStrength / nearSettlement.turns;
					settRes->totalSupportReceiving += res->army->totalStrength / nearSettlement.turns;
					res->nearSetts.emplace_back(settRes);
				}
			}
		}
		for (auto& nearArmy : moveData->armies)
		{
			if (nearArmy.army->isAdmiral || !nearArmy.army->gen || nearArmy.army->gen->getTypeID() == characterTypeStrat::admiral || !m_Faction->canSeeCharacter(nearArmy.army->gen))
				continue;
			if (nearArmy.turns == 0)
				nearArmy.turns = 1;
			if (const auto armyRes = findArmyResource(nearArmy.army); armyRes)
			{
				armyRes->nearResources.emplace_back(res);
				if (nearArmy.army->isEnemyToFaction(m_Faction))
				{
					const auto threatGiving = res->army->totalStrength - armyRes->positionPower;
					if (threatGiving > 0)
						res->totalThreatGiving += threatGiving / nearArmy.turns;
					armyRes->totalThreatReceiving += res->army->totalStrength;
				}
				else
				{
					res->totalSupportGiving += res->army->totalStrength / nearArmy.turns;
					armyRes->totalSupportReceiving += res->army->totalStrength / nearArmy.turns;
				}
				armyRes->nearResources.back()->moveCost = nearArmy.moveCost;
				armyRes->nearResources.back()->turns = nearArmy.turns;
			}
		}
	}

	//const std::string logString = "End of turn move for faction: " + std::string(fac->factionRecord->facName);
	//gameHelpers::logStringGame(logString);
}

/*--------------------------------------------------------------------------------------------------------------------*\
									   Campaign AI Helpers
\*--------------------------------------------------------------------------------------------------------------------*/
#pragma region Campaign AI Helpers

void sortCampaigns(const campaignControllerArray* array)
{
	auto comparison = [](const aiCampaignController* a, const aiCampaignController* b)
	{
		if (!a->regionData || !b->regionData)
			return false;
		return a->regionData->priority > b->regionData->priority;
	};
	std::stable_sort(array->campaigns, array->campaigns + array->count, comparison);
}

void clearRegionData(aiRegionData** regionList)
{
	GAME_FUNC(void(__thiscall*)(aiRegionData**), clearAiRegionData)(regionList);
}

aiNavalRegion* addToNavalTargets(aiNavalRegion** list)
{
	return GAME_FUNC(aiNavalRegion*(__thiscall*)(aiNavalRegion**), addNavalTargetRegion)(list);
}

void removeNavalTarget(aiNavalRegion** list, const int index)
{
	GAME_FUNC(void(__thiscall*)(aiNavalRegion**, int), removeNavalTargetRegion)(list, index);
}

int calculateBorderChange(const aiGlobalStrategyDirector* director, aiRegionData* regionData)
{
	if (!regionData)
		return 0;
	int border = 0;
	const auto region = stratMapHelpers::getRegion(regionData->regionID);
	const auto settlement = regionData->getSettlement();
	if (!settlement)
		return 0;
	const auto directorFaction = director->aiFaction->faction;
	const int regionSettCount = region->settlementCount();
	for (int i = 0; i < regionSettCount; i++)
	{
		const auto sett = region->getSettlement(i);
		if (sett->isEnemyToFaction(directorFaction))
			border -= 40;
		if (sett->isAllyToFaction(directorFaction))
			border += 30;
		else
			border -= 20;
	}
	for (int i = 0; i < region->neighbourRegionsNum; i++)
	{
		const auto nRegion = region->neighbourRegions[i];
		const int borderTiles = nRegion.borderTilesCount;
		if (nRegion.isBlocked || nRegion.region->isSea || !nRegion.region->settlement)
			continue;
		const int nSettCount = nRegion.region->settlementCount();
		for (int j = 0; j < nSettCount; j++)
		{
			const auto sett = nRegion.region->getSettlement(j);
			if (sett->isEnemyToFaction(directorFaction))
				border -= borderTiles / nSettCount;
			if (sett->isAllyToFaction(directorFaction))
				border += borderTiles / nSettCount;
			else
				border -= (borderTiles / nSettCount) / 2;
		}
	}
	if (border < 0)
		return 0;
	return border;
}

int calculateRegionValue(const aiGlobalStrategyDirector* director, aiRegionData* regionData)
{
	if (!regionData)
		return 0;
	const auto settlement = regionData->getSettlement();
	if (!settlement)
		return 0;
	const auto region = stratMapHelpers::getRegion(regionData->regionID);
	int value = calculateBorderChange(director, regionData);
	value += settlement->stats.settlementStats.TotalIncomeWithoutAdmin / 10;
	const int resourceValue = region->getResourcesValue() * 2;
	if (!settlement->isMinorSettlement || region->factionOwner == director->faction)
		value += resourceValue;
	else if(campaignHelpers::getCampaignData()->getFactionDiplomacy(director->faction->factionID, region->factionOwner->factionID)->hasTradeRights)
		value += (resourceValue >> 1);
	value += GAME_FUNC(int(__cdecl*)(int), getRegionIsolationScore)(region->regionID);
	value += settlement->getSettlementValue();
	if (value < 0)
		return 1;
	if (settlement->isEnemyToFaction(director->faction))
	{
		const auto invadePriority = campaignAi::getLtgdConfig()->getPriorityMod(director->faction, settlement->faction);
		value = static_cast<int>(value * (invadePriority + 0.25f));
		if (!settlement->army)
			value *= 2;
	}
	if (globalEopAiConfig::getInstance()->enabled)
	{
		const auto facData = globalEopAiConfig::getInstance()->getFactionDataLua(director->faction);
		value = static_cast<int>(roundf(value * facData->getSettlementFactor(settlement)));
	}
	return value;
}

aiRegionData* createRegionInArray(aiGlobalStrategyDirector* director, settlementStruct* sett, aiRegionData** regionList)
{
	const auto regionData = GAME_FUNC(aiRegionData*(__thiscall*)(aiRegionData**), createRegionInArray)(regionList);
	const auto region = stratMapHelpers::getRegion(sett->regionID);
	regionData->navalTarget = false;
	regionData->isLastResort = false;
	regionData->regionController = nullptr;
	regionData->factionID = director->faction->factionID;
	regionData->regionID = sett->regionID;
	regionData->regionRisk = 0;
	regionData->settlementIndex = sett->getMinorSettlementIndex();
	regionData->regionValue = calculateRegionValue(director, regionData);
	regionData->calculateRegionStrengths();
	int enemyNum = 0;
	int neutralNum = 0;
	const int settCount = region->settlementCount();
	for (int i = 0; i < settCount; i++)
	{
		const auto settlement = region->getSettlement(i);
		if (!settlement || settlement->faction->factionID == director->faction->factionID)
			continue;
		if (settlement->isEnemyToFaction(director->faction))
			enemyNum++;
		else if (!settlement->isAllyToFaction(director->faction))
			neutralNum++;
	}
	for (int i = 0; i < region->neighbourRegionsNum; i++)
	{
		const auto nRegion = region->neighbourRegions[i];
		if (nRegion.isBlocked || nRegion.region->isSea || !nRegion.region->settlement)
			continue;
		const int settCount2 = nRegion.region->settlementCount();
		for (int j = 0; j < settCount2; j++)
		{
			const auto settlement = nRegion.region->getSettlement(j);
			if (!settlement || settlement->faction->factionID == director->faction->factionID)
				continue;
			if (settlement->isEnemyToFaction(director->faction))
				enemyNum++;
			else if (!settlement->isAllyToFaction(director->faction))
				neutralNum++;
		}
	}
	regionData->neighbourEnemyNum = enemyNum;
	regionData->neighbourOtherNum = neutralNum;
	return regionData;
}

aiRegionController* createNewRegionController(aiFaction* aiFac, settlementStruct* sett)
{
	const auto controller = techFuncs::createGameClass<aiRegionController>();
	GAME_FUNC(aiRegionController*(__thiscall*)(aiRegionController*, aiFaction*, int), createRegionController)(controller, aiFac, sett->regionID);
	controller->settlement = sett;
	return controller;
}
#pragma endregion Campaign AI Helpers

/*--------------------------------------------------------------------------------------------------------------------*\
									   aiRegionData methods
\*--------------------------------------------------------------------------------------------------------------------*/
#pragma region aiRegionData methods

settlementStruct* aiRegionData::getSettlement()
{
	if (reinterpret_cast<DWORD>(this) == 0x2896)
	{
		gameHelpers::logStringGame("error");
	}
	return minorSettlementDb::getSettlement(regionID, settlementIndex);
}

factionStruct* aiRegionData::getTargetFaction()
{
	if (const auto settlement = getSettlement(); settlement)
		return settlement->faction;
	return stratMapHelpers::getRegion(regionID)->factionOwner;
}

void aiRegionData::calculateRegionStrengths()
{
	strength.ownStrength = 0;
	strength.ownCount = 0;
	strength.enemyStrength = 0;
	strength.enemyCount = 0;
	strength.neutralStrength = 0;
	strength.neutralCount = 0;
	neighboursStrength.ownStrength = 0;
	neighboursStrength.ownCount = 0;
	neighboursStrength.enemyStrength = 0;
	neighboursStrength.enemyCount = 0;
	neighboursStrength.neutralStrength = 0;
	neighboursStrength.neutralCount = 0;
	const auto reg = stratMapHelpers::getRegion(regionID);
	const auto sett = getSettlement();
	const auto fac = campaignHelpers::getCampaignData()->getFactionById(factionID);
	const int slaveId = campaignHelpers::getCampaignData()->slaveFactionID;
	for (int i = 0; i < reg->stacksNum; i++)
	{
		if (const auto army = reg->getArmy(i); army->faction->factionID == factionID)
		{
			const int val = (army->settlement && army->settlement != sett) ? army->totalStrength / 2 : army->totalStrength;
			strength.ownStrength += army->faction->factionID == slaveId ? val / 2 : val;
			strength.ownCount++;
		}
		else if (army->isEnemyToFaction(fac))
		{
			if (army->settlement && army->settlement == sett)
			{
				strength.enemyStrength += army->totalStrength;
				strength.enemyCount++;
				continue;
			}
			const int val = army->settlement ? army->totalStrength / 2 : army->totalStrength;
			strength.enemyStrength += army->faction->factionID == slaveId ? val / 2 : val;
			strength.enemyCount++;
		}
		else if (!army->isAllyToFaction(fac))
		{
			const int val = (army->settlement && army->settlement != sett) ? army->totalStrength / 2 : army->totalStrength;
			strength.neutralStrength += army->faction->factionID == slaveId ? val / 2 : val;
			strength.neutralCount++;
		}
	}
	for (int i = 0; i < reg->neighbourRegionsNum; i++)
	{
		const auto nRegion = reg->neighbourRegions[i];
		if (nRegion.isBlocked || nRegion.region->isSea || !nRegion.region->settlement)
				continue;
		for (int j = 0; j < nRegion.region->stacksNum; j++)
		{
			if (const auto army = nRegion.region->getArmy(j); army->faction->factionID == fac->factionID)
			{
				const int val = army->settlement ? army->totalStrength / 2 : army->totalStrength;
				neighboursStrength.ownStrength += army->faction->factionID == slaveId ? val / 2 : val;
				neighboursStrength.ownCount++;
			}
			else if (army->isEnemyToFaction(fac))
			{
				const int val = army->settlement ? army->totalStrength / 2 : army->totalStrength;
				neighboursStrength.enemyStrength += army->faction->factionID == slaveId ? val / 2 : val;
				neighboursStrength.enemyCount++;
			}
			else if (!army->isAllyToFaction(fac))
			{
				const int val = army->settlement ? army->totalStrength / 2 : army->totalStrength;
				neighboursStrength.neutralStrength += army->faction->factionID == slaveId ? val / 2 : val;
				neighboursStrength.neutralCount++;
			}
		}
	}
}
#pragma endregion aiRegionData methods

/*--------------------------------------------------------------------------------------------------------------------*\
									   aiRegionController methods
\*--------------------------------------------------------------------------------------------------------------------*/
#pragma region aiRegionController methods

void aiRegionController::initialize()
{
	if (!region->hasFaction(aiFaction->factionID))
	{
		if (garrison)
			aiFaction->aiResourceManager->releaseResource(garrison);
		garrison = nullptr;
	}
	requiredGarrisonStrength = campaignAi::assessGarrisonStrength(gsdData, settlement, aiFaction->faction);
	if (gsdData && garrison)
	{
		if (gsdData->strength.enemyStrength == 0
			|| (gsdData->strength.ownStrength / static_cast<float>(gsdData->strength.enemyStrength) > 0.9))
		{
			garrisonType = 1;
		}
	}
}
#pragma endregion aiRegionController methods

/*--------------------------------------------------------------------------------------------------------------------*\
									   aiSubterfugeController methods
\*--------------------------------------------------------------------------------------------------------------------*/
#pragma region aiSubterfugeController methods

void aiSubterfugeController::destroyFaction(factionStruct* fac, const int value)
{
	GAME_FUNC(void(__thiscall*)(aiSubterfugeController*, factionStruct*, int),
		destroyFaction)(this, fac, value);
}

void aiSubterfugeController::reduceUnitProduction(factionStruct* fac, int category, int value)
{
	GAME_FUNC(void(__thiscall*)(aiSubterfugeController*, factionStruct*, int, int),
		reduceEconomicProduction)(this, fac, category, value);
}

void aiSubterfugeController::reduceEconomicProduction(factionStruct* fac, int value)
{
	GAME_FUNC(void(__thiscall*)(aiSubterfugeController*, factionStruct*, int),
		reduceEconomicProduction)(this, fac, value);
}
#pragma endregion aiSubterfugeController methods

/*--------------------------------------------------------------------------------------------------------------------*\
									   aiMilitaryDirector methods
\*--------------------------------------------------------------------------------------------------------------------*/
#pragma region aiMilitaryDirector methods

void aiMilitaryDirector::initialize()
{
	int i = 0;
	while (i < defensiveCampaigns.count)
	{
		const auto defCampaign = defensiveCampaigns.campaigns[i];
		if (defCampaign->isDefensive())
		{
			++i;
			continue;
		}
		removeCampaign(&defensiveCampaigns, i);
		defCampaign->attacking = true;
		addCampaign(&offensiveCampaigns, defCampaign);
	}
	i = 0;
	while (i < offensiveCampaigns.count)
	{
		const auto offCampaign = offensiveCampaigns.campaigns[i];
		if (!offCampaign->isDefensive())
		{
			++i;
			continue;
		}
		removeCampaign(&offensiveCampaigns, i);
		offCampaign->attacking = false;
		addCampaign(&defensiveCampaigns, offCampaign);
	}
	checkDefensiveCampaigns(aiFaction->aiGlobalStrategyDirector);
	checkOffensiveCampaigns(aiFaction->aiGlobalStrategyDirector);
	checkAidingCampaigns(aiFaction->aiGlobalStrategyDirector);
	decideStrategies();
	
	for (i = 0; i < offensiveCampaigns.count; i++)
		offensiveCampaigns.campaigns[i]->initialize();
	
	for (i = 0; i < defensiveCampaigns.count; i++)
		defensiveCampaigns.campaigns[i]->initialize();
	
	for (i = 0; i < aidingCampaigns.count; i++)
		aidingCampaigns.campaigns[i]->initialize();
	
	for (i = 0; i < aiBrigandControllersNum; i++)
		callClassFunc<aiBrigandController*, void>(aiBrigandControllers[i], 0x4); //initialize
}

void aiMilitaryDirector::removeCampaign(campaignControllerArray* array, int index)
{
	if (const auto newCount = --array->count; index < newCount)
	{
		do
		{
			const auto c1 = array->campaigns[index + 1];
			const auto c2 = &array->campaigns[index++];
			*c2 = c1;
		}
		while (index < array->count);
	}
}

aiCampaignController* aiMilitaryDirector::createCampaignController(aiRegionData* regionData)
{
	const auto newController = techFuncs::createGameClass<aiCampaignController>();
	GAME_FUNC(aiCampaignController*(__thiscall*)(aiCampaignController*, int, aiRegionData*, struct aiFaction*),
		createCampaignController)(newController, regionData->regionID, regionData, aiFaction);
	return newController;
}

float ltgdConfig::getPriorityScaled(const factionStruct* fac1, const factionStruct* fac2)
{
	const auto campData = campaignHelpers::getCampaignData();
	const auto facDip = campData->getFactionDiplomacy(fac1->factionID, fac2->factionID);
	auto invPriority = static_cast<float>(fac1->aiFaction->ltgd->longTermGoalValues[fac2->factionID].invadePriority);
	invPriority *= (facDip->factionStanding * invPriorityFsModifier);
	invPriority = scalePriority(static_cast<int>(invPriority)) * 1000.f;
	return invPriority;
}

float ltgdConfig::getPriorityMod(const factionStruct* fac1, const factionStruct* fac2)
{
	const auto campData = campaignHelpers::getCampaignData();
	const auto facDip = campData->getFactionDiplomacy(fac1->factionID, fac2->factionID);
	auto invPriority = static_cast<float>(fac1->aiFaction->ltgd->longTermGoalValues[fac2->factionID].invadePriority);
	invPriority *= (facDip->factionStanding * invPriorityFsModifier);
	invPriority = scalePriority(static_cast<int>(invPriority)) + 0.5f;
	return invPriority;
}

void aiMilitaryDirector::addCampaign(campaignControllerArray* array, aiCampaignController* controller)
{
	GAME_FUNC(void(__thiscall*)(campaignControllerArray*, aiCampaignController**), addCampaignController)(array, &controller);
}

void aiMilitaryDirector::checkDefensiveCampaigns(const aiGlobalStrategyDirector* director)
{
	for (int i = defensiveCampaigns.count - 1; i >= 0; --i)
	{
		const auto defCampaign = defensiveCampaigns.campaigns[i];
		bool found = false;
		for (int j = 0; j < director->ownRegionsCount; j++)
		{
			if (const auto ownRegion = &director->ownRegions[j];
				defCampaign->regionData->targetsSameSettlement(ownRegion))
			{
				defCampaign->regionData = ownRegion;
				found = true;
			}
		}
		if (!found)
		{
			removeCampaign(&defensiveCampaigns, i);
			if (defCampaign)
				callClassFunc<aiCampaignController*, void, int>(defCampaign, 0x0, 1); //destructor
		}
	}
	for (int i = 0; i < director->ownRegionsCount; i++)
	{
		const auto ownRegion = &director->ownRegions[i];
		bool found = false;
		for (int j = 0; j < defensiveCampaigns.count; j++)
		{
			if (const auto defRegion = defensiveCampaigns.campaigns[j]->regionData;
				defRegion->targetsSameSettlement(ownRegion))
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			const auto newController = createCampaignController(ownRegion);
			addCampaign(&defensiveCampaigns, newController);
		}
	}
	
	if (defensiveCampaigns.count > 1)
		sortCampaigns(&defensiveCampaigns);
}

void aiMilitaryDirector::checkOffensiveCampaigns(const aiGlobalStrategyDirector* director)
{
	for (int i = offensiveCampaigns.count - 1; i >= 0; --i)
	{
		const auto offCampaign = offensiveCampaigns.campaigns[i];
		bool found = false;
		for (int j = 0; j < director->targetRegionsCount; j++)
		{
			if (const auto targetRegion = &director->targetRegions[j];
				offCampaign->regionData->targetsSameSettlement(targetRegion))
			{
				offCampaign->regionData = targetRegion;
				found = true;
			}
		}
		if (!found)
		{
			removeCampaign(&offensiveCampaigns, i);
			if (offCampaign)
				callClassFunc<aiCampaignController*, void, int>(offCampaign, 0x0, 1); //destructor
		}
	}
	for (int i = 0; i < director->targetRegionsCount; i++)
	{
		const auto targetRegion = &director->targetRegions[i];
		bool found = false;
		for (int j = 0; j < offensiveCampaigns.count; j++)
		{
			if (const auto offRegion = offensiveCampaigns.campaigns[j]->regionData;
				offRegion->targetsSameSettlement(targetRegion))
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			const auto newController = createCampaignController(targetRegion);
			addCampaign(&offensiveCampaigns, newController);
		}
	}
	if (offensiveCampaigns.count > 1)
		sortCampaigns(&offensiveCampaigns);
}

void aiMilitaryDirector::checkAidingCampaigns(const aiGlobalStrategyDirector* director)
{
	for (int i = aidingCampaigns.count - 1; i >= 0; --i)
	{
		const auto aidCampaign = aidingCampaigns.campaigns[i];
		bool found = false;
		if (aiFaction->ltgd->isTrustedAlly(aidCampaign->regionData->getTargetFaction()->factionID)
			&& aidCampaign->regionData->strength.enemyStrength > 1500)
		{
			for (int j = 0; j < director->allNeighbourRegionsCount; j++)
			{
				if (const auto targetRegion = &director->allNeighbourRegions[j];
					aidCampaign->regionData->targetsSameSettlement(targetRegion))
				{
					aidCampaign->regionData = targetRegion;
					found = true;
				}
			}
		}
		if (!found)
		{
			removeCampaign(&aidingCampaigns, i);
			if (aidCampaign)
				callClassFunc<aiCampaignController*, void, int>(aidCampaign, 0x0, 1); //destructor
		}
	}
	for (int i = 0; i < director->allNeighbourRegionsCount; i++)
	{
		const auto targetRegion = &director->allNeighbourRegions[i];
		bool found = false;
		if (!aiFaction->ltgd->isTrustedAlly(targetRegion->getTargetFaction()->factionID))
			continue;
		for (int j = 0; j < aidingCampaigns.count; j++)
		{
			if (const auto aidRegion = aidingCampaigns.campaigns[j]->regionData;
				aidRegion->targetsSameSettlement(targetRegion))
			{
				found = true;
				break;
			}
		}
		if (!found && targetRegion->strength.enemyStrength > 1500)
		{
			const auto newController = createCampaignController(targetRegion);
			newController->setStrategy(aiCampaignStrategy::attackAid);
			newController->strengthRequiredType = static_cast<int>(aiCampaignStrength::medium);
			addCampaign(&aidingCampaigns, newController);
		}
	}
	if (aidingCampaigns.count > 1)
		sortCampaigns(&aidingCampaigns);
}

void aiMilitaryDirector::decideStrategies()
{
	int highestStagingValue = 0;
	int mostImportantRegion = -1;
	for (int i = 0; i < defensiveCampaigns.count; i++)
	{
		const auto defCampaign = defensiveCampaigns.campaigns[i];
		decideDefensiveStrategy(defCampaign);
		defCampaign->canRequestMore = false;
		if (const int stageValue = aiFaction->ltgd->getInvasionTargetPriority(defCampaign->regionData->regionID);
			stageValue > 0 && (stageValue + defCampaign->regionData->regionValue > highestStagingValue))
		{
			highestStagingValue = stageValue + defCampaign->regionData->regionValue;
			mostImportantRegion = i;
		}
	}
	if (mostImportantRegion > -1 && aiFaction->faction->settlementsNum > 5)
		defensiveCampaigns.campaigns[mostImportantRegion]->canRequestMore = true;
	
	for (int i = 0; i < offensiveCampaigns.count; i++)
	{
		const auto offCampaign = offensiveCampaigns.campaigns[i];
		if (offCampaign->regionData->isLastResort)
		{
			offCampaign->setStrategy(aiCampaignStrategy::attackNormal);
			offCampaign->strengthRequiredType = static_cast<int>(aiCampaignStrength::medium);
			offCampaign->canAttack = true;
			continue;
		}
		decideOffensiveStrategy(offCampaign);
	}

	const auto campaign = campaignHelpers::getCampaignData();
	for (int i = 0; i < campaign->factionCount; i++)
	{
		const int targetId = campaign->factionsSortedByTurnOrder[i]->factionID;
		const auto factionData = aiFaction->aiGlobalStrategyDirector->stratFacData[targetId];
		const auto ltgdData = aiFaction->ltgd->longTermGoalValues[targetId];
		if (factionData.activeWar)
		{
			bool authorized = false;
			if(ltgdData.forceInvade)
				authorized = true;
			else
			{
				switch (static_cast<invasionTypes>(ltgdData.invasionType))
				{
				case invasionTypes::buildup:
					{
						int offCampaignCount = 0;
						int readyCampaignCount = 0;
						for (int j = 0; j < offensiveCampaigns.count; j++)
						{
							if (const auto offCampaign = offensiveCampaigns.campaigns[j];
								offCampaign->regionData->getTargetFaction()->factionID == targetId)
							{
								offCampaignCount++;
								if (offCampaign->totalAllocated >= offCampaign->totalStrengthRequired)
									readyCampaignCount++;
							}
						}
						if (readyCampaignCount >= offCampaignCount >> 1 || readyCampaignCount >= 2)
							authorized = true;
						break;
					}
				case invasionTypes::immediate:
				case invasionTypes::raids:
				case invasionTypes::opportunistic:
					{
						authorized = true;
						break;
					}
				case invasionTypes::start:
				case invasionTypes::none:
				case invasionTypes::count:
				default:
					break;
				}
			}
			if (authorized)
			{
				for (int j = 0; j < offensiveCampaigns.count; j++)
				{
					if (const auto offCampaign = offensiveCampaigns.campaigns[j];
						offCampaign->regionData->getTargetFaction()->factionID == targetId)
					{
						offCampaign->canAttack = true;
					}
				}
			}
		}
		else
		{
			for (int j = 0; j < offensiveCampaigns.count; j++)
			{
				if (const auto offCampaign = offensiveCampaigns.campaigns[j];
					offCampaign->regionData->getTargetFaction()->factionID == targetId
					&& !offCampaign->regionData->navalTarget)
				{
					offCampaign->canAttack = false;
				}
			}
		}
	}
}

void aiMilitaryDirector::decideOffensiveStrategy(aiCampaignController* controller)
{
	const auto targetValues = aiFaction->ltgd->longTermGoalValues[controller->regionData->getTargetFaction()->factionID];
	switch (static_cast<invasionTypes>(targetValues.invasionType))
	{
	case invasionTypes::buildup:
		{
			controller->setStrategy(aiCampaignStrategy::attackGrind);
			controller->strengthRequiredType = static_cast<int>(aiCampaignStrength::high);
			break;
		}
	case invasionTypes::immediate:
		{
			controller->setStrategy(aiCampaignStrategy::attackNormal);
			controller->strengthRequiredType = static_cast<int>(aiCampaignStrength::high);
			break;
		}
	case invasionTypes::raids:
		{
			controller->setStrategy(aiCampaignStrategy::attackRaid);
			controller->strengthRequiredType = static_cast<int>(aiCampaignStrength::medium);
			break;
		}
	case invasionTypes::opportunistic:
		{
			controller->setStrategy(aiCampaignStrategy::attackBlitz);
			controller->strengthRequiredType = static_cast<int>(aiCampaignStrength::high);
			break;
		}
	case invasionTypes::start:
		{
			controller->setStrategy(aiCampaignStrategy::attackNormal);
			controller->strengthRequiredType = static_cast<int>(aiCampaignStrength::medium);
			break;
		}
	case invasionTypes::none:
	case invasionTypes::count:
	default:
		{
			controller->setStrategy(aiCampaignStrategy::dormant);
			controller->strengthRequiredType = static_cast<int>(aiCampaignStrength::veryLow);
			break;
		}
	}
	if (controller->regionData->navalTarget)
		controller->strengthRequiredType = static_cast<int>(aiCampaignStrength::veryLow);
}

void aiMilitaryDirector::decideDefensiveStrategy(aiCampaignController* controller)
{
	const auto region = stratMapHelpers::getRegion(controller->regionData->regionID);
	auto chosenType = defendTypes::minimal;
	const auto settCount = region->settlementCount();
	for (int i = 0; i < settCount; i++)
	{
		const auto settlement = region->getSettlement(i);
		if (!settlement || settlement->faction->factionID == aiFaction->factionID)
			continue;
		if (const auto facDefendType = static_cast<defendTypes>(aiFaction->ltgd->longTermGoalValues[settlement->faction->factionID].defendType);
			facDefendType > chosenType)
		{
			chosenType = facDefendType;
		}
	}
	for (int i = 0; i < region->neighbourRegionsNum; i++)
	{
		const auto nRegion = region->neighbourRegions[i];
		if (nRegion.region->isSea || nRegion.isBlocked || !nRegion.region->settlement)
			continue;
		const int nSettCount = nRegion.region->settlementCount();
		for (int j = 0; j < nSettCount; j++)
		{
			const auto nSett = nRegion.region->getSettlement(j);
			if (!nSett || nSett->faction->factionID == aiFaction->factionID)
				continue;
			if (const auto facDefendType = static_cast<defendTypes>(aiFaction->ltgd->longTermGoalValues[nSett->faction->factionID].defendType);
				facDefendType > chosenType)
			{
				chosenType = facDefendType;
			}
		}
	}
	switch(chosenType)
	{
		case defendTypes::minimal:
			{
				controller->setStrategy(aiCampaignStrategy::defendHidden);
				controller->strengthRequiredType = static_cast<int>(aiCampaignStrength::low);
				return;
			}
		case defendTypes::normal:
			{
				controller->setStrategy(aiCampaignStrategy::defendHidden);
				controller->strengthRequiredType = static_cast<int>(aiCampaignStrength::medium);
				return;
			}
		case defendTypes::raid:
			{
				controller->setStrategy(aiCampaignStrategy::defendBorder);
				controller->strengthRequiredType = static_cast<int>(aiCampaignStrength::medium);
				return;
			}
		case defendTypes::frontline:
			{
				controller->setStrategy(aiCampaignStrategy::defendBorder);
				controller->strengthRequiredType = static_cast<int>(aiCampaignStrength::high);
				return;
			}
		case defendTypes::fortified:
			{
				controller->setStrategy(aiCampaignStrategy::defendFortified);
				controller->strengthRequiredType = static_cast<int>(aiCampaignStrength::medium);
				return;
			}
		case defendTypes::deep:
			{
				controller->setStrategy(aiCampaignStrategy::defendDeep);
				controller->strengthRequiredType = static_cast<int>(aiCampaignStrength::medium);
				return;
			}
		case defendTypes::count:
		default:
			return;
	}
}
#pragma endregion aiMilitaryDirector methods

/*--------------------------------------------------------------------------------------------------------------------*\
									   aiCampaignController methods
\*--------------------------------------------------------------------------------------------------------------------*/
#pragma region aiCampaignController methods

bool aiCampaignController::isDefensive()
{
	if (const auto factionId = regionData->getTargetFaction()->factionID;
		!regionData->navalTarget && factionId == aiFaction->faction->factionID)
	{
		attacking = false;
		targetFactionId = 31;
		return true;
	}
	else 
	{
		attacking = true;
		if (aiFaction->aiGlobalStrategyDirector->stratFacData[factionId].plannedWar)
			targetFactionId = factionId;
		return false;
	}
}

void aiCampaignController::initialize()
{
	currentMission = nullptr;
	isDefensive();
	if (nextStrategyType != aiCampaignStrategy::dormant && strategyType == aiCampaignStrategy::dormant)
	{
		strategyType = static_cast<int>(attacking ? aiCampaignStrategy::gathering : aiCampaignStrategy::defendBorder);
	}
	else if (attacking && nextStrategyType != aiCampaignStrategy::attackAid
		&& !aiFaction->aiGlobalStrategyDirector->stratFacData[regionData->getTargetFaction()->factionID].atWar
		&& !regionData->navalTarget)
	{
		canRequestMore = false;
		canAttack = false;
		nextStrategyType = static_cast<int>(aiCampaignStrategy::dormant);
	}
	else if (!attacking && strategyType < aiCampaignStrategy::defendBorder)
	{
		strategyType = static_cast<int>(aiCampaignStrategy::defendBorder);
	}
	if (!regionData->regionController)
		attacking = true;

	merge = true;
	
	GAME_FUNC(void(__thiscall*)(aiCampaignController*), calculateCampaignControllerStrengths)(this);
	
	for ( auto objective = objectives; objective; objective = objective->next )
	{
		callClassFunc<aiCampaignObjective*, void>(objective, 0x18);
	}
}

void aiCampaignController::update()
{
	callClassFunc<aiCampaignController*, void>(this, 0x68); //validate resources

	while (true)
	{
		const auto current =  getCurrentStrategy();
		switch(current)
		{
		case aiCampaignStrategy::dormant:
			break;
		case aiCampaignStrategy::gathering:
			break;
		case aiCampaignStrategy::attackNormal:
			break;
		case aiCampaignStrategy::attackBlitz:
			break;
		case aiCampaignStrategy::attackGrind:
			break;
		case aiCampaignStrategy::attackRaid:
			break;
		case aiCampaignStrategy::defendBorder:
			break;
		case aiCampaignStrategy::defendFortified:
			break;
		case aiCampaignStrategy::defendHidden:
			break;
		case aiCampaignStrategy::defendDeep:
			break;
		case aiCampaignStrategy::attackAid:
			break;
		case aiCampaignStrategy::count:
		default:
			break;
		}
		if (current == getCurrentStrategy())
			break;
	}
}

std::string strategyTypeToString(aiCampaignStrategy strategy)
{
	switch (strategy)
	{
	case aiCampaignStrategy::dormant:
		return "dormant";
	case aiCampaignStrategy::gathering:
		return "gathering";
	case aiCampaignStrategy::attackNormal:
		return "attackNormal";
	case aiCampaignStrategy::attackBlitz:
		return "attackBlitz";
	case aiCampaignStrategy::attackGrind:
		return "attackGrind";
	case aiCampaignStrategy::attackRaid:
		return "attackRaid";
	case aiCampaignStrategy::defendBorder:
		return "defendBorder";
	case aiCampaignStrategy::defendFortified:
		return "defendFortified";
	case aiCampaignStrategy::defendHidden:
		return "defendHidden";
	case aiCampaignStrategy::defendDeep:
		return "defendDeep";
	case aiCampaignStrategy::attackAid:
		return "attackAid";
	case aiCampaignStrategy::count:
	default:
		return "unknown";
	}
}

void aiCampaignController::logData()
{
	const auto facName = string(aiFaction->faction->factionRecord->facName);
	std::string logString = "Campaign Controller Data: ";
	logString += "\nFaction: " + facName;
	logString += "\nSettlement: " + string(regionData->getSettlement()->name);
	logString += "\nDefensive: " + string(attacking ? "no" : "yes");
	logString += "\nRegion Priority: " + std::to_string(regionData->priority);
	logString += "\nRegion value: " + std::to_string(regionData->regionValue);
	logString += "\nCurrent strategy: " + strategyTypeToString(getCurrentStrategy());
	logString += "\nReq strategy: " + strategyTypeToString(getNextStrategy());
	logString += "\nStr required: " + std::to_string(totalStrengthRequired);
	logString += "\nStr allocated: " + std::to_string(totalAllocated);
	gameHelpers::logStringGame(logString);
}

void aiCampaignController::updateAllocation()
{
	if (!regionData)
		return;
	if (attacking)
	{
		regionData->calculateRegionStrengths();
		totalStrengthRequired = regionData->strength.enemyStrength - regionData->strength.ownStrength / 2;
		totalStrengthRequired += regionData->neighboursStrength.enemyStrength / 4 - regionData->neighboursStrength.ownStrength / 8;
		if (const auto targetSett = regionData->getSettlement(); targetSett && targetSett->army)
		{
			if (const int settStrength = static_cast<int>(targetSett->army->totalStrength * 1.5f); totalStrengthRequired < settStrength)
				totalStrengthRequired = settStrength;
		}
		if (totalStrengthRequired < 0)
			totalStrengthRequired = 500;
		strengthThreshold = static_cast<int>(totalStrengthRequired * 0.95f);
	}
}

void aiCampaignController::setStrategy(aiCampaignStrategy strategy)
{
	if (strategyType == aiCampaignStrategy::dormant)
		strategyType = static_cast<int>(aiCampaignStrategy::gathering);
	nextStrategyType = static_cast<int>(strategy);
}
#pragma endregion aiCampaignController methods

/*--------------------------------------------------------------------------------------------------------------------*\
									   aiLongTermGoalDirector methods
\*--------------------------------------------------------------------------------------------------------------------*/
#pragma region aiLongTermGoalDirector methods

int aiLongTermGoalDirector::getInvasionTargetNum(const int regionId)
{
	int num = 0;
	const auto region = stratMapHelpers::getRegion(regionId);
	const int settCount = region->settlementCount();
	for (int i = 0; i < settCount; i++)
	{
		const auto settlement = region->getSettlement(i);
		if (!settlement || settlement->faction->factionID == faction->factionID)
			continue;
		if (longTermGoalValues[settlement->faction->factionID].invasionType < invasionTypes::none)
			num++;
	}
	for (int i = 0; i < region->neighbourRegionsNum; i++)
	{
		const auto nRegion = region->neighbourRegions[i];
		const int nSettCount = nRegion.region->settlementCount();
		for (int j = 0; j < nSettCount; j++)
		{
			const auto settlement = nRegion.region->getSettlement(j);
			if (!settlement || settlement->faction->factionID == faction->factionID)
				continue;
			if (longTermGoalValues[settlement->faction->factionID].invasionType < invasionTypes::none)
				num++;
		}
	}
	return num;
}
int aiLongTermGoalDirector::getInvasionTargetPriority(const int regionId)
{
	int num = 0;
	const auto region = stratMapHelpers::getRegion(regionId);
	const int settCount = region->settlementCount();
	for (int i = 0; i < settCount; i++)
	{
		const auto settlement = region->getSettlement(i);
		if (!settlement || settlement->faction->factionID == faction->factionID)
			continue;
		if (longTermGoalValues[settlement->faction->factionID].invasionType < invasionTypes::none)
			num += longTermGoalValues[settlement->faction->factionID].invadePriority;
	}
	for (int i = 0; i < region->neighbourRegionsNum; i++)
	{
		const auto nRegion = region->neighbourRegions[i];
		const int nSettCount = nRegion.region->settlementCount();
		for (int j = 0; j < nSettCount; j++)
		{
			const auto settlement = nRegion.region->getSettlement(j);
			if (!settlement || settlement->faction->factionID == faction->factionID)
				continue;
			if (longTermGoalValues[settlement->faction->factionID].invasionType < invasionTypes::none)
				num += longTermGoalValues[settlement->faction->factionID].invadePriority;
		}
	}
	return num;
}

bool aiLongTermGoalDirector::regionsBordersOnlyTrusted(int regionId)
{
	const auto region = stratMapHelpers::getRegion(regionId);
	const auto settCount = region->settlementCount();
	for (int i = 0; i < settCount; i++)
	{
		const auto settlement = region->getSettlement(i);
		if (!settlement || settlement->faction->factionID == faction->factionID)
			continue;
		if (!isTrustedAlly(settlement->faction->factionID))
			return false;
	}
	for (int i = 0; i < region->neighbourRegionsNum; i++)
	{
		const auto nRegion = region->neighbourRegions[i];
		const auto nSettCount = nRegion.region->settlementCount();
		for (int j = 0; j < nSettCount; j++)
		{
			const auto settlement = nRegion.region->getSettlement(j);
			if (!settlement || settlement->faction->factionID == faction->factionID)
				continue;
			if (!isTrustedAlly(settlement->faction->factionID))
				return false;
		}
	}
	return true;
}

bool aiLongTermGoalDirector::isTrustedAlly(int targetFactionId)
{
	return GAME_FUNC(bool(__thiscall*)(aiLongTermGoalDirector*, int), isTrustedAlly)(this, targetFactionId);
}
#pragma endregion aiLongTermGoalDirector methods

/*--------------------------------------------------------------------------------------------------------------------*\
									   aiPersonalityValues methods
\*--------------------------------------------------------------------------------------------------------------------*/
#pragma region aiPersonalityValues methods

void aiPersonalityValues::evaluatePolicies(const int regionId, const int settlementIndex)
{
	if (settlementPoliciesCount == 0)
		return;
	std::vector<int> regionPolicies;
	for (int i = 0; i < settlementPoliciesCount; i++)
	{
		if (settlementPolicies[i].regionID == regionId)
		{
			regionPolicies.push_back(i);
		}
	}
	if (settlementIndex < static_cast<int>(regionPolicies.size()))
	{
		const auto settlement = minorSettlementDb::getSettlement(regionId, settlementIndex);
		const int policyIndex = regionPolicies[settlementIndex];
		const auto policy = &settlementPolicies[policyIndex];
		policy->autoManageBuildPolicy = static_cast<int>(decideSettlementPolicy(settlement));
		policy->secondaryPolicy = static_cast<int>(decideSettlementTroopPolicy(settlement));
		settlement->aiProductionController->autoManagePolicy = policy->autoManageBuildPolicy;
		settlement->aiProductionController->secondaryPolicy = policy->secondaryPolicy;
	}
}

settlementPolicy aiPersonalityValues::decideSettlementPolicy(const settlementStruct* settlement)
{
	const int oldPolicy = settlement->aiProductionController->autoManagePolicy;
	const auto region = stratMapHelpers::getRegion(settlement->regionID);
	const int neighbourNum = region->neighbourRegionsNum;
	int enemyNum = region->getEnemySettsToFaction(settlement->factionID);
	int neutralNum = region->getNeutralSettsToFaction(settlement->factionID);
	for (int i = 0; i < neighbourNum; i++)
	{
		const auto nRegion = region->neighbourRegions[i];
		if (nRegion.region->isSea || nRegion.isBlocked || !nRegion.region->settlement)
			continue;
		enemyNum += nRegion.region->getEnemySettsToFaction(settlement->factionID);
		neutralNum += nRegion.region->getNeutralSettsToFaction(settlement->factionID);
	}
	const int allOtherNum = enemyNum + neutralNum;
	float priorities[5] = {};
	priorities[static_cast<int>(settlementPolicy::financial)] = getPolicyPriority(settlementPolicy::financial) * (allOtherNum == 0 ? 2 : 1);
	priorities[static_cast<int>(settlementPolicy::cultural)] = getPolicyPriority(settlementPolicy::cultural) * (allOtherNum / 2.f);
	priorities[static_cast<int>(settlementPolicy::military)] = getPolicyPriority(settlementPolicy::military) * (allOtherNum + (enemyNum * 2));
	priorities[static_cast<int>(settlementPolicy::balanced)] = getPolicyPriority(settlementPolicy::balanced) * (allOtherNum == 0 ? 1 : 2);
	float highest = 0.0f;
	int bestPolicy = oldPolicy;
	for (int i = 0; i < 5; i++)
	{
		if (priorities[i] > highest)
		{
			if ((oldPolicy != settlementPolicy::balanced && oldPolicy != settlementPolicy::none)
				&& priorities[oldPolicy] + 0.4f > priorities[i])
				continue;
			highest = priorities[i];
			bestPolicy = i;
		}
	}
	return static_cast<settlementPolicy>(bestPolicy);
}

settlementTroopPolicy aiPersonalityValues::decideSettlementTroopPolicy(const settlementStruct* settlement)
{
	float priorities[5] = {};
	const auto region = stratMapHelpers::getRegion(settlement->regionID);
	const int neighbourNum = region->neighbourRegionsNum;
	int enemyNum = region->getEnemySettsToFaction(settlement->factionID);
	int neutralNum = region->getEnemySettsToFaction(settlement->factionID);
	for (int i = 0; i < neighbourNum; i++)
	{
		const auto nRegion = region->neighbourRegions[i];
		if (nRegion.region->isSea || nRegion.isBlocked || !nRegion.region->settlement)
			continue;
		enemyNum += nRegion.region->getEnemySettsToFaction(settlement->factionID);
		neutralNum += nRegion.region->getNeutralSettsToFaction(settlement->factionID);
	}
	const bool isUnderRisk = enemyNum > 0;
	const bool safeRegion = enemyNum + neutralNum == 0;
	float garrisonMissileRatio = 0.0f;
	float garrisonCavalryRatio = 0.0f;
	float garrisonInfantryRatio = 0.0f;
	float garrisonSpearmenRatio = 0.0f;
	float garrisonArtilleryRatio = 0.0f;
	if (settlement->army)
	{
		int garrisonArtilleryNum = 0;
		int garrisonSpearmenNum = 0;
		int garrisonInfantryNum = 0;
		int garrisonCavalryNum = 0;
		int garrisonMissileNum = 0;
		for (int i = 0; i < settlement->army->numOfUnits; i++)
		{
			const auto un = settlement->army->units[i];
			if (un->eduEntry->unitClass == unitClass::missile)
				garrisonMissileNum++;
			if (un->eduEntry->unitClass == unitClass::spearmen)
				garrisonSpearmenNum++;
			if (un->eduEntry->category == unitCategory::cavalry)
				garrisonCavalryNum++;
			if (un->eduEntry->category == unitCategory::siege)
				garrisonArtilleryNum++;
			if (un->eduEntry->category == unitCategory::infantry)
				garrisonInfantryNum++;
		}
		garrisonArtilleryRatio = garrisonArtilleryNum / static_cast<float>(settlement->army->numOfUnits);
		garrisonCavalryRatio = garrisonCavalryNum / static_cast<float>(settlement->army->numOfUnits);
		garrisonInfantryRatio = garrisonInfantryNum / static_cast<float>(settlement->army->numOfUnits);
		garrisonMissileRatio = garrisonMissileNum / static_cast<float>(settlement->army->numOfUnits);
		garrisonSpearmenRatio = garrisonSpearmenNum / static_cast<float>(settlement->army->numOfUnits);
	}
	priorities[static_cast<int>(settlementTroopPolicy::infantry)] = (1.f - garrisonInfantryRatio) * (isUnderRisk ? 4 : 2);
	priorities[static_cast<int>(settlementTroopPolicy::cavalry)] = (1.f - garrisonCavalryRatio) * (safeRegion ? 4 : 2);
	priorities[static_cast<int>(settlementTroopPolicy::missile)] = (1.f - garrisonMissileRatio) * (isUnderRisk ? 3 : 1);
	priorities[static_cast<int>(settlementTroopPolicy::spearmen)] = (1.f - garrisonSpearmenRatio) * (isUnderRisk ? 2 : 1);
	priorities[static_cast<int>(settlementTroopPolicy::siege)] = (1.f - garrisonArtilleryRatio) * (safeRegion ? 2 : 1);
	float highest = 0.0f;
	int bestPolicy = 0;
	for (int i = 0; i < 5; i++)
	{
		if (priorities[i] > highest)
		{
			highest = priorities[i];
			bestPolicy = i;
		}
	}
	return static_cast<settlementTroopPolicy>(bestPolicy);
}

float aiPersonalityValues::getPolicyPriority(const settlementPolicy policyType)
{
	switch (policyType)
	{
		case settlementPolicy::balanced:
			return 1.0f;
		case settlementPolicy::financial:
			{
				if (financialPolicyNum == 0)
					return 2.5f;
				if (financialPolicyNum * 2 > aiFaction->faction->settlementsNum)
					return 0.6f;
				return 1.75f;
			}
		case settlementPolicy::military:
			{
				if (militaryPolicyNum * 4 < aiFaction->faction->settlementsNum)
					return 1.7f;
				return 1.0f;
			}
		case settlementPolicy::cultural:
			{
				if (culturalPolicyNum == 0)
					return 2.0f;
				if (culturalPolicyNum * 8 > aiFaction->faction->settlementsNum)
					return 0.5f;
				return 1.4f;
			}
		case settlementPolicy::growth:
			{
				if (growthPolicyNum == 0)
					return 1.5f;
				return 0.5f;
			}
		case settlementPolicy::none:
			return 0.0f;
	}
	return 0.0f;
}
#pragma endregion aiPersonalityValues methods

/*--------------------------------------------------------------------------------------------------------------------*\
									   aiGlobalStrategyDirector methods
\*--------------------------------------------------------------------------------------------------------------------*/
#pragma region aiGlobalStrategyDirector methods

void aiGlobalStrategyDirector::initialize()
{
	initOwnRegions();
	initNeighbourRegions();
	initNavalRegions();
	GAME_FUNC(void(__thiscall*)(aiGlobalStrategyDirector*), createHordeController)(this);
	if (hordeController)
		return;
	updateRegionControllers();
	GAME_FUNC(void(__cdecl*)(), recalcRegionGroupStrengths)();
	GAME_FUNC(void(__thiscall*)(aiGlobalStrategyDirector*), manageRegionGroups)(this);
	int maxValue = -100000;
	int minValue = 100000;
	for (int i = 0; i < ownRegionsCount; i++)
	{
		const auto regionData = &ownRegions[i];
		if (regionData->regionValue > maxValue)
			maxValue = regionData->regionValue;
		if (regionData->regionValue < minValue)
			minValue = regionData->regionValue;
	}
	for (int i = 0; i < allNeighbourRegionsCount; i++)
	{
		const auto regionData = &allNeighbourRegions[i];
		if (regionData->regionValue > maxValue)
			maxValue = regionData->regionValue;
		if (regionData->regionValue < minValue)
			minValue = regionData->regionValue;
	}
	const float scale = 1000.f / (maxValue - minValue);
	for (int i = 0; i < ownRegionsCount; i++)
	{
		const auto regionData = &ownRegions[i];
		if (minValue == maxValue)
			regionData->priority = 650;
		else
			regionData->priority = static_cast<int>((regionData->regionValue - minValue) * scale);
	}
	for (int i = 0; i < allNeighbourRegionsCount; i++)
	{
		const auto regionData = &allNeighbourRegions[i];
		if (minValue == maxValue)
			regionData->priority = 650;
		else
			regionData->priority = static_cast<int>(((regionData->regionValue - minValue) * scale));
	}
	GAME_FUNC(void(__thiscall*)(aiGlobalStrategyDirector*), initGsdFactionData)(this);
}

void aiGlobalStrategyDirector::initialize2()
{
	const auto campaign = campaignHelpers::getCampaignData();
	GAME_FUNC(void(__thiscall*)(aiNavalTransportRegion**), clearTransports)(&navalTransportTargets);
	if (hordeController)
		callClassFunc<void*, void>(hordeController, 0x4);
	else
	{
		initTargetRegions();
		GAME_FUNC(void(__thiscall*)(aiGlobalStrategyDirector*), aiCrusadeController)(this);
		for (int i = 0; i < campaign->factionCount; i++)
		{
			const auto fac = campaign->getFactionByOrder(i);
			const auto facDip = campaign->getFactionDiplomacy(faction->factionID, fac->factionID);
			if (faction->isNeighbourFaction(fac) && facDip->state == dipStance::war)
			{
				stratFacData[fac->factionID].atWar = true;
				stratFacData[fac->factionID].activeWar = true;
			}
		}
		militaryDirector.initialize();
		if (crusadeController)
			callClassFunc<void*, void>(crusadeController, 0x4);
		GAME_FUNC(bool(__thiscall*)(aiGlobalStrategyDirector*), manageNavalTransports)(this);
		GAME_FUNC(bool(__thiscall*)(aiGlobalStrategyDirector*), balanceRegionGroups)(this);
		
		for (int i = 0; i < regionGroupControllersNum; i++)
			callClassFunc<void*, void>(regionGroupControllers[i], 0x4);
	}
	
	for (int i = 0; i < aiNavalControllersNum; i++)
		GAME_FUNC(void(__thiscall*)(void*, aiRegionData**), initNavalController)(aiNavalControllers[i], &ownRegions);

	for (int i = 0; i < campaign->factionCount; i++)
	{
		const auto fac = campaign->factionsSortedByTurnOrder[i];
		const auto facId = faction->factionID;
		if (facId == campaign->slaveFactionID || fac->deadStatus > 0)
			continue;
		const auto stratData = stratFacData[facId];
		if (stratData.atWar && fac->getAliveCharacterNumOfType(characterTypeStrat::namedCharacter) < 6)
			aiFaction->subterfugeController->destroyFaction(fac, stratFacData->desire + 500);
		if (stratData.activeWar)
		{
			if (aiFaction->faction->incomeDoubled < fac->incomeDoubled)
				aiFaction->subterfugeController->reduceEconomicProduction(fac, stratFacData->desire);
			else
				aiFaction->subterfugeController->reduceUnitProduction(fac, static_cast<int>(unitCategory::infantry) ,stratFacData->desire);
		}
	}
}

void aiGlobalStrategyDirector::initOwnRegions()
{
	clearRegionData(&ownRegions);
	const auto fac = faction;
	for (int i = 0; i < fac->settlementsNum; i++)
	{
		const auto sett = fac->settlements[i];
		if (const auto regionData = createRegionInArray(this, sett, &ownRegions); regionData->strength.enemyStrength <= 0)
		{
			if (regionData->neighboursStrength.enemyStrength > 0 && regionData->neighbourEnemyNum > 0)
				regionData->setRisk(regionRisk::mediumRisk);
			else if (regionData->neighbourOtherNum > 0
				&& !aiFaction->ltgd->regionsBordersOnlyTrusted(sett->regionID))
				regionData->setRisk(regionRisk::lowRisk);
		}
		else
			regionData->setRisk(regionRisk::highRisk);
	}
	for (int i = 0; i < ownRegionsCount; i++)
		GAME_FUNC(void(__cdecl*)(aiRegionData*), resetRegionGroupControllers)(&ownRegions[i]);
}

void aiGlobalStrategyDirector::initNeighbourRegions()
{
	clearRegionData(&allNeighbourRegions);
	for (int n = 0; n < faction->neighBourRegionsNum; n++)
	{
		const int neighbourId = faction->neighBourRegions[n];
		const auto nRegion = stratMapHelpers::getRegion(neighbourId);
		const auto settCount = nRegion->settlementCount();
		for (int i = 0; i < settCount; i++)
		{
			const auto sett = nRegion->getSettlement(i);
			if (sett->faction->factionID == faction->factionID)
				continue;
			const auto regionData = createRegionInArray(this, sett, &allNeighbourRegions);
			if (sett->isPlayerControlled())
				regionData->regionValue = static_cast<int>(regionData->regionValue * campaignHelpers::getCampaignDifficulty1()->playerRegionValueModifier);
			if (faction->factionRecord->shadowFactionId == sett->faction->factionID)
				regionData->regionValue += 500;
			if (sett->isMinorSettlement && nRegion->factionOwner->factionID == faction->factionID)
				regionData->regionValue += 500;
			else if (nRegion->hasFaction(faction->factionID))
				regionData->regionValue += 250;
			if(sett->faction->factionRecord->slave)
				regionData->regionValue += campaignHelpers::getCampaignData()->turnNumber < 30 ? 500 : 250;
			if (regionData->regionValue < 0)
				regionData->regionValue = 0;
			regionData->neighbourEnemyNum = -1;
			regionData->neighbourOtherNum = -1;
			regionData->setRisk(regionRisk::safe);
		}
	}
}

void aiGlobalStrategyDirector::updateRegionControllers()
{
	if (hordeController)
		return;
	for (int i = regionControllersNum - 1; i >= 0; --i)
	{
		const auto controller = regionControllers[i];
		const auto settlement = controller->settlement;
		const auto region = stratMapHelpers::getRegion(settlement->regionID);
		if (settlement->faction == faction)
		{
			for (int j = 0; j < ownRegionsCount; j++)
			{
				auto regData = ownRegions[j];
				if (regData.regionID == settlement->regionID && settlement->getMinorSettlementIndex() == regData.settlementIndex)
				{
					controller->gsdData = &regData;
					break;
				}
			}
		}
		else
		{
			const int settlementCount = region->settlementCount();
			for (int j = 0; j < settlementCount; j++)
			{
				if (const auto sett = region->getSettlement(j); sett->faction == faction)
					aiFaction->aiProductionControllers->evaluatePolicies(sett->regionID, sett->getMinorSettlementIndex());
			}
			for (int j = 0; j < region->neighbourRegionsNum; j++)
			{
				const auto nRegion = region->neighbourRegions[j].region;
				const int nSettCount = nRegion->settlementCount();
				for (int n = 0; n < nSettCount; n++)
				{
					if (const auto sett = nRegion->getSettlement(n); sett->faction == faction)
						aiFaction->aiProductionControllers->evaluatePolicies(sett->regionID, sett->getMinorSettlementIndex());
				}
			}
			--regionControllersNum;
			aiRegionController** rc1;
			aiRegionController* rc2;
			for (int j = i; j < regionControllersNum; *rc1 = rc2 )
			{
				rc2 = regionControllers[j + 1];
				rc1 = &regionControllers[j++];
			}
			callClassFunc<aiRegionController*, void, int>(controller, 0, 1);
		}
	}
	for (int i = 0; i < ownRegionsCount; i++)
	{
		bool found = false;
		for (int j = 0; j < regionControllersNum; j++)
		{
			const auto controller = regionControllers[j];
			if ((controller->settlement->regionID == ownRegions[i].regionID)
				&& (controller->settlement->getMinorSettlementIndex() == ownRegions[i].settlementIndex))
			{
				ownRegions[i].regionController = controller;
				found = true;
				break;
			}
		}
		if (!found)
		{
			const auto sett = ownRegions[i].getSettlement();
			auto newController = createNewRegionController(aiFaction, sett);
			newController->gsdData = &ownRegions[i];
			GAME_FUNC(void(__thiscall*)(aiRegionController***, aiRegionController**),
				addToRegionControllers)(&regionControllers, &newController);
			ownRegions[i].regionController = newController;
			const auto region = stratMapHelpers::getRegion(sett->regionID);
			const int settlementCount = region->settlementCount();
			for (int j = 0; j < settlementCount; j++)
			{
				if (const auto sett2 = region->getSettlement(j); sett2->faction == faction)
					aiFaction->aiProductionControllers->evaluatePolicies(sett2->regionID, sett2->getMinorSettlementIndex());
			}
			for (int j = 0; j < region->neighbourRegionsNum; j++)
			{
				const auto nRegion = region->neighbourRegions[j].region;
				const int nSettCount = nRegion->settlementCount();
				for (int n = 0; n < nSettCount; n++)
				{
					if (const auto sett2 = nRegion->getSettlement(n); sett2->faction == faction)
						aiFaction->aiProductionControllers->evaluatePolicies(sett2->regionID, sett2->getMinorSettlementIndex());
				}
			}
		}
	}
	
	for( int i = 0; i < regionControllersNum; i++ )
		regionControllers[i]->initialize();
}

void aiMilitaryDirector::logData()
{
	gameHelpers::logStringGame("========================Military Director Data============================");
	gameHelpers::logStringGame("Faction: " + std::string(aiFaction->faction->factionRecord->facName));
	gameHelpers::logStringGame("Defensive campaigns: " + std::to_string(defensiveCampaigns.count));
	for (int i = 0; i < defensiveCampaigns.count; i++)
	{
		const auto campaign = defensiveCampaigns.campaigns[i];
		campaign->logData();
	}
	gameHelpers::logStringGame("Offensive campaigns: " + std::to_string(offensiveCampaigns.count));
	for (int i = 0; i < offensiveCampaigns.count; i++)
	{
		const auto campaign = offensiveCampaigns.campaigns[i];
		campaign->logData();
	}
	gameHelpers::logStringGame("Aiding campaigns: " + std::to_string(aidingCampaigns.count));
	for (int i = 0; i < aidingCampaigns.count; i++)
	{
		const auto campaign = aidingCampaigns.campaigns[i];
		campaign->logData();
	}
}

void aiGlobalStrategyDirector::initTargetRegions()
{
	clearRegionData(&targetRegions);
	const auto campaign = campaignHelpers::getCampaignData();
	if (faction->factionID == campaign->slaveFactionID)
		return;
	for (int i = 0; i < allNeighbourRegionsCount; i++)
	{
		const auto targetFaction = allNeighbourRegions[i].getTargetFaction();
		const int targetId = targetFaction->factionID;
		if ((stratFacData[targetId].activeWar && !aiFaction->ltgd->longTermGoalValues[targetId].wantPeace)
			|| targetId == campaign->slaveFactionID)
		{
			const auto regionData = &allNeighbourRegions[i];
			GAME_FUNC(void(__thiscall*)(aiRegionData**, aiRegionData*), addGsdRegion)(&targetRegions, regionData);
		}
	}
	for (int i = 0; i < navalInvasionTargetsNum; i++)
	{
		const auto regionData = &navalInvasionTargets[i].regionData;
		GAME_FUNC(void(__thiscall*)(aiRegionData**, aiRegionData*), addGsdRegion)(&targetRegions, regionData);
	}
	if (!faction->leader)
		return;
	if (targetRegionsCount == 0 && ownRegionsCount == 0 && faction->leader->isAlive && !faction->leader->isOffMap())
	{
		float closestEnemySett = 100000.f;
		settlementStruct* closestSett = nullptr;
		const auto leaderChar = faction->leader->gen;
		const auto leaderRegion = stratMapHelpers::getRegion(leaderChar->regionID);
		if (!leaderRegion->getEnemySettsToFaction(faction->factionID))
		{
			for (int i = 0; i < leaderRegion->neighbourRegionsNum; i++)
			{
				const auto nRegion = leaderRegion->neighbourRegions[i];
				if (nRegion.isBlocked || nRegion.region->isSea || !nRegion.region->settlement || !nRegion.region->getEnemySettsToFaction(faction->factionID))
					continue;
				const int settCount = nRegion.region->settlementCount();
				for (int j = 0; j < settCount; j++)
				{
					const auto sett = nRegion.region->getSettlement(j);
					if (campaign->getFactionDiplomacy(faction->factionID, sett->faction->factionID)->state <= dipStance::peace)
						continue;
					const auto distance = stratMapHelpers::getDistanceInTiles(leaderChar->xCoord, leaderChar->yCoord, sett->xCoord, sett->yCoord);
					if (distance < closestEnemySett)
					{
						closestEnemySett = distance;
						closestSett = sett;
					}
				}
			}
		}
		else
		{
			const int settCount = leaderRegion->settlementCount();
			for (int j = 0; j < settCount; j++)
			{
				const auto sett = leaderRegion->getSettlement(j);
				if (campaign->getFactionDiplomacy(faction->factionID, sett->faction->factionID)->state <= dipStance::peace)
					continue;
				const auto distance = stratMapHelpers::getDistanceInTiles(leaderChar->xCoord, leaderChar->yCoord, sett->xCoord, sett->yCoord);
				if (distance < closestEnemySett)
				{
					closestEnemySett = distance;
					closestSett = sett;
				}
			}
		}
		if (closestSett != nullptr)
		{
			const auto regionData = GAME_FUNC(aiRegionData*(__thiscall*)(aiRegionData**), createRegionInArray)(&targetRegions);
			const auto region = stratMapHelpers::getRegion(closestSett->regionID);
			regionData->navalTarget = false;
			regionData->isLastResort = false;
			regionData->regionController = nullptr;
			regionData->factionID = faction->factionID;
			regionData->regionID = closestSett->regionID;
			regionData->settlementIndex = closestSett->getMinorSettlementIndex();
			regionData->regionValue = 400;
			regionData->priority = 650;
			regionData->calculateRegionStrengths();
			regionData->setRisk(regionRisk::safe);
		}
	}
}


void aiGlobalStrategyDirector::initNavalRegions()
{
	const auto ltgd = aiFaction->ltgd;
	for (int i = 0; i < navalInvasionTargetsNum; i++)
	{
		const auto target = &navalInvasionTargets[i];
		target->active = false;
		target->highPriority = false;
		target->duration++;
	}
	if (ltgd->consideringNavalInvasion)
	{
		bool found = false;
		for (int i = 0; i < navalInvasionTargetsNum; i++)
		{
			if (navalInvasionTargets[i].regionData.regionID == ltgd->navalTargetRegionID)
			{
				navalInvasionTargets[i].highPriority = true;
				found = true;
			}
		}
		if (!found)
		{
			const auto invTarget = addToNavalTargets(&navalInvasionTargets);
			invTarget->stagingId = ltgd->stagingRegionID;
			invTarget->highPriority = true;
			invTarget->duration = 0;
			invTarget->regionData.factionID = faction->factionID;
			invTarget->regionData.regionID = ltgd->navalTargetRegionID;
			invTarget->regionData.navalTarget = true;
			invTarget->regionData.isLastResort = false;
			invTarget->regionData.regionController = nullptr;
		}
	}
	for (int i = 0; i < navalInvasionTargetsNum; i++)
	{
		const auto target = &navalInvasionTargets[i];
		const auto targetRegion = stratMapHelpers::getRegion(target->regionData.regionID);
		target->regionData.regionValue = calculateRegionValue(this, &target->regionData);
		if (targetRegion->factionOwner->factionRecord->slave)
			target->regionData.regionValue += campaignHelpers::getCampaignData()->turnNumber < 30 ? 500 : 250;
		if (faction->factionRecord->prefersNavalInvasions > 0)
		{
			target->regionData.regionValue += 100;
			if (faction->settlementsNum > 3)
				target->regionData.regionValue += 200;
		}
		if (target->regionData.regionValue < 0)
			target->regionData.regionValue = 0;
		target->regionData.priority = 650;
		target->regionData.calculateRegionStrengths();
		target->regionData.neighbourEnemyNum = -1;
		target->regionData.neighbourOtherNum = -1;
		target->regionData.setRisk(regionRisk::safe);
	}
	for (int i = navalInvasionTargetsNum - 1; i >= 0; --i)
	{
		const auto target = &navalInvasionTargets[i];
		const auto [ownStrength, ownCount, enemyStrength, enemyCount, neutralStrength, neutralCount] = target->regionData.strength;
		if (((ownStrength < ((neutralStrength + enemyStrength) >> 2)) && target->duration > 8 && !target->highPriority)
			|| !stratMapHelpers::getRegion(target->regionData.regionID)->getEnemySettsToFaction(faction->factionID))
		{
			removeNavalTarget(&navalInvasionTargets, i);
		}
		else if (ownStrength == 0 && target->regionData.neighboursStrength.ownStrength == 0)
		{
			if (target->duration > 8 && !target->highPriority)
				removeNavalTarget(&navalInvasionTargets, i);
			else
				target->active = true;
		}
	}
}
#pragma endregion aiGlobalStrategyDirector methods

/*--------------------------------------------------------------------------------------------------------------------*\
									   aiResourceManager methods
\*--------------------------------------------------------------------------------------------------------------------*/
#pragma region aiResourceManager methods

void aiResourceManager::releaseResource(aiResourcePrivate* res)
{
	GAME_FUNC(void(__thiscall*)(aiResourceManager*, aiResourcePrivate*), releaseResource)(this, res);
}
#pragma endregion aiResourceManager methods
