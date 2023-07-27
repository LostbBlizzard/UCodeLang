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
		:Base(std::move(Value)){}
	AnyInterpreter(Jit_Interpreter&& Value)
		:Base(std::move(Value)){}
	AnyInterpreter(NativeInterpreter&& Value)
		:Base(std::move(Value)){}
	
	AnyInterpreter& operator=(Interpreter&& Value)
	{
		Base = std::move(Value);
	}
	AnyInterpreter& operator=(Jit_Interpreter&& Value)
	{
		Base = std::move(Value);
	}
	AnyInterpreter& operator=(NativeInterpreter&& Value)
	{
		Base = std::move(Value);
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
	void Init(RunTimeLangState* State)
	{
		Get_Ptr().Init(State);
	}
	void UnLoad()
	{
		Get_Ptr().UnLoad();
	}

	InterpreterTypes Get_InterpreterType() const
	{
		return Get_Ptr().Get_InterpreterType();
	}

	Interpreter& GetAs_Interpreter()
	{
		return Base.Get<Interpreter>();
	}
	Jit_Interpreter& GetAs_JitInterpreter()
	{
		return Base.Get<Jit_Interpreter>();
	}
	NativeInterpreter& GetAs_NativeInterpreter()
	{
		return Base.Get<NativeInterpreter>();
	}

	const Interpreter& GetAs_Interpreter()const
	{
		return Base.Get<Interpreter>();
	}
	const Jit_Interpreter& GetAs_JitInterpreter()const
	{
		return Base.Get<Jit_Interpreter>();
	}
	const NativeInterpreter GetAs_NativeInterpreter()const
	{
		return Base.Get<NativeInterpreter>();
	}
	AnyInterpreterPtr GetPtr()
	{
		return  Get_Ptr();
	}
private:
	struct Null {};
	Variant<Null, Interpreter, Jit_Interpreter, NativeInterpreter> Base;

	UCodeLangForceinline AnyInterpreterPtr Get_Ptr()
	{
		if (auto Val = Base.Get_If<Interpreter>())
		{
			return AnyInterpreterPtr(Val);
		}
		else if (auto Val = Base.Get_If<Jit_Interpreter>())
		{
			return AnyInterpreterPtr(Val);
		}
		else if (auto Val = Base.Get_If<NativeInterpreter>())
		{
			return AnyInterpreterPtr(Val);
		}
		else
		{
			throw std::exception("Ptr Is Null");
		}
	}

	 UCodeLangForceinline const AnyInterpreterPtr Get_Ptr() const
	{
		if (auto Val = Base.Get_If<Interpreter>())
		{
			return AnyInterpreterPtr::Make(Val);
		}
		else if (auto Val = Base.Get_If<Jit_Interpreter>())
		{
			return AnyInterpreterPtr::Make(Val);
		}
		else if (auto Val = Base.Get_If<NativeInterpreter>())
		{
			return AnyInterpreterPtr::Make(Val);
		}
		else
		{
			throw std::exception("Ptr Is Null");
		}
	}
};

UCodeLangEnd