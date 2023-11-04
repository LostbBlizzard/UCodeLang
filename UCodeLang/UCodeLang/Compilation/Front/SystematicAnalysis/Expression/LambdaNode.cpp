#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnLambdaNode(const LambdaNode& node)
{
	const String LambdaName = CompilerGenerated("Lambda") + std::to_string((uintptr_t)&node);
	if (_PassType == PassType::GetTypes)
	{
		_Table.AddScope(LambdaName);

		auto& LambdaSym = Symbol_AddSymbol(SymbolType::LambdaObject, LambdaName, _Table._Scope.ThisScope, AccessModifierType::Public);
		_Table.AddSymbolID(LambdaSym, Symbol_GetSymbolID(node));
		LambdaSym.Info = Unique_ptr<LambdaInfo>(new LambdaInfo());


		for (auto& Item : node._Pars._Parameters)
		{
			auto& Sym = Symbol_AddSymbol(SymbolType::ParameterVarable, Item._Name.AsString()
				, _Table._Scope.ScopeHelper::GetApendedString(Item._Name.AsStringView()), AccessModifierType::Public);

			ParameterInfo* Par = new ParameterInfo();
			Sym.Info.reset(Par);

			Par->IsOutValue = false;
			Par->MyFunc = _FuncStack.front().Pointer;


			_Table.AddSymbolID(Sym, Symbol_GetSymbolID(Item));
		}

		if (node._Statements.has_value())
		{
			for (const auto& node2 : node._Statements.value()._Nodes)
			{
				OnStatement(*node2);
			}
		}

		_Table.RemoveScope();
	}
	else
		if (_PassType == PassType::FixedTypes)
		{

			_Table.AddScope(LambdaName);

			auto& LambdaSym = *Symbol_GetSymbol(Symbol_GetSymbolID(node));
			LambdaInfo* Info = LambdaSym.Get_Info<LambdaInfo>();
			Info->Ret = TypeSymbol(TypesEnum::Var);

			const Vector<ParInfo>* ParHint = nullptr;
			const TypeSymbol* RetHint = nullptr;
			{
				auto hinttype = _LookingForTypes.top();
				if (auto Sym = Symbol_GetSymbol(hinttype).value_unchecked())
				{
					if (Sym->Type == SymbolType::Func_ptr
						|| Sym->Type == SymbolType::Hard_Func_ptr)
					{
						const FuncPtrInfo* PtrInfo = Sym->Get_Info<FuncPtrInfo>();
						ParHint = &PtrInfo->Pars;
						RetHint = &PtrInfo->Ret;
					}
				}
			}

			for (size_t i = 0; i < node._Pars._Parameters.size(); i++)
			{
				auto& Item = node._Pars._Parameters[i];
				Symbol& Sym = _Table.GetSymbol(Symbol_GetSymbolID(Item));

				Type_ConvertAndValidateType(Item._Type, Sym.VarType, NodeSyb_t::Parameter);

				if (ParHint)
				{
					if (Sym.VarType._Type == TypesEnum::Var)
					{
						Sym.VarType = (*ParHint)[i].Type;
					}
				}
				Info->Pars.push_back({ Item._IsOutVarable,Sym.VarType });
			}
			if (RetHint)
			{
				if (Info->Ret._Type == TypesEnum::Var)
				{
					Info->Ret = *RetHint;
				}
			}



			if (node._Statements.has_value())
			{
				{
					VarableUseData Data;
					_Varable.push(std::move(Data));
				}

				OnStatementsWithSetableRet(node._Statements.value(), Info->Ret, NeverNullptr(node._LambdaStart));


				//
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

					for (auto& Item : UseData._SymbolsToPassBecauseInerLamdba)
					{
						LambdaFieldInfo NewValue;
						NewValue.Name = ScopeHelper::GetNameFromFullName(Item->FullName);
						NewValue.Type = Item->VarType;
						NewValue.Sym = Item;

						Info->_IndirectCapturedVarables.push_back(std::move(NewValue));
					}

					if (_Varable.size() > 1)//copy Symbols
					{
						VarableUseData UseData2 = std::move(_Varable.top()); _Varable.pop();

						VarableUseData& ThisData = _Varable.top();
						for (auto& Item : UseData2._UsedSymbols)
						{
							/*
							auto Item_Name = ScopeHelper::GetNameFromFullName(Item->FullName);
							auto Item_FullName = _Table._Scope.ThisScope;
							ScopeHelper::GetApendedString(Item_FullName, Item_Name);

							auto& Sym = AddSymbol(SymbolType::Class_Field, Item_Name
								, Item_FullName);

							_Table.AddSymbolID(Sym, (SymbolID)&Item);
							*/

							ThisData._SymbolsToPassBecauseInerLamdba.push_back(Item);
						}
					}
					else
					{
						_Varable.pop();
					}

				}
				//
			}


			_Table.RemoveScope();

			{
				String LambdFuncScope = _Table._Scope.ThisScope;
				ScopeHelper::GetApendedString(LambdFuncScope, LambdaName);
				{
					Vector<LambdaFieldInfo> Tep_CapturedVarables;
					for (auto& Item : Info->_CapturedVarables)
					{
						if (Item.Sym->Type == SymbolType::ThreadVarable
							|| Item.Sym->Type == SymbolType::StaticVarable)
						{
							continue;
						}


						for (auto& Item2 : Tep_CapturedVarables)
						{
							if (Item.Name == Item2.Name)
							{
								goto OutLoop;
							}
						}


						for (size_t i = 0; i < node._Pars._Parameters.size(); i++)
						{
							auto& ParItem = node._Pars._Parameters[i];
							SymbolID ParID = Symbol_GetSymbolID(ParItem);
							if (Item.Sym->ID == ParID)
							{
								goto OutLoop;
							}
						}


						if (Item.Sym->FullName.size() > LambdFuncScope.size())
						{
							continue;
						}

						Tep_CapturedVarables.push_back(Item);
					OutLoop:continue;
					}
					Info->_CapturedVarables = std::move(Tep_CapturedVarables);
				}
				//
				{

					Vector<LambdaFieldInfo> Tep_CapturedVarables;
					for (auto& Item : Info->_IndirectCapturedVarables)
					{
						if (Item.Sym->Type == SymbolType::ThreadVarable
							|| Item.Sym->Type == SymbolType::StaticVarable)
						{
							continue;
						}


						for (auto& Item2 : Tep_CapturedVarables)
						{
							if (Item.Name == Item2.Name)
							{
								goto OutLoop2;
							}
						}

						/*
						for (auto& Item2 : Info->_CapturedVarables)
						{
							if (Item.Name == Item2.Name)
							{
								goto OutLoop2;
							}
						}
						*/


						for (size_t i = 0; i < node._Pars._Parameters.size(); i++)
						{
							auto& ParItem = node._Pars._Parameters[i];
							SymbolID ParID = Symbol_GetSymbolID(ParItem);
							if (Item.Sym->ID == ParID)
							{
								goto OutLoop2;
							}
						}


						if (Item.Sym->FullName.size() > LambdFuncScope.size())
						{
							continue;
						}

						Tep_CapturedVarables.push_back(Item);
					OutLoop2:continue;
					}
					Info->_IndirectCapturedVarables = std::move(Tep_CapturedVarables);


					for (auto& Item : Info->_IndirectCapturedVarables)
					{
						const NeverNullPtr<Token> Token = NeverNullptr(node._LambdaStart);

						//LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos, "can't indirect pass the varable  '" +
							//Item.Name + "' between lambdas.be explicit and make a new varable and assign using '" + Item.Name + "' in this lambda.");

						Info->_CapturedVarables.push_back(Item);
					}

				}
			}

			if (Info->UsesOuterScope())
			{


				const String LambdaClassName = LambdaName + "class";
				String FullName = _Table._Scope.ThisScope;
				ScopeHelper::GetApendedString(FullName, LambdaClassName);
				auto& SymClass = Symbol_AddSymbol(SymbolType::Type_class, LambdaClassName, FullName, AccessModifierType::Public);
				SymClass.PassState = PassType::FixedTypes;


				_Table.AddScope(LambdaName);
				const SymbolID ClassSymID = Symbol_GetSymbolID(&node._Capture);
				_Table.RemoveScope();

				_Table.AddSymbolID(SymClass, ClassSymID);

				ClassInfo* Classinfo = new ClassInfo();
				SymClass.Info = Unique_ptr<Symbol_Info>(Classinfo);

				Classinfo->FullName = SymClass.FullName;
				//

				bool HasDropCall = false;
				for (auto& Item : Info->_CapturedVarables)
				{
					Classinfo->AddField(Item.Name, Item.Type);

					if (Symbol_HasDestructor(Item.Type))
					{
						HasDropCall = true;
					}
				}

				if (HasDropCall)
				{
					auto Sym = Symbol_MakeNewDropFuncSymbol(Classinfo, TypeSymbol(ClassSymID));
					Info->_ClassDrop = Sym->Get_Info<FuncInfo>();

				}


				{
					FuncInfo TepFuninfo;
					TepFuninfo.Pars = Info->Pars;
					TepFuninfo.Ret = Info->Ret;
					TepFuninfo.FrontParIsUnNamed = true;
					TepFuninfo._FuncType = FuncInfo::FuncType::Invoke;


					auto ClassPtr = TypeSymbol(ClassSymID);
					ClassPtr._IsAddress = true;

					TepFuninfo.Pars.insert(TepFuninfo.Pars.begin(), { false,ClassPtr });

					String funcFullName = SymClass.FullName;
					ScopeHelper::GetApendedString(funcFullName, Overload_Invoke_Func);

					TepFuninfo.FullName = funcFullName;

					auto& SymClass = Symbol_AddSymbol(SymbolType::Func, Overload_Invoke_Func, funcFullName, AccessModifierType::Public);
					SymClass.Info = Unique_ptr<FuncInfo>(new FuncInfo(std::move(TepFuninfo)));
					SymClass.PassState = _PassType;

					Info->_ClassCall = (FuncInfo*)SymClass.Info.get();
				}

				//
				_LastExpressionType = TypeSymbol(ClassSymID);
			}
			else
			{
				String funcPtrName = Str_GetLambdaFuncTepName(LambdaName);
				String funcFullName = _Table._Scope.ThisScope;
				ScopeHelper::GetApendedString(funcFullName, funcPtrName);

				FuncInfo TepFuninfo;
				TepFuninfo.Pars = Info->Pars;
				TepFuninfo.Ret = Info->Ret;
				TepFuninfo.FullName = funcFullName;
				auto TepLambdaFunc = GetTepFuncPtrSyb(funcPtrName, &TepFuninfo).value();
				TepLambdaFunc->PassState = _PassType;


				_LastExpressionType = TypeSymbol(TepLambdaFunc->ID);
			}
		}
		else if (_PassType == PassType::BuidCode)
		{
			_Table.AddScope(LambdaName);

			auto& LambdaSym = _Table.GetSymbol(Symbol_GetSymbolID(node));
			LambdaInfo* Info = LambdaSym.Get_Info<LambdaInfo>();

			if (Info->UsesOuterScope())
			{
				const SymbolID ClassSymID = Symbol_GetSymbolID(&node._Capture);
				auto& SymClass = _Table.GetSymbol(ClassSymID);

				ClassInfo* ClassSymInfo = SymClass.Get_Info<ClassInfo>();

				auto ClassType = TypeSymbol(ClassSymID);
				auto ClassTypeIR = IR_ConvertToIRType(ClassType);

				auto ClassTypeIRPtr = ClassType;
				ClassTypeIRPtr._IsAddress = true;

				auto TepIRObject = _IR_LookingAtIRBlock->NewLoad(ClassTypeIR);




				IRStruct* ClassStruct = _IR_Builder.GetSymbol(ClassTypeIR._symbol)->Get_ExAs<IRStruct>();

				for (size_t i = 0; i < Info->_CapturedVarables.size(); i++)
				{
					auto& Item = Info->_CapturedVarables[i];

					auto newsyb = Symbol_GetSymbol(Item.Name, Item.Sym->Type).value();
					//because lambda's may have other lambdas and may make a new symbol for us to use


					IRInstruction* Value = nullptr;
					if (newsyb->Type == SymbolType::StackVarable)
					{
						Value = _IR_LookingAtIRBlock->NewLoad(newsyb->IR_Ins);
					}
					else if (newsyb->Type == SymbolType::ParameterVarable)
					{
						Value = _IR_LookingAtIRBlock->NewLoad(newsyb->IR_Par);
					}
					else if (newsyb->Type == SymbolType::Class_Field)//this.X
					{
						auto& Func = _FuncStack.back().Pointer;
						auto ThisParSym = Symbol_GetSymbol(Func->Pars.front().Type).value();
						//if (IsSymbolLambdaObjectClass(ThisParSym))
						{
							//If The ThisPar an Lambda Object
							//auto parsym = Symbol_GetSymbol(ScopeHelper::ApendedStrings(Symbol_GetSymbol(Func)->FullName, ThisSymbolName), SymbolType::ParameterVarable).value();
							//Output = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(In._Symbol->VarType));
							//Value = LoadSymbolWhenInLambdaObjectInvoke(newsyb);
						}
						//else
						{
							auto classSb = _ClassStack.top().Syb;
							auto Classtype = TypeSymbol(classSb->ID);
							auto GG = classSb->Get_Info<ClassInfo>();
							auto IndexField = GG->GetFieldIndex(ScopeHelper::GetNameFromFullName(newsyb->FullName)).value();


							IRStruct* V = _IR_Builder.GetSymbol(IR_Build_ConvertToIRClassIR(*classSb))->Get_ExAs<IRStruct>();
							Value = _IR_LookingAtIRBlock->New_Member_Dereference(&_IR_LookingAtIRFunc->Pars[0], IR_ConvertToIRType(classSb->ID), IndexField);
						}
					}
					else
					{
						UCodeLangUnreachable();
					}


					auto Member = _IR_LookingAtIRBlock->New_Member_Access(TepIRObject, ClassStruct, i);
					_IR_LookingAtIRBlock->NewStore(Member, Value);

					{
						String FullName = _Table._Scope.ThisScope;

						ScopeHelper::GetApendedString(FullName, Item.Name);


						auto& Sym = Symbol_AddSymbol(SymbolType::Class_Field, Item.Name, FullName, AccessModifierType::Public);//to make a this indirection.
						Sym.VarType = Item.Type;
					}
				}

				if (Info->_ClassDrop)
				{//build drop func
					auto oldFunc = _IR_LookingAtIRFunc;
					auto oldblock = _IR_LookingAtIRBlock;
					//



					_IR_LookingAtIRFunc = _IR_Builder.NewFunc(IR_MangleName(Info->_ClassDrop), IR_ConvertToIRType(Info->Ret));
					_IR_LookingAtIRBlock = _IR_LookingAtIRFunc->NewBlock(".");

					_IR_LookingAtIRFunc->Pars.resize(1);
					{
						IRPar V;
						V.identifier = _IR_Builder.ToID(ThisSymbolName);
						V.type = IR_ConvertToIRType(ClassTypeIRPtr);
						_IR_LookingAtIRFunc->Pars[0] = V;
					}

					IR_Build_FuncDropUsingFields(ClassSymInfo, ClassTypeIR);

					_IR_LookingAtIRBlock->NewRet();
					//
					_IR_LookingAtIRFunc = oldFunc;
					_IR_LookingAtIRBlock = oldblock;
				}

				{//build func
					auto oldFunc = _IR_LookingAtIRFunc;
					auto oldblock = _IR_LookingAtIRBlock;
					//



					_IR_LookingAtIRFunc = _IR_Builder.NewFunc(IR_MangleName(Info->_ClassCall), IR_ConvertToIRType(Info->Ret));
					_IR_LookingAtIRBlock = _IR_LookingAtIRFunc->NewBlock(".");


					_IR_LookingAtIRFunc->Pars.resize(node._Pars._Parameters.size() + 1);
					{
						IRPar V;
						V.identifier = _IR_Builder.ToID(ThisSymbolName);
						V.type = IR_ConvertToIRType(ClassTypeIRPtr);
						_IR_LookingAtIRFunc->Pars[0] = V;
					}
					for (size_t i = 0; i < node._Pars._Parameters.size(); i++)
					{
						auto& Item = node._Pars._Parameters[i];
						Symbol& Sym = _Table.GetSymbol(Symbol_GetSymbolID(Item));

						IRPar V;
						V.type = IR_ConvertToIRType(Info->Pars[i]);
						V.identifier = _IR_Builder.ToID((IRidentifier)Item._Name.token->Value._String);
						_IR_LookingAtIRFunc->Pars[i + 1] = V;

						Sym.IR_Par = &_IR_LookingAtIRFunc->Pars[i + 1];
					}

					ClassStackInfo _Data;
					_Data.Info = ClassSymInfo;
					_Data.Syb = &SymClass;
					_Data._InStatements = true;
					_ClassStack.push(_Data);

					FuncStackInfo _FuncData(Info->_ClassCall);
					_FuncStack.push_back(_FuncData);

					Push_NewStackFrame();
					if (node._Statements.has_value())
					{
						for (const auto& node2 : node._Statements.value()._Nodes)
						{
							OnStatement(*node2);
						}
					}

					_ClassStack.pop();
					_FuncStack.pop_back();

					bool hasins = _IR_LookingAtIRBlock->Instructions.size();
					auto lastbefordrop = hasins ? _IR_LookingAtIRBlock->Instructions.back().get() : nullptr;

					Pop_StackFrame();
					size_t droploc = hasins ? _IR_LookingAtIRBlock->Instructions.size() - 1 : 0;

					for (auto& Item : _IR_Rets)
					{
						_IR_LookingAtIRBlock->UpdateJump(Item.JumpIns, droploc);
						if (Item.JumpIns == lastbefordrop)
						{
							Item.JumpIns->SetAsNone();
						}
					}
					_IR_Rets.clear();

					_IR_LookingAtIRBlock->NewRet();
					//
					_IR_LookingAtIRFunc = oldFunc;
					_IR_LookingAtIRBlock = oldblock;
				}


				_IR_LastExpressionField = TepIRObject;
				_LastExpressionType = ClassType;
			}
			else
			{
				auto oldFunc = _IR_LookingAtIRFunc;
				auto oldblock = _IR_LookingAtIRBlock;
				//

				auto FuncName = _Table._Scope.ThisScope;

				_IR_LookingAtIRFunc = _IR_Builder.NewFunc(FuncName, IR_ConvertToIRType(Info->Ret));
				_IR_LookingAtIRBlock = _IR_LookingAtIRFunc->NewBlock(".");


				_IR_LookingAtIRFunc->Pars.resize(node._Pars._Parameters.size());
				for (size_t i = 0; i < node._Pars._Parameters.size(); i++)
				{
					auto& Item = node._Pars._Parameters[i];
					Symbol& Sym = _Table.GetSymbol(Symbol_GetSymbolID(Item));

					IRPar V;
					V.type = IR_ConvertToIRType(Info->Pars[i]);
					V.identifier = _IR_Builder.ToID((IRidentifier)Item._Name.token->Value._String);
					_IR_LookingAtIRFunc->Pars[i] = V;

					Sym.IR_Par = &_IR_LookingAtIRFunc->Pars[i];
				}

				Push_NewStackFrame();
				if (node._Statements.has_value())
				{
					for (const auto& node2 : node._Statements.value()._Nodes)
					{
						OnStatement(*node2);
					}
				}

				bool hasins = _IR_LookingAtIRBlock->Instructions.size();
				auto lastbefordrop = hasins ? _IR_LookingAtIRBlock->Instructions.back().get() : nullptr;

				Pop_StackFrame();
				size_t droploc = hasins ? _IR_LookingAtIRBlock->Instructions.size() - 2 : 0;

				for (auto& Item : _IR_Rets)
				{
					_IR_LookingAtIRBlock->UpdateJump(Item.JumpIns, droploc);
					if (Item.JumpIns == lastbefordrop)
					{
						Item.JumpIns->SetAsNone();
					}
				}
				_IR_Rets.clear();

				_IR_LookingAtIRBlock->NewRet();
				//
				_IR_LookingAtIRFunc = oldFunc;
				_IR_LookingAtIRBlock = oldblock;

				_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoadFuncPtr(_IR_Builder.ToID(FuncName));


				FuncInfo TepFuninfo;
				TepFuninfo.Pars = Info->Pars;
				TepFuninfo.Ret = Info->Ret;

				String funcPtrName = Str_GetLambdaFuncTepName(LambdaName);
				String funcFullName = ScopeHelper::GetReMoveScope(_Table._Scope.ThisScope);
				ScopeHelper::GetApendedString(funcFullName, funcPtrName);
				_LastExpressionType = TypeSymbol(GetTepFuncPtrSyb(funcFullName, &TepFuninfo).value()->ID);
			}


			_Table.RemoveScope();
		}
}

bool SystematicAnalysis::IsSymbolLambdaObjectClass(const NeverNullPtr<Symbol> Sym) const
{
	return StringHelper::StartWith(ScopeHelper::GetNameFromFullName(Sym->FullName), CompilerGenerated("Lambda"));
}
IRInstruction* SystematicAnalysis::LoadSymbolWhenInLambdaObjectInvoke(const NeverNullPtr<Symbol> Sym)
{
	return _IR_LookingAtIRBlock->NewLoad((int)5000000);
}
UCodeLangFrontEnd