#include "UCodeRunTime.hpp"
#include "Jit/Jit.hpp"
UCodeLangStart
UCodeRunTime::UCodeRunTime()
{
	
}
UCodeRunTime::~UCodeRunTime()
{
}
void UCodeRunTime::Init(RunTimeLangState* State, Optional<int> OtherInterpreterCount)
{	

	
	#ifdef UCodeLangCPUCoreKnow

	#else
	if (OtherInterpreterCount.has_value())
	{
		_Interpreters.resize(OtherInterpreterCount.value());
	}
	else
	{
		const auto& RunTimeData = Get_EnvironmentData();
		_Interpreters.resize(RunTimeData.ProcessorsCount);
	}

	for (size_t i = 0; i < _Interpreters.size(); i++)
	{
		_Interpreters[i].reset(new MyInterpreter());
	}
	#endif // DEBUG


	_MainInterpreter.Init(State);
	for (auto& Item : _Interpreters)
	{
		Item->Init(State,&_State);
	}
}

void UCodeRunTime::UnLoad()
{
	_MainInterpreter.UnLoad();
	for (auto& Item : _Interpreters)
	{
		Item->UnLoad();
	}
}

void UCodeRunTime::Get_Return(void* Output, size_t OutputSize)
{
	return Get_MyInterpreter().Get_Return(Output, OutputSize);
}
UCodeRunTime::Return_t UCodeRunTime::Call(const String& FunctionName)
{
	return Get_MyInterpreter().Call(FunctionName);
}
UCodeRunTime::Return_t UCodeRunTime::Call(UAddress address)
{
	return Get_MyInterpreter().Call(address);
}
UCodeLangEnd
