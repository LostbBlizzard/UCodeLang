#include "UCodeLang/Compilation/Helpers/CompilerTypes.hpp"
#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart


void SystematicAnalysis::OnExpressionNode(const BinaryExpressionNode& node)
{
	auto Ex0node = node._Value0._Value.get();
	auto Ex1node = node._Value1._Value.get();

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

	IRInstruction* lazylogicalandret = nullptr;
	bool islazylogicaland = node._BinaryOp->Type == TokenType::logical_and;
	IRInstruction* Ex0 = nullptr;
	TypeSymbol Ex0Type;

	IRInstruction* Ex1 = nullptr;
	TypeSymbol Ex1Type;
	if (IsBuildFunc == false)
	{
		IRBlock::NewConditionalFalseJump_t lazyifjump = {};
		IRInstruction* lazyjumpiftrue = nullptr;

		OnExpressionTypeNode(Ex0node, GetValueMode::Read);
		Ex0 = _IR_LastExpressionField;
		Ex0Type = _LastExpressionType;
	
		if (islazylogicaland && BuildCode)
		{
			lazylogicalandret =_IR_LookingAtIRBlock->NewLoad(IRTypes::Bool);

			lazyifjump = _IR_LookingAtIRBlock->NewConditionalFalseJump(Ex0,0);
		
			_IR_LookingAtIRBlock->NewStore(lazylogicalandret,Ex0);
			
			lazyjumpiftrue =_IR_LookingAtIRBlock->NewJump(0);

			_IR_LookingAtIRBlock->UpdateConditionaJump(lazyifjump.ConditionalJump,lazyifjump.logicalNot, _IR_LookingAtIRBlock->GetIndex());
		}

		if (BuildCode)
		{
			_LookingForTypes.top() = Data->Op0;
		}
		else
		{
			_LookingForTypes.top() = _LastExpressionType;
		}

		OnExpressionTypeNode(Ex1node, GetValueMode::Read);
		Ex1 = _IR_LastExpressionField;
		Ex1Type = _LastExpressionType;

		_LookingForTypes.pop();

		if (islazylogicaland && BuildCode)
		{
			
			_IR_LookingAtIRBlock->NewStore(lazylogicalandret,Ex1);

			_IR_LookingAtIRBlock->UpdateJump(lazyjumpiftrue,_IR_LookingAtIRBlock->GetIndex());
		}

		if (_PassType == PassType::FixedTypes)
		{
			auto BinaryOp = NeverNullptr(node._BinaryOp);
			auto Info = Type_HasBinaryOverLoadWith(Ex0Type, BinaryOp->Type, Ex1Type);

			if (!Info.HasValue)
			{
				auto v = ToString(Ex0Type);

				LogError_CantFindBinaryOpForTypes(BinaryOp, Ex0Type, Ex1Type);
			}

			auto Op = node._BinaryOp->Type;


			BinaryExpressionNode_Data V;
			V.Op0 = Ex0Type;
			V.Op1 = Ex1Type;


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
				//all float bool int types
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

			
			bool didpush = _IR_IRlocations.size();
			if (didpush) {
				IRLocation_Cotr v;
				v.UsedlocationIR = true;
				v.Value = _IR_IRlocations.top().Value;
				_IR_IRlocations.push(v);
			}

			IR_Build_FuncCall(V, Tep, pars);

			if (didpush) {
				_IR_IRlocations.pop();
			}
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
		case TokenType::plus:_IR_LastExpressionField=_IR_LookingAtIRBlock->NewAdd(Ex0, Ex1);break;\
		case TokenType::minus:_IR_LastExpressionField=_IR_LookingAtIRBlock->NewSub(Ex0, Ex1);break;\
		case TokenType::star:_IR_LastExpressionField=_IR_LookingAtIRBlock->NewUMul(Ex0, Ex1);break; \
		case TokenType::forwardslash:_IR_LastExpressionField=_IR_LookingAtIRBlock->NewUDiv(Ex0, Ex1);break; \
		case TokenType::equal_Comparison:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewC_Equalto(Ex0, Ex1); break; \
		case TokenType::Notequal_Comparison:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewC_NotEqualto(Ex0, Ex1);break; \
		case TokenType::greaterthan:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewUGreaterThan(Ex0, Ex1); break; \
		case TokenType::lessthan:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewULessThan(Ex0, Ex1); break; \
		case TokenType::greater_than_or_equalto:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewUGreaterThanOrEqual(Ex0, Ex1); break; \
		case TokenType::less_than_or_equalto:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewULessThanOrEqual(Ex0, Ex1); break; \
		case TokenType::bitwise_LeftShift:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewBitWiseShiftL(Ex0, Ex1); break; \
		case TokenType::bitwise_RightShift:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewBitWiseShiftR(Ex0, Ex1); break; \
		case TokenType::bitwise_and:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewBitWiseAnd(Ex0, Ex1); break; \
		case TokenType::bitwise_XOr:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewBitWiseXOr(Ex0, Ex1); break; \
		case TokenType::bitwise_or:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewBitWiseOr(Ex0, Ex1); break; \
		case TokenType::modulo:_IR_LastExpressionField =  _IR_LookingAtIRBlock->NewUModulo(Ex0, Ex1); break; \
		default:\
			UCodeLangUnreachable();\
			break;\
		}\

#define BindaryBuildS(x) switch (Op) \
			{\
			case TokenType::plus:_IR_LastExpressionField=_IR_LookingAtIRBlock->NewAdd(Ex0, Ex1);break;\
			case TokenType::minus:_IR_LastExpressionField=_IR_LookingAtIRBlock->NewSub(Ex0, Ex1);break;\
			case TokenType::star:_IR_LastExpressionField=_IR_LookingAtIRBlock->NewSMul(Ex0, Ex1);break; \
			case TokenType::forwardslash:_IR_LastExpressionField=_IR_LookingAtIRBlock->NewSDiv(Ex0, Ex1);break; \
			case TokenType::equal_Comparison:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewC_Equalto(Ex0, Ex1); break; \
			case TokenType::Notequal_Comparison:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewC_NotEqualto(Ex0, Ex1); break; \
			case TokenType::greaterthan:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewSGreaterThan(Ex0, Ex1); break; \
			case TokenType::lessthan:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewSLessThan(Ex0, Ex1); break; \
			case TokenType::greater_than_or_equalto:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewSGreaterThanOrEqual(Ex0, Ex1); break; \
			case TokenType::less_than_or_equalto:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewSLessThanOrEqual(Ex0, Ex1); break; \
			case TokenType::bitwise_LeftShift:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewBitWiseShiftL(Ex0, Ex1); break; \
			case TokenType::bitwise_RightShift:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewBitWiseShiftR(Ex0, Ex1); break; \
			case TokenType::bitwise_and:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewBitWiseAnd(Ex0, Ex1); break; \
			case TokenType::bitwise_XOr:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewBitWiseXOr(Ex0, Ex1); break; \
			case TokenType::bitwise_or:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewBitWiseOr(Ex0, Ex1); break; \
			case TokenType::modulo:_IR_LastExpressionField =  _IR_LookingAtIRBlock->NewSModulo(Ex0, Ex1); break; \
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
				case TokenType::equal_Comparison:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewC_Equalto(Ex0, Ex1); break;
				case TokenType::Notequal_Comparison:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewC_NotEqualto(Ex0, Ex1); break;
				case TokenType::logical_and:_IR_LastExpressionField = lazylogicalandret; break;
				case TokenType::logical_or:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewlogicalOr(Ex0, Ex1); break;
				default:
					UCodeLangUnreachable();
					break;
				}
				break;
			case TypesEnum::Char:
			case TypesEnum::Uft8:
			case TypesEnum::Uft16:
			case TypesEnum::Uft32:
				switch (Op)
				{
				case TokenType::equal_Comparison:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewC_Equalto(Ex0, Ex1); break;
				case TokenType::Notequal_Comparison:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewC_NotEqualto(Ex0, Ex1); break;
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
	case TokenType::bitwise_LeftShift:
	case TokenType::bitwise_RightShift:
	case TokenType::bitwise_and:
	case TokenType::bitwise_or:
	case TokenType::bitwise_XOr:
	case TokenType::modulo:
		V = Ex0Type;
		break;
	default:
		V.SetType(TypesEnum::Void);
		break;
	}
	return V;
}
UCodeLangFrontEnd

#endif
