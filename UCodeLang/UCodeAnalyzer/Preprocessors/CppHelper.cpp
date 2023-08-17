#include "CppHelper.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <UCodeLang/LangCore/ScopeHelper.hpp>
UCodeAnalyzerStart
void Replace(String& string, char ToUpdate, char ToChar)
{
	for (auto& Item : string)
	{
		if (Item == ToUpdate)
		{
			Item = ToChar;
		}
	}
}
String  Replace2(const String& string, char ToUpdate, char ToChar)
{
	String R = string;
	Replace(R, ToUpdate, ToChar);
	return R;
}
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

String ToCType(const CppHelper::CPPType& Value)
{
	return Value.Value;
}

String ToCType(const CppHelper::FuncData::Par& Value)
{
	String r= ToCType(Value.Type);
	if (Value.IsOut)
	{
		r += "*";
	}
	return r;
}

bool CppHelper::ParseCppfileAndOutULang(const Path& SrcCpp,const Path& CppLinkFile, const Path& ULangOut)
{


	Vector<SymbolData> Symbols;
	{
		auto FileText = GetString(SrcCpp);
		ParseCppToSybs(FileText, Symbols);
	}
	
	{
		String R;
		CppToULangState state;
		for (auto& Item : Symbols)
		{
			R += ToString(state, Item);
		}

		for (auto& Item : state.InternalNameSpaces)
		{
			R += Item._Key + "::" + "Internal:\n";
			R += Item._Value;

		}

		WriteStringToFile(ULangOut, R);
	}

	{//Link for Cpp
		auto CppLinkText = GetString(CppLinkFile);
		
		UpdateCppLinks(CppLinkText, Symbols);
		WriteStringToFile(CppLinkFile, CppLinkText);
	}
	


	return false;
}


void CppHelper::UpdateCppLinks(UCodeAnalyzer::String& CppLinkText, UCodeAnalyzer::Vector<UCodeAnalyzer::CppHelper::SymbolData>& Symbols)
{
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
					i++;//pass the last char.

					{//pass the (
						MovePass(i, CppLinkText, ' ');
						i++;
					}

					MovePass(i, CppLinkText, ' ');

					String CppLibVar = "";
					{
						GetIndentifier(i, CppLinkText, CppLibVar);
					}

					{//pass the ,
						MovePass(i, CppLinkText, ' ');
						i++;
					}

					MovePass(i, CppLinkText, ' ');

					String CppNameSpace;
					{
						GetIndentifier(i, CppLinkText, CppNameSpace);
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
					const char* FuncPtrEnder = "_ptr";

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
						String EndText = CppLinkText.substr(i);


						//removeing the {...}
						String newfile = StartText;
						//newfile += '\n';
						newfile += EndText;


						CppLinkText = newfile;
						i = RemoveOffset;

						int a = 0;
						//CppLinkText = CppLinkText.substr
					}

					{
						String Linkstr;
						Linkstr += "\n";

						AddTabCount(TabCount + 1, Linkstr);
						Linkstr += '{' + (String)WasSetHeader + " \n";


						
						Vector< FuncInfo> V;
						for (auto& Item : Symbols)
						{
							OutputIRLineInfo(Item, V);
						}
						for (auto& Item : V)
						{
							AddTabCount(TabCount + 2, Linkstr);
							Linkstr += "using " + Replace2(Item.FuncFullName,':','_');
							if (Item.OverloadValue.has_value())
							{
								Linkstr += std::to_string(Item.OverloadValue.value());
							}
							Linkstr += FuncPtrEnder;

							Linkstr += " = " + Item.Ret + "(*UCodeLangAPI)(";

							bool IsMemberFuncion = Item.MemberFuncClass.has_value();
							if (IsMemberFuncion)
							{
								String ClassName = CppNameSpace + "::" + Item.MemberFuncClass.value();
								Linkstr += ClassName + "*";
								if (Item.Pars.size())
								{
									Linkstr += ",";
								}
							}

							for (auto& Par : Item.Pars)
							{
								Linkstr += Par;
								if (&Par != &Item.Pars.back())
								{
									Linkstr += ",";
								}
							}

							Linkstr += ");" + (String)" \n";
						}


						for (auto& Item : V)
						{
							String FuncName = CppNameSpace + "::" + Item.FuncName;

							AddTabCount(TabCount + 2, Linkstr);

							Linkstr += CppLibVar;
							Linkstr += ".Add_CPPCall(\"";
							Linkstr += Item.Ulangnamespace + UCodeLang::ScopeHelper::_ScopeSep;


							if (Item.OverloadValue.has_value())
							{
								Linkstr += "Internal:";

								String V = Item.FuncFullName.substr(0, Item.FuncFullName.size() - Item.FuncName.size());
								Linkstr += Replace2(V, ':', '_');
								Linkstr += Item.FuncName;

								Linkstr += std::to_string(Item.OverloadValue.value());
							}
							else
							{

								String NewStr;

								{//remove :: and make into :
									char LastChar = 0;
									for (size_t i = 0; i < Item.FuncFullName.size(); i++)
									{
										char Cha = Item.FuncFullName[i];

										if (Cha == ':' && LastChar == ':')
										{
											continue;
										}
										NewStr += Cha;

										LastChar = Cha;
									}
								}
								Linkstr += NewStr;
							}

							Linkstr += "\",[](UCodeLang::InterpreterCPPinterface& Input) \n";
							AddTabCount(TabCount + 3, Linkstr);

							Linkstr += "{\n";
							AddTabCount(TabCount + 3, Linkstr);

							bool IsMemberFuncion = Item.MemberFuncClass.has_value();
							String ClassName;
							if (IsMemberFuncion)
							{
								Linkstr += "\n";
								AddTabCount(TabCount + 4, Linkstr);

								ClassName = CppNameSpace + "::" + Item.MemberFuncClass.value();

								Linkstr += ClassName + "*" + (String)" thisPar" + " = ";
								Linkstr += "Input.GetParameter<" + ClassName + "*" + ">();";

								Linkstr += "\n";
								AddTabCount(TabCount + 4, Linkstr);
							}

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

							if (IsMemberFuncion)
							{
								Linkstr += "thisPar->" + Item.FuncName + "(";
							}
							else 
							{
								Linkstr += FuncName + "(";
							}
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

							if (IsMemberFuncion)
							{
								//c++  member funcion can be casted into funcion pointer.
								//but this is less unsafe.

								Linkstr += "(";
								Linkstr += Replace2(Item.FuncFullName, ':', '_');
								if (Item.OverloadValue.has_value())
								{
									Linkstr += std::to_string(Item.OverloadValue.value());
								}
								Linkstr += FuncPtrEnder + (String)")";

								Linkstr += "[](";
								Linkstr += ClassName + "*" + (String)" thisPar";

								if (Item.Pars.size())
								{
									Linkstr += ',';
								}

								{
									size_t ParCount = 0;
									for (auto& Par : Item.Pars)
									{
										Linkstr += Par;

										Linkstr += " Par";
										Linkstr += std::to_string(ParCount);

										if (&Par != &Item.Pars.back())
										{
											Linkstr += ",";
										}

										ParCount++;
									}
								}
								Linkstr += ")\n";

								AddTabCount(TabCount + 3, Linkstr);

								Linkstr += "{\n";

								AddTabCount(TabCount + 4, Linkstr);
								if (Item.Ret != "void")
								{
									Linkstr += "return ";
								}
								Linkstr += "thisPar->" + Item.FuncName + "(";
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
								Linkstr += ");\n";

								AddTabCount(TabCount + 3, Linkstr);
								Linkstr += "}";
							}
							else
							{
								Linkstr += "(";
								Linkstr += Replace2(Item.FuncFullName, ':', '_');
								if (Item.OverloadValue.has_value())
								{
									Linkstr += std::to_string(Item.OverloadValue.value());
								}
								Linkstr += FuncPtrEnder + (String)")";


								Linkstr += FuncName;

							}

							Linkstr += "); \n";
						}

						AddTabCount(TabCount + 1, Linkstr);
						Linkstr += "}//" + (String)EndHeader;
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
}

void CppHelper::OutputIRLineInfo(UCodeAnalyzer::CppHelper::SymbolData& Item, UCodeAnalyzer::Vector<FuncInfo>& V)
{
	if (auto Val = Item._Type.Get_If<FuncData>())
	{
		FuncInfo Vinfo;
		Vinfo.Ret = ToCType(Val->Ret);
		Vinfo.FuncName = Item._Name;
		Vinfo.OverloadValue = Val->OverloadNumber;
		Vinfo.FuncFullName = Item._FullName;

		Vinfo.MemberFuncClass = Val->MemberClassName;

		Vinfo.Ulangnamespace = Item._NameSpace;
		Vinfo.Pars.resize(Val->Pars.size());
		for (size_t i = 0; i < Val->Pars.size(); i++)
		{
			Vinfo.Pars[i] = ToCType(Val->Pars[i]);
		}
		V.push_back(std::move(Vinfo));
	}
	else if (auto Val = Item._Type.Get_If<ClassType>())
	{
		for (auto& Item : Val->Symbols)
		{
			OutputIRLineInfo(Item, V);
		}
	}
}

void CppHelper::ParseCppToSybs(UCodeAnalyzer::String& FileText, UCodeAnalyzer::Vector<UCodeAnalyzer::CppHelper::SymbolData>& Symbols)
{
	size_t IndexBuffer = 0;
	UCodeLang::VectorMap<String, size_t> Overloads;
	ParseCppState State;
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
				if (OnDo(StartIndex, Indentifer, i, FileText, Tep, Symbols, State))
				{
					auto& Last = Symbols.back();
					if (auto Val = Last._Type.Get_If<FuncData>())
					{
						DoOverLoadOnFunc(Overloads, Last, Val);

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

void CppHelper::DoOverLoadOnFunc(UCodeLang::VectorMap<UCodeAnalyzer::String, size_t>& Overloads, UCodeAnalyzer::CppHelper::SymbolData& Last, UCodeAnalyzer::CppHelper::FuncData* Val)
{
	if (Overloads.HasValue(Last._Name))
	{
		auto& Item = Overloads.at(Last._Name);


		Val->OverloadNumber = Item;

		Item++;
	}
	else
	{
		Overloads.AddValue(Last._Name, 0);
	}
}

void CppHelper::DoConstexprType(size_t& i, UCodeAnalyzer::String& FileText, UCodeAnalyzer::CppHelper::SymbolData& Tep, UCodeAnalyzer::Vector<UCodeAnalyzer::CppHelper::SymbolData>& Symbols, ParseCppState& State)
{
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
	Tep._FullName = State.ScopesAsString() + Tep._Name;
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

void CppHelper::DoEnumType(size_t& i, UCodeAnalyzer::String& FileText, UCodeAnalyzer::CppHelper::SymbolData& Tep, UCodeAnalyzer::Vector<UCodeAnalyzer::CppHelper::SymbolData>& Symbols, ParseCppState& State)
{
	EnumType _type;
	{//pass the spaces
		MovePass(i, FileText, ' ');
	}

	{
		String V;
		size_t oldindex = i;
		GetIndentifier(i, FileText,V);

		if (V != "class")
		{
			i = oldindex;
		}

	}

	{//pass the spaces
		MovePassSpace(i, FileText);
	}

	{
		GetIndentifier(i, FileText, Tep._Name);
		Tep._FullName = State.ScopesAsString() + Tep._Name;
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
		CPPType V;
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

				bool Exclude = false;
				if (field.Name == UCodeLangExcludeString)
				{
					Exclude = true;

					MovePassSpace(i, Scope);
					GetIndentifier(i, Scope, field.Name);
				}

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

				
				if (Exclude==false) {
					_type.Fields.push_back(std::move(field));
				}
				
				if (Scope[i] != ',')
				{
					break;
				}
				else 
				{
					i++;
				}

			}
		}
	}
	int a = 0;

	Tep._Type = std::move(_type);


	Symbols.push_back(std::move(Tep));
}

void CppHelper::DoClassOrStruct(const String& Keywordlet, size_t& i, UCodeAnalyzer::String& FileText, UCodeAnalyzer::CppHelper::SymbolData& Tep, UCodeAnalyzer::Vector<UCodeAnalyzer::CppHelper::SymbolData>& Symbols, ParseCppState& State)
{
	ClassType _type;
	UCodeLang::VectorMap<String, size_t> Overloads;
	{//pass the spaces
		MovePassSpace(i, FileText);
	}

	{
		GetIndentifier(i, FileText, Tep._Name);
		Tep._FullName = State.ScopesAsString() + Tep._Name;
	}


	{//pass the spaces
		MovePassSpace(i, FileText);
	}
	
	//pass '{'
	{
		i++;
	}

	State.Scopes.push_back(Tep._Name);
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

				if (UCodeLangExportVSymbolSize - 1 == IndexBuffer)
				{
					i++;
					MovePassSpace(i, Scope);
					size_t StrStart = i;
					int a = 0;

					SymbolData ItemTep;

					String TepStr;
					{
						GetIndentifier(i, Scope, TepStr);
					}

					if (TepStr == "constexpr")
					{
						i = StrStart;
						DoConstexprType(i, Scope, ItemTep, _type.Symbols,State);
					}
					else 
					{
						i = StrStart;
						bool v = OnDo(StrStart, TepStr, i, Scope, ItemTep, _type.Symbols, State);
						if (v)
						{
							auto& LastSybol = _type.Symbols.back();
							if (auto Val = LastSybol._Type.Get_If<FuncData>())
							{
								LastSybol._NameSpace = Tep._NameSpace;
								DoOverLoadOnFunc(Overloads, LastSybol, Val);

								auto ScopeStr = State.ScopesAsString();
								Val->MemberClassName = ScopeStr.substr(0, ScopeStr.size() -2 );//removeing the  :: at the end.
							}
						}
						else
						{

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
	State.Scopes.pop_back();

	Tep._Type = std::move(_type);


	Symbols.push_back(std::move(Tep));

}

void CppHelper::DoVarableOrFunc(size_t StartIndex,const String& Keywordlet, size_t& i, String& FileText, SymbolData& Tep, Vector<SymbolData>& Symbols, ParseCppState& State)
{
	Optional<SummaryTag> Sum;
	GetSummaryTag(i,FileText,Sum);
	i = StartIndex;
	CPPType V;
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
		

		while (FileText[i] != ')' && i < FileText.size())
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

			char B = FileText[i];
			if (B != ',') { break;}
			else
			{
				i++;
			}
		}

		Tep._Name = std::move(Indentifier);
		Tep._FullName = State.ScopesAsString() + Tep._Name;
		Tep.Summary = std::move(Sum);
		Tep._Type = std::move(func);
		Symbols.push_back(Tep);
	}
	else
	{

	}

}

bool CppHelper::OnDo(size_t StartIndex,const String& Keywordlet, size_t& i, UCodeAnalyzer::String& Scope, UCodeAnalyzer::CppHelper::SymbolData& Tep, UCodeAnalyzer::Vector<UCodeAnalyzer::CppHelper::SymbolData>& Symbols, ParseCppState& State)
{
	if (Keywordlet == "enum")
	{
		DoEnumType(i, Scope, Tep, Symbols, State);
		return true;
	}
	else if (Keywordlet == "constexpr")
	{
		DoConstexprType(i, Scope, Tep, Symbols, State);
		return true;
	}
	else if (Keywordlet == "struct" || Keywordlet == "class")
	{
		DoClassOrStruct(Keywordlet, i, Scope, Tep, Symbols, State);
		return true;
	}
	else if (Keywordlet == "inline")
	{
		DoVarableOrFunc(StartIndex, Keywordlet, i, Scope, Tep, Symbols, State);
		return true;
	}
	else
	{
		DoVarableOrFunc(StartIndex,Keywordlet, i, Scope, Tep, Symbols, State);
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
void CppHelper::GetType(size_t& i, UCodeAnalyzer::String& FileText, CPPType& Out)
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
	else if (auto Item = Syb._Type.Get_If<FuncData>())
	{
		return ToString(State, *Item, Syb);
	}
	else
	{
		UCodeLangUnreachable();//Ptr was not set
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
	R += "$" + Syb._Name + ":\n";

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
	for (auto& Item : Value.Symbols)
	{
		R += ToString(State, Item);
	}

	State.ScopeCount--;
	R += '\n';
	R += '\n';
	
	return R;
}


String CppHelper::ToString(CppToULangState& State, const FuncData& Value, const SymbolData& Syb, bool IsInClass)
{
	String R;
	DoNameSpace(State, Syb, R);

	State.AddScopesUseingScopeCount(R);
	ToString(State, Syb.Summary);


	if (!Value.OverloadNumber.has_value())
	{
		R += "extern dynamic ";
	}
	R += "|" + Syb._Name + "[";
	
	if (Value.MemberClassName.has_value())
	{
		R += "this&";
		if (Value.Pars.size())
		{
			R += ',';
		}
	}

	for (auto& Item : Value.Pars)
	{
		if (Item.IsOut)
		{
			R += "out ";
		}

		R += ToString(State, Item.Type) + " ";
		R += Item.Name;
		if (&Item != &Value.Pars.back())
		{
			R += ",";
		}
	}

	R += "] ";

	if (Value.OverloadNumber.has_value())
	{
		String CppFuncName = Replace2(Syb._FullName, ':', '_');
		CppFuncName += std::to_string(Value.OverloadNumber.value());

		R += "=> " + (String)"Internal::" + CppFuncName;

		R += "(";
		for (auto& Item : Value.Pars)
		{
			R += Item.Name;
			if (&Item != &Value.Pars.back())
			{
				R += ",";
			}
		}

		R += ");\n";

		String AddStr;
		State.AddScope(AddStr);
		State.AddScope(AddStr);
		AddStr += "extern dynamic |" + CppFuncName;
		AddStr += "[";
		if (Value.MemberClassName.has_value())
		{
			AddStr += "this&";
			if (Value.Pars.size())
			{
				AddStr += ',';
			}
		}
		for (auto& Item : Value.Pars)
		{
			if (Item.IsOut)
			{
				AddStr += "out ";
			}

			AddStr += ToString(State, Item.Type) + " ";
			AddStr += Item.Name;

			if (&Item != &Value.Pars.back())
			{
				AddStr += ",";
			}
		}

		AddStr += "] -> " + ToString(State, Value.Ret) + ";\n";

		String NameSpace;
		if (Syb._NameSpace.size()==0)
		{
			NameSpace = State.LastNameSpace;
		}
		else
		{
			NameSpace = Syb._NameSpace;
		}

		State.InternalNameSpaces[NameSpace] += AddStr;

	}
	else 
	{
		R += "-> " + ToString(State, Value.Ret);

		R += ";\n";
	}
	return R;
}
String CppHelper::ToString(CppToULangState& State, const CPPType& Value)
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
	else if (CMPStr == "size_t"
		 || CMPStr == "uintptr_t"
		 || CMPStr == "uintptr")
	{
		R += "uintptr";
	}
	else if (CMPStr == "intptr_t"
		|| CMPStr == "intptr")
	{
		
		R += "sintptr";
	}
	else if (CMPStr == "void")
	{
		R += "void";
	}
	else
	{
		R += CMPStr;
	}


	return R;
}

String CppHelper::ToString(CppToULangState& State, const CPPExpression& Value)
{
	return Value.Value;
}

void CppHelper::DoNameSpace(UCodeAnalyzer::CppHelper::CppToULangState& State, const UCodeAnalyzer::CppHelper::SymbolData& Syb, UCodeAnalyzer::String& R)
{
	if (Syb._FullName == Syb._Name) {
		if (State.LastNameSpace != Syb._NameSpace)
		{
			State.LastNameSpace = Syb._NameSpace;
			R += State.LastNameSpace;
			R += ":\n";
		}
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
