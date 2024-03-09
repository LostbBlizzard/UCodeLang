#pragma once
#include "../LangCore.hpp"
#include "UCLibManger.hpp"
#include <stdlib.h>
#include <iostream>
#include "Jit/Jit.hpp"
#include "UCodeLang/LangCore/DataType/UnorderedMap.hpp"
#include "Interpreters/ParameterPassingHelper.hpp"
#include "UCodeLang/LangCore/TaskManger.hpp"
UCodeLangStart

class RunTimeLangState;
class Allocator
{
public:
	typedef PtrType(*Malloc_t)(RunTimeLangState& This, NSize_t Size);
	typedef void (*Free_t)(RunTimeLangState& This, PtrType ptr);
	
	Allocator(Malloc_t malloc, Free_t free) :
		_Malloc(malloc), _Free(free), _CanReserveData(false), _MallocOnlyInPages(true)
	{
		Tep_Values.reserve(10);
	}
	PtrType Malloc(RunTimeLangState& This, NSize_t Size);
	void Free(RunTimeLangState& This, PtrType ptr);
	PtrType Realloc(RunTimeLangState& This, PtrType oldptr, NSize_t Size);
	void ReservedBytes(RunTimeLangState& This, NSize_t Size);
	
	void FreeAllAllocations(RunTimeLangState& This);
	void FreeAllReservedAllocations(RunTimeLangState& This);
	void MergeReservedAllocations();

	UCodeLangForceinline void Set_Malloc(Malloc_t V){_Malloc = V;}
	UCodeLangForceinline void Set_Free(Free_t  V){_Free = V;}
	UCodeLangForceinline void Set_CanReserveData(bool V){_CanReserveData = true;}
	UCodeLangForceinline void Set_MallocOnlyInPages(bool V){_MallocOnlyInPages = true;}

	auto& Get_AlocsBeingUsed()
	{
		return _Data;
	}
	auto& Get_ReservedAlocs()
	{
		return _ReservedData;
	}
	
	struct MemData
	{
		bool IsfakePtr;
		NSize_t Size;
	};
private:
	Malloc_t _Malloc;
	Free_t _Free;
	bool _CanReserveData;
	bool _MallocOnlyInPages;
	
	UnorderedMap<PtrType, MemData> _Data;
	UnorderedMap<PtrType, MemData> _ReservedData;
	Vector<void*> Tep_Values;
	PtrType FindReservedPtr(NSize_t Size);
};

//if you like OOP use can Use this reference a Object
struct UserMadeContext
{
	constexpr UserMadeContext() : _Ptr(nullptr)
	{

	}
	constexpr UserMadeContext(void* Ptr) : _Ptr(Ptr)
	{

	}
	void* _Ptr = nullptr;
};

struct DebugContext
{
	enum class Type
	{
		Interpreter,
		Jit_Interpreter,
		Native_Interpreter,
	};

	//We Dont want to include AnyInterpreterPtr
	struct InterpreterInfo
	{
		void* ThisInterpreter = nullptr;
		Type type;
	};
	using FuncStart = void(*)(RunTimeLangState& This, void* _Ptr, InterpreterInfo Info);
	using FuncEnd = void(*)(RunTimeLangState& This,void* _Ptr, InterpreterInfo Info);
	using FuncOnLine = void(*)(RunTimeLangState& This, void* _Ptr, InterpreterInfo Info);

	void* _Ptr =nullptr;
	FuncStart _FuncStart = nullptr;
	FuncEnd _FuncEnd = nullptr;
	FuncOnLine _FuncOnLine = nullptr;

	void TryFuncStart(RunTimeLangState& This, InterpreterInfo Info) const
	{
		if (_FuncStart)
		{
			_FuncStart(This, _Ptr, Info);
		}
	}
	void TryFuncEnd(RunTimeLangState& This, InterpreterInfo Info) const
	{
		if (_FuncStart)
		{
			_FuncEnd(This, _Ptr, Info);
		}
	}
	void TryFuncOnLine(RunTimeLangState& This, InterpreterInfo Info) const
	{
		if (_FuncStart)
		{
			_FuncOnLine(This, _Ptr,Info);
		}
	}

};

class PackagedTask
{
public:
	using InterpreterInfo = DebugContext::InterpreterInfo;
	PackagedTask() { Reset(); }
	~PackagedTask() {}

	void Reset()
	{
		_Parameters.Clear();
		Func = NullAddress;
	}


	void Invoke(InterpreterInfo& This);

	template<typename T> T RInvoke(InterpreterInfo& This)
	{
		T CompilerRet;
		RInvoke(This, &CompilerRet, sizeof(T));
		return CompilerRet;
	}

	void RInvoke(InterpreterInfo& This, void* OutObject, size_t ReturnObjectSize);


	template<typename T> UCodeLangForceinline void PushParameter(const T& Value)
	{
		PushParameter((const void*)&Value, sizeof(Value));
	}
	void PushParameter(const void* Value, size_t ValueSize)
	{
		_Parameters.Push(Value, ValueSize);
	}
	void Set_Func(UAddress func)
	{
		Func = func;
	}

private:
	ParameterPassingHelper _Parameters;
	UAddress Func = NullAddress;
};

class RunTimeLangState
{
public:
	using InterpreterInfo = DebugContext::InterpreterInfo;

	using LogCallBack = void(*)(RunTimeLangState& This,const char* Text,size_t Size);
	using ReadCharCallBack =  void(*)(RunTimeLangState& This,char* Buffer,size_t Size);
	
	using AwaitedTask = void*;
	using AwaitNawTaskCallBack = AwaitedTask(*)(RunTimeLangState& This,PackagedTask&& Task);
	using AwaitFreeTaskCallBack = void(*)(RunTimeLangState& This,AwaitedTask TaskToFree);
	using AwaitIsDoneCallBack = bool(*)(RunTimeLangState& This,AwaitedTask Task);
	using AwaitGetValueCallBack  = void(*)(RunTimeLangState& This,AwaitedTask Task, void* OutValue);


	static void Default_Log(RunTimeLangState& This,const char* Text,size_t Size)
	{
		std::cout.write(Text, Size);
	}
	static void Default_Read(RunTimeLangState& This,char* Buffer, size_t Size)
	{
		std::cin.read(Buffer, Size);
	}

	static PtrType Default_Malloc(RunTimeLangState& This, NSize_t Size)
	{
		return malloc(Size);
	}
	static void Default_Free(RunTimeLangState& This, PtrType Ptr)
	{
		return free(Ptr);
	}

	static AwaitedTask Default_AwaitNawTask(RunTimeLangState& This,PackagedTask&& Task)
	{
		#if UCodeLangDebug
		UCodeLangThrowException("AwaitNewTask was not overloaded")
		#endif 
		return AwaitedTask();
	}
	static void Default_AwaitFreeTask(RunTimeLangState& This, AwaitedTask TaskToFree)
	{
		#if UCodeLangDebug
		UCodeLangThrowException("AwaitFreeTask was not overloaded")
		#endif 
	}
	static bool Default_AwaitIsDone(RunTimeLangState& This, AwaitedTask Task)
	{
		#if UCodeLangDebug
		UCodeLangThrowException("AwaitIsDone was not overloaded");
		#endif
		return false;
	}
	static void Default_AwaitGetValue(RunTimeLangState& This, AwaitedTask Task, void* OutValue)
	{
		#if UCodeLangDebug
		UCodeLangThrowException("AwaitGetValue was not overloaded");
		#endif
	}


	RunTimeLangState():_Allocator(Default_Malloc,Default_Free), 
		_StaticMemPtr(nullptr), _Log(Default_Log),_Read(Default_Read),
		_AwaitNewTask(Default_AwaitNawTask),_AwaitFreeTask(Default_AwaitFreeTask),
		_AwaitIsDone(Default_AwaitIsDone),_AwaitGetValue(Default_AwaitGetValue)
	{

	}
	~RunTimeLangState()
	{
		_Allocator.FreeAllAllocations(*this);
		_Allocator.MergeReservedAllocations();
		_Allocator.FreeAllReservedAllocations(*this);
	}

	RunTimeLangState(const RunTimeLangState& V) = delete;
	RunTimeLangState& operator=(const RunTimeLangState& V) = delete;



	//Out/In
	UCodeLangForceinline PtrType Malloc(NSize_t Size) {return _Allocator.Malloc(*this,Size); }
	UCodeLangForceinline void Free(PtrType Ptr) { _Allocator.Free(*this, Ptr); }
	UCodeLangForceinline PtrType Realloc(PtrType Ptr, NSize_t Size) {return _Allocator.Realloc(*this, Ptr, Size);}
	UCodeLangForceinline void ReservedBytes(NSize_t Size){_Allocator.ReservedBytes(*this, Size);}
	PtrType Calloc(NSize_t Size) 
	{
		UInt8* Ptr = (UInt8*)Malloc(Size);
		memset(Ptr, 0, Size);
		return (PtrType)Ptr;
	}
	UCodeLangForceinline Allocator& Get_Allocator()
	{
		return _Allocator;
	}
	
	UCodeLangForceinline void Log(const char* ptr)
	{
		Log(ptr, strlen(ptr));
	}
	void Log(const char* ptr, size_t Size)
	{
		_Log(*this, ptr, Size);
	}

	void ReadChar(char* Out, size_t Size)
	{
		return _Read(*this, Out, Size);
	}
	char ReadChar()
	{
		char V;
		ReadChar(&V, 1);
		return V;
	}

	AwaitedTask AwaitNewTask(PackagedTask&& Task)
	{
		return _AwaitNewTask(*this,std::move(Task));
	}
	bool AwaitIsDone(AwaitedTask Task)
	{
		return _AwaitIsDone(*this, Task);
	}
	void AwaitGetValue(AwaitedTask Task,void* OutValue)
	{
		_AwaitGetValue(*this,Task, OutValue);
	}
	void AwaitFreeTask(AwaitedTask Task)
	{
		_AwaitFreeTask(*this, Task);
	}

	UCodeLangForceinline void AddLib(RunTimeLib* lib)
	{
		_Data.AddLib(lib);
	}
	UCodeLangForceinline void ClearsLibs()
	{
		_Data.ClearLibs();
	}
	UCodeLangForceinline void ClearRunTimeState()
	{
		_Data.ClearRunTimeState();
	}

	void LinkLibs();
	UCodeLangForceinline Optional<UAddress> FindAddress(const String& FunctionName) const
	{
		return _Data.FindAddress(FunctionName);
	}
	UCodeLangForceinline Optional<const RunTimeLib::CPPCall*> FindCPPCall(const String& FunctionName) const
	{
		return _Data.FindCPPCall(FunctionName);
	}
	UCodeLangForceinline Instruction& GetInst(UAddress address)
	{
		return _Data.GetInst(address);
	}
	UCodeLangForceinline const void* GetNativeCode(UAddress address) const
	{
		return &_Data.Get_Code()[address];
	}
	UCodeLangForceinline const void* GetNativeCodeEx(UAddress address) const
	{
		return &((Byte*)_Data._ExCode.Data)[address];
	}
	UCodeLangForceinline PtrType Get_StaticMemPtr()
	{
		return _StaticMemPtr;
	}
	
	UCodeLangForceinline const ClassAssembly& Get_Assembly()
	{
		return _Data.Get_Assembly();
	}
	UCodeLangForceinline UCLibManger& Get_Libs()
	{
		return _Data;
	}
	UCodeLangForceinline const UserMadeContext& Get_UserMadeContext()
	{
		return _UserMadeContext;
	}
	UCodeLangForceinline void Set_UserMadeContext(UserMadeContext Context)
	{
		_UserMadeContext = Context;
	}


	struct HotReloadObject
	{
		ReflectionTypeInfo Type;
		void* Pointer =nullptr;
	};
	struct HotReloadLib
	{
		RunTimeLib* NewLib = nullptr;
		RunTimeLib* LibToUpdate = nullptr;
	};
	struct HotReloadData
	{
		Vector<HotReloadLib> LibsToUpdate;

		//pass in all UCode Objects.
		Vector<HotReloadObject> Objects;

		//
		Vector<DebugContext::InterpreterInfo> Interpreters;
		size_t IndexOfInterpreterToCallForRelocations = 0;
	};
	

	//the types that have been updated
	struct Diffs
	{

	};
	bool HotReload(const HotReloadData& Item,TaskManger& tasks);
	bool HotReload(const HotReloadData& Item)
	{
		TaskManger task;
		task.Init();
		return HotReload(Item, task);
	}
	const ClassMethod* GetMethod(const UAddress& address);
	String GetName(UAddress address)  const
	{
		return _Data.GetName(address);
	}

	const DebugContext& Get_DebugContext() const
	{
		return _Debug;
	}
	DebugContext& Get_DebugContext()
	{
		return _Debug;
	}

	//Set CallBacks
	void Set_Malloc(Allocator::Malloc_t CallBack)
	{
		_Allocator.Set_Malloc(CallBack);
	}
	void Set_Free(Allocator::Free_t CallBack)
	{
		_Allocator.Set_Free(CallBack);
	}
	void Set_Log(LogCallBack CallBack)
	{
		_Log = CallBack;
	}
	void Set_Read(ReadCharCallBack CallBack)
	{
		_Read = CallBack;
	}
	void Set_AwaitNew(AwaitNawTaskCallBack CallBack)
	{
		_AwaitNewTask = CallBack;
	}
	void Set_AwaitFree(AwaitFreeTaskCallBack CallBack)
	{
		_AwaitFreeTask = CallBack;
	}
	void Set_AwaitIsDone(AwaitIsDoneCallBack CallBack)
	{
		_AwaitIsDone = CallBack;
	}
	void Set_AwaitGetValue(AwaitGetValueCallBack CallBack)
	{
		_AwaitGetValue = CallBack;
	}
private:
	Allocator _Allocator;
	UCLibManger _Data;
	PtrType _StaticMemPtr;
	LogCallBack _Log;
	ReadCharCallBack _Read;

	AwaitNawTaskCallBack _AwaitNewTask;
	AwaitFreeTaskCallBack _AwaitFreeTask;
	AwaitIsDoneCallBack _AwaitIsDone;
	AwaitGetValueCallBack _AwaitGetValue;

	UserMadeContext _UserMadeContext;
	DebugContext _Debug;
};
UCodeLangEnd
