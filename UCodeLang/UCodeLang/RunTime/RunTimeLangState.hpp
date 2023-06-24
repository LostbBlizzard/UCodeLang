#pragma once
#include "../LangCore.hpp"
#include "UCLibManger.hpp"
#include <stdlib.h>
#include <iostream>
#include "Jit/Jit.hpp"
#include "UCodeLang/LangCore/DataType/BinaryVectorMap.hpp"
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
private:
	Malloc_t _Malloc;
	Free_t _Free;
	bool _CanReserveData;
	bool _MallocOnlyInPages;
	struct MemData
	{
		bool IsfakePtr;
		NSize_t Size;
	};
	VectorMap<PtrType, MemData> _Data;
	VectorMap<PtrType, MemData> _ReservedData;
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
	void* _Ptr;
};

class RunTimeLangState
{
public:
	typedef void(*LogCallBack)(RunTimeLangState& This,const char* Text,size_t Size);
	typedef char(*ReadCharCallBack)(RunTimeLangState& This);

	static PtrType Default_Malloc(RunTimeLangState& This, NSize_t Size)
	{
		return malloc(Size);
	}
	static void Default_Free(RunTimeLangState& This, PtrType Ptr)
	{
		return free(Ptr);
	}

	RunTimeLangState():_Allocator(Default_Malloc,Default_Free), _StaticMemPtr(nullptr), _Log(nullptr),_Read(nullptr)
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
		for (NSize_t i = 0; i < Size; i++)
		{
			Ptr[i] = 0;
		}
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
	void Log(const char* ptr,size_t Size)
	{
		if (_Log)
		{
			_Log(*this, ptr, Size);
		}
		else
		{
			std::cout.write(ptr, Size);
		}
	}

	char ReadChar()
	{
		if (_Read)
		{
			return _Read(*this);
		}
		else
		{
			char Value;
			std::cin.read(&Value,1);
			return Value;
		}
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
	struct HotReloadData
	{
		//pass in all UCode Objects.
		Vector<HotReloadObject> Objects;

		//

	};
	struct HotReloadLib
	{
		const RunTimeLib* NewLib = nullptr;
		RunTimeLib* LibToUpdate = nullptr;
	};

	//the types that have been updated
	struct Diffs
	{

	};
	Optional<RunTimeLangState::Diffs> HotReload(const HotReloadData& Item, const HotReloadLib& Data);
	Optional<RunTimeLangState::Diffs> HotReload(const HotReloadData& Item, const Vector<HotReloadLib> LibsToUpdate);

	RunTimeLangState::Diffs HotReloadOrReset(const HotReloadData& Item, const HotReloadLib& LibsToUpdate);
	RunTimeLangState::Diffs HotReloadOrReset(const HotReloadData& Item, const Vector<HotReloadLib> LibsToUpdate);

	const ClassMethod* GetMethod(const UAddress& address);

private:
	Allocator _Allocator;
	UCLibManger _Data;
	PtrType _StaticMemPtr;
	LogCallBack _Log;
	ReadCharCallBack _Read;

	UserMadeContext _UserMadeContext;
};
UCodeLangEnd
