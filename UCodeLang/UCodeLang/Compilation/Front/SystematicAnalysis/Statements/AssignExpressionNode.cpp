#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
#include "UCodeLang/Compilation/Helpers/ParseHelper.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnAssignExpressionNode(const AssignExpressionNode& node)
{
	if (_PassType == PassType::GetTypes)
	{
		OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);
		OnExpressionTypeNode(node._ToAssign._Value.get(), GetValueMode::Write);

		if (node._ReassignAddress && !IsInUnSafeBlock())
		{
			auto Token = NeverNullptr(node._Token);
			LogError(ErrorCodes::ExpectingSequence, Token->OnLine, Token->OnPos, "Cant use Address Resassignment in safe mode.");
		}
	}
	else if (_PassType == PassType::FixedTypes)
	{
		_LookingForTypes.push(TypesEnum::Var);
		OnExpressionTypeNode(node._ToAssign._Value.get(), GetValueMode::Write);
		_LookingForTypes.pop();

		auto AssignType = _LastExpressionType;

		_LookingForTypes.push(AssignType);
		OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);
		_LookingForTypes.pop();


		auto ExpressionType = _LastExpressionType;

		if (node._ReassignAddress == false)
		{
			ExpressionType._IsAddress = false;
		}



		if (!Type_CanBeImplicitConverted(ExpressionType, AssignType, false))
		{
			auto  Token = _LastLookedAtToken;
			LogError_CantCastImplicitTypes(Token.value(), ExpressionType, AssignType , false);

		}
		else if (AssignType.IsMovedType())
		{
			auto Token = _LastLookedAtToken;
			LogError(ErrorCodes::InValidType,"Cant Reassign because the assignment an moved type", Token.value());
		}
		auto ID = Symbol_GetSymbolID(node);

		AssignExpression_Data Data;
		Data.Op0 = ExpressionType;
		Data.Op1 = AssignType;
		
		_AssignExpressionDatas.AddValue(ID, Data);
	}
	else if (_PassType == PassType::BuidCode)
	{

		auto ID = Symbol_GetSymbolID(node);
		auto& AssignType = _AssignExpressionDatas.GetValue(ID);

		Debug_Add_SetLineNumber(NeverNullptr(node._Token), _IR_LookingAtIRBlock->Instructions.size());

		_LookingForTypes.push(AssignType.Op0);
		OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);
		_LookingForTypes.pop();

		auto ExpressionType = _LastExpressionType;
		auto ExIR = _IR_LastExpressionField;

		

			

		auto implictype = Type_AreTheSameWithOutMoveAndimmutable(ExpressionType, AssignType.Op1) ? AssignType.Op0 : AssignType.Op1;

		IR_Build_ImplicitConversion(ExIR, ExpressionType, implictype);
		ExIR = _IR_LastExpressionField;

		auto t = AssignType.Op1;
		if (Symbol_HasDestructor(AssignType.Op1))
		{
			t.SetAsAddress();
		}

		_LookingForTypes.push(t);
		OnExpressionTypeNode(node._ToAssign._Value.get(),
			node._ReassignAddress ? GetValueMode::WritePointerReassment : GetValueMode::Write);
		_LookingForTypes.pop();

		auto AssignIR = _IR_LastExpressionField;
		auto AssignExType = _LastExpressionType;

		auto tw = AssignType.Op1;
		tw._IsAddress = false;
		if (Symbol_HasDestructor(tw))
		{
			ObjectToDrop dropinfo;
			dropinfo.DropType = ObjectToDropType::Operator;

			IROperator obj;
			auto store = _IR_LastStoreField;
			switch (_IR_LastStoreField.Type)
			{
			case IROperatorType::IRInstruction:
				obj = _IR_LastStoreField.Pointer;
				break;
			case IROperatorType::DereferenceOf_IRParameter:
				obj = _IR_LastStoreField.Parameter;
				AssignExType._IsAddress = false;
			break;
			default:
				UCodeLangUnreachable();
				break;
			}

			dropinfo._Operator = obj;
			dropinfo.Type = AssignExType;

			IR_Build_DestructorCall(dropinfo);

			auto& callinsir = _IR_LookingAtIRBlock->Instructions.back();

			if (callinsir->Type == IRInstructionType::CleanupFuncCall)
			{
				auto tocall = callinsir->B.identifier;
				callinsir->Type = IRInstructionType::Call;
				callinsir->A = IROperator(tocall);
				callinsir->B = IROperator();
			}

			_IR_LastStoreField = store;
			if (_IR_LastStoreField.Type == IROperatorType::DereferenceOf_IRParameter)
			{
				_IR_LastStoreField = IROperator(_IR_LastStoreField.Parameter);
			}
		}

		if (node._ReassignAddress)
		{
			IR_WriteTo(ExIR, _IR_LastStoreField);
		}
		else
		{
			IR_WriteTo(ExIR, AssignType.Op1, _IR_LastStoreField);
		}
	}
}

void SystematicAnalysis::IR_WriteTo(IRInstruction* IR, const IROperator& Value)
{
	if (Value.Type == IROperatorType::DereferenceOf_IRParameter)
	{
		//Because of Out Parameters
		IROperator op = Value;
		op.Type = IROperatorType::IRParameter;
		IR_WriteToDef(IR, op);
		return;
	}

	_IR_LookingAtIRBlock->NewStore((IRInstruction*)nullptr, IR);
	auto& Item = _IR_LookingAtIRBlock->Instructions.back();
	Item->Target() = Value;
}
void SystematicAnalysis::IR_WriteToDef(IRInstruction* IR, const IROperator& Value)
{
	_IR_LookingAtIRBlock->NewDereferenc_Store((IRInstruction*)nullptr, IR);
	auto& Item = _IR_LookingAtIRBlock->Instructions.back();
	Item->Target() = Value;
}
void SystematicAnalysis::IR_WriteTo(IRInstruction* IR, const TypeSymbol& Type, const IROperator& Value)
{
	if (Type.IsAddress())
	{
		IR_WriteToDef(IR, Value);
	}
	else
	{
		IR_WriteTo(IR, Value);
	}
}



UCodeLangFrontEnd

#endif