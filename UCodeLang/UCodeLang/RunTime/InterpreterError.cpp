#include "InterpreterError.hpp"
#include "RunTimeLangState.hpp"
UCodeLangStart

void GetDetils(UAddress Input,StackFrameDetails& Out,const RunTimeLangState* State,GetDetilsCach& cach)
{
	auto& DebugInfo = State->Get_Libs().Get_DebugInfo();

	UAddress funccall = 0;
	
	if (!cach.cach.has_value()) 
	{
		cach.cach = DebugInfo.MakeCach();
	}

	{
		for (int i = Input; i >= 0; i--)
		{
			auto Ins = State->GetInst(i);

			auto infosop = DebugInfo.GetForIns(i,cach.cach.value());
			if (infosop.has_value())
			{
				auto infos = *infosop.value();

				for (auto& Item : infos)
				{
					if (auto val = Item->Debug.Get_If<UDebugSetFile>())
					{
						if (!Out.FilePath.has_value()) 
						{
							auto& str = val->FileName;

							if (str.size() && str.front() == '[')
							{
								UCodeLangToDo();
							}
							else 
							{
								Out.FilePath = val->FileName;
							}
						}
					}
					else if (auto val = Item->Debug.Get_If<UDebugSetLineNumber>())
					{
						if (!Out.CallerLineNumber.has_value()) 
						{
							Out.CallerLineNumber = val->LineNumber;
						}
					}
			
				}
			}
			if (Ins.OpCode == InstructionSet::Return)
			{
				funccall = i + 1;
				break;
			}
		}
	}

	auto Ins = State->GetName(funccall);

	Out.FuncionName = Ins;

}
void GetDetils(const StackFrames& Input,const RunTimeLangState* State, Vector<StackFrameDetails>& Out,GetDetilsCach& cach)
{
	Out.clear();
	Out.resize(Input.StackOfCallers.size());
	
	for (size_t i = 0; i < Input.StackOfCallers.size(); i++)
	{
		GetDetils(Input.StackOfCallers[i],Out[i], State,cach);
	}
}

Vector<StackFrameDetails> GetDetils(const StackFrames& Input,const RunTimeLangState* State,GetDetilsCach& cach)
{
	Vector<StackFrameDetails> r;

	GetDetils(Input,State,r,cach);

	return r;
}

UCodeLangEnd
