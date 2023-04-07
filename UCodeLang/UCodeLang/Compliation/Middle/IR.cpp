#include "IR.hpp"
UCodeLangStart



IRidentifierID IRBuilder::ToID(const IRidentifier& Value)
{
	String V = Value;

	auto r = std::hash<IRidentifier>()(V);
	_Map[r] = V;
	return r;
}
void IRBuilder::Reset()
{
	this->~IRBuilder();
	new (this)   IRBuilder;
}

//uses UCodeLang syntax


void IRBuilder::Fix_Size(IRStruct* Struct)
{
	if (Struct->IsUnion) 
	{
		size_t R = 0;

		for (size_t i = 0; i < Struct->Fields.size(); i++)
		{
			auto& Item = Struct->Fields[i];
			if (!Item.Offset.has_value())
			{
				size_t fieldsize = GetSize(Struct->Fields[i].Type);
				
				if (fieldsize > R) 
				{
					R = fieldsize;
				}

				Item.Offset = 0;
			}
		}
		Struct->ObjectSize = R;
	}
	else
	{
		size_t R = 0;

		for (size_t i = 0; i < Struct->Fields.size(); i++)
		{
			auto& Item = Struct->Fields[i];
			if (!Item.Offset.has_value())
			{
				size_t fieldsize = GetSize(Struct->Fields[i].Type);
				R += fieldsize;
				Item.Offset = R;
			}
		}
		Struct->ObjectSize = R;
	}
}


//for backends

size_t IRBuilder::GetSize(const IRType& Type) const
{
	switch (Type._Type)
	{
	case IRTypes::i8:return 1;
	case IRTypes::i16:return 2;
	
	case IRTypes::f32:
	case IRTypes::i32:return 4;
	
	case IRTypes::f64:
	case IRTypes::i64:return 8;

	Pointer:
	case IRTypes::pointer:return 8;
	case IRTypes::IRsymbol:
	{
		const IRSymbolData* Sym = GetSymbol(Type._symbol);
		switch (Sym->SymType)
		{
		case IRSymbolType::Struct:
			return Sym->Get_ExAs<IRStruct>()->ObjectSize;
			break;
		case IRSymbolType::StaticArray:
		{
			auto V = Sym->Get_ExAs<IRStaticArray>();
			return V->Count * GetSize(V->Type);
		}
		case IRSymbolType::FuncPtr:goto Pointer;
		default:break;
		}
	}
	break;
	default:
		throw std::exception("not added");
		break;
	}
	return 0;
}

size_t IRBuilder::GetSize(const IRStruct* Struct) const
{
	return  Struct->ObjectSize;
}

size_t IRBuilder::GetOffset(const IRStruct* Struct, size_t Index) const
{
	return  Struct->Fields[Index].Offset.value();
}

//

IRBuilder::IRBuilder()
{
	_StaticInit.identifier = ToID("_StaticInit");
	_StaticInit.ReturnType = IRTypes::Void;

	_StaticdeInit.identifier = ToID("_Static_deInit");
	_StaticdeInit.ReturnType = IRTypes::Void;

	_threadInit.identifier = ToID("_threadInit");
	_threadInit.ReturnType = IRTypes::Void;

	_threaddeInit.identifier = ToID("_thread_deInit");
	_threaddeInit.ReturnType = IRTypes::Void;
}

bool IRBuilder::IsTheSame(const IRFuncPtr* Func, const IRFuncPtr* Func2)const
{
	bool Ok = Func->CallConvention == Func2->CallConvention
		&& Func->Pars.size() == Func2->Pars.size()
		&& IsTheSame(Func->Ret, Func2->Ret);

	if (Ok)
	{
		for (size_t i = 0; i < Func2->Pars.size(); i++)
		{
			if (!IsTheSame(Func->Pars[i], Func2->Pars[i]))
			{
				return false;
			}
		}
	}

	return Ok;
}
bool IRBuilder::IsTheSame(const IRFuncPtr* Func, const IRFunc* Func2)const
{
	bool Ok = Func->CallConvention == Func2->CallConvention
		&& Func->Pars.size() == Func2->Pars.size()
		&& IsTheSame(Func->Ret, Func2->ReturnType);

	if (Ok)
	{
		for (size_t i = 0; i < Func2->Pars.size(); i++)
		{
			if (!IsTheSame(Func->Pars[i], Func2->Pars[i].type))
			{
				return false;
			}
		}
	}

	return Ok;
}
bool IRBuilder::IsTheSame(const IRFunc* Func, const IRFunc* Func2)const
{
	bool Ok = Func->CallConvention == Func2->CallConvention
		&& IsTheSame(Func->ReturnType, Func2->ReturnType)
		&& Func->Pars.size() == Func2->Pars.size();

	if (Ok)
	{
		for (size_t i = 0; i < Func2->Pars.size(); i++)
		{
			if (!IsTheSame(Func->Pars[i].type, Func2->Pars[i].type))
			{
				return false;
			}
		}
	}

	return Ok;
}

String IRBuilder::ToString()
{
	String r;
	ToStringState State;

	for (auto& Item : _Symbols)
	{
		String SybName =FromID(Item->identifier);
		switch (Item->SymType)
		{
		case IRSymbolType::FuncPtr:
		{
			IRFuncPtr* V = Item->Get_ExAs<IRFuncPtr>();
			r += "$" + SybName + " = |[";

			for (auto& Item2 : V->Pars)
			{
				r += ToString(Item2);
				if (&Item2 != &V->Pars.back())
				{
					r += ",";
				}
			}

			r += +"] -> ";
			r += ToString(V->Ret);
			r += ";\n\n";
		}
		break;
		case IRSymbolType::Struct:
		{
			IRStruct* V = Item->Get_ExAs<IRStruct>();

			if (V->IsUnion) 
			{
				r += "union " + SybName + "\n";
			}
			else
			{
				r += "$" + SybName + "\n";
			}

			for (size_t i = 0; i < V->Fields.size(); i++)
			{
				r += " " + ToString(V->Fields[i].Type) + " __" + std::to_string(i) + "; \n";
			}

			r += "\n";
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
			r += "static " + (String)"uint8" + "[/" + std::to_string(V->Bytes.size()) + "]:" + ToString(Item->Type) + " " + SybName + "= [";
			
			for (auto& Item2 : V->Bytes)
			{
				if (V->IsInitialized) {
					r += std::to_string(Item2);
				}
				else
				{
					r += '?';
				}

				if (&Item2 != &V->Bytes.back()) {
					r += ",";
				}
			}

			r += "]\n";
		}
		break;
		case IRSymbolType::ThreadLocalVarable:
		{
			IRBufferData* V = Item->Get_ExAs<IRBufferData>();
			r += "thread " + (String)"uint8" + "[/" + std::to_string(V->Bytes.size()) + "]:" + ToString(Item->Type) + " " + SybName + "= [";

			for (auto& Item2 : V->Bytes)
			{
				if (V->IsInitialized) {
					r += std::to_string(Item2);
				}
				else
				{
					r += '?';
				}

				if (&Item2 != &V->Bytes.back()) {
					r += ",";
				}
			}

			r += "]\n";
		}
		break;
		default:
			throw std::exception("not added");
			break;
		}
	}
	r += "\n";


	//r += "//_Static_init\n";

	ToString(State, &_StaticInit, r);

	//r += "//_Static_deInit\n";


	ToString(State, &_StaticdeInit, r);

	//r += "//_threadInit\n";

	ToString(State, &_threadInit, r);
	
	//r += "//_thread_deInit\n";
	
	ToString(State, &_threaddeInit, r);

	for (auto& Item : Funcs)
	{
		State.StrValue = 0;
		State.PointerToName.clear();
		State.TepPushedParameters.clear();
		//

		ToString(State, Item.get(), r);
	}

	return r;
}
void IRBuilder::ToString(ToStringState& State, IRFunc* Item, String& r)
{
	State._Func = Item;
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

			BinaryVectorMap<size_t, String> Names;
			for (size_t i = 0; i < Block->Instructions.size(); i++)
			{
				auto& I = Block->Instructions[i];
				switch (I->Type)
				{
				case IRInstructionType::Jump:

				case IRInstructionType::ConditionalJump:
					if (!Names.HasValue(I->Target().identifer)) {
						Names.AddValue(I->Target().identifer, "_label" + std::to_string(Names.size()));
					}
					break;
				}
			}

			for (size_t i = 0; i < Block->Instructions.size(); i++)
			{
				auto& I = Block->Instructions[i];
				if (I->Type == IRInstructionType::None) { continue; }

				if (I->Type != IRInstructionType::PushParameter
					&& I->Type != IRInstructionType::Member_Access
					&& I->Type != IRInstructionType::Member_Access_Dereference)
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
				case  IRInstructionType::Member_Access:
				{
					State.PointerToName[I.get()] = ToString(State, *I, I->Target()) + ".__" + std::to_string(I->Input().Value.AsUIntNative);
					continue;
				}
				case  IRInstructionType::Member_Access_Dereference:
				{
					State.PointerToName[I.get()] = ToString(State, *I, I->Target()) + "->__" + std::to_string(I->Input().Value.AsUIntNative);
					continue;
				}
				default:
					throw std::exception("not added");
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
	}
	else
	{
		r += ";\n";
	}
	r += "\n";
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
	case IRTypes::pointer:return "void*";
	case IRTypes::IRsymbol:
	{
		auto Syb = GetSymbol(Type._symbol);
		if (Syb)
		{
			return FromID(Syb->identifier);
		}
	}
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
	case IROperatorType::Get_PointerOf_IRidentifier:
	{
		return "&" + FromID(Value.identifer);
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