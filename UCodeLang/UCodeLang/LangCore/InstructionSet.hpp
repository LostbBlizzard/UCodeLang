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
using InstructionSet_t = UInt8;

#define AddSetInt(bitsize) \
StoreFromPtrToReg##bitsize, StoreRegToPtr##bitsize, Push##bitsize, Pop##bitsize, StoreRegToReg##bitsize, StoreRegOnStack##bitsize,StoreRegOnStackSub##bitsize, GetFromStack##bitsize, \
GetFromStackSub##bitsize,\
Add##bitsize, Sub##bitsize, MultS##bitsize, MultU##bitsize, DivS##bitsize, DivU##bitsize,\
LogicalAnd##bitsize, Logicalor##bitsize, LogicalNot##bitsize,\
equalto##bitsize, notequalto##bitsize, lessthan##bitsize, greaterthan##bitsize, equal_lessthan##bitsize, equal_greaterthan##bitsize,\
bitwiseAnd##bitsize, bitwiseOr##bitsize, bitwiseLeftShift##bitsize, bitwiseRightShift##bitsize, bitwiseXor##bitsize, bitwise_Not##bitsize,\
UInt##bitsize##ToSInt##bitsize,SInt##bitsize##ToUInt##bitsize,\

#define AddSetfloat(bitsize)\


enum class InstructionSet : InstructionSet_t
{
	Exit,Return,
	
	Call,CallIf,CallReg,
	
	Jump, Jumpif,JumpReg,

	DoNothing,
	
	//Set 8 Bits
	Store8,
	Store16,
	Store32v1,
	Store32v2,
	Store64v1,
	Store64v2,
	Store64v3,
	Store64v4,

	AddSetInt(8)
	//Set 16
	AddSetInt(16)
	//Set 32
	AddSetInt(32)
	//Set 64
	AddSetInt(64)

	//float 32
	Storef32v1,
	Storef32v2,

	AddSetfloat(32)
	//float 64

	Storef64v1,
	Storef64v2,
	Storef64v3,
	Storef64v4,

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
	//PointerMember Set
	PointerMemberLoad8,
	PointerMemberRead8,

	PointerMemberLoad16,
	PointerMemberRead16,

	PointerMemberLoad32,
	PointerMemberRead32,

	PointerMemberLoad64,
	PointerMemberRead64,

	LoadFuncPtr,

	//Stack,Thread,Static
	GetPointerOfStack, GetPointerOfStackSub, 
	GetPointerOfStaticMem, GetPointerOfThreadMem,
	IncrementStackPointer, DecrementStackPointer,
	
	//C func
	Malloc,Free,ReAlloc, Calloc,
	MemCopy,Memset,Strlen,

	//Cpp func Set
	CPPCall,CppCallNamed,
	Link_Bytes,Link_Path,
	Call_Code,

	//SysCall Set
	Cout_CString,
	Cout_Char,
	Cout_Buffer,

	Cout_ReadChar,
	Cout_ReadBuffer,

	File_Open,
	File_Close,
	File_Read,
	File_Write,

	FileP_Open,

	//Debuging Set
	Debug_FuncStart,
	Debug_FuncEnd,
	Debug_LineEnter,


	//Await Set
	Await_NewTask,
	Await_PassPar,
	Await_Run,
	Await_IsDone,
	Await_GetValue,
	Await_FreeTask,

	MAXVALUE,
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