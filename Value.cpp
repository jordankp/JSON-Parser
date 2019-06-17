#include <iostream>

#include "Value.h"
#include "Pair.h"
#include "TrivialValues.h"
#include "JsonArray.h"
#include "JsonObject.h"
#include "String.h"
#include "JsonText.h"

using std::nothrow;

bool isDigit(char c);
unsigned toDigit(char c);

bool Value::operator!=(const Value& rhs) const
{
	return !(*this == rhs);
}

Value* Value::create(const JsonText& text)
{
	unsigned pos = 0;
	return createValue(pos, text);
}

Value* Value::createValue(unsigned& pos, const JsonText& text)
{
	text.moveToValue(pos);
	if (text[pos] == '{')
	{
		return (Value*)createObject(pos, text);
	}
	else if (text[pos] == '[')
	{
		return (Value*)createArray(pos, text);
	}
	else if (text[pos] == '"')
	{
		return createString(pos, text);
	}
	else if (isDigit(text[pos]))
	{
		return createNumber(pos, text);
	}
	else if (text[pos] == 't' || text[pos] == 'f')
	{
		return createBool(pos, text);
	}
	else if (text[pos] == 'n')
	{
		return createNull(pos, text);
	}
	else
		return nullptr;
}

JsonObject* Value::createObject(unsigned& pos, const JsonText& text)
{
	JsonObject* obj = new JsonObject;
	++pos;
	text.ignoreSpace(pos);

	while (text[pos] != '}')
	{
		text.goToKey(pos);
		String* key = nullptr;
		Value* val = nullptr;

		try
		{
			key = createString(pos, text);
			val = createValue(pos, text);
			Pair pair(*key, *val);
			*obj += pair;
		}
		catch (const std::bad_alloc&)
		{
			delete obj;
			delete key;
			delete val;
			throw;
		}
		delete val;
		delete key;
		text.ignoreSpace(pos);
	}
	++pos;
	return obj;
}

JsonArray* Value::createArray(unsigned& pos, const JsonText& text)
{
	JsonArray* arr = new JsonArray;
	++pos;
	text.ignoreSpace(pos);

	while (text[pos] != ']')
	{
		Value* val = nullptr;
		try
		{
			val = createValue(pos, text);
			arr->add(*val);
		}
		catch (const std::bad_alloc&)
		{
			delete arr;
			delete val;
			throw;
		}
		delete val;
		text.ignoreSpace(pos);
	}
	++pos;
	return arr;
}

String* Value::createString(unsigned& pos, const JsonText& text)
{
	//v momenta sme na "
	++pos;
	char buffer[256];
	text.extractString(buffer, pos);
	++pos;
	return new String(buffer);
}

Number* Value::createNumber(unsigned& pos, const JsonText& text)
{
	bool isNegative = (text[pos] == '-') ? true : false;
	if (isNegative)
		++pos;
	double num = 0;
	while (isDigit(text[pos]))
	{
		num = 10 * num + toDigit(text[pos]);
		++pos;
	}
	if (text[pos] == '.')
	{
		++pos;
		unsigned t = 10;
		while (isDigit(text[pos]))
		{
			num += (double)toDigit(text[pos]) / t;
			t *= 10;
			++pos;
		}
	}
	if (isNegative)
		num *= -1;
	return new Number(num);
}

Bool* Value::createBool(unsigned& pos, const JsonText& text)
{
	bool b;
	if (text[pos] == 't')
		b = true;
	else
		b = false;
	text.jumpOver(pos);
	return new Bool(b);
}

Null* Value::createNull(unsigned& pos, const JsonText& text)
{
	text.jumpOver(pos);
	return new Null;
}

bool isDigit(char c)
{
	return c >= '0' && c <= '9';
}

unsigned toDigit(char c)
{
	return c - '0';
}