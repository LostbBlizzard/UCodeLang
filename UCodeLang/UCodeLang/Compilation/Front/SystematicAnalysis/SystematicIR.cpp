#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
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
			auto BaseAsIR = IR_Load_UIntptr(BaseSize);
			for (size_t i = 0; i < Info->Count; i++)
			{
				void* ItemOffset = Value.Object_AsPointer.get() + (BaseSize * i);
				Eval_Set_ObjectAs(Base, _DataAsIndex, ItemOffset, BaseSize);

				auto ItemIR = LoadEvaluatedEx(_DataAsIndex, Base);

				_IR_LookingAtIRBlock->New_Index_Vetor(Ptr, IR_Load_UIntptr(i), BaseAsIR);
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
		bool ShouldCallCopyFunc = ExType._ValueInfo != TypeValueInfo::IsValue && !ExType.IsAddress();
		if (ShouldCallCopyFunc)
		{
			auto GetSymOp = Symbol_GetSymbol(ExType);
			if (GetSymOp.has_value())
			{
				auto GetSym = GetSymOp.value();
				if (GetSym->Type == SymbolType::Type_class)
				{
					auto info = GetSym->Get_Info<ClassInfo>();

					if (info->_ClassHasCopyConstructor || info->_AutoGenerateCopyConstructor)
					{
						auto CopySybID = info->_ClassHasCopyConstructor.has_value() ? info->_ClassHasCopyConstructor.value() : info->_AutoGenerateCopyConstructor.value();
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

		bool hasattrbutsbutaddress =
			ExType.IsAddressArray() ||
			ExType.IsDynamicTrait();

		if (ShouldCallMoveFunc && HasMoveContructerHasIRFunc(ExType) && !hasattrbutsbutaddress)
		{
			auto GetSymOp = Symbol_GetSymbol(ExType);
			if (GetSymOp.has_value())
			{
				auto GetSym = GetSymOp.value();
				if (GetSym->Type == SymbolType::Type_class)
				{
					auto info = GetSym->Get_Info<ClassInfo>();

					if (info->_ClassHasMoveConstructor.has_value() || info->_AutoGenerateMoveConstructor.has_value())
					{
						SymbolID MoveSybID;
						if (info->_ClassHasMoveConstructor.has_value())
						{
							MoveSybID = info->_ClassHasMoveConstructor.value();
						}
						else
						{
							MoveSybID = info->_AutoGenerateMoveConstructor.value();
						}

						auto Syb = Symbol_GetSymbol(MoveSybID);
						auto irfuncid = IR_GetIRID(Syb->Get_Info<FuncInfo>());

						auto v = ExType;
						v._IsAddress = false;
						v._MoveData = MoveData::None;
						auto tep = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(v));

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
			bool callconstruct = !ToType.IsAddress();

			if (!callconstruct)
			{
				return false;
			}
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
						par._IsAddress = ExType.IsAddress();
						par._ValueInfo = ExType._ValueInfo;
						if (par.Isimmutable())
						{
							par._Isimmutable = ExType._Isimmutable;
						}
						if (Type_AreTheSame(par, ExType))
						{
							auto v = ToType;
							v._IsAddress = false;
							IRInstruction* ret = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(v));

							_IR_LookingAtIRBlock->NewPushParameter(_IR_LookingAtIRBlock->NewLoadPtr(ret));

							if (!ExType.IsAddress() && (Par.Type.IsAddress() || Par.Type.IsMovedType()))
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

			for (auto& Item2 : ConstructorSymbols)
			{
				if (Item2->Type == SymbolType::GenericFunc)
				{
					FuncInfo* funcinfo = Item2->Get_Info<FuncInfo>();
					if (funcinfo->Pars.size() == 2)
					{
						auto funcnode = Item2->Get_NodeInfo<FuncNode>();
						auto& funcgenericdata = funcinfo->_GenericData;

						Vector<TypeSymbol> InputGeneric;
						Vector<ParInfo> ValueTypes = { funcinfo->Pars[0],ParInfo{false,ExType} };

						UseGenericsNode usegeneric;

						auto v = Type_FuncinferGenerics(InputGeneric, ValueTypes, &usegeneric, Item2, true);

						if (InputGeneric.size() == funcgenericdata._Genericlist.size() && !funcgenericdata.IsPack())
						{
							{
								bool exit = false;
								for (auto& Item : InputGeneric)
								{
									if (Type_IsUnMapType(Item))
									{
										exit = true;
										break;
									}
								}
								if (exit) { break; }
							}
							
							String NewName = Generic_SymbolGenericFullName(Item2, InputGeneric);
							auto FuncIsMade = Symbol_GetSymbol(NewName, SymbolType::Func);
							
							if (FuncIsMade.has_value())
							{
								auto func = FuncIsMade.value()->Get_Info<FuncInfo>();

								auto v = func->Pars[0].Type;
								v._IsAddress = false;
								IRInstruction* ret = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(v));

								_IR_LookingAtIRBlock->NewPushParameter(_IR_LookingAtIRBlock->NewLoadPtr(ret));

								auto& Par = func->Pars[1];
								if (!ExType.IsAddress() && (Par.Type.IsAddress() || Par.Type.IsMovedType()))
								{
									Ex = _IR_LookingAtIRBlock->NewLoadPtr(Ex);
								}

								_IR_LookingAtIRBlock->NewPushParameter(Ex);
							
								_IR_LookingAtIRBlock->NewCall(IR_GetIRID(func));
								_IR_LastExpressionField = ret;
								return true;
							}
							else
							{
								//Should be made already.
								UCodeLangUnreachable();
							}
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

			ScopedNameNode tep;
			ScopedName tep2;
			tep2._token = _LastLookedAtToken.value().value();
			tep._ScopedName.push_back(std::move(tep2));

			_IR_LastExpressionField = Ex;

			IR_Build_FuncCall(v, tep, {});
		}
		else
		{
			if (Type_IsIntType(ToType))
			{

				if (Type_IsUIntType(ExType))
				{
					switch (ToType._Type)
					{
					case TypesEnum::sInt8:_IR_LastExpressionField = Type_GetSize(ToType) == Type_GetSize(ExType) ? Ex : _IR_LookingAtIRBlock->New_UIntToUInt8(Ex); break;
					case TypesEnum::sInt16:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_UIntToUInt16(Ex); break;

					ULablesint32:
					case TypesEnum::sInt32:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_UIntToUInt32(Ex); break;

					ULablesint64:
					case TypesEnum::sInt64:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_UIntToUInt64(Ex); break;


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
					case TypesEnum::uInt8:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_SIntToSInt8(Ex); break;
					case TypesEnum::uInt16:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_SIntToSInt16(Ex); break;

					SLableuint32:
					case TypesEnum::uInt32:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_SIntToSInt32(Ex); break;

					SLableuint64:
					case TypesEnum::uInt64:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_SIntToSInt64(Ex); break;


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
				else if (ToType._Type == TypesEnum::sInt32 && ExType._Type == TypesEnum::float32)
				{
					_IR_LastExpressionField = _IR_LookingAtIRBlock->New_f32Toi32(Ex);
				}
				else if (ToType._Type == TypesEnum::sInt64 && ExType._Type == TypesEnum::float64)
				{
					_IR_LastExpressionField = _IR_LookingAtIRBlock->New_f64Toi64(Ex);
				}
				else if (Type_IsCharType(ExType._Type)
					&& Type_GetSize(ExType) == Type_GetSize(ToType))
				{
					_IR_LastExpressionField = Ex;
				}
				else if (Symbol_GetSymbol(ExType).has_value())
				{
					auto sym = Symbol_GetSymbol(ExType).value();

					if (sym->Type == SymbolType::Enum)
					{
						auto info = sym->Get_Info<EnumInfo>();

						if (!info->VariantData.has_value() && Type_AreTheSame(info->Basetype, ToType))
						{
							_IR_LastExpressionField = Ex;
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
				else
				{
					UCodeLangUnreachable();
				}
			}
			else if (ToType._Type == TypesEnum::float32 && ExType._Type == TypesEnum::sInt32)
			{
				_IR_LastExpressionField = _IR_LookingAtIRBlock->New_Si32Tof32(Ex);
			}
			else if (ToType._Type == TypesEnum::float64 && ExType._Type == TypesEnum::sInt64)
			{
				_IR_LastExpressionField = _IR_LookingAtIRBlock->New_Si64Tof64(Ex);
			}
			else if (ToType._Type == TypesEnum::float32 && ExType._Type == TypesEnum::uInt32)
			{
				_IR_LastExpressionField = _IR_LookingAtIRBlock->New_Ui32Tof32(Ex);
			}
			else if (ToType._Type == TypesEnum::float64 && ExType._Type == TypesEnum::uInt64)
			{
				_IR_LastExpressionField = _IR_LookingAtIRBlock->New_Ui64Tof64(Ex);
			}
			else if (ToType._Type == TypesEnum::float32 && ExType._Type == TypesEnum::float64)
			{
				_IR_LastExpressionField = _IR_LookingAtIRBlock->New_f64Tof32(Ex);
			}
			else if (ToType._Type == TypesEnum::float64 && ExType._Type == TypesEnum::float32)
			{
				_IR_LastExpressionField = _IR_LookingAtIRBlock->New_f32Tof64(Ex);
			}
			else if (Type_IsCharType(ToType) && Type_IsUIntType(ExType._Type)
				&& ExType._Type != TypesEnum::uIntPtr && Type_GetSize(ToType) == Type_GetSize(ExType._Type))
			{
				_IR_LastExpressionField = Ex;
			}
			else if (Symbol_GetSymbol(ToType).has_value())
			{
				auto sym = Symbol_GetSymbol(ToType).value();

				if (sym->Type == SymbolType::Enum)
				{
					auto info = sym->Get_Info<EnumInfo>();

					if (!info->VariantData.has_value() && Type_AreTheSame(info->Basetype, ToType))
					{
						_IR_LastExpressionField = Ex;
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

#endif