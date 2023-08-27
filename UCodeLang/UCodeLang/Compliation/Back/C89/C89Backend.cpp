#include "C89Backend.hpp"
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
			OutBuffer += "//Made using UCodeLang C89 Backend.Next Part is an IR Binary this will be used when linking.";
			
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
	OutBuffer += "//Made using UCodeLang C89 Backend.\n";
	OutBuffer += "//\n";
	OutBuffer += "//\n";
}

void C89Backend::AddBaseTypes()
{
	
	OutBuffer += "//defs\n\n";
	
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
	OutBuffer += "#include \"TargetConditionals.h\";";

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
	OutBuffer += "#define" + (String)IRForceinlineDefineName + "__forceinline\n";
	OutBuffer += "#elif " + (String)IRGNUCDefineName + "\n";
	OutBuffer += "#define " + (String)IRForceinlineDefineName + " __attribute__((always_inline))\n";
	OutBuffer += "#else\n";
	OutBuffer += "#define " + (String)IRForceinlineDefineName + " inline\n";
	OutBuffer += "#endif\n\n";

	OutBuffer += "//includes\n\n";
	
	OutBuffer += "#include <inttypes.h>\n";

	OutBuffer += "//Types\n";
	OutBuffer += "//typedef float float32_t;\n";
	OutBuffer += "//typedef double float64_t;\n";

	
	OutBuffer += '\n';
	
}

String C89Backend::ToString(const IRType& Type)

{
	switch (Type._Type)
	{
	case IRTypes::i8:return "int8_t";
	case IRTypes::i16:return "int16_t";
	case IRTypes::i32:return "int32_t";
	case IRTypes::i64:return "int64_t";
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
		break;
	}
	return "void";
}

String C89Backend::ToString()
{
	String r;
	ToStringState State;

	r += "\n\n//file.h\n\n";

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
	r += "\n\n//file.cpp\n\n";
	{
		{
			ToString(r, &_Input->_StaticInit, State,true);
			ToString(r, &_Input->_StaticdeInit, State, true);
			ToString(r, &_Input->_threadInit, State, true);
			ToString(r, &_Input->_threaddeInit, State, true);

			for (auto& Item : _Input->Funcs)
			{

				_Func = Item.get();
				ToString(r, Item.get(), State,true);
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
					r += "\n return 0;";
				}
				r += "}";
			}
		}
	}

	return r;
}

void C89Backend::AddSybToString(UCodeLang::String& r)
{
	for (auto& Item : _Input->_Symbols)
	{
		String SybName = FromIDToCindentifier(Item->identifier);
		switch (Item->SymType)
		{
		case IRSymbolType::FuncPtr:
		{
			IRFuncPtr* V = Item->Get_ExAs<IRFuncPtr>();
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

			r += "\n} " + SybName + ";\n\n";
		}
		break;
		case IRSymbolType::StaticArray:
		{
			IRStaticArray* V = Item->Get_ExAs<IRStaticArray>();
			r += "$" + SybName + " = " + ToString(V->Type) + "[/" + std::to_string(V->Count) + "]\n";
		}
		break;
		case IRSymbolType::StaticVarable:
		{
			IRBufferData* V = Item->Get_ExAs<IRBufferData>();
			r += "static " + ToString(Item->Type) + " " + SybName;
			UpdateCppLinks(r, V);
		}
		break;
		case IRSymbolType::ThreadLocalVarable:
		{
			IRBufferData* V = Item->Get_ExAs<IRBufferData>();
			r += "thread_local " + ToString(Item->Type) + " " + SybName;
			UpdateCppLinks(r, V);
		}
		break;
		default:
			UCodeLangUnreachable();//Ptr was not set
			break;
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

	if (OutputBody)
	{

		r += "\n{";
		String Tabs = " ";

		
		for (auto& Block : Item->Blocks)
		{

			r += Tabs + "//Block \n";

			BinaryVectorMap<size_t, String> Names;
			for (size_t i = 0; i < Block->Instructions.size(); i++)
			{
				auto& I = Block->Instructions[i];
				switch (I->Type)
				{
				case IRInstructionType::Jump:

				case IRInstructionType::ConditionalJump:
					Names[I->Target().identifer] = "_label" + std::to_string(Names.size());
					break;
				}
			}

			for (size_t i = 0; i < Block->Instructions.size(); i++)
			{
				auto& I = Block->Instructions[i];
				if (I->Type == IRInstructionType::None) { continue; }

				if (I->Type != IRInstructionType::PushParameter)
				{
					r += Tabs;
				}



				switch (I->Type)
				{
				case IRInstructionType::LoadReturn:
					r += "return " + ToString(State, *I, I->Target());
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
				case IRInstructionType::Add:
					r += ToStringBinary(State, I.get(), "+");
					break;
				case IRInstructionType::Sub:
					r += ToStringBinary(State, I.get(), "-");
					break;
				case IRInstructionType::UMult:
				case IRInstructionType::SMult:
					r += ToStringBinary(State, I.get(), "*");
					break;
				case IRInstructionType::SDiv:
				case IRInstructionType::UDiv:
					r += ToStringBinary(State, I.get(), "/");
					break;
				case IRInstructionType::EqualTo:
					r += ToStringBinary(State, I.get(), "==");
					break;
				case IRInstructionType::NotEqualTo:
					r += ToStringBinary(State, I.get(), "!=");
					break;

				case IRInstructionType::UGreaterThan:
					r += ToStringBinary(State, I.get(), ">");
					break;
				case IRInstructionType::ULessThan:
					r += ToStringBinary(State, I.get(), "<");
					break;
				case IRInstructionType::UGreaterThanOrEqual:
					r += ToStringBinary(State, I.get(), ">=");
					break;
				case IRInstructionType::ULessThanOrEqual:
					r += ToStringBinary(State, I.get(), "<=");
					break;

				case IRInstructionType::SGreaterThan:
					r += ToStringBinary(State, I.get(), ">");
					break;
				case IRInstructionType::SLessThan:
					r += ToStringBinary(State, I.get(), "<");
					break;
				case IRInstructionType::SGreaterThanOrEqual:
					r += ToStringBinary(State, I.get(), ">=");
					break;
				case IRInstructionType::SLessThanOrEqual:
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
					r += Names[I->Target().identifer];
					break;
				case IRInstructionType::ConditionalJump:
					r += "gotoif (";
					r += ToString(State, *I, I->Input());
					r += ") ";
					r += Names[I->Target().identifer];
					break;
				case IRInstructionType::Call:
				{
					r += ToString(I->ObjectType);
					r += " " + State.GetName(I.get());
					r += " = ";
					r += FromIDToCindentifier(I->Target().identifer) + "(";
					for (auto& Item : State.TepPushedParameters)
					{
						r += State.PointerToName.at(Item->Target().Pointer);
						if (&Item != &State.TepPushedParameters.back())
						{
							r += ",";
						}
					}
					State.TepPushedParameters.clear();
					r += ")";
				}break;
				case IRInstructionType::Return:
					if (i != 0 && Block->Instructions[i - 1]->Type == IRInstructionType::LoadReturn) { continue; }
					r += "return";
					break;
				default:
					UCodeLangUnreachable();
					break;
				}
				r += ";\n";

				for (auto& Item : Names)
				{
					if (Item._Key == i)
					{
						r += Tabs;
						r += Item._Value + ":";
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
	switch (Value.Type)
	{
	case IROperatorType::Value:
	{
		switch (Ins.ObjectType._Type)
		{
		case IRTypes::i8:return std::to_string(Value.Value.AsInt8);
		case IRTypes::i16:return std::to_string(Value.Value.AsInt16);
		case IRTypes::i32:return std::to_string(Value.Value.AsInt32);
		case IRTypes::i64:return std::to_string(Value.Value.AsInt64);
		case IRTypes::f32:return std::to_string(Value.Value.Asfloat32);
		case IRTypes::f64:return std::to_string(Value.Value.Asfloat64);
		default:return "[]";
		}
	}
	case IROperatorType::IRidentifier:
	{
		return _Input->FromID(Value.identifer);
	}

	case IROperatorType::IRInstruction:
	{
		//for

		return  State.PointerToName.at(Value.Pointer);
	}
	case IROperatorType::Get_PointerOf_IRInstruction:
	{
		return "&" + State.PointerToName.at(Value.Pointer);
	}
	case IROperatorType::IRParameter:
	{
		const IRPar* Par = _Func->GetPar(Value.identifer);
		
		return _Input->FromID(Par->identifier);
	}
	default:return "[]";
	}
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

String C89Backend::FromIDToCindentifier(IRidentifierID Value)
{ 
	return UpdateToCindentifier(_Input->FromID(Value));
}

UCodeLangEnd


