///
//![Lua logo](../Lua.png)
//@module LuaPlugin
//@author Fynn
//@license GPL-3.0
#include "characterRecord.h"

#include "gameStringHelpers.h"
#include "functionsOffsets.h"
#include "character.h"
#include "faction.h"
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
			return gameStringHelpers::uniStringToStr(genChar->localizedNameForSave);
		if (fieldIndex == namedChar_localizedNextNameForSave)
			return gameStringHelpers::uniStringToStr(genChar->localizedNextNameForSave);
		if (fieldIndex == namedChar_localizedNicknameForSave)
			return gameStringHelpers::uniStringToStr(genChar->localizedNicknameForSave);
		return "";
	}
	
	void namedCharSetLocalizedFullName(characterRecord* genChar, const char* str)
	{
		gameStringHelpers::createUniString(genChar->localizedFullName, str);
	}

	void namedCharSetLocalizedNameForSave(characterRecord* genChar, const char* str)
	{
		gameStringHelpers::createUniString(genChar->localizedNameForSave, str);
	}

	void namedCharSetLocalizedNextNameForSave(characterRecord* genChar, const char* str)
	{
		gameStringHelpers::createUniString(genChar->localizedNextNameForSave, str);
	}

	void namedCharSetLocalizedNicknameForSave(characterRecord* genChar, const char* str)
	{
		gameStringHelpers::createUniString(genChar->localizedNicknameForSave, str);
	}
	
	void setHeir(characterRecord* gen, bool isJustSet)
	{
		factionStruct* fac = gen->faction;
		if (isJustSet == true)
		{
			for (int i = 0; i < fac->numOfCharacters; i++)
				fac->characters[i]->characterRecord->status &= ~2;
			fac->heir = gen;
			gen->status |= 2;
		}
		else
		{
			_asm
			{
				push gen
				mov ecx, fac
				mov eax, [fac]
				mov eax, [eax]
				mov eax, [eax + 0x20]
				call eax
			}
		}
		return;
	}

#pragma endregion Character Record helpers

	/*----------------------------------------------------------------------------------------------------------------*\
										       Ancillaries
	\*----------------------------------------------------------------------------------------------------------------*/
#pragma region Ancillary helpers
	
	ancillary* findAncillary(const char* ancName)
	{
		if (ancName == nullptr)
			return nullptr;
		DWORD adr = codes::offsets.findAncillary;
		ancillary* anc = nullptr;
		_asm
		{
			push ancName
			mov eax, adr
			call eax
			add esp, 4
			mov anc, eax
		}
		return anc;
	}
	
	int addAncillaryName(characterRecord* character, const std::string& ancName)
	{
		ancillary* anc = findAncillary(ancName.c_str());
		if (anc == nullptr)return 0;
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
	
	void removeAncillary(characterRecord* character, ancillary* anc)
	{
		if (character == nullptr || anc == nullptr)return;
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
	
	void addTrait(characterRecord* character, const char* traitName, int traitLevel)
	{
		DWORD adrFunc = codes::offsets.getTrait;
		char** cryptS = gameStringHelpers::createHashedString(traitName);
		void* resTrait = nullptr;
		auto oneArg = cryptS[0];
		auto nextArg = cryptS[1];
		_asm
		{
			push nextArg
			push oneArg

			mov eax, adrFunc
			call eax

			mov resTrait, eax
			add esp, 0x8
		}
		if (resTrait == nullptr)
			return;
		
		//set trait
		adrFunc = codes::offsets.addTrait;
		_asm
		{
			push 1
			push traitLevel
			push resTrait
			mov ecx, character
			mov eax, adrFunc
			call eax
		}
	}

	void removeTrait(characterRecord* character, const char* traitName)
	{
		DWORD adrFunc = codes::offsets.getTrait;
		char** cryptS = gameStringHelpers::createHashedString(traitName);
		void* resTrait = nullptr;
		auto oneArg = cryptS[0];
		auto nextArg = cryptS[1];
		_asm
		{
			push nextArg
			push oneArg

			mov eax, adrFunc
			call eax

			mov resTrait, eax
			add esp, 0x8
		}
		if (resTrait == nullptr)
			return;
		
		//set trait
		adrFunc = codes::offsets.removeTrait;
		_asm
		{
			push resTrait
			mov ecx, character

			mov eax, adrFunc
			call eax
		}
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

		///character Record
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
		@tfield string portrait Wives (who have never been princesses) and children do not have anything for this field. Path to 'young' portrait used starting from 'mods' folder. Resets upon reloading a save.
		@tfield string portrait2 Wives (who have never been princesses) and children do not have anything for this field. Path to 'old' portrait used starting from 'mods' folder. Resets upon reloading a save.
		@tfield string portrait_custom Wives (who have never been princesses) and children do not have anything for this field. Folder name in ui/custom_portraits folder.
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
		@tfield int hitpoints
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
		@tfield int[10] combatVsReligion Maximum 10. EVEN IF YOU SET RELIGION LIMIT.
		@tfield int[31] combatVsFaction Maximum 31.

		@table characterRecord
		*/
		types.characterRecord = luaState.new_usertype<characterRecord>("characterRecord");
		types.characterRecord.set("index", &characterRecord::index);
		types.characterRecord.set("character", &characterRecord::gen);
		types.characterRecord.set("shortName", sol::property(&getStringPropertyGenChar<characterRecord_shortName>, &setStringPropertyGenChar<characterRecord_shortName>));
		types.characterRecord.set("lastName", sol::property(&getStringPropertyGenChar<characterRecord_lastName>, &setStringPropertyGenChar<characterRecord_lastName>));
		types.characterRecord.set("fullName", sol::property(&getStringPropertyGenChar<characterRecord_fullName>, &setStringPropertyGenChar<characterRecord_fullName>));
		types.characterRecord.set("localizedDisplayName", sol::property(&namedCharUniStringToStr<namedChar_localizedFullName>, &namedCharSetLocalizedFullName));
		types.characterRecord.set("label", sol::property(&getStringPropertyGenChar<characterRecord_label>, &setStringPropertyGenChar<characterRecord_label>));
		types.characterRecord.set("portrait", sol::property(&getStringPropertyGenChar<characterRecord_portrait>, &setStringPropertyGenChar<characterRecord_portrait>));
		types.characterRecord.set("portrait2", sol::property(&getStringPropertyGenChar<characterRecord_portrait2>, &setStringPropertyGenChar<characterRecord_portrait2>));
		types.characterRecord.set("portrait_custom", sol::property(&getStringPropertyGenChar<characterRecord_portrait_custom>, &setStringPropertyGenChar<characterRecord_portrait_custom>));
		types.characterRecord.set("modelName", sol::property(&getStringPropertyGenChar<characterRecord_modelName>, &setStringPropertyGenChar<characterRecord_modelName>));
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
		Add a trait to the character.
		@function characterRecord:addTrait
		@tparam string traitName Trait's internal name per export\_descr\_character\_traits.txt
		@tparam int traitLevel Trait's level.
		@usage
		ourNamedCharacter:addTrait("GoodCommander", 2)
		*/
		types.characterRecord.set_function("addTrait", &addTrait);
		/***
		Remove a trait from the character.
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
		//@section ancillaryTable

		/***
		Basic ancillary table
	
		@tfield int index
		@tfield int effectsNum
		@tfield bool isUnique
		@tfield bool transferable
		@tfield string name
		@tfield string type
		@tfield string imagePath
		@tfield getEffect getEffect
	
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
		/***
		Get an ancillary effect.
		@function ancillary:getEffect
		@tparam int index
		@treturn traitEffect effect
		@usage
		      ancillary:getEffect(2);
		*/
		types.ancillary.set_function("getEffect", &ancillary::getEffect);
		
		///capturedFactionInfo
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

		/// Trait Container
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
		@tfield traitLevel[10] levels Maximum: 10
		@tfield int levelCount
		@tfield traitEntry[20] antiTraits Maximum: 20
		@tfield int antiTraitCount
		@tfield int noGoingBackLevel
		@tfield int hidden
	
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
		Basic traitLevel table
	
		@tfield int level
		@tfield int threshold
		@tfield int effectsCount
		@tfield getTraitEffect getTraitEffect
	
		@table traitLevel
		*/
		types.traitLevel = luaState.new_usertype<traitLevel>("traitLevel");
		types.traitLevel.set("level", &traitLevel::level);
		types.traitLevel.set("threshold", &traitLevel::threshold);
		types.traitLevel.set("effectsCount", &traitLevel::effectsCount);
		
		/***
		Get trait effect.
		@function traitLevel:getTraitEffect
		@tparam int index
		@treturn traitEffect effect
		@usage
			local effect = level:getTraitEffect(0);
		*/
		types.traitLevel.set_function("getTraitEffect", &getTraitEffect);
		
		/// Trait Effect Struct
		//@section traitEffect

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
}
