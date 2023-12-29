#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnTag(const TagTypeNode& node)
{
	bool IsgenericInstantiation = _Generic_GenericSymbolStack.size() && _Generic_GenericSymbolStack.top().NodeTarget == &node;
	bool Isgeneric = node._generic._Values.size();
	bool Isgeneric_t = Isgeneric && IsgenericInstantiation == false;


	const String ClassName = IsgenericInstantiation ? ScopeHelper::GetNameFromFullName(_Generic_GenericSymbolStack.top()._IR_GenericFuncName) : (String)node._AttributeName.token->Value._String;
	_Table.AddScope(ClassName);
	SymbolID sybId = Symbol_GetSymbolID(node);//Must be pass AddScope thats how GetSymbolID works for Generics.

	auto& Syb = _PassType == PassType::GetTypes ?
		Symbol_AddSymbol(Isgeneric_t ? SymbolType::Generic_Tag : SymbolType::Tag_class
			, (String)ClassName, _Table._Scope.ThisScope, node._Access) :
		*Symbol_GetSymbol(sybId);

	//we may jump to this node non linearly
	if (Syb.PassState == _PassType)
	{
		_Table.RemoveScope();
		return;
	}

	if (_PassType == PassType::GetTypes)
	{
		_Table.AddSymbolID(Syb, sybId);
		Syb.NodePtr = node.As();
		
		Syb.VarType = sybId;

		TagInfo* info = new TagInfo();
		Syb.Info.reset(info);
		info->Context = Save_SymbolContextRemoveOneScopeName();
		Generic_InitGenericalias(node._generic, IsgenericInstantiation, info->_GenericData);
	}

	TagInfo* info = Syb.Get_Info<TagInfo>();

	ClassStackInfo val;
	val.Info = info;
	val.Syb = &Syb;
	_ClassStack.push(std::move(val));

	if (!Isgeneric_t)
	{
		if (_PassType != PassType::BuidCode)
		{
			for (const auto& node : node._Nodes)
			{
				Push_ToNodeScope(*node.get());
				switch (node->Get_Type())
				{
				case NodeType::ClassNode: OnClassNode(*ClassNode::As(node.get())); break;
				case NodeType::AliasNode:OnAliasNode(*AliasNode::As(node.get())); break;
				case NodeType::EnumNode:OnEnum(*EnumNode::As(node.get())); break;
				case NodeType::UsingNode: OnUseingNode(*UsingNode::As(node.get())); break;
				case NodeType::FuncNode:OnFuncNode(*FuncNode::As(node.get())); break;
				case NodeType::DeclareVariableNode:OnDeclareVariablenode(*DeclareVariableNode::As(node.get()), DeclareStaticVariableNode_t::ClassField); break;
				case NodeType::DeclareStaticVariableNode:OnDeclareStaticVariableNode(*DeclareStaticVariableNode::As(node.get())); break;
				case NodeType::DeclareThreadVariableNode:OnDeclareThreadVariableNode(*DeclareThreadVariableNode::As(node.get())); break;
				case NodeType::DeclareEvalVariableNode:OnDeclareEvalVariableNode(*DeclareEvalVariableNode::As(node.get())); break;
				case NodeType::TraitNode:OnTrait(*TraitNode::As(node.get())); break;
				case NodeType::TagTypeNode:OnTag(*TagTypeNode::As(node.get())); break;
				default:break;
				}
				Pop_NodeScope();
			}
		}
		if (_PassType == PassType::FixedTypes)
		{

		}
	}


	_ClassStack.pop();

	if (_PassType == PassType::BuidCode)
	{

		if (Syb.Type == SymbolType::Tag_class)
		{
			Tag_Data& TagData = _Lib.Get_Assembly().AddTag(ScopeHelper::GetNameFromFullName(Syb.FullName), Syb.FullName);
			TagData.TypeID = Type_GetTypeID(TypesEnum::CustomType, Syb.ID);

			TagData.Fields.resize(info->Fields.size());
		
			size_t offset = 0;
			for (size_t i = 0; i < info->Fields.size(); i++)
			{
				auto& OutField = TagData.Fields[i];
				auto& InField = info->Fields[i];

				OutField.Name = InField.Name;
				OutField.Type = Assembly_ConvertToType(InField.Type);
			
				OutField.offset = offset;

				offset += Type_GetSize(InField.Type).value();
			}

		}
		else
		{

		}
	}


	_Table.RemoveScope();
}

UCodeLangFrontEnd

#endif