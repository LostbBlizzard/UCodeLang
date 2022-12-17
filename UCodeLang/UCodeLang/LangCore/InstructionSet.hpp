#pragma once
#include "UCodeLangNameSpace.hpp"
#include "LangTypes.hpp"
UCodeLangStart


using ExitState_t = UInt8;
enum class ExitState : ExitState_t
{
	Null,
	Success,
	Failure,
};
using InstructionSet_t = UInt64;
enum class InstructionSet : InstructionSet_t
{
	Exit,Return,
	
	Call,CallIf,CallPtr,
	
	Jump, Jumpif,JumpPtr,

	DoNothing,
	
	//Set 8 Bits

	Store8, Store8FromPtrToReg, Store8RegToPtr,Push8,Pop8, Store8RegToReg, Store8RegOnStack, Get8FromStack,
	Get8FromStackSub,
	
	AddS8, AddU8, SubS8, SubU8, MultS8, MultU8, DivS8, DivU8,
	
	LogicalAnd8, Logicalor8,LogicalNot8,

	equalto8, notequalto8, lessthan8, greaterthan8, equal_lessthan8, equal_greaterthan8,

	bitwiseAnd8, bitwiseOr, bitwiseLeftShift8, bitwiseRightShift8, bitwiseXor8, bitwise_Not8,
	//Set 32
	Store32, Store32FromPtrToReg, Store32RegToPtr, Push32, Pop32,


	//Set 64
	Store64, Store64FromPtrToReg, Store64RegToPtr,Push64, Pop64, Store64RegToReg,
	AddS64, AddU64, SubS64, SubU64, MultS64, MultU64, DivS64, DivU64,

	//Not Real Instructions The RunTimeLib will tanslate them
	StoreNativeU, StoreNativeS, StoreNativeFromPtrToReg, StoreNativeRegToPtr, PushNative, PopNative, StoreNativeRegToReg,
	AddSNative, AddUNative, SubSNative, SubUNative, MultSNative, MultUNative, DivSNative, DivUNative,
	//Cpp func Set
	GetPointerOfStack, GetPointerOfStaticMem,IncrementStackPointer, DecrementStackPointer,
	
	Malloc,Free,MemCopy, Calloc,ReAlloc,
	Log,CppCall,Link_Bytes,Link_Path,

	MAXVALUE,

	NativeSetMin = StoreNativeU,
	NativeSetMax = GetPointerOfStack -1,
};
enum class Intermediate_Set : InstructionSet_t
{
	Null = (InstructionSet_t)InstructionSet::MAXVALUE,
	TepFuncCall,
	GetRawStringStaticOffset,
	
	
	DeclareVar,
	DeclareParameter,
	DeclareStaticVar,
	DeclareThisVar,
	DeclareThis,

	PushParameter,
	PopParameter,
	CallFunc,

	GetVar,
	StoreVar,
	Ret,

	DeclareFunc,
	FuncEnd,
	
	StoreBool,
	StoreNumber,

	DeclareExpression,
	DeclareBinaryExpression,
	
	Binary_plus,//Is use in DeclareBinaryExpression value 0
	Binary_minus,
	Binary_Mult,
	Binary_Div,

	Binary_logical_and,
	Binary_logical_or,

	Binary_equal_Comparison,
	Binary_Notequal_Comparison,
	Bianry_greaterthan,
	Bianry_Lessthan,
	Bianry_less_than_or_equalto,
	Bianry_greater_than_or_equalto,

	Bianry_bitwise_and,
	Bianry_bitwise_or, 
	
	Bianry_bitwise_LeftShift,
	Bianry_bitwise_RightShift,
	Bianry_bitwise_Xor,

	UnaryExpression,
	bitwise_not,

	AsmBlock,
	FileEnd,

	MAXVALUE
};
using RegisterID_t = UInt8;
enum class RegisterID : RegisterID_t
{
	A, B, C, D, E, F,

	//
	StartRegister = (RegisterID_t)RegisterID::A,
	EndRegister = (RegisterID_t)RegisterID::F,


	ThisRegister = (RegisterID_t)RegisterID::D,
	OuPutRegister = (RegisterID_t)RegisterID::E,
	InPutRegister = (RegisterID_t)RegisterID::F,

	PopAndTrashRegister = (RegisterID_t)RegisterID::D,
	MathOuPutRegister = OuPutRegister,
	BoolRegister = OuPutRegister,
	BitwiseRegister = OuPutRegister,

	NullRegister = 155,
};
UCodeLangEnd