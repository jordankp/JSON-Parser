#include <iostream>
#include <fstream>
#include "Json.h"
#include "JsonPath.h"
#include "JsonText.h"

using std::cout;
using std::cin;
using std::endl;

extern bool isDigit(char c);
static char* read(std::ifstream& fin);
static bool resizeStr(char** str, unsigned* size, unsigned newSize);
static unsigned separate(char* str);
static bool isJsonObject(const char* str);


Json& Json::getJson()
{
	static Json theJson;
	return theJson;
}

Json::Json()
	: object(nullptr)
	, items(nullptr)
{
}

Json::~Json()
{
	clear();
}

void Json::validateCommand(const char* fileName) const
{
	std::ifstream fin(fileName);
	if (!fin)
	{
		cout << "Can't open file " << fileName << "." << endl;
		return;
	}
	char* str = read(fin);
	if (!str)
	{
		fin.clear();
		fin.close();
		return;
	}
	try
	{
		JsonText txt(str);
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
		fin.clear();
		fin.close();
		return;
	}
	cout << "Json syntax is valid." << endl;
	fin.clear();
	fin.close();
}

void Json::loadCommand(const char* fileName)
{
	std::ifstream fin(fileName);
	if (!fin)
	{
		cout << "Can't open file " << fileName << "." << endl;
		return;
	}
	char* str = read(fin);
	if (!str)
		cout << "Bad allocation." << endl;
	else if(!isJsonObject(str))
		cout << "File does not start with '{'." << endl;
	else
	{
		delete object;
		object = parse(str);
		if (object)
			cout << "Successfully loaded object." << endl;
	}
	fin.clear();
	fin.close();
	delete[] str;
}

void Json::loadItemsCommand(const char* key)
{
	if (!object)
	{
		cout << "No object is loaded." << endl;
		return;
	}
	delete items;
	try
	{
		items = new JsonArray;
		object->findAndGet(key, *items);
	}
	catch (const std::bad_alloc&)
	{
		cout << "Bad allocation" << endl;
		return;
	}
	cout << items->getSize() << " items found and loaded." << endl;
}

void Json::printCommand() const
{
	print(object, cout, false);
}

void Json::printItemsCommand() const
{
	print(items, cout, false);
}

void Json::printSingleItemCommand(const char* idx) const
{
	unsigned i = atoi(idx);
	if (*idx != '0' && i == 0)
	{
		cout << "Invalid number." << endl;
		return;
	}

	try
	{
		const Value& curVal = (*items)[i];
		print(&curVal, cout, false);
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
		return;
	}
}

void Json::saveCommand(char* buffer, bool saveSpace, bool hasPath) const
{
	save(object, buffer, saveSpace, hasPath);
}

void Json::saveItemsCommand(char* buffer, bool saveSpace) const
{
	save(items, buffer, saveSpace, false);
}

void Json::saveSingleItemCommand(char* buffer, bool saveSpace) const
{
	unsigned i = atoi(buffer);
	if (*buffer != '0' && i == 0)
	{
		cout << "Invalid number." << endl;
		return;
	}
	while (isDigit(*buffer))
		++buffer;
	if (*buffer == NULL)
	{
		cout << "No file name is given." << endl;
		return;
	}
	else
		++buffer;

	try
	{
		const Value& curVal = (*items)[i];
		save(&curVal, buffer, saveSpace, false);
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
		return;
	}
}

void Json::replaceCommand(char* buffer)
{
	if (!object)
	{
		cout << "No object is loaded." << endl;
		return;
	}
	unsigned i = separate(buffer);
	char* path = buffer;
	char* text = buffer + i;
	Value* newVal = nullptr;
	try
	{
		JsonPath pathObj(path);
		newVal = parse(text);
		if (!newVal)
			return;
		if (object->findAndReplace(pathObj, *newVal, 0))
			cout << "Successfully replaced value." << endl;
		else
			cout << "This path does not lead to an existing value." << endl;
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}
	delete newVal;
}

void Json::swapCommand(char* buffer)
{
	if (!object)
	{
		cout << "No object is loaded." << endl;
		return;
	}

	unsigned i = separate(buffer);
	char* path1Str = buffer;
	char* path2Str = buffer + i;

	try
	{
		JsonPath path1(path1Str);
		JsonPath path2(path2Str);
		if (areConnected(path1, path2))
			cout << "Paths are connected." << endl;
		else if(swap(path1, path2))
			cout << "Successfully swapped values." << endl;
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}
}

void Json::deleteCommand(const char* pathStr)
{
	if (!object)
	{
		cout << "No object is loaded." << endl;
		return;
	}
	try
	{
		JsonPath path(pathStr);
		if (object->findAndDelete(path, 0))
			cout << "Successfully deleted." << endl;
		else
			cout << "Path does not lead to an existing value." << endl;
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}
}

void Json::addCommand(char* buffer)
{
	if (!object)
	{
		cout << "No object is loaded." << endl;
		return;
	}
	unsigned i = separate(buffer);
	char* pathStr = buffer;
	char* valueStr = buffer + i;
	Value* value = nullptr;
	try
	{
		JsonPath path(pathStr);
		JsonText valueText(valueStr);
		value = Value::create(valueText);
		if (!object->findAndAdd(path, *value, 0))
			cout << "Path leads to an existing value." << endl;
		else
			cout << "Successfully added value." << endl;
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}
	delete value;
}

Value* Json::parse(const char* str)
{
	if (!str)
		return nullptr;
	try
	{
		JsonText text(str);
		return Value::create(text);
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}
	return nullptr;
}

bool Json::printExcerpt(const Value* curVal, std::ofstream& fout, bool saveSpace, const char* path) const
{
	try
	{
		JsonPath pathObj(path);
		const Value* excerpt = curVal->findByPath(pathObj, 0);
		if (!excerpt)
		{
			cout << "This path does not lead to an existing value." << endl;
			return false;
		}
		else
			excerpt->print(fout, saveSpace, 0);
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
		return false;
	}
	return true;
}

void Json::print(const Value* curVal, std::ostream& out, bool saveSpace) const
{
	if (curVal)
	{
		curVal->print(out, saveSpace, 0);
		if (!saveSpace)
			out << endl;
	}
	else
	{
		cout << "Nothing to loaded." << endl;
	}
}

void Json::save(const Value* curVal, char* buffer, bool saveSpace, bool hasPath) const
{
	if (!curVal)
	{
		cout << "Nothing is loaded." << endl;
		return;
	}
	char* fileName = nullptr;
	char* path = nullptr;
	if (hasPath)
	{
		unsigned i = separate(buffer);
		path = buffer;
		fileName = buffer + i;
	}
	else
		fileName = buffer;

	changeFileName(fileName);
	std::ofstream fout(fileName);
	if (!fout)
	{
		cout << "Can't open/create file " << fileName << "." << endl;
		return;
	}

	bool success = true;
	if (hasPath)
		success = printExcerpt(curVal, fout, saveSpace, path);
	else
		print(curVal, fout, saveSpace);

	if (!fout)
		cout << "Problem with file stream." << endl;
	else if (success)
		cout << "Successfully saved in " << fileName << "." << endl;
	fout.clear();
	fout.close();
}

bool Json::swap(const JsonPath& path1, const JsonPath& path2)
{
	const Value* val1 = object->findByPath(path1, 0);
	const Value* val2 = object->findByPath(path2, 0);
	if (!val1)
	{
		cout << "First path leads to an unexisting value." << endl;
		return false;
	}
	if (!val2)
	{
		cout << "Second path leads to an unexisting value." << endl;
		return false;
	}

	const Value* tmpVal = val2->clone();

	try
	{
		object->findAndReplace(path2, *val1, 0);
		object->findAndReplace(path1, *tmpVal, 0);
	}
	catch (const std::bad_alloc&)
	{
		cout << "Bad allocation." << endl;
		delete tmpVal;
		return false;
	}
	delete tmpVal;
	return true;
}

bool Json::areConnected(const JsonPath& path1, const JsonPath& path2) const
{
	unsigned i = 0;
	unsigned size1 = path1.getSize();
	unsigned size2 = path2.getSize();
	while (i < size1 && i < size2 && path1[i].str == path2[i].str && path1[i].idx == path2[i].idx)
		++i;

	if (i == size1 || i == size2)
		return true;

	if (path1[i].str == path2[i].str &&
		((path1[i].idx == -1 && path2[i].idx != -1) || (path1[i].idx != -1 && path2[i].idx == -1)))
		return true;
	else
		return false;

	return false;
}

void Json::clear()
{
	delete object;
	delete items;
	object = nullptr;
	items = nullptr;
}

void Json::changeFileName(char* name) const
{
	bool done = false;
	do
	{
		std::ifstream tmp(name);
		if (tmp)
		{
			cout << "File " << name << " already exists. Enter 'y' if you want to overwrite it," << endl;
			cout << "and 'n' if you want to enter another file name: ";
			char c;
			cin >> c;
			cin.ignore();
			if (c == 'y')
			{
				done = true;
			}
			else if (c == 'n')
			{
				cout << "Enter file name: ";
				cin >> name;
			}
			else
			{
				cout << "Invalid answear." << endl;
			}
			tmp.clear();
			tmp.close();
		}
		else
			done = true;
	} while (!done);
}

static char* read(std::ifstream& fin)
{
	unsigned size = 16;
	char* str = new(std::nothrow) char[size];
	if (!str)
		return nullptr;
	
	unsigned i = 0;
	while (fin)
	{
		if (i == size && !resizeStr(&str, &size, 2 * size))
		{
			delete[] str;
			return nullptr;
		}

		char c = fin.get();
		if (fin.eof())
			break;
		else if (!fin)
		{
			cout << "Can't read from file." << endl;
			delete[] str;
			return nullptr;
		}

		if (c != '\n')
		{
			str[i] = c;
			++i;
		}
	}
	str[i] = NULL;
	return str;
}

static bool resizeStr(char** str, unsigned* size, unsigned newSize)
{
	char* tmp = new(std::nothrow) char[newSize];
	if (!tmp)
		return false;

	strcpy_s(tmp, strlen(*str) + 1, *str);
	delete[] *str;
	*str = tmp;
	*size = newSize;
	return true;
}

static unsigned separate(char* str)
{
	char* beg = str;
	while (*str && *str != ' ')
		++str;
	if (*str == NULL)
		return str - beg;
	*str = NULL;
	return str - beg + 1;
}

static bool isJsonObject(const char* str)
{
	while (*str == ' ' || *str == '\t')
		++str;
	if (*str != '{')
		return false;
	return true;
}