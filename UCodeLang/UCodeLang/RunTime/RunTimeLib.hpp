#pragma once
#include "../LangCore.hpp"
#include "UCodeLang/LangCore/UClib.hpp"
UCodeLangStart

#define UCodeLangAPI __cdecl 
class InterpreterCPPinterface;

using CPPCallRet = UInt64;

class RunTimeLib
{
public:

	typedef CPPCallRet(UCodeLangAPI*CPPCallBack)();

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

	template<typename... T>
	using CppCallBack_t = CPPCallRet(UCodeLangAPI*)(T...);

	template<typename... T>
	using CppCallBackv_t = void(UCodeLangAPI*)(T...);

	template<typename... T>
	UCodeLangForceinline void Add_CPPCall(const String& Name, CppCallBack_t<T...> CPP)
	{
		Add_CPPCall(Name, (CPPCallBack)CPP);
	}

	template<typename... T>
	UCodeLangForceinline void Add_CPPCall(const String& Name, CppCallBackv_t<T...> CPP)
	{
		Add_CPPCall(Name, (CPPCallBack)CPP);
	}

private:
	UClib* _Lib;
	Vector<Instruction> _Instruction;
	Unordered_map<String, CPPCallBack> _NameToCppCall;
};

UCodeLangEnd

