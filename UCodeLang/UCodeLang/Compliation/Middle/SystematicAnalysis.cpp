#include "SystematicAnalysis.hpp"
#include "UCodeLang/Compliation/Helpers/KeyWords.hpp"

#include "UCodeLang/Compliation/Helpers/InstructionBuilder.hpp"
#include "UCodeLang/Compliation/Back/UCodeBackEnd/UCodeBackEnd.hpp"
#include "UCodeLang/Compliation/Helpers/ParseHelper.hpp"
UCodeLangStart

void SystematicAnalysis::Reset()
{
	_Lib.ClearState();
	passtype = PassType::Null;
	_Builder.Reset();
}

bool SystematicAnalysis::Analyze(const FileNode& File)
{
	Vector< UClib*> Libs;
	Vector<FileNode*> files;
	files.push_back((FileNode*)&File);//shh
	return Analyze(files, Libs);
}
bool SystematicAnalysis::Analyze(const Vector<FileNode*>& Files, const Vector<UClib*>& Libs)
{


	passtype = PassType::Null;
	_Files = &Files;
	_Libs = &Libs;

	passtype = PassType::GetTypes;
	Pass();

	if (_ErrorsOutput->Has_Errors()) { goto EndFunc; };

	LoadLibSymbols();
	passtype = PassType::FixedTypes;
	Pass();


	if (!_ErrorsOutput->Has_Errors()) {
		BuildCode();
	}

EndFunc:
	_Files = nullptr;
	_Libs = nullptr;

	_Lib._LibType = OutputTypeAsLibType();

	return !_ErrorsOutput->Has_Errors();
}
void SystematicAnalysis::BuildCode()
{
	if (BackEnd == nullptr)
	{
		BackEnd = UCodeBackEnd::Get();
	}
	BackEndObject = BackEnd->GenNewBackEnd();
	BackEnd->SetBackEndAnalysis(BackEndObject, this);
	BackEnd->SetBackEndErrors(BackEndObject, _ErrorsOutput);

	passtype = PassType::BuidCode;
	Pass();

	BackEndInput input;
	input._Builder = &_Builder;
	input._Table = &_Table;
	BackEnd->Build(BackEndObject, input);

	BackEnd->DeleteBackEnd(BackEndObject);
	BackEndObject = nullptr;
}
void SystematicAnalysis::Pass()
{
	for (const auto& File : *_Files)
	{
		OnFileNode(File);
	}
	
}
void SystematicAnalysis::OnNamespace(const NamespaceNode& node)
{
	auto UseingIndex = _Table.GetUseingIndex();

	const auto Namespace = GetScopedNameAsString(node.NamespaceName);
	_Table.AddScope(Namespace);
	for (auto node : node._Nodes)
	{
		switch (node->Get_Type())
		{
		case NodeType::NamespaceNode:OnNamespace(*NamespaceNode::As(node)); break;
		case NodeType::AttributeNode:OnAttributeNode(*AttributeNode::As(node)); break;
		case NodeType::ClassNode: OnClassNode(*ClassNode::As(node)); break;
		case NodeType::AliasNode:OnAliasNode(*AliasNode::As(node)); break;
		case NodeType::EnumNode:OnEnum(*EnumNode::As(node)); break;
		case NodeType::FuncNode:OnFuncNode(*FuncNode::As(node)); break;
		case NodeType::UsingNode: OnUseingNode(*UsingNode::As(node)); break;
		default:break;
		}
	}
	_Table.RemoveScope();

	_Table.RemovePopUseing(UseingIndex);
}
void SystematicAnalysis::OnAttributeNode(const AttributeNode& node)
{
	if (passtype == PassType::GetTypes) {
		_TepAttributes.push_back(&node);
	}
}
void SystematicAnalysis::OnNonAttributeable(size_t Line, size_t Pos)
{
	if (passtype == PassType::GetTypes)
	{
		if (_TepAttributes.size())
		{
			_ErrorsOutput->AddError(ErrorCodes::TreeAnalyerError, Line, Pos, "You cant put a Tag on this");
		}
	}
}
void SystematicAnalysis::OnFileNode(UCodeLang::FileNode* const& File)
{
	for (auto node : File->_Nodes)
	{
		switch (node->Get_Type())
		{
		case NodeType::NamespaceNode:OnNamespace(*NamespaceNode::As(node)); break;
		case NodeType::AttributeNode:OnAttributeNode(*AttributeNode::As(node)); break;
		case NodeType::AliasNode:OnAliasNode(*AliasNode::As(node)); break;
		case NodeType::ClassNode: OnClassNode(*ClassNode::As(node)); break;
		case NodeType::EnumNode:OnEnum(*EnumNode::As(node)); break;
		case NodeType::FuncNode:OnFuncNode(*FuncNode::As(node)); break;
		case NodeType::UsingNode: OnUseingNode(*UsingNode::As(node)); break;
		default:break;
		}
	}
	_Table.ClearUseings();
}
void SystematicAnalysis::OnClassNode(const ClassNode& Node)
{
	bool IsgenericInstantiation = GenericFuncName.size();
	bool Isgeneric = Node.Generic.Values.size();
	bool Isgeneric_t = Isgeneric && IsgenericInstantiation == false;
	const auto& ClassName = IsgenericInstantiation ? GenericFuncName.top().GenericFuncName : Node.ClassName.Token->Value._String;
	_Table.AddScope(ClassName);

	auto SybID = IsgenericInstantiation ? (SymbolID)GenericFuncName.top().Type : (SymbolID)&Node;

	auto& Syb = passtype == PassType::GetTypes ?
		_Table.AddSybol(Isgeneric_t ? SymbolType::Generic_class : SymbolType::Type_class
			, (String)ClassName, _Table._Scope.ThisScope) :
		_Table.GetSymbol(SybID);

	if (passtype == PassType::GetTypes)
	{
		_Table.AddSymbolID(Syb, SybID);


		auto& GenericList = Node.Generic;
		for (size_t i = 0; i < GenericList.Values.size(); i++)
		{
			auto& Item = GenericList.Values[i];

			auto GenericTypeName = Item.AsString();
			auto GenericType = &_Table.AddSybol(SymbolType::Type_alias, GenericTypeName,
				_Table._Scope.GetApendedString(GenericTypeName));

			if (GenericFuncName.size())
			{
				GenericFuncInfo& V2 = GenericFuncName.top();
				GenericType->VarType = V2.Type->operator[](i);

			}
		}
	}

	if (!Isgeneric_t)
	{
		auto& Class = passtype == PassType::GetTypes ?
			_Lib.Get_Assembly().AddClass(String(ClassName), _Table._Scope.ThisScope)
			: *_Lib.Get_Assembly().Find_Class(_Table._Scope.ThisScope);
		_ClassStack.push(&Class);

		if (passtype == PassType::GetTypes)
		{
			PushTepAttributesInTo(Class._Class.Attributes);
		}

		auto UseingIndex = _Table.GetUseingIndex();

		for (const auto& node : Node._Nodes)
		{
			switch (node->Get_Type())
			{
			case NodeType::AttributeNode:OnAttributeNode(*AttributeNode::As(node)); break;
			case NodeType::ClassNode: OnClassNode(*ClassNode::As(node)); break;
			case NodeType::AliasNode:OnAliasNode(*AliasNode::As(node)); break;
			case NodeType::EnumNode:OnEnum(*EnumNode::As(node)); break;
			case NodeType::UsingNode: OnUseingNode(*UsingNode::As(node)); break;
			case NodeType::FuncNode:OnFuncNode(*FuncNode::As(node)); break;
			case NodeType::DeclareVariableNode:OnDeclareVariablenode(*DeclareVariableNode::As(node)); break;
			default:break;
			}
		}

		if (passtype == PassType::BuidCode)
		{
			GetTypesClass(Class);
		}

		_Table.RemovePopUseing(UseingIndex);


		_ClassStack.pop();

	}
	else
	{
		Syb.SomePtr = (void*)&Node;

	}


	_Table.RemoveScope();
}
void SystematicAnalysis::OnAliasNode(const AliasNode& node)
{
	const auto& ClassName = node.AliasName.Token->Value._String;
	auto SybID = (SymbolID)&node;

	_Table.AddScope(ClassName);
	auto& Syb = passtype == PassType::GetTypes ?
		_Table.AddSybol(SymbolType::Type_alias, (String)ClassName, _Table._Scope.ThisScope) :
		_Table.GetSymbol(SybID);

	if (passtype == PassType::GetTypes)
	{
		_Table.AddSymbolID(Syb, SybID);
	}
	if (passtype == PassType::FixedTypes)
	{
		Convert(node.Type, Syb.VarType);
	}

	_Table.RemoveScope();
}
void SystematicAnalysis::OnUseingNode(const UsingNode& node)
{
	auto T = node.ScopedName.ScopedName[0];
	OnNonAttributeable(T->OnLine, T->OnPos);
	const auto UseingString =GetScopedNameAsString(node.ScopedName);
	_Table.AddUseing(UseingString);

}
void SystematicAnalysis::OnFuncNode(const FuncNode& node)
{
	bool IsgenericInstantiation = GenericFuncName.size();
	bool IsGenericS = node.Signature.Generic.Values.size();

	auto FuncName = IsgenericInstantiation ? GenericFuncName.top().GenericFuncName
		: node.Signature.Name.AsStringView();
	auto NameToken = node.Signature.Name.Token;
	if (NameToken->Type == TokenType::KeyWorld_This)
	{
		FuncName = ClassConstructorfunc;
		if (_ClassStack.empty())
		{
			CantUseThisKeyWordHereError(NameToken);
		}
	}
	else if (NameToken->Type == TokenType::KeyWorld_Drop)
	{
		FuncName = ClassDestructorFunc;
	}



	_Table.AddScope(FuncName);
	auto FullName = _Table._Scope.ThisScope;

	ClassData* Ptr = nullptr;
	if (_ClassStack.empty())
	{
		auto& Assembly = _Lib.Get_Assembly();

		auto globalAssemblyObjectName = (String_view)ScopeHelper::_globalAssemblyObject;

		Ptr = Assembly.Find_Class(globalAssemblyObjectName);
		if (Ptr == nullptr)
		{
			Ptr = &_Lib.Get_Assembly().AddClass(String(globalAssemblyObjectName));
		}
	}
	else
	{
		Ptr = _ClassStack.top();
	}



	Symbol* syb;
	SymbolID sybId = IsgenericInstantiation ? (SymbolID)GenericFuncName.top().Type : (SymbolID)&node;


	auto UseingIndex = _Table.GetUseingIndex();

	if (passtype == PassType::GetTypes)
	{
		SymbolType Type = IsGenericS && IsgenericInstantiation == false ?
			SymbolType::GenericFunc : SymbolType::Func;



		syb = &_Table.AddSybol(Type, (String)FuncName, FullName);
		syb->SomePtr = (void*)&node;//the node will not get update anyway.
		_Table.AddSymbolID(*syb, sybId);


		auto& GenericList = node.Signature.Generic;
		for (size_t i = 0; i < GenericList.Values.size(); i++)
		{
			auto& Item = GenericList.Values[i];

			auto GenericTypeName = Item.AsString();
			auto GenericType = &_Table.AddSybol(SymbolType::Type_alias, GenericTypeName, 
				_Table._Scope.GetApendedString(GenericTypeName)
				);

			if (GenericFuncName.size())
			{
				GenericFuncInfo& V2 = GenericFuncName.top();
				GenericType->VarType = V2.Type->operator[](i);

			}
		}

		

		auto RetType = node.Signature.ReturnType.node;
		if (RetType && RetType->Get_Type() == NodeType::AnonymousTypeNode)
		{
			auto NewName = GetFuncAnonymousObjectFullName(FullName);
			auto& Class = _Lib.Get_Assembly().AddClass(NewName, NewName);


			AnonymousTypeNode* Typenode = AnonymousTypeNode::As(RetType);
			for (auto& Item3 : Typenode->Fields.Parameters)
			{
				ClassField V;
				V.FullNameType = Item3.Type.AsString();
				V.Name = Item3.Name.AsString();
				Class._Class.Fields.push_back(V);
			}
			GetTypesClass(Class);
		}

		

		for (auto& Item : node.Signature.Parameters.Parameters)
		{
			auto GenericTypeName = Item.Name.AsStringView();
			auto GenericType = &_Table.AddSybol(SymbolType::ParameterVarable, (String)GenericTypeName,
				_Table._Scope.GetApendedString(GenericTypeName)
			);
			auto ParSybID = (SymbolID)&Item;
			_Table.AddSymbolID(*GenericType, ParSybID);
		}
		
		
		syb = &_Table.GetSymbol(sybId);//resized _Table
	}
	else
	{
		syb = &_Table.GetSymbol(sybId);
	}

	if (passtype == PassType::FixedTypes)
	{
		Convert(node.Signature.ReturnType, syb->VarType);

		for (auto& Item : node.Signature.Parameters.Parameters)
		{
			auto GenericTypeName = Item.Name.AsStringView();
			auto ParSybID = (SymbolID)&Item;
			auto& Sybol = _Table.GetSymbol(ParSybID);

			Convert(Item.Type, Sybol.VarType);
		}
	}

	bool buidCode = passtype == PassType::BuidCode;
	bool ignoreBody = false;
	if (!IsgenericInstantiation && IsGenericS) {
		ignoreBody = true;
	}

	if (buidCode && !ignoreBody)
	{
		_Builder.Build_Func(sybId);
		for (auto& Item : node.Signature.Parameters.Parameters)
		{
			auto ParSybID = (SymbolID)&Item;
			_Builder.Build_Parameter(ParSybID);
		}

		bool DLLCall = false;
		for (auto& Item : _TepAttributes)
		{
			String AttType;
			Item->ScopedName.GetScopedName(AttType);
			if (AttType == "DLL")
			{
				DLLCall = true;
				break;
			}
		}
		if (!node.Body.has_value()&& DLLCall)
		{
			_Builder.Build_DLLJump(node.Signature.Name.AsStringView());
		}
	}

	

	if (node.Body.has_value() && !ignoreBody)
	{
		auto& Body = node.Body.value();
		_InStatements = true;
		bool HasARet = false;
		LookingForTypes.push(syb->VarType);

		for (const auto& node2 : Body.Statements._Nodes)
		{
			switch (node2->Get_Type())
			{
			case NodeType::AttributeNode:OnAttributeNode(*AttributeNode::As(node2)); break;
			case NodeType::ClassNode: OnClassNode(*ClassNode::As(node2)); break;
			case NodeType::EnumNode:OnEnum(*EnumNode::As(node2)); break;
			case NodeType::UsingNode: OnUseingNode(*UsingNode::As(node2)); break;
			case NodeType::DeclareVariableNode:OnDeclareVariablenode(*DeclareVariableNode::As(node2)); break;
			case NodeType::AssignVariableNode:OnAssignVariableNode(*AssignVariableNode::As(node2)); break;
			case NodeType::PostfixVariableNode:OnPostfixVariableNode(*PostfixVariableNode::As(node2)); break;
			case NodeType::CompoundStatementNode:OnCompoundStatementNode(*CompoundStatementNode::As(node2)); break;
			case NodeType::FuncCallStatementNode:OnFuncCallNode(FuncCallStatementNode::As(node2)->Base); break;
			case NodeType::DropStatementNode:OnDropStatementNode(*DropStatementNode::As(node2)); break;
			case NodeType::RetStatementNode:
				syb = &_Table.GetSymbol(sybId);

				OnRetStatement(*RetStatementNode::As(node2));
				HasARet = true;

				if (passtype == PassType::FixedTypes && syb->VarType._Type == TypesEnum::Var)
				{
					Get_LookingForType() = syb->VarType = LastExpressionType;

					if (LastExpressionType._Type == TypesEnum::Var)
					{
						auto Token = LastLookedAtToken;
						CantguessVarTypeError(Token);
					}
				}

				break;
			default:break;
			}
		}



		_InStatements = false;

		syb = &_Table.GetSymbol(sybId);
		if (passtype == PassType::FixedTypes)
		{
			if (!HasARet)
			{
				if (syb->VarType._Type == TypesEnum::Var)
				{
					syb->VarType.SetType(TypesEnum::Void);
				}
				else if (syb->VarType._Type != TypesEnum::Void)//Update This when control flow get added.
				{
					auto Token = node.Signature.Name.Token;
					YouMustReturnSomethingError(Token);
				}

			}
		}

		LookingForTypes.pop();
	}

	if (buidCode && !ignoreBody)
	{
		_Builder.Build_Ret();
	}

	_Table.RemovePopUseing(UseingIndex);



	if (buidCode)
	{
		ClassMethod V;
		V.FullName = _Table._Scope.ThisScope;
		V.RetType.FullNameType = ToString(syb->VarType);

		PushTepAttributesInTo(V.Attributes);


		Ptr->_Class.Methods.push_back(V);
	}

	_Table.RemoveScope();
}
void SystematicAnalysis::OnRetStatement(const RetStatementNode& node)
{
	if (node.Expression.Value)
	{
		auto LookForT = Get_LookingForType();
		LookForT.SetAsRawValue();

		LookingForTypes.push(LookForT);
		OnExpressionTypeNode(node.Expression.Value);

		LookingForTypes.pop();
	}
	else
	{
		LastExpressionType.SetType(TypesEnum::Void);
	}


	if (passtype == PassType::FixedTypes) 
	{
		auto& T = Get_LookingForType();
		if (T._Type != TypesEnum::Var) 
		{
			if (!CanBeImplicitConverted(LastExpressionType, T))
			{
				if (T._Type != TypesEnum::Null) 
				{
					LogCantCastImplicitTypes(LastLookedAtToken, LastExpressionType, T);
				}
			}
		}
	}
	if (passtype == PassType::BuidCode)
	{
		if (node.Expression.Value) {
			_Builder.Build_AssignRet(IROperand::AsLocation(_Builder.GetLastField()));
		}
	}
}
void SystematicAnalysis::OnEnum(const EnumNode& node)
{
}

String SystematicAnalysis::GetScopedNameAsString(const ScopedNameNode& node)
{
	String Text;
	for (const auto& Item : node.ScopedName)
	{
		Text += Item->Value._String;
	}
	return Text;
}
void SystematicAnalysis::OnDeclareVariablenode(const DeclareVariableNode& node)
{
	auto& StrVarName = node.Name.AsString();
	auto FullName = _Table._Scope.GetApendedString(StrVarName);

	SymbolID sybId = (SymbolID)&node;
	Symbol* syb;


	if (passtype == PassType::GetTypes)
	{
		syb = &_Table.AddSybol(SymbolType::StackVarable, StrVarName, FullName);
		_Table.AddSymbolID(*syb, sybId);

		syb->Size = NullAddress;
		if (_ClassStack.size() && _InStatements == false)
		{
			ClassField V;
			auto& Class = *_ClassStack.top();
			V.FullNameType = node.Type.AsString();
			V.Name = node.Name.AsString();

			Class._Class.Fields.push_back(V);
		}
	}
	else
	{
		syb = &_Table.GetSymbol(sybId);
	}

	if (passtype == PassType::FixedTypes)
	{
		auto& VarType = syb->VarType;
		Convert(node.Type, VarType);
		VarType.SetAsLocation();
		
		if (VarType._Type == TypesEnum::Var && node.Expression.Value == nullptr)
		{
			auto Token = node.Type.Name.Token;
			CantgussTypesTheresnoassignment(Token);
		}
		else
		{
			GetSize(VarType, syb->Size);
		}
	}
	LookingForTypes.push(syb->VarType);

	if (node.Expression.Value)
	{
		OnExpressionTypeNode(node.Expression.Value);
	}

	if (passtype == PassType::FixedTypes)
	{
		syb = &_Table.GetSymbol(sybId);
		if (node.Expression.Value)
		{
			syb->SetTovalid();

			auto& VarType = syb->VarType;


			auto& Ex = LastExpressionType;
			auto Token = node.Type.Name.Token;
			if (VarType._Type == TypesEnum::Var)
			{
				if (Ex._Type == TypesEnum::Var)
				{
					CantguessVarTypeError(Token);
				}
				else
				{
					bool WasImutable = VarType.Isimmutable();
					bool WasIsAddress = VarType.IsAddress();
					

					VarType = Ex;
					if (WasImutable) { VarType.SetAsimmutable(); }
					if (WasIsAddress) { VarType.SetAsAddress(); }
					VarType.SetAsLocation();
					GetSize(VarType, syb->Size);
				}
			}
			if (!CanBeImplicitConverted(Ex, VarType))
			{
				LogCantCastImplicitTypes(Token, Ex, VarType);
			}
		}
		else
		{
			syb->SetToInvalid();
		}
	}
	LookingForTypes.pop();

	if (passtype == PassType::BuidCode && node.Expression.Value)
	{
		auto Op = IROperand::AsLocation(_LastExpressionField);
		auto NewOp = IROperand::AsVarable(sybId);
		_Builder.Build_Assign(NewOp, Op);	
	}
}
void SystematicAnalysis::OnAssignVariableNode(const AssignVariableNode& node)
{
	if (passtype != PassType::GetTypes)
	{
		auto Symbol = GetSymbol(node.Name.AsStringView(), SymbolType::Varable_t);
		LookingForTypes.push(Symbol->VarType);
	}
	OnExpressionTypeNode(node.Expression.Value);
	if (passtype != PassType::GetTypes)
	{
		LookingForTypes.pop();
	}
	

	if (passtype == PassType::FixedTypes)
	{
		auto Name = node.Name.AsStringView();
		auto Symbol = GetSymbol(Name, SymbolType::Varable_t);
		
		CheckVarWritingErrors(Symbol, node.Name.Token, Name);

		if (!CanBeImplicitConverted(LastExpressionType, Symbol->VarType))
		{
			auto  Token = LastLookedAtToken;
			LogCantCastImplicitTypes(Token, LastExpressionType, Symbol->VarType);

		}
		
	}

	


	if (passtype == PassType::BuidCode)
	{
		auto Symbol = GetSymbol(node.Name.AsStringView(), SymbolType::Varable_t);
		SymbolID sybId = Symbol->ID;

		auto Op = IROperand::AsLocation(_LastExpressionField);
		auto NewOp = IROperand::AsVarable(sybId);
		_Builder.Build_Assign(NewOp, Op);
	}
}
void SystematicAnalysis::OnPostfixVariableNode(const PostfixVariableNode& node)
{
	if (passtype == PassType::FixedTypes)
	{
		auto Name = node.Name.AsStringView();
		auto Symbol = GetSymbol(Name, SymbolType::Varable_t);

		CheckVarWritingErrors(Symbol, node.Name.Token, Name);
		LogTryReadVar(Name, node.Name.Token, Symbol);
	}
	if (passtype == PassType::BuidCode)
	{
		auto Symbol = GetSymbol(node.Name.AsStringView(), SymbolType::Varable_t);
		SymbolID sybId = Symbol->ID;


		_Builder.Build_Assign(IROperand::AsInt8((UInt8)1));
		auto V0 = IROperand::AsLocation(_Builder.GetLastField());
		_Builder.Build_Assign(IROperand::AsReadVarable(sybId));
		auto V1 = IROperand::AsLocation(_Builder.GetLastField());

		if (node.PostfixOp->Type == TokenType::increment) 
		{
			_Builder.MakeAdd8(V0, V1);
		}
		else
		{
			_Builder.MakeSub8(V0, V1);
		}
		auto Op = IROperand::AsLocation(_LastExpressionField);
		auto NewOp = IROperand::AsVarable(sybId);
		_Builder.Build_Assign(NewOp, Op);
	}
}
void SystematicAnalysis::OnCompoundStatementNode(const CompoundStatementNode& node)
{
	OnExpressionTypeNode(node.Expession.Value);
	
	if (passtype == PassType::FixedTypes)
	{
		auto Name = node.VariableName.AsStringView();
		auto Symbol = GetSymbol(Name, SymbolType::Varable_t);
		CheckVarWritingErrors(Symbol, node.VariableName.Token, Name);
		LogTryReadVar(Name, node.VariableName.Token, Symbol);
	}
	if (passtype == PassType::BuidCode)
	{
		auto Symbol = GetSymbol(node.VariableName.AsStringView(), SymbolType::Varable_t);
		SymbolID sybId = Symbol->ID;

		auto V0 = IROperand::AsLocation(_Builder.GetLastField());
		_Builder.Build_Assign(IROperand::AsReadVarable(sybId));
		auto V1 = IROperand::AsLocation(_Builder.GetLastField());
		switch (node.CompoundOp->Type)
		{
		case TokenType::CompoundAdd:
			_Builder.MakeAdd8(V0, V1);
			break;
		case TokenType::CompoundSub:
			_Builder.MakeSub8(V0, V1);
			break;
		default:
			throw std::exception("Bad Op");
			break;
		}
		auto Op = IROperand::AsLocation(_LastExpressionField);
		auto NewOp = IROperand::AsVarable(sybId);
		_Builder.Build_Assign(NewOp, Op);
	}
}
void SystematicAnalysis::OnExpressionTypeNode(const Node* node)
{
	switch (node->Get_Type())
	{
	case NodeType::BinaryExpressionNode:OnExpressionNode(*BinaryExpressionNode::As(node));break;
	case NodeType::ValueExpressionNode:OnExpressionNode(*ValueExpressionNode::As(node)); break;
	case NodeType::CastNode:OnExpressionNode(*CastNode::As(node)); break;
	default:
		throw std::exception("not added");
		break;
	}
	

}
void SystematicAnalysis::OnExpressionNode(const ValueExpressionNode& node)
{
	
	if (passtype == PassType::BuidCode
		|| passtype == PassType::FixedTypes)
	{
		
		switch (node.Value->Get_Type())
		{
		case NodeType::NumberliteralNode:
		{
			NumberliteralNode* num = NumberliteralNode::As(node.Value);
#define Set_NumberliteralNodeU(x) \
			UInt##x V; \
			ParseHelper::ParseStringToUInt##x(Str, V); \
			_Builder.Build_Assign(IROperand::AsInt##x(V));\

#define Set_NumberliteralNodeS(x) \
			Int##x V; \
			ParseHelper::ParseStringToInt##x(Str, V); \
			_Builder.Build_Assign(IROperand::AsInt##x(V));\


			auto& lookT = Get_LookingForType();
			if (passtype == PassType::BuidCode)
			{
				auto& Str = num->Token->Value._String;
				

				
				switch (lookT._Type)
				{
				case TypesEnum::uInt8:
				{
					Set_NumberliteralNodeU(8);
				};
				break;
				case TypesEnum::uInt16:
				{
					Set_NumberliteralNodeU(16);
				};
				break;
				case TypesEnum::uInt32:
				{
					Set_NumberliteralNodeU(32);
				};
				break;
				case TypesEnum::uInt64:
				{
					Set_NumberliteralNodeU(64);
				};
				break;
				case TypesEnum::uIntPtr:
				{
					UInt64 V;
					ParseHelper::ParseStringToUInt64(Str, V);
					Build_Assign_uIntPtr(V);
				};
				break;

				case TypesEnum::sInt8:
				{
					Set_NumberliteralNodeS(8);
				};
				break;
				case TypesEnum::sInt16:
				{
					Set_NumberliteralNodeS(16);
				};
				break;
				case TypesEnum::sInt32:
				{
					Set_NumberliteralNodeS(32);
				};
				break;
				case TypesEnum::sInt64:
				{
					Set_NumberliteralNodeS(64);
				};
				break;
				case TypesEnum::sIntPtr:
				{
					Int64 V;
					ParseHelper::ParseStringToInt64(Str, V);
					Build_Assign_sIntPtr(V);
				};
				
				break;
				default:
					throw std::exception("not added");
					break;
				} 


				_LastExpressionField = _Builder.GetLastField();

			}

			TypesEnum NewEx = lookT._Type == TypesEnum::Var ? TypesEnum::sInt32 : lookT._Type;
			

			LastExpressionType.SetType(NewEx);
			LastLookedAtToken = num->Token;
		}
		break;
		case NodeType::BoolliteralNode:
		{
			BoolliteralNode* num = BoolliteralNode::As(node.Value);
			if (passtype == PassType::BuidCode)
			{
				

				_Builder.Build_Assign(IROperand::AsInt8((UInt8)num->Value));
				_LastExpressionField = _Builder.GetLastField();
			}
			LastExpressionType.SetType(TypesEnum::Bool);
		}
		break;
		case NodeType::ReadVariableNode:
		{
			ReadVariableNode* nod = ReadVariableNode::As(node.Value);
			auto Str = GetScopedNameAsString(nod->VariableName);

			auto Symbol = GetSymbol(Str, SymbolType::Varable_t);
			auto Token = nod->VariableName.ScopedName[0];

			auto Info = LogTryReadVar(Str, Token, Symbol);
			if (Info.CantFindVar)
			{
				LogCantFindVarError(Token, Str);
				return;
			}
			if (Info.VarIsInvalid)
			{
				LogReadingFromInvaidVariable(Token, Str);
				goto SetExpressionInfo;

			}

			SymbolID sybId = Symbol->ID;
			if (passtype == PassType::BuidCode)
			{
				auto& LookForT = Get_LookingForType();
				if (LookForT.IsLocationValue())
				{
					_Builder.Build_Assign(IROperand::AsPointer(sybId));
				}
				else
				{
					_Builder.Build_Assign(IROperand::AsReadVarable(sybId));
				}
				_LastExpressionField = _Builder.GetLastField();
			}

			SetExpressionInfo:
			LastExpressionType = Symbol->VarType;
			LastLookedAtToken = Token;
		}
		break;
		case NodeType::AnonymousObjectConstructorNode:
		{
			auto& Type = Get_LookingForType();
			if (Type._Type != TypesEnum::Var)//function who called this can deal with var
			{

			}

			LastExpressionType = Type;
		}
		break;
		case NodeType::FuncCallNode:
		{
			OnFuncCallNode(*FuncCallNode::As(node.Value));//LastExpressionType was set by OnFuncCall
		}
		break;
		case NodeType::SizeofExpresionNode:
		{
			SizeofExpresionNode* nod = SizeofExpresionNode::As(node.Value);

			auto& lookT = Get_LookingForType();
			TypeSymbol Type;

			if (passtype == PassType::FixedTypes || passtype == PassType::BuidCode)
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

			if (passtype == PassType::BuidCode)
			{
				TypeSymbol Info;
				Convert(nod->Type, Info);
				UAddress TypeSize;
				GetSize(Info, TypeSize);
				switch (lookT._Type)
				{
				case TypesEnum::sInt8:
				case TypesEnum::uInt8:
					_Builder.Build_Assign(IROperand::AsInt8((UInt8)TypeSize));
					break;
				case TypesEnum::sInt16:
				case TypesEnum::uInt16:
					_Builder.Build_Assign(IROperand::AsInt16((UInt16)TypeSize));
					break;
				case TypesEnum::sInt32:
				case TypesEnum::uInt32:
					_Builder.Build_Assign(IROperand::AsInt32((UInt32)TypeSize));
					break;
				case TypesEnum::sInt64:
				case TypesEnum::uInt64:
					_Builder.Build_Assign(IROperand::AsInt64((UInt64)TypeSize));
					break;
				default:
					Type.SetType(TypesEnum::uIntPtr);
					Build_Assign_uIntPtr(TypeSize);
					break;
				}
			}

			LastExpressionType = Type;
		}
		break;
		case NodeType::NewExpresionNode:
		{
			NewExpresionNode* nod = NewExpresionNode::As(node.Value);
			TypeSymbol Type;
			Convert(nod->Type, Type);
			bool IsArray = nod->Arrayexpression.Value;

			

			if (passtype == PassType::BuidCode)
			{
				UAddress TypeSize;
				GetSize(Type, TypeSize);

				if (IsArray)
				{
					TypeSymbol UintptrType = TypeSymbol();
					UintptrType.SetType(TypesEnum::uIntPtr);
					UAddress UintptrSize;
					GetSize(UintptrType, UintptrSize);
					bool TypeHaveDestructor = false;

					LookingForTypes.push(UintptrType);

					
					OnExpressionTypeNode(nod->Arrayexpression.Value);
					auto Ex0 = _LastExpressionField;

					Build_Assign_uIntPtr(TypeSize);//UintptrSize is for the array length for Drop 
					auto Ex1 = _Builder.GetLastField();
					
					Build_Mult_uIntPtr(IROperand::AsLocation(Ex0), IROperand::AsLocation(Ex1));//uintptr
					auto DataSize = _Builder.GetLastField();

					if (TypeHaveDestructor) 
					{
						Build_Increment_uIntPtr(UintptrSize);//Make room for ItemSize onPtr
					}

					_Builder.Build_Malloc(IROperand::AsLocation(_Builder.GetLastField()));
					auto MallocData = _Builder.GetLastField();

					
					if (TypeHaveDestructor)
					{   
						//Call default on every
						Build_Increment_uIntPtr(UintptrSize);
					}
					


					LookingForTypes.pop();
				}
				else
				{
					_Builder.Build_Malloc(TypeSize);
					//Call ObjectNew
				}
			}

			Type.SetAsAddress();
			
			if (IsArray)
			{
				if (passtype != PassType::BuidCode) 
				{
					TypeSymbol UintptrType = TypeSymbol();
					UintptrType.SetType(TypesEnum::uIntPtr);
					LookingForTypes.push(UintptrType);

					OnExpressionTypeNode(nod->Arrayexpression.Value);
					auto Ex1Type = LastExpressionType;
					

					if (!CanBeImplicitConverted(Ex1Type, UintptrType))
					{
						auto  Token =LastLookedAtToken;

						LogCantCastImplicitTypes(Token, Ex1Type, UintptrType);

					}

					LookingForTypes.pop();
				}
				Type.SetAsAddressArray();

			}
			
			LastExpressionType = Type;
		}
		break;
		default:
			throw std::exception("not added");
			break;
		}
	}
}

void SystematicAnalysis::OnExpressionNode(const BinaryExpressionNode& node)
{
	OnExpressionTypeNode(node.Value1.Value);
	auto Ex0 = _LastExpressionField;
	auto Ex0Type = LastExpressionType;
	OnExpressionTypeNode(node.Value0.Value);
	auto Ex1 = _LastExpressionField;
	auto Ex1Type = LastExpressionType;

	if (passtype == PassType::FixedTypes)
	{
		auto BinaryOp = node.BinaryOp;
		bool V = HasBinaryOverLoadWith(Ex0Type, BinaryOp->Type, Ex1Type);
		if (!V)
		{
			LogCantFindBinaryOpForTypes(BinaryOp, Ex0Type, Ex1Type);
		}
	}

	if (passtype == PassType::BuidCode)
	{
		auto Op0 = IROperand::AsLocation(Ex0);
		auto Op1 = IROperand::AsLocation(Ex1);

		bool IsSame = AreTheSame(Ex0Type, Ex1Type);//For Testing


		switch (node.BinaryOp->Type)
		{
		case TokenType::plus:_Builder.MakeAdd8(Op0, Op1);
			break;
		case TokenType::minus:_Builder.MakeSub8(Op0, Op1);
			break;
		default:
			throw std::exception("not added");
			break;
		}


		_LastExpressionField = _Builder.GetLastField();
	}
}
void SystematicAnalysis::OnExpressionNode(const CastNode& node)
{
	OnExpressionTypeNode(node.Expression.Value);
	auto Ex0 = _LastExpressionField;
	auto Ex0Type = LastExpressionType;


	if (passtype == PassType::FixedTypes) 
	{
		TypeSymbol ToTypeAs;
		Convert(node.ToType, ToTypeAs);
		if (!CanBeExplicitlyConverted(Ex0Type, ToTypeAs))
		{
			auto  Token = node.ToType.Name.Token;

			LogCantCastExplicityTypes(Token, Ex0Type, ToTypeAs);
		}
		LastExpressionType = ToTypeAs;
	}
}
void SystematicAnalysis::OnFuncCallNode(const FuncCallNode& node)
{
	if (passtype == PassType::FixedTypes)
	{
		auto FuncName = node.FuncName.AsStringView();
		auto Syb = GetSymbol(FuncName, SymbolType::GenericFunc);
		SymbolID FuncSyb;
		if (Syb == nullptr)
		{
			auto  Token = node.FuncName.Token;
			LogCantFindFuncError(Token, FuncName);
			return;
		}
		if (node.Generics.Values.size())
		{

			const FuncNode& fnode = *(const FuncNode*)Syb->SomePtr;
			const auto& Tnode = node.Generics.Values[0];

			TypeSymbol Type; Convert(Tnode, Type);

			auto TypeToSwap = GetSymbol((String)FuncName + ScopeHelper::_ScopeSep + "T", SymbolType::Type)->VarType;

			Vector<TypeSymbol>* TypesToChange =new Vector<TypeSymbol>();//Symbol ID is from Ptr
			Vector<TypeSymbol>* TypeToHave = new Vector<TypeSymbol>();
			TypesToChange->push_back(TypeToSwap);
			TypeToHave->push_back(Type);

			String NewName = GetGenericFuncName(Syb, *TypeToHave);
			auto GenerisESyb = GetSymbol(NewName, SymbolType::Func);
			if (GenerisESyb == nullptr)
			{
				GenericFuncInstantiate(Syb, fnode, *TypesToChange, *TypeToHave);
				GenerisESyb = GetSymbol(NewName, SymbolType::Func);
			}
			FuncSyb = GenerisESyb->ID;
			LastExpressionType = GenerisESyb->VarType;
			delete  TypesToChange;
			delete TypeToHave;
		}
		else 
		{
			LastExpressionType = Syb->VarType;
			FuncSyb = Syb->ID;
		}

		for (auto& Item : node.Parameters._Nodes)
		{
			OnExpressionTypeNode(Item);
		}

		FuncToSyboID[&node] = FuncSyb;
	}
	else if (passtype == PassType::BuidCode)
	{
		auto SybID = FuncToSyboID.at(&node);
		
		for (auto& Item : node.Parameters._Nodes)
		{
			TypeSymbol V;
			V.SetType(TypesEnum::uInt32);
			LookingForTypes.push(V);
			
			OnExpressionTypeNode(Item);
			
			_Builder.Build_PassLastAsParameter();
		
			LookingForTypes.pop();
		}

		_Builder.Build_FuncCall(SybID);
	}
	else
	{
		for (auto& Item : node.Parameters._Nodes)
		{
			OnExpressionTypeNode(Item);
		}
	}
}
void SystematicAnalysis::OnDropStatementNode(const DropStatementNode& node)
{
	OnExpressionTypeNode(node.expression.Value);
	auto Ex0 = _LastExpressionField;
	auto Ex0Type = LastExpressionType;
	if (passtype == PassType::FixedTypes)
	{
		if (!Ex0Type.IsAddress())
		{
			auto Token = LastLookedAtToken;
			ExpressionMustbeAnLocationValueError(Token, Ex0Type);
		}
		else if (!Ex0Type.IsLocationValue())
		{
			auto Token = LastLookedAtToken;
			ExpressionMustbeAnLocationValueError(Token, Ex0Type);
		}
			
	}
	if (passtype == PassType::BuidCode)
	{
		if (Ex0Type.IsAddressArray())
		{
			Ex0Type._IsAddressArray = false;

			bool TypeHaveDestructor = false;
		
			

			if (TypeHaveDestructor)
			{
				TypeSymbol UintptrType = TypeSymbol();
				UintptrType.SetType(TypesEnum::uIntPtr);
				UAddress UintptrSize;
				GetSize(UintptrType, UintptrSize);

				Build_Decrement_uIntPtr(UintptrSize);
				//Def here to get size

				_Builder.Build_Free(IROperand::AsLocation(_Builder.GetLastField()));
			}
			else
			{
				_Builder.Build_Free(IROperand::AsLocation(Ex0));
			}

			Ex0Type._IsAddressArray = true;
		}
		else 
		{
			//Call Object Drop
			_Builder.Build_Free(IROperand::AsLocation(Ex0));
		}
	}
}
void SystematicAnalysis::PushTepAttributesInTo(Vector<AttributeData>& Input)
{
	for (auto& Item : _TepAttributes)
	{
		AttributeData Data;
		Item->ScopedName.GetScopedName(Data.Name);
		Input.push_back(Data);
	}
	_TepAttributes.clear();
}
void SystematicAnalysis::LoadLibSymbols()
{
	for (auto Item : *_Libs)
	{
		LoadLibSymbols(*Item);
	}
}
void SystematicAnalysis::LoadLibSymbols(const UClib& lib)
{
	auto OutputType = OutputTypeAsLibType();
	auto libType = lib._LibType;
	
	LibType ImportType;
	if (OutputType == LibType::Lib)
	{
		ImportType = libType;
	}
	else
	{
		ImportType = LibType::Dll;
	}

	for (auto Item : lib.Get_NameToPtr())
	{
		const auto Offset = Item.second;
		const auto& FuncStr = Item.first;
		

		auto SymbolType = ImportType == LibType::Dll ? SymbolType::ImportedDllFunc : SymbolType::ImportedLibFunc;
		
		auto& Syb = _Table.AddSybol(SymbolType,FuncStr, ScopeHelper::GetNameFromFullName(FuncStr));
		
		
	}

	for (auto Item : lib.Get_Assembly().Classes)
	{
		switch (Item->Type)
		{
		case ClassType::Class:
		{

		}
			break;
		case ClassType::Alias:
			break;
		case ClassType::Enum:
			break;
		default:
			break;
		}
	}
}
Symbol* SystematicAnalysis::GetSymbol(String_view Name, SymbolType Type)
{
	auto& Symbols = _Table.GetSymbolsWithName(Name,Type);
	auto Symbol = Symbols.size() ? Symbols[0] : nullptr;
	return Symbol;
}
String SystematicAnalysis::GetFuncAnonymousObjectFullName(const String& FullFuncName)
{
	return FullFuncName + "!";
}
void SystematicAnalysis::GetTypesClass(ClassData& Class)
{
	for (auto& node : Class._Class.Fields)
	{
		UAddress Size;
		TypeSymbol Syb;
		
		//GetSize(GetSymbol(node.FullNameType, SymbolType::Type)->VarType, Size);
	}
}

bool SystematicAnalysis::AreTheSame(const TypeSymbol& TypeA, const TypeSymbol& TypeB)
{
	if (TypeA._Type == TypeB._Type &&
		TypeA.IsAddress() == TypeB.IsAddress() && 
		TypeA.IsAddressArray() == TypeB.IsAddressArray() &&
		TypeA.Isimmutable() == TypeB.Isimmutable()
	   )
	{
		return true;
	}

	if (TypeA._Type == TypesEnum::CustomType
		&& TypeB._Type == TypesEnum::CustomType)
	{
		if (TypeA._CustomTypeSymbol == TypeB._CustomTypeSymbol)
		{
			return true;
		}
	}

	return false;
}
bool SystematicAnalysis::AreTheSameWithOutimmutable(const TypeSymbol& TypeA, const TypeSymbol& TypeB)
{
	if (TypeA._Type == TypeB._Type 
		//&&
		//TypeA.IsAddress() == TypeB.IsAddress() &&
		//TypeA.IsAddressArray() == TypeB.IsAddressArray() 
		)
	{
		return true;
	}

	if (TypeA._Type == TypesEnum::CustomType
		&& TypeB._Type == TypesEnum::CustomType)
	{
		if (TypeA._CustomTypeSymbol == TypeB._CustomTypeSymbol)
		{
			return true;
		}
	}

	return false;
}
bool SystematicAnalysis::HasBinaryOverLoadWith(const TypeSymbol& TypeA, TokenType BinaryOp, const TypeSymbol& TypeB)
{
	return false;
}
String SystematicAnalysis::ToString(const TypeSymbol& Type)
{
	String r;
	if (Type.Isimmutable())
	{
		r = "umut ";
	}

	switch (Type._Type)
	{
	case TypesEnum::Var:r = "var";	break;
	case TypesEnum::Int_t:r = "Int_t";	break;
	case TypesEnum::uInt_t:r = "uInt_t";	break;
	case TypesEnum::sInt_t:r = "sInt_t";	break;


	case TypesEnum::uInt8:r = "uInt8";	break;
	case TypesEnum::uInt16:r = "uInt16";	break;
	case TypesEnum::uInt32:r = "uInt32";	break;
	case TypesEnum::uInt64:r = "uInt64";	break;


	case TypesEnum::sInt8:r = "sInt8";	break;
	case TypesEnum::sInt16:r = "sInt16";	break;
	case TypesEnum::sInt32:r = "sInt32";	break;
	case TypesEnum::sInt64:r = "sInt64";	break;

	case TypesEnum::uIntPtr:r = "uIntptr";	break;
	case TypesEnum::sIntPtr:r = "sIntptr";	break;

	case TypesEnum::Bool:r = "bool";	break;
	case TypesEnum::Char:r = "char";	break;
	case TypesEnum::CustomType:
	{
		auto Syb = _Table.GetSymbol(Type._CustomTypeSymbol);
		if (Syb.Type == SymbolType::Func
			|| Syb.Type == SymbolType::GenericFunc)
		{
			r = ToString(Syb.VarType);
		}
		else
		{
			r = Syb.FullName;
		}
	}	break;
	case TypesEnum::Void:
		r = "void";	break;
	case TypesEnum::Null:
		r = "[badtype]";	break;
	default:
		break;
	}

	if (Type.IsAddress())
	{
		r += "&";
	}
	if (Type.IsAddressArray())
	{
		r += "[&]";
	}

	return r;
}
void SystematicAnalysis::Convert(const TypeNode& V, TypeSymbol& Out)
{
	switch (V.Name.Token->Type)
	{
	case TokenType::KeyWorld_var:
		Out.SetType(TypesEnum::Var);
		break;
	case TokenType::KeyWorld_UInt8:
		Out.SetType(TypesEnum::uInt8);
		break;
	case TokenType::KeyWorld_UInt16:
		Out.SetType(TypesEnum::uInt16);
		break;
	case TokenType::KeyWorld_UInt32:
		Out.SetType(TypesEnum::uInt32);
		break;
	case TokenType::KeyWorld_UInt64:
		Out.SetType(TypesEnum::uInt64);
		break;

	case TokenType::KeyWorld_SInt8:
		Out.SetType(TypesEnum::sInt8);
		break;
	case TokenType::KeyWorld_SInt16:
		Out.SetType(TypesEnum::sInt16);
		break;
	case TokenType::KeyWorld_SInt32:
		Out.SetType(TypesEnum::sInt32);
		break;
	case TokenType::KeyWorld_SInt64:
		Out.SetType(TypesEnum::sInt64);
		break;

	case TokenType::KeyWorld_uintptr:
		Out.SetType(TypesEnum::uInt64);
		break;
	case TokenType::KeyWorld_sintptr:
		Out.SetType(TypesEnum::sInt64);
		break;

	case TokenType::KeyWorld_Bool:
		Out.SetType(TypesEnum::Bool);
		break;
	case TokenType::KeyWorld_Char:
		Out.SetType(TypesEnum::Char);
		break;
	case TokenType::Name: 
	{
		Out._Type = TypesEnum::CustomType;

		auto Name = V.Name.AsStringView();
		Symbol* SybV;
		if (V.Generic.Values.size())
		{
			SybV = GetSymbol(Name, SymbolType::Generic_class);

			const auto& Tnode = V.Generic.Values[0];
			TypeSymbol Type; Convert(Tnode, Type);

			auto TypeToSwap = GetSymbol((String)Name + ScopeHelper::_ScopeSep + "T", SymbolType::Type)->VarType;

			Vector<TypeSymbol>* TypesToChange =new Vector<TypeSymbol>();//Symbol ID is from Ptr
			Vector<TypeSymbol>* TypeToHave = new Vector<TypeSymbol>();
			TypesToChange->push_back(TypeToSwap);
			TypeToHave->push_back(Type);

			GenericTypeInstantiate(SybV, *TypeToHave);

			delete TypesToChange;
			delete TypeToHave;
		}
		else
		{
			SybV = GetSymbol(Name, SymbolType::Type);
		}
		if (SybV == nullptr)
		{
			auto Token = V.Name.Token;
			
			LogCantFindTypeError(Token, Name);
		}
		else
		{
			Out =SybV->VarType;
		}
	}break;
	default:
		throw std::exception("not added");
		break;
	}
	if (V.IsAddess) {Out._IsAddress = true;}
	if (V.IsAddessArray){Out._IsAddressArray = true;}
	if (V.Isimmutable){Out._Isimmutable = true;}
}

bool SystematicAnalysis::IsVaidType(TypeSymbol& Out)
{
	return false;
}
bool SystematicAnalysis::CanBeImplicitConverted(const TypeSymbol& TypeToCheck, const TypeSymbol& Type)
{
	if (AreTheSameWithOutimmutable(TypeToCheck, Type)) 
	{ 
		bool V0 =IsimmutableRulesfollowed(TypeToCheck, Type);

		bool V1 = IsAddessAndLValuesRulesfollowed(TypeToCheck, Type);

		return V0 && V1;
	}

	
	
	return false;
}
bool SystematicAnalysis::CanBeExplicitlyConverted(const TypeSymbol& TypeToCheck, const TypeSymbol& Type)
{
	if (CanBeImplicitConverted(TypeToCheck, Type)) { return true; }
	
	if (IsIntType(TypeToCheck) && IsIntType(TypeToCheck)) { return true; }
	return false;
}
bool SystematicAnalysis::IsSIntType(const UCodeLang::TypeSymbol& TypeToCheck)
{
	return
		TypeToCheck._Type == TypesEnum::sInt8 ||
		TypeToCheck._Type == TypesEnum::sInt16 ||
		TypeToCheck._Type == TypesEnum::sInt32 ||
		TypeToCheck._Type == TypesEnum::sInt64 ||
		TypeToCheck._Type == TypesEnum::sIntPtr;
}
bool SystematicAnalysis::IsUIntType(const UCodeLang::TypeSymbol& TypeToCheck)
{
	return
		TypeToCheck._Type == TypesEnum::uInt8 ||
		TypeToCheck._Type == TypesEnum::uInt16 ||
		TypeToCheck._Type == TypesEnum::uInt32 ||
		TypeToCheck._Type == TypesEnum::uInt64 ||
		TypeToCheck._Type == TypesEnum::uIntPtr ;
}
inline bool SystematicAnalysis::IsimmutableRulesfollowed(const TypeSymbol& TypeToCheck, const TypeSymbol& Type)
{
	return  (!TypeToCheck.Isimmutable() == Type.Isimmutable()) ||
		(TypeToCheck.Isimmutable() == Type.Isimmutable());
}
inline bool SystematicAnalysis::IsAddessAndLValuesRulesfollowed(const TypeSymbol& TypeToCheck, const TypeSymbol& Type)
{
	return (TypeToCheck.IsLocationValue() == Type.IsLocationValue()) ||
		(!TypeToCheck.IsRawValue() == Type.IsAddress()) ||
		(TypeToCheck.IsRawValue() == Type.IsRawValue())
		;
}
bool SystematicAnalysis::GetSize(TypeSymbol& Type, UAddress& OutSize)
{
	if (Type.IsAddress()){goto IntPtr;}
	switch (Type._Type)
	{
	case TypesEnum::sInt8:
	case TypesEnum::uInt8:
	case TypesEnum::Bool:
	case TypesEnum::Char:
		OutSize = 1;
		return true;
	case TypesEnum::sInt16:
	case TypesEnum::uInt16:
		OutSize = sizeof(UInt16);
		return true;

	case TypesEnum::sInt32:
	case TypesEnum::uInt32:
		OutSize = sizeof(UInt32);
		return true;
	case TypesEnum::uInt64:
	case TypesEnum::sInt64:
		OutSize = sizeof(UInt64);
		return true;

	case TypesEnum::sIntPtr:
	case TypesEnum::uIntPtr:
	IntPtr:
		OutSize = sizeof(UInt64);
		return true;
	default:
		OutSize = 0;
		return false;
	}
	


}
void SystematicAnalysis::GenericFuncInstantiate(Symbol* Func, const FuncNode& FuncBase, const Vector<TypeSymbol>& TypeToChage, const Vector<TypeSymbol>& Type)
{
	String NewName = GetGenericFuncName(Func, Type);
	GenericFuncInfo Info;
	Info.GenericFuncName = NewName;
	Info.Type = &Type;
	Info.TypeToChage = &TypeToChage;
	GenericFuncName.push(Info);

	auto OldScope = _Table._Scope.ThisScope;
	_Table._Scope.ThisScope.clear();

	passtype = PassType::GetTypes;
	OnFuncNode(FuncBase);
	passtype = PassType::FixedTypes;
	OnFuncNode(FuncBase);
	
	if (!_ErrorsOutput->Has_Errors()) {
		passtype = PassType::BuidCode;
		OnFuncNode(FuncBase);
	}
	passtype = PassType::FixedTypes;

	GenericFuncName.pop();
	//
	_Table._Scope.ThisScope = OldScope;
}
String SystematicAnalysis::GetGenericFuncName(UCodeLang::Symbol* Func, const UCodeLang::Vector<UCodeLang::TypeSymbol>& Type)
{
	String NewName = Func->FullName + "<";
	for (auto& Item : Type)
	{
		NewName += ToString(Item);
		if (&Type.back() != &Item)
		{
			NewName += ".";
		}
	}
	NewName += ">";
	return NewName;
}
void SystematicAnalysis::GenericTypeInstantiate(Symbol* Class, const Vector<TypeSymbol>& Type)
{
	String NewName = GetGenericFuncName(Class, Type);


	GenericFuncInfo Info;
	Info.GenericFuncName = NewName;
	Info.Type = &Type;
	Info.TypeToChage = &Type;
	GenericFuncName.push(Info);


	auto OldScope = _Table._Scope.ThisScope;
	_Table._Scope.ThisScope.clear();

	const ClassNode* node = (const ClassNode*)Class->SomePtr;
	passtype = PassType::GetTypes;
	OnClassNode(*node);
	passtype = PassType::FixedTypes;
	OnClassNode(*node);

	if (!_ErrorsOutput->Has_Errors()) {
		passtype = PassType::BuidCode;
		OnClassNode(*node);
	}
	passtype = PassType::FixedTypes;

	GenericFuncName.pop();
	//
	_Table._Scope.ThisScope = OldScope;
}

void SystematicAnalysis::CheckVarWritingErrors(UCodeLang::Symbol* Symbol, const Token* Token, UCodeLang::String_view& Name)
{
	if (Symbol->VarType.Isimmutable())
	{
		LogCantModifyiMutableError(Token, Name);
	}
}

void SystematicAnalysis::LogCantCastImplicitTypes(const UCodeLang::Token* Token, UCodeLang::TypeSymbol& Ex1Type, UCodeLang::TypeSymbol& UintptrType)
{
	bool V1 = IsAddessAndLValuesRulesfollowed(Ex1Type, UintptrType);
	if (!V1)
	{
		_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
			, "The expression is not an Location in memory'");
	}
	else
	{
		_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
			, "Cant cast Type '" + ToString(Ex1Type) + " to '" + ToString(UintptrType) + "'");
	}
}
void SystematicAnalysis::LogReadingFromInvaidVariable(const UCodeLang::Token* Token, UCodeLang::String& Str)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "the variable named '" + Str + "'" + " cant be read from you.can not read an invaid variable");
}
void SystematicAnalysis::LogCantFindVarError(const UCodeLang::Token* Token, UCodeLang::String& Str)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Cant find Variable Named '" + Str + "'");
}
void SystematicAnalysis::LogCantFindBinaryOpForTypes(const UCodeLang::Token* BinaryOp, UCodeLang::TypeSymbol& Ex0Type, UCodeLang::TypeSymbol& Ex1Type)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidType, BinaryOp->OnLine, BinaryOp->OnPos,
		"The type '" + ToString(Ex0Type) + "'" + " cant be '"
		+ ToString(BinaryOp->Type) + "' with '" + ToString(Ex1Type) + "'");
}
void SystematicAnalysis::ExpressionMustbeAnLocationValueError(const UCodeLang::Token* Token, UCodeLang::TypeSymbol& Ex0Type)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos,
		"expression must be an Location not an Value'" + ToString(Ex0Type) + "'");
}
void SystematicAnalysis::YouMustReturnSomethingError(const UCodeLang::Token* Token)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos
		, "you must return something");
}
void SystematicAnalysis::CantguessVarTypeError(const UCodeLang::Token* Token)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos
		, "cant guess 'var' type");
}
void SystematicAnalysis::CantUseThisKeyWordHereError(const UCodeLang::Token* NameToken)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidType, NameToken->OnLine, NameToken->OnPos, "cant use this here");
}
void SystematicAnalysis::CantgussTypesTheresnoassignment(const UCodeLang::Token* Token)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos
		, "cant guess type theres no '=' [expression]");
}
void SystematicAnalysis::LogCantCastExplicityTypes(const UCodeLang::Token* Token, UCodeLang::TypeSymbol& Ex0Type, UCodeLang::TypeSymbol& ToTypeAs)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Cant cast Type '" + ToString(Ex0Type) + " to '" + ToString(ToTypeAs) + "'");
}

SystematicAnalysis::ReadVarErrorCheck_t SystematicAnalysis::LogTryReadVar(String_view VarName, const UCodeLang::Token* Token, const Symbol* Syb)
{
	ReadVarErrorCheck_t r;
	if (Syb == nullptr)
	{
		LogCantFindVarError(Token, (String)VarName);
		r.CantFindVar = true;
	}
	else
	{
		if (Syb->IsInvalid())
		{
			LogReadingFromInvaidVariable(Token, (String)VarName);
			r.VarIsInvalid = true;
		}
	}
	return r;
}

void SystematicAnalysis::LogCantFindFuncError(const UCodeLang::Token* Token, UCodeLang::String_view& FuncName)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Cant Find function '" + (String)FuncName);
}
void SystematicAnalysis::LogCantModifyiMutableError(const UCodeLang::Token* Token, UCodeLang::String_view& Name)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Cant modify '" + (String)Name + " it's immutable");
}
void SystematicAnalysis::LogCantFindTypeError(const UCodeLang::Token* Token, UCodeLang::String_view& Name)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Cant Find Type '" + (String)Name + "'");
}
void SystematicAnalysis::CheckBackEnd()
{
	if (BackEnd == nullptr)
	{
		_ErrorsOutput->AddError(ErrorCodes::BackEndError, 0, 0, "There is not backend to compile to");

		return;
	}
}
UCodeLangEnd


