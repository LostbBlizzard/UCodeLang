#include "X86Gen.hpp"
UCodeLangStart



void X86Gen::Build(const IRBuilder* Input)
{
	_Input = Input;
	for (auto& Item : Input->Funcs)
	{
		OnFunc(Item.get());
	}
}

void X86Gen::OnFunc(const IRFunc* IR)
{
	if (IR->Blocks.size())
	{
		OnBlock(IR->Blocks.front().get());
	}
}
//en.wikibooks.org/wiki/X86_Assembly/X86_Architecture#General-Purpose_Registers_(GPR)_-_16-bit_naming_conventions
\
void X86Gen::OnBlock(const IRBlock* IR)
{
	using namespace x86;
	for (auto& Item_ : IR->Instructions)
	{
		auto& Item = *Item_;

		switch (Item.Type)
		{
		case IRInstructionType::Return:
			Push_Ins_ret();
			break;
		}
	}
}

UCodeLangEnd
