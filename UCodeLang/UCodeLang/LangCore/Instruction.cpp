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
	case InstructionSet::Add##IntSize: return Instruction::OpType::ThreeReg;\
	case InstructionSet::Sub##IntSize: return Instruction::OpType::ThreeReg;\
	case InstructionSet::MultS##IntSize: return Instruction::OpType::ThreeReg;\
	case InstructionSet::MultU##IntSize: return Instruction::OpType::ThreeReg;\
	case InstructionSet::DivS##IntSize: return Instruction::OpType::ThreeReg;\
	case InstructionSet::DivU##IntSize: return Instruction::OpType::ThreeReg;\
	case InstructionSet::LogicalAnd##IntSize: return Instruction::OpType::ThreeReg;\
	case InstructionSet::Logicalor##IntSize: return Instruction::OpType::ThreeReg;\
	case InstructionSet::LogicalNot##IntSize: return Instruction::OpType::TwoReg;\
	case InstructionSet::equalto##IntSize: return Instruction::OpType::ThreeReg;\
	case InstructionSet::notequalto##IntSize: return Instruction::OpType::ThreeReg;\
	case InstructionSet::lessthan##IntSize: return Instruction::OpType::ThreeReg; \
	case InstructionSet::greaterthan##IntSize: return Instruction::OpType::ThreeReg; \
	case InstructionSet::equal_lessthan##IntSize: return Instruction::OpType::ThreeReg; \
	case InstructionSet::equal_greaterthan##IntSize: return Instruction::OpType::ThreeReg; \
	case InstructionSet::bitwiseAnd##IntSize: return Instruction::OpType::ThreeReg; \
	case InstructionSet::bitwiseOr##IntSize: return Instruction::OpType::ThreeReg; \
	case InstructionSet::bitwiseLeftShift##IntSize: return Instruction::OpType::ThreeReg; \
	case InstructionSet::bitwiseRightShift##IntSize: return Instruction::OpType::ThreeReg; \
	case InstructionSet::bitwiseXor##IntSize: return Instruction::OpType::ThreeReg; \
	case InstructionSet::bitwise_Not##IntSize: return Instruction::OpType::TwoReg; \
	case InstructionSet::ModS##IntSize: return Instruction::OpType::ThreeReg; \
	case InstructionSet::ModU##IntSize: return Instruction::OpType::ThreeReg; \


#define Setfloat(IntSize)\
case InstructionSet::Addf##IntSize: return Instruction::OpType::ThreeReg; \
case InstructionSet::Subf##IntSize: return Instruction::OpType::ThreeReg; \
case InstructionSet::Multf##IntSize: return Instruction::OpType::ThreeReg; \
case InstructionSet::Divf##IntSize: return Instruction::OpType::ThreeReg; \
case InstructionSet::equaltof##IntSize: return Instruction::OpType::ThreeReg; \
case InstructionSet::notequaltof##IntSize: return Instruction::OpType::ThreeReg; \

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
	case InstructionSet::Callv1:
	case InstructionSet::Callv2:
	case InstructionSet::Callv3:
	case InstructionSet::Callv4:
	case InstructionSet::Jumpv1:
	case InstructionSet::Jumpv2:
	case InstructionSet::Jumpv3:
	case InstructionSet::Jumpv4:
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

	case InstructionSet::float32Tofloat64:
	case InstructionSet::float64Tofloat32:
	case InstructionSet::float32ToInt32:
	case InstructionSet::float64ToInt64:
	case InstructionSet::SInt32Tofloat32:
	case InstructionSet::SInt64Tofloat64:
	case InstructionSet::UInt32Tofloat32:
	case InstructionSet::UInt64Tofloat64:
	case InstructionSet::UInt8ToUInt16:
	case InstructionSet::UInt16ToUInt32:
	case InstructionSet::UInt32ToUInt64:
	case InstructionSet::UInt64ToUInt32:
	case InstructionSet::UInt32ToUInt16:
	case InstructionSet::UInt16ToUInt8:
	case InstructionSet::SInt8ToSInt16:
	case InstructionSet::SInt16ToSInt32:
	case InstructionSet::SInt32ToSInt64:
	case InstructionSet::SInt64ToSInt32:
	case InstructionSet::SInt32ToSInt16:
	case InstructionSet::SInt16ToSInt8:
	case InstructionSet::Malloc:
		return Instruction::OpType::TwoReg;
	case InstructionSet::Free:
		return Instruction::OpType::OneReg;

	case InstructionSet::DoNothing:
		return Instruction::OpType::NoneOp;

	case InstructionSet::GetPointerOfStaticMem:
	case InstructionSet::GetPointerOfThreadMem:
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
	case InstructionSet::LoadFuncPtrV1:
	case InstructionSet::LoadFuncPtrV2:
	case InstructionSet::LoadFuncPtrV3:
	case InstructionSet::LoadFuncPtrV4:
		return Instruction::OpType::RegUInt16;
	default:
		UCodeLangUnreachable();
		break;
	}
}
UCodeLangEnd