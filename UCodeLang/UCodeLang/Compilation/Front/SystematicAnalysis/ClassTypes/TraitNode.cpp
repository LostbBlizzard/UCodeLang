#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart
void SystematicAnalysis::OnTrait(const TraitNode& node)
{
	const bool IsgenericInstantiation = _Generic_GenericSymbolStack.size() && _Generic_GenericSymbolStack.top().NodeTarget == &node;
	const bool Isgeneric = node._generic._Values.size();
	const bool Isgeneric_t = Isgeneric && IsgenericInstantiation == false;
	const bool CheckgenericForErr = (Isgeneric_t && (_PassType == PassType::GetTypes || _PassType == PassType::FixedTypes));

	const String ClassName = IsgenericInstantiation ? ScopeHelper::GetNameFromFullName(_Generic_GenericSymbolStack.top()._IR_GenericFuncName) : node._Name.AsString();

	_Table.AddScope(ClassName);
	UCodeLangDefer(_Table.RemoveScope();)
		SymbolID sybId = Symbol_GetSymbolID(node);//Must be pass AddScope thats how GetSymbolID works for Generics.

	auto& Syb = _PassType == PassType::GetTypes ?
		Symbol_AddSymbol(Isgeneric_t ? SymbolType::Generic_Trait : SymbolType::Trait_class
			, (String)ClassName, _Table._Scope.ThisScope, node._Access) :
		*Symbol_GetSymbol(sybId);

	//we may jump to this node non linearly
	if (Syb.PassState == _PassType)
	{
		return;
	}

	if (_PassType == PassType::GetTypes)
	{
		_Table.AddSymbolID(Syb, sybId);


		TraitInfo* info = new TraitInfo();
		Syb.Info.reset(info);


		auto& SybClass = Symbol_AddSymbol(SymbolType::Type_class
			, (String)ClassName + TraitClassEnd, _Table._Scope.ThisScope + TraitClassEnd, node._Access);
		_Table.AddSymbolID(SybClass, Symbol_GetSymbolID(node._Name));

		Syb.NodePtr = node.As();

		auto TepClass = new ClassInfo();
		TepClass->FullName = SybClass.FullName;

		info->TraitClassInfo = &SybClass;
		SybClass.Info.reset(TepClass);


		SybClass.VarType.SetType(SybClass.ID);
		info->Context = Save_SymbolContextRemoveOneScopeName();
		Generic_InitGenericalias(node._generic, IsgenericInstantiation, info->_GenericData);


		if (CheckgenericForErr)
		{
			_Table.AddScope(GenericTestStr);
		}

		ClassStackInfo classStackInfo;
		classStackInfo.Syb = &SybClass;
		classStackInfo.Info = TepClass;
		_ClassStack.push(classStackInfo);


		SybClass.OutputIR = false;

		bool HasDrop = false;

		for (const auto& node : node._Nodes)
		{
			switch (node->Get_Type())
			{
			case NodeType::FuncNode:
			{
				size_t Index = _Table.Symbols.size();

				FuncNode& funcnode = *FuncNode::As(node.get());

				OnFuncNode(funcnode);

				Symbol* funcSyb = _Table.Symbols[Index].get();

				TraitFunc _Func;
				_Func.Syb = funcSyb;
				_Func.HasBody = funcnode._Body.has_value();

				info->_Funcs.push_back(_Func);

				if (funcSyb->Get_Info<FuncInfo>()->_FuncType == FuncInfo::FuncType::Drop)
				{
					HasDrop = true;
				}
			}break;
			case NodeType::DeclareVariableNode:
			{
				size_t Index = _Table.Symbols.size();

				DeclareVariableNode& funcnode = *DeclareVariableNode::As(node.get());

				OnDeclareVariablenode(funcnode, DeclareStaticVariableNode_t::ClassField);

				Symbol* funcSyb = _Table.Symbols[Index].get();

				TraitVar _Func;
				_Func.Syb = funcSyb;

				info->_Vars.push_back(_Func);
			}break;
			case NodeType::AliasNode:
			{
				size_t Index = _Table.Symbols.size();

				AliasNode& funcnode = *AliasNode::As(node.get());
				OnAliasNode(funcnode);

				Symbol* funcSyb = _Table.Symbols[Index].get();
				info->_Symbols.push_back(funcSyb);
			}
			break;
			case NodeType::ClassNode:
			{
				size_t Index = _Table.Symbols.size();

				ClassNode& funcnode = *ClassNode::As(node.get());
				OnClassNode(funcnode);

				Symbol* funcSyb = _Table.Symbols[Index].get();
				info->_Symbols.push_back(funcSyb);
			}
			break;
			default:
				UCodeLangUnreachable();
				break;
			}
		}

		bool hasanyDynamicdispatchFuncions = false;
		for (auto& item : info->_Funcs)
		{
			bool IsDynamicDispatch = item.Syb->Get_Info<FuncInfo>()->IsTraitDynamicDispatch;
			if (IsDynamicDispatch)
			{
				hasanyDynamicdispatchFuncions = true;
				break;
			}
		}
		if (!HasDrop && hasanyDynamicdispatchFuncions)
		{
			String FuncDropFunc = _Table._Scope.ThisScope;
			ScopeHelper::GetApendedString(FuncDropFunc, ClassDestructorFunc);

			TraitFunc _Func;
			_Func.Syb = &Symbol_AddSymbol(SymbolType::Func, ClassDestructorFunc, FuncDropFunc, AccessModifierType::Public);

			FuncInfo* Funcinfo = new FuncInfo();
			_Func.Syb->Info.reset(Funcinfo);

			Funcinfo->_FuncType = FuncInfo::FuncType::Drop;
			Funcinfo->FullName = FuncDropFunc;


			TypeSymbol FuncPtr = SybClass.ID;
			FuncPtr._IsAddress = true;
			Funcinfo->Pars.push_back({ false,FuncPtr });
			Funcinfo->Ret = TypesEnum::Void;
			Funcinfo->FrontParIsUnNamed = true;
			Funcinfo->IsTraitDynamicDispatch = true;

			_Func.HasBody = false;
			info->_Funcs.push_back(_Func);
		}


		if (CheckgenericForErr)
		{
			_Table.RemoveScope();
		}
		_ClassStack.pop();
	}
	else if (_PassType == PassType::FixedTypes)
	{
		TraitInfo* info = Syb.Get_Info<TraitInfo>();

		if (Isgeneric_t)
		{
			auto& GenericList = node._generic;
			Generic_GenericAliasFixTypes(GenericList, IsgenericInstantiation, info->_GenericData);
		}

		auto TepClass = info->TraitClassInfo->Get_Info<ClassInfo>();


		ClassStackInfo classStackInfo;
		classStackInfo.Syb = Symbol_GetSymbol(Symbol_GetSymbolID(node._Name)).value();
		classStackInfo.Info = TepClass;
		_ClassStack.push(classStackInfo);

		if (CheckgenericForErr)
		{
			_Table.AddScope(GenericTestStr);
		}
		CaptureErrorContext errorcontext = GetErrorCaptureContext();

		for (const auto& node : node._Nodes)
		{
			switch (node->Get_Type())
			{
			case NodeType::FuncNode:
			{
				FuncNode& funcnode = *FuncNode::As(node.get());
				OnFuncNode(funcnode);
			}
			break;
			case NodeType::DeclareVariableNode:
			{
				DeclareVariableNode& funcnode = *DeclareVariableNode::As(node.get());
				OnDeclareVariablenode(funcnode, DeclareStaticVariableNode_t::ClassField);
			}
			break;
			case NodeType::AliasNode:
			{
				AliasNode& funcnode = *AliasNode::As(node.get());
				OnAliasNode(funcnode);
			}
			break;
			case NodeType::ClassNode:
			{
				ClassNode& funcnode = *ClassNode::As(node.get());
				OnClassNode(funcnode);
			}
			break;
			default:
				UCodeLangUnreachable();
				break;
			}
		}


		if (CheckgenericForErr)
		{
			_Table.RemoveScope();
		}
		_ClassStack.pop();

		bool ispublic = node._Access == AccessModifierType::Public;
		if (node._IsExport && ispublic)
		{
			for (size_t i = 0; i < info->_Vars.size(); i++)
			{
				auto& cfield = info->_Vars[i];

				bool ispublic = false;
				const Token* t = nullptr;
				{
					size_t fieldcount = 0;
					for (auto& Item : node._Nodes)
					{
						if (Item->Get_Type() == NodeType::DeclareVariableNode)
						{
							auto* declar = DeclareVariableNode::As(Item.get());
							if (fieldcount == i)
							{
								t = declar->_Name.token;
								ispublic = declar->_Access == AccessModifierType::Public;
								break;
							}
							fieldcount++;
						}
					}
				}

				if (ispublic)
				{
					if (!Type_IsTypeExported(cfield.Syb->VarType))
					{
						LogError_TypeIsNotExport(NeverNullptr(t), cfield.Syb->VarType, NeverNullptr(&Syb));
					}
				}
			}
		}
		if (!ispublic && node._IsExport)
		{
			LogError_ExportIsPrivate(NeverNullptr(node._Name.token), NeverNullptr(&Syb));
		}

		if (Isgeneric_t)
		{
			bool hasanyerrors = ErrorCaptureHasErrors(errorcontext);
			if (hasanyerrors)
			{
				Syb.ValidState = SymbolValidState::Invalid;
			}
		}
	}
	else if (_PassType == PassType::BuidCode)
	{
		if (Isgeneric_t)
		{
			TraitInfo* info = Syb.Get_Info<TraitInfo>();

			String_view Text = _LookingAtFile->FileText;

			String ClassStr = "$";
			ClassStr += node._Name.token->Value._String;

			size_t offset = 0;
			if (node.EndOfClass->Type == TokenType::Semicolon)
			{
				offset += 1;
			}


			String_view ClassBody =
				String_view(&Text[node._Name.token->OnPos],
					node.EndOfClass->OnPos - node._Name.token->OnPos + offset);

			GenericClass_Data& VClass = _Lib.Get_Assembly().AddGenericClass((String)ScopeHelper::GetNameFromFullName(Syb.FullName), RemoveSymboolFuncOverloadMangling(Syb.FullName));

			VClass.Base.Implementation = ClassStr + String(ClassBody);
			VClass.Base.Implementation += "\n\n";
			VClass.AccessModifier = Syb.Access;
			VClass.IsExported = node._IsExport ? ExportType::Exported : ExportType::NotExported;
			VClass.UseStatments = Generic_GetCurrentUseStatements();
		}
		else
		{
			//
			TraitInfo* info = Syb.Get_Info<TraitInfo>();
			bool hasanyDynamicdispatchFuncions = false;
			for (auto& item : info->_Funcs)
			{
				bool IsDynamicDispatch = item.Syb->Get_Info<FuncInfo>()->IsTraitDynamicDispatch;
				if (IsDynamicDispatch)
				{
					hasanyDynamicdispatchFuncions = true;
					break;
				}
			}
			if (hasanyDynamicdispatchFuncions)
			{
				auto StructVtablueClass = _IR_Builder.NewStruct(_IR_Builder.ToID(Str_GetTraitVStructTableName(Syb.FullName)));

				for (auto& Item : info->_Funcs)
				{
					FuncInfo* ItemInfo = Item.Syb->Get_Info<FuncInfo>();
					if (ItemInfo->IsTraitDynamicDispatch)
					{
						auto StrFunc = GetTepFuncPtrName(ItemInfo);
						auto PtrFunc = GetTepFuncPtrSyb(StrFunc, ItemInfo).value();
						PtrFunc->FullName = StrFunc;
						TypeSymbol PtrType = PtrFunc->ID;

						auto IRType = IR_ConvertToIRType(PtrType);

						IRStructField V;
						V.Type = IRType;
						StructVtablueClass->Fields.push_back(V);
					}
				}
			}


			Trait_Data& TraitData = _Lib.Get_Assembly().AddTrait(ScopeHelper::GetNameFromFullName(Syb.FullName), RemoveSymboolFuncOverloadMangling(Syb.FullName));
			TraitData.TypeID = Type_GetTypeID(TypesEnum::CustomType, Syb.ID);
			TraitData.AccessModifier = Syb.Access;
			TraitData.IsExported = node._IsExport ? ExportType::Exported : ExportType::NotExported;

			TraitData.Fields.reserve(info->_Vars.size());
			for (auto& Item : info->_Vars)
			{
				auto Name = ScopeHelper::GetNameFromFullName(Item.Syb->FullName);
				auto& VarType = Item.Syb->VarType;

				ClassField field;
				field.Name = Name;
				field.Type = Assembly_ConvertToType(VarType);

				TraitData.Fields.push_back(std::move(field));
			}

			TraitData.Methods.reserve(info->_Funcs.size());
			for (auto& Item : info->_Funcs)
			{
				auto Name = ScopeHelper::GetNameFromFullName(Item.Syb->FullName);
				FuncInfo* funcinfo = Item.Syb->Get_Info<FuncInfo>();

				TraitMethod method;

				method.method.FullName = Item.Syb->FullName;
				method.method.IsThisFunction = funcinfo->FrontParIsUnNamed;
				method.method.IsUnsafe = funcinfo->IsUnsafe;
				method.method.IsRemoved = funcinfo->IsRemoved;
				method.method.IsExternC = funcinfo->IsExternC;
				method.method.IsTraitDynamicDispatch = funcinfo->IsTraitDynamicDispatch;
				method.method.RetType = Assembly_ConvertToType(funcinfo->Ret);

				bool isexport = true;
				if (Item.Syb->NodePtr)
				{
					isexport = Item.Syb->Get_NodeInfo<FuncNode>()->_Signature._IsExport;
				}

				method.method.IsExport = isexport ? ExportType::Exported : ExportType::NotExported;
				method.method.ParsType.resize(funcinfo->Pars.size());
				for (size_t i = 0; i < funcinfo->Pars.size(); i++)
				{
					method.method.ParsType[i].IsOutPar = funcinfo->Pars[i].IsOutPar;
					method.method.ParsType[i].Type = Assembly_ConvertToType(funcinfo->Pars[i].Type);
				}

				if (Item.HasBody)
				{
					const FuncNode* body = Item.Syb->Get_NodeInfo<FuncNode>();

					auto nametoken = body->_Signature._Name.token;
					auto endtoken = body->EndOfFunc;

					String_view filetext = this->_LookingAtFile->FileText;

					method.FuncBody = GetImplementationFromFunc(filetext, nametoken, endtoken);
				}

				TraitData.Methods.push_back(std::move(method));
			}


			TraitData.Symbols.reserve(info->_Symbols.size());

			for (auto& Item : info->_Symbols)
			{
				TraitSymbol method;
				method.AccessModifier = Item->Access;

				String implementtion = "";
				bool isexport;

				switch (Item->Type)
				{
				case SymbolType::Type_class:
				{
					auto nod = Item->Get_NodeInfo<ClassNode>();
					isexport = nod->_IsExport;

					String_view Text = _LookingAtFile->FileText;

					String ClassStr = "$";
					ClassStr += nod->_className.token->Value._String;

					size_t offset = 0;
					if (nod->EndOfClass->Type == TokenType::Semicolon)
					{
						offset += 1;
					}


					String_view ClassBody =
						String_view(&Text[nod->_className.token->OnPos],
							nod->EndOfClass->OnPos - nod->_className.token->OnPos + offset);

					implementtion = ClassStr + String(ClassBody);
					implementtion += "\n\n";
				}
				break;
				case SymbolType::Type_alias:
				{
					auto nod = Item->Get_NodeInfo<AliasNode>();
					isexport = nod->IsExport;

					String_view Text = _LookingAtFile->FileText;

					String ClassStr = "$";
					ClassStr += nod->_AliasName.token->Value._String;

					String_view ClassBody =
						String_view(&Text[nod->_AliasName.token->OnPos],
							nod->EndOfClass->OnPos - nod->_AliasName.token->OnPos + 1);

					implementtion = ClassStr + String(ClassBody);
					implementtion += "\n\n";
				}
				break;
				default:
					UCodeLangUnreachable();
					break;
				}
				method.IsExported = isexport;
				method.Implementation = std::move(implementtion);

				TraitData.Symbols.push_back(std::move(method));
			}

			TraitData.GenericAlias.reserve(info->_GenericAlias.size());
			for (auto& Item : info->_GenericAlias)
			{
				TraitAlias val;
				val.AliasName = Item.Name;
				val.Type = Assembly_ConvertToType(Item.Type);

				TraitData.GenericAlias.push_back(std::move(val));
			}
		}
	}

	Syb.PassState = _PassType;
}

void SystematicAnalysis::Symbol_InheritTrait(NeverNullPtr<Symbol> Syb, ClassInfo* ClassInfo, const NeverNullPtr<Symbol> Trait, const NeverNullPtr<Token> ClassNameToken)
{
	const TraitInfo* Traitinfo = Trait->Get_Info<TraitInfo>();

	auto ID = ClassInfo->Get_InheritedTypesIndex(Trait.value());
	ClassInfo_InheritTypeInfo& IDSyb = ClassInfo->_InheritedTypes[ID.value()];

	bool HasErr = false;



	for (auto& Item : Traitinfo->_Vars)
	{
		auto VarName = ScopeHelper::GetNameFromFullName((String_view)Item.Syb->FullName);


		for (auto& Field : ClassInfo->Fields)
		{
			if (Field.Name == VarName)
			{
				HasErr = true;
				LogError(ErrorCodes::Redefinition, "can't inherit '" + Trait->FullName + "' because " + ClassInfo->FullName + " already has a field named '" + (String)VarName + "'", ClassNameToken);
				continue;
			}
		}

		ClassInfo->AddField(VarName, Item.Syb->VarType);

		auto& Fieldsyb = Symbol_AddSymbol(SymbolType::Class_Field, (String)VarName,
			ScopeHelper::ApendedStrings(ClassInfo->FullName, VarName), Item.Syb->Access);
		Fieldsyb.VarType = Item.Syb->VarType;

		if (true)
		{
			ClassInfo->_WillHaveFieldInit = true;
		}
		if (Symbol_HasDestructor(Item.Syb->VarType))
		{
			ClassInfo->_WillHaveFielddeInit = true;
		}

	}

	for (auto& Item : Traitinfo->_Funcs)
	{
		if (Item.HasBody)
		{
			ClassInfo_InheritTypeInfo::AddedFuncInfo InfoB;
			InfoB.FuncNode = Item.Syb->NodePtr;
			IDSyb.AddedFuncs.push_back(InfoB);


			ClassInfo_InheritTypeInfo::FuncIndex V;
			V.Index = IDSyb.AddedFuncs.size() - 1;
			V.Type = ClassInfo_InheritTypeInfo::FuncType::Added;
			IDSyb.Funcs.push_back(V);
		}
		else
		{
			FuncInfo* Info = Item.Syb->Get_Info<FuncInfo>();

			auto FuncName = RemoveSymboolFuncOverloadMangling(Info->Get_Name());
			auto& List = GetSymbolsWithName(FuncName);

			bool HasFunc = false;

			for (auto& ItemL : List)
			{
				if (HasFunc)
				{
					break;
				}
				if (ItemL->Type == SymbolType::Func)
				{
					FuncInfo* ItemFunc = ItemL->Get_Info<FuncInfo>();


					if (ItemFunc->Pars.size() == Info->Pars.size())
					{
						if (Info->Pars.size() == 0)
						{
							HasFunc = true;
						}

						for (size_t i = 0; i < Info->Pars.size(); i++)
						{
							const auto& TraitPar = Info->Pars[i];
							const auto& FuncPar = ItemFunc->Pars[i];

							auto SubTraitParType = TraitPar;

							if (TraitPar.Type._CustomTypeSymbol == Traitinfo->TraitClassInfo->ID)
							{
								SubTraitParType.Type._CustomTypeSymbol = Syb->ID;
							}

							if (!Type_AreTheSame(SubTraitParType, FuncPar))
							{
								goto ConstinueSybList;
							}


							HasFunc = true;
							//
							ClassInfo_InheritTypeInfo::AddedFuncInfo InfoB;
							InfoB.FuncNode = ItemL->NodePtr;
							InfoB.Func = ItemL;
							IDSyb.OverLoadedFuncs.push_back(InfoB);


							ClassInfo_InheritTypeInfo::FuncIndex V;
							V.Index = IDSyb.OverLoadedFuncs.size() - 1;
							V.Type = ClassInfo_InheritTypeInfo::FuncType::OverLoaded;
							IDSyb.Funcs.push_back(V);
							break;
						}

					}


				ConstinueSybList:continue;//check if same thing
				}
			}


			if (!HasFunc) {
				LogError_MissingFunctionforTrait(FuncName, Info, Trait, ClassNameToken);
				HasErr = true;
			}
		}
	}


	if (!HasErr)
	{

		//
		{
			ClassStackInfo _ClStack;
			_ClStack._InStatements = false;
			_ClStack.Syb = Syb.value();
			_ClStack.Info = ClassInfo;
			_ClassStack.push(_ClStack);
		}

		auto oldpass = _PassType;
		_PassType = PassType::GetTypes;

		{
			Push_ExtendedErr("Were this = " + Syb->FullName + ".When instantiateing trait " + Trait->FullName, ClassNameToken);
		}

		auto OnTraitSymbol = [this](Symbol* Item)
			{
				if (Item->Type == SymbolType::Type_alias || Item->Type == SymbolType::Hard_Type_alias || Item->Type == SymbolType::Generic_Alias)
				{
					OnAliasNode(*Item->Get_NodeInfo<AliasNode>());
				}
				else if (Item->Type == SymbolType::Type_class || Item->Type == SymbolType::Generic_class)
				{
					OnClassNode(*Item->Get_NodeInfo<ClassNode>());
				}
				else
				{
					UCodeLangUnreachable();
				}

			};

		{
			_Table.AddUseing(Trait->FullName);

			for (auto& Item : Traitinfo->_Symbols)
			{
				auto SybsIndex = _Table.Symbols.size();

				OnTraitSymbol(Item);


				auto Symbol = _Table.Symbols[SybsIndex].get();
				_GeneratedTraitSymbols.push_back(Symbol);
			}
			_Table.Useings.pop_back();
		}

		auto oldcurrentfile = this->_LookingAtFile;
		_LookingAtFile = Trait->_File;

		for (auto& Item : IDSyb.AddedFuncs)
		{
			const FuncNode& func = *(FuncNode*)Item.FuncNode;

			auto SybsIndex = _Table.Symbols.size();

			OnFuncNode(func);


			Item.Func = _Table.Symbols[SybsIndex].get();

		}

		_PassType = PassType::FixedTypes;

		{
			_Table.AddUseing(Trait->FullName);
			const TraitInfo* info = Trait->Get_Info<TraitInfo>();

			for (auto& Item : info->_GenericAlias)
			{
				String scope = Trait->FullName;
				ScopeHelper::GetApendedString(scope, Item.Name);
				auto s = Symbol_GetSymbol(scope, SymbolType::Generic_Alias).value();
				s->Access = AccessModifierType::Public;
			}	
			for (auto& Item : Traitinfo->_Symbols)
			{
				OnTraitSymbol(Item);
			}
			for (auto& Item : info->_GenericAlias)
			{
				String scope = Trait->FullName;
				ScopeHelper::GetApendedString(scope, Item.Name);
				auto s = Symbol_GetSymbol(scope, SymbolType::Generic_Alias).value();
				s->Access = AccessModifierType::Private;
			}
			_Table.Useings.pop_back();
		}

		for (auto& Item : IDSyb.AddedFuncs)
		{
			const FuncNode& func = *(FuncNode*)Item.FuncNode;
			OnFuncNode(func);
		}

		_LookingAtFile = oldcurrentfile;
		{
			_PassType = oldpass;
			_ClassStack.pop();
		}
		{
			Pop_ExtendedErr();
		}
	}
}
void SystematicAnalysis::Symbol_BuildTrait(const NeverNullPtr<Symbol> Syb, ClassInfo* ClassInfo, const NeverNullPtr<Symbol> Trait, const NeverNullPtr<Token> ClassNameToken)
{

	auto ID = ClassInfo->Get_InheritedTypesIndex(Trait.value());
	ClassInfo_InheritTypeInfo& IDSyb = ClassInfo->_InheritedTypes[ID.value()];

	{
		ClassStackInfo _ClStack;
		_ClStack._InStatements = false;
		_ClStack.Syb = Syb.value();
		_ClStack.Info = ClassInfo;
		_ClassStack.push(_ClStack);
	}

	{
		Push_ExtendedErr("Were this = " + Syb->FullName + ".When instantiateing trait " + Trait->FullName, ClassNameToken);
	}
	for (auto& Item : IDSyb.AddedFuncs)
	{
		const FuncNode& func = *(FuncNode*)Item.FuncNode;
		OnFuncNode(func);
	}
	{
		Pop_ExtendedErr();
	}

	if (IDSyb.Syb->PassState == PassType::FixedTypes)
	{
		auto& symbol = IDSyb.Syb;
		auto info = symbol->Get_Info<TraitInfo>();
		auto context = SaveAndMove_SymbolContext();

		NullablePtr<GenericFuncInfo> GenericFunc;
		for (auto& Item : _Generic_GeneratedGenericSymbol)//I know this is slow i just need something working
		{
			if (Item.ID == symbol->ID)
			{
				GenericFunc = &Item.Info;
				break;
			}
		}

		if (GenericFunc.has_value())
		{
			_Generic_GenericSymbolStack.push(*GenericFunc.value());
		}
		Set_SymbolContext(info->Context.value());

		OnTrait(*symbol->Get_NodeInfo<TraitNode>());

		Set_SymbolContext(std::move(context));

		if (GenericFunc.has_value())
		{
			_Generic_GenericSymbolStack.pop();
		}

		UCodeLangAssert(IDSyb.Syb->PassState == PassType::BuidCode);
	}
	auto& symbol = IDSyb.Syb;
	auto info = symbol->Get_Info<TraitInfo>();
	bool hasanyDynamicdispatchFuncions = false;
	for (auto& item : info->_Funcs)
	{
		bool IsDynamicDispatch = item.Syb->Get_Info<FuncInfo>()->IsTraitDynamicDispatch;
		if (IsDynamicDispatch)
		{
			hasanyDynamicdispatchFuncions = true;
			break;
		}

	}
	if (hasanyDynamicdispatchFuncions)
	{
		String VTableName = Str_GetClassWithTraitVTableName(Syb->FullName, Trait->FullName);
		auto StaticVarableToID = _IR_Builder.ToID(VTableName);

		auto StaticVarableType = IRType(_IR_Builder.GetSymbol(_IR_Builder.ToID(Str_GetTraitVStructTableName(Trait->FullName)))->identifier);

		auto StaticVarable = _IR_Builder.NewStaticVarable(StaticVarableToID, StaticVarableType);


		auto oldIRFunc = _IR_LookingAtIRFunc;
		auto oldblock = _IR_LookingAtIRBlock;



		if (_IR_Builder._StaticInit.Blocks.size() == 0)
		{
			_IR_Builder._StaticInit.NewBlock(".");
		}

		_IR_LookingAtIRFunc = &_IR_Builder._StaticInit;
		_IR_LookingAtIRBlock = _IR_LookingAtIRFunc->Blocks.front().get();

		//

		auto Ptr = _IR_LookingAtIRBlock->NewLoadPtr(StaticVarableToID);
		size_t StaticVarMemberIndex = 0;
		for (size_t i = 0; i < IDSyb.Funcs.size(); i++)
		{
			auto& Item = IDSyb.Funcs[i];

			Symbol* Func = Item.Type == ClassInfo_InheritTypeInfo::FuncType::Added ? IDSyb.AddedFuncs[Item.Index].Func : IDSyb.OverLoadedFuncs[Item.Index].Func;

			bool IsDynamicDispatch = info->_Funcs[i].Syb->Get_Info<FuncInfo>()->IsTraitDynamicDispatch;

			if (IsDynamicDispatch)
			{
				auto Member = _IR_LookingAtIRBlock->New_Member_Dereference(Ptr, StaticVarableType, StaticVarMemberIndex);
				_IR_LookingAtIRBlock->NewStore(Member, _IR_LookingAtIRBlock->NewLoadFuncPtr(IR_GetIRID(Func->Get_Info<FuncInfo>())));
				StaticVarMemberIndex++;
			}
		}
		//
		_IR_LookingAtIRFunc = oldIRFunc;
		_IR_LookingAtIRBlock = oldblock;
	}


	{
		_ClassStack.pop();
	}
}
bool SystematicAnalysis::Type_CanDoTypeToTrait(const TypeSymbol& TypeToCheck, const TypeSymbol& Type)
{
	if (Type._IsDynamic && Type._Type == TypesEnum::CustomType)
	{
		auto SymbolB = Symbol_GetSymbol(Type).value();
		TraitInfo* Info = SymbolB->Get_Info<TraitInfo>();
		auto TypeSybOp = Symbol_GetSymbol(TypeToCheck);

		if (TypeSybOp)
		{
			auto TypeSyb = TypeSybOp.value();
			if (TypeSyb->Type == SymbolType::Type_class)
			{
				ClassInfo* ClassF = TypeSyb->Get_Info<ClassInfo>();

				auto Indexo = ClassF->Get_InheritedTypesIndex(SymbolB.value());

				if (Indexo.has_value())
				{
					return true;
				}
			}
		}
	}
	return false;
}


UCodeLangFrontEnd

#endif
