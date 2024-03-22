#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnCompileTimeforNode(const CompileTimeForNode& node)
{
	if (_PassType == PassType::GetTypes)
	{
		if (node._Type == CompileTimeForNode::ForType::Traditional)
		{
			auto Token = node._Name;
			LogError(ErrorCodes::ExpectingSequence, Token->OnLine, Token->OnPos, "Traditional for loops are not yet added to CompileTimeforNode");
		}
		else
		{

		}
	}
	else if (_PassType == PassType::FixedTypes)
	{
		if (node._Type == CompileTimeForNode::ForType::modern)
		{

			_LookingForTypes.push(TypesEnum::Any);
			OnExpressionTypeNode(node._Modern_List._Value.get(), GetValueMode::Read);
			_LookingForTypes.pop();

			auto ListType = _LastExpressionType;

			bool CanBeLooped = false;
			Symbol* ListTypeSyb;
			if (ListType._CustomTypeSymbol.HasValue())
			{
				ListTypeSyb = Symbol_GetSymbol(ListType).value().value();
				if (ListTypeSyb)
				{
					if (ListTypeSyb->Type == SymbolType::Type_Pack)
					{
						CanBeLooped = true;
					}
					else
						if (ListTypeSyb->Type == SymbolType::Type_StaticArray)
						{
							CanBeLooped = true;
						}
						else if (Type_IsUnMapType(*ListTypeSyb))
						{
							CanBeLooped = true;
						}
				}
			}


			if (!CanBeLooped)
			{
				auto Token = node._Name;
				LogError(ErrorCodes::ExpectingSequence, Token->OnLine, Token->OnPos, "Cant Loop over type of '" + ToString(ListType) + "'");
			}
			else
			{

				{//test loop for Errs
					size_t OldErrCount = _ErrorsOutput->Get_Errors().size();
					_Table.AddScope("___forTest");
					const String VarableName = (String)node._Name->Value._String;

					auto& ParSyb = Symbol_AddSymbol(SymbolType::Unmaped_Varable, VarableName, _Table._Scope.GetApendedString(VarableName), AccessModifierType::Public);
					ParSyb.VarType = Type_GetUnMapType();
					_Table.AddSymbolID(ParSyb, Symbol_GetSymbolID(&ParSyb));
					{
						auto Token = NeverNullptr(node._Name);
						Push_ExtendedErr("Were '" + VarableName + "' is unmaped for Errors.Before for loop expansion", Token);
					}
					CompileTimeforNodeEvaluateStatements(node);
					{
						Pop_ExtendedErr();
					}
					_Table.RemoveScope();


					bool GotErrs = OldErrCount != _ErrorsOutput->Get_Errors().size();
					if (GotErrs) { return; }
				}


				//
				if (ListTypeSyb->Type == SymbolType::Type_Pack)
				{
					const Node* NodePtr = node._Modern_List._Value.get();
					if (NodePtr->Get_Type() == NodeType::ValueExpressionNode)
					{
						const ValueExpressionNode* ValueNodePtr = ValueExpressionNode::As(NodePtr);
						const auto ExValuePtr = ValueNodePtr->_Value.get();
						if (ExValuePtr->Get_Type() == NodeType::ReadVariableNode)
						{
							const auto ReadVarablePtr = ReadVariableNode::As(ExValuePtr);

							_GetExpressionMode.push(GetValueMode::Read);
							GetMemberTypeSymbolFromVar_t V;
							bool VBool = Symbol_MemberTypeSymbolFromVar(ReadVarablePtr->_VariableName, V);
							_GetExpressionMode.pop();

							if (VBool)
							{
								auto ParSyb = V._Symbol;
								if (ParSyb->Type == SymbolType::ParameterVarable)
								{
									const ParameterInfo* ParSybInfo = ParSyb->Get_Info<ParameterInfo>();

									const TypePackInfo* PackInfo = ListTypeSyb->Get_Info<TypePackInfo>();

									const String ScopeName = std::to_string(Symbol_GetSymbolID(node).AsInt());
									const String VarableName = (String)node._Name->Value._String;

									CompileTimeforNode TepData;
									TepData.SybToLoopOver = V._Symbol;


									for (size_t i = 0; i < PackInfo->List.size(); i++)
									{
										auto& Item = PackInfo->List[i];

										_Table.AddScope(ScopeName + std::to_string(i));


										auto& ParSyb = Symbol_AddSymbol(SymbolType::ParameterVarable, VarableName, _Table._Scope.GetApendedString(VarableName), AccessModifierType::Public);

										{
											ParameterInfo* ParInfo = new ParameterInfo();
											*ParInfo = *ParSybInfo;
											ParSyb.Info.reset(ParInfo);
										}

										_Table.AddSymbolID(ParSyb, Symbol_GetSymbolID(&ParSyb));
										ParSyb.VarType = Item;

										size_t OldErrCount = _ErrorsOutput->Get_Errors().size();
										{
											auto Token = NeverNullptr(node._Name);
											Push_ExtendedErr("Were '" + VarableName + "' is type of " + ToString(ParSyb.VarType), Token);
										}
										CompileTimeforNodeEvaluateStatements(node);
										{
											Pop_ExtendedErr();
										}

										TepData.SybItems.push_back(&ParSyb);


										_Table.RemoveScope();

										bool GotErrs = OldErrCount != _ErrorsOutput->Get_Errors().size();
										if (GotErrs) { continue; }
									}

									_ForNodes.AddValue(Symbol_GetSymbolID(node), std::move(TepData));
								}
							}
						}
					}

				}
				else if (ListTypeSyb->Type == SymbolType::Type_StaticArray)
				{
					const StaticArrayInfo* StaticInfo = ListTypeSyb->Get_Info<StaticArrayInfo>();


					const String ScopeName = std::to_string(Symbol_GetSymbolID(node).AsInt());
					const String VarableName = (String)node._Name->Value._String;

					auto ListArray = Eval_Evaluate(ListType, node._Modern_List);
					if (ListArray.has_value())
					{
						size_t ItemSize = Type_GetSize(StaticInfo->Type).value();

						RawEvaluatedObject _DataAsIndex;
						_DataAsIndex.ObjectSize = ItemSize;
						_DataAsIndex.Object_AsPointer.reset(new Byte[ItemSize]);

						CompileTimeforNode TepData;
						TepData.SybToLoopOver = ListTypeSyb;

						auto& ListArrayValue = ListArray.value();

						for (size_t i = 0; i < StaticInfo->Count; i++)
						{
							void* ItemOffset = ListArrayValue.EvaluatedObject.Object_AsPointer.get() + (i * ItemSize);
							Eval_Set_ObjectAs(StaticInfo->Type, _DataAsIndex, ItemOffset, ItemSize);

							_Table.AddScope(ScopeName + std::to_string(i));


							auto& ParSyb = Symbol_AddSymbol(SymbolType::ConstantExpression, VarableName, _Table._Scope.GetApendedString(VarableName), AccessModifierType::Public);
							_Table.AddSymbolID(ParSyb, Symbol_GetSymbolID(&ParSyb));


							ConstantExpressionInfo* ContInfo = new ConstantExpressionInfo();
							ParSyb.Info.reset(ContInfo);

							ContInfo->Ex = _DataAsIndex;
							ParSyb.VarType = StaticInfo->Type;

							size_t OldErrCount = _ErrorsOutput->Get_Errors().size();
							{
								auto Token = NeverNullptr(node._Name);
								Push_ExtendedErr("Were '" + VarableName + "' = " + ToString(ParSyb.VarType, ContInfo->Ex), Token);
							}
							CompileTimeforNodeEvaluateStatements(node);
							{
								Pop_ExtendedErr();
							}
							TepData.SybItems.push_back(&ParSyb);




							_Table.RemoveScope();

							bool GotErrs = OldErrCount != _ErrorsOutput->Get_Errors().size();
							if (GotErrs) { continue; }
						}


						_ForNodes.AddValue(Symbol_GetSymbolID(node), std::move(TepData));
					}
				}
				else if (Type_IsUnMapType(*ListTypeSyb))
				{

				}
				else
				{
					UCodeLangUnreachable();
				}
			}
		}
	}
	else if (_PassType == PassType::BuidCode)
	{
		CompileTimeforNode& Nodes = _ForNodes.GetValue(Symbol_GetSymbolID(node));
		if (Nodes.SybToLoopOver->Type == SymbolType::ParameterVarable)
		{
			const String ScopeName = std::to_string(Symbol_GetSymbolID(node).AsInt());

			for (size_t i = 0; i < Nodes.SybItems.size(); i++)
			{
				auto& Item = Nodes.SybItems[i];

				size_t IRParIndex = _IR_LookingAtIRFunc->Pars.size() - Nodes.SybItems.size() + i;
				Item->IR_Par = &_IR_LookingAtIRFunc->Pars[IRParIndex];

				{
					auto Token = NeverNullptr(node._Name);
					Push_ExtendedErr("Were '" + (String)node._Name->Value._String + "' is type of " + ToString(Item->VarType), Token);
				}
				_Table.AddScope(ScopeName + std::to_string(i));

				for (const auto& node2 : node._Body._Nodes)
				{
					OnStatement(*node2);
				}

				_Table.RemoveScope();
				{
					Pop_ExtendedErr();
				}
			}
		}
		else if (Nodes.SybToLoopOver->Type == SymbolType::Type_StaticArray)
		{
			const String ScopeName = std::to_string(Symbol_GetSymbolID(node).AsInt());


			for (size_t i = 0; i < Nodes.SybItems.size(); i++)
			{
				auto& Item = Nodes.SybItems[i];

				_Table.AddScope(ScopeName + std::to_string(i));
				{
					ConstantExpressionInfo* ConstExpressionInfo = Item->Get_Info< ConstantExpressionInfo>();
					auto Token = NeverNullptr(node._Name);
					Push_ExtendedErr("Were '" + ScopeHelper::GetNameFromFullName(Item->FullName) + "' = " + ToString(Item->VarType, ConstExpressionInfo->Ex), Token);
				}
				for (const auto& node2 : node._Body._Nodes)
				{
					OnStatement(*node2);
				}
				{
					Pop_ExtendedErr();
				}
				_Table.RemoveScope();
			}
		}
	}
}


UCodeLangFrontEnd

#endif