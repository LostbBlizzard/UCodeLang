#include "UCodeLang/Compliation/Front/SystematicAnalysis.hpp"
#include "UCodeLang/Compliation/Helpers/ParseHelper.hpp"
UCodeLangFrontStart
bool SystematicAnalysis::Type_IsStringSpan8(const TypeSymbol& type)
{
	auto symOp = Symbol_GetSymbol(type);
	if (symOp.has_value())
	{
		auto sym = symOp.value();
		auto& name = sym->FullName;
		return StringHelper::Contains(name, Uft8typeName)
			|| StringHelper::Contains(name, UCodeLangStringSpan8Type);

	}
	return false;
}

bool SystematicAnalysis::Type_IsStringSpan16(const TypeSymbol& type)
{
	auto symOp = Symbol_GetSymbol(type);
	if (symOp.has_value())
	{
		auto sym = symOp.value();
		auto& name = sym->FullName;
		return StringHelper::Contains(name, Uft16typeName)
			|| StringHelper::Contains(name, UCodeLangStringSpan16Type);
	}
	return false;
}

bool SystematicAnalysis::Type_IsStringSpan32(const TypeSymbol& type)
{
	auto symOp = Symbol_GetSymbol(type);
	if (symOp.has_value())
	{
		auto sym = symOp.value();
		const auto& name = sym->FullName;
		return StringHelper::Contains(name, Uft32typeName)
			|| StringHelper::Contains(name, UCodeLangStringSpan32Type);
	}
	return false;
}

bool SystematicAnalysis::Type_IsString8(const TypeSymbol& type)
{
	auto symOp = Symbol_GetSymbol(type);
	if (symOp.has_value())
	{
		auto sym = symOp.value();
		const auto& name = sym->FullName;
		return StringHelper::Contains(name, Uft8typeName)
			|| StringHelper::Contains(name, UCodeLangString8Type);
	}
	return false;
}

bool SystematicAnalysis::Type_IsString16(const TypeSymbol& type)
{
	auto symOp = Symbol_GetSymbol(type);
	if (symOp.has_value())
	{
		auto sym = symOp.value();
		const auto& name = sym->FullName;
		return StringHelper::Contains(name, Uft16typeName)
			|| StringHelper::Contains(name, UCodeLangString16Type);
	}
	return false;
}

bool SystematicAnalysis::Type_IsString32(const TypeSymbol& type)
{
	auto symOp = Symbol_GetSymbol(type);
	if (symOp.has_value())
	{
		auto sym = symOp.value();
		const auto& name = sym->FullName;
		return StringHelper::Contains(name, Uft32typeName)
			|| StringHelper::Contains(name, UCodeLangString32Type);
	}
	return false;
}

Optional<TypeSymbol> SystematicAnalysis::Type_GetStringSpan(const NeverNullPtr<Token> Token)
{
	if (_PassType != PassType::GetTypes)
	{
		auto symOp = Symbol_GetSymbol(UCodeLangStringSpanType, SymbolType::Type_class);
		if (symOp)
		{
			auto sym = symOp.value();
			Symbol_Update_Sym_ToFixedTypes(sym);
			return sym->VarType;
		}
		else
		{
			LogError_CantFindTypeError(Token, UCodeLangStringSpanType);
			return {};
		}


	}
	return {};
}

Optional<TypeSymbol> SystematicAnalysis::Type_GetStringSpan8(const NeverNullPtr<Token> Token)
{
	if (_PassType != PassType::GetTypes)
	{
		auto symOp = Symbol_GetSymbol(UCodeLangStringSpan8Type, SymbolType::Type_class);
		if (symOp)
		{
			auto sym = symOp.value();
			return sym->VarType;
		}
		else
		{
			LogError_CantFindTypeError(Token, UCodeLangStringSpan8Type);
			return {};
		}


	}
	return {};
}

Optional<TypeSymbol> SystematicAnalysis::Type_GetStringSpan16(const NeverNullPtr<Token> Token)
{
	if (_PassType != PassType::GetTypes)
	{
		auto symOp = Symbol_GetSymbol(UCodeLangStringSpan16Type, SymbolType::Type_class);
		if (symOp)
		{
			auto sym = symOp.value();
			return sym->VarType;
		}
		else
		{
			LogError_CantFindTypeError(Token, UCodeLangStringSpan16Type);
			return {};
		}


	}
	return {};
}

Optional<TypeSymbol> SystematicAnalysis::Type_GetStringSpan32(const NeverNullPtr<Token> Token)
{
	if (_PassType != PassType::GetTypes) {
		auto symOp = Symbol_GetSymbol(UCodeLangStringSpan32Type, SymbolType::Type_class);
		if (symOp)
		{
			auto sym = symOp.value();
			return sym->VarType;
		}
		else
		{
			LogError_CantFindTypeError(Token, UCodeLangStringSpan32Type);
			return {};
		}


	}
	return {};
}

void SystematicAnalysis::OnStringLiteral(const StringliteralNode* nod, bool& retflag)
{
	retflag = true;



	auto& Type = Type_Get_LookingForType();
	enum class StringType
	{
		Char,
		Utf8,
		Utf16,
		Utf32,
	};
	StringType stringtype = StringType::Char;

	if (Type_IsStringSpan8(Type) || Type_IsString8(Type))
	{
		stringtype = StringType::Utf8;
	}
	else if (Type_IsStringSpan16(Type) || Type_IsString16(Type))
	{
		stringtype = StringType::Utf16;
	}
	else if (Type_IsStringSpan32(Type) || Type_IsString32(Type))
	{
		stringtype = StringType::Utf32;
	}
	else
	{
		stringtype = StringType::Char;
	}

	if (_PassType == PassType::FixedTypes)
	{
		//TODO check for valid String,String8,String16,String32

		TypeSymbol CharType;
		TypeSymbol SpanStringType;
		auto Token = NeverNullptr(nod->token);
		switch (stringtype)
		{
		case StringType::Char:
		{
			CharType = TypeSymbol(TypesEnum::Char);
			SpanStringType = Type_GetStringSpan(Token).value_or(TypeSymbol(TypesEnum::Null));
		}
		break;
		case StringType::Utf8:
		{
			CharType = TypeSymbol(TypesEnum::Uft8);
			SpanStringType = Type_GetStringSpan8(Token).value_or(TypeSymbol(TypesEnum::Null));
		}
		break;
		case StringType::Utf16:
		{
			CharType = TypeSymbol(TypesEnum::Uft16);
			SpanStringType = Type_GetStringSpan16(Token).value_or(TypeSymbol(TypesEnum::Null));
		}
		break;
		case StringType::Utf32:
		{
			CharType = TypeSymbol(TypesEnum::Uft32);
			SpanStringType = Type_GetStringSpan32(Token).value_or(TypeSymbol(TypesEnum::Null));
		}
		break;
		default:
			UCodeLangUnreachable();
			break;
		}

		if (!SpanStringType.IsBadType())
		{
			FuncInfo* func = nullptr;
			{
				String scope = ScopeHelper::ApendedStrings(ToString(SpanStringType), ClassConstructorfunc);

				auto list = GetSymbolsWithName(scope);
				for (auto& Item : list)
				{
					if (Item->Type == SymbolType::Func)
					{
						FuncInfo* finfo = Item->Get_Info<FuncInfo>();
						if (finfo->Pars.size() == 3)
						{
							auto& pointerpar = finfo->Pars[1];
							auto& sizepar = finfo->Pars[2];
							if (pointerpar.IsOutPar == true
								|| sizepar.IsOutPar == true)
							{
								continue;
							}
							if (pointerpar.Type._Type != CharType._Type)
							{
								continue;
							}
							if (!pointerpar.Type.IsAddressArray())
							{
								continue;
							}
							if (sizepar.Type.IsAddress()
								|| sizepar.Type.IsAddressArray()
								|| sizepar.Type._Type != TypesEnum::uIntPtr)
							{
								continue;
							}
							func = finfo;
							break;
						}
					}
				}
			}
			if (func == nullptr)
			{
				ParInfo par0;
				par0.Type = TypeSymbol(SpanStringType._CustomTypeSymbol);
				par0.Type.SetAsAddress();

				ParInfo par1;
				par1.Type = CharType;
				par1.Type.SetAsimmutable();
				par1.Type.SetAsAddressArray();

				ParInfo par2;
				par2.Type = TypesEnum::uIntPtr;
				LogError_CantFindFuncError(Token, "new"
					, {}, { par0,par1,par2 }, TypeSymbol(TypesEnum::Void));
			}
		}

		SpanStringType.SetAsimmutable();
		_LastExpressionType = SpanStringType;
	}

	bool IsStaticArr = Type_IsStaticCharArr(Type);



	if (IsStaticArr)
	{
		switch (stringtype)
		{
		case StringType::Char:
		{

		}
		break;
		case StringType::Utf8:
		{
			UCodeLangToDo();//"add utf8"
		}
		break;
		case StringType::Utf16:
		{

			UCodeLangToDo();//"add utf16"
		}
		break;
		case StringType::Utf32:
		{

			UCodeLangToDo();//"add utf32"
		}
		break;
		default:
			UCodeLangUnreachable();
			break;
		}
		auto V = Symbol_GetSymbol(Type).value();
		StaticArrayInfo* StaticArr = V->Get_Info<StaticArrayInfo>();

		if (_PassType == PassType::FixedTypes)
		{
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
					LogError_CanIncorrectStaticArrCount(Token, Type, BufferSize, StaticArr->Count);
					_LastExpressionType.SetType(TypesEnum::Null);
					return;
				}
			}
		}

		if (_PassType == PassType::BuidCode)
		{
			String V;
			bool ItWorked = !ParseHelper::ParseStringliteralToString(nod->token->Value._String, V);


			Debug_Add_SetLineNumber(NeverNullptr(nod->token), _IR_LookingAtIRBlock->Instructions.size());

			auto& BufferIR = _IR_IRlocations.top();
			BufferIR.UsedlocationIR = true;
			auto BufferIRIns = BufferIR.Value;

			const auto& ArrItemType = StaticArr->Type;
			const auto IRItemType = IR_ConvertToIRType(ArrItemType);
			UAddress Size;
			Type_GetSize(ArrItemType, Size);
			auto ValueSizeIR = IR_Load_UIntptr(Size);

			if (!Type.IsAddress())
			{
				BufferIRIns = _IR_LookingAtIRBlock->NewLoadPtr(BufferIRIns);
			}

			for (size_t i = 0; i < V.size(); i++)
			{
				auto VIR = _IR_LookingAtIRBlock->NewLoad(V[i]);


				auto f = _IR_LookingAtIRBlock->New_Index_Vetor(BufferIRIns, IR_Load_UIntptr(i), ValueSizeIR);

				_IR_LookingAtIRBlock->NewDereferenc_Store(f, VIR);
			}


		}

		_LastExpressionType = Type;
	}
	else
	{
		auto Token = NeverNullptr(nod->token);
		TypeSymbol SpanStringType;
		switch (stringtype)
		{
		case StringType::Char:
		{
			SpanStringType = Type_GetStringSpan(Token).value_or(TypesEnum::Null);
		}
		break;
		case StringType::Utf8:
		{
			SpanStringType = Type_GetStringSpan8(Token).value_or(TypesEnum::Null);
		}
		break;
		case StringType::Utf16:
		{
			SpanStringType = Type_GetStringSpan16(Token).value_or(TypesEnum::Null);
		}
		break;
		case StringType::Utf32:
		{
			SpanStringType = Type_GetStringSpan32(Token).value_or(TypesEnum::Null);
		}
		break;
		default:
			UCodeLangUnreachable();
			break;
		}
		SpanStringType.SetAsimmutable();
		_LastExpressionType = SpanStringType;

		if (_PassType == PassType::BuidCode)
		{
			IRBuilder::StringSpanInfo Span;
			size_t SpanSize;
			switch (stringtype)
			{
			case StringType::Char:
			{
				String V;
				bool ItWorked = !ParseHelper::ParseStringliteralToString(nod->token->Value._String, V);
				Span = _IR_Builder.FindOrAddStaticSpanString((String_view)V);
				SpanSize = V.size();
			}
			break;
			case StringType::Utf8:
			{
				String8 V;
				bool ItWorked = !ParseHelper::ParseStringliteralToString(nod->token->Value._String, V);
				Span = _IR_Builder.FindOrAddStaticSpanString8((String_view8)V);
				SpanSize = V.size();
			}
			break;
			case StringType::Utf16:
			{
				String16 V;
				bool ItWorked = !ParseHelper::ParseStringliteralToString(nod->token->Value._String, V);
				Span = _IR_Builder.FindOrAddStaticSpanString16((String_view16)V);
				SpanSize = V.size();
			}
			break;
			case StringType::Utf32:
			{
				String32 V;
				bool ItWorked = !ParseHelper::ParseStringliteralToString(nod->token->Value._String, V);
				Span = _IR_Builder.FindOrAddStaticSpanString8((String_view32)V);
				SpanSize = V.size();
			}
			break;
			default:
				UCodeLangUnreachable();
				break;
			}

			TypeSymbol spantype = SpanStringType;
			auto irpointer = _IR_LookingAtIRBlock->NewLoadPtr(Span.StaticVar);
			if (Span.Offset != 0)
			{
				size_t mult = 0;
				switch (stringtype)
				{
				case StringType::Char:
				{
					mult = 1;
				}
				break;
				case StringType::Utf8:
				{
					mult = 1;
				}
				break;
				case StringType::Utf16:
				{
					mult = 2;
				}
				break;
				case StringType::Utf32:
				{
					mult = 4;
				}
				break;
				default:
					UCodeLangUnreachable();
					break;
				}
				irpointer = _IR_LookingAtIRBlock->NewAdd(IR_Load_UIntptr(mult), irpointer);
			}
			auto irsize = IR_Load_UIntptr(SpanSize);
			IRInstruction* irspan = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(SpanStringType));

			FuncInfo* func = nullptr;

			{
				spantype._Isimmutable = false;
				String scope = ScopeHelper::ApendedStrings(ToString(spantype), ClassConstructorfunc);
				spantype.SetAsimmutable();

				auto list = GetSymbolsWithName(scope);
				for (auto& Item : list)
				{
					if (Item->Type == SymbolType::Func)
					{
						FuncInfo* finfo = Item->Get_Info<FuncInfo>();
						if (finfo->Pars.size() == 3)
						{
							func = finfo;
							break;
						}
					}
				}
			}
			_IR_LookingAtIRBlock->NewPushParameter(_IR_LookingAtIRBlock->NewLoadPtr(irspan));
			_IR_LookingAtIRBlock->NewPushParameter(irpointer);
			_IR_LookingAtIRBlock->NewPushParameter(irsize);
			_IR_LookingAtIRBlock->NewCall(IR_GetIRID(func));

			_IR_LastExpressionField = irspan;


		}

	}
	retflag = false;
}


UCodeLangFrontEnd