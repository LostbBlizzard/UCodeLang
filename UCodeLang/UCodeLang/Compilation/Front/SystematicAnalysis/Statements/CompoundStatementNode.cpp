#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnCompoundStatementNode(const CompoundStatementNode& node)
{

	if (_PassType == PassType::GetTypes)
	{
		OnExpressionTypeNode(node._ToAssign._Value.get(), GetValueMode::ReadAndWrite);
		OnExpressionTypeNode(node._Expession._Value.get(), GetValueMode::Read);
		return;
	}
	else if (_PassType == PassType::FixedTypes)
	{
		_LookingForTypes.push(TypesEnum::Any);
		OnExpressionTypeNode(node._ToAssign._Value.get(), GetValueMode::ReadAndWrite);
		_LookingForTypes.pop();

		TypeSymbol ToAssignType = _LastExpressionType;

		_LookingForTypes.push(ToAssignType);
		OnExpressionTypeNode(node._Expession._Value.get(), GetValueMode::Read);
		_LookingForTypes.pop();

		auto ExType = _LastExpressionType;


		auto HasInfo = Type_HasCompoundOverLoadWith(ToAssignType, node._CompoundOp->Type, ExType);
		if (!HasInfo.HasValue)
		{
			LogError_CantFindCompoundOpForTypes(NeverNullptr(node._CompoundOp), ToAssignType, ExType);
		}

		CompoundExpresion_Data r;

		if (HasInfo.Value)
		{
			FuncInfo* f = HasInfo.Value.value()->Get_Info<FuncInfo>();
			r.FuncToCall = HasInfo.Value.value();
			r.Op0 = f->Pars[0].Type;
			r.Op1 = f->Pars[1].Type;
		}
		else
		{
			r.Op0 = ToAssignType;
			r.Op1 = ExType;

			r.Op0._IsAddress = false;
			r.Op1._IsAddress = false;
		}

		_Compound_Datas.AddValue(Symbol_GetSymbolID(node), r);
	}
	else if (_PassType == PassType::BuidCode)
	{
		const auto& Data = _Compound_Datas.GetValue(Symbol_GetSymbolID(node));

		if (Data.FuncToCall)
		{

			FuncInfo* f = Data.FuncToCall->Get_Info<FuncInfo>();

			Get_FuncInfo V;
			V.Func = f;
			V.SymFunc = Data.FuncToCall;
			V.ThisPar = Get_FuncInfo::ThisPar_t::NoThisPar;


			ScopedNameNode Tep;
			ScopedName TepV;
			TepV._token = _LastLookedAtToken.value().value();
			Tep._ScopedName.push_back(TepV);

			ValueParametersNode pars;
			pars._Nodes.push_back(Unique_ptr<Node>(node._ToAssign._Value.get()));
			pars._Nodes.push_back(Unique_ptr<Node>(node._Expession._Value.get()));


			IR_Build_FuncCall(V, Tep, pars);

			pars._Nodes[0].release();
			pars._Nodes[1].release();
			//no mem leak node as a Unique_ptr to Ex

			_LastExpressionType = V.Func->Ret;
		}
		else
		{

			_LookingForTypes.push(Data.Op0);

			OnExpressionTypeNode(node._ToAssign._Value.get(), GetValueMode::ReadAndWrite);
			auto AssignType = _LastExpressionType;

			_LookingForTypes.pop();

			IRInstruction* LoadV = _IR_LastExpressionField;

			_LookingForTypes.push(Data.Op1);

			OnExpressionTypeNode(node._Expession._Value.get(), GetValueMode::Read);

			_LookingForTypes.pop();


			switch (node._CompoundOp->Type)
			{
			case TokenType::CompoundAdd:
				_IR_LastExpressionField = _IR_LookingAtIRBlock->NewAdd(LoadV, _IR_LastExpressionField);
				break;
			case TokenType::CompoundSub:
				_IR_LastExpressionField = _IR_LookingAtIRBlock->NewSub(LoadV, _IR_LastExpressionField);
				break;
			case TokenType::CompoundMult:
				if (Type_IsUIntType(Data.Op0)) {
					_IR_LastExpressionField = _IR_LookingAtIRBlock->NewUMul(LoadV, _IR_LastExpressionField);
				}
				else
				{
					_IR_LastExpressionField = _IR_LookingAtIRBlock->NewSMul(LoadV, _IR_LastExpressionField);
				}
				break;
			case TokenType::CompoundDiv:
				if (Type_IsUIntType(Data.Op0)) {
					_IR_LastExpressionField = _IR_LookingAtIRBlock->NewUDiv(LoadV, _IR_LastExpressionField);
				}
				else
				{
					_IR_LastExpressionField = _IR_LookingAtIRBlock->NewSMul(LoadV, _IR_LastExpressionField);
				}
				break;
			default:
				UCodeLangUnreachable();
				break;
			}

			IR_WriteTo(_IR_LastExpressionField, AssignType, _IR_LastStoreField);
		}
	}
}


UCodeLangFrontEnd