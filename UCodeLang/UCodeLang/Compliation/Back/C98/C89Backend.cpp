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
	OutBuffer.clear();
}
void C89Backend::Build(const IRBuilder* Input)
{
	_Input = Input; 
	
	AddTextSignature();
	
	
	//types
	AddBaseTypes();
	
	OutBuffer += ToString();
}

void C89Backend::AddTextSignature()
{
	OutBuffer += "//Made using UCodeLang C89 Backend.\n";
	OutBuffer += "//\n";
	OutBuffer += "//\n";
}

void C89Backend::AddBaseTypes()
{
	OutBuffer += "typedef unsigned char UInt8 ;\n";
	OutBuffer += "typedef unsigned short UInt16;\n";
	OutBuffer += "typedef unsigned int UInt32;\n";
	OutBuffer += "typedef unsigned long long UInt64;\n";

	OutBuffer += "typedef signed char SInt8;\n";
	OutBuffer += "typedef signed short SInt16;\n";
	OutBuffer += "typedef signed int SInt32;\n";
	OutBuffer += "typedef signed long long SInt64;\n";

	OutBuffer += "typedef float float32;\n";
	OutBuffer += "typedef double float64;\n";
}

String C89Backend::ToString(const IRType& Type)

{
	switch (Type._Type)
	{
	case IRTypes::i8:return "UInt8";
	case IRTypes::i16:return "UInt16";
	case IRTypes::i32:return "UInt32";
	case IRTypes::i64:return "UInt64";
	case IRTypes::f32:return "float32";
	case IRTypes::f64:return "float64";

	case IRTypes::pointer:return "void*";
	case IRTypes::Void:return "void";
	default:
		break;
	}
	return "var";
}

String C89Backend::ToString()
{
	String r;
	ToStringState State;

	for (auto& Item : _Input->Funcs)
	{
		r += ToString(Item->ReturnType) + " " + _Input->FromID(Item->identifier);
		r += "(";
		for (auto& Par : Item->Pars)
		{
			r += ToString(Par.type) + " " + _Input->FromID(Par.identifier);
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

				Unordered_map<size_t, String> Names;
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
						r += _Input->FromID(I->Target().identifer) + "(";
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
						if (Item.first == i)
						{
							r += Tabs;
							r += Item.second + ":";
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

	return r;
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

UCodeLangEnd


