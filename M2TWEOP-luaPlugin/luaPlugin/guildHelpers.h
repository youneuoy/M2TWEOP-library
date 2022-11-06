#pragma once
#define guild_name 1
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
namespace guildHelpers
{
	template <char fieldIndex>
	std::string getStringProperty(const guild* guild)
	{
		char* retS = nullptr;
		if (fieldIndex == guild_name)
		{
			retS = guild->name;
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
	void setStringProperty(guild* guild, std::string newS)
	{
		char* arg = nullptr;
		if (fieldIndex == guild_name)
		{

			arg = reinterpret_cast<char*>(&guild->name);
		}
		luaGetSetFuncs::setGameString(arg, newS.c_str());
	}
}
