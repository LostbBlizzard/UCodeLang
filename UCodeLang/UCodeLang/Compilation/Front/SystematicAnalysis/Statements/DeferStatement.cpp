#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

//how defer is as a Lambda that just so happened to have its destructor have the statements
void SystematicAnalysis::OnDeferStatement(const DeferStatementNode& node)
{
	const String LambdaName = CompilerGenerated("DeferLambda") + std::to_string((uintptr_t)&node);


	if (_PassType == PassType::GetTypes)
	{
		auto& LambdaSym = Symbol_AddSymbol(SymbolType::LambdaObject, LambdaName, _Table._Scope.ThisScope, AccessModifierType::Public);
		_Table.AddSymbolID(LambdaSym, Symbol_GetSymbolID(node));
		LambdaSym.Info = Unique_ptr<LambdaInfo>(new LambdaInfo());


		for (auto& Item : node._Base._Nodes)
		{
			if (Item->Get_Type() == NodeType::RetStatementNode)
			{
				RetStatementNode* node = RetStatementNode::As(Item.get());
				NeverNullPtr<Token> token = _LastLookedAtToken.value();

				this->LogError(ErrorCodes::ExpectingToken, "Cant use return statement in Defer Statement.", token);
			}
			else
			{
				OnStatement(*Item);
			}
		}
	}
	else if (_PassType == PassType::FixedTypes)
	{
		auto& LambdaSym = *Symbol_GetSymbol(Symbol_GetSymbolID(node));
		LambdaInfo* Info = LambdaSym.Get_Info<LambdaInfo>();

		{
			VarableUseData Data;
			_Varable.push(std::move(Data));
		}

		for (auto& Item : node._Base._Nodes)
		{
			OnStatement(*Item);
		}

		{
			VarableUseData& UseData = _Varable.top();

			for (auto& Item : UseData._UsedSymbols)
			{
				LambdaFieldInfo NewValue;
				NewValue.Name = ScopeHelper::GetNameFromFullName(Item->FullName);
				NewValue.Type = Item->VarType;
				NewValue.Sym = Item;

				Info->_CapturedVarables.push_back(std::move(NewValue));
			}

			int a = 10;
		}
	}
	else if (_PassType == PassType::BuidCode)
	{
		auto& LambdaSym = *Symbol_GetSymbol(Symbol_GetSymbolID(node));
		LambdaInfo* Info = LambdaSym.Get_Info<LambdaInfo>();

		for (auto& Item : node._Base._Nodes)
		{
			OnStatement(*Item);
		}//only for tests shold be updated
	}
}

UCodeLangFrontEnd

#endif