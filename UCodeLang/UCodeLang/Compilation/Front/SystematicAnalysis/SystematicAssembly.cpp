#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
#include "UCodeLang/Compilation/Front/Lexer.hpp"
#include "UCodeLang/Compilation/Helpers/NameDecoratior.hpp"
UCodeLangFrontStart

void
SystematicAnalysis::Assembly_ConvertAttributes(const Vector<Unique_ptr<AttributeNode>>& nodes, Vector<UsedTagValueData>& Out)
{
	bool IsSubModule = _Settings->_Type == OutPutType::IRAndSymbols;
	bool AddSubModuleTests = !IsSubModule;

	if (AddSubModuleTests)
	{
		Out.resize(nodes.size());
		for (size_t i = 0; i < nodes.size(); i++)
		{
			Assembly_ConvertAttribute(*nodes[i], Out[i]);
		}
	}
	else
	{
		for (size_t i = 0; i < nodes.size(); i++)
		{
			auto& Item = nodes[i];

			String str;
			Item->_ScopedName.GetScopedName(str);
			{
				// good enough for now
				// TODO verify that it's the actual test tag in StandardLibrary
				bool istesttag = StringHelper::EndWith(str, "Test");

				if (istesttag)
				{
					continue;
				}
			}
			Assembly_ConvertAttribute(*nodes[i], Out.emplace_back());
		}
	}
}
void SystematicAnalysis::Assembly_ConvertAttribute(const AttributeNode& nodes, UsedTagValueData& Out)
{
	auto Syb = Symbol_GetSymbol(Symbol_GetSymbolID(nodes));
	Out.TypeID = Type_GetTypeID(TypesEnum::CustomType, Syb->VarType._CustomTypeSymbol);

	auto& p = Syb->Get_Info<UsedTagInfo>()->RawObj;
	Out._Data.Resize(p.ObjectSize);

	memcpy(Out._Data.Bytes.get(), p.Object_AsPointer.get(), p.ObjectSize);
}
void SystematicAnalysis::Assembly_LoadLibSymbols()
{
	LoadLibMode Mode = LoadLibMode::GetTypes;

	_LibsFiles.resize(_Libs->size());
	for (size_t i = 0; i < _Libs->size(); i++)
	{
		FileNode& V = _LibsFiles[i];
		V.FileName = (*_LibsNames)[i];
	}

	Set<String> AddedSymbols;
	UnorderedMap<FileNode*, ImportLibInfo> Importinfo;

	while (Mode != LoadLibMode::Done)
	{
		for (size_t i = 0; i < _Libs->size(); i++)
		{
			auto& Item = (*_Libs)[i];
			FileNode& FileNode = _LibsFiles[i];
			this->_LookingAtFile = &FileNode;

			if (LoadLibMode::GetTypes == Mode)
			{
				_FilesData.AddValue(NeverNullptr((FileNode_t*)&FileNode), std::make_shared<FileNodeData>());
				ImportLibInfo V;
				for (auto& LibNode : Item->_Assembly.Classes)
				{
					if (!AddedSymbols.HasValue(LibNode->FullName) || LibNode->Get_Type() == ClassType::ForType)
					{
						if (LibNode->Get_Type() != ClassType::ForType) {
							AddedSymbols.AddValue(LibNode->FullName);
						}
						V.ClassesToAdd.AddValue(LibNode.get());

						_Lib._Assembly.Classes.push_back(Unique_ptr<AssemblyNode>(LibNode.get())); // make ref
					}
				}

				Importinfo.AddValue(&FileNode, std::move(V));
			}

			Assembly_LoadLibSymbols(*Item, Importinfo.GetValue(&FileNode), Mode);
		}

		//
		switch (Mode)
		{
		case LoadLibMode::GetTypes:
			Mode = LoadLibMode::FixTypes;
			break;
		case LoadLibMode::FixTypes:
			Mode = LoadLibMode::Done;
			break;
		case LoadLibMode::Done:
			break;
		default:
			UCodeLangUnreachable();
			break;
		}

		//
	}

	for (auto& Item : _Lib._Assembly.Classes)
	{
		auto Ptr = Item.release(); // are just refs
	}
	_Lib._Assembly.Classes.clear(); // remove nullptr Nodes

	// The CPU is going to hate this.
	for (auto& Item : _Lib_TypesToFix)
	{
		*Item.TypeToFix = *Item.ToGetTypeFrom;
	}
	_Lib_TypesToFix.clear(); // free Mem.

	auto globalAssemblyObjectName = (String_view)ScopeHelper::_globalAssemblyObject;
	_Lib.Get_Assembly().AddClass(String(globalAssemblyObjectName), String(globalAssemblyObjectName));
}
void SystematicAnalysis::Assembly_LoadLibSymbols(const UClib& lib, ImportLibInfo& libinfo, LoadLibMode Mode)
{

	auto OutputType = Output_TypeAsLibType();
	auto libType = lib._LibType;

	auto GlobalObject = lib.Get_Assembly().Get_GlobalObject_Class();
	if (GlobalObject)
	{
		String Scope;
		Assembly_LoadClassSymbol(*GlobalObject, Scope, Scope, Mode);
	}
	auto libname = _LookingAtFile->FileName;

	Vector<AssemblyNode*> Classes;
	Classes.reserve(libinfo.ClassesToAdd.size());
	for (auto& Item : lib.Get_Assembly().Classes)
	{
		if (libinfo.ClassesToAdd.HasValue(Item.get()))
		{
			Classes.push_back(Item.get());
		}
	}

	std::sort(Classes.begin(), Classes.end(), [](AssemblyNode*& A, AssemblyNode*& B)
			  { return (int)A->Get_Type() < (int)B->Get_Type(); });

	for (auto& Item : Classes)
	{
		if (Item->FullName == ScopeHelper::_globalAssemblyObject)
		{
			continue;
		}
		String Scope;
		String FullName = Item->FullName;
		switch (Item->Get_Type())
		{
		case ClassType::Class:
		{
			Assembly_LoadClassSymbol(Item->Get_ClassData(), FullName, Scope, Mode);
		}
		break;
		case ClassType::Alias:
		{
			Assembly_LoadAliasSymbol(Item->Get_AliasData(), FullName, Scope, Mode);
		}
		break;
		case ClassType::Enum:
		{
			Assembly_LoadEnumSymbol(Item->Get_EnumData(), FullName, Scope, Mode);
		}
		break;
		case ClassType::Tag:
		{
			Assembly_LoadTagSymbol(Item->Get_TagData(), FullName, Scope, Mode);
		}
		break;
		case ClassType::Trait:
		{
			Assembly_LoadTraitSymbol(Item->Get_TraitData(), FullName, Scope, Mode);
		}
		break;
		case ClassType::NameSpace:
		{
			Assembly_LoadSymbol(Item->Get_NameSpace(), FullName, Mode);
		}
		break;
		case ClassType::ForType:
		{
			Assembly_LoadSymbol(Item->Get_ForType(), Mode);
		}
		break;
		case ClassType::StaticArray:
		case ClassType::GenericFunction:
		case ClassType::GenericClass:
		{

		}
		break;
		default:
			UCodeLangUnreachable();
			break;
		}
	}
	if (Mode == LoadLibMode::GetTypes)
	{
		// this feals off
		FrontEnd::Lexer _Lexer;
		FrontEnd::Parser _Parser;

		_Lexer.Set_ErrorsOutput(_ErrorsOutput);
		_Parser.Set_ErrorsOutput(_ErrorsOutput);

		_Lexer.Set_Settings(_Settings);
		_Parser.Set_Settings(_Settings);

		for (auto& Item : Classes)
		{
			Optional<String_view> TextOp;
			AccessModifierType Access = AccessModifierType::Default;
			bool IsExport = false;
			CapturedUseStatements* Uses = nullptr;
			if (Item->Get_Type() == ClassType::GenericClass)
			{
				auto& data = Item->Get_GenericClass();
				TextOp = data.Base.Implementation;
				Access = data.AccessModifier;
				IsExport = data.IsExported;
				Uses = &data.UseStatments;
			}
			else if (Item->Get_Type() == ClassType::GenericFunction)
			{
				auto& data = Item->Get_GenericFunctionData();
				TextOp = data.Base.Implementation;
				Access = data.AccessModifier;
				IsExport = data.IsExported;
				Uses = &data.UseStatments;
			}

			if (TextOp.has_value() && IsExport)
			{
				StringsFromLoadLib.push_back(std::make_unique<String>(TextOp.value()));
				String_view Text = *StringsFromLoadLib.back().get();

				_Lexer.Lex(Text);

				TokensFromLoadLib.push_back(std::make_unique<Vector<Token>>(std::move(_Lexer.Get_Tokens())));
				auto& tokenslist = *TokensFromLoadLib.back().get();

				_Parser.Parse(Text, tokenslist);

				UCodeLangAssert(!_ErrorsOutput->Has_Errors());

				bool shouldskip = false;
				{
					auto list = GetSymbolsWithName(Item->FullName);

					for (auto& Item : list)
					{
						//TODO

					}
				}
				if (shouldskip)
				{
					continue;
				}

				NodesFromLoadLib.push_back(
					std::make_unique<FileNode>(std::move(_Parser.Get_Tree())));

				_FilesData.AddValue(NeverNullptr((FileNode_t*)NodesFromLoadLib.back().get()),
									std::make_shared<FileNodeData>());

				NodesFromLoadLib.back()->FileName = libname;

				auto& list = NodesFromLoadLib.back().get()->_Nodes;
				_LookingAtFile = NodesFromLoadLib.back().get();

				auto namespaceV = ScopeHelper::GetReMoveScope(Item->FullName);

				size_t ScopeCount = ScopeHelper::Get_ScopeCount(namespaceV) + 1;

				_Table.AddScope(namespaceV);

				auto namespacesyb = GetSymbolsWithName(namespaceV);
				Symbol* nameSymbol = nullptr;

				if (namespacesyb.size())
				{
					auto SymV = namespacesyb.front();

					nameSymbol = SymV;

					if (nameSymbol->Type == SymbolType::Type_class)
					{
						ClassStackInfo tep;
						tep.Syb = nameSymbol;
						tep.Info = nameSymbol->Get_Info<ClassInfo>();

						_ClassStack.push(std::move(tep));
					}
				}
				for (auto& Item : Uses->NameSpaces)
				{
					_Table.Useings.push_back(Item);
				}

				auto pass = _PassType;
				for (auto& Item2 : list)
				{
					Push_ToNodeScope(*Item2.get());
					auto nextsymindex = _Table.Symbols.size();
					switch (Item2->Get_Type())
					{
					case NodeType::ClassNode:
						OnClassNode(*ClassNode::As(Item2.get()));
						break;
					case NodeType::AliasNode:
						OnAliasNode(*AliasNode::As(Item2.get()));
						break;
					case NodeType::EnumNode:
						OnEnum(*EnumNode::As(Item2.get()));
						break;
					case NodeType::FuncNode:
						OnFuncNode(*FuncNode::As(Item2.get()));
						break;
					case NodeType::UsingNode:
						OnUseingNode(*UsingNode::As(Item2.get()));
						break;
					case NodeType::TraitNode:
						OnTrait(*TraitNode::As(Item2.get()));
						break;
					case NodeType::TagTypeNode:
						OnTag(*TagTypeNode::As(Item2.get()));
						break;
					default:
						UCodeLangUnreachable();
						break;
					}
					auto& sym = _Table.Symbols[nextsymindex];
					LibGenericSymbolLoad.AddValue(Item, sym.get());

					_PassType = pass;

					Pop_NodeScope();
				}

				if (nameSymbol)
				{
					if (nameSymbol->Type == SymbolType::Type_class)
					{
						_ClassStack.pop();
					}
				}

				for (size_t i = 0; i < Uses->NameSpaces.size(); i++)
				{
					_Table.Useings.erase(_Table.Useings.end());
				}
				for (size_t i = 0; i < ScopeCount; i++)
				{
					_Table.RemoveScope();
				}
				UCodeLangAssert(_Table._Scope.ThisScope.size() == 0);
			}
		}
	}

	if (Mode == LoadLibMode::FixTypes)
	{

		auto pass = _PassType;

		_PassType = PassType::FixedTypes;

		for (auto& Item : Classes)
		{
			if (Item->Get_Type() == ClassType::GenericClass || Item->Get_Type() == ClassType::GenericFunction)
			{
				bool Export;
				CapturedUseStatements* Uses = nullptr;
				if (Item->Get_Type() == ClassType::GenericClass)
				{
					auto& Data = Item->Get_GenericClass();
					Export = Data.IsExported;
					Uses = &Data.UseStatments;
				}
				else
				{
					auto& Data = Item->Get_GenericFunctionData();
					Export = Data.IsExported;
					Uses = &Data.UseStatments;
				}
				if (!Export) { continue; }
				if (!LibGenericSymbolLoad.HasValue(Item)) { continue; }


				auto Sym = Nullableptr(LibGenericSymbolLoad.GetValue(Item)); 	
				UCodeLangAssert(Sym);

				Node* Item2 = (Node*)Sym.value()->NodePtr;

				auto namespaceV = ScopeHelper::GetReMoveScope(Item->FullName);

				size_t ScopeCount = ScopeHelper::Get_ScopeCount(namespaceV) + 1;

				_Table.AddScope(namespaceV);

				auto namespacesyb = GetSymbolsWithName(namespaceV);
				Symbol* nameSymbol = nullptr;

				if (namespacesyb.size())
				{
					auto SymV = namespacesyb.front();

					nameSymbol = SymV;

					if (nameSymbol->Type == SymbolType::Type_class)
					{
						ClassStackInfo tep;
						tep.Syb = nameSymbol;
						tep.Info = nameSymbol->Get_Info<ClassInfo>();

						_ClassStack.push(std::move(tep));
					}
				}
				for (auto& Item : Uses->NameSpaces)
				{
					_Table.Useings.push_back(Item);
				}

				switch (Item2->Get_Type())
				{
				case NodeType::ClassNode:
					OnClassNode(*ClassNode::As(Item2));
					break;
				case NodeType::AliasNode:
					OnAliasNode(*AliasNode::As(Item2));
					break;
				case NodeType::EnumNode:
					OnEnum(*EnumNode::As(Item2));
					break;
				case NodeType::FuncNode:
					OnFuncNode(*FuncNode::As(Item2));
					break;
				case NodeType::UsingNode:
					OnUseingNode(*UsingNode::As(Item2));
					break;
				case NodeType::TraitNode:
					OnTrait(*TraitNode::As(Item2));
					break;
				case NodeType::TagTypeNode:
					OnTag(*TagTypeNode::As(Item2));
					break;
				default:
					UCodeLangUnreachable();
					break;
				}

				if (nameSymbol)
				{
					if (nameSymbol->Type == SymbolType::Type_class)
					{
						_ClassStack.pop();
					}
				}

				for (size_t i = 0; i < Uses->NameSpaces.size(); i++)
				{
					_Table.Useings.erase(_Table.Useings.end());
				}
				for (size_t i = 0; i < ScopeCount; i++)
				{
					_Table.RemoveScope();
				}

				UCodeLangAssert(_Table._Scope.ThisScope.size() == 0);
			}
		}
		_PassType = pass;
	}
}
void SystematicAnalysis::Assembly_LoadTraitAliases(const String& FullName, const Vector<TraitAlias>& GenericAlias)
{
	for (auto& Item : GenericAlias)
	{
		if (Item.TypePack.has_value()) 
		{
			auto& varsyb = Symbol_AddSymbol(SymbolType::Type_Pack, Item.AliasName, ScopeHelper::ApendedStrings(FullName, Item.AliasName), AccessModifierType::Private);
			TypePackInfo* info = new TypePackInfo();
			varsyb.Info.reset(info);

			info->List.reserve(Item.TypePack.value().size());
			
			_Table.AddSymbolID(varsyb, Symbol_GetSymbolID(&Item));
		}
		else if (Item.Expression.has_value())
		{
			auto& Ex = Item.Expression.value();
			
			auto& varsyb = Symbol_AddSymbol(SymbolType::ConstantExpression, Item.AliasName, ScopeHelper::ApendedStrings(FullName, Item.AliasName), AccessModifierType::Private);
			ConstantExpressionInfo* info = new ConstantExpressionInfo();
			varsyb.Info.reset(info);
			
			info->Ex.Object_AsPointer = Unique_Array<Byte>(new Byte[Ex.Size]);
			memcpy(info->Ex.Object_AsPointer.get(), Ex.Get_Data(), Ex.Size);
				

			_Table.AddSymbolID(varsyb, Symbol_GetSymbolID(&Item));
		}
		else
		{
			auto& varsyb = Symbol_AddSymbol(SymbolType::Type_alias, Item.AliasName, ScopeHelper::ApendedStrings(FullName, Item.AliasName), AccessModifierType::Private);
			_Table.AddSymbolID(varsyb, Symbol_GetSymbolID(&Item));	
		}
	}
}

void SystematicAnalysis::Assembly_LoadTraitAliases_FixTypes(const Vector<TraitAlias>& GenericAlias)
{
	for (auto& Item : GenericAlias)
	{
		auto& Sym = _Table.GetSymbol(Symbol_GetSymbolID(&Item));

		if (Item.TypePack.has_value())
		{
			auto& pack = Item.TypePack.value();

			TypePackInfo* info = Sym.Get_Info<TypePackInfo>();
			
			for (auto& Item : pack)
			{

				info->List.push_back(Assembly_LoadType(Item));
			}
		}
		else 
		{
			Sym.VarType = Assembly_LoadType(Item.Type);
		}
	}
}
void SystematicAnalysis::Assembly_LoadClassSymbol(const Class_Data& Item, const String& FullName, const String& Scope, SystematicAnalysis::LoadLibMode Mode)
{
	bool isexpot = Item.IsExported;
	if (FullName == "")
	{
		isexpot = true;
	}
	if (!isexpot)
	{
		return;
	}

	auto TepScope = std::move(_Table._Scope);

	_Table._Scope = {};
	_Table._Scope.ThisScope = Scope;

	if (Mode == LoadLibMode::GetTypes)
	{
		auto Name = ScopeHelper::GetNameFromFullName(FullName);
		auto& Syb = Symbol_AddSymbol(SymbolType::Type_class, Name, FullName, Item.AccessModifier);
		_Table.AddSymbolID(Syb, Symbol_GetSymbolID(&Item));

		
		Assembly_LoadTraitAliases(FullName, Item.GenericAlias);
	
		ClassInfo* Info = new ClassInfo();
		Syb.Info.reset(Info);

		Syb.VarType = TypeSymbol(Syb.ID);

		Syb.PassState = PassType::BuidCode;
		Syb.OutputIR = false;

		Info->FullName = FullName;

		Info->Size = Item.Size;
		Info->SizeInitialized = true;

		Info->Fields.resize(Item.Fields.size());

		for (size_t i = 0; i < Item.Fields.size(); i++)
		{
			const auto& FieldItem = Item.Fields[i];
			SymbolID id = Symbol_GetSymbolID(&FieldItem);

			auto& InfoItem = Info->Fields[i];

			InfoItem.Name = FieldItem.Name;
			// InfoItem.offset = FieldItem.offset;

			auto& FieldSyb = Symbol_AddSymbol(SymbolType::Class_Field, Name, ScopeHelper::ApendedStrings(FullName, FieldItem.Name), FieldItem.Protection);
			_Table.AddSymbolID(FieldSyb, id);
		}
	}
	else if (Mode == LoadLibMode::FixTypes)
	{
		auto& Syb = _Table.GetSymbol(Symbol_GetSymbolID(&Item));
		ClassInfo* Info = Syb.Get_Info<ClassInfo>();

		Assembly_LoadTraitAliases_FixTypes(Item.GenericAlias);

		for (size_t i = 0; i < Item.Fields.size(); i++)
		{
			const auto& FieldItem = Item.Fields[i];
			auto& InfoItem = Info->Fields[i];
			Assembly_LoadType(FieldItem.Type, InfoItem.Type);

			SymbolID id = Symbol_GetSymbolID(&FieldItem);

			auto FieldSyb = Symbol_GetSymbol(id);
			FieldSyb->VarType = InfoItem.Type;
		}
	}
	
	auto& Syb = _Table.GetSymbol(Symbol_GetSymbolID(&Item));
	ClassInfo* Info = Syb.Get_Info<ClassInfo>();


	for (auto& Item : Item.Methods)
	{
		auto nextsymindex = _Table.Symbols.size();
		Assembly_LoadSymbol(Item, Mode);

		if (Mode == LoadLibMode::GetTypes && Item.IsExport) 
		{
			auto& sym = _Table.Symbols[nextsymindex];
			auto finfo = sym->Get_Info<FuncInfo>();
			
			if (finfo->FrontParIsUnNamed)
			{
				if (finfo->Pars.size() == 1)
				{
					if (finfo->Get_Name() == ClassDestructorFunc)
					{
						Info->_WillHaveFielddeInit = true;
					}
				}
				else if (Item.ParsType.size() == 2)
				{
					auto& par = Item.ParsType[1];
					
					if (par.Type._CustomTypeID == Item.ParsType[0].Type._CustomTypeID) 
					{
						if (par.Type.Isimmutable() && par.Type.IsAddress())
						{
							Info->_ClassHasCopyConstructor = sym->ID;
						}
						else if (par.Type.IsMovedType())
						{
							Info->_ClassHasMoveConstructor = sym->ID;
						}
					}
				}
			}
		}
	}

	_Table._Scope = std::move(TepScope);
}
void SystematicAnalysis::Assembly_LoadEnumSymbol(const Enum_Data& Item, const String& FullName, const String& Scope, SystematicAnalysis::LoadLibMode Mode)
{
	if (!Item.IsExported)
	{
		return;
	}

	auto TepScope = std::move(_Table._Scope);

	_Table._Scope = {};
	_Table._Scope.ThisScope = Scope;

	if (Mode == LoadLibMode::GetTypes)
	{
		auto Name = ScopeHelper::GetNameFromFullName(FullName);
		auto& Syb = Symbol_AddSymbol(SymbolType::Enum, Name, FullName, Item.AccessModifier);
		_Table.AddSymbolID(Syb, Symbol_GetSymbolID(&Item));

		Syb.PassState = PassType::BuidCode;
		Syb.OutputIR = false;

		Syb.VarType = TypeSymbol(Syb.ID);

		auto enumInfo = new EnumInfo();
		Syb.Info.reset(enumInfo);

		enumInfo->FullName = FullName;
		enumInfo->Fields.resize(Item.Values.size());

		for (size_t i = 0; i < Item.Values.size(); i++)
		{
			auto& enumInfoItem = enumInfo->Fields[i];
			const auto& ValueItem = Item.Values[i];
			enumInfoItem.Name = ValueItem.Name;
			enumInfoItem.Ex.Object_AsPointer.reset(new Byte[ValueItem._Data.Size]);
			memcpy(enumInfoItem.Ex.Object_AsPointer.get(), ValueItem._Data.Get_Data(), ValueItem._Data.Size);

			{
				auto& FieldSyb = Symbol_AddSymbol(SymbolType::Enum_Field, ValueItem.Name, ScopeHelper::ApendedStrings(FullName, ValueItem.Name), AccessModifierType::Public);
				FieldSyb.PassState = PassType::BuidCode;
			}
		}

		if (Item.EnumVariantUnion.has_value())
		{
			EnumVariantData Data;
			Data.Variants.resize(Item.Values.size());
			enumInfo->VariantData = std::move(Data);
		}
	}
	else if (Mode == LoadLibMode::FixTypes)
	{
		auto& Syb = _Table.GetSymbol(Symbol_GetSymbolID(&Item));
		auto enumInfo = Syb.Get_Info<EnumInfo>();

		Assembly_LoadType(Item.BaseType, enumInfo->Basetype);

		if (Item.EnumVariantUnion.has_value())
		{
			EnumVariantData& Data = enumInfo->VariantData.value();
			for (size_t i = 0; i < Item.Values.size(); i++)
			{
				auto& VariantItem = Data.Variants[i];
				const auto& ValueItem = Item.Values[i];

				if (ValueItem.EnumVariantType.has_value())
				{
					auto Type = Assembly_LoadType(ValueItem.EnumVariantType.value());
					Symbol* Sym = Symbol_GetSymbol(Type).value_unchecked();
					if (Sym)
					{
						if (Sym->Type == SymbolType::Type_class)
						{
							bool IsUnNamed = false;
							if (Sym->FullName.back() == '!') // the unnamed Enum Symbol post fix
							{
								IsUnNamed = true;
							}

							if (IsUnNamed)
							{
								ClassInfo* CInfo = Sym->Get_Info<ClassInfo>();

								VariantItem.Types.resize(CInfo->Fields.size()); // Field type may not be loaded.

								for (size_t ix = 0; ix < CInfo->Fields.size(); ix++)
								{
									auto& Item = CInfo->Fields[ix];

									LibLoadTypeSeter Seter;
									Seter.ToGetTypeFrom = &Item.Type;
									Seter.TypeToFix = &VariantItem.Types[ix];
									_Lib_TypesToFix.push_back(Seter);
								}
								VariantItem.ClassSymbol = Sym->ID;
							}
							else
							{
								VariantItem.Types.push_back(Type);
							}
						}
						else
						{
							VariantItem.Types.push_back(Type);
						}
					}
					else
					{
						VariantItem.Types.push_back(Type);
					}
				}
			}
		}
	}

	_Table._Scope = std::move(TepScope);
}
void SystematicAnalysis::Assembly_LoadAliasSymbol(const Alias_Data& Item, const String& FullName, const String& Scope, SystematicAnalysis::LoadLibMode Mode)
{
	if (!Item.IsExported)
	{
		return;
	}

	auto TepScope = std::move(_Table._Scope);

	_Table._Scope = {};
	_Table._Scope.ThisScope = Scope;

	if (Mode == LoadLibMode::GetTypes)
	{
		auto Name = ScopeHelper::GetNameFromFullName(FullName);
		auto& Syb = Symbol_AddSymbol(
			Item.HardAliasTypeID.has_value()
				? SymbolType::Hard_Type_alias
				: SymbolType::Type_alias,
			Name, FullName, Item.AccessModifier);
		_Table.AddSymbolID(Syb, Symbol_GetSymbolID(&Item));

		Syb.PassState = PassType::BuidCode;
		Syb.OutputIR = false;

		Syb.VarType = TypeSymbol();

		auto enumInfo = new AliasInfo();
		Syb.Info.reset(enumInfo);
	}
	else if (Mode == LoadLibMode::FixTypes)
	{
		auto& Syb = _Table.GetSymbol(Symbol_GetSymbolID(&Item));

		Assembly_LoadType(Item.Type, Syb.VarType);
	}

	_Table._Scope = std::move(TepScope);
}
void SystematicAnalysis::Assembly_LoadTagSymbol(const Tag_Data& Item, const String& FullName, const String& Scope, SystematicAnalysis::LoadLibMode Mode)
{
	if (!Item.IsExported)
	{
		return;
	}

	auto TepScope = std::move(_Table._Scope);

	_Table._Scope = {};
	_Table._Scope.ThisScope = Scope;

	if (Mode == LoadLibMode::GetTypes)
	{
		auto Name = ScopeHelper::GetNameFromFullName(FullName);
		auto& Syb = Symbol_AddSymbol(SymbolType::Tag_class, Name, FullName, Item.AccessModifier);
		_Table.AddSymbolID(Syb, Symbol_GetSymbolID(&Item));

		Syb.PassState = PassType::BuidCode;
		Syb.OutputIR = false;

		Syb.VarType = TypeSymbol();

		auto enumInfo = new TagInfo();
		Syb.Info.reset(enumInfo);
	}
	else if (Mode == LoadLibMode::FixTypes)
	{
		auto& Syb = _Table.GetSymbol(Symbol_GetSymbolID(&Item));
	}

	_Table._Scope = std::move(TepScope);
}
void SystematicAnalysis::Assembly_LoadTraitSymbol(const Trait_Data& Item, const String& FullName, const String& Scope, SystematicAnalysis::LoadLibMode Mode)
{
	if (!Item.IsExported)
	{
		return;
	}

	auto TepScope = std::move(_Table._Scope);

	_Table._Scope = {};
	_Table._Scope.ThisScope = Scope;

	if (Mode == LoadLibMode::GetTypes)
	{
		auto Name = ScopeHelper::GetNameFromFullName(FullName);
		auto& Syb = Symbol_AddSymbol(SymbolType::Trait_class, Name, FullName, Item.AccessModifier);
		_Table.AddSymbolID(Syb, Symbol_GetSymbolID(&Item));

		_Table.AddScope(Name);

		auto& SybClass = Symbol_AddSymbol(SymbolType::Type_class, (String)Name + TraitClassEnd, _Table._Scope.ThisScope + TraitClassEnd, Syb.Access);
		_Table.AddSymbolID(SybClass, Symbol_GetSymbolID(&Item.Methods));

		_Table.RemoveScope();

		SybClass.VarType = SybClass.ID;
		SybClass.PassState = PassType::BuidCode;
		SybClass.Info.reset(new ClassInfo());

		SybClass.OutputIR = false;
		SybClass.PassState = PassType::BuidCode;

		Syb.PassState = PassType::BuidCode;
		Syb.OutputIR = false;

		Syb.VarType = Syb.ID;

		auto enumInfo = new TraitInfo();
		Syb.Info.reset(enumInfo);

		enumInfo->TraitClassInfo = &SybClass;

		for (auto& Item : Item.Fields)
		{
			auto& varsyb = Symbol_AddSymbol(SymbolType::Class_Field, Item.Name, ScopeHelper::ApendedStrings(FullName, Item.Name), AccessModifierType::Public);
			varsyb.OutputIR = false;
			varsyb.PassState = PassType::BuidCode;

			TraitVar var;
			var.Syb = &varsyb;
			enumInfo->_Vars.push_back(std::move(var));
		}

		auto oldpasstype = _PassType;
		_PassType = PassType::GetTypes;

		auto oldscope = _Table._Scope.ThisScope;
		_Table._Scope.ThisScope = Syb.FullName;

	
		Assembly_LoadTraitAliases(FullName, Item.GenericAlias);
		
		ClassStackInfo stackinfo;
		stackinfo.Syb = &SybClass;

		_ClassStack.push(stackinfo);

		{
			FrontEnd::Lexer _Lexer;
			FrontEnd::Parser _Parser;
			// this feals off
			_Lexer.Set_ErrorsOutput(_ErrorsOutput);
			_Parser.Set_ErrorsOutput(_ErrorsOutput);

			_Lexer.Set_Settings(_Settings);
			_Parser.Set_Settings(_Settings);

			for (auto& Item : Item.Symbols)
			{
				_Lexer.Reset();
				_Parser.Reset();

				StringsFromLoadLib.push_back(std::make_unique<String>(Item.Implementation));
				String_view Text = *StringsFromLoadLib.back().get();

				_Lexer.Lex(Text);

				TokensFromLoadLib.push_back(std::make_unique<Vector<Token>>(std::move(_Lexer.Get_Tokens())));
				auto& tokenslist = *TokensFromLoadLib.back().get();

				_Parser.Parse(Text, tokenslist);

				UCodeLangAssert(!_ErrorsOutput->Has_Errors());
				
				NodesFromLoadLib.push_back(std::make_unique<FileNode>(std::move(_Parser.Get_Tree())));

				_FilesData.AddValue(NeverNullptr((FileNode_t*)NodesFromLoadLib.back().get()),
									std::make_shared<FileNodeData>());

				auto& list = NodesFromLoadLib.back()->_Nodes;
				for (auto& Item2 : list)
				{
					size_t Index = _Table.Symbols.size();
					switch (Item2->Get_Type())
					{
					case NodeType::ClassNode:
					{
						OnClassNode(*ClassNode::As(Item2.get()));
					}
					break;
					case NodeType::AliasNode:
					{
						OnAliasNode(*AliasNode::As(Item2.get()));
					}
					break;
					default:UCodeLangUnreachable();
						break;
					}
					Symbol* funcSyb = _Table.Symbols[Index].get();
					enumInfo->_Symbols.push_back(funcSyb);


					size_t EndIndex = _Table.Symbols.size();
					for (size_t i = Index; i < EndIndex; i++)
					{
						Symbol* funcSyb = _Table.Symbols[i].get();

						funcSyb->OutputIR = false;
					}
				}
			}
		}
		_ClassStack.pop();

		for (auto& Item : Item.Methods)
		{
			Symbol* funcsyb = nullptr;

			if (Item.FuncBody.has_value())
			{
				auto& FuncStr = Item.FuncBody.value();

				// this feals off
				FrontEnd::Lexer _Lexer;
				FrontEnd::Parser _Parser;

				_Lexer.Set_ErrorsOutput(_ErrorsOutput);
				_Parser.Set_ErrorsOutput(_ErrorsOutput);

				_Lexer.Set_Settings(_Settings);
				_Parser.Set_Settings(_Settings);

				_Lexer.Lex(FuncStr);

				TokensFromLoadLib.push_back(std::make_unique<Vector<Token>>(std::move(_Lexer.Get_Tokens())));

				_Parser.Parse(FuncStr, *TokensFromLoadLib.back());

				UCodeLangAssert(!_ErrorsOutput->Has_Errors());

				NodesFromLoadLib.push_back(std::make_unique<FileNode>(std::move(_Parser.Get_Tree())));
				Node* node = NodesFromLoadLib.back()->_Nodes[0].get();

				ClassStackInfo info;
				info.Syb = &SybClass;

				_ClassStack.push(info);

				size_t Index = _Table.Symbols.size();

				OnFuncNode(*FuncNode::As(node));

				_ClassStack.pop();

				funcsyb = _Table.Symbols[Index].get();
				funcsyb->OutputIR = false;
			}
			else
			{
				funcsyb = &Symbol_AddSymbol(SymbolType::Func, ScopeHelper::GetNameFromFullName(Item.method.FullName), Item.method.FullName, AccessModifierType::Public);
				funcsyb->OutputIR = false;
				funcsyb->PassState = PassType::BuidCode;

				auto Funcinfo = new FuncInfo();
				funcsyb->Info.reset(Funcinfo);
				LoadFuncInfoGetTypes(Funcinfo, Item.method);

				if (Item.method.IsThisFunction)
				{
					auto& FuncP = Funcinfo->Pars.front();
					FuncP.Type._Type = TypesEnum::CustomType;
					FuncP.Type._CustomTypeSymbol = SybClass.ID;
				}
			}

			TraitFunc func;
			func.HasBody = Item.FuncBody.has_value();
			func.Syb = funcsyb;
			enumInfo->_Funcs.push_back(std::move(func));
		}

		_Table._Scope.ThisScope = oldscope;

		_PassType = oldpasstype;
	}
	else if (Mode == LoadLibMode::FixTypes)
	{
		auto Name = ScopeHelper::GetNameFromFullName(FullName);
		auto& Syb = _Table.GetSymbol(Symbol_GetSymbolID(&Item));
		TraitInfo* info = Syb.Get_Info<TraitInfo>();

		for (size_t i = 0; i < info->_Vars.size(); i++)
		{
			info->_Vars[i].Syb->VarType = Assembly_LoadType(Item.Fields[i].Type);
		}

		ClassStackInfo stackinfo;
		stackinfo.Syb = info->TraitClassInfo;

		_ClassStack.push(stackinfo);

		auto oldpasstype = _PassType;
		_PassType = PassType::FixedTypes;

		auto oldscope = _Table._Scope.ThisScope;
		_Table._Scope.ThisScope = Syb.FullName;
		{
			for (auto& Item : Item.GenericAlias)
			{
				auto& item = _Table.GetSymbol(Symbol_GetSymbolID(&Item));
				item.VarType = Assembly_LoadType(Item.Type);
			}
		}
		for (size_t i = 0; i < info->_Funcs.size(); i++)
		{
			auto& Item2 = info->_Funcs[i];
			if (!Item2.HasBody)
			{
				FuncInfo* finfo = Item2.Syb->Get_Info<FuncInfo>();
				LoadFuncInfoFixTypes(finfo, Item.Methods[i].method);
				if (Item.Methods[i].method.IsThisFunction)
				{
					auto& FuncP = finfo->Pars.front();
					FuncP.Type._Type = TypesEnum::CustomType;
					FuncP.Type._CustomTypeSymbol = info->TraitClassInfo->ID;
				}
			}
		}
		for (size_t i = 0; i < info->_Funcs.size(); i++)
		{
			auto& Item2 = info->_Funcs[i];
			if (Item2.HasBody)
			{
				Node* node = (Node*)Item2.Syb->NodePtr;

				OnFuncNode(*FuncNode::As(node));
			}	
		}

		for (auto& Item : info->_Symbols)
		{
			switch (Item->Type)
			{
			case SymbolType::Type_class:
			{
				auto nod = Item->Get_NodeInfo<ClassNode>();
				OnClassNode(*nod);
			}
			break;
			case SymbolType::Type_alias:
			{
				auto nod = Item->Get_NodeInfo<AliasNode>();
				OnAliasNode(*nod);
			}
			break;
			default:
				UCodeLangUnreachable();
				break;
			} 
		}

	

		_Table._Scope.ThisScope = oldscope;

		_ClassStack.pop();
		//
		/*
		auto StructVtablueClass = _IR_Builder.NewStruct(_IR_Builder.ToID(Str_GetTraitVStructTableName(Syb.FullName)));

		for (auto& Item : info->_Funcs)
		{
			FuncInfo* ItemInfo = Item.Syb->Get_Info<FuncInfo>();
			auto StrFunc = GetTepFuncPtrName(ItemInfo);
			auto PtrFunc = GetTepFuncPtrSyb(StrFunc, ItemInfo).value();
			PtrFunc->FullName = StrFunc;
			TypeSymbol PtrType = PtrFunc->ID;

			auto IRType = IR_ConvertToIRType(PtrType);

			IRStructField V;
			V.Type = IRType;
			StructVtablueClass->Fields.push_back(V);
		}
		*/

		_PassType = oldpasstype;
	}

	_Table._Scope = std::move(TepScope);
}
void SystematicAnalysis::Assembly_LoadSymbol(const ForType_Data& Item, SystematicAnalysis::LoadLibMode Mode)
{
	if (!Item.IsExported)
	{
		return;
	}

	if (Mode == LoadLibMode::GetTypes)
	{
		auto& Syb = Symbol_AddSymbol(SymbolType::ForType,ForTypeScope,ScopeHelper::ApendedStrings(Item._Scope,ForTypeScope), Item.AccessModifier);
		_Table.AddSymbolID(Syb, Symbol_GetSymbolID(&Item));

		auto Funcinfo = new ForTypeInfo();
		Syb.Info.reset(Funcinfo);


	}


	{
		auto& Syb = _Table.GetSymbol(Symbol_GetSymbolID(&Item));
		auto Funcinfo = Syb.Get_Info<ForTypeInfo>();

		Funcinfo->Funcs.reserve(Item._AddedMethods.size());
		for (auto& Item : Item._AddedMethods)
		{
			Assembly_LoadSymbol(Item, Mode);

			auto sym = _Table.Symbols.back().get();

			Funcinfo->Funcs.push_back(sym);
		}
	}

	if (Mode == LoadLibMode::FixTypes)
	{
		auto& Syb = _Table.GetSymbol(Symbol_GetSymbolID(&Item));

		Assembly_LoadType(Item._TargetType, Syb.VarType);
	}
}
void SystematicAnalysis::Assembly_LoadSymbol(const NameSpace_Data& Item, const String& FullName, SystematicAnalysis::LoadLibMode Mode)
{
	if (Mode == LoadLibMode::GetTypes)
	{
		auto& Syb = Symbol_AddSymbol(SymbolType::Namespace, FullName, FullName, AccessModifierType::Public);
	}
}
void SystematicAnalysis::Assembly_LoadSymbol(const ClassMethod& Item, SystematicAnalysis::LoadLibMode Mode)
{
	if (!Item.IsExport)
	{
		return;
	}

	if (Mode == LoadLibMode::GetTypes)
	{
		auto Name = ScopeHelper::GetNameFromFullName(Item.FullName);
		auto& Syb = Symbol_AddSymbol(SymbolType::Func, Name, Item.FullName, Item.Protection);
		_Table.AddSymbolID(Syb, Symbol_GetSymbolID(&Item));
		Syb.OutputIR = false;
		Syb.PassState = PassType::BuidCode;
		auto Funcinfo = new FuncInfo();
		Syb.Info.reset(Funcinfo);

		if (StringHelper::StartWith(Name,ClassConstructorfunc))
		{
			Funcinfo->_FuncType = FuncInfo::FuncType::New;
		}
		else if (StringHelper::StartWith(Name,ClassDestructorFunc))
		{
			Funcinfo->_FuncType = FuncInfo::FuncType::Drop;
		}
		else if (StringHelper::StartWith(Name,Overload_Cast_Func))
		{
			Funcinfo->_FuncType = FuncInfo::FuncType::Cast;
		}
		else if (StringHelper::StartWith(Name,Overload_For_Func))
		{
			Funcinfo->_FuncType = FuncInfo::FuncType::For;
		}
		else if (StringHelper::StartWith(Name,Overload_Invoke_Func))
		{
			Funcinfo->_FuncType = FuncInfo::FuncType::Invoke;
		}
		else if (StringHelper::StartWith(Name,Overload_Index_Func))
		{
			Funcinfo->_FuncType = FuncInfo::FuncType::Index;
		}	
		else
		{
			bool wasset = false;
			{
				for (auto& item : Systematic_BinaryOverloadData::data)
				{
					if (StringHelper::StartWith(Name,item.CompilerName))
					{
						Funcinfo->_FuncType = item.Type;
						wasset = true;
						break;
					}
				}
			}
			if (wasset == false)
			{
				for (auto& item : Systematic_PostfixOverloadData::data)
				{
					if (StringHelper::StartWith(Name,item.CompilerName))
					{
						Funcinfo->_FuncType = item.Type;
						break;
					}
				}
			}
			if (wasset == false)
			{
				for (auto& item : Systematic_UrinaryOverloadData::data)
				{
					if (StringHelper::StartWith(Name,item.CompilerName))
					{
						Funcinfo->_FuncType = item.Type;
						break;
					}
				}
			}
			if (wasset == false)
			{
				for (auto& item : Systematic_CompoundOverloadData::data)
				{
					if (StringHelper::StartWith(Name,item.CompilerName))
					{
						Funcinfo->_FuncType = item.Type;
						break;
					}
				}
			}
			if (wasset == false)
			{
				for (auto& item : Systematic_MemberOverloadData::data)
				{
					if (StringHelper::StartWith(Name,item.CompilerName))
					{
						Funcinfo->_FuncType = item.Type;
						break;
					}
				}
			}
		}

		LoadFuncInfoGetTypes(Funcinfo, Item);
	}
	else if (Mode == LoadLibMode::FixTypes)
	{
		auto& Syb = _Table.GetSymbol(Symbol_GetSymbolID(&Item));
		auto Funcinfo = Syb.Get_Info<FuncInfo>();

		LoadFuncInfoFixTypes(Funcinfo, Item);

		Syb.VarType = Funcinfo->Ret;
	}
}

void SystematicAnalysis::LoadFuncInfoGetTypes(UCodeLang::FrontEnd::FuncInfo* Funcinfo, const UCodeLang::ClassMethod& Item)
{
	Funcinfo->FullName = "";
	for (auto& val : Item.DecorationName)
	{
		if (val == NameDecoratior::ParSpit) 
		{
			break;
		}
		Funcinfo->FullName += val;
	}
	Funcinfo->FrontParIsUnNamed = Item.IsThisFunction;
	Funcinfo->IsUnsafe = Item.IsUnsafe;
	Funcinfo->IsExternC = Item.IsExternC;
	Funcinfo->IsRemoved = Item.IsRemoved;
	Funcinfo->IsTraitDynamicDispatch = Item.IsTraitDynamicDispatch;

	Funcinfo->Pars.resize(Item.ParsType.size());
}
void SystematicAnalysis::LoadFuncInfoFixTypes(FuncInfo* Funcinfo, const ClassMethod& Item)
{
	Assembly_LoadType(Item.RetType, Funcinfo->Ret);

	for (size_t i = 0; i < Funcinfo->Pars.size(); i++)
	{
		const ClassMethod::Par& ItemPar = Item.ParsType[i];
		Funcinfo->Pars[i].IsOutPar = ItemPar.IsOutPar;
		Assembly_LoadType(ItemPar.Type, Funcinfo->Pars[i].Type);
	}
}

void SystematicAnalysis::Assembly_AddClass(const Vector<Unique_ptr<AttributeNode>>& attributes, const NeverNullPtr<Symbol> ClassSyb)
{
	const ClassInfo* Class = ClassSyb->Get_Info<ClassInfo>();
	Class_Data& VClass = _Lib.Get_Assembly().AddClass((String)Class->Get_Name(), RemoveSymboolFuncOverloadMangling(Class->FullName));

	TypeSymbol AsType = TypeSymbol(ClassSyb->ID);

	VClass.Size = Type_GetSize(AsType).value();
	VClass.TypeID = Type_GetTypeID(AsType._Type, AsType._CustomTypeSymbol);
	VClass.AccessModifier = ClassSyb->Access;

	if (ClassSyb->NodePtr)
	{
		VClass.IsExported = ClassSyb->Get_NodeInfo<ClassNode>()->_IsExport;
	}
	else
	{
		VClass.IsExported = true;//most likey generated from enum or something similar
	}
	for (const auto& node : Class->Fields)
	{
		auto& Item = VClass.Fields.emplace_back();

		UAddress Size;
		Type_GetSize(node.Type, Size);

		Item.Name = ScopeHelper::GetNameFromFullName(node.Name);
		Item.Type = Assembly_ConvertToType(node.Type);
		Item.offset = Type_GetOffset(*Class, &node).value();

		String fullnameforfield = ClassSyb->FullName;
		ScopeHelper::GetApendedString(fullnameforfield, node.Name);
		auto fieldsymbol = Symbol_GetSymbol(fullnameforfield, SymbolType::Class_Field);

		if (fieldsymbol.has_value())
		{
			Item.Protection = fieldsymbol.value()->Access;
		}
		else
		{
			Item.Protection = AccessModifierType::Public;//most likey generated from enum or something similar

		}


	}

	for (const auto& Trait : Class->_InheritedTypes)
	{
		auto Typeid = Type_GetTypeID(TypesEnum::CustomType, Trait.Syb->ID);

		InheritedTrait_Data Item;
		Item.TraitID = Typeid;
		VClass.InheritedTypes.push_back(std::move(Item));
	}
	Assembly_ConvertAttributes(attributes, VClass.Attributes.Attributes);
}

ReflectionTypeInfo SystematicAnalysis::Assembly_ConvertToType(const TypeSymbol& Type)
{
	ReflectionTypeInfo r;
	r._Type = Type._Type;
	r._CustomTypeID = Type_GetTypeID(Type._Type, Type._CustomTypeSymbol);

	r._IsAddress = Type.IsAddress();
	r._IsAddressArray = Type.IsAddressArray();
	r._Isimmutable = Type.Isimmutable();
	r._IsDynamic = Type.IsDynamicTrait();
	r._MoveData = Type._MoveData;
	return r;
}

void SystematicAnalysis::Assembly_AddEnum(const NeverNullPtr<Symbol> ClassSyb)
{
	auto& Syb = *ClassSyb;
	const EnumInfo* ClassInf = Syb.Get_Info<EnumInfo>();

	Enum_Data& EnumData = _Lib.Get_Assembly().AddEnum(ScopeHelper::GetNameFromFullName(Syb.FullName), RemoveSymboolFuncOverloadMangling(Syb.FullName));
	EnumData.BaseType = Assembly_ConvertToType(ClassInf->Basetype);
	EnumData.TypeID = Type_GetTypeID(TypesEnum::CustomType, Syb.ID);
	EnumData.AccessModifier = ClassSyb->Access;

	if (ClassSyb->NodePtr) 
	{
		EnumData.IsExported = ClassSyb->Get_NodeInfo<EnumNode>()->_IsExport;
	}
	else
	{
		EnumData.IsExported = true;
	}

	if (ClassInf->FuncDestructer)
	{
		EnumData.DestructorFuncFullName = IR_MangleName(ClassInf->FuncDestructer.value());
	}
	if (ClassInf->FuncMoveContructer)
	{
		EnumData.MoveFuncFullName = IR_MangleName(ClassInf->FuncMoveContructer.value());
	}
	if (ClassInf->FuncCopyContructer)
	{
		EnumData.MoveFuncFullName = IR_MangleName(ClassInf->FuncCopyContructer.value());
	}

	EnumData.Values.resize(ClassInf->Fields.size());
	for (size_t i = 0; i < ClassInf->Fields.size(); i++)
	{
		auto& ClassDataItem = ClassInf->Fields[i];
		auto& EnumDataItem = EnumData.Values[i];
		EnumDataItem.Name = ClassDataItem.Name;
		EnumDataItem._Data.Resize(ClassDataItem.Ex.ObjectSize);
		memcpy(EnumDataItem._Data.Get_Data(), ClassDataItem.Ex.Object_AsPointer.get(), ClassDataItem.Ex.ObjectSize);
	}
	if (ClassInf->VariantData)
	{
		auto UnionFullName = Str_GetUnrefencedableName(Str_GetEnumVariantUnionName(ClassInf->FullName));
		auto UnionName = Str_GetEnumVariantUnionName(Str_GetUnrefencedableName((String)ClassInf->Get_Name()));

		Class_Data& EnumUnion = _Lib.Get_Assembly().AddClass(UnionName, UnionFullName);

		auto& UnionSyb = Symbol_AddSymbol(SymbolType::Type_class, UnionName, UnionFullName, AccessModifierType::Default);
		UnionSyb.OutputIR = false; // used only to have the union have a type.
		_Table.AddSymbolID(UnionSyb, Symbol_GetSymbolID((Node&)UnionSyb));

		EnumUnion.TypeID = Type_GetTypeID(TypesEnum::CustomType, UnionSyb.ID);
		EnumData.EnumVariantUnion = EnumUnion.TypeID;

		auto& List = ClassInf->VariantData.value().Variants;

		size_t MaxSize = 0;
		for (size_t i = 0; i < List.size(); i++)
		{
			auto& Item = List[i];

			if (Item.ClassSymbol.has_value())
			{
				Symbol* Sym = Symbol_GetSymbol(Item.ClassSymbol.value()).value();
				Sym->PassState = PassType::Done;

				Assembly_AddClass({}, Sym); // has '!' post fix so its Unrefencedable

				auto Type = TypeSymbol(Sym->ID);

				ClassField V;
				V.offset = 0;
				V.Name = ClassInf->Fields[i].Name;
				V.Type = Assembly_ConvertToType(Type);
				EnumUnion.Fields.push_back(std::move(V));

				auto& EnumDataItem = EnumData.Values[i];
				EnumDataItem.EnumVariantType = Assembly_ConvertToType(Type);

				size_t TypeSize = Type_GetSize(Type).value();
				if (TypeSize > MaxSize)
				{
					MaxSize = TypeSize;
				}
			}
			else
			{
				if (Item.Types.size())
				{

					auto Type = Item.Types.front();
					ClassField V;
					V.offset = 0;
					V.Name = ClassInf->Fields[i].Name;
					V.Type = Assembly_ConvertToType(Type);
					EnumUnion.Fields.push_back(std::move(V));

					auto& EnumDataItem = EnumData.Values[i];
					EnumDataItem.EnumVariantType = V.Type;

					size_t TypeSize = Type_GetSize(Type).value();
					if (TypeSize > MaxSize)
					{
						MaxSize = TypeSize;
					}
				}
			}

			for (auto& Item2 : Item.Types)
			{
				FileDependency_AddDependencyToCurrentFile(Item2);
			}
		}

		EnumUnion.Size = MaxSize;
	}
}
void SystematicAnalysis::Assembly_AddStaticArray(const NeverNullPtr<Symbol> ClassSyb)
{
	const StaticArrayInfo* info = ClassSyb->Get_Info<StaticArrayInfo>();

	String AssemblyName = ToString(info->Type) + "[/" + std::to_string(info->Count) + "]";

	auto& Assembly = _Lib.Get_Assembly();

	if (!Assembly.Find_Node(AssemblyName))
	{
		auto& node = Assembly.AddStaticArray(AssemblyName, AssemblyName);

		node.TypeID = Type_GetTypeID(TypesEnum::CustomType, ClassSyb->ID);
		node.Count = info->Count;
		node.BaseType = Assembly_ConvertToType(info->Type);
	}
}

Class_Data* SystematicAnalysis::Assembly_GetAssemblyClass(const String& FullName)
{
	if (_ClassStack.empty())
	{
		auto& Assembly = _Lib.Get_Assembly();

		auto globalAssemblyObjectName = (String_view)ScopeHelper::_globalAssemblyObject;

		auto Ptr = Assembly.Find_Class(globalAssemblyObjectName);
		if (Ptr == nullptr)
		{
			UCodeLangUnreachable();
		}
		return Ptr;
	}
	else
	{
		auto& Assembly = _Lib.Get_Assembly();
		auto ClassName = ScopeHelper::GetReMoveScope((String_view)FullName);
		for (auto& Item : Assembly.Classes)
		{
			if (Item->FullName == ClassName && Item->Get_Type() ==ClassType::Class)
			{
				return &Item->Get_ClassData();
			}
		}
	}
	if (StringHelper::Contains(FullName,ForTypeScope))
	{
		auto& Assembly = _Lib.Get_Assembly();
		auto globalAssemblyObjectName = (String_view)ScopeHelper::_globalAssemblyObject;

		auto Ptr = Assembly.Find_Class(globalAssemblyObjectName);
		if (Ptr == nullptr)
		{
			UCodeLangUnreachable();
		}
		return Ptr;

	}
	UCodeLangUnreachable();
}

SystematicAnalysis::CaptureErrorContext SystematicAnalysis::GetErrorCaptureContext()
{
	SystematicAnalysis::CaptureErrorContext r;
	r.errcount = this->_ErrorsOutput->Get_ErrorCount();

	return r;
}
bool SystematicAnalysis::ErrorCaptureHasErrors(const CaptureErrorContext& capture)
{
	return capture.errcount != this->_ErrorsOutput->Get_ErrorCount();
}
void SystematicAnalysis::Lib_BuildLibs(bool DoIR)
{
	// if (_Settings->_Type != OutPutType::IRAndSymbols)
	{
		for (size_t i = 0; i < _Libs->size(); i++)
		{
			auto Item = (*_Libs)[i];

			Lib_BuildLib(*Item, (*_LibsNames)[i], DoIR);
		}
	}
}
void SystematicAnalysis::Lib_BuildLib(const UClib& lib, const Path& LibName, bool DoIR)
{
	if (!DoIR)
	{
		ClassAssembly::PushCopyClasses(lib._Assembly, _Lib.Get_Assembly());
	}
	else
	{
		auto IRLayer = lib.GetLayer(UCode_CodeLayer_IR_Name);

		bool GotIRCode = false;
		if (IRLayer)
		{
			IRBuilder IRToImport;
			auto& LayerInfo = IRLayer->_Data.Get<CodeLayer::JustData>();

			if (IRBuilder::FromBytes(IRToImport, BytesView((Byte*)LayerInfo._Data.data(), LayerInfo._Data.size())))
			{
				GotIRCode = true;
				_IR_Builder.CombineWith(std::move(IRToImport));
			}
		}

		if (GotIRCode == false)
		{
			LogError(ErrorCodes::CouldNotFindFunc, 0, 0, "Cant get IR from '" + LibName.generic_string() + "' Object file.Try deleting it");
		}
	}
}

void SystematicAnalysis::ToIntFile(FileNode_t* File, const Path& path)
{
	auto& FileData = GetFileData(File);

	UClib Tep;
	auto& globalAssemblyObject = Tep.Get_Assembly().AddClass(ScopeHelper::_globalAssemblyObject, ScopeHelper::_globalAssemblyObject);

	for (size_t i = 0; i < FileData.AssemblyInfoSpan.Count; i++)
	{
		auto& Item = _Lib.Get_Assembly().Classes[FileData.AssemblyInfoSpan.Index + i];
		Tep.Get_Assembly().Classes.push_back(Unique_ptr<AssemblyNode>(Item.get()));
	}

	for (size_t i = 0; i < FileData.GlobalObjectMethodInfoSpan.Count; i++)
	{
		auto& Item = _Lib.Get_Assembly().Get_GlobalObject_Class()->Methods[FileData.GlobalObjectMethodInfoSpan.Index + i];
		globalAssemblyObject.Methods.push_back(Item);
	}

	IRBuilder TepIR;
	TepIR._StaticInit.NewBlock(".");
	TepIR._threadInit.NewBlock(".");
	TepIR._StaticdeInit.NewBlock(".");
	TepIR._threaddeInit.NewBlock(".");

	for (size_t i = 0; i < FileData.IRInitStaticSpan.Count; i++)
	{
		size_t Index = FileData.IRInitStaticSpan.Index + i;
		auto& Item = _IR_Builder._StaticInit.Blocks.front()->Instructions[Index];

		TepIR._StaticInit.Blocks.front()->Instructions.push_back(Unique_ptr<IRInstruction>(Item.get()));
	}
	for (size_t i = 0; i < FileData.IRInitThreadSpan.Count; i++)
	{
		size_t Index = FileData.IRInitThreadSpan.Index + i;
		auto& Item = _IR_Builder._threadInit.Blocks.front()->Instructions[Index];

		TepIR._threadInit.Blocks.front()->Instructions.push_back(Unique_ptr<IRInstruction>(Item.get()));
	}
	for (size_t i = 0; i < FileData.IRDeInitStaticSpan.Count; i++)
	{
		size_t Index = FileData.IRDeInitStaticSpan.Index + i;
		auto& Item = _IR_Builder._StaticdeInit.Blocks.front()->Instructions[Index];

		TepIR._StaticdeInit.Blocks.front()->Instructions.push_back(Unique_ptr<IRInstruction>(Item.get()));
	}
	for (size_t i = 0; i < FileData.IRDeInitThreadSpan.Count; i++)
	{
		size_t Index = FileData.IRDeInitThreadSpan.Index + i;
		auto& Item = _IR_Builder._threaddeInit.Blocks.front()->Instructions[Index];

		TepIR._threaddeInit.Blocks.front()->Instructions.push_back(Unique_ptr<IRInstruction>(Item.get()));
	}

	for (size_t i = 0; i < FileData.IRFuncsSpan.Count; i++)
	{
		size_t Index = FileData.IRFuncsSpan.Index + i;
		auto& Item = _IR_Builder.Funcs[Index];

		TepIR.Funcs.push_back(Unique_ptr<IRFunc>(Item.get()));
	}

	for (size_t i = 0; i < FileData.IRSymbolSpan.Count; i++)
	{
		size_t Index = FileData.IRSymbolSpan.Index + i;
		auto& Item = _IR_Builder._Symbols[Index];

		TepIR._Symbols.push_back(Unique_ptr<IRSymbolData>(Item.get()));
	}

	TepIR.ConstStaticStrings = _IR_Builder.ConstStaticStrings;
	TepIR._Map = _IR_Builder._Map;

	auto IRLayer = Tep.AddLayer(UCode_CodeLayer_IR_Name);
	CodeLayer::JustData V;
	V._Data = TepIR.ToBytes().MoveToVector();
	IRLayer->_Data = std::move(V);

	Tep.ToFile(&Tep, path);

	{ // was borrowed.
		for (size_t i = 0; i < Tep.Get_Assembly().Classes.size(); i++)
		{
			auto& Item = Tep.Get_Assembly().Classes[i];
			Item.release();
		}
		for (auto& Item : TepIR._StaticInit.Blocks.front()->Instructions)
		{
			Item.release();
		}
		for (auto& Item : TepIR._threadInit.Blocks.front()->Instructions)
		{
			Item.release();
		}
		for (auto& Item : TepIR._StaticdeInit.Blocks.front()->Instructions)
		{
			Item.release();
		}
		for (auto& Item : TepIR._threaddeInit.Blocks.front()->Instructions)
		{
			Item.release();
		}
		for (auto& Item : TepIR.Funcs)
		{
			Item.release();
		}
		for (auto& Item : TepIR._Symbols)
		{
			Item.release();
		}
	}
}

UCodeLangFrontEnd

#endif