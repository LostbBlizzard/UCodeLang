#include "UCLibManger.hpp"
#include "UCodeLang/Compliation/Helpers/NameDecoratior.hpp"
UCodeLangStart
UCLibManger::UCLibManger()
{
}

UCLibManger::~UCLibManger()
{
}

void UCLibManger::Link()
{
	for (const auto Item : Libs)
	{
		LinkLib(Item);
	}
	InitExCode();
}

void UCLibManger::LinkLib(UCodeLang::RunTimeLib* Item)
{
	auto& Ins_s = Item->Get_Instructions();
	size_t oldSize = _Instructions.size();
	_Instructions.reserve(oldSize + Ins_s.size());

	size_t oldSize2 = _Code.size();
	_Code.reserve(oldSize2 + _Code.size());

	for (const auto& Item2 : Ins_s)
	{
		Instruction Tep = Item2;

		if (Tep.OpCode == InstructionSet::LoadFuncPtr)
		{
			Tep.Op_RegUInt16.B += oldSize;
		}
		else if (Tep.OpCode == InstructionSet::Call)
		{
			Tep.Op_ValUInt16.A += oldSize;
		}
		else if (Tep.OpCode == InstructionSet::Call_Code)
		{
			Tep.Op_ValUInt16.A += oldSize;
		}

		_Instructions.push_back(Tep);
	}

	if (Item->Get_Lib())
	{
		auto Layer = Item->Get_Lib()->GetLayer(UCode_CodeLayer_UCodeVM_Name);
		if (Layer) 
		{
			auto& InsLayer = Layer->_Data.Get<CodeLayer::UCodeByteCode>();
			for (const auto& Item : Item->Get_Lib()->Get_StaticBytes())
			{
				StaticBytes.push_back(Item);
			}
			for (const auto& Item : Item->Get_Lib()->Get_ThreadBytes())
			{
				ThreadBytes.push_back(Item);
			}

			

			for (const auto& Item2 : InsLayer.Get_NameToPtr())
			{
				_NameToAddress[Item2._Key] = Item2._Value;
			}
			auto& _Assembly = Item->Get_Lib()->Get_Assembly();
			ClassAssembly::PushCopyClasses(_Assembly, Assembly);

			if (InsLayer.DebugInfo.has_value())
			{
				auto& Debug = InsLayer.DebugInfo.value();

				for (auto& Item : Debug.VarablesInfo)
				{
					//TODO Add offset based on this lib StaticBytes and ThreadBytes
					_DebugInfo.VarablesInfo.AddValue(Item._Key, Item._Value);
				}
				for (auto& Item : Debug.DebugInfo)
				{
					//TODO Add offset based on this lib StaticBytes and ThreadBytes
					_DebugInfo.DebugInfo.push_back(Item);
				}
			}
		}
	}
	for (const auto& Item2 : Item->Get_CPPCalls())
	{
		_NameToCPP[Item2._Key] = Item2._Value;
	}
}

Optional<UAddress> UCLibManger::FindAddress(const String& FunctionName) const
{
	if (_NameToAddress.count(FunctionName))
	{
		return _NameToAddress.at(FunctionName);
	}
	else
	{
		auto tep = NameDecoratior::GetUnDecoratedName(FunctionName);
		if (_NameToAddress.count(tep))
		{
			return _NameToAddress.at(tep);
		}

		return {};
	}
}

Optional<const RunTimeLib::CPPCall*> UCLibManger::FindCPPCall(const String& FunctionName) const
{
	if (_NameToCPP.count(FunctionName))
	{
		return &_NameToCPP.at(FunctionName);
	}
	else
	{
		auto tep = NameDecoratior::GetUnDecoratedName(FunctionName);
		if (_NameToCPP.count(tep))
		{
			return &_NameToCPP.at(tep);
		}


		return  {};
	}
}

void UCLibManger::InitExCode()
{
	_ExCode.Alloc(_Code.data(), _Code.size());
	_ExCode.SetToExecuteMode();
}

String UCLibManger::GetName(UAddress address) const
{
	for (auto& item : _NameToAddress)
	{
		if (item._Value == address)
		{
			return item._Key;
		}
	}
	return "";
}
void UCLibManger::ClearRunTimeState()
{
	_NameToAddress.clear();
	_NameToCPP.clear();
	_Instructions.clear();

	_Code.clear();
	_ExCode = AsmBuffer();

	Libs.clear();
	StaticBytes.clear();
	ThreadBytes.clear();
	Assembly.Classes.clear();

	_DebugInfo.DebugInfo.clear();
	_DebugInfo.VarablesInfo.clear();
}


UCodeLangEnd

