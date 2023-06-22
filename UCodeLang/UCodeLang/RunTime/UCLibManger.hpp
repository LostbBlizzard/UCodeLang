#pragma once

#include "../LangCore.hpp"
#include "RunTimeLib.hpp"
#include "Jit/Jit.hpp"
UCodeLangStart

class UCLibManger
{
public:
	UCLibManger();
	~UCLibManger();

	UCLibManger(const UCLibManger& V) = delete;
	UCLibManger& operator=(const UCLibManger& V) = delete;

	void Link();

	void LinkLib(RunTimeLib* Item);

	Optional < UAddress> FindAddress(const String& FunctionName);
	Optional<RunTimeLib::CPPCall*> FindCPPCall(const String& FunctionName);
	inline Instruction& GetInst(UAddress address)
	{
		return _Instructions[(NSize_t)address];
	}
	inline const Vector<UInt8>& GetStaticBytes()
	{
		return StaticBytes;
	}
	inline const Vector<UInt8>& GetThreadBytes()
	{
		return ThreadBytes;
	}
	inline const ClassAssembly& Get_Assembly()
	{
		return  Assembly;
	}
	inline auto& GetInstructions()
	{
		return _Instructions;
	}
	inline void AddLib(RunTimeLib* lib){ Libs.push_back(lib); }
	inline void ClearLibs() { Libs.clear(); }
	void ClearRunTimeState()
	{
		_NameToAddress.clear();
		_NameToCPP.clear();
		_Instructions.clear();
		Libs.clear();
		StaticBytes.clear();
		ThreadBytes.clear();
	}

	inline const auto& Get_Code()
	{
		return _Code;
	}
	inline const AsmBuffer& Get_ExCode()
	{
		return _ExCode;
	}

	using CallAble =RunTimeLib::CPPCallBack;
	inline CallAble Get_ExFunc(size_t Offset)
	{
		uintptr_t Ex = (uintptr_t)_ExCode.Data;
		Ex += Offset;
		return CallAble(Ex);
	}

	void InitExCode();
private:
	VectorMap<String, UAddress> _NameToAddress;
	VectorMap<String, RunTimeLib::CPPCall> _NameToCPP;
	Vector<Instruction> _Instructions;

	Vector<Byte> _Code;
	AsmBuffer _ExCode;
	
	Vector<Byte> StaticBytes;
	Vector<Byte> ThreadBytes;
	Vector<RunTimeLib*> Libs;
	ClassAssembly Assembly;
};
UCodeLangEnd
