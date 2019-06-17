#ifndef JSONARRAY_HEADER_INCLUDED
#define JSONARRAY_HEADER_INCLUDED

#include "Value.h"

class JsonArray : public Value
{
	Value** values;
	unsigned size;
	unsigned capacity;

public:
	JsonArray();
	JsonArray(const JsonArray& rhs);
	JsonArray& operator=(const JsonArray& rhs);
	virtual ~JsonArray();

	virtual Value* clone() const;
	virtual void print(std::ostream& out, const bool saveSpace, unsigned spaces) const;
	virtual bool findAndReplace(const JsonPath& path, const Value& val, unsigned i);
	virtual const Value* findByPath(const JsonPath& path, unsigned i) const;
	virtual void findAndGet(const String& key, JsonArray& arr) const;
	virtual bool findAndDelete(const JsonPath& path, unsigned i);
	virtual bool findAndAdd(const JsonPath& path, const Value& val, unsigned i);
	virtual bool operator==(const Value& rhs) const;

	void add(const Value& t);
	unsigned getSize() const;

	const Value& operator[](unsigned idx) const;
private:
	bool areEqual(const JsonArray& oth) const;
	void copy(const JsonArray& rhs);
	void clear();
	void resize(unsigned newCap);
	void remove(int idx);
};

#endif
