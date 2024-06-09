#pragma once
#include "pluginM2TWEOP.h"
#include <unordered_map>
#include <functional>
using namespace std;

extern "C" PLUGINM2TWEOP_API void onReadGameDbsAtStart();
extern "C" PLUGINM2TWEOP_API void onGameInit();
extern "C" PLUGINM2TWEOP_API void onUnloadCampaign();
extern "C" PLUGINM2TWEOP_API void onAiTurn(aiFaction* aifaction);
extern "C" PLUGINM2TWEOP_API void onEndSiege(int x, int y);
extern "C" PLUGINM2TWEOP_API void onStartSiege(int x, int y);


extern "C" PLUGINM2TWEOP_API void onClickAtTile(int x, int y);
extern "C" PLUGINM2TWEOP_API std::string * onSelectWorldpkgdesc(const char* selectedRec, const char* selectedGroup);
extern "C" PLUGINM2TWEOP_API int onfortificationlevelS(settlementStruct * settlement, bool* isCastle, bool* isChanged);
extern "C" PLUGINM2TWEOP_API float onCalculateUnitValue(eduEntry* entry, float value);
extern "C" PLUGINM2TWEOP_API void onChangeTurnNum(int num);
extern "C" PLUGINM2TWEOP_API void onNewGameStart();
extern "C" PLUGINM2TWEOP_API void onCampaignMapLoaded();
extern "C" PLUGINM2TWEOP_API void onLoadGamePl(std::vector<std::string>*saveFiles);
extern "C" PLUGINM2TWEOP_API std::vector<std::string>*onSaveGamePl(UNICODE_STRING * *&savePath);
extern "C" PLUGINM2TWEOP_API void onEventWrapper(DWORD eventAddr, DWORD * *vTab, DWORD arg2);


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
