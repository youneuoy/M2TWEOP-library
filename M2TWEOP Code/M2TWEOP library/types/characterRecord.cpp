///
//![Lua logo](../Lua.png)
//@module LuaPlugin
//@author Fynn
//@license GPL-3.0
#include "pch.h"
#include "characterRecord.h"

#include "campaign.h"
#include "campaignDb.h"
#include "gameStringHelpers.h"
#include "functionsOffsets.h"
#include "character.h"
#include "cultures.h"
#include "faction.h"
#include "gameHelpers.h"
#include "luaPlugin.h"
#include "stratModelsChange.h"
#include "techFuncs.h"
#include "unit.h"

enum
{
	namedChar_localizedFullName = 1,
	namedChar_localizedNameForSave = 2,
	namedChar_localizedNextNameForSave = 3,
	namedChar_localizedNicknameForSave = 4,
	characterRecord_shortName = 1,
	characterRecord_fullName = 2,
	characterRecord_label = 3,
	characterRecord_modelName = 4,
	characterRecord_portrait = 5,
	characterRecord_portrait2 = 6,
	characterRecord_portrait_custom = 7,
	characterRecord_lastName = 8
};
std::shared_ptr<eopCharacterDataDb> eopCharacterDataDb::instance = std::make_shared<eopCharacterDataDb>();

std::string traitLevel::getLocalizedName()
{
	if (localizedName == nullptr)
		return "";
	return gameStringHelpers::uniStringToStr(*localizedName);
}
	
void traitLevel::setLocalizedName(const std::string& newName)
{
	localizedName = techFuncs::createGameClass<UNICODE_STRING**>();
	gameStringHelpers::createUniString(*localizedName, newName.c_str());
}

std::string traitLevel::getLocalizedDescription()
{
	if (localizedDescription == nullptr)
		return "";
	return gameStringHelpers::uniStringToStr(*localizedDescription);
}

void traitLevel::setLocalizedDescription(const std::string& newDescr)
{
	localizedDescription = techFuncs::createGameClass<UNICODE_STRING**>();
	gameStringHelpers::createUniString(*localizedDescription, newDescr.c_str());
}

std::string traitLevel::getLocalizedEpithetDescription()
{
	if (localizedEpithetDescription == nullptr)
		return "";
	return gameStringHelpers::uniStringToStr(*localizedEpithetDescription);
}

void traitLevel::setLocalizedEpithetDescription(const std::string& newDescr)
{
	localizedEpithetDescription = techFuncs::createGameClass<UNICODE_STRING**>();
	gameStringHelpers::createUniString(*localizedEpithetDescription, newDescr.c_str());
}

std::string traitLevel::getLocalizedGainDescription()
{
	if (localizedGainDescription == nullptr)
		return "";
	return gameStringHelpers::uniStringToStr(*localizedGainDescription);
}

void traitLevel::setLocalizedGainDescription(const std::string& newDescr)
{
	localizedGainDescription = techFuncs::createGameClass<UNICODE_STRING**>();
	gameStringHelpers::createUniString(*localizedGainDescription, newDescr.c_str());
}

std::string traitLevel::getLocalizedLoseDescription()
{
	if (localizedLoseDescription == nullptr)
		return "";
	return gameStringHelpers::uniStringToStr(*localizedLoseDescription);
}

void traitLevel::setLocalizedLoseDescription(const std::string& newDescr)
{
	localizedLoseDescription = techFuncs::createGameClass<UNICODE_STRING**>();
	gameStringHelpers::createUniString(*localizedLoseDescription, newDescr.c_str());
}

std::string traitLevel::getLocalizedEffectsDescription()
{
	if (localizedEffectsDescription == nullptr)
		return "";
	return gameStringHelpers::uniStringToStr(*localizedEffectsDescription);
}

void traitLevel::setLocalizedEffectsDescription(const std::string& newDescr)
{
	localizedEffectsDescription = techFuncs::createGameClass<UNICODE_STRING**>();
	//*localizedEffectsDescription = techFuncs::createGameClass<UNICODE_STRING*>();
	//**localizedEffectsDescription = techFuncs::createGameClass<UNICODE_STRING>();
	gameStringHelpers::createUniString(*localizedEffectsDescription, newDescr.c_str());
}

traitEntry* traitDb::getTrait(const int i)
{
	if (i < 0 || i >= traitsNum)
	{
		gameHelpers::logStringGame("traitDb::getTrait - Trait index out of bounds: " + std::to_string(i));
		return nullptr;
	}
	return &traits[i];
}

ancillary* ancillaryDb::getAncillary(const int i)
{
	if (i < 0 || i >= ancillariesNum)
	{
		gameHelpers::logStringGame("ancillaryDb::getAncillary - Ancillary index out of bounds: " + std::to_string(i));
		return nullptr;
	}
	return &ancillaries[i];
}

namespace characterRecordHelpers
{

	/*----------------------------------------------------------------------------------------------------------------*\
										 Character Record helpers
    \*----------------------------------------------------------------------------------------------------------------*/
#pragma region Character Record helpers
	
	//characterRecord
	template <char fieldIndex>
	std::string getStringPropertyGenChar(const characterRecord* genChar)
	{
		char* retS = nullptr;
		if (fieldIndex == characterRecord_shortName)
			retS = genChar->shortName;
		else if (fieldIndex == characterRecord_fullName)
			retS = genChar->fullName;
		else if (fieldIndex == characterRecord_label)
			retS = genChar->label;
		else if (fieldIndex == characterRecord_modelName)
			retS = genChar->modelName;
		else if (fieldIndex == characterRecord_portrait)
			retS = genChar->portrait;
		else if (fieldIndex == characterRecord_portrait2)
			retS = genChar->portrait2;
		else if (fieldIndex == characterRecord_portrait_custom)
			retS = genChar->portrait_custom;
		else if (fieldIndex == characterRecord_lastName)
			retS = genChar->lastName;

		if (retS != nullptr)
			return std::string(retS);
		return std::string("");
	}
	template <char fieldIndex>
	void setStringPropertyGenChar(characterRecord* genChar, std::string newS)
	{
		if (fieldIndex == characterRecord_shortName)
			gameStringHelpers::setHashedString(&genChar->shortName, newS.c_str());
		else if (fieldIndex == characterRecord_fullName)
			gameStringHelpers::setHashedString(&genChar->fullName, newS.c_str());
		else if (fieldIndex == characterRecord_label)
			gameStringHelpers::setHashedString(&genChar->label, newS.c_str());
		else if (fieldIndex == characterRecord_modelName)
			gameStringHelpers::setHashedString(&genChar->modelName, newS.c_str());
		else if (fieldIndex == characterRecord_portrait)
			gameStringHelpers::setHashedString(&genChar->portrait, newS.c_str());
		else if (fieldIndex == characterRecord_portrait2)
			gameStringHelpers::setHashedString(&genChar->portrait2, newS.c_str());
		else if (fieldIndex == characterRecord_portrait_custom)
			gameStringHelpers::setHashedString(&genChar->portrait_custom, newS.c_str());
		else if (fieldIndex == characterRecord_lastName)
			gameStringHelpers::setHashedString(&genChar->lastName, newS.c_str());
	}
	
	template <char fieldIndex>
	std::string namedCharUniStringToStr(characterRecord* genChar)
	{
		if (fieldIndex == namedChar_localizedFullName)
			return gameStringHelpers::uniStringToStr(genChar->localizedFullName);
		if (fieldIndex == namedChar_localizedNameForSave)
			return gameStringHelpers::uniStringToStr(*genChar->localizedNameForSave);
		if (fieldIndex == namedChar_localizedNextNameForSave)
			return gameStringHelpers::uniStringToStr(*genChar->localizedNextNameForSave);
		if (fieldIndex == namedChar_localizedNicknameForSave)
			return gameStringHelpers::uniStringToStr(*genChar->localizedNicknameForSave);
		return "";
	}
	
	void namedCharSetLocalizedFullName(characterRecord* genChar, const char* str)
	{
		if (const auto charData = eopCharacterDataDb::instance->getOrCreateData(genChar->giveValidLabel(), genChar->gen ? genChar->gen->getTypeID() : 7))
			charData->localizedDisplayName = str;
		gameStringHelpers::createUniString(genChar->localizedFullName, str);
	}

	struct uni16pp
	{
		UNICODE_STRING* str;
		uni16pp()
		{
			str = techFuncs::createGameClass<UNICODE_STRING>();
		}
	};

	struct uni16ppp
	{
		uni16pp* str;
		uni16ppp()
		{
			str = new uni16pp();
		}
	};

	std::tuple<const char*, const char*> getRandomNames(const factionStruct* faction, const int nameFactionId, const bool isMale)
	{
		const auto campaign = campaignHelpers::getCampaignData();
		const int checkCountMax = faction->characterRecordNum * 10;
		int checkCount = 0;
		int firstNameIndex = 0;
		int secondNameIndex = 0;
		while (checkCount < checkCountMax)
		{
			GAME_FUNC(int(__cdecl*)(int*, int, bool, int*, int*), getRandomNameFunc)
			(&campaign->lastRandomSeed, nameFactionId, isMale, &firstNameIndex, &secondNameIndex);
			bool research = false;
			for(int i = 0; i < faction->characterRecordNum; i++)
			{
				if (const int nameIndex = GAME_FUNC(int(__thiscall*)(characterRecord*), getNameIndexFunc)(faction->characterRecords[i]);
					firstNameIndex == nameIndex)
				{
					research = true;
					break;
				}
			}
			checkCount++;
			if (research)
				continue;
			break;
		}
		const auto name = GAME_FUNC(const char*(__cdecl*)(int, int, int), getCharacterName)(isMale ? 0 : 1 , nameFactionId, firstNameIndex);
		const auto name2 = GAME_FUNC(const char*(__cdecl*)(int, int, int), getCharacterName)(2, nameFactionId, secondNameIndex);
		return std::make_tuple(name, name2);
	}

	void namedCharSetLocalizedNameForSave(characterRecord* genChar, const char* str)
	{
		const auto newStr = new uni16ppp();
		genChar->localizedNameForSave = reinterpret_cast<UNICODE_STRING***>(newStr);
		gameStringHelpers::createUniString(*genChar->localizedNameForSave, str);
		genChar->applyName();
		if (const auto charData = eopCharacterDataDb::instance->getOrCreateData(genChar->giveValidLabel(), genChar->gen ? genChar->gen->getTypeID() : 7))
		{
			charData->localizedName = str;
			if (charData->internalName.empty())
				charData->internalName = characterRecordHelpers::getStringPropertyGenChar<characterRecord_shortName>(genChar);
		}
	}

	void namedCharSetLocalizedNextNameForSave(characterRecord* genChar, const char* str)
	{
		const auto newStr = new uni16ppp();
		genChar->localizedNextNameForSave = reinterpret_cast<UNICODE_STRING***>(newStr);
		gameStringHelpers::createUniString(*genChar->localizedNextNameForSave, str);
		genChar->applyName();
		if (const auto charData = eopCharacterDataDb::instance->getOrCreateData(genChar->giveValidLabel(), genChar->gen ? genChar->gen->getTypeID() : 7))
		{
			charData->localizedLastName = str;
			if (charData->internalLastName.empty())
				charData->internalLastName = characterRecordHelpers::getStringPropertyGenChar<characterRecord_lastName>(genChar);
		}
	}

	void namedCharSetLocalizedNicknameForSave(characterRecord* genChar, const char* str)
	{
		const auto newStr = new uni16ppp();
		genChar->localizedNicknameForSave = reinterpret_cast<UNICODE_STRING***>(newStr);
		gameStringHelpers::createUniString(*genChar->localizedNicknameForSave, str);
		genChar->applyName();
		if (const auto charData = eopCharacterDataDb::instance->getOrCreateData(genChar->giveValidLabel(), genChar->gen ? genChar->gen->getTypeID() : 7))
		{
			charData->localizedEpithet = str;
		}
	}
	
	void setHeir(characterRecord* gen, bool isJustSet)
	{
		factionStruct* fac = gen->faction;
		if (isJustSet)
		{
			for (int i = 0; i < fac->numOfCharacters; i++)
				fac->characters[i]->characterRecord->status &= ~2;
			fac->heir = gen;
			gen->status |= 2;
		}
		callClassFunc<factionStruct*, void, characterRecord*>(fac, 0x20, gen);
	}

#pragma endregion Character Record helpers

	/*----------------------------------------------------------------------------------------------------------------*\
										       Ancillaries
	\*----------------------------------------------------------------------------------------------------------------*/
#pragma region Ancillary helpers
	
	ancillary* findAncillary(const std::string& ancName)
	{
		if (ancName.empty())
			return nullptr;
		if (plugData::data.luaAll.ancillaries.empty())
			plugData::data.luaAll.fillHashMapsVnv();
		const auto ancDb = getAncillaryDb();
		if (const auto ancIndex = plugData::data.luaAll.ancillaries.find(ancName); ancIndex != plugData::data.luaAll.ancillaries.end()) 
			return &ancDb->ancillaries[ancIndex->second];
		gameHelpers::logStringGame("WARNING: findAncillary: Ancillary not found: " + ancName);
		return nullptr;
	}
	
	int addAncillaryName(characterRecord* character, const std::string& ancName)
	{
		ancillary* anc = findAncillary(ancName);
		if (!anc)
			return 0;
		return addAncillary(character, anc);
	}

	int addAncillary(characterRecord* character, ancillary* anc)
	{
		if (character == nullptr || anc == nullptr)
			return 0;
		DWORD adr = codes::offsets.addAncillary;
		int retAnc = 0;
		_asm
		{
			mov ecx, character
			push anc
			mov eax, adr
			call eax
			mov retAnc, eax
		}
		return retAnc;
	}
	
	void removeAncillaryManual(characterRecord* character, ancillary* anc)
	{
		if (character == nullptr || anc == nullptr) return;
		UINT32 index = 0;
		bool found = false;
		for (; index < character->ancNum; index++)
		{
			if (auto currAnc = character->ancillaries[index]->dataAnc; currAnc == anc)
			{
				found = true;
				delete currAnc;
				break;
			}
		}
		if (!found) return;
		--character->ancNum;
		while (index < character->ancNum)
		{
			const auto a1 = character->ancillaries[index + 1];
			const auto a2 = &character->ancillaries[index++];
			*a2 = a1;
		}
	}
	
	void removeAncillary(characterRecord* character, ancillary* anc)
	{
		if (character == nullptr || anc == nullptr) return;
		if (character->deathType > 0 || !character->gen)
		{
			removeAncillaryManual(character, anc);
			return;
		}
		DWORD adr = codes::offsets.removeAncillary;
		_asm
		{
			mov ecx, character
			push anc
			mov eax, adr
			call eax
		}
		return;
	}

#define ancillaryStruct_name 1
#define ancillaryStruct_imagePath 2
#define ancillaryStruct_ancType 3
	//ancillary
	template <char fieldIndex>
	std::string getStringPropertyAnc(const ancillary* anc)
	{
		char* retS = nullptr;
		if (fieldIndex == ancillaryStruct_name)
		{
			retS = anc->ancName;
		}
		else if (fieldIndex == ancillaryStruct_imagePath)
		{
			retS = anc->patchToAncImage;
		}
		else if (fieldIndex == ancillaryStruct_ancType)
		{
			retS = anc->type;
		}

		if (retS != nullptr)
		{
			return std::string(retS);
		}
		else
		{
			return std::string("");
		}
	}

	template <char fieldIndex>
	void setStringPropertyAnc(ancillary* anc, std::string newS)
	{
		if (fieldIndex == ancillaryStruct_name)
			gameStringHelpers::setHashedString(&anc->ancName, newS.c_str());
		else if (fieldIndex == ancillaryStruct_imagePath)
			gameStringHelpers::setHashedString(&anc->patchToAncImage, newS.c_str());
		else if (fieldIndex == ancillaryStruct_ancType)
			gameStringHelpers::setHashedString(&anc->type, newS.c_str());
	}
#pragma endregion Ancillary helpers
	
	/*----------------------------------------------------------------------------------------------------------------*\
									            Traits
    \*----------------------------------------------------------------------------------------------------------------*/
#pragma region Trait helpers	 


	
	int getTraitLevel(const traitContainer* currTrait)
	{
		return currTrait->trait->level->level;
	}
	std::string getTraitName(const traitContainer* currTrait)
	{
		return currTrait->trait->traitEntry->name;
	}
	traitContainer* getNextTrait(const traitContainer* currTrait)
	{
		return currTrait->next;
	}
	traitContainer* getPrevTrait(const traitContainer* currTrait)
	{
		return currTrait->prev;
	}
	traitEffect* getTraitEffect(const traitLevel* level, int index)
	{
		return &level->effects[index];
	}

	traitDb* getTraitDb()
	{
		return reinterpret_cast<traitDb*>(dataOffsets::offsets.traitDb);
	}

	ancillaryDb* getAncillaryDb()
	{
		return reinterpret_cast<ancillaryDb*>(dataOffsets::offsets.ancillaryDb);
	}

	traitEntry* findTrait(const std::string& name)
	{
		if (plugData::data.luaAll.traits.empty())
			plugData::data.luaAll.fillHashMapsVnv();
		const auto traitsDb = getTraitDb();
		if (const auto traitIndex = plugData::data.luaAll.traits.find(name); traitIndex != plugData::data.luaAll.traits.end()) 
			return &traitsDb->traits[traitIndex->second];
		gameHelpers::logStringGame("WARNING: findTrait: Trait not found: " + name);
		return nullptr;
	}
	
	void addTrait(characterRecord* character, const char* traitName, int traitLevel)
	{
		const auto traitsDb = getTraitDb();
		traitEntry* trait = nullptr;
		if (const auto traitIndex = plugData::data.luaAll.traits.find(traitName); traitIndex != plugData::data.luaAll.traits.end()) 
			trait = &traitsDb->traits[traitIndex->second];
		if (!trait)
			return;
		GAME_FUNC(void(__thiscall*)(characterRecord*, traitEntry*, int, bool), addTrait)(character, trait, traitLevel, true);
	}

	void removeTrait(characterRecord* character, const char* traitName)
	{
		const auto traitsDb = getTraitDb();
		traitEntry* trait = nullptr;
		if (const auto traitIndex = plugData::data.luaAll.traits.find(traitName); traitIndex != plugData::data.luaAll.traits.end()) 
			trait = &traitsDb->traits[traitIndex->second];
		if (!trait)
			return;
		GAME_FUNC(void(__thiscall*)(characterRecord*, traitEntry*), removeTrait)(character, trait);
	}
#pragma endregion Trait helpers
	
	void addToLua(sol::state& luaState)
	{
		struct
		{
			sol::usertype<characterRecord>characterRecord;
			sol::usertype<ancillary>ancillary;
		    sol::usertype<traitContainer>traitContainerT;
			sol::usertype<capturedFactionInfo>capturedFactionInfo;
			sol::usertype<capturedUnit>capturedUnit;
			sol::usertype<capturedCharacter>capturedCharacter;
			sol::usertype<trait> traitStruct;
			sol::usertype<traitEntry> traitEntry;
			sol::usertype<traitLevel> traitLevel;
			sol::usertype<traitEffect> traitEffect;
		}types;

		///Character Record
		//@section Character record

		/***
		Basic characterRecord table. All named characters have these fields including dead characters, wives, children, and off-map characters.

		@tfield int index
		@tfield character character
		@tfield string shortName Internal name.
		@tfield string lastName Internal name.
		@tfield string fullName Internal name including surname.
		@tfield string localizedDisplayName Display name, resets upon reloading a save.
		@tfield string label
		@tfield string portrait Wives (who have never been princesses) and children do not have anything for this field. Path to 'young' portrait used starting from 'mods' folder. Resets upon reloading a save. Use setPortrait() to set a new persistent portrait.
		@tfield string portrait2 Wives (who have never been princesses) and children do not have anything for this field. Path to 'old' portrait used starting from 'mods' folder. Resets upon reloading a save. Use setPortrait() to set a new persistent portrait.
		@tfield string portrait_custom Wives (who have never been princesses) and children do not have anything for this field. Folder name in ui/custom_portraits folder. Use setPortrait() to set a new persistent portrait.
		@tfield string modelName Battle model (needs battle_models.modeldb entry).
		@tfield int status 5-leader,2 - heir, 0 - ordinary character, read only, do not set value
		@tfield setAsHeir setAsHeir
		@tfield bool isAlive
		@tfield bool isFamily
		@tfield bool isMale
		@tfield bool wasLeader
		@tfield isOffMap isOffMap
		@tfield bool isChild
		@tfield int age
		@tfield int traitCount
		@tfield float yearOfBirth For example with 4 turns per year, the yearOfBirth could be 1840.25
		@tfield int seasonOfBirth
		@tfield float yearOfDeath
		@tfield int seasonOfDeath
		@tfield int numberOfChildren
		@tfield factionStruct faction
		@tfield int originalFaction
		@tfield int nameFaction
		@tfield int deathType
		@tfield bool wasMarriageAlliance
		@tfield bool isFamilyHead
		@tfield int kills
		@tfield characterRecord parent
		@tfield characterRecord spouse
		@tfield characterRecord[4] children Maximum 4.
		@tfield getTraits getTraits
		@tfield addTrait addTrait
		@tfield removeTrait removeTrait
		@tfield int ancNum
		@tfield getAncillary getAncillary
		@tfield addAncillary addAncillary
		@tfield removeAncillary removeAncillary
		@tfield int level
		@tfield int authority
		@tfield int command
		@tfield int chivalryAndDread positive = Chivalry, negative = Dread
		@tfield int loyalty
		@tfield int piety
		@tfield int ambush
		@tfield int artilleryCommand
		@tfield int assassination
		@tfield int attack
		@tfield int battleSurgery
		@tfield int bodyguardSize
		@tfield int bodyguardValour
		@tfield int boldness
		@tfield int bribeResistance
		@tfield int bribery
		@tfield int cavalryCommand
		@tfield int charm
		@tfield int construction
		@tfield int defence
		@tfield int disposition
		@tfield int electability
		@tfield int eligibility
		@tfield int farming
		@tfield int fertility
		@tfield int finance
		@tfield int footInTheDoor
		@tfield int generosity
		@tfield int gunpowderCommand
		@tfield int health
		@tfield int heresyImmunity
		@tfield int hitpoints Note: Generals have base 5 hitpoints and can gain up to 5 more from traits/ancillaries. Hitpoint values can exceed 10 but effective hitpoints are capped at 10.
		@tfield int infantryCommand
		@tfield int influence
		@tfield int law
		@tfield int lineOfSight
		@tfield int localPopularity
		@tfield int looting
		@tfield int magic
		@tfield int management
		@tfield int mining
		@tfield int movementPointsBonus
		@tfield int navalCommand
		@tfield int nightBattle
		@tfield int personalSecurity
		@tfield int publicSecurity
		@tfield int purity
		@tfield int sabotage
		@tfield int siegeAttack
		@tfield int siegeDefense
		@tfield int siegeEngineering
		@tfield int squalor
		@tfield int subterfuge
		@tfield int taxCollection
		@tfield int trading
		@tfield int trainingAgents
		@tfield int trainingAnimalUnits
		@tfield int trainingUnits
		@tfield int troopMorale
		@tfield int unorthodoxy
		@tfield int unrest
		@tfield int violence
		@tfield int portraitIndex
		@tfield string savedDisplayName
		@tfield string savedDisplayLastName
		@tfield string savedDisplayEpithet
		@tfield int[10] combatVsReligion Maximum 10. EVEN IF YOU SET RELIGION LIMIT.
		@tfield int[31] combatVsFaction Maximum 31.
		@tfield getEopSetModel getEopSetModel
		@tfield giveValidLabel giveValidLabel
		@tfield isLeader isLeader
		@tfield isHeir isHeir
		@tfield hasAncillary hasAncillary
		@tfield getTrait getTrait
		@tfield addTraitPoints addTraitPoints
		@tfield getTraitLevel getTraitLevel
		@tfield giveRandomName giveRandomName
		@tfield setPortrait setPortrait
		@tfield giveRandomPortrait giveRandomPortrait
		@tfield acquireAncillary acquireAncillary
		@tfield hasAncType hasAncType
		@tfield birthChild birthChild
		@tfield marryWife marryWife

		@table characterRecord
		*/
		types.characterRecord = luaState.new_usertype<characterRecord>("characterRecord");
		types.characterRecord.set("index", &characterRecord::index);
		types.characterRecord.set("character", &characterRecord::gen);
		types.characterRecord.set("traitCount", sol::property(&characterRecord::getTraitCount));
		types.characterRecord.set("shortName", sol::property(&getStringPropertyGenChar<characterRecord_shortName>, &setStringPropertyGenChar<characterRecord_shortName>));
		types.characterRecord.set("lastName", sol::property(&getStringPropertyGenChar<characterRecord_lastName>, &setStringPropertyGenChar<characterRecord_lastName>));
		types.characterRecord.set("fullName", sol::property(&getStringPropertyGenChar<characterRecord_fullName>, &setStringPropertyGenChar<characterRecord_fullName>));
		types.characterRecord.set("localizedDisplayName", sol::property(&namedCharUniStringToStr<namedChar_localizedFullName>, &namedCharSetLocalizedFullName));
		types.characterRecord.set("savedDisplayName", sol::property(&namedCharUniStringToStr<namedChar_localizedNameForSave>, &namedCharSetLocalizedNameForSave));
		types.characterRecord.set("savedDisplayLastName", sol::property(&namedCharUniStringToStr<namedChar_localizedNextNameForSave>, &namedCharSetLocalizedNextNameForSave));
		types.characterRecord.set("savedDisplayEpithet", sol::property(&namedCharUniStringToStr<namedChar_localizedNicknameForSave>, &namedCharSetLocalizedNicknameForSave));
		types.characterRecord.set("label", sol::property(&getStringPropertyGenChar<characterRecord_label>, &setStringPropertyGenChar<characterRecord_label>));
		types.characterRecord.set("portrait", sol::property(&getStringPropertyGenChar<characterRecord_portrait>, &setStringPropertyGenChar<characterRecord_portrait>));
		types.characterRecord.set("portrait2", sol::property(&getStringPropertyGenChar<characterRecord_portrait2>, &setStringPropertyGenChar<characterRecord_portrait2>));
		types.characterRecord.set("portrait_custom", sol::property(&getStringPropertyGenChar<characterRecord_portrait_custom>, &setStringPropertyGenChar<characterRecord_portrait_custom>));
		types.characterRecord.set("modelName", sol::property(&getStringPropertyGenChar<characterRecord_modelName>, &characterRecord::setBattleModel));
		types.characterRecord.set("status", &characterRecord::status);
		/***
		Sets the named character as the faction heir.
		@function characterRecord:setAsHeir
		@tparam bool onlyHeir True = this character will be the only heir, false = add another heir (faction can appear to have multiple heirs but only one will become leader).
		@usage
		ourcharacter:setAsHeir(true)
		*/
		types.characterRecord.set_function("setAsHeir", &setHeir);
		/***
		Checks if character is off map, read only, do not set this value.
		@function characterRecord:isOffMap
		@treturn bool offMap
		@usage
		if ourcharacter:isOffMap() == true then
			--do something
		end
		*/
		types.characterRecord.set_function("isOffMap", &characterRecord::isOffMap);
		types.characterRecord.set("isMale", sol::property(&characterRecord::getIsMale, &characterRecord::setIsMale));
		types.characterRecord.set("isAlive", sol::property(&characterRecord::getIsAlive, &characterRecord::setIsAlive));
		types.characterRecord.set("isChild", sol::property(&characterRecord::getIsChild, &characterRecord::setIsChild));
		types.characterRecord.set("isFamily", sol::property(&characterRecord::getIsFamily, &characterRecord::setIsFamily));
		types.characterRecord.set("age", sol::property(&characterRecord::getAge, &characterRecord::setAge));
		types.characterRecord.set("yearOfBirth", &characterRecord::yearOfBirth);
		types.characterRecord.set("seasonOfBirth", &characterRecord::seasonOfBirth);
		types.characterRecord.set("seasonOfDeath", &characterRecord::deathSeason);
		types.characterRecord.set("nameFaction", &characterRecord::nameFaction);
		types.characterRecord.set("yearOfDeath", &characterRecord::deathYear);
		types.characterRecord.set("numberOfChildren", &characterRecord::numberOfChildren);
		types.characterRecord.set("faction", &characterRecord::faction);
		types.characterRecord.set("isFamilyHead", sol::property(&characterRecord::getIsFamilyHead, &characterRecord::setIsFamilyHead));
		types.characterRecord.set("kills", sol::property(&characterRecord::getKills, &characterRecord::setKills));
		types.characterRecord.set("wasLeader", sol::property(&characterRecord::wasLeader, &characterRecord::setWasLeader));
		types.characterRecord.set("wasMarriageAlliance", &characterRecord::wasMarriageAlliance);
		types.characterRecord.set("deathType", &characterRecord::deathType);
		types.characterRecord.set("originalFaction", &characterRecord::originalFaction);
		types.characterRecord.set("parent", &characterRecord::parent);
		types.characterRecord.set("portraitIndex", &characterRecord::portraitIndex);
		types.characterRecord.set("spouse", &characterRecord::spouse);
		types.characterRecord.set("childs", sol::property([](characterRecord& self) { return std::ref(self.childs); }));
		types.characterRecord.set("children", sol::property([](characterRecord& self) { return std::ref(self.childs); }));
		types.characterRecord.set("combatVsReligion", sol::property([](characterRecord& self) { return std::ref(self.combatVsReligion); }));
		types.characterRecord.set("combatVsFaction", sol::property([](characterRecord& self) { return std::ref(self.combatVsFaction); }));
		/***
		Get the pointer to the character's traits container.
		@function characterRecord:getTraits
		@treturn traitContainer The character's traits.
		@usage
		local thisTrait, traitsList, index = selectedChar:getTraits(), selectedChar.fullName.." traits:", 0
		while thisTrait ~= nil do
			traitsList, thisTrait, index = traitsList.."\n\tTrait "..index.." - Name: "..thisTrait.name.." - Level: "..thisTrait.level, thisTrait.nextTrait, index + 1
		end
		print(traitsList)
		*/
		types.characterRecord.set_function("getTraits", &characterRecord::getTraits);
		/***
		Add a trait to the character. Case sensitive.
		@function characterRecord:addTrait
		@tparam string traitName Trait's internal name per export\_descr\_character\_traits.txt
		@tparam int traitLevel Trait's level.
		@usage
		ourNamedCharacter:addTrait("GoodCommander", 2)
		*/
		types.characterRecord.set_function("addTrait", &addTrait);
		/***
		Remove a trait from the character. Case sensitive.
		@function characterRecord:removeTrait
		@tparam string traitName Trait's internal name per export\_descr\_character\_traits.txt
		@usage
		ourNamedCharacter:removeTrait("GoodCommander");
		*/
		types.characterRecord.set_function("removeTrait", &removeTrait);
		types.characterRecord.set("ancNum", &characterRecord::ancNum);
		/***
		Get the pointer to the ancillary using it's index. You can iterate over a character's ancillaries for example by going from index 0 to ancNum - 1.
		@function characterRecord:getAncillary
		@tparam int index
		@treturn ancillary ancillary
		@usage
		ancillary = ourNamedCharacter:getAncillary(2)
		*/
		types.characterRecord.set_function("getAncillary", &characterRecord::getAncillary);
		/***
		Add an ancillary to the named character using the name per export\_descr\_ancillaries.txt.
		@function characterRecord:addAncillary
		@tparam string ancillaryName
		@usage
		ourNamedCharacter:addAncillary("VeryVeryGoodMan");
		*/
		types.characterRecord.set_function("addAncillary", &addAncillaryName);
		/***
		Remove an ancillary from the named character using it's pointer. Use getAncillary function to get the specific ancillary.
		@function characterRecord:removeAncillary
		@tparam ancillary ancillary
		@usage
		ourAnc=ourNamedCharacter:getAncillary(2);
		ourNamedCharacter:removeAncillary(ourAnc);
		*/
		types.characterRecord.set_function("removeAncillary", &removeAncillary);
		/***
		Get the model set with eop.
		@function characterRecord:getEopSetModel
		@treturn string modelName empty if no model set.
		@usage
		      local model = record:getEopSetModel()
		      if model == "" then
				  --do something
		      end
		*/
		types.characterRecord.set_function("getEopSetModel", &characterRecord::getEopSetModel);
		/***
		Give an unused label based on shortName + _eop_number.
		@function characterRecord:giveValidLabel
		@treturn string newLabel
		@usage
		      local newLabel = record:giveValidLabel()
		*/
		types.characterRecord.set_function("giveValidLabel", &characterRecord::giveValidLabel);
		/***
		Check if character is a faction leader.
		@function characterRecord:isLeader
		@treturn bool isLeader
		@usage
		      local isLeader = record:isLeader()
		*/
		types.characterRecord.set_function("isLeader", &characterRecord::isLeader);
		/***
		Check if character is a faction heir.
		@function characterRecord:isHeir
		@treturn bool isHeir
		@usage
		      local isHeir = record:isHeir()
		*/
		types.characterRecord.set_function("isHeir", &characterRecord::isHeir);
		/***
		Check if the character has a certain ancillary.
		@function characterRecord:hasAncillary
		@tparam string ancName
		@treturn bool hasAnc
		@usage
		      local hasAnc = record:hasAncillary("one_ring")
		*/
		types.characterRecord.set_function("hasAncillary", &characterRecord::hasAncillary);
		/***
		Check if the character has a certain ancillary type.
		@function characterRecord:hasAncType
		@tparam string ancType
		@treturn bool hasAnc
		@usage
		      local hasAnc = record:hasAncType("relic_ring")
		*/
		types.characterRecord.set_function("hasAncType", &characterRecord::hasAncType);
		/***
		Get a character trait.
		@function characterRecord:getTrait
		@tparam int index
		@treturn traitStruct trait
		@usage
		      local trait = record:getTrait(0)
		*/
		types.characterRecord.set_function("getTrait", &characterRecord::getTrait);
		/***
		Get the level of a trait the character has (or 0 if the character does not have the trait). Case sensitive.
		@function characterRecord:getTraitLevel
		@tparam string traitName
		@treturn int level
		@usage
		      local level = record:getTraitLevel("GoodCommander")
		*/
		types.characterRecord.set_function("getTraitLevel", &characterRecord::getTraitLevel);
		/***
		Add trait points to a trait, will upgrade/downgrade the level if it passes the thresholds.  Case sensitive.
		@function characterRecord:addTraitPoints
		@tparam string traitName
		@tparam int points
		@usage
		      record:addTraitPoints("GoodCommander", 3)
		*/
		types.characterRecord.set_function("addTraitPoints", &characterRecord::addTraitPoints);
		/***
		Give character an ancillary, checks excluded cultures.
		@function characterRecord:acquireAncillary
		@tparam string ancillaryName
		@tparam bool noDuplicate Only add if character doesn't already have this ancillary.
		@usage
		      record:acquireAncillary("ancillary_name", true)
		*/
		types.characterRecord.set_function("acquireAncillary", &characterRecord::acquireAncillary);
		/***
		Give a random name.
		@function characterRecord:giveRandomName
		@tparam int nameFactionId
		@usage
		      record:giveRandomName(3)
		*/
		types.characterRecord.set_function("giveRandomName", &characterRecord::giveRandomName);
		/***
		Set a characters portrait. Use a relative path from the mod folder! This is persistent and the recommended method for setting a portrait, including custom ones.
		@function characterRecord:setPortrait
		@tparam string portraitPath
		@usage
			  // Setting specific portrait from the generic folder    
		      record:setPortrait("data/ui/mesoamerican/portraits/portraits/young/generals/028.tga")
			  // Setting specific portrait from the custom folder    
		      record:setPortrait("/data/ui/custom_portraits/saruman_manycolours/portrait_young.tga")
		*/
		types.characterRecord.set_function("setPortrait", &characterRecord::setPortrait);
		/***
		Select a random portrait from a culture for a character.
		@function characterRecord:giveRandomPortrait
		@tparam int cultureID
		@tparam int religionID (Only for cultures that are set up with religion variations! Use -1 to disable.)
		@usage
		      record:giveRandomPortrait(M2TWEOP.getCultureID("southern_european"), -1)
		*/
		types.characterRecord.set_function("giveRandomPortrait", &characterRecord::giveRandomPortrait);
		/***
		Create a new child for the character. You need to select the father for this.
		@function characterRecord:birthChild
		@tparam string name random_name for random
		@tparam string lastName
		@tparam int age
		@tparam bool isMale
		@tparam bool isAlive
		@treturn characterRecord child
		@usage
		      local newKid = record:birthChild("random_name", "", 0, true, true)
		*/
		types.characterRecord.set_function("birthChild", &characterRecord::birthChild);
		/***
		Create and marry a wife.
		@function characterRecord:marryWife
		@tparam string name random_name for random
		@tparam int age
		@treturn characterRecord wife
		@usage
		      local wife = record:marryWife("random_name", 30)
		*/
		types.characterRecord.set_function("marryWife", &characterRecord::marryWife);
			
		types.characterRecord.set("level", &characterRecord::level);
		types.characterRecord.set("authority", &characterRecord::authority);
		types.characterRecord.set("command", &characterRecord::command);
		types.characterRecord.set("chivalryAndDread", &characterRecord::chivalry);
		types.characterRecord.set("loyalty", &characterRecord::loyalty);
		types.characterRecord.set("piety", &characterRecord::piety);
		types.characterRecord.set("influence", &characterRecord::influence);
		types.characterRecord.set("subterfuge", &characterRecord::subterfuge);
		types.characterRecord.set("charm", &characterRecord::charm);
		types.characterRecord.set("finance", &characterRecord::finance);
		types.characterRecord.set("magic", &characterRecord::magic);
		types.characterRecord.set("unorthodoxy", &characterRecord::unorthodoxy);
		types.characterRecord.set("heresyImmunity", &characterRecord::heresyImmunity);
		types.characterRecord.set("assassination", &characterRecord::assassination);
		types.characterRecord.set("sabotage", &characterRecord::sabotage);
		types.characterRecord.set("eligibility", &characterRecord::eligibility);
		types.characterRecord.set("purity", &characterRecord::purity);
		types.characterRecord.set("violence", &characterRecord::violence);
		types.characterRecord.set("disposition", &characterRecord::disposition);
		types.characterRecord.set("boldness", &characterRecord::boldness);
		types.characterRecord.set("generosity", &characterRecord::generosity);
		types.characterRecord.set("management", &characterRecord::management);
		types.characterRecord.set("bodyguardSize", &characterRecord::bodyguardSize);
		types.characterRecord.set("troopMorale", &characterRecord::troopMorale);
		types.characterRecord.set("movementPointsBonus", &characterRecord::movementPointsBonus);
		types.characterRecord.set("attack", &characterRecord::attack);
		types.characterRecord.set("defence", &characterRecord::defence);
		types.characterRecord.set("siegeAttack", &characterRecord::siegeAttack);
		types.characterRecord.set("siegeDefense", &characterRecord::siegeDefense);
		types.characterRecord.set("ambush", &characterRecord::ambush);
		types.characterRecord.set("navalCommand", &characterRecord::navalCommand);
		types.characterRecord.set("siegeEngineering", &characterRecord::siegeEngineering);
		types.characterRecord.set("nightBattle", &characterRecord::nightBattle);
		types.characterRecord.set("personalSecurity", &characterRecord::personalSecurity);
		types.characterRecord.set("publicSecurity", &characterRecord::publicSecurity);
		types.characterRecord.set("bribery", &characterRecord::bribery);
		types.characterRecord.set("bribeResistance", &characterRecord::bribeResistance);
		types.characterRecord.set("electability", &characterRecord::electability);
		types.characterRecord.set("lineOfSight", &characterRecord::lineOfSight);
		types.characterRecord.set("trainingUnits", &characterRecord::trainingUnits);
		types.characterRecord.set("trainingAgents", &characterRecord::trainingAgents);
		types.characterRecord.set("construction", &characterRecord::construction);
		types.characterRecord.set("trading", &characterRecord::trading);
		types.characterRecord.set("localPopularity", &characterRecord::localPopularity);
		types.characterRecord.set("footInTheDoor", &characterRecord::footInTheDoor);
		types.characterRecord.set("farming", &characterRecord::farming);
		types.characterRecord.set("mining", &characterRecord::mining);
		types.characterRecord.set("taxCollection", &characterRecord::taxCollection);
		types.characterRecord.set("fertility", &characterRecord::fertility);
		types.characterRecord.set("cavalryCommand", &characterRecord::cavalryCommand);
		types.characterRecord.set("infantryCommand", &characterRecord::infantryCommand);
		types.characterRecord.set("gunpowderCommand", &characterRecord::gunpowderCommand);
		types.characterRecord.set("artilleryCommand", &characterRecord::artilleryCommand);
		types.characterRecord.set("health", &characterRecord::health);
		types.characterRecord.set("squalor", &characterRecord::squalor);
		types.characterRecord.set("unrest", &characterRecord::unrest);
		types.characterRecord.set("law", &characterRecord::law);
		types.characterRecord.set("looting", &characterRecord::looting);
		types.characterRecord.set("bodyguardValour", &characterRecord::bodyguardValour);
		types.characterRecord.set("hitpoints", &characterRecord::hitpoints);
		types.characterRecord.set("trainingAnimalUnits", &characterRecord::trainingAnimalUnits);
		types.characterRecord.set("battleSurgery", &characterRecord::battleSurgery);

		
		///Ancillary
		//@section Ancillary

		/***
		Basic ancillary table
	
		@tfield int index
		@tfield string localizedName
		@tfield string description
		@tfield string effectsDescription
		@tfield string name (internal)
		@tfield int effectsNum
		@tfield bool isUnique
		@tfield bool transferable
		@tfield string type
		@tfield string imagePath
		@tfield getEffect getEffect
		@tfield isCultureExcluded isCultureExcluded
	
		@table ancillary
		*/
		types.ancillary = luaState.new_usertype<ancillary>("ancillary");
		types.ancillary.set("index", &ancillary::index);
		types.ancillary.set("isUnique", &ancillary::isUnique);
		types.ancillary.set("effectsNum", &ancillary::effectsNum);
		types.ancillary.set("transferable", &ancillary::transferable);
		types.ancillary.set("name", sol::property(
			&getStringPropertyAnc<ancillaryStruct_name>, &setStringPropertyAnc<ancillaryStruct_name>
			));
		types.ancillary.set("type", sol::property(
			&getStringPropertyAnc<ancillaryStruct_ancType>, &setStringPropertyAnc<ancillaryStruct_ancType>
			));
		types.ancillary.set("imagePath", sol::property(
			&getStringPropertyAnc<ancillaryStruct_imagePath>, &setStringPropertyAnc<ancillaryStruct_imagePath>
			));
		types.ancillary.set("localizedName", sol::property(
			&ancillary::getLocalizedName, &ancillary::setLocalizedName
			));
		types.ancillary.set("description", sol::property(
			&ancillary::getLocalizedDescr, &ancillary::setLocalizedDescr
			));
		types.ancillary.set("effectsDescription", sol::property(
			&ancillary::getLocalizedEffectsDescr, &ancillary::setLocalizedEffectsDescr
			));
		/***
		Get an ancillary effect.
		@function ancillary:getEffect
		@tparam int index
		@treturn traitEffect effect
		@usage
		      local eff = anc:getEffect(2);
		*/
		types.ancillary.set_function("getEffect", &ancillary::getEffect);
		/***
		Is culture exluded?
		@function ancillary:isCultureExcluded
		@tparam int cultureId
		@treturn bool isExcluded
		@usage
		      local isExcluded = anc:isCultureExcluded(1);
		*/
		types.ancillary.set_function("isCultureExcluded", &ancillary::isExcluded);
		
		///Captured Faction Info
		//@section Captured Faction Info

		/***
		Basic capturedFactionInfo table.

		@tfield int targetFactionID
		@tfield int factionID
		@tfield characterRecord namedChar
		@tfield characterRecord targetCharacter
		@tfield getCapturedCharacter getCapturedCharacter
		@tfield int capturedCharactersNum
		@tfield getCapturedUnit getCapturedUnit
		@tfield int capturedUnitsNum
		@tfield int ransomValue
		@tfield bool heirCaptured
		@tfield bool leaderCaptured

		@table capturedFactionInfo
		*/
		types.capturedFactionInfo = luaState.new_usertype<capturedFactionInfo>("capturedFactionInfo");
		types.capturedFactionInfo.set("targetFactionID", &capturedFactionInfo::targetFactionID);
		types.capturedFactionInfo.set("factionID", &capturedFactionInfo::factionID);
		types.capturedFactionInfo.set("namedChar", &capturedFactionInfo::character);
		types.capturedFactionInfo.set("targetCharacter", &capturedFactionInfo::targetCharacter);
		types.capturedFactionInfo.set("capturedCharactersNum", sol::property(&capturedFactionInfo::getCapturedCharacterNum));
		types.capturedFactionInfo.set("capturedUnitsNum", sol::property(&capturedFactionInfo::getCapturedUnitNum));
		types.capturedFactionInfo.set("ransomValue", &capturedFactionInfo::ransomValue);
		types.capturedFactionInfo.set("heirCaptured", &capturedFactionInfo::heirCaptured);
		types.capturedFactionInfo.set("leaderCaptured", &capturedFactionInfo::leaderCaptured);

		/***
		Get a captured unit by it's index.
		@function capturedFactionInfo:getCapturedUnit
		@tparam int index
		@treturn capturedUnit capUnit
		@usage
		capUnit = capInfo:getCapturedUnit(0);
		*/
		types.capturedFactionInfo.set_function("getCapturedUnit", &capturedFactionInfo::getCapturedUnit);

		/***
		Get a captured character by it's index.
		@function capturedFactionInfo:getCapturedCharacter
		@tparam int index
		@treturn capturedCharacter capChar
		@usage
		capUnit = capInfo:getCapturedCharacter(0);
		*/
		types.capturedFactionInfo.set_function("getCapturedCharacter", &capturedFactionInfo::getCapturedCharacter);

		/***
		Basic capturedUnit table.

		@tfield unit unit
		@tfield int capturedSoldiers
		@tfield int capturedValue

		@table capturedUnit
		*/
		types.capturedUnit = luaState.new_usertype<capturedUnit>("capturedUnit");
		types.capturedUnit.set("unit", &capturedUnit::unit);
		types.capturedUnit.set("capturedSoldiers", &capturedUnit::capturedSoldiers);
		types.capturedUnit.set("capturedValue", &capturedUnit::capturedValue);
		
		/***
		Basic capturedCharacter table.
	
		@tfield characterRecord namedChar
		@tfield int capturedValue
	
		@table capturedCharacter
		*/
		types.capturedCharacter = luaState.new_usertype<capturedCharacter>("capturedCharacter");
		types.capturedCharacter.set("namedChar", &capturedCharacter::namedChar);
		types.capturedCharacter.set("capturedValue", &capturedCharacter::capturedValue);

		///Traits
		//@section Traits

		/***
		Basic traits table
	
		@tfield int level
		@tfield string name
		@tfield traitContainer nextTrait
		@tfield traitContainer prevTrait
		@tfield traitStruct traitInfo
	
		@table traitContainer
		*/
		types.traitContainerT = luaState.new_usertype<traitContainer>("traitContainer");
		types.traitContainerT.set("level", sol::property(&getTraitLevel));
		types.traitContainerT.set("name", sol::property(&getTraitName));
		types.traitContainerT.set("nextTrait", sol::property(&getNextTrait));
		types.traitContainerT.set("prevTrait", sol::property(&getPrevTrait));
		types.traitContainerT.set("traitInfo", &traitContainer::trait);

		/***
		Basic trait table
	
		@tfield traitEntry traitEntry
		@tfield traitLevel levelEntry
		@tfield int traitPoints
	
		@table traitStruct
		*/
		types.traitStruct = luaState.new_usertype<trait>("traitStruct");
		types.traitStruct.set("traitEntry", &trait::traitEntry);
		types.traitStruct.set("levelEntry", &trait::level);
		types.traitStruct.set("traitPoints", &trait::traitPoints);
		
		/***
		Basic traitEntry table
	
		@tfield int index
		@tfield string name
		@tfield traitLevel[10] levels Maximum: 10, first real level seems to be at index 1 (so 2 in lua), use function instead
		@tfield int levelCount will be one over the actual count
		@tfield traitEntry[20] antiTraits Maximum: 20
		@tfield int antiTraitCount
		@tfield int noGoingBackLevel
		@tfield bool hidden
		@tfield getLevel getLevel
		@tfield isCharacterTypeValid isCharacterTypeValid
		@tfield isCultureExcluded isCultureExcluded
	
		@table traitEntry
		*/
		types.traitEntry = luaState.new_usertype<traitEntry>("traitEntry");
		types.traitEntry.set("index", &traitEntry::index);
		types.traitEntry.set("name", &traitEntry::name);
		types.traitEntry.set("levels", sol::property([](traitEntry& self) { return std::ref(self.levels); }));
		types.traitEntry.set("antiTraits", sol::property([](traitEntry& self) { return std::ref(self.antiTraits); }));
		types.traitEntry.set("levelCount", &traitEntry::levelCount);
		types.traitEntry.set("antiTraitCount", &traitEntry::antiTraitCount);
		types.traitEntry.set("noGoingBackLevel", &traitEntry::noGoingBackLevel);
		types.traitEntry.set("hidden", &traitEntry::hidden);
		
		/***
		Get trait level, dont need to account for the first level being at index 1.
		@function traitEntry:getLevel
		@tparam int index
		@treturn traitLevel lvl
		@usage
			local lvl = entry:getLevel(0);
		*/
		types.traitEntry.set_function("getLevel", &traitEntry::getLevel);
		
		/***
		Is character type valid?
		@function traitEntry:isCharacterTypeValid
		@tparam int characterType
		@treturn bool isValid
		@usage
			local isValid = entry:isCharacterTypeValid(characterType.spy);
		*/
		types.traitEntry.set_function("isCharacterTypeValid", &traitEntry::isCharacterTypeValid);
		
		/***
		Is culture excluded?
		@function traitEntry:isCultureExcluded
		@tparam int cultureId
		@treturn bool isExcluded
		@usage
			local isExcluded = entry:isCultureExcluded(1);
		*/
		types.traitEntry.set_function("isCultureExcluded", &traitEntry::isExcluded);
		
		/***
		Basic traitLevel table
	
		@tfield int level
		@tfield int threshold
		@tfield int effectsCount
		@tfield string name
		@tfield string description
		@tfield string effectsDescription
		@tfield string epithetDescription
		@tfield string gainDescription
		@tfield string loseDescription
		
		@tfield getTraitEffect getTraitEffect
	
		@table traitLevel
		*/
		types.traitLevel = luaState.new_usertype<traitLevel>("traitLevel");
		types.traitLevel.set("level", &traitLevel::level);
		types.traitLevel.set("threshold", &traitLevel::threshold);
		types.traitLevel.set("effectsCount", &traitLevel::effectsCount);
		types.traitLevel.set("name", sol::property(&traitLevel::getLocalizedName, &traitLevel::setLocalizedName));
		types.traitLevel.set("description", sol::property(&traitLevel::getLocalizedDescription, &traitLevel::setLocalizedDescription));
		types.traitLevel.set("effectsDescription", sol::property(&traitLevel::getLocalizedEffectsDescription, &traitLevel::setLocalizedEffectsDescription));
		types.traitLevel.set("epithetDescription", sol::property(&traitLevel::getLocalizedEpithetDescription, &traitLevel::setLocalizedEpithetDescription));
		types.traitLevel.set("gainDescription", sol::property(&traitLevel::getLocalizedGainDescription, &traitLevel::setLocalizedGainDescription));
		types.traitLevel.set("loseDescription", sol::property(&traitLevel::getLocalizedLoseDescription, &traitLevel::setLocalizedLoseDescription));
		
		/***
		Get trait effect.
		@function traitLevel:getTraitEffect
		@tparam int index
		@treturn traitEffect effect
		@usage
			local effect = level:getTraitEffect(0);
		*/
		types.traitLevel.set_function("getTraitEffect", &getTraitEffect);

		/***
		Basic traitEffect table
	
		@tfield int id
		@tfield int value
	
		@table traitEffect
		*/
		types.traitEffect = luaState.new_usertype<traitEffect>("traitEffect");
		types.traitEffect.set("id", &traitEffect::effectID);
		types.traitEffect.set("value", &traitEffect::value);

	}

	int getAncillaryCount()
	{
		return  getAncillaryDb()->ancillariesNum;
	}

	ancillary* getAncillaryByIndex(const int index)
	{
		return getAncillaryDb()->getAncillary(index);
	}

	int getTraitCount()
	{
		return getTraitDb()->traitsNum;
	}

	traitEntry* getTraitByIndex(const int index)
	{
		return getTraitDb()->getTrait(index);
	}
	
}

std::string ancillary::getLocalizedName()
{
	return gameStringHelpers::uniStringToStr(*localizedAncName);
}

void ancillary::setLocalizedName(const std::string& newName)
{
	gameStringHelpers::createUniString(*localizedAncName, newName.c_str());
}

std::string ancillary::getLocalizedDescr()
{
	return gameStringHelpers::uniStringToStr(*localizedAncDesc);
}

void ancillary::setLocalizedDescr(const std::string& newDescr)
{
	gameStringHelpers::createUniString(*localizedAncDesc, newDescr.c_str());
}

std::string ancillary::getLocalizedEffectsDescr()
{
	return gameStringHelpers::uniStringToStr(*effectsDescr);
}

void ancillary::setLocalizedEffectsDescr(const std::string& newDescr)
{
	effectsDescr = techFuncs::createGameClass<UNICODE_STRING**>();
	gameStringHelpers::createUniString(*effectsDescr, newDescr.c_str());
}

characterRecord* characterRecord::birthChild(const std::string& name, const std::string& childLastName, const int childAge,
	const bool childMale, const bool childAlive)
{
	if (!this->isMale)
	{
		gameHelpers::logStringGame("characterRecord::birthChild: only fathers can birth children");
		return nullptr;
	}
	if (!this->spouse)
	{
		gameHelpers::logStringGame("characterRecord::birthChild: character does not have a spouse");
		return nullptr;
	}
	if (this->numberOfChildren > 7)
	{
		gameHelpers::logStringGame("characterRecord::birthChild: can only have 8 children");
		return nullptr;
	}
	const auto campaignDb = campaignHelpers::getCampaignDb();
	if (this->numberOfChildren == campaignDb->campaignDbFamilyTree.maxNumberOfChildren)
	{
		gameHelpers::logStringGame("characterRecord::birthChild: character at campaign db max number of children");
		return nullptr;
	}
	if ((age - childAge < static_cast<uint32_t>(campaignDb->campaignDbFamilyTree.ageOfManhood))
		||(spouse->age - childAge < static_cast<uint32_t>(campaignDb->campaignDbFamilyTree.daughtersAgeOfConsent)))
	{
		gameHelpers::logStringGame("characterRecord::birthChild: character is not old enough to birth this child");
		return nullptr;
	}
	if (childMale && childAge >= campaignDb->campaignDbFamilyTree.ageOfManhood)
	{
		gameHelpers::logStringGame("characterRecord::birthChild: child is too old, create a general and adopt him");
		return nullptr;
	}
	const auto newRecord = this->faction->newRecord();
	newRecord->setAge(childAge);
	newRecord->setWasLeader(false);
	newRecord->isMale = childMale;
	if (name == "random_name" || name.empty())
	{
		auto [rName, rName2] = characterRecordHelpers::getRandomNames(faction, this->faction->factionID, isMale);
		if (!childLastName.empty())
			rName2 = childLastName.c_str();
		GAME_FUNC(void(__thiscall*)(characterRecord*, const char*, const char*, UNICODE_STRING***, int, bool)
				  , setCharacterName)(newRecord, rName, rName2, localizedNicknameForSave, 7, false);
	}
	else
	{
		GAME_FUNC(void(__thiscall*)(characterRecord*, const char*, const char*, UNICODE_STRING***, int, bool)
		          , setCharacterName)(newRecord, name.c_str(), childLastName.c_str(), localizedNicknameForSave, 7, false);
	}
	newRecord->yearOfBirth = campaignHelpers::getCampaignData()->currentDate - static_cast<float>(childAge);
	newRecord->seasonOfBirth = childAge == 0 ? campaignHelpers::getCampaignData()->season : campaignHelpers::getCampaignData()->tickCount % 2;
	newRecord->isAlive = childAlive;
	if (!childAlive)
	{
		newRecord->deathYear = campaignHelpers::getCampaignData()->currentDate;
		newRecord->deathSeason = campaignHelpers::getCampaignData()->season;
	}
	if (isMale || (childAge < campaignDb->campaignDbFamilyTree.daughtersAgeOfConsent))
		newRecord->isChild = true;
	newRecord->parent = this;
	newRecord->isFamily = true;
	this->childs[numberOfChildren++] = newRecord;
	GAME_FUNC(void(__cdecl*)(characterRecord*), birthLog)(newRecord);
	return newRecord;
}

characterRecord* characterRecord::marryWife(const std::string& name, int wifeAge)
{
	if (!isMale)
	{
		gameHelpers::logStringGame("characterRecord::marryWife: Character is not male");
		return nullptr;
	}
	if (spouse)
	{
		gameHelpers::logStringGame("characterRecord::marryWife: Character already married");
		return nullptr;
	}
	const auto campaignDb = campaignHelpers::getCampaignDb();
	if (age < static_cast<uint32_t>(campaignDb->campaignDbFamilyTree.ageOfManhood))
	{
		gameHelpers::logStringGame("characterRecord::marryWife: Character is too young");
		return nullptr;
	}
	if (wifeAge < campaignDb->campaignDbFamilyTree.daughtersAgeOfConsent)
	{
		gameHelpers::logStringGame("characterRecord::marryWife: Wife is too young");
		return nullptr;
	}
	const auto newRecord = this->faction->newRecord();
	newRecord->setAge(wifeAge);
	newRecord->setWasLeader(false);
	newRecord->isMale = false;
	if (name == "random_name" || name.empty())
		newRecord->giveRandomName(this->faction->factionID);
	else
	{
		GAME_FUNC(void(__thiscall*)(characterRecord*, const char*, const char*, UNICODE_STRING***, int, bool)
		, setCharacterName)(newRecord, name.c_str(), "", localizedNicknameForSave, 7, false);
	}
	newRecord->yearOfBirth = campaignHelpers::getCampaignData()->currentDate - static_cast<float>(wifeAge);
	newRecord->seasonOfBirth = campaignHelpers::getCampaignData()->tickCount % 2;
	newRecord->isAlive = true;
	newRecord->isFamily = true;
	GAME_FUNC(void(__thiscall*)(characterRecord*, characterRecord*), marryCharacters)(newRecord, this);
	return newRecord;
}

std::string characterRecord::giveValidLabel()
{
	if (labelCrypt != 0 && label)
		return label;
	std::string name;
	if (shortName)
		name = shortName;
	if (name.empty())
		name = "unnamed";
	int nameCount = 1;
	std::string testLabel;
	const auto campData = campaignHelpers::getCampaignData();
	while (true)
	{
		testLabel = name + "_eop_" + std::to_string(nameCount);
		if (campData->getCharacterByLabel(testLabel))
			nameCount++;
		else
			break;
	}
	gameStringHelpers::setHashedString(&label, testLabel.c_str());
	return label;
}

void characterRecord::removeEpithet()
{
	std::string firstNameStr;
	if (localizedNameForSave)
		firstNameStr = characterRecordHelpers::namedCharUniStringToStr<namedChar_localizedNameForSave>(this);
	std::string lastNameStr;
	if (localizedNextNameForSave)
		lastNameStr = characterRecordHelpers::namedCharUniStringToStr<namedChar_localizedNextNameForSave>(this);
	if (firstNameStr.empty() && lastNameStr.empty())
		return;
	std::string fullNameStr = firstNameStr;
	if (!lastNameStr.empty())
		fullNameStr += " " + lastNameStr;
	characterRecordHelpers::namedCharSetLocalizedFullName(this, fullNameStr.c_str());
}

void characterRecord::applyName()
{
	std::string name;
	if (gen)
	{
		if (gen->getTypeID() == characterTypeStrat::general)
			name = "Captain ";
		if (gen->getTypeID() == characterTypeStrat::admiral)
			name = "Admiral ";
	}
	if (localizedNameForSave)
		name += characterRecordHelpers::namedCharUniStringToStr<namedChar_localizedNameForSave>(this);
	if (localizedNextNameForSave)
		name += " " + characterRecordHelpers::namedCharUniStringToStr<namedChar_localizedNextNameForSave>(this);
	if (localizedNicknameForSave)
		name += " " + characterRecordHelpers::namedCharUniStringToStr<namedChar_localizedNicknameForSave>(this);
	characterRecordHelpers::namedCharSetLocalizedFullName(this, name.c_str());
}

void characterRecord::acquireAncillary(const std::string& name, const bool noDuplicate)
{
	if (gen && gen->getTypeID() == characterTypeStrat::general)
		return;
	const auto anc = characterRecordHelpers::findAncillary(name);
	if (!anc)
		return;
	if (anc->isExcluded(faction->cultureID))
		return;
	if (noDuplicate && hasAncillary(name))
		return;
	characterRecordHelpers::addAncillary(this, anc);
}

void characterRecord::setPortrait(const std::string& portraitPath)
{
	string path = gameHelpers::getModPath();
	path.append("/");
	path.append(portraitPath);
	if (!std::filesystem::exists(path))
	{
		gameHelpers::logStringGame("characterRecord::setPortrait: file not found: " + path);
		return;
	}
	if (const auto charData = eopCharacterDataDb::instance->getOrCreateData(giveValidLabel(), gen ? gen->getTypeID() : 7))
	{
		charData->portrait = portraitPath;
		charData->portrait2 = portraitPath;
	}
	std::string setPath = "mods/";
	setPath.append(gameHelpers::getModFolderName());
	setPath.append("/");
	setPath.append(portraitPath);
	characterRecordHelpers::setStringPropertyGenChar<characterRecord_portrait>(this, setPath);
	characterRecordHelpers::setStringPropertyGenChar<characterRecord_portrait2>(this, setPath);
}

void characterRecord::giveRandomName(const int nameFactionId)
{
	auto [name, name2] = characterRecordHelpers::getRandomNames(faction, nameFactionId, isMale);
	GAME_FUNC(void(__thiscall*)(characterRecord*, const char*, const char*, UNICODE_STRING***, int, bool)
		, setCharacterName)(this, name, name2, localizedNicknameForSave, gen ? gen->getTypeID() : 7, false);
}

void characterRecord::giveRandomPortrait(const int cultureId, const int religionId)
{
	const auto randomPortrait = cultures::getRandomPortrait(cultureId, religionId);
	if (!randomPortrait.empty())
		setPortrait(randomPortrait);
	else
	{
		gameHelpers::logStringGame("characterRecord::giveRandomPortrait: no portrait found for cultureId: " + std::to_string(cultureId) + " religionId: " + std::to_string(religionId));
		gameHelpers::logStringGame("Make sure the portraits start with 000.tga");
	}
}

stringWithHash* LOOKUP_STRING_ANC = new stringWithHash();

bool characterRecord::hasAncillary(const std::string& ancName)
{
	gameStringHelpers::setHashedString(&LOOKUP_STRING_ANC->name, ancName.c_str());
	for (uint32_t i = 0; i < ancNum; i++)
	{
		if (const auto anc = ancillaries[i];
			anc->dataAnc->ancNameHash == LOOKUP_STRING_ANC->hash
			&& strcmp(anc->dataAnc->ancName, ancName.c_str()) == 0)
			return true;
	}
	return false;
}

bool characterRecord::hasAncType(const std::string& ancType)
{
	gameStringHelpers::setHashedString(&LOOKUP_STRING_ANC->name, ancType.c_str());
	for (uint32_t i = 0; i < ancNum; i++)
	{
		if (const auto anc = ancillaries[i];
			anc->dataAnc->typeHash == LOOKUP_STRING_ANC->hash
			&& strcmp(anc->dataAnc->type, ancType.c_str()) == 0)
			return true;
	}
	return false;
}

std::string characterRecord::getEopSetModel()
{
	if (const auto data = eopCharacterDataDb::get()->getCharacterData(label);
		data && !data->model.empty())
		return data->model;
	return "";
}

stringWithHash* LOOKUP_STRING = new stringWithHash();

int characterRecord::getTraitLevel(const std::string& traitName)
{
	gameStringHelpers::setHashedString(&LOOKUP_STRING->name, traitName.c_str());
	const int traitCount = getTraitCount();
	for (int i = 0; i < traitCount; i++)
	{
		if (const auto trait = &traitList[i];
			trait->traitEntry->nameHash == LOOKUP_STRING->hash
			&& strcmp(trait->traitEntry->name, traitName.c_str()) == 0)
			return trait->level->level;
	}
	return 0;
}

void characterRecord::setBattleModel(const std::string& model)
{
	if (const auto battleMod = unitHelpers::findBattleModel(model.c_str()); !battleMod)
	{
		gameHelpers::logStringGame("characterRecord::setBattleModel: model not found: " + model);
		return;
	}
	gameStringHelpers::setHashedString(&this->modelName, model.c_str());
}

void characterRecord::addTraitPoints(const std::string& trait, const int points)
{
	const auto entry = characterRecordHelpers::findTrait(trait);
	if (!entry)
		return;
	bool isCharTypeValid;
	if (gen)
		isCharTypeValid = entry->isCharacterTypeValid(gen->getTypeID());
	else
		isCharTypeValid = entry->isCharacterTypeValid(static_cast<int>(characterTypeStrat::namedCharacter));
	if (!isCharTypeValid)
		return;
	if (entry->isExcluded(faction->cultureID))
		return;
	const int addPoints = points;
	GAME_FUNC(void(__thiscall*)(characterRecord*, traitEntry*, int, bool),
		addTraitPoints)(this, entry, addPoints, true);
}

void eopCharacterDataDb::validate()
{
	const auto campaign = campaignHelpers::getCampaignData();
	for (auto it = instance->eopCharData->begin(); it != instance->eopCharData->end();)
	{
		if (const auto rec = campaign->getCharacterByLabel(it->first);
			!campaign->getCharacterByLabel(it->first) || (rec && rec->gen && rec->gen->getTypeID() != it->second->characterType))
			it = instance->eopCharData->erase(it);
		else
			++it;
	}
}

std::string eopCharacterDataDb::onGameSave()
{
	std::string fPath = gameHelpers::getModPath();
	fPath += "\\eopData\\TempSaveData";
	std::string outFile = fPath;
	outFile += "\\characterData.json";
	ofstream f1(outFile);
	jsn::json json = serialize();
	f1 << setw(4) << json;
	f1.close();
	return outFile;
}

void eopCharacterDataDb::onGameLoad(const std::vector<std::string>& filePaths)
{
	for (auto& path : filePaths)
	{
		if (path.find("characterData.json") == string::npos)
			continue;
		jsn::json json;
		try
		{
			std::ifstream file(path);
			file >> json;
			file.close();
		}
		catch (jsn::json::parse_error& e)
		{
			MessageBoxA(nullptr, e.what(), "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
		}
		try
		{
			deserialize(json);
			m_Loaded = true;
		}
		catch (jsn::json::exception& e)
		{
			MessageBoxA(nullptr, e.what(), "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
		}
		return;
	}
}

void eopCharacterDataDb::onGameLoaded()
{
	const auto campaign = campaignHelpers::getCampaignData();
	if (!campaign)
		return;
	const auto facCount = campaign->factionCount;
	for (int i = 0; i < facCount; i++)
	{
		const auto fac = campaign->getFactionByOrder(i);
		const int charNum = fac->numOfCharacters;
		for (int c = 0; c < charNum; c++)
		{
			const auto thisChar = fac->characters[c];
			if (const auto rec = thisChar->characterRecord; rec && rec->labelCrypt != 0 && rec->label)
			{
				if (const auto data = getCharacterData(rec->label); data)
				{
					if (rec->gen->getTypeID() == data->characterType)
					{
						if (!data->model.empty())
							stratModelsChange::setCharacterModel(thisChar, data->model.c_str());
						if (!data->portrait.empty())
							rec->setPortrait(data->portrait);
						if (!data->internalName.empty())
							characterRecordHelpers::setStringPropertyGenChar<characterRecord_shortName>(rec, data->internalName);
						if (!data->internalLastName.empty())
							characterRecordHelpers::setStringPropertyGenChar<characterRecord_lastName>(rec, data->internalLastName);
						if (!data->localizedDisplayName.empty())
							characterRecordHelpers::namedCharSetLocalizedFullName(rec, data->localizedDisplayName.c_str());
						if (!data->localizedName.empty())
							characterRecordHelpers::namedCharSetLocalizedNameForSave(rec, data->localizedName.c_str());
						if (!data->localizedLastName.empty())
							characterRecordHelpers::namedCharSetLocalizedNextNameForSave(rec, data->localizedLastName.c_str());
						if (!data->localizedEpithet.empty())
							characterRecordHelpers::namedCharSetLocalizedNicknameForSave(rec, data->localizedEpithet.c_str());
					}
					else
						eopCharData->erase(rec->label);
				}
			}
		}
	}
}
