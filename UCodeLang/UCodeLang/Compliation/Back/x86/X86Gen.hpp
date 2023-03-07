#pragma once
#include "../BackEndInterface.hpp"
#include "x86Data.hpp"
#include "../InsCodeGen.h"
UCodeLangStart

//resources
//https://en.wikipedia.org/wiki/X86_instruction_listings
//https://www.felixcloutier.com/x86/

class X86Gen
{
#define Use86 using namespace x86;
#define X86Gen_NotAdded throw std::exception("not added");
public:
	enum class GeneralRegisterDataState
	{
		notUsed,
		HasIRValue,
	};
	struct GeneralRegisterData
	{
		GeneralRegisterDataState State = GeneralRegisterDataState::notUsed;
		IRInstruction* HasValue = nullptr;
	};
	using Value8 = CodeGen::Value8;
	using Value16 = CodeGen::Value16;
	using Value32 = CodeGen::Value32;
	using Value64 = CodeGen::Value64;

	using GReg = x86::GeneralRegisters;
	//
	X86Gen() {}
	~X86Gen() {}
	//members
	CodeGen _Output;

	void Reset()
	{

	}

	//
	void PushByte(Byte Value)
	{
		_Output.PushByte(Value);
	}
	void PushByte(const Byte* Value, size_t Size)
	{
		_Output.PushByte(Value, Size);
	}
	template<typename T>void PushValue_t(const T& Value)
	{
		PushByte((const Byte*)&Value, sizeof(Value));
	}
	template<typename T>void PushValue_t_little_endian(const T& Value)
	{
		_Output.PushValue_t_little_endian(Value);
	}
	template<typename T>void PushValue_t_Big_endian(const T& Value)
	{
		_Output.PushValue_t_Big_endian(Value);
	}

	//https://www.felixcloutier.com/x86/index.html
	//x86 instructions
	

	//move
	void Push_Ins_MovImm8(GReg Reg, Value8 Value)
	{
		Use86
		PushByte(0xb0 + RegisterOffset(Reg));
		PushByte(Value);
	}
	void Push_Ins_MovImm16(GReg Reg, Value16 Value)
	{
		Use86
		PushByte(0x66);
		PushByte(0xb9 + RegisterOffset(Reg));
		PushValue_t_little_endian(Value);
	}
	void Push_Ins_MovImm32(GReg Reg, Value32 Value)
	{
		Use86
			PushByte(0xb8 + RegisterOffset(Reg));
		PushValue_t_little_endian(Value);
	}

	void Push_Ins_RegToReg8(GReg Reg, GReg OutReg)
	{
		Use86
		PushByte(0x88);
		PushByte(modrm(Reg, OutReg));
	}
	void Push_Ins_RegToReg16(GReg Reg, GReg OutReg)
	{
		Use86
		PushByte(0x66);
		PushByte(0x89);
		PushByte(modrm(Reg, OutReg));
	}
	void Push_Ins_RegToReg32(GReg Reg, GReg OutReg)
	{
		Use86
		PushByte(0x89);
		PushByte(modrm(Reg, OutReg));
	}

	//add
	void Push_Ins_Add8(GReg Reg, GReg OutReg)
	{
		Use86
		PushByte(0x00);
		PushByte(modrm(Reg, OutReg));
	}
	void Push_Ins_Add16(GReg Reg, GReg OutReg)
	{
		Use86
		PushByte(0x66);
		PushByte(0x01);
		PushByte(modrm(Reg, OutReg));
	}
	void Push_Ins_Add32(GReg Reg, GReg OutReg)
	{
		Use86
		X86Gen_NotAdded
	}
	//control flow
	void Push_Ins_syscall()
	{
		Use86
		PushByte(0xf);
		PushByte(0x05);
	}
	void Push_Ins_ret()
	{
		Use86
		PushByte(0xc3);
	}
	void Push_Ins_shortJump()
	{

	}
};

class IRToX86
{
public:
	using GReg = X86Gen::GReg;
	enum class GeneralRegisterDataState
	{
		notUsed,
		HasIRValue,
	};
	struct GeneralRegisterData
	{
		GeneralRegisterDataState State = GeneralRegisterDataState::notUsed;
		IRInstruction* HasValue = nullptr;
	};
	void Reset();
	void Build(const IRBuilder* Input);

	X86Gen _Output;
	CompliationSettings* _Settings = nullptr;

	struct FuncInfos
	{
		IRFunc* Func = nullptr;
		size_t location = 0;
	};
	Vector<FuncInfos> _Funcs;

private:
	void OnFunc(const IRFunc* IR);
	void OnBlock(const IRBlock* IR);

	const IRBuilder* _Input = nullptr;
	static constexpr size_t GeneralRegisters_Count = (size_t)GReg::Count;
	Array<GeneralRegisterData, GeneralRegisters_Count> GeneralRegisters;


	inline CompliationSettings& Get_Settings()
	{
		return *_Settings;
	}

	
	GReg GetFreeGeneralRegister();
	GReg FindGeneral(const IRInstruction* Ins);
	void SetRegister(GReg Reg, IRInstruction* Ins);
	GReg LoadOpINGeneralRegister(IRInstruction& Ins,const IROperator& Op);
	void LoadOpToReg(IRInstruction& Ins, const IROperator& Op, GReg Out);
	
	void RegToReg(IRTypes Type, GReg In, GReg Out);
	GReg FindOp(IRInstruction& Ins, IROperator Op);
};
UCodeLangEnd