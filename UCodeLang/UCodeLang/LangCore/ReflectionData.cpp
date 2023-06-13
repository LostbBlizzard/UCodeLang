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
		throw std::exception("bad path");
		break;
	}
}
UCodeLangEnd