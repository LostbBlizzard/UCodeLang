#pragma once
#include "Typedef.hpp"

#include "IntellisenseData.hpp"
UCodeAnalyzerStart

enum class SyntaxTokenType : UInt8
{
	Null,
	Name,
	KeyWord,
};
struct SyntaxToken
{
	SyntaxTokenType Type = SyntaxTokenType::Null;
	size_t FileStart = 0;
	size_t FileEnd = 0;
};
class SyntaxHighlighter
{
public:
	SyntaxHighlighter(){}
	~SyntaxHighlighter(){}

	void Parse(StringView Text);

	inline auto& Get_Tokens()
	{
		return _Tokens;
	}

	inline void Set_Data(IntellisenseData* Value)
	{
		_Data = Value;
	}
	inline IntellisenseData* Get_Data()
	{
		return _Data;
	}
private:
	Vector<SyntaxToken> _Tokens;
	IntellisenseData* _Data=nullptr;

};
UCodeAnalyzerEnd