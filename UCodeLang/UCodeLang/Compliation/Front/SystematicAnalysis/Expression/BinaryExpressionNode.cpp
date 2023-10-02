#include "UCodeLang/Compliation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

Byte SystematicAnalysis::OperatorPrecedenceValue(const Node* node)
{
	if (node->Get_Type() == NodeType::ValueExpressionNode)
	{
		const ValueExpressionNode* nod = ValueExpressionNode::As(node);

		if (nod->_Value->Get_Type() == NodeType::ParenthesesExpresionNode)
		{
			return 8;
		}

	}

	if (node->Get_Type() == NodeType::BinaryExpressionNode)
	{
		const BinaryExpressionNode* nod = BinaryExpressionNode::As(node);

		auto V = nod->_BinaryOp->Type;
		return OperatorPrecedence(V);
	}


	return 0;
}

Byte SystematicAnalysis::OperatorPrecedence(TokenType V)
{
	//https://en.cppreference.com/w/c/language/operator_precedence

	//the biger number will have a higher precedence
	switch (V)
	{
	case TokenType::modulo:
	case TokenType::forwardslash:
	case TokenType::star:
		return 6;

	case TokenType::Not:
	case TokenType::bitwise_not:

	case TokenType::plus:
	case TokenType::minus:
	default:
		return 0;
	}
}


bool SystematicAnalysis::Node_SwapForOperatorPrecedence(const Node* nodeA, const Node* nodeB)
{
	return OperatorPrecedenceValue(nodeA) < OperatorPrecedenceValue(nodeB);
}

void SystematicAnalysis::OnExpressionNode(const BinaryExpressionNode& node)
{
	auto Ex0node = node._Value0._Value.get();
	auto Ex1node = node._Value1._Value.get();

	if (_PassType != PassType::GetTypes &&
		(
			Node_SwapForOperatorPrecedence(Ex0node, Ex1node) && Node_SwapForOperatorPrecedence(&node, Ex1node)//i have no clue why this works
			)
		)
	{
		std::swap(Ex0node, Ex1node);
	}



	if (_LookingForTypes.size() && _LookingForTypes.top()._Type != TypesEnum::Var)
	{
		_LookingForTypes.push(_LookingForTypes.top());
	}
	else
	{
		TypeSymbol V; V.SetType(TypesEnum::Any);
		_LookingForTypes.push(V);
	}

	BinaryExpressionNode_Data* Data = nullptr;
	bool BuildCode = _PassType == PassType::BuidCode;
	bool IsBuildFunc = false;
	if (BuildCode)
	{
		Data = &_BinaryExpressionNode_Datas.GetValue(Symbol_GetSymbolID(node));
		_LookingForTypes.top() = Data->Op1;

		if (Data->FuncToCall)
		{
			_LookingForTypes.pop();
			IsBuildFunc = true;
		}
	}


	IRInstruction* Ex0 = nullptr;
	TypeSymbol Ex0Type;

	IRInstruction* Ex1 = nullptr;
	TypeSymbol Ex1Type;
	if (IsBuildFunc == false)
	{
		OnExpressionTypeNode(Ex1node, GetValueMode::Read);
		Ex0 = _IR_LastExpressionField;
		Ex0Type = _LastExpressionType;

		if (BuildCode)
		{
			_LookingForTypes.top() = Data->Op0;
		}

		OnExpressionTypeNode(Ex0node, GetValueMode::Read);
		Ex1 = _IR_LastExpressionField;
		Ex1Type = _LastExpressionType;


		_LookingForTypes.pop();

		if (_PassType == PassType::FixedTypes)
		{
			auto BinaryOp = NeverNullptr(node._BinaryOp);
			auto Info = Type_HasBinaryOverLoadWith(Ex0Type, BinaryOp->Type, Ex1Type);

			if (!Info.HasValue)
			{
				LogError_CantFindBinaryOpForTypes(BinaryOp, Ex0Type, Ex1Type);
			}

			auto Op = node._BinaryOp->Type;


			BinaryExpressionNode_Data V;
			V.Op0 = Ex0Type;
			V.Op1 = Ex1Type;


			//all float bool int types
			if (Info.Value.has_value())
			{
				FuncInfo* f = Info.Value.value()->Get_Info<FuncInfo>();
				V.Op0 = f->Pars[0].Type;
				V.Op1 = f->Pars[1].Type;
				V.FuncToCall = Info.Value.value();

				_LastExpressionType = f->Ret;
			}
			else
			{
				V.Op0._IsAddress = false;
				V.Op1._IsAddress = false;
				_LastExpressionType = Type_BinaryExpressionShouldRurn(Op, Ex0Type);
			}

			_BinaryExpressionNode_Datas.AddValue(Symbol_GetSymbolID(node), V);


		}
	}
	if (_PassType == PassType::BuidCode)
	{
		auto& Data = _BinaryExpressionNode_Datas.GetValue(Symbol_GetSymbolID(node));

		if (Data.FuncToCall)
		{
			FuncInfo* f = Data.FuncToCall->Get_Info<FuncInfo>();


			Get_FuncInfo V;
			V.Func = f;
			V.SymFunc = Data.FuncToCall;
			V.ThisPar = Get_FuncInfo::ThisPar_t::NoThisPar;


			ScopedNameNode Tep;
			ScopedName TepV;
			TepV._token = node._BinaryOp;
			Tep._ScopedName.push_back(TepV);

			ValueParametersNode pars;
			pars._Nodes.push_back(Unique_ptr<Node>(Ex0node));
			pars._Nodes.push_back(Unique_ptr<Node>(Ex1node));

			IR_Build_FuncCall(V, Tep, pars);

			//save so not free mem
			auto par0 = pars._Nodes[0].release();
			auto par1 = pars._Nodes[1].release();
			//its ok.no mem leak Par node has Unique_ptr to Ex0 and Ex1 just borrowing them
			_LastExpressionType = V.Func->Ret;
		}
		else
		{

			auto Type = Ex0Type;
			auto Op = node._BinaryOp->Type;
			_LastExpressionType = Type_BinaryExpressionShouldRurn(Op, Ex0Type);

			auto TypeSybOp = Symbol_GetSymbol(Type);
			if (TypeSybOp)
			{
				auto TypeSyb = TypeSybOp.value();
				if (TypeSyb->Type == SymbolType::Enum)
				{
					Type = TypeSyb->Get_Info<EnumInfo>()->Basetype;
				}
				else
				{
					UCodeLangUnreachable();
				}
			}

#define BindaryBuildU(x) switch (Op) \
		{\
		case TokenType::plus:_IR_LastExpressionField=_IR_LookingAtIRBlock->NewAdd(Ex1, Ex0);break;\
		case TokenType::minus:_IR_LastExpressionField=_IR_LookingAtIRBlock->NewSub(Ex1, Ex0);break;\
		case TokenType::star:_IR_LastExpressionField=_IR_LookingAtIRBlock->NewUMul(Ex1, Ex0);break; \
		case TokenType::forwardslash:_IR_LastExpressionField=_IR_LookingAtIRBlock->NewUDiv(Ex1, Ex0);break; \
		case TokenType::equal_Comparison:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewC_Equalto(Ex1, Ex0); break; \
		case TokenType::Notequal_Comparison:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewC_NotEqualto(Ex1, Ex0);break; \
		case TokenType::greaterthan:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewUGreaterThan(Ex1, Ex0); break; \
		case TokenType::lessthan:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewULessThan(Ex1, Ex0); break; \
		case TokenType::greater_than_or_equalto:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewUGreaterThanOrEqual(Ex1, Ex0); break; \
		case TokenType::less_than_or_equalto:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewULessThanOrEqual(Ex1, Ex0); break; \
		default:\
			UCodeLangUnreachable();\
			break;\
		}\


#define BindaryBuildS(x) switch (Op) \
			{\
			case TokenType::plus:_IR_LastExpressionField=_IR_LookingAtIRBlock->NewAdd(Ex1, Ex0);break;\
			case TokenType::minus:_IR_LastExpressionField=_IR_LookingAtIRBlock->NewSub(Ex1, Ex0);break;\
			case TokenType::star:_IR_LastExpressionField=_IR_LookingAtIRBlock->NewSMul(Ex1, Ex0);break; \
			case TokenType::forwardslash:_IR_LastExpressionField=_IR_LookingAtIRBlock->NewSDiv(Ex1, Ex0);break; \
			case TokenType::equal_Comparison:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewC_Equalto(Ex1, Ex0); break; \
			case TokenType::Notequal_Comparison:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewC_NotEqualto(Ex1, Ex0); break; \
			case TokenType::greaterthan:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewSGreaterThan(Ex1, Ex0); break; \
			case TokenType::lessthan:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewSLessThan(Ex1, Ex0); break; \
			case TokenType::greater_than_or_equalto:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewSGreaterThanOrEqual(Ex1, Ex0); break; \
			case TokenType::less_than_or_equalto:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewSLessThanOrEqual(Ex1, Ex0); break; \
			default:\
				UCodeLangUnreachable(); \
				break; \
			}\

			switch (Type._Type)
			{
			case TypesEnum::uInt8:BindaryBuildU(8); break;
			case TypesEnum::uInt16:BindaryBuildU(16); break;
			case TypesEnum::uInt32:BindaryBuildU(32); break;
			case TypesEnum::uInt64:BindaryBuildU(64); break;


			case TypesEnum::sInt8:BindaryBuildS(8); break;
			case TypesEnum::sInt16:BindaryBuildS(16); break;
			case TypesEnum::sInt32:BindaryBuildS(32); break;
			case TypesEnum::sInt64:BindaryBuildS(64); break;

			case TypesEnum::float32:BindaryBuildS(32); break;
			case TypesEnum::float64:BindaryBuildS(64); break;

			case TypesEnum::uIntPtr:BindaryBuildU(64); break;
			case TypesEnum::sIntPtr:BindaryBuildS(64); break;

			case TypesEnum::Bool:
				switch (Op)
				{
				case TokenType::equal_Comparison:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewC_Equalto(Ex1, Ex0); break;
				case TokenType::Notequal_Comparison:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewC_NotEqualto(Ex1, Ex0); break;
				case TokenType::logical_and:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewlogicalAnd(Ex1, Ex0); break;
				case TokenType::logical_or:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewlogicalOr(Ex1, Ex0); break;
				default:
					UCodeLangUnreachable();
					break;
				}
				break;
			case TypesEnum::Char:
				switch (Op)
				{
				case TokenType::equal_Comparison:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewC_Equalto(Ex1, Ex0); break;
				case TokenType::Notequal_Comparison:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewC_NotEqualto(Ex1, Ex0); break;
				default:
					UCodeLangUnreachable();
					break;
				}
				break;
			default:
				UCodeLangUnreachable();
				break;
			}
		}
	}
}
TypeSymbol SystematicAnalysis::Type_BinaryExpressionShouldRurn(TokenType Op, const TypeSymbol& Ex0Type)
{
	TypeSymbol V;
	switch (Op)
	{
	case TokenType::plus:
	case TokenType::minus:
	case TokenType::star:
	case TokenType::forwardslash:
		V = Ex0Type;
		break;
	case TokenType::logical_and:
	case TokenType::logical_or:
	case TokenType::equal_Comparison:
	case TokenType::Notequal_Comparison:
	case TokenType::lessthan:
	case TokenType::greaterthan:
	case TokenType::less_than_or_equalto:
	case TokenType::greater_than_or_equalto:
		V.SetType(TypesEnum::Bool);
		break;
	default:
		V.SetType(TypesEnum::Void);
		break;
	}
	return V;
}
UCodeLangFrontEnd