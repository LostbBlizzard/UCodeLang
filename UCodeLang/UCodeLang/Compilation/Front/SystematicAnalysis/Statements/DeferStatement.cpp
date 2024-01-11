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

		auto classSymname = LambdaName + "_";
		
		auto& ClassSym = Symbol_AddSymbol(SymbolType::Type_class, classSymname, classSymname, AccessModifierType::Public);
		ClassInfo* val = new ClassInfo();
		ClassSym.Info.reset(val);
		_Table.AddSymbolID(ClassSym, Symbol_GetSymbolID(val));

		auto& DropFunc = Symbol_AddSymbol(SymbolType::Func, ClassDestructorFunc,ScopeHelper::ApendedStrings(classSymname, ClassDestructorFunc), AccessModifierType::Public);
		FuncInfo* DropFuncInfo = new FuncInfo();
		DropFunc.Info.reset(DropFuncInfo);
		DropFuncInfo->Ret = TypesEnum::Void;

		DropFuncInfo->FullName = DropFunc.FullName;


		auto lambdatype = TypeSymbol(ClassSym.ID);

		ParInfo par;
		par.Type = lambdatype;
		par.Type._IsAddress = true;
		DropFuncInfo->Pars.push_back(par);
		
		IRInstruction* tep =_IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(lambdatype));
		
		IR_Build_AddDestructorToStack(lambdatype, tep);
		{
			auto oldFunc = _IR_LookingAtIRFunc;
			auto oldblock = _IR_LookingAtIRBlock;
			//

			_IR_LookingAtIRFunc = _IR_Builder.NewFunc(IR_MangleName(DropFuncInfo), IR_ConvertToIRType(DropFuncInfo->Ret));
	
			auto tepp = lambdatype;
			tepp._IsAddress = true;

			IRPar p; 
			p.type = IR_ConvertToIRType(tepp);
			p.identifier = _IR_Builder.ToID(ThisSymbolName);

			_IR_LookingAtIRFunc->Pars.push_back(p);


			_IR_LookingAtIRBlock = _IR_LookingAtIRFunc->NewBlock(".");

			for (auto& Item : node._Base._Nodes)
			{
				OnStatement(*Item);
			}

			_IR_LookingAtIRBlock->NewRet();
			//
			_IR_LookingAtIRFunc = oldFunc;
			_IR_LookingAtIRBlock = oldblock;
		}
	}
}

UCodeLangFrontEnd

#endif