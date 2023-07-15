#pragma once
#include "../Jit/Jit.hpp"
#include "X86_64Gen.hpp"
#include "UCodeLang/RunTime/RunTimeLangState.hpp"
UCodeLangStart



class X86_64JitCompiler
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
	void Reset();


	bool BuildFunc(Vector<Instruction>& Ins, UAddress funcAddress, Vector<UInt8>& X64Output);

	void SubCall(JitInfo::FuncType Value, uintptr_t CPPOffset, void* X64Output);
	
	UAddress OnUAddressPar=0;

	//UCodeLang::InterpreterCPPinterface::Set_Return
	InterpreterCPPinterface_SetRet  InterpreterCPPinterface_Set_ReturnPtr = nullptr;
	InterpreterCPPinterface_GetParm InterpreterCPPinterface_Get_Par = nullptr;
	JitBuildAddress BuildAddressPtr = nullptr;
	const ClassMethod* Func =nullptr;
	RunTimeLangState* State = nullptr;


	struct UnLoadedFuncPlaceHolder
	{
		size_t Offset = 0;
	};

	struct FuncToLink
	{
		size_t CPPOffset = 0;
		UAddress OnUAddress = 0;
	};
	Vector<FuncToLink> LinkingData;

	BinaryVectorMap<UAddress,UnLoadedFuncPlaceHolder> FuncsPlaceHolder;
	size_t Out_NativeCallOffset =0;
	size_t BufferOffset = 0;
private:
	Vector<UInt8>* Output = nullptr;
	Vector<Instruction>* _Ins =nullptr;
	
	X86_64Gen _Gen;
	Vector<NullJitCalls> NullCalls;

	using GReg = X86_64Gen::GReg;
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
				throw std::exception("bad path");
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
	Optional<JitFuncData> As(const ClassMethod* Method, bool PointerSizeIs32Bit);

	struct Nothing{};
	struct RegData
	{
		Variant<Nothing,AnyInt64, X86_64Gen::GReg> Contains= Nothing();
	};
	struct NativeRegData
	{

	};
	Array<RegData, (size_t)RegisterID::EndParameterRegister> Regs;
	Array<NativeRegData,(size_t)GReg::Count> NativeReg;
	void SynchronizeNativeRegs();
	NativeRegData& GetNativeReg(GReg Value)
	{
		SynchronizeNativeRegs();
		return NativeReg[(size_t)Value] ;
	}

	RegData& GetRegData(RegisterID ID)
	{
		return Regs[(size_t)ID];
	}
	inline size_t GetIndex() 
	{
		return _Gen.GetIndex() + BufferOffset;
	}

	void mov(X86_64Gen::GReg R, X86_64Gen::Value8 Value);
	void mov(X86_64Gen::GReg R, X86_64Gen::Value16 Value);
	void mov(X86_64Gen::GReg R, X86_64Gen::Value32 Value);
	void mov(X86_64Gen::GReg R, X86_64Gen::Value64 Value);
	
	void MoveRegToNative(const RegData& Reg, const JitType& TypeInReg, X86_64Gen::GReg NativeReg);

	void PassNativePars(const Vector<JitType>& Pars);
	void PopPassNativePars(const Vector<JitType>& Pars);
	void PushFuncEnd();
	void PushFuncStart();

	void PushAllParsOnStack(const Vector<JitType>& Pars);
	void PopAllParsOnStack(const Vector<JitType>& Pars);

	void Gen_InvaildNear32Call();

	Optional<GReg> GetFreeReg();
	GReg GetRegFor(RegisterID ID);
	GReg GetFreeRegOrMovToGetFree();
	JitType AsJitType(const ReflectionTypeInfo& V, const ClassAssembly& assembly, bool PointerSizeIs32Bit);

	void NewFunction2(GReg* Val1, AnyInt64* Val2);
	GReg GetAsNative(RegisterID ID, IntSizes Size);
};
UCodeLangEnd