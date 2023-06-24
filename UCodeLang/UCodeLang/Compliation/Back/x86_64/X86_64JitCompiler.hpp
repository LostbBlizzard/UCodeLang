#pragma once
#include "../Jit/Jit.hpp"
#include "X86_64Gen.hpp"
#include "UCodeLang/RunTime/RunTimeLangState.hpp"
UCodeLangStart



class X86_64JitCompiler
{
public:
	using InterpreterCPPinterface_SetRet = void(*)(void* Input);
	using JitBuildAddress = void(*)(UAddress Input);
	X86_64JitCompiler();
	~X86_64JitCompiler();
	void Reset();


	bool BuildFunc(Vector<Instruction>& Ins, UAddress funcAddress, Vector<UInt8>& X64Output);
	void SubCall(JitInfo::FuncType Value, uintptr_t CPPOffset, void* X64Output);
	
	UAddress OnUAddressPar=0;

	//UCodeLang::InterpreterCPPinterface::Set_Return
	InterpreterCPPinterface_SetRet  InterpreterCPPinterface_Set_ReturnPtr = nullptr;
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
private:
	Vector<UInt8>* Output = nullptr;
	Vector<Instruction>* _Ins =nullptr;
	
	X86_64Gen _Gen;
	Vector<NullJitCalls> NullCalls;
	struct RegData
	{


		Variant<AnyInt64> Contains;
	};
	Array<RegData, (size_t)RegisterID::EndParameterRegister> Regs;

	RegData& GetRegData(RegisterID ID)
	{
		return Regs[(size_t)ID];
	}

	void mov(X86_64Gen::GReg R, X86_64Gen::Value8 Value);
	void mov(X86_64Gen::GReg R, X86_64Gen::Value16 Value);
	void mov(X86_64Gen::GReg R, X86_64Gen::Value32 Value);
	void mov(X86_64Gen::GReg R, X86_64Gen::Value64 Value);
};
UCodeLangEnd