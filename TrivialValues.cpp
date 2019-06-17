#include <iostream>
#include "TrivialValues.h"
#include "JsonPath.h"

Number::Number(double x)
	: num(x)
{
}

Value* Number::clone() const
{
	return new Number(*this);
}

void Number::print(std::ostream& out, const bool saveSpace, unsigned spaces) const
{
	out << num;
}

bool Number::findAndReplace(const JsonPath& path, const Value& val, unsigned i)
{
	return false;
}

const Value* Number::findByPath(const JsonPath& path, unsigned i) const
{
	return nullptr;
}

void Number::findAndGet(const String& key, JsonArray& arr) const
{
}

bool Number::findAndDelete(const JsonPath& path, unsigned i)
{
	return false;
}

bool Number::findAndAdd(const JsonPath& path, const Value& val, unsigned i)
{
	return false;
}

bool Number::operator==(const Value& rhs) const
{
	try
	{
		const Number& rhsNum = dynamic_cast<const Number&>(rhs);
		if (num == rhsNum.num)
			return true;
	}
	catch (const std::bad_cast&)
	{
	}
	return false;
}

double Number::get() const
{
	return num;
}

void Number::set(double x)
{
	num = x;
}

Bool::Bool(bool b)
	: val(b)
{
}

Value* Bool::clone() const
{
	return new Bool(*this);
}

void Bool::print(std::ostream& out, const bool saveSpace, unsigned spaces) const
{
	out << ((val) ? "true" : "false");
}

bool Bool::findAndReplace(const JsonPath& path, const Value& val, unsigned i)
{
	return false;
}

const Value* Bool::findByPath(const JsonPath& path, unsigned i) const
{
	return nullptr;
}

void Bool::findAndGet(const String& key, JsonArray& arr) const
{
}

bool Bool::findAndDelete(const JsonPath& path, unsigned i)
{
	return false;
}

bool Bool::findAndAdd(const JsonPath& path, const Value& val, unsigned i)
{
	return false;
}

bool Bool::operator==(const Value& rhs) const
{
	try
	{
		const Bool& rhsBool = dynamic_cast<const Bool&>(rhs);
		if (val == rhsBool.val)
			return true;
	}
	catch (const std::bad_cast&)
	{
	}
	return false;
}

bool Bool::get() const
{
	return val;
}

void Bool::set(bool b)
{
	val = b;
}

Value* Null::clone() const
{
	return new Null(*this);
}

void Null::print(std::ostream& out, const bool saveSpace, unsigned spaces) const
{
	out << "null";
}

bool Null::findAndReplace(const JsonPath& path, const Value& val, unsigned i)
{
	return false;
}

const Value* Null::findByPath(const JsonPath& path, unsigned i) const
{
	return nullptr;
}

void Null::findAndGet(const String& key, JsonArray& arr) const
{
}

bool Null::findAndDelete(const JsonPath& path, unsigned i)
{
	return false;
}

bool Null::findAndAdd(const JsonPath& path, const Value& val, unsigned i)
{
	return false;
}

bool Null::operator==(const Value& rhs) const
{
	try
	{
		const Null& rhsNull = dynamic_cast<const Null&>(rhs);
		return true;
	}
	catch (const std::bad_cast&)
	{
	}
	return false;
}