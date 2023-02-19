#include "UCodeBackEnd.hpp"
#include "UCodeLang/Compliation/Helpers/InstructionBuilder.hpp"
UCodeLangStart

#define GenIns(x) ResetIns(); x;
#define GenInsPush(x) ResetIns(); x;	ULib.Add_Instruction(_Ins);



UCodeBackEndObject::UCodeBackEndObject()
{
	
}

UCodeBackEndObject::~UCodeBackEndObject()
{
}


void UCodeBackEndObject::Reset()
{
	_Input = nullptr;
	_Output = nullptr;
}

void UCodeBackEndObject::Build(const IRBuilder* Input)
{
	_Input = Input;
	_Output = &Get_Output();
	for (auto& Item : Input->Funcs)
	{
		OnFunc(Item.get());
	}
}
void UCodeBackEndObject::OnFunc(const IRFunc* IR)
{

	//build code
	UAddress FuncStart = _Output->GetLastInstruction()+1;

	if (IR->Blocks.size()) 
	{
		OnBlock(IR->Blocks.front().get());
	}

	_Output->Add_NameToInstruction(FuncStart, _Input->FromID(IR->identifier));
}
void UCodeBackEndObject::OnBlock(const IRBlock* IR)
{
	OnBlockBuildCode(IR);
	
}
void UCodeBackEndObject::OnBlockBuildCode(const IRBlock* IR)
{
	for (auto& Item_ : IR->Instructions)
	{
		auto& Item = *Item_;

		switch (Item.Type)
		{
		case IRInstructionType::Return:
			InstructionBuilder::Return(ExitState::Success, Ins); PushIns();
			break;
		default:
			break;
		}
	}
}
UCodeLangEnd

