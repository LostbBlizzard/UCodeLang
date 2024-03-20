#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::Generic_GenericFuncInstantiate(const NeverNullPtr<Symbol> Func, const Vector<TypeSymbol>& GenericInput)
{
	UCodeLangAssert(Func->Type == SymbolType::GenericFunc);



	const String NewName = Generic_SymbolGenericName(Func, GenericInput);
	const String FullName = Generic_SymbolGenericFullName(Func, GenericInput);
	const FuncInfo* FInfo = Func->Get_Info<FuncInfo>();

	{
		auto& GenericData = FInfo->_GenericData;
		Push_GenericInfo(NewName, GenericInput, Func, GenericData);
	}


	const FuncNode& FuncBase = *Func->Get_NodeInfo<FuncNode>();






	{
		Push_ExtendedErr(Generic_GetGenericExtendedErrValue(FInfo->_GenericData, FuncBase._Signature._generic, GenericInput), NeverNullptr(FuncBase._Signature._Name.token));
	}
	{
		size_t NewSymbolIndex = _Table.Symbols.size();

		auto OldContext = SaveAndMove_SymbolContext();
		auto Oldpasstype = _PassType;
		auto Olderrcount = _ErrorsOutput->Get_ErrorCount();

		Set_SymbolContext(FInfo->Context.value());
		_PassType = PassType::GetTypes;
		OnFuncNode(FuncBase);

		auto& addedSymbol = *_Table.Symbols[NewSymbolIndex].get();
		UCodeLangAssert(addedSymbol.FullName == FullName);
		UCodeLangAssert(addedSymbol.Type == SymbolType::Func);
		UCodeLangAssert(addedSymbol.PassState == PassType::GetTypes);


		if (_ErrorsOutput->Get_ErrorCount() <= Olderrcount)
		{
			_PassType = PassType::FixedTypes;
			OnFuncNode(FuncBase);
			UCodeLangAssert(addedSymbol.PassState == PassType::FixedTypes);
		}
		Set_SymbolContext(std::move(OldContext));
		_PassType = Oldpasstype;

		Pop_AddToGeneratedGenricSymbol(addedSymbol, GenericInput);
	}
	{
		Pop_ExtendedErr();
	}



	FileDependency_AddDependencyToCurrentFile(Func);
}
void SystematicAnalysis::Push_GenericInfo(const String& NewName, const Vector<TypeSymbol>& GenericInput, const
	NeverNullPtr<Symbol> Func, const Generic& GenericData)
{
	GenericFuncInfo Info;
	Info._IR_GenericFuncName = NewName;
	Info.GenericInput = &GenericInput;
	Info.NodeTarget = Func->NodePtr;
	Info.Pack = Generic_MakeTypePackSymbolIfNeeded(NewName, GenericInput, GenericData);
	_Generic_GenericSymbolStack.push(std::move(Info));
}
void SystematicAnalysis::Pop_AddToGeneratedGenricSymbol(Symbol& addedSymbol, const Vector<TypeSymbol>& GenericInput)
{

	GeneratedGenericSymbolData NewData;
	NewData.ID = addedSymbol.ID;
	NewData.Info = std::move(_Generic_GenericSymbolStack.top());
	_Generic_GenericSymbolStack.pop();

	NewData.Types = GenericInput;
	NewData.Info.GenericInput = &NewData.Types;

	_Generic_GeneratedGenericSymbol.push_back(std::move(NewData));

}

void SystematicAnalysis::Generic_TypeInstantiate(const NeverNullPtr<Symbol> Class, const Vector<TypeSymbol>& GenericInput)
{
	UCodeLangAssert(Class->Type == SymbolType::Generic_class);

	const String NewName = Generic_SymbolGenericName(Class, GenericInput);
	const String FullName = Generic_SymbolGenericFullName(Class, GenericInput);
	const ClassNode* node = ClassNode::As(Class->Get_NodeInfo<Node>());

	const ClassInfo* classInfo = Class->Get_Info<ClassInfo>();

	{
		auto& GenericData = classInfo->_GenericData;
		Push_GenericInfo(NewName, GenericInput, Class, GenericData);
	}

	{
		Push_ExtendedErr(Generic_GetGenericExtendedErrValue(classInfo->_GenericData, node->_generic, GenericInput), NeverNullptr(node->_className.token));
	}
	{
		auto OldContext = SaveAndMove_SymbolContext();
		auto Oldpasstype = _PassType;
		auto Olderrcount = _ErrorsOutput->Get_ErrorCount();

		Set_SymbolContext(classInfo->Context.value());

		size_t NewSymbolIndex = _Table.Symbols.size();
		_PassType = PassType::GetTypes;
		OnClassNode(*node);

		auto& addedSymbol = *_Table.Symbols[NewSymbolIndex].get();
		UCodeLangAssert(addedSymbol.FullName == FullName);
		UCodeLangAssert(addedSymbol.Type == SymbolType::Type_class);
		UCodeLangAssert(addedSymbol.PassState == PassType::GetTypes);

		if (_ErrorsOutput->Get_ErrorCount() <= Olderrcount)
		{
			_PassType = PassType::FixedTypes;
			OnClassNode(*node);

			UCodeLangAssert(addedSymbol.PassState == PassType::FixedTypes);
		}

		Pop_AddToGeneratedGenricSymbol(addedSymbol, GenericInput);




		//
		Set_SymbolContext(std::move(OldContext));
		_PassType = Oldpasstype;
		//
	}
	{
		Pop_ExtendedErr();
	}

	FileDependency_AddDependencyToCurrentFile(Class);
}

void SystematicAnalysis::Generic_TypeInstantiate_Trait(const NeverNullPtr<Symbol> Trait, const Vector<TypeSymbol>& GenericInput)
{
	UCodeLangAssert(Trait->Type == SymbolType::Trait_class);

	const String NewName = Generic_SymbolGenericName(Trait, GenericInput);
	const String FullName = Generic_SymbolGenericFullName(Trait, GenericInput);
	const TraitNode* node = TraitNode::As(Trait->Get_NodeInfo<Node>());

	const TraitInfo* classInfo = Trait->Get_Info<TraitInfo>();


	{
		auto& GenericData = classInfo->_GenericData;
		Push_GenericInfo(NewName, GenericInput, Trait, GenericData);
	}



	{
		Push_ExtendedErr(Generic_GetGenericExtendedErrValue(classInfo->_GenericData, node->_generic, GenericInput), NeverNullptr(node->_Name.token));
	}
	{
		auto OldContext = SaveAndMove_SymbolContext();
		auto Oldpasstype = _PassType;
		auto Olderrcount = _ErrorsOutput->Get_ErrorCount();

		Set_SymbolContext(classInfo->Context.value());

		size_t NewSymbolIndex = _Table.Symbols.size();

		_PassType = PassType::GetTypes;
		OnTrait(*node);


		auto& addedSymbol = *_Table.Symbols[NewSymbolIndex].get();
		UCodeLangAssert(addedSymbol.FullName == NewName);
		UCodeLangAssert(addedSymbol.Type == SymbolType::Trait_class);
		UCodeLangAssert(addedSymbol.PassState == PassType::GetTypes);


		if (_ErrorsOutput->Get_ErrorCount() <= Olderrcount)
		{
			_PassType = PassType::FixedTypes;
			OnTrait(*node);

			UCodeLangAssert(addedSymbol.PassState == PassType::FixedTypes);
		}

		Pop_AddToGeneratedGenricSymbol(addedSymbol, GenericInput);

		//
		Set_SymbolContext(std::move(OldContext));
		_PassType = Oldpasstype;
		//
	}
	{
		Pop_ExtendedErr();
	}

	FileDependency_AddDependencyToCurrentFile(Trait);
}

void SystematicAnalysis::Generic_TypeInstantiate_Alias(const NeverNullPtr<Symbol> Alias, const Vector<TypeSymbol>& GenericInput)
{
	UCodeLangAssert(Alias->Type == SymbolType::Generic_Alias);

	const String NewName = Generic_SymbolGenericName(Alias, GenericInput);
	const String FullName = Generic_SymbolGenericFullName(Alias, GenericInput);
	const AliasNode* node = AliasNode::As(Alias->Get_NodeInfo<Node>());

	const Generic_AliasInfo* classInfo = Alias->Get_Info<Generic_AliasInfo>();

	{
		auto& GenericData = classInfo->_GenericData;
		Push_GenericInfo(NewName, GenericInput, Alias, GenericData);
	}
	{
		Push_ExtendedErr(Generic_GetGenericExtendedErrValue(classInfo->_GenericData, node->Generic, GenericInput), NeverNullptr(node->_AliasName.token));
	}
	{
		auto OldContext = SaveAndMove_SymbolContext();
		auto Oldpasstype = _PassType;
		auto Olderrcount = _ErrorsOutput->Get_ErrorCount();

		Set_SymbolContext(classInfo->Context.value());

		size_t NewSymbolIndex = _Table.Symbols.size();

		_PassType = PassType::GetTypes;
		OnAliasNode(*node);


		auto& addedSymbol = *_Table.Symbols[NewSymbolIndex].get();

		UCodeLangAssert(addedSymbol.FullName == FullName);
		UCodeLangAssert(addedSymbol.Type == SymbolType::Type_alias
			|| addedSymbol.Type == SymbolType::Hard_Type_alias
			|| addedSymbol.Type == SymbolType::Func_ptr
			|| addedSymbol.Type == SymbolType::Hard_Func_ptr);
		UCodeLangAssert(addedSymbol.PassState == PassType::GetTypes);



		if (_ErrorsOutput->Get_ErrorCount() <= Olderrcount)
		{
			_PassType = PassType::FixedTypes;
			OnAliasNode(*node);

			UCodeLangAssert(addedSymbol.PassState == PassType::FixedTypes);
		}

		Pop_AddToGeneratedGenricSymbol(addedSymbol, GenericInput);
		//
		Set_SymbolContext(std::move(OldContext));
		_PassType = Oldpasstype;
		//
	}
	{
		Pop_ExtendedErr();
	}

	FileDependency_AddDependencyToCurrentFile(Alias);
}

void SystematicAnalysis::Generic_TypeInstantiate_Enum(const NeverNullPtr<Symbol> Enum, const Vector<TypeSymbol>& GenericInput)
{
	UCodeLangAssert(Enum->Type == SymbolType::Generic_Enum);

	const String NewName = Generic_SymbolGenericName(Enum, GenericInput);
	const String FullName = Generic_SymbolGenericFullName(Enum, GenericInput);
	const EnumNode* node = EnumNode::As(Enum->Get_NodeInfo<Node>());

	const EnumInfo* classInfo = Enum->Get_Info<EnumInfo>();

	{
		auto& GenericData = classInfo->_GenericData;
		Push_GenericInfo(NewName, GenericInput, Enum, GenericData);
	}

	{
		Push_ExtendedErr(Generic_GetGenericExtendedErrValue(classInfo->_GenericData, node->_generic, GenericInput), NeverNullptr(node->_EnumName.token));
	}
	{
		auto OldContext = SaveAndMove_SymbolContext();
		auto Oldpasstype = _PassType;
		auto Olderrcount = _ErrorsOutput->Get_ErrorCount();

		Set_SymbolContext(classInfo->Context.value());

		size_t NewSymbolIndex = _Table.Symbols.size();

		_PassType = PassType::GetTypes;
		OnEnum(*node);


		auto& addedSymbol = *_Table.Symbols[NewSymbolIndex].get();

		UCodeLangAssert(addedSymbol.FullName == FullName);
		UCodeLangAssert(addedSymbol.Type == SymbolType::Enum);
		UCodeLangAssert(addedSymbol.PassState == PassType::GetTypes);


		if (_ErrorsOutput->Get_ErrorCount() <= Olderrcount)
		{
			_PassType = PassType::FixedTypes;
			OnEnum(*node);

			UCodeLangAssert(addedSymbol.PassState == PassType::FixedTypes);
		}


		Pop_AddToGeneratedGenricSymbol(addedSymbol, GenericInput);
		//
		Set_SymbolContext(std::move(OldContext));
		_PassType = Oldpasstype;
		//
	}
	{
		Pop_ExtendedErr();
	}

	FileDependency_AddDependencyToCurrentFile(Enum);
}

void SystematicAnalysis::Generic_TypeInstantiate_Tag(const NeverNullPtr<Symbol> Tag, const Vector<TypeSymbol>& GenericInput)
{
	UCodeLangAssert(Tag->Type == SymbolType::Generic_Tag);

	const String NewName = Generic_SymbolGenericName(Tag, GenericInput);
	const String FullName = Generic_SymbolGenericFullName(Tag, GenericInput);
	const TagTypeNode* node = TagTypeNode::As(Tag->Get_NodeInfo<Node>());

	const TagInfo* classInfo = Tag->Get_Info<TagInfo>();

	{
		auto& GenericData = classInfo->_GenericData;
		Push_GenericInfo(NewName, GenericInput, Tag, GenericData);
	}
	{
		Push_ExtendedErr(Generic_GetGenericExtendedErrValue(classInfo->_GenericData, node->_generic, GenericInput), NeverNullptr(node->_AttributeName.token));
	}
	{
		auto OldContext = SaveAndMove_SymbolContext();
		auto Oldpasstype = _PassType;
		auto Olderrcount = _ErrorsOutput->Get_ErrorCount();

		Set_SymbolContext(classInfo->Context.value());

		_Table._Scope.ThisScope = ScopeHelper::GetReMoveScope(FullName);

		size_t NewSymbolIndex = _Table.Symbols.size();

		_PassType = PassType::GetTypes;
		OnTag(*node);


		auto& addedSymbol = *_Table.Symbols[NewSymbolIndex].get();

		UCodeLangAssert(addedSymbol.FullName == NewName);
		UCodeLangAssert(addedSymbol.Type == SymbolType::Tag_class);
		UCodeLangAssert(addedSymbol.PassState == PassType::GetTypes);


		if (_ErrorsOutput->Get_ErrorCount() <= Olderrcount)
		{
			_PassType = PassType::FixedTypes;
			OnTag(*node);

			UCodeLangAssert(addedSymbol.PassState == PassType::FixedTypes);
		}


		Pop_AddToGeneratedGenricSymbol(addedSymbol, GenericInput);
		//
		Set_SymbolContext(std::move(OldContext));
		_PassType = Oldpasstype;
		//
	}
	{
		Pop_ExtendedErr();
	}

	FileDependency_AddDependencyToCurrentFile(Tag);
}
void SystematicAnalysis::Generic_TypeInstantiate_ForType(const NeverNullPtr<Symbol> Tag, const Vector<TypeSymbol>& GenericInput)
{
	UCodeLangAssert(Tag->Type == SymbolType::GenericForType);

	const String NewName = Generic_SymbolGenericName(Tag, GenericInput);
	const String FullName = Generic_SymbolGenericFullName(Tag, GenericInput);
	const ForTypeNode* node = ForTypeNode::As(Tag->Get_NodeInfo<Node>());

	const ForTypeInfo* classInfo = Tag->Get_Info<ForTypeInfo>();

	{
		auto& GenericData = classInfo->_GenericData;
		Push_GenericInfo(NewName, GenericInput, Tag, GenericData);
	}
	{
		Push_ExtendedErr(Generic_GetGenericExtendedErrValue(classInfo->_GenericData, node->_generic, GenericInput), NeverNullptr(node->_typetoaddto._name._ScopedName.front()._token));
	}
	{
		auto OldContext = SaveAndMove_SymbolContext();
		auto Oldpasstype = _PassType;
		auto Olderrcount = _ErrorsOutput->Get_ErrorCount();

		Set_SymbolContext(classInfo->Context.value());

		_Table._Scope.ThisScope = ScopeHelper::GetReMoveScope(FullName);

		size_t NewSymbolIndex = _Table.Symbols.size();

		_PassType = PassType::GetTypes;
		OnForTypeNode(*node);


		auto& addedSymbol = *_Table.Symbols[NewSymbolIndex].get();

		UCodeLangAssert(addedSymbol.FullName == NewName);
		UCodeLangAssert(addedSymbol.Type == SymbolType::ForType);
		UCodeLangAssert(addedSymbol.PassState == PassType::GetTypes);


		if (_ErrorsOutput->Get_ErrorCount() <= Olderrcount)
		{
			_PassType = PassType::FixedTypes;
			OnForTypeNode(*node);

			UCodeLangAssert(addedSymbol.PassState == PassType::FixedTypes);
		}


		Pop_AddToGeneratedGenricSymbol(addedSymbol, GenericInput);
		//
		Set_SymbolContext(std::move(OldContext));
		_PassType = Oldpasstype;
		//
	}
	{
		Pop_ExtendedErr();
	}

	FileDependency_AddDependencyToCurrentFile(Tag);
}

GenericData::Type SystematicAnalysis::Generic_TypeToGenericDataType(GenericValueNode::GenericType type)
{
	switch (type)
	{
	case GenericValueNode::GenericType::Name:
		return GenericData::Type::Name;
		break;
	case GenericValueNode::GenericType::ConstantExpression:
		return GenericData::Type::ConstantExpression;
		break;
	case GenericValueNode::GenericType::Pack:
		return GenericData::Type::Pack;
		break;
	default:
		UCodeLangUnreachable();
		break;
	}
}
const Generic* GetGenercInfo(const NeverNullPtr<Symbol> Func)
{
	const Generic* info = nullptr;
	switch (Func->Type)
	{
	case SymbolType::GenericFunc:
		info = &Func->Get_Info<FuncInfo>()->_GenericData;
		break;
	default:
		UCodeLangUnreachable();
		break;
	}
	return info;
}
String SystematicAnalysis::Generic_SymbolGenericFullName(const NeverNullPtr<Symbol> Func, const Vector<TypeSymbol>& Type) const
{
#if UCodeLangDebug
	for (auto& Item : Type)
	{
		UCodeLangAssert(!Type_IsUnMapType(Item));//trying use UnMaped Type when Generic Instantiate.
	}

	bool shouldhaveinputpars = true;
	if (Type.size() == 0)
	{
		const Generic* info = GetGenercInfo(Func);
		shouldhaveinputpars = info->IsPack() == false;
	}
	
	if (shouldhaveinputpars) 
	{
		UCodeLangAssert(Type.size());//you need input types for Generic Instantiate.
	}
#endif // DEBUG


	String NewName = Func->FullName + "<";
	for (auto& Item : Type)
	{
		NewName += ToString(Item);
		if (&Type.back() != &Item)
		{
			NewName += ",";
		}
	}
	NewName += ">";
	return NewName;
}
String SystematicAnalysis::Generic_SymbolGenericName(const NeverNullPtr<Symbol> Func, const Vector<TypeSymbol>& Type) const
{
#if UCodeLangDebug
	for (auto& Item : Type)
	{
		UCodeLangAssert(!Type_IsUnMapType(Item));//trying use UnMaped Type when Generic Instantiate.
	}
	bool shouldhaveinputpars = true;
	if (Type.size() == 0)
	{
		const Generic* info = GetGenercInfo(Func);
		shouldhaveinputpars = info->IsPack() == false;
	}
	
	if (shouldhaveinputpars) 
	{
		UCodeLangAssert(Type.size());//you need input types for Generic Instantiate.
	}
#endif // DEBUG

	String NewName = ScopeHelper::GetNameFromFullName(Func->FullName) + "<";
	for (auto& Item : Type)
	{
		NewName += ToString(Item);
		if (&Type.back() != &Item)
		{
			NewName += ",";
		}
	}
	NewName += ">";
	return NewName;
}
NullablePtr<Symbol> SystematicAnalysis::Generic_InstantiateOrFindGenericSymbol(const NeverNullPtr<Token> Token, const UseGenericsNode& GenericsVals, const String_view& Name)
{
	auto SybVOp = Symbol_GetSymbol(Name, SymbolType::Generic_class);
	if (SybVOp == nullptr)
	{
		LogError_CantFindTypeError(Token, Name);
		return {};
	}

	auto SybV = SybVOp.value();
	if (SybV->Type != SymbolType::Generic_class
		&& SybV->Type != SymbolType::Generic_Alias
		&& SybV->Type != SymbolType::Generic_Enum)
	{
		LogError_ExpectedSymbolToBea(Token, *SybV, SymbolType::Generic_class);
		return {};
	}

	NullablePtr<Symbol> R = nullptr;

	Symbol_Update_Sym_ToFixedTypes(SybV);

	if (SybV->Type == SymbolType::Generic_class)
	{
		auto CInfo = SybV->Get_Info<ClassInfo>();
		auto classnode = ClassNode::As(SybV->Get_NodeInfo<Node>());

		R = Generic_InstantiateOrFindGeneric_Class(Token, SybV, classnode->_generic, CInfo->_GenericData, GenericsVals);
	}
	else if (SybV->Type == SymbolType::Generic_Alias)
	{
		auto CInfo = SybV->Get_Info<Generic_AliasInfo>();
		auto classnode = AliasNode::As(SybV->Get_NodeInfo<Node>());
		R = Generic_InstantiateOrFindGeneric_Alias(Token, SybV, classnode->Generic, CInfo->_GenericData, GenericsVals);
	}
	else if (SybV->Type == SymbolType::Generic_Enum)
	{
		auto CInfo = SybV->Get_Info<EnumInfo>();
		auto classnode = EnumNode::As(SybV->Get_NodeInfo<Node>());
		R = Generic_InstantiateOrFindGeneric_Enum(Token, SybV, classnode->_generic, CInfo->_GenericData, GenericsVals);
	}

	return R;
}
void SystematicAnalysis::Generic_InitGenericalias(const GenericValuesNode& GenericList, bool IsgenericInstantiation, Generic& Out)
{
	for (size_t i = 0; i < GenericList._Values.size(); i++)
	{
		auto& Item = GenericList._Values[i];

		auto GenericTypeName = Item.AsString();
		auto GenericType = &Symbol_AddSymbol(SymbolType::Type_alias, GenericTypeName,
			_Table._Scope.GetApendedString(GenericTypeName), AccessModifierType::Private);

		GenericType->NodePtr = &Item;


		if (IsgenericInstantiation)
		{
			GenericType->Type = SymbolType::Type_alias;

			GenericFuncInfo& V2 = _Generic_GenericSymbolStack.top();

			if (Item._Generictype == GenericValueNode::GenericType::Pack)
			{
				GenericType->VarType = V2.Pack.value();
			}
			else
			{
				GenericType->VarType = (*V2.GenericInput)[i];
			}
		}
		else
		{
			if (Item._Generictype == GenericValueNode::GenericType::Name)
			{
				GenericType->Type = SymbolType::Unmaped_Generic_Type;
			}
			else if (Item._Generictype == GenericValueNode::GenericType::Pack)
			{
				GenericType->Type = SymbolType::Unmaped_Generic_Type;
			}
			else if (Item._Generictype == GenericValueNode::GenericType::ConstantExpression)
			{
				GenericType->Type = SymbolType::Unmaped_Varable;
			}
			else
			{
				UCodeLangUnreachable();
			}

			GenericType->OutputIR = false;

			SymbolID ID = Symbol_GetSymbolID(GenericType->NodePtr);
			_Table.AddSymbolID(*GenericType, ID);

			GenericData Info;
			Info.SybID = ID;
			Info.type = Generic_TypeToGenericDataType(Item._Generictype);

			if (Item._BaseOrRuleScopeName.has_value()) {
				Info._node = Nullableptr(&Item);
			}

			Out._Genericlist.push_back(Info);

			if (Info.type == GenericData::Type::Pack)
			{
				bool IsLast = i == GenericList._Values.size() - 1;
				if (!IsLast)
				{
					LogError_ParPackTypeIsNotLast(NeverNullptr(Item.token));
				}
			}
		}
	}
}


UCodeLangFrontEnd

#endif