#include "UCodeRunTime.hpp"
#include "Jit/Jit.hpp"
UCodeLangStart
UCodeRunTime::UCodeRunTime()
{
	const auto& RunTimeData = Get_EnvironmentData();
	_Interpreters.resize(RunTimeData.ProcessorsCount);
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
UCodeRunTime::Return_t UCodeRunTime::ThisCall(UAddress This, const String& FunctionName, parameters Pars)
{
	return _MainInterpreter.ThisCall(This, FunctionName, Pars);
}
UCodeRunTime::Return_t UCodeRunTime::ThisCall(UAddress This, UAddress address, parameters Pars)
{
	return _MainInterpreter.ThisCall(This,address, Pars);
}
UCodeRunTime::Return_t UCodeRunTime::Call(const String& FunctionName, parameters Pars)
{
	return _MainInterpreter.Call(FunctionName, Pars);
}
UCodeRunTime::Return_t UCodeRunTime::Call(UAddress address, parameters Pars)
{
	return _MainInterpreter.Call(address,Pars);
}
UCodeLangEnd
