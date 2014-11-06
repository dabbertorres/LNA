#include <iostream>

#include "LuaCpp/LuaCpp.hpp"

int main(int argc, char **argv)
{
	lpp::State luaState;
	luaState.openLib("base", luaopen_base);
	luaState.openLib("math", luaopen_math);
	luaState.openLib("string", luaopen_string);
	luaState.openLib("table", luaopen_table);
	
	luaState.loadFile("main.lua");
	
	luaState["i"] = 9;
	luaState["j"] = std::vector<int>{5, 3, 2};
	luaState["m"] = std::map<std::string, std::string>{{"first", "hello"}, {"second", "world"}};
	
	luaState.run();
	
	std::string str0 = luaState["i"];
	std::string str1 = luaState["j"][1];
	std::string str2 = luaState["j"][2];
	std::string str3 = luaState["j"][3];
	
	std::cout << str0 << '\n';
	std::cout << str1 << '\n';
	std::cout << str2 << '\n';
	std::cout << str3 << '\n';
	
	return 0;
}
