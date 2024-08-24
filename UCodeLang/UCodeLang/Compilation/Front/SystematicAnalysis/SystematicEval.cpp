#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
#include "UCodeLang/Compilation/Helpers/ParseHelper.hpp"
UCodeLangFrontStart
void* SystematicAnalysis::Eval_Get_Object(const TypeSymbol& Input, const RawEvaluatedObject& Input2)
{
	return Input2.Object_AsPointer.get();
}

void* SystematicAnalysis::Eval_Get_Object(const EvaluatedEx& Input)
{
	return Eval_Get_Object(Input.Type, Input.EvaluatedObject);
}
const void* SystematicAnalysis::Eval_Get_Object(const TypeSymbol& Input, const RawEvaluatedObject& Input2) const
{
	return Input2.Object_AsPointer.get();
}

const void* SystematicAnalysis::Eval_Get_Object(const EvaluatedEx& Input) const
{
	return Eval_Get_Object(Input.Type, Input.EvaluatedObject);
}

SystematicAnalysis::StrExELav SystematicAnalysis::Eval_GetStrEVal(const Node* node)
{
	StrExELav CompilerRet;

	OnExpressionTypeNode(node, GetValueMode::Read);

	if (node->Get_Type() == NodeType::ValueExpressionNode)
	{
		const ValueExpressionNode* V = ValueExpressionNode::As(node);
		if (V->_Value.get()->Get_Type() == NodeType::StringliteralNode)
		{
			StringliteralNode* strnod = StringliteralNode::As(V->_Value.get());

			String Buffer;
			ParseHelper::ParseStringliteralToString(strnod->token->Value._String, Buffer);
			CompilerRet.OwnedStr = std::move(Buffer);
			CompilerRet.Msg = CompilerRet.OwnedStr;
			CompilerRet.IsConstantExpression = true;
		}
	}

	return CompilerRet;
}
bool SystematicAnalysis::Eavl_ConstantExpressionAbleType(const TypeSymbol& Type)
{
	return Type_IsPrimitive(Type) && !Type.IsAddress() && !Type.IsAddressArray();
}

SystematicAnalysis::EvaluatedEx SystematicAnalysis::Eval_MakeEx(const TypeSymbol& Type)
{
	EvaluatedEx r;
	r.Type = Type;

	size_t Size = 0;
	Type_GetSize(Type, Size);

	r.EvaluatedObject.Object_AsPointer = std::make_unique<Byte[]>(Size);
	r.EvaluatedObject.ObjectSize = Size;

	return r;
}
RawEvaluatedObject SystematicAnalysis::Eval_MakeExr(const TypeSymbol& Type)
{
	auto val = Eval_MakeEx(Type);
	return val.EvaluatedObject;
}

bool SystematicAnalysis::Eval_EvaluateDefaultConstructor(EvaluatedEx& Out)
{
	void* Object = Eval_Get_Object(Out);
	switch (Out.Type._Type)
	{
	case TypesEnum::Char:
	case TypesEnum::Bool:
	case TypesEnum::sInt8:
	case TypesEnum::uInt8:
		*(UInt8*)Object = 0;
		break;
	case TypesEnum::uInt16:
	case TypesEnum::sInt16:
		*(UInt16*)Object = 0;
		break;
	case TypesEnum::uInt32:
	case TypesEnum::sInt32:
		*(UInt32*)Object = 0;
		break;
	case TypesEnum::uInt64:
	case TypesEnum::sInt64:
		*(UInt64*)Object = 0;
		break;

	case TypesEnum::uIntPtr:
	case TypesEnum::sIntPtr:
	{
		size_t PtrSize = 0;
		TypeSymbol V;
		V.SetType(TypesEnum::uIntPtr);
		Type_GetSize(V, PtrSize);
		switch (PtrSize)
		{
			case sizeof(UInt8) : *(UInt8*)Object = 0; break;
				case sizeof(UInt16) : *(UInt16*)Object = 0; break;
					case sizeof(UInt32) : *(UInt32*)Object = 0; break;
						case sizeof(UInt64) : *(UInt64*)Object = 0; break;
						default:
							UCodeLangUnreachable();
							break;
		}
	}break;

	case TypesEnum::float32:
		*(float32*)Object = 0;
		break;
	case TypesEnum::float64:
		*(float64*)Object = 0;
		break;
	default:
		UCodeLangUnreachable();
		break;
	}
	return false;
}

bool SystematicAnalysis::Eval_Evaluate(EvaluatedEx& Out, const ValueExpressionNode& node)
{
	switch (node._Value->Get_Type())
	{
	case NodeType::NumberliteralNode:
	{
		NumberliteralNode* num = NumberliteralNode::As(node._Value.get());
#define Set_NumberliteralNodeU2(x) \
			UInt##x V; \
			ParseHelper::ParseStringToUInt##x(Str, V); \
			Eval_Set_ObjectAs(Out,V);\

#define Set_NumberliteralNodeS2(x) \
			Int##x V; \
			ParseHelper::ParseStringToInt##x(Str, V); \
			Eval_Set_ObjectAs(Out,V);\

		auto& lookT = Type_Get_LookingForType();
		TypesEnum NewEx;
		if (lookT._Type == TypesEnum::Var)
		{
			NewEx = TypesEnum::sInt32;
		}
		else
		{
			NewEx = (Type_IsfloatType(lookT) || Type_IsIntType(lookT)) ? lookT._Type : TypesEnum::sInt32;
		}
		//if (passtype == PassType::BuidCode)
		{
			auto& Str = num->token->Value._String;



			switch (NewEx)
			{
			sint8case:
			case TypesEnum::sInt8:
			case TypesEnum::uInt8:
			{
				Set_NumberliteralNodeU2(8);
			};
			break;
		sint16case:
			case TypesEnum::sInt16:
			case TypesEnum::uInt16:
			{
				Set_NumberliteralNodeU2(16);
			};
			break;
		sint32case:
			case TypesEnum::sInt32:
			case TypesEnum::uInt32:
			{
				Set_NumberliteralNodeU2(32);
			};
			break;
		sint64case:
			case TypesEnum::sInt64:
			case TypesEnum::uInt64:
			{
				Set_NumberliteralNodeU2(64);
			};
			break;
			case TypesEnum::sIntPtr:
			case TypesEnum::uIntPtr:
			{
				size_t PtrSize = 0;
				TypeSymbol V;
				V.SetType(TypesEnum::uIntPtr);
				Type_GetSize(V, PtrSize);
				switch (PtrSize)
				{
					case sizeof(UInt8) : goto sint8case;
						case sizeof(UInt16) : goto sint16case;
							case sizeof(UInt32) : goto sint32case;
								case sizeof(UInt64) : goto sint64case;
								default:
									UCodeLangUnreachable();
									break;
				}
			};
			break;

			case TypesEnum::float32:
			{
				Int32 V;
				ParseHelper::ParseStringToInt32(Str, V);
				Eval_Set_ObjectAs(Out, (float32)V);
				break;
			};
			case TypesEnum::float64:
			{
				Int64 V;
				ParseHelper::ParseStringToInt64(Str, V);
				Eval_Set_ObjectAs(Out, (float64)V);
				break;
			};
			default:
				UCodeLangUnreachable();
				break;
			}

		}


		_LastExpressionType.SetType(NewEx);
		_LastLookedAtToken = Nullableptr(num->token);
	}
	break;
	case NodeType::BoolliteralNode:
	{
		BoolliteralNode* num = BoolliteralNode::As(node._Value.get());

		//if (passtype == PassType::BuidCode)
		{
			Eval_Set_ObjectAs(Out, num->Get_Value());
		}
		_LastExpressionType.SetType(TypesEnum::Bool);
		_LastLookedAtToken = Nullableptr(num->token);
	}
	break;
	case NodeType::CharliteralNode:
	{
		CharliteralNode* num = CharliteralNode::As(node._Value.get());

		//if (passtype == PassType::BuidCode)
		{
			String V;
			bool ItWorked = !ParseHelper::ParseCharliteralToChar(num->token->Value._String, V);

			Eval_Set_ObjectAs(Out, (UInt8)V.front());
		}
		_LastExpressionType.SetType(TypesEnum::Char);
		_LastLookedAtToken = Nullableptr(num->token);
	}
	break;
	case NodeType::FloatliteralNode:
	{
		FloatliteralNode* num = FloatliteralNode::As(node._Value.get());
		auto& lookT = Type_Get_LookingForType();

		auto& Str = num->token->Value._String;


		TypesEnum NewEx;
		if (lookT._Type == TypesEnum::Var)
		{
			NewEx = TypesEnum::float32;
		}
		else
		{
			NewEx = Type_IsfloatType(lookT) ? lookT._Type : TypesEnum::float32;
		}
		//if (passtype == PassType::BuidCode)
		{
			switch (NewEx)
			{
			case TypesEnum::float32:
			{
				float32 V;
				ParseHelper::ParseStringTofloat32(Str, V);
				Eval_Set_ObjectAs(Out, V);
				break;
			}
			case TypesEnum::float64:
			{
				float64 V;
				ParseHelper::ParseStringTofloat64(Str, V);
				Eval_Set_ObjectAs(Out, V);
				break;
			}
			default:
				UCodeLangUnreachable();
				break;
			}
		}



		_LastExpressionType.SetType(NewEx);
		_LastLookedAtToken = Nullableptr(num->token);
	}
	break;
	case NodeType::StringliteralNode:
	{
		StringliteralNode* nod = StringliteralNode::As(node._Value.get());
		auto& lookT = Type_Get_LookingForType();

		bool IsStaticArr = Type_IsStaticCharArr(lookT);
		if (IsStaticArr)
		{
			auto VSyb = Symbol_GetSymbol(lookT).value();
			StaticArrayInfo* StaticArr = VSyb->Get_Info<StaticArrayInfo>();


			String V;
			bool ItWorked = !ParseHelper::ParseStringliteralToString(nod->token->Value._String, V);
			size_t BufferSize = V.size();

			if (StaticArr->IsCountInitialized == false)
			{


				StaticArr->Count = V.size() + 1;//with null char;
				StaticArr->IsCountInitialized = true;

			}
			else
			{
				if (StaticArr->Count != BufferSize)
				{
					const NeverNullPtr<Token> Token = _LastLookedAtToken.value();
					LogError_CanIncorrectStaticArrCount(Token, lookT, BufferSize, StaticArr->Count);
					_LastExpressionType.SetType(TypesEnum::Null);
					return false;
				}
			}


			Out = Eval_MakeEx(lookT);
			Eval_Set_ObjectAs(Out, V.data(), V.size());
			_LastExpressionType = lookT;
		}
		else
		{
			auto Token = nod->token;
			LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos, "Cant use char[&] in Compile Time.");
			_LastExpressionType.SetType(TypesEnum::Null);
			return false;
		}
	}
	break;
	case NodeType::SizeofExpresionNode:
	{
		SizeofExpresionNode* nod = SizeofExpresionNode::As(node._Value.get());

		auto& lookT = Type_Get_LookingForType();
		TypeSymbol Type;

		//if (passtype == PassType::FixedTypes || passtype == PassType::BuidCode)
		{
			switch (lookT._Type)
			{
			case TypesEnum::sInt8:
			case TypesEnum::uInt8:
				Type.SetType(TypesEnum::uInt8);
				break;
			case TypesEnum::sInt16:
			case TypesEnum::uInt16:
				Type.SetType(TypesEnum::uInt16);
				break;
			case TypesEnum::sInt32:
			case TypesEnum::uInt32:
				Type.SetType(TypesEnum::uInt32);
				break;
			case TypesEnum::sInt64:
			case TypesEnum::uInt64:
				Type.SetType(TypesEnum::uInt64);
				break;
			default:
				Type.SetType(TypesEnum::uIntPtr);
				break;
			}
		}

		//if (passtype == PassType::BuidCode)
		{
			TypeSymbol Info;
			Type_Convert(nod->_Type, Info);
			UAddress TypeSize;
			Type_GetSize(Info, TypeSize);



			switch (lookT._Type)
			{
			case TypesEnum::sInt8:
				Eval_Set_ObjectAs(Out, (Int8)TypeSize);
				break;
			Int8Case:
			case TypesEnum::uInt8:
				Eval_Set_ObjectAs(Out, (UInt8)TypeSize);
				break;
			case TypesEnum::sInt16:
				Eval_Set_ObjectAs(Out, (Int16)TypeSize);
				break;
			Int16Case:

			case TypesEnum::uInt16:
				Eval_Set_ObjectAs(Out, (UInt16)TypeSize);
				break;

			case TypesEnum::sInt32:
				Eval_Set_ObjectAs(Out, (Int32)TypeSize);
				break;
			Int32Case:
			case TypesEnum::uInt32:
				Eval_Set_ObjectAs(Out, (UInt32)TypeSize);
				break;

			case TypesEnum::sInt64:
				Eval_Set_ObjectAs(Out, (Int64)TypeSize);
				break;
			Int64Case:
			case TypesEnum::uInt64:
				Eval_Set_ObjectAs(Out, (UInt64)TypeSize);
				break;
			default:
			{
				Type.SetType(TypesEnum::uIntPtr);

				UAddress PtrSize = 0;
				Type_GetSize(Type, PtrSize);

				switch (PtrSize)
				{
					case sizeof(UInt8) : goto Int8Case;
						case sizeof(UInt16) : goto Int16Case;
							case sizeof(UInt32) : goto Int32Case;
								case sizeof(UInt64) : goto Int64Case;
								default:
									UCodeLangUnreachable();
									break;
				}

			}break;
			}
		}

		_LastExpressionType = Type;
	}
	break;
	case NodeType::ReadVariableNode:
	{
		ReadVariableNode* nod = ReadVariableNode::As(node._Value.get());
		return Eval_Evaluate(Out, *nod);
	}
	break;
	case NodeType::CMPTypesNode:
	{
		CMPTypesNode* nod = CMPTypesNode::As(node._Value.get());
		return Eval_EvalutateCMPTypesNode(Out, *nod);
	}
	break;
	case NodeType::ValidNode:
	{
		ValidNode* nod = ValidNode::As(node._Value.get());
		return Eval_EvalutateValidNode(Out, *nod);
	}
	break;
	case NodeType::TypeToValueNode:
	{
		auto oldpasstype = _PassType;
		_PassType = PassType::FixedTypes;

		OnTypeToValueNode(*TypeToValueNode::As(node._Value.get()));
		Eval_Set_ObjectAs(Out, _LastExpressionType);

		_PassType = oldpasstype;

		return true;
	}
	break;
	case NodeType::ExpressionToTypeValueNode:
	{
		auto oldpasstype = _PassType;

		_PassType = PassType::FixedTypes;

		OnExpressionToTypeValueNode(*ExpressionToTypeValueNode::As(node._Value.get()));
		Eval_Set_ObjectAs(Out, _LastExpressionType);

		_PassType = oldpasstype;

		return true;
	}
	break;
	case NodeType::FuncCallNode:
	{
		return Eval_EvalutateFunc(Out, *FuncCallNode::As(node._Value.get()));
	}
	break;
	default:
		UCodeLangUnreachable();
		break;
	}
	return true;
}
bool SystematicAnalysis::Eval_Evaluate(EvaluatedEx& Out, const ReadVariableNode& nod)
{
	GetMemberTypeSymbolFromVar_t V;
	return Eval_EvalutateScopedName(Out, nod._VariableName, V);
}

bool SystematicAnalysis::Eval_Evaluate(EvaluatedEx& Out, const MatchExpression& nod,bool runmatchcheck)
{
	if (runmatchcheck)
	{
		//To do added run time check.
		UCodeLangAssert(false);
	}

	auto& matchinfo = _MatchExpressionDatas.GetValue(Symbol_GetSymbolID(nod));

	auto matchtype = TypesEnum::Bool;

	_LookingForTypes.push(matchtype);
	auto matchval = Eval_Evaluate(matchtype,nod._Expression);
	_LookingForTypes.pop();
	
	auto ToMatchType = matchtype;

	if (!matchval.has_value()) { return false; }

	auto& matchcmptype = matchval.value();

	size_t ScopeCounter = 0;
	const String ScopeName = GetScopeLabelName(&nod);
	{

		for (size_t i = 0; i < nod._Arms.size(); i++)
		{
			auto& Item = nod._Arms[i];
			auto& VItem = matchinfo.ArmData.Arms[i];

			_Table.AddScope(ScopeName + std::to_string(ScopeCounter));
			UCodeLangDefer(_Table.RemoveScope(); );
			{
				if (Eval_MatchArm(ToMatchType, matchcmptype, VItem, Item._Expression))
				{
					EvaluatedEx ArmType = Eval_MakeEx(matchinfo._MatchAssignmentType);
					auto matchval = Eval_Evaluate(matchinfo._MatchAssignmentType, Item._AssignmentExpression);
					
					if (matchval.has_value()) {
						Out = std::move(matchval.value());
					}
					return matchval.has_value();
				}
			}
			ScopeCounter++;
		}
	}
	if (nod._InvaidCase)
	{
		auto matchval = Eval_Evaluate(matchinfo._MatchAssignmentType, nod._InvaidCase.value());

		_Table.AddScope(ScopeName + std::to_string(ScopeCounter));
		if (matchval.has_value()) 
		{
			Out = std::move(matchval.value());
		}
		_Table.RemoveScope();
		
		return matchval.has_value();
	}
	else 
	{
		auto val = matchval.value();
		LogError(ErrorCodes::ExpectingSequence
			, "None of match arms was true for '" + ToString(val.Type,val.EvaluatedObject) + "'"
			,NeverNullptr(nod._Token));
	}
	

	return false;
}
void SystematicAnalysis::DoBinaryOpContextWith(TypeSymbol type,const SystematicAnalysis::DoBinaryOpContext& context)
{
	if (type._Type == TypesEnum::Bool)
	{
		bool& val0 = *(bool*)context.Op1->Object_AsPointer.get();
		bool& val1 = *(bool*)context.Op2->Object_AsPointer.get();

		auto outex = Eval_MakeExr(TypesEnum::Bool);

		bool& outval = *(bool*)outex.Object_AsPointer.get();
		switch (context.type)
		{
		case TokenType::equal_Comparison:outval = val0 == val1; break;
		case TokenType::Notequal_Comparison:outval = val0 != val1; break;
		default:
			UCodeLangUnreachable();
			break;
		}

		*context.OpOut = std::move(outex);
		return;
	}
	else if (Type_IsIntType(type))
	{
		TypeSymbol maintype = type;
		if (maintype._Type == TypesEnum::uIntPtr)
		{
			maintype = Type_GetSize(TypesEnum::uIntPtr).value() == 8
				? TypeSymbol(TypesEnum::uInt64) : TypeSymbol(TypesEnum::uInt32);
		}
		else if (maintype._Type == TypesEnum::sIntPtr)
		{
			maintype = Type_GetSize(TypesEnum::sIntPtr).value() == 8
				? TypeSymbol(TypesEnum::sInt64) : TypeSymbol(TypesEnum::sInt32);
		}

		switch (maintype._Type)
		{
		case TypesEnum::sInt8:DoBinaryIntOp<Int8>(context); break;
		case TypesEnum::sInt16:DoBinaryIntOp<Int16>(context); break;
		case TypesEnum::sInt32:DoBinaryIntOp<Int32>(context); break;
		case TypesEnum::sInt64:DoBinaryIntOp<Int64>(context); break;

		case TypesEnum::uInt8:DoBinaryIntOp<UInt8>(context); break;
		case TypesEnum::uInt16:DoBinaryIntOp<UInt16>(context); break;
		case TypesEnum::uInt32:DoBinaryIntOp<UInt32>(context); break;
		case TypesEnum::uInt64:DoBinaryIntOp<UInt64>(context); break;


		default:
			UCodeLangUnreachable();
			break;
		}
		return;
	}
	UCodeLangUnreachable();
}
bool SystematicAnalysis::Eval_MatchArm(const TypeSymbol& MatchItem,const EvaluatedEx& Item, MatchArm& Arm, const ExpressionNodeType& ArmEx)
{
	bool IsJust =
		MatchItem._IsAddressArray == false
		&& MatchItem._IsDynamic == false
		&& MatchItem._TypeInfo == TypeInfoPrimitive::Null;
	
	if (IsJust)
	{
		if (Type_IsIntType(MatchItem) ||
			Type_IsfloatType(MatchItem._Type) ||
			Type_IsCharType(MatchItem._Type) ||
			MatchItem._Type == TypesEnum::Bool)
		{

			if (ArmEx._Value.get()->Get_Type() == NodeType::ValueExpressionNode)
			{
				_LookingForTypes.push(MatchItem);
				auto val = Eval_Evaluate(MatchItem,ArmEx);
				_LookingForTypes.pop();

				if (val.has_value())
				{
					auto& eval = val.value();

					DoBinaryOpContext context;
					context.Op1 = &eval.EvaluatedObject;
					context.Op2 = &Item.EvaluatedObject;
					context.type = TokenType::equal_Comparison;


					EvaluatedEx returnedeval = Eval_MakeEx(TypesEnum::Bool);
					context.OpOut = &returnedeval.EvaluatedObject;
					DoBinaryOpContextWith(MatchItem,context);

					return *Eval_Get_ObjectAs<bool>(returnedeval);
				}
				return false;
			}
			else
			{
				UCodeLangUnreachable();
			}
		}
	}
	return false;
}
bool SystematicAnalysis::Eval_Evaluate(EvaluatedEx& Out, const BinaryExpressionNode& node)
{
	auto Ex0node = node._Value0._Value.get();
	auto Ex1node = node._Value1._Value.get();

	OnExpressionTypeNode(Ex0node, GetValueMode::Read);//check
	TypeSymbol Ex0Type = _LastExpressionType;

	_LookingForTypes.push(Ex0Type);

	OnExpressionTypeNode(Ex1node, GetValueMode::Read);//check
	TypeSymbol Ex1Type = _LastExpressionType;
	
	_LookingForTypes.pop();

	auto overload = Type_HasBinaryOverLoadWith(Ex0Type, node._BinaryOp->Type, Ex1Type);

	if (!overload.HasValue) {
		return false;
	}

	auto ex0 = Eval_Evaluate(Ex0Type, node._Value0);
	
	_LookingForTypes.push(Ex0Type);
	auto ex1 = Eval_Evaluate(Ex1Type, node._Value1);
	_LookingForTypes.pop();

	if (!ex0.has_value() || !ex1.has_value())
	{
		return false;
	}

	auto extype0 = ex0.value().Type;
	auto extype1 = ex1.value().Type;

	auto& exval0 = ex0.value().EvaluatedObject;
	auto& exval1 = ex1.value().EvaluatedObject;
	bool sametype = Type_AreTheSame(extype0, extype1);

	if (sametype)
	{
		DoBinaryOpContext context;
		context.Op1 = &exval0;
		context.Op2 = &exval1;
		context.OpOut = &Out.EvaluatedObject;
		context.type = node._BinaryOp->Type;
		DoBinaryOpContextWith(extype0,context);
		return true;
	}

	return false;
}

bool SystematicAnalysis::Eval_Evaluate(EvaluatedEx& Out, const CastNode& node)
{
	TypeSymbol ToTypeAs;
	Type_ConvertAndValidateType(node._ToType, ToTypeAs, NodeSyb_t::Any);

	_LookingForTypes.push(ToTypeAs);

	bool Ex0Bool = Eval_Evaluate_t(Out, node._Expression._Value.get(), GetValueMode::Read);

	_LookingForTypes.pop();


	auto Ex0Type = _LastExpressionType;
	auto HasInfo = Type_CanBeExplicitlyConverted(Ex0Type, ToTypeAs);
	if (!HasInfo.HasValue)
	{
		auto  Token = node._ToType._name._ScopedName.back()._token;

		LogError_CantCastExplicityTypes(NeverNullptr(Token), Ex0Type, ToTypeAs);
		return false;
	}
	else
	{
		if (HasInfo.Value.has_value())
		{
			auto symop = Symbol_GetSymbol(ToTypeAs);
			if (symop.has_value())
			{
				auto sym = symop.value();

				if (sym->Type == SymbolType::Enum)
				{
					auto info = sym->Get_Info<EnumInfo>();

					if (info->VariantData.has_value())
					{
						UCodeLangUnreachable();
					}
					else if (Type_AreTheSame(info->Basetype, ToTypeAs))
					{
						return true;
					}
				}
			}
			return false;
		}
		else
		{
			_LastExpressionType = ToTypeAs;
			Out.Type = _LastExpressionType;
		}

	}
	return true;
}

bool SystematicAnalysis::Eval_Evaluate_t(EvaluatedEx& Out, const Node* node, GetValueMode Mode)
{
	_GetExpressionMode.push(Mode);
	bool R = false;
	switch (node->Get_Type())
	{
	case NodeType::BinaryExpressionNode: R = Eval_Evaluate(Out, *BinaryExpressionNode::As(node)); break;
	case NodeType::ValueExpressionNode: R = Eval_Evaluate(Out, *ValueExpressionNode::As(node)); break;
	case NodeType::CastNode: R = Eval_Evaluate(Out, *CastNode::As(node)); break;
	case NodeType::ExtendedFuncExpression: R = Eval_Evaluate(Out, *ExtendedFuncExpression::As(node)); break;
	case NodeType::ExtendedScopeExpression: R = Eval_Evaluate(Out, *ExtendedScopeExpression::As(node)); break;
	default:
		UCodeLangUnreachable();
		break;
	}
	_GetExpressionMode.pop();

	return R;
}

bool SystematicAnalysis::Eval_Evaluate(EvaluatedEx& Out, const ExpressionNodeType& node, GetValueMode Mode)
{
	return Eval_Evaluate_t(Out, node._Value.get(), Mode);
}

bool SystematicAnalysis::Eval_EvaluatePostfixOperator(EvaluatedEx& Out, TokenType Op)
{
	void* Object = Eval_Get_Object(Out);
	switch (Out.Type._Type)
	{

	uint8case:
	case TypesEnum::sInt8:
	case TypesEnum::uInt8:
	{
		if (Op == TokenType::increment)
		{
			(*(UInt8*)Object)++;
		}
		else
			if (Op == TokenType::decrement)
			{
				(*(UInt8*)Object)--;
			}
			else
			{
				UCodeLangUnreachable();
			}
	}
	break;

uint16case:
	case TypesEnum::uInt16:
	case TypesEnum::sInt16:
	{
		if (Op == TokenType::increment)
		{
			(*(UInt16*)Object)++;
		}
		else
			if (Op == TokenType::decrement)
			{
				(*(UInt16*)Object)--;
			}
			else
			{
				UCodeLangUnreachable();
			}
	}break;

uint32case:
	case TypesEnum::uInt32:
	case TypesEnum::sInt32:
	{
		if (Op == TokenType::increment)
		{
			(*(UInt32*)Object)++;
		}
		else
			if (Op == TokenType::decrement)
			{
				(*(UInt32*)Object)--;
			}
			else
			{
				UCodeLangUnreachable();
			}
	}
	break;
uint64case:
	case TypesEnum::uInt64:
	case TypesEnum::sInt64:
	{
		if (Op == TokenType::increment)
		{
			(*(UInt64*)Object)++;
		}
		else
			if (Op == TokenType::decrement)
			{
				(*(UInt64*)Object)--;
			}
			else
			{
				UCodeLangUnreachable();
			}
	}
	break;

	case TypesEnum::uIntPtr:
	case TypesEnum::sIntPtr:
	{
		size_t PtrSize = 0;
		TypeSymbol V;
		V.SetType(TypesEnum::uIntPtr);
		Type_GetSize(V, PtrSize);
		switch (PtrSize)
		{
			case sizeof(UInt8) : goto uint8case;
				case sizeof(UInt16) : goto uint16case;
					case sizeof(UInt32) : goto uint32case;
						case sizeof(UInt64) : goto uint64case;
						default:
							UCodeLangUnreachable();
							break;
		}
	}break;
	default:
		UCodeLangUnreachable();
		break;
	}
	return true;
}

bool SystematicAnalysis::Eval_HasConstantPostfixOperator(const TypeSymbol& Type, TokenType Op)
{
	if ((Op == TokenType::increment || Op == TokenType::decrement) && Type_IsIntType(Type))
	{
		return true;
	}
	return false;
}

bool SystematicAnalysis::Eval_CanEvaluateImplicitConversionConstant(const TypeSymbol& Type, const TypeSymbol& ToType)
{
	if (Type_AreTheSame(Type, ToType))
	{
		return true;
	}
	return false;
}

bool SystematicAnalysis::Eval_EvaluateImplicitConversion(EvaluatedEx& In, const TypeSymbol& ToType, EvaluatedEx& out)
{
	if (Type_AreTheSame(In.Type, ToType) || (In.Type.IsTypeInfo() && ToType.IsTypeInfo()))
	{
		out.Type = In.Type;
		out.EvaluatedObject = In.EvaluatedObject;
		return true;
	}
	return false;
}
bool SystematicAnalysis::Eval_EvalutateStepScopedName(EvaluatedEx& Out, const ScopedNameNode& node, size_t Index, ScopedName::Operator_t OpType, GetMemberTypeSymbolFromVar_t& OtherOut)
{
	return false;
}
bool SystematicAnalysis::Eval_CanEvalutateFuncCheck(const Get_FuncInfo& Func)
{
	if (Func._BuiltFunc.has_value())
	{
		return true;
	}

	if (Func.SymFunc && Func.SymFunc->NodePtr)
	{
		const FuncNode* node = FuncNode::As(Func.SymFunc->Get_NodeInfo<Node>());

		if (node->_Signature._HasEvalKeyWord)
		{
			return true;
		}

	}


	return false;
}
bool SystematicAnalysis::Eval_Evaluate(EvaluatedEx& Out, const TypeSymbol& MustBeType, const ExpressionNodeType& node)
{
	OnExpressionTypeNode(node._Value.get(), GetValueMode::Read);//check

	if (Type_IsUnMapType(_LastExpressionType))
	{
		int a = 0;
		//Out = Eval_MakeEx(MustBeType);
		//Eval_EvaluateDefaultConstructor(Out);
		//return true;
	}

	if (!Type_CanBeImplicitConverted(_LastExpressionType, MustBeType, false) && !(_LastExpressionType.IsTypeInfo() && MustBeType.IsTypeInfo()))
	{
		LogError_CantCastImplicitTypes(_LastLookedAtToken.value(), _LastExpressionType, MustBeType, false);
		return false;
	}
	if (!Eval_CanEvaluateImplicitConversionConstant(_LastExpressionType, MustBeType) && !(_LastExpressionType.IsTypeInfo() && MustBeType.IsTypeInfo()))
	{
		LogError_Eval_CantCastImplicitTypes(_LastLookedAtToken.value(), _LastExpressionType, MustBeType);
		return false;
	}

	EvaluatedEx ex1 = Eval_MakeEx(_LastExpressionType);
	if (Eval_Evaluate_t(ex1, node._Value.get(), GetValueMode::Read))
	{
		return Eval_EvaluateImplicitConversion(ex1, MustBeType, Out);
	}
	return false;
}
Optional<SystematicAnalysis::EvaluatedEx> SystematicAnalysis::Eval_Evaluate(const TypeSymbol& MustBeType, const ExpressionNodeType& node)
{
	EvaluatedEx Out;
	bool V = Eval_Evaluate(Out, MustBeType, node);

	if (V)
	{
		return { Out };
	}
	else
	{
		return {};
	}
}
bool SystematicAnalysis::Eval_EvaluateToAnyType(EvaluatedEx& Out, const ExpressionNodeType& node)
{
	//Nodes like type and typeof dont will throw an error if _PassType == PassType::Build
	auto oldpasstype = _PassType;
	_PassType = PassType::FixedTypes;
	OnExpressionTypeNode(node._Value.get(), GetValueMode::Read);//check
	_PassType = oldpasstype;

	EvaluatedEx ex1 = Eval_MakeEx(_LastExpressionType);
	bool CompilerRet = Eval_Evaluate_t(ex1, node._Value.get(), GetValueMode::Read);
	Out = std::move(ex1);
	return CompilerRet;
}
bool SystematicAnalysis::Eval_EvalutateCMPTypesNode(EvaluatedEx& Out, const CMPTypesNode& node)
{
	TypeSymbol Op0 = Type_ConvertAndValidateType(node._TypeOp0, NodeSyb_t::Any);
	TypeSymbol Op1 = Type_ConvertAndValidateType(node._TypeOp1, NodeSyb_t::Any);

	Eval_Set_ObjectAs(Out, Type_Compare(Op0, Op1, NeverNullptr(node._Op)));
	return true;
}
bool SystematicAnalysis::Eval_EvalutateValidNode(EvaluatedEx& Out, const ValidNode& node)
{

	bool IsValid = true;
	auto ErrCount = _ErrorsOutput->Get_Errors().size();

	if (node._IsExpression)
	{
		OnExpressionTypeNode(node._ExpressionToCheck._Value.get(), GetValueMode::Read);
	}
	else
	{
		for (auto& Item : node._StatementToCheck._Nodes)
		{
			OnStatement(*Item);
		}
	}

	if (ErrCount < _ErrorsOutput->Get_Errors().size())
	{
		size_t CountToPop = _ErrorsOutput->Get_Errors().size() - ErrCount;
		for (size_t i = 0; i < CountToPop; i++)
		{
			_ErrorsOutput->Get_Errors().pop_back();
		}
		IsValid = false;
	}

	Eval_Set_ObjectAs(Out, IsValid);

	_LastExpressionType = TypesEnum::Bool;
	return true;
}
bool SystematicAnalysis::Eval_EvalutateFunc(EvaluatedEx& Out, const FuncCallNode& node)
{
	Get_FuncInfo FuncInfo;

	auto symid = Symbol_GetSymbolID(node);
	if (_FuncToSyboID.HasValue(symid))
	{
		FuncInfo = _FuncToSyboID.GetValue(symid);
	}
	else
	{
		FuncInfo = Type_GetFunc(node._FuncName, node.Parameters, Type_Get_LookingForType());
	}

	if (Eval_CanEvalutateFuncCheck(FuncInfo))
	{
		Vector<EvaluatedEx> ValuePars;
		if (FuncInfo.Func)
		{
			ValuePars.resize(FuncInfo.Func->Pars.size());

			for (size_t i = 0; i < node.Parameters._Nodes.size(); i++)
			{
				const TypeSymbol& Par = FuncInfo.Func->Pars[i].Type;
				auto& Item = node.Parameters._Nodes[i];

				auto Info = Eval_Evaluate(Par, *Item.get());

				if (!Info.has_value())
				{
					return false;
				}

				ValuePars.push_back(std::move(Info.value()));
			}
		}
		return Eval_EvalutateFunc(Out, FuncInfo, node._FuncName, ValuePars);
	}
	return false;
}

bool SystematicAnalysis::Eval_EvalutateFunc(EvaluatedEx& Out, const Get_FuncInfo& Func, const ScopedNameNode& Name, const Vector<EvaluatedEx>& ValuePars)
{

	if (Func.SymFunc)
	{
		FileDependency_AddDependencyToCurrentFile(Func.SymFunc);
	}
	Type_SetFuncRetAsLastEx(Func);


	if (Func._BuiltFunc.has_value())
	{
		auto& BuiltFunc = Func._BuiltFunc.value();
		if (BuiltFunc.EvalObject.has_value())
		{
			auto& EvalObject = BuiltFunc.EvalObject.value();
			Out.Type = _LastExpressionType;
			Out.EvaluatedObject = EvalObject;
			return true;
		}
	}
	else if (Func.SymFunc)
	{
		const FuncNode* Funcnode = FuncNode::As(Func.SymFunc->Get_NodeInfo<Node>());


		EvalFuncData* State = new EvalFuncData();
		_Eval_FuncStackFrames.push_back(Unique_ptr<EvalFuncData>(State));
		constexpr size_t StackSize = 100;
		State->CallFrom = Name._ScopedName.back()._token;
		State->FuncSyb = Func.SymFunc;
		bool Ok = false;
		if (_Eval_FuncStackFrames.size() >= StackSize)
		{
			const NeverNullPtr<Token> token = NeverNullptr(State->CallFrom);
			String Str;
			Str += "Eval func Stack overflow.";
			Str += "--Call Stack \n";
			for (auto& Item : _Eval_FuncStackFrames)
			{
				Str += "On line " + std::to_string(Item->CallFrom->OnLine) + (String)" Called '" + Item->FuncSyb->FullName + "'. \n";
			}

			_ErrorsOutput->AddError(ErrorCodes::InValidType, token->OnLine, token->OnPos, Str);
			Ok = false;
		}
		else
		{
			Ok = Eval_EvalutateFunc(*State, Func.SymFunc, ValuePars);
		}
		if (Ok)
		{
			Out.Type = Func.SymFunc->Get_Info<FuncInfo>()->Ret;
			Out.EvaluatedObject = State->Ret;
		}
		_Eval_FuncStackFrames.pop_back();
		return Ok;
	}

	return false;
}
bool SystematicAnalysis::Eval_EvalutateFunc(EvaluatedEx& Out, const TypeSymbol& Type, const Get_FuncInfo& Func, const Vector<EvaluatedEx>& ValuePars)
{
	String B = ToString(Type);
	Token T;
	T.Type = TokenType::Name;
	T.Value._String = B;


	ScopedNameNode Tep;
	ScopedName V;
	V._token = &T;

	Tep._ScopedName.push_back(std::move(V));

	return Eval_EvalutateFunc(Out, Func, Tep, ValuePars);
}
bool SystematicAnalysis::Eval_Evaluate(EvaluatedEx& Out, const ExtendedScopeExpression& node)
{
	Optional<EvaluatedEx> Ex = Eval_EvaluateToAnyType(node._Expression);

	if (Ex.has_value())
	{
		auto ExpressionType = _LastExpressionType;

		GetMemberTypeSymbolFromVar_t V;

		auto ExValue = Ex.value();

		_GetExpressionMode.push(_GetExpressionMode.top());
		{
			V.Type = ExpressionType;
			V._Symbol = Symbol_GetSymbol(ExpressionType).value_unchecked();


			if (Eval_EvalutateStepScopedName(ExValue, node._Extended, 0, node._Operator, V))
			{
				return false;
			}

			for (size_t i = 1; i < node._Extended._ScopedName.size(); i++)
			{
				if (Eval_EvalutateStepScopedName(ExValue, node._Extended, i, node._Extended._ScopedName[i]._operator, V))
				{
					return false;
				}
			}
		}
		_GetExpressionMode.pop();

		_LastExpressionType = V.Type;
		Out = std::move(ExValue);
		return true;
	}
	return false;
}
SystematicAnalysis::EvaluatedEx SystematicAnalysis::Eval_Evaluate_GetPointer()
{
	return {};
}

bool SystematicAnalysis::Eval_Evaluate(EvaluatedEx& Out, const ExtendedFuncExpression& node)
{
	Optional<EvaluatedEx> Ex = Eval_EvaluateToAnyType(node._Expression);

	auto ExpressionType = _LastExpressionType;

	_GetExpressionMode.push(_GetExpressionMode.top());
	{

		TypeSymbol TypeToStart = ExtendedFuncExpressionGetTypeToStart(ExpressionType, node);



		//Boring boiler plate for Tep ScopedNameNode
		const Token& ToGetLinesFrom = *node._Extended._FuncName._ScopedName.begin()->_token;

		ScopedNameNode Name;

		ScopedName TepV;


		Token TepToken;

		TepToken.OnLine = ToGetLinesFrom.OnLine;
		TepToken.OnPos = ToGetLinesFrom.OnPos;
		TepToken.Type = TokenType::Name;

		Type_RemoveTypeattributes(TypeToStart);

		String Buffer = ToString(TypeToStart);

		TepToken.Value._String = Buffer;

		TepV._token = &TepToken;


		TepV._operator = ScopedName::Operator_t::ScopeResolution;
		Name._ScopedName.push_back(std::move(TepV));

		{

			{
				auto Copy = node._Extended._FuncName._ScopedName.back();
				Name._ScopedName.push_back(std::move(Copy));
			}
			for (size_t i = 1; i < node._Extended._FuncName._ScopedName.size(); i++)
			{
				auto& Item = node._Extended._FuncName._ScopedName[i];
				auto Copy = Item;
				Name._ScopedName.push_back(std::move(Copy));
			}
		}
		//

		ValueParametersNode Pars;
		Pars._Nodes.push_back(Unique_ptr<Node>(node._Expression._Value.get()));

		for (size_t i = 0; i < node._Extended.Parameters._Nodes.size(); i++)
		{
			auto& Item = node._Extended.Parameters._Nodes[i];
			Pars._Nodes.push_back(Unique_ptr<Node>(Item.get()));
		}

		auto FuncInfo = Type_GetFunc(Name, Pars, Type_Get_LookingForType());


		Vector<EvaluatedEx> ValuePars;
		bool BadPars = false;

		if (Eval_CanEvalutateFuncCheck(FuncInfo))
		{
			if (FuncInfo.Func != nullptr)
			{

				ValuePars.resize(FuncInfo.Func->Pars.size());


				for (size_t i = 0; i < Pars._Nodes.size(); i++)
				{
					const TypeSymbol& Par = FuncInfo.Func->Pars[i].Type;
					auto& Item = Pars._Nodes[i];

					auto Info = Eval_Evaluate(Par, *Item.get());

					if (!Info.has_value())
					{
						BadPars = true;
					}

					ValuePars.push_back(std::move(Info.value()));
				}
			}
		}

		for (auto& Item : Pars._Nodes)
		{
			auto Node = Item.release();//is ok it was borrwed.
		}

		if (BadPars)
		{
			return false;
		}

		return Eval_EvalutateFunc(Out, FuncInfo, node._Extended._FuncName, ValuePars);

	}
	_GetExpressionMode.pop();
	return false;
}
bool SystematicAnalysis::Eval_EvalutateFunc(EvalFuncData& State, const NeverNullPtr<Symbol> Func, const Vector<EvaluatedEx>& Pars)
{



	bool Fail = false;
	auto Conxet = SaveAndMove_SymbolContext();

	const FuncInfo* funcInfo = Func->Get_Info<FuncInfo>();
	Set_SymbolContext(funcInfo->Context.value());
	{
		_Table._Scope.ThisScope = funcInfo->FullName;

		const FuncNode& Body = *Func->Get_NodeInfo<FuncNode>();

		State.Pars.reserve(Pars.size());
		for (size_t i = 0; i < Pars.size(); i++)
		{
			auto ID = Symbol_GetSymbolID(Body._Signature._Parameters._Parameters[i]);
			State.Pars.AddValue(ID, Pars[i].EvaluatedObject);
		}
		if (funcInfo->Ret._Type != TypesEnum::Void)
		{
			State.Ret.ObjectSize = Type_GetSize(funcInfo->Ret).value();
			State.Ret.Object_AsPointer.reset(new Byte[State.Ret.ObjectSize]);
		}
		State.FuncSyb = Func.value();

		{
			FuncStackInfo v = { (FuncInfo*)funcInfo };
			_FuncStack.push_back(std::move(v));
		}
		//
		_Table.AddScope("__EvalFunc");


		for (auto& Item : Body._Body.value()._Statements._Nodes)
		{
			auto Statement = Item.get();

			if (!Eval_EvalutateStatement(State, Statement))
			{
				Fail = true;
				break;
			}

		}

		{
			_FuncStack.pop_back();
		}

		_Table.RemoveScope();
	}

	Set_SymbolContext(std::move(Conxet));
	return !Fail;
}

bool SystematicAnalysis::EvalStore(EvalFuncData& State, const ExpressionNodeType& Storenode, EvaluatedEx& In)
{
	switch (Storenode._Value.get()->Get_Type())
	{
	case NodeType::ValueExpressionNode:
	{
		const ValueExpressionNode* v = ValueExpressionNode::As(Storenode._Value.get());

		switch (v->_Value.get()->Get_Type())
		{
		case NodeType::ReadVariableNode:
		{
			ReadVariableNode* nod = ReadVariableNode::As(v->_Value.get());

			String Scope;
			NullablePtr<Symbol> oldsyb;
			for (size_t i = 0; i < nod->_VariableName._ScopedName.size(); i++)
			{
				auto& item = nod->_VariableName._ScopedName[i];

				bool isstart = i == 0;
				bool islast = i == nod->_VariableName._ScopedName.size() - 1;

				if (oldsyb.has_value())
				{
					return false;
				}
				else
				{
					Scope += item._token->Value._String;

					auto syb = Symbol_GetSymbol(Scope, SymbolType::Any);

					if (syb.has_value())
					{
						if (islast)
						{
							auto Sym = syb.value();

							if (Sym->Type == SymbolType::Class_Field)
							{
								auto& frame = _Eval_FuncStackFrames.back();

								auto ThisSym = Symbol_GetSymbol(
									ScopeHelper::ApendedStrings(frame->FuncSyb->Get_Info<FuncInfo>()->FullName, ThisSymbolName)
									, SymbolType::ParameterVarable).value();


								size_t offset = 0;
								{
									auto P = ThisSym->VarType;
									P._IsAddress = false;

									auto v = Symbol_GetSymbol(P._CustomTypeSymbol);

									String_view FieldName = item._token->Value._String;
									if (v->Type == SymbolType::Tag_class)
									{
										TagInfo* pp = v->Get_Info<TagInfo>();

										size_t tepoffset = 0;

										for (size_t i = 0; i < pp->GetFieldIndex(FieldName).value(); i++)
										{
											auto& field = pp->Fields[i];
											tepoffset += Type_GetSize(field.Type).value_or(0);
										}

										offset = tepoffset;
									}
									else
									{
										UCodeLangUnreachable();
									}
								}

								auto& Par = frame->Pars.GetValue(ThisSym->ID);

								auto p = *Eval_Get_ObjectAs<EvalPointer>(ThisSym->VarType, Par);


								GetSharedEval().value()->PointerWrite(p, offset, In.EvaluatedObject);

								return true;
							}
							else
							{
								return false;
							}
						}
						else
						{
							return false;
						}
					}
					else
					{
						return false;
					}

				}
			}
		}
		break;
		default:
			return false;
			break;
		}

	}break;
	default:
		break;
	}
	return false;
}
bool SystematicAnalysis::Eval_EvalutateStatement(EvalFuncData& State, const Node* node)
{
	switch (node->Get_Type())
	{
	case NodeType::RetStatementNode:
	{
		const RetStatementNode* Node = RetStatementNode::As(node);
		auto Val = Eval_Evaluate(State.Get_funcInfo()->Ret, Node->_Expression);
		if (Val.has_value()) {
			State.Ret = std::move(Val.value().EvaluatedObject);
		}
		return Val.has_value();
	}
	case NodeType::AssignExpressionNode:
	{
		const AssignExpressionNode* Node = AssignExpressionNode::As(node);
		auto& assign = Node->_ToAssign;


		_LookingForTypes.push(TypesEnum::Any);
		OnExpressionTypeNode(assign, GetValueMode::Write);
		_LookingForTypes.pop();

		auto StoreType = _LastExpressionType;

		_LookingForTypes.push(std::move(StoreType));
		auto ex = Eval_Evaluate(StoreType, Node->_Expression);
		_LookingForTypes.pop();

		if (!ex.has_value())
		{
			return false;
		}

		return EvalStore(State, assign, ex.value());
	}
	break;
	default:
		return false;
		UCodeLangUnreachable();
		break;
	}

}
bool SystematicAnalysis::Eval_EvalutateScopedName(EvaluatedEx& Out, size_t Start, size_t End, const ScopedNameNode& node, GetMemberTypeSymbolFromVar_t& OtherOut)
{
	_GetExpressionMode.push(GetValueMode::Read);
	GetMemberTypeSymbolFromVar_t V;
	auto RetBool = Symbol_MemberTypeSymbolFromVar(Start, End, node, V);
	_GetExpressionMode.pop();

	_LastExpressionType = V.Type;

	if (V._Symbol)
	{
		if (V._Symbol->Type == SymbolType::ConstantExpression)
		{
			ConstantExpressionInfo* Info = V._Symbol->Get_Info<ConstantExpressionInfo>();
			Out.EvaluatedObject = Info->Ex;
			Out.Type = V.Type;
			return true;
		}
		else if (V._Symbol->Type == SymbolType::ParameterVarable)
		{
			if (_Eval_FuncStackFrames.size())
			{
				auto& StatckFrame = _Eval_FuncStackFrames.back();

				if (StatckFrame->Pars.HasValue(V._Symbol->ID))
				{
					auto& par = StatckFrame->Pars.GetValue(V._Symbol->ID);

					Out.EvaluatedObject = par;
					return true;
				}

			}

		}
		else if (V._Symbol->Type == SymbolType::Enum_Field)
		{
			const String FieldName = ScopeHelper::GetNameFromFullName(V._Symbol->FullName);
			String enumsybfullname = V._Symbol->FullName;
			ScopeHelper::ReMoveScope(enumsybfullname);

			auto v = Symbol_GetSymbol(enumsybfullname, SymbolType::Enum).value();
			EnumInfo* info = v->Get_Info<EnumInfo>();

			auto field = info->GetField(FieldName).value();

			Out.EvaluatedObject = field->Ex;
			Out.Type = V.Type;
			return true;
		}

	}
	return false;
}
Optional<SystematicAnalysis::EvaluatedEx> SystematicAnalysis::Eval_EvaluateToAnyType(const ExpressionNodeType& node)
{
	EvaluatedEx Out;
	bool V = Eval_EvaluateToAnyType(Out, node);

	if (V)
	{
		return { Out };
	}
	else
	{
		return {};
	}
}
void SystematicAnalysis::CompileTimeforNodeEvaluateStatements(const CompileTimeForNode& node)
{
	auto TepPass = _PassType;
	size_t OldErrCount = _ErrorsOutput->Get_Errors().size();

	_PassType = PassType::GetTypes;
	for (const auto& node2 : node._Body._Nodes)
	{
		OnStatement(*node2);
	}

	bool GotErrs = OldErrCount != _ErrorsOutput->Get_Errors().size();
	if (!GotErrs)
	{
		_PassType = PassType::FixedTypes;
		for (const auto& node2 : node._Body._Nodes)
		{
			OnStatement(*node2);
		}

	}
	_PassType = TepPass;
}
constexpr size_t BuiltInCount = (size_t)Systematic_BuiltInFunctions::ID::Max - 1;



static const Array<Systematic_BuiltInFunctions::FunctionData, BuiltInCount> BuiltFuncList =
{
	 Systematic_BuiltInFunctions::FunctionData("Name",Systematic_BuiltInFunctions::ID::TypeInfo_GetName),
	 Systematic_BuiltInFunctions::FunctionData("FullName",Systematic_BuiltInFunctions::ID::TypeInfo_GetFullName),
};
Optional<Systematic_BuiltInFunctions::Func> Systematic_BuiltInFunctions::GetFunction(const String_view Name, const Vector<FunctionPar>& Pars, SystematicAnalysis& This)
{
	auto FuncName = ScopeHelper::GetNameFromFullName(Name);

	if ((FuncName == "Name" || FuncName == "FullName") && Pars.size() == 1)
	{
		auto& Type = Pars.front();
		if (Type.IsOutPar == false)
		{

			bool WantsimutCString = false;
			if (This._LookingForTypes.size())
			{
				auto Type = This._LookingForTypes.top();
				WantsimutCString = Type._Type == TypesEnum::Char && Type._IsAddressArray && Type._Isimmutable;
			}

			if (Type.Type._TypeInfo == TypeInfoPrimitive::TypeInfo)
			{
				TypeSymbol NewType;
				NewType._CustomTypeSymbol = Type.Type._CustomTypeSymbol;
				NewType._Type = Type.Type._Type;




				String Value = This.ToString(NewType);
				if (FuncName == "Name")
				{
					Value = ScopeHelper::GetNameFromFullName(FuncName);
				}

				Func _Func;
				_Func.RetType = This.Type_GetStaticArrayType(TypesEnum::Char, Value.size());
				auto Ex = This.Eval_MakeEx(_Func.RetType);
				This.Eval_Set_ObjectAs(Ex, Value.data(), Value.size());


				_Func.EvalObject = std::move(Ex.EvaluatedObject);
				_Func.EvalAsCString = WantsimutCString;
				return _Func;
			}
			else if (FuncName == "Name" && Type.Type._TypeInfo == TypeInfoPrimitive::ClassFieldInfo && Type.ExpressionNode)
			{
				auto EvalObject = This.Eval_EvaluateToAnyType(*ExpressionNodeType::As(Type.ExpressionNode));
				if (EvalObject.has_value())
				{
					auto& EvalObjectAsValue = EvalObject.value();
					const ClassField& Field = *This.Eval_Get_ObjectAs<const ClassField>(EvalObjectAsValue);

					const String& Value = Field.Field->Name;

					Func _Func;
					_Func.RetType = This.Type_GetStaticArrayType(TypesEnum::Char, Value.size());
					auto Ex = This.Eval_MakeEx(_Func.RetType);
					This.Eval_Set_ObjectAs(Ex, Value.data(), Value.size());

					_Func.EvalObject = std::move(Ex.EvaluatedObject);
					_Func.EvalAsCString = WantsimutCString;
					return _Func;
				}
				else
				{
					Func _Func;
					_Func.RetType = TypesEnum::Null;
					return _Func;
				}

			}
			else if (FuncName == "Name" && Type.Type._TypeInfo == TypeInfoPrimitive::EnumFieldInfo && Type.ExpressionNode)
			{
				auto EvalObject = This.Eval_EvaluateToAnyType(*ExpressionNodeType::As(Type.ExpressionNode));
				if (EvalObject.has_value())
				{
					auto& EvalObjectAsValue = EvalObject.value();
					const EnumField& Field = *This.Eval_Get_ObjectAs<const EnumField>(EvalObjectAsValue);

					const String& Value = Field.Field->Name;

					Func _Func;
					_Func.RetType = This.Type_GetStaticArrayType(TypesEnum::Char, Value.size());
					auto Ex = This.Eval_MakeEx(_Func.RetType);
					This.Eval_Set_ObjectAs(Ex, Value.data(), Value.size());

					_Func.EvalObject = std::move(Ex.EvaluatedObject);
					_Func.EvalAsCString = WantsimutCString;
					return _Func;
				}
				else
				{
					Func _Func;
					_Func.RetType = TypesEnum::Null;
					return _Func;
				}

			}

		}
	}
	if (FuncName == "Value" && Pars.size() == 1)
	{
		auto& Type = Pars.front();
		if (Type.IsOutPar == false)
		{
			if (Type.Type._TypeInfo == TypeInfoPrimitive::EnumFieldInfo && Type.ExpressionNode)
			{
				auto EvalObject = This.Eval_EvaluateToAnyType(*ExpressionNodeType::As(Type.ExpressionNode));
				if (EvalObject.has_value())
				{
					auto& EvalObjectAsValue = EvalObject.value();
					const EnumField& Field = *This.Eval_Get_ObjectAs<const EnumField>(EvalObjectAsValue);

					const auto& Value = Field.Field->Ex;

					Func _Func;
					_Func.RetType = Field._EnumInfo->Basetype;
					auto Ex = This.Eval_MakeEx(_Func.RetType);
					This.Eval_Set_ObjectAs(Ex, Value.Object_AsPointer.get(), Value.ObjectSize);

					_Func.EvalObject = std::move(Ex.EvaluatedObject);
					return _Func;
				}
				else
				{
					Func _Func;
					_Func.RetType = TypesEnum::Null;
					return _Func;
				}

			}
		}
	}
	if (FuncName == "Offset" && Pars.size() == 1)
	{
		auto& Type = Pars.front();
		if (Type.IsOutPar == false)
		{
			if (Type.Type._TypeInfo == TypeInfoPrimitive::ClassFieldInfo && Type.ExpressionNode)
			{
				auto EvalObject = This.Eval_EvaluateToAnyType(*ExpressionNodeType::As(Type.ExpressionNode));
				if (EvalObject.has_value())
				{
					auto& EvalObjectAsValue = EvalObject.value();
					const ClassField& Field = *This.Eval_Get_ObjectAs<const ClassField>(EvalObjectAsValue);

					auto OffsetInfo = This.Type_GetOffset(*Field._ClassInfo, Field.Field);

					const size_t& Value = OffsetInfo.value();

					Func _Func;
					_Func.RetType = TypesEnum::uIntPtr;
					auto Ex = This.Eval_MakeEx(_Func.RetType);
					This.Eval_Set_ObjectAs(Ex, Value);

					_Func.EvalObject = std::move(Ex.EvaluatedObject);
					return _Func;
				}
				else
				{
					Func _Func;
					_Func.RetType = TypesEnum::Null;
					return _Func;
				}

			}

		}
	}

	if (FuncName == "Type" && Pars.size() == 1)
	{
		auto& Type = Pars.front();
		if (Type.IsOutPar == false)
		{
			if (Type.Type._TypeInfo == TypeInfoPrimitive::ClassFieldInfo && Type.ExpressionNode)
			{
				auto EvalObject = This.Eval_EvaluateToAnyType(*ExpressionNodeType::As(Type.ExpressionNode));
				if (EvalObject.has_value())
				{
					auto& EvalObjectAsValue = EvalObject.value();
					const ClassField& Field = *This.Eval_Get_ObjectAs<ClassField>(EvalObjectAsValue);

					const TypeSymbol& Value = Field.Field->Type;

					Func _Func;
					_Func.RetType = Value;
					_Func.RetType.SetAsTypeInfo();
					auto Ex = This.Eval_MakeEx(_Func.RetType);
					This.Eval_Set_ObjectAs(Ex, Value);

					_Func.EvalObject = std::move(Ex.EvaluatedObject);
					return _Func;
				}
				else
				{
					Func _Func;
					_Func.RetType = TypesEnum::Null;
					return _Func;
				}

			}

		}
	}
	if (FuncName == "TypeID" && Pars.size() == 1)
	{
		auto& Type = Pars.front();
		if (Type.IsOutPar == false)
		{
			if (Type.Type._TypeInfo == TypeInfoPrimitive::TypeInfo)
			{
				TypeSymbol NewType = Type.Type;
				NewType.BindType();

				auto Value = This.Type_GetTypeID(NewType._Type, NewType._CustomTypeSymbol);

				if (This._Settings->PtrSize == IntSizes::Int32)
				{
					Value = (UInt32)Value;
				}
				else
				{
					Value = (UInt64)Value;
				}

				Func _Func;
				_Func.RetType = TypesEnum::uIntPtr;
				auto Ex = This.Eval_MakeEx(_Func.RetType);
				memcpy(Ex.EvaluatedObject.Object_AsPointer.get(), &Value, Ex.EvaluatedObject.ObjectSize);

				_Func.EvalObject = std::move(Ex.EvaluatedObject);
				return _Func;
			}
		}

	}

	if (Pars.size() == 1)
	{
		auto& Type = Pars.front();
		if (Type.IsOutPar == false
			&& Type.Type._TypeInfo == TypeInfoPrimitive::TypeInfo)
		{
			Optional<bool> Value;

			TypeSymbol NewType = Type.Type;
			if (FuncName == "IsAddress")
			{
				Value = NewType.IsAddress();
			}
			else if (FuncName == "IsAddressArray")
			{
				Value = NewType.IsAddressArray();
			}
			else if (FuncName == "IsImmutable")
			{
				Value = NewType.Isimmutable();
			}
			else if (FuncName == "IsDynamic")
			{
				Value = NewType.IsDynamicTrait();
			}
			else if (FuncName == "IsMoved")
			{
				Value = NewType.IsMovedType();
			}


			if (Value.has_value())
			{
				Func _Func;
				_Func.RetType = TypesEnum::Bool;
				auto Ex = This.Eval_MakeEx(_Func.RetType);
				This.Eval_Set_ObjectAs(Ex, Value.value());

				_Func.EvalObject = std::move(Ex.EvaluatedObject);
				return _Func;
			}
		}
	}


	if (FuncName == "BaseType")
	{
		if (Pars.size() == 1)
		{
			auto& Par = Pars.front();
			if (Par.IsOutPar == false)
			{
				if (Par.Type._TypeInfo == TypeInfoPrimitive::EnumInfo)
				{
					const auto Sym = This.Symbol_GetSymbol(Par.Type).value();
					const auto classInfo = Sym->Get_Info<EnumInfo>();
					auto BaseType = classInfo->Basetype;

					Func _Func;
					_Func.RetType = BaseType;
					_Func.RetType.SetAsTypeInfo();
					auto Ex = This.Eval_MakeEx(_Func.RetType);
					This.Eval_Set_ObjectAs(Ex, BaseType);

					_Func.EvalObject = std::move(Ex.EvaluatedObject);

					return _Func;
				}
			}
		}
	}

	if (FuncName == "TryGetVariantUnion")
	{
		if (Pars.size() == 1)
		{
			auto& Par = Pars.front();
			if (Par.IsOutPar == false)
			{
				if (Par.Type._TypeInfo == TypeInfoPrimitive::EnumInfo)
				{
					const auto Sym = This.Symbol_GetSymbol(Par.Type).value();
					const auto classInfo = Sym->Get_Info<EnumInfo>();
					bool IsVariant = classInfo->VariantData.has_value();


					Func _Func;
					_Func.RetType = TypesEnum::Bool;
					_Func.RetType.SetAsTypeInfo();
					auto Ex = This.Eval_MakeEx(_Func.RetType);
					This.Eval_Set_ObjectAs(Ex, IsVariant);

					_Func.EvalObject = std::move(Ex.EvaluatedObject);

					{
						Func::OutParData Par;
						if (IsVariant)
						{
							const auto& Variant = classInfo->VariantData.value();


							Par.Type = TypesEnum::InternalType;
							Par.Type._TypeInfo = TypeInfoPrimitive::EnumVariantInfo;

							auto Ex = This.Eval_MakeEx(Par.Type);
							This.Eval_Set_ObjectAs(Ex, &Variant);

							Par.EvalObject = std::move(Ex.EvaluatedObject);
						}
						_Func._OutPars.push_back(std::move(Par));
					}
					return _Func;
				}
			}
		}
	}


	if (Pars.size() == 2)
	{
		auto& Type = Pars.front();

		bool IsTypeInfo = Type.Type._TypeInfo == TypeInfoPrimitive::TypeInfo;
		bool OtherIsOutVal = Pars[1].IsOutPar == true && IsTypeInfo;

		if (OtherIsOutVal)
		{
			if (FuncName == "GetClassInfo")
			{
				const auto Sym = This.Symbol_GetSymbol(Type.Type);
				bool IsClass = Sym ? Sym.value()->Type == SymbolType::Type_class : false;

				Func _Func;
				_Func.RetType = TypesEnum::Bool;
				auto Ex = This.Eval_MakeEx(_Func.RetType);
				This.Eval_Set_ObjectAs(Ex, IsClass);

				_Func.EvalObject = std::move(Ex.EvaluatedObject);

				{
					Func::OutParData Par;
					if (IsClass)
					{
						Par.Type = Type.Type;
						Par.Type._IsAddress = false;//If Pass as Ref
						Par.Type._TypeInfo = TypeInfoPrimitive::ClassInfo;

						auto Ex = This.Eval_MakeEx(Par.Type);
						This.Eval_Set_ObjectAs(Ex, Par.Type);

						Par.EvalObject = std::move(Ex.EvaluatedObject);
					}
					_Func._OutPars.push_back(std::move(Par));
				}
				return _Func;
			}
			else if (FuncName == "GetEnumInfo")
			{
				const auto Sym = This.Symbol_GetSymbol(Type.Type);
				bool IsEnum = Sym.has_value() ? Sym.value()->Type == SymbolType::Enum : false;

				Func _Func;
				_Func.RetType = TypesEnum::Bool;
				auto Ex = This.Eval_MakeEx(_Func.RetType);
				This.Eval_Set_ObjectAs(Ex, IsEnum);

				_Func.EvalObject = std::move(Ex.EvaluatedObject);

				{
					Func::OutParData Par;
					if (IsEnum)
					{
						Par.Type = Type.Type;
						Par.Type._IsAddress = false;//If Pass as Ref
						Par.Type._TypeInfo = TypeInfoPrimitive::EnumInfo;

						auto Ex = This.Eval_MakeEx(Par.Type);
						This.Eval_Set_ObjectAs(Ex, Par.Type);

						Par.EvalObject = std::move(Ex.EvaluatedObject);
					}
					_Func._OutPars.push_back(std::move(Par));
				}
				return _Func;
			}
			else if (FuncName == "GetFuncionPointerInfo")
			{
				const auto Sym = This.Symbol_GetSymbol(Type.Type);
				bool IsFuncPointer = Sym.has_value() ? Sym.value()->Type == SymbolType::Func_ptr : false;

				Func _Func;
				_Func.RetType = TypesEnum::Bool;
				auto Ex = This.Eval_MakeEx(_Func.RetType);
				This.Eval_Set_ObjectAs(Ex, IsFuncPointer);

				_Func.EvalObject = std::move(Ex.EvaluatedObject);

				{
					Func::OutParData Par;
					if (IsFuncPointer)
					{
						Par.Type = Type.Type;
						Par.Type._IsAddress = false;//If Pass as Ref
						Par.Type._TypeInfo = TypeInfoPrimitive::FuncPtrInfo;

						auto Ex = This.Eval_MakeEx(Par.Type);
						This.Eval_Set_ObjectAs(Ex, Par.Type);

						Par.EvalObject = std::move(Ex.EvaluatedObject);
					}
					_Func._OutPars.push_back(std::move(Par));
				}
				return _Func;
			}
		}

	}


	if (Pars.size() == 1 && FuncName == "IsDynmaicTrait")
	{
		auto& Type = Pars.front();
		const auto Sym = This.Symbol_GetSymbol(Type.Type);
		bool IsDynTrait = Type.Type.IsDynamicTrait();

		Func _Func;
		_Func.RetType = TypesEnum::Bool;
		auto Ex = This.Eval_MakeEx(_Func.RetType);
		This.Eval_Set_ObjectAs(Ex, IsDynTrait);

		_Func.EvalObject = std::move(Ex.EvaluatedObject);

		return _Func;
	}
	if (FuncName == "Fields" && Pars.size() == 1)
	{
		auto& Type = Pars.front();
		bool IsTypeInfo = Type.Type._TypeInfo == TypeInfoPrimitive::TypeInfo;
		switch (Type.Type._TypeInfo)
		{
		case TypeInfoPrimitive::ClassInfo:
		{
			const auto Sym = This.Symbol_GetSymbol(Type.Type).value();
			const auto classInfo = Sym->Get_Info<ClassInfo>();

			const auto& Fields = classInfo->Fields;


			Vector<ClassField> FieldsAs;
			FieldsAs.resize(Fields.size());
			for (size_t i = 0; i < Fields.size(); i++)
			{
				FieldsAs[i].Field = &Fields[i];
				FieldsAs[i]._ClassInfo = classInfo;
			}

			TypeSymbol ArrItemType = TypesEnum::InternalType;
			ArrItemType._TypeInfo = TypeInfoPrimitive::ClassFieldInfo;

			Func _Func;
			_Func.RetType = This.Type_GetStaticArrayType(ArrItemType, FieldsAs.size());

			auto Ex = This.Eval_MakeEx(_Func.RetType);
			This.Eval_Set_ObjectAs(Ex, FieldsAs.data(), FieldsAs.size() * sizeof(ClassField));
			_Func.EvalObject = std::move(Ex.EvaluatedObject);

			return _Func;
		}
		break;
		case TypeInfoPrimitive::EnumInfo:
		{
			const auto Sym = This.Symbol_GetSymbol(Type.Type).value();
			const auto enumInfo = Sym->Get_Info<EnumInfo>();

			const auto& Fields = enumInfo->Fields;


			Vector<EnumField> FieldsAs;
			FieldsAs.resize(Fields.size());
			for (size_t i = 0; i < Fields.size(); i++)
			{
				FieldsAs[i].Field = &Fields[i];
				FieldsAs[i]._EnumInfo = enumInfo;
			}

			TypeSymbol ArrItemType = TypesEnum::InternalType;
			ArrItemType._TypeInfo = TypeInfoPrimitive::EnumFieldInfo;

			Func _Func;
			_Func.RetType = This.Type_GetStaticArrayType(ArrItemType, FieldsAs.size());

			auto Ex = This.Eval_MakeEx(_Func.RetType);
			This.Eval_Set_ObjectAs(Ex, FieldsAs.data(), FieldsAs.size() * sizeof(EnumField));
			_Func.EvalObject = std::move(Ex.EvaluatedObject);

			return _Func;
		}
		break;
		default:
			break;
		}
	}

	return {};
}

UCodeLangFrontEnd

#endif