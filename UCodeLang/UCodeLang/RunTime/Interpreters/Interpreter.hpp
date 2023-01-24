#pragma once
#include "../RunTimeLangState.hpp"
#include "..//../LangCore.hpp"
#include "UCodeLang/LangCore/LangTypes.hpp"
#include "ParameterPassingHelper.hpp"
UCodeLangStart


class InterpreterCPPinterface;
class Jit_Interpreter;

class Interpreter
{
public:
	friend InterpreterCPPinterface;
	friend Jit_Interpreter;

	enum class RetState : ExitState_t
	{
		Null,
		Success,
		Error,
		Error_Function_doesnt_exist,
	};
	struct Register
	{
		AnyInt64 Value;
		constexpr  Register() :Value()
		{

		}
		constexpr  Register(AnyInt64 V) : Value(V)
		{

		}
	};

	struct Return_t
	{
		RetState _Succeed;
		Register ReturnValue;

		constexpr Return_t() : _Succeed(RetState::Null), ReturnValue()
		{

		}
		constexpr Return_t(RetState Succeed) : _Succeed(Succeed), ReturnValue()
		{

		}
		constexpr Return_t(RetState Succeed, Register Ret) : _Succeed(Succeed), ReturnValue(Ret)
		{

		}
	};

	Interpreter() {}
	~Interpreter() { UnLoad(); }


	void Init(RunTimeLangState* State)
	{
		_State = State;
		constexpr size_t bufferForHeap = 0xff;
		auto P = Calloc(CPUData::MaxStackSize + bufferForHeap);

		_CPU.InitStack(P);
	}
	void UnLoad()
	{
		if (_CPU.Stack._Data) {
			Free(_CPU.Stack._Data);
			_CPU.Stack._Data = nullptr;
		}
	}

	Return_t Call(const String& FunctionName);
	Return_t Call(UAddress address);

	void Extecute(Instruction& Inst);

	Return_t ThisCall(UAddress This, const String& FunctionName)
	{
		if (CheckIfFunctionExist(FunctionName))
		{
			PushParameter(This);
			return Call(FunctionName);
		}
		return  Return_t(RetState::Error_Function_doesnt_exist);
	}
	Return_t ThisCall(UAddress This, UAddress address)
	{
		PushParameter(This);
		return Call(address);
	}
	UCodeLangForceinline Return_t ThisCall(PtrType This, UAddress address)
	{
		return ThisCall((UAddress)This, address);
	}
	UCodeLangForceinline Return_t ThisCall(PtrType This, const String& FunctionName)
	{
		return ThisCall((UAddress)This, FunctionName);
	}
	UCodeLangForceinline Return_t ThisCall(PtrType This, const ClassMethod& Function)
	{
		return ThisCall((UAddress)This, Function.FullName);
	}



	template<typename... Args> Return_t ThisCall(UAddress This, const String& FunctionName, Args&&... parameters)
	{
		if (CheckIfFunctionExist(FunctionName))
		{
			PushParameter(This);
			PushParameters(parameters...);
			return Call(FunctionName);
		}
		return Return_t(RetState::Error_Function_doesnt_exist);
	}
	template<typename... Args> Return_t ThisCall(UAddress This, UAddress address, Args&&... parameters)
	{
		PushParameter(This);
		PushParameters(parameters...);
		return Call(address);
	}
	template<typename... Args> UCodeLangForceinline Return_t ThisCall(PtrType This, UAddress address, Args&&... parameters)
	{
		return ThisCall((UAddress)This, address, parameters);
	}
	template<typename... Args>UCodeLangForceinline Return_t ThisCall(PtrType This, const String& FunctionName, Args&&... parameters)
	{
		return ThisCall((UAddress)This, FunctionName, parameters);
	}
	template<typename... Args> UCodeLangForceinline Return_t ThisCall(PtrType This, const ClassMethod& Function, Args&&... parameters)
	{
		return ThisCall((UAddress)This, Function.FullName, parameters);
	}



	template<typename T, typename... Args>
	T retCall(const String& FunctionName, Args&&... parameters)
	{
		if (CheckIfFunctionExist(FunctionName))
		{
			PushParameters(parameters...);

			auto V = Call(FunctionName);
			if (V._Succeed == RetState::Success)
			{
				return Get_Return<T>();
			}
		}
		return {};
	}
	template<typename T,typename... Args>
	T retThisCall(PtrType This, const ClassMethod& Function, Args&&... parameters)
	{
		return retThisCall(This, Function.FullName, Args&&... parameters)
	}
	template<typename T, typename... Args> T retThisCall(PtrType This, const String& Function, Args&&... parameters)
	{
		if (CheckIfFunctionExist(FunctionName))
		{
			auto V = ThisCall(This, Function, parameters);
			if (V._Succeed == RetState::Success)
			{
				return Get_Return<T>();
			}
		}
		return {};
	}
	//

	template<typename... Args> void PushParameters(Args&&... parameters)
	{
		(
		
		 PushParameter(parameters)
		, ...);
	}

	template<typename T> UCodeLangForceinline void PushParameter(const T& Value)
	{
		PushParameter((const void*)&Value, sizeof(Value));
	}
	void PushParameter(const void* Value, size_t ValueSize)
	{
		_Parameters.Push(Value, ValueSize);
	}


	//
	UCodeLangForceinline PtrType Calloc(NSize_t Size) { return _State->Calloc(Size); }
	UCodeLangForceinline PtrType Realloc(PtrType OldPtr, NSize_t Size) { return _State->Realloc(OldPtr, Size); }
	UCodeLangForceinline void MemCopy(PtrType destination, const PtrType source, NSize_t Size) { memcpy(destination, source, Size); }
	UCodeLangForceinline PtrType Malloc(NSize_t Size) { return _State->Malloc(Size); }
	UCodeLangForceinline void Free(PtrType Ptr) { return _State->Free(Ptr); }
	UCodeLangForceinline void Log(const char* Ptr) { return _State->Log(Ptr); }

	UCodeLangForceinline const UserMadeContext& Get_UserMadeContext() { return _UserMadeContext; }
	UCodeLangForceinline void Set_UserMadeContext(UserMadeContext Context) { _UserMadeContext = Context; }
	UCodeLangForceinline auto Get_State() { return _State; }
	bool CheckIfFunctionExist(const String& FunctionName);

	template<typename T> T Get_Return()
	{
		T r;
		Get_Return(&r, sizeof(T));
		return r;
	}
	void Get_Return(void* Output, size_t OutputSize);
private:
	
	struct CPUReturn_t
	{
		ExitState _Succeed;
		Register ReturnValue;
		constexpr CPUReturn_t() : _Succeed(ExitState::Null), ReturnValue()
		{

		}
		constexpr CPUReturn_t(ExitState Succeed) : _Succeed(Succeed), ReturnValue()
		{

		}
		constexpr CPUReturn_t(ExitState Succeed, Register Ret) : _Succeed(Succeed), ReturnValue(Ret)
		{

		}

	};
	
	struct CPUData
	{
		UAddress ProgramCounter;
		CPUReturn_t RetValue;
		Register A,B,C,D,E,F;
		//
		static constexpr UIntNative MaxStackSize = 0xff * 4;
		struct Stack
		{
			PtrType _Data;
			UIntNative StackOffSet;
			Stack() :_Data(nullptr), StackOffSet(0)
			{

			}
			Stack(PtrType V) :_Data(V), StackOffSet(0)
			{

			}//Dont use String yet
		
			template<typename T> void PushStack(const T& V)
			{
				void* DataPtr = (void*)((UIntNative)_Data + StackOffSet);
				T* SetData = (T*)DataPtr;
				*SetData = V;

				if (StackOffSet + sizeof(T) > MaxStackSize){throw std::exception("stack overflow");}

				StackOffSet += sizeof(T);
			}
			void PushBytes(const void* Ptr,NSize_t Size)
			{
				void* DataPtr = (void*)((UIntNative)_Data + StackOffSet);
				UInt8* StackBytes = (UInt8*)DataPtr;
				UInt8* Bytes = (UInt8*)Ptr;

				if (StackOffSet + Size > MaxStackSize){throw std::exception("stack overflow");}

				for (size_t i = 0; i < Size; i++)
				{
					StackBytes[i] = Bytes[i];
				}

				StackOffSet += Size;
			}
			
			
			template<typename T> T PopStack()
			{
				if (StackOffSet - sizeof(T) < 0) { throw std::exception("stack underflow"); }//This May not work

				StackOffSet -= sizeof(T);
				void* DataPtr = (void*)((UIntNative)_Data + StackOffSet);
				return *(T*)DataPtr;
			}
			
			
			template<typename T> void SetValue(const T& V,NSize_t offset)
			{
				if (StackOffSet + offset + sizeof(T) < 0) { throw std::exception("stack overflow"); }

				void* DataPtr = (void*)((UIntNative)_Data + StackOffSet + offset);

				*(T*)DataPtr = V;
			}
			template<typename T> T GetValue(NSize_t offset)
			{
				if (StackOffSet + offset + sizeof(T) < 0) { throw std::exception("stack overflow"); }

				void* DataPtr = (void*)((UIntNative)_Data + StackOffSet + offset);

				return *(T*)DataPtr;
			}
			template<typename T> T GetValueSub(NSize_t offset)
			{
				if (StackOffSet + offset - sizeof(T) < 0) { throw std::exception("stack overflow"); }

				void* DataPtr = (void*)((UIntNative)_Data + StackOffSet - offset);

				return *(T*)DataPtr;
			}

			void IncrementStack(NSize_t offset)
			{
				StackOffSet += offset;
			}
			void DecrementStack(NSize_t offset)
			{
				StackOffSet -= offset;
			}

			PtrType GetTopOfStack()
			{
				void* DataPtr = (void*)((UIntNative)_Data + StackOffSet);
				return  DataPtr;
			}
			PtrType GetTopOfStackWithoffset(NSize_t offset)
			{
				void* DataPtr = (void*)((UIntNative)_Data + StackOffSet + offset);
				return  DataPtr;
			}
			PtrType GetTopOfStackWithoffsetSub(NSize_t offset)
			{
				void* DataPtr = (void*)((UIntNative)_Data + StackOffSet - offset);
				return  DataPtr;
			}

			PtrType GetFloorOfStack()
			{
				void* DataPtr = (void*)((UIntNative)_Data);
				return  DataPtr;
			}
		};
		Stack Stack;
		
		//
		
		
		CPUData(): ProgramCounter(NullAddress), Stack(), RetValue()
		{

		}
		inline void InitStack(void* StackValue)
		{
			Stack._Data = StackValue;
		}
		
	};
	CPUData _CPU;
	RunTimeLangState* _State = nullptr;
	InterpreterCPPinterface* _CPPHelper = nullptr;
	UserMadeContext _UserMadeContext;
	ParameterPassingHelper _Parameters;
	void FlushParametersIntoCPU();

	UCodeLangForceinline PtrType Get_StaticMemPtr(){return _State->Get_StaticMemPtr();}
	UCodeLangConstexprForceinline Register& Get_ThisRegister() { return Get_Register(RegisterID::ThisRegister); }
	UCodeLangConstexprForceinline Register& Get_OutRegister() { return Get_Register(RegisterID::OuPutRegister); }
	UCodeLangConstexprForceinline Register& Get_InRegister() { return Get_Register(RegisterID::InPutRegister); }
	
	UCodeLangConstexprForceinline Register& Get_MathOutRegister() { return Get_Register(RegisterID::MathOuPutRegister); }
	UCodeLangConstexprForceinline Register& Get_BoolRegister() { return Get_Register(RegisterID::BoolRegister); }
	UCodeLangConstexprForceinline Register& Get_bitwiseRegister() { return Get_Register(RegisterID::BitwiseRegister); }
	UCodeLangConstexprForceinline Register& Get_Register(RegisterID Id)
	{
		switch (Id)
		{
		case UCodeLang::RegisterID::A:return _CPU.A;
		case UCodeLang::RegisterID::B:return _CPU.B;
		case UCodeLang::RegisterID::C:return _CPU.B;
		case UCodeLang::RegisterID::D:return _CPU.D;
		case UCodeLang::RegisterID::E:return _CPU.E;
		case UCodeLang::RegisterID::F:return _CPU.F;
		default:return _CPU.D;
		}
	}
};

class InterpreterCPPinterface
{
	friend Interpreter;

	inline static thread_local void* GetParametersPointer = nullptr;
	inline static thread_local RegisterID ParValue = RegisterID::StartParameterRegister;
	inline static thread_local size_t ParStackOffset = 0;

	void ParInfoReset()
	{
		ParValue = RegisterID::StartParameterRegister;
		ParStackOffset = 0;
	}
public:	
	void SetParametersPointer(void* V)
	{
		ParInfoReset();
		GetParametersPointer = V;
	}

	template<typename... Args> void GetParameters(Args&&... Out)
	{
		throw std::exception("");
		//The Compiler does not like me.
		
		//uncomment this and remove the exception if the Compiler let you Compile it.
		// else you have use GetParameter manually.
		// 
		// 
		//(//GetParameter(Out)
			//,...);
	}


	template<typename T> void GetParameter(T& Out)
	{
		Out = GetParameter<T>();
	}
	template<typename T> T GetParameter()
	{
		constexpr bool IsBigerRegister = sizeof(T) > sizeof(Interpreter::Register);
		if (IsBigerRegister)
		{
			ParStackOffset += sizeof(T);
			auto r = *(T*)_Ptr->_CPU.Stack.GetTopOfStackWithoffset(ParStackOffset);

			return r;
		}
		else
		{
			auto r = *(T*)(&_Ptr->Get_Register(ParValue).Value);

			(*(RegisterID_t*)&ParValue)++;
			return r;
		}
	}

	

	template<typename T> void Set_Return(const T& Value) 
	{

		constexpr bool IsBigerRegister = sizeof(T) > sizeof(Interpreter::Register);
		if (IsBigerRegister) 
		{
			_Ptr->_CPU.Stack.SetValue(Value,sizeof(T));
			Get_OutPutRegister().Value = _Ptr->_CPU.Stack.GetTopOfStack();
		}
		else
		{
			Get_OutPutRegister().Value = *(UInt64*)&Value;
		}
	}
	void Set_Return(void){}

	template<typename T> UCodeLangForceinline T* Get_This()
	{
		return (T*)&Get_ThisRegister().Value;
	}

	UCodeLangForceinline const UserMadeContext& Get_UserMadeContext()
	{
		return _Ptr->Get_UserMadeContext();
	}
	UCodeLangForceinline const RunTimeLangState* Get_State()
	{
		return _Ptr->_State;
	}

	UCodeLangForceinline auto& Get_InPutRegister() { return _Ptr->Get_InRegister(); }
	UCodeLangForceinline auto& Get_OutPutRegister() { return _Ptr->Get_OutRegister(); }
	UCodeLangForceinline auto& Get_ThisRegister() { return _Ptr->Get_ThisRegister(); }
	
//
	UCodeLangForceinline PtrType Malloc(NSize_t Size) { return _Ptr->Malloc(Size); }
	UCodeLangForceinline void Free(PtrType Ptr) { return _Ptr->Free(Ptr); }
	UCodeLangForceinline PtrType Realloc(PtrType Ptr,NSize_t Size) { return _Ptr->Realloc(Ptr,Size); }
	UCodeLangForceinline PtrType Calloc(NSize_t Size) { return _Ptr->Calloc(Size); }
	
	InterpreterCPPinterface(Interpreter* Ptr) : _Ptr(Ptr)
	{

	}
private:
	

	Interpreter* _Ptr = nullptr;
};
UCodeLangEnd

