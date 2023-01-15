#pragma once
#include "../RunTimeLangState.hpp"
#include "..//../LangCore.hpp"
#include "UCodeLang/LangCore/LangTypes.hpp"
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
		constexpr Return_t(RetState Succeed): _Succeed(Succeed), ReturnValue()
		{

		}
		constexpr Return_t(RetState Succeed, Register Ret) : _Succeed(Succeed), ReturnValue(Ret)
		{

		}
	};

	Interpreter() {}
	~Interpreter(){}


	void Init(RunTimeLangState* State)
	{
		_State = State;
		constexpr size_t bufferForHeap = 0xff;
		auto P = Calloc(CPUData::MaxStackSize + bufferForHeap);

		_CPU.InitStack(P);
	}
	void UnLoad()
	{
		Free(_CPU.Stack._Data);
	}

	Return_t Call(const String& FunctionName, parameters Pars = NullParameters);
	Return_t Call(UAddress address,parameters Pars = NullParameters);

	void Extecute(Instruction& Inst);
	
	Return_t ThisCall(UAddress This, const String& FunctionName, parameters Pars = NullParameters);
	Return_t ThisCall(UAddress This, UAddress address, parameters Pars = NullParameters);
	UCodeLangForceinline Return_t ThisCall(PtrType This, UAddress address, parameters Pars = NullParameters)
	{
		return ThisCall((UAddress)This,address,Pars);
	}
	UCodeLangForceinline Return_t ThisCall(PtrType This, const String& FunctionName, parameters Pars = NullParameters)
	{
		return ThisCall((UAddress)This, FunctionName, Pars);
	}
	UCodeLangForceinline Return_t ThisCall(PtrType This, const ClassMethod& Function, parameters Pars = NullParameters)
	{
		return ThisCall((UAddress)This, Function.FullName, Pars);
	}
	//
	UCodeLangForceinline PtrType Calloc(NSize_t Size) { return _State->Calloc(Size); }
	UCodeLangForceinline PtrType Realloc(PtrType OldPtr, NSize_t Size) { return _State->Realloc(OldPtr, Size); }
	UCodeLangForceinline void MemCopy(PtrType destination, const PtrType source, NSize_t Size) { memcpy(destination, source, Size); }
	UCodeLangForceinline PtrType Malloc(NSize_t Size) { return _State->Malloc(Size); }
	UCodeLangForceinline void Free(PtrType Ptr) { return _State->Free(Ptr); }
	UCodeLangForceinline void Log(const char* Ptr) { return _State->Log(Ptr); }

	UCodeLangForceinline const UserMadeContext& Get_UserMadeContext(){return _UserMadeContext;}
	UCodeLangForceinline void Set_UserMadeContext(UserMadeContext Context){_UserMadeContext = Context;}
	UCodeLangForceinline auto Get_State() { return _State; }
	void PushParameters(parameters Pars)
	{
		_CPU.Stack.PushParameters(Pars);
	}
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
			inline void PushParameters(const parameters V)
			{
				PushBytes(V.Data, V.Size);
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
			parameters PopStackParameters(NSize_t DataSize)
			{
				if (StackOffSet - DataSize < 0) { throw std::exception("stack underflow"); }//This May not work

				StackOffSet -= DataSize;
				void* DataPtr = (void*)((UIntNative)_Data + StackOffSet);
				return parameters(DataPtr, DataSize);
			};
			
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


public:	
	
	template<typename T> UCodeLangForceinline T GetParameters()
	{
		constexpr bool IsBigerRegister = sizeof(T) > sizeof(Interpreter::Register);
		if (IsBigerRegister)
		{
			return GetParametersFromStack<T>();
		}
		else
		{
			return *(T*)&Get_InPutRegister().Value;
		}
	}
	template<typename T> UCodeLangForceinline void Set_Return(const T& Value) {

		constexpr bool IsBigerRegister = sizeof(T) > sizeof(Interpreter::Register);
		if (IsBigerRegister) 
		{
			_Ptr->_CPU.Stack.PushBytes(&Value,sizeof(T));
		}
		else
		{
			Get_OutPutRegister().Value = *(UInt64*)&Value;
		}
	}
	UCodeLangForceinline void Set_Return(void){}
	template<typename T> UCodeLangForceinline T* Get_This()
	{
		constexpr bool IsBigerThenRegister =sizeof(T*) > sizeof(Interpreter::Register);
		static_assert(!IsBigerThenRegister, " 'T' is too big to be in a Register");
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

	UCodeLangForceinline parameters GetParametersOnStack()
	{
		UIntNative Size = _Ptr->_CPU.Stack.PopStack<UIntNative>();
		return  _Ptr->_CPU.Stack.PopStackParameters(Size);
	}
	template<typename T> UCodeLangForceinline T GetParametersFromStack()
	{
		return parameters::From<T>(GetParametersOnStack());
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

