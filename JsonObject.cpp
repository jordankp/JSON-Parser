#include <iostream>

#include "JsonObject.h"
#include "JsonArray.h"
#include "Pair.h"
#include "JsonPath.h"

using std::endl;

void printSpaces(std::ostream& out, unsigned cnt)
{
	for (unsigned i = 0; i < cnt; ++i)
		out << ' ';
}

JsonObject::JsonObject()
	: capacity(2)
	, size(0)
{
	pairs = new Pair*[capacity];
}

JsonObject::JsonObject(const JsonObject& rhs)
{
	copy(rhs);
}

JsonObject& JsonObject::operator=(const JsonObject& rhs)
{
	if (this != &rhs)
	{
		clear();
		copy(rhs);
	}
	return *this;
}

JsonObject::~JsonObject()
{
	clear();
}

Value* JsonObject::clone() const
{
	return new JsonObject(*this);
}

void JsonObject::print(std::ostream& out, const bool saveSpace, unsigned spaces) const
{
	out << "{";
	if (!saveSpace)
		out << endl;

	for (unsigned i = 0; i < size; ++i)
	{
		if (!saveSpace)
			printSpaces(out, spaces + 1);

		pairs[i]->print(out, saveSpace, spaces + 1);
		if (i != size - 1)
			out << ",";
		if (!saveSpace)
			out << endl;
	}
	if (!saveSpace)
		printSpaces(out, spaces);

	out << "}";
}

bool JsonObject::findAndReplace(const JsonPath& path, const Value& val, unsigned i)
{
	if (i >= path.getSize())
		return false;
	for (unsigned j = 0; j < size; ++j)
	{
		if (path[i].str == pairs[j]->getKey())
		{
			if (i == path.getSize() - 1 && path[i].idx == -1)
			{
				pairs[j]->setVal(val);
				return true;
			}
			else
				return pairs[j]->val->findAndReplace(path, val, (path[i].idx == -1) ? i + 1 : i);
		}
	}
	return false;
}

const Value* JsonObject::findByPath(const JsonPath& path, unsigned i) const
{
	if (i >= path.getSize())
		return nullptr;
	for (unsigned j = 0; j < size; ++j)
	{
		if (path[i].str == pairs[j]->getKey())
		{
			if (i == path.getSize() - 1 && path[i].idx == -1)
				return pairs[j]->getVal();
			else
				return pairs[j]->getVal()->findByPath(path, (path[i].idx == -1) ? i + 1 : i);
		}
	}
	return nullptr;
}

void JsonObject::findAndGet(const String& key, JsonArray& arr) const
{
	for (unsigned i = 0; i < size; ++i)
	{
		if (pairs[i]->getKey() == key)
			arr.add(*(pairs[i]->getVal()));
		else
			pairs[i]->getVal()->findAndGet(key, arr);
	}
}

bool JsonObject::findAndDelete(const JsonPath& path, unsigned i)
{
	if (i >= path.getSize())
		return false;
	for (unsigned j = 0; j < size; ++j)
	{
		if (path[i].str == pairs[j]->getKey())
		{
			if (path[i].idx == -1 && i == path.getSize() - 1)
			{
				delete pairs[j];
				for (unsigned k = j; k < size - 1; ++k)
				{
					pairs[k] = pairs[k + 1];
				}
				--size;
				return true;
			}
			else
			{
				return pairs[j]->val->findAndDelete(path, (path[i].idx == -1) ? i + 1 : i);
			}
		}
	}
	return false;
}

bool JsonObject::findAndAdd(const JsonPath& path, const Value& val, unsigned i)
{
	if (i >= path.getSize())
		return false;

	if (i == path.getSize() - 1)
	{
		return findAndAddCase1(path, val, i);
	}
	else
	{
		return findAndAddCase2(path, val, i);
	}
}

bool JsonObject::operator==(const Value& rhs) const
{
	try
	{
		const JsonObject& rhsObj = dynamic_cast<const JsonObject&>(rhs);
		
	}
	catch (const std::bad_cast&)
	{
	}
	return false;
}

JsonObject& JsonObject::operator+=(const Pair& t)
{
	if (size == capacity)
	{
		resize(2 * capacity);
	}
	pairs[size++] = t.clone();
	return *this;
}

JsonObject JsonObject::operator+(const Pair& t) const
{
	JsonObject res(*this);
	res += t;
	return res;
}

unsigned JsonObject::getSize() const
{
	return size;
}

bool JsonObject::areEqual(const JsonObject& oth) const
{
	if (size == oth.size)
		for (unsigned i = 0; i < size; ++i)
			if (!(pairs[i]->getKey() == oth.pairs[i]->getKey() && *(pairs[i]->getVal()) == *(oth.pairs[i]->getVal())))
				return false;
	return true;
}

Pair* JsonObject::find(const String& key)
{
	for (unsigned i = 0; i < size; ++i)
		if (key == pairs[i]->getKey())
			return pairs[i];
	return nullptr;
}

bool JsonObject::findAndAddCase1(const JsonPath& path, const Value& val, unsigned i)
{
	Pair* pair = find(path[i].str);
	if (path[i].idx == -1)
	{
		if (pair)
			return false;
		*this += Pair(path[i].str, val);
		return true;
	}
	else
	{
		if (pair)
			return pair->val->findAndAdd(path, val, i);
		else
		{
			JsonArray jsonArr;
			if (!jsonArr.findAndAdd(path, val, i))
				return false;
			*this += Pair(path[i].str, jsonArr);
		}
		return true;
	}
}

bool JsonObject::findAndAddCase2(const JsonPath& path, const Value& val, unsigned i)
{
	Pair* pair = find(path[i].str);
	if (path[i].idx == -1)
	{
		if (pair)
			return pair->val->findAndAdd(path, val, i + 1);
		else
		{
			JsonObject newObj;
			if (!newObj.findAndAdd(path, val, i + 1))
				return false;
			*this += Pair(path[i].str, newObj);
			return true;
		}
	}
	else
	{
		if (pair)
			return pair->val->findAndAdd(path, val, i);
		else
		{
			JsonArray newArr;
			if (!newArr.findAndAdd(path, val, i))
				return false;
			*this += Pair(path[i].str, newArr);
			return true;
		}
	}
}

void JsonObject::copy(const JsonObject& rhs)
{
	pairs = new Pair*[rhs.capacity];
	for (size = 0; size < rhs.size; ++size)
	{
		pairs[size] = rhs.pairs[size]->clone();
	}
	capacity = rhs.capacity;
}

void JsonObject::clear()
{
	for (unsigned i = 0; i < size; ++i)
	{
		delete pairs[i];
	}
	delete[] pairs;
	pairs = nullptr;
	size = 0;
	capacity = 0;
}

void JsonObject::resize(unsigned newCap)
{
	Pair** tmp = new Pair*[newCap];
	for (unsigned i = 0; i < size; ++i)
	{
		tmp[i] = pairs[i];
	}
	delete[] pairs;
	pairs = tmp;
	capacity = newCap;
}

void JsonObject::remove(int idx)
{
	delete pairs[idx];
	for (unsigned i = idx; i < size - 1; ++i)
	{
		pairs[i] = pairs[i + 1];
	}
	--size;
}