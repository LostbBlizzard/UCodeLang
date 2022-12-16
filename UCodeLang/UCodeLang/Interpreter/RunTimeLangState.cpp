#include "RunTimeLangState.hpp"
UCodeLangStart

Allocator::Allocator(Malloc_t malloc, Free_t free) :
	_Malloc(malloc), _Free(free), _CanReserveData(false), _MallocOnlyInPages(true)
{
	Tep_Values.reserve(10);
}
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
		PtrType ptr = Item.first;
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
		if (!Item.second.IsfakePtr)
		{
			_Free(This, Item.first);
		}
	}
	_ReservedData.clear();
}

void Allocator::MergeReservedAllocations()//This no work
{
	for (auto it = _ReservedData.begin();it != _ReservedData.end();)
	{
		bool ReMoved = false;
		auto & Item = *it;
	
		NSize_t ItemSize = Item.second.Size;
		NSize_t BuffSize = ItemSize;
		void* Ptr = (void*)((uintptr_t)Item.first + (uintptr_t)ItemSize);
		Tep_Values.push_back(Item.first);
		while (true)
		{
			if (_ReservedData.count(Ptr))
			{
				auto& NewData = _ReservedData.at(Ptr);
				BuffSize += NewData.Size;
				Tep_Values.push_back(Ptr);
				Ptr = (void*)((uintptr_t)Item.first + (uintptr_t)NewData.Size);
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
		if (Item.second.Size >= Size)
		{
			auto Ptr = Item.first;


			_Data[Ptr] = { Item.second.IsfakePtr, Size };

			auto Sizediff = Item.second.Size - Size;
			if (Sizediff != 0)
			{
				void* subptr = (void*)((uintptr_t)Ptr + (uintptr_t)Sizediff);
				_ReservedData[subptr] = { true,Sizediff };
			}
			_ReservedData.erase(Ptr);
			return Ptr;
		}
		else
		{
			NSize_t ItemSize = Item.second.Size;
			NSize_t BuffSize = ItemSize;
			void* Ptr = (void*)((uintptr_t)Item.first - (uintptr_t)ItemSize);
			Tep_Values.push_back(Item.first);
			while (true)
			{
				if (_ReservedData.count(Ptr))
				{
					auto& NewData = _ReservedData.at(Ptr);
					BuffSize += NewData.Size;
					Tep_Values.push_back(Ptr);
					if (BuffSize >= Size)
					{
						_Data[Ptr] = { Item.second.IsfakePtr, Size };
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
					Ptr = (void*)((uintptr_t)Item.first - (uintptr_t)NewData.Size);
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

UCodeLangEnd