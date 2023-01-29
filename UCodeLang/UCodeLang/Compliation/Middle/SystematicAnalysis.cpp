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
	Vector<const UClib*> Libs;
	Vector<const FileNode*> files;
	files.push_back(&File);
	return Analyze(files, Libs);
}
bool SystematicAnalysis::Analyze(const Vector<Unique_ptr<FileNode>>& Files, const Vector<Unique_ptr<UClib>>& Libs)
{
	Vector<const UClib*> libs;
	Vector<const FileNode*> files;

	for (auto& Item : Files)
	{
		files.push_back(Item.get());
	}
	for (auto& Item : Libs)
	{
		libs.push_back(Item.get());
	}

	return Analyze(files,libs);
}
bool SystematicAnalysis::Analyze(const Vector<const FileNode*>& Files, const Vector<const UClib*>& Libs)
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
	for (auto& node : node._Nodes)
	{
		switch (node->Get_Type())
		{
		case NodeType::NamespaceNode:OnNamespace(*NamespaceNode::As(node.get())); break;
		case NodeType::AttributeNode:OnAttributeNode(*AttributeNode::As(node.get())); break;
		case NodeType::ClassNode: OnClassNode(*ClassNode::As(node.get())); break;
		case NodeType::AliasNode:OnAliasNode(*AliasNode::As(node.get())); break;
		case NodeType::EnumNode:OnEnum(*EnumNode::As(node.get())); break;
		case NodeType::FuncNode:OnFuncNode(*FuncNode::As(node.get())); break;
		case NodeType::UsingNode: OnUseingNode(*UsingNode::As(node.get())); break;
		default:break;
		}
	}
	_Table.RemoveScope();

	_Table.RemovePopUseing(UseingIndex);
}
void SystematicAnalysis::OnAttributeNode(const AttributeNode& node)
{
	//if (passtype == PassType::GetTypes) {
		_TepAttributes.push_back(&node);
	//}
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
void SystematicAnalysis::OnFileNode(const FileNode* const& File)
{
	for (auto& node : File->_Nodes)
	{
		switch (node->Get_Type())
		{
		case NodeType::NamespaceNode:OnNamespace(*NamespaceNode::As(node.get())); break;
		case NodeType::AttributeNode:OnAttributeNode(*AttributeNode::As(node.get())); break;
		case NodeType::AliasNode:OnAliasNode(*AliasNode::As(node.get())); break;
		case NodeType::ClassNode: OnClassNode(*ClassNode::As(node.get())); break;
		case NodeType::EnumNode:OnEnum(*EnumNode::As(node.get())); break;
		case NodeType::FuncNode:OnFuncNode(*FuncNode::As(node.get())); break;
		case NodeType::UsingNode: OnUseingNode(*UsingNode::As(node.get())); break;
		default:break;
		}
	}
	_Table.ClearUseings();
}
void SystematicAnalysis::OnClassNode(const ClassNode& Node)
{
	bool IsgenericInstantiation = GenericFuncName.size() && GenericFuncName.top().NodeTarget == &Node;
	bool Isgeneric = Node.Generic.Values.size();
	bool Isgeneric_t = Isgeneric && IsgenericInstantiation == false;


	const auto& ClassName = IsgenericInstantiation ? GenericFuncName.top().GenericFuncName : Node.ClassName.Token->Value._String;
	_Table.AddScope(ClassName);

	auto SybID = IsgenericInstantiation ? (SymbolID)GenericFuncName.top().GenericInput : (SymbolID)&Node;

	auto& Syb = passtype == PassType::GetTypes ?
		_Table.AddSybol(Isgeneric_t ? SymbolType::Generic_class : SymbolType::Type_class
			, (String)ClassName, _Table._Scope.ThisScope) :
		_Table.GetSymbol(SybID);



	ClassInfo* ClassInf = nullptr;
	if (passtype == PassType::GetTypes)
	{
		_Table.AddSymbolID(Syb, SybID);
		Syb.NodePtr = (void*)&Node;



		ClassInf = new ClassInfo();
		ClassInf->FullName = Syb.FullName;
		Syb.Info.reset(ClassInf);
		Syb.VarType.SetType(Syb.ID);


		
		auto& GenericList = Node.Generic;
		for (size_t i = 0; i < GenericList.Values.size(); i++)
		{
			auto& Item = GenericList.Values[i];

			auto GenericTypeName = Item.AsString();
			auto GenericType = &_Table.AddSybol(SymbolType::Type_alias, GenericTypeName,
				_Table._Scope.GetApendedString(GenericTypeName));



			if (IsgenericInstantiation)
			{
				GenericType->Type = SymbolType::Type_alias;

				GenericFuncInfo& V2 = GenericFuncName.top();
				GenericType->VarType = (*V2.GenericInput)[i];
			}
			else
			{
				GenericType->Type = SymbolType::Unmaped_Generic_Type;

				GenericType->NodePtr = (void*)&Item;
				SymbolID ID = (SymbolID)GenericType->NodePtr;
				_Table.AddSymbolID(*GenericType, ID);

				ClassInf->_Generic.push_back(ID);
			}
		}
	}
	else
	{
		ClassInf = (ClassInfo*)Syb.Info.get();
	}
	
	
	if (!Isgeneric_t)
	{
		
		_ClassStack.push(ClassInf);
		PushClassDependencie(ClassInf);

		//if (passtype == PassType::GetTypes)
		{
			//PushTepAttributesInTo(Class._Class.Attributes);
		}
		
		if (passtype == PassType::BuidCode)
		{
			AddClass_tToAssemblyInfo(ClassInf);
		}

		auto UseingIndex = _Table.GetUseingIndex();

		for (const auto& node : Node._Nodes)
		{
			switch (node->Get_Type())
			{
			case NodeType::AttributeNode:OnAttributeNode(*AttributeNode::As(node.get())); break;
			case NodeType::ClassNode: OnClassNode(*ClassNode::As(node.get())); break;
			case NodeType::AliasNode:OnAliasNode(*AliasNode::As(node.get())); break;
			case NodeType::EnumNode:OnEnum(*EnumNode::As(node.get())); break;
			case NodeType::UsingNode: OnUseingNode(*UsingNode::As(node.get())); break;
			case NodeType::FuncNode:OnFuncNode(*FuncNode::As(node.get())); break;
			case NodeType::DeclareVariableNode:OnDeclareVariablenode(*DeclareVariableNode::As(node.get())); break;
			default:break;
			}
		}

		if (passtype == PassType::FixedTypes)
		{
			ClassInf->SizeInitialized = true;
		}

		PopClassDependencie();
		_Table.RemovePopUseing(UseingIndex);


		_ClassStack.pop();

	}
	else
	{
		Syb.NodePtr = (void*)&Node;

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

	if (passtype == PassType::BuidCode)
	{
		auto& V = _Lib.Get_Assembly().Add_Alias((String)ClassName, _Table._Scope.ThisScope);
		V.StringValue = ToString(Syb.VarType);

	}

	_Table.RemoveScope();
}
void SystematicAnalysis::OnUseingNode(const UsingNode& node)
{
	auto& T =node.ScopedName.ScopedName.back().token;
	OnNonAttributeable(T->OnLine, T->OnPos);
	const auto UseingString =GetScopedNameAsString(node.ScopedName);
	_Table.AddUseing(UseingString);

}
void SystematicAnalysis::OnFuncNode(const FuncNode& node)
{
	bool IsgenericInstantiation = GenericFuncName.size() && GenericFuncName.top().NodeTarget == &node;
	bool IsGenericS = node.Signature.Generic.Values.size();

	auto FuncName = IsgenericInstantiation ? GenericFuncName.top().GenericFuncName
		: node.Signature.Name.AsStringView();
	auto NameToken = node.Signature.Name.Token;

	if (NameToken->Type == TokenType::KeyWorld_new)
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
	

	

	Symbol* syb;
	SymbolID sybId;
	if (IsgenericInstantiation)
	{
		sybId = (SymbolID)GenericFuncName.top().GenericInput;
	}
	else
	{
		sybId = (SymbolID)&node;
		if (GenericFuncName.size())
		{
			sybId += (SymbolID)GenericFuncName.top().GenericInput;
		}
	}
	

	auto UseingIndex = _Table.GetUseingIndex();

	if (passtype == PassType::GetTypes)
	{
		SymbolType Type = IsGenericS && IsgenericInstantiation == false ?
			SymbolType::GenericFunc : SymbolType::Func;

		

		syb = &_Table.AddSybol(Type, (String)FuncName, FullName);
		syb->NodePtr = (void*)&node;//the node will not get update anyway.
		_Table.AddSymbolID(*syb, sybId);

		FuncInfo* newInfo = new FuncInfo();
		newInfo->FullName = FullName;
		
		syb->Info.reset(newInfo);

		auto& GenericList = node.Signature.Generic;	
		
		
		for (size_t i = 0; i < GenericList.Values.size(); i++)
		{
			auto& Item = GenericList.Values[i];

			auto GenericTypeName = Item.AsString();
			auto GenericType = &_Table.AddSybol(SymbolType::Type_alias, GenericTypeName, 
				_Table._Scope.GetApendedString(GenericTypeName)
				);

			if (IsgenericInstantiation)
			{
				GenericType->Type = SymbolType::Type_alias;

				GenericFuncInfo& V2 = GenericFuncName.top();
				GenericType->VarType = (*V2.GenericInput)[i];
			}
			else
			{
				GenericType->Type = SymbolType::Unmaped_Generic_Type;

				GenericType->NodePtr = (void*)&Item;
				SymbolID ID = (SymbolID)GenericType->NodePtr;
				_Table.AddSymbolID(*GenericType,ID);	
				
				newInfo->_Generic.push_back(ID);
			}
		
		}

		

		auto& RetType = node.Signature.ReturnType.node;
		if (RetType && RetType->Get_Type() == NodeType::AnonymousTypeNode)
		{
			auto NewName = GetFuncAnonymousObjectFullName(FullName);


			SymbolID AnonymousSybID = (SymbolID)RetType.get();
			auto& AnonymousSyb = _Table.AddSybol(SymbolType::Type_class, (String)NewName, NewName); 
			
			_Table.AddSymbolID(AnonymousSyb,AnonymousSybID);


			auto ClassInf = new ClassInfo();
			ClassInf->FullName = NewName;
			AnonymousSyb.Info.reset(ClassInf);
			AnonymousSyb.VarType.SetType(AnonymousSyb.ID);

			AnonymousTypeNode* Typenode = AnonymousTypeNode::As(RetType.get());
			for (auto& Item3 : Typenode->Fields.Parameters)
			{
				TypeSymbol T;
				Convert(Item3.Type, T);

				ClassInf->AddField(Item3.Name.AsString(), T);
			}
		}

		

		auto ClassSymBool = _ClassStack.size() ? GetSymbol(_ClassStack.top()) : nullptr;
		for (auto& Item : node.Signature.Parameters.Parameters)
		{
			TypeSymbol VP;
			Convert(Item.Type,VP);



			String_view GenericTypeName;
			if (VP.IsAddress()  && (ClassSymBool && VP._CustomTypeSymbol == ClassSymBool->VarType._CustomTypeSymbol) )
			{
				GenericTypeName = ThisSymbolName;
			}
			else
			{
				if (Item.Name.Token != nullptr)
				{
					GenericTypeName = Item.Name.AsStringView();
				}
			}




			auto GenericType = &_Table.AddSybol(SymbolType::ParameterVarable, (String)GenericTypeName,
				_Table._Scope.GetApendedString(GenericTypeName)
			);
			auto ParSybID = (SymbolID)&Item;
			_Table.AddSymbolID(*GenericType, ParSybID);

			newInfo->Pars.push_back(VP);
		}
		
		
		syb = &_Table.GetSymbol(sybId);//resized _Table
	}
	else
	{
		syb = &_Table.GetSymbol(sybId);
	}
	FuncInfo* Info = (FuncInfo*)syb->Info.get();


	_FuncStack.push(Info);


	if (passtype == PassType::FixedTypes
		|| (IsGenericS && passtype == PassType::GetTypes))
	{
		
		auto& RetType = node.Signature.ReturnType.node;
		if (RetType && RetType->Get_Type() == NodeType::AnonymousTypeNode)
		{
			SymbolID AnonymousSybID = (SymbolID)RetType.get();
			auto& V = _Table.GetSymbol(AnonymousSybID);

			auto ClassInf = (ClassInfo*)V.Info.get(); 

			AnonymousTypeNode* Typenode = AnonymousTypeNode::As(RetType.get());

			for (size_t i = 0; i < Typenode->Fields.Parameters.size(); i++)
			{
				auto& Item3 = Typenode->Fields.Parameters[i];
				auto ItemOut = ClassInf->Fields[i];
				Convert(Item3.Type, ItemOut.Type);
			}

			
			PushClassDependencie(ClassInf);

			UAddress ClassSize=0;
			for (auto& Item : ClassInf->Fields)
			{
				UAddress V = NullAddress;
				GetSize(Item.Type, V);
				ClassSize += V;
			}

			PopClassDependencie();

			ClassInf->Size = ClassSize;
			ClassInf->SizeInitialized = true;
			

			syb->VarType.SetType(AnonymousSybID);
			Info->Ret = syb->VarType;
		}
		else
		{
			Convert(node.Signature.ReturnType, syb->VarType);
			Info->Ret = syb->VarType;
		}

		

		for (size_t i = 0; i < node.Signature.Parameters.Parameters.size(); i++)
		{
			auto& Item = node.Signature.Parameters.Parameters[i];
			auto& Item2 = Info->Pars[i];


			auto ParSybID = (SymbolID)&Item;
			auto& Sybol = _Table.GetSymbol(ParSybID);
			Convert(Item.Type, Sybol.VarType);
			Item2 = Sybol.VarType;

		}
		
	}	
	
	if (passtype == PassType::BuidCode)
	{
		ClassData* Ptr = nullptr;
		if (_ClassStack.empty())
		{
			auto& Assembly = _Lib.Get_Assembly();

			auto globalAssemblyObjectName = (String_view)ScopeHelper::_globalAssemblyObject;

			Ptr = Assembly.Find_Class(globalAssemblyObjectName);
			if (Ptr == nullptr)
			{
				Ptr = &_Lib.Get_Assembly().AddClass(String(globalAssemblyObjectName), String(globalAssemblyObjectName));
			}
		}
		else
		{
			auto& Assembly = _Lib.Get_Assembly();
			for (auto& Item : Assembly.Classes)
			{
				if (Item->FullName == ScopeHelper::GetReMoveScope((String_view)FullName))
				{
					Ptr = Item.get();
					break;
				}
		
			}
		}
		ClassMethod V;
		V.FullName = FullName;
		V.RetType.FullNameType = ToString(syb->VarType);

		for (auto& Item : Info->Pars)
		{
			auto& VT = V.ParsType.emplace_back(ToString(Item));

		}

		PushTepAttributesInTo(V.Attributes);


		Ptr->_Class.Methods.push_back(std::move(V));

		auto& RetType = node.Signature.ReturnType.node;
		if (RetType && RetType->Get_Type() == NodeType::AnonymousTypeNode)
		{
			SymbolID AnonymousSybID = (SymbolID)RetType.get();
			auto& V = _Table.GetSymbol(AnonymousSybID);

			auto ClassInf = (ClassInfo*)V.Info.get();

			AddClass_tToAssemblyInfo(ClassInf);

		}
	}


	bool buidCode = passtype == PassType::BuidCode;
	bool ignoreBody = !IsgenericInstantiation && IsGenericS;

	if (buidCode && !ignoreBody)
	{
		_Builder.Build_Func(sybId);

		for (auto& Item : node.Signature.Parameters.Parameters)
		{
			auto ParSybID = (SymbolID)&Item;
			_Builder.Build_Parameter(ParSybID);
			IRParameters.push_back(ParSybID);
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
			OnStatement(node2);
			if (node2->Get_Type() == NodeType::RetStatementNode)
			{
				syb = &_Table.GetSymbol(sybId);
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

			Info->Ret = syb->VarType;
		}

		LookingForTypes.pop();
	}

	if (buidCode && !ignoreBody)
	{
		_Builder.Build_Ret();
		IRParameters.clear();
	}

	_FuncStack.pop();

	_Table.RemovePopUseing(UseingIndex);



	
	_Table.RemoveScope();
}
void SystematicAnalysis::OnStatement(const Unique_ptr<UCodeLang::Node>& node2)
{
	switch (node2->Get_Type())
	{
	case NodeType::AttributeNode:OnAttributeNode(*AttributeNode::As(node2.get())); break;
	case NodeType::ClassNode: OnClassNode(*ClassNode::As(node2.get())); break;
	case NodeType::EnumNode:OnEnum(*EnumNode::As(node2.get())); break;
	case NodeType::UsingNode: OnUseingNode(*UsingNode::As(node2.get())); break;
	case NodeType::DeclareVariableNode:OnDeclareVariablenode(*DeclareVariableNode::As(node2.get())); break;
	case NodeType::AssignVariableNode:OnAssignVariableNode(*AssignVariableNode::As(node2.get())); break;
	case NodeType::PostfixVariableNode:OnPostfixVariableNode(*PostfixVariableNode::As(node2.get())); break;
	case NodeType::CompoundStatementNode:OnCompoundStatementNode(*CompoundStatementNode::As(node2.get())); break;
	case NodeType::FuncCallStatementNode:OnFuncCallNode(FuncCallStatementNode::As(node2.get())->Base); break;
	case NodeType::DropStatementNode:OnDropStatementNode(*DropStatementNode::As(node2.get())); break;
	case NodeType::IfNode:OnIfNode(*IfNode::As(node2.get())); break;
	case NodeType::WhileNode:OnWhileNode(*WhileNode::As(node2.get())); break;
	case NodeType::DoNode:OnDoNode(*DoNode::As(node2.get())); break;
	case NodeType::RetStatementNode:OnRetStatement(*RetStatementNode::As(node2.get())); break;
	default:break;
	}
}
void SystematicAnalysis::OnRetStatement(const RetStatementNode& node)
{
	if (node.Expression.Value)
	{
		auto& LookForT = Get_LookingForType();
		LookForT.SetAsRawValue();

		LookingForTypes.push(LookForT);
		OnExpressionTypeNode(node.Expression.Value.get());

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
		auto& T = Get_LookingForType();
		DoImplicitConversion(IROperand::AsLocation(_Builder.GetLastField()),LastExpressionType, T);
		if (node.Expression.Value) 
		{
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
	node.GetScopedName(Text);
	return Text;
}
void SystematicAnalysis::OnDeclareVariablenode(const DeclareVariableNode& node)
{
	auto& StrVarName = node.Name.AsString();
	auto FullName = _Table._Scope.GetApendedString(StrVarName);

	SymbolID sybId = (SymbolID)&node;
	Symbol* syb;

	bool InSideClass = _ClassStack.size() && _InStatements == false;

	if (passtype == PassType::GetTypes)
	{
		syb = &_Table.AddSybol(SymbolType::StackVarable, StrVarName, FullName);
		_Table.AddSymbolID(*syb, sybId);

		syb->Size = NullAddress;
		if (InSideClass)
		{
			syb->Type = SymbolType::Class_Field;
			auto& Class = *_ClassStack.top();

			if (Class.GetField(FullName))
			{
				//TODO Err
			}
			Class.AddField(ScopeHelper::GetNameFromFullName((String_view)FullName), TypeSymbol());
			
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
		OnExpressionTypeNode(node.Expression.Value.get());
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


		if (InSideClass)
		{
			auto& Class = *_ClassStack.top();
			if (!Class.SizeInitialized)
			{
				if (Class.Size == NullAddress)
				{
					Class.Size = 0;
				}

				auto& Type = syb->VarType;
				UAddress V;
				//GetSize(Type, V);//for DependencyCycle
			}
			auto V = Class.GetField(ScopeHelper::GetNameFromFullName(FullName));
			if (V)
			{
				V->Type = syb->VarType;

				Class.Size += syb->Size;
			}
		}
	}
	LookingForTypes.pop();

	if (passtype == PassType::BuidCode && node.Expression.Value)
	{

		DoImplicitConversion(IROperand::AsLocation(_Builder.GetLastField()), LastExpressionType, syb->VarType);

		auto Op = IROperand::AsLocation(_LastExpressionField);
		auto NewOp = IROperand::AsVarable(sybId);
		_Builder.Build_Assign(NewOp, Op);	
		BindTypeToLastIR(syb->VarType);

	}
}
void SystematicAnalysis::OnAssignVariableNode(const AssignVariableNode& node)
{
	GetMemberTypeSymbolFromVar_t MemberInfo;
	if (passtype != PassType::GetTypes)
	{
		if (!GetMemberTypeSymbolFromVar(node.Name, MemberInfo))
		{
			return;
		}
		LookingForTypes.push(MemberInfo.Type);
	}
	OnExpressionTypeNode(node.Expression.Value.get());
	if (passtype != PassType::GetTypes)
	{
		LookingForTypes.pop();
	}
	

	if (passtype == PassType::FixedTypes)
	{

		if (!CanBeImplicitConverted(LastExpressionType, MemberInfo.Type))
		{
			auto  Token = LastLookedAtToken;
			LogCantCastImplicitTypes(Token, LastExpressionType, MemberInfo.Type);

		}
		
	}

	


	if (passtype == PassType::BuidCode)
	{

		DoImplicitConversion(IROperand::AsLocation(_Builder.GetLastField()), LastExpressionType, MemberInfo.Type);

		auto Token = node.Name.ScopedName.begin()->token;
		auto& Str = Token->Value._String;
		Symbol* Symbol = GetSymbol(Str, SymbolType::Varable_t);


		bool BuildVarableCode = true;

		if (Symbol->Type == SymbolType::Class_Field && _FuncStack.size())
		{
			auto& Func = _FuncStack.top();

			if (auto ObjectType = Func->GetObjectForCall())
			{
				TypeSymbol ThisClassType;

				if (!AreTheSame(*ObjectType, ThisClassType))
				{

				}




				auto Op = IROperand::AsLocation(_LastExpressionField);
				auto NewOp = IROperand::AsPointer(IRParameters.front());
				_Builder.Build_Assign(NewOp, Op, MemberInfo.Offset);
				BindTypeToLastIR(MemberInfo.Type);
			}

		}
		
		if (BuildVarableCode) 
		{
			SymbolID sybId = Symbol->ID;
			auto Op = IROperand::AsLocation(_LastExpressionField);
			auto NewOp = IROperand::AsVarable(sybId);
			_Builder.Build_Assign(NewOp, Op, MemberInfo.Offset);
			BindTypeToLastIR(MemberInfo.Type);
		}
	}
}
void SystematicAnalysis::OnIfNode(const IfNode& node)
{

	TypeSymbol BoolType;
	BoolType.SetType(TypesEnum::Bool);

	String ScopeName = std::to_string((size_t)&node);
	_Table.AddScope(ScopeName);
	
	LookingForTypes.push(BoolType);

	OnExpressionTypeNode(node.Expression.Value.get());

	if (passtype == PassType::FixedTypes)
	{
		if (!CanBeImplicitConverted(LastExpressionType, BoolType))
		{
			auto  Token = LastLookedAtToken;
			LogCantCastImplicitTypes(Token, LastExpressionType, BoolType);
		}
	}

	
	IRField IfIndex;
	IROperand BoolCode;
	if (passtype == PassType::BuidCode)
	{
		DoImplicitConversion(IROperand::AsLocation(_Builder.GetLastField()), LastExpressionType, BoolType);
	
		BoolCode = IROperand::AsLocation(_Builder.GetLastField());
		_Builder.Build_IfFalseJump(BoolCode, 0);
		IfIndex = _Builder.GetLastField();
	}

	
	

	LookingForTypes.pop();

	for (const auto& node2 :node.Body._Nodes)
	{
		OnStatement(node2);
	}
	
	_Table.RemoveScope();
	
	if (node.Else)
	{
		IRField ElseIndex;
		if (passtype == PassType::BuidCode)
		{
			_Builder.Build_Jump(0);//ElseJump
			ElseIndex = _Builder.GetLastField();
		}


		if (node.Else->Get_Type() != NodeType::ElseNode)
		{
			OnStatement(node.Else);
		}
		else
		{
			ElseNode* Elsenode = ElseNode::As(node.Else.get());

			for (const auto& node3 : Elsenode->Body._Nodes)
			{
				OnStatement(node3);
			}
		}


		if (passtype == PassType::BuidCode)
		{
			auto& ElseJumpCode = _Builder.Get_IR(ElseIndex);
			_Builder.Update_Jump(ElseJumpCode,_Builder.GetNextField());

			auto& IFFalseCode = _Builder.Get_IR(IfIndex);
			_Builder.Update_IfFalseJump(IFFalseCode, BoolCode, ElseIndex + 1);
		}
	}

	if (passtype == PassType::BuidCode && node.Else == nullptr)
	{
		auto& IFFalseCode=_Builder.Get_IR(IfIndex);
		_Builder.Update_IfFalseJump(IFFalseCode, BoolCode, _Builder.GetNextField());
	}

	
}
void SystematicAnalysis::OnWhileNode(const WhileNode& node)
{
	TypeSymbol BoolType;
	BoolType.SetType(TypesEnum::Bool);

	String ScopeName = std::to_string((size_t)&node);
	_Table.AddScope(ScopeName);

	LookingForTypes.push(BoolType);

	OnExpressionTypeNode(node.Expression.Value.get());

	if (passtype == PassType::FixedTypes)
	{
		if (!CanBeImplicitConverted(LastExpressionType, BoolType))
		{
			auto  Token = LastLookedAtToken;
			LogCantCastImplicitTypes(Token, LastExpressionType, BoolType);
		}
	}


	IRField IfIndex;
	IROperand BoolCode;
	if (passtype == PassType::BuidCode)
	{
		DoImplicitConversion(IROperand::AsLocation(_Builder.GetLastField()), LastExpressionType, BoolType);

		BoolCode = IROperand::AsLocation(_Builder.GetLastField());
		_Builder.Build_IfFalseJump(BoolCode, 0);
		IfIndex = _Builder.GetLastField();
	}




	LookingForTypes.pop();

	for (const auto& node2 : node.Body._Nodes)
	{
		OnStatement(node2);
	}

	if (passtype == PassType::BuidCode)
	{
		_Builder.Build_Jump(IfIndex);
		
		auto& IFFalseCode = _Builder.Get_IR(IfIndex);
		_Builder.Update_IfFalseJump(IFFalseCode, BoolCode, _Builder.GetNextField());
	}

	_Table.RemoveScope();
}
void SystematicAnalysis::OnDoNode(const DoNode& node)
{

	String ScopeName = std::to_string((size_t)&node);
	_Table.AddScope(ScopeName);


	IRField StartIndex;
	if (passtype == PassType::BuidCode)
	{
		StartIndex = _Builder.GetNextField();
	}


	for (const auto& node2 : node.Body._Nodes)
	{
		OnStatement(node2);
	}

	_Table.RemoveScope();

	TypeSymbol BoolType;
	BoolType.SetType(TypesEnum::Bool);
	LookingForTypes.push(BoolType);

	OnExpressionTypeNode(node.Expression.Value.get());

	if (passtype == PassType::FixedTypes)
	{
		if (!CanBeImplicitConverted(LastExpressionType, BoolType))
		{
			auto  Token = LastLookedAtToken;
			LogCantCastImplicitTypes(Token, LastExpressionType, BoolType);
		}
	}


	IROperand BoolCode;
	if (passtype == PassType::BuidCode)
	{
		DoImplicitConversion(IROperand::AsLocation(_Builder.GetLastField()), LastExpressionType, BoolType);

		BoolCode = IROperand::AsLocation(_Builder.GetLastField());

		_Builder.Build_IfFalseJump(BoolCode, _Builder.GetNextField() + 2);//???
		_Builder.Build_Jump(StartIndex);
	}




	LookingForTypes.pop();

}
bool SystematicAnalysis::GetMemberTypeSymbolFromVar(const ScopedNameNode& node, GetMemberTypeSymbolFromVar_t& Out)
{
	size_t VarOffset = 0;
	auto Token = node.ScopedName.begin()->token;
	auto& Str = Token->Value._String;
	auto SymbolVar = GetSymbol(Str, SymbolType::Varable_t);
	if (SymbolVar == nullptr)
	{
		LogCantFindVarError(Token, Str);
		return false;
	}

	CheckVarWritingErrors(SymbolVar, Token, String_view(Token->Value._String));

	TypeSymbol FeildType = SymbolVar->VarType;
	Symbol* FeildTypeAsSymbol =nullptr;
	
	if (FeildType._Type == TypesEnum::CustomType)
	{
		FeildTypeAsSymbol = &_Table.GetSymbol(SymbolVar->VarType._CustomTypeSymbol);
		FeildType = SymbolVar->VarType;
	}
	


	for (size_t i = 1; i < node.ScopedName.size(); i++)
	{
		auto& Item = node.ScopedName[i];


		auto& ItemToken = Item.token;

		if (FeildType._Type != TypesEnum::CustomType)
		{

			if (passtype == PassType::FixedTypes) {
				LogCantFindVarMemberError(ItemToken, ItemToken->Value._String, FeildType);
			}break;
		}
		ClassInfo* CInfo = (ClassInfo*)FeildTypeAsSymbol->Info.get();


		auto FeldInfo = CInfo->GetField(ItemToken->Value._String);
		if (FeldInfo == nullptr)
		{
			if (passtype == PassType::FixedTypes) {
				LogCantFindVarMemberError(ItemToken, ItemToken->Value._String, FeildType);
			}
			return false;
		}

		UAddress _FieldOffset = 0;
		GetOffset(*CInfo, FeldInfo, _FieldOffset);

		VarOffset+=_FieldOffset;
		
		auto& FieldType2 = FeldInfo->Type;
		if (FieldType2._Type == TypesEnum::CustomType)
		{
			FeildTypeAsSymbol = &_Table.GetSymbol(FieldType2._CustomTypeSymbol);
			FeildType = FieldType2;
		}
		else
		{
			FeildType = FieldType2;
			
			if (i + 1 < node.ScopedName.size())
			{
				auto Token2 = node.ScopedName[i + 1].token;
				auto& Str2 = Token->Value._String;
				if (passtype == PassType::FixedTypes) {
					LogCantFindVarMemberError(Token2, Str2, FeildType);
				}
				break;
			}
		}
	}
	Out.Type = FeildType;
	Out.Offset = VarOffset;
	return true;
}
void SystematicAnalysis::OnPostfixVariableNode(const PostfixVariableNode& node)
{
	if (passtype == PassType::FixedTypes)
	{
		auto Name = GetScopedNameAsString(node.Name);
		auto Symbol = GetSymbol(Name, SymbolType::Varable_t);

		CheckVarWritingErrors(Symbol, node.Name.ScopedName.back().token,String_view(Name));
		LogTryReadVar(Name, node.Name.ScopedName.back().token, Symbol);


		if (!HasPostfixOverLoadWith(Symbol->VarType,node.PostfixOp->Type))
		{
			LogCantFindPostfixOpForTypes(node.PostfixOp, Symbol->VarType);
		}

	}
	if (passtype == PassType::BuidCode)
	{
		auto Symbol = GetSymbol(GetScopedNameAsString(node.Name), SymbolType::Varable_t);
		SymbolID sybId = Symbol->ID;
		auto& Type = Symbol->VarType;

		
		#define buildPortFixS(x)\
		_Builder.Build_Assign(IROperand::AsInt##x((Int##x)1));\
		auto V0 = IROperand::AsLocation(_Builder.GetLastField());\
		_Builder.Build_Assign(IROperand::AsReadVarable(sybId));\
		auto V1 = IROperand::AsLocation(_Builder.GetLastField());\
		if (node.PostfixOp->Type == TokenType::increment)\
		{\
			_Builder.MakeAdd##x(V0, V1);\
		}\
		else\
		{\
			_Builder.MakeSub##x(V0, V1);\
		}\

		#define buildPortFixU(x)\
		_Builder.Build_Assign(IROperand::AsInt##x((UInt##x)1));\
		auto V0 = IROperand::AsLocation(_Builder.GetLastField());\
		_Builder.Build_Assign(IROperand::AsReadVarable(sybId));\
		auto V1 = IROperand::AsLocation(_Builder.GetLastField());\
		if (node.PostfixOp->Type == TokenType::increment)\
		{\
			_Builder.MakeAdd##x(V0, V1);\
		}\
		else\
		{\
			_Builder.MakeSub##x(V0, V1);\
		}\


		switch (Type._Type)
		{
		case TypesEnum::uInt8:
		{
			buildPortFixU(8);
		}
		break;

		case TypesEnum::uInt16:
		{
			buildPortFixU(16);
		}
		break;
		case TypesEnum::uInt32:
		{
			buildPortFixU(32);
		}
		break;
		case TypesEnum::uInt64:
		{
			buildPortFixU(64);
		}
		break;
		case TypesEnum::uIntPtr:
		{
			Build_Assign_uIntPtr(1);
			auto V0 = IROperand::AsLocation(_Builder.GetLastField());
			_Builder.Build_Assign(IROperand::AsReadVarable(sybId));
			auto V1 = IROperand::AsLocation(_Builder.GetLastField());
			if (node.PostfixOp->Type == TokenType::increment)
			{
				Build_Add_uIntPtr(V0, V1);; 
			}
			else
			{
				Build_Sub_uIntPtr(V0, V1);
			}
		}
		break;
		case TypesEnum::sInt8:
		{
			buildPortFixS(8);
		}
		break;
		case TypesEnum::sInt16:
		{
			buildPortFixS(16);
		}
		break;
		case TypesEnum::sInt32:
		{
			buildPortFixS(32);
		}
		break;
		case TypesEnum::sInt64:
		{
			buildPortFixS(64);
		}
		break;
		case TypesEnum::sIntPtr:
		{
			Build_Assign_sIntPtr(1);
			auto V0 = IROperand::AsLocation(_Builder.GetLastField());
			_Builder.Build_Assign(IROperand::AsReadVarable(sybId));
			auto V1 = IROperand::AsLocation(_Builder.GetLastField());
			if (node.PostfixOp->Type == TokenType::increment)
			{
				Build_Add_sIntPtr(V0, V1);;
			}
			else
			{
				Build_Sub_sIntPtr(V0, V1);
			}
		}
		break;
		default:
			break;
		}
	


		auto Op = IROperand::AsLocation(_Builder.GetLastField());
		auto NewOp = IROperand::AsVarable(sybId);
		_Builder.Build_Assign(NewOp, Op);
		BindTypeToLastIR(Type);
	}
}
void SystematicAnalysis::OnCompoundStatementNode(const CompoundStatementNode& node)
{
	auto Name = GetScopedNameAsString(node.VariableName);
	auto Symbol = GetSymbol(Name, SymbolType::Varable_t);
	if (Symbol == nullptr){return;}

	LookingForTypes.push(Symbol->VarType);

	OnExpressionTypeNode(node.Expession.Value.get());

	LookingForTypes.pop();

	auto ExType = LastExpressionType;
	if (passtype == PassType::FixedTypes)
	{
	
		CheckVarWritingErrors(Symbol, node.VariableName.ScopedName.back().token,String_view(Name));
		LogTryReadVar(Name, node.VariableName.ScopedName.back().token, Symbol);

		if (!HasCompoundOverLoadWith(Symbol->VarType, node.CompoundOp->Type, ExType))
		{
			LogCantFindCompoundOpForTypes(node.CompoundOp, Symbol->VarType, ExType);
		}
	}
	if (passtype == PassType::BuidCode)
	{
		auto Symbol = GetSymbol(GetScopedNameAsString(node.VariableName), SymbolType::Varable_t);
		SymbolID sybId = Symbol->ID;
		auto& Type = Symbol->VarType;

		auto V0 = IROperand::AsLocation(_Builder.GetLastField());
		_Builder.Build_Assign(IROperand::AsReadVarable(sybId));
		BindTypeToLastIR(Type);

		auto V1 = IROperand::AsLocation(_Builder.GetLastField());
		BindTypeToLastIR(Type);

		

#define Set_CompoundU(x) \
			switch (node.CompoundOp->Type) \
			{ \
			case TokenType::CompoundAdd: \
				_Builder.MakeAdd##x(V0, V1); \
				break; \
			case TokenType::CompoundSub:\
				_Builder.MakeSub##x(V0, V1); \
				break; \
			case TokenType::CompoundMult:\
				_Builder.MakeUMult##x(V0, V1);\
			    break; \
			case TokenType::CompoundDiv:\
				_Builder.MakeUDiv##x(V0, V1);\
			    break; \
			default:\
				throw std::exception("Bad Op"); \
				break; \
			}\

#define Set_CompoundS(x) \
		switch (node.CompoundOp->Type) \
		{ \
		case TokenType::CompoundAdd: \
			_Builder.MakeAdd##x(V0, V1); \
			break; \
		case TokenType::CompoundSub:\
			_Builder.MakeSub##x(V0, V1); \
			break; \
		case TokenType::CompoundMult:\
			_Builder.MakeSMult##x(V0, V1);\
			break; \
			case TokenType::CompoundDiv:\
			_Builder.MakeSDiv##x(V0, V1);\
			break; \
		default:\
			throw std::exception("Bad Op"); \
			break; \
		}\


			switch (Type._Type)
			{
			case TypesEnum::uInt8:
			{
				Set_CompoundU(8);
			};
			break;
			case TypesEnum::uInt16:
			{
				Set_CompoundU(16);
			};
			break;
			case TypesEnum::uInt32:
			{
				Set_CompoundU(32);
			};
			break;
			case TypesEnum::uInt64:
			{
				Set_CompoundU(64);
			};
			break;

			case TypesEnum::sInt8:
			{
				Set_CompoundS(8);
			};
			break;
			case TypesEnum::sInt16:
			{
				Set_CompoundS(16);
			};
			break;
			case TypesEnum::sInt32:
			{
				Set_CompoundS(32);
			};
			break;
			case TypesEnum::sInt64:
			{
				Set_CompoundS(64);
			};
			break;

			case TypesEnum::uIntPtr:
			{
				switch (node.CompoundOp->Type)
				{
				case TokenType::CompoundAdd:
					Build_Add_uIntPtr(V0, V1);
					break;
				case TokenType::CompoundSub:
					Build_Sub_uIntPtr(V0, V1);
					break;
				case TokenType::CompoundMult:
					Build_Mult_uIntPtr(V0, V1);
					break;
				case TokenType::CompoundDiv:
					Build_Div_uIntPtr(V0, V1);
					break;
				}
			};
			break;

			case TypesEnum::sIntPtr:
			{
				switch (node.CompoundOp->Type)
				{
				case TokenType::CompoundAdd:
					Build_Add_sIntPtr(V0, V1);
					break;
				case TokenType::CompoundSub:
					Build_Sub_sIntPtr(V0, V1);
					break;
				case TokenType::CompoundMult:
					Build_Mult_sIntPtr(V0, V1);
					break;
				case TokenType::CompoundDiv:
					Build_Div_sIntPtr(V0, V1);
					break;
				}
			};
			break;



			break;

			default:
				throw std::exception("Bad Op");
				break;
			}


		auto Op = IROperand::AsLocation(_Builder.GetLastField());
		auto NewOp = IROperand::AsVarable(sybId);
		_Builder.Build_Assign(NewOp, Op);
		BindTypeToLastIR(Symbol->VarType);
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
	
	if (passtype == PassType::BuidCode)
	{
		BindTypeToLastIR(LastExpressionType);
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
			NumberliteralNode* num = NumberliteralNode::As(node.Value.get());
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

			TypesEnum NewEx = lookT._Type == TypesEnum::Var || IsIntType(lookT) ? lookT._Type : TypesEnum::sInt32;


			LastExpressionType.SetType(NewEx);
			LastLookedAtToken = num->Token;
		}
		break;
		case NodeType::BoolliteralNode:
		{
			BoolliteralNode* num = BoolliteralNode::As(node.Value.get());
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
			ReadVariableNode* nod = ReadVariableNode::As(node.Value.get());
			OnReadVariable(*nod);
		}
		break;
		case NodeType::AnonymousObjectConstructorNode:
		{
			AnonymousObjectConstructorNode* nod = AnonymousObjectConstructorNode::As(node.Value.get());


			OnAnonymousObjectConstructor(nod);
		}
		break;
		case NodeType::FuncCallNode:
		{
			OnFuncCallNode(*FuncCallNode::As(node.Value.get()));//LastExpressionType was set by OnFuncCall
		}
		break;
		case NodeType::SizeofExpresionNode:
		{
			SizeofExpresionNode* nod = SizeofExpresionNode::As(node.Value.get());

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
			NewExpresionNode* nod = NewExpresionNode::As(node.Value.get());
			TypeSymbol Type;
			Convert(nod->Type, Type);
			bool IsArray = nod->Arrayexpression.Value.get();

			

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

					
					OnExpressionTypeNode(nod->Arrayexpression.Value.get());
				
					auto Ex0 = _LastExpressionField;	
					DoImplicitConversion(IROperand::AsLocation(_Builder.GetLastField()), LastExpressionType, UintptrType);

					Ex0 = _LastExpressionField;

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

					OnExpressionTypeNode(nod->Arrayexpression.Value.get());
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

void SystematicAnalysis::OnAnonymousObjectConstructor(AnonymousObjectConstructorNode*& nod)
{
	auto& Type = Get_LookingForType();
	if (Type.IsnotAn(TypesEnum::Var))//function who called this can deal with var
	{
		

		if (passtype == PassType::FixedTypes)
		{
			auto Func = GetFunc(Type, nod->Fields);
			FuncToSyboID[nod] = Func;
		}
		else if (passtype == PassType::BuidCode)
		{
			auto Func = FuncToSyboID.at(nod);




			String B = ToString(Type);
			Token T;
			T.Type = TokenType::Name;
			T.Value._String = B;


			ScopedNameNode Tep;
			ScopedName V;
			V.token = &T;

			Tep.ScopedName.push_back(std::move(V));

			DoFuncCall(Func, Tep, nod->Fields);


		}
	}

	LastExpressionType = Type;
}

void SystematicAnalysis::OnReadVariable(const ReadVariableNode& nod)
{
	GetMemberTypeSymbolFromVar_t V;

	if (!GetMemberTypeSymbolFromVar(nod.VariableName, V))
	{
		return;
	}
	

	
	auto Str = nod.VariableName.ScopedName.front().token->Value._String;
	auto Symbol = GetSymbol(Str,SymbolType::Varable_t);
	auto Token = nod.VariableName.ScopedName.back().token;
	

	
	auto Info = LogTryReadVar(Str, Token, Symbol);
	if (Info.CantFindVar)
	{
		return;
	}
	if (Info.VarIsInvalid)
	{
		goto SetExpressionInfo;
	}

	SymbolID sybId = Symbol->ID;
	if (passtype == PassType::BuidCode)
	{
		auto& LookForT = Get_LookingForType();
		//if (LookForT.IsLocationValue())
		{
			//_Builder.Build_Assign(IROperand::AsPointer(sybId));
		}
		//else
		{
			_Builder.Build_Assign(IROperand::AsReadVarable(sybId),V.Offset);

			BindTypeToLastIR(V.Type);

		}
		_LastExpressionField = _Builder.GetLastField();
	}

SetExpressionInfo:
	LastExpressionType = V.Type;
	LastLookedAtToken = Token;
}

void SystematicAnalysis::BindTypeToLastIR(TypeSymbol& Type)
{

	auto& V2 = _Builder.GetLast_IR();
	V2.InfoType = std::make_unique<TypeSymbol>(Type);
}

void SystematicAnalysis::OnExpressionNode(const BinaryExpressionNode& node)
{
	OnExpressionTypeNode(node.Value1.Value.get());
	auto Ex0 = _LastExpressionField;
	auto Ex0Type = LastExpressionType;
	OnExpressionTypeNode(node.Value0.Value.get());
	auto Ex1 = _LastExpressionField;
	auto Ex1Type = LastExpressionType;

	if (passtype == PassType::FixedTypes)
	{
		auto BinaryOp = node.BinaryOp;
	
		if (!HasBinaryOverLoadWith(Ex0Type, BinaryOp->Type, Ex1Type))
		{
			LogCantFindBinaryOpForTypes(BinaryOp, Ex0Type, Ex1Type);
		}
	}

	if (passtype == PassType::BuidCode)
	{
		auto Op0 = IROperand::AsLocation(Ex0);
		auto Op1 = IROperand::AsLocation(Ex1);

		auto& Type = Ex0Type;
		auto Op = node.BinaryOp->Type;

		#define BindaryBuildU(x) switch (Op) \
		{\
		case TokenType::plus:_Builder.MakeAdd##x(Op0, Op1);\
			break;\
		case TokenType::minus:_Builder.MakeSub##x(Op0, Op1);\
			break;\
		case TokenType::star:_Builder.MakeUMult##x(Op0, Op1); \
			break; \
		case TokenType::forwardslash:_Builder.MakeUDiv##x(Op0, Op1); \
			break; \
		default:\
			throw std::exception("not added");\
			break;\
		}\

	
		#define BindaryBuildS(x) switch (Op) \
			{\
			case TokenType::plus:_Builder.MakeAdd##x(Op0, Op1); \
				break; \
			case TokenType::minus:_Builder.MakeSub##x(Op0, Op1); \
				break; \
			case TokenType::star:_Builder.MakeSMult##x(Op0, Op1); \
			break; \
				case TokenType::forwardslash:_Builder.MakeSDiv##x(Op0, Op1); \
			break; \
			default:\
				throw std::exception("not added"); \
				break; \
			}\


			switch (Type._Type)
			{
			case TypesEnum::uInt8:BindaryBuildU(8);break;
			case TypesEnum::uInt16:BindaryBuildU(16); break;
			case TypesEnum::uInt32:BindaryBuildU(32); break;
			case TypesEnum::uInt64:BindaryBuildU(64); break;


			case TypesEnum::sInt8:BindaryBuildS(8); break;
			case TypesEnum::sInt16:BindaryBuildS(16); break;
			case TypesEnum::sInt32:BindaryBuildS(32); break;
			case TypesEnum::sInt64:BindaryBuildS(64); break;
			default:
				break;
			}

		_LastExpressionField = _Builder.GetLastField();
	}
}
void SystematicAnalysis::OnExpressionNode(const CastNode& node)
{
	TypeSymbol ToTypeAs;
	Convert(node.ToType, ToTypeAs);
	LookingForTypes.push(ToTypeAs);
	
	
	OnExpressionTypeNode(node.Expression.Value.get());
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

	if (passtype == PassType::BuidCode)
	{
		DoExplicitlConversion(IROperand::AsLocation(_Builder.GetLastField()), LastExpressionType, ToTypeAs);

		LastExpressionType = ToTypeAs;
	}
	LookingForTypes.pop();
}
void SystematicAnalysis::OnFuncCallNode(const FuncCallNode& node)
{
	if (passtype == PassType::FixedTypes)
	{
		auto Info = GetFunc(node.FuncName, node.Generics, node.Parameters, Get_LookingForType());

		DoFuncCall(Info, node.FuncName, node.Parameters);
		FuncToSyboID[&node] = Info;
	}
	else if (passtype == PassType::BuidCode)
	{
		auto SybID = FuncToSyboID.at(&node);
		DoFuncCall(SybID, node.FuncName, node.Parameters);
	}
}
void SystematicAnalysis::OnDropStatementNode(const DropStatementNode& node)
{
	OnExpressionTypeNode(node.expression.Value.get());
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

	for (auto& Item : lib.Get_Assembly().Classes)
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
void SystematicAnalysis::AddClass_tToAssemblyInfo(const ClassInfo* Class)
{
	ClassData& V = _Lib.Get_Assembly().AddClass((String)Class->Get_Name(), Class->FullName);
	auto& VClass = V._Class;

	VClass.Size = 0;

	
	for (const auto& node : Class->Fields)
	{
		auto& Item = VClass.Fields.emplace_back();

		UAddress Size;
		GetSize(node.Type, Size);
	
		Item.Name =ScopeHelper::GetNameFromFullName(node.Name);
		Item.FullNameType = ToString(node.Type);
		Item.offset = VClass.Size;
		VClass.Size += Size;
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

	if (AreTheSame(TypeA, TypeB))
	{	
		if (IsIntType(TypeA))
		{
			return true;
		}
	}
	return false;
}
bool SystematicAnalysis::HasCompoundOverLoadWith(const TypeSymbol& TypeA, TokenType BinaryOp, const TypeSymbol& TypeB)
{
	if (AreTheSame(TypeA, TypeB))
	{
		if (IsIntType(TypeA))
		{
			return true;
		}
	}
	return false;
}
bool SystematicAnalysis::HasPostfixOverLoadWith(const TypeSymbol& TypeA, TokenType BinaryOp)
{

	if (IsIntType(TypeA))
	{
		return true;
	}

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


	case TypesEnum::uInt8:r = Uint8TypeName;	break;
	case TypesEnum::uInt16:r = Uint16TypeName;	break;
	case TypesEnum::uInt32:r = Uint32TypeName;	break;
	case TypesEnum::uInt64:r = Uint64TypeName;	break;


	case TypesEnum::sInt8:r = Sint8TypeName;	break;
	case TypesEnum::sInt16:r = Sint16TypeName;	break;
	case TypesEnum::sInt32:r = Sint32TypeName;	break;
	case TypesEnum::sInt64:r = Sint64TypeName;	break;

	case TypesEnum::uIntPtr:r = UintPtrTypeName;	break;
	case TypesEnum::sIntPtr:r = SintPtrTypeName;	break;

	case TypesEnum::Bool:r = boolTypeName;	break;
	case TypesEnum::Char:r = CharTypeName;	break;
	case TypesEnum::CustomType:
	{
		auto& Syb = _Table.GetSymbol(Type._CustomTypeSymbol);
		if (Syb.Type == SymbolType::Func
			|| Syb.Type == SymbolType::GenericFunc)
		{
			r = ToString(Syb.VarType);
		}
		else if (Syb.Type == SymbolType::Type_alias)
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
	case TokenType::Void:
		Out.SetType(TypesEnum::Void);
		break;
	case TokenType::KeyWorld_This: 
	{

		if (_ClassStack.size()) 
		{
			Symbol* SybV = GetSymbol(_ClassStack.top());
			Out = SybV->VarType;
		}
		else
		{
			LogCantUseThisHere(V.Name.Token);
			Out.SetType(TypesEnum::uInt8);
		}
	}
		break;
	case TokenType::Name: 
	{
		auto Name = V.Name.AsStringView();
		Symbol* SybV;
		if (V.Generic.Values.size())
		{
			SybV = GetSymbol(Name, SymbolType::Generic_class);

			ClassInfo* CInfo = (ClassInfo*)SybV->Info.get();

			if (CInfo->_Generic.size() != V.Generic.Values.size())
			{
				LogCanIncorrectGenericCount(V.Name.Token, Name, V.Generic.Values.size(), CInfo->_Generic.size());
				return;
			}

			
			auto GenericInput = std::make_unique<Vector<TypeSymbol>>();//pointer must be unique so it cant be on the stack
			for (size_t i = 0; i < CInfo->_Generic.size(); i++)
			{
				const auto& Tnode = V.Generic.Values[i];
				TypeSymbol Type; 
				Convert(Tnode, Type);
				
				GenericInput->push_back(Type);
			}
			
			String NewName = GetGenericFuncName(SybV, *GenericInput);
			auto FuncIsMade = GetSymbol(NewName, SymbolType::Func);
			if (!FuncIsMade)
			{
				GenericTypeInstantiate(SybV, *GenericInput);

				TepFuncs.push_back({std::move(GenericInput)});
			}

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
			if (SybV->Type == SymbolType::Type_alias)
			{
				Out = SybV->VarType;
			}
			else
			{
				Out.SetType(SybV->ID);
			}
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
bool SystematicAnalysis::DoImplicitConversion(IROperand Ex, const TypeSymbol ExType, const TypeSymbol& ToType)
{

	if (AreTheSame(ExType, ToType))
	{
		return true;
	}
	return false;
}
void SystematicAnalysis::DoExplicitlConversion(IROperand Ex, const TypeSymbol ExType, const TypeSymbol& ToType)
{
	if (!DoImplicitConversion(Ex, ExType, ToType))
	{
		//DO Stuff
	}
}
bool SystematicAnalysis::IsSIntType(const TypeSymbol& TypeToCheck)
{
	return
		TypeToCheck._Type == TypesEnum::sInt8 ||
		TypeToCheck._Type == TypesEnum::sInt16 ||
		TypeToCheck._Type == TypesEnum::sInt32 ||
		TypeToCheck._Type == TypesEnum::sInt64 ||
		TypeToCheck._Type == TypesEnum::sIntPtr;
}
bool SystematicAnalysis::IsUIntType(const TypeSymbol& TypeToCheck)
{
	return
		TypeToCheck._Type == TypesEnum::uInt8 ||
		TypeToCheck._Type == TypesEnum::uInt16 ||
		TypeToCheck._Type == TypesEnum::uInt32 ||
		TypeToCheck._Type == TypesEnum::uInt64 ||
		TypeToCheck._Type == TypesEnum::uIntPtr ;
}
bool SystematicAnalysis::IsPrimitive(const TypeSymbol& TypeToCheck)
{
	return false;
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
bool SystematicAnalysis::GetSize(const TypeSymbol& Type, UAddress& OutSize)
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

	case TypesEnum::CustomType:
	{
		Symbol& V = _Table.GetSymbol(Type._CustomTypeSymbol);
	
		if (V.Type == SymbolType::Type_class)
		{

			ClassInfo* Vp = (ClassInfo*)V.Info.get();
			
			if (!Vp->SizeInitialized)
			{
				Vp->Size = 0;
				auto& classNode = *(ClassNode*)V.NodePtr;
				if (!IsDependencies(Vp))
				{
					OnClassNode(classNode);//update class fields
				}
				else
				{
					LogTypeDependencyCycle(classNode.ClassName.Token,Vp);
				}
			}
			
			
			OutSize = Vp->Size;
			return true;
		}
		else if(V.Type == SymbolType::Type_alias)
		{
			return GetSize(V.VarType,OutSize);
		}
		else
		{
			throw std::exception("not added");
		}
	}
		
	default:
		OutSize = 0;
		return false;
	}
	


}
bool SystematicAnalysis::GetOffset(const ClassInfo& Type, const FieldInfo* Field, UAddress& OutOffset)
{
	UAddress offset = 0;
	

	for (auto& Item : Type.Fields)
	{
		if (&Item == Field)
		{
			OutOffset = offset;
			return true;
		}
		UAddress FieldSize = 0;
		GetSize(Item.Type, FieldSize);
		offset += FieldSize;
	}

	OutOffset = offset;
	return false;
}
FuncInfo* SystematicAnalysis::GetFunc(const TypeSymbol& Name, const ValueParametersNode& Pars)
{
	String B =ToString(Name);
	Token T;
	T.Type = TokenType::Name;
	T.Value._String = B;


	ScopedNameNode Tep;
	ScopedName V;
	V.token = &T; 

	Tep.ScopedName.push_back(std::move(V));

	return GetFunc(Tep, Pars, Name);
}
void SystematicAnalysis::DoFuncCall(const FuncInfo* Func, const ScopedNameNode& Name, const ValueParametersNode& Pars)
{
	{
#define PrimitiveTypeCall(FullName,TypeEnum,DefaultValue) if (ScopedName == FullName) \
		{\
			TypeSymbol iNfo;\
			iNfo.SetType(TypeEnum);\
			if (Pars._Nodes.size())\
			{\
				LookingForTypes.push(iNfo);\
				auto& Item = Pars._Nodes[0];\
				OnExpressionTypeNode(Item.get());\
				DoImplicitConversion(IROperand::AsLocation(_Builder.GetLastField()), LastExpressionType, iNfo);\
				LookingForTypes.pop();\
			}\
			else\
			{\
				LastExpressionType = iNfo;\
				DefaultValue;\
			}\
			return;\
		}\

		auto ScopedName = GetScopedNameAsString(Name);
		auto SymbolsV = GetSymbol(ScopedName, SymbolType::Any);
		if (SymbolsV  && SymbolsV->Type == SymbolType::Type_alias)
		{
			ScopedName = ToString(SymbolsV->VarType);
		}

		PrimitiveTypeCall(Uint8TypeName, TypesEnum::uInt8, _Builder.Build_Assign(IROperand::AsInt8((UInt8)0));)
		else PrimitiveTypeCall(Uint16TypeName, TypesEnum::uInt16, _Builder.Build_Assign(IROperand::AsInt16((UInt16)0)))
		else PrimitiveTypeCall(Uint32TypeName, TypesEnum::uInt32, _Builder.Build_Assign(IROperand::AsInt32((UInt32)0)))
		else PrimitiveTypeCall(Uint16TypeName, TypesEnum::uInt64, _Builder.Build_Assign(IROperand::AsInt64((UInt64)0)))

		else PrimitiveTypeCall(Sint8TypeName, TypesEnum::sInt8, _Builder.Build_Assign(IROperand::AsInt8((Int8)0));)
		else PrimitiveTypeCall(Sint16TypeName, TypesEnum::sInt16, _Builder.Build_Assign(IROperand::AsInt16((Int16)0)))
		else PrimitiveTypeCall(Sint32TypeName, TypesEnum::sInt32, _Builder.Build_Assign(IROperand::AsInt32((Int32)0)))
		else PrimitiveTypeCall(Sint16TypeName, TypesEnum::sInt64, _Builder.Build_Assign(IROperand::AsInt64((Int64)0)))

		else PrimitiveTypeCall(boolTypeName, TypesEnum::Bool, _Builder.Build_Assign(IROperand::AsInt8((UInt8)false));)
		else PrimitiveTypeCall(CharTypeName, TypesEnum::Char, _Builder.Build_Assign(IROperand::AsInt8((UInt8)'\0')))
	}
	if (Func == nullptr)
	{
		return;
	}

	for (size_t i = 0; i < Pars._Nodes.size(); i++)
	{
		auto& Item = Pars._Nodes[i];
		auto& FuncParInfo = Func->Pars[i];


		
		LookingForTypes.push(FuncParInfo);

		OnExpressionTypeNode(Item.get());
		DoImplicitConversion(IROperand::AsLocation(_Builder.GetLastField()), LastExpressionType, FuncParInfo);

		_Builder.Build_PassLastAsParameter();

		LookingForTypes.pop();
	}
	_Builder.Build_FuncCall(GetSymbol(Func)->ID);

	LastExpressionType = Func->Ret;
}
FuncInfo* SystematicAnalysis::GetFunc(const ScopedNameNode& Name, const UseGenericsNode& Generics, const ValueParametersNode& Pars, TypeSymbol Ret)
{
	auto ScopedName = GetScopedNameAsString(Name);
	LastLookedAtToken = Name.ScopedName.back().token;
	{

		auto SymbolsV = GetSymbol(ScopedName, SymbolType::Any);

		if (SymbolsV && SymbolsV->Type == SymbolType::Type_alias)
		{
			ScopedName = ToString(SymbolsV->VarType);
		}
	
		if (ScopedName == Uint8TypeName) 
		{

			if (Pars._Nodes.size() > 1)
			{
				LogCanIncorrectParCount(Name.ScopedName.back().token, ScopedName, Pars._Nodes.size(), 1);
			}

			return nullptr;

		}

		
		if (ScopedName == Uint8TypeName ||
			ScopedName == Uint16TypeName ||
			ScopedName == Uint32TypeName ||
			ScopedName == Uint64TypeName ||
			ScopedName == Sint8TypeName ||
			ScopedName == Sint16TypeName ||
			ScopedName == Sint32TypeName ||
			ScopedName == Sint64TypeName || 
			ScopedName == SintPtrTypeName ||
			ScopedName == UintPtrTypeName ||
			ScopedName == boolTypeName ||
			ScopedName == CharTypeName)
		{
			
			if (Pars._Nodes.size() > 1)
			{
				LogCanIncorrectParCount(Name.ScopedName.back().token, ScopedName, Pars._Nodes.size(), 1);
			}
			return nullptr;
		}
	}
	
	auto& Symbols = _Table.GetSymbolsWithName(ScopedName, SymbolType::Any);
	FuncInfo* r = nullptr;

	auto& RetType = Get_LookingForType();
	bool RetIsSet = RetType.IsnotAn(TypesEnum::Var);

	Vector<TypeSymbol> ValueTypes;
	ValueTypes.resize(Pars._Nodes.size());

	TypeSymbol NullSymbol;
	NullSymbol.SetType(TypesEnum::Any);

	for (size_t i = 0; i < Pars._Nodes.size(); i++)
	{
		auto& Item = Pars._Nodes[i];
		auto& ValueItem = ValueTypes[i];

		LookingForTypes.push(NullSymbol);

		OnExpressionTypeNode(Item.get());
		ValueItem = LastExpressionType;

		LookingForTypes.pop();
	}
	for (auto& Item : Symbols)
	{
		if (Item->Type == SymbolType::Func)
		{
			FuncInfo* Info = (FuncInfo*)Item->Info.get();
			if (RetIsSet)
			{
				if (!AreTheSame(Info->Ret, RetType))
				{
					continue;
				}
			}

			if (Info->Pars.size() != ValueTypes.size())
			{
				continue;
			}


			for (size_t i = 0; i < Info->Pars.size(); i++)
			{
				auto& Item = Info->Pars[i];
				auto& Item2 = ValueTypes[i];

				if (!CanBeImplicitConverted(Item2, Item))
				{
					continue;
				}
			}
			r = Info;
			break;
		}
		else if (Item->Type == SymbolType::GenericFunc)
		{
			FuncInfo* Info = (FuncInfo*)Item->Info.get();

			if (Info->Pars.size() != ValueTypes.size())
			{
				continue;
			}

			//I have felling this can be optimized
			Vector<bool> HasBenAdded;
			HasBenAdded.resize(Info->_Generic.size());


			Vector<TypeSymbol> GenericInput;
			for (size_t i = 0; i < Generics.Values.size(); i++)
			{
				auto& Item = Generics.Values[i];
				Convert(Item, GenericInput.emplace_back());
				HasBenAdded[i] = true;
			}



			for (size_t i = 0; i < ValueTypes.size(); i++)
			{
				auto& Item = ValueTypes[i];
				auto& Par = Info->Pars[i];


				for (size_t i2 = 0; i2 < Info->_Generic.size(); i2++)
				{
					auto& V3 = Info->_Generic[i2];
					if (V3 == Par._CustomTypeSymbol)
					{
						if (HasBenAdded[i2] == false)
						{
							GenericInput.push_back(Item);
							HasBenAdded[i2] = true;
						}
						break;
					}
				}
			}


			if (RetIsSet)
			{
				for (size_t i2 = 0; i2 < Info->_Generic.size(); i2++)
				{
					auto& V3 = Info->_Generic[i2];
					if (V3 == Info->Ret._CustomTypeSymbol)
					{
						if (HasBenAdded[i2] == false)
						{
							GenericInput.push_back(Ret);
							HasBenAdded[i2] = true;
						}
						break;
					}
				}

			}


			{
				



				auto FuncSym = GetSymbol(Info);
				String NewName = GetGenericFuncName(FuncSym, GenericInput);
				auto FuncIsMade = GetSymbol(NewName,SymbolType::Func);
				
				
				
				if (!FuncIsMade)
				{
					auto Pointer = std::make_unique<Vector<TypeSymbol>>(std::move(GenericInput));
					//pointer must be unique so it cant be on the stack

					GenericFuncInstantiate(FuncSym, *Pointer);

					TepFuncs.push_back({ std::move(Pointer) });//keep pointer 


					FuncSym = GetSymbol(NewName, SymbolType::Func);
					r = (FuncInfo*)FuncSym->Info.get();
				}
				else
				{
					r = (FuncInfo*)FuncIsMade->Info.get();
				}


				


			
			}
			break;
		}
	}
	if (r == nullptr)
	{
		LogCantFindFuncError(Name.ScopedName.back().token, ScopedName, {}, ValueTypes,RetType);
	}
	return r;
}
String SystematicAnalysis::GetGenericFuncName(Symbol* Func, const Vector<TypeSymbol>& Type)
{
	String NewName = Func->FullName + "<";
	for (auto& Item : Type)
	{
		NewName += ToString(Item);
		if (&Type.back() != &Item)
		{
			NewName += ",";
		}
	}
	NewName += ">";
	return NewName;
}
void SystematicAnalysis::GenericFuncInstantiate(Symbol* Func, const Vector<TypeSymbol>& GenericInput)
{
	String NewName = GetGenericFuncName(Func, GenericInput);
	
	FuncNode& FuncBase = *(FuncNode*)Func->NodePtr;

	GenericFuncInfo Info;
	Info.GenericFuncName = NewName;
	Info.GenericInput = &GenericInput;
	Info.NodeTarget = Func->NodePtr;
	GenericFuncName.push(Info);

	

	auto OldPass = passtype;
	auto OldScope = _Table._Scope.ThisScope;
	_Table._Scope.ThisScope.clear();



	passtype = PassType::GetTypes;
	OnFuncNode(FuncBase);

	if (!_ErrorsOutput->Has_Errors())
	{
		passtype = PassType::FixedTypes;
		OnFuncNode(FuncBase);
	}
	if (!_ErrorsOutput->Has_Errors())
	{
		passtype = PassType::BuidCode;
		OnFuncNode(FuncBase);
	}


	GenericFuncName.pop();
	//
	_Table._Scope.ThisScope = OldScope;
	passtype = OldPass;
}
void SystematicAnalysis::GenericTypeInstantiate(Symbol* Class, const Vector<TypeSymbol>& Type)
{
	String NewName = GetGenericFuncName(Class, Type);
	const ClassNode* node = (const ClassNode*)Class->NodePtr;

	GenericFuncInfo Info;
	Info.GenericFuncName = NewName;
	Info.GenericInput = &Type;
	Info.NodeTarget = Class->NodePtr;
	GenericFuncName.push(Info);


	auto OldScope = _Table._Scope.ThisScope;
	auto Oldpasstype = passtype;
	_Table._Scope.ThisScope.clear();

	


	passtype = PassType::GetTypes;
	OnClassNode(*node);

	if (!_ErrorsOutput->Has_Errors()) {
		passtype = PassType::FixedTypes;
		OnClassNode(*node);
	}

	if (!_ErrorsOutput->Has_Errors()) {
		passtype = PassType::BuidCode;
		OnClassNode(*node);
	}

	GenericFuncName.pop();
	//
	_Table._Scope.ThisScope = OldScope;
	passtype = Oldpasstype;
}

void SystematicAnalysis::CheckVarWritingErrors(Symbol* Symbol, const Token* Token, String_view& Name)
{
	if (Symbol->VarType.Isimmutable())
	{
		LogCantModifyiMutableError(Token, Name);
	}
}

void SystematicAnalysis::LogCantCastImplicitTypes(const Token* Token, TypeSymbol& Ex1Type, TypeSymbol& UintptrType)
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
void SystematicAnalysis::LogReadingFromInvaidVariable(const Token* Token, String_view Str)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "the variable named '" + (String)Str + "'" + " cant be read from you.can not read an invaid variable");
}
void SystematicAnalysis::LogCantFindVarError(const Token* Token, String_view Str)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Cant find Variable Named '" + (String)Str + "'");
}
void SystematicAnalysis::LogCantFindVarMemberError(const Token* Token, String_view Str, const TypeSymbol& OnType)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Cant find Member Named '" + (String)Str + "' on type '" + ToString(OnType) +"'");
}

void SystematicAnalysis::LogCantFindCompoundOpForTypes(const Token* BinaryOp, TypeSymbol& Ex0Type, TypeSymbol& Ex1Type)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidType, BinaryOp->OnLine, BinaryOp->OnPos,
		"The type '" + ToString(Ex0Type) + "'" + " cant be '"
		+ ToString(BinaryOp->Type) + "' with '" + ToString(Ex0Type) + "'");
}

void SystematicAnalysis::LogCantFindPostfixOpForTypes(const Token* BinaryOp, TypeSymbol& Ex0Type)
{
		_ErrorsOutput->AddError(ErrorCodes::InValidType, BinaryOp->OnLine, BinaryOp->OnPos,
			"The type '" + ToString(Ex0Type) + "'" + " cant be '"
			+ ToString(BinaryOp->Type) + "'");
}
void SystematicAnalysis::LogCantFindBinaryOpForTypes(const Token* BinaryOp, TypeSymbol& Ex0Type, TypeSymbol& Ex1Type)
{
	
	_ErrorsOutput->AddError(ErrorCodes::InValidType, BinaryOp->OnLine, BinaryOp->OnPos,
		"The type '" + ToString(Ex0Type) + "'" + " cant be '"
		+ ToString(BinaryOp->Type) + "' with '" + ToString(Ex0Type) + "'");
}
void SystematicAnalysis::ExpressionMustbeAnLocationValueError(const Token* Token, TypeSymbol& Ex0Type)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos,
		"expression must be an Location not an Value'" + ToString(Ex0Type) + "'");
}
void SystematicAnalysis::YouMustReturnSomethingError(const Token* Token)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos
		, "you must return something");
}
void SystematicAnalysis::CantguessVarTypeError(const Token* Token)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos
		, "cant guess 'var' type");
}
void SystematicAnalysis::CantUseThisKeyWordHereError(const Token* NameToken)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidType, NameToken->OnLine, NameToken->OnPos, "cant use this here");
}
void SystematicAnalysis::CantgussTypesTheresnoassignment(const Token* Token)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos
		, "cant guess type theres no '=' [expression]");
}
void SystematicAnalysis::LogCantCastExplicityTypes(const Token* Token, TypeSymbol& Ex0Type, TypeSymbol& ToTypeAs)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Cant cast Type '" + ToString(Ex0Type) + " to '" + ToString(ToTypeAs) + "'");
}

SystematicAnalysis::ReadVarErrorCheck_t SystematicAnalysis::LogTryReadVar(String_view VarName, const Token* Token, const Symbol* Syb)
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

void SystematicAnalysis::LogCantFindFuncError(const Token* Token, String_view FuncName,
	const Vector<TypeSymbol>& Generics,
	const Vector<TypeSymbol>& WithTypes,
	const TypeSymbol& RetType)
{
	String Text = "Cant Find function '" + (String)FuncName;


	if (Generics.size())
	{
		Text += "<";

		for (auto& Item : Generics)
		{
			Text += ToString(Item);
			if (&Item != &Generics.back())
			{
				Text += ',';
			}
		}

		Text += ">";

	}

	//if (WithTypes.size()) 
	{
		Text += "[";

		for (auto& Item : WithTypes)
		{
			Text += ToString(Item);
			if (&Item != &WithTypes.back())
			{
				Text += ',';
			}

			
		}

		Text += "]";
	}

	if (RetType.IsnotAn(TypesEnum::Null) && RetType.IsnotAn(TypesEnum::Var))
	{
		Text += " -> " + ToString(RetType);
	}
	Text += "'";

	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, Text);
}
void SystematicAnalysis::LogCantModifyiMutableError(const Token* Token, String_view Name)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Cant modify '" + (String)Name + "' it's immutable");
}
void SystematicAnalysis::LogCantFindTypeError(const Token* Token, String_view Name)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Cant Find Type '" + (String)Name + "'");
}
void SystematicAnalysis::LogTypeDependencyCycle(const Token* Token, const ClassInfo* Value)
{
	String Msg = "Class Dependency Cycle On Type '" + (String)Value->FullName + "' <- ";

	for (int i = ClassDependencies.size() - 1; i >= 0; i--)
	{
		auto& Item = ClassDependencies[i];
	
		Msg += "'" + (String)Item->FullName + "'";
		if (&Item != &ClassDependencies.front())
		{
			Msg += " <- ";
		}
	}
	Msg += ".";
	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, Msg);

	
}
void SystematicAnalysis::LogCantUseThisHere(const Token* Token)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		,"Cant Find Type for '" + (String)StringHelper::ToString(TokenType::KeyWorld_This) + "'");
}
void SystematicAnalysis::LogCanIncorrectParCount(const Token* Token, String_view FuncName, size_t Count, size_t FuncCount)
{
	String Msg;

	if (Count > FuncCount)
	{
		Msg = "Too Many parameters for function '" + (String)FuncName + "'";
	}
	else
	{
		Msg = "too little parameters for function '" + (String)FuncName + "'";
	}

	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, Msg);
}
void SystematicAnalysis::LogCanIncorrectGenericCount(const Token* Token, String_view FuncName, size_t Count, size_t FuncCount)
{
	String Msg;

	if (Count > FuncCount)
	{
		Msg = "Too Many Generic types for instantiation'" + (String)FuncName + "'";
	}
	else
	{
		Msg = "too little Generic types for instantiation'" + (String)FuncName + "'";
	}

	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, Msg);
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


