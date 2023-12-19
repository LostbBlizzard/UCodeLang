#pragma once

#ifndef UCodeLangNoCompiler
#include "../LangCore/UCodeLangNameSpace.hpp"
#include "../LangCore/LangTypes.hpp"
#include "../LangCore/StringHelper.hpp"
UCodeLangStart
enum class OutPutType : UInt8
{
	Lib,
	DLL,
	IRAndSymbols,
};
typedef UInt8 OptimizationFlags_t;
enum class OptimizationFlags : OptimizationFlags_t
{
	O_None = 0,
	O_1 = 1 << 0,
	O_2 = 1 << 1,
	Debug = 1 << 2,

	O_3 = O_2 | O_1,




	//PreSets
	NoOptimization = O_None,
	ForSize = O_1,
	ForSpeed = O_2,
	ForMaxSpeed = O_3,
	ForDebuging = O_1 | Debug,
};


struct CompilationBackEndInfo
{
	enum class BackEnd :Byte
	{
		UCodeVm,
		C11,
		Wasm,
		
		Windows,
		Linux,
		MacOs,

		Other,
	};
	enum class InsSet :Byte
	{
		X86,
		Arm,
		Other,
	};

	BackEnd Output = BackEnd::Other;
	InsSet OutputSet = InsSet::Other;



	bool IsUCodeVm() { return Output == BackEnd::UCodeVm; }
	bool IsC11() { return Output == BackEnd::C11; }
	bool IsWasm() { return Output == BackEnd::Wasm; }

	bool IsWindows() { return Output == BackEnd::Windows; }
	bool IsLinux() { return Output == BackEnd::Linux; }
	bool IsMacOs() { return Output == BackEnd::MacOs; }

	bool IsX86(){ return OutputSet == InsSet::X86; }
	bool IsArm() { return OutputSet == InsSet::Arm; }
};
struct CompilationSettings
{
	OutPutType _Type = OutPutType::Lib;
	OptimizationFlags _Flags = OptimizationFlags::ForDebuging;
	IntSizes PtrSize = IntSizes::Native;
	CompilationBackEndInfo _BackEndInfo;
	Vector<String> _Args;

	

	void AddArgFlag(const String& FlagName)
	{
		_Args.push_back("--" + FlagName);
	}
	void AddArgValue(const String& ValueName,const String& ValueAsString)
	{
		_Args.push_back("--" + ValueName + ":" + ValueAsString);
	}
	bool HasArg(const String& Arg) const
	{
		for (auto& Item : _Args)
		{
			if (Item == Arg)
			{
				return true;
			}
		}

		return false;
	}
	bool HasFlagArg(const String& Arg) const
	{
		return HasArg("--" + Arg);
	}
	Optional<String> GetArgValue(const String& ValueName)const
	{
		Optional<String> R;
		for (auto& Item : _Args)
		{
			if (StringHelper::StartWith(Item,ValueName))
			{
				R = Item.substr(ValueName.size() + 1);//pass the :
			}
		}

		return R;
	}
	Optional<String> GetArgValueFlag(const String& ValueName)const
	{
		return GetArgValue("--" + ValueName);
	}
};
UCodeLangEnd
#endif