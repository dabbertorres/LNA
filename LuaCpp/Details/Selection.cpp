#include "Selection.hpp"

namespace lpp
{
	Selection::Selection(lua_State* s, const std::string& n, const std::string& modName)
	:	state(s),
		name(n),
		moduleName(modName),
		index(0)
		
	{}
	
	Selection::Selection(lua_State* s, const std::string& n, const std::string& modName, int idx)
	:	state(s),
		name(n),
		moduleName(modName),
		index(idx)
	{}

	Selection::~Selection()
	{}
	
	/* Selection operators */
	Selection Selection::operator [](const std::string& n) const
	{
		lua_getglobal(state, name.c_str());
		std::string newName = name + '.' + n;
		
		if(!lua_istable(state, -1))
			luaL_error(state, "Value on top of stack is not a table");
		
		lua_pushlstring(state, n.c_str(), n.size());
		lua_rawget(state, -2);
		
		return Selection(state, newName, moduleName);
	}
	
	Selection Selection::operator [](const int i) const
	{
		lua_getglobal(state, name.c_str());
		std::string newName = name + '.' + std::to_string(i);
		
		if(!lua_istable(state, -1))
			luaL_error(state, "Value on top of stack is not a table");
		
		lua_rawgeti(state, -1, i);
		
		return Selection(state, newName, moduleName, -1);
	}
}
