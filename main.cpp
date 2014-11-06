#include <iostream>

#include "LuaCpp/LuaCpp.hpp"

template<typename Type>
struct Vector2D
{
	Type x, y;
};

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
	
	Vector2D<int> vec2d{5, 3};
	luaState["vec2d"] = vec2d;
	
	luaState.run();
	
	Vector2D<int>* anotherVec2d = luaState["vec2d"];
	
	std::cout << "vec2d.x: " << anotherVec2d->x << '\n';
	std::cout << "vec2d.y: " << anotherVec2d->y << '\n';
	
	std::string str0 = luaState["i"];
	std::string str1 = luaState["j"][1];
	std::string str2 = luaState["j"][2];
	std::string str3 = luaState["j"][3];
	
	luaState["printSomething"](5, 3, 2);
	
	std::cout << str0 << '\n';
	std::cout << str1 << '\n';
	std::cout << str2 << '\n';
	std::cout << str3 << '\n';
	
	return 0;
}
