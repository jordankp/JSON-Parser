#include <iostream>

#include "JsonArray.h"
#include "JsonObject.h"
#include "Pair.h"
#include "JsonPath.h"

using std::endl;

extern void printSpaces(std::ostream& out, unsigned cnt);

JsonArray::JsonArray()
	: capacity(2)
	, size(0)
{
	values = new Value*[capacity];
}

JsonArray::JsonArray(const JsonArray& rhs)
{
	copy(rhs);
}

JsonArray& JsonArray::operator=(const JsonArray& rhs)
{
	if (this != &rhs)
	{
		clear();
		copy(rhs);
	}
	return *this;
}

JsonArray::~JsonArray()
{
	clear();
}

Value* JsonArray::clone() const
{
	return new JsonArray(*this);
}

void JsonArray::print(std::ostream& out, const bool saveSpace, unsigned spaces) const
{
	out << "[";
	if (!saveSpace)
		out << endl;

	for (unsigned i = 0; i < size; ++i)
	{
		if (!saveSpace)
			printSpaces(out, spaces + 1);

		values[i]->print(out, saveSpace, spaces + 1);
		if (i != size - 1)
			out << ",";
		if (!saveSpace)
			out << endl;
	}
	if (!saveSpace)
		printSpaces(out, spaces);

	out << "]";
}

bool JsonArray::findAndReplace(const JsonPath& path, const Value& val, unsigned i)
{
	int idx = path[i].idx;
	if (i >= path.getSize() || idx == -1 || idx >= size)
		return false;
	if (i == path.getSize() - 1)
	{
		Value* tmp = val.clone();
		delete values[idx];
		values[idx] = tmp;
		return true;
	}
	return values[idx]->findAndReplace(path, val, i + 1);
}

const Value* JsonArray::findByPath(const JsonPath& path, unsigned i) const
{
	int idx = path[i].idx;
	if (i >= path.getSize() || idx == -1 || idx >= size)
		return false;
	if (i == path.getSize() - 1)
	{
		return values[idx];
	}
	return values[idx]->findByPath(path, i + 1);
}

void JsonArray::findAndGet(const String& key, JsonArray& arr) const
{
	for (unsigned i = 0; i < size; ++i)
		values[i]->findAndGet(key, arr);
}

bool JsonArray::findAndDelete(const JsonPath& path, unsigned i)
{
	int idx = path[i].idx;
	if (i == path.getSize() || idx == -1 || idx >= size)
		return false;
	if (i == path.getSize() - 1)
	{
		remove(idx);
		return true;
	}
	return values[idx]->findAndDelete(path, i + 1);
}

bool JsonArray::findAndAdd(const JsonPath& path, const Value& val, unsigned i)
{
	if (i >= path.getSize())
		return false;

	int idx = path[i].idx;
	if (idx == -1)
		return false;
	if (i == path.getSize() - 1)
	{
		if (idx < size)
			return false;
		add(val);
		return true;
	}
	else
	{
		if (idx < size)
			return values[idx]->findAndAdd(path, val, i + 1);

		JsonObject obj;
		if (!obj.findAndAdd(path, val, i + 1))
			return false;
		add(obj);
		return true;
	}
}

bool JsonArray::operator==(const Value& rhs) const
{
	try
	{
		const JsonArray& rhsArr = dynamic_cast<const JsonArray&>(rhs);
		return areEqual(rhsArr);
	}
	catch (const std::bad_cast&)
	{
	}
	return false;
}

void JsonArray::add(const Value& t)
{
	if (size == capacity)
	{
		resize(2 * capacity);
	}
	values[size++] = t.clone();
}

unsigned JsonArray::getSize() const
{
	return size;
}

const Value& JsonArray::operator[](unsigned idx) const
{
	if(idx >= size)
		throw std::exception("Out of range.");
	return *(values[idx]);
}

bool JsonArray::areEqual(const JsonArray& oth) const
{
	if (size == oth.size)
		for (unsigned i = 0; i < size; ++i)
			if (values[i] != oth.values[i])
				return false;
	return true;
}

void JsonArray::copy(const JsonArray& rhs)
{
	values = new Value*[rhs.capacity];
	for (size = 0; size < rhs.size; ++size)
	{
		values[size] = rhs.values[size]->clone();
	}
	capacity = rhs.capacity;
}

void JsonArray::clear()
{
	for (unsigned i = 0; i < size; ++i)
	{
		delete values[i];
	}
	delete[] values;
	values = nullptr;
	size = 0;
	capacity = 0;
}

void JsonArray::resize(unsigned newCap)
{
	Value** tmp = new Value*[newCap];
	for (unsigned i = 0; i < size; ++i)
	{
		tmp[i] = values[i];
	}
	delete[] values;
	values = tmp;
	capacity = newCap;
}

void JsonArray::remove(int idx)
{
	delete values[idx];
	for (unsigned i = idx; i < size - 1; ++i)
	{
		values[i] = values[i + 1];
	}
	--size;
}