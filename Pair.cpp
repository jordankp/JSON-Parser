#include <iostream>
#include "Pair.h"
#include "JsonPath.h"

Pair::Pair(const String& key, const Value& val)
	: val(nullptr)
	, key(key)
{
	this->val = val.clone();
}

Pair::Pair(const Pair& rhs)
{
	copy(rhs);
}

Pair& Pair::operator=(const Pair& rhs)
{
	if (this != &rhs)
	{
		copy(rhs);
	}
	return *this;
}

Pair::~Pair()
{
	delete val;
}

void Pair::print(std::ostream& out, bool saveSpace, unsigned spaces) const
{
	key.print(out, saveSpace, spaces);
	if (!saveSpace)
		out << " ";
	out << ":";
	if (!saveSpace)
		out << " ";
	spaces += strlen(key.get()) + 5;
	val->print(out, saveSpace, spaces);
}

Pair* Pair::clone() const
{
	return new Pair(*this);
}

const String& Pair::getKey() const
{
	return key;
}

const Value* Pair::getVal() const
{
	return val;
}

void Pair::setKey(const String& key)
{
	this->key = key;
}

void Pair::setVal(const Value& val)
{
	Value* tmp = val.clone();
	delete this->val;
	this->val = tmp;
}

void Pair::copy(const Pair& rhs)
{
	key = rhs.key;
	val = rhs.val->clone();
}