#include "Instruction.hpp"
UCodeLangStart


#define SetInt(IntSize) \
	case InstructionSet::StoreRegToReg##IntSize: return Instruction::OpType::TwoReg; \
	case InstructionSet::StoreFromPtrToReg##IntSize: return Instruction::OpType::TwoReg; \
	case InstructionSet::StoreRegToPtr##IntSize: return Instruction::OpType::TwoReg; \
	case InstructionSet::Push##IntSize: return Instruction::OpType::OneReg;\
	case InstructionSet::Pop##IntSize: return Instruction::OpType::OneReg;\
	case InstructionSet::StoreRegOnStack##IntSize: return Instruction::OpType::RegUInt16;\
	case InstructionSet::StoreRegOnStackSub##IntSize: return Instruction::OpType::RegUInt16;\
	case InstructionSet::GetFromStack##IntSize: return Instruction::OpType::RegUInt16;\
	case InstructionSet::GetFromStackSub##IntSize: return Instruction::OpType::RegUInt16;\
	case InstructionSet::Add##IntSize: return Instruction::OpType::TwoReg;\
	case InstructionSet::Sub##IntSize: return Instruction::OpType::TwoReg;\
	case InstructionSet::MultS##IntSize: return Instruction::OpType::TwoReg;\
	case InstructionSet::MultU##IntSize: return Instruction::OpType::TwoReg;\
	case InstructionSet::DivS##IntSize: return Instruction::OpType::TwoReg;\
	case InstructionSet::DivU##IntSize: return Instruction::OpType::TwoReg;\
	case InstructionSet::LogicalAnd##IntSize: return Instruction::OpType::TwoReg;\
	case InstructionSet::Logicalor##IntSize: return Instruction::OpType::TwoReg;\
	case InstructionSet::LogicalNot##IntSize: return Instruction::OpType::TwoReg;\
	case InstructionSet::equalto##IntSize: return Instruction::OpType::TwoReg;\
	case InstructionSet::notequalto##IntSize: return Instruction::OpType::TwoReg;\
	case InstructionSet::lessthan##IntSize: return Instruction::OpType::TwoReg; \
	case InstructionSet::greaterthan##IntSize: return Instruction::OpType::TwoReg; \
	case InstructionSet::equal_lessthan##IntSize: return Instruction::OpType::TwoReg; \
	case InstructionSet::equal_greaterthan##IntSize: return Instruction::OpType::TwoReg; \
	case InstructionSet::bitwiseAnd##IntSize: return Instruction::OpType::TwoReg; \
	case InstructionSet::bitwiseOr##IntSize: return Instruction::OpType::TwoReg; \
	case InstructionSet::bitwiseLeftShift##IntSize: return Instruction::OpType::TwoReg; \
	case InstructionSet::bitwiseRightShift##IntSize: return Instruction::OpType::TwoReg; \
	case InstructionSet::bitwiseXor##IntSize: return Instruction::OpType::TwoReg; \
	case InstructionSet::bitwise_Not##IntSize: return Instruction::OpType::TwoReg; \
	case InstructionSet::UInt##IntSize##ToSInt##IntSize: return Instruction::OpType::TwoReg; \
	case InstructionSet::SInt##IntSize##ToUInt##IntSize: return Instruction::OpType::TwoReg; \


#define Setfloat(IntSize)

Instruction::OpType Instruction::GetOpType(InstructionSet OpCode)
{
	switch (OpCode)
	{

	case InstructionSet::Debug_FuncEnd:
	case InstructionSet::Debug_LineEnter:
	case InstructionSet::Debug_FuncStart:
		return Instruction::OpType::NoneOp;
	case InstructionSet::Exit:
	case InstructionSet::Return:
		return Instruction::OpType::ValUInt8;

	case InstructionSet::CppCallNamed:
	case InstructionSet::Call:
	case InstructionSet::Jump:
		return Instruction::OpType::ValUInt16;

	case InstructionSet::CallIf:
	case InstructionSet::Jumpif:
		return Instruction::OpType::RegUInt16;
	
	case InstructionSet::CallReg:
	case InstructionSet::JumpReg:
		return Instruction::OpType::OneReg;

	case InstructionSet::Store8:
		return Instruction::OpType::RegUInt8;

	case InstructionSet::Store16:
		return Instruction::OpType::RegUInt16;

	case InstructionSet::Store32v1:
	case InstructionSet::Store32v2:
	case InstructionSet::Storef32v1:
	case InstructionSet::Storef32v2:
	case InstructionSet::Store64v1:
	case InstructionSet::Store64v2:
	case InstructionSet::Store64v3:
	case InstructionSet::Store64v4:
	case InstructionSet::Storef64v1:
	case InstructionSet::Storef64v2:
	case InstructionSet::Storef64v3:
	case InstructionSet::Storef64v4:
		return Instruction::OpType::RegUInt16;
	
		SetInt(8);

		SetInt(16);

		SetInt(32);

		SetInt(64);

		Setfloat(32);


		Setfloat(64);

	case InstructionSet::Malloc:
		return Instruction::OpType::TwoReg;
	case InstructionSet::Free:
		return Instruction::OpType::OneReg;

	case InstructionSet::DoNothing:
		return Instruction::OpType::NoneOp;

	case InstructionSet::GetPointerOfStack:
	case InstructionSet::GetPointerOfStackSub:
		return Instruction::OpType::RegUInt16;

	case InstructionSet::DecrementStackPointer:
	case InstructionSet::IncrementStackPointer:
		return Instruction::OpType::OneReg;

	case InstructionSet::LoadEffectiveAddressA:
	case InstructionSet::LoadEffectiveAddressS:
	case InstructionSet::LoadEffectiveAddressX:
		return Instruction::OpType::TwoRegInt8;

	case InstructionSet::PointerMemberLoad8:
	case InstructionSet::PointerMemberLoad16:
	case InstructionSet::PointerMemberLoad32:
	case InstructionSet::PointerMemberLoad64:
		return Instruction::OpType::TwoRegInt8;
	case InstructionSet::PointerMemberRead8:
	case InstructionSet::PointerMemberRead16:
	case InstructionSet::PointerMemberRead32:
	case InstructionSet::PointerMemberRead64:
		return Instruction::OpType::TwoRegInt8;
	default:
		UCodeLangUnreachable();
		break;
	}
}
UCodeLangEnd