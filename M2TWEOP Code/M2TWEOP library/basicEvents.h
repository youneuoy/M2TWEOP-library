#pragma once
#include "pluginM2TWEOP.h"
#include <unordered_map>
#include <functional>
using namespace std;



#define tryLuaBasicEventFunk(luaFunc)  \
auto funcResult = luaFunc;\
if (!funcResult.valid())\
{\
	sol::error luaError = funcResult;\
	MessageBoxA(NULL, luaError.what(), "Lua exception!", NULL);\
	if (plugData::data.luaAll.checkVar("terminateAtLuaException", 1) == true)\
	{\
		terminate();\
	}\
}\


void checkLuaFunc(sol::function** lRef);

namespace gameEvents
{
	regionStruct* getCharacterRegion(const namedCharacter* character);

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
	void onAiTurn(aiFaction* aifaction);
	void onEndSiege(int x, int y);
	void onStartSiege(int x, int y);


	void onClickAtTile(int x, int y);
	std::string * onSelectWorldpkgdesc(const char* selectedRec, const char* selectedGroup);
	int onfortificationlevelS(settlementStruct * settlement, bool* isCastle, bool* isChanged);
	float onCalculateUnitValue(eduEntry* entry, float value);
	void onChangeTurnNum(int num);
	void onNewGameStart();
	void onCampaignMapLoaded();
	void onLoadGamePl(std::vector<std::string>*saveFiles);
	std::vector<std::string>*onSaveGamePl(UNICODE_STRING * *&savePath);
	void onEventWrapper(DWORD eventAddr, DWORD * *vTab, DWORD arg2);
	
	template<EventType EvType> class Event :public EventBase
	{
	public:

		sol::function* funk;

		Event(const std::string& luaFunctionName, sol::state& luaState) : funk(new sol::function(luaState[luaFunctionName]))
		{
			checkLuaFunc(&funk);
		}

		int callEvent(DWORD** vTab) override
		{
			if (EvType == EventType::standardEvent)
			{
				auto eventData = reinterpret_cast<eventTrigger*>(vTab);
				if (funk != nullptr) {
					tryLuaBasicEventFunk((*funk)(eventData))
				}
				return 1;
			}
			else if (EvType == EventType::attackResidenceEvent)
			{
				auto eventData = reinterpret_cast<eventTrigger*>(vTab);
				auto character = gameHelpers::getEventNamedCharacter(eventData);
				auto settlement = character->gen->besiegingSettlement;
				fortStruct* fort = nullptr;
				if (settlement)
				{
					fort = gameHelpers::getTileFort(gameHelpers::getTile(settlement->xCoord, settlement->yCoord));
					if (fort)
						settlement = nullptr;
				}
				if (funk != nullptr) {
					tryLuaBasicEventFunk((*funk)(eventData, settlement, fort))
				}
				return 2;
			}
			else
			{
				return 0;
			}
		}
	};

	std::unordered_map<DWORD, std::unique_ptr<EventBase>> events;
	template<EventType EvType> void addEvent(DWORD key, const char* name);
}

/*
*/
