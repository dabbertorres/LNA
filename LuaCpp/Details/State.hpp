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
			~State();
			
			bool loadFile(const std::string& f);
			
			bool run();

			void openLib(const std::string& name, lua_CFunction open);

			Selection operator[](const std::string& name);
			
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
