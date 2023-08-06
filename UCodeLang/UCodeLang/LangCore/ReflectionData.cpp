#include "ReflectionData.hpp"
UCodeLangStart
void ClassAssembly::PushCopyClasses(const ClassAssembly& source, ClassAssembly& Out)
{
	for (auto& Item : source.Classes)
	{
		Out.Classes.push_back(std::make_unique<AssemblyNode>(*Item));
	}
}
const AssemblyNode* ClassAssembly::Find_Node(ReflectionCustomTypeID TypeID) const
{
	for (auto& Item : Classes)
	{
		Optional< ReflectionCustomTypeID> Valu;
	
		switch (Item->Get_Type())
		{
		case ClassType::Class:Valu = Item->Get_ClassData().TypeID; break;
		case ClassType::Enum:Valu = Item->Get_EnumData().TypeID; break;
		case ClassType::Alias:Valu = Item->Get_AliasData().HardAliasTypeID; break;
		case ClassType::Trait:Valu = Item->Get_TraitData().TypeID; break;
		default:
			break;
		}

		if (Valu.has_value())
		{
			if (Valu.value() == TypeID)
			{
				return Item.get();
			}
		}
	}

	return {};
}
AssemblyNode* ClassAssembly::Find_Node(ReflectionCustomTypeID TypeID)
{
	const ClassAssembly* This = (ClassAssembly*)this;
	return (AssemblyNode*)This->Find_Node(TypeID);
}
const AssemblyNode* ClassAssembly::Find_Node(const  ReflectionTypeInfo& Type) const
{
	switch (Type._Type)
	{
	case ReflectionTypes::CustomType:return Find_Node(Type._CustomTypeID);
	default:
		break;
	}
	return nullptr;
}
AssemblyNode* ClassAssembly::Find_Node(const  ReflectionTypeInfo& Type)
{
	const ClassAssembly* This = (ClassAssembly*)this;
	return (AssemblyNode*)This->Find_Node(Type);
}
Optional<size_t> ClassAssembly::GetSize(const ReflectionTypeInfo& Type, bool Is32Bit) const
{
	auto PtrSize = Is32Bit ? sizeof(UInt32) : sizeof(UInt64);
	if (Type.IsDynamicTrait())
	{
		return PtrSize * 2;
	}

	if (Type.IsAddress() || Type.IsAddressArray())
	{
		return PtrSize;
	}

	switch (Type._Type)
	{
	case ReflectionTypes::Bool:
	case ReflectionTypes::Char:
	case ReflectionTypes::sInt8:
	case ReflectionTypes::uInt8:
		return sizeof(UInt8);

	case ReflectionTypes::sInt16:
	case ReflectionTypes::uInt16:
		return sizeof(UInt16);

	case ReflectionTypes::float32:
	case ReflectionTypes::sInt32:
	case ReflectionTypes::uInt32:
		return sizeof(UInt32);

	case ReflectionTypes::float64:
	case ReflectionTypes::sInt64:
	case ReflectionTypes::uInt64:
		return sizeof(UInt64);

	case ReflectionTypes::uIntPtr:
	case ReflectionTypes::sIntPtr:
		return PtrSize;
	case ReflectionTypes::CustomType:
	{
		auto Node = Find_Node(Type);
		if (Node)
		{
			switch (Node->Get_Type())
			{
			case ClassType::Class:
			{
				auto Data = Node->Get_ClassData();
				return Data.Size;
			}
			break;
			case ClassType::Enum:
			{
				auto Data = Node->Get_EnumData();
				Optional<size_t> Size = GetSize(Data.BaseType, Is32Bit);
				if (Size.has_value())
				{
					if (Data.EnumVariantUnion.has_value())
					{
						auto Union = Find_Node(Data.EnumVariantUnion.value())->Get_ClassData();
						Size.value() += Union.Size;
					}
				}
				return Size;
			}
			break;
			case ClassType::Alias:
				return GetSize(Type, Is32Bit);
			default:
				break;
			}
		}
	}
		break;
	default:
		break;
	}
	return {};
}
Optional<size_t> ClassAssembly::GetSize(const ClassMethod::Par& Type, bool Is32Bit) const
{
	auto PtrSize = Is32Bit ? sizeof(UInt32) : sizeof(UInt64);
	if (Type.IsOutPar)
	{
		return PtrSize;
	}
	return GetSize(Type.Type, Is32Bit);

}

//Get the DefaultConstructor or directly does the operation if a Primitive.
//if the first Optional is empty the operation failed

Optional<Optional<Vector<ClassAssembly::OnDoDefaultConstructorCall>>> ClassAssembly::CallDefaultConstructor(const ReflectionTypeInfo& Type, void* Object, bool Is32Bit) const
{
	if (Type.IsAddress())
	{
		*(void**)(Object) = nullptr;

		return { {} };
	}

	switch (Type._Type)
	{
	case ReflectionTypes::sInt8:
	{
		*(Int8*)(Object) = Int8();

		return  { {} };
	}
	break;
	case ReflectionTypes::sInt16:
	{
		*(Int16*)(Object) = Int16();

		return  { {} };
	}
	break;
	case ReflectionTypes::sInt32:
	{
		*(Int32*)(Object) = Int32();

		return  { {} };
	}
	break;
	case ReflectionTypes::sInt64:
	{
		*(UInt64*)(Object) = UInt64();

		return  { {} };
	}
	break;
	case ReflectionTypes::uInt8:
	{
		*(UInt8*)(Object) = UInt8();

		return  { {} };
	}
	break;
	case ReflectionTypes::uInt16:
	{
		*(UInt16*)(Object) = UInt16();

		return  { {} };
	}
	break;
	case ReflectionTypes::uInt32:
	{
		*(UInt32*)(Object) = UInt32();

		return  { {} };
	}
	break;
	case ReflectionTypes::uInt64:
	{
		*(UInt64*)(Object) = UInt64();

		return  { {} };
	}
	break;
	case ReflectionTypes::Char:
	{
		*(char*)(Object) = char();

		return  { {} };
	}
	case ReflectionTypes::Uft8:
	{
		*(Utf8*)(Object) = Utf8();

		return  { {} };
	}
	case ReflectionTypes::Uft16:
	{
		*(Utf16*)(Object) = Utf16();

		return  { {} };
	}
	case ReflectionTypes::Uft32:
	{
		*(Utf32*)(Object) = Utf32();

		return  { {} };
	}
	case ReflectionTypes::Bool:
	{
		*(bool*)(Object) = bool();

		return  { {} };
	}
	break;
	case ReflectionTypes::float32:
	{
		*(float32*)(Object) = float32();

		return  { {} };
	}
	case ReflectionTypes::float64:
	{
		*(float64*)(Object) = float64();

		return  { {} };
	}
	case ReflectionTypes::CustomType:
	{
		auto Node = this->Find_Node(Type._CustomTypeID);
		if (Node) 
		{
			switch (Node->Get_Type())
			{
			case ClassType::Enum:
			{
				auto& EnumClass = Node->Get_EnumData();

				Optional<size_t> EnumSize = GetSize(EnumClass.BaseType,Is32Bit);

				if (EnumSize.has_value() && EnumClass.Values.size())
				{
					auto& EValue = EnumClass.Values.front();
				
					memcpy(Object, EValue._Data.Get_Data(), EnumSize.value());
					if (EValue.EnumVariantType.has_value())
					{
						void* UPtr = (void*)((uintptr_t)Object + EnumSize.value());
						return this->CallDefaultConstructor(EValue.EnumVariantType.value(), UPtr, Is32Bit);
					}
					else
					{
						return { {} };
					}
				}
			}
			break;
			case ClassType::Class:
			{
				auto& ClassData = Node->Get_ClassData();

				
				if (auto FuncToCall = ClassData.Get_ClassConstructor())
				{
					OnDoDefaultConstructorCall r;
					r.MethodToCall = FuncToCall;
					r.ThisPtr = Object;

					Vector<ClassAssembly::OnDoDefaultConstructorCall> V;
					V.push_back(std::move(r));
					return V;
				}
				else if (auto FuncToCall = ClassData.Get_ClassInit())
				{
					OnDoDefaultConstructorCall r;
					r.MethodToCall = FuncToCall;
					r.ThisPtr = Object;
					
					Vector<ClassAssembly::OnDoDefaultConstructorCall> V;
					V.push_back(std::move(r));
					return { V };
				}
				else
				{
					Vector<ClassAssembly::OnDoDefaultConstructorCall> r;
					for (auto& Field : ClassData.Fields)
					{
						void* FieldObject = (void*)((uintptr_t)Object + (uintptr_t)Field.offset);
						auto RItem = CallDefaultConstructor(Field.Type, FieldObject, Is32Bit);
						if (!RItem.has_value())
						{
							return {};
						}
						if (RItem.value().has_value())
						{
							auto& Val = RItem.value().value();
							for (auto& Item : Val)
							{
								r.push_back(std::move(Item));
							}
						}
					}
					return {r};
				}
			}
			break;
			default:
				break;
			}
		}
	}
	break;
	default:
		return {};
		break;
	}
	return {};
}
Optional<Optional<Vector<ClassAssembly::OnDoDefaultConstructorCall>>> ClassAssembly::CallDefaultConstructor(const ClassMethod::Par& Type, void* Object, bool Is32Bit) const
{
	if (Type.IsOutPar)
	{
		return { {} };
	}
	return CallDefaultConstructor(Type.Type,Object,Is32Bit);
}


Optional<Optional<Vector<ClassAssembly::OnMoveConstructorCall>>> ClassAssembly::CallCopyConstructor(const ReflectionTypeInfo& Type,void* Source, void* Output, bool Is32Bit) const
{
	if (Type.IsAddress())
	{
		*(void**)(Output) = *(void**)Source;
		return { {} };
	}

	switch (Type._Type)
	{
	case ReflectionTypes::sInt8:
	{
		*(Int8*)(Output) = *(Int8*)Source;

		return  { {} };
	}
	break;
	case ReflectionTypes::sInt16:
	{
		*(Int16*)(Output) = *(Int16*)Source;

		return  { {} };
	}
	break;
	case ReflectionTypes::sInt32:
	{
		*(Int32*)(Output) = *(Int32*)Source;

		return  { {} };
	}
	break;
	case ReflectionTypes::sInt64:
	{
		*(Int64*)(Output) = *(Int64*)Source;

		return  { {} };
	}
	break;
	case ReflectionTypes::uInt8:
	{
		*(UInt8*)(Output) = *(UInt8*)Source;

		return  { {} };
	}
	break;
	case ReflectionTypes::uInt16:
	{
		*(UInt16*)(Output) = *(UInt16*)Source;


		return  { {} };
	}
	break;
	case ReflectionTypes::uInt32:
	{
		*(UInt32*)(Output) = *(UInt32*)Source;


		return  { {} };
	}
	break;
	case ReflectionTypes::uInt64:
	{
		*(UInt64*)(Output) = *(UInt64*)Source;


		return  { {} };
	}
	break;
	case ReflectionTypes::Char:
	{
		*(char*)(Output) = *(char*)Source;


		return  { {} };
	}
	case ReflectionTypes::Uft8:
	{
		*(Utf8*)(Output) = *(Utf8*)Source;


		return  { {} };
	}
	case ReflectionTypes::Uft16:
	{
		*(Utf16*)(Output) = *(Utf16*)Source;

		return  { {} };
	}
	case ReflectionTypes::Uft32:
	{
		*(Utf32*)(Output) = *(Utf32*)Source;

		return  { {} };
	}
	case ReflectionTypes::Bool:
	{
		*(bool*)(Output) = *(bool*)Source;

		return  { {} };
	}
	break;
	case ReflectionTypes::float32:
	{
		*(float32*)(Output) = *(float32*)Source;

		return  { {} };
	}
	case ReflectionTypes::float64:
	{
		*(float64*)(Output) = *(float64*)Source;

		return  { {} };
	}
	case ReflectionTypes::CustomType:
	{


	}

	}
	return {};
}
Optional<Optional<Vector<ClassAssembly::OnMoveConstructorCall>>>  ClassAssembly::CallMoveConstructor(const ReflectionTypeInfo& Type, void* Source, void* Output, bool Is32Bit) const
{
	//std::move for intent
	if (Type.IsAddress())
	{
		*(void**)(Output) = std::move(*(void**)Source);
		return { {} };
	}

	switch (Type._Type)
	{
	case ReflectionTypes::sInt8:
	{
		*(Int8*)(Output) =std::move(*(Int8*)Source);

		return  { {} };
	}
	break;
	case ReflectionTypes::sInt16:
	{
		*(Int16*)(Output) = std::move(*(Int16*)Source);

		return  { {} };
	}
	break;
	case ReflectionTypes::sInt32:
	{
		*(Int32*)(Output) = std::move(*(Int32*)Source);

		return  { {} };
	}
	break;
	case ReflectionTypes::sInt64:
	{
		*(Int64*)(Output) = std::move(*(Int64*)Source);

		return  { {} };
	}
	break;
	case ReflectionTypes::uInt8:
	{
		*(UInt8*)(Output) = std::move(*(UInt8*)Source);

		return  { {} };
	}
	break;
	case ReflectionTypes::uInt16:
	{
		*(UInt16*)(Output) = std::move(*(UInt16*)Source);


		return  { {} };
	}
	break;
	case ReflectionTypes::uInt32:
	{
		*(UInt32*)(Output) = std::move(*(UInt32*)Source);


		return  { {} };
	}
	break;
	case ReflectionTypes::uInt64:
	{
		*(UInt64*)(Output) = std::move(*(UInt64*)Source);


		return  { {} };
	}
	break;
	case ReflectionTypes::Char:
	{
		*(char*)(Output) = std::move(*(char*)Source);


		return  { {} };
	}
	case ReflectionTypes::Uft8:
	{
		*(Utf8*)(Output) = std::move(*(Utf8*)Source);
		

		return  { {} };
	}
	case ReflectionTypes::Uft16:
	{
		*(Utf16*)(Output) = std::move(*(Utf16*)Source);

		return  { {} };
	}
	case ReflectionTypes::Uft32:
	{
		*(Utf32*)(Output) = std::move(*(Utf32*)Source);

		return  { {} };
	}
	case ReflectionTypes::Bool:
	{
		*(bool*)(Output) = std::move(*(bool*)Source);

		return  { {} };
	}
	break;
	case ReflectionTypes::float32:
	{
		*(float32*)(Output) = std::move(*(float32*)Source);
		
		return  { {} };
	}
	case ReflectionTypes::float64:
	{
		*(float64*)(Output) = std::move(*(float64*)Source);

		return  { {} };
	}
	case ReflectionTypes::CustomType:
	{


	}

	}
	return {};
}
Optional<Optional<Vector<ClassAssembly::OnDoDefaultConstructorCall>>> ClassAssembly::CallDestructor(const ReflectionTypeInfo& Type, void* Object, bool Is32Bit) const
{
	return { {} };
}
ClassAssembly::CompareType_t ClassAssembly::CompareType(const ReflectionTypeInfo& TypeA, const ClassAssembly& TypeAAssembly, const ReflectionTypeInfo& TypeB, const ClassAssembly& TypeBAssembly)
{
	if (TypeA == TypeB)
	{
		return  CompareType_t::Identical;
	}

	if (TypeA._Type == TypeB._Type)
	{
		if (TypeA._CustomTypeID == TypeB._CustomTypeID)
		{
			auto nodeA = TypeAAssembly.Find_Node(TypeA);
			auto nodeB = TypeAAssembly.Find_Node(TypeB);
			if (nodeA && nodeB
				&& nodeA->Get_Type() == nodeB->Get_Type())
			{
				return CompareType_t::Similar;
			}
		}

	}
	return CompareType_t::TooDifferent;
}

Optional<Optional<Vector<ClassAssembly::OnMoveConstructorCall>>> ClassAssembly::DoTypeCoercion(
	const ReflectionTypeInfo& TypeSource, void* Source, const ClassAssembly& SourceAssembly,
	const ReflectionTypeInfo& TypeOutput, void* Output, const ClassAssembly& OutputAssembly, bool Is32Bit)
{
	if (TypeOutput.IsAddress() || TypeOutput.IsAddressArray() || TypeOutput.IsDynamicTrait() || TypeOutput.IsMovedType()
		|| TypeSource.IsAddress() || TypeSource.IsAddressArray() || TypeSource.IsDynamicTrait() || TypeSource.IsMovedType())
	{
		return {};
	}
	bool IsSrcUIntType =
		TypeSource._Type == ReflectionTypes::Bool ||
		TypeSource._Type == ReflectionTypes::Char ||
		TypeSource._Type == ReflectionTypes::Uft8 ||
		TypeSource._Type == ReflectionTypes::Uft16 ||
		TypeSource._Type == ReflectionTypes::Uft32 ||
		TypeSource._Type == ReflectionTypes::uInt8 ||
		TypeSource._Type == ReflectionTypes::uInt16 ||
		TypeSource._Type == ReflectionTypes::uInt32 ||
		TypeSource._Type == ReflectionTypes::uInt64 ||
		TypeSource._Type == ReflectionTypes::uIntPtr;

	bool IsSrcSIntType =
		TypeSource._Type == ReflectionTypes::sInt8 ||
		TypeSource._Type == ReflectionTypes::sInt16 ||
		TypeSource._Type == ReflectionTypes::sInt32 ||
		TypeSource._Type == ReflectionTypes::sInt64 ||
		TypeSource._Type == ReflectionTypes::sIntPtr;

	bool IsOutUIntType =
		TypeOutput._Type == ReflectionTypes::Bool ||
		TypeOutput._Type == ReflectionTypes::Char ||
		TypeOutput._Type == ReflectionTypes::Uft8 ||
		TypeOutput._Type == ReflectionTypes::Uft16 ||
		TypeOutput._Type == ReflectionTypes::Uft32 ||
		TypeOutput._Type == ReflectionTypes::uInt8 ||
		TypeOutput._Type == ReflectionTypes::uInt16 ||
		TypeOutput._Type == ReflectionTypes::uInt32 ||
		TypeOutput._Type == ReflectionTypes::uInt64 ||
		TypeOutput._Type == ReflectionTypes::uIntPtr;

	bool IsOutSIntType =
		TypeOutput._Type == ReflectionTypes::sInt8 ||
		TypeOutput._Type == ReflectionTypes::sInt16 ||
		TypeOutput._Type == ReflectionTypes::sInt32 ||
		TypeOutput._Type == ReflectionTypes::sInt64 ||
		TypeOutput._Type == ReflectionTypes::sIntPtr;

	AnyInt64 SrcAsInt;
	if (IsSrcUIntType)
	{
		switch (SourceAssembly.GetSize(TypeSource,Is32Bit).value_or(0))
		{
		case 1:
			SrcAsInt = *(UInt8*)Source;
			break;

		case 2:
			SrcAsInt = *(UInt16*)Source;
			break;

		case 4:
			SrcAsInt = *(UInt32*)Source;
			break;
		case 8:
			SrcAsInt = *(UInt64*)Source;
			break;
		default:
			return {};
			break;
		}

	}
	else if (IsSrcSIntType)
	{
		switch (SourceAssembly.GetSize(TypeSource, Is32Bit).value_or(0))
		{
		case 1:
			SrcAsInt = *(Int8*)Source;
			break;

		case 2:
			SrcAsInt = *(Int16*)Source;
			break;

		case 4:
			SrcAsInt = *(Int32*)Source;
			break;
		case 8:
			SrcAsInt = *(Int64*)Source;
			break;
		default:
			return {};
			break;
		}

	}

	if (IsOutUIntType)
	{
		size_t Size = OutputAssembly.GetSize(TypeOutput, Is32Bit).value_or(0);
		if (IsSrcUIntType) 
		{
			switch (Size)
			{
			case 1:
				*(UInt8*)Output = SrcAsInt.AsUInt8;
				return { {} };
				break;
			case 2:
				*(UInt8*)Output = SrcAsInt.AsUInt16;
				return { {} };
				break;
			case 4:
				*(UInt8*)Output = SrcAsInt.AsUInt32;
				return { {} };
				break;
			case 8:
				*(UInt8*)Output = SrcAsInt.AsUInt64;
				return { {} };
				break;
			default:
				return {};
				break;
			}
		}
		else if (IsSrcSIntType)
		{
			switch (Size)
			{
			case 1:
				*(Int8*)Output = SrcAsInt.AsInt8;
				return { {} };
				break;
			case 2:
				*(Int8*)Output = SrcAsInt.AsInt16;
				return { {} };
				break;
			case 4:
				*(Int8*)Output = SrcAsInt.AsInt32;
				return { {} };
				break;
			case 8:
				*(Int8*)Output = SrcAsInt.AsInt64;
				return { {} };
				break;
			default:
				return {};
				break;
			}
		}
	}

	return {};
}


Optional<ClassAssembly::InfoVec2_t> ClassAssembly::IsVec2_t(const ReflectionTypeInfo& Type)
{
	return {};
}


Optional<ClassAssembly::InfoVec3_t> ClassAssembly::IsVec3_t(const ReflectionTypeInfo& Type)
{
	return {};

}

Optional<ClassAssembly::InfoVector_t> ClassAssembly::IsVector_t(const ReflectionTypeInfo& Type)
{
	return {};
}

Optional<ClassAssembly::InfoOptional_t> ClassAssembly::IsOptional_t(const ReflectionTypeInfo& Type)
{
	return {};
}
Optional<ClassAssembly::InfoResult_t> ClassAssembly::IsResult_t(const ReflectionTypeInfo& Type)
{
	return {};
}

Optional<ClassAssembly::InfoString_t> ClassAssembly::IsString_t(const ReflectionTypeInfo& Type)
{
	return Optional<InfoString_t>();
}

Optional<ClassAssembly::InfoStringView_t> ClassAssembly::IsStringView_t(const ReflectionTypeInfo& Type)
{
	return Optional<InfoStringView_t>();
}

Optional<ClassAssembly::InfoSpan_t> ClassAssembly::IsSpan_t(const ReflectionTypeInfo& Type)
{
	return Optional<InfoSpan_t>();
}



AssemblyNode::AssemblyNode(ClassType type) : Type(type)
{
	switch (type)
	{
	case ClassType::Null:
		break;
	case ClassType::Class:
		 new (&_Class) Class_Data();
		break;
	case ClassType::Enum:
		new (&_Enum) Enum_Data();
		break;
	case ClassType::Alias:
		new (&_Alias) Alias_Data();
		break;
	case ClassType::Eval:
		new (&_Eval) Eval_Data();
		break;
	case ClassType::Trait:
		new (&_Trait) Trait_Data();
		break;
	case ClassType::Tag:
		new (&_Tag) Tag_Data();
		break;
	case ClassType::StaticVarable:
		new (&_StaticVar) StaticVar_Data();
		break;
	case ClassType::ThreadVarable:
		new (&_ThreadVar) ThreadVar_Data();
		break;
	case ClassType::StaticArray:
		new (&_StaticArr) StaticArray_Data();
		break;
	case ClassType::FuncPtr:
		new (&_FuncPtr) FuncPtr_Data();
		break;
	case ClassType::GenericClass:
		new (&_GenericClass) GenericClass_Data();
		break;
	case ClassType::GenericFuncion:
		new (&_GenericFunc) GenericFuncion_Data();
		break;
	default:
		throw std::exception("bad path");
		break;
	}
}
AssemblyNode& AssemblyNode::operator=(AssemblyNode&& node)
{
	this->~AssemblyNode();
	Type = node.Type;
	Name = std::move(node.Name);
	FullName = std::move(node.FullName);

	switch (node.Type)
	{
	case ClassType::Null:
		break;
	case ClassType::Class:
		new (&_Class) Class_Data(std::move(node.Get_ClassData()));
		break;
	case ClassType::Enum:
		new (&_Enum) Enum_Data(node.Get_EnumData());
		break;
	case ClassType::Alias:
		new (&_Alias) Alias_Data(node.Get_AliasData());
		break;
	case ClassType::Eval:
		new (&_Eval) Eval_Data(node.Get_EvalData());
		break;
	case ClassType::Trait:
		new (&_Trait) Trait_Data(node.Get_TraitData());
		break;
	case ClassType::Tag:
		new (&_Tag) Tag_Data(node.Get_TagData());
		break;
	case ClassType::StaticVarable:
		new (&_StaticVar) StaticVar_Data(node.Get_StaticVar());
		break;
	case ClassType::ThreadVarable:
		new (&_ThreadVar) ThreadVar_Data(node.Get_ThreadVar());
		break;
	case ClassType::StaticArray:
		new (&_StaticArr) StaticArray_Data(node.Get_StaticArray());
		break;
	case ClassType::FuncPtr:
		new (&_FuncPtr) FuncPtr_Data(node.Get_FuncPtr());
		break;
	case ClassType::GenericClass:
		new (&_GenericClass) GenericClass_Data(node.Get_GenericClass());
		break;
	case ClassType::GenericFuncion:
		new (&_GenericFunc) GenericFuncion_Data(node.Get_GenericFuncionData());
		break;
	default:
		throw std::exception("bad path");
		break;
	}

	node.Type = ClassType::Null;
	return *this;
}
AssemblyNode& AssemblyNode::operator=(const AssemblyNode& node)
{
	this->~AssemblyNode();
	Type = node.Type;
	Name = node.Name;
	FullName = node.FullName;

	switch (node.Type)
	{
	case ClassType::Class:
		new (&_Class) Class_Data(node.Get_ClassData());
		break;
	case ClassType::Enum:
		new (&_Enum) Enum_Data(node.Get_EnumData());
		break;
	case ClassType::Alias:
		new (&_Alias) Alias_Data(node.Get_AliasData());
		break;
	case ClassType::Eval:
		new (&_Eval) Eval_Data(node.Get_EvalData());
		break;
	case ClassType::Trait:
		new (&_Trait) Trait_Data(node.Get_TraitData());
		break;
	case ClassType::Tag:
		new (&_Tag) Tag_Data(node.Get_TagData());
		break;
	case ClassType::StaticVarable:
		new (&_StaticVar) StaticVar_Data(node.Get_StaticVar());
		break;
	case ClassType::ThreadVarable:
		new (&_ThreadVar) ThreadVar_Data(node.Get_ThreadVar());
		break;
	case ClassType::StaticArray:
		new (&_StaticArr) StaticArray_Data(node.Get_StaticArray());
		break;
	case ClassType::FuncPtr:
		new (&_FuncPtr) FuncPtr_Data(node.Get_FuncPtr());
		break;
	case ClassType::GenericClass:
		new (&_GenericClass) GenericClass_Data(node.Get_GenericClass());
		break;
	case ClassType::GenericFuncion:
		new (&_GenericFunc) GenericFuncion_Data(node.Get_GenericFuncionData());
		break;
	default:
		throw std::exception("bad path");
		break;
	}
	return *this;
}
AssemblyNode::~AssemblyNode()
{
	switch (Type)
	{
	case ClassType::Null:
		break;
	case ClassType::Class:
		_Class.~Class_Data();
		break;
	case ClassType::Enum:
		_Enum.~Enum_Data();
		break;
	case ClassType::Alias:
		_Alias.~Alias_Data();
		break;
	case ClassType::Eval:
		_Eval.~Eval_Data();
		break;
	case ClassType::Trait:
		_Trait.~Trait_Data();
		break;
	case ClassType::Tag:
		_Tag.~Tag_Data();
		break;
	case ClassType::StaticVarable:
		_StaticVar.~StaticVar_Data();
		break;
	case ClassType::ThreadVarable:
		_ThreadVar.~ThreadVar_Data();
		break;
	case ClassType::StaticArray:
		_StaticArr.~StaticArray_Data();
		break;
	case ClassType::FuncPtr:
		_FuncPtr.~FuncPtr_Data();
		break;
	case ClassType::GenericClass:
		_GenericClass.~GenericClass_Data();
		break;
	case ClassType::GenericFuncion:
		_GenericFunc.~GenericFuncion_Data();
		break;
	default:
		//throw std::exception("bad path");
		break;
	}
}
UCodeLangEnd