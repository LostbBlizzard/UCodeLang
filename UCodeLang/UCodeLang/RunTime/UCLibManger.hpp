#pragma once

#include "../LangCore.hpp"
#include "RunTimeLib.hpp"
UCodeLangStart

class UCLibManger
{
public:
	UCLibManger();
	~UCLibManger();

	UCLibManger(const UCLibManger& V) = delete;
	UCLibManger& operator=(const UCLibManger& V) = delete;

	void Link();

	inline UAddress FindAddress(const String& FunctionName)
	{
		if (_NameToAddress.count(FunctionName))
		{
			return _NameToAddress[FunctionName];
		}
		else
		{
			return NullAddress;
		}
	}
	inline RunTimeLib::CPPCallBack* FindCPPCall(const String& FunctionName)
	{
		if (_NameToCPP.count(FunctionName))
		{
			return &_NameToCPP[FunctionName];
		}
		else
		{
			return nullptr;
		}
	}
	inline Instruction& GetInst(UAddress address)
	{
		return _Instructions[(NSize_t)address];
	}
	inline const Vector<UInt8>& GetStaticBytes()
	{
		return StaticBytes;
	}
	inline const ClassAssembly& Get_Assembly()
	{
		return  Assembly;
	}
	inline void AddLib(RunTimeLib* lib){ Libs.push_back(lib); }
	inline void Clears() { Libs.clear(); }
private:
	unordered_map<String, UAddress> _NameToAddress;
	unordered_map<String, RunTimeLib::CPPCallBack> _NameToCPP;
	Vector<Instruction> _Instructions;
	Vector<RunTimeLib*> Libs;
	Vector <UInt8> StaticBytes;
	ClassAssembly Assembly;
};
UCodeLangEnd
