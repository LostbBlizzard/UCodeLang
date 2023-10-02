#include "UCodeLang/Compliation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnMatchStatement(const MatchStatement& node)
{
	if (_PassType == PassType::GetTypes)
	{
		OnExpressionTypeNode(node._Expression, GetValueMode::Read);

		size_t ScopeCounter = 0;
		const String ScopeName = std::to_string((uintptr_t)&node);

		for (auto& Item : node._Arms)
		{

			_Table.AddScope(ScopeName + std::to_string(ScopeCounter));

			//OnExpressionTypeNode(Item._Expression, GetValueMode::Read);

			for (auto& Statement : Item._Statements._Nodes)
			{
				OnStatement(*Statement);
			}
			_Table.RemoveScope();

			ScopeCounter++;
		}


		if (node._InvaidCase.has_value())
		{
			_Table.AddScope(ScopeName + std::to_string(ScopeCounter));

			for (auto& Statement : node._InvaidCase.value()._Nodes)
			{
				OnStatement(*Statement);
			}

			_Table.RemoveScope();
		}
	}
	else if (_PassType == PassType::FixedTypes)
	{
		OnExpressionTypeNode(node._Expression, GetValueMode::Read);

		size_t ScopeCounter = 0;

		auto ToMatchType = _LastExpressionType;

		const String ScopeName = std::to_string((uintptr_t)&node);


		MatchStatementData V;

		for (auto& Item : node._Arms)
		{
			_Table.AddScope(ScopeName + std::to_string(ScopeCounter));

			Type_CanMatch(ToMatchType, node._Expression, Item._Expression, V.ArmData);

			for (auto& Statement : Item._Statements._Nodes)
			{
				OnStatement(*Statement);
			}

			_Table.RemoveScope();

			ScopeCounter++;
		}

		if (node._InvaidCase.has_value())
		{
			_Table.AddScope(ScopeName + std::to_string(ScopeCounter));

			for (auto& Statement : node._InvaidCase.value()._Nodes)
			{
				OnStatement(*Statement);
			}

			_Table.RemoveScope();
		}
		else
		{
			TryError_AllValuesAreMatched(ToMatchType, V.ArmData);
		}

		_MatchStatementDatas.AddValue(Symbol_GetSymbolID(node), std::move(V));
	}
	else if (_PassType == PassType::BuidCode)
	{
		size_t ScopeCounter = 0;
		const String ScopeName = std::to_string((uintptr_t)&node);

		OnExpressionTypeNode(node._Expression, GetValueMode::Read);

		auto Ex = _IR_LastExpressionField;
		auto ToMatchType = _LastExpressionType;

		MatchStatementData& V = _MatchStatementDatas.GetValue(Symbol_GetSymbolID(node));


		BuildMatch_State State;

		for (size_t i = 0; i < node._Arms.size(); i++)
		{
			auto& Item = node._Arms[i];

			auto& VItem = V.ArmData.Arms[i];

			_Table.AddScope(ScopeName + std::to_string(ScopeCounter));

			auto V = IR_Build_Match(ToMatchType, node._Expression, Ex, State, VItem, Item._Expression);

			for (auto& Statement : Item._Statements._Nodes)
			{
				OnStatement(*Statement);
			}

			IR_Build_Match(V, State);

			State.MatchList.push_back(std::move(V));

			_Table.RemoveScope();

			ScopeCounter++;
		}

		if (node._InvaidCase.has_value())
		{
			auto& Item = node._InvaidCase.value();

			_Table.AddScope(ScopeName + std::to_string(ScopeCounter));

			auto V = IR_Build_InvaildMatch(ToMatchType, Ex, State);

			for (auto& Statement : Item._Nodes)
			{
				OnStatement(*Statement);
			}

			IR_Build_Match(V, State);

			State.MatchList.push_back(std::move(V));

			_Table.RemoveScope();
		}
		else
		{
			auto V = IR_Build_InvaildMatch(ToMatchType, Ex, State);

			_IR_LookingAtIRBlock->NewUnreachable();

			IR_Build_Match(V, State);

			State.MatchList.push_back(std::move(V));
		}

		IR_Build_MatchState(State);
	}
}
bool SystematicAnalysis::MatchShouldOutPassEnumValue(const ExpressionNodeType& node)
{
	ValueExpressionNode* Val = ValueExpressionNode::As(node._Value.get());
	if (Val->_Value->Get_Type() == NodeType::FuncCallNode)
	{
		const FuncCallNode* Call = FuncCallNode::As(Val->_Value.get());

		for (auto& Item : Call->Parameters._Nodes)
		{
			if (Item->Get_Type() == NodeType::OutExpression)
			{
				return true;
			}

		}

	}
	return false;
}
void SystematicAnalysis::MatchAutoPassEnumValueStart(MatchAutoPassEnum& V, const ExpressionNodeType& node, const ValueExpressionNode* Val, const FuncCallNode* Call)
{
	V.Func._FuncName._ScopedName = Call->_FuncName._ScopedName;

	V.Func.Parameters._Nodes.resize(Call->Parameters._Nodes.size() + 1);
	V.Func.Parameters._Nodes[0].reset(node._Value.get());
	memcpy(V.Func.Parameters._Nodes.data() + 1, Call->Parameters._Nodes.data(), Call->Parameters._Nodes.size() * sizeof(Unique_ptr<Node>));


	V.NewNode._Value.reset(&V.NewValNode);
	V.NewValNode._Value.reset(&V.Func);
}
void SystematicAnalysis::MatchAutoPassEnd(MatchAutoPassEnum& V)
{
	V.NewNode._Value.release();
	V.NewValNode._Value.release();
	for (auto& Item : V.Func.Parameters._Nodes)
	{
		Item.release();
	}
}
void SystematicAnalysis::Type_CanMatch(const TypeSymbol& MatchItem, const ExpressionNodeType& MatchValueNode, const ExpressionNodeType& node, MatchArmData& Data)
{
	bool IsJust =
		MatchItem._IsAddressArray == false
		&& MatchItem._IsDynamic == false
		&& MatchItem._TypeInfo == TypeInfoPrimitive::Null;

	bool IsOk = false;
	if (IsJust)
	{
		if (Type_IsIntType(MatchItem) ||
			Type_IsfloatType(MatchItem._Type) ||
			Type_IsCharType(MatchItem._Type) ||
			MatchItem._Type == TypesEnum::Bool)
		{

			if (node._Value.get()->Get_Type() == NodeType::ValueExpressionNode)
			{
				_LookingForTypes.push(MatchItem);
				OnExpressionTypeNode(node, GetValueMode::Read);
				_LookingForTypes.pop();

				auto Type = _LastExpressionType;
				if (!Type_CanBeImplicitConverted(MatchItem, Type, false))
				{
					const NeverNullPtr<Token> token = _LastLookedAtToken.value();
					LogError_CantCastImplicitTypes(token, MatchItem, Type, false);
				}

				Data.Arms.push_back({});

				IsOk = true;
			}
			else
			{
				const NeverNullPtr<Token> token = _LastLookedAtToken.value();
				LogError(ErrorCodes::InValidType, token->OnLine, token->OnPos, "The Expression cant be Matched use only ValueExpression");
			}

		}
		else
		{
			auto SybOp = Symbol_GetSymbol(MatchItem);
			if (SybOp)
			{
				auto Syb = SybOp.value();
				if (Syb->Type == SymbolType::Enum)
				{
					if (node._Value.get()->Get_Type() == NodeType::ValueExpressionNode)
					{

						auto Arm = MatchArm();
						Arm._AutoPassEnum = std::make_shared<MatchAutoPassEnum>();
						if (MatchShouldOutPassEnumValue(node))
						{

							const ValueExpressionNode* Val = ValueExpressionNode::As(node._Value.get());
							const FuncCallNode* Call = FuncCallNode::As(Val->_Value.get());


							auto& Ptr = Arm.Get_AutoPassEnum();

							MatchAutoPassEnumValueStart(Ptr, MatchValueNode, Val, Call);

							_NodeTypeStack.push_back(NodeType::MatchArm);
							_LookingForTypes.push(MatchItem);
							OnExpressionTypeNode(Ptr.NewNode, GetValueMode::Read);
							_LookingForTypes.pop();
							_NodeTypeStack.pop_back();

							//MatchAutoPassEnd(Ptr);

							_LastExpressionType = MatchItem;
						}
						else
						{
							_NodeTypeStack.push_back(NodeType::MatchArm);
							_LookingForTypes.push(MatchItem);
							OnExpressionTypeNode(node, GetValueMode::Read);
							_LookingForTypes.pop();
							_NodeTypeStack.pop_back();
						}
						auto Type = _LastExpressionType;
						if (!Type_CanBeImplicitConverted(MatchItem, Type, false))
						{
							const NeverNullPtr<Token> token = _LastLookedAtToken.value();
							LogError_CantCastImplicitTypes(token, MatchItem, Type, false);
						}

						Data.Arms.push_back(std::move(Arm));

						IsOk = true;
					}
					else
					{
						const NeverNullPtr<Token> token = _LastLookedAtToken.value();
						LogError(ErrorCodes::InValidType, token->OnLine, token->OnPos, "The Expression cant be Matched use only ValueExpression");
					}
				}
				else
				{
					if (node._Value.get()->Get_Type() == NodeType::ValueExpressionNode)
					{

						auto Arm = MatchArm();
						Arm._AutoPassEnum = std::make_shared<MatchAutoPassEnum>();
						if (MatchShouldOutPassEnumValue(node))
						{

							const ValueExpressionNode* Val = ValueExpressionNode::As(node._Value.get());
							const FuncCallNode* Call = FuncCallNode::As(Val->_Value.get());


							auto& Ptr = Arm.Get_AutoPassEnum();

							MatchAutoPassEnumValueStart(Ptr, MatchValueNode, Val, Call);

							_NodeTypeStack.push_back(NodeType::MatchArm);
							_LookingForTypes.push(MatchItem);
							OnExpressionTypeNode(Ptr.NewNode, GetValueMode::Read);
							_LookingForTypes.pop();
							_NodeTypeStack.pop_back();

							//MatchAutoPassEnd(Ptr);

							_LastExpressionType = MatchItem;
						}
						else
						{
							_NodeTypeStack.push_back(NodeType::MatchArm);
							_LookingForTypes.push(MatchItem);
							OnExpressionTypeNode(node, GetValueMode::Read);
							_LookingForTypes.pop();
							_NodeTypeStack.pop_back();
						}
						auto Type = _LastExpressionType;
						if (!Type_CanBeImplicitConverted(MatchItem, Type, false))
						{
							const NeverNullPtr<Token> token = _LastLookedAtToken.value();
							LogError_CantCastImplicitTypes(token, MatchItem, Type, false);
						}

						Data.Arms.push_back(std::move(Arm));

						IsOk = true;
					}
					else
					{
						const NeverNullPtr<Token> token = _LastLookedAtToken.value();
						LogError(ErrorCodes::InValidType, token->OnLine, token->OnPos, "The Expression cant be Matched use only ValueExpression");
					}
				}
			}
		}
	}

	if (IsOk == false)
	{
		const NeverNullPtr<Token> token = _LastLookedAtToken.value();
		LogError(ErrorCodes::InValidType, token->OnLine, token->OnPos, "The type '" + ToString(MatchItem) + "' cant be Matched");
	}
}
void SystematicAnalysis::TryError_AllValuesAreMatched(const TypeSymbol& MatchItem, const MatchArmData& Data)
{
	if (MatchItem.IsAn(TypesEnum::Bool))
	{
		bool ValuesSet[2] = { false,false };
		for (auto& Item : Data.Arms)
		{

		}
	}
}
SystematicAnalysis::BuildMatch_ret SystematicAnalysis::IR_Build_Match(const TypeSymbol& MatchItem, const ExpressionNodeType& MatchValueNode, IRInstruction* Item, BuildMatch_State& State, MatchArm& Arm, const ExpressionNodeType& ArmEx)
{
	bool IsJust =
		MatchItem._IsAddressArray == false
		&& MatchItem._IsDynamic == false
		&& MatchItem._TypeInfo == TypeInfoPrimitive::Null;


	size_t EndMatchIndex = _IR_LookingAtIRBlock->GetIndex();
	if (State.MatchList.size())
	{
		auto& Last = State.MatchList.back();

		_IR_LookingAtIRBlock->UpdateConditionaJump(Last.JumpToUpdateIFMatchTrue.ConditionalJump, Last.JumpToUpdateIFMatchTrue.logicalNot, EndMatchIndex);
	}

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
				OnExpressionTypeNode(ArmEx, GetValueMode::Read);
				_LookingForTypes.pop();

				auto Type = _LastExpressionType;
				auto ArmExIR = _IR_LastExpressionField;
				auto IRToTest = _IR_LookingAtIRBlock->NewC_Equalto(Item, ArmExIR);


				SystematicAnalysis::BuildMatch_ret R;
				R.JumpToUpdateIFMatchTrue = _IR_LookingAtIRBlock->NewConditionalFalseJump(IRToTest, 0);
				return R;
			}
			else
			{
				UCodeLangUnreachable();
			}
		}
		else
		{
			auto Syb = Symbol_GetSymbol(MatchItem).value();
			if (Syb->Type == SymbolType::Enum)
			{
				auto eInfo = Syb->Get_Info<EnumInfo>();
				if (MatchShouldOutPassEnumValue(ArmEx))
				{
					const ValueExpressionNode* Val = ValueExpressionNode::As(ArmEx._Value.get());
					const FuncCallNode* Call = FuncCallNode::As(Val->_Value.get());

					auto& Ptr = Arm.Get_AutoPassEnum();

					//MatchAutoPassEnumValueStart(Ptr, MatchValueNode, Val, Call);


					_LookingForTypes.push(MatchItem);
					OnExpressionTypeNode(Ptr.NewNode, GetValueMode::Read);
					_LookingForTypes.pop();

					MatchAutoPassEnd(Ptr);

					auto Type = _LastExpressionType;
					auto ArmExIR = _IR_LastExpressionField;

					SystematicAnalysis::BuildMatch_ret R;
					R.JumpToUpdateIFMatchTrue = _IR_LookingAtIRBlock->NewConditionalFalseJump(ArmExIR, 0);
					return R;
				}
				else
				{
					_LookingForTypes.push(MatchItem);
					OnExpressionTypeNode(ArmEx, GetValueMode::Read);
					_LookingForTypes.pop();

					auto Type = _LastExpressionType;
					auto ArmExIR = _IR_LastExpressionField;

					if (eInfo->VariantData.has_value())
					{
						const IRStruct* B = _IR_Builder.GetSymbol(IR_ConvertToIRType(Type)._symbol)->Get_ExAs<IRStruct>();
						Item = _IR_LookingAtIRBlock->New_Member_Access(Item, B, EnumVarantKeyIndex);
						ArmExIR = _IR_LookingAtIRBlock->New_Member_Access(ArmExIR, B, EnumVarantKeyIndex);
					}

					auto IRToTest = _IR_LookingAtIRBlock->NewC_Equalto(Item, ArmExIR);


					SystematicAnalysis::BuildMatch_ret R;
					R.JumpToUpdateIFMatchTrue = _IR_LookingAtIRBlock->NewConditionalFalseJump(IRToTest, 0);
					return R;
				}
			}
			else
			{
				UCodeLangUnreachable();
			}
		}
	}

	SystematicAnalysis::BuildMatch_ret R;
	return R;
}
SystematicAnalysis::BuildMatch_ret SystematicAnalysis::IR_Build_InvaildMatch(const TypeSymbol& MatchItem, IRInstruction* Item, BuildMatch_State& State)
{
	size_t EndMatchIndex = _IR_LookingAtIRBlock->GetIndex();
	if (State.MatchList.size())
	{
		auto& Last = State.MatchList.back();

		_IR_LookingAtIRBlock->UpdateConditionaJump(Last.JumpToUpdateIFMatchTrue.ConditionalJump, Last.JumpToUpdateIFMatchTrue.logicalNot, EndMatchIndex);
	}
	return BuildMatch_ret();
}
void SystematicAnalysis::IR_Build_Match(BuildMatch_ret& Value, BuildMatch_State& State)
{
	size_t EndMatchIndex = _IR_LookingAtIRBlock->GetIndex();

	Value.JumpToUpdateEndIndex = _IR_LookingAtIRBlock->NewJump();
}
void SystematicAnalysis::IR_Build_MatchState(BuildMatch_State& State)
{
	size_t EndIndex = _IR_LookingAtIRBlock->GetIndex();
	for (auto& Item : State.MatchList)
	{
		_IR_LookingAtIRBlock->UpdateJump(Item.JumpToUpdateEndIndex, EndIndex);

	}
}
void SystematicAnalysis::OnMatchExpression(const MatchExpression& node)
{
	if (_PassType == PassType::GetTypes)
	{

		OnExpressionTypeNode(node._Expression, GetValueMode::Read);

		size_t ScopeCounter = 0;
		const String ScopeName = std::to_string((uintptr_t)&node);

		for (auto& Item : node._Arms)
		{

			_Table.AddScope(ScopeName + std::to_string(ScopeCounter));

			OnExpressionTypeNode(Item._Expression, GetValueMode::Read);

			OnExpressionTypeNode(Item._AssignmentExpression, GetValueMode::Read);
		}


		if (node._InvaidCase.has_value())
		{
			_Table.AddScope(ScopeName + std::to_string(ScopeCounter));

			OnExpressionTypeNode(node._InvaidCase.value(), GetValueMode::Read);

			_Table.RemoveScope();
		}
	}
	else if (_PassType == PassType::FixedTypes)
	{
		OnExpressionTypeNode(node._Expression, GetValueMode::Read);

		size_t ScopeCounter = 0;

		auto ToMatchType = _LastExpressionType;

		auto MatchAssignmentType = _LookingForTypes.top();

		const String ScopeName = std::to_string((uintptr_t)&node);


		MatchExpressionData V;

		for (auto& Item : node._Arms)
		{
			_Table.AddScope(ScopeName + std::to_string(ScopeCounter));

			Type_CanMatch(ToMatchType, node._Expression, Item._Expression, V.ArmData);

			OnExpressionTypeNode(Item._AssignmentExpression, GetValueMode::Read);
			auto AssignmentType = _LastExpressionType;
			if (!Type_CanBeImplicitConverted(AssignmentType, MatchAssignmentType))
			{
				const NeverNullPtr<Token> token = _LastLookedAtToken.value();
				LogError_CantCastImplicitTypes(token, MatchAssignmentType, AssignmentType, false);
			}


			_Table.RemoveScope();

			ScopeCounter++;
		}



		if (node._InvaidCase.has_value())
		{
			_Table.AddScope(ScopeName + std::to_string(ScopeCounter));

			OnExpressionTypeNode(node._InvaidCase.value(), GetValueMode::Read);
			auto AssignmentType = _LastExpressionType;
			if (!Type_CanBeImplicitConverted(AssignmentType, MatchAssignmentType))
			{
				const NeverNullPtr<Token> token = _LastLookedAtToken.value();
				LogError_CantCastImplicitTypes(token, MatchAssignmentType, AssignmentType, false);
			}


			_Table.RemoveScope();
		}
		else
		{
			TryError_AllValuesAreMatched(ToMatchType, V.ArmData);
		}

		V._MatchAssignmentType = MatchAssignmentType;
		_MatchExpressionDatas.AddValue(Symbol_GetSymbolID(node), std::move(V));
	}
	else if (_PassType == PassType::BuidCode)
	{
		size_t ScopeCounter = 0;
		const String ScopeName = std::to_string((uintptr_t)&node);

		OnExpressionTypeNode(node._Expression, GetValueMode::Read);

		auto Ex = _IR_LastExpressionField;
		auto ToMatchType = _LastExpressionType;

		MatchExpressionData& V = _MatchExpressionDatas.GetValue(Symbol_GetSymbolID(node));



		IRInstruction* OutEx = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(V._MatchAssignmentType));

		BuildMatch_State State;

		_LookingForTypes.push(V._MatchAssignmentType);
		for (size_t i = 0; i < node._Arms.size(); i++)
		{
			auto& Item = node._Arms[i];

			auto& VItem = V.ArmData.Arms[i];

			_Table.AddScope(ScopeName + std::to_string(ScopeCounter));

			auto V2 = IR_Build_Match(ToMatchType, node._Expression, Ex, State, VItem, Item._Expression);

			OnExpressionTypeNode(Item._AssignmentExpression, GetValueMode::Read);

			IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, V._MatchAssignmentType);

			_IR_LookingAtIRBlock->NewStore(OutEx, _IR_LastExpressionField);

			IR_Build_Match(V2, State);

			State.MatchList.push_back(std::move(V2));

			_Table.RemoveScope();

			ScopeCounter++;
		}

		if (node._InvaidCase.has_value())
		{
			_Table.AddScope(ScopeName + std::to_string(ScopeCounter));

			auto V2 = IR_Build_InvaildMatch(ToMatchType, Ex, State);

			OnExpressionTypeNode(node._InvaidCase.value(), GetValueMode::Read);

			IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, V._MatchAssignmentType);

			_IR_LookingAtIRBlock->NewStore(OutEx, _IR_LastExpressionField);

			IR_Build_Match(V2, State);

			State.MatchList.push_back(std::move(V2));

			_Table.RemoveScope();
		}
		else
		{
			auto V = IR_Build_InvaildMatch(ToMatchType, Ex, State);

			_IR_LookingAtIRBlock->NewUnreachable();

			IR_Build_Match(V, State);

			State.MatchList.push_back(std::move(V));
		}


		IR_Build_MatchState(State);

		_LookingForTypes.pop();

		_IR_LastExpressionField = OutEx;
	}
}
void SystematicAnalysis::Assembly_LoadType(const ReflectionTypeInfo& Item, TypeSymbol& Out)
{
	if (Item._Type == ReflectionTypes::CustomType)
	{
		const auto& LibsAssembby = _Lib._Assembly;

		auto Node = LibsAssembby.Find_Node(Item._CustomTypeID);
		if (Node)
		{
			switch (Node->Get_Type())
			{
			case ClassType::Class:
			{
				auto& Syb = GetSymbolsWithName(Node->FullName).front();
				Out.SetType(Syb->ID);
			}
			break;
			case ClassType::Alias:
			{
				auto& Syb = GetSymbolsWithName(Node->FullName).front();
				Out.SetType(Syb->ID);
			}
			break;
			case ClassType::Enum:
			{
				auto& Syb = GetSymbolsWithName(Node->FullName).front();
				Out.SetType(Syb->ID);
			}
			break;
			default:
				break;
			}
		}
		else
		{
			Out.SetType(TypesEnum::Null);
		}
	}
	else
	{
		Out.SetType(Item._Type);
	}
	Out._IsAddress = Item._IsAddress;
	Out._IsAddressArray = Item._IsAddressArray;
	Out._Isimmutable = Item._Isimmutable;
	Out._IsDynamic = Item._IsDynamic;
	Out._MoveData = Item._MoveData;
}
TypeSymbol SystematicAnalysis::Assembly_LoadType(const ReflectionTypeInfo& Item)
{
	TypeSymbol R;
	Assembly_LoadType(Item, R);
	return R;
}

UCodeLangFrontEnd