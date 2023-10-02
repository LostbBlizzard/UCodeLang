#include "UCodeLang/Compliation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::Assembly_ConvertAttributes(const Vector<Unique_ptr<AttributeNode>>& nodes, Vector<UsedTagValueData>& Out)
{
	Out.resize(nodes.size());
	for (size_t i = 0; i < nodes.size(); i++)
	{
		Assembly_ConvertAttribute(*nodes[i], Out[i]);
	}
}
void SystematicAnalysis::Assembly_ConvertAttribute(const AttributeNode& nodes, UsedTagValueData& Out)
{
	auto Syb = Symbol_GetSymbol(Symbol_GetSymbolID(nodes));
	Out.TypeID = Type_GetTypeID(TypesEnum::CustomType, Syb->VarType._CustomTypeSymbol);
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

	while (Mode != LoadLibMode::Done)
	{
		for (size_t i = 0; i < _Libs->size(); i++)
		{
			auto& Item = (*_Libs)[i];
			FileNode& FileNode = _LibsFiles[i];
			this->_LookingAtFile = &FileNode;

			if (LoadLibMode::GetTypes == Mode)
			{
				for (auto& LibNode : Item->_Assembly.Classes)
				{
					_Lib._Assembly.Classes.push_back(Unique_ptr<AssemblyNode>(LibNode.get()));//make ref
				}
			}

			Assembly_LoadLibSymbols(*Item, Mode);
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
		auto Ptr = Item.release();//are just refs
	}
	_Lib._Assembly.Classes.clear();//remove nullptr Nodes

	//The CPU is going to hate this.
	for (auto& Item : _Lib_TypesToFix)
	{
		*Item.TypeToFix = *Item.ToGetTypeFrom;
	}
	_Lib_TypesToFix.clear();//free Mem.


	auto globalAssemblyObjectName = (String_view)ScopeHelper::_globalAssemblyObject;
	_Lib.Get_Assembly().AddClass(String(globalAssemblyObjectName), String(globalAssemblyObjectName));

}
void SystematicAnalysis::Assembly_LoadLibSymbols(const UClib& lib, LoadLibMode Mode)
{

	auto OutputType = Output_TypeAsLibType();
	auto libType = lib._LibType;

	auto GlobalObject = lib.Get_Assembly().Get_GlobalObject_Class();
	if (GlobalObject)
	{
		String Scope;
		Assembly_LoadClassSymbol(*GlobalObject, Scope, Scope, Mode);
	}


	for (auto& Item : lib.Get_Assembly().Classes)
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
		default:
			break;
		}
	}

}
void SystematicAnalysis::Assembly_LoadClassSymbol(const Class_Data& Item, const String& FullName, const String& Scope, SystematicAnalysis::LoadLibMode Mode)
{
	auto TepScope = std::move(_Table._Scope);

	_Table._Scope = {};
	_Table._Scope.ThisScope = Scope;

	if (Mode == LoadLibMode::GetTypes)
	{
		auto Name = ScopeHelper::GetNameFromFullName(FullName);
		auto& Syb = Symbol_AddSymbol(SymbolType::Type_class, Name, FullName, AccessModifierType::Public);
		_Table.AddSymbolID(Syb, Symbol_GetSymbolID(&Item));

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
			auto& InfoItem = Info->Fields[i];

			InfoItem.Name = FieldItem.Name;
			//InfoItem.offset = FieldItem.offset;
		}
	}
	else if (Mode == LoadLibMode::FixTypes)
	{
		auto& Syb = _Table.GetSymbol(Symbol_GetSymbolID(&Item));
		ClassInfo* Info = Syb.Get_Info<ClassInfo>();

		for (size_t i = 0; i < Item.Fields.size(); i++)
		{
			const auto& FieldItem = Item.Fields[i];
			auto& InfoItem = Info->Fields[i];
			Assembly_LoadType(FieldItem.Type, InfoItem.Type);
		}
	}

	for (auto& Item : Item.Methods)
	{
		Assembly_LoadSymbol(Item, Mode);
	}

	_Table._Scope = std::move(TepScope);
}
void SystematicAnalysis::Assembly_LoadEnumSymbol(const Enum_Data& Item, const String& FullName, const String& Scope, SystematicAnalysis::LoadLibMode Mode)
{
	auto TepScope = std::move(_Table._Scope);

	_Table._Scope = {};
	_Table._Scope.ThisScope = Scope;

	if (Mode == LoadLibMode::GetTypes)
	{
		auto Name = ScopeHelper::GetNameFromFullName(FullName);
		auto& Syb = Symbol_AddSymbol(SymbolType::Enum, Name, FullName, AccessModifierType::Public);
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
		auto  enumInfo = Syb.Get_Info<EnumInfo>();

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
						if (Syb.Type == SymbolType::Type_class)
						{
							bool IsUnNamed = false;
							if (Syb.FullName.back() == '!')//the unnamed Enum Sybol post fix
							{
								IsUnNamed = true;
							}

							if (IsUnNamed)
							{
								ClassInfo* CInfo = Syb.Get_Info<ClassInfo>();

								VariantItem.Types.resize(CInfo->Fields.size());//Field type may not be loaded. 

								for (size_t ix = 0; ix < CInfo->Fields.size(); ix++)
								{
									auto& Item = CInfo->Fields[ix];

									LibLoadTypeSeter Seter;
									Seter.ToGetTypeFrom = &Item.Type;
									Seter.TypeToFix = &VariantItem.Types[ix];
									_Lib_TypesToFix.push_back(Seter);
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
	auto TepScope = std::move(_Table._Scope);

	_Table._Scope = {};
	_Table._Scope.ThisScope = Scope;

	if (Mode == LoadLibMode::GetTypes)
	{
		auto Name = ScopeHelper::GetNameFromFullName(FullName);
		auto& Syb = Symbol_AddSymbol(
			Item.HardAliasTypeID.has_value()
			? SymbolType::Hard_Type_alias : SymbolType::Type_alias, Name, FullName, AccessModifierType::Public);
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
void SystematicAnalysis::Assembly_LoadSymbol(const ClassMethod& Item, SystematicAnalysis::LoadLibMode Mode)
{
	if (Mode == LoadLibMode::GetTypes)
	{
		auto Name = ScopeHelper::GetNameFromFullName(Item.FullName);
		auto& Syb = Symbol_AddSymbol(SymbolType::Func, Name, Item.FullName, AccessModifierType::Public);
		_Table.AddSymbolID(Syb, Symbol_GetSymbolID(&Item));
		Syb.OutputIR = false;
		Syb.PassState = PassType::BuidCode;
		auto Funcinfo = new FuncInfo();
		Syb.Info.reset(Funcinfo);

		Funcinfo->FullName = Syb.FullName;
		Funcinfo->FrontParIsUnNamed = Item.IsThisFuncion;
		Funcinfo->IsUnsafe = Item.IsUnsafe;
		Funcinfo->IsExternC = Item.IsExternC;
		Funcinfo->IsRemoved = Item.IsRemoved;

		Funcinfo->Pars.resize(Item.ParsType.size());
	}
	else if (Mode == LoadLibMode::FixTypes)
	{
		auto& Syb = _Table.GetSymbol(Symbol_GetSymbolID(&Item));
		auto Funcinfo = Syb.Get_Info<FuncInfo>();

		Assembly_LoadType(Item.RetType, Funcinfo->Ret);
		Syb.VarType = Funcinfo->Ret;

		for (size_t i = 0; i < Funcinfo->Pars.size(); i++)
		{
			const ClassMethod::Par& ItemPar = Item.ParsType[i];
			Funcinfo->Pars[i].IsOutPar = ItemPar.IsOutPar;
			Assembly_LoadType(ItemPar.Type, Funcinfo->Pars[i].Type);
		}
	}
}

void SystematicAnalysis::Assembly_AddClass(const Vector<Unique_ptr<AttributeNode>>& attributes, const NeverNullPtr<Symbol> ClassSyb)
{
	const ClassInfo* Class = ClassSyb->Get_Info<ClassInfo>();
	Class_Data& VClass = _Lib.Get_Assembly().AddClass((String)Class->Get_Name(), Class->FullName);

	TypeSymbol AsType = TypeSymbol(ClassSyb->ID);

	VClass.Size = Type_GetSize(AsType).value();
	VClass.TypeID = Type_GetTypeID(AsType._Type, AsType._CustomTypeSymbol);

	for (const auto& node : Class->Fields)
	{
		auto& Item = VClass.Fields.emplace_back();

		UAddress Size;
		Type_GetSize(node.Type, Size);

		Item.Name = ScopeHelper::GetNameFromFullName(node.Name);
		Item.Type = Assembly_ConvertToType(node.Type);
		Item.offset = Type_GetOffset(*Class, &node).value();
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

	Enum_Data& EnumData = _Lib.Get_Assembly().AddEnum(ScopeHelper::GetNameFromFullName(Syb.FullName), Syb.FullName);
	EnumData.BaseType = Assembly_ConvertToType(ClassInf->Basetype);
	EnumData.TypeID = Type_GetTypeID(TypesEnum::CustomType, Syb.ID);

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
		UnionSyb.OutputIR = false;//used only to have the union have a type.
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

				Assembly_AddClass({}, Sym);//has '!' post fix so its Unrefencedable

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
			if (Item->FullName == ClassName)
			{
				return &Item->Get_ClassData();
			}

		}
	}
	UCodeLangUnreachable();
}
void SystematicAnalysis::Lib_BuildLibs()
{
	//if (_Settings->_Type != OutPutType::IRAndSymbols) 
	{
		for (size_t i = 0; i < _Libs->size(); i++)
		{
			auto Item = (*_Libs)[i];

			Lib_BuildLib(*Item, (*_LibsNames)[i]);
		}
	}
}
void SystematicAnalysis::Lib_BuildLib(const UClib& lib, const Path& LibName)
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
	else
	{
		ClassAssembly::PushCopyClasses(lib._Assembly, _Lib.Get_Assembly());
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

	{//was borrowed.
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