#include "UCodeLang/Compliation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

IRInstruction* SystematicAnalysis::IR_Load_UIntptr(UAddress Value)
{

	switch (_Settings->PtrSize)
	{
	case IntSizes::Int8:
		return _IR_LookingAtIRBlock->NewLoad((UInt8)Value);
	case IntSizes::Int16:
		return _IR_LookingAtIRBlock->NewLoad((UInt16)Value);
	case IntSizes::Int32:
		return _IR_LookingAtIRBlock->NewLoad((UInt32)Value);
	case IntSizes::Int64:
		return _IR_LookingAtIRBlock->NewLoad((UInt64)Value);
	default:
		UCodeLangUnreachable();
		break;
	}
}
IRInstruction* SystematicAnalysis::IR_Load_SIntptr(SIntNative Value)
{
	return IR_Load_UIntptr(*(UAddress*)&Value);
}

IRInstruction* SystematicAnalysis::IR_Build_Add_uIntPtr(IRInstruction* field, IRInstruction* field2)
{
	return _IR_LookingAtIRBlock->NewAdd(field, field2);
}

IRInstruction* SystematicAnalysis::IR_Build_Sub_uIntPtr(IRInstruction* field, IRInstruction* field2)
{
	return _IR_LookingAtIRBlock->NewSub(field, field2);
}

IRInstruction* SystematicAnalysis::IR_Build_Add_sIntPtr(IRInstruction* field, IRInstruction* field2)
{
	return _IR_LookingAtIRBlock->NewAdd(field, field2);
}

IRInstruction* SystematicAnalysis::IR_Build_Sub_sIntPtr(IRInstruction* field, IRInstruction* field2)
{
	return _IR_LookingAtIRBlock->NewSub(field, field2);
}

IRInstruction* SystematicAnalysis::IR_Build_Mult_uIntPtr(IRInstruction* field, IRInstruction* field2)
{
	return _IR_LookingAtIRBlock->NewUMul(field, field2);
}

IRInstruction* SystematicAnalysis::IR_Build_Mult_sIntPtr(IRInstruction* field, IRInstruction* field2)
{
	return _IR_LookingAtIRBlock->NewSMul(field, field2);
}

IRInstruction* SystematicAnalysis::Build_Div_uIntPtr(IRInstruction* field, IRInstruction* field2)
{
	return _IR_LookingAtIRBlock->NewUDiv(field, field2);
}

IRInstruction* SystematicAnalysis::IR_Build_Div_sIntPtr(IRInstruction* field, IRInstruction* field2)
{
	return _IR_LookingAtIRBlock->NewSDiv(field, field2);
}

void SystematicAnalysis::IR_Build_Increment_uIntPtr(IRInstruction* field, UAddress Value)
{
	switch (_Settings->PtrSize)
	{
	case IntSizes::Int32:
		_IR_LookingAtIRBlock->New_Increment(field, _IR_LookingAtIRBlock->NewLoad((UInt32)Value));
		break;
	case IntSizes::Int64:
		_IR_LookingAtIRBlock->New_Increment(field, _IR_LookingAtIRBlock->NewLoad((UInt64)Value));
		break;
	default:
		UCodeLangUnreachable();
		break;
	}
}

void SystematicAnalysis::IR_Build_Decrement_uIntPtr(IRInstruction* field, UAddress Value)
{
	switch (_Settings->PtrSize)
	{
	case IntSizes::Int32:
		_IR_LookingAtIRBlock->New_Decrement(field, _IR_LookingAtIRBlock->NewLoad((UInt32)Value));
		break;
	case IntSizes::Int64:
		_IR_LookingAtIRBlock->New_Decrement(field, _IR_LookingAtIRBlock->NewLoad((UInt64)Value));
		break;
	default:
		UCodeLangUnreachable();
		break;
	}
}

void SystematicAnalysis::IR_Build_Increment_sIntPtr(IRInstruction* field, SIntNative Value)
{
	return IR_Build_Increment_uIntPtr(field, *(UAddress*)&Value);
}
IRInstruction* SystematicAnalysis::LoadEvaluatedEx(const RawEvaluatedObject& Value, const TypeSymbol& ValueType)
{
	void* ObjectData = Eval_Get_Object(ValueType, Value);
	switch (ValueType._Type)
	{
	case TypesEnum::Bool:
	case TypesEnum::Char:
	case TypesEnum::sInt8:
	case TypesEnum::uInt8:
		return _IR_LookingAtIRBlock->NewLoad(*(UInt8*)ObjectData);
		break;
	case TypesEnum::sInt16:
	case TypesEnum::uInt16:
		return _IR_LookingAtIRBlock->NewLoad(*(UInt16*)ObjectData);
		break;
	case TypesEnum::sInt32:
	case TypesEnum::uInt32:
		return _IR_LookingAtIRBlock->NewLoad(*(UInt32*)ObjectData);
		break;
	case TypesEnum::float32:
		return _IR_LookingAtIRBlock->NewLoad(*(float32*)ObjectData);
		break;
	case TypesEnum::float64:
		return _IR_LookingAtIRBlock->NewLoad(*(float64*)ObjectData);
		break;
	case TypesEnum::sInt64:
	case TypesEnum::uInt64:
		return _IR_LookingAtIRBlock->NewLoad(*(UInt64*)ObjectData);
		break;

	case TypesEnum::sIntPtr:
	case TypesEnum::uIntPtr:
		if (_Settings->PtrSize == IntSizes::Int64)
		{
			return _IR_LookingAtIRBlock->NewLoad(*(UInt64*)ObjectData);
		}
		else
		{
			return _IR_LookingAtIRBlock->NewLoad(*(UInt32*)ObjectData);
		}
	case TypesEnum::CustomType:
	{
		auto Syb = Symbol_GetSymbol(ValueType._CustomTypeSymbol);


		if (Syb->Type == SymbolType::Type_StaticArray)
		{
			StaticArrayInfo* Info = Syb->Get_Info<StaticArrayInfo>();
			auto R = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(ValueType));
			auto Base = Info->Type;
			auto BaseSize = Type_GetSize(Base).value();

			auto Ptr = _IR_LookingAtIRBlock->NewLoadPtr(R);

			RawEvaluatedObject _DataAsIndex;
			_DataAsIndex.ObjectSize = BaseSize;
			_DataAsIndex.Object_AsPointer.reset(new Byte[BaseSize]);
			auto BaseAsIR = _IR_LookingAtIRBlock->NewLoad(BaseSize);
			for (size_t i = 0; i < Info->Count; i++)
			{
				void* ItemOffset = Value.Object_AsPointer.get() + (BaseSize * i);
				Eval_Set_ObjectAs(Base, _DataAsIndex, ItemOffset, BaseSize);

				auto ItemIR = LoadEvaluatedEx(_DataAsIndex, Base);

				_IR_LookingAtIRBlock->New_Index_Vetor(Ptr, _IR_LookingAtIRBlock->NewLoad(i), BaseAsIR);
			}

			return R;
		}
		else
		{
			UCodeLangUnreachable();
		}

	}
	default:
		UCodeLangUnreachable();
		break;
	}
}

void SystematicAnalysis::IR_Build_Decrement_sIntPtr(IRInstruction* field, SIntNative Value)
{
	return IR_Build_Decrement_uIntPtr(field, *(UAddress*)&Value);
}

bool SystematicAnalysis::IR_Build_ImplicitConversion(IRInstruction* Ex, const TypeSymbol ExType, const TypeSymbol& ToType)
{

	if (Type_AreTheSame(ExType, ToType))
	{
		bool ShouldCallCopyFunc = ExType._ValueInfo != TypeValueInfo::IsValue;
		if (ShouldCallCopyFunc)
		{
			auto GetSymOp = Symbol_GetSymbol(ExType);
			if (GetSymOp.has_value())
			{
				auto GetSym = GetSymOp.value();
				if (GetSym->Type == SymbolType::Type_class)
				{
					auto info = GetSym->Get_Info<ClassInfo>();

					if (info->_ClassHasCopyConstructor)
					{
						auto CopySybID = info->_ClassHasCopyConstructor.value();
						auto Syb = Symbol_GetSymbol(CopySybID);
						auto irfuncid = IR_GetIRID(Syb->Get_Info<FuncInfo>());


						auto tep = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(ExType));

						_IR_LookingAtIRBlock->NewPushParameter(_IR_LookingAtIRBlock->NewLoadPtr(tep));
						if (ExType.IsAddress())
						{
							_IR_LookingAtIRBlock->NewPushParameter(Ex);
						}
						else
						{
							_IR_LookingAtIRBlock->NewPushParameter(_IR_LookingAtIRBlock->NewLoadPtr(Ex));
						}
						_IR_LookingAtIRBlock->NewCall(irfuncid);
						_IR_LastExpressionField = tep;
					}
				}
			}
		}

		return true;
	}
	else if (Type_AreTheSameWithOutMoveAndimmutable(ExType, ToType))
	{
		//if (ExType.IsMovedType() && ExType.IsAddress() 
		//	&& ToType.IsMovedType() == false)
		bool calledmoved = false;

		bool ShouldCallMoveFunc = ExType._ValueInfo != TypeValueInfo::IsValue;
		if (ShouldCallMoveFunc && HasMoveContructerHasIRFunc(ExType))
		{
			auto GetSymOp = Symbol_GetSymbol(ExType);
			if (GetSymOp.has_value())
			{
				auto GetSym = GetSymOp.value();
				if (GetSym->Type == SymbolType::Type_class)
				{
					auto info = GetSym->Get_Info<ClassInfo>();

					if (info->_ClassHasMoveConstructor)
					{
						auto MoveSybID = info->_ClassHasMoveConstructor.value();
						auto Syb = Symbol_GetSymbol(MoveSybID);
						auto irfuncid = IR_GetIRID(Syb->Get_Info<FuncInfo>());


						auto tep = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(ExType));

						_IR_LookingAtIRBlock->NewPushParameter(_IR_LookingAtIRBlock->NewLoadPtr(tep));

						_IR_LookingAtIRBlock->NewPushParameter(Ex);

						_IR_LookingAtIRBlock->NewCall(irfuncid);
						_IR_LastExpressionField = tep;

						calledmoved = true;
					}
				}
			}
		}

		if (ToType.IsAddress() && !ExType.IsAddress() && ExType._ValueInfo == TypeValueInfo::IsValue)
		{
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoadPtr(Ex);
		}

		if (calledmoved)
		{
			return true;
		}
	}

	if (Type_CanDoTypeToTrait(ExType, ToType))
	{
		auto IRType = IR_ConvertToIRType(ToType);
		IRInstruction* structV = _IR_LookingAtIRBlock->NewLoad(IRType);

		IRStruct* IRStructPtr = _IR_Builder.GetSymbol(IRType._symbol)->Get_ExAs<IRStruct>();

		auto Member = _IR_LookingAtIRBlock->New_Member_Access(structV, IRStructPtr, DymTraitIRPointerIndex);

		if (ExType.IsAddress())
		{
			_IR_LookingAtIRBlock->NewStore(Member, Ex);
		}
		else
		{
			_IR_LookingAtIRBlock->NewStore(Member, _IR_LookingAtIRBlock->NewLoadPtr(Ex));
		}

		auto Member2 = _IR_LookingAtIRBlock->New_Member_Access(structV, IRStructPtr, DymTraitIRVTableIndex);

		auto IDVTable = _IR_Builder.ToID(Str_GetClassWithTraitVTableName(Symbol_GetSymbol(ExType).value()->FullName, Symbol_GetSymbol(ToType).value()->FullName));

		_IR_LookingAtIRBlock->NewStore(Member2, _IR_LookingAtIRBlock->NewLoadPtr(IDVTable));

		_IR_LastExpressionField = structV;
		return true;
	}

	if (auto syb = Symbol_GetSymbol(ToType).value_unchecked())
	{
		if (syb->Type == SymbolType::Type_class)
		{
			auto info = syb->Get_Info<ClassInfo>();


			Symbol_Update_ClassSym_ToFixedTypes(NeverNullptr(syb));

			String Scope = info->FullName;
			ScopeHelper::GetApendedString(Scope, ClassConstructorfunc);

			auto ConstructorSymbols = GetSymbolsWithName(Scope, SymbolType::Any);


			for (auto& Item2 : ConstructorSymbols)
			{
				if (Item2->Type == SymbolType::Func)
				{
					FuncInfo* funcinfo = Item2->Get_Info<FuncInfo>();

					if (funcinfo->Pars.size() == 2)
					{
						Symbol_Update_FuncSym_ToFixedTypes(NeverNullptr(Item2));

						auto& Par = funcinfo->Pars[1];
						auto par = Par.Type;
						par._IsAddress = false;
						if (Type_AreTheSame(par, ExType))
						{
							IRInstruction* ret = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(ToType));

							_IR_LookingAtIRBlock->NewPushParameter(_IR_LookingAtIRBlock->NewLoadPtr(ret));

							if (!ExType.IsAddress() && Par.Type.IsAddress())
							{
								Ex = _IR_LookingAtIRBlock->NewLoadPtr(Ex);
							}

							_IR_LookingAtIRBlock->NewPushParameter(Ex);

							_IR_LookingAtIRBlock->NewCall(IR_GetIRID(funcinfo));

							_IR_LastExpressionField = ret;

							

							return true;
						}
					}
				}

			}
		}
	}

	return false;
}
void SystematicAnalysis::IR_Build_ExplicitConversion(IRInstruction* Ex, const TypeSymbol ExType, const TypeSymbol& ToType, const CastExpressionNode_Data& Data)
{
	if (!IR_Build_ImplicitConversion(Ex, ExType, ToType))
	{
		if (ExType._Type == TypesEnum::CustomType)
		{
			Symbol& syb = *Symbol_GetSymbol(ExType._CustomTypeSymbol);
			if (syb.Type == SymbolType::Hard_Type_alias)
			{
				if (Type_AreTheSameWithOutimmutable(syb.VarType, ToType))
				{
					return;//do nothing
				}
			}

		}
		else if (ToType._Type == TypesEnum::CustomType)
		{
			Symbol& syb = *Symbol_GetSymbol(ToType._CustomTypeSymbol);
			if (syb.Type == SymbolType::Hard_Type_alias)
			{
				if (Type_AreTheSameWithOutimmutable(syb.VarType, ExType))
				{
					return;//do nothing
				}
			}

		}

		//DO Stuff
		if (Data.FuncToCall)
		{
			FuncInfo* f = Data.FuncToCall->Get_Info<FuncInfo>();
			Get_FuncInfo v;
			v.Func = f;
			v.SymFunc = Data.FuncToCall;
			v.ThisPar = Get_FuncInfo::ThisPar_t::PushFromLast;

			_IR_LastExpressionField = Ex;

			IR_Build_FuncCall(v, {}, {});
		}
		else
		{
			if (Type_IsIntType(ToType))
			{

				if (Type_IsUIntType(ExType))
				{
					switch (ToType._Type)
					{
					case TypesEnum::sInt8:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_UIntToSInt(_IR_LookingAtIRBlock->New_UIntToUInt8(Ex)); break;
					case TypesEnum::sInt16:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_UIntToSInt(_IR_LookingAtIRBlock->New_UIntToUInt16(Ex)); break;

					ULablesint32:
					case TypesEnum::sInt32:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_UIntToSInt(_IR_LookingAtIRBlock->New_UIntToUInt32(Ex)); break;

					ULablesint64:
					case TypesEnum::sInt64:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_UIntToSInt(_IR_LookingAtIRBlock->New_UIntToUInt64(Ex)); break;


					case TypesEnum::uInt8:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_UIntToUInt8(Ex); break;
					case TypesEnum::uInt16:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_UIntToUInt16(Ex); break;

					ULableuint32:
					case TypesEnum::uInt32:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_UIntToUInt32(Ex); break;

					ULableuint64:
					case TypesEnum::uInt64:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_UIntToUInt64(Ex); break;


					case TypesEnum::sIntPtr:
						if (_Settings->PtrSize == IntSizes::Int32)
						{
							goto ULablesint32;
						}
						else
						{
							goto ULablesint64;
						}
					case TypesEnum::uIntPtr:
						if (_Settings->PtrSize == IntSizes::Int32)
						{
							goto ULableuint32;
						}
						else
						{
							goto ULableuint64;
						}
					default:UCodeLangUnreachable(); break;
					}
				}
				else if (Type_IsSIntType(ExType))
				{
					switch (ToType._Type)
					{
					case TypesEnum::uInt8:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_SIntToUInt(_IR_LookingAtIRBlock->New_SIntToSInt8(Ex)); break;
					case TypesEnum::uInt16:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_SIntToUInt(_IR_LookingAtIRBlock->New_SIntToSInt16(Ex)); break;

					SLableuint32:
					case TypesEnum::uInt32:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_SIntToUInt(_IR_LookingAtIRBlock->New_SIntToSInt32(Ex)); break;

					SLableuint64:
					case TypesEnum::uInt64:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_SIntToUInt(_IR_LookingAtIRBlock->New_SIntToSInt64(Ex)); break;


					case TypesEnum::sInt8:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_SIntToSInt8(Ex); break;
					case TypesEnum::sInt16:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_SIntToSInt16(Ex); break;

					SLablesint32:
					case TypesEnum::sInt32:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_SIntToSInt32(Ex); break;

					SLablesint64:
					case TypesEnum::sInt64:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_SIntToSInt64(Ex); break;

					case TypesEnum::sIntPtr:
						if (_Settings->PtrSize == IntSizes::Int32)
						{
							goto SLablesint32;
						}
						else
						{
							goto SLablesint64;
						}
					case TypesEnum::uIntPtr:
						if (_Settings->PtrSize == IntSizes::Int32)
						{
							goto SLableuint32;
						}
						else
						{
							goto SLableuint64;
						}
						break;
					default:UCodeLangUnreachable(); break;
					}
				}
				else
				{
					UCodeLangUnreachable();
				}
			}
			else
			{
				UCodeLangUnreachable();
			}
		}

	}
}


UCodeLangFrontEnd