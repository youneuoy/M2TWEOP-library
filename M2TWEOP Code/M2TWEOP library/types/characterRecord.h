#pragma once
#include "realGameTypes.h"
#include "lua/sol.hpp"

enum class deathType
{
	alive,
	natural,
	disaster,
	battle,
	assassinated,
	executed,
	executedPrisoner,
	poisoned,
	plague,
	starvation,
	captainRemoved,
	removedRetraining,
	incompatibleFaction,
	bribed,
	disbanded,
	sinkingShip,
	married,
	acquired,
	retired,
	disgraced,
	denounced
};

struct traitLevel
{
	int32_t level;
	struct UNICODE_STRING*** localizedName;
	struct UNICODE_STRING*** localizedDescription;
	struct UNICODE_STRING*** localizedEpithetDescription;
	struct UNICODE_STRING*** localizedGainDescription;
	struct UNICODE_STRING*** localizedLoseDescription;
	int32_t threshold;
	struct traitEffect* effects;
	int32_t effectsSize;
	int32_t effectsCount;
	struct UNICODE_STRING*** localizedEffectsDescription;
};

struct traitEntry { /* char* at 0x4 */
	int32_t index;
	char* name;
	int32_t nameHash;
	struct traitLevel levels[10];
	int32_t levelCount;
	struct traitEntry* antiTraits[20];
	int32_t antiTraitCount;
	int32_t characterTypeNum;
	uint32_t characterType;
	int32_t noGoingBackLevel;
	int32_t excludeCulturesNum;
	uint32_t excludeCulturesStart;
	int32_t hidden;
	struct stringWithHash antiTraitNames[20];
	int32_t antiTraitNameCount;
};

struct traitContainer {
	struct trait* trait;
	struct traitContainer* prev;
	struct traitContainer* next;
	int hasEpithet;
};

struct trait { /* traits of the character */
	struct traitEntry* traitEntry;
	struct traitLevel* level; /* level of trait */
	int traitPoints;
};

struct traitEffect
{
	int32_t effectID;
	int32_t value;
};

//ancillary of character
struct ancillary { /* structure of ancillary */
	UINT32 index;
	UNICODE_STRING*** localizedAncName; //04
	UNICODE_STRING*** localizedAncDesc; //08
	char* ancName; //000C
	int ancNameHash; //0010
	char* patchToAncImage; //0014
	int pathHash; //0018
	bool isUnique; //0x001C
	char pad_001D[3]; //0x001D
	ancillary* exclusions[3]; //0020
	int exclusionsNum; //002C
	uint32_t somethingBitfield; //0030
	uint32_t excludedCultures; //0034
	traitEffect* effects; //0038
	int effectsSize; //003C
	int effectsNum; //0040
	UNICODE_STRING*** effectsDescr; //044
	char* type; //0048
	int typeHash; //004C
	bool transferable;//00050
	char pad_0051[3];
	
	traitEffect* getEffect(int i)
	{
		return &effects[i];
	}
};

//additional character data(name,label,traits, etc)
struct characterRecord { /* many important info about character */
	UINT32 index; /* index of character */
	UNICODE_STRING** localizedFullName; /* displaying name */ //04
	UNICODE_STRING** localizedNameForSave; /* name - saved to save file */ //0008
	UNICODE_STRING** localizedNextNameForSave; /* surname - saved to save file */ //000C
	UNICODE_STRING** localizedNicknameForSave; /* epithet - saved to save file */ //0010
	char* shortName; /* not a full name) */ //0014
	int shortNameHash;
	char* lastName; //001C
	int lastNameHash;
	char* fullName; /* full name of character) */ //0024
	int fullNameHash;
	char* label; /* label of character */ //002C
	UINT32 labelCrypt; /* crypt  of the label */
	char* modelName; /* custom model */ //0034
	int modelNameCrypt;
	uchar status; /* bits: offmap/wasleader/isheir/isleader*/ //003C
	undefined field_0x3d[3];
	int nameFaction; //0040
	int32_t command; /* command */ //0044 attributes start here
	int32_t influence;
	int32_t subterfuge;
	int32_t loyalty;
	int32_t charm;
	int32_t finance;
	int32_t piety;
	int32_t magic;
	int32_t chivalry; /* chivalry/dread */
	int32_t authority; /* authority of leader */
	int32_t level;
	int32_t unorthodoxy; //0x0070
	int32_t heresyImmunity; //0x0074
	int32_t assassination; //0x0078
	int32_t sabotage; //0x007C
	int32_t eligibility; //0x0080
	int32_t purity; //0x0084
	int32_t violence; //0x0088
	int32_t disposition; //0x008C
	int32_t boldness; //0x0090
	int32_t generosity; //0x0094
	int32_t management; //0x0098
	int32_t bodyguardSize; //0x009C
	int32_t troopMorale; //0x00A0
	int32_t movementPointsBonus; //0x00A4
	int32_t attack; //0x00A8
	int32_t defence; //0x00AC
	int32_t siegeAttack; //0x00B0
	int32_t siegeDefense; //0x00B4
	int32_t ambush; //0x00B8
	int32_t navalCommand; //0x00BC
	int32_t siegeEngineering; //0x00C0
	int32_t nightBattle; //0x00C4
	int32_t personalSecurity; //0x00C8
	int32_t publicSecurity; //0x00CC
	int32_t bribery; //0x00D0
	int32_t bribeResistance; //0x00D4
	int32_t electability; //0x00D8
	int32_t lineOfSight; //0x00DC
	int32_t trainingUnits; //0x00E0
	int32_t trainingAgents; //0x00E4
	int32_t construction; //0x00E8
	int32_t trading; //0x00EC
	int32_t localPopularity; //0x00F0
	int32_t footInTheDoor; //0x00F4
	int32_t farming; //0x00F8
	int32_t mining; //0x00FC
	int32_t taxCollection; //0x0100
	int32_t fertility; //0x0104
	int32_t cavalryCommand; //0x0108
	int32_t infantryCommand; //0x010C
	int32_t gunpowderCommand; //0x0110
	int32_t artilleryCommand; //0x0114
	int32_t combatVsReligion[10]; //0x0118
	int32_t combatVsFaction[32]; //0x0140
	int32_t health; //0x01C0
	int32_t squalor; //0x01C4
	int32_t unrest; //0x01C8
	int32_t law; //0x01CC
	int32_t looting; //0x01D0
	int32_t bodyguardValour; //0x01D4
	int32_t hitpoints; //0x01D8
	int32_t trainingAnimalUnits; //0x01DC
	int32_t battleSurgery; //0x01E0
	struct traitContainer* traits; /* Linked list*/
	struct traitContainer* traitLast; /* Linked list*/
	struct ancData** ancillaries; /* pointers to character ancillaries, names at  [item number] -0-0c-here) */
	int ancillariesSize;
	UINT32 ancNum; /* number of character  ancillaries */
	struct character* gen; /* on stratmap */
	int epithetDescrIndex;
	int epithetTitleIndex;
	float yearOfBirth; /* yearOfBirth */
	int seasonOfBirth;
	float deathYear;
	int deathSeason;
	int32_t numberOfChildren;
	struct factionStruct* faction;
	int originalFaction;
	uint32_t previousFactions;
	struct characterRecord* parent; /* father */
	struct characterRecord* spouse;
	struct characterRecord* childs[8]; /* children, women wont have children */
	int32_t deathType;
	char* portrait;
	int portraitHash;
	char* portrait2;
	int portrait2Hash;
	int portraitIndex;
	char* portrait_custom; /* custom portrait name */
	int customPortraitHash;
	unsigned __int32 isAlive : 1;
	unsigned __int32 isMale : 1;
	unsigned __int32 isChild : 1;
	unsigned __int32 age : 7;
	unsigned __int32 kills : 3;
	unsigned __int32 isFamily : 1;
	unsigned __int32 isFamilyHead : 1;
	bool wasMarriageAlliance;
public:
	traitContainer* getTraits() const
	{
		return traits;
	}
	ancillary* getAncillary(const int ancIndex) const
	{
		return ancillaries[ancIndex]->dataAnc;
	}
	int getAge() const
	{
		return age;
	}
	void setAge(int newAge)
	{
		age = newAge & 0x7F;
	}
	int getKills() const
	{
		return kills;
	}
	void setKills(int newKills)
	{
		kills = newKills & 0x7;
	}
	bool isOffMap() const
	{
		return status & 8;
	}
	bool isLeader() const
	{
		return status & 1;
	}
	bool wasLeader() const
	{
		return status & 4;
	}
	void setWasLeader(bool set)
	{
		status = set ? status | 4 : status & ~4;
	}
	bool getIsMale() const
	{
		return isMale;
	}
	void setIsMale(bool set)
	{
		isMale = set;
	}
	bool getIsAlive() const
	{
		return isAlive;
	}
	void setIsAlive(bool set)
	{
		isAlive = set;
	}
	bool getIsChild() const
	{
		return isChild;
	}
	void setIsChild(bool set)
	{
		isChild = set;
	}
	bool getIsFamily() const
	{
		return isFamily;
	}
	void setIsFamily(bool set)
	{
		isFamily = set;
	}
	bool getIsFamilyHead() const
	{
		return isFamilyHead;
	}
	void setIsFamilyHead(bool set)
	{
		isFamilyHead = set;
	}
};

struct capturedCharacter
{
	struct characterRecord* namedChar;
	int32_t capturedValue;
};

struct capturedUnit
{
	struct unit* unit;
	int32_t capturedSoldiers;
	int32_t capturedValue;
};

struct capturedFactionInfo
{
	int32_t targetFactionID;
	int32_t factionID;
	struct characterRecord* character;
	struct characterRecord* targetCharacter;
	std::vector<capturedCharacter> capturedCharacters;
	std::vector<capturedUnit> capturedUnits;
	int32_t ransomValue;
	bool heirCaptured;
	bool leaderCaptured;
	char pad_001E[2];
public:
	int getCapturedUnitNum()
	{
		return capturedUnits.size();
	}
	int getCapturedCharacterNum()
	{
		return capturedCharacters.size();
	}
	capturedUnit* getCapturedUnit(const int index)
	{
		return &capturedUnits[index];
	}
	capturedCharacter* getCapturedCharacter(const int index)
	{
		return &capturedCharacters[index];
	}
};

namespace characterRecordHelpers
{
	std::string namedCharUniStringToStr(characterRecord* genChar);
	void setStringPropertyGenChar(characterRecord* genChar, std::string newS);
	std::string getStringPropertyGenChar(const characterRecord* genChar);
	void namedCharSetLocalizedFullName(characterRecord* genChar, const char* str);
	void namedCharSetLocalizedNameForSave(characterRecord* genChar, const char* str);
	void namedCharSetLocalizedNextNameForSave(characterRecord* genChar, const char* str);
	void namedCharSetLocalizedNicknameForSave(characterRecord* genChar, const char* str);
	void setHeir(characterRecord* gen, bool isJustSet);
	int addAncillaryName(characterRecord* character, const std::string& ancName);
	int addAncillary(characterRecord* character, ancillary* anc);
	void removeAncillary(characterRecord* character, ancillary* anc);
	void removeTrait(characterRecord* character, const char* traitName);
	void addTrait(characterRecord* character, const char* traitName, int traitLevel);
	void addToLua(sol::state& luaState);
}
