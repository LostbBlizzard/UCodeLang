#include "SyntaxHelper.hpp"

#include "UCodeLang/Compliation/Front/Lexer.hpp"
#include "UCodeLang/Compliation/Helpers/KeyWords.hpp"
UCodeAnalyzerStart

Vector<String> SyntaxHelper::GetControlFlowKeywords()
{
	using namespace UCodeLang::FrontEnd;
	Vector<String> r = { S_KeyWord_if, S_KeyWord_else ,S_KeyWord_while , S_KeyWord_for, S_KeyWord_do,S_KeyWord_ret,S_KeyWord_await,S_KeyWord_yield ,S_KeyWord_continue,S_KeyWord_break };
	return r;
}
Vector<String> SyntaxHelper::GetTypeKeywords()
{
	using namespace UCodeLang::FrontEnd;
	Vector<String> r = 
	{ S_KeyWord_bool , S_KeyWord_char,S_KeyWord_utf8 ,S_KeyWord_utf16 , S_KeyWord_utf32 , S_KeyWord_utf64,
	S_KeyWord_Int,S_KeyWord_Int8,S_KeyWord_Int16,S_KeyWord_Int32,S_KeyWord_Int64 
	,S_KeyWord_uInt8,S_KeyWord_uInt16,S_KeyWord_uInt32,S_KeyWord_uInt64, S_KeyWord_byte , S_KeyWord_sbyte
	,S_KeyWord_float,S_KeyWord_float32,S_KeyWord_float64,S_KeyWord_uintptr,S_KeyWord_sintptr,S_KeyWord_void
	,S_KeyWord_async,S_KeyWord_dynamic, S_KeyWord_var };
	return r;
}
Vector<String> SyntaxHelper::GetOtherKeywords()
{
	Vector<String> r = { "if","else","for","while","break",
		"continue","ret","public","private","unsafe","new","unq","move","this"};
	return r;
}
Vector<String> SyntaxHelper::GetKeyWords()
{
	Vector<String> r;
	for (auto& Item : UCodeLang::FrontEnd::KeyWords)
	{
		r.push_back(Item.keyWord);
	}
	return r;
}
String SyntaxHelper::TmLanguageString()
{
	String r;

	r += R"({
	"$schema": "https://raw.githubusercontent.com/martinring/tmlanguage/master/tmlanguage.json",
	"name": "UCodeLang",
	"patterns": [
		{
			"include": "#keywords"
		},
		{
			"include": "#strings"
		}
	],
	"repository": {
		"keywords": {
			"patterns": [{
				"name": "keyword.control.uclang",
				"match": "\\b(if|while|for|return|import|true|false|bool|int|int8|int16|int32|int64|intptr|uint8|uint16|uint32|uint64|uintptr|char|utf8|utf16|utf32|float|float32|float64,void)\\b"
			}]
		},
		"strings": {
			"name": "string.quoted.double.uclang",
			"begin": "\"",
			"end": "\"",
			"patterns": [
				{
					"name": "constant.character.escape.uclang",
					"match": "\\\\."
				}
			]
		}
	},
	"scopeName": "source.uclang"
})";

	return r;
}
UCodeAnalyzerEnd