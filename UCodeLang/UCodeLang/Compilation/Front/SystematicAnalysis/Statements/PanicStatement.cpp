#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnPanicStatement(const PanicNode& node)
{
	if (_PassType == PassType::GetTypes)
	{
		OnExpressionTypeNode(node._StringExpression,GetValueMode::Read);
	}
	else if (_PassType == PassType::FixedTypes)
	{
		_LookingForTypes.push(Type_GetStringSpan(NeverNullptr(node._KeyWord)).value_or(TypesEnum::Any));

		OnExpressionTypeNode(node._StringExpression, GetValueMode::Read);
	
		_LookingForTypes.pop();

		auto ex = _LastExpressionType;

		bool isstrlike = false;

		auto typesyb = Symbol_GetSymbol(ex);
		if (typesyb)
		{
			auto typev = typesyb.value();
			if (Type_IsStringSpan(ex) || Type_IsString(ex))
			{
				isstrlike = true;

				bool hasdatafunc = false;
				bool hassizefunc = false;
			
				TypeSymbol dataret;
				dataret.SetAsAddressArray();
				dataret._Type = TypesEnum::Char;

				auto ptrtype = ex;
				ptrtype._IsAddress = true;

				NullablePtr<Symbol> func1Op;
				{
					auto& symbols = GetSymbolsWithName(ScopeHelper::ApendedStrings(typev->FullName, "Data"));

					for (auto& func1 : symbols)
					{
						if (func1->Type == SymbolType::Func)
						{
							auto funcinfo = func1->Get_Info<FuncInfo>();

							if (funcinfo->Pars.size() == 1
								&& funcinfo->Pars[0].Type._CustomTypeSymbol == ptrtype._CustomTypeSymbol)
							{
								if (Type_AreTheSame(funcinfo->Ret, dataret))
								{
									hasdatafunc = true;
									func1Op = func1;
									break;
								}
							}
						}
					}
				}

				NullablePtr<Symbol> func2Op;
				{
					auto& symbols = GetSymbolsWithName(ScopeHelper::ApendedStrings(typev->FullName, "Size"));
					
					for (auto& func2 : symbols)
					{
						if (func2->Type == SymbolType::Func)
						{
							auto funcinfo = func2->Get_Info<FuncInfo>();

							if (funcinfo->Pars.size() == 1
								&& funcinfo->Pars[0].Type._CustomTypeSymbol == ptrtype._CustomTypeSymbol)
							{

								if (funcinfo->Ret._Type == TypesEnum::uIntPtr)
								{
									hassizefunc = true;
									func2Op = func2;
									break;
								}
							}
						}
					}
				}


				if (hasdatafunc ==false)
				{
					auto func = ScopeHelper::ApendedStrings(typev->FullName, "Data");

					Vector<TypeSymbol> par;
					par.push_back(ptrtype);
					
					LogError_CantFindFuncError(
						NeverNullptr(node._KeyWord),
						(String_view)func,
						{},
						par, dataret);
				}
				if (hassizefunc== false)
				{
					auto func = ScopeHelper::ApendedStrings(typev->FullName, "Size");

					Vector<TypeSymbol> par;
					par.push_back(ptrtype);

					LogError_CantFindFuncError(
						NeverNullptr(node._KeyWord),
						(String_view)func,
						{},
						par, TypesEnum::uIntPtr);
				}
			}
		}

		if (isstrlike == false)
		{
			LogError_CantCastImplicitTypes(NeverNullptr(node._KeyWord)
				, ex,Type_GetStringSpan(NeverNullptr(node._KeyWord)).value_or(TypeSymbol())
				,false);
		}

	}
	else if (_PassType == PassType::BuidCode)
	{
		_LookingForTypes.push(Type_GetStringSpan(NeverNullptr(node._KeyWord)).value_or(TypesEnum::Any));

		OnExpressionTypeNode(node._StringExpression, GetValueMode::Read);

		_LookingForTypes.pop();

		auto ex = _LastExpressionType;
		auto irval = _IR_LastExpressionField;

		if (Type_IsStringSpan(ex) || Type_IsString(ex))
		{
			auto  typev = Symbol_GetSymbol(ex).value();

			auto func1 = Symbol_GetSymbol(ScopeHelper::ApendedStrings(typev->FullName, "Data"), SymbolType::Func).value();
			auto func2 = Symbol_GetSymbol(ScopeHelper::ApendedStrings(typev->FullName, "Size"), SymbolType::Func).value();

			auto func1info = func1->Get_Info<FuncInfo>();
			auto func2info = func2->Get_Info<FuncInfo>();


			IRInstruction* inputpar = nullptr;
			if (ex.IsAddress())
			{
				inputpar = irval;
			}
			else
			{
				inputpar = _IR_LookingAtIRBlock->NewLoadPtr(irval);
			}

			_IR_LookingAtIRBlock->NewPushParameter(inputpar);
			IRInstruction* pointer = _IR_LookingAtIRBlock->NewCall(IR_GetIRID(func1info));


			_IR_LookingAtIRBlock->NewPushParameter(inputpar);
			IRInstruction* size = _IR_LookingAtIRBlock->NewCall(IR_GetIRID(func2info));

			
			_IR_LookingAtIRBlock->ThrowException(pointer, size);
			auto v = _IR_LookingAtIRBlock->NewLoad((bool)0);//padding for if statements
			v->SetAsNone();
		}
		else
		{
			UCodeLangUnreachable();
		}
	}
}

UCodeLangFrontEnd

#endif
