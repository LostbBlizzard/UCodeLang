#include "UCLibManger.hpp"

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
		for (const auto& Item2 : Item->Get_Instructions())
		{
			_Instructions.push_back(Item2);
		}
		if (Item->Get_Lib())
		{
			for (const auto& Item2 : Item->Get_Lib()->Get_NameToPtr())
			{
				_NameToAddress[Item2.first] = Item2.second;
			}
			for (const auto& Item : Item->Get_Lib()->Get_StaticBytes())
			{
				StaticBytes.push_back(Item);
			}

			auto& _Assembly = Item->Get_Lib()->Get_Assembly();
			ClassAssembly::PushCopyClasses(_Assembly, Assembly);
		}
		for (const auto& Item2 : Item->Get_CPPCalls())
		{
			_NameToCPP[Item2.first] = Item2.second;
		}
	}
}

UCodeLangEnd

