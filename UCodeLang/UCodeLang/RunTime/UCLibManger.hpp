#pragma once

#include "../LangCore.hpp"
#include "RunTimeLib.hpp"
#include "Jit/Jit.hpp"
UCodeLangStart

class RunTimeLangState;
class UCLibManger
{
public:
	friend RunTimeLangState;//for hot reloading
	UCLibManger();
	~UCLibManger();

	UCLibManger(const UCLibManger& V) = delete;
	UCLibManger& operator=(const UCLibManger& V) = delete;

	void Link();

	void LinkLib(RunTimeLib* Item);

	Optional < UAddress> FindAddress(const String& FunctionName) const;
	Optional<const RunTimeLib::CPPCall*> FindCPPCall(const String& FunctionName) const;
	inline const Instruction& GetInst(UAddress address) const
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
	void ClearRunTimeState();

	inline const auto& Get_Code() const
	{
		return _Code;
	}
	inline const AsmBuffer& Get_ExCode() const
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

	String GetName(UAddress address) const;

	inline UnorderedMap<String, RunTimeLib::CPPCall>& Get_CPPCalls()
	{
		return _NameToCPP;
	}
	inline const UnorderedMap<String, RunTimeLib::CPPCall>& Get_CPPCalls() const
	{
		return _NameToCPP;
	}	
	
	inline const ULangDebugInfo& Get_DebugInfo() const
	{
		return _DebugInfo;
	}
	inline const auto& Get_NameToAddress() const
	{
		return _NameToAddress;
	}
	inline ULangDebugInfo& Get_DebugInfo()
	{
		return _DebugInfo;
	}
private:
	void LinkLib(RunTimeLib* Item,bool HotReloadKeepStatic);
	void HotReloadClearState();

	UnorderedMap<String, UAddress> _NameToAddress;
	UnorderedMap<String, RunTimeLib::CPPCall> _NameToCPP;
	Vector<Instruction> _Instructions;

	Vector<Byte> _Code;
	AsmBuffer _ExCode;
	
	Vector<Byte> StaticBytes;
	Vector<Byte> ThreadBytes;
	Vector<RunTimeLib*> Libs;
	ClassAssembly Assembly;
	ULangDebugInfo _DebugInfo;
};
UCodeLangEnd
