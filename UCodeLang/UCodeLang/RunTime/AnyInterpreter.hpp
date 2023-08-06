#pragma once
#include "Interpreters/Interpreter.hpp"
#include "Jit/Jit_Interpreter.hpp"
#include "Interpreters/NativeInterpreter.hpp"


UCodeLangStart

using InterpreterTypes_t = int;
enum class InterpreterTypes : InterpreterTypes_t
{
	Null,
	Interpreter,
	Jit_Interpreter,
	NativeInterpreter,
};

struct AnyInterpreterPtr
{
	AnyInterpreterPtr(){}
	AnyInterpreterPtr(Interpreter* ptr): Base(ptr){}
	AnyInterpreterPtr(Jit_Interpreter* ptr) : Base(ptr){}
	AnyInterpreterPtr(NativeInterpreter* ptr) : Base(ptr){}

	

	void Init(RunTimeLangState* State)
	{
		if (auto Val =Base.Get_If<Interpreter*>())
		{
			return (*Val)->Init(State);
		}
		else if (auto Val = Base.Get_If<Jit_Interpreter*>())
		{
			return (*Val)->Init(State);
		}
		else if (auto Val = Base.Get_If<NativeInterpreter*>())
		{
			return (*Val)->Init(State);
		}
		else
		{
			throw std::exception("Ptr Is Null");
		}
	}
	void UnLoad()
	{
		if (auto Val = Base.Get_If<Interpreter*>())
		{
			return (*Val)->UnLoad();
		}
		else if (auto Val = Base.Get_If<Jit_Interpreter*>())
		{
			return (*Val)->UnLoad();
		}
		else if (auto Val = Base.Get_If<NativeInterpreter*>())
		{
			return (*Val)->UnLoad();
		}
		else
		{
			throw std::exception("Ptr Is Null");
		}
	}
	Interpreter::Return_t Call(UAddress Address)
	{
		if (auto Val = Base.Get_If<Interpreter*>())
		{
			return (*Val)->Call(Address);
		}
		else if (auto Val = Base.Get_If<Jit_Interpreter*>())
		{
			return (*Val)->Call(Address);
		}
		else if (auto Val = Base.Get_If<NativeInterpreter*>())
		{
			return (*Val)->Call(Address);
		}
		else
		{
			throw std::exception("Ptr Is Null");
		}
	}
	Interpreter::Return_t Call(const String& FunctionName)
	{
		if (auto Val = Base.Get_If<Interpreter*>())
		{
			return (*Val)->Call(FunctionName);
		}
		else if (auto Val = Base.Get_If<Jit_Interpreter*>())
		{
			return (*Val)->Call(FunctionName);
		}
		else if (auto Val = Base.Get_If<NativeInterpreter*>())
		{
			return (*Val)->Call(FunctionName);
		}
		else
		{
			throw std::exception("Ptr Is Null");
		}
	}
	Interpreter::Return_t Call(const ClassMethod* Function)
	{
		if (auto Val = Base.Get_If<Interpreter*>())
		{
			return (*Val)->Call(Function);
		}
		else if (auto Val = Base.Get_If<Jit_Interpreter*>())
		{
			return (*Val)->Call(Function);
		}
		else if (auto Val = Base.Get_If<NativeInterpreter*>())
		{
			return (*Val)->Call(Function);
		}
		else
		{
			throw std::exception("Ptr Is Null");
		}
	}
	void PushParameter(const void* Value, size_t ValueSize)
	{
		if (auto Val = Base.Get_If<Interpreter*>())
		{
			return (*Val)->PushParameter(Value,ValueSize);
		}
		else if (auto Val = Base.Get_If<Jit_Interpreter*>())
		{
			return (*Val)->PushParameter(Value, ValueSize);
		}
		else if (auto Val = Base.Get_If<NativeInterpreter*>())
		{
			return (*Val)->PushParameter(Value, ValueSize);
		}
		else
		{
			throw std::exception("Ptr Is Null");
		}
	}
	template<typename T> void PushParameter(const T& Value)
	{
		if (auto Val = Base.Get_If<Interpreter*>())
		{
			return (*Val)->PushParameter(Value);
		}
		else if (auto Val = Base.Get_If<Jit_Interpreter*>())
		{
			return (*Val)->PushParameter(Value);
		}
		else if (auto Val = Base.Get_If<NativeInterpreter*>())
		{
			return (*Val)->PushParameter(Value);
		}
		else
		{
			throw std::exception("Ptr Is Null");
		}
	}
	template<typename... Args> void PushParameters(Args&&... parameters)
	{
		if (auto Val = Base.Get_If<Interpreter*>())
		{
			return (*Val)->PushParameters(parameters...);
		}
		else if (auto Val = Base.Get_If<Jit_Interpreter*>())
		{
			return (*Val)->PushParameters(parameters...);
		}
		else if (auto Val = Base.Get_If<NativeInterpreter*>())
		{
			return (*Val)->PushParameters(parameters...);
		}
		else
		{
			throw std::exception("Ptr Is Null");
		}
	}
	template<typename T> T Get_Return()
	{
		if (auto Val = Base.Get_If<Interpreter*>())
		{
			return (*Val)->Get_Return<T>();
		}
		else if (auto Val = Base.Get_If<Jit_Interpreter*>())
		{
			return (*Val)->Get_Return<T>();
		}
		else if (auto Val = Base.Get_If<NativeInterpreter*>())
		{
			return (*Val)->Get_Return<T>();
		}
		else
		{
			throw std::exception("Ptr Is Null");
		}
	}
	void Get_Return(void* Output, size_t OutputSize)
	{
		if (auto Val = Base.Get_If<Interpreter*>())
		{
			return (*Val)->Get_Return(Output,OutputSize);
		}
		else if (auto Val = Base.Get_If<Jit_Interpreter*>())
		{
			return (*Val)->Get_Return(Output, OutputSize);
		}
		else if (auto Val = Base.Get_If<NativeInterpreter*>())
		{
			return (*Val)->Get_Return(Output, OutputSize);
		}
		else
		{
			throw std::exception("Ptr Is Null");
		}
	}
	template<typename... Args> 	Interpreter::Return_t ThisCall(UAddress address, PtrType This, Args... parameters)
	{
		if (auto Val = Base.Get_If<Interpreter*>())
		{
			return (*Val)->ThisCall(address,This,parameters...);
		}
		else if (auto Val = Base.Get_If<Jit_Interpreter*>())
		{
			return (*Val)->ThisCall(address, This, parameters...);
		}
		else if (auto Val = Base.Get_If<NativeInterpreter*>())
		{
			return (*Val)->ThisCall(address, This, parameters...);
		}
		else
		{
			throw std::exception("Ptr Is Null");
		}
	}
	template<typename... Args> Interpreter::Return_t ThisCall(const String& FunctionName, PtrType This, Args... parameters)
	{
		if (auto Val = Base.Get_If<Interpreter*>())
		{
			return (*Val)->ThisCall(FunctionName, This, parameters...);
		}
		else if (auto Val = Base.Get_If<Jit_Interpreter*>())
		{
			return (*Val)->ThisCall(FunctionName, This, parameters...);
		}
		else if (auto Val = Base.Get_If<NativeInterpreter*>())
		{
			return (*Val)->ThisCall(FunctionName, This, parameters...);
		}
		else
		{
			throw std::exception("Ptr Is Null");
		}
	}
	template<typename... Args> Interpreter::Return_t ThisCall(const ClassMethod* Function, PtrType This, Args... parameters)
	{
		if (auto Val = Base.Get_If<Interpreter*>())
		{
			return (*Val)->ThisCall(Function, This, parameters...);
		}
		else if (auto Val = Base.Get_If<Jit_Interpreter*>())
		{
			return (*Val)->ThisCall(Function, This, parameters...);
		}
		else if (auto Val = Base.Get_If<NativeInterpreter*>())
		{
			return (*Val)->ThisCall(Function, This, parameters...);
		}
		else
		{
			throw std::exception("Ptr Is Null");
		}
	}



	template<typename T, typename... Args>
	T RCall(const String& FunctionName, Args... parameters)
	{
		if (auto Val = Base.Get_If<Interpreter*>())
		{
			return (*Val)->RCall<T>(FunctionName,parameters);
		}
		else if (auto Val = Base.Get_If<Jit_Interpreter*>())
		{
			return  (*Val)->RCall<T>(FunctionName, parameters);
		}
		else if (auto Val = Base.Get_If<NativeInterpreter*>())
		{
			return  (*Val)->RCall<T>(FunctionName, parameters);
		}
		else
		{
			throw std::exception("Ptr Is Null");
		}
	}
	template<typename T, typename... Args>
	T RCall(const ClassMethod* Function, Args... parameters)
	{
		if (auto Val = Base.Get_If<Interpreter*>())
		{
			return (*Val)->RCall<T>(Function, parameters...);
		}
		else if (auto Val = Base.Get_If<Jit_Interpreter*>())
		{
			return  (*Val)->RCall<T>(Function, parameters...);
		}
		else if (auto Val = Base.Get_If<NativeInterpreter*>())
		{
			return  (*Val)->RCall<T>(Function, parameters...);
		}
		else
		{
			throw std::exception("Ptr Is Null");
		}
	}
	template<typename T, typename... Args>
	T RThisCall(const ClassMethod* Function, PtrType This, Args... parameters)
	{
		if (auto Val = Base.Get_If<Interpreter*>())
		{
			return (*Val)->RThisCall<T>(Function,This, parameters...);
		}
		else if (auto Val = Base.Get_If<Jit_Interpreter*>())
		{
			return  (*Val)->RThisCall<T>(Function,This, parameters...);
		}
		else if (auto Val = Base.Get_If<NativeInterpreter*>())
		{
			return  (*Val)->RThisCall<T>(Function,This, parameters...);
		}
		else
		{
			throw std::exception("Ptr Is Null");
		}
	}
	template<typename T, typename... Args> T RThisCall(const String& Function, PtrType This, Args... parameters)
	{
		if (auto Val = Base.Get_If<Interpreter*>())
		{
			return (*Val)->RThisCall<T>(Function, This, parameters...);
		}
		else if (auto Val = Base.Get_If<Jit_Interpreter*>())
		{
			return  (*Val)->RThisCall<T>(Function, This, parameters...);
		}
		else if (auto Val = Base.Get_If<NativeInterpreter*>())
		{
			return  (*Val)->RThisCall<T>(Function, This, parameters...);
		}
		else
		{
			throw std::exception("Ptr Is Null");
		}
	}

	static const AnyInterpreterPtr Make(const Interpreter* ptr)
	{
		return AnyInterpreterPtr((Interpreter*)ptr);
	}
	static const AnyInterpreterPtr Make(const Jit_Interpreter* ptr)
	{
		return AnyInterpreterPtr((Interpreter*)ptr);
	}
	static const AnyInterpreterPtr Make(const NativeInterpreter* ptr)
	{
		return AnyInterpreterPtr((Interpreter*)ptr);
	}
	static AnyInterpreterPtr Make(Interpreter* ptr)
	{

		return AnyInterpreterPtr(ptr);
	}
	static AnyInterpreterPtr Make(Jit_Interpreter* ptr)
	{

		return AnyInterpreterPtr(ptr);
	}
	static AnyInterpreterPtr Make(NativeInterpreter* ptr)
	{
		return AnyInterpreterPtr(ptr);
	}

	InterpreterTypes Get_InterpreterType() const
	{
		if (auto Val = Base.Get_If<Interpreter*>())
		{
			return InterpreterTypes::Interpreter;
		}
		else if (auto Val = Base.Get_If<Jit_Interpreter*>())
		{
			return InterpreterTypes::Jit_Interpreter;
		}
		else if (auto Val = Base.Get_If<NativeInterpreter*>())
		{
			return InterpreterTypes::NativeInterpreter;
		}
		else
		{
			return InterpreterTypes::Null;
		}
	}
	
	Interpreter* GetAs_Interpreter() 
	{
		return Base.Get<Interpreter*>();
	}
	Jit_Interpreter* GetAs_JitInterpreter()
	{
		return Base.Get<Jit_Interpreter*>();
	}
	NativeInterpreter* GetAs_NativeInterpreter()
	{
		return Base.Get<NativeInterpreter*>();
	}

	const Interpreter* GetAs_Interpreter()const
	{
		return Base.Get<Interpreter*>();
	}
	const Jit_Interpreter* GetAs_JitInterpreter()const
	{
		return Base.Get<Jit_Interpreter*>();
	}
	const NativeInterpreter* GetAs_NativeInterpreter()const
	{
		return Base.Get<NativeInterpreter*>();
	}

	inline void* GetThreadPtr()
	{
		if (auto Val = Base.Get_If<Interpreter*>())
		{
			return (*Val)->GetThreadPtr();
		}
		else if (auto Val = Base.Get_If<Jit_Interpreter*>())
		{
			return  (*Val)->GetThreadPtr();
		}
		else if (auto Val = Base.Get_If<NativeInterpreter*>())
		{
			return  (*Val)->GetThreadPtr();
		}
		else
		{
			throw std::exception("Ptr Is Null");
		}
	}
	inline const void* GetThreadPtr() const
	{
		if (auto Val = Base.Get_If<Interpreter*>())
		{
			return (*Val)->GetThreadPtr();
		}
		else if (auto Val = Base.Get_If<Jit_Interpreter*>())
		{
			return  (*Val)->GetThreadPtr();
		}
		else if (auto Val = Base.Get_If<NativeInterpreter*>())
		{
			return  (*Val)->GetThreadPtr();
		}
		else
		{
			throw std::exception("Ptr Is Null");
		}
	}
private:
	struct Null{};
	Variant<Null,Interpreter*, Jit_Interpreter*, NativeInterpreter*> Base;
};


//a thin wapper over the 
class AnyInterpreter
{
public:
	AnyInterpreter()
	{

	}
	AnyInterpreter(AnyInterpreter&& Value) = default;
	AnyInterpreter& operator=(AnyInterpreter&& Value) = default;

	AnyInterpreter(Interpreter&& Value)
		:Base(std::make_shared<Interpreter>(std::move(Value))) {}
	AnyInterpreter(Jit_Interpreter&& Value)
		:Base(std::make_shared<Jit_Interpreter>(std::move(Value))) {}
	AnyInterpreter(NativeInterpreter&& Value)
		:Base(std::make_shared<NativeInterpreter>(std::move(Value))) {}

	AnyInterpreter& operator=(Interpreter&& Value)
	{
		Base = std::make_shared<Interpreter>(std::move(Value));
	}
	AnyInterpreter& operator=(Jit_Interpreter&& Value)
	{
		Base = std::make_shared<Jit_Interpreter>(std::move(Value));
	}
	AnyInterpreter& operator=(NativeInterpreter&& Value)
	{
		Base = std::make_shared<NativeInterpreter>(std::move(Value));
	}
	~AnyInterpreter()
	{

	}
	void SetAsInterpreter()
	{
		*this = AnyInterpreter(Interpreter());
	}
	void SetAsJitInterpreter()
	{
		*this = AnyInterpreter(Jit_Interpreter());
	}
	void SetAsNativeInterpreter()
	{
		*this = AnyInterpreter(NativeInterpreter());
	}


	InterpreterTypes Get_InterpreterType() const
	{
		return Get_Ptr().Get_InterpreterType();
	}

	Interpreter& GetAs_Interpreter()
	{
		return *Base.Get<Shared_ptr<Interpreter>>();
	}
	Jit_Interpreter& GetAs_JitInterpreter()
	{
		return *Base.Get<Shared_ptr<Jit_Interpreter>>();
	}
	NativeInterpreter& GetAs_NativeInterpreter()
	{
		return *Base.Get<Shared_ptr<NativeInterpreter>>();
	}

	const Interpreter& GetAs_Interpreter()const
	{
		return *Base.Get< Shared_ptr<Interpreter>>();
	}
	const Jit_Interpreter& GetAs_JitInterpreter()const
	{
		return *Base.Get< Shared_ptr<Jit_Interpreter>>();
	}
	const NativeInterpreter& GetAs_NativeInterpreter()const
	{
		return *Base.Get<Shared_ptr<NativeInterpreter>>();
	}
	AnyInterpreterPtr GetPtr()
	{
		return  Get_Ptr();
	}

	inline void* GetThreadPtr()
	{
		return Get_Ptr().GetThreadPtr();
	}
	inline const void* GetThreadPtr() const
	{
		return  Get_Ptr().GetThreadPtr();
	}

	void Init(RunTimeLangState* State)
	{
		Get_Ptr().Init(State);
	}
	void UnLoad()
	{
		Get_Ptr().UnLoad();
	}
	Interpreter::Return_t Call(UAddress Address)
	{
		return Get_Ptr().Call(Address);
	}
	Interpreter::Return_t Call(const String& FunctionName)
	{
		return Get_Ptr().Call(FunctionName);
	}
	Interpreter::Return_t Call(const ClassMethod* Function)
	{
		return Get_Ptr().Call(Function);
	}
	void PushParameter(const void* Value, size_t ValueSize)
	{
		return Get_Ptr().PushParameter(Value, ValueSize);
	}
	template<typename T> void PushParameter(const T& Value)
	{
		return Get_Ptr().PushParameter(Value);
	}
	template<typename... Args> void PushParameters(Args&&... parameters)
	{
		return Get_Ptr().PushParameters(parameters...);
	}
	template<typename T> T Get_Return()
	{
		return Get_Ptr().Get_Return<T>();
	}
	void Get_Return(void* Output, size_t OutputSize)
	{
		return Get_Ptr().Get_Return(Output, OutputSize);
	}
	template<typename... Args> 	Interpreter::Return_t ThisCall(UAddress address, PtrType This, Args... parameters)
	{
		return Get_Ptr().ThisCall(address, This, parameters...);
	}
	template<typename... Args> Interpreter::Return_t ThisCall(const String& FunctionName, PtrType This, Args... parameters)
	{
		return Get_Ptr().ThisCall(FunctionName, This, parameters...);
	}
	template<typename... Args> Interpreter::Return_t ThisCall(const ClassMethod* Function, PtrType This, Args... parameters)
	{
		return Get_Ptr().ThisCall(Function, This, parameters...);
	}



	template<typename T, typename... Args>
	T RCall(const String& FunctionName, Args... parameters)
	{
		return Get_Ptr().RCall<T>(FunctionName, parameters...);
	}
	template<typename T, typename... Args>
	T RCall(const ClassMethod& Function, Args... parameters)
	{
		return Get_Ptr().RCall<T>(Function, parameters...);
	}
	template<typename T, typename... Args>
	T RThisCall(const ClassMethod& Function, PtrType This, Args... parameters)
	{
		return Get_Ptr().RThisCall<T>(Function,This, parameters...);
	}
	template<typename T, typename... Args> T RThisCall(const String& Function, PtrType This, Args... parameters)
	{
		return Get_Ptr().RThisCall<T>(Function, This, parameters...);
	}
private:
	struct Null {};
	//useing Shared_ptr because std::Variant need it to be copyable
	Variant<Null,Shared_ptr<Interpreter>,Shared_ptr<Jit_Interpreter>, Shared_ptr<NativeInterpreter>> Base;

	UCodeLangForceinline AnyInterpreterPtr Get_Ptr()
	{
		if (auto Val = Base.Get_If< Shared_ptr<Interpreter>>())
		{
			return AnyInterpreterPtr(Val->get());
		}
		else if (auto Val = Base.Get_If< Shared_ptr<Jit_Interpreter>>())
		{
			return AnyInterpreterPtr(Val->get());
		}
		else if (auto Val = Base.Get_If< Shared_ptr<NativeInterpreter>>())
		{
			return AnyInterpreterPtr(Val->get());
		}
		else
		{
			throw std::exception("Ptr Is Null");
		}
	}

	 UCodeLangForceinline const AnyInterpreterPtr Get_Ptr() const
	{
		if (auto Val = Base.Get_If< Shared_ptr<Interpreter>>())
		{
			return AnyInterpreterPtr::Make(Val->get());
		}
		else if (auto Val = Base.Get_If< Shared_ptr<Jit_Interpreter>>())
		{
			return AnyInterpreterPtr::Make(Val->get());
		}
		else if (auto Val = Base.Get_If< Shared_ptr<NativeInterpreter>>())
		{
			return AnyInterpreterPtr::Make(Val->get());
		}
		else
		{
			throw std::exception("Ptr Is Null");
		}
	}
};

UCodeLangEnd