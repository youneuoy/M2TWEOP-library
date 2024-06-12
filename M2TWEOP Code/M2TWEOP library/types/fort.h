#pragma once
#include "realGameTypes.h"
#include "lua/sol.hpp"

struct fortStruct
{
public:
	//fields
    DWORD* fortVtable;
    void* nextObject;
	void* prevObject;
    UINT32 xCoord;
    UINT32 yCoord;
	int fade;
	bool render;
	char pad0[3];
	float opacity;
	bool highlighted;
	char pad1[3];
    float stratPosX;
    float stratPosZ;
    float stratPosY;
    void* trackedPointerArmyVtable;
    struct stackStruct* army;
    struct oneSiege sieges[8];
    int8_t siegeNum; //0x0088
    char pad_0089[3]; //0x0089
    int32_t maxHoldoutTurns;
    int32_t siegeDuration;
    int32_t siegeCasualties;
    int32_t preSiegePopulation;
    int32_t factionTradedFrom;
    int32_t plagueDuration;
    int8_t plagued;
    int8_t salliedOut; //0x00A5
    int8_t gatesAreOpened;
    int8_t readyToSurrender;
    crusade* takenByCrusade;
    struct descrRebelEntry* descrRebel; //0x00AC
    int32_t subFactionID; //0x00B0
	spyingInfo spyInfo;
    struct stratFortMod* stratModel;
    int regionID;
    struct factionStruct* faction;
    int32_t creatorFactionID; //0x00EC
    int32_t creatorCultureID; //0x00F0
    UNICODE_STRING** name; //0x00F4
    char* fortType;
    int32_t fortTypeHash;
	int32_t fortFortificationLevel; //0x0100

public :
	//methods
	siegeS* getSiege(const int index)
	{
		return sieges[index].siege;
	}
	character* getGovernor(const int index)
	{
		if (!nextObject)
			return nullptr;
		if (CallVFunc<4, int>(nextObject) == 28)
		{
			return static_cast<character*>(nextObject);
		}
		return nullptr;
	}
};

namespace fortHelpers
{
	void addToLua(sol::state& luaState);
	void changeOwner(fortStruct* fort, factionStruct* newFaction, bool convertGarrison);
};