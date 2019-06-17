#include <iostream>
#include "String.h"


String::String(const char* str)
{
	copy(str);
}

String::String(const String& rhs)
{
	copy(rhs.str);
}

String& String::operator=(const String& rhs)
{
	if (this != &rhs)
	{
		char* t = str;
		copy(rhs.str);
		delete[] t;
	}
	return *this;
}

String::~String()
{
	delete[] str;
}

Value* String::clone() const
{
	return new String(*this);
}

void String::print(std::ostream& out, bool saveSpace, unsigned spaces) const
{
	out << "\"" << str << "\"";
}

bool String::findAndReplace(const JsonPath& path, const Value& val, unsigned i)
{
	return false;
}

const Value* String::findByPath(const JsonPath& path, unsigned i) const
{
	return nullptr;
}

void String::findAndGet(const String& key, JsonArray& arr) const
{
}

bool String::findAndDelete(const JsonPath& path, unsigned i)
{
	return false;
}

bool String::findAndAdd(const JsonPath& path, const Value& val, unsigned i)
{
	return false;
}

bool String::operator==(const Value& rhs) const
{
	try
	{
		const String& rhsStr = dynamic_cast<const String&>(rhs);
		if ((strcmp(str, rhsStr.get()) == 0))
			return true;
	}
	catch (const std::bad_cast&)
	{
	}
	return false;
}

const char* String::get() const
{
	return str;
}

void String::set(const char* str)
{
	delete[] this->str;
	copy(str);
}

void String::copy(const char* str)
{
	if (str)
	{
		unsigned len = strlen(str);
		this->str = new char[len + 1];
		strcpy_s(this->str, len + 1, str);
	}
	else
	{
		this->str = new char[1];
		this->str[0] = NULL;
	}
}
