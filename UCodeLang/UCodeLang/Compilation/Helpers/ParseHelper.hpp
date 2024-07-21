#pragma once
#ifndef UCodeLangNoCompiler

#include "UCodeLang/LangCore/LangTypes.hpp"
#include "UCodeLang/LangCore/UCodeLangNameSpace.hpp"
#include <sstream>
UCodeLangStart
class ParseHelper
{
public:
#define ParseHelpertoIntSet(x) \
	static bool ParseStringToInt##x(String_view string, Int##x& out) \
	{ \
		String tep = String(string); \
		std::istringstream iss(tep); \
		iss >> out; \
		return !iss.fail() && iss.eof(); \
	} \
		static bool ParseStringToUInt##x(String_view string, UInt##x& out) \
	{ \
		String tep = String(string); \
		std::istringstream iss(tep); \
		iss >> out; \
		return !iss.fail() && iss.eof(); \
	} \

	static bool ParseStringToInt8(String_view string, Int8& out) 
	{ 
		int val = 0;
		if (!ParseStringToInt32(string,val))
		{
			return false;
		}

		if (val < std::numeric_limits<Int8>::min() || val > std::numeric_limits<Int8>::max())
		{
			return false;
		}
		out = val;
		return true;
	} 
	static bool ParseStringToUInt8(String_view string, UInt8& out) 
	{ 
		int val = 0;
		if (!ParseStringToInt32(string,val))
		{
			return false;
		}

		if (val < std::numeric_limits<UInt8>::min() || val > std::numeric_limits<UInt8>::max())
		{
			return false;
		}
		out = val;
		return true;
	} 
	ParseHelpertoIntSet(16);
	ParseHelpertoIntSet(32);
	ParseHelpertoIntSet(64);

	static bool ParseStringTofloat32(String_view string, float32& out)
	{
		out = std::stof((String)string);
		return true;
	}
	static bool ParseStringTofloat64(String_view string, float64& out)
	{
		out = std::stod((String)string);
		return true;
	}

	static bool ParseCharliteralToChar(String_view string, char& out);
	static bool ParseCharliteralToChar(String_view string, Utf8& out);
	static bool ParseCharliteralToChar(String_view string, Utf16& out);
	static bool ParseCharliteralToChar(String_view string, Utf32& out);

	//if ret is 0 then good else bad and the Index-1 of the bad char is  
	static size_t ParseStringliteralToString(String_view string, String& out);
	//if ret is 0 then good else bad and the Index-1 of the bad char is  
	static size_t ParseStringliteralToString(String_view string, String8& out);
	//if ret is 0 then good else bad and the Index-1 of the bad char is  
	static size_t ParseStringliteralToString(String_view string, String16& out);
	//if ret is 0 then good else bad and the Index-1 of the bad char is  
	static size_t ParseStringliteralToString(String_view string, String32& out);

	//if ret is 0 then bad else good and its how much to move by
	static size_t ParseCharliteralToChar(String_view string, String& out);
	//if ret is 0 then bad else good and its how much to move by
	static size_t ParseCharliteralToChar(String_view string, String8& out);
	//if ret is 0 then bad else good and its how much to move by
	static size_t ParseCharliteralToChar(String_view string, String16& out);
	//if ret is 0 then bad else good and its how much to move by
	static size_t ParseCharliteralToChar(String_view string, String32& out);
};
UCodeLangEnd

#endif
