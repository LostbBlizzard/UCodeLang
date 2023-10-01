#pragma once
#include "../LangCore.hpp"
#include "UCodeLang/LangCore/UClib.hpp"
#include "UCodeLang/LangCore/DataType/UnorderedMap.hpp"
UCodeLangStart

class InterpreterCPPinterface;
class RunTimeLib
{
public:
	using CPPCallBack = void(UCodeLangAPI*)(InterpreterCPPinterface& Input);
	template<typename T, typename... Pars>
	using NativeCall = T(UCodeLangAPI*)(Pars...);
	struct CPPCall
	{
		CPPCallBack InterpreterCall = nullptr;
		void* NativeCall = nullptr;
	};

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
		_NameToCppCall[Name] = { CPP,nullptr };
	}
	


	
	template<typename T,typename... Pars>
	UCodeLangForceinline void Add_CPPCall(const String& Name,CPPCallBack CPP, NativeCall<T,Pars...> Native)
	{
		_NameToCppCall[Name] = { CPP,(void*)Native };
	}
	
private:
	UClib* _Lib;
	Vector<Instruction> _Instruction;
	
	
	UnorderedMap<String, CPPCall> _NameToCppCall;
};

UCodeLangEnd

