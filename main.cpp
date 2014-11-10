#include <iostream>

#include "LuaCpp/LuaCpp.hpp"

template<typename T>
struct Vector2
{
	T x, y;
};

using Vector2i = Vector2<int>;

float dot(float oneX, float oneY, float twoX, float twoY)
{
	return oneX * twoX + oneY * twoY;
}

float dotVec(const Vector2i* one, const Vector2i* two)
{
	return one->x * two->x + one->y * two->y;
}

void say(std::string msg)
{
	std::cout << msg << '\n';
}

std::tuple<int, int> twoNums()
{
	return std::make_tuple(5, 3);
}

int main(int argc, char **argv)
{
	lpp::State luaState;
	luaState.openLib("base", luaopen_base);
	luaState.openLib("math", luaopen_math);
	luaState.openLib("string", luaopen_string);
	luaState.openLib("table", luaopen_table);
	
	// assign a Lua variable to C++ function
	luaState["add"] = static_cast<std::function<int(int, int)>>([](int x, int y) -> int
																{
																	return x + y;
																});
	luaState["dot"] = &dot;
	luaState["dotVec"] = &dotVec;
	luaState["say"] = &say;
	luaState["twoNums"] = &twoNums;
	
	// assign values to Lua variables
	luaState["i"] = 9;
	luaState["j"] = std::vector<int>{5, 3, 2};
	luaState["m"] = std::map<std::string, std::string>{{"first", "hello"}, {"second", "world"}};
	
	if(luaState("print(j[2])") != LUA_OK)
	{
		std::cout << luaState.getErrors() << '\n';
		return 1;
	}
	
	luaState["vec1"] = Vector2i{14, 19};
	luaState["vec2"] = Vector2i{12, 13};
	
	// load a file and catch syntax errors
	if(luaState.loadFile("main.lua") != LUA_OK)
	{
		std::cout << luaState.getErrors() << '\n';
		return 1;
	}
	
	// run the file
	if(luaState.run() != LUA_OK)
	{
		std::cout << luaState.getErrors() << '\n';
		return 1;
	}
	
	// Get value of Lua variables
	std::string str0 = luaState["i"];
	std::cout << str0 << '\n';
	
	std::string str1 = luaState["j"][1];
	std::cout << str1 << '\n';
	
	std::string str2 = luaState["j"][2][1];
	std::cout << str2 << '\n';
	
	std::string str3 = luaState["j"][2][2];
	std::cout << str3 << '\n';
	
	std::string str4 = luaState["j"][3];
	std::cout << str4 << '\n';
	
	std::vector<int> nums = luaState["nums"];
	
	std::map<std::string, int> strNums = luaState["map"];
	
	for(auto& n : nums)
		std::cout << "Num: " << n << '\n';
		
	for(auto& it : strNums)
		std::cout << it.first << ": " << it.second << '\n';
	
	// call lua function
	luaState["printSomething"](1, 7);
	
	std::string hello = luaState["sayHello"]();
	std::cout << hello << '\n';
	
	int one, two;
	std::tie(one, two) = luaState["return2Nums"]().getMultiReturn<int, int>();
	
	std::cout << "one: " << one << '\n';
	std::cout << "two: " << two << '\n';
	
	return 0;
}
