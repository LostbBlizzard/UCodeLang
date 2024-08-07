#include "RunTimeLangState.hpp"
#include "AnyInterpreter.hpp"
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
		_Data.AddValue(r,{ false, Size });
		return r;
	}
}
void Allocator::Free(RunTimeLangState& This, PtrType ptr)
{
	if (ptr ==nullptr) { return; }


	auto& M = _Data.GetValue(ptr);
	if (_CanReserveData)
	{
		


		_ReservedData.AddValue(ptr,{ M });
		_Data.erase(ptr);

		return;
	}

	if (!M.IsfakePtr) {_Free(This, ptr);}
	_Data.erase(ptr);
}
PtrType Allocator::Realloc(RunTimeLangState& This, PtrType oldptr, NSize_t Size)
{
	auto& Data = _Data.GetValue(oldptr);
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
	_ReservedData.AddValue(r,{false, Size });
}
void Allocator::FreeAllAllocations(RunTimeLangState& This)
{
	for (auto& Item : _Data)
	{
		PtrType ptr = Item.first;
		auto& M = _Data.GetValue(ptr);
		if (_CanReserveData)
		{
			
			_ReservedData.AddValue(ptr,{ M });
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
		auto& Item = *it;

		
		auto& ItemPtr = Item.first;
		auto& ItemValue = Item.second;
	
		NSize_t ItemSize = ItemValue.Size;
		NSize_t BuffSize = ItemSize;
		void* Ptr = (void*)((uintptr_t)ItemPtr + (uintptr_t)ItemSize);
		Tep_Values.push_back(ItemPtr);
		while (true)
		{
			if (_ReservedData.HasValue(Ptr))
			{
				auto& NewData = _ReservedData.GetValue(Ptr);
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
		auto& Itemfirst = Item.first;
		auto& Itemsecond = Item.second;

		if (Itemsecond.Size >= Size)
		{
			auto Ptr = Itemfirst;


			_Data.AddValue(Ptr,{ Itemsecond.IsfakePtr, Size });

			auto Sizediff = Itemsecond.Size - Size;
			if (Sizediff != 0)
			{
				void* subptr = (void*)((uintptr_t)Ptr + (uintptr_t)Sizediff);
				_ReservedData.AddValue(subptr,{ true,Sizediff });
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
				if (_ReservedData.HasValue(Ptr))
				{
					auto& NewData = _ReservedData.GetValue(Ptr);
					BuffSize += NewData.Size;
					Tep_Values.push_back(Ptr);
					if (BuffSize >= Size)
					{
						_Data.AddValue(Ptr,{ Itemsecond.IsfakePtr, Size });
						auto Sizediff = BuffSize - Size;
						if (Sizediff != 0)
						{
							void* subptr = (void*)((uintptr_t)Ptr + (uintptr_t)Sizediff);
							_ReservedData.AddValue(subptr,{ true,Sizediff });
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

	memcpy(_StaticMemPtr, Bits.data(), Bits.size());
}

AnyInterpreterPtr As(DebugContext::InterpreterInfo Info)
{
	switch (Info.type)
	{
	case DebugContext::Type::Interpreter:
	{
		return (Interpreter*)Info.ThisInterpreter;
	}
	break;
	case DebugContext::Type::Jit_Interpreter:
	{
		return (Jit_Interpreter*)Info.ThisInterpreter;
	}
	break;
	case DebugContext::Type::Native_Interpreter:
	{
		return (NativeInterpreter*)Info.ThisInterpreter;
	}
	break;
	default:
		UCodeLangUnreachable();
		break;
	}
}

bool RunTimeLangState::HotReload(const HotReloadData& Item,TaskManger& tasks)
{
	return false;

	struct StaticObjectInfo
	{
		struct State
		{
			size_t Offset = 0;
			ReflectionTypeInfo Type;

			size_t LineDeclared;
			String FileDeclared;
		};
		Optional<State> OldRunTimeState;
		Optional<State> NewRunTimeState;
	};
	struct ThreadObjectInfo
	{
		using State = StaticObjectInfo::State;
		Optional<State> OldRunTimeState;
		Optional<State> NewRunTimeState;
	};
	struct UBytePtr
	{
		UBytePtr()
		{
			this->State = nullptr;
		}
		UBytePtr(RunTimeLangState* State)
		{
			SetState(State);
		}
		UBytePtr(UBytePtr&& Other) noexcept
		{
			SetState(Other.State);

			_Size = Other._Size;
			_Ptr = Other.Release();

		}
		UBytePtr& operator=(UBytePtr&& Other) noexcept
		{
			TryFree();

			SetState(Other.State);
			
			_Size = Other._Size;
			_Ptr = Other.Release();
			return *this;
		}

		void SetState(RunTimeLangState* State)
		{
			this->State = State;
		}

		void Resize(size_t Size)
		{
			if (_Ptr)
			{
				State->Free(_Ptr);
			}
			_Ptr = State->Malloc(Size);
			_Size = Size;
		}
		PtrType Data()
		{
			return _Ptr;
		}
		const PtrType Data() const
		{
			return _Ptr;
		}
		size_t Size() const
		{
			return _Size;
		}
		Vector<Byte> CopyAsVector()
		{
			Vector<Byte> r;
			r.resize(_Size);
			memcpy(r.data(), _Ptr, _Size);

			return r;
		}

		~UBytePtr()
		{
			TryFree();
		}

		PtrType Release()
		{
			auto r = _Ptr;
			_Ptr = nullptr;
			_Size = 0;

			return r;
		}

		
	private:
		RunTimeLangState* State = nullptr;
		PtrType _Ptr=nullptr;
		size_t _Size =0;
		void TryFree()
		{
			if (_Ptr)
			{
				State->Free(_Ptr);
			}
		}
	};
	struct GlobalInfo
	{
		UBytePtr NewStaticMem;
		Vector<Byte> ReadOnlyStaticMem;

		UnorderedMap<String,StaticObjectInfo> StaticObjectsInfo;
		UnorderedMap<String,ThreadObjectInfo> ThreadObjectsInfo;

		Vector<Instruction> NewIns;
		ULangDebugInfo NewDebugInfo;
	};
	struct InterpreterLocal
	{
		BytesView RunTimeThreadValues;
		UBytePtr NewThreadMem;

		Vector<Byte> ReadOnlyThreadMem;

		AnyInterpreterPtr _Ptr;
	};
	struct LibInfo
	{
		size_t OldInsOffset = 0;
		size_t OldStaticOffset = 0;
		size_t OldThreadOffset = 0;

		size_t NewInsOffset = 0;
		size_t NewStaticOffset = 0;
		size_t NewThreadOffset = 0;

		BytesView RunTimeStaticValues;

		
		Vector<InterpreterLocal> InterpreterLocals;
	};

	GlobalInfo GlobalInfo;
	Vector<LibInfo> LibsInfo;
	LibsInfo.reserve(Item.LibsToUpdate.size());


	auto& RunTimeDebugInfo = Get_Libs().Get_DebugInfo();
	{//Get RunTimeInfo
		size_t NewStaticBufferSize = 0;
		size_t NewThreadBufferSize = 0;
		size_t NewInsSize = 0;

		size_t OldStaticBufferSize = 0;
		size_t OldThreadBufferSize = 0;
		size_t OldInsSize = 0;
		for (auto& Lib : Item.LibsToUpdate)
		{
			const auto& NewStaticBytes = Lib.NewLib->Get_Lib()->Get_StaticBytes();
			const auto& NewThreadBytes = Lib.NewLib->Get_Lib()->Get_ThreadBytes();
			const auto& NewIns = Lib.NewLib->Get_Instructions();
			
			const auto& OldStaticBytes = Lib.LibToUpdate->Get_Lib()->Get_StaticBytes();
			const auto& OldThreadBytes = Lib.LibToUpdate->Get_Lib()->Get_ThreadBytes();
			const auto& OldIns = Lib.LibToUpdate->Get_Instructions();

			
			
			LibInfo V;
			V.NewStaticOffset = NewStaticBufferSize;
			V.NewThreadOffset = NewThreadBufferSize;
			V.NewInsOffset = NewInsSize;

			V.OldStaticOffset = OldStaticBufferSize;
			V.OldThreadOffset = OldThreadBufferSize;
			V.OldInsOffset = OldInsSize;

			NewStaticBufferSize += NewStaticBytes.size();
			NewThreadBufferSize += NewThreadBytes.size();
			NewInsSize += NewIns.size();

			OldStaticBufferSize += OldStaticBytes.size();
			OldThreadBufferSize += OldThreadBytes.size();
			OldInsSize += OldIns.size();

			{
				if (auto Val = Lib.NewLib->Get_Lib()->GetLayer(UCode_CodeLayer_UCodeVM_Name))
				{
					if (auto bytecode = Val->_Data.Get_If<CodeLayer::UCodeByteCode>())
					{
						if (bytecode->DebugInfo.has_value())
						{
							auto& DebugInfo = bytecode->DebugInfo.value();
							
							for (auto& Item : DebugInfo.VarablesInfo)
							{
								GlobalInfo.NewDebugInfo.VarablesInfo.AddValue(Item.first, Item.second);
							}
							for (auto& Item : DebugInfo.DebugInfo)
							{
								auto Copy = Item;
								Copy.Set_Ins(Item.Get_Ins().value_or(0) + V.NewInsOffset);
								GlobalInfo.NewDebugInfo.DebugInfo.push_back(Copy);
							}
						}
					}
				}
			}
			LibsInfo.push_back(std::move(V));
		}
		auto& RunTimeStaticBytes = Get_Libs().GetStaticBytes();

		GlobalInfo.NewStaticMem.SetState(this);
		GlobalInfo.NewStaticMem.Resize(RunTimeStaticBytes.size());
		GlobalInfo.NewIns.resize(NewInsSize);

		for (auto& Item : RunTimeDebugInfo.VarablesInfo)
		{
			if (auto Val =Item.second.TypeLoc.Get_If<VarableInfo::Static>())
			{
				StaticObjectInfo::State oldstate;
				oldstate.Offset = Val->offset;
				oldstate.Type = Item.second.ReflectionType;

				StaticObjectInfo b;
				b.OldRunTimeState = std::move(oldstate);

				GlobalInfo.StaticObjectsInfo.AddValue(Item.first,std::move(b));
			}
			else if (auto Val = Item.second.TypeLoc.Get_If<VarableInfo::Thread>())
			{
				ThreadObjectInfo::State oldstate;
				oldstate.Offset = Val->offset;
				oldstate.Type = Item.second.ReflectionType;

				ThreadObjectInfo b;
				b.OldRunTimeState = std::move(oldstate);

				GlobalInfo.ThreadObjectsInfo.AddValue(Item.first, std::move(b));
			}
		}
		for (auto& Item : GlobalInfo.NewDebugInfo.VarablesInfo)
		{
			if (auto Val = Item.second.TypeLoc.Get_If<VarableInfo::Static>())
			{
				StaticObjectInfo::State oldstate;
				oldstate.Offset = Val->offset;
				oldstate.Type = Item.second.ReflectionType;
				oldstate.FileDeclared = Item.second.FileDeclaredIn;
				oldstate.LineDeclared = Item.second.DeclaredLine;

				StaticObjectInfo b =std::move(GlobalInfo.StaticObjectsInfo.GetValue(Item.first));
				b.NewRunTimeState = std::move(oldstate);

				GlobalInfo.StaticObjectsInfo.AddValue(Item.first,std::move(b));
			}
			else if (auto Val = Item.second.TypeLoc.Get_If<VarableInfo::Thread>())
			{
				ThreadObjectInfo::State oldstate;
				oldstate.Offset = Val->offset;
				oldstate.Type = Item.second.ReflectionType;
				oldstate.FileDeclared = Item.second.FileDeclaredIn;
				oldstate.LineDeclared = Item.second.DeclaredLine;

				ThreadObjectInfo b = std::move(GlobalInfo.ThreadObjectsInfo.GetValue(Item.first));
				b.NewRunTimeState = std::move(oldstate);

				GlobalInfo.ThreadObjectsInfo.AddValue(Item.first,std::move(b));
			}
		}

		for (size_t i = 0; i < Item.LibsToUpdate.size(); i++)
		{
			auto& Lib = Item.LibsToUpdate[i];
			auto& LibInfo = LibsInfo[i];
			const auto& NewStaticBytes = Lib.LibToUpdate->Get_Lib()->Get_StaticBytes();
			const auto& NewIns = Lib.NewLib->Get_Instructions();

			memcpy((Byte*)GlobalInfo.NewStaticMem.Data() + LibInfo.NewStaticOffset, NewStaticBytes.data(), NewStaticBytes.size());
			
			memcpy(GlobalInfo.NewIns.data() + LibInfo.NewInsOffset, NewIns.data(), NewIns.size()*sizeof(Instruction));

			for (auto& InterpreterV : Item.Interpreters)
			{
				const auto& NewThreadBytes = Lib.LibToUpdate->Get_Lib()->Get_ThreadBytes();
				InterpreterLocal Val;
				Val.NewThreadMem.SetState(this);
				
				//this is wrong
				Val.NewThreadMem.Resize(LibInfo.NewThreadOffset);
				memcpy((Byte*)Val.NewThreadMem.Data(), NewThreadBytes.data(), NewThreadBytes.size());
				
				Val._Ptr = As(InterpreterV);

				Val.RunTimeThreadValues = BytesView::Make((Byte*)Val._Ptr.GetThreadPtr(),Get_Libs().GetThreadBytes().size());


				Val.ReadOnlyThreadMem = Val.NewThreadMem.CopyAsVector();

				LibInfo.InterpreterLocals.push_back(std::move(Val));

			}

			

		}

		GlobalInfo.ReadOnlyStaticMem = GlobalInfo.NewStaticMem.CopyAsVector();
		
	}
	{//Move Values  Constructors and destructors

		bool Is32Bit = sizeof(void*) == 4;
		
		auto InterpreterWithNewState = LibsInfo[0].InterpreterLocals[Item.IndexOfInterpreterToCallForRelocations]._Ptr;
		auto InterpreterWithOldState = InterpreterWithNewState;

		auto Func = [this, InterpreterWithNewState, InterpreterWithOldState, Is32Bit](Optional<StaticObjectInfo::State>& OldRunTimeState,
		Optional<StaticObjectInfo::State>& NewRunTimeState, void* OldBasePtr,void* NewBasePtr) mutable
		{
			bool HasNew = NewRunTimeState.has_value();
			bool HasOld = OldRunTimeState.has_value();

			auto& NewAssembly = Get_Assembly();
			auto& OldAssembly = Get_Assembly();


			if (HasNew && HasOld)//StillExist:Move Value
			{
				auto& NewState = NewRunTimeState.value();
				auto& OldState = OldRunTimeState.value();

				bool IsTypeTheSame = true;

				void* NewObject =(void*)((uintptr_t)NewBasePtr + NewState.Offset);
				void* OldObject =(void*)((uintptr_t)OldBasePtr + OldState.Offset);

				auto op = ClassAssembly::CompareType(OldState.Type, Get_Assembly(), NewState.Type, Get_Assembly());

				if (op == ClassAssembly::CompareType_t::Identical)
				{
					auto ItWorked = Get_Assembly().CallMoveConstructor(NewState.Type, OldObject, NewObject, Is32Bit);
					if (!ItWorked.has_value())
					{
						return false;
					}
					if (ItWorked.value().has_value())
					{
						for (auto& call : ItWorked.value().value())
						{
							InterpreterWithNewState.PushParameter(call.ThisPtr);
							InterpreterWithNewState.PushParameter(call.OtherPtr);
							InterpreterWithNewState.Call(call.MethodToCall);
						}
					}

				}
				else if (op == ClassAssembly::CompareType_t::TooDifferent)
				{
					{
						auto ItWorked = Get_Assembly().CallDestructor(OldState.Type, OldObject, Is32Bit);
						if (!ItWorked.has_value())
						{
							return false;
						}
						if (ItWorked.value().has_value())
						{
							for (auto& call : ItWorked.value().value())
							{
								InterpreterWithOldState.PushParameter(call.ThisPtr);
								InterpreterWithOldState.Call(call.MethodToCall);
							}
						}
					}

					{
						auto ItWorked = Get_Assembly().CallDefaultConstructor(NewState.Type, NewObject, Is32Bit);
						if (!ItWorked.has_value())
						{
							return false;
						}
						if (ItWorked.value().has_value())
						{
							for (auto& call : ItWorked.value().value())
							{
								InterpreterWithNewState.PushParameter(call.ThisPtr);
								InterpreterWithNewState.Call(call.MethodToCall);
							}
						}
					}
				}
				else//transmute type
				{
					auto ItWorked = ClassAssembly::DoTypeCoercion(OldState.Type, OldObject
						,OldAssembly,NewState.Type, NewObject,NewAssembly, Is32Bit);
					if (!ItWorked.has_value())
					{
						return false;
					}
					if (ItWorked.value().has_value())
					{
						for (auto& call : ItWorked.value().value())
						{
							InterpreterWithNewState.PushParameter(call.ThisPtr);
							InterpreterWithNewState.PushParameter(call.OtherPtr);
							InterpreterWithNewState.Call(call.MethodToCall);
						}
					}

				}
			}
			else if (HasNew == true && HasOld == false)//Added:Make Value
			{
				auto& NewState = NewRunTimeState.value();
				void* NewObject = (void*)((uintptr_t)NewBasePtr + NewState.Offset);

				
				bool FoundItsExpression = false;
				if (FoundItsExpression)
				{

				}
				else
				{
					auto ItWorked = Get_Assembly().CallDefaultConstructor(NewState.Type, NewObject, Is32Bit);
					if (!ItWorked.has_value())
					{
						return false;
					}
					if (ItWorked.value().has_value())
					{
						for (auto& call : ItWorked.value().value())
						{
							InterpreterWithNewState.PushParameter(call.ThisPtr);
							InterpreterWithNewState.Call(call.MethodToCall);
						}
					}
				}
			}
			else if (HasNew == false && HasOld == true)//Removed:Remove Value
			{
				auto& OldState = OldRunTimeState.value();
				void* OldObject = (void*)((uintptr_t)OldBasePtr + OldState.Offset);


				auto ItWorked = Get_Assembly().CallDestructor(OldState.Type, OldObject, Is32Bit);
				if (!ItWorked.has_value())
				{
					return false;
				}
				if (ItWorked.value().has_value())
				{
					for (auto& call : ItWorked.value().value())
					{
						InterpreterWithOldState.PushParameter(call.ThisPtr);
						InterpreterWithOldState.Call(call.MethodToCall);
					}
				}
			}

			return true;
		};

		for (auto& Item : GlobalInfo.StaticObjectsInfo)
		{
			if (!Func(Item.second.OldRunTimeState, Item.second.NewRunTimeState, _StaticMemPtr, GlobalInfo.NewStaticMem.Data()))
			{
				return false;
			}
		}

		auto& LibInfo = LibsInfo[0];
		for (size_t i = 0; i < Item.Interpreters.size(); i++)
		{
			InterpreterLocal& Loc = LibInfo.InterpreterLocals[i];

			for (auto& Item : GlobalInfo.ThreadObjectsInfo)
			{
				if (!Func(Item.second.OldRunTimeState, Item.second.NewRunTimeState, Loc.RunTimeThreadValues.Data(), Loc.NewThreadMem.Data()))
				{
					return false;
				}
			}
		}
	}

	{//Replace Static and Thread buffers 
		Free(_StaticMemPtr);
		_StaticMemPtr = GlobalInfo.NewStaticMem.Release();

		//_Data._Instructions = std::move(GlobalInfo.NewIns);



		auto& LibInfo = LibsInfo[0];
		for (size_t i = 0; i < Item.Interpreters.size(); i++)
		{
			InterpreterLocal& Loc = LibInfo.InterpreterLocals[i];

			Free(Loc._Ptr.GetThreadPtr());
			if (Loc._Ptr.Get_InterpreterType() == InterpreterTypes::Interpreter)
			{
				auto Interpreter = Loc._Ptr.GetAs_Interpreter();

				Interpreter->ResetThreadPointer(Loc.NewThreadMem.Release());
			}
			else if (Loc._Ptr.Get_InterpreterType() == InterpreterTypes::NativeInterpreter)
			{

			}
			else
			{
				UCodeLangThrowException("not added");
			}
		}
		this->_Data.Get_DebugInfo() =std::move(GlobalInfo.NewDebugInfo);

		_Data.StaticBytes = GlobalInfo.ReadOnlyStaticMem;
		_Data.ThreadBytes = LibsInfo[0].InterpreterLocals[0].ReadOnlyThreadMem;
	}

	_Data.HotReloadClearState();

	for (auto& runtimelib : _Data.Libs)
	{
		for (auto& Lib : Item.LibsToUpdate)
		{
			if (Lib.LibToUpdate == runtimelib)
			{
				runtimelib = Lib.NewLib; 
				break;
			}
		}
	}
	for (auto& runtimelib : _Data.Libs)
	{
		_Data.LinkLib(runtimelib,true);
	}

	
	return true;
}

const ClassMethod* RunTimeLangState::GetMethod(const UAddress& address) 
{
	for (auto& node : Get_Assembly().Classes)
	{
		if (node->Get_Type() == ClassType::Class)
		{
			auto& Class = node->Get_ClassData();
			for (auto& Item : Class.Methods)
			{
				auto FuncAddress = FindAddress(Item.DecorationName);
				if (FuncAddress == address)
				{
					return &Item;
				}
			}
		}
	}
	return nullptr;
}

void PackagedTask::Invoke(InterpreterInfo& This)
{
	auto _This = As(This);
	auto State = _Parameters.StartLoop();
	while (_Parameters.Next(State))
	{
		size_t Size = _Parameters.GetSize(State);
		const void* Pointer = _Parameters.GetPointer(State);
		_This.PushParameter(Pointer, Size);
	}
	_This.Call(Func);
}

void PackagedTask::RInvoke(InterpreterInfo& This, void* OutObject, size_t ReturnObjectSize)
{
	auto _This = As(This);
	auto State = _Parameters.StartLoop();
	while (_Parameters.Next(State))
	{
		size_t Size = _Parameters.GetSize(State);
		const void* Pointer = _Parameters.GetPointer(State);
		_This.PushParameter(Pointer, Size);
	}
	_This.Call(Func);

	_This.Get_Return(OutObject, ReturnObjectSize);
}

UCodeLangEnd
