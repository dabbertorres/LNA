#ifndef LUAVALUE_HPP
#define LUAVALUE_HPP

#include <lua.hpp>

#include <string>

namespace lpp
{
	class LuaValue
	{
		public:
			LuaValue();
			~LuaValue();
			
			enum class Value
			{
				BOOL,
				INT,
				UINT,
				NUM,
				STR,
				NONE
			};
			
			void operator =(std::nullptr_t);
			void operator =(bool b);
			void operator =(int i);
			void operator =(unsigned int ui);
			void operator =(lua_Number n);
			void operator =(const std::string& s);
			
			operator bool() const;
			operator int() const;
			operator unsigned int() const;
			operator lua_Number() const;
			operator std::string() const;
			
		private:
			union ValueU
			{
				bool b;
				int i;
				unsigned int ui;
				lua_Number n;
				std::string s;
				ValueU() {}
				~ValueU() {}
			};
			
			ValueU value;
			Value current;
	};
}

#endif // LUAVALUE_HPP
