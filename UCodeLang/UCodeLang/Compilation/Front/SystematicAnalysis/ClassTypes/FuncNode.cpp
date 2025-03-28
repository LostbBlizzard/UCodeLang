#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"

#include "UCodeLang/Compilation/Helpers/NameDecoratior.hpp"
UCodeLangFrontStart
void SystematicAnalysis::Str_FuncGetName(const NeverNullPtr<Token> NameToken, String_view& FuncName, FuncInfo::FuncType& FuncType)
{
	bool ObjectOverLoad = false;
	switch (NameToken->Type)
	{
	case TokenType::KeyWord_new:
		FuncName = ClassConstructorfunc;
		FuncType = FuncInfo::FuncType::New;
		ObjectOverLoad = true;
		break;
	case TokenType::KeyWord_Drop:
		FuncName = ClassDestructorFunc;
		FuncType = FuncInfo::FuncType::Drop;
		ObjectOverLoad = true;
		break;
	case TokenType::Left_Bracket:
		FuncName = Overload_Index_Func;
		FuncType = FuncInfo::FuncType::Index;
		ObjectOverLoad = true;
		break;
	case TokenType::Left_Parentheses:
		FuncName = Overload_Invoke_Func;
		FuncType = FuncInfo::FuncType::Invoke;
		ObjectOverLoad = true;
		break;
	case TokenType::KeyWord_for:
		FuncName = Overload_For_Func;
		FuncType = FuncInfo::FuncType::For;
		ObjectOverLoad = true;
		break;
	case TokenType::RightArrow:
		FuncName = Overload_Cast_Func;
		FuncType = FuncInfo::FuncType::Cast;
		ObjectOverLoad = true;
		break;
	case TokenType::Name:
		break;
	default:


		for (auto& Item : Systematic_BinaryOverloadData::data)
		{
			if (NameToken->Type == Item.token)
			{
				FuncName = Item.CompilerName;
				FuncType = Item.Type;
				ObjectOverLoad = true;
				goto DoStuff;
			}
		}

		for (auto& Item : Systematic_PostfixOverloadData::data)
		{
			if (NameToken->Type == Item.token)
			{
				FuncName = Item.CompilerName;
				FuncType = Item.Type;
				ObjectOverLoad = true;
				goto DoStuff;
			}
		}

		for (auto& Item : Systematic_UrinaryOverloadData::data)
		{
			if (NameToken->Type == Item.token)
			{
				FuncName = Item.CompilerName;
				FuncType = Item.Type;
				ObjectOverLoad = true;
				goto DoStuff;
			}
		}

		for (auto& Item : Systematic_CompoundOverloadData::data)
		{
			if (NameToken->Type == Item.token)
			{
				FuncName = Item.CompilerName;
				FuncType = Item.Type;
				ObjectOverLoad = true;
				goto DoStuff;
			}
		}

		for (auto& Item : Systematic_MemberOverloadData::data)
		{
			if (NameToken->Type == Item.token)
			{
				FuncName = Item.CompilerName;
				FuncType = Item.Type;
				ObjectOverLoad = true;
				goto DoStuff;
			}
		}

		LogError_CantOverLoadOverload(NameToken);

		break;
	}

DoStuff:
	if (ObjectOverLoad)
	{
		if (_ClassStack.empty())
		{
			LogError_CantUseThisKeyWordHereError(NameToken);
		}
	}
}

void SystematicAnalysis::FuncRetCheck(const Token& Name, const NeverNullPtr<Symbol> FuncSyb, const FuncInfo* Func)
{
	if (Func->IsRemoved == true) { return; }
	switch (Func->_FuncType)
	{
	case FuncInfo::FuncType::Drop:
	{
		TypeSymbol V(TypesEnum::Void);
		if (!Type_AreTheSame(Func->Ret, V))
		{
			LogError_LogFuncMustBe(NeverNullptr(&Name), FuncSyb->FullName, V);
		}
		if (Func->Pars.size() != 1)
		{
			LogError_PostfixOverloadPars(Name, Func);
		}
		break;
	}
	case FuncInfo::FuncType::New:
	{

		TypeSymbol V(TypesEnum::Void);
		if (!Type_AreTheSame(Func->Ret, V))
		{
			LogError_LogFuncMustBe(NeverNullptr(&Name), FuncSyb->FullName, V);
		}

	}
	break;
	case FuncInfo::FuncType::Index:
	{
		if (Func->Pars.size() != 2)
		{
			LogError_IndexOverloadPars(Name, Func);
		}
		else if (Func->Pars.size() == 2 && (Func->Pars[0].IsOutPar || Func->Pars[1].IsOutPar))
		{
			LogError_CantUseOutInOverloadFunc(Name);
		}
	}
	break;
	case FuncInfo::FuncType::Cast:
	{
		if (Func->Pars.size() != 1)
		{


			LogError_PostfixOverloadPars(Name, Func);

		}
		else if (Func->Pars.size() == 1 && Func->Pars[0].IsOutPar)
		{
			LogError_CantUseOutInOverloadFunc(Name);
		}
	}
	break;
	case FuncInfo::FuncType::NameFunc:
		break;
	default:
		if (Systematic_BinaryOverloadData::IsBinaryOverload(Func->_FuncType))
		{
			if (Func->Pars.size() != 2)
			{
				LogError_BinaryOverloadPars(Name, Func);
			}
			else if (Func->Pars.size() == 2 && (Func->Pars[0].IsOutPar
				|| Func->Pars[1].IsOutPar))
			{
				LogError_CantUseOutInOverloadFunc(Name);
			}
		}
		else if (Systematic_PostfixOverloadData::IsPostfixOverload(Func->_FuncType))
		{
			if (Func->Pars.size() != 1)
			{
				LogError_PostfixOverloadPars(Name, Func);
			}
			else if (Func->Pars.size() == 1 && Func->Pars[0].IsOutPar)
			{
				LogError_CantUseOutInOverloadFunc(Name);
			}
		}
		else if (Systematic_CompoundOverloadData::IsCompoundOverload(Func->_FuncType))
		{
			if (Func->Pars.size() != 2)
			{
				LogError_BinaryOverloadPars(Name, Func);

			}
			else if (Func->Pars.size() == 2 &&
				(Func->Pars[0].IsOutPar || Func->Pars[1].IsOutPar))
			{
				LogError_CantUseOutInOverloadFunc(Name);
			}

		}
		else if (Systematic_UrinaryOverloadData::IsUrinaryOverload(Func->_FuncType))
		{
			if (Func->Pars.size() != 1)
			{
				LogError_PostfixOverloadPars(Name, Func);
			}
			else if (Func->Pars.size() == 1 && Func->Pars[0].IsOutPar)
			{
				LogError_CantUseOutInOverloadFunc(Name);
			}
		}
		else if (Systematic_MemberOverloadData::IsMemerOverload(Func->_FuncType))
		{
			if (Func->Pars.size() != 1)
			{
				LogError_PostfixOverloadPars(Name, Func);
			}
			else if (Func->Pars.size() == 1 && Func->Pars[0].IsOutPar)
			{
				LogError_CantUseOutInOverloadFunc(Name);
			}
		}
		break;
	}
}



#define FunctionOverloadStartSymbol "{<!"
#define FunctionOverloadEndSymbol "!>}"

constexpr size_t FunctionOverloadStartSymbolSize = sizeof(FunctionOverloadStartSymbol)-1;
constexpr size_t FunctionOverloadEndSymbolSize = sizeof(FunctionOverloadEndSymbol)-1;


String SystematicAnalysis::RemoveSymboolFuncOverloadMangling(const String_view fullname)
{
	bool IsAdding = true;

	String r;
	for (size_t i = 0; i < fullname.size(); i++)
	{
		char v = fullname[i];
		if (IsAdding)
		{
			bool isstartofFuncoverload = false;
			for (size_t i2 = 0; i2 < std::min(FunctionOverloadStartSymbolSize, fullname.size() - i); i2++)
			{
				char OverloadChar = FunctionOverloadStartSymbol[i2];
				if (fullname[i + i2] !=OverloadChar)
				{
					break;
				}

				if (i2 == FunctionOverloadStartSymbolSize-1)
				{
					isstartofFuncoverload = true;
				}
			}
			if (isstartofFuncoverload)
			{
				IsAdding = false;
				i += FunctionOverloadStartSymbolSize - 1;
			}

			if (IsAdding)
			{
				r += v;
			}
		}
		else
		{
			bool isstartofendFuncoverload = false;
			for (size_t i2 = 0; i2 < std::min(FunctionOverloadEndSymbolSize, fullname.size() - i); i2++)
			{
				char OverloadChar = FunctionOverloadEndSymbol[i2];
				if (fullname[i + i2] != OverloadChar)
				{
					break;
				}

				if (i2 == FunctionOverloadEndSymbolSize - 1)
				{
					isstartofendFuncoverload = true;
				}
			}

			if (isstartofendFuncoverload)
			{
				IsAdding = true;
				i += FunctionOverloadEndSymbolSize - 1;
			}
		}
	}

	return r;
}

void SystematicAnalysis::OnFuncNode(const FuncNode& node)
{

	const bool IsgenericInstantiation = _Generic_GenericSymbolStack.size() && _Generic_GenericSymbolStack.top().NodeTarget == &node;
	const bool IsGenericS = node._Signature._generic._Values.size();
	const bool Isgeneric_t = IsGenericS && IsgenericInstantiation == false;
	const bool CheckgenericForErr = (Isgeneric_t && (_PassType == PassType::GetTypes || _PassType == PassType::FixedTypes));

	auto FuncName = IsgenericInstantiation ? _Generic_GenericSymbolStack.top()._IR_GenericFuncName
		: node._Signature._Name.AsStringView();
	auto NameToken = NeverNullptr(node._Signature._Name.token);


	FuncInfo::FuncType FuncType = FuncInfo::FuncType::NameFunc;



	Str_FuncGetName(NameToken, FuncName, FuncType);
	if (IsgenericInstantiation)
	{
		FuncName = _Generic_GenericSymbolStack.top()._IR_GenericFuncName;
	}


	String t;
	if (node._Signature._Parameters._Parameters.size())
	{
		t = FunctionOverloadStartSymbol;
	}
	for (auto& item : node._Signature._Parameters._Parameters)
	{
		if (item._Type._IsAddess) { t += "&"; }
		if (item._Type._Isimmutable) { t += "imut"; }
		if (item._Type._IsTypedMoved) { t += "mov"; }
		if (item._Type._IsDynamic) { t += "dyn"; }
		t += item._Type.AsString();
		if (&item != &node._Signature._Parameters._Parameters.back())
		{
			t += ",";
		}
	}
	if (node._Signature._Parameters._Parameters.size())
	{
		t += FunctionOverloadEndSymbol;
	}
	t += FuncName;

	_Table.AddScope(FuncName);
	auto FullName = _Table._Scope.ThisScope;
	_Table.RemoveScope();

	_Table.AddScope(t);



	Symbol* syb;
	SymbolID sybId = Symbol_GetSymbolID(node);//Must be pass AddScope thats how GetSymbolID works.


	auto UseingIndex = _Table.GetUseingIndex();

	if (_PassType == PassType::GetTypes)
	{
		SymbolType Type = IsGenericS && IsgenericInstantiation == false ?
			SymbolType::GenericFunc : SymbolType::Func;



		syb = &Symbol_AddSymbol(Type, (String)FuncName, FullName, node._Signature._Access);
		syb->NodePtr = node.As();
		_Table.AddSymbolID(*syb, sybId);

		FuncInfo* newInfo = new FuncInfo();
		newInfo->Context = Opt(Save_SymbolContextRemoveOneScopeName());
		newInfo->FullName = _Table._Scope.ThisScope;
		newInfo->_FuncType = FuncType;
		newInfo->IsUnsafe = node._Signature._HasUnsafeKeyWord;
		newInfo->IsExternC = node._Signature.Externtype == ExternType::ExternC
			|| node._Signature.Externtype == ExternType::ExternSystem;
		newInfo->IsRemoved = node._Signature._IsRemoved;
		newInfo->IsTraitDynamicDispatch = node._Signature._HasDynamicKeyWordForTrait;

		syb->Info.reset(newInfo);

		auto& GenericList = node._Signature._generic;

		Generic_InitGenericalias(GenericList, IsgenericInstantiation, newInfo->_GenericData);


		{
			auto& RetType = node._Signature._ReturnType._node;
			if (RetType && RetType->Get_Type() == NodeType::AnonymousTypeNode)
			{
				auto NewName = Str_GetFuncAnonymousObjectFullName(FullName);


				SymbolID AnonymousSybID = Symbol_GetSymbolID(RetType.get());
				auto& AnonymousSyb = Symbol_AddSymbol(SymbolType::Type_class, (String)NewName, NewName, node._Signature._Access);

				_Table.AddSymbolID(AnonymousSyb, AnonymousSybID);


				auto ClassInf = new ClassInfo();
				ClassInf->FullName = NewName;
				AnonymousSyb.Info.reset(ClassInf);
				AnonymousSyb.VarType.SetType(AnonymousSyb.ID);

				AnonymousTypeNode* Typenode = AnonymousTypeNode::As(RetType.get());
				for (auto& Item3 : Typenode->_Fields._Parameters)
				{
					ClassInf->AddField(Item3._Name.AsString(), Type_ConvertAndValidateType(Item3._Type, NodeSyb_t::Parameter));
				}
			}
		}



		auto ClassSymBool = _ClassStack.size() ? _ClassStack.top().Syb : nullptr;
		for (auto& Item : node._Signature._Parameters._Parameters)
		{

			String_view GenericTypeName;
			if (Item._Name.token == nullptr)
			{
				GenericTypeName = ThisSymbolName;
			}
			else
			{
				if (Item._Name.token != nullptr)
				{
					GenericTypeName = Item._Name.AsStringView();
				}
			}




			auto GenericType = &Symbol_AddSymbol(SymbolType::ParameterVarable, (String)GenericTypeName,
				_Table._Scope.GetApendedString(GenericTypeName)
				, node._Signature._Access);
			auto ParSybID = Symbol_GetSymbolID(Item);

			auto ParInfo = new ParameterInfo();
			ParInfo->IsOutValue = Item._IsOutVarable;
			ParInfo->MyFunc = newInfo;

			GenericType->Info.reset(ParInfo);
			_Table.AddSymbolID(*GenericType, ParSybID);

			if (Item._Name.token == nullptr)
			{
				newInfo->FrontParIsUnNamed = true;
			}
			newInfo->Pars.push_back({ Item._IsOutVarable,Type_ConvertAndValidateType(Item._Type,NodeSyb_t::Parameter) });
		}

	}
	else
	{
		syb = Symbol_GetSymbol(sybId).value();
	}

	//we may jump to this node non linearly
	if (syb->PassState == _PassType)
	{
		_Table.RemoveScope();
		return;
	}

	FuncInfo* Info = syb->Get_Info<FuncInfo>();
	OnAttributesNode(node._Attributes, Optionalref(Info->Attributes));

	if (_PassType == PassType::FixedTypes && Isgeneric_t)
	{
		auto& GenericList = node._Signature._generic;
		Generic_GenericAliasFixTypes(GenericList, IsgenericInstantiation, Info->_GenericData);
	}
	syb->PassState = _PassType;
	_FuncStack.push_back(Info);

	Optional<bool> IsEnabled;
	for (auto& Item : Info->Attributes)
	{
		if (!Item->VarType.IsBadType())
		{
			auto& sym = *Symbol_GetSymbol(Item->VarType).value().value();
			if (IsEnableAttribute(sym))
			{
				IsEnabled = GetEnableAttribute(*Item).IsEnable;
				break;
			}
		}
	}

	if (_PassType == PassType::FixedTypes
		|| (IsGenericS && _PassType == PassType::GetTypes))
	{
		auto& RetType = node._Signature._ReturnType._node;
		if (RetType && RetType->Get_Type() == NodeType::AnonymousTypeNode)
		{
			SymbolID AnonymousSybID = Symbol_GetSymbolID(RetType.get());
			auto& V = *Symbol_GetSymbol(AnonymousSybID);

			auto ClassInf = (ClassInfo*)V.Get_Info<ClassInfo>();

			AnonymousTypeNode* Typenode = AnonymousTypeNode::As(RetType.get());

			for (size_t i = 0; i < Typenode->_Fields._Parameters.size(); i++)
			{
				auto& Item3 = Typenode->_Fields._Parameters[i];
				auto ItemOut = ClassInf->Fields[i];
				Type_ConvertAndValidateType(Item3._Type, ItemOut.Type, NodeSyb_t::Parameter);
			}


			Push_ClassDependencie(ClassInf);

			size_t ClassSize = 0;
			for (auto& Item : ClassInf->Fields)
			{
				size_t V = NullAddress;
				Type_GetSize(Item.Type, V);
				ClassSize += V;
			}

			Pop_ClassDependencie();

			ClassInf->Size = ClassSize;
			ClassInf->SizeInitialized = true;


			syb->VarType.SetType(AnonymousSybID);
			Info->Ret = syb->VarType;
		}
		else
		{
			if (!node._Signature._IsRemoved)
			{
				Type_ConvertAndValidateType(node._Signature._ReturnType, Info->Ret, NodeSyb_t::Ret);
				syb->VarType = Info->Ret;
			}
		}



		for (size_t i = 0; i < node._Signature._Parameters._Parameters.size(); i++)
		{
			auto& Item = node._Signature._Parameters._Parameters[i];
			auto& Item2 = Info->Pars[i];


			auto ParSybID = Symbol_GetSymbolID(Item);
			auto& Symbol = *Symbol_GetSymbol(ParSybID);
			Type_ConvertAndValidateType(Item._Type, Symbol.VarType, NodeSyb_t::Parameter);
			Item2.Type = Symbol.VarType;



			{//Par Pack Err
				if (Item2.Type._CustomTypeSymbol.HasValue() && (&Item2 != &Info->Pars.back()))
				{
					for (auto& GenericItem : Info->_GenericData._Genericlist)
					{
						if (GenericItem.SybID == Item2.Type._CustomTypeSymbol)
						{
							if (GenericItem.type == GenericData::Type::Pack)
							{
								auto Token = NeverNullptr(Item._Name.token);
								LogError_LogParPackIsNotLast(Token);
							}

							break;
						}
					}
				}
			}

			if (Item2.Type.IsAn(TypesEnum::Void))
			{
				auto Token = NeverNullptr(Item._Name.token);
				LogError_CantUseTypeVoidHere(Token);
			}
			if (Item2.Type.IsTypeInfo())
			{
				auto Token = NeverNullptr(Item._Name.token);
				LogError_UseingTypeinfoInEvalFuncPar(Token);
			}
		}

		if (FuncType == FuncInfo::FuncType::New)
		{
			if (Info->Pars.size() == 2)
			{
				auto& Classinfo = _ClassStack.top().Info;
				auto classsybid = _ClassStack.top().Syb->ID;
				auto& OtherPar = Info->Pars[1];
				if (OtherPar.IsOutPar == false && OtherPar.Type._CustomTypeSymbol == classsybid)
				{
					if (OtherPar.Type.IsMovedType())
					{
						//Move Contructer
						Classinfo->_ClassHasMoveConstructor = Opt<SymbolID>(sybId);
					}
					else
					{
						if (!OtherPar.Type.Isimmutable() && OtherPar.Type.IsAddress())
						{
							auto ParToken = node._Signature._Parameters._Parameters[1]._Name.token;
							LogError(ErrorCodes::InValidType,
								(String)"Copy Constructor '" + (String)ParToken->Value._String + (String)"' should be imut.", NeverNullptr(ParToken));
						}
						//Copy Contructer
						Classinfo->_ClassHasCopyConstructor = Opt<SymbolID>(sybId);
					}
				}
			}
		}

	}




	bool buidCode = _PassType == PassType::BuidCode && Info->IsRemoved == false;
	bool ignoreBody = (!IsgenericInstantiation && IsGenericS);
	if (ignoreBody == true)
	{
		if (IsgenericInstantiation == false && IsGenericS && _PassType != PassType::BuidCode) 
		{
			ignoreBody = false;
		}
	}
	bool igorebecausedisable = false;
	if (ignoreBody == false)
	{
		if (IsEnabled.has_value())
		{
			if (IsEnabled.value() == false) 
			{
				igorebecausedisable = true;
			}
		}
	}
	if (!IsgenericInstantiation && IsGenericS && node._Signature._IsExport) 
	{
		FuncGenericIndirectInfo2 indirectinfo;
		indirectinfo.Indirect = syb;
		IndirectGenericFunc.push_back(indirectinfo);
	}


	IRFunc* OldIRFunc = nullptr;
	IRBlock* OldIRBlock = nullptr;

	if (buidCode && !ignoreBody)
	{
		bool IsBuildingIR = igorebecausedisable == false;
		auto DecName = IR_MangleName(Info);

		if (IsBuildingIR)
		{
			OldIRFunc = _IR_LookingAtIRFunc;
			OldIRBlock = _IR_LookingAtIRBlock;
		
			_IR_LookingAtIRFunc = _IR_Builder.NewFunc(IR_GetIRID(Info), {});
			_IR_LookingAtIRBlock = _IR_LookingAtIRFunc->NewBlock("");
			Push_NewStackFrame();



			auto& ParNodes = node._Signature._Parameters._Parameters;

			bool IsPackParLast = false;
			if (ParNodes.size())
			{
				auto& LastPar = ParNodes[ParNodes.size() - 1];
			
				auto ParSybID = Symbol_GetSymbolID(LastPar);
				auto& V = *Symbol_GetSymbol(ParSybID);
				

				auto symop = Symbol_GetSymbol(V.VarType);
				if (symop.has_value())
				{
					auto sym = symop.value();

					if (sym->Type == SymbolType::Type_Pack)
					{
						IsPackParLast = true;
					}
				}

			}

			size_t ParNodeSize = ParNodes.size();
			if (IsPackParLast)
			{
				_IR_LookingAtIRFunc->Pars.resize(GetParCountResolveTypePack(Info->Pars));

				ParNodeSize -= 1;
			}
			else
			{
				_IR_LookingAtIRFunc->Pars.resize(ParNodes.size());//becuase we are using ptrs.
			}

			for (size_t i = 0; i < ParNodeSize; i++)
			{
				auto& Item = ParNodes[i];

				auto ParSybID = Symbol_GetSymbolID(Item);
				auto& V = *Symbol_GetSymbol(ParSybID);


				auto& VarType = Info->Pars[i];

				auto& d = _IR_LookingAtIRFunc->Pars[i];
				d.identifier = _IR_Builder.ToID(ScopeHelper::GetNameFromFullName(V.FullName));

				if (Item._IsOutVarable)
				{
					d.type = IRType(IRTypes::pointer);
				}
				else
				{
					d.type = IR_ConvertToIRType(VarType);
				}


				Debug_Add_SetVarableInfo(V, _IR_LookingAtIRBlock->Instructions.size() == 0 ? 0 : _IR_LookingAtIRBlock->GetIndex());

				if (Symbol_HasDestructor(VarType.Type))
				{
					ObjectToDrop V;
					V.DropType = ObjectToDropType::Operator;
					V.ID = ParSybID;
					V._Operator = IROperator(&d);
					V.Type = VarType.Type;

					_IR_StackFrames.back().OnEndStackFrame.push_back(V);
				}

				V.IR_Par = &d;
			}

			if (IsPackParLast)
			{
				const TypePackInfo* PackPar = Symbol_GetSymbol(Info->Pars.back().Type).value()->Get_Info<TypePackInfo>();

				size_t V = ParNodeSize;


				auto PackParSymbol = Symbol_GetSymbol(Symbol_GetSymbolID(ParNodes.back()));
				auto PackParSymbolName = ScopeHelper::GetNameFromFullName(PackParSymbol->FullName);

				for (size_t i = 0; i < PackPar->List.size(); i++)
				{
					auto& PackType = PackPar->List[i];
					auto& d = _IR_LookingAtIRFunc->Pars[V];

					auto ParName = PackParSymbolName + std::to_string(i);



					auto& TepPar = Symbol_AddSymbol(SymbolType::ParameterVarable, ParName, PackParSymbol->FullName + std::to_string(i), AccessModifierType::Public);
					auto ParSybID = Symbol_GetSymbolID(&TepPar);
					TepPar.IR_Par = &d;

					_Table.AddSymbolID(TepPar, ParSybID);


					d.identifier = _IR_Builder.ToID(ParName);
					d.type = IR_ConvertToIRType(PackType);

					Debug_Add_SetVarableInfo(TepPar, _IR_LookingAtIRBlock->Instructions.size() == 0 ? 0 : _IR_LookingAtIRBlock->GetIndex());

					if (Symbol_HasDestructor(PackType))
					{
						ObjectToDrop V;
						V.DropType = ObjectToDropType::Operator;
						V.ID = ParSybID;
						V._Operator = IROperator(_IR_LookingAtIRBlock->NewLoadPtr(&d));
						V.Type = PackType;

						_IR_StackFrames.back().OnEndStackFrame.push_back(V);
					}


					//

					V++;
				}
			}


			_IR_LookingAtIRFunc->ReturnType = IR_ConvertToIRType(Info->Ret);
		}


		if (node._Signature.Externtype != ExternType::NoExternKeyWord)
		{
			bool HasBody = node._Body.has_value();
			if (HasBody)
			{
				if (node._Signature._HasDynamicKeyWord)
				{
					_IR_LookingAtIRFunc->Linkage = IRFuncLink::DynamicExport;
				}
				else
				{
					_IR_LookingAtIRFunc->Linkage = IRFuncLink::StaticExport;
				}
			}
			else
			{
				if (node._Signature._HasDynamicKeyWord)
				{
					_IR_LookingAtIRFunc->Linkage = IRFuncLink::DynamicExternalLink;
				}
				else
				{
					_IR_LookingAtIRFunc->Linkage = IRFuncLink::StaticExternalLink;
				}
			}

		}


		Class_Data* Ptr = Assembly_GetAssemblyClass(RemoveSymboolFuncOverloadMangling(FullName));

		ClassMethod V;
		V.FullName = RemoveSymboolFuncOverloadMangling(FullName);
		V.DecorationName = RemoveSymboolFuncOverloadMangling(DecName);
		V.RetType = Assembly_ConvertToType(Info->Ret);
	

		V.IsThisFunction = Info->FrontParIsUnNamed;
		V.IsExternC = Info->IsExternC;
		V.IsUnsafe = Info->IsUnsafe;
		V.IsRemoved = Info->IsRemoved;
		V.Protection = syb->Access;


		bool shouldExport = node._Signature._IsExport;
		if (shouldExport && IsgenericInstantiation)
		{
			if (!Type_IsTypeExported(Info->Ret))
			{
				shouldExport = false;
			}
			else
			{
				for (auto& Item : Info->Pars)
				{
					if (!Type_IsTypeExported(Item.Type))
					{
						shouldExport = false;
						break;
					}
				}
			}
		}
		V.IsExport = shouldExport ? ExportType::Exported : ExportType::NotExported;

		if (V.IsExport == ExportType::NotExported && Info->_IsIndirectExport) 
		{
			V.IsExport = ExportType::IndrectExported;
		}

		for (size_t i = 0; i < Info->Pars.size(); i++)
		{
			auto& Item = Info->Pars[i];

					
			auto symop = Symbol_GetSymbol(Item.Type);
			if (symop)
			{
				auto sym = symop.value();
				if (sym->Type == SymbolType::Type_Pack)
				{
					auto info = sym->Get_Info<TypePackInfo>();

					for (auto& Item : info->List)
					{
						auto& F = V.ParsType.emplace_back();
						F.IsOutPar = false;
						F.Type = Assembly_ConvertToType(Item);
					}
					continue;
				}
			}
			auto& F = V.ParsType.emplace_back();
			F.IsOutPar = node._Signature._Parameters._Parameters[i]._IsOutVarable;
			F.Type = Assembly_ConvertToType(Item.Type);


		}

		Assembly_ConvertAttributes(node._Attributes, V.Attributes.Attributes);


		Ptr->Methods.push_back(std::move(V));

		

		auto& RetType = node._Signature._ReturnType._node;
		if (RetType && RetType->Get_Type() == NodeType::AnonymousTypeNode)
		{
			SymbolID AnonymousSybID = Symbol_GetSymbolID(RetType.get());
			auto V = Symbol_GetSymbol(AnonymousSybID);

			Assembly_AddClass({}, V);

		}
		//

		if (IsBuildingIR)
		{
			if (FuncType == FuncInfo::FuncType::New)
			{
				auto& Classinfo = _ClassStack.top().Info;
				if (Classinfo->_WillHaveFieldInit)
				{
					auto InitFunc = Classinfo->FullName;
					ScopeHelper::GetApendedString(InitFunc, ClassInitializefuncName);


					_IR_LookingAtIRBlock->NewPushParameter(_IR_LookingAtIRBlock->NewLoad(&_IR_LookingAtIRFunc->Pars.front()));
					_IR_LookingAtIRBlock->NewCall(_IR_Builder.ToID(InitFunc));
				}
			}
		}
		ignoreBody = igorebecausedisable;
		//
	}

	

	if (CheckgenericForErr)
	{
		ignoreBody = false;
		_Table.AddScope(GenericTestStr);
	}

	if (node._Body.has_value() && !ignoreBody)
	{
		auto& Body = node._Body.value();
		size_t ErrorCount = _ErrorsOutput->Get_Errors().size();

		OnStatementsWithSetableRet(Body._Statements, Info->Ret, NeverNullptr(node._Signature._Name.token));

		bool GotErr = ErrorCount < _ErrorsOutput->Get_Errors().size();
		if (GotErr)
		{
			syb->SetToInvalid();
		}
		syb->VarType = Info->Ret;
	}

	if (CheckgenericForErr)
	{
		_Table.RemoveScope();
	}

	if (buidCode && !ignoreBody)
	{
		bool hasins = _IR_LookingAtIRBlock->Instructions.size();
		auto lastbefordrop = hasins ? _IR_LookingAtIRBlock->Instructions.back().get() : nullptr;

		Pop_StackFrame();
		size_t droploc = hasins ? _IR_LookingAtIRBlock->Instructions.size() - 1 : 0;

		if (FuncType == FuncInfo::FuncType::Drop)
		{

			auto& Classinfo = _ClassStack.top().Info;
			if (Classinfo->_WillHaveFielddeInit)
			{
				auto InitFunc = Classinfo->FullName;
				ScopeHelper::GetApendedString(InitFunc, ClassInitializefuncName);

				_IR_LookingAtIRBlock->NewPushParameter(_IR_LookingAtIRBlock->NewLoad(&_IR_LookingAtIRFunc->Pars.front()));
				_IR_LookingAtIRBlock->NewCall(_IR_Builder.ToID(InitFunc));
			}

		}

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




		if (FuncName == "main" && _IR_LookingAtIRFunc->Pars.size() == 0)
		{
			_IR_Builder.EntryPoint = Opt<IRidentifierID>(_IR_LookingAtIRFunc->identifier);
		}

		_IR_LookingAtIRBlock = OldIRBlock;
		_IR_LookingAtIRFunc = OldIRFunc;
	}

	if (syb->Type == SymbolType::GenericFunc && _PassType == PassType::BuidCode)
	{
		const FuncNode* body = &node;

		auto nametoken = body->_Signature._Name.token;
		auto endtoken = body->EndOfFunc;
		String_view filetext = syb->_File->FileText;

		auto& assemblyfunc = _Lib.Get_Assembly().AddGenericFunc(ScopeHelper::GetNameFromFullName(syb->FullName), syb->FullName);

		assemblyfunc.Base.Implementation = GetImplementationFromFunc(filetext, nametoken, endtoken);
		assemblyfunc.AccessModifier = syb->Access;
		assemblyfunc.IsExported = node._Signature._IsExport ? ExportType::Exported : ExportType::NotExported;
		if (assemblyfunc.IsExported == ExportType::NotExported && Info->_IsIndirectExport) 
		{
			assemblyfunc.IsExported = ExportType::IndrectExported;
		}
		assemblyfunc.UseStatments = Generic_GetCurrentUseStatements();
	}

	if (_PassType == PassType::FixedTypes)
	{
		if (!node._Body.has_value() && syb->VarType._Type == TypesEnum::Var)
		{
			auto Token = NeverNullptr(node._Signature._Name.token);
			LogError_BeMoreSpecifiicForRetType(FuncName, Token);
		}
		else
		{
			FuncRetCheck(*node._Signature._Name.token, syb, Info);
			auto Token = NeverNullptr(node._Signature._Name.token);
			Symbol_RedefinitionCheck(syb, Info, Token);
		}


		bool ispublic = node._Signature._Access == AccessModifierType::Public;
		if (node._Signature._IsExport && ispublic)
		{
			const Vector<TypeSymbol>* TypesToIgnore = nullptr;

			if (IsgenericInstantiation)
			{
				TypesToIgnore = _Generic_GenericSymbolStack.top().GenericInput;
			}

			if (!Type_IsTypeExported(Info->Ret))
			{
				bool isinlist =false;

				if (TypesToIgnore)
				{
					for (auto& Item : *TypesToIgnore)
					{
						if (Type_AreTheSame(Info->Ret,Item))
						{
							isinlist =true;
							break;
						}
					}
				}

				if (isinlist ==false)
				{
					LogError_TypeIsNotExport(NeverNullptr(node._Signature._ReturnType._name._ScopedName.back()._token), Info->Ret, syb);
				}
			}

			for (auto& Item : Info->Pars)
			{
				if (!Type_IsTypeExported(Item.Type))
				{
					bool isinlist =false;

					if (TypesToIgnore)
					{
						for (auto& TypeItem : *TypesToIgnore)
						{
							if (Type_AreTheSame(Item.Type,TypeItem))
							{
								isinlist =true;
								break;
							}
						}
					}

					if (isinlist == false) 
					{
						LogError_TypeIsNotExport(NeverNullptr(node._Signature._Name.token), Item.Type, syb);
					}
				}
			}
		}

		if (!ispublic && node._Signature._IsExport)
		{
			LogError_ExportIsPrivate(NeverNullptr(node._Signature._Name.token), syb);
		}
	}

	_FuncStack.pop_back();

	_Table.RemovePopUseing(UseingIndex);




	_Table.RemoveScope();
}

String SystematicAnalysis::GetImplementationFromFunc(String_view filetext, const Token* nametoken, const Token* endtoken)
{

	String funcstr = (String)"|";

	if (nametoken->Type == TokenType::Name)
	{
		funcstr += nametoken->Value._String;
	}
	else if (nametoken->Type == TokenType::KeyWord_for)
	{
		funcstr += "for";
	}
	else if (nametoken->Type == TokenType::KeyWord_new)
	{
		funcstr += "new";
	}
	else if (nametoken->Type == TokenType::Left_Bracket)
	{
		funcstr += "";
	}
	else
	{
		UCodeLangUnreachable();
	}


	size_t offset = 0;
	if (endtoken->Type == TokenType::Semicolon)
	{
		offset++;
	}


	funcstr += (String)filetext.substr(nametoken->OnPos,
		endtoken->OnPos - nametoken->OnPos + offset);

	funcstr += "\n ";


	return funcstr;
}

String SystematicAnalysis::IR_MangleName(const FuncInfo* Func)
{
	if (Func->IsExternC)
	{
		return RemoveSymboolFuncOverloadMangling(Func->FullName);
	}
	else
	{
		Vector<NameDecoratior::Par> Vect;
		for (auto& Item : Func->Pars)
		{
			NameDecoratior::Par V;
			V.par.Type = Assembly_ConvertToType(Item.Type);
			V.par.IsOutPar = Item.IsOutPar;

			if (auto Val = Symbol_GetSymbol(Item.Type).value_unchecked())
			{
				if (Val->Type == SymbolType::Type_Pack)
				{
					auto info = Val->Get_Info<TypePackInfo>();

					for (auto& ItemList : info->List) 
					{
						NameDecoratior::Par V;
						V.par.Type = Assembly_ConvertToType(ItemList);
						V.par.IsOutPar = Item.IsOutPar;
						Vect.push_back(std::move(V));
					}
					continue;
				}
				else 
				{
					V.symbolFullName = Val->FullName;
				}
			}
			Vect.push_back(std::move(V));
		}

		return RemoveSymboolFuncOverloadMangling(NameDecoratior::GetDecoratedName(Func->FullName, Vect));
	}
}

IRidentifierID SystematicAnalysis::IR_GetIRID(const FuncInfo* Func)
{
	auto FuncName = IR_MangleName(Func);
	return _IR_Builder.ToID(FuncName);
}

bool SystematicAnalysis::IsExported(SymbolID type) 
{

	auto syb = Symbol_GetSymbol(type);

	switch (syb->Type) 
	{
		case SymbolType::Type_class:
		{
			if (syb->NodePtr)
			{
				return syb->Get_NodeInfo<ClassNode>()->_IsExport;
			}
			else
			{
				return true;
			}
		}
		break;
		case SymbolType::Enum:
		{
			if (syb->NodePtr)
			{
				return syb->Get_NodeInfo<EnumNode>()->_IsExport;
			}
			else
			{
				return true;
			}
		}
		break;
		case SymbolType::Type_alias:
		case SymbolType::Func_ptr:
		{
			if (syb->NodePtr)
			{
				return syb->Get_NodeInfo<AliasNode>()->IsExport;
			}
			else
			{
				return true;
			}
		}
		break;
	
		case SymbolType::Trait_class:
		{
			if (syb->NodePtr)
			{
				return syb->Get_NodeInfo<TraitNode>()->_IsExport;
			}
			else
			{
				return true;
			}
		}
		break;
		case SymbolType::Func:
		{
			if (syb->NodePtr)
			{
				return syb->Get_NodeInfo<FuncNode>()->_Signature._IsExport;
			}
			else
			{
				return true;
			}
		}
		break;
		default:
			return  false;
	}
	return false;
}
UCodeLangFrontEnd

#endif
