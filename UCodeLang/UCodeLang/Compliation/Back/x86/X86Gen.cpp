#include "X86Gen.hpp"
UCodeLangStart



void IRToX86::Reset()
{
	_Funcs.clear();
	_Output.Reset();
}

void IRToX86::Build(const IRBuilder* Input)
{
	_Input = Input;
	for (auto& Item : Input->Funcs)
	{
		size_t FuncLoc = _Output._Output.ByteOutput.size();
		OnFunc(Item.get());
		_Funcs.push_back({ Item.get() ,FuncLoc });
	}
	
}

void IRToX86::OnFunc(const IRFunc* IR)
{
	if (IR->Blocks.size())
	{
		OnBlock(IR->Blocks.front().get());
	}
}
//en.wikibooks.org/wiki/X86_Assembly/X86_Architecture#General-Purpose_Registers_(GPR)_-_16-bit_naming_conventions
\
void IRToX86::OnBlock(const IRBlock* IR)
{
	using namespace x86;
	for (auto& Item_ : IR->Instructions)
	{
		auto& Item = *Item_;

		switch (Item.Type)
		{
		case IRInstructionType::LoadNone:
			break;
		case IRInstructionType::Load: 
		{
			X86Gen::GReg V = LoadOpINGeneralRegister(Item, Item.Target());
			SetRegister(V, &Item);
		}
			break;
		case IRInstructionType::Reassign:
		{
			auto V = FindOp(Item, Item.Target());
			if (V != GReg::Null)
			{
				LoadOpToReg(Item, Item.Input(), V);
			}
			else
			{
				throw std::exception("not added");
			}
		}
		break;
		case IRInstructionType::LoadReturn:
		{
			LoadOpToReg(Item, Item.Target(), GReg::EAX);
		}
		break;
		case IRInstructionType::Return:
			_Output.Push_Ins_ret();
			break;
		default:
			throw std::exception("not added");
			break;
		}
	}
}

X86Gen::GReg IRToX86::LoadOpINGeneralRegister(IRInstruction& Ins, const IROperator& Op)
{
	if (Op.Type == IROperatorType::Value)
	{
		auto V = GetFreeGeneralRegister();
		if (V == GReg::Null)
		{
			throw std::exception("not added");
			//
		}
		switch (Ins.ObjectType._Type)
		{
		case IRTypes::i8:
			_Output.Push_Ins_MovImm8(V, Op.Value.AsInt8);
			break;
		case IRTypes::i16:
			_Output.Push_Ins_MovImm16(V, Op.Value.AsInt16);
			break;

		bit32label:
		case IRTypes::i32:
			_Output.Push_Ins_MovImm32(V, Op.Value.AsInt32);
			break;
		case IRTypes::f32:
			//InstructionBuilder::Storef32(_Ins, V, Op.Value.Asfloat32); PushIns();
			throw std::exception("not added");
			break;
		case IRTypes::f64:
			//InstructionBuilder::Storef64(_Ins, V, Op.Value.Asfloat64); PushIns();
			throw std::exception("not added");
			break;

		bit64label:
		case IRTypes::i64:
			//InstructionBuilder::Store64(_Ins, V, Op.Value.AsInt64); PushIns();
			throw std::exception("not added");
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
	else if (Op.Type == IROperatorType::IRInstruction)
	{
		return FindOp(Ins, Op);
	}
	else
	{
		throw std::exception("not added");
	}
}
X86Gen::GReg IRToX86::GetFreeGeneralRegister()
{
	for (size_t i = 0; i < GeneralRegisters_Count; i++)
	{
		auto& Item = GeneralRegisters[i];
		if (Item.State == GeneralRegisterDataState::notUsed)
		{
			return (x86::GeneralRegisters)i;
		}
	}
	return x86::GeneralRegisters::Null;
}
IRToX86::GReg IRToX86::FindGeneral(const IRInstruction* Ins)
{
	for (size_t i = 0; i < GeneralRegisters_Count; i++)
	{
		auto& Item = GeneralRegisters[i];
		if (Item.State == GeneralRegisterDataState::HasIRValue
			&& Item.HasValue == Ins)
		{
			return (x86::GeneralRegisters)i;
		}
	}
	return GReg::Null;
}
void IRToX86::SetRegister(GReg Reg, IRInstruction* Ins)
{
	auto& Item = GeneralRegisters[(size_t)Reg];
	Item.State = GeneralRegisterDataState::HasIRValue;
	Item.HasValue = Ins;
}
void IRToX86::LoadOpToReg(IRInstruction& Ins, const IROperator& Op, GReg  Out)
{
	RegToReg(Ins.ObjectType._Type, FindOp(Ins, Op), Out);
}
void IRToX86::RegToReg(IRTypes Type, GReg In, GReg Out)
{
	if (In != Out)
	{
		switch (Type)
		{
		case IRTypes::i8:
			_Output.Push_Ins_RegToReg8(In, Out);
			break;
		case IRTypes::i16:
			_Output.Push_Ins_RegToReg16(In, Out);
			break;
		Int32L:
		case IRTypes::f32:
		case IRTypes::i32:
			_Output.Push_Ins_RegToReg32(In, Out);
			break;

		Int64L:
		case IRTypes::f64:
		case IRTypes::i64:
			throw std::exception("not added");
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
IRToX86::GReg IRToX86::FindOp(IRInstruction& Ins, IROperator Op)
{
	auto V = FindGeneral(Op.Pointer);
	if (V == GReg::Null)
	{

		if (Op.Type == IROperatorType::IRInstruction
			|| Op.Type == IROperatorType::DereferenceOf_IRInstruction)
		{
			if (Op.Pointer->Type == IRInstructionType::LoadNone)
			{
				auto T = GetFreeGeneralRegister();
				SetRegister(T, Op.Pointer);
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
	return V;
}
UCodeLangEnd
