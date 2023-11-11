#pragma once
#include "UAssembly_NameSpace.hpp"
#include "UCodeLang/LangCore/UClib.hpp"

#define AddMapValueValue(InsName,Ins,Op_0,Op_1) {#InsName, {#InsName,Ins,Op_0,Op_1} }
#define AddMapValueValue2(InsName,Ins,Op_0,Op_1,Op_2) {#InsName, {#InsName,Ins,Op_0,Op_1,Op_2} }

UAssemblyStart
using TokenType_t = UInt8;
enum class TokenType : TokenType_t
{
	Null,
	Name,
	Number,
	Left_Parentheses,
	Right_Parentheses,
	Left_Bracket,
	Right_Bracket,
	Colon,
	Semicolon,
	Dot,
	plus,
	minus,
	Comma,
};

struct TokenValue
{
	TokenValue() :_Ptr(nullptr), _String(), _Int8(0), _Size_t(0)
	{

	}


	void* _Ptr;
	String_view _String;
	Int8 _Int8;

	size_t _Size_t;

	TokenValue& operator=(void* ptr)
	{
		_Ptr = ptr;
		return *this;
	}
	TokenValue& operator=(Int8 Value)
	{
		_Int8 = Value;
		return *this;
	}
	TokenValue& operator=(size_t Value)
	{
		_Size_t = Value;
		return *this;
	}
	TokenValue& operator=(String_view String_literal)
	{
		_String = String_literal;
		return *this;
	}
};

struct Token
{
	TokenType Type = TokenType::Null;
	TokenValue Value;

	size_t OnLine = 0;
	size_t OnPos = 0;
};
using OpCodeType_t = UInt8;
enum class OpCodeType :OpCodeType_t
{
	NoOpCode,

	AnyInt8,
	AnyInt16,
	AnyInt32,
	AnyInt64,
	Anyfloat32,
	Anyfloat64,

	Register,

	InsAddress,
	StaticCString,
};

struct InsMapValue
{
	String_view InsName;
	InstructionSet OpCode;
	OpCodeType Op_A;
	OpCodeType Op_B;
	OpCodeType Op_C;
};

#define MapValueIntSet(bitsize)\
AddMapValueValue2(Add##bitsize, InstructionSet::Add##bitsize, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register),\
AddMapValueValue2(Sub##bitsize, InstructionSet::Sub##bitsize, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register),\
AddMapValueValue2(MultU##bitsize, InstructionSet::MultU##bitsize, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register),\
AddMapValueValue2(MultS##bitsize, InstructionSet::MultS##bitsize, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register),\
AddMapValueValue2(DivU##bitsize, InstructionSet::DivU##bitsize, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register),\
AddMapValueValue2(DivS##bitsize, InstructionSet::DivS##bitsize, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register),\
AddMapValueValue(StoreRegOnStack##bitsize, InstructionSet::StoreRegOnStack##bitsize, OpCodeType::Register, OpCodeType::AnyInt16),\
AddMapValueValue(StoreRegOnStackSub##bitsize, InstructionSet::StoreRegOnStackSub##bitsize, OpCodeType::Register, OpCodeType::AnyInt16),\
AddMapValueValue(Push##bitsize, InstructionSet::Push##bitsize, OpCodeType::Register, OpCodeType::NoOpCode),\
AddMapValueValue(Pop##bitsize, InstructionSet::Pop##bitsize, OpCodeType::Register, OpCodeType::NoOpCode),\
AddMapValueValue(StoreRegToReg##bitsize, InstructionSet::StoreRegToReg##bitsize, OpCodeType::Register, OpCodeType::Register),\
AddMapValueValue(GetFromStack##bitsize, InstructionSet::GetFromStack##bitsize, OpCodeType::Register,OpCodeType::AnyInt16),\
AddMapValueValue(GetFromStackSub##bitsize, InstructionSet::GetFromStackSub##bitsize,OpCodeType::Register,OpCodeType::AnyInt16),\
AddMapValueValue(StoreRegToPtr##bitsize, InstructionSet::StoreRegToPtr##bitsize, OpCodeType::Register,OpCodeType::Register),\
AddMapValueValue(LNot##bitsize, InstructionSet::LogicalNot##bitsize, OpCodeType::Register, OpCodeType::Register), \
AddMapValueValue2(equalto##bitsize,InstructionSet::equalto##bitsize,OpCodeType::Register, OpCodeType::Register, OpCodeType::Register),\
AddMapValueValue(StoreFromPtrToReg##bitsize,InstructionSet::StoreFromPtrToReg##bitsize,OpCodeType::Register, OpCodeType::Register),\
AddMapValueValue2(lessthan##bitsize, InstructionSet::lessthan##bitsize, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register), \
AddMapValueValue2(greaterthan##bitsize, InstructionSet::greaterthan##bitsize, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register), \
AddMapValueValue2(equal_greaterthan##bitsize, InstructionSet::equal_greaterthan##bitsize, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register), \
AddMapValueValue2(equal_lessthan##bitsize, InstructionSet::equal_lessthan##bitsize, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register), \
AddMapValueValue2(notequalto##bitsize, InstructionSet::notequalto##bitsize, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register), \
AddMapValueValue2(LAnd##bitsize, InstructionSet::LogicalAnd##bitsize, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register), \
AddMapValueValue2(ModU##bitsize, InstructionSet::ModU##bitsize, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register), \
AddMapValueValue2(ModS##bitsize, InstructionSet::ModS##bitsize, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register), \

#define MapValuefloatSet(bitsize)\
AddMapValueValue(Addf##bitsize, InstructionSet::Addf##bitsize, OpCodeType::Register, OpCodeType::Register),\
AddMapValueValue(Subf##bitsize, InstructionSet::Subf##bitsize, OpCodeType::Register, OpCodeType::Register),\
AddMapValueValue(Multf##bitsize, InstructionSet::Multf##bitsize, OpCodeType::Register, OpCodeType::Register),\
AddMapValueValue(Divf##bitsize, InstructionSet::Divf##bitsize, OpCodeType::Register, OpCodeType::Register),\
AddMapValueValue2(equaltof##bitsize, InstructionSet::equaltof##bitsize, OpCodeType::Register, OpCodeType::Register,OpCodeType::Register),\
AddMapValueValue2(notequaltof##bitsize, InstructionSet::notequaltof##bitsize, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register), \

static inline const Unordered_map<String_view, InsMapValue> StringToInsMap =
{
	AddMapValueValue(Exit,InstructionSet::Exit,OpCodeType::AnyInt8,OpCodeType::NoOpCode),
	AddMapValueValue(Ret,InstructionSet::Return,OpCodeType::NoOpCode,OpCodeType::NoOpCode),

	AddMapValueValue(Callv1,InstructionSet::Callv1,OpCodeType::InsAddress,OpCodeType::NoOpCode),
	AddMapValueValue(Callv2,InstructionSet::Callv2,OpCodeType::InsAddress,OpCodeType::NoOpCode),
	AddMapValueValue(Callv3,InstructionSet::Callv3,OpCodeType::InsAddress,OpCodeType::NoOpCode),
	AddMapValueValue(Callv4,InstructionSet::Callv4,OpCodeType::InsAddress,OpCodeType::NoOpCode),
	
	AddMapValueValue(Callif,InstructionSet::CallIf,OpCodeType::Register,OpCodeType::InsAddress),
	AddMapValueValue(CallReg,InstructionSet::CallReg,OpCodeType::Register,OpCodeType::NoOpCode),

	AddMapValueValue(Jumpv1,InstructionSet::Jumpv1,OpCodeType::InsAddress,OpCodeType::NoOpCode),
	AddMapValueValue(Jumpv2,InstructionSet::Jumpv2,OpCodeType::InsAddress,OpCodeType::NoOpCode),
	AddMapValueValue(Jumpv3,InstructionSet::Jumpv3,OpCodeType::InsAddress,OpCodeType::NoOpCode),
	AddMapValueValue(Jumpv4,InstructionSet::Jumpv4,OpCodeType::InsAddress,OpCodeType::NoOpCode),

	AddMapValueValue(Jumpif,InstructionSet::Jumpif,OpCodeType::Register,OpCodeType::InsAddress),
	AddMapValueValue(JumpReg,InstructionSet::JumpReg,OpCodeType::Register,OpCodeType::NoOpCode),

	AddMapValueValue(NoOp,InstructionSet::DoNothing,OpCodeType::NoOpCode,OpCodeType::NoOpCode),

	AddMapValueValue(Store8,InstructionSet::Store8,OpCodeType::Register,OpCodeType::AnyInt8),
	AddMapValueValue(Store16,InstructionSet::Store16,OpCodeType::Register,OpCodeType::AnyInt16),

	AddMapValueValue(Store32V1,InstructionSet::Store32v1,OpCodeType::Register,OpCodeType::AnyInt16),
	AddMapValueValue(Store32V2,InstructionSet::Store32v2,OpCodeType::Register,OpCodeType::AnyInt16),

	AddMapValueValue(Store64V1,InstructionSet::Store64v1,OpCodeType::Register,OpCodeType::AnyInt16),
	AddMapValueValue(Store64V2,InstructionSet::Store64v2,OpCodeType::Register,OpCodeType::AnyInt16),
	AddMapValueValue(Store64V3,InstructionSet::Store64v3,OpCodeType::Register,OpCodeType::AnyInt16),
	AddMapValueValue(Store64V4,InstructionSet::Store64v4,OpCodeType::Register,OpCodeType::AnyInt16),

	MapValueIntSet(8)
	MapValueIntSet(16)
	MapValueIntSet(32)
	MapValueIntSet(64)
	
	MapValuefloatSet(32)

	MapValuefloatSet(64)

	AddMapValueValue(Storef32V1,InstructionSet::Storef32v1,OpCodeType::Register,OpCodeType::AnyInt16),
	AddMapValueValue(Storef32V2,InstructionSet::Storef32v2,OpCodeType::Register,OpCodeType::AnyInt16),

	AddMapValueValue(Storef64V1,InstructionSet::Storef64v1,OpCodeType::Register,OpCodeType::AnyInt16),
	AddMapValueValue(Storef64V2,InstructionSet::Storef64v2,OpCodeType::Register,OpCodeType::AnyInt16),
	AddMapValueValue(Storef64V3,InstructionSet::Storef64v3,OpCodeType::Register,OpCodeType::AnyInt16),
	AddMapValueValue(Storef64V4,InstructionSet::Storef64v4,OpCodeType::Register,OpCodeType::AnyInt16),

	AddMapValueValue(UInt8ToUInt16,InstructionSet::UInt8ToUInt16,OpCodeType::Register,OpCodeType::Register),
	AddMapValueValue(UInt16ToUInt32,InstructionSet::UInt16ToUInt32,OpCodeType::Register,OpCodeType::Register),
	AddMapValueValue(UInt32ToUInt64,InstructionSet::UInt32ToUInt64,OpCodeType::Register,OpCodeType::Register),

	AddMapValueValue(UInt64ToUInt32,InstructionSet::UInt64ToUInt32,OpCodeType::Register,OpCodeType::Register),
	AddMapValueValue(UInt32ToUInt16,InstructionSet::UInt32ToUInt16,OpCodeType::Register,OpCodeType::Register),
	AddMapValueValue(UInt16ToUInt8,InstructionSet::UInt16ToUInt8,OpCodeType::Register,OpCodeType::Register),

	AddMapValueValue(SInt8ToSInt16,InstructionSet::SInt8ToSInt16,OpCodeType::Register,OpCodeType::Register),
	AddMapValueValue(SInt16ToSInt32,InstructionSet::SInt16ToSInt32,OpCodeType::Register,OpCodeType::Register),
	AddMapValueValue(SInt32ToSInt64,InstructionSet::SInt32ToSInt64,OpCodeType::Register,OpCodeType::Register),

	AddMapValueValue(SInt64ToSInt32,InstructionSet::SInt64ToSInt32,OpCodeType::Register,OpCodeType::Register),
	AddMapValueValue(SInt32ToSInt16,InstructionSet::SInt32ToSInt16,OpCodeType::Register,OpCodeType::Register),
	AddMapValueValue(SInt16ToSInt8,InstructionSet::SInt16ToSInt8,OpCodeType::Register,OpCodeType::Register),

	AddMapValueValue(SInt32Tofloat32,InstructionSet::SInt32Tofloat32,OpCodeType::Register,OpCodeType::Register),
	AddMapValueValue(SInt64Tofloat64,InstructionSet::SInt64Tofloat64,OpCodeType::Register,OpCodeType::Register),


	AddMapValueValue(UInt32Tofloat32,InstructionSet::UInt32Tofloat32,OpCodeType::Register,OpCodeType::Register),
	AddMapValueValue(UInt64Tofloat64,InstructionSet::UInt64Tofloat64,OpCodeType::Register,OpCodeType::Register),

	AddMapValueValue(float32ToInt32,InstructionSet::float32ToInt32,OpCodeType::Register,OpCodeType::Register),
	AddMapValueValue(float64ToInt64,InstructionSet::float64ToInt64,OpCodeType::Register,OpCodeType::Register),

	AddMapValueValue(float32Tofloat64,InstructionSet::float32Tofloat64,OpCodeType::Register,OpCodeType::Register),
	AddMapValueValue(float64Tofloat32,InstructionSet::float64Tofloat32,OpCodeType::Register,OpCodeType::Register),

	AddMapValueValue(Malloc,InstructionSet::Malloc,OpCodeType::Register,OpCodeType::Register),
	AddMapValueValue(Free,InstructionSet::Free,OpCodeType::Register,OpCodeType::NoOpCode),

	AddMapValueValue(LoadFuncPtrv1,InstructionSet::LoadFuncPtrV1,OpCodeType::Register,OpCodeType::InsAddress),
	AddMapValueValue(LoadFuncPtrv2,InstructionSet::LoadFuncPtrV2,OpCodeType::Register,OpCodeType::InsAddress),
	AddMapValueValue(LoadFuncPtrv3,InstructionSet::LoadFuncPtrV3,OpCodeType::Register,OpCodeType::InsAddress),
	AddMapValueValue(LoadFuncPtrv4,InstructionSet::LoadFuncPtrV4,OpCodeType::Register,OpCodeType::InsAddress),

	AddMapValueValue(GetPointerOfStack,InstructionSet::GetPointerOfStack,OpCodeType::Register,OpCodeType::AnyInt16),
	AddMapValueValue(GetPointerOfStackSub,InstructionSet::GetPointerOfStackSub,OpCodeType::Register,OpCodeType::AnyInt16),

	AddMapValueValue(IncrementStackPointer,InstructionSet::IncrementStackPointer,OpCodeType::Register,OpCodeType::NoOpCode),
	AddMapValueValue(DecrementStackPointer,InstructionSet::DecrementStackPointer,OpCodeType::Register,OpCodeType::NoOpCode),

	AddMapValueValue(Call_Code,InstructionSet::Call_Code,OpCodeType::AnyInt16,OpCodeType::NoOpCode),

	AddMapValueValue(CppCallNamed,InstructionSet::CppCallNamed,OpCodeType::StaticCString,OpCodeType::NoOpCode),

	AddMapValueValue2(LEA_A,InstructionSet::LoadEffectiveAddressA,OpCodeType::Register,OpCodeType::Register,OpCodeType::AnyInt8),
	AddMapValueValue2(LEA_S,InstructionSet::LoadEffectiveAddressS,OpCodeType::Register,OpCodeType::Register,OpCodeType::AnyInt8),
	AddMapValueValue2(LEA_X,InstructionSet::LoadEffectiveAddressX,OpCodeType::Register,OpCodeType::Register,OpCodeType::AnyInt8),

	AddMapValueValue2(Load8,InstructionSet::PointerMemberLoad8,OpCodeType::Register,OpCodeType::Register,OpCodeType::AnyInt8),
	AddMapValueValue2(Load16,InstructionSet::PointerMemberLoad16,OpCodeType::Register,OpCodeType::Register,OpCodeType::AnyInt8),
	AddMapValueValue2(Load32,InstructionSet::PointerMemberLoad32,OpCodeType::Register,OpCodeType::Register,OpCodeType::AnyInt8),
	AddMapValueValue2(Load64,InstructionSet::PointerMemberLoad64,OpCodeType::Register,OpCodeType::Register,OpCodeType::AnyInt8),

	AddMapValueValue2(Read8,InstructionSet::PointerMemberRead8,OpCodeType::Register,OpCodeType::Register,OpCodeType::AnyInt8),
	AddMapValueValue2(Read16,InstructionSet::PointerMemberRead16,OpCodeType::Register,OpCodeType::Register,OpCodeType::AnyInt8),
	AddMapValueValue2(Read32,InstructionSet::PointerMemberRead32,OpCodeType::Register,OpCodeType::Register,OpCodeType::AnyInt8),
	AddMapValueValue2(Read64,InstructionSet::PointerMemberRead64,OpCodeType::Register,OpCodeType::Register,OpCodeType::AnyInt8),

	AddMapValueValue(GetPointerOfStaticMem,InstructionSet::GetPointerOfStaticMem,OpCodeType::Register,OpCodeType::AnyInt16),
	AddMapValueValue(GetPointerOfThreadMem,InstructionSet::GetPointerOfThreadMem,OpCodeType::Register,OpCodeType::AnyInt16),

	AddMapValueValue(Debug_FuncStart,InstructionSet::Debug_FuncStart,OpCodeType::NoOpCode,OpCodeType::NoOpCode),
	AddMapValueValue(Debug_FuncEnd,InstructionSet::Debug_FuncEnd,OpCodeType::NoOpCode,OpCodeType::NoOpCode),
	AddMapValueValue(Debug_LineEnter,InstructionSet::Debug_LineEnter,OpCodeType::NoOpCode,OpCodeType::NoOpCode),

	AddMapValueValue2(BOr8, InstructionSet::bitwiseOr8, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register),
	AddMapValueValue2(BOr16, InstructionSet::bitwiseOr16, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register),
	AddMapValueValue2(BOr32, InstructionSet::bitwiseOr32, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register),
	AddMapValueValue2(BOr64, InstructionSet::bitwiseOr64, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register),

	AddMapValueValue2(Xor8, InstructionSet::bitwiseXor8, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register),
	AddMapValueValue2(Xor16, InstructionSet::bitwiseXor16, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register),
	AddMapValueValue2(Xor32, InstructionSet::bitwiseXor32, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register),
	AddMapValueValue2(Xor64, InstructionSet::bitwiseXor64, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register),

	AddMapValueValue2(LShift8, InstructionSet::bitwiseLeftShift8, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register),
	AddMapValueValue2(LShift16, InstructionSet::bitwiseLeftShift16, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register),
	AddMapValueValue2(LShift32, InstructionSet::bitwiseLeftShift32, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register),
	AddMapValueValue2(LShift64, InstructionSet::bitwiseLeftShift64, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register),

	AddMapValueValue2(RShift8, InstructionSet::bitwiseRightShift8, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register),
	AddMapValueValue2(RShift16, InstructionSet::bitwiseRightShift16, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register),
	AddMapValueValue2(RShift32, InstructionSet::bitwiseRightShift32, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register),
	AddMapValueValue2(RShift64, InstructionSet::bitwiseRightShift64, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register),

	AddMapValueValue2(BAnd8, InstructionSet::bitwiseAnd8, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register),
	AddMapValueValue2(BAnd16, InstructionSet::bitwiseAnd16, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register),
	AddMapValueValue2(BAnd32, InstructionSet::bitwiseAnd32, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register),
	AddMapValueValue2(BAnd64, InstructionSet::bitwiseAnd64, OpCodeType::Register, OpCodeType::Register, OpCodeType::Register),
	
	AddMapValueValue(BNot8, InstructionSet::bitwise_Not8, OpCodeType::Register, OpCodeType::Register), 
	AddMapValueValue(BNot16, InstructionSet::bitwise_Not16, OpCodeType::Register, OpCodeType::Register),
	AddMapValueValue(BNot32, InstructionSet::bitwise_Not32, OpCodeType::Register, OpCodeType::Register),
	AddMapValueValue(BNot64, InstructionSet::bitwise_Not64, OpCodeType::Register, OpCodeType::Register),
};

thread_local inline Unordered_map<InstructionSet, const InsMapValue*> InsToInsMapValue;
inline void SetUp()
{
	for (auto& Item : StringToInsMap)
	{
		InsToInsMapValue[Item.second.OpCode] = &Item.second;
	}
}
inline auto& Get_InsToInsMapValue()
{
	if (InsToInsMapValue.size() == 0)
	{
		SetUp();
	}
	return InsToInsMapValue;
}
UAssemblyEnd