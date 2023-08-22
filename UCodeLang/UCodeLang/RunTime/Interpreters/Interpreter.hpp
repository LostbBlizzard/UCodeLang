#pragma once
#include "../RunTimeLangState.hpp"
#include "..//../LangCore.hpp"
#include "UCodeLang/LangCore/LangTypes.hpp"
#include "ParameterPassingHelper.hpp"
UCodeLangStart


class InterpreterCPPinterface;
class Jit_Interpreter;
class UPackagedTask_t;
class ProfilerDebuger;
class Interpreter
{
public:
	friend InterpreterCPPinterface;
	friend Jit_Interpreter;
	friend UPackagedTask_t;
	friend ProfilerDebuger;
	friend RunTimeLangState;

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
	~Interpreter() 
	{ 
		#ifdef UCodeLangDebug
		InDestruct = true;
		#endif
		UnLoad(); 
	}


	void Init(RunTimeLangState* State)
	{
		#if UCodeLangDebug
		WasInit = true;
		#endif

		_State = State;
		constexpr size_t bufferForHeap = 0xff;
		auto P = Calloc(CPUData::MaxStackSize + bufferForHeap);

		_CPU.InitStack(P);

		const auto& ThreadBytes = State->Get_Libs().GetThreadBytes();
		_CPU.ThreadRegister = Malloc(ThreadBytes.size());
		MemCopy(_CPU.ThreadRegister,(PtrType)ThreadBytes.data(), ThreadBytes.size());
	}
	void UnLoad()
	{
		#if UCodeLangDebug
		if (!InDestruct) 
		{
			UCodeLangAssert(WasInit == true);
		}
		GotRetValue = false;
		CalledFuncBefor = false;
		#endif
		if (_CPU.Stack._Data) 
		{
			Free(_CPU.Stack._Data);
			_CPU.Stack._Data = nullptr;
		}
		if (_CPU.ThreadRegister)
		{
			Free(_CPU.ThreadRegister);
			_CPU.ThreadRegister = nullptr;
		}
	}

	Return_t Call(const String& FunctionName);
	Return_t Call(UAddress address);
	Return_t Call(const ClassMethod* Function)
	{
		return Call(Function->DecorationName);
	}

	void Extecute(Instruction& Inst);

	

	template<typename... Args> Return_t ThisCall(UAddress address, PtrType This, Args... parameters)
	{
		PushParameter(This);
		PushParameters(parameters...);
		return Call(address);
	}
	template<typename... Args> Return_t ThisCall(const String& FunctionName, PtrType This, Args... parameters)
	{
		if (CheckIfFunctionExist(FunctionName))
		{
			return ThisCall(Get_State()->FindAddress(FunctionName).value(),This,parameters...);
		}
		return Return_t(RetState::Error_Function_doesnt_exist);
	}
	template<typename... Args> Return_t ThisCall(const ClassMethod* Function, PtrType This, Args... parameters)
	{
		return ThisCall(Function->DecorationName, This, parameters...);
	}



	template<typename T, typename... Args>
	T RCall(const String& FunctionName, Args... parameters)
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
	template<typename T, typename... Args>
	T RCall(const ClassMethod* Function, Args... parameters)
	{
		return RCall<T>(Function->DecorationName, parameters...);
	}
	template<typename T,typename... Args>
	T RThisCall(const ClassMethod* Function, PtrType This, Args... parameters)
	{
		return RThisCall<T>(Function->DecorationName,This,parameters...);
	}
	template<typename T, typename... Args> T RThisCall(const String& Function, PtrType This, Args... parameters)
	{
		if (CheckIfFunctionExist(Function))
		{
			auto V = ThisCall(Function, This, parameters...);
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
	UCodeLangForceinline void Log(const char* Ptr) { return _State->Log(Ptr);}
	UCodeLangForceinline void Log(const char* Ptr,size_t Size) { return _State->Log(Ptr,Size); }

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

	inline void* GetThreadPtr()
	{
		return _CPU.ThreadRegister;
	}
	inline const void* GetThreadPtr() const
	{
		return _CPU.ThreadRegister;
	}

	inline BytesView GetStackSpan()
	{
		if (_CPU.Stack._Data)
		{
			return BytesView::Make((Byte*)_CPU.Stack._Data,_CPU.MaxStackSize);
		}
		else
		{
			return BytesView();
		}
	}
	inline const BytesView GetStackSpan() const
	{
		if (_CPU.Stack._Data)
		{
			return BytesView::Make((Byte*)_CPU.Stack._Data, _CPU.MaxStackSize);
		}
		else
		{
			return BytesView();
		}
	}
	inline size_t GetStackOffset()
	{
		return _CPU.Stack.StackOffSet;
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
		UAddress ProgramCounter=NullAddress;
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

				if (StackOffSet + sizeof(T) > MaxStackSize){ ThrowException("stack overflow");}

				StackOffSet += sizeof(T);
			}
			void PushBytes(const void* Ptr,NSize_t Size)
			{
				void* DataPtr = (void*)((UIntNative)_Data + StackOffSet);
				UInt8* StackBytes = (UInt8*)DataPtr;
				UInt8* Bytes = (UInt8*)Ptr;

				if (StackOffSet + Size > MaxStackSize){ ThrowException("stack overflow");}

				for (size_t i = 0; i < Size; i++)
				{
					StackBytes[i] = Bytes[i];
				}

				StackOffSet += Size;
			}
			
			
			template<typename T> T PopStack()
			{
				if (StackOffSet - sizeof(T) < 0) { ThrowException("stack underflow"); }//This May not work

				StackOffSet -= sizeof(T);
				void* DataPtr = (void*)((UIntNative)_Data + StackOffSet);
				return *(T*)DataPtr;
			}
			
			
			template<typename T> void SetValue(const T& V,NSize_t offset)
			{
				if (StackOffSet + offset + sizeof(T) < 0) { ThrowException("stack overflow"); }

				void* DataPtr = (void*)((UIntNative)_Data + StackOffSet + offset);

				*(T*)DataPtr = V;
			}
			template<typename T> void SetValueSub(const T& V, NSize_t offset)
			{
				if (StackOffSet + offset + sizeof(T) < 0) { ThrowException("stack overflow"); }

				void* DataPtr = (void*)((UIntNative)_Data + StackOffSet - offset);

				*(T*)DataPtr = V;
			}
			template<typename T> T GetValue(NSize_t offset)
			{
				if (StackOffSet + offset + sizeof(T) < 0) { ThrowException("stack overflow"); }

				void* DataPtr = (void*)((UIntNative)_Data + StackOffSet + offset);

				return *(T*)DataPtr;
			}
			template<typename T> T GetValueSub(NSize_t offset)
			{
				if (StackOffSet + offset - sizeof(T) < 0) { ThrowException("stack overflow"); }

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
			void ThrowException(const char* Err)
			{
				UCodeLangThrowException(Err);
			}
		};
		Stack Stack;
		void* ThreadRegister = nullptr;
		//
		
		
		CPUData(): ProgramCounter(NullAddress), Stack(), RetValue()
		{

		}
		inline void InitStack(void* StackValue)
		{
			Stack._Data = StackValue;
		}
		inline void InitThread(void* ThreadValue)
		{
			ThreadRegister = ThreadValue;
		}
		void ThrowException(const char* Err)
		{
			UCodeLangThrowException(Err);
		}
	};
	CPUData _CPU;
	RunTimeLangState* _State = nullptr;
	InterpreterCPPinterface* _CPPHelper = nullptr;
	UserMadeContext _UserMadeContext;
	ParameterPassingHelper _Parameters;

	#if UCodeLangDebug
	bool WasInit = false;
	bool InDestruct = false;
	bool GotRetValue = false;
	bool CalledFuncBefor = false;
	#endif // DEBUG


	void FlushParametersIntoCPU();

	UCodeLangForceinline PtrType Get_StaticMemPtr(){return _State->Get_StaticMemPtr();}
	UCodeLangForceinline PtrType Get_ThreadMemPtr() { return _CPU.ThreadRegister; }
	UCodeLangConstexprForceinline Register& Get_ThisRegister() { return Get_Register(RegisterID::ThisRegister); }
	UCodeLangConstexprForceinline Register& Get_OutRegister() { return Get_Register(RegisterID::OutPutRegister); }
	UCodeLangConstexprForceinline Register& Get_InputRegister() { return Get_Register(RegisterID::InPutRegister); }
	
	UCodeLangConstexprForceinline Register& Get_MathOutRegister() { return Get_Register(RegisterID::MathOutPutRegister); }
	UCodeLangConstexprForceinline Register& Get_BoolRegister() { return Get_Register(RegisterID::BoolRegister); }
	UCodeLangConstexprForceinline Register& Get_BitwiseRegister() { return Get_Register(RegisterID::BitwiseRegister); }
	UCodeLangConstexprForceinline Register& Get_AwaitOutRegister() { return Get_Register(RegisterID::AwaitOutRegister); }


	UCodeLangConstexprForceinline Register& Get_Register(RegisterID Id)
	{
		switch (Id)
		{
		case UCodeLang::RegisterID::A:return _CPU.A;
		case UCodeLang::RegisterID::B:return _CPU.B;
		case UCodeLang::RegisterID::C:return _CPU.C;
		case UCodeLang::RegisterID::D:return _CPU.D;
		case UCodeLang::RegisterID::E:return _CPU.E;
		case UCodeLang::RegisterID::F:return _CPU.F;
		default:
			UCodeLangUnreachable();
		}
	}
	void InterpreterSysCall(InstructionSysCall SysCall,RegisterID ParReg);

	//Just in Hot reloading
	void ResetThreadPointer(PtrType NewThreadBase)
	{
		_CPU.ThreadRegister = NewThreadBase;
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

	
	template<typename T> void GetParameter(T& Out)
	{
		Out = GetParameter<T>();
	}
	template<typename T> void GetParameter(T* Out)
	{
		*Out = GetParameter<T>();
	}

	///please dont use Input.Set_Return(Func(Input.GetParameter<T>(),Input.GetParameter<T>())) 
	///c++ will evaluate and pass Parameter in right to left order but Ucode want parameters passed in left to right order.
	///do 
	/// auto A = Input.GetParameter<T>();
	/// auto B = Input.GetParameter<T>();
	/// Input.Set_Return(Func(A, B));
	template<typename T> T GetParameter()
	{
		Byte Value[sizeof(T)];
		GetParameter_jit(*this, &Value, sizeof(T));

		return *(T*)&Value;
	}

	

	template<typename T> void Set_Return(const T& Value) 
	{

		constexpr bool IsBigerRegister = sizeof(T) > sizeof(Interpreter::Register);
		if (IsBigerRegister) 
		{
			_Ptr->_CPU.Stack.SetValue(Value,0);
			Get_OutPutRegister().Value = _Ptr->_CPU.Stack.GetTopOfStack();
		}
		else
		{
			memcpy(&Get_OutPutRegister(), &Value, sizeof(T));
		}
	}


	

	void Set_Return(){}

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

	UCodeLangForceinline auto& Get_InPutRegister() { return _Ptr->Get_InputRegister(); }
	UCodeLangForceinline auto& Get_OutPutRegister() { return _Ptr->Get_OutRegister(); }
	UCodeLangForceinline auto& Get_ThisRegister() { return _Ptr->Get_ThisRegister(); }
	
//
	UCodeLangForceinline PtrType Malloc(NSize_t Size) { return _Ptr->Malloc(Size); }
	UCodeLangForceinline void Free(PtrType Ptr) { return _Ptr->Free(Ptr); }
	UCodeLangForceinline PtrType Realloc(PtrType Ptr,NSize_t Size) { return _Ptr->Realloc(Ptr,Size); }
	UCodeLangForceinline PtrType Calloc(NSize_t Size) { return _Ptr->Calloc(Size); }
	
	InterpreterCPPinterface(Interpreter* Ptr) : _Ptr(Ptr)
	{
		ParInfoReset();
	}
	
	//used by jit
	static void  UCodeLangAPI  Set_Return_jit(InterpreterCPPinterface& This,const void* Pointer,size_t Buffer)
	{

		bool IsBigerRegister = Buffer > sizeof(Interpreter::Register);
		if (IsBigerRegister)
		{
			memcpy(This._Ptr->_CPU.Stack.GetTopOfStack(), Pointer, Buffer);
			This.Get_OutPutRegister().Value = This._Ptr->_CPU.Stack.GetTopOfStack();
		}
		else
		{
			memcpy(&This.Get_OutPutRegister(), Pointer, Buffer);
		}
	}
	static void  UCodeLangAPI GetParameter_jit(InterpreterCPPinterface& This,void* Pointer, size_t BufferSize)
	{
		bool IsBigerRegister = BufferSize > sizeof(Interpreter::Register);
		if (IsBigerRegister || ParValue == RegisterID::EndParameterRegister)
		{
			ParStackOffset += BufferSize;
			auto r = This._Ptr->_CPU.Stack.GetTopOfStackWithoffset(ParStackOffset);
			memcpy(Pointer,r,BufferSize);
		}
		else
		{
			auto r = &This._Ptr->Get_Register(ParValue).Value;

			memcpy(Pointer, r, BufferSize);

			(*(RegisterID_t*)&ParValue)++;
		}
	}
private:
	

	Interpreter* _Ptr = nullptr;
};
UCodeLangEnd

