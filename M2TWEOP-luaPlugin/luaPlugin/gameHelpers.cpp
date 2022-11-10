#include "gameHelpers.h"
#include "plugData.h"

UINT32 gameHelpers::getFactionsCount()
{
	return (*(*plugData::data.funcs.getFactionsCount))();
}

std::string gameHelpers::callConsole(std::string cmdName, sol::variadic_args va)
{
	char buffer[100]{};
	buffer[0] = '\0';
	bool isOk = false;
	if (va.size() == 0)
	{
		isOk=(*(*plugData::data.funcs.callGameConsoleCommand))(cmdName.c_str(),"", buffer);
	}
	else
	{
		isOk = (*(*plugData::data.funcs.callGameConsoleCommand))(cmdName.c_str(), va.begin()->as<std::string>().c_str(), buffer);
	}

    if (isOk == false && buffer[0] == '\0')
	{
		sprintf_s(buffer, "error");
	}
	return buffer;;
}

factionStruct* gameHelpers::getFaction(int index)
{
	return (*(*plugData::data.funcs.getFactionsList))()[index];
}

guild* gameHelpers::getGuild(unsigned char index)
{
	return (*(*plugData::data.funcs.getGuild))(index);
}

general* gameHelpers::createCharacter(const char* type, factionStruct* fac, int age, const char* name, const char* name2, int subFaction, const char* portrait, int x, int y)
{
	if (portrait!=nullptr&&strlen(portrait) == 0)
	{
		portrait = nullptr;
	}
	if (name !=nullptr&&strlen(name) == 0)
	{
		name = nullptr;
	}
	if (name2 !=nullptr&&strlen(name2) == 0)
	{
		name2 = nullptr;
	}
	return (*(*plugData::data.funcs.createCharacter))(type, fac, age, name, name2, subFaction, portrait, x, y);
}

stackStruct* gameHelpers::createArmy(general* character)
{
	return (*(*plugData::data.funcs.createArmy))(character);
}

stackStruct* gameHelpers::createArmyInSettlement(settlementStruct* sett)
{
	return (*(*plugData::data.funcs.createArmyInSettlement))(sett);
}


std::tuple<bool, int> gameHelpers::getScriptCounter(const char* type)
{
	bool isExist = false;
	int counterValue=(*(*plugData::data.funcs.getScriptCounter))(type,isExist);

	return std::make_tuple(isExist, counterValue);
}

void gameHelpers::setScriptCounter(const char* type, int value)
{
	(*(*plugData::data.funcs.setScriptCounter))(type, value);
}
