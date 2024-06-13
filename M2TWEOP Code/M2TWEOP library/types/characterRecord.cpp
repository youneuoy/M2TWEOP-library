#include "characterRecord.h"

#include "fastFuncts.h"
#include "fastFunctsHelpers.h"
#include "functionsOffsets.h"
#include "smallFuncs.h"
#include "technicalHelpers.h"

#define namedChar_localizedFullName 1
#define namedChar_localizedNameForSave 2
#define namedChar_localizedNextNameForSave 3
#define namedChar_localizedNicknameForSave 4
#define characterRecord_shortName 1
#define characterRecord_fullName 2
#define characterRecord_label 3
#define characterRecord_modelName 4
#define characterRecord_portrait 5
#define characterRecord_portrait2 6
#define characterRecord_portrait_custom 7
#define characterRecord_lastName 8

namespace characterRecordHelpers
{
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
			fastFunctsHelpers::setCryptedString(&genChar->shortName, newS.c_str());
		else if (fieldIndex == characterRecord_fullName)
			fastFunctsHelpers::setCryptedString(&genChar->fullName, newS.c_str());
		else if (fieldIndex == characterRecord_label)
			fastFunctsHelpers::setCryptedString(&genChar->label, newS.c_str());
		else if (fieldIndex == characterRecord_modelName)
			fastFunctsHelpers::setCryptedString(&genChar->modelName, newS.c_str());
		else if (fieldIndex == characterRecord_portrait)
			fastFunctsHelpers::setCryptedString(&genChar->portrait, newS.c_str());
		else if (fieldIndex == characterRecord_portrait2)
			fastFunctsHelpers::setCryptedString(&genChar->portrait2, newS.c_str());
		else if (fieldIndex == characterRecord_portrait_custom)
			fastFunctsHelpers::setCryptedString(&genChar->portrait_custom, newS.c_str());
		else if (fieldIndex == characterRecord_lastName)
			fastFunctsHelpers::setCryptedString(&genChar->lastName, newS.c_str());
	}
	
	template <char fieldIndex>
	std::string namedCharUniStringToStr(characterRecord* genChar)
	{
		if (fieldIndex == namedChar_localizedFullName)
			return technicalHelpers::uniStringToStr(genChar->localizedFullName);
		if (fieldIndex == namedChar_localizedNameForSave)
			return technicalHelpers::uniStringToStr(genChar->localizedNameForSave);
		if (fieldIndex == namedChar_localizedNextNameForSave)
			return technicalHelpers::uniStringToStr(genChar->localizedNextNameForSave);
		if (fieldIndex == namedChar_localizedNicknameForSave)
			return technicalHelpers::uniStringToStr(genChar->localizedNicknameForSave);
		return "";
	}
	
	void namedCharSetLocalizedFullName(characterRecord* genChar, const char* str)
	{
		smallFuncs::createUniString(genChar->localizedFullName, str);
	}

	void namedCharSetLocalizedNameForSave(characterRecord* genChar, const char* str)
	{
		smallFuncs::createUniString(genChar->localizedNameForSave, str);
	}

	void namedCharSetLocalizedNextNameForSave(characterRecord* genChar, const char* str)
	{
		smallFuncs::createUniString(genChar->localizedNextNameForSave, str);
	}

	void namedCharSetLocalizedNicknameForSave(characterRecord* genChar, const char* str)
	{
		smallFuncs::createUniString(genChar->localizedNicknameForSave, str);
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
	int addAncillaryName(characterRecord* character, const std::string& ancName)
	{
		ancillary* anc = fastFuncts::findAncillary(ancName.c_str());
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

	
	void addTrait(characterRecord* character, const char* traitName, int traitLevel)
	{
		DWORD adrFunc = codes::offsets.getTrait;
		char** cryptS = fastFunctsHelpers::makeCryptedString(traitName);
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
		char** cryptS = fastFunctsHelpers::makeCryptedString(traitName);
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

	
	void addToLua(sol::state& luaState)
	{
		struct
		{
			sol::usertype<characterRecord>characterRecord;
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
	@tfield float yearOfMaturity
	@tfield int seasonOfMaturity
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
	@tfield int gunpowerCommand
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
	types.characterRecord.set("isFamily", sol::property(&characterRecord::getIsFamily, &characterRecord::setIsFamily));
	types.characterRecord.set("age", sol::property(&characterRecord::getAge, &characterRecord::setAge));
	types.characterRecord.set("yearOfBirth", &characterRecord::yearOfBirth);
	types.characterRecord.set("seasonOfBirth", &characterRecord::seasonOfBirth);
	types.characterRecord.set("seasonOfDeath", &characterRecord::deathSeason);
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
		
	}
}
