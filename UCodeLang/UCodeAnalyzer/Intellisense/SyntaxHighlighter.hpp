#pragma once
#include "../Typedef.hpp"

#include "IntellisenseData.hpp"
UCodeAnalyzerStart
class SyntaxHighlighter
{

public:
	enum class TokenType : UInt8
	{
		Name,
		KeyWord,
	};
	struct Token
	{
		TokenType Type;
		size_t Start =0;
		size_t End = 0;
	};
	
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
	Vector<Token> _Tokens;
	IntellisenseData* _Data=nullptr;

};
UCodeAnalyzerEnd