#include "Linker.hpp"
UCodeLangStart
void Linker::Link(const UClib* ToSelf, UClib* outLib)
{
	Vector<const UClib*> libs;
	libs.resize(1);
	libs[0] = ToSelf;

	Link(libs, outLib);
}
void Linker::Link(const Vector<const UClib*>& libsToLink, UClib* outLib)
{

	outLib->ClearState();




	Combine(libsToLink, outLib);

}
void Linker::Combine(const UCodeLang::Vector<const UCodeLang::UClib*>& libsToLink, UCodeLang::UClib* outLib)
{
	UAddress Insoffset = 0;
	UAddress Staticoffset = 0;
	UAddress Debugoffset = 0;
	bool HaveNullInst = false;
	for (auto& Lib : libsToLink)
	{
		const auto& StaticBytes = Lib->Get_StaticBytes();
		const auto& Instr = Lib->Get_Instructions();
		const auto& NameToPtr = Lib->Get_NameToPtr();

		const auto& DebugBytes = Lib->Get_DebugByte();



		for (auto& Item : StaticBytes)//use resize and memcopy
		{
			outLib->AddStaticBytes(Item);
		}
		ClassAssembly::Copy(Lib->Get_Assembly(),outLib->Get_Assembly());


		Instruction Tep;
		for (auto& Item : Instr)
		{
			outLib->Add_Instruction(Item);
		}
		
		//

		auto MyNameToPtr = Lib->Get_NameToPtr();
		for (auto& Item : NameToPtr)
		{
			UCodeLang::UAddress NewAddress = Item.second + Insoffset;
			//some may be the same
			if (!outLib->Get_HasNameToPtr(Item.first))
			{
				outLib->Add_NameToInstruction(NewAddress, Item.first);
			}
			else
			{
				auto& E = _ErrorsOutput->AddError(UCodeLang::ErrorCodes::Redefinition, 0, 0);
				E._Msg = "The 'Class/Func' Named '" + Item.first + "' already exist in this scope";
			}

		}

		for (auto& Item : DebugBytes)//use resize and memcopy
		{
			outLib->AddDebugBytes(Item);
		}

		Insoffset += (UAddress)Instr.size();
		Staticoffset += (UAddress)StaticBytes.size();
		Debugoffset += (UAddress)DebugBytes.size();
		
		Tep.OpCode_AsInt = (InstructionSet_t)Intermediate_Set::FileEnd;
		Tep.Value0_AsInt64 = Debugoffset;
		outLib->Add_Instruction(Tep);
	}

	//

	for (auto& item : _Strings)
	{
		auto Pos = outLib->AddStaticBytes(item.first);
		item.second = Pos;
	}
}

void Linker::Reset()
{
}

UCodeLangEnd