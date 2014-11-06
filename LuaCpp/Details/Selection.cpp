#include "Selection.hpp"
#include <iostream>
namespace lpp
{
	Selection::Selection(lua_State* s, const std::string& n)
	:	state(s),
		name(n),
		index(0)
	{}
	
	Selection::Selection(lua_State* s, const std::string& n, int idx)
	:	state(s),
		name(n),
		index(idx)
	{}

	Selection::~Selection()
	{}
	
	/* assignment functions */
	// primitive assignment
	void Selection::operator =(bool b)
	{
		lua_pushboolean(state, b);
		lua_setglobal(state, name.c_str());
	}
	
	void Selection::operator =(int i)
	{
		lua_pushinteger(state, i);
		lua_setglobal(state, name.c_str());
	}
	
	void Selection::operator =(unsigned int ui)
	{
		lua_pushunsigned(state, ui);
		lua_setglobal(state, name.c_str());
	}
	
	void Selection::operator =(lua_Number n)
	{
		lua_pushnumber(state, n);
		lua_setglobal(state, name.c_str());
	}
	
	void Selection::operator =(const std::string& s)
	{
		lua_pushlstring(state, s.c_str(), s.size());
		lua_setglobal(state, name.c_str());
	}
	
	// vector to table assignment
	// "i + 1" in following functions is to convert to Lua's tables starting at 1, not 0
	template<>
	void Selection::operator =(const std::vector<bool>& vec)
	{
		lua_createtable(state, vec.size(), 0);
		
		for(unsigned int i = 0; i < vec.size(); i++)
		{
			lua_pushboolean(state, vec[i]);
			lua_rawseti(state, -2, i + 1);
		}
		
		lua_setglobal(state, name.c_str());		
	}
	
	template<>
	void Selection::operator =(const std::vector<int>& vec)
	{
		lua_createtable(state, vec.size(), 0);
		
		for(unsigned int i = 0; i < vec.size(); i++)
		{
			lua_pushinteger(state, vec[i]);
			lua_rawseti(state, -2, i + 1);
		}
		
		lua_setglobal(state, name.c_str());		
	}
	
	template<>
	void Selection::operator =(const std::vector<unsigned>& vec)
	{
		lua_createtable(state, vec.size(), 0);
		
		for(unsigned int i = 0; i < vec.size(); i++)
		{
			lua_pushunsigned(state, vec[i]);
			lua_rawseti(state, -2, i + 1);
		}
		
		lua_setglobal(state, name.c_str());		
	}
	
	template<>
	void Selection::operator =(const std::vector<lua_Number>& vec)
	{
		lua_createtable(state, vec.size(), 0);
		
		for(unsigned int i = 0; i < vec.size(); i++)
		{
			lua_pushnumber(state, vec[i]);
			lua_rawseti(state, -2, i + 1);
		}
		
		lua_setglobal(state, name.c_str());		
	}
	
	template<>
	void Selection::operator =(const std::vector<std::string>& vec)
	{
		lua_createtable(state, vec.size(), 0);
		
		for(unsigned int i = 0; i < vec.size(); i++)
		{
			lua_pushstring(state, vec[i].c_str());
			lua_rawseti(state, -2, i + 1);
		}
		
		lua_setglobal(state, name.c_str());		
	}
	
	// map to table assignment
	// guessing " * 2" because a map has twice as many elements as a vector (key, value)
	template<>
	void Selection::operator =(const std::map<std::string, bool>& map)
	{
		lua_createtable(state, map.size() * 2, 0);
		
		for(auto& v : map)
		{
			lua_pushstring(state, v.first.c_str());
			lua_pushboolean(state, v.second);
			lua_rawset(state, -3);
		}
		
		lua_setglobal(state, name.c_str());		
	}
	
	template<>
	void Selection::operator =(const std::map<std::string, int>& map)
	{
		lua_createtable(state, map.size() * 2, 0);
		
		for(auto& v : map)
		{
			lua_pushstring(state, v.first.c_str());
			lua_pushinteger(state, v.second);
			lua_rawset(state, -3);
		}
		
		lua_setglobal(state, name.c_str());		
	}
	
	template<>
	void Selection::operator =(const std::map<std::string, unsigned int>& map)
	{
		lua_createtable(state, map.size() * 2, 0);
		
		for(auto& v : map)
		{
			lua_pushstring(state, v.first.c_str());
			lua_pushunsigned(state, v.second);
			lua_rawset(state, -3);
		}
		
		lua_setglobal(state, name.c_str());		
	}
	
	template<>
	void Selection::operator =(const std::map<std::string, lua_Number>& map)
	{
		lua_createtable(state, map.size() * 2, 0);
		
		for(auto& v : map)
		{
			lua_pushstring(state, v.first.c_str());
			lua_pushnumber(state, v.second);
			lua_rawset(state, -3);
		}
		
		lua_setglobal(state, name.c_str());		
	}
	
	template<>
	void Selection::operator =(const std::map<std::string, std::string>& map)
	{
		lua_createtable(state, map.size() * 2, 0);
		
		for(auto& v : map)
		{
			lua_pushstring(state, v.first.c_str());
			lua_pushstring(state, v.second.c_str());
			lua_rawset(state, -3);
		}
		
		lua_setglobal(state, name.c_str());		
	}
	
	/* casting functions */
	// casting primitives
	Selection::operator bool() const
	{
		if(index == 0)
			lua_getglobal(state, name.c_str());
			
		bool ret = lua_toboolean(state, -1);
		
		lua_settop(state, 0);
		return ret;
	}
	
	Selection::operator int() const
	{
		if(index == 0)
			lua_getglobal(state, name.c_str());
			
		int ret = lua_tointeger(state, -1);
		
		lua_settop(state, 0);
		return ret;
	}
	
	Selection::operator unsigned int() const
	{
		if(index == 0)
			lua_getglobal(state, name.c_str());
			
		unsigned int ret = lua_tounsigned(state, -1);
		
		lua_settop(state, 0);
		return ret;
	}
	
	Selection::operator lua_Number() const
	{
		if(index == 0)
			lua_getglobal(state, name.c_str());
			
		lua_Number ret = lua_tonumber(state, -1);
		
		lua_settop(state, 0);
		return ret;
	}
	
	Selection::operator std::string() const
	{
		if(index == 0)
			lua_getglobal(state, name.c_str());
			
		std::size_t size;
		const char* ret = lua_tolstring(state, -1, &size);
		
		lua_settop(state, 0);
		return std::string{ret, size};
	}
	
	// casting vectors
	template<>
	Selection::operator std::vector<bool>() const
	{
		if(index == 0)
			lua_getglobal(state, name.c_str());
			
		if(!lua_istable(state, -1))
			luaL_error(state, "Value on top of stack is not a table");
		
		std::vector<bool> newVec;
		
		std::size_t tableLength = lua_rawlen(state, -1);
		
		for(unsigned int i = 1; i <= tableLength; i++)
		{
			lua_rawgeti(state, -1, i);
			newVec.push_back(lua_toboolean(state, -1));
			lua_pop(state, 1);
		}
		
		lua_settop(state, 0);
		return newVec;
	}
	
	template<>
	Selection::operator std::vector<int>() const
	{
		if(index == 0)
			lua_getglobal(state, name.c_str());
			
		if(!lua_istable(state, -1))
			luaL_error(state, "Value on top of stack is not a table");
		
		std::vector<int> newVec;
		
		std::size_t tableLength = lua_rawlen(state, -1);
		
		for(unsigned int i = 1; i <= tableLength; i++)
		{
			lua_rawgeti(state, -1, i);
			newVec.push_back(lua_tointeger(state, -1));
			lua_pop(state, 1);
		}
		
		lua_settop(state, 0);
		return newVec;
	}
	
	template<>
	Selection::operator std::vector<unsigned int>() const
	{
		if(index == 0)
			lua_getglobal(state, name.c_str());
			
		if(!lua_istable(state, -1))
			luaL_error(state, "Value on top of stack is not a table");
		
		std::vector<unsigned int> newVec;
		
		std::size_t tableLength = lua_rawlen(state, -1);
		
		for(unsigned int i = 1; i <= tableLength; i++)
		{
			lua_rawgeti(state, -1, i);
			newVec.push_back(lua_tounsigned(state, -1));
			lua_pop(state, 1);
		}
		
		lua_settop(state, 0);
		return newVec;
	}
	
	template<>
	Selection::operator std::vector<lua_Number>() const
	{
		if(index == 0)
			lua_getglobal(state, name.c_str());
			
		if(!lua_istable(state, -1))
			luaL_error(state, "Value on top of stack is not a table");
		
		std::vector<lua_Number> newVec;
		
		std::size_t tableLength = lua_rawlen(state, -1);
		
		for(unsigned int i = 1; i <= tableLength; i++)
		{
			lua_rawgeti(state, -1, i);
			newVec.push_back(lua_tonumber(state, -1));
			lua_pop(state, 1);
		}
		
		lua_settop(state, 0);
		return newVec;
	}
	
	template<>
	Selection::operator std::vector<std::string>() const
	{
		if(index == 0)
			lua_getglobal(state, name.c_str());
			
		if(!lua_istable(state, -1))
			luaL_error(state, "Value on top of stack is not a table");
		
		std::vector<std::string> newVec;
		
		std::size_t tableLength = lua_rawlen(state, -1);
		
		for(unsigned int i = 1; i <= tableLength; i++)
		{
			lua_rawgeti(state, -1, i);
			std::size_t strSize;
			const char* str = lua_tolstring(state, -1, &strSize);
			newVec.push_back({str, strSize});
			lua_pop(state, 1);
		}
		
		lua_settop(state, 0);
		return newVec;
	}
	
	// casting maps
	template<>
	Selection::operator std::map<std::string, bool>() const
	{
		if(index == 0)
			lua_getglobal(state, name.c_str());
		
		if(!lua_istable(state, -1))
			luaL_error(state, "Value on top of stack is not a table");
		
		std::map<std::string, bool> newMap;
		
		lua_pushnil(state);
		while(lua_next(state, -2))
		{
			std::size_t strSize;
			const char* str = lua_tolstring(state, -2, &strSize);
			bool val = lua_toboolean(state, -1);
			
			newMap.emplace(std::string{str, strSize}, val);
			lua_pop(state, 1);
		}
		
		lua_settop(state, 0);
		return newMap;
	}
	
	template<>
	Selection::operator std::map<std::string, int>() const
	{
		if(index == 0)
			lua_getglobal(state, name.c_str());
		
		if(!lua_istable(state, -1))
			luaL_error(state, "Value on top of stack is not a table");
		
		std::map<std::string, int> newMap;
		
		lua_pushnil(state);
		while(lua_next(state, -2))
		{
			std::size_t strSize;
			const char* str = lua_tolstring(state, -2, &strSize);
			int val = lua_tointeger(state, -1);
			
			newMap.emplace(std::string{str, strSize}, val);
			lua_pop(state, 1);
		}
		
		lua_settop(state, 0);
		return newMap;
	}
	
	template<>
	Selection::operator std::map<std::string, unsigned int>() const
	{
		if(index == 0)
			lua_getglobal(state, name.c_str());
		
		if(!lua_istable(state, -1))
			luaL_error(state, "Value on top of stack is not a table");
		
		std::map<std::string, unsigned int> newMap;
		
		lua_pushnil(state);
		while(lua_next(state, -2))
		{
			std::size_t strSize;
			const char* str = lua_tolstring(state, -2, &strSize);
			unsigned int val = lua_tounsigned(state, -1);
			
			newMap.emplace(std::string{str, strSize}, val);
			lua_pop(state, 1);
		}
		
		lua_settop(state, 0);
		return newMap;
	}
	
	template<>
	Selection::operator std::map<std::string, lua_Number>() const
	{
		if(index == 0)
			lua_getglobal(state, name.c_str());
		
		if(!lua_istable(state, -1))
			luaL_error(state, "Value on top of stack is not a table");
		
		std::map<std::string, lua_Number> newMap;
		
		lua_pushnil(state);
		while(lua_next(state, -2))
		{
			std::size_t strSize;
			const char* str = lua_tolstring(state, -2, &strSize);
			lua_Number val = lua_tonumber(state, -1);
			
			newMap.emplace(std::string{str, strSize}, val);
			lua_pop(state, 1);
		}
		
		lua_settop(state, 0);
		return newMap;
	}
	
	template<>
	Selection::operator std::map<std::string, std::string>() const
	{
		if(index == 0)
			lua_getglobal(state, name.c_str());
		
		if(!lua_istable(state, -1))
			luaL_error(state, "Value on top of stack is not a table");
		
		std::map<std::string, std::string> newMap;
		
		lua_pushnil(state);
		while(lua_next(state, -2))
		{
			std::size_t strSize;
			const char* str = lua_tolstring(state, -2, &strSize);
			std::string key = {str, strSize};
			
			str = lua_tolstring(state, -1, &strSize);
			std::string val = {str, strSize};
			
			newMap.emplace(key, val);
			lua_pop(state, 1);
		}
		
		lua_settop(state, 0);
		return newMap;
	}
	
	/* Selection operators */
	Selection Selection::operator [](const std::string& n) const
	{
		lua_getglobal(state, name.c_str());
		std::string newName = name + '.' + n;
		
		if(!lua_istable(state, -1))
			newName = "";
		
		lua_pushlstring(state, n.c_str(), n.size());
		lua_rawget(state, -2);
		
		return Selection(state, newName);
	}
	
	Selection Selection::operator [](const int i) const
	{
		lua_getglobal(state, name.c_str());
		std::string newName = name + '.' + std::to_string(i);
		
		if(!lua_istable(state, -1))
			newName = "";
		
		lua_rawgeti(state, -1, i);
		
		return Selection(state, newName, -1);
	}
	
	/* pushing primitives */
	void Selection::pushValue(bool b)
	{
		lua_pushboolean(state, b);
	}
	
	void Selection::pushValue(int i)
	{
		lua_pushinteger(state, i);
	}
	
	void Selection::pushValue(unsigned int ui)
	{
		lua_pushunsigned(state, ui);
	}
	
	void Selection::pushValue(lua_Number n)
	{
		lua_pushnumber(state, n);
	}
	
	void Selection::pushValue(const std::string& s)
	{
		lua_pushlstring(state, s.c_str(), s.size());
	}
}
