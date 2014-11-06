#include "State.hpp"

#include <iostream>

namespace lpp
{
	State::State()
	{
		state = luaL_newstate();
	}

	State::~State()
	{
		lua_close(state);
	}
	
	bool State::loadFile(const std::string& f)
	{
		if(luaL_loadfile(state, f.c_str()))
			return false;
		else
			return true;
	}
	
	bool State::run()
	{
		if(lua_pcall(state, 0, 0, 0))
			return false;
		else
			return true;
	}
	
	void State::openLib(const std::string& name, lua_CFunction open)
	{
		luaL_requiref(state, name.c_str(), open, 1);
		clean();
	}
	
	Selection State::operator[](const std::string& name) const
	{
		return Selection(state, name);
	}
	
	void State::stackDump()
	{
		int top = lua_gettop(state);
		
		for(int i = 1; i <= top; i++)
		{
			int type = lua_type(state, i);
			
			switch(type)
			{
				case LUA_TNIL:
					std::cout << i << ": nil\n";
					break;
				case LUA_TNUMBER:
					std::cout << i << ": number = " << lua_tonumber(state, i) << '\n';
					break;
				case LUA_TBOOLEAN:
					std::cout << i << ": bool = " << (lua_toboolean(state, i) ? "true" : "false") << '\n';
					break;
				case LUA_TSTRING:
					std::cout << i << ": string = " << lua_tostring(state, i) << '\n';
					break;
				case LUA_TTABLE:
					std::cout << i << ": table = {";
					
					lua_pushnil(state);
					while(lua_next(state, -2))
					{
						std::cout << lua_tostring(state, -2) << " = " << lua_tostring(state, -1) << ", ";
						lua_pop(state, 1);
					}
					
					std::cout << "}\n";
				default:
					std::cout << i << ": " << lua_typename(state, i) << '\n';
					break;
			}
		}
	}
	
	void State::clean()
	{
		lua_settop(state, 0);
	}
}
