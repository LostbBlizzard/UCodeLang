#include "UCodeBackEnd.hpp"
#include "UCodeLang/Compliation/Helpers/InstructionBuilder.hpp"
#include "UCodeLang/Compliation/Helpers/NameDecoratior.hpp"
UCodeLangStart

#define GenIns(x) ResetIns(); x;
#define GenInsPush(x) ResetIns(); x;	ULib.Add_Instruction(_Ins);



UCodeBackEndObject::UCodeBackEndObject()
{
	
}

UCodeBackEndObject::~UCodeBackEndObject()
{
}


void UCodeBackEndObject::Reset()
{
	this->~UCodeBackEndObject();
	new (this)  UCodeBackEndObject;
}
void UCodeBackEndObject::BuildSymbols()
{
	for (auto& Item : _Input->_Symbols)
	{
		if (Item->SymType == IRSymbolType::StaticVarable
			|| Item->SymType == IRSymbolType::ThreadLocalVarable)
		{
			const IRBufferData* ItemBuf = Item->Get_ExAs<IRBufferData>();
			size_t Offset = _Output->Get_StaticBytes().size();
			size_t ItemSize = ItemBuf->Bytes.size();


			StaticMemoryManager::StaticMemInfo newinfo;
			newinfo.Offset = Offset;
			if (ItemBuf->IsInitialized)
			{
				_Output->AddStaticBytes(ItemBuf->Bytes.data(), ItemSize);
			}
			else
			{
				_Output->AddStaticBytes(ItemBuf->Bytes.data(), ItemSize);
			}


			if (Item->SymType == IRSymbolType::StaticVarable)
			{
				_StaticMemory._List.AddValue(Item->identifier, newinfo);
			}
			else
			{
				_ThreadMemory._List.AddValue(Item->identifier, newinfo);
			}
		}
	}
}
void  UCodeBackEndObject::BuildFuncs()
{
	if (_Input->_StaticInit.HasInstructions()) {
		OnFunc(&_Input->_StaticInit);
	}

	if (_Input->_StaticdeInit.HasInstructions()) {
		OnFunc(&_Input->_StaticdeInit);
	}

	if (_Input->_threadInit.HasInstructions()) {
		OnFunc(&_Input->_threadInit);
	}

	if (_Input->_threaddeInit.HasInstructions()) {
		OnFunc(&_Input->_threaddeInit);
	}
	for (auto& Item : _Input->Funcs)
	{
		OnFunc(Item.get());
	}
}
void UCodeBackEndObject::Build(const IRBuilder* Input)
{
	_Input = Input;
	_Output = &Get_Output();


	UpdateOptimizations();
 
	BuildSymbols();

	BuildFuncs();
	
	DoOptimizations();

	LinkFuncs();
}
void UCodeBackEndObject::UpdateOptimizations()
{
	auto& Setings = Get_Settings();
	bool Debug = (OptimizationFlags_t)Setings._Flags & (OptimizationFlags_t)OptimizationFlags::Debug;
	bool ForSize = (OptimizationFlags_t)Setings._Flags & (OptimizationFlags_t)OptimizationFlags::ForSize;
	bool ForSpeed = (OptimizationFlags_t)Setings._Flags & (OptimizationFlags_t)OptimizationFlags::ForSpeed;
	bool ForMaxSpeed = (OptimizationFlags_t)Setings._Flags & (OptimizationFlags_t)OptimizationFlags::ForMaxSpeed;


	if (Debug == false)
	{
		_Optimizations.InlineFuncionCopys = ForSize;
	}
	else
	{
		if (ForSpeed)
		{
			_Optimizations.ReOderForCacheHit = true;
		}
	}
}
void UCodeBackEndObject::DoOptimizations()
{

	if (_Optimizations.InlineFuncionCopys)
	{
		BinaryVectorMap<size_t, UCodeFunc*> FuncsHash;
		for (auto& Item : Funcs)
		{
			Item->_Hash = UAssembly::UAssembly::BuildHashForSub(Item->_Ins.data(), Item->_Ins.size());
		}

		for (auto& Item : Funcs)
		{
			if (Item->_AutoJumpTo) { continue; }

			size_t Hash = Item->_Hash.value();
			for (auto& Item2 : Funcs)
			{
				if (Item2->_AutoJumpTo) { continue; }
				if (Item.get() != Item2.get())
				{
					size_t Hash2 = Item2->_Hash.value();
					if (Hash == Hash2)
					{
						Item2->_AutoJumpTo = Item.get();

						Item2->_Ins.clear();
						Item2->FuncsToLink.clear();

						InstructionBuilder::Jump(NullAddress, _Ins);

						FuncInsID Jump;
						Jump.Index = 0;
						Jump._FuncID = Item->IRName;
						Item->FuncsToLink.push_back(Jump);
					}
					break;
				}
			}


		}
	}
}
void UCodeBackEndObject::LinkFuncs()
{
	for (auto& Item : FuncsToLink)
	{
		Instruction& Ins = Get_Output().Get_Instructions()[Item.Index];


		UAddress funcpos = NullAddress;
		for (auto& Item2 : _Funcpos)
		{
			if (Item2._FuncID == Item._FuncID)
			{
				funcpos = Item2.Index;
				break;
			}
		}

		if (Ins.OpCode == InstructionSet::Call)
		{
			InstructionBuilder::Call(funcpos, Ins);
		}
		else if (Ins.OpCode == InstructionSet::LoadFuncPtr)
		{
			InstructionBuilder::LoadFuncPtr(funcpos, Ins.Value1.AsRegister, Ins);
		}
		else
		{
			throw std::exception("not added");
		}


	}
}
void UCodeBackEndObject::RegWillBeUsed(RegisterID Value)
{
	auto& Info = _Registers.GetInfo(Value);

	if (Info.Inuse == RegistersManager::RegisterInUse::InUseSybol && IsReferencedAfterThisIndex(Info.IRField))
	{
		Info.Inuse = RegistersManager::RegisterInUse::NotInUse;
		CopyValueToStack(Info.IRField, GetType(Info.IRField), Value);
	}
}
void UCodeBackEndObject::OnFunc(const IRFunc* IR)
{

	//build code
	UAddress FuncStart = _Output->GetLastInstruction() + 1;


	{
		auto V = GetParsLoc(IR->Pars);
		CurrentFuncParPos =std::move(V.ParsPos);
	}


	auto FuncName = _Input->FromID(IR->identifier);
	if (&_Input->_StaticInit == IR)
	{
		FuncName = StaticVariablesInitializeFunc;
	}
	else if(&_Input->_StaticdeInit == IR)
	{
		FuncName = StaticVariablesUnLoadFunc;
	}
	else if (&_Input->_threadInit == IR)
	{
		FuncName = ThreadVariablesInitializeFunc;
	}
	else if (&_Input->_threaddeInit == IR)
	{
		FuncName = ThreadVariablesUnLoadFunc;
	}
		

	size_t LinkedCallsIndex = FuncsToLink.size();

	if (IR->IsCPPCall)
	{
		DoCPPCall(FuncName);
	}



	if (IR->Blocks.size())
	{
		OnBlock(IR->Blocks.front().get());
	}

	_InputPar = RegisterID::StartParameterRegister;

	if (_Stack.Size)
	{
		RegisterID V = RegisterID::A;

		auto Ptr = Get_Settings().PtrSize;
		auto& instr = _Output->Get_Instructions();
		
		InstructionBuilder::IncrementStackPointer(_Ins, V);
		instr.insert(instr.begin() + FuncStart, _Ins);
		
		if (Ptr == IntSizes::Int32)
		{
			InstructionBuilder::Store32(_Ins, V, (UInt32)_Stack.Size);
		}
		else
		{
			InstructionBuilder::Store64(_Ins, V, (UInt64)_Stack.Size);
		}

		instr.insert(instr.begin() + FuncStart, _Ins);


		for (size_t i = LinkedCallsIndex; i < FuncsToLink.size(); i++)
		{
			auto& Item = FuncsToLink[i];
			Item.Index += 2;
		}
	}

	_Stack.Reset();
	_Registers.Reset();
	_Output->Add_NameToInstruction(FuncStart, FuncName);


	Funcpos V;
	V.Index = FuncStart-1;
	V._FuncID = IR->identifier;
	_Funcpos.push_back(V);
}
void UCodeBackEndObject::OnBlock(const IRBlock* IR)
{
	LookingBlock = IR;
	OnBlockBuildCode(IR);
	
}
void UCodeBackEndObject::OnBlockBuildCode(const IRBlock* IR)
{
	struct  InsToUpdate_t
	{
		size_t InsToUpdate;
		size_t Jumpto;
	};
	Vector<InsToUpdate_t> InsToUpdate;
	VectorMap<size_t, UAddress> IRToUCodeIns;

	for (size_t i = 0; i < IR->Instructions.size(); i++)
	{
		auto& Item_ = IR->Instructions[i];
		auto& Item = *Item_;
		Index = i;

		switch (Item.Type)
		{
		case IRInstructionType::LoadNone:
			break;
		case IRInstructionType::Load:
		{
			RegisterID V = LoadOp(Item, Item.Target());
			GiveNameToReg(V, &Item);
		}
		break;
		case IRInstructionType::LoadReturn:
		{
			LoadOpToReg(Item, Item.Target(), RegisterID::OuPutRegister);
		}
		break;
		case IRInstructionType::Reassign:
		{
			StoreValue(Item, Item.Target(), Item.Input());
		}
		break;

		case IRInstructionType::SMult:
		{
			RegisterID V = RegisterID::MathOuPutRegister;
			RegisterID A = LoadOp(Item, Item.A);
			LockRegister(A);
			RegisterID B = LoadOp(Item, Item.B);

			RegWillBeUsed(V);
			switch (Item.ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::MultS8(_Ins, A, B); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::MultS16(_Ins, A, B); PushIns(); break;

			bit32label:
			case IRTypes::i32:InstructionBuilder::MultS32(_Ins, A, B); PushIns(); break;

			bit64label:
			case IRTypes::i64:InstructionBuilder::MultS64(_Ins, A, B); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					goto bit32label;
				}
				else
				{
					goto bit64label;
				}
				break;
			default:
				throw std::exception("not added");
				break;
			}

			UnLockRegister(A);
			GiveNameToReg(V, &Item);
		}
		break;
		case IRInstructionType::UMult:
		{
			RegisterID V = RegisterID::MathOuPutRegister;
			RegisterID A = LoadOp(Item, Item.A);
			LockRegister(A);
			RegisterID B = LoadOp(Item, Item.B);


			RegWillBeUsed(V);
			auto& Object_t = Item.ObjectType;
			switch (Object_t._Type)
			{
			case IRTypes::i8:InstructionBuilder::MultU8(_Ins, A, B); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::MultU16(_Ins, A, B); PushIns(); break;

			bit32label1:
			case IRTypes::i32:InstructionBuilder::MultU32(_Ins, A, B); PushIns(); break;

			bit64label1:
			case IRTypes::i64:InstructionBuilder::MultU64(_Ins, A, B); PushIns(); break;
			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					goto bit32label1;
				}
				else
				{
					goto bit64label1;
				}
				break;
			default:
				throw std::exception("not added");
				break;
			}

			UnLockRegister(A);
			GiveNameToReg(V, &Item);
		}
		break;

		case IRInstructionType::SDiv:
		{
			RegisterID V = RegisterID::MathOuPutRegister;
			RegisterID A = LoadOp(Item, Item.A);
			LockRegister(A);
			RegisterID B = LoadOp(Item, Item.B);


			RegWillBeUsed(V);
			switch (Item.ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::DivS8(_Ins, A, B); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::DivS16(_Ins, A, B); PushIns(); break;

			bit32label2:
			case IRTypes::i32:InstructionBuilder::DivS32(_Ins, A, B); PushIns(); break;

			bit64label2:
			case IRTypes::i64:InstructionBuilder::DivS64(_Ins, A, B); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					goto bit32label2;
				}
				else
				{
					goto bit64label2;
				}
				break;
			default:
				throw std::exception("not added");
				break;
			}

			UnLockRegister(A);
			GiveNameToReg(V, &Item);
		}
		break;
		case IRInstructionType::UDiv:
		{
			RegisterID V = RegisterID::MathOuPutRegister;
			RegisterID A = LoadOp(Item, Item.A);
			LockRegister(A);
			RegisterID B = LoadOp(Item, Item.B);


			RegWillBeUsed(V);
			switch (Item.ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::DivU8(_Ins, A, B); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::DivU16(_Ins, A, B); PushIns(); break;

			bit32label5:
			case IRTypes::i32:InstructionBuilder::DivU32(_Ins, A, B); PushIns(); break;

			bit64label5:
			case IRTypes::i64:InstructionBuilder::DivU64(_Ins, A, B); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					goto bit32label5;
				}
				else
				{
					goto bit64label5;
				}
				break;
			default:
				throw std::exception("not added");
				break;
			}

			UnLockRegister(A);
			GiveNameToReg(V, &Item);
		}
		break;

		case IRInstructionType::Add:
		{
			RegisterID V = RegisterID::MathOuPutRegister;
			RegisterID A = LoadOp(Item, Item.A);
			LockRegister(A);
			RegisterID B = LoadOp(Item, Item.B);


			RegWillBeUsed(V);
			switch (Item.ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::Add8(_Ins, A, B); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::Add16(_Ins, A, B); PushIns(); break;

			bit32label6:
			case IRTypes::i32:InstructionBuilder::Add32(_Ins, A, B); PushIns(); break;

			bit64label6:
			case IRTypes::i64:InstructionBuilder::Add64(_Ins, A, B); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					goto bit32label6;
				}
				else
				{
					goto bit64label6;
				}
				break;
			default:
				throw std::exception("not added");
				break;
			}

			UnLockRegister(A);
			GiveNameToReg(V, &Item);
		}
		break;
		case IRInstructionType::Sub:
		{
			RegisterID V = RegisterID::MathOuPutRegister;
			RegisterID A = LoadOp(Item, Item.A);
			LockRegister(A);
			RegisterID B = LoadOp(Item, Item.B);


			RegWillBeUsed(V);
			switch (Item.ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::Sub8(_Ins, A, B); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::Sub16(_Ins, A, B); PushIns(); break;
			bit32label4:
			case IRTypes::i32:InstructionBuilder::Sub32(_Ins, A, B); PushIns(); break;

			bit64label4:
			case IRTypes::i64:InstructionBuilder::Sub64(_Ins, A, B); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					goto bit32label4;
				}
				else
				{
					goto bit64label4;
				}
				break;
			default:
				throw std::exception("not added");
				break;
			}

			UnLockRegister(A);
			GiveNameToReg(V, &Item);
		}
		break;
		case IRInstructionType::PushParameter:
		{
			RegisterID_t& Reg = *(RegisterID_t*)&_InputPar;

			
			if (Reg < (RegisterID_t)RegisterID::EndParameterRegister)
			{
				LoadOpToReg(Item, Item.Target(), _InputPar);
				LockRegister(_InputPar);
				Reg++;
			}
			else
			{
				PushOpStack(Item, Item.Target());
			}
		}
		break;
		case IRInstructionType::Call:
		{
			auto FuncInfo = _Input->GetFunc(Item.Target().identifer);
			auto FData = FuncCallStart(FuncInfo->Pars, FuncInfo->ReturnType);
			
			InstructionBuilder::Call(NullAddress, _Ins); PushIns();

			FuncInsID Tep;
			Tep.Index = _Output->Get_Instructions().size() - 1;
			Tep._FuncID = Item.Target().identifer;

			FuncsToLink.push_back(Tep);

		

			FuncCallEnd(FData);
		}
		break;
		case IRInstructionType::MallocCall:
		{
			RegisterID ID = GetRegisterForTep();
			LockRegister(ID);
			auto SizeReg = LoadOp(Item, Item.Target());
			InstructionBuilder::Malloc(_Ins, SizeReg, ID); PushIns();

			GiveNameToReg(ID, &Item);
		}
		break;
		case IRInstructionType::FreeCall:
		{
			InstructionBuilder::Free(_Ins, LoadOp(Item, Item.Target())); PushIns();
		}break;
		case IRInstructionType::Return:
			DropStack();
			DropPars();
			InstructionBuilder::Return(ExitState::Success, _Ins); PushIns();
			break;
		case IRInstructionType::Jump:
			InstructionBuilder::Jump(NullAddress, _Ins); PushIns();
			InsToUpdate.push_back({Getliboutput().Get_Instructions().size(), Item.Target().Value.AsUIntNative });
			break;
		case IRInstructionType::ConditionalJump:
			InstructionBuilder::Jumpif(NullAddress, LoadOp(Item, Item.Input()), _Ins); PushIns();
			InsToUpdate.push_back({ Getliboutput().Get_Instructions().size(),Item.Target().Value.AsUIntNative });
			break;
		case IRInstructionType::Logical_Not:
		{	
			RegisterID Out = GetRegisterForTep();
			LogicalNot(Item.ObjectType._Type,LoadOp(Item, Item.Target()),Out);
			GiveNameToReg(Out, &Item);

		}
		break;
		case IRInstructionType::Reassign_dereference:
		{
			RegisterID A = LoadOp(Item, Item.Target());
			StoreValueInPointer(Item.ObjectType, A, LoadOp(Item, Item.Input()));
		}
		break; 
		case IRInstructionType::EqualTo:
		{
			RegisterID V = RegisterID::BoolRegister;
			RegisterID A = LoadOp(Item, Item.A);
			LockRegister(A);
			RegisterID B = LoadOp(Item, Item.B);

			RegWillBeUsed(V);
			switch (Item.ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::equalto8(_Ins, A, B); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::equalto16(_Ins, A, B); PushIns(); break;

			bit32label3:
			case IRTypes::i32:InstructionBuilder::equalto32(_Ins, A, B); PushIns(); break;
			bit64label3:
			case IRTypes::i64:InstructionBuilder::equalto64(_Ins, A, B); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					goto bit32label3;
				}
				else
				{
					goto bit64label3;
				}
				break;
			default:
				throw std::exception("not added");
				break;
			}

			UnLockRegister(A);
			GiveNameToReg(V, &Item);
		}
		break;
		case IRInstructionType::CallFuncPtr:
		{
			auto FuncInfo = _Input->GetSymbol(Item.Target().identifer)->Get_ExAs<IRFuncPtr>();
			auto& FData = FuncCallStart(FuncInfo->Pars, FuncInfo->Ret);
 
			InstructionBuilder::CallReg(LoadOp(Item,Item.Target()), _Ins); PushIns();


			FuncCallEnd(FData);
		}
		break;
		

		case IRInstructionType::Member_Access://Vec2.X
		{
		}
		break;
		case IRInstructionType::Member_Access_Dereference://Vec2->X
		{
		
		}
		break;
		case IRInstructionType::SIntToUInt:
		{
			RegisterID V = GetRegisterForTep();
			RegisterID A = LoadOp(Item, Item.Target());
			LockRegister(A);

			switch (GetType(Item.Target())._Type)
			{
			case IRTypes::i8:InstructionBuilder::SInt8ToUInt8(_Ins, A, V); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::SInt16ToUInt16(_Ins, A, V); PushIns(); break;
			bit32label44:
			case IRTypes::i32:InstructionBuilder::SInt32ToUInt32(_Ins, A, V); PushIns(); break;

			bit64label44:
			case IRTypes::i64:InstructionBuilder::SInt64ToUInt64(_Ins, A, V); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					goto bit32label44;
				}
				else
				{
					goto bit64label44;
				}
				break;
			default:
				throw std::exception("not added");
				break;
			}

			UnLockRegister(A);
			GiveNameToReg(V, &Item);
		}
		break;
		case IRInstructionType::UIntToSInt:
		{
			RegisterID V = GetRegisterForTep();
			RegisterID A = LoadOp(Item, Item.Target());
			LockRegister(A);

			switch (GetType(Item.Target())._Type)
			{
			case IRTypes::i8:InstructionBuilder::UInt8ToSInt8(_Ins, A, V); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::UInt16ToSInt16(_Ins, A, V); PushIns(); break;
			bit32label9:
			case IRTypes::i32:InstructionBuilder::UInt32ToSInt32(_Ins, A, V); PushIns(); break;

			bit64label9:
			case IRTypes::i64:InstructionBuilder::UInt64ToSInt64(_Ins, A, V); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					goto bit32label9;
				}
				else
				{
					goto bit64label9;
				}
				break;
			default:
				throw std::exception("not added");
				break;
			}

			UnLockRegister(A);
			GiveNameToReg(V, &Item);
		}
		break;
		case IRInstructionType::SIntToSInt8:
		{
			BuildSIntToIntCast(Item, Item.Target(), 1);
		}
		break;
		case IRInstructionType::SIntToSInt16:
		{
			BuildSIntToIntCast(Item, Item.Target(), 2);
		}
		break;
		case IRInstructionType::SIntToSInt32:
		{
			BuildSIntToIntCast(Item, Item.Target(), 4);
		}
		break;
		case IRInstructionType::SIntToSInt64:
		{
			BuildSIntToIntCast(Item, Item.Target(), 8);
		}
		break;
		default:
			throw std::exception("not added");
			break;
		}


		IRToUCodeIns[i] = Getliboutput().Get_Instructions().size() - 1;
	}

	for (auto& Item : InsToUpdate)
	{
		Instruction& Ins = Getliboutput().Get_Instructions()[Item.InsToUpdate - 1];
		UAddress JumpPos = IRToUCodeIns[Item.Jumpto];

		if (Ins.OpCode == InstructionSet::Jump) 
		{
			InstructionBuilder::Jump(JumpPos, Ins);
		}
		else
		{
			InstructionBuilder::Jumpif(JumpPos,Ins.Value1.AsRegister, Ins);
		}
	}
}
UCodeBackEndObject::FuncCallEndData UCodeBackEndObject::FuncCallStart(const Vector<IRType>& Pars, const IRType& RetType)
{
	Vector<IRPar> Tep;
	Tep.resize(Pars.size());
	for (size_t i = 0; i < Pars.size(); i++)
	{
		auto& Item = Pars[i];
		auto& TepItem = Tep[i];
		TepItem.type = Item;
	}
	return FuncCallStart(Tep,RetType);
}
Optional< RegisterID> UCodeBackEndObject::FindIRInRegister(const IRInstruction* Value)
{
	return _Registers.GetInfo(Value);
}
Optional<RegisterID>  UCodeBackEndObject::FindValueInRegister(AnyInt64 Value)
{
	auto R = _Registers.GetValue(Value);
	if (R.has_value())
	{
		return R.value();
	}

	for (size_t i = 0; i < _Registers.Registers.size(); i++)
	{
		auto& Item = _Registers.Registers[i];
		if (Item.Inuse == RegistersManager::RegisterInUse::InUseSybol)
		{
			auto IR = Item.IRField;
			if (IR->Type == IRInstructionType::Load)
			{
				AnyInt64 ItemValue = IR->Target().Value;
				if (ItemValue.Value == Value.Value)
				{
					return (RegisterID)i;
				}
			}
		}
		if (Item.Inuse == RegistersManager::RegisterInUse::Locked)
		{
		
			if (Item.IRField != nullptr)
			{
				auto IR = Item.IRField;
				if (IR->Type == IRInstructionType::Load)
				{
					AnyInt64 ItemValue = IR->Target().Value;

					if (ItemValue.Value == Value.Value)
					{
						return (RegisterID)i;
					}
				}
			}
			else if (Item.BitValue.Value == Value.Value)
			{
				return (RegisterID)i;
			}
		}
	}
	return {};
}

UCodeBackEndObject::FuncCallEndData  UCodeBackEndObject::FuncCallStart(const Vector<IRPar>& Pars, const IRType& RetType)
{
	FuncCallRet(RetType);
	for (RegisterID_t i = (RegisterID_t)RegisterID::StartRegister; i < (RegisterID_t)RegisterID::EndRegister; i++)
	{
		RegisterID R = (RegisterID)i;
		auto& Item = _Registers.Registers[i];
		if (Item.Inuse == RegistersManager::RegisterInUse::InUseSybol)
		{
			if (IsReferencedAfterThisIndex(Item.IRField))
			{
				auto VType = GetType(Item.IRField);
				switch (VType._Type)
				{
				case IRTypes::i8:
					InstructionBuilder::Push8(_Ins, R); PushIns();
					break;
				case IRTypes::i16:
					InstructionBuilder::Push16(_Ins, R); PushIns();
					break;
				bit32labelGG:
				case IRTypes::f32:
				case IRTypes::i32:
					InstructionBuilder::Push32(_Ins, R); PushIns();
					break;
				bit64labelGG:
				case IRTypes::f64:
				case IRTypes::i64:
					InstructionBuilder::Push64(_Ins, R); PushIns();
					break;

				case IRTypes::pointer:
					if (Get_Settings().PtrSize == IntSizes::Int32)
					{
						goto bit32labelGG;
					}
					else
					{
						goto bit64labelGG;
					}
					break;
				default:
					throw std::exception("not added");
					break;
				}
			}
			else
			{
				Item.Inuse = RegistersManager::RegisterInUse::NotInUse;
			}

		}
	}

	FuncCallEndData r;
	r.Pars = Pars;
	return r;
}
void UCodeBackEndObject::FuncCallRet(const IRType& RetType)
{
	if (RetType._Type != IRTypes::Void)
	{
		RegWillBeUsed(RegisterID::OuPutRegister);
	}
}
void UCodeBackEndObject::FuncCallEnd(UCodeBackEndObject::FuncCallEndData& Data)
{
	auto V = GetParsLoc(Data.Pars);
	
	
	size_t PopBufferSize=0;
	for (size_t i = 0; i < V.OverflowedPars.size(); i++)
	{
		auto& Item = V.ParsPos[V.OverflowedPars[i]];
		PopBufferSize += _Input->GetSize(Item.Par->type);
	}


	if (PopBufferSize != 0)
	{
		RegisterID PopRegister = GetRegisterForTep();
		while (PopBufferSize != 0)
		{
			if (PopBufferSize >= 8)
			{
				InstructionBuilder::Pop64(_Ins, PopRegister); 
				PopBufferSize -= 8;
			}
			else if (PopBufferSize >= 4)
			{
				InstructionBuilder::Pop32(_Ins, PopRegister);
				PopBufferSize -= 4;
			}
			else if (PopBufferSize >= 2)
			{
				InstructionBuilder::Pop16(_Ins, PopRegister);
				PopBufferSize -= 2;
			}
			else if (PopBufferSize >= 8)
			{
				InstructionBuilder::Pop8(_Ins, PopRegister);
				PopBufferSize -= 1;
			}
			PushIns();
		}

	}
	_InputPar = RegisterID::StartParameterRegister;




	for (RegisterID_t i = (RegisterID_t)RegisterID::StartRegister; i < (RegisterID_t)RegisterID::EndRegister; i++)
	{
		RegisterID R = (RegisterID)i;
		auto& Item = _Registers.Registers[i];
		if (Item.Inuse == RegistersManager::RegisterInUse::InUseSybol)
		{
			if (IsReferencedAfterThisIndex(Item.IRField))
			{
				auto VType = GetType(Item.IRField);
				switch (VType._Type)
				{
				case IRTypes::i8:
					InstructionBuilder::Pop8(_Ins, R); PushIns();
					break;
				case IRTypes::i16:
					InstructionBuilder::Pop16(_Ins, R); PushIns();
					break;
				bit32labelGG2:
				case IRTypes::f32:
				case IRTypes::i32:
					InstructionBuilder::Pop32(_Ins, R); PushIns();
					break;
				bit64labelGG2:
				case IRTypes::f64:
				case IRTypes::i64:
					InstructionBuilder::Pop64(_Ins, R); PushIns();
					break;

				case IRTypes::pointer:
					if (Get_Settings().PtrSize == IntSizes::Int32)
					{
						goto bit32labelGG2;
					}
					else
					{
						goto bit64labelGG2;
					}
					break;
				default:
					throw std::exception("not added");
					break;
				}
			}

		}
	}


	
}
void UCodeBackEndObject::LockRegister(RegisterID ID)
{
	_Registers.LockRegister(ID);
}
void UCodeBackEndObject::DropStack()
{
	if (_Stack.Size)
	{
		auto Ptr = Get_Settings().PtrSize;
		auto V = GetRegisterForTep();
		if (Ptr == IntSizes::Int32)
		{
			InstructionBuilder::Store32(_Ins, V, (UInt32)_Stack.Size); PushIns();
		}
		else
		{
			InstructionBuilder::Store64(_Ins, V, (UInt64)_Stack.Size); PushIns();
		}
		InstructionBuilder::DecrementStackPointer(_Ins, V); PushIns();
	}
}
void UCodeBackEndObject::DropPars()
{

}
void UCodeBackEndObject::StoreValueInPointer(const IRType& ObjectType, RegisterID Pointer, const  IROperator& Value,IRInstruction& Ins)
{
	StoreValueInPointer(ObjectType, Pointer, LoadOp(Ins, Value));
}
void UCodeBackEndObject::StoreValueInPointer(const IRType& ObjectType, RegisterID Pointer, RegisterID Value)
{
	switch (ObjectType._Type)
	{
	case IRTypes::i8:InstructionBuilder::StoreRegToPtr8(_Ins, Value, Pointer); PushIns(); break;
	case IRTypes::i16:InstructionBuilder::StoreRegToPtr16(_Ins, Value, Pointer); PushIns(); break;
	bit32label9:
	case IRTypes::i32:InstructionBuilder::StoreRegToPtr32(_Ins, Value, Pointer); PushIns(); break;

	bit64label9:
	case IRTypes::i64:InstructionBuilder::StoreRegToPtr64(_Ins, Value, Pointer); PushIns(); break;

	case IRTypes::pointer:
		if (Get_Settings().PtrSize == IntSizes::Int32)
		{
			goto bit32label9;
		}
		else
		{
			goto bit64label9;
		}
		break;
	default:
		throw std::exception("not added");
		break;
	}
	UnLockRegister(Value);
}
RegisterID  UCodeBackEndObject::ReadValueFromPointer(const IRType& ObjectType, RegisterID Pointer)
{
	auto Out = GetRegisterForTep();
	switch (ObjectType._Type)
	{
	case IRTypes::i8:
		InstructionBuilder::StoreFromPtrToReg8(_Ins, Pointer, Out); PushIns();
		break;
	case IRTypes::i16:
		InstructionBuilder::StoreFromPtrToReg16(_Ins, Pointer, Out); PushIns();
		break;
	Int32L5:
	case IRTypes::f32:
	case IRTypes::i32:
		InstructionBuilder::StoreFromPtrToReg32(_Ins, Pointer, Out); PushIns();
		break;

	Int64L5:
	case IRTypes::f64:
	case IRTypes::i64:
		InstructionBuilder::StoreFromPtrToReg64(_Ins, Pointer, Out); PushIns();
		break;
	case IRTypes::pointer:
		switch (Get_Settings().PtrSize)
		{
		case IntSizes::Int32:goto Int32L5;
		case IntSizes::Int64:goto Int64L5;
		default:
			throw std::exception("not added");
			break;
		}
		break;
	default:
		throw std::exception("not added");
		break;
	}
	return Out;
}
inline void UCodeBackEndObject::BuildSIntToIntCast(const IRInstruction& Ins, const IROperator& Op, size_t IntSize)
{
	RegisterID V = GetRegisterForTep();
	RegisterID A = LoadOp(Ins, Op);
	LockRegister(A);

	size_t ItemSize = _Input->GetSize(GetType(Op));


	while (ItemSize != IntSize)
	{
		if (ItemSize > IntSize)//cast down
		{
			switch (ItemSize)
			{
			case 1:
			default:break;
			}
		}
		else//cast up
		{

		}


	}

	UnLockRegister(A);
	GiveNameToReg(V, &Ins);
}
RegisterID UCodeBackEndObject::LoadOp(const IRInstruction& Ins, const  IROperator& Op)
{
	if (Op.Type == IROperatorType::Value)
	{
		
			AnyInt64 Value = ToAnyInt(Ins.ObjectType, Op);
			auto R = FindValueInRegister(Value);
			if (R.has_value())
			{
				return R.value();
			}
		
		auto V = GetRegisterForTep();
		switch (Ins.ObjectType._Type)
		{
		case IRTypes::i8:
			InstructionBuilder::Store8(_Ins,V,Op.Value.AsInt8); PushIns();
			break;
		case IRTypes::i16:
			InstructionBuilder::Store16(_Ins, V, Op.Value.AsInt16); PushIns();
			break;

		bit32label:
		case IRTypes::i32:
			InstructionBuilder::Store32(_Ins, V, Op.Value.AsInt32); PushIns();
			break;
		case IRTypes::f32:
			InstructionBuilder::Storef32(_Ins, V, Op.Value.Asfloat32); PushIns();
			break;
		case IRTypes::f64:
			InstructionBuilder::Storef64(_Ins, V, Op.Value.Asfloat64); PushIns();
			break;

		bit64label:
		case IRTypes::i64:
			InstructionBuilder::Store64(_Ins, V, Op.Value.AsInt64); PushIns();
			break;

		case IRTypes::pointer:
			if (Get_Settings().PtrSize == IntSizes::Int32)
			{
				goto bit32label;
			}
			else
			{
				goto bit64label;
			}
			break;
		default:
			throw std::exception("not added");
			break;
		}
		return V;
	}
	else if (Op.Type == IROperatorType::IRParameter)
	{
		auto V = GetParData(Op.Parameter);

		if (V->Type == Parloc::Register)
		{
			return V->_Reg.value();
		}
		else
		{
			auto Type = Op.Parameter->type;

			RegisterID R = GetRegisterForTep();
			size_t Offset = GetStatckOffset(*V);

			switch (Type._Type)
			{
			case IRTypes::i8:
				InstructionBuilder::GetFromStackSub8(_Ins, Offset, R); PushIns();
				break;
			case IRTypes::i16:
				InstructionBuilder::GetFromStackSub16(_Ins, Offset, R); PushIns();
				break;
			Case32Bit2:
			case IRTypes::f32:
			case IRTypes::i32:
				InstructionBuilder::GetFromStackSub32(_Ins, Offset, R); PushIns();
				break;
			Case64Bit2:
			case IRTypes::f64:
			case IRTypes::i64:
				InstructionBuilder::GetFromStackSub64(_Ins, Offset, R); PushIns();
				break;
			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int64)
				{
					goto Case64Bit2;
				}
				else
				{
					goto Case32Bit2;
				}
				break;
			default:
				throw std::exception("not added");
				break;

			}
			GiveNameToReg(R, &Ins);

			V->SetAsRegister(R);
			return R;
		}
	}
	else if (Op.Type == IROperatorType::IRInstruction)
	{
		bool MustDereference = false;

		if (Op.Pointer->Type == IRInstructionType::Member_Access_Dereference)
		{
			MustDereference = true;
		}

		if (MustDereference)
		{
			auto In = FindOp(Ins, Op);
			return ReadValueFromPointer(Ins.ObjectType, In);
		}
		else
		{
			return FindOp(Ins, Op);
		}
	}
	else if (Op.Type == IROperatorType::Get_PointerOf_IRInstruction)
	{
		return  FindOp(Ins, Op);
	}
	else if (Op.Type == IROperatorType::DereferenceOf_IRInstruction
	|| Op.Type == IROperatorType::DereferenceOf_IRParameter)
	{
		auto In = FindOp(Ins, Op);
		return  ReadValueFromPointer(Ins.ObjectType,In);
	}
	else if (Op.Type == IROperatorType::Get_Func_Pointer)
	{
		auto V = GetRegisterForTep();

		InstructionBuilder::LoadFuncPtr(NullAddress, V, _Ins); PushIns();

		FuncInsID Tep;
		Tep.Index = _Output->Get_Instructions().size() - 1;
		Tep._FuncID = Op.identifer;

		FuncsToLink.push_back(Tep);

		return V;
	}
	else if (Op.Type == IROperatorType::Get_PointerOf_IRidentifier)
	{
		auto Syb = _Input->GetSymbol(Op.identifer);
		if (Syb)
		{
			if (Syb->SymType == IRSymbolType::StaticVarable)
			{
				StaticMemoryManager::StaticMemInfo& Value = _StaticMemory._List[Op.identifer];

				auto V = GetRegisterForTep();
				InstructionBuilder::GetPointerOfStaticMem(_Ins, V, Value.Offset); PushIns();

				return V;
			}
			else if (Syb->SymType == IRSymbolType::ThreadLocalVarable)
			{
				StaticMemoryManager::StaticMemInfo& Value = _ThreadMemory._List[Op.identifer];
			
				auto V = GetRegisterForTep();
				InstructionBuilder::GetPointerOfStaticMem(_Ins, V, Value.Offset); PushIns();

				return V;
			}

		}
	}
	throw std::exception("not added");
}

void UCodeBackEndObject::StoreValue(const IRInstruction& Ins, const  IROperator& OutputLocationIR, const  IROperator& Input)
{
	bool IRusedereference = false;
	if (OutputLocationIR.Type ==IROperatorType::IRInstruction)
	{
		auto& Item = OutputLocationIR.Pointer;
		if (Item->Type == IRInstructionType::Member_Access_Dereference)
		{
			IRusedereference = true;
		}
	}


	if (IRusedereference)
	{
		RegisterID Value = LoadOp(Ins, Input);

		size_t IndexV = Getliboutput().Get_Instructions().size();

		auto Pointer = FindOp(Ins, OutputLocationIR);


		if (Pointer == Value)
		{
			auto Reg = GetRegisterForTep();
			InstructionBuilder::Push64(_Ins, Value);
			
			Getliboutput().Get_Instructions().insert(Getliboutput().Get_Instructions().begin() + IndexV, _Ins);

			InstructionBuilder::Pop64(_Ins, Reg); PushIns();
			Value = Reg;
		}

		StoreValueInPointer(Ins.ObjectType, Pointer, Value);
	}
	else
	{
		RegToReg(Ins.ObjectType._Type, LoadOp(Ins, Input),FindOp(Ins,OutputLocationIR));
	}

}
void  UCodeBackEndObject::CopyValueToStack(const IRInstruction* IRName, const IRType& ObjectType, RegisterID Item)
{
	size_t StackPos = _Stack.Size;
	switch (ObjectType._Type)
	{
	case IRTypes::i8://move value to stack
		_Stack.Size += 1;
		InstructionBuilder::StoreRegOnStackSub8(_Ins, Item, _Stack.Size); PushIns();
		break;
	case IRTypes::i16:
		_Stack.Size += 2;
		InstructionBuilder::StoreRegOnStackSub16(_Ins, Item, _Stack.Size); PushIns();
		break;
	Int32L:
	case IRTypes::f32:
	case IRTypes::i32:
		_Stack.Size += 4;
		InstructionBuilder::StoreRegOnStackSub32(_Ins, Item, _Stack.Size); PushIns();
		break;
	Int64L:
	case IRTypes::f64:
	case IRTypes::i64:
		_Stack.Size += 8;
		InstructionBuilder::StoreRegOnStackSub64(_Ins, Item, _Stack.Size); PushIns();
		break;
	case IRTypes::pointer:
		switch (Get_Settings().PtrSize)
		{
		case IntSizes::Int32:goto Int32L;
		case IntSizes::Int64:goto Int64L;
		default:
			throw std::exception("not added");
			break;
		}
		break;
	case IRTypes::IRsymbol:
	{
		auto Syb = _Input->GetSymbol(ObjectType._symbol);
		switch (Syb->SymType)
		{
		case IRSymbolType::Struct:
		{
			const IRStruct* V = Syb->Get_ExAs<IRStruct>();
			_Stack.Size += _Input->GetSize(V);
		}
		break;
		default:
			throw std::exception("not added");
			break;
		}
	}
	break;
	default:

		throw std::exception("not added");
		break;
	}
	StackItem StItem;
	StItem.Offset = StackPos;
	StItem.IR = IRName;
	_Stack.Items.push_back(StItem);
}
void UCodeBackEndObject::MoveValueToStack(const IRInstruction* IRName, const IRType& ObjectType, RegisterID Item)
{
	CopyValueToStack(IRName, ObjectType, Item);
	_Registers.GetInfo(Item).Inuse = RegistersManager::RegisterInUse::NotInUse;
}
void UCodeBackEndObject::LoadOpToReg(const IRInstruction& Ins, const  IROperator& Op, RegisterID Out)
{
	RegToReg(Ins.ObjectType._Type, LoadOp(Ins, Op), Out);
}
void UCodeBackEndObject::RegToReg(IRTypes Type, RegisterID In, RegisterID Out)
{
	if (In != Out)
	{
		switch (Type)
		{
			InstructionBuilder::StoreRegToReg8(_Ins,In,Out); PushIns();
			break;
		case IRTypes::i16:
			InstructionBuilder::StoreRegToReg16(_Ins, In, Out); PushIns();
			break;
		Int32L:
		case IRTypes::f32:
		case IRTypes::i32:
			InstructionBuilder::StoreRegToReg32(_Ins, In, Out); PushIns();
			break;

		Int64L:
		case IRTypes::f64:
		case IRTypes::i64:
		case IRTypes::i8:
			InstructionBuilder::StoreRegToReg64(_Ins, In, Out); PushIns();
			break;
			Pointer:
		case IRTypes::pointer:
			switch (Get_Settings().PtrSize)
			{
			case IntSizes::Int32:goto Int32L;
			case IntSizes::Int64:goto Int64L;
			default:
				throw std::exception("not added");
				break;
			}
			break;
		case IRTypes::IRsymbol:
		{
			goto Pointer;
		}//FuncPtrs
			break;
		default:
			throw std::exception("not added");
			break;
		}
	}
}
void UCodeBackEndObject::PushOpStack(const IRInstruction& Ins, const  IROperator& Op)
{
	auto R =LoadOp(Ins,Op);
	switch (Ins.ObjectType._Type)
	{
	case IRTypes::i8:
		InstructionBuilder::Push8(_Ins, R); PushIns();
		_Stack.PushedOffset += 1;
		break;
	case IRTypes::i16:
		InstructionBuilder::Push16(_Ins, R); PushIns();
		_Stack.PushedOffset += 2;
		break;

	bit32label:
	case IRTypes::f32:
	case IRTypes::i32:
		InstructionBuilder::Push32(_Ins, R); PushIns();
		_Stack.PushedOffset += 4;
		break;

	bit64label:
	case IRTypes::f64:
	case IRTypes::i64:
		InstructionBuilder::Push64(_Ins, R); PushIns();
		_Stack.PushedOffset += 8;
		break;

	case IRTypes::pointer:
		if (Get_Settings().PtrSize == IntSizes::Int32)
		{
			goto bit32label;
		}
		else
		{
			goto bit64label;
		}
		break;
	default:
		throw std::exception("not added");
		break;
	}
}
RegisterID UCodeBackEndObject::FindOp(const IRInstruction& Ins,const IROperator& Op)
{
	auto V = _Registers.GetInfo(Op.Pointer);
	if (!V.has_value())
	{
		auto Item = _Stack.Has(Op.Pointer);
		if (Item)
		{
			auto V = GetRegisterForTep();
			auto V2 = _Stack.Has(Op.Pointer);

			auto Type = GetType(Op.Pointer);

			switch (Type._Type)
			{
			case IRTypes::i8:
				InstructionBuilder::GetFromStackSub8(_Ins,_Stack.Size - V2->Offset, V); PushIns();
				break;
			case IRTypes::i16:
				InstructionBuilder::GetFromStackSub16(_Ins, _Stack.Size - V2->Offset, V); PushIns();
				break;
			case IRTypes::f32:
			case IRTypes::i32:
				InstructionBuilder::GetFromStackSub32(_Ins, _Stack.Size - V2->Offset, V); PushIns();
				break;
			case IRTypes::f64:
			case IRTypes::i64:
				InstructionBuilder::GetFromStackSub64(_Ins, _Stack.Size - V2->Offset, V); PushIns();
				break;
			default:
				throw std::exception("not added");
				break;
			}
			GiveNameToReg(V, &Ins);
			return V;
		}

		if (Op.Type == IROperatorType::IRInstruction
			|| Op.Type == IROperatorType::DereferenceOf_IRInstruction)
		{
			if (Op.Pointer->Type == IRInstructionType::LoadNone)
			{
				auto T = GetRegisterForTep();
				GiveNameToReg(T, Op.Pointer);
				return T;
			}
			else if (Op.Pointer->Type == IRInstructionType::Call)
			{
				return RegisterID::OuPutRegister;
			}
			else if (Op.Pointer->Type == IRInstructionType::MallocCall)
			{
				return  _Registers.GetInfo(Op.Pointer).value();
			}
			else if (Op.Pointer->Type == IRInstructionType::Member_Access_Dereference)
			{
				auto V = LoadOp(*Op.Pointer, Op.Pointer->Target());
				LockRegister(V);

				IRType ObjectType = Op.Pointer->ObjectType;
				const IRStruct* Struct = _Input->GetSymbol(ObjectType._symbol)->Get_ExAs<IRStruct>();

				size_t StructIndex = Op.Pointer->Input().Value.AsUIntNative;
				size_t Offset = _Input->GetOffset(Struct, StructIndex);

				if (Offset==0)
				{
					return V;
				}
				else
				{

					AnyInt64 OffsetAnyint = Get_Settings().PtrSize == IntSizes::Int32 ? (UInt32)Offset : (UInt64)Offset;
					Optional<RegisterID> Item = FindValueInRegister(OffsetAnyint);

					if (!Item.has_value())
					{

						RegisterID tep = GetRegisterForTep();

						auto OutValue = RegisterID::MathOuPutRegister;
						RegWillBeUsed(OutValue);

						if (Get_Settings().PtrSize == IntSizes::Int32)
						{
							InstructionBuilder::Store32(_Ins, tep, OffsetAnyint.AsUInt32); PushIns();
							InstructionBuilder::Add32(_Ins, V, tep); PushIns();
						}
						else
						{
							InstructionBuilder::Store64(_Ins, tep, OffsetAnyint.AsUInt64); PushIns();
							InstructionBuilder::Add64(_Ins, V, tep); PushIns();
						}
						GiveNameToReg(tep,OffsetAnyint);

						UnLockRegister(OutValue);
						return  OutValue;
					}
					else
					{

						auto OutValue = RegisterID::MathOuPutRegister;
						RegWillBeUsed(OutValue);

						RegisterID tep = Item.value();
						if (Get_Settings().PtrSize == IntSizes::Int32)
						{
							InstructionBuilder::Add32(_Ins, V, tep); PushIns();
						}
						else
						{
							InstructionBuilder::Add64(_Ins, V, tep); PushIns();
						}

						UnLockRegister(OutValue);
						return  OutValue;
					}
					
					
				}
			}
			else if (IsLocation(Op.Pointer->Type))
			{
				return LoadOp(*Op.Pointer,Op.Pointer->Target());
			}
			else
			{
				throw std::exception("not added");
			}
		}
		else if (Op.Type == IROperatorType::IRParameter
			|| Op.Type == IROperatorType::DereferenceOf_IRParameter)
		{
			auto V = GetParData(Op.Parameter);

			if (V->Type == Parloc::Register)
			{
				return V->_Reg.value();
			}
			else
			{
				throw std::exception("not added");
			}
		}
		else if (Op.Type == IROperatorType::Get_Func_Pointer)
		{
			throw std::exception("not added");
		}
		else if (Op.Type == IROperatorType::Get_PointerOf_IRInstruction)
		{
			auto PointerV = Op.Pointer;
			auto S = _Stack.Has(PointerV);

			auto V = _Registers.GetInfo(PointerV);
			if (V.has_value())
			{
				return V.value();
			}

			if (S)
			{
				auto V = GetRegisterForTep();


				auto V2 = _Stack.Has(PointerV);
				InstructionBuilder::GetPointerOfStackSub(_Ins, V, _Stack.Size - V2->Offset); PushIns();
			}
			else
			{

				auto Item = FindOp(*PointerV, IROperator(Op));

				MoveValueToStack(PointerV, GetType(PointerV), Item);

				auto V = GetRegisterForTep();


				auto V2 = _Stack.Has(PointerV);
				InstructionBuilder::GetPointerOfStackSub(_Ins, V, _Stack.Size - V2->Offset); PushIns();


				GiveNameToReg(V, &Ins);

				return V;
			}
		}
		else
		{
			throw std::exception("not added");
		}
	}
	else
	{
		return V.value();
	}
}
void UCodeBackEndObject::FindOpToReg(const  IRInstruction& Ins, const IROperator& Op, RegisterID Out)
{
	RegToReg(Ins.ObjectType._Type, FindOp(Ins, Op), Out);
}
void UCodeBackEndObject::LogicalNot(IRTypes Type, RegisterID In, RegisterID Out)
{
	if (In != Out)
	{
		switch (Type)
		{
		case IRTypes::i8:
			InstructionBuilder::LogicalNot8(_Ins, In, Out); PushIns();
			break;
		case IRTypes::i16:
			InstructionBuilder::LogicalNot16(_Ins, In, Out); PushIns();
			break;
		Int32L:
		case IRTypes::f32:
		case IRTypes::i32:
			InstructionBuilder::LogicalNot32(_Ins, In, Out); PushIns();
			break;

		Int64L:
		case IRTypes::f64:
		case IRTypes::i64:
			InstructionBuilder::LogicalNot64(_Ins, In, Out); PushIns();
			break;
		case IRTypes::pointer:
			switch (Get_Settings().PtrSize)
			{
			case IntSizes::Int32:goto Int32L;
			case IntSizes::Int64:goto Int64L;
			default:
				throw std::exception("not added");
				break;
			}
			break;
		default:
			throw std::exception("not added");
			break;
		}
	}
}
RegisterID UCodeBackEndObject::GetRegisterForTep()
{
	auto Item = _Registers.GetFreeRegister();
	if (!Item.has_value())
	{
		for (RegisterID_t i = (RegisterID_t)RegisterID::StartRegister; i < (RegisterID_t)RegisterID::EndRegister; i++)
		{
			RegisterID R = (RegisterID)i;
			auto& Item = _Registers.Registers[i];
			if (Item.Inuse == RegistersManager::RegisterInUse::InUseSybol)
			{
				if (!IsReferencedAfterThisIndex(Item.IRField))
				{
					Item.Inuse = RegistersManager::RegisterInUse::NotInUse;
					return R;
				}
			}
		}


		constexpr size_t BuffSize = (RegisterID_t)RegisterID::EndParameterRegister;
		Array<WeightType, BuffSize> RegisterWeights;
		for (RegisterID_t i = 0; i < BuffSize; i++)
		{
			RegisterID R = (RegisterID)i;
			auto& Item = _Registers.Registers[i];


			RegisterWeights[i] = IsReferencedAfterThisIndexWeighted(Item.IRField);


		}


		size_t BestToOverLoad = 0;
		WeightType LestToBeat = SIZE_MAX;

		for (size_t i = 0; i < RegisterWeights.size(); i++)
		{
			auto Item = RegisterWeights[i];

			if (Item < LestToBeat)
			{
				BestToOverLoad = i;
				LestToBeat = Item;
			}

		}

		auto r = (RegisterID)BestToOverLoad;
		RegWillBeUsed(r);
		return r;
	}
	return Item.value();
}
AnyInt64 UCodeBackEndObject::ToAnyInt(const IRType& ObjectType,const IROperator& Op)
{
	AnyInt64 Value;
	switch (ObjectType._Type)
	{
	case IRTypes::i8:
		Value = Op.Value.AsInt8;
		break;
	case IRTypes::i16:
		Value = Op.Value.AsInt16;
		break;

	bit32label:
	case IRTypes::i32:
		Value = Op.Value.AsInt32;
		break;
	case IRTypes::f32:
		Value = Op.Value.Asfloat32;
		break;
	case IRTypes::f64:
		Value = Op.Value.Asfloat64;
		break;

	bit64label:
	case IRTypes::i64:
		Value = Op.Value.AsInt64;
		break;

	case IRTypes::pointer:
		if (Get_Settings().PtrSize == IntSizes::Int32)
		{
			goto bit32label;
		}
		else
		{
			goto bit64label;
		}
		break;
	default:
		throw std::exception("not added");
		break;
	}
	return Value;
}
UCodeBackEndObject::WeightType UCodeBackEndObject::IsReferencedAfterThisIndexWeighted(const IROperator& Op)
{
	WeightType R = 0;
	WeightType NextUseOffset = 0;

	WeightType Count = 0;
	for (size_t i = Index + 1; i < LookingBlock->Instructions.size(); i++)
	{
		auto Item = LookingBlock->Instructions[i].get();
		Count++;
		if (IsOperatorValueInTarget(Item->Type))
		{
			if (IsReferenceingTheSame(Op, Item->Target()))
			{
				R++;

				if (NextUseOffset == 0)
				{
					NextUseOffset = Count;
				}
			}
		}

		if (IsOperatorValueInInput(Item->Type))
		{
			if (IsReferenceingTheSame(Op, Item->Input()))
			{
				R++;

				if (NextUseOffset == 0)
				{
					NextUseOffset = Count;
				}
			}
		}
	}
	return R - NextUseOffset;
}
bool  UCodeBackEndObject::IsReferenceingTheSame(const IROperator& Test, const IROperator& Other)
{
	if (Test.Type == Other.Type)
	{
		if (Test.identifer == Other.identifer)
		{
			return true;
		}
	}

	return false;
}
bool UCodeBackEndObject::IsReferencedAfterThisIndex(const IROperator& Op)
{
	for (size_t i = Index + 1; i < LookingBlock->Instructions.size(); i++)
	{
		auto Item = LookingBlock->Instructions[i].get();

		if (IsOperatorValueInTarget(Item->Type))
		{
			if (IsReferenceingTheSame(Op, Item->Target()))
			{
				return true;
			}
		}

		if (IsOperatorValueInInput(Item->Type))
		{
			if (IsReferenceingTheSame(Op, Item->Input()))
			{
				return true;
			}
		}
	}
	return false;
}
void  UCodeBackEndObject::SynchronizePar(ParlocData* Par)
{
	for (size_t i = 0; i < RegistersManager::RegisterSize; i++)
	{
		auto& Info = _Registers.Registers[i];
		if (Info.Inuse == RegistersManager::RegisterInUse::InUseSybol)
		{
			const IRInstruction* IR = Info.IRField;
			if (IsLookingAtPar(IR, Par->Par))
			{
				Par->SetAsRegister((RegisterID)i);
				break;
			}
		}
	}
	for (size_t i = 0; i < _Stack.Items.size(); i++)
	{
		auto& Item = _Stack.Items[i];

		if (IsLookingAtPar(Item.IR, Par->Par))
		{
			Par->SetAsStackPostCall(Item.Offset);
		}
	}
}
UCodeBackEndObject::FindParsLoc UCodeBackEndObject::GetParsLoc(const Vector<IRType>& Pars)
{
	Vector<IRPar> Tep;
	Tep.resize(Pars.size());
	for (size_t i = 0; i < Pars.size(); i++)
	{
		auto& Item = Pars[i];
		auto& TepItem = Tep[i];
		TepItem.type = Item;
	}
	return GetParsLoc(Tep);
}
UCodeBackEndObject::FindParsLoc UCodeBackEndObject::GetParsLoc(const Vector<IRPar>& Pars)
{
	FindParsLoc R;

	auto V = RegisterID::StartParameterRegister;
	RegisterID_t& V2 = (RegisterID_t&)V;
	size_t StackOffset = 0;
	bool IsUseingStack = false;

	
	for (size_t i = 0; i < Pars.size(); i++)
	{
		auto& Item = Pars[i];

		ParlocData Loc;
		Loc.Par = &Item;

		auto ParSize = _Input->GetSize(Item.type);

		if (IsUseingStack == false && ParSize <= sizeof(AnyInt64))
		{
			Loc.SetAsRegister(V);

			LockRegister(V);

			V2++;
			if (V == RegisterID::EndParameterRegister)
			{
				IsUseingStack = true;
			}
		}
		else
		{
			Loc.SetAsStackPreCall(StackOffset);
			StackOffset += _Input->GetSize(Item.type);
			R.OverflowedPars.push_back(i);
		}
		R.ParsPos.push_back(Loc);
	}



	for (size_t i = 0; i < R.OverflowedPars.size() / 2; i++)// flip offsets
	{
		auto& Item0 = R.ParsPos[R.OverflowedPars[i]];
		auto& Item1 = R.ParsPos[R.OverflowedPars[R.OverflowedPars.size() - 1 - i]];

		std::swap(Item0.StackOffset, Item1.StackOffset);
	}

	return R;
}
void UCodeBackEndObject::DoCPPCall(const IRidentifier& FuncName)
{
	auto VFuncName = NameDecoratior::GetUnDecoratedName(FuncName);
	if (VFuncName == "__Log")
	{
		InstructionBuilder::Log(_Ins, RegisterID::StartParameterRegister); PushIns();
	}
	else if(VFuncName == "__LogChar")
	{
		InstructionBuilder::LogChar(_Ins, RegisterID::StartParameterRegister); PushIns();
	}
	else if (VFuncName == "__LogBuffer")
	{
		//move next Par to input
		RegToReg(IRTypes::i8, RegisterID((RegisterID_t)RegisterID::StartParameterRegister + 1), RegisterID::InPutRegister);
		
		InstructionBuilder::LogBuffer(_Ins, RegisterID::StartParameterRegister); PushIns();
	}
	else if (VFuncName == "__ReadChar")
	{
		InstructionBuilder::ReadChar(_Ins,RegisterID::OuPutRegister); PushIns();
	}
	else
	{
		auto Ptr = _Output->AddStaticBytes((String_view)FuncName);
		InstructionBuilder::CPPCall(Ptr, _Ins); PushIns();
	}

}
UCodeLangEnd

