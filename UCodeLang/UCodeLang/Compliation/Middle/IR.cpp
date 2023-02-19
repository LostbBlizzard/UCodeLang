#include "IR.hpp"
UCodeLangStart
 void IRBuilder::Reset()
{
	Funcs.clear();
	_Map.clear();
}

//uses UCodeLang syntax

String IRBuilder::ToString()
{
	String r;
	ToStringState State;

	for (auto& Item : Funcs)
	{
		r += "|" + FromID(Item->identifier);
		r += "[";
		for (auto& Par : Item->Pars)
		{
			r += Par.identifier + " " + ToString(Par.type);
			if (&Par != &Item->Pars.back())
			{
				r += ",";
			}
		}
		r += "] -> ";
		r += ToString(Item->ReturnType);

		if (Item->Blocks.size())
		{
			String Tabs = " ";
		
			
			for (auto& Block : Item->Blocks)
			{
				
				r += Tabs + "//Block \n";
				for (size_t i = 0; i < Block->Instructions.size(); i++)
				{
					auto& I = Block->Instructions[i];
					if (I->Type == IRInstructionType::None){continue;}

					r += Tabs;
					switch (I->Type)
					{
					case IRInstructionType::LoadReturn:
						r += "ret " + ToString(State, *I, I->Target());
						break;
					case IRInstructionType::Load:
						r += ToString(I->ObjectType);
						r += " " + State.GetName(I.get());
						r += " = " + ToString(State,*I, I->Target());
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
					case IRInstructionType::Return:
						if (i != 0 && Block->Instructions[i - 1]->Type == IRInstructionType::LoadReturn) { continue; }
						r += "ret";
						break;
					default:
						break;
					}
					r += ";\n";
				}
				State.PointerToName.clear();
			}
		}
		else
		{
			r += ";\n";
		}

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
		return  State.PointerToName[Value.Pointer];
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