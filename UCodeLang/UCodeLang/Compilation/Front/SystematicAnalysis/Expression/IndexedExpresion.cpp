#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnExpressionNode(const IndexedExpresionNode& node)
{
	if (_LookingForTypes.size() && _LookingForTypes.top()._Type == TypesEnum::Var)
	{
		auto& top = _LookingForTypes.top();

		TypeSymbol V;
		V.SetType(TypesEnum::Any);
		V._IsAddress = top.IsAddress();

		_LookingForTypes.push(V);
	}
	else
	{
		_LookingForTypes.push(_LookingForTypes.top());
	}

	if (_PassType == PassType::GetTypes)
	{
		OnExpressionTypeNode(node._SourceExpression._Value.get(), _GetExpressionMode.top());
		OnExpressionTypeNode(node._IndexExpression._Value.get(), GetValueMode::Read);
	}

	if (_PassType == PassType::FixedTypes)
	{
		auto lookingfortype = Type_Get_LookingForType();
		OnExpressionTypeNode(node._SourceExpression._Value.get(), _GetExpressionMode.top());
		TypeSymbol SourcType = _LastExpressionType;


		TypeSymbol gesstype;
		gesstype.SetType(TypesEnum::Any);


		bool IsSrcAddress = SourcType.IsAddressArray();
		bool IsSrcStaticArray = Type_IsStaticArray(SourcType);

		//This is mostly only here for the range types.
		Optional<String> TypeHint;
		{
			auto ex = node._IndexExpression._Value.get();
	
			if (ex->Get_Type() == NodeType::ValueExpressionNode)
			{
				auto valex = ValueExpressionNode::As(ex)->_Value.get();

				if (valex->Get_Type() == NodeType::FuncCallNode)
				{
					auto Funccall = FuncCallNode::As(valex);

					String scopename;
					Funccall->_FuncName.GetScopedName(scopename);


					auto symbols = GetSymbolsWithName(scopename);

					for (auto Item : symbols)
					{
						if (Item->Type == SymbolType::Func || Item->Type == SymbolType::GenericFunc)
						{
							auto funcnode = Item->Get_NodeInfo<FuncNode>();
						
							if (funcnode != nullptr)
							{
								auto& retnode = funcnode->_Signature._ReturnType;

								String str;
								retnode._name.GetScopedName(str);
								
								TypeHint = std::move(str);
							}
						}

					}
			}
			}	
		}

		if (IsSrcAddress || IsSrcStaticArray)
		{
			gesstype.SetType(TypesEnum::uIntPtr);
		}
		else
		{
			auto SybOp = Symbol_GetSymbol(SourcType);
			if (SybOp)
			{
				auto Syb = SybOp.value();
				if (Syb->Type == SymbolType::Type_class)
				{
					String funcName = Syb->FullName;
					ScopeHelper::GetApendedString(funcName, Overload_Index_Func);

					auto V = GetSymbolsWithName(funcName, SymbolType::Func);
				
					if (TypeHint.has_value())
					{
						auto& typehint = TypeHint.value();

						for (auto& Item : V)
						{
							FuncInfo* func = Item->Get_Info<FuncInfo>();

							if (func->Pars.size() == 2) 
							{
								Symbol_Update_FuncSym_ToFixedTypes(Item);

								auto parstring = ToString(func->Pars[1]);

								if (StringHelper::Contains(parstring,typehint))
								{
									gesstype = func->Pars[1].Type;
									break;
								}
							}
						}
					}
				
					if (gesstype._Type == TypesEnum::Any)
					{
						gesstype.SetType(TypesEnum::uIntPtr);
					}
				}
			}
		}

		_LookingForTypes.push(gesstype);


		OnExpressionTypeNode(node._IndexExpression._Value.get(), GetValueMode::Read);
		TypeSymbol IndexType = _LastExpressionType;


		_LookingForTypes.pop();

		auto HasInfo = Type_HasIndexedOverLoadWith(SourcType, IndexType);
		if (!HasInfo.HasValue)
		{
			auto  Token =NeverNullptr(node._token);
			LogError_CantBeIndexWithType(Token, SourcType, IndexType);

		}
		else
		{
			bool isnativearraytype = false;
			if (SourcType.IsAddressArray())
			{
				isnativearraytype = true;
			}
			else
			{
				auto SymOp = Symbol_GetSymbol(SourcType);
				if (auto Sym = SymOp.value_unchecked())
				{
					if (Sym->Type == SymbolType::Type_StaticArray)
					{
						isnativearraytype = true;
					}
				}
			}

			if (isnativearraytype)
			{
				if (!IsInUnSafeBlock())
				{
					auto Token = NeverNullptr(node._token);
					LogError(ErrorCodes::ExpectingSequence, Token->OnLine, Token->OnPos, "Cant Index Native Array Type '" + ToString(SourcType) + "' in safe mode.");
				}
			}
		}




		IndexedExpresion_Data V;



		//all float bool int types
		if (!HasInfo.HasValue)
		{
			_LastExpressionType = TypesEnum::Null;
		}
		else
		{
			if (HasInfo.Value.has_value())
			{
				FuncInfo* f = HasInfo.Value.value()->Get_Info<FuncInfo>();

				V.Op0 = f->Pars[0].Type;
				V.Op1 = f->Pars[1].Type;
				V.FuncToCall = HasInfo.Value.value();

				_LastExpressionType = f->Ret;

				TypeSymbol lookingfor = _LookingForTypes.top();
				if (!lookingfor.IsAddress() && !IsWrite(_GetExpressionMode.top()))
				{
					_LastExpressionType._IsAddress = false;
					_LastExpressionType._Isimmutable = false;
				}
			}
			else
			{

				TypeSymbol lookingfor = _LookingForTypes.top();
				V.Op0 = SourcType;


				V.Op0._IsAddress = true;


				V.Op1 = IndexType;
				V.Op1._IsAddress = false;

				if (IsSrcStaticArray)
				{
					auto Syb = Symbol_GetSymbol(SourcType).value();
					lookingfor = Syb->Get_Info<StaticArrayInfo>()->Type;

					if (lookingfortype.IsAddressArray())
					{

						lookingfor.SetAsAddressArray();
					}
					else
					{
						if (lookingfortype.IsAddress() || IsWrite(_GetExpressionMode.top()))
						{
							lookingfor.SetAsAddress();
						}
					}

					_LastExpressionType = lookingfor;
				}
				else if (lookingfortype.IsAddressArray())
				{
					lookingfor = SourcType;
					_LastExpressionType = lookingfor;
				}
				else
				{
					auto newtype = SourcType;
					if (IsWrite(_GetExpressionMode.top()))
					{
						newtype.SetAsAddress();
					}
					newtype._IsAddressArray = false;

					if (!lookingfor.IsAddress())
					{
						newtype._Isimmutable = false;
					}
					_LastExpressionType = newtype;
				}
			}
		}



		auto symid = Symbol_GetSymbolID(node);
		
		if (!_IndexedExpresion_Datas.HasValue(symid)) 
		{
			_IndexedExpresion_Datas.AddValue(symid, V);
		}

	}


	if (_PassType == PassType::BuidCode)
	{
		auto& Data = _IndexedExpresion_Datas.GetValue(Symbol_GetSymbolID(node));

		if (Data.FuncToCall)
		{
			FuncInfo* f = Data.FuncToCall->Get_Info<FuncInfo>();

			Get_FuncInfo V;
			V.Func = f;
			V.SymFunc = Data.FuncToCall;
			V.ThisPar = Get_FuncInfo::ThisPar_t::NoThisPar;


			ScopedNameNode Tep;
			ScopedName TepV;
			TepV._token = _LastLookedAtToken.value_unchecked();
			Tep._ScopedName.push_back(TepV);

			ValueParametersNode pars;
			pars._Nodes.push_back(Unique_ptr<Node>(node._SourceExpression._Value.get()));
			pars._Nodes.push_back(Unique_ptr<Node>(node._IndexExpression._Value.get()));

			IR_Build_FuncCall(V, Tep, pars);

			//save so not free mem
			auto par0 = pars._Nodes[0].release();
			auto par1 = pars._Nodes[1].release();
			//its ok.no mem leak Par node has Unique_ptr to SourceExpression and IndexExpression just borrowing them
			_LastExpressionType = V.Func->Ret;



			if (_LastExpressionType.IsAddress()) {
				bool LookCopyByValue = !(_LookingForTypes.top().IsAddress());

				if (LookCopyByValue)
				{
					bool CopyByValue = _LastExpressionType.IsAddress();

					if (CopyByValue && !IsWrite(_GetExpressionMode.top()))
					{
						auto rawtype = _LastExpressionType;
						rawtype._IsAddress = false;
						rawtype._ValueInfo = TypeValueInfo::IsValue;

						_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad_Dereferenc(_IR_LastExpressionField
							, IR_ConvertToIRType(rawtype));

						_LastExpressionType = rawtype;
					}
				}
			}
			
		}
		else
		{
			_LookingForTypes.push(Data.Op0);
			OnExpressionTypeNode(node._SourceExpression._Value.get(), GetValueMode::Read);
			_LookingForTypes.pop();

			auto Pointer = _IR_LastExpressionField;
			auto SourcType = _LastExpressionType;

			_LookingForTypes.push(Data.Op1);
			OnExpressionTypeNode(node._IndexExpression._Value.get(), GetValueMode::Read);
			_LookingForTypes.pop();

			auto IndexField = _IR_LastExpressionField;

			TypeSymbol IndexedObjectPointer = Data.Op0;
			IndexedObjectPointer._IsAddressArray = false;
			IndexedObjectPointer._IsAddress = false;

			size_t V;
			{
				auto VSybOp = Symbol_GetSymbol(IndexedObjectPointer);
				if (VSybOp)
				{
					auto VSyb = VSybOp.value();
					if (VSyb->Type == SymbolType::Type_StaticArray)
					{
						StaticArrayInfo* info = VSyb->Get_Info<StaticArrayInfo>();
						Type_GetSize(info->Type, V);
					}
					else
					{
						Type_GetSize(IndexedObjectPointer, V);
					}
				}
				else
				{
					Type_GetSize(IndexedObjectPointer, V);
				}


			}
			if (Type_IsStaticArray(SourcType) && !SourcType.IsAddress())
			{
				Pointer = _IR_LookingAtIRBlock->NewLoadPtr(Pointer);
			}


			_IR_LastExpressionField = _IR_LookingAtIRBlock->New_Index_Vetor(Pointer, IndexField,IR_Load_UIntptr(V));

			TypeSymbol lookingfor = _LookingForTypes.top();
			{
				if (lookingfor.IsAddressArray())
				{
					lookingfor = SourcType;
					_LastExpressionType = lookingfor;
				}
				else if (Type_IsStaticArray(SourcType))
				{
					auto Syb = Symbol_GetSymbol(SourcType).value();

					bool isaddress = lookingfor.IsAddress();

					lookingfor = Syb->Get_Info<StaticArrayInfo>()->Type;

					if (isaddress) {
						lookingfor.SetAsAddress();
					}

					_LastExpressionType = lookingfor;
				}
				else
				{
					lookingfor = SourcType;
					if (IsWrite(_GetExpressionMode.top()))
					{
						lookingfor.SetAsAddress();
					}
					lookingfor._IsAddressArray = false;
					lookingfor._ValueInfo = TypeValueInfo::IsValue;
					_LastExpressionType = lookingfor;
				}

			}

			auto copy = lookingfor;
			_LastExpressionType = lookingfor;


			auto beforex = _IR_LastExpressionField;

			if (IsRead(_GetExpressionMode.top()))
			{
				bool LookCopyByValue = _LookingForTypes.top().IsAddress()
					|| _LookingForTypes.top().IsAddressArray();
				if (LookCopyByValue == false)
				{
					if (_LookingForTypes.top().IsMovedType() && HasMoveContructerHasIRFunc(_LastExpressionType))
					{
						LookCopyByValue = true;
					}
				}

				if (LookCopyByValue == false)
				{
					bool CopyByValue = _LastExpressionType.IsAddress() ||
						_LastExpressionType.IsAddressArray();

					if (CopyByValue || !IsWrite(_GetExpressionMode.top()))
					{
						auto v = _LookingForTypes.top();
						v._MoveData = MoveData::None;

						_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad_Dereferenc(_IR_LastExpressionField
							, IR_ConvertToIRType(v));

						_LastExpressionType._IsAddress = false;
						_LastExpressionType._MoveData = MoveData::None;
					}
				}
				
			}

			if (IsWrite(_GetExpressionMode.top()))
			{
				_IR_LastStoreField = IROperator(beforex);
				_LastExpressionType = lookingfor;
			}

			if (_GetExpressionMode.top() == GetValueMode::ReadAndWrite && 
				_LookingForTypes.top()._IsAddress ==true)
			{
				auto p = _LookingForTypes.top();
				p._IsAddress = false;

				_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad_Dereferenc(_IR_LastExpressionField
						, IR_ConvertToIRType(p));

			}
		}

	}

	_LookingForTypes.pop();
}


UCodeLangFrontEnd

#endif
