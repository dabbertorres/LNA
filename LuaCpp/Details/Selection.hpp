#ifndef SELECTION_HPP
#define SELECTION_HPP

#include "lua.hpp"

#include <string>
#include <functional>
#include <vector>
#include <map>

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
			template<typename First, typename... Args>
			Selection operator()(First first, Args... args);
			
			// assignment operators
			void operator =(bool b);
			void operator =(int i);
			void operator =(unsigned int ui);
			void operator =(lua_Number n);
			void operator =(const std::string& s);
			
			// assigning Lua table from C++ containers
			template<typename Type>
			void operator =(const std::vector<Type>& vec);

			template<typename Key, typename Value>
			void operator =(const std::map<Key, Value>& map);
			
			// assigning Lua variable to C++ object
			template<typename Type>
			void operator =(Type* t);
			
			template<typename Type>
			void operator =(Type& t);
			
			// casting to primitive types
			operator bool() const;
			operator int() const;
			operator unsigned int() const;
			operator lua_Number() const;
			operator std::string() const;
			
			// casting Lua table to C++ containers
			template<typename Type>
			operator std::vector<Type>() const;
			
			template<typename Key, typename Value>
			operator std::map<Key, Value>() const;
			
			// casting Lua type to C++ object
			template<typename Type>
			operator Type() const;
			
			template<typename Type>
			operator Type*() const;
			
			// operator chaining
			Selection operator [](const std::string& n) const;
			Selection operator [](const int i) const;

		private:
			// "generic" value pushing (all primitives)
			void pushValue(bool b);
			void pushValue(int i);
			void pushValue(unsigned int ui);
			void pushValue(lua_Number n);
			void pushValue(const std::string& s);
			
			// case where we've distributed and pushed all arguments
			void distributeArgs() {};
			
			// distribute arguments to function call
			template<typename First, typename... Args>
			void distributeArgs(First first, Args... args);
			
			lua_State* state;
			std::string name;
			int index;
	};
	
	// calling Lua function
	template<typename First, typename... Args>
	Selection Selection::operator()(First first, Args... args)
	{
		lua_getglobal(state, name.c_str());
		
		distributeArgs(first, args...);
		
		constexpr int nargs = sizeof...(Args) + 1;
		
		lua_pcall(state, nargs, 1, 0);
		
		return Selection(state, name, -1);
	}
	
	// assignment of C++ pointer object to Lua var
	template<typename Type>
	void Selection::operator =(Type* t)
	{
		lua_pushlightuserdata(state, t);
		lua_setglobal(state, name.c_str());
	}
	
	template<typename Type>
	void Selection::operator =(Type& t)
	{
		lua_pushlightuserdata(state, &t);
		lua_setglobal(state, name.c_str());
	}
	
	// casting to C++ object from Lua var
	template<typename Type>
	Selection::operator Type() const
	{
		if(index == 0)
			lua_getglobal(state, name.c_str());
		
		Type* ret = static_cast<Type*>(lua_touserdata(state, -1));
		
		lua_settop(state, 0);
		return *ret;
	}
	
	template<typename Type>
	Selection::operator Type*() const
	{
		if(index == 0)
			lua_getglobal(state, name.c_str());
		
		Type* ret = static_cast<Type*>(lua_touserdata(state, -1));
		
		lua_settop(state, 0);
		return ret;
	}
	
	// distribute arguments to function call
	// recursive, eventually calls pushValue for each type in ...args
	template<typename First, typename... Args>
	void Selection::distributeArgs(First first, Args... args)
	{
		pushValue(first);
		distributeArgs(args...);
	}
}

#endif // SELECTION_HPP
