#ifndef STATE_HPP
#define STATE_HPP

#include <lua.hpp>

#include <string>

#include "Selection.hpp"

namespace lpp
{
	class State
	{
		public:
			State();
			
			// non-copyable
			State(const State&) = delete;
			State& operator =(const State&) = delete;
			
			~State();
			
			// comparing a state to another doesn't make much sense
			bool operator ==(State&) = delete;
			
			auto loadFile(const std::string& f) -> decltype(LUA_OK);
			
			auto run() -> decltype(LUA_OK);

			void openLib(const std::string& name, lua_CFunction open);

			Selection operator[](const std::string& name);
			
			auto operator() (const std::string& name) -> decltype(LUA_OK);
			
			std::string getErrors() const;
			void stackDump() const;
			
			// act as a lua_State
			operator lua_State*() const;

		private:
			void clean();

			lua_State* state;
	};
}

#endif // STATE_HPP
