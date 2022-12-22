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
	TokenType Type;
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
	AnyIntNative,

	Register,

	UIntPtr,
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
AddMapValueValue(StoreRegOnStack##bitsize, InstructionSet::StoreRegOnStack##bitsize, OpCodeType::Register, OpCodeType::UIntPtr),\
AddMapValueValue(Push##bitsize, InstructionSet::Push##bitsize, OpCodeType::Register, OpCodeType::NoOpCode),\
AddMapValueValue(Pop##bitsize, InstructionSet::Pop##bitsize, OpCodeType::Register, OpCodeType::NoOpCode),\
AddMapValueValue(StoreRegToReg##bitsize, InstructionSet::StoreRegToReg##bitsize, OpCodeType::Register, OpCodeType::Register),\
AddMapValueValue(GetFromStack##bitsize, InstructionSet::GetFromStack##bitsize, OpCodeType::Register, OpCodeType::UIntPtr),\

static inline const unordered_map<String_view, InsMapValue> StringToInsMap =
{	
	AddMapValueValue(Exit,InstructionSet::Exit,OpCodeType::AnyInt8,OpCodeType::NoOpCode),
	AddMapValueValue(Ret,InstructionSet::Return,OpCodeType::NoOpCode,OpCodeType::NoOpCode),
	AddMapValueValue(Call,InstructionSet::Call,OpCodeType::UIntPtr,OpCodeType::NoOpCode),
	AddMapValueValue(Callif,InstructionSet::CallIf,OpCodeType::UIntPtr,OpCodeType::NoOpCode),
	AddMapValueValue(CallPtr,InstructionSet::CallPtr,OpCodeType::UIntPtr,OpCodeType::NoOpCode),

	AddMapValueValue(Jump,InstructionSet::Jump,OpCodeType::UIntPtr,OpCodeType::NoOpCode),
	AddMapValueValue(Jumpif,InstructionSet::Jumpif,OpCodeType::UIntPtr,OpCodeType::NoOpCode),
	AddMapValueValue(JumpPtr,InstructionSet::JumpPtr,OpCodeType::UIntPtr,OpCodeType::NoOpCode),

	AddMapValueValue(NoOp,InstructionSet::DoNothing,OpCodeType::NoOpCode,OpCodeType::NoOpCode),

	MapValueIntSet(8)
	MapValueIntSet(16)
	MapValueIntSet(32)
	MapValueIntSet(64)
	MapValueIntSet(Native)
};

static inline unordered_map<InstructionSet, const InsMapValue*> InsToInsMapValue;
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