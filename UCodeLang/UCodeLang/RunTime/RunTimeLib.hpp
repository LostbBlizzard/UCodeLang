#pragma once
#include "../LangCore.hpp"
#include "UCodeLang/LangCore/UClib.hpp"
UCodeLangStart

class InterpreterCPPinterface;
class RunTimeLib
{
public:
	typedef void (*CPPCallBack)(InterpreterCPPinterface& interpreter);

	RunTimeLib(): _Lib(nullptr)
	{
	}
	~RunTimeLib()
	{
	}

	RunTimeLib(const RunTimeLib& V) = delete;
	RunTimeLib& operator=(const RunTimeLib& V) = delete;
	void Init(UClib* Lib);
	void UnLoad();
	UCodeLangForceinline UClib* Get_Lib(){return _Lib;}
	UCodeLangForceinline auto& Get_Instructions() { return _Instruction; }
	UCodeLangForceinline auto& Get_CPPCalls() { return _NameToCppCall; }

	UCodeLangForceinline void Add_CPPCall(const String& Name, CPPCallBack CPP)
	{
		_NameToCppCall[Name] = CPP;
	}
	
private:
	UClib* _Lib;
	Vector<Instruction> _Instruction;
	unordered_map<String, CPPCallBack> _NameToCppCall;
};

UCodeLangEnd

