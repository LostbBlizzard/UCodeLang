#pragma once
#include "UCodeLangNameSpace.hpp"
#include "LangTypes.hpp"
UCodeLangStart


using ExitState_t = UInt8;
enum class ExitState : ExitState_t
{
	Success,
	Failure,
	Null,
};
using InstructionSet_t = UInt64;

#define AddSetInt(bitsize) \
Store##bitsize, StoreFromPtrToReg##bitsize, StoreRegToPtr##bitsize, Push##bitsize, Pop##bitsize, StoreRegToReg##bitsize, StoreRegOnStack##bitsize,StoreRegOnStackSub##bitsize, GetFromStack##bitsize, \
GetFromStackSub##bitsize,\
Add##bitsize, Sub##bitsize, MultS##bitsize, MultU##bitsize, DivS##bitsize, DivU##bitsize,\
LogicalAnd##bitsize, Logicalor##bitsize, LogicalNot##bitsize,\
equalto##bitsize, notequalto##bitsize, lessthan##bitsize, greaterthan##bitsize, equal_lessthan##bitsize, equal_greaterthan##bitsize,\
bitwiseAnd##bitsize, bitwiseOr##bitsize, bitwiseLeftShift##bitsize, bitwiseRightShift##bitsize, bitwiseXor##bitsize, bitwise_Not##bitsize,\
UInt##bitsize##ToSInt##bitsize,SInt##bitsize##ToUInt##bitsize,\

#define AddSetfloat(bitsize)\
Store##bitsize##f,

enum class InstructionSet : InstructionSet_t
{
	Exit,Return,
	
	Call,CallIf,CallReg,
	
	Jump, Jumpif,JumpReg,

	DoNothing,
	
	//Set 8 Bits
	AddSetInt(8)
	//Set 16
	AddSetInt(16)
	//Set 32
	AddSetInt(32)
	//Set 64
	AddSetInt(64)

	//float 32
	AddSetfloat(32)
	//float 64
	AddSetfloat(64)

	//Casting Set
	Int8ToInt16,
	Int16ToInt32,
	Int32ToInt64,

	Int64ToInt32,
	Int32ToInt16,
	Int16ToInt8,

	float32ToInt32,
	float64ToInt64,

	Int32Tofloat32,
	Int64Tofloat64,
	//
	PointerMemberLoad8,
	PointerMemberRead8,

	PointerMemberLoad16,
	PointerMemberRead16,

	PointerMemberLoad32,
	PointerMemberRead32,

	PointerMemberLoad64,
	PointerMemberRead64,

	//Cpp func Set
	GetPointerOfStack, GetPointerOfStackSub, GetPointerOfStaticMem, GetPointerOfThreadMem,IncrementStackPointer, DecrementStackPointer,
	
	Malloc,Free,MemCopy, Calloc,ReAlloc,LoadFuncPtr,
	CPPCall,CppCallNamed,Link_Bytes,Link_Path,
	SysCall,
	Call_Code,

	//Debuging
	Debug_FuncStart,
	Debug_FuncEnd,
	Debug_LineEnter,

	MAXVALUE,
};
enum class Intermediate_Set : InstructionSet_t
{
	Null = (InstructionSet_t)InstructionSet::MAXVALUE,
	TepFuncCall,
	GetRawStringStaticOffset,
	
	
	DeclareVar,
	DeclareParameter,
	DeclareStaticVar,
	DeclareThreadVar,
	DeclareThisVar,
	DeclareThisParameter,


	AssignVariable,
	PushParameter,
	PopParameter,
	CallFunc,

	GetVar,
	StoreVar,
	Ret,
	Type,

	DeclareFunc,
	FuncEnd,
	
	Boolliteral,
	NumberNumberliteral,

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

	Class,
	ClassEnd,

	EnumClass,
	EnumValue,
	EnumEnd,

	FileStart,
	SetFilePos,
	FileEnd,

	MAXVALUE
};


enum class InstructionSysCall : InstructionSet_t
{
	Cout_CString,
	Cout_Char,
	Cout_Buffer,
	
	Cout_ReadChar,

	File_Open,
	File_Close,
	File_Read,
};

using BitSizeType_t = UInt8;
enum class BitSizeType : BitSizeType_t
{
	Null,
	Bit8,
	Bit16,
	Bit32,
	Bit64,
	BitNative,
};
UCodeLangEnd