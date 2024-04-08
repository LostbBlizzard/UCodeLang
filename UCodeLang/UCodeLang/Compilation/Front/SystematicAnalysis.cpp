#ifndef UCodeLangNoCompiler

#include "SystematicAnalysis.hpp"
#include "UCodeLang/Compilation/Helpers/KeyWords.hpp"
#include "UCodeLang/Compilation/Helpers/InstructionBuilder.hpp"
UCodeLangFrontStart


//Dev Note:So this may be the bigest file in the project.
//Some parts may be pulled out for the Language_Server.
//It's a lot if theres any Questions just ask Lost blizzard.
//I don't plan on separating into different files because of c++ compiles time
//and how everything is weirdly dependent on everything else and just may increase complexity.
//Its an self-contained mess of a file.

//New Note so i did Spit the files into diffent .cpp files.
//downside it longer to compile on msvc the compiler i use
//I think its beter.this file used to be 17,000 to 20,000 lines long

constexpr size_t GenericTestStrSize = sizeof(GenericTestStr);


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
	Vector<NeverNullPtr<UClib>> Libs;
	Vector<NeverNullPtr<FileNode>> files;
	files.push_back(NeverNullptr(&File));
	return Analyze(files, Libs);
}
bool SystematicAnalysis::Analyze(const Vector<Unique_ptr<FileNode>>& Files, const Vector<Unique_ptr<UClib>>& Libs)
{
	Vector<NeverNullPtr<UClib>> libs;
	Vector<NeverNullPtr<FileNode>> files;

	for (auto& Item : Files)
	{
		files.push_back(Item.get());
	}
	for (auto& Item : Libs)
	{
		libs.push_back(Item.get());
	}

	return Analyze(files, libs);
}
bool SystematicAnalysis::Analyze(const Vector<NeverNullPtr<FileNode>>& Files, const Vector<NeverNullPtr<UClib>>& Libs)
{


	_PassType = PassType::Null;
	_Files = &Files;
	_Libs = &Libs;


	{
		for (const auto& File : *_Files)
		{
			_FilesData.AddValue(NeverNullptr<FileNode_t>(File.value()), Unique_ptr<FileNodeData>(new FileNodeData()));
		}

	}
	{
		_ForceImportArgWasPassed = _Settings->HasFlagArg("ForceImport");
		_ImmutabilityIsForced = _Settings->HasFlagArg("ForcedImmutability");

		_StartingNameSpace = _Settings->GetArgValueFlag("StartingNameSpace");
	}

	_PassType = PassType::GetTypes;
	Pass();

	if (!_ErrorsOutput->Has_Errors())
	{


		Assembly_LoadLibSymbols();
		_PassType = PassType::FixedTypes;
		Pass();


		if (!_ErrorsOutput->Has_Errors())
		{
			Lib_BuildLibs(false);//Because of generics the ClassAssembly may need them.
			Lib_BuildLibs(true);//Because of Enums  may need them.

			BuildCode();	
		}
	};

	_Files = nullptr;
	_Libs = nullptr;

	_Lib._LibType = Output_TypeAsLibType();

	return !_ErrorsOutput->Has_Errors();
}
void SystematicAnalysis::BuildCode()
{
	UCodeLangAssert(_PassType == PassType::FixedTypes);
	for (auto& Item : _Table.Symbols)
	{
		if (!Item->OutputIR || StringHelper::Contains(Item->FullName, GenericTestStr)) { continue; }
		UCodeLangAssert(Item->ValidState == SymbolValidState::valid);



		switch (Item->Type)
		{
		case SymbolType::Type_class:
		{
			UCodeLangAssert(Item->PassState == PassType::FixedTypes);
			IR_Build_ConvertToIRClassIR(*Item);
		}
		break;
		case SymbolType::Enum:
		{
			UCodeLangAssert(Item->PassState == PassType::FixedTypes);
			IR_Build_ConveToIRVariantEnum(*Item);
		}
		break;
		case SymbolType::Type_StaticArray:
		{
			UCodeLangAssert(Item->PassState == PassType::FixedTypes);
			IR_Build_ConvertToStaticArray(*Item);
		}
		break;
		case SymbolType::StaticVarable:
		{
			UCodeLangAssert(Item->PassState == PassType::FixedTypes);
			auto StaticVarIR = _IR_Builder.NewStaticVarable(_IR_Builder.ToID(Item->FullName), IR_ConvertToIRType(Item->VarType));
		}
		break;
		case SymbolType::ThreadVarable:
		{
			UCodeLangAssert(Item->PassState == PassType::FixedTypes);
			auto ThreadVarIR = _IR_Builder.NewThreadLocalVarable(_IR_Builder.ToID(Item->FullName), IR_ConvertToIRType(Item->VarType));
		}
		break;
		default:
			break;
		}
	}



	_PassType = PassType::BuidCode;
	Pass();

	{
		for (auto& Symbol : _InlineEnums)
		{


			auto Info = Symbol->Get_Info<EnumInfo>();

			if (Info->VariantData.has_value())
			{
				auto& variantVal = Info->VariantData.value();

				Assembly_AddEnum(Symbol);

			}
		}
		for (auto& Symbol : _Table.Symbols)
		{
			if (Symbol->Type == SymbolType::Type_StaticArray)
			{
				Assembly_AddStaticArray(NeverNullptr(Symbol.get()));
			}
		}
	}
	{
		auto oldcontext = SaveAndMove_SymbolContext();

		for (auto& Symbol : _GeneratedTraitSymbols)
		{
			switch (Symbol->Type)
			{
			case SymbolType::Type_class:
			{
				auto Info = Symbol->Get_Info<ClassInfo>();
				auto node = ClassNode::As(Symbol->Get_NodeInfo<Node>());


				Set_SymbolContext(std::move(Info->Context.value()));
				OnClassNode(*node);
				Info->Context = SaveAndMove_SymbolContext();
			}
			break;
			case SymbolType::Type_alias:
			{
				auto Info = Symbol->Get_Info<AliasInfo>();
				auto node = AliasNode::As(Symbol->Get_NodeInfo<Node>());


				Set_SymbolContext(std::move(Info->Context.value()));
				OnAliasNode(*node);
				Info->Context = SaveAndMove_SymbolContext();
			}
			break;
			default:
				UCodeLangUnreachable();
				break;
			}
		}

		Set_SymbolContext(std::move(oldcontext));
	}
	{
		auto oldcontext = SaveAndMove_SymbolContext();
		for (auto& Item : _Generic_GeneratedGenericSymbol)
		{
			auto Symbol = Symbol_GetSymbol(Item.ID);

			_Generic_GenericSymbolStack.push(std::move(Item.Info));
			switch (Symbol->Type)
			{
			case SymbolType::Func:
			{
				auto Info = Symbol->Get_Info<FuncInfo>();
				auto node = FuncNode::As(Symbol->Get_NodeInfo<Node>());


				Set_SymbolContext(std::move(Info->Context.value()));
				OnFuncNode(*node);
				Info->Context = SaveAndMove_SymbolContext();
			}
			break;
			case SymbolType::Type_class:
			{
				auto Info = Symbol->Get_Info<ClassInfo>();
				auto node = ClassNode::As(Symbol->Get_NodeInfo<Node>());


				Set_SymbolContext(std::move(Info->Context.value()));
				OnClassNode(*node);
				Info->Context = SaveAndMove_SymbolContext();
			}
			break;
			case SymbolType::Enum:
			{
				auto Info = Symbol->Get_Info<EnumInfo>();
				auto node = EnumNode::As(Symbol->Get_NodeInfo<Node>());


				Set_SymbolContext(std::move(Info->Context.value()));
				OnEnum(*node);
				Info->Context = SaveAndMove_SymbolContext();


			}
			break;
			case SymbolType::Type_alias:
			{
				auto Info = Symbol->Get_Info<Generic_AliasInfo>();
				auto node = AliasNode::As(Symbol->Get_NodeInfo<Node>());


				Set_SymbolContext(std::move(Info->Context.value()));
				OnAliasNode(*node);
				Info->Context = SaveAndMove_SymbolContext();


			}
			break;
			case SymbolType::Tag_class:
			{
				auto Info = Symbol->Get_Info<TagInfo>();
				auto node = TagTypeNode::As(Symbol->Get_NodeInfo<Node>());


				Set_SymbolContext(std::move(Info->Context.value()));
				OnTag(*node);
				Info->Context = SaveAndMove_SymbolContext();


			}
			break;
			case SymbolType::ForType:
			{
				auto Info = Symbol->Get_Info<ForTypeInfo>();
				auto node = ForTypeNode::As(Symbol->Get_NodeInfo<Node>());


				Set_SymbolContext(std::move(Info->Context.value()));

				OnForTypeNode(*node);
				Info->Context = SaveAndMove_SymbolContext();


			}
			break;
			case SymbolType::Trait_class:
			{
				auto Info = Symbol->Get_Info<TraitInfo>();
				auto node = TraitNode::As(Symbol->Get_NodeInfo<Node>());

				Set_SymbolContext(std::move(Info->Context.value()));
				OnTrait(*node);
				Info->Context = SaveAndMove_SymbolContext();
			}
			break;
			default:
				UCodeLangUnreachable();
				break;
			}

			Item.Info = std::move(_Generic_GenericSymbolStack.top());
			_Generic_GenericSymbolStack.pop();
		}
	}
}

void SystematicAnalysis::Debug_Add_SetLineNumber(const NeverNullPtr<Token> token, size_t InsInBlock)
{
	auto ThisFileName = _LookingAtFile->FileName.generic_string();
	auto LineNumber = token->OnLine;
	if (_LastLineNumber != LineNumber || (ThisFileName != _LastIRFileName) || (_Debug_LastLookAtDebugBlock != &_IR_LookingAtIRBlock->DebugInfo))
	{
		if (_Debug_LastLookAtDebugBlock != &_IR_LookingAtIRBlock->DebugInfo)
		{
			_LastIRFileName = "";
			_LastLineNumber = -1;

			_Debug_LastLookAtDebugBlock = &_IR_LookingAtIRBlock->DebugInfo;
		}

		if (ThisFileName != _LastIRFileName)
		{
			_LastIRFileName = ThisFileName;
			_IR_LookingAtIRBlock->DebugInfo.Add_SetFile(ThisFileName, InsInBlock);
		}
		_LastLineNumber = LineNumber;

		_IR_LookingAtIRBlock->DebugInfo.Add_SetLineNumber(token->OnLine, InsInBlock);
	}
}
void SystematicAnalysis::Debug_Add_SetVarableInfo(const Symbol& Syb, size_t InsInBlock)
{
	String irvarname;
	if (Syb.Type == SymbolType::StaticVarable)
	{
		irvarname = Syb.FullName;
	}
	else if (Syb.Type == SymbolType::ThreadVarable)
	{
		irvarname = Syb.FullName;
	}
	else
	{
		auto scope = Syb.FullName.substr(_FuncStack.front().Pointer->FullName.size());

		auto irfuncname = _IR_LookingAtIRFunc ? _IR_Builder.FromID(_IR_LookingAtIRFunc->identifier) : "";

		irvarname = irfuncname + scope + ScopeHelper::GetNameFromFullName(Syb.FullName);
	}
	auto ID = _IR_Builder.ToID(RemoveSymboolFuncOverloadMangling(irvarname));
	IRDebugSetVarableName V;

	V.IRVarableName = irvarname;
	V.VarableName = Syb.FullName;
	V.InsInBlock = InsInBlock;

	_IR_LookingAtIRBlock->DebugInfo.Add_SetVarableName(std::move(V));

	IRDebugSymbol Info;
	Info.IRVarableName = irvarname;
	Info.VarableName = Syb.FullName;

	ReflectionTypeInfo Type = Assembly_ConvertToType(Syb.VarType);

	Info.LangType = UCode_LangType_UCodeLang;

	BitMaker bitm;
	UClib::ToBytes(bitm, Type);
	Info.TypeInfo = std::move(bitm.Get_Bytes());

	switch (Syb.Type)
	{
	case SymbolType::ParameterVarable:
		Info._Type = IRDebugSymbol::Type::Par;
		break;
	case SymbolType::StackVarable:
		Info._Type = IRDebugSymbol::Type::Stack;
		break;
	case SymbolType::StaticVarable:
		Info._Type = IRDebugSymbol::Type::Static;
		break;
	case SymbolType::ThreadVarable:
		Info._Type = IRDebugSymbol::Type::Thread;
		break;
	default:
		UCodeLangUnreachable();
		break;
	}


	_IR_Builder._Debug.Symbols.AddValue(ID, std::move(Info));
}
const NeverNullPtr<FileNode> SystematicAnalysis::FileDependency_Get_FileUseingSymbol(const NeverNullPtr<Symbol> Syb)
{
	return NeverNullptr(Syb->_File);
}
void SystematicAnalysis::FileDependency_AddDependencyToCurrentFile(const NeverNullPtr<Symbol> Syb)
{
	FileDependency_AddDependencyToCurrentFile(FileDependency_Get_FileUseingSymbol(Syb));


	auto CurrentFile = _LookingAtFile;
	auto& FileData = GetFileData(NeverNullptr<FileNode_t>(CurrentFile));
	NullablePtr<Symbol> LookingAtSyb = Syb.AsNullable();
	bool IsAnImport = false;
	while (LookingAtSyb)
	{
		NeverNullPtr<Symbol> LookingAtSybval = LookingAtSyb.value();


		const String* StringToLookAt = nullptr;
		if (LookingAtSybval->Type == SymbolType::Func)
		{
			StringToLookAt = &LookingAtSybval->Get_Info<FuncInfo>()->FullName;//because of how Import alias works with Funcs
		}
		else
		{
			StringToLookAt = &LookingAtSybval->FullName;

		}

		/*
		for (auto& Item : FileData._Imports)
		{

			if (Item.ImportSymbolFullName == *StringToLookAt)
			{
				*Item.IsImportUsed = true;
				IsAnImport = true;
				break;
			}

		}
		*/

		if (LookingAtSybval->Type == SymbolType::Type_alias
			|| LookingAtSybval->Type == SymbolType::Hard_Type_alias)
		{
			LookingAtSyb = Symbol_GetSymbol(LookingAtSybval->VarType);
		}
		else
		{
			LookingAtSyb = nullptr;
		}
	}

	if (CurrentFile != Syb->_File && !IsAnImport && (FileData._Imports.size() || _ForceImportArgWasPassed))
	{
		auto Token = _LastLookedAtToken.value();
		LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "Trying to use the Symbol '" + Syb->FullName + "[" + ToString(Syb->Type) + "]' but it's not Imported in the file.");
	}
}
void SystematicAnalysis::FileDependency_AddDependencyToCurrentFile(const NeverNullPtr<FileNode> file)
{
	auto CurrentFile = NeverNullptr(_LookingAtFile);
	const NeverNullPtr<FileNode_t> CurrentFile_t = NeverNullptr<FileNode_t>(CurrentFile.value());
	const NeverNullPtr<FileNode_t> file_t = NeverNullptr<FileNode_t>(file.value());
	if (CurrentFile != file)
	{
		FileNodeData& Data = *_FilesData.GetValue(CurrentFile_t);

		for (auto& Item : Data._Dependencys)
		{
			const NeverNullPtr<FileNode_t> Itemfile_t = NeverNullptr<FileNode_t>(Item.value());
			if (Itemfile_t == file_t)
			{
				return;
			}
		}
		Data._Dependencys.push_back(file_t);

	}

}
void SystematicAnalysis::FileDependency_AddDependencyToCurrentFile(const TypeSymbol& type)
{
}
Symbol& SystematicAnalysis::Symbol_AddSymbol(SymbolType type, const String& Name, const String& FullName, AccessModifierType Access)
{

	auto& r = _Table.AddSymbol(type, Name, FullName, Access);
	r._File = _LookingAtFile;
	return r;
}
void SystematicAnalysis::Pass()
{
	for (const auto& File : *_Files)
	{
		OnFileNode(*File);
	}

}
void SystematicAnalysis::OnNamespace(const NamespaceNode& node)
{
	auto UseingIndex = _Table.GetUseingIndex();

	const auto Namespace = Str_GetScopedNameAsString(node._NamespaceName);
	_Table.AddScope(Namespace);

	if (_PassType == PassType::GetTypes)
	{
		if (!Symbol_GetSymbol(String_view(Namespace), SymbolType::Namespace))
		{
			Symbol_AddSymbol(SymbolType::Namespace, Namespace, _Table._Scope.ThisScope, AccessModifierType::Public);
		}
	}


	for (auto& node : node._Nodes)
	{
		Push_ToNodeScope(*node.get());
		switch (node->Get_Type())
		{
		case NodeType::NamespaceNode:OnNamespace(*NamespaceNode::As(node.get())); break;
		case NodeType::ClassNode: OnClassNode(*ClassNode::As(node.get())); break;
		case NodeType::AliasNode:OnAliasNode(*AliasNode::As(node.get())); break;
		case NodeType::EnumNode:OnEnum(*EnumNode::As(node.get())); break;
		case NodeType::FuncNode:OnFuncNode(*FuncNode::As(node.get())); break;
		case NodeType::UsingNode: OnUseingNode(*UsingNode::As(node.get())); break;
		case NodeType::TraitNode:OnTrait(*TraitNode::As(node.get())); break;
		case NodeType::TagTypeNode:OnTag(*TagTypeNode::As(node.get())); break;
		default:break;
		}
		Pop_NodeScope();
	}

	if (_PassType == PassType::BuidCode)
	{
		auto name = Namespace;
		bool hasnamespace = _Lib.Get_Assembly().Find_Node(ClassType::NameSpace, _Table._Scope.ThisScope);

		if (hasnamespace == false)
		{
			NameSpace_Data& VClass = _Lib.Get_Assembly().AddNameSpace(name, _Table._Scope.ThisScope);
		}
	}

	_Table.RemoveScope();

	_Table.RemovePopUseing(UseingIndex);
}
void SystematicAnalysis::OnFileNode(const FileNode& File)
{
	_LookingAtFile = &File;
	_ErrorsOutput->FilePath = File.FileName;

	size_t ClassesStart = PassType::BuidCode == _PassType ? _Lib._Assembly.Classes.size() : 0;
	size_t GlobalMethodStart = PassType::BuidCode == _PassType ? _Lib._Assembly.Get_GlobalObject_Class()->Methods.size() : 0;
	size_t StaticInitStart = PassType::BuidCode == _PassType && _IR_Builder._StaticInit.Blocks.size() ? _IR_Builder._StaticInit.Blocks.front()->Instructions.size() : 0;
	size_t ThreadInitStart = PassType::BuidCode == _PassType && _IR_Builder._threadInit.Blocks.size() ? _IR_Builder._threadInit.Blocks.front()->Instructions.size() : 0;
	size_t StaticDeInitStart = PassType::BuidCode == _PassType && _IR_Builder._StaticdeInit.Blocks.size() ? _IR_Builder._StaticdeInit.Blocks.front()->Instructions.size() : 0;
	size_t ThreadDeInitStart = PassType::BuidCode == _PassType && _IR_Builder._threaddeInit.Blocks.size() ? _IR_Builder._threaddeInit.Blocks.front()->Instructions.size() : 0;
	size_t IRFunsStart = PassType::BuidCode == _PassType ? _IR_Builder.Funcs.size() : 0;
	size_t IRSymbolsStart = PassType::BuidCode == _PassType ? _IR_Builder._Symbols.size() : 0;
	size_t IRConstStaticStart = PassType::BuidCode == _PassType ? _IR_Builder.ConstStaticStrings.size() : 0;

	bool DoneWithImports = false;

	if (_PassType == PassType::BuidCode)
	{
		auto& FileData = GetFileData(NeverNullptr((FileNode_t*)&File));

		for (auto& Import : FileData._Imports)
		{
			for (size_t i = 0; i < Import._AliasSymbols.size(); i++)
			{
				auto& Item = Import._AliasSymbols[i];
				Item->FullName = std::move(Import._TepSymbolFullNames[i]);//allow import alias to be seen.
			}
		}
	}

	if (_StartingNameSpace.has_value())
	{
		_Table.AddScope(_StartingNameSpace.value());

		if (_PassType == PassType::BuidCode)
		{
			auto name = _StartingNameSpace.value();
			bool hasnamespace = _Lib.Get_Assembly().Find_Node(ClassType::NameSpace, _Table._Scope.ThisScope);

			if (hasnamespace == false)
			{
				NameSpace_Data& VClass = _Lib.Get_Assembly().AddNameSpace(name, _Table._Scope.ThisScope);
			}
		}
	}

	auto UseingIndex = _Table.GetUseingIndex();
	for (auto& node : File._Nodes)
	{
		Push_ToNodeScope(*node.get());
		UCodeLangDefer(Pop_NodeScope());

		if (DoneWithImports == false && node->Get_Type() != NodeType::ImportStatement && _PassType == PassType::FixedTypes)
		{
			DoneWithImports = true;


			auto& FileData = GetFileData(NeverNullptr<FileNode_t>(_LookingAtFile));

			for (auto& Import : FileData._Imports)
			{
				Import._TepSymbolFullNames.resize(Import._AliasSymbols.size());
			}
		}

		switch (node->Get_Type())
		{
		case NodeType::NamespaceNode:OnNamespace(*NamespaceNode::As(node.get())); break;
		case NodeType::AliasNode:OnAliasNode(*AliasNode::As(node.get())); break;
		case NodeType::ClassNode: OnClassNode(*ClassNode::As(node.get())); break;
		case NodeType::EnumNode:OnEnum(*EnumNode::As(node.get())); break;
		case NodeType::FuncNode:OnFuncNode(*FuncNode::As(node.get())); break;
		case NodeType::UsingNode: OnUseingNode(*UsingNode::As(node.get())); break;
		case NodeType::DeclareStaticVariableNode:OnDeclareStaticVariableNode(*DeclareStaticVariableNode::As(node.get())); break;
		case NodeType::DeclareThreadVariableNode:OnDeclareThreadVariableNode(*DeclareThreadVariableNode::As(node.get())); break;
		case NodeType::DeclareEvalVariableNode:OnDeclareEvalVariableNode(*DeclareEvalVariableNode::As(node.get())); break;
		case NodeType::TraitNode:OnTrait(*TraitNode::As(node.get())); break;
		case NodeType::TagTypeNode:OnTag(*TagTypeNode::As(node.get())); break;
		case NodeType::ImportStatement:
		{
			OnImportNode(*ImportStatement::As(node.get()));
			if (_ErrorsOutput->Has_Errors())
			{
				goto OutofLoop;
			}
			break;
		}
		case NodeType::CompileTimeIfNode:
			OnCompileTimeIfNode(*CompileTimeIfNode::As(node.get()), false);
			break;
		case NodeType::ForTypeNode:
			OnForTypeNode(*ForTypeNode::As(node.get()));
			break;
		default:
			UCodeLangUnreachable();
			break;
		}


	}
	_Table.RemovePopUseing(UseingIndex);

	if (_StartingNameSpace.has_value())
	{
		for (size_t i = 0; i < ScopeHelper::Get_ScopeCount(_StartingNameSpace.value()) + 1; i++)
		{
			_Table.RemoveScope();
		}
	}

OutofLoop:

	if (_PassType == PassType::BuidCode)
	{
		auto& FileData = GetFileData(NeverNullptr<FileNode_t>(&File));


		FileData.AssemblyInfoSpan = FileNodeData::SpanData::NewWithNewIndex(ClassesStart, _Lib._Assembly.Classes.size());
		FileData.GlobalObjectMethodInfoSpan = FileNodeData::SpanData::NewWithNewIndex(GlobalMethodStart, _Lib._Assembly.Get_GlobalObject_Class()->Methods.size());
		FileData.IRInitStaticSpan = FileNodeData::SpanData::NewWithNewIndex(StaticInitStart, _IR_Builder._StaticInit.Blocks.size() ? _IR_Builder._StaticInit.Blocks.front()->Instructions.size() : 0);
		FileData.IRInitThreadSpan = FileNodeData::SpanData::NewWithNewIndex(ThreadInitStart, _IR_Builder._threadInit.Blocks.size() ? _IR_Builder._threadInit.Blocks.front()->Instructions.size() : 0);
		FileData.IRDeInitStaticSpan = FileNodeData::SpanData::NewWithNewIndex(StaticDeInitStart, _IR_Builder._StaticdeInit.Blocks.size() ? _IR_Builder._StaticdeInit.Blocks.front()->Instructions.size() : 0);
		FileData.IRDeInitThreadSpan = FileNodeData::SpanData::NewWithNewIndex(ThreadDeInitStart, _IR_Builder._threaddeInit.Blocks.size() ? _IR_Builder._threaddeInit.Blocks.front()->Instructions.size() : 0);
		FileData.IRFuncsSpan = FileNodeData::SpanData::NewWithNewIndex(IRFunsStart, _IR_Builder.Funcs.size());
		FileData.IRSymbolSpan = FileNodeData::SpanData::NewWithNewIndex(IRSymbolsStart, _IR_Builder._Symbols.size());
		FileData.IRConstStringSpan = FileNodeData::SpanData::NewWithNewIndex(IRConstStaticStart, _IR_Builder.ConstStaticStrings.size());
	}

	if (_PassType == PassType::FixedTypes || _PassType == PassType::BuidCode)
	{
		auto& FileData = GetFileData(NeverNullptr<FileNode_t>(&File));

		for (auto& Import : FileData._Imports)
		{
			for (size_t i = 0; i < Import._AliasSymbols.size(); i++)
			{
				auto& Item = Import._AliasSymbols[i];
				Import._TepSymbolFullNames[i] = std::move(Item->FullName);//removing name to remove exposure from other files.
			}
		}
	}

	_Table.ClearUseings();
}


void SystematicAnalysis::Node_InStatetements(bool Value)
{
	if (_ClassStack.size())
	{
		_ClassStack.top()._InStatements = Value;
	}
}
void SystematicAnalysis::LogError_CantUseOutInOverloadFunc(const Token& Name)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidType, Name.OnLine, Name.OnPos, "you may not use Out parameters for overload.");
}



String SystematicAnalysis::Str_GetClassWithTraitVTableName(const String& ClassFullName, const String& TraitFullName)
{
	return TraitFullName + ":$" + ClassFullName;
}
String SystematicAnalysis::Str_GetTraitVTableName(const String& TraitName)
{
	return TraitName + "vtable";
}
String SystematicAnalysis::Str_GetTraitVStructTableName(const String& TraitName)
{
	return TraitName + "vtable_type";
}

void SystematicAnalysis::LogError_MissingFunctionforTrait(const String_view& FuncName, const FuncInfo* Info, const NeverNullPtr<Symbol> Trait, const NeverNullPtr<Token> ClassNameToken)
{
	String Msg = "Missing Function '" + (String)FuncName + "' with the parameters [";

	for (auto& ItemP : Info->Pars)
	{
		Msg += ToString(ItemP);

		if (&ItemP != &Info->Pars.back())
		{
			Msg += ",";
		}
	}

	Msg += "] and returns '" + ToString(Info->Ret) + "' for the trait '" + Trait->FullName + '\'';

	LogError(ErrorCodes::ExpectingSequence, ClassNameToken->OnLine, ClassNameToken->OnPos, Msg);
}
NeverNullPtr<Symbol> SystematicAnalysis::Symbol_MakeNewDropFuncSymbol(ClassInfo* ClassInfo, const TypeSymbol& ClassAsType)
{

	String FullName = ClassInfo->FullName;
	ScopeHelper::GetApendedString(FullName, (String)ClassDestructorFunc);

	auto DropFunc = &Symbol_AddSymbol(SymbolType::Func, ClassDestructorFunc, FullName, AccessModifierType::Public);
	FuncInfo* V = new FuncInfo();
	DropFunc->Info.reset(V);

	V->FullName = DropFunc->FullName;
	V->_FuncType = FuncInfo::FuncType::Drop;
	V->Ret = TypesEnum::Void;

	V->FrontParIsUnNamed = true;

	auto ThisParType = ClassAsType;
	ThisParType._IsAddress = true;
	V->Pars.push_back({ false,ThisParType });
	ClassInfo->_ClassAutoGenerateDrop = true;

	DropFunc->PassState = PassType::FixedTypes;
	return DropFunc;
}

void SystematicAnalysis::IR_Build_FuncDropUsingFields(const ClassInfo* ClassInfo, const IRType& ThisPar)
{
	for (size_t i = ClassInfo->Fields.size() - 1; i != (Vector<FieldInfo>::size_type) - 1; i--)
	{
		auto& Item = ClassInfo->Fields[i];
		if (Symbol_HasDestructor(Item.Type))
		{
			ObjectToDrop obj;
			obj.DropType = ObjectToDropType::IRInstruction;
			obj.Type = Item.Type;
			obj._Object = _IR_LookingAtIRBlock->New_Member_Dereference(&_IR_LookingAtIRFunc->Pars.front(), ThisPar, i);

			IR_Build_DestructorCall(obj);
		}
	}
}
void SystematicAnalysis::OnStatementsWithSetableRet(const StatementsNode& node, TypeSymbol& RetOut, const NeverNullPtr<Token> Token)
{
	_LookingForTypes.push(RetOut);

	bool  HasARet = false;

	Node_InStatetements(true);
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

				auto NewType = _LastExpressionType;

				if (OldType.IsAddress()) {
					NewType.SetAsAddress();
				}
				if (OldType.IsAddressArray()) {
					NewType.SetAsAddressArray();
				}

				bool IsPassedYield = IsAfterYield();

				if (IsPassedYield)
				{
					NewType = Type_MakeFutureFromType(NewType);
				}

				RetOut = NewType;
				RetOut._MoveData = MoveData::None;
				Type_Get_LookingForType() = RetOut;

				if (_LastExpressionType._Type == TypesEnum::Var)
				{
					auto Token = _LastLookedAtToken;
					LogError_CantguessVarTypeError(Token.value());
				}

				/*
				if (ErrCount == _ErrorsOutput->Get_Errors().size()) {
					OnStatement(*node2);//re do
				}
				*/
			}

			break;
		}
	}
	Node_InStatetements(false);

	_LookingForTypes.pop();


	if (!HasARet)
	{
		if (RetOut._Type == TypesEnum::Var)
		{
			RetOut.SetType(TypesEnum::Void);
		}
		else if (RetOut._Type != TypesEnum::Void
			&& !RetOut.IsBadType())//Update This when control flow get added.
		{
			//LogError_YouMustReturnSomethingError(Token);
		}

	}
}

bool SystematicAnalysis::Type_IsStructPassByRef(const NeverNullPtr<Symbol> syb) const
{
	auto r = !(Type_IsPrimitive(syb->VarType) || syb->VarType.IsAddress());
	if (r == false && syb->VarType._Type == TypesEnum::CustomType)
	{
		auto V = Symbol_GetSymbol(syb->VarType);
		r = V.value()->Type == SymbolType::Type_StaticArray;
	}
	return r;
}

size_t SystematicAnalysis::IR_GetJumpsIndex() { return _IR_Jumps.size() ? _IR_Jumps.size() - 1 : 0; }
void SystematicAnalysis::IR_RemoveJumps(size_t Index)
{
	size_t toremove = _IR_Jumps.size() - Index;
	for (size_t i = 0; i < toremove; i++)
	{
		_IR_Jumps.pop_back();
	}
}
IRidentifierID SystematicAnalysis::IR_Build_ConvertToIRClassIR(const Symbol& Class)
{
	auto ClassSybID = Class.ID;
	if (_Symbol_SybToIRMap.HasValue(ClassSybID))
	{
		return _Symbol_SybToIRMap.GetValue(ClassSybID);
	}
	
	const ClassInfo* clasinfo = Class.Get_Info < ClassInfo>();

	IRidentifierID V = _IR_Builder.ToID(Class.FullName);
	for (auto& Item : _IR_Builder._Symbols)
	{
		if (Item->identifier == V)
		{
			_Symbol_SybToIRMap.AddValue(ClassSybID,V);
			return V;
		}
	}

	auto IRStuct = _IR_Builder.NewStruct(V);

	IRStuct->Fields.resize(clasinfo->Fields.size());

	for (size_t i = 0; i < clasinfo->Fields.size(); i++)
	{
		auto& Item = clasinfo->Fields[i];
		auto& Out = IRStuct->Fields[i];
		Out.Type = IR_ConvertToIRType(Item.Type);
		Out.Offset = Type_GetOffset(*clasinfo, &Item).value();
	}

	_Symbol_SybToIRMap.AddValue(ClassSybID, V);
	return V;
}

IRidentifierID SystematicAnalysis::IR_Build_ConvertToStaticArray(const Symbol& Class)
{
	auto ClassSybID = Class.ID;
	if (_Symbol_SybToIRMap.HasValue(ClassSybID))
	{
		return _Symbol_SybToIRMap.GetValue(ClassSybID);
	}
	const StaticArrayInfo* clasinfo = Class.Get_Info <StaticArrayInfo>();


	if (clasinfo->Type.IsTypeInfo())
	{
		IRidentifierID V = 0;
		_Symbol_SybToIRMap.AddValue(ClassSybID, V);
		return V;
	}

	IRidentifierID V = _IR_Builder.ToID(Class.FullName);
	for (auto& Item : _IR_Builder._Symbols)
	{
		if (Item->identifier == V)
		{
			_Symbol_SybToIRMap.AddValue(ClassSybID,V);
			return V;
		}
	}

	auto IRStuct = _IR_Builder.NewStaticArray(V, IR_ConvertToIRType(clasinfo->Type), clasinfo->Count);


	if (Symbol_HasDestructor(clasinfo->Type))
	{
		auto DropFuncName = ToString(TypeSymbol(Class.ID));
		ScopeHelper::GetApendedString(DropFuncName, ClassDestructorFunc);

		auto oldfunc = _IR_LookingAtIRFunc;
		auto oldblock = _IR_LookingAtIRBlock;

		auto DropFunc = Symbol_GetSymbol(DropFuncName, SymbolType::Func).value();
		_IR_LookingAtIRFunc = _IR_Builder.NewFunc(IR_GetIRID(DropFunc->Get_Info<FuncInfo>()), IRTypes::Void);
		{
			IRPar Vt;
			Vt.identifier = _IR_Builder.ToID(ThisSymbolName);
			Vt.type._symbol = V;
			Vt.type._Type = IRTypes::pointer;

			_IR_LookingAtIRFunc->Pars.push_back(Vt);
		}
		IRPar* ThisPar = &_IR_LookingAtIRFunc->Pars.front();
		_IR_LookingAtIRBlock = _IR_LookingAtIRFunc->NewBlock(".");

		auto oldpass = _PassType;
		_PassType = PassType::BuidCode;

		auto elmsize = Type_GetSize(clasinfo->Type).value();

		bool UseLoop = clasinfo->Count > 6;
		if (UseLoop)
		{
			auto Ptr = _IR_LookingAtIRBlock->NewLoad(ThisPar);
			//our index
			auto Indexir = IR_Load_UIntptr(0);

			size_t JumpLabel = _IR_LookingAtIRBlock->GetIndex();
			auto Cmpbool = _IR_LookingAtIRBlock->NewC_Equalto(Indexir, IR_Load_UIntptr(clasinfo->Count));


			auto JumpIns = _IR_LookingAtIRBlock->NewConditionalJump(Cmpbool, NullUInt64);

			auto OffsetIr = _IR_LookingAtIRBlock->New_Index_Vetor(Ptr, Indexir, IR_Load_UIntptr(elmsize));

			//loop on every

			{
				ObjectToDrop tep;
				tep._Object = OffsetIr;
				tep.DropType = ObjectToDropType::IRInstructionNoMod;
				tep.Type = clasinfo->Type;
				IR_Build_DestructorCall(tep);//call on Object
			}


			_IR_LookingAtIRBlock->New_Increment(Indexir);//index++



			_IR_LookingAtIRBlock->NewJump(JumpLabel);

			size_t ThisJumpLable = _IR_LookingAtIRBlock->GetIndex();
			_IR_LookingAtIRBlock->UpdateConditionaJump(JumpIns, Cmpbool, ThisJumpLable);


		}
		else
		{
			auto accumulatorPtr = _IR_LookingAtIRBlock->NewLoad(ThisPar);
			for (size_t i = 0; i < clasinfo->Count; i++)
			{

				auto elmsizeir = IR_Load_UIntptr(elmsize);

				ObjectToDrop tep;
				tep._Object = accumulatorPtr;
				tep.DropType = ObjectToDropType::IRInstructionNoMod;
				tep.Type = clasinfo->Type;

				IR_Build_DestructorCall(tep);


				if (i + 1 < clasinfo->Count) {
					_IR_LookingAtIRBlock->NewStore(accumulatorPtr, _IR_LookingAtIRBlock->NewAdd(elmsizeir, accumulatorPtr));
				}
			}
		}
		_PassType = oldpass;

		_IR_LookingAtIRBlock->NewRet();

		_IR_LookingAtIRFunc = oldfunc;
		_IR_LookingAtIRBlock = oldblock;
	}

	_Symbol_SybToIRMap.AddValue(ClassSybID, V);
	return V;
}

IRidentifierID SystematicAnalysis::IR_Build_ConveToIRVariantEnum(const Symbol& Enum)
{
	const EnumInfo* Info = Enum.Get_Info<EnumInfo>();


	if (Info->VariantData.has_value() && Info->IsOptionalAddress().has_value() == false)
	{
		auto ClassSybID = Enum.ID;
		if (_Symbol_SybToIRMap.HasValue(ClassSybID))
		{
			return _Symbol_SybToIRMap.GetValue(ClassSybID);
		}


		IRidentifierID V = _IR_Builder.ToID(Enum.FullName);
		for (auto& Item : _IR_Builder._Symbols)
		{
			if (Item->identifier == V)
			{
				_Symbol_SybToIRMap.AddValue(ClassSybID, V);
				return V;
			}
		}
		auto IRStuct = _IR_Builder.NewStruct(V);


		String UnionName = Str_GetEnumVariantUnionName(Enum.FullName);

		IRidentifierID UnionID = _IR_Builder.ToID(UnionName);
		auto IRUnion = _IR_Builder.NewStruct(UnionID);
		IRUnion->IsUnion = true;

		auto& Item = Info->VariantData.value();
		//

		for (auto& Item2 : Item.Variants)
		{
			if (Item2.ClassSymbol.has_value())
			{
				SymbolID ClassSymbol = Item2.ClassSymbol.value();
				const Symbol* ClassSym = Symbol_GetSymbol(ClassSymbol).value();

				auto irstuctID = _IR_Builder.ToID(ClassSym->FullName);
				IRStructField Vtep;
				Vtep.Type = IRType(irstuctID);
				IRUnion->Fields.push_back(Vtep);
			}
			else
			{
				if (Item2.Types.size())
				{

					IRStructField Vtep;
					Vtep.Type = IR_ConvertToIRType(Item2.Types.front());
					IRUnion->Fields.push_back(Vtep);
				}
			}
		}
		//

		{//key
			IRStructField field;
			field.Type = IR_ConvertToIRType(Info->Basetype);
			IRStuct->Fields.push_back(field);
		}
		{//Union
			IRStructField field;
			field.Type = IRType(UnionID);
			IRStuct->Fields.push_back(field);
		}

		//
		_Symbol_SybToIRMap.AddValue(ClassSybID, UnionID);
		return V;
	}
	return 0;
}

IRType SystematicAnalysis::IR_ConvertToIRType(const TypeSymbol& Value)
{

	UCodeLangAssert(!Value.IsTypeInfo());

	IRType r;

	switch (Value._Type)
	{

	case TypesEnum::Bool:
	case TypesEnum::Char:
	case TypesEnum::sInt8:
	case TypesEnum::Uft8:
	case TypesEnum::uInt8:r = IRType(IRTypes::i8);
		break;

	case TypesEnum::Uft16:
	case TypesEnum::sInt16:
	case TypesEnum::uInt16:r = IRType(IRTypes::i16);
		break;


	case TypesEnum::Uft32:
	case TypesEnum::sInt32:
	case TypesEnum::uInt32:r = IRType(IRTypes::i32);
		break;

	case TypesEnum::sInt64:
	case TypesEnum::uInt64:r = IRType(IRTypes::i64);
		break;

	case TypesEnum::float32:r = IRType(IRTypes::f32);
		break;

	case TypesEnum::float64:r = IRType(IRTypes::f64);
		break;

	case TypesEnum::Void:r = IRType(IRTypes::Void);
		break;

	case TypesEnum::CustomType:
	{
		Symbol& syb = *Symbol_GetSymbol(Value._CustomTypeSymbol);
		if (syb.Type == SymbolType::Enum)
		{
			EnumInfo* V = syb.Get_Info <EnumInfo>();
			if (auto opt = V->IsOptionalAddress().value_unchecked())
			{
				r = IR_ConvertToIRType(*opt);
			}
			else if (V->VariantData.has_value())
			{
				r = IRType(_IR_Builder.ToID(V->FullName));
			}
			else
			{
				r = IR_ConvertToIRType(V->Basetype);
			}
		}
		else if (syb.Type == SymbolType::Type_alias
			|| syb.Type == SymbolType::Hard_Type_alias)
		{
			r = IR_ConvertToIRType(syb.VarType);
		}
		else if (syb.Type == SymbolType::Func_ptr || syb.Type == SymbolType::Hard_Func_ptr)
		{
			if (_Symbol_SybToIRMap.HasValue(syb.ID))
			{
				r = IRType(_Symbol_SybToIRMap.GetValue(syb.ID));
			}
			else
			{
				FuncPtrInfo* V = syb.Get_Info<FuncPtrInfo>();
				IRidentifierID IRid = _IR_Builder.ToID(syb.FullName);
				for (auto& Item : _IR_Builder._Symbols)
				{
					if (Item->identifier == IRid)
					{
						_Symbol_SybToIRMap.AddValue(syb.ID, IRid);
						r = IRType(IRid);
						return r;
					}
				}
				IRType r = IRType(IRid);
				auto tep = _IR_Builder.NewFuncPtr(_IR_Builder.ToID(syb.FullName), IR_ConvertToIRType(V->Ret));

				tep->Pars.resize(V->Pars.size());

				for (size_t i = 0; i < tep->Pars.size(); i++)
				{
					auto& Item = tep->Pars[i];
					Item = IR_ConvertToIRType(V->Pars[i]);
				}
				tep->Ret = IR_ConvertToIRType(V->Ret);

				_Symbol_SybToIRMap.AddValue(syb.ID, IRid);
				return r;
			}
		}
		else if (syb.Type == SymbolType::Type_class)
		{
			r = IRType(IR_Build_ConvertToIRClassIR(syb));
		}
		else if (syb.Type == SymbolType::Type_StaticArray)
		{
			r = IRType(IR_Build_ConvertToStaticArray(syb));
		}
		else if (syb.Type == SymbolType::ConstantExpression)
		{
			r = IR_ConvertToIRType(syb.VarType);
		}
		else if (syb.Type == SymbolType::Trait_class && Value._IsDynamic)
		{
			if (_Symbol_SybToIRMap.HasValue(syb.ID))
			{
				r = IRType(_Symbol_SybToIRMap.GetValue(syb.ID));
			}
			else
			{
				String DynSybName = "Dynamic" + syb.FullName;

				IRidentifierID IRid = _IR_Builder.ToID(DynSybName);
				for (auto& Item : _IR_Builder._Symbols)
				{
					if (Item->identifier == IRid)
					{
						_Symbol_SybToIRMap.AddValue(syb.ID, IRid);
						r = IRType(IRid);
						return r;
					}
				}
				auto StructIR = _IR_Builder.NewStruct(IRid);

				{
					IRStructField V;
					V.Type = IRType(IRTypes::pointer);
					StructIR->Fields.push_back(V);
				}
				{
					IRStructField V;
					V.Type = IRType(IRTypes::pointer);
					V.Type._symbol = _IR_Builder.ToID(Str_GetTraitVStructTableName(syb.FullName));
					StructIR->Fields.push_back(V);
				}

				r = IRType(IRid);
				_Symbol_SybToIRMap.AddValue(syb.ID, IRid);
			}
		}
		else
		{
			UCodeLangUnreachable();
		}
	}
	break;
	case TypesEnum::sIntPtr:
	case TypesEnum::uIntPtr:
		r = _Settings->PtrSize == IntSizes::Int32 ? IRType(IRTypes::i32) : IRType(IRTypes::i64);
		break;
	default:
		UCodeLangUnreachable();
		break;
	}

	if (Value.IsAddress() || Value.IsAddressArray() || Value.IsMovedType())
	{
		r._Type = IRTypes::pointer;
	}
	return r;
}
void SystematicAnalysis::Push_NewStackFrame()
{
	_IR_StackFrames.emplace_back();
}
void SystematicAnalysis::Pop_StackFrame()
{
	auto& TopStack = _IR_StackFrames.back();

	for (auto& Item : TopStack.OnEndStackFrame)
	{
		IR_Build_DestructorCall(Item);
	}


	_IR_StackFrames.pop_back();
}
SymbolID SystematicAnalysis::Symbol_GetSymbolID(const void* Item)
{

	auto Scope = _Table._Scope.ThisScope;

	if (!_SybIdMap.HasValue(Scope))
	{
		_SybIdMap.AddValue(Scope, {});
	}
	UnorderedMap<const void*, SymbolID>& ID = _SybIdMap.GetValue(Scope);

	if (!ID.HasValue(Item))
	{
		auto R = SymbolID(_IDIndex++);
		ID.AddValue(Item, R);
		return R;
	}
	else
	{
		return ID.GetValue(Item);
	}
}
void SystematicAnalysis::OnStatement(const Node& node2)
{
	Push_ToNodeScope(node2);
	switch (node2.Get_Type())
	{
	case NodeType::ClassNode: OnClassNode(*ClassNode::As(&node2)); break;
	case NodeType::AliasNode:OnAliasNode(*AliasNode::As(&node2)); break;
	case NodeType::EnumNode:OnEnum(*EnumNode::As(&node2)); break;
	case NodeType::UsingNode: OnUseingNode(*UsingNode::As(&node2)); break;
	case NodeType::DeclareVariableNode:OnDeclareVariablenode(*DeclareVariableNode::As(&node2), DeclareStaticVariableNode_t::Stack); break;
	case NodeType::AssignExpressionNode:OnAssignExpressionNode(*AssignExpressionNode::As(&node2)); break;
	case NodeType::PostfixVariableNode:OnPostfixVariableNode(*PostfixVariableNode::As(&node2)); break;
	case NodeType::CompoundStatementNode:OnCompoundStatementNode(*CompoundStatementNode::As(&node2)); break;
	case NodeType::FuncCallStatementNode:
	{
		TypeSymbol V(TypesEnum::Any);
		_LookingForTypes.push(V);

		OnFuncCallNode(FuncCallStatementNode::As(&node2)->_Base);

		_LookingForTypes.pop();
	}
	break;
	case NodeType::DropStatementNode:OnDropStatementNode(*DropStatementNode::As(&node2)); break;
	case NodeType::IfNode:OnIfNode(*IfNode::As(&node2)); break;
	case NodeType::WhileNode:OnWhileNode(*WhileNode::As(&node2)); break;
	case NodeType::DoNode:OnDoNode(*DoNode::As(&node2)); break;
	case NodeType::DeclareStaticVariableNode:OnDeclareStaticVariableNode(*DeclareStaticVariableNode::As(&node2)); break;
	case NodeType::DeclareThreadVariableNode:OnDeclareThreadVariableNode(*DeclareThreadVariableNode::As(&node2)); break;
	case NodeType::DeclareEvalVariableNode:OnDeclareEvalVariableNode(*DeclareEvalVariableNode::As(&node2)); break;
	case NodeType::ForNode:OnForNode(*ForNode::As(&node2)); break;
	case NodeType::ContinueNode:OnContinueNode(*ContinueNode::As(&node2)); break;
	case NodeType::BreakNode:OnBreakNode(*BreakNode::As(&node2)); break;
	case NodeType::RetStatementNode:OnRetStatement(*RetStatementNode::As(&node2)); break;
	case NodeType::TraitNode:OnTrait(*TraitNode::As(&node2)); break;
	case NodeType::TagTypeNode:OnTag(*TagTypeNode::As(&node2)); break;
	case NodeType::InvalidNode:OnInvalidNode(*InvalidNode::As(&node2)); break;
	case NodeType::CompileTimeIfNode:OnCompileTimeIfNode(*CompileTimeIfNode::As(&node2)); break;
	case NodeType::CompileTimeForNode:OnCompileTimeforNode(*CompileTimeForNode::As(&node2)); break;
	case NodeType::MatchStatement:OnMatchStatement(*MatchStatement::As(&node2)); break;
	case NodeType::AwaitStatement:OnAwaitStatement(*AwaitStatement::As(&node2)); break;
	case NodeType::YieldStatement:OnYieldStatement(*YieldStatement::As(&node2)); break;
	case NodeType::UnsafeStatementsNode:OnUnsafeStatement(*UnsafeStatementsNode::As(&node2)); break;
	case NodeType::DeferStatementNode:OnDeferStatement(*DeferStatementNode::As(&node2)); break;
	case NodeType::StatementsNode:OnStatements(*StatementsNode::As(&node2)); break;
	case NodeType::PanicNode:OnPanicStatement(*PanicNode::As(&node2)); break;
	default:UCodeLangUnreachable(); break;
	}
	Pop_NodeScope();
}
void SystematicAnalysis::OnStatements(const StatementsNode& node)
{
	String ScopeName = std::to_string((size_t)&node);

	Push_NewStackFrame();
	_Table.AddScope(ScopeName);

	for (const auto& node2 : node._Nodes)
	{
		OnStatement(*node2);
	}

	Pop_StackFrame();
	_Table.RemoveScope();
}
void SystematicAnalysis::OnRetStatement(const RetStatementNode& node)
{

	auto& LookForT = Type_Get_LookingForType();
	if (node._Expression._Value)
	{
		_FuncStack.back().IsOnRetStatemnt = true;
		//LookForT.SetAsRawValue();

		auto lookfortypecopy = LookForT;
		if (HasMoveContructerHasIRFunc(lookfortypecopy)) {
			lookfortypecopy.SetAsMoved();
		}
		_LookingForTypes.push(lookfortypecopy);

		OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);

		_LookingForTypes.pop();

		_FuncStack.back().IsOnRetStatemnt = false;
	}
	else
	{
		_LastExpressionType.SetType(TypesEnum::Void);
	}


	if (_PassType == PassType::FixedTypes)
	{
		auto& T = LookForT;

		if (T._Type != TypesEnum::Var)
		{
			bool IsPassedYield = IsAfterYield();

			if (IsPassedYield)
			{
				_LastExpressionType = Type_MakeFutureFromType(_LastExpressionType);
			}
			_LastExpressionType.SetAsMoved();

			if (!Type_CanBeImplicitConverted(_LastExpressionType, T, false))
			{
				LogError_CantCastImplicitTypes(_LastLookedAtToken.value(), _LastExpressionType, T, false);
			}
		}
	}
	else if (_PassType == PassType::BuidCode)
	{
		auto& T = Type_Get_LookingForType();

		if (HasMoveContructerHasIRFunc(T)) {
			_LastExpressionType.SetAsMoved();
		}

		IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, T);
		if (node._Expression._Value)
		{

			//Cant ret A Void Type In IR.
			if (!_FuncStack.back().Pointer->Ret.IsAn(TypesEnum::Void))
			{
				_IR_LookingAtIRBlock->NewRetValue(_IR_LastExpressionField);
			}

		}
		RetData v;
		v.JumpIns = _IR_LookingAtIRBlock->NewJump();
		_IR_Rets.push_back(std::move(v));
	}

}

String SystematicAnalysis::Str_GetScopedNameAsString(const ScopedNameNode& node)
{
	String Text;
	if (node._ScopedName.size() && node._ScopedName[0]._token->Type == TokenType::KeyWord_This)
	{
		auto Type = *_FuncStack.back().Pointer->GetObjectForCall();
		Type._IsAddress = false;

		Text += ToString(Type);

		if (node._ScopedName.size() > 1)
		{
			Text += ScopeHelper::_ScopeSep;
			node.GetScopedName(Text, 1);
		}
	}
	else {
		node.GetScopedName(Text);
	}return Text;
}

void SystematicAnalysis::IR_Build_OnStoreVarable(bool IsStructObjectPassRef, IRInstruction* OnVarable, Symbol* syb, const SymbolID& sybId)
{

	if (syb->Type == SymbolType::StaticVarable || syb->Type == SymbolType::ThreadVarable)
	{
		auto id = _IR_Builder.ToID(syb->FullName);


		if (IsStructObjectPassRef)
		{
			_IR_LookingAtIRBlock->NewStore(id, OnVarable);
		}
		else
		{
			_IR_LookingAtIRBlock->NewStore(id, _IR_LastExpressionField);
		}


		if (Symbol_HasDestructor(syb->VarType))
		{

			IRFunc* Func = syb->Type == SymbolType::StaticVarable ? &_IR_Builder._StaticdeInit : &_IR_Builder._threaddeInit;

			auto old = _IR_LookingAtIRBlock;

			if (Func->Blocks.size() == 0)
			{
				Func->NewBlock(".");
			}

			_IR_LookingAtIRBlock = Func->Blocks.front().get();


			auto Variable = _IR_LookingAtIRBlock->NewLoadPtr(id);

			ObjectToDrop V;
			V.ID = sybId;
			V._Object = Variable;
			V.DropType = ObjectToDropType::IRInstructionNoMod;
			V.Type = syb->VarType;
			IR_Build_DestructorCall(V);


			_IR_LookingAtIRBlock = old;
		}


	}
	else if (syb->Type == SymbolType::Class_Field)
	{
		auto Classinfo = _ClassStack.top().Info;
		auto classSb = _ClassStack.top().Syb;
		auto Classtype = TypeSymbol(classSb->ID);
		auto GG = classSb->Get_Info<ClassInfo>();
		auto IndexField = GG->GetFieldIndex(ScopeHelper::GetNameFromFullName(syb->FullName)).value();

		IRStruct* V = _IR_Builder.GetSymbol(IR_Build_ConvertToIRClassIR(*classSb))->Get_ExAs<IRStruct>();
		auto output = _IR_LookingAtIRBlock->New_Member_Dereference(&_IR_LookingAtIRFunc->Pars[0], IR_ConvertToIRType(classSb->ID), IndexField);

		if (IsStructObjectPassRef)
		{
			_IR_LookingAtIRBlock->NewStore(output, OnVarable);
		}
		else
		{
			_IR_LookingAtIRBlock->NewStore(output, _IR_LastExpressionField);
		}

	}
	else
	{


		if (IsStructObjectPassRef)
		{
			if (_IR_IRlocations.top().UsedlocationIR == false)
			{

				if (_IR_LastExpressionField->Type == IRInstructionType::Load
					&& _IR_LastExpressionField->Target().Type == IROperatorType::IRInstruction)
				{//to stop copying big objects
					_IR_LookingAtIRBlock->NewStore(OnVarable, _IR_LastExpressionField->Target().Pointer);
					_IR_LastExpressionField->SetAsNone();
				}
				else
				{
					_IR_LookingAtIRBlock->NewStore(OnVarable, _IR_LastExpressionField);
				}
			}
			_IR_IRlocations.pop();
		}
		else
		{
			_IR_LookingAtIRBlock->NewStore(OnVarable, _IR_LastExpressionField);
		}
		_IR_LastExpressionField = nullptr;

		IR_Build_AddDestructorToStack(syb, sybId, OnVarable);
	}
}
String SystematicAnalysis::Str_GetEnumVariantUnionName(const String& FullName)
{
	return FullName + "u";
}
void SystematicAnalysis::IR_Build_AddDestructorToStack(const NeverNullPtr<Symbol> syb, const SymbolID& sybId, IRInstruction* OnVarable)
{
	if (Symbol_HasDestructor(syb->VarType))
	{
		ObjectToDrop V;
		V.ID = sybId;
		V._Object = OnVarable;
		V.Type = syb->VarType;

		_IR_StackFrames.back().OnEndStackFrame.push_back(V);
	}
}
String SystematicAnalysis::Str_GetUnrefencedableName(const String& FullName)
{
	return "#" + FullName;
}
void SystematicAnalysis::IR_Build_AddDestructorToStack(const TypeSymbol& Type, IRInstruction* OnVarable)
{
	if (Symbol_HasDestructor(Type))
	{
		ObjectToDrop V;
		V._Object = OnVarable;
		V.Type = Type;
		V.DropType = ObjectToDropType::IRInstruction;

		_IR_StackFrames.back().OnEndStackFrame.push_back(V);
	}
}
void SystematicAnalysis::Type_DeclareVariableTypeCheck(TypeSymbol& VarType, const TypeSymbol& Ex, const NeverNullPtr<Token> Token)
{
	if (VarType._Type == TypesEnum::Var)
	{
		if (Ex._Type == TypesEnum::Var)
		{
			LogError_CantguessVarTypeError(Token);
		}
		else
		{
			bool WasImutable = VarType.Isimmutable();
			bool WasIsAddress = VarType.IsAddress();
			bool WasIsAddressArry = VarType.IsAddressArray();
			auto OldTypeInfo = VarType._TypeInfo;

			VarType = Ex;

			if (WasImutable) { VarType.SetAsimmutable(); }
			if (WasIsAddress) { VarType.SetAsAddress(); }
			else { VarType._IsAddress = false; }
			if (WasIsAddressArry) { VarType.SetAsAddressArray(); }

			if (OldTypeInfo == TypeInfoPrimitive::Null)
			{
				VarType._TypeInfo = OldTypeInfo;
			}
			VarType._MoveData = MoveData::None;
			VarType.SetAsLocation();


		}
	}


	if (!Type_CanBeImplicitConverted(Ex, VarType, false, true))
	{
		LogError_CantCastImplicitTypes(Token, Ex, VarType, false, true);
	}
}
void SystematicAnalysis::Type_DeclareVarableCheck(TypeSymbol& VarType, const Node* Ex, const NeverNullPtr<Token> Token)
{
	if (VarType._Type == TypesEnum::Var && Ex == nullptr)
	{
		LogError_CantgussTypesTheresnoassignment(Token);
	}
	else if (VarType._Type == TypesEnum::CustomType && Ex == nullptr)
	{
		auto Syb = Symbol_GetSymbol(VarType).value();
		if (Syb->Type == SymbolType::Type_StaticArray)
		{
			StaticArrayInfo* V = Syb->Get_Info<StaticArrayInfo>();

			if (!V->IsCountInitialized)
			{
				LogError_BeMoreSpecifiicWithStaticArrSize(Token, V->Type);
			}
		}
	}
}
void SystematicAnalysis::LogError_UseDotAndNotScopeResolution(const NeverNullPtr<Symbol>& Sym, const NeverNullPtr<Token>& ItemToken)
{
	LogError(ErrorCodes::InValidName, "Use '.' and not '::' to use " + ToString(Sym->Type) + "'" + Sym->FullName + "'", ItemToken);
}
void SystematicAnalysis::LogError_UseScopeResolutionAndNotDot(const NeverNullPtr<Symbol>& Sym, const NeverNullPtr<Token>& ItemToken)
{
	LogError(ErrorCodes::InValidName, "Use '::' and not '.' to use " + ToString(Sym->Type) + "'" + Sym->FullName + "'", ItemToken);
}

NullablePtr<Symbol> SystematicAnalysis::GetTepFuncPtrSyb(const String& TepFuncPtr, const FuncInfo* Finfo)
{
	NullablePtr<Symbol> V2 = Symbol_GetSymbol(TepFuncPtr, SymbolType::Func_ptr);
	if (!V2.has_value())
	{
		NeverNullPtr<Symbol> V = &Symbol_AddSymbol(SymbolType::Func_ptr, TepFuncPtr, Finfo->FullName, AccessModifierType::Public);
		FuncPtrInfo* V2 = new FuncPtrInfo();
		V->Info.reset(V2);

		SymbolID VID = Symbol_GetSymbolID(V.value());

		V2->FullName = Finfo->FullName;

		V2->Pars = Finfo->Pars;
		V2->Ret = Finfo->Ret;
		_Table.AddSymbolID(*V, VID);

		return V.AsNullable();
	}

	return V2;
}
bool SystematicAnalysis::Type_IsTypeExported(TypeSymbol type)
{
	if (type._Type == TypesEnum::Null)
	{
		return true;
	}

	Type_RemoveTypeattributes(type);


	if (Type_IsPrimitive(type))
	{
		return true;
	}

	auto symop = Symbol_GetSymbol(type);
	if (symop.has_value())
	{
		auto sym = symop.value();
		bool hasnode = sym->NodePtr;

		if (!hasnode)
		{
			//Most likely generate from enum
			return true;
		}
		if (Type_IsUnMapType(type))
		{
			return true;
		}

		switch (sym->Type)
		{
		case SymbolType::Type_class:
		{
			return sym->Get_NodeInfo<ClassNode>()->_IsExport;
		}
		break;
		case SymbolType::Enum:
		{
			return sym->Get_NodeInfo<EnumNode>()->_IsExport;
		}
		break;
		case SymbolType::Trait_class:
		{
			return sym->Get_NodeInfo<TraitNode>()->_IsExport;
		}
		break;
		case SymbolType::Hard_Type_alias:
		{
			return sym->Get_NodeInfo<AliasNode>()->IsExport;
		}
		break;
		default:
			UCodeLangUnreachable();
			break;
		}

	}


	return false;
}

#define TepFuncPtrNameMangleStr "_tepfptr|"
String SystematicAnalysis::GetTepFuncPtrName(const FuncInfo* SymbolVar)
{
	return TepFuncPtrNameMangleStr + SymbolVar->FullName;
}
String_view SystematicAnalysis::GetTepFuncPtrNameAsName(const String_view Str)
{
	return Str.substr(sizeof(TepFuncPtrNameMangleStr) - 1);//remove null char
}
void SystematicAnalysis::OnExpressionTypeNode(const Node* node, GetValueMode Mode)
{
	_GetExpressionMode.push(Mode);

	Push_ToNodeScope(*node);

	switch (node->Get_Type())
	{
	case NodeType::BinaryExpressionNode:OnExpressionNode(*BinaryExpressionNode::As(node)); break;
	case NodeType::ValueExpressionNode:OnExpressionNode(*ValueExpressionNode::As(node)); break;
	case NodeType::UnaryExpressionNode:OnExpressionNode(*UnaryExpressionNode::As(node)); break;
	case NodeType::CastNode:OnExpressionNode(*CastNode::As(node)); break;
	case NodeType::IndexedExpresionNode:OnExpressionNode(*IndexedExpresionNode::As(node)); break;
	case NodeType::ExtendedScopeExpression:OnExpressionNode(*ExtendedScopeExpression::As(node)); break;
	case NodeType::ExtendedFuncExpression:OnExpressionNode(*ExtendedFuncExpression::As(node)); break;
	default:
		UCodeLangUnreachable();
		break;
	}

	Pop_NodeScope();
	_GetExpressionMode.pop();
}
void SystematicAnalysis::OnExpressionNode(const ValueExpressionNode& node)
{
	/*
	if (_PassType == PassType::BuidCode
		|| _PassType == PassType::FixedTypes

		|| (_PassType == PassType::GetTypes &&
		(
			(node._Value->Get_Type() == NodeType::LambdaNode) ||
			(node._Value->Get_Type() == NodeType::YieldExpression) ||
			(node._Value->Get_Type() == NodeType::AwaitExpression)
		))
	   )
	*/
	{
		const auto Value = node._Value.get();
		switch (node._Value->Get_Type())
		{
		case NodeType::NumberliteralNode:
		{
			const 	auto num = NumberliteralNode::As(Value);

			OnNumberliteralNode(num);
		}
		break;
		case NodeType::BoolliteralNode:
		{
			const 	auto num = BoolliteralNode::As(Value);

			OnBoolliteralNode(num);
		}
		break;
		case NodeType::CharliteralNode:
		{
			const 	auto num = CharliteralNode::As(Value);

			OnCharliteralNode(num);
		}
		break;
		case NodeType::FloatliteralNode:
		{
			const 	auto num = FloatliteralNode::As(Value);
			OnFloatLiteralNode(num);
		}
		break;
		case NodeType::StringliteralNode:
		{
			const 	auto nod = StringliteralNode::As(Value);

			bool retflag;
			OnStringLiteral(nod, retflag);
			if (retflag) return;
		}break;
		case NodeType::ReadVariableNode:
		{
			const 	auto nod = ReadVariableNode::As(Value);
			OnReadVariable(*nod);
		}
		break;
		case NodeType::AnonymousObjectConstructorNode:
		{
			const 	auto nod = AnonymousObjectConstructorNode::As(Value);
			OnAnonymousObjectConstructor(nod);
		}
		break;
		case NodeType::FuncCallNode:
		{
			OnFuncCallNode(*FuncCallNode::As(Value));
		}
		break;
		case NodeType::SizeofExpresionNode:
		{
			const 	auto nod = SizeofExpresionNode::As(Value);

			OnSizeofNode(nod);
		}
		break;
		case NodeType::NewExpresionNode:
		{
			const 	auto nod = NewExpresionNode::As(Value);

			OnNewNode(nod);
		}
		break;
		case NodeType::ParenthesesExpresionNode:
		{
			const ParenthesesExpresionNode* nod = ParenthesesExpresionNode::As(Value);

			OnExpressionTypeNode(nod->_Expression._Value.get(), GetValueMode::Read);
		}
		break;
		case NodeType::MoveNode:
		{
			const MoveNode* nod = MoveNode::As(Value);

			OnMovedNode(nod);
		}
		break;
		case NodeType::LambdaNode:
		{
			const  auto nod = LambdaNode::As(Value);
			OnLambdaNode(*nod);
		}
		break;
		case NodeType::BitCastExpression:
		{
			const auto nod = BitCastExpression::As(Value);
			OnBitCast(*nod);
		}
		break;
		case NodeType::ValidNode:
		{
			const 	auto nod = ValidNode::As(Value);
			OnvalidNode(*nod);
		}
		break;
		case NodeType::CMPTypesNode:
		{
			const 	auto nod = CMPTypesNode::As(Value);
			OnCompareTypesNode(*nod);
		}
		break;
		case NodeType::MatchExpression:
		{
			const auto nod = MatchExpression::As(Value);
			OnMatchExpression(*nod);
		}
		break;
		case NodeType::TypeToValueNode:
		{
			const auto nod = TypeToValueNode::As(Value);
			OnTypeToValueNode(*nod);
		}
		break;
		case NodeType::ExpressionToTypeValueNode:
		{
			const auto nod = ExpressionToTypeValueNode::As(Value);
			OnExpressionToTypeValueNode(*nod);
		}
		break;
		case NodeType::AwaitExpression:
		{
			const auto nod = AwaitExpression::As(Value);
			OnAwaitExpression(*nod);
		}
		break;
		case NodeType::YieldExpression:
		{
			const auto nod = YieldExpression::As(Value);
			OnYieldExpression(*nod);
		}
		break;
		case NodeType::UnsafeExpression:
		{
			const auto nod = UnsafeExpression::As(Value);
			OnUnsafeExpression(*nod);
		}
		break;
		default:
			UCodeLangUnreachable();
			break;
		}
	}
}

bool SystematicAnalysis::Type_Compare(const TypeSymbol& Type0, const TypeSymbol& Type1, const NeverNullPtr<Token> Value)
{
	bool CMPValue = false;
	switch (Value->Type)
	{
	case TokenType::equal_Comparison:
		CMPValue = Type_AreTheSame(Type0, Type1);
		break;
	case TokenType::Notequal_Comparison:
		CMPValue = !Type_AreTheSame(Type0, Type1);
		break;
	case TokenType::approximate_Comparison:
		CMPValue = Type_AreTheSameWithOutimmutable(Type0, Type1);
		break;
	default:
		break;
	}
	return CMPValue;
}

void SystematicAnalysis::OnCompareTypesNode(const CMPTypesNode& node)
{
	if (_PassType == PassType::GetTypes)
	{

	}
	if (_PassType == PassType::FixedTypes)
	{
		TypeSymbol Type0 = Type_ConvertAndValidateType(node._TypeOp0, NodeSyb_t::Any);
		TypeSymbol Type1 = Type_ConvertAndValidateType(node._TypeOp1, NodeSyb_t::Any);


		bool CMPValue = Type_Compare(Type0, Type1, NeverNullptr(node._Op));


		_ValidNodes.AddValue(Symbol_GetSymbolID(node), CMPValue);
	}
	if (_PassType == PassType::BuidCode)
	{
		_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad(_ValidNodes.GetValue(Symbol_GetSymbolID(node)));
	}

	_LastExpressionType.SetType(TypesEnum::Bool);
}

void SystematicAnalysis::OnMovedNode(const MoveNode* nod)
{

	auto newtype = Type_Get_LookingForType();
	newtype.SetAsMoved();
	_LookingForTypes.push(newtype);

	OnExpressionTypeNode(nod->_expression._Value.get(), GetValueMode::Read);

	_LookingForTypes.pop();

	auto ExType = _LastExpressionType;
	ExType.SetAsMoved();
	_LastExpressionType = ExType;
}

bool SystematicAnalysis::Type_IsStaticCharArr(const TypeSymbol& Type)
{
	if (Type._Type == TypesEnum::CustomType)
	{
		auto V = Symbol_GetSymbol(Type).value();
		if (V->Type == SymbolType::Type_StaticArray)
		{
			StaticArrayInfo* StaticArr = V->Get_Info< StaticArrayInfo>();

			TypeSymbol CharType;
			CharType.SetType(TypesEnum::Char);

			return Type_AreTheSame(CharType, StaticArr->Type);
		}
	}
	return false;
}

void SystematicAnalysis::Type_SetFuncRetAsLastEx(const Get_FuncInfo& Info)
{
	if (Info.Func)
	{
		if (Symbol_IsVarableType(Info.SymFunc->Type))
		{
			_LastExpressionType = ((FuncPtrInfo*)Info.Func)->Ret;
		}
		else
		{
			if (Info.Func->_FuncType == FuncInfo::FuncType::New)
			{
				_LastExpressionType = (*Info.Func->GetObjectForCall());
				_LastExpressionType._IsAddress = false;
				_LastExpressionType._ValueInfo = TypeValueInfo::IsValue;
			}
			else
			{
				_LastExpressionType = Info.Func->Ret;
			}
		}
	}
	else if (Info._BuiltFunc.has_value())
	{
		auto& Item = Info._BuiltFunc.value();
		if (Item.EvalAsCString)
		{
			_LastExpressionType = TypesEnum::Char;
			_LastExpressionType.SetAsAddressArray();
			_LastExpressionType.SetAsimmutable();
		}
		else
		{
			_LastExpressionType = Info._BuiltFunc.value().RetType;
		}
	}
	else if (Info.CantCheckBecauseIsUnMaped)
	{
		_LastExpressionType = Type_GetUnMapType();
	}
	else if (Info.SymFunc && Info.SymFunc->Type == SymbolType::Enum_Field)
	{
		if (Info.ThisPar == Get_FuncInfo::ThisPar_t::NoThisPar_GetValue_EnumVariant)
		{
			_LastExpressionType = TypesEnum::Bool;
		}
		else
		{
			_LastExpressionType = Symbol_GetSymbol(ScopeHelper::GetReMoveScope(Info.SymFunc->FullName), SymbolType::Enum).value()->VarType;

		}
	}
	else if (Info.BulitInTypeContructer.has_value())
	{
		_LastExpressionType = Info.BulitInTypeContructer.value();
	}
	else
	{
		_LastExpressionType = TypesEnum::Null;
	}
}
NullablePtr<Symbol> SystematicAnalysis::Symbol_GetSymbol(String_view Name, SymbolType Type)
{
	auto& Symbols = GetSymbolsWithName(Name, Type);
	auto Symbol = Symbols.size() ? Symbols[0] : nullptr;
	return Symbol;
}
const NullablePtr<Symbol> SystematicAnalysis::Symbol_GetSymbol(String_view Name, SymbolType Type) const
{
	return  const_cast<SystematicAnalysis*>(this)->Symbol_GetSymbol(Name, Type);
}

void SystematicAnalysis::OnTypeToValueNode(const TypeToValueNode& node)
{

	if (_PassType == PassType::FixedTypes)
	{

		auto Type = Type_ConvertAndValidateType(node.TypeOp, NodeSyb_t::Any);
		Type.SetAsTypeInfo();


		_LastExpressionType = Type;
	}
	else if (_PassType == PassType::BuidCode)
	{

		const NeverNullPtr<Token> Token = NeverNullptr(node.TypeOp._name._ScopedName.back()._token);
		LogError_CantOutputTypeinfo(Token);
	}
}

void SystematicAnalysis::OnExpressionToTypeValueNode(const ExpressionToTypeValueNode& node)
{
	if (_PassType == PassType::GetTypes)
	{
		_LookingForTypes.push(TypesEnum::Any);
		OnExpressionTypeNode(node._TypeEx, GetValueMode::Read);
		_LookingForTypes.pop();
	}
	else if (_PassType == PassType::FixedTypes)
	{
		_LookingForTypes.push(TypesEnum::Any);
		OnExpressionTypeNode(node._TypeEx, GetValueMode::Read);
		_LookingForTypes.pop();

		auto Type = _LastExpressionType;
		Type.SetAsTypeInfo();

		_LastExpressionType = Type;
	}
	else if (_PassType == PassType::BuidCode)
	{
		const NeverNullPtr<Token> Token = _LastLookedAtToken.value();
		LogError_CantOutputTypeinfo(Token);
	}
}

String SystematicAnalysis::Str_GetFuncAnonymousObjectFullName(const String& FullFuncName)
{
	return FullFuncName + "!";
}


String SystematicAnalysis::ToString(const TypeSymbol& Type) const
{
	String r;

	if (Type._Type == TypesEnum::InternalType)
	{
		r += CompilerGenerated("InternalType");
		if (Type._TypeInfo == TypeInfoPrimitive::ClassFieldInfo)
		{
			r += "::ClassFieldInfo";
		}
		else
			if (Type._TypeInfo == TypeInfoPrimitive::EnumFieldInfo)
			{
				r += "::EnumFieldInfo";
			}
		return r;
	}

	if (Type.Isimmutable())
	{
		r = "imut ";
	}

	if (Type._MoveData == MoveData::Moved)
	{
		r += "moved ";
	}

	if (Type.IsTypeInfo())
	{
		r += "typeinfo<";
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
	case TypesEnum::Uft8:r += Uft8typeName;	break;
	case TypesEnum::Uft16:r += Uft16typeName;	break;
	case TypesEnum::Uft32:r += Uft32typeName;	break;


	case TypesEnum::float32:r += float32TypeName;	break;
	case TypesEnum::float64:r += float64TypeName;	break;
	case TypesEnum::CustomType:
	{
		auto& Syb = *Symbol_GetSymbol(Type._CustomTypeSymbol);
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
				r += ToString(Item);
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
			const StaticArrayInfo* Info = Syb.Get_Info<StaticArrayInfo>();
			r += ToString(Info->Type);
			r += "[/";
			r += std::to_string(Info->Count);
			r += "]";
		}
		else if (Syb.Type == SymbolType::ConstantExpression)
		{
			const ConstantExpressionInfo* Info = Syb.Get_Info<ConstantExpressionInfo>();
			r += "(" + ToString(Syb.VarType);
			r += ";";
			r += ToString(Syb.VarType, Info->Ex) + ")";
		}
		else if (Syb.Type == SymbolType::Type_Pack)
		{
			const TypePackInfo* Info = Syb.Get_Info<TypePackInfo>();
			r += "TypePack[";
			//+ToString(Syb.VarType);
			for (auto& Item : Info->List)
			{
				r += ToString(Item);
				if (&Item != &Info->List.back())
				{
					r += ",";
				}
			}
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
		UCodeLangUnreachable();
		break;
	}

	if (Type._IsDynamic)
	{
		r += ">";
	}

	if (Type.IsTypeInfo())
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

TypeSymbol SystematicAnalysis::Type_GetStaticArrayType(const TypeSymbol& BaseType, size_t Size)
{
	auto BaseTypeName = ToString(BaseType);
	auto FullName = CompilerGenerated("StaticArray_") + BaseTypeName + std::to_string(Size);
	auto Syb = Symbol_GetSymbol(FullName, SymbolType::Type);
	if (Syb == nullptr)
	{
		Syb = &Symbol_AddSymbol(SymbolType::Type_StaticArray, FullName, FullName, AccessModifierType::Public);
		SymbolID id = Symbol_GetSymbolID(Syb.value().value());
		_Table.AddSymbolID(*Syb.value(), id);

		Syb.value()->PassState = _PassType;

		StaticArrayInfo* info = new StaticArrayInfo();


		info->Type = BaseType;
		info->Count = Size;

		Syb.value()->Info.reset(info);
	}
	return TypeSymbol(Syb.value()->ID);
}

IRInstruction* SystematicAnalysis::IR_RawObjectDataToCString(const RawEvaluatedObject& EvalObject)
{
	String Str = String(String_view((const char*)EvalObject.Object_AsPointer.get(), EvalObject.ObjectSize));
	String_view Buffer{ Str.data(),Str.size() + 1 };//for null char

	auto BufferIR = _IR_Builder.FindOrAddStaticCString(Buffer);
	return _IR_LookingAtIRBlock->NewLoadPtr(BufferIR);
}

SystematicAnalysis::CastOverLoadWith_t  SystematicAnalysis::Type_CanBeExplicitlyConverted(const TypeSymbol& TypeToCheck, const TypeSymbol& Type)
{
	if (Type_CanBeImplicitConverted(TypeToCheck, Type)) { return { true }; }

	if (TypeToCheck._Type == TypesEnum::CustomType)
	{
		Symbol& syb = *Symbol_GetSymbol(TypeToCheck._CustomTypeSymbol);
		if (syb.Type == SymbolType::Hard_Type_alias)
		{
			if (Type_AreTheSameWithOutimmutable(syb.VarType, Type))
			{
				return { true };
			}
		}

	}
	else if (Type._Type == TypesEnum::CustomType)
	{
		Symbol& syb = *Symbol_GetSymbol(Type._CustomTypeSymbol);
		if (syb.Type == SymbolType::Hard_Type_alias)
		{
			if (Type_AreTheSameWithOutimmutable(syb.VarType, TypeToCheck))
			{
				return { true };
			}
		}

	}

	if (Type_IsIntType(TypeToCheck) && Type_IsIntType(Type)) { return { true }; }
	if (TypeToCheck._Type == TypesEnum::float32 && Type._Type == TypesEnum::sInt32) { return { true }; }
	if (TypeToCheck._Type == TypesEnum::float64 && Type._Type == TypesEnum::sInt64) { return { true }; }

	if (TypeToCheck._Type == TypesEnum::sInt32 && Type._Type == TypesEnum::float32) { return { true }; }
	if (TypeToCheck._Type == TypesEnum::sInt64 && Type._Type == TypesEnum::float64) { return { true }; }
	if (TypeToCheck._Type == TypesEnum::uInt32 && Type._Type == TypesEnum::float32) { return { true }; }
	if (TypeToCheck._Type == TypesEnum::uInt64 && Type._Type == TypesEnum::float64) { return { true }; }

	auto Syb = Symbol_GetSymbol(TypeToCheck);
	if (Syb)
	{
		if (Syb.value()->Type == SymbolType::Type_class)
		{

			String funcName = Syb.value()->FullName;
			ScopeHelper::GetApendedString(funcName, Overload_Cast_Func);

			auto& V = GetSymbolsWithName(funcName, SymbolType::Func);

			for (auto& Item : V)
			{
				if (Item->Type == SymbolType::Func)
				{
					auto funcInfo = Item->Get_Info<FuncInfo>();
					if (funcInfo->Pars.size() == 1)
					{
						bool r = Type_AreTheSame(Type, funcInfo->Ret)
							&& Type_CanBeImplicitConverted(TypeToCheck, funcInfo->Pars[0].Type);
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
Optional < FuncInfo*> SystematicAnalysis::Symbol_GetAnExplicitlyConvertedFunc(const TypeSymbol& TypeToCheck)
{

	auto Syb = Symbol_GetSymbol(TypeToCheck);
	if (Syb)
	{
		if (Syb.value()->Type == SymbolType::Type_class)
		{

			String funcName = Syb.value()->FullName;
			ScopeHelper::GetApendedString(funcName, Overload_Cast_Func);

			auto& V = GetSymbolsWithName(funcName, SymbolType::Func);

			for (auto& Item : V)
			{
				if (Item->Type == SymbolType::Func)
				{
					auto funcInfo = Item->Get_Info<FuncInfo>();
					if (funcInfo->Pars.size() == 1)
					{
						bool r = Type_CanBeImplicitConverted(TypeToCheck, funcInfo->Pars[0].Type);
						if (r)
						{
							return  funcInfo;
						}

					}
				}
			}
		}
	}
	return { nullptr };
}
bool SystematicAnalysis::HasMoveContructerHasIRFunc(const TypeSymbol& ExType)
{
	auto GetSymOp = Symbol_GetSymbol(ExType);
	if (GetSymOp.has_value())
	{
		auto GetSym = GetSymOp.value();
		if (GetSym->Type == SymbolType::Type_class)
		{
			auto info = GetSym->Get_Info<ClassInfo>();

			if (info->_ClassHasMoveConstructor)
			{
				return true;
			}
		}
	}
	return false;
}

bool SystematicAnalysis::CheckForGenericInputIsConstantExpression(const FuncInfo* Info, const Vector<TypeSymbol>& GenericInput)
{
	for (size_t i = 0; i < GenericInput.size(); i++)
	{
		const auto& Tnode = GenericInput[i];

		if (i < Info->_GenericData._Genericlist.size())
		{
			const auto& GenericInfo = Info->_GenericData._Genericlist[i];

			bool InputTypeIsConstantExpression = false;

			auto TypeSyb = Symbol_GetSymbol(GenericInfo.SybID);


			InputTypeIsConstantExpression = TypeSyb->Type == SymbolType::ConstantExpression;


			if (InputTypeIsConstantExpression != GenericInfo.IsConstantExpression())
			{
				return true;
			}
		}
	}
	return false;
}
void SystematicAnalysis::Type_RemoveTypeattributes(TypeSymbol& tep_)
{
	if (tep_.IsMovedType())
	{
		tep_._MoveData = MoveData::None;
	}
	if (tep_.Isimmutable())
	{
		tep_._Isimmutable = false;
	}
	if (tep_.IsAddress())
	{
		tep_._IsAddress = false;
	}
	if (tep_.IsAddressArray())
	{
		tep_._IsAddressArray = false;
	}
}
String SystematicAnalysis::Generic_GetGenericExtendedErrValue(const Generic& Generic, const GenericValuesNode& GenericAsNode, const Vector<TypeSymbol>& GenericInput)
{
	String V = "Were ";


	bool IsPack = Generic.IsPack();
	size_t LoopSize = IsPack ? Generic._Genericlist.size() - 1 : Generic._Genericlist.size();
	for (size_t i = 0; i < LoopSize; i++)
	{
		const auto& Item = GenericInput[i];
		bool IsExpression = Generic._Genericlist[i].IsConstantExpression();

		const auto& GenericName = GenericAsNode._Values[i].token->Value._String;
		if (IsExpression)
		{
			Symbol* Syb = Symbol_GetSymbol(Item).value_unchecked();
			ConstantExpressionInfo* Info = Syb->Get_Info<ConstantExpressionInfo>();
			V += "(" + (String)GenericName + ") = " + ToString(Syb->VarType, Info->Ex);
		}
		else
		{
			V += (String)GenericName + " = " + ToString(Item);
		}

		if (i + 1 < LoopSize || IsPack)
		{
			V += ",";
		}
	}
	if (IsPack)
	{
		//auto Item = GenericInput.
		const auto& PackName = GenericAsNode._Values.back().token->Value._String;
		V += "[" + (String)PackName + "] = [";

		size_t Index = Generic._Genericlist.size() - 1;
		for (size_t i = Index; i < GenericInput.size(); i++)
		{
			auto& Item = GenericInput[i];

			auto Sybv = Symbol_GetSymbol(Item);
			if (Sybv.has_value())
			{
				auto Syb = Sybv.value();
				if (Syb->Type == SymbolType::ConstantExpression)
				{
					ConstantExpressionInfo* Info = Syb->Get_Info<ConstantExpressionInfo>();
					V += "(" + ToString(Syb->VarType, Info->Ex) + ")";
				}
				else
				{
					V += ToString(Item);
				}
			}
			else
			{
				V += ToString(Item);
			}

			if (i + 1 < GenericInput.size())
			{
				V += ",";
			}
		}

		V += "]";
	}
	return V;
}
Optional<SymbolID>  SystematicAnalysis::Generic_MakeTypePackSymbolIfNeeded(const String& NewName, const Vector<TypeSymbol>& GenericInput, const Generic& Generic)
{
	if (Generic.IsPack())
	{
		size_t Index = Generic._Genericlist.size() - 1;
		Vector<TypeSymbol> _PackList;

		_PackList.resize(GenericInput.size() - Index);

		size_t V = 0;
		for (size_t i = Index; i < GenericInput.size(); i++)
		{
			auto& Item = GenericInput[i];
			_PackList[V] = Item;
			V++;
		}



		auto& PackSyb = Symbol_AddSymbol(SymbolType::Type_Pack, "!Pack", ScopeHelper::ApendedStrings(NewName, "!Pack"), AccessModifierType::Public);
		_Table.AddSymbolID(PackSyb, Symbol_GetSymbolID(&PackSyb));
		TypePackInfo* PackInfo = new TypePackInfo();
		PackInfo->List = std::move(_PackList);

		PackSyb.Info.reset(PackInfo);

		return PackSyb.ID;
	}
	return {};
}

bool SystematicAnalysis::Symbol_AccessCheck(const NeverNullPtr<Symbol> Syb, const NeverNullPtr<Token> Token, const String_view Scope)
{
	if (Syb->Access == AccessModifierType::Private)
	{

		auto Str1 = ScopeHelper::GetReMoveScope(Syb->FullName);
		auto Str2 = ScopeHelper::GetReMoveScope(Scope);


		auto CmpStr = Str1.substr(0, Str2.size());
		bool IsOk = (Str1 == Scope) || Str1 == CmpStr;


		if (!IsOk)
		{
			LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos, "Cant use the Symbol '" + Syb->FullName + "' its Private");
		}
		return !IsOk;
	}
	return false;
}

String SystematicAnalysis::ToString(const TypeSymbol& Type, const RawEvaluatedObject& Data) const
{
	auto DataPtr = Eval_Get_Object(Type, Data);



	switch (Type._Type)
	{
	case TypesEnum::sInt8:return std::to_string(*(const Int8*)DataPtr);
	case TypesEnum::sInt16:return std::to_string(*(const Int16*)DataPtr);

	Sint32Case:
	case TypesEnum::sInt32:return std::to_string(*(const Int32*)DataPtr);

	Sint64Case:
	case TypesEnum::sInt64:return std::to_string(*(const Int64*)DataPtr);
	case TypesEnum::uInt8:return std::to_string(*(const UInt8*)DataPtr);
	case TypesEnum::uInt16:return std::to_string(*(const UInt16*)DataPtr);

	Uint32Case:
	case TypesEnum::uInt32:return std::to_string(*(const UInt32*)DataPtr);

	Uint64Case:
	case TypesEnum::uInt64:return std::to_string(*(const UInt64*)DataPtr);
	case TypesEnum::Char:return String(*(const char*)DataPtr, 1);
	case TypesEnum::Bool:return *(bool*)DataPtr ? "true" : "false";

	case TypesEnum::InternalType:
	{
		if (Type._TypeInfo == TypeInfoPrimitive::ClassFieldInfo)
		{
			auto Value = (const Systematic_BuiltInFunctions::ClassField*)DataPtr;
			return Value->_ClassInfo->FullName + ":" + Value->Field->Name;
		}

		return "???";
	}
	case TypesEnum::uIntPtr:
		if (_Settings->PtrSize == IntSizes::Int64)
		{
			goto Uint64Case;
		}
		else
		{
			goto Uint32Case;
		}
	case TypesEnum::sIntPtr:
		if (_Settings->PtrSize == IntSizes::Int64)
		{
			goto Sint64Case;
		}
		else
		{
			goto Sint32Case;
		}
	case TypesEnum::CustomType:
	{
		auto syb = Symbol_GetSymbol(Type).value();

		if (syb->Type == SymbolType::Enum)
		{
			EnumInfo* info = syb->Get_Info<EnumInfo>();

			//UCodeLangAssert(Data.ObjectSize == Type_GetSize(Type));

			for (auto& Item : info->Fields)
			{
				if (memcmp(Item.Ex.Object_AsPointer.get(), Data.Object_AsPointer.get(), Data.ObjectSize) == 0)
				{
					return ScopeHelper::ApendedStrings(syb->FullName, Item.Name);
				}
			}
		}

	}break;
	default:
		break;
	}

	String CompilerRet = "{";
	for (size_t i = 0; i < Data.ObjectSize; i++)
	{
		char V = ((const char*)DataPtr)[i];
		CompilerRet += std::to_string((Byte)V);

		if (i != Data.ObjectSize - 1)
		{
			CompilerRet += ",";
		}
	}
	CompilerRet += "}";

	return CompilerRet;
}

void SystematicAnalysis::TryLogError_OnWritingVar(NeverNullPtr<Symbol> Symbol, const NeverNullPtr<Token> Token, const String_view Name,GetValueMode mode)
{
	if (mode != GetValueMode::WritePointerReassment) {
		if (Symbol->VarType.Isimmutable())
		{
			LogError_CantModifyiMutableError(Token, Name);
		}
	}
}

void SystematicAnalysis::LogError_LogInvalidNodeError(const NeverNullPtr<Token> Token, String_view ErrStr)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "Invaild:" + (String)ErrStr);
}
void SystematicAnalysis::LogError_EmptyInvalidError(const NeverNullPtr<Token> Token)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "Reached Invaild Statemet");
}

void SystematicAnalysis::LogError_CantCastImplicitTypes(const NeverNullPtr<Token> Token, const TypeSymbol& Ex1Type, const TypeSymbol& UintptrType, bool ReassignMode, bool isdeclare)
{
	if (Ex1Type.IsBadType() || UintptrType.IsBadType()
		|| Type_IsUnMapType(UintptrType) || Type_IsUnMapType(Ex1Type)) {
		return;
	}

	bool V1 = Type_IsAddessAndLValuesRulesfollowed(Ex1Type, UintptrType, ReassignMode, isdeclare);
	if (!V1 || Type_CanDoTypeToTrait(Ex1Type, UintptrType))
	{
		LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
			, "The expression is not an Location in memory'");
	}
	else if (
		Ex1Type._ValueInfo != TypeValueInfo::IsValue
		&& !Ex1Type.IsMovedType() && (!Type_IsCopyable(Ex1Type))
		&& !UintptrType.IsAddress()
		)
	{
		LogError_TypeIsNotCopyable(Token, Ex1Type);
	}
	else
	{
		LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
			, "Cant Implicitly cast Type '" + ToString(Ex1Type) + "' to '" + ToString(UintptrType) + "'");
	}
}
void SystematicAnalysis::LogError_ReadingFromInvaidVariable(const NeverNullPtr<Token> Token, String_view Str)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "the variable named '" + (String)Str + "'" + " can't be read from you.can not read an invaid variable");
}
void SystematicAnalysis::LogError_CantFindVarError(const NeverNullPtr<Token> Token, String_view Str)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Cant find Variable Named '" + (String)Str + "'");
}
void SystematicAnalysis::LogError_CantFindVarMemberError(const NeverNullPtr<Token> Token, String_view Str, const TypeSymbol& OnType)
{
	if (OnType.IsBadType() || Type_IsUnMapType(OnType)) { return; }

	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Cant find Member Named '" + (String)Str + "' on type '" + ToString(OnType) + "'");
}

void SystematicAnalysis::LogError_CantFindCompoundOpForTypes(const NeverNullPtr<Token> BinaryOp, TypeSymbol& Ex0Type, TypeSymbol& Ex1Type)
{
	if (Ex1Type.IsBadType() || Ex0Type.IsBadType()
		|| Type_IsUnMapType(Ex0Type) || Type_IsUnMapType(Ex1Type)) {
		return;
	}

	LogError(ErrorCodes::InValidType, BinaryOp->OnLine, BinaryOp->OnPos,
		"The type '" + ToString(Ex0Type) + "'" + " can't be '"
		+ ToString(BinaryOp->Type) + "' with '" + ToString(Ex1Type) + "'");
}

void SystematicAnalysis::LogError_CantFindPostfixOpForTypes(const NeverNullPtr<Token> BinaryOp, TypeSymbol& Ex0Type)
{
	if (Ex0Type.IsBadType() || Type_IsUnMapType(Ex0Type)) { return; }

	LogError(ErrorCodes::InValidType, BinaryOp->OnLine, BinaryOp->OnPos,
		"The type '" + ToString(Ex0Type) + "'" + " can't be '"
		+ ToString(BinaryOp->Type) + "'");
}
void SystematicAnalysis::LogError_CantFindBinaryOpForTypes(const NeverNullPtr<Token> BinaryOp, TypeSymbol& Ex0Type, TypeSymbol& Ex1Type)
{
	if (Ex1Type.IsBadType() || Ex0Type.IsBadType()
		|| Type_IsUnMapType(Ex1Type) || Type_IsUnMapType(Ex0Type)) {
		return;
	}

	LogError(ErrorCodes::InValidType, BinaryOp->OnLine, BinaryOp->OnPos,
		"The type '" + ToString(Ex0Type) + "'" + " can't be '"
		+ ToString(BinaryOp->Type) + "' with '" + ToString(Ex1Type) + "'");
}
void SystematicAnalysis::LogError_CantFindUnaryOpForTypes(const NeverNullPtr<Token> BinaryOp, TypeSymbol& Ex0Type)
{
	if (Ex0Type.IsBadType() || Type_IsUnMapType(Ex0Type)) { return; }

	LogError(ErrorCodes::InValidType, BinaryOp->OnLine, BinaryOp->OnPos,
		"The type '" + ToString(Ex0Type) + "'" + " can't be '"
		+ ToString(BinaryOp->Type) + "'");
}
void SystematicAnalysis::LogError_TypeIsNotCopyable(const NeverNullPtr<Token> Token, const TypeSymbol& Ex0Type)
{
	if (Ex0Type.IsBadType() || Type_IsUnMapType(Ex0Type)) { return; }

	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos,
		"The type '" + ToString(Ex0Type) + "'" + " is not copyable.");
}
void SystematicAnalysis::LogError_ExpressionMustbeAnLocationValueError(const NeverNullPtr<Token> Token, TypeSymbol& Ex0Type)
{
	if (Ex0Type.IsBadType() || Type_IsUnMapType(Ex0Type)) { return; }
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos,
		"expression must be an Location not an Value'" + ToString(Ex0Type) + "'");
}
void SystematicAnalysis::LogError_YouMustReturnSomethingError(const NeverNullPtr<Token> Token)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos
		, "you must return something");
}
void SystematicAnalysis::LogError_CantguessVarTypeError(const NeverNullPtr<Token> Token)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos
		, "can't guess 'var' type");
}
void SystematicAnalysis::LogError_CantUseThisKeyWordHereError(const NeverNullPtr<Token> NameToken)
{
	LogError(ErrorCodes::InValidType, NameToken->OnLine, NameToken->OnPos, "can't use this here");
}
void SystematicAnalysis::LogError_CantgussTypesTheresnoassignment(const NeverNullPtr<Token> Token)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos
		, "can't guess type theres no '=' [expression]");
}
void SystematicAnalysis::LogError_CantCastExplicityTypes(const NeverNullPtr<Token> Token, TypeSymbol& Ex0Type, TypeSymbol& ToTypeAs)
{
	if (Ex0Type.IsBadType() || ToTypeAs.IsBadType()
		|| Type_IsUnMapType(Ex0Type) || Type_IsUnMapType(ToTypeAs)) {
		return;
	}

	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Cant Explicity cast Type '" + ToString(Ex0Type) + "' to '" + ToString(ToTypeAs) + "'");
}

SystematicAnalysis::ReadVarErrorCheck_t SystematicAnalysis::TryLogError_OnReadVar(String_view VarName, const NeverNullPtr<Token> Token, const NullablePtr<Symbol> Syb)
{
	ReadVarErrorCheck_t r;
	if (!Syb.has_value())
	{
		LogError_CantFindVarError(Token, (String)VarName);
		r.CantFindVar = true;
	}
	else
	{
		if (Syb.value()->IsInvalid())
		{
			LogError_ReadingFromInvaidVariable(Token, (String)VarName);
			r.VarIsInvalid = true;
		}
	}
	return r;
}

void SystematicAnalysis::LogError_CantFindFuncError(const NeverNullPtr<Token> Token, String_view FuncName,
	const Vector<TypeSymbol>& Generics,
	const Vector<TypeSymbol>& WithTypes,
	const TypeSymbol& RetType)
{
	for (auto& Item : WithTypes)
	{
		if (Item.IsBadType()) { return; }
	}
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

	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, Text);
}
void SystematicAnalysis::LogError_CantFindFuncError(const NeverNullPtr<Token> Token, String_view FuncName,
	const Vector<TypeSymbol>& Generics,
	const Vector<ParInfo>& WithTypes,
	const TypeSymbol& RetType)
{
	for (auto& Item : WithTypes)
	{
		if (Item.Type.IsBadType()) { return; }
	}


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
		bool unpacktypepack = true;

		Text += "[";

		for (auto& Item : WithTypes)
		{

			if (unpacktypepack)
			{
				auto symop = Symbol_GetSymbol(Item.Type);
				if (symop.has_value())
				{
					auto sym = symop.value();

					if (sym->Type == SymbolType::Type_Pack)
					{
						auto info = sym->Get_Info<TypePackInfo>();
						for (auto& Item : info->List)
						{
							Text += ToString(Item);
							if (&Item != &info->List.back())
							{
								Text += ',';
							}
						}
						continue;
					}
				}
			}
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

	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, Text);
}
void SystematicAnalysis::LogError_CantFindPostfixOpForTypes_Constant(const NeverNullPtr<Token> BinaryOp, TypeSymbol& Ex0Type)
{
	LogError_CantFindPostfixOpForTypes(BinaryOp, Ex0Type);
}
void SystematicAnalysis::LogError_Eval_CantDoPostfixOpForTypes(const NeverNullPtr<Token> BinaryOp, TypeSymbol& Ex0Type)
{
	LogError(ErrorCodes::InValidName, BinaryOp->OnLine, BinaryOp->OnPos
		, "The Type operation '" + ToString(Ex0Type) + "' must be an compile time constant.");
}
void SystematicAnalysis::LogError_Eval_CantCastImplicitTypes(const NeverNullPtr<Token> Token, const TypeSymbol& Ex1Type, const TypeSymbol& UintptrType)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Casting Type '" + ToString(Ex1Type) + " to '" + ToString(UintptrType) + "' can't be done at compile time.");
}
void SystematicAnalysis::LogError_CantFindNamespace(const NeverNullPtr<Token> Token, const String_view Namespace)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos
		, "the can't find the Namespace '" + (String)Namespace + "'.");
}
void SystematicAnalysis::LogError_TypeMustBeAnConstantExpressionAble(const NeverNullPtr<Token> Token, const TypeSymbol& Type)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos
		, "the type " + ToString(Type) + " must be an Constant Expression able type'");
}
void SystematicAnalysis::LogError_CantModifyiMutableError(const NeverNullPtr<Token> Token, String_view Name)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Cant modify '" + (String)Name + "' it's immutable");
}
void SystematicAnalysis::LogError_CantFindTypeError(const NeverNullPtr<Token> Token, String_view Name)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Cant Find Type '" + (String)Name + "'");
}
void SystematicAnalysis::LogError_CantFindSymbolError(const NeverNullPtr<Token> Token, String_view Name)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Cant Find Symbol '" + (String)Name + "'");
}
void SystematicAnalysis::LogError_TypeDependencyCycle(const NeverNullPtr<Token> Token, const ClassInfo* Value)
{
	String Msg = "Class Dependency Cycle On Type '" + (String)Value->FullName + "' <- ";

	for (size_t i = _ClassDependencies.size() - 1; i != (Vector<const ClassInfo*>::size_type) - 1; i--)
	{
		auto& Item = _ClassDependencies[i];

		Msg += "'" + (String)Item->FullName + "'";
		if (&Item != &_ClassDependencies.front())
		{
			Msg += " <- ";
		}
	}
	Msg += ".";
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, Msg);


}
void SystematicAnalysis::LogError_CantUseThisHere(const NeverNullPtr<Token> Token)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Cant Find Type for '" + (String)TokenStringHelper::ToString(TokenType::KeyWord_This) + "'");
}
void SystematicAnalysis::LogError_CanIncorrectParCount(const NeverNullPtr<Token> Token, String_view FuncName, size_t Count, size_t FuncCount)
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

	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, Msg);
}
void SystematicAnalysis::LogError_CanIncorrectGenericCount(const NeverNullPtr<Token> Token, String_view FuncName, size_t Count, size_t FuncCount)
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

	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, Msg);
}
void SystematicAnalysis::LogError_LogFuncMustBe(const NeverNullPtr<Token> Token, const String_view FuncName, TypeSymbol& TypeSybToBe)
{

	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "The function named " + (String)FuncName + " must return to the type of '" + ToString(TypeSybToBe) + '\'');
}
void SystematicAnalysis::LogError_SymbolRedefinition(const NeverNullPtr<Token> Token, const NeverNullPtr<Symbol> Symbol)
{

	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "the symbol '" + Symbol->FullName + "\' is already define. try using a different name");
}
void SystematicAnalysis::LogError_UseingVarableBeforDeclared(const NeverNullPtr<Token> Token)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "trying to use the variable '" + (String)Token->Value._String + "\' before its defined.");
}
void SystematicAnalysis::LogError_BeMoreSpecifiicForRetType(const String_view FuncName, const NeverNullPtr<Token> Token)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "be more Specifiic For return Type. like |" + (String)FuncName + "[...] -> [Type]; or give the function a body.");
}
void SystematicAnalysis::LogError_CantBeIndexWithType(const NeverNullPtr<Token> Token, const  TypeSymbol& Ex0Type, const  TypeSymbol& IndexType)
{
	if (Ex0Type.IsBadType() || IndexType.IsBadType()) { return; }

	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "The Type '" + ToString(Ex0Type) + "\' Cant be Index with '" + ToString(IndexType) + "'.");
}
void SystematicAnalysis::LogError_CantUseThisInStaticFunction(const NeverNullPtr<Token> Token)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "The 'this' parameter can't be accessed in a static function.A 'this' function must look like |[this&,...] -> [Type];");
}
void SystematicAnalysis::LogError_FuncDependencyCycle(const NeverNullPtr<Token> Token, const FuncInfo* Value)
{
	String Msg = "function return type Dependency Cycle On function '" + (String)Value->FullName + "' <- ";

	for (size_t i = _FuncStack.size() - 1; i != (Vector<FuncStackInfo>::size_type) - 1; i--)
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
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, Msg);

}
void SystematicAnalysis::LogError_GenericInputWantsaExpressionNotType(const NeverNullPtr<Token> Token, const String_view NameOfPar)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Generic The generic parameter '" + (String)NameOfPar + "' Wants a Expression not a Type.");
}
void SystematicAnalysis::LogError_GenericInputWantsaTypeNotExpression(const NeverNullPtr<Token> Token, const String_view NameOfPar)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Generic The generic parameter '" + (String)NameOfPar + "'Type Wants a  not a Expression.");

}

void SystematicAnalysis::LogError_ExpectedSymbolToBea(const NeverNullPtr<Token> Token, const Symbol& Syb, SymbolType Value)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Did not expect Symbol the Symbol '" + Syb.FullName + "' to be '" + ToString(Syb.Type) +
		"' .Expected '" + ToString(Value) + '\'');

}
String SystematicAnalysis::ToString(SymbolType Value) const
{
	switch (Value)
	{
	case SymbolType::Null:return "null";
	case SymbolType::Any:return "Any";
	case SymbolType::Varable_t:return "Varable_t";
	case SymbolType::StackVarable:return "Stack Varable";
	case SymbolType::ParameterVarable:return "Parameter Varable";
	case SymbolType::Type:return "Type";
	case SymbolType::Type_alias:return "Type Alias";
	case SymbolType::Hard_Type_alias:return "Hard Type Alias";
	case SymbolType::Type_class:return "Type Class";
	case SymbolType::Class_Field:return "Class Field";
	case SymbolType::Enum:return "Enum";
	case SymbolType::Enum_Field:return "Enum Field";
	case SymbolType::Func:return "Func";
	case SymbolType::ImportedDllFunc:return "ImportedDllFunc";
	case SymbolType::ImportedLibFunc:return "ImportedLibFunc";
	case SymbolType::FuncCall:return "Func Call";
	case SymbolType::GenericFunc:return "Generic Func";
	case SymbolType::Generic_class:return"Generic Class";
	case SymbolType::Unmaped_Generic_Type:return "Unmaped Generic Type";
	case SymbolType::Unmaped_Varable:return "Unmaped Varable";
	case SymbolType::Namespace:return "Namespace";
	case SymbolType::Hard_Func_ptr:return "Hard Func ptr";
	case SymbolType::Func_ptr:return "Func ptr";
	case SymbolType::ConstantExpression:return "Constant Expression";
	case SymbolType::Generic_Alias:return "Generic Alias";
	case SymbolType::Generic_Tag:return "Generic Tag";
	case SymbolType::Generic_Enum:return "Generic Enum";
	case SymbolType::Trait_class:return "Trait Class";
	case SymbolType::Generic_Trait:return "Generic Trait";
	default:
		UCodeLangUnreachable();
		return "[n/a]";
	}
}
void SystematicAnalysis::LogError_CanIncorrectStaticArrCount(const NeverNullPtr<Token> Token, const TypeSymbol& Type, size_t Count, size_t FuncCount)
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

	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, Msg);
}
void SystematicAnalysis::LogError_BeMoreSpecifiicWithStaticArrSize(const NeverNullPtr<Token> Token, const TypeSymbol& Type)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Be More Specifiic with Static Array Size.Ex: " + ToString(Type) + "[/1]");
}
void SystematicAnalysis::LogError_LogWantedAVariable(const NeverNullPtr<Token>& Item, Symbol* TepSyb)
{
	LogError(ErrorCodes::BackEndError, Item->OnLine, Item->OnPos,
		"found a " + ToString(TepSyb->Type) + "(" + TepSyb->FullName + ")" + ".but wanted a Variable or a class field");
}

void SystematicAnalysis::LogError_BinaryOverloadPars(const Token& Name, const FuncInfo* Func)
{
	LogError(ErrorCodes::InValidType, Name.OnLine, Name.OnPos
		, "The Binary Overload '" + ToString(Name.Type) + "'" + " must have 2 paameters it has " + std::to_string(Func->Pars.size()) + " Pameters");
}
void SystematicAnalysis::LogError_IndexOverloadPars(const Token& Name, const FuncInfo* Func)
{
	LogError(ErrorCodes::InValidType, Name.OnLine, Name.OnPos
		, "The Index Overload '" + ToString(Name.Type) + "'" + " must have 2 paameters it has " + std::to_string(Func->Pars.size()) + " Pameters");
}
void SystematicAnalysis::LogError_PostfixOverloadPars(const Token& Name, const FuncInfo* Func)
{
	LogError(ErrorCodes::InValidType, Name.OnLine, Name.OnPos
		, "The Index Overload '" + ToString(Name.Type) + "'" + " must have 1 paameters it has " + std::to_string(Func->Pars.size()) + " Pameters");
}

void SystematicAnalysis::LogError_CantOverLoadOverload(const NeverNullPtr<Token> NameToken)
{
	LogError(ErrorCodes::InValidName, NameToken->OnLine, NameToken->OnPos, "You may not Overload '" + ToString(NameToken->Type) + "'.");
}

void SystematicAnalysis::LogError_CantFindMemberOverloadForType(const NeverNullPtr<Token> Item, TokenType Op, const TypeSymbol& Out)
{
	LogError(ErrorCodes::InValidName, Item->OnLine, Item->OnPos
		, "Cant find operator overload for '" + ToString(Op) + "' For Type " + ToString(Out));

}
void SystematicAnalysis::LogError_MustMakeEnumLikeafunction(EnumInfo* Einfo, size_t Index, const NeverNullPtr<Token> Token)
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
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos,
		Msg);
}
void SystematicAnalysis::LogError_CantUseMoveTypeHere(const NeverNullPtr<Token> Token)
{

	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "Cant use moved Type Here.it can only be used in Parameters");
}
void SystematicAnalysis::LogError_DynamicMustBeRrait(const TypeNode& V, const TypeSymbol& Out)
{
	auto token = V._name._ScopedName.back()._token;
	LogError(ErrorCodes::InValidType, token->OnLine, token->OnPos, "using a Dynamic type on a none trait the type found '" + ToString(Out) + "'");
}
void SystematicAnalysis::LogError_TraitCantBeAlone(const NeverNullPtr<Token> Token)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "the Type uses a Trait.but Traits can't be use alone.");
}

void SystematicAnalysis::LogError_WantedAType(const TypeNode& V, Symbol* SybV)
{
	auto token = V._name._ScopedName.back()._token;
	LogError(ErrorCodes::InValidType, token->OnLine, token->OnPos,
		"found a '" + ToString(SybV->Type) + "' for the Symbol " + SybV->FullName + " but wanted a type");
}
void SystematicAnalysis::LogError_OutCanOnlyBeInControlFlow(const NeverNullPtr<Token> Token)
{

	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "The 'out' can only be used in Control flow like if");
}
void SystematicAnalysis::LogError_ParamterMustBeAnOutExpression(const NeverNullPtr<Token> Token, const size_t& i)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "parameter '" + std::to_string(i) + "' does not use the out keyword");
}
void SystematicAnalysis::LogError_LogParPackIsNotLast(const NeverNullPtr<Token> Token)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "Parameter  named '" + (String)Token->Value._String + "' is using a Parameter pact.But Parameter pact must be last Paramter");
}
void SystematicAnalysis::LogError_ParPackTypeIsNotLast(const NeverNullPtr<Token> Token)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "Type Pack named '" + (String)Token->Value._String + "' is not declarded last.");
}
void SystematicAnalysis::LogError_UseingTypeinfoInNonEvalVarable(const NeverNullPtr<Token> Token)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos, "Trying to use typeinfo in a Non-eval Variable");
}
void SystematicAnalysis::LogError_UseingTypeinfoInEvalFuncPar(const NeverNullPtr<Token> Token)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos, "Trying to use typeinfo in a Non-eval Func");
}
void SystematicAnalysis::LogError_CantOutputTypeinfo(const NeverNullPtr<Token> Token)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "Cant Output IR of an typeinfo. place this in an eval function or an eval variable");
}
void SystematicAnalysis::LogError_DoesNotHaveForOverload(const NeverNullPtr<Token> Token, TypeSymbol& ExType)
{
	if (ExType.IsBadType()) { return; }
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "The Type '" + ToString(ExType) + "\' does not have the for overload.");
}
void SystematicAnalysis::LogError_CantUseTypeVoidHere(const NeverNullPtr<Token> Token)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "Cant use type void here");
}
void SystematicAnalysis::LogError(ErrorCodes Err, const String& MSG, const NeverNullPtr<Token> Token)
{
	LogError(Err, Token->OnLine, Token->OnPos, MSG);
}

void SystematicAnalysis::LogError_CantBindTypeItNotTypeInfo(const NeverNullPtr<Token> Token, TypeSymbol& Type)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "Cant Bind type.Expression is not a typeinfo it is an '" + ToString(Type) + "'");
}
void SystematicAnalysis::LogError_yieldnotAsync(const NeverNullPtr<Token> token)
{
	LogError(ErrorCodes::InValidType, "yield Expression must be type async<T>.", token);
}
void SystematicAnalysis::LogError_TypeIsNotExport(const NeverNullPtr<Token> Token, const TypeSymbol Type, NeverNullPtr<Symbol> Symbol)
{
	LogError(ErrorCodes::InValidType, "Cant Export '" + Symbol->FullName + "' because '" + ToString(Type) + "' is a type that is not exported", Token);
}
void SystematicAnalysis::LogError_ExportIsPrivate(const NeverNullPtr<Token> Token, NeverNullPtr<Symbol> Symbol)
{
	LogError(ErrorCodes::InValidType, "Cant Export '" + Symbol->FullName + "' because its private", Token);
}

void SystematicAnalysis::LogError(ErrorCodes Err, size_t Line, size_t Pos, const String& MSG)
{
	String Str;
	size_t Added = 0;
	for (auto& Item : _ExtendedErr)
	{
		Str += Item + ": \n";

		Added++;

		for (size_t i = 0; i < Added; i++)
		{
			Str += "  ";
		}
	}


	Str += MSG;


	_ErrorsOutput->AddError(Err, Line, Pos, Str);
}

UCodeLangFrontEnd

#endif