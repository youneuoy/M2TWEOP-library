#pragma once
#include "pluginM2TWEOP.h"
#include <unordered_map>
#include <functional>
using namespace std;

extern "C" PLUGINM2TWEOP_API void onReadGameDbsAtStart();
extern "C" PLUGINM2TWEOP_API void onEndSiege(int x, int y);
extern "C" PLUGINM2TWEOP_API void onStartSiege(int x, int y);


extern "C" PLUGINM2TWEOP_API void onClickAtTile(int x, int y);
extern "C" PLUGINM2TWEOP_API std::string * onSelectWorldpkgdesc(const char* selectedRec, const char* selectedGroup);
extern "C" PLUGINM2TWEOP_API int onfortificationlevelS(settlementStruct * settlement, bool* isCastle, bool* isChanged);
extern "C" PLUGINM2TWEOP_API void onChangeTurnNum(int num);
extern "C" PLUGINM2TWEOP_API void onNewGameStart();
extern "C" PLUGINM2TWEOP_API void onCampaignMapLoaded();
extern "C" PLUGINM2TWEOP_API void onLoadGamePl(std::vector<std::string>*saveFiles);
extern "C" PLUGINM2TWEOP_API std::vector<std::string>*onSaveGamePl(UNICODE_STRING * *&savePath);
extern "C" PLUGINM2TWEOP_API void onEventWrapper(DWORD eventAddr, DWORD * *vTab, DWORD arg2);

std::unordered_map<int, const char*> characterTypes = {
	{0,"spy"},
	{1,"assassin"},
	{2,"diplomat"},
	{3,"admiral"},
	{4,"merchant"},
	{5,"priest"},
	{6,"general"},
	{7,"named_character"},
	{8,"princess"},
	{9,"heretic"},
	{10,"witch"},
	{11,"inquisitor"},
	{13,"pope"}
};
std::unordered_map<int, const char*> disasterTypes = {
	{0,"earthquake"},
	{1,"flood"},
	{2,"horde"},
	{3,"storm"},
	{4,"volcano"},
	{5,"dustbowl"},
	{6,"locusts"},
	{7,"famine"},
	{8,"plague"},
	{9,"riot"},
	{10,"fire"}
};
std::unordered_map<int, const char*> ransomTypes = {
	{0,"ransom"},
	{1,"execute"},
	{2,"release"},
	{3,"cannot_pay_ransom"}
};
std::unordered_map<int, const char*> missionSuccessLvl = {
	{0,"not_successful"},
	{1,"slightly_successful"},
	{2,"partly_successful"},
	{3,"highly_successful"}
};
std::unordered_map<int, const char*> religionNames = {
};

struct crusade
{
public:
	char pad_0x0000[60]; //0x0000
	settlementStruct* targetSettlement; //0x003C
};

void checkLuaFunc(sol::function** lRef);
void initReligionNames();
namespace gameEvents
{
	regionStruct* getCharacterRegion(const namedCharacter* character);

	enum class EventType
	{
		namedCharacterEvent = 1,
		gameEvent = 2,
		crusadeEvent = 3,
		crusadeEventEnds = 4,
		factionEvent = 5,
		battlePlayerUnitAttackEvent = 6,
		battleAiUnitAttackEvent = 7,
		battlePlayerUnitEvent = 8,
		battleAiUnitEvent = 9,
		battlePlayerArmyUnitEvent = 10,
		battleArmyFromUnitEvent = 11,
		battleArmyFromUnitEvent2 = 12,
		battleUnitEvent = 13,
		armyEvent = 14,
		characterEvent = 15,
		battleUnitEvent2 = 16,
		diplomacyEvent = 17,
		diplomacyMoneyEvent = 18,
		namedCharacterTargetSettEvent = 19,
		disasterEvent = 20,
		namedCharacterDisasterEvent = 21,
		eventCounterEvent = 22,
		namedCharacterTargetCharEvent = 23,
		fortEvent = 24,
		tileEvent = 25,
		objEvent = 26,
		factionCrusadeEvent = 27,
		crusadeTargetEvent = 28,
		transgressionEvent = 29,
		stratUnitEvent = 30,
		namedCharacterSettEvent = 31,
		namedCharacterCrusadeEvent = 32,
		ransomEvent = 33,
		spyMissionEvent = 34,
		orderSpyMissionEvent = 35,
		agentMissionEvent = 36,
		orderKillMissionEvent = 37,
		diplomacyMissionEvent = 38,
		missionEvent = 39,
		settlementEvent = 40,
		stratUnitSettEvent = 41,
		characterUnitSettEvent = 42,
		characterBuildingEvent = 43,
		agentCreatedEvent = 44,
		buildingEvent = 45,
		targetSettlementEvent = 46,
		namedCharacterRiotEvents = 47,
		settOccupationEvent = 48,
		guildEvent = 49,
		uiEvent = 50,
		factionUiEvent = 51,
		buildingCompletedEvent = 52,
		closeMessageEvent = 53
	};

	class EventBase
	{
	public:
		virtual ~EventBase() = default;
		virtual int callEvent(DWORD** vTab) = 0;
	};

	template<EventType EvType> class Event :public EventBase
	{
	public:

		sol::function* funk;

		Event(const std::string& luaFunctionName, sol::state& luaState)
			: funk(new sol::function(luaState[luaFunctionName])) {}

		static const char* getType(const namedCharacter* character)
		{
			if (character->gen != nullptr)
			{
				const auto charType = characterTypes.find(character->gen->genType->type);
				if (charType != characterTypes.end())
				{
					return charType->second;
				}
			}
			return "unknown";
		}
		static const char* getTypeInteger(int type)
		{
			const auto charType = characterTypes.find(type);
			if (charType != characterTypes.end())
			{
				return charType->second;
			}
			return "unknown";
		}
		static const regionStruct* getRegion(const namedCharacter* character)
		{
			if (character->gen != nullptr)
			{
				return getCharacterRegion(character);
			}
			return nullptr;
		}
		static const char* getReligion(const int index)
		{
			if (index == -1)
			{
				return "unknown";
			}
			const auto name = religionNames.find(index);
			if (name != religionNames.end())
			{
				return name->second;
			}
			return "unknown";
		}
		int callEvent(DWORD** vTab) override
		{
			if (EvType == EventType::namedCharacterEvent)
			{
				auto character = reinterpret_cast<namedCharacter*>(vTab[1]);
				factionStruct* faction = character->faction;
				const char* characterType = getType(character);
				const regionStruct* region = getRegion(character);
				const char* religion = getReligion(faction->religion);
				if (&(*funk) != nullptr) {
					(*funk)(character, faction, region, characterType, religion);
				}
				return 1;
			}
			else if (EvType == EventType::gameEvent)
			{
				if (&(*funk) != nullptr) {
					(*funk)();
				}
				return 2;
			}
			else if (EvType == EventType::crusadeEvent)
			{
				auto targetSettlement = reinterpret_cast<settlementStruct*>(vTab[2]);
				factionStruct* targetFaction = targetSettlement->faction;
				regionStruct* targetRegion = gameHelpers::getRegion(targetSettlement->regionID);
				const char* targetReligion = getReligion(targetFaction->religion);
				if (&(*funk) != nullptr) {
					(*funk)(targetSettlement, targetFaction, targetRegion, targetReligion);
				}
				return 3;
			}
			else if (EvType == EventType::crusadeEventEnds)
			{
				auto targetSettlement = reinterpret_cast<settlementStruct*>(vTab[2]);
				regionStruct* targetRegion = gameHelpers::getRegion(targetSettlement->regionID);
				if (&(*funk) != nullptr) {
					(*funk)(targetSettlement, targetRegion);
				}
				return 4;
			}
			else if (EvType == EventType::factionEvent)
			{
				auto faction = reinterpret_cast<factionStruct*>(vTab[1]);
				const char* religion = getReligion(faction->religion);
				if (&(*funk) != nullptr) {
					(*funk)(faction, religion);
				}
				return 5;
			}
			else if (EvType == EventType::battlePlayerUnitAttackEvent)
			{
				auto attackingUnit = reinterpret_cast<unit*>(vTab[1]);
				auto defendingUnit = reinterpret_cast<unit*>(vTab[2]);
				auto playerUnit = reinterpret_cast<unit*>(vTab[1]);
				auto enemyUnit = reinterpret_cast<unit*>(vTab[2]);
				if (&(*funk) != nullptr) {
					(*funk)(attackingUnit, defendingUnit, playerUnit, enemyUnit);
				}
				return 6;
			}
			else if (EvType == EventType::battleAiUnitAttackEvent)
			{
				auto attackingUnit = reinterpret_cast<unit*>(vTab[1]);
				auto defendingUnit = reinterpret_cast<unit*>(vTab[2]);
				auto playerUnit = reinterpret_cast<unit*>(vTab[2]);
				auto enemyUnit = reinterpret_cast<unit*>(vTab[1]);
				if (&(*funk) != nullptr) {
					(*funk)(attackingUnit, defendingUnit, playerUnit, enemyUnit);
				}
				return 7;
			}
			else if (EvType == EventType::battlePlayerUnitEvent)
			{
				auto playerUnit = reinterpret_cast<unit*>(vTab[1]);
				if (&(*funk) != nullptr) {
					(*funk)(playerUnit);
				}
				return 8;
			}
			else if (EvType == EventType::battleAiUnitEvent)
			{
				auto enemyUnit = reinterpret_cast<unit*>(vTab[1]);
				if (&(*funk) != nullptr) {
					(*funk)(enemyUnit);
				}
				return 9;
			}
			else if (EvType == EventType::battlePlayerArmyUnitEvent)
			{
				auto playerUnit = reinterpret_cast<unit*>(vTab[20]);
				auto army = reinterpret_cast<stackStruct*>(vTab[1]);
				if (&(*funk) != nullptr) {
					(*funk)(army, playerUnit);
				}
				return 10;
			}
			else if (EvType == EventType::battleArmyFromUnitEvent)
			{
				auto playerUnit = reinterpret_cast<unit*>(vTab[1]);
				stackStruct* army = playerUnit->army;
				if (&(*funk) != nullptr) {
					(*funk)(army);
				}
				return 11;
			}
			else if (EvType == EventType::battleArmyFromUnitEvent2)
			{
				auto playerUnit = reinterpret_cast<unit*>(vTab[2]);
				stackStruct* army = playerUnit->army;
				if (&(*funk) != nullptr) {
					(*funk)(army);
				}
				return 12;
			}
			else if (EvType == EventType::battleUnitEvent)
			{
				auto playerUnit = reinterpret_cast<unit*>(vTab[1]);
				if (&(*funk) != nullptr) {
					(*funk)(playerUnit);
				}
				return 13;
			}
			else if (EvType == EventType::armyEvent)
			{
				auto army = reinterpret_cast<stackStruct*>(vTab[1]);
				if (&(*funk) != nullptr) {
					(*funk)(army);
				}
				return 14;
			}
			else if (EvType == EventType::characterEvent)
			{
				auto gen = reinterpret_cast<general*>(vTab[1]);
				namedCharacter* namedChar = gen->genChar;
				if (&(*funk) != nullptr) {
					(*funk)(gen, namedChar);
				}
				return 15;
			}
			else if (EvType == EventType::battleUnitEvent2)
			{
				auto playerUnit = reinterpret_cast<unit*>(vTab[2]);
				if (&(*funk) != nullptr) {
					(*funk)(playerUnit);
				}
				return 16;
			}
			else if (EvType == EventType::diplomacyEvent)
			{
				auto faction = reinterpret_cast<factionStruct*>(vTab[1]);
				const char* religion = getReligion(faction->religion);
				auto targetFaction = reinterpret_cast<factionStruct*>(vTab[2]);
				const char* targetReligion = getReligion(targetFaction->religion);
				if (&(*funk) != nullptr) {
					(*funk)(faction, religion, targetFaction, targetReligion);
				}
				return 17;
			}
			else if (EvType == EventType::diplomacyMoneyEvent)
			{
				auto faction = reinterpret_cast<factionStruct*>(vTab[1]);
				const char* religion = getReligion(faction->religion);
				auto targetFaction = reinterpret_cast<factionStruct*>(vTab[2]);
				const char* targetReligion = getReligion(targetFaction->religion);
				float amount = *reinterpret_cast<float*>((DWORD)&vTab[3]);
				if (&(*funk) != nullptr) {
					(*funk)(faction, religion, targetFaction, targetReligion, amount);
				}
				return 18;
			}
			else if (EvType == EventType::namedCharacterTargetSettEvent)
			{
				auto character = reinterpret_cast<namedCharacter*>(vTab[1]);
				auto settlement = reinterpret_cast<settlementStruct*>(vTab[2]);
				fortStruct* fort = nullptr;
				factionStruct* faction = character->faction;
				factionStruct* targetFaction = settlement->faction;
				if ((DWORD)(*settlement).vTable != 0x012FCF34 && (DWORD)(*settlement).vTable != 0x01341F54)
				{
					fort = reinterpret_cast<fortStruct*>(vTab[2]);
					targetFaction = fort->faction;
				}
				const char* characterType = getType(character);
				regionStruct* region = gameHelpers::getRegion(settlement->regionID);
				const char* religion = getReligion(faction->religion);
				const char* targetReligion = getReligion(targetFaction->religion);
				if (&(*funk) != nullptr) {
					if (!fort)
					{
						(*funk)(character, settlement, faction, targetFaction, region, characterType, religion, targetReligion, false);
					}
					else
					{
						(*funk)(character, fort, faction, targetFaction, region, characterType, religion, targetReligion, true);
					}
				}
				return 19;
			}
			else if (EvType == EventType::disasterEvent)
			{
				auto eventType = "unknown";
				if (disasterTypes.find(reinterpret_cast<int>(vTab[1])) != disasterTypes.end()) {
					eventType = disasterTypes.at(reinterpret_cast<int>(vTab[1]));
				}
				if (&(*funk) != nullptr) {
					(*funk)(eventType);
				}
				return 20;
			}
			else if (EvType == EventType::namedCharacterDisasterEvent)
			{
				auto character = reinterpret_cast<namedCharacter*>(vTab[1]);
				factionStruct* faction = character->faction;
				const char* characterType = getType(character);
				const regionStruct* region = getRegion(character);
				const char* religion = getReligion(faction->religion);
				auto eventType = "unknown";
				if (disasterTypes.find(reinterpret_cast<int>(vTab[2])) != disasterTypes.end()) {
					eventType = disasterTypes.at(reinterpret_cast<int>(vTab[2]));
				}
				if (&(*funk) != nullptr) {
					(*funk)(eventType, character, faction, region, characterType, religion);
				}
				return 21;
			}
			else if (EvType == EventType::eventCounterEvent)
			{

				const char* counter = reinterpret_cast<char*>(vTab[1]);
				if (&(*funk) != nullptr) {
					(*funk)(counter);
				}
				return 22;
			}
			else if (EvType == EventType::namedCharacterTargetCharEvent)
			{
				auto character = reinterpret_cast<namedCharacter*>(vTab[1]);
				auto targetCharacter = reinterpret_cast<namedCharacter*>(vTab[2]);
				factionStruct* faction = character->faction;
				factionStruct* targetFaction = character->faction;
				const char* characterType = getType(character);
				const regionStruct* region = getRegion(character);
				const char* religion = getReligion(faction->religion);
				const char* targetCharacterType = getType(targetCharacter);
				const char* targetReligion = getReligion(targetFaction->religion);
				if (&(*funk) != nullptr) {

					(*funk)(character, targetCharacter, faction, targetFaction, region, characterType, targetCharacterType, religion, targetReligion);
				}
				return 23;
			}
			else if (EvType == EventType::fortEvent)
			{
				auto fort = reinterpret_cast<fortStruct*>(vTab[1]);
				factionStruct* faction = fort->faction;
				regionStruct* region = gameHelpers::getRegion(fort->regionID);
				const char* religion = getReligion(faction->religion);
				if (&(*funk) != nullptr) {
					(*funk)(fort, faction, region, religion);
				}
				return 24;
			}
			else if (EvType == EventType::tileEvent)
			{
				auto faction = reinterpret_cast<factionStruct*>(vTab[1]);
				int x = reinterpret_cast<int>(vTab[2]);
				int y = reinterpret_cast<int>(vTab[3]);
				regionStruct* region = gameHelpers::getRegion(m2tweopHelpers::getTileRegionID(x, y));
				const char* religion = getReligion(faction->religion);
				if (&(*funk) != nullptr) {
					(*funk)(faction, x, y, region, religion);
				}
				return 25;
			}
			else if (EvType == EventType::objEvent)
			{
				auto faction = reinterpret_cast<factionStruct*>(vTab[1]);
				auto targetFaction = reinterpret_cast<factionStruct*>(vTab[2]);
				int x = reinterpret_cast<int>(vTab[3]);
				int y = reinterpret_cast<int>(vTab[4]);
				regionStruct* region = gameHelpers::getRegion(m2tweopHelpers::getTileRegionID(x, y));
				const char* religion = getReligion(faction->religion);
				if (&(*funk) != nullptr) {
					(*funk)(faction, targetFaction, x, y, region, religion);
				}
				return 26;
			}
			else if (EvType == EventType::factionCrusadeEvent)
			{
				auto targetSettlement = reinterpret_cast<settlementStruct*>(vTab[2]);
				auto faction = reinterpret_cast<factionStruct*>(vTab[3]);
				factionStruct* targetFaction = targetSettlement->faction;
				regionStruct* targetRegion = gameHelpers::getRegion(targetSettlement->regionID);
				const char* religion = getReligion(faction->religion);
				const char* targetReligion = getReligion(targetFaction->religion);
				if (&(*funk) != nullptr) {
					(*funk)(targetSettlement, faction, targetFaction, targetRegion, religion, targetReligion);
				}
				return 27;
			}
			else if (EvType == EventType::factionCrusadeEvent)
			{
				auto targetSettlement = reinterpret_cast<settlementStruct*>(vTab[2]);
				auto army = reinterpret_cast<stackStruct*>(vTab[3]);
				factionStruct* faction = army->faction;
				factionStruct* targetFaction = targetSettlement->faction;
				regionStruct* targetRegion = gameHelpers::getRegion(targetSettlement->regionID);
				const char* religion = getReligion(faction->religion);
				const char* targetReligion = getReligion(targetFaction->religion);
				if (&(*funk) != nullptr) {
					(*funk)(targetSettlement, army, faction, targetFaction, targetRegion, religion, targetReligion);
				}
				return 28;
			}
			else if (EvType == EventType::transgressionEvent)
			{
				auto faction = reinterpret_cast<factionStruct*>(vTab[1]);
				auto targetFaction = reinterpret_cast<factionStruct*>(vTab[4]);
				const char* transgression = reinterpret_cast<char*>(vTab[2]);
				const char* religion = getReligion(faction->religion);
				const char* targetReligion = getReligion(targetFaction->religion);
				if (&(*funk) != nullptr) {
					(*funk)(faction, targetFaction, transgression, religion, targetReligion);
				}
				return 29;
			}
			else if (EvType == EventType::stratUnitEvent)
			{
				auto faction = reinterpret_cast<factionStruct*>(vTab[1]);
				auto playerUnit = reinterpret_cast<unit*>(vTab[2]);
				eduEntry* edu = playerUnit->eduEntry;
				const char* religion = getReligion(faction->religion);
				if (&(*funk) != nullptr) {
					(*funk)(faction, playerUnit, edu, religion);
				}
				return 30;
			}
			else if (EvType == EventType::namedCharacterSettEvent)
			{
				auto character = reinterpret_cast<namedCharacter*>(vTab[1]);
				auto settlement = reinterpret_cast<settlementStruct*>(vTab[2]);
				factionStruct* faction = character->faction;
				const char* characterType = getType(character);
				regionStruct* region;
				if (settlement)
				{
					region = gameHelpers::getRegion(settlement->regionID);
				}
				else
				{
					region = const_cast<regionStruct*>(getRegion(character));
				}
				const char* religion = getReligion(faction->religion);
				if (&(*funk) != nullptr) {
					(*funk)(character, settlement, faction, region, characterType, religion);
				}
				return 31;
			}
			else if (EvType == EventType::namedCharacterCrusadeEvent)
			{
				auto character = reinterpret_cast<namedCharacter*>(vTab[1]);
				factionStruct* faction = character->faction;
				auto crusadeActive = reinterpret_cast<crusade*>(vTab[2]);
				auto targetSettlement = crusadeActive->targetSettlement;
				factionStruct* targetFaction = targetSettlement->faction;
				regionStruct* targetRegion = gameHelpers::getRegion(targetSettlement->regionID);
				const char* characterType = getType(character);
				const char* religion = getReligion(faction->religion);
				const char* targetReligion = getReligion(targetFaction->religion);
				if (&(*funk) != nullptr) {
					(*funk)(character, faction, targetSettlement, targetFaction, targetRegion, characterType, religion, targetReligion);
				}
				return 32;
			}
			else if (EvType == EventType::ransomEvent)
			{
				auto character = reinterpret_cast<namedCharacter*>(vTab[1]);
				auto capInfo = reinterpret_cast<capturedFactionInfo*>(vTab[2]);
				auto ransomType = "unknown";
				if (ransomTypes.find(reinterpret_cast<int>(vTab[3])) != ransomTypes.end()) {
					ransomType = ransomTypes.at(reinterpret_cast<int>(vTab[3]));
				}
				factionStruct* faction = character->faction;
				factionStruct* targetFaction = gameHelpers::getFaction(capInfo->targetFactionID);
				const regionStruct* region = getRegion(character);
				const char* characterType = getType(character);
				const char* religion = getReligion(faction->religion);
				const char* targetReligion = getReligion(targetFaction->religion);
				if (&(*funk) != nullptr) {
					(*funk)(character, capInfo, ransomType, faction, targetFaction, region, characterType, religion, targetReligion);
				}
				return 33;
			}
			else if (EvType == EventType::spyMissionEvent)
			{
				auto character = reinterpret_cast<namedCharacter*>(vTab[1]);
				factionStruct* faction = character->faction;
				auto targetFaction = reinterpret_cast<factionStruct*>(vTab[2]);
				auto missionSuccess = "unknown";
				if (missionSuccessLvl.find(reinterpret_cast<int>(vTab[3])) != missionSuccessLvl.end()) {
					missionSuccess = missionSuccessLvl.at(reinterpret_cast<int>(vTab[3]));
				}
				int missionProbability = reinterpret_cast<int>(vTab[4]);
				settlementStruct* targetSettlement = nullptr;
				fortStruct* targetFort = nullptr;
				if (vTab[5] != nullptr)
				{
					targetSettlement = reinterpret_cast<settlementStruct*>(vTab[5]);
					if ((DWORD)(*targetSettlement).vTable != 0x012FCF34 && (DWORD)(*targetSettlement).vTable != 0x01341F54)
					{
						targetFort = reinterpret_cast<fortStruct*>(vTab[5]);
					}
				}
				const regionStruct* region = getRegion(character);
				const char* characterType = getType(character);
				const char* religion = getReligion(faction->religion);
				const char* targetReligion = getReligion(targetFaction->religion);
				if (&(*funk) != nullptr) {
					if (!targetFort)
					{
						(*funk)(character, missionSuccess, missionProbability, targetSettlement, faction, targetFaction, region, characterType, religion, targetReligion);
					}
					else
					{
						(*funk)(character, missionSuccess, missionProbability, targetFort, faction, targetFaction, region, characterType, religion, targetReligion);
					}
				}
				return 34;
			}
			else if (EvType == EventType::orderSpyMissionEvent)
			{
				auto character = reinterpret_cast<namedCharacter*>(vTab[1]);
				factionStruct* faction = character->faction;
				auto targetFaction = reinterpret_cast<factionStruct*>(vTab[2]);
				auto missionSuccess = "unknown";
				if (missionSuccessLvl.find(reinterpret_cast<int>(vTab[3])) != missionSuccessLvl.end()) {
					missionSuccess = missionSuccessLvl.at(reinterpret_cast<int>(vTab[3]));
				}
				settlementStruct* targetSettlement = nullptr;
				fortStruct* targetFort = nullptr;
				if (vTab[4] != nullptr)
				{
					targetSettlement = reinterpret_cast<settlementStruct*>(vTab[4]);
					if ((DWORD)(*targetSettlement).vTable != 0x012FCF34 && (DWORD)(*targetSettlement).vTable != 0x01341F54)
					{
						targetFort = reinterpret_cast<fortStruct*>(vTab[4]);
					}
				}
				const regionStruct* region = getRegion(character);
				const char* characterType = getType(character);
				const char* religion = getReligion(faction->religion);
				const char* targetReligion = getReligion(targetFaction->religion);
				if (&(*funk) != nullptr) {
					if (!targetFort)
					{
						(*funk)(character, missionSuccess, targetSettlement, faction, targetFaction, region, characterType, religion, targetReligion);
					}
					else
					{
						(*funk)(character, missionSuccess, targetFort, faction, targetFaction, region, characterType, religion, targetReligion);
					}
				}
				return 35;
			}
			else if (EvType == EventType::agentMissionEvent)
			{
				auto character = reinterpret_cast<namedCharacter*>(vTab[1]);
				factionStruct* faction = character->faction;
				auto missionSuccess = "unknown";
				if (missionSuccessLvl.find(reinterpret_cast<int>(vTab[2])) != missionSuccessLvl.end()) {
					missionSuccess = missionSuccessLvl.at(reinterpret_cast<int>(vTab[2]));
				}
				int missionProbability = reinterpret_cast<int>(vTab[3]);
				const regionStruct* region = getRegion(character);
				const char* characterType = getType(character);
				const char* religion = getReligion(faction->religion);
				if (&(*funk) != nullptr) {

					(*funk)(character, missionSuccess, missionProbability, faction, region, characterType, religion);

				}
				return 36;
			}
			else if (EvType == EventType::orderKillMissionEvent)
			{
				auto character = reinterpret_cast<namedCharacter*>(vTab[1]);
				factionStruct* faction = character->faction;
				auto targetCharacter = reinterpret_cast<namedCharacter*>(vTab[2]);
				auto missionSuccess = "unknown";
				if (missionSuccessLvl.find(reinterpret_cast<int>(vTab[3])) != missionSuccessLvl.end()) {
					missionSuccess = missionSuccessLvl.at(reinterpret_cast<int>(vTab[3]));
				}
				const regionStruct* region = getRegion(character);
				const char* characterType = getType(character);
				const char* religion = getReligion(faction->religion);
				if (&(*funk) != nullptr) {

					(*funk)(character, missionSuccess, targetCharacter, faction, region, characterType, religion);

				}
				return 37;
			}
			else if (EvType == EventType::diplomacyMissionEvent)
			{
				auto character = reinterpret_cast<namedCharacter*>(vTab[1]);
				factionStruct* faction = character->faction;
				auto targetFaction = reinterpret_cast<factionStruct*>(vTab[2]);
				auto missionSuccess = "unknown";
				if (missionSuccessLvl.find(reinterpret_cast<int>(vTab[3])) != missionSuccessLvl.end()) {
					missionSuccess = missionSuccessLvl.at(reinterpret_cast<int>(vTab[3]));
				}
				const regionStruct* region = getRegion(character);
				const char* characterType = getType(character);
				const char* religion = getReligion(faction->religion);
				const char* targetReligion = getReligion(targetFaction->religion);
				if (&(*funk) != nullptr) {

					(*funk)(character, missionSuccess, faction, targetFaction, region, characterType, religion, targetReligion);

				}
				return 38;
			}
			else if (EvType == EventType::missionEvent)
			{
				auto character = reinterpret_cast<namedCharacter*>(vTab[1]);
				factionStruct* faction = character->faction;
				const char* missionName = reinterpret_cast<char*>(vTab[2]);
				const regionStruct* region = getRegion(character);
				const char* characterType = getType(character);
				const char* religion = getReligion(faction->religion);
				if (&(*funk) != nullptr) {

					(*funk)(character, faction, missionName, region, characterType, religion);

				}
				return 39;
			}
			else if (EvType == EventType::settlementEvent)
			{
				auto settlement = reinterpret_cast<settlementStruct*>(vTab[1]);
				fortStruct* fort;
				factionStruct* faction = settlement->faction;
				regionStruct* region = gameHelpers::getRegion(settlement->regionID);
				bool isFort = false;
				if ((DWORD)(*settlement).vTable != 0x012FCF34 && (DWORD)(*settlement).vTable != 0x01341F54)
				{
					fort = (*(*plugData::data.funcs.findFort))(settlement->xCoord, settlement->yCoord);
					isFort = true;
					faction = fort->faction;
					region = gameHelpers::getRegion(fort->regionID);
				}
				const char* religion = getReligion(faction->religion);
				if (&(*funk) != nullptr) {
					if (!isFort)
					{
						(*funk)(settlement, faction, region, religion);
					}
					else
					{
						(*funk)(fort, faction, region, religion);
					}
				}
				return 40;
			}
			else if (EvType == EventType::stratUnitSettEvent)
			{
				auto faction = reinterpret_cast<factionStruct*>(vTab[1]);
				auto settlement = reinterpret_cast<settlementStruct*>(vTab[2]);
				auto playerUnit = reinterpret_cast<unit*>(vTab[3]);
				eduEntry* edu = playerUnit->eduEntry;
				const char* religion = getReligion(faction->religion);
				if (&(*funk) != nullptr) {
					(*funk)(faction, settlement, playerUnit, edu, religion);
				}
				return 41;
			}
			else if (EvType == EventType::characterUnitSettEvent)
			{
				auto character = reinterpret_cast<namedCharacter*>(vTab[1]);
				factionStruct* faction = character->faction;
				auto settlement = reinterpret_cast<settlementStruct*>(vTab[2]);
				auto playerUnit = reinterpret_cast<unit*>(vTab[3]);
				const regionStruct* region = getRegion(character);
				eduEntry* edu = playerUnit->eduEntry;
				const char* characterType = getType(character);
				const char* religion = getReligion(faction->religion);
				if (&(*funk) != nullptr) {
					(*funk)(character, faction, settlement, playerUnit, edu, region, characterType, religion);
				}
				return 42;
			}
			else if (EvType == EventType::characterBuildingEvent)
			{
				auto character = reinterpret_cast<namedCharacter*>(vTab[1]);
				factionStruct* faction = character->faction;
				auto settlement = reinterpret_cast<settlementStruct*>(vTab[2]);
				building* building = settlement->buildings[settlement->buildingsNum - 1];
				const regionStruct* region = getRegion(character);
				const char* characterType = getType(character);
				const char* religion = getReligion(faction->religion);
				if (&(*funk) != nullptr) {
					(*funk)(character, faction, settlement, building, region, characterType, religion);
				}
				return 43;
			}
			else if (EvType == EventType::agentCreatedEvent)
			{
				auto character = reinterpret_cast<namedCharacter*>(vTab[1]);
				factionStruct* faction = character->faction;
				auto settlement = reinterpret_cast<settlementStruct*>(vTab[3]);
				const char* characterType = getTypeInteger(reinterpret_cast<int>(vTab[2]));
				regionStruct* region = gameHelpers::getRegion(settlement->regionID);
				const char* religion = getReligion(faction->religion);
				if (&(*funk) != nullptr) {
					(*funk)(character, faction, settlement, region, characterType, religion);
				}
				return 44;
			}
			else if (EvType == EventType::buildingEvent)
			{
				auto settlement = reinterpret_cast<settlementStruct*>(vTab[1]);
				factionStruct* faction = settlement->faction;
				regionStruct* region = gameHelpers::getRegion(settlement->regionID);
				const char* build = reinterpret_cast<char*>(vTab[2]);
				const char* religion = getReligion(faction->religion);
				if (&(*funk) != nullptr) {
					(*funk)(settlement, faction, build, region, religion);
				}
				return 45;
			}
			else if (EvType == EventType::targetSettlementEvent)
			{
				auto settlement = reinterpret_cast<settlementStruct*>(vTab[1]);
				factionStruct* faction = settlement->faction;
				factionStruct* targetFaction = reinterpret_cast<factionStruct*>(vTab[2]);
				regionStruct* region = gameHelpers::getRegion(settlement->regionID);
				const char* religion = getReligion(faction->religion);
				const char* targetReligion = getReligion(targetFaction->religion);
				if (&(*funk) != nullptr) {
					(*funk)(settlement, faction, region, religion, targetFaction, targetReligion);
				}
				return 46;
			}
			else if (EvType == EventType::namedCharacterRiotEvents)
			{
				auto character = reinterpret_cast<namedCharacter*>(vTab[1]);
				factionStruct* faction = character->faction;
				auto settlement = reinterpret_cast<settlementStruct*>(vTab[2]);
				regionStruct* region = gameHelpers::getRegion(settlement->regionID);
				const char* religion = getReligion(faction->religion);
				factionStruct* targetFaction = reinterpret_cast<factionStruct*>(vTab[3]);
				const char* targetReligion = getReligion(targetFaction->religion);
				if (&(*funk) != nullptr) {
					(*funk)(character, faction, settlement, region, religion, targetFaction, targetReligion);
				}
				return 47;
			}
			else if (EvType == EventType::settOccupationEvent)
			{
				auto character = reinterpret_cast<namedCharacter*>(vTab[1]);
				factionStruct* faction = character->faction;
				const regionStruct* region = getRegion(character);
				const char* religion = getReligion(faction->religion);
				factionStruct* targetFaction = reinterpret_cast<factionStruct*>(vTab[2]);
				const char* targetReligion = getReligion(targetFaction->religion);
				if (&(*funk) != nullptr) {
					(*funk)(character, faction, region, religion, targetFaction, targetReligion);
				}
				return 48;
			}
			else if (EvType == EventType::guildEvent)
			{
				auto settlement = reinterpret_cast<settlementStruct*>(vTab[1]);
				factionStruct* faction = settlement->faction;
				regionStruct* region = gameHelpers::getRegion(settlement->regionID);
				const char* religion = getReligion(faction->religion);
				auto guild_handler = reinterpret_cast< guild* >(vTab[2]);
				if (&(*funk) != nullptr) {

					(*funk)(settlement, faction, region, religion, guild_handler);

				}
				return 49;
			}
			else if (EvType == EventType::uiEvent)
			{
				auto name = reinterpret_cast<char*>(vTab[1]);
				if (&(*funk) != nullptr) {

					(*funk)(name);

				}
				return 50;
			}
			else if (EvType == EventType::factionUiEvent)
			{
				auto faction = reinterpret_cast<factionStruct*>(vTab[1]);
				int name = reinterpret_cast<int>(vTab[2]);
				const char* religion = getReligion(faction->religion);
				if (&(*funk) != nullptr) {
					(*funk)(faction, religion, name);
				}
				return 51;
			}
			else if (EvType == EventType::buildingCompletedEvent)
			{
				auto faction = reinterpret_cast<factionStruct*>(vTab[1]);
				auto settlement = reinterpret_cast<settlementStruct*>(vTab[2]);
				regionStruct* region = gameHelpers::getRegion(settlement->regionID);
				building* building = settlement->buildings[settlement->buildingsNum - 1];
				const char* religion = getReligion(faction->religion);
				if (&(*funk) != nullptr) {
					(*funk)(settlement, faction, building, region, religion);
				}
				return 52;
			}
			else if (EvType == EventType::closeMessageEvent)
			{
				int name = reinterpret_cast<int>(vTab[1]);
				if (&(*funk) != nullptr) {
					(*funk)(name);
				}
				return 53;
			}
			else
			{
				return 0;
			}
		}
	};

	std::unordered_map<DWORD, std::unique_ptr<EventBase>> events;
	void initEvents();
	template<EventType EvType> void addEvent(DWORD key, const char* name);
}

/*
*/
sol::function* onCharacterTurnStart = nullptr;
sol::function* onCapturedCharacterRansomed = nullptr;
sol::function* onCapturedCharacterReleased = nullptr;
sol::function* onPreBattleWithdrawal = nullptr;
sol::function* onPostBattle = nullptr;
sol::function* onAbandonShowMe = nullptr;
sol::function* onGameReloaded = nullptr;
sol::function* onCrusadeCalled = nullptr;
sol::function* onPopeAcceptsCrusadeTarget = nullptr;
sol::function* onPopeRejectsCrusadeTarget = nullptr;
sol::function* onCrusadeEnds = nullptr;
sol::function* onBattleAiCommenced = nullptr;
sol::function* onBattleDelayPhaseCommenced = nullptr;
sol::function* onBattleDeploymentPhaseCommenced = nullptr;
sol::function* onBattleConflictPhaseCommenced = nullptr;
sol::function* onBattlePlayerUnitAttacksEnemyUnit = nullptr;
sol::function* onBattlePlayerUnitGoesBerserk = nullptr;
sol::function* onBattlePlayerUnitRouts = nullptr;
sol::function* onBattlePlayerSiegeEngineDestroyed = nullptr;
sol::function* onBattleGatesAttackedByPlayerEngine = nullptr;
sol::function* onBattleEnemyUnitAttacksPlayerUnit = nullptr;
sol::function* onBattleEnemyUnitGoesBerserk = nullptr;
sol::function* onBattnemyUnitRouts = nullptr;
sol::function* onBattnemySiegeEngineDestroyed = nullptr;
sol::function* onBattleGatesAttackedByEnemyEngine = nullptr;
sol::function* onBattleWinningPlaza = nullptr;
sol::function* onBattleStopsWinningPlaza = nullptr;
sol::function* onBattleDominatingPlaza = nullptr;
sol::function* onBattngineUnmanned = nullptr;
sol::function* onBattlePlayerArmyHalfDestroyed = nullptr;
sol::function* onBattnemyArmyHalfDestroyed = nullptr;
sol::function* onBattlePlayerUnderAttackIdle = nullptr;
sol::function* onBattleWinningCombat = nullptr;
sol::function* onBattleArmyTired = nullptr;
sol::function* onBattleTideofBattle = nullptr;
sol::function* onBattleUnitGoesBerserk = nullptr;
sol::function* onBattleArmyRouted = nullptr;
sol::function* onBattleSiegeEngineDestroyed = nullptr;
sol::function* onBattleReinforcementsArrive = nullptr;
sol::function* onBattleSiegeEngineDocksWall = nullptr;
sol::function* onBattleGatesAttackedByEngine = nullptr;
sol::function* onBattleGatesDestroyedByEngine = nullptr;
sol::function* onBattleWallsBreachedByEngine = nullptr;
sol::function* onBattleWallsCaptured = nullptr;
sol::function* onBattleFinished = nullptr;
sol::function* onBattleArmyHalfDestroyed = nullptr;
sol::function* onBattleUnitRouts = nullptr;
sol::function* onBattleGeneralKilled = nullptr;
sol::function* onFactionTurnStart = nullptr;
sol::function* onFactionNewCapital = nullptr;
sol::function* onPreFactionTurnStart = nullptr;
sol::function* onFactionWarDeclared = nullptr;
sol::function* onFactionAllianceDeclared = nullptr;
sol::function* onInterFactionMarriage = nullptr;
sol::function* onFactionTradeAgreementMade = nullptr;
sol::function* onGiveMoney = nullptr;
sol::function* onPopeElected = nullptr;
sol::function* onVotedForPope = nullptr;
sol::function* onFactionExcommunicated = nullptr;
sol::function* onFactionBreakAlliance = nullptr;
sol::function* onHordeFormed = nullptr;
sol::function* onGeneralCaptureResidence = nullptr;
sol::function* onGeneralCaptureSettlement = nullptr;
sol::function* onLeaderDestroyedFaction = nullptr;
sol::function* onDisaster = nullptr;
sol::function* onEventCounter = nullptr;
sol::function* onGeneralAssaultsResidence = nullptr;
sol::function* onGeneralAssaultsGeneral = nullptr;
sol::function* onOfferedForAdoption = nullptr;
sol::function* onLesserGeneralOfferedForAdoption = nullptr;
sol::function* onOfferedForMarriage = nullptr;
sol::function* onBrotherAdopted = nullptr;
sol::function* onFatherDiesNatural = nullptr;
sol::function* onBecomesFactionLeader = nullptr;
sol::function* onBecomesFactionHeir = nullptr;
sol::function* onCeasedFactionLeader = nullptr;
sol::function* onCeasedFactionHeir = nullptr;
sol::function* onUngarrisonedFort = nullptr;
sol::function* onTileSeen = nullptr;
sol::function* onObjSeen = nullptr;
sol::function* onUnitsDesertCrusade = nullptr;
sol::function* onArmyTakesCrusadeTarget = nullptr;
sol::function* onTransgression = nullptr;
sol::function* onForgiveness = nullptr;
sol::function* onUpdateAttitude = nullptr;
sol::function* onDemeanour = nullptr;
sol::function* onBirth = nullptr;
sol::function* onCharacterComesOfAge = nullptr;
sol::function* onCharacterMarries = nullptr;
sol::function* onCharacterMarriesPrincess = nullptr;
sol::function* onMarriageAlliancePossible = nullptr;
sol::function* onMarriageAllianceOffered = nullptr;
sol::function* onPriestBecomesHeretic = nullptr;
sol::function* onCharacterNearHeretic = nullptr;
sol::function* onCharacterNearWitch = nullptr;
sol::function* onCardinalPromoted = nullptr;
sol::function* onCardinalRemoved = nullptr;
sol::function* onInquisitorAppointed = nullptr;
sol::function* onCharacterBecomesAFather = nullptr;
sol::function* onCharacterTurnEnd = nullptr;
sol::function* onCharacterTurnEndInSettlement = nullptr;
sol::function* onGeneralDevastatesTile = nullptr;
sol::function* onGeneralJoinCrusade = nullptr;
sol::function* onGeneralAbandonCrusade = nullptr;
sol::function* onGeneralArrivesCrusadeTargetRegion = nullptr;
sol::function* onGeneralTakesCrusadeTarget = nullptr;
sol::function* onCharacterAttacksCrusadingGeneral = nullptr;
sol::function* onGeneralPrisonersRansomedCaptor = nullptr;
sol::function* onGeneralPrisonersRansomedCaptive = nullptr;
sol::function* onFactionLeaderPrisonersRansomedCaptor = nullptr;
sol::function* onFactionLeaderPrisonersRansomedCaptive = nullptr;
sol::function* onSpyMission = nullptr;
sol::function* onExecutesASpyOnAMission = nullptr;
sol::function* onLeaderOrderedSpyingMission = nullptr;
sol::function* onAssassinationMission = nullptr;
sol::function* onExecutesAnAssassinOnAMission = nullptr;
sol::function* onAssassinCaughtAttackingPope = nullptr;
sol::function* onLeaderOrderedAssassination = nullptr;
sol::function* onSufferAssassinationAttempt = nullptr;
sol::function* onAcquisitionMission = nullptr;
sol::function* onSufferAcquisitionAttempt = nullptr;
sol::function* onMarriageMission = nullptr;
sol::function* onSufferMarriageAttempt = nullptr;
sol::function* onDenouncementMission = nullptr;
sol::function* onSufferDenouncementAttempt = nullptr;
sol::function* onSabotageMission = nullptr;
sol::function* onLeaderOrderedSabotage = nullptr;
sol::function* onBriberyMission = nullptr;
sol::function* onLeaderOrderedBribery = nullptr;
sol::function* onAcceptBribe = nullptr;
sol::function* onRefuseBribe = nullptr;
sol::function* onInsurrection = nullptr;
sol::function* onDiplomacyMission = nullptr;
sol::function* onLeaderOrderedDiplomacy = nullptr;
sol::function* onLeaderMissionSuccess = nullptr;
sol::function* onLeaderMissionFailed = nullptr;
sol::function* onSettlementTurnStart = nullptr;
sol::function* onNewAdmiralCreated = nullptr;
sol::function* onUnitTrained = nullptr;
sol::function* onGovernorUnitTrained = nullptr;
sol::function* onGovernorBuildingCompleted = nullptr;
sol::function* onAgentCreated = nullptr;
sol::function* onGovernorAgentCreated = nullptr;
sol::function* onBuildingDestroyed = nullptr;
sol::function* onAddedToBuildingQueue = nullptr;
sol::function* onAddedToTrainingQueue = nullptr;
sol::function* onGovernorBuildingDestroyed = nullptr;
sol::function* onCityRiots = nullptr;
sol::function* onGiveSettlement = nullptr;
sol::function* onGovernorCityRiots = nullptr;
sol::function* onCityRebels = nullptr;
sol::function* onGovernorCityRebels = nullptr;
sol::function* onGovernorThrowGames = nullptr;
sol::function* onGovernorThrowRaces = nullptr;
sol::function* onUngarrisonedSettlement = nullptr;
sol::function* onOccupySettlement = nullptr;
sol::function* onSackSettlement = nullptr;
sol::function* onExterminatePopulation = nullptr;
sol::function* onCitySacked = nullptr;
sol::function* onSettlementUpgraded = nullptr;
sol::function* onSettlementConverted = nullptr;
sol::function* onGuildUpgraded = nullptr;
sol::function* onGuildDestroyed = nullptr;
sol::function* onSiegeEquipmentCompleted = nullptr;
sol::function* onSettlementTurnEnd = nullptr;
sol::function* onBuildingCompleted = nullptr;
sol::function* onCharacterSelected = nullptr;
sol::function* onEnemyCharacterSelected = nullptr;
sol::function* onSettlementSelected = nullptr;
sol::function* onMultiTurnMove = nullptr;
sol::function* onSettlementPanelOpen = nullptr;
sol::function* onFinancesPanelOpen = nullptr;
sol::function* onFactionSummaryPanelOpen = nullptr;
sol::function* onFamilyTreePanelOpen = nullptr;
sol::function* onDiplomacyPanelOpen = nullptr;
sol::function* onPreBattlePanelOpen = nullptr;
sol::function* onRecruitmentPanelOpen = nullptr;
sol::function* onConstructionPanelOpen = nullptr;
sol::function* onTradePanelOpen = nullptr;
sol::function* onIncomingMessage = nullptr;
sol::function* onMessageOpen = nullptr;
sol::function* onMessageClosed = nullptr;
sol::function* onAdviceSupressed = nullptr;
sol::function* onSettlementScrollAdviceRequested = nullptr;
sol::function* onDeclineAutomatedSettlementManagement = nullptr;
sol::function* onCharacterPanelOpen = nullptr;
sol::function* onRequestBuildingAdvice = nullptr;
sol::function* onRequestTrainingAdvice = nullptr;
sol::function* onRequestMercenariesAdvice = nullptr;
sol::function* onButtonPressed = nullptr;
sol::function* onShortcutTriggered = nullptr;
sol::function* onUIElementVisible = nullptr;
sol::function* onScrollOpened = nullptr;
sol::function* onScrollClosed = nullptr;
sol::function* onScrollAdviceRequested = nullptr;
sol::function* onEscPressed = nullptr;
sol::function* onScriptedAdvice = nullptr;
sol::function* onNavalPreBattleScrollAdviceRequested = nullptr;
sol::function* onPreBattleScrollAdviceRequested = nullptr;
sol::function* onCollegeOfCardinalsPanelOpen = nullptr;
sol::function* onDiplomaticStandingPanelOpen = nullptr;

