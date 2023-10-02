#include "UCodeLang/Compliation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart
void SystematicAnalysis::OnAnonymousObjectConstructor(const AnonymousObjectConstructorNode* nod)
{
	auto& Type = Type_Get_LookingForType();

	if (Type._Type == TypesEnum::CustomType)
	{
		auto V = Symbol_GetSymbol(Type).value();
		if (V->Type == SymbolType::Type_StaticArray)
		{
			StaticArrayInfo* StaticArr = V->Get_Info< StaticArrayInfo>();
			if (_PassType == PassType::FixedTypes)
			{

				if (StaticArr->IsCountInitialized)
				{
					if (StaticArr->Count != nod->_Fields._Nodes.size())
					{
						const NeverNullPtr<Token> Token = _LastLookedAtToken.value();
						LogError_CanIncorrectStaticArrCount(Token, Type, nod->_Fields._Nodes.size(), StaticArr->Count);
						_LastExpressionType.SetType(TypesEnum::Null);
						return;
					}
				}
				else
				{
					StaticArr->Count = nod->_Fields._Nodes.size();
					StaticArr->IsCountInitialized = true;
				}

				const auto& ArrItemType = StaticArr->Type;
				for (size_t i = 0; i < nod->_Fields._Nodes.size(); i++)
				{
					OnExpressionTypeNode(nod->_Fields._Nodes[i].get(), GetValueMode::Read);

					if (!Type_CanBeImplicitConverted(_LastExpressionType, ArrItemType, false))
					{
						LogError_CantCastImplicitTypes(_LastLookedAtToken.value(), _LastExpressionType, ArrItemType, false);
					}
				}
			}
			if (_PassType == PassType::BuidCode)
			{
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

				for (size_t i = 0; i < nod->_Fields._Nodes.size(); i++)
				{
					OnExpressionTypeNode(nod->_Fields._Nodes[i].get(), GetValueMode::Read);

					IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, ArrItemType);

					auto V = _IR_LookingAtIRBlock->New_Index_Vetor(BufferIRIns, IR_Load_UIntptr(i), ValueSizeIR);

					_IR_LookingAtIRBlock->NewDereferenc_Store(V, _IR_LastExpressionField);
				}
			}

			_LastExpressionType = Type;
			return;
		}
	}

	if (Type.IsnotAn(TypesEnum::Var))//function who called this can deal with var
	{


		if (_PassType == PassType::FixedTypes)
		{
			auto Func = Type_GetFunc(Type, nod->_Fields);
			_FuncToSyboID.AddValue(Symbol_GetSymbolID(nod),Func);

			Type_SetFuncRetAsLastEx(Func);
			return;
		}
		else if (_PassType == PassType::BuidCode)
		{
			const auto& Func = _FuncToSyboID.GetValue(Symbol_GetSymbolID(nod));
			auto& ValuePars = nod->_Fields;



			IR_Build_FuncCall(Type, Func, ValuePars);

			Type_SetFuncRetAsLastEx(Func);
		}
	}
	else
	{
		if (_PassType == PassType::FixedTypes)
		{
			{
				TypeSymbol AnyType; AnyType.SetType(TypesEnum::Any);
				_LookingForTypes.push(AnyType);
			}

			TypeSymbol ArrItemType;
			if (nod->_Fields._Nodes.size())
			{
				OnExpressionTypeNode(nod->_Fields._Nodes[0].get(), GetValueMode::Read);
				ArrItemType = _LastExpressionType;
				_LookingForTypes.top() = ArrItemType;
			}
			else
			{
				ArrItemType.SetType(TypesEnum::uInt8);
			}


			TypeSymbol NewType;
			StaticArrayInfo* info = nullptr;
			{
				SymbolID id = Symbol_GetSymbolID(nod);
				auto FullName = CompilerGenerated("StaticArray_") + ToString(ArrItemType) + std::to_string(id.AsInt());


				auto Syb = &Symbol_AddSymbol(SymbolType::Type_StaticArray, FullName, FullName, AccessModifierType::Public);
				_Table.AddSymbolID(*Syb, id);

				Syb->PassState = _PassType;

				info = new StaticArrayInfo();


				info->Type = ArrItemType;
				info->Exnode = nullptr;

				Syb->Info.reset(info);

				NewType.SetType(id);
			}


			info->Count = nod->_Fields._Nodes.size();
			info->IsCountInitialized = true;


			for (size_t i = 1; i < nod->_Fields._Nodes.size(); i++)
			{
				OnExpressionTypeNode(nod->_Fields._Nodes[i].get(), GetValueMode::Read);

				if (!Type_CanBeImplicitConverted(_LastExpressionType, ArrItemType, false))
				{
					LogError_CantCastImplicitTypes(_LastLookedAtToken.value(), _LastExpressionType, ArrItemType, false);
				}
			}

			_LastExpressionType = NewType;
			{
				_LookingForTypes.pop();
			}
			return;
		}
	}
}
UCodeLangFrontEnd