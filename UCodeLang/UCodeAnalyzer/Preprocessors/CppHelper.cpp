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
//UCodeLangOutPartype(Type)
UCodeLangExclude
const char UCodeLangExportString[] = "UCodeLangExportSymbol";
const char UCodeLangExportV[] = "UCodeLangExport";
const char UCodeLangImportString[] = "UCodeLangImportSymbol";
const char UCodeLangOutPartypeString[] = "UCodeLangOutPartype";
const char UCodeLangExcludeString[] = "UCodeLangExclude";
const char UCodeLangAutoLinkString[] = "UCodeLangAutoLink";

constexpr size_t UCodeLangExportSymbolSize = sizeof(UCodeLangExportString);
constexpr size_t UCodeLangExportVSymbolSize = sizeof(UCodeLangExportV);
constexpr size_t UCodeLangOutPartypeSize = sizeof(UCodeLangOutPartypeString);
constexpr size_t UCodeLangExcludeSize = sizeof(UCodeLangExcludeString);
constexpr size_t UCodeLangAutoLinkStringSize = sizeof(UCodeLangAutoLinkString);

void WriteStringToFile(const Path& path,const String& string)
{
	std::ofstream File(path);
	if (File.is_open())
	{
		File.write((const char*)string.data(), string.size());
		File.close();
	}
}
void AddTabCount(size_t Tabs, String& Str)
{
	for (size_t i = 0; i < Tabs; i++)
	{
		Str += '\t';
	}
}

String ToCType(const CppHelper::Type& Value)
{
	return Value.Value;
}

String ToCType(const CppHelper::FuncData::Par& Value)
{
	return ToCType(Value.Type);
}

bool CppHelper::ParseCppfileAndOutULang(const Path& SrcCpp,const Path& CppLinkFile, const Path& ULangOut)
{


	Vector<SymbolData> Symbols;
	{
		auto FileText = GetString(SrcCpp);
		size_t IndexBuffer = 0;
		for (size_t i = 0; i < FileText.size(); i++)
		{
			auto Item = FileText[i];

			if (Item == UCodeLangExportString[IndexBuffer])
			{
				IndexBuffer++;

				if (UCodeLangExportSymbolSize - 1 == IndexBuffer)
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

					size_t StartIndex = i;
					String Indentifer;
					GetIndentifier(i, FileText, Indentifer);
					OnDo(StartIndex ,Indentifer, i, FileText, Tep, Symbols);
				}
			}
			else
			{
				IndexBuffer = 0;
			}
		}
	}
	String R;
	CppToULangState state;
	for (auto& Item : Symbols)
	{
		R += ToString(state,Item);
	}

	WriteStringToFile(ULangOut, R);

	{//Link for Cpp
		auto CppLinkText = GetString(CppLinkFile);
		
		{
			size_t TabCount = 0;
			size_t IndexBuffer = 0;
			for (size_t i = 0; i < CppLinkText.size(); i++)
			{
				auto Item = CppLinkText[i];
				if (Item == ' ')
				{
					TabCount++;
				}
				if (Item == '\t')
				{
					TabCount = 0;
				}
				if (Item == UCodeLangAutoLinkString[IndexBuffer])
				{
					IndexBuffer++;
					if (UCodeLangAutoLinkStringSize - 1 == IndexBuffer)
					{

						{//pass the (
							MovePass(i, CppLinkText, ' ');
							i++;
						}

						MovePass(i, CppLinkText, ' ');

						String Var = "";
						{
							GetIndentifier(i, CppLinkText, Var);
							Var = Var.substr(1);
						}

						{//pass the ,
							MovePass(i, CppLinkText, ' ');
							i++;
						}

						MovePass(i, CppLinkText, ' ');

						String CppNameSpace;
						{
							GetIndentifier(i, CppLinkText, CppNameSpace);
							CppNameSpace = CppNameSpace.substr(0, CppNameSpace.size() - 1);
						}

						{//pass the )
							MovePass(i, CppLinkText, ' ');
							i++;
						}

						{//pass the ;
							MovePass(i, CppLinkText, ' ');
							i++;
						}
						const char* WasSetHeader = "//Made by UCodeAutoLink";
						size_t WasSetHeaderSize = strlen(WasSetHeader);
						const char* EndHeader = "Made by UCodeAutoLink End";
						size_t EndHeaderSize = strlen(EndHeader);

						bool WasSet = false;
						size_t RemoveOffset = i;
						{	
							size_t IndexBuffer = 0;
							for (size_t i2 = i; i2 < CppLinkText.size(); i2++)
							{
								auto Item = CppLinkText[i2];
								if (Item == WasSetHeader[IndexBuffer])
								{
									if (WasSetHeaderSize - 1 == IndexBuffer)
									{
										WasSet = true;
										i = i2;
										break;
									}
									IndexBuffer++;
								}
								else
								{
									IndexBuffer = 0;
								}
							}
						}

						if (WasSet)
						{
							size_t Bracks = 1;//we passed the { //all ready
							for (size_t i2 = i; i2 < CppLinkText.size(); i2++)
							{
								auto Item = CppLinkText[i2];
								if (Item == '{')
								{
									Bracks++;
								}
								if (Item == '}')
								{
									Bracks--;
									if (Bracks == 0)
									{
										i = i2;
										break;
									}
								}
							}


							//remove the { ... }
							String StartText = CppLinkText.substr(0, RemoveOffset);

							{
								size_t IndexBuffer = 0;
								for (size_t i2 = i; i2 < CppLinkText.size(); i2++)
								{
									auto Item = CppLinkText[i2];
									if (Item == EndHeader[IndexBuffer])
									{
										if (EndHeaderSize - 1 == IndexBuffer)
										{
											i = i2;
											break;
										}
										IndexBuffer++;
									}
									else
									{
										IndexBuffer = 0;
									}
								}
							}
							i++;//to pass that char
							String EndText = CppLinkText.substr(i+1);//use i+2 to pass the both \n.


							//removeing the {...}
							CppLinkText = StartText + EndText;
							i = RemoveOffset;

							int a = 0;
							//CppLinkText = CppLinkText.substr
						}

						{
							String Linkstr;
							

							AddTabCount(TabCount + 1,Linkstr);
							Linkstr += '{' + (String)WasSetHeader + " \n";


							struct FuncInfo
							{
								String FuncName;
								Vector<String> Pars;
								String Ret;
							};
							Vector< FuncInfo> V;
							for (auto& Item : Symbols)
							{
								if (auto Val = Item._Type.Get_If<FuncData>())
								{
									FuncInfo Vinfo;
									Vinfo.Ret = ToCType(Val->Ret);
									Vinfo.FuncName = Item._Name;
									Vinfo.Pars.resize(Val->Pars.size());
									for (size_t i = 0; i < Val->Pars.size(); i++)
									{
										Vinfo.Pars[i] = ToCType(Val->Pars[i]);
									}
									V.push_back(std::move(Vinfo));
								}
							}
							for (auto& Item : V)
							{
								String FuncName = CppNameSpace + "::" + Item.FuncName;

								AddTabCount(TabCount + 2, Linkstr);


								Linkstr += Var;
								Linkstr += ".Add_CPPCall(\"";
								Linkstr += FuncName;
								Linkstr += "\",[](UCodeLang::InterpreterCPPinterface& Input) \n";
								AddTabCount(TabCount + 3, Linkstr);

								Linkstr += "{\n";
								AddTabCount(TabCount + 3, Linkstr);

								

								{
									size_t ParCount = 0;
									for (auto& Par : Item.Pars)
									{
										Linkstr += "\n";
										AddTabCount(TabCount + 4, Linkstr);

										Linkstr += Par + " Par" + std::to_string(ParCount) + " = ";
										Linkstr += "Input.GetParameter<" + Par + ">();";

										Linkstr += "\n";
										AddTabCount(TabCount + 4, Linkstr);

										ParCount++;
									}
								}


								Linkstr += "\n";
								AddTabCount(TabCount + 4, Linkstr);
								if (Item.Ret != "void")
								{
									Linkstr += Item.Ret + " Ret =";
								}
								Linkstr += FuncName + "(";

								{
									size_t ParCount = 0;
									for (auto& Par : Item.Pars)
									{
										Linkstr += "Par" + std::to_string(ParCount);
										if (&Par != &Item.Pars.back())
										{
											Linkstr += ",";
										}
										ParCount++;
									}
								}
								
								Linkstr += ");";

								Linkstr += "\n";
								AddTabCount(TabCount + 4, Linkstr);
								Linkstr += "\n";
								AddTabCount(TabCount + 4, Linkstr);


								if (Item.Ret != "void")
								{
									Linkstr += "Input.Set_Return<" + Item.Ret + ">(Ret);";
								}
								else
								{
									Linkstr += "Input.Set_Return();";
								}

								Linkstr += "\n";
								AddTabCount(TabCount + 3, Linkstr);
								Linkstr += "\n";
								AddTabCount(TabCount + 3, Linkstr);



								Linkstr += "},";
								Linkstr += FuncName;
								Linkstr += "); \n";
							}

							AddTabCount(TabCount + 1, Linkstr);
							Linkstr += "}//"  + (String)EndHeader + '\n';
							CppLinkText.insert(i, Linkstr);
						}

						break;
					}
				}
				else
				{
					IndexBuffer = 0;
				}

			}
		
		
		}
		WriteStringToFile(CppLinkFile, CppLinkText);
	}
	


	return false;
}

void CppHelper::DoConstexprType(size_t& i, UCodeAnalyzer::String& FileText, UCodeAnalyzer::CppHelper::SymbolData& Tep, UCodeAnalyzer::Vector<UCodeAnalyzer::CppHelper::SymbolData>& Symbols)
{
	i += 9;

	{//pass the spaces
		MovePassSpace(i, FileText);
	}
	{
		size_t OldIndex = i;
		String StrV;
		GetIndentifier(i, FileText, StrV);

		if (StrV == "static")
		{
			MovePassSpace(i, FileText);
		}
		else
		{
			i = OldIndex;
		}
	}
	ConstexprType _type;
	GetType(i, FileText, _type._Type);
	{//pass the spaces
		MovePassSpace(i, FileText);
	}
	GetIndentifier(i, FileText, Tep._Name);
	{//pass the spaces
		MovePassSpace(i, FileText);
	}
	i++;//pass =
	{//pass the spaces
		MovePassSpace(i, FileText);
	}
	GetCPPExpression(i, FileText, _type._Value);


	Tep._Type = std::move(_type);
	Symbols.push_back(std::move(Tep));
}

void CppHelper::DoEnumType(size_t& i, UCodeAnalyzer::String& FileText, UCodeAnalyzer::CppHelper::SymbolData& Tep, UCodeAnalyzer::Vector<UCodeAnalyzer::CppHelper::SymbolData>& Symbols)
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

	Tep._Type = std::move(_type);


	Symbols.push_back(std::move(Tep));
}

void CppHelper::DoClassOrStruct(const String& Keywordlet, size_t& i, UCodeAnalyzer::String& FileText, UCodeAnalyzer::CppHelper::SymbolData& Tep, UCodeAnalyzer::Vector<UCodeAnalyzer::CppHelper::SymbolData>& Symbols)
{
	if (Keywordlet == "class")
	{
		i += 5;
	}
	else
	{
		i += 6;
	}
	ClassType _type;

	{//pass the spaces
		MovePassSpace(i, FileText);
	}

	{
		GetIndentifier(i, FileText, Tep._Name);
	}

	{//pass the spaces
		MovePassSpace(i, FileText);
	}
	//pass '{'
	{
		i++;
	}

	{
		//i--;

		String Scope;
		GetStringScope(i, FileText, Scope);
		size_t IndexBuffer = 0;
		for (size_t i = 0; i < Scope.size(); i++)
		{
			auto Item = Scope[i];

			if (Item == UCodeLangExportV[IndexBuffer])
			{
				IndexBuffer++;

				if (UCodeLangExportSymbolSize - 1 == IndexBuffer)
				{
					{//pass the ("
						for (size_t iq = i; iq < Scope.size(); iq++)
						{
							auto Item = Scope[iq];

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
						GetStringliteral(i, Scope, Out);
					}


					{//pass the )
						MovePass(i, Scope, ' ');
						i++;
					}

					{//pass the spaces
						MovePassSpace(i, Scope);
					}

					
					size_t StartIndex = i;
					String Indentifer;
					GetIndentifier(i,FileText,Indentifer);

					if (!OnDo(StartIndex ,Indentifer, i, Scope, Tep, _type.Symbols))
					{
						ClassType::Field V;
						V.Exported = true;

						GetSummaryTag(i, Scope, V.Summary);

						{//pass the spaces
							MovePassSpace(i, Scope);
						}

						GetType(i, Scope, V.Type);
						{//pass the spaces
							MovePassSpace(i, Scope);
						}
						GetIndentifier(i, Scope, V.Name);

						if (Scope[i] == '(')
						{

						}
						else {
							_type.Fields.push_back(std::move(V));
						}
					}
				}
			}
			else
			{
				IndexBuffer = 0;
			}
		}
	}


	Tep._Type = std::move(_type);


	Symbols.push_back(std::move(Tep));

}

void CppHelper::DoVarableOrFunc(size_t StartIndex,const String& Keywordlet, size_t& i, String& FileText, Vector<SymbolData>& Symbols)
{
	Optional<SummaryTag> Sum;
	GetSummaryTag(i,FileText,Sum);
	i = StartIndex;
	Type V;
	GetType(i, FileText, V);

	String Indentifier;
	
	MovePassSpace(i, FileText);
	
	GetIndentifier(i, FileText, Indentifier);


	
	MovePassSpace(i, FileText);
	
	char nextchar = FileText[i];

	if (nextchar == '(')
	{
		i++;
		FuncData func;
		func.Ret = std::move(V);
		func.IsStatic = false;
		

		while (FileText[i] != ')')
		{
			FuncData::Par tep;

			size_t OldNext = i;
			
			String AsStr;
			GetIndentifier(i, FileText, AsStr);
			if (AsStr == UCodeLangOutPartypeString)
			{
				tep.IsOut = true;

				MovePassSpace(i, FileText);
				i++;//pass (
				GetType(i, FileText, tep.Type);

				MovePassSpace(i, FileText);
				i++;//pass )
			}
			else
			{
				i = OldNext;
				GetType(i, FileText, tep.Type);
			}

			

			MovePassSpace(i, FileText);

			GetIndentifier(i, FileText, tep.Name);

			func.Pars.push_back(std::move(tep));

			MovePassSpace(i, FileText);
			if (FileText[i] != ',') { break;}
		}

		SymbolData V;
		V._Name = std::move(Indentifier);
		V.Summary = std::move(Sum);
		V._Type = std::move(func);
		Symbols.push_back(V);
	}
	else
	{

	}

}

bool CppHelper::OnDo(size_t StartIndex,const String& Keywordlet, size_t& i, UCodeAnalyzer::String& Scope, UCodeAnalyzer::CppHelper::SymbolData& Tep, UCodeAnalyzer::Vector<UCodeAnalyzer::CppHelper::SymbolData>& Symbols)
{
	if (Keywordlet == "enum")
	{
		DoEnumType(i, Scope, Tep, Symbols);
		return true;
	}
	else if (Keywordlet == "constexpr")
	{
		DoConstexprType(i, Scope, Tep, Symbols);
		return true;
	}
	else if (Keywordlet == "struct" || Keywordlet == "class")
	{
		DoClassOrStruct(Keywordlet, i, Scope, Tep, Symbols);
		return true;
	}
	else
	{
		DoVarableOrFunc(StartIndex,Keywordlet, i, Scope, Symbols);
		return true;
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
	size_t ScopeCount = 1;
	for (size_t iq = i; iq < FileText.size(); iq++)
	{
		auto Item = FileText[iq];

		if (Item == '{')
		{
			ScopeCount++;
		}
		else if (Item == '}')
		{
			ScopeCount--;
			if (ScopeCount == 0) {
				Out = FileText.substr(i, iq - i);
				i = iq + 1;
				break;
			}
		}
	}
}

const char Letters[] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
const char Numers[] = "1234567890";

bool IsLetter(char Value)
{
	for (size_t i = 0; i < sizeof(Letters); i++)
	{
		if (Value == Letters[i])
		{
			return true;
		}
	}
	return false;
}
bool Isdigit(char Value)
{
	for (size_t i = 0; i < sizeof(Numers); i++)
	{
		if (Value == Numers[i])
		{
			return true;
		}
	}
	return false;
}

bool IsIndextifierChar(char Value)
{
	return IsLetter(Value) || Isdigit(Value) || Value == '_';
}

void CppHelper::GetIndentifier(size_t& i, UCodeAnalyzer::String& FileText, UCodeAnalyzer::String& Out)
{
	for (size_t iq = i; iq < FileText.size(); iq++)
	{
		auto Item = FileText[iq];

		if (!IsIndextifierChar(Item))
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

String CppHelper::ToString(CppToULangState& State, const SymbolData& Syb)
{
	if (auto Item = Syb._Type.Get_If<EnumType>())
	{
		return ToString(State, *Item, Syb);
	}
	else if (auto Item = Syb._Type.Get_If<ConstexprType>())
	{
		return ToString(State, *Item, Syb);
	}
	else if (auto Item = Syb._Type.Get_If<ClassType>())
	{
		return ToString(State, *Item, Syb);
	}
	else
	{
		//throw std::exception("bad path");
	}
	return "";
}

String CppHelper::ToString(CppToULangState& State, const EnumType& Value, const SymbolData& Syb)
{
	String R;
	DoNameSpace(State, Syb, R);


	State.AddScopesUseingScopeCount(R);

	ToString(State, Syb.Summary);
	R += "$" + Syb._Name + "";
	if (Value._EnumBaseType.has_value())
	{
		R += "[";
		R += ToString(State, Value._EnumBaseType.value());
		R += "]";
	}
	else
	{
		R += "[int]";
	}


	if (Value.Fields.size() == 0)
	{
		R += ";";
	}
	else
	{
		R += ":";
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
	R += "eval " + ToString(State,Value._Type) + " " + Syb._Name + " = " + ToString(State, Value._Value);
	R += ";\n";
	return R;
}
String CppHelper::ToString(CppToULangState& State, const ClassType& Value, const SymbolData& Syb)
{
	String R;
	DoNameSpace(State, Syb, R);

	State.AddScopesUseingScopeCount(R);
	ToString(State, Syb.Summary);
	R += "$" + Syb._Name + ":";

	State.ScopeCount++;
	for (auto& Item : Value.Fields)
	{
		R += '\n';
		State.AddScopesUseingScopeCount(R);

		ToString(State, Syb.Summary);
		R += ToString(State, Item.Type) + " ";
		R += Item.Name;

		if (Item.Value.has_value())
		{
			R += " = ";
			R += ToString(State, Item.Value.value());

		}

		R += ";";

	}

	State.ScopeCount--;
	R += '\n';
	R += '\n';
	
	return R;
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
