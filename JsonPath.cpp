#include <iostream>
#include "JsonPath.h"

extern bool isDigit(char c);
extern unsigned toDigit(char c);

JsonPath::JsonPath()
	: size(0)
	, capacity(2)
{
	path = new Key*[capacity];
}

JsonPath::JsonPath(const char* str)
	: size(0)
	, capacity(2)
{
	if (!str)
		throw std::exception("Path can't be empty.");

	if (!isValid(str))
		throw std::exception("Bad path syntax.");

	makePath(str);
}

JsonPath::JsonPath(const JsonPath& rhs)
{
	copy(rhs);
}

JsonPath& JsonPath::operator=(const JsonPath& rhs)
{
	if (this != &rhs)
	{
		clear();
		copy(rhs);
	}
	return *this;
}

JsonPath::~JsonPath()
{
	clear();
}

void JsonPath::add(const String& rhs, int idx)
{
	if (size == capacity)
	{
		resize(2 * capacity);
	}
	path[size] = new Key;
	path[size]->str = String(rhs);
	path[size]->idx = idx;
	++size;
}

void JsonPath::remove()
{
	if (size > 0)
	{
		delete path[--size];
	}
}

void JsonPath::setLastIndex(int idx)
{
	if (size == 0)
		return;
	path[size - 1]->idx = (idx < -1) ? -1 : idx;
}

unsigned JsonPath::getSize() const
{
	return size;
}

const JsonPath::Key& JsonPath::operator[](unsigned i) const
{
	if (i >= size)
		throw std::exception("Out of range.");
	return *path[i];
}

void JsonPath::resize(unsigned newCap)
{
	Key** tmp = new Key*[newCap];
	for (unsigned i = 0; i < size; ++i)
	{
		tmp[i] = path[i];
	}
	delete[] path;
	path = tmp;
	capacity = newCap;
}

void JsonPath::clear()
{
	for (unsigned i = 0; i < size; ++i)
		delete path[i];
	delete[] path;
	path = nullptr;
	size = 0;
}

void JsonPath::copy(const JsonPath& rhs)
{
	path = new Key*[rhs.capacity];
	while (size < rhs.size)
	{
		try
		{
			path[size] = new Key(*(rhs.path[size]));
		}
		catch (const std::bad_alloc&)
		{
			clear();
			throw;
		}
		++size;
	}
	capacity = rhs.capacity;
}

bool JsonPath::isValid(const char* str) const
{
	if (*str == NULL)
		return false;
	while (*str)
	{
		if (*str == '.' || *str == '[')
			return false;
		while (*str != '.')
		{
			if (*str == '[')
			{
				++str;
				if (!isDigit(*str))
					return false;
				while (isDigit(*str))
					++str;
				if (*str != ']')
					return false;
			}
			++str;
			if (*str == NULL)
				return true;
		}
		++str;
		if (*str == NULL)
			return false;
	}
	return true;
}

void JsonPath::makePath(const char* str)
{
	unsigned len = strlen(str);
	char* tmp = new char[len + 1];
	strcpy_s(tmp, len + 1, str);
	path = new(std::nothrow) Key*[capacity];
	if (!path)
	{
		delete[] tmp;
		throw std::bad_alloc();
	}

	unsigned i = 0;
	while (tmp[i])
	{
		char* beg = tmp + i;
		while (tmp[i] && tmp[i] != '.' && tmp[i] != '[')
			++i;
		bool addIndex = (tmp[i] == '[') ? true : false;
		int idx = -1;

		if (tmp[i])
		{
			tmp[i++] = NULL;
			if(addIndex)
				idx = toInt(tmp, &i);
			if (tmp[i] == '.')
				++i;
		}

		try
		{
			add(String(beg), idx);
		}
		catch (const std::bad_alloc&)
		{
			delete[] tmp;
			delete[] path;
			throw;
		}
	}
}

int JsonPath::toInt(char* str, unsigned* i) const
{
	int num = 0;
	while (str[*i] != ']')
	{
		num = 10 * num + toDigit(str[*i]);
		++*i;
	}
	++*i;
	return num;
}

std::ostream& operator<<(std::ostream& out, const JsonPath& path)
{
	unsigned size = path.getSize();
	if (size == 0)
	{
		out << "the beginning";
		return out;
	}

	out << "\"";
	for (unsigned i = 0; i < size; ++i)
	{
		out << path[i].str.get();
		if (path[i].idx != -1)
			out << "[" << path[i].idx << "]";
		if (i != size - 1)
			out << ".";
	}
	out << "\"";
	return out;
}