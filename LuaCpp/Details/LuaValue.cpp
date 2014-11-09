#include "LuaValue.hpp"

namespace lpp
{
	LuaValue::LuaValue()
		:	current(Value::NONE)
	{
	}

	LuaValue::~LuaValue()
	{
	}

	void LuaValue::operator =(std::nullptr_t)
	{
		current = Value::NONE;
	}

	void LuaValue::operator =(bool b)
	{
		value.b = b;
		current = Value::BOOL;
	}

	void LuaValue::operator =(int i)
	{
		value.i = i;
		current = Value::INT;
	}

	void LuaValue::operator =(unsigned int ui)
	{
		value.ui = ui;
		current = Value::UINT;
	}

	void LuaValue::operator =(lua_Number n)
	{
		value.n = n;
		current = Value::NUM;
	}

	void LuaValue::operator =(const std::string& s)
	{
		value.s = s;
		current = Value::STR;
	}

	LuaValue::operator bool() const
	{
		if(current == Value::BOOL)
			return value.b;
		else
			return false;
	}
	
	LuaValue::operator int() const
	{
		if(current == Value::INT)
			return value.i;
		else
			return 0;
	}
	
	LuaValue::operator unsigned int() const
	{
		if(current == Value::UINT)
			return value.ui;
		else
			return 0;
	}
	
	LuaValue::operator lua_Number() const
	{
		if(current == Value::NUM)
			return value.n;
		else
			return 0;
	}
	
	LuaValue::operator std::string() const
	{
		if(current == Value::STR)
			return value.s;
		else
			return "";
	}
}
