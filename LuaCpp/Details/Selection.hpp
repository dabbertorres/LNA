#ifndef SELECTION_HPP
#define SELECTION_HPP

#include <lua.hpp>

#include <string>
#include <functional>
#include <vector>
#include <map>
#include <memory>

#include "Details.hpp"
#include "CppFunction.hpp"
#include "LuaFunction.hpp"

namespace lpp
{
	class Selection
	{
		public:
			Selection(lua_State* s, const std::string& n);
			// if this Selection is part of a table, idx will not be 0, and will be the index in the stack of the table this Selection is part of
			Selection(lua_State* s, const std::string& n, int idx);
			~Selection();
			
			// lua function call
			template<typename... Ret, typename... Args>
			std::tuple<Ret...> operator()(Args... args);
			
			// assignment operators
			template<typename T>
			void operator =(T& t);
			
			template<typename T>
			void operator =(const T& t);
			
			// assigning Lua table from C++ containers
			template<typename T>
			void operator =(const std::vector<T>& vec);

			template<typename K, typename V>
			void operator =(const std::map<K, V>& map);
			
			// assigning Lua variable to C++ function
			template<typename Ret, typename... Args>
			void operator =(const std::function<Ret(Args...)>& f);
			
			// casting to types
			template<typename T>
			operator T() const;
			
			template<typename T>
			operator std::vector<T>() const;
			
			template<typename K, typename V>
			operator std::map<K, V>() const;
			
			// operator chaining
			Selection operator [](const std::string& n) const;
			Selection operator [](const int i) const;

		private:
			lua_State* state;
			std::string name;
			int index;
	};
	
	// calling Lua function
	template<typename... Ret, typename... Args>
	std::tuple<Ret...> Selection::operator()(Args... args)
	{
		lua_getglobal(state, name.c_str());
		
		detail::distributeArgs(state, args...);
		
		constexpr int nargs = sizeof...(Args);
		constexpr int nret = sizeof...(Ret);
		
		lua_pcall(state, nargs, nret, 0);
		
		std::tuple<Ret...> final;
		
		return final;
	}
	
	// pushing primitives
	template<typename T>
	void Selection::operator =(T& t)
	{
		detail::pushValue(state, t);
		lua_setglobal(state, name.c_str());
	}
	
	template<typename T>
	void Selection::operator =(const T& t)
	{
		detail::pushValue(state, t);
		lua_setglobal(state, name.c_str());
	}
	
	// vector to table assignment
	// "i + 1" here is to convert to Lua's tables starting at 1, not 0
	template<typename T>
	void Selection::operator =(const std::vector<T>& vec)
	{
		lua_createtable(state, vec.size(), 0);
		
		for(unsigned int i = 0; i < vec.size(); i++)
		{
			detail::pushValue(state, vec[i]);
			lua_rawseti(state, -2, i + 1);
		}
		
		lua_setglobal(state, name.c_str());		
	}
	
	// map to table assignment
	template<typename K, typename V>
	void Selection::operator =(const std::map<K, V>& map)
	{
		lua_createtable(state, map.size(), 0);
		
		for(auto& v : map)
		{
			detail::pushValue(state, v.first);
			detail::pushValue(state, v.second);
			lua_rawset(state, -3);
		}
		
		lua_setglobal(state, name.c_str());		
	}
	
	// assignment of Lua variable to C++ function
	template<typename Ret, typename... Args>
	void Selection::operator =(const std::function<Ret(Args...)>& f)
	{
		BaseCppFunction::functions.emplace(name, std::unique_ptr<BaseCppFunction>(new CppFunction<Ret, Args...>(state, name, f)));
	}
	
	template<typename T>
	Selection::operator T() const
	{
		if(index == 0)
			lua_getglobal(state, name.c_str());
		
		T ret = detail::checkGet(detail::id<T>{}, state);
		
		lua_pop(state, 1);
		return ret;
	}
	
	template<typename T>
	Selection::operator std::vector<T>() const
	{
		if(index == 0)
			lua_getglobal(state, name.c_str());
			
		if(!lua_istable(state, -1))
			luaL_error(state, "Value on top of stack is not a table");
		
		std::vector<T> newVec;
		
		std::size_t tableLength = lua_rawlen(state, -1);
		
		for(unsigned int i = 1; i <= tableLength; i++)
		{
			lua_rawgeti(state, -1, i);
			
			T t = detail::checkGet(detail::id<T>{}, state);
			
			newVec.push_back(t);
			
			lua_pop(state, 1);
		}
		
		lua_settop(state, 0);
		return newVec;
	}
			
	template<typename K, typename V>
	Selection::operator std::map<K, V>() const
	{
		if(index == 0)
			lua_getglobal(state, name.c_str());
		
		if(!lua_istable(state, -1))
			luaL_error(state, "Value on top of stack is not a table");
		
		std::map<K, V> newMap;
		
		lua_pushnil(state);
		while(lua_next(state, -2))
		{
			K key = detail::checkGet(detail::id<K>{}, state, -2);
			
			V val = detail::checkGet(detail::id<V>{}, state);
			
			newMap.emplace(key, val);
			lua_pop(state, 1);
		}
		
		lua_settop(state, 0);
		return newMap;
	}
}

#endif // SELECTION_HPP
