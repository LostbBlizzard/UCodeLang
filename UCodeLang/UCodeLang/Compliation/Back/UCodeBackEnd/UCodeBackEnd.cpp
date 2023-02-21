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
}
void UCodeBackEndObject::OnFunc(const IRFunc* IR)
{

	//build code
	UAddress FuncStart = _Output->GetLastInstruction()+1;

	if (IR->Blocks.size()) 
	{
		OnBlock(IR->Blocks.front().get());
	}


	_Registers.Reset();
	_Output->Add_NameToInstruction(FuncStart, _Input->FromID(IR->identifier));
}
void UCodeBackEndObject::OnBlock(const IRBlock* IR)
{
	OnBlockBuildCode(IR);
	
}
void UCodeBackEndObject::OnBlockBuildCode(const IRBlock* IR)
{
	for (auto& Item_ : IR->Instructions)
	{
		auto& Item = *Item_;

		switch (Item.Type)
		{
		case IRInstructionType::LoadNone:
			break;
		case IRInstructionType::Load:
		{
			int a = 0;
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
			auto V =FindOp(Item,Item.Target());
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
			_Registers.LockRegister(A);
			RegisterID B = LoadOp(Item, Item.B);
			
			switch (Item.ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::MultS8(_Ins, A,B); PushIns();break;
			case IRTypes::i16:InstructionBuilder::MultS16(_Ins, A, B); PushIns(); break;
			case IRTypes::i32:InstructionBuilder::MultS32(_Ins, A, B); PushIns(); break;
			case IRTypes::i64:InstructionBuilder::MultS64(_Ins, A, B); PushIns(); break;
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
			_Registers.LockRegister(A);
			RegisterID B = LoadOp(Item, Item.B);

			switch (Item.ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::MultU8(_Ins, A, B); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::MultU16(_Ins, A, B); PushIns(); break;
			case IRTypes::i32:InstructionBuilder::MultU32(_Ins, A, B); PushIns(); break;
			case IRTypes::i64:InstructionBuilder::MultU64(_Ins, A, B); PushIns(); break;
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
			_Registers.LockRegister(A);
			RegisterID B = LoadOp(Item, Item.B);

			switch (Item.ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::DivS8(_Ins, A, B); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::DivS16(_Ins, A, B); PushIns(); break;
			case IRTypes::i32:InstructionBuilder::DivS32(_Ins, A, B); PushIns(); break;
			case IRTypes::i64:InstructionBuilder::DivS64(_Ins, A, B); PushIns(); break;
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
			_Registers.LockRegister(A);
			RegisterID B = LoadOp(Item, Item.B);

			switch (Item.ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::DivU8(_Ins, A, B); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::DivU16(_Ins, A, B); PushIns(); break;
			case IRTypes::i32:InstructionBuilder::DivU32(_Ins, A, B); PushIns(); break;
			case IRTypes::i64:InstructionBuilder::DivU64(_Ins, A, B); PushIns(); break;
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
			_Registers.LockRegister(A);
			RegisterID B = LoadOp(Item, Item.B);

			switch (Item.ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::Add8(_Ins, A, B); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::Add16(_Ins, A, B); PushIns(); break;
			case IRTypes::i32:InstructionBuilder::Add32(_Ins, A, B); PushIns(); break;
			case IRTypes::i64:InstructionBuilder::Add64(_Ins, A, B); PushIns(); break;
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
			_Registers.LockRegister(A);
			RegisterID B = LoadOp(Item, Item.B);

			switch (Item.ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::Sub8(_Ins, A, B); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::Sub16(_Ins, A, B); PushIns(); break;
			case IRTypes::i32:InstructionBuilder::Sub32(_Ins, A, B); PushIns(); break;
			case IRTypes::i64:InstructionBuilder::Sub64(_Ins, A, B); PushIns(); break;
			default:
				throw std::exception("not added");
				break;
			}

			_Registers.UnLockRegister(A);
			_Registers.WeakLockRegisterValue(V, &Item);
		}
		break;
		case IRInstructionType::Return:
			InstructionBuilder::Return(ExitState::Success, _Ins); PushIns();
			break;
		default:
			throw std::exception("not added");
			break;
		}
	}
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
		case IRTypes::i32:
			InstructionBuilder::Store32(_Ins, V, Op.Value.AsInt32); PushIns();
			break;
		case IRTypes::f32:
			InstructionBuilder::Storef32(_Ins, V, Op.Value.Asfloat32); PushIns();
			break;
		case IRTypes::f64:
			InstructionBuilder::Storef64(_Ins, V, Op.Value.Asfloat64); PushIns();
			break;
		case IRTypes::i64:
			InstructionBuilder::Store64(_Ins, V, Op.Value.AsInt64); PushIns();
			break;
		default:
			throw std::exception("not added");
			break;
		}
		return V;
	}
	else if (Op.Type == IROperatorType::IRInstruction)
	{
		return FindOp(Ins, Op);
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
		case IRTypes::f32:
		case IRTypes::i32:
			InstructionBuilder::StoreRegToReg32(_Ins, In, Out); PushIns();
			break;
		case IRTypes::f64:
		case IRTypes::i64:
			InstructionBuilder::StoreRegToReg64(_Ins, In, Out); PushIns();
			break;
		default:
			throw std::exception("not added");
			break;
		}
	}
}
RegisterID UCodeBackEndObject::FindOp(IRInstruction& Ins, IROperator Op)
{
	auto V = _Registers.GetInfo(Op.Pointer);
	if (V == RegisterID::NullRegister)
	{
		if (Op.Type == IROperatorType::IRInstruction)
		{
			if (Op.Pointer->Type == IRInstructionType::LoadNone)
			{
				auto T = _Registers.GetFreeRegister();
				_Registers.WeakLockRegisterValue(T, Op.Pointer);
				return T;
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
UCodeLangEnd

