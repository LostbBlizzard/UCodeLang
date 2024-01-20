#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnExpressionNode(const UnaryExpressionNode& node)
{
	if (_PassType == PassType::GetTypes)
	{
		OnExpressionTypeNode(node._Value0, GetValueMode::Read);
	}
	else if (_PassType == PassType::FixedTypes)
	{
		OnExpressionTypeNode(node._Value0, GetValueMode::Read);

		auto extype = _LastExpressionType;

		auto BinaryOp = NeverNullptr(node._UnaryOp);
		auto Info = Type_HasUrinaryOverLoadWith(extype, node._UnaryOp->Type);

		if (!Info.HasValue)
		{
			LogError_CantFindUnaryOpForTypes(BinaryOp, extype);
			
			_LastExpressionType = TypesEnum::Null;
		}
		else
		{
			SymbolID SymID = Symbol_GetSymbolID(node);

			UnaryExpression_Data Data;
			Data.FuncToCall = nullptr;
			if (Info.Value.has_value())
			{
				Data.FuncToCall = Info.Value.value();
			}
			Data.Op0 = extype;

			_UnaryDatas.AddValue(SymID, std::move(Data));
		}

	}
	else if (_PassType == PassType::BuidCode)
	{
		SymbolID SymID = Symbol_GetSymbolID(node);
		auto& Data = _UnaryDatas.GetValue(SymID);


		OnExpressionTypeNode(node._Value0, GetValueMode::Read);
		auto ex = _IR_LastExpressionField;
		auto lasttype = _LastExpressionType;

		switch (node._UnaryOp->Type)
		{
		case TokenType::Not:
		{
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewlogicalNot(ex);
		}
		break;
		case TokenType::bitwise_not:
		{
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewBitWiseNot(ex);
		}
		break;
		case TokenType::QuestionMark:
		{
			

			if (auto V = Symbol_GetSymbol(lasttype).value_unchecked())
			{
				if (V->Type == SymbolType::Enum)
				{
					const EnumInfo* info = V->Get_Info<EnumInfo>();

					auto name = ScopeHelper::GetNameFromFullName(V->FullName);

					bool IsResultType = false;
					bool IsOpType = false;

					if (StringHelper::StartWith(name, UCode_OptionalType))
					{
						size_t NoneIndexKey = 0;
						size_t SomeIndexKey = 0;
						const RawEvaluatedObject* SomeEnumVal = nullptr;
						const RawEvaluatedObject* NoneEnumVal = nullptr;
						Optional<SymbolID> enumsometypeclasssym;
						for (size_t i = 0; i < info->VariantData.value().Variants.size(); i++)
						{
							auto& Item = info->VariantData.value().Variants[i];
							if (Item.Types.size() == 1)
							{
								enumsometypeclasssym = Item.ClassSymbol;
								SomeIndexKey = i;
								SomeEnumVal = &info->Fields[i].Ex;
							}
							else
							{
								NoneIndexKey = i;
								NoneEnumVal = &info->Fields[i].Ex;
							}
						}
						const IRStruct* structir = _IR_Builder.GetSymbol(IR_ConvertToIRType(lasttype)._symbol)->Get_ExAs<IRStruct>();


						UCodeLangAssert(NoneEnumVal);
						UCodeLangAssert(SomeEnumVal);

						auto Ptr = ex;

						auto key = _IR_LookingAtIRBlock->New_Member_Access(ex, structir, EnumVarantKeyIndex);

						auto same = _IR_LookingAtIRBlock->NewC_Equalto(key, LoadEvaluatedEx(*SomeEnumVal, info->Basetype));

						auto  V = _IR_LookingAtIRBlock->NewConditionalJump(same);

						//ret nothing
						{

							//Bug happens if bo
							auto funcret = _FuncStack.front().Pointer->Ret;
							auto v = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(funcret));
							auto retkey = _IR_LookingAtIRBlock->New_Member_Access(v, structir, EnumVarantKeyIndex);


							auto info = Symbol_GetSymbol(funcret).value()->Get_Info<EnumInfo>();

							_IR_LookingAtIRBlock->NewStore(retkey, LoadEvaluatedEx(*NoneEnumVal, info->Basetype));

							_IR_LookingAtIRBlock->NewRetValue(v);

							RetData tep;
							tep.JumpIns = _IR_LookingAtIRBlock->NewJump(0);
							_IR_Rets.push_back(tep);
						}
						_IR_LookingAtIRBlock->UpdateConditionaJump(V, same, _IR_LookingAtIRBlock->InsCount());

						auto unionV = _IR_LookingAtIRBlock->New_Member_Access(ex, structir, EnumVarantUnionIndex);
						auto eumv = _IR_LookingAtIRBlock->New_Member_Access(unionV, _IR_Builder.GetSymbol(unionV->ObjectType._symbol)->Get_ExAs<IRStruct>(), SomeIndexKey);

						if (enumsometypeclasssym.has_value())
						{
							auto v = IR_ConvertToIRType(TypeSymbol(enumsometypeclasssym.value()));
							_IR_LastExpressionField = _IR_LookingAtIRBlock->New_Member_Access(eumv, _IR_Builder.GetSymbol(v._symbol)->Get_ExAs<IRStruct>(),0);
						}
						else 
						{
							_IR_LastExpressionField = eumv;
						}
					}
					else if (StringHelper::StartWith(name, UCode_ResultType))
					{
						size_t Indexval = 0;
						size_t Indexerr = 0;
						{
							const Symbol* Resultgeneric = Symbol_GetSymbol(UCode_ResultType, SymbolType::Generic_Enum).value().value();
							const EnumInfo* ResultgenericInfo = Resultgeneric->Get_Info<EnumInfo>();


							if (ResultgenericInfo->VariantData.value().Variants[0].Types.front()._CustomTypeSymbol == ResultgenericInfo->_GenericData._Genericlist[0].SybID)
							{
								Indexval = 0;
								Indexerr = 1;
							}
							else
							{
								Indexval = 1;
								Indexerr = 0;
							}
						}

						const RawEvaluatedObject* ValEnumVal = &info->Fields[Indexval].Ex;
						const RawEvaluatedObject* ErrEnumVal = &info->Fields[Indexerr].Ex;
						Optional<SymbolID> enumValtypeclasssym;
						Optional<SymbolID> enumErrtypeclasssym;
						{
							auto& variantinfo = info->VariantData.value();

							enumValtypeclasssym = variantinfo.Variants[Indexval].ClassSymbol;
							enumErrtypeclasssym = variantinfo.Variants[Indexerr].ClassSymbol;
						}

						auto lasttypeir = IR_ConvertToIRType(lasttype);
						const IRStruct* structir = _IR_Builder.GetSymbol(lasttypeir._symbol)->Get_ExAs<IRStruct>();

						auto Ptr = ex;

						auto key = _IR_LookingAtIRBlock->New_Member_Access(ex, structir, EnumVarantKeyIndex);

						auto same = _IR_LookingAtIRBlock->NewC_Equalto(key, LoadEvaluatedEx(*ValEnumVal, info->Basetype));

						auto  V = _IR_LookingAtIRBlock->NewConditionalJump(same);

						//ret nothing
						{
							auto funcret = _FuncStack.front().Pointer->Ret;
							auto funcretir = IR_ConvertToIRType(funcret);
							auto v = _IR_LookingAtIRBlock->NewLoad(funcretir);


							//
							if (funcretir == lasttypeir) //if return type is the same as the Ex Err value
							{
								auto retkey = _IR_LookingAtIRBlock->New_Member_Access(v, structir, EnumVarantKeyIndex);
								auto retunion = _IR_LookingAtIRBlock->New_Member_Access(v, structir, EnumVarantUnionIndex);

								auto info = Symbol_GetSymbol(funcret).value()->Get_Info<EnumInfo>();

								_IR_LookingAtIRBlock->NewStore(retkey, LoadEvaluatedEx(*ErrEnumVal, info->Basetype));

								auto unionV = _IR_LookingAtIRBlock->New_Member_Access(ex, structir, EnumVarantUnionIndex);

								IRStruct* t = _IR_Builder.GetSymbol(unionV->ObjectType._symbol)->Get_ExAs<IRStruct>();

								auto eumv = _IR_LookingAtIRBlock->New_Member_Access(unionV,t, Indexerr);

								auto unionstore = _IR_LookingAtIRBlock->New_Member_Access(retunion, t, Indexerr);

								if (enumErrtypeclasssym.has_value())
								{
									eumv = _IR_LookingAtIRBlock->New_Member_Access(eumv, t,0);
									unionstore = _IR_LookingAtIRBlock->New_Member_Access(unionstore, t, 0);
								}
								//missing copy or move

								

								_IR_LookingAtIRBlock->NewStore(unionstore, eumv);

								_IR_LookingAtIRBlock->NewRetValue(v);
							}
							else{
								UCodeLangUnreachable();
							}

							RetData tep;
							tep.JumpIns = _IR_LookingAtIRBlock->NewJump(0);
							_IR_Rets.push_back(tep);
						}
						_IR_LookingAtIRBlock->UpdateConditionaJump(V, same, _IR_LookingAtIRBlock->InsCount());

						auto unionV = _IR_LookingAtIRBlock->New_Member_Access(ex, structir, EnumVarantUnionIndex);
						auto eumv = _IR_LookingAtIRBlock->New_Member_Access(unionV, _IR_Builder.GetSymbol(unionV->ObjectType._symbol)->Get_ExAs<IRStruct>(), Indexval);

						if (enumValtypeclasssym.has_value())
						{
							auto v = IR_ConvertToIRType(TypeSymbol(enumValtypeclasssym.value()));
							_IR_LastExpressionField = _IR_LookingAtIRBlock->New_Member_Access(eumv, _IR_Builder.GetSymbol(v._symbol)->Get_ExAs<IRStruct>(), 0);
						}
						else
						{
							_IR_LastExpressionField = eumv;
						}
					}


				}
			}

		}
		break;
		case TokenType::plus:
		{
			UCodeLangUnreachable();
		}
		break;
		case TokenType::minus:
		{
			UCodeLangUnreachable();
		}
		break;
		default:
			UCodeLangUnreachable();
			break;
		}
	}
}


UCodeLangFrontEnd

#endif