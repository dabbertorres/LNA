#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "lua.hpp"

#include <string>
#include <functional>

namespace lpp
{
	template<typename Ret, typename... Args>
	class Function
	{
		public:
			Function(const std::function<Ret(Args...)>& f)
			:	baseFunction([&](lua_State* state)
				{
					return sizeof...(Args);
				}),
				function(f)
			{}
			
			~Function();
			
			const std::function<int(lua_State*)> baseFunction;
			const std::function<Ret(Args...)> function;
			std::tuple<Args...> args;
	};
}

#endif // FUNCTION_HPP
