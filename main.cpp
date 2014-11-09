#include <iostream>

#include "LuaCpp/LuaCpp.hpp"

template<typename Type>
struct Vector2D
{
	Type x, y;
};

int add(int x, int y)
{
	return x + y;
}

int main(int argc, char **argv)
{
	lpp::State luaState;
	luaState.openLib("base", luaopen_base);
	luaState.openLib("math", luaopen_math);
	luaState.openLib("string", luaopen_string);
	luaState.openLib("table", luaopen_table);
	
	// load a file
	luaState.loadFile("main.lua");
	
	// assign a Lua variable to C++ function
	luaState["add"] = static_cast<std::function<int(int, int)>>(add);
	
	// assign values to Lua variables
	luaState["i"] = 9;
	luaState["j"] = std::vector<int>{5, 3, 2};
	luaState["m"] = std::map<std::string, std::string>{{"first", "hello"}, {"second", "world"}};
	
	// assign C++ object to Lua variable
	Vector2D<int> vec2d{10, 4};
	luaState["vec2d"] = vec2d;
	
	// run the file
	luaState.run();
	
	// C++ object set from Lua variable
	Vector2D<int>* anotherVec2d = luaState["vec2d"];
	
	std::cout << "vec2d.x: " << anotherVec2d->x << '\n';
	std::cout << "vec2d.y: " << anotherVec2d->y << '\n';
	
	// Get value of Lua variables
	std::string str0 = luaState["i"];
	std::string str1 = luaState["j"][1];
	std::string str2 = luaState["j"][2];
	std::string str3 = luaState["j"][3];
	
	std::cout << str0 << '\n';
	std::cout << str1 << '\n';
	std::cout << str2 << '\n';
	std::cout << str3 << '\n';
	
	std::vector<int> nums = luaState["nums"];
	
	std::map<std::string, int> strNums = luaState["map"];
	
	for(auto& n : nums)
		std::cout << "Num: " << n << '\n';
		
	for(auto& it : strNums)
		std::cout << it.first << ": " << it.second << '\n';
	
	// call lua function
	luaState["printSomething"](1, 7);
	luaState["sayHello"]();
	
	int one, two;
	//std::tie(one, two) = luaState["return2Nums"]();
	
	return 0;
}
