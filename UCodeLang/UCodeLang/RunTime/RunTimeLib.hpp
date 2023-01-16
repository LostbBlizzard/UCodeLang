#pragma once
#include "../LangCore.hpp"
#include "UCodeLang/LangCore/UClib.hpp"
UCodeLangStart

#define UCodeLangAPI __stdcall
class InterpreterCPPinterface;
class RunTimeLib
{
public:

	typedef void (UCodeLangAPI*CPPCallBack)(InterpreterCPPinterface& interpreter);

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
	


	template<size_t V,typename... Pars>
	void Add_CPPCall(const String& Name,void(*CPP)(Pars...) )
	{
		using FuncType = void(*)(Pars...);
		const static thread_local FuncType   _Cpp = CPP;
		struct CPPFunc
		{
			static void UCodeLangAPI Invoke(InterpreterCPPinterface& interpreter)
			{
				Pars... Par;
				interpreter.GetParameters(Par);
				_Cpp(Par);
				interpreter.Set_Return();
			}
		};
		Add_CPPCall(Name, CPPFunc::Invoke);
	}

	template<size_t V,typename T, typename... Pars>
	void Add_CPPCall(const String& Name, T(*CPP)(Pars...) )
	{
		using FuncType = T(*)(Pars...);
		const static thread_local FuncType   _Cpp = CPP;
		struct CPPFunc
		{
			static void UCodeLangAPI Invoke(InterpreterCPPinterface& interpreter)
			{
				Pars... Par;
				interpreter.GetParameters(Par);
				interpreter.Set_Return(_Cpp(Par));
			}
		};
		Add_CPPCall(Name, CPPFunc::Invoke);
	}


	/*
	template<typename This,typename... Pars>
	void Add_CPPCall(const String& Name, void(This::*CPP)(Pars...))
	{
		using FuncType = void(This::*)(Pars...);
		const static thread_local FuncType   _Cpp = CPP;
		struct CPPFunc
		{
			static void UCodeLangAPI Invoke(InterpreterCPPinterface& interpreter)
			{
				This* _This = interpreter.GetParameter<This*>();
				Pars... Par;
				interpreter.GetParameters(std::forward<Pars>(Par)...);

				(_This->*_Cpp)(Par);

				interpreter.Set_Return();
			}
		};
		Add_CPPCall(Name, CPPFunc::Invoke);
	}

	template<typename R, typename This,typename... Pars> 
	void Add_CPPCall(const String& Name, R(This::* CPP)(Pars...))
	{
		using FuncType = R(This::*)(Pars...);
		//const static thread_local FuncType _Cpp = CPP;
		struct CPPFunc
		{
			static void UCodeLangAPI Invoke(InterpreterCPPinterface& interpreter)
			{
				This* _This = interpreter.GetParameter<This*>();
				Pars... Par;
				 interpreter.GetParameters(Par);

				//interpreter.Set_Return((_This->*_Cpp)(Par));
			}
		};
		Add_CPPCall(Name, CPPFunc::Invoke);
	}
	*/

private:
	UClib* _Lib;
	Vector<Instruction> _Instruction;
	unordered_map<String, CPPCallBack> _NameToCppCall;
};

UCodeLangEnd

