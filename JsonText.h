#ifndef JSONText_HEADER_INCLUDED
#define JSONText_HEADER_INCLUDED

#include "JsonPath.h"

class JsonText
{
	friend class Value;

	char* text;
public:
	JsonText(const char* text);
	JsonText(const JsonText& rhs);
	JsonText& operator=(const JsonText& rhs);
	~JsonText();

	const char* get() const;
	
private:
	char operator[](unsigned pos) const;

	bool isValidValue(unsigned& pos, JsonPath& path, bool mustEnd) const;
	bool isValidObject(unsigned& pos, JsonPath& path) const;
	bool isValidArray(unsigned& pos, JsonPath& path) const;
	bool isValidString(unsigned& pos, JsonPath& path) const;
	bool isValidNumber(unsigned& pos, JsonPath& path) const;
	bool isValidBool(unsigned& pos, JsonPath& path) const;
	bool isValidNull(unsigned& pos, JsonPath& path) const;

	void getKey(char* dest, const char* src) const;
	void goToKey(unsigned& pos) const;
	void ignoreSpace(unsigned& pos) const;
	void jumpOver(unsigned& pos) const;
	void moveToValue(unsigned& pos) const;
	void extractString(char* dest, unsigned& pos) const;
	
private:
	void copy(const JsonText& rhs);
	void clear();
};

#endif
