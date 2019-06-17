#ifndef JSONPATH_HEADER_INCLUDED
#define JSONPATH_HEADER_INCLUDED

#include "String.h"

class JsonPath
{
	struct Key
	{
		String str;
		int idx;
	};

	Key** path;
	unsigned size;
	unsigned capacity;
public:
	JsonPath();
	JsonPath(const char* str);
	JsonPath(const JsonPath& rhs);
	JsonPath& operator=(const JsonPath& rhs);
	~JsonPath();

	void add(const String& str, int idx = -1);
	void remove();
	void setLastIndex(int idx);
	unsigned getSize() const;

	const Key& operator[](unsigned i) const;

private:
	void resize(unsigned newCap);
	void clear();
	void copy(const JsonPath& rhs);
	bool isValid(const char* str) const;
	void makePath(const char* str);
	int toInt(char* str, unsigned* i) const;
};

std::ostream& operator<<(std::ostream& out, const JsonPath& path);

#endif
