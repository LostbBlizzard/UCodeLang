#include "RunTimeLangState.hpp"
UCodeLangStart


PtrType Allocator::Malloc(RunTimeLangState& This, NSize_t Size)
{
	if (Size == 0) { return nullptr; }
	if (_CanReserveData)
	{
		auto r = FindReservedPtr(Size);
		if (r != nullptr) 
		{
			return r;
		}


		if (_MallocOnlyInPages)
		{
			const size_t PageSize = Get_EnvironmentData().PageSize;
			ReservedBytes(This, PageSize);

			return FindReservedPtr(Size);
		}
	}
	
	{
		auto r = _Malloc(This, Size);
		_Data[r] = { false, Size };
		return r;
	}
}
void Allocator::Free(RunTimeLangState& This, PtrType ptr)
{
	auto& M = _Data[ptr];
	if (_CanReserveData)
	{
		


		_ReservedData[ptr] = { M };
		_Data.erase(ptr);

		return;
	}

	if (!M.IsfakePtr) {_Free(This, ptr);}
	_Data.erase(ptr);
}
PtrType Allocator::Realloc(RunTimeLangState& This, PtrType oldptr, NSize_t Size)
{
	auto& Data = _Data[oldptr];
	if (Data.Size < Size)
	{
		Free(This, oldptr);
		return Malloc(This, Size);
	}
	return oldptr;
}
void Allocator::ReservedBytes(RunTimeLangState& This, NSize_t Size)
{
	auto r = _Malloc(This, Size);
	_ReservedData[r] = {false, Size };
}
void Allocator::FreeAllAllocations(RunTimeLangState& This)
{
	for (auto& Item : _Data)
	{
		PtrType ptr = Item._Key;
		auto& M = _Data[ptr];
		if (_CanReserveData)
		{
			
			_ReservedData[ptr] = { M };
			continue;
		}
		if (!M.IsfakePtr) { _Free(This, ptr); }
	}
	_Data.clear();
}
void Allocator::FreeAllReservedAllocations(RunTimeLangState& This)
{
	for (auto& Item : _ReservedData)
	{
		if (!Item._Value.IsfakePtr)
		{
			_Free(This, Item._Key);
		}
	}
	_ReservedData.clear();
}

void Allocator::MergeReservedAllocations()//This no work
{
	for (auto it = _ReservedData.begin();it != _ReservedData.end();)
	{
		bool ReMoved = false;
		auto& Item = *it;

		
		auto& ItemPtr = Item._Key;
		auto& ItemValue = Item._Value;
	
		NSize_t ItemSize = ItemValue.Size;
		NSize_t BuffSize = ItemSize;
		void* Ptr = (void*)((uintptr_t)ItemPtr + (uintptr_t)ItemSize);
		Tep_Values.push_back(ItemPtr);
		while (true)
		{
			if (_ReservedData.count(Ptr))
			{
				auto& NewData = _ReservedData.at(Ptr);
				BuffSize += NewData.Size;
				Tep_Values.push_back(Ptr);
				Ptr = (void*)((uintptr_t)ItemPtr + (uintptr_t)NewData.Size);
			}
			else
			{
				if (Tep_Values.size() != 1) 
				{
					for (auto& Items : Tep_Values)
					{
						it = _ReservedData.erase(it);
					}
					
					ReMoved = true;

				}
				Tep_Values.clear();
				break;
			}
		}

		if (!ReMoved)
		{
			it++;
		}
	}
}

PtrType Allocator::FindReservedPtr(NSize_t Size)
{
	for (auto& Item : _ReservedData)
	{
		auto& Itemfirst = Item._Key;
		auto& Itemsecond = Item._Value;

		if (Itemsecond.Size >= Size)
		{
			auto Ptr = Itemfirst;


			_Data[Ptr] = { Itemsecond.IsfakePtr, Size };

			auto Sizediff = Itemsecond.Size - Size;
			if (Sizediff != 0)
			{
				void* subptr = (void*)((uintptr_t)Ptr + (uintptr_t)Sizediff);
				_ReservedData[subptr] = { true,Sizediff };
			}
			_ReservedData.erase(Itemfirst);
			return Ptr;
		}
		else
		{
			NSize_t ItemSize = Itemsecond.Size;
			NSize_t BuffSize = ItemSize;
			void* Ptr = (void*)((uintptr_t)Itemfirst - (uintptr_t)ItemSize);
			Tep_Values.push_back(Itemfirst);
			while (true)
			{
				if (_ReservedData.count(Ptr))
				{
					auto& NewData = _ReservedData.at(Ptr);
					BuffSize += NewData.Size;
					Tep_Values.push_back(Ptr);
					if (BuffSize >= Size)
					{
						_Data[Ptr] = { Itemsecond.IsfakePtr, Size };
						auto Sizediff = BuffSize - Size;
						if (Sizediff != 0)
						{
							void* subptr = (void*)((uintptr_t)Ptr + (uintptr_t)Sizediff);
							_ReservedData[subptr] = { true,Sizediff };
						}
						for (auto& Items : Tep_Values) { _ReservedData.erase(Items); }

						Tep_Values.clear();
						return Ptr;
					}
					Ptr = (void*)((uintptr_t)Itemfirst - (uintptr_t)NewData.Size);
				}
				else
				{
					Tep_Values.clear();
					break;
				}
			}
		}
	}
	return nullptr;
}

void RunTimeLangState::LinkLibs()
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

Optional<RunTimeLangState::Diffs> RunTimeLangState::HotReload(const HotReloadData& Item, const HotReloadLib& Data)
{

	return {};
}

Optional<RunTimeLangState::Diffs> RunTimeLangState::HotReload(const HotReloadData& Item, const Vector<HotReloadLib> LibsToUpdate)
{
	for (auto& Lib : LibsToUpdate)
	{
		if (!HotReload(Item, Lib))
		{
			return {};
		}
	}
	return {};
}

RunTimeLangState::Diffs RunTimeLangState::HotReloadOrReset(const HotReloadData& Item, const HotReloadLib& LibsToUpdate)
{
	Vector<HotReloadLib> V;
	V.push_back(LibsToUpdate);

	return HotReloadOrReset(Item, V);
}

RunTimeLangState::Diffs RunTimeLangState::HotReloadOrReset(const HotReloadData& Item, const Vector<HotReloadLib> LibsToUpdate)
{
	Optional<RunTimeLangState::Diffs> OK = HotReload(Item, LibsToUpdate);

	return OK.value();
}

UCodeLangEnd
