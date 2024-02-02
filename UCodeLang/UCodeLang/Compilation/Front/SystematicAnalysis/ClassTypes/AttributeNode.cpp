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
					//LogError_ExpectedSymbolToBea(token, *Att, SymbolType::Tag_class);
				}
			}
			else
			{
				LogError_ExpectedSymbolToBea(token, *Att, SymbolType::Tag_class);
			}
		}
		else
		{
			auto token = NeverNullptr(node._ScopedName._ScopedName.back()._token);
			LogError_CantFindTypeError(token, V);
		}

		if (AttOp.has_value())
		{
			auto Tag = AttOp.value();

			String functocall = ScopeHelper::ApendedStrings(Tag->FullName, ClassConstructorfunc);
			
			auto FuncCalls = GetSymbolsWithName(functocall,SymbolType::Func);
			
			
			if (FuncCalls.size() == 0 && node._Parameters._Nodes.size()==0)
			{
				Syb.VarType = TypeSymbol(AttOp.value()->ID);
			}
			else
			{
				_LookingForTypes.push(TypesEnum::Void);
				auto f = Type_GetFunc(Tag->VarType, node._Parameters);
				_LookingForTypes.pop();


				Vector<EvaluatedEx> EvalParsInfo;
				Optional<NeverNullPtr<Symbol>> FuncToCall;
				if (f.Func)
				{
					Syb.VarType = f.Func->Pars[0].Type;


					FuncToCall = NeverNullptr(f.SymFunc);

					EvalParsInfo.resize(f.Func->Pars.size()-1);

					for (size_t i = 0; i < node._Parameters._Nodes.size(); i++)
					{
						auto& parnode = node._Parameters._Nodes[i];
						auto& type = f.Func->Pars[i+1];

						ExpressionNodeType V;
						V._Value.reset(parnode.get());

						_LookingForTypes.push(type.Type);
						auto op = Eval_Evaluate(type.Type,V);
						_LookingForTypes.pop();

						V._Value.release();


						if (op.has_value())
						{
							EvalParsInfo[i] = op.value();
						}
						else
						{
							return;
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