#include "UClib.hpp"
#include <fstream>
#include "UCodeLang/LangCore/Version.hpp"
#include <bitset>
UCodeLangStart

UClib::UClib() : LibEndianess(BitConverter::InputOutEndian)
{
}

UClib::~UClib()
{

}
void UClib::Reset()
{
	this->~UClib();
	new (this) UClib;
}
using Size_tAsBits = BitMaker::SizeAsBits;

#if UCodeLangDebug
const unsigned char UClibSignature[] = "LBlib32";
#else
const unsigned char UClibSignature[] = "LBlib";
#endif

constexpr size_t UClibSignature_Size = sizeof(UClibSignature);


BytesPtr UClib::ToRawBytes(const UClib* Lib)
{
	BitMaker Output;



	//Ulib signature
	{
		Output.WriteType((Size_tAsBits)UClibSignature_Size);
		Output.WriteBytes(UClibSignature, UClibSignature_Size);
		Output.WriteType((UInt32)UCodeLangVersionNumber);
	}

	Output.WriteType((NTypeSize_t)Lib->BitSize);

	Output.WriteType((LibType_t)Lib->_LibType);

	Output.WriteType((Endian_t)Lib->LibEndianess);

	{//StaticBytes
		Output.WriteType((Size_tAsBits)Lib->_StaticBytes.size());
		Output.WriteBytes(Lib->_StaticBytes.data(), Lib->_StaticBytes.size());
	}

	{//ThreadBytes
		Output.WriteType((Size_tAsBits)Lib->_ThreadBytes.size());
		Output.WriteBytes(Lib->_ThreadBytes.data(), Lib->_ThreadBytes.size());
	}

	{//DebugBytes
		Output.WriteType((Size_tAsBits)Lib->_DebugBytes.size());
		Output.WriteBytes(Lib->_DebugBytes.data(), Lib->_DebugBytes.size());
	}

	{//Layers
		Output.WriteType((Size_tAsBits)Lib->_Layers.size());
		for (auto& Item : Lib->_Layers)
		{
			ToBytes(Output, *Item);
		}
	}

	//ClassAssembly
	{
		auto& Assembly = Lib->_Assembly;
		ToBytes(Output, Assembly);
	}

	return  Output.AsBytePtrAndMove();
}
void UClib::ToBytes(BitMaker& Output, const ClassAssembly& Assembly)
{
	Output.WriteType((Size_tAsBits)Assembly.Classes.size());


	for (auto& Item : Assembly.Classes)
	{
		Output.WriteType(Item->Name);
		Output.WriteType(Item->FullName);

		Output.WriteType((ClassType_t)Item->Get_Type());

		switch (Item->Get_Type())
		{
		case ClassType::Alias:
		{
			auto& Alias = Item->Get_AliasData();
			ToBytes(Output, Alias);
		}
		break;
		case ClassType::Class:
		{
			auto& ClassData = Item->Get_ClassData();
			ToBytes(Output, ClassData);
		}
		break;
		case ClassType::Enum:
		{
			auto& EnumData = Item->Get_EnumData();

			ToBytes(Output, EnumData);
		}
		break;
		case ClassType::Trait:
		{
			auto& TraitData = Item->Get_TraitData();

			ToBytes(Output, TraitData);
		}
		break;
		case ClassType::FuncPtr:
		{
			auto& FuncPtrData = Item->Get_FuncPtr();

			ToBytes(Output, FuncPtrData);
		}
		break;
		case ClassType::Tag:
		{
			auto& TagData = Item->Get_TagData();

			ToBytes(Output, TagData);
		}
		break;
		case ClassType::GenericClass:
		{
			auto& TagData = Item->Get_GenericClass();

			ToBytes(Output, TagData);
		}
		break;
		case ClassType::GenericFunction:
		{
			auto& TagData = Item->Get_GenericFunctionData();

			ToBytes(Output, TagData);
		}
		break;
		case ClassType::StaticArray:
		{
			auto& TagData = Item->Get_StaticArray();

			ToBytes(Output, TagData);
		}
		break;
		case ClassType::NameSpace:
		{
			auto& TagData = Item->Get_NameSpace();

			ToBytes(Output, TagData);
		}
		break;
		case ClassType::ForType:
		{
			auto& TagData = Item->Get_ForType();

			ToBytes(Output, TagData);
		}
		break;
		case ClassType::Eval:
		{
			auto& TagData = Item->Get_EvalData();

			ToBytes(Output, TagData);
		}
		break;
		default:
			UCodeLangUnreachable();
			break;
		}
	}
}
void UClib::ToBytes(BitMaker& Output, const CodeLayer& Data)
{
	Output.WriteType(Data._Name);

	Output.WriteType((CodeLayer::DataTypes_t)Data.GetDataType());

	if (auto Val = Data._Data.Get_If<CodeLayer::JustData>())
	{
		Output.WriteType(Val->_Data);
	}
	else if (auto Val = Data._Data.Get_If<CodeLayer::UCodeByteCode>())
	{
		Output.WriteType((Size_tAsBits)Val->_Instructions.size());
		Output.WriteBytes((const Byte*)Val->_Instructions.data(), Val->_Instructions.size() * sizeof(Instruction));

		{// _NameToPtr

			Output.WriteType((Size_tAsBits)Val->_NameToPtr.size());
			for (auto& Item : Val->_NameToPtr)
			{
				Output.WriteType(Item.first);
				Output.WriteType((Size_tAsBits)Item.second);
			}
		}

		Output.WriteType(Val->DebugInfo.has_value());

		if (Val->DebugInfo.has_value())
		{
			ULangDebugInfo::ToBytes(Output, Val->DebugInfo.value());
		}
	}
	else if (auto Val = Data._Data.Get_If<CodeLayer::MachineCode>())
	{
		Output.WriteType(Val->_Code);

		{// _NameToPtr

			Output.WriteType((Size_tAsBits)Val->_NameToPtr.size());
			for (auto& Item : Val->_NameToPtr)
			{
				Output.WriteType(Item.first);
				Output.WriteType((Size_tAsBits)Item.second);
			}
		}

		Output.WriteType(Val->DebugInfo);
	}

}
void UClib::ToBytes(BitMaker& Output, const ReflectionRawData& Data)
{
	Output.WriteType((BitMaker::SizeAsBits)Data.Size);
	Output.WriteBytes((const Byte*)Data.Get_Data(), Data.Size);
}
void UClib::ToBytes(BitMaker& Output, const TypedRawReflectionData& Data)
{
	ToBytes(Output, Data._Type);
	ToBytes(Output, Data._Data);
}
void UClib::ToBytes(BitMaker& Output, const Optional<ReflectionCustomTypeID>& Data)
{
	Output.WriteType(Data.has_value());
	if (Data.has_value())
	{
		Output.WriteType(Data.value());
	}
}
void UClib::ToBytes(BitMaker& Output, const Optional<ReflectionTypeInfo>& Data)
{
	Output.WriteType(Data.has_value());
	if (Data.has_value())
	{
		ToBytes(Output, Data.value());
	}
}
void UClib::ToBytes(BitMaker& Output, const Trait_Data& TraitData)
{
	Output.WriteType(TraitData.TypeID);
	Output.WriteType((AccessModifierType_t)TraitData.AccessModifier);
	Output.WriteType(TraitData.IsExported);

	Output.WriteType((Size_tAsBits)TraitData.Fields.size());
	for (auto& Item2 : TraitData.Fields)
	{
		ToBytes(Output, Item2);
	}

	Output.WriteType((Size_tAsBits)TraitData.Methods.size());
	for (auto& Item2 : TraitData.Methods)
	{
		ToBytes(Output, Item2);
	}

	Output.WriteType((Size_tAsBits)TraitData.Symbols.size());
	for (auto& Item2 : TraitData.Symbols)
	{
		ToBytes(Output, Item2);
	}

	Output.WriteType((Size_tAsBits)TraitData.GenericAlias.size());
	for (auto& Item2 : TraitData.GenericAlias)
	{
		ToBytes(Output, Item2);
	}
}
void UClib::ToBytes(BitMaker& Output, const TraitSymbol& TraitData)
{
	Output.WriteType(TraitData.Implementation);
	Output.WriteType((AccessModifierType_t)TraitData.AccessModifier);
	Output.WriteType(TraitData.IsExported);
}
void UClib::ToBytes(BitMaker& Output, const TraitAlias& TraitData)
{
	Output.WriteType(TraitData.AliasName);
	ToBytes(Output,TraitData.Type);

	Output.WriteType(TraitData.Expression.has_value());
	if (TraitData.Expression.has_value())
	{
		ToBytes(Output, TraitData.Expression.value());
	}

	Output.WriteType(TraitData.TypePack.has_value());
	if (TraitData.TypePack.has_value())
	{
		auto& pack = TraitData.TypePack.value();
		Output.WriteType((BitMaker::SizeAsBits)pack.size());

		for (auto& Item : pack)
		{
			ToBytes(Output,Item);
		}
	}
}
void UClib::ToBytes(BitMaker& Output, const InheritedTrait_Data& TraitData)
{
	Output.WriteType(TraitData.TraitID);
}
void UClib::ToBytes(BitMaker& Output, const Enum_Data& EnumData)
{
	Output.WriteType(EnumData.TypeID);
	ToBytes(Output, EnumData.BaseType);
	ToBytes(Output, EnumData.EnumVariantUnion);

	Output.WriteType((Size_tAsBits)EnumData.Values.size());

	for (auto& Item2 : EnumData.Values)
	{
		Output.WriteType(Item2.Name);
		ToBytes(Output, Item2._Data);
		ToBytes(Output, Item2.EnumVariantType);
	}

	Output.WriteType(EnumData.DestructorFuncFullName.has_value());

	if (EnumData.DestructorFuncFullName.has_value())
	{
		Output.WriteType(EnumData.DestructorFuncFullName.value());
	}

	Output.WriteType(EnumData.MoveFuncFullName.has_value());

	if (EnumData.MoveFuncFullName.has_value())
	{
		Output.WriteType(EnumData.MoveFuncFullName.value());
	}

	Output.WriteType(EnumData.CopyFuncFullName.has_value());

	if (EnumData.CopyFuncFullName.has_value())
	{
		Output.WriteType(EnumData.CopyFuncFullName.value());
	}
	Output.WriteType((AccessModifierType_t)EnumData.AccessModifier);
	Output.WriteType(EnumData.IsExported);
}
void UClib::ToBytes(BitMaker& Output, const Alias_Data& Alias)
{
	Output.WriteType(Alias.HardAliasTypeID.has_value());
	if (Alias.HardAliasTypeID.has_value())
	{
		Output.WriteType(Alias.HardAliasTypeID.value());
	}
	ToBytes(Output, Alias.Type);
	Output.WriteType((AccessModifierType_t)Alias.AccessModifier);
	Output.WriteType(Alias.IsExported);
}
void UClib::ToBytes(BitMaker& Output, const Class_Data& ClassData)
{
	Output.WriteType(ClassData.TypeID);
	Output.WriteType((Size_tAsBits)ClassData.Size);
	Output.WriteType((AccessModifierType_t)ClassData.AccessModifier);
	Output.WriteType(ClassData.IsExported);

	ToBytes(Output, ClassData.Attributes);

	Output.WriteType((Size_tAsBits)ClassData.Fields.size());
	for (auto& Item2 : ClassData.Fields)
	{
		ToBytes(Output, Item2);
	}

	Output.WriteType((Size_tAsBits)ClassData.Methods.size());
	for (auto& Item2 : ClassData.Methods)
	{
		ToBytes(Output, Item2);
	}

	Output.WriteType((Size_tAsBits)ClassData.InheritedTypes.size());
	for (auto& Item2 : ClassData.InheritedTypes)
	{
		ToBytes(Output, Item2);
	}

	Output.WriteType((Size_tAsBits)ClassData.GenericAlias.size());
	for (auto& Item2 : ClassData.GenericAlias)
	{
		ToBytes(Output, Item2);
	}
}
void UClib::ToBytes(UCodeLang::BitMaker& Output, const Vector<UsedTagValueData>& Attributes)
{
	Output.WriteType((Size_tAsBits)Attributes.size());
	for (auto& Item2 : Attributes)
	{
		ToBytes(Output, Item2);
	}
}
void UClib::ToBytes(BitMaker& Output, const ClassField& Item2)
{
	Output.WriteType(Item2.Name);
	ToBytes(Output, Item2.Type);
	Output.WriteType((Size_tAsBits)Item2.offset);
	Output.WriteType((AccessModifierType_t)Item2.Protection);
}
void UClib::ToBytes(BitMaker& Output, const Tag_Data& Data)
{
	Output.WriteType(Data.TypeID);
	Output.WriteType((AccessModifierType_t)Data.AccessModifier);
	Output.WriteType(Data.IsExported);

	Output.WriteType((Size_tAsBits)Data.Fields.size());
	for (auto& Item2 : Data.Fields)
	{
		ToBytes(Output, Item2);
	}

	Output.WriteType((Size_tAsBits)Data.Methods.size());
	for (auto& Item2 : Data.Methods)
	{
		ToBytes(Output, Item2);
	}
}
void UClib::ToBytes(BitMaker& Output, const UsedTagValueData& Data)
{
	Output.WriteType(Data.TypeID);
	ToBytes(Output, Data._Data);
}
void UClib::ToBytes(BitMaker& Output, const ClassMethod& Data)
{
	Output.WriteType(Data.FullName);
	Output.WriteType(Data.DecorationName);

	ToBytes(Output, Data.RetType);
	Output.WriteType((Size_tAsBits)Data.ParsType.size());
	for (auto& Item : Data.ParsType)
	{
		ToBytes(Output, Item);
	}
	Output.WriteType(Data.IsThisFunction);
	Output.WriteType(Data.IsUnsafe);
	Output.WriteType(Data.IsExternC);
	Output.WriteType(Data.IsRemoved);
	Output.WriteType(Data.IsExport);
	Output.WriteType(Data.IsTraitDynamicDispatch);
	Output.WriteType((AccessModifierType_t)Data.Protection);

	ToBytes(Output, Data.Attributes);
}

enum class ReflectionTypeInfoBools
{
	IsAddress,
	IsAddressArray,
	Isimmutable,
	IsDynamic,
	IsMove,
	Max,
};
size_t AsIndex(ReflectionTypeInfoBools index)
{
	return (size_t)index;
}
bool NextIsCusomTypeID(ReflectionTypes type)
{
	return type == ReflectionTypes::CustomType;
}
void UClib::ToBytes(BitMaker& Output, const ReflectionTypeInfo& Data)
{
	//We should combined _Type and _Type attribute into one byte
	Output.WriteType((ReflectionTypes_t)Data._Type);

	std::bitset<(size_t)ReflectionTypeInfoBools::Max> bits;
	bits[(size_t)ReflectionTypeInfoBools::IsAddress] = Data.IsAddress();
	bits[(size_t)ReflectionTypeInfoBools::IsAddressArray] = Data.IsAddressArray();
	bits[(size_t)ReflectionTypeInfoBools::Isimmutable] = Data.Isimmutable();
	bits[(size_t)ReflectionTypeInfoBools::IsDynamic] = Data.IsDynamicTrait();
	bits[(size_t)ReflectionTypeInfoBools::IsMove] = Data.IsMovedType();

	Output.WriteType(bits);

	if (NextIsCusomTypeID(Data._Type))
	{
		Output.WriteType(Data._CustomTypeID);
	}

}
void UClib::ToBytes(BitMaker& Output, const ClassMethod::Par& Par)
{
	Output.WriteType(Par.IsOutPar);
	ToBytes(Output, Par.Type);
}
void UClib::ToBytes(BitMaker& Output, const FuncPtr_Data& FuncPtrData)
{
	Output.WriteType(FuncPtrData.TypeID);
	ToBytes(Output, FuncPtrData.RetType);
	Output.WriteType((BitMaker::SizeAsBits)FuncPtrData.ParsType.size());
	for (auto& Item : FuncPtrData.ParsType)
	{
		ToBytes(Output, Item);
	}
	Output.WriteType((AccessModifierType_t)FuncPtrData.AccessModifier);
	Output.WriteType(FuncPtrData.IsExported);
}
void UClib::ToBytes(BitMaker& Output, const GenericClass_Data& FuncPtrData)
{
	ToBytes(Output, FuncPtrData.Base);
	Output.WriteType((AccessModifierType_t)FuncPtrData.AccessModifier);
	Output.WriteType(FuncPtrData.IsExported);
}
void UClib::ToBytes(BitMaker& Output, const GenericFunction_Data& FuncPtrData)
{
	ToBytes(Output, FuncPtrData.Base);
	Output.WriteType((AccessModifierType_t)FuncPtrData.AccessModifier);
	Output.WriteType(FuncPtrData.IsExported);
}
void UClib::ToBytes(BitMaker& Output, const StaticArray_Data& FuncPtrData)
{
	Output.WriteType(FuncPtrData.TypeID);
	ToBytes(Output, FuncPtrData.BaseType);
	Output.WriteType((BitMaker::SizeAsBits)FuncPtrData.Count);
}
void UClib::ToBytes(BitMaker& Output, const NameSpace_Data& FuncPtrData)
{

}
void UClib::ToBytes(BitMaker& Output, const ForType_Data& FuncPtrData)
{
	ToBytes(Output,FuncPtrData._TargetType);	
	Output.WriteType(FuncPtrData._Scope);
	Output.WriteType((AccessModifierType_t)FuncPtrData.AccessModifier);
	Output.WriteType(FuncPtrData.IsExported);

	Output.WriteType((BitMaker::SizeAsBits)FuncPtrData._AddedMethods.size());
	for (auto& Item : FuncPtrData._AddedMethods)
	{
		ToBytes(Output, Item);
	}
}
void UClib::ToBytes(BitMaker& Output, const Eval_Data& FuncPtrData)
{
	ToBytes(Output, FuncPtrData.Value);
	Output.WriteType((AccessModifierType_t)FuncPtrData.AccessModifier);
	Output.WriteType(FuncPtrData.IsExported);
}
void UClib::ToBytes(BitMaker& Output, const GenericBase_Data& FuncPtrData)
{
	Output.WriteType(FuncPtrData.Implementation);
}
void UClib::ToBytes(BitMaker& Output, const TraitMethod& FuncPtrData)
{
	ToBytes(Output, FuncPtrData.method);

	Output.WriteType(FuncPtrData.FuncBody.has_value());

	if (FuncPtrData.FuncBody.has_value())
	{
		Output.WriteType(FuncPtrData.FuncBody.value());
	}
}
bool UClib::FromBytes(UClib* Lib, const BytesView& Data)
{
	Lib->Reset();

	BitReader reader(Data.Data(), Data.Size());


	//Signature
	{

		Size_tAsBits bits_Size_tAsBits = 0;
		size_t bits;


		reader.ReadType(bits_Size_tAsBits, bits_Size_tAsBits);

		bits = bits_Size_tAsBits;

		if (bits != UClibSignature_Size) { return false; }

		for (size_t i = 0; i < UClibSignature_Size; i++)
		{
			char Bit = reader.GetByteWith_offset(i);
			if (Bit != UClibSignature[i]) { return false; }
		}
		reader.Increment_offset(UClibSignature_Size);


		UInt32 Value = 0;
		reader.ReadType(Value, Value);
		if (Value != UCodeLangVersionNumber)
		{
			return false;
		}

	}

	reader.ReadType(*(NTypeSize_t*)&Lib->BitSize, *(NTypeSize_t*)&Lib->BitSize);

	reader.ReadType(*(LibType_t*)&Lib->_LibType, *(NTypeSize_t*)&Lib->_LibType);

	reader.ReadType(*(Endian_t*)&Lib->LibEndianess, *(Endian_t*)&Lib->LibEndianess);

	auto Old = BitConverter::InputOutEndian;
	BitConverter::InputOutEndian = Lib->LibEndianess;

	{//StaticBytes

		Size_tAsBits bits = 0;
		size_t bits_Size;


		reader.ReadType(bits, bits);
		bits_Size = bits;

		Lib->_StaticBytes.resize(bits_Size);


		memcpy(Lib->_StaticBytes.data(), &reader.GetByteWith_offset(0), bits_Size);

		reader.Increment_offset(bits_Size);
	}

	{//ThreadBytes
		Size_tAsBits bits = 0;
		size_t bits_Size;


		reader.ReadType(bits, bits);
		bits_Size = bits;

		Lib->_ThreadBytes.resize(bits_Size);


		memcpy(Lib->_ThreadBytes.data(), &reader.GetByteWith_offset(0), bits_Size);

		reader.Increment_offset(bits_Size);
	}

	{//DebugBytes

		Size_tAsBits bits = 0;
		size_t bits_Size;


		reader.ReadType(bits, bits);
		bits_Size = bits;

		Lib->_DebugBytes.resize(bits_Size);



		memcpy(Lib->_DebugBytes.data(), &reader.GetByteWith_offset(0), bits_Size);

		reader.Increment_offset(bits_Size);
	}




	{//Layers

		Size_tAsBits bits = 0;
		size_t bits_Size;


		reader.ReadType(bits, bits);
		bits_Size = bits;


		Lib->_Layers.resize(bits_Size);

		for (size_t i = 0; i < bits_Size; i++)
		{
			CodeLayer* Layer = new CodeLayer();

			FromBytes(reader, *Layer);

			Lib->_Layers[i] = Unique_ptr<CodeLayer>(Layer);
		}
	}


	//ClassAssembly
	{
		auto& Assembly = Lib->Get_Assembly();
		FromBytes(reader, Assembly);

		FixAssemblyRawValues(Lib->LibEndianess, Lib->BitSize, Assembly);
		Lib->LibEndianess = BitConverter::_CPUEndian;
	}

	BitConverter::InputOutEndian = Old;
	return true;
}
void UClib::FromBytes(BitReader& Input, CodeLayer& Data)
{
	Input.ReadType(Data._Name);

	CodeLayer::DataTypes _type = CodeLayer::DataTypes::JustData;
	Input.ReadType(*(CodeLayer::DataTypes_t*)&_type, *(CodeLayer::DataTypes_t*)&_type);

	switch (_type)
	{
	case CodeLayer::DataTypes::JustData:
	{
		CodeLayer::JustData V;
		Input.ReadType(V._Data, V._Data);
		Data._Data = std::move(V);
	}
	break;
	case CodeLayer::DataTypes::UCodeByteCode:
	{
		CodeLayer::UCodeByteCode V;
		{// Instructions

			Size_tAsBits bits = 0;
			size_t bits_Size;


			Input.ReadType(bits, bits);
			bits_Size = bits;

			V._Instructions.resize(bits_Size);


			memcpy(V._Instructions.data(), &Input.GetByteWith_offset(0), bits_Size * sizeof(Instruction));

			Input.Increment_offset(bits_Size * sizeof(Instruction));
		}

		bool WillNeedtoSwapBytes = BitConverter::_CPUEndian != BitConverter::InputOutEndian;

		if (WillNeedtoSwapBytes)
		{
			for (auto& Item : V._Instructions)
			{
				auto Optype = Instruction::GetOpType(Item.OpCode);

				switch (Optype)
				{
				case UCodeLang::Instruction::OpType::NoneOp:
					break;
				case UCodeLang::Instruction::OpType::ThreeUInt8:
					break;
				case UCodeLang::Instruction::OpType::OneReg:
					break;
				case UCodeLang::Instruction::OpType::TwoReg:
					break;
				case UCodeLang::Instruction::OpType::ThreeReg:
					break;
				case UCodeLang::Instruction::OpType::RegUInt8:
					break;
				case UCodeLang::Instruction::OpType::RegUInt16:
				{
					auto& Op = Item.Op_RegUInt16;
					auto Copy = Op.B;
					((Byte*)&Op.B)[0] = ((Byte*)&Copy)[1];
					((Byte*)&Op.B)[1] = ((Byte*)&Copy)[0];
				}
				break;
				case UCodeLang::Instruction::OpType::ValUInt8:
					break;
				case UCodeLang::Instruction::OpType::ValUInt16:
				{
					auto& Op = Item.Op_ValUInt16;
					auto Copy = Op.A;
					((Byte*)&Op.A)[0] = ((Byte*)&Copy)[1];
					((Byte*)&Op.A)[1] = ((Byte*)&Copy)[0];
				}
				break;
				case UCodeLang::Instruction::OpType::TwoRegInt8:
					break;
				default:
					UCodeLangUnreachable();
					break;
				}
			}
		}

		{// _NameToPtr

			Size_tAsBits bits = 0;
			size_t bits_Size;


			Input.ReadType(bits, bits);
			bits_Size = bits;

			V._NameToPtr.clear();
			V._NameToPtr.reserve(bits_Size);

			for (size_t i = 0; i < bits_Size; i++)
			{
				String V1;

				Size_tAsBits V2 = 0;
				size_t V2bits_Size;

				Input.ReadType(V1, V1);

				Input.ReadType(V2, V2);
				V2bits_Size = V2;

				V._NameToPtr.AddValue(V1, V2);
			}

			bool HasDebugInfo = false;
			Input.ReadType(HasDebugInfo, HasDebugInfo);

			if (HasDebugInfo)
			{
				ULangDebugInfo V2;
				ULangDebugInfo::FromBytes(Input, V2);

				V.DebugInfo = std::move(V2);
			}
			Data._Data = std::move(V);
		}


	}
	break;
	case CodeLayer::DataTypes::MachineCode:
	{
		CodeLayer::MachineCode V;
		Input.ReadType(V._Code, V._Code);

		{// _NameToPtr

			Size_tAsBits bits = 0;
			size_t bits_Size;


			Input.ReadType(bits, bits);
			bits_Size = bits;

			V._NameToPtr.clear();
			V._NameToPtr.reserve(bits_Size);

			for (size_t i = 0; i < bits_Size; i++)
			{
				String V1;

				Size_tAsBits V2 = 0;
				size_t V2bits_Size;

				Input.ReadType(V1, V1);

				Input.ReadType(V2, V2);
				V2bits_Size = V2;

				V._NameToPtr.AddValue(V1, V2);
			}

			Input.ReadType(V.DebugInfo, V.DebugInfo);
		}

		Data._Data = std::move(V);
	}
	break;
	default:
		UCodeLangThrowException("bad path");
		break;
	}
}
void UClib::FromBytes(BitReader& reader, ClassAssembly& Assembly)
{
	Size_tAsBits bits = 0;
	size_t bits_Size;

	reader.ReadType(bits, bits);
	bits_Size = bits;

	for (size_t i = 0; i < bits_Size; i++)
	{
		String TepName;
		String TepFullName;
		ClassType TepType;


		reader.ReadType(TepName, TepName);
		reader.ReadType(TepFullName, TepFullName);
		reader.ReadType(*(ClassType_t*)&TepType, *(ClassType_t*)&TepType);

		AssemblyNode _Node = AssemblyNode(TepType);
		_Node.Name = std::move(TepName);
		_Node.FullName = std::move(TepFullName);

		switch (TepType)
		{
		case ClassType::Alias:
		{
			auto& Alias = _Node.Get_AliasData();
			FromBytes(reader, Alias);
		}
		break;
		case ClassType::Class:
		{
			auto& Class = _Node.Get_ClassData();
			FromBytes(reader, Class);
		}
		break;
		case ClassType::Enum:
		{
			auto& Enum = _Node.Get_EnumData();
			FromBytes(reader, Enum);
		}
		break;
		case ClassType::Trait:
		{
			auto& Trait = _Node.Get_TraitData();
			FromBytes(reader, Trait);
		}
		break;
		case ClassType::FuncPtr:
		{
			auto& FuncPtr = _Node.Get_FuncPtr();
			FromBytes(reader, FuncPtr);
		}
		break;
		case ClassType::Tag:
		{
			auto& Tag = _Node.Get_TagData();
			FromBytes(reader, Tag);
		}
		break;
		case ClassType::GenericClass:
		{
			auto& Tag = _Node.Get_GenericClass();
			FromBytes(reader, Tag);
		}
		break;
		case ClassType::GenericFunction:
		{
			auto& Tag = _Node.Get_GenericFunctionData();
			FromBytes(reader, Tag);
		}
		break;
		case ClassType::StaticArray:
		{
			auto& Tag = _Node.Get_StaticArray();
			FromBytes(reader, Tag);
		}
		break;
		case ClassType::NameSpace:
		{
			auto& Tag = _Node.Get_NameSpace();
			FromBytes(reader, Tag);
		}
		break;
		case ClassType::ForType:
		{
			auto& Tag = _Node.Get_ForType();
			FromBytes(reader, Tag);
		}
		break;
		case ClassType::Eval:
		{
			auto& Tag = _Node.Get_EvalData();
			FromBytes(reader, Tag);
		}
		break;
		default:
			UCodeLangUnreachable();
			break;
		}
		Assembly.Classes.push_back(std::make_unique<AssemblyNode>(std::move(_Node)));
	}

}
void UClib::FromBytes(BitReader& Input, Optional<ReflectionCustomTypeID>& Data)
{
	bool HasValue = false;
	Input.ReadType(HasValue);
	if (HasValue)
	{
		ReflectionCustomTypeID Value = {};
		Input.ReadType(Value);
		Data = Value;
	}
}
void UClib::FromBytes(BitReader& reader, Enum_Data& Enum)
{
	reader.ReadType(Enum.TypeID, Enum.TypeID);
	FromBytes(reader, Enum.BaseType);
	FromBytes(reader, Enum.EnumVariantUnion);


	Size_tAsBits  Sizebits = 0;
	size_t Size;


	reader.ReadType(Sizebits, Sizebits);
	Size = Sizebits;

	Enum.Values.resize(Size);
	for (size_t i2 = 0; i2 < Size; i2++)
	{
		auto& Item2 = Enum.Values[i2];
		reader.ReadType(Item2.Name, Item2.Name);
		FromBytes(reader, Item2._Data);
		FromBytes(reader, Item2.EnumVariantType);
	}

	bool has = false;

	reader.ReadType(has, has);
	if (has)
	{
		String tep;
		reader.ReadType(tep);
		Enum.DestructorFuncFullName = tep;
	}

	reader.ReadType(has, has);
	if (has)
	{
		String tep;
		reader.ReadType(tep);
		Enum.MoveFuncFullName = tep;
	}

	reader.ReadType(has, has);
	if (has)
	{
		String tep;
		reader.ReadType(tep);
		Enum.CopyFuncFullName = tep;
	}	
	reader.ReadType(*(AccessModifierType_t*)&Enum.AccessModifier,*(AccessModifierType_t*)&Enum.AccessModifier);
	reader.ReadType(Enum.IsExported, Enum.IsExported);
}
void UClib::FromBytes(BitReader& Input, Optional<ReflectionTypeInfo>& Data)
{
	bool HasValue = false;
	Input.ReadType(HasValue);
	if (HasValue)
	{
		ReflectionTypeInfo Val;
		FromBytes(Input, Val);
		Data = std::move(Val);
	}
}
void UClib::FromBytes(BitReader& reader, Class_Data& Class)
{
	reader.ReadType(Class.TypeID, Class.TypeID);

	Size_tAsBits _Classbits = 0;
	reader.ReadType(_Classbits, _Classbits);
	Class.Size = _Classbits;
	
	reader.ReadType(*(AccessModifierType_t*)&Class.AccessModifier,*(AccessModifierType_t*)&Class.AccessModifier);
	reader.ReadType(Class.IsExported, Class.IsExported);
	

	FromBytes(reader, Class.Attributes);

	{

		Size_tAsBits  Feld_Sizebits = 0;
		size_t Feld_Size;

		reader.ReadType(Feld_Sizebits, Feld_Sizebits);
		Feld_Size = Feld_Sizebits;

		Class.Fields.resize(Feld_Size);
		for (size_t i2 = 0; i2 < Feld_Size; i2++)
		{
			auto& Item2 = Class.Fields[i2];
			FromBytes(reader, Item2);
		}
	}

	{

		Size_tAsBits  Methods_Sizebits = 0;
		size_t Methods_Size;

		reader.ReadType(Methods_Sizebits, Methods_Sizebits);
		Methods_Size = Methods_Sizebits;

		Class.Methods.resize(Methods_Size);
		for (size_t i2 = 0; i2 < Methods_Size; i2++)
		{
			auto& Item2 = Class.Methods[i2];
			FromBytes(reader, Item2);
		}
	}

	{

		Size_tAsBits Sizebits = 0;
		size_t Size;

		reader.ReadType(Sizebits, Sizebits);
		Size = Sizebits;

		Class.InheritedTypes.resize(Size);
		for (size_t i2 = 0; i2 < Size; i2++)
		{
			auto& Item2 = Class.InheritedTypes[i2];
			FromBytes(reader, Item2);
		}
	}

	{
		Size_tAsBits Sizebits = 0;
		size_t Size;

		reader.ReadType(Sizebits, Sizebits);
		Size = Sizebits;

		Class.GenericAlias.resize(Size);
		for (size_t i2 = 0; i2 < Size; i2++)
		{
			auto& Item2 = Class.GenericAlias[i2];
			FromBytes(reader, Item2);
		}
	}
}
void UClib::FromBytes(BitReader& Input, ReflectionRawData& Data)
{
	BitReader::SizeAsBits BufferSize = 0;
	Input.ReadType(BufferSize);

	Data.Resize(BufferSize);
	memcpy(Data.Get_Data(), &Input.GetByteWith_offset(0), BufferSize);
	Input.Increment_offset(BufferSize);
}
void UClib::FromBytes(BitReader& Input, Trait_Data& Data)
{
	Input.ReadType(Data.TypeID, Data.TypeID);
	Input.ReadType(*(AccessModifierType_t*)&Data.AccessModifier,*(AccessModifierType_t*)&Data.AccessModifier);
	Input.ReadType(Data.IsExported, Data.IsExported);
	{

		Size_tAsBits  Feld_Sizebits = 0;
		size_t Feld_Size;

		Input.ReadType(Feld_Sizebits, Feld_Sizebits);
		Feld_Size = Feld_Sizebits;

		Data.Fields.resize(Feld_Size);
		for (size_t i2 = 0; i2 < Feld_Size; i2++)
		{
			auto& Item2 = Data.Fields[i2];
			FromBytes(Input, Item2);
		}
	}

	{

		Size_tAsBits  Methods_Sizebits = 0;
		size_t Methods_Size;

		Input.ReadType(Methods_Sizebits, Methods_Sizebits);
		Methods_Size = Methods_Sizebits;

		Data.Methods.resize(Methods_Size);
		for (size_t i2 = 0; i2 < Methods_Size; i2++)
		{
			auto& Item2 = Data.Methods[i2];
			FromBytes(Input, Item2);
		}
	}
	
	{

		Size_tAsBits  Methods_Sizebits = 0;
		size_t Methods_Size;

		Input.ReadType(Methods_Sizebits, Methods_Sizebits);
		Methods_Size = Methods_Sizebits;

		Data.Symbols.resize(Methods_Size);
		for (size_t i2 = 0; i2 < Methods_Size; i2++)
		{
			auto& Item2 = Data.Symbols[i2];
			FromBytes(Input, Item2);
		}
	}
	{

		Size_tAsBits  Methods_Sizebits = 0;
		size_t Methods_Size;

		Input.ReadType(Methods_Sizebits, Methods_Sizebits);
		Methods_Size = Methods_Sizebits;

		Data.GenericAlias.resize(Methods_Size);
		for (size_t i2 = 0; i2 < Methods_Size; i2++)
		{
			auto& Item2 = Data.GenericAlias[i2];
			FromBytes(Input, Item2);
		}
	}

}
void UClib::FromBytes(BitReader& Input, InheritedTrait_Data& Data)
{
	Input.ReadType(Data.TraitID, Data.TraitID);
}
void UClib::FromBytes(BitReader& reader, FuncPtr_Data& Ptr)
{
	reader.ReadType(Ptr.TypeID);
	FromBytes(reader, Ptr.RetType);

	BitMaker::SizeAsBits V = 0;
	reader.ReadType(V, V);
	Ptr.ParsType.resize(V);
	for (size_t i = 0; i < (size_t)V; i++)
	{
		FromBytes(reader, Ptr.ParsType[i]);
	}
	reader.ReadType(*(AccessModifierType_t*)&Ptr.AccessModifier, *(AccessModifierType_t*)&Ptr.AccessModifier);
	reader.ReadType(Ptr.IsExported, Ptr.IsExported);
}
void UClib::FromBytes(BitReader& Input, ClassMethod::Par& Data)
{
	Input.ReadType(Data.IsOutPar);
	FromBytes(Input, Data.Type);
}
void UClib::FromBytes(BitReader& Input, TypedRawReflectionData& Data)
{
	FromBytes(Input,Data._Type);
	FromBytes(Input,Data._Data);
}
void UClib::FromBytes(BitReader& reader, GenericClass_Data& Ptr)
{
	FromBytes(reader, Ptr.Base);
	reader.ReadType(*(AccessModifierType_t*)&Ptr.AccessModifier,*(AccessModifierType_t*)&Ptr.AccessModifier);
	reader.ReadType(Ptr.IsExported, Ptr.IsExported);
}
void UClib::FromBytes(BitReader& reader, GenericFunction_Data& Ptr)
{
	FromBytes(reader, Ptr.Base);
	reader.ReadType(*(AccessModifierType_t*)&Ptr.AccessModifier,*(AccessModifierType_t*)&Ptr.AccessModifier);
	reader.ReadType(Ptr.IsExported, Ptr.IsExported);
}
void UClib::FromBytes(BitReader& Input, GenericBase_Data& Data)
{
	Input.ReadType(Data.Implementation);
}
void UClib::FromBytes(BitReader& Input, TraitMethod& Data)
{
	FromBytes(Input, Data.method);

	bool v = false;
	Input.ReadType(v);

	if (v)
	{
		String str;
		Input.ReadType(str);

		Data.FuncBody = std::move(str);
	}
}
void UClib::FromBytes(BitReader& Input, StaticArray_Data& Data)
{
	Input.ReadType(Data.TypeID, Data.TypeID);
	FromBytes(Input, Data.BaseType);

	Size_tAsBits Size = 0;
	Input.ReadType(Size, Size);
	Data.Count = Size;
}
void UClib::FromBytes(BitReader& Input, NameSpace_Data& Data)
{
}
void UClib::FromBytes(BitReader& Input, ForType_Data& Data)
{
	FromBytes(Input, Data._TargetType);
	Input.ReadType(Data._Scope);	
	Input.ReadType(*(AccessModifierType_t*)&Data.AccessModifier,*(AccessModifierType_t*)&Data.AccessModifier);
	Input.ReadType(Data.IsExported, Data.IsExported);

	BitMaker::SizeAsBits s = 0;
	Input.ReadType(s, s);

	Data._AddedMethods.reserve(s);

	for (size_t i = 0; i < s; i++)
	{
		ClassMethod m;
		FromBytes(Input, m);

		Data._AddedMethods.push_back(std::move(m));
	}
}
void UClib::FromBytes(BitReader& Input, Eval_Data& Data)
{
	FromBytes(Input,Data.Value);
	Input.ReadType(*(AccessModifierType_t*)&Data.AccessModifier, *(AccessModifierType_t*)&Data.AccessModifier);
	Input.ReadType(Data.IsExported, Data.IsExported);
}
void UClib::FromBytes(BitReader& reader, Vector<UsedTagValueData>& Attributes)
{

	Size_tAsBits  Attributes_Sizebits = 0;
	size_t Attributes_Size;

	reader.ReadType(Attributes_Sizebits, Attributes_Sizebits);
	Attributes_Size = Attributes_Sizebits;

	Attributes.resize(Attributes_Size);
	for (size_t i2 = 0; i2 < Attributes_Size; i2++)
	{
		auto& Item2 = Attributes[i2];
		FromBytes(reader, Item2);
	}
}
void UClib::FromBytes(BitReader& reader, ClassField& Item2)
{
	reader.ReadType(Item2.Name, Item2.Name);
	FromBytes(reader, Item2.Type);

	Size_tAsBits offset;
	reader.ReadType(offset, offset);
	Item2.offset = offset;

	AccessModifierType_t proc = (AccessModifierType_t)AccessModifierType::Default;
	reader.ReadType(proc,proc);
	Item2.Protection = (AccessModifierType)proc;
}
void UClib::FromBytes(BitReader& reader, Alias_Data& Alias)
{
	{
		bool HasV = false;
		reader.ReadType(HasV, HasV);
		if (HasV)
		{
			ReflectionCustomTypeID V = {};
			reader.ReadType(V, V);
			Alias.HardAliasTypeID = V;
		}
	}
	FromBytes(reader, Alias.Type);
	reader.ReadType(*(AccessModifierType_t*)&Alias.AccessModifier,*(AccessModifierType_t*)&Alias.AccessModifier);
	reader.ReadType(Alias.IsExported, Alias.IsExported);
}
void UClib::FromBytes(BitReader& Input, Tag_Data& Data)
{
	Input.ReadType(Data.TypeID, Data.TypeID);
	Input.ReadType(*(AccessModifierType_t*)&Data.AccessModifier,*(AccessModifierType_t*)&Data.AccessModifier);
	Input.ReadType(Data.IsExported, Data.IsExported);
	{

		Size_tAsBits  Feld_Sizebits = 0;
		size_t Feld_Size;

		Input.ReadType(Feld_Sizebits, Feld_Sizebits);
		Feld_Size = Feld_Sizebits;

		Data.Fields.resize(Feld_Size);
		for (size_t i2 = 0; i2 < Feld_Size; i2++)
		{
			auto& Item2 = Data.Fields[i2];
			FromBytes(Input, Item2);
		}
	}

	{

		Size_tAsBits  Methods_Sizebits = 0;
		size_t Methods_Size;

		Input.ReadType(Methods_Sizebits, Methods_Sizebits);
		Methods_Size = Methods_Sizebits;

		Data.Methods.resize(Methods_Size);
		for (size_t i2 = 0; i2 < Methods_Size; i2++)
		{
			auto& Item2 = Data.Methods[i2];
			FromBytes(Input, Item2);
		}
	}
}
void UClib::FromBytes(BitReader& Input, UsedTagValueData& Data)
{
	Input.ReadType(Data.TypeID, Data.TypeID);
	FromBytes(Input, Data._Data);
}
void UClib::FromBytes(BitReader& Input, ClassMethod& Data)
{
	Input.ReadType(Data.FullName, Data.FullName);
	Input.ReadType(Data.DecorationName, Data.DecorationName);



	FromBytes(Input, Data.RetType);

	{
		Size_tAsBits bits_Size_tAsBits = 0;
		size_t bits;


		Input.ReadType(bits_Size_tAsBits, bits_Size_tAsBits);

		bits = bits_Size_tAsBits;
		Data.ParsType.resize(bits);

		for (size_t i = 0; i < bits; i++)
		{
			FromBytes(Input, Data.ParsType[i]);
		}
	}
	Input.ReadType(Data.IsThisFunction);
	Input.ReadType(Data.IsUnsafe);
	Input.ReadType(Data.IsExternC);
	Input.ReadType(Data.IsRemoved);
	Input.ReadType(Data.IsExport);
	Input.ReadType(Data.IsTraitDynamicDispatch);
	Input.ReadType(*(AccessModifierType_t*)&Data.Protection);

	FromBytes(Input, Data.Attributes);
}
void UClib::FromBytes(BitReader& Input, ReflectionTypeInfo& Data)
{
	//We should combined _Type and _Type attribute into one byte
	Input.ReadType(*(ReflectionTypes_t*)&Data._Type, *(ReflectionTypes_t*)Data._Type);

	std::bitset<(size_t)ReflectionTypeInfoBools::Max> bits;
	Input.ReadType(bits, bits);

	Data._IsAddress = bits[(size_t)ReflectionTypeInfoBools::IsAddress];
	Data._IsAddressArray = bits[(size_t)ReflectionTypeInfoBools::IsAddressArray];
	Data._Isimmutable = bits[(size_t)ReflectionTypeInfoBools::Isimmutable];
	Data._IsDynamic = bits[(size_t)ReflectionTypeInfoBools::IsDynamic];
	Data._MoveData = bits[(size_t)ReflectionTypeInfoBools::IsMove] ? ReflectionMoveData::Moved : ReflectionMoveData::None;

	if (NextIsCusomTypeID(Data._Type))
	{
		Input.ReadType(Data._CustomTypeID, Data._CustomTypeID);
	}
	else
	{
		Data._CustomTypeID = {};
	}
}
void UClib::FromBytes(BitReader& Input, TraitSymbol& Data)
{
	Input.ReadType(Data.Implementation);
	Input.ReadType(*(AccessModifierType_t*)&Data.AccessModifier);
	Input.ReadType(Data.IsExported);
}
void UClib::FromBytes(BitReader& Input, TraitAlias& Data)
{
	Input.ReadType(Data.AliasName);
	FromBytes(Input,Data.Type);

	bool op1 = false;
	Input.ReadType(op1);
	if (op1)
	{
		ReflectionRawData Val;
		FromBytes(Input, Val);


		Data.Expression = std::move(Val);
	}

	bool op2 = false;
	Input.ReadType(op2);
	if (op2)
	{
		Vector<ReflectionTypeInfo> types;
		BitReader::SizeAsBits size = 0;

		Input.ReadType(size);

		types.reserve(size);
		for (size_t i = 0; i < size; i++)
		{
			ReflectionTypeInfo val;
			FromBytes(Input, val);

			types.push_back(std::move(val));
		}

		Data.TypePack = std::move(types);
	}
}
void UClib::FixRawValue(Endian AssemblyEndian, NTypeSize BitSize, const ClassAssembly& Types, ReflectionRawData& RawValue, const ReflectionTypeInfo& Type)
{
	auto CPUEndian = BitConverter::_CPUEndian;

	if (CPUEndian != AssemblyEndian)
	{
		switch (Type._Type)
		{
		case ReflectionTypes::sInt16:
		case ReflectionTypes::uInt16:
		{
			BitConverter::Byte16* Ptr = (BitConverter::Byte16*)RawValue.Get_Data();
			Ptr->FlpBytes();
		}
		break;
	Bit32Type:
		case ReflectionTypes::sInt32:
		case ReflectionTypes::uInt32:
		case ReflectionTypes::float32:
		{
			BitConverter::Byte32* Ptr = (BitConverter::Byte32*)RawValue.Get_Data();
			Ptr->FlpBytes();
		}
		break;
	Bit64Type:
		case ReflectionTypes::sInt64:
		case ReflectionTypes::uInt64:
		case ReflectionTypes::float64:
		{
			BitConverter::Byte64* Ptr = (BitConverter::Byte64*)RawValue.Get_Data();
			Ptr->FlpBytes();
		}
		break;
		case ReflectionTypes::uIntPtr:
		case ReflectionTypes::sIntPtr:
		{
			switch (BitSize)
			{
			case NTypeSize::int32:goto Bit32Type;
			case NTypeSize::int64:goto Bit64Type;
			default:
				UCodeLangUnreachable();
				break;
			}
		}
		case ReflectionTypes::CustomType:
		{
			UCodeLangThrowException("not added yet");
		}
		default:
			UCodeLangUnreachable();
			break;
		}
	}
}
void UClib::FixAssemblyRawValues(Endian AssemblyEndian, NTypeSize BitSize, const ClassAssembly& Assembly)
{
	for (auto& Item : Assembly.Classes)
	{
		switch (Item.get()->Get_Type())
		{
		case ClassType::Enum:
		{
			Enum_Data& Data = Item->Get_EnumData();

			for (auto& Item : Data.Values)
			{
				FixRawValue(AssemblyEndian, BitSize, Assembly, Item._Data, Data.BaseType);
			}
		}
		break;
		case ClassType::Eval:
		{
			Eval_Data& Data = Item->Get_EvalData();

			FixRawValue(AssemblyEndian, BitSize, Assembly, Data.Value);
		}
		break;
		default:
			break;
		}
	}
}
bool UClib::ToFile(const UClib* Lib, const Path& path)
{
	std::ofstream File(path, std::ios::binary);
	if (File.is_open())
	{

		BytesPtr Bits = ToRawBytes(Lib);

		File.write((const char*)Bits.Data(), Bits.Size());


		File.close();
		return true;
	}
	else
	{
		return false;
	}
}
bool UClib::FromFile(UClib* Lib, const Path& path)
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
		auto V = FromBytes(Lib, Bits.AsSpan());

		return V;
	}
	else
	{
		return false;
	}

}
UCodeLangEnd