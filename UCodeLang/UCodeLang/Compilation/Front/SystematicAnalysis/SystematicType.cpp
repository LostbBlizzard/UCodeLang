#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

bool SystematicAnalysis::Type_IsUIntType(const TypeSymbol& TypeToCheck) const
{
	return
		TypeToCheck._Type == TypesEnum::uInt8 ||
		TypeToCheck._Type == TypesEnum::uInt16 ||
		TypeToCheck._Type == TypesEnum::uInt32 ||
		TypeToCheck._Type == TypesEnum::uInt64 ||
		TypeToCheck._Type == TypesEnum::uIntPtr;
}
bool SystematicAnalysis::Type_IsfloatType(const TypeSymbol& TypeToCheck) const
{
	return
		TypeToCheck._Type == TypesEnum::float32 ||
		TypeToCheck._Type == TypesEnum::float64;
}
bool SystematicAnalysis::Type_IsCharType(const TypeSymbol& TypeToCheck) const
{
	return
		TypeToCheck._Type == TypesEnum::Char ||
		TypeToCheck._Type == TypesEnum::Uft8 ||
		TypeToCheck._Type == TypesEnum::Uft16 ||
		TypeToCheck._Type == TypesEnum::Uft32;
}
bool SystematicAnalysis::Type_IsPrimitive(const TypeSymbol& TypeToCheck) const
{
	bool r = TypeToCheck.IsAddress() || Type_IsPrimitiveNotIncludingPointers(TypeToCheck);

	if (!r && TypeToCheck.IsAn(TypesEnum::CustomType))
	{
		const Symbol& V = *Symbol_GetSymbol(TypeToCheck).value();
		if (V.Type == SymbolType::Func_ptr)
		{
			return true;
		}
	}

	return r;
}
bool SystematicAnalysis::Type_IsPrimitiveNotIncludingPointers(const TypeSymbol& TypeToCheck) const
{
	bool r = Type_IsIntType(TypeToCheck)
		|| TypeToCheck._Type == TypesEnum::Bool
		|| Type_IsCharType(TypeToCheck)
		|| TypeToCheck._Type == TypesEnum::Void
		|| Type_IsfloatType(TypeToCheck);

	if (r == false && TypeToCheck._Type == TypesEnum::CustomType)
	{
		auto V = Symbol_GetSymbol(TypeToCheck._CustomTypeSymbol).value();
		if (V && (V->Type == SymbolType::Hard_Type_alias
			|| V->Type == SymbolType::Type_alias))
		{
			return Type_IsPrimitiveNotIncludingPointers(V->VarType);
		}
	}

	return r;
}
bool SystematicAnalysis::Type_IsStaticArray(const TypeSymbol& TypeToCheck) const
{
	if (TypeToCheck.IsAn(TypesEnum::CustomType))
	{
		auto Syb = Symbol_GetSymbol(TypeToCheck).value();
		return Syb->Type == SymbolType::Type_StaticArray;
	}


	return false;
}


bool SystematicAnalysis::Type_IsimmutableRulesfollowed(const TypeSymbol& TypeToCheck, const TypeSymbol& Type) const
{

	bool Chechimm = TypeToCheck.Isimmutable();
	bool CmpTypeimm = Type.Isimmutable();


	if (Chechimm == CmpTypeimm || CmpTypeimm == true)
	{
		return true;
	}
	else if (CmpTypeimm == false && Chechimm == true)
	{
		return true;
		//return TypeToCheck.IsAddress() && Type.IsAddress();

	}

	return false;
}
bool SystematicAnalysis::Type_IsAddessAndLValuesRulesfollowed(const TypeSymbol& TypeToCheck, const TypeSymbol& Type, bool ReassignMode, bool isdeclare) const
{
	if (ReassignMode) { return true; }
	bool CheckIsLocation = TypeToCheck.IsLocationValue() || TypeToCheck.IsAddress();
	bool WantsALocation = Type.IsAddress();

	if (isdeclare)
	{
		if (!CheckIsLocation && WantsALocation)
		{
			return false;
		}
	}

	return (
		(CheckIsLocation)
		|| (CheckIsLocation == false || WantsALocation == false)
		|| (TypeToCheck.IsRawValue() && Type.IsRawValue())//constant expression
		);
}
bool SystematicAnalysis::Symbol_HasDestructor(const TypeSymbol& TypeToCheck)
{

	TypeSymbol Tep = TypeToCheck;
	if (Tep.IsAddress())
	{
		Tep._IsAddress = false;
	}
	if (Tep.IsAddressArray())
	{
		Tep._IsAddressArray = false;
	}


	String TypeDestructorFuncName = ToString(Tep);
	ScopeHelper::GetApendedString(TypeDestructorFuncName, ClassDestructorFunc);
	auto dropfunc = Symbol_GetSymbol(TypeDestructorFuncName, SymbolType::Func);
	if (!dropfunc.has_value())
	{
		auto Sym = Symbol_GetSymbol(TypeToCheck);

		if (Sym && Sym.value()->Type == SymbolType::Type_class)
		{
			auto Classinfo = Sym.value()->Get_Info<ClassInfo>();

			if (!Symbol_IsDependencies(Classinfo))
			{
				Symbol_Update_ClassSym_ToFixedTypes(Sym.value());
			}
			else
			{
				//do nothing so GetSize will make the error.				
			}

			return Classinfo->_WillHaveFielddeInit;
		}
		else if (Sym && Sym.value()->Type == SymbolType::Enum)
		{
			auto Classinfo = Sym.value()->Get_Info<EnumInfo>();

			return  Classinfo->HasDestructer;
		}
		else if (Sym && Sym.value()->Type == SymbolType::Type_StaticArray)
		{
			auto Classinfo = Sym.value()->Get_Info<StaticArrayInfo>();

			return  Symbol_HasDestructor(Classinfo->Type);
		}

	}
	return dropfunc.has_value();
}


bool SystematicAnalysis::Type_GetSize(const TypeSymbol& Type, size_t& OutSize)
{
	if (Type.IsAddress() || Type.IsAddressArray()) { goto IntPtr; }

	if (Type.IsTypeInfo())
	{

		if (Type._TypeInfo == TypeInfoPrimitive::ClassFieldInfo)
		{
			OutSize = sizeof(Systematic_BuiltInFunctions::ClassField);
		}
		else if (Type._TypeInfo == TypeInfoPrimitive::EnumFieldInfo)
		{
			OutSize = sizeof(Systematic_BuiltInFunctions::EnumField);
		}
		else if (Type._TypeInfo == TypeInfoPrimitive::EnumVariantInfo)
		{
			OutSize = sizeof(EnumVariantData);
		}
		else
		{
			OutSize = sizeof(TypeSymbol);
		}
		return true;
	}

	switch (Type._Type)
	{
	case TypesEnum::sInt8:
	case TypesEnum::uInt8:
	case TypesEnum::Bool:
	case TypesEnum::Char:
	case TypesEnum::Uft8:
		OutSize = 1;
		return true;
	case TypesEnum::Uft16:
	case TypesEnum::sInt16:
	case TypesEnum::uInt16:
		OutSize = sizeof(UInt16);
		return true;

	case TypesEnum::Uft32:
	case TypesEnum::float32:
	case TypesEnum::sInt32:
	case TypesEnum::uInt32:
		OutSize = sizeof(UInt32);
		return true;

	case TypesEnum::float64:
	case TypesEnum::uInt64:
	case TypesEnum::sInt64:
		OutSize = sizeof(UInt64);
		return true;

	case TypesEnum::sIntPtr:
	case TypesEnum::uIntPtr:
	IntPtr:

		switch (_Settings->PtrSize)
		{
		case IntSizes::Int8:
			OutSize = sizeof(UInt8);
			break;
		case IntSizes::Int16:
			OutSize = sizeof(UInt8);
			break;
		case IntSizes::Int32:
			OutSize = sizeof(UInt32);
			break;
		case IntSizes::Int64:
			OutSize = sizeof(UInt64);
			break;
		default:
			UCodeLangUnreachable();
			break;
		}
		return true;

	case TypesEnum::CustomType:
	{
		Symbol& V = *Symbol_GetSymbol(Type._CustomTypeSymbol);

		if (V.Type == SymbolType::Type_class)
		{

			ClassInfo* Vp = V.Get_Info<ClassInfo>();

			if (!Vp->SizeInitialized)
			{
				Vp->Size = 0;
				auto& classNode = *V.Get_NodeInfo<ClassNode>();
				if (!Symbol_IsDependencies(Vp))
				{
					auto Old = SaveAndMove_SymbolContext();
					Set_SymbolContext(Vp->Context.value());

					OnClassNode(classNode);//update class fields


					Set_SymbolContext(std::move(Old));
				}
				else
				{
					LogError_TypeDependencyCycle(NeverNullptr(classNode._className.token), Vp);
				}
			}


			OutSize = Vp->Size;
			return true;
		}
		else if (V.Type == SymbolType::Type_alias
			|| V.Type == SymbolType::Hard_Type_alias)
		{



			return Type_GetSize(V.VarType, OutSize);
		}
		else if (V.Type == SymbolType::Enum)
		{
			EnumInfo* Info = V.Get_Info<EnumInfo>();
			if (Info->IsOptionalAddress())
			{
				TypeSymbol pointer(TypesEnum::Bool);
				pointer.SetAsAddress();
				OutSize = Type_GetSize(pointer).value();
				return true;
			}
			else
				if (Info->VariantData.has_value())
				{
					auto tagsize = Type_GetSize(Info->Basetype, OutSize);
					auto& Item = Info->VariantData.value();


					if (!Item.VariantSize.has_value())
					{
						size_t MaxSize = 0;

						for (auto& Item2 : Item.Variants)
						{
							size_t ItemSize = 0;
							for (auto& Item3 : Item2.Types)
							{
								UAddress tep = 0;
								Type_GetSize(Item3, tep);
								ItemSize += tep;
							}
							if (ItemSize > MaxSize)
							{
								MaxSize = ItemSize;
							}
						}
						Item.VariantSize = MaxSize;
					}
					OutSize += Item.VariantSize.value();

					return true;
				}
				else
				{
					return Type_GetSize(Info->Basetype, OutSize);
				}
		}
		else if (V.Type == SymbolType::Func_ptr
			|| V.Type == SymbolType::Hard_Func_ptr)
		{
			goto IntPtr;
		}
		else if (V.Type == SymbolType::Type_StaticArray)
		{
			StaticArrayInfo* Info = V.Get_Info< StaticArrayInfo>();
			bool V = Type_GetSize(Info->Type, OutSize);
			OutSize *= Info->Count;
			return V;
		}
		else if (V.Type == SymbolType::Trait_class && Type._IsDynamic)
		{
			TypeSymbol pointer(TypesEnum::Bool);
			pointer.SetAsAddress();
			OutSize = Type_GetSize(pointer).value() * 2;
			return true;
		}
		else if (V.Type == SymbolType::Tag_class)
		{
			TagInfo* info = V.Get_Info<TagInfo>();
			size_t r = 0;

			for (auto& Item : info->Fields)
			{
				r += Type_GetSize(Item.Type).value();
			}

			OutSize = r;
			return true;
		}
		else if (V.Type == SymbolType::Unmaped_Generic_Type)
		{
			OutSize = 0;
			return true;
		}
		else
		{
			OutSize = 0;
			return false;
		}
	}
	default:
		OutSize = 0;
		return false;
	}



}
bool SystematicAnalysis::Type_GetOffset(const ClassInfo& Type, const FieldInfo* Field, UAddress& OutOffset)
{
	UAddress offset = 0;

	bool packed = true;//for testing
	if (Type._IsExternalC)
	{
		auto maxalignment = _Settings->PtrSize == IntSizes::Int64 ? 8 : 4;
		for (auto& Item : Type.Fields)
		{
			UAddress FieldSize = 0;
			Type_GetSize(Item.Type, FieldSize);



			auto alignment = FieldSize;
			if (alignment > maxalignment)
			{
				alignment = maxalignment;
			}

			auto misalignment = offset % alignment;
			auto padding = alignment - misalignment;

			if (&Item != &Type.Fields.front()) {
				offset += padding;
			}
			if (&Item == Field)
			{
				OutOffset = offset;
				return true;
			}

		}
	}
	else if (packed)
	{
		for (auto& Item : Type.Fields)
		{
			if (&Item == Field)
			{
				OutOffset = offset;
				return true;
			}
			UAddress FieldSize = 0;
			Type_GetSize(Item.Type, FieldSize);
			offset += FieldSize;
		}
	}
	else
	{
		//UCodePacking
		Vector<const FieldInfo*>  bestorder;
		bestorder.resize(Type.Fields.size());

		for (size_t i = 0; i < Type.Fields.size(); i++)
		{
			bestorder[i] = &Type.Fields[i];
		}

		std::sort(bestorder.begin(), bestorder.end(), [this](const FieldInfo* A, const FieldInfo* B)
			{

				return Type_GetSize(A->Type).value() > Type_GetSize(B->Type).value();
			});

		auto maxalignment = _Settings->PtrSize == IntSizes::Int64 ? 8 : 4;



		for (auto& Item : bestorder)
		{

			UAddress FieldSize = 0;
			Type_GetSize(Item->Type, FieldSize);



			auto alignment = FieldSize;

			if (alignment > maxalignment)
			{
				alignment = maxalignment;
			}

			auto misalignment = offset % alignment;
			auto padding = alignment - misalignment;

			if (Item != bestorder.back()) {
				offset += padding;
			}
			if (Item == Field)
			{
				OutOffset = offset;
				return true;
			}
		}
	}


	OutOffset = offset;
	return false;
}
bool SystematicAnalysis::Type_IsSIntType(const TypeSymbol& TypeToCheck) const
{
	return
		TypeToCheck._Type == TypesEnum::sInt8 ||
		TypeToCheck._Type == TypesEnum::sInt16 ||
		TypeToCheck._Type == TypesEnum::sInt32 ||
		TypeToCheck._Type == TypesEnum::sInt64 ||
		TypeToCheck._Type == TypesEnum::sIntPtr;
}

bool SystematicAnalysis::Type_IsCompatible(const IsCompatiblePar& FuncPar, const Vector<ParInfo>& ValueTypes, bool _ThisTypeIsNotNull, const NeverNullPtr<Token> Token)
{
	size_t valcount = ValueTypes.size();

	bool unpackparpack = true;
	bool typepack = false;
	if (ValueTypes.size())
	{
		auto& lastpartype = ValueTypes.back().Type;
		auto symop = Symbol_GetSymbol(lastpartype);
		if (symop.has_value())
		{
			auto val = symop.value();

			if (val->Type == SymbolType::Type_Pack)
			{
				if (unpackparpack)
				{
					auto typelist = val->Get_Info<TypePackInfo>();
					valcount -= 1;
					valcount += typelist->List.size();

					typepack = true;
				}
			}
		}

	}

	if (FuncPar.Pars->size() != valcount)
	{
		return false;
	}


	//
	if ((PassType_t)FuncPar.Item->PassState < (PassType_t)_PassType)
	{
		if (FuncPar.Item->Type != SymbolType::Func)
		{
			UCodeLangUnreachable();
		}

		FuncInfo* Info = FuncPar.Item->Get_Info<FuncInfo>();
		if (!Symbol_IsDependencies(Info))
		{
			auto OldPass = _PassType;
			auto oldcontext = SaveAndMove_SymbolContext();

			Set_SymbolContext(Info->Context.value());

			_RetLoopStack.push_back(Info);

			OnFuncNode(*FuncPar.Item->Get_NodeInfo<FuncNode>());

			_RetLoopStack.pop_back();

			Set_SymbolContext(std::move(oldcontext));
		}
		else
		{
			auto V = Symbol_GetDependencies(Info);
			if (V->IsOnRetStatemnt && Info->Ret.IsNull())
			{
				LogError_FuncDependencyCycle(Token, Info);
				Info->Ret.SetType(TypesEnum::Null);//to stop err spam
				return false;
			}
			else
			{
				return true;//ret got fixed
			}

		}
	}
	//

	OptionalRef<Vector<TypeSymbol>> _TypePack;
	if (unpackparpack && typepack)
	{
		auto& lastpartype = ValueTypes.back().Type;
		_TypePack = Optionalref(Symbol_GetSymbol(lastpartype).value()->Get_Info<TypePackInfo>()->List);
	}
	for (size_t i = _ThisTypeIsNotNull ? 1 : 0; i < FuncPar.Pars->size(); i++)
	{
		auto& Item = (*FuncPar.Pars)[i];

		bool Item2IsOutpar = false;
		const TypeSymbol* Item2ptr = nullptr;
		if (_TypePack.has_value())
		{
			if (i >= ValueTypes.size() - 1)
			{
				Item2IsOutpar = false;
				auto newindex = i - (ValueTypes.size() - 1);
				Item2ptr = &_TypePack.value()[newindex];
			}
			else
			{
				Item2ptr = &ValueTypes[i].Type;
				Item2IsOutpar = ValueTypes[i].IsOutPar;
			}
		}
		else
		{
			Item2ptr = &ValueTypes[i].Type;
			Item2IsOutpar = ValueTypes[i].IsOutPar;
		}

		auto& Item2 = *Item2ptr;
		if (Item.IsOutPar != Item2IsOutpar)
		{
			return false;
		}
		else if (Item2IsOutpar && Item2.IsAn(TypesEnum::Var))
		{
			//is ok
			continue;
		}

		if (!Type_CanBeImplicitConverted(Item2, Item.Type, true))
		{
			return false;
		}
	}
	return true;
}

int SystematicAnalysis::Type_GetCompatibleScore(const ParInfo& ParFunc, const ParInfo& Value)
{
	int r = 0;

	if (Type_AreTheSameWithOutimmutable(ParFunc.Type, Value.Type))
	{
		r += 5;
	}
	else if (Type_CanBeImplicitConverted(ParFunc.Type, Value.Type, false))
	{
		r += 3;
	}
	else if (Type_CanBeExplicitlyConverted(ParFunc.Type, Value.Type).HasValue)
	{
		r += 1;
	}

	if (ParFunc.IsOutPar == Value.IsOutPar)
	{
		r += 1;
	}
	if (ParFunc.Type.IsAddress() == Value.Type.IsAddress())
	{
		r += 1;
	}
	if (ParFunc.Type.Isimmutable() == Value.Type.Isimmutable())
	{
		r += 1;
	}


	return r;
}

int SystematicAnalysis::Type_GetCompatibleScore(const IsCompatiblePar& Func, const Vector<ParInfo>& ValueTypes)
{
	bool IsNewFunc = false;

	size_t StartIndex = 0;

	if (Func.Item)
	{
		if (Func.Item->Type == SymbolType::Func)
		{
			auto Info = Func.Item->Get_Info<FuncInfo>();
			if (Info->_FuncType == FuncInfo::FuncType::New)
			{
				StartIndex++;
			}
			else if (Info->_FuncType == FuncInfo::FuncType::Invoke)
			{
				StartIndex++;
			}
		}
	}
	int r = 0;
	auto& Pars = *Func.Pars;

	bool unpackparpack = true;
	bool typepack = false;
	if (ValueTypes.size())
	{
		auto& lastpartype = ValueTypes.back().Type;
		auto symop = Symbol_GetSymbol(lastpartype);
		if (symop.has_value())
		{
			auto val = symop.value();

			if (val->Type == SymbolType::Type_Pack)
			{
				if (unpackparpack)
				{
					auto typelist = val->Get_Info<TypePackInfo>();
					typepack = true;
				}
			}
		}

	}

	OptionalRef<Vector<TypeSymbol>> _TypePack;
	if (unpackparpack && typepack)
	{
		auto& lastpartype = ValueTypes.back().Type;
		_TypePack = Optionalref(Symbol_GetSymbol(lastpartype).value()->Get_Info<TypePackInfo>()->List);
	}
	for (size_t i = StartIndex; i < (Pars).size(); i++)
	{
		size_t ValueTypesIndex = StartIndex == 1 ? i - 1 : i;

		ParInfo parinfo;
		if (_TypePack.has_value())
		{
			if (ValueTypesIndex >= ValueTypes.size() - 1)
			{
				auto newindex = i - (ValueTypes.size() - 1);

				parinfo.IsOutPar = false;
				parinfo.Type = _TypePack.value()[newindex];
			}
			else
			{
				parinfo = ValueTypes[ValueTypesIndex];
			}
		}
		else
		{
			auto& par = Pars[i];
			{
				auto symop = Symbol_GetSymbol(par.Type);
				if (symop.has_value())
				{
					auto sym = symop.value();

					if (sym->Type == SymbolType::Type_Pack || sym->Type == SymbolType::Unmaped_Generic_Type)
					{
						if (ValueTypesIndex >= ValueTypes.size())
						{
							continue;
						}
					}
				}
			}
			parinfo = ValueTypes[ValueTypesIndex];
		}

		r += Type_GetCompatibleScore(Pars[i], parinfo);
	}


	return  Pars.size() ? (int)(r / Pars.size()) : (int)r;
}
bool AreSameimmutable(const TypeSymbol& TypeA, const TypeSymbol& TypeB)
{
	return 	TypeA.IsAddress() == TypeB.IsAddress() &&
		TypeA.IsAddressArray() == TypeB.IsAddressArray() &&
		TypeA.Isimmutable() == TypeB.Isimmutable();
}
bool SystematicAnalysis::Type_AreTheSame(const TypeSymbol& TypeA, const TypeSymbol& TypeB)
{
	return Type_AreTheSameWithOutimmutable(TypeA, TypeB) && AreSameimmutable(TypeA, TypeB);
}
bool SystematicAnalysis::Type_AreTheSame(const ParInfo& TypeA, const ParInfo& TypeB)
{
	if (TypeA.IsOutPar != TypeB.IsOutPar)
	{
		return false;
	}

	return Type_AreTheSame(TypeA.Type, TypeB.Type);
}
bool  SystematicAnalysis::Type_AreTheSameWithOutMoveAndimmutable(const TypeSymbol& TypeA, const TypeSymbol& TypeB)
{
	if (TypeA.IsAddressArray() != TypeB.IsAddressArray())
	{
		return false;
	}
	if (TypeA.IsTypeInfo() != TypeB.IsTypeInfo())
	{
		return false;
	}

	if (TypeA._Type == TypesEnum::CustomType
		&& TypeB._Type == TypesEnum::CustomType)
	{
		if (TypeA._CustomTypeSymbol == TypeB._CustomTypeSymbol)
		{
			return true;
		}

		Symbol& TypeOne = *Symbol_GetSymbol(TypeA).value();
		Symbol& TypeTwo = *Symbol_GetSymbol(TypeB).value();
		if (TypeOne.Type == SymbolType::Func_ptr && TypeTwo.Type == SymbolType::Func_ptr)
		{
			FuncPtrInfo* F1 = TypeOne.Get_Info<FuncPtrInfo>();
			FuncPtrInfo* F2 = TypeTwo.Get_Info<FuncPtrInfo>();
			if (F1->Pars.size() != F2->Pars.size())
			{
				return false;
			}
			if (!Type_AreTheSameWithOutimmutable(F1->Ret, F2->Ret))
			{
				return false;
			}


			for (size_t i = 0; i < F1->Pars.size(); i++)
			{
				auto& Item1 = F1->Pars[i];
				auto& Item2 = F2->Pars[i];
				if (!Type_AreTheSameWithOutimmutable(Item1.Type, Item2.Type) || Item1.IsOutPar != Item2.IsOutPar)
				{
					return false;
				}
			}
			return true;
		}
		if (TypeOne.Type == SymbolType::Type_StaticArray && TypeTwo.Type == SymbolType::Type_StaticArray)
		{
			StaticArrayInfo* F1 = TypeOne.Get_Info<StaticArrayInfo>();
			StaticArrayInfo* F2 = TypeTwo.Get_Info<StaticArrayInfo>();

			if (F1->Count == F2->Count)
			{
				return Type_AreTheSameWithOutimmutable(F1->Type, F2->Type);
			}
		}
		if (Type_IsUnMapType(TypeOne) && Type_IsUnMapType(TypeTwo))
		{
			return true;
		}

	}
	else if ((Type_IsPrimitive(TypeA) && Type_IsPrimitive(TypeB)) && TypeA._Type == TypeB._Type
		)
	{
		return true;
	}



	return false;
}
bool SystematicAnalysis::Type_AreTheSameWithOutimmutable(const TypeSymbol& TypeA, const TypeSymbol& TypeB)
{
	if (TypeA._MoveData != TypeB._MoveData)
	{
		return false;
	}
	else
	{
		return Type_AreTheSameWithOutMoveAndimmutable(TypeA, TypeB);
	}
}
bool SystematicAnalysis::Type_HasDefaultConstructorFunc(const TypeSymbol& Type) const
{
	if (Type.IsAddress() == false)
	{
		auto symOp = Symbol_GetSymbol(Type);
		if (symOp.has_value())
		{
			auto sym = symOp.value();
			if (sym->Type == SymbolType::Type_class)
			{
				auto scopename = sym->FullName;
				ScopeHelper::GetApendedString(scopename, ClassConstructorfunc);

				for (auto& Item : GetSymbolsWithName(scopename))
				{
					if (Item->Type == SymbolType::Func)
					{
						auto funcinfo = Item->Get_Info<FuncInfo>();
						if (funcinfo->Pars.size() == 1)
						{
							return true;
						}
					}
				}

			}
		}
	}
	return false;
}
SystematicAnalysis::BinaryOverLoadWith_t SystematicAnalysis::Type_HasBinaryOverLoadWith(const TypeSymbol& TypeA, TokenType BinaryOp, const TypeSymbol& TypeB)
{

	if (Type_AreTheSameWithOutimmutable(TypeA, TypeB))
	{
		bool IsMathOp = BinaryOp == TokenType::plus
			|| BinaryOp == TokenType::minus
			|| BinaryOp == TokenType::star
			|| BinaryOp == TokenType::forwardslash;

		bool IsSameValueComparisonOp = BinaryOp == TokenType::equal_Comparison
			|| BinaryOp == TokenType::Notequal_Comparison;

		bool IsMathValueComparisonOp = BinaryOp == TokenType::greaterthan
			|| BinaryOp == TokenType::lessthan
			|| BinaryOp == TokenType::less_than_or_equalto
			|| BinaryOp == TokenType::greater_than_or_equalto;

		bool IsBitShift = BinaryOp == TokenType::bitwise_LeftShift
			|| BinaryOp == TokenType::bitwise_RightShift;

		bool Isbitwise = IsBitShift || BinaryOp == TokenType::bitwise_and
			|| BinaryOp == TokenType::bitwise_or
			|| BinaryOp == TokenType::bitwise_XOr;

		bool IslogicalOperator = BinaryOp == TokenType::logical_and
			|| BinaryOp == TokenType::logical_or;

		if (Type_IsIntType(TypeA))
		{
			return { IsMathOp || IsSameValueComparisonOp || IsMathValueComparisonOp || Isbitwise || BinaryOp == TokenType::modulo ,{} };
		}
		if (Type_IsfloatType(TypeA))
		{
			return { IsMathOp || IsSameValueComparisonOp || IsMathValueComparisonOp,{} };
		}

		if (Type_IsCharType(TypeA))
		{
			return { IsSameValueComparisonOp, {} };
		}
		if (TypeA._Type == TypesEnum::Bool)
		{
			return { IsSameValueComparisonOp || IslogicalOperator , {} };
		}

		if (TypeA._Type == TypesEnum::CustomType)
		{
			auto Syb = Symbol_GetSymbol(TypeA);
			if (Syb.value()->Type == SymbolType::Enum)
			{
				return { IsSameValueComparisonOp, {} };
			}
		}

	}

	auto SybOp = Symbol_GetSymbol(TypeA);
	if (SybOp)
	{
		auto Syb = SybOp.value();
		if (Syb->Type == SymbolType::Type_class)
		{

			for (auto& Item : Systematic_BinaryOverloadData::data)
			{
				if (Item.token == BinaryOp)
				{
					String funcName = Syb->FullName;
					ScopeHelper::GetApendedString(funcName, Item.CompilerName);

					auto V = GetSymbolsWithName(funcName, SymbolType::Func);
					Optional<Symbol*> BestFit;
					Optional<int> BestScore;

					for (auto& Item : V)
					{
						Symbol_Update_FuncSym_ToFixedTypes(NeverNullptr(Item));
						if (Item->Type == SymbolType::Func)
						{
							auto funcInfo = Item->Get_Info<FuncInfo>();
							if (funcInfo->Pars.size() == 2)
							{
								auto ParA = funcInfo->Pars[0];
								auto ParB = funcInfo->Pars[1];
								bool r = Type_CanBeImplicitConverted(TypeA, ParA.Type)
									&& Type_CanBeImplicitConverted(TypeB, ParA.Type);
								if (r)
								{
									ParInfo pinfo;
									pinfo.Type = TypeA;
									ParInfo pinfo2;
									pinfo2.Type = TypeB;

									int itemscore = (Type_GetCompatibleScore(pinfo, ParA) + Type_GetCompatibleScore(pinfo2, ParB)) / 2;

									if (!BestScore.has_value() || BestScore.value() < itemscore)
									{
										BestFit = Item;
										BestScore = itemscore;
									}
								}

							}
						}
					}

					if (BestFit.has_value())
					{
						return { true, BestFit.value() };
					}
					break;
				}
			}
		}
	}


	return {};
}
SystematicAnalysis::CompoundOverLoadWith_t SystematicAnalysis::Type_HasCompoundOverLoadWith(const TypeSymbol& TypeA, TokenType BinaryOp, const TypeSymbol& TypeB)
{
	if (Type_AreTheSameWithOutimmutable(TypeA, TypeB))
	{
		if (Type_IsIntType(TypeA) || Type_IsfloatType(TypeA))
		{
			return { true };
		}
	}

	auto SybOp = Symbol_GetSymbol(TypeA);
	if (SybOp)
	{
		auto Syb = SybOp.value();
		if (Syb->Type == SymbolType::Type_class)
		{

			for (auto& Item : Systematic_CompoundOverloadData::data)
			{
				if (Item.token == BinaryOp)
				{
					String funcName = Syb->FullName;
					ScopeHelper::GetApendedString(funcName, Item.CompilerName);

					auto V = GetSymbolsWithName(funcName, SymbolType::Func);

					for (auto& Item : V)
					{
						Symbol_Update_FuncSym_ToFixedTypes(NeverNullptr(Item));
						if (Item->Type == SymbolType::Func)
						{
							auto funcInfo = Item->Get_Info<FuncInfo>();
							if (funcInfo->Pars.size() == 2)
							{
								bool r = Type_CanBeImplicitConverted(TypeA, funcInfo->Pars[0].Type)
									&& Type_CanBeImplicitConverted(TypeB, funcInfo->Pars[1].Type);
								if (r)
								{
									return { r, Item };
								}

							}
						}
					}
					break;
				}
			}
		}
	}

	return {};
}
SystematicAnalysis::PostFixOverLoadWith_t SystematicAnalysis::Type_HasPostfixOverLoadWith(const TypeSymbol& TypeA, TokenType BinaryOp)
{

	if (Type_IsIntType(TypeA))
	{
		return { true,{} };
	}

	auto SybOp = Symbol_GetSymbol(TypeA);
	if (SybOp)
	{
		auto Syb = SybOp.value();
		if (Syb->Type == SymbolType::Type_class)
		{

			for (auto& Item : Systematic_PostfixOverloadData::data)
			{
				if (Item.token == BinaryOp)
				{
					String funcName = Syb->FullName;
					ScopeHelper::GetApendedString(funcName, Item.CompilerName);

					auto& V = GetSymbolsWithName(funcName, SymbolType::Func);

					for (auto& Item : V)
					{
						Symbol_Update_FuncSym_ToFixedTypes(NeverNullptr(Item));
						if (Item->Type == SymbolType::Func)
						{
							auto funcInfo = Item->Get_Info<FuncInfo>();
							if (funcInfo->Pars.size() == 1)
							{
								bool r = Type_CanBeImplicitConverted(TypeA, funcInfo->Pars[0].Type);
								if (r)
								{
									return { r, Item };
								}

							}
						}
					}
					break;
				}
			}
		}
	}

	return {  };
}
SystematicAnalysis::IndexOverLoadWith_t SystematicAnalysis::Type_HasIndexedOverLoadWith(const TypeSymbol& TypeA, const TypeSymbol& TypeB)
{
	if (Type_IsUnMapType(TypeB) || Type_IsUnMapType(TypeA))
	{
		return { true, {} };
	}

	if (Type_IsUIntType(TypeB))
	{
		if (TypeA.IsAddressArray())
		{
			return { true, {} };
		}

		if (Type_IsStaticArray(TypeA))
		{
			return { true,{} };
		}
	}

	auto SybOp = Symbol_GetSymbol(TypeA);
	if (SybOp)
	{
		auto Syb = SybOp.value();
		if (Syb->Type == SymbolType::Type_class)
		{

			String funcName = Syb->FullName;
			ScopeHelper::GetApendedString(funcName, Overload_Index_Func);

			auto V = GetSymbolsWithName(funcName, SymbolType::Func);

			for (auto& Item : V)
			{
				Symbol_Update_FuncSym_ToFixedTypes(NeverNullptr(Item));
				if (Item->Type == SymbolType::Func)
				{
					auto funcInfo = Item->Get_Info<FuncInfo>();
					if (funcInfo->Pars.size() == 2)
					{
						bool r = Type_CanBeImplicitConverted(TypeA, funcInfo->Pars[0].Type)
							&& Type_CanBeImplicitConverted(TypeB, funcInfo->Pars[1].Type);
						if (r)
						{
							return { r, Item };
						}

					}
				}
			}
		}
	}

	return {};
}
SystematicAnalysis::ForOverLoadWith_t SystematicAnalysis::Type_HasForOverLoadWith(const TypeSymbol& TypeA)
{
	auto SybOp = Symbol_GetSymbol(TypeA);
	if (SybOp)
	{
		auto Syb = SybOp.value();
		if (Syb->Type == SymbolType::Type_class)
		{

			String funcName = Syb->FullName;
			ScopeHelper::GetApendedString(funcName, Overload_For_Func);

			auto& V = GetSymbolsWithName(funcName, SymbolType::Func);

			for (auto& Item : V)
			{
				Symbol_Update_FuncSym_ToFixedTypes(NeverNullptr(Item));
				if (Item->Type == SymbolType::Func)
				{
					auto funcInfo = Item->Get_Info<FuncInfo>();
					if (funcInfo->Pars.size() == 1)
					{
						bool r = Type_CanBeImplicitConverted(TypeA, funcInfo->Pars[0].Type);
						if (r)
						{
							return { r, Item };
						}

					}
				}
			}
		}
	}

	return {};
}
SystematicAnalysis::UrinaryOverLoadWith_t SystematicAnalysis::Type_HasUrinaryOverLoadWith(const TypeSymbol& TypeA, TokenType Op)
{
	auto SybOp = Symbol_GetSymbol(TypeA);
	if (SybOp)
	{
		auto Syb = SybOp.value();
		if (Syb->Type == SymbolType::Type_class)
		{

			for (auto& Item : Systematic_UrinaryOverloadData::data)
			{
				if (Item.token == Op)
				{
					String funcName = Syb->FullName;
					ScopeHelper::GetApendedString(funcName, Item.CompilerName);

					auto& V = GetSymbolsWithName(funcName, SymbolType::Func);

					for (auto& Item : V)
					{
						Symbol_Update_FuncSym_ToFixedTypes(NeverNullptr(Item));
						if (Item->Type == SymbolType::Func)
						{
							auto funcInfo = Item->Get_Info<FuncInfo>();
							if (funcInfo->Pars.size() == 1)
							{
								bool r = Type_CanBeImplicitConverted(TypeA, funcInfo->Pars[0].Type);
								if (r)
								{
									return { r, Item };
								}

							}
						}
					}
					break;
				}
			}
		}
		else if (Syb->Type == SymbolType::Enum)
		{
			if (Op == TokenType::QuestionMark)
			{

				auto optinfo = IsOptionalType(TypeA);
				if (optinfo.has_value())
				{
					auto rettype = optinfo.value().SomeType;


					_LastExpressionType = rettype;//Should return rettype and not set _LastExpressionType

					return { {true} };
				}
				else
				{
					auto resultinfo = IsResultType(TypeA);

					if (resultinfo.has_value())
					{
						auto& resulttype = resultinfo.value();

						_LastExpressionType = resulttype.SomeType;

						return { {true} };
					}
				}
			}
		}
	}
	else
	{
		if (TypeA._Type == TypesEnum::Bool)
		{
			if (Op == TokenType::Not)
			{
				return { {true} };
			}
		}
		if (Type_IsIntType(TypeA) && Op == TokenType::bitwise_not)
		{
			return { {true} };
		}
	}
	return {  };
}
bool SystematicAnalysis::Type_IsCopyable(const TypeSymbol& Type)
{
	if (Type_IsPrimitive(Type))
	{
		return true;
	}
	if (auto val = Symbol_GetSymbol(Type))
	{
		auto& Syb = *val.value().value();

		switch (Syb.Type)
		{
		case SymbolType::Type_class:
		{
			auto v = Syb.Get_Info<ClassInfo>();
			bool r = v->_ClassAutoGenerateCopyConstructor
				|| v->_ClassHasCopyConstructor.has_value();

			if (r == false)
			{
				for (auto& Item : v->Fields)
				{
					if (!Type_IsCopyable(Item.Type))
					{
						return false;
					}
				}
				return true;
			}

			return r;
		}
		break;

		default:
			break;
		}
	}
	return true;
}
TypeSymbolID SystematicAnalysis::Type_GetTypeID(TypesEnum Type, SymbolID SymbolId)
{
	TypeSymbolID R = 0;
	switch (Type)
	{
	case TypesEnum::Void:
		R = 1;
		break;
	case TypesEnum::Bool:
		R = 2;
		break;
	case TypesEnum::Char:
		R = 3;
		break;
	case TypesEnum::uInt8:
		R = 4;
		break;
	case TypesEnum::uInt16:
		R = 5;
		break;
	case TypesEnum::uInt32:
		R = 6;
		break;
	case TypesEnum::uInt64:
		R = 7;
		break;
	case TypesEnum::sInt8:
		R = 8;
		break;
	case TypesEnum::sInt16:
		R = 9;
		break;
	case TypesEnum::sInt32:
		R = 10;
		break;
	case TypesEnum::uIntPtr:
		R = 11;
		break;
	case TypesEnum::sIntPtr:
		R = 12;
		break;
	case TypesEnum::float32:
		R = 13;
		break;
	case TypesEnum::float64:
		R = 14;
		break;
	case TypesEnum::CustomType:
	{
		auto Syb = Symbol_GetSymbol(SymbolId);
		String ReflectionName;
		if (Syb->Type == SymbolType::Type_StaticArray)
		{
			const StaticArrayInfo* info = Syb->Get_Info<StaticArrayInfo>();
			ReflectionName = ToString(info->Type) + "[/" + std::to_string(info->Count) + "]";
		}
		else
		{
			ReflectionName = Syb->FullName;
		}


		R = (ReflectionCustomTypeID)std::hash<String>()(ReflectionName);
	}
	break;
	default:
		R = 0;
		break;
	}
	return R;
}
void SystematicAnalysis::Type_Convert(const TypeNode& V, TypeSymbol& Out)
{
	switch (V._name._ScopedName.back()._token->Type)
	{
	case TokenType::KeyWorld_var:
		Out.SetType(TypesEnum::Var);
		break;
	case TokenType::KeyWord_UInt8:
		Out.SetType(TypesEnum::uInt8);
		break;
	case TokenType::KeyWord_UInt16:
		Out.SetType(TypesEnum::uInt16);
		break;
	case TokenType::KeyWord_UInt32:
		Out.SetType(TypesEnum::uInt32);
		break;
	case TokenType::KeyWord_UInt64:
		Out.SetType(TypesEnum::uInt64);
		break;

	case TokenType::KeyWord_SInt8:
		Out.SetType(TypesEnum::sInt8);
		break;
	case TokenType::KeyWord_SInt16:
		Out.SetType(TypesEnum::sInt16);
		break;
	case TokenType::KeyWord_SInt32:
		Out.SetType(TypesEnum::sInt32);
		break;
	case TokenType::KeyWord_SInt64:
		Out.SetType(TypesEnum::sInt64);
		break;

	case TokenType::KeyWord_uintptr:
		Out.SetType(TypesEnum::uIntPtr);
		break;
	case TokenType::KeyWord_sintptr:
		Out.SetType(TypesEnum::sIntPtr);
		break;

	case TokenType::KeyWord_Bool:
		Out.SetType(TypesEnum::Bool);
		break;
	case TokenType::KeyWord_Char:
		Out.SetType(TypesEnum::Char);
		break;
	case TokenType::KeyWord_uft8:
		Out.SetType(TypesEnum::Uft8);
		break;
	case TokenType::KeyWord_uft16:
		Out.SetType(TypesEnum::Uft16);
		break;
	case TokenType::KeyWord_uft32:
		Out.SetType(TypesEnum::Uft32);
		break;
	case TokenType::KeyWord_float32:
		Out.SetType(TypesEnum::float32);
		break;
	case TokenType::KeyWord_float64:
		Out.SetType(TypesEnum::float64);
		break;
	case TokenType::Void:
		Out.SetType(TypesEnum::Void);
		break;
	case TokenType::KeyWord_This:
	{

		if (_ClassStack.size())
		{
			Symbol* SybV = _ClassStack.top().Syb;
			if (SybV->Type == SymbolType::Generic_class)
			{
				Out = Type_GetUnMapType();
			}
			else
			{
				Out = SybV->VarType;
			}
		}
		else
		{
			LogError_CantUseThisHere(NeverNullptr(V._name._ScopedName.back()._token));
			Out.SetType(TypesEnum::Null);
		}
	}
	break;
	case TokenType::Name:
	{
		if (_PassType == PassType::GetTypes) { return; }
		auto Name = Str_GetScopedNameAsString(V._name);

		Symbol* SybV;
		_LastLookedAtToken = Nullableptr(V._name._ScopedName.back()._token);


		NullablePtr<UseGenericsNode> _generic;
		if (V._name._ScopedName.back()._generic.get())
		{
			_generic = V._name._ScopedName.back()._generic.get();
		}
		if (_generic.has_value() && _generic.value()->_Values.size())
		{
			const UseGenericsNode* _genericval = _generic.value().value();

			auto Val = Generic_InstantiateOrFindGenericSymbol(NeverNullptr(V._name._ScopedName.back()._token), *_genericval, Name);
			if (!Val.has_value()) { Out = Type_GetUnMapType(); return; }
			SybV = Val.value().value();
		}
		else
		{
			SybV = Symbol_GetSymbol(Name, SymbolType::Type).value_unchecked();
		}

		if (SybV == nullptr)
		{
			auto Token = V._name._ScopedName.back()._token;

			LogError_CantFindTypeError(NeverNullptr(Token), Name);
		}
		else
		{
			if (Symbol_AccessCheck(SybV, NeverNullptr(V._name._ScopedName.back()._token)))
			{
				Out.SetType(TypesEnum::Null);
				return;
			}



			if (SybV->Type == SymbolType::Type_alias || SybV->Type == SymbolType::Func_ptr)
			{
				Symbol_Update_Sym_ToFixedTypes(SybV);
				Out = SybV->VarType;
			}
			else if (SybV->Type == SymbolType::Hard_Type_alias
				|| SybV->Type == SymbolType::Enum
				|| SybV->Type == SymbolType::Type_class
				|| SybV->Type == SymbolType::Hard_Func_ptr)
			{
				Symbol_Update_Sym_ToFixedTypes(SybV);
				Out.SetType(SybV->ID);
			}
			else if (SybV->Type == SymbolType::Trait_class)
			{
				if (!V._IsDynamic)
				{
					auto Token = V._name._ScopedName.back()._token;
					LogError_TraitCantBeAlone(NeverNullptr(Token));
					Out.SetType(TypesEnum::Null);
				}
				else
				{
					Symbol_Update_Sym_ToFixedTypes(SybV);
					Out.SetType(SybV->ID);
				}
			}
			else if (SybV->Type == SymbolType::Unmaped_Generic_Type)
			{
				Out.SetType(SybV->ID);
			}
			else
			{
				LogError_WantedAType(V, SybV);
				Out.SetType(TypesEnum::Null);
			}
		}
	}break;
	case TokenType::internal_Constant_expression:
	{
		//note this can only happen in a generic substitution
		auto* node = V._node.get();
		if (_ConstantExpressionMap.HasValue(node))
		{
			auto& item = _ConstantExpressionMap.GetValue(node);
			Out.SetType(item);
		}
		else
		{
			SymbolID id = Symbol_GetSymbolID(node);
			auto& Syb = Symbol_AddSymbol(SymbolType::ConstantExpression, "?", "?", AccessModifierType::Private);
			_Table.AddSymbolID(Syb, id);

			ConstantExpressionInfo* info = new ConstantExpressionInfo();
			info->Exnode = ExpressionNodeType::As(node);
			info->Context = Save_SymbolContext();

			_LookingForTypes.push(_LookingForTypes.top());

			auto IsOk = Eval_EvaluateToAnyType(*info->Exnode);

			_LookingForTypes.pop();

			if (IsOk)
			{
				info->Ex = IsOk.value().EvaluatedObject;
				Syb.VarType = IsOk.value().Type;
			}

			Syb.Info.reset(info);


			Out.SetType(id);
		}
	}
	break;
	case TokenType::KeyWord_TypeInfo:
	{
		Out.SetType(TypesEnum::Var);
		Out.SetAsTypeInfo();
	}
	break;
	case TokenType::KeyWord_bind:
	{
		if (_PassType != PassType::GetTypes)
		{
			auto ExNode = ExpressionNodeType::As(V._node.get());


			auto IsOk = Eval_EvaluateToAnyType(*ExNode);

			if (IsOk.has_value())
			{
				auto& Object = IsOk.value();

				if (!Object.Type.IsTypeInfo())
				{
					Out.SetType(TypesEnum::Null);

					auto Token = NeverNullptr(V._name._ScopedName.back()._token);
					auto& Type = Object.Type;
					LogError_CantBindTypeItNotTypeInfo(Token, Type);
				}
				else
				{
					auto* TypeSyb = Eval_Get_ObjectAs<TypeSymbol>(Object);
					Out = *TypeSyb;
					Out.BindType();
				}
			}
			else
			{
				Out.SetType(TypesEnum::Null);
			}
		}
		else
		{
			Out.SetType(TypesEnum::Null);
		}

	}break;
	case TokenType::internal_InlineEnumVariant:
	{
		const auto& TypeList = V._name._ScopedName.back()._generic->_Values;
		Vector<TypeSymbol> outtypelist;
		outtypelist.resize(TypeList.size());

		bool fail = false;
		for (size_t i = 0; i < TypeList.size(); i++)
		{
			auto& typenode = TypeList[i];
			auto& outype = outtypelist[i];
			Type_Convert(typenode, outype);
			if (!Type_ValidateType(outype, NeverNullptr(V._name._ScopedName.front()._token), NodeSyb_t::Any) || outype.IsBadType())
			{
				fail = true;
				break;
			}
		}

		if (fail)
		{
			Out.SetType(TypesEnum::Null);
		}
		else
		{

			bool hassametype = false;
			TypeSymbol* sametype = nullptr;
			for (auto& Item : outtypelist)
			{
				for (auto& Item2 : outtypelist)
				{
					if (&Item2 != &Item)
					{
						if (Type_AreTheSameWithOutimmutable(Item, Item2))
						{
							sametype = &Item;
							hassametype = true;
							break;
						}

					}
				}
				if (hassametype)
				{
					break;
				}
			}

			if (hassametype)
			{
				Out.SetType(TypesEnum::Null);

				String MSG;
				MSG += "has multiple of exact same type of '";
				MSG += ToString(*sametype);
				MSG += "'";
				LogError(ErrorCodes::InValidType, MSG, NeverNullptr(V._name._ScopedName.back()._token));
			}
			else
			{
				std::sort(outtypelist.begin(), outtypelist.end(), [this](TypeSymbol& A, TypeSymbol& B)
					{
						auto aid = Type_GetTypeID(A._Type, A._CustomTypeSymbol);
						auto bid = Type_GetTypeID(B._Type, B._CustomTypeSymbol);
						return aid > bid;
					});
				String SymName = CompilerGeneratedStart;
				SymName += "inlineenum_";
				for (auto& Item : outtypelist)
				{
					SymName += ToString(Item);
					SymName += "_";
				}
				SymName += CompilerGeneratedEnd;


				//This may causes type depemdency problems between
				if (auto oldsyb = Symbol_GetSymbol(SymName, SymbolType::Enum))
				{
					Out.SetType(oldsyb.value()->ID);
				}
				else
				{
					auto& Syb = Symbol_AddSymbol(SymbolType::Enum, SymName, SymName, AccessModifierType::Public);
					{
						Syb.PassState = PassType::FixedTypes;
						_Table.AddSymbolID(Syb, Symbol_GetSymbolID(&Syb));

						EnumInfo* info = new EnumInfo();
						Syb.Info.reset(info);

						info->FullName = SymName;
						info->Basetype = TypesEnum::uInt8;
						if (outtypelist.size() > UInt8_MaxSize)
						{
							LogError(ErrorCodes::InValidType, "bad inline enum more then 255 values", NeverNullptr(V._name._ScopedName.back()._token));
						}

						Byte EnumValue = 0;
						for (auto& Item : outtypelist)
						{
							EnumFieldInfo F;

							String valuename;
							switch (Item._Type)
							{
							case TypesEnum::Bool:valuename = "Bool"; break;
							case TypesEnum::Char:valuename = "Char"; break;

							case TypesEnum::Uft8:valuename = "Uft8"; break;
							case TypesEnum::Uft16:valuename = "Uft16"; break;
							case TypesEnum::Uft32:valuename = "Uft32"; break;
							case TypesEnum::sInt8:valuename = "Int8"; break;

							case TypesEnum::sInt16:valuename = "Int16"; break;
							case TypesEnum::sInt32:valuename = "Int32"; break;
							case TypesEnum::sInt64:valuename = "Int64"; break;
							case TypesEnum::sIntPtr:valuename = "IntPtr"; break;

							case TypesEnum::uInt8:valuename = "UInt8"; break;
							case TypesEnum::uInt16:valuename = "UInt16"; break;
							case TypesEnum::uInt32:valuename = "UInt32"; break;
							case TypesEnum::uInt64:valuename = "UInt64"; break;
							case TypesEnum::uIntPtr:valuename = "UIntPtr"; break;
							case TypesEnum::float32:valuename = "Float32"; break;
							case TypesEnum::float64:valuename = "Float64"; break;
							case TypesEnum::CustomType:
							{
								auto syb = Symbol_GetSymbol(Item);
								valuename = ScopeHelper::GetNameFromFullName(syb.value()->FullName);
							}
							break;

							default:
								UCodeLangUnreachable();
								break;
							}

							F.Ex.ObjectSize = sizeof(EnumValue);
							F.Ex.Object_AsPointer.reset(new Byte[sizeof(EnumValue)]);
							memcpy(F.Ex.Object_AsPointer.get(), &EnumValue, sizeof(EnumValue));

							F.Name = valuename;

							info->Fields.push_back(std::move(F));
							EnumValue++;
						}

						EnumVariantData Variantdata;
						for (auto& Item : outtypelist)
						{
							EnumVariantField F;
							F.Types.push_back(Item);

							Variantdata.Variants.push_back(std::move(F));
						}

						info->VariantData = std::move(Variantdata);

						_InlineEnums.push_back(NeverNullptr(&Syb));
					}

					Out.SetType(Syb.ID);
				}
			}
		}

	}break;
	default:
		UCodeLangUnreachable();
		break;
	}
	if (V._IsAddess) { Out._IsAddress = true; }
	if (V._IsAddessArray) { Out._IsAddressArray = true; }
	if (V._Isimmutable) { Out._Isimmutable = true; }

	if (V._IsTypedMoved) { Out._MoveData = MoveData::Moved; }

	if (V._IsDynamic)
	{
		auto syb = Symbol_GetSymbol(Out);
		bool IsTrait = false;
		if (syb)
		{
			IsTrait = syb.value()->Type == SymbolType::Trait_class;
		}

		if (!IsTrait)
		{
			LogError_DynamicMustBeRrait(V, Out);
			Out.SetType(TypesEnum::Null);
		}
		Out._IsDynamic = true;
	}

	if (V._IsStackArray)
	{
		ExpressionNodeType* node = (ExpressionNodeType*)V._node.get();
		SymbolID id = Symbol_GetSymbolID(V);
		auto BaseTypeName = ToString(Out);
		auto FullName = CompilerGenerated("StaticArray_") + BaseTypeName + std::to_string(id.AsInt());

		if (_PassType == PassType::GetTypes)
		{
			return;
		}

		Symbol* Syb = Symbol_GetSymbol(FullName, SymbolType::Null).value_unchecked();
		if (Syb == nullptr)
		{
			Syb = &Symbol_AddSymbol(SymbolType::Type_StaticArray, FullName, FullName, AccessModifierType::Public);
			_Table.AddSymbolID(*Syb, id);

			Syb->PassState = _PassType;

			StaticArrayInfo* info = new StaticArrayInfo();


			info->Type = Out;
			info->Exnode = node;

			Syb->Info.reset(info);

		}
		StaticArrayInfo& Info = *Syb->Get_Info<StaticArrayInfo>();

		if (_PassType == PassType::FixedTypes && node != nullptr && Info.IsCountInitialized == false)
		{
			TypeSymbol UIntType;
			UIntType.SetType(TypesEnum::uIntPtr);


			_LookingForTypes.push(UIntType);
			auto IsOk = Eval_Evaluate(UIntType, *node);
			_LookingForTypes.pop();

			if (IsOk)
			{
				size_t V = *Eval_Get_ObjectAs<size_t>(IsOk.value());

				Info.Count = V;
				Info.IsCountInitialized = true;

			}
			else
			{
				Info.Count = 0;
				Info.IsCountInitialized = true;
				Info.IsCountError = true;
			}

			if (Symbol_HasDestructor(Info.Type))
			{
				auto DropFuncName = ToString(TypeSymbol(Syb->ID));
				ScopeHelper::GetApendedString(DropFuncName, ClassDestructorFunc);


				auto DropFunc = &Symbol_AddSymbol(SymbolType::Func, ClassDestructorFunc, DropFuncName, AccessModifierType::Public);
				FuncInfo* V = new FuncInfo();
				DropFunc->Info.reset(V);

				V->FullName = DropFunc->FullName;
				V->_FuncType = FuncInfo::FuncType::Drop;
				V->Ret = TypesEnum::Void;
				V->FrontParIsUnNamed = true;

				auto ThisParType = TypeSymbol(Syb->ID);
				ThisParType._IsAddress = true;

				ParInfo parinfo;
				parinfo.IsOutPar = false;
				parinfo.Type = ThisParType;
				V->Pars.push_back(parinfo);

				DropFunc->PassState = PassType::BuidCode;
			}
		}
		Out.SetType(Syb->ID);
		if (Type_IsUnMapType(Info.Type))
		{
			Syb->OutputIR = false;
		}

	}
}
void SystematicAnalysis::Type_ConvertAndValidateType(const TypeNode& V, TypeSymbol& Out, NodeSyb_t Syb)
{
	Type_Convert(V, Out);
	if (Type_ValidateType(Out, NeverNullptr(V._name._ScopedName.back()._token), Syb) == false)
	{
		Out.SetType(TypesEnum::Null);
	}
	if (Syb == NodeSyb_t::Parameter)
	{
		Out.SetAsLocation();
	}
}
bool SystematicAnalysis::Type_ValidateType(const TypeSymbol& V, const NeverNullPtr<Token> Token, NodeSyb_t Syb)
{
	if (V._Type == TypesEnum::CustomType)
	{
		auto Syb2 = Symbol_GetSymbol(V).value();
		if (Syb2->Type == SymbolType::Type_StaticArray)
		{
			StaticArrayInfo* V = Syb2->Get_Info<StaticArrayInfo>();

			if (!V->IsCountInitialized)
			{
				//Is Checked
				//LogError_BeMoreSpecifiicWithStaticArrSize(Token, V->Type);
				//return false;
			}
		}
		if (Syb2->Type == SymbolType::ConstantExpression && Syb != NodeSyb_t::Any)
		{
			auto V = Syb2->FullName;
			LogError_CantFindTypeError(Token, V);
		}
	}


	if (V._MoveData == MoveData::Moved && Syb != NodeSyb_t::Parameter)
	{
		LogError_CantUseMoveTypeHere(Token);
	}

	return true;
}



TypeSymbol SystematicAnalysis::Type_ConvertAndValidateType(const TypeNode& V, NodeSyb_t Syb)
{
	TypeSymbol r;
	Type_ConvertAndValidateType(V, r, Syb);
	return r;
}

bool SystematicAnalysis::Type_IsValid(TypeSymbol& Out)
{
	return false;
}
bool SystematicAnalysis::Type_CanBeImplicitConverted(const TypeSymbol& TypeToCheck, const TypeSymbol& Type, bool ReassignMode, bool isdeclare)
{
	if (Type_AreTheSameWithOutMoveAndimmutable(TypeToCheck, Type))
	{
		bool V0 = Type_IsimmutableRulesfollowed(TypeToCheck, Type);

		bool V1 = Type_IsAddessAndLValuesRulesfollowed(TypeToCheck, Type, ReassignMode, isdeclare);

		if (TypeToCheck._ValueInfo != TypeValueInfo::IsValue
			&& !TypeToCheck.IsMovedType() && (!Type_IsCopyable(TypeToCheck))
			&& !Type.IsAddress())
		{
			return false;
		}

		return V0 && V1;
	}


	if (Type_CanDoTypeToTrait(TypeToCheck, Type))
	{

		if (TypeToCheck.IsAddress() || TypeToCheck._ValueInfo == TypeValueInfo::IsLocation)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	if (auto syb = Symbol_GetSymbol(Type).value_unchecked())
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
						if (Type_AreTheSame(par, TypeToCheck))
						{
							return true;
						}
					}
				}

			}
		}
	}


	return false;
}
TypeSymbol SystematicAnalysis::Type_GetUnMapType()
{
	if (!_Type_UnMapTypeSymbol.has_value())
	{
		auto& TypeSyb = Symbol_AddSymbol(SymbolType::Unmaped_Generic_Type, CompilerGenerated("UnMapedType"), CompilerGenerated("UnMapedType"), AccessModifierType::Public);
		_Table.AddSymbolID(TypeSyb, Symbol_GetSymbolID(&TypeSyb));
		_Type_UnMapTypeSymbol = TypeSyb.ID;
	}
	return TypeSymbol(_Type_UnMapTypeSymbol.value());

}
bool SystematicAnalysis::Type_IsUnMapType(const TypeSymbol& Type) const
{
	auto Syb = Symbol_GetSymbol(Type);
	if (Syb)
	{
		return Type_IsUnMapType(*Syb.value_unchecked());
	}
	return {};
}
bool SystematicAnalysis::Type_IsUnMapType(const Symbol& Syb) const
{
	return Syb.Type == SymbolType::Unmaped_Generic_Type;
}

UCodeLangFrontEnd

#endif