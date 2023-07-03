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
	
	inline size_t GetIndex() { return _Output.GetIndex(); }
	inline Byte* GetData(size_t offset) { return _Output.ByteOutput.data() + offset; }
	inline Byte* GetData() { return GetData(0); }
	inline size_t Size() const { return _Output.ByteOutput.size(); }
	//move
	inline void mov(GReg Reg, Value8 Value)
	{
		Use86
		PushByte(0xb0 + RegisterOffset(Reg));
		PushByte(Value);
	}
	inline void mov(GReg Reg, Value16 Value)
	{
		Use86
		PushByte(0x66);
		PushByte(0xb9 + RegisterOffset(Reg));
		PushValue_t_little_endian(Value);
	}
	inline void mov(GReg Reg, Value32 Value)
	{
		Use86
			PushByte(0xb8 + RegisterOffset(Reg));
		PushValue_t_little_endian(Value);
	}

	inline void mov8(GReg dest, GReg src)
	{
		Use86
		PushByte(0x88);
		PushByte(modrm(src,dest));
	}
	inline void mov16(GReg dest, GReg src)
	{
		Use86
		PushByte(0x66);
		PushByte(0x89);
		PushByte(modrm(src, dest));
	}
	inline void mov32(GReg dest, GReg src)
	{
		Use86
		PushByte(0x89);
		PushByte(modrm(src, dest));
	}

	//add

	//dest := src + dest;
	inline void add8(GReg dest, GReg src)
	{
		Use86
		PushByte(0x00);
		PushByte(modrm(src, dest));
	}

	//dest := src + dest;
	inline void add16(GReg dest, GReg src)
	{
		Use86
		PushByte(0x66);
		PushByte(0x01);
		PushByte(modrm(src, dest));
	}

	//dest := src + dest;
	inline void add32(GReg dest, GReg src)
	{
		Use86
		PushByte(0x01);
		PushByte(modrm(src, dest));
	}
	

	//control flow
	inline void syscall()
	{
		Use86
		PushByte(0xf);
		PushByte(0x05);
	}
	inline void ret()
	{
		Use86
		PushByte(0xc3);
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