#ifndef VALUE_HEADER_INCLUDED
#define VALUE_HEADER_INCLUDED

class Pair;
class Value;
class String;
class Number;
class Bool;
class Null;
class JsonObject;
class JsonArray;
class JsonText;
class JsonPath;


class Value
{
public:
	virtual ~Value() {};
	virtual Value* clone() const = 0;
	virtual void print(std::ostream& out, const bool saveSpace, unsigned spaces) const = 0;
	virtual bool findAndReplace(const JsonPath& path, const Value& val, unsigned i) = 0;
	virtual const Value* findByPath(const JsonPath& path, unsigned i) const = 0;
	virtual void findAndGet(const String& key, JsonArray& arr) const = 0;
	virtual bool findAndDelete(const JsonPath& path, unsigned i) = 0;
	virtual bool findAndAdd(const JsonPath& path, const Value& val, unsigned i) = 0;
	
	virtual bool operator==(const Value& rhs) const = 0;
	virtual bool operator!=(const Value& rhs) const;

	static Value* create(const JsonText& text);
	
private:
	static Value* createValue(unsigned& pos, const JsonText& text);
	static JsonObject* createObject(unsigned& pos, const JsonText& text);
	static JsonArray* createArray(unsigned& pos, const JsonText& text);
	static String* createString(unsigned& pos, const JsonText& text);
	static Number* createNumber(unsigned& pos, const JsonText& text);
	static Bool* createBool(unsigned& pos, const JsonText& text);
	static Null* createNull(unsigned& pos, const JsonText& text);
};

#endif