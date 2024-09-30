#pragma once

#include "lua/sol.hpp"
namespace eopVideos
{
	void addToLua(sol::state& luaState);
	void* createEOPVideo(const char* path);
	void deleteEOPVideo(void* video);
	void playEOPVideo(void* video);
};


