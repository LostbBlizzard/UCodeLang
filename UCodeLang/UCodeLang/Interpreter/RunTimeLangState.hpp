#pragma once
#include "../LangCore.hpp"
#include "UCLibManger.hpp"
#include <stdlib.h>
#include <iostream>
#include "../Jit.hpp"
UCodeLangStart

class RunTimeLangState;
class Allocator
{
public:
	typedef PtrType(*Malloc_t)(RunTimeLangState& This, NSize_t Size);
	typedef void (*Free_t)(RunTimeLangState& This, PtrType ptr);
	
	Allocator(Malloc_t malloc, Free_t free);
	PtrType Malloc(RunTimeLangState& This, NSize_t Size);
	void Free(RunTimeLangState& This, PtrType ptr);
	PtrType Realloc(RunTimeLangState& This, PtrType oldptr, NSize_t Size);
	void ReservedBytes(RunTimeLangState& This, NSize_t Size);
	
	void FreeAllAllocations(RunTimeLangState& This);
	void FreeAllReservedAllocations(RunTimeLangState& This);
	void MergeReservedAllocations();

	inline void Set_Malloc(Malloc_t V)
	{
		_Malloc = V;
	}
	inline void Set_Free(Free_t  V)
	{
		_Free = V;
	}
	inline void Set_CanReserveData(bool V)
	{
		_CanReserveData = true;
	}
	inline void Set_MallocOnlyInPages(bool V)
	{
		_MallocOnlyInPages = true;
	}
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
	unordered_map<PtrType, MemData> _Data;
	unordered_map<PtrType, MemData> _ReservedData;
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
	typedef void(*LogCallBack)(RunTimeLangState& This,const char* Text);

	static PtrType Default_Malloc(RunTimeLangState& This, NSize_t Size)
	{
		return malloc(Size);
	}
	static void Default_Free(RunTimeLangState& This, PtrType Ptr)
	{
		return free(Ptr);
	}

	RunTimeLangState():_Allocator(Default_Malloc,Default_Free), _StaticMemPtr(nullptr), _Log(nullptr)
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
	inline PtrType Malloc(NSize_t Size) {return _Allocator.Malloc(*this,Size); }
	inline void Free(PtrType Ptr) { _Allocator.Free(*this, Ptr); }
	inline PtrType Realloc(PtrType Ptr, NSize_t Size) {return _Allocator.Realloc(*this, Ptr, Size);}
	inline void ReservedBytes(NSize_t Size){_Allocator.ReservedBytes(*this, Size);}
	inline Allocator& Get_Allocator()
	{
		return _Allocator;
	}
	
	inline void Log(const char* ptr)
	{
		if (_Log) 
		{
			_Log(*this,ptr);
		}
		else
		{
			std::cout << "ULang::Log:" << ptr << std::endl;
		}
	}
	//
	
	inline void AddLib(RunTimeLib* lib)
	{
		_Data.AddLib(lib);
	}
	inline void Clears()
	{
		_Data.Clears();
	}
	inline void LinkLibs()
	{
		_Data.Link();
		
		
		if (_StaticMemPtr) { Free(_StaticMemPtr); }
		auto& Bits = _Data.GetStaticBytes();
		_StaticMemPtr = Malloc(Bits.size());
		
		UInt8* _StaticMemPtrBits = (UInt8*)_StaticMemPtr;
		for (size_t i = 0; i < Bits.size(); i++)
		{
			_StaticMemPtrBits[i] = Bits[i];
		}
	}
	inline UAddress FindAddress(const String& FunctionName)
	{
		return _Data.FindAddress(FunctionName);
	}
	inline RunTimeLib::CPPCallBack* FindCPPCall(const String& FunctionName)
	{
		return _Data.FindCPPCall(FunctionName);
	}
	inline Instruction& GetInst(UAddress address)
	{
		return _Data.GetInst(address);
	}
	inline PtrType Get_StaticMemPtr()
	{
		return _StaticMemPtr;
	}
	
	inline const ClassAssembly& Get_Assembly()
	{
		return _Data.Get_Assembly();
	}
	inline const UserMadeContext& Get_UserMadeContext()
	{
		return _UserMadeContext;
	}
	inline void Set_UserMadeContext(UserMadeContext Context)
	{
		_UserMadeContext = Context;
	}
private:
	Allocator _Allocator;
	UCLibManger _Data;
	PtrType _StaticMemPtr;
	LogCallBack _Log;

	UserMadeContext _UserMadeContext;
};
UCodeLangEnd
