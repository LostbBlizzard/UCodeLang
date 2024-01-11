#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

//how defer is as a Lambda that just so happened to have its destructor have the statements
void SystematicAnalysis::OnDeferStatement(const DeferStatementNode& node)
{
	const String LambdaName = CompilerGenerated("DeferLambda") + std::to_string((uintptr_t)&node);
	auto classSymname = LambdaName + "_";

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

		auto oldscope = std::move(_Table._Scope);

		_Table._Scope.ThisScope = ScopeHelper::ApendedStrings(ScopeHelper::ApendedStrings(oldscope.ThisScope ,classSymname), ClassDestructorFunc);

		for (auto& Item : node._Base._Nodes)
		{
			OnStatement(*Item);
		}

		_Table._Scope = std::move(oldscope);

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

		
		
		auto& ClassSym = Symbol_AddSymbol(SymbolType::Type_class,classSymname, ScopeHelper::ApendedStrings(_Table._Scope.ThisScope, classSymname), AccessModifierType::Public);
		ClassInfo* val = new ClassInfo();
		ClassSym.Info.reset(val);
		_Table.AddSymbolID(ClassSym, Symbol_GetSymbolID(val));

		for (auto& Item : Info->_CapturedVarables)
		{
			FieldInfo field;
			field.Name = Item.Name;
			field.Type = Item.Type;
			val->Fields.push_back(std::move(field));
		}

		auto& DropFunc = Symbol_AddSymbol(SymbolType::Func, ClassDestructorFunc,ScopeHelper::ApendedStrings(ClassSym.FullName, ClassDestructorFunc), AccessModifierType::Public);
		FuncInfo* DropFuncInfo = new FuncInfo();
		DropFunc.Info.reset(DropFuncInfo);
		DropFuncInfo->Ret = TypesEnum::Void;

		DropFuncInfo->FullName = DropFunc.FullName;
		DropFuncInfo->FrontParIsUnNamed = true;

		auto lambdatype = TypeSymbol(ClassSym.ID);

		ParInfo par;
		par.Type = lambdatype;
		par.Type._IsAddress = true;
		DropFuncInfo->Pars.push_back(par);
		

		auto irstructtype = IR_ConvertToIRType(lambdatype);
		IRInstruction* tep =_IR_LookingAtIRBlock->NewLoad(irstructtype);


		IRStruct* ClassStruct = _IR_Builder.GetSymbol(irstructtype._symbol)->Get_ExAs<IRStruct>();


		for (size_t i = 0; i < Info->_CapturedVarables.size(); i++)
		{
			auto& Item = Info->_CapturedVarables[i];

			auto Sym = Item.Sym;

			IRInstruction* Value = nullptr;
			if (Sym->Type == SymbolType::StackVarable)
			{
				Value = _IR_LookingAtIRBlock->NewLoad(Sym->IR_Ins);
			}
			else if (Sym->Type == SymbolType::ParameterVarable)
			{
				Value = _IR_LookingAtIRBlock->NewLoad(Sym->IR_Par);
			}
			else 
			{
				UCodeLangUnreachable();
			}


			auto Member = _IR_LookingAtIRBlock->New_Member_Access(tep, ClassStruct, i);
			_IR_LookingAtIRBlock->NewStore(Member, Value);

			{
				String FullName = ClassSym.FullName;

				ScopeHelper::GetApendedString(FullName, Item.Name);


				auto& Sym = Symbol_AddSymbol(SymbolType::Class_Field, Item.Name, FullName, AccessModifierType::Public);//to make a this indirection.
				Sym.VarType = Item.Type;
			}

		}
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

			{
				auto oldscope =std::move(_Table._Scope);

				_Table._Scope.ThisScope = DropFuncInfo->FullName;

				ClassStackInfo _Data;
				_Data.Info = val;
				_Data.Syb = &ClassSym;
				_Data._InStatements = true;
				_ClassStack.push(_Data);

				FuncStackInfo _FuncData(DropFuncInfo);
				_FuncStack.push_back(_FuncData);


				Push_NewStackFrame();

				for (auto& Item : node._Base._Nodes)
				{
					OnStatement(*Item);
				}

				Pop_StackFrame();

				_ClassStack.pop();
				_FuncStack.pop_back();

				_Table._Scope = std::move(oldscope);
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