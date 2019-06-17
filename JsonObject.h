#ifndef JSONOBJECT_HEADER_INCLUDED
#define JSONOBJECT_HEADER_INCLUDED

#include "Value.h"

class JsonObject : public Value
{
	Pair** pairs;
	unsigned size;
	unsigned capacity;

public:
	JsonObject();
	JsonObject(const JsonObject& rhs);
	JsonObject& operator=(const JsonObject& rhs);
	virtual ~JsonObject();

	virtual Value* clone() const;
	virtual void print(std::ostream& out, const bool saveSpace, unsigned spaces) const;
	virtual bool findAndReplace(const JsonPath& path, const Value& val, unsigned i);
	virtual const Value* findByPath(const JsonPath& path, unsigned i) const;
	virtual void findAndGet(const String& key, JsonArray& arr) const;
	virtual bool findAndDelete(const JsonPath& path, unsigned i);
	virtual bool findAndAdd(const JsonPath& path, const Value& val, unsigned i);
	virtual bool operator==(const Value& rhs) const;

	JsonObject& operator+=(const Pair& t);
	JsonObject operator+(const Pair& t) const;
	unsigned getSize() const;
private:
	bool areEqual(const JsonObject& oth) const;
	Pair * find(const String& key);
	bool findAndAddCase1(const JsonPath& path, const Value& val, unsigned i);
	bool findAndAddCase2(const JsonPath& path, const Value& val, unsigned i);
	void copy(const JsonObject& rhs);
	void clear();
	void resize(unsigned newCap);
	void remove(int idx);
};


#endif
