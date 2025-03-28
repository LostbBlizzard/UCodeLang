#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnDeclareVariablenode(const DeclareVariableNode& node, DeclareStaticVariableNode_t type)
{

	auto StrVarName = node._Name.AsString();
	auto FullName = _Table._Scope.GetApendedString(StrVarName);

	SymbolID sybId = Symbol_GetSymbolID(node);
	Symbol* syb;

	bool InSideClass = Context_InSideClass();
	bool IsField = InSideClass && DeclareStaticVariableNode_t::ClassField == type;

	if (_PassType == PassType::GetTypes)
	{
		SymbolType SysType = SymbolType::StackVarable;
		//
		switch (type)
		{
		case DeclareStaticVariableNode_t::Stack:
			SysType = SymbolType::StackVarable;
			break;
		case DeclareStaticVariableNode_t::Static:
			SysType = SymbolType::StaticVarable;
			break;
		case DeclareStaticVariableNode_t::Thread:
			SysType = SymbolType::ThreadVarable;
			break;
		case DeclareStaticVariableNode_t::Eval:
			SysType = SymbolType::ConstantExpression;
			break;
		case DeclareStaticVariableNode_t::ClassField:
		{
			SysType = SymbolType::Class_Field;
			auto& Class = *_ClassStack.top().Info;

			Class.AddField(ScopeHelper::GetNameFromFullName((String_view)FullName), TypeSymbol());
		}
		break;
		default:
			SysType = SymbolType::StackVarable;
			break;
		}
		//
		Symbol_RedefinitionCheck(FullName, SysType, NeverNullptr(node._Name.token));


		syb = &Symbol_AddSymbol(SysType, StrVarName, FullName, node._Access);
		_Table.AddSymbolID(*syb, sybId);

		syb->Set_NodeInfo(node.As());
		if (syb->Type == SymbolType::ConstantExpression)
		{
			ConstantExpressionInfo* info = new ConstantExpressionInfo();
			info->Exnode = &node._Expression;
			info->Context = Save_SymbolContext();
			syb->Info.reset(info);
		}
		else
		{
			DeclareVariableInfo* info = new DeclareVariableInfo();
			info->LineNumber = node._Name.token->OnPos;
			info->Context = Save_SymbolContext();
			syb->Info.reset(info);
		}

		//
		if (type == DeclareStaticVariableNode_t::Static
			|| type == DeclareStaticVariableNode_t::Thread
			|| type == DeclareStaticVariableNode_t::Eval)
		{
			if (!node._Expression._Value)
			{
				auto Token = node._Name.token;
				String VarType;
				switch (type)
				{
				case DeclareStaticVariableNode_t::Static:
					VarType = "static";
					break;
				case DeclareStaticVariableNode_t::Thread:
					VarType = "thread";
					break;
				case DeclareStaticVariableNode_t::Eval:
					VarType = "eval";
					break;
				default:
					UCodeLangUnreachable();
					break;
				}

				LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos, VarType + " Variable must be assigned.missing '='.");
			}
		}

		//
		_LookingForTypes.push(TypesEnum::Any);
		if (node._Expression._Value)
		{
			OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);
		}
		_LookingForTypes.pop();
	}
	else
	{
		syb = Symbol_GetSymbol(sybId).value();
	}

	//we may jump to this node non linearly
	if (syb->PassState == _PassType)
	{
		return;
	}

	if (_PassType == PassType::FixedTypes)
	{
		auto& VarType = syb->VarType;
		Type_ConvertAndValidateType(node._Type, VarType, NodeSyb_t::ClassField);
		VarType.SetAsLocation();


		auto Ex = node._Expression._Value.get();
		Type_DeclareVarableCheck(VarType, Ex, NeverNullptr(node._Name.token));

		if (VarType.IsTypeInfo() && type != DeclareStaticVariableNode_t::Eval)
		{
			auto Token = NeverNullptr(node._Name.token);
			LogError_UseingTypeinfoInNonEvalVarable(Token);
		}

		if (VarType.IsAn(TypesEnum::Void))
		{
			auto Token = NeverNullptr(node._Name.token);
			LogError_CantUseTypeVoidHere(Token);
		}
		
	}
	_LookingForTypes.push(syb->VarType);

	IRInstruction* OnVarable{};
	bool IsStructObjectPassRef = false;

	IRFunc* oldIRFunc{};
	IRBlock* oldblock{};

	if (_PassType == PassType::BuidCode)
	{
		if (node._Expression._Value)
		{
			if (syb->Type != SymbolType::ConstantExpression)
			{



				if (syb->Type == SymbolType::StackVarable)
				{
					auto pos = _IR_LookingAtIRBlock->InsCount() ? _IR_LookingAtIRBlock->GetIndex() : 0;

					Debug_Add_SetLineNumber(NeverNullptr(node._Name.token), pos);
					Debug_Add_SetVarableInfo(*syb, pos);
					OnVarable = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(syb->VarType));

					syb->IR_Ins = OnVarable;


				}
				else if (syb->Type == SymbolType::StaticVarable)
				{
					oldIRFunc = _IR_LookingAtIRFunc;
					oldblock = _IR_LookingAtIRBlock;



					if (_IR_Builder._StaticInit.Blocks.size() == 0)
					{
						_IR_Builder._StaticInit.NewBlock(".");
					}

					_IR_LookingAtIRFunc = &_IR_Builder._StaticInit;
					_IR_LookingAtIRBlock = _IR_LookingAtIRFunc->Blocks.front().get();


					auto pos = _IR_LookingAtIRBlock->InsCount() ? _IR_LookingAtIRBlock->GetIndex() : 0;
					Debug_Add_SetLineNumber(NeverNullptr(node._Name.token), pos);


					Debug_Add_SetVarableInfo(*syb, pos);
					if (Type_IsStructPassByRef(syb)) {
						OnVarable = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(syb->VarType));
					}
				}
				else if (syb->Type == SymbolType::ThreadVarable)
				{
					oldIRFunc = _IR_LookingAtIRFunc;
					oldblock = _IR_LookingAtIRBlock;

					if (_IR_Builder._threadInit.Blocks.size() == 0)
					{
						_IR_Builder._threadInit.NewBlock(".");
					}


					_IR_LookingAtIRFunc = &_IR_Builder._threadInit;
					_IR_LookingAtIRBlock = _IR_LookingAtIRFunc->Blocks.front().get();

					auto pos = _IR_LookingAtIRBlock->InsCount() ? _IR_LookingAtIRBlock->GetIndex() : 0;
					Debug_Add_SetLineNumber(NeverNullptr(node._Name.token), pos);
					Debug_Add_SetVarableInfo(*syb, pos);

					if (Type_IsStructPassByRef(syb)) {
						OnVarable = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(syb->VarType));
					}
				}
				else if (syb->Type == SymbolType::Class_Field)
				{
					oldIRFunc = _IR_LookingAtIRFunc;
					oldblock = _IR_LookingAtIRBlock;

					auto* Classinfo = _ClassStack.top().Info;
					if (Classinfo->_ClassFieldInit == nullptr)
					{
						String funcName = _Table._Scope.GetApendedString((String)ClassInitializefuncName);


						Classinfo->_ClassFieldInit = _IR_Builder.NewFunc(funcName, IRTypes::Void);
						Classinfo->_ClassFieldInit->NewBlock(".");
						{
							auto Classtype = TypeSymbol(_ClassStack.top().Syb->ID);
							Classtype._IsAddress = true;

							IRPar ThisPar;
							ThisPar.identifier = _IR_Builder.ToID(ThisSymbolName);
							ThisPar.type = IR_ConvertToIRType(Classtype);
							Classinfo->_ClassFieldInit->Pars.push_back(ThisPar);
						}
					}

					_IR_LookingAtIRFunc = Classinfo->_ClassFieldInit;
					_IR_LookingAtIRBlock = _IR_LookingAtIRFunc->Blocks.front().get();

					auto pos = _IR_LookingAtIRBlock->InsCount() ? _IR_LookingAtIRBlock->GetIndex() : 0;
					Debug_Add_SetLineNumber(NeverNullptr(node._Name.token), pos);

					if (Type_IsStructPassByRef(syb)) {
						OnVarable = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(syb->VarType));
					}
				}
				else
				{
					UCodeLangUnreachable();
				}

				IsStructObjectPassRef = Type_IsStructPassByRef(syb);

				if (IsStructObjectPassRef)
				{
					_IR_IRlocations.push({ OnVarable ,false });
				}

				auto tep = _LookingForTypes.top();
				if (DelareVarableImplicit.HasValue(sybId))
				{
					_LookingForTypes.top() = DelareVarableImplicit.GetValue(sybId);
				}

				OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);

				if (DelareVarableImplicit.HasValue(sybId))
				{
					_LookingForTypes.top() = tep;
				}
			}
		}
		else
		{
			if (syb->Type == SymbolType::StackVarable)
			{
				Debug_Add_SetLineNumber(NeverNullptr(node._Name.token), _IR_LookingAtIRBlock->GetIndex());

				OnVarable = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(syb->VarType));
				syb->IR_Ins = OnVarable;
			}
			else if (syb->Type == SymbolType::Class_Field)
			{
				bool hasdefaultConstructor = Type_HasDefaultConstructorFunc(syb->VarType);
				if (hasdefaultConstructor) 
				{

					oldIRFunc = _IR_LookingAtIRFunc;
					oldblock = _IR_LookingAtIRBlock;

					auto* Classinfo = _ClassStack.top().Info;
					if (Classinfo->_ClassFieldInit == nullptr)
					{
						String funcName = _Table._Scope.GetApendedString((String)ClassInitializefuncName);


						Classinfo->_ClassFieldInit = _IR_Builder.NewFunc(funcName, IRTypes::Void);
						Classinfo->_ClassFieldInit->NewBlock(".");
						{
							auto Classtype = TypeSymbol(_ClassStack.top().Syb->ID);
							Classtype._IsAddress = true;

							IRPar ThisPar;
							ThisPar.identifier = _IR_Builder.ToID(ThisSymbolName);
							ThisPar.type = IR_ConvertToIRType(Classtype);
							Classinfo->_ClassFieldInit->Pars.push_back(ThisPar);
						}
					}


					_IR_LookingAtIRFunc = Classinfo->_ClassFieldInit;
					_IR_LookingAtIRBlock = _IR_LookingAtIRFunc->Blocks.front().get();

					auto pos = _IR_LookingAtIRBlock->InsCount() ? _IR_LookingAtIRBlock->GetIndex() : 0;
					Debug_Add_SetLineNumber(NeverNullptr(node._Name.token), pos);

				
					{
						auto Classinfo = _ClassStack.top().Info;
						auto classSb = _ClassStack.top().Syb;
						auto Classtype = TypeSymbol(classSb->ID);
						auto GG = classSb->Get_Info<ClassInfo>();
						auto IndexField = GG->GetFieldIndex(ScopeHelper::GetNameFromFullName(syb->FullName)).value();

						IRStruct* V = _IR_Builder.GetSymbol(IR_Build_ConvertToIRClassIR(*classSb))->Get_ExAs<IRStruct>();
						auto output = _IR_LookingAtIRBlock->New_Member_Dereference(&_IR_LookingAtIRFunc->Pars[0], IR_ConvertToIRType(classSb->ID), IndexField);

						auto Func = Type_GetFunc(syb->VarType, {},NeverNullptr(node._Name.token));
						Func.ThisPar = Get_FuncInfo::ThisPar_t::PushFromLast;
						_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoadPtr(output);

						IR_Build_FuncCall(syb->VarType, Func, {});
					}

					

					_IR_LookingAtIRFunc = oldIRFunc;
					_IR_LookingAtIRBlock = oldblock;
				}
			}

		}
	}

	if (_PassType == PassType::FixedTypes)
	{
		syb = Symbol_GetSymbol(sybId).value();
		if (node._Expression._Value)
		{
			OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);

			syb->SetTovalid();

			auto& VarType = syb->VarType;


			auto Ex = _LastExpressionType;
			auto Token = NeverNullptr(node._Type._name._ScopedName.back()._token);
			Type_DeclareVariableTypeCheck(VarType, Ex, Token);

			bool doesimplconv = !Type_AreTheSameWithOutMoveAndimmutable(VarType, Ex);
			if (doesimplconv)
			{
				auto f = Symbol_GetAnImplicitConvertedFunc(VarType, Ex);
				if (f.has_value()) 
				{
					TypeSymbol ToFindSymbol = f.value()->Pars[1].Type;
					ToFindSymbol._Isimmutable = false;
					DelareVarableImplicit.AddValue(sybId, ToFindSymbol);
				}
			}
		

	
			

			if (syb->Type == SymbolType::ConstantExpression && !VarType.IsNull())
			{
				ConstantExpressionInfo* Info = syb->Get_Info<ConstantExpressionInfo>();

				_LookingForTypes.push(VarType);
				auto Item = Eval_EvaluateToAnyType(node._Expression);
				_LookingForTypes.pop();

				if (Item)
				{
					Info->Ex = std::move(Item.value().EvaluatedObject);
				}
			}
		}
		else
		{
			if (!IsField) {
				syb->SetToInvalid();
			}
		}


		if (IsField)
		{
			auto& Class = *_ClassStack.top().Info;
			auto Field = Class.GetField(ScopeHelper::GetNameFromFullName(FullName));
			if (Field)
			{
				auto& Item = (*Field);
				Item->Type = syb->VarType;

				bool hasdefaultConstructor = Type_HasDefaultConstructorFunc(syb->VarType);
				if (node._Expression._Value || hasdefaultConstructor)
				{
					Class._WillHaveFieldInit = true;
				}
				if (Symbol_HasDestructor(Item->Type))
				{
					Class._WillHaveFielddeInit = true;
				}
				if (Type_HasCopyFunc(Item->Type) && !Type_IsPrimitive(Item->Type))
				{
					Class._ClassAutoGenerateCopyConstructor = true;
				}
				if (HasMoveContructerHasIRFunc(Item->Type) && !Type_IsPrimitive(Item->Type))
				{
					Class._ClassAutoGenerateMoveConstructor = true;
				}
			}


		}
	}
	_LookingForTypes.pop();

	if (_PassType == PassType::BuidCode && node._Expression._Value)
	{
		if (syb->Type != SymbolType::ConstantExpression)
		{
			Debug_Add_SetLineNumber(NeverNullptr(node._Name.token), _IR_LookingAtIRBlock->GetIndex());


			IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, syb->VarType);

			
			IR_Build_OnStoreVarable(IsStructObjectPassRef, OnVarable, syb, sybId);

			FileDependency_AddDependencyToCurrentFile(syb->VarType);


			//
			if (syb->Type == SymbolType::StaticVarable)
			{
				_IR_LookingAtIRFunc = oldIRFunc;
				_IR_LookingAtIRBlock = oldblock;
			}
			else if (syb->Type == SymbolType::ThreadVarable)
			{
				_IR_LookingAtIRFunc = oldIRFunc;
				_IR_LookingAtIRBlock = oldblock;
			}
			else if (syb->Type == SymbolType::Class_Field)
			{
				_IR_LookingAtIRFunc = oldIRFunc;
				_IR_LookingAtIRBlock = oldblock;
			}

		}
		else
		{
			if (syb->VarType._TypeInfo == TypeInfoPrimitive::Null)//dont feel like dealing with type info right now
			{
				auto data = syb->Get_Info<ConstantExpressionInfo>();

				auto& eval = _Lib.Get_Assembly().AddEvalVarable((String)node._Name.token->Value._String, syb->FullName);
				eval.IsExported = node._IsExport ? ExportType::Exported : ExportType::NotExported;
				eval.Value._Data.Resize(data->Ex.ObjectSize);
				memcpy(eval.Value._Data.Get_Data(), data->Ex.Object_AsPointer.get(), data->Ex.ObjectSize);

				eval.Value._Type = Assembly_ConvertToType(syb->VarType);
			}
		}
	}

	syb->PassState = _PassType;
}
void SystematicAnalysis::OnDeclareStaticVariableNode(const DeclareStaticVariableNode& node)
{
	OnDeclareVariablenode(node._Variable, DeclareStaticVariableNode_t::Static);
}
void SystematicAnalysis::OnDeclareThreadVariableNode(const DeclareThreadVariableNode& node)
{
	OnDeclareVariablenode(node._Variable, DeclareStaticVariableNode_t::Thread);
}
void SystematicAnalysis::OnDeclareEvalVariableNode(const DeclareEvalVariableNode& node)
{
	OnDeclareVariablenode(node._Variable, DeclareStaticVariableNode_t::Eval);
}
void SystematicAnalysis::IR_Build_Member_Store(const GetMemberTypeSymbolFromVar_t& In, IRInstruction* Value)
{
	_IR_LookingAtIRBlock->NewStore(IR_Build_Member_Store(In), Value);
}

UCodeLangFrontEnd

#endif
