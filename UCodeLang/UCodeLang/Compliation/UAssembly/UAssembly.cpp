#include "UAssembly.hpp"
#define StackName "Stack"
#define StackName_ "[" + StackName + "]"

UAssemblyStart

void UAssembly::Assemble(const String_view& Text, UCodeLang::UClib* Out,CompliationErrors* output)
{
	Lexer Lex; Lex.Set_ErrorsOutput(output);
	Lex.Lex(Text);
	Parser Parse;  Parse.Set_ErrorsOutput(output);
	Parse.Parse(Lex.Get_Output(), Out);
}
String UAssembly::ToString(const UCodeLang::UClib* Lib)
{
    String r;
	unordered_map<UAddress, String> AddressToName;
	for (const auto& Item2 : Lib->Get_NameToPtr())
	{
		AddressToName[Item2.second] = Item2.first;
	}

	r += "[Instructions]-- \n";
	auto& Insts = Lib->Get_Instructions();
	for (size_t i = 0; i < Insts.size(); i++)
	{
		auto& Item = Insts[i];
		UAddress address = (UAddress)i;
		if (AddressToName.count(address))
		{
			String Name = AddressToName[address];
			r += "---" + Name + ": \n";
		}
		

		r += "   " + std::to_string(i) + " :";
	
		
		r += '\n';
	}

    return r;
}
UAssemblyEnd
