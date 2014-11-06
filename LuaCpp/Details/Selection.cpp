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
	
	// primitive assignment
	void Selection::operator=(bool b)
	{
		lua_pushboolean(state, b);
		lua_setglobal(state, name.c_str());
	}
	
	void Selection::operator=(int i)
	{
		lua_pushinteger(state, i);
		lua_setglobal(state, name.c_str());
	}
	
	void Selection::operator=(unsigned int ui)
	{
		lua_pushunsigned(state, ui);
		lua_setglobal(state, name.c_str());
	}
	
	void Selection::operator=(lua_Number n)
	{
		lua_pushnumber(state, n);
		lua_setglobal(state, name.c_str());
	}
	
	void Selection::operator=(const std::string& s)
	{
		lua_pushlstring(state, s.c_str(), s.size());
		lua_setglobal(state, name.c_str());
	}
	
	// vector to table assignment
	// "i + 1" in following functions is to convert to Lua's tables starting at 1, not 0
	template<>
	void Selection::operator=(const std::vector<bool>& vec)
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
	void Selection::operator=(const std::vector<int>& vec)
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
	void Selection::operator=(const std::vector<unsigned>& vec)
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
	void Selection::operator=(const std::vector<lua_Number>& vec)
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
	void Selection::operator=(const std::vector<std::string>& vec)
	{
		lua_createtable(state, vec.size(), 0);
		
		for(unsigned int i = 0; i < vec.size(); i++)
		{
			lua_pushstring(state, vec[i].c_str());
			lua_rawseti(state, -2, i + 1);
		}
		
		lua_setglobal(state, name.c_str());		
	}
	
	// vector to map assignment
	// guessing " * 2" because a map has twice as many elements as a vector
	template<>
	void Selection::operator=(const std::map<std::string, bool>& map)
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
	void Selection::operator=(const std::map<std::string, int>& map)
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
	void Selection::operator=(const std::map<std::string, unsigned int>& map)
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
	void Selection::operator=(const std::map<std::string, lua_Number>& map)
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
	void Selection::operator=(const std::map<std::string, std::string>& map)
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
	
	// casting
	template<>
	Selection::operator bool() const
	{
		lua_getglobal(state, name.c_str());
		bool ret = lua_toboolean(state, -1);
		lua_pop(state, 1);
		return ret;
	}
	
	template<>
	Selection::operator int() const
	{
		lua_getglobal(state, name.c_str());
		int ret = lua_tointeger(state, -1);
		lua_pop(state, 1);
		return ret;
	}
	
	template<>
	Selection::operator unsigned int() const
	{
		lua_getglobal(state, name.c_str());
		unsigned int ret = lua_tounsigned(state, -1);
		lua_pop(state, 1);
		return ret;
	}
	
	template<>
	Selection::operator lua_Number() const
	{
		lua_getglobal(state, name.c_str());
		lua_Number ret = lua_tonumber(state, -1);
		lua_pop(state, 1);
		return ret;
	}
	
	template<>
	Selection::operator std::string() const
	{
		lua_getglobal(state, name.c_str());
		std::size_t size;
		const char* ret = lua_tolstring(state, -1, &size);
		lua_pop(state, 1);
		return std::string{ret, size};
	}
	
	Selection Selection::operator[](const std::string& n) const
	{
		lua_getglobal(state, name.c_str());
		std::string newName = name + '.' + n;
		
		if(!lua_istable(state, -1))
			newName = name;
		
		return Selection(state, newName);
	}
	
	Selection Selection::operator[](const int i) const
	{
		lua_getglobal(state, name.c_str());
		std::string newName = name + '[' + std::to_string(i) + ']';
		
		if(!lua_istable(state, -1))
			newName = name;
		
		return Selection(state, newName);
	}
}
