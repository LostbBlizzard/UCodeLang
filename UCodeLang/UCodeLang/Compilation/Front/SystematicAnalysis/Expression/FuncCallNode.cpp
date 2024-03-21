#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::IR_Build_FuncCall(const TypeSymbol& Type, const Get_FuncInfo& Func, const ValueParametersNode& ValuePars)
{
	String B = ToString(Type);
	Token T;
	T.Type = TokenType::Name;
	T.Value._String = B;


	ScopedNameNode Tep;
	ScopedName V;
	V._token = &T;

	Tep._ScopedName.push_back(std::move(V));

	IR_Build_FuncCall(Func, Tep, ValuePars);
}

void SystematicAnalysis::OnFuncCallNode(const FuncCallNode& node)
{
	if (_PassType == PassType::GetTypes)
	{
		for (auto& Item : node.Parameters._Nodes)
		{
			if (Item->Get_Type() != NodeType::OutExpression) 
			{
				OnExpressionTypeNode(Item.get(), GetValueMode::Read);
			}
		}
	}
	else if (_PassType == PassType::FixedTypes)
	{
		auto symid = Symbol_GetSymbolID(node);
		if (!_FuncToSyboID.HasValue(symid))
		{

			auto Info = Type_GetFunc(node._FuncName, node.Parameters, Type_Get_LookingForType());

			if (Info.SymFunc)
			{
				FileDependency_AddDependencyToCurrentFile(Info.SymFunc);
			}

			Type_SetFuncRetAsLastEx(Info);

			if (Info.ThisPar == Get_FuncInfo::ThisPar_t::AutoPushThis && _Varable.size())
			{
				auto& Data = _Varable.top();


				String ThisP = ScopeHelper::ApendedStrings(_FuncStack.front().Pointer->FullName,ThisSymbolName);

				Data._UsedSymbols.push_back(Symbol_GetSymbol(ThisP,SymbolType::ParameterVarable).value().value());
			}

			_FuncToSyboID.AddValue(symid, std::move(Info));
		}
		else
		{
			Type_SetFuncRetAsLastEx(_FuncToSyboID.GetValue(symid));
		}
	}
	else if (_PassType == PassType::BuidCode)
	{
		auto& SybID = _FuncToSyboID.GetValue(Symbol_GetSymbolID(node));
		IR_Build_FuncCall(SybID, node._FuncName, node.Parameters);


		auto lasttype = _LastExpressionType;
		auto ir = _IR_LastExpressionField;
		auto lookfortype = _LookingForTypes.top();
		if (!lookfortype.IsAddress() && lasttype.IsAddress())
		{
			auto typetoget = lasttype;
			typetoget._IsAddress = false;
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad_Dereferenc(ir, IR_ConvertToIRType(typetoget));
		}

		if (_GetExpressionMode.size() && IsWrite(_GetExpressionMode.top()))
		{
			int a = 0;
		}
	}
}
void SystematicAnalysis::IR_Build_EnumOut(NeverNullPtr<Symbol> EnumSymbol, IRInstruction* ThisEnum, size_t EnumIndex, const ValueParametersNode& Pars, size_t StartIndex)
{
	EnumInfo* EnumSybInfo = EnumSymbol.value()->Get_Info<EnumInfo>();
	auto& VariantData = EnumSybInfo->VariantData.value();

	EnumFieldInfo& EnumFieldinfo = EnumSybInfo->Fields[EnumIndex];
	EnumVariantField& EnumVariantFieldData = VariantData.Variants[EnumIndex];

	if (EnumSybInfo->IsOptionalAddress())
	{
		IRInstruction* ThisObj = ThisEnum;
		IRInstruction* BoolObj = nullptr;
		


		IRInstruction* ThisobjV = nullptr;
		if (_LastExpressionType.IsAddress())
		{
			auto readtype = _LastExpressionType;
			readtype._IsAddress = false;

			ThisobjV = _IR_LookingAtIRBlock->NewLoad_Dereferenc(ThisObj, IR_ConvertToIRType(readtype));
		}
		else
		{
			ThisobjV = ThisObj;
		}

		auto nullptrval = IR_Load_UIntptr(0);
		BoolObj = EnumVariantFieldData.Types.size() ? _IR_LookingAtIRBlock->NewC_NotEqualto(ThisobjV, nullptrval) : _IR_LookingAtIRBlock->NewC_Equalto(ThisobjV, nullptrval);

		_LastExpressionType = TypeSymbol(TypesEnum::Bool);
		_IR_LastExpressionField = BoolObj;

		for (size_t i = StartIndex; i < Pars._Nodes.size(); i++)
		{
			auto& Item = Pars._Nodes[i];
			OutExpression* outEx = OutExpression::As(Item.get());
			SymbolID ID = Symbol_GetSymbolID(outEx);


			auto& Syb = _Table.GetSymbol(ID);
			IRInstruction* ItemMember;


			ItemMember = ThisobjV;



			Syb.IR_Ins = ItemMember;
		}
	}
	else
	{
		IRInstruction* ThisObj = ThisEnum;
		IRInstruction* BoolObj = nullptr;

		auto Key = LoadEvaluatedEx(EnumFieldinfo.Ex, EnumSybInfo->Basetype);



		auto Member = _IR_LookingAtIRBlock->New_Member_Dereference(ThisObj, IR_ConvertToIRType(EnumSymbol.value()->ID), EnumVarantKeyIndex);

		auto ObjUnion = _IR_LookingAtIRBlock->New_Member_Dereference(ThisObj, IR_ConvertToIRType(EnumSymbol.value()->ID), EnumVarantUnionIndex);


		if (Type_IsPrimitiveNotIncludingPointers(EnumSybInfo->Basetype))
		{
			BoolObj = _IR_LookingAtIRBlock->NewC_Equalto(Member, Key);
		}
		else
		{
			UCodeLangUnreachable();
		}


		IRStruct* UnionStruct = nullptr;
		{
			String UnionName = Str_GetEnumVariantUnionName(EnumSybInfo->FullName);
			IRidentifierID UnionID = _IR_Builder.ToID(UnionName);
			UnionStruct = _IR_Builder.GetSymbol(UnionID)->Get_ExAs<IRStruct>();
		}

		IRStruct* VStruct = nullptr;
		if (EnumVariantFieldData.ClassSymbol.has_value())
		{
			TypeSymbol VSyb = TypeSymbol(EnumVariantFieldData.ClassSymbol.value());
			VStruct = _IR_Builder.GetSymbol(IR_ConvertToIRType(VSyb)._symbol)->Get_ExAs<IRStruct>();
		}

		for (size_t i = StartIndex; i < Pars._Nodes.size(); i++)
		{
			auto& Item = Pars._Nodes[i];
			OutExpression* outEx = OutExpression::As(Item.get());
			SymbolID ID = Symbol_GetSymbolID(outEx);


			auto& Syb = _Table.GetSymbol(ID);
			IRInstruction* ItemMember;

			Optional<size_t> IRFelidIndex;
			for (size_t i = 0; i < EnumIndex + 1; i++)
			{
				auto& VData = EnumSybInfo->VariantData.value().Variants[i];
				if (VData.Types.size())
				{
					IRFelidIndex = IRFelidIndex.has_value() ? IRFelidIndex.value() + 1 : 0;
				}
			}

			if (EnumVariantFieldData.ClassSymbol.has_value())
			{
				auto Struct = _IR_LookingAtIRBlock->New_Member_Access(ObjUnion, UnionStruct, IRFelidIndex.value());
				ItemMember = _IR_LookingAtIRBlock->New_Member_Access(Struct, VStruct, i - StartIndex);
			}
			else
			{
				ItemMember = _IR_LookingAtIRBlock->New_Member_Access(ObjUnion, UnionStruct, IRFelidIndex.value());

			}

			Syb.IR_Ins = ItemMember;
		}

		_LastExpressionType = TypeSymbol(TypesEnum::Bool);
		_IR_LastExpressionField = BoolObj;
	}
}
void SystematicAnalysis::IR_Build_EnumOut(NeverNullPtr<Symbol> EnumSymbol, size_t EnumIndex, const ValueParametersNode& Pars)
{
	EnumInfo* EnumSybInfo = EnumSymbol.value()->Get_Info<EnumInfo>();
	auto& VariantData = EnumSybInfo->VariantData.value();

	EnumFieldInfo& EnumFieldinfo = EnumSybInfo->Fields[EnumIndex];
	EnumVariantField& EnumVariantFieldData = VariantData.Variants[EnumIndex];


	IRInstruction* ThisObj = nullptr;
	{
		TypeSymbol Par = EnumSymbol.value()->ID;
		Par._IsAddress = true;

		auto& Item = Pars._Nodes[0];

		_LookingForTypes.push(Par);

		OnExpressionTypeNode(Item.get(), GetValueMode::Read);
		IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, Par);

		ThisObj = _IR_LastExpressionField;

		_LookingForTypes.pop();
	}
	IR_Build_EnumOut(EnumSymbol,ThisObj, EnumIndex, Pars, 1);

}
void SystematicAnalysis::IR_Build_FuncCall(Get_FuncInfo Func, const ScopedNameNode& Name, const ValueParametersNode& Pars)
{
	if (_PassType != PassType::BuidCode) { return; }

	Debug_Add_SetLineNumber(NeverNullptr(Name._ScopedName.begin()->_token), _IR_LookingAtIRBlock->Instructions.size());
	{

#define PrimitiveTypeCall(FullName,TypeEnum,DefaultValue) if (ScopedName == FullName) \
		{\
			TypeSymbol iNfo;\
			iNfo.SetType(TypeEnum);\
			if (Pars._Nodes.size())\
			{\
				_LookingForTypes.push(iNfo);\
				auto& Item = Pars._Nodes[0];\
				OnExpressionTypeNode(Item.get(),GetValueMode::Read);\
				auto extype = _LastExpressionType; \
				{ \
					auto symop = Symbol_GetSymbol(extype); \
					if (symop.has_value())\
					{\
						auto sym = symop.value();\
						if (sym->Type == SymbolType::Type_Pack)\
						{\
							TypePackInfo* info = sym->Get_Info<TypePackInfo>();\
							if (info->List.size() == 0)\
							{\
								_LastExpressionType = iNfo;\
								DefaultValue;\
							}\
							else \
							{\
								_LastExpressionType = info->List[0]; \
								size_t irindex = _IR_LookingAtIRFunc->Pars.size() - info->List.size(); \
								auto irpar = &_IR_LookingAtIRFunc->Pars[irindex]; \
								_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad(irpar); \
							}\
						}\
					}\
				}\
				IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, iNfo);\
				_LookingForTypes.pop();\
			}\
			else\
			{\
				_LastExpressionType = iNfo;\
				DefaultValue;\
			}\
			return;\
		}\

				auto ScopedName = Str_GetScopedNameAsString(Name);
		auto SymbolsV = Symbol_GetSymbol(ScopedName, SymbolType::Any);
		if (SymbolsV && SymbolsV.value()->Type == SymbolType::Type_alias)
		{
			ScopedName = ToString(SymbolsV.value()->VarType);
		}
				
		PrimitiveTypeCall(Uint8TypeName, TypesEnum::uInt8, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((UInt8)0);)
		else PrimitiveTypeCall(Uint16TypeName, TypesEnum::uInt16, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((UInt16)0))
		else PrimitiveTypeCall(Uint32TypeName, TypesEnum::uInt32, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((UInt32)0))
		else PrimitiveTypeCall(Uint64TypeName, TypesEnum::uInt64, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad(((UInt64)0)))

		else PrimitiveTypeCall(Sint8TypeName, TypesEnum::sInt8, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((Int8)0);)
		else PrimitiveTypeCall(Sint16TypeName, TypesEnum::sInt16, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((Int16)0))
		else PrimitiveTypeCall(Sint32TypeName, TypesEnum::sInt32, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((Int32)0))
		else PrimitiveTypeCall(Sint64TypeName, TypesEnum::sInt64, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((Int64)0))

		else PrimitiveTypeCall(boolTypeName, TypesEnum::Bool, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad(false))
		else PrimitiveTypeCall(CharTypeName, TypesEnum::Char, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad('\0'))
		else PrimitiveTypeCall(Uft8typeName, TypesEnum::Uft8, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad(Utf8('\0')))
		else PrimitiveTypeCall(Uft16typeName, TypesEnum::Uft16, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad(Utf16('\0')))
		else PrimitiveTypeCall(Uft32typeName, TypesEnum::Uft32, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad(Utf32('\0')))



		else PrimitiveTypeCall(float32TypeName, TypesEnum::float32, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((float32)0))
		else PrimitiveTypeCall(float64TypeName, TypesEnum::float64, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((float64)0))

		else PrimitiveTypeCall(UintPtrTypeName, TypesEnum::uIntPtr, _IR_LastExpressionField = IR_Load_UIntptr(0))
		else PrimitiveTypeCall(SintPtrTypeName, TypesEnum::sIntPtr, _IR_LastExpressionField = IR_Load_SIntptr(0))


	}
	if (Func.Func == nullptr)
	{

		if (Func.SymFunc)
		{
			if (Func.SymFunc->Type == SymbolType::Enum_Field)
			{
				auto ScopedName = Str_GetScopedNameAsString(Name);
				String EnumClassFullName = Func.SymFunc->FullName;
				ScopeHelper::ReMoveScope(EnumClassFullName);


				auto EnumSymbol = Symbol_GetSymbol(EnumClassFullName, SymbolType::Enum);
				if (EnumSymbol)
				{
					if (Func.ThisPar == Get_FuncInfo::ThisPar_t::NoThisPar_GetValue_EnumVariant)
					{
						EnumInfo* EnumSybInfo = EnumSymbol.value()->Get_Info<EnumInfo>();
						IR_Build_EnumOut(EnumSymbol.value(), EnumSybInfo->GetFieldIndex(ScopeHelper::GetNameFromFullName(ScopedName)).value(), Pars);
					}
					else
					{
						EnumInfo* EnumSybInfo = EnumSymbol.value()->Get_Info<EnumInfo>();
						auto& VariantData = EnumSybInfo->VariantData.value();
						size_t EnumIndex = EnumSybInfo->GetFieldIndex(ScopeHelper::GetNameFromFullName(ScopedName)).value();

						EnumFieldInfo& EnumFieldinfo = EnumSybInfo->Fields[EnumIndex];
						EnumVariantField& EnumVariantFieldData = VariantData.Variants[EnumIndex];

						if (EnumSybInfo->IsOptionalAddress())
						{
							auto& FuncParInfo = EnumVariantFieldData.Types[0];
							auto& Item = Pars._Nodes[0];



							_LookingForTypes.push(FuncParInfo);
							OnExpressionTypeNode(Item.get(), GetValueMode::Read);
							//OnExpressionTypeNode sets	_IR_LastExpressionField 


							_LookingForTypes.pop();
						}
						else
						{
							auto ID = _IR_Builder.ToID(EnumSybInfo->FullName);

							auto Key = LoadEvaluatedEx(EnumFieldinfo.Ex, EnumSybInfo->Basetype);

							auto VariantClass = _IR_LookingAtIRBlock->NewLoad(IRType(ID));
							IRStruct* V = _IR_Builder.GetSymbol(ID)->Get_ExAs<IRStruct>();
							auto Member = _IR_LookingAtIRBlock->New_Member_Access(VariantClass, V, EnumVarantKeyIndex);
							_IR_LookingAtIRBlock->NewStore(Member, Key);

							if (EnumVariantFieldData.Types.size()) {
								auto UnionMember = _IR_LookingAtIRBlock->New_Member_Access(VariantClass, V, EnumVarantUnionIndex);


								String UnionName = Str_GetEnumVariantUnionName(EnumSybInfo->FullName);
								IRidentifierID UnionID = _IR_Builder.ToID(UnionName);

								Optional<size_t> IRFelidIndex;
								for (size_t i = 0; i < EnumIndex + 1; i++)
								{
									auto& VData = EnumSybInfo->VariantData.value().Variants[i];
									if (VData.Types.size())
									{
										IRFelidIndex = IRFelidIndex.has_value() ? IRFelidIndex.value() + 1 : 0;
									}
								}

								auto ObjectMember = _IR_LookingAtIRBlock->New_Member_Access(UnionMember, _IR_Builder.GetSymbol(UnionID)->Get_ExAs<IRStruct>(), IRFelidIndex.value());

								IRStruct* VStruct = nullptr;
								if (EnumVariantFieldData.ClassSymbol.has_value())
								{
									TypeSymbol VSyb = TypeSymbol(EnumVariantFieldData.ClassSymbol.value());
									VStruct = _IR_Builder.GetSymbol(IR_ConvertToIRType(VSyb)._symbol)->Get_ExAs<IRStruct>();
								}
								//
								for (size_t i = 0; i < Pars._Nodes.size(); i++)
								{
									auto& Item = Pars._Nodes[i];
									auto& FuncParInfo = EnumVariantFieldData.Types[i];



									_LookingForTypes.push(FuncParInfo);

									OnExpressionTypeNode(Item.get(), GetValueMode::Read);
									IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, FuncParInfo);

									auto ParEx = _IR_LastExpressionField;

									if (EnumVariantFieldData.ClassSymbol.has_value())
									{
										auto VMember = _IR_LookingAtIRBlock->New_Member_Access(ObjectMember, VStruct, i);
										_IR_LookingAtIRBlock->NewStore(VMember, ParEx);
									}
									else
									{
										_IR_LookingAtIRBlock->NewStore(ObjectMember, ParEx);
									}
									_LookingForTypes.pop();
								}
								//


							}

							_IR_LastExpressionField = VariantClass;
						}
						_LastExpressionType = EnumSymbol.value()->VarType;
					}
				}
			}
		}
		else if (Func._BuiltFunc.has_value())
		{
			auto& Value = Func._BuiltFunc.value();

			if (Value.EvalObject.has_value())
			{
				auto& EvalObject = Value.EvalObject.value();
				if (Value.EvalAsCString)
				{
					_IR_LastExpressionField = IR_RawObjectDataToCString(EvalObject);

					_LastExpressionType = TypesEnum::Char;
					_LastExpressionType.SetAsAddressArray();
					_LastExpressionType.SetAsimmutable();
				}
				else
				{
					_IR_LastExpressionField = LoadEvaluatedEx(EvalObject, Value.RetType);
				}
			}
			else
			{

				UCodeLangUnreachable();
			}

			_LastExpressionType = Value.RetType;
		}
		else
		{
			UCodeLangUnreachable();
		}
		return;
	}

	IRInstruction* PushIRStackRet = nullptr;
	bool AutoPushThis = Get_FuncInfo::AddOneToGetParNode(Func.ThisPar);

	Vector< IRInstruction*> IRParsList;

	if (AutoPushThis)
	{
		if (Func.ThisPar == Get_FuncInfo::ThisPar_t::FullScopedName)
		{
			_GetExpressionMode.push(GetValueMode::Read);
			GetMemberTypeSymbolFromVar_t V;
			Symbol_MemberTypeSymbolFromVar(0, Name._ScopedName.size(), Name, V);
			_GetExpressionMode.pop();

			IRParsList.push_back(IR_Build_Member_AsPointer(V));
		}
		else
			if (Func.ThisPar == Get_FuncInfo::ThisPar_t::PushFromScopedName)
			{
				_LookingForTypes.push(Func.Func->Pars[0].Type);
				
				_GetExpressionMode.push(GetValueMode::Read);
				GetMemberTypeSymbolFromVar_t V;
				Symbol_MemberTypeSymbolFromVar(0, Name._ScopedName.size() - 1, Name, V);
				_GetExpressionMode.pop();


				IRParsList.push_back(IR_Build_Member_AsPointer(V));

				_LookingForTypes.pop();
			}
			else if (Func.ThisPar == Get_FuncInfo::ThisPar_t::PushFromLast)
			{
				IRParsList.push_back(_IR_LastExpressionField);
			}
			else if (Func.ThisPar == Get_FuncInfo::ThisPar_t::OnIRlocationStack)
			{

				bool UseedTopIR = _IR_IRlocations.size() != 0 && _IR_IRlocations.top().UsedlocationIR == false;
				if (UseedTopIR)
				{
					auto Type = Func.Func->Pars[0];

					auto v = IR_ConvertToIRType(Type);



					if (v._symbol.ID != _IR_IRlocations.top().Value->ObjectType._symbol.ID)
					{
						UseedTopIR = false;
					}
				}
			
				if (!UseedTopIR)
				{
					IRLocation_Cotr tep;
					tep.UsedlocationIR = false;

					auto Type = Func.Func->Pars[0];
					if (Type.Type.IsAddress())
					{
						Type.Type._IsAddress = false;
					}

					PushIRStackRet = tep.Value = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(Type));
					_IR_IRlocations.push(tep);
				}
				else
				{
					PushIRStackRet = _IR_IRlocations.top().Value;

				}



				{
					auto Defe = _IR_LookingAtIRBlock->NewLoadPtr(_IR_IRlocations.top().Value);
					_IR_IRlocations.top().UsedlocationIR = true;
					IRParsList.push_back(Defe);
				}

				if (!UseedTopIR)
				{
					_IR_IRlocations.pop();
				}

			}
			else if (Func.ThisPar == Get_FuncInfo::ThisPar_t::OnIRlocationStackNonedef)
			{
				bool UseedTopIR = _IR_IRlocations.size() != 0 && _IR_IRlocations.top().UsedlocationIR == false;
				if (!UseedTopIR)
				{
					IRLocation_Cotr tep;
					tep.UsedlocationIR = false;

					auto Type = Func.Func->Pars[0];
					if (Type.Type.IsAddress())
					{
						Type.Type._IsAddress = false;
					}


					PushIRStackRet = tep.Value = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(Type.Type));
					_IR_IRlocations.push(tep);
				}

				{
					IRParsList.push_back(_IR_IRlocations.top().Value);
					_IR_IRlocations.top().UsedlocationIR = true;
				}

				if (!UseedTopIR)
				{
					_IR_IRlocations.pop();
				}
			}
			else if (Func.ThisPar == Get_FuncInfo::ThisPar_t::PushWasCalled)
			{
				UCodeLangUnreachable();//just add IRPar list
				//what does  "just add IRPar list" mean
			}
			else if (Func.ThisPar == Get_FuncInfo::ThisPar_t::AutoPushThis)
			{

				auto& InFunc = _FuncStack.back().Pointer;

				auto ThisParSym = Symbol_GetSymbol(InFunc->Pars.front().Type).value();
				if (IsSymbolLambdaObjectClass(ThisParSym))
				{
					ClassInfo* f = ThisParSym->Get_Info<ClassInfo>();
					auto parsym = Symbol_GetSymbol(ScopeHelper::ApendedStrings(ThisParSym->FullName, ThisSymbolName), SymbolType::ParameterVarable).value();

					auto PointerIr = _IR_LookingAtIRBlock->New_Member_Dereference(
						&_IR_LookingAtIRFunc->Pars.front(),
						_IR_LookingAtIRFunc->Pars.front().type,
						f->GetFieldIndex(ThisSymbolName).value());

					IRParsList.push_back(PointerIr);
				}
				else
				{
					IRParsList.push_back(_IR_LookingAtIRBlock->NewLoad(&_IR_LookingAtIRFunc->Pars.front()));
				}
			}
			else if (Func.ThisPar == Get_FuncInfo::ThisPar_t::PushFromScopedNameDynamicTrait)
			{
				_GetExpressionMode.push(GetValueMode::Read);
				GetMemberTypeSymbolFromVar_t V;
				Symbol_MemberTypeSymbolFromVar(0, Name._ScopedName.size() - 1, Name, V);
				_GetExpressionMode.pop();

				IRParsList.push_back(IR_Build_Member_AsPointer(V));
			}
			else
			{
				UCodeLangUnreachable();;
			}
	}


	for (size_t i = 0; i < Pars._Nodes.size(); i++)
	{
		auto& Item = Pars._Nodes[i];

		auto Index = i + (AutoPushThis ? 1 : 0);

		const Vector<ParInfo>& Pars = Func.SymFunc->Type == SymbolType::Func
			? Func.Func->Pars
			: ((FuncPtrInfo*)Func.Func)->Pars;



		auto FuncParInfoPtr = Index < Pars.size() ? Pars[Index]
			: Pars.back();

		if (FuncParInfoPtr.Type._CustomTypeSymbol.HasValue())
		{
			const auto Syb = Symbol_GetSymbol(FuncParInfoPtr.Type).value();
			if (Syb->Type == SymbolType::Type_Pack)
			{
				auto* typepack = Syb->Get_Info<TypePackInfo>();
				size_t NewIndex = Index - (Pars.size() - 1);


				FuncParInfoPtr.Type = typepack->List[NewIndex];
			}
		}
		auto& FuncParInfo = FuncParInfoPtr;

		

		_LookingForTypes.push(FuncParInfo.Type);

		if (Item->Get_Type() == NodeType::OutExpression)
		{
			const OutExpression* OutEx = OutExpression::As(Item.get());

			auto OutVal = Symbol_GetSymbolFromExpression(NeverNullptr(OutEx));

			OutVal->IR_Ins = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(OutVal->VarType));

			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoadPtr(OutVal->IR_Ins);
		}
		else
		{
			OnExpressionTypeNode(Item.get(), GetValueMode::Read);
			IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, FuncParInfo.Type);
		}
		
		bool unpackparpack = true;
		if (unpackparpack)
		{
			auto extype = _LastExpressionType;

			auto symop = Symbol_GetSymbol(extype);
			if (symop.has_value())
			{
				auto& sym = symop.value();

				if (sym->Type == SymbolType::Type_Pack)
				{
					const TypePackInfo* info = sym->Get_Info<TypePackInfo>();

					_LookingForTypes.pop();

					size_t parindex = Index;
					for (size_t i = 0; i < info->List.size(); i++)
					{
						auto& packtype = info->List[i];
						auto& irpar = _IR_LookingAtIRFunc->Pars[parindex + i];

						IRParsList.push_back(_IR_LookingAtIRBlock->NewLoad(&irpar));
					}

					break;
				}

			}
		}

		IRParsList.push_back(_IR_LastExpressionField);
		_LookingForTypes.pop();

	}
	auto Syb = Func.SymFunc;

	FileDependency_AddDependencyToCurrentFile(Syb);

	//

	if (Func.ThisPar != Get_FuncInfo::ThisPar_t::PushFromScopedNameDynamicTrait)
	{
		for (auto& Item : IRParsList)
		{
			_IR_LookingAtIRBlock->NewPushParameter(Item);
		}
	}
	//
	if (Func.ThisPar == Get_FuncInfo::ThisPar_t::PushFromScopedNameDynamicTrait)
	{
		GetMemberTypeSymbolFromVar_t V;
		Symbol_MemberTypeSymbolFromVar(0, Name._ScopedName.size() - 1, Name, V);

		auto TraitType = V.Type;
		auto TraitSyb = Symbol_GetSymbol(TraitType).value();
		auto TraitAsIR = IR_ConvertToIRType(TraitType);

		auto VPtrMember = _IR_LookingAtIRBlock->New_Member_Dereference(IRParsList.front(), TraitAsIR, 1);


		TraitInfo* Info = TraitSyb->Get_Info<TraitInfo>();
		size_t FuncIndex = Info->GetIndex_Func(Func.SymFunc).value();

		auto PtrCall = _IR_LookingAtIRBlock->New_Member_Dereference(VPtrMember, IRType(_IR_Builder.ToID(Str_GetTraitVStructTableName(TraitSyb->FullName))), FuncIndex);



		IRParsList.front() = _IR_LookingAtIRBlock->New_Member_Dereference(IRParsList.front(), TraitAsIR, 0);

		//
		for (auto& Item : IRParsList)
		{
			_IR_LookingAtIRBlock->NewPushParameter(Item);
		}
		_IR_LastExpressionField = _IR_LookingAtIRBlock->NewCallFuncPtr(PtrCall);
		//
	}
	else if (Syb->Type == SymbolType::Func)
	{
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewCall(IR_GetIRID(Func.Func));
	}
	else if (Syb->Type == SymbolType::StackVarable)
	{
		_IR_LastExpressionField = _IR_LookingAtIRBlock->NewCallFuncPtr(Syb->IR_Ins);
	}
	else if (Syb->Type == SymbolType::ParameterVarable)
	{
		_IR_LastExpressionField = _IR_LookingAtIRBlock->NewCallFuncPtr(Syb->IR_Par);
	}
	else
	{
		UCodeLangUnreachable();
	}

	{
		auto Tep = _IR_LastExpressionField;

		/*
		for (size_t i = 0; i < IRParsList.size(); i++)
		{
			auto& Item = IRParsList[i];
			auto& ItemType = Func.SymFunc->Type == SymbolType::Func ? Func.Func->Pars[i]
				: ((FuncPtrInfo*)Func.Func)->Pars[i];

			if (ItemType._IsAddress == false && HasDestructor(ItemType))
			{
				ObjectToDrop obj;
				obj.DropType = ObjectToDropType::IRInstructionNoMod;
				obj.Type = ItemType;
				obj._Object = Item;



				if (!obj.Type.IsAddress())//will not work if Destructor doesn't taken in pointer
				{
					obj._Object = LookingAtIRBlock->NewLoadPtr(obj._Object);
				}

				obj.Type._IsAddress = false;
				DoDestructorCall(obj);
			}

		}
		*/
		_IR_LastExpressionField = Tep;
	}

	if (_LookingForTypes.size() && Type_Get_LookingForType().IsnotAn(TypesEnum::Void) && PushIRStackRet)//constructors are just void functions so just set last as the input this
	{
		if (ScopeHelper::GetNameFromFullName(Syb->FullName) == ClassConstructorfunc) {
			_IR_LastExpressionField = PushIRStackRet;
			_LastExpressionType = Func.Func->Pars.front().Type;


			if (Type_Get_LookingForType().IsAddress())
			{
				_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoadPtr(_IR_LastExpressionField);
			}
			else
			{
				_LastExpressionType._IsAddress = false;
			}

		}
	}
	else
	{
		Type_SetFuncRetAsLastEx(Func);

		if (_GetExpressionMode.size() && IsWrite(_GetExpressionMode.top()))
		{
			_IR_LastStoreField = IROperator(_IR_LastExpressionField);
		}

	}
}


bool FuncInferFuncCountOk(SymbolType symtype,const FuncInfo* Info,size_t parcount,bool& islastparispack)
{
	bool r = Info->Pars.size() == parcount;
	{
		if (symtype == SymbolType::GenericFunc && Info->_GenericData.IsPack())
		{
			bool lastparispack = false;
			if (Info->Pars.size())
			{
				auto& last = Info->Pars.back();
				lastparispack = last.Type._CustomTypeSymbol == Info->_GenericData._Genericlist.back().SybID;
			}

			if (lastparispack)
			{
				size_t minfuncparcount = Info->Pars.size() - 1;
				if (parcount >= minfuncparcount)
				{
					r = true;
					islastparispack = true;
				}
			}
		}
	}

	return r;
}
void TryaddParPackInferTypes(bool islastparpack,const FuncInfo* Info,size_t parcount,bool PushThisPar,bool& Inferautopushtis,Vector<ParInfo>& Infer,bool useinferpars)
{
	if (islastparpack)
	{
		size_t minfuncparcount = Info->Pars.size() - 1;

		auto& pack = Info->_GenericData._Genericlist.back();
		if (pack.BaseOrRule.has_value())
		{

			auto& rule = pack.BaseOrRule.value();
			if (auto basetype = rule.Get_If<TypeSymbol>())
			{
				if (!useinferpars)
				{
					Infer = Info->Pars;
				}
				Infer.pop_back();

				size_t addparcount = parcount - minfuncparcount;
				for (size_t i = 0; i < addparcount; i++)
				{
					ParInfo v;
					v.Type = *basetype;
					Infer.push_back(v);
				}

				Inferautopushtis = PushThisPar;
			}
			else
			{
				UCodeLangUnreachable();
			}

		}
	}
}

SystematicAnalysis::Get_FuncInfo  SystematicAnalysis::Type_GetFunc(const ScopedNameNode& Name, const ValueParametersNode& Pars, TypeSymbol Ret)
{




	TypeSymbol _ThisType;
	Get_FuncInfo::ThisPar_t ThisParType = Get_FuncInfo::ThisPar_t::NoThisPar;
	String ScopedName;
	{

		bool IsThisCall = false;
		for (auto& Item : Name._ScopedName)
		{
			if (Item._operator == ScopedName::Operator_t::Dot)
			{
				IsThisCall = true;
				break;
			}
		}

		if (IsThisCall)
		{
			_GetExpressionMode.push(GetValueMode::Read);
			GetMemberTypeSymbolFromVar_t V;
			bool VBool = Symbol_MemberTypeSymbolFromVar(0, Name._ScopedName.size() - 1, Name, V);
			_GetExpressionMode.pop();

			if (VBool)
			{
				_ThisType = V.Type;
				_ThisType.SetAsAddress();

				auto tep_ = V.Type;

				if (tep_._IsDynamic)
				{
					tep_._IsDynamic = false;
					ThisParType = Get_FuncInfo::ThisPar_t::PushFromScopedNameDynamicTrait;
				}
				else
				{
					ThisParType = Get_FuncInfo::ThisPar_t::PushFromScopedName;
				}
				Type_RemoveTypeattributes(tep_);


				ScopedName = ToString(tep_) + ScopeHelper::_ScopeSep;
				ScopedName += Name._ScopedName.back()._token->Value._String;
				if (_ThisType.IsBadType())
				{
					Get_FuncInfo V;
					V.CantCheckBecauseIsUnMaped = true;

					return V;//can't check because we are just testing.
				}
			}
			else
			{
				return { Get_FuncInfo::ThisPar_t::NoThisPar, nullptr };
			}
		}
		else
		{
			ScopedName = "";
			for (auto& Item : Name._ScopedName)
			{
				if (Item._generic.get() && Item._generic->_Values.size())
				{

					ScopedName += Item._token->Value._String;

					auto TypeName = ScopedName;

					bool IsLast = &Item == &Name._ScopedName.back();
					if (!IsLast)
					{
						auto Syb = Generic_InstantiateOrFindGenericSymbol(NeverNullptr(Item._token), *Item._generic, ScopedName);

						if (!Syb.has_value())
						{
							return {};
						}
						else
						{
							ScopedName = Syb.value()->FullName;
						}
					}
					if (Item._operator != ScopedName::Operator_t::Null)
					{
						ScopedName += ScopeHelper::_ScopeSep;
					}
				}
				else
				{
					String Str;
					Item.GetScopedName(Str);
					ScopedName += Str;
					if (Item._operator != ScopedName::Operator_t::Null)
					{
						ScopedName += ScopeHelper::_ScopeSep;
					}
				}
			}
		}
	}
	bool _ThisTypeIsNotNull = !_ThisType.IsNull();

	_LastLookedAtToken = Nullableptr(Name._ScopedName.back()._token);
	{

		auto SymbolsV = Symbol_GetSymbol(ScopedName, SymbolType::Any);


		if (SymbolsV && SymbolsV.value()->Type == SymbolType::Type_alias)
		{
			ScopedName = ToString(SymbolsV.value()->VarType);
		}


		if (ScopedName == Uint8TypeName ||
			ScopedName == Uint16TypeName ||
			ScopedName == Uint32TypeName ||
			ScopedName == Uint64TypeName ||
			ScopedName == Sint8TypeName ||
			ScopedName == Sint16TypeName ||
			ScopedName == Sint32TypeName ||
			ScopedName == Sint64TypeName ||
			ScopedName == SintPtrTypeName ||
			ScopedName == UintPtrTypeName ||
			ScopedName == boolTypeName ||
			ScopedName == CharTypeName ||
			ScopedName == Uft8typeName ||
			ScopedName == Uft16typeName ||
			ScopedName == Uft32typeName ||
			ScopedName == float32TypeName ||
			ScopedName == float64TypeName)
		{
			{
				if (ScopedName == Uint8TypeName) { _LastExpressionType = TypesEnum::uInt8; }
				else if (ScopedName == Uint16TypeName) { _LastExpressionType = TypesEnum::uInt16; }
				else if (ScopedName == Uint32TypeName) { _LastExpressionType = TypesEnum::uInt32; }
				else if (ScopedName == Uint64TypeName) { _LastExpressionType = TypesEnum::uInt64; }

				else if (ScopedName == Sint8TypeName) { _LastExpressionType = TypesEnum::sInt8; }
				else if (ScopedName == Sint16TypeName) { _LastExpressionType = TypesEnum::sInt16; }
				else if (ScopedName == Sint32TypeName) { _LastExpressionType = TypesEnum::sInt32; }
				else if (ScopedName == Sint64TypeName) { _LastExpressionType = TypesEnum::sInt64; }

				else if (ScopedName == SintPtrTypeName) { _LastExpressionType = TypesEnum::sIntPtr; }
				else if (ScopedName == UintPtrTypeName) { _LastExpressionType = TypesEnum::uIntPtr; }

				else if (ScopedName == boolTypeName) { _LastExpressionType = TypesEnum::Bool; }
				else if (ScopedName == CharTypeName) { _LastExpressionType = TypesEnum::Char; }
				else if (ScopedName == Uft8typeName) { _LastExpressionType = TypesEnum::Uft8; }
				else if (ScopedName == Uft16typeName) { _LastExpressionType = TypesEnum::Uft16; }
				else if (ScopedName == Uft32typeName) { _LastExpressionType = TypesEnum::Uft32; }

				else if (ScopedName == float32TypeName) { _LastExpressionType = TypesEnum::float32; }
				else if (ScopedName == float64TypeName) { _LastExpressionType = TypesEnum::float64; }

				else
				{
					UCodeLangUnreachable();
				}
			}


			size_t parcount = 0;
			if (Pars._Nodes.size() == 1)
			{
				auto FuncType = _LastExpressionType;

				_LookingForTypes.push(FuncType);
				OnExpressionTypeNode(Pars._Nodes[0].get(), GetValueMode::Read);
				_LookingForTypes.pop();

				auto ex = _LastExpressionType;
				parcount = 1;
				auto symop = Symbol_GetSymbol(ex);
				if (symop.has_value())
				{
					auto sym = symop.value();
					if (sym->Type == SymbolType::Type_Pack)
					{
						auto info = sym->Get_Info<TypePackInfo>();

						parcount = info->List.size();
						if (parcount) {
							ex = info->List[0];
						}
					}
				}

				if (parcount != 0 && !Type_CanBeImplicitConverted(ex, FuncType, true))
				{
					LogError_CantCastImplicitTypes(_LastLookedAtToken.value(),ex, FuncType, true);
				}
				_LastExpressionType = FuncType;

				
			}
			else
			{
				parcount = Pars._Nodes.size();
			}

			if (parcount > 1)
			{
				LogError_CanIncorrectParCount(NeverNullptr(Name._ScopedName.back()._token), ScopedName, parcount, 1);
			}

			Get_FuncInfo r = { Get_FuncInfo::ThisPar_t::NoThisPar, nullptr };
			r.BulitInTypeContructer = _LastExpressionType;
			return r;
		}




	}

	bool AutoThisCall = false;

	SymbolType T = SymbolType::Null;
	Symbol* FuncSymbol = nullptr;
	FuncInfo* r = nullptr;

	auto& RetType = Type_Get_LookingForType();
	bool RetIsSet = !(RetType.IsAn(TypesEnum::Var) || RetType.IsAn(TypesEnum::Any));


	Vector<ParInfo> ValueTypes;
	ValueTypes.reserve(_ThisTypeIsNotNull ? Pars._Nodes.size() + 1 : Pars._Nodes.size());

	auto Symbols = GetSymbolsWithName(ScopedName, SymbolType::Any);

	Vector<ParInfo> Infer;
	bool Inferautopushtis = false;

	{//for type
		auto fortypeSyms = GetSymbolsWithName(ForTypeScope, SymbolType::ForType);

		TypeSymbol maintype;

		if (ThisParType == Get_FuncInfo::ThisPar_t::PushFromScopedName)
		{
			auto v = _ThisType;
			v._IsAddress = false;
			maintype = v;
		}
		else
		{
			auto scope = ScopedName;
			ScopeHelper::ReMoveScope(scope);
			auto v = Symbol_GetSymbol(scope, SymbolType::Type);

			if (auto val = v.value_unchecked())
			{
				maintype = val->VarType;
			}
		}

		if (auto maintypesym = Symbol_GetSymbol(maintype).value_unchecked()) 
		{
			String mytypestr = ToString(maintype);
			
			for (auto& Item : fortypeSyms)
			{
				if (Item->Type == SymbolType::ForType)
				{
					Symbol_Update_ForType_ToFixedTypes(Item);

					bool isreferringtomytype = false;

					auto fortype = Item->VarType;

					isreferringtomytype = Type_AreTheSame(maintype, fortype);

					if (isreferringtomytype)
					{
						ForTypeInfo* info = Item->Get_Info<ForTypeInfo>();

						for (auto& Item : info->Funcs)
						{
							Symbols.push_back(Item);
						}
					}
				}
				else if (Item->Type == SymbolType::GenericForType)
				{


					Symbol_Update_ForType_ToFixedTypes(Item);

					ForTypeInfo* info = Item->Get_Info<ForTypeInfo>();
					const ForTypeNode* fornod = Item->Get_NodeInfo<ForTypeNode>();

					String GennericName;

					fornod->_typetoaddto._name.GetScopedName(GennericName);

					if (StringHelper::Contains(mytypestr, GennericName))
					{
						NullablePtr<Symbol> forgeneric;
						NullablePtr<Symbol> FindSym;
						{
							auto OldContext = SaveAndMove_SymbolContext();
							Set_SymbolContext(info->Context.value());

							FindSym = Symbol_GetSymbol(GennericName, SymbolType::Generic_class);

							Set_SymbolContext(std::move(OldContext));
						}

						auto fortypetoken = NeverNullptr(fornod->_typetoaddto._name._ScopedName.front()._token);

						if (!FindSym.has_value())
						{
							LogError_CantFindSymbolError(fortypetoken, GennericName);

							return {};
						}

						auto foundsym = FindSym.value();

						bool isgennerictype = (foundsym->Type == SymbolType::Generic_Alias
							|| foundsym->Type == SymbolType::Generic_class
							|| foundsym->Type == SymbolType::Generic_Enum);
						//TODO Generic_traits


						if (!isgennerictype)
						{
							LogError(ErrorCodes::InValidType, "The Symbol '" + foundsym->FullName + "' in for type must be an generic type", fortypetoken);

							return {};
						}

						NeverNullPtr<Generic> GenericData;
						NeverNullPtr<Generic> MyBaseTypeGenericData;


						NeverNullPtr<Symbol> mytypebasesymbol;
						{
							String scope = mytypestr;

							{
								size_t gwnericcount = 0;
								for (int i = scope.size() - 1; i >= 0; i--)
								{
									char item = scope[i];

									if (item == '<')
									{
										gwnericcount--;

										if (gwnericcount == 0)
										{
											scope = scope.substr(0, scope.size() - i);
											break;
										}
									}
									else if (item == '>')
									{
										gwnericcount++;
									}


								}
							}

							mytypebasesymbol = Symbol_GetSymbol(scope, SymbolType::Type).value();
						}

						switch (mytypebasesymbol->Type)
						{
						case SymbolType::Generic_Alias:
							MyBaseTypeGenericData = NeverNullptr(&mytypebasesymbol->Get_Info<Generic_AliasInfo>()->_GenericData);
							break;
						case SymbolType::Generic_class:
							MyBaseTypeGenericData = NeverNullptr(&mytypebasesymbol->Get_Info<ClassInfo>()->_GenericData);
							break;
						case SymbolType::Generic_Enum:
							MyBaseTypeGenericData = NeverNullptr(&mytypebasesymbol->Get_Info<EnumInfo>()->_GenericData);
							break;
						default:
							continue;
							break;
						}

						switch (foundsym->Type)
						{
						case SymbolType::Generic_Alias:
							GenericData = NeverNullptr(&foundsym->Get_Info<Generic_AliasInfo>()->_GenericData);
							break;
						case SymbolType::Generic_class:
							GenericData = NeverNullptr(&foundsym->Get_Info<ClassInfo>()->_GenericData);
							break;
						case SymbolType::Generic_Enum:
							GenericData = NeverNullptr(&foundsym->Get_Info<EnumInfo>()->_GenericData);
							break;
						default:
							UCodeLangUnreachable();
							break;
						}


						size_t myTypeGenericCount = MyBaseTypeGenericData->GetMinimumCount();
						size_t foundGenericCount = GenericData->GetMinimumCount();

						if (myTypeGenericCount != foundGenericCount)
						{
							LogError_CanIncorrectGenericCount(fortypetoken, foundsym->FullName, myTypeGenericCount, foundGenericCount);
							return {};
						}

						for (size_t i = 0; i < myTypeGenericCount; i++)
						{
							auto& mygen = MyBaseTypeGenericData->_Genericlist[i];
							auto& foundgen = GenericData->_Genericlist[i];

							if (mygen.IsConstantExpression() != foundgen.IsConstantExpression())
							{
								LogError(ErrorCodes::TreeAnalyerError, "Generic type/Generic Constant miss match on '" + std::to_string(i) + "' generic element", fortypetoken);
								return {};
							}
						}

						Vector<TypeSymbol> generics;

						UCodeLangAssert(!GenericData->IsPack());
						for (auto& item : MyBaseTypeGenericData->_Genericlist)
						{
							String GenericAliasName;

							{
								auto v = Symbol_GetSymbol(item.SybID);
								GenericAliasName = ScopeHelper::GetNameFromFullName(v->FullName);
							}

							NeverNullPtr<Symbol> mytypegenericalias;
							{
								String scope = ScopeHelper::ApendedStrings(mytypestr, GenericAliasName);

								mytypegenericalias = Symbol_GetSymbol(scope, SymbolType::Any).value();
							}

							generics.push_back(TypeSymbol(mytypegenericalias->VarType));
						}

					
						auto newsymname = Generic_SymbolGenericFullName(Item, generics);
						auto hasthisformade = Symbol_GetSymbol(newsymname,SymbolType::ForType);

						if (!hasthisformade.has_value())
						{
							Generic_TypeInstantiate_ForType(NeverNullptr(Item), generics);
							hasthisformade = Symbol_GetSymbol(newsymname,SymbolType::ForType);
						}
		
						{
							ForTypeInfo* info = hasthisformade.value()->Get_Info<ForTypeInfo>();

							for (auto& Item : info->Funcs)
							{
								Symbols.push_back(Item);
							}
						}
	
					}

				}

			}
		}
	}

	{

		for (auto& Item : Symbols)
		{
			if (Item->Type == SymbolType::Func)
			{
				if (Item->IsInvalid())
				{
					return {};
				}
				const FuncInfo* Info = Item->Get_Info<FuncInfo>();
				Symbol_Update_FuncSym_ToFixedTypes(Item);

				size_t parcount = _ThisTypeIsNotNull ? Pars._Nodes.size() + 1 : Pars._Nodes.size();
				if (Info->Pars.size() == parcount) {
					Infer = Info->Pars;
					Inferautopushtis = false;
				}
			}
			else if (Item->Type == SymbolType::GenericFunc)
			{
				// Func<T>[T V] -> T
				// uintptr a = Func(0) 
				// becomes
				// uintptr a = Func<uintptr>(0) 
				// and not
				// uintptr a = Func<int>(0) 

				if (Item->IsInvalid())
				{
					return {};
				}
				const FuncInfo* Info = Item->Get_Info<FuncInfo>();
				Symbol_Update_FuncSym_ToFixedTypes(Item);

				bool islastparispack = false;
				bool PushThisPar = Info->IsObjectCall();
				size_t parcount = PushThisPar ? Pars._Nodes.size() + 1 : Pars._Nodes.size();
				if (FuncInferFuncCountOk(Item->Type,Info,parcount,islastparispack)) 
				{
					//Infer = Info->Pars;
					//Inferautopushtis = false;
					//InferFunc = Item;

					auto FuncRet = Info->Ret;
					auto LookForType = _LookingForTypes.top();

					bool IsGenericLookForType = true;
					//To Do Check if LookForType is a Generic of FuncRet
					if (auto Syb = Symbol_GetSymbol(FuncRet).value_unchecked())
					{
						//IsGenericLookForType = Syb->Type == SymbolType::Generic_class;
					}


					if (IsGenericLookForType) {
						auto& Pars = Info->Pars;
						auto& Generic = Info->_GenericData;
						Vector<ParInfo> R;

						UnorderedMap<SymbolID, TypeSymbol> typemap;
						if (auto Val = Symbol_GetSymbol(LookForType).value_unchecked())
						{
							const auto& Funcnode = Item->Get_NodeInfo<FuncNode>();
							auto& rettypenode = Funcnode->_Signature._ReturnType;

							if (rettypenode._name._ScopedName.back()._generic.get()) 
							{
								auto& _generic = *rettypenode._name._ScopedName.back()._generic;
								for (auto& Gtype : _generic._Values)
								{

									Optional<size_t> ToMap;
									for (size_t i = 0; i < Funcnode->_Signature._generic._Values.size(); i++)
									{
										auto& G = Funcnode->_Signature._generic._Values[i];
										if (G.token->Value._String == Gtype._name._ScopedName.back()._token->Value._String)
										{
											ToMap = i;

										}
									}


									if (ToMap.has_value())
									{
										auto MapVal = ToMap.value();
										String FName = (String)Funcnode->_Signature._generic._Values[MapVal].token->Value._String;

										String GName = (String)Gtype._name._ScopedName.back()._token->Value._String;

										String Scope = ScopeHelper::ApendedStrings(Val->FullName, GName);

										auto key = Info->_GenericData._Genericlist[MapVal].SybID;

										if (!typemap.HasValue(key))
										{
											auto ty = Symbol_GetSymbol(Scope, SymbolType::Type_class).value()->VarType;
											typemap.AddValue(key, ty);
										}
									}
								}
							}
						}

						for (auto& Item : Pars)
						{
							Optional<ParInfo> RetType;

							for (auto& GItem : Generic._Genericlist)
							{
								if (typemap.HasValue(GItem.SybID))
								{
									RetType =Opt<ParInfo>({ Item.IsOutPar,typemap.GetValue(GItem.SybID) });
									break;
								}
							}


							R.push_back(RetType.value_or(Item));


						}


						TryaddParPackInferTypes(islastparispack, Info, parcount, false, Inferautopushtis, R,true);
						Infer = std::move(R);
					}
				}
			}
			else if (Item->Type == SymbolType::Type_class)
			{
				ClassInfo* V = Item->Get_Info<ClassInfo>();
				Symbol_Update_ClassSym_ToFixedTypes(Item);
				String Scope = V->FullName;
				ScopeHelper::GetApendedString(Scope, ClassConstructorfunc);

				auto ConstructorSymbols = GetSymbolsWithName(Scope, SymbolType::Any);


				for (auto& Item2 : ConstructorSymbols)
				{
					FuncInfo* Info = Item2->Get_Info<FuncInfo>();
					bool PushThisPar = Info->IsObjectCall();

					size_t parcount = PushThisPar ? Pars._Nodes.size() + 1 : Pars._Nodes.size();

					bool islastparpack = false;

					if (FuncInferFuncCountOk(Item2->Type, Info, parcount, islastparpack))
					{
						Infer = Info->Pars;
						Inferautopushtis = PushThisPar;

						TryaddParPackInferTypes(islastparpack, Info, parcount, PushThisPar, Inferautopushtis, Infer,false);
					}
				}
			}	
			else if (Item->Type == SymbolType::Tag_class)
			{
				TagInfo* V = Item->Get_Info<TagInfo>();
				Symbol_Update_ClassSym_ToFixedTypes(Item);
				String Scope = V->FullName;
				ScopeHelper::GetApendedString(Scope, ClassConstructorfunc);

				auto ConstructorSymbols = GetSymbolsWithName(Scope, SymbolType::Any);


				for (auto& Item2 : ConstructorSymbols)
				{
					FuncInfo* Info = Item2->Get_Info<FuncInfo>();
					bool PushThisPar = Info->IsObjectCall();

					size_t parcount = PushThisPar ? Pars._Nodes.size() + 1 : Pars._Nodes.size();
					if (Info->Pars.size() == parcount) {
						Infer = Info->Pars;
						Inferautopushtis = PushThisPar;
					}
				}
			}
			else if (Item->Type == SymbolType::Enum_Field)
			{
				String EnumClassFullName = ScopedName;
				ScopeHelper::ReMoveScope(EnumClassFullName);

				auto EnumSymbolop = Symbol_GetSymbol(EnumClassFullName, SymbolType::Enum);
				if (EnumSymbolop)
				{
					auto EnumSymbol = EnumSymbolop.value();
					if (EnumSymbol->Type == SymbolType::Enum)
					{
						const EnumInfo* Enuminfo = EnumSymbol->Get_Info<EnumInfo>();
						if (Enuminfo->VariantData.has_value())
						{
							auto FeildIndex = Enuminfo->GetFieldIndex(ScopeHelper::GetNameFromFullName(ScopedName));
							if (FeildIndex.has_value())
							{
								auto& VariantInfo = Enuminfo->VariantData.value().Variants[FeildIndex.value()];
								Infer.reserve(VariantInfo.Types.size());
								for (auto& Item : VariantInfo.Types)
								{
									Infer.push_back({ false,Item });
								} 
							}	
						}
					}

				}
			}
			else if (Item->Type == SymbolType::Generic_Tag)
			{
				TagInfo* V = Item->Get_Info<TagInfo>();
				Symbol_Update_ClassSym_ToFixedTypes(Item);
				String Scope = V->FullName;
				ScopeHelper::GetApendedString(Scope, GenericTestStr);
				ScopeHelper::GetApendedString(Scope, ClassConstructorfunc);
				
				auto ConstructorSymbols = GetSymbolsWithName(Scope, SymbolType::Any);


				for (auto& Item2 : ConstructorSymbols)
				{
					FuncInfo* Info = Item2->Get_Info<FuncInfo>();
					bool PushThisPar = Info->IsObjectCall();

					size_t parcount = PushThisPar ? Pars._Nodes.size() + 1 : Pars._Nodes.size();
					if (Info->Pars.size() == parcount) {
						Infer = Info->Pars;
						Inferautopushtis = PushThisPar;
					}
				}
			}
		}
	}

	if (_ThisTypeIsNotNull)
	{
		ValueTypes.push_back({ false,_ThisType });
	}

	TypeSymbol NullSymbol;
	NullSymbol.SetType(TypesEnum::Any);

	bool HasOutPar = false;
	for (size_t i = 0; i < Pars._Nodes.size(); i++)
	{
		auto& Item = Pars._Nodes[i];
		auto& ValueItem = ValueTypes.emplace_back();

		TypeSymbol InferedType = NullSymbol;

		{
			bool OneOrOtherButNotBoth = false;
			if (Inferautopushtis)
			{
				if (_ThisTypeIsNotNull == false) {
					OneOrOtherButNotBoth = true;
				}
			}
			else
				if (_ThisTypeIsNotNull)
				{
					if (Inferautopushtis == false) {
						OneOrOtherButNotBoth = true;
					}
				}
			size_t newindex = OneOrOtherButNotBoth ? i + 1 : i;

			if (newindex < Infer.size())
			{
				InferedType = Infer[newindex].Type;
			}
		}


		_LookingForTypes.push(InferedType);


		if (Item->Get_Type() == NodeType::OutExpression)
		{
			const OutExpression* outEx = OutExpression::As(Item.get());
			HasOutPar = true;
			ValueItem.IsOutPar = true;
			ValueItem.Type = Type_ConvertAndValidateType(outEx->_Type, NodeSyb_t::Variable);
		}
		else
		{
			OnExpressionTypeNode(Item.get(), GetValueMode::Read);
			ValueItem.IsOutPar = false;
			ValueItem.Type = _LastExpressionType;
		}

		_LookingForTypes.pop();



	}


	//badtype
	{
		for (auto& Item : ValueTypes)
		{
			if (Item.IsOutPar == false && Item.Type.IsBadType())
			{
				Get_FuncInfo V;

				return V;//can't check because we are just testing.
			}
		}
	}

	{
		for (size_t i = 0; i < ValueTypes.size(); i++)
		{
			auto& Item = ValueTypes[i];
			auto syb = Symbol_GetSymbol(Item.Type);

			if (syb.has_value())
			{
				auto symbol = syb.value();

				if (symbol->Type == SymbolType::Type_Pack)
				{
					bool last = &Item == &ValueTypes.back();

					if (!last)
					{
						const Token* token = Name._ScopedName.back()._token;
	
						String msg;
						msg += "Argment " + std::to_string(i);
						msg += " is invaid because it is an Type_Pack but is not the last Argment";

						LogError(ErrorCodes::InValidType, msg,NeverNullptr(token));

						Get_FuncInfo V;
						return V;
					}
				}
			}
		}
	}

	//unmaped
	{
		for (auto& Item : ValueTypes)
		{
			if (Type_IsUnMapType(Item.Type))
			{

				Get_FuncInfo V;
				V.CantCheckBecauseIsUnMaped = true;

				return V;//can't check because we are just testing.
			}
		}



	}

	//Out-Par
	{
		if (HasOutPar)
		{
			bool IsControlFlow = false;

			for (int i = _NodeTypeStack.size() - 1; i >= 0; i--)
			{
				auto& Last = _NodeTypeStack[i];
				if (Last == NodeType::IfNode || Last == NodeType::WhileNode || Last == NodeType::DoNode
					|| Last == NodeType::RetStatementNode
					|| Last == NodeType::CompileTimeIfNode
					|| Last == NodeType::MatchArm
					)
				{
					IsControlFlow = true;
					break;
				}
				else if (_NodeTypeStack.back() == NodeType::CompileTimeIfNode)
				{
					IsControlFlow = true;
					break;
				}
			}

			if (!IsControlFlow)
			{
				auto Token = NeverNullptr(Name._ScopedName.back()._token);
				LogError_OutCanOnlyBeInControlFlow(Token);
				return { };
			}
		}
	}

	//Pack
	{
		bool IsPack = ValueTypes.size() && ValueTypes.front().Type._Type == TypesEnum::CustomType;

		if (IsPack)
		{
			if (auto Val = Symbol_GetSymbol(ValueTypes.front().Type).value_unchecked())
			{
				if (Val->Type == SymbolType::Type_Pack)
				{

					const TypePackInfo* Info = Val->Get_Info<TypePackInfo>();

					String TryingToCallFunc = ScopeHelper::GetNameFromFullName(ScopedName);


					if (TryingToCallFunc == "Count")
					{
						Systematic_BuiltInFunctions::Func F;
						F.RetType = TypeSymbol(TypesEnum::uIntPtr);

						auto ValEx = Eval_MakeEx(F.RetType);

						size_t Count = Info->List.size();
						if (_Settings->PtrSize == IntSizes::Int32)
						{
							Eval_Set_ObjectAs(ValEx, (UInt32)Count);
						}
						else
						{
							Eval_Set_ObjectAs(ValEx, (UInt64)Count);
						}

						F.EvalObject =Opt(std::move(ValEx.EvaluatedObject));



						Get_FuncInfo R;
						R.ThisPar = ThisParType;
						R._BuiltFunc = Opt(std::move(F));
						return R;
					}
					else if (TryingToCallFunc == "IsAllSameType")
					{
						Systematic_BuiltInFunctions::Func F;
						F.RetType = TypeSymbol(TypesEnum::Bool);

						auto ValEx = Eval_MakeEx(F.RetType);

						bool AllSameType = true;
						Optional<TypeSymbol> MainType;
						for (auto& Item : Info->List)
						{
							if (!MainType.has_value())
							{
								MainType = Opt(Item);
							}
							else
							{
								if (!Type_AreTheSame(MainType.value(), Item))
								{
									AllSameType = false;
									break;
								}

							}
						}

						Eval_Set_ObjectAs<bool>(ValEx, AllSameType);
						F.EvalObject = std::move(ValEx.EvaluatedObject);



						Get_FuncInfo R;
						R.ThisPar = ThisParType;
						R._BuiltFunc = Opt(std::move(F));
						return R;
					}
				}
			}
		}
	}
	
	//TypeInfo
	{
		bool IsTypeInfo = ValueTypes.size() && ValueTypes.front().Type.IsTypeInfo();

		if (IsTypeInfo)
		{
			bool AutoPassThis = Get_FuncInfo::AddOneToGetParNode(ThisParType);
			Vector< Systematic_BuiltInFunctions::FunctionPar> BuiltInPars;
			BuiltInPars.resize(ValueTypes.size());

			ExpressionNodeType _TepThisPar;
			ValueExpressionNode _TepThisValue;
			ReadVariableNode _TepThisReadNode;
			if (AutoPassThis)
			{
				_TepThisPar._Value.reset(&_TepThisValue);
				_TepThisValue._Value.reset(&_TepThisReadNode);
				_TepThisReadNode._VariableName._ScopedName = Name._ScopedName;

				_TepThisReadNode._VariableName._ScopedName.pop_back();
				_TepThisReadNode._VariableName._ScopedName.back()._operator = ScopedName::Operator_t::Null;
			}


			for (size_t i = 0; i < BuiltInPars.size(); i++)
			{
				auto& ItemFuncPar = BuiltInPars[i];
				auto& ValuePar = ValueTypes[i];

				ItemFuncPar.Type = ValuePar.Type;
				ItemFuncPar.IsOutPar = ValuePar.IsOutPar;
				const Node* ItemNode = nullptr;

				if (AutoPassThis && i == 0)
				{
					ItemNode = _TepThisPar.As();
				}
				else
				{
					ItemNode = Pars._Nodes[AutoPassThis ? i - 1 : i].get();
				}

				ItemFuncPar.ExpressionNode = ItemNode;
			}

			auto FuncData = Systematic_BuiltInFunctions::GetFunction(ScopedName, BuiltInPars, *this);

			if (AutoPassThis)
			{
				auto _ = _TepThisPar._Value.release();//On Stack.
				auto _1 = _TepThisValue._Value.release();//On Stack.
			}

			if (FuncData.has_value())
			{
				auto& FuncDataValue = FuncData.value();
				{//OutPars

					size_t OutParIndex = 0;

					for (size_t i = 0; i < BuiltInPars.size(); i++)
					{
						bool IsOutPar = BuiltInPars[i].IsOutPar;

						if (IsOutPar)
						{
							const auto& ItemNode = Pars._Nodes[AutoPassThis ? i - 1 : i];
							auto& FuncDataOutPar = FuncDataValue._OutPars[OutParIndex];

							OutExpression* Ex = OutExpression::As(ItemNode.get());
							if (FuncDataOutPar.EvalObject.has_value())
							{
								EvaluatedEx EvaluatedValue;
								EvaluatedValue.Type = FuncDataOutPar.Type;
								EvaluatedValue.EvaluatedObject = std::move(FuncDataOutPar.EvalObject.value());
								Eval_SetOutExpressionEval(Ex, EvaluatedValue);
							}
							else
							{
								Symbol_SetOutExpression(Ex, FuncDataOutPar.Type);
							}

							OutParIndex++;
						}
					}
					FuncDataValue._OutPars.clear();//Free it's unused. 
				}


				Get_FuncInfo R;
				R.ThisPar = ThisParType;
				R._BuiltFunc = Opt(std::move(FuncDataValue));



				return R;
			}
		}
		else
		{

		}
	}
	
	//compiler
	{
		if (Name._ScopedName.size() == 2)
		{
			auto& compilername = Name._ScopedName[0];
			auto compilertoken = compilername._token;
			if (compilertoken->Type == TokenType::KeyWord_compiler)
			{
				String_view functocall = Name._ScopedName[1]._token->Value._String;

				if (functocall == "IsBackendUCodeVM" && ValueTypes.size() == 0)
				{
					bool RetValue = _Settings->_BackEndInfo.IsUCodeVm();

					Systematic_BuiltInFunctions::Func F;
					F.RetType = TypeSymbol(TypesEnum::Bool);

					auto ValEx = Eval_MakeEx(F.RetType);
					Eval_Set_ObjectAs<bool>(ValEx, RetValue);
					F.EvalObject = std::move(ValEx.EvaluatedObject);
					Get_FuncInfo R;
					R.ThisPar = ThisParType;
					R._BuiltFunc = std::move(F);
					return R;
				}
				if (functocall == "IsBackendC11" && ValueTypes.size() == 0)
				{
					bool RetValue = _Settings->_BackEndInfo.IsC11();

					Systematic_BuiltInFunctions::Func F;
					F.RetType = TypeSymbol(TypesEnum::Bool);

					auto ValEx = Eval_MakeEx(F.RetType);
					Eval_Set_ObjectAs<bool>(ValEx, RetValue);
					F.EvalObject = std::move(ValEx.EvaluatedObject);
					Get_FuncInfo R;
					R.ThisPar = ThisParType;
					R._BuiltFunc = std::move(F);
					return R;
				}
				if (functocall == "IsBackendWasm" && ValueTypes.size() == 0)
				{
					bool RetValue = _Settings->_BackEndInfo.IsWasm();

					Systematic_BuiltInFunctions::Func F;
					F.RetType = TypeSymbol(TypesEnum::Bool);

					auto ValEx = Eval_MakeEx(F.RetType);
					Eval_Set_ObjectAs<bool>(ValEx, RetValue);
					F.EvalObject = Opt(std::move(ValEx.EvaluatedObject));
					Get_FuncInfo R;
					R.ThisPar = ThisParType;
					R._BuiltFunc = Opt(std::move(F));
					return R;
				}
				if (functocall == "IsBackendWindows" && ValueTypes.size() == 0)
				{
					bool RetValue = _Settings->_BackEndInfo.IsWindows();

					Systematic_BuiltInFunctions::Func F;
					F.RetType = TypeSymbol(TypesEnum::Bool);

					auto ValEx = Eval_MakeEx(F.RetType);
					Eval_Set_ObjectAs<bool>(ValEx, RetValue);
					F.EvalObject = Opt(std::move(ValEx.EvaluatedObject));
					Get_FuncInfo R;
					R.ThisPar = ThisParType;
					R._BuiltFunc = Opt(std::move(F));
					return R;
				}
				if (functocall == "IsBackendLinux" && ValueTypes.size() == 0)
				{
					bool RetValue = _Settings->_BackEndInfo.IsLinux();

					Systematic_BuiltInFunctions::Func F;
					F.RetType = TypeSymbol(TypesEnum::Bool);

					auto ValEx = Eval_MakeEx(F.RetType);
					Eval_Set_ObjectAs<bool>(ValEx, RetValue);
					F.EvalObject = Opt(std::move(ValEx.EvaluatedObject));
					Get_FuncInfo R;
					R.ThisPar = ThisParType;
					R._BuiltFunc = Opt(std::move(F));
					return R;
				}
				if (functocall == "IsBackendMacOS" && ValueTypes.size() == 0)
				{
					bool RetValue = _Settings->_BackEndInfo.IsMacOs();

					Systematic_BuiltInFunctions::Func F;
					F.RetType = TypeSymbol(TypesEnum::Bool);

					auto ValEx = Eval_MakeEx(F.RetType);
					Eval_Set_ObjectAs<bool>(ValEx, RetValue);
					F.EvalObject = Opt(std::move(ValEx.EvaluatedObject));
					Get_FuncInfo R;
					R.ThisPar = ThisParType;
					R._BuiltFunc = Opt(std::move(F));
					return R;
				}
				if (functocall == "IsCPU_X86" && ValueTypes.size() == 0)
				{
					bool RetValue = _Settings->_BackEndInfo.IsX86();

					Systematic_BuiltInFunctions::Func F;
					F.RetType = TypeSymbol(TypesEnum::Bool);

					auto ValEx = Eval_MakeEx(F.RetType);
					Eval_Set_ObjectAs<bool>(ValEx, RetValue);
					F.EvalObject = Opt(std::move(ValEx.EvaluatedObject));
					Get_FuncInfo R;
					R.ThisPar = ThisParType;
					R._BuiltFunc = Opt(std::move(F));
					return R;
				}
				if (functocall == "IsCPU_Arm" && ValueTypes.size() == 0)
				{
					bool RetValue = _Settings->_BackEndInfo.IsArm();

					Systematic_BuiltInFunctions::Func F;
					F.RetType = TypeSymbol(TypesEnum::Bool);

					auto ValEx = Eval_MakeEx(F.RetType);
					Eval_Set_ObjectAs<bool>(ValEx, RetValue);
					F.EvalObject = Opt(std::move(ValEx.EvaluatedObject));
					Get_FuncInfo R;
					R.ThisPar = ThisParType;
					R._BuiltFunc = Opt(std::move(F));
					return R;
				}
				if (functocall == "IsDebug" && ValueTypes.size() == 0)
				{
					bool RetValue =
						(OptimizationFlags_t)_Settings->_Flags & (OptimizationFlags_t)OptimizationFlags::Debug;


					Systematic_BuiltInFunctions::Func F;
					F.RetType = TypeSymbol(TypesEnum::Bool);

					auto ValEx = Eval_MakeEx(F.RetType);
					Eval_Set_ObjectAs<bool>(ValEx, RetValue);
					F.EvalObject = std::move(ValEx.EvaluatedObject);
					Get_FuncInfo R;
					R.ThisPar = ThisParType;
					R._BuiltFunc = std::move(F);
					return R;
				}

			}
		}
	}

	//

StartSymbolsLoop:

	Vector<Get_FuncInfo> OkFunctions;

	const UseGenericsNode* Generics = Name._ScopedName.back()._generic.get();

	if (Symbols.size() == 0 && Name._ScopedName.size() == 1)
	{
		auto look = Ret;
		if (auto Val = Symbol_GetSymbol(look).value_unchecked())
		{
			if (Val->Type == SymbolType::Enum)
			{
				String_view EnumFieldName = ScopedName;

				EnumInfo* Enuminfo = Val->Get_Info<EnumInfo>();
				auto FieldIndexOp = Enuminfo->GetFieldIndex(EnumFieldName);
				if (FieldIndexOp.has_value()) {
					size_t FieldIndex = FieldIndexOp.value();

					auto ScopeName = ScopeHelper::ApendedStrings(Val->FullName, EnumFieldName);
					auto FieldSym = Symbol_GetSymbol(ScopeName, SymbolType::Enum_Field).value();

					return Symbol_GetEnumVariantFunc(Val, FieldIndex, FieldSym, Pars, NeverNullptr(Name._ScopedName.back()._token), ValueTypes);
				}
			}
		}
	}

	for (auto& Item : Symbols)
	{

		if (Item->Type == SymbolType::Func)
		{
			if (Item->IsInvalid())
			{
				return {};
			}
			FuncInfo* Info = Item->Get_Info<FuncInfo>();
			Symbol_Update_FuncSym_ToFixedTypes(Item);

			IsCompatiblePar CMPPar;
			CMPPar.SetAsFuncInfo(Item);

			if (!Type_IsCompatible(CMPPar, ValueTypes, _ThisTypeIsNotNull, NeverNullptr(Name._ScopedName.back()._token)))
			{
				continue;
			}

			{
				r = Info;
				FuncSymbol = Item;
				T = SymbolType::FuncCall;
				OkFunctions.push_back({ ThisParType,r,FuncSymbol });
			}
		}
		else if (Item->Type == SymbolType::GenericFunc)
		{
			if (Item->IsInvalid())
			{
				return {};
			}
			FuncInfo* Info = Item->Get_Info<FuncInfo>();
			Symbol_Update_FuncSym_ToFixedTypes(Item);

			Vector<TypeSymbol> GenericInput;
			auto v = Type_FuncinferGenerics(GenericInput, ValueTypes, Generics, Item, _ThisTypeIsNotNull);

			if (v.has_value())
			{
				r = Info;
				FuncSymbol = Item;
				T = SymbolType::FuncCall;
				OkFunctions.push_back({ ThisParType,r,FuncSymbol });
			}
		}
		else if (Item->Type == SymbolType::Type_class)
		{
			ClassInfo* V = Item->Get_Info<ClassInfo>();
			Symbol_Update_ClassSym_ToFixedTypes(Item);
			String Scope = V->FullName;
			ScopeHelper::GetApendedString(Scope, ClassConstructorfunc);

			auto ConstructorSymbols = GetSymbolsWithName(Scope, SymbolType::Any);


			for (auto& Item2 : ConstructorSymbols)
			{
				if (Item2->Type == SymbolType::Func)
				{
					FuncInfo* Info = Item2->Get_Info<FuncInfo>();
					bool PushThisPar = Info->IsObjectCall();


					if (PushThisPar)
					{
						TypeSymbol V;
						V.SetType(Item->ID);
						V.SetAsAddress();
						V.SetAsMoved();
						ValueTypes.insert(ValueTypes.begin(), { false,V });
					}

					IsCompatiblePar CMPPar;
					CMPPar.SetAsFuncInfo(Item2);

					bool Compatible = Type_IsCompatible(CMPPar, ValueTypes, _ThisTypeIsNotNull, NeverNullptr(Name._ScopedName.back()._token));

					if (PushThisPar)
					{
						ValueTypes.erase(ValueTypes.begin());
					}

					if (!Compatible)
					{
						continue;
					}

					{
						r = Info;
						FuncSymbol = Item2;
						T = SymbolType::FuncCall;
						OkFunctions.push_back({ PushThisPar ? Get_FuncInfo::ThisPar_t::OnIRlocationStack : ThisParType,r,FuncSymbol });
					}
				}
				else if (Item2->Type == SymbolType::GenericFunc)
				{
					FuncInfo* Info = Item2->Get_Info<FuncInfo>();
					bool PushThisPar = Info->IsObjectCall();


					if (PushThisPar)
					{
						TypeSymbol V;
						V.SetType(Item->ID);
						V.SetAsAddress();
						V.SetAsMoved();
						ValueTypes.insert(ValueTypes.begin(), { false,V });
					}


					Vector<TypeSymbol> GenericInput;
					auto v = Type_FuncinferGenerics(GenericInput, ValueTypes, Generics, Item2, _ThisTypeIsNotNull);

					if (v.has_value())
					{
						auto& val = v.value();
						if (val.has_value())
						{
							return val.value();
						}
						else
						{
							auto FuncSym = NeverNullptr(Item2);
							String NewName = Generic_SymbolGenericFullName(FuncSym, GenericInput);
							auto FuncIsMade = Symbol_GetSymbol(NewName, SymbolType::Func);


							if (FuncIsMade)
							{
								FuncSym = FuncIsMade.value();
							}


							{
								r = FuncSym->Get_Info<FuncInfo>();
								FuncSymbol = FuncSym.value();
								T = SymbolType::FuncCall;
								OkFunctions.push_back({ PushThisPar ? Get_FuncInfo::ThisPar_t::OnIRlocationStack : ThisParType,r,FuncSymbol });
							}

						}
					}

					if (PushThisPar)
					{
						ValueTypes.erase(ValueTypes.begin());
					}

				}
			}
		}
		else if (Item->Type == SymbolType::Generic_class)
		{
			Symbol_Update_ClassSym_ToFixedTypes(Item);
			ClassInfo* V = Item->Get_Info<ClassInfo>();

			const ClassNode& node = *Item->Get_NodeInfo<ClassNode>();

			auto classsybOp = Generic_InstantiateOrFindGeneric_Class(
				NeverNullptr(Name._ScopedName.front()._token),
				Item,
				node._generic, V->_GenericData, *Generics);
			if (classsybOp.has_value())
			{
				auto classsyb = classsybOp.value();
				String Scope = classsyb->FullName;
				ScopeHelper::GetApendedString(Scope, ClassConstructorfunc);

				auto ConstructorSymbols = GetSymbolsWithName(Scope, SymbolType::Any);


				for (auto& Item2 : ConstructorSymbols)
				{
					if (Item2->Type == SymbolType::Func)
					{
						FuncInfo* Info = Item2->Get_Info<FuncInfo>();
						bool PushThisPar = Info->IsObjectCall();


						if (PushThisPar)
						{
							TypeSymbol V;
							V.SetType(classsyb->ID);
							V.SetAsAddress();
							ValueTypes.insert(ValueTypes.begin(), { false,V });
						}

						IsCompatiblePar CMPPar;
						CMPPar.SetAsFuncInfo(Item2);

						bool Compatible = Type_IsCompatible(CMPPar, ValueTypes, _ThisTypeIsNotNull, NeverNullptr(Name._ScopedName.back()._token));

						if (PushThisPar)
						{
							ValueTypes.erase(ValueTypes.begin());
						}

						if (!Compatible)
						{
							continue;
						}

						{
							r = Info;
							FuncSymbol = Item2;
							T = SymbolType::FuncCall;
							OkFunctions.push_back({ PushThisPar ? Get_FuncInfo::ThisPar_t::OnIRlocationStack : ThisParType,r,FuncSymbol });
						}
					}


				}
			}
		}
		else if (Symbol_IsVarableType(Item->Type))
		{
			Symbol* Type = Symbol_GetSymbol(Item->VarType).value_unchecked();
			if (Type)
			{

				if (Type->Type == SymbolType::Func_ptr || Type->Type == SymbolType::Hard_Func_ptr)
				{
					FuncPtrInfo* Info = Type->Get_Info<FuncPtrInfo>();//must be the same as Item->Type == SymbolType::Func

					IsCompatiblePar CMPPar;
					CMPPar.SetAsFuncPtrInfo(Type);

					if (!Type_IsCompatible(CMPPar, ValueTypes, _ThisTypeIsNotNull, NeverNullptr(Name._ScopedName.back()._token)))
					{
						continue;
					}

					{
						r = (FuncInfo*)Info;
						FuncSymbol = Item;
						T = SymbolType::FuncCall;
						OkFunctions.push_back({ ThisParType,r,FuncSymbol });
					}
				}
				else if (Type->Type == SymbolType::Type_class)
				{
					String Scope = Type->FullName;
					ScopeHelper::GetApendedString(Scope, Overload_Invoke_Func);
					auto ConstructorSymbols = GetSymbolsWithName(Scope, SymbolType::Any);


					for (auto& Item2 : ConstructorSymbols)
					{
						if (Item2->Type == SymbolType::Func)
						{
							FuncInfo* Info = Item2->Get_Info<FuncInfo>();
							bool PushThisPar = Info->IsObjectCall();


							if (PushThisPar)
							{
								TypeSymbol V;
								V.SetType(Type->ID);
								V.SetAsAddress();
								ValueTypes.insert(ValueTypes.begin(), { false,V });
							}

							IsCompatiblePar CMPPar;
							CMPPar.SetAsFuncInfo(Item2);

							bool Compatible = Type_IsCompatible(CMPPar, ValueTypes, _ThisTypeIsNotNull, NeverNullptr(Name._ScopedName.back()._token));

							if (PushThisPar)
							{
								ValueTypes.erase(ValueTypes.begin());
							}

							if (!Compatible)
							{
								continue;
							}

							{
								r = Info;
								FuncSymbol = Item2;
								T = SymbolType::FuncCall;
								OkFunctions.push_back({ Get_FuncInfo::ThisPar_t::FullScopedName,r,FuncSymbol });
							}
						}
					}
				}
			}
		}
		else if (Item->Type == SymbolType::Enum_Field)
		{
			String EnumClassFullName = ScopedName;
			ScopeHelper::ReMoveScope(EnumClassFullName);

			auto EnumSymbolop = Symbol_GetSymbol(EnumClassFullName, SymbolType::Enum);
			if (EnumSymbolop)
			{
				auto EnumSymbol = EnumSymbolop.value();
				if (EnumSymbol->Type == SymbolType::Enum)
				{
					EnumInfo* Enuminfo = EnumSymbol->Get_Info<EnumInfo>();
					if (Enuminfo->VariantData.has_value())
					{
						auto FieldIndexOp = Enuminfo->GetFieldIndex(ScopeHelper::GetNameFromFullName(Item->FullName));
						if (FieldIndexOp.has_value()) {
							size_t FieldIndex = FieldIndexOp.value();

							return Symbol_GetEnumVariantFunc(EnumSymbol, FieldIndex, Item, Pars, NeverNullptr(Name._ScopedName.back()._token), ValueTypes);
						}
					}
				}
			}
		}
		else if (Item->Type == SymbolType::Tag_class)
		{
			TagInfo* V = Item->Get_Info<TagInfo>();
			Symbol_Update_ClassSym_ToFixedTypes(Item);
			String Scope = V->FullName;
			ScopeHelper::GetApendedString(Scope, ClassConstructorfunc);

			auto ConstructorSymbols = GetSymbolsWithName(Scope, SymbolType::Any);


			for (auto& Item2 : ConstructorSymbols)
			{
				if (Item2->Type == SymbolType::Func)
				{
					FuncInfo* Info = Item2->Get_Info<FuncInfo>();
					bool PushThisPar = Info->IsObjectCall();


					if (PushThisPar)
					{
						TypeSymbol V;
						V.SetType(Item->ID);
						V.SetAsAddress();
						V.SetAsMoved();
						ValueTypes.insert(ValueTypes.begin(), { false,V });
					}

					IsCompatiblePar CMPPar;
					CMPPar.SetAsFuncInfo(Item2);

					bool Compatible = Type_IsCompatible(CMPPar, ValueTypes, _ThisTypeIsNotNull, NeverNullptr(Name._ScopedName.back()._token));

					if (PushThisPar)
					{
						ValueTypes.erase(ValueTypes.begin());
					}

					if (!Compatible)
					{
						continue;
					}

					{
						r = Info;
						FuncSymbol = Item2;
						T = SymbolType::FuncCall;
						OkFunctions.push_back({ PushThisPar ? Get_FuncInfo::ThisPar_t::OnIRlocationStack : ThisParType,r,FuncSymbol });
					}
				}
				else if (Item2->Type == SymbolType::GenericFunc)
				{
					FuncInfo* Info = Item2->Get_Info<FuncInfo>();
					bool PushThisPar = Info->IsObjectCall();


					if (PushThisPar)
					{
						TypeSymbol V;
						V.SetType(Item->ID);
						V.SetAsAddress();
						V.SetAsMoved();
						ValueTypes.insert(ValueTypes.begin(), { false,V });
					}


					Vector<TypeSymbol> GenericInput;
					auto v = Type_FuncinferGenerics(GenericInput, ValueTypes, Generics, Item2, _ThisTypeIsNotNull);

					if (v.has_value())
					{
						auto& val = v.value();
						if (val.has_value())
						{
							return val.value();
						}
						else
						{
							auto FuncSym = NeverNullptr(Item2);
							String NewName = Generic_SymbolGenericFullName(FuncSym, GenericInput);
							auto FuncIsMade = Symbol_GetSymbol(NewName, SymbolType::Func);



							if (!FuncIsMade)
							{

								{
									if (CheckForGenericInputIsConstantExpression(Info, GenericInput))
									{
										continue;
									}
								}
								auto Pointer = std::make_unique<Vector<TypeSymbol>>(std::move(GenericInput));
								//pointer must be unique so it can't be on the stack

								Generic_GenericFuncInstantiate(FuncSym, *Pointer);
								_TepFuncs.push_back({ std::move(Pointer) });//keep pointer 
								FuncSym = Symbol_GetSymbol(NewName, SymbolType::Func).value();
							}
							else
							{
								FuncSym = FuncIsMade.value();
							}


							{
								r = FuncSym->Get_Info<FuncInfo>();
								FuncSymbol = FuncSym.value();
								T = SymbolType::FuncCall;
								OkFunctions.push_back({ PushThisPar ? Get_FuncInfo::ThisPar_t::OnIRlocationStack : ThisParType,r,FuncSymbol });
							}

						}
					}

					if (PushThisPar)
					{
						ValueTypes.erase(ValueTypes.begin());
					}

				}
			}
		}
		else if (Item->Type == SymbolType::Generic_Tag)
		{
			Symbol_Update_ClassSym_ToFixedTypes(Item);
			TagInfo* V = Item->Get_Info<TagInfo>();

			const TagTypeNode& node = *Item->Get_NodeInfo<TagTypeNode>();
			
			NullablePtr<Symbol>  classsymop;
			/*
			auto classsybOp = Generic_InstantiateOrFindGeneric_Class(
				NeverNullptr(Name._ScopedName.front()._token),
				Item,
				node._generic, V->_GenericData, *Generics);

			if (classsybOp.has_value())
			{
				auto classsyb = classsybOp.value();
				String Scope = classsyb->FullName;
				ScopeHelper::GetApendedString(Scope, ClassConstructorfunc);

				auto ConstructorSymbols = GetSymbolsWithName(Scope, SymbolType::Any);


				for (auto& Item2 : ConstructorSymbols)
				{
					if (Item2->Type == SymbolType::Func)
					{
						FuncInfo* Info = Item2->Get_Info<FuncInfo>();
						bool PushThisPar = Info->IsObjectCall();
					}
				}
			}
			*/
			{
				String Scope = V->FullName;
				ScopeHelper::GetApendedString(Scope, GenericTestStr);
				ScopeHelper::GetApendedString(Scope, ClassConstructorfunc);

				auto ConstructorSymbols = GetSymbolsWithName(Scope, SymbolType::Any);
				for (auto& FuncItem : ConstructorSymbols)
				{
					if (FuncItem->Type == SymbolType::Func)
					{
						const FuncInfo* finfo = FuncItem->Get_Info<FuncInfo>();
						const FuncNode* fnode = FuncItem->Get_NodeInfo<FuncNode>();

						Vector<Variant<TypeSymbol, EvaluatedEx>> genericinput;
						genericinput.resize(V->_GenericData.GetMinimumCount());

						for (size_t i = 1; i < finfo->Pars.size(); i++)
						{
							auto& Par = finfo->Pars[i];
							auto& ParNode = fnode->_Signature._Parameters._Parameters[i];
							auto& InputType = ValueTypes[i - 1];

							Optional<size_t> GenericExIndex;

							{
								for (auto& Item : ParNode._Type._name._ScopedName)
								{
									if (ParNode._Type._node.get())
									{
										auto n = ExpressionNodeType::As(ParNode._Type._node.get());
										if (n->_Value->Get_Type() == NodeType::ValueExpressionNode)
										{
											auto vn = ValueExpressionNode::As(n->_Value.get());

											if (vn->_Value->Get_Type() == NodeType::ReadVariableNode)
											{
												auto readvarnode = ReadVariableNode::As(vn->_Value.get());

												String varname;
												readvarnode->_VariableName.GetScopedName(varname);


												for (size_t i = 0; i < node._generic._Values.size(); i++)
												{
													auto& g = node._generic._Values[i];

													if (varname == g.AsStringView())
													{
														GenericExIndex = i;
														break;
													}
												}
											}
										}
										if (GenericExIndex.has_value())
										{
											break;
										}
									}
								}
							}


							if (GenericExIndex.has_value())
							{
								auto gindex = GenericExIndex.value();
								auto insym = Symbol_GetSymbol(InputType.Type);
								auto parsym = Symbol_GetSymbol(Par.Type);

								if (insym.has_value() && parsym.has_value())
								{
									auto in = insym.value();
									auto pa = parsym.value();

									if (in->Type == pa->Type && in->Type == SymbolType::Type_StaticArray)
									{
										StaticArrayInfo* ininfo = in->Get_Info<StaticArrayInfo>();

										auto v = Eval_MakeEx(TypesEnum::uIntPtr);
										if (_Settings->PtrSize == IntSizes::Int32)
										{
											Eval_Set_ObjectAs(v, (UInt32)ininfo->Count);
										}
										else
										{
											Eval_Set_ObjectAs(v, (UInt64)ininfo->Count);
										}

										genericinput[gindex] = std::move(v);
									}
								}
							}
						}


						bool isgenericok = false;

						UCodeLangAssert(!V->_GenericData.IsPack());

						if (genericinput.size() == V->_GenericData._Genericlist.size())
						{
							isgenericok = true;
							for (size_t i = 0; i < genericinput.size(); i++)
							{
								auto& ginput = genericinput[i];
								auto& glist = V->_GenericData._Genericlist[i];

								bool isginputEx = ginput.Is<EvaluatedEx>();
								bool isglistEx = glist.IsConstantExpression();

								if (isginputEx != isglistEx)
								{
									isgenericok = false;
									break;
								}
							}
						}


						if (isgenericok)
						{
							Vector<TypeSymbol> in;
							in.resize(genericinput.size());

							for (size_t i = 0; i < genericinput.size(); i++)
							{
								auto& input = genericinput[i];
								TypeSymbol t;

								if (auto tep = input.Get_If<TypeSymbol>())
								{
									t = *tep;
								}
								else
								{
									auto& v = input.Get< EvaluatedEx>();

									auto& sym = Symbol_AddSymbol(SymbolType::ConstantExpression, "", "", AccessModifierType::Private);
									_Table.AddSymbolID(sym, Symbol_GetSymbolID(&sym));

									ConstantExpressionInfo* info = new ConstantExpressionInfo();

									info->Ex = std::move(v.EvaluatedObject);
									sym.VarType = v.Type;

									sym.Info.reset(info);

									t = TypeSymbol(sym.ID);
								}

								in[i] = t;
							}

							String NewName = Generic_SymbolGenericFullName(Item, in);
							auto FuncIsMade = Symbol_GetSymbol(NewName, SymbolType::Tag_class);

							if (!FuncIsMade)
							{
								Generic_TypeInstantiate_Tag(Item, in);
							}

							auto newtagsym = FuncIsMade ? FuncIsMade : Symbol_GetSymbol(NewName, SymbolType::Tag_class);


							classsymop = newtagsym;
						}
					}
				}
			}

			if (classsymop.has_value())
			{
				auto classsyb = classsymop.value();
				String Scope = classsyb->FullName;
				ScopeHelper::GetApendedString(Scope, ClassConstructorfunc);

				auto ConstructorSymbols = GetSymbolsWithName(Scope, SymbolType::Any);


				for (auto& Item2 : ConstructorSymbols)
				{
					if (Item2->Type == SymbolType::Func)
					{
						if (Item2->Type == SymbolType::Func)
						{
							FuncInfo* Info = Item2->Get_Info<FuncInfo>();
							bool PushThisPar = Info->IsObjectCall();


							if (PushThisPar)
							{
								TypeSymbol V;
								V.SetType(classsyb->ID);
								V.SetAsAddress();
								ValueTypes.insert(ValueTypes.begin(), { false,V });
							}

							IsCompatiblePar CMPPar;
							CMPPar.SetAsFuncInfo(Item2);

							bool Compatible = Type_IsCompatible(CMPPar, ValueTypes, _ThisTypeIsNotNull, NeverNullptr(Name._ScopedName.back()._token));

							if (PushThisPar)
							{
								ValueTypes.erase(ValueTypes.begin());
							}

							if (!Compatible)
							{
								continue;
							}

							{
								r = Info;
								FuncSymbol = Item2;
								T = SymbolType::FuncCall;
								OkFunctions.push_back({ PushThisPar ? Get_FuncInfo::ThisPar_t::OnIRlocationStack : ThisParType,r,FuncSymbol });
							}
						}
					}
				}
			}


		}
	ContinueOutloop:continue;
	}
	if (OkFunctions.size() == 0)
	{
		bool MayBeAutoThisFuncCall = Name._ScopedName.size() == 1 && Context_IsInThisFuncCall();


		if (ThisParType == Get_FuncInfo::ThisPar_t::NoThisPar && MayBeAutoThisFuncCall && AutoThisCall == false)
		{
			AutoThisCall = true;

			ValueTypes.insert(ValueTypes.begin(), { false ,*_FuncStack.back().Pointer->GetObjectForCall() });
			ThisParType = Get_FuncInfo::ThisPar_t::AutoPushThis;



			//the code here should be removed.
			if (Type_IsUnMapType(ValueTypes.front().Type))
			{

				Get_FuncInfo V;
				V.CantCheckBecauseIsUnMaped = true;

				return V;//can't check because we are just testing.
			}

			goto StartSymbolsLoop;
		}
		else
		{
			if (ThisParType == Get_FuncInfo::ThisPar_t::NoThisPar && MayBeAutoThisFuncCall) {
				ValueTypes.erase(ValueTypes.begin());
			}

			_LastExpressionType = TypeSymbol(TypesEnum::Null);
			LogError_CantFindFuncError(NeverNullptr(Name._ScopedName.back()._token), ScopedName, {}, ValueTypes, RetType);
			return { };
		}
	}
	else
	{




		Optional<int> MinScore;
		Get_FuncInfo* Ret = nullptr;
		for (auto& Item : OkFunctions)
		{
			Vector<ParInfo> _TepGeneric;
			IsCompatiblePar CMPPar;
			if (Item.SymFunc->Type == SymbolType::Func)
			{
				CMPPar.SetAsFuncInfo(Item.SymFunc);
			}
			else if (Symbol_IsVarableType(Item.SymFunc->Type))
			{
				Symbol* Type = Symbol_GetSymbol(Item.SymFunc->VarType).value_unchecked();
				if (Type && (Type->Type == SymbolType::Func_ptr || Type->Type == SymbolType::Hard_Func_ptr))
				{
					CMPPar.SetAsFuncPtrInfo(Type);
				}
				else
				{
					UCodeLangUnreachable();
				}
			}
			else if (Item.SymFunc->Type == SymbolType::GenericFunc)
			{
				CMPPar.SetAsFuncInfo(Item.SymFunc);

				//Get Par type like it was instantiated
				_TepGeneric.resize(Item.Func->Pars.size());

				Vector<TypeSymbol> GenericInput;
				auto v = Type_FuncinferGenerics(GenericInput, ValueTypes, Generics, Item.SymFunc, _ThisTypeIsNotNull);

				auto& Generic = Item.Func->_GenericData;

				if (!Generic.IsPack())
				{
					//Next code does not work when is Generic Is Pack.



					const FuncSignatureNode* Signature = &Item.SymFunc->Get_NodeInfo<FuncNode>()->_Signature;
					
					if (GenericInput.size() != Signature->_generic._Values.size())
					{
						continue;
					}
					_Table.AddScope(std::to_string((uintptr_t)Item.Func));
					
					for (size_t i = 0; i < Signature->_generic._Values.size(); i++)
					{
						auto& Item = Signature->_generic._Values[i];

						auto& Syb = Symbol_AddSymbol(SymbolType::Type_alias, String(Item.token->Value._String),
							_Table._Scope.ApendedStrings(_Table._Scope.ThisScope, Item.token->Value._String),
							AccessModifierType::Public);

						Syb.VarType = GenericInput[i];
					}
					for (size_t i = 0; i < Signature->_Parameters._Parameters.size(); i++)
					{
						auto& Item = Signature->_Parameters._Parameters[i];
						Type_Convert(Item._Type, _TepGeneric[i].Type);
					}

					_Table.RemoveScope();
					CMPPar.Pars = &_TepGeneric;
				}
			}
			else
			{
				UCodeLangUnreachable();
			}


			int Score = Type_GetCompatibleScore(CMPPar, ValueTypes);
			if (!MinScore.has_value() || Score > MinScore.value())
			{
				MinScore = Score;
				Ret = &Item;
			}
		}
		if (Ret == nullptr) {
			UCodeLangUnreachable();
		}

		auto RValue = *Ret;

		if (RValue.SymFunc->Type == SymbolType::GenericFunc)
		{
			Vector<TypeSymbol> GenericInput;
			auto Info  = RValue.SymFunc->Get_Info<FuncInfo>();

			bool PushThisPar = Info->IsObjectCall();


			if (PushThisPar != _ThisTypeIsNotNull)
			{
				TypeSymbol V;
				V.SetType(Info->Pars.begin()->Type._CustomTypeSymbol);
				V.SetAsAddress();
				V.SetAsMoved();
				ValueTypes.insert(ValueTypes.begin(), { false,V });
			}
			auto v = Type_FuncinferGenerics(GenericInput, ValueTypes, Generics, RValue.SymFunc, _ThisTypeIsNotNull);

			if (PushThisPar != _ThisTypeIsNotNull)
			{
				ValueTypes.erase(ValueTypes.begin() + 0);
			}

			if (v.has_value())
			{
				auto& val = v.value();
				if (val.has_value())
				{
					return val.value();
				}
				else
				{
					auto FuncSym = NeverNullptr(RValue.SymFunc);
					String NewName = Generic_SymbolGenericFullName(FuncSym, GenericInput);
					auto FuncIsMade = Symbol_GetSymbol(NewName, SymbolType::Func);



					if (!FuncIsMade)
					{
						bool funcisbad = false;
						if (Info->_GenericData.IsPack())
						{

							auto& GInfo = Info->_GenericData._Genericlist.back();

							if (GInfo.BaseOrRule.has_value())
							{
								bool islastparpack = false;

								if (Info->Pars.size())
								{
									auto& last = Info->Pars.back();
									auto symop = Symbol_GetSymbol(last.Type);
									if (symop.has_value())
									{
										auto& sym = symop.value();

										if (GInfo.SybID == sym->ID)
										{
											islastparpack = true;
										}

									}
								}

								if (islastparpack)
								{
									auto& rule = GInfo.BaseOrRule.value();

									if (rule.Is<TypeSymbol>())
									{
										auto& val = rule.Get<TypeSymbol>();

										size_t startindex = Info->_GenericData._Genericlist.size() - 1;

										bool isthesame = true;
										for (size_t i = startindex; i < GenericInput.size(); i++)
										{
											auto& Input = GenericInput[i];

											if (!Type_AreTheSame(Input, val))
											{
												isthesame = false;
												break;
											}
										}

										if (!isthesame)
										{
											funcisbad = true;
											String msg = "Type_Pack on '" + FuncSym->FullName + "' Requres all types to be '";
											msg += ToString(val);
											msg += "'";
											msg += " but the types were ";

											bool isfirst = true;
											for (size_t i = startindex; i < GenericInput.size(); i++)
											{
												auto& Input = GenericInput[i];

												if (!Type_AreTheSame(Input, val))
												{
													if (!isfirst)
													{
														msg += ",";
													}

													msg += ToString(Input);
													msg += " at Generic Index ";
													msg += std::to_string(i);
													isfirst = false;
												}
											}

											LogError(ErrorCodes::InValidType, msg, NeverNullptr(Name._ScopedName.back()._token));
											funcisbad = true;
										}
									}
									else
									{
										UCodeLangUnreachable();
									}
								}
							}
						}

						if (funcisbad)
						{
							return {};
						}

						{
							UCodeLangAssert(CheckForGenericInputIsConstantExpression(Info, GenericInput) == false);
						}
						auto Pointer = std::make_unique<Vector<TypeSymbol>>(std::move(GenericInput));
						//pointer must be unique so it can't be on the stack

						Generic_GenericFuncInstantiate(FuncSym, *Pointer);

						_TepFuncs.push_back({ std::move(Pointer) });//keep pointer 


						FuncSym = Symbol_GetSymbol(NewName, SymbolType::Func).value();
					}
					else
					{
						FuncSym = FuncIsMade.value();
					}

					{
						r = FuncSym->Get_Info<FuncInfo>();
						FuncSymbol = FuncSym.value();
						RValue.SymFunc = FuncSymbol;
						RValue.Func = r;
					}

				}
			}
		}

		Symbol_AccessCheck(RValue.SymFunc, NeverNullptr(Name._ScopedName.back()._token));


		bool AutoPassThis = Get_FuncInfo::AddOneToGetParNode(ThisParType);
		for (size_t i = 0; i < ValueTypes.size(); i++)
		{
			auto& Item = ValueTypes[i];
			if (AutoPassThis && i == 0) { continue; }

			const auto& ItemNode = Pars._Nodes[AutoPassThis ? i - 1 : i];
			if (Item.IsOutPar)
			{
				const OutExpression* Item = OutExpression::As(ItemNode.get());

				TypeSymbol TypeSyb;

				if (RValue.SymFunc->Type == SymbolType::Func)
				{
					TypeSyb = RValue.SymFunc->Get_Info<FuncInfo>()->Pars[i].Type;
				}
				else if (RValue.SymFunc->Type == SymbolType::Func_ptr)
				{
					TypeSyb = RValue.SymFunc->Get_Info<FuncInfo>()->Pars[i].Type;
				}
				else
				{
					UCodeLangUnreachable();
				}

				Symbol_SetOutExpression(Item, TypeSyb);
			}
		}

		if (!IsInUnSafeBlock())
		{
			auto token = NeverNullptr(Name._ScopedName.back()._token);
			if (RValue.SymFunc->Type == SymbolType::Func)
			{
				auto funcinfo = RValue.SymFunc->Get_Info<FuncInfo>();

				if (funcinfo->IsUnsafe)
				{
					LogError(ErrorCodes::InValidType, "trying to call 'unsafe' function but in safe mode", token);
					return { };
				}
			}
		}


		return RValue;


	}
	return { };
}
Optional< Optional<SystematicAnalysis::Get_FuncInfo>> SystematicAnalysis::Type_FuncinferGenerics(Vector<TypeSymbol>& GenericInput, const Vector<ParInfo>& ValueTypes
	, const UseGenericsNode* Generics
	, Symbol* Item
	, bool _ThisTypeIsNotNull)
{
	FuncInfo* Info = Item->Get_Info<FuncInfo>();
	bool IsParPack = Info->_GenericData.IsPack();
	bool LastParIsPack = IsParPack && Info->Pars.back().Type._CustomTypeSymbol == Info->_GenericData._Genericlist.back().SybID;

	if (LastParIsPack && IsParPack)
	{
		bool parcountcheck = Info->Pars.size() - 1 >= ValueTypes.size();
	
		if (!(Info->Pars.size() == 1 && ValueTypes.size()))
		{
			parcountcheck = false;
		}
		if (LastParIsPack && parcountcheck)
		{
			return {};
		}
	}
	else
	{
		if (Info->Pars.size() != ValueTypes.size())
		{
			return {};
		}
	}

	Vector<bool> HasBenAdded;
	HasBenAdded.resize(Info->_GenericData._Genericlist.size());

	if (LastParIsPack)
	{
		if (Generics) {
			HasBenAdded.resize(Generics->_Values.size());
		}
	}
	else
	{
		HasBenAdded.resize(Info->_GenericData._Genericlist.size());
	}

	if (Generics) {
		for (size_t i = 0; i < Generics->_Values.size(); i++)
		{
			auto& Item = Generics->_Values[i];
			Type_Convert(Item, GenericInput.emplace_back());

			if (Type_IsUnMapType(GenericInput.back()))
			{
				Get_FuncInfo V;
				V.CantCheckBecauseIsUnMaped = true;
				return { V };//can't check because we are just testing.
			}

			HasBenAdded[i] = true;
		}
	}




	for (size_t i = 0; i < ValueTypes.size(); i++)
	{
		auto& ItemValueType = ValueTypes[i];
		if (_ThisTypeIsNotNull && i == 0) { continue; }

		bool Added = false;
		auto InputType = ItemValueType.Type;

		if (i < Info->Pars.size())
		{
			auto& Par = Info->Pars[i];
			for (size_t i2 = 0; i2 < Info->_GenericData._Genericlist.size(); i2++)
			{
				auto& V3 = Info->_GenericData._Genericlist[i2];
				bool addinput = false;
				if (V3.SybID == Par.Type._CustomTypeSymbol)//Func<T>[T Item] ,caller: Func(3)
				{
					addinput = true;
				}
				else
				{
					if (auto ItemSybOp = Symbol_GetSymbol(ItemValueType.Type))//Func<T>[Item<T> Item] ,caller:Item<int> V; Func(V)
					{
						if (Par.Type.IsBadType())//Type was not made because its generic.
						{
							auto ItemSyb = ItemSybOp.value();
							auto& Funcnode = *Item->Get_NodeInfo<FuncNode>();
							auto& FuncnodePars = Funcnode._Signature._Parameters;
							auto& FuncnodePartype = FuncnodePars._Parameters[i]._Type;


							auto GenericOp = Symbol_GetSymbol(FuncnodePartype._name._ScopedName.back()._token->Value._String, ItemSyb->Type);

							if (GenericOp && GenericOp.value()->Type == SymbolType::Generic_class)
							{


								if (ItemSyb->Type == SymbolType::Type_class)
								{
									auto& ItemGenericData = ItemSyb->Get_Info<ClassInfo>()->_GenericData;

									bool HasSameGenericHasThis = true;
									//TODO check if is the same Generic
									if (HasSameGenericHasThis)
									{
										auto& GenericOpGenericData = GenericOp.value()->Get_NodeInfo<ClassNode>()->_generic._Values[i2];

										auto name = GenericOpGenericData.token->Value._String;

										auto scopedname = ItemSyb->FullName;
										ScopeHelper::GetApendedString(scopedname, name);


										InputType = Symbol_GetSymbol(scopedname, SymbolType::Type_alias).value()->VarType;
										addinput = true;
									}
								}
							}
						}
					}
				}

				if (addinput)
				{
					if (i2 >= HasBenAdded.size())
					{
						GenericInput.push_back(InputType);
						Added = true;
						continue;
					}
					else if (HasBenAdded[i2] == false)
					{
						GenericInput.push_back(InputType);
						HasBenAdded[i2] = true;
						Added = true;
						continue;
					}
					break;
				}
			}
		}

		if (Added == false
			&& LastParIsPack
			&& i >= Info->Pars.size() - 1
			)
		{
			auto PackParsIndex = Info->Pars.size() - 1;
			auto PackAddedPar = i - PackParsIndex;

			auto PackGenericIndex = Info->_GenericData._Genericlist.size() - 1;
			auto PackAddedG = i - PackGenericIndex;

			//is logically incorrect

			bool CMPBool = true;
			if (CMPBool) {
				GenericInput.push_back(ItemValueType.Type);
			}
		}
	}

	bool cangenericinputbeused = false;

	bool isinputcountgood = false;
	
	if (IsParPack)
	{	
		isinputcountgood = true;
	}
	else
	{
		isinputcountgood = GenericInput.size() == Info->_GenericData._Genericlist.size();
	}

	if (isinputcountgood)
	{
		cangenericinputbeused = true;

		const FuncNode* fnode = Item->Get_NodeInfo<FuncNode>();

		const auto& fpars = fnode->_Signature._Parameters._Parameters;		
		const auto& fInput = GenericInput;
		const auto& fgenericnode = fnode->_Signature._generic;

		//We cant use Type_Convert because if Generic it will generate unneeded IR,Symbols.
		// if cangenericinputbeused becomes false
		// and have a way to roleback would be a headache
		// so we just lazy attempt Convert compare the possible Symbol name with the Par.

		//Lambda because it's never used any were else
		std::function<Optional<String>(const TypeNode&)> lazyattemp;

		
		lazyattemp = [this,&lazyattemp,&fInput,&fgenericnode,&fpars](const TypeNode& node) -> Optional<String>
		{
				if (node._name._ScopedName.back()._token->Type== TokenType::Name)
				{
					String fullname;
					node._name.GetScopedName(fullname);

					for (size_t i = 0; i < fgenericnode._Values.size(); i++)
					{
						auto& gnode = fgenericnode._Values[i];
						auto& fin = fInput[i];

						if (gnode.AsString() == fullname)
						{
							return ToString(fin);
						}
					}
					
					auto sym = Symbol_GetSymbol(fullname, SymbolType::Type);
					if (sym)
					{
						const auto typegenics = node._name._ScopedName.back()._generic.get();
						bool hasgenics = typegenics ?  typegenics->_Values.size() : 0;

						if (hasgenics)
						{
							fullname += "<";

							for (auto& Item : typegenics->_Values)
							{
								fullname += lazyattemp(Item).value_or("");

								if (&Item != &typegenics->_Values.back())
								{
									fullname += ",";
								}
							}


							fullname += ">";
						}
						return fullname;
					}
				}
				return {};
		};

		if (ValueTypes.size() != 0)
		{
			for (size_t i = 0; i < fpars.size(); i++)
			{
				auto& Item = fpars[i];
				auto& Par = ValueTypes[i].Type;
				auto sym = lazyattemp(Item._Type);

				if (sym.has_value())
				{
					auto symname = sym.value();
					auto parname = ToString(Par);

					if (symname != parname)
					{
						cangenericinputbeused = false;
						break;
					}
				}
			}
		}
	}

	if (cangenericinputbeused) {
		return { {} };
	}
	else
	{
		return {};
	}
}
SystematicAnalysis::Get_FuncInfo SystematicAnalysis::Symbol_GetEnumVariantFunc(NeverNullPtr<Symbol> EnumSyb, size_t FieldIndex, NeverNullPtr<Symbol> EnumFieldSyb, const ValueParametersNode& Pars, const NeverNullPtr<Token> Token, const Vector<ParInfo>& ValueTypes)
{
	const EnumInfo* Enuminfo = EnumSyb->Get_Info<EnumInfo>();
	auto& Field = Enuminfo->Fields[FieldIndex];
	auto& Field_Variant = Enuminfo->VariantData.value().Variants[FieldIndex];


	bool HasOut = false;
	for (size_t i = 0; i < Pars._Nodes.size(); i++)
	{
		auto& Item = Pars._Nodes[i];


		if (Item->Get_Type() == NodeType::OutExpression)
		{
			HasOut = true;
			break;
		}
	}


	if (HasOut)
	{

		if (Field_Variant.Types.size() + 1 != Pars._Nodes.size())
		{
			String FullName = Enuminfo->FullName;
			ScopeHelper::GetApendedString(FullName, Field.Name);
			LogError_CanIncorrectParCount(Token, FullName, Pars._Nodes.size(), Field_Variant.Types.size() + 1);

			_LastExpressionType = TypeSymbol(EnumSyb->ID);
			return {};
		}


		for (size_t i = 0; i < Pars._Nodes.size(); i++)
		{
			auto& Item = Pars._Nodes[i];


			if (i == 0)
			{
				auto& ExItemType = ValueTypes[i];

				TypeSymbol Vthis = EnumSyb->ID;
				if (!Type_CanBeImplicitConverted(ExItemType.Type, Vthis))
				{
					LogError_CantCastImplicitTypes(Token, ExItemType.Type, Vthis, true);
				}
			}
			else
			{
				auto& ItemVariant = Field_Variant.Types[i - 1];
				if (Item->Get_Type() != NodeType::OutExpression)
				{
					LogError_ParamterMustBeAnOutExpression(Token, i);
				}
				else
				{
					OutExpression* Ex = OutExpression::As(Item.get());


					auto& TypeToSet = ItemVariant;

					Symbol_SetOutExpression(Ex, TypeToSet);
				}

			}




		}
		Get_FuncInfo r;
		r.ThisPar = Get_FuncInfo::ThisPar_t::NoThisPar_GetValue_EnumVariant;
		r.SymFunc = EnumFieldSyb.value();
		r.Func = nullptr;

		_LastExpressionType = TypeSymbol(TypesEnum::Bool);
		return r;
	}
	else
	{
		if (Field_Variant.Types.size() == 1)
		{
			TypeSymbol VoidType(TypesEnum::Void);

			if (Type_AreTheSame(VoidType, Field_Variant.Types.front()))
			{
				if (Pars._Nodes.size() != 0)
				{
					String FullName = Enuminfo->FullName;
					ScopeHelper::GetApendedString(FullName, Field.Name);
					LogError_CanIncorrectParCount(Token, FullName, Pars._Nodes.size(), 0);

					_LastExpressionType = TypeSymbol(EnumSyb->ID);
					return {};
				}
				else
				{
					Get_FuncInfo r;
					r.ThisPar = Get_FuncInfo::ThisPar_t::NoThisPar;
					r.SymFunc = EnumFieldSyb.value();
					r.Func = nullptr;

					_LastExpressionType = TypeSymbol(EnumSyb->ID);
					return r;
				}
			}
		}

		if (Field_Variant.Types.size() != Pars._Nodes.size())
		{
			String FullName = Enuminfo->FullName;
			ScopeHelper::GetApendedString(FullName, Field.Name);
			LogError_CanIncorrectParCount(Token, FullName, Pars._Nodes.size(), Field_Variant.Types.size());

			_LastExpressionType = TypeSymbol(EnumSyb->ID);
			return {};
		}

		for (size_t i = 0; i < Field_Variant.Types.size(); i++)
		{
			auto& Item = Field_Variant.Types[i];
			auto& ExItemType = ValueTypes[i];

			if (!Type_CanBeImplicitConverted(ExItemType.Type, Item))
			{
				LogError_CantCastImplicitTypes(Token, ExItemType.Type, Item, true);
			}
		}

		Get_FuncInfo r;
		r.ThisPar = Get_FuncInfo::ThisPar_t::NoThisPar;
		r.SymFunc = EnumFieldSyb.value();
		r.Func = nullptr;

		_LastExpressionType = TypeSymbol(EnumSyb->ID);
		return r;
	}
}
void SystematicAnalysis::IR_Build_DestructorCall(const ObjectToDrop& Object)
{

	if (Type_IsPrimitive(Object.Type))
	{
		/*
		return;
		UAddress NewValue;
		UAddress ObjectSize;
		GetSize(Object.Type, ObjectSize);

		Byte* ValuePtr = (Byte*)&NewValue;

		size_t Length = ObjectSize < sizeof(NewValue) ? ObjectSize : sizeof(NewValue);
		for (size_t i = 0; i < Length; i++)
		{
			*ValuePtr = DebugGarbageByte;

			ValuePtr++;
		}

		#define Primitive_Destructor(X) \
		case sizeof(UInt##X):\
		{\
			_Builder.Build_Assign(IROperand::AsInt##X(*(UInt##X*)&NewValue));\
		}break;\

		switch (ObjectSize)
		{
			Primitive_Destructor(8)
			Primitive_Destructor(16)
			Primitive_Destructor(32)
			Primitive_Destructor(64)

		default:break;
		}

		auto Op = IROperand::AsLocation(_Builder.GetLastField());
		auto NewOp = IROperand::AsLocation(Object.Object);
		_Builder.Build_Assign(NewOp, Op);
		*/
	}
	else
	{

		if (Object.Type.IsAn(TypesEnum::CustomType))
		{
			int a = 0;

			TypeSymbol tep = Object.Type;

			if (tep.IsAddress())
			{
				tep._IsAddress = false;
			}


			String TypeDestructorFuncName = ToString(tep);
			ScopeHelper::GetApendedString(TypeDestructorFuncName, ClassDestructorFunc);

			Symbol* Sym = Symbol_GetSymbol(TypeDestructorFuncName, SymbolType::Func).value_unchecked();
			if (Sym)
			{
				FuncInfo* node = Sym->Get_Info<FuncInfo>();

				Get_FuncInfo FuncInfo;
				FuncInfo.Func = node;
				FuncInfo.SymFunc = Sym;


				//par

				switch (Object.DropType)
				{
				case ObjectToDropType::IRInstruction:
					FuncInfo.ThisPar = Get_FuncInfo::ThisPar_t::OnIRlocationStack;
					_IR_IRlocations.push({ Object._Object, false });
					break;
				case ObjectToDropType::IRInstructionNoMod:
					FuncInfo.ThisPar = Get_FuncInfo::ThisPar_t::OnIRlocationStackNonedef;
					_IR_IRlocations.push({ Object._Object, false });
					break;
				case ObjectToDropType::Operator:
					FuncInfo.ThisPar = Get_FuncInfo::ThisPar_t::OnIRlocationStackNonedef;

					

					IRInstruction* ir;
					switch (Object._Operator.Type)
					{
					case IROperatorType::IRInstruction:
						ir = _IR_LookingAtIRBlock->NewLoad(Object._Operator.Pointer);
						break;
					case IROperatorType::IRParameter:
						ir =  _IR_LookingAtIRBlock->NewLoad(Object._Operator.Parameter);
						break;
					default:
						UCodeLangUnreachable();
						break;
					}

					_IR_IRlocations.push({ ir, false });
					break;
				default:
					UCodeLangUnreachable();
					break;
				}

				//

				ValueParametersNode Vtemp;
				IR_Build_FuncCall(Object.Type, FuncInfo, Vtemp);


				if (_IR_LastExpressionField->Type == IRInstructionType::Call)
				{
					if (Object._Object != nullptr)
					{
						Optional<size_t> FieldIndex;

						for (size_t i = 0; i < _IR_LookingAtIRBlock->Instructions.size(); i++)
						{
							auto& Item = _IR_LookingAtIRBlock->Instructions[i];

							if (Item.get() == Object._Object)
							{
								FieldIndex = i;
								break;
							}
						}

						_IR_LastExpressionField->Type = IRInstructionType::CleanupFuncCall;
						_IR_LastExpressionField->B = IROperator(AnyInt64(FieldIndex.value()));
					}
				}

				_IR_IRlocations.pop();
			}
		}

	}
}
SystematicAnalysis::Get_FuncInfo SystematicAnalysis::Type_GetFunc(const TypeSymbol& Name, const ValueParametersNode& Pars)
{
	if (Name.IsBadType())
	{
		return {};
	}
	{
		auto SybOp = Symbol_GetSymbol(Name);
		if (SybOp)
		{
			auto Syb = SybOp.value();
			if (Syb->Type == SymbolType::Generic_class || Syb->Type == SymbolType::Generic_Trait)
			{

				//most likely Generic Testing
				Get_FuncInfo R;
				R.CantCheckBecauseIsUnMaped = true;
				return R;
			}
			if (Type_IsUnMapType(Name))
			{
				Get_FuncInfo R;
				R.CantCheckBecauseIsUnMaped = true;
				return R;
			}
		}
	}
	auto copyname = Name;
	copyname._Isimmutable = false;
	copyname._MoveData = MoveData::None;
	copyname._IsAddress = false;
	
	String B = ToString(copyname);
	Token T;
	T.Type = TokenType::Name;
	T.Value._String = B;


	ScopedNameNode Tep;
	ScopedName V;
	V._token = &T;

	Tep._ScopedName.push_back(std::move(V));


	return Type_GetFunc(Tep, Pars, Name);
}

UCodeLangFrontEnd

#endif