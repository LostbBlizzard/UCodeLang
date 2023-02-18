#include "IROptimizer.hpp"
UCodeLangStart

using OptimizationFunc = void(IROptimizer::*)();

struct OptimizationInfo
{
	OptimizationFlags AcitveIf;
	bool InverseAcitveIf;

	
	union 
	{
		OptimizationFunc FuncCall;
	};
	OptimizationInfo()
	{

	};
	OptimizationInfo(OptimizationFlags f, bool Inverse, OptimizationFunc Call)
		:AcitveIf(f), InverseAcitveIf(Inverse), FuncCall(Call)
	{

	};
};
static const Array<OptimizationInfo, IROptimizer::OptimizationCount> OptimizationList =
{

};

void IROptimizer::Reset() 
{

}
void IROptimizer::Optimized(IRBuilder& IRcode)
{
	Input = &IRcode;
}
void IROptimizer::UpdateOptimizationList()
{
	auto& Stettings = *_Settings;

	bool ForDebuging = (OptimizationFlags_t)Stettings._Flags & (OptimizationFlags_t)OptimizationFlags::Debug;
	bool ForSize =  (OptimizationFlags_t)Stettings._Flags & (OptimizationFlags_t)OptimizationFlags::ForSize;
	bool ForSpeed = (OptimizationFlags_t)Stettings._Flags & (OptimizationFlags_t)OptimizationFlags::ForSpeed;

	for (size_t i = 0; i < OptimizationCount; i++)
	{
		auto& OptimizationInfo = OptimizationList[i];
		auto& OptimizationState = OptimizationListState[i];
	

		bool IsAcitve = (OptimizationFlags_t)Stettings._Flags & (OptimizationFlags_t)OptimizationInfo.AcitveIf;
		if (OptimizationInfo.InverseAcitveIf){IsAcitve = !IsAcitve;}

		OptimizationState.IsActive = IsAcitve;
	}
}
UCodeLangEnd