#pragma once
#include "luaPlugin.h"
#include <unordered_map>
#include <functional>

#include "gameHelpers.h"
#include "realGameTypes.h"
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
	void onEventWrapper(DWORD eventAddr, DWORD** vTab);
	
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
