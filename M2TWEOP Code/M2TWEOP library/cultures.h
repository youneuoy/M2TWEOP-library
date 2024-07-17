#pragma once
#include <memory>
#include <string>
#include "realGameTypes.h"

struct cultureModels
{
public:
	char pad_0000[5584]; //0x0000
}; //Size: 0x15D0

struct cultureAgent
{
public:
	char *cardName;
	int32_t cardNameHash;
	char *infoCardName;
	int32_t infoCardNameHash;
	char *cardName2;
	int32_t cardName2Hash;
	int index1;
	int index2;
	int16_t cost;
	int8_t time;
	int8_t pad23;
	int16_t popCost;
	int16_t someOtherRomeShit;
	int recruitBiasValue;
}; //Size: 0x002C

struct underlay
{
	char *name;
	int nameHash;
	char *path;
	int pathHash;
	float float10;
	int field14;
};

struct cultureCasEntry
{
	int field_0;
	int field_4;
	char *casName;
	int casNameHash;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
	underlay *underlay;
};

struct cultureFort
{
	void *modelRigid;
	void *stratModelArrayEntry;
	stringWithHash fortCasName;
	stringWithHash fortWallsCasName;
	int array_18[31];
	stringWithHash stringsHash_94[31];
	char *strings_18c[31];
	stringWithHash stringsHash_208[31];
	stringWithHash uiGenericFort;
	underlay *underlay;
};

struct smthingCult2
{
	int field_0;
	int field_4;
	stringWithHash field_8;
	stringWithHash field_10;
	int field_18;
};

struct cultureSettlement
{
	int field_0;
	int field_4;
	char *casNameCity;
	int casNameCityHash;
	void *casNameCastle;
	int casNameCastleHash;
	int field_18;
	char *strings1C[31];
	char *strings98[31];
	stringWithHash stringHash114[31];
	stringWithHash stringHash20c[31];
	smthingCult2 field_304[5];
	char *tgaFileCity;
	int field_394;
	char *tgaFileCastle;
	int field_39C;
	underlay *underlay;
};

struct culture
{
	int32_t cultureID;
	char *cultureName;
	int32_t cultureNameHash;
	cultureFort fort;
	cultureCasEntry fishingVillage;
	cultureCasEntry portWalls[3];
	cultureCasEntry portBuildings[3];
	cultureCasEntry watchTower;
	int cultureCasEntryCount;
	int32_t fortCost;
	int32_t watchTowerCost;
	int field_444;
	char *portraitMapping;
	int field_44C;
	int rebelStandardIndex;
	struct cultureSettlement cultureSettlements[6];
	struct cultureAgent cultureAgents[6];
	int maxLevel;
};

struct culturesDB
{
	struct culture *cultures;
	int32_t culturesSize;
	int32_t culturesCount;
};

struct portraitsEntry
{
	int32_t count{}; //0x0000
	int32_t *portraitIndexes{}; //0x0004
	int32_t usageCount{}; //0x0008
};

struct portraitDdCharacterEntry
{
public:
	portraitsEntry ages[3]; //0x0000
}; //Size: 0x0024

struct portraitDbEntry
{
public:
	char *cultureName{}; //0x0000
	int32_t cultureNameHash{}; //0x0004
	struct portraitDdCharacterEntry portraits[9]; //0x0008
}; //Size: 0x014C

struct portraitDb
{
public:
	struct portraitDbEntry cultures[7]; //0x0000
}; //Size: 0x0914

namespace cultures
{
	std::string getRandomPortrait(int cultureId, int religionId);
    enum class portraitType
    {
        civilians = 0,
        generals = 1,
        rogues = 2,
        princesses = 3,
        merchants = 4,
        priests = 5,
        heretics = 6,
        witches = 7,
        inquisitors = 8,
        MAX
    };
    
    enum class portraitAge
    {
        young = 0,
        old = 1,
        dead = 2,
    };

    int getCultureCount();
    culturesDB* getCultureDb();
    std::string getCultureName(int cultureID);
    
    struct eopPortraitDb
    {
        static std::vector<std::shared_ptr<portraitDbEntry>> portraits;
        static int entryCount;
        static portraitDbEntry* getEntry(int cultureID);
        static void createEopPortraitDb();
    };
}
