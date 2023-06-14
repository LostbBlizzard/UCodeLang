#include "IR.hpp"
#include <fstream>
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
		size_t CompilerRet = 0;

		for (size_t i = 0; i < Struct->Fields.size(); i++)
		{
			auto& Item = Struct->Fields[i];
			if (!Item.Offset.has_value())
			{
				size_t fieldsize = GetSize(Struct->Fields[i].Type);
				
				if (fieldsize > CompilerRet) 
				{
					CompilerRet = fieldsize;
				}

				Item.Offset = 0;
			}
		}
		Struct->ObjectSize = CompilerRet;
	}
	else
	{
		size_t CompilerRet = 0;

		for (size_t i = 0; i < Struct->Fields.size(); i++)
		{
			auto& Item = Struct->Fields[i];
			if (!Item.Offset.has_value())
			{
				size_t fieldsize = GetSize(Struct->Fields[i].Type);
				Item.Offset = CompilerRet;
				CompilerRet += fieldsize;
			}
		}
		Struct->ObjectSize = CompilerRet;
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

BytesPtr IRBuilder::ToBytes() const
{
	BitMaker V;
	ToBytes(V, _StaticInit);
	ToBytes(V, _StaticdeInit);
	ToBytes(V, _threadInit);
	ToBytes(V, _threaddeInit);

	{
		V.WriteType((BitMaker::SizeAsBits)Funcs.size());
		for (auto& Item : Funcs)
		{
			ToBytes(V,*Item);
		}
	}

	{
		V.WriteType((BitMaker::SizeAsBits)_Symbols.size());
		for (auto& Item : _Symbols)
		{
			ToBytes(V, *Item);
		}
	}

	{
		V.WriteType((BitMaker::SizeAsBits)_Map.size());
		for (auto& Item : _Map)
		{
			V.WriteType(Item._Key);
			V.WriteType(Item._Value);
		}
	}

	{
		V.WriteType((BitMaker::SizeAsBits)ConstStaticStrings.size());
		for (auto& Item : ConstStaticStrings)
		{
			V.WriteType(Item._Key);
			V.WriteType(Item._Value);
		}
	}
	return V.AsBytePtr();
}

bool IRBuilder::FromBytes(IRBuilder& Out, const BytesView Bytes)
{
	return false;
}

bool IRBuilder::ToFile(const Path& path, const IRBuilder& Value)
{
	std::ofstream File(path, std::ios::binary);
	if (File.is_open())
	{

		BytesPtr Bits = Value.ToBytes();

		File.write((const char*)Bits.Bytes.get(), Bits.Size);


		File.close();
		return true;
	}
	else
	{
		return false;
	}
}

bool IRBuilder::FromFile(IRBuilder& Out, const Path& path)
{
	std::ifstream File(path, std::ios::binary);
	if (File.is_open())
	{
		BytesPtr Bits;
		File.seekg(0, File.end);
		Bits.Size = File.tellg();
		File.seekg(0, File.beg);
		Bits.Bytes = std::make_unique<Byte[]>(Bits.Size);

		File.read((char*)Bits.Bytes.get(), Bits.Size);
		File.close();
		auto V = FromBytes(Out, { Bits.Bytes.get(),Bits.Size });

		return V;
	}
	else
	{
		return false;
	}

}

void IRBuilder::ToBytes(BitMaker& Out, const IRFunc& Value)
{
	Out.WriteType(Value.identifier);
	Out.WriteType(*(IRCallConvention_t*)&Value.CallConvention);
	Out.WriteType(*(IRFuncLink_t*)&Value.Linkage);
	ToBytes(Out, Value.ReturnType);

	{
		Out.WriteType((BitMaker::SizeAsBits)Value.Pars.size());
		for (auto& Item : Value.Pars)
		{
			ToBytes(Out, Item);
		}
	}

	{
		Out.WriteType((BitMaker::SizeAsBits)Value.Blocks.size());
		for (auto& Item : Value.Blocks)
		{
			ToBytes(Out, *Item);
		}
	}
}

void IRBuilder::FromBytes(BitReader& Out, IRFunc& Value)
{
	Out.ReadType(Value.identifier, Value.identifier);
	Out.ReadType(*(IRCallConvention_t*)&Value.CallConvention, *(IRCallConvention_t*)&Value.CallConvention);
	Out.ReadType(*(IRFuncLink_t*)&Value.Linkage, *(IRFuncLink_t*)&Value.Linkage);
	FromBytes(Out, Value.ReturnType);

	{
		BitMaker::SizeAsBits V = 0;
		Out.ReadType(V, V);
		size_t Size = V;

		Value.Pars.resize(Size);
		for (size_t i = 0; i < Size; i++)
		{
			FromBytes(Out, Value.Pars[i]);
		}
	}

	{
		BitMaker::SizeAsBits V = 0;
		Out.ReadType(V, V);
		size_t Size = V;

		Value.Blocks.resize(Size);
		for (size_t i = 0; i < Size; i++)
		{
			Value.Blocks[i] =Unique_ptr<IRBlock>(new IRBlock());
			FromBytes(Out, *Value.Blocks[i]);
		}
	}
}


void IRBuilder::ToBytes(BitMaker& Out, const IRSymbolData& Value)
{
	Out.WriteType(Value.identifier);
	Out.WriteType((IRSymbolType_t)Value.SymType);
	Out.WriteType(Value.Type);

	switch (Value.SymType)
	{
	case IRSymbolType::Struct:
		ToBytes(Out,*Value.Get_ExAs<IRStruct>());
		break;
	case IRSymbolType::StaticArray:
		ToBytes(Out, *Value.Get_ExAs<IRStaticArray>());
		break;
	case IRSymbolType::FuncPtr:
		ToBytes(Out, *Value.Get_ExAs<IRFuncPtr>());
		break;
	case IRSymbolType::StaticVarable:
		break;
	case IRSymbolType::ThreadLocalVarable:
		break;
	default:
		break;
	}
}

void IRBuilder::FromBytes(BitReader& Out, IRSymbolData& Value)
{
	Out.ReadType(Value.identifier, Value.identifier);
	Out.ReadType(*(IRSymbolType_t*)&Value.SymType, *(IRSymbolType_t*)&Value.SymType);
	FromBytes(Out,Value.Type);

	switch (Value.SymType)
	{
	case IRSymbolType::Struct:
		FromBytes(Out, *Value.Get_ExAs<IRStruct>());
		break;
	case IRSymbolType::StaticArray:
		FromBytes(Out, *Value.Get_ExAs<IRStaticArray>());
		break;
	case IRSymbolType::FuncPtr:
		FromBytes(Out, *Value.Get_ExAs<IRFuncPtr>());
		break;
	case IRSymbolType::StaticVarable:
		break;
	case IRSymbolType::ThreadLocalVarable:
		break;
	default:
		break;
	}
}

void IRBuilder::ToBytes(BitMaker& Out, const IRStruct& Value)
{

	{
		Out.WriteType((BitMaker::SizeAsBits)Value.Fields.size());
		for (auto& Item : Value.Fields)
		{
			ToBytes(Out,Item.Type);
			
			Out.WriteType(Item.Offset.has_value());
			if (Item.Offset.has_value())
			{
				Out.WriteType((BitMaker::SizeAsBits)Item.Offset.value());
			}
		}

	}
	Out.WriteType((BitMaker::SizeAsBits)Value.ObjectSize);
	Out.WriteType(Value.IsUnion);
}

void IRBuilder::FromBytes(BitReader& Out, IRStruct& Value)
{

	{
		BitMaker::SizeAsBits V;
		Out.ReadType(V, V);
		size_t Size = V;
		Value.Fields.resize(Size);
		for (auto& Item : Value.Fields)
		{
			FromBytes(Out, Item.Type);

			bool V = false;
			Out.ReadType(V, V);
			if (V)
			{
				BitMaker::SizeAsBits size = 0;
				Out.ReadType(size, size);
			
				Item.Offset = size;
			}
		}

	}

	{
		BitMaker::SizeAsBits Size = 0;
		Out.ReadType(Size, Size);
		Value.ObjectSize = Size;
	}
	Out.ReadType(Value.IsUnion, Value.IsUnion);
}

void IRBuilder::ToBytes(BitMaker& Out, const IRStaticArray& Value)
{
	Out.WriteType((BitMaker::SizeAsBits)Value.Count);
	ToBytes(Out,Value.Type);
}

void IRBuilder::FromBytes(BitReader& Out, IRStaticArray& Value)
{
	BitMaker::SizeAsBits V = 0;
	Out.ReadType(V, V);
	Value.Count = V;
	FromBytes(Out, Value.Type);
}

void IRBuilder::ToBytes(BitMaker& Out, const IRFuncPtr& Value)
{
	Out.WriteType((IRCallConvention_t)Value.CallConvention);

	ToBytes(Out, Value.Ret);

	Out.WriteType((BitMaker::SizeAsBits)Value.Pars.size());
	
	for (auto& Item : Value.Pars)
	{
		ToBytes(Out,Item);
	}
}
void IRBuilder::FromBytes(BitReader& Out, IRFuncPtr& Value)
{
	Out.ReadType(*(IRCallConvention_t*)&Value.CallConvention);
	FromBytes(Out,Value.Ret);

	BitMaker::SizeAsBits V = 0;
	Out.ReadType(V, V);
	Value.Pars.resize(V);

	for (auto& Item : Value.Pars)
	{
		FromBytes(Out, Item);
	}
}

void IRBuilder::ToBytes(BitMaker& Out, const IRSymbol& Value)
{
	Out.WriteType(Value.ID);
}

void IRBuilder::FromBytes(BitReader& Out, IRSymbol& Value)
{
	Out.ReadType(Value.ID, Value.ID);
}

void IRBuilder::ToBytes(BitMaker& Out, const IRType& Value)
{
	Out.WriteType((IRTypes_t)Value._Type);

	ToBytes(Out,Value._symbol);
}

void IRBuilder::FromBytes(BitReader& Out, IRType& Value)
{
	Out.ReadType(*(IRTypes_t*)&Value._Type, *(IRTypes_t*)&Value._Type);

	FromBytes(Out, Value._symbol);
}

void IRBuilder::ToBytes(BitMaker& Out, const IRPar& Value)
{
	Out.WriteType(Value.identifier);
	ToBytes(Out, Value.type);
}

void IRBuilder::FromBytes(BitReader& Out, IRPar& Value)
{
	Out.ReadType(Value.identifier, Value.identifier);
	FromBytes(Out, Value.type);
}

void IRBuilder::ToBytes(BitMaker& Out, const IRBlock& Value)
{
	Out.WriteType((BitMaker::SizeAsBits)Value.Instructions.size());
	for (auto& Item : Value.Instructions)
	{
		ToBytes(Out, *Item,Value);
	}
}

void IRBuilder::FromBytes(BitReader& Out, IRBlock& Value)
{
	BitMaker::SizeAsBits V = 0;
	Out.ReadType(V, V);
	Value.Instructions.resize(V);
	for (auto& Item : Value.Instructions)
	{
		Item = Unique_ptr<IRInstruction>(new IRInstruction());
	}
	for (auto& Item : Value.Instructions)
	{
		FromBytes(Out, *Item, Value);
	}
}

void IRBuilder::ToBytes(BitMaker& Out, const IRInstruction& Value, const IRBlock& MyBlock)
{
	Out.WriteType(*(IRInstructionType_t*)&Value.Type);

	ToBytes(Out, Value.ObjectType);

	if (IsOperatorValueInTarget(Value.Type))
	{
		ToBytes(Out, Value.Target(), Value.ObjectType, MyBlock);
	}
	if (IsOperatorValueInInput(Value.Type))
	{
		ToBytes(Out, Value.Input(), Value.ObjectType, MyBlock);
	}
}

void IRBuilder::FromBytes(BitReader& Out, IRInstruction& Value, const IRBlock& MyBlock)
{
	Out.ReadType(*(IRInstructionType_t*)&Value.Type, *(IRInstructionType_t*)&Value.Type);
	FromBytes(Out, Value.ObjectType);

	if (IsOperatorValueInTarget(Value.Type))
	{
		FromBytes(Out, Value.Target(), Value.ObjectType, MyBlock);
	}
	if (IsOperatorValueInInput(Value.Type))
	{
		FromBytes(Out, Value.Input(), Value.ObjectType, MyBlock);
	}
}

void IRBuilder::ToBytes(BitMaker& Out, const IROperator& Value, const IRType& Type, const IRBlock& MyBlock)
{
	Out.WriteType(*(IROperator_t*)&Value.Type);
	if (Value.Type == IROperatorType::Value)
	{
		switch (Type._Type)
		{
		case IRTypes::i8:
			Out.WriteType(Value.Value.AsInt8);
			break;
		case IRTypes::i16:
			Out.WriteType(Value.Value.AsInt16);
			break;
		case IRTypes::i32:
			Out.WriteType(Value.Value.AsInt32);
			break;
		case IRTypes::i64:
			Out.WriteType(Value.Value.AsInt64);
			break;
		case IRTypes::f32:
			Out.WriteType(Value.Value.Asfloat32);
			break;
		case IRTypes::f64:
			Out.WriteType(Value.Value.Asfloat64);
			break;
		default:
			break;
		}
	}
}

void IRBuilder::FromBytes(BitReader& Out, IROperator& Value, const IRType& Type, const IRBlock& MyBlock)
{
	Out.ReadType(*(IROperator_t*)&Value.Type, *(IROperator_t*)&Value.Type);
	if (Value.Type == IROperatorType::Value)
	{
		switch (Type._Type)
		{
		case IRTypes::i8:
			Out.ReadType(Value.Value.AsInt8, Value.Value.AsInt8);
			break;
		case IRTypes::i16:
			Out.ReadType(Value.Value.AsInt16, Value.Value.AsInt16);
			break;
		case IRTypes::i32:
			Out.ReadType(Value.Value.AsInt32, Value.Value.AsInt32);
			break;
		case IRTypes::i64:
			Out.ReadType(Value.Value.AsInt64, Value.Value.AsInt64);
			break;
		case IRTypes::f32:
			Out.ReadType(Value.Value.AsInt32, Value.Value.AsInt32);
			break;
		case IRTypes::f64:
			Out.ReadType(Value.Value.AsInt64, Value.Value.AsInt64);
			break;
		default:
			break;
		}
	}
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
bool IRBuilder::IsPrimitive(const IRType& Type) const
{
	if (Type._Type == IRTypes::IRsymbol)
	{
		auto Syb = GetSymbol(Type._symbol);

		return Syb->SymType == IRSymbolType::FuncPtr;
	}
	return true;
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
				case IRInstructionType::SIntToUInt:
					r += ToString(I->ObjectType);
					r += " " + State.GetName(I.get());
					r += " = ";
					r += ToString(State, *I, I->Target()) + " -> uint";
					break;
				case IRInstructionType::UIntToSInt:
					r += ToString(I->ObjectType);
					r += " " + State.GetName(I.get());
					r += " = ";
					r += ToString(State, *I, I->Target()) + " -> sint";
					break;


				case IRInstructionType::UIntToUInt8:
					r += ToString(I->ObjectType);
					r += " " + State.GetName(I.get());
					r += " = ";
					r += ToString(State, *I, I->Target()) + " -> uint8";
					break;
				case IRInstructionType::UIntToUInt16:
					r += ToString(I->ObjectType);
					r += " " + State.GetName(I.get());
					r += " = ";
					r += ToString(State, *I, I->Target()) + " -> uint16";
					break;
				case IRInstructionType::UIntToUInt32:
					r += ToString(I->ObjectType);
					r += " " + State.GetName(I.get());
					r += " = ";
					r += ToString(State, *I, I->Target()) + " -> uint32";
					break;
				case IRInstructionType::UIntToUInt64:
					r += ToString(I->ObjectType);
					r += " " + State.GetName(I.get());
					r += " = ";
					r += ToString(State, *I, I->Target()) + " -> uint64";
				break;

				case IRInstructionType::SIntToSInt8:
					r += ToString(I->ObjectType);
					r += " " + State.GetName(I.get());
					r += " = ";
					r += ToString(State, *I, I->Target()) + " -> sint8";
					break;
				case IRInstructionType::SIntToSInt16:
					r += ToString(I->ObjectType);
					r += " " + State.GetName(I.get());
					r += " = ";
					r += ToString(State, *I, I->Target()) + " -> sint16";
					break;
				case IRInstructionType::SIntToSInt32:
					r += ToString(I->ObjectType);
					r += " " + State.GetName(I.get());
					r += " = ";
					r += ToString(State, *I, I->Target()) + " -> sint32";
					break;
				case IRInstructionType::SIntToSInt64:
					r += ToString(I->ObjectType);
					r += " " + State.GetName(I.get());
					r += " = ";
					r += ToString(State, *I, I->Target()) + " -> sint64";
					break;

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
	break;
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