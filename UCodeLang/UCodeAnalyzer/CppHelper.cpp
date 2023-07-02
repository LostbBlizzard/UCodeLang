#include "CppHelper.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>


UCodeAnalyzerStart

String GetString(const Path& AsPath)
{
	std::ifstream File(AsPath);
	if (File.is_open())
	{
		std::string Text;
		std::string line;
		while (std::getline(File, line)) { Text += line + '\n'; }
		File.close();


		return Text;
	}
	else
	{
		return "";
	}
}

//UCodeLangExportSymbol
//UCodeLangImportSymbol

const char UCodeLangExportString[] = "UCodeLangExportSymbol";
const char UCodeLangImportString[] = "UCodeLangImportSymbol";
constexpr size_t UCodeLangImportSymbolSize = sizeof(UCodeLangExportString);


bool CppHelper::ParseCppfileAndOutULang(const Path& SrcCpp, const Path& ULangOut)
{
	auto FileText = GetString(SrcCpp);


	Vector<SymbolData> Symbols;
	size_t IndexBuffer = 0;	
	for (size_t i = 0; i < FileText.size(); i++)
	{
		auto Item = FileText[i];

		if (Item == UCodeLangExportString[IndexBuffer])
		{
			IndexBuffer++;

			if (UCodeLangImportSymbolSize - 1 == IndexBuffer)
			{
				{//pass the ("
					for (size_t iq = i; iq < FileText.size(); iq++)
					{
						auto Item = FileText[iq];

						if (Item == '\"')
						{
							i = iq + 1;
							break;
						}
					}
				}
				size_t StrStart = i;
				int a = 0;

				SymbolData Tep;

				{
					String& Out = Tep._NameSpace;
					GetStringliteral(i, FileText, Out);
				}
				

				{//pass the )
					MovePass(i, FileText, ' ');
					i++;
				}

				{//pass the spaces
					MovePassSpace(i, FileText);
				}

				auto Keywordlet = FileText[i];

				if (Keywordlet == 'e')//enum
				{
					i += 4;
					
					EnumType _type;
					{//pass the spaces
						MovePass(i, FileText, ' ');
					}

					if (FileText[i] == 'c')//class
					{
						i += 5;
					}

					{//pass the spaces
						MovePassSpace(i, FileText);
					}

					{
						GetIndentifier(i, FileText, Tep._Name);
					}

					{//pass the spaces
						MovePassSpace(i, FileText);
					}

					if (FileText[i] == ':')
					{
						i++;
						{//pass the spaces
							MovePass(i, FileText, ' ');
						}
						Type V;
						GetType(i, FileText, V);
						_type._EnumBaseType = V;
					}

					{//pass the spaces
						MovePassSpace(i, FileText);
					}
					//pass '{'
					{
						i++;
					}


					{
						String Scope;
						GetStringScope(i, FileText, Scope);

						for (size_t i = 0; i < Scope.size(); i++)
						{
							MovePassSpace(i, Scope);

							if (i < Scope.size())
							{
								EnumType::Field field;
								GetIndentifier(i, Scope, field.Name);
								MovePassSpace(i, Scope);
								if (Scope[i] == '=')
								{
									i++;
									MovePassSpace(i, Scope);
									CPPExpression V;
									GetCPPExpression(i, Scope, V);
									field.Value = V;

									MovePassSpace(i, Scope);
								}

								if (Scope[i] == ',')
								{
									i++;
								}
								_type.Fields.push_back(std::move(field));
							}
						}
					}
					int a = 0;

					Tep._Type = _type;


					Symbols.push_back(std::move(Tep));
				}

					
			}
		}
		else
		{
			IndexBuffer = 0;
		}
	}

	String R;
	CppToULangState state;
	for (auto& Item : Symbols)
	{
		R += ToString(state,Item);
	}

	return false;
}

void CppHelper::GetStringliteral(size_t& i, UCodeAnalyzer::String& FileText, UCodeAnalyzer::String& Out)
{
	for (size_t iq = i; iq < FileText.size(); iq++)
	{
		auto Item = FileText[iq];

		if (Item == '\"')
		{
			Out = FileText.substr(i, iq - i);
			i = iq + 1;
			break;
		}
	}
}

void CppHelper::GetStringScope(size_t& i, UCodeAnalyzer::String& FileText, UCodeAnalyzer::String& Out)
{
	for (size_t iq = i; iq < FileText.size(); iq++)
	{
		auto Item = FileText[iq];

		if (Item == '}')
		{
			Out = FileText.substr(i, iq - i);
			i = iq + 1;
			break;
		}
	}
}

void CppHelper::GetIndentifier(size_t& i, UCodeAnalyzer::String& FileText, UCodeAnalyzer::String& Out)
{
	for (size_t iq = i; iq < FileText.size(); iq++)
	{
		auto Item = FileText[iq];

		if (Item == ' ' || Item == '\n' || Item == ',' || Item == ':')
		{
			Out = FileText.substr(i, iq - i);
			i = iq;
			break;
		}
	}
}
void CppHelper::GetType(size_t& i, UCodeAnalyzer::String& FileText, Type& Out)
{
	GetIndentifier(i, FileText, Out.Value);
}
void CppHelper::MovePass(size_t& i, UCodeAnalyzer::String& FileText, const char& passChar)
{
	for (size_t iq = i; iq < FileText.size(); iq++)
	{
		auto Item = FileText[iq];

		if (Item != passChar)
		{
			i = iq;
			break;
		}
	}
}
void CppHelper::MovePassSpace(size_t& i, UCodeAnalyzer::String& FileText)
{
	for (size_t iq = i; iq < FileText.size(); iq++)
	{
		auto Item = FileText[iq];

		if (Item != '\n' && Item != '\r' && Item != ' ' && Item != '\t')
		{
			i = iq;
			break;
		}
	}
}

void CppHelper::GetCPPExpression(size_t& i, UCodeAnalyzer::String& FileText, CPPExpression& Out)
{
	GetIndentifier(i, FileText, Out.Value);
}

void CppHelper::GetSummaryTag(size_t& i, UCodeAnalyzer::String& FileText, SummaryTag& Out)
{
}

void CppHelper::GetSummaryTag(size_t& i, UCodeAnalyzer::String& FileText, Optional<SummaryTag>& Out)
{
}

inline String CppHelper::ToString(CppToULangState& State, const SymbolData& Syb)
{
	if (auto Item = Syb._Type.Get_If<EnumType>())
	{
		return ToString(State, *Item, Syb);
	}
	else if (auto Item = Syb._Type.Get_If<ConstexprType>())
	{
		return ToString(State, *Item, Syb);
	}
	else
	{
		throw std::exception("bad path");
	}
	return "";
}

String CppHelper::ToString(CppToULangState& State, const EnumType& Value, const SymbolData& Syb)
{
	String R;
	DoNameSpace(State, Syb, R);


	State.AddScopesUseingScopeCount(R);

	ToString(State, Syb.Summary);
	R += "$" + Syb._Name + " : ";
	if (Value._EnumBaseType.has_value())
	{
		R += ToString(State, Value._EnumBaseType.value());
	}
	else
	{
		R += "int";
	}

	if (Value.Fields.size() == 0)
	{
		R += ";";
	}
	State.ScopeCount++;
	for (auto& Item : Value.Fields)
	{
		R += '\n';
		State.AddScopesUseingScopeCount(R);

		ToString(State, Syb.Summary);
		R += Item.Name;

		if (Item.Value.has_value())
		{
			R += " = ";
			R += ToString(State, Item.Value.value());

		}

		if (&Item != &Value.Fields.back())
		{
			R += ",";
		}
	}
	
	State.ScopeCount--;
	R += '\n';
	R += '\n';

	return R;
}
String CppHelper::ToString(CppToULangState& State, const ConstexprType& Value, const SymbolData& Syb)
{
	String R;
	DoNameSpace(State, Syb, R);


	State.AddScopesUseingScopeCount(R);

	ToString(State, Syb.Summary);
	R += "eval " + ToString(State,Value._Type) + " " + Syb._Name + " = " + ToString(State, Value._Value) + '\n';
}
String CppHelper::ToString(CppToULangState& State, const Type& Value)
{
	String R;
	String CMPStr = Value.Value;

	if (CMPStr == "uint8_t" || CMPStr == "uint8" || CMPStr == "UInt8" || CMPStr == "uInt8")
	{
		R += "uint8";
	}
	else if (CMPStr == "unsigned char" || CMPStr == "Byte" || CMPStr == "byte")
	{
		R += "byte";
	}
	else if (CMPStr == "uint16_t" || CMPStr == "uint16" || CMPStr == "UInt16" || CMPStr == "uInt16"
		|| CMPStr == "unsigned short")
	{
		R += "uint16";
	}
	else if (CMPStr == "uint32_t" || CMPStr == "uint32" || CMPStr == "UInt32" || CMPStr == "uInt32"
		|| CMPStr == "unsigned int")
	{
		R += "uint32";
	}
	else if (CMPStr == "uint64_t" || CMPStr == "uint64" || CMPStr == "UInt64" || CMPStr == "uInt64"
		|| CMPStr == "unsigned long long")
	{
		R += "uint32";
	}
	else if (CMPStr == "int8_t" || CMPStr == "int8" || CMPStr == "Int8" || CMPStr == "sInt8"
		|| CMPStr == "sint8" || CMPStr == "signed char")
	{
		R += "uint8";
	}
	else if (CMPStr == "int16_t" || CMPStr == "int16" || CMPStr == "Int16" || CMPStr == "sInt16"
		|| CMPStr == "sint8" || CMPStr == "signed short" || CMPStr == "short")
	{
		R += "int16";
	}
	else if (CMPStr == "int" || CMPStr == "int32_t" || CMPStr == "int32" || CMPStr == "Int32" || CMPStr == "sInt32"
		|| CMPStr == "sint32" || CMPStr == "signed int")
	{
		R += "int32";
	}
	else if (CMPStr == "int64_t" || CMPStr == "int64" || CMPStr == "Int64" || CMPStr == "sInt64"
		|| CMPStr == "sint64" || CMPStr == "signed long long" || CMPStr == "long long")
	{
		R += "int64";
	}
	else if (CMPStr == "bool")
	{
		R += "bool";
	}
	else if (CMPStr == "char")
	{
		R += "char";
	}
	else if (CMPStr == "char8_t")
	{
		R += "uft8";
	}
	else if (CMPStr == "char16_t")
	{
		R += "uft16";
	}
	else if (CMPStr == "char32_t")
	{
		R += "uft32";
	}


	return R;
}

String CppHelper::ToString(CppToULangState& State, const CPPExpression& Value)
{
	return Value.Value;
}

void CppHelper::DoNameSpace(UCodeAnalyzer::CppHelper::CppToULangState& State, const UCodeAnalyzer::CppHelper::SymbolData& Syb, UCodeAnalyzer::String& R)
{
	if (State.LastNameSpace != Syb._NameSpace)
	{
		State.LastNameSpace = Syb._NameSpace;
		R += State.LastNameSpace;
		R += ":\n";
	}
}

String CppHelper::ToString(const CppToULangState& State, const Optional<SummaryTag>& Value)
{
	if (Value.has_value())
	{
		return ToString(State, Value.value());
	}
	return "";

}

String CppHelper::ToString(const CppToULangState& State, const SummaryTag& Value)
{
	return "";
}

bool CppHelper::ParseULangfileAndUpdateCpp(const Path& SrcLang, const Path& CppOut)
{

	return false;
}

UCodeAnalyzerEnd
