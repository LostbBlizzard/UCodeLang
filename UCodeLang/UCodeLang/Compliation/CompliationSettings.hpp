#pragma once
#include "../LangCore/UCodeLangNameSpace.hpp"
#include "../LangCore/LangTypes.hpp"
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
	//use these til the top are stable
	Stable_ForSize = O_None,
	Stable_ForSpeed = O_None,
	Stable_MaxSpeed = O_None,
	Stable_ForDebuging = O_None | Debug,
};
struct CompliationSettings
{
	OutPutType _Type = OutPutType::Lib;
	OptimizationFlags _Flags = OptimizationFlags::ForDebuging;
	IntSizes PtrSize = IntSizes::Native;
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
			if (Item._Starts_with(ValueName))
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