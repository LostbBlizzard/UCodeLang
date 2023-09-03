#pragma once
#include "UCodeLang/LangCore/LangTypes.hpp"
#include "UCodeLang/LangCore/UCodeLangNameSpace.hpp"
UCodeLangStart
class ParseHelper
{
public:
	#define ParseHelpertoIntSet(x) \
	static bool ParseStringToInt##x(String_view string, Int##x& out) \
	{ \
		out = std::stoi((String)string); \
		return true; \
	} \
		static bool ParseStringToUInt##x(String_view string, UInt##x& out) \
	{ \
		out = std::stoi((String)string); \
		return true; \
	} \

	ParseHelpertoIntSet(8);
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

