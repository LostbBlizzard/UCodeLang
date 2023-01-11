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
	const auto& ClassName = Node.ClassName.Token->Value._String;


	_Table.AddScope(ClassName);

	
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
		case NodeType::EnumNode:OnEnum(*EnumNode::As(node)); break;
		case NodeType::UsingNode: OnUseingNode(*UsingNode::As(node)); break;
		case NodeType::FuncNode:OnFuncNode(*FuncNode::As(node)); break;
		case NodeType::DeclareVariableNode:OnDeclareVariablenode(*DeclareVariableNode::As(node)); break;
		default:break;
		}
	}

	if (passtype == PassType::GetTypes)
	{
		GetTypesClass(Class);
	}

	_Table.RemovePopUseing(UseingIndex);


	_ClassStack.pop();
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

	auto FuncName = IsgenericInstantiation ? GenericFuncName.top().GenericFuncName
		: node.Signature.Name.AsStringView();
	auto NameToken = node.Signature.Name.Token;
	if (NameToken->Type == TokenType::KeyWorld_This)
	{
		FuncName = ClassConstructorfunc;
		if (_ClassStack.empty())
		{
			_ErrorsOutput->AddError(ErrorCodes::InValidType, NameToken->OnLine, NameToken->OnPos, "cant use this here");
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
		ClassMethod V;
		V.FullName = _Table._Scope.ThisScope;
		PushTepAttributesInTo(V.Attributes);


		Ptr->_Class.Methods.push_back(V);

		SymbolType Type = node.Signature.Generic.Values.size() && IsgenericInstantiation ==false ?
			SymbolType::GenericFunc : SymbolType::Func ;



		syb = &_Table.AddSybol(Type, (String)FuncName, FullName);
		syb->SomePtr = (void*)&node;//the node will not get update anyway.
		_Table.AddSymbolID(*syb, sybId);


		auto& GenericList = node.Signature.Generic;
		for (size_t i = 0; i < GenericList.Values.size(); i++)
		{
			auto& Item = GenericList.Values[i];
			
			auto GenericTypeName = Item.AsString();
			auto GenericType = &_Table.AddSybol(SymbolType::Type, GenericTypeName, FullName + ScopeHelper::_ScopeSep
				+ GenericTypeName);
			
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
			auto& Class = _Lib.Get_Assembly().AddClass(NewName,NewName);
			

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

		syb = &_Table.GetSymbol(sybId);//resized _Table
		Convert(node.Signature.ReturnType,syb->VarType);
	}
	else
	{
		syb = &_Table.GetSymbol(sybId);
	}
	bool buidCode = passtype == PassType::BuidCode;
	bool IsGenericS = node.Signature.Generic.Values.size();
	bool ignoreBody = false;
	if (!IsgenericInstantiation && IsGenericS) {
		ignoreBody = true;
	}
	
	if (buidCode && !ignoreBody)
	{
		_Builder.Build_Func(sybId);
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
						_ErrorsOutput->AddError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos
							, "cant guess 'var' type");
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
					_ErrorsOutput->AddError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos
						, "you must return something");
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

	_Table.RemoveScope();
}
void SystematicAnalysis::OnRetStatement(const RetStatementNode& node)
{
	if (node.Expression.Value)
	{
		OnExpressionTypeNode(node.Expression.Value);
	}
	else
	{
		LastExpressionType.SetType(TypesEnum::Void);
	}


	if (passtype == PassType::FixedTypes) 
	{
		auto T = Get_LookingForType();
		if (T._Type != TypesEnum::Var) 
		{
			if (!CanBeImplicitConverted(LastExpressionType, T))
			{
				if (T._Type != TypesEnum::Null) {
					_ErrorsOutput->AddError(ErrorCodes::InValidType, LastLookedAtToken->OnLine, LastLookedAtToken->OnPos,
						"cant convert '" + ToString(LastExpressionType) + "' to" + "'" + ToString(T) + "'");
				}
			}
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
	auto FullName = _Table._Scope.ThisScope + ScopeHelper::_ScopeSep + StrVarName;

	SymbolID sybId = (SymbolID)&node;
	Symbol* syb;


	if (passtype == PassType::GetTypes)
	{
		syb = &_Table.AddSybol(SymbolType::StackVarable, StrVarName, FullName);
		_Table.AddSymbolID(*syb, sybId);

		syb->Size = sizeof(UInt8);
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
		Convert(node.Type, syb->VarType);
		if (VarType._Type == TypesEnum::Var && node.Expression.Value == nullptr)
		{
			auto Token = node.Type.Name.Token;
			_ErrorsOutput->AddError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos
				, "cant guess type theres no '=' [expression]");
		}
	}
	LookingForTypes.push(syb->VarType);

	if (node.Expression.Value)
	{
		OnExpressionTypeNode(node.Expression.Value);
	}

	if (passtype == PassType::FixedTypes)
	{
		if (node.Expression.Value)
		{
			syb = &_Table.GetSymbol(sybId);
			auto& VarType = syb->VarType;
			auto& Ex = LastExpressionType;
			auto Token = node.Type.Name.Token;
			if (VarType._Type == TypesEnum::Var)
			{
				if (Ex._Type == TypesEnum::Var)
				{
					_ErrorsOutput->AddError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos
						, "cant guess 'var' type");
				}
				else
				{ 
					VarType = Ex;
				}
			}
			if (!CanBeImplicitConverted(Ex, VarType))
			{
				_ErrorsOutput->AddError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos,
					"cant convert '" + ToString(Ex) + "' to" + "'" + ToString(VarType) + "'");
			}
		}
	}
	LookingForTypes.pop();

	if (passtype == PassType::BuidCode)
	{
		auto Op = IROperand::AsLocation(_LastExpressionField);
		auto NewOp = IROperand::AsVarable(sybId);
		_Builder.Build_Assign(NewOp, Op);	
	}
}
void SystematicAnalysis::OnAssignVariableNode(const AssignVariableNode& node)
{
	OnExpressionTypeNode(node.Expression.Value);

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
	if (passtype == PassType::BuidCode)
	{
		auto Symbol = GetSymbol(node.Name.AsStringView(), SymbolType::Varable_t);
		SymbolID sybId = Symbol->ID;


		_Builder.Build_Assign(IROperand::AsInt8(1));
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
			if (passtype == PassType::BuidCode)
			{
				auto& Str = num->Token->Value._String;
				UInt8 V;
				ParseHelper::ParseStringToUInt8(Str, V);


				_Builder.Build_Assign(IROperand::AsInt8(V));
				_LastExpressionField = _Builder.GetLastField();

			}


			LastExpressionType.SetType(TypesEnum::sInt32);
			LastLookedAtToken = num->Token;
		}
		break;
		case NodeType::BoolliteralNode:
		{BoolliteralNode* num = BoolliteralNode::As(node.Value);
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
			if (Symbol == nullptr)
			{
				
				_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
					, "Cant find Variable Named '" + Str + "'");
					return;

			}


			SymbolID sybId = Symbol->ID;
			if (passtype == PassType::BuidCode)
			{


				_Builder.Build_Assign(IROperand::AsReadVarable(sybId));
				_LastExpressionField = _Builder.GetLastField();
			}

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
			_ErrorsOutput->AddError(ErrorCodes::InValidType, BinaryOp->OnLine, BinaryOp->OnPos,
				"The type '" + ToString(Ex0Type) + "'" + " cant be '"
			+ ToString(BinaryOp->Type) + "' with '" + ToString(Ex1Type) + "'");
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

			_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
				, "Cant cast Type '" + ToString(Ex0Type) + " to '" + ToString(ToTypeAs) + "'");
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
		if (Syb == nullptr)
		{
			auto  Token = node.FuncName.Token;
			_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
				, "Cant Find function '" + (String)FuncName);
			return;
		}
		if (node.Generics.Values.size())
		{

			const FuncNode& fnode = *(const FuncNode*)Syb->SomePtr;
			const auto& Tnode = node.Generics.Values[0];

			TypeSymbol Type; Convert(Tnode.node, Type);

			auto TypeToSwap = GetSymbol((String)FuncName + ScopeHelper::_ScopeSep + "T", SymbolType::Type)->VarType;

			Vector<TypeSymbol>* TypesToChange =new Vector<TypeSymbol>();
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

			LastExpressionType = GenerisESyb->VarType;
			delete  TypesToChange;
			delete TypeToHave;
		}
		else {
			LastExpressionType = Syb->VarType;
		}
	}
}
void SystematicAnalysis::CheckBackEnd()
{
	if (BackEnd == nullptr)
	{
		_ErrorsOutput->AddError(ErrorCodes::BackEndError, 0, 0,"There is not backend to compile to");

		return;
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
		if (node.FullNameType == Uint8TypeName || node.FullNameType == Sint8TypeName
			|| node.FullNameType == CharTypeName || node.FullNameType == boolTypeName)
		{
			node.offset = Class._Class.Size;
			Class._Class.Size += sizeof(UInt8);
		}
		else if (node.FullNameType == Uint16TypeName || node.FullNameType == Sint16TypeName)
		{
			node.offset = Class._Class.Size;
			Class._Class.Size += sizeof(UInt16);
		}
		else if (node.FullNameType == Uint32TypeName || node.FullNameType == Sint32TypeName)
		{
			node.offset = Class._Class.Size;
			Class._Class.Size += sizeof(UInt32);
		}
		else if (node.FullNameType == Uint64TypeName || node.FullNameType == Sint64TypeName
			|| node.FullNameType == UintPtrTypeName || node.FullNameType == SintPtrTypeName)
		{
			node.offset = Class._Class.Size;
			Class._Class.Size += sizeof(UInt64);
		}
		else
		{
			node.offset = Class._Class.Size;
			Class._Class.Size += sizeof(UInt64);
		}
	}
}
bool SystematicAnalysis::AreTheSame(const TypeSymbol& TypeA, const TypeSymbol& TypeB)
{
	if (TypeA._Type == TypeB._Type)
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
	switch (Type._Type)
	{
	case TypesEnum::Var:return "var";
	case TypesEnum::Int_t:return "Int_t";
	case TypesEnum::uInt_t:return "uInt_t";
	case TypesEnum::sInt_t:return "sInt_t";


	case TypesEnum::uInt8:return "uInt8";
	case TypesEnum::uInt16:return "uInt16";
	case TypesEnum::uInt32:return "uInt32";
	case TypesEnum::uInt64:return "uInt64";


	case TypesEnum::sInt8:return "sInt8";
	case TypesEnum::sInt16:return "sInt16";
	case TypesEnum::sInt32:return "sInt32";
	case TypesEnum::sInt64:return "sInt64";

	case TypesEnum::Bool:return "bool";
	case TypesEnum::CustomType:
	{
		auto Syb = _Table.GetSymbol(Type._CustomTypeSymbol);
		if (Syb.Type == SymbolType::Func
			|| Syb.Type == SymbolType::GenericFunc)
		{
			return ToString(Syb.VarType);
		}
		else
		{
			return Syb.FullName;
		}
	}
	case TypesEnum::Null:
		return "null (incomplete/broken Type)";
	default:
		break;
	}
	return "[Type Name Here]";
}
void SystematicAnalysis::Convert(const TypeNode& V, TypeSymbol& Out)
{
	switch (V.Name.Token->Type)
	{
	case TokenType::KeyWorld_var:
		Out._Type = TypesEnum::Var;
		break;
	case TokenType::KeyWorld_UInt8:
		Out._Type = TypesEnum::uInt8;
		break;
	case TokenType::KeyWorld_UInt16:
		Out._Type = TypesEnum::uInt16;
		break;
	case TokenType::KeyWorld_UInt32:
		Out._Type = TypesEnum::uInt32;
		break;
	case TokenType::KeyWorld_UInt64:
		Out._Type = TypesEnum::uInt64;
		break;

	case TokenType::KeyWorld_SInt8:
		Out._Type = TypesEnum::sInt8;
		break;
	case TokenType::KeyWorld_SInt16:
		Out._Type = TypesEnum::sInt16;
		break;
	case TokenType::KeyWorld_SInt32:
		Out._Type = TypesEnum::sInt32;
		break;
	case TokenType::KeyWorld_SInt64:
		Out._Type = TypesEnum::sInt64;
		break;


	case TokenType::KeyWorld_Bool:
		Out._Type = TypesEnum::Bool;
		break;
	case TokenType::Name: 
	{
		Out._Type = TypesEnum::CustomType;

		auto Name = V.Name.AsStringView();
		auto SybV = GetSymbol(Name, SymbolType::Type);
		if (SybV == nullptr)
		{
			auto Token = V.Name.Token;
			_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
				, "Cant Find Type '" + (String)Name + "'");
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
}
bool SystematicAnalysis::IsVaidType(TypeSymbol& Out)
{
	return false;
}
bool SystematicAnalysis::CanBeImplicitConverted(const TypeSymbol& TypeToCheck, const TypeSymbol& Type)
{
	if (AreTheSame(TypeToCheck, Type)) { return true; }

	
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
		TypeToCheck._Type == TypesEnum::sInt64;
}
bool SystematicAnalysis::IsUIntType(const UCodeLang::TypeSymbol& TypeToCheck)
{
	return
		TypeToCheck._Type == TypesEnum::uInt8 ||
		TypeToCheck._Type == TypesEnum::uInt16 ||
		TypeToCheck._Type == TypesEnum::uInt32 ||
		TypeToCheck._Type == TypesEnum::uInt64;
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
UCodeLangEnd


