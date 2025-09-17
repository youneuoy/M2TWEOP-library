#pragma once
#include "luaPlugin.h"
#include "console.h"
#include <unordered_map>
#include <functional>

#include "gameHelpers.h"
#include "realGameTypes.h"
struct aiPersonalityValues;
struct aiLongTermGoalDirector;
struct buildingBattle;
using namespace std;

#define tryLuaBasicEventFunk(luaFunc)  \
auto funcResult = luaFunc;\
if (!funcResult.valid())\
{\
	sol::error luaError = funcResult;\
	UINT defaultFlags = MB_ABORTRETRYIGNORE | MB_ICONEXCLAMATION;\
	gameHelpers::logStringGame(luaError.what());\
	int result = MessageBoxA(NULL, luaError.what(), "Lua exception!", defaultFlags);\
	if (plugData::data.luaAll.checkVar("TERMINATE_AT_LUA_EXCEPTION", 1) == true)\
	{\
		terminate();\
	}\
	console::handleMessageBoxResult(result);\
}\

struct missionDetails
{
	char* missionName;
	int missionNameHash;
	char* paybackName;
	int paybackNameHash;
	std::string getMissionName()
	{
		return missionName;
	}
	std::string getPaybackName()
	{
		return paybackName;
	}
};

struct eventTrigger
{
	void* eventTrigger_vtbl; //0x0000
	char pad_0004[28]; //0x0004
	unit* getEventAttackingUnit()
	{
		return callVFunc<2, unit*>(this);
	}
	unit* getEventDefendingUnit()
	{
		return callVFunc<3, unit*>(this);
	}
	character* getEventCharacter()
	{
		return callVFunc<4, character*>(this);
	}
	characterRecord* getEventCharacterRecord()
	{
		return callVFunc<5, characterRecord*>(this);
	}
	characterRecord* getEventNamedCharacter2()
	{
		return callVFunc<6, characterRecord*>(this);
	}
	//7 status thing
	characterRecord* getEventTargetNamedCharacter()
	{
		return callVFunc<8, characterRecord*>(this);
	}
	settlementStruct* getEventSettlement()
	{
		return callVFunc<9, settlementStruct*>(this);
	}
	settlementStruct* getEventTargetSettlement()
	{
		return callVFunc<10, settlementStruct*>(this);
	}
	fortStruct* getEventFort()
	{
		return callVFunc<11, fortStruct*>(this);
	}
	factionStruct* getEventFaction()
	{
		return callVFunc<12, factionStruct*>(this);
	}
	factionStruct* getEventTargetFaction()
	{
		return callVFunc<13, factionStruct*>(this);
	}
	armyStruct* getEventArmy()
	{
		return callVFunc<14, armyStruct*>(this);
	}
	int getEventRegion()
	{
		return callVFunc<15, int>(this);
	}
	int getEventTargetRegion()
	{
		return callVFunc<16, int>(this);
	}
	unit* getEventPlayerUnit()
	{
		return callVFunc<17, unit*>(this);
	}
	unit* getEventEnemyUnit()
	{
		return callVFunc<18, unit*>(this);
	}
	//19 building_battle
	buildingBattle* getBuildingBattle()
	{
		return callVFunc<19, buildingBattle*>(this);
	}
	//20 prior_build
	buildingInQueue* getPriorBuild()
	{
		return callVFunc<20, buildingInQueue*>(this);
	}
	//21 advised_build
	//22 best_finance_option
	//23 resource_description
	const char* getResourceDescription()
	{
		return *callVFunc<23, const char**>(this);
	}
	eduEntry* getEduEntry()
	{
		return callVFunc<23, eduEntry*>(this);
	}
	int getEventCharacterType()
	{
		return callVFunc<24, int>(this);
	}
	int getEventTargetCharacterType()
	{
		return callVFunc<25, int>(this);
	}
	const char* getEventType();
	const char* getMissionSuccessLevel();
	int getMissionProbability()
	{
		return callVFunc<28, int>(this);
	}
	//29 mission_details
	missionDetails* getMissionDetails()
	{
		return callVFunc<29, missionDetails*>(this);
	}
	//30 character_action_advice
	//31 event
	int getEventID()
	{
		return callVFunc<31, int>(this);;
	}
	//32 unk
	//33 unk2
	//34 guild handler
	int getEventGuild()
	{
		return callVFunc<34, int>(this);
	}
	//35 event_counter
	const char* getEventCounter()
	{
		return *callVFunc<35, const char**>(this);
	}
	//36 sm_position
	coordPair* getPosition()
	{
		return callVFunc<36, coordPair*>(this);
	}
	//37 get religion
	int getReligion()
	{
		return callVFunc<37, int>(this);;
	}
	//38 get target religion
	int getTargetReligion()
	{
		return callVFunc<38, int>(this);;
	}
	//39 amount
	float getAmount()
	{
		return callVFunc<39, float>(this);;
	}
	//40 crusade
	crusade* getCrusade()
	{
		return callVFunc<40, crusade*>(this);
	}
	//41 captured faction info
	capturedFactionInfo* getCapturedFactionInfo()
	{
		return callVFunc<41, capturedFactionInfo*>(this);
	}
	const char* getRansomType();
	//43 unk
	//44 unk
	//45 unit
	unit* getUnit()
	{
		return callVFunc<45, unit*>(this);
	}
};

void checkLuaFunc(sol::function** lRef);

namespace gameEvents
{
	regionStruct* getCharacterRegion(const characterRecord* character);

	enum class EventType
	{
		standardEvent = 1,
		attackResidenceEvent = 2
	};

	class EventBase
	{
	public:
		virtual ~EventBase() = default;
		virtual int callEvent(DWORD** vTab) = 0;
	};
	
	void onReadGameDbsAtStart();
	void onGameInit();
	void onUnloadCampaign();
	void onRemoveFromUnitQueue(const unitRQ* item);
	void onNewGameLoaded();
	void onAiTurn(aiFaction* aiFaction);
	void onEndSiege(int x, int y);
	void onStartSiege(int x, int y);


	void onClickAtTile(int x, int y);
	void onCharacterClicked(character* enemy);
	void onCampaignTick();
	void onBattleTick();
	void onCalculateLTGD(aiLongTermGoalDirector* ltgd);
	void onSetProductionControllers(aiPersonalityValues* personality);
	std::string * onSelectWorldpkgdesc(const char* selectedRec, const char* selectedGroup);
	int onFortificationLevelS(settlementStruct * settlement, bool* isCastle, bool* isChanged);
	float onCalculateUnitValue(eduEntry* entry, float value);
	void onChangeTurnNum(int num);
	void onNewGameStart();
	void onCampaignMapLoaded();
	void onLoadGamePl(const std::vector<std::string>*saveFiles);
	std::vector<std::string>*onSaveGamePl(UNICODE_STRING * *&savePath);
	void onEventWrapper(DWORD eventAddr, DWORD** vTab);
	void onFactionSymbolSelect(int factionID);
	
	template<EventType EvType> class Event :public EventBase
	{
	public:

		sol::function* funk;
		
		Event(const std::string& luaFunctionName, sol::state& luaState) : funk(new sol::function(luaState[luaFunctionName]))
		{
			checkLuaFunc(&funk);
		}

		int callEvent(DWORD** vTab) override;
	};

	template<EventType EvType>
	void addEvent(DWORD key, const char* name);
	
	class gameEventManager
	{
	public:
		static EventBase* getEvent(DWORD key);
		
		template<EventType EvType>
		static void addEvent(DWORD key, const char* name);
	private:
		static std::unordered_map<DWORD, std::unique_ptr<EventBase>> m_Events;
	};

}

/*
*/
