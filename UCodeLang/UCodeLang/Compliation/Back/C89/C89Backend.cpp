#include "C89Backend.hpp"
UCodeLangStart

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

		OutBuffer += "//file.h";

		//because C89 is a primitive compiler we need to reoder all struct types definitions befor there used

		OutBuffer += ToString();

		OutBuffer += "//file.cpp";

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
	OutBuffer += "typedef unsigned char uInt8;\n";
	OutBuffer += "typedef unsigned short uInt16;\n";
	OutBuffer += "typedef unsigned int uInt32;\n";
	OutBuffer += "typedef unsigned long long uInt64;\n";

	OutBuffer += "typedef signed char Int8;\n";
	OutBuffer += "typedef signed short Int16;\n";
	OutBuffer += "typedef signed int Int32;\n";
	OutBuffer += "typedef signed long long Int64;\n";

	OutBuffer += "typedef float float32;\n";
	OutBuffer += "typedef double float64;\n";


	OutBuffer += "//defs\n";
	
	
	OutBuffer += '\n';
	
}

String C89Backend::ToString(const IRType& Type)

{
	switch (Type._Type)
	{
	case IRTypes::i8:return "Int8";
	case IRTypes::i16:return "Int16";
	case IRTypes::i32:return "Int32";
	case IRTypes::i64:return "Int64";
	case IRTypes::f32:return "float32";
	case IRTypes::f64:return "float64";

	case IRTypes::pointer:return "void*";
	case IRTypes::Void:return "void";
	default:
		break;
	}
	return "void";
}

String C89Backend::ToString()
{
	String r;
	ToStringState State;

	
	AddSybToString(r);
	r += "\n";

	ToString(r, &_Input->_StaticInit, State);
	ToString(r, &_Input->_StaticdeInit, State);
	ToString(r, &_Input->_threadInit, State);
	ToString(r, &_Input->_threaddeInit, State);

	for (auto& Item : _Input->Funcs)
	{

		_Func = Item.get();
		ToString(r, Item.get(), State);
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

			r += "typedef struct\n{";

			for (size_t i = 0; i < V->Fields.size(); i++)
			{
				r += " " + ToString(V->Fields[i].Type) + " __" + std::to_string(i) + "; \n";
			}

			r += "}" + SybName + ";\n";
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
			throw std::exception("not added");
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

void C89Backend::ToString(UCodeLang::String& r, const IRFunc* Item, UCodeLang::C89Backend::ToStringState& State)
{
	r += ToString(Item->ReturnType) + " " + FromIDToCindentifier(Item->identifier);
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
	if (Item->Blocks.size())
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
		r += ";\n";
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


