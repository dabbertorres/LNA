#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <lua.hpp>

#include <string>
#include <functional>
#include <unordered_map>
#include <memory>

#include "Details.hpp"
#include "LuaValue.hpp"

namespace lpp
{
	class BaseFunction
	{
		public:
			virtual int run(lua_State* state) = 0;
			
			static int luaDispatcher(lua_State* state)
			{
				BaseFunction* func = static_cast<BaseFunction*>(lua_touserdata(state, lua_upvalueindex(1)));
				return func->run(state);
			}
			
			static std::unordered_map<std::string, std::unique_ptr<BaseFunction>> functions;
	};
	
	template<typename Ret, typename... Args>
	class Function : public BaseFunction
	{
		public:
			Function(lua_State* s, const std::string& n, const std::function<Ret(Args...)>& f)
			:	state(s),
				name(n),
				function(f)
			{
				lua_pushlightuserdata(state, static_cast<BaseFunction*>(this));
				
				lua_pushcclosure(state, luaDispatcher, 1);
				
				lua_setglobal(state, name.c_str());
			}
			
			~Function() {}
			
			virtual int run(lua_State* state)
			{
				std::tuple<Args...> args = detail::getArgs<Args...>(state);
				auto value = detail::tupleToPack(function, args);
				detail::distributeArgs(state, value);
				return 1;
			}

		private:
			lua_State* state;
			std::string name;
			const std::function<Ret(Args...)> function;
	};
	
	template<typename... Args>
	class Function<void, Args...> : public BaseFunction
	{
		public:
			Function(lua_State* s, const std::string& n, const std::function<void(Args...)>& f)
			:	state(s),
				name(n),
				function(f)
			{
				lua_pushlightuserdata(state, static_cast<BaseFunction*>(this));
				
				lua_pushcclosure(state, luaDispatcher, 1);
				
				lua_setglobal(state, name.c_str());
			}
			
			~Function() {}
			
			virtual int run(lua_State* state)
			{
				std::tuple<Args...> args = detail::getArgs<Args...>(state);
				return 0;
			}

		private:
			lua_State* state;
			std::string name;
			const std::function<void(Args...)> function;
	};
}

#endif // FUNCTION_HPP
