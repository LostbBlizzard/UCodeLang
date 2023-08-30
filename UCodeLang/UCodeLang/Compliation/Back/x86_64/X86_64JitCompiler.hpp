#pragma once
#include "../Jit/Jit.hpp"
#include "UCodeLang/RunTime/RunTimeLangState.hpp"

#include "X86_64Builder.hpp"
#include "X86_64IR.hpp"
UCodeLangStart



class X86_64JitCompiler :public JitCompiler
{
public:
	using InterpreterCPPinterface_SetRet = void(*)(void* Input);
	using InterpreterCPPinterface_GetParm = void(*)(void* Input,void* Pointer,size_t ParSize);
	using JitBuildAddress = void(*)(UAddress Input);


	enum class JitType_t : Byte
	{
		Null,
		Void,
		Int8,
		Int16,
		Int32,
		Int64,

		float32,
		float64,

		IsStruct,

		//Maps to Int64
		Pointer = (Byte)Int64,
	};

	

	X86_64JitCompiler();
	~X86_64JitCompiler();
	void Reset()  JitCompilerOverride;


	bool BuildFunc(Vector<Instruction>& Ins, UAddress funcAddress, Vector<Byte>& X64Output) JitCompilerOverride;

	void SubCall(FuncType Value, uintptr_t CPPOffset, void* X64Output) JitCompilerOverride;
	
	UAddress OnUAddressPar=0;
private:
	Vector<Byte>* Output = nullptr;
	Vector<Instruction>* _Ins =nullptr;
	

	X86_64IR _IR;
	X86_64IR::CallConventionID _CallConvention;
	X86_64IR::CallConventionID _IntrCallConvention;

	Vector<NullJitCalls> NullCalls;

	using GReg = X86_64Builder::GReg;
	struct JitType
	{
		JitType_t Type = JitType_t::Null;
		size_t StructSize = 0;

		JitType()
		{

		}
		JitType(JitType_t Type)
		{
			Set(Type);
		}

		bool IsVoid()
		{
			return Type == JitType_t::Void;
		}
		bool IsBadType()
		{
			return  Type == JitType_t::Null;
		}
		void SetAsBadType()
		{
			Type = JitType_t::Null;
		}
		bool IsInt()
		{
			return
				Type == JitType_t::Int8 ||
				Type == JitType_t::Int16 ||
				Type == JitType_t::Int32 ||
				Type == JitType_t::Int64;
		}
		bool IsFloat()
		{
			return
				Type == JitType_t::float32 ||
				Type == JitType_t::float64;
		}
		size_t GetSize() const
		{
			switch (Type)
			{
			case JitType_t::Void:return 0;
			case JitType_t::Int8:return 1;
			case JitType_t::Int16:return 2;
			case JitType_t::Int32:return 4;
			case JitType_t::Int64:return 8;
			case JitType_t::float32:return 4;
			case JitType_t::float64:return 6;
			case JitType_t::IsStruct:return StructSize;
			case JitType_t::Null:
			default:
				UCodeLangThrowException("bad path");
			}
		}

		void Set(JitType_t Type)
		{
			this->Type = Type;
			this->StructSize = 0;
		}
		void SetStruct(size_t StructSize)
		{
			this->Type = JitType_t::IsStruct;
			this->StructSize = StructSize;
		}
	};
	struct JitFuncData
	{
		JitType Ret;
		Vector<JitType> Pars;
	};
	X86_64Builder::GReg To(RegisterID id);
};
UCodeLangEnd