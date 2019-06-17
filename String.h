#ifndef STRING_HEADER_INCLUDED
#define STRING_HEADER_INCLUDED

#include "Value.h"

class String : public Value
{
	char* str;

public:
	String(const char* str = nullptr);
	String(const String& rhs);
	String& operator=(const String& rhs);
	virtual ~String();

	virtual Value* clone() const;
	virtual void print(std::ostream& out, const bool saveSpace, unsigned spaces) const;
	virtual bool findAndReplace(const JsonPath& path, const Value& val, unsigned i);
	virtual const Value* findByPath(const JsonPath& path, unsigned i) const;
	virtual void findAndGet(const String& key, JsonArray& arr) const;
	virtual bool findAndDelete(const JsonPath& path, unsigned i);
	virtual bool findAndAdd(const JsonPath& path, const Value& val, unsigned i);
	virtual bool operator==(const Value& rhs) const;

	const char* get() const;
	void set(const char* str);

private:
	void copy(const char* str);
};


#endif
