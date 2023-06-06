#include "ReflectionData.hpp"
UCodeLangStart
void ClassAssembly::PushCopyClasses(const ClassAssembly& source, ClassAssembly& Out)
{
	for (auto& Item : source.Classes)
	{
		Out.Classes.push_back(std::make_unique<AssemblyNode>(*Item));
	}
}
AssemblyNode::AssemblyNode(ClassType type) : Type(type)
{
	switch (type)
	{
	case ClassType::Null:
		break;
	case ClassType::Class:
		_Class = Class_Data();
		break;
	case ClassType::Enum:
		_Enum = Enum_Data();
		break;
	case ClassType::Alias:
		_Alias = Alias_Data();
		break;
	case ClassType::Eval:
		_Eval = Eval_Data();
		break;
	case ClassType::Trait:
		_Trait = Trait_Data();
		break;
	case ClassType::Tag:
		_Tag = Tag_Data();
		break;
	case ClassType::StaticVarable:
		_StaticVar = StaticVar_Data();
		break;
	case ClassType::ThreadVarable:
		_ThreadVar = ThreadVar_Data();
		break;
	case ClassType::StaticArray:
		_StaticArr = StaticArray_Data();
		break;
	case ClassType::FuncPtr:
		_FuncPtr = FuncPtr_Data();
		break;
	case ClassType::GenericClass:
		_GenericClass = GenericClass_Data();
		break;
	case ClassType::GenericFuncion:
		_GenericFunc = GenericFuncion_Data();
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

	switch (node.Type)
	{
	case ClassType::Null:
		break;
	case ClassType::Class:
		_Class = std::move(node.Get_ClassData());
		break;
	case ClassType::Enum:
		_Enum = std::move(node.Get_EnumData());
		break;
	case ClassType::Alias:
		_Alias = std::move(node.Get_AliasData());
		break;
	case ClassType::Eval:
		_Eval = std::move(node.Get_EvalData());
		break;
	case ClassType::Trait:
		_Trait = std::move(node.Get_TraitData());
		break;
	case ClassType::Tag:
		_Tag = std::move(node.Get_TagData());
		break;
	case ClassType::StaticVarable:
		_StaticVar = std::move(node.Get_StaticVar());
		break;
	case ClassType::ThreadVarable:
		_ThreadVar = std::move(node.Get_ThreadVar());
		break;
	case ClassType::StaticArray:
		_StaticArr = std::move(node.Get_StaticArray());
		break;
	case ClassType::FuncPtr:
		_FuncPtr = std::move(node.Get_FuncPtr());
		break;
	case ClassType::GenericClass:
		_GenericClass = std::move(node.Get_GenericClass());
		break;
	case ClassType::GenericFuncion:
		_GenericFunc = std::move(node.Get_GenericFuncionData());
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

	switch (node.Type)
	{
	case ClassType::Class:
		_Class = node.Get_ClassData();
		break;
	case ClassType::Enum:
		_Enum = node.Get_EnumData();
		break;
	case ClassType::Alias:
		_Alias =node.Get_AliasData();
		break;
	case ClassType::Eval:
		_Eval = node.Get_EvalData();
		break;
	case ClassType::Trait:
		_Trait = node.Get_TraitData();
		break;
	case ClassType::Tag:
		_Tag = node.Get_TagData();
		break;
	case ClassType::StaticVarable:
		_StaticVar = node.Get_StaticVar();
		break;
	case ClassType::ThreadVarable:
		_ThreadVar = node.Get_ThreadVar();
		break;
	case ClassType::StaticArray:
		_StaticArr = node.Get_StaticArray();
		break;
	case ClassType::FuncPtr:
		_FuncPtr = node.Get_FuncPtr();
		break;
	case ClassType::GenericClass:
		_GenericClass = node.Get_GenericClass();
		break;
	case ClassType::GenericFuncion:
		_GenericFunc =node.Get_GenericFuncionData();
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