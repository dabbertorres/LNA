luapp
=====

another C++/Lua binding

Heavily inspired by <a href="github.com/jeremyong/selene">Selene</a>.

Main difference is that I'm starting off trying to cross the bridge between 
std containers and Lua tables. We'll see how that goes. 

What's Working
==============
- Getting variables/values to/from Lua/C++
(including std::vector and std::map atm)

- Simple lightuserdata implementation
    - Passing objects around works. One condition: C++ functions that work on said objects, cannot accept references.
    - Only pass by value or pointer.

- Calling Lua functions

- Calling C++ functions

Examples
========
See <a href="https://github.com/dabbertorres/luapp/blob/master/main.cpp">main.cpp</a> and <a href="https://github.com/dabbertorres/luapp/blob/master/Debug/main.lua">Debug/main.lua</a> for examples.
