#include "UCodeBackEnd.hpp"
#include "UCodeLang/Compliation/Helpers/InstructionBuilder.hpp"
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
	_Input = nullptr;
	_Output = nullptr;
}

void UCodeBackEndObject::Build(const IRBuilder* Input)
{
	_Input = Input;
	_Output = &Get_Output();
	for (auto& Item : Input->Funcs)
	{
		OnFunc(Item.get());
	}


	//link
	for (auto& Item : FuncsToLink)
	{
		Instruction& Ins = Get_Output().Get_Instructions()[Item.Index];

		if (Ins.OpCode == InstructionSet::Call)
		{

			UAddress funcpos = NullAddress;
			for (auto& Item2 : _Funcpos)
			{
				if (Item2._FuncID == Item._FuncID)
				{
					funcpos = Item2.Index;
					break;
				}
			}
			InstructionBuilder::Call(funcpos, Ins);
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
		for (auto& Item : IR->Pars)
		{
			ParlocData Loc;


			
			if (V != RegisterID::NullRegister)
			{
				Loc.Par = &Item;
				Loc._Reg = V;
				Loc.Type = Parloc::Register;

				_Registers.LockRegister(V);

				V2++;
				if (V > RegisterID::EndParameterRegister)
				{
					V = RegisterID::NullRegister;
				}
			}
			else
			{
				Loc.Par = &Item;
				Loc._Reg = V;
				Loc.StackOffset = StackOffset;

				StackOffset += _Input->GetSize(Item.type);

			}
			ParsPos.push_back(Loc);
		}


		
		for (auto& Item : ParsPos)// flip offsets because how pushing and poping the stack works
		{
			//throw std::exception("not added");
		}
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
		//useing insert is slow but it works
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



	}

	_Stack.Reset();
	_Registers.Reset();
	_Output->Add_NameToInstruction(FuncStart, _Input->FromID(IR->identifier));


	Funcpos V;
	V.Index = FuncStart-1;
	V._FuncID = IR->identifier;
	_Funcpos.push_back(V);
}
void UCodeBackEndObject::OnBlock(const IRBlock* IR)
{
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
	Unordered_map<size_t, UAddress> IRToUCodeIns;

	for (size_t i = 0; i < IR->Instructions.size(); i++)
	{
		auto& Item_ = IR->Instructions[i];
		auto& Item = *Item_;

		switch (Item.Type)
		{
		case IRInstructionType::LoadNone:
			break;
		case IRInstructionType::Load:
		{
			RegisterID V = LoadOp(Item, Item.Target());
			_Registers.WeakLockRegisterValue(V, &Item);
		}
		break;
		case IRInstructionType::LoadReturn:
		{
			LoadOpToReg(Item, Item.Target(), RegisterID::OuPutRegister);
		}
		break;
		case IRInstructionType::Reassign:
		{
			auto V = FindOp(Item, Item.Target());
			if (V != RegisterID::NullRegister)
			{
				LoadOpToReg(Item, Item.Input(), V);
			}
			else
			{
				throw std::exception("not added");
			}
		}
		break;

		case IRInstructionType::SMult:
		{
			RegisterID V = RegisterID::MathOuPutRegister;
			RegisterID A = LoadOp(Item, Item.A);
			LockRegister(A);
			RegisterID B = LoadOp(Item, Item.B);

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

			_Registers.UnLockRegister(A);
			_Registers.WeakLockRegisterValue(V, &Item);
		}
		break;
		case IRInstructionType::UMult:
		{
			RegisterID V = RegisterID::MathOuPutRegister;
			RegisterID A = LoadOp(Item, Item.A);
			LockRegister(A);
			RegisterID B = LoadOp(Item, Item.B);

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

			_Registers.UnLockRegister(A);
			_Registers.WeakLockRegisterValue(V, &Item);
		}
		break;

		case IRInstructionType::SDiv:
		{
			RegisterID V = RegisterID::MathOuPutRegister;
			RegisterID A = LoadOp(Item, Item.A);
			LockRegister(A);
			RegisterID B = LoadOp(Item, Item.B);

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

			_Registers.UnLockRegister(A);
			_Registers.WeakLockRegisterValue(V, &Item);
		}
		break;
		case IRInstructionType::UDiv:
		{
			RegisterID V = RegisterID::MathOuPutRegister;
			RegisterID A = LoadOp(Item, Item.A);
			LockRegister(A);
			RegisterID B = LoadOp(Item, Item.B);

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

			_Registers.UnLockRegister(A);
			_Registers.WeakLockRegisterValue(V, &Item);
		}
		break;

		case IRInstructionType::Add:
		{
			RegisterID V = RegisterID::MathOuPutRegister;
			RegisterID A = LoadOp(Item, Item.A);
			LockRegister(A);
			RegisterID B = LoadOp(Item, Item.B);

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

			_Registers.UnLockRegister(A);
			_Registers.WeakLockRegisterValue(V, &Item);
		}
		break;
		case IRInstructionType::Sub:
		{
			RegisterID V = RegisterID::MathOuPutRegister;
			RegisterID A = LoadOp(Item, Item.A);
			LockRegister(A);
			RegisterID B = LoadOp(Item, Item.B);

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

			_Registers.UnLockRegister(A);
			_Registers.WeakLockRegisterValue(V, &Item);
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
			InstructionBuilder::Call(NullAddress, _Ins); PushIns();

			FuncInsID Tep;
			Tep.Index = _Output->Get_Instructions().size() - 1;
			Tep._FuncID = Item.Target().identifer;

			FuncsToLink.push_back(Tep);

			_InputPar = RegisterID::StartParameterRegister;
		}
		break;
		case IRInstructionType::MallocCall:
		{
			RegisterID ID = _Registers.GetFreeRegister();
			LockRegister(ID);
			auto SizeReg = LoadOp(Item, Item.Target());
			InstructionBuilder::Malloc(_Ins, SizeReg, ID); PushIns();
			_Registers.WeakLockRegisterValue(ID, &Item);
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
			RegisterID Out = _Registers.GetFreeRegister();
			LogicalNot(Item.ObjectType._Type,LoadOp(Item, Item.Target()),Out);
			_Registers.WeakLockRegisterValue(Out, &Item);

		}
		break;
		case IRInstructionType::Reassign_dereference:
		{
			RegisterID B = LoadOp(Item, Item.Input());
			LockRegister(B);
			RegisterID A = LoadOp(Item, Item.Target());






			switch (Item.ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::StoreRegToPtr8(_Ins, B, A); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::StoreRegToPtr16(_Ins, B, A); PushIns(); break;
			bit32label9:
			case IRTypes::i32:InstructionBuilder::StoreRegToPtr32(_Ins, B, A); PushIns(); break;

			bit64label9:
			case IRTypes::i64:InstructionBuilder::StoreRegToPtr64(_Ins, B, A); PushIns(); break;

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
			_Registers.UnLockRegister(B);
		}
		break; 
		case IRInstructionType::EqualTo:
		{
			RegisterID V = RegisterID::BoolRegister;
			RegisterID A = LoadOp(Item, Item.A);
			LockRegister(A);
			RegisterID B = LoadOp(Item, Item.B);

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

			_Registers.UnLockRegister(A);
			_Registers.WeakLockRegisterValue(V, &Item);
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
void UCodeBackEndObject::LockRegister(RegisterID ID)
{
	_Registers.LockRegister(ID);
}
void UCodeBackEndObject::DropStack()
{
	if (_Stack.Size)
	{
		auto Ptr = Get_Settings().PtrSize;
		auto V = _Registers.GetFreeRegister();
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
RegisterID UCodeBackEndObject::LoadOp(IRInstruction& Ins, IROperator Op)
{
	if (Op.Type == IROperatorType::Value)
	{
		auto V = _Registers.GetFreeRegister();
		if (V == RegisterID::NullRegister)
		{
			throw std::exception("not added");
			//
		}
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
			return V->_Reg;
		}
		else
		{
			throw std::exception("not added");
		}
	}
	else if (Op.Type == IROperatorType::IRInstruction)
	{
		return FindOp(Ins, Op);
	}
	else if (Op.Type == IROperatorType::Get_PointerOf_IRInstruction)
	{
		auto PointerV = Op.Pointer;
		auto S = _Stack.Has(PointerV);
		if (S)
		{
			auto V = _Registers.GetInfo(PointerV);
			if (V == RegisterID::NullRegister)
			{
				V= _Registers.GetFreeRegister();
				InstructionBuilder::GetPointerOfStackSub(_Ins, V, S->Offset); PushIns();
				return V;
			}
			else
			{
				return V;
			}
		}
		else
		{
			size_t StackPos = _Stack.Size;
			auto V = _Registers.GetFreeRegister();	
			
			switch (PointerV->ObjectType._Type)
			{
			case IRTypes::i8://move value to stack
				InstructionBuilder::DoNothing(_Ins); PushIns();
				_Stack.Size += 1;
				break;
			case IRTypes::i16:
				InstructionBuilder::DoNothing(_Ins); PushIns();
				_Stack.Size += 2;
				break;
			Int32L:
			case IRTypes::f32:
			case IRTypes::i32:
				InstructionBuilder::StoreRegOnStack32(_Ins, _Registers.GetInfo(PointerV),StackPos); PushIns();//move value to stack
				_Stack.Size += 4;
				break;

			Int64L:
			case IRTypes::f64:
			case IRTypes::i64:
				InstructionBuilder::DoNothing(_Ins); PushIns();
				_Stack.Size += 8;
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
			InstructionBuilder::GetPointerOfStackSub(_Ins,V,StackPos); PushIns();
			
			StackItem StItem;
			StItem.Offset = StackPos;
			StItem.IR = PointerV;
			_Stack.Items.push_back(StItem);

			//_Registers.WeakLockRegisterValue(V, PointerV);
			
			return V;
		}
	}
	else if (Op.Type == IROperatorType::DereferenceOf_IRInstruction
	|| Op.Type == IROperatorType::DereferenceOf_IRParameter)
	{
		auto In = FindOp(Ins, Op);
		auto Out = _Registers.GetFreeRegister();
		if (Out == RegisterID::NullRegister)
		{
			throw std::exception("not added");
			//
		}

		switch (Ins.ObjectType._Type)
		{
		case IRTypes::i8:
			InstructionBuilder::StoreFromPtrToReg8(_Ins, In, Out); PushIns();
			break;
		case IRTypes::i16:
			InstructionBuilder::StoreFromPtrToReg16(_Ins, In, Out); PushIns();
			break;
		Int32L5:
		case IRTypes::f32:
		case IRTypes::i32:
			InstructionBuilder::StoreFromPtrToReg32(_Ins, In, Out); PushIns();
			break;

		Int64L5:
		case IRTypes::f64:
		case IRTypes::i64:
			InstructionBuilder::StoreFromPtrToReg64(_Ins, In, Out); PushIns();
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
		return Out;
	}

	throw std::exception("not added");
	return RegisterID::NullRegister;
}
void UCodeBackEndObject::LoadOpToReg(IRInstruction& Ins, IROperator Op, RegisterID Out)
{
	RegToReg(Ins.ObjectType._Type, LoadOp(Ins, Op), Out);
}
void UCodeBackEndObject::RegToReg(IRTypes Type, RegisterID In, RegisterID Out)
{
	if (In != Out)
	{
		switch (Type)
		{
		case IRTypes::i8:
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
			InstructionBuilder::StoreRegToReg64(_Ins, In, Out); PushIns();
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
void UCodeBackEndObject::PushOpStack(IRInstruction& Ins, IROperator Op)
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
RegisterID UCodeBackEndObject::FindOp(IRInstruction& Ins, IROperator Op)
{
	auto V = _Registers.GetInfo(Op.Pointer);
	if (V == RegisterID::NullRegister)
	{
		if (Op.Type == IROperatorType::IRInstruction
			|| Op.Type == IROperatorType::DereferenceOf_IRInstruction)
		{
			if (Op.Pointer->Type == IRInstructionType::LoadNone)
			{
				auto T = _Registers.GetFreeRegister();
				_Registers.WeakLockRegisterValue(T, Op.Pointer);
				return T;
			}
			else if (Op.Pointer->Type == IRInstructionType::Call)
			{
				return RegisterID::OuPutRegister;
			}
			else if (Op.Pointer->Type == IRInstructionType::MallocCall)
			{
				return  _Registers.GetInfo(Op.Pointer);
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
				return V->_Reg;
			}
			else
			{
				throw std::exception("not added");
			}
		}
		else
		{
			throw std::exception("not added");
		}
	}
	else
	{
		return V;
	}
}
void UCodeBackEndObject::FindOpToReg(IRInstruction& Ins, IROperator Op, RegisterID Out)
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
UCodeLangEnd

