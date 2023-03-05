#include "IR.hpp"
UCodeLangStart

bool In3list(char V,const char* N)
{
	for (size_t i = 0; i < strlen(N); i++)
	{
		if (N[i] == V) {
			return true;
		}
	}
	return false;
}

IRidentifierID IRBuilder::ToID(const IRidentifier& Value)
{
	String V = Value;


	for (size_t i = 0; i < V.size(); i++)
	{
		auto C = V[i];


		if (!In3list(C,"qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890"))
		{
			V[i] = '_';
		}
	}

	auto r = std::hash<IRidentifier>()(V);
	_Map[r] = V;
	return r;
}
void IRBuilder::Reset()
{
	Funcs.clear();
	_Map.clear();
}

//uses UCodeLang syntax

size_t IRBuilder::GetSize(const IRType& Type) const
{
	switch (Type._Type)
	{
	case IRTypes::i8:return 1;
	case IRTypes::i16:return 2;
	case IRTypes::i32:return 4;
	case IRTypes::i64:return 8;
	default:
		throw std::exception("not added");
		break;
	}
}

String IRBuilder::ToString()
{
	String r;
	ToStringState State;

	for (auto& Item : Funcs)
	{
		State._Func = Item.get();
		r += "|" + FromID(Item->identifier);
		r += "[";
		for (auto& Par : Item->Pars)
		{
			r += ToString(Par.type) + " " + FromID(Par.identifier);
			if (&Par != &Item->Pars.back())
			{
				r += ",";
			}
		}
		r += "] -> ";
		r += ToString(Item->ReturnType);
		r += ":";
		if (Item->Blocks.size())
		{
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
					case IRInstructionType::MallocCall:
						r += ToString(I->ObjectType);
						r += " " + State.GetName(I.get());
						r += " = LowLevel::Malloc(" + ToString(State, *I, I->Target()) + ")";
						break;
					case IRInstructionType::FreeCall:
						r += "LowLevel::Free(" + ToString(State, *I, I->Target()) + ")";
						break;
					case IRInstructionType::LoadReturn:
						r += "ret " + ToString(State, *I, I->Target());
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
						r += FromID(I->Target().identifer) + "(";
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
					case IRInstructionType::Reassign_dereference:
						r += "*" + ToString(State, *I, I->Target());
						r += " = " + ToString(State, *I, I->Input());
						break;
					case IRInstructionType::CallFuncPtr:
						r += ToString(I->ObjectType);
						r += " " + State.GetName(I.get());
						r += " = ";
						r += ToString(State, *I, I->Target()) + "(";
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
						break;
					case IRInstructionType::Return:
						if (i != 0 && Block->Instructions[i - 1]->Type == IRInstructionType::LoadReturn) { continue; }
						r += "ret";
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
		}
		else
		{
			r += ";\n";
		}
		r += "\n";
	}

	return r;
}
String IRBuilder::ToString(const IRType& Type)
{
	switch (Type._Type)
	{
	case IRTypes::i8:return "int8";
	case IRTypes::i16:return "int16";
	case IRTypes::i32:return "int32";
	case IRTypes::i64:return "int64";
	case IRTypes::f32:return "float32";
	case IRTypes::f64:return "float64";


	case IRTypes::Void:return "void";
	case IRTypes::pointer:return "var&";
	default:
		break;
	}
	return "var";
}
String IRBuilder::ToString(ToStringState& State, IRInstruction& Ins, IROperator& Value)
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
		return FromID(Value.identifer);
	}

	case IROperatorType::IRInstruction:
	{
		//for

		return  State.PointerToName.at(Value.Pointer);
	}
	case IROperatorType::IRParameter:
	{
		const IRPar* Par = Value.Parameter;

		return FromID(Par->identifier);
	}
	case IROperatorType::Get_PointerOf_IRInstruction:
	{
		return "&" + State.PointerToName.at(Value.Pointer); 
	}
	case IROperatorType::Get_PointerOf_IRParameter:
	{
		const IRPar* Par = Value.Parameter;
		return "&" + FromID(Par->identifier);
	}

	case IROperatorType::DereferenceOf_IRInstruction:
	{
		return "*" + State.PointerToName.at(Value.Pointer);
	}
	case IROperatorType::DereferenceOf_IRParameter:
	{
		const IRPar* Par = Value.Parameter;
	    return "*" + FromID(Par->identifier);
	}
	case IROperatorType::Get_Func_Pointer:
	{
		return "(&)" + FromID(Value.identifer);
	}
	
	default:return "[]";
	}
}
String IRBuilder::ToStringBinary(ToStringState& State, IRInstruction* Ins, const char* V)
{
	String r;
	r += ToString(Ins->ObjectType);
	r += " " + State.GetName(Ins);
	r += " = " + ToString(State, *Ins, Ins->A) + String(V) + ToString(State, *Ins, Ins->B);
	return r;
}
UCodeLangEnd