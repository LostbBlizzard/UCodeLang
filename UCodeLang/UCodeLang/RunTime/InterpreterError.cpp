#include "InterpreterError.hpp"
#include "RunTimeLangState.hpp"
UCodeLangStart

void GetDetils(UAddress Input,StackFrameDetails& Out,const RunTimeLangState* State)
{
	UAddress funccall = 0;
	{
		for (int i = Input; i >= 0; i--)
		{
			auto Ins = State->GetInst(i);

			if (Ins.OpCode == InstructionSet::Return)
			{
				funccall = i + 1;
			}
		}
	}

	auto Ins = State->GetName(funccall);

	Out.FuncionName = Ins;

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