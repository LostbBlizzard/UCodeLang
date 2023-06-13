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
	return {};
}
AssemblyNode* ClassAssembly::Find_Node(ReflectionCustomTypeID TypeID)
{

	return {};
}
const AssemblyNode* ClassAssembly::Find_Node(const ReflectionTypes& Type) const
{
	return nullptr;
}
AssemblyNode* ClassAssembly::Find_Node(const ReflectionTypes& Type)
{
	return nullptr;
}
size_t ClassAssembly::GetSize(const ReflectionTypes& Type) const
{
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