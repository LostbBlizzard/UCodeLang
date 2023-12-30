#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnAttributeNode(const AttributeNode& node)
{


	if (_PassType == PassType::GetTypes)
	{

	}
	else if (_PassType == PassType::FixedTypes)
	{
		String V;
		node._ScopedName.GetScopedName(V);

		auto SybID = Symbol_GetSymbolID(node);
		auto& Syb = Symbol_AddSymbol(SymbolType::UsedTag, CompilerGeneratedStart + V + CompilerGeneratedEnd, CompilerGeneratedStart + V + CompilerGeneratedEnd, AccessModifierType::Private);
		

		UsedTagInfo* info = new UsedTagInfo();
		Syb.Info.reset(info);

		_Table.AddSymbolID(Syb, SybID);

		auto AttOp = Symbol_GetSymbol(V, SymbolType::Tag_class);
		if (AttOp)
		{
			auto Att = AttOp.value();
			auto token = NeverNullptr(node._ScopedName._ScopedName.back()._token);
			if (Att->Type == SymbolType::Tag_class)
			{
				auto& Generic = *node._ScopedName._ScopedName.back()._generic;
				if (Generic._Values.size())
				{
					LogError_ExpectedSymbolToBea(token, *Att, SymbolType::Generic_Tag);
				}

			}
			else if (Att->Type == SymbolType::Generic_Tag)
			{
				auto& Generic = *node._ScopedName._ScopedName.back()._generic;
				if (Generic._Values.size())
				{
					auto Info = Att->Get_Info<TagInfo>();
					auto TagNode = TagTypeNode::As(Att->Get_NodeInfo<Node>());

					Att = Generic_InstantiateOrFindGeneric_Tag(token, Att, TagNode->_generic, Info->_GenericData, Generic).value();
				}
				else
				{
					LogError_ExpectedSymbolToBea(token, *Att, SymbolType::Tag_class);
				}
			}
			else
			{
				LogError_ExpectedSymbolToBea(token, *Att, SymbolType::Tag_class);
			}
			Syb.VarType = TypeSymbol(Att->ID);
		}
		else
		{
			auto token = NeverNullptr(node._ScopedName._ScopedName.back()._token);
			LogError_CantFindTypeError(token, V);
		}

		if (AttOp.has_value())
		{
			auto Tag = AttOp.value();

			String functocall;
			if (Tag->Type == SymbolType::Generic_Tag)
			{

			}
			else
			{
				functocall = ScopeHelper::ApendedStrings(Tag->FullName, ClassConstructorfunc);
			}
			auto FuncCalls = GetSymbolsWithName(functocall,SymbolType::Func);
			
			
			if (FuncCalls.size() == 0 && node._Parameters._Nodes.size())
			{

			}
			else
			{
				Vector<ParInfo> InferedParInfo;
				for (auto& Item : FuncCalls)
				{
					if (Item->Type == SymbolType::Func)
					{
						FuncInfo* finfo = Item->Get_Info<FuncInfo>();
							
						if (finfo->Pars.size()-1 == node._Parameters._Nodes.size())
						{
							InferedParInfo = finfo->Pars;
						}
					}
				}

				Vector<EvaluatedEx> EvalParsInfo;
				for (size_t i = 0; i < node._Parameters._Nodes.size(); i++)
				{
					auto& Item = node._Parameters._Nodes[i];



					TypeSymbol lookingfor;
					if (i + 1 < InferedParInfo.size())
					{
						lookingfor = InferedParInfo[i + 1].Type;
					}

					auto& ex = EvalParsInfo.emplace_back();

					_LookingForTypes.push(lookingfor);
					OnExpressionNode(*ValueExpressionNode::As(Item.get()));
					
					ex = Eval_MakeEx(_LastExpressionType);

					auto b = Eval_Evaluate_t(ex, Item.get(), GetValueMode::Read);

					_LookingForTypes.pop();

					if (!b)
					{
						return;
					}
				
				}
				
				int a = 10;
				Vector<Symbol> A;
				Optional<NeverNullPtr<Symbol>> FuncToCall;
				
				
				for (auto& Item : FuncCalls)
				{
					bool next = false;

					if (Item->Type == SymbolType::Func)
					{
						FuncInfo* finfo = Item->Get_Info<FuncInfo>();

						if (finfo->Pars.size() - 1 == EvalParsInfo.size())
						{
							for (size_t i = 0; i < EvalParsInfo.size(); i++)
							{
								auto& ItemP = finfo->Pars[i+1];
								auto& ItemV = EvalParsInfo[i];

								if (!Eval_CanEvaluateImplicitConversionConstant(ItemV.Type,ItemP.Type))
								{
									next = true;
									break;
								}
							}

							if (next == false)
							{
								FuncToCall = Item;
							}
						}
					}
				}

				if (FuncToCall.has_value())
				{
					auto& callingfunc = (*FuncToCall);
					FuncInfo* finfo = callingfunc->Get_Info<FuncInfo>();


					EvalFuncData state;

					Vector<EvaluatedEx> Val;
					Val.resize(EvalParsInfo.size() + 1);
				

					auto& thispointertype = *callingfunc->Get_Info<FuncInfo>()->GetObjectForCall();

					EvaluatedEx ThisVal;
					auto classtype = thispointertype;
					classtype._IsAddress = false;

					ThisVal = Eval_MakeEx(classtype);

					EvalSharedState newevalstate;
					Val[0] = Eval_MakeEx(thispointertype);
					
					Eval_Set_ObjectAs(Val[0], newevalstate.GivePointerAccess(&ThisVal));

					_SharedEvalStates.push_back(std::move(newevalstate));

					
					for (size_t i = 0; i < EvalParsInfo.size(); i++)
					{
						auto& Item = EvalParsInfo[i];
						auto& Out = Val[i + 1];

						Out = std::move(Item);


					}


					_Eval_FuncStackFrames.push_back(Unique_ptr<EvalFuncData>(new EvalFuncData(std::move(state))));
					
					Eval_EvalutateFunc(*_Eval_FuncStackFrames.back(), callingfunc, Val);
					
					_Eval_FuncStackFrames.pop_back();

					_SharedEvalStates.pop_back();

					info->RawObj = std::move(ThisVal.EvaluatedObject);
				}
				else
				{
					Vector<ParInfo> p;
					p.resize(node._Parameters._Nodes.size());

					for (size_t i = 0; i < EvalParsInfo.size(); i++)
					{
						 p[i].Type = EvalParsInfo[i].Type;
					}

					LogError_CantFindFuncError(NeverNullptr(node._ScopedName._ScopedName.front()._token), functocall,
						{}, p, {});
				}
			}

		}
	}
	else if (_PassType == PassType::BuidCode)
	{

	}
}
void SystematicAnalysis::OnAttributesNode(const Vector<Unique_ptr<AttributeNode>>& nodes)
{
	for (auto& Item : nodes)
	{
		OnAttributeNode(*Item);
	}
}


UCodeLangFrontEnd

#endif