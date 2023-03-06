#include "X86Gen.hpp"
UCodeLangStart



void IRToX86::Reset()
{
	_Output.Reset();
}

void IRToX86::Build(const IRBuilder* Input)
{
	_Input = Input;
	for (auto& Item : Input->Funcs)
	{
		OnFunc(Item.get());
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
		case IRInstructionType::Load:
			X86Gen::GReg V = LoadOpINGeneralRegister(Item, Item.Target());
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

X86Gen::GReg IRToX86::LoadOpINGeneralRegister(IRInstruction& Ins, IROperator Op)
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
			break;
		case IRTypes::f64:
			//InstructionBuilder::Storef64(_Ins, V, Op.Value.Asfloat64); PushIns();
			break;

		bit64label:
		case IRTypes::i64:
			//InstructionBuilder::Store64(_Ins, V, Op.Value.AsInt64); PushIns();
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

UCodeLangEnd
