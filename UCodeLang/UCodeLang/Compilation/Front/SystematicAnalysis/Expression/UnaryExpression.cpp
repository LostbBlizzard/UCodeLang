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
							const RawEvaluatedObject* NoneEnumVal = nullptr;
							for (size_t i = 0; i < info->VariantData.value().Variants.size(); i++)
							{
								auto& Item = info->VariantData.value().Variants[i];
								if (Item.Types.size() == 0)
								{
									NoneEnumVal = &info->Fields[i].Ex;
								}
							}

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
					else if (StringHelper::StartWith(name,UCode_ResultType))
					{

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