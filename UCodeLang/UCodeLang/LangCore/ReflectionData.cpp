#include "ReflectionData.hpp"

#include "../Compilation/Helpers/ParseHelper.hpp"
#include "../Compilation/LexerDefs.h"
#include "../Compilation/UAssembly/UAssembly.hpp"
UCodeLangStart
void ClassAssembly::PushCopyClasses(const ClassAssembly& source, ClassAssembly& Out)
{
	for (auto& Item : source.Classes)
	{
		Out.Classes.push_back(std::make_unique<AssemblyNode>(*Item));
	}
}
Optional<ReflectionCustomTypeID> ClassAssembly::GetReflectionTypeID(const AssemblyNode* Item)
{
	Optional< ReflectionCustomTypeID> Value;

	switch (Item->Get_Type())
	{
	case ClassType::Class:Value = Item->Get_ClassData().TypeID; break;
	case ClassType::Enum:Value = Item->Get_EnumData().TypeID; break;
	case ClassType::Alias:Value = Item->Get_AliasData().HardAliasTypeID; break;
	case ClassType::Trait:Value = Item->Get_TraitData().TypeID; break;
	case ClassType::Tag:Value = Item->Get_TagData().TypeID; break;
	case ClassType::StaticArray:Value = Item->Get_StaticArray().TypeID; break;
	case ClassType::FuncPtr:Value = Item->Get_FuncPtr().TypeID; break;
	case ClassType::GenericClass:break;
	case ClassType::GenericFunction:break;
	case ClassType::NameSpace:break;
	case ClassType::ForType:break;
	case ClassType::Eval:break;
	default:
		UCodeLangUnreachable();
		break;
	}

	return Value;
}
const AssemblyNode* ClassAssembly::Find_Node(ReflectionCustomTypeID TypeID) const
{
	for (auto& Item : Classes)
	{
		Optional< ReflectionCustomTypeID> Value = GetReflectionTypeID(Item.get());

		if (Value.has_value())
		{
			if (Value.value() == TypeID)
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

	if (Type.IsAddress() || Type.IsAddressArray() || Type.IsMovedType())
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
	using InerRet = Optional<Vector<ClassAssembly::OnDoDefaultConstructorCall>>;
	if (Type.IsAddress())
	{
		*(void**)(Object) = nullptr;

		return { InerRet() };
	}

	switch (Type._Type)
	{
	case ReflectionTypes::sInt8:
	{
		*(Int8*)(Object) = Int8();

		return  { InerRet() };
	}
	break;
	case ReflectionTypes::sInt16:
	{
		*(Int16*)(Object) = Int16();

		return  { InerRet() };
	}
	break;
	case ReflectionTypes::sInt32:
	{
		*(Int32*)(Object) = Int32();

		return  { InerRet() };
	}
	break;
	case ReflectionTypes::sInt64:
	{
		*(UInt64*)(Object) = UInt64();

		return  { InerRet() };
	}
	break;
	case ReflectionTypes::uInt8:
	{
		*(UInt8*)(Object) = UInt8();

		return  { InerRet() };
	}
	break;
	case ReflectionTypes::uInt16:
	{
		*(UInt16*)(Object) = UInt16();

		return  { InerRet() };
	}
	break;
	case ReflectionTypes::uInt32:
	{
		*(UInt32*)(Object) = UInt32();

		return  { InerRet() };
	}
	break;
	case ReflectionTypes::uInt64:
	{
		*(UInt64*)(Object) = UInt64();

		return  { InerRet() };
	}
	break;
	case ReflectionTypes::Char:
	{
		*(char*)(Object) = char();

		return  { InerRet() };
	}
	case ReflectionTypes::Uft8:
	{
		*(Utf8*)(Object) = Utf8();

		return  { InerRet() };
	}
	case ReflectionTypes::Uft16:
	{
		*(Utf16*)(Object) = Utf16();

		return  { InerRet() };
	}
	case ReflectionTypes::Uft32:
	{
		*(Utf32*)(Object) = Utf32();

		return  { InerRet() };
	}
	case ReflectionTypes::Bool:
	{
		*(bool*)(Object) = bool();

		return  { InerRet() };
	}
	break;
	case ReflectionTypes::float32:
	{
		*(float32*)(Object) = float32();

		return  { InerRet() };
	}
	case ReflectionTypes::float64:
	{
		*(float64*)(Object) = float64();

		return  { InerRet() };
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
						return { InerRet() };
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
	using InerRetType = Optional<Vector<ClassAssembly::OnDoDefaultConstructorCall>>;

	if (Type.IsOutPar)
	{
		return { InerRetType() };
	}
	return CallDefaultConstructor(Type.Type,Object,Is32Bit);
}


Optional<Optional<Vector<ClassAssembly::OnMoveConstructorCall>>> ClassAssembly::CallCopyConstructor(const ReflectionTypeInfo& Type,void* Source, void* Output, bool Is32Bit) const
{

	using InerRetType = Optional<Vector<ClassAssembly::OnMoveConstructorCall>>;

	if (Type.IsAddress())
	{
		*(void**)(Output) = *(void**)Source;
		return { InerRetType() };
	}

	switch (Type._Type)
	{
	case ReflectionTypes::sInt8:
	{
		*(Int8*)(Output) = *(Int8*)Source;

		return   { InerRetType() };
	}
	break;
	case ReflectionTypes::sInt16:
	{
		*(Int16*)(Output) = *(Int16*)Source;

		return   { InerRetType() };
	}
	break;
	case ReflectionTypes::sInt32:
	{
		*(Int32*)(Output) = *(Int32*)Source;

		return   { InerRetType() };
	}
	break;
	case ReflectionTypes::sInt64:
	{
		*(Int64*)(Output) = *(Int64*)Source;

		return   { InerRetType() };
	}
	break;
	case ReflectionTypes::uInt8:
	{
		*(UInt8*)(Output) = *(UInt8*)Source;

		return   { InerRetType() };
	}
	break;
	case ReflectionTypes::uInt16:
	{
		*(UInt16*)(Output) = *(UInt16*)Source;


		return   { InerRetType() };
	}
	break;
	case ReflectionTypes::uInt32:
	{
		*(UInt32*)(Output) = *(UInt32*)Source;


		return   { InerRetType() };
	}
	break;
	case ReflectionTypes::uInt64:
	{
		*(UInt64*)(Output) = *(UInt64*)Source;


		return   { InerRetType() };
	}
	break;
	case ReflectionTypes::Char:
	{
		*(char*)(Output) = *(char*)Source;


		return   { InerRetType() };
	}
	case ReflectionTypes::Uft8:
	{
		*(Utf8*)(Output) = *(Utf8*)Source;


		return   { InerRetType() };
	}
	case ReflectionTypes::Uft16:
	{
		*(Utf16*)(Output) = *(Utf16*)Source;

		return   { InerRetType() };
	}
	case ReflectionTypes::Uft32:
	{
		*(Utf32*)(Output) = *(Utf32*)Source;

		return   { InerRetType() };
	}
	case ReflectionTypes::Bool:
	{
		*(bool*)(Output) = *(bool*)Source;

		return   { InerRetType() };
	}
	break;
	case ReflectionTypes::float32:
	{
		*(float32*)(Output) = *(float32*)Source;

		return   { InerRetType() };
	}
	case ReflectionTypes::float64:
	{
		*(float64*)(Output) = *(float64*)Source;

		return   { InerRetType() };
	}
	case ReflectionTypes::CustomType:
	{


	}

	}
	return {};
}
Optional<Optional<Vector<ClassAssembly::OnMoveConstructorCall>>>  ClassAssembly::CallMoveConstructor(const ReflectionTypeInfo& Type, void* Source, void* Output, bool Is32Bit) const
{
	using InerRetType = Optional<Vector<ClassAssembly::OnMoveConstructorCall>>;
	//std::move for intent
	if (Type.IsAddress())
	{
		*(void**)(Output) = std::move(*(void**)Source);
		return { InerRetType() };
	}

	switch (Type._Type)
	{
	case ReflectionTypes::sInt8:
	{
		*(Int8*)(Output) =std::move(*(Int8*)Source);

		return   { InerRetType() };
	}
	break;
	case ReflectionTypes::sInt16:
	{
		*(Int16*)(Output) = std::move(*(Int16*)Source);

		return   { InerRetType() };
	}
	break;
	case ReflectionTypes::sInt32:
	{
		*(Int32*)(Output) = std::move(*(Int32*)Source);

		return   { InerRetType() };
	}
	break;
	case ReflectionTypes::sInt64:
	{
		*(Int64*)(Output) = std::move(*(Int64*)Source);

		return   { InerRetType() };
	}
	break;
	case ReflectionTypes::uInt8:
	{
		*(UInt8*)(Output) = std::move(*(UInt8*)Source);

		return   { InerRetType() };
	}
	break;
	case ReflectionTypes::uInt16:
	{
		*(UInt16*)(Output) = std::move(*(UInt16*)Source);


		return   { InerRetType() };
	}
	break;
	case ReflectionTypes::uInt32:
	{
		*(UInt32*)(Output) = std::move(*(UInt32*)Source);


		return   { InerRetType() };
	}
	break;
	case ReflectionTypes::uInt64:
	{
		*(UInt64*)(Output) = std::move(*(UInt64*)Source);


		return   { InerRetType() };
	}
	break;
	case ReflectionTypes::Char:
	{
		*(char*)(Output) = std::move(*(char*)Source);


		return   { InerRetType() };
	}
	case ReflectionTypes::Uft8:
	{
		*(Utf8*)(Output) = std::move(*(Utf8*)Source);
		

		return   { InerRetType() };
	}
	case ReflectionTypes::Uft16:
	{
		*(Utf16*)(Output) = std::move(*(Utf16*)Source);

		return   { InerRetType() };
	}
	case ReflectionTypes::Uft32:
	{
		*(Utf32*)(Output) = std::move(*(Utf32*)Source);

		return   { InerRetType() };
	}
	case ReflectionTypes::Bool:
	{
		*(bool*)(Output) = std::move(*(bool*)Source);

		return   { InerRetType() };
	}
	break;
	case ReflectionTypes::float32:
	{
		*(float32*)(Output) = std::move(*(float32*)Source);
		
		return   { InerRetType() };
	}
	case ReflectionTypes::float64:
	{
		*(float64*)(Output) = std::move(*(float64*)Source);

		return   { InerRetType() };
	}
	case ReflectionTypes::CustomType:
	{


	}

	}
	return {};
}
Optional<Optional<Vector<ClassAssembly::OnDoDefaultConstructorCall>>> ClassAssembly::CallDestructor(const ReflectionTypeInfo& Type, void* Object, bool Is32Bit) const
{
	using InerRetType = Optional<Vector<ClassAssembly::OnDoDefaultConstructorCall>>;

	return {};
}
Optional<ClassAssembly::ParsedValue> ClassAssembly::ParseToValue(const String_view txt, const ClassAssembly& Assembly, Vector<ReflectionTypeInfo> Hints)
{
	return {};
#ifndef UCodeLangNoCompiler
	if (txt.size()) 
	{
		if (txt == "true" || txt == "false")
		{
			ParsedValue r;
			r.Value._Type = ReflectionTypes::Bool;
			r.Value._Data.Resize(sizeof(bool));
			*r.Value._Data.Get_DataAs<bool>() = bool(txt == "true");
			return r;
		}
		else if (LexerHelper::IsDigit(txt.front()))
		{
			ParsedValue r;
			r.Value._Type = ReflectionTypes::sInt32;
			r.Value._Data.Resize(sizeof(Int32));
			if (ParseHelper::ParseStringToInt32(txt, *r.Value._Data.Get_DataAs<Int32>()))
			{
				return r;
			}
		}
		else if (txt.front() == '\"' && txt.back() == '\'' && txt.size() > 1)
		{
			UCodeLangToDo();//add stringspan,string,path,pathspan,string8,string16,string32,stringspan8,stringspan16,stringspan32
		}
		else if (txt.front() == '\'' && txt.back() == '\'' && txt.size() > 2)
		{
			//TODO:add uft8,uft16,uft32 based on hints
			
			ParsedValue r;
			r.Value._Type = ReflectionTypes::Char;
			r.Value._Data.Resize(sizeof(char));
			String_view charliteral;
			charliteral = txt.substr(1, txt.size() - 1);
			if (ParseHelper::ParseCharliteralToChar(txt, *r.Value._Data.Get_DataAs<char>()))
			{
				return r;
			}
		}
	}

	return {};
#endif
}
String ClassAssembly::ToString(const ClassMethod::Par& data, const ClassAssembly& Assembly)
{
#ifndef UCodeLangNoCompiler
	return UCodeLang::UAssembly::UAssembly::ToString(data, Assembly);
#endif 
	return "";
}
String ClassAssembly::ToString(const ReflectionTypeInfo& data, const ClassAssembly& Assembly)
{
#ifndef UCodeLangNoCompiler
	return UCodeLang::UAssembly::UAssembly::ToString(data, Assembly);
#endif 
	return "";
}
String ClassAssembly::ToString(const TypedRawReflectionData& data, const ClassAssembly& Assembly, bool is32mode)
{
#ifndef UCodeLangNoCompiler
	return UCodeLang::UAssembly::UAssembly::ToString(data,Assembly, is32mode ? UClib::NTypeSize::int32: UClib::NTypeSize::int64);
#endif 
	return "";
}
String ClassAssembly::ToStringJson(const TypedRawReflectionData& data, const ClassAssembly& Assembly, bool is32mode)
{
	if (data._Type == ReflectionTypes::Void) { return "void"; }
	return {};
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
	using InerRet = Optional<Vector<ClassAssembly::OnMoveConstructorCall>>;
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
				return { InerRet() };
				break;
			case 2:
				*(UInt16*)Output = SrcAsInt.AsUInt16;
				return { InerRet() };
				break;
			case 4:
				*(UInt32*)Output = SrcAsInt.AsUInt32;
				return { InerRet() };
				break;
			case 8:
				*(UInt64*)Output = SrcAsInt.AsUInt64;
				return { InerRet() };
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
				return { InerRet() };
				break;
			case 2:
				*(Int16*)Output = SrcAsInt.AsInt16;
				return { InerRet() };
				break;
			case 4:
				*(Int32*)Output = SrcAsInt.AsInt32;
				return { InerRet() };
				break;
			case 8:
				*(Int64*)Output = SrcAsInt.AsInt64;
				return { InerRet() };
				break;
			default:
				return {};
				break;
			}
		}
	}

	return {};
}


Optional<ClassAssembly::InfoVec2_t> ClassAssembly::IsVec2_t(const ReflectionTypeInfo& Type) const
{
	auto node = Find_Node(Type);
	if (node)
	{
		if (node->Get_Type() == ClassType::Class)
		{
			auto& classnode = node->Get_ClassData();

			if (classnode.Fields.size() == 2)
			{
				if (classnode.Fields[0].Name == "x" ||
					classnode.Fields[0].Name == "X")
				{
					if (classnode.Fields[1].Name == "y" ||
						classnode.Fields[1].Name == "Y")
					{
						InfoVec2_t r;
						r.XAndYType = classnode.Fields[0].Type;
						return r;
					}
				}
			}
		}
	}
	return {};
}


Optional<ClassAssembly::InfoVec3_t> ClassAssembly::IsVec3_t(const ReflectionTypeInfo& Type) const
{
	auto node = Find_Node(Type);
	if (node)
	{
		if (node->Get_Type() == ClassType::Class)
		{
			auto& classnode = node->Get_ClassData();

			if (classnode.Fields.size() == 3)
			{
				if (classnode.Fields[0].Name == "x" ||
					classnode.Fields[0].Name == "X")
				{
					if (classnode.Fields[1].Name == "y" ||
						classnode.Fields[1].Name == "Y")
					{
						if (classnode.Fields[2].Name == "z" ||
							classnode.Fields[2].Name == "Z")
						{
							InfoVec3_t r;
							r.XAndYType = classnode.Fields[0].Type;
							return r;
						}
					}
				}
			}
		}
	}
	return {};
}

bool Contains(String_view ItemTolookfor,String_view Str)
{
	size_t B = 0;
	for (size_t i = 0; i < Str.size(); i++)
	{
		if (Str[i] == ItemTolookfor[B])
		{
			B++;
		}
		else
		{
			B = 0;
		}
		if (B == ItemTolookfor.size()) { return true; }
	}
	return false;
}

Optional<ClassAssembly::InfoVector_t> ClassAssembly::IsVector_t(const ReflectionTypeInfo& Type) const
{
	auto node = Find_Node(Type);
	if (node)
	{
		if (node->Get_Type() == ClassType::Class)
		{
			auto& classnode = node->Get_ClassData();
			if (!Contains(UCode_VectorType, node->FullName))
			{
				return {};
			}
			const ClassMethod* DataMethod = classnode.Get_ClassMethod("data");
			if (DataMethod == nullptr)
			{
				DataMethod = classnode.Get_ClassMethod("Data");
			}

			if (DataMethod)
			{
				auto ElementType = DataMethod->RetType;
				auto SizeMethod = classnode.Get_ClassMethod("Size");
				if (SizeMethod == nullptr)
				{
					SizeMethod = classnode.Get_ClassMethod("size");
				}

				auto CapacityMethod = classnode.Get_ClassMethod("Capacity");
				if (CapacityMethod == nullptr)
				{
					CapacityMethod = classnode.Get_ClassMethod("capacity");
				}

				auto ResizeMethod = classnode.Get_ClassMethod("Resize");
				if (ResizeMethod == nullptr)
				{
					ResizeMethod = classnode.Get_ClassMethod("resize");
				}

				auto ReserveMethod = classnode.Get_ClassMethod("Reserve");
				if (ReserveMethod == nullptr)
				{
					ReserveMethod = classnode.Get_ClassMethod("reserve");
				}

				auto ClearMethod = classnode.Get_ClassMethod("Clear");
				if (ClearMethod == nullptr)
				{
					ClearMethod = classnode.Get_ClassMethod("clear");
				}
				const ClassMethod* PushMovedMethod = nullptr;
				const ClassMethod* PushCopyMethod = nullptr;

				{
					auto list = classnode.Get_ClassMethods("Push");
					for (auto& Item : classnode.Get_ClassMethods("push"))
					{
						list.push_back(Item);
					}
					for (auto& Item : list)
					{
						if (Item->ParsType.size() == 2) 
						{
							auto& ElemPar = Item->ParsType[1];

							if (ElemPar.IsOutPar == false
								&& ElemPar.Type._CustomTypeID == ElementType._CustomTypeID
								&& ElemPar.Type.IsAddressArray() == false)
							{
								
								if (PushMovedMethod == nullptr && ElemPar.Type.IsMovedType())
								{
									PushMovedMethod = Item;
								}
								if (PushCopyMethod == nullptr && ElemPar.Type.IsAddress())
								{
									PushCopyMethod = Item;
								}
							}
						
						}
					}
				}

				auto PopMethod = classnode.Get_ClassMethod("Pop");
				if (PopMethod == nullptr)
				{
					PopMethod = classnode.Get_ClassMethod("pop");
				}

				auto RemoveMethodMethod = classnode.Get_ClassMethod("Remove");
				if (RemoveMethodMethod == nullptr)
				{
					RemoveMethodMethod = classnode.Get_ClassMethod("remove");
				}


				const ClassMethod* InsertMovedMethod = nullptr;
				const ClassMethod* InsertCopyMethod = nullptr;

				{
					auto list = classnode.Get_ClassMethods("Insert");
					for (auto& Item : classnode.Get_ClassMethods("insert"))
					{
						list.push_back(Item);
					}
					for (auto& Item : list)
					{
						if (Item->ParsType.size() == 3)
						{
							if (Item->ParsType[1].Type._Type == ReflectionTypes::uIntPtr) {
								auto& ElemPar = Item->ParsType[2];

								if (ElemPar.IsOutPar == false
									&& ElemPar.Type._CustomTypeID == ElementType._CustomTypeID
									&& ElemPar.Type.IsAddressArray() == false)
								{

									if (InsertMovedMethod == nullptr && ElemPar.Type.IsMovedType())
									{
										InsertMovedMethod = Item;
									}
									if (InsertCopyMethod == nullptr && ElemPar.Type.IsAddress())
									{
										InsertCopyMethod = Item;
									}
								}
							}
						}
					}
				}

				if (SizeMethod 
					&& CapacityMethod
					&& ResizeMethod
					&& ReserveMethod
					&& ClearMethod
					&& PushMovedMethod
					&& PopMethod 
					&& RemoveMethodMethod
					&& InsertMovedMethod)
				{
					InfoVector_t r;
					r.ElementType = DataMethod->RetType;
					r.ElementType._IsAddressArray = false;

					r.Data_Method = DataMethod;
					r.Size_Method = SizeMethod;
					r.Capacity_Method = CapacityMethod;

					r.Resize_Method = ResizeMethod;
					r.Reserve_Method = ReserveMethod;
					r.Clear_Method = ClearMethod;

					r.Push_moved_Method = PushMovedMethod;
					r.Push_copy_Method = PushCopyMethod;
					r.Pop_Method = PopMethod;

					r.Remove_Method = RemoveMethodMethod;

					r.Insert_Moved_Method = InsertMovedMethod;
					r.Insert_Copy_Method = InsertCopyMethod;

					return r;
				}
			}

		}
	}
	return {};
}
Optional<ClassAssembly::InfoString_t> ClassAssembly::IsString_t(const ReflectionTypeInfo& Type) const
{
	auto node = Find_Node(Type);
	if (node)
	{
		if (node->Get_Type() == ClassType::Class)
		{
			auto& classnode = node->Get_ClassData();
			const ClassMethod* DataMethod = classnode.Get_ClassMethod("data");
			if (DataMethod == nullptr)
			{
				DataMethod = classnode.Get_ClassMethod("Data");
			}

			if (DataMethod)
			{
				auto ElementType = DataMethod->RetType;
				auto SizeMethod = classnode.Get_ClassMethod("Size");
				if (SizeMethod == nullptr)
				{
					SizeMethod = classnode.Get_ClassMethod("size");
				}

				auto CapacityMethod = classnode.Get_ClassMethod("Capacity");
				if (CapacityMethod == nullptr)
				{
					CapacityMethod = classnode.Get_ClassMethod("capacity");
				}

				auto ResizeMethod = classnode.Get_ClassMethod("Resize");
				if (ResizeMethod == nullptr)
				{
					ResizeMethod = classnode.Get_ClassMethod("resize");
				}

				auto ReserveMethod = classnode.Get_ClassMethod("Reserve");
				if (ReserveMethod == nullptr)
				{
					ReserveMethod = classnode.Get_ClassMethod("reserve");
				}

				auto ClearMethod = classnode.Get_ClassMethod("Clear");
				if (ClearMethod == nullptr)
				{
					ClearMethod = classnode.Get_ClassMethod("clear");
				}
				const ClassMethod* PushMethod = classnode.Get_ClassMethod("Push");
				if (PushMethod == nullptr)
				{
					PushMethod = classnode.Get_ClassMethod("push");
				}

				auto PopMethod = classnode.Get_ClassMethod("Pop");
				if (PopMethod == nullptr)
				{
					PopMethod = classnode.Get_ClassMethod("pop");
				}

				auto RemoveMethodMethod = classnode.Get_ClassMethod("Remove");
				if (RemoveMethodMethod == nullptr)
				{
					RemoveMethodMethod = classnode.Get_ClassMethod("remove");
				}


				const ClassMethod* InsertMethod = classnode.Get_ClassMethod("Insert");
				if (InsertMethod == nullptr)
				{
					InsertMethod = classnode.Get_ClassMethod("insert");
				}


				if (SizeMethod
					&& CapacityMethod
					&& ResizeMethod
					&& ReserveMethod
					&& ClearMethod
					&& PushMethod
					&& PopMethod
					&& RemoveMethodMethod
					&& InsertMethod)
				{
					InfoString_t r;
					r.ElementType = DataMethod->RetType;
					r.ElementType._IsAddressArray = false;

					r.Data_Method = DataMethod;
					r.Size_Method = SizeMethod;
					r.Capacity_Method = CapacityMethod;

					r.Resize_Method = ResizeMethod;
					r.Reserve_Method = ReserveMethod;
					r.Clear_Method = ClearMethod;

					r.Push_Method = PushMethod;
					r.Pop_Method = PopMethod;

					r.Remove_Method = RemoveMethodMethod;

					r.Insert_Method = InsertMethod;

					return r;
				}
			}

		}
	}
	return {};
}

Optional<ClassAssembly::InfoOptional_t> ClassAssembly::IsOptional_t(const ReflectionTypeInfo& Type) const
{
	return {};
}
Optional<ClassAssembly::InfoResult_t> ClassAssembly::IsResult_t(const ReflectionTypeInfo& Type) const
{
	return {};
}


Optional<ClassAssembly::InfoStringView_t> ClassAssembly::IsStringView_t(const ReflectionTypeInfo& Type) const
{
	return Optional<InfoStringView_t>();
}

Optional<ClassAssembly::InfoSpan_t> ClassAssembly::IsSpan_t(const ReflectionTypeInfo& Type) const
{
	return Optional<InfoSpan_t>();
}

Optional<ClassAssembly::InfoMap_t> ClassAssembly::IsMap_t(const ReflectionTypeInfo& Type) const
{
	return Optional<InfoMap_t>();
}

Optional<ClassAssembly::InfoArray_t> ClassAssembly::IsArray_t(const ReflectionTypeInfo& Type) const
{
	return Optional<InfoArray_t>();
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
	case ClassType::GenericFunction:
		new (&_GenericFunc) GenericFunction_Data();
		break;
	case ClassType::NameSpace:
		new (&_NameSapce) NameSpace_Data();
		break;
	case ClassType::ForType:
		new (&_ForType) ForType_Data();
		break;
	default:
		UCodeLangUnreachable();
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
		new (&_Enum) Enum_Data(std::move(node.Get_EnumData()));
		break;
	case ClassType::Alias:
		new (&_Alias) Alias_Data(std::move(node.Get_AliasData()));
		break;
	case ClassType::Eval:
		new (&_Eval) Eval_Data(std::move(node.Get_EvalData()));
		break;
	case ClassType::Trait:
		new (&_Trait) Trait_Data(std::move(node.Get_TraitData()));
		break;
	case ClassType::Tag:
		new (&_Tag) Tag_Data(std::move(node.Get_TagData()));
		break;
	case ClassType::StaticVarable:
		new (&_StaticVar) StaticVar_Data(std::move(node.Get_StaticVar()));
		break;
	case ClassType::ThreadVarable:
		new (&_ThreadVar) ThreadVar_Data(std::move(node.Get_ThreadVar()));
		break;
	case ClassType::StaticArray:
		new (&_StaticArr) StaticArray_Data(std::move(node.Get_StaticArray()));
		break;
	case ClassType::FuncPtr:
		new (&_FuncPtr) FuncPtr_Data(std::move(node.Get_FuncPtr()));
		break;
	case ClassType::GenericClass:
		new (&_GenericClass) GenericClass_Data(std::move(node.Get_GenericClass()));
		break;
	case ClassType::GenericFunction:
		new (&_GenericFunc) GenericFunction_Data(std::move(node.Get_GenericFunctionData()));
		break;
	case ClassType::NameSpace:
		new (&_NameSapce) NameSpace_Data(std::move(node.Get_NameSpace()));
		break;
	case ClassType::ForType:
		new (&_ForType) ForType_Data(std::move(node.Get_ForType()));
		break;
	default:
		UCodeLangUnreachable();
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
	case ClassType::GenericFunction:
		new (&_GenericFunc) GenericFunction_Data(node.Get_GenericFunctionData());
		break;
	case ClassType::ForType:
		new (&_ForType) ForType_Data(node.Get_ForType());
		break;
	case ClassType::NameSpace:
		new (&_NameSapce) NameSpace_Data(node.Get_NameSpace());
		break;
	default:
		UCodeLangUnreachable();
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
	case ClassType::GenericFunction:
		_GenericFunc.~GenericFunction_Data();
		break;
	case ClassType::NameSpace:
		_NameSapce.~NameSpace_Data();
		break;
	case ClassType::ForType:
		_ForType.~ForType_Data();
		break;
	default:
		UCodeLangUnreachable();
		break;
	}
}
UCodeLangEnd