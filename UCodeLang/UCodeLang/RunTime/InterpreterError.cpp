#include "InterpreterError.hpp"
#include "RunTimeLangState.hpp"
UCodeLangStart

void GetDetils(UAddress Input,StackFrameDetails& Out,const RunTimeLangState* State)
{
	UAddress funccall = 0;
	{
		for (int i = funccall - 1; i >= 0; i--)
		{
			auto Ins = State->GetInst(i);

			if (Ins.OpCode == InstructionSet::Callv1)
			{
				funccall = i;
			}
		}
	}

	auto Ins = State->GetName(funccall);


	int a = 0;

}
void GetDetils(const StackFrames& Input,const RunTimeLangState* State, Vector<StackFrameDetails>& Out)
{
	Out.clear();
	Out.resize(Input.StackOfCallers.size());
	
	for (size_t i = 0; i < Input.StackOfCallers.size(); i++)
	{
		GetDetils(Input.StackOfCallers[i],Out[i], State);
	}
}

Vector<StackFrameDetails> GetDetils(const StackFrames& Input,const RunTimeLangState* State)
{
	Vector<StackFrameDetails> r;

	GetDetils(Input,State,r);

	return r;
}

UCodeLangEnd