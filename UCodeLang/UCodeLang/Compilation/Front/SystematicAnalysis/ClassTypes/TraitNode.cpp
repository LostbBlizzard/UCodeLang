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
	SymbolID sybId = Symbol_GetSymbolID(node);//Must be pass AddScope thats how GetSymbolID works for Generics.

	auto& Syb = _PassType == PassType::GetTypes ?
		Symbol_AddSymbol(Isgeneric_t ? SymbolType::Generic_Trait : SymbolType::Trait_class
			, (String)ClassName, _Table._Scope.ThisScope, node._Access) :
		*Symbol_GetSymbol(sybId);

	//we may jump to this node non linearly
	if (Syb.PassState == _PassType)
	{
		_Table.RemoveScope();
		return;
	}

	if (_PassType == PassType::GetTypes)
	{
		_Table.AddSymbolID(Syb, sybId);


		TraitInfo* info = new TraitInfo();
		Syb.Info.reset(info);


		auto& SybClass = Symbol_AddSymbol(SymbolType::Type_class
			, (String)ClassName + "%Class", _Table._Scope.ThisScope + "%Class", node._Access);
		_Table.AddSymbolID(SybClass, Symbol_GetSymbolID(node._Name));

		Syb.NodePtr = node.As();

		auto TepClass = new ClassInfo();
		TepClass->FullName = SybClass.FullName;

		info->TraitClassInfo = &SybClass;
		SybClass.Info.reset(TepClass);


		SybClass.VarType.SetType(SybClass.ID);
		info->Conext = Save_SymbolContextRemoveOneScopeName();
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
			default:break;
			}
		}


		if (!HasDrop)
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

		auto TepClass = info->TraitClassInfo->Get_Info<ClassInfo>();

		if (CheckgenericForErr)
		{
			_Table.AddScope(GenericTestStr);
		}

		ClassStackInfo classStackInfo;
		classStackInfo.Syb = Symbol_GetSymbol(Symbol_GetSymbolID(node._Name)).value();
		classStackInfo.Info = TepClass;
		_ClassStack.push(classStackInfo);

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
			default:break;
			}
		}


		if (CheckgenericForErr)
		{
			_Table.RemoveScope();
		}
		_ClassStack.pop();
	}
	else if (_PassType == PassType::BuidCode && !Isgeneric_t)
	{
		//
		TraitInfo* info = Syb.Get_Info<TraitInfo>();
		auto StructVtablueClass = _IR_Builder.NewStruct(_IR_Builder.ToID(Str_GetTraitVStructTableName(Syb.FullName)));

		for (auto& Item : info->_Funcs)
		{
			FuncInfo* ItemInfo = Item.Syb->Get_Info<FuncInfo>();
			auto StrFunc = GetTepFuncPtrName(ItemInfo);
			auto PtrFunc = GetTepFuncPtrSyb(StrFunc, ItemInfo).value();
			PtrFunc->FullName = StrFunc;
			TypeSymbol PtrType = PtrFunc->ID;

			auto IRType = IR_ConvertToIRType(PtrType);

			IRStructField V;
			V.Type = IRType;
			StructVtablueClass->Fields.push_back(V);
		}


		Trait_Data& TraitData = _Lib.Get_Assembly().AddTrait(ScopeHelper::GetNameFromFullName(Syb.FullName), Syb.FullName);
		TraitData.TypeID = Type_GetTypeID(TypesEnum::CustomType, Syb.ID);

		for (auto& Item : info->_Vars)
		{
			auto Name = ScopeHelper::GetNameFromFullName(Item.Syb->FullName);
			auto& VarType = Item.Syb->VarType;

			ClassField field;
			field.Name = Name;
			field.Type = Assembly_ConvertToType(VarType);

			TraitData.Fields.push_back(std::move(field));
		}

		for (auto& Item : info->_Funcs)
		{
			auto Name = ScopeHelper::GetNameFromFullName(Item.Syb->FullName);
			FuncInfo* funcinfo = Item.Syb->Get_Info<FuncInfo>();

			TraitMethod method;

			method.method.FullName = funcinfo->FullName;
			method.method.IsThisFuncion = funcinfo->FrontParIsUnNamed;
			method.method.IsUnsafe = funcinfo->IsUnsafe;
			method.method.IsRemoved = funcinfo->IsRemoved;
			method.method.IsExternC = funcinfo->IsExternC;
			method.method.RetType = Assembly_ConvertToType(funcinfo->Ret);

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
	}


	_Table.RemoveScope();
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
				LogError(ErrorCodes::Redefinition, "cant inherit '" + Trait->FullName + "' because " + ClassInfo->FullName + " already has a field named '" + (String)VarName + "'", ClassNameToken);
				continue;
			}
		}

		ClassInfo->AddField(VarName, Item.Syb->VarType);

		auto& Fieldsyb = Symbol_AddSymbol(SymbolType::Class_Field, (String)VarName,
			ScopeHelper::ApendedStrings(ClassInfo->FullName, VarName) , Item.Syb->Access);
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

			auto FuncName = Info->Get_Name();
			auto& List = GetSymbolsWithName(Info->Get_Name());

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
				LogError_MissingFuncionforTrait(FuncName, Info, Trait, ClassNameToken);
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

		for (auto& Item : IDSyb.AddedFuncs)
		{
			const FuncNode& func = *(FuncNode*)Item.FuncNode;

			auto SybsIndex = _Table.Symbols.size();

			OnFuncNode(func);


			Item.Func = _Table.Symbols[SybsIndex].get();

		}


		_PassType = PassType::FixedTypes;

		for (auto& Item : IDSyb.AddedFuncs)
		{
			const FuncNode& func = *(FuncNode*)Item.FuncNode;
			OnFuncNode(func);
		}

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
		for (size_t i = 0; i < IDSyb.Funcs.size(); i++)
		{
			auto& Item = IDSyb.Funcs[i];


			Symbol* Func = Item.Type == ClassInfo_InheritTypeInfo::FuncType::Added ? IDSyb.AddedFuncs[Item.Index].Func : IDSyb.OverLoadedFuncs[Item.Index].Func;

			auto Member = _IR_LookingAtIRBlock->New_Member_Dereference(Ptr, StaticVarableType, i);
			_IR_LookingAtIRBlock->NewStore(Member, _IR_LookingAtIRBlock->NewLoadFuncPtr(IR_GetIRID(Func->Get_Info<FuncInfo>())));
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
	if (Type._IsDynamic)
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