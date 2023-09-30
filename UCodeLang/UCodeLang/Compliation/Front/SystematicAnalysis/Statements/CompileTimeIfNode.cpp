#include "UCodeLang/Compliation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnCompileTimeIfNode(const CompileTimeIfNode& node, bool IsInFunc)
{
	auto FuncNodeCall = [this](Node* Item)
	{
		switch (Item->Get_Type())
		{
		case NodeType::FuncNode:
			OnFuncNode(*FuncNode::As(Item));
			break;
		case NodeType::AliasNode:
			OnAliasNode(*AliasNode::As(Item));
			break;
		case NodeType::EnumNode:
			OnEnum(*EnumNode::As(Item));
			break;
		case NodeType::ClassNode:
			OnClassNode(*ClassNode::As(Item));
			break;
		default:
			UCodeLangUnreachable();
			break;
		}

	};

	if (_PassType == PassType::GetTypes)
	{
		_LookingForTypes.push(TypesEnum::Bool);
		OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);
		_LookingForTypes.pop();

		if (IsInFunc == false)
		{
			Vector<String_view> PossibleSymbolNames;

			for (auto& Item : node._Body._Nodes)
			{
				String_view SymbolName;


				switch (Item->Get_Type())
				{
				case NodeType::FuncNode:
					SymbolName = FuncNode::As(Item.get())->_Signature._Name.token->Value._String;
					break;
				case NodeType::AliasNode:
					SymbolName = AliasNode::As(Item.get())->_AliasName.token->Value._String;
					break;
				case NodeType::EnumNode:
					SymbolName = EnumNode::As(Item.get())->_EnumName.token->Value._String;
					break;
				case NodeType::ClassNode:
					SymbolName = ClassNode::As(Item.get())->_className.token->Value._String;
					break;
				default:
					UCodeLangUnreachable();
					break;
				}

				PossibleSymbolNames.push_back(SymbolName);
			}
			Test info;
			info.node = &node;
			info.Context = Save_SymbolContext();
			info._SymID = Symbol_GetSymbolID(node);

			Test1 M;
			M.PossibleSymbolNames = std::move(PossibleSymbolNames);
			M.node = std::move(info);


			NodeCompileTimeIfs.push_back(std::move(M));
		}
	}
	else if (_PassType == PassType::FixedTypes)
	{
		bool hasevalthis = true;
		if (IsInFunc == false)
		{
			auto SymID = Symbol_GetSymbolID(node);
			for (size_t i = 0; i < NodeCompileTimeIfs.size(); i++)
			{
				auto& Item = NodeCompileTimeIfs[i];

				if (Item.node._SymID == SymID)
				{
					NodeCompileTimeIfs.erase(NodeCompileTimeIfs.begin() + i);
					hasevalthis = false;
					break;
				}
			}
		}
		else
		{
			hasevalthis = false;
		}

		if (hasevalthis == false)
		{
			_LookingForTypes.push(TypesEnum::Bool);
			auto BoolValue = Eval_Evaluate(TypesEnum::Bool, node._Expression);
			_LookingForTypes.pop();



			if (BoolValue.has_value())
			{
				const bool& EvalValue = *Eval_Get_ObjectAs<bool>(BoolValue.value());


				if (EvalValue)
				{
					auto PassOld = _PassType;
					_PassType = PassType::GetTypes;

					if (IsInFunc)
					{
						for (auto& Item : node._Body._Nodes)
						{
							OnStatement(*Item.get());
						}
					}
					else
					{
						for (auto& Item : node._Body._Nodes)
						{
							FuncNodeCall(Item.get());
						}
					}
					_PassType = PassType::FixedTypes;
					if (IsInFunc)
					{
						for (auto& Item : node._Body._Nodes)
						{
							OnStatement(*Item.get());
						}
					}
					else
					{
						for (auto& Item : node._Body._Nodes)
						{
							FuncNodeCall(Item.get());
						}
					}

					_PassType = PassOld;
				}
				else if (node._Else.get())
				{
					ElseNode* Elsenode = ElseNode::As(node._Else.get());

					auto PassOld = _PassType;
					_PassType = PassType::GetTypes;

					if (IsInFunc)
					{
						for (const auto& node3 : Elsenode->_Body._Nodes)
						{
							OnStatement(*node3);
						}
					}
					else
					{
						for (auto& Item : Elsenode->_Body._Nodes)
						{
							FuncNodeCall(Item.get());
						}
					}

					_PassType = PassType::FixedTypes;

					if (IsInFunc)
					{
						for (const auto& node3 : Elsenode->_Body._Nodes)
						{
							OnStatement(*node3);
						}
					}
					else
					{
						for (auto& Item : Elsenode->_Body._Nodes)
						{
							FuncNodeCall(Item.get());
						}
					}

					_PassType = PassOld;

				}

				_ValidNodes.AddValue(Symbol_GetSymbolID(node), EvalValue);

			}

		}
	}
	else if (_PassType == PassType::BuidCode)
	{
		const bool EvalValue = _ValidNodes.at(Symbol_GetSymbolID(node));

		if (EvalValue)
		{
			if (IsInFunc)
			{
				for (auto& Item : node._Body._Nodes)
				{
					OnStatement(*Item.get());
				}
			}
			else
			{
				for (auto& Item : node._Body._Nodes)
				{
					FuncNodeCall(Item.get());
				}
			}
		}
		else if (node._Else.get())
		{
			ElseNode* Elsenode = ElseNode::As(node._Else.get());
			if (IsInFunc)
			{
				for (const auto& node3 : Elsenode->_Body._Nodes)
				{
					OnStatement(*node3);
				}
			}
			else
			{
				for (auto& Item : Elsenode->_Body._Nodes)
				{
					FuncNodeCall(Item.get());
				}
			}
		}
	}
}

UCodeLangFrontEnd