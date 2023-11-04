#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnExpressionNode(const ExtendedScopeExpression& node)
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

		GetMemberTypeSymbolFromVar_t V;


		_GetExpressionMode.push(_GetExpressionMode.top());
		{
			V.Type = ExpressionType;
			V._Symbol = Symbol_GetSymbol(ExpressionType).value_unchecked();


			Symbol_StepGetMemberTypeSymbolFromVar(node._Extended, 0, node._Operator, V);

			for (size_t i = 1; i < node._Extended._ScopedName.size(); i++)
			{
				Symbol_StepGetMemberTypeSymbolFromVar(node._Extended, i, node._Extended._ScopedName[i]._operator, V);
			}
		}
		_GetExpressionMode.pop();

		_LastExpressionType = V.Type;
	}
	if (_PassType == PassType::BuidCode)
	{
		_LookingForTypes.push(TypesEnum::Any);
		OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);
		_LookingForTypes.pop();


		auto ExpressionType = _LastExpressionType;
		auto ExpressionIR = _IR_LastExpressionField;

		GetMemberTypeSymbolFromVar_t V;
		V.Start = node._Extended._ScopedName.data();
		V.End = node._Extended._ScopedName.size();

		_GetExpressionMode.push(_GetExpressionMode.top());
		{
			V.Type = ExpressionType;
			V._Symbol = Symbol_GetSymbol(ExpressionType).value_unchecked();


			Symbol_StepGetMemberTypeSymbolFromVar(node._Extended, 0, node._Operator, V);

			for (size_t i = 1; i < node._Extended._ScopedName.size(); i++)
			{
				Symbol_StepGetMemberTypeSymbolFromVar(node._Extended, i, node._Extended._ScopedName[i]._operator, V);
			}
		}
		_GetExpressionMode.pop();

		auto GetValue = _GetExpressionMode.top();

		IRInstruction* VPtr = ExpressionIR;



		StepBuildMember_Access(node._Extended._ScopedName[0], ExpressionType, node._Operator, V, VPtr);
		for (size_t i = 1; i < node._Extended._ScopedName.size(); i++)
		{
			StepBuildMember_Access(node._Extended._ScopedName[i], ExpressionType, node._Operator, V, VPtr);
		}

		if (IsRead(GetValue))
		{
			_IR_LastExpressionField = VPtr;
		}

		if (IsWrite(GetValue))
		{
			_IR_LastStoreField = VPtr;
		}


		_LastExpressionType = V.Type;
	}
}


UCodeLangFrontEnd