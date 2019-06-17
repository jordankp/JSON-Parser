#ifndef JSON_HEADER_INCLUDED
#define JSON_HEADER_INCLUDED

#include "Value.h"
#include "JsonArray.h"

class Json
{
	Value* object;
	JsonArray* items;
private:
	Json();
	Json(const Json& rhs);
	void operator=(const Json& rhs);
	~Json();
public:
	static Json& getJson();

	void validateCommand(const char* fileName) const;
	void loadCommand(const char* fileName);
	void loadItemsCommand(const char* buffer);

	void printCommand() const;
	void printItemsCommand() const;
	void printSingleItemCommand(const char* idx) const;

	void saveCommand(char* buffer, bool saveSpace, bool hasPath) const;
	void saveItemsCommand(char* buffer, bool saveSpace) const;
	void saveSingleItemCommand(char* buffer, bool saveSpace) const;

	void replaceCommand(char* buffer);
	void swapCommand(char* buffer);
	void deleteCommand(const char* path);
	void addCommand(char* buffer);

private:
	Value * parse(const char* str);
	bool printExcerpt(const Value* curVal, std::ofstream& fout, bool saveSpace, const char* path) const;
	void print(const Value* curVal, std::ostream& out, bool saveSpace) const;
	void save(const Value* curVal, char* buffer, bool saveSpace, bool hasPath) const;
	bool swap(const JsonPath& path1, const JsonPath& path2);
	bool areConnected(const JsonPath& path1, const JsonPath& path2) const;

	void clear();
	void changeFileName(char* name) const;
};


#endif