#ifndef DETAILS_HPP
#define DETAILS_HPP

#include <vector>
#include <map>

namespace detail
{	
	// pushing primitives
	inline void pushValue(lua_State* state, bool b)
	{
		lua_pushboolean(state, b);
	}
	
	inline void pushValue(lua_State* state, int i)
	{
		lua_pushinteger(state, i);
	}
	
	inline void pushValue(lua_State* state, unsigned int ui)
	{
		lua_pushunsigned(state, ui);
	}
	
	inline void pushValue(lua_State* state, lua_Number n)
	{
		lua_pushnumber(state, n);
	}
	
	inline void pushValue(lua_State* state, const std::string& s)
	{
		lua_pushlstring(state, s.c_str(), s.size());
	}
	
	// pushing vectors
	// "i + 1" here is to convert to Lua's tables starting at 1, not 0
	template<typename T>
	inline void pushValue(lua_State* state, const std::vector<T>& vec)
	{
		lua_createtable(state, vec.size(), 0);
		
		for(unsigned int i = 0; i < vec.size(); i++)
		{
			pushValue(state, vec[i]);
			lua_rawseti(state, -2, i + 1);
		}
	}
	
	// pushing maps
	template<typename K, typename V>
	inline void pushValue(lua_State* state, const std::map<K, V>& map)
	{
		lua_createtable(state, map.size(), 0);
		
		for(auto& v : map)
		{
			pushValue(state, v.first);
			pushValue(state, v.second);
			lua_rawset(state, -3);
		}
	}
	
	// pushing objects
	template<typename T>
	inline void pushValue(lua_State* state, T* t)
	{
		lua_pushlightuserdata(state, t);
	}
	
	template<typename T>
	inline void pushValue(lua_State* state, T& t)
	{
		lua_pushlightuserdata(state, &t);
	}
	
	// recursive argument distribution
	inline void distributeArgs(lua_State* state) {};
			
	template<typename First, typename... Args>
	inline void distributeArgs(lua_State* state, First first, Args... args)
	{
		pushValue(state, first);
		distributeArgs(state, args...);
	}
	
	// getter functions
	template<typename T>
	struct id {};
	
	// primitives
	inline int checkGet(id<int>, lua_State* state, std::size_t idx = -1)
	{
		return luaL_checkint(state, idx);
	}

	inline unsigned int checkGet(id<unsigned int>, lua_State* state, std::size_t idx = -1)
	{
		return luaL_checkunsigned(state, idx);
	}

	inline lua_Number checkGet(id<lua_Number>, lua_State* state, std::size_t idx = -1)
	{
		return luaL_checknumber(state, idx);
	}

	inline std::string checkGet(id<std::string>, lua_State* state, std::size_t idx = -1)
	{
		std::size_t size;
		const char* buff = luaL_checklstring(state, idx, &size);
		return {buff, size};
	}
	
	// objects
	template<typename T>
	inline T checkGet(id<T>, lua_State* state, std::size_t idx = -1)
	{
		return static_cast<T>(lua_touserdata(state, idx));
	}

	template<typename T>
	inline T* checkGet(id<T*>, lua_State* state, std::size_t idx = -1)
	{
		return static_cast<T*>(lua_touserdata(state, idx));
	}

	template<typename T>
	inline T& checkGet(id<T&>, lua_State* state, std::size_t idx = -1)
	{
		return *static_cast<T*>(lua_touserdata(state, idx));
	}
	
	// getting arguments
	template<std::size_t... Is>
	struct indices {};

	template<std::size_t N, std::size_t... Is>
	struct indicesBuilder : indicesBuilder<N - 1, N - 1, Is...> {};

	template<std::size_t... Is>
	struct indicesBuilder<0, Is...>
	{
		using type = indices<Is...>;
	};

	template<typename... T, std::size_t... N>
	std::tuple<T...> getArgs(lua_State* state, indices<N...>)
	{
		return std::make_tuple(detail::checkGet(id<T>{}, state, N + 1)...);
	}

	template<typename... T>
	std::tuple<T...> getArgs(lua_State* state)
	{
		constexpr std::size_t nargs = sizeof...(T);

		return getArgs<T...>(state, typename indicesBuilder<nargs>::type());
	}

	// tuple to pack conversion
	template<typename Ret, typename... Args, std::size_t... N>
	inline Ret tupleToPack(const std::function<Ret(Args...)>& func, const std::tuple<Args...>& args, indices<N...>)
	{
		return func(std::get<N>(args)...);
	}

	template<typename Ret, typename... Args>
	inline Ret tupleToPack(const std::function<Ret(Args...)>& func, const std::tuple<Args...> args)
	{
		return tupleToPack(func, args, typename indicesBuilder<sizeof...(Args)>::type());
	}
}

#endif // DETAILS_HPP
