LNA = Lua is Not an Acronym
=====
(Amazing how many people use Lua as LUA)

Yet another C++/Lua binding

Syntax is heavily inspired by <a href="https://github.com/jeremyong/Selene">Selene</a>.

Main difference is that I'm starting off trying to cross the bridge between 
std containers and Lua tables. We'll see how that goes. 

Requirements
============
Lua 5.1 or higher

C++ 11 & 14 features are used, including constexpr, so you'll need a pretty decent standards compliant compiler. (Off the top of my head, this makes VS 2013 not usable, but g++ and clang should do just fine.)

All you need then is a working Lua install, and to link to Lua in your final application, and it should be good to go.
Otherwise, it's header only (due to being template heavy).

Also: RTTI is NOT required. :)

Am I working?
=============
[![Build Status](https://travis-ci.org/dabbertorres/LNA.svg)](https://travis-ci.org/dabbertorres/LNA)

What's Working
==============
- Getting variables/values to/from Lua/C++
(including std::vector and std::map at the moment)

- C++ classes being registed to Lua
- Classes are "remembered" as being registered. Rather than a map of type_index to metatables or something, this was accomplished via the Class (the actual class "Class" in the lib) having two static variables: a bool, defining whether an instance of Class<YourClass> has been created or not, and a string, with the class name you provided, which is also the name of the metatable. These variables are initialized to 'false' and an empty string, respectively, and are only set to true and filled when an instance of Class<YourClass> has been created.

- Objects will only be returned by pointer or value, not by reference. Lua is written in C, and thus does not know about references.
- Writing objects is current done via const reference/value (copying), which creates an actual Lua userdata, or by pointer, which simply pushes a lightuserdata. This will be be changed so object pointers can be pushed and have actual data.

- If you try to read a type that Lua does not know about, Lua will throw an error complaining: "Attempted read of unknown type."
- If you try to write a type that Lua does not know about to a Lua variable, it won't get pushed, and Lua will throw an error: "Attempted push of unknown type."

- Calling Lua functions from C++

- Calling C++ functions from Lua (including member functions)

Examples
========
See <a href="https://github.com/dabbertorres/luapp/blob/master/main.cpp">main.cpp</a> and <a href="https://github.com/dabbertorres/luapp/blob/master/main.lua">main.lua</a> for examples.
