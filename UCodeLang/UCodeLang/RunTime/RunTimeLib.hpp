#pragma once
#include "../LangCore.hpp"
#include "UCodeLang/LangCore/UClib.hpp"
UCodeLangStart

#define UCodeLangAPI __cdecl 

#define CPPCallPars(Pars) InterpreterCPPinterface& Input
#define CPPCallParsNone InterpreterCPPinterface& Input

#define SetCPPRet(Value)  Input.Set_Return(Value); 
#define SetCPPRetNone 


#define GetCPPCallName(x) CPP_Invoke##x
#define MakeNewCPPCall(FuncName,Ret_t,...) CPPCallRet UCodeLangAPI GetCPPCallName(FuncName)(CPPCallPars(Pars)) \
	{\
		##__VA_ARGS__ V;\
		Input.GetParameter<##__VA_ARGS__>(&V);\
		auto r = FuncName(V); \
		SetCPPRet(r);\
	};\

#define MakeNewCPPCall_void(FuncName,...) CPPCallRet UCodeLangAPI GetCPPCallName(FuncName)(CPPCallPars(Pars)) \
	{\
		##__VA_ARGS__ V;\
		Input.GetParameter<##__VA_ARGS__>(&V);\
		FuncName(V); \
		SetCPPRetNone \
	};\

#define MakeNewCPPCall_voidNoPar(FuncName) CPPCallRet UCodeLangAPI GetCPPCallName(FuncName)(CPPCallPars(Pars)) \
	{\
		FuncName(); \
		SetCPPRetNone \
	};\

#define MakeNewCPPCall_NoPar(FuncName) CPPCallRet UCodeLangAPI GetCPPCallName(FuncName)(CPPCallPars(Pars)) \
	{\
		auto r = FuncName(); \
		SetCPPRet(r);\
	};\

class InterpreterCPPinterface;

using CPPCallRet = void;

class RunTimeLib
{
public:

	typedef CPPCallRet(UCodeLangAPI*CPPCallBack)(InterpreterCPPinterface& Input);

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
	Unordered_map<String, CPPCallBack> _NameToCppCall;
};

UCodeLangEnd

