#pragma once
#include "UAssembly_NameSpace.hpp"
#include "UCodeLang/LangCore/UClib.hpp"

#define AddMapValueValue(InsName,Ins,Op_0,Op_1) {#InsName, {#InsName,Ins,Op_0,Op_1} }

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

	UIntPtr,
	InsAddress,
	StaticCString,
};

struct InsMapValue
{
	String_view InsName;
	InstructionSet OpCode;
	OpCodeType Op_0;
	OpCodeType Op_1;
};

#define MapValueIntSet(bitsize)\
AddMapValueValue(Store##bitsize, InstructionSet::Store##bitsize, OpCodeType::Register, OpCodeType::AnyInt##bitsize),\
AddMapValueValue(Add##bitsize, InstructionSet::Add##bitsize, OpCodeType::Register, OpCodeType::Register),\
AddMapValueValue(Sub##bitsize, InstructionSet::Sub##bitsize, OpCodeType::Register, OpCodeType::Register),\
AddMapValueValue(MultU##bitsize, InstructionSet::MultU##bitsize, OpCodeType::Register, OpCodeType::Register),\
AddMapValueValue(MultS##bitsize, InstructionSet::MultS##bitsize, OpCodeType::Register, OpCodeType::Register),\
AddMapValueValue(DivU##bitsize, InstructionSet::DivU##bitsize, OpCodeType::Register, OpCodeType::Register),\
AddMapValueValue(DivS##bitsize, InstructionSet::DivS##bitsize, OpCodeType::Register, OpCodeType::Register),\
AddMapValueValue(StoreRegOnStack##bitsize, InstructionSet::StoreRegOnStack##bitsize, OpCodeType::Register, OpCodeType::UIntPtr),\
AddMapValueValue(StoreRegOnStackSub##bitsize, InstructionSet::StoreRegOnStackSub##bitsize, OpCodeType::Register, OpCodeType::UIntPtr),\
AddMapValueValue(Push##bitsize, InstructionSet::Push##bitsize, OpCodeType::Register, OpCodeType::NoOpCode),\
AddMapValueValue(Pop##bitsize, InstructionSet::Pop##bitsize, OpCodeType::Register, OpCodeType::NoOpCode),\
AddMapValueValue(StoreRegToReg##bitsize, InstructionSet::StoreRegToReg##bitsize, OpCodeType::Register, OpCodeType::Register),\
AddMapValueValue(GetFromStack##bitsize, InstructionSet::GetFromStack##bitsize, OpCodeType::UIntPtr,OpCodeType::Register),\
AddMapValueValue(GetFromStackSub##bitsize, InstructionSet::GetFromStackSub##bitsize, OpCodeType::UIntPtr,OpCodeType::Register),\
AddMapValueValue(StoreRegToPtr##bitsize, InstructionSet::StoreRegToPtr##bitsize, OpCodeType::Register,OpCodeType::Register),\
AddMapValueValue(LogicalNot##bitsize, InstructionSet::LogicalNot##bitsize, OpCodeType::Register, OpCodeType::Register), \
AddMapValueValue(equalto##bitsize,InstructionSet::equalto##bitsize,OpCodeType::Register, OpCodeType::Register),\
AddMapValueValue(StoreFromPtrToReg##bitsize,InstructionSet::StoreFromPtrToReg##bitsize,OpCodeType::Register, OpCodeType::Register),\
AddMapValueValue(UInt##bitsize##ToSInt##bitsize,InstructionSet::UInt##bitsize##ToSInt##bitsize,OpCodeType::Register, OpCodeType::Register),\
AddMapValueValue(SInt##bitsize##ToUInt##bitsize,InstructionSet::SInt##bitsize##ToUInt##bitsize,OpCodeType::Register, OpCodeType::Register),\


#define MapValuefloatSet(bitsize)\
AddMapValueValue(Store##bitsize##f, InstructionSet::Store##bitsize##f, OpCodeType::Register, OpCodeType::Anyfloat##bitsize),\


static inline const Unordered_map<String_view, InsMapValue> StringToInsMap =
{	
	AddMapValueValue(Exit,InstructionSet::Exit,OpCodeType::AnyInt8,OpCodeType::NoOpCode),
	AddMapValueValue(Ret,InstructionSet::Return,OpCodeType::NoOpCode,OpCodeType::NoOpCode),
	AddMapValueValue(Call,InstructionSet::Call,OpCodeType::InsAddress,OpCodeType::NoOpCode),
	AddMapValueValue(Callif,InstructionSet::CallIf,OpCodeType::InsAddress,OpCodeType::Register),
	AddMapValueValue(CallReg,InstructionSet::CallReg,OpCodeType::Register,OpCodeType::NoOpCode),

	AddMapValueValue(Jump,InstructionSet::Jump,OpCodeType::InsAddress,OpCodeType::NoOpCode),
	AddMapValueValue(Jumpif,InstructionSet::Jumpif,OpCodeType::InsAddress,OpCodeType::Register),
	AddMapValueValue(JumpReg,InstructionSet::JumpReg,OpCodeType::Register,OpCodeType::NoOpCode),

	AddMapValueValue(NoOp,InstructionSet::DoNothing,OpCodeType::NoOpCode,OpCodeType::NoOpCode),

	MapValueIntSet(8)
	MapValueIntSet(16)
	MapValueIntSet(32)
	MapValueIntSet(64)
	
	MapValuefloatSet(32)

	MapValuefloatSet(64)

	AddMapValueValue(Int8ToInt16,InstructionSet::Int8ToInt16,OpCodeType::Register,OpCodeType::Register),
	AddMapValueValue(Int16ToInt32,InstructionSet::Int16ToInt32,OpCodeType::Register,OpCodeType::Register),
	AddMapValueValue(Int32ToInt64,InstructionSet::Int32ToInt64,OpCodeType::Register,OpCodeType::Register),

	AddMapValueValue(Int64ToInt32,InstructionSet::Int8ToInt16,OpCodeType::Register,OpCodeType::Register),
	AddMapValueValue(Int32ToInt16,InstructionSet::Int16ToInt32,OpCodeType::Register,OpCodeType::Register),
	AddMapValueValue(Int16ToInt8,InstructionSet::Int16ToInt8,OpCodeType::Register,OpCodeType::Register),

	AddMapValueValue(float32ToInt32,InstructionSet::float32ToInt32,OpCodeType::Register,OpCodeType::Register),
	AddMapValueValue(float64ToInt64,InstructionSet::float64ToInt64,OpCodeType::Register,OpCodeType::Register),

	AddMapValueValue(Int32Tofloat32,InstructionSet::Int32Tofloat32,OpCodeType::Register,OpCodeType::Register),
	AddMapValueValue(Int64Tofloat64,InstructionSet::Int64Tofloat64,OpCodeType::Register,OpCodeType::Register),

	AddMapValueValue(Malloc,InstructionSet::Malloc,OpCodeType::Register,OpCodeType::Register),
	AddMapValueValue(Free,InstructionSet::Free,OpCodeType::Register,OpCodeType::NoOpCode),
	AddMapValueValue(LoadFuncPtr,InstructionSet::LoadFuncPtr,OpCodeType::InsAddress,OpCodeType::Register),
	AddMapValueValue(GetPointerOfStack,InstructionSet::GetPointerOfStack,OpCodeType::Register,OpCodeType::UIntPtr),
	AddMapValueValue(GetPointerOfStackSub,InstructionSet::GetPointerOfStackSub,OpCodeType::Register,OpCodeType::UIntPtr),

	AddMapValueValue(IncrementStackPointer,InstructionSet::IncrementStackPointer,OpCodeType::Register,OpCodeType::NoOpCode),
	AddMapValueValue(DecrementStackPointer,InstructionSet::DecrementStackPointer,OpCodeType::Register,OpCodeType::NoOpCode),

	AddMapValueValue(Call_Code,InstructionSet::Call_Code,OpCodeType::UIntPtr,OpCodeType::NoOpCode),

	AddMapValueValue(CppCallNamed,InstructionSet::CppCallNamed,OpCodeType::StaticCString,OpCodeType::NoOpCode),
	AddMapValueValue(SysCall,InstructionSet::SysCall,OpCodeType::AnyInt64,OpCodeType::Register),
};

static inline Unordered_map<InstructionSet, const InsMapValue*> InsToInsMapValue;
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