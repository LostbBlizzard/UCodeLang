#include "C89Backend.hpp"
#include "../../Middle/IR.hpp"
#include "../../CompliationSettings.hpp"
#include "../../Helpers/CompliationErrors.hpp"
#include "UCodeLang/LangCore/UClib.hpp"
UCodeLangStart




#define IRFuncEntryPointName "_Entry"


#define IRPrefix "UCIR_"

#define IRMDefinesName IRPrefix "_C89Defs"

#define IRMSVCDefineName IRPrefix "Compiler_MSVC"
#define IRCLangDefineName IRPrefix "Compiler_Clang"
#define IRGNUCDefineName IRPrefix "Compiler_GCC"
#define IREmscriptenDefineName IRPrefix "Compiler_Emscripten"

#define IRWindowsDefineName IRPrefix "Platform_Windows"
#define IRLinuxDefineName IRPrefix "Platform_Linux"
#define IRMacOSDefineName IRPrefix "Platform_MacOS"
#define IRANDROIDDefineName IRPrefix "Platform_ANDROID"
#define IRIPHONEDefineName IRPrefix "Platform_IPHONE"
#define IRWasmDefineName IRPrefix "Platform_Wasm"

#define IRForceinlineDefineName IRPrefix "Forceinline"

#define IRUnreachableDefineName IRPrefix "Unreachable"

#define IRAssumeDefineName IRPrefix "Assume"

#define IRhreadLocal "ThreadLocalKeyWord"

#define IRReturnValue "_ReturnVal"

#define IRReinterpretCastTep "_tep"

C89Backend::C89Backend()
{

}
C89Backend::~C89Backend()
{

}
void C89Backend::Reset()
{

	this->~C89Backend();
	new (this)  C89Backend;
}
void C89Backend::Build(const IRBuilder* Input)
{
	_Input = Input; 
	
	if (Get_Settings()._Type == OutPutType::IRAndSymbols)
	{
		if (HasLibOutput())
		{
			auto _OutLayer = Getliboutput().AddLayer(UCode_CodeLayer_IR_Name);
			CodeLayer::JustData V; 
			V._Data = Input->ToBytes().MoveToVector();
			_OutLayer->_Data = std::move(V);

		}
		else
		{
			OutBuffer += "/*Made using UCodeLang C89 Backend.Next Part is an IR Binary this will be used when linking.*/";
			
			auto Data = Input->ToBytes();
			OutBuffer += String_view((char*)Data.Data(), Data.Size());

			Set_Output(OutBuffer);
		}
	}
	else
	{
		AddTextSignature();


		//types
		AddBaseTypes();

		
		//in C89 need to reoder all struct types definitions befor there used

		OutBuffer += ToString();

		

		Set_Output(OutBuffer);
	}
}

void C89Backend::AddTextSignature()
{
	OutBuffer += "/*Made using UCodeLang C89 Backend.\n";
	OutBuffer += "\n";
	OutBuffer += "\n";
	OutBuffer += "*/\n";
}

void C89Backend::AddBaseTypes()
{
	
	OutBuffer += "/*defs*/\n\n";
	
	OutBuffer += "#if defined(_MSC_VER)\n";
	OutBuffer += "#define " + (String)IRMSVCDefineName + " 1 \n";
	OutBuffer += "#else\n";
	OutBuffer += "#define " + (String)IRMSVCDefineName + " 0 \n";
	OutBuffer += "#endif\n\n";

	OutBuffer += "#if defined(__GNUC__)\n";
	OutBuffer += "#define " + (String)IRGNUCDefineName + " 1 \n";
	OutBuffer += "#else\n";
	OutBuffer += "#define " + (String)IRGNUCDefineName + " 0 \n";
	OutBuffer += "#endif\n\n";

	OutBuffer += "#if defined(__clang__)\n";
	OutBuffer += "#define " + (String)IRCLangDefineName + " 1 \n";
	OutBuffer += "#else\n";
	OutBuffer += "#define " + (String)IRCLangDefineName + " 0 \n";
	OutBuffer += "#endif\n\n";

	OutBuffer += "#if _WIN64 || _WIN32\n";
	OutBuffer += "#define " + (String)IRWindowsDefineName + " 1 \n";
	OutBuffer += "#else\n";
	OutBuffer += "#define " + (String)IRWindowsDefineName + " 0 \n";
	OutBuffer += "#endif\n\n";

	
	OutBuffer += "#if __gnu_linux__ || __linux__\n";
	OutBuffer += "#define " + (String)IRLinuxDefineName + " 1 \n";
	OutBuffer += "#else\n";
	OutBuffer += "#define " + (String)IRLinuxDefineName + " 0 \n";
	OutBuffer += "#endif\n\n";
	
	OutBuffer += "#if __APPLE__ && __MACH__\n";
	OutBuffer += "#define " + (String)IRMacOSDefineName + " 1 \n";
	OutBuffer += "#else\n";
	OutBuffer += "#define " + (String)IRMacOSDefineName + " 0 \n";
	OutBuffer += "#endif\n\n";

	OutBuffer += "#if __APPLE__\n";
	OutBuffer += "#include \"TargetConditionals.h\";\n";

	OutBuffer += "#if TARGET_OS_IPHONE\n";
	OutBuffer += "#define " + (String)IRIPHONEDefineName + " 1 \n";
	OutBuffer += "#else\n";
	OutBuffer += "#define " + (String)IRIPHONEDefineName + " 0 \n";
	OutBuffer += "#endif\n";

	OutBuffer += "#else\n";
	OutBuffer += "#define UCodeLang_Platform_IPHONE 0\n";
	OutBuffer += "#endif\n\n"; 

	OutBuffer += "#if __ANDROID__\n";
	OutBuffer += "#define " + (String)IRANDROIDDefineName + " 1 \n";
	OutBuffer += "#else\n";
	OutBuffer += "#define " + (String)IRANDROIDDefineName + " 0 \n";
	OutBuffer += "#endif\n\n";

	OutBuffer += "#if defined(__wasm32__) || defined(__wasm64__)\n";
	OutBuffer += "#define " + (String)IRWasmDefineName + " 1 \n";
	OutBuffer += "#else\n";
	OutBuffer += "#define " + (String)IRWasmDefineName + " 0 \n";
	OutBuffer += "#endif\n\n";

	OutBuffer += "#if " + (String)IRMSVCDefineName + "\n";
	OutBuffer += "#define " + (String)IRForceinlineDefineName + " __forceinline\n";
	OutBuffer += "#elif " + (String)IRGNUCDefineName + "\n";
	OutBuffer += "#define " + (String)IRForceinlineDefineName + " __attribute__((always_inline))\n";
	OutBuffer += "#else\n";
	OutBuffer += "#define " + (String)IRForceinlineDefineName + " inline\n";
	OutBuffer += "#endif\n\n";

	//OutBuffer += "#if " + (String)IRMSVCDefineName + "\n";
	OutBuffer += "#define " + (String)IRhreadLocal + " \n";
	//OutBuffer += "#elif " + (String)IRGNUCDefineName + "\n";
	//OutBuffer += "#define " + (String)IRThreadLocal  + " __attribute__((always_inline))\n";
	//OutBuffer += "#else\n";
	//OutBuffer += "#define " + (String)IRForceinlineDefineName + " inline\n";
	//OutBuffer += "#endif\n\n";

	OutBuffer += "#if " + (String)IRMSVCDefineName + "\n";
	OutBuffer += "#define " + (String)IRUnreachableDefineName + " __assume(0)\n";
	OutBuffer += "#elif " + (String)IRGNUCDefineName + "\n";
	OutBuffer += "#define " + (String)IRUnreachableDefineName + " __builtin_unreachable()\n";
	OutBuffer += "#else\n";
	OutBuffer += "#define " + (String)IRUnreachableDefineName + " \n";
	OutBuffer += "#endif\n\n";


	OutBuffer += "#if " + (String)IRMSVCDefineName + "\n";
	OutBuffer += "#define " + (String)IRAssumeDefineName + "(X) __assume(X)\n";
	OutBuffer += "#elif " + (String)IRCLangDefineName + "\n";
	OutBuffer += "#define " + (String)IRAssumeDefineName + "(X) __builtin_assume(X)\n";
	OutBuffer += "#else\n";
	OutBuffer += "#define " + (String)IRAssumeDefineName + "(X) \n";
	OutBuffer += "#endif\n\n";

	OutBuffer += "/*includes*/\n\n";
	
	OutBuffer += "#include <inttypes.h>\n";
	OutBuffer += "#include <stdlib.h>\n";

	OutBuffer += "/*Types*/\n";
	OutBuffer += "typedef float float32_t;\n";
	OutBuffer += "typedef double float64_t;\n";

	
	OutBuffer += '\n';
	
}

String C89Backend::ToString(const IRType& Type, bool IsUnsigned)

{
	switch (Type._Type)
	{
	case IRTypes::i8:return IsUnsigned ? "uint8_t" : "int8_t";
	case IRTypes::i16:return IsUnsigned ? "uint16_t" : "int16_t";
	case IRTypes::i32:return IsUnsigned ? "uint32_t" : "int32_t";
	case IRTypes::i64:return IsUnsigned ? "uint64_t"  : "int64_t";
	case IRTypes::f32:return "float32_t";
	case IRTypes::f64:return "float64_t";

	
	case IRTypes::Void:return "void";

	case IRTypes::pointer:
	{
		String r;
		auto Syb = _Input->GetSymbol(Type._symbol);
		if (Syb)
		{
			r += FromIDToCindentifier(Syb->identifier) + "*";
		}
		else {
			r += "void*";
		}
		return r;
	}
	break;
	case IRTypes::IRsymbol:
	{
		auto Syb = _Input->GetSymbol(Type._symbol);
		if (Syb)
		{
			return FromIDToCindentifier(Syb->identifier);
		}
	}
	break;
	default:
		UCodeLangUnreachable();
		break;
	}
	return "void";
}

String C89Backend::ToString()
{
	String r;
	ToStringState State;

	r += "\n\n/*file.h*/\n\n";

	AddSybToString(r);
	r += "\n";


	{
		ToString(r, &_Input->_StaticInit, State);
		ToString(r, &_Input->_StaticdeInit, State);
		ToString(r, &_Input->_threadInit, State);
		ToString(r, &_Input->_threaddeInit, State);

		for (auto& Item : _Input->Funcs)
		{

			_Func = Item.get();
			ToString(r, Item.get(), State);
		}

		if (_Input->EntryPoint.has_value())
		{
			r += "int main(int argc, char** argv);";
		}
	}
	r += "\n\n/*file.cpp*/\n\n";
	{
		{
			ToString(r, &_Input->_StaticInit, State,true);
			ToString(r, &_Input->_StaticdeInit, State, true);
			ToString(r, &_Input->_threadInit, State, true);
			ToString(r, &_Input->_threaddeInit, State, true);

			for (auto& Item : _Input->Funcs)
			{

				_Func = Item.get();


				if (_Func->Linkage == IRFuncLink::StaticLink) {
					ToString(r, Item.get(), State, true);
				}

			}

			if (_Input->EntryPoint.has_value()) 
			{
				r += "int main(int argc, char** argv)\n";
				
				r += "{\n";

				r += FromIDToCindentifier(_Input->_StaticInit.identifier) + "();\n";
				r += FromIDToCindentifier(_Input->_threadInit.identifier) + "();\n\n";
				
				auto IsVoid = _Input->GetFunc(_Input->EntryPoint.value())->ReturnType._Type == IRTypes::Void;
				
				if (!IsVoid)
				{
					r += "int exitcode = ";
				}
				
				r += IRFuncEntryPointName;
				r += "();\n";
				
				r += + "\n" + FromIDToCindentifier(_Input->_threaddeInit.identifier) + "();\n";
				r += FromIDToCindentifier(_Input->_StaticdeInit.identifier) + "();\n";

				if (!IsVoid)
				{
					r += "\n return exitcode;\n";
				}
				else
				{
					r += "\n return 0;\n";
				}
				r += "}";
			}
		}
	}

	return r;
}

void C89Backend::AddSybToString(UCodeLang::String& r)
{
	UnorderedMap<IRidentifierID, bool> Vals;
	Vals.reserve(_Input->_Symbols.size());

	for (auto& Item : _Input->_Symbols)
	{
		Vals.AddValue(Item->identifier, false);
	}
	size_t Values = _Input->_Symbols.size();


	while (Values != 0)
	{


		for (auto& Item : _Input->_Symbols)
		{
			if (Vals.GetValue(Item->identifier) == false)
			{

				String SybName = FromIDToCindentifier(Item->identifier);
				switch (Item->SymType)
				{
				case IRSymbolType::FuncPtr:
				{
					IRFuncPtr* V = Item->Get_ExAs<IRFuncPtr>();

					for (auto& Item2 : V->Pars)
					{
						if (Item2._Type == IRTypes::IRsymbol || Item2._Type == IRTypes::pointer)
						{
							if (Vals.HasValue(Item2._symbol.ID))
							{
								if (Vals.GetValue(Item2._symbol.ID) == false)
								{

									goto NextMainLoop;
								}
							}
						}
					}

					r += "typedef ";
					r += ToString(V->Ret);
					r += "(*" + SybName + ")";
					r += "(";

					for (auto& Item2 : V->Pars)
					{
						r += ToString(Item2);
						if (&Item2 != &V->Pars.back())
						{
							r += ",";
						}
					}

					r += ")";
					r += ";\n\n";

				}
				break;
				case IRSymbolType::Struct:
				{
					IRStruct* V = Item->Get_ExAs<IRStruct>();

					for (size_t i = 0; i < V->Fields.size(); i++)
					{
						auto& Item2 = V->Fields[i].Type;
						if (Item2._Type == IRTypes::IRsymbol || Item2._Type == IRTypes::pointer)
						{
							if (Vals.HasValue(Item2._symbol.ID))
							{
								if (Vals.GetValue(Item2._symbol.ID) == false)
								{

									goto NextMainLoop;
								}
							}
						}
					}

					r += "typedef ";

					if (V->IsUnion)
					{
						r += "union";
					}
					else
					{
						r += "struct";
					}

					r += " \n{\n";

					for (size_t i = 0; i < V->Fields.size(); i++)
					{
						r += " " + ToString(V->Fields[i].Type) + " __" + std::to_string(i) + "; \n";
					}
					if (V->Fields.size() == 0)
					{
						r += " int __Empty; /*C requires that a struct or union has at least one member*/";
					}

					r += "\n} " + SybName + ";\n\n";
				}
				break;
				case IRSymbolType::StaticArray:
				{
					IRStaticArray* V = Item->Get_ExAs<IRStaticArray>();

					{
						auto& Item2 = V->Type;
						if (Item2._Type == IRTypes::IRsymbol || Item2._Type == IRTypes::pointer)
						{
							if (Vals.HasValue(Item2._symbol.ID))
							{
								if (Vals.GetValue(Item2._symbol.ID) == false)
								{

									goto NextMainLoop;
								}
							}
						}
					}

					r += "typedef struct ";
					r += SybName + " {";
					r += ToString(V->Type);
					r += " base[";
					r += std::to_string(V->Count) + "];";
					r += " } ";
					r += SybName;
					r += ";\n\n";
				}
				break;
				case IRSymbolType::StaticVarable:
				{
					IRBufferData* V = Item->Get_ExAs<IRBufferData>();

					{
						auto& Item2 = Item->Type;
						if (Item2._Type == IRTypes::IRsymbol || Item2._Type == IRTypes::pointer)
						{
							if (Vals.HasValue(Item2._symbol.ID))
							{
								if (Vals.GetValue(Item2._symbol.ID) == false)
								{
									goto NextMainLoop;
								}
							}
						}
					}


					r += "static " + ToString(Item->Type);

					r += " " + SybName;
					if (V->Bytes.size())
					{
						r += "[]";
						r += " = ";

						r += "{";
						for (auto& Item : V->Bytes)
						{
							r += std::to_string((int)Item);
							if (&Item != &V->Bytes.back())
							{
								r += ",";
							}
						}
						r += "}";
					}

					UpdateCppLinks(r, V);
				}
				break;
				case IRSymbolType::ThreadLocalVarable:
				{
					IRBufferData* V = Item->Get_ExAs<IRBufferData>();

					{
						auto& Item2 = Item->Type;
						if (Item2._Type == IRTypes::IRsymbol || Item2._Type == IRTypes::pointer)
						{
							if (Vals.HasValue(Item2._symbol.ID))
							{
								if (Vals.GetValue(Item2._symbol.ID) == false)
								{
									continue;
								}
							}
						}
					}

					r += IRhreadLocal + ToString(Item->Type);

					r += " " + SybName;

					if (V->Bytes.size())
					{
						r += "[]";
						r += " = ";

						r += "{";
						for (auto& Item : V->Bytes)
						{
							r += std::to_string((int)Item);
							if (&Item != &V->Bytes.back())
							{
								r += ",";
							}
						}
						r += "}";
					}

					UpdateCppLinks(r, V);
				}
				break;
				default:
					UCodeLangUnreachable();//Ptr was not set
					break;
				}

				Vals.GetValue(Item->identifier) = true;
				Values--;
			NextMainLoop:
				int a = 0;
			}
		}
	}
}





void C89Backend::UpdateCppLinks(UCodeLang::String& r, UCodeLang::IRBufferData* V)
{
	//String_view text = String_view((char*)V->Bytes.data(), V->Bytes.size() - 1);
	bool IsCString = false;// V->Bytes.size() && _Input->ConstStaticStrings.HasValue(text);
	
	r += ";\n";
	return;

	if (IsCString)
	{
		r += ";";
		//r += "=" + (String)text;
	}
	else {
		r += "= {";

		for (auto& Item2 : V->Bytes)
		{
			r += std::to_string(Item2);

			if (&Item2 != &V->Bytes.back()) {
				r += ",";
			}
		}

		r += "};\n";
	}
}

void C89Backend::ToString(UCodeLang::String& r, const IRFunc* Item, UCodeLang::C89Backend::ToStringState& State, bool OutputBody)
{
	{
		auto str = FromIDToCindentifier(Item->identifier);
		if (str == "malloc"
			|| str == "free")
		{
			return;
		}
	}

	if (_Input->EntryPoint.has_value() && Item->identifier == _Input->EntryPoint.value())
	{

	}
	r += ToString(Item->ReturnType) + " ";

	if (_Input->EntryPoint.has_value() && Item->identifier == _Input->EntryPoint.value())
	{
		r += IRFuncEntryPointName;
	}
	else
	{
		r += FromIDToCindentifier(Item->identifier);
	}
	r += "(";
	for (auto& Par : Item->Pars)
	{
		r += ToString(Par.type) + " " + FromIDToCindentifier(Par.identifier);
		if (&Par != &Item->Pars.back())
		{
			r += ",";
		}
	}
	r += ")";

	State.PointerToName.clear();
	State.TepPushedParameters.clear();
	State.Val.clear();
	if (OutputBody)
	{

		r += "\n{";
		String Tabs = " ";

		if (!Item->ReturnType.IsSame(IRTypes::Void))
		{
			r +=  "\n ";
			r += ToString(Item->ReturnType) + " " + (String)IRReturnValue ";";
			r +=  "\n";
		}
		
		for (auto& Block : Item->Blocks)
		{

			r += Tabs + "/*Block*/ \n";

			

			UnorderedMap<size_t, String> Names;
			for (size_t i = 0; i < Block->Instructions.size(); i++)
			{
				
				
				
				
				auto& I = Block->Instructions[i];
				switch (I->Type)
				{
				case IRInstructionType::Jump:

				case IRInstructionType::ConditionalJump:
					if (!Names.HasValue(I->Target().identifer))
					{
						auto LabelName = "_label" + std::to_string(Names.size());
						Names.AddValue(I->Target().identifer,LabelName);
					}
					break;
				}
			}

			for (size_t i = 0; i < Block->Instructions.size(); i++)
			{
				auto& I = Block->Instructions[i];
				if (I->Type == IRInstructionType::None) { continue; }

				auto DebugInfo = Block->DebugInfo.Get_debugfor(i);
				for (auto& Item : DebugInfo)
				{
					if (auto Val = Item->Debug.Get_If<IRDebugSetFile>())
					{
						r += '\n';
						r += " /*File:" + Val->FileName;
						r += "*/";
					}
					else if (auto Val = Item->Debug.Get_If<IRDebugSetLineNumber>())
					{
						r += '\n';
						r += " /*Line:" + std::to_string(Val->LineNumber);
						r += "*/";
					}
				}
				if (DebugInfo.size())
				{
					r += '\n';
					r += '\n';
				}

				if (I->Type != IRInstructionType::PushParameter)
				{
					r += Tabs;
				}


				OutType = &I->ObjectType;
				IRType tep;
				switch (I->Type)
				{
				case IRInstructionType::LoadReturn:
					r += (String)IRReturnValue + " = " + ToString(State, *I, I->Target());
					break;
				case IRInstructionType::LoadNone:
					r += ToString(I->ObjectType);
					r += " " + State.GetName(I.get());
					break;
				case IRInstructionType::Load:
					r += ToString(I->ObjectType);
					r += " " + State.GetName(I.get());
					r += " = " + ToString(State, *I, I->Target());
					break;
				case IRInstructionType::Reassign:
					r += ToString(State, *I, I->Target());
					r += " = " + ToString(State, *I, I->Input());
					break;
				case IRInstructionType::Reassign_dereference:
					r += "*(";
					r += ToString(I->ObjectType);
					r += "*)";
					r += ToString(State, *I, I->Target());
					r += " = " + ToString(State, *I, I->Input());
					break;
				case IRInstructionType::Add:
					tep = *OutType;
					OutType = &tep;
					if (tep._Type == IRTypes::pointer)
					{
						tep = Get_Settings().PtrSize == IntSizes::Int64 ? IRTypes::i64 : IRTypes::i32;
					}

					r += ToStringBinary(State, I.get(), "+");
					break;
				case IRInstructionType::Sub:
					tep = *OutType;
					OutType = &tep;
					if (tep._Type == IRTypes::pointer)
					{
						tep = Get_Settings().PtrSize == IntSizes::Int64 ? IRTypes::i64 : IRTypes::i32;
					}

					r += ToStringBinary(State, I.get(), "-");
					break;
				case IRInstructionType::UMult:
				case IRInstructionType::SMult:
					tep = *OutType;
					OutType = &tep;
					if (tep._Type == IRTypes::pointer)
					{
						tep = Get_Settings().PtrSize == IntSizes::Int64 ? IRTypes::i64 : IRTypes::i32;
					}

					r += ToStringBinary(State, I.get(), "*");
					break;
				case IRInstructionType::SDiv:
				case IRInstructionType::UDiv:
					tep = *OutType;
					OutType = &tep;
					if (tep._Type == IRTypes::pointer)
					{
						tep = Get_Settings().PtrSize == IntSizes::Int64 ? IRTypes::i64 : IRTypes::i32;
					}

					r += ToStringBinary(State, I.get(), "/");
					break;
				case IRInstructionType::EqualTo:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += ToStringBinary(State, I.get(), "==");
					break;
				case IRInstructionType::NotEqualTo:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += ToStringBinary(State, I.get(), "!=");
					break;

				case IRInstructionType::UGreaterThan:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += ToStringBinary(State, I.get(), ">");
					break;
				case IRInstructionType::ULessThan:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += ToStringBinary(State, I.get(), "<");
					break;
				case IRInstructionType::UGreaterThanOrEqual:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += ToStringBinary(State, I.get(), ">=");
					break;
				case IRInstructionType::ULessThanOrEqual:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += ToStringBinary(State, I.get(), "<=");
					break;

				case IRInstructionType::SGreaterThan:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += ToStringBinary(State, I.get(), ">");
					break;
				case IRInstructionType::SLessThan:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += ToStringBinary(State, I.get(), "<");
					break;
				case IRInstructionType::SGreaterThanOrEqual:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += ToStringBinary(State, I.get(), ">=");
					break;
				case IRInstructionType::SLessThanOrEqual:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += ToStringBinary(State, I.get(), "<=");
					break;

				case IRInstructionType::Logical_And:
					r += ToStringBinary(State, I.get(), "&&");
					break;
				case IRInstructionType::Logical_Or:
					r += ToStringBinary(State, I.get(), "||");
					break;
				case IRInstructionType::PushParameter:
					State.TepPushedParameters.push_back(I.get());
					continue;
				case IRInstructionType::Logical_Not:
					r += ToString(I->ObjectType);
					r += " " + State.GetName(I.get());
					r += " = !" + ToString(State, *I, I->Target());
					break;
				case IRInstructionType::Jump:
					r += "goto ";
					r += Names.GetValue(I->Target().identifer);
					break;
				case IRInstructionType::ConditionalJump:
					r += "if (";
					r += ToString(State, *I, I->Input());
					r += "){goto ";
					r += Names.GetValue(I->Target().identifer);
					r += "; }";
					break;
				case IRInstructionType::Call:
				{
					if (!I->ObjectType.IsSame(IRTypes::Void))
					{
						r += ToString(I->ObjectType);
						r += " " + State.GetName(I.get());
						r += " = ";
					}
					r += FromIDToCindentifier(I->Target().identifer) + "(";
					for (auto& Item : State.TepPushedParameters)
					{
						r += State.PointerToName.GetValue(Item->Target().Pointer);
						if (&Item != &State.TepPushedParameters.back())
						{
							r += ",";
						}
					}
					State.TepPushedParameters.clear();
					r += ")";
				}break;
				case IRInstructionType::CallFuncPtr:
				{
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += ToString(I->ObjectType);
					r += " " + State.GetName(I.get());
					r += " = ";
					r += ToString(State, *I, I->Target()) + "(";
					for (auto& Item : State.TepPushedParameters)
					{
						r += State.PointerToName.GetValue(Item->Target().Pointer);
						if (&Item != &State.TepPushedParameters.back())
						{
							r += ",";
						}
					}
					State.TepPushedParameters.clear();
					r += ")";
				}break;
				case IRInstructionType::Return:
					if (Item->ReturnType.IsSame(IRTypes::Void))
					{
						r += "return";
					}
					else
					{
						r += "return " + (String)IRReturnValue;
					}
					break;
				case IRInstructionType::Member_Access:
					State.PointerToName.AddValue(I.get(),ToString(State, *I, I->Target()) + ".__" + std::to_string(I->Input().Value.AsUIntNative));
					goto GoOver;
					break;
				case IRInstructionType::Member_Access_Dereference:
					State.PointerToName.AddValue(I.get(),ToString(State, *I, I->Target()) + "->__" + std::to_string(I->Input().Value.AsUIntNative));
					goto GoOver;
					break;
				case IRInstructionType::MallocCall:
					OutType = &tep;
					tep = Get_Settings().PtrSize == IntSizes::Int64 ? IRTypes::i64 : IRTypes::i32;
					r += ToString(I->ObjectType);
					r += " " + State.GetName(I.get());
					r += " = malloc(" + ToString(State, *I, I->Target()) + ")";
					break;
				case IRInstructionType::FreeCall:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += "free(" + ToString(State, *I, I->Target()) + ")";
					break;
				case IRInstructionType::SIntToUInt:
				{
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());

					auto valname = State.GetName(I.get());

					r += ToString(I->ObjectType, true);
					r += " " + valname + IRReinterpretCastTep;
					r += " = ";
					r += "(";
					r += ToString(I->ObjectType, true);
					r += ")";

					r += ToString(State, *I, I->Target());

					r += ";\n ";

					r += ToString(I->ObjectType);
					r += " " + valname;
					r += " = ";


					r += "*(";
					r += ToString(I->ObjectType);
					r += "*)&";

					r += valname + IRReinterpretCastTep;
				}
				break;
				case IRInstructionType::UIntToSInt:
				{
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					auto valname = State.GetName(I.get());

					r += ToString(I->ObjectType);
					r += " " + valname + IRReinterpretCastTep;
					r += " = ";
					r += ToString(State, *I, I->Target());
					r += ";\n ";


					r += ToString(I->ObjectType);
					r += " " + valname;
					r += " = ";

					r += "(";
					r += ToString(I->ObjectType);
					r += ")";

					r += "(";

					r += "*(";
					r += ToString(I->ObjectType, true);
					r += "*)&";

					r += valname + IRReinterpretCastTep;

					r += ")";
				}
					break;

				case IRInstructionType::SIntToSInt8:
				case IRInstructionType::SIntToSInt16:
				case IRInstructionType::SIntToSInt32:
				case IRInstructionType::SIntToSInt64:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());

					r += ToString(I->ObjectType);
					r += " " + State.GetName(I.get());
					r += " = ";

					r += "(";
					r += ToString(I->ObjectType);
					r += ")";

					r += ToString(State, *I, I->Target());
					break;

				case IRInstructionType::UIntToUInt8:
				case IRInstructionType::UIntToUInt16:
				case IRInstructionType::UIntToUInt32:
				case IRInstructionType::UIntToUInt64:
				{
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());

					auto valname = State.GetName(I.get());


					r += ToString(tep, true);
					r += " " + valname + IRReinterpretCastTep;
					r += " = ";
					r += "*(";
					r += ToString(tep,true);
					r += "*)&";

					r += ToString(State, *I, I->Target());

					r += ";\n ";

					r += ToString(I->ObjectType,true);
					r += " " + valname + (String)IRReinterpretCastTep + (String)"2";
					r += " = ";

					r += "(";
					r += ToString(I->ObjectType, true);
					r += ")";
					r += valname + IRReinterpretCastTep;
					r += ";\n ";

					r += ToString(I->ObjectType);
					r += " " + valname;
					r += " = ";

					r += "*(";
					r += ToString(I->ObjectType);
					r += "*)&";
					r += valname + String(IRReinterpretCastTep) + String("2");
				}	
				break;
				case IRInstructionType::Unreachable:
					r += IRUnreachableDefineName;
					break;
				case IRInstructionType::Assume:
					r += IRAssumeDefineName;
					r += "(";
					r += ToString(State, *I, I->Target());
					r += ")";
					break;
				default:
					UCodeLangUnreachable();
					break;
				}
				r += ";\n";
				GoOver:
				for (auto& Item : Names)
				{
					if (Item.first == i)
					{
						r += Tabs;
						r += Item.second+ ":";
						r += "\n";

					}
				}
			}
			State.PointerToName.clear();
		}
		

		r += "\n}";
	}
	else
	{
		r += ';';
	}
	r += "\n";
}


String C89Backend::ToString(ToStringState& State, IRInstruction& Ins, IROperator& Value)
{
	String r;

	if (Ins.Type == IRInstructionType::Member_Access || Ins.Type == IRInstructionType::Member_Access_Dereference)
	{
		int a = 0;
	}
	
	if (Ins.Type != IRInstructionType::Member_Access
		&& Ins.Type != IRInstructionType::Member_Access_Dereference) {

		IRType ThisVal = _Input->GetType(&Ins, Value);

		if (!ThisVal.IsSame(*OutType))
		{
			bool isok = false;
			if (Ins.Type == IRInstructionType::Reassign
				|| Ins.Type == IRInstructionType::Reassign_dereference)
			{
				isok = &Ins.Target() != &Value;
			}
			else
			{
				isok = true;
			}

			if (isok)
			{
				r += "(";
				r += ToString(*OutType);
				r += ")";
			}
		}
	}
	switch (Value.Type)
	{
	case IROperatorType::Value:
	{
		switch (Ins.ObjectType._Type)
		{
		case IRTypes::i8:r += std::to_string(Value.Value.AsInt8); break;
		case IRTypes::i16:r += std::to_string(Value.Value.AsInt16); break;
		case IRTypes::i32:r += std::to_string(Value.Value.AsInt32); break;
		case IRTypes::i64:r += std::to_string(Value.Value.AsInt64); break;
		case IRTypes::f32:r += std::to_string(Value.Value.Asfloat32); break;
		case IRTypes::f64:r += std::to_string(Value.Value.Asfloat64); break;
		default:UCodeLangUnreachable();
		}
	}
	break;
	case IROperatorType::IRidentifier:
	{
		r += FromIDToCindentifier(Value.identifer);
	}
	break;
	case IROperatorType::Get_PointerOf_IRidentifier:
	{
		r += "&" + FromIDToCindentifier(Value.identifer);
	}
	break;
	case IROperatorType::IRInstruction:
	{
		r += State.PointerToName.GetValue(Value.Pointer);
	}
	break;
	case IROperatorType::Get_PointerOf_IRInstruction:
	{
		r += "&" + State.PointerToName.GetValue(Value.Pointer);
	}
	break;
	case IROperatorType::DereferenceOf_IRInstruction:
	{
		r += "*(" + ToString(*OutType) + "*)" + State.PointerToName.GetValue(Value.Pointer);
	}
	break;
	case IROperatorType::IRParameter:
	{
		r += FromIDToCindentifier(Value.Parameter->identifier);
	}
	break;
	case IROperatorType::DereferenceOf_IRParameter:
	{
		r +=  "*(" + ToString(*OutType) + "*)" + FromIDToCindentifier(Value.Parameter->identifier);
	}
	break;
	case IROperatorType::Get_PointerOf_IRParameter:
	{
		r += "&" + FromIDToCindentifier(Value.Parameter->identifier);
	}
	break;
	case IROperatorType::Get_Func_Pointer:
	{
		r += "&" + FromIDToCindentifier(Value.identifer);
	}
	break;
	default:UCodeLangUnreachable();
	}

	return r;
}

String C89Backend::ToStringBinary(ToStringState& State, IRInstruction* Ins, const char* V)
{
	String r;
	r += ToString(Ins->ObjectType);
	r += " " + State.GetName(Ins);
	r += " = " + ToString(State, *Ins, Ins->A) + String(V) + ToString(State, *Ins, Ins->B);
	return r;
}

String C89Backend::UpdateToCindentifier(const String& Value)
{
	const char* GoodChar = "qwerttyuiopasdfghjklzxcvbnm1234567890QWERTYUIOPASDFGHJKLZXCVBNM_";
	size_t GoodCharSize = strlen(GoodChar);

	String v;
	for (size_t i = 0; i < Value.size(); i++)
	{
		char Char = Value[i];

		bool IsGoodChar = false;
		for (size_t i = 0; i < GoodCharSize; i++)
		{
			if (Char == GoodChar[i])
			{
				IsGoodChar = true;
				break;
			}
		}


		if (IsGoodChar)
		{
			v.push_back(Char);

		}
		else
		{
			v.push_back('_');
		}

	}
	return v;
}

void C89Backend::UpdateBackInfo(CompliationBackEndInfo& BackInfo)
{
	BackInfo.Output = CompliationBackEndInfo::BackEnd::C89;
	BackInfo.OutputSet = CompliationBackEndInfo::InsSet::Other;
}

String C89Backend::FromIDToCindentifier(IRidentifierID Value)
{ 
	return UpdateToCindentifier(_Input->FromID(Value));
}
String C89Backend::ToStringState::GetName(IRInstruction* Ptr)
{
	if (Val.size() == 0)
	{
		Val = 'A';
	}
	else
	{
		if (Val.back() == 'Z')
		{
			Val += 'A';
		}
		char r = Val.back();
		Val.back()++;


	}
	Val += "tepvir";

	auto V = Val;
	if (!PointerToName.HasValue(Ptr)) {
		PointerToName.AddValue(Ptr, V);
	}
	return V;
}

UCodeLangEnd

