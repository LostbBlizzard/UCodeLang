#include "UCodeRunTime.hpp"
#include "Jit/Jit.hpp"
UCodeLangStart
UCodeRunTime::UCodeRunTime()
{
	#ifdef UCodeLangCPUCoreKnow

	#else
	const auto& RunTimeData = Get_EnvironmentData();
	_Interpreters.resize(RunTimeData.ProcessorsCount);
	#endif // DEBUG
}
UCodeRunTime::~UCodeRunTime()
{
}
void UCodeRunTime::Init(RunTimeLangState* State)
{
	_MainInterpreter.Init(State);
	for (auto& Item : _Interpreters)
	{
		Item.Init(State);
	}
}

void UCodeRunTime::UnLoad()
{
	_MainInterpreter.UnLoad();
	for (auto& Item : _Interpreters)
	{
		Item.UnLoad();
	}
}
UCodeRunTime::Return_t UCodeRunTime::ThisCall(UAddress This, const String& FunctionName)
{
	return _MainInterpreter.ThisCall(This, FunctionName);
}
UCodeRunTime::Return_t UCodeRunTime::ThisCall(UAddress This, UAddress address)
{
	return _MainInterpreter.ThisCall(This,address);
}
UCodeRunTime::Return_t UCodeRunTime::Call(const String& FunctionName)
{
	return _MainInterpreter.Call(FunctionName);
}
UCodeRunTime::Return_t UCodeRunTime::Call(UAddress address)
{
	return _MainInterpreter.Call(address);
}
UCodeLangEnd
