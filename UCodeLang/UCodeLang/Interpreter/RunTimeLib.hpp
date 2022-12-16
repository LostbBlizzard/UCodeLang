#pragma once
#include "../LangCore.hpp"
#include "../Compliation/UClib.hpp"
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
	inline UClib* Get_Lib(){return _Lib;}
	inline auto& Get_Instructions() { return _Instruction; }
	inline auto& Get_CPPCalls() { return _NameToCppCall; }

	inline void Add_CPPCall(const String& Name, CPPCallBack CPP)
	{
		_NameToCppCall[Name] = CPP;
	}
	
private:
	UClib* _Lib;
	Vector<Instruction> _Instruction;
	unordered_map<String, CPPCallBack> _NameToCppCall;
};

UCodeLangEnd

