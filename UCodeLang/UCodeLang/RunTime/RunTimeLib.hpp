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

	#define GetUinqID __LINE__ 

	//It should be unique for every function pointer you have
	#define Add_CppCall(Lib,FuncPointer,Name)  \
    {\
	Lib.Add_CPPCall<0>(Name, FuncPointer); } \


	template<size_t FuncUniqueID,typename... Pars>
	void Add_CPPCall(const String& Name,void(*CPP)(Pars...) )
	{
		using FuncType = void(*)(Pars...);
		const static FuncType _Cpp = CPP;
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

	template<size_t FuncUniqueID,typename T, typename... Pars>
	void Add_CPPCall(const String& Name, T(*CPP)(Pars...) )
	{
		using FuncType = T(*)(Pars...);
		const static FuncType _Cpp = CPP;
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
	
	template<size_t FuncUniqueID, typename This,typename... Pars>
	void Add_CPPCall(const String& Name, void(This::*CPP)(Pars...))
	{
		using FuncType = void(This::*)(Pars...);
		const static FuncType _Cpp = CPP;
		struct CPPFunc
		{
			static void UCodeLangAPI Invoke(InterpreterCPPinterface& interpreter)
			{
				This* _This = interpreter.GetParameter<This*>();
				Pars... Par;
				interpreter.SetParametersPointer(&Par);
				interpreter.GetParametersIntoPointer<Pars...>();

				(_This->*_Cpp)(Par);

				interpreter.Set_Return();
			}
		};
		Add_CPPCall(Name, CPPFunc::Invoke);
	}

	template<size_t FuncUniqueID, typename R, typename This,typename... Pars>
	void Add_CPPCall(const String& Name, R(This::* CPP)(Pars...))
	{
		using FuncType = R(This::*)(Pars...);
		const static FuncType _Cpp = CPP;
		struct CPPFunc
		{
			static void UCodeLangAPI Invoke(InterpreterCPPinterface& interpreter)
			{
				This* _This = interpreter.GetParameter<This*>();
				Pars... Par;
				interpreter.SetParametersPointer(&Par);
				interpreter.GetParametersIntoPointer<Pars...>();


				interpreter.Set_Return((_This->*_Cpp)(Par));
			}
		};
		Add_CPPCall(Name, CPPFunc::Invoke);
	}
	
	//No Pars
	template<size_t FuncUniqueID>
	void Add_CPPCall(const String& Name, void(*CPP)())
	{
		using FuncType = void(*)();
		const static FuncType _Cpp = CPP;
		struct CPPFunc
		{
			static void UCodeLangAPI Invoke(InterpreterCPPinterface& interpreter)
			{
				_Cpp();
				interpreter.Set_Return();
			}
		};
		Add_CPPCall(Name, CPPFunc::Invoke);
	}

	template<size_t FuncUniqueID, typename T>
	void Add_CPPCall(const String& Name, T(*CPP)())
	{
		using FuncType = T(*)();
		const static FuncType _Cpp = CPP;
		struct CPPFunc
		{
			static void UCodeLangAPI Invoke(InterpreterCPPinterface& interpreter)
			{
				interpreter.Set_Return(_Cpp());
			}
		};
		Add_CPPCall(Name, CPPFunc::Invoke);
	}

	template<size_t FuncUniqueID, typename This>
	void Add_CPPCall(const String& Name, void(This::* CPP)())
	{
		using FuncType = void(This::*)();
		const static FuncType _Cpp = CPP;
		struct CPPFunc
		{
			static void UCodeLangAPI Invoke(InterpreterCPPinterface& interpreter)
			{
				This* _This = interpreter.GetParameter<This*>();
				
				(_This->*_Cpp)();

				interpreter.Set_Return();
			}
		};
		Add_CPPCall(Name, CPPFunc::Invoke);
	}

	template<size_t FuncUniqueID, typename R, typename This>
	void Add_CPPCall(const String& Name, R(This::* CPP)())
	{
		using FuncType = R(This::*)();
		const static FuncType _Cpp = CPP;
		struct CPPFunc
		{
			static void UCodeLangAPI Invoke(InterpreterCPPinterface& interpreter)
			{
				This* _This = interpreter.GetParameter<This*>();
				

				interpreter.Set_Return((_This->*_Cpp)());
			}
		};
		Add_CPPCall(Name, CPPFunc::Invoke);
	}

private:
	UClib* _Lib;
	Vector<Instruction> _Instruction;
	unordered_map<String, CPPCallBack> _NameToCppCall;
};

UCodeLangEnd

