#include "SystematicAnalysis.hpp"
#include "UCodeLang/Compliation/Helpers/KeyWords.hpp"

#include "UCodeLang/Compliation/Helpers/InstructionBuilder.hpp"
#include "UCodeLang/Compliation/Back/UCodeBackEnd/UCodeBackEnd.hpp"
#include "UCodeLang/Compliation/Helpers/ParseHelper.hpp"
#include "UCodeLang/Compliation/Helpers/NameDecoratior.hpp"
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
		case SymbolType::Enum:
		{
			ConveToIRVariantEnum(*Item);
		}
		break;
		case SymbolType::Type_StaticArray:
		{
			ConveToStaticArray(*Item);
		}
		break;
		case SymbolType::StaticVarable:
		{
			auto StaticVarIR = _Builder.NewStaticVarable(_Builder.ToID(Item->FullName),ConvertToIR(Item->VarType));
		}
		break;
		case SymbolType::ThreadVarable:
		{
			auto ThreadVarIR = _Builder.NewThreadLocalVarable(_Builder.ToID(Item->FullName), ConvertToIR(Item->VarType));
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
		PushToNodeScope(*node.get());
		switch (node->Get_Type())
		{
		case NodeType::NamespaceNode:OnNamespace(*NamespaceNode::As(node.get())); break;
		case NodeType::AttributeNode:OnAttributeNode(*AttributeNode::As(node.get())); break;
		case NodeType::ClassNode: OnClassNode(*ClassNode::As(node.get())); break;
		case NodeType::AliasNode:OnAliasNode(*AliasNode::As(node.get())); break;
		case NodeType::EnumNode:OnEnum(*EnumNode::As(node.get())); break;
		case NodeType::FuncNode:OnFuncNode(*FuncNode::As(node.get())); break;
		case NodeType::UsingNode: OnUseingNode(*UsingNode::As(node.get())); break;
		case NodeType::TraitNode:OnTrait(*TraitNode::As(node.get())); break;
		case NodeType::TagTypeNode:OnTag(*TagTypeNode::As(node.get())); break;
		default:break;
		}
		PopNodeScope();
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
		PushToNodeScope(*node.get());
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
		case NodeType::TraitNode:OnTrait(*TraitNode::As(node.get())); break;
		case NodeType::TagTypeNode:OnTag(*TagTypeNode::As(node.get())); break;
		default:break;
		}
		PopNodeScope();
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
		
		_ClassStack.push({ ClassInf });
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
			PushToNodeScope(*node.get());
			switch (node->Get_Type())
			{
			case NodeType::AttributeNode:OnAttributeNode(*AttributeNode::As(node.get())); break;
			case NodeType::ClassNode: OnClassNode(*ClassNode::As(node.get())); break;
			case NodeType::AliasNode:OnAliasNode(*AliasNode::As(node.get())); break;
			case NodeType::EnumNode:OnEnum(*EnumNode::As(node.get())); break;
			case NodeType::UsingNode: OnUseingNode(*UsingNode::As(node.get())); break;
			case NodeType::FuncNode:OnFuncNode(*FuncNode::As(node.get())); break;
			case NodeType::DeclareVariableNode:OnDeclareVariablenode(*DeclareVariableNode::As(node.get()), DeclareStaticVariableNode_t::ClassField); break;
			case NodeType::DeclareStaticVariableNode:OnDeclareStaticVariableNode(*DeclareStaticVariableNode::As(node.get())); break;
			case NodeType::DeclareThreadVariableNode:OnDeclareThreadVariableNode(*DeclareThreadVariableNode::As(node.get())); break;
			case NodeType::TraitNode:OnTrait(*TraitNode::As(node.get())); break;
			case NodeType::TagTypeNode:OnTag(*TagTypeNode::As(node.get())); break;
			default:break;
			}
			PopNodeScope();
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
	
	
	
	
	if (passtype == PassType::FixedTypes)//auto make funcs
	{
		auto ThisCallType = TypeSymbol(Syb.ID);
		
		if ( (Node.Inherited.Values.size() || ClassInf->_WillHaveFielddeInit) && GetSymbol(_Table._Scope.GetApendedString((String)ClassDestructorFunc),SymbolType::Func) == nullptr)
		{
			NewDropFuncSymbol(ClassInf, ThisCallType);
			ClassInf->_ClassAutoGenerateDrop = true;
		}
		

		{//ClassConstructorfuncs
			bool HasDefaultConstructor = false;
			bool HasCopyConstructor = false;
			bool HasMoveConstructor = false;

			const auto& Funcs = _Table.GetSymbolsWithName((String)ClassConstructorfunc, SymbolType::Func);
			for (auto& Item : Funcs)
			{
				if (Item->Type == SymbolType::Func)
				{
					FuncInfo* V = Item->Get_Info<FuncInfo>();
					if (V->Pars.size() == 1)
					{
						HasDefaultConstructor = true;
					}
					if (V->Pars.size() == 2)
					{
						auto& OtherPar = V->Pars[1];
						if (OtherPar.IsAddress() && OtherPar._CustomTypeSymbol == SybID)
						{

							if (OtherPar._MoveData == MoveData::None)
							{
								HasCopyConstructor = true;
							}
							else
							{
								HasMoveConstructor = true;
							}

						}
					}
				}
			}
		
		
			if (HasDefaultConstructor == false)
			{
				auto DropFunc = &AddSybol(SymbolType::Func,ClassConstructorfunc, _Table._Scope.GetApendedString((String)ClassConstructorfunc));
				FuncInfo* V = new FuncInfo();
				DropFunc->Info.reset(V);

				V->FullName = DropFunc->FullName;
				V->_FuncType = FuncInfo::FuncType::New;
				V->Ret = TypesEnum::Void;
				V->FrontParIsUnNamed = true;

				auto ThisParType = ThisCallType;
				ThisParType._IsAddress = true;
				V->Pars.push_back(ThisParType);
				ClassInf->_ClassAutoGenerateDefaultConstructor = true;

				DropFunc->PassState = PassType::FixedTypes;
			}
		}


		//Inherited Values
		for (auto& Item : Node.Inherited.Values)
		{
			auto& Str = Item.Token->Value._String;

			auto Syb = GetSymbol(Str, SymbolType::Type);

			if (Syb == nullptr)
			{
				LogCantFindTypeError(Item.Token, Str);
				continue;
			}

			if (Syb->Type != SymbolType::Trait_class)
			{
				LogExpectedSymbolToBea(Item.Token, *Syb, SymbolType::Trait_class);
				continue;
			}

			/*
			for (auto& Item2 : ClassInf->_InheritedTypes)
			{
				if (Syb == Item2)
				{
					_ErrorsOutput->AddError(ErrorCodes::InValidType, Item.Token->OnLine, Item.Token->OnPos,
						"duplicate Inherit Trait");
				}
			}
			*/


			ClassInfo_InheritTypeInfo _Data;
			_Data.Syb = Syb;
			ClassInf->_InheritedTypes.push_back(_Data);
		

		}

		for (auto& Item : ClassInf->_InheritedTypes)
		{
			InheritTrait(&Syb, ClassInf, Item.Syb,Node.ClassName.Token);
		}
	}
	if (passtype == PassType::BuidCode)
	{
		if (ClassInf->_ClassAutoGenerateDrop)
		{
			auto OldFunc = LookingAtIRFunc;
			auto OldBlock = LookingAtIRBlock;
			//

			auto FuncSyb = GetSymbol(_Table._Scope.GetApendedString((String)ClassDestructorFunc), SymbolType::Func);
			auto Funcinfo = FuncSyb->Get_Info<FuncInfo>();

			LookingAtIRFunc = _Builder.NewFunc(GetIRID(Funcinfo), ConvertToIR(Funcinfo->Ret));
			
			IRType ThisPar = ConvertToIR(Funcinfo->Pars.front());
			{
				IRPar par = IRPar();
				par.identifier = _Builder.ToID(ThisSymbolName);
				par.type = ThisPar;
				LookingAtIRFunc->Pars.push_back(par);
			}
			LookingAtIRBlock = LookingAtIRFunc->NewBlock(".");


			if (ClassInf->_WillHaveFielddeInit)
			{
				auto ClassInit = _Table._Scope.GetApendedString((String)ClassDeInitializefuncName);

				LookingAtIRBlock->NewPushParameter(LookingAtIRBlock->NewLoad(&LookingAtIRFunc->Pars.front()));
				LookingAtIRBlock->NewCall(_Builder.ToID(ClassInit));

				
			}LookingAtIRBlock->NewRet();

			//
			LookingAtIRFunc = OldFunc;
			LookingAtIRBlock = OldBlock;
		}


		if (ClassInf->_WillHaveFielddeInit)
		{//de-init func
			auto ThisCallType = TypeSymbol(Syb.ID);

			FuncInfo V = FuncInfo();
			{
				V.FullName = _Table._Scope.GetApendedString((String)ClassDeInitializefuncName);
				V._FuncType = FuncInfo::FuncType::Drop;
				V.Ret = TypesEnum::Void;

				auto ThisParType = ThisCallType;
				ThisParType._IsAddress = true;
				V.Pars.push_back(ThisParType);
			}
		

			auto OldFunc = LookingAtIRFunc;
			auto OldBlock = LookingAtIRBlock;
			//
			LookingAtIRFunc = _Builder.NewFunc(GetIRID(&V), ConvertToIR(V.Ret));
			IRType ThisPar = ConvertToIR(V.Pars.front());
			{
				IRPar par = IRPar();
				par.identifier = _Builder.ToID(ThisSymbolName);
				par.type = ThisPar;
				LookingAtIRFunc->Pars.push_back(par);
			}

			LookingAtIRBlock = LookingAtIRFunc->NewBlock(".");
			{
				BuildFuncDropUsesingFields(ClassInf, ThisPar);
				LookingAtIRBlock->NewRet();
			}

			//
			LookingAtIRFunc = OldFunc;
			LookingAtIRBlock = OldBlock;
		}
	
		if (ClassInf->_ClassAutoGenerateDefaultConstructor)
		{
			auto ThisCallType = TypeSymbol(Syb.ID);

			FuncInfo V = FuncInfo();
			{
				V.FullName = _Table._Scope.GetApendedString((String)ClassConstructorfunc);
				V._FuncType = FuncInfo::FuncType::New;
				V.Ret = TypesEnum::Void;

				auto ThisParType = ThisCallType;
				ThisParType._IsAddress = true;
				V.Pars.push_back(ThisParType);
			}


			auto OldFunc = LookingAtIRFunc;
			auto OldBlock = LookingAtIRBlock;
			//
			LookingAtIRFunc = _Builder.NewFunc(GetIRID(&V), ConvertToIR(V.Ret));
			IRType ThisPar = ConvertToIR(V.Pars.front());
			{
				IRPar par = IRPar();
				par.identifier = _Builder.ToID(ThisSymbolName);
				par.type = ThisPar;
				LookingAtIRFunc->Pars.push_back(par);
			}

			LookingAtIRBlock = LookingAtIRFunc->NewBlock(".");
			{
				if (ClassInf->_ClassFieldInit)
				{
					auto ClassInit = _Table._Scope.GetApendedString((String)ClassInitializefuncName);

					LookingAtIRBlock->NewPushParameter(LookingAtIRBlock->NewLoad(&LookingAtIRFunc->Pars.front()));
					LookingAtIRBlock->NewCall(_Builder.ToID(ClassInit));

					
				}LookingAtIRBlock->NewRet();
			}

			//
			LookingAtIRFunc = OldFunc;
			LookingAtIRBlock = OldBlock;
		}

		if (ClassInf->_ClassFieldInit)
		{
			ClassInf->_ClassFieldInit->Blocks[0]->NewRet();
		}

		for (auto& Item : ClassInf->_InheritedTypes)
		{
			BuildTrait(&Syb, ClassInf, Item.Syb, Node.ClassName.Token);
		}
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
			ConvertAndValidateType(node.Type, Syb.VarType,NodeSyb_t::Any);
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
				ConvertAndValidateType(NodePar.Type, Par,NodeSyb_t::Parameter);
			}

			ConvertAndValidateType(node_->ReturnType, V->Ret,NodeSyb_t::Ret);

			Syb.VarType.SetType(SybID);

			Syb.Type = SymbolType::Func_ptr;
		}
	}
	if (passtype == PassType::FixedTypes)
	{
		if (node._Type == AliasType::Type) 
		{
			ConvertAndValidateType(node.Type, Syb.VarType,NodeSyb_t::Any);
		}
		else
		{
			AliasNode_Func* node_ = (AliasNode_Func*)node._Node.get();
			FuncPtrInfo* nodeinfo_ = (FuncPtrInfo*)Syb.Info.get();

			for (size_t i = 0; i < nodeinfo_->Pars.size(); i++)
			{
				auto& NodePar = node_->Parameters.Parameters[i];
				auto& Par = nodeinfo_->Pars[i];
				ConvertAndValidateType(NodePar.Type, Par, NodeSyb_t::Parameter);
			}

			ConvertAndValidateType(node_->ReturnType, nodeinfo_->Ret,NodeSyb_t::Ret);
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

	FuncGetName(NameToken, FuncName, FuncType);



	_Table.AddScope(FuncName);
	auto FullName = _Table._Scope.ThisScope;




	Symbol* syb;
	SymbolID sybId = GetSymbolID(node);


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
				_Table.AddSymbolID(*GenericType, ID);

				GenericData Info;
				Info.SybID = ID;
				Info.IsConstantExpression = Item.IsConstantExpression;

				newInfo->_Generic.push_back(Info);
			}

		}


		{
			auto& RetType = node.Signature.ReturnType.node;
			if (RetType && RetType->Get_Type() == NodeType::AnonymousTypeNode)
			{
				auto NewName = GetFuncAnonymousObjectFullName(FullName);


				SymbolID AnonymousSybID = (SymbolID)RetType.get();
				auto& AnonymousSyb = AddSybol(SymbolType::Type_class, (String)NewName, NewName);

				_Table.AddSymbolID(AnonymousSyb, AnonymousSybID);


				auto ClassInf = new ClassInfo();
				ClassInf->FullName = NewName;
				AnonymousSyb.Info.reset(ClassInf);
				AnonymousSyb.VarType.SetType(AnonymousSyb.ID);

				AnonymousTypeNode* Typenode = AnonymousTypeNode::As(RetType.get());
				for (auto& Item3 : Typenode->Fields.Parameters)
				{
					ClassInf->AddField(Item3.Name.AsString(), ConvertAndValidateType(Item3.Type,NodeSyb_t::Parameter));
				}
			}
		}



		auto ClassSymBool = _ClassStack.size() ? GetSymbol(_ClassStack.top().Info) : nullptr;
		for (auto& Item : node.Signature.Parameters.Parameters)
		{

			String_view GenericTypeName;
			if (Item.Name.Token == nullptr)
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
			newInfo->Pars.push_back(ConvertAndValidateType(Item.Type,NodeSyb_t::Parameter));
		}

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
				ConvertAndValidateType(Item3.Type, ItemOut.Type,NodeSyb_t::Parameter);
			}


			PushClassDependencie(ClassInf);

			UAddress ClassSize = 0;
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
			ConvertAndValidateType(node.Signature.ReturnType, syb->VarType,NodeSyb_t::Ret);
			Info->Ret = syb->VarType;
		}



		for (size_t i = 0; i < node.Signature.Parameters.Parameters.size(); i++)
		{
			auto& Item = node.Signature.Parameters.Parameters[i];
			auto& Item2 = Info->Pars[i];


			auto ParSybID = (SymbolID)&Item;
			auto& Sybol = *GetSymbol(ParSybID);
			ConvertAndValidateType(Item.Type, Sybol.VarType,NodeSyb_t::Parameter);
			Item2 = Sybol.VarType;

		}

	}




	bool buidCode = passtype == PassType::BuidCode;
	bool ignoreBody = !IsgenericInstantiation && IsGenericS;


	if (buidCode)
	{

		auto DecName = MangleName(Info);
		LookingAtIRFunc = _Builder.NewFunc(GetIRID(Info), {});
		LookingAtIRBlock = LookingAtIRFunc->NewBlock("");
		PushNewStackFrame();

		auto& ParNodes = node.Signature.Parameters.Parameters;



		LookingAtIRFunc->Pars.resize(ParNodes.size());//becuase we are useing ptrs.
		for (size_t i = 0; i < ParNodes.size(); i++)
		{
			auto& Item = ParNodes[i];

			auto ParSybID = (SymbolID)&Item;
			auto& V = *GetSymbol(ParSybID);


			auto& VarType = Info->Pars[i];

			auto& d = LookingAtIRFunc->Pars[i];
			d.identifier = _Builder.ToID(ScopeHelper::GetNameFromFullName(V.FullName));
			d.type = ConvertToIR(VarType);




			if (HasDestructor(VarType))
			{
				ObjectToDrop V;
				V.DropType = ObjectToDropType::Operator;
				V.ID = ParSybID;
				V._Operator = IROperator(&d);
				V.Type = VarType;

				StackFrames.back().OnEndStackFrame.push_back(V);
			}

			V.IR_Par = &d;
		}
		LookingAtIRFunc->ReturnType = ConvertToIR(Info->Ret);

		for (auto& Item : _TepAttributes)
		{
			String AttType;
			Item->ScopedName.GetScopedName(AttType);
			if (AttType == "CPPCall")
			{
				LookingAtIRFunc->IsCPPCall = true;
				break;
			}
		}


		ClassData* Ptr = GetAssemblyClass(FullName);

		ClassMethod V;
		V.FullName = FullName;
		V.DecorationName = DecName;
		V.RetType = ConvertToTypeInfo(Info->Ret);

		for (auto& Item : Info->Pars)
		{
			auto F = V.ParsType.emplace_back();
			F = ConvertToTypeInfo(Item);
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
		//


		if (FuncType == FuncInfo::FuncType::New)
		{
			auto& Classinfo = _ClassStack.top().Info;
			if (Classinfo->_WillHaveFieldInit)
			{
				auto InitFunc = Classinfo->FullName;
				ScopeHelper::GetApendedString(InitFunc, ClassInitializefuncName);


				LookingAtIRBlock->NewPushParameter(LookingAtIRBlock->NewLoad(&LookingAtIRFunc->Pars.front()));
				LookingAtIRBlock->NewCall(_Builder.ToID(InitFunc));
			}
		}
		//
	}

	if (node.Body.has_value() && !ignoreBody)
	{
		auto& Body = node.Body.value();
		OnStatementsWithSetableRet(Body.Statements, Info->Ret, node.Signature.Name.Token);
		syb->VarType = Info->Ret;
	}


	if (buidCode && !ignoreBody)
	{
		PopStackFrame();


		if (FuncType == FuncInfo::FuncType::Drop)
		{

			auto& Classinfo = _ClassStack.top().Info;
			if (Classinfo->_WillHaveFielddeInit) 
			{
				auto InitFunc = Classinfo->FullName;
				ScopeHelper::GetApendedString(InitFunc, ClassInitializefuncName);

				LookingAtIRBlock->NewPushParameter(LookingAtIRBlock->NewLoad(&LookingAtIRFunc->Pars.front()));
				LookingAtIRBlock->NewCall(_Builder.ToID(InitFunc));
				LookingAtIRBlock->NewRet();
			}
			
		}

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
void SystematicAnalysis::SetInStatetements(bool Value)
{
	if (_ClassStack.size())
	{
		_ClassStack.top()._InStatements = Value;
	}
}
void SystematicAnalysis::FuncGetName(const UCodeLang::Token* NameToken, std::string_view& FuncName, UCodeLang::FrontEnd::FuncInfo::FuncType& FuncType)
{
	bool ObjectOverLoad = false;
	switch (NameToken->Type)
	{
	case TokenType::KeyWord_new:
		FuncName = ClassConstructorfunc;
		FuncType = FuncInfo::FuncType::New;
		ObjectOverLoad = true;
		break;
	case TokenType::KeyWord_Drop:
		FuncName = ClassDestructorFunc;
		FuncType = FuncInfo::FuncType::Drop;
		ObjectOverLoad = true;
		break;
	case TokenType::Left_Bracket:
		FuncName = Overload_Index_Func;
		FuncType = FuncInfo::FuncType::Index;
		ObjectOverLoad = true;
		break;
	case TokenType::Left_Parentheses:
		FuncName = Overload_Invoke_Func;
		FuncType = FuncInfo::FuncType::Invoke;
		ObjectOverLoad = true;
		break;
	case TokenType::KeyWord_for:
		FuncName = Overload_For_Func;
		FuncType = FuncInfo::FuncType::For;
		ObjectOverLoad = true;
		break;
	case TokenType::RightArrow:
		FuncName = Overload_Cast_Func;
		FuncType = FuncInfo::FuncType::Cast;
		ObjectOverLoad = true;
		break;
	case TokenType::Name:
		break;
	default:


		for (auto& Item : Systematic_BinaryOverloadData::Data)
		{
			if (NameToken->Type == Item.token)
			{
				FuncName = Item.CompilerName;
				FuncType = Item.Type;
				ObjectOverLoad = true;
				goto DoStuff;
			}
		}

		for (auto& Item : Systematic_PostfixOverloadData::Data)
		{
			if (NameToken->Type == Item.token)
			{
				FuncName = Item.CompilerName;
				FuncType = Item.Type;
				ObjectOverLoad = true;
				goto DoStuff;
			}
		}

		for (auto& Item : Systematic_UrinaryOverloadData::Data)
		{
			if (NameToken->Type == Item.token)
			{
				FuncName = Item.CompilerName;
				FuncType = Item.Type;
				ObjectOverLoad = true;
				goto DoStuff;
			}
		}

		for (auto& Item : Systematic_CompoundOverloadData::Data)
		{
			if (NameToken->Type == Item.token)
			{
				FuncName = Item.CompilerName;
				FuncType = Item.Type;
				ObjectOverLoad = true;
				goto DoStuff;
			}
		}

		for (auto& Item : Systematic_MemberOverloadData::Data)
		{
			if (NameToken->Type == Item.token)
			{
				FuncName = Item.CompilerName;
				FuncType = Item.Type;
				ObjectOverLoad = true;
				goto DoStuff;
			}
		}

		LogCantOverLoadOverload(NameToken);

		break;
	}

	DoStuff:
	if (ObjectOverLoad) 
	{
		if (_ClassStack.empty())
		{
			CantUseThisKeyWordHereError(NameToken);
		}
	}
}
ClassData* SystematicAnalysis::GetAssemblyClass(const String& FullName)
{
	if (_ClassStack.empty())
	{
		auto& Assembly = _Lib.Get_Assembly();

		auto globalAssemblyObjectName = (String_view)ScopeHelper::_globalAssemblyObject;

		auto Ptr =  Assembly.Find_Class(globalAssemblyObjectName);
		if (Ptr == nullptr)
		{
			return &_Lib.Get_Assembly().AddClass(String(globalAssemblyObjectName), String(globalAssemblyObjectName));
		}
		return Ptr;
	}
	else
	{
		auto& Assembly = _Lib.Get_Assembly();
		auto ClassName = ScopeHelper::GetReMoveScope((String_view)FullName);
		for (auto& Item : Assembly.Classes)
		{
			if (Item->FullName == ClassName)
			{
				return Item.get();
			}

		}
	}
	throw std::exception("bad path");
}
void SystematicAnalysis::FuncRetCheck(const Token& Name, const Symbol* FuncSyb, const FuncInfo* Func)
{
	switch (Func->_FuncType)
	{
	case FuncInfo::FuncType::Drop:
	{
		TypeSymbol V(TypesEnum::Void);
		if (!AreTheSame(Func->Ret, V))
		{
			LogFuncMustBe(&Name, FuncSyb->FullName, V);
		}
		if (Func->Pars.size() != 1)
		{
			LogPostfixOverloadPars(Name, Func);
		}
		break;
	}
	case FuncInfo::FuncType::New:
	{

		TypeSymbol V(TypesEnum::Void);
		if (!AreTheSame(Func->Ret, V)) 
		{
			LogFuncMustBe(&Name, FuncSyb->FullName, V);
		}
	}
	break;
	case FuncInfo::FuncType::Index:
	{
		if (Func->Pars.size() != 2)
		{
			LogIndexOverloadPars(Name, Func);
		}
	}
	break;
	case FuncInfo::FuncType::Cast:
	{
		if (Func->Pars.size() != 1)
		{
			LogPostfixOverloadPars(Name, Func);
		}
	}
	break;
	case FuncInfo::FuncType::NameFunc:
		break;
	default:
		if (Systematic_BinaryOverloadData::IsBinaryOverload(Func->_FuncType))
		{
			if (Func->Pars.size() != 2) 
			{
				LogBinaryOverloadPars(Name, Func);
			}
		}
		else if (Systematic_PostfixOverloadData::IsPostfixOverload(Func->_FuncType))
		{
			if (Func->Pars.size() != 1)
			{
				LogPostfixOverloadPars(Name, Func);
			}
		}
		else if (Systematic_CompoundOverloadData::IsCompoundOverload(Func->_FuncType))
		{
			if (Func->Pars.size() != 2)
			{
				LogBinaryOverloadPars(Name, Func);
			}
		}
		else if (Systematic_UrinaryOverloadData::IsUrinaryOverload(Func->_FuncType))
		{
			if (Func->Pars.size() != 1)
			{
				LogPostfixOverloadPars(Name, Func);
			}
		}
		else if (Systematic_MemberOverloadData::IsMemerOverload(Func->_FuncType))
		{
			if (Func->Pars.size() != 1)
			{
				LogPostfixOverloadPars(Name, Func);
			}
		}
		break;
	}
}
void SystematicAnalysis::OnForNode(const ForNode& node)
{
	auto& StrVarName = node.Name->Value._String;
	auto FullName = _Table._Scope.GetApendedString(StrVarName);

	SymbolID sybId = GetSymbolID(node);
	Symbol* syb;

	String ScopeName = std::to_string((size_t)&node);

	PushNewStackFrame();
	_Table.AddScope(ScopeName);

	if (passtype == PassType::GetTypes)
	{
		DoSymbolRedefinitionCheck(FullName, SymbolType::StackVarable, node.Name);


		syb = &AddSybol(SymbolType::StackVarable, (String)StrVarName, FullName);
		_Table.AddSymbolID(*syb, sybId);


		if (node.Type == ForNode::ForType::Traditional)
		{
			TypeSymbol BoolType(TypesEnum::Bool);

			LookingForTypes.push(BoolType);
			OnExpressionTypeNode(node.BoolExpression.Value.get(),GetValueMode::Read);
			LookingForTypes.pop();
		
			OnPostfixVariableNode(node.OnNextStatement);
		}
		else
		{
			OnExpressionTypeNode(node.Modern_List.Value.get(), GetValueMode::Read);
		}

		for (const auto& node2 : node.Body._Nodes)
		{
			OnStatement(*node2);
		}
	}
	else
	{
		syb = GetSymbol(sybId);
	}

	if (passtype == PassType::FixedTypes)
	{
		if (node.Type == ForNode::ForType::Traditional)
		{
			{
				auto& VarType = syb->VarType;
				Convert(node.TypeNode, VarType);
				VarType.SetAsLocation();


				auto Ex = node.Traditional_Assignment_Expression.Value.get();
				ExTypeDeclareVarableCheck(VarType, Ex, node.TypeNode.Name.Token);
				
				if (node.Traditional_Assignment_Expression.Value)
				{
					OnExpressionTypeNode(node.Traditional_Assignment_Expression.Value.get(), GetValueMode::Read);

					
					syb->SetTovalid();

					auto& VarType = syb->VarType;


					auto& Ex = LastExpressionType;
					auto Token = node.TypeNode.Name.Token;
					ExDeclareVariableTypeCheck(VarType, Ex, Token);
				}
			}

			{
				TypeSymbol BoolType(TypesEnum::Bool);

				LookingForTypes.push(BoolType);
				OnExpressionTypeNode(node.BoolExpression.Value.get(), GetValueMode::Read);
				LookingForTypes.pop();

				if (passtype == PassType::FixedTypes)
				{
					if (!CanBeImplicitConverted(LastExpressionType, BoolType))
					{
						auto  Token = LastLookedAtToken;
						LogCantCastImplicitTypes(Token, LastExpressionType, BoolType, true);
					}
				}
			}

			{
				OnPostfixVariableNode(node.OnNextStatement);
			}

			for (const auto& node2 : node.Body._Nodes)
			{
				OnStatement(*node2);
			}
		}
		else
		{
			{
				auto& VarType = syb->VarType;
				Convert(node.TypeNode, VarType);
				VarType.SetAsLocation();
			}

			auto Ex = node.Modern_List.Value.get();

			{
				LookingForTypes.push(TypeSymbol(TypesEnum::Any));
				OnExpressionTypeNode(Ex, GetValueMode::Read);
				LookingForTypes.pop();
			}
			auto& ExType = LastExpressionType;

			auto HasInfo = HasForOverLoadWith(ExType);
			if (!HasInfo.HasValue)
			{
				auto  Token = LastLookedAtToken;
				TypeDoesNotHaveForOverload(Token, ExType);

			}
			else
			{
				if (HasInfo.Value.value())
				{
					FuncInfo* FuncSym = HasInfo.Value.value()->Get_Info< FuncInfo>();
					const auto& TypeForType = FuncSym->Ret;

					Optional<Symbol*> GetFunc = {};

					if (syb->VarType.IsAn(TypesEnum::Var))
					{
						auto tep = GetAnExplicitlyConvertedFunc(TypeForType);
						if (tep.has_value())
						{
							GetFunc = GetSymbol(tep.value());
						}
					}
					else
					{
						auto Tep = CanBeExplicitlyConverted(TypeForType, syb->VarType);
						if (Tep.HasValue && Tep.Value.has_value())
						{
							GetFunc = Tep.Value.value();
						}
					}

					Optional<Symbol*>CheckFunc = {};
					auto V = HasUrinaryOverLoadWith(TypeForType, TokenType::QuestionMark);

					if (V.HasValue && V.Value.has_value() )
					{
						auto BoolType = TypeSymbol(TypesEnum::Bool);
						auto retType = V.Value.value()->VarType;
						if (CanBeImplicitConverted(retType, BoolType, false)) 
						{
							CheckFunc = V.Value.value();
						}
						else
						{
							auto  Token = LastLookedAtToken;
							LogCantCastImplicitTypes(Token, BoolType, retType, false);
						}
					}

					if (!GetFunc.has_value())
					{
						const Token* token = node.TypeNode.Name.Token;

						if (syb->VarType.IsAn(TypesEnum::Var)) {
							_ErrorsOutput->AddError(ErrorCodes::InValidType, token->OnLine, token->OnPos,
								"The Type '" + ToString(TypeForType) + "' has no cast(->) overload.it is needed to access the object for the 'for' loop.");
						}
						else
						{
							_ErrorsOutput->AddError(ErrorCodes::InValidType, token->OnLine, token->OnPos,
								"The Type '" + ToString(TypeForType) + "' has no explicit cast(->) overload for the type '" + ToString(syb->VarType) + "'.it is needed to access the object for  the 'for' loop.");
						}

						syb->VarType.SetType(TypesEnum::Null);
					}
					else if (!CheckFunc.has_value())
					{
						const Token* token = node.TypeNode.Name.Token;

						_ErrorsOutput->AddError(ErrorCodes::InValidType, token->OnLine, token->OnPos,
							"The Type '" + ToString(TypeForType) + "' has no exist(?) overload.it is needed to check when to end the loop.");

						syb->VarType.SetType(TypesEnum::Null);
					}
					else
					{
						ForExpresion_Data g;
						g.FuncGetLoopAble = HasInfo.Value.value();
						g.FuncToGet = GetFunc.value();
						g.FuncToCheck = CheckFunc.value();
						For_Datas.AddValue(&node, g);


						
						auto Token = node.TypeNode.Name.Token;
						ExDeclareVariableTypeCheck(syb->VarType, g.FuncToGet->Get_Info<FuncInfo>()->Ret, Token);
					}
				}
				else
				{
					throw std::exception("bad path");
				}
			}


			for (const auto& node2 : node.Body._Nodes)
			{
				OnStatement(*node2);
			}
		}

	}

	if (passtype == PassType::BuidCode)
	{

		if (node.Type == ForNode::ForType::Traditional)
		{
			IRInstruction* OnVarable{};
			bool IsStructObjectPassRef = false;
			if (node.Traditional_Assignment_Expression.Value)
			{


				OnVarable = LookingAtIRBlock->NewLoad(ConvertToIR(syb->VarType));
				syb->IR_Ins = OnVarable;


				IsStructObjectPassRef = ISStructPassByRef(syb);

				if (IsStructObjectPassRef)
				{
					IRlocations.push({ OnVarable ,false });
				}


				OnExpressionTypeNode(node.Traditional_Assignment_Expression.Value.get(), GetValueMode::Read);

				DoImplicitConversion(_LastExpressionField, LastExpressionType, syb->VarType);

				OnStoreVarable(IsStructObjectPassRef, OnVarable, syb, sybId);

				AddDependencyToCurrentFile(syb->VarType);
			}


			{
				TypeSymbol BoolType(TypesEnum::Bool);
				LookingForTypes.push(BoolType);

				size_t BoolCode;
				size_t BoolJumps;
				if (passtype == PassType::BuidCode)
				{
					BoolCode = LookingAtIRBlock->GetIndex();
					BoolJumps =GetJumpsIndex();
				}
				OnExpressionTypeNode(node.BoolExpression.Value.get(), GetValueMode::Read);

				DoImplicitConversion(_LastExpressionField, LastExpressionType, BoolType);

				IRInstruction* BoolCode2 = _LastExpressionField;
				IRBlock::NewConditionalFalseJump_t IfIndex = LookingAtIRBlock->NewConditionalFalseJump(BoolCode2);


				LookingForTypes.pop();


				for (const auto& node2 : node.Body._Nodes)
				{
					OnStatement(*node2);
				}

				OnPostfixVariableNode(node.OnNextStatement);

				LookingAtIRBlock->NewJump(BoolCode);

				size_t BreakCode = LookingAtIRBlock->GetIndex();

				LookingAtIRBlock->UpdateConditionaJump(IfIndex.ConditionalJump, IfIndex.logicalNot, BreakCode);


				DoJumpsBreakAndContiunes(BoolJumps,BoolCode, BreakCode);
			}

		}
		else
		{

			const ForExpresion_Data& Data = For_Datas.at(&node);
			{
				AddDependencyToCurrentFile(Data.FuncGetLoopAble);
				AddDependencyToCurrentFile(Data.FuncToCheck);
				AddDependencyToCurrentFile(Data.FuncToGet);
			}

			{
				auto finfo = Data.FuncGetLoopAble->Get_Info<FuncInfo>();
				auto Ex = node.Modern_List.Value.get();
				LookingForTypes.push(finfo->Pars[0]);
				OnExpressionTypeNode(Ex, GetValueMode::Read);
				LookingForTypes.pop();

				AddDestructorToStack(LastExpressionType,_LastExpressionField);
			}
			
			{
				Get_FuncInfo f;
				f.Func = Data.FuncGetLoopAble->Get_Info<FuncInfo>();
				f.SymFunc = Data.FuncGetLoopAble;
				f.ThisPar = Get_FuncInfo::ThisPar_t::PushFromLast;
				DoFuncCall(f, {}, {});
			}
			IRInstruction* Loopobject = _LastExpressionField;
			auto LoopObjectType = LastExpressionType;
			{
				TypeSymbol BoolType(TypesEnum::Bool);
				LookingForTypes.push(BoolType);
				auto BoolCode = LookingAtIRBlock->GetIndex();
				auto BoolJumps = GetJumpsIndex();
				{//get if check
					Get_FuncInfo f;
					f.Func = Data.FuncToCheck->Get_Info<FuncInfo>();
					f.SymFunc = Data.FuncToCheck;
					f.ThisPar = Get_FuncInfo::ThisPar_t::PushFromLast;

					
					if (f.Func->Pars[0].IsAddress())
					{
						if (!LoopObjectType.IsAddress())
						{
							_LastExpressionField = LookingAtIRBlock->NewLoadPtr(Loopobject);
						}
					}

					DoFuncCall(f, {}, {});

					DoImplicitConversion(_LastExpressionField, LastExpressionType, TypesEnum::Bool);
				}
				LookingForTypes.pop();

				IRInstruction* BoolCode2 = _LastExpressionField;
				IRBlock::NewConditionalFalseJump_t IfIndex = LookingAtIRBlock->NewConditionalFalseJump(BoolCode2);

				{//get item
					_LastExpressionField = Loopobject;
					Get_FuncInfo f;
					f.Func = Data.FuncToGet->Get_Info<FuncInfo>();
					f.SymFunc = Data.FuncToGet;
					f.ThisPar = Get_FuncInfo::ThisPar_t::PushFromLast;

					if (f.Func->Pars[0].IsAddress())
					{
						if (!LoopObjectType.IsAddress())
						{
							_LastExpressionField = LookingAtIRBlock->NewLoadPtr(Loopobject);
						}
					}

					DoFuncCall(f, {}, {});

					DoImplicitConversion(_LastExpressionField, LastExpressionType, syb->VarType);
					auto FuncRet = _LastExpressionField;
					syb->IR_Ins = FuncRet;
					AddDestructorToStack(syb, syb->ID,syb->IR_Ins);
				}

				for (const auto& node2 : node.Body._Nodes)
				{
					OnStatement(*node2);
				}


				LookingAtIRBlock->NewJump(BoolCode);

				size_t BreakCode = LookingAtIRBlock->GetIndex();

				LookingAtIRBlock->UpdateConditionaJump(IfIndex.ConditionalJump, IfIndex.logicalNot, BreakCode);
			
				
			

				
				DoJumpsBreakAndContiunes(BoolJumps,BoolCode, BreakCode);
			}
			
			
		}
	}

	_Table.RemoveScope();

	PopStackFrame();
}
void SystematicAnalysis::DoJumpsBreakAndContiunes(size_t JumpIndex, size_t BoolCode, size_t BreakCode)
{
	for (size_t i = JumpIndex; i < _Jumps.size(); i++)
	{
		auto& Item = _Jumps[i];

		switch (Item.Type)
		{
		case Jumps_t::Break:
		{
			LookingAtIRBlock->UpdateJump(Item.JumpIns, BreakCode);
		}
		break;
		case Jumps_t::Continue:
		{
			LookingAtIRBlock->UpdateJump(Item.JumpIns, BoolCode);
		}
		break;
		default:
			break;
		}
	}
	RemoveJumps(JumpIndex);
}
void SystematicAnalysis::OnContinueNode(const ContinueNode& node)
{
	if (passtype == PassType::GetTypes)
	{

	}


	if (passtype == PassType::BuidCode)
	{

		JumpsData v;
		v.Type = Jumps_t::Continue;
		v.JumpIns = LookingAtIRBlock->NewJump();
		_Jumps.push_back(v);
	}
}
void SystematicAnalysis::OnBreakNode(const BreakNode& node)
{
	if (passtype == PassType::GetTypes)
	{

	}

	if (passtype == PassType::BuidCode)
	{
		JumpsData v;
		v.Type = Jumps_t::Break;
		v.JumpIns = LookingAtIRBlock->NewJump();
		_Jumps.push_back(v);
	}
}
void SystematicAnalysis::OnLambdaNode(const LambdaNode& node)
{
	const String LambdaName = CompilerGenerated("Lambda") + std::to_string((uintptr_t)&node);
	if (passtype == PassType::GetTypes)
	{
		_Table.AddScope(LambdaName);

		auto& LambdaSym = AddSybol(SymbolType::LambdaObject, LambdaName, _Table._Scope.ThisScope);
		_Table.AddSymbolID(LambdaSym, (SymbolID)&node);
		LambdaSym.Info =Unique_ptr<LambdaInfo>(new LambdaInfo());


		for (auto& Item : node.Pars.Parameters)
		{
			auto& Sym = AddSybol(SymbolType::ParameterVarable, Item.Name.AsString()
				, _Table._Scope.ScopeHelper::GetApendedString(Item.Name.AsStringView()));

			_Table.AddSymbolID(Sym, (SymbolID)&Item);
		}

		if (node._Statements.has_value())
		{
			for (const auto& node2 : node._Statements.value()._Nodes)
			{
				OnStatement(*node2);
			}
		}

		_Table.RemoveScope();
	}
	else
	if (passtype == PassType::FixedTypes)
	{
		
		_Table.AddScope(LambdaName);

		auto& LambdaSym = _Table.GetSymbol((SymbolID)&node);
		LambdaInfo* Info = LambdaSym.Get_Info<LambdaInfo>();
		Info->Ret = TypeSymbol(TypesEnum::Var);

		for (size_t i = 0; i < node.Pars.Parameters.size(); i++)
		{
			auto& Item = node.Pars.Parameters[i];
			Symbol& Sym = _Table.GetSymbol((SymbolID)&Item);

			ConvertAndValidateType(Item.Type,Sym.VarType, NodeSyb_t::Parameter);
			Info->Pars.push_back(Sym.VarType);
		}
		
		


		if (node._Statements.has_value())
		{
			{
				VarableUseData Data;
				_Varable.push(std::move(Data));
			}

			OnStatementsWithSetableRet(node._Statements.value(), Info->Ret, node.LambdaStart);


			//
			{
				VarableUseData& UseData = _Varable.top();


				for (auto& Item : UseData._UsedSymbols)
				{
					LambdaFieldInfo NewValue;
					NewValue.Name = ScopeHelper::GetNameFromFullName(Item->FullName);
					NewValue.Type = Item->VarType;
					NewValue.Sym = Item;

					Info->_CapturedVarables.push_back(std::move(NewValue));
				}

				for (auto& Item : UseData._SymbolsToPassBecauseInerLamdba)
				{
					LambdaFieldInfo NewValue;
					NewValue.Name = ScopeHelper::GetNameFromFullName(Item->FullName);
					NewValue.Type = Item->VarType;
					NewValue.Sym = Item;

					Info->_IndirectCapturedVarables.push_back(std::move(NewValue));
				}

				if (_Varable.size() > 1)//copy Symbols
				{
					VarableUseData UseData2 = std::move(_Varable.top());_Varable.pop();

					VarableUseData& ThisData = _Varable.top();
					for (auto& Item : UseData2._UsedSymbols)
					{
						/*
						auto Item_Name = ScopeHelper::GetNameFromFullName(Item->FullName);
						auto Item_FullName = _Table._Scope.ThisScope;
						ScopeHelper::GetApendedString(Item_FullName, Item_Name);

						auto& Sym = AddSybol(SymbolType::Class_Field, Item_Name
							, Item_FullName);

						_Table.AddSymbolID(Sym, (SymbolID)&Item);
						*/

						ThisData._SymbolsToPassBecauseInerLamdba.push_back(Item);
					}
				}
				else
				{
					_Varable.pop();
				}

			}
			//
		}

		
		_Table.RemoveScope();

		{
			String LambdFuncScope = _Table._Scope.ThisScope;
			ScopeHelper::GetApendedString(LambdFuncScope, LambdaName);
			{
				Vector<LambdaFieldInfo> Tep_CapturedVarables;
				for (auto& Item : Info->_CapturedVarables)
				{
					if (Item.Sym->Type == SymbolType::ThreadVarable
						|| Item.Sym->Type == SymbolType::StaticVarable)
					{
						continue;
					}


					for (auto& Item2 : Tep_CapturedVarables)
					{
						if (Item.Name == Item2.Name)
						{
							goto OutLoop;
						}
					}


					for (size_t i = 0; i < node.Pars.Parameters.size(); i++)
					{
						auto& ParItem = node.Pars.Parameters[i];
						SymbolID ParID = (SymbolID)&ParItem;
						if (Item.Sym->ID == ParID)
						{
							goto OutLoop;
						}
					}


					if (Item.Sym->FullName.size() > LambdFuncScope.size())
					{
						continue;
					}

					Tep_CapturedVarables.push_back(Item);
				OutLoop:continue;
				}
				Info->_CapturedVarables = std::move(Tep_CapturedVarables);
			}
			//
			{

				Vector<LambdaFieldInfo> Tep_CapturedVarables;
				for (auto& Item : Info->_IndirectCapturedVarables)
				{
					if (Item.Sym->Type == SymbolType::ThreadVarable
						|| Item.Sym->Type == SymbolType::StaticVarable)
					{
						continue;
					}


					for (auto& Item2 : Tep_CapturedVarables)
					{
						if (Item.Name == Item2.Name)
						{
							goto OutLoop2;
						}
					}

					/*
					for (auto& Item2 : Info->_CapturedVarables)
					{
						if (Item.Name == Item2.Name)
						{
							goto OutLoop2;
						}
					}
					*/


					for (size_t i = 0; i < node.Pars.Parameters.size(); i++)
					{
						auto& ParItem = node.Pars.Parameters[i];
						SymbolID ParID = (SymbolID)&ParItem;
						if (Item.Sym->ID == ParID)
						{
							goto OutLoop2;
						}
					}


					if (Item.Sym->FullName.size() > LambdFuncScope.size())
					{
						continue;
					}

					Tep_CapturedVarables.push_back(Item);
				OutLoop2:continue;
				}
				Info->_IndirectCapturedVarables = std::move(Tep_CapturedVarables);


				for (auto& Item : Info->_IndirectCapturedVarables) 
				{
					const Token* Token =node.LambdaStart;

					_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos, "cant indirect pass the varable  '" + 
						Item.Name + "' between lambdas.be explicit and make a new varable and assign useing '"+ Item.Name + "' in this lambda.");

					Info->_CapturedVarables.push_back(Item);
				}
				
			}
		}

		if (Info->UsesOuterScope())
		{
			

			const String LambdaClassName = LambdaName + "class";
			String FullName = _Table._Scope.ThisScope;
			ScopeHelper::GetApendedString(FullName, LambdaClassName);
			auto& SymClass = AddSybol(SymbolType::Type_class, LambdaClassName, FullName);
		
			const SymbolID ClassSymID = (SymbolID)&node._Capture;

			_Table.AddSymbolID(SymClass, ClassSymID);

			ClassInfo* Classinfo = new ClassInfo();
			SymClass.Info =Unique_ptr<Symbol_Info>(Classinfo);

			Classinfo->FullName = SymClass.FullName;
			//

			bool HasDropCall = false;
			for (auto& Item : Info->_CapturedVarables)
			{
				Classinfo->AddField(Item.Name, Item.Type);
				
				if (HasDestructor(Item.Type))
				{
					HasDropCall = true;
				}
			}

			if (HasDropCall)
			{
				auto Sym = NewDropFuncSymbol(Classinfo, TypeSymbol(ClassSymID));
				Info->_ClassDrop = Sym->Get_Info<FuncInfo>();

			}


			{
				FuncInfo TepFuninfo;
				TepFuninfo.Pars = Info->Pars;
				TepFuninfo.Ret = Info->Ret;
				TepFuninfo.FrontParIsUnNamed = true;
				TepFuninfo._FuncType = FuncInfo::FuncType::Invoke;


				auto ClassPtr = TypeSymbol(ClassSymID);
				ClassPtr._IsAddress = true;

				TepFuninfo.Pars.insert(TepFuninfo.Pars.begin(),ClassPtr);

				String funcFullName= SymClass.FullName;
				ScopeHelper::GetApendedString(funcFullName, Overload_Invoke_Func);

				TepFuninfo.FullName = funcFullName;

				auto& SymClass = AddSybol(SymbolType::Func,Overload_Invoke_Func, funcFullName);
				SymClass.Info =Unique_ptr<FuncInfo>(new FuncInfo(std::move(TepFuninfo)));
				SymClass.PassState = passtype;

				Info->_ClassCall = (FuncInfo*)SymClass.Info.get();
			}

			//
			LastExpressionType = TypeSymbol(ClassSymID);
		}
		else
		{
			String funcPtrName = GetLambdaFuncTepName(LambdaName);
			String funcFullName = _Table._Scope.ThisScope;
			ScopeHelper::GetApendedString(funcFullName, funcPtrName);

			FuncInfo TepFuninfo;
			TepFuninfo.Pars = Info->Pars;
			TepFuninfo.Ret = Info->Ret;
			TepFuninfo.FullName = funcFullName;
			auto TepLambdaFunc = GetTepFuncPtrSyb(funcPtrName, &TepFuninfo);
			TepLambdaFunc->PassState = passtype;


			LastExpressionType = TypeSymbol(TepLambdaFunc->ID);
		}
	}
	else if (passtype == PassType::BuidCode)
	{
		_Table.AddScope(LambdaName);

		auto& LambdaSym = _Table.GetSymbol((SymbolID)&node);
		LambdaInfo* Info = LambdaSym.Get_Info<LambdaInfo>();

		if (Info->UsesOuterScope())
		{
			const SymbolID ClassSymID = (SymbolID)&node._Capture;
			auto& SymClass = _Table.GetSymbol(ClassSymID);

			ClassInfo* ClassSymInfo = SymClass.Get_Info<ClassInfo>();

			auto ClassType = TypeSymbol(ClassSymID);
			auto ClassTypeIR = ConvertToIR(ClassType);

			auto ClassTypeIRPtr = ClassType;
			ClassTypeIRPtr._IsAddress = true;

			auto TepIRObject = LookingAtIRBlock->NewLoad(ClassTypeIR);


			

			IRStruct* ClassStruct = _Builder.GetSymbol(ClassTypeIR._symbol)->Get_ExAs<IRStruct>();

			for (size_t i = 0; i < Info->_CapturedVarables.size(); i++)
			{
				auto& Item = Info->_CapturedVarables[i];


				IRInstruction* Value = nullptr;
				if (Item.Sym->Type == SymbolType::StackVarable)
				{
					Value = LookingAtIRBlock->NewLoad(Item.Sym->IR_Ins);
				}
				else if (Item.Sym->Type == SymbolType::ParameterVarable)
				{
					Value = LookingAtIRBlock->NewLoad(Item.Sym->IR_Par);
				}
				else
				{
					throw std::exception("not added");
				}


				auto Member = LookingAtIRBlock->New_Member_Access(TepIRObject, ClassStruct, i);
				LookingAtIRBlock->NewStore(Member, Value);

				{
					auto FullName = _Table._Scope.ThisScope;
					ScopeHelper::GetApendedString(FullName, Item.Name);
					auto& Sym = AddSybol(SymbolType::Class_Field, Item.Name, FullName);//to make a this indirection.
					Sym.VarType = Item.Type;
				}
			}
			
			if (Info->_ClassDrop)
			{//build drop func
				auto oldFunc = LookingAtIRFunc;
				auto oldblock = LookingAtIRBlock;
				//



				LookingAtIRFunc = _Builder.NewFunc(MangleName(Info->_ClassDrop), ConvertToIR(Info->Ret));
				LookingAtIRBlock = LookingAtIRFunc->NewBlock(".");

				LookingAtIRFunc->Pars.resize(1);
				{
					IRPar V;
					V.identifier = _Builder.ToID(ThisSymbolName);
					V.type = ConvertToIR(ClassTypeIRPtr);
					LookingAtIRFunc->Pars[0] = V;
				}

				BuildFuncDropUsesingFields(ClassSymInfo, ClassTypeIR);

				LookingAtIRBlock->NewRet();
				//
				LookingAtIRFunc = oldFunc;
				LookingAtIRBlock = oldblock;
			}
			
			{//build func
				auto oldFunc = LookingAtIRFunc;
				auto oldblock = LookingAtIRBlock;
				//


				
				LookingAtIRFunc = _Builder.NewFunc(MangleName(Info->_ClassCall), ConvertToIR(Info->Ret));
				LookingAtIRBlock = LookingAtIRFunc->NewBlock(".");


				LookingAtIRFunc->Pars.resize(node.Pars.Parameters.size()+1);
				{
					IRPar V;
					V.identifier = _Builder.ToID(ThisSymbolName);
					V.type = ConvertToIR(ClassTypeIRPtr);
					LookingAtIRFunc->Pars[0] = V;
				}
				for (size_t i = 0; i < node.Pars.Parameters.size(); i++)
				{
					auto& Item = node.Pars.Parameters[i];
					Symbol& Sym = _Table.GetSymbol((SymbolID)&Item);

					IRPar V;
					V.type = ConvertToIR(Info->Pars[i]);
					V.identifier = _Builder.ToID((IRidentifier)Item.Name.Token->Value._String);
					LookingAtIRFunc->Pars[i+1] = V;

					Sym.IR_Par = &LookingAtIRFunc->Pars[i+1];
				}

				ClassStackInfo _Data; 
				_Data.Info = ClassSymInfo;
				_Data._InStatements = true;
				_ClassStack.push(_Data);

				FuncStackInfo _FuncData(Info->_ClassCall);
				_FuncStack.push_back(_FuncData);

				if (node._Statements.has_value())
				{
					for (const auto& node2 : node._Statements.value()._Nodes)
					{
						OnStatement(*node2);
					}
				}

				_ClassStack.pop();
				_FuncStack.pop_back();

				LookingAtIRBlock->NewRet();
				//
				LookingAtIRFunc = oldFunc;
				LookingAtIRBlock = oldblock;
			}


			_LastExpressionField = TepIRObject;
			LastExpressionType = ClassType;
		}
		else
		{
			auto oldFunc = LookingAtIRFunc;
			auto oldblock = LookingAtIRBlock;
			//
			
			auto FuncName = _Table._Scope.ThisScope;

			LookingAtIRFunc = _Builder.NewFunc(FuncName, ConvertToIR(Info->Ret));
			LookingAtIRBlock = LookingAtIRFunc->NewBlock(".");


			LookingAtIRFunc->Pars.resize(node.Pars.Parameters.size());
			for (size_t i = 0; i < node.Pars.Parameters.size(); i++)
			{
				auto& Item = node.Pars.Parameters[i];
				Symbol& Sym = _Table.GetSymbol((SymbolID)&Item);

				IRPar V;
				V.type = ConvertToIR(Info->Pars[i]);
				V.identifier = _Builder.ToID((IRidentifier)Item.Name.Token->Value._String);
				LookingAtIRFunc->Pars[i] =V;

				Sym.IR_Par = &LookingAtIRFunc->Pars[i];
			}


			if (node._Statements.has_value())
			{
				for (const auto& node2 : node._Statements.value()._Nodes)
				{
					OnStatement(*node2);
				}
			}

			LookingAtIRBlock->NewRet();
			//
			LookingAtIRFunc = oldFunc;
			LookingAtIRBlock = oldblock;

			_LastExpressionField = LookingAtIRBlock->NewLoadFuncPtr(_Builder.ToID(FuncName));


			FuncInfo TepFuninfo;
			TepFuninfo.Pars = Info->Pars;
			TepFuninfo.Ret = Info->Ret;

			String funcPtrName = GetLambdaFuncTepName(LambdaName);
			String funcFullName =ScopeHelper::GetReMoveScope(_Table._Scope.ThisScope);
			ScopeHelper::GetApendedString(funcFullName, funcPtrName);
			LastExpressionType = TypeSymbol(GetTepFuncPtrSyb(funcFullName, &TepFuninfo)->ID);
		}


		_Table.RemoveScope();
	}
}
void SystematicAnalysis::OnTrait(const TraitNode& node)
{
	SymbolID sybId = GetSymbolID(node);
	const auto& ClassName = node._Name.Token->Value._String;
	_Table.AddScope(ClassName);

	auto& Syb = passtype == PassType::GetTypes ?
		AddSybol(SymbolType::Trait_class
			, (String)ClassName, _Table._Scope.ThisScope) :
		*GetSymbol(sybId);

	if (passtype == PassType::GetTypes)
	{
		_Table.AddSymbolID(Syb, sybId);


		TraitInfo* info = new TraitInfo();
		Syb.Info.reset(info);


		auto& SybClass = AddSybol(SymbolType::Type_class
			, (String)ClassName + "%Class", _Table._Scope.ThisScope + "%Class");
		_Table.AddSymbolID(SybClass, (SymbolID)&node._Name);

		auto TepClass = new ClassInfo();
		TepClass->FullName = SybClass.FullName;

		info->TraitClassInfo = &SybClass;
		SybClass.Info.reset(TepClass);


		SybClass.VarType.SetType(SybClass.ID);


		_ClassStack.push({ TepClass });


		bool HasDrop = false;

		for (const auto& node : node._Nodes)
		{
			switch (node->Get_Type())
			{
			case NodeType::FuncNode:
			{
				size_t Index = _Table.Symbols.size();

				FuncNode& funcnode = *FuncNode::As(node.get());

				OnFuncNode(funcnode);

				Symbol* funcSyb = _Table.Symbols[Index].get();

				TraitFunc _Func;
				_Func.Syb = funcSyb;
				_Func.HasBody = funcnode.Body.has_value();

				info->_Funcs.push_back(_Func);

				if (funcSyb->Get_Info<FuncInfo>()->_FuncType == FuncInfo::FuncType::Drop)
				{
					HasDrop = true;
				}
			}break;
			default:break;
			}
		}


		if (!HasDrop)
		{
			String FuncDropFunc = _Table._Scope.ThisScope;
			ScopeHelper::GetApendedString(FuncDropFunc, ClassDestructorFunc);

			TraitFunc _Func;
			_Func.Syb =&AddSybol(SymbolType::Func,ClassDestructorFunc, FuncDropFunc);
			
			FuncInfo* Funcinfo = new FuncInfo();
			_Func.Syb->Info.reset(Funcinfo);

			Funcinfo->_FuncType = FuncInfo::FuncType::Drop;
			Funcinfo->FullName = FuncDropFunc;


			TypeSymbol FuncPtr = SybClass.ID;
			FuncPtr._IsAddress = true;
			Funcinfo->Pars.push_back(FuncPtr);
			Funcinfo->Ret = TypesEnum::Void;

			_Func.HasBody = false;
			info->_Funcs.push_back(_Func);
		}

		

		_ClassStack.pop();
	}
	else if (passtype == PassType::FixedTypes)
	{
		TraitInfo* info = Syb.Get_Info<TraitInfo>();

		auto TepClass = info->TraitClassInfo->Get_Info<ClassInfo>();


		_ClassStack.push({ TepClass });

		for (const auto& node : node._Nodes)
		{
			switch (node->Get_Type())
			{
			case NodeType::FuncNode:
			{
				FuncNode& funcnode = *FuncNode::As(node.get());

				OnFuncNode(funcnode);

			}
			break;
			default:break;
			}
		}
	
		_ClassStack.pop();
	}
	else if (passtype == PassType::BuidCode)
	{
		//
		TraitInfo* info = Syb.Get_Info<TraitInfo>();
		auto StructVtablueClass = _Builder.NewStruct(_Builder.ToID(GetTraitVStructTableName(Syb.FullName)));
	
		for (auto& Item : info->_Funcs)
		{
			FuncInfo* ItemInfo = Item.Syb->Get_Info<FuncInfo>();
			auto StrFunc = GetTepFuncPtrName(ItemInfo);
			auto PtrFunc = GetTepFuncPtrSyb(StrFunc, ItemInfo);
			PtrFunc->FullName = StrFunc;
			TypeSymbol PtrType = PtrFunc->ID;

			auto IRType = ConvertToIR(PtrType);

			IRStructField V;
			V.Type = IRType;
			StructVtablueClass->Fields.push_back(V);
		}
	
	}


	_Table.RemoveScope();
}
void SystematicAnalysis::OnTag(const TagTypeNode& node)
{
	SymbolID sybId = GetSymbolID(node);
	const auto& ClassName = node.AttributeName.Token->Value._String;
	_Table.AddScope(ClassName);

	auto& Syb = passtype == PassType::GetTypes ?
		AddSybol(SymbolType::Tag_class
			, (String)ClassName, _Table._Scope.ThisScope) :
		*GetSymbol(sybId);

	if (passtype == PassType::GetTypes)
	{
		_Table.AddSymbolID(Syb, sybId);
	}
	else if (passtype == PassType::FixedTypes)
	{

	}
	else if (passtype == PassType::BuidCode)
	{

	}

	_Table.RemoveScope();
}
void SystematicAnalysis::OnBitCast(const BitCastExpression& node)
{
	
	if (passtype == PassType::FixedTypes)
	{
		TypeSymbol ToType = ConvertAndValidateType(node._Type,NodeSyb_t::Any);
		OnExpressionTypeNode(node._Expression.Value.get(), GetValueMode::Read);
		auto ExType = LastExpressionType;

		LastExpressionType = ToType;
	}
	else if (passtype == PassType::BuidCode)
	{
		TypeSymbol ToType = ConvertAndValidateType(node._Type, NodeSyb_t::Any);

		OnExpressionTypeNode(node._Expression.Value.get(), GetValueMode::Read);
		auto ExType = LastExpressionType;

		LastExpressionType = ToType;
	}
}
String SystematicAnalysis::GetClassWithTraitVTableName(const String& ClassFullName, const String& TraitFullName)
{
	return TraitFullName + ":$" + ClassFullName;
}
String SystematicAnalysis::GetTraitVTableName(const String& TraitName)
{
	return TraitName + "vtable";
}
String SystematicAnalysis::GetTraitVStructTableName(const String& TraitName)
{
	return TraitName + "vtable_type";
}
void SystematicAnalysis::InheritTrait(Symbol* Syb,ClassInfo* ClassInfo, Symbol* Trait, const Token* ClassNameToken)
{
	TraitInfo* Traitinfo = Trait->Get_Info<TraitInfo>();

	auto ID = ClassInfo->Get_InheritedTypesIndex(Trait);
	ClassInfo_InheritTypeInfo& IDSyb = ClassInfo->_InheritedTypes[ID.value()];

	bool HasErr = false;

	for (auto& Item : Traitinfo->_Funcs)
	{
		if (Item.HasBody)
		{
			ClassInfo_InheritTypeInfo::AddedFuncInfo InfoB;
			InfoB.FuncNode = Item.Syb->NodePtr;
			IDSyb.AddedFuncs.push_back(InfoB);


			ClassInfo_InheritTypeInfo::FuncIndex V;
			V.Index = IDSyb.AddedFuncs.size() - 1;
			V.Type = ClassInfo_InheritTypeInfo::FuncType::Added;
			IDSyb.Funcs.push_back(V);
		}
		else
		{
			FuncInfo* Info = Item.Syb->Get_Info<FuncInfo>();

			auto FuncName = Info->Get_Name();
			auto& List = _Table.GetSymbolsWithName(Info->Get_Name());

			bool HasFunc = false;

			for (auto& ItemL : List)
			{
				if (HasFunc)
				{
					break;
				}
				if (ItemL->Type == SymbolType::Func)
				{
					FuncInfo* ItemFunc = ItemL->Get_Info<FuncInfo>();

					
					if (ItemFunc->Pars.size() == Info->Pars.size())
					{
						for (size_t i = 0; i < Info->Pars.size(); i++)
						{
							const auto& TraitPar = Info->Pars[i];
							const auto& FuncPar = ItemFunc->Pars[i];

							auto SubTraitParType = TraitPar;

							if (TraitPar._CustomTypeSymbol == Traitinfo->TraitClassInfo->ID)
							{
								SubTraitParType._CustomTypeSymbol = Syb->ID;
							}

							if (!AreTheSame(SubTraitParType, FuncPar))
							{
								goto ConstinueSybList;
							}


							HasFunc = true;
							//
							ClassInfo_InheritTypeInfo::AddedFuncInfo InfoB;
							InfoB.FuncNode = ItemL->NodePtr;
							InfoB.Func = ItemL;
							IDSyb.OverLoadedFuncs.push_back(InfoB);


							ClassInfo_InheritTypeInfo::FuncIndex V;
							V.Index = IDSyb.OverLoadedFuncs.size() - 1;
							V.Type = ClassInfo_InheritTypeInfo::FuncType::OverLoaded;
							IDSyb.Funcs.push_back(V);
							break;
						}

					}


				ConstinueSybList:continue;//check if same thing
				}
			}


			if (!HasFunc) {
				LogMissingFuncionforTrait(FuncName, Info, Trait, ClassNameToken);
				HasErr = true;
			}
		}
	}


	if (!HasErr)
	{
	
		//
		{
			ClassStackInfo _ClStack;
			_ClStack._InStatements = false;
			_ClStack.Info = ClassInfo;
			_ClassStack.push(_ClStack);
		}
		
		auto oldpass = passtype;
		passtype = PassType::GetTypes;


		for (auto& Item : IDSyb.AddedFuncs)
		{
			const FuncNode& func = *(FuncNode*)Item.FuncNode;

			auto SybsIndex = _Table.Symbols.size();

			OnFuncNode(func);
		

			Item.Func = _Table.Symbols[SybsIndex].get();

		}


		passtype = PassType::FixedTypes;

		for (auto& Item : IDSyb.AddedFuncs)
		{
			const FuncNode& func = *(FuncNode*)Item.FuncNode;
			OnFuncNode(func);
		}

		{
			passtype = oldpass;
			_ClassStack.pop();
		}
	}
}
void SystematicAnalysis::BuildTrait(Symbol* Syb, ClassInfo* ClassInfo, Symbol* Trait, const Token* ClassNameToken)
{

	auto ID = ClassInfo->Get_InheritedTypesIndex(Trait);
	ClassInfo_InheritTypeInfo& IDSyb = ClassInfo->_InheritedTypes[ID.value()];

	{
		ClassStackInfo _ClStack;
		_ClStack._InStatements = false;
		_ClStack.Info = ClassInfo;
		_ClassStack.push(_ClStack);
	}


	for (auto& Item : IDSyb.AddedFuncs)
	{
		const FuncNode& func = *(FuncNode*)Item.FuncNode;
		OnFuncNode(func);
	}

	{
		String VTableName = GetClassWithTraitVTableName(Syb->FullName, Trait->FullName);
		auto StaticVarableToID = _Builder.ToID(VTableName);

		auto StaticVarableType = IRType(_Builder.GetSymbol(_Builder.ToID(GetTraitVStructTableName(Trait->FullName)))->identifier);

		auto StaticVarable = _Builder.NewStaticVarable(StaticVarableToID, StaticVarableType);


		auto oldIRFunc = LookingAtIRFunc;
		auto oldblock = LookingAtIRBlock;



		if (_Builder._StaticInit.Blocks.size() == 0)
		{
			_Builder._StaticInit.NewBlock(".");
		}

		LookingAtIRFunc = &_Builder._StaticInit;
		LookingAtIRBlock = LookingAtIRFunc->Blocks.front().get();

		//

		auto Ptr = LookingAtIRBlock->NewLoadPtr(StaticVarableToID);
		for (size_t i = 0; i < IDSyb.Funcs.size(); i++)
		{
			auto& Item = IDSyb.Funcs[i];


			Symbol* Func = Item.Type == ClassInfo_InheritTypeInfo::FuncType::Added ? IDSyb.AddedFuncs[Item.Index].Func : IDSyb.OverLoadedFuncs[Item.Index].Func;
			
			auto Member = LookingAtIRBlock->New_Member_Dereference(Ptr, StaticVarableType, i);
			LookingAtIRBlock->NewStore(Member, LookingAtIRBlock->NewLoadFuncPtr(_Builder.ToID(Func->FullName)));
		}
		//
		LookingAtIRFunc = oldIRFunc;
		LookingAtIRBlock = oldblock;
	}


	{
		_ClassStack.pop();
	}
}
void SystematicAnalysis::LogMissingFuncionforTrait(UCodeLang::String_view& FuncName, UCodeLang::FrontEnd::FuncInfo* Info, UCodeLang::FrontEnd::Symbol* Trait, const UCodeLang::Token* ClassNameToken)
{
	String Msg = "Missing Funcion '" + (String)FuncName + "' with the parameters [";

	for (auto& ItemP : Info->Pars)
	{
		Msg += ToString(ItemP);

		if (&ItemP != &Info->Pars.back())
		{
			Msg += ",";
		}
	}

	Msg += "] and returns '" + ToString(Info->Ret) + "' for the trait '" + Trait->FullName + '\'';

	_ErrorsOutput->AddError(ErrorCodes::ExpectingSequence, ClassNameToken->OnLine, ClassNameToken->OnPos, Msg);
}
Symbol* SystematicAnalysis::NewDropFuncSymbol(ClassInfo* ClassInfo, TypeSymbol& ClassAsType)
{

	String FullName = ClassInfo->FullName;
	ScopeHelper::GetApendedString(FullName, (String)ClassDestructorFunc);

	auto DropFunc = &AddSybol(SymbolType::Func, ClassDestructorFunc, FullName);
	FuncInfo* V = new FuncInfo();
	DropFunc->Info.reset(V);

	V->FullName = DropFunc->FullName;
	V->_FuncType = FuncInfo::FuncType::Drop;
	V->Ret = TypesEnum::Void;

	V->FrontParIsUnNamed = true;

	auto ThisParType = ClassAsType;
	ThisParType._IsAddress = true;
	V->Pars.push_back(ThisParType);
	ClassInfo->_ClassAutoGenerateDrop = true;

	DropFunc->PassState = PassType::FixedTypes;
	return DropFunc;
}

void SystematicAnalysis::BuildFuncDropUsesingFields(const ClassInfo* ClassInfo, const IRType& ThisPar)
{
	for (int i = ClassInfo->Fields.size() - 1; i >= 0; i--)
	{
		auto& Item = ClassInfo->Fields[i];
		if (HasDestructor(Item.Type))
		{
			ObjectToDrop obj;
			obj.DropType = ObjectToDropType::IRInstruction;
			obj.Type = Item.Type;
			obj._Object = LookingAtIRBlock->New_Member_Dereference(&LookingAtIRFunc->Pars.front(), ThisPar, i);

			DoDestructorCall(obj);
		}
	}
}
void SystematicAnalysis::OnStatementsWithSetableRet(const StatementsNode& node, TypeSymbol& RetOut, const Token* Token)
{
	LookingForTypes.push(RetOut);

	bool  HasARet = false;

	SetInStatetements(true);
	for (const auto& node2 : node._Nodes)
	{
		size_t ErrCount = _ErrorsOutput->Get_Errors().size();
		OnStatement(*node2);

		if (node2->Get_Type() == NodeType::RetStatementNode)
		{
			HasARet = true;

			if (RetOut._Type == TypesEnum::Var)
			{
				auto OldType = RetOut;

				auto NewType = LastExpressionType;

				if (OldType.IsAddress()) {
					NewType.SetAsAddress();
				}
				if (OldType.IsAddressArray()) {
					NewType.SetAsAddressArray();
				}



				Get_LookingForType() = RetOut = NewType;

				if (LastExpressionType._Type == TypesEnum::Var)
				{
					auto Token = LastLookedAtToken;
					CantguessVarTypeError(Token);
				}

				if (ErrCount == _ErrorsOutput->Get_Errors().size()) {
					OnStatement(*node2);//re do
				}
			}

			break;
		}
	}
	SetInStatetements(false);

	LookingForTypes.pop();


	if (!HasARet)
	{
		if (RetOut._Type == TypesEnum::Var)
		{
			RetOut.SetType(TypesEnum::Void);
		}
		else if (RetOut._Type != TypesEnum::Void
			&& !RetOut.IsBadType())//Update This when control flow get added.
		{
			YouMustReturnSomethingError(Token);
		}

	}
}
void SystematicAnalysis::TypeDoesNotHaveForOverload(const UCodeLang::Token* Token, UCodeLang::FrontEnd::TypeSymbol& ExType)
{
	if (ExType.IsBadType()) { return; }
	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "The Type '" + ToString(ExType) + "\' does not have the for overload.");
}
bool SystematicAnalysis::ISStructPassByRef(Symbol* syb)
{
	auto r = !(IsPrimitive(syb->VarType) || syb->VarType.IsAddress());
	if (r == false && syb->VarType._Type == TypesEnum::CustomType)
	{
		auto V = GetSymbol(syb->VarType);
		r = V->Type == SymbolType::Type_StaticArray;
	}
	return r;
}

//Funcs

void SystematicAnalysis::WriteTo(IRInstruction* IR, const IROperator& Value)
{
	LookingAtIRBlock->NewStore((IRInstruction*)nullptr, IR);
	auto& Item = LookingAtIRBlock->Instructions.back();
	Item->Target() = Value;
}
void SystematicAnalysis::WriteToDef(IRInstruction* IR, const IROperator& Value)
{
	LookingAtIRBlock->NewDereferenc_Store((IRInstruction*)nullptr, IR);
	auto& Item = LookingAtIRBlock->Instructions.back();
	Item->Target() = Value;
}
void SystematicAnalysis::WriteTo(IRInstruction* IR, const TypeSymbol& Type, const IROperator& Value)
{
	if (Type.IsAddress())
	{
		WriteToDef(IR, Value);
	}
	else
	{
		WriteTo(IR, Value);
	}
}

size_t SystematicAnalysis::GetJumpsIndex() { return _Jumps.size() ? _Jumps.size() - 1 : 0; }
void SystematicAnalysis::RemoveJumps(size_t Index)
{
	size_t toremove = _Jumps.size() - Index;
	for (size_t i = 0; i < toremove; i++)
	{
		_Jumps.pop_back();
	}
}
IRidentifierID SystematicAnalysis::ConveToIRClassIR(const Symbol& Class)
{
	auto ClassSybID = Class.ID;
	if (SybToIRMap.HasValue(ClassSybID))
	{
		return SybToIRMap.at(ClassSybID);
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
	return V;
}

IRidentifierID SystematicAnalysis::ConveToStaticArray(const Symbol& Class)
{
	auto ClassSybID = Class.ID;
	if (SybToIRMap.HasValue(ClassSybID))
	{
		return SybToIRMap.at(ClassSybID);
	}
	const StaticArrayInfo* clasinfo = Class.Get_Info <StaticArrayInfo>();

	IRidentifierID V = _Builder.ToID(Class.FullName);

	auto IRStuct = _Builder.NewStaticArray(V,ConvertToIR(clasinfo->Type),clasinfo->Count);


	SybToIRMap[ClassSybID] = V;
	return V;
}

IRidentifierID SystematicAnalysis::ConveToIRVariantEnum(const Symbol& Enum)
{
	const EnumInfo* Info = Enum.Get_Info<EnumInfo>();
	
	if (Info->VariantData.has_value()) 
	{
		auto ClassSybID = Enum.ID;
		if (SybToIRMap.HasValue(ClassSybID))
		{
			return SybToIRMap.at(ClassSybID);
		}


		IRidentifierID V = _Builder.ToID(Enum.FullName);
		auto IRStuct = _Builder.NewStruct(V);


		String UnionName = GetEnumVariantUnionName(Enum.FullName);

		IRidentifierID UnionID = _Builder.ToID(UnionName);
		auto IRUnion= _Builder.NewStruct(UnionID);
		IRUnion->IsUnion = true;

		auto& Item = Info->VariantData.value();
		//

		for (auto& Item2 : Item.Variants)
		{
			if (Item2.ClassSymbol.has_value())
			{
				SymbolID ClassSymbol = Item2.ClassSymbol.value();
				Symbol* ClassSym = GetSymbol(ClassSymbol);

				auto irstuctID = _Builder.ToID(ClassSym->FullName);
				IRStructField Vtep;
				Vtep.Type = IRType(irstuctID);
				IRUnion->Fields.push_back(Vtep);
			}
			else 
			{
				if (Item2.Types.size())
				{

					IRStructField Vtep;
					Vtep.Type = ConvertToIR(Item2.Types.front());
					IRUnion->Fields.push_back(Vtep);
				}
			}
		}
		//
		
		{//key
			IRStructField field;
			field.Type = ConvertToIR(Info->Basetype);
			IRStuct->Fields.push_back(field);
		}
		{//Union
			IRStructField field;
			field.Type = IRType(UnionID);
			IRStuct->Fields.push_back(field);
		}

		//
		SybToIRMap[ClassSybID] = UnionID;
		return V;
	}
	return 0;
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
			if (V->VariantData.has_value())
			{
				return IRType(_Builder.ToID(V->FullName));
			}
			else 
			{
				return ConvertToIR(V->Basetype);
			}
		}
		else if (syb.Type == SymbolType::Type_alias
			|| syb.Type == SymbolType::Hard_Type_alias)
		{
			return ConvertToIR(syb.VarType);
		}
		else if (syb.Type == SymbolType::Func_ptr || syb.Type == SymbolType::Hard_Func_ptr)
		{
			if (SybToIRMap.HasValue(syb.ID))
			{
				return IRType(SybToIRMap.at(syb.ID));
			}
			else
			{
				FuncPtrInfo* V = syb.Get_Info<FuncPtrInfo>();
				IRidentifierID IRid = _Builder.ToID(syb.FullName);
				IRType r = IRid;
				auto tep = _Builder.NewFuncPtr(_Builder.ToID(syb.FullName), ConvertToIR(V->Ret));

				tep->Pars.resize(V->Pars.size());
				
				for (size_t i = 0; i < tep->Pars.size(); i++)
				{
					auto& Item = tep->Pars[i];
					Item = ConvertToIR(V->Pars[i]);
				}
				tep->Ret = ConvertToIR(V->Ret);

				SybToIRMap[syb.ID] = IRid;
				return r;
			}
		}
		else if (syb.Type == SymbolType::Type_class)
		{
			return IRType(ConveToIRClassIR(syb));
		}
		else if (syb.Type == SymbolType::Type_StaticArray)
		{
			return IRType(ConveToStaticArray(syb));
		}
		else if (syb.Type == SymbolType::Trait_class && Value._IsDynamic)
		{
			if (SybToIRMap.HasValue(syb.ID))
			{
				return IRType(SybToIRMap.at(syb.ID));
			}
			else
			{
				String DynSybName = "Dynamic" + syb.FullName;

				IRidentifierID IRid = _Builder.ToID(DynSybName);
				auto StructIR = _Builder.NewStruct(IRid);

				{
					IRStructField V;
					V.Type = IRType(IRTypes::pointer);
					StructIR->Fields.push_back(V);
				}
				{
					IRStructField V;
					V.Type = IRType(IRTypes::pointer);
					StructIR->Fields.push_back(V);
				}

				IRType r = IRid;
				SybToIRMap[syb.ID] = IRid;
				return r;
			}
		}
		else
		{
			throw std::exception("not added");
		}
	}
	break;
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
void SystematicAnalysis::OnStatement(const Node& node2)
{
	PushToNodeScope(node2);
	switch (node2.Get_Type())
	{
	case NodeType::AttributeNode:OnAttributeNode(*AttributeNode::As(&node2)); break;
	case NodeType::ClassNode: OnClassNode(*ClassNode::As(&node2)); break;
	case NodeType::EnumNode:OnEnum(*EnumNode::As(&node2)); break;
	case NodeType::UsingNode: OnUseingNode(*UsingNode::As(&node2)); break;
	case NodeType::DeclareVariableNode:OnDeclareVariablenode(*DeclareVariableNode::As(&node2),DeclareStaticVariableNode_t::Stack); break;
	case NodeType::AssignExpressionNode:OnAssignExpressionNode(*AssignExpressionNode::As(&node2)); break;
	case NodeType::PostfixVariableNode:OnPostfixVariableNode(*PostfixVariableNode::As(&node2)); break;
	case NodeType::CompoundStatementNode:OnCompoundStatementNode(*CompoundStatementNode::As(&node2)); break;
	case NodeType::FuncCallStatementNode:
	{
		TypeSymbol V(TypesEnum::Any);
		LookingForTypes.push(V);

		OnFuncCallNode(FuncCallStatementNode::As(&node2)->Base);

		LookingForTypes.pop();
	}
	break;
	case NodeType::DropStatementNode:OnDropStatementNode(*DropStatementNode::As(&node2)); break;
	case NodeType::IfNode:OnIfNode(*IfNode::As(&node2)); break;
	case NodeType::WhileNode:OnWhileNode(*WhileNode::As(&node2)); break;
	case NodeType::DoNode:OnDoNode(*DoNode::As(&node2)); break;
	case NodeType::DeclareStaticVariableNode:OnDeclareStaticVariableNode(*DeclareStaticVariableNode::As(&node2)); break;
	case NodeType::DeclareThreadVariableNode:OnDeclareThreadVariableNode(*DeclareThreadVariableNode::As(&node2)); break;
	case NodeType::ForNode:OnForNode(*ForNode::As(&node2)); break;
	case NodeType::ContinueNode:OnContinueNode(*ContinueNode::As(&node2)); break;
	case NodeType::BreakNode:OnBreakNode(*BreakNode::As(&node2)); break;
	case NodeType::RetStatementNode:OnRetStatement(*RetStatementNode::As(&node2)); break;
	case NodeType::TraitNode:OnTrait(*TraitNode::As(&node2)); break;
	case NodeType::TagTypeNode:OnTag(*TagTypeNode::As(&node2)); break;
	default:break;
	}
	PopNodeScope();
}
void SystematicAnalysis::OnRetStatement(const RetStatementNode& node)
{
	
	auto& LookForT = Get_LookingForType();
	if (node.Expression.Value)
	{
	_FuncStack.back().IsOnRetStatemnt = true;
		//LookForT.SetAsRawValue();

		LookingForTypes.push(LookForT);
		OnExpressionTypeNode(node.Expression.Value.get(), GetValueMode::Read);

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
		ConvertAndValidateType(node.BaseType, ClassInf->Basetype, NodeSyb_t::Any);
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
				OnExpressionTypeNode(Item.Expression.Value.get(),GetValueMode::Read);
			}


			_Table._Scope.AddScope(ItemName);
			{
				auto& EnumFeild = AddSybol(SymbolType::Enum_Field, (String)ItemName, _Table._Scope.ThisScope);
				EnumFeild.NodePtr = &Item;
			}
			_Table._Scope.ReMoveScope();


			if (Item.VariantType)
			{
				if (!ClassInf->VariantData.has_value())
				{
					ClassInf->VariantData = EnumVariantData();
				}
			}
		}
		else if (passtype == PassType::FixedTypes)
		{
			if (ClassInf->VariantData)
			{
				
				EnumVariantData& EnumVa = ClassInf->VariantData.value();
				
				
				if (Item.VariantType)
				{
					auto& VariantType_ = Item.VariantType.value();
					if (VariantType_.node && VariantType_.node->Get_Type() == NodeType::AnonymousTypeNode)
					{
						EnumVariantFeild V;

						{
							//
								_Table._Scope.AddScope(ItemName);
								auto NewName = GetFuncAnonymousObjectFullName(_Table._Scope.ThisScope);
								_Table._Scope.ReMoveScope();
							//

							SymbolID AnonymousSybID = (SymbolID)VariantType_.node.get();
							auto& AnonymousSyb = AddSybol(SymbolType::Type_class, (String)NewName, NewName);

							_Table.AddSymbolID(AnonymousSyb, AnonymousSybID);


							auto ClassInf = new ClassInfo();
							ClassInf->FullName = NewName;
							AnonymousSyb.Info.reset(ClassInf);
							AnonymousSyb.VarType.SetType(AnonymousSyb.ID);

							AnonymousTypeNode* Typenode = AnonymousTypeNode::As(VariantType_.node.get());
							for (auto& Item3 : Typenode->Fields.Parameters)
							{
								auto Fieldtype = ConvertAndValidateType(Item3.Type, NodeSyb_t::Parameter);
								V.Types.push_back(Fieldtype);
								ClassInf->AddField(Item3.Name.AsString(), Fieldtype);
							}
							V.ClassSymbol = AnonymousSybID;
						} 

						EnumVa.Variants.push_back(std::move(V));
					}
					else
					{
						EnumVariantFeild V; 
						V.Types.push_back(ConvertAndValidateType(VariantType_,NodeSyb_t::Parameter));
						
						EnumVa.Variants.push_back(std::move(V));
					}
				}
				else
				{
					EnumVariantFeild V;
					EnumVa.Variants.push_back(std::move(V));
				}

			}

			auto& Field = *ClassInf->GetField(ItemName).value();
			if (Item.Expression.Value)
			{
				auto& Type = ClassInf->Basetype;
				OnExpressionTypeNode(Item.Expression.Value.get(),GetValueMode::Read);//check
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

						auto HasInfo = HasPostfixOverLoadWith(Type, TokenType::increment);

						if (!HasInfo.HasValue)
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
		if (ClassInf->VariantData) 
		{
			ClassData& EnumUnion = _Lib.Get_Assembly().AddClass(GetEnumVariantUnionName(
				GetUnrefencedableName((String)ClassInf->Get_Name())), GetUnrefencedableName(GetEnumVariantUnionName(ClassInf->FullName)));
			

			auto& List = ClassInf->VariantData.value().Variants;
			for (size_t i = 0; i < List.size(); i++)
			{
				auto& Item = List[i];
			
				if (Item.ClassSymbol.has_value())
				{
					Symbol* Sym = GetSymbol(Item.ClassSymbol.value());

					AddClass_tToAssemblyInfo(Sym->Get_Info<ClassInfo>());//has '!' post fix so its Unrefencedable


					ClassField V;
					V.offset = 0;
					V.Name = ClassInf->Fields[i].Name;
					V.Type = ConvertToTypeInfo(TypeSymbol(Sym->ID));
					EnumUnion._Class.Fields.push_back(std::move(V));
				}
				else
				{
					if (Item.Types.size()) 
					{
						ClassField V;
						V.offset = 0;
						V.Name = ClassInf->Fields[i].Name;
						V.Type = ConvertToTypeInfo(Item.Types.front());
						EnumUnion._Class.Fields.push_back(std::move(V));
					}
				}


				for (auto& Item2 : Item.Types)
				{
					AddDependencyToCurrentFile(Item2);
				}
				
			}
			

			int a = 0;
		}
		else
		{
			//enum
		}
	}

	_Table.RemoveScope();
}

String SystematicAnalysis::GetScopedNameAsString(const ScopedNameNode& node)
{
	String Text;
	if (node.ScopedName.size() && node.ScopedName[0].token->Type == TokenType::KeyWord_This)
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
void SystematicAnalysis::OnDeclareVariablenode(const DeclareVariableNode& node, DeclareStaticVariableNode_t type)
{
	
	auto& StrVarName = node.Name.AsString();
	auto FullName = _Table._Scope.GetApendedString(StrVarName);

	SymbolID sybId = GetSymbolID(node);
	Symbol* syb;

	bool InSideClass = _InSideClass();

	if (passtype == PassType::GetTypes)
	{
		SymbolType SysType = SymbolType::StackVarable;
		//
		switch (type)
		{
		case DeclareStaticVariableNode_t::Stack:
			SysType = SymbolType::StackVarable;
			break;
		case DeclareStaticVariableNode_t::Static:
			SysType = SymbolType::StaticVarable;
			break;
		case DeclareStaticVariableNode_t::Thread:
			SysType = SymbolType::ThreadVarable;
			break;
		case DeclareStaticVariableNode_t::ClassField:
			break;
		default:
			SysType = SymbolType::StackVarable;
			break;
		}
		//
		DoSymbolRedefinitionCheck(FullName, SymbolType::StackVarable, node.Name.Token);


		syb = &AddSybol(SysType, StrVarName, FullName);
		_Table.AddSymbolID(*syb, sybId);


		if (InSideClass)
		{
			syb->Type = SymbolType::Class_Field;
			auto& Class = *_ClassStack.top().Info;

			Class.AddField(ScopeHelper::GetNameFromFullName((String_view)FullName), TypeSymbol());

		}
		syb->NodePtr = (void*)node.Name.Token->OnPos;//a tep solution. 


		//
		if (type == DeclareStaticVariableNode_t::Static
			|| type == DeclareStaticVariableNode_t::Thread)
		{
			if (!node.Expression.Value)
			{
				auto Token = node.Name.Token;
				String VarType = type == DeclareStaticVariableNode_t::Static ? "Static" : "Thread";
				_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos, VarType + " Varable must be assigned.missing '='.");
			}
		}

		//
		if (node.Expression.Value)
		{
			OnExpressionTypeNode(node.Expression.Value.get(), GetValueMode::Read);
		}
	}
	else
	{
		syb = GetSymbol(sybId);
	}

	if (passtype == PassType::FixedTypes)
	{
		auto& VarType = syb->VarType;
		ConvertAndValidateType(node.Type, VarType,NodeSyb_t::ClassFeild);
		VarType.SetAsLocation();
		
		
		auto Ex = node.Expression.Value.get();
		ExTypeDeclareVarableCheck(VarType, Ex, node.Name.Token);

		

		
	}
	LookingForTypes.push(syb->VarType);

	IRInstruction* OnVarable{};
	bool IsStructObjectPassRef =false;
	
	IRFunc* oldIRFunc{};
	IRBlock* oldblock{};
	
	if (passtype == PassType::BuidCode) 
	{

		if (node.Expression.Value)
		{

			if (syb->Type == SymbolType::StackVarable)
			{
				OnVarable = LookingAtIRBlock->NewLoad(ConvertToIR(syb->VarType));
				syb->IR_Ins = OnVarable;


			}
			else if (syb->Type == SymbolType::StaticVarable)
			{
				oldIRFunc = LookingAtIRFunc;
				oldblock = LookingAtIRBlock;

			

				if (_Builder._StaticInit.Blocks.size() == 0)
				{
					_Builder._StaticInit.NewBlock(".");
				}	
				
				LookingAtIRFunc = &_Builder._StaticInit;
				LookingAtIRBlock = LookingAtIRFunc->Blocks.front().get();


				if (ISStructPassByRef(syb)) {
					OnVarable = LookingAtIRBlock->NewLoad(ConvertToIR(syb->VarType));
				}
			}
			else if (syb->Type == SymbolType::ThreadVarable)
			{
				oldIRFunc = LookingAtIRFunc;
				oldblock = LookingAtIRBlock;

				if (_Builder._threadInit.Blocks.size() == 0)
				{
					_Builder._threadInit.NewBlock(".");
				}

				LookingAtIRFunc = &_Builder._threadInit;
				LookingAtIRBlock = LookingAtIRFunc->Blocks.front().get();


				if (ISStructPassByRef(syb)) {
					OnVarable = LookingAtIRBlock->NewLoad(ConvertToIR(syb->VarType));
				}
			}
			else if (syb->Type == SymbolType::Class_Field)
			{
				oldIRFunc = LookingAtIRFunc;
				oldblock = LookingAtIRBlock;

				auto* Classinfo = _ClassStack.top().Info;
				if (Classinfo->_ClassFieldInit == nullptr)
				{
					String funcName = _Table._Scope.GetApendedString((String)ClassInitializefuncName);


					Classinfo->_ClassFieldInit = _Builder.NewFunc(funcName, IRTypes::Void);
					Classinfo->_ClassFieldInit->NewBlock(".");
					{
						auto Classtype = TypeSymbol(GetSymbol(Classinfo)->ID);
						Classtype._IsAddress = true;

						IRPar ThisPar;
						ThisPar.identifier = _Builder.ToID(ThisSymbolName);
						ThisPar.type = ConvertToIR(Classtype);
						Classinfo->_ClassFieldInit->Pars.push_back(ThisPar);
					}
				}

				LookingAtIRFunc = Classinfo->_ClassFieldInit;
				LookingAtIRBlock = LookingAtIRFunc->Blocks.front().get();


				if (ISStructPassByRef(syb)) {
					OnVarable = LookingAtIRBlock->NewLoad(ConvertToIR(syb->VarType));
				}
			}
			else
			{
				throw std::exception("not added");
			}

			IsStructObjectPassRef = ISStructPassByRef(syb);

			if (IsStructObjectPassRef)
			{
				IRlocations.push({ OnVarable ,false });
			}

			OnExpressionTypeNode(node.Expression.Value.get(), GetValueMode::Read);
		}
		else
		{
			if (syb->Type == SymbolType::StackVarable)
			{
				OnVarable = LookingAtIRBlock->NewLoad(ConvertToIR(syb->VarType));
				syb->IR_Ins = OnVarable;
			}


		}
	}

	if (passtype == PassType::FixedTypes)
	{
		syb = GetSymbol(sybId);
		if (node.Expression.Value)
		{
			OnExpressionTypeNode(node.Expression.Value.get(), GetValueMode::Read);

			syb->SetTovalid();

			auto& VarType = syb->VarType;


			auto& Ex = LastExpressionType;
			auto Token = node.Type.Name.Token;
			ExDeclareVariableTypeCheck(VarType, Ex, Token);
		}
		else
		{
			if (!InSideClass) {
				syb->SetToInvalid();
			}
		}


		if (InSideClass)
		{
			auto& Class = *_ClassStack.top().Info;
			if (!Class.SizeInitialized)
			{
				if (Class.Size == NullAddress)
				{
					Class.Size = 0;
				}

				auto& Type = syb->VarType;
			}
			auto Field = Class.GetField(ScopeHelper::GetNameFromFullName(FullName));
			if (Field)
			{
				auto& Item = (*Field);
				Item->Type = syb->VarType;

				Class.Size += GetSize(Item->Type).value();
				
				if (node.Expression.Value) 
				{
					Class._WillHaveFieldInit= true;
				}
				if (HasDestructor(Item->Type))
				{
					Class._WillHaveFielddeInit = true;
				}
			}

			

		}
	}
	LookingForTypes.pop();

	if (passtype == PassType::BuidCode && node.Expression.Value)
	{
		
		DoImplicitConversion(_LastExpressionField, LastExpressionType, syb->VarType);

	
		OnStoreVarable(IsStructObjectPassRef, OnVarable, syb, sybId);
		
		AddDependencyToCurrentFile(syb->VarType);
	
	
		//
		if (syb->Type == SymbolType::StaticVarable)
		{
			LookingAtIRFunc = oldIRFunc;
			LookingAtIRBlock = oldblock;
		}
		else if (syb->Type == SymbolType::ThreadVarable)
		{
			LookingAtIRFunc = oldIRFunc;
			LookingAtIRBlock = oldblock;
		}
		else if (syb->Type == SymbolType::Class_Field)
		{
			LookingAtIRFunc = oldIRFunc;
			LookingAtIRBlock = oldblock;
		}
	
	}
}
void SystematicAnalysis::OnStoreVarable(bool IsStructObjectPassRef, UCodeLang::IRInstruction* OnVarable, UCodeLang::FrontEnd::Symbol* syb, const UCodeLang::SymbolID& sybId)
{

	if (syb->Type == SymbolType::StaticVarable || syb->Type == SymbolType::ThreadVarable)
	{
		auto id = _Builder.ToID(syb->FullName);


		if (IsStructObjectPassRef)
		{
			LookingAtIRBlock->NewStore(id, OnVarable);
		}
		else
		{
			LookingAtIRBlock->NewStore(id, _LastExpressionField);
		}


		if (HasDestructor(syb->VarType))
		{

			IRFunc* Func = syb->Type == SymbolType::StaticVarable ? &_Builder._StaticdeInit : &_Builder._threaddeInit;

			auto old = LookingAtIRBlock;

			if (Func->Blocks.size() == 0)
			{
				Func->NewBlock(".");
			}

			LookingAtIRBlock = Func->Blocks.front().get();


			auto Varable = LookingAtIRBlock->NewLoadPtr(id);

			ObjectToDrop V;
			V.ID = sybId;
			V._Object = Varable;
			V.DropType = ObjectToDropType::IRInstructionNoMod;
			V.Type = syb->VarType;
			DoDestructorCall(V);


			LookingAtIRBlock = old;
		}


	}
	else if (syb->Type == SymbolType::Class_Field)
	{
		auto Classinfo = this->_ClassStack.top().Info;
		auto classSb = GetSymbol(Classinfo);
		auto Classtype = TypeSymbol(classSb->ID);
		auto GG = classSb->Get_Info<ClassInfo>();
		auto IndexFeild = GG->GetFieldIndex(ScopeHelper::GetNameFromFullName(syb->FullName)).value();

		IRStruct* V = _Builder.GetSymbol(ConveToIRClassIR(*classSb))->Get_ExAs<IRStruct>();
		auto output = LookingAtIRBlock->New_Member_Dereference(&LookingAtIRFunc->Pars[0], ConvertToIR(classSb->ID), IndexFeild);

		if (IsStructObjectPassRef)
		{
			LookingAtIRBlock->NewStore(output, OnVarable);
		}
		else
		{
			LookingAtIRBlock->NewStore(output, _LastExpressionField);
		}

	}
	else
	{


		if (IsStructObjectPassRef)
		{
			if (IRlocations.top().UsedlocationIR == false)
			{

				if (_LastExpressionField->Type == IRInstructionType::Load
					&& _LastExpressionField->Target().Type == IROperatorType::IRInstruction)
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

		AddDestructorToStack(syb, sybId, OnVarable);
	}
}
String SystematicAnalysis::GetEnumVariantUnionName(const String& FullName)
{
	return FullName + "u";
}
void SystematicAnalysis::AddDestructorToStack(UCodeLang::FrontEnd::Symbol* syb, const UCodeLang::SymbolID& sybId, UCodeLang::IRInstruction* OnVarable)
{
	if (HasDestructor(syb->VarType))
	{
		ObjectToDrop V;
		V.ID = sybId;
		V._Object = OnVarable;
		V.Type = syb->VarType;

		StackFrames.back().OnEndStackFrame.push_back(V);
	}
}
String SystematicAnalysis::GetUnrefencedableName(const String& FullName)
{
	return "#" + FullName;
}
void SystematicAnalysis::AddDestructorToStack(const TypeSymbol& Type, IRInstruction* OnVarable)
{
	if (HasDestructor(Type))
	{
		ObjectToDrop V;
		V._Object = OnVarable;
		V.Type = Type;
		V.DropType = ObjectToDropType::IRInstruction;

		StackFrames.back().OnEndStackFrame.push_back(V);
	}
}
void SystematicAnalysis::ExDeclareVariableTypeCheck(UCodeLang::FrontEnd::TypeSymbol& VarType, UCodeLang::FrontEnd::TypeSymbol& Ex, const UCodeLang::Token* Token)
{
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
		}
	}
	if (!CanBeImplicitConverted(Ex, VarType, false))
	{
		LogCantCastImplicitTypes(Token, Ex, VarType, false);
	}
}
void SystematicAnalysis::ExTypeDeclareVarableCheck(TypeSymbol& VarType, const Node* Ex, const Token* Token)
{
	if (VarType._Type == TypesEnum::Var && Ex == nullptr)
	{
		CantgussTypesTheresnoassignment(Token);
	}
	else if (VarType._Type == TypesEnum::CustomType && Ex == nullptr)
	{
		auto Syb = GetSymbol(VarType);
		if (Syb->Type == SymbolType::Type_StaticArray)
		{
			StaticArrayInfo* V = Syb->Get_Info<StaticArrayInfo>();

			if (!V->IsCountInitialized)
			{
				LogBeMoreSpecifiicWithStaticArrSize(Token, V->Type);
			}
		}
	}
}
void SystematicAnalysis::OnAssignExpressionNode(const AssignExpressionNode& node)
{
	if (passtype == PassType::GetTypes)
	{
		OnExpressionTypeNode(node.Expression.Value.get(), GetValueMode::Read);
		OnExpressionTypeNode(node.ToAssign.Value.get(), GetValueMode::Write);
	}
	else if (passtype == PassType::FixedTypes)
	{
		LookingForTypes.push(TypesEnum::Var);
		OnExpressionTypeNode(node.Expression.Value.get(), GetValueMode::Read);
		LookingForTypes.pop();


		auto ExpressionType = LastExpressionType;

		LookingForTypes.push(TypesEnum::Var);
		OnExpressionTypeNode(node.ToAssign.Value.get(), GetValueMode::Write);
		LookingForTypes.pop();

		auto AssignType = LastExpressionType;

		
		if (!CanBeImplicitConverted(AssignType, ExpressionType,false))
		{
			auto  Token = LastLookedAtToken;
			LogCantCastImplicitTypes(Token,AssignType, ExpressionType, false);

		}
		auto ID = GetSymbolID(node);

		AssignExpression_Data Data;
		Data.Op0 = ExpressionType;
		Data.Op1 = AssignType;

		AssignExpressionDatas.AddValue((void*)ID, Data);
	}
	else if (passtype == PassType::BuidCode)
	{

		auto ID = GetSymbolID(node);
		auto& AssignType = AssignExpressionDatas.at((void*)ID);



		LookingForTypes.push(AssignType.Op0);
		OnExpressionTypeNode(node.Expression.Value.get(), GetValueMode::Read);
		LookingForTypes.pop();

		auto ExpressionType = LastExpressionType;
		auto ExIR = _LastExpressionField;
		
		DoImplicitConversion(ExIR, ExpressionType, AssignType.Op1);
		ExIR = _LastExpressionField;


		LookingForTypes.push(AssignType.Op1);
		OnExpressionTypeNode(node.ToAssign.Value.get(), GetValueMode::Write);
		LookingForTypes.pop();
		
		auto AssignIR = _LastExpressionField;

		WriteTo(ExIR, ExpressionType, _LastStoreField);
	}
}
void SystematicAnalysis::OnIfNode(const IfNode& node)
{
	
	TypeSymbol BoolType(TypesEnum::Bool);

	String ScopeName = std::to_string((size_t)&node);

	PushNewStackFrame();
	_Table.AddScope(ScopeName);
	
	LookingForTypes.push(BoolType);


	

	OnExpressionTypeNode(node.Expression.Value.get(), GetValueMode::Read);


	if (passtype == PassType::FixedTypes)
	{
		if (!CanBeImplicitConverted(LastExpressionType, BoolType))
		{
			auto  Token = LastLookedAtToken;
			LogCantCastImplicitTypes(Token, LastExpressionType, BoolType, true);
		}
	}

	
	IRBlock::NewConditionalFalseJump_t IfIndex{};
	IRInstruction* BoolCode{};
	if (passtype == PassType::BuidCode)
	{
		DoImplicitConversion(_LastExpressionField, LastExpressionType, BoolType);
		BoolCode = _LastExpressionField;
		IfIndex = LookingAtIRBlock->NewConditionalFalseJump(BoolCode);
	}
	
	
	

	LookingForTypes.pop();

	for (const auto& node2 :node.Body._Nodes)
	{
		OnStatement(*node2);
	}
	
	PopStackFrame();
	_Table.RemoveScope();
	
	if (node.Else)
	{
		IRInstruction* ElseIndex{};
		size_t ElseI{};
		if (passtype == PassType::BuidCode)
		{
			ElseIndex = LookingAtIRBlock->NewJump();
			ElseI = LookingAtIRBlock->GetIndex();
		}


		if (node.Else->Get_Type() != NodeType::ElseNode)
		{
			OnStatement(*node.Else);
		}
		else
		{
			ElseNode* Elsenode = ElseNode::As(node.Else.get());

			for (const auto& node3 : Elsenode->Body._Nodes)
			{
				OnStatement(*node3);
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
	TypeSymbol BoolType(TypesEnum::Bool);

	String ScopeName = std::to_string((size_t)&node);

	PushNewStackFrame();
	_Table.AddScope(ScopeName);

	LookingForTypes.push(BoolType);


	size_t BoolCode;
	size_t BoolJumps;
	if (passtype == PassType::BuidCode)
	{
		BoolCode = LookingAtIRBlock->GetIndex();
		BoolJumps = GetJumpsIndex();
	}
	OnExpressionTypeNode(node.Expression.Value.get(), GetValueMode::Read);

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
		OnStatement(*node2);
	}


	if (passtype == PassType::BuidCode)
	{
		LookingAtIRBlock->NewJump(BoolCode);

		size_t BreakCode = LookingAtIRBlock->GetIndex();

		LookingAtIRBlock->UpdateConditionaJump(IfIndex.ConditionalJump, IfIndex.logicalNot, BreakCode);

		DoJumpsBreakAndContiunes(BoolJumps,BoolCode, BreakCode);
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
	size_t JumpIndex;
	if (passtype == PassType::BuidCode)
	{
		StartIndex= LookingAtIRBlock->GetIndex();
		JumpIndex = GetJumpsIndex();
	}


	for (const auto& node2 : node.Body._Nodes)
	{
		
		OnStatement(*node2);

	}

	_Table.RemoveScope();
	PopStackFrame();

	TypeSymbol BoolType(TypesEnum::Bool);
	LookingForTypes.push(BoolType);

	size_t boolCode;
	if (passtype == PassType::BuidCode)
	{
		boolCode = LookingAtIRBlock->GetIndex();
	}


	OnExpressionTypeNode(node.Expression.Value.get(), GetValueMode::Read);

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

		size_t BreakCode = 0;

		LookingAtIRBlock->NewConditionalFalseJump(_LastExpressionField, StartIndex);


		DoJumpsBreakAndContiunes(JumpIndex,StartIndex, BreakCode);
	}
	



	LookingForTypes.pop();
}
void SystematicAnalysis::OnDeclareStaticVariableNode(const DeclareStaticVariableNode& node)
{
	OnDeclareVariablenode(node.Variable, DeclareStaticVariableNode_t::Static);
}
void SystematicAnalysis::OnDeclareThreadVariableNode(const DeclareThreadVariableNode& node)
{
	OnDeclareVariablenode(node.Variable, DeclareStaticVariableNode_t::Thread);
}

bool SystematicAnalysis::GetMemberTypeSymbolFromVar(const ScopedNameNode& node, GetMemberTypeSymbolFromVar_t& Out)
{
	if (passtype==PassType::GetTypes) {
		return false; 
	}

	auto Token = node.ScopedName.begin()->token;

	TypeSymbol FeildType;
	Symbol* FeildTypeAsSymbol;
	if (Token->Type != TokenType::KeyWord_This) 
	{
		auto& Str = Token->Value._String;
		auto SymbolVar = GetSymbol(Str, SymbolType::Varable_t);
		LastLookedAtToken = Token;
		if (SymbolVar == nullptr)
		{
			LogCantFindVarError(Token, Str);
			return false;
		}

		FeildType = SymbolVar->VarType;
		FeildTypeAsSymbol = SymbolVar;


		if (SymbolVar->Type == SymbolType::Func)
		{
			FuncInfo* Finfo = SymbolVar->Get_Info<FuncInfo>();
			String TepFuncPtr = GetTepFuncPtrName(Finfo);
			//Finfo->FullName = TepFuncPtr;

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
	
	if (Out.Symbol->Type == SymbolType::Class_Field && _FuncStack.size() && _ClassStack.top().Info)
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
	LookingAtIRBlock->NewStore(BuildMember_Store(In), Value);
}
IROperator  SystematicAnalysis::BuildMember_Store(const GetMemberTypeSymbolFromVar_t& In)
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
		return Output;
	case  SymbolType::ParameterVarable:
		return UseOutput ? IROperator(Output) : IROperator(In.Symbol->IR_Par);
		break;
	case SymbolType::StaticVarable:
	case SymbolType::ThreadVarable:
		return IROperator(_Builder.ToID(In.Symbol->FullName));
		break;
	default:
		throw std::exception("not added");
		break;
	}
}
IROperator  SystematicAnalysis::BuildMember_DereferencStore(const GetMemberTypeSymbolFromVar_t& In)
{
	switch (In.Symbol->Type)
	{
	case  SymbolType::StackVarable:
		return IROperator(In.Symbol->IR_Ins);
		break;
	case  SymbolType::ParameterVarable:
		return IROperator(In.Symbol->IR_Par);
		break;
	case  SymbolType::StaticVarable:
	case  SymbolType::ThreadVarable:
		return IROperator(_Builder.ToID(In.Symbol->FullName));
		break;
	default:
		throw std::exception("not added");
		break;
	}
}
IROperator SystematicAnalysis::BuildMember_Store(const GetMemberTypeSymbolFromVar_t& In, const TypeSymbol& Type)
{
	if (Type.IsAddress())
	{
		return BuildMember_DereferencStore(In);
	}
	else
	{
		return BuildMember_Store(In);
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
	case  SymbolType::StaticVarable:
	case  SymbolType::ThreadVarable:
		return LookingAtIRBlock->NewLoadPtr(_Builder.ToID(In.Symbol->FullName));
	default:
		throw std::exception("not added");
		break;
	}
}
void SystematicAnalysis::BuildMemberDereferencStore(const GetMemberTypeSymbolFromVar_t& In, IRInstruction* Value)
{
	LookingAtIRBlock->NewDereferenc_Store(BuildMember_DereferencStore(In), Value);
}
IRInstruction* SystematicAnalysis::BuildMember_GetValue(const GetMemberTypeSymbolFromVar_t& In)
{
	switch (In.Symbol->Type)
	{
	case SymbolType::StaticVarable:
	case SymbolType::ThreadVarable:
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
		else if (In.Symbol->Type == SymbolType::StaticVarable || In.Symbol->Type == SymbolType::ThreadVarable)
		{
			return LookingAtIRBlock->NewLoad_IRID(_Builder.ToID(In.Symbol->FullName));
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
		
		return LookingAtIRBlock->NewLoadFuncPtr(GetIRID(Finfo));
	}
	
	break;
	case SymbolType::Enum_Field:
	{
		auto Einfo = In.Get_V2<EnumInfo>();
		auto FeldInfo = In.Get_V1<EnumFieldInfo>();

		if (Einfo->VariantData.has_value())
		{
			auto ID = _Builder.ToID(Einfo->FullName);
			
			auto Key = LoadEvaluatedEx(FeldInfo->Ex, Einfo->Basetype);

			auto VariantClass = LookingAtIRBlock->NewLoad(IRType(ID));
			IRStruct* V = _Builder.GetSymbol(ID)->Get_ExAs<IRStruct>();
			auto Member = LookingAtIRBlock->New_Member_Access(VariantClass, V, 0);
			LookingAtIRBlock->NewStore(Member, Key);
			return VariantClass;
		}
		else 
		{
			return LoadEvaluatedEx(FeldInfo->Ex, Einfo->Basetype);
		}
	}
	case SymbolType::Type_class:
	{
		if (In.Start[0].token->Type == TokenType::KeyWord_This)
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
	throw std::exception("bad path");
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

	if (In.Symbol->Type == SymbolType::Class_Field && _FuncStack.size() && _ClassStack.top().Info)
	{
		auto& Func = _FuncStack.back();
		auto& PointerIr = LookingAtIRFunc->Pars.front();

		auto ObjectType = *Func.Pointer->GetObjectForCall();
		ObjectType._IsAddress = false;

		auto objecttypesyb = GetSymbol(ObjectType);




		auto IRStructV = ConveToIRClassIR(*objecttypesyb);
		auto F = _Builder.GetSymbol(IRStructV)->Get_ExAs<IRStruct>();

		auto Token = In.Start[In.End - 1].token;
		auto& Str = Token->Value._String;
		ClassInfo* V = objecttypesyb->Get_Info<ClassInfo>();
		size_t MemberIndex = V->GetFieldIndex(Str).value();


		Output = LookingAtIRBlock->New_Member_Dereference(&PointerIr, IRType(IRSymbol(IRStructV)), MemberIndex);
		return;
	}
	if (In.Start[0].token->Type == TokenType::KeyWord_This)
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
		ScopedName::Operator_t OpType = i == 0 ? ScopedName::Operator_t::Null : In.Start[i - 1].Operator;

		if (!(OpType == ScopedName::Operator_t::Null
			|| OpType == ScopedName::Operator_t::Dot
			|| OpType == ScopedName::Operator_t::ScopeResolution))
		{

			auto Datav = Systematic_MemberOverloadData::GetOverloadData(OpType);
			if (Datav.has_value())
			{
				auto& Data = *Datav.value();
				String Scope = ToString(Last_Type);
				ScopeHelper::GetApendedString(Scope, Data.CompilerName);

				auto ConstructorSymbols = _Table.GetSymbolsWithName(Scope, SymbolType::Any);

				Symbol* funcToCallSys = nullptr;

				for (auto& Item2 : ConstructorSymbols)
				{
					if (Item2->Type == SymbolType::Func)
					{
						auto Info = Item2->Get_Info<FuncInfo>();

						if (Info->Pars.size() == 1) {
							funcToCallSys = Item2;
						}
						break;
					}
				}
				if (!funcToCallSys)
				{
					throw std::exception("bad path");
				}

				auto Funcf = funcToCallSys->Get_Info< FuncInfo>();
				Get_FuncInfo V;
				V.Func = Funcf;
				V.SymFunc = funcToCallSys;
				V.ThisPar = Get_FuncInfo::ThisPar_t::PushFromLast;

				if (Output == nullptr)
				{
					_LastExpressionField = In.Symbol->IR_Ins;
				}
				else
				{
					_LastExpressionField = Output;
				}


				ValueParametersNode h;
				DoFuncCall(Last_Type, V, h);

				Last_Type = Funcf->Ret;
				Sym = GetSymbol(Last_Type);
				Output = _LastExpressionField;
			}
		}

		switch (Sym->Type)
		{
		case  SymbolType::Type_class:
		{
			auto* Classinfo = Sym->Get_Info<ClassInfo>();
			size_t MemberIndex = Classinfo->GetFieldIndex(ITem.token->Value._String).value();
			FieldInfo* FInfo = &Classinfo->Fields[MemberIndex];
			IRStruct* IRstruct = _Builder.GetSymbol(SybToIRMap[Sym->ID])->Get_ExAs<IRStruct>();
			if (Output == nullptr)
			{
				switch (In.Symbol->Type)
				{
				case  SymbolType::StackVarable:
				{
					TypeSymbol& TypeSys = Last_Type;
					if (TypeSys.IsAddress())
					{
						Output = LookingAtIRBlock->New_Member_Dereference(In.Symbol->IR_Ins, ConvertToIR(Sym->VarType), MemberIndex);
					}
					else
					{
						Output = LookingAtIRBlock->New_Member_Access(In.Symbol->IR_Ins, IRstruct, MemberIndex);
					}
				}


				break;
				case  SymbolType::ParameterVarable:
				{
					TypeSymbol& TypeSys = Last_Type;
					if (TypeSys.IsAddress())
					{
						Output = LookingAtIRBlock->New_Member_Dereference(In.Symbol->IR_Par, ConvertToIR(Sym->VarType), MemberIndex);
					}
					else
					{
						Output = LookingAtIRBlock->New_Member_Access(In.Symbol->IR_Par, IRstruct, MemberIndex);
					}

				}
				break;
				}
			}
			else
			{
				TypeSymbol& TypeSys = Last_Type;
				if (TypeSys.IsAddress())
				{
					Output = LookingAtIRBlock->New_Member_Dereference(Output, ConvertToIR(Sym->VarType), MemberIndex);
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
		V = &AddSybol(SymbolType::Func_ptr, TepFuncPtr, Finfo->FullName);
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
String SystematicAnalysis::GetTepFuncPtrName(FuncInfo* SymbolVar)
{
	return TepFuncPtrNameMangleStr + SymbolVar->FullName;
}
String_view SystematicAnalysis::GetTepFuncPtrNameAsName(const String_view Str)
{
	return Str.substr(sizeof(TepFuncPtrNameMangleStr)-1);//remove null char
}
bool SystematicAnalysis::GetMemberTypeSymbolFromVar(size_t Start, size_t End, const ScopedNameNode& node, GetMemberTypeSymbolFromVar_t& Out)
{

	if (passtype == PassType::GetTypes) { return false; }

	auto& Mod = GetExpressionMode.top();

	auto TepSyb = Out.Symbol;
	size_t ScopedCount = 0;
	if (TepSyb == nullptr && Out.Type.IsBadType())
	{


		if (node.ScopedName[Start].token->Type != TokenType::KeyWord_This)
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


			
			if (IsWrite(Mod))
			{
				CheckVarWritingErrors(SymbolVar, Token, String_view(Str));
			}
			if (IsRead(Mod))
			{
				LogTryReadVar(String_view(Str), Token, SymbolVar);
			}


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
		if (TepSyb == nullptr)
		{
			return false;
		}
		ScopedName::Operator_t OpType = i == 0 ? ScopedName::Operator_t::Null : node.ScopedName[i - 1].Operator;

		if (OpType == ScopedName::Operator_t::Null
			|| OpType == ScopedName::Operator_t::ScopeResolution
			|| OpType == ScopedName::Operator_t::Dot)
		{

			if (Out.Symbol->Type == SymbolType::Type_class)
			{
				ClassInfo* CInfo = Out.Symbol->Get_Info<ClassInfo>();


				auto FeldInfo = CInfo->GetField(ItemToken->Value._String);
				if (!FeldInfo.has_value())
				{
					if (passtype == PassType::FixedTypes)
					{
						LogCantFindVarMemberError(ItemToken, ItemToken->Value._String, Out.Type);
					}
					return false;
				}

				auto& FieldType2 = (*FeldInfo)->Type;
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
				auto& NameString = ItemToken->Value._String;

				auto FeldInfo = Einfo->GetFieldIndex(NameString);
				if (!FeldInfo.has_value())
				{
					if (passtype == PassType::FixedTypes)
					{
						LogCantFindVarMemberError(ItemToken, ItemToken->Value._String, Out.Type);
					}
					return false;
				}
				else if (Einfo->VariantData)
				{
					auto& Item = Einfo->VariantData.value().Variants[FeldInfo.value()];
					
					bool IsOk = Item.Types.size() == 0;

					if (!IsOk)
					{

						LogMustMakeEnumLikeafuncion(Einfo, FeldInfo.value(), ItemToken);
						return false;
					}
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


				Out.Type.SetType(Out.Symbol->ID);//set enum type

				{
					String FeildSymFullName = Out.Symbol->FullName;
					ScopeHelper::GetApendedString(FeildSymFullName, NameString);

					Symbol* FeildSym = GetSymbol(FeildSymFullName, SymbolType::Enum_Field);

					TepSyb = FeildSym;//set symbol as enum feild


					Out.Set_V1(&Einfo->Fields[*FeldInfo]);
					Out.Set_V2(Einfo);

				}

				AddDependencyToCurrentFile(Out.Symbol);
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
				String TepFuncPtr = GetTepFuncPtrName(Finfo);

				Symbol* V = GetTepFuncPtrSyb(TepFuncPtr, Finfo);
				Out.Symbol = V;
				Out.Type.SetType(V->ID);


				if (passtype == PassType::BuidCode)
				{
					AddDependencyToCurrentFile(Func);
				}

			}
			else if (IsVarableType(Out.Symbol->Type))
			{
				TypeSymbol VarableType = Out.Symbol->VarType;
				Symbol* TypeAsSybol = GetSymbol(VarableType);
				if (TypeAsSybol)
				{
					if (TypeAsSybol->Type != SymbolType::Type_class)
					{
						LogCantFindVarMemberError(ItemToken, ItemToken->Value._String, Out.Type);
						return false;
					}
					ClassInfo* CInfo = TypeAsSybol->Get_Info<ClassInfo>();

					auto FeldInfo = CInfo->GetField(ItemToken->Value._String);
					if (!FeldInfo.has_value())
					{
						if (passtype == PassType::FixedTypes)
						{
							LogCantFindVarMemberError(ItemToken, ItemToken->Value._String, Out.Type);
						}
						return false;
					}

					auto& FieldType2 = (*FeldInfo)->Type;
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
		else//overloadable
		{
			auto Datav = Systematic_MemberOverloadData::GetOverloadData(OpType);
			if (Datav.has_value())
			{
				auto& Data = *Datav.value();
				String Scope = ToString(Out.Type);
				ScopeHelper::GetApendedString(Scope, Data.CompilerName);

				auto ConstructorSymbols = _Table.GetSymbolsWithName(Scope, SymbolType::Any);

				Symbol* funcToCallSys = nullptr;

				for (auto& Item2 : ConstructorSymbols)
				{
					if (Item2->Type == SymbolType::Func)
					{
						auto Info = Item2->Get_Info<FuncInfo>();
						
						if (Info->Pars.size() == 1) {
							funcToCallSys = Item2;
						}
						break;
					}
				}
				if (!funcToCallSys)
				{
					Out.Type.SetType(TypesEnum::Null);
					Out.Symbol = nullptr;

					auto& Item = ItemToken;

					LogCantFindMemberOverloadForType(Item, Data.token, Out.Type);
					return false;
				}
				else
				{
					Out.Symbol = funcToCallSys;
					Out.Type = funcToCallSys->Get_Info< FuncInfo>()->Ret;
				}
			}
		}
	}

	Out.Start = &node.ScopedName[Start - 1];
	Out.End = ScopedCount;
	if (End == -1)
	{
		Out.End++;//i dont know why this needed its most likely its us useing two diff funcs for the same thing to get the start off syb.
	}

	Out.Symbol = TepSyb;

	if (!(TepSyb->Type ==SymbolType::Class_Field
		|| TepSyb->Type == SymbolType::Enum_Field
		|| TepSyb->Type == SymbolType::Func
		|| IsVarableType(TepSyb->Type)))
	{
		Out.Type.SetType(TypesEnum::Null);
		Out.Symbol = nullptr;
		
		if (passtype == PassType::FixedTypes)
		{
			auto& Item = node.ScopedName.back().token;
			LogWantedAVariable(Item, TepSyb);
		}
		
		return false;
	}

	if (IsWrite(Mod) && !(TepSyb->Type == SymbolType::Class_Field || IsVarableType(TepSyb->Type)) )
	{
		auto& Item = node.ScopedName.back().token;
		_ErrorsOutput->AddError(ErrorCodes::InValidType, Item->OnLine, Item->OnPos, "You Cant Write to a " + ToString(TepSyb->Type));
	}



	if (_Varable.size())
	{
		auto& Data = _Varable.top();
		Data._UsedSymbols.push_back(TepSyb);
	}
	return true;
}
void SystematicAnalysis::OnPostfixVariableNode(const PostfixVariableNode& node)
{
	if (passtype == PassType::GetTypes)
	{
		OnExpressionTypeNode(node.ToAssign.Value.get(),GetValueMode::ReadAndWrite);
	}
	else
	if (passtype == PassType::FixedTypes)
	{
		LookingForTypes.push(TypesEnum::Any);
		OnExpressionTypeNode(node.ToAssign.Value.get(), GetValueMode::ReadAndWrite);
		LookingForTypes.pop();

		auto ExType = LastExpressionType;

		auto HasInfo = HasPostfixOverLoadWith(ExType, node.PostfixOp->Type);
		if (!HasInfo.HasValue)
		{
			LogCantFindPostfixOpForTypes(node.PostfixOp, ExType);
		}


		PostFixExpressionNode_Data t;
		
		if (HasInfo.Value) 
		{
			FuncInfo* f = HasInfo.Value.value()->Get_Info<FuncInfo>();
			t.FuncToCall = HasInfo.Value.value();
			t.Op0 = f->Pars[0];
		}
		else
		{
			t.Op0 = LastExpressionType;
		}

		PostFix_Datas.AddValue(&node,t);
	}
	else
	if (passtype == PassType::BuidCode)
	{
		const PostFixExpressionNode_Data& Data = PostFix_Datas.at(&node);

		if (Data.FuncToCall)
		{
			FuncInfo* f = Data.FuncToCall->Get_Info<FuncInfo>();

			Get_FuncInfo V;
			V.Func = f;
			V.SymFunc = Data.FuncToCall;
			V.ThisPar = Get_FuncInfo::ThisPar_t::NoThisPar;

			ScopedNameNode Tep;

			ValueParametersNode pars;
			pars._Nodes.push_back(Unique_ptr<Node>(node.ToAssign.Value.get()));

			DoFuncCall(V, Tep, pars);

			pars._Nodes[0].release();//its ok Tree has pointer.

			LastExpressionType = V.Func->Ret;
		}
		else 
		{
			OnExpressionTypeNode(node.ToAssign.Value.get(), GetValueMode::ReadAndWrite);

			IRInstruction* LoadV = _LastExpressionField;

			auto Type = LastExpressionType;

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
				if (_Settings->PtrSize == IntSizes::Int64) {
					buildPortFixU(64)
				}
				else
				{
					buildPortFixU(32)
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
				if (_Settings->PtrSize == IntSizes::Int64) {
					buildPortFixS(64)
				}
				else
				{
					buildPortFixS(32)
				}
			}
			break;
			default:
				break;
			}
			WriteTo(_LastExpressionField, Type, _LastStoreField);
		}
	}
}
void SystematicAnalysis::OnCompoundStatementNode(const CompoundStatementNode& node)
{

	if (passtype == PassType::GetTypes)
	{
		OnExpressionTypeNode(node.ToAssign.Value.get(),GetValueMode::ReadAndWrite);
		OnExpressionTypeNode(node.Expession.Value.get(),GetValueMode::Read);
		return;
	}
	else if (passtype == PassType::FixedTypes)
	{
		LookingForTypes.push(TypesEnum::Any);
		OnExpressionTypeNode(node.ToAssign.Value.get(), GetValueMode::ReadAndWrite);
		LookingForTypes.pop();

		TypeSymbol ToAssignType = LastExpressionType;

		LookingForTypes.push(TypesEnum::Any);
		OnExpressionTypeNode(node.Expession.Value.get(), GetValueMode::Read);
		LookingForTypes.pop();

		auto ExType = LastExpressionType;


		auto HasInfo = HasCompoundOverLoadWith(ToAssignType, node.CompoundOp->Type, ExType);
		if (!HasInfo.HasValue)
		{
			LogCantFindCompoundOpForTypes(node.CompoundOp, ToAssignType, ExType);
		}

		CompoundExpresion_Data r;

		if (HasInfo.Value)
		{
			FuncInfo* f = HasInfo.Value.value()->Get_Info<FuncInfo>();
			r.FuncToCall = HasInfo.Value.value();
			r.Op0 = f->Pars[0];
			r.Op1 = f->Pars[1];
		}
		else
		{
			r.Op0 = ToAssignType;
			r.Op1 = ExType;

			r.Op0._IsAddress = false;
			r.Op1._IsAddress = false;
		}

		Compound_Datas.AddValue(&node, r);
	}
	else if (passtype == PassType::BuidCode)
	{
		const auto& Data = Compound_Datas.at(&node);

		if (Data.FuncToCall)
		{

			FuncInfo* f = Data.FuncToCall->Get_Info<FuncInfo>();

			Get_FuncInfo V;
			V.Func = f;
			V.SymFunc = Data.FuncToCall;
			V.ThisPar = Get_FuncInfo::ThisPar_t::PushFromLast;


			ScopedNameNode Tep;
			ScopedName TepV;
			TepV.token = LastLookedAtToken;
			Tep.ScopedName.push_back(TepV);

			ValueParametersNode pars;
			pars._Nodes.push_back(Unique_ptr<Node>(node.ToAssign.Value.get()));
			pars._Nodes.push_back(Unique_ptr<Node>(node.Expession.Value.get()));


			DoFuncCall(V, Tep, pars);

			pars._Nodes[0].release();
			pars._Nodes[1].release();
			//no mem leak node as a Unique_ptr to Ex

			LastExpressionType = V.Func->Ret;
		}
		else
		{

			LookingForTypes.push(Data.Op0);

			OnExpressionTypeNode(node.ToAssign.Value.get(), GetValueMode::ReadAndWrite);
			auto AssignType = LastExpressionType;

			LookingForTypes.pop();

			IRInstruction* LoadV = _LastExpressionField;

			LookingForTypes.push(Data.Op1);

			OnExpressionTypeNode(node.Expession.Value.get(), GetValueMode::Read);

			LookingForTypes.pop();


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


			switch (Data.Op0._Type)
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
				if (_Settings->PtrSize == IntSizes::Int64) {
					Set_CompoundU(64)
				}
				else
				{
					Set_CompoundU(32)
				}
			};
			break;

			case TypesEnum::sIntPtr:
			{
				if (_Settings->PtrSize == IntSizes::Int64) {
					Set_CompoundS(64)
				}
				else
				{
					Set_CompoundS(32)
				}
			};
			break;



			break;

			default:
				throw std::exception("Bad Op");
				break;
			}

			WriteTo(_LastExpressionField, AssignType, _LastStoreField);
		}
	}
}
void SystematicAnalysis::OnExpressionTypeNode(const Node* node, GetValueMode Mode)
{
	GetExpressionMode.push(Mode);

	PushToNodeScope(*node);

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
	
	PopNodeScope();
	GetExpressionMode.pop();
}
void SystematicAnalysis::OnExpressionNode(const ValueExpressionNode& node)
{
	
	if (passtype == PassType::BuidCode
		|| passtype == PassType::FixedTypes
		
		|| (passtype == PassType::GetTypes && 
			(node.Value->Get_Type() == NodeType::LambdaNode)
		))
	{
		
		switch (node.Value->Get_Type())
		{
		case NodeType::NumberliteralNode:
		{
			NumberliteralNode* num = NumberliteralNode::As(node.Value.get());

			OnNumberliteralNode(num);
		}
		break;
		case NodeType::BoolliteralNode:
		{
			BoolliteralNode* num = BoolliteralNode::As(node.Value.get());
			
			OnBoolliteralNode(num);
		}
		break;
		case NodeType::CharliteralNode:
		{
			CharliteralNode* num = CharliteralNode::As(node.Value.get());

			OnCharliteralNode(num);
		}
		break;
		case NodeType::FloatliteralNode:
		{
			FloatliteralNode* num = FloatliteralNode::As(node.Value.get());
			OnFloatLiteralNode(num);
		}
		break;
		case NodeType::StringliteralNode:
		{
			StringliteralNode* nod = StringliteralNode::As(node.Value.get());

			bool retflag;
			OnStringLiteral(nod, retflag);
			if (retflag) return;
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

			OnSizeofNode(nod);
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

			OnExpressionTypeNode(nod->Expression.Value.get(), GetValueMode::Read);
		}
		break;
		case NodeType::MoveNode:
		{
			MoveNode* nod = MoveNode::As(node.Value.get());

			OnMovedNode(nod);
		}
		break;
		case NodeType::LambdaNode:
		{
			auto nod = LambdaNode::As(node.Value.get());
			OnLambdaNode(*nod);
		}
		break;
		case NodeType::BitCastExpression:
		{
			auto nod = BitCastExpression::As(node.Value.get());
			OnBitCast(*nod);
		}
		break;
		default:
			throw std::exception("not added");
			break;
		}
	}
}

void SystematicAnalysis::OnMovedNode(UCodeLang::FrontEnd::MoveNode* nod)
{

	OnExpressionTypeNode(nod->expression.Value.get(), GetValueMode::Read);
	
	auto ExType = LastExpressionType;
	ExType.SetAsMoved();
	LastExpressionType = ExType;
}

void SystematicAnalysis::OnNumberliteralNode(UCodeLang::FrontEnd::NumberliteralNode* num)
{
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
#define Set_NumberliteralNodeU(x) \
			UInt##x V; \
			ParseHelper::ParseStringToUInt##x(Str, V); \
			_LastExpressionField = LookingAtIRBlock->NewLoad(V);\

#define Set_NumberliteralNodeS(x) \
			Int##x V; \
			ParseHelper::ParseStringToInt##x(Str, V); \
			_LastExpressionField = LookingAtIRBlock->NewLoad(V);\

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

void SystematicAnalysis::OnBoolliteralNode(UCodeLang::FrontEnd::BoolliteralNode* num)
{
	if (passtype == PassType::BuidCode)
	{
		_LastExpressionField = LookingAtIRBlock->NewLoad(num->Get_Value());
	}
	LastExpressionType.SetType(TypesEnum::Bool);
	LastLookedAtToken = num->Token;
}

void SystematicAnalysis::OnCharliteralNode(UCodeLang::FrontEnd::CharliteralNode* num)
{
	if (passtype == PassType::BuidCode)
	{
		String V;
		bool ItWorked = !ParseHelper::ParseCharliteralToChar(num->Token->Value._String, V);


		_LastExpressionField = LookingAtIRBlock->NewLoad((char)V.front());
	}
	LastExpressionType.SetType(TypesEnum::Char);
	LastLookedAtToken = num->Token;
}

void SystematicAnalysis::OnFloatLiteralNode(UCodeLang::FrontEnd::FloatliteralNode* num)
{
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

void SystematicAnalysis::OnStringLiteral(UCodeLang::FrontEnd::StringliteralNode* nod, bool& retflag)
{
	retflag = true;
	if (passtype == PassType::GetTypes)
	{//check for ok string

	}


	auto& Type = Get_LookingForType();

	bool IsStaticArr = false;
	if (Type._Type == TypesEnum::CustomType)
	{
		auto V = GetSymbol(Type);
		if (V->Type == SymbolType::Type_StaticArray)
		{
			StaticArrayInfo* StaticArr = V->Get_Info< StaticArrayInfo>();

			TypeSymbol CharType;
			CharType.SetType(TypesEnum::Char);

			IsStaticArr = AreTheSame(CharType, StaticArr->Type);
		}
	}



	if (IsStaticArr)
	{
		auto V = GetSymbol(Type);
		StaticArrayInfo* StaticArr = V->Get_Info< StaticArrayInfo>();

		if (passtype == PassType::FixedTypes)
		{
			String V;
			bool ItWorked = !ParseHelper::ParseStringliteralToString(nod->Token->Value._String, V);
			size_t BufferSize = V.size();

			if (StaticArr->IsCountInitialized == false)
			{


				StaticArr->Count = V.size() + 1;//with null char;
				StaticArr->IsCountInitialized = true;

			}
			else
			{
				if (StaticArr->Count != BufferSize)
				{
					const Token* Token = LastLookedAtToken;
					LogCanIncorrectStaticArrCount(Token, Type, BufferSize, StaticArr->Count);
					LastExpressionType.SetType(TypesEnum::Null);
					return;
				}
			}
		}

		if (passtype == PassType::BuidCode)
		{
			String V;
			bool ItWorked = !ParseHelper::ParseStringliteralToString(nod->Token->Value._String, V);



			auto& BufferIR = IRlocations.top();
			BufferIR.UsedlocationIR = true;
			auto BufferIRIns = BufferIR.Value;

			const auto& ArrItemType = StaticArr->Type;
			const auto IRItemType = ConvertToIR(ArrItemType);
			UAddress Size;
			GetSize(ArrItemType, Size);
			auto ValueSizeIR = IR_Load_UIntptr(Size);

			if (!Type.IsAddress())
			{
				BufferIRIns = LookingAtIRBlock->NewLoadPtr(BufferIRIns);
			}

			for (size_t i = 0; i < V.size(); i++)
			{
				auto VIR = LookingAtIRBlock->NewLoad(V[i]);


				auto f = LookingAtIRBlock->New_Index_Vetor(BufferIRIns, IR_Load_UIntptr(i), ValueSizeIR);

				LookingAtIRBlock->NewDereferenc_Store(f, VIR);
			}


		}

		LastExpressionType = Type;
	}
	else
	{

		TypeSymbol CStringType;//umut char[&]
		CStringType.SetType(TypesEnum::Char);
		CStringType.SetAsAddressArray();
		CStringType.SetAsimmutable();
		LastExpressionType = CStringType;

		if (passtype == PassType::BuidCode)
		{
			String V;
			bool ItWorked = !ParseHelper::ParseStringliteralToString(nod->Token->Value._String, V);

			String_view Buffer{ V.c_str(),V.size() + 1 };//for null char

			auto BufferIR = _Builder.FindOrAddConstStrings(Buffer);
			_LastExpressionField = LookingAtIRBlock->NewLoadPtr(BufferIR);


		}

	}
	retflag = false;
}

void SystematicAnalysis::OnSizeofNode(UCodeLang::FrontEnd::SizeofExpresionNode* nod)
{
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
		ConvertAndValidateType(nod->Type, Info,NodeSyb_t::Any);
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


void SystematicAnalysis::OnNewNode(NewExpresionNode* nod)
{
	TypeSymbol Type;
	Convert(nod->Type, Type);
	bool IsArray = nod->Arrayexpression.Value.get();

	if (passtype == PassType::FixedTypes)
	{
		if (IsArray)
		{
			TypeSymbol UintptrType(TypesEnum::uIntPtr);

			LookingForTypes.push(UintptrType);
			OnExpressionTypeNode(nod->Arrayexpression.Value.get(),GetValueMode::Read);

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
		LastExpressionType = Type;
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


			OnExpressionTypeNode(nod->Arrayexpression.Value.get(), GetValueMode::Read);

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



			LastExpressionType = Type;
			LastExpressionType._IsAddressArray =true;
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
			
			LastExpressionType= Type;
			LastExpressionType._IsAddress = true;
		}
	
	}

	

}

void SystematicAnalysis::OnAnonymousObjectConstructor(AnonymousObjectConstructorNode*& nod)
{
	auto& Type = Get_LookingForType();

	if (Type._Type == TypesEnum::CustomType)
	{
		auto V = GetSymbol(Type);
		if (V->Type == SymbolType::Type_StaticArray)
		{
			StaticArrayInfo* StaticArr = V->Get_Info< StaticArrayInfo>();
			if (passtype == PassType::FixedTypes)
			{

				if (StaticArr->IsCountInitialized)
				{
					if (StaticArr->Count != nod->Fields._Nodes.size())
					{
						const Token* Token = LastLookedAtToken;
						LogCanIncorrectStaticArrCount(Token, Type, nod->Fields._Nodes.size(), StaticArr->Count);
						LastExpressionType.SetType(TypesEnum::Null);
						return;
					}
				}
				else
				{
					StaticArr->Count = nod->Fields._Nodes.size();
					StaticArr->IsCountInitialized = true;
				}

				const auto& ArrItemType = StaticArr->Type;
				for (size_t i = 0; i < nod->Fields._Nodes.size(); i++)
				{
					OnExpressionTypeNode(nod->Fields._Nodes[i].get(), GetValueMode::Read);

					if (!CanBeImplicitConverted(LastExpressionType, ArrItemType, false))
					{
						LogCantCastImplicitTypes(LastLookedAtToken, LastExpressionType, ArrItemType, false);
					}
				}
			}
			if (passtype == PassType::BuidCode)
			{
				auto& BufferIR = IRlocations.top();
				BufferIR.UsedlocationIR = true;
				auto BufferIRIns = BufferIR.Value;

				const auto& ArrItemType = StaticArr->Type;
				const auto IRItemType = ConvertToIR(ArrItemType);
				UAddress Size;
				GetSize(ArrItemType, Size);
				auto ValueSizeIR = IR_Load_UIntptr(Size);

				if (!Type.IsAddress())
				{
					BufferIRIns = LookingAtIRBlock->NewLoadPtr(BufferIRIns);
				}

				for (size_t i = 0; i < nod->Fields._Nodes.size(); i++)
				{
					OnExpressionTypeNode(nod->Fields._Nodes[i].get(), GetValueMode::Read);

					DoImplicitConversion(_LastExpressionField, LastExpressionType, ArrItemType);

					auto V = LookingAtIRBlock->New_Index_Vetor(BufferIRIns, IR_Load_UIntptr(i), ValueSizeIR);

					LookingAtIRBlock->NewDereferenc_Store(V, _LastExpressionField);
				}
			}

			LastExpressionType = Type;
			return;
		}
	}

	if (Type.IsnotAn(TypesEnum::Var))//function who called this can deal with var
	{
		

		if (passtype == PassType::FixedTypes)
		{
			auto Func = GetFunc(Type, nod->Fields);
			FuncToSyboID[nod] = Func;

			SetFuncRetAsLastEx(Func);
			return;
		}
		else if (passtype == PassType::BuidCode)
		{
			const auto& Func = FuncToSyboID.at(nod);
			auto& ValuePars = nod->Fields;



			DoFuncCall(Type, Func, ValuePars);


		}
	}
	else
	{
		if (passtype == PassType::FixedTypes)
		{
			{
				TypeSymbol AnyType; AnyType.SetType(TypesEnum::Any);
				LookingForTypes.push(AnyType);
			}

			TypeSymbol ArrItemType;
			if (nod->Fields._Nodes.size())
			{
				OnExpressionTypeNode(nod->Fields._Nodes[0].get(), GetValueMode::Read);
				ArrItemType = LastExpressionType;
				LookingForTypes.top() = ArrItemType;
			}
			else
			{
				ArrItemType.SetType(TypesEnum::uInt8);
			}


			TypeSymbol NewType;
			StaticArrayInfo* info = nullptr;
			{
				SymbolID id = (SymbolID)nod;
				auto FullName = CompilerGenerated("StaticArray_") + ToString(ArrItemType) + std::to_string(id);


				auto Syb = &AddSybol(SymbolType::Type_StaticArray, FullName, FullName);
				_Table.AddSymbolID(*Syb, id);

				info = new StaticArrayInfo();


				info->Type = ArrItemType;
				info->Exnode = nullptr;

				Syb->Info.reset(info);

				NewType.SetType(id);
			}


			info->Count = nod->Fields._Nodes.size();
			info->IsCountInitialized = true;

			for (size_t i = 1; i < nod->Fields._Nodes.size(); i++)
			{
				OnExpressionTypeNode(nod->Fields._Nodes[i].get(), GetValueMode::Read);
				
				if (!CanBeImplicitConverted(LastExpressionType, ArrItemType,false))
				{
					LogCantCastImplicitTypes(LastLookedAtToken,LastExpressionType, ArrItemType, false);
				}
			}

			LastExpressionType = NewType;
			{
				LookingForTypes.pop();
			}
			return;
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
	auto Str = FToken->Value._String;

	if (FToken->Type == TokenType::KeyWord_This)
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


	Symbol = GetSymbol(Str,SymbolType::Varable_t);
	
	ReadVarErrorCheck_t Info;

	

	if (IsRead(GetExpressionMode.top()))
	{
		Info = LogTryReadVar(Str, Token, Symbol);
	}
	if (IsWrite(GetExpressionMode.top()))
	{
		Symbol->SetTovalid();

	}

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

			if (IsWrite(GetExpressionMode.top()))
			{
				_LastStoreField = BuildMember_Store(V, V.Type);
			}

			if (IsRead(GetExpressionMode.top())) 
			{
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
		LookingForTypes.push(LookingForTypes.top());
	}
	else
	{
		TypeSymbol V; V.SetType(TypesEnum::Any);
		LookingForTypes.push(V);
	}

	BinaryExpressionNode_Data* Data =nullptr;
	bool BuildCode = passtype == PassType::BuidCode;
	if (BuildCode)
	{
		Data = &BinaryExpressionNode_Datas.at(&node);
		LookingForTypes.top() = Data->Op1;

		if (Data->FuncToCall)
		{
			LookingForTypes.pop();
			goto BuildCodePart;
		}
	}

	OnExpressionTypeNode(Ex1node, GetValueMode::Read);
	auto Ex0 = _LastExpressionField;
	auto Ex0Type = LastExpressionType;
	
	if (BuildCode)
	{
		LookingForTypes.top() = Data->Op0;
	}
	

	OnExpressionTypeNode(Ex0node, GetValueMode::Read);
	auto Ex1 = _LastExpressionField;
	auto Ex1Type = LastExpressionType;


	LookingForTypes.pop();

	if (passtype == PassType::FixedTypes)
	{
		auto BinaryOp = node.BinaryOp;
		auto Info = HasBinaryOverLoadWith(Ex0Type, BinaryOp->Type, Ex1Type);

		if (!Info.HasValue)
		{
			LogCantFindBinaryOpForTypes(BinaryOp, Ex0Type, Ex1Type);
		}

		auto Op = node.BinaryOp->Type;
		

		BinaryExpressionNode_Data V;
		V.Op0 = Ex0Type;
		V.Op1 = Ex1Type;
	
		
		//all float bool int types
		if (Info.Value.has_value())
		{
			FuncInfo* f = Info.Value.value()->Get_Info<FuncInfo>();
			V.Op0 = f->Pars[0];
			V.Op1 = f->Pars[1];
			V.FuncToCall = Info.Value.value();
		
			LastExpressionType = f->Ret;
		}
		else
		{
			V.Op0._IsAddress = false;
			V.Op1._IsAddress = false;
			LastExpressionType = BinaryExpressionShouldRurn(Op, Ex0Type);
		}

		BinaryExpressionNode_Datas[&node] = V;

		
	}


	BuildCodePart:
	if (passtype == PassType::BuidCode)
	{
		auto& Data = BinaryExpressionNode_Datas[&node];

		if (Data.FuncToCall)
		{
			FuncInfo* f = Data.FuncToCall->Get_Info<FuncInfo>();


			Get_FuncInfo V;
			V.Func = f;
			V.SymFunc = Data.FuncToCall;
			V.ThisPar = Get_FuncInfo::ThisPar_t::NoThisPar;
			
			
			ScopedNameNode Tep;
			ScopedName TepV;
			TepV.token = node.BinaryOp;
			Tep.ScopedName.push_back(TepV);

			ValueParametersNode pars;
			pars._Nodes.push_back(Unique_ptr<Node>(Ex0node));
			pars._Nodes.push_back(Unique_ptr<Node>(Ex1node));

			DoFuncCall(V, Tep,pars);

			//save so not free mem
			auto par0 = pars._Nodes[0].release();
			auto par1 = pars._Nodes[1].release();
			//its ok.no mem leak Par node has Unique_ptr to Ex0 and Ex1 just borrowing them
			LastExpressionType = V.Func->Ret;
		}
		else 
		{

			auto& Type = Ex0Type;
			auto Op = node.BinaryOp->Type;
			LastExpressionType = BinaryExpressionShouldRurn(Op, Ex0Type);

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
	case TokenType::lessthan:
	case TokenType::greaterthan:
	case TokenType::less_than_or_equalto:
	case TokenType::greater_than_or_equalto:
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
	ConvertAndValidateType(node.ToType, ToTypeAs,NodeSyb_t::Any);
	LookingForTypes.push(ToTypeAs);
	

	if (passtype == PassType::BuidCode)
	{
		auto& Item =CastDatas.at(&node);
		if (Item.FuncToCall) 
		{
			LookingForTypes.push(Item.FuncToCall->Get_Info<FuncInfo>()->Pars[0]);

			OnExpressionTypeNode(node.Expression.Value.get(), GetValueMode::Read);
			auto Ex0 = _LastExpressionField;
			auto Ex0Type = LastExpressionType;

			LookingForTypes.pop();
		}
		else
		{
			OnExpressionTypeNode(node.Expression.Value.get(), GetValueMode::Read);
			auto Ex0 = _LastExpressionField;
			auto Ex0Type = LastExpressionType;
		}
	}
	else
	{
		OnExpressionTypeNode(node.Expression.Value.get(), GetValueMode::Read);
		auto Ex0 = _LastExpressionField;
		auto Ex0Type = LastExpressionType;
	}
	

	if (passtype == PassType::FixedTypes) 
	{
		CastExpressionNode_Data data;
		

		TypeSymbol ToTypeAs;
		ConvertAndValidateType(node.ToType, ToTypeAs, NodeSyb_t::Any);

		auto Ex0Type = LastExpressionType;
		auto HasInfo = CanBeExplicitlyConverted(Ex0Type, ToTypeAs);
		if (!HasInfo.HasValue)
		{
			auto  Token = node.ToType.Name.Token;

			LogCantCastExplicityTypes(Token, Ex0Type, ToTypeAs);
		}
		else
		{
			if (HasInfo.Value.has_value()) 
			{
				data.FuncToCall = HasInfo.Value.value();
				LastExpressionType = HasInfo.Value.value()->Get_Info<FuncInfo>()->Ret;
			}
			else
			{
				LastExpressionType = ToTypeAs;
			}

		}

		
		CastDatas.AddValue(&node, data);
	}

	if (passtype == PassType::BuidCode)
	{
		DoExplicitlConversion(_LastExpressionField, LastExpressionType, ToTypeAs,CastDatas.at(&node));

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
		OnExpressionTypeNode(node.SourceExpression.Value.get(), GetExpressionMode.top());
		OnExpressionTypeNode(node.IndexExpression.Value.get(), GetValueMode::Read);
	}

	if (passtype == PassType::FixedTypes)
	{

		OnExpressionTypeNode(node.SourceExpression.Value.get(), GetExpressionMode.top());
		TypeSymbol SourcType = LastExpressionType;

		
		TypeSymbol gesstype;
		gesstype.SetType(TypesEnum::Any);


		bool IsSrcAddress = SourcType.IsAddress();
		bool IsSrcStaticArray = IsStaticArray(SourcType);

		if (IsSrcAddress || IsSrcStaticArray)
		{

			gesstype.SetType(TypesEnum::uIntPtr);
		}


		LookingForTypes.push(gesstype);


		OnExpressionTypeNode(node.IndexExpression.Value.get(), GetValueMode::Read);
		TypeSymbol IndexType = LastExpressionType;


		LookingForTypes.pop();

		auto HasInfo = HasIndexedOverLoadWith(SourcType, IndexType);
		if (!HasInfo.HasValue)
		{
			auto  Token = LastLookedAtToken;
			LogCantBeIndexWithType(Token, SourcType, IndexType);
		}



		


		IndexedExpresion_Data V;
		


		//all float bool int types
		if (HasInfo.Value.has_value())
		{
			FuncInfo* f = HasInfo.Value.value()->Get_Info<FuncInfo>();

			V.Op0 = f->Pars[0];
			V.Op1 = f->Pars[1];
			V.FuncToCall = HasInfo.Value.value();

			LastExpressionType = f->Ret;
		}
		else
		{
			TypeSymbol lookingfor = LookingForTypes.top();
			V.Op0 = SourcType;
			V.Op1 = IndexType;
			V.Op0._IsAddress = true;
			V.Op1._IsAddress = false;

			if (IsSrcAddress)
			{
				lookingfor = SourcType;
				LastExpressionType = lookingfor;
			}
			else if (IsSrcStaticArray)
			{
				auto Syb = GetSymbol(SourcType);
				lookingfor = Syb->Get_Info<StaticArrayInfo>()->Type;
				lookingfor.SetAsAddress();

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

		if (Data.FuncToCall)
		{
			FuncInfo* f =Data.FuncToCall->Get_Info<FuncInfo>();

			Get_FuncInfo V;
			V.Func = f;
			V.SymFunc = Data.FuncToCall;
			V.ThisPar = Get_FuncInfo::ThisPar_t::NoThisPar;


			ScopedNameNode Tep;
			ScopedName TepV;
			TepV.token = LastLookedAtToken;
			Tep.ScopedName.push_back(TepV);

			ValueParametersNode pars;
			pars._Nodes.push_back(Unique_ptr<Node>(node.SourceExpression.Value.get()));
			pars._Nodes.push_back(Unique_ptr<Node>(node.IndexExpression.Value.get()));

			DoFuncCall(V, Tep, pars);

			//save so not free mem
			auto par0 = pars._Nodes[0].release();
			auto par1 = pars._Nodes[1].release();
			//its ok.no mem leak Par node has Unique_ptr to SourceExpression and IndexExpression just borrowing them
			LastExpressionType = V.Func->Ret;
		}
		else
		{
			LookingForTypes.push(Data.Op0);
			OnExpressionTypeNode(node.SourceExpression.Value.get(), GetValueMode::Read);
			LookingForTypes.pop();

			auto Pointer = _LastExpressionField;
			auto SourcType = LastExpressionType;

			LookingForTypes.push(Data.Op1);
			OnExpressionTypeNode(node.IndexExpression.Value.get(), GetValueMode::Read);
			LookingForTypes.pop();

			auto IndexField = _LastExpressionField;

			TypeSymbol IndexedObjectPointer = Data.Op0;
			IndexedObjectPointer._IsAddressArray = false;
			IndexedObjectPointer._IsAddress = false;

			UAddress V;
			{
				auto VSyb = GetSymbol(IndexedObjectPointer);
				if (VSyb->Type == SymbolType::Type_StaticArray)
				{
					StaticArrayInfo* info = VSyb->Get_Info<StaticArrayInfo>();
					GetSize(info->Type, V);
				}
				else
				{
					GetSize(IndexedObjectPointer, V);
				}

			}
			
			

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

			if (IsWrite(GetExpressionMode.top()))
			{
				_LastStoreField =IROperator(_LastExpressionField);
			}
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
		
		SetFuncRetAsLastEx(Info);
	}
	else if (passtype == PassType::BuidCode)
	{
		auto& SybID = FuncToSyboID.at(&node);
		DoFuncCall(SybID, node.FuncName, node.Parameters);
	}
}
void SystematicAnalysis::SetFuncRetAsLastEx(Get_FuncInfo& Info)
{
	if (Info.Func)
	{
		if (IsVarableType(Info.SymFunc->Type))
		{
			LastExpressionType = ((FuncPtrInfo*)Info.Func)->Ret;
		}
		else 
		{
			if (Info.Func->_FuncType == FuncInfo::FuncType::New)
			{
				LastExpressionType = (*Info.Func->GetObjectForCall());
				LastExpressionType._IsAddress = false;
			}
			else
			{
				LastExpressionType = Info.Func->Ret;
			}
		}
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

	OnExpressionTypeNode(node.expression.Value.get(), GetValueMode::Read);

	if (passtype == PassType::BuidCode)
	{
		LookingForTypes.pop();
	}

	auto Ex0 = _LastExpressionField;
	auto Ex0Type = LastExpressionType;
	if (passtype == PassType::FixedTypes)
	{

		if (!(Ex0Type.IsAddress() || Ex0Type.IsAddressArray() || Ex0Type._IsDynamic))
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

			if (Ex0Type._IsDynamic)
			{

				auto ExPtr = Ex0Type.IsAddress() ? Ex0 : LookingAtIRBlock->NewLoadPtr(Ex0);

				auto IRStructType = ConvertToIR(Ex0Type);

				auto PtrMember = LookingAtIRBlock->New_Member_Dereference(ExPtr, IRStructType, 0);
				auto VTableMember = LookingAtIRBlock->New_Member_Dereference(ExPtr, IRStructType, 1);

				auto Trait = GetSymbol(Ex0Type);
				auto Traitinfo = Trait->Get_Info<TraitInfo>();
				
				size_t Index = 0;

				for (size_t i = 0; i < Traitinfo->_Funcs.size(); i++)
				{
					auto& Item = Traitinfo->_Funcs[i];
					if (Item.Syb->Get_Info<FuncInfo>()->_FuncType == FuncInfo::FuncType::Drop)
					{
						Index = i;
						break;
					}
				}
				auto DestructorFuncPtr = LookingAtIRBlock->New_Member_Dereference(VTableMember,IRType(_Builder.ToID(GetTraitVStructTableName(Trait->FullName))), Index);

				LookingAtIRBlock->NewPushParameter(PtrMember);
				LookingAtIRBlock->NewCallFuncPtr(DestructorFuncPtr);

				LookingAtIRBlock->NewFreeCall(PtrMember);
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

	for (const auto& Item : lib.Get_NameToPtr())
	{
		const auto& Offset = Item._Value;
		const auto& FuncStr = Item._Key;
		

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
	if (TypeA._MoveData != TypeB._MoveData)
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
			FuncPtrInfo* F1 = TypeOne.Get_Info<FuncPtrInfo>();
			FuncPtrInfo* F2 = TypeTwo.Get_Info<FuncPtrInfo>();
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
		if (TypeOne.Type == SymbolType::Type_StaticArray && TypeOne.Type == SymbolType::Type_StaticArray)
		{
			StaticArrayInfo* F1 = TypeOne.Get_Info<StaticArrayInfo>();
			StaticArrayInfo* F2 = TypeTwo.Get_Info<StaticArrayInfo>();

			if (F1->Count == F2->Count)
			{
				return AreTheSameWithOutimmutable(F1->Type, F2->Type);
			}
		}

	}
	else if ((IsPrimitive(TypeA) && IsPrimitive(TypeB)) && TypeA._Type == TypeB._Type
		)
	{
		return true;
	}



	return false;
}
SystematicAnalysis::BinaryOverLoadWith_t SystematicAnalysis::HasBinaryOverLoadWith(const TypeSymbol& TypeA, TokenType BinaryOp, const TypeSymbol& TypeB)
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
			return { IsMathOp || IsSameValueComparisonOp || IsMathValueComparisonOp || Isbitwise ,{} };
		}
		if (IsfloatType(TypeA))
		{
			return { IsMathOp || IsSameValueComparisonOp || IsMathValueComparisonOp,{} };
		}

		if (TypeA._Type == TypesEnum::Char)
		{
			return { IsSameValueComparisonOp, {} };
		}
		if (TypeA._Type == TypesEnum::Bool)
		{
			return { IsSameValueComparisonOp || IslogicalOperator , {} };
		}

	}

	auto Syb = GetSymbol(TypeA);
	if (Syb)
	{
		if (Syb->Type == SymbolType::Type_class)
		{

			for (auto& Item : Systematic_BinaryOverloadData::Data)
			{
				if (Item.token == BinaryOp)
				{
					String funcName = Syb->FullName;
					ScopeHelper::GetApendedString(funcName,Item.CompilerName);

					auto& V = _Table.GetSymbolsWithName(funcName, SymbolType::Func);

					for (auto& Item : V)
					{
						if (Item->Type == SymbolType::Func)
						{
							auto funcInfo = Item->Get_Info<FuncInfo>();
							if (funcInfo->Pars.size() == 2)
							{
								bool r = CanBeImplicitConverted(TypeA, funcInfo->Pars[0])
									&& CanBeImplicitConverted(TypeB, funcInfo->Pars[1]);
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
SystematicAnalysis::CompoundOverLoadWith_t SystematicAnalysis::HasCompoundOverLoadWith(const TypeSymbol& TypeA, TokenType BinaryOp, const TypeSymbol& TypeB)
{
	if (AreTheSameWithOutimmutable(TypeA, TypeB))
	{
		if (IsIntType(TypeA))
		{
			return { true };
		}
	}

	auto Syb = GetSymbol(TypeA);
	if (Syb)
	{
		if (Syb->Type == SymbolType::Type_class)
		{

			for (auto& Item : Systematic_CompoundOverloadData::Data)
			{
				if (Item.token == BinaryOp)
				{
					String funcName = Syb->FullName;
					ScopeHelper::GetApendedString(funcName, Item.CompilerName);

					auto& V = _Table.GetSymbolsWithName(funcName, SymbolType::Func);

					for (auto& Item : V)
					{
						if (Item->Type == SymbolType::Func)
						{
							auto funcInfo = Item->Get_Info<FuncInfo>();
							if (funcInfo->Pars.size() == 2)
							{
								bool r = CanBeImplicitConverted(TypeA, funcInfo->Pars[0])
									 && CanBeImplicitConverted(TypeB, funcInfo->Pars[1]);
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
SystematicAnalysis::PostFixOverLoadWith_t SystematicAnalysis::HasPostfixOverLoadWith(const TypeSymbol& TypeA, TokenType BinaryOp)
{

	if (IsIntType(TypeA))
	{
		return { true,{} };
	}

	auto Syb = GetSymbol(TypeA);
	if (Syb)
	{
		if (Syb->Type == SymbolType::Type_class)
		{

			for (auto& Item : Systematic_PostfixOverloadData::Data)
			{
				if (Item.token == BinaryOp)
				{
					String funcName = Syb->FullName;
					ScopeHelper::GetApendedString(funcName, Item.CompilerName);

					auto& V = _Table.GetSymbolsWithName(funcName, SymbolType::Func);

					for (auto& Item : V)
					{
						if (Item->Type == SymbolType::Func)
						{
							auto funcInfo = Item->Get_Info<FuncInfo>();
							if (funcInfo->Pars.size() == 1)
							{
								bool r = CanBeImplicitConverted(TypeA, funcInfo->Pars[0]);
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
SystematicAnalysis::IndexOverLoadWith_t SystematicAnalysis::HasIndexedOverLoadWith(const TypeSymbol& TypeA, const TypeSymbol& TypeB)
{

	if (IsUIntType(TypeB)) 
	{
		if (TypeA.IsAddressArray())
		{
			return { true, {} };
		}

		if (IsStaticArray(TypeA))
		{
			return { true,{} };
		}
	}

	auto Syb = GetSymbol(TypeA);
	if (Syb)
	{
		if (Syb->Type == SymbolType::Type_class)
		{

			String funcName = Syb->FullName;
			ScopeHelper::GetApendedString(funcName,Overload_Index_Func);

			auto& V = _Table.GetSymbolsWithName(funcName, SymbolType::Func);

			for (auto& Item : V)
			{
				if (Item->Type == SymbolType::Func)
				{
					auto funcInfo = Item->Get_Info<FuncInfo>();
					if (funcInfo->Pars.size() == 2)
					{
						bool r = CanBeImplicitConverted(TypeA, funcInfo->Pars[0])
							&& CanBeImplicitConverted(TypeB, funcInfo->Pars[1]);
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
SystematicAnalysis::ForOverLoadWith_t SystematicAnalysis::HasForOverLoadWith(const TypeSymbol& TypeA)
{
	auto Syb = GetSymbol(TypeA);
	if (Syb)
	{
		if (Syb->Type == SymbolType::Type_class)
		{

			String funcName = Syb->FullName;
			ScopeHelper::GetApendedString(funcName, Overload_For_Func);

			auto& V = _Table.GetSymbolsWithName(funcName, SymbolType::Func);

			for (auto& Item : V)
			{
				if (Item->Type == SymbolType::Func)
				{
					auto funcInfo = Item->Get_Info<FuncInfo>();
					if (funcInfo->Pars.size() == 1)
					{
						bool r = CanBeImplicitConverted(TypeA, funcInfo->Pars[0]);
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
SystematicAnalysis::UrinaryOverLoadWith_t SystematicAnalysis::HasUrinaryOverLoadWith(const TypeSymbol& TypeA, TokenType Op)
{
	auto Syb = GetSymbol(TypeA);
	if (Syb)
	{
		if (Syb->Type == SymbolType::Type_class)
		{

			for (auto& Item : Systematic_UrinaryOverloadData::Data)
			{
				if (Item.token == Op)
				{
					String funcName = Syb->FullName;
					ScopeHelper::GetApendedString(funcName, Item.CompilerName);

					auto& V = _Table.GetSymbolsWithName(funcName, SymbolType::Func);

					for (auto& Item : V)
					{
						if (Item->Type == SymbolType::Func)
						{
							auto funcInfo = Item->Get_Info<FuncInfo>();
							if (funcInfo->Pars.size() == 1)
							{
								bool r = CanBeImplicitConverted(TypeA, funcInfo->Pars[0]);
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
String SystematicAnalysis::ToString(const TypeSymbol& Type)
{
	String r;
	if (Type.Isimmutable())
	{
		r = "umut ";
	}


	if (Type._MoveData == MoveData::Moved)
	{
		r += "moved ";
	}

	if (Type._IsDynamic)
	{
		r += "dynamic<";
	}

	switch (Type._Type)
	{
	case TypesEnum::Var:r += "var";	break;
	case TypesEnum::Int_t:r += "Int_t";	break;
	case TypesEnum::uInt_t:r += "uInt_t";	break;
	case TypesEnum::sInt_t:r += "sInt_t";	break;


	case TypesEnum::uInt8:r += Uint8TypeName;	break;
	case TypesEnum::uInt16:r += Uint16TypeName;	break;
	case TypesEnum::uInt32:r += Uint32TypeName;	break;
	case TypesEnum::uInt64:r += Uint64TypeName;	break;


	case TypesEnum::sInt8:r += Sint8TypeName;	break;
	case TypesEnum::sInt16:r += Sint16TypeName;	break;
	case TypesEnum::sInt32:r += Sint32TypeName;	break;
	case TypesEnum::sInt64:r += Sint64TypeName;	break;

	case TypesEnum::uIntPtr:r += UintPtrTypeName;	break;
	case TypesEnum::sIntPtr:r += SintPtrTypeName;	break;

	case TypesEnum::Bool:r += boolTypeName;	break;
	case TypesEnum::Char:r += CharTypeName;	break;

	case TypesEnum::float32:r += float32TypeName;	break;
	case TypesEnum::float64:r += float64TypeName;	break;
	case TypesEnum::CustomType:
	{
		auto& Syb = *GetSymbol(Type._CustomTypeSymbol);
		if (Syb.Type == SymbolType::Func
			|| Syb.Type == SymbolType::GenericFunc)
		{
			r += ToString(Syb.VarType);
		}
		else if (Syb.Type == SymbolType::Type_alias)
		{
			r += ToString(Syb.VarType);
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
		else if (Syb.Type == SymbolType::Type_StaticArray)
		{
			StaticArrayInfo* Info = Syb.Get_Info<StaticArrayInfo>();
			r += ToString(Info->Type);
			r += "[/";
			r += std::to_string(Info->Count);
			r += "]";
		}
		else
		{
			r += Syb.FullName;
		}
	
	}	break;
	case TypesEnum::Void:
		r += "void";	break;
	case TypesEnum::Any:
		r += "[any]";	
		break;
	case TypesEnum::Null:
		r += "[badtype]";	break;
	default:
		throw std::exception("bad Type");
		break;
	}

	if (Type._IsDynamic)
	{
		r += ">";
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
			Symbol* SybV = GetSymbol(_ClassStack.top().Info);
			Out = SybV->VarType;
		}
		else
		{
			LogCantUseThisHere(V.Name.Token);
			Out.SetType(TypesEnum::Null);
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
				ConvertAndValidateType(Tnode, Type, NodeSyb_t::Any);

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
			else if (SybV->Type == SymbolType::Hard_Type_alias
				|| SybV->Type == SymbolType::Enum
				|| SybV->Type == SymbolType::Type_class)
			{
				Out.SetType(SybV->ID);
			}
			else if (SybV->Type == SymbolType::Trait_class)
			{
				if (!V.IsDynamic)
				{
					auto Token = V.Name.Token;
					TraitCantBeAlone(Token);
					Out.SetType(TypesEnum::Null);
				}
				else
				{
					Out.SetType(SybV->ID);
				}
			}
			else
			{
				LogWantedAType(V, SybV);
				Out.SetType(TypesEnum::Null);
			}
		}
	}break;
	case TokenType::internal_Constant_expression:
	{
		//note this can only happen in a generic substitution
		auto* node = V.node.get();
		if (_ConstantExpressionMap.HasValue(node))
		{
			auto& item = _ConstantExpressionMap.at(node);
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

	if (V.IsTypedMoved) { Out._MoveData = MoveData::Moved; }

	if (V.IsDynamic)
	{
		auto syb = GetSymbol(Out);
		bool IsTrait = false;
		if (syb)
		{
			IsTrait = syb->Type==SymbolType::Trait_class;
		}

		if (!IsTrait)
		{
			LogDynamicMustBeRrait(V, Out);
			Out.SetType(TypesEnum::Null);
		}
		Out._IsDynamic = true;
	}

	if (V.IsStackArray)
	{
		ExpressionNodeType* node = (ExpressionNodeType*)V.node.get();
		SymbolID id = (SymbolID)node;
		auto BaseTypeName = ToString(Out);
		auto FullName = CompilerGenerated("StaticArray_") + BaseTypeName + std::to_string(id);

		if (passtype == PassType::GetTypes)
		{
			return;
		}

		Symbol* Syb = GetSymbol(FullName,SymbolType::Null);
		if (Syb == nullptr)
		{
			SymbolID id = (SymbolID)node;
			Syb = &AddSybol(SymbolType::Type_StaticArray, FullName, FullName);
			_Table.AddSymbolID(*Syb, id);

			StaticArrayInfo* info = new StaticArrayInfo();


			info->Type = Out;
			info->Exnode = node;
			
			Syb->Info.reset(info);

		}
		StaticArrayInfo& Info = *Syb->Get_Info<StaticArrayInfo>();

		if (passtype == PassType::FixedTypes && node != nullptr && Info.IsCountInitialized == false)
		{
			TypeSymbol UIntType;
			UIntType.SetType(TypesEnum::uIntPtr);

			auto NodeV = node->Value.get();

			LookingForTypes.push(UIntType);

			OnExpressionTypeNode(NodeV, GetValueMode::Read);//check
			
			
			
			if (!CanBeImplicitConverted(LastExpressionType, UIntType, false))
			{
				LogCantCastImplicitTypes(LastLookedAtToken, LastExpressionType, UIntType, false);
				Out.SetType(TypesEnum::Null);
				return;
			}
			if (!CanEvaluateImplicitConversionConstant(LastExpressionType, UIntType))
			{
				Out.SetType(TypesEnum::Null);
				LogCantCastImplicitTypes_Constant(LastLookedAtToken, LastExpressionType, UIntType);
				return;
			}
			EvaluatedEx Ex;
			EvaluatedEx ex1 = MakeEx(LastExpressionType);
			Evaluate_t(ex1, NodeV);
			
			
			LookingForTypes.pop();

			EvaluateImplicitConversion(ex1, UIntType, Ex);


			void* V = Get_Object(Ex);

			Info.Count = *(size_t*)V;
			Info.IsCountInitialized = true;

		}
		Out.SetType(Syb->ID);
	}
}



void SystematicAnalysis::ConvertAndValidateType(const TypeNode& V, TypeSymbol& Out,NodeSyb_t Syb)
{
	Convert(V, Out);
	if (ValidateType(Out,V.Name.Token,Syb) == false)
	{
		Out.SetType(TypesEnum::Null);
	}
}
bool SystematicAnalysis::ValidateType(const TypeSymbol& V, const Token* Token,NodeSyb_t Syb)
{
	if (V._Type == TypesEnum::CustomType)
	{
		auto Syb = GetSymbol(V);
		if (Syb->Type == SymbolType::Type_StaticArray)
		{
			StaticArrayInfo* V = Syb->Get_Info<StaticArrayInfo>();

			if (!V->IsCountInitialized)
			{
				LogBeMoreSpecifiicWithStaticArrSize(Token, V->Type);
				return false;
			}
		}
	}


	if (V._MoveData == MoveData::Moved && Syb != NodeSyb_t::Parameter)
	{
		LogCantUseMoveTypeHere(Token);
	}

	return true;
}



TypeSymbol SystematicAnalysis::ConvertAndValidateType(const TypeNode& V,NodeSyb_t Syb)
{
	TypeSymbol r;
	ConvertAndValidateType(V, r,Syb);
	return r;
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

bool SystematicAnalysis::CanDoTypeToTrait(const TypeSymbol& TypeToCheck, const TypeSymbol& Type)
{
	if (Type._IsDynamic)
	{
		auto SybolB = GetSymbol(Type);
		TraitInfo* Info = SybolB->Get_Info<TraitInfo>();
		auto TypeSyb = GetSymbol(TypeToCheck);

		if (TypeSyb)
		{

			if (TypeSyb->Type == SymbolType::Type_class)
			{
				ClassInfo* ClassF = TypeSyb->Get_Info<ClassInfo>();

				auto& Indexo = ClassF->Get_InheritedTypesIndex(SybolB);

				if (Indexo.has_value())
				{
					return true;
				}
			}
		}
	}
	return false;
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
	
	
	if (CanDoTypeToTrait(TypeToCheck, Type))
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
	
	
	return false;
}
SystematicAnalysis::CastOverLoadWith_t  SystematicAnalysis::CanBeExplicitlyConverted(const TypeSymbol& TypeToCheck, const TypeSymbol& Type)
{
	if (CanBeImplicitConverted(TypeToCheck, Type)) { return { true }; }
	
	if (TypeToCheck._Type == TypesEnum::CustomType) 
	{
		Symbol& syb = *GetSymbol(TypeToCheck._CustomTypeSymbol);
		if (syb.Type == SymbolType::Hard_Type_alias)
		{
			if (AreTheSameWithOutimmutable(syb.VarType, Type))
			{
				return { true};
			}
		}

	}

	if (IsIntType(TypeToCheck) && IsIntType(TypeToCheck)) { return { true }; }

	auto Syb = GetSymbol(TypeToCheck);
	if (Syb)
	{
		if (Syb->Type == SymbolType::Type_class)
		{

			String funcName = Syb->FullName;
			ScopeHelper::GetApendedString(funcName, Overload_Cast_Func);

			auto& V = _Table.GetSymbolsWithName(funcName, SymbolType::Func);

			for (auto& Item : V)
			{
				if (Item->Type == SymbolType::Func)
				{
					auto funcInfo = Item->Get_Info<FuncInfo>();
					if (funcInfo->Pars.size() == 1)
					{
						bool r = AreTheSame(Type, funcInfo->Ret)
							&& CanBeImplicitConverted(TypeToCheck, funcInfo->Pars[0]);
						if (r)
						{
							return { r, Item };
						}

					}
				}
			}
		}
	}

	return { false };
}
Optional < FuncInfo*> SystematicAnalysis::GetAnExplicitlyConvertedFunc(const TypeSymbol& TypeToCheck)
{

	auto Syb = GetSymbol(TypeToCheck);
	if (Syb)
	{
		if (Syb->Type == SymbolType::Type_class)
		{

			String funcName = Syb->FullName;
			ScopeHelper::GetApendedString(funcName, Overload_Cast_Func);

			auto& V = _Table.GetSymbolsWithName(funcName, SymbolType::Func);

			for (auto& Item : V)
			{
				if (Item->Type == SymbolType::Func)
				{
					auto funcInfo = Item->Get_Info<FuncInfo>();
					if (funcInfo->Pars.size() == 1)
					{
						bool r = CanBeImplicitConverted(TypeToCheck, funcInfo->Pars[0]);
						if (r)
						{
							return  funcInfo;
						}

					}
				}
			}
		}
	}
	return {nullptr};
}
bool SystematicAnalysis::DoImplicitConversion(IRInstruction* Ex, const TypeSymbol ExType, const TypeSymbol& ToType)
{

	if (AreTheSame(ExType, ToType))
	{
		return true;
	}

	if (CanDoTypeToTrait(ExType,ToType))
	{
		auto IRType = ConvertToIR(ToType);
		IRInstruction* structV = LookingAtIRBlock->NewLoad(IRType);

		IRStruct* IRStructPtr = _Builder.GetSymbol(IRType._symbol)->Get_ExAs<IRStruct>();

		auto Member = LookingAtIRBlock->New_Member_Access(structV, IRStructPtr, 0);
		
		if (ExType.IsAddress())
		{
			LookingAtIRBlock->NewStore(Member, Ex);
		}
		else
		{
			LookingAtIRBlock->NewStore(Member, LookingAtIRBlock->NewLoadPtr(Ex));
		}

		auto Member2 = LookingAtIRBlock->New_Member_Access(structV, IRStructPtr, 1);

		auto IDVTable = _Builder.ToID(GetClassWithTraitVTableName(GetSymbol(ExType)->FullName, GetSymbol(ToType)->FullName));

		LookingAtIRBlock->NewStore(Member2,LookingAtIRBlock->NewLoadPtr(IDVTable));

		_LastExpressionField = structV;
		return true;
	}

	return false;
}
void SystematicAnalysis::DoExplicitlConversion(IRInstruction* Ex, const TypeSymbol ExType, const TypeSymbol& ToType, const CastExpressionNode_Data& Data)
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
		if (Data.FuncToCall)
		{
			FuncInfo* f = Data.FuncToCall->Get_Info<FuncInfo>();
			Get_FuncInfo v;
			v.Func = f;
			v.SymFunc = Data.FuncToCall;
			v.ThisPar = Get_FuncInfo::ThisPar_t::PushFromLast;
			
			_LastExpressionField = Ex;

			DoFuncCall(v, {}, {});
		}
		else
		{
			if (IsIntType(ToType))
			{
				
				if (IsUIntType(ExType))
				{
					switch (ToType._Type)
					{
					case TypesEnum::sInt8:_LastExpressionField = LookingAtIRBlock->New_UIntToSInt(LookingAtIRBlock->New_UIntToUInt8(Ex));break;
					case TypesEnum::sInt16:_LastExpressionField = LookingAtIRBlock->New_UIntToSInt(LookingAtIRBlock->New_UIntToUInt16(Ex));break;
					
					ULablesint32:
					case TypesEnum::sInt32:_LastExpressionField = LookingAtIRBlock->New_UIntToSInt(LookingAtIRBlock->New_UIntToUInt32(Ex));break;
					
					ULablesint64:
					case TypesEnum::sInt64:_LastExpressionField = LookingAtIRBlock->New_UIntToSInt(LookingAtIRBlock->New_UIntToUInt64(Ex));break;


					case TypesEnum::uInt8:_LastExpressionField = LookingAtIRBlock->New_UIntToUInt8(Ex); break;
					case TypesEnum::uInt16:_LastExpressionField = LookingAtIRBlock->New_UIntToUInt16(Ex); break;
					
					ULableuint32:
					case TypesEnum::uInt32:_LastExpressionField = LookingAtIRBlock->New_UIntToUInt32(Ex); break;
					
					ULableuint64:
					case TypesEnum::uInt64:_LastExpressionField = LookingAtIRBlock->New_UIntToUInt64(Ex); break;


					case TypesEnum::sIntPtr:
						if (_Settings->PtrSize == IntSizes::Int32)
						{
							goto ULablesint32;
						}
						else
						{
							goto ULablesint64;
						}
					case TypesEnum::uIntPtr:
						if (_Settings->PtrSize == IntSizes::Int32)
						{
							goto ULableuint32;
						}
						else
						{
							goto ULableuint64;
						}
					default:throw std::exception("bad path"); break;
					}
				}
				else if (IsSIntType(ExType))
				{
					switch (ToType._Type)
					{
					case TypesEnum::uInt8:_LastExpressionField = LookingAtIRBlock->New_SIntToUInt(LookingAtIRBlock->New_SIntToSInt8(Ex)); break;
					case TypesEnum::uInt16:_LastExpressionField = LookingAtIRBlock->New_SIntToUInt(LookingAtIRBlock->New_SIntToSInt16(Ex)); break;
					
					SLableuint32:
					case TypesEnum::uInt32:_LastExpressionField = LookingAtIRBlock->New_SIntToUInt(LookingAtIRBlock->New_SIntToSInt32(Ex)); break;
					
					SLableuint64:
					case TypesEnum::uInt64:_LastExpressionField = LookingAtIRBlock->New_SIntToUInt(LookingAtIRBlock->New_SIntToSInt64(Ex)); break;


					case TypesEnum::sInt8:_LastExpressionField = LookingAtIRBlock->New_SIntToSInt8(Ex); break;
					case TypesEnum::sInt16:_LastExpressionField = LookingAtIRBlock->New_SIntToSInt16(Ex); break;
				
					SLablesint32:
					case TypesEnum::sInt32:_LastExpressionField = LookingAtIRBlock->New_SIntToSInt32(Ex); break;
					
					SLablesint64:
					case TypesEnum::sInt64:_LastExpressionField = LookingAtIRBlock->New_SIntToSInt64(Ex); break;

					case TypesEnum::sIntPtr:
						if (_Settings->PtrSize == IntSizes::Int32)
						{
							goto SLablesint32;
						}
						else
						{
							goto SLablesint64;
						}
					case TypesEnum::uIntPtr:
						if (_Settings->PtrSize == IntSizes::Int32)
						{
							goto SLableuint32;
						}
						else
						{
							goto SLableuint64;
						}
					break;
					default:throw std::exception("bad path"); break;
					}
				}
				else
				{
					throw std::exception("bad path");
				}
			}
			else
			{
				throw std::exception("bad path");
			}
		}

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
inline bool SystematicAnalysis::IsVarableType(SymbolType type)
{
	switch (type)
	{
	case SymbolType::ParameterVarable:
	case SymbolType::StackVarable:
	case SymbolType::StaticVarable:
	case SymbolType::ThreadVarable:
		return true;
	default:
		return false;
		break;
	}
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
bool SystematicAnalysis::IsStaticArray(const TypeSymbol& TypeToCheck)
{
	if (TypeToCheck.IsAn(TypesEnum::CustomType))
	{
		auto Syb = GetSymbol(TypeToCheck);
		return Syb->Type == SymbolType::Type_StaticArray;
	}


	return false;
}


bool SystematicAnalysis::IsimmutableRulesfollowed(const TypeSymbol& TypeToCheck, const TypeSymbol& Type)
{

	bool Chechimm = TypeToCheck.Isimmutable();
	bool CmpTypeimm = Type.Isimmutable();


	if (Chechimm == CmpTypeimm || CmpTypeimm == true)
	{
		return true;
	}

	return false;
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
	auto dropfunc = GetSymbol(TypeDestructorFuncName, SymbolType::Func);
	if (dropfunc == nullptr)
	{
		auto Sym = GetSymbol(TypeToCheck);

		if (Sym && Sym->Type == SymbolType::Type_class)
		{
			auto Classinfo = Sym->Get_Info<ClassInfo>();
			Update_ClassSym_ToFixedTypes(Sym);

			return Classinfo->_WillHaveFielddeInit;
		}
	}
	return dropfunc;
}
void  SystematicAnalysis::Update_ClassSym_ToFixedTypes(Symbol* Sym)
{
	if (Sym->PassState == PassType::GetTypes)
	{
		OnClassNode(*(ClassNode*)Sym->NodePtr);
	}
}
void  SystematicAnalysis::Update_FuncSym_ToFixedTypes(Symbol* Sym)
{
	if (Sym->PassState == PassType::GetTypes)
	{
		OnFuncNode(*(FuncNode*)Sym->NodePtr);
	}
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
			if (Info->VariantData.has_value()) 
			{
				auto tagsize = GetSize(Info->Basetype, OutSize);
				auto& Item = Info->VariantData.value();
				
				
				if (!Item.VariantSize.has_value())
				{
					auto MaxSize = 0;
					
					for (auto& Item2 : Item.Variants)
					{
						auto ItemSize = 0;
						for (auto& Item3 : Item2.Types)
						{
							UAddress tep = 0;
							GetSize(Item3, tep);
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
				return GetSize(Info->Basetype, OutSize);
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
			bool V = GetSize(Info->Type,OutSize);
			OutSize *= Info->Count;
			return V;
		}
		else if (V.Type == SymbolType::Trait_class && Type._IsDynamic)
		{
			TypeSymbol pointer(TypesEnum::Bool);
			pointer.SetAsAddress();
			return GetSize(pointer).value() * 2;
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
				OnExpressionTypeNode(Item.get(),GetValueMode::Read);\
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
		if (SymbolsV && SymbolsV->Type == SymbolType::Type_alias)
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

		if (Func.SymFunc)
		{
			if (Func.SymFunc->Type == SymbolType::Enum_Field)
			{
				auto ScopedName = GetScopedNameAsString(Name);
				String EnumClassFullName = ScopedName;
				ScopeHelper::ReMoveScope(EnumClassFullName);



				auto EnumSymbol = GetSymbol(EnumClassFullName, SymbolType::Enum);
				if (EnumSymbol)
				{
					EnumInfo* EnumSybInfo = EnumSymbol->Get_Info<EnumInfo>();
					auto& VariantData = EnumSybInfo->VariantData.value();
					size_t EnumIndex = EnumSybInfo->GetFieldIndex(ScopeHelper::GetNameFromFullName(ScopedName)).value();

					EnumFieldInfo& EnumFieldinfo = EnumSybInfo->Fields[EnumIndex];
					EnumVariantFeild& EnumVariantFeildData = VariantData.Variants[EnumIndex];

					if (Func.ThisPar == Get_FuncInfo::ThisPar_t::NoThisPar_GetValue_EnumVariant)
					{
						IRInstruction* ThisObj = nullptr;
						IRInstruction* BoolObj = nullptr;
						{
							TypeSymbol Par =EnumSymbol->ID;
							Par._IsAddress = true;

							auto& Item = Pars._Nodes[0];

							LookingForTypes.push(Par);

							OnExpressionTypeNode(Item.get(), GetValueMode::Read);
							DoImplicitConversion(_LastExpressionField, LastExpressionType, Par);

							ThisObj = _LastExpressionField;

							LookingForTypes.pop();
						}

					
						auto Key = LoadEvaluatedEx(EnumFieldinfo.Ex, EnumSybInfo->Basetype);


						
						auto Member = LookingAtIRBlock->New_Member_Dereference(ThisObj, ConvertToIR(EnumSymbol->ID), 0);

						auto ObjUnion = LookingAtIRBlock->New_Member_Dereference(ThisObj, ConvertToIR(EnumSymbol->ID), 1);


						if (IsPrimitiveNotIncludingPointers(EnumSybInfo->Basetype))
						{
							BoolObj = LookingAtIRBlock->NewC_Equalto(Member, Key);
						}
						else
						{
							throw std::exception("not added");
						}


						IRStruct* UnionStruct = nullptr;
						{
							String UnionName = GetEnumVariantUnionName(EnumSybInfo->FullName);
							IRidentifierID UnionID = _Builder.ToID(UnionName);
							UnionStruct = _Builder.GetSymbol(UnionID)->Get_ExAs<IRStruct>();
						}
						
						IRStruct* VStruct = nullptr;
						if (EnumVariantFeildData.ClassSymbol.has_value())
						{
							TypeSymbol VSyb = TypeSymbol(EnumVariantFeildData.ClassSymbol.value());
							VStruct = _Builder.GetSymbol(ConvertToIR(VSyb)._symbol)->Get_ExAs<IRStruct>();
						}

						for (size_t i = 1; i < Pars._Nodes.size(); i++)
						{
							auto& Item = Pars._Nodes[i];
							OutExpression* outEx = OutExpression::As(Item.get());
							SymbolID ID = (SymbolID)outEx;
						

							auto& Syb = _Table.GetSymbol(ID);
							IRInstruction* ItemMember;

							if (EnumVariantFeildData.ClassSymbol.has_value())
							{
								auto Struct = LookingAtIRBlock->New_Member_Access(ObjUnion, UnionStruct, EnumIndex);
								ItemMember = LookingAtIRBlock->New_Member_Access(Struct, VStruct, i - 1);
							}
							else
							{
								ItemMember = LookingAtIRBlock->New_Member_Access(ObjUnion, UnionStruct, EnumIndex);

							}

							Syb.IR_Ins = ItemMember;
						}

						LastExpressionType = TypeSymbol(TypesEnum::Bool);
						_LastExpressionField = BoolObj;
					}
					else
					{
						auto ID = _Builder.ToID(EnumSybInfo->FullName);

						auto Key = LoadEvaluatedEx(EnumFieldinfo.Ex, EnumSybInfo->Basetype);

						auto VariantClass = LookingAtIRBlock->NewLoad(IRType(ID));
						IRStruct* V = _Builder.GetSymbol(ID)->Get_ExAs<IRStruct>();
						auto Member = LookingAtIRBlock->New_Member_Access(VariantClass, V, 0);
						LookingAtIRBlock->NewStore(Member, Key);

						if (EnumVariantFeildData.Types.size()) {
							auto UnionMember = LookingAtIRBlock->New_Member_Access(VariantClass, V, 1);


							String UnionName = GetEnumVariantUnionName(EnumSybInfo->FullName);
							IRidentifierID UnionID = _Builder.ToID(UnionName);

							auto ObjectMember = LookingAtIRBlock->New_Member_Access(UnionMember, _Builder.GetSymbol(UnionID)->Get_ExAs<IRStruct>(), EnumIndex);

							IRStruct* VStruct = nullptr;
							if (EnumVariantFeildData.ClassSymbol.has_value())
							{
								TypeSymbol VSyb = TypeSymbol(EnumVariantFeildData.ClassSymbol.value());
								VStruct = _Builder.GetSymbol(ConvertToIR(VSyb)._symbol)->Get_ExAs<IRStruct>();
							}
							//
							for (size_t i = 0; i < Pars._Nodes.size(); i++)
							{
								auto& Item = Pars._Nodes[i];
								auto& FuncParInfo = EnumVariantFeildData.Types[i];



								LookingForTypes.push(FuncParInfo);

								OnExpressionTypeNode(Item.get(), GetValueMode::Read);
								DoImplicitConversion(_LastExpressionField, LastExpressionType, FuncParInfo);

								auto ParEx = _LastExpressionField;

								if (EnumVariantFeildData.ClassSymbol.has_value())
								{
									auto VMember = LookingAtIRBlock->New_Member_Access(ObjectMember, VStruct, i);
									LookingAtIRBlock->NewStore(VMember, ParEx);
								}
								else
								{
									LookingAtIRBlock->NewStore(ObjectMember, ParEx);
								}
								LookingForTypes.pop();
							}
							//


						}
						LastExpressionType = EnumSymbol->VarType;
						_LastExpressionField = VariantClass;
					}
				}
			}
		}

		return;
	}

	IRInstruction* PushIRStackRet = false;
	bool AutoPushThis = Func.ThisPar != Get_FuncInfo::ThisPar_t::NoThisPar;

	Vector< IRInstruction*> IRParsList;

	if (AutoPushThis)
	{
		if (Func.ThisPar == Get_FuncInfo::ThisPar_t::FullScopedName)
		{

			GetMemberTypeSymbolFromVar_t V;
			GetMemberTypeSymbolFromVar(0, Name.ScopedName.size(), Name, V);

			IRParsList.push_back(BuildMember_AsPointer(V));
		}
		else
			if (Func.ThisPar == Get_FuncInfo::ThisPar_t::PushFromScopedName)
			{

				GetMemberTypeSymbolFromVar_t V;
				GetMemberTypeSymbolFromVar(0, Name.ScopedName.size() - 1, Name, V);

				IRParsList.push_back(BuildMember_AsPointer(V));
			}
			else if (Func.ThisPar == Get_FuncInfo::ThisPar_t::PushFromLast)
			{
				IRParsList.push_back(_LastExpressionField);
			}
			else if (Func.ThisPar == Get_FuncInfo::ThisPar_t::OnIRlocationStack)
			{

				bool UseedTopIR = IRlocations.size() != 0 && IRlocations.top().UsedlocationIR == false;
				if (!UseedTopIR)
				{
					IRLocation_Cotr tep;
					tep.UsedlocationIR = false;

					auto Type = Func.Func->Pars[0];
					if (Type.IsAddress())
					{
						Type._IsAddress = false;
					}

					PushIRStackRet = tep.Value = LookingAtIRBlock->NewLoad(ConvertToIR(Type));
					IRlocations.push(tep);
				}
				else
				{
					PushIRStackRet = IRlocations.top().Value;

				}



				{
					auto Defe = LookingAtIRBlock->NewLoadPtr(IRlocations.top().Value);
					IRlocations.top().UsedlocationIR = true;
					IRParsList.push_back(Defe);
				}

				if (!UseedTopIR)
				{
					IRlocations.pop();
				}

			}
			else if (Func.ThisPar == Get_FuncInfo::ThisPar_t::OnIRlocationStackNonedef)
			{
				bool UseedTopIR = IRlocations.size() != 0 && IRlocations.top().UsedlocationIR == false;
				if (!UseedTopIR)
				{
					IRLocation_Cotr tep;
					tep.UsedlocationIR = false;

					auto Type = Func.Func->Pars[0];
					if (Type.IsAddress())
					{
						Type._IsAddress = false;
					}

					PushIRStackRet = tep.Value = LookingAtIRBlock->NewLoad(ConvertToIR(Type));
					IRlocations.push(tep);
				}

				{
					IRParsList.push_back(IRlocations.top().Value);
					IRlocations.top().UsedlocationIR = true;
				}

				if (!UseedTopIR)
				{
					IRlocations.pop();
				}
			}
			else if (Func.ThisPar == Get_FuncInfo::ThisPar_t::PushWasCalled)
			{
				throw std::exception("not added");//just add IRPar list
			}
			else if (Func.ThisPar == Get_FuncInfo::ThisPar_t::AutoPushThis)
			{
				IRParsList.push_back(LookingAtIRBlock->NewLoad(&LookingAtIRFunc->Pars.front()));
			}
			else if (Func.ThisPar == Get_FuncInfo::ThisPar_t::PushFromScopedNameDynamicTrait)
			{

				GetMemberTypeSymbolFromVar_t V;
				GetMemberTypeSymbolFromVar(0, Name.ScopedName.size() - 1, Name, V);

				IRParsList.push_back(BuildMember_AsPointer(V));
			}
			else
			{
				throw std::exception("not added");
			}
	}


	for (size_t i = 0; i < Pars._Nodes.size(); i++)
	{
		auto& Item = Pars._Nodes[i];
		auto& FuncParInfo = Func.SymFunc->Type == SymbolType::Func
			? Func.Func->Pars[i + (AutoPushThis ? 1 : 0)]
			: ((FuncPtrInfo*)Func.Func)->Pars[i + (AutoPushThis ? 1 : 0)];



		LookingForTypes.push(FuncParInfo);

		OnExpressionTypeNode(Item.get(), GetValueMode::Read);
		DoImplicitConversion(_LastExpressionField, LastExpressionType, FuncParInfo);

		IRParsList.push_back(_LastExpressionField);


		LookingForTypes.pop();
	}
	auto Syb = Func.SymFunc;

	AddDependencyToCurrentFile(Syb);

	//

	if (Func.ThisPar != Get_FuncInfo::ThisPar_t::PushFromScopedNameDynamicTrait)
	{
		for (auto& Item : IRParsList)
		{
			LookingAtIRBlock->NewPushParameter(Item);
		}
	}
	//
	if (Func.ThisPar == Get_FuncInfo::ThisPar_t::PushFromScopedNameDynamicTrait) 
	{
		GetMemberTypeSymbolFromVar_t V;
		GetMemberTypeSymbolFromVar(0, Name.ScopedName.size() - 1, Name, V);

		auto TraitType = V.Type;
		auto TraitSyb = GetSymbol(TraitType);
		auto TraitAsIR = ConvertToIR(TraitType);

		auto VPtrMember = LookingAtIRBlock->New_Member_Dereference(IRParsList.front(), TraitAsIR,1);


		TraitInfo* Info = TraitSyb->Get_Info<TraitInfo>();
		size_t FuncIndex = Info->GetIndex_Func(Func.SymFunc).value();

		auto PtrCall = LookingAtIRBlock->New_Member_Dereference(VPtrMember,IRType(_Builder.ToID(GetTraitVStructTableName(TraitSyb->FullName))), FuncIndex);



		IRParsList.front() = LookingAtIRBlock->New_Member_Dereference(IRParsList.front(), TraitAsIR, 0);

		//
		for (auto& Item : IRParsList)
		{
			LookingAtIRBlock->NewPushParameter(Item);
		}
		_LastExpressionField = LookingAtIRBlock->NewCallFuncPtr(PtrCall);
		//
	}
	else
	if (Syb->Type== SymbolType::Func)
	{
		_LastExpressionField = LookingAtIRBlock->NewCall(GetIRID(Func.Func));
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

	{
		auto Tep = _LastExpressionField;
		for (size_t i = 0; i < IRParsList.size(); i++)
		{
			auto& Item = IRParsList[i];
			auto& ItemType = Func.SymFunc->Type == SymbolType::Func ? Func.Func->Pars[i]
				: ((FuncPtrInfo*)Func.Func)->Pars[i];

			if (ItemType._IsAddress == false && HasDestructor(ItemType))
			{
				ObjectToDrop obj;
				obj.DropType = ObjectToDropType::IRInstructionNoMod;
				obj.Type = ItemType;
				obj._Object = Item;



				if (!obj.Type.IsAddress())//will not work if Destructor doesn't taken in pointer
				{
					obj._Object = LookingAtIRBlock->NewLoadPtr(obj._Object);
				}

				obj.Type._IsAddress = false;
				DoDestructorCall(obj);
			}

		}
		_LastExpressionField = Tep;
	}

	if (LookingForTypes.size() && Get_LookingForType().IsnotAn(TypesEnum::Void) && PushIRStackRet)//constructors are just void funcions so just set last as the input this
	{

		_LastExpressionField = PushIRStackRet;
		LastExpressionType = Func.Func->Pars.front();
		
		
		if (Get_LookingForType().IsAddress())
		{
			_LastExpressionField = LookingAtIRBlock->NewLoadPtr(_LastExpressionField);
		}
		else
		{
			LastExpressionType._IsAddress = false;
		}
		

	}
	else
	{
		LastExpressionType = Func.Func->Ret;
	}
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
				case ObjectToDropType::Operator:
					FuncInfo.ThisPar = Get_FuncInfo::ThisPar_t::OnIRlocationStackNonedef;

					if (Object._Operator.Type != IROperatorType::IRInstruction)
					{
						throw std::exception("not added");
					}

					IRlocations.push({LookingAtIRBlock->NewLoad(Object._Operator.Pointer), false });
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

				if (tep_._IsDynamic)
				{
					tep_._IsDynamic = false;
					ThisParType = Get_FuncInfo::ThisPar_t::PushFromScopedNameDynamicTrait;
				}
				else
				{
					ThisParType = Get_FuncInfo::ThisPar_t::PushFromScopedName;
				}

				ScopedName = ToString(tep_) + ScopeHelper::_ScopeSep;
				ScopedName += Name.ScopedName.back().token->Value._String;
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

			
			
			{
				if (ScopedName == Uint8TypeName){LastExpressionType = TypesEnum::uInt8;}
				else if (ScopedName == Uint16TypeName) { LastExpressionType = TypesEnum::uInt16; }
				else if (ScopedName == Uint32TypeName) { LastExpressionType = TypesEnum::uInt32; }
				else if (ScopedName == Uint64TypeName) { LastExpressionType = TypesEnum::uInt64; }
				 
				else if (ScopedName == Sint8TypeName) { LastExpressionType = TypesEnum::sInt8; }
				else if (ScopedName == Sint16TypeName) { LastExpressionType = TypesEnum::sInt16; }
				else if (ScopedName == Sint32TypeName) { LastExpressionType = TypesEnum::sInt32; }
				else if (ScopedName == Sint64TypeName) { LastExpressionType = TypesEnum::sInt64; }

				else if (ScopedName == SintPtrTypeName) { LastExpressionType = TypesEnum::sIntPtr; }
				else if (ScopedName == UintPtrTypeName) { LastExpressionType = TypesEnum::uIntPtr; }

				else if (ScopedName == boolTypeName) { LastExpressionType = TypesEnum::Bool; }
				else if (ScopedName == CharTypeName) { LastExpressionType = TypesEnum::Char; }

				else if (ScopedName == float32TypeName) { LastExpressionType = TypesEnum::float32; }
				else if (ScopedName == float64TypeName) { LastExpressionType = TypesEnum::float64; }

				else
				{
					throw std::exception("bad path");
				}
			}

			if (Pars._Nodes.size() == 1)
			{
				auto FuncType = LastExpressionType;
				OnExpressionTypeNode(Pars._Nodes[0].get(), GetValueMode::Read);

				if (CanBeImplicitConverted(LastExpressionType, FuncType, true))
				{
					LogCantCastImplicitTypes(LastLookedAtToken, LastExpressionType, FuncType,true);
				}
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


	bool HasOutPar = false;
	for (size_t i = 0; i < Pars._Nodes.size(); i++)
	{
		auto& Item = Pars._Nodes[i];
		auto& ValueItem = ValueTypes.emplace_back();

		LookingForTypes.push(NullSymbol);


		if (Item->Get_Type() == NodeType::OutExpression)
		{
			HasOutPar = true;
			ValueItem = TypeSymbol(TypesEnum::Null);
		}
		else
		{
			OnExpressionTypeNode(Item.get(), GetValueMode::Read);
			ValueItem = LastExpressionType;
		}

		LookingForTypes.pop();
		
		
		
	}

	//
	{
		if (HasOutPar)
		{
			bool IsControlFlow = false;
			if (NodeTypeStack.size() > 2) 
			{
				size_t Index = NodeTypeStack.size() - 1;
				Index--;
				auto& Last = NodeTypeStack[Index];
				if (Last == NodeType::IfNode || Last == NodeType::WhileNode || Last == NodeType::DoNode)
				{
					IsControlFlow = true;
				}
			}
		
			if (!IsControlFlow)
			{
				auto Token = Name.ScopedName.back().token; 
				LogOutCanOnlyBeInControlFlow(Token); 
				return { };
			}
		}
	}
	//


	auto Symbols = _Table.GetSymbolsWithName(ScopedName, SymbolType::Any);
	StartSymbolsLoop:

	Vector<Get_FuncInfo> OkFuncions;
	for (auto& Item : Symbols)
	{

		if (Item->Type == SymbolType::Func)
		{

			FuncInfo* Info = Item->Get_Info<FuncInfo>();

			IsCompatiblePar CMPPar;
			CMPPar.SetAsFuncInfo(Item);

			if (!IsCompatible(CMPPar, ValueTypes, _ThisTypeIsNotNull, Name.ScopedName.back().token))
			{
				continue;
			}

			{
				r = Info;
				FuncSymbol = Item;
				T = SymbolType::FuncCall;
				OkFuncions.push_back({ ThisParType,r,FuncSymbol });
			}
		}
		else if (Item->Type == SymbolType::GenericFunc)//TODO try for other befor this
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

			auto ConstructorSymbols = _Table.GetSymbolsWithName(Scope, SymbolType::Any);


			for (auto& Item2 : ConstructorSymbols)
			{
				if (Item2->Type == SymbolType::Func)
				{
					FuncInfo* Info = Item2->Get_Info<FuncInfo>();
					bool PushThisPar = Info->IsObjectCall();


					if (PushThisPar)
					{
						TypeSymbol V;
						V.SetType(Item->ID);
						V.SetAsAddress();
						ValueTypes.insert(ValueTypes.begin(), V);
					}

					IsCompatiblePar CMPPar;
					CMPPar.SetAsFuncInfo(Item2);

					bool Compatible = IsCompatible(CMPPar, ValueTypes, _ThisTypeIsNotNull, Name.ScopedName.back().token);
					
					if (PushThisPar)
					{
						ValueTypes.erase(ValueTypes.begin());
					}

					if (!Compatible)
					{
						continue;
					}

					{
						r = Info;
						FuncSymbol = Item2;
						T = SymbolType::FuncCall;
						OkFuncions.push_back({ PushThisPar ? Get_FuncInfo::ThisPar_t::OnIRlocationStack : ThisParType,r,FuncSymbol });
					}
				}
			}
		}
		else if (IsVarableType(Item->Type))
		{
			Symbol* Type = GetSymbol(Item->VarType);
			if (Type) 
			{

				if (Type->Type == SymbolType::Func_ptr || Type->Type == SymbolType::Hard_Func_ptr)
				{
					FuncPtrInfo* Info = Type->Get_Info<FuncPtrInfo>();//must be the same as Item->Type == SymbolType::Func

					IsCompatiblePar CMPPar;
					CMPPar.SetAsFuncPtrInfo(Type);

					if (!IsCompatible(CMPPar, ValueTypes, _ThisTypeIsNotNull, Name.ScopedName.back().token))
					{
						continue;
					}

					{
						r = (FuncInfo*)Info;
						FuncSymbol = Item;
						T = SymbolType::FuncCall;
						OkFuncions.push_back({ ThisParType,r,FuncSymbol });
					}
				}
				else if (Type->Type == SymbolType::Type_class)
				{
					String Scope = Type->FullName;
					ScopeHelper::GetApendedString(Scope, Overload_Invoke_Func);
					auto ConstructorSymbols = _Table.GetSymbolsWithName(Scope, SymbolType::Any);


					for (auto& Item2 : ConstructorSymbols)
					{
						if (Item2->Type == SymbolType::Func)
						{
							FuncInfo* Info = Item2->Get_Info<FuncInfo>();
							bool PushThisPar = Info->IsObjectCall();


							if (PushThisPar)
							{
								TypeSymbol V;
								V.SetType(Type->ID);
								V.SetAsAddress();
								ValueTypes.insert(ValueTypes.begin(), V);
							}

							IsCompatiblePar CMPPar;
							CMPPar.SetAsFuncInfo(Item2);

							bool Compatible = IsCompatible(CMPPar, ValueTypes, _ThisTypeIsNotNull, Name.ScopedName.back().token);

							if (PushThisPar)
							{
								ValueTypes.erase(ValueTypes.begin());
							}

							if (!Compatible)
							{
								continue;
							}

							{
								r = Info;
								FuncSymbol = Item2;
								T = SymbolType::FuncCall;
								OkFuncions.push_back({ Get_FuncInfo::ThisPar_t::FullScopedName,r,FuncSymbol });
							}
						}
					}
				}
			}
		}
		else if (Item->Type == SymbolType::Enum_Field)
		{
			String EnumClassFullName = ScopedName;
			ScopeHelper::ReMoveScope(EnumClassFullName);

			auto EnumSymbol = GetSymbol(EnumClassFullName, SymbolType::Enum);
			if (EnumSymbol)
			{
				if (EnumSymbol->Type == SymbolType::Enum)
				{
					EnumInfo* Enuminfo = EnumSymbol->Get_Info<EnumInfo>();
					if (Enuminfo->VariantData.has_value())
					{
						size_t FeildIndex = Enuminfo->GetFieldIndex(ScopeHelper::GetNameFromFullName(Item->FullName)).value();

						return GetEnumVariantFunc(EnumSymbol, FeildIndex,Item, Pars,Name.ScopedName.back().token,ValueTypes);
					}
				}
			}
		}
		ContinueOutloop:continue;
	}
	if (OkFuncions.size() == 0)
	{
		bool MayBeAutoThisFuncCall = Name.ScopedName.size() == 1 && IsInThisFuncCall();


		if (ThisParType == Get_FuncInfo::ThisPar_t::NoThisPar && MayBeAutoThisFuncCall && AutoThisCall == false)
		{
			AutoThisCall = true;

			ValueTypes.insert(ValueTypes.begin(), *_FuncStack.back().Pointer->GetObjectForCall());
			ThisParType = Get_FuncInfo::ThisPar_t::AutoPushThis;
			goto StartSymbolsLoop;
		}
		else 
		{
			LogCantFindFuncError(Name.ScopedName.back().token, ScopedName, {}, ValueTypes, RetType);
			return { };
		}
	}
	else
	{
		
		


		Optional<int> MinScore;
		Get_FuncInfo* Ret =nullptr;
		for (auto& Item : OkFuncions)
		{
			IsCompatiblePar CMPPar;
			if (Item.SymFunc->Type == SymbolType::Func) 
			{
				CMPPar.SetAsFuncInfo(Item.SymFunc);
			}
			else if (IsVarableType(Item.SymFunc->Type))
			{
				Symbol* Type = GetSymbol(Item.SymFunc->VarType);
				if (Type && (Type->Type == SymbolType::Func_ptr || Type->Type == SymbolType::Hard_Func_ptr))
				{
					CMPPar.SetAsFuncPtrInfo(Type);
				}
				else
				{
					throw std::exception("not added");
				}	
			}
			else
			{
				throw std::exception("not added");
			}


			int Score = GetCompatibleScore(CMPPar, ValueTypes);
			if (!MinScore.has_value() ||  Score > MinScore.value())
			{
				MinScore = Score;
				Ret = &Item;
			}
		}
		if (Ret == nullptr) {
			throw std::exception("bad path");
		}
		
		return *Ret;
		
		
	}
	return { };
}

SystematicAnalysis::Get_FuncInfo SystematicAnalysis::GetEnumVariantFunc(Symbol* EnumSyb, size_t FeildIndex, Symbol* EnumFieldSyb,const ValueParametersNode& Pars, const Token* Token, const Vector<TypeSymbol>& ValueTypes)
{
	EnumInfo* Enuminfo = EnumSyb->Get_Info<EnumInfo>();
	auto& Feild = Enuminfo->Fields[FeildIndex];
	auto& Feild_Variant = Enuminfo->VariantData.value().Variants[FeildIndex];


	bool HasOut = false;
	for (size_t i = 0; i < Pars._Nodes.size(); i++)
	{
		auto& Item = Pars._Nodes[i];
		

		if (Item->Get_Type() == NodeType::OutExpression)
		{
			HasOut = true;
			break;
		}
	}


	if (HasOut)
	{

		if (Feild_Variant.Types.size() + 1 != Pars._Nodes.size())
		{
			String FullName = Enuminfo->FullName;
			ScopeHelper::GetApendedString(FullName, Feild.Name);
			LogCanIncorrectParCount(Token, FullName, Pars._Nodes.size(), Feild_Variant.Types.size());

			LastExpressionType = TypeSymbol(EnumSyb->ID);
			return {};
		}


		for (size_t i = 0; i < Pars._Nodes.size(); i++)
		{
			auto& Item = Pars._Nodes[i];


			if (i == 0)
			{
				auto& ExItemType = ValueTypes[i];

				TypeSymbol Vthis = EnumSyb->ID;
				if (!CanBeImplicitConverted(ExItemType, Vthis))
				{
					LogCantCastImplicitTypes(Token, ExItemType, Vthis, true);
				}
			}
			else
			{
				auto& ItemVariant = Feild_Variant.Types[i - 1];
				if (Item->Get_Type() != NodeType::OutExpression)
				{
					LogParamterMustBeAnOutExpression(Token, i);
				}
				else
				{
					OutExpression* Ex = OutExpression::As(Item.get());
					auto Str = Ex->_Name.Token->Value._String;

					String FullName = this->_Table._Scope.ThisScope;
					ScopeHelper::GetApendedString(FullName, Str);

					auto Syb = &AddSybol(SymbolType::StackVarable, (String)Str, FullName);

					this->LookingForTypes.push(ItemVariant);
					Syb->VarType = ConvertAndValidateType(Ex->_Type, NodeSyb_t::Varable);
					this->LookingForTypes.pop();

					ExDeclareVariableTypeCheck(Syb->VarType, ItemVariant, Ex->_Name.Token);
					_Table.AddSymbolID(*Syb, (SymbolID)Item.get());
				}

			}




		}
		Get_FuncInfo r;
		r.ThisPar = Get_FuncInfo::ThisPar_t::NoThisPar_GetValue_EnumVariant;
		r.SymFunc = EnumFieldSyb;
		r.Func = nullptr;

		LastExpressionType = TypeSymbol(TypesEnum::Bool);
		return r;
	}
	else 
	{
		if (Feild_Variant.Types.size() == 1)
		{
			TypeSymbol VoidType(TypesEnum::Void);

			if (AreTheSame(VoidType, Feild_Variant.Types.front()))
			{
				if (Pars._Nodes.size() != 0)
				{
					String FullName = Enuminfo->FullName;
					ScopeHelper::GetApendedString(FullName, Feild.Name);
					LogCanIncorrectParCount(Token, FullName, Pars._Nodes.size(), 0);

					LastExpressionType = TypeSymbol(EnumSyb->ID);
					return {};
				}
				else
				{
					Get_FuncInfo r;
					r.ThisPar = Get_FuncInfo::ThisPar_t::NoThisPar;
					r.SymFunc = EnumFieldSyb;
					r.Func = nullptr;

					LastExpressionType = TypeSymbol(EnumSyb->ID);
					return r;
				}
			}
		}

		if (Feild_Variant.Types.size() != Pars._Nodes.size())
		{
			String FullName = Enuminfo->FullName;
			ScopeHelper::GetApendedString(FullName, Feild.Name);
			LogCanIncorrectParCount(Token, FullName, Pars._Nodes.size(), Feild_Variant.Types.size());

			LastExpressionType = TypeSymbol(EnumSyb->ID);
			return {};
		}

		for (size_t i = 0; i < Feild_Variant.Types.size(); i++)
		{
			auto& Item = Feild_Variant.Types[i];
			auto& ExItemType = ValueTypes[i];

			if (!CanBeImplicitConverted(ExItemType, Item))
			{
				LogCantCastImplicitTypes(Token, ExItemType, Item, true);
			}
		}

		Get_FuncInfo r;
		r.ThisPar = Get_FuncInfo::ThisPar_t::NoThisPar;
		r.SymFunc = EnumFieldSyb;
		r.Func = nullptr;

		LastExpressionType = TypeSymbol(EnumSyb->ID);
		return r;
	}
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

bool SystematicAnalysis::IsCompatible(const IsCompatiblePar& FuncPar,const Vector<TypeSymbol>& ValueTypes, bool _ThisTypeIsNotNull, const Token* Token)
{

	if (FuncPar.Pars->size() != ValueTypes.size())
	{
		return false;
	}


	//
	if ((PassType_t)FuncPar.Item->PassState < (PassType_t)passtype)
	{
		if (FuncPar.Item->Type != SymbolType::Func)
		{
			throw std::exception("not added");
		}

		FuncInfo* Info = FuncPar.Item->Get_Info<FuncInfo>();
		if (!IsDependencies(Info))
		{
			auto OldPass = passtype;
			auto OldScope = _Table._Scope.ThisScope;
			_Table._Scope.ThisScope = Info->FullName;
			ScopeHelper::ReMoveScope(_Table._Scope.ThisScope);

			_RetLoopStack.push_back(Info);

			OnFuncNode(*(FuncNode*)FuncPar.Item->NodePtr);

			_RetLoopStack.pop_back();

			_Table._Scope.ThisScope = OldScope;
		}
		else
		{
			auto V = GetDependencies(Info);
			if (V->IsOnRetStatemnt)
			{
				LogFuncDependencyCycle(Token, Info);
				Info->Ret.SetType(TypesEnum::Null);//to stop err spam
			}
			return false;
		}
	}
	//

	for (size_t i = _ThisTypeIsNotNull ? 1 : 0; i < FuncPar.Pars->size(); i++)
	{
		auto& Item = (*FuncPar.Pars)[i];
		auto& Item2 = ValueTypes[i];

		if (!CanBeImplicitConverted(Item2, Item, true))
		{
			return false;
		}
	}
	return true;
}

int SystematicAnalysis::GetCompatibleScore(const TypeSymbol& ParFunc, const TypeSymbol& Value)
{
	int r = 0;
	
	if (AreTheSameWithOutimmutable(ParFunc,Value))
	{
		r += 5;
	}
	else if (CanBeImplicitConverted(ParFunc, Value, false))
	{
		r += 3;
	}
	else if (CanBeExplicitlyConverted(ParFunc,Value).HasValue)
	{
		r += 1;
	}


	if (ParFunc.IsAddress() == Value.IsAddress())
	{
		r += 1;
	}
	if (ParFunc.Isimmutable() == Value.Isimmutable())
	{
		r += 1;
	}


	return r;
}

int SystematicAnalysis::GetCompatibleScore(const IsCompatiblePar& Func, const Vector<TypeSymbol>& ValueTypes)
{

	int r = 0;
	for (size_t i = 0; i < ValueTypes.size(); i++)
	{
		r += GetCompatibleScore((*Func.Pars)[i], ValueTypes[i]);
	}


	return ValueTypes.size() ? r / ValueTypes.size() : r;
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

IRInstruction* SystematicAnalysis::Build_Add_uIntPtr(IRInstruction* field, IRInstruction* field2)
{
	return LookingAtIRBlock->NewAdd(field, field2);
}

IRInstruction* SystematicAnalysis::Build_Sub_uIntPtr(IRInstruction* field, IRInstruction* field2)
{
	return LookingAtIRBlock->NewSub(field, field2);
}

IRInstruction* SystematicAnalysis::Build_Add_sIntPtr(IRInstruction* field, IRInstruction* field2)
{
	return LookingAtIRBlock->NewAdd(field, field2);
}

IRInstruction* SystematicAnalysis::Build_Sub_sIntPtr(IRInstruction* field, IRInstruction* field2)
{
	return LookingAtIRBlock->NewSub(field, field2);
}

IRInstruction* SystematicAnalysis::Build_Mult_uIntPtr(IRInstruction* field, IRInstruction* field2)
{
	return LookingAtIRBlock->NewUMul(field, field2);
}

IRInstruction* SystematicAnalysis::Build_Mult_sIntPtr(IRInstruction* field, IRInstruction* field2)
{
	return LookingAtIRBlock->NewSMul(field, field2);
}

IRInstruction* SystematicAnalysis::Build_Div_uIntPtr(IRInstruction* field, IRInstruction* field2)
{
	return LookingAtIRBlock->NewUDiv(field, field2);
}

IRInstruction* SystematicAnalysis::Build_Div_sIntPtr(IRInstruction* field, IRInstruction* field2)
{
	return LookingAtIRBlock->NewSDiv(field, field2);
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
IRInstruction* SystematicAnalysis::LoadEvaluatedEx(const RawEvaluatedObject& Value, const TypeSymbol& ValueType)
{
	void* ObjectData = Get_Object(ValueType, Value);
	switch (ValueType._Type)
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

void SystematicAnalysis::Build_Decrement_sIntPtr(IRInstruction* field, SIntNative Value)
{
	return Build_Decrement_uIntPtr(field, *(UAddress*)&Value);
}

void SystematicAnalysis::CheckVarWritingErrors(Symbol* Symbol, const Token* Token, String_view& Name)
{
	if (Symbol->VarType.Isimmutable())
	{
		LogCantModifyiMutableError(Token, Name);
	}
}

String SystematicAnalysis::MangleName(const FuncInfo* Func)
{
	Vector< ReflectionTypeInfo> Vect;
	for (auto& Item : Func->Pars)
	{
		Vect.push_back(ConvertToTypeInfo(Item));
	}

	return NameDecoratior::GetDecoratedName(Func->FullName, Vect);
}

IRidentifierID SystematicAnalysis::GetIRID(const FuncInfo* Func)
{
	auto FuncName = MangleName(Func);
	return _Builder.ToID(FuncName);
}

void SystematicAnalysis::LogCantCastImplicitTypes(const Token* Token, const TypeSymbol& Ex1Type, const TypeSymbol& UintptrType, bool ReassignMode)
{
	if (Ex1Type.IsBadType() || UintptrType.IsBadType()) { return; }

	bool V1 = IsAddessAndLValuesRulesfollowed(Ex1Type, UintptrType, ReassignMode);
	if (!V1 || CanDoTypeToTrait(Ex1Type, UintptrType))
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
void SystematicAnalysis::LogCantCastImplicitTypes_Constant(const Token* Token,const TypeSymbol& Ex1Type, const TypeSymbol& UintptrType)
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
		,"Cant Find Type for '" + (String)StringHelper::ToString(TokenType::KeyWord_This) + "'");
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

	Msg += "Wanted " + std::to_string(FuncCount) + " parameters Found " + std::to_string(Count);

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

	Msg += "Wanted " + std::to_string(FuncCount) + " Generic types Found " + std::to_string(Count);

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
void SystematicAnalysis::LogCanIncorrectStaticArrCount(const Token* Token, const TypeSymbol& Type, size_t Count, size_t FuncCount)
{
	String Msg;

	if (Count > FuncCount)
	{
		Msg = "Too Many Values for Type '" + ToString(Type) + "'";
	}
	else
	{
		Msg = "too little Values for Type'" + ToString(Type) + "'";
	}

	Msg += "Wanted " + std::to_string(FuncCount) + " Values Found " + std::to_string(Count);

	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, Msg);
}
void SystematicAnalysis::LogBeMoreSpecifiicWithStaticArrSize(const Token* Token, const TypeSymbol& Type)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Be More Specifiic with Static Array Size.Ex: " + ToString(Type) + "[/1]");
}
void SystematicAnalysis::LogWantedAVariable(const Token* const& Item,Symbol* TepSyb)
{
	_ErrorsOutput->AddError(ErrorCodes::BackEndError, Item->OnLine, Item->OnPos,
		"found a " + ToString(TepSyb->Type) + "(" + TepSyb->FullName + ")" + ".but wanted a Variable or a class field");
}

void SystematicAnalysis::LogBinaryOverloadPars(const Token& Name, const FuncInfo* Func)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidType, Name.OnLine, Name.OnPos
		, "The Binary Overload '" + ToString(Name.Type) + "'" + " must have 2 paameters it has " + std::to_string(Func->Pars.size()) + " Pameters");
}
void SystematicAnalysis::LogIndexOverloadPars(const Token& Name, const FuncInfo* Func)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidType, Name.OnLine, Name.OnPos
		, "The Index Overload '" + ToString(Name.Type) + "'" + " must have 2 paameters it has " + std::to_string(Func->Pars.size()) + " Pameters");
}
void SystematicAnalysis::LogPostfixOverloadPars(const Token& Name, const FuncInfo* Func)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidType, Name.OnLine, Name.OnPos
		, "The Index Overload '" + ToString(Name.Type) + "'" + " must have 1 paameters it has " + std::to_string(Func->Pars.size()) + " Pameters");
}

void SystematicAnalysis::LogCantOverLoadOverload(const UCodeLang::Token* NameToken)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidName, NameToken->OnLine, NameToken->OnPos, "You may not Overload '" + ToString(NameToken->Type) + "'.");
}

void SystematicAnalysis::LogCantFindMemberOverloadForType(const Token* Item, TokenType Op, const TypeSymbol& Out)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidName, Item->OnLine, Item->OnPos
		, "Cant find operator overload for '" + ToString(Op) + "' For Type " + ToString(Out));

}
void SystematicAnalysis::LogMustMakeEnumLikeafuncion(EnumInfo* Einfo, size_t Index, const Token* Token)
{
	auto& Str = Einfo->Fields[Index].Name;
	String Msg = "The enum field '" + Str + "' on '" + Einfo->FullName + "' must be created like a function.Ex:";
	Msg += Einfo->FullName + "::" + Str + "(";

	auto& List = Einfo->VariantData.value().Variants[Index].Types;
	for (auto& Item2 : List)
	{
		Msg += ToString(Item2);

		if (&Item2 != &List.back()) {
			Msg += ",";
		}
	}

	Msg += ")";
	_ErrorsOutput->AddError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos,
		Msg);
}
void SystematicAnalysis::LogCantUseMoveTypeHere(const UCodeLang::Token* Token)
{

	_ErrorsOutput->AddError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "Cant use moved Type Here.it can only be used in Parameters");
}
void SystematicAnalysis::LogDynamicMustBeRrait(const TypeNode& V,const TypeSymbol& Out)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidType, V.Name.Token->OnLine, V.Name.Token->OnPos, "useing a Dynamic type on a none trait the type found '" + ToString(Out) + "'");
}
void SystematicAnalysis::TraitCantBeAlone(const UCodeLang::Token* Token)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "the Type uses a Trait.but Traits cant be use alone.");
}

void SystematicAnalysis::LogWantedAType(const UCodeLang::FrontEnd::TypeNode& V, UCodeLang::FrontEnd::Symbol* SybV)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidType, V.Name.Token->OnLine, V.Name.Token->OnPos,
		"found a '" + ToString(SybV->Type) + "' for the Symbol " + SybV->FullName + " but wanted a type");
}
void SystematicAnalysis::LogOutCanOnlyBeInControlFlow(const UCodeLang::Token* Token)
{

	_ErrorsOutput->AddError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "The 'out' can only be used in Control flow like if");
}
void SystematicAnalysis::LogParamterMustBeAnOutExpression(const UCodeLang::Token* Token, const size_t& i)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "parameter '" + std::to_string(i) + "' does not use the out keyword");
}
UCodeLangFrontEnd


