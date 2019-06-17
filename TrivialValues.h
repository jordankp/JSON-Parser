#ifndef TRIVIALVALUES_HEADER_INCLUDED
#define TRIVIALVALUES_HEADER_INCLUDED

#include "Value.h"

class Number : public Value
{
	double num;
public:
	Number(double x);
	virtual Value* clone() const;
	virtual void print(std::ostream& out, bool saveSpace, unsigned spaces) const;
	virtual bool findAndReplace(const JsonPath& path, const Value& val, unsigned i);
	virtual const Value* findByPath(const JsonPath& path, unsigned i) const;
	virtual void findAndGet(const String& key, JsonArray& arr) const;
	virtual bool findAndDelete(const JsonPath& path, unsigned i);
	virtual bool findAndAdd(const JsonPath& path, const Value& val, unsigned i);
	virtual bool operator==(const Value& rhs) const;

	double get() const;
	void set(double x);
};

class Bool : public Value
{
	bool val;
public:
	Bool(bool b);
	virtual Value* clone() const;
	virtual void print(std::ostream& out, const bool saveSpace, unsigned spaces) const;
	virtual bool findAndReplace(const JsonPath& path, const Value& val, unsigned i);
	virtual const Value* findByPath(const JsonPath& path, unsigned i) const;
	virtual void findAndGet(const String& key, JsonArray& arr) const;
	virtual bool findAndDelete(const JsonPath& path, unsigned i);
	virtual bool findAndAdd(const JsonPath& path, const Value& val, unsigned i);
	virtual bool operator==(const Value& rhs) const;

	bool get() const;
	void set(bool b);
};

class Null : public Value
{
public:
	virtual Value* clone() const;
	virtual void print(std::ostream& out, bool saveSpace, unsigned spaces) const;
	virtual bool findAndReplace(const JsonPath& path, const Value& val, unsigned i);
	virtual const Value* findByPath(const JsonPath& path, unsigned i) const;
	virtual void findAndGet(const String& key, JsonArray& arr) const;
	virtual bool findAndDelete(const JsonPath& path, unsigned i);
	virtual bool findAndAdd(const JsonPath& path, const Value& val, unsigned i);
	virtual bool operator==(const Value& rhs) const;
};



#endif