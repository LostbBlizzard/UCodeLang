#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnAwaitExpression(const AwaitExpression& node)
{
	if (_PassType == PassType::GetTypes)
	{
		if (node._IsFunc)
		{
			const FuncCallNode& Funcnode = node._Func;
			OnFuncCallNode(Funcnode);
		}
		else
		{
			const LambdaNode& LambdaNode = node._Lambda;
			OnLambdaNode(LambdaNode);
		}
	}
	else if (_PassType == PassType::FixedTypes)
	{
		TypeSymbol FuncRetType;
		if (node._IsFunc)
		{
			const FuncCallNode& Funcnode = node._Func;
			OnFuncCallNode(Funcnode);

			const auto& GetFunc = _FuncToSyboID.GetValue(Symbol_GetSymbolID(Funcnode));

			Type_SetFuncRetAsLastEx(GetFunc);
			FuncRetType = _LastExpressionType;

		}
		else
		{
			const LambdaNode& LambdaNode = node._Lambda;
			OnLambdaNode(LambdaNode);


			const String LambdaName = CompilerGenerated("Lambda") + GetScopeLabelName(&LambdaNode);
			_Table.AddScope(LambdaName);

			auto& LambdaSym = *Symbol_GetSymbol(Symbol_GetSymbolID(LambdaNode));
			LambdaInfo* Info = LambdaSym.Get_Info<LambdaInfo>();
			FuncRetType = Info->Ret;

			_Table.RemoveScope();
		}

		auto AsFuture = Type_MakeFutureFromType(FuncRetType);

		_LastExpressionType = AsFuture;
	}
	else if (_PassType == PassType::BuidCode)
	{
		TypeSymbol FuncRetType;

		if (node._IsFunc)
		{
			const FuncCallNode& Funcnode = node._Func;
			const auto& GetFunc = _FuncToSyboID.GetValue(Symbol_GetSymbolID(Funcnode));

			thread_local int a = 0; a++;
			auto NewAwaitSetIRName = _FuncStack.begin()->Pointer->FullName + "awaitset_" + std::to_string(a);
			{
				auto newawaitsetfunc = _IR_Builder.NewFunc(NewAwaitSetIRName, IRType(IRTypes::Void));
				auto newawaitsetblock = newawaitsetfunc->NewBlock(".");
				IRPar awaitpar;
				awaitpar.identifier = _IR_Builder.ToID("await_handle");
				awaitpar.type = IRTypes::pointer;
				newawaitsetfunc->Pars.push_back(std::move(awaitpar));

				IRInstruction* AwaitHandleIR = newawaitsetblock->NewLoad(&newawaitsetfunc->Pars.back());

				IRInstruction* funcv = newawaitsetblock->NewCall(IR_GetIRID(GetFunc.Func));

				newawaitsetblock->New_Await_SetValue(AwaitHandleIR, funcv);
				newawaitsetblock->NewRet();
			}


			Type_SetFuncRetAsLastEx(GetFunc);
			FuncRetType = _LastExpressionType;

			IRInstruction* funcptr = _IR_LookingAtIRBlock->NewLoadFuncPtr(_IR_Builder.ToID(NewAwaitSetIRName));

			auto awaittask = _IR_LookingAtIRBlock->New_Await_Task(funcptr);



			_IR_LastExpressionField = awaittask;
		}
		else
		{
			const LambdaNode& LambdaNode = node._Lambda;

			const String LambdaName = CompilerGenerated("Lambda") + GetScopeLabelName(&LambdaNode);
			_Table.AddScope(LambdaName);

			auto& LambdaSym = *Symbol_GetSymbol(Symbol_GetSymbolID(LambdaNode));
			LambdaInfo* Info = LambdaSym.Get_Info<LambdaInfo>();
			FuncRetType = Info->Ret;

			_Table.RemoveScope();


			IRInstruction* funcptr = _IR_LookingAtIRBlock->NewLoadFuncPtr(IR_GetIRID(Context_GetCuruntFunc().value().value()));

			auto awaittask = _IR_LookingAtIRBlock->New_Await_Task(funcptr);

			_IR_LookingAtIRBlock->New_Await_RunTask(awaittask);


			_IR_LastExpressionField = awaittask;
		}

		auto AsFuture = Type_MakeFutureFromType(FuncRetType);

		_IR_LastExpressionField = MakeFutureFromHandle(AsFuture, _IR_LastExpressionField);
		_LastExpressionType = AsFuture;
	}
}
void SystematicAnalysis::OnAwaitStatement(const AwaitStatement& node)
{
	OnAwaitExpression(node._Base);
}
TypeSymbol SystematicAnalysis::Type_MakeFutureFromType(const TypeSymbol& BaseType)
{
	auto symbolsOp = Symbol_GetSymbol(UCode_FutureType, SymbolType::Generic_class);

	auto token = _LastLookedAtToken.value();
	if (symbolsOp)
	{
		auto symbols = symbolsOp.value();
		if (symbols->Type != SymbolType::Generic_class
			//&& symbols->Type != SymbolType::Generic_Alias
			//&& symbols->Type != SymbolType::Generic_Enum
			)
		{
			LogError_ExpectedSymbolToBea(token, *symbols, SymbolType::Generic_class);
			return {};
		}

		if (symbols->Type == SymbolType::Generic_class)
		{
			auto CInfo = symbols->Get_Info<ClassInfo>();
			auto classnode = ClassNode::As(symbols->Get_NodeInfo<Node>());

			if (classnode->_generic._Values.size() == 1)
			{
				auto symfullname = Generic_SymbolGenericFullName(symbols, { BaseType });

				if (auto val = Symbol_GetSymbol(symfullname, SymbolType::Type))
				{
					return TypeSymbol(val.value()->ID);
				}
				else
				{
					Generic_TypeInstantiate(symbols, { BaseType });
					return  TypeSymbol(Symbol_GetSymbol(symfullname, SymbolType::Type).value()->ID);
				}

			}
			else
			{
				LogError_CanIncorrectGenericCount(token, UCode_FutureType, 1, classnode->_generic._Values.size());
			}
		}
		else
		{
			UCodeLangUnreachable();
		}
	}
	else
	{
		LogError_CantFindTypeError(token, UCode_FutureType);
	}

	return {};
}
bool SystematicAnalysis::Type_IsFuture(const TypeSymbol& Future)
{
	auto future = Symbol_GetSymbol(Future);

	if (future) {
		auto v = ScopeHelper::GetNameFromFullName(future.value()->FullName);
		if (StringHelper::StartWith(v, UCode_FutureType))
		{
			return true;
		}
	}

	return false;
}
TypeSymbol SystematicAnalysis::Type_GetBaseFromFuture(const TypeSymbol& Future)
{
	UCodeLangAssert(Type_IsFuture(Future));

	auto symname = ScopeHelper::ApendedStrings(Symbol_GetSymbol(Future).value()->FullName, "T");
	auto sym = Symbol_GetSymbol(symname, SymbolType::Type).value();
	return sym->VarType;
}
IRInstruction* SystematicAnalysis::GetFutureHandle(const TypeSymbol& Future, IRInstruction* IR)
{
	return IR;
}
IRInstruction* SystematicAnalysis::MakeFutureFromHandle(const TypeSymbol& Future, IRInstruction* IR)
{
	return IR;
}

UCodeLangFrontEnd

#endif