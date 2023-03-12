#include "SystematicAnalysis.hpp"
#include "UCodeLang/Compliation/Helpers/KeyWords.hpp"

#include "UCodeLang/Compliation/Helpers/InstructionBuilder.hpp"
#include "UCodeLang/Compliation/Back/UCodeBackEnd/UCodeBackEnd.hpp"
#include "UCodeLang/Compliation/Helpers/ParseHelper.hpp"
UCodeLangFrontStart

void SystematicAnalysis::Reset()
{
	auto ErrorsOutput = _ErrorsOutput;
	auto Settings = _Settings;

	this->~SystematicAnalysis();
	new (this)SystematicAnalysis;
	
	this->_ErrorsOutput = ErrorsOutput;
	this->_Settings = Settings;
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

	if (!_ErrorsOutput->Has_Errors()) {

		LoadLibSymbols();
		passtype = PassType::FixedTypes;
		Pass();


		if (!_ErrorsOutput->Has_Errors()) {
			BuildCode();
		}
	};

	_Files = nullptr;
	_Libs = nullptr;

	_Lib._LibType = OutputTypeAsLibType();

	return !_ErrorsOutput->Has_Errors();
}
void SystematicAnalysis::BuildCode()
{
	for (auto& Item : _Table.Symbols)
	{
		switch (Item->Type)
		{
		case SymbolType::Type_class:
		{
			ConveToIRClassIR(*Item);
		}
			break;
		default:
			break;
		}
	}

	passtype = PassType::BuidCode;
	Pass();
}
const FileNode* SystematicAnalysis::Get_FileUseingSybol(Symbol* Syb)
{
	return Syb->_File;
}
void SystematicAnalysis::AddDependencyToCurrentFile(Symbol* Syb)
{
	AddDependencyToCurrentFile(Get_FileUseingSybol(Syb));
}
void SystematicAnalysis::AddDependencyToCurrentFile(const FileNode* file)
{
	auto CurrentFile = LookingAtFile;
	const FileNode_t* CurrentFile_t =(const FileNode_t*)CurrentFile;

	const FileNode_t* file_t = (const FileNode_t*)file;
	if (CurrentFile != file)
	{
		FileNodeData& Data = _FilesData[CurrentFile_t];
		
		for (auto& Item : Data._Dependencys)
		{
			const FileNode_t* Itemfile_t = (const FileNode_t*)Item;
			if (Itemfile_t == file_t)
			{
				return;
			}
		}
		Data._Dependencys.push_back(file_t);

	}
	
}
void SystematicAnalysis::AddDependencyToCurrentFile(const TypeSymbol& type)
{
}
Symbol& SystematicAnalysis::AddSybol(SymbolType type, const String& Name, const String& FullName)
{

	auto& r = _Table.AddSybol(type, Name, FullName);
	r._File = LookingAtFile;
	return r;
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
	
	if (passtype == PassType::GetTypes)
	{
		if (!GetSymbol(String_view(Namespace),SymbolType::Namespace))
		{
			AddSybol(SymbolType::Namespace, Namespace, _Table._Scope.ThisScope);
		}
	}

	
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
void SystematicAnalysis::OnFileNode(const FileNode* File)
{
	LookingAtFile = File;
	_ErrorsOutput->FilePath = File->FileName;
	auto V = _FilesData[File];//add 

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
		case NodeType::DeclareStaticVariableNode:OnDeclareStaticVariableNode(*DeclareStaticVariableNode::As(node.get())); break;
		case NodeType::DeclareThreadVariableNode:OnDeclareThreadVariableNode(*DeclareThreadVariableNode::As(node.get())); break;
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

	auto SybID = GetSymbolID(Node);


	if (passtype == PassType::GetTypes)
	{
		DoSymbolRedefinitionCheck((String_view)_Table._Scope.ThisScope,SymbolType::Type_class, Node.ClassName.Token);
	}

	auto& Syb = passtype == PassType::GetTypes ?
		AddSybol(Isgeneric_t ? SymbolType::Generic_class : SymbolType::Type_class
			, (String)ClassName, _Table._Scope.ThisScope) :
		*GetSymbol(SybID);



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
			auto GenericType = &AddSybol(SymbolType::Type_alias, GenericTypeName,
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

				GenericData Info;
				Info.SybID = ID;
				Info.IsConstantExpression = Item.IsConstantExpression;

				ClassInf->_Generic.push_back(Info);
			}
		}
	}
	else
	{
		ClassInf = Syb.Get_Info<ClassInfo>();
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
			case NodeType::DeclareStaticVariableNode:OnDeclareStaticVariableNode(*DeclareStaticVariableNode::As(node.get())); break;
			case NodeType::DeclareThreadVariableNode:OnDeclareThreadVariableNode(*DeclareThreadVariableNode::As(node.get())); break;
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

	if (IsgenericInstantiation) 
	{
		for (size_t i = 0; i < ScopeHelper::Get_ScopeCount(GenericFuncName.top().GenericFuncName) + 1; i++)
		{
			_Table.RemoveScope();
		}
	}
	else
	{
		_Table.RemoveScope();

	}
	Syb.PassState = passtype;
}
void SystematicAnalysis::OnAliasNode(const AliasNode& node)
{
	const auto& ClassName = node.AliasName.Token->Value._String;
	auto SybID = GetSymbolID(node);

	_Table.AddScope(ClassName);

	if (passtype == PassType::GetTypes)
	{
		DoSymbolRedefinitionCheck((String_view)_Table._Scope.ThisScope, node.IsHardAlias ? SymbolType::Hard_Type_alias : SymbolType::Type_alias, node.AliasName.Token);
	}

	auto& Syb = passtype == PassType::GetTypes ?
		AddSybol(node.IsHardAlias ? SymbolType::Hard_Type_alias : SymbolType::Type_alias, (String)ClassName, _Table._Scope.ThisScope) :
		*GetSymbol(SybID);

	if (passtype == PassType::GetTypes)
	{
		_Table.AddSymbolID(Syb, SybID);

		if (node._Type == AliasType::Type) 
		{
			Convert(node.Type, Syb.VarType);
		}
		else
		{
			
			AliasNode_Func* node_ = (AliasNode_Func*)node._Node.get();
			auto V = new FuncPtrInfo();
			Syb.Info.reset(V);

			V->Pars.resize(node_->Parameters.Parameters.size());

			for (size_t i = 0; i < V->Pars.size(); i++)
			{
				auto& NodePar = node_->Parameters.Parameters[i];
				auto& Par = V->Pars[i];
				Convert(NodePar.Type, Par);
			}

			Convert(node_->ReturnType, V->Ret);

			Syb.VarType.SetType(SybID);
		}
	}
	if (passtype == PassType::FixedTypes)
	{
		if (node._Type == AliasType::Type) 
		{
			Convert(node.Type, Syb.VarType);
		}
		else
		{
			AliasNode_Func* node_ = (AliasNode_Func*)node._Node.get();
			FuncPtrInfo* nodeinfo_ = (FuncPtrInfo*)Syb.Info.get();

			for (size_t i = 0; i < nodeinfo_->Pars.size(); i++)
			{
				auto& NodePar = node_->Parameters.Parameters[i];
				auto& Par = nodeinfo_->Pars[i];
				Convert(NodePar.Type, Par);
			}

			Convert(node_->ReturnType, nodeinfo_->Ret);
		}
	}

	if (passtype == PassType::BuidCode)
	{
		auto& V = _Lib.Get_Assembly().Add_Alias((String)ClassName, _Table._Scope.ThisScope);
		V.Type =ConvertToTypeInfo(Syb.VarType);


		AddDependencyToCurrentFile(Syb.VarType);
	}

	_Table.RemoveScope();

	Syb.PassState = passtype;
}
void SystematicAnalysis::OnUseingNode(const UsingNode& node)
{
	auto& T =node.ScopedName.ScopedName.back().token;
	OnNonAttributeable(T->OnLine, T->OnPos);
	const auto UseingString =GetScopedNameAsString(node.ScopedName);
	_Table.AddUseing(UseingString);

	if (passtype == PassType::FixedTypes)
	{
		if (!GetSymbol(UseingString, SymbolType::Namespace))
		{
			LogCantFindNamespace(T, UseingString);
		}
	}
}
void SystematicAnalysis::OnFuncNode(const FuncNode& node)
{


	bool IsgenericInstantiation = GenericFuncName.size() && GenericFuncName.top().NodeTarget == &node;
	bool IsGenericS = node.Signature.Generic.Values.size();

	auto FuncName = IsgenericInstantiation ? GenericFuncName.top().GenericFuncName
		: node.Signature.Name.AsStringView();
	auto NameToken = node.Signature.Name.Token;


	FuncInfo::FuncType FuncType = FuncInfo::FuncType::NameFunc;

	if (NameToken->Type == TokenType::KeyWorld_new)
	{
		FuncName = ClassConstructorfunc;
		FuncType = FuncInfo::FuncType::New;
		if (_ClassStack.empty())
		{
			CantUseThisKeyWordHereError(NameToken);
		}
	}
	else if (NameToken->Type == TokenType::KeyWorld_Drop)
	{
		FuncName = ClassDestructorFunc;
		FuncType = FuncInfo::FuncType::Drop;
		if (_ClassStack.empty())
		{
			CantUseThisKeyWordHereError(NameToken);
		}
	}



	_Table.AddScope(FuncName);
	auto FullName = _Table._Scope.ThisScope;
	

	

	Symbol* syb;
	SymbolID sybId =GetSymbolID(node);
	

	auto UseingIndex = _Table.GetUseingIndex();

	if (passtype == PassType::GetTypes)
	{
		SymbolType Type = IsGenericS && IsgenericInstantiation == false ?
			SymbolType::GenericFunc : SymbolType::Func;

		

		syb = &AddSybol(Type, (String)FuncName, FullName);
		syb->NodePtr = (void*)&node;//the node will not get update anyway.
		_Table.AddSymbolID(*syb, sybId);

		FuncInfo* newInfo = new FuncInfo();
		newInfo->FullName = FullName;
		newInfo->_FuncType = FuncType;
		
		syb->Info.reset(newInfo);

		auto& GenericList = node.Signature.Generic;	
		
		
		for (size_t i = 0; i < GenericList.Values.size(); i++)
		{
			auto& Item = GenericList.Values[i];

			auto GenericTypeName = Item.AsString();
			auto GenericType = &AddSybol(SymbolType::Type_alias, GenericTypeName, 
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
				
				GenericData Info;
				Info.SybID = ID;
				Info.IsConstantExpression = Item.IsConstantExpression;

				newInfo->_Generic.push_back(Info);
			}
		
		}

		

		auto& RetType = node.Signature.ReturnType.node;
		if (RetType && RetType->Get_Type() == NodeType::AnonymousTypeNode)
		{
			auto NewName = GetFuncAnonymousObjectFullName(FullName);


			SymbolID AnonymousSybID = (SymbolID)RetType.get();
			auto& AnonymousSyb =AddSybol(SymbolType::Type_class, (String)NewName, NewName); 
			
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




			auto GenericType = &AddSybol(SymbolType::ParameterVarable, (String)GenericTypeName,
				_Table._Scope.GetApendedString(GenericTypeName)
			);
			auto ParSybID = (SymbolID)&Item;
			_Table.AddSymbolID(*GenericType, ParSybID);

			if (Item.Name.Token == nullptr)
			{
				newInfo->FrontParIsUnNamed = true;
			}
			newInfo->Pars.push_back(VP);
		}
		
		
		syb = GetSymbol(sybId);//resized _Table
	}
	else
	{
		syb = GetSymbol(sybId);
	}

	//we may jump to this node non linearly
	if (syb->PassState == passtype)
	{
		_Table.RemoveScope();
		return;
	}
	FuncInfo* Info = syb->Get_Info<FuncInfo>();


	_FuncStack.push_back(Info);


	if (passtype == PassType::FixedTypes
		|| (IsGenericS && passtype == PassType::GetTypes))
	{
		
		auto& RetType = node.Signature.ReturnType.node;
		if (RetType && RetType->Get_Type() == NodeType::AnonymousTypeNode)
		{
			SymbolID AnonymousSybID = (SymbolID)RetType.get();
			auto& V = *GetSymbol(AnonymousSybID);

			auto ClassInf = (ClassInfo*)V.Get_Info<ClassInfo>();

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
			auto& Sybol =*GetSymbol(ParSybID);
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
			auto F = V.ParsType.emplace_back();
			auto& VT = ConvertToTypeInfo(Item);

		}

		PushTepAttributesInTo(V.Attributes);


		Ptr->_Class.Methods.push_back(std::move(V));

		auto& RetType = node.Signature.ReturnType.node;
		if (RetType && RetType->Get_Type() == NodeType::AnonymousTypeNode)
		{
			SymbolID AnonymousSybID = (SymbolID)RetType.get();
			auto& V = *GetSymbol(AnonymousSybID);

			auto ClassInf = V.Get_Info<ClassInfo>();

			AddClass_tToAssemblyInfo(ClassInf);

		}
	}


	bool buidCode = passtype == PassType::BuidCode;
	bool ignoreBody = !IsgenericInstantiation && IsGenericS;

	if (buidCode && !ignoreBody)
	{
		LookingAtIRFunc =_Builder.NewFunc(syb->FullName, {});
		LookingAtIRBlock = LookingAtIRFunc->NewBlock("");
		PushNewStackFrame();

		auto& ParNodes = node.Signature.Parameters.Parameters;
		
		LookingAtIRFunc->Pars.resize(ParNodes.size());//becuase we are useing ptrs.
		for (size_t i = 0; i < ParNodes.size(); i++)
		{
			auto& Item = ParNodes[i];

			auto ParSybID = (SymbolID)&Item;
			auto& V = *GetSymbol(ParSybID);
			

			auto& d = LookingAtIRFunc->Pars[i];
			d.identifier = _Builder.ToID(V.FullName);
			d.type = ConvertToIR(V.VarType);




			if (HasDestructor(V.VarType))
			{
				ObjectToDrop V;
				V.DropType = ObjectToDropType::Operator;
				V.ID = ParSybID;
				V._Operator = IROperator(&d);
				V.Type = V.Type;

				StackFrames.back().OnEndStackFrame.push_back(V);
			}

			V.IR_Par = &d;
		}
		LookingAtIRFunc->ReturnType = ConvertToIR(syb->VarType);

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
			//_Builder.Build_DLLJump(node.Signature.Name.AsStringView());
		}
	}
	

	if (node.Body.has_value() && !ignoreBody)
	{
		auto& Body = node.Body.value();
		_InStatements = true;
		bool HasARet = false;
		LookingForTypes.push(syb->VarType);


		if (passtype == PassType::FixedTypes && IsRetDependencies(Info) && syb->VarType._Type == TypesEnum::Var)
		{
			for (const auto& node2 : Body.Statements._Nodes)
			{
				if (node2->Get_Type() == NodeType::RetStatementNode)
				{
					OnStatement(node2);

					auto OldType = syb->VarType;

					auto NewType = LastExpressionType;

					if (OldType.IsAddress()) {
						NewType.SetAsAddress();
					}
					if (OldType.IsAddressArray()) {
						NewType.SetAsAddressArray();
					}

					Get_LookingForType() = syb->VarType = NewType;
					break;
				}
			}

			if (syb->VarType._Type == TypesEnum::Var)
			{
				syb->VarType.SetType(TypesEnum::Void);
			}
			else if (syb->VarType._Type != TypesEnum::Void
				 && !syb->VarType.IsBadType())//Update This when control flow get added.
			{
				auto Token = node.Signature.Name.Token;
				YouMustReturnSomethingError(Token);
			}
		}

		for (const auto& node2 : Body.Statements._Nodes)
		{
			OnStatement(node2);
			if (node2->Get_Type() == NodeType::RetStatementNode)
			{
				HasARet = true;

				if (passtype == PassType::FixedTypes && syb->VarType._Type == TypesEnum::Var)
				{
					auto OldType = syb->VarType;

					auto NewType = LastExpressionType;

					if (OldType.IsAddress()) {
						NewType.SetAsAddress();
					}
					if (OldType.IsAddressArray()) {
						NewType.SetAsAddressArray();
					}

					

					Get_LookingForType() = syb->VarType = NewType;

					if (LastExpressionType._Type == TypesEnum::Var)
					{
						auto Token = LastLookedAtToken;
						CantguessVarTypeError(Token);
					}

					OnStatement(node2);//re do
				}

				break;
			}
		}

		_InStatements = false;

		
		if (passtype == PassType::FixedTypes)
		{
			if (!HasARet)
			{
				if (syb->VarType._Type == TypesEnum::Var)
				{
					syb->VarType.SetType(TypesEnum::Void);
				}
				else if (syb->VarType._Type != TypesEnum::Void
					&& !syb->VarType.IsBadType())//Update This when control flow get added.
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
		PopStackFrame();
		
		LookingAtIRBlock->NewRet();

	}

	

	if (passtype == PassType::FixedTypes)
	{
		if (!node.Body.has_value() && syb->VarType._Type == TypesEnum::Var)
		{
			auto Token = node.Signature.Name.Token;
			LogBeMoreSpecifiicForRetType(FuncName, Token);
		}
		else {
			FuncRetCheck(*node.Signature.Name.Token, syb, Info);
			auto Token = node.Signature.Name.Token;
			DoSymbolRedefinitionCheck(syb, Info, Token);
		}
	}

	_FuncStack.pop_back();

	_Table.RemovePopUseing(UseingIndex);



	
	_Table.RemoveScope();

	syb->PassState = passtype;
}
void SystematicAnalysis::FuncRetCheck(const Token& Name, const Symbol* FuncSyb, const FuncInfo* Func)
{
	switch (Func->_FuncType)
	{
	case FuncInfo::FuncType::Copy_Ctor:
	case FuncInfo::FuncType::Drop:
	case FuncInfo::FuncType::New:
	{

		TypeSymbol V;
		V.SetType(TypesEnum::Void);
		if (!AreTheSame(Func->Ret, V)) 
		{
			LogFuncMustBe(&Name, FuncSyb->FullName, V);
		}
	}
	break;
	case FuncInfo::FuncType::NameFunc:
		break;
	default:
		throw std::exception("not added");
		break;
	}
}
IRidentifierID SystematicAnalysis::ConveToIRClassIR(const Symbol& Class)
{
	auto ClassSybID = Class.ID;
	if (SybToIRMap.count(ClassSybID))
	{
		return SybToIRMap[ClassSybID];
	}
	const ClassInfo* clasinfo = Class.Get_Info < ClassInfo>();

	IRidentifierID V = _Builder.ToID(Class.FullName);
	
	auto IRStuct = _Builder.NewStruct(V);

	IRStuct->Fields.resize(clasinfo->Fields.size());

	for (size_t i = 0; i < clasinfo->Fields.size(); i++)
	{
		auto& Item = clasinfo->Fields[i];
		auto& Out = IRStuct->Fields[i];
		Out.Type = ConvertToIR(Item.Type);
	}

	SybToIRMap[ClassSybID] = V;
}
IRType SystematicAnalysis::ConvertToIR(const TypeSymbol& Value)
{
	if (Value.IsAddress() || Value.IsAddressArray())
	{
		return IRType(IRTypes::pointer);
	}

	switch (Value._Type)
	{
		
	case TypesEnum::Bool:
	case TypesEnum::Char:
	case TypesEnum::sInt8:
	case TypesEnum::uInt8:return IRType(IRTypes::i8);
		
	case TypesEnum::sInt16:
	case TypesEnum::uInt16:return IRType(IRTypes::i16);

	case TypesEnum::sInt32:
	case TypesEnum::uInt32:return IRType(IRTypes::i32);

	case TypesEnum::sInt64:
	case TypesEnum::uInt64:return IRType(IRTypes::i64);

	case TypesEnum::float32:return IRType(IRTypes::f32);
	case TypesEnum::float64:return IRType(IRTypes::f64);

	case TypesEnum::Void:return IRType(IRTypes::Void);

	case TypesEnum::CustomType: 
	{
		Symbol& syb = *GetSymbol(Value._CustomTypeSymbol);
		if (syb.Type == SymbolType::Enum)
		{
			EnumInfo* V = syb.Get_Info <EnumInfo>();
			return ConvertToIR(V->Basetype);
		}
		else if (syb.Type == SymbolType::Type_alias
			|| syb.Type == SymbolType::Hard_Type_alias)
		{
			return ConvertToIR(syb.VarType);
		}
		else if (syb.Type == SymbolType::Func_ptr || syb.Type == SymbolType::Hard_Func_ptr)
		{
			if (SybToIRMap.count(syb.ID))
			{
				return IRType(SybToIRMap[syb.ID]);
			}
			else
			{
				FuncInfo* V = syb.Get_Info<FuncInfo>();
				IRidentifierID IRid = _Builder.ToID(syb.FullName);
				IRType r = IRid;
				auto tep = _Builder.NewFuncPtr(_Builder.ToID(syb.FullName), ConvertToIR(V->Ret));

				tep->Pars.resize(V->Pars.size());
				for (auto& Item : tep->Pars)
				{
					Item = ConvertToIR(V->Ret);
				}

				SybToIRMap[syb.ID] = IRid;
				return r;
			}
		}
		else if (syb.Type == SymbolType::Type_class)
		{
			return IRType(ConveToIRClassIR(syb));
		}
		else
		{
			throw std::exception("not added");
		}
	}
	break;
	PtrSize:
	case TypesEnum::sIntPtr:
	case TypesEnum::uIntPtr:
		return IRType(IRTypes::pointer);
	default:
		
		throw std::exception("not added");
		break;
	}
}
void SystematicAnalysis::PushNewStackFrame()
{
	StackFrames.emplace_back();
}
void SystematicAnalysis::PopStackFrame()
{
	auto& TopStack = StackFrames.back();

	for (auto& Item : TopStack.OnEndStackFrame)
	{
		DoDestructorCall(Item);
	}


	StackFrames.pop_back();
}
SymbolID SystematicAnalysis::GetSymbolID(const Node& node)
{
	bool IsgenericInstantiation = GenericFuncName.size() && GenericFuncName.top().NodeTarget == &node;
	if (IsgenericInstantiation)
	{
		return (SymbolID)GenericFuncName.top().GenericInput;
	}
	else
	{
		auto sybId = (SymbolID)&node;
		if (GenericFuncName.size())
		{
			sybId += (SymbolID)GenericFuncName.top().GenericInput;
		}

		return sybId;
	}
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
	case NodeType::FuncCallStatementNode:
	{
		TypeSymbol V; V.SetType(TypesEnum::Any);
		LookingForTypes.push(V);

		OnFuncCallNode(FuncCallStatementNode::As(node2.get())->Base);

		LookingForTypes.pop();
	}
	break;
	case NodeType::DropStatementNode:OnDropStatementNode(*DropStatementNode::As(node2.get())); break;
	case NodeType::IfNode:OnIfNode(*IfNode::As(node2.get())); break;
	case NodeType::WhileNode:OnWhileNode(*WhileNode::As(node2.get())); break;
	case NodeType::DoNode:OnDoNode(*DoNode::As(node2.get())); break;
	case NodeType::DeclareStaticVariableNode:OnDeclareStaticVariableNode(*DeclareStaticVariableNode::As(node2.get())); break;
	case NodeType::DeclareThreadVariableNode:OnDeclareThreadVariableNode(*DeclareThreadVariableNode::As(node2.get())); break;
	case NodeType::RetStatementNode:OnRetStatement(*RetStatementNode::As(node2.get())); break;
	default:break;
	}
}
void SystematicAnalysis::OnRetStatement(const RetStatementNode& node)
{

	auto& LookForT = Get_LookingForType();
	if (node.Expression.Value)
	{
	_FuncStack.back().IsOnRetStatemnt = true;
		//LookForT.SetAsRawValue();

		LookingForTypes.push(LookForT);
		OnExpressionTypeNode(node.Expression.Value.get());

		LookingForTypes.pop();

		_FuncStack.back().IsOnRetStatemnt = false;
	}
	else
	{
		LastExpressionType.SetType(TypesEnum::Void);
	}


	if (passtype == PassType::FixedTypes)
	{
		auto& T = LookForT;
		if (T._Type != TypesEnum::Var)
		{
			if (!CanBeImplicitConverted(LastExpressionType, T,false))
			{
				LogCantCastImplicitTypes(LastLookedAtToken, LastExpressionType, T, false);
			}
		}
	}
	if (passtype == PassType::BuidCode)
	{
		auto& T = Get_LookingForType();
		DoImplicitConversion(_LastExpressionField, LastExpressionType, T);
		if (node.Expression.Value)
		{
			LookingAtIRBlock->NewRetValue(_LastExpressionField);
		}
	}
}
void SystematicAnalysis::OnEnum(const EnumNode& node)
{
	SymbolID SybID = GetSymbolID(node);

	const auto& ClassName = node.EnumName.Token->Value._String;
	_Table.AddScope(ClassName);

	if (passtype == PassType::GetTypes)
	{
		DoSymbolRedefinitionCheck((String_view)_Table._Scope.ThisScope, SymbolType::Type_class, node.EnumName.Token);
	}

	auto& Syb = passtype == PassType::GetTypes ?
		AddSybol(SymbolType::Enum
			, (String)ClassName, _Table._Scope.ThisScope) :
		*GetSymbol(SybID);

	EnumInfo* ClassInf;
	if (passtype == PassType::GetTypes)
	{
		_Table.AddSymbolID(Syb, SybID);
		Syb.NodePtr = (void*)&node;


		ClassInf = new EnumInfo();
		ClassInf->FullName = Syb.FullName;
		Syb.Info.reset(ClassInf);
		Syb.VarType.SetType(Syb.ID);
	}
	else
	{
		ClassInf = (EnumInfo*)Syb.Get_Info<EnumInfo>();
	}
	//add  dependency cycle.
	EvaluatedEx ex;
	if (passtype == PassType::FixedTypes)
	{
		Convert(node.BaseType, ClassInf->Basetype);
		if (ClassInf->Basetype.IsBadType()) { return; }
		if (!ConstantExpressionAbleType(ClassInf->Basetype))
		{
			LogTypeMustBeAnConstantExpressionAble(node.BaseType.Name.Token, ClassInf->Basetype);
			return;
		}
		ex = std::move(MakeEx(ClassInf->Basetype));
	}


	bool HasCheckedForincrementOp = false;
	LookingForTypes.push(ClassInf->Basetype);

	for (size_t i = 0; i < node.Values.size(); i++)
	{
		auto& Item = node.Values[i];
	
		String_view ItemName = Item.Name.Token->Value._String;

		if (passtype == PassType::GetTypes)
		{
			ClassInf->AddField(ItemName);

			if (Item.Expression.Value)
			{
				OnExpressionTypeNode(Item.Expression.Value.get());
			}

		}
		else if (passtype == PassType::FixedTypes)
		{


			auto& Field = *ClassInf->GetField(ItemName);
			if (Item.Expression.Value)
			{
				auto& Type = ClassInf->Basetype;
				OnExpressionTypeNode(Item.Expression.Value.get());//check
				if (!CanBeImplicitConverted(LastExpressionType, Type,false))
				{
					LogCantCastImplicitTypes(LastLookedAtToken, LastExpressionType, Type,false);
					return;
				}
				if (!CanEvaluateImplicitConversionConstant(LastExpressionType, Type))
				{
					LogCantCastImplicitTypes_Constant(LastLookedAtToken, LastExpressionType, Type);
					return;
				}

				EvaluatedEx ex1 =MakeEx(LastExpressionType);
				Evaluate_t(ex1, Item.Expression.Value.get());

				EvaluateImplicitConversion(ex1, Type, ex);

				Field.Ex = ex.EvaluatedObject;
			}
			else
			{
				if (i != 0)
				{
					auto& Type = ClassInf->Basetype;
					if (HasCheckedForincrementOp == false)
					{
						const Token* LineDataToken = Item.Name.Token;
						if (!HasPostfixOverLoadWith(Type, TokenType::increment))
						{

							Token temp;
							temp.OnLine = LineDataToken->OnLine;
							temp.OnPos = LineDataToken->OnPos;
							temp.Type = TokenType::increment;

							LogCantFindPostfixOpForTypes(&temp, Type);
							return;
						}
						if (!HasConstantPostfixOperator(Type, TokenType::increment))
						{
							Token temp;
							temp.OnLine = LineDataToken->OnLine;
							temp.OnPos = LineDataToken->OnPos;
							temp.Type = TokenType::increment;


							LogCantDoPostfixOpForTypes_Constant(&temp, Type);
							return;
						}
						HasCheckedForincrementOp = true;
					}

					EvaluatePostfixOperator(ex, TokenType::increment);
				}
				else
				{
					EvaluateDefaultConstructor(ex);
				}


				Field.Ex = ex.EvaluatedObject;
			}
		}

	}

	LookingForTypes.pop();
	if (passtype == PassType::BuidCode)
	{
		AddDependencyToCurrentFile(ClassInf->Basetype);
	}

	_Table.RemoveScope();
}

String SystematicAnalysis::GetScopedNameAsString(const ScopedNameNode& node)
{
	String Text;
	if (node.ScopedName[0].token->Type == TokenType::KeyWorld_This)
	{
		auto Type = *_FuncStack.back().Pointer->GetObjectForCall();
		Type._IsAddress = false;

		Text += ToString(Type);

		if (node.ScopedName.size() > 1) 
		{
			Text += ScopeHelper::_ScopeSep;
			node.GetScopedName(Text, 1);
		}
	}
	else {
		node.GetScopedName(Text);
	}return Text;
}
void SystematicAnalysis::OnDeclareVariablenode(const DeclareVariableNode& node)
{
	auto& StrVarName = node.Name.AsString();
	auto FullName = _Table._Scope.GetApendedString(StrVarName);

	SymbolID sybId = GetSymbolID(node);
	Symbol* syb;

	bool InSideClass = _InSideClass();

	if (passtype == PassType::GetTypes)
	{
		DoSymbolRedefinitionCheck(FullName, SymbolType::StackVarable, node.Name.Token);


		syb = &AddSybol(SymbolType::StackVarable, StrVarName, FullName);
		_Table.AddSymbolID(*syb, sybId);

		syb->Size = NullAddress;
		if (InSideClass)
		{
			syb->Type = SymbolType::Class_Field;
			auto& Class = *_ClassStack.top();

			Class.AddField(ScopeHelper::GetNameFromFullName((String_view)FullName), TypeSymbol());

		}
		syb->NodePtr = (void*)node.Name.Token->OnPos;//a tep solution. 
	}
	else
	{
		syb = GetSymbol(sybId);
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

	IRInstruction* OnVarable{};
	bool Ind =false;
	if (node.Expression.Value)
	{
		if (passtype == PassType::BuidCode)
		{
		
			if (syb->Type == SymbolType::StackVarable)
			{
				OnVarable = LookingAtIRBlock->NewLoad(ConvertToIR(syb->VarType));
				syb->IR_Ins = OnVarable;
			}

			Ind = !(IsPrimitive(syb->VarType) || syb->VarType.IsAddress());

			if (Ind)
			{
				IRlocations.push({ OnVarable ,false});
			}

		}
		OnExpressionTypeNode(node.Expression.Value.get());
	}
	else if (passtype == PassType::BuidCode)
	{
		if (syb->Type == SymbolType::StackVarable)
		{
			OnVarable = LookingAtIRBlock->NewLoad(ConvertToIR(syb->VarType));
			syb->IR_Ins = OnVarable;
		}

		
	}

	if (passtype == PassType::FixedTypes)
	{
		syb = GetSymbol(sybId);
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
					bool WasIsAddressArry = VarType.IsAddressArray();

					VarType = Ex;

					if (WasImutable) { VarType.SetAsimmutable(); }
					if (WasIsAddress) { VarType.SetAsAddress(); }
					if (WasIsAddressArry) { VarType.SetAsAddressArray(); }

					VarType.SetAsLocation();
					GetSize(VarType, syb->Size);
				}
			}
			if (!CanBeImplicitConverted(Ex, VarType,false))
			{
				LogCantCastImplicitTypes(Token, Ex, VarType,false);
			}
		}
		else
		{
			if (!InSideClass) {
				syb->SetToInvalid();
			}
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
				//UAddress V;
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
		
		DoImplicitConversion(_LastExpressionField, LastExpressionType, syb->VarType);

		

		if (Ind)
		{
			if (IRlocations.top().UsedlocationIR == false)
			{
				
				if (_LastExpressionField->Type == IRInstructionType::Load 
					&& _LastExpressionField->Target().Type ==IROperatorType::IRInstruction)
				{//to stop copying big objects
					LookingAtIRBlock->NewStore(OnVarable, _LastExpressionField->Target().Pointer);
					_LastExpressionField->SetAsNone();
				}
				else
				{
					LookingAtIRBlock->NewStore(OnVarable, _LastExpressionField);
				}
			}
			IRlocations.pop();
		}
		else 
		{
			LookingAtIRBlock->NewStore(OnVarable, _LastExpressionField);
		}
		_LastExpressionField = nullptr;

		if (HasDestructor(syb->VarType))
		{
			ObjectToDrop V;
			V.ID = sybId;
			V._Object= OnVarable;
			V.Type = syb->VarType;

			StackFrames.back().OnEndStackFrame.push_back(V);
		}
		
		AddDependencyToCurrentFile(syb->VarType);
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
		auto Type = MemberInfo.Type;
		
		if (Type.IsAddress())
		{
			Type._IsAddress = false;
		}
		LookingForTypes.push(Type);
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
			LogCantCastImplicitTypes(Token, LastExpressionType, MemberInfo.Type,true);

		}
	}

	


	if (passtype == PassType::BuidCode)
	{

		DoImplicitConversion(_LastExpressionField, LastExpressionType, MemberInfo.Type);

		auto Token = node.Name.ScopedName.begin()->token;
		auto& Str = Token->Value._String;
		Symbol* Symbol = GetSymbol(Str, SymbolType::Varable_t);
		TypeSymbol& SymbolType = Symbol->VarType;


		BuildMember_Reassignment(MemberInfo, SymbolType, _LastExpressionField);

	}
}
void SystematicAnalysis::OnIfNode(const IfNode& node)
{

	TypeSymbol BoolType;
	BoolType.SetType(TypesEnum::Bool);

	String ScopeName = std::to_string((size_t)&node);

	PushNewStackFrame();
	_Table.AddScope(ScopeName);
	
	LookingForTypes.push(BoolType);


	

	OnExpressionTypeNode(node.Expression.Value.get());


	if (passtype == PassType::FixedTypes)
	{
		if (!CanBeImplicitConverted(LastExpressionType, BoolType))
		{
			auto  Token = LastLookedAtToken;
			LogCantCastImplicitTypes(Token, LastExpressionType, BoolType, true);
		}
	}

	
	IRBlock::NewConditionalFalseJump_t IfIndex;
	IRInstruction* BoolCode;
	if (passtype == PassType::BuidCode)
	{
		DoImplicitConversion(_LastExpressionField, LastExpressionType, BoolType);
		BoolCode = _LastExpressionField;
		IfIndex = LookingAtIRBlock->NewConditionalFalseJump(BoolCode);
	}
	
	
	

	LookingForTypes.pop();

	for (const auto& node2 :node.Body._Nodes)
	{
		OnStatement(node2);
	}
	
	PopStackFrame();
	_Table.RemoveScope();
	
	if (node.Else)
	{
		IRInstruction* ElseIndex{};
		size_t ElseI;
		if (passtype == PassType::BuidCode)
		{
			ElseIndex = LookingAtIRBlock->NewJump();
			ElseI = LookingAtIRBlock->GetIndex();
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
			auto JumpIndex = LookingAtIRBlock->GetIndex() ;
			LookingAtIRBlock->UpdateJump(ElseIndex, JumpIndex);
			LookingAtIRBlock->UpdateConditionaJump(IfIndex.ConditionalJump, IfIndex.logicalNot, ElseI);
		}
	}
	else if (passtype == PassType::BuidCode)
	{
		LookingAtIRBlock->UpdateConditionaJump(IfIndex.ConditionalJump, BoolCode, LookingAtIRBlock->GetIndex());
	}
	
	
}
void SystematicAnalysis::OnWhileNode(const WhileNode& node)
{
	TypeSymbol BoolType;
	BoolType.SetType(TypesEnum::Bool);

	String ScopeName = std::to_string((size_t)&node);

	PushNewStackFrame();
	_Table.AddScope(ScopeName);

	LookingForTypes.push(BoolType);


	size_t BoolCode;
	if (passtype == PassType::BuidCode)
	{
		BoolCode = LookingAtIRBlock->GetIndex();
	}
	OnExpressionTypeNode(node.Expression.Value.get());

	if (passtype == PassType::FixedTypes)
	{
		if (!CanBeImplicitConverted(LastExpressionType, BoolType))
		{
			auto  Token = LastLookedAtToken;
			LogCantCastImplicitTypes(Token, LastExpressionType, BoolType,true);
		}
	}


	IRBlock::NewConditionalFalseJump_t IfIndex;
	IRInstruction* BoolCode2;
	if (passtype == PassType::BuidCode)
	{
		DoImplicitConversion(_LastExpressionField, LastExpressionType, BoolType);
		BoolCode2 = _LastExpressionField;
		IfIndex = LookingAtIRBlock->NewConditionalFalseJump(BoolCode2);

	}




	LookingForTypes.pop();

	for (const auto& node2 : node.Body._Nodes)
	{
		OnStatement(node2);
	}


	if (passtype == PassType::BuidCode)
	{
		LookingAtIRBlock->NewJump(BoolCode);
		LookingAtIRBlock->UpdateConditionaJump(IfIndex.ConditionalJump, IfIndex.logicalNot, LookingAtIRBlock->GetIndex());
	}


	_Table.RemoveScope();

	PopStackFrame();
}
void SystematicAnalysis::OnDoNode(const DoNode& node)
{
	String ScopeName = std::to_string((size_t)&node);

	PushNewStackFrame();
	_Table.AddScope(ScopeName);


	size_t StartIndex;
	if (passtype == PassType::BuidCode)
	{
		StartIndex= LookingAtIRBlock->GetIndex();
	}


	for (const auto& node2 : node.Body._Nodes)
	{
		OnStatement(node2);
	}

	_Table.RemoveScope();
	PopStackFrame();

	TypeSymbol BoolType;
	BoolType.SetType(TypesEnum::Bool);
	LookingForTypes.push(BoolType);

	OnExpressionTypeNode(node.Expression.Value.get());

	if (passtype == PassType::FixedTypes)
	{
		if (!CanBeImplicitConverted(LastExpressionType, BoolType))
		{
			auto  Token = LastLookedAtToken;
			LogCantCastImplicitTypes(Token, LastExpressionType, BoolType,true);
		}
	}

	
	if (passtype == PassType::BuidCode)
	{
		DoImplicitConversion(_LastExpressionField, LastExpressionType, BoolType);

		LookingAtIRBlock->NewConditionalFalseJump(_LastExpressionField, StartIndex);
	}
	



	LookingForTypes.pop();

}
void SystematicAnalysis::OnDeclareStaticVariableNode(const DeclareStaticVariableNode& node)
{

}
void SystematicAnalysis::OnDeclareThreadVariableNode(const DeclareThreadVariableNode& node)
{
	OnDeclareStaticVariableNode((DeclareStaticVariableNode&)node);//for testing
}

bool SystematicAnalysis::GetMemberTypeSymbolFromVar(const ScopedNameNode& node, GetMemberTypeSymbolFromVar_t& Out)
{
	auto Token = node.ScopedName.begin()->token;

	TypeSymbol FeildType;
	Symbol* FeildTypeAsSymbol;
	if (Token->Type != TokenType::KeyWorld_This) 
	{
		auto& Str = Token->Value._String;
		auto SymbolVar = GetSymbol(Str, SymbolType::Varable_t);
		LastLookedAtToken = Token;
		if (SymbolVar == nullptr)
		{
			LogCantFindVarError(Token, Str);
			return false;
		}

		CheckVarWritingErrors(SymbolVar, Token, String_view(Str));

		FeildType = SymbolVar->VarType;
		FeildTypeAsSymbol = SymbolVar;


		if (SymbolVar->Type == SymbolType::Func)
		{
			FuncInfo* Finfo = SymbolVar->Get_Info<FuncInfo>();
			String TepFuncPtr = GetTepFuncPtrName(SymbolVar);
			Finfo->FullName = TepFuncPtr;

			Symbol* V = GetTepFuncPtrSyb(TepFuncPtr, Finfo);
			FeildType.SetType(V->ID);
		}
		else if (FeildType._Type == TypesEnum::CustomType)
		{
			FeildType = SymbolVar->VarType;
		}
	}
	else
	{
		if (_ClassStack.size() == 0)
		{
			CantUseThisKeyWordHereError(Token);
			return false;
		}


		if (!IsInThisFuncCall())
		{
			LogCantUseThisInStaticFunction(Token);
		}



		auto& Func = _FuncStack.back().Pointer;
		auto ObjectType = Func->GetObjectForCall();

		auto objecttypesyb = GetSymbol(*ObjectType);
		ClassInfo* V = objecttypesyb->Get_Info<ClassInfo>();

		FeildType = *Func->GetObjectForCall();
		FeildTypeAsSymbol = GetSymbol(*ObjectType);
	}

	size_t Start = 1;


	Out.Symbol = FeildTypeAsSymbol;
	Out.Type = FeildType;
	if (!GetMemberTypeSymbolFromVar(Start, node, Out))
	{
		return false;
	}

	if (passtype == PassType::BuidCode)
	{
		AddDependencyToCurrentFile(FeildTypeAsSymbol);
	}
	
	if (Out.Symbol->Type == SymbolType::Class_Field && _FuncStack.size() && _ClassStack.top())
	{
		if (!IsInThisFuncCall())
		{
			LogCantUseThisInStaticFunction(Token);
		}
	}

	return true;
}
void SystematicAnalysis::BuildMember_Store(const GetMemberTypeSymbolFromVar_t& In, IRInstruction* Value)
{

	IRInstruction* Output = nullptr;

	BuildMember_Access(In, Output);
	if (Output == nullptr)
	{
		Output = In.Symbol->IR_Ins;
	}
	bool UseOutput = In.Symbol->IR_Ins != Output;
	//


	switch (In.Symbol->Type)
	{
		case  SymbolType::Type_class://this
	case  SymbolType::Class_Field:
	case  SymbolType::StackVarable:
		LookingAtIRBlock->NewStore(Output, Value);
		break;
	case  SymbolType::ParameterVarable:
		UseOutput ? LookingAtIRBlock->NewStore(Output, Value) : LookingAtIRBlock->NewStore(In.Symbol->IR_Par, Value);
		break;
	default:
		throw std::exception("not added");
		break;
	}
}
IRInstruction* SystematicAnalysis::BuildMember_GetPointer(const GetMemberTypeSymbolFromVar_t& In)
{
	IRInstruction* Output = nullptr;

	BuildMember_Access(In, Output);
	if (Output == nullptr)
	{
		Output = In.Symbol->IR_Ins;
	}
	bool UseOutput = In.Symbol->IR_Ins != Output;

	switch (In.Symbol->Type)
	{
	case  SymbolType::Type_class://this
	case  SymbolType::Class_Field:
	case  SymbolType::StackVarable:
		return LookingAtIRBlock->NewLoadPtr(Output);
		break;
	case  SymbolType::ParameterVarable:
		return UseOutput ? LookingAtIRBlock->NewLoadPtr(Output): LookingAtIRBlock->NewLoadPtr(In.Symbol->IR_Par);
		break;
	default:
		throw std::exception("not added");
		break;
	}
}
void SystematicAnalysis::BuildMemberDereferencStore(const GetMemberTypeSymbolFromVar_t& In, IRInstruction* Value)
{
	switch (In.Symbol->Type)
	{
	case  SymbolType::StackVarable:
		LookingAtIRBlock->NewDereferenc_Store(In.Symbol->IR_Ins,Value);
		break;
	case  SymbolType::ParameterVarable:
		LookingAtIRBlock->NewDereferenc_Store(In.Symbol->IR_Par, Value);
		break;
	default:
		throw std::exception("not added");
		break;
	}
}
IRInstruction* SystematicAnalysis::BuildMember_GetValue(const GetMemberTypeSymbolFromVar_t& In)
{
	switch (In.Symbol->Type)
	{
	case  SymbolType::Class_Field:
	case  SymbolType::StackVarable:
	case  SymbolType::ParameterVarable:
	{
		IRInstruction* Output = nullptr;

		BuildMember_Access(In, Output);
		if (Output == nullptr)
		{
			Output = In.Symbol->IR_Ins;
		}
		bool UseOutput = In.Symbol->IR_Ins != Output;

		if (In.Symbol->Type == SymbolType::StackVarable
			|| In.Symbol->Type == SymbolType::Class_Field)
		{
			return LookingAtIRBlock->NewLoad(Output);
		}
		else if (In.Symbol->Type == SymbolType::ParameterVarable)
		{
			return UseOutput ? LookingAtIRBlock->NewLoad(Output) : LookingAtIRBlock->NewLoad(In.Symbol->IR_Par);
		}
		else
		{
			throw std::exception("not added");
		}

			
	}
	break;
	case SymbolType::Hard_Func_ptr:
	case SymbolType::Func_ptr:
	case SymbolType::Func:
	{
		FuncInfo* Finfo = In.Symbol->Get_Info<FuncInfo>();
		return LookingAtIRBlock->NewLoadFuncPtr(_Builder.ToID((String)GetTepFuncPtrNameAsName(Finfo->FullName)));
	}
	
	break;
	case SymbolType::Enum:
	{
		auto Einfo = In.Symbol->Get_Info<EnumInfo>();
		auto FeldInfo = In.Get_V1<EnumFieldInfo>();

		void* ObjectData = Get_Object( Einfo->Basetype, FeldInfo->Ex);

		switch (Einfo->Basetype._Type)
		{
		case TypesEnum::Bool:
		case TypesEnum::Char:
		case TypesEnum::sInt8:
		case TypesEnum::uInt8:
			return LookingAtIRBlock->NewLoad(*(UInt8*)ObjectData);
			break;
		case TypesEnum::sInt16:
		case TypesEnum::uInt16:
			return LookingAtIRBlock->NewLoad(*(UInt16*)ObjectData);
			break;
		case TypesEnum::sInt32:
		case TypesEnum::uInt32:
			return LookingAtIRBlock->NewLoad(*(UInt32*)ObjectData);
			break;
		case TypesEnum::float32:
			return LookingAtIRBlock->NewLoad(*(float32*)ObjectData);
			break;
		case TypesEnum::float64:
			return LookingAtIRBlock->NewLoad(*(float64*)ObjectData);
			break;
		case TypesEnum::sInt64:
		case TypesEnum::uInt64:
			return LookingAtIRBlock->NewLoad(*(UInt64*)ObjectData);
			break;
		default:
			throw std::exception("not added");
			break;
		}
	}
	case SymbolType::Type_class:
	{
		if (In.Start[0].token->Type == TokenType::KeyWorld_This)
		{
			IRInstruction* Output = nullptr;
			BuildMember_Access(In, Output);
			return LookingAtIRBlock->NewLoad(Output);
		}
		else
		{
			throw std::exception("not added");
		}
	}
		break;
	default:
		throw std::exception("not added");
		break;
	}
}
IRInstruction* SystematicAnalysis::BuildMember_AsPointer(const GetMemberTypeSymbolFromVar_t& In)
{
	if (In.Type.IsAddress()|| In.Type.IsAddressArray())
	{
		return BuildMember_GetValue(In);
	}
	else
	{
		return BuildMember_GetPointer(In);
	}
}
IRInstruction* SystematicAnalysis::BuildMember_DereferenceValue(const GetMemberTypeSymbolFromVar_t& In)
{
	TypeSymbol T = In.Type;
	T._IsAddress = false;
	IRType IRT = ConvertToIR(T);

	switch (In.Symbol->Type)
	{
	case  SymbolType::Class_Field:
	case  SymbolType::StackVarable:
		return LookingAtIRBlock->NewLoad_Dereferenc(In.Symbol->IR_Ins, IRT);
		break;
	case  SymbolType::ParameterVarable:
		return  LookingAtIRBlock->NewLoad_Dereferenc(In.Symbol->IR_Par, IRT);
		break;
	default:
		break;
	}
}
IRInstruction* SystematicAnalysis::BuildMember_AsValue(const GetMemberTypeSymbolFromVar_t& In)
{
	if (In.Type.IsAddress() || In.Type.IsAddressArray())
	{
		return BuildMember_DereferenceValue(In);
	}
	else
	{
		return BuildMember_GetValue(In);
	}
}
void SystematicAnalysis::BuildMember_Reassignment(const GetMemberTypeSymbolFromVar_t& In, const TypeSymbol& Type, IRInstruction* Value)
{
	if (Type.IsAddress())
	{
		BuildMemberDereferencStore(In, Value);
	}
	else
	{
		BuildMember_Store(In, Value);
	}
}

void  SystematicAnalysis::BuildMember_Access(const GetMemberTypeSymbolFromVar_t& In, IRInstruction*& Output)
{
	TypeSymbol Last_Type = In.Symbol->VarType;

	//

	if (In.Symbol->Type == SymbolType::Class_Field && _FuncStack.size() && _ClassStack.top())
	{
		auto& Func = _FuncStack.back();
		auto& PointerIr = LookingAtIRFunc->Pars.front();

		auto ObjectType = *Func.Pointer->GetObjectForCall();
		ObjectType._IsAddress = false;

		auto objecttypesyb = GetSymbol(ObjectType);




		auto IRStructV = ConveToIRClassIR(*objecttypesyb);
		auto F = _Builder.GetSymbol(ConveToIRClassIR(*objecttypesyb))->Get_ExAs<IRStruct>();

		auto Token = In.Start[In.End - 1].token;
		auto& Str = Token->Value._String;
		ClassInfo* V = objecttypesyb->Get_Info<ClassInfo>();
		size_t MemberIndex = V->GetFieldIndex(Str);


		Output = LookingAtIRBlock->New_Member_Dereference(&PointerIr, F, MemberIndex);
		return;
	}
	if (In.Start[0].token->Type == TokenType::KeyWorld_This)
	{
		auto& PointerIr = LookingAtIRFunc->Pars.front();
		Output = LookingAtIRBlock->NewLoad(&PointerIr);

		auto& Func = _FuncStack.back();
		Last_Type = *Func.Pointer->GetObjectForCall();
	}
	//


	for (size_t i = 1; i < In.End; i++)
	{
		Symbol* Sym = GetSymbol(Last_Type);

		auto& ITem = In.Start[i];
		switch (Sym->Type)
		{
		case  SymbolType::Type_class:
		{
			auto* Classinfo = Sym->Get_Info<ClassInfo>();
			size_t MemberIndex = Classinfo->GetFieldIndex(ITem.token->Value._String);
			FieldInfo* FInfo = &Classinfo->Fields[MemberIndex];
			IRStruct* IRstruct = _Builder.GetSymbol(SybToIRMap[Sym->ID])->Get_ExAs<IRStruct>();
			if (Output == nullptr)
			{
				switch (In.Symbol->Type)
				{
				case  SymbolType::StackVarable:
					Output = LookingAtIRBlock->New_Member_Access(In.Symbol->IR_Ins, IRstruct, MemberIndex);
					break;
				case  SymbolType::ParameterVarable:
					Output = LookingAtIRBlock->New_Member_Access(In.Symbol->IR_Par, IRstruct, MemberIndex);
					break;
				}
			}
			else
			{
				TypeSymbol& TypeSys = Last_Type;
				if (TypeSys.IsAddress())
				{
					Output = LookingAtIRBlock->New_Member_Dereference(Output, IRstruct, MemberIndex);
				}
				else
				{
					Output = LookingAtIRBlock->New_Member_Access(Output, IRstruct, MemberIndex);
				}
			}
			Last_Type = FInfo->Type;
		}
		break;
		default:
			throw std::exception("not added");
			break;
		}
	}
}

Symbol* SystematicAnalysis::GetTepFuncPtrSyb(const String& TepFuncPtr, const FuncInfo* Finfo)
{
	Symbol* V =GetSymbol(TepFuncPtr, SymbolType::Func_ptr);
	if (V == nullptr)
	{
		V = &AddSybol(SymbolType::Func_ptr, TepFuncPtr, TepFuncPtr);
		FuncPtrInfo* V2 = new FuncPtrInfo();
		V->Info.reset(V2);

		SymbolID VID = (SymbolID)V;

		V2->FullName = Finfo->FullName;

		V2->Pars = Finfo->Pars;
		V2->Ret = Finfo->Ret;
		_Table.AddSymbolID(*V, VID);
	}

	return V;
}

#define TepFuncPtrNameMangleStr "_tepfptr|"
String SystematicAnalysis::GetTepFuncPtrName(Symbol* SymbolVar)
{
	return TepFuncPtrNameMangleStr + SymbolVar->FullName;
}
String_view SystematicAnalysis::GetTepFuncPtrNameAsName(const String_view Str)
{
	return Str.substr(sizeof(TepFuncPtrNameMangleStr)-1);//remove null char
}
bool SystematicAnalysis::GetMemberTypeSymbolFromVar(size_t Start, size_t End, const ScopedNameNode& node, GetMemberTypeSymbolFromVar_t& Out)
{
	auto TepSyb = Out.Symbol;
	size_t ScopedCount = 0;
	if (TepSyb == nullptr && Out.Type.IsBadType())
	{


		if (node.ScopedName[Start].token->Type != TokenType::KeyWorld_This)
		{
			auto Token = node.ScopedName[Start].token;
			auto& Str = Token->Value._String;
			auto SymbolVar = GetSymbol(Str, SymbolType::Varable_t);
			LastLookedAtToken = Token;
			if (SymbolVar == nullptr)
			{
				LogCantFindVarError(Token, Str);
				return false;
			}

			CheckVarWritingErrors(SymbolVar, Token, String_view(Str));

			Out.Type = SymbolVar->VarType;
			Out.Symbol = GetSymbol(SymbolVar->VarType);
			//
			TepSyb = SymbolVar;
			Start++;
			End--;
			ScopedCount++;
		}
		else
		{
			if (_ClassStack.size() == 0)
			{
				CantUseThisKeyWordHereError(node.ScopedName[Start].token);
				return false;
			}


			if (!IsInThisFuncCall())
			{
				LogCantUseThisInStaticFunction(node.ScopedName[Start].token);
			}



			auto& Func = _FuncStack.back().Pointer;
			auto ObjectType = Func->GetObjectForCall();

			auto objecttypesyb = GetSymbol(*ObjectType);
			ClassInfo* V = objecttypesyb->Get_Info<ClassInfo>();

			Out.Type = *Func->GetObjectForCall();
			Out.Symbol = GetSymbol(*ObjectType);
			//
			TepSyb = Out.Symbol;
			Start++;
			End--;
			ScopedCount++;
		}
	}


	for (size_t i = Start; i < node.ScopedName.size(); i++)
	{

		if (i > End) { break; }
		ScopedCount++;


		auto& Item = node.ScopedName[i];


		auto& ItemToken = Item.token;
		LastLookedAtToken = ItemToken;

		if (Out.Type._Type != TypesEnum::CustomType)
		{
			if (passtype == PassType::FixedTypes)
			{
				LogCantFindVarMemberError(ItemToken, ItemToken->Value._String, Out.Type);
			}
			break;
		}

		if (TepSyb->Type == SymbolType::Type_class)
		{
			ClassInfo* CInfo = Out.Symbol->Get_Info<ClassInfo>();


			auto FeldInfo = CInfo->GetField(ItemToken->Value._String);
			if (FeldInfo == nullptr)
			{
				if (passtype == PassType::FixedTypes)
				{
					LogCantFindVarMemberError(ItemToken, ItemToken->Value._String, Out.Type);
				}
				return false;
			}

			auto& FieldType2 = FeldInfo->Type;
			if (FieldType2._Type == TypesEnum::CustomType)
			{
				Out.Symbol = GetSymbol(FieldType2._CustomTypeSymbol);
				Out.Type = FieldType2;
			}
			else
			{
				Out.Type = FieldType2;

				if (i + 1 < node.ScopedName.size())
				{
					const UCodeLang::Token* Token = node.ScopedName.begin()->token;

					auto Token2 = node.ScopedName[i + 1].token;
					auto& Str2 = Token->Value._String;
					if (passtype == PassType::FixedTypes)
					{
						LogCantFindVarMemberError(Token2, Str2, Out.Type);
					}
					break;
				}
			}
			if (passtype == PassType::BuidCode)
			{
				AddDependencyToCurrentFile(Out.Symbol);
			}
		}
		else if (Out.Symbol->Type == SymbolType::Enum)
		{
			EnumInfo* Einfo = Out.Symbol->Get_Info<EnumInfo>();
			auto FeldInfo = Einfo->GetField(ItemToken->Value._String);
			if (FeldInfo == nullptr)
			{
				if (passtype == PassType::FixedTypes)
				{
					LogCantFindVarMemberError(ItemToken, ItemToken->Value._String, Out.Type);
				}
				return false;
			}



			if (i + 1 < node.ScopedName.size())
			{
				const UCodeLang::Token* Token = node.ScopedName.begin()->token;

				auto Token2 = node.ScopedName[i + 1].token;
				auto& Str2 = Token->Value._String;
				if (passtype == PassType::FixedTypes)
				{
					LogCantFindVarMemberError(Token2, Str2, Out.Type);
				}
				break;
			}
			Out.Set_V1(&FeldInfo->Ex);

			AddDependencyToCurrentFile(Out.Symbol);
			Out.Type.SetType(Out.Symbol->ID);
		}
		else if (Out.Symbol->Type == SymbolType::Func)
		{
			if (i + 1 < node.ScopedName.size())
			{
				const UCodeLang::Token* Token = node.ScopedName.begin()->token;

				auto Token2 = node.ScopedName[i + 1].token;
				auto& Str2 = Token->Value._String;
				if (passtype == PassType::FixedTypes)
				{
					LogCantFindVarMemberError(Token2, Str2, Out.Type);
				}
				break;
			}


			Symbol* Func = Out.Symbol;

			FuncInfo* Finfo = Out.Symbol->Get_Info<FuncInfo>();
			String TepFuncPtr = GetTepFuncPtrName(Out.Symbol);

			Symbol* V = GetTepFuncPtrSyb(TepFuncPtr, Finfo);
			Out.Symbol = V;
			Out.Type.SetType(V->ID);


			if (passtype == PassType::BuidCode)
			{
				AddDependencyToCurrentFile(Func);
			}

		}
		else if (Out.Symbol->Type == SymbolType::StackVarable)
		{
			TypeSymbol VarableType = Out.Symbol->VarType;
			Symbol* TypeAsSybol = GetSymbol(VarableType);
			if (TypeAsSybol)
			{
				ClassInfo* CInfo = TypeAsSybol->Get_Info<ClassInfo>();

				auto FeldInfo = CInfo->GetField(ItemToken->Value._String);
				if (FeldInfo == nullptr)
				{
					if (passtype == PassType::FixedTypes)
					{
						LogCantFindVarMemberError(ItemToken, ItemToken->Value._String, Out.Type);
					}
					return false;
				}

				auto& FieldType2 = FeldInfo->Type;
				if (FieldType2._Type == TypesEnum::CustomType)
				{
					Out.Symbol = GetSymbol(FieldType2._CustomTypeSymbol);
					Out.Type = FieldType2;
				}
				else
				{
					Out.Type = FieldType2;

					if (i + 1 < node.ScopedName.size())
					{
						const UCodeLang::Token* Token = node.ScopedName.begin()->token;

						auto Token2 = node.ScopedName[i + 1].token;
						auto& Str2 = Token->Value._String;
						if (passtype == PassType::FixedTypes)
						{
							LogCantFindVarMemberError(Token2, Str2, Out.Type);
						}
						break;
					}
				}
				if (passtype == PassType::BuidCode)
				{
					AddDependencyToCurrentFile(Out.Symbol);
					//throw std::exception("not added");
				}
			}
			else
			{
				if (i + 1 < node.ScopedName.size())
				{
					const UCodeLang::Token* Token = node.ScopedName.begin()->token;

					auto Token2 = node.ScopedName[i + 1].token;
					auto& Str2 = Token->Value._String;
					if (passtype == PassType::FixedTypes)
					{
						LogCantFindVarMemberError(Token2, Str2, VarableType);
					}
					break;
				}
			}

		}
		else
		{
			throw std::exception("bad object");
		}
	}

	Out.Start = &node.ScopedName[Start - 1];
	Out.End = ScopedCount;
	if (End == -1)
	{
		Out.End++;//i dont know why this needed its most likely its us useing two diff funcs for the same thing to get the start off syb.
	}

	Out.Symbol = TepSyb;
	return true;
}
void SystematicAnalysis::OnPostfixVariableNode(const PostfixVariableNode& node)
{
	GetMemberTypeSymbolFromVar_t V;
	if (!GetMemberTypeSymbolFromVar(node.Name, V)) { return; }

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
		
		
		auto _Symbol = GetSymbol(GetScopedNameAsString(node.Name), SymbolType::Varable_t);
		SymbolID sybId = _Symbol->ID;
		auto& Type = _Symbol->VarType;

		AddDependencyToCurrentFile(_Symbol);
		
		IRInstruction* LoadV = BuildMember_AsValue(V);

		#define buildPortFixU(x)\
		if (node.PostfixOp->Type == TokenType::increment)\
		{\
			_LastExpressionField=LookingAtIRBlock->NewAdd(LoadV,LookingAtIRBlock->NewLoad((UInt##x)1));\
		}\
		else\
		{\
			_LastExpressionField=LookingAtIRBlock->NewSub(LoadV, LookingAtIRBlock->NewLoad((UInt##x)1));\
		}\

		

		#define buildPortFixS(x) buildPortFixU(x)

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
			throw std::exception("not added");
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
			throw std::exception("not added");
		}
		break;
		default:
			break;
		}

		BuildMember_Reassignment(V, Type, _LastExpressionField);
	}
}
void SystematicAnalysis::OnCompoundStatementNode(const CompoundStatementNode& node)
{
	auto Name = GetScopedNameAsString(node.VariableName);

	GetMemberTypeSymbolFromVar_t V;
	if (!GetMemberTypeSymbolFromVar(node.VariableName, V)){return;}

	TypeSymbol LookType = V.Symbol->VarType;

	if (passtype == PassType::BuidCode)
	{
		if (IsPrimitive(LookType))
		{
			LookType._IsAddress = false;//we want to def if pointer
		}
		else
		{
			throw std::exception();
		}

	}

	LookingForTypes.push(LookType);

	OnExpressionTypeNode(node.Expession.Value.get());

	LookingForTypes.pop();

	auto ExType = LastExpressionType;
	if (passtype == PassType::FixedTypes)
	{
		CheckVarWritingErrors(V.Symbol, node.VariableName.ScopedName.back().token,String_view(Name));
		LogTryReadVar(Name, node.VariableName.ScopedName.back().token, V.Symbol);

		if (!HasCompoundOverLoadWith(V.Type, node.CompoundOp->Type, ExType))
		{
			LogCantFindCompoundOpForTypes(node.CompoundOp, V.Type, ExType);
		}
	}
	if (passtype == PassType::BuidCode)
	{
		auto Symbol = GetSymbol(GetScopedNameAsString(node.VariableName), SymbolType::Varable_t);
		SymbolID sybId = Symbol->ID;
		auto& Type = Symbol->VarType;

		AddDependencyToCurrentFile(Symbol);


		IRInstruction* LoadV = BuildMember_AsValue(V);

		#define Set_CompoundU(x) \
			switch (node.CompoundOp->Type) \
			{ \
			case TokenType::CompoundAdd: \
				_LastExpressionField=LookingAtIRBlock->NewAdd(LoadV,_LastExpressionField);\
				break; \
			case TokenType::CompoundSub:\
				_LastExpressionField=LookingAtIRBlock->NewSub(LoadV,_LastExpressionField); \
				break; \
			case TokenType::CompoundMult:\
				_LastExpressionField = LookingAtIRBlock->NewUMul(LoadV, _LastExpressionField);\
			    break; \
			case TokenType::CompoundDiv:\
				_LastExpressionField = LookingAtIRBlock->NewUDiv(LoadV, _LastExpressionField);\
			    break; \
			default:\
				throw std::exception("Bad Op"); \
				break; \
			}\

		#define Set_CompoundS(x) \
		switch (node.CompoundOp->Type) \
		{ \
		case TokenType::CompoundAdd: \
				_LastExpressionField=LookingAtIRBlock->NewAdd(LoadV,_LastExpressionField);\
				break; \
			case TokenType::CompoundSub:\
				_LastExpressionField=LookingAtIRBlock->NewSub(LoadV,_LastExpressionField); \
				break; \
			case TokenType::CompoundMult:\
				_LastExpressionField = LookingAtIRBlock->NewSMul(LoadV, _LastExpressionField);\
			    break; \
			case TokenType::CompoundDiv:\
				_LastExpressionField = LookingAtIRBlock->NewSDiv(LoadV, _LastExpressionField);\
			    break; \
			default:\
				throw std::exception("Bad Op"); \
				break; \
		}\


		switch (V.Type._Type)
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
			throw std::exception("not added");
		};
		break;

		case TypesEnum::sIntPtr:
		{
			throw std::exception("not added");
		};
		break;



		break;

		default:
			throw std::exception("Bad Op");
			break;
		}

		BuildMember_Reassignment(V, Type, _LastExpressionField); 
	}
}
void SystematicAnalysis::OnExpressionTypeNode(const Node* node)
{
	switch (node->Get_Type())
	{
	case NodeType::BinaryExpressionNode:OnExpressionNode(*BinaryExpressionNode::As(node));break;
	case NodeType::ValueExpressionNode:OnExpressionNode(*ValueExpressionNode::As(node)); break;
	case NodeType::CastNode:OnExpressionNode(*CastNode::As(node)); break;
	case NodeType::IndexedExpresionNode:OnExpressionNode(*IndexedExpresionNode::As(node)); break;
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
			NumberliteralNode* num = NumberliteralNode::As(node.Value.get());
#define Set_NumberliteralNodeU(x) \
			UInt##x V; \
			ParseHelper::ParseStringToUInt##x(Str, V); \
			_LastExpressionField = LookingAtIRBlock->NewLoad(V);\

#define Set_NumberliteralNodeS(x) \
			Int##x V; \
			ParseHelper::ParseStringToInt##x(Str, V); \
			_LastExpressionField = LookingAtIRBlock->NewLoad(V);\


			auto& lookT = Get_LookingForType();
			TypesEnum NewEx;
			if (lookT._Type == TypesEnum::Var)
			{
				NewEx = TypesEnum::sInt32;
			}
			else
			{
				NewEx = (IsfloatType(lookT) || IsIntType(lookT)) ? lookT._Type : TypesEnum::sInt32;
			}

			if (passtype == PassType::BuidCode)
			{
				auto& Str = num->Token->Value._String;



				switch (NewEx)
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
					_LastExpressionField = IR_Load_UIntptr(V);
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
					_LastExpressionField = IR_Load_SIntptr(V);
					break;
				};


				case TypesEnum::float32:
				{
					Int32 V;
					ParseHelper::ParseStringToInt32(Str, V); 
					_LastExpressionField = LookingAtIRBlock->NewLoad((float32)V);
					break;
				};
				case TypesEnum::float64:
				{
					Int64 V;
					ParseHelper::ParseStringToInt64(Str, V);
					_LastExpressionField = LookingAtIRBlock->NewLoad((float64)V);
					break;
				};
				default:
					throw std::exception("not added");
					break;
				}
			}
			

			LastExpressionType.SetType(NewEx);
			LastLookedAtToken = num->Token;
		}
		break;
		case NodeType::BoolliteralNode:
		{
			BoolliteralNode* num = BoolliteralNode::As(node.Value.get());
			
			if (passtype == PassType::BuidCode)
			{
				_LastExpressionField = LookingAtIRBlock->NewLoad(num->Get_Value());
			}
			LastExpressionType.SetType(TypesEnum::Bool);
			LastLookedAtToken = num->Token;
		}
		break;
		case NodeType::CharliteralNode:
		{
			CharliteralNode* num = CharliteralNode::As(node.Value.get());

			if (passtype == PassType::BuidCode)
			{
				String V;
				bool ItWorked = !ParseHelper::ParseCharliteralToChar(num->Token->Value._String,V);
				

				_LastExpressionField = LookingAtIRBlock->NewLoad((char)V.front());
			}
			LastExpressionType.SetType(TypesEnum::Char);
			LastLookedAtToken = num->Token;
		}
		break;
		case NodeType::FloatliteralNode:
		{
			FloatliteralNode* num = FloatliteralNode::As(node.Value.get());
			auto& lookT = Get_LookingForType();
			if (passtype == PassType::BuidCode)
			{	
				
				switch (lookT._Type)
				{
				case TypesEnum::float32:
				{
					float32 V;
					bool ItWorked = ParseHelper::ParseStringTofloat32(num->Token->Value._String, V);

					_LastExpressionField = LookingAtIRBlock->NewLoad(V);
					break;
				}
				case TypesEnum::float64:
				{
					float64 V;
					bool ItWorked = ParseHelper::ParseStringTofloat64(num->Token->Value._String, V);
					_LastExpressionField = LookingAtIRBlock->NewLoad(V);
					break;
				}
				default:
					throw std::exception("not added");
					break;
				}
				
			}

			TypesEnum NewEx;
			if (lookT._Type == TypesEnum::Var)
			{
				NewEx = TypesEnum::float32;
			}
			else
			{
				NewEx = (IsfloatType(lookT)) ? lookT._Type : TypesEnum::float32;
			}


			LastExpressionType.SetType(NewEx);
			LastLookedAtToken = num->Token;
		}
		break;
		case NodeType::StringliteralNode:
		{
			StringliteralNode* nod = StringliteralNode::As(node.Value.get());

			TypeSymbol CStringType;//umut char[&]

			CStringType.SetType(TypesEnum::Char);
			CStringType.SetAsAddressArray();
			CStringType.SetAsimmutable();

			LastExpressionType = CStringType;
		}break;
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
					_LastExpressionField = LookingAtIRBlock->NewLoad((UInt8)TypeSize);
					break;
				case TypesEnum::sInt16:
				case TypesEnum::uInt16:
					_LastExpressionField = LookingAtIRBlock->NewLoad((UInt16)TypeSize);
					break;
				case TypesEnum::sInt32:
				case TypesEnum::uInt32:
					_LastExpressionField = LookingAtIRBlock->NewLoad((UInt32)TypeSize);
					break;
				case TypesEnum::sInt64:
				case TypesEnum::uInt64:
					_LastExpressionField = LookingAtIRBlock->NewLoad((UInt64)TypeSize);
					break;
				default:
					Type.SetType(TypesEnum::uIntPtr);
					_LastExpressionField = IR_Load_UIntptr(TypeSize);
					break;
				}
			}

			LastExpressionType = Type;
		}
		break;
		case NodeType::NewExpresionNode:
		{
			NewExpresionNode* nod = NewExpresionNode::As(node.Value.get());
			OnNewNode(nod);
		}
		break;
		case NodeType::ParenthesesExpresionNode:
		{
			ParenthesesExpresionNode* nod = ParenthesesExpresionNode::As(node.Value.get());
			OnExpressionTypeNode(nod->Expression.Value.get());
		}
		break;
		default:
			throw std::exception("not added");
			break;
		}
	}
}

void SystematicAnalysis::OnNewNode(NewExpresionNode* nod)
{
	TypeSymbol Type;
	Convert(nod->Type, Type);
	bool IsArray = nod->Arrayexpression.Value.get();

	if (passtype == PassType::FixedTypes)
	{
		if (IsArray)
		{
			TypeSymbol UintptrType = TypeSymbol();
			UintptrType.SetType(TypesEnum::uIntPtr);

			LookingForTypes.push(UintptrType);
			OnExpressionTypeNode(nod->Arrayexpression.Value.get());

			if (!CanBeImplicitConverted(LastExpressionType, UintptrType))
			{
				auto  Token = LastLookedAtToken;
				LogCantCastImplicitTypes(Token, LastExpressionType, UintptrType, true);
			}

			LookingForTypes.pop();

		}
	
		auto Func = GetFunc(Type, nod->Parameters);
		FuncToSyboID[nod] = Func;



		if (IsArray)
		{
			Type.SetAsAddressArray();
		}
		else
		{
			Type.SetAsAddress();
		}
	}

	if (passtype == PassType::BuidCode)
	{
	

		auto Func = FuncToSyboID.at(nod);
		auto& ValuePars = nod->Parameters;
		

		UAddress TypeSize;
		GetSize(Type, TypeSize);

		
		if (IsArray)
		{

			TypeSymbol UintptrType = TypeSymbol();
			UAddress UintptrSize;
			{
				UintptrType.SetType(TypesEnum::uIntPtr);
				GetSize(UintptrType, UintptrSize);
			}
			bool TypeHaveDestructor = HasDestructor(Type);

			LookingForTypes.push(UintptrType);


			OnExpressionTypeNode(nod->Arrayexpression.Value.get());

			auto Ex0 = _LastExpressionField;
			DoImplicitConversion(Ex0, LastExpressionType, UintptrType);

			Ex0 = _LastExpressionField;


			auto SizeV = IR_Load_UIntptr(TypeSize);//UintptrSize is for the array length for Drop 



			auto DataSize = Build_Mult_uIntPtr(Ex0, SizeV);//uintptr 
			//malloc(sizeof(Type) * ArrayExpression); 


			if (TypeHaveDestructor)
			{
				Build_Increment_uIntPtr(DataSize,UintptrSize);//Make room for Size on the Ptr
			}

			auto MallocPtr = _LastExpressionField = LookingAtIRBlock->NewMallocCall(DataSize);

			if (TypeHaveDestructor)
			{
				LookingAtIRBlock->NewDereferenc_Store(MallocPtr, Ex0);//set Size.
				Build_Increment_uIntPtr(MallocPtr,UintptrSize);//move pointer

			}

			//Call default on every

			//our index
			auto Indexir = IR_Load_UIntptr(0);

			size_t JumpLabel = LookingAtIRBlock->GetIndex();
			auto Cmpbool = LookingAtIRBlock->NewC_Equalto(Indexir, Ex0);


			auto JumpIns = LookingAtIRBlock->NewConditionalJump(Cmpbool, NullUInt64);

			auto OffsetIr = LookingAtIRBlock->New_Index_Vetor(MallocPtr, Indexir, SizeV);

			//loop on every
			bool IsPrimitiveType = IsPrimitive(Type);

			if (!IsPrimitiveType) 
			{
				Func.ThisPar = Get_FuncInfo::ThisPar_t::PushFromLast;
				_LastExpressionField = OffsetIr;
			}

			DoFuncCall(Type, Func, ValuePars);

			if (IsPrimitiveType)
			{
				LookingAtIRBlock->NewDereferenc_Store(OffsetIr, _LastExpressionField);
			}

			LookingAtIRBlock->New_Increment(Indexir);//index++



			LookingAtIRBlock->NewJump(JumpLabel);

			size_t ThisJumpLable = LookingAtIRBlock->GetIndex();
			LookingAtIRBlock->UpdateConditionaJump(JumpIns, Cmpbool, ThisJumpLable);

			_LastExpressionField = MallocPtr;

			LookingForTypes.pop();
		}
		else
		{
			auto SizeIR = IR_Load_UIntptr(TypeSize);
			auto MallocPtr =  LookingAtIRBlock->NewMallocCall(SizeIR);
			_LastExpressionField = MallocPtr;
			
			
			//Call ObjectNew
			if (IsPrimitive(Type)) 
			{
				DoFuncCall(Type, Func, ValuePars);
				LookingAtIRBlock->NewDereferenc_Store(MallocPtr, _LastExpressionField);
			}
			else
			{
				Func.ThisPar = Get_FuncInfo::ThisPar_t::PushFromLast;
				DoFuncCall(Type, Func, ValuePars);
			}
			_LastExpressionField = MallocPtr;
			
		}
	
	}

	


	LastExpressionType = Type;
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
			auto& ValuePars = nod->Fields;



			DoFuncCall(Type, Func, ValuePars);


		}
	}

	LastExpressionType = Type;
}

void SystematicAnalysis::DoFuncCall(const TypeSymbol& Type, const Get_FuncInfo& Func, ValueParametersNode& ValuePars)
{
	String B = ToString(Type);
	Token T;
	T.Type = TokenType::Name;
	T.Value._String = B;


	ScopedNameNode Tep;
	ScopedName V;
	V.token = &T;

	Tep.ScopedName.push_back(std::move(V));

	DoFuncCall(Func, Tep, ValuePars);
}

void SystematicAnalysis::OnReadVariable(const ReadVariableNode& nod)
{

	if (passtype != PassType::BuidCode) 
	{
		GetMemberTypeSymbolFromVar_t V;
		if (!GetMemberTypeSymbolFromVar(nod.VariableName, V))
		{
			return;
		}
		LastExpressionType = V.Type;
	}
	

	auto FToken = nod.VariableName.ScopedName.front().token;

	Symbol* Symbol;
	auto Token = nod.VariableName.ScopedName.back().token;
	if (FToken->Type == TokenType::KeyWorld_This)
	{
		if (_ClassStack.size() == 0)
		{
			CantUseThisKeyWordHereError(FToken);
			return;
		}


		if (!IsInThisFuncCall())
		{
			LogCantUseThisInStaticFunction(FToken);
		}

		
		auto& Func = _FuncStack.back();
		auto ObjectType = Func.Pointer->GetObjectForCall();

		auto objecttypesyb = GetSymbol(*ObjectType);
		ClassInfo* V = objecttypesyb->Get_Info<ClassInfo>();

		
		Symbol = GetSymbol(*ObjectType);
			
		goto DoStuff;
		
	}


	auto Str = FToken->Value._String;
	Symbol = GetSymbol(Str,SymbolType::Varable_t);
	
	
	auto Info = LogTryReadVar(Str, Token, Symbol);
	if (Info.CantFindVar)
	{
		return;
	}
	if (Symbol->Type == SymbolType::StackVarable)
	{
		size_t declaredpos = (size_t)Symbol->NodePtr;//just tep solution
		if (declaredpos > FToken->OnPos)
		{
			LogUseingVarableBeforDeclared(FToken);
		}
	}

	if (!Info.VarIsInvalid)
	{
	DoStuff:
		SymbolID sybId = Symbol->ID;
		if (passtype == PassType::BuidCode)
		{
			AddDependencyToCurrentFile(Symbol);

			auto& LookForT = Get_LookingForType();

			

			GetMemberTypeSymbolFromVar_t V;
			if (!GetMemberTypeSymbolFromVar(nod.VariableName, V))
			{
				return;
			}

			
			bool LookIsAddress = LookForT.IsAddress() || LookForT.IsAddressArray();
			bool AmIsAddress = V.Type.IsAddress();

			bool AsPointer = LookForT.IsAddress();

			//


			if (LookIsAddress == true && AmIsAddress == true)
			{
				_LastExpressionField = BuildMember_GetValue(V);
			}
			else if (LookIsAddress == false && AmIsAddress == false)
			{
				_LastExpressionField = BuildMember_AsValue(V);
			}
			else if (LookIsAddress == true && AmIsAddress == false)
			{
				_LastExpressionField = BuildMember_AsPointer(V);
			}
			else if (LookIsAddress == false && AmIsAddress == true)
			{
				_LastExpressionField = BuildMember_AsValue(V);
			}
			else
			{
				throw std::exception("bad path");
			}

			LastExpressionType = V.Type;
		}
	}

	LastLookedAtToken = Token;
}

Byte SystematicAnalysis::OperatorPrecedenceValue(const Node* node)
{
	if (node->Get_Type() == NodeType::ValueExpressionNode) 
	{ 
		const ValueExpressionNode* nod = ValueExpressionNode::As(node);
		
		if (nod->Value->Get_Type() == NodeType::ParenthesesExpresionNode)
		{
			return 8;
		}
	
	}

	if (node->Get_Type() == NodeType::BinaryExpressionNode)
	{
		const BinaryExpressionNode* nod = BinaryExpressionNode::As(node);

		auto V = nod->BinaryOp->Type;
		return OperatorPrecedence(V);
	}


	return 0;
}

Byte SystematicAnalysis::OperatorPrecedence(TokenType V)
{
	//https://en.cppreference.com/w/c/language/operator_precedence
	
	//the biger number will have a higher precedence
	switch (V)
	{
	case TokenType::modulo:
	case TokenType::forwardslash:
	case TokenType::star:
		return 6;

	case TokenType::Not:
	case TokenType::bitwise_not:

	case TokenType::plus:
	case TokenType::minus:
	default:
		return 0;
	}
}

bool SystematicAnalysis::SwapForOperatorPrecedence(const Node* nodeA, const Node* nodeB)
{
	return OperatorPrecedenceValue(nodeA) < OperatorPrecedenceValue(nodeB);
}

void SystematicAnalysis::OnExpressionNode(const BinaryExpressionNode& node)
{
	auto Ex0node = node.Value0.Value.get();
	auto Ex1node = node.Value1.Value.get();

	if (passtype != PassType::GetTypes && 
		(
			SwapForOperatorPrecedence(Ex0node,Ex1node) && SwapForOperatorPrecedence(&node, Ex1node)//i have no clue why this works
		)
		)
	{
		std::swap(Ex0node, Ex1node);
	}

	

	if (LookingForTypes.size() && LookingForTypes.top()._Type != TypesEnum::Var)
	{
		TypeSymbol V; V.SetType(TypesEnum::Any);
		LookingForTypes.push(V);
	}
	else
	{
		LookingForTypes.push(LookingForTypes.top());
	}

	BinaryExpressionNode_Data* Data;
	if (passtype == PassType::BuidCode)
	{
		Data = &BinaryExpressionNode_Datas.at(&node);
		LookingForTypes.top() = Data->Op1;
	}

	OnExpressionTypeNode(Ex1node);
	auto Ex0 = _LastExpressionField;
	auto Ex0Type = LastExpressionType;
	
	if (passtype == PassType::BuidCode)
	{
		LookingForTypes.top() = Data->Op0;
	}
	

	OnExpressionTypeNode(Ex0node);
	auto Ex1 = _LastExpressionField;
	auto Ex1Type = LastExpressionType;


	LookingForTypes.pop();

	if (passtype == PassType::FixedTypes)
	{
		auto BinaryOp = node.BinaryOp;

		if (!HasBinaryOverLoadWith(Ex0Type, BinaryOp->Type, Ex1Type))
		{
			LogCantFindBinaryOpForTypes(BinaryOp, Ex0Type, Ex1Type);
		}

		auto Op = node.BinaryOp->Type;
		LastExpressionType = BinaryExpressionShouldRurn(Op, Ex0Type);

		BinaryExpressionNode_Data V;
		V.Op0 = Ex0Type;
		V.Op1 = Ex1Type;
		
		
		//all float bool int types
		if (V.IsBuitIn())
		{
			V.Op0._IsAddress = false;
			V.Op1._IsAddress = false;
		}
		BinaryExpressionNode_Datas[&node] = V;

		
	}

	if (passtype == PassType::BuidCode)
	{
	
		auto& Type = Ex0Type;
		auto Op = node.BinaryOp->Type;
#define BindaryBuildU(x) switch (Op) \
		{\
		case TokenType::plus:_LastExpressionField=LookingAtIRBlock->NewAdd(Ex1, Ex0);break;\
		case TokenType::minus:_LastExpressionField=LookingAtIRBlock->NewSub(Ex1, Ex0);break;\
		case TokenType::star:_LastExpressionField=LookingAtIRBlock->NewUMul(Ex1, Ex0);break; \
		case TokenType::forwardslash:_LastExpressionField=LookingAtIRBlock->NewUDiv(Ex1, Ex0);break; \
		case TokenType::equal_Comparison:_LastExpressionField = LookingAtIRBlock->NewC_Equalto(Ex1, Ex0); break; \
		case TokenType::Notequal_Comparison:_LastExpressionField = LookingAtIRBlock->NewC_NotEqualto(Ex1, Ex0);break; \
		case TokenType::greaterthan:_LastExpressionField = LookingAtIRBlock->NewUGreaterThan(Ex1, Ex0); break; \
		case TokenType::lessthan:_LastExpressionField = LookingAtIRBlock->NewULessThan(Ex1, Ex0); break; \
		case TokenType::greater_than_or_equalto:_LastExpressionField = LookingAtIRBlock->NewUGreaterThanOrEqual(Ex1, Ex0); break; \
		case TokenType::less_than_or_equalto:_LastExpressionField = LookingAtIRBlock->NewULessThanOrEqual(Ex1, Ex0); break; \
		default:\
			throw std::exception("not added");\
			break;\
		}\


#define BindaryBuildS(x) switch (Op) \
			{\
			case TokenType::plus:_LastExpressionField=LookingAtIRBlock->NewAdd(Ex1, Ex0);break;\
			case TokenType::minus:_LastExpressionField=LookingAtIRBlock->NewSub(Ex1, Ex0);break;\
			case TokenType::star:_LastExpressionField=LookingAtIRBlock->NewSMul(Ex1, Ex0);break; \
			case TokenType::forwardslash:_LastExpressionField=LookingAtIRBlock->NewSDiv(Ex1, Ex0);break; \
			case TokenType::equal_Comparison:_LastExpressionField = LookingAtIRBlock->NewC_Equalto(Ex1, Ex0); break; \
			case TokenType::Notequal_Comparison:_LastExpressionField = LookingAtIRBlock->NewC_NotEqualto(Ex1, Ex0); break; \
			case TokenType::greaterthan:_LastExpressionField = LookingAtIRBlock->NewSGreaterThan(Ex1, Ex0); break; \
			case TokenType::lessthan:_LastExpressionField = LookingAtIRBlock->NewSLessThan(Ex1, Ex0); break; \
			case TokenType::greater_than_or_equalto:_LastExpressionField = LookingAtIRBlock->NewSGreaterThanOrEqual(Ex1, Ex0); break; \
			case TokenType::less_than_or_equalto:_LastExpressionField = LookingAtIRBlock->NewSLessThanOrEqual(Ex1, Ex0); break; \
			default:\
				throw std::exception("not added"); \
				break; \
			}\

		switch (Type._Type)
		{
		case TypesEnum::uInt8:BindaryBuildU(8); break;
		case TypesEnum::uInt16:BindaryBuildU(16); break;
		case TypesEnum::uInt32:BindaryBuildU(32); break;
		case TypesEnum::uInt64:BindaryBuildU(64); break;


		case TypesEnum::sInt8:BindaryBuildS(8); break;
		case TypesEnum::sInt16:BindaryBuildS(16); break;
		case TypesEnum::sInt32:BindaryBuildS(32); break;
		case TypesEnum::sInt64:BindaryBuildS(64); break;

		case TypesEnum::float32:BindaryBuildS(32); break;
		case TypesEnum::float64:BindaryBuildS(64); break;


		case TypesEnum::Bool:
			switch (Op) 
			{
			case TokenType::equal_Comparison:_LastExpressionField = LookingAtIRBlock->NewC_Equalto(Ex1, Ex0); break; 
			case TokenType::Notequal_Comparison:_LastExpressionField = LookingAtIRBlock->NewC_NotEqualto(Ex1, Ex0); break; 
			case TokenType::logical_and:_LastExpressionField = LookingAtIRBlock->NewlogicalAnd(Ex1, Ex0); break;
			case TokenType::logical_or:_LastExpressionField = LookingAtIRBlock->NewlogicalOr(Ex1, Ex0); break;
			default:
				throw std::exception("not added"); 
				break;
			}
			break;
		case TypesEnum::Char:
			switch (Op)
			{
			case TokenType::equal_Comparison:_LastExpressionField = LookingAtIRBlock->NewC_Equalto(Ex1, Ex0); break;
			case TokenType::Notequal_Comparison:_LastExpressionField = LookingAtIRBlock->NewC_NotEqualto(Ex1, Ex0); break;
			default:
				throw std::exception("not added");
				break;
			}
			break;
		default:
			break;
		}
	}
}
TypeSymbol SystematicAnalysis::BinaryExpressionShouldRurn(TokenType Op, const TypeSymbol& Ex0Type)
{
	TypeSymbol V;
	switch (Op)
	{
	case TokenType::plus:
	case TokenType::minus:
	case TokenType::star:
	case TokenType::forwardslash:
		V = Ex0Type;
		break;
	case TokenType::logical_and:
	case TokenType::logical_or:
	case TokenType::equal_Comparison:
	case TokenType::Notequal_Comparison:
		V.SetType(TypesEnum::Bool);
		break;
	default:
		V.SetType(TypesEnum::Void);
		break;
	}
	return V;
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
		DoExplicitlConversion(_LastExpressionField, LastExpressionType, ToTypeAs);

		LastExpressionType = ToTypeAs;
	}
	LookingForTypes.pop();
}
void SystematicAnalysis::OnExpressionNode(const IndexedExpresionNode& node)
{
	if (LookingForTypes.size() && LookingForTypes.top()._Type == TypesEnum::Var)
	{
		TypeSymbol V; V.SetType(TypesEnum::Any);
		LookingForTypes.push(V);
	}
	else
	{
		LookingForTypes.push(LookingForTypes.top());
	}

	if (passtype == PassType::GetTypes)
	{
		OnExpressionTypeNode(node.SourceExpression.Value.get());
		OnExpressionTypeNode(node.IndexExpression.Value.get());
	}

	if (passtype == PassType::FixedTypes)
	{

		OnExpressionTypeNode(node.SourceExpression.Value.get());
		TypeSymbol SourcType = LastExpressionType;

		
		TypeSymbol gesstype;
		gesstype.SetType(TypesEnum::Any);
		if (SourcType.IsAddressArray())
		{
			gesstype.SetType(TypesEnum::uIntPtr);
		}


		LookingForTypes.push(gesstype);


		OnExpressionTypeNode(node.IndexExpression.Value.get());
		TypeSymbol IndexType = LastExpressionType;


		LookingForTypes.pop();

		if (!HasIndexedOverLoadWith(SourcType, IndexType))
		{
			auto  Token = LastLookedAtToken;
			LogCantBeIndexWithType(Token, SourcType, IndexType);
		}



		TypeSymbol lookingfor = LookingForTypes.top();


		IndexedExpresion_Data V;
		V.Op0 = SourcType;
		V.Op1 = IndexType;


		//all float bool int types
		if (V.IsBuitIn())
		{
			V.Op0._IsAddress = true;
			V.Op1._IsAddress = false;

			if (lookingfor.IsAddressArray())
			{
				lookingfor = SourcType;
				LastExpressionType = lookingfor;
			}
			else
			{
				lookingfor = SourcType;
				lookingfor.SetAsAddress();
				lookingfor._IsAddressArray = false;

				LastExpressionType = lookingfor;
			}

		}

		IndexedExpresion_Datas[&node] = V;

	}


	if (passtype == PassType::BuidCode)
	{
		auto& Data = IndexedExpresion_Datas[&node];

		if (Data.IsBuitIn())
		{
			LookingForTypes.push(Data.Op0);
			OnExpressionTypeNode(node.SourceExpression.Value.get());
			LookingForTypes.pop();

			auto Pointer = _LastExpressionField;
			auto SourcType = LastExpressionType;

			LookingForTypes.push(Data.Op1);
			OnExpressionTypeNode(node.IndexExpression.Value.get());
			LookingForTypes.pop();

			auto IndexField = _LastExpressionField;

			TypeSymbol IndexedObjectPointer = Data.Op0;
			IndexedObjectPointer._IsAddressArray = false;
			IndexedObjectPointer._IsAddress = false;

			UAddress V; GetSize(IndexedObjectPointer, V);

			_LastExpressionField = LookingAtIRBlock->New_Index_Vetor(Pointer, IndexField,LookingAtIRBlock->NewLoad(V));

			TypeSymbol lookingfor = LookingForTypes.top();
			{
				if (lookingfor.IsAddressArray())
				{
					lookingfor = SourcType;
					LastExpressionType = lookingfor;
				}
				else
				{
					lookingfor = SourcType;
					lookingfor.SetAsAddress();
					lookingfor._IsAddressArray = false;

					LastExpressionType = lookingfor;
				}
			}

			LastExpressionType = lookingfor;
		}
		else
		{
			throw std::exception("not added");
		}

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
		
		if (Info.Func)
		{
			LastExpressionType = Info.Func->Ret;
		}
	}
	else if (passtype == PassType::BuidCode)
	{
		auto& SybID = FuncToSyboID.at(&node);
		DoFuncCall(SybID, node.FuncName, node.Parameters);
	}
}
void SystematicAnalysis::OnDropStatementNode(const DropStatementNode& node)
{
	if (passtype == PassType::BuidCode)
	{

		auto TypeToPush = TypeSymbol();
		TypeToPush.SetAsAddress();

		LookingForTypes.push(TypeToPush);
	}

	OnExpressionTypeNode(node.expression.Value.get());

	if (passtype == PassType::BuidCode)
	{
		LookingForTypes.pop();
	}

	auto Ex0 = _LastExpressionField;
	auto Ex0Type = LastExpressionType;
	if (passtype == PassType::FixedTypes)
	{

		if (!(Ex0Type.IsAddress() || Ex0Type.IsAddressArray()))
		{
			auto Token = LastLookedAtToken;
			ExpressionMustbeAnLocationValueError(Token, Ex0Type);
		}
	}
	if (passtype == PassType::BuidCode)
	{
		
		bool TypeHaveDestructor = HasDestructor(Ex0Type);
		if (Ex0Type.IsAddressArray())
		{
			Ex0Type._IsAddressArray = false;
			

			if (TypeHaveDestructor)
			{
				UAddress UintptrSize;
				TypeSymbol UintptrType;
				{
					UintptrType = TypeSymbol();
					UintptrType.SetType(TypesEnum::uIntPtr);
					GetSize(UintptrType, UintptrSize);
				}
				UAddress TypeObjectSize;
				TypeSymbol TypeOfArr;
				{
					TypeOfArr = Ex0Type;
					TypeOfArr._IsAddressArray = false;
					GetSize(TypeOfArr, TypeObjectSize);
				}

				auto StartArrPointer = LookingAtIRBlock->NewLoad(Ex0);
				Build_Decrement_uIntPtr(Ex0, UintptrSize);//get the size the object
				auto ArrSize = LookingAtIRBlock->NewLoad_Dereferenc(Ex0,ConvertToIR(UintptrType));




				//Call default on every

				//our index
				auto Indexir = IR_Load_UIntptr(0);

				size_t JumpLabel = LookingAtIRBlock->GetIndex();
				auto Cmpbool = LookingAtIRBlock->NewC_Equalto(Indexir, Ex0);


				auto JumpIns = LookingAtIRBlock->NewConditionalJump(Cmpbool, NullUInt64);

				auto OffsetIr = LookingAtIRBlock->New_Index_Vetor(StartArrPointer, Indexir, IR_Load_UIntptr(TypeObjectSize));

				//loop on every
				bool IsPrimitiveType = IsPrimitive(TypeOfArr);


				{
					ObjectToDrop Data;
					Data._Object = OffsetIr;
					Data.Type = TypeOfArr;
					Data.DropType = ObjectToDropType::IRInstructionNoMod;
					DoDestructorCall(Data);//call on Object
				}


				LookingAtIRBlock->New_Increment(Indexir);//index++



				LookingAtIRBlock->NewJump(JumpLabel);

				size_t ThisJumpLable = LookingAtIRBlock->GetIndex();
				LookingAtIRBlock->UpdateConditionaJump(JumpIns, Cmpbool, ThisJumpLable);


				LookingAtIRBlock->NewFreeCall(Ex0);
			}
			else
			{
				LookingAtIRBlock->NewFreeCall(Ex0);
			}

			Ex0Type._IsAddressArray = true;
		}
		else 
		{
			
			if (TypeHaveDestructor)
			{
				TypeSymbol tep = Ex0Type;
				if (tep.IsAddress())
				{
					tep._IsAddress = false;//DoDestructorCall will not do it if is IsPrimitive
				}
				
				ObjectToDrop Data;
				Data._Object = Ex0;
				Data.Type = tep;
				Data.DropType = ObjectToDropType::IRInstructionNoMod;
				DoDestructorCall(Data);//call on Object
			}
			
			LookingAtIRBlock->NewFreeCall(Ex0);
			
			
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
		
		auto& Syb = AddSybol(SymbolType,FuncStr, ScopeHelper::GetNameFromFullName(FuncStr));
		
		
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
		{

		}
		break;
		case ClassType::Enum:
		{

		}
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
		Item.Type = ConvertToTypeInfo(node.Type);
		Item.offset = VClass.Size;
		VClass.Size += Size;
	}
	
}

ReflectionTypeInfo SystematicAnalysis::ConvertToTypeInfo(const TypeSymbol& Type)
{
	ReflectionTypeInfo r;
	r.FullNameType = ToString(Type);
	return r;
}

TypeSymbol SystematicAnalysis::Convert(const ReflectionTypeInfo& Type)
{
	return TypeSymbol();
}
bool AreSameimmutable(const TypeSymbol& TypeA, const TypeSymbol& TypeB)
{
	return 	TypeA.IsAddress() == TypeB.IsAddress() &&
		TypeA.IsAddressArray() == TypeB.IsAddressArray() &&
		TypeA.Isimmutable() == TypeB.Isimmutable();
}
bool SystematicAnalysis::AreTheSame(const TypeSymbol& TypeA, const TypeSymbol& TypeB)
{
	return AreTheSameWithOutimmutable(TypeA, TypeB) && AreSameimmutable(TypeA, TypeB);
}
bool SystematicAnalysis::AreTheSameWithOutimmutable(const TypeSymbol& TypeA, const TypeSymbol& TypeB)
{
	if (TypeA.IsAddressArray() != TypeB.IsAddressArray())
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

		Symbol& TypeOne = *GetSymbol(TypeA);
		Symbol& TypeTwo = *GetSymbol(TypeB);
		if (TypeOne.Type == SymbolType::Func_ptr && TypeTwo.Type == SymbolType::Func_ptr)
		{
			FuncInfo* F1 = TypeOne.Get_Info<FuncInfo>();
			FuncInfo* F2 = TypeTwo.Get_Info<FuncInfo>();
			if (F1->Pars.size() != F2->Pars.size())
			{
				return false;
			}
			if (!AreTheSameWithOutimmutable(F1->Ret, F2->Ret))
			{
				return false;
			}

			for (size_t i = 0; i < F1->Pars.size(); i++)
			{
				auto& Item1 = F1->Pars[i];
				auto& Item2 = F2->Pars[i];
				if (!AreTheSameWithOutimmutable(Item1, Item2))
				{
					return false;
				}
			}
			return true;
		}

	}
	else if ((IsPrimitive(TypeA) && IsPrimitive(TypeB)) && TypeA._Type == TypeB._Type
		)
	{
		return true;
	}



	return false;
}
bool SystematicAnalysis::HasBinaryOverLoadWith(const TypeSymbol& TypeA, TokenType BinaryOp, const TypeSymbol& TypeB)
{

	if (AreTheSameWithOutimmutable(TypeA, TypeB))
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
			|| BinaryOp == TokenType::bitwise_or;

		bool IslogicalOperator = BinaryOp == TokenType::logical_and
			|| BinaryOp == TokenType::logical_or;

		if (IsIntType(TypeA))
		{
			return IsMathOp || IsSameValueComparisonOp || IsMathValueComparisonOp || Isbitwise;
		}
		if (IsfloatType(TypeA))
		{
			return IsMathOp || IsSameValueComparisonOp || IsMathValueComparisonOp;
		}

		if (TypeA._Type == TypesEnum::Char)
		{
			return IsSameValueComparisonOp;
		}
		if (TypeA._Type == TypesEnum::Bool)
		{
			return IsSameValueComparisonOp || IslogicalOperator;
		}

	}
	return false;
}
bool SystematicAnalysis::HasCompoundOverLoadWith(const TypeSymbol& TypeA, TokenType BinaryOp, const TypeSymbol& TypeB)
{
	if (AreTheSameWithOutimmutable(TypeA, TypeB))
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
bool SystematicAnalysis::HasIndexedOverLoadWith(const TypeSymbol& TypeA, const TypeSymbol& TypeB)
{
	if (TypeA.IsAddressArray())
	{
		return IsUIntType(TypeB);
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

	case TypesEnum::float32:r = float32TypeName;	break;
	case TypesEnum::float64:r = float64TypeName;	break;
	case TypesEnum::CustomType:
	{
		auto& Syb = *GetSymbol(Type._CustomTypeSymbol);
		if (Syb.Type == SymbolType::Func
			|| Syb.Type == SymbolType::GenericFunc)
		{
			r = ToString(Syb.VarType);
		}
		else if (Syb.Type == SymbolType::Type_alias)
		{
			r = ToString(Syb.VarType);
		}
		else if (Syb.Type == SymbolType::Func_ptr)
		{
			FuncPtrInfo* Funptr = (FuncPtrInfo*)Syb.Info.get();
			r += "&|[";

			for (auto& Item : Funptr->Pars)
			{
				r += ToString(Funptr->Ret);
				if (&Item != &Funptr->Pars.back())
				{
					r += ",";
				}
			}

			r += "] -> ";

			r += ToString(Funptr->Ret);
		}
		else
		{
			r = Syb.FullName;
		}
	}	break;
	case TypesEnum::Void:
		r = "void";	break;
	case TypesEnum::Any:
		r = "[any]";	
		break;
	case TypesEnum::Null:
		r = "[badtype]";	break;
	default:
		throw std::exception("bad Type");
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
		Out.SetType(TypesEnum::uIntPtr);
		break;
	case TokenType::KeyWorld_sintptr:
		Out.SetType(TypesEnum::sIntPtr);
		break;

	case TokenType::KeyWorld_Bool:
		Out.SetType(TypesEnum::Bool);
		break;
	case TokenType::KeyWorld_Char:
		Out.SetType(TypesEnum::Char);
		break;
	case TokenType::KeyWorld_float32:
		Out.SetType(TypesEnum::float32);
		break;
	case TokenType::KeyWorld_float64:
		Out.SetType(TypesEnum::float64);
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
		if (passtype == PassType::GetTypes) { return; }
		auto Name = V.Name.AsStringView();
		Symbol* SybV;
		if (V.Generic.Values.size())
		{
			SybV = GetSymbol(Name, SymbolType::Generic_class);
			if (SybV == nullptr)
			{
				auto Token = V.Name.Token;

				LogCantFindTypeError(Token, Name);
				return;
			}
			if (SybV->Type != SymbolType::Generic_class)
			{
				LogExpectedSymbolToBea(V.Name.Token, *SybV, SymbolType::Generic_class);
				return;
			}

			ClassInfo* CInfo = SybV->Get_Info<ClassInfo>();

			if (CInfo->_Generic.size() != V.Generic.Values.size())
			{
				LogCanIncorrectGenericCount(V.Name.Token, Name, V.Generic.Values.size(), CInfo->_Generic.size());
				return;
			}

			
			auto GenericInput = std::make_unique<Vector<TypeSymbol>>();//pointer must be unique so it cant be on the stack
			for (size_t i = 0; i < V.Generic.Values.size(); i++)
			{
				const auto& Tnode = V.Generic.Values[i];
				const auto& GenericInfo = CInfo->_Generic[i];
				TypeSymbol Type; 
				Convert(Tnode, Type);

				{
					bool InputTypeIsConstantExpression = false;

					auto TypeSyb = GetSymbol(Type);
					if (TypeSyb)
					{
						InputTypeIsConstantExpression = TypeSyb->Type == SymbolType::ConstantExpression;
					}

					if (InputTypeIsConstantExpression != GenericInfo.IsConstantExpression)
					{
						const Token* nodeToken = Tnode.Name.Token;
						const ClassNode* classnode = (const ClassNode*)SybV->NodePtr;
						auto& GenericNo = classnode->Generic.Values[i];
						if (InputTypeIsConstantExpression)
						{
							LogGenericInputWantsaExpressionNotType(nodeToken, GenericNo.Token->Value._String);
						}
						else
						{
							LogGenericInputWantsaExpressionNotType(nodeToken, GenericNo.Token->Value._String);
						}

						return;
					}	
				}

				
				GenericInput->push_back(Type);
			}
			
			String NewName = GetGenericFuncName(SybV, *GenericInput);
			auto FuncIsMade = GetSymbol(NewName, SymbolType::Class_Field);
			if (!FuncIsMade)
			{
				GenericTypeInstantiate(SybV, *GenericInput);

				TepFuncs.push_back({std::move(GenericInput)});
			}

			SybV = GetSymbol(NewName, SymbolType::Type);
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
			else if (SybV->Type == SymbolType::Hard_Type_alias)
			{
				Out.SetType(SybV->ID);
			}
			else
			{
				Out.SetType(SybV->ID);
			}
		}
	}break;
	case TokenType::internal_Constant_expression:
	{
		//note this can only happen in a generic substitution
		auto* node = V.node.get();
		if (_ConstantExpressionMap.count(node))
		{
			auto& item = _ConstantExpressionMap[node];
			Out.SetType(item);
		}
		else
		{
			SymbolID id = (SymbolID)node;
			auto& Syb = AddSybol(SymbolType::ConstantExpression, "?", "?");
			_Table.AddSymbolID(Syb, id);

			ConstantExpressionInfo* info = new ConstantExpressionInfo();
			info->Exnode = ExpressionNodeType::As(node);

			Syb.Info.reset(info);


			Out.SetType(id);
		}
	}
	break;
	default:
		throw std::exception("not added");
		break;
	}
	if (V.IsAddess) {Out._IsAddress = true;}
	if (V.IsAddessArray){Out._IsAddressArray = true;}
	if (V.Isimmutable){Out._Isimmutable = true;}
}
void SystematicAnalysis::DoSymbolRedefinitionCheck(const Symbol* Syb, const Token* Value)
{
	auto other = GetSymbol(Syb->FullName,Syb->Type);
	if (other)
	{
		LogSymbolRedefinition(Value,Syb);
	}
}
void SystematicAnalysis::DoSymbolRedefinitionCheck(const String_view FullName, SymbolType Type, const Token* Value)
{
	auto other = GetSymbol(FullName,Type);
	if (other)
	{
		LogSymbolRedefinition(Value, other);
	}
}
void SystematicAnalysis::DoSymbolRedefinitionCheck(const Symbol* Syb, const FuncInfo* Fvalue, const Token* Value)
{
	auto other = GetSymbol(Syb->FullName, Syb->Type);
	if (other != Syb)
	{
		bool Good = false;

		if (other->Type == SymbolType::Func)
		{
			FuncInfo* SybValue = other->Get_Info<FuncInfo>();


			/*if (!AreTheSameWithOutimmutable(Fvalue->Ret, SybValue->Ret))
			{
				goto GoodJump;
			}
			*/

			if (SybValue->Pars.size() == Fvalue->Pars.size())
			{

				for (size_t i = 0; i < Fvalue->Pars.size(); i++)
				{
					auto& V1 = Fvalue->Pars[i];
					auto& V2 = SybValue->Pars[i];
					if (!AreTheSameWithOutimmutable(V1, V2))
					{

						return;
					}
				}
				goto GoodJump;
			}
			else 
			{
				return;
			}
		}


		GoodJump:
		if (!Good) 
		{
			LogSymbolRedefinition(Value, Syb);
		}
	}
}

bool SystematicAnalysis::IsVaidType(TypeSymbol& Out)
{
	return false;
}
bool SystematicAnalysis::CanBeImplicitConverted(const TypeSymbol& TypeToCheck, const TypeSymbol& Type, bool ReassignMode )
{
	if (AreTheSameWithOutimmutable(TypeToCheck, Type)) 
	{ 
		bool V0 =IsimmutableRulesfollowed(TypeToCheck, Type);

		bool V1 = IsAddessAndLValuesRulesfollowed(TypeToCheck, Type, ReassignMode);

		return V0 && V1;
	}

	
	
	return false;
}
bool SystematicAnalysis::CanBeExplicitlyConverted(const TypeSymbol& TypeToCheck, const TypeSymbol& Type)
{
	if (CanBeImplicitConverted(TypeToCheck, Type)) { return true; }
	
	if (TypeToCheck._Type == TypesEnum::CustomType) 
	{
		Symbol& syb = *GetSymbol(TypeToCheck._CustomTypeSymbol);
		if (syb.Type == SymbolType::Hard_Type_alias)
		{
			if (AreTheSameWithOutimmutable(syb.VarType, Type))
			{
				return true;
			}
		}

	}

	if (IsIntType(TypeToCheck) && IsIntType(TypeToCheck)) { return true; }
	return false;
}
bool SystematicAnalysis::DoImplicitConversion(IRInstruction* Ex, const TypeSymbol ExType, const TypeSymbol& ToType)
{

	if (AreTheSame(ExType, ToType))
	{
		return true;
	}
	return false;
}
void SystematicAnalysis::DoExplicitlConversion(IRInstruction* Ex, const TypeSymbol ExType, const TypeSymbol& ToType)
{
	if (!DoImplicitConversion(Ex, ExType, ToType))
	{
		if (ExType._Type == TypesEnum::CustomType)
		{
			Symbol& syb = *GetSymbol(ExType._CustomTypeSymbol);
			if (syb.Type == SymbolType::Hard_Type_alias)
			{
				if (AreTheSameWithOutimmutable(syb.VarType, ExType))
				{
					return;//do nothing
				}
			}

		}

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
bool SystematicAnalysis::IsfloatType(const TypeSymbol& TypeToCheck)
{
	return
		TypeToCheck._Type == TypesEnum::float32 ||
		TypeToCheck._Type == TypesEnum::float64;
}
bool SystematicAnalysis::IsPrimitive(const TypeSymbol& TypeToCheck)
{
	bool r = TypeToCheck.IsAddress() || IsPrimitiveNotIncludingPointers(TypeToCheck);

	if (!r && TypeToCheck.IsAn(TypesEnum::CustomType))
	{
		Symbol& V = *GetSymbol(TypeToCheck);
		if (V.Type == SymbolType::Func_ptr)
		{
			return true;
		}
	}

	return r;
}
bool SystematicAnalysis::IsPrimitiveNotIncludingPointers(const TypeSymbol& TypeToCheck)
{
	bool r = IsIntType(TypeToCheck)
		|| TypeToCheck._Type == TypesEnum::Bool
		|| TypeToCheck._Type == TypesEnum::Char
		|| TypeToCheck._Type == TypesEnum::Void
		|| IsfloatType(TypeToCheck);


	return r;
}

bool SystematicAnalysis::IsimmutableRulesfollowed(const TypeSymbol& TypeToCheck, const TypeSymbol& Type)
{
	return  (!TypeToCheck.Isimmutable() == Type.Isimmutable()) ||
		(TypeToCheck.Isimmutable() == Type.Isimmutable());
}
bool SystematicAnalysis::IsAddessAndLValuesRulesfollowed(const TypeSymbol& TypeToCheck, const TypeSymbol& Type, bool ReassignMode)
{
	if (ReassignMode) { return true; }
	bool CheckIsLocation = TypeToCheck.IsLocationValue() || TypeToCheck.IsAddress();
	bool WantsALocation = Type.IsAddress();
	
	if (!CheckIsLocation && WantsALocation)
	{
		return false;
	}

	return ( 
		(CheckIsLocation)
		|| (CheckIsLocation == false || WantsALocation == false)
		|| (TypeToCheck.IsRawValue() && Type.IsRawValue())//constant expression
		);
}
bool SystematicAnalysis::HasDestructor(const TypeSymbol& TypeToCheck)
{
	bool DegBugFlag = (OptimizationFlags_t)_Settings->_Flags & (OptimizationFlags_t)OptimizationFlags::ForDebuging;


	if (!DegBugFlag && IsPrimitive(TypeToCheck))
	{
		return false;
	}

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

	return GetSymbol(TypeDestructorFuncName, SymbolType::Func);
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
			throw std::exception("");
			break;
		}
		return true;

	case TypesEnum::CustomType:
	{
		Symbol& V = *GetSymbol(Type._CustomTypeSymbol);
	
		if (V.Type == SymbolType::Type_class)
		{

			ClassInfo* Vp = V.Get_Info<ClassInfo>();
			
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
		else if(V.Type == SymbolType::Type_alias
			|| V.Type == SymbolType::Hard_Type_alias)
		{

		

			return GetSize(V.VarType,OutSize);
		}
		else if (V.Type == SymbolType::Enum)
		{
			EnumInfo* Info = V.Get_Info<EnumInfo>();
			return GetSize(Info->Basetype, OutSize);
		}
		else if (V.Type == SymbolType::Func_ptr
			|| V.Type == SymbolType::Hard_Func_ptr)
		{
			goto IntPtr;
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
SystematicAnalysis::Get_FuncInfo SystematicAnalysis::GetFunc(const TypeSymbol& Name, const ValueParametersNode& Pars)
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
void SystematicAnalysis::DoFuncCall(Get_FuncInfo Func, const ScopedNameNode& Name, const ValueParametersNode& Pars)
{
	if (passtype != PassType::BuidCode) { return; }
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
				DoImplicitConversion(_LastExpressionField, LastExpressionType, iNfo);\
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

		PrimitiveTypeCall(Uint8TypeName, TypesEnum::uInt8, _LastExpressionField = LookingAtIRBlock->NewLoad((UInt8)0);)
		else PrimitiveTypeCall(Uint16TypeName, TypesEnum::uInt16, _LastExpressionField = LookingAtIRBlock->NewLoad((UInt16)0))
		else PrimitiveTypeCall(Uint32TypeName, TypesEnum::uInt32, _LastExpressionField = LookingAtIRBlock->NewLoad((UInt32)0))
		else PrimitiveTypeCall(Uint16TypeName, TypesEnum::uInt64, _LastExpressionField = LookingAtIRBlock->NewLoad(((UInt64)0)))

		else PrimitiveTypeCall(Sint8TypeName, TypesEnum::sInt8, _LastExpressionField = LookingAtIRBlock->NewLoad((Int8)0);)
		else PrimitiveTypeCall(Sint16TypeName, TypesEnum::sInt16, _LastExpressionField = LookingAtIRBlock->NewLoad((Int16)0))
		else PrimitiveTypeCall(Sint32TypeName, TypesEnum::sInt32, _LastExpressionField = LookingAtIRBlock->NewLoad((Int32)0))
		else PrimitiveTypeCall(Sint16TypeName, TypesEnum::sInt64, _LastExpressionField = LookingAtIRBlock->NewLoad((Int64)0))

		else PrimitiveTypeCall(boolTypeName, TypesEnum::Bool, _LastExpressionField = LookingAtIRBlock->NewLoad(false))
		else PrimitiveTypeCall(CharTypeName, TypesEnum::Char, _LastExpressionField = LookingAtIRBlock->NewLoad('\0'))

		else PrimitiveTypeCall(float32TypeName, TypesEnum::float32, _LastExpressionField = LookingAtIRBlock->NewLoad((float32)0))
		else PrimitiveTypeCall(float64TypeName, TypesEnum::float64, _LastExpressionField = LookingAtIRBlock->NewLoad((float64)0))
		
	}
	if (Func.Func == nullptr)
	{
		return;
	}

	bool AutoPushThis = Func.ThisPar != Get_FuncInfo::ThisPar_t::NoThisPar;
	if (AutoPushThis)
	{
		if (Func.ThisPar == Get_FuncInfo::ThisPar_t::PushFromScopedName)
		{
			
			GetMemberTypeSymbolFromVar_t V;
			GetMemberTypeSymbolFromVar(0,Name.ScopedName.size() - 1, Name, V);

			LookingAtIRBlock->NewPushParameter(BuildMember_AsPointer(V));
		}
		else if (Func.ThisPar == Get_FuncInfo::ThisPar_t::PushFromLast)
		{
			LookingAtIRBlock->NewPushParameter(_LastExpressionField);
		}
		else if (Func.ThisPar == Get_FuncInfo::ThisPar_t::OnIRlocationStack)
		{
			auto Type = Func.Func->Pars[0];
			if (Type.IsAddress())
			{
				Type._IsAddress = false;
			}

			auto Defe = LookingAtIRBlock->NewLoadPtr(IRlocations.top().Value);
			IRlocations.top().UsedlocationIR = true;
			LookingAtIRBlock->NewPushParameter(Defe);
		}
		else if (Func.ThisPar == Get_FuncInfo::ThisPar_t::OnIRlocationStackNonedef)
		{
			LookingAtIRBlock->NewPushParameter(IRlocations.top().Value);
			IRlocations.top().UsedlocationIR = true;
		}
		else if (Func.ThisPar == Get_FuncInfo::ThisPar_t::PushWasCalled)
		{

		}
		else if (Func.ThisPar == Get_FuncInfo::ThisPar_t::AutoPushThis)
		{
			LookingAtIRBlock->NewPushParameter(LookingAtIRBlock->NewLoad(&LookingAtIRFunc->Pars.front()));
		}
		else
		{
			throw std::exception("not added");
		}
	}

	for (size_t i = 0; i < Pars._Nodes.size(); i++)
	{
		auto& Item = Pars._Nodes[i];
		auto& FuncParInfo = Func.Func->Pars[AutoPushThis ? i + 1 : i];


		
		LookingForTypes.push(FuncParInfo);

		OnExpressionTypeNode(Item.get());
		DoImplicitConversion(_LastExpressionField, LastExpressionType, FuncParInfo);

		LookingAtIRBlock->NewPushParameter(_LastExpressionField);
		

		LookingForTypes.pop();
	}
	auto Syb = Func.SymFunc;

	AddDependencyToCurrentFile(Syb);
	
	if (Syb->Type== SymbolType::Func)
	{
		_LastExpressionField = LookingAtIRBlock->NewCall(_Builder.ToID(Syb->FullName));
	}
	else if (Syb->Type == SymbolType::StackVarable)
	{
		_LastExpressionField = LookingAtIRBlock->NewCallFuncPtr(Syb->IR_Ins);
	}
	else if (Syb->Type == SymbolType::ParameterVarable)
	{
		_LastExpressionField = LookingAtIRBlock->NewCallFuncPtr(Syb->IR_Par);
	}
	else
	{
		throw std::exception("not added");
	}

	LastExpressionType = Func.Func->Ret;
}
void SystematicAnalysis::DoDestructorCall(const ObjectToDrop& Object)
{
	
	if (IsPrimitive(Object.Type))
	{
		/*
		return;
		UAddress NewValue;
		UAddress ObjectSize;
		GetSize(Object.Type, ObjectSize);

		Byte* ValuePtr = (Byte*)&NewValue;

		size_t Length = ObjectSize < sizeof(NewValue) ? ObjectSize : sizeof(NewValue);
		for (size_t i = 0; i < Length; i++)
		{
			*ValuePtr = DebugGarbageByte;

			ValuePtr++;
		}
		
		#define Primitive_Destructor(X) \
		case sizeof(UInt##X):\
		{\
			_Builder.Build_Assign(IROperand::AsInt##X(*(UInt##X*)&NewValue));\
		}break;\

		switch (ObjectSize)
		{
			Primitive_Destructor(8)
			Primitive_Destructor(16)
			Primitive_Destructor(32)
			Primitive_Destructor(64)

		default:break;
		}

		auto Op = IROperand::AsLocation(_Builder.GetLastField());
		auto NewOp = IROperand::AsLocation(Object.Object);
		_Builder.Build_Assign(NewOp, Op);
		*/
	}
	else
	{
		
		if (Object.Type.IsAn(TypesEnum::CustomType))
		{
			int a = 0;

			TypeSymbol tep = Object.Type;
			if (tep.IsAddress())
			{
				tep._IsAddress = false;
			}


			String TypeDestructorFuncName = ToString(tep);
			ScopeHelper::GetApendedString(TypeDestructorFuncName, ClassDestructorFunc);

			Symbol* Sym = GetSymbol(TypeDestructorFuncName, SymbolType::Func);
			if (Sym) 
			{
				FuncInfo* node = Sym->Get_Info<FuncInfo>();

				Get_FuncInfo FuncInfo;
				FuncInfo.Func = node;
				FuncInfo.SymFunc = Sym;
				

				//par

				switch (Object.DropType)
				{
				case ObjectToDropType::IRInstruction:
					FuncInfo.ThisPar = Get_FuncInfo::ThisPar_t::OnIRlocationStack;
					IRlocations.push({Object._Object, false});
					break;	
				case ObjectToDropType::IRInstructionNoMod:
					FuncInfo.ThisPar = Get_FuncInfo::ThisPar_t::OnIRlocationStackNonedef;
						IRlocations.push({ Object._Object, false });
						break;
				default:
					throw std::exception("not added");
					break;
				}

				//

				ValueParametersNode Vtemp;
				DoFuncCall(Object.Type, FuncInfo, Vtemp);
			

				IRlocations.pop();
			}
		}
		
	}
}
SystematicAnalysis::Get_FuncInfo  SystematicAnalysis::GetFunc(const ScopedNameNode& Name, const UseGenericsNode& Generics, const ValueParametersNode& Pars, TypeSymbol Ret)
{
	TypeSymbol _ThisType;
	Get_FuncInfo::ThisPar_t ThisParType = Get_FuncInfo::ThisPar_t::NoThisPar;
	String ScopedName;
	{

		bool IsThisCall = false;
		for (auto& Item : Name.ScopedName)
		{
			if (Item.Operator == ScopedName::Operator_t::Dot)
			{
				IsThisCall = true;
				break;
			}
		}

		if (IsThisCall)
		{
			GetMemberTypeSymbolFromVar_t V;
			if (GetMemberTypeSymbolFromVar(0,Name.ScopedName.size() - 1, Name, V))
			{
				_ThisType = V.Type;
				_ThisType.SetAsAddress();

				auto tep_ = V.Type;
				if (tep_.IsAddress())
				{
					tep_._IsAddress = false;
				}
				if (tep_.IsAddressArray())
				{
					tep_._IsAddressArray = false;
				}


				ScopedName = ToString(tep_) + ScopeHelper::_ScopeSep;
				ScopedName += Name.ScopedName.back().token->Value._String;

				ThisParType = Get_FuncInfo::ThisPar_t::PushFromScopedName;
			}
			else
			{
				return { Get_FuncInfo::ThisPar_t::NoThisPar, nullptr };
			}
		}
		else 
		{
			ScopedName = GetScopedNameAsString(Name);
		}
	}
	bool _ThisTypeIsNotNull = !_ThisType.IsNull();
	
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

			return { Get_FuncInfo::ThisPar_t::NoThisPar, nullptr };

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
			ScopedName == CharTypeName ||
			ScopedName == float32TypeName ||
			ScopedName == float64TypeName)
		{
			
			if (Pars._Nodes.size() > 1)
			{
				LogCanIncorrectParCount(Name.ScopedName.back().token, ScopedName, Pars._Nodes.size(), 1);
			}
			return { Get_FuncInfo::ThisPar_t::NoThisPar, nullptr };
		}
	}
	
	bool AutoThisCall = false;
	
	SymbolType T = SymbolType::Null;
	Symbol* FuncSymbol = nullptr;
	FuncInfo* r = nullptr;

	auto& RetType = Get_LookingForType();
	bool RetIsSet = !(RetType.IsAn(TypesEnum::Var) || RetType.IsAn(TypesEnum::Any));

	Vector<TypeSymbol> ValueTypes;
	ValueTypes.reserve(_ThisTypeIsNotNull ? Pars._Nodes.size() + 1 : Pars._Nodes.size());

	if (_ThisTypeIsNotNull)
	{
		ValueTypes.push_back(_ThisType);
	}

	TypeSymbol NullSymbol;
	NullSymbol.SetType(TypesEnum::Any);

	for (size_t i = 0; i < Pars._Nodes.size(); i++)
	{
		auto& Item = Pars._Nodes[i];
		auto& ValueItem = ValueTypes.emplace_back();

		LookingForTypes.push(NullSymbol);

		OnExpressionTypeNode(Item.get());
		ValueItem = LastExpressionType;

		LookingForTypes.pop();
	}
	auto& Symbols = _Table.GetSymbolsWithName(ScopedName, SymbolType::Any);
	StartSymbolsLoop:
	for (auto& Item : Symbols)
	{
		if (Item->Type == SymbolType::Func)
		{
			

			FuncInfo* Info = Item->Get_Info<FuncInfo>();
			
			

			/*
			if (RetIsSet)
			{
				if (!AreTheSame(Info->Ret, RetType))
				{
					continue;
				}
			}
			*/

			if (Info->Pars.size() != ValueTypes.size())
			{
				continue;
			}


			//
			if (Item->PassState != passtype)
			{
				if (!IsDependencies(Info)) 
				{
					auto OldPass = passtype;
					auto OldScope = _Table._Scope.ThisScope;
					_Table._Scope.ThisScope = Info->FullName;
					ScopeHelper::ReMoveScope(_Table._Scope.ThisScope);

					_RetLoopStack.push_back(Info);

					OnFuncNode(*(FuncNode*)Item->NodePtr);

					_RetLoopStack.pop_back();

					_Table._Scope.ThisScope = OldScope;
				}
				else
				{
					auto V = GetDependencies(Info);
					if (V->IsOnRetStatemnt)
					{
						LogFuncDependencyCycle(Name.ScopedName.back().token, Info);
						Info->Ret.SetType(TypesEnum::Null);//to stop err spam
					}
				}
			}
			//

			for (size_t i = 0; i < Info->Pars.size(); i++)
			{
				auto& Item = Info->Pars[i];
				if (_ThisTypeIsNotNull && i == 0) { continue; }
				auto& Item2 = ValueTypes[i];

				if (!CanBeImplicitConverted(Item2, Item, true))
				{
					goto ContinueOutloop;
				}
			}
			r = Info;
			FuncSymbol = Item;
			T = SymbolType::FuncCall;
			break;

		ContinueOutloop:continue;
		}
		else if (Item->Type == SymbolType::GenericFunc)
		{
			FuncInfo* Info = Item->Get_Info<FuncInfo>();

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
				if (_ThisTypeIsNotNull && i == 0) { continue; }

				auto& Par = Info->Pars[i];


				for (size_t i2 = 0; i2 < Info->_Generic.size(); i2++)
				{
					auto& V3 = Info->_Generic[i2];
					if (V3.SybID == Par._CustomTypeSymbol)
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
					if (V3.SybID == Info->Ret._CustomTypeSymbol)
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
				auto FuncIsMade = GetSymbol(NewName, SymbolType::Func);



				if (!FuncIsMade)
				{

					{

						for (size_t i = 0; i < GenericInput.size(); i++)
						{
							const auto& Tnode = GenericInput[i];
							const auto& GenericInfo = Info->_Generic[i];

							bool InputTypeIsConstantExpression = false;

							auto TypeSyb = GetSymbol(GenericInfo.IsConstantExpression);
							if (TypeSyb)
							{
								InputTypeIsConstantExpression = TypeSyb->Type == SymbolType::ConstantExpression;
							}

							if (InputTypeIsConstantExpression != GenericInfo.IsConstantExpression)
							{
								goto ContinueOutloop;
							}
						}
					}
					auto Pointer = std::make_unique<Vector<TypeSymbol>>(std::move(GenericInput));
					//pointer must be unique so it cant be on the stack

					GenericFuncInstantiate(FuncSym, *Pointer);

					TepFuncs.push_back({ std::move(Pointer) });//keep pointer 


					FuncSym = GetSymbol(NewName, SymbolType::Func);
					r = FuncSym->Get_Info<FuncInfo>();
					FuncSymbol = Item;
				}
				else
				{
					r = FuncSym->Get_Info<FuncInfo>();
					FuncSymbol = Item;
				}






			}
			break;
		}
		else if (Item->Type == SymbolType::Type_class)
		{
			ClassInfo* V = Item->Get_Info<ClassInfo>();

			String Scope = V->FullName;
			ScopeHelper::GetApendedString(Scope, ClassConstructorfunc);

			auto& ConstructorSymbols = _Table.GetSymbolsWithName(Scope, SymbolType::Any);
			for (auto& Item2 : ConstructorSymbols)
			{
				if (Item->Type == SymbolType::Func)
				{
					FuncInfo* Info = Item->Get_Info<FuncInfo>();
					r = Info;
					FuncSymbol = Item;
					ThisParType = Get_FuncInfo::ThisPar_t::OnIRlocationStack;
					break;
				}
			}

			if (r) { break; }
		}
		else if (Item->Type == SymbolType::StackVarable|| Item->Type == SymbolType::ParameterVarable)
		{
			Symbol* Type = GetSymbol(Item->VarType);
		    if (Type && (Type->Type == SymbolType::Func_ptr || Type->Type == SymbolType::Hard_Func_ptr))
			{
				FuncInfo* Info = Type->Get_Info<FuncInfo>();//must be the same as Item->Type == SymbolType::Func

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
					if (_ThisTypeIsNotNull && i == 0) { continue; }
					auto& Item2 = ValueTypes[i];

					if (!CanBeImplicitConverted(Item2, Item, true))
					{
						goto ContinueOutloop;
					}
				}
				FuncSymbol = Item;
				r = Info;
			}
		}
	}
	if (r == nullptr)
	{
		bool MayBeAutoThisFuncCall = Name.ScopedName.size() == 1 && IsInThisFuncCall();

		if (MayBeAutoThisFuncCall && AutoThisCall == false)
		{
			AutoThisCall = true;

			ValueTypes.insert(ValueTypes.begin(),*_FuncStack.back().Pointer->GetObjectForCall());
			ThisParType = Get_FuncInfo::ThisPar_t::AutoPushThis;
			goto StartSymbolsLoop;
		}
		else 
		{
			LogCantFindFuncError(Name.ScopedName.back().token, ScopedName, {}, ValueTypes, RetType);
		}
	}
	return { ThisParType,r,FuncSymbol };
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


	AddDependencyToCurrentFile(Func);
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

	_Table._Scope.ThisScope  = ScopeHelper::GetReMoveScope(NewName);


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

	//

	AddDependencyToCurrentFile(Class);
}


//

void* SystematicAnalysis::Get_Object(const TypeSymbol& Input, const RawEvaluatedObject& Input2)
{
	return Input2.Object_AsPointer.get();
}

void* SystematicAnalysis::Get_Object(const EvaluatedEx& Input)
{
	return Get_Object(Input.Type, Input.EvaluatedObject);
}

bool SystematicAnalysis::ConstantExpressionAbleType(const TypeSymbol& Type)
{
	return IsPrimitive(Type) && !Type.IsAddress() && !Type.IsAddressArray();
}

SystematicAnalysis::EvaluatedEx SystematicAnalysis::MakeEx(const TypeSymbol& Type)
{
	EvaluatedEx r;
	r.Type = Type;

	size_t Size = 0;
	GetSize(Type, Size);

	r.EvaluatedObject.Object_AsPointer = std::make_unique<Byte[]>(Size);
	r.EvaluatedObject.ObjectSize = Size;

	return r;
}

RawEvaluatedObject SystematicAnalysis::MakeExr(const TypeSymbol& Type)
{
	throw std::exception("not added");
	return RawEvaluatedObject();
}

bool SystematicAnalysis::EvaluateDefaultConstructor(EvaluatedEx& Out)
{
	void* Object = Get_Object(Out);
	switch (Out.Type._Type)
	{
	case TypesEnum::Char:
	case TypesEnum::Bool:
	case TypesEnum::sInt8:
	case TypesEnum::uInt8:
		*(UInt8*)Object = 0;
		break;
	case TypesEnum::uInt16:
	case TypesEnum::sInt16:
		*(UInt16*)Object = 0;
		break;
	case TypesEnum::uInt32:
	case TypesEnum::sInt32:
		*(UInt32*)Object = 0;
		break;
	case TypesEnum::uInt64:
	case TypesEnum::sInt64:
		*(UInt64*)Object = 0;
		break;

	case TypesEnum::uIntPtr:
	case TypesEnum::sIntPtr:
	{
		size_t PtrSize = 0;
		TypeSymbol V;
		V.SetType(TypesEnum::uIntPtr);
		GetSize(V, PtrSize);
		switch (PtrSize)
		{
		case sizeof(UInt8): *(UInt8*)Object = 0; break;
		case sizeof(UInt16) : *(UInt16*)Object = 0; break;
		case sizeof(UInt32) : *(UInt32*)Object = 0; break;
		case sizeof(UInt64) : *(UInt64*)Object = 0; break;
		default:
			throw std::exception("not added");
			break;
		}
	}break;
	
	case TypesEnum::float32:
		*(float32*)Object = 0;
		break;
	case TypesEnum::float64:
		*(float64*)Object = 0;
		break;
	default:
		throw std::exception("?");
		break;
	}
	return false;
}

bool SystematicAnalysis::Evaluate(EvaluatedEx& Out, const ValueExpressionNode& node)
{
	switch (node.Value->Get_Type())
	{
	case NodeType::NumberliteralNode:
	{
		NumberliteralNode* num = NumberliteralNode::As(node.Value.get());
#define Set_NumberliteralNodeU2(x) \
			UInt##x V; \
			ParseHelper::ParseStringToUInt##x(Str, V); \
			*(UInt##x*)Get_Object(Out) = V;\

#define Set_NumberliteralNodeS2(x) \
			Int##x V; \
			ParseHelper::ParseStringToInt##x(Str, V); \
			*(Int##x*)Get_Object(Out) = V;\


		auto& lookT = Get_LookingForType();
		TypesEnum NewEx;
		if (lookT._Type == TypesEnum::Var)
		{
			NewEx = TypesEnum::sInt32;
		}
		else
		{
			NewEx = (IsfloatType(lookT) || IsIntType(lookT)) ? lookT._Type : TypesEnum::sInt32;
		}
			//if (passtype == PassType::BuidCode)
		{
			auto& Str = num->Token->Value._String;



			switch (NewEx)
			{
			sint8case:
			case TypesEnum::sInt8:
			case TypesEnum::uInt8:
			{
				Set_NumberliteralNodeU2(8);
			};
			break;
		sint16case:
			case TypesEnum::sInt16:
			case TypesEnum::uInt16:
			{
				Set_NumberliteralNodeU2(16);
			};
			break;
		sint32case:
			case TypesEnum::sInt32:
			case TypesEnum::uInt32:
			{
				Set_NumberliteralNodeU2(32);
			};
			break;
		sint64case:
			case TypesEnum::sInt64:
			case TypesEnum::uInt64:
			{
				Set_NumberliteralNodeU2(64);
			};
			break;
			case TypesEnum::sIntPtr:
			case TypesEnum::uIntPtr:
			{
				size_t PtrSize = 0;
				TypeSymbol V;
				V.SetType(TypesEnum::uIntPtr);
				GetSize(V, PtrSize);
				switch (PtrSize)
				{
					case sizeof(UInt8) : goto sint8case;
					case sizeof(UInt16) : goto sint16case;
					case sizeof(UInt32) : goto sint32case;
					case sizeof(UInt64) : goto sint64case;
					default:
				    throw std::exception("not added");
					break;
				}
			};
			break;

			case TypesEnum::float32:
			{
				Int32 V;
				ParseHelper::ParseStringToInt32(Str, V);
				*(float32*)Get_Object(Out) = (float32)V;
				break;
			};
			case TypesEnum::float64:
			{
				Int64 V;
				ParseHelper::ParseStringToInt64(Str, V);
				*(float64*)Get_Object(Out) = (float64)V;
				break;
			};
			default:
				throw std::exception("not added");
				break;
			}

		}


		LastExpressionType.SetType(NewEx);
		LastLookedAtToken = num->Token;
	}
	break;
	case NodeType::BoolliteralNode:
	{
		BoolliteralNode* num = BoolliteralNode::As(node.Value.get());

		//if (passtype == PassType::BuidCode)
		{
			*(bool*)Get_Object(Out) = num->Get_Value();
		}
		LastExpressionType.SetType(TypesEnum::Bool);
		LastLookedAtToken = num->Token;
	}
	break;
	case NodeType::CharliteralNode:
	{
		CharliteralNode* num = CharliteralNode::As(node.Value.get());

		//if (passtype == PassType::BuidCode)
		{
			String V;
			bool ItWorked = !ParseHelper::ParseCharliteralToChar(num->Token->Value._String, V);


			*(char*)Get_Object(Out) = (UInt8)V.front();
		}
		LastExpressionType.SetType(TypesEnum::Char);
		LastLookedAtToken = num->Token;
	}
	break;
	case NodeType::FloatliteralNode:
	{
		FloatliteralNode* num = FloatliteralNode::As(node.Value.get());
		auto& lookT = Get_LookingForType();
		
		auto& Str = num->Token->Value._String;
		

		TypesEnum NewEx;
		if (lookT._Type == TypesEnum::Var)
		{
			NewEx = TypesEnum::float32;
		}
		else
		{
			NewEx = IsfloatType(lookT) ? lookT._Type : TypesEnum::float32;
		}
		//if (passtype == PassType::BuidCode)
		{
			switch (NewEx)
			{
			case TypesEnum::float32:
			{
				float32 V;
				ParseHelper::ParseStringTofloat32(Str, V);
				*(float32*)Get_Object(Out) = V;
				break;
			}
			case TypesEnum::float64:
			{
				float64 V;
				ParseHelper::ParseStringTofloat64(Str, V);
				*(float64*)Get_Object(Out) = V;
				break;
			}
			default:
				throw std::exception("not added");
				break;
			}
		}
		


		LastExpressionType.SetType(NewEx);
		LastLookedAtToken = num->Token;
	}
	case NodeType::SizeofExpresionNode:
	{
		SizeofExpresionNode* nod = SizeofExpresionNode::As(node.Value.get());

		auto& lookT = Get_LookingForType();
		TypeSymbol Type;

		//if (passtype == PassType::FixedTypes || passtype == PassType::BuidCode)
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

		//if (passtype == PassType::BuidCode)
		{
			TypeSymbol Info;
			Convert(nod->Type, Info);
			UAddress TypeSize;
			GetSize(Info, TypeSize);



			switch (lookT._Type)
			{
			Int8Case:
			case TypesEnum::sInt8:
			case TypesEnum::uInt8:
				*(UInt8*)Get_Object(Out) = (UInt8)TypeSize;
				break;
			Int16Case:
			case TypesEnum::sInt16:
			case TypesEnum::uInt16:
				*(UInt16*)Get_Object(Out) = (UInt16)TypeSize;
				break;
			Int32Case:
			case TypesEnum::sInt32:
			case TypesEnum::uInt32:
				*(UInt32*)Get_Object(Out) = (UInt32)TypeSize;
				break;
			Int64Case:
			case TypesEnum::sInt64:
			case TypesEnum::uInt64:
				*(UInt64*)Get_Object(Out) = (UInt64)TypeSize;
				break;
			default:
			{
				Type.SetType(TypesEnum::uIntPtr);

				UAddress PtrSize;
				GetSize(Info, PtrSize);

				switch (PtrSize)
				{
					case sizeof(UInt8) : goto Int8Case;
					case sizeof(UInt16) : goto Int16Case;
					case sizeof(UInt32) : goto Int32Case;
					case sizeof(UInt64) : goto Int64Case;
				default:
					throw std::exception("not added");
					break;
				}

			}break;
			}
		}

		LastExpressionType = Type;
	}
	break;
	default:
		throw std::exception("not added");
		break;
	}
	return true;
}

bool SystematicAnalysis::Evaluate(EvaluatedEx& Out, const BinaryExpressionNode& node)
{
	auto Ex0node = node.Value0.Value.get();
	auto Ex1node = node.Value1.Value.get();
	if (passtype == PassType::BuidCode && 
		(
		SwapForOperatorPrecedence(Ex0node, Ex1node) && SwapForOperatorPrecedence(&node, Ex1node)//i have no clue why this works
		)
		)
	{
		std::swap(Ex0node, Ex1node);
	}

	return false;
}

bool SystematicAnalysis::Evaluate(EvaluatedEx& Out, const CastNode& node)
{
	return false;
}

bool SystematicAnalysis::Evaluate_t(EvaluatedEx& Out, const Node* node)
{
	switch (node->Get_Type())
	{
	case NodeType::BinaryExpressionNode:return Evaluate(Out,*BinaryExpressionNode::As(node)); break;
	case NodeType::ValueExpressionNode:return Evaluate(Out, *ValueExpressionNode::As(node)); break;
	case NodeType::CastNode:return Evaluate(Out, *CastNode::As(node)); break;
	default:
		throw std::exception("not added");
		break;
	}
}

bool SystematicAnalysis::EvaluatePostfixOperator(EvaluatedEx& Out, TokenType Op)
{
	void* Object = Get_Object(Out);
	switch (Out.Type._Type)
	{

	uint8case:
	case TypesEnum::sInt8:
	case TypesEnum::uInt8:
	{
		if (Op == TokenType::increment)
		{
			(*(UInt8*)Object)++;
		}
		else
			if (Op == TokenType::decrement)
			{
				(*(UInt8*)Object)++;
			}
			else
			{
				throw std::exception("not added");
			}
	}
	break;

uint16case:
	case TypesEnum::uInt16:
	case TypesEnum::sInt16:
	{
		if (Op == TokenType::increment)
		{
			(*(UInt16*)Object)++;
		}
		else
			if (Op == TokenType::decrement)
			{
				(*(UInt16*)Object)++;
			}
			else
			{
				throw std::exception("not added");
			}
	}break;

uint32case:
	case TypesEnum::uInt32:
	case TypesEnum::sInt32:
	{
		if (Op == TokenType::increment)
		{
			(*(UInt32*)Object)++;
		}
		else
			if (Op == TokenType::decrement)
			{
				(*(UInt32*)Object)++;
			}
			else
			{
				throw std::exception("not added");
			}
	}
	break;
uint64case:
	case TypesEnum::uInt64:
	case TypesEnum::sInt64:
	{
		if (Op == TokenType::increment)
		{
			(*(UInt64*)Object)++;
		}
		else
			if (Op == TokenType::decrement)
			{
				(*(UInt64*)Object)++;
			}
			else
			{
				throw std::exception("not added");
			}
	}
	break;

	case TypesEnum::uIntPtr:
	case TypesEnum::sIntPtr:
	{
		size_t PtrSize = 0;
		TypeSymbol V;
		V.SetType(TypesEnum::uIntPtr);
		GetSize(V, PtrSize);
		switch (PtrSize)
		{
			case sizeof(UInt8) : goto uint8case;
			case sizeof(UInt16) : goto uint16case;
			case sizeof(UInt32) : goto uint32case;
			case sizeof(UInt64) : goto uint64case;
			default:
			throw std::exception("not added");
			break;
		}
	}break;
	default:
		throw std::exception("not added");
		break;
	}
	return true;
}

bool SystematicAnalysis::HasConstantPostfixOperator(const TypeSymbol& Type, TokenType Op)
{
	if ((Op == TokenType::increment || Op == TokenType::decrement) && IsIntType(Type))
	{
		return true;
	}
	return false;
}

bool SystematicAnalysis::CanEvaluateImplicitConversionConstant(const TypeSymbol& Type, const TypeSymbol& ToType)
{
	if (AreTheSame(Type, ToType))
	{
		return true;
	}
	return false;
}

bool SystematicAnalysis::EvaluateImplicitConversion(EvaluatedEx& In, const TypeSymbol& ToType, EvaluatedEx& out)
{
	if (AreTheSame(In.Type, ToType))
	{
		out.Type = In.Type;
		out.EvaluatedObject = In.EvaluatedObject;
		return true;
	}
	return false;
}

IRInstruction* SystematicAnalysis::IR_Load_UIntptr(UAddress Value)
{

	switch (_Settings->PtrSize)
	{
	case IntSizes::Int8:
		return LookingAtIRBlock->NewLoad((UInt8)Value);
	case IntSizes::Int16:
		return LookingAtIRBlock->NewLoad((UInt16)Value);
	case IntSizes::Int32:
		return LookingAtIRBlock->NewLoad((UInt32)Value);
	case IntSizes::Int64:
		return LookingAtIRBlock->NewLoad((UInt64)Value);
	default:
		throw std::exception("");
		break;
	}
}
IRInstruction* SystematicAnalysis::IR_Load_SIntptr(SIntNative Value)
{
	return IR_Load_UIntptr(*(UAddress*)&Value);
}

IRInstruction* SystematicAnalysis::Build_Add_uIntPtr(IROperator field, IROperator field2)
{
	switch (_Settings->PtrSize)
	{
	case IntSizes::Int8:
		//_Builder.MakeAdd8(field, field2);
		break;	
	case IntSizes::Int16:
		//_Builder.MakeAdd16(field, field2);
		break;
	case IntSizes::Int32:
		//_Builder.MakeAdd32(field, field2);
		break;
	case IntSizes::Int64:
		//_Builder.MakeAdd64(field, field2);
		break;
	default:
		throw std::exception("");
		break;

	}
}

IRInstruction* SystematicAnalysis::Build_Sub_uIntPtr(IROperator field, IROperator field2)
{
	switch (_Settings->PtrSize)
	{
	case IntSizes::Int8:
		//_Builder.MakeSub8(field, field2);
		break;
	case IntSizes::Int16:
		//_Builder.MakeSub16(field, field2);
		break;
	case IntSizes::Int32:
		//_Builder.MakeSub32(field, field2);
		break;
	case IntSizes::Int64:
		//_Builder.MakeSub64(field, field2);
		break;
	default:
		throw std::exception("");
		break;
	}
}

IRInstruction* SystematicAnalysis::Build_Add_sIntPtr(IROperator field, IROperator field2)
{
	switch (_Settings->PtrSize)
	{
	case IntSizes::Int8:
		//_Builder.MakeAdd8(field, field2);
		break;
	case IntSizes::Int16:
		//_Builder.MakeAdd16(field, field2);
		break;
	case IntSizes::Int32:
		//_Builder.MakeAdd32(field, field2);
		break;
	case IntSizes::Int64:
		//_Builder.MakeAdd64(field, field2);
		break;
	default:
		throw std::exception("");
		break;
	}
}

IRInstruction* SystematicAnalysis::Build_Sub_sIntPtr(IROperator field, IROperator field2)
{
	switch (_Settings->PtrSize)
	{
	case IntSizes::Int8:
		//_Builder.MakeSub8(field, field2);
		break;
	case IntSizes::Int16:
		//_Builder.MakeSub16(field, field2);
		break;
	case IntSizes::Int32:
		//_Builder.MakeSub32(field, field2);
		break;
	case IntSizes::Int64:
		//_Builder.MakeSub64(field, field2);
		break;
	default:
		throw std::exception("");
		break;
	}
}

IRInstruction* SystematicAnalysis::Build_Mult_uIntPtr(IRInstruction* field, IRInstruction* field2)
{
	return LookingAtIRBlock->NewUMul(field, field2);
}

IRInstruction* SystematicAnalysis::Build_Mult_sIntPtr(IRInstruction* field, IRInstruction* field2)
{
	return LookingAtIRBlock->NewSMul(field, field2);
}

IRInstruction* SystematicAnalysis::Build_Div_uIntPtr(IROperator field, IROperator field2)
{
	throw std::exception("");
	switch (_Settings->PtrSize)
	{
	case IntSizes::Int8:
		//_Builder.MakeUDiv8(field, field2);
		break;
	case IntSizes::Int16:
		//_Builder.MakeUDiv16(field, field2);
		break;
	case IntSizes::Int32:
		//_Builder.MakeUDiv32(field, field2);
		break;
	case IntSizes::Int64:
		//_Builder.MakeUDiv64(field, field2);
		break;
	default:
		throw std::exception("");
		break;
	}
}

IRInstruction* SystematicAnalysis::Build_Div_sIntPtr(IROperator field, IROperator field2)
{
	throw std::exception("");
	switch (_Settings->PtrSize)
	{
	case IntSizes::Int8:
		//_Builder.MakeSDiv8(field, field2);
		break;
	case IntSizes::Int16:
		//_Builder.MakeSDiv16(field, field2);
		break;
	case IntSizes::Int32:
		//_Builder.MakeSDiv32(field, field2);
		break;
	case IntSizes::Int64:
		//_Builder.MakeSDiv64(field, field2);
		break;
	default:
		throw std::exception("");
		break;
	}
}

void SystematicAnalysis::Build_Increment_uIntPtr(IRInstruction* field, UAddress Value)
{
	switch (_Settings->PtrSize)
	{
	case IntSizes::Int32:
		LookingAtIRBlock->New_Increment(field,LookingAtIRBlock->NewLoad((UInt32)Value));
		break;
	case IntSizes::Int64:
		LookingAtIRBlock->New_Increment(field,LookingAtIRBlock->NewLoad((UInt64)Value));
		break;
	default:
		throw std::exception("");
		break;
	}
}

void SystematicAnalysis::Build_Decrement_uIntPtr(IRInstruction* field, UAddress Value)
{
	switch (_Settings->PtrSize)
	{
	case IntSizes::Int32:
		LookingAtIRBlock->New_Decrement(field, LookingAtIRBlock->NewLoad((UInt32)Value));
		break;
	case IntSizes::Int64:
		LookingAtIRBlock->New_Decrement(field, LookingAtIRBlock->NewLoad((UInt64)Value));
		break;
	default:
		throw std::exception("");
		break;
	}
}

void SystematicAnalysis::Build_Increment_sIntPtr(IRInstruction* field, SIntNative Value)
{
	return Build_Increment_uIntPtr(field, *(UAddress*)&Value);
}

void SystematicAnalysis::Build_Decrement_sIntPtr(IRInstruction* field, SIntNative Value)
{
	return Build_Decrement_uIntPtr(field,*(UAddress*)&Value);
}

void SystematicAnalysis::CheckVarWritingErrors(Symbol* Symbol, const Token* Token, String_view& Name)
{
	if (Symbol->VarType.Isimmutable())
	{
		LogCantModifyiMutableError(Token, Name);
	}
}

void SystematicAnalysis::LogCantCastImplicitTypes(const Token* Token, TypeSymbol& Ex1Type, TypeSymbol& UintptrType,bool ReassignMode)
{
	if (Ex1Type.IsBadType() || UintptrType.IsBadType()) { return; }

	bool V1 = IsAddessAndLValuesRulesfollowed(Ex1Type, UintptrType, ReassignMode);
	if (!V1)
	{
		_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
			, "The expression is not an Location in memory'");
	}
	else
	{
		_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
			, "Cant Implicitly cast Type '" + ToString(Ex1Type) + "' to '" + ToString(UintptrType) + "'");
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
	if (OnType.IsBadType()) { return; }

	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Cant find Member Named '" + (String)Str + "' on type '" + ToString(OnType) +"'");
}

void SystematicAnalysis::LogCantFindCompoundOpForTypes(const Token* BinaryOp, TypeSymbol& Ex0Type, TypeSymbol& Ex1Type)
{
	if (Ex1Type.IsBadType() || Ex0Type.IsBadType()) { return; }

	_ErrorsOutput->AddError(ErrorCodes::InValidType, BinaryOp->OnLine, BinaryOp->OnPos,
		"The type '" + ToString(Ex0Type) + "'" + " cant be '"
		+ ToString(BinaryOp->Type) + "' with '" + ToString(Ex0Type) + "'");
}

void SystematicAnalysis::LogCantFindPostfixOpForTypes(const Token* BinaryOp, TypeSymbol& Ex0Type)
{
	if (Ex0Type.IsBadType()) { return; }

		_ErrorsOutput->AddError(ErrorCodes::InValidType, BinaryOp->OnLine, BinaryOp->OnPos,
			"The type '" + ToString(Ex0Type) + "'" + " cant be '"
			+ ToString(BinaryOp->Type) + "'");
}
void SystematicAnalysis::LogCantFindBinaryOpForTypes(const Token* BinaryOp, TypeSymbol& Ex0Type, TypeSymbol& Ex1Type)
{
	if (Ex1Type.IsBadType() || Ex0Type.IsBadType()) { return; }

	_ErrorsOutput->AddError(ErrorCodes::InValidType, BinaryOp->OnLine, BinaryOp->OnPos,
		"The type '" + ToString(Ex0Type) + "'" + " cant be '"
		+ ToString(BinaryOp->Type) + "' with '" + ToString(Ex1Type) + "'");
}
void SystematicAnalysis::ExpressionMustbeAnLocationValueError(const Token* Token, TypeSymbol& Ex0Type)
{
	if (Ex0Type.IsBadType() ) { return; }
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
	if (Ex0Type.IsBadType() || ToTypeAs.IsBadType()){return;}

	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Cant Explicity cast Type '" + ToString(Ex0Type) + "' to '" + ToString(ToTypeAs) + "'");
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
void SystematicAnalysis::LogCantFindPostfixOpForTypes_Constant(const Token* BinaryOp, TypeSymbol& Ex0Type)
{
	LogCantFindPostfixOpForTypes(BinaryOp, Ex0Type);
}
void SystematicAnalysis::LogCantDoPostfixOpForTypes_Constant(const Token* BinaryOp, TypeSymbol& Ex0Type)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidName, BinaryOp->OnLine, BinaryOp->OnPos
		, "The Type operation '" + ToString(Ex0Type) + "' must be an compile time constant.");
}
void SystematicAnalysis::LogCantCastImplicitTypes_Constant(const Token* Token, TypeSymbol& Ex1Type, TypeSymbol& UintptrType)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Casting Type '" + ToString(Ex1Type) + " to '" + ToString(UintptrType) + "' cant be done at compile time.");
}
void SystematicAnalysis::LogCantFindNamespace(const Token* Token, const String_view Namespace)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos
		, "the cant find the Namespace '" + (String)Namespace + "'.");
}
void SystematicAnalysis::LogTypeMustBeAnConstantExpressionAble(const Token* Token, const TypeSymbol& Type)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos
		, "the type " + ToString(Type) + " must be an Constant Expression able type'");
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
void SystematicAnalysis::LogFuncMustBe(const Token* Token, const String_view FuncName, TypeSymbol& TypeSybToBe)
{

	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		,"The function named " + (String)FuncName + " must return to the type of '" + ToString(TypeSybToBe) + '\'');
}
void SystematicAnalysis::LogSymbolRedefinition(const Token* Token, const Symbol* Symbol)
{

	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "the symbol '"+  Symbol->FullName + "\' is already define. try useing a different name");
}
void SystematicAnalysis::LogUseingVarableBeforDeclared(const Token* Token)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "trying to use the varable '" + (String)Token->Value._String + "\' before its defined.");
}
void SystematicAnalysis::LogBeMoreSpecifiicForRetType(const String_view FuncName, const Token* Token)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "be more Specifiic For return Type. like |" + (String)FuncName + "[...] -> [Type]; or give the funcion a body.");
}
void SystematicAnalysis::LogCantBeIndexWithType(const Token* Token, const  TypeSymbol& Ex0Type, const  TypeSymbol& IndexType)
{
	if (Ex0Type.IsBadType() || IndexType.IsBadType()) { return; }

	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "The Type '" + ToString(Ex0Type) + "\' Cant be Index with '" + ToString(IndexType) + "'.");
}
void SystematicAnalysis::LogCantUseThisInStaticFunction(const Token* Token)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "The 'this' parameter can't be accessed in a static function.A 'this' function must look like |[this&,...] -> [Type];");
}
void SystematicAnalysis::LogFuncDependencyCycle(const Token* Token, const FuncInfo* Value)
{
	String Msg = "function return type Dependency Cycle On function '" + (String)Value->FullName + "' <- ";

	for (int i = _FuncStack.size() - 1; i >= 0; i--)
	{
		auto& Item = _FuncStack[i];

		Msg += "'" + (String)Item.Pointer->FullName + "'";
		if (&Item != &_FuncStack.front())
		{
			Msg += " <- ";
		}

		if (Item.Pointer->Ret.IsBadType())
		{
			return;
		}
	}
	Msg += ".";
	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, Msg);

}
void SystematicAnalysis::LogGenericInputWantsaExpressionNotType(const Token* Token, const String_view NameOfPar)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Generic The generic parameter '" + (String)NameOfPar + "' Wants a Expression not a Type.");
}
void SystematicAnalysis::LogGenericInputWantsaTypeNotExpression(const Token* Token, const String_view NameOfPar)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Generic The generic parameter '" + (String)NameOfPar + "'Type Wants a  not a Expression.");

}

void SystematicAnalysis::LogExpectedSymbolToBea(const Token* Token, const Symbol& Syb, SymbolType Value)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Did not expect Symbol the Symbol '" + Syb.FullName + "' to be '" + ToString(Syb.Type) + 
		"' .Expected '" + ToString(Value) + '\'');

}
String SystematicAnalysis::ToString(SymbolType Value)
{
	switch (Value)
	{
	case UCodeLang::FrontEnd::SymbolType::Null:return "null";
	case UCodeLang::FrontEnd::SymbolType::Any:return "Any";
	case UCodeLang::FrontEnd::SymbolType::Varable_t:return "Varable_t";
	case UCodeLang::FrontEnd::SymbolType::StackVarable:return "StackVarable";
	case UCodeLang::FrontEnd::SymbolType::ParameterVarable:return "ParameterVarable";
	case UCodeLang::FrontEnd::SymbolType::Type:return "Type";
	case UCodeLang::FrontEnd::SymbolType::Type_alias:return "Type_alias";
	case UCodeLang::FrontEnd::SymbolType::Hard_Type_alias:return "Hard_Type_alias";
	case UCodeLang::FrontEnd::SymbolType::Type_class:return "Type_class";
	case UCodeLang::FrontEnd::SymbolType::Class_Field:return "Class_Field";
	case UCodeLang::FrontEnd::SymbolType::Enum:return "Enum";
	case UCodeLang::FrontEnd::SymbolType::Func:return "Func";
	case UCodeLang::FrontEnd::SymbolType::ImportedDllFunc:return "ImportedDllFunc";
	case UCodeLang::FrontEnd::SymbolType::ImportedLibFunc:return "ImportedLibFunc";
	case UCodeLang::FrontEnd::SymbolType::FuncCall:return "FuncCall";
	case UCodeLang::FrontEnd::SymbolType::GenericFunc:return "GenericFunc";
	case UCodeLang::FrontEnd::SymbolType::Generic_class:return"Generic_class";
	case UCodeLang::FrontEnd::SymbolType::Unmaped_Generic_Type:return "Unmaped_Generic_Type";
	case UCodeLang::FrontEnd::SymbolType::Namespace:return "Namespace";
	case UCodeLang::FrontEnd::SymbolType::Hard_Func_ptr:return "Hard_Func_ptr";
	case UCodeLang::FrontEnd::SymbolType::Func_ptr:return "Func_ptr";
	case UCodeLang::FrontEnd::SymbolType::ConstantExpression:return "ConstantExpression";
	default:return "[n/a]";
	}
}
UCodeLangFrontEnd


