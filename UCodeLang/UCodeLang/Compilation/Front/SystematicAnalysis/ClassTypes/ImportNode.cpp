#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart
ImportBindType SymbolTypeToImportBindType(SymbolType Type)
{
	if (Type == SymbolType::Type_class ||
		Type == SymbolType::Type_alias ||
		Type == SymbolType::Hard_Type_alias ||
		Type == SymbolType::Type_Pack ||
		Type == SymbolType::Type_StaticArray ||
		Type == SymbolType::Generic_class ||
		Type == SymbolType::Trait_class)
	{
		return ImportBindType::Type;
	}
	else if (Type == SymbolType::Func)
	{
		return ImportBindType::Func;
	}
	else if (Type == SymbolType::GenericFunc)
	{
		return ImportBindType::GenericFunc;
	}
	return ImportBindType::Null;
}

void SystematicAnalysis::OnImportNode(const ImportStatement& node)
{
	SymbolID ImportSymbolID = Symbol_GetSymbolID(node);
	if (_PassType == PassType::GetTypes)
	{
		auto& Syb = Symbol_AddSymbol(SymbolType::Null, CompilerGenerated("importData") + std::to_string(node._Token->OnLine), "", AccessModifierType::Public);

		Imports_Info* NewImports = new  Imports_Info();
		Syb.Info.reset(NewImports);
		_Table.AddSymbolID(Syb, ImportSymbolID);


		NewImports->NewAliases.resize(node._Imports.size());

		for (auto& Item : node._Imports)
		{


			for (auto& Item2 : Item._ImportedSymbol._ScopedName)
			{
				if (Item2._operator != ScopedName::Operator_t::ScopeResolution
					&& Item2._operator != ScopedName::Operator_t::Null)
				{
					LogError(ErrorCodes::ExpectingSequence, Item2._token->OnLine, Item2._token->OnPos,
						"Must use a '::' as Opetator Here");
				}
			}

		}
	}
	else if (_PassType == PassType::FixedTypes)
	{
		auto& ImportSyb = _Table.GetSymbol(ImportSymbolID);
		Imports_Info* NewImports = ImportSyb.Get_Info<Imports_Info>();

		for (size_t i = 0; i < node._Imports.size(); i++)
		{
			auto& Item = node._Imports[i];
			auto& ImportInfo = NewImports->NewAliases[i];

			String Name;
			if (node._StartingNameSpace.has_value())
			{
				Name += Str_GetScopedNameAsString(node._StartingNameSpace.value());
				Name += ScopeHelper::_ScopeSep;
			}

			Name += Str_GetScopedNameAsString(Item._ImportedSymbol);


			auto List = GetSymbolsWithName(Name);

			if (List.empty())
			{
				auto Token = Item._ImportedSymbol._ScopedName.front()._token;
				LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos,
					"Cant find any Symbol for '" + Name + "'");
				continue;
			}
			if (Item._AliasName.has_value())
			{
				auto& AliasName = Item._AliasName.value();

				ImportInfo.NewSymbols.resize(List.size());
				bool IsOkToBind = false;
				bool IsOuterfile = false;

				for (size_t i = 0; i < List.size(); i++)
				{
					auto& SybToBind = List[i];
					auto& NewSybInfo = ImportInfo.NewSymbols[i];
					ImportBindType SybType = SymbolTypeToImportBindType(SybToBind->Type);

					if (SybType == ImportBindType::Type
						|| SybType == ImportBindType::Func)
					{

						IsOkToBind = true;
					}
					if (ImportSyb._File != SybToBind->_File)
					{
						IsOuterfile = true;
					}
					NewSybInfo.Type = SybType;
				}

				if (!IsOkToBind)
				{
					auto Token = Item._ImportedSymbol._ScopedName.front()._token;
					auto Symbol = List.front();
					LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos,
						"Cant Map Symbol '" + Symbol->FullName + "[" + ToString(Symbol->Type) + "]' to Alias");
				}
				else if (!IsOuterfile)
				{
					auto Token = Item._ImportedSymbol._ScopedName.front()._token;
					auto Symbol = List.front();

					String V = "importing '" + Symbol->FullName + "' but it's Declared in this file.";
					LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos, V);

				}
				else
				{
					FileNodeData::ImportData _ImportData;
					_ImportData.ImportSymbolFullName = Name;
					_ImportData.IsImportUsed = &ImportInfo.IsUsed;

					for (size_t i = 0; i < List.size(); i++)
					{
						auto& SybToBind = List[i];
						auto& NewSybInfo = ImportInfo.NewSymbols[i];

						if (NewSybInfo.Type == ImportBindType::Type)
						{
							auto& NewSyb = Symbol_AddSymbol(SymbolType::Type_alias, (String)AliasName->Value._String, (String)AliasName->Value._String, AccessModifierType::Public);
							_Table.AddSymbolID(NewSyb, Symbol_GetSymbolID(&NewSybInfo));

							NewSyb.VarType = SybToBind->VarType;

							NewSybInfo.Sym = &NewSyb;

							NewSyb.PassState = SybToBind->PassState;
							NewSyb.VarType = SybToBind->VarType;
						}
						else if (NewSybInfo.Type == ImportBindType::Func)
						{
							auto& NewSyb = Symbol_AddSymbol(SymbolType::Func, (String)AliasName->Value._String, (String)AliasName->Value._String, AccessModifierType::Public);
							_Table.AddSymbolID(NewSyb, Symbol_GetSymbolID(&NewSybInfo));
							NewSybInfo.Sym = &NewSyb;

							FuncInfo* NewFunc = new FuncInfo();
							NewSyb.Info.reset(NewFunc);

							NewSyb.PassState = SybToBind->PassState;
							*NewFunc = *SybToBind->Get_Info<FuncInfo>();

							NewSyb.VarType = SybToBind->VarType;


						}
						_ImportData._AliasSymbols.push_back(NewSybInfo.Sym);

					}

					GetFileData(NeverNullptr<FileNode_t>(_LookingAtFile))._Imports.push_back(std::move(_ImportData));
				}

			}
			else
			{
				bool IsOuterfile = false;
				for (size_t i = 0; i < List.size(); i++)
				{
					auto& SybToBind = List[i];
					if (ImportSyb._File != SybToBind->_File)
					{
						IsOuterfile = true;
					}
				}

				if (!IsOuterfile)
				{
					auto Token = Item._ImportedSymbol._ScopedName.front()._token;
					auto Symbol = List.front();

					String V = "importing '" + Symbol->FullName + "' but it's Declared in this file.";
					LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos, V);

				}

				FileNodeData::ImportData _ImportData;
				_ImportData.ImportSymbolFullName = Name;
				_ImportData.IsImportUsed = &ImportInfo.IsUsed;
				GetFileData(NeverNullptr<FileNode_t>(_LookingAtFile))._Imports.push_back(std::move(_ImportData));
			}

		}
	}
	else if (_PassType == PassType::BuidCode)
	{
		Imports_Info* NewImports = _Table.GetSymbol(ImportSymbolID).Get_Info<Imports_Info>();
		for (size_t i = 0; i < node._Imports.size(); i++)
		{
			auto& Item = node._Imports[i];
			auto& ImportInfo = NewImports->NewAliases[i];

			for (auto& ItemSymbol : ImportInfo.NewSymbols)
			{
				if (ItemSymbol.Type == ImportBindType::Func)//build func?
				{

				}
			}

			if (!ImportInfo.IsUsed)
			{
				auto Token = Item._ImportedSymbol._ScopedName.front()._token;
				auto Name = Str_GetScopedNameAsString(Item._ImportedSymbol);

				LogError(ErrorCodes::ExpectingSequence, Token->OnLine, Token->OnPos, "'" + Name + "' Import Symbol was not Used");
			}
		}
	}
}


UCodeLangFrontEnd

#endif