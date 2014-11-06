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
			// if this Selection is part of a table, idx will not be 0, and will be the index in the stack of the table
			Selection(lua_State* s, const std::string& n, int idx);
			~Selection();
			
			// assignment operators
			void operator=(bool b);
			void operator=(int i);
			void operator=(unsigned int ui);
			void operator=(lua_Number n);
			void operator=(const std::string& s);
			
			template<typename Type>
			void operator=(const std::vector<Type>& vec);

			template<typename Key, typename Value>
			void operator=(const std::map<Key, Value>& map);
			
			// lua function call
			template<typename Ret, typename... Args>
			Ret operator()(Args... args);
			
			// casting selector to C++ types
			template<typename T>
			operator T() const;
			
			// operator chaining
			Selection operator[](const std::string& n) const;
			Selection operator[](const int i) const;

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
}

#endif // SELECTION_HPP
