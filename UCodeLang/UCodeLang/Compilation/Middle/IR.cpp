#ifndef UCodeLangNoCompiler
#include "IR.hpp"
#include <fstream>
#include <UCodeLang/LangCore/TypeNames.hpp>
UCodeLangStart



IRidentifierID IRBuilder::ToID(const IRidentifier& Value)
{
	String V = Value;
	auto r = std::hash<IRidentifier>()(V);

	if (!_Map.HasValue(r))
	{
		_Map.AddValue(r, V);
	}
	return r;
}
void IRBuilder::Reset()
{
	this->~IRBuilder();
	new (this)   IRBuilder;
}

//uses UCodeLang syntax


void IRBuilder::Fix_Size(IRStruct* Struct, bool is32mode)
{
	if (Struct->IsSizeSet == false)
	{
		if (Struct->IsUnion)
		{
			size_t CompilerRet = 0;

			for (size_t i = 0; i < Struct->Fields.size(); i++)
			{
				auto& Item = Struct->Fields[i];
				if (!Item.Offset.has_value())
				{
					if (Item.Type._symbol.ID)
					{
						auto Sym = GetSymbol(Item.Type._symbol);
						if (Sym->SymType == IRSymbolType::Struct)
						{
							IRStruct* structInfo = Sym->Get_ExAs<IRStruct>();

							if (structInfo->IsSizeSet == false)
							{
								Fix_Size(structInfo, is32mode);
							}
						}

					}
					size_t fieldsize = GetSize(Item.Type, is32mode);

					if (fieldsize > CompilerRet)
					{
						CompilerRet = fieldsize;
					}

					Item.Offset = 0;
				}
			}
			Struct->ObjectSize = CompilerRet;
			Struct->IsSizeSet = true;
		}
		else
		{
			size_t CompilerRet = 0;

			for (size_t i = 0; i < Struct->Fields.size(); i++)
			{
				auto& Item = Struct->Fields[i];

				if (Item.Type._symbol.ID)
				{
					auto Sym = GetSymbol(Item.Type._symbol);
					if (Sym->SymType == IRSymbolType::Struct)
					{
						IRStruct* structInfo = Sym->Get_ExAs<IRStruct>();

						if (structInfo->IsSizeSet == false)
						{
							Fix_Size(structInfo, is32mode);
						}
					}

				}

				size_t fieldsize = GetSize(Item.Type, is32mode);

				if (!Item.Offset.has_value())
				{
					Item.Offset = CompilerRet;
				}
				CompilerRet += fieldsize;
			}
			Struct->ObjectSize = CompilerRet;
			Struct->IsSizeSet = true;
		}
	}
}


//for backends

size_t IRBuilder::GetSize(const IRType& Type, bool is32mode) const
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
	case IRTypes::pointer:return is32mode ? 4 : 8;
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
			return V->Count * GetSize(V->Type, is32mode);
		}
		case IRSymbolType::FuncPtr:goto Pointer;
		default:

			UCodeLangUnreachable();
			break;
		}
	}
	break;
	default:
		UCodeLangUnreachable();
		break;
	}
}

size_t IRBuilder::GetSize(const IRStruct* Struct) const
{
	return  Struct->ObjectSize;
}

size_t IRBuilder::GetOffset(const IRStruct* Struct, size_t Index) const
{
	return  Struct->Fields[Index].Offset.value();
}

IRType IRBuilder::GetType(const IRInstruction* IR, const IROperator& Op) const
{
	UCodeLangAssert(IR->Type != IRInstructionType::Member_Access);//use GetType(IR);
	UCodeLangAssert(IR->Type != IRInstructionType::Member_Access_Dereference);//use GetType(IR);


	if (IsLoadValueOnInput(IR->Type))
	{
		switch (Op.Type)
		{
		case IROperatorType::DereferenceOf_IRInstruction:
		{
			return IR->ObjectType;
		}
		case IROperatorType::DereferenceOf_IRParameter:
		{
			return IR->ObjectType;
		}
		case IROperatorType::Value:
			return  IR->ObjectType;
		default:

			break;
		}
	}

	if (IsOperatorValueInTarget(IR->Type))
	{
		if (IR->Type == IRInstructionType::Reassign || IR->Type == IRInstructionType::Reassign_dereference)
		{
			if (Op.Type == IROperatorType::Value)
			{
				return IR->ObjectType;
			}
		}

		switch (Op.Type)
		{
		case IROperatorType::DereferenceOf_IRInstruction:
		{
			return IR->ObjectType;
		}
		case IROperatorType::DereferenceOf_IRParameter:
		{
			return IR->ObjectType;
		}
		case IROperatorType::Value:
			return  IR->ObjectType;
		default:
			break;
		}
	}

	return GetType(Op);
}

IRType IRBuilder::GetType(const IRInstruction* IR) const
{
	if (IR->Type == IRInstructionType::PushParameter)
	{
		return GetType(IR, IR->Target());
	}

	if (IR->Type == IRInstructionType::Member_Access)
	{
		return IR->ObjectType;
	}
	if (IR->Type == IRInstructionType::Member_Access_Dereference)
	{
		auto Struct = GetSymbol(IR->ObjectType._symbol)->Get_ExAs<IRStruct>();

		return Struct->Fields[IR->Input().Value.AsUInt64].Type;
	}

	return IR->ObjectType;
}

IRType IRBuilder::GetType(const IROperator& IR) const
{
	switch (IR.Type)
	{
	case IROperatorType::IRParameter:return IR.Parameter->type;
	case IROperatorType::IRInstruction:return GetType(IR.Pointer);

	case IROperatorType::IRidentifier:
	{
		auto Id = IR.identifier;
		if (auto Syb = GetSymbol(Id))
		{
			return Syb->Type;
		}
		else
		{
			return IRTypes::Null;
		}

	}
	case IROperatorType::Get_PointerOf_IRInstruction:
	{
		return IRType(IRTypes::pointer, GetType(IR.Pointer)._symbol);
	}
	case IROperatorType::Get_PointerOf_IRParameter:
	{
		return IRType(IRTypes::pointer, IR.Parameter->type._symbol);
	}
	case IROperatorType::Get_PointerOf_IRidentifier:
	{
		auto Id = IR.identifier;
		if (auto Syb = GetSymbol(Id))
		{
			return IRType(IRTypes::pointer, Syb->Type._symbol);
		}
		else
		{
			return IRType(IRTypes::pointer);
		}
	}
	case IROperatorType::Get_Func_Pointer:
	{
		return  IRType(IRTypes::pointer);
	}
	default:
		UCodeLangUnreachable();
		break;
	}
}

//

IRBuilder::IRBuilder()
{
	_StaticInit.identifier = ToID(StaticVariablesInitializeFunc);
	_StaticInit.ReturnType = IRTypes::Void;

	_StaticdeInit.identifier = ToID(StaticVariablesUnLoadFunc);
	_StaticdeInit.ReturnType = IRTypes::Void;


	_threadInit.identifier = ToID(ThreadVariablesInitializeFunc);
	_threadInit.ReturnType = IRTypes::Void;

	_threaddeInit.identifier = ToID(ThreadVariablesUnLoadFunc);
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
			ToBytes(V, *Item);
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
			V.WriteType(Item.first);
			V.WriteType(Item.second);
		}
	}

	{
		V.WriteType((BitMaker::SizeAsBits)ConstStaticStrings.size());
		for (auto& Item : ConstStaticStrings)
		{
			V.WriteType(Item.first);
			V.WriteType(Item.second);
		}
	}
	{
		ToBytes(V, _Debug);
	}
	{
		V.WriteType(EntryPoint.has_value());
		if (EntryPoint.has_value())
		{
			V.WriteType(EntryPoint.value());
		}
	}
	return V.AsBytePtr();
}

bool IRBuilder::FromBytes(IRBuilder& Out, const BytesView Bytes)
{
	BitReader Bits;
	Bits.SetBytes(Bytes.Data(), Bytes.Size());

	FromBytes(Bits, Out._StaticInit);
	FromBytes(Bits, Out._StaticdeInit);
	FromBytes(Bits, Out._threadInit);
	FromBytes(Bits, Out._threaddeInit);

	{
		BitMaker::SizeAsBits V = 0;
		Bits.ReadType(V, V);
		size_t FuncSize = V;

		Out.Funcs.resize(FuncSize);
		for (size_t i = 0; i < FuncSize; i++)
		{
			auto& Item = Out.Funcs[i];
			Item = Unique_ptr<IRFunc>(new IRFunc());
			FromBytes(Bits, *Item);
		}
	}

	{
		BitMaker::SizeAsBits V = 0;
		Bits.ReadType(V, V);
		size_t Size = V;

		Out._Symbols.resize(Size);
		for (size_t i = 0; i < Size; i++)
		{
			auto& Item = Out._Symbols[i];
			Item = Unique_ptr<IRSymbolData>(new IRSymbolData());
			FromBytes(Bits, *Item);
		}
	}

	{
		BitMaker::SizeAsBits V = 0;
		Bits.ReadType(V, V);
		size_t Size = V;

		Out._Map.reserve(Size);
		for (size_t i = 0; i < Size; i++)
		{
			IRidentifierID Key = 0;
			Bits.ReadType(Key, Key);

			IRidentifier Value = {};
			Bits.ReadType(Value, Value);

			Out._Map.AddIfNotHaveKey(std::move(Key), std::move(Value));
		}

	}

	{
		BitMaker::SizeAsBits V = 0;
		Bits.ReadType(V, V);
		size_t Size = V;

		Out.ConstStaticStrings.reserve(Size);
		for (size_t i = 0; i < Size; i++)
		{
			String Key;
			Bits.ReadType(Key, Key);
			IRidentifierID Value;
			Bits.ReadType(Value, Value);

			Out.ConstStaticStrings.AddValue(std::move(Key), std::move(Value));
		}
	}
	{
		FromBytes(Bits, Out._Debug);
	}
	{
		bool HasV = false;
		Bits.ReadType(HasV, HasV);
		if (HasV)
		{
			IRidentifierID Value;
			Bits.ReadType(Value, Value);
			Out.EntryPoint = Value;
		}
	}
	return true;
}

bool IRBuilder::ToFile(const Path& path, const IRBuilder& Value)
{
	std::ofstream File(path, std::ios::binary);
	if (File.is_open())
	{

		BytesPtr Bits = Value.ToBytes();

		File.write((const char*)Bits.Data(), Bits.Size());


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
		Bits.Resize(File.tellg());
		File.seekg(0, File.beg);
		File.read((char*)Bits.Data(), Bits.Size());
		File.close();
		auto V = FromBytes(Out, Bits.AsSpan());

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
			ToBytes(Out, *Item, Value.Pars);
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
			Value.Blocks[i] = Unique_ptr<IRBlock>(new IRBlock());
			FromBytes(Out, *Value.Blocks[i], Value.Pars);
		}
	}
}


void IRBuilder::ToBytes(BitMaker& Out, const IRSymbolData& Value)
{
	Out.WriteType(Value.identifier);
	Out.WriteType((IRSymbolType_t)Value.SymType);
	ToBytes(Out, Value.Type);

	switch (Value.SymType)
	{
	case IRSymbolType::Struct:
		ToBytes(Out, *Value.Get_ExAs<IRStruct>());
		break;
	case IRSymbolType::StaticArray:
		ToBytes(Out, *Value.Get_ExAs<IRStaticArray>());
		break;
	case IRSymbolType::FuncPtr:
		ToBytes(Out, *Value.Get_ExAs<IRFuncPtr>());
		break;
	case IRSymbolType::StaticVarable:
		ToBytes(Out, *Value.Get_ExAs<IRBufferData>());
		break;
	case IRSymbolType::ThreadLocalVarable:
		ToBytes(Out, *Value.Get_ExAs<IRBufferData>());
		break;
	default:
		break;
	}
}

void IRBuilder::FromBytes(BitReader& Out, IRSymbolData& Value)
{
	Out.ReadType(Value.identifier, Value.identifier);
	Out.ReadType(*(IRSymbolType_t*)&Value.SymType, *(IRSymbolType_t*)&Value.SymType);
	FromBytes(Out, Value.Type);

	switch (Value.SymType)
	{
	case IRSymbolType::Struct:
	{
		Value.Ex.reset(new IRStruct());
		FromBytes(Out, *Value.Get_ExAs<IRStruct>());
	}
	break;
	case IRSymbolType::StaticArray:
	{
		Value.Ex.reset(new IRStaticArray());
		FromBytes(Out, *Value.Get_ExAs<IRStaticArray>());
	}
	break;
	case IRSymbolType::FuncPtr:
	{
		Value.Ex.reset(new IRFuncPtr());
		FromBytes(Out, *Value.Get_ExAs<IRFuncPtr>());
	}
	break;
	case IRSymbolType::StaticVarable:
		Value.Ex.reset(new IRBufferData());
		FromBytes(Out, *Value.Get_ExAs<IRBufferData>());
		break;
	case IRSymbolType::ThreadLocalVarable:
		Value.Ex.reset(new IRBufferData());
		FromBytes(Out, *Value.Get_ExAs<IRBufferData>());
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
			ToBytes(Out, Item.Type);

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
	ToBytes(Out, Value.Type);
}

void IRBuilder::FromBytes(BitReader& Out, IRStaticArray& Value)
{
	BitMaker::SizeAsBits V = 0;
	Out.ReadType(V, V);
	Value.Count = V;
	FromBytes(Out, Value.Type);
}

void IRBuilder::ToBytes(BitMaker& Out, const IRBufferData& Value)
{
	Out.WriteType(Value.Bytes);
	Out.WriteType(Value.IsInitialized);
}

void IRBuilder::FromBytes(BitReader& Out, IRBufferData& Value)
{
	Out.ReadType(Value.Bytes);
	Out.ReadType(Value.IsInitialized);
}

void IRBuilder::ToBytes(BitMaker& Out, const IRFuncPtr& Value)
{
	Out.WriteType((IRCallConvention_t)Value.CallConvention);

	ToBytes(Out, Value.Ret);

	Out.WriteType((BitMaker::SizeAsBits)Value.Pars.size());

	for (auto& Item : Value.Pars)
	{
		ToBytes(Out, Item);
	}
}
void IRBuilder::FromBytes(BitReader& Out, IRFuncPtr& Value)
{
	Out.ReadType(*(IRCallConvention_t*)&Value.CallConvention);
	FromBytes(Out, Value.Ret);

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

	ToBytes(Out, Value._symbol);
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

void IRBuilder::ToBytes(BitMaker& Out, const IRBlock& Value, const Vector<IRPar>& Pars)
{
	Out.WriteType((BitMaker::SizeAsBits)Value.Instructions.size());
	for (auto& Item : Value.Instructions)
	{
		ToBytes(Out, *Item, Value, Pars);
	}
	ToBytes(Out, Value.DebugInfo);
}

void IRBuilder::FromBytes(BitReader& Out, IRBlock& Value, Vector<IRPar>& Pars)
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
		FromBytes(Out, *Item, Value, Pars);
	}
	FromBytes(Out, Value.DebugInfo);
}

void IRBuilder::ToBytes(BitMaker& Out, const IRInstruction& Value, const IRBlock& MyBlock, const Vector<IRPar>& Pars)
{
	Out.WriteType(*(IRInstructionType_t*)&Value.Type);

	ToBytes(Out, Value.ObjectType);

	ToBytes(Out, Value.Target(), Value.ObjectType, MyBlock, Pars);
	ToBytes(Out, Value.Input(), Value.ObjectType, MyBlock, Pars);
}

void IRBuilder::FromBytes(BitReader& Out, IRInstruction& Value, const IRBlock& MyBlock, Vector<IRPar>& Pars)
{
	Out.ReadType(*(IRInstructionType_t*)&Value.Type, *(IRInstructionType_t*)&Value.Type);
	FromBytes(Out, Value.ObjectType);

	FromBytes(Out, Value.Target(), Value.ObjectType, MyBlock, Pars);
	FromBytes(Out, Value.Input(), Value.ObjectType, MyBlock, Pars);
}

void IRBuilder::ToBytes(BitMaker& Out, const IROperator& Value, const IRType& Type, const IRBlock& MyBlock, const Vector<IRPar>& Pars)
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
	else if (Value.Type == IROperatorType::IRInstruction
		|| Value.Type == IROperatorType::DereferenceOf_IRInstruction
		|| Value.Type == IROperatorType::Get_PointerOf_IRInstruction)
	{

		size_t Index = 0;
		for (size_t i = 0; i < MyBlock.Instructions.size(); i++)
		{
			if (MyBlock.Instructions[i].get() == Value.Pointer)
			{
				Index = i;
			}
		}
		Out.WriteType((BitMaker::SizeAsBits)Index);
	}
	else if (Value.Type == IROperatorType::IRParameter
		|| Value.Type == IROperatorType::DereferenceOf_IRParameter
		|| Value.Type == IROperatorType::Get_PointerOf_IRParameter)
	{

		size_t Index = Value.Parameter - Pars.data();
		Out.WriteType((BitMaker::SizeAsBits)Index);
	}
	else if (Value.Type == IROperatorType::Get_Func_Pointer
		|| Value.Type == IROperatorType::Get_PointerOf_IRidentifier
		|| Value.Type == IROperatorType::IRidentifier)
	{
		Out.WriteType(Value.identifier);
	}
}

void IRBuilder::FromBytes(BitReader& Out, IROperator& Value, const IRType& Type, const IRBlock& MyBlock, Vector<IRPar>& Pars)
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
			Out.ReadType(Value.Value.Asfloat32, Value.Value.Asfloat32);
			break;
		case IRTypes::f64:
			Out.ReadType(Value.Value.Asfloat64, Value.Value.Asfloat64);
			break;
		default:
			break;
		}
	}
	else if (Value.Type == IROperatorType::IRInstruction
		|| Value.Type == IROperatorType::DereferenceOf_IRInstruction
		|| Value.Type == IROperatorType::Get_PointerOf_IRInstruction)
	{

		size_t Index = 0;
		BitMaker::SizeAsBits V = 0;
		Out.ReadType(V, V);
		Index = V;

		Value.Pointer = MyBlock.Instructions[Index].get();
	}
	else if (Value.Type == IROperatorType::IRParameter
		|| Value.Type == IROperatorType::DereferenceOf_IRParameter
		|| Value.Type == IROperatorType::Get_PointerOf_IRParameter)
	{

		size_t Index = 0;
		BitMaker::SizeAsBits V = 0;
		Out.ReadType(V, V);
		Index = V;

		Value.Parameter = &Pars[Index];
	}
	else if (Value.Type == IROperatorType::Get_Func_Pointer
		|| Value.Type == IROperatorType::Get_PointerOf_IRidentifier
		|| Value.Type == IROperatorType::IRidentifier)
	{
		Out.ReadType(Value.identifier, Value.identifier);
	}
}

void IRBuilder::ToBytes(BitMaker& Out, const IRDebugSybInfo& Value)
{
	Out.WriteType((BitMaker::SizeAsBits)Value.Symbols.size());
	for (auto& Item : Value.Symbols)
	{
		Out.WriteType(Item.first);

		const auto& Value = Item.second;
		ToBytes(Out, Value);
	}
}

void IRBuilder::ToBytes(UCodeLang::BitMaker& Out, const UCodeLang::IRDebugSymbol& Value)
{
	Out.WriteType(Value.IRVarableName);

	Out.WriteType(Value.VarableName);

	Out.WriteType(Value.LangType);

	Out.WriteType(Value.TypeInfo);

	Out.WriteType((IRDebugSymbol::Type_t)Value._Type);
}
void IRBuilder::FromBytes(BitReader& Out, IRDebugSymbol& OtherValue)
{
	Out.ReadType(OtherValue.IRVarableName);

	Out.ReadType(OtherValue.VarableName);

	Out.ReadType(OtherValue.LangType);

	Out.ReadType(OtherValue.TypeInfo);

	Out.ReadType(*(IRDebugSymbol::Type_t*)&OtherValue._Type);
}

void IRBuilder::FromBytes(BitReader& Out, IRDebugSybInfo& Value)
{
	BitMaker::SizeAsBits Size = 0;
	Out.ReadType(Size, Size);

	Value.Symbols.reserve(Size);
	for (size_t i = 0; i < (size_t)Size; i++)
	{
		IRidentifierID Key = 0;
		Out.ReadType(Key);

		IRDebugSymbol OtherValue;

		FromBytes(Out, OtherValue);

		Value.Symbols.AddValue(std::move(Key), std::move(OtherValue));
	}

}


void IRBuilder::ToBytes(BitMaker& Out, const IRBlockDebugInfo& Value)
{
	Out.WriteType((BitMaker::SizeAsBits)Value.DebugInfo.size());
	for (auto& Item : Value.DebugInfo)
	{
		ToBytes(Out, Item);
	}
}

void IRBuilder::ToBytes(UCodeLang::BitMaker& Out, const UCodeLang::IRDebugIns& Item)
{
	Out.WriteType((IRDebugIns::Variant_t)Item.GetVariantType());

	if (auto Val = Item.Debug.Get_If<IRDebugSetFile>())
	{
		Out.WriteType(Val->FileName);
		Out.WriteType((BitMaker::SizeAsBits)Val->InsInBlock);
	}
	else if (auto Val = Item.Debug.Get_If<IRDebugSetLineNumber>())
	{
		Out.WriteType((BitMaker::SizeAsBits)Val->LineNumber);
		Out.WriteType((BitMaker::SizeAsBits)Val->InsInBlock);
	}
	else if (auto Val = Item.Debug.Get_If<IRDebugSetVarableName>())
	{
		Out.WriteType(Val->IRVarableName);
		Out.WriteType(Val->VarableName);
		Out.WriteType((BitMaker::SizeAsBits)Val->InsInBlock);
	}
	else
	{
		UCodeLangUnreachable();
	}
}

void IRBuilder::FromBytes(BitReader& Out, IRDebugIns& Value)
{
	IRDebugIns::IRVariant Type;
	Out.ReadType(*(IRDebugIns::Variant_t*)&Type);

	switch (Type)
	{
	case IRDebugIns::IRVariant::IRDebugSetFile:
	{
		IRDebugSetFile R = IRDebugSetFile();
		Out.ReadType(R.FileName);

		BitMaker::SizeAsBits Tep;
		Out.ReadType(Tep);
		R.InsInBlock = Tep;

		Value.Debug = std::move(R);
	}
	break;
	case IRDebugIns::IRVariant::IRDebugSetLineNumber:
	{
		IRDebugSetLineNumber R = IRDebugSetLineNumber();
		{
			BitMaker::SizeAsBits Tep;
			Out.ReadType(Tep);
			R.LineNumber = Tep;
		}

		{
			BitMaker::SizeAsBits Tep;
			Out.ReadType(Tep);
			R.InsInBlock = Tep;
		}
		Value.Debug = std::move(R);
	}
	break;
	case IRDebugIns::IRVariant::IRDebugSetVarableName:
	{
		IRDebugSetVarableName R = IRDebugSetVarableName();
		Out.ReadType(R.IRVarableName);
		Out.ReadType(R.VarableName);

		BitReader::SizeAsBits tep = 0;
		Out.ReadType(tep);
		R.InsInBlock = tep;

		Value.Debug = std::move(R);
	}
	break;
	default:
		UCodeLangUnreachable();
		break;
	}
}

void IRBuilder::FromBytes(BitReader& Out, IRBlockDebugInfo& Value)
{
	BitMaker::SizeAsBits SizeValue = 0;
	Out.ReadType(SizeValue);

	Value.DebugInfo.resize(SizeValue);

	for (size_t i = 0; i < Value.DebugInfo.size(); i++)
	{
		FromBytes(Out, Value.DebugInfo[i]);
	}
}


size_t IRBuilder::GetImplementationHash(const IRFunc* Func)
{
	BitMaker V;
	return GetImplementationHash(V, Func);
}

size_t IRBuilder::GetImplementationHash(BitMaker& BitsOutput, const IRFunc* Func)
{
	BitsOutput.clear();
	ToBytes(BitsOutput, *Func);

	String_view Str = String_view((char*)BitsOutput.data(), BitsOutput.size());

	return std::hash<String_view>()(Str);
}

//very slow

size_t IRBuilder::GetImplementationHash(const IRBlock* Func, const Vector<IRPar>& Pars)
{
	BitMaker V;
	return GetImplementationHash(V, Func, Pars);
}

//very slow

size_t IRBuilder::GetImplementationHash(BitMaker BitsOutput, const IRBlock* Func, const Vector<IRPar>& Pars)
{
	BitsOutput.clear();
	ToBytes(BitsOutput, *Func, Pars);

	String_view Str = String_view((char*)BitsOutput.data(), BitsOutput.size());

	return std::hash<String_view>()(Str);
}


//

void IRBuilder::CombineWith(const IRBuilder& Other)
{
	CopyBodyInTo(_StaticInit, Other._StaticInit);
	CopyBodyInTo(_StaticdeInit, Other._StaticdeInit);
	CopyBodyInTo(_threadInit, Other._threadInit);
	CopyBodyInTo(_threaddeInit, Other._threaddeInit);

	{
		for (auto& Item : Other.ConstStaticStrings)
		{
			ConstStaticStrings.AddIfNotHaveKey(Item.first, Item.second);
		}
	}

	{
		for (auto& Item : Other._Map)
		{
			_Map.AddIfNotHaveKey(Item.first, Item.second);
		}
	}

	{

		for (size_t i = 0; i < Other.Funcs.size(); i++)
		{
			const auto Item = Other.Funcs[i].get();

			bool HasThis = GetFunc(Item->identifier);
			if (!HasThis)
			{
				auto& CopyTo = Funcs.emplace_back(new IRFunc());

				CopyTo->identifier = Item->identifier;
				CopyTo->CallConvention = Item->CallConvention;
				CopyTo->Linkage = Item->Linkage;
				CopyTo->ReturnType = Item->ReturnType;
				CopyTo->Pars = Item->Pars;
				CopyBodyInTo(*CopyTo, *Item);
			}
		}
	}

	{
		for (auto& Item : Other._Symbols)
		{
			bool HasThis = GetSymbol(Item->identifier);
			if (!HasThis)
			{
				auto& Syb = _Symbols.emplace_back(new IRSymbolData());
				auto SybPtr = Syb.get();


				SybPtr->identifier = Item->identifier;
				SybPtr->SymType = Item->SymType;
				SybPtr->Type = Item->Type;

				switch (SybPtr->SymType)
				{
				case IRSymbolType::FuncPtr:
				{
					auto Ptr = new IRFuncPtr();
					SybPtr->Ex.reset(Ptr);

					*Ptr = *Item->Get_ExAs<IRFuncPtr>();
				}
				break;
				case IRSymbolType::StaticArray:
				{
					auto Ptr = new IRStaticArray();
					SybPtr->Ex.reset(Ptr);

					*Ptr = *Item->Get_ExAs<IRStaticArray>();
				}
				break;
				case IRSymbolType::Struct:
				{
					auto Ptr = new IRStruct();
					SybPtr->Ex.reset(Ptr);

					*Ptr = *Item->Get_ExAs<IRStruct>();
				}
				break;
				case IRSymbolType::StaticVarable:
				{
					auto Ptr = new IRBufferData();
					SybPtr->Ex.reset(Ptr);

					*Ptr = *Item->Get_ExAs<IRBufferData>();
				}
				break;
				case IRSymbolType::ThreadLocalVarable:
				{
					auto Ptr = new IRBufferData();
					SybPtr->Ex.reset(Ptr);

					*Ptr = *Item->Get_ExAs<IRBufferData>();
				}
				break;
				default:
					UCodeLangUnreachable();
					break;
				}
			}
		}
	}
}

void IRBuilder::CombineWith(IRBuilder&& Other)
{
	//temporary body
	CombineWith(Other);
	//Other.Reset();
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
void IRBuilder::CopyBodyInTo(IRFunc& ToUpdate, const IRFunc& Func)
{
	for (size_t i = 0; i < Func.Blocks.size(); i++)
	{
		ToUpdate.Blocks.push_back({});
		const auto& Item = Func.Blocks[i];
		auto& ToUpdateItem = ToUpdate.Blocks.back();

		auto Ptr = new IRBlock();
		ToUpdateItem.reset(Ptr);


		BitMaker Bits;
		ToBytes(Bits, *Item, Func.Pars);


		BitReader BitReader;
		BitReader.SetBytes(Bits.data(), Bits.size());
		FromBytes(BitReader, *ToUpdateItem, ToUpdate.Pars);
	}
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
		String SybName = FromID(Item->identifier);
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
			UCodeLangUnreachable();
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
		State.SSANames.clear();
		//

		ToString(State, Item.get(), r);
	}

	return r;
}
void IRBuilder::ToString(ToStringState& State, IRFunc* Item, String& r)
{
	State._Func = Item;
	State.PointerToName.clear();

	if (EntryPoint.has_value() && Item->identifier == EntryPoint.value())
	{
		r += "//EntryPoint \n";
	}

	r += "|" + FromID(Item->identifier);
	r += "[";
	for (auto& Par : Item->Pars)
	{
		r += ToString(Par.type)
			+ " "
			+ FromID(Par.identifier);
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

		for (size_t Blocki = 0; Blocki < Item->Blocks.size(); Blocki++)
		{
			auto& Block = Item->Blocks[Blocki];

			r += "\n\n";
			r += Tabs + "[Block:";
			r += std::to_string(Blocki);
			r += "]\n\n";

			Tabs += "  ";

			UnorderedMap<IRidentifierID, String> Names;
			for (size_t i = 0; i < Block->Instructions.size(); i++)
			{
				auto& I = Block->Instructions[i];
				switch (I->Type)
				{
				case IRInstructionType::Jump:
				case IRInstructionType::ConditionalJump:
					if (!Names.HasValue(I->Target().identifier))
					{
						auto LabelName = "_label" + std::to_string(Names.size());
						Names.AddValue(I->Target().identifier, LabelName);


					}
					break;
				}
			}

			for (size_t i = 0; i < Block->Instructions.size(); i++)
			{
				auto& I = Block->Instructions[i];

				auto DebugInfo = Block->DebugInfo.Get_debugfor(i);

				for (auto& Item : DebugInfo)
				{
					if (auto Val = Item->Debug.Get_If<IRDebugSetFile>())
					{
						r += '\n';
						r += "//File:" + Val->FileName;
					}
					else if (auto Val = Item->Debug.Get_If<IRDebugSetLineNumber>())
					{
						r += '\n';
						r += "//Line:" + std::to_string(Val->LineNumber);
					}
					else if (auto Val = Item->Debug.Get_If<IRDebugSetVarableName>())
					{
						r += "\n";
						r += "//Variable:" + Val->VarableName;
					}
				}

				bool name = false;
				for (auto& Item : Names)
				{
					if (Item.first == i)
					{
						r += "\n";
						r += Tabs;
						r += Item.second + ":";
						name = true;
					}
				}

				if (DebugInfo.size() || name)
				{
					r += '\n';
					r += '\n';
				}

				if (I->Type == IRInstructionType::None) { continue; }

				if (I->Type != IRInstructionType::PushParameter
					&& I->Type != IRInstructionType::Member_Access
					&& I->Type != IRInstructionType::Member_Access_Dereference)
				{
					r += Tabs;
				}

				{

					if (!ToString(I.get(), r, State, Names, i, Block.get()))
					{
						continue;
					}
				}
				r += ";";

				r += "\n";


			}


			Tabs.pop_back();
			Tabs.pop_back();
		}
	}
	else
	{
		r += ";\n";
	}
	r += "\n";
}
bool IRBuilder::ToString(
	const IRInstruction* I
	, String& r
	, IRBuilder::ToStringState& State
	, UnorderedMap<IRidentifierID, IRidentifier>& Names
	, const size_t& i
	, const IRBlock* Block)
{
	switch (I->Type)
	{
	case IRInstructionType::MallocCall:
		r += ToString(I->ObjectType);
		r += " " + State.GetName(I);
		r += " = LowLevel::Malloc(" + ToString(State, *I, I->Target()) + ")";
		break;
	case IRInstructionType::FreeCall:
		r += "LowLevel::Free(" + ToString(State, *I, I->Target()) + ")";
		break;
	case IRInstructionType::LoadReturn:
		r += "ret = " + ToString(State, *I, I->Target());
		break;
	case IRInstructionType::LoadNone:
		r += ToString(I->ObjectType);
		r += " " + State.GetName(I);
		break;
	case IRInstructionType::Load:
		r += ToString(I->ObjectType);
		r += " " + State.GetName(I);
		r += " = " + ToString(State, *I, I->Target());
		break;
	case IRInstructionType::Reassign:
		r += ToString(State, *I, I->Target());
		r += " = " + ToString(State, *I, I->Input());
		break;
	case IRInstructionType::Add:
		r += ToStringBinary(State, I, "+");
		break;
	case IRInstructionType::Sub:
		r += ToStringBinary(State, I, "-");
		break;
	case IRInstructionType::UMult:
	case IRInstructionType::SMult:
		r += ToStringBinary(State, I, "*");
		break;
	case IRInstructionType::SDiv:
	case IRInstructionType::UDiv:
		r += ToStringBinary(State, I, "/");
		break;
	case IRInstructionType::EqualTo:
		r += ToStringBinary(State, I, "==");
		break;
	case IRInstructionType::NotEqualTo:
		r += ToStringBinary(State, I, "!=");
		break;

	case IRInstructionType::UGreaterThan:
		r += ToStringBinary(State, I, ">");
		break;
	case IRInstructionType::ULessThan:
		r += ToStringBinary(State, I, "<");
		break;
	case IRInstructionType::UGreaterThanOrEqual:
		r += ToStringBinary(State, I, ">=");
		break;
	case IRInstructionType::ULessThanOrEqual:
		r += ToStringBinary(State, I, "<=");
		break;

	case IRInstructionType::SGreaterThan:
		r += ToStringBinary(State, I, ">");
		break;
	case IRInstructionType::SLessThan:
		r += ToStringBinary(State, I, "<");
		break;
	case IRInstructionType::SGreaterThanOrEqual:
		r += ToStringBinary(State, I, ">=");
		break;
	case IRInstructionType::SLessThanOrEqual:
		r += ToStringBinary(State, I, "<=");
		break;

	case IRInstructionType::Logical_And:
		r += ToStringBinary(State, I, "&&");
		break;
	case IRInstructionType::Logical_Or:
		r += ToStringBinary(State, I, "||");
		break;
	case IRInstructionType::PushParameter:
		State.TepPushedParameters.push_back(I);
		return false;
	case IRInstructionType::Logical_Not:
		r += ToString(I->ObjectType);
		r += " " + State.GetName(I);
		r += " = !" + ToString(State, *I, I->Target());
		break;
	case IRInstructionType::Jump:
		r += "goto ";
		r += Names.GetValue(I->Target().identifier);
		break;
	case IRInstructionType::ConditionalJump:
		r += "gotoif (";
		r += ToString(State, *I, I->Input());
		r += ") ";
		r += Names.GetValue(I->Target().identifier);
		break;
	case IRInstructionType::Call:
	{
		r += ToString(I->ObjectType);
		r += " " + State.GetName(I);
		r += " = ";
		r += FromID(I->Target().identifier) + "(";
		for (auto& Item : State.TepPushedParameters)
		{
			r += ToString(State, *Item, Item->Target());
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
		r += " " + State.GetName(I);
		r += " = ";
		r += ToString(State, *I, I->Target()) + "(";
		for (auto& Item : State.TepPushedParameters)
		{
			r += ToString(State, *Item, Item->Target());
			if (&Item != &State.TepPushedParameters.back())
			{
				r += ",";
			}
		}
		State.TepPushedParameters.clear();
		r += ")";
		break;
	case IRInstructionType::Return:
		r += "ret";
		break;
	case  IRInstructionType::Member_Access:
	{
		State.PointerToName.AddValue(I, ToString(State, *I, I->Target()) + ".__" + std::to_string(I->Input().Value.AsUIntNative));
		return false;
	}
	case  IRInstructionType::Member_Access_Dereference:
	{
		State.PointerToName.AddValue(I, ToString(State, *I, I->Target()) + "->__" + std::to_string(I->Input().Value.AsUIntNative));
		return false;
	}
	case IRInstructionType::UIntToUInt8:
		r += ToString(I->ObjectType);
		r += " " + State.GetName(I);
		r += " = ";
		r += ToString(State, *I, I->Target()) + " -> uint8";
		break;
	case IRInstructionType::UIntToUInt16:
		r += ToString(I->ObjectType);
		r += " " + State.GetName(I);
		r += " = ";
		r += ToString(State, *I, I->Target()) + " -> uint16";
		break;
	case IRInstructionType::UIntToUInt32:
		r += ToString(I->ObjectType);
		r += " " + State.GetName(I);
		r += " = ";
		r += ToString(State, *I, I->Target()) + " -> uint32";
		break;
	case IRInstructionType::UIntToUInt64:
		r += ToString(I->ObjectType);
		r += " " + State.GetName(I);
		r += " = ";
		r += ToString(State, *I, I->Target()) + " -> uint64";
		break;

	case IRInstructionType::SIntToSInt8:
		r += ToString(I->ObjectType);
		r += " " + State.GetName(I);
		r += " = ";
		r += ToString(State, *I, I->Target()) + " -> sint8";
		break;
	case IRInstructionType::SIntToSInt16:
		r += ToString(I->ObjectType);
		r += " " + State.GetName(I);
		r += " = ";
		r += ToString(State, *I, I->Target()) + " -> sint16";
		break;
	case IRInstructionType::SIntToSInt32:
		r += ToString(I->ObjectType);
		r += " " + State.GetName(I);
		r += " = ";
		r += ToString(State, *I, I->Target()) + " -> sint32";
		break;
	case IRInstructionType::SIntToSInt64:
		r += ToString(I->ObjectType);
		r += " " + State.GetName(I);
		r += " = ";
		r += ToString(State, *I, I->Target()) + " -> sint64";
		break;
	case IRInstructionType::New_Await_Task:
		r += ToString(I->ObjectType);
		r += " " + State.GetName(I);
		r += " = ";
		r += "await::new(" + ToString(State, *I, I->Target()) + ")";
		break;
	case IRInstructionType::Await_RunTask:
		r += "await::run(" + ToString(State, *I, I->Target()) + ")";
		break;
	case IRInstructionType::Await_SetComplete:
		r += "await::SetComplete(" + ToString(State, *I, I->Target()) + "," + ToString(State, *I, I->Input()) + ")";
		break;
	case IRInstructionType::Await_SetValue:
		r += ToString(I->ObjectType);
		r += " " + State.GetName(I);
		r += " = ";
		r += "await::SetValue(" + ToString(State, *I, I->Target()) + "," + ToString(State, *I, I->Input()) + ")";
		break;
	case IRInstructionType::Await_GetValue:
		r += ToString(I->ObjectType);
		r += " " + State.GetName(I);
		r += " = ";
		r += "await::GetValue(" + ToString(State, *I, I->Target()) + ")";
		break;
	case IRInstructionType::Unreachable:
		r += "LowLevel::Unreachable()";
		break;
	case IRInstructionType::JumpBlockIf:
		if (Block->Instructions.size() > i + 1 && Block->Instructions[i + 1]->Type == IRInstructionType::JumpBlock)
		{
			r += (String)"  JumpIf (" + ToString(State, *I, I->Input()) + ") : [" + std::to_string(I->Target().Value.AsUIntNative) + "] else ["
				+ std::to_string(Block->Instructions[i + 1]->Target().Value.AsUIntNative) + "]";
		}
		else
		{
			r += (String)"  JumpIf (" + ToString(State, *I, I->Input()) + ") : [" + std::to_string(I->Target().Value.AsUIntNative) + "]";
		}
		break;
	case IRInstructionType::JumpBlock:
		if (i != 0 && Block->Instructions[i - 1]->Type == IRInstructionType::JumpBlockIf)
		{
			return false;
		}
		r += (String)"  Jump : [" + std::to_string(I->Target().Value.AsUIntNative) + "]";
		break;
	case IRInstructionType::Ui32Tof32:
		r += ToString(I->ObjectType);
		r += " " + State.GetName(I);
		r += " = ";
		r += ToString(State, *I, I->Target()) + " -> float32";
		break;
	case IRInstructionType::Ui64Tof64:
		r += ToString(I->ObjectType);
		r += " " + State.GetName(I);
		r += " = ";
		r += ToString(State, *I, I->Target()) + " -> float64";
		break;
	case IRInstructionType::Si32Tof32:
		r += ToString(I->ObjectType);
		r += " " + State.GetName(I);
		r += " = ";
		r += ToString(State, *I, I->Target()) + " -> float32";
		break;
	case IRInstructionType::Si64Tof64:
		r += ToString(I->ObjectType);
		r += " " + State.GetName(I);
		r += " = ";
		r += ToString(State, *I, I->Target()) + " -> float64";
		break;
	case IRInstructionType::f32Toi32:
		r += ToString(I->ObjectType);
		r += " " + State.GetName(I);
		r += " = ";
		r += ToString(State, *I, I->Target()) + " -> int32";
		break;
	case IRInstructionType::f64Toi64:
		r += ToString(I->ObjectType);
		r += " " + State.GetName(I);
		r += " = ";
		r += ToString(State, *I, I->Target()) + " -> int64";
		break;
	case IRInstructionType::f32Tof64:
		r += ToString(I->ObjectType);
		r += " " + State.GetName(I);
		r += " = ";
		r += ToString(State, *I, I->Target()) + " -> float64";
		break;
	case IRInstructionType::f64Tof32:
		r += ToString(I->ObjectType);
		r += " " + State.GetName(I);
		r += " = ";
		r += ToString(State, *I, I->Target()) + " -> float32";
		break;
	case IRInstructionType::BitWise_And:
		r += ToStringBinary(State, I, "&");
		break;
	case IRInstructionType::BitWise_Or:
		r += ToStringBinary(State, I, "|");
		break;
	case IRInstructionType::BitWise_ShiftL:
		r += ToStringBinary(State, I, "<<");
		break;
	case IRInstructionType::BitWise_ShiftR:
		r += ToStringBinary(State, I, ">>");
		break;
	case IRInstructionType::BitWise_XOr:
		r += ToStringBinary(State, I, "^");
		break;
	case IRInstructionType::BitWise_Not:
		r += ToString(I->ObjectType);
		r += " " + State.GetName(I);
		r += " = ~" + ToString(State, *I, I->Target());
		break;
	case IRInstructionType::SMod:
		r += ToStringBinary(State, I, "%");
		break;
	case IRInstructionType::UMod:
		r += ToStringBinary(State, I, "%");
		break;
	case IRInstructionType::SSA_Reassign:
		r += ToString(I->ObjectType);
		if (State.SSANames.HasValue(I->Input()))
		{
			auto& o = State.SSANames.GetValue(I->Input());
			o++;

			State.PointerToName.AddValue(I, ToString(State, *I, I->Input()) + "^" + std::to_string(o));
		}
		else
		{
			State.PointerToName.AddValue(I, ToString(State, *I, I->Input()) + "^2");
			State.SSANames.AddValue(I->Input(), 2);
		}
		r += " " + State.PointerToName.GetValue(I);
		r += " = " + ToString(State, *I, I->Target());
		break;
	case IRInstructionType::ThrowException:
		r += "throw ";
		r += ToString(State, *I, I->A);
		r += ",";
		r += ToString(State, *I, I->B);
		break;
	default:
		UCodeLangUnreachable();
		break;
	}
	return true;
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
	case IRTypes::pointer:
	{
		auto Syb = GetSymbol(Type._symbol);
		if (Syb)
		{
			return FromID(Syb->identifier) + "*";
		}
		return "void*";

	}
	case IRTypes::IRsymbol:
	{
		auto Syb = GetSymbol(Type._symbol);
		if (Syb)
		{
			return FromID(Syb->identifier);
		}

		UCodeLangUnreachable();
	}
	break;
	default:
		UCodeLangUnreachable();
		break;
	}
	return "null";
}
String IRBuilder::ToString(ToStringState& State, const IRInstruction& Ins, const IROperator& Value)
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
		return FromID(Value.identifier);
	}

	case IROperatorType::IRInstruction:
	{
		//for

		return  State.PointerToName.GetValue(Value.Pointer);
	}
	case IROperatorType::IRParameter:
	{
		const IRPar* Par = Value.Parameter;
		return FromID(Par->identifier);
	}
	case IROperatorType::Get_PointerOf_IRInstruction:
	{
		return "&" + State.PointerToName.GetValue(Value.Pointer);
	}
	case IROperatorType::Get_PointerOf_IRParameter:
	{
		const IRPar* Par = Value.Parameter;
		return "&" + FromID(Par->identifier);
	}
	case IROperatorType::Get_PointerOf_IRidentifier:
	{
		return "&" + FromID(Value.identifier);
	}

	case IROperatorType::DereferenceOf_IRInstruction:
	{
		return "*" + State.PointerToName.GetValue(Value.Pointer);
	}
	case IROperatorType::DereferenceOf_IRParameter:
	{
		const IRPar* Par = Value.Parameter;
		return "*" + FromID(Par->identifier);
	}

	case IROperatorType::Get_Func_Pointer:
	{
		return "(&)" + FromID(Value.identifier);
	}
	default:return "[]";
	}
}
String IRBuilder::ToStringBinary(ToStringState& State, const IRInstruction* Ins, const char* Op)
{
	String r;
	r += ToString(Ins->ObjectType);
	r += " " + State.GetName(Ins);
	r += " = " + ToString(State, *Ins, Ins->A) + String(Op) + ToString(State, *Ins, Ins->B);
	return r;
}


void IRBlockDebugInfo::Add_SetFile(const String& file, size_t InsInBlock)
{
	IRDebugIns V;
	IRDebugSetFile F;
	F.FileName = file;
	F.InsInBlock = InsInBlock;

	V.Debug = F;
	DebugInfo.push_back(std::move(V));
}

void IRBlockDebugInfo::Add_SetLineNumber(size_t LineNumber, size_t InsInBlock)
{
	IRDebugIns V;
	IRDebugSetLineNumber F;
	F.LineNumber = LineNumber;
	F.InsInBlock = InsInBlock;

	V.Debug = F;
	DebugInfo.push_back(std::move(V));
}

void IRBlockDebugInfo::Add_SetVarableName(IRDebugSetVarableName&& Info)
{
	IRDebugIns V;
	V.Debug = Info;
	DebugInfo.push_back(std::move(V));
}

Vector<const IRDebugIns*> IRBlockDebugInfo::Get_debugfor(size_t Index) const
{
	Vector<const IRDebugIns*> R;
	for (auto& Item : DebugInfo)
	{
		if (auto Val = Item.Debug.Get_If< IRDebugSetFile>())
		{
			if (Val->InsInBlock == Index)
			{
				R.push_back(&Item);
			}
		}
		else if (auto Val = Item.Debug.Get_If<IRDebugSetLineNumber>())
		{
			if (Val->InsInBlock == Index)
			{
				R.push_back(&Item);
			}
		}
		else if (auto Val = Item.Debug.Get_If<IRDebugSetVarableName>())
		{
			if (Val->InsInBlock == Index)
			{
				R.push_back(&Item);
			}
		}
	}

	return R;
}

Vector<const IRDebugIns*> IRBlock::Get_debugfor(const IRInstruction* Value) const
{
	for (size_t i = 0; i < Instructions.size(); i++)
	{
		auto& Item = Instructions[i];
		if (Item.get() == Value)
		{
			return DebugInfo.Get_debugfor(i);
		}
	}
	return {};
}

UCodeLangEnd

#endif