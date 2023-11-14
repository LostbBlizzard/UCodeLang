#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnYieldExpression(const YieldExpression& node)
{
	if (_PassType == PassType::GetTypes)
	{
		OnExpressionTypeNode(node._Expression, GetValueMode::Read);
	}
	else if (_PassType == PassType::FixedTypes)
	{
		OnExpressionTypeNode(node._Expression, GetValueMode::Read);
		auto extype = _LastExpressionType;


		if (!Type_IsFuture(extype))
		{
			auto token = NeverNullptr(node._Token);
			LogError_yieldnotAsync(token);

			_LastExpressionType = TypesEnum::Null;
		}
		else
		{
			_LastExpressionType = Type_GetBaseFromFuture(extype);
		}
		SetAfterYeld();
	}
	else if (_PassType == PassType::BuidCode)
	{
		thread_local int a = 0;

		auto NewYieldIRName = _FuncStack.begin()->Pointer->FullName + "yield_" + std::to_string(a);
		auto NewAwaitCompleteIRName = _FuncStack.begin()->Pointer->FullName + "awaitComplete_" + std::to_string(a);



		OnExpressionTypeNode(node._Expression, GetValueMode::Read);

		auto extype = _LastExpressionType;
		auto base = Type_GetBaseFromFuture(extype);


		auto FutureIR = _IR_LastExpressionField;
		auto AwaitHandleIR = GetFutureHandle(extype, FutureIR);
		_IR_LookingAtIRBlock->New_Await_OnComplete(AwaitHandleIR, _IR_LookingAtIRBlock->NewLoadFuncPtr(_IR_Builder.ToID(NewAwaitCompleteIRName)));
		_IR_LookingAtIRBlock->New_Await_RunTask(AwaitHandleIR);

		bool isbasevoid = base._Type == TypesEnum::Void;
		{
			auto newawaitsetfunc = _IR_Builder.NewFunc(NewAwaitCompleteIRName, IRType(IRTypes::Void));
			auto newawaitsetblock = newawaitsetfunc->NewBlock(".");
			IRPar awaitpar;
			awaitpar.identifier = _IR_Builder.ToID("await_handle");
			awaitpar.type = IRTypes::pointer;
			newawaitsetfunc->Pars.push_back(std::move(awaitpar));


			if (!isbasevoid)
			{
				IRInstruction* AwaitHandleIR = newawaitsetblock->NewLoad(&newawaitsetfunc->Pars.back());

				IRInstruction* awaitretv = newawaitsetblock->New_Await_GetValue(AwaitHandleIR, IR_ConvertToIRType(base));

				newawaitsetblock->NewPushParameter(awaitretv);
			}
			IRInstruction* funcv = newawaitsetblock->NewCall(_IR_Builder.ToID(NewYieldIRName));

			newawaitsetblock->NewRet();
		}
		_IR_LookingAtIRBlock->NewRetValue(FutureIR);
		_IR_LookingAtIRBlock->NewRet();


		a++;
		auto newfunc = _IR_Builder.NewFunc(NewYieldIRName, IR_ConvertToIRType(base));

		if (!isbasevoid)
		{
			IRPar p;
			p.identifier = _IR_Builder.ToID("yieldvalue");
			p.type = IR_ConvertToIRType(base);
			newfunc->Pars.push_back(std::move(p));
		}

		auto newblock = newfunc->NewBlock(".");





		_IR_LookingAtIRFunc = newfunc;
		_IR_LookingAtIRBlock = newblock;

		SetAfterYeld();
		if (base._Type == TypesEnum::Void)
		{
			_IR_LastExpressionField = nullptr;
		}
		else
		{
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad(&newfunc->Pars.front());
		}
		_LastExpressionType = base;



	}
}

void SystematicAnalysis::OnYieldStatement(const YieldStatement& node)
{
	OnYieldExpression(node._Base);
}

UCodeLangFrontEnd

#endif