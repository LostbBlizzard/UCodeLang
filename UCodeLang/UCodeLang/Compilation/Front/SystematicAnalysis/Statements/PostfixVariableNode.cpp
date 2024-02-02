#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnPostfixVariableNode(const PostfixVariableNode& node)
{
	if (_PassType == PassType::GetTypes)
	{
		OnExpressionTypeNode(node._ToAssign._Value.get(), GetValueMode::ReadAndWrite);
	}
	else if (_PassType == PassType::FixedTypes)
	{
		_LookingForTypes.push(TypesEnum::Any);
		OnExpressionTypeNode(node._ToAssign._Value.get(), GetValueMode::ReadAndWrite);
		_LookingForTypes.pop();

		auto ExType = _LastExpressionType;

		auto HasInfo = Type_HasPostfixOverLoadWith(ExType, node._PostfixOp->Type);
		if (!HasInfo.HasValue)
		{
			LogError_CantFindPostfixOpForTypes(NeverNullptr(node._PostfixOp), ExType);
		}


		PostFixExpressionNode_Data t;

		if (HasInfo.Value)
		{
			FuncInfo* f = HasInfo.Value.value()->Get_Info<FuncInfo>();
			t.FuncToCall = HasInfo.Value.value();
			t.Op0 = f->Pars[0].Type;
		}
		else
		{
			t.Op0 = _LastExpressionType;
		}

		_PostFix_Datas.AddValue(Symbol_GetSymbolID(node), t);
	}
	else if (_PassType == PassType::BuidCode)
	{
		const PostFixExpressionNode_Data& Data = _PostFix_Datas.GetValue(Symbol_GetSymbolID(node));

		if (Data.FuncToCall)
		{
			FuncInfo* f = Data.FuncToCall->Get_Info<FuncInfo>();

			Get_FuncInfo V;
			V.Func = f;
			V.SymFunc = Data.FuncToCall;
			V.ThisPar = Get_FuncInfo::ThisPar_t::NoThisPar;

			ScopedNameNode Tep;

			ValueParametersNode pars;
			pars._Nodes.push_back(Unique_ptr<Node>(node._ToAssign._Value.get()));

			IR_Build_FuncCall(V, Tep, pars);

			pars._Nodes[0].release();//its ok Tree has pointer.

			_LastExpressionType = V.Func->Ret;
		}
		else
		{
			_LookingForTypes.push(Data.Op0);
			OnExpressionTypeNode(node._ToAssign._Value.get(), GetValueMode::ReadAndWrite);
			_LookingForTypes.pop();

			IRInstruction* LoadV = _IR_LastExpressionField;

			auto Type = _LastExpressionType;

#define buildPortFixU(x)\
		if (node._PostfixOp->Type == TokenType::increment)\
		{\
			_IR_LastExpressionField=_IR_LookingAtIRBlock->NewAdd(LoadV,_IR_LookingAtIRBlock->NewLoad((UInt##x)1));\
		}\
		else\
		{\
			_IR_LastExpressionField=_IR_LookingAtIRBlock->NewSub(LoadV, _IR_LookingAtIRBlock->NewLoad((UInt##x)1));\
		}\



#define buildPortFixS(x) buildPortFixU(x)


			Debug_Add_SetLineNumber(NeverNullptr(node._PostfixOp), _IR_LookingAtIRBlock->Instructions.size());

			switch (Type._Type)
			{
			case TypesEnum::uInt8:
			{
				buildPortFixU(8);
			}
			break;

			case TypesEnum::uInt16:
			{
				buildPortFixU(16);
			}
			break;
			case TypesEnum::uInt32:
			{
				buildPortFixU(32);
			}
			break;
			case TypesEnum::uInt64:
			{
				buildPortFixU(64);
			}
			break;
			case TypesEnum::uIntPtr:
			{
				if (_Settings->PtrSize == IntSizes::Int64) {
					buildPortFixU(64)
				}
				else
				{
					buildPortFixU(32)
				}
			}
			break;
			case TypesEnum::sInt8:
			{
				buildPortFixS(8);
			}
			break;
			case TypesEnum::sInt16:
			{
				buildPortFixS(16);
			}
			break;
			case TypesEnum::sInt32:
			{
				buildPortFixS(32);
			}
			break;
			case TypesEnum::sInt64:
			{
				buildPortFixS(64);
			}
			break;
			case TypesEnum::sIntPtr:
			{
				if (_Settings->PtrSize == IntSizes::Int64) {
					buildPortFixS(64)
				}
				else
				{
					buildPortFixS(32)
				}
			}
			break;
			default:
				UCodeLangUnreachable();
				break;
			}
			IR_WriteTo(_IR_LastExpressionField, Type, _IR_LastStoreField);
		}
	}
}


UCodeLangFrontEnd

#endif