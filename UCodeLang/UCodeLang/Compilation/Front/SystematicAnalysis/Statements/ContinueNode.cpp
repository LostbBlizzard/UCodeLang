#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnContinueNode(const ContinueNode& node)
{
	if (_PassType == PassType::GetTypes)
	{

	}


	if (_PassType == PassType::BuidCode)
	{

		JumpsData v;
		v.Type = Jumps_t::Continue;
		v.JumpIns = _IR_LookingAtIRBlock->NewJump();
		_IR_Jumps.push_back(v);
	}
}
void SystematicAnalysis::IR_Build_UpdateJumpsBreakContiunes(size_t JumpIndex, size_t BoolCode, size_t BreakCode)
{
	for (size_t i = JumpIndex; i < _IR_Jumps.size(); i++)
	{
		auto& Item = _IR_Jumps[i];

		switch (Item.Type)
		{
		case Jumps_t::Break:
		{
			_IR_LookingAtIRBlock->UpdateJump(Item.JumpIns, BreakCode);
		}
		break;
		case Jumps_t::Continue:
		{
			_IR_LookingAtIRBlock->UpdateJump(Item.JumpIns, BoolCode);
		}
		break;
		default:
			UCodeLangUnreachable();
			break;
		}
	}
	IR_RemoveJumps(JumpIndex);
}

UCodeLangFrontEnd

#endif