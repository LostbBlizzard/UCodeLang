#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnBreakNode(const BreakNode& node)
{
	if (_PassType == PassType::GetTypes)
	{

	}

	if (_PassType == PassType::BuidCode)
	{
		JumpsData v;
		v.Type = Jumps_t::Break;
		v.JumpIns = _IR_LookingAtIRBlock->NewJump();
		_IR_Jumps.push_back(v);
	}
}

UCodeLangFrontEnd