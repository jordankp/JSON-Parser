#include <iostream>
#include <fstream>
#include "JsonText.h"
#include <cassert>

using std::cout;
using std::endl;

extern bool isDigit(char c);

JsonText::JsonText(const char* text)
{
	if (!text || *text == NULL)
		throw std::exception("Text is empty.");

	unsigned len = strlen(text);
	this->text = new char[len + 1];
	strcpy_s(this->text, len + 1, text);
	JsonPath path;
	unsigned pos = 0;
	if (!isValidValue(pos, path, true))
	{
		delete[] this->text;
		throw std::exception("Invalid JSON syntax.");
	}
}

JsonText::JsonText(const JsonText& rhs)
{
	copy(rhs);
}

JsonText& JsonText::operator=(const JsonText& rhs)
{
	if (this != &rhs)
	{
		char* t = text;
		copy(rhs);
		delete[] t;
	}
	return *this;
}

JsonText::~JsonText()
{
	clear();
}

const char* JsonText::get() const
{
	return text;
}

char JsonText::operator[](unsigned pos) const
{
	return text[pos];
}

bool JsonText::isValidValue(unsigned& pos, JsonPath& path, bool mustEnd) const
{
	ignoreSpace(pos);
	bool valid;
	if (text[pos] == '{')
	{
		valid = isValidObject(pos, path);
	}
	else if (text[pos] == '[')
	{
		valid = isValidArray(pos, path);
	}
	else if (text[pos] == '"')
	{
		valid = isValidString(pos, path);
	}
	else if (isDigit(text[pos]))
	{
		valid = isValidNumber(pos, path);
	}
	else if (text[pos] == 't' || text[pos] == 'f')
	{
		valid = isValidBool(pos, path);
	}
	else if (text[pos] == 'n')
	{
		valid = isValidNull(pos, path);
	}
	else if (text[pos] == NULL)
	{
		return true;
	}
	else
	{
		cout << "Unknown value at " << path << "." << endl;
		return false;
	}
	if (!valid)
		return false;
	if (mustEnd && text[pos] != NULL)
	{
		cout << "Text doesn't end properly." << endl;
		return false;
	}
	return true;
}

bool JsonText::isValidObject(unsigned& pos, JsonPath& path) const
{
	++pos;
	ignoreSpace(pos);
	
	unsigned keyCnt = 0;
	while (text[pos] != '}')
	{
		if (text[pos] != '"')
		{
			cout << "Key " << keyCnt << " must begin with '\"' at " << path << "." << endl;
			return false;
		}
		if (!isValidString(pos, path))
			return false;

		if (text[pos] != ':')
		{
			cout << "Must have a ':' after key " << keyCnt << " at " << path << "." << endl;
			return false;
		}

		char key[256];
		getKey(key, text + pos);
		path.add(key);

		++pos;
		if (!isValidValue(pos, path, false))
			return false;
		path.remove();
		++keyCnt;

		if (text[pos] == ',')
		{
			++pos;
			ignoreSpace(pos);
			if (text[pos] == '}')
			{
				cout << "Object must not have a ',' after last value at " << path << "." << endl;
				return false;
			}
		}
		else if (text[pos] != '}')
		{
			cout << "Object must end with '}' or have a ',' after value \"" << key << "\" at " << path << "." << endl;
			return false;
		}
	}
	++pos;
	ignoreSpace(pos);
	return true;
}

bool JsonText::isValidArray(unsigned& pos, JsonPath& path) const
{
	++pos;
	ignoreSpace(pos);
	
	unsigned i = 0;
	while (text[pos] != ']')
	{
		path.setLastIndex(i);
		if (!isValidValue(pos, path, false))
		{
			return false;
		}
		if (text[pos] == ',')
		{
			++i;
			++pos;
			ignoreSpace(pos);
			if (text[pos] == ']')
			{
				cout << "Array must not have a ',' after last value at " << path << "." << endl;
				return false;
			}
		}
		else if (text[pos] != ']')
		{
			path.setLastIndex(-1);
			cout << "Array must end with ']' or have a ',' after value " << i << " at " << path << "." << endl;
			return false;
		}
	}
	path.setLastIndex(-1);
	++pos;
	ignoreSpace(pos);
	return true;
}

bool JsonText::isValidString(unsigned& pos, JsonPath& path) const
{
	++pos;
	while (text[pos] && text[pos] != '"')
		++pos;
	if (text[pos] != '"')
	{
		cout << "String must end with '\"' at " << path << "." << endl;
		return false;
	}
		
	++pos;
	ignoreSpace(pos);
	return true;
}

bool JsonText::isValidNumber(unsigned& pos, JsonPath& path) const
{
	while (isDigit(text[pos]))
		++pos;
	if (text[pos] == '.')
	{
		++pos;
		while (isDigit(text[pos]))
			++pos;
	}

	ignoreSpace(pos);
	return true;
}

bool JsonText::isValidBool(unsigned& pos, JsonPath& path) const
{
	unsigned move = 0;
	if (strncmp("true", text + pos, 4) == 0)
	{
		move = 4;
	}
	else if (strncmp("false", text + pos, 5) == 0)
	{
		move = 5;
	}
	if (move)
	{
		pos += move;
		ignoreSpace(pos);
		return true;
	}
	else
	{
		cout << "Invalid value at " << path << "." << endl;
		return false;
	}
}

bool JsonText::isValidNull(unsigned& pos, JsonPath& path) const
{
	if (strncmp("null", text + pos, 4) == 0)
	{
		pos += 4;
		ignoreSpace(pos);
		return true;
	}
	else
	{
		cout << "Invalid value at " << path << "." << endl;
		return false;
	}
}

void JsonText::copy(const JsonText& rhs)
{
	unsigned len = strlen(rhs.text);
	text = new char[len + 1];
	strcpy_s(text, len + 1, rhs.text);
}

void JsonText::clear()
{
	delete[] text;
}

void JsonText::getKey(char* dest, const char* src) const
{
	while (*src != '"')
		--src;
	--src;
	while (*src != '"')
		--src;
	++src;
	while (*src != '"')
	{
		*dest = *src;
		++dest;
		++src;
	}
	*dest = NULL;
}

void JsonText::goToKey(unsigned& pos) const
{
	while (text[pos] != '"')
	{
		assert(text[pos] != NULL);
		++pos;
	}
}

void JsonText::ignoreSpace(unsigned& pos) const
{
	while (text[pos] == ' ' || text[pos] == '\t')
		++pos;
}

void JsonText::jumpOver(unsigned& pos) const
{
	while (text[pos] != ' ' && text[pos] != '\t' && text[pos] != ',' && text[pos] != '}' && text[pos] != ']' && text[pos] != NULL)
	{
		++pos;
	}
	if (text[pos] == ',')
		++pos;
}

void JsonText::extractString(char* dest, unsigned& pos) const
{
	while (text[pos] != '"')
	{
		*dest = text[pos];
		++dest;
		++pos;
	}
	*dest = NULL;
}

void JsonText::moveToValue(unsigned& pos) const
{
	while (text[pos] == ' ' || text[pos] == '\t' || text[pos] == ',' ||
		text[pos] == ':' || text[pos] == ']' || text[pos] == '}')
		++pos;
}