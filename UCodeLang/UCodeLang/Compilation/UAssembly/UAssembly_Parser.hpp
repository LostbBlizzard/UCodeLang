#pragma once

#include "UCodeLang/LangCore/UClib.hpp"
#include "UAssembly_NameSpace.hpp"
#include "UCodeLang/Compilation/LexerDefs.h"
#include "CompilerTypes.hpp"
#include "../Helpers/CompilationErrors.hpp"
#include "../CompilationSettings.hpp"
UAssemblyStart


class Parser
{
public:
	Parser() {}
	~Parser() {}
	void Reset();
	UCodeLangForceinline void Set_ErrorsOutput(CompilationErrors* V) { _ErrorsOutput = V; }
	UCodeLangForceinline void Set_Settings(CompilationSettings* V) { _Settings = V; }

	void Parse(const Vector<Token>& Tokens, UClib* Output);
	UCodeLangForceinline bool Get_ParseSucces() { return _ParseSuccess; }
private:
	bool _ParseSuccess = false;
	CompilationErrors* _ErrorsOutput = nullptr;
	CompilationSettings* _Settings = nullptr;
	UClib* _Output = nullptr;
	CodeLayer::UCodeByteCode* _OutLayer = nullptr;
	const Vector<Token>* _Nodes = nullptr;
	size_t _TokenIndex = 0;
	Instruction _TepIns;

	
	inline const Token* TryPeekNextToken(size_t offset)
	{
		size_t Index = _TokenIndex + offset;
		if (Index < _Nodes->size())
		{
			return &_Nodes->operator[](Index);
		}
		else
		{
			return nullptr;
		}
	}
	UCodeLangForceinline const  Token* TryPeekNextToken() { return   TryPeekNextToken(1); }
	UCodeLangForceinline const  Token* TryGetToken() { return TryPeekNextToken(0); }
	UCodeLangForceinline void NextToken() { _TokenIndex++; }
	UCodeLangForceinline void NextToken(size_t offfset) { _TokenIndex += offfset; }

	void ParseIns();
	void ParseOp(AnyInt64& Out, OpCodeType Type);
	void ParseOpRegister(AnyInt64& Out);
	void ParseAny8(AnyInt64& Out);
};
UAssemblyEnd
