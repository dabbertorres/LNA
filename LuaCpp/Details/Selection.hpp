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
			template<typename Ret, typename... Args>
			Ret operator()(Args... args);
			
			// assignment operators
			void operator =(bool b);
			void operator =(int i);
			void operator =(unsigned int ui);
			void operator =(lua_Number n);
			void operator =(const std::string& s);
			
			template<typename Type>
			void operator =(Type* t);
			
			template<typename Type>
			void operator =(Type& t);
			
			template<typename Type>
			void operator =(const std::vector<Type>& vec);

			template<typename Key, typename Value>
			void operator =(const std::map<Key, Value>& map);
			
			// casting selector to C++ types
			operator bool() const;
			operator int() const;
			operator unsigned int() const;
			operator lua_Number() const;
			operator std::string() const;
			
			template<typename Type>
			operator Type() const;
			
			template<typename Type>
			operator Type*() const;
			
			// operator chaining
			Selection operator [](const std::string& n) const;
			Selection operator [](const int i) const;

		private:
			lua_State* state;
			std::string name;
			int index;
	};
	
	template<typename Ret, typename... Args>
	Ret Selection::operator()(Args... args)
	{
		lua_getglobal(state, name.c_str());
		
		//lua_pushvalue(state, )
	}
	
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
}

#endif // SELECTION_HPP
