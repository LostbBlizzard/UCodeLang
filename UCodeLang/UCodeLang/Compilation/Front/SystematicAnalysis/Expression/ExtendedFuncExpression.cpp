#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnExpressionNode(const ExtendedFuncExpression& node)
{
	if (_PassType == PassType::GetTypes)
	{
		_LookingForTypes.push(TypesEnum::Any);
		OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);
		_LookingForTypes.pop();
	}
	if (_PassType == PassType::FixedTypes)
	{
		_LookingForTypes.push(TypesEnum::Any);
		OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);
		_LookingForTypes.pop();

		auto ExpressionType = _LastExpressionType;

		_GetExpressionMode.push(_GetExpressionMode.top());
		{

			TypeSymbol TypeToStart = ExtendedFuncExpressionGetTypeToStart(ExpressionType, node);

			//Boring boiler plate for Tep ScopedNameNode should be the same in  Evaluate(EvaluatedEx& Out, const ExtendedFuncExpression& node)
			const Token& ToGetLinesFrom = *node._Extended._FuncName._ScopedName.begin()->_token;

			ScopedNameNode Name;

			ScopedName TepV;


			Token TepToken;

			TepToken.OnLine = ToGetLinesFrom.OnLine;
			TepToken.OnPos = ToGetLinesFrom.OnPos;
			TepToken.Type = TokenType::Name;

			Type_RemoveTypeattributes(TypeToStart);

			String Buffer = ToString(TypeToStart);

			TepToken.Value._String = Buffer;

			TepV._token = &TepToken;


			TepV._operator = ScopedName::Operator_t::ScopeResolution;
			Name._ScopedName.push_back(std::move(TepV));




			{

				{
					auto Copy = node._Extended._FuncName._ScopedName.back();
					Name._ScopedName.push_back(std::move(Copy));
				}
				for (size_t i = 1; i < node._Extended._FuncName._ScopedName.size(); i++)
				{
					auto& Item = node._Extended._FuncName._ScopedName[i];
					auto Copy = Item;
					Name._ScopedName.push_back(std::move(Copy));
				}
			}
			//

			ValueParametersNode Pars;
			Pars._Nodes.push_back(Unique_ptr<Node>(node._Expression._Value.get()));

			for (size_t i = 0; i < node._Extended.Parameters._Nodes.size(); i++)
			{
				auto& Item = node._Extended.Parameters._Nodes[i];
				Pars._Nodes.push_back(Unique_ptr<Node>(Item.get()));
			}

			auto FuncInfo = Type_GetFunc(Name, Pars, Type_Get_LookingForType());


			for (auto& Item : Pars._Nodes)
			{
				auto Node = Item.release();//is ok it was borrwed.
			}


			_FuncToSyboID.AddValue(Symbol_GetSymbolID(node), FuncInfo);


			Type_SetFuncRetAsLastEx(FuncInfo);
		}
		_GetExpressionMode.pop();

	}
	if (_PassType == PassType::BuidCode)
	{
		auto& FuncInfo = _FuncToSyboID.GetValue(Symbol_GetSymbolID(node));



		ValueParametersNode Pars;
		Pars._Nodes.push_back(Unique_ptr<Node>(node._Expression._Value.get()));

		for (size_t i = 0; i < node._Extended.Parameters._Nodes.size(); i++)
		{
			auto& Item = node._Extended.Parameters._Nodes[i];
			Pars._Nodes.push_back(Unique_ptr<Node>(Item.get()));
		}
		IR_Build_FuncCall(FuncInfo, node._Extended._FuncName, Pars);


		for (auto& Item : Pars._Nodes)
		{
			auto Node = Item.release();//is ok it was borrwed.
		}
	}
}
TypeSymbol SystematicAnalysis::ExtendedFuncExpressionGetTypeToStart(const TypeSymbol& ExpressionType, const ExtendedFuncExpression& node)
{

	if (node._Operator == ScopedName::Operator_t::Dot)
	{
		return ExpressionType;
	}
	else
	{
		return TypesEnum::Null;
	}

}

UCodeLangFrontEnd

#endif