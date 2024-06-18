///
//![Lua logo](../Lua.png)
//M2TWEOP structures and functions. There are not many examples and descriptions here. Also note that the examples do not include many of the checks that would be required when creating modifications.
//@module LuaPlugin
//@author youneuoy
//@license GPL-3.0
#include "luaP.h"
#include "eopBuildings.h"
#include "eopdu.h"

void luaP::initEopEdu()
{

	eopDuHelpers::addToLua(luaState);
}
