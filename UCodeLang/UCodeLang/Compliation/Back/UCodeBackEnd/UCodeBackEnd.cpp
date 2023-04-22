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

void UCodeBackEndObject::Build(const IRBuilder* Input)
{
	_Input = Input;
	_Output = &Get_Output();
	
	for (auto& Item : Input->_Symbols)
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

	if (Input->_StaticInit.HasInstructions()) {
		OnFunc(&Input->_StaticInit);
	}

	if (Input->_StaticdeInit.HasInstructions()) {
		OnFunc(&Input->_StaticdeInit);
	}

	if (Input->_threadInit.HasInstructions()) {
		OnFunc(&Input->_threadInit);
	}

	if (Input->_threaddeInit.HasInstructions()) {
		OnFunc(&Input->_threaddeInit);
	}
	for (auto& Item : Input->Funcs)
	{
		OnFunc(Item.get());
	}


	//link
	
		
	
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
void UCodeBackEndObject::OnFunc(const IRFunc* IR)
{

	//build code
	UAddress FuncStart = _Output->GetLastInstruction() + 1;


	{
		ParsPos.clear();
		auto V = RegisterID::StartParameterRegister;
		RegisterID_t& V2 = (RegisterID_t&)V;
		size_t StackOffset = 0;
		bool IsUseingStack = false;
		for (auto& Item : IR->Pars)
		{
			ParlocData Loc;
			Loc.Par = &Item;

			
			if (IsUseingStack == false)
			{
				Loc.SetAsRegister(V);

				LockRegister(V);
				_Registers.LockRegister(V);

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
			}
			ParsPos.push_back(Loc);
		}


		
		for (auto& Item : ParsPos)// flip offsets because how pushing and poping the stack works
		{
			//throw std::exception("not added");
		}
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

			auto Tep = RegisterID::EndParameterRegister;


			if (Reg <= (RegisterID_t&)Tep)
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

			FuncCallStart();	
			
			
			FuncCallRet(_Input->GetFunc(Item.Target().identifer)->ReturnType);

			InstructionBuilder::Call(NullAddress, _Ins); PushIns();

			FuncInsID Tep;
			Tep.Index = _Output->Get_Instructions().size() - 1;
			Tep._FuncID = Item.Target().identifer;

			FuncsToLink.push_back(Tep);

		

			FuncCallEnd();
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
			FuncCallStart();

			//FuncCallRet(_Input->GetSymbol(Item.Target().identifer)->ReturnType);

			throw std::exception("not added FuncCall Ret");

			InstructionBuilder::CallReg(LoadOp(Item,Item.Target()), _Ins); PushIns();


			FuncCallEnd();
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
void UCodeBackEndObject::FuncCallStart()
{
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
				Item.Inuse == RegistersManager::RegisterInUse::NotInUse;
			}

		}
	}
}
void UCodeBackEndObject::FuncCallRet(const IRType& RetType)
{
	if (RetType._Type != IRTypes::Void)
	{
		RegWillBeUsed(RegisterID::OuPutRegister);
	}
}
void UCodeBackEndObject::FuncCallEnd()
{
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
RegisterID UCodeBackEndObject::LoadOp(const IRInstruction& Ins, const  IROperator& Op)
{
	if (Op.Type == IROperatorType::Value)
	{
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

			if (V->Type == Parloc::StackPostCall) 
			{
				size_t Offset = _Stack.Size - V->StackOffset;
				switch (Type._Type)
				{
				case IRTypes::i8:
					InstructionBuilder::GetFromStackSub8(_Ins, Offset, R); PushIns();
					break;
				case IRTypes::i16:
					InstructionBuilder::GetFromStackSub16(_Ins, Offset, R); PushIns();
					break;

				Case32Bit:
				case IRTypes::f32:
				case IRTypes::i32:
					InstructionBuilder::GetFromStackSub32(_Ins, Offset, R); PushIns();
					break;

				Case64Bit:
				case IRTypes::f64:
				case IRTypes::i64:
					InstructionBuilder::GetFromStackSub64(_Ins, Offset, R); PushIns();
					break;
				case IRTypes::pointer:
					if (Get_Settings().PtrSize == IntSizes::Int64)
					{
						goto Case64Bit;
					}
					else
					{
						goto Case32Bit;
					}
					break;
				default:
					throw std::exception("not added");
					break;
				}
			}
			else
			{
				size_t FuncPointerSize = Get_Settings().PtrSize == IntSizes::Int64 ? 8 : 4;
				size_t Offset = _Stack.Size + V->StackOffset + FuncPointerSize;
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
					RegisterID tep = GetRegisterForTep();

					auto OutValue = RegisterID::MathOuPutRegister;
					RegWillBeUsed(OutValue);

					if (Get_Settings().PtrSize == IntSizes::Int32)
					{
						InstructionBuilder::Store32(_Ins, tep, (UInt32)Offset); PushIns();
						InstructionBuilder::Add32(_Ins, V, tep); PushIns();
					}
					else
					{
						InstructionBuilder::Store64(_Ins, tep, (UInt64)Offset); PushIns();
						InstructionBuilder::Add64(_Ins, V, tep); PushIns();
					}
					
					UnLockRegister(OutValue);
					return  OutValue;
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

				CopyValueToStack(PointerV, GetType(PointerV), Item);

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
	}
	return  _Registers.GetFreeRegister().value();
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

