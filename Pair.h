#ifndef PAIR_HEADER_INCLUDED
#define PAIR_HEADER_INCLUDED

#include "String.h"

class Pair
{
	friend class JsonObject;

	String key;
	Value* val;
public:
	Pair(const String& key, const Value& val);
	Pair(const Pair& rhs);
	Pair& operator=(const Pair& rhs);
	~Pair();

	void print(std::ostream& out, const bool saveSpace, unsigned spaces) const;
	Pair* clone() const;

	const String& getKey() const;
	const Value* getVal() const;
	void setKey(const String& key);
	void setVal(const Value& val);
private:
	void copy(const Pair& rhs);
};



#endif
