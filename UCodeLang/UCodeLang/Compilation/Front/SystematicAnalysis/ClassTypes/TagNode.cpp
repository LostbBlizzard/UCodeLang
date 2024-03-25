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

		info->FullName = Syb.FullName;
	}
	Syb.PassState = _PassType;

	TagInfo* info = Syb.Get_Info<TagInfo>();

	ClassStackInfo val;
	val.Info = info;
	val.Syb = &Syb;
	_ClassStack.push(std::move(val));

	bool CheckgenericForErr = (Isgeneric_t && (_PassType == PassType::GetTypes || _PassType == PassType::FixedTypes));
	if (!Isgeneric_t || CheckgenericForErr)
	{
		if (_PassType != PassType::BuidCode)
		{
			if (CheckgenericForErr)
			{
				_Table.AddScope(GenericTestStr);
				Syb.FullName = _Table._Scope.ThisScope;
			}
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

			if (CheckgenericForErr)
			{
				_Table.RemoveScope();
				Syb.FullName = _Table._Scope.ThisScope;
			}
		}
		if (_PassType == PassType::FixedTypes)
		{
			bool ispublic = node._Access == AccessModifierType::Public;
			if (node._IsExport && ispublic)
			{
				for (size_t i = 0; i < info->Fields.size(); i++)
				{
					auto& cfield = info->Fields[i];

					bool ispublic = false;
					const Token* t = nullptr;
					{
						size_t fieldcount = 0;
						for (auto& Item : node._Nodes)
						{
							if (Item->Get_Type() == NodeType::DeclareVariableNode)
							{
								auto* declar = DeclareVariableNode::As(Item.get());
								if (fieldcount == i)
								{
									t = declar->_Name.token;
									ispublic = declar->_Access == AccessModifierType::Public;
									break;
								}
								fieldcount++;
							}
						}
					}

					if (ispublic)
					{
						if (!Type_IsTypeExported(cfield.Type))
						{
							LogError_TypeIsNotExport(NeverNullptr(t), cfield.Type, NeverNullptr(&Syb));
						}
					}
				}
			}
			if (!ispublic && node._IsExport)
			{
				LogError_ExportIsPrivate(NeverNullptr(node._AttributeName.token), NeverNullptr(&Syb));
			}
		}
	}

	if (_PassType == PassType::FixedTypes && Isgeneric_t)
	{
		auto& GenericList = node._generic;
		Generic_GenericAliasFixTypes(GenericList, IsgenericInstantiation, info->_GenericData);
	}

	_ClassStack.pop();

	if (_PassType == PassType::BuidCode)
	{

		if (Syb.Type == SymbolType::Tag_class)
		{
			Tag_Data& TagData = _Lib.Get_Assembly().AddTag(ScopeHelper::GetNameFromFullName(Syb.FullName), Syb.FullName);
			TagData.TypeID = Type_GetTypeID(TypesEnum::CustomType, Syb.ID);

			TagData.Fields.resize(info->Fields.size());
			TagData.AccessModifier = Syb.Access;
			TagData.IsExported = node._IsExport;
		
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
			String_view Text = _LookingAtFile->FileText;

			String ClassStr = "$";
			ClassStr += node._AttributeName.token->Value._String;

			String_view ClassBody =
				String_view(&Text[node._AttributeName.token->OnPos],
					node.EndOfClass->OnPos - node._AttributeName.token->OnPos);

			GenericClass_Data& VClass = _Lib.Get_Assembly().AddGenericClass((String)ScopeHelper::GetNameFromFullName(Syb.FullName), Syb.FullName);

			VClass.Base.Implementation = ClassStr + String(ClassBody);
			VClass.Base.Implementation += "\n\n";
			
		}
	}


	_Table.RemoveScope();
}

UCodeLangFrontEnd

#endif