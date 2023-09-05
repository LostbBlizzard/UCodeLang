#include "SystematicAnalysis.hpp"
#include "UCodeLang/Compliation/Helpers/KeyWords.hpp"

#include "UCodeLang/Compliation/Helpers/InstructionBuilder.hpp"
#include "UCodeLang/Compliation/Back/UCodeBackEnd/UCodeBackEnd.hpp"
#include "UCodeLang/Compliation/Helpers/ParseHelper.hpp"
#include "UCodeLang/Compliation/Helpers/NameDecoratior.hpp"
#include "UCodeLang/LangCore/DataType/Defer.hpp"
UCodeLangFrontStart


//Dev Note:So this may be the bigest file in the project.
//Some parts may be pulled out for the Language_Server.
//It's a lot if theres any Questions just ask Lost blizzard.
//I don't plan on separating into different files because of c++ compiles time
//and how everything is weirdly dependent on everything else and just may increase complexity.
//Its an self-contained mess of a file.
#define GenericTestStr CompilerGenerated("___GenericTest")

constexpr size_t GenericTestStrSize = sizeof(GenericTestStr);


constexpr size_t BuiltInCount = (size_t)Systematic_BuiltInFunctions::ID::Max - 1;

static const Array<Systematic_BuiltInFunctions::FunctionData, BuiltInCount> BuiltFuncList =
{
	 Systematic_BuiltInFunctions::FunctionData("Name",Systematic_BuiltInFunctions::ID::TypeInfo_GetName),
	 Systematic_BuiltInFunctions::FunctionData("FullName",Systematic_BuiltInFunctions::ID::TypeInfo_GetFullName),
};
Optional<Systematic_BuiltInFunctions::Func> Systematic_BuiltInFunctions::GetFunction(const String_view Name, const Vector<FunctionPar>& Pars, SystematicAnalysis& This)
{
	auto FuncName = ScopeHelper::GetNameFromFullName(Name);

	if ( (FuncName == "Name" || FuncName == "FullName") && Pars.size() == 1)
	{
		auto& Type = Pars.front();
		if (Type.IsOutPar == false)
		{
			
			bool WantsUmutCString =false;
			if (This._LookingForTypes.size())
			{
				auto Type = This._LookingForTypes.top();
				WantsUmutCString = Type._Type == TypesEnum::Char && Type._IsAddressArray && Type._Isimmutable;
			}

			if (Type.Type._TypeInfo == TypeInfoPrimitive::TypeInfo)
			{
				TypeSymbol NewType;
				NewType._CustomTypeSymbol = Type.Type._CustomTypeSymbol;
				NewType._Type = Type.Type._Type;

				

				
				String Value = This.ToString(NewType);
				if (FuncName == "Name")
				{
					Value = ScopeHelper::GetNameFromFullName(FuncName);
				}

				Func _Func;
				_Func.RetType = This.Type_GetStaticArrayType(TypesEnum::Char, Value.size());
				auto Ex = This.Eval_MakeEx(_Func.RetType);
				This.Eval_Set_ObjectAs(Ex, Value.data(), Value.size());


				_Func.EvalObject = std::move(Ex.EvaluatedObject);
				_Func.EvalAsCString = WantsUmutCString;
				return _Func;
			}
			else if (FuncName == "Name" && Type.Type._TypeInfo == TypeInfoPrimitive::ClassFieldInfo && Type.ExpressionNode)
			{
				auto EvalObject = This.Eval_EvaluateToAnyType(*ExpressionNodeType::As(Type.ExpressionNode));
				if (EvalObject.has_value())
				{
					auto& EvalObjectAsValue = EvalObject.value();
					const ClassField& Field = *This.Eval_Get_ObjectAs<const ClassField>(EvalObjectAsValue);

					const String& Value = Field.Field->Name;

					Func _Func;
					_Func.RetType = This.Type_GetStaticArrayType(TypesEnum::Char, Value.size());
					auto Ex = This.Eval_MakeEx(_Func.RetType);
					This.Eval_Set_ObjectAs(Ex,Value.data(), Value.size());

					_Func.EvalObject = std::move(Ex.EvaluatedObject);
					_Func.EvalAsCString = WantsUmutCString;
					return _Func;
				}
				else
				{
					Func _Func;
					_Func.RetType = TypesEnum::Null;
					return _Func;
				}

			}
			else if (FuncName == "Name" && Type.Type._TypeInfo == TypeInfoPrimitive::EnumFieldInfo && Type.ExpressionNode)
			{
				auto EvalObject = This.Eval_EvaluateToAnyType(*ExpressionNodeType::As(Type.ExpressionNode));
				if (EvalObject.has_value())
				{
					auto& EvalObjectAsValue = EvalObject.value();
					const EnumField& Field = *This.Eval_Get_ObjectAs<const EnumField>(EvalObjectAsValue);

					const String& Value = Field.Field->Name;

					Func _Func;
					_Func.RetType = This.Type_GetStaticArrayType(TypesEnum::Char, Value.size());
					auto Ex = This.Eval_MakeEx(_Func.RetType);
					This.Eval_Set_ObjectAs(Ex, Value.data(), Value.size());

					_Func.EvalObject = std::move(Ex.EvaluatedObject);
					_Func.EvalAsCString = WantsUmutCString;
					return _Func;
				}
				else
				{
					Func _Func;
					_Func.RetType = TypesEnum::Null;
					return _Func;
				}

			}

		}
	}
	if (FuncName == "Value" && Pars.size() == 1)
	{
		auto& Type = Pars.front();
		if (Type.IsOutPar == false)
		{
			if (Type.Type._TypeInfo == TypeInfoPrimitive::EnumFieldInfo && Type.ExpressionNode)
			{
				auto EvalObject = This.Eval_EvaluateToAnyType(*ExpressionNodeType::As(Type.ExpressionNode));
				if (EvalObject.has_value())
				{
					auto& EvalObjectAsValue = EvalObject.value();
					const EnumField& Field = *This.Eval_Get_ObjectAs<const EnumField>(EvalObjectAsValue);

					const auto& Value = Field.Field->Ex;

					Func _Func;
					_Func.RetType = Field._EnumInfo->Basetype;
					auto Ex = This.Eval_MakeEx(_Func.RetType);
					This.Eval_Set_ObjectAs(Ex, Value.Object_AsPointer.get(),Value.ObjectSize);

					_Func.EvalObject = std::move(Ex.EvaluatedObject);
					return _Func;
				}
				else
				{
					Func _Func;
					_Func.RetType = TypesEnum::Null;
					return _Func;
				}

			}
		}
	}
	if (FuncName == "Offset" && Pars.size() == 1)
	{
		auto& Type = Pars.front();
		if (Type.IsOutPar == false)
		{
			if (Type.Type._TypeInfo == TypeInfoPrimitive::ClassFieldInfo && Type.ExpressionNode)
			{
				auto EvalObject = This.Eval_EvaluateToAnyType(*ExpressionNodeType::As(Type.ExpressionNode));
				if (EvalObject.has_value())
				{
					auto& EvalObjectAsValue = EvalObject.value();
					const ClassField& Field = *This.Eval_Get_ObjectAs<const ClassField>(EvalObjectAsValue);

					auto OffsetInfo = This.Type_GetOffset(*Field._ClassInfo, Field.Field);

					const size_t& Value = OffsetInfo.value();

					Func _Func;
					_Func.RetType = TypesEnum::uIntPtr;
					auto Ex = This.Eval_MakeEx(_Func.RetType);
					This.Eval_Set_ObjectAs(Ex, Value);

					_Func.EvalObject = std::move(Ex.EvaluatedObject);
					return _Func;
				}
				else
				{
					Func _Func;
					_Func.RetType = TypesEnum::Null;
					return _Func;
				}

			}

		}
	}

	if (FuncName == "Type" && Pars.size() == 1)
	{
		auto& Type = Pars.front();
		if (Type.IsOutPar == false)
		{
			if (Type.Type._TypeInfo == TypeInfoPrimitive::ClassFieldInfo && Type.ExpressionNode)
			{
				auto EvalObject = This.Eval_EvaluateToAnyType(*ExpressionNodeType::As(Type.ExpressionNode));
				if (EvalObject.has_value())
				{
					auto& EvalObjectAsValue = EvalObject.value();
					const ClassField& Field = *This.Eval_Get_ObjectAs<ClassField>(EvalObjectAsValue);

					const TypeSymbol& Value = Field.Field->Type;

					Func _Func;
					_Func.RetType = Value;
					_Func.RetType.SetAsTypeInfo();
					auto Ex = This.Eval_MakeEx(_Func.RetType);
					This.Eval_Set_ObjectAs(Ex, Value);

					_Func.EvalObject = std::move(Ex.EvaluatedObject);
					return _Func;
				}
				else
				{
					Func _Func;
					_Func.RetType = TypesEnum::Null;
					return _Func;
				}

			}

		}
	}
	if (FuncName == "TypeID" && Pars.size() == 1)
	{
		auto& Type = Pars.front();
		if (Type.IsOutPar == false)
		{
			if (Type.Type._TypeInfo == TypeInfoPrimitive::TypeInfo)
			{
				TypeSymbol NewType = Type.Type;
				NewType.BindType();

				auto Value = This.Type_GetTypeID(NewType._Type, NewType._CustomTypeSymbol);

				if (This._Settings->PtrSize == IntSizes::Int32)
				{
					Value = (UInt32)Value;
				}
				else
				{
					Value = (UInt64)Value;
				}

				Func _Func;
				_Func.RetType = TypesEnum::uIntPtr;
				auto Ex = This.Eval_MakeEx(_Func.RetType);
				memcpy(Ex.EvaluatedObject.Object_AsPointer.get(),&Value, Ex.EvaluatedObject.ObjectSize);

				_Func.EvalObject = std::move(Ex.EvaluatedObject);
				return _Func;
			}
		}

	}

	if (Pars.size() == 1)
	{
		auto& Type = Pars.front();
		if (Type.IsOutPar == false
			&& Type.Type._TypeInfo == TypeInfoPrimitive::TypeInfo)
		{
			Optional<bool> Value;

			TypeSymbol NewType = Type.Type;
			if (FuncName == "IsAddress")
			{
				Value = NewType.IsAddress();
			}
			else if (FuncName == "IsAddressArray")
			{
				Value = NewType.IsAddressArray();
			}
			else if (FuncName == "IsImmutable")
			{
				Value = NewType.Isimmutable();
			}
			else if (FuncName == "IsDynamic")
			{
				Value = NewType.IsDynamicTrait();
			}
			else if (FuncName == "IsMoved")
			{
				Value = NewType.IsMovedType();
			}


			if (Value.has_value())
			{
				Func _Func;
				_Func.RetType = TypesEnum::Bool;
				auto Ex = This.Eval_MakeEx(_Func.RetType);
				This.Eval_Set_ObjectAs(Ex, Value.value());

				_Func.EvalObject = std::move(Ex.EvaluatedObject);
				return _Func;
			}
		}
	}
	

	if (FuncName == "BaseType")
	{
		if (Pars.size() == 1 )
		{
			auto& Par = Pars.front();
			if (Par.IsOutPar == false) 
			{	
				if (Par.Type._TypeInfo == TypeInfoPrimitive::EnumInfo)
				{
					const auto Sym = This.Symbol_GetSymbol(Par.Type).value();
					const auto classInfo = Sym->Get_Info<EnumInfo>();
					auto BaseType = classInfo->Basetype;

					Func _Func;
					_Func.RetType = BaseType;
					_Func.RetType.SetAsTypeInfo();
					auto Ex = This.Eval_MakeEx(_Func.RetType);
					This.Eval_Set_ObjectAs(Ex, BaseType);

					_Func.EvalObject = std::move(Ex.EvaluatedObject);

					return _Func;
				}
			}
		}
	}
	
	if (FuncName == "TryGetVariantUnion")
	{
		if (Pars.size() == 1)
		{
			auto& Par = Pars.front();
			if (Par.IsOutPar == false)
			{
				if (Par.Type._TypeInfo == TypeInfoPrimitive::EnumInfo)
				{
					const auto Sym = This.Symbol_GetSymbol(Par.Type).value();
					const auto classInfo = Sym->Get_Info<EnumInfo>();
					bool IsVariant = classInfo->VariantData.has_value();


					Func _Func;
					_Func.RetType = TypesEnum::Bool;
					_Func.RetType.SetAsTypeInfo();
					auto Ex = This.Eval_MakeEx(_Func.RetType);
					This.Eval_Set_ObjectAs(Ex, IsVariant);

					_Func.EvalObject = std::move(Ex.EvaluatedObject);

					{	
						Func::OutParData Par;
						if (IsVariant)
						{
							const auto& Variant = classInfo->VariantData.value();
							

							Par.Type = TypesEnum::InternalType;
							Par.Type._TypeInfo = TypeInfoPrimitive::EnumVariantInfo;

							auto Ex = This.Eval_MakeEx(Par.Type);
							This.Eval_Set_ObjectAs(Ex,&Variant);

							Par.EvalObject = std::move(Ex.EvaluatedObject);
						}
						_Func._OutPars.push_back(std::move(Par));
					}
					return _Func;
				}
			}
		}
	}


	if (Pars.size() == 2)
	{
		auto& Type = Pars.front();

		bool IsTypeInfo = Type.Type._TypeInfo == TypeInfoPrimitive::TypeInfo;
		bool OtherIsOutVal = Pars[1].IsOutPar == true && IsTypeInfo;

		if (OtherIsOutVal)
		{
			if (FuncName == "GetClassInfo")
			{
				const auto Sym = This.Symbol_GetSymbol(Type.Type);
				bool IsClass = Sym ? Sym.value()->Type == SymbolType::Type_class : false;

				Func _Func;
				_Func.RetType = TypesEnum::Bool;
				auto Ex = This.Eval_MakeEx(_Func.RetType);
				This.Eval_Set_ObjectAs(Ex, IsClass);

				_Func.EvalObject = std::move(Ex.EvaluatedObject);

				{
					Func::OutParData Par;
					if (IsClass)
					{
						Par.Type = Type.Type;
						Par.Type._IsAddress = false;//If Pass as Ref
						Par.Type._TypeInfo = TypeInfoPrimitive::ClassInfo;

						auto Ex = This.Eval_MakeEx(Par.Type);
						This.Eval_Set_ObjectAs(Ex, Par.Type);

						Par.EvalObject = std::move(Ex.EvaluatedObject);
					}
					_Func._OutPars.push_back(std::move(Par));
				}
				return _Func;
			}
			else if (FuncName == "GetEnumInfo")
			{
				const auto Sym = This.Symbol_GetSymbol(Type.Type);
				bool IsEnum = Sym.has_value() ? Sym.value()->Type == SymbolType::Enum : false;

				Func _Func;
				_Func.RetType = TypesEnum::Bool;
				auto Ex = This.Eval_MakeEx(_Func.RetType);
				This.Eval_Set_ObjectAs(Ex, IsEnum);

				_Func.EvalObject = std::move(Ex.EvaluatedObject);

				{
					Func::OutParData Par;
					if (IsEnum) 
					{
						Par.Type = Type.Type;
						Par.Type._IsAddress = false;//If Pass as Ref
						Par.Type._TypeInfo = TypeInfoPrimitive::EnumInfo;

						auto Ex = This.Eval_MakeEx(Par.Type);
						This.Eval_Set_ObjectAs(Ex, Par.Type);

						Par.EvalObject = std::move(Ex.EvaluatedObject);
					}
					_Func._OutPars.push_back(std::move(Par));
				}
				return _Func;
			}
		}
	
	}

	
	if (FuncName == "Fields" && Pars.size() == 1)
	{
		auto& Type = Pars.front();
		bool IsTypeInfo = Type.Type._TypeInfo == TypeInfoPrimitive::TypeInfo;
		switch (Type.Type._TypeInfo)
		{
		case TypeInfoPrimitive::ClassInfo:
		{
			const auto Sym = This.Symbol_GetSymbol(Type.Type).value();
			const auto classInfo = Sym->Get_Info<ClassInfo>();
		
			const auto& Fields = classInfo->Fields;


			Vector<ClassField> FieldsAs;
			FieldsAs.resize(Fields.size());
			for (size_t i = 0; i < Fields.size(); i++)
			{
				FieldsAs[i].Field = &Fields[i];
				FieldsAs[i]._ClassInfo = classInfo;
			}

			TypeSymbol ArrItemType = TypesEnum::InternalType;
			ArrItemType._TypeInfo = TypeInfoPrimitive::ClassFieldInfo;

			Func _Func;
			_Func.RetType = This.Type_GetStaticArrayType(ArrItemType, FieldsAs.size());

			auto Ex = This.Eval_MakeEx(_Func.RetType);
			This.Eval_Set_ObjectAs(Ex, FieldsAs.data(), FieldsAs.size() * sizeof(ClassField));
			_Func.EvalObject = std::move(Ex.EvaluatedObject);

			return _Func;
		}
		break;
		case TypeInfoPrimitive::EnumInfo:
		{
			const auto Sym = This.Symbol_GetSymbol(Type.Type).value();
			const auto enumInfo = Sym->Get_Info<EnumInfo>();

			const auto& Fields = enumInfo->Fields;


			Vector<EnumField> FieldsAs;
			FieldsAs.resize(Fields.size());
			for (size_t i = 0; i < Fields.size(); i++)
			{
				FieldsAs[i].Field = &Fields[i];
				FieldsAs[i]._EnumInfo = enumInfo;
			}

			TypeSymbol ArrItemType = TypesEnum::InternalType;
			ArrItemType._TypeInfo = TypeInfoPrimitive::EnumFieldInfo;

			Func _Func;
			_Func.RetType = This.Type_GetStaticArrayType(ArrItemType, FieldsAs.size());

			auto Ex = This.Eval_MakeEx(_Func.RetType);
			This.Eval_Set_ObjectAs(Ex, FieldsAs.data(), FieldsAs.size() * sizeof(EnumField));
			_Func.EvalObject = std::move(Ex.EvaluatedObject);

			return _Func;
		}
		break;
		default:
			break;
		}
	}

	return {};
}

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

	return Analyze(files,libs);
}
bool SystematicAnalysis::Analyze(const Vector<NeverNullPtr<FileNode>>& Files, const Vector<NeverNullPtr<UClib>>& Libs)
{


	_PassType = PassType::Null;
	_Files = &Files;
	_Libs = &Libs;

	int V = 0;
	int* Ptr = &V;
	Symbol_GetSymbolID(Ptr);

	{
		for (const auto& File : *_Files)
		{
			_FilesData.AddValue(NeverNullptr<FileNode_t>(File.value()),Unique_ptr<FileNodeData>(new FileNodeData()));
		}

	}
	{
		_ForceImportArgWasPassed = _Settings->HasFlagArg("ForceImport");
		_ImmutabilityIsForced = _Settings->HasFlagArg("ForcedImmutability");

		_StartingNameSpace = _Settings->GetArgValueFlag("StartingNameSpac");
	}

	_PassType = PassType::GetTypes;
	Pass();

	if (!_ErrorsOutput->Has_Errors()) 
	{
		

		Assembly_LoadLibSymbols();
		_PassType = PassType::FixedTypes;
		Pass();


		if (!_ErrorsOutput->Has_Errors()) {
			Lib_BuildLibs();
			if (!_ErrorsOutput->Has_Errors()) {
				BuildCode();
			}
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
		if (!Item->OutputIR){continue;}
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
			auto StaticVarIR = _IR_Builder.NewStaticVarable(_IR_Builder.ToID(Item->FullName),IR_ConvertToIRType(Item->VarType));
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
	}
	{
		auto oldconext = SaveAndMove_SymbolContext();
		for (auto& Item : _Generic_GeneratedGenericSybol)
		{
			auto Symbol = Symbol_GetSymbol(Item.ID);

			_Generic_GenericSymbolStack.push(std::move(Item.Info));
			switch (Symbol->Type)
			{
			case SymbolType::Func:
			{
				auto Info = Symbol->Get_Info<FuncInfo>();
				auto node = FuncNode::As(Symbol->Get_NodeInfo<Node>());
				
				
				Set_SymbolConext(std::move(Info->Conext.value()));
				OnFuncNode(*node);
				Info->Conext = SaveAndMove_SymbolContext();
			}
			break;
			case SymbolType::Type_class:
			{
				auto Info = Symbol->Get_Info<ClassInfo>();
				auto node = ClassNode::As(Symbol->Get_NodeInfo<Node>());


				Set_SymbolConext(std::move(Info->Conext.value()));
				OnClassNode(*node);
				Info->Conext = SaveAndMove_SymbolContext();
			}
			break;
			case SymbolType::Enum:
			{
				auto Info = Symbol->Get_Info<EnumInfo>();
				auto node = EnumNode::As(Symbol->Get_NodeInfo<Node>());


				Set_SymbolConext(std::move(Info->Conext.value()));
				OnEnum(*node);
				Info->Conext = SaveAndMove_SymbolContext();

				
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
void SystematicAnalysis::Lib_BuildLibs()
{
	if (_Settings->_Type != OutPutType::IRAndSymbols) 
	{
		for (size_t i = 0; i < _Libs->size(); i++)
		{
			auto Item = (*_Libs)[i];

			Lib_BuildLib(*Item,(*_LibsNames)[i]);
		}
	}
}
void SystematicAnalysis::Lib_BuildLib(const UClib& lib,const Path& LibName)
{
	auto IRLayer = lib.GetLayer(UCode_CodeLayer_IR_Name);

	bool GotIRCode = false;
	if (IRLayer)
	{
		IRBuilder IRToImport;
		auto& LayerInfo = IRLayer->_Data.Get<CodeLayer::JustData>();

		if (IRBuilder::FromBytes(IRToImport,BytesView((Byte*)LayerInfo._Data.data(), LayerInfo._Data.size())))
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
		auto& Item =_IR_Builder._StaticInit.Blocks.front()->Instructions[Index];
		
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


	auto IRLayer = Tep.AddLayer(UCode_CodeLayer_IR_Name);
	CodeLayer::JustData V;
	V._Data = TepIR.ToBytes().MoveToVector();
	IRLayer->_Data =std::move(V);

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
		auto irfuncname = _IR_LookingAtIRFunc ? _IR_Builder.FromID(_IR_LookingAtIRFunc->identifier) : "";
		irvarname = irfuncname + ScopeHelper::_ScopeSep + ScopeHelper::GetNameFromFullName(Syb.FullName);
	}
	auto ID = _IR_Builder.ToID(irvarname);
	IRDebugSetVarableName V;

	V.IRVarableName = irvarname;
	V.VarableName = Syb.FullName;
	V.InsInBlock = InsInBlock;
	
	_IR_LookingAtIRBlock->DebugInfo.Add_SetVarableName(std::move(V));

	IRDebugSybol Info;
	Info.IRVarableName = irvarname;
	Info.VarableName = Syb.FullName;
	
	ReflectionTypeInfo Type =Assembly_ConvertToType(Syb.VarType);

	Info.LangType = UCode_LangType_UCodeLang;

	BitMaker bitm; 
	UClib::ToBytes(bitm, Type);
	Info.TypeInfo = std::move(bitm.Get_Bytes());

	switch (Syb.Type)
	{	
	case SymbolType::ParameterVarable:
		Info._Type = IRDebugSybol::Type::Par;
		break;
	case SymbolType::StackVarable:
		Info._Type = IRDebugSybol::Type::Stack;
		break;
	case SymbolType::StaticVarable:
		Info._Type = IRDebugSybol::Type::Static;
		break;
	case SymbolType::ThreadVarable:
		Info._Type = IRDebugSybol::Type::Thread;
		break;
	default:
		UCodeLangUnreachable();
		break;
	}
	_IR_Builder._Debug.Symbols.AddValue(ID,std::move(Info));
}
const NeverNullPtr<FileNode> SystematicAnalysis::FileDependency_Get_FileUseingSybol(const NeverNullPtr<Symbol> Syb)
{
	return NeverNullptr(Syb->_File);
}
void SystematicAnalysis::FileDependency_AddDependencyToCurrentFile(const NeverNullPtr<Symbol> Syb)
{
	FileDependency_AddDependencyToCurrentFile(FileDependency_Get_FileUseingSybol(Syb));
	
	
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

		for (auto& Item : FileData._Imports)
		{

			if (Item.ImportSymbolFullName == *StringToLookAt)
			{
				*Item.IsImportUsed = true;
				IsAnImport = true;
				break;
			}

		}

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
		FileNodeData& Data = *_FilesData[CurrentFile_t];
		
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
Symbol& SystematicAnalysis::Symbol_AddSymbol(SymbolType type, const String& Name, const String& FullName,AccessModifierType Access)
{

	auto& r = _Table.AddSybol(type, Name, FullName,Access);
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
		if (!Symbol_GetSymbol(String_view(Namespace),SymbolType::Namespace))
		{
			Symbol_AddSymbol(SymbolType::Namespace, Namespace, _Table._Scope.ThisScope,AccessModifierType::Public);
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
	_Table.RemoveScope();

	_Table.RemovePopUseing(UseingIndex);
}
void SystematicAnalysis::OnAttributeNode(const AttributeNode& node)
{


	if (_PassType == PassType::GetTypes)
	{

	}
	else if (_PassType == PassType::FixedTypes)
	{	
		auto SybID = Symbol_GetSymbolID(node);
		auto& Syb = Symbol_AddSymbol(SymbolType::UsedTag,"_","_",AccessModifierType::Private);
		_Table.AddSymbolID(Syb, SybID);
		

		String V;
		node._ScopedName.GetScopedName(V);
		auto AttOp = Symbol_GetSymbol(V,SymbolType::Tag_class);
		if (AttOp)
		{
			auto Att = AttOp.value();
			auto token =NeverNullptr(node._ScopedName._ScopedName.back()._token);
			if (Att->Type == SymbolType::Tag_class)
			{
				auto& Generic = *node._ScopedName._ScopedName.back()._generic;
				if (Generic._Values.size())
				{
					LogError_ExpectedSymbolToBea(token, *Att, SymbolType::Generic_Tag);
				}
				
			}
			else if (Att->Type == SymbolType::Generic_Tag)
			{
				auto& Generic = *node._ScopedName._ScopedName.back()._generic;
				if (Generic._Values.size())
				{
					auto Info = Att->Get_Info<TagInfo>();
					auto TagNode =TagTypeNode::As(Att->Get_NodeInfo<Node>());

					Att = Generic_InstantiateOrFindGeneric_Tag(token, Att,TagNode->_generic,Info->_GenericData,Generic).value();
				}
				else
				{
					LogError_ExpectedSymbolToBea(token, *Att, SymbolType::Tag_class);
				}
			}
			else
			{
				LogError_ExpectedSymbolToBea(token, *Att, SymbolType::Tag_class);
			}
			Syb.VarType = TypeSymbol(Att->ID);
		}
		else
		{
			auto token = NeverNullptr(node._ScopedName._ScopedName.back()._token);
			LogError_CantFindTypeError(token, V);
		}
	}
	else if (_PassType == PassType::BuidCode)
	{

	}
}
void SystematicAnalysis::OnAttributesNode(const Vector<Unique_ptr<AttributeNode>>& nodes)
{
	for (auto& Item : nodes)
	{
		OnAttributeNode(*Item);
	}
}
void SystematicAnalysis::OnFileNode(const FileNode& File)
{
	_LookingAtFile = &File;
	_ErrorsOutput->FilePath = File.FileName;
	
	size_t ClassesStart = PassType::BuidCode == _PassType ? _Lib._Assembly.Classes.size() : 0;
	size_t GlobalMethodStart = PassType::BuidCode == _PassType ? _Lib._Assembly.Get_GlobalObject_Class()->Methods.size() : 0;
	size_t StaticInitStart = PassType::BuidCode == _PassType && _IR_Builder._StaticInit.Blocks.size() ? _IR_Builder._StaticInit.Blocks.front()->Instructions.size(): 0;
	size_t ThreadInitStart = PassType::BuidCode == _PassType && _IR_Builder._threadInit.Blocks.size() ? _IR_Builder._threadInit.Blocks.front()->Instructions.size() : 0;
	size_t StaticDeInitStart = PassType::BuidCode == _PassType && _IR_Builder._StaticdeInit.Blocks.size() ? _IR_Builder._StaticdeInit.Blocks.front()->Instructions.size() : 0;
	size_t ThreadDeInitStart = PassType::BuidCode == _PassType && _IR_Builder._threaddeInit.Blocks.size() ? _IR_Builder._threaddeInit.Blocks.front()->Instructions.size() : 0;
	size_t IRFunsStart = PassType::BuidCode == _PassType ? _IR_Builder.Funcs.size() : 0;
	size_t IRSybolsStart = PassType::BuidCode == _PassType ? _IR_Builder._Symbols.size() : 0;
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
	}

	for (auto& node : File._Nodes)
	{
		Push_ToNodeScope(*node.get());
		Defer _{ [this](){Pop_NodeScope(); } };


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
		default:break;
		}

	}

	if (_StartingNameSpace.has_value())
	{
		_Table.RemoveScope();
	}

	OutofLoop:

	if (_PassType == PassType::BuidCode)
	{
		auto& FileData = GetFileData(NeverNullptr<FileNode_t>(&File));


		FileData.AssemblyInfoSpan = FileNodeData::SpanData::NewWithNewIndex(ClassesStart, _Lib._Assembly.Classes.size());
		FileData.GlobalObjectMethodInfoSpan = FileNodeData::SpanData::NewWithNewIndex(GlobalMethodStart, _Lib._Assembly.Get_GlobalObject_Class()->Methods.size());
		FileData.IRInitStaticSpan = FileNodeData::SpanData::NewWithNewIndex(StaticInitStart, _IR_Builder._StaticInit.Blocks.size() ? _IR_Builder._StaticInit.Blocks.front()->Instructions.size() : 0);
		FileData.IRInitThreadSpan = FileNodeData::SpanData::NewWithNewIndex(ThreadInitStart, _IR_Builder._threadInit.Blocks.size() ? _IR_Builder._threadInit.Blocks.front()->Instructions.size() : 0);
		FileData.IRDeInitStaticSpan= FileNodeData::SpanData::NewWithNewIndex(StaticDeInitStart, _IR_Builder._StaticdeInit.Blocks.size() ? _IR_Builder._StaticdeInit.Blocks.front()->Instructions.size() : 0);
		FileData.IRDeInitThreadSpan = FileNodeData::SpanData::NewWithNewIndex(ThreadDeInitStart, _IR_Builder._threaddeInit.Blocks.size() ? _IR_Builder._threaddeInit.Blocks.front()->Instructions.size() : 0);
		FileData.IRFuncsSpan = FileNodeData::SpanData::NewWithNewIndex(IRFunsStart, _IR_Builder.Funcs.size());
		FileData.IRSymbolSpan = FileNodeData::SpanData::NewWithNewIndex(IRSybolsStart, _IR_Builder._Symbols.size());
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
				Import._TepSymbolFullNames[i] = std::move(Item->FullName);//removeing name to remove exposure from other files.
			}
		}
	}

	_Table.ClearUseings();
}
void SystematicAnalysis::OnClassNode(const ClassNode& Node)
{

	bool IsgenericInstantiation = _Generic_GenericSymbolStack.size() && _Generic_GenericSymbolStack.top().NodeTarget == &Node;
	bool Isgeneric = Node._generic._Values.size();
	bool Isgeneric_t = Isgeneric && IsgenericInstantiation == false;


	const String ClassName = IsgenericInstantiation ? (String)ScopeHelper::GetNameFromFullName(_Generic_GenericSymbolStack.top()._IR_GenericFuncName) : (String)Node._className.token->Value._String;
	_Table.AddScope(ClassName);

	auto SybID = Symbol_GetSymbolID(Node);


	if (_PassType == PassType::GetTypes)
	{
		Symbol_RedefinitionCheck((String_view)_Table._Scope.ThisScope, SymbolType::Type_class,NeverNullptr(Node._className.token));
	}

	auto& Syb = _PassType == PassType::GetTypes ?
		Symbol_AddSymbol(Isgeneric_t ? SymbolType::Generic_class : SymbolType::Type_class
			, (String)ClassName, _Table._Scope.ThisScope, Node._Access) :
		*Symbol_GetSymbol(SybID);

	//we may jump to this node non linearly
	if (Syb.PassState == _PassType)
	{
		_Table.RemoveScope();
		return;
	}

	OnAttributesNode(Node._Attributes);

	ClassInfo* ClassInf = nullptr;
	if (_PassType == PassType::GetTypes)
	{
		_Table.AddSymbolID(Syb, SybID);
		Syb.NodePtr = Node.As();



		ClassInf = new ClassInfo();
		
		ClassInf->Conext = Save_SymbolContextRemoveOneScopeName();
		ClassInf->FullName = Syb.FullName;
		Syb.Info.reset(ClassInf);
		Syb.VarType.SetType(Syb.ID);

		if (Isgeneric_t)
		{
			_Table.AddScope(GenericTestStr);


			auto& GenericList = Node._generic;
			Generic_InitGenericalias(GenericList, IsgenericInstantiation, ClassInf->_GenericData);

			_Table.RemoveScope();
		}

		ClassInf->_GenericData = {};
		auto& GenericList = Node._generic;
		Generic_InitGenericalias(GenericList, IsgenericInstantiation, ClassInf->_GenericData);
	}
	else
	{
		ClassInf = Syb.Get_Info<ClassInfo>();
	}


	bool CheckgenericForErr = (Isgeneric_t && (_PassType == PassType::GetTypes || _PassType == PassType::FixedTypes));
	if (!Isgeneric_t || CheckgenericForErr)
	{
		
		ClassStackInfo classStackInfo;
		classStackInfo.Syb = &Syb;
		classStackInfo.Info = ClassInf;
		_ClassStack.push(classStackInfo);
		Push_ClassDependencie(ClassInf);
		if (CheckgenericForErr)
		{
			_Table.AddScope(GenericTestStr);
			Syb.FullName = _Table._Scope.ThisScope;
		}
		
		auto UseingIndex = _Table.GetUseingIndex();

		if (_PassType == PassType::BuidCode) {
			Assembly_AddClass(Node._Attributes, &Syb);
		}

		for (const auto& node : Node._Nodes)
		{
			Push_ToNodeScope(*node.get());
			switch (node->Get_Type())
			{
			case NodeType::ClassNode: OnClassNode(*ClassNode::As(node.get())); break;
			case NodeType::AliasNode:OnAliasNode(*AliasNode::As(node.get())); break;
			case NodeType::EnumNode:OnEnum(*EnumNode::As(node.get())); break;
			case NodeType::UsingNode: OnUseingNode(*UsingNode::As(node.get())); break;
			case NodeType::FuncNode:OnFuncNode(*FuncNode::As(node.get())); break;
			case NodeType::DeclareVariableNode:OnDeclareVariablenode(*DeclareVariableNode::As(node.get()), DeclareStaticVariableNode_t::ClassField); break;
			case NodeType::DeclareStaticVariableNode:OnDeclareStaticVariableNode(*DeclareStaticVariableNode::As(node.get())); break;
			case NodeType::DeclareThreadVariableNode:OnDeclareThreadVariableNode(*DeclareThreadVariableNode::As(node.get())); break;
			case NodeType::DeclareEvalVariableNode:OnDeclareEvalVariableNode(*DeclareEvalVariableNode::As(node.get())); break;
			case NodeType::TraitNode:OnTrait(*TraitNode::As(node.get())); break;
			case NodeType::TagTypeNode:OnTag(*TagTypeNode::As(node.get())); break;
			default:break;
			}
			Pop_NodeScope();
		}



		if (_PassType == PassType::FixedTypes)
		{
			ClassInf->SizeInitialized = true;
		}

		Pop_ClassDependencie();
		_Table.RemovePopUseing(UseingIndex);


		_ClassStack.pop();

		if (CheckgenericForErr)
		{
			_Table.RemoveScope();
			Syb.FullName = _Table._Scope.ThisScope;
		}
	}
	else
	{
		Syb.NodePtr = Node.As();

	}




	if (_PassType == PassType::FixedTypes)//auto make funcs
	{
		auto ThisCallType = TypeSymbol(Syb.ID);

		if ((Node._Inherited._values.size() || ClassInf->_WillHaveFielddeInit) && Symbol_GetSymbol(_Table._Scope.GetApendedString((String)ClassDestructorFunc), SymbolType::Func) == nullptr)
		{
			Symbol_MakeNewDropFuncSymbol(ClassInf, ThisCallType);
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
						if (OtherPar.Type.IsAddress() && OtherPar.IsOutPar == false && OtherPar.Type._CustomTypeSymbol == SybID)
						{

							if (OtherPar.Type._MoveData == MoveData::None)
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
				auto DropFunc = &Symbol_AddSymbol(SymbolType::Func, ClassConstructorfunc, _Table._Scope.GetApendedString((String)ClassConstructorfunc), AccessModifierType::Public);
				FuncInfo* V = new FuncInfo();
				DropFunc->Info.reset(V);

				V->FullName = DropFunc->FullName;
				V->_FuncType = FuncInfo::FuncType::New;
				V->Ret = TypesEnum::Void;
				V->FrontParIsUnNamed = true;

				auto ThisParType = ThisCallType;
				ThisParType._IsAddress = true;

				ParInfo parinfo;
				parinfo.IsOutPar = false;
				parinfo.Type = ThisParType;
				V->Pars.push_back(parinfo);
				ClassInf->_ClassAutoGenerateDefaultConstructor = true;

				DropFunc->PassState = PassType::FixedTypes;
			}
		}


		//Inherited Values
		for (auto& Item : Node._Inherited._values)
		{
			auto& Str = Item._Name.token->Value._String;

			auto SybOp = Symbol_GetSymbol(Str, SymbolType::Type);

			if (!SybOp.has_value())
			{
				LogError_CantFindTypeError(NeverNullptr(Item._Name.token), Str);
				continue;
			}
			auto Syb = SybOp.value();
			if (Item._generic._Values.size() == 0)
			{
				if (Syb->Type != SymbolType::Trait_class)
				{
					LogError_ExpectedSymbolToBea(NeverNullptr(Item._Name.token), *Syb, SymbolType::Trait_class);
					continue;
				}
			}
			else
			{
				if (Syb->Type != SymbolType::Generic_Trait)
				{
					LogError_ExpectedSymbolToBea(NeverNullptr(Item._Name.token), *Syb, SymbolType::Generic_Trait);
					continue;
				}

				auto CInfo = Syb->Get_Info<TraitInfo>();
				auto classnode = TraitNode::As(Syb->Get_NodeInfo<UCodeLang::Node>());
				Syb = Generic_InstantiateOrFindGeneric_Trait(NeverNullptr(Item._Name.token), Syb, classnode->_generic, CInfo->_GenericData, Item._generic).value();
			}

			/*
			for (auto& Item2 : ClassInf->_InheritedTypes)
			{
				if (Syb == Item2)
				{
					LogError(ErrorCodes::InValidType, Item.Token->OnLine, Item.Token->OnPos,
						"duplicate Inherit Trait");
				}
			}
			*/


			ClassInfo_InheritTypeInfo _Data;
			_Data.Syb = Syb.value();
			ClassInf->_InheritedTypes.push_back(_Data);


		}

		for (auto& Item : ClassInf->_InheritedTypes)
		{
			Symbol_InheritTrait(&Syb, ClassInf, Item.Syb,NeverNullptr(Node._className.token));
		}
	}
	if (_PassType == PassType::BuidCode)
	{
		
		bool IsOkMakeIR = (Isgeneric_t && IsgenericInstantiation == true) || Isgeneric_t == false;
		if (IsOkMakeIR)
		{

			if (ClassInf->_ClassAutoGenerateDrop)
			{
				auto OldFunc = _IR_LookingAtIRFunc;
				auto OldBlock = _IR_LookingAtIRBlock;
				//

				auto FuncSyb = Symbol_GetSymbol(_Table._Scope.GetApendedString((String)ClassDestructorFunc), SymbolType::Func).value();
				auto Funcinfo = FuncSyb->Get_Info<FuncInfo>();

				_IR_LookingAtIRFunc = _IR_Builder.NewFunc(IR_GetIRID(Funcinfo), IR_ConvertToIRType(Funcinfo->Ret));

				IRType ThisPar = IR_ConvertToIRType(Funcinfo->Pars.front());
				{
					IRPar par = IRPar();
					par.identifier = _IR_Builder.ToID(ThisSymbolName);
					par.type = ThisPar;
					_IR_LookingAtIRFunc->Pars.push_back(par);
				}
				_IR_LookingAtIRBlock = _IR_LookingAtIRFunc->NewBlock(".");


				if (ClassInf->_WillHaveFielddeInit)
				{
					auto ClassInit = _Table._Scope.GetApendedString((String)ClassDeInitializefuncName);

					_IR_LookingAtIRBlock->NewPushParameter(_IR_LookingAtIRBlock->NewLoad(&_IR_LookingAtIRFunc->Pars.front()));
					_IR_LookingAtIRBlock->NewCall(_IR_Builder.ToID(ClassInit));


				}_IR_LookingAtIRBlock->NewRet();

				//
				_IR_LookingAtIRFunc = OldFunc;
				_IR_LookingAtIRBlock = OldBlock;
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
					V.Pars.push_back({ false,ThisParType });
				}


				auto OldFunc = _IR_LookingAtIRFunc;
				auto OldBlock = _IR_LookingAtIRBlock;
				//
				_IR_LookingAtIRFunc = _IR_Builder.NewFunc(IR_GetIRID(&V), IR_ConvertToIRType(V.Ret));
				IRType ThisPar = IR_ConvertToIRType(V.Pars.front());
				{
					IRPar par = IRPar();
					par.identifier = _IR_Builder.ToID(ThisSymbolName);
					par.type = ThisPar;
					_IR_LookingAtIRFunc->Pars.push_back(par);
				}

				_IR_LookingAtIRBlock = _IR_LookingAtIRFunc->NewBlock(".");
				{
					IR_Build_FuncDropUsingFields(ClassInf, ThisPar);
					_IR_LookingAtIRBlock->NewRet();
				}

				//
				_IR_LookingAtIRFunc = OldFunc;
				_IR_LookingAtIRBlock = OldBlock;
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
					V.Pars.push_back({ false,ThisParType });
				}


				auto OldFunc = _IR_LookingAtIRFunc;
				auto OldBlock = _IR_LookingAtIRBlock;
				//
				_IR_LookingAtIRFunc = _IR_Builder.NewFunc(IR_GetIRID(&V), IR_ConvertToIRType(V.Ret));
				IRType ThisPar = IR_ConvertToIRType(V.Pars.front());
				{
					IRPar par = IRPar();
					par.identifier = _IR_Builder.ToID(ThisSymbolName);
					par.type = ThisPar;
					_IR_LookingAtIRFunc->Pars.push_back(par);
				}

				_IR_LookingAtIRBlock = _IR_LookingAtIRFunc->NewBlock(".");
				{
					if (ClassInf->_ClassFieldInit)
					{
						auto ClassInit = _Table._Scope.GetApendedString((String)ClassInitializefuncName);

						_IR_LookingAtIRBlock->NewPushParameter(_IR_LookingAtIRBlock->NewLoad(&_IR_LookingAtIRFunc->Pars.front()));
						_IR_LookingAtIRBlock->NewCall(_IR_Builder.ToID(ClassInit));


					}_IR_LookingAtIRBlock->NewRet();
				}

				//
				_IR_LookingAtIRFunc = OldFunc;
				_IR_LookingAtIRBlock = OldBlock;
			}

			if (ClassInf->_ClassFieldInit)
			{
				ClassInf->_ClassFieldInit->Blocks[0]->NewRet();
			}

			for (auto& Item : ClassInf->_InheritedTypes)
			{
				Symbol_BuildTrait(&Syb, ClassInf, Item.Syb,NeverNullptr(Node._className.token));
			}
		}
	}


	_Table.RemoveScope();






	Syb.PassState = _PassType;
}

void SystematicAnalysis::OnAliasNode(const AliasNode& node)
{
	const bool IsgenericInstantiation = _Generic_GenericSymbolStack.size() && _Generic_GenericSymbolStack.top().NodeTarget == &node;
	const bool Isgeneric = node.Generic._Values.size();
	const bool Isgeneric_t = Isgeneric && IsgenericInstantiation == false;


	const String ClassName = IsgenericInstantiation ? (String)ScopeHelper::GetNameFromFullName(_Generic_GenericSymbolStack.top()._IR_GenericFuncName) : (String)node._AliasName.token->Value._String;
	

	_Table.AddScope(ClassName);
	auto SybID = Symbol_GetSymbolID(node);//Must be pass AddScope thats how GetSymbolID works.

	if (_PassType == PassType::GetTypes)
	{
		Symbol_RedefinitionCheck((String_view)_Table._Scope.ThisScope, node._IsHardAlias ? SymbolType::Hard_Type_alias : SymbolType::Type_alias,NeverNullptr(node._AliasName.token));
	}

	SymbolType _Type;
	if (Isgeneric_t) 
	{
		_Type = SymbolType::Generic_Alias;
	}
	else
	{
		_Type =node._IsHardAlias ? SymbolType::Hard_Type_alias : SymbolType::Type_alias;
	}
	auto& Syb = _PassType == PassType::GetTypes ?
		Symbol_AddSymbol(_Type, (String)ClassName, _Table._Scope.ThisScope,node._Access) :
		*Symbol_GetSymbol(SybID);

	//we may jump to this node non linearly
	if (Syb.PassState == _PassType)
	{
		_Table.RemoveScope();
		return;
	}

	if (_PassType == PassType::GetTypes)
	{
		_Table.AddSymbolID(Syb, SybID);


		Syb.NodePtr = node.As();

		if (!Isgeneric_t)
		{
			if (IsgenericInstantiation)
			{
				auto Ptr = new Generic_AliasInfo();
				Syb.Info.reset(Ptr);
				Generic_InitGenericalias(node.Generic, true, Ptr->_GenericData);
				Ptr->Conext = Save_SymbolContextRemoveOneScopeName();
			}
			else
				if (node._AliasType == AliasType::Type)
				{
					auto V = new AliasInfo();
					Syb.Info.reset(V);

					Type_ConvertAndValidateType(node._Type, Syb.VarType, NodeSyb_t::Any);

					V->Conext = Save_SymbolContextRemoveOneScopeName();
				}
				else
				{

					AliasNode_Func* node_ = (AliasNode_Func*)node._Node.get();
					auto V = new FuncPtrInfo();
					Syb.Info.reset(V);

					V->Pars.resize(node_->_Parameters._Parameters.size());

					for (size_t i = 0; i < V->Pars.size(); i++)
					{
						auto& NodePar = node_->_Parameters._Parameters[i];
						auto& Par = V->Pars[i];
						Par.IsOutPar = NodePar._IsOutVarable;
						Type_ConvertAndValidateType(NodePar._Type, Par.Type, NodeSyb_t::Parameter);
					}

					Type_ConvertAndValidateType(node_->_ReturnType, V->Ret, NodeSyb_t::Ret);

					Syb.VarType.SetType(SybID);

					Syb.Type = SymbolType::Func_ptr;

					V->Conext = Save_SymbolContextRemoveOneScopeName();
				}
		}
		else
		{
			auto Ptr =new Generic_AliasInfo();
			Syb.Info.reset(Ptr);
			Generic_InitGenericalias(node.Generic,false, Ptr->_GenericData);
		}
	}
	else if (_PassType == PassType::FixedTypes)
	{
		if (!Isgeneric_t) 
		{
			if (node._AliasType == AliasType::Type)
			{
				Type_ConvertAndValidateType(node._Type, Syb.VarType, NodeSyb_t::Any);
			}
			else
			{
				AliasNode_Func* node_ = (AliasNode_Func*)node._Node.get();
				FuncPtrInfo* nodeinfo_ = (FuncPtrInfo*)Syb.Info.get();

				for (size_t i = 0; i < nodeinfo_->Pars.size(); i++)
				{
					auto& NodePar = node_->_Parameters._Parameters[i];
					auto& Par = nodeinfo_->Pars[i];
					Par.IsOutPar = NodePar._IsOutVarable;
					Type_ConvertAndValidateType(NodePar._Type, Par.Type, NodeSyb_t::Parameter);
				}

				Type_ConvertAndValidateType(node_->_ReturnType, nodeinfo_->Ret, NodeSyb_t::Ret);
			}
		}
	}
	else if (_PassType == PassType::BuidCode)
	{
		if (!Isgeneric_t) 
		{
			if (node._AliasType == AliasType::Type)
			{
				auto& V = _Lib.Get_Assembly().AddAlias((String)ClassName, _Table._Scope.ThisScope);
				V.Type = Assembly_ConvertToType(Syb.VarType);

				if (node._IsHardAlias)
				{
					V.HardAliasTypeID = Type_GetTypeID(TypesEnum::CustomType, Syb.ID);
				}
				FileDependency_AddDependencyToCurrentFile(Syb.VarType);
			}
			else
			{
				auto& V = _Lib.Get_Assembly().AddFuncPtr((String)ClassName, _Table._Scope.ThisScope);
				const FuncPtrInfo* nodeinfo_ = Syb.Get_Info<FuncPtrInfo>();
				V.ParsType.resize(nodeinfo_->Pars.size());
				for (size_t i = 0; i < nodeinfo_->Pars.size(); i++)
				{
					//TODO should use a funcion like Assembly_ConvertToParType.
					V.ParsType[i].IsOutPar = nodeinfo_->Pars[i].IsOutPar;
					V.ParsType[i].Type = Assembly_ConvertToType(nodeinfo_->Pars[i].Type);
				}
				V.RetType = Assembly_ConvertToType(nodeinfo_->Ret);
			}
		}
	}

	_Table.RemoveScope();

	Syb.PassState = _PassType;
}
void SystematicAnalysis::OnUseingNode(const UsingNode& node)
{
	auto& T =NeverNullptr(node._ScopedName._ScopedName.back()._token);
	const auto UseingString =Str_GetScopedNameAsString(node._ScopedName);
	_Table.AddUseing(UseingString);

	if (_PassType == PassType::FixedTypes)
	{
		if (!Symbol_GetSymbol(UseingString, SymbolType::Namespace))
		{
			LogError_CantFindNamespace(T, UseingString);
		}
	}
}
void SystematicAnalysis::Generic_InitGenericalias(const GenericValuesNode& GenericList, bool IsgenericInstantiation, Generic& Out)
{
	for (size_t i = 0; i < GenericList._Values.size(); i++)
	{
		auto& Item = GenericList._Values[i];

		auto GenericTypeName = Item.AsString();
		auto GenericType = &Symbol_AddSymbol(SymbolType::Type_alias, GenericTypeName,
			_Table._Scope.GetApendedString(GenericTypeName), AccessModifierType::Private);

		GenericType->NodePtr = &Item;


		if (IsgenericInstantiation)
		{
			GenericType->Type = SymbolType::Type_alias;

			GenericFuncInfo& V2 = _Generic_GenericSymbolStack.top();
			GenericType->VarType = (*V2.GenericInput)[i];
		}
		else
		{
			GenericType->Type = SymbolType::Unmaped_Generic_Type;
			GenericType->OutputIR = false;

			SymbolID ID = Symbol_GetSymbolID(GenericType->NodePtr);
			_Table.AddSymbolID(*GenericType, ID);

			GenericData Info;
			Info.SybID = ID;
			Info.type = Generic_TypeToGenericDataType(Item._Generictype);
			Out._Generic.push_back(Info);

			if (Info.type == GenericData::Type::Pack)
			{
				bool IsLast = i == GenericList._Values.size() - 1;
				if (!IsLast)
				{
					LogError_ParPackTypeIsNotLast(NeverNullptr(Item.token));
				}
			}
		}
	}
}
void SystematicAnalysis::OnFuncNode(const FuncNode& node)
{
	
	const bool IsgenericInstantiation = _Generic_GenericSymbolStack.size() && _Generic_GenericSymbolStack.top().NodeTarget == &node;
	const bool IsGenericS = node._Signature._generic._Values.size();
	const bool Isgeneric_t = IsGenericS && IsgenericInstantiation == false;
	const bool CheckgenericForErr = (Isgeneric_t && (_PassType == PassType::GetTypes || _PassType == PassType::FixedTypes));

	auto FuncName = IsgenericInstantiation ? _Generic_GenericSymbolStack.top()._IR_GenericFuncName
		: node._Signature._Name.AsStringView();
	auto NameToken =NeverNullptr(node._Signature._Name.token);


	FuncInfo::FuncType FuncType = FuncInfo::FuncType::NameFunc;

	Str_FuncGetName( NameToken, FuncName, FuncType);



	_Table.AddScope(FuncName);
	auto FullName = _Table._Scope.ThisScope;




	Symbol* syb;
	SymbolID sybId = Symbol_GetSymbolID(node);//Must be pass AddScope thats how GetSymbolID works.


	auto UseingIndex = _Table.GetUseingIndex();
	OnAttributesNode(node._Attributes);
	if (_PassType == PassType::GetTypes)
	{
		SymbolType Type = IsGenericS && IsgenericInstantiation == false ?
			SymbolType::GenericFunc : SymbolType::Func;



		syb = &Symbol_AddSymbol(Type, (String)FuncName, FullName,node._Signature._Access);
		syb->NodePtr = node.As();
		_Table.AddSymbolID(*syb, sybId);

		FuncInfo* newInfo = new FuncInfo();
		newInfo->Conext = Save_SymbolContextRemoveOneScopeName();
		newInfo->FullName = FullName;
		newInfo->_FuncType = FuncType;
		newInfo->IsUnsafe = node._Signature._HasUnsafeKeyWord;

		syb->Info.reset(newInfo);

		auto& GenericList = node._Signature._generic;


		for (size_t i = 0; i < GenericList._Values.size(); i++)
		{
			auto& Item = GenericList._Values[i];

			auto GenericTypeName = Item.AsString();
			auto GenericType = &Symbol_AddSymbol(SymbolType::Type_alias, GenericTypeName,
				_Table._Scope.GetApendedString(GenericTypeName)
			,node._Signature._Access);

			if (IsgenericInstantiation)
			{
				GenericType->Type = SymbolType::Type_alias;

				GenericFuncInfo& V2 = _Generic_GenericSymbolStack.top();
			
				auto TepVarable = Generic_TypeToGenericDataType(Item._Generictype);
				

				if (TepVarable == GenericData::Type::Pack) 
				{
					GenericType->VarType = TypeSymbol(V2.Pack.value());
				}
				else
				{
					GenericType->VarType = (*V2.GenericInput)[i];
				}
			
			}
			else
			{
				GenericType->Type = SymbolType::Unmaped_Generic_Type;

				
				SymbolID ID = Symbol_GetSymbolID(&Item);
				_Table.AddSymbolID(*GenericType, ID);

				GenericData Info;
				Info.SybID = ID;
				Info.type = Generic_TypeToGenericDataType(Item._Generictype);

				newInfo->_GenericData._Generic.push_back(Info);

				if (Info.type == GenericData::Type::Pack)
				{
					bool IsLast = i == GenericList._Values.size() - 1;
					if (!IsLast)
					{
						LogError_ParPackTypeIsNotLast(NeverNullptr(Item.token));
					}
				}
			}

		}


		{
			auto& RetType = node._Signature._ReturnType._node;
			if (RetType && RetType->Get_Type() == NodeType::AnonymousTypeNode)
			{
				auto NewName = Str_GetFuncAnonymousObjectFullName(FullName);


				SymbolID AnonymousSybID = Symbol_GetSymbolID(RetType.get());
				auto& AnonymousSyb = Symbol_AddSymbol(SymbolType::Type_class, (String)NewName, NewName,node._Signature._Access);

				_Table.AddSymbolID(AnonymousSyb, AnonymousSybID);


				auto ClassInf = new ClassInfo();
				ClassInf->FullName = NewName;
				AnonymousSyb.Info.reset(ClassInf);
				AnonymousSyb.VarType.SetType(AnonymousSyb.ID);

				AnonymousTypeNode* Typenode = AnonymousTypeNode::As(RetType.get());
				for (auto& Item3 : Typenode->_Fields._Parameters)
				{
					ClassInf->AddField(Item3._Name.AsString(), Type_ConvertAndValidateType(Item3._Type,NodeSyb_t::Parameter));
				}
			}
		}



		auto ClassSymBool = _ClassStack.size() ? _ClassStack.top().Syb : nullptr;
		for (auto& Item : node._Signature._Parameters._Parameters)
		{

			String_view GenericTypeName;
			if (Item._Name.token == nullptr)
			{
				GenericTypeName = ThisSymbolName;
			}
			else
			{
				if (Item._Name.token != nullptr)
				{
					GenericTypeName = Item._Name.AsStringView();
				}
			}




			auto GenericType = &Symbol_AddSymbol(SymbolType::ParameterVarable, (String)GenericTypeName,
				_Table._Scope.GetApendedString(GenericTypeName)
			,node._Signature._Access);
			auto ParSybID = Symbol_GetSymbolID(Item);

			auto ParInfo = new ParameterInfo();
			ParInfo->IsOutValue = Item._IsOutVarable;
			ParInfo->MyFunc = newInfo;

			GenericType->Info.reset(ParInfo);
			_Table.AddSymbolID(*GenericType, ParSybID);

			if (Item._Name.token == nullptr)
			{
				newInfo->FrontParIsUnNamed = true;
			}
			newInfo->Pars.push_back({ Item._IsOutVarable,Type_ConvertAndValidateType(Item._Type,NodeSyb_t::Parameter) });
		}

	}
	else
	{
		syb = Symbol_GetSymbol(sybId).value();
	}

	//we may jump to this node non linearly
	if (syb->PassState == _PassType)
	{
		_Table.RemoveScope();
		return;
	}
	FuncInfo* Info = syb->Get_Info<FuncInfo>();


	_FuncStack.push_back(Info);


	if (_PassType == PassType::FixedTypes
		|| (IsGenericS && _PassType == PassType::GetTypes))
	{

		auto& RetType = node._Signature._ReturnType._node;
		if (RetType && RetType->Get_Type() == NodeType::AnonymousTypeNode)
		{
			SymbolID AnonymousSybID = Symbol_GetSymbolID(RetType.get());
			auto& V = *Symbol_GetSymbol(AnonymousSybID);

			auto ClassInf = (ClassInfo*)V.Get_Info<ClassInfo>();

			AnonymousTypeNode* Typenode = AnonymousTypeNode::As(RetType.get());

			for (size_t i = 0; i < Typenode->_Fields._Parameters.size(); i++)
			{
				auto& Item3 = Typenode->_Fields._Parameters[i];
				auto ItemOut = ClassInf->Fields[i];
				Type_ConvertAndValidateType(Item3._Type, ItemOut.Type,NodeSyb_t::Parameter);
			}


			Push_ClassDependencie(ClassInf);

			UAddress ClassSize = 0;
			for (auto& Item : ClassInf->Fields)
			{
				UAddress V = NullAddress;
				Type_GetSize(Item.Type, V);
				ClassSize += V;
			}

			Pop_ClassDependencie();

			ClassInf->Size = ClassSize;
			ClassInf->SizeInitialized = true;


			syb->VarType.SetType(AnonymousSybID);
			Info->Ret = syb->VarType;
		}
		else
		{
			Type_ConvertAndValidateType(node._Signature._ReturnType, syb->VarType,NodeSyb_t::Ret);
			Info->Ret = syb->VarType;
		}



		for (size_t i = 0; i < node._Signature._Parameters._Parameters.size(); i++)
		{
			auto& Item = node._Signature._Parameters._Parameters[i];
			auto& Item2 = Info->Pars[i];


			auto ParSybID = Symbol_GetSymbolID(Item);
			auto& Sybol = *Symbol_GetSymbol(ParSybID);
			Type_ConvertAndValidateType(Item._Type, Sybol.VarType,NodeSyb_t::Parameter);
			Item2.Type = Sybol.VarType;



			{//Par Pack Err
				if (Item2.Type._CustomTypeSymbol.HasValue() && (&Item2 != &Info->Pars.back()))
				{
					for (auto& GenericItem : Info->_GenericData._Generic)
					{
						if (GenericItem.SybID == Item2.Type._CustomTypeSymbol)
						{
							if (GenericItem.type == GenericData::Type::Pack)
							{
								auto Token = NeverNullptr(Item._Name.token);
								LogError_LogParPackIsNotLast(Token);
							}

							break;
						}
					}
				}
			}

			if (Item2.Type.IsAn(TypesEnum::Void))
			{
				auto Token = NeverNullptr(Item._Name.token);
				LogError_CantUseTypeVoidHere(Token);
			}
			if (Item2.Type.IsTypeInfo())
			{
				auto Token = NeverNullptr(Item._Name.token);
				LogError_UseingTypeinfoInEvalFuncPar(Token);
			}
		}

	}




	bool buidCode = _PassType == PassType::BuidCode;
	bool ignoreBody = !IsgenericInstantiation && IsGenericS;
	
	
	if (buidCode && !ignoreBody)
	{
		bool IsBuildingIR = true;
		auto DecName = IR_MangleName(Info);

		if (IsBuildingIR)
		{

			_IR_LookingAtIRFunc = _IR_Builder.NewFunc(IR_GetIRID(Info), {});
			_IR_LookingAtIRBlock = _IR_LookingAtIRFunc->NewBlock("");
			Push_NewStackFrame();



			auto& ParNodes = node._Signature._Parameters._Parameters;

			bool IsPackParLast = false;
			if (IsgenericInstantiation && ParNodes.size())
			{
				auto& TopGenericSymbolStack = _Generic_GenericSymbolStack.top();
				if (TopGenericSymbolStack.Pack.has_value()) 
				{
					if (Info->Pars.back().Type._CustomTypeSymbol == TopGenericSymbolStack.Pack.value())
					{
						IsPackParLast = true;
					}
				}
			}

			size_t ParNodeSize = ParNodes.size();
			if (IsPackParLast)
			{
				size_t ParsCount = ParNodes.size() - 1;

				const TypePackInfo* PackPar = Symbol_GetSymbol(_Generic_GenericSymbolStack.top().Pack.value())->Get_Info<TypePackInfo>();
				ParsCount += PackPar->List.size();
				_IR_LookingAtIRFunc->Pars.resize(ParsCount);

				ParNodeSize -= 1;
			}
			else
			{
				_IR_LookingAtIRFunc->Pars.resize(ParNodes.size());//becuase we are useing ptrs.
			}

			for (size_t i = 0; i < ParNodeSize; i++)
			{
				auto& Item = ParNodes[i];

				auto ParSybID = Symbol_GetSymbolID(Item);
				auto& V = *Symbol_GetSymbol(ParSybID);


				auto& VarType = Info->Pars[i];

				auto& d = _IR_LookingAtIRFunc->Pars[i];
				d.identifier = _IR_Builder.ToID(ScopeHelper::GetNameFromFullName(V.FullName));
				
				if (Item._IsOutVarable)
				{
					d.type = IRType(IRTypes::pointer);
				}
				else
				{
					d.type = IR_ConvertToIRType(VarType);
				}


				Debug_Add_SetVarableInfo(V, _IR_LookingAtIRBlock->Instructions.size() == 0 ? 0 : _IR_LookingAtIRBlock->GetIndex());

				if (Symbol_HasDestructor(VarType.Type))
				{
					ObjectToDrop V;
					V.DropType = ObjectToDropType::Operator;
					V.ID = ParSybID;
					V._Operator = IROperator(&d);
					V.Type = VarType.Type;

					_IR_StackFrames.back().OnEndStackFrame.push_back(V);
				}

				V.IR_Par = &d;
			}

			if (IsPackParLast)
			{
				const TypePackInfo* PackPar = Symbol_GetSymbol(_Generic_GenericSymbolStack.top().Pack.value())->Get_Info<TypePackInfo>();

				size_t V = ParNodeSize;


				auto PackParSybol = Symbol_GetSymbol(Symbol_GetSymbolID(ParNodes.back()));
				auto PackParSybolName = ScopeHelper::GetNameFromFullName(PackParSybol->FullName);

				for (size_t i = 0; i < PackPar->List.size(); i++)
				{
					auto& PackType = PackPar->List[i];
					auto& d = _IR_LookingAtIRFunc->Pars[V];

					auto ParName = PackParSybolName + std::to_string(i);



					auto& TepPar = Symbol_AddSymbol(SymbolType::ParameterVarable, ParName, PackParSybol->FullName + std::to_string(i), AccessModifierType::Public);
					auto ParSybID = Symbol_GetSymbolID(&TepPar);
					TepPar.IR_Par = &d;

					_Table.AddSymbolID(TepPar, ParSybID);


					d.identifier = _IR_Builder.ToID(ParName);
					d.type = IR_ConvertToIRType(PackType);

					Debug_Add_SetVarableInfo(TepPar, _IR_LookingAtIRBlock->Instructions.size() == 0 ? 0 : _IR_LookingAtIRBlock->GetIndex());

					if (Symbol_HasDestructor(PackType))
					{
						ObjectToDrop V;
						V.DropType = ObjectToDropType::Operator;
						V.ID = ParSybID;
						V._Operator = IROperator(&d);
						V.Type = PackType;

						_IR_StackFrames.back().OnEndStackFrame.push_back(V);
					}


					//

					V++;
				}
			}


			_IR_LookingAtIRFunc->ReturnType = IR_ConvertToIRType(Info->Ret);
		}


		if (node._Signature._HasExternKeyWord)
		{
			bool HasBody = node._Body.has_value();
			if (HasBody)
			{
				if (node._Signature._HasDynamicKeyWord)
				{
					_IR_LookingAtIRFunc->Linkage = IRFuncLink::DynamicExport;
				}
				else
				{
					_IR_LookingAtIRFunc->Linkage = IRFuncLink::StaticExport;
				}
			}
			else
			{
				if (node._Signature._HasDynamicKeyWord)
				{
					_IR_LookingAtIRFunc->Linkage = IRFuncLink::DynamicExternalLink;
				}
				else
				{
					_IR_LookingAtIRFunc->Linkage = IRFuncLink::StaticExternalLink;
				}
			}

		}


		Class_Data* Ptr = Assembly_GetAssemblyClass(FullName);

		ClassMethod V;
		V.FullName = FullName;
		V.DecorationName = DecName;
		V.RetType = Assembly_ConvertToType(Info->Ret);

		V.IsThisFuncion = Info->FrontParIsUnNamed;
		for (size_t i = 0; i < Info->Pars.size(); i++)
		{
			auto& Item = Info->Pars[i];

			auto& F = V.ParsType.emplace_back();
			F.IsOutPar = node._Signature._Parameters._Parameters[i]._IsOutVarable;
			F.Type = Assembly_ConvertToType(Item.Type);
		}

		Assembly_ConvertAttributes(node._Attributes,V.Attributes.Attributes);


		Ptr->Methods.push_back(std::move(V));

		auto& RetType = node._Signature._ReturnType._node;
		if (RetType && RetType->Get_Type() == NodeType::AnonymousTypeNode)
		{
			SymbolID AnonymousSybID = Symbol_GetSymbolID(RetType.get());
			auto V = Symbol_GetSymbol(AnonymousSybID);

			Assembly_AddClass({}, V);

		}
		//

		if (IsBuildingIR)
		{
			if (FuncType == FuncInfo::FuncType::New)
			{
				auto& Classinfo = _ClassStack.top().Info;
				if (Classinfo->_WillHaveFieldInit)
				{
					auto InitFunc = Classinfo->FullName;
					ScopeHelper::GetApendedString(InitFunc, ClassInitializefuncName);


					_IR_LookingAtIRBlock->NewPushParameter(_IR_LookingAtIRBlock->NewLoad(&_IR_LookingAtIRFunc->Pars.front()));
					_IR_LookingAtIRBlock->NewCall(_IR_Builder.ToID(InitFunc));
				}
			}
		}
		//
	}


	
	if (CheckgenericForErr)
	{
		ignoreBody = false;
		_Table.AddScope(GenericTestStr);
	}

	if (node._Body.has_value() && !ignoreBody)
	{
		auto& Body = node._Body.value();
		size_t ErrorCount = _ErrorsOutput->Get_Errors().size();

		OnStatementsWithSetableRet(Body._Statements, Info->Ret,NeverNullptr(node._Signature._Name.token));

		bool GotErr =ErrorCount < _ErrorsOutput->Get_Errors().size();
		if (GotErr)
		{
			syb->SetToInvalid();
		}
		syb->VarType = Info->Ret;
	}

	if (CheckgenericForErr)
	{
		_Table.RemoveScope();
	}

	if (buidCode && !ignoreBody)
	{
		Pop_StackFrame();


		if (FuncType == FuncInfo::FuncType::Drop)
		{

			auto& Classinfo = _ClassStack.top().Info;
			if (Classinfo->_WillHaveFielddeInit) 
			{
				auto InitFunc = Classinfo->FullName;
				ScopeHelper::GetApendedString(InitFunc, ClassInitializefuncName);

				_IR_LookingAtIRBlock->NewPushParameter(_IR_LookingAtIRBlock->NewLoad(&_IR_LookingAtIRFunc->Pars.front()));
				_IR_LookingAtIRBlock->NewCall(_IR_Builder.ToID(InitFunc));
				_IR_LookingAtIRBlock->NewRet();
			}
			
		}

		_IR_LookingAtIRBlock->NewRet();

		if (FuncName == "main" && _IR_LookingAtIRFunc->Pars.size() == 0)
		{
			_IR_Builder.EntryPoint = _IR_LookingAtIRFunc->identifier;
		}

	}



	if (_PassType == PassType::FixedTypes)
	{
		if (!node._Body.has_value() && syb->VarType._Type == TypesEnum::Var)
		{
			auto Token = NeverNullptr(node._Signature._Name.token);
			LogError_BeMoreSpecifiicForRetType(FuncName, Token);
		}
		else {
			FuncRetCheck(*node._Signature._Name.token, syb, Info);
			auto Token = NeverNullptr(node._Signature._Name.token);
			Symbol_RedefinitionCheck(syb, Info, Token);
		}
	}

	_FuncStack.pop_back();

	_Table.RemovePopUseing(UseingIndex);




	_Table.RemoveScope();


	syb->PassState = _PassType;

}

void SystematicAnalysis::Node_InStatetements(bool Value)
{
	if (_ClassStack.size())
	{
		_ClassStack.top()._InStatements = Value;
	}
}
void SystematicAnalysis::Str_FuncGetName(const NeverNullPtr<Token> NameToken, String_view& FuncName, FuncInfo::FuncType& FuncType)
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

		LogError_CantOverLoadOverload(NameToken);

		break;
	}

	DoStuff:
	if (ObjectOverLoad) 
	{
		if (_ClassStack.empty())
		{
			LogError_CantUseThisKeyWordHereError(NameToken);
		}
	}
}
Class_Data* SystematicAnalysis::Assembly_GetAssemblyClass(const String& FullName)
{
	if (_ClassStack.empty())
	{
		auto& Assembly = _Lib.Get_Assembly();

		auto globalAssemblyObjectName = (String_view)ScopeHelper::_globalAssemblyObject;

		auto Ptr =  Assembly.Find_Class(globalAssemblyObjectName);
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
void SystematicAnalysis::FuncRetCheck(const Token& Name, const NeverNullPtr<Symbol> FuncSyb, const FuncInfo* Func)
{
	switch (Func->_FuncType)
	{
	case FuncInfo::FuncType::Drop:
	{
		TypeSymbol V(TypesEnum::Void);
		if (!Type_AreTheSame(Func->Ret, V))
		{
			LogError_LogFuncMustBe(NeverNullptr(&Name), FuncSyb->FullName, V);
		}
		if (Func->Pars.size() != 1)
		{
			LogError_PostfixOverloadPars(Name, Func);
		}
		break;
	}
	case FuncInfo::FuncType::New:
	{

		TypeSymbol V(TypesEnum::Void);
		if (!Type_AreTheSame(Func->Ret, V)) 
		{
			LogError_LogFuncMustBe(NeverNullptr(&Name), FuncSyb->FullName, V);
		}
		
	}
	break;
	case FuncInfo::FuncType::Index:
	{
		if (Func->Pars.size() != 2)
		{
			LogError_IndexOverloadPars(Name, Func);
		}
		else if (Func->Pars.size() == 2 && (Func->Pars[0].IsOutPar|| Func->Pars[1].IsOutPar))
		{
			LogError_CantUseOutInOverloadFunc(Name);
		}
	}
	break;
	case FuncInfo::FuncType::Cast:
	{
		if (Func->Pars.size() != 1)
		{


			LogError_PostfixOverloadPars(Name, Func);

		}
		else if (Func->Pars.size() == 1 && Func->Pars[0].IsOutPar)
		{
			LogError_CantUseOutInOverloadFunc(Name);
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
				LogError_BinaryOverloadPars(Name, Func);
			}
			else if (Func->Pars.size() == 2 && (Func->Pars[0].IsOutPar
				|| Func->Pars[1].IsOutPar))
			{
				LogError_CantUseOutInOverloadFunc(Name);
			}
		}
		else if (Systematic_PostfixOverloadData::IsPostfixOverload(Func->_FuncType))
		{
			if (Func->Pars.size() != 1)
			{
				LogError_PostfixOverloadPars(Name, Func);
			}
			else if (Func->Pars.size() == 1 && Func->Pars[0].IsOutPar)
			{
				LogError_CantUseOutInOverloadFunc(Name);
			}
		}
		else if (Systematic_CompoundOverloadData::IsCompoundOverload(Func->_FuncType))
		{
			if (Func->Pars.size() != 2)
			{
				LogError_BinaryOverloadPars(Name, Func);

			}
			else if (Func->Pars.size() == 2 && 
				(Func->Pars[0].IsOutPar || Func->Pars[1].IsOutPar))
			{
				LogError_CantUseOutInOverloadFunc(Name);
			}

		}
		else if (Systematic_UrinaryOverloadData::IsUrinaryOverload(Func->_FuncType))
		{
			if (Func->Pars.size() != 1)
			{
				LogError_PostfixOverloadPars(Name, Func);
			}
			else if (Func->Pars.size() == 1 && Func->Pars[0].IsOutPar)
			{
				LogError_CantUseOutInOverloadFunc(Name);
			}
		}
		else if (Systematic_MemberOverloadData::IsMemerOverload(Func->_FuncType))
		{
			if (Func->Pars.size() != 1)
			{
				LogError_PostfixOverloadPars(Name, Func);
			}
			else if (Func->Pars.size() == 1 && Func->Pars[0].IsOutPar)
			{
				LogError_CantUseOutInOverloadFunc(Name);
			}
		}
		break;
	}
}
void SystematicAnalysis::LogError_CantUseOutInOverloadFunc(const Token& Name)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidType, Name.OnLine, Name.OnPos, "you may not use Out parameters for overload.");
}
void SystematicAnalysis::OnForNode(const ForNode& node)
{
	auto& StrVarName = node._Name->Value._String;
	auto FullName = _Table._Scope.GetApendedString(StrVarName);

	
	Symbol* syb;

	String ScopeName = std::to_string((size_t)&node);

	Push_NewStackFrame();
	_Table.AddScope(ScopeName);
	SymbolID sybId = Symbol_GetSymbolID(node);

	if (_PassType == PassType::GetTypes)
	{
		Symbol_RedefinitionCheck(FullName, SymbolType::StackVarable,NeverNullptr(node._Name));


		syb = &Symbol_AddSymbol(SymbolType::StackVarable, (String)StrVarName, FullName,AccessModifierType::Public);
		_Table.AddSymbolID(*syb, sybId);


		if (node._Type == ForNode::ForType::Traditional)
		{
			TypeSymbol BoolType(TypesEnum::Bool);

			_LookingForTypes.push(BoolType);
			OnExpressionTypeNode(node._BoolExpression._Value.get(),GetValueMode::Read);
			_LookingForTypes.pop();
		
			OnPostfixVariableNode(node._OnNextStatement);
		}
		else
		{
			OnExpressionTypeNode(node._Modern_List._Value.get(), GetValueMode::Read);
		}

		for (const auto& node2 : node._Body._Nodes)
		{
			OnStatement(*node2);
		}
	}
	else
	{
		syb = Symbol_GetSymbol(sybId).value();
	}

	if (_PassType == PassType::FixedTypes)
	{
		if (node._Type == ForNode::ForType::Traditional)
		{
			{
				auto& VarType = syb->VarType;
				Type_Convert(node._typeNode, VarType);
				VarType.SetAsLocation();


				auto Ex = node._Traditional_Assignment_Expression._Value.get();
				Type_DeclareVarableCheck(VarType, Ex, NeverNullptr(node._typeNode._name.token));
				
				if (node._Traditional_Assignment_Expression._Value)
				{
					OnExpressionTypeNode(node._Traditional_Assignment_Expression._Value.get(), GetValueMode::Read);

					
					syb->SetTovalid();

					auto& VarType = syb->VarType;


					auto& Ex = _LastExpressionType;
					auto token = node._typeNode._name.token;
					Type_DeclareVariableTypeCheck(VarType, Ex, NeverNullptr(token));
				}
			}

			{
				TypeSymbol BoolType(TypesEnum::Bool);

				_LookingForTypes.push(BoolType);
				OnExpressionTypeNode(node._BoolExpression._Value.get(), GetValueMode::Read);
				_LookingForTypes.pop();

				if (_PassType == PassType::FixedTypes)
				{
					if (!Type_CanBeImplicitConverted(_LastExpressionType, BoolType))
					{
						auto  Token = _LastLookedAtToken.value();
						LogError_CantCastImplicitTypes(Token, _LastExpressionType, BoolType, true);
					}
				}
			}

			{
				OnPostfixVariableNode(node._OnNextStatement);
			}

			for (const auto& node2 : node._Body._Nodes)
			{
				OnStatement(*node2);
			}
		}
		else
		{
			{
				auto& VarType = syb->VarType;
				Type_Convert(node._typeNode, VarType);
				VarType.SetAsLocation();
			}

			auto Ex = node._Modern_List._Value.get();

			{
				_LookingForTypes.push(TypeSymbol(TypesEnum::Any));
				OnExpressionTypeNode(Ex, GetValueMode::Read);
				_LookingForTypes.pop();
			}
			auto& ExType = _LastExpressionType;

			auto HasInfo = Type_HasForOverLoadWith(ExType);
			if (!HasInfo.HasValue)
			{
				auto  Token = _LastLookedAtToken.value();
				LogError_DoesNotHaveForOverload(Token, ExType);

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
						auto tep = Symbol_GetAnExplicitlyConvertedFunc(TypeForType);
						if (tep.has_value())
						{
							GetFunc = Symbol_GetSymbol(tep.value()).value();
						}
					}
					else
					{
						auto Tep = Type_CanBeExplicitlyConverted(TypeForType, syb->VarType);
						if (Tep.HasValue && Tep.Value.has_value())
						{
							GetFunc = Tep.Value.value();
						}
					}

					Optional<Symbol*>CheckFunc = {};
					auto V = Type_HasUrinaryOverLoadWith(TypeForType, TokenType::QuestionMark);

					if (V.HasValue && V.Value.has_value() )
					{
						auto BoolType = TypeSymbol(TypesEnum::Bool);
						auto retType = V.Value.value()->VarType;
						if (Type_CanBeImplicitConverted(retType, BoolType, false)) 
						{
							CheckFunc = V.Value.value();
						}
						else
						{
							auto  Token = _LastLookedAtToken;
							LogError_CantCastImplicitTypes(Token.value(), BoolType, retType, false);
						}
					}

					if (!GetFunc.has_value())
					{
						const NeverNullPtr<Token> token = NeverNullptr(node._typeNode._name.token);

						if (syb->VarType.IsAn(TypesEnum::Var)) {
							LogError(ErrorCodes::InValidType, token->OnLine, token->OnPos,
								"The Type '" + ToString(TypeForType) + "' has no cast(->) overload.it is needed to access the object for the 'for' loop.");
						}
						else
						{
							LogError(ErrorCodes::InValidType, token->OnLine, token->OnPos,
								"The Type '" + ToString(TypeForType) + "' has no explicit cast(->) overload for the type '" + ToString(syb->VarType) + "'.it is needed to access the object for  the 'for' loop.");
						}

						syb->VarType.SetType(TypesEnum::Null);
					}
					else if (!CheckFunc.has_value())
					{
						const NeverNullPtr<Token> token =NeverNullptr(node._typeNode._name.token);

						LogError(ErrorCodes::InValidType, token->OnLine, token->OnPos,
							"The Type '" + ToString(TypeForType) + "' has no exist(?) overload.it is needed to check when to end the loop.");

						syb->VarType.SetType(TypesEnum::Null);
					}
					else
					{
						ForExpresion_Data g;
						g.FuncGetLoopAble = HasInfo.Value.value();
						g.FuncToGet = GetFunc.value();
						g.FuncToCheck = CheckFunc.value();
						_For_Datas.AddValue(Symbol_GetSymbolID(node), g);


						
						auto token = NeverNullptr(node._typeNode._name.token);
						Type_DeclareVariableTypeCheck(syb->VarType, g.FuncToGet->Get_Info<FuncInfo>()->Ret, token);
					}
				}
				else
				{
					UCodeLangUnreachable();
				}
			}


			for (const auto& node2 : node._Body._Nodes)
			{
				OnStatement(*node2);
			}
		}

	}
	else if (_PassType == PassType::BuidCode)
	{
		Debug_Add_SetLineNumber(NeverNullptr(node._Name), _IR_LookingAtIRBlock->GetIndex());
		if (node._Type == ForNode::ForType::Traditional)
		{
			IRInstruction* OnVarable{};
			bool IsStructObjectPassRef = false;
			if (node._Traditional_Assignment_Expression._Value)
			{


				OnVarable = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(syb->VarType));
				syb->IR_Ins = OnVarable;


				IsStructObjectPassRef = Type_IsStructPassByRef(syb);

				if (IsStructObjectPassRef)
				{
					_IR_IRlocations.push({ OnVarable ,false });
				}


				OnExpressionTypeNode(node._Traditional_Assignment_Expression._Value.get(), GetValueMode::Read);

				IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, syb->VarType);

				IR_Build_OnStoreVarable(IsStructObjectPassRef, OnVarable, syb, sybId);

				FileDependency_AddDependencyToCurrentFile(syb->VarType);
			}


			{
				TypeSymbol BoolType(TypesEnum::Bool);
				_LookingForTypes.push(BoolType);

				size_t BoolCode;
				size_t BoolJumps;
				if (_PassType == PassType::BuidCode)
				{
					BoolCode = _IR_LookingAtIRBlock->GetIndex();
					BoolJumps =IR_GetJumpsIndex();
				}
				OnExpressionTypeNode(node._BoolExpression._Value.get(), GetValueMode::Read);

				IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, BoolType);

				IRInstruction* BoolCode2 = _IR_LastExpressionField;
				IRBlock::NewConditionalFalseJump_t IfIndex = _IR_LookingAtIRBlock->NewConditionalFalseJump(BoolCode2);


				_LookingForTypes.pop();


				for (const auto& node2 : node._Body._Nodes)
				{
					OnStatement(*node2);
				}

				OnPostfixVariableNode(node._OnNextStatement);

				_IR_LookingAtIRBlock->NewJump(BoolCode);

				size_t BreakCode = _IR_LookingAtIRBlock->GetIndex();

				_IR_LookingAtIRBlock->UpdateConditionaJump(IfIndex.ConditionalJump, IfIndex.logicalNot, BreakCode);


				IR_Build_UpdateJumpsBreakContiunes(BoolJumps,BoolCode, BreakCode);
			}

		}
		else
		{

			const ForExpresion_Data& Data = _For_Datas.at(Symbol_GetSymbolID(node));
			{
				FileDependency_AddDependencyToCurrentFile(Data.FuncGetLoopAble);
				FileDependency_AddDependencyToCurrentFile(Data.FuncToCheck);
				FileDependency_AddDependencyToCurrentFile(Data.FuncToGet);
			}

			{
				auto finfo = Data.FuncGetLoopAble->Get_Info<FuncInfo>();
				auto Ex = node._Modern_List._Value.get();
				_LookingForTypes.push(finfo->Pars[0].Type);
				OnExpressionTypeNode(Ex, GetValueMode::Read);
				_LookingForTypes.pop();

				IR_Build_AddDestructorToStack(_LastExpressionType,_IR_LastExpressionField);
			}
			
			{
				Get_FuncInfo f;
				f.Func = Data.FuncGetLoopAble->Get_Info<FuncInfo>();
				f.SymFunc = Data.FuncGetLoopAble;
				f.ThisPar = Get_FuncInfo::ThisPar_t::PushFromLast;
				IR_Build_FuncCall(f, {}, {});
			}
			IRInstruction* Loopobject = _IR_LastExpressionField;
			auto LoopObjectType = _LastExpressionType;
			{
				TypeSymbol BoolType(TypesEnum::Bool);
				_LookingForTypes.push(BoolType);
				auto BoolCode = _IR_LookingAtIRBlock->GetIndex();
				auto BoolJumps = IR_GetJumpsIndex();
				{//get if check
					Get_FuncInfo f;
					f.Func = Data.FuncToCheck->Get_Info<FuncInfo>();
					f.SymFunc = Data.FuncToCheck;
					f.ThisPar = Get_FuncInfo::ThisPar_t::PushFromLast;

					
					if (f.Func->Pars[0].Type.IsAddress())
					{
						if (!LoopObjectType.IsAddress())
						{
							_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoadPtr(Loopobject);
						}
					}

					IR_Build_FuncCall(f, {}, {});

					IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, TypesEnum::Bool);
				}
				_LookingForTypes.pop();

				IRInstruction* BoolCode2 = _IR_LastExpressionField;
				IRBlock::NewConditionalFalseJump_t IfIndex = _IR_LookingAtIRBlock->NewConditionalFalseJump(BoolCode2);

				{//get item
					_IR_LastExpressionField = Loopobject;
					Get_FuncInfo f;
					f.Func = Data.FuncToGet->Get_Info<FuncInfo>();
					f.SymFunc = Data.FuncToGet;
					f.ThisPar = Get_FuncInfo::ThisPar_t::PushFromLast;

					if (f.Func->Pars[0].Type.IsAddress())
					{
						if (!LoopObjectType.IsAddress())
						{
							_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoadPtr(Loopobject);
						}
					}

					IR_Build_FuncCall(f, {}, {});

					IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, syb->VarType);
					auto FuncRet = _IR_LastExpressionField;
					syb->IR_Ins = FuncRet;
					IR_Build_AddDestructorToStack(syb, syb->ID,syb->IR_Ins);
				}

				for (const auto& node2 : node._Body._Nodes)
				{
					OnStatement(*node2);
				}


				_IR_LookingAtIRBlock->NewJump(BoolCode);

				size_t BreakCode = _IR_LookingAtIRBlock->GetIndex();

				_IR_LookingAtIRBlock->UpdateConditionaJump(IfIndex.ConditionalJump, IfIndex.logicalNot, BreakCode);
			
				
			

				
				IR_Build_UpdateJumpsBreakContiunes(BoolJumps,BoolCode, BreakCode);
			}
			
			
		}
	}

	_Table.RemoveScope();

	Pop_StackFrame();
}
void SystematicAnalysis::IR_Build_UpdateJumpsBreakContiunes(size_t JumpIndex, size_t BoolCode, size_t BreakCode)
{
	for (size_t i = JumpIndex; i < _IR_Jumps.size(); i++)
	{
		auto& Item = _IR_Jumps[i];

		switch (Item.Type)
		{
		case Jumps_t::Break:
		{
			_IR_LookingAtIRBlock->UpdateJump(Item.JumpIns, BreakCode);
		}
		break;
		case Jumps_t::Continue:
		{
			_IR_LookingAtIRBlock->UpdateJump(Item.JumpIns, BoolCode);
		}
		break;
		default:
			break;
		}
	}
	IR_RemoveJumps(JumpIndex);
}
void SystematicAnalysis::OnContinueNode(const ContinueNode& node)
{
	if (_PassType == PassType::GetTypes)
	{

	}


	if (_PassType == PassType::BuidCode)
	{

		JumpsData v;
		v.Type = Jumps_t::Continue;
		v.JumpIns = _IR_LookingAtIRBlock->NewJump();
		_IR_Jumps.push_back(v);
	}
}
void SystematicAnalysis::OnBreakNode(const BreakNode& node)
{
	if (_PassType == PassType::GetTypes)
	{

	}

	if (_PassType == PassType::BuidCode)
	{
		JumpsData v;
		v.Type = Jumps_t::Break;
		v.JumpIns = _IR_LookingAtIRBlock->NewJump();
		_IR_Jumps.push_back(v);
	}
}
void SystematicAnalysis::OnLambdaNode(const LambdaNode& node)
{
	const String LambdaName = CompilerGenerated("Lambda") + std::to_string((uintptr_t)&node);
	if (_PassType == PassType::GetTypes)
	{
		_Table.AddScope(LambdaName);

		auto& LambdaSym = Symbol_AddSymbol(SymbolType::LambdaObject, LambdaName, _Table._Scope.ThisScope, AccessModifierType::Public);
		_Table.AddSymbolID(LambdaSym, Symbol_GetSymbolID(node));
		LambdaSym.Info =Unique_ptr<LambdaInfo>(new LambdaInfo());


		for (auto& Item : node._Pars._Parameters)
		{
			auto& Sym = Symbol_AddSymbol(SymbolType::ParameterVarable, Item._Name.AsString()
				, _Table._Scope.ScopeHelper::GetApendedString(Item._Name.AsStringView()),AccessModifierType::Public);

			_Table.AddSymbolID(Sym, Symbol_GetSymbolID(Item));
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
	if (_PassType == PassType::FixedTypes)
	{
		
		_Table.AddScope(LambdaName);

		auto& LambdaSym = *Symbol_GetSymbol(Symbol_GetSymbolID(node));
		LambdaInfo* Info = LambdaSym.Get_Info<LambdaInfo>();
		Info->Ret = TypeSymbol(TypesEnum::Var);

		for (size_t i = 0; i < node._Pars._Parameters.size(); i++)
		{
			auto& Item = node._Pars._Parameters[i];
			Symbol& Sym = _Table.GetSymbol(Symbol_GetSymbolID(Item));

			Type_ConvertAndValidateType(Item._Type,Sym.VarType, NodeSyb_t::Parameter);

			Info->Pars.push_back({ Item._IsOutVarable,Sym.VarType });
		}
		
		


		if (node._Statements.has_value())
		{
			{
				VarableUseData Data;
				_Varable.push(std::move(Data));
			}

			OnStatementsWithSetableRet(node._Statements.value(), Info->Ret, NeverNullptr(node._LambdaStart));


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


					for (size_t i = 0; i < node._Pars._Parameters.size(); i++)
					{
						auto& ParItem = node._Pars._Parameters[i];
						SymbolID ParID = Symbol_GetSymbolID(ParItem);
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


					for (size_t i = 0; i < node._Pars._Parameters.size(); i++)
					{
						auto& ParItem = node._Pars._Parameters[i];
						SymbolID ParID = Symbol_GetSymbolID(ParItem);
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
					const NeverNullPtr<Token> Token = NeverNullptr(node._LambdaStart);

					LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos, "cant indirect pass the varable  '" + 
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
			auto& SymClass = Symbol_AddSymbol(SymbolType::Type_class, LambdaClassName, FullName,AccessModifierType::Public);
		
			const SymbolID ClassSymID = Symbol_GetSymbolID(&node._Capture);

			_Table.AddSymbolID(SymClass, ClassSymID);

			ClassInfo* Classinfo = new ClassInfo();
			SymClass.Info =Unique_ptr<Symbol_Info>(Classinfo);

			Classinfo->FullName = SymClass.FullName;
			//

			bool HasDropCall = false;
			for (auto& Item : Info->_CapturedVarables)
			{
				Classinfo->AddField(Item.Name, Item.Type);
				
				if (Symbol_HasDestructor(Item.Type))
				{
					HasDropCall = true;
				}
			}

			if (HasDropCall)
			{
				auto Sym = Symbol_MakeNewDropFuncSymbol(Classinfo, TypeSymbol(ClassSymID));
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

				TepFuninfo.Pars.insert(TepFuninfo.Pars.begin(), { false,ClassPtr });

				String funcFullName= SymClass.FullName;
				ScopeHelper::GetApendedString(funcFullName, Overload_Invoke_Func);

				TepFuninfo.FullName = funcFullName;

				auto& SymClass = Symbol_AddSymbol(SymbolType::Func,Overload_Invoke_Func, funcFullName,AccessModifierType::Public);
				SymClass.Info =Unique_ptr<FuncInfo>(new FuncInfo(std::move(TepFuninfo)));
				SymClass.PassState = _PassType;

				Info->_ClassCall = (FuncInfo*)SymClass.Info.get();
			}

			//
			_LastExpressionType = TypeSymbol(ClassSymID);
		}
		else
		{
			String funcPtrName = Str_GetLambdaFuncTepName(LambdaName);
			String funcFullName = _Table._Scope.ThisScope;
			ScopeHelper::GetApendedString(funcFullName, funcPtrName);

			FuncInfo TepFuninfo;
			TepFuninfo.Pars = Info->Pars;
			TepFuninfo.Ret = Info->Ret;
			TepFuninfo.FullName = funcFullName;
			auto TepLambdaFunc = GetTepFuncPtrSyb(funcPtrName, &TepFuninfo).value();
			TepLambdaFunc->PassState = _PassType;


			_LastExpressionType = TypeSymbol(TepLambdaFunc->ID);
		}
	}
	else if (_PassType == PassType::BuidCode)
	{
		_Table.AddScope(LambdaName);

		auto& LambdaSym = _Table.GetSymbol(Symbol_GetSymbolID(node));
		LambdaInfo* Info = LambdaSym.Get_Info<LambdaInfo>();

		if (Info->UsesOuterScope())
		{
			const SymbolID ClassSymID = Symbol_GetSymbolID(&node._Capture);
			auto& SymClass = _Table.GetSymbol(ClassSymID);

			ClassInfo* ClassSymInfo = SymClass.Get_Info<ClassInfo>();

			auto ClassType = TypeSymbol(ClassSymID);
			auto ClassTypeIR = IR_ConvertToIRType(ClassType);

			auto ClassTypeIRPtr = ClassType;
			ClassTypeIRPtr._IsAddress = true;

			auto TepIRObject = _IR_LookingAtIRBlock->NewLoad(ClassTypeIR);


			

			IRStruct* ClassStruct = _IR_Builder.GetSymbol(ClassTypeIR._symbol)->Get_ExAs<IRStruct>();

			for (size_t i = 0; i < Info->_CapturedVarables.size(); i++)
			{
				auto& Item = Info->_CapturedVarables[i];


				IRInstruction* Value = nullptr;
				if (Item.Sym->Type == SymbolType::StackVarable)
				{
					Value = _IR_LookingAtIRBlock->NewLoad(Item.Sym->IR_Ins);
				}
				else if (Item.Sym->Type == SymbolType::ParameterVarable)
				{
					Value = _IR_LookingAtIRBlock->NewLoad(Item.Sym->IR_Par);
				}
				else
				{
					UCodeLangUnreachable();
				}


				auto Member = _IR_LookingAtIRBlock->New_Member_Access(TepIRObject, ClassStruct, i);
				_IR_LookingAtIRBlock->NewStore(Member, Value);

				{
					auto FullName = _Table._Scope.ThisScope;
					ScopeHelper::GetApendedString(FullName, Item.Name);
					auto& Sym = Symbol_AddSymbol(SymbolType::Class_Field, Item.Name, FullName,AccessModifierType::Public);//to make a this indirection.
					Sym.VarType = Item.Type;
				}
			}
			
			if (Info->_ClassDrop)
			{//build drop func
				auto oldFunc = _IR_LookingAtIRFunc;
				auto oldblock = _IR_LookingAtIRBlock;
				//



				_IR_LookingAtIRFunc = _IR_Builder.NewFunc(IR_MangleName(Info->_ClassDrop), IR_ConvertToIRType(Info->Ret));
				_IR_LookingAtIRBlock = _IR_LookingAtIRFunc->NewBlock(".");

				_IR_LookingAtIRFunc->Pars.resize(1);
				{
					IRPar V;
					V.identifier = _IR_Builder.ToID(ThisSymbolName);
					V.type = IR_ConvertToIRType(ClassTypeIRPtr);
					_IR_LookingAtIRFunc->Pars[0] = V;
				}

				IR_Build_FuncDropUsingFields(ClassSymInfo, ClassTypeIR);

				_IR_LookingAtIRBlock->NewRet();
				//
				_IR_LookingAtIRFunc = oldFunc;
				_IR_LookingAtIRBlock = oldblock;
			}
			
			{//build func
				auto oldFunc = _IR_LookingAtIRFunc;
				auto oldblock = _IR_LookingAtIRBlock;
				//


				
				_IR_LookingAtIRFunc = _IR_Builder.NewFunc(IR_MangleName(Info->_ClassCall), IR_ConvertToIRType(Info->Ret));
				_IR_LookingAtIRBlock = _IR_LookingAtIRFunc->NewBlock(".");


				_IR_LookingAtIRFunc->Pars.resize(node._Pars._Parameters.size()+1);
				{
					IRPar V;
					V.identifier = _IR_Builder.ToID(ThisSymbolName);
					V.type = IR_ConvertToIRType(ClassTypeIRPtr);
					_IR_LookingAtIRFunc->Pars[0] = V;
				}
				for (size_t i = 0; i < node._Pars._Parameters.size(); i++)
				{
					auto& Item = node._Pars._Parameters[i];
					Symbol& Sym = _Table.GetSymbol(Symbol_GetSymbolID(Item));

					IRPar V;
					V.type = IR_ConvertToIRType(Info->Pars[i]);
					V.identifier = _IR_Builder.ToID((IRidentifier)Item._Name.token->Value._String);
					_IR_LookingAtIRFunc->Pars[i+1] = V;

					Sym.IR_Par = &_IR_LookingAtIRFunc->Pars[i+1];
				}

				ClassStackInfo _Data; 
				_Data.Info = ClassSymInfo;
				_Data.Syb = &SymClass;
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

				_IR_LookingAtIRBlock->NewRet();
				//
				_IR_LookingAtIRFunc = oldFunc;
				_IR_LookingAtIRBlock = oldblock;
			}


			_IR_LastExpressionField = TepIRObject;
			_LastExpressionType = ClassType;
		}
		else
		{
			auto oldFunc = _IR_LookingAtIRFunc;
			auto oldblock = _IR_LookingAtIRBlock;
			//
			
			auto FuncName = _Table._Scope.ThisScope;

			_IR_LookingAtIRFunc = _IR_Builder.NewFunc(FuncName, IR_ConvertToIRType(Info->Ret));
			_IR_LookingAtIRBlock = _IR_LookingAtIRFunc->NewBlock(".");


			_IR_LookingAtIRFunc->Pars.resize(node._Pars._Parameters.size());
			for (size_t i = 0; i < node._Pars._Parameters.size(); i++)
			{
				auto& Item = node._Pars._Parameters[i];
				Symbol& Sym = _Table.GetSymbol(Symbol_GetSymbolID(Item));

				IRPar V;
				V.type = IR_ConvertToIRType(Info->Pars[i]);
				V.identifier = _IR_Builder.ToID((IRidentifier)Item._Name.token->Value._String);
				_IR_LookingAtIRFunc->Pars[i] =V;

				Sym.IR_Par = &_IR_LookingAtIRFunc->Pars[i];
			}


			if (node._Statements.has_value())
			{
				for (const auto& node2 : node._Statements.value()._Nodes)
				{
					OnStatement(*node2);
				}
			}

			_IR_LookingAtIRBlock->NewRet();
			//
			_IR_LookingAtIRFunc = oldFunc;
			_IR_LookingAtIRBlock = oldblock;

			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoadFuncPtr(_IR_Builder.ToID(FuncName));


			FuncInfo TepFuninfo;
			TepFuninfo.Pars = Info->Pars;
			TepFuninfo.Ret = Info->Ret;

			String funcPtrName = Str_GetLambdaFuncTepName(LambdaName);
			String funcFullName =ScopeHelper::GetReMoveScope(_Table._Scope.ThisScope);
			ScopeHelper::GetApendedString(funcFullName, funcPtrName);
			_LastExpressionType = TypeSymbol(GetTepFuncPtrSyb(funcFullName, &TepFuninfo).value()->ID);
		}


		_Table.RemoveScope();
	}
}
void SystematicAnalysis::OnTrait(const TraitNode& node)
{
	const bool IsgenericInstantiation = _Generic_GenericSymbolStack.size() && _Generic_GenericSymbolStack.top().NodeTarget == &node;
	const bool Isgeneric = node._generic._Values.size();
	const bool Isgeneric_t = Isgeneric && IsgenericInstantiation == false;
	const bool CheckgenericForErr = (Isgeneric_t && (_PassType == PassType::GetTypes || _PassType == PassType::FixedTypes));

	const String ClassName = IsgenericInstantiation ? ScopeHelper::GetNameFromFullName(_Generic_GenericSymbolStack.top()._IR_GenericFuncName) : node._Name.AsString();
	
	_Table.AddScope(ClassName);
	SymbolID sybId = Symbol_GetSymbolID(node);//Must be pass AddScope thats how GetSymbolID works for Generics.

	auto& Syb = _PassType == PassType::GetTypes ?
		Symbol_AddSymbol(Isgeneric_t ? SymbolType::Generic_Trait : SymbolType::Trait_class
			, (String)ClassName, _Table._Scope.ThisScope,node._Access) :
		*Symbol_GetSymbol(sybId);

	//we may jump to this node non linearly
	if (Syb.PassState == _PassType)
	{
		_Table.RemoveScope();
		return;
	}

	if (_PassType == PassType::GetTypes)
	{
		_Table.AddSymbolID(Syb, sybId);


		TraitInfo* info = new TraitInfo();
		Syb.Info.reset(info);


		auto& SybClass = Symbol_AddSymbol(SymbolType::Type_class
			, (String)ClassName + "%Class", _Table._Scope.ThisScope + "%Class",node._Access);
		_Table.AddSymbolID(SybClass, Symbol_GetSymbolID(node._Name));

		Syb.NodePtr = node.As();

		auto TepClass = new ClassInfo();
		TepClass->FullName = SybClass.FullName;

		info->TraitClassInfo = &SybClass;
		SybClass.Info.reset(TepClass);


		SybClass.VarType.SetType(SybClass.ID);
		info->Conext = Save_SymbolContextRemoveOneScopeName();
		Generic_InitGenericalias(node._generic,IsgenericInstantiation, info->_GenericData);

		
		if (CheckgenericForErr)
		{
			_Table.AddScope(GenericTestStr);
		}

		ClassStackInfo classStackInfo;
		classStackInfo.Syb = &SybClass;
		classStackInfo.Info = TepClass;
		_ClassStack.push(classStackInfo);


		SybClass.OutputIR = false;

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
				_Func.HasBody = funcnode._Body.has_value();

				info->_Funcs.push_back(_Func);

				if (funcSyb->Get_Info<FuncInfo>()->_FuncType == FuncInfo::FuncType::Drop)
				{
					HasDrop = true;
				}
			}break;
			case NodeType::DeclareVariableNode:
			{
				size_t Index = _Table.Symbols.size();

				DeclareVariableNode& funcnode = *DeclareVariableNode::As(node.get());

				OnDeclareVariablenode(funcnode,DeclareStaticVariableNode_t::ClassField);

				Symbol* funcSyb = _Table.Symbols[Index].get();

				TraitVar _Func;
				_Func.Syb = funcSyb;

				info->_Vars.push_back(_Func);
			}break;
			default:break;
			}
		}


		if (!HasDrop)
		{
			String FuncDropFunc = _Table._Scope.ThisScope;
			ScopeHelper::GetApendedString(FuncDropFunc, ClassDestructorFunc);

			TraitFunc _Func;
			_Func.Syb =&Symbol_AddSymbol(SymbolType::Func,ClassDestructorFunc, FuncDropFunc,AccessModifierType::Public);
			
			FuncInfo* Funcinfo = new FuncInfo();
			_Func.Syb->Info.reset(Funcinfo);

			Funcinfo->_FuncType = FuncInfo::FuncType::Drop;
			Funcinfo->FullName = FuncDropFunc;


			TypeSymbol FuncPtr = SybClass.ID;
			FuncPtr._IsAddress = true;
			Funcinfo->Pars.push_back({ false,FuncPtr });
			Funcinfo->Ret = TypesEnum::Void;

			_Func.HasBody = false;
			info->_Funcs.push_back(_Func);
		}

		
		if (CheckgenericForErr)
		{
			_Table.RemoveScope();
		}
		_ClassStack.pop();
	}
	else if (_PassType == PassType::FixedTypes)
	{
		TraitInfo* info = Syb.Get_Info<TraitInfo>();

		auto TepClass = info->TraitClassInfo->Get_Info<ClassInfo>();

		if (CheckgenericForErr)
		{
			_Table.AddScope(GenericTestStr);
		}

		ClassStackInfo classStackInfo;
		classStackInfo.Syb =Symbol_GetSymbol(Symbol_GetSymbolID(node._Name)).value();
		classStackInfo.Info = TepClass;
		_ClassStack.push(classStackInfo);

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
			case NodeType::DeclareVariableNode:
			{
				DeclareVariableNode& funcnode = *DeclareVariableNode::As(node.get());
				OnDeclareVariablenode(funcnode, DeclareStaticVariableNode_t::ClassField);
			}
			break;
			default:break;
			}
		}


		if (CheckgenericForErr)
		{
			_Table.RemoveScope();
		}
		_ClassStack.pop();
	}
	else if (_PassType == PassType::BuidCode && !Isgeneric_t)
	{
		//
		TraitInfo* info = Syb.Get_Info<TraitInfo>();
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
	

		Trait_Data& TraitData = _Lib.Get_Assembly().AddTrait(ScopeHelper::GetNameFromFullName(Syb.FullName), Syb.FullName);
		TraitData.TypeID = Type_GetTypeID(TypesEnum::CustomType, Syb.ID);
}


	_Table.RemoveScope();
}
void SystematicAnalysis::OnTag(const TagTypeNode& node)
{
	bool IsgenericInstantiation = _Generic_GenericSymbolStack.size() && _Generic_GenericSymbolStack.top().NodeTarget == &node;
	bool Isgeneric = node._generic._Values.size();
	bool Isgeneric_t = Isgeneric && IsgenericInstantiation == false;


	const String ClassName = IsgenericInstantiation ? ScopeHelper::GetNameFromFullName(_Generic_GenericSymbolStack.top()._IR_GenericFuncName) : (String)node._AttributeName.token->Value._String;
	_Table.AddScope(ClassName);
	SymbolID sybId = Symbol_GetSymbolID(node);//Must be pass AddScope thats how GetSymbolID works for Generics.

	auto& Syb = _PassType == PassType::GetTypes ?
		Symbol_AddSymbol(Isgeneric_t ? SymbolType::Generic_Tag : SymbolType::Tag_class
			, (String)ClassName, _Table._Scope.ThisScope,node._Access) :
		*Symbol_GetSymbol(sybId);

	//we may jump to this node non linearly
	if (Syb.PassState == _PassType)
	{
		_Table.RemoveScope();
		return;
	}

	if (_PassType == PassType::GetTypes)
	{
		_Table.AddSymbolID(Syb, sybId);
		Syb.NodePtr = node.As();

	
		TagInfo* info = new TagInfo();
		Syb.Info.reset(info);
		info->Conext = Save_SymbolContextRemoveOneScopeName();
		Generic_InitGenericalias(node._generic, IsgenericInstantiation, info->_GenericData);
	}
	else if (_PassType == PassType::FixedTypes)
	{

	}
	else if (_PassType == PassType::BuidCode)
	{
		if (Syb.Type == SymbolType::Tag_class) 
		{
			Tag_Data& TagData = _Lib.Get_Assembly().AddTag(ScopeHelper::GetNameFromFullName(Syb.FullName), Syb.FullName);
			TagData.TypeID = Type_GetTypeID(TypesEnum::CustomType, Syb.ID);
		}
		else
		{

		}
	}


	_Table.RemoveScope();
}
void SystematicAnalysis::OnBitCast(const BitCastExpression& node)
{
	if (_PassType == PassType::FixedTypes)
	{
		if (!IsInUnSafeBlock())
		{
			auto Token = node._KeywordToken;
			LogError(ErrorCodes::ExpectingSequence, Token->OnLine, Token->OnPos, "Cant do bitcast in safe mode.");
			return;
		}

		auto Token = node._KeywordToken;
		TypeSymbol ToType = Type_ConvertAndValidateType(node._Type,NodeSyb_t::Any);
		
		if (!ToType.IsBadType() && !Type_IsUnMapType(ToType)) 
		{
			if (!ToType.IsAddress() && !ToType.IsAddressArray() && (ToType._Type != TypesEnum::uIntPtr))
			{
				LogError(ErrorCodes::ExpectingSequence, Token->OnLine, Token->OnPos, "bitcast<T>. T must be an address or an uintptr and not '" + ToString(ToType) + "'");
			}
		}
		bool IsIntPtr = !ToType.IsAddress();

		if (IsIntPtr) {
			_LookingForTypes.push(TypesEnum::uIntPtr);
		}
		else
		{
			TypeSymbol V = TypesEnum::Any;
			V.SetAsAddress();
			_LookingForTypes.push(V);
		}
		
		OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);
		
		_LookingForTypes.pop();

		auto ExType = _LastExpressionType;
		

		_LastExpressionType = ToType;
	}
	else if (_PassType == PassType::BuidCode)
	{
		TypeSymbol ToType = Type_ConvertAndValidateType(node._Type, NodeSyb_t::Any);

		bool IsIntPtr = !ToType.IsAddress();

		//!to stop *(_Expression) and do (_Expression)
		if (!IsIntPtr) {
			_LookingForTypes.push(TypesEnum::uIntPtr);
		}
		else
		{
			TypeSymbol V = TypesEnum::Any;
			V.SetAsAddress();
			_LookingForTypes.push(V);
		}

		OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);

		_LookingForTypes.pop();

		auto ExType = _LastExpressionType;

		_LastExpressionType = ToType;
	}
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
void SystematicAnalysis::Symbol_InheritTrait(NeverNullPtr<Symbol> Syb, ClassInfo* ClassInfo,const NeverNullPtr<Symbol> Trait, const NeverNullPtr<Token> ClassNameToken)
{
	const TraitInfo* Traitinfo = Trait->Get_Info<TraitInfo>();

	auto ID = ClassInfo->Get_InheritedTypesIndex(Trait.value());
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

							if (TraitPar.Type._CustomTypeSymbol == Traitinfo->TraitClassInfo->ID)
							{
								SubTraitParType.Type._CustomTypeSymbol = Syb->ID;
							}

							if (!Type_AreTheSame(SubTraitParType, FuncPar))
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
				LogError_MissingFuncionforTrait(FuncName, Info, Trait, ClassNameToken);
				HasErr = true;
			}
		}
	}

	for (auto& Item : Traitinfo->_Vars)
	{
		auto VarName = ScopeHelper::GetNameFromFullName((String_view)Item.Syb->FullName);
		

		for (auto& Field : ClassInfo->Fields)
		{
			if (Field.Name == VarName)
			{
				HasErr = true;
				LogError(ErrorCodes::Redefinition,"cant inherit '" + Trait->FullName + "' because " + ClassInfo->FullName + " already has a field named '" + (String)VarName + "'", ClassNameToken);
				continue;
			}
		}
		
		ClassInfo->AddField(VarName, Item.Syb->VarType);

		
	}

	if (!HasErr)
	{
	
		//
		{
			ClassStackInfo _ClStack;
			_ClStack._InStatements = false;
			_ClStack.Syb = Syb.value();
			_ClStack.Info = ClassInfo;
			_ClassStack.push(_ClStack);
		}
		
		auto oldpass = _PassType;
		_PassType = PassType::GetTypes;

		{
			Push_ExtendedErr("Were this = " + Syb->FullName + ".When instantiateing trait " + Trait->FullName, ClassNameToken);
		}

		for (auto& Item : IDSyb.AddedFuncs)
		{
			const FuncNode& func = *(FuncNode*)Item.FuncNode;

			auto SybsIndex = _Table.Symbols.size();

			OnFuncNode(func);
		

			Item.Func = _Table.Symbols[SybsIndex].get();

		}


		_PassType = PassType::FixedTypes;

		for (auto& Item : IDSyb.AddedFuncs)
		{
			const FuncNode& func = *(FuncNode*)Item.FuncNode;
			OnFuncNode(func);
		}

		{
			_PassType = oldpass;
			_ClassStack.pop();
		}
		{
			Pop_ExtendedErr();
		}
	}
}
void SystematicAnalysis::Symbol_BuildTrait(const NeverNullPtr<Symbol> Syb, ClassInfo* ClassInfo,const NeverNullPtr<Symbol> Trait, const NeverNullPtr<Token> ClassNameToken)
{

	auto ID = ClassInfo->Get_InheritedTypesIndex(Trait.value());
	ClassInfo_InheritTypeInfo& IDSyb = ClassInfo->_InheritedTypes[ID.value()];

	{
		ClassStackInfo _ClStack;
		_ClStack._InStatements = false;
		_ClStack.Info = ClassInfo;
		_ClassStack.push(_ClStack);
	}

	{
		Push_ExtendedErr("Were this = " + Syb->FullName + ".When instantiateing trait " + Trait->FullName, ClassNameToken);
	}
	for (auto& Item : IDSyb.AddedFuncs)
	{
		const FuncNode& func = *(FuncNode*)Item.FuncNode;
		OnFuncNode(func);
	}
	{
		Pop_ExtendedErr();
	}

	{
		String VTableName = Str_GetClassWithTraitVTableName(Syb->FullName, Trait->FullName);
		auto StaticVarableToID = _IR_Builder.ToID(VTableName);

		auto StaticVarableType = IRType(_IR_Builder.GetSymbol(_IR_Builder.ToID(Str_GetTraitVStructTableName(Trait->FullName)))->identifier);

		auto StaticVarable = _IR_Builder.NewStaticVarable(StaticVarableToID, StaticVarableType);


		auto oldIRFunc = _IR_LookingAtIRFunc;
		auto oldblock = _IR_LookingAtIRBlock;



		if (_IR_Builder._StaticInit.Blocks.size() == 0)
		{
			_IR_Builder._StaticInit.NewBlock(".");
		}

		_IR_LookingAtIRFunc = &_IR_Builder._StaticInit;
		_IR_LookingAtIRBlock = _IR_LookingAtIRFunc->Blocks.front().get();

		//

		auto Ptr = _IR_LookingAtIRBlock->NewLoadPtr(StaticVarableToID);
		for (size_t i = 0; i < IDSyb.Funcs.size(); i++)
		{
			auto& Item = IDSyb.Funcs[i];


			Symbol* Func = Item.Type == ClassInfo_InheritTypeInfo::FuncType::Added ? IDSyb.AddedFuncs[Item.Index].Func : IDSyb.OverLoadedFuncs[Item.Index].Func;
			
			auto Member = _IR_LookingAtIRBlock->New_Member_Dereference(Ptr, StaticVarableType, i);
			_IR_LookingAtIRBlock->NewStore(Member, _IR_LookingAtIRBlock->NewLoadFuncPtr(_IR_Builder.ToID(Func->FullName)));
		}
		//
		_IR_LookingAtIRFunc = oldIRFunc;
		_IR_LookingAtIRBlock = oldblock;
	}


	{
		_ClassStack.pop();
	}
}
void SystematicAnalysis::OnCompileTimeIfNode(const CompileTimeIfNode& node)
{
	if (_PassType == PassType::GetTypes)
	{
		_LookingForTypes.push(TypesEnum::Bool);
		OnExpressionTypeNode(node._Expression._Value.get(),GetValueMode::Read);
		_LookingForTypes.pop();
	}
	else if (_PassType == PassType::FixedTypes)
	{
		
		_LookingForTypes.push(TypesEnum::Bool);
		auto BoolValue = Eval_Evaluate(TypesEnum::Bool,node._Expression);
		_LookingForTypes.pop();

		if (BoolValue.has_value())
		{
			const bool& EvalValue = *(bool*)Eval_Get_Object(BoolValue.value());
		

			if (EvalValue)
			{
				auto PassOld = _PassType;
				_PassType = PassType::GetTypes;
				for (auto& Item : node._Body._Nodes)
				{
					OnStatement(*Item.get());
				}
				_PassType = PassType::FixedTypes;
				for (auto& Item : node._Body._Nodes)
				{
					OnStatement(*Item.get());
				}

				_PassType = PassOld;
			}
			else
			{
				if (node._Else.get())
				{
					ElseNode* Elsenode = ElseNode::As(node._Else.get());

					auto PassOld = _PassType;
					_PassType = PassType::GetTypes;
					
					for (const auto& node3 : Elsenode->_Body._Nodes)
					{
						OnStatement(*node3);
					}

					_PassType = PassType::FixedTypes;
					
					for (const auto& node3 : Elsenode->_Body._Nodes)
					{
						OnStatement(*node3);
					}

					_PassType = PassOld;
				}
			}

			_ValidNodes.AddValue(Symbol_GetSymbolID(node), EvalValue);

		}

	}
	else if (_PassType == PassType::BuidCode)
	{
		const bool EvalValue = _ValidNodes.at(Symbol_GetSymbolID(node));

		if (EvalValue)
		{
			for (auto& Item : node._Body._Nodes)
			{
				OnStatement(*Item.get());
			}
		}
		else
		{
			ElseNode* Elsenode = ElseNode::As(node._Else.get());

			for (const auto& node3 : Elsenode->_Body._Nodes)
			{
				OnStatement(*node3);
			}
		}
	}
}
TypeSymbol SystematicAnalysis::Type_GetUnMapType()
{
	if (!_Type_UnMapTypeSymbol.has_value())
	{
		auto& TypeSyb = Symbol_AddSymbol(SymbolType::Unmaped_Generic_Type, CompilerGenerated("UnMapedType"), CompilerGenerated("UnMapedType"), AccessModifierType::Public);
		_Table.AddSymbolID(TypeSyb, Symbol_GetSymbolID(&TypeSyb));
		_Type_UnMapTypeSymbol = TypeSyb.ID;
	}	
	return TypeSymbol(_Type_UnMapTypeSymbol.value());

}
bool SystematicAnalysis::Type_IsUnMapType(const TypeSymbol& Type) const
{
	auto Syb = Symbol_GetSymbol(Type);
	if (Syb)
	{
		return Syb.value()->Type == SymbolType::Unmaped_Generic_Type;
	}
	return {};
}
void SystematicAnalysis::OnCompileTimeforNode(const CompileTimeForNode& node)
{
	if (_PassType == PassType::GetTypes)
	{
		if (node._Type == CompileTimeForNode::ForType::Traditional)
		{
			auto Token = node._Name;
			LogError(ErrorCodes::ExpectingSequence, Token->OnLine, Token->OnPos, "Traditional for loops are not yet added to CompileTimeforNode");
		}
		else
		{

		}
	}
	else if (_PassType == PassType::FixedTypes)
	{
		if (node._Type == CompileTimeForNode::ForType::modern)
		{

			_LookingForTypes.push(TypesEnum::Any);
			OnExpressionTypeNode(node._Modern_List._Value.get(), GetValueMode::Read);
			_LookingForTypes.pop();

			auto ListType = _LastExpressionType;

			bool CanBeLooped = false;
			Symbol* ListTypeSyb;
			if (ListType._CustomTypeSymbol.HasValue())
			{
				ListTypeSyb = Symbol_GetSymbol(ListType).value().value();
				if (ListTypeSyb)
				{
					if (ListTypeSyb->Type == SymbolType::Type_Pack)
					{
						CanBeLooped = true;
					}
					else
					if (ListTypeSyb->Type == SymbolType::Type_StaticArray)
					{
						CanBeLooped = true;
					}
				}
			}


			if (!CanBeLooped)
			{
				auto Token = node._Name;
				LogError(ErrorCodes::ExpectingSequence, Token->OnLine, Token->OnPos, "Cant Loop over type of '" + ToString(ListType) + "'");
			}
			else
			{

				{//test loop for Errs
					size_t OldErrCount = _ErrorsOutput->Get_Errors().size();
					_Table.AddScope("___forTest");
					const String VarableName = (String)node._Name->Value._String;

					auto& ParSyb = Symbol_AddSymbol(SymbolType::Unmaped_Varable, VarableName, _Table._Scope.GetApendedString(VarableName), AccessModifierType::Public);
					ParSyb.VarType = Type_GetUnMapType();
					_Table.AddSymbolID(ParSyb, Symbol_GetSymbolID(&ParSyb));
					{
						auto Token = NeverNullptr(node._Name);
						Push_ExtendedErr("Were '" + VarableName + "' is unmaped for Errors.Before for loop expansion", Token);
					}
					CompileTimeforNodeEvaluateStatements(node);
					{
						Pop_ExtendedErr();
					}
					_Table.RemoveScope();


					bool GotErrs = OldErrCount != _ErrorsOutput->Get_Errors().size();
					if (GotErrs) { return; }
				}


				//
				if (ListTypeSyb->Type == SymbolType::Type_Pack)
				{
					const Node* NodePtr = node._Modern_List._Value.get();
					if (NodePtr->Get_Type() == NodeType::ValueExpressionNode)
					{
						const ValueExpressionNode* ValueNodePtr = ValueExpressionNode::As(NodePtr);
						const auto ExValuePtr = ValueNodePtr->_Value.get();
						if (ExValuePtr->Get_Type() == NodeType::ReadVariableNode)
						{
							const auto ReadVarablePtr = ReadVariableNode::As(ExValuePtr);

							_GetExpressionMode.push(GetValueMode::Read);
							GetMemberTypeSymbolFromVar_t V;
							bool VBool = Symbol_MemberTypeSymbolFromVar(ReadVarablePtr->_VariableName, V);
							_GetExpressionMode.pop();
							
							if (VBool)
							{
								auto ParSyb = V._Symbol;
								if (ParSyb->Type == SymbolType::ParameterVarable)
								{
									const TypePackInfo* PackInfo = ListTypeSyb->Get_Info<TypePackInfo>();

									const String ScopeName = std::to_string(Symbol_GetSymbolID(node).AsInt());
									const String VarableName = (String)node._Name->Value._String;

									CompileTimeforNode TepData;
									TepData.SybToLoopOver = V._Symbol;
									

									for (size_t i = 0; i < PackInfo->List.size(); i++)
									{
										auto& Item = PackInfo->List[i];
									
										_Table.AddScope(ScopeName + std::to_string(i));

										
										auto& ParSyb = Symbol_AddSymbol(SymbolType::ParameterVarable, VarableName, _Table._Scope.GetApendedString(VarableName), AccessModifierType::Public);
										_Table.AddSymbolID(ParSyb, Symbol_GetSymbolID(&ParSyb));
										ParSyb.VarType = Item;

										size_t OldErrCount = _ErrorsOutput->Get_Errors().size();
										{
											auto Token = NeverNullptr(node._Name);
											Push_ExtendedErr("Were '" + VarableName + "' is type of " + ToString(ParSyb.VarType), Token);
										}
										CompileTimeforNodeEvaluateStatements(node);
										{
											Pop_ExtendedErr();
										}

										TepData.SybItems.push_back(&ParSyb);

										
										_Table.RemoveScope();

										bool GotErrs = OldErrCount != _ErrorsOutput->Get_Errors().size();
										if (GotErrs) { continue; }
									}

									_ForNodes.AddValue(Symbol_GetSymbolID(node),std::move(TepData));
								}
							}
						}
					}
				
				}
				else if (ListTypeSyb->Type == SymbolType::Type_StaticArray)
				{
					const StaticArrayInfo* StaticInfo = ListTypeSyb->Get_Info<StaticArrayInfo>();


					const String ScopeName = std::to_string(Symbol_GetSymbolID(node).AsInt());
					const String VarableName = (String)node._Name->Value._String;

					auto ListArray = Eval_Evaluate(ListType,node._Modern_List);
					if (ListArray.has_value())
					{
						size_t ItemSize = Type_GetSize(StaticInfo->Type).value();

						RawEvaluatedObject _DataAsIndex;
						_DataAsIndex.ObjectSize = ItemSize;
						_DataAsIndex.Object_AsPointer.reset(new Byte[ItemSize]);
						
						CompileTimeforNode TepData;
						TepData.SybToLoopOver = ListTypeSyb;

						auto& ListArrayValue = ListArray.value();

						for (size_t i = 0; i < StaticInfo->Count; i++)
						{
							void* ItemOffset = ListArrayValue.EvaluatedObject.Object_AsPointer.get() + (i * ItemSize);
							Eval_Set_ObjectAs(StaticInfo->Type,_DataAsIndex,ItemOffset, ItemSize);

							_Table.AddScope(ScopeName + std::to_string(i));
							

							auto& ParSyb = Symbol_AddSymbol(SymbolType::ConstantExpression, VarableName, _Table._Scope.GetApendedString(VarableName), AccessModifierType::Public);
							_Table.AddSymbolID(ParSyb, Symbol_GetSymbolID(&ParSyb));

							
							ConstantExpressionInfo* ContInfo = new ConstantExpressionInfo();
							ParSyb.Info.reset(ContInfo);

							ContInfo->Ex = _DataAsIndex;
							ParSyb.VarType = StaticInfo->Type;

							size_t OldErrCount = _ErrorsOutput->Get_Errors().size();
							{
								auto Token = NeverNullptr(node._Name);
								Push_ExtendedErr("Were '" + VarableName + "' = " + ToString(ParSyb.VarType, ContInfo->Ex), Token);
							}
							CompileTimeforNodeEvaluateStatements(node);
							{
								Pop_ExtendedErr();
							}
							TepData.SybItems.push_back(&ParSyb);

							


							_Table.RemoveScope();

							bool GotErrs = OldErrCount != _ErrorsOutput->Get_Errors().size();
							if (GotErrs){continue;}
						}

						
						_ForNodes.AddValue(Symbol_GetSymbolID(node), std::move(TepData));
					}
				}
				else
				{
					UCodeLangUnreachable();
				}
			}
		}
	}
	else if (_PassType == PassType::BuidCode)
	{
		CompileTimeforNode& Nodes = _ForNodes.at(Symbol_GetSymbolID(node));
		if (Nodes.SybToLoopOver->Type == SymbolType::ParameterVarable)
		{
			const String ScopeName = std::to_string(Symbol_GetSymbolID(node).AsInt());
			
			for (size_t i = 0; i < Nodes.SybItems.size(); i++)
			{
				auto& Item = Nodes.SybItems[i];

				size_t IRParIndex = _IR_LookingAtIRFunc->Pars.size() - Nodes.SybItems.size() + i;
				Item->IR_Par =&_IR_LookingAtIRFunc->Pars[IRParIndex];

				{
					auto Token = NeverNullptr(node._Name);
					Push_ExtendedErr("Were '" + (String)node._Name->Value._String + "' is type of " + ToString(Item->VarType), Token);
				}
				_Table.AddScope(ScopeName + std::to_string(i));

				for (const auto& node2 : node._body._Nodes)
				{
					OnStatement(*node2);
				}

				_Table.RemoveScope();
				{
					Pop_ExtendedErr();
				}
			}
		}
		else if (Nodes.SybToLoopOver->Type == SymbolType::Type_StaticArray)
		{
			const String ScopeName = std::to_string(Symbol_GetSymbolID(node).AsInt());
			

			for (size_t i = 0; i < Nodes.SybItems.size(); i++)
			{
				auto& Item = Nodes.SybItems[i];

				_Table.AddScope(ScopeName + std::to_string(i));
				{
					ConstantExpressionInfo* ConstExpressionInfo = Item->Get_Info< ConstantExpressionInfo>();
					auto Token = NeverNullptr(node._Name);
					Push_ExtendedErr("Were '" + ScopeHelper::GetNameFromFullName(Item->FullName) + "' = " + ToString(Item->VarType, ConstExpressionInfo->Ex), Token);
				}
				for (const auto& node2 : node._body._Nodes)
				{
					OnStatement(*node2);
				}
				{
					Pop_ExtendedErr();
				}
				_Table.RemoveScope();
			}
		}
	}
}
void SystematicAnalysis::CompileTimeforNodeEvaluateStatements(const CompileTimeForNode& node)
{
	auto TepPass = _PassType;
	size_t OldErrCount = _ErrorsOutput->Get_Errors().size();

	_PassType = PassType::GetTypes;
	for (const auto& node2 : node._body._Nodes)
	{
		OnStatement(*node2);
	}

	bool GotErrs = OldErrCount != _ErrorsOutput->Get_Errors().size();
	if (!GotErrs)
	{
		_PassType = PassType::FixedTypes;
		for (const auto& node2 : node._body._Nodes)
		{
			OnStatement(*node2);
		}

	}
	_PassType = TepPass;
}

void SystematicAnalysis::LogError_MissingFuncionforTrait(const String_view& FuncName, const FuncInfo* Info, const NeverNullPtr<Symbol> Trait, const NeverNullPtr<Token> ClassNameToken)
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

	LogError(ErrorCodes::ExpectingSequence, ClassNameToken->OnLine, ClassNameToken->OnPos, Msg);
}
NeverNullPtr<Symbol> SystematicAnalysis::Symbol_MakeNewDropFuncSymbol(ClassInfo* ClassInfo,const TypeSymbol& ClassAsType)
{

	String FullName = ClassInfo->FullName;
	ScopeHelper::GetApendedString(FullName, (String)ClassDestructorFunc);

	auto DropFunc = &Symbol_AddSymbol(SymbolType::Func, ClassDestructorFunc, FullName,AccessModifierType::Public);
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
	for (size_t i = ClassInfo->Fields.size() - 1; i != (Vector<FieldInfo>::size_type)-1; i--)
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
			LogError_YouMustReturnSomethingError(Token);
		}

	}
}



ImportBindType SybolTypeToImportBindType(SymbolType Type)
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
		auto& Syb = Symbol_AddSymbol(SymbolType::Null, CompilerGenerated("importData") + std::to_string(node._Token->OnLine),"",AccessModifierType::Public);
		
		Imports_Info* NewImports = new  Imports_Info();
		Syb.Info.reset(NewImports);
		_Table.AddSymbolID(Syb,ImportSymbolID);

		
		NewImports->NewAliases.resize(node._Imports.size());

		for (auto& Item : node._Imports)
		{
			

			for (auto& Item2 : Item._ImportedSybol._ScopedName)
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
					
			Name += Str_GetScopedNameAsString(Item._ImportedSybol);
			
			
			auto List = _Table.GetSymbolsWithName(Name);

			if (List.empty())
			{
				auto Token = Item._ImportedSybol._ScopedName.front()._token;
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
					ImportBindType SybType = SybolTypeToImportBindType(SybToBind->Type);

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
					auto Token = Item._ImportedSybol._ScopedName.front()._token;
					auto Sybol = List.front();
					LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos,
						"Cant Map Symbol '" + Sybol->FullName + "[" + ToString(Sybol->Type) + "]' to Alias");
				}
				else if (!IsOuterfile)
				{
					auto Token = Item._ImportedSybol._ScopedName.front()._token;
					auto Sybol = List.front();
					
					String V = "importing '" + Sybol->FullName + "' but it's Declared in this file.";
					LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos,V);
					
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
					auto Token = Item._ImportedSybol._ScopedName.front()._token;
					auto Sybol = List.front();

					String V = "importing '" + Sybol->FullName + "' but it's Declared in this file.";
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

			for (auto& ItemSybol : ImportInfo.NewSymbols)
			{
				if (ItemSybol.Type == ImportBindType::Func)//build func?
				{

				}
			}

			if (!ImportInfo.IsUsed)
			{
				auto Token = Item._ImportedSybol._ScopedName.front()._token;
				auto Name = Str_GetScopedNameAsString(Item._ImportedSybol);

				LogError(ErrorCodes::ExpectingSequence, Token->OnLine, Token->OnPos, "'" + Name + "' Import Symbol was not Used");
			}
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

void SystematicAnalysis::IR_WriteTo(IRInstruction* IR, const IROperator& Value)
{
	_IR_LookingAtIRBlock->NewStore((IRInstruction*)nullptr, IR);
	auto& Item = _IR_LookingAtIRBlock->Instructions.back();
	Item->Target() = Value;
}
void SystematicAnalysis::IR_WriteToDef(IRInstruction* IR, const IROperator& Value)
{
	_IR_LookingAtIRBlock->NewDereferenc_Store((IRInstruction*)nullptr, IR);
	auto& Item = _IR_LookingAtIRBlock->Instructions.back();
	Item->Target() = Value;
}
void SystematicAnalysis::IR_WriteTo(IRInstruction* IR, const TypeSymbol& Type, const IROperator& Value)
{
	if (Type.IsAddress())
	{
		IR_WriteToDef(IR, Value);
	}
	else
	{
		IR_WriteTo(IR, Value);
	}
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
		return _Symbol_SybToIRMap.at(ClassSybID);
	}
	const ClassInfo* clasinfo = Class.Get_Info < ClassInfo>();

	IRidentifierID V = _IR_Builder.ToID(Class.FullName);
	
	auto IRStuct = _IR_Builder.NewStruct(V);

	IRStuct->Fields.resize(clasinfo->Fields.size());

	for (size_t i = 0; i < clasinfo->Fields.size(); i++)
	{
		auto& Item = clasinfo->Fields[i];
		auto& Out = IRStuct->Fields[i];
		Out.Type = IR_ConvertToIRType(Item.Type);
	}

	_Symbol_SybToIRMap[ClassSybID] = V;
	return V;
}

IRidentifierID SystematicAnalysis::IR_Build_ConvertToStaticArray(const Symbol& Class)
{
	auto ClassSybID = Class.ID;
	if (_Symbol_SybToIRMap.HasValue(ClassSybID))
	{
		return _Symbol_SybToIRMap.at(ClassSybID);
	}
	const StaticArrayInfo* clasinfo = Class.Get_Info <StaticArrayInfo>();

	IRidentifierID V = _IR_Builder.ToID(Class.FullName);

	auto IRStuct = _IR_Builder.NewStaticArray(V,IR_ConvertToIRType(clasinfo->Type),clasinfo->Count);


	_Symbol_SybToIRMap[ClassSybID] = V;
	return V;
}

IRidentifierID SystematicAnalysis::IR_Build_ConveToIRVariantEnum(const Symbol& Enum)
{
	const EnumInfo* Info = Enum.Get_Info<EnumInfo>();
	
	if (Info->VariantData.has_value()) 
	{
		auto ClassSybID = Enum.ID;
		if (_Symbol_SybToIRMap.HasValue(ClassSybID))
		{
			return _Symbol_SybToIRMap.at(ClassSybID);
		}


		IRidentifierID V = _IR_Builder.ToID(Enum.FullName);
		auto IRStuct = _IR_Builder.NewStruct(V);


		String UnionName = Str_GetEnumVariantUnionName(Enum.FullName);

		IRidentifierID UnionID = _IR_Builder.ToID(UnionName);
		auto IRUnion= _IR_Builder.NewStruct(UnionID);
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
		_Symbol_SybToIRMap[ClassSybID] = UnionID;
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
			if (V->VariantData.has_value())
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
				r = IRType(_Symbol_SybToIRMap.at(syb.ID));
			}
			else
			{
				FuncPtrInfo* V = syb.Get_Info<FuncPtrInfo>();
				IRidentifierID IRid = _IR_Builder.ToID(syb.FullName);
				IRType r = IRType(IRid);
				auto tep = _IR_Builder.NewFuncPtr(_IR_Builder.ToID(syb.FullName), IR_ConvertToIRType(V->Ret));

				tep->Pars.resize(V->Pars.size());

				for (size_t i = 0; i < tep->Pars.size(); i++)
				{
					auto& Item = tep->Pars[i];
					Item = IR_ConvertToIRType(V->Pars[i]);
				}
				tep->Ret = IR_ConvertToIRType(V->Ret);

				_Symbol_SybToIRMap[syb.ID] = IRid;
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
				r = IRType(_Symbol_SybToIRMap.at(syb.ID));
			}
			else
			{
				String DynSybName = "Dynamic" + syb.FullName;

				IRidentifierID IRid = _IR_Builder.ToID(DynSybName);
				auto StructIR = _IR_Builder.NewStruct(IRid);

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

				r = IRType(IRid);
				_Symbol_SybToIRMap[syb.ID] = IRid;
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

	if (Value.IsAddress() || Value.IsAddressArray())
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
	#if UCodeLangDebug
	bool OnStack = false;

	

	constexpr size_t KbMul =  1024;
	constexpr size_t MbMul = KbMul * 1024;
	constexpr size_t StackSize = MbMul * 4;

	uintptr_t StackPointer = (uintptr_t)&OnStack;
	uintptr_t ItemInt = (uintptr_t)Item;
	if (StackPointer + StackSize < ItemInt 
	 && StackPointer - StackSize > ItemInt)
	{
		OnStack = true;
	}


	if (OnStack)
	{
		UCodeLangThrowException("Item Is on Stack not on heep");
	}
	#endif // DEBUG
	auto Scope = _Table._Scope.ThisScope;

	if (!_SybIdMap.HasValue(Scope))
	{
		_SybIdMap.AddValue(Scope, {});
	}
	BinaryVectorMap<const void*, SymbolID>& ID = _SybIdMap.at(Scope);

	if (!ID.HasValue(Item))
	{
		auto R = SymbolID(_IDIndex++);
		ID.AddValue(Item,R);
		return R;
	}
	else
	{
		return ID.at(Item);
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
	case NodeType::DeclareVariableNode:OnDeclareVariablenode(*DeclareVariableNode::As(&node2),DeclareStaticVariableNode_t::Stack); break;
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
	default:UCodeLangUnreachable(); break;
	}
	Pop_NodeScope();
}
void SystematicAnalysis::OnRetStatement(const RetStatementNode& node)
{
	
	auto& LookForT = Type_Get_LookingForType();
	if (node._Expression._Value)
	{
		_FuncStack.back().IsOnRetStatemnt = true;
		//LookForT.SetAsRawValue();

		_LookingForTypes.push(LookForT);
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

			if (!Type_CanBeImplicitConverted(_LastExpressionType, T,false))
			{
				LogError_CantCastImplicitTypes(_LastLookedAtToken.value(), _LastExpressionType, T, false);
			}
		}
	}
	else if (_PassType == PassType::BuidCode)
	{
		auto& T = Type_Get_LookingForType();
		IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, T);
		if (node._Expression._Value)
		{

			//Cant ret A Void Type In IR.
			if (!_FuncStack.back().Pointer->Ret.IsAn(TypesEnum::Void))
			{
				_IR_LookingAtIRBlock->NewRetValue(_IR_LastExpressionField);
			}
		}
	}

}
void SystematicAnalysis::OnEnum(const EnumNode& node)
{
	const bool IsgenericInstantiation = _Generic_GenericSymbolStack.size() && _Generic_GenericSymbolStack.top().NodeTarget == &node;
	const bool Isgeneric = node._generic._Values.size();
	const bool Isgeneric_t = Isgeneric && IsgenericInstantiation == false;


	
	const String ClassName = IsgenericInstantiation ? (String)ScopeHelper::GetNameFromFullName(_Generic_GenericSymbolStack.top()._IR_GenericFuncName) : (String)node._EnumName.token->Value._String;
	_Table.AddScope(ClassName);
	SymbolID SybID = Symbol_GetSymbolID(node);//Must be pass AddScope thats how GetSymbolID works.

	if (_PassType == PassType::GetTypes)
	{
		Symbol_RedefinitionCheck((String_view)_Table._Scope.ThisScope, SymbolType::Type_class,NeverNullptr(node._EnumName.token));
	}

	auto& Syb = _PassType == PassType::GetTypes ?
		Symbol_AddSymbol(Isgeneric_t ? SymbolType::Generic_Enum : SymbolType::Enum
			, (String)ClassName, _Table._Scope.ThisScope,node._Access) :
		*Symbol_GetSymbol(SybID);

	//we may jump to this node non linearly
	if (Syb.PassState == _PassType)
	{
		_Table.RemoveScope();
		return;
	}

	EnumInfo* ClassInf;
	if (_PassType == PassType::GetTypes)
	{
		_Table.AddSymbolID(Syb, SybID);
		Syb.NodePtr = node.As();


		ClassInf = new EnumInfo();
		ClassInf->Conext = Save_SymbolContextRemoveOneScopeName();
		ClassInf->FullName = Syb.FullName;
		Syb.Info.reset(ClassInf);
		Syb.VarType.SetType(Syb.ID);

		Generic_InitGenericalias(node._generic,IsgenericInstantiation, ClassInf->_GenericData);

	}
	else
	{
		ClassInf = (EnumInfo*)Syb.Get_Info<EnumInfo>();
	}
	//add  dependency cycle.
	EvaluatedEx ex;
	if (_PassType == PassType::FixedTypes)
	{
		Type_ConvertAndValidateType(node._BaseType, ClassInf->Basetype, NodeSyb_t::Any);
		if (ClassInf->Basetype.IsBadType() || Type_IsUnMapType(ClassInf->Basetype)) { _Table.RemoveScope(); return; }
		if (!Eavl_ConstantExpressionAbleType(ClassInf->Basetype))
		{
			LogError_TypeMustBeAnConstantExpressionAble(NeverNullptr(node._BaseType._name.token), ClassInf->Basetype);
		}
		ex = std::move(Eval_MakeEx(ClassInf->Basetype));
	}


	bool HasCheckedForincrementOp = false;
	_LookingForTypes.push(ClassInf->Basetype);

	for (size_t i = 0; i < node._Values.size(); i++)
	{
		auto& Item = node._Values[i];
	
		String_view ItemName = Item._Name.token->Value._String;

		if (_PassType == PassType::GetTypes)
		{
			ClassInf->AddField(ItemName);

			if (Item._Expression._Value)
			{
				OnExpressionTypeNode(Item._Expression._Value.get(),GetValueMode::Read);
			}


			_Table._Scope.AddScope(ItemName);
			{
				auto& EnumFeild = Symbol_AddSymbol(SymbolType::Enum_Field, (String)ItemName, _Table._Scope.ThisScope,AccessModifierType::Public);
				EnumFeild.NodePtr = &Item;
			}
			_Table._Scope.ReMoveScope();


			if (Item._VariantType)
			{
				if (!ClassInf->VariantData.has_value())
				{
					ClassInf->VariantData = EnumVariantData();
				}
			}
		}
		else if (_PassType == PassType::FixedTypes)
		{
			if (ClassInf->VariantData)
			{
				
				EnumVariantData& EnumVa = ClassInf->VariantData.value();
				
				
				if (Item._VariantType)
				{
					auto& VariantType_ = Item._VariantType.value();
					if (VariantType_._node && VariantType_._node->Get_Type() == NodeType::AnonymousTypeNode)
					{
						EnumVariantFeild V;

						{
							//
								_Table._Scope.AddScope(ItemName);
								auto NewName = Str_GetFuncAnonymousObjectFullName(_Table._Scope.ThisScope);
								_Table._Scope.ReMoveScope();
							//

							SymbolID AnonymousSybID = Symbol_GetSymbolID(VariantType_._node.get());
							auto& AnonymousSyb = Symbol_AddSymbol(SymbolType::Type_class, (String)NewName, NewName,AccessModifierType::Public);
							AnonymousSyb.OutputIR = Syb.Type == SymbolType::Enum;//Dont Output IR type if Generic
							AnonymousSyb.PassState = PassType::FixedTypes;
							_Table.AddSymbolID(AnonymousSyb, AnonymousSybID);


							auto ClassInf = new ClassInfo();
							ClassInf->FullName = NewName;
							AnonymousSyb.Info.reset(ClassInf);
							AnonymousSyb.VarType.SetType(AnonymousSyb.ID);

							AnonymousTypeNode* Typenode = AnonymousTypeNode::As(VariantType_._node.get());
							for (auto& Item3 : Typenode->_Fields._Parameters)
							{
								auto Fieldtype = Type_ConvertAndValidateType(Item3._Type, NodeSyb_t::Parameter);
								V.Types.push_back(Fieldtype);
								ClassInf->AddField(Item3._Name.AsString(), Fieldtype);
							}
							V.ClassSymbol = AnonymousSybID;
						} 

						EnumVa.Variants.push_back(std::move(V));
					}
					else
					{
						EnumVariantFeild V; 
						V.Types.push_back(Type_ConvertAndValidateType(VariantType_,NodeSyb_t::Parameter));
						
						EnumVa.Variants.push_back(std::move(V));
					}
				}
				else
				{
					EnumVariantFeild V;
					EnumVa.Variants.push_back(std::move(V));
				}

			}

			auto FieldOpt = ClassInf->GetField(ItemName);
			auto& Field = *FieldOpt.value();
			if (Item._Expression._Value)
			{
				auto& Type = ClassInf->Basetype;
				
				auto V = Eval_Evaluate(ex,Type, Item._Expression);

				Field.Ex = ex.EvaluatedObject;
			}
			else
			{
				if (i != 0)
				{
					auto& Type = ClassInf->Basetype;
					if (HasCheckedForincrementOp == false)
					{
						const NeverNullPtr<Token> LineDataToken = NeverNullptr(Item._Name.token);

						auto HasInfo = Type_HasPostfixOverLoadWith(Type, TokenType::increment);

						if (!HasInfo.HasValue)
						{

							Token temp;
							temp.OnLine = LineDataToken->OnLine;
							temp.OnPos = LineDataToken->OnPos;
							temp.Type = TokenType::increment;

							LogError_CantFindPostfixOpForTypes(&temp, Type);

							_Table.RemoveScope();
							return;
						}
						if (!Eval_HasConstantPostfixOperator(Type, TokenType::increment))
						{
							Token temp;
							temp.OnLine = LineDataToken->OnLine;
							temp.OnPos = LineDataToken->OnPos;
							temp.Type = TokenType::increment;


							LogError_Eval_CantDoPostfixOpForTypes(&temp, Type);

							_Table.RemoveScope();
							return;
						}
						HasCheckedForincrementOp = true;
					}

					Eval_EvaluatePostfixOperator(ex, TokenType::increment);
				}
				else
				{
					Eval_EvaluateDefaultConstructor(ex);
				}


				Field.Ex = ex.EvaluatedObject;
			}
		}

	}

	_LookingForTypes.pop();
	if (_PassType == PassType::BuidCode)
	{
		FileDependency_AddDependencyToCurrentFile(ClassInf->Basetype);
		if (Syb.Type == SymbolType::Enum) //Dont output type if Generic
		{
			Assembly_AddEnum(NeverNullptr(&Syb));
		}
	}

	_Table.RemoveScope();
	Syb.PassState = _PassType;
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
void SystematicAnalysis::OnDeclareVariablenode(const DeclareVariableNode& node, DeclareStaticVariableNode_t type)
{
	
	auto StrVarName = node._Name.AsString();
	auto FullName = _Table._Scope.GetApendedString(StrVarName);

	SymbolID sybId = Symbol_GetSymbolID(node);
	Symbol* syb;

	bool InSideClass = Context_InSideClass();
	bool IsField = InSideClass && DeclareStaticVariableNode_t::ClassField == type;

	if (_PassType == PassType::GetTypes)
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
		case DeclareStaticVariableNode_t::Eval:
			SysType = SymbolType::ConstantExpression;
			break;
		case DeclareStaticVariableNode_t::ClassField:
		{
			SysType = SymbolType::Class_Field;
			auto& Class = *_ClassStack.top().Info;

			Class.AddField(ScopeHelper::GetNameFromFullName((String_view)FullName), TypeSymbol());
		}
			break;
		default:
			SysType = SymbolType::StackVarable;
			break;
		}
		//
		Symbol_RedefinitionCheck(FullName, SysType,NeverNullptr(node._Name.token));


		syb = &Symbol_AddSymbol(SysType, StrVarName, FullName,node._Access);
		_Table.AddSymbolID(*syb, sybId);

		syb->Set_NodeInfo(node.As());
		if (syb->Type == SymbolType::ConstantExpression)
		{
			ConstantExpressionInfo* info = new ConstantExpressionInfo();
			info->Exnode = &node._Expression;
			info->Conext = Save_SymbolContext();
			syb->Info.reset(info);
		}
		else
		{
			DeclareVariableInfo* info = new DeclareVariableInfo();
			info->LineNumber = node._Name.token->OnPos;
			info->Conext = Save_SymbolContext();
			syb->Info.reset(info);
		}

		//
		if (type == DeclareStaticVariableNode_t::Static
			|| type == DeclareStaticVariableNode_t::Thread
			|| type == DeclareStaticVariableNode_t::Eval)
		{
			if (!node._Expression._Value)
			{
				auto Token = node._Name.token;
				String VarType;
				switch (type)
				{
				case DeclareStaticVariableNode_t::Static:
					VarType = "static";
					break;
				case DeclareStaticVariableNode_t::Thread:
					VarType = "thread";
					break;
				case DeclareStaticVariableNode_t::Eval:
					VarType = "eval";
					break;
				default:
					UCodeLangUnreachable();
					break;
				}

				LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos, VarType + " Varable must be assigned.missing '='.");
			}
		}

		//
		if (node._Expression._Value)
		{
			OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);
		}
	}
	else
	{
		syb = Symbol_GetSymbol(sybId).value();
	}

	//we may jump to this node non linearly
	if (syb->PassState == _PassType)
	{
		return;
	}

	if (_PassType == PassType::FixedTypes)
	{
		auto& VarType = syb->VarType;
		Type_ConvertAndValidateType(node._Type, VarType, NodeSyb_t::ClassFeild);
		VarType.SetAsLocation();


		auto Ex = node._Expression._Value.get();
		Type_DeclareVarableCheck(VarType, Ex, NeverNullptr(node._Name.token));

		if (VarType.IsTypeInfo() && type != DeclareStaticVariableNode_t::Eval)
		{
			auto Token = NeverNullptr(node._Name.token);
			LogError_UseingTypeinfoInNonEvalVarable(Token);
		}

		if (VarType.IsAn(TypesEnum::Void))
		{
			auto Token = NeverNullptr(node._Name.token);
			LogError_CantUseTypeVoidHere(Token);
		}
	}
	_LookingForTypes.push(syb->VarType);

	IRInstruction* OnVarable{};
	bool IsStructObjectPassRef =false;
	
	IRFunc* oldIRFunc{};
	IRBlock* oldblock{};
	
	if (_PassType == PassType::BuidCode) 
	{
		if (node._Expression._Value)
		{
			if (syb->Type != SymbolType::ConstantExpression)
			{



				if (syb->Type == SymbolType::StackVarable)
				{
					OnVarable = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(syb->VarType));

					Debug_Add_SetLineNumber(NeverNullptr(node._Name.token), _IR_LookingAtIRBlock->GetIndex());
					Debug_Add_SetVarableInfo(*syb, _IR_LookingAtIRBlock->GetIndex());
					syb->IR_Ins = OnVarable;


				}
				else if (syb->Type == SymbolType::StaticVarable)
				{
					oldIRFunc = _IR_LookingAtIRFunc;
					oldblock = _IR_LookingAtIRBlock;

					

					if (_IR_Builder._StaticInit.Blocks.size() == 0)
					{
						_IR_Builder._StaticInit.NewBlock(".");
					}

					_IR_LookingAtIRFunc = &_IR_Builder._StaticInit;
					_IR_LookingAtIRBlock = _IR_LookingAtIRFunc->Blocks.front().get();

					Debug_Add_SetLineNumber(NeverNullptr(node._Name.token), _IR_LookingAtIRBlock->GetIndex());


					Debug_Add_SetVarableInfo(*syb, _IR_LookingAtIRBlock->GetIndex());
					if (Type_IsStructPassByRef(syb)) {
						OnVarable = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(syb->VarType));
					}
				}
				else if (syb->Type == SymbolType::ThreadVarable)
				{
					oldIRFunc = _IR_LookingAtIRFunc;
					oldblock = _IR_LookingAtIRBlock;

					if (_IR_Builder._threadInit.Blocks.size() == 0)
					{
						_IR_Builder._threadInit.NewBlock(".");
					}


					_IR_LookingAtIRFunc = &_IR_Builder._threadInit;
					_IR_LookingAtIRBlock = _IR_LookingAtIRFunc->Blocks.front().get();

					Debug_Add_SetLineNumber(NeverNullptr(node._Name.token), _IR_LookingAtIRBlock->GetIndex());
					Debug_Add_SetVarableInfo(*syb, _IR_LookingAtIRBlock->GetIndex());

					if (Type_IsStructPassByRef(syb)) {
						OnVarable = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(syb->VarType));
					}
				}
				else if (syb->Type == SymbolType::Class_Field)
				{
					oldIRFunc = _IR_LookingAtIRFunc;
					oldblock = _IR_LookingAtIRBlock;

					auto* Classinfo = _ClassStack.top().Info;
					if (Classinfo->_ClassFieldInit == nullptr)
					{
						String funcName = _Table._Scope.GetApendedString((String)ClassInitializefuncName);


						Classinfo->_ClassFieldInit = _IR_Builder.NewFunc(funcName, IRTypes::Void);
						Classinfo->_ClassFieldInit->NewBlock(".");
						{
							auto Classtype = TypeSymbol(_ClassStack.top().Syb->ID);
							Classtype._IsAddress = true;

							IRPar ThisPar;
							ThisPar.identifier = _IR_Builder.ToID(ThisSymbolName);
							ThisPar.type = IR_ConvertToIRType(Classtype);
							Classinfo->_ClassFieldInit->Pars.push_back(ThisPar);
						}
					}

					_IR_LookingAtIRFunc = Classinfo->_ClassFieldInit;
					_IR_LookingAtIRBlock = _IR_LookingAtIRFunc->Blocks.front().get();

					Debug_Add_SetLineNumber(NeverNullptr(node._Name.token), _IR_LookingAtIRBlock->GetIndex());

					if (Type_IsStructPassByRef(syb)) {
						OnVarable = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(syb->VarType));
					}
				}
				else
				{
					UCodeLangUnreachable();
				}

				IsStructObjectPassRef = Type_IsStructPassByRef(syb);

				if (IsStructObjectPassRef)
				{
					_IR_IRlocations.push({ OnVarable ,false });
				}

				OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);
			}

		}
		else
		{
			if (syb->Type == SymbolType::StackVarable)
			{
				Debug_Add_SetLineNumber(NeverNullptr(node._Name.token), _IR_LookingAtIRBlock->GetIndex());

				OnVarable = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(syb->VarType));
				syb->IR_Ins = OnVarable;
			}


		}
	}

	if (_PassType == PassType::FixedTypes)
	{
		syb = Symbol_GetSymbol(sybId).value();
		if (node._Expression._Value)
		{
			OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);

			syb->SetTovalid();

			auto& VarType = syb->VarType;


			auto& Ex = _LastExpressionType;
			auto Token = NeverNullptr(node._Type._name.token);
			Type_DeclareVariableTypeCheck(VarType, Ex, Token);

			if (syb->Type == SymbolType::ConstantExpression && !VarType.IsNull())
			{
				ConstantExpressionInfo* Info = syb->Get_Info<ConstantExpressionInfo>();

				_LookingForTypes.push(VarType);
				auto Item = Eval_EvaluateToAnyType(node._Expression);
				_LookingForTypes.pop();

				if (Item) 
				{
					Info->Ex =std::move(Item.value().EvaluatedObject);
				}
			}
		}
		else
		{
			if (!IsField) {
				syb->SetToInvalid();
			}
		}


		if (IsField)
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

				Class.Size += Type_GetSize(Item->Type).value_or(0);
				
				if (node._Expression._Value) 
				{
					Class._WillHaveFieldInit= true;
				}
				if (Symbol_HasDestructor(Item->Type))
				{
					Class._WillHaveFielddeInit = true;
				}
			}

			

		}
	}
	_LookingForTypes.pop();

	if (_PassType == PassType::BuidCode && node._Expression._Value)
	{
		if (syb->Type != SymbolType::ConstantExpression) 
		{
			Debug_Add_SetLineNumber(NeverNullptr(node._Name.token), _IR_LookingAtIRBlock->GetIndex());


			IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, syb->VarType);


			IR_Build_OnStoreVarable(IsStructObjectPassRef, OnVarable, syb, sybId);

			FileDependency_AddDependencyToCurrentFile(syb->VarType);


			//
			if (syb->Type == SymbolType::StaticVarable)
			{
				_IR_LookingAtIRFunc = oldIRFunc;
				_IR_LookingAtIRBlock = oldblock;
			}
			else if (syb->Type == SymbolType::ThreadVarable)
			{
				_IR_LookingAtIRFunc = oldIRFunc;
				_IR_LookingAtIRBlock = oldblock;
			}
			else if (syb->Type == SymbolType::Class_Field)
			{
				_IR_LookingAtIRFunc = oldIRFunc;
				_IR_LookingAtIRBlock = oldblock;
			}

		}
	}

	syb->PassState = _PassType;
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


			auto Varable = _IR_LookingAtIRBlock->NewLoadPtr(id);

			ObjectToDrop V;
			V.ID = sybId;
			V._Object = Varable;
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
		auto IndexFeild = GG->GetFieldIndex(ScopeHelper::GetNameFromFullName(syb->FullName)).value();

		IRStruct* V = _IR_Builder.GetSymbol(IR_Build_ConvertToIRClassIR(*classSb))->Get_ExAs<IRStruct>();
		auto output = _IR_LookingAtIRBlock->New_Member_Dereference(&_IR_LookingAtIRFunc->Pars[0], IR_ConvertToIRType(classSb->ID), IndexFeild);

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
			if (WasIsAddressArry) { VarType.SetAsAddressArray(); }

			if (OldTypeInfo == TypeInfoPrimitive::Null)
			{
				VarType._TypeInfo = OldTypeInfo;
			}
			VarType.SetAsLocation();
		}
	}
	
	if (!Type_CanBeImplicitConverted(Ex, VarType, false))
	{
		LogError_CantCastImplicitTypes(Token, Ex, VarType, false);
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
void SystematicAnalysis::OnAssignExpressionNode(const AssignExpressionNode& node)
{
	if (_PassType == PassType::GetTypes)
	{
		OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);
		OnExpressionTypeNode(node._ToAssign._Value.get(), GetValueMode::Write);
	}
	else if (_PassType == PassType::FixedTypes)
	{
		_LookingForTypes.push(TypesEnum::Var);
		OnExpressionTypeNode(node._ToAssign._Value.get(), GetValueMode::Write);
		_LookingForTypes.pop();

		auto AssignType = _LastExpressionType;

		_LookingForTypes.push(AssignType);
		OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);
		_LookingForTypes.pop();


		auto ExpressionType = _LastExpressionType;

		

		
		if (!Type_CanBeImplicitConverted(AssignType, ExpressionType,false))
		{
			auto  Token = _LastLookedAtToken;
			LogError_CantCastImplicitTypes(Token.value(),AssignType, ExpressionType, false);

		}
		auto ID = Symbol_GetSymbolID(node);

		AssignExpression_Data Data;
		Data.Op0 = ExpressionType;
		Data.Op1 = AssignType;

		_AssignExpressionDatas.AddValue(ID, Data);
	}
	else if (_PassType == PassType::BuidCode)
	{

		auto ID = Symbol_GetSymbolID(node);
		auto& AssignType = _AssignExpressionDatas.at(ID);

		Debug_Add_SetLineNumber(NeverNullptr(node._Token), _IR_LookingAtIRBlock->Instructions.size());

		_LookingForTypes.push(AssignType.Op0);
		OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);
		_LookingForTypes.pop();

		auto ExpressionType = _LastExpressionType;
		auto ExIR = _IR_LastExpressionField;

		IR_Build_ImplicitConversion(ExIR, ExpressionType, AssignType.Op1);
		ExIR = _IR_LastExpressionField;


		_LookingForTypes.push(AssignType.Op1);
		OnExpressionTypeNode(node._ToAssign._Value.get(), GetValueMode::Write);
		_LookingForTypes.pop();

		auto AssignIR = _IR_LastExpressionField;

		if (node._ReassignAddress)
		{
			IR_WriteTo(ExIR,_IR_LastStoreField);
		}
		else 
		{
			IR_WriteTo(ExIR, AssignType.Op1, _IR_LastStoreField);
		}
	}
}
void SystematicAnalysis::OnIfNode(const IfNode& node)
{
	
	TypeSymbol BoolType(TypesEnum::Bool);

	String ScopeName = std::to_string((size_t)&node);

	Push_NewStackFrame();
	_Table.AddScope(ScopeName);
	
	_LookingForTypes.push(BoolType);


	

	OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);


	if (_PassType == PassType::FixedTypes)
	{
		if (!Type_CanBeImplicitConverted(_LastExpressionType, BoolType))
		{
			auto  Token = _LastLookedAtToken;
			LogError_CantCastImplicitTypes(Token.value(), _LastExpressionType, BoolType, true);
		}
	}

	
	IRBlock::NewConditionalFalseJump_t IfIndex{};
	IRInstruction* BoolCode{};
	if (_PassType == PassType::BuidCode)
	{
		IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, BoolType);
		BoolCode = _IR_LastExpressionField;
		IfIndex = _IR_LookingAtIRBlock->NewConditionalFalseJump(BoolCode);
		BoolCode = IfIndex.logicalNot;
	}
	
	
	

	_LookingForTypes.pop();

	for (const auto& node2 :node._Body._Nodes)
	{
		OnStatement(*node2);
	}
	
	Pop_StackFrame();
	_Table.RemoveScope();
	
	if (node._Else)
	{
		IRInstruction* ElseIndex{};
		size_t ElseI{};
		if (_PassType == PassType::BuidCode)
		{
			ElseIndex = _IR_LookingAtIRBlock->NewJump();
			ElseI = _IR_LookingAtIRBlock->GetIndex();
		}


		if (node._Else->Get_Type() != NodeType::ElseNode)
		{
			OnStatement(*node._Else);
		}
		else
		{
			ElseNode* Elsenode = ElseNode::As(node._Else.get());

			for (const auto& node3 : Elsenode->_Body._Nodes)
			{
				OnStatement(*node3);
			}
		}


		if (_PassType == PassType::BuidCode)
		{
			auto JumpIndex = _IR_LookingAtIRBlock->GetIndex() ;
			_IR_LookingAtIRBlock->UpdateJump(ElseIndex, JumpIndex);
			_IR_LookingAtIRBlock->UpdateConditionaJump(IfIndex.ConditionalJump, IfIndex.logicalNot, ElseI);
		}
	}
	else if (_PassType == PassType::BuidCode)
	{
		_IR_LookingAtIRBlock->UpdateConditionaJump(IfIndex.ConditionalJump, BoolCode, _IR_LookingAtIRBlock->GetIndex());
	}
	
	
}
void SystematicAnalysis::OnWhileNode(const WhileNode& node)
{
	TypeSymbol BoolType(TypesEnum::Bool);

	String ScopeName = std::to_string((size_t)&node);

	Push_NewStackFrame();
	_Table.AddScope(ScopeName);

	_LookingForTypes.push(BoolType);


	size_t BoolCode;
	size_t BoolJumps;
	if (_PassType == PassType::BuidCode)
	{
		BoolCode = _IR_LookingAtIRBlock->GetIndex();
		BoolJumps = IR_GetJumpsIndex();
	}
	OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);

	if (_PassType == PassType::FixedTypes)
	{
		if (!Type_CanBeImplicitConverted(_LastExpressionType, BoolType))
		{
			auto  Token = _LastLookedAtToken;
			LogError_CantCastImplicitTypes(Token.value(), _LastExpressionType, BoolType,true);
		}
	}


	IRBlock::NewConditionalFalseJump_t IfIndex;
	IRInstruction* BoolCode2;
	if (_PassType == PassType::BuidCode)
	{
		IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, BoolType);
		BoolCode2 = _IR_LastExpressionField;
		IfIndex = _IR_LookingAtIRBlock->NewConditionalFalseJump(BoolCode2);

	}




	_LookingForTypes.pop();

	for (const auto& node2 : node._Body._Nodes)
	{
		OnStatement(*node2);
	}


	if (_PassType == PassType::BuidCode)
	{
		_IR_LookingAtIRBlock->NewJump(BoolCode);

		size_t BreakCode = _IR_LookingAtIRBlock->GetIndex();

		_IR_LookingAtIRBlock->UpdateConditionaJump(IfIndex.ConditionalJump, IfIndex.logicalNot, BreakCode);

		IR_Build_UpdateJumpsBreakContiunes(BoolJumps,BoolCode, BreakCode);
	}


	_Table.RemoveScope();

	Pop_StackFrame();
}
void SystematicAnalysis::OnDoNode(const DoNode& node)
{
	String ScopeName = std::to_string((size_t)&node);

	Push_NewStackFrame();
	_Table.AddScope(ScopeName);


	size_t StartIndex;
	size_t JumpIndex;
	if (_PassType == PassType::BuidCode)
	{
		StartIndex= _IR_LookingAtIRBlock->GetIndex();
		JumpIndex = IR_GetJumpsIndex();
	}


	for (const auto& node2 : node._Body._Nodes)
	{
		
		OnStatement(*node2);

	}

	_Table.RemoveScope();
	Pop_StackFrame();

	TypeSymbol BoolType(TypesEnum::Bool);
	_LookingForTypes.push(BoolType);

	size_t boolCode;
	if (_PassType == PassType::BuidCode)
	{
		boolCode = _IR_LookingAtIRBlock->GetIndex();
	}


	OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);

	if (_PassType == PassType::FixedTypes)
	{
		if (!Type_CanBeImplicitConverted(_LastExpressionType, BoolType))
		{
			auto  Token = _LastLookedAtToken;
			LogError_CantCastImplicitTypes(Token.value(), _LastExpressionType, BoolType,true);
		}
	}

	
	if (_PassType == PassType::BuidCode)
	{
		IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, BoolType);

		size_t BreakCode = 0;

		_IR_LookingAtIRBlock->NewConditionalFalseJump(_IR_LastExpressionField, StartIndex);


		IR_Build_UpdateJumpsBreakContiunes(JumpIndex,StartIndex, BreakCode);
	}
	



	_LookingForTypes.pop();
}
void SystematicAnalysis::OnDeclareStaticVariableNode(const DeclareStaticVariableNode& node)
{
	OnDeclareVariablenode(node._Variable, DeclareStaticVariableNode_t::Static);
}
void SystematicAnalysis::OnDeclareThreadVariableNode(const DeclareThreadVariableNode& node)
{
	OnDeclareVariablenode(node._Variable, DeclareStaticVariableNode_t::Thread);
}
void SystematicAnalysis::OnDeclareEvalVariableNode(const DeclareEvalVariableNode& node)
{
	OnDeclareVariablenode(node._Variable, DeclareStaticVariableNode_t::Eval);
}
void SystematicAnalysis::IR_Build_Member_Store(const GetMemberTypeSymbolFromVar_t& In, IRInstruction* Value)
{
	_IR_LookingAtIRBlock->NewStore(IR_Build_Member_Store(In), Value);
}
IROperator  SystematicAnalysis::IR_Build_Member_Store(const GetMemberTypeSymbolFromVar_t& In)
{

	IRInstruction* Output = nullptr;

	BuildMember_Access(In, Output);
	if (Output == nullptr)
	{
		Output = In._Symbol->IR_Ins;
	}
	bool UseOutput = In._Symbol->IR_Ins != Output;
	
	//


	switch (In._Symbol->Type)
	{
	case  SymbolType::Type_class://this
	case  SymbolType::Class_Field:
	case  SymbolType::StackVarable:
		return Output;
	case  SymbolType::ParameterVarable:
		return UseOutput ? IROperator(Output) : IROperator(In._Symbol->IR_Par);
		break;
	case SymbolType::StaticVarable:
	case SymbolType::ThreadVarable:
		return UseOutput ? IROperator(Output) : IROperator(_IR_Builder.ToID(In._Symbol->FullName));
		break;
	default:
		UCodeLangUnreachable();
		break;
	}
}
IROperator  SystematicAnalysis::IR_Build_Member_DereferencStore(const GetMemberTypeSymbolFromVar_t& In)
{
	IRInstruction* Output = nullptr;

	BuildMember_Access(In, Output);
	if (Output == nullptr)
	{
		Output = In._Symbol->IR_Ins;
	}
	bool UseOutput = In._Symbol->IR_Ins != Output;

	switch (In._Symbol->Type)
	{
	case  SymbolType::Type_class://this
	case  SymbolType::Class_Field:
	case  SymbolType::StackVarable:
		return  Output;
	case  SymbolType::ParameterVarable:
		return UseOutput ? IROperator(IROperatorType::DereferenceOf_IRInstruction,Output) : IROperator(IROperatorType::DereferenceOf_IRParameter,In._Symbol->IR_Par);
		break;
	case  SymbolType::StaticVarable:
	case  SymbolType::ThreadVarable:
		return UseOutput ? IROperator(IROperatorType::DereferenceOf_IRInstruction, Output) : IROperator(IROperatorType::DereferenceOf_IRParameter, _IR_Builder.ToID(In._Symbol->FullName));
		break;
	default:
		UCodeLangUnreachable();
		break;
	}
}
IROperator SystematicAnalysis::IR_Build_Member_Store(const GetMemberTypeSymbolFromVar_t& In, const TypeSymbol& Type)
{
	if (Type.IsAddress())
	{
		return IR_Build_Member_DereferencStore(In);
	}
	else
	{
		return IR_Build_Member_Store(In);
	}
}

IRInstruction* SystematicAnalysis::IR_Build_Member_GetPointer(const GetMemberTypeSymbolFromVar_t& In)
{
	IRInstruction* Output = nullptr;

	BuildMember_Access(In, Output);
	if (Output == nullptr)
	{
		Output = In._Symbol->IR_Ins;
	}
	bool UseOutput = In._Symbol->IR_Ins != Output;

	Debug_Add_SetLineNumber(NeverNullptr(In.Start->_token), _IR_LookingAtIRBlock->Instructions.size());
	switch (In._Symbol->Type)
	{
	case  SymbolType::Type_class://this
	case  SymbolType::Class_Field:
	case  SymbolType::StackVarable:
		return _IR_LookingAtIRBlock->NewLoadPtr(Output);
		break;
	case  SymbolType::ParameterVarable:
		return UseOutput ? _IR_LookingAtIRBlock->NewLoadPtr(Output): _IR_LookingAtIRBlock->NewLoadPtr(In._Symbol->IR_Par);
		break;
	case  SymbolType::StaticVarable:
	case  SymbolType::ThreadVarable:
		return UseOutput ? _IR_LookingAtIRBlock->NewLoadPtr(Output) : _IR_LookingAtIRBlock->NewLoadPtr(_IR_Builder.ToID(In._Symbol->FullName));
	default:
		UCodeLangUnreachable();
		break;
	}
}
bool SystematicAnalysis::Symbol_StepGetMemberTypeSymbolFromVar(const ScopedNameNode& node, size_t Index, ScopedName::Operator_t OpType, GetMemberTypeSymbolFromVar_t& Out)
{
	auto& Item = node._ScopedName[Index];


	auto ItemToken =NeverNullptr(Item._token);
	_LastLookedAtToken = ItemToken.AsNullable();


	String_view ItemTokenString;
	
	if (ItemToken->Type == TokenType::Class)
	{
		if (_PassType == PassType::FixedTypes) 
		{
			VarableMemberData Data;
			ExpressionNodeType TepEx;
			ValueExpressionNode TepExValue;
			ReadVariableNode TepExName;
			ScopedName ScopeN;
			Token TepToken = *ItemToken;
			TepToken.Type = TokenType::Name;


			ScopeN._token = &TepToken;
			TepExName._VariableName._ScopedName.push_back(ScopeN);

			TepExValue._Value.reset(&TepExName);
			TepEx._Value.reset(&TepExValue);


			OnExpressionTypeNode(TepEx, GetValueMode::Read);

			auto Type = _LastExpressionType;

			TypeSymbol StackCharArr;
			StackCharArr.SetType(TypesEnum::Char);



			bool IsCharArr = Type_IsStaticCharArr(Type);


			if (IsCharArr)
			{
				auto V = Eval_Evaluate(Type, TepEx);

				{//stop double free 
					TepEx._Value.release();
					TepExValue._Value.release();
				}

				if (V.has_value())
				{
					auto BuffData = V.value();
					char* Buffer = (char*)Eval_Get_Object(BuffData);


					Data.MemberString.resize(BuffData.EvaluatedObject.ObjectSize);
					memcpy(Data.MemberString.data(), BuffData.EvaluatedObject.Object_AsPointer.get(), BuffData.EvaluatedObject.ObjectSize);

					
					_VarableMemberDatas.AddValue(Symbol_GetSymbolID(&Item), std::move(Data));
					ItemTokenString = _VarableMemberDatas.at(Symbol_GetSymbolID(&Item)).MemberString;
				}
				else
				{
					return false;
				}
			}
			else
			{

				{//stop double free 
					TepEx._Value.release();
					TepExValue._Value.release();
				}

				auto Token = Item._token;
				LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos, "Type must be a char[/] and not a '" + ToString(Type) + "' to be used as a identfier.");
				return false;
			}
		}
		else if (_PassType == PassType::BuidCode)
		{
			auto Data = _VarableMemberDatas.at(Symbol_GetSymbolID(&Item));
			ItemTokenString = Data.MemberString;
		}

	}
	else
	{
		ItemTokenString = ItemToken->Value._String;
	}

	if (Out.Type._Type != TypesEnum::CustomType
		|| Out._Symbol == nullptr)
	{
		if (_PassType == PassType::FixedTypes)
		{
			LogError_CantFindVarMemberError(ItemToken, ItemTokenString, Out.Type);
		}
		return false;
	}

	if (Out._Symbol->PassState == PassType::GetTypes)
	{
		Symbol_Update_Sym_ToFixedTypes(Out._Symbol);
	}

	if (OpType == ScopedName::Operator_t::Null
		|| OpType == ScopedName::Operator_t::ScopeResolution
		|| OpType == ScopedName::Operator_t::Dot)
	{

		if (Out._Symbol->Type == SymbolType::Type_class
			 || Out._Symbol->Type == SymbolType::Generic_class)
		{
			ClassInfo* CInfo = Out._Symbol->Get_Info<ClassInfo>();

			if (OpType != ScopedName::Operator_t::ScopeResolution) 
			{
				
				auto FeldInfo = CInfo->GetField(ItemTokenString);
				if (!FeldInfo.has_value())
				{
					if (_PassType == PassType::FixedTypes)
					{
						auto SymFullName = Out._Symbol->FullName;
						ScopeHelper::GetApendedString(SymFullName, ItemTokenString);
						auto SybOp = Symbol_GetSymbol(SymFullName, SymbolType::Type);
						if (SybOp)
						{
							auto Sym = SybOp.value();
							LogError_UseScopeResolutionAndNotDot(Sym, ItemToken);
						}
						else 
						{
							LogError_CantFindVarMemberError(ItemToken, ItemTokenString, Out.Type);
						}
					}
					Out._Symbol = nullptr;
					Out.Type = TypesEnum::Null;
					return false;
				}

				auto FeldFullName = Out._Symbol->FullName;
				ScopeHelper::GetApendedString(FeldFullName, ItemTokenString);
				auto FeldSyb = Symbol_GetSymbol(FeldFullName, SymbolType::Class_Field).value();
				{
					Symbol_AccessCheck(FeldSyb, ItemToken);
				}


				auto& FieldType2 = (*FeldInfo)->Type;
				if (FieldType2._Type == TypesEnum::CustomType)
				{
					Out._Symbol = Symbol_GetSymbol(FieldType2._CustomTypeSymbol).value();
					Out.Type = FieldType2;
				}
				else
				{
					Out.Type = FieldType2;

					if (Index + 1 < node._ScopedName.size())
					{
						const NeverNullPtr<Token> Token = NeverNullptr(node._ScopedName.begin()->_token);

						auto Token2 = NeverNullptr(node._ScopedName[Index + 1]._token);
						auto& Str2 = Token->Value._String;
						if (_PassType == PassType::FixedTypes)
						{
							LogError_CantFindVarMemberError(Token2, Str2, Out.Type);
						}

						Out._Symbol = nullptr;
						Out.Type = TypesEnum::Null;
						return false;
					}
				}
				if (_PassType == PassType::BuidCode)
				{
					FileDependency_AddDependencyToCurrentFile(Out._Symbol);
				}
			}
			else
			{
				auto SymFullName = Out._Symbol->FullName;
				ScopeHelper::GetApendedString(SymFullName, ItemTokenString);
				auto FeldSybOp = Symbol_GetSymbol(SymFullName, SymbolType::Type);
				if (FeldSybOp)
				{
					auto FeldSyb = FeldSybOp.value();
					
					if (FeldSyb->Type == SymbolType::Class_Field)
					{
						LogError_UseDotAndNotScopeResolution(FeldSyb, ItemToken);
						Out._Symbol = nullptr;
						Out.Type = TypesEnum::Null;
						return false;
					}
					else
					{
						Out._Symbol = FeldSyb.value();
						Out.Type = FeldSyb->VarType;
					}
				}
				else
				{
					LogError_CantFindSymbolError(ItemToken, SymFullName);
					Out._Symbol = nullptr;
					Out.Type = TypesEnum::Null;
					return false;
				}
			}
		}
		else if (Out._Symbol->Type == SymbolType::Enum)
		{
			if (OpType == ScopedName::Operator_t::ScopeResolution)
			{
				{
					const NeverNullPtr<Token> Token = NeverNullptr(node._ScopedName.begin()->_token);
					Symbol_AccessCheck(Out._Symbol, Token);
				}

				EnumInfo* Einfo = Out._Symbol->Get_Info<EnumInfo>();
				auto& NameString = ItemTokenString;

				auto FeldInfo = Einfo->GetFieldIndex(NameString);
				if (!FeldInfo.has_value())
				{
					if (_PassType == PassType::FixedTypes)
					{
						LogError_CantFindVarMemberError(ItemToken, ItemTokenString, Out.Type);
					}

					Out._Symbol = nullptr;
					Out.Type = TypesEnum::Null;
					return false;
				}
				else if (Einfo->VariantData)
				{
					auto& Item = Einfo->VariantData.value().Variants[FeldInfo.value()];

					bool IsOk = Item.Types.size() == 0;

					if (!IsOk)
					{

						LogError_MustMakeEnumLikeafuncion(Einfo, FeldInfo.value(), ItemToken);


						Out._Symbol = nullptr;
						Out.Type = TypesEnum::Null;
						return false;
					}
				}

				if (Index + 1 < node._ScopedName.size())
				{
					const NeverNullPtr<Token> Token = NeverNullptr(node._ScopedName.begin()->_token);

					auto Token2 = NeverNullptr(node._ScopedName[Index + 1]._token);
					auto& Str2 = Token->Value._String;
					if (_PassType == PassType::FixedTypes)
					{
						LogError_CantFindVarMemberError(Token2, Str2, Out.Type);
					}

					Out._Symbol = nullptr;
					Out.Type = TypesEnum::Null;
					return false;
				}


				Out.Type.SetType(Out._Symbol->ID);//set enum type

				{
					String FeildSymFullName = Out._Symbol->FullName;
					ScopeHelper::GetApendedString(FeildSymFullName, NameString);

					Symbol* FeildSym = Symbol_GetSymbol(FeildSymFullName, SymbolType::Enum_Field).value().value();

					Out._Symbol = FeildSym;//set symbol as enum feild


					Out.Set_V1(&Einfo->Fields[*FeldInfo]);
					Out.Set_V2(Einfo);

				}

				if (_PassType == PassType::BuidCode)
				{
					FileDependency_AddDependencyToCurrentFile(Out._Symbol);
				}
			}
			else
			{
				auto SymFullName = Out._Symbol->FullName;
				ScopeHelper::GetApendedString(SymFullName, ItemTokenString);
				auto FeldSybOp = Symbol_GetSymbol(SymFullName, SymbolType::Type);
				if (FeldSybOp)
				{
					auto FeldSyb = FeldSybOp.value();


					LogError_UseDotAndNotScopeResolution(FeldSyb, ItemToken);
				}
				else
				{
					LogError_CantFindVarMemberError(ItemToken, ItemTokenString, Out.Type);

				}
				Out._Symbol = nullptr;
				Out.Type = TypesEnum::Null;
				return false;
			}
		}
		else if (Out._Symbol->Type == SymbolType::Func)
		{
			if (Index + 1 < node._ScopedName.size())
			{
				const NeverNullPtr<Token> Token = NeverNullptr(node._ScopedName.begin()->_token);

				auto Token2 = NeverNullptr(node._ScopedName[Index + 1]._token);
				auto& Str2 = Token->Value._String;
				if (_PassType == PassType::FixedTypes)
				{
					LogError_CantFindVarMemberError(Token2, Str2, Out.Type);
				}

				Out._Symbol = nullptr;
				Out.Type = TypesEnum::Null;
				return false;
			}


			Symbol* Func = Out._Symbol;

			FuncInfo* Finfo = Out._Symbol->Get_Info<FuncInfo>();
			String TepFuncPtr = GetTepFuncPtrName(Finfo);

			NeverNullPtr<Symbol> V = GetTepFuncPtrSyb(TepFuncPtr, Finfo).value();
			Out._Symbol = V.value();
			Out.Type.SetType(V->ID);


			if (_PassType == PassType::BuidCode)
			{
				FileDependency_AddDependencyToCurrentFile(Func);
			}

		}
		else if (Symbol_IsVarableType(Out._Symbol->Type))
		{
			TypeSymbol VarableType = Out._Symbol->VarType;
			NullablePtr<Symbol> TypeAsSybolOp = Symbol_GetSymbol(VarableType);
			if (TypeAsSybolOp)
			{
				auto TypeAsSybol = TypeAsSybolOp.value();
				if (Out._Symbol->Type == SymbolType::Class_Field)
				{
					const NeverNullPtr<Token> token = NeverNullptr(node._ScopedName.begin()->_token);
					

					Symbol_AccessCheck(Out._Symbol, token);
				}

				if (TypeAsSybol->Type != SymbolType::Type_class)
				{
					LogError_CantFindVarMemberError(ItemToken, ItemTokenString, Out.Type);

					Out._Symbol = nullptr;
					Out.Type = TypesEnum::Null;
					return false;
				}
				ClassInfo* CInfo = TypeAsSybol->Get_Info<ClassInfo>();


				auto FeldInfo = CInfo->GetField(ItemTokenString);
				if (!FeldInfo.has_value())
				{
					if (_PassType == PassType::FixedTypes)
					{
						LogError_CantFindVarMemberError(ItemToken, ItemTokenString, Out.Type);
					}

					Out._Symbol = nullptr;
					Out.Type = TypesEnum::Null;
					return false;
				}

				{
					auto FeldFullName = TypeAsSybol->FullName;
					ScopeHelper::GetApendedString(FeldFullName, ItemTokenString);
					auto FeldSyb = Symbol_GetSymbol(FeldFullName, SymbolType::Class_Field).value();
					{
						Symbol_AccessCheck(FeldSyb,ItemToken);
					}
				}

				auto& FieldType2 = (*FeldInfo)->Type;
				if (FieldType2._Type == TypesEnum::CustomType)
				{
					Out._Symbol = Symbol_GetSymbol(FieldType2._CustomTypeSymbol).value();
					Out.Type = FieldType2;
				}
				else
				{
					Out.Type = FieldType2;

					if (Index + 1 < node._ScopedName.size())
					{
						const NeverNullPtr<Token> Token = NeverNullptr(node._ScopedName.begin()->_token);

						auto Token2 = NeverNullptr(node._ScopedName[Index + 1]._token);
						auto& Str2 = Token->Value._String;
						if (_PassType == PassType::FixedTypes)
						{
							LogError_CantFindVarMemberError(Token2, Str2, Out.Type);
						}

						Out._Symbol = nullptr;
						Out.Type = TypesEnum::Null;
						return false;
					}
				}
				if (_PassType == PassType::BuidCode)
				{
					FileDependency_AddDependencyToCurrentFile(Out._Symbol);
					//UCodeLangThrowException("not added");
				}
			}
			else
			{
				if (Index + 1 < node._ScopedName.size())
				{
					const NeverNullPtr<Token> Token = NeverNullptr(node._ScopedName.begin()->_token);

					auto Token2 = node._ScopedName[Index + 1]._token;
					auto& Str2 = Token->Value._String;
					if (_PassType == PassType::FixedTypes)
					{
						LogError_CantFindVarMemberError(NeverNullptr(Token2), Str2, VarableType);
					}

					Out._Symbol = nullptr;
					Out.Type = TypesEnum::Null;
					return false;
				}
			}
			 
		}
		else
		{
			UCodeLangUnreachable();//Bad Object
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
				Out._Symbol = nullptr;

				auto& Item = ItemToken;

				LogError_CantFindMemberOverloadForType(Item, Data.token, Out.Type);
				return false;
			}
			else
			{
				Out._Symbol = funcToCallSys;
				Out.Type = funcToCallSys->Get_Info< FuncInfo>()->Ret;
			}
		}
	}

	return true;
}
void SystematicAnalysis::LogError_UseDotAndNotScopeResolution(const NeverNullPtr<Symbol>& Sym, const NeverNullPtr<Token>& ItemToken)
{
	LogError(ErrorCodes::InValidName, "Use '.' and not '::' to use " + ToString(Sym->Type) + "'" + Sym->FullName + "'", ItemToken);
}
void SystematicAnalysis::LogError_UseScopeResolutionAndNotDot(const NeverNullPtr<Symbol>& Sym, const NeverNullPtr<Token>& ItemToken)
{
	LogError(ErrorCodes::InValidName, "Use '::' and not '.' to use " + ToString(Sym->Type) + "'" + Sym->FullName + "'", ItemToken);
}
void SystematicAnalysis::IR_Build_MemberDereferencStore(const GetMemberTypeSymbolFromVar_t& In, IRInstruction* Value)
{
	_IR_LookingAtIRBlock->NewDereferenc_Store(IR_Build_Member_DereferencStore(In), Value);
}
IRInstruction* SystematicAnalysis::IR_Build_Member_GetValue(const GetMemberTypeSymbolFromVar_t& In)
{
	switch (In._Symbol->Type)
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
			Output = In._Symbol->IR_Ins;
		}
		bool UseOutput = In._Symbol->IR_Ins != Output;

		Debug_Add_SetLineNumber(NeverNullptr(In.Start->_token), _IR_LookingAtIRBlock->Instructions.size());

		if (In._Symbol->Type == SymbolType::StackVarable
			|| In._Symbol->Type == SymbolType::Class_Field)
		{
			return _IR_LookingAtIRBlock->NewLoad(Output);
		}
		else if (In._Symbol->Type == SymbolType::ParameterVarable)
		{
			return UseOutput ? _IR_LookingAtIRBlock->NewLoad(Output) : _IR_LookingAtIRBlock->NewLoad(In._Symbol->IR_Par);
		}
		else if (In._Symbol->Type == SymbolType::StaticVarable || In._Symbol->Type == SymbolType::ThreadVarable)
		{
			return UseOutput ? _IR_LookingAtIRBlock->NewLoad(Output) : _IR_LookingAtIRBlock->NewLoad_IRID(_IR_Builder.ToID(In._Symbol->FullName));
		}
		else
		{
			UCodeLangUnreachable();
		}

			
	}
	break;
	case SymbolType::Hard_Func_ptr:
	case SymbolType::Func_ptr:
	case SymbolType::Func:
	{
		FuncInfo* Finfo = In._Symbol->Get_Info<FuncInfo>();
		
		return _IR_LookingAtIRBlock->NewLoadFuncPtr(IR_GetIRID(Finfo));
	}
	
	break;
	case SymbolType::Enum_Field:
	{
		auto Einfo = In.Get_V2<EnumInfo>();
		auto FeldInfo = In.Get_V1<EnumFieldInfo>();

		if (Einfo->VariantData.has_value())
		{
			auto ID = _IR_Builder.ToID(Einfo->FullName);
			
			auto Key = LoadEvaluatedEx(FeldInfo->Ex, Einfo->Basetype);

			auto VariantClass = _IR_LookingAtIRBlock->NewLoad(IRType(ID));
			IRStruct* V = _IR_Builder.GetSymbol(ID)->Get_ExAs<IRStruct>();
			auto Member = _IR_LookingAtIRBlock->New_Member_Access(VariantClass, V, 0);
			_IR_LookingAtIRBlock->NewStore(Member, Key);
			return VariantClass;
		}
		else 
		{
			return LoadEvaluatedEx(FeldInfo->Ex, Einfo->Basetype);
		}
	}
	case SymbolType::Type_class:
	{
		if (In.Start[0]._token->Type == TokenType::KeyWord_This)
		{
			IRInstruction* Output = nullptr;
			BuildMember_Access(In, Output);
			return _IR_LookingAtIRBlock->NewLoad(Output);
		}
		else
		{
			UCodeLangUnreachable();
		}
	}
	case  SymbolType::ConstantExpression:
	{
		auto Item = In._Symbol->Get_Info<ConstantExpressionInfo>();
		return LoadEvaluatedEx(Item->Ex, In._Symbol->VarType);
	}
	default:
		UCodeLangUnreachable();
		break;
	}
}
IRInstruction* SystematicAnalysis::IR_Build_Member_AsPointer(const GetMemberTypeSymbolFromVar_t& In)
{
	if (In.Type.IsAddress()|| In.Type.IsAddressArray())
	{
		return IR_Build_Member_GetValue(In);
	}
	else
	{
		return IR_Build_Member_GetPointer(In);
	}
}
IRInstruction* SystematicAnalysis::IR_Build_Member_DereferenceValue(const GetMemberTypeSymbolFromVar_t& In)
{
	TypeSymbol T = In.Type;
	T._IsAddress = false;
	IRType IRT = IR_ConvertToIRType(T);

	IRInstruction* Output = nullptr;

	BuildMember_Access(In, Output);
	if (Output == nullptr)
	{
		Output = In._Symbol->IR_Ins;
	}
	bool UseOutput = In._Symbol->IR_Ins != Output;

	Debug_Add_SetLineNumber(NeverNullptr(In.Start->_token), _IR_LookingAtIRBlock->Instructions.size());

	switch (In._Symbol->Type)
	{
	case  SymbolType::Class_Field:
	case  SymbolType::StackVarable:
		return UseOutput ? _IR_LookingAtIRBlock->NewLoad_Dereferenc(Output,IRT) : _IR_LookingAtIRBlock->NewLoad_Dereferenc(In._Symbol->IR_Ins, IRT);
		break;
	case  SymbolType::ParameterVarable:
		return UseOutput ? _IR_LookingAtIRBlock->NewLoad_Dereferenc(Output,IRT) : _IR_LookingAtIRBlock->NewLoad_Dereferenc(In._Symbol->IR_Par, IRT);
	default:
		UCodeLangUnreachable();
		break;
	}
}
IRInstruction* SystematicAnalysis::IR_Build_Member_AsValue(const GetMemberTypeSymbolFromVar_t& In)
{
	if (In.Type.IsAddress() || In.Type.IsAddressArray())
	{
		return IR_Build_Member_DereferenceValue(In);
	}
	else
	{
		return IR_Build_Member_GetValue(In);
	}
}
void SystematicAnalysis::BuildMember_Reassignment(const GetMemberTypeSymbolFromVar_t& In, const TypeSymbol& Type, IRInstruction* Value)
{
	if (Type.IsAddress())
	{
		IR_Build_MemberDereferencStore(In, Value);
	}
	else
	{
		IR_Build_Member_Store(In, Value);
	}
}

void  SystematicAnalysis::BuildMember_Access(const GetMemberTypeSymbolFromVar_t& In, IRInstruction*& Output)
{
	TypeSymbol Last_Type = In._Symbol->VarType;

	//

	if (In._Symbol->Type == SymbolType::Class_Field && _FuncStack.size() && _ClassStack.top().Info)
	{
		auto& Func = _FuncStack.back();
		auto& PointerIr = _IR_LookingAtIRFunc->Pars.front();

		auto ObjectType = *Func.Pointer->GetObjectForCall();
		ObjectType._IsAddress = false;

		auto objecttypesyb = Symbol_GetSymbol(ObjectType).value();




		auto IRStructV = IR_Build_ConvertToIRClassIR(*objecttypesyb);
		auto F = _IR_Builder.GetSymbol(IRStructV)->Get_ExAs<IRStruct>();

		auto Token = In.Start[In.End - 1]._token;
		auto& Str = Token->Value._String;
		ClassInfo* V = objecttypesyb->Get_Info<ClassInfo>();
		size_t MemberIndex = V->GetFieldIndex(Str).value();


		Output = _IR_LookingAtIRBlock->New_Member_Dereference(&PointerIr, IRType(IRSymbol(IRStructV)), MemberIndex);
		return;
	}
	if (In.Start[0]._token->Type == TokenType::KeyWord_This)
	{
		auto& PointerIr = _IR_LookingAtIRFunc->Pars.front();
		Output = _IR_LookingAtIRBlock->NewLoad(&PointerIr);

		auto& Func = _FuncStack.back();
		Last_Type = *Func.Pointer->GetObjectForCall();
	}
	//


	for (size_t i = 1; i < In.End; i++)
	{
		Symbol* Sym = Symbol_GetSymbol(Last_Type).value_unchecked();

		auto& ITem = In.Start[i];
		ScopedName::Operator_t OpType = i == 0 ? ScopedName::Operator_t::Null : In.Start[i - 1]._operator;

		StepBuildMember_Access(ITem, Last_Type, OpType, In, Output);
	}

}

void SystematicAnalysis::StepBuildMember_Access(const ScopedName& ITem, TypeSymbol& Last_Type, ScopedName::Operator_t OpType, const GetMemberTypeSymbolFromVar_t& In, IRInstruction*& Output)
{
	Symbol* Sym = Symbol_GetSymbol(Last_Type).value_unchecked();

	Debug_Add_SetLineNumber(NeverNullptr(ITem._token), _IR_LookingAtIRBlock->Instructions.size());

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
				UCodeLangUnreachable();
			}

			auto Funcf = funcToCallSys->Get_Info< FuncInfo>();
			Get_FuncInfo V;
			V.Func = Funcf;
			V.SymFunc = funcToCallSys;
			V.ThisPar = Get_FuncInfo::ThisPar_t::PushFromLast;

			if (Output == nullptr)
			{
				_IR_LastExpressionField = In._Symbol->IR_Ins;
			}
			else
			{
				_IR_LastExpressionField = Output;
			}


			ValueParametersNode h;
			IR_Build_FuncCall(Last_Type, V, h);

			Last_Type = Funcf->Ret;
			Sym = Symbol_GetSymbol(Last_Type).value().value();
			Output = _IR_LastExpressionField;
		}
	}

	String MemberName;
	if (ITem._token->Type == TokenType::Class)
	{
		MemberName = _VarableMemberDatas.at(Symbol_GetSymbolID(&ITem)).MemberString;
	}
	else
	{
		MemberName = ITem._token->Value._String;
	}


	switch (Sym->Type)
	{
	case  SymbolType::Type_class:
	{
		auto* Classinfo = Sym->Get_Info<ClassInfo>();
		size_t MemberIndex = Classinfo->GetFieldIndex(MemberName).value();
		FieldInfo* FInfo = &Classinfo->Fields[MemberIndex];
		IRStruct* IRstruct = _IR_Builder.GetSymbol(_Symbol_SybToIRMap[Sym->ID])->Get_ExAs<IRStruct>();
		if (Output == nullptr)
		{
			switch (In._Symbol->Type)
			{
			case  SymbolType::StackVarable:
			{
				TypeSymbol& TypeSys = Last_Type;
				if (TypeSys.IsAddress())
				{
					Output = _IR_LookingAtIRBlock->New_Member_Dereference(In._Symbol->IR_Ins, IR_ConvertToIRType(Sym->VarType), MemberIndex);
				}
				else
				{
					Output = _IR_LookingAtIRBlock->New_Member_Access(In._Symbol->IR_Ins, IRstruct, MemberIndex);
				}
			}
			break;
			case  SymbolType::ParameterVarable:
			{
				TypeSymbol& TypeSys = Last_Type;
				if (TypeSys.IsAddress())
				{
					Output = _IR_LookingAtIRBlock->New_Member_Dereference(In._Symbol->IR_Par, IR_ConvertToIRType(Sym->VarType), MemberIndex);
				}
				else
				{
					Output = _IR_LookingAtIRBlock->New_Member_Access(In._Symbol->IR_Par, IRstruct, MemberIndex);
				}

			}
			break;
			case SymbolType::ThreadVarable:
			case SymbolType::StaticVarable:
			{
				TypeSymbol& TypeSys = Last_Type;
				auto id = _IR_Builder.ToID(In._Symbol->FullName);
				if (TypeSys.IsAddress())
				{
					Output = _IR_LookingAtIRBlock->New_Member_Dereference(id,IR_ConvertToIRType(Sym->VarType), MemberIndex);
				}
				else
				{
					Output = _IR_LookingAtIRBlock->New_Member_Access(id, IRstruct, MemberIndex);
				}

			}
			break;
			default:
				UCodeLangUnreachable();
				break;
			}
		}
		else
		{
			TypeSymbol& TypeSys = Last_Type;
			if (TypeSys.IsAddress())
			{
				Output = _IR_LookingAtIRBlock->New_Member_Dereference(Output, IR_ConvertToIRType(Sym->VarType), MemberIndex);
			}
			else
			{
				Output = _IR_LookingAtIRBlock->New_Member_Access(Output, IRstruct, MemberIndex);
			}
		}
		Last_Type = FInfo->Type;
	}
	break;

	default:
		UCodeLangUnreachable();
		break;
	}
}

NullablePtr<Symbol> SystematicAnalysis::GetTepFuncPtrSyb(const String& TepFuncPtr, const FuncInfo* Finfo)
{
	NullablePtr<Symbol> V2 =Symbol_GetSymbol(TepFuncPtr, SymbolType::Func_ptr);
	if (!V2.has_value())
	{
		NeverNullPtr<Symbol> V = &Symbol_AddSymbol(SymbolType::Func_ptr, TepFuncPtr, Finfo->FullName,AccessModifierType::Public);
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

#define TepFuncPtrNameMangleStr "_tepfptr|"
String SystematicAnalysis::GetTepFuncPtrName(const FuncInfo* SymbolVar)
{
	return TepFuncPtrNameMangleStr + SymbolVar->FullName;
}
String_view SystematicAnalysis::GetTepFuncPtrNameAsName(const String_view Str)
{
	return Str.substr(sizeof(TepFuncPtrNameMangleStr)-1);//remove null char
}
bool SystematicAnalysis::Symbol_MemberTypeSymbolFromVar(size_t Start, size_t End, const ScopedNameNode& node, GetMemberTypeSymbolFromVar_t& Out)
{

	if (_PassType == PassType::GetTypes) { return false; }

	auto& Mod = _GetExpressionMode.top();


	size_t ScopedCount = 0;
	if (Out._Symbol == nullptr && Out.Type.IsBadType())
	{


		if (node._ScopedName[Start]._token->Type != TokenType::KeyWord_This)
		{
			auto ScopeName = node._ScopedName[Start];
			auto Token = NeverNullptr(ScopeName._token);
			auto& Str = Token->Value._String;
			auto SymbolVarOp = Symbol_GetSymbol(Str, SymbolType::Varable_t);
			_LastLookedAtToken = Token.AsNullable();

		
			if (!SymbolVarOp.has_value())
			{
				LogError_CantFindVarError(Token, Str);
				return false;
			}	
			auto SymbolVar = SymbolVarOp.value();

			if (SymbolVar->PassState == PassType::GetTypes)
			{
				Symbol_Update_Sym_ToFixedTypes(SymbolVar);
			}


			if (IsWrite(Mod))
			{
				TryLogError_OnWritingVar(SymbolVar, Token, String_view(Str));
			}
			if (IsRead(Mod))
			{
				TryLogError_OnReadVar(String_view(Str), Token,SymbolVar.AsNullable());
			}


			Out.Type = SymbolVar->VarType;
			Out._Symbol = SymbolVar.value();

			if (SymbolVar->Type == SymbolType::Func)
			{
				auto funcinfo = SymbolVar->Get_Info<FuncInfo>();
				auto StrFunc = GetTepFuncPtrName(funcinfo);
				Out.Type = TypeSymbol(GetTepFuncPtrSyb(StrFunc, funcinfo).value()->ID);
			}
			
			if (SymbolVar->Type == SymbolType::Class_Field)
			{
				if (!Context_IsInThisFuncCall())
				{
					LogError_CantUseThisInStaticFunction(Token);
					Out._Symbol = nullptr;
					Out.Type = TypeSymbol();
					return false;
				}
			}

			if (ScopeName._generic.get() && ScopeName._generic->_Values.size())
			{
				TypeNode Tep;
				Tep._name.token = ScopeName._token;
				auto& Other = *ScopeName._generic;


				{//cant copy TypeNode but we need anyway.
					Tep._generic._Values.resize(Other._Values.size());
					memcpy(Tep._generic._Values.data(), Other._Values.data(), sizeof(TypeNode) * Other._Values.size());

				}

				TypeSymbol Type;
				Type_Convert(Tep, Type);
				if (Type._Type != TypesEnum::Null)
				{
					Out.Type = Type;
					Out._Symbol = Symbol_GetSymbol(Type).value_unchecked();
				}
				
				{// TypeNode has Unique_ptr we do this to not free it.
					new (Tep._generic._Values.data()) TypeNode[Other._Values.size()];
				}
			}
			//
			Start++;
			End--;
			ScopedCount++;
		}
		else
		{
			if (_ClassStack.size() == 0)
			{
				LogError_CantUseThisKeyWordHereError(NeverNullptr(node._ScopedName[Start]._token));
				return false;
			}


			if (!Context_IsInThisFuncCall())
			{
				LogError_CantUseThisInStaticFunction(NeverNullptr(node._ScopedName[Start]._token));
				return false;
			}



			auto& Func = _FuncStack.back().Pointer;
			auto ObjectType = Func->GetObjectForCall();

			auto objecttypesyb = Symbol_GetSymbol(*ObjectType).value();
			ClassInfo* V = objecttypesyb->Get_Info<ClassInfo>();

			Out.Type = *Func->GetObjectForCall();
			Out._Symbol = Symbol_GetSymbol(*ObjectType).value().value();
			//
			Start++;
			End--;
			ScopedCount++;
		}
	}


	bool BadValue = false;
	for (size_t i = Start; i < node._ScopedName.size(); i++)
	{
		if (i > End) { break; }
		ScopedCount++;

		ScopedName::Operator_t OpType = i == 0 ? ScopedName::Operator_t::Null : node._ScopedName[i - 1]._operator;
		if (Symbol_StepGetMemberTypeSymbolFromVar(node, i, OpType, Out) == false)
		{
			BadValue = true;
			continue;
		}
	}
	if (BadValue == false)
	{
		Out.Start = &node._ScopedName[Start - 1];
		Out.End = ScopedCount;
		if (End == -1)
		{
			Out.End++;//i dont know why this needed its most likely its us useing two diff funcs for the same thing to get the start off syb.
		}

		{
			auto OldTepSyb = Out._Symbol;
			auto ConstExCheck = Out._Symbol;
			while (ConstExCheck &&
				(ConstExCheck->Type == SymbolType::Type_alias || ConstExCheck->Type == SymbolType::Hard_Type_alias))
			{
				ConstExCheck = Symbol_GetSymbol(ConstExCheck->VarType).value_unchecked();
			}
			if (ConstExCheck == nullptr)
			{
				Out._Symbol = OldTepSyb;

			}
			else if (ConstExCheck->Type == SymbolType::ConstantExpression)
			{
				ConstantExpressionInfo* ConstInfo = ConstExCheck->Get_Info<ConstantExpressionInfo>();

				Out.Type = ConstExCheck->VarType;
				Out._Symbol = ConstExCheck;
			}
		}

		if (!(Out._Symbol->Type == SymbolType::Class_Field
			|| Out._Symbol->Type == SymbolType::Enum_Field
			|| Out._Symbol->Type == SymbolType::Func
			|| Out._Symbol->Type == SymbolType::ConstantExpression
			|| Symbol_IsVarableType(Out._Symbol->Type)))
		{

			if (_PassType == PassType::FixedTypes)
			{
				auto& Item = NeverNullptr(node._ScopedName.back()._token);
				LogError_LogWantedAVariable(Item, Out._Symbol);
			}
			Out.Type.SetType(TypesEnum::Null);
			Out._Symbol = nullptr;

			return false;
		}

		if (IsWrite(Mod) && !(Out._Symbol->Type == SymbolType::Class_Field || Symbol_IsVarableType(Out._Symbol->Type)))
		{
			auto& Item = NeverNullptr(node._ScopedName.back()._token);
			LogError(ErrorCodes::InValidType, Item->OnLine, Item->OnPos, "You Cant Write to a " + ToString(Out._Symbol->Type));
		}



		if (_Varable.size())
		{
			auto& Data = _Varable.top();
			Data._UsedSymbols.push_back(Out._Symbol);
		}
		return true;
	}
	else
	{
		return false;
	}
}
void SystematicAnalysis::OnPostfixVariableNode(const PostfixVariableNode& node)
{
	if (_PassType == PassType::GetTypes)
	{
		OnExpressionTypeNode(node._ToAssign._Value.get(),GetValueMode::ReadAndWrite);
	}
	else
	if (_PassType == PassType::FixedTypes)
	{
		_LookingForTypes.push(TypesEnum::Any);
		OnExpressionTypeNode(node._ToAssign._Value.get(), GetValueMode::ReadAndWrite);
		_LookingForTypes.pop();

		auto ExType = _LastExpressionType;

		auto HasInfo = Type_HasPostfixOverLoadWith(ExType, node._PostfixOp->Type);
		if (!HasInfo.HasValue)
		{
			LogError_CantFindPostfixOpForTypes(NeverNullptr(node._PostfixOp), ExType);
		}


		PostFixExpressionNode_Data t;
		
		if (HasInfo.Value) 
		{
			FuncInfo* f = HasInfo.Value.value()->Get_Info<FuncInfo>();
			t.FuncToCall = HasInfo.Value.value();
			t.Op0 = f->Pars[0].Type;
		}
		else
		{
			t.Op0 = _LastExpressionType;
		}

		_PostFix_Datas.AddValue(Symbol_GetSymbolID(node),t);
	}
	else
	if (_PassType == PassType::BuidCode)
	{
		const PostFixExpressionNode_Data& Data = _PostFix_Datas.at(Symbol_GetSymbolID(node));

		if (Data.FuncToCall)
		{
			FuncInfo* f = Data.FuncToCall->Get_Info<FuncInfo>();

			Get_FuncInfo V;
			V.Func = f;
			V.SymFunc = Data.FuncToCall;
			V.ThisPar = Get_FuncInfo::ThisPar_t::NoThisPar;

			ScopedNameNode Tep;

			ValueParametersNode pars;
			pars._Nodes.push_back(Unique_ptr<Node>(node._ToAssign._Value.get()));

			IR_Build_FuncCall(V, Tep, pars);

			pars._Nodes[0].release();//its ok Tree has pointer.

			_LastExpressionType = V.Func->Ret;
		}
		else 
		{
			OnExpressionTypeNode(node._ToAssign._Value.get(), GetValueMode::ReadAndWrite);

			IRInstruction* LoadV = _IR_LastExpressionField;

			auto Type = _LastExpressionType;

#define buildPortFixU(x)\
		if (node._PostfixOp->Type == TokenType::increment)\
		{\
			_IR_LastExpressionField=_IR_LookingAtIRBlock->NewAdd(LoadV,_IR_LookingAtIRBlock->NewLoad((UInt##x)1));\
		}\
		else\
		{\
			_IR_LastExpressionField=_IR_LookingAtIRBlock->NewSub(LoadV, _IR_LookingAtIRBlock->NewLoad((UInt##x)1));\
		}\



#define buildPortFixS(x) buildPortFixU(x)


			Debug_Add_SetLineNumber(NeverNullptr(node._PostfixOp), _IR_LookingAtIRBlock->Instructions.size());

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
			IR_WriteTo(_IR_LastExpressionField, Type, _IR_LastStoreField);
		}
	}
}
void SystematicAnalysis::OnCompoundStatementNode(const CompoundStatementNode& node)
{

	if (_PassType == PassType::GetTypes)
	{
		OnExpressionTypeNode(node._ToAssign._Value.get(),GetValueMode::ReadAndWrite);
		OnExpressionTypeNode(node._Expession._Value.get(),GetValueMode::Read);
		return;
	}
	else if (_PassType == PassType::FixedTypes)
	{
		_LookingForTypes.push(TypesEnum::Any);
		OnExpressionTypeNode(node._ToAssign._Value.get(), GetValueMode::ReadAndWrite);
		_LookingForTypes.pop();

		TypeSymbol ToAssignType = _LastExpressionType;

		_LookingForTypes.push(TypesEnum::Any);
		OnExpressionTypeNode(node._Expession._Value.get(), GetValueMode::Read);
		_LookingForTypes.pop();

		auto ExType = _LastExpressionType;


		auto HasInfo = Type_HasCompoundOverLoadWith(ToAssignType, node._CompoundOp->Type, ExType);
		if (!HasInfo.HasValue)
		{
			LogError_CantFindCompoundOpForTypes(NeverNullptr(node._CompoundOp), ToAssignType, ExType);
		}

		CompoundExpresion_Data r;

		if (HasInfo.Value)
		{
			FuncInfo* f = HasInfo.Value.value()->Get_Info<FuncInfo>();
			r.FuncToCall = HasInfo.Value.value();
			r.Op0 = f->Pars[0].Type;
			r.Op1 = f->Pars[1].Type;
		}
		else
		{
			r.Op0 = ToAssignType;
			r.Op1 = ExType;

			r.Op0._IsAddress = false;
			r.Op1._IsAddress = false;
		}

		_Compound_Datas.AddValue(Symbol_GetSymbolID(node), r);
	}
	else if (_PassType == PassType::BuidCode)
	{
		const auto& Data = _Compound_Datas.at(Symbol_GetSymbolID(node));

		if (Data.FuncToCall)
		{

			FuncInfo* f = Data.FuncToCall->Get_Info<FuncInfo>();

			Get_FuncInfo V;
			V.Func = f;
			V.SymFunc = Data.FuncToCall;
			V.ThisPar = Get_FuncInfo::ThisPar_t::PushFromLast;


			ScopedNameNode Tep;
			ScopedName TepV;
			TepV._token = _LastLookedAtToken.value().value();
			Tep._ScopedName.push_back(TepV);

			ValueParametersNode pars;
			pars._Nodes.push_back(Unique_ptr<Node>(node._ToAssign._Value.get()));
			pars._Nodes.push_back(Unique_ptr<Node>(node._Expession._Value.get()));


			IR_Build_FuncCall(V, Tep, pars);

			pars._Nodes[0].release();
			pars._Nodes[1].release();
			//no mem leak node as a Unique_ptr to Ex

			_LastExpressionType = V.Func->Ret;
		}
		else
		{

			_LookingForTypes.push(Data.Op0);

			OnExpressionTypeNode(node._ToAssign._Value.get(), GetValueMode::ReadAndWrite);
			auto AssignType = _LastExpressionType;

			_LookingForTypes.pop();

			IRInstruction* LoadV = _IR_LastExpressionField;

			_LookingForTypes.push(Data.Op1);

			OnExpressionTypeNode(node._Expession._Value.get(), GetValueMode::Read);

			_LookingForTypes.pop();


#define Set_CompoundU(x) \
			switch (node._CompoundOp->Type) \
			{ \
			case TokenType::CompoundAdd: \
				_IR_LastExpressionField=_IR_LookingAtIRBlock->NewAdd(LoadV,_IR_LastExpressionField);\
				break; \
			case TokenType::CompoundSub:\
				_IR_LastExpressionField=_IR_LookingAtIRBlock->NewSub(LoadV,_IR_LastExpressionField); \
				break; \
			case TokenType::CompoundMult:\
				_IR_LastExpressionField = _IR_LookingAtIRBlock->NewUMul(LoadV, _IR_LastExpressionField);\
			    break; \
			case TokenType::CompoundDiv:\
				_IR_LastExpressionField = _IR_LookingAtIRBlock->NewUDiv(LoadV, _IR_LastExpressionField);\
			    break; \
			default:\
				UCodeLangUnreachable(); \
				break; \
			}\

#define Set_CompoundS(x) \
		switch (node._CompoundOp->Type) \
		{ \
		case TokenType::CompoundAdd: \
				_IR_LastExpressionField=_IR_LookingAtIRBlock->NewAdd(LoadV,_IR_LastExpressionField);\
				break; \
			case TokenType::CompoundSub:\
				_IR_LastExpressionField=_IR_LookingAtIRBlock->NewSub(LoadV,_IR_LastExpressionField); \
				break; \
			case TokenType::CompoundMult:\
				_IR_LastExpressionField = _IR_LookingAtIRBlock->NewSMul(LoadV, _IR_LastExpressionField);\
			    break; \
			case TokenType::CompoundDiv:\
				_IR_LastExpressionField = _IR_LookingAtIRBlock->NewSDiv(LoadV, _IR_LastExpressionField);\
			    break; \
			default:\
				UCodeLangUnreachable(); \
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
				UCodeLangUnreachable();
				break;
			}

			IR_WriteTo(_IR_LastExpressionField, AssignType, _IR_LastStoreField);
		}
	}
}
void SystematicAnalysis::OnExpressionTypeNode(const Node* node, GetValueMode Mode)
{
	_GetExpressionMode.push(Mode);

	Push_ToNodeScope(*node);

	switch (node->Get_Type())
	{
	case NodeType::BinaryExpressionNode:OnExpressionNode(*BinaryExpressionNode::As(node));break;
	case NodeType::ValueExpressionNode:OnExpressionNode(*ValueExpressionNode::As(node)); break;
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
	
	if (_PassType == PassType::BuidCode
		|| _PassType == PassType::FixedTypes
		
		|| (_PassType == PassType::GetTypes && 
			((node._Value->Get_Type() == NodeType::LambdaNode) ||
			(node._Value->Get_Type() == NodeType::YieldExpression) ||
			(node._Value->Get_Type() == NodeType::AwaitExpression)
		)))
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

bool SystematicAnalysis::Type_Compare(const TypeSymbol& Type0, const TypeSymbol& Type1,const NeverNullPtr<Token> Value)
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


		bool CMPValue =Type_Compare(Type0,Type1, NeverNullptr(node._Op));


		_ValidNodes.AddValue(Symbol_GetSymbolID(node), CMPValue);
	}
	if (_PassType == PassType::BuidCode)
	{
		_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad(_ValidNodes[Symbol_GetSymbolID(node)]);
	}

	_LastExpressionType.SetType(TypesEnum::Bool);
}

void SystematicAnalysis::OnMovedNode(const MoveNode* nod)
{

	OnExpressionTypeNode(nod->_expression._Value.get(), GetValueMode::Read);
	
	auto ExType = _LastExpressionType;
	ExType.SetAsMoved();
	_LastExpressionType = ExType;
}

void SystematicAnalysis::OnNumberliteralNode(const NumberliteralNode* num)
{
	auto& lookT = Type_Get_LookingForType();
	TypesEnum NewEx;
	if (lookT._Type == TypesEnum::Var)
	{
		NewEx = TypesEnum::sInt32;
	}
	else
	{
		NewEx = (Type_IsfloatType(lookT) || Type_IsIntType(lookT)) ? lookT._Type : TypesEnum::sInt32;
	}
#define Set_NumberliteralNodeU(x) \
			UInt##x V; \
			ParseHelper::ParseStringToUInt##x(Str, V); \
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad(V);\

#define Set_NumberliteralNodeS(x) \
			Int##x V; \
			ParseHelper::ParseStringToInt##x(Str, V); \
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad(V);\

	if (_PassType == PassType::BuidCode)
	{
		auto& Str = num->token->Value._String;

		Debug_Add_SetLineNumber(NeverNullptr(num->token), _IR_LookingAtIRBlock->Instructions.size());

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
			_IR_LastExpressionField = IR_Load_UIntptr(V);
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
			_IR_LastExpressionField = IR_Load_SIntptr(V);
			break;
		};


		case TypesEnum::float32:
		{
			Int32 V;
			ParseHelper::ParseStringToInt32(Str, V);
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((float32)V);
			break;
		};
		case TypesEnum::float64:
		{
			Int64 V;
			ParseHelper::ParseStringToInt64(Str, V);
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((float64)V);
			break;
		};
		default:
			UCodeLangUnreachable();
			break;
		}
	}


	_LastExpressionType.SetType(NewEx);
	_LastLookedAtToken =Nullableptr(num->token);
}

void SystematicAnalysis::OnBoolliteralNode(const BoolliteralNode* num)
{
	if (_PassType == PassType::BuidCode)
	{
		Debug_Add_SetLineNumber(NeverNullptr(num->token), _IR_LookingAtIRBlock->Instructions.size());
		_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad(num->Get_Value());
	}
	_LastExpressionType.SetType(TypesEnum::Bool);
	_LastLookedAtToken = Nullableptr(num->token);
}

void SystematicAnalysis::OnCharliteralNode(const CharliteralNode* num)
{
	auto& lookT = Type_Get_LookingForType();

	if (_PassType == PassType::BuidCode)
	{
		String V;
		bool ItWorked = false;
		Debug_Add_SetLineNumber(NeverNullptr(num->token), _IR_LookingAtIRBlock->Instructions.size());

		if (lookT._Type == TypesEnum::Uft8)
		{
			String8 V;
			ItWorked = !ParseHelper::ParseCharliteralToChar(num->token->Value._String, V);
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((Utf8)V.front());
		}
		else if (lookT._Type == TypesEnum::Uft16)
		{
			String16 V;
			ItWorked = !ParseHelper::ParseCharliteralToChar(num->token->Value._String, V);
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((Utf16)V.front());
		}
		else if (lookT._Type == TypesEnum::Uft32)
		{
			String32 V;
			ItWorked = !ParseHelper::ParseCharliteralToChar(num->token->Value._String, V);
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((Utf32)V.front());
		}
		else
		{
			String V;
			ItWorked = !ParseHelper::ParseCharliteralToChar(num->token->Value._String, V);
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((char)V.front());
		}
	

	}
	if (lookT._Type == TypesEnum::Uft8 || lookT._Type == TypesEnum::Uft16
		|| lookT._Type == TypesEnum::Uft32)
	{
		_LastExpressionType = TypeSymbol(lookT._Type);
	}
	else 
	{
		_LastExpressionType = TypeSymbol(TypesEnum::Char);
	}
	_LastLookedAtToken = Nullableptr(num->token);
}

void SystematicAnalysis::OnFloatLiteralNode(const FloatliteralNode* num)
{
	auto& lookT = Type_Get_LookingForType();
	if (_PassType == PassType::BuidCode)
	{

		switch (lookT._Type)
		{
		case TypesEnum::float32:
		{
			float32 V;
			bool ItWorked = ParseHelper::ParseStringTofloat32(num->token->Value._String, V);

			Debug_Add_SetLineNumber(NeverNullptr(num->token), _IR_LookingAtIRBlock->Instructions.size());
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad(V);
			break;
		}
		case TypesEnum::float64:
		{
			float64 V;
			bool ItWorked = ParseHelper::ParseStringTofloat64(num->token->Value._String, V);

			Debug_Add_SetLineNumber(NeverNullptr(num->token), _IR_LookingAtIRBlock->Instructions.size());
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad(V);
			break;
		}
		default:
			UCodeLangUnreachable();
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
		NewEx = (Type_IsfloatType(lookT)) ? lookT._Type : TypesEnum::float32;
	}


	_LastExpressionType.SetType(NewEx);
	_LastLookedAtToken =Nullableptr(num->token);
}

void SystematicAnalysis::OnStringLiteral(const StringliteralNode* nod, bool& retflag)
{
	retflag = true;
	


	auto& Type = Type_Get_LookingForType();
	enum class StringType
	{
			Char,
			Utf8,
			Utf16,
			Utf32,
	};
	StringType stringtype = StringType::Char;

	if (Type_IsStringSpan8(Type) || Type_IsString8(Type))
	{
		stringtype = StringType::Utf8;
	}
	else if (Type_IsStringSpan16(Type) || Type_IsString16(Type))
	{
		stringtype = StringType::Utf16;
	}
	else if (Type_IsStringSpan32(Type) || Type_IsString32(Type))
	{
		stringtype = StringType::Utf32;
	}
	else 
	{
		stringtype = StringType::Char;
	}

	if (_PassType == PassType::FixedTypes)
	{
		//TODO check for valid String,String8,String16,String32

		TypeSymbol CharType;
		TypeSymbol SpanStringType;
		auto Token = NeverNullptr(nod->token);
		switch (stringtype)
		{
		case StringType::Char:
		{
			CharType = TypeSymbol(TypesEnum::Char);
			SpanStringType = Type_GetStringSpan(Token).value_or(TypeSymbol(TypesEnum::Null));
		}
		break;
		case StringType::Utf8:
		{
			CharType = TypeSymbol(TypesEnum::Uft8);
			SpanStringType = Type_GetStringSpan8(Token).value_or(TypeSymbol(TypesEnum::Null));
		}
		break;
		case StringType::Utf16:
		{
			CharType = TypeSymbol(TypesEnum::Uft16);
			SpanStringType = Type_GetStringSpan16(Token).value_or(TypeSymbol(TypesEnum::Null));
		}
		break;
		case StringType::Utf32:
		{
			CharType = TypeSymbol(TypesEnum::Uft32);
			SpanStringType = Type_GetStringSpan32(Token).value_or(TypeSymbol(TypesEnum::Null));
		}
		break;
		default:
			UCodeLangUnreachable();
			break;
		}

		if (!SpanStringType.IsBadType())
		{
			FuncInfo* func = nullptr;
			{
				String scope = ScopeHelper::ApendedStrings(ToString(SpanStringType), ClassConstructorfunc);

				auto list = _Table.GetSymbolsWithName(scope);
				for (auto& Item : list)
				{
					if (Item->Type == SymbolType::Func)
					{
						FuncInfo* finfo = Item->Get_Info<FuncInfo>();
						if (finfo->Pars.size() == 3)
						{
							auto& pointerpar = finfo->Pars[1];
							auto& sizepar = finfo->Pars[2];
							if (pointerpar.IsOutPar == true
								|| sizepar.IsOutPar == true)
							{
								continue;
							}
							if (pointerpar.Type._Type != CharType._Type)
							{
								continue;
							}
							if (!pointerpar.Type.IsAddressArray())
							{
								continue;
							}
							if (sizepar.Type.IsAddress()
								|| sizepar.Type.IsAddressArray()
								|| sizepar.Type._Type != TypesEnum::uIntPtr)
							{
								continue;
							}
							func = finfo;
							break;
						}
					}
				}
			}
			if (func == nullptr)
			{
				ParInfo par0;
				par0.Type = TypeSymbol(SpanStringType._CustomTypeSymbol);
				par0.Type.SetAsAddress();

				ParInfo par1;
				par1.Type = CharType;
				par1.Type.SetAsimmutable();
				par1.Type.SetAsAddressArray();

				ParInfo par2;
				par2.Type = TypesEnum::uIntPtr;
				LogError_CantFindFuncError(Token, "new"
					, {}, { par0,par1,par2 }, TypeSymbol(TypesEnum::Void));
			}
		}

		SpanStringType.SetAsimmutable();
		_LastExpressionType = SpanStringType;
	}

	bool IsStaticArr = Type_IsStaticCharArr(Type);



	if (IsStaticArr)
	{
		switch (stringtype)
		{
		case StringType::Char:
		{
			
		}
		break;
		case StringType::Utf8:
		{
			UCodeLangToDo();//"add utf8"
		}
		break;
		case StringType::Utf16:
		{

			UCodeLangToDo();//"add utf16"
		}
		break;
		case StringType::Utf32:
		{

			UCodeLangToDo();//"add utf32"
		}
		break;
		default:
			UCodeLangUnreachable();
			break;
		}
		auto V = Symbol_GetSymbol(Type).value();
		StaticArrayInfo* StaticArr = V->Get_Info<StaticArrayInfo>();

		if (_PassType == PassType::FixedTypes)
		{
			String V;
			bool ItWorked = !ParseHelper::ParseStringliteralToString(nod->token->Value._String, V);
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
					const NeverNullPtr<Token> Token = _LastLookedAtToken.value();
					LogError_CanIncorrectStaticArrCount(Token, Type, BufferSize, StaticArr->Count);
					_LastExpressionType.SetType(TypesEnum::Null);
					return;
				}
			}
		}

		if (_PassType == PassType::BuidCode)
		{
			String V;
			bool ItWorked = !ParseHelper::ParseStringliteralToString(nod->token->Value._String, V);


			Debug_Add_SetLineNumber(NeverNullptr(nod->token), _IR_LookingAtIRBlock->Instructions.size());

			auto& BufferIR = _IR_IRlocations.top();
			BufferIR.UsedlocationIR = true;
			auto BufferIRIns = BufferIR.Value;

			const auto& ArrItemType = StaticArr->Type;
			const auto IRItemType = IR_ConvertToIRType(ArrItemType);
			UAddress Size;
			Type_GetSize(ArrItemType, Size);
			auto ValueSizeIR = IR_Load_UIntptr(Size);

			if (!Type.IsAddress())
			{
				BufferIRIns = _IR_LookingAtIRBlock->NewLoadPtr(BufferIRIns);
			}

			for (size_t i = 0; i < V.size(); i++)
			{
				auto VIR = _IR_LookingAtIRBlock->NewLoad(V[i]);


				auto f = _IR_LookingAtIRBlock->New_Index_Vetor(BufferIRIns, IR_Load_UIntptr(i), ValueSizeIR);

				_IR_LookingAtIRBlock->NewDereferenc_Store(f, VIR);
			}


		}

		_LastExpressionType = Type;
	}
	else
	{
		auto Token = NeverNullptr(nod->token);
		TypeSymbol SpanStringType;
		switch (stringtype)
		{
		case StringType::Char:
		{
			SpanStringType = Type_GetStringSpan(Token).value();
		}
		break;
		case StringType::Utf8:
		{
			SpanStringType = Type_GetStringSpan8(Token).value();
		}
		break;
		case StringType::Utf16:
		{
			SpanStringType = Type_GetStringSpan16(Token).value();
		}
		break;
		case StringType::Utf32:
		{
			SpanStringType = Type_GetStringSpan32(Token).value();
		}
		break;
		default:
			UCodeLangUnreachable();
			break;
		}
		SpanStringType.SetAsimmutable();
		_LastExpressionType = SpanStringType;

		if (_PassType == PassType::BuidCode)
		{
			IRBuilder::StringSpanInfo Span;
			size_t SpanSize;
			switch (stringtype)
			{
			case StringType::Char:
			{
				String V;
				bool ItWorked = !ParseHelper::ParseStringliteralToString(nod->token->Value._String, V);
				Span = _IR_Builder.FindOrAddStaticSpanString((String_view)V);
				SpanSize = V.size();
			}
			break;
			case StringType::Utf8:
			{
				String8 V;
				bool ItWorked = !ParseHelper::ParseStringliteralToString(nod->token->Value._String, V);
				Span = _IR_Builder.FindOrAddStaticSpanString8((String_view8)V);
				SpanSize = V.size();
			}
			break;
			case StringType::Utf16:
			{
				String16 V;
				bool ItWorked = !ParseHelper::ParseStringliteralToString(nod->token->Value._String, V);
				Span = _IR_Builder.FindOrAddStaticSpanString16((String_view16)V);
				SpanSize = V.size();
			}
			break;
			case StringType::Utf32:
			{
				String32 V;
				bool ItWorked = !ParseHelper::ParseStringliteralToString(nod->token->Value._String, V);
				Span = _IR_Builder.FindOrAddStaticSpanString8((String_view32)V);
				SpanSize = V.size();
			}
			break;
			default:
				UCodeLangUnreachable();
				break;
			}
			
			TypeSymbol spantype = SpanStringType;
			auto irpointer = _IR_LookingAtIRBlock->NewLoadPtr(Span.StaticVar);
			if (Span.Offset != 0)
			{
				size_t mult = 0;
				switch (stringtype)
				{
				case StringType::Char:
				{
					mult = 1;
				}
				break;
				case StringType::Utf8:
				{
					mult = 1;
				}
				break;
				case StringType::Utf16:
				{
					mult = 2;
				}
				break;
				case StringType::Utf32:
				{
					mult = 4;
				}
				break;
				default:
					UCodeLangUnreachable();
					break;
				}
				irpointer = _IR_LookingAtIRBlock->NewAdd(IR_Load_UIntptr(mult),irpointer);
			}
			auto irsize = IR_Load_UIntptr(SpanSize);
			IRInstruction* irspan = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(SpanStringType));
			
			FuncInfo* func = nullptr;
			
			{
				spantype._Isimmutable = false;
				String scope = ScopeHelper::ApendedStrings(ToString(spantype), ClassConstructorfunc);
				spantype.SetAsimmutable();

				auto list = _Table.GetSymbolsWithName(scope);
				for (auto& Item : list)
				{
					if (Item->Type == SymbolType::Func)
					{
						FuncInfo* finfo = Item->Get_Info<FuncInfo>();
						if (finfo->Pars.size() == 3)
						{
							func = finfo;
							break;
						}
					}
				}
			}
			_IR_LookingAtIRBlock->NewPushParameter(_IR_LookingAtIRBlock->NewLoadPtr(irspan));
			_IR_LookingAtIRBlock->NewPushParameter(irpointer);
			_IR_LookingAtIRBlock->NewPushParameter(irsize);
			_IR_LookingAtIRBlock->NewCall(IR_GetIRID(func));

			_IR_LastExpressionField = irspan;


		}

	}
	retflag = false;
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

void SystematicAnalysis::OnSizeofNode(const SizeofExpresionNode* nod)
{
	auto& lookT = Type_Get_LookingForType();
	TypeSymbol Type;

	if (_PassType == PassType::FixedTypes || _PassType == PassType::BuidCode)
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

	if (_PassType == PassType::BuidCode)
	{
		TypeSymbol Info;
		Type_ConvertAndValidateType(nod->_Type, Info,NodeSyb_t::Any);
		UAddress TypeSize;
		Type_GetSize(Info, TypeSize);
		switch (lookT._Type)
		{
		case TypesEnum::sInt8:
		case TypesEnum::uInt8:
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((UInt8)TypeSize);
			break;
		case TypesEnum::sInt16:
		case TypesEnum::uInt16:
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((UInt16)TypeSize);
			break;
		case TypesEnum::sInt32:
		case TypesEnum::uInt32:
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((UInt32)TypeSize);
			break;
		case TypesEnum::sInt64:
		case TypesEnum::uInt64:
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((UInt64)TypeSize);
			break;
		default:
			Type.SetType(TypesEnum::uIntPtr);
			_IR_LastExpressionField = IR_Load_UIntptr(TypeSize);
			break;
		}
	}

	_LastExpressionType = Type;
}


void SystematicAnalysis::OnNewNode(const NewExpresionNode* nod)
{
	if (_PassType == PassType::FixedTypes)
	{
		if (!IsInUnSafeBlock())
		{
			auto Token = nod->_KeywordToken;
			LogError(ErrorCodes::ExpectingSequence, Token->OnLine, Token->OnPos, "Cant use 'new' keyword in safe mode.");
			return;
		}
	}

	TypeSymbol Type;
	Type_Convert(nod->_Type, Type);
	bool IsArray = nod->_Arrayexpression._Value.get();

	if (_PassType == PassType::FixedTypes)
	{
		if (IsArray)
		{
			TypeSymbol UintptrType(TypesEnum::uIntPtr);

			_LookingForTypes.push(UintptrType);
			OnExpressionTypeNode(nod->_Arrayexpression._Value.get(),GetValueMode::Read);

			if (!Type_CanBeImplicitConverted(_LastExpressionType, UintptrType))
			{
				auto  Token = _LastLookedAtToken;
				LogError_CantCastImplicitTypes(Token.value(), _LastExpressionType, UintptrType, true);
			}

			_LookingForTypes.pop();

		}
	
		auto Func = Type_GetFunc(Type, nod->_Parameters);
		_FuncToSyboID[Symbol_GetSymbolID(nod)] = Func;



		if (IsArray)
		{
			Type.SetAsAddressArray();
		}
		else
		{
			Type.SetAsAddress();
		}
		_LastExpressionType = Type;
	}

	if (_PassType == PassType::BuidCode)
	{
	

		auto Func = _FuncToSyboID.at(Symbol_GetSymbolID(nod));
		auto& ValuePars = nod->_Parameters;
		

		UAddress TypeSize;
		Type_GetSize(Type, TypeSize);

		
		if (IsArray)
		{

			TypeSymbol UintptrType = TypeSymbol();
			UAddress UintptrSize;
			{
				UintptrType.SetType(TypesEnum::uIntPtr);
				Type_GetSize(UintptrType, UintptrSize);
			}
			bool TypeHaveDestructor = Symbol_HasDestructor(Type);

			_LookingForTypes.push(UintptrType);


			OnExpressionTypeNode(nod->_Arrayexpression._Value.get(), GetValueMode::Read);

			auto Ex0 = _IR_LastExpressionField;
			IR_Build_ImplicitConversion(Ex0, _LastExpressionType, UintptrType);

			Ex0 = _IR_LastExpressionField;


			auto SizeV = IR_Load_UIntptr(TypeSize);//UintptrSize is for the array length for Drop 



			auto DataSize = IR_Build_Mult_uIntPtr(Ex0, SizeV);//uintptr 
			//malloc(sizeof(Type) * ArrayExpression); 


			if (TypeHaveDestructor)
			{
				IR_Build_Increment_uIntPtr(DataSize,UintptrSize);//Make room for Size on the Ptr
			}

			auto MallocPtr = _IR_LastExpressionField = _IR_LookingAtIRBlock->NewMallocCall(DataSize);

			if (TypeHaveDestructor)
			{
				_IR_LookingAtIRBlock->NewDereferenc_Store(MallocPtr, Ex0);//set Size.
				IR_Build_Increment_uIntPtr(MallocPtr,UintptrSize);//move pointer

			}

			//Call default on every

			//our index
			auto Indexir = IR_Load_UIntptr(0);

			size_t JumpLabel = _IR_LookingAtIRBlock->GetIndex();
			auto Cmpbool = _IR_LookingAtIRBlock->NewC_Equalto(Indexir, Ex0);


			auto JumpIns = _IR_LookingAtIRBlock->NewConditionalJump(Cmpbool, NullUInt64);

			auto OffsetIr = _IR_LookingAtIRBlock->New_Index_Vetor(MallocPtr, Indexir, SizeV);

			//loop on every
			bool IsPrimitiveType = Type_IsPrimitive(Type);

			if (!IsPrimitiveType) 
			{
				Func.ThisPar = Get_FuncInfo::ThisPar_t::PushFromLast;
				_IR_LastExpressionField = OffsetIr;
			}

			IR_Build_FuncCall(Type, Func, ValuePars);

			if (IsPrimitiveType)
			{
				_IR_LookingAtIRBlock->NewDereferenc_Store(OffsetIr, _IR_LastExpressionField);
			}

			_IR_LookingAtIRBlock->New_Increment(Indexir);//index++



			_IR_LookingAtIRBlock->NewJump(JumpLabel);

			size_t ThisJumpLable = _IR_LookingAtIRBlock->GetIndex();
			_IR_LookingAtIRBlock->UpdateConditionaJump(JumpIns, Cmpbool, ThisJumpLable);

			_IR_LastExpressionField = MallocPtr;

			_LookingForTypes.pop();



			_LastExpressionType = Type;
			_LastExpressionType._IsAddressArray =true;
		}
		else
		{
			auto SizeIR = IR_Load_UIntptr(TypeSize);
			auto MallocPtr =  _IR_LookingAtIRBlock->NewMallocCall(SizeIR);
			_IR_LastExpressionField = MallocPtr;
			
			
			//Call ObjectNew
			if (Type_IsPrimitive(Type)) 
			{
				IR_Build_FuncCall(Type, Func, ValuePars);
				_IR_LookingAtIRBlock->NewDereferenc_Store(MallocPtr, _IR_LastExpressionField);
			}
			else
			{
				Func.ThisPar = Get_FuncInfo::ThisPar_t::PushFromLast;
				IR_Build_FuncCall(Type, Func, ValuePars);
			}
			_IR_LastExpressionField = MallocPtr;
			
			_LastExpressionType= Type;
			_LastExpressionType._IsAddress = true;
		}
	
	}

	

}

void SystematicAnalysis::OnAnonymousObjectConstructor(const AnonymousObjectConstructorNode* nod)
{
	auto& Type = Type_Get_LookingForType();

	if (Type._Type == TypesEnum::CustomType)
	{
		auto V = Symbol_GetSymbol(Type).value();
		if (V->Type == SymbolType::Type_StaticArray)
		{
			StaticArrayInfo* StaticArr = V->Get_Info< StaticArrayInfo>();
			if (_PassType == PassType::FixedTypes)
			{

				if (StaticArr->IsCountInitialized)
				{
					if (StaticArr->Count != nod->_Fields._Nodes.size())
					{
						const NeverNullPtr<Token> Token = _LastLookedAtToken.value();
						LogError_CanIncorrectStaticArrCount(Token, Type, nod->_Fields._Nodes.size(), StaticArr->Count);
						_LastExpressionType.SetType(TypesEnum::Null);
						return;
					}
				}
				else
				{
					StaticArr->Count = nod->_Fields._Nodes.size();
					StaticArr->IsCountInitialized = true;
				}

				const auto& ArrItemType = StaticArr->Type;
				for (size_t i = 0; i < nod->_Fields._Nodes.size(); i++)
				{
					OnExpressionTypeNode(nod->_Fields._Nodes[i].get(), GetValueMode::Read);

					if (!Type_CanBeImplicitConverted(_LastExpressionType, ArrItemType, false))
					{
						LogError_CantCastImplicitTypes(_LastLookedAtToken.value(), _LastExpressionType, ArrItemType, false);
					}
				}
			}
			if (_PassType == PassType::BuidCode)
			{
				auto& BufferIR = _IR_IRlocations.top();
				BufferIR.UsedlocationIR = true;
				auto BufferIRIns = BufferIR.Value;

				const auto& ArrItemType = StaticArr->Type;
				const auto IRItemType = IR_ConvertToIRType(ArrItemType);
				UAddress Size;
				Type_GetSize(ArrItemType, Size);
				auto ValueSizeIR = IR_Load_UIntptr(Size);

				if (!Type.IsAddress())
				{
					BufferIRIns = _IR_LookingAtIRBlock->NewLoadPtr(BufferIRIns);
				}

				for (size_t i = 0; i < nod->_Fields._Nodes.size(); i++)
				{
					OnExpressionTypeNode(nod->_Fields._Nodes[i].get(), GetValueMode::Read);

					IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, ArrItemType);

					auto V = _IR_LookingAtIRBlock->New_Index_Vetor(BufferIRIns, IR_Load_UIntptr(i), ValueSizeIR);

					_IR_LookingAtIRBlock->NewDereferenc_Store(V, _IR_LastExpressionField);
				}
			}

			_LastExpressionType = Type;
			return;
		}
	}

	if (Type.IsnotAn(TypesEnum::Var))//function who called this can deal with var
	{
		

		if (_PassType == PassType::FixedTypes)
		{
			auto Func = Type_GetFunc(Type, nod->_Fields);
			_FuncToSyboID[Symbol_GetSymbolID(nod)] = Func;

			Type_SetFuncRetAsLastEx(Func);
			return;
		}
		else if (_PassType == PassType::BuidCode)
		{
			const auto& Func = _FuncToSyboID.at(Symbol_GetSymbolID(nod));
			auto& ValuePars = nod->_Fields;



			IR_Build_FuncCall(Type, Func, ValuePars);


		}
	}
	else
	{
		if (_PassType == PassType::FixedTypes)
		{
			{
				TypeSymbol AnyType; AnyType.SetType(TypesEnum::Any);
				_LookingForTypes.push(AnyType);
			}

			TypeSymbol ArrItemType;
			if (nod->_Fields._Nodes.size())
			{
				OnExpressionTypeNode(nod->_Fields._Nodes[0].get(), GetValueMode::Read);
				ArrItemType = _LastExpressionType;
				_LookingForTypes.top() = ArrItemType;
			}
			else
			{
				ArrItemType.SetType(TypesEnum::uInt8);
			}


			TypeSymbol NewType;
			StaticArrayInfo* info = nullptr;
			{
				SymbolID id = Symbol_GetSymbolID(nod);
				auto FullName = CompilerGenerated("StaticArray_") + ToString(ArrItemType) + std::to_string(id.AsInt());


				auto Syb = &Symbol_AddSymbol(SymbolType::Type_StaticArray, FullName, FullName,AccessModifierType::Public);
				_Table.AddSymbolID(*Syb, id);

				info = new StaticArrayInfo();


				info->Type = ArrItemType;
				info->Exnode = nullptr;

				Syb->Info.reset(info);

				NewType.SetType(id);
			}


			info->Count = nod->_Fields._Nodes.size();
			info->IsCountInitialized = true;

			for (size_t i = 1; i < nod->_Fields._Nodes.size(); i++)
			{
				OnExpressionTypeNode(nod->_Fields._Nodes[i].get(), GetValueMode::Read);
				
				if (!Type_CanBeImplicitConverted(_LastExpressionType, ArrItemType,false))
				{
					LogError_CantCastImplicitTypes(_LastLookedAtToken.value(),_LastExpressionType, ArrItemType, false);
				}
			}

			_LastExpressionType = NewType;
			{
				_LookingForTypes.pop();
			}
			return;
		}
	}

	_LastExpressionType = Type;
}

void SystematicAnalysis::IR_Build_FuncCall(const TypeSymbol& Type, const Get_FuncInfo& Func, const ValueParametersNode& ValuePars)
{
	String B = ToString(Type);
	Token T;
	T.Type = TokenType::Name;
	T.Value._String = B;


	ScopedNameNode Tep;
	ScopedName V;
	V._token = &T;

	Tep._ScopedName.push_back(std::move(V));

	IR_Build_FuncCall(Func, Tep, ValuePars);
}

void SystematicAnalysis::OnReadVariable(const ReadVariableNode& nod)
{

	if (_PassType != PassType::BuidCode) 
	{
		GetMemberTypeSymbolFromVar_t V;
		if (!Symbol_MemberTypeSymbolFromVar(nod._VariableName, V))
		{
			return;
		}
		_LastExpressionType = V.Type;
	}
	

	auto FToken =NeverNullptr(nod._VariableName._ScopedName.front()._token);

	Symbol* Symbol;
	auto Token = NeverNullptr(nod._VariableName._ScopedName.back()._token);
	auto Str = FToken->Value._String;


	bool DoStuff = false;
	if (FToken->Type == TokenType::KeyWord_This)
	{
		if (_ClassStack.size() == 0)
		{
			LogError_CantUseThisKeyWordHereError(FToken);
			return;
		}


		if (!Context_IsInThisFuncCall())
		{
			LogError_CantUseThisInStaticFunction(FToken);
		}

		
		auto& Func = _FuncStack.back();
		auto ObjectType = Func.Pointer->GetObjectForCall();

		auto objecttypesyb = Symbol_GetSymbol(*ObjectType).value();
		ClassInfo* V = objecttypesyb->Get_Info<ClassInfo>();

		
		Symbol = Symbol_GetSymbol(*ObjectType).value_unchecked();
			
		DoStuff = true;
	}
	
	ReadVarErrorCheck_t Info;
	if (DoStuff == false) 
	{
		Symbol = Symbol_GetSymbol(Str, SymbolType::Varable_t).value_unchecked();

		



		if (IsRead(_GetExpressionMode.top()))
		{
			Info = TryLogError_OnReadVar(Str, Token, Symbol);
		}
		if (IsWrite(_GetExpressionMode.top()))
		{
			Symbol->SetTovalid();

		}

		if (Info.CantFindVar)
		{
			return;
		}
		if (Symbol->Type == SymbolType::StackVarable)
		{
			auto Info = Symbol->Get_Info<DeclareVariableInfo>();


			if (Info->LineNumber > FToken->OnPos)
			{
				LogError_UseingVarableBeforDeclared(FToken);
			}
		}
	}

	if (!Info.VarIsInvalid)
	{
		SymbolID sybId = Symbol->ID;
		if (_PassType == PassType::BuidCode)
		{
			FileDependency_AddDependencyToCurrentFile(Symbol);

			auto& LookForT = Type_Get_LookingForType();



			GetMemberTypeSymbolFromVar_t V;
			if (!Symbol_MemberTypeSymbolFromVar(nod._VariableName, V))
			{
				return;
			}


			bool LookIsAddress = LookForT.IsAddress() || LookForT.IsAddressArray();
			bool AmIsAddress = V.Type.IsAddress();

			bool AsPointer = LookForT.IsAddress();

			//

			if (IsWrite(_GetExpressionMode.top()))
			{
				bool DoStore = true;
				if (V._Symbol->Type == SymbolType::ParameterVarable)
				{
					auto ParInfo = V._Symbol->Get_Info<ParameterInfo>();
					if (ParInfo->IsOutValue)
					{
						DoStore = false;
						_IR_LastStoreField = IR_Build_Member_DereferencStore(V);
					}
				}
				

				if (DoStore) {
					_IR_LastStoreField = IR_Build_Member_Store(V, V.Type);
				}
			}

			if (IsRead(_GetExpressionMode.top())) 
			{
				if (LookIsAddress == true && AmIsAddress == true)
				{
					_IR_LastExpressionField = IR_Build_Member_GetValue(V);
				}
				else if (LookIsAddress == false && AmIsAddress == false)
				{
					_IR_LastExpressionField = IR_Build_Member_AsValue(V);
				}
				else if (LookIsAddress == true && AmIsAddress == false)
				{
					_IR_LastExpressionField = IR_Build_Member_AsPointer(V);
				}
				else if (LookIsAddress == false && AmIsAddress == true)
				{
					_IR_LastExpressionField = IR_Build_Member_AsValue(V);
				}
				else
				{
					UCodeLangUnreachable();
				}

			}

			_LastExpressionType = V.Type;
		}
	}

	_LastLookedAtToken = Token.AsNullable();
}

Byte SystematicAnalysis::OperatorPrecedenceValue(const Node* node)
{
	if (node->Get_Type() == NodeType::ValueExpressionNode) 
	{ 
		const ValueExpressionNode* nod = ValueExpressionNode::As(node);
		
		if (nod->_Value->Get_Type() == NodeType::ParenthesesExpresionNode)
		{
			return 8;
		}
	
	}

	if (node->Get_Type() == NodeType::BinaryExpressionNode)
	{
		const BinaryExpressionNode* nod = BinaryExpressionNode::As(node);

		auto V = nod->_BinaryOp->Type;
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

bool SystematicAnalysis::Node_SwapForOperatorPrecedence(const Node* nodeA, const Node* nodeB)
{
	return OperatorPrecedenceValue(nodeA) < OperatorPrecedenceValue(nodeB);
}

void SystematicAnalysis::OnExpressionNode(const BinaryExpressionNode& node)
{
	auto Ex0node = node._Value0._Value.get();
	auto Ex1node = node._Value1._Value.get();

	if (_PassType != PassType::GetTypes && 
		(
			Node_SwapForOperatorPrecedence(Ex0node,Ex1node) && Node_SwapForOperatorPrecedence(&node, Ex1node)//i have no clue why this works
		)
		)
	{
		std::swap(Ex0node, Ex1node);
	}

	

	if (_LookingForTypes.size() && _LookingForTypes.top()._Type != TypesEnum::Var)
	{
		_LookingForTypes.push(_LookingForTypes.top());
	}
	else
	{
		TypeSymbol V; V.SetType(TypesEnum::Any);
		_LookingForTypes.push(V);
	}

	BinaryExpressionNode_Data* Data =nullptr;
	bool BuildCode = _PassType == PassType::BuidCode;
	bool IsBuildFunc = false;
	if (BuildCode)
	{
		Data = &_BinaryExpressionNode_Datas.at(Symbol_GetSymbolID(node));
		_LookingForTypes.top() = Data->Op1;

		if (Data->FuncToCall)
		{
			_LookingForTypes.pop();
			IsBuildFunc = true;
		}
	}


	IRInstruction* Ex0 =nullptr;
	TypeSymbol Ex0Type;

	IRInstruction* Ex1 = nullptr;
	TypeSymbol Ex1Type;
	if (IsBuildFunc == false) 
	{
		OnExpressionTypeNode(Ex1node, GetValueMode::Read);
		Ex0 = _IR_LastExpressionField;
		Ex0Type = _LastExpressionType;

		if (BuildCode)
		{
			_LookingForTypes.top() = Data->Op0;
		}

		OnExpressionTypeNode(Ex0node, GetValueMode::Read);
		Ex1 = _IR_LastExpressionField;
		Ex1Type = _LastExpressionType;


		_LookingForTypes.pop();

		if (_PassType == PassType::FixedTypes)
		{
			auto BinaryOp = NeverNullptr(node._BinaryOp);
			auto Info = Type_HasBinaryOverLoadWith(Ex0Type, BinaryOp->Type, Ex1Type);

			if (!Info.HasValue)
			{
				LogError_CantFindBinaryOpForTypes(BinaryOp, Ex0Type, Ex1Type);
			}

			auto Op = node._BinaryOp->Type;


			BinaryExpressionNode_Data V;
			V.Op0 = Ex0Type;
			V.Op1 = Ex1Type;


			//all float bool int types
			if (Info.Value.has_value())
			{
				FuncInfo* f = Info.Value.value()->Get_Info<FuncInfo>();
				V.Op0 = f->Pars[0].Type;
				V.Op1 = f->Pars[1].Type;
				V.FuncToCall = Info.Value.value();

				_LastExpressionType = f->Ret;
			}
			else
			{
				V.Op0._IsAddress = false;
				V.Op1._IsAddress = false;
				_LastExpressionType = Type_BinaryExpressionShouldRurn(Op, Ex0Type);
			}

			_BinaryExpressionNode_Datas.AddValue(Symbol_GetSymbolID(node), V);


		}
	}
	if (_PassType == PassType::BuidCode)
	{
		auto& Data = _BinaryExpressionNode_Datas.at(Symbol_GetSymbolID(node));

		if (Data.FuncToCall)
		{
			FuncInfo* f = Data.FuncToCall->Get_Info<FuncInfo>();


			Get_FuncInfo V;
			V.Func = f;
			V.SymFunc = Data.FuncToCall;
			V.ThisPar = Get_FuncInfo::ThisPar_t::NoThisPar;
			
			
			ScopedNameNode Tep;
			ScopedName TepV;
			TepV._token = node._BinaryOp;
			Tep._ScopedName.push_back(TepV);

			ValueParametersNode pars;
			pars._Nodes.push_back(Unique_ptr<Node>(Ex0node));
			pars._Nodes.push_back(Unique_ptr<Node>(Ex1node));

			IR_Build_FuncCall(V, Tep,pars);

			//save so not free mem
			auto par0 = pars._Nodes[0].release();
			auto par1 = pars._Nodes[1].release();
			//its ok.no mem leak Par node has Unique_ptr to Ex0 and Ex1 just borrowing them
			_LastExpressionType = V.Func->Ret;
		}
		else 
		{

			auto Type = Ex0Type;
			auto Op = node._BinaryOp->Type;
			_LastExpressionType = Type_BinaryExpressionShouldRurn(Op, Ex0Type);

			auto TypeSybOp = Symbol_GetSymbol(Type);
			if (TypeSybOp)
			{
				auto TypeSyb = TypeSybOp.value();
				if (TypeSyb->Type == SymbolType::Enum)
				{
					Type = TypeSyb->Get_Info<EnumInfo>()->Basetype;
				}
				else
				{
					UCodeLangUnreachable();
				}
			}

#define BindaryBuildU(x) switch (Op) \
		{\
		case TokenType::plus:_IR_LastExpressionField=_IR_LookingAtIRBlock->NewAdd(Ex1, Ex0);break;\
		case TokenType::minus:_IR_LastExpressionField=_IR_LookingAtIRBlock->NewSub(Ex1, Ex0);break;\
		case TokenType::star:_IR_LastExpressionField=_IR_LookingAtIRBlock->NewUMul(Ex1, Ex0);break; \
		case TokenType::forwardslash:_IR_LastExpressionField=_IR_LookingAtIRBlock->NewUDiv(Ex1, Ex0);break; \
		case TokenType::equal_Comparison:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewC_Equalto(Ex1, Ex0); break; \
		case TokenType::Notequal_Comparison:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewC_NotEqualto(Ex1, Ex0);break; \
		case TokenType::greaterthan:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewUGreaterThan(Ex1, Ex0); break; \
		case TokenType::lessthan:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewULessThan(Ex1, Ex0); break; \
		case TokenType::greater_than_or_equalto:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewUGreaterThanOrEqual(Ex1, Ex0); break; \
		case TokenType::less_than_or_equalto:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewULessThanOrEqual(Ex1, Ex0); break; \
		default:\
			UCodeLangUnreachable();\
			break;\
		}\


#define BindaryBuildS(x) switch (Op) \
			{\
			case TokenType::plus:_IR_LastExpressionField=_IR_LookingAtIRBlock->NewAdd(Ex1, Ex0);break;\
			case TokenType::minus:_IR_LastExpressionField=_IR_LookingAtIRBlock->NewSub(Ex1, Ex0);break;\
			case TokenType::star:_IR_LastExpressionField=_IR_LookingAtIRBlock->NewSMul(Ex1, Ex0);break; \
			case TokenType::forwardslash:_IR_LastExpressionField=_IR_LookingAtIRBlock->NewSDiv(Ex1, Ex0);break; \
			case TokenType::equal_Comparison:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewC_Equalto(Ex1, Ex0); break; \
			case TokenType::Notequal_Comparison:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewC_NotEqualto(Ex1, Ex0); break; \
			case TokenType::greaterthan:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewSGreaterThan(Ex1, Ex0); break; \
			case TokenType::lessthan:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewSLessThan(Ex1, Ex0); break; \
			case TokenType::greater_than_or_equalto:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewSGreaterThanOrEqual(Ex1, Ex0); break; \
			case TokenType::less_than_or_equalto:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewSLessThanOrEqual(Ex1, Ex0); break; \
			default:\
				UCodeLangUnreachable(); \
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

			case TypesEnum::uIntPtr:BindaryBuildU(64); break;
			case TypesEnum::sIntPtr:BindaryBuildS(64); break;

			case TypesEnum::Bool:
				switch (Op)
				{
				case TokenType::equal_Comparison:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewC_Equalto(Ex1, Ex0); break;
				case TokenType::Notequal_Comparison:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewC_NotEqualto(Ex1, Ex0); break;
				case TokenType::logical_and:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewlogicalAnd(Ex1, Ex0); break;
				case TokenType::logical_or:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewlogicalOr(Ex1, Ex0); break;
				default:
					UCodeLangUnreachable();
					break;
				}
				break;
			case TypesEnum::Char:
				switch (Op)
				{
				case TokenType::equal_Comparison:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewC_Equalto(Ex1, Ex0); break;
				case TokenType::Notequal_Comparison:_IR_LastExpressionField = _IR_LookingAtIRBlock->NewC_NotEqualto(Ex1, Ex0); break;
				default:
					UCodeLangUnreachable();
					break;
				}
				break;
			default:
				UCodeLangUnreachable();
				break;
			}
		}
	}
}
TypeSymbol SystematicAnalysis::Type_BinaryExpressionShouldRurn(TokenType Op, const TypeSymbol& Ex0Type)
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
	Type_ConvertAndValidateType(node._ToType, ToTypeAs,NodeSyb_t::Any);
	_LookingForTypes.push(ToTypeAs);
	

	if (_PassType == PassType::BuidCode)
	{
		auto& Item =_CastDatas.at(Symbol_GetSymbolID(node));
		if (Item.FuncToCall) 
		{
			_LookingForTypes.push(Item.FuncToCall->Get_Info<FuncInfo>()->Pars[0].Type);

			OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);
			auto Ex0 = _IR_LastExpressionField;
			auto Ex0Type = _LastExpressionType;

			_LookingForTypes.pop();
		}
		else
		{
			OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);
			auto Ex0 = _IR_LastExpressionField;
			auto Ex0Type = _LastExpressionType;
		}
	}
	else
	{
		OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);
		auto Ex0 = _IR_LastExpressionField;
		auto Ex0Type = _LastExpressionType;
	}
	

	if (_PassType == PassType::FixedTypes) 
	{
		if (!ToTypeAs.IsNull()) 
		{
			CastExpressionNode_Data data;


			TypeSymbol ToTypeAs;
			Type_ConvertAndValidateType(node._ToType, ToTypeAs, NodeSyb_t::Any);

			auto Ex0Type = _LastExpressionType;
			auto HasInfo = Type_CanBeExplicitlyConverted(Ex0Type, ToTypeAs);
			if (!HasInfo.HasValue)
			{
				auto  Token = NeverNullptr(node._ToType._name.token);

				LogError_CantCastExplicityTypes(Token, Ex0Type, ToTypeAs);
			}
			else
			{
				if (HasInfo.Value.has_value())
				{
					data.FuncToCall = HasInfo.Value.value();
					_LastExpressionType = HasInfo.Value.value()->Get_Info<FuncInfo>()->Ret;
				}
				else
				{
					_LastExpressionType = ToTypeAs;
				}

			}


			_CastDatas.AddValue(Symbol_GetSymbolID(node), data);
		}
	}

	if (_PassType == PassType::BuidCode)
	{
		IR_Build_ExplicitConversion(_IR_LastExpressionField, _LastExpressionType, ToTypeAs,_CastDatas.at(Symbol_GetSymbolID(node)));

		_LastExpressionType = ToTypeAs;
	}
	_LookingForTypes.pop();
}
void SystematicAnalysis::OnExpressionNode(const IndexedExpresionNode& node)
{
	if (_LookingForTypes.size() && _LookingForTypes.top()._Type == TypesEnum::Var)
	{
		TypeSymbol V; V.SetType(TypesEnum::Any);
		_LookingForTypes.push(V);
	}
	else
	{
		_LookingForTypes.push(_LookingForTypes.top());
	}

	if (_PassType == PassType::GetTypes)
	{
		OnExpressionTypeNode(node._SourceExpression._Value.get(), _GetExpressionMode.top());
		OnExpressionTypeNode(node._IndexExpression._Value.get(), GetValueMode::Read);
	}

	if (_PassType == PassType::FixedTypes)
	{

		OnExpressionTypeNode(node._SourceExpression._Value.get(), _GetExpressionMode.top());
		TypeSymbol SourcType = _LastExpressionType;

		
		TypeSymbol gesstype;
		gesstype.SetType(TypesEnum::Any);


		bool IsSrcAddress = SourcType.IsAddress();
		bool IsSrcStaticArray = Type_IsStaticArray(SourcType);

		if (IsSrcAddress || IsSrcStaticArray)
		{

			gesstype.SetType(TypesEnum::uIntPtr);
		}


		_LookingForTypes.push(gesstype);


		OnExpressionTypeNode(node._IndexExpression._Value.get(), GetValueMode::Read);
		TypeSymbol IndexType = _LastExpressionType;


		_LookingForTypes.pop();

		auto HasInfo = Type_HasIndexedOverLoadWith(SourcType, IndexType);
		if (!HasInfo.HasValue)
		{
			auto  Token = _LastLookedAtToken;
			LogError_CantBeIndexWithType(Token.value(), SourcType, IndexType);
		}



		


		IndexedExpresion_Data V;
		


		//all float bool int types
		if (HasInfo.Value.has_value())
		{
			FuncInfo* f = HasInfo.Value.value()->Get_Info<FuncInfo>();

			V.Op0 = f->Pars[0].Type;
			V.Op1 = f->Pars[1].Type;
			V.FuncToCall = HasInfo.Value.value();

			_LastExpressionType = f->Ret;
		}
		else
		{
			TypeSymbol lookingfor = _LookingForTypes.top();
			V.Op0 = SourcType;
			V.Op1 = IndexType;
			V.Op0._IsAddress = true;
			V.Op1._IsAddress = false;

			if (IsSrcAddress)
			{
				lookingfor = SourcType;
				_LastExpressionType = lookingfor;
			}
			else if (IsSrcStaticArray)
			{
				auto Syb = Symbol_GetSymbol(SourcType).value();
				lookingfor = Syb->Get_Info<StaticArrayInfo>()->Type;
				lookingfor.SetAsAddress();

				_LastExpressionType = lookingfor;
			}
			else
			{
				lookingfor = SourcType;
				lookingfor.SetAsAddress();
				lookingfor._IsAddressArray = false;

				_LastExpressionType = lookingfor;
			}
		}

			

		_IndexedExpresion_Datas.AddValue(Symbol_GetSymbolID(node),V);

	}


	if (_PassType == PassType::BuidCode)
	{
		auto& Data = _IndexedExpresion_Datas.at(Symbol_GetSymbolID(node));

		if (Data.FuncToCall)
		{
			FuncInfo* f =Data.FuncToCall->Get_Info<FuncInfo>();

			Get_FuncInfo V;
			V.Func = f;
			V.SymFunc = Data.FuncToCall;
			V.ThisPar = Get_FuncInfo::ThisPar_t::NoThisPar;


			ScopedNameNode Tep;
			ScopedName TepV;
			TepV._token = _LastLookedAtToken.value_unchecked();
			Tep._ScopedName.push_back(TepV);

			ValueParametersNode pars;
			pars._Nodes.push_back(Unique_ptr<Node>(node._SourceExpression._Value.get()));
			pars._Nodes.push_back(Unique_ptr<Node>(node._IndexExpression._Value.get()));

			IR_Build_FuncCall(V, Tep, pars);

			//save so not free mem
			auto par0 = pars._Nodes[0].release();
			auto par1 = pars._Nodes[1].release();
			//its ok.no mem leak Par node has Unique_ptr to SourceExpression and IndexExpression just borrowing them
			_LastExpressionType = V.Func->Ret;
		}
		else
		{
			_LookingForTypes.push(Data.Op0);
			OnExpressionTypeNode(node._SourceExpression._Value.get(), GetValueMode::Read);
			_LookingForTypes.pop();

			auto Pointer = _IR_LastExpressionField;
			auto SourcType = _LastExpressionType;

			_LookingForTypes.push(Data.Op1);
			OnExpressionTypeNode(node._IndexExpression._Value.get(), GetValueMode::Read);
			_LookingForTypes.pop();

			auto IndexField = _IR_LastExpressionField;

			TypeSymbol IndexedObjectPointer = Data.Op0;
			IndexedObjectPointer._IsAddressArray = false;
			IndexedObjectPointer._IsAddress = false;

			UAddress V;
			{
				auto VSybOp = Symbol_GetSymbol(IndexedObjectPointer);
				if (VSybOp)
				{
					auto VSyb = VSybOp.value();
					if (VSyb->Type == SymbolType::Type_StaticArray)
					{
						StaticArrayInfo* info = VSyb->Get_Info<StaticArrayInfo>();
						Type_GetSize(info->Type, V);
					}
					else
					{
						Type_GetSize(IndexedObjectPointer, V);
					}
				}
				else
				{
					Type_GetSize(IndexedObjectPointer, V);
				}


			}
			
			

			_IR_LastExpressionField = _IR_LookingAtIRBlock->New_Index_Vetor(Pointer, IndexField,_IR_LookingAtIRBlock->NewLoad(V));

			TypeSymbol lookingfor = _LookingForTypes.top();
			{
				if (lookingfor.IsAddressArray())
				{
					lookingfor = SourcType;
					_LastExpressionType = lookingfor;
				}
				else
				{
					lookingfor = SourcType;
					lookingfor.SetAsAddress();
					lookingfor._IsAddressArray = false;

					_LastExpressionType = lookingfor;
				}
			}

			_LastExpressionType = lookingfor;

			if (IsWrite(_GetExpressionMode.top()))
			{
				_IR_LastStoreField =IROperator(_IR_LastExpressionField);
			}
		}

	}

	_LookingForTypes.pop();
}
void SystematicAnalysis::OnFuncCallNode(const FuncCallNode& node)
{
	
	if (_PassType == PassType::FixedTypes)
	{
		if (!_FuncToSyboID.HasValue(Symbol_GetSymbolID(node))) 
		{

			auto Info = Type_GetFunc(node._FuncName, node.Parameters, Type_Get_LookingForType());

			if (Info.SymFunc)
			{
				FileDependency_AddDependencyToCurrentFile(Info.SymFunc);
			}

			_FuncToSyboID.AddValue(Symbol_GetSymbolID(node), std::move(Info));

			Type_SetFuncRetAsLastEx(Info);
		}
	}
	else if (_PassType == PassType::BuidCode)
	{
		auto& SybID = _FuncToSyboID.at(Symbol_GetSymbolID(node));
		IR_Build_FuncCall(SybID, node._FuncName, node.Parameters);
	}
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
}
void SystematicAnalysis::OnDropStatementNode(const DropStatementNode& node)
{
	if (_PassType == PassType::GetTypes)
	{
		if (!IsInUnSafeBlock())
		{
			auto Token = node._KeywordToken;
			LogError(ErrorCodes::ExpectingSequence, Token->OnLine, Token->OnPos, "Cant use 'drop' keyword in safe mode.");
		}
	}

	if (_PassType == PassType::BuidCode)
	{

		auto TypeToPush = TypeSymbol();
		TypeToPush.SetAsAddress();

		_LookingForTypes.push(TypeToPush);
	}

	OnExpressionTypeNode(node._expression._Value.get(), GetValueMode::Read);

	if (_PassType == PassType::BuidCode)
	{
		_LookingForTypes.pop();
	}

	auto Ex0 = _IR_LastExpressionField;
	auto Ex0Type = _LastExpressionType;
	if (_PassType == PassType::FixedTypes)
	{

		if (!(Ex0Type.IsAddress() || Ex0Type.IsAddressArray() || Ex0Type._IsDynamic))
		{
			auto Token = _LastLookedAtToken.value();
			LogError_ExpressionMustbeAnLocationValueError(Token, Ex0Type);
		}
	}
	if (_PassType == PassType::BuidCode)
	{
		
		bool TypeHaveDestructor = Symbol_HasDestructor(Ex0Type);
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
					Type_GetSize(UintptrType, UintptrSize);
				}
				UAddress TypeObjectSize;
				TypeSymbol TypeOfArr;
				{
					TypeOfArr = Ex0Type;
					TypeOfArr._IsAddressArray = false;
					Type_GetSize(TypeOfArr, TypeObjectSize);
				}

				auto StartArrPointer = _IR_LookingAtIRBlock->NewLoad(Ex0);
				IR_Build_Decrement_uIntPtr(Ex0, UintptrSize);//get the size the object
				auto ArrSize = _IR_LookingAtIRBlock->NewLoad_Dereferenc(Ex0,IR_ConvertToIRType(UintptrType));




				//Call default on every

				//our index
				auto Indexir = IR_Load_UIntptr(0);

				size_t JumpLabel = _IR_LookingAtIRBlock->GetIndex();
				auto Cmpbool = _IR_LookingAtIRBlock->NewC_Equalto(Indexir, Ex0);


				auto JumpIns = _IR_LookingAtIRBlock->NewConditionalJump(Cmpbool, NullUInt64);

				auto OffsetIr = _IR_LookingAtIRBlock->New_Index_Vetor(StartArrPointer, Indexir, IR_Load_UIntptr(TypeObjectSize));

				//loop on every
				bool IsPrimitiveType = Type_IsPrimitive(TypeOfArr);


				{
					ObjectToDrop Data;
					Data._Object = OffsetIr;
					Data.Type = TypeOfArr;
					Data.DropType = ObjectToDropType::IRInstructionNoMod;
					IR_Build_DestructorCall(Data);//call on Object
				}


				_IR_LookingAtIRBlock->New_Increment(Indexir);//index++



				_IR_LookingAtIRBlock->NewJump(JumpLabel);

				size_t ThisJumpLable = _IR_LookingAtIRBlock->GetIndex();
				_IR_LookingAtIRBlock->UpdateConditionaJump(JumpIns, Cmpbool, ThisJumpLable);


				_IR_LookingAtIRBlock->NewFreeCall(Ex0);
			}
			else
			{
				_IR_LookingAtIRBlock->NewFreeCall(Ex0);
			}

			Ex0Type._IsAddressArray = true;
		}
		else
		{

			if (Ex0Type._IsDynamic)
			{

				auto ExPtr = Ex0Type.IsAddress() ? Ex0 : _IR_LookingAtIRBlock->NewLoadPtr(Ex0);

				auto IRStructType = IR_ConvertToIRType(Ex0Type);

				auto PtrMember = _IR_LookingAtIRBlock->New_Member_Dereference(ExPtr, IRStructType, 0);
				auto VTableMember = _IR_LookingAtIRBlock->New_Member_Dereference(ExPtr, IRStructType, 1);

				auto Trait = Symbol_GetSymbol(Ex0Type).value();
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
				auto DestructorFuncPtr = _IR_LookingAtIRBlock->New_Member_Dereference(VTableMember,IRType(_IR_Builder.ToID(Str_GetTraitVStructTableName(Trait->FullName))), Index);

				_IR_LookingAtIRBlock->NewPushParameter(PtrMember);
				_IR_LookingAtIRBlock->NewCallFuncPtr(DestructorFuncPtr);

				_IR_LookingAtIRBlock->NewFreeCall(PtrMember);
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
					IR_Build_DestructorCall(Data);//call on Object
				}

				_IR_LookingAtIRBlock->NewFreeCall(Ex0);
			}
			
		}
	}
}
void SystematicAnalysis::Assembly_ConvertAttributes(const Vector<Unique_ptr<AttributeNode>>& nodes, Vector<UsedTagValueData>& Out)
{
	Out.resize(nodes.size());
	for (size_t i = 0; i < nodes.size(); i++)
	{
		Assembly_ConvertAttribute(*nodes[i], Out[i]);
	}
}
void SystematicAnalysis::Assembly_ConvertAttribute(const AttributeNode& nodes, UsedTagValueData Out)
{
	auto Syb = Symbol_GetSymbol(Symbol_GetSymbolID(nodes));
	Out.TypeID = Type_GetTypeID(TypesEnum::CustomType,Syb->VarType._CustomTypeSymbol);
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

			Assembly_LoadLibSymbols(*Item,Mode);
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
		Assembly_LoadClassSymbol(*GlobalObject,Scope, Scope, Mode);
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
			Assembly_LoadClassSymbol(Item->Get_ClassData(),FullName, Scope, Mode);
		}
		break;
		case ClassType::Alias:
		{

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

			InfoItem.Name =FieldItem.Name;
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
				auto& FieldSyb = Symbol_AddSymbol(SymbolType::Enum_Field, ValueItem.Name,ScopeHelper::ApendedStrings(FullName,ValueItem.Name), AccessModifierType::Public);
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

		Assembly_LoadType(Item.BaseType,enumInfo->Basetype);
		
		
		
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
void SystematicAnalysis::Assembly_LoadSymbol(const ClassMethod& Item, SystematicAnalysis::LoadLibMode Mode)
{
	if (Mode == LoadLibMode::GetTypes)
	{	
		auto Name =ScopeHelper::GetNameFromFullName(Item.FullName);
		auto& Syb = Symbol_AddSymbol(SymbolType::Func, Name, _Table._Scope.GetApendedString(Name), AccessModifierType::Public);
		_Table.AddSymbolID(Syb, Symbol_GetSymbolID(&Item));
		Syb.OutputIR = false;
		Syb.PassState = PassType::BuidCode;
		auto Funcinfo =new FuncInfo();
		Syb.Info.reset(Funcinfo);

		Funcinfo->FullName = Syb.FullName;
		Funcinfo->FrontParIsUnNamed = Item.IsThisFuncion;
		
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
NullablePtr<Symbol> SystematicAnalysis::Symbol_GetSymbol(String_view Name, SymbolType Type)
{
	auto& Symbols = _Table.GetSymbolsWithName(Name,Type);
	auto Symbol = Symbols.size() ? Symbols[0] : nullptr;

	if (Symbol && Symbol->Type == SymbolType::ParameterVarable)
	{
		for (auto& Item : Symbols)
		{
			ParameterInfo* p = Item->Get_Info<ParameterInfo>();
			if (p->MyFunc == Context_GetCuruntFunc().value().value())
			{
				return Item;
			}
		}
	}

	return Symbol;
}
const NullablePtr<Symbol> SystematicAnalysis::Symbol_GetSymbol(String_view Name, SymbolType Type) const
{
	auto& Symbols = _Table.GetSymbolsWithName(Name, Type);
	auto Symbol = Symbols.size() ? Symbols[0] : nullptr;
	return Nullableptr(Symbol);
}
void SystematicAnalysis::OnInvalidNode(const InvalidNode& node)
{
	if (_PassType == PassType::GetTypes){return;}

	if (node._StringExpression._Value)
	{
		auto StrData = Eval_GetStrEVal(node._StringExpression._Value.get());

		if (StrData.IsConstantExpression) 
		{
			auto Token =NeverNullptr(node._KeyWord);
			LogError_LogInvalidNodeError(Token, StrData.Msg);
		}
		else
		{

			TypeSymbol B = TypesEnum::Char;
			B.SetAsAddressArray();

			LogError_Eval_CantCastImplicitTypes(_LastLookedAtToken.value(), _LastExpressionType,B);
		}
	}
	else
	{
		auto Token = NeverNullptr(node._KeyWord);
		LogError_EmptyInvalidError(Token);
	}
}
void SystematicAnalysis::OnvalidNode(const ValidNode& node)
{
	if (_PassType == PassType::GetTypes) 
	{

		if (node._IsExpression)
		{
			OnExpressionTypeNode(node._ExpressionToCheck._Value.get(), GetValueMode::Read);
		}
		else
		{
			for (auto& Item : node._StatementToCheck._Nodes)
			{
				OnStatement(*Item);
			}
		}
		_LastExpressionType = TypesEnum::Bool;
	}
	if (_PassType == PassType::FixedTypes)
	{
		bool IsValid = true;
	 	auto ErrCount = _ErrorsOutput->Get_Errors().size();

		if (node._IsExpression)
		{
			OnExpressionTypeNode(node._ExpressionToCheck._Value.get(), GetValueMode::Read);
		}
		else
		{
			for (auto& Item : node._StatementToCheck._Nodes)
			{
				OnStatement(*Item);
			}
		}

		if (ErrCount < _ErrorsOutput->Get_Errors().size())
		{
			size_t CountToPop = _ErrorsOutput->Get_Errors().size() - ErrCount;
			for (size_t i = 0; i < CountToPop; i++)
			{
				_ErrorsOutput->Get_Errors().pop_back();
			}
			IsValid = false;
		}

	
		_LastExpressionType = TypesEnum::Bool;

		_ValidNodes.AddValue(Symbol_GetSymbolID(node), IsValid);
	}
	if (_PassType == PassType::BuidCode)
	{
		bool IsValid = _ValidNodes.at(Symbol_GetSymbolID(node));


		_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad(IsValid);
		_LastExpressionType = TypesEnum::Bool;
	}
}
void SystematicAnalysis::OnExpressionNode(const ExtendedScopeExpression& node)
{
	if (_PassType == PassType::GetTypes)
	{
		_LookingForTypes.push(TypesEnum::Any);
		OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);
		_LookingForTypes.pop();
	}
	if (_PassType == PassType::FixedTypes)
	{
		_LookingForTypes.push(TypesEnum::Any);
		OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);
		_LookingForTypes.pop();

		auto ExpressionType = _LastExpressionType;
	
		GetMemberTypeSymbolFromVar_t V;
		

		_GetExpressionMode.push(_GetExpressionMode.top());
		{
			V.Type = ExpressionType;
			V._Symbol = Symbol_GetSymbol(ExpressionType).value_unchecked();

		
			Symbol_StepGetMemberTypeSymbolFromVar(node._Extended, 0, node._Operator, V);
		
			for (size_t i = 1; i < node._Extended._ScopedName.size(); i++)
			{
				Symbol_StepGetMemberTypeSymbolFromVar(node._Extended, i, node._Extended._ScopedName[i]._operator, V);
			}
		}
		_GetExpressionMode.pop();

		_LastExpressionType = V.Type;
	}
	if (_PassType == PassType::BuidCode)
	{
		_LookingForTypes.push(TypesEnum::Any);
		OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);
		_LookingForTypes.pop();


		auto ExpressionType = _LastExpressionType;
		auto ExpressionIR = _IR_LastExpressionField;

		GetMemberTypeSymbolFromVar_t V;
		V.Start = node._Extended._ScopedName.data();
		V.End = node._Extended._ScopedName.size();

		_GetExpressionMode.push(_GetExpressionMode.top());
		{
			V.Type = ExpressionType;
			V._Symbol = Symbol_GetSymbol(ExpressionType).value_unchecked();


			Symbol_StepGetMemberTypeSymbolFromVar(node._Extended, 0, node._Operator, V);

			for (size_t i = 1; i < node._Extended._ScopedName.size(); i++)
			{
				Symbol_StepGetMemberTypeSymbolFromVar(node._Extended, i, node._Extended._ScopedName[i]._operator, V);
			}
		}
		_GetExpressionMode.pop();

		auto GetValue = _GetExpressionMode.top();

		IRInstruction* VPtr= ExpressionIR;


		
		StepBuildMember_Access(node._Extended._ScopedName[0], ExpressionType, node._Operator, V, VPtr);
		for (size_t i = 1; i < node._Extended._ScopedName.size(); i++)
		{
			StepBuildMember_Access(node._Extended._ScopedName[i], ExpressionType, node._Operator, V, VPtr);
		}

		if (IsRead(GetValue)) 
		{
			_IR_LastExpressionField = VPtr;
		}
		
		if (IsWrite(GetValue))
		{
			_IR_LastStoreField = VPtr;
		}


		_LastExpressionType = V.Type;
	}
}
void SystematicAnalysis::OnExpressionNode(const ExtendedFuncExpression& node)
{
	if (_PassType == PassType::GetTypes)
	{
		_LookingForTypes.push(TypesEnum::Any);
		OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);
		_LookingForTypes.pop();
	}
	if (_PassType == PassType::FixedTypes)
	{
		_LookingForTypes.push(TypesEnum::Any);
		OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);
		_LookingForTypes.pop();

		auto ExpressionType = _LastExpressionType;

		_GetExpressionMode.push(_GetExpressionMode.top());
		{

			TypeSymbol TypeToStart = ExtendedFuncExpressionGetTypeToStart(ExpressionType, node);

			//Boring boiler plate for Tep ScopedNameNode should be the same in  Evaluate(EvaluatedEx& Out, const ExtendedFuncExpression& node)
			const Token& ToGetLinesFrom = *node._Extended._FuncName._ScopedName.begin()->_token;

			ScopedNameNode Name;
			
			ScopedName TepV;
			

			Token TepToken; 

			TepToken.OnLine = ToGetLinesFrom.OnLine;
			TepToken.OnPos = ToGetLinesFrom.OnPos;
			TepToken.Type = TokenType::Name;

			Type_RemoveTypeattributes(TypeToStart);

			String Buffer= ToString(TypeToStart);

			TepToken.Value._String= Buffer;
			
			TepV._token = &TepToken;
			

			TepV._operator = ScopedName::Operator_t::ScopeResolution;
			Name._ScopedName.push_back(std::move(TepV));
			


			
			{
			
				{
					auto Copy = node._Extended._FuncName._ScopedName.back();
					Name._ScopedName.push_back(std::move(Copy));
				}
				for (size_t i = 1; i < node._Extended._FuncName._ScopedName.size(); i++)
				{
					auto& Item = node._Extended._FuncName._ScopedName[i];
					auto Copy = Item;
					Name._ScopedName.push_back(std::move(Copy));
				}
			}
			//

			ValueParametersNode Pars;
			Pars._Nodes.push_back(Unique_ptr<Node>(node._Expression._Value.get()));

			for (size_t i = 0; i < node._Extended.Parameters._Nodes.size(); i++)
			{
				auto& Item = node._Extended.Parameters._Nodes[i];
				Pars._Nodes.push_back(Unique_ptr<Node>(Item.get()));
			}

			auto FuncInfo = Type_GetFunc(Name,Pars, Type_Get_LookingForType());
		
		
			for (auto& Item : Pars._Nodes)
			{
				auto Node = Item.release();//is ok it was borrwed.
			}
		

			_FuncToSyboID.AddValue(Symbol_GetSymbolID(node), FuncInfo);


			Type_SetFuncRetAsLastEx(FuncInfo);
		}
		_GetExpressionMode.pop();

	}
	if (_PassType == PassType::BuidCode)
	{
		auto& FuncInfo = _FuncToSyboID.at(Symbol_GetSymbolID(node));



		ValueParametersNode Pars;
		Pars._Nodes.push_back(Unique_ptr<Node>(node._Expression._Value.get()));

		for (size_t i = 0; i < node._Extended.Parameters._Nodes.size(); i++)
		{
			auto& Item = node._Extended.Parameters._Nodes[i];
			Pars._Nodes.push_back(Unique_ptr<Node>(Item.get()));
		}
		IR_Build_FuncCall(FuncInfo, {}, Pars);


		for (auto& Item : Pars._Nodes)
		{
			auto Node = Item.release();//is ok it was borrwed.
		}
	}
}
TypeSymbol SystematicAnalysis::ExtendedFuncExpressionGetTypeToStart(const TypeSymbol& ExpressionType, const ExtendedFuncExpression& node)
{

	if (node._Operator == ScopedName::Operator_t::Dot)
	{
		return ExpressionType;
	}
	else
	{
		return TypesEnum::Null;
	}

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
		
		const NeverNullPtr<Token> Token = NeverNullptr(node.TypeOp._name.token); 
		LogError_CantOutputTypeinfo(Token);
	}
}

void SystematicAnalysis::OnExpressionToTypeValueNode(const ExpressionToTypeValueNode& node)
{
	if (_PassType == PassType::GetTypes)
	{
		_LookingForTypes.push(TypesEnum::Any);
		OnExpressionTypeNode(node._TypeEx,GetValueMode::Read);
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
void SystematicAnalysis::OnAwaitExpression(const AwaitExpression& node)
{
	if (_PassType == PassType::GetTypes)
	{
		if (node._IsFunc)
		{
			const FuncCallNode& Funcnode = node._Func;
			OnFuncCallNode(Funcnode);
		}
		else
		{
			const LambdaNode& LambdaNode = node._Lambda;
			OnLambdaNode(LambdaNode);
		}
	}
	else if (_PassType == PassType::FixedTypes)
	{
		TypeSymbol FuncRetType;
		if (node._IsFunc)
		{
			const FuncCallNode& Funcnode = node._Func;
			OnFuncCallNode(Funcnode);

			const auto& GetFunc = _FuncToSyboID.at(Symbol_GetSymbolID(Funcnode));

			Type_SetFuncRetAsLastEx(GetFunc);
			FuncRetType = _LastExpressionType;
			
		}
		else
		{
			const LambdaNode& LambdaNode = node._Lambda;
			OnLambdaNode(LambdaNode);


			const String LambdaName = CompilerGenerated("Lambda") + std::to_string((uintptr_t)&LambdaNode);
			_Table.AddScope(LambdaName);

			auto& LambdaSym = *Symbol_GetSymbol(Symbol_GetSymbolID(LambdaNode));
			LambdaInfo* Info = LambdaSym.Get_Info<LambdaInfo>();
			FuncRetType = Info->Ret;

			_Table.RemoveScope();
		}

		auto AsFuture = Type_MakeFutureFromType(FuncRetType);

		_LastExpressionType = AsFuture;
	}
	else if (_PassType == PassType::BuidCode)
	{
		TypeSymbol FuncRetType;

		if (node._IsFunc)
		{
			const FuncCallNode& Funcnode = node._Func;
			const auto& GetFunc = _FuncToSyboID.at(Symbol_GetSymbolID(Funcnode));

			thread_local int a = 0; a++;
			auto NewAwaitSetIRName = _FuncStack.begin()->Pointer->FullName + "awaitset_" + std::to_string(a);
			{
				auto newawaitsetfunc = _IR_Builder.NewFunc(NewAwaitSetIRName, IRType(IRTypes::Void));
				auto newawaitsetblock = newawaitsetfunc->NewBlock(".");
				IRPar awaitpar;
				awaitpar.identifier = _IR_Builder.ToID("await_handle");
				awaitpar.type = IRTypes::pointer;
				newawaitsetfunc->Pars.push_back(std::move(awaitpar));

				IRInstruction* AwaitHandleIR = newawaitsetblock->NewLoad(&newawaitsetfunc->Pars.back());

				IRInstruction* funcv = newawaitsetblock->NewCall(IR_GetIRID(GetFunc.Func));

				newawaitsetblock->New_Await_SetValue(AwaitHandleIR, funcv);
				newawaitsetblock->NewRet();
			}
			

			Type_SetFuncRetAsLastEx(GetFunc);
			FuncRetType = _LastExpressionType;

			IRInstruction* funcptr = _IR_LookingAtIRBlock->NewLoadFuncPtr(_IR_Builder.ToID(NewAwaitSetIRName));
			
			auto awaittask = _IR_LookingAtIRBlock->New_Await_Task(funcptr);

			

			_IR_LastExpressionField = awaittask;
		}
		else
		{
			const LambdaNode& LambdaNode = node._Lambda;

			const String LambdaName = CompilerGenerated("Lambda") + std::to_string((uintptr_t)&LambdaNode);
			_Table.AddScope(LambdaName);

			auto& LambdaSym = *Symbol_GetSymbol(Symbol_GetSymbolID(LambdaNode));
			LambdaInfo* Info = LambdaSym.Get_Info<LambdaInfo>();
			FuncRetType = Info->Ret;

			_Table.RemoveScope();


			IRInstruction* funcptr = _IR_LookingAtIRBlock->NewLoadFuncPtr(IR_GetIRID(Context_GetCuruntFunc().value().value()));

			auto awaittask = _IR_LookingAtIRBlock->New_Await_Task(funcptr);

			_IR_LookingAtIRBlock->New_Await_RunTask(awaittask);


			_IR_LastExpressionField = awaittask;
		}

		auto AsFuture = Type_MakeFutureFromType(FuncRetType);

		_IR_LastExpressionField = MakeFutureFromHandle(AsFuture, _IR_LastExpressionField);
		_LastExpressionType = AsFuture;
	}
}
void SystematicAnalysis::OnYieldExpression(const YieldExpression& node)
{
	if (_PassType == PassType::GetTypes)
	{
		OnExpressionTypeNode(node._Expression,GetValueMode::Read);
	}
	else if (_PassType == PassType::FixedTypes)
	{
		OnExpressionTypeNode(node._Expression, GetValueMode::Read);
		auto extype = _LastExpressionType;
		

		if (!Type_IsFuture(extype))
		{
			auto token = NeverNullptr(node._Token);
			LogError_yieldnotAsync(token);

			_LastExpressionType = TypesEnum::Null;
		}
		else
		{
			_LastExpressionType = Type_GetBaseFromFuture(extype);
		}
		SetAfterYeld();
	}
	else if (_PassType == PassType::BuidCode)
	{
		thread_local int a = 0;

		auto NewYieldIRName = _FuncStack.begin()->Pointer->FullName + "yield_" + std::to_string(a);
		auto NewAwaitCompleteIRName = _FuncStack.begin()->Pointer->FullName + "awaitComplete_" + std::to_string(a);



		OnExpressionTypeNode(node._Expression, GetValueMode::Read);

		auto extype = _LastExpressionType;
		auto base = Type_GetBaseFromFuture(extype);


		auto FutureIR = _IR_LastExpressionField;
		auto AwaitHandleIR =GetFutureHandle(extype,FutureIR);
		_IR_LookingAtIRBlock->New_Await_OnComplete(AwaitHandleIR, _IR_LookingAtIRBlock->NewLoadFuncPtr(_IR_Builder.ToID(NewAwaitCompleteIRName)));
		_IR_LookingAtIRBlock->New_Await_RunTask(AwaitHandleIR);

		bool isbasevoid = base._Type == TypesEnum::Void;
		{
			auto newawaitsetfunc = _IR_Builder.NewFunc(NewAwaitCompleteIRName, IRType(IRTypes::Void));
			auto newawaitsetblock = newawaitsetfunc->NewBlock(".");
			IRPar awaitpar;
			awaitpar.identifier = _IR_Builder.ToID("await_handle");
			awaitpar.type = IRTypes::pointer;
			newawaitsetfunc->Pars.push_back(std::move(awaitpar));


			if (!isbasevoid)
			{
				IRInstruction* AwaitHandleIR = newawaitsetblock->NewLoad(&newawaitsetfunc->Pars.back());

				IRInstruction* awaitretv = newawaitsetblock->New_Await_GetValue(AwaitHandleIR, IR_ConvertToIRType(base));

				newawaitsetblock->NewPushParameter(awaitretv);
			}
			IRInstruction* funcv = newawaitsetblock->NewCall(_IR_Builder.ToID(NewYieldIRName));

			newawaitsetblock->NewRet();
		}
		_IR_LookingAtIRBlock->NewRetValue(FutureIR);
		_IR_LookingAtIRBlock->NewRet();


		a++;
		auto newfunc = _IR_Builder.NewFunc(NewYieldIRName, IR_ConvertToIRType(base));

		if (!isbasevoid)
		{
			IRPar p;
			p.identifier = _IR_Builder.ToID("yieldvalue");
			p.type = IR_ConvertToIRType(base);
			newfunc->Pars.push_back(std::move(p));
		}

		auto newblock = newfunc->NewBlock(".");





		_IR_LookingAtIRFunc = newfunc;
		_IR_LookingAtIRBlock = newblock;

		SetAfterYeld();
		if (base._Type == TypesEnum::Void)
		{
			_IR_LastExpressionField = nullptr;
		}
		else
		{
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad(&newfunc->Pars.front());
		}
		_LastExpressionType = base;



	}
}

void SystematicAnalysis::OnAwaitStatement(const AwaitStatement& node)
{
	OnAwaitExpression(node._Base);
}
void SystematicAnalysis::OnYieldStatement(const YieldStatement& node)
{
	OnYieldExpression(node._Base);
}
void SystematicAnalysis::OnUnsafeStatement(const UnsafeStatementsNode& node)
{
	size_t ScopeCounter = 0;
	const String ScopeName = std::to_string((uintptr_t)&node);

	_Table.AddScope(ScopeName + std::to_string(ScopeCounter));

	auto& block = _FuncStack.front().BlockConexts.front();
	auto oldblock = block.IsUnSafeBlock;

	block.IsUnSafeBlock = true;
	for (auto& Item : node._Base._Nodes)
	{
		OnStatement(*Item);
	}

	block.IsUnSafeBlock = oldblock;

	_Table.RemoveScope();
}
void SystematicAnalysis::OnUnsafeExpression(const UnsafeExpression& node)
{
	auto& block = _FuncStack.front().BlockConexts.front();
	auto oldblock = block.IsUnSafeBlock;
	block.IsUnSafeBlock = true;
	OnExpressionTypeNode(node._Base, _GetExpressionMode.top());
	block.IsUnSafeBlock = oldblock;

}
TypeSymbol SystematicAnalysis::Type_MakeFutureFromType(const TypeSymbol& BaseType)
{
	auto symbolsOp = Symbol_GetSymbol(UCode_FutureType,SymbolType::Generic_class);
	
	auto token = _LastLookedAtToken.value();
	if (symbolsOp)
	{
		auto symbols = symbolsOp.value();
		if (symbols->Type != SymbolType::Generic_class
			//&& symbols->Type != SymbolType::Generic_Alias
			//&& symbols->Type != SymbolType::Generic_Enum
			)
		{
			LogError_ExpectedSymbolToBea(token, *symbols, SymbolType::Generic_class);
			return {};
		}

		if (symbols->Type == SymbolType::Generic_class)
		{
			auto CInfo = symbols->Get_Info<ClassInfo>();
			auto classnode = ClassNode::As(symbols->Get_NodeInfo<Node>());

			if (classnode->_generic._Values.size() == 1)
			{
				auto symfullname = Generic_SymbolGenericFullName(symbols, { BaseType });

				if (auto val = Symbol_GetSymbol(symfullname, SymbolType::Type))
				{
					return TypeSymbol(val.value()->ID);
				}
				else
				{
					Generic_TypeInstantiate(symbols, { BaseType });
					return  TypeSymbol(Symbol_GetSymbol(symfullname, SymbolType::Type).value()->ID);
				}

			}
			else
			{
				LogError_CanIncorrectGenericCount(token, UCode_FutureType, 1, classnode->_generic._Values.size());
			}
		}
		else
		{
			UCodeLangUnreachable();
		}
	}
	else
	{
		LogError_CantFindTypeError(token, UCode_FutureType);
	}

	return {};
}
bool SystematicAnalysis::Type_IsFuture(const TypeSymbol& Future)
{
	auto future = Symbol_GetSymbol(Future);

	if (future) {
		auto v = ScopeHelper::GetNameFromFullName(future.value()->FullName);
		if (StringHelper::StartWith(v, UCode_FutureType))
		{
			return true;
		}
	}

	return false;
}
TypeSymbol SystematicAnalysis::Type_GetBaseFromFuture(const TypeSymbol& Future)
{
	UCodeLangAssert(Type_IsFuture(Future));

	auto symname = ScopeHelper::ApendedStrings(Symbol_GetSymbol(Future).value()->FullName,"T");
	auto sym = Symbol_GetSymbol(symname,SymbolType::Type).value();
	return sym->VarType;
}
IRInstruction* SystematicAnalysis::GetFutureHandle(const TypeSymbol& Future, IRInstruction* IR)
{
	return IR;
}
IRInstruction* SystematicAnalysis::MakeFutureFromHandle(const TypeSymbol& Future, IRInstruction* IR)
{
	return IR;
}
void SystematicAnalysis::OnMatchStatement(const MatchStatement& node)
{
	if (_PassType == PassType::GetTypes)
	{
		OnExpressionTypeNode(node._Expression,GetValueMode::Read);

		size_t ScopeCounter = 0;
		const String ScopeName = std::to_string((uintptr_t)&node);

		for (auto& Item : node._Arms)
		{

			_Table.AddScope(ScopeName + std::to_string(ScopeCounter));

			OnExpressionTypeNode(Item._Expression, GetValueMode::Read);
			
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
			
			Type_CanMatch(ToMatchType,node._Expression, Item._Expression,V.ArmData);

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

		MatchStatementData& V = _MatchStatementDatas.at(Symbol_GetSymbolID(node));
	
		
		BuildMatch_State State;

		for (size_t i = 0; i < node._Arms.size(); i++)
		{
			auto& Item = node._Arms[i];

			auto& VItem = V.ArmData.Arms[i];

			_Table.AddScope(ScopeName + std::to_string(ScopeCounter));

			auto V = IR_Build_Match(ToMatchType,node._Expression,Ex, State,VItem,Item._Expression);

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
void SystematicAnalysis::Type_CanMatch(const TypeSymbol& MatchItem,const ExpressionNodeType& MatchValueNode, const ExpressionNodeType& node, MatchArmData& Data)
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

			OnExpressionTypeNode(Item._AssignmentExpression,GetValueMode::Read);
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
			TryError_AllValuesAreMatched(ToMatchType,V.ArmData);
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

		MatchExpressionData& V = _MatchExpressionDatas.at(Symbol_GetSymbolID(node));

		

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

			IR_Build_Match(V2,State);

			State.MatchList.push_back(std::move(V2));

			_Table.RemoveScope();

			ScopeCounter++;
		}

		if (node._InvaidCase.has_value())
		{
			_Table.AddScope(ScopeName + std::to_string(ScopeCounter));

			auto V2 =IR_Build_InvaildMatch(ToMatchType, Ex, State);

			OnExpressionTypeNode(node._InvaidCase.value(), GetValueMode::Read);

			IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, V._MatchAssignmentType);

			_IR_LookingAtIRBlock->NewStore(OutEx, _IR_LastExpressionField);

			IR_Build_Match(V2, State);

			State.MatchList.push_back(std::move(V2));

			_Table.RemoveScope();
		}

		IR_Build_MatchState(State);

		_LookingForTypes.pop();

		_IR_LastExpressionField = OutEx;
	}
}
String SystematicAnalysis::Str_GetFuncAnonymousObjectFullName(const String& FullFuncName)
{
	return FullFuncName + "!";
}
void SystematicAnalysis::Assembly_AddClass(const Vector<Unique_ptr<AttributeNode>>& attributes, const NeverNullPtr<Symbol> ClassSyb)
{
	const ClassInfo* Class = ClassSyb->Get_Info<ClassInfo>();
	Class_Data& VClass = _Lib.Get_Assembly().AddClass((String)Class->Get_Name(), Class->FullName);
	
	TypeSymbol AsType = TypeSymbol(ClassSyb->ID);

	VClass.Size = 0;
	VClass.TypeID = Type_GetTypeID(AsType._Type, AsType._CustomTypeSymbol);
	
	for (const auto& node : Class->Fields)
	{
		auto& Item = VClass.Fields.emplace_back();

		UAddress Size;
		Type_GetSize(node.Type, Size);
	
		Item.Name =ScopeHelper::GetNameFromFullName(node.Name);
		Item.Type = Assembly_ConvertToType(node.Type);
		Item.offset = VClass.Size;
		VClass.Size += Size;
	}
	
	for (const auto& Trait : Class->_InheritedTypes)
	{
		auto Typeid = Type_GetTypeID(TypesEnum::CustomType, Trait.Syb->ID);
		
		InheritedTrait_Data Item;
		Item.TraitID = Typeid;
		VClass.InheritedTypes.push_back(std::move(Item));
	}
	Assembly_ConvertAttributes(attributes,VClass.Attributes.Attributes);
}

ReflectionTypeInfo SystematicAnalysis::Assembly_ConvertToType(const TypeSymbol& Type)
{
	ReflectionTypeInfo r;
	r._Type = Type._Type;
	r._CustomTypeID =Type_GetTypeID(Type._Type, Type._CustomTypeSymbol);
	

	r._IsAddress= Type.IsAddress();
	r._IsAddressArray= Type.IsAddressArray();
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




				size_t TypeSize = 0;
				for (auto& Field : Sym->Get_Info<ClassInfo>()->Fields)
				{
					TypeSize += Type_GetSize(Field.Type).value();
				}
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

bool AreSameimmutable(const TypeSymbol& TypeA, const TypeSymbol& TypeB)
{
	return 	TypeA.IsAddress() == TypeB.IsAddress() &&
		TypeA.IsAddressArray() == TypeB.IsAddressArray() &&
		TypeA.Isimmutable() == TypeB.Isimmutable();
}
bool SystematicAnalysis::Type_AreTheSame(const TypeSymbol& TypeA, const TypeSymbol& TypeB)
{
	return Type_AreTheSameWithOutimmutable(TypeA, TypeB) && AreSameimmutable(TypeA, TypeB);
}
bool SystematicAnalysis::Type_AreTheSame(const ParInfo& TypeA, const ParInfo& TypeB)
{
	if (TypeA.IsOutPar != TypeB.IsOutPar)
	{
		return false;
	}

	return Type_AreTheSame(TypeA.Type, TypeB.Type);
}
bool SystematicAnalysis::Type_AreTheSameWithOutimmutable(const TypeSymbol& TypeA, const TypeSymbol& TypeB)
{
	if (TypeA.IsAddressArray() != TypeB.IsAddressArray())
	{
		return false;
	}
	if (TypeA._MoveData != TypeB._MoveData)
	{
		return false;
	}
	if (TypeA.IsTypeInfo() != TypeB.IsTypeInfo())
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

		Symbol& TypeOne = *Symbol_GetSymbol(TypeA).value();
		Symbol& TypeTwo = *Symbol_GetSymbol(TypeB).value();
		if (TypeOne.Type == SymbolType::Func_ptr && TypeTwo.Type == SymbolType::Func_ptr)
		{
			FuncPtrInfo* F1 = TypeOne.Get_Info<FuncPtrInfo>();
			FuncPtrInfo* F2 = TypeTwo.Get_Info<FuncPtrInfo>();
			if (F1->Pars.size() != F2->Pars.size())
			{
				return false;
			}
			if (!Type_AreTheSameWithOutimmutable(F1->Ret, F2->Ret))
			{
				return false;
			}
		

			for (size_t i = 0; i < F1->Pars.size(); i++)
			{
				auto& Item1 = F1->Pars[i];
				auto& Item2 = F2->Pars[i];
				if (!Type_AreTheSameWithOutimmutable(Item1.Type, Item2.Type) || Item1.IsOutPar != Item2.IsOutPar)
				{
					return false;
				}
			}
			return true;
		}
		if (TypeOne.Type == SymbolType::Type_StaticArray && TypeTwo.Type == SymbolType::Type_StaticArray)
		{
			StaticArrayInfo* F1 = TypeOne.Get_Info<StaticArrayInfo>();
			StaticArrayInfo* F2 = TypeTwo.Get_Info<StaticArrayInfo>();

			if (F1->Count == F2->Count)
			{
				return Type_AreTheSameWithOutimmutable(F1->Type, F2->Type);
			}
		}

	}
	else if ((Type_IsPrimitive(TypeA) && Type_IsPrimitive(TypeB)) && TypeA._Type == TypeB._Type
		)
	{
		return true;
	}



	return false;
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
				auto& Syb = _Table.GetSymbolsWithName(Node->FullName).front();
				Out.SetType(Syb->ID);
			}
				break;
			case ClassType::Alias:
			{
				auto& Syb = _Table.GetSymbolsWithName(Node->FullName).front();
				Out.SetType(Syb->ID);
			}
			break;
			case ClassType::Enum:
			{
				auto& Syb = _Table.GetSymbolsWithName(Node->FullName).front();
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
SystematicAnalysis::BinaryOverLoadWith_t SystematicAnalysis::Type_HasBinaryOverLoadWith(const TypeSymbol& TypeA, TokenType BinaryOp, const TypeSymbol& TypeB)
{

	if (Type_AreTheSameWithOutimmutable(TypeA, TypeB))
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

		if (Type_IsIntType(TypeA))
		{
			return { IsMathOp || IsSameValueComparisonOp || IsMathValueComparisonOp || Isbitwise ,{} };
		}
		if (Type_IsfloatType(TypeA))
		{
			return { IsMathOp || IsSameValueComparisonOp || IsMathValueComparisonOp,{} };
		}

		if (Type_IsCharType(TypeA))
		{
			return { IsSameValueComparisonOp, {} };
		}
		if (TypeA._Type == TypesEnum::Bool)
		{
			return { IsSameValueComparisonOp || IslogicalOperator , {} };
		}

		if (TypeA._Type == TypesEnum::CustomType)
		{
			auto Syb = Symbol_GetSymbol(TypeA);
			if (Syb.value()->Type == SymbolType::Enum) 
			{
				return { IsSameValueComparisonOp, {} };
			}
		}

	}

	auto SybOp = Symbol_GetSymbol(TypeA);
	if (SybOp)
	{
		auto Syb = SybOp.value();
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
								bool r = Type_CanBeImplicitConverted(TypeA, funcInfo->Pars[0].Type)
									&& Type_CanBeImplicitConverted(TypeB, funcInfo->Pars[1].Type);
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
SystematicAnalysis::CompoundOverLoadWith_t SystematicAnalysis::Type_HasCompoundOverLoadWith(const TypeSymbol& TypeA, TokenType BinaryOp, const TypeSymbol& TypeB)
{
	if (Type_AreTheSameWithOutimmutable(TypeA, TypeB))
	{
		if (Type_IsIntType(TypeA))
		{
			return { true };
		}
	}

	auto SybOp = Symbol_GetSymbol(TypeA);
	if (SybOp)
	{
		auto Syb = SybOp.value();
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
								bool r = Type_CanBeImplicitConverted(TypeA, funcInfo->Pars[0].Type)
									 && Type_CanBeImplicitConverted(TypeB, funcInfo->Pars[1].Type);
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
SystematicAnalysis::PostFixOverLoadWith_t SystematicAnalysis::Type_HasPostfixOverLoadWith(const TypeSymbol& TypeA, TokenType BinaryOp)
{

	if (Type_IsIntType(TypeA))
	{
		return { true,{} };
	}

	auto SybOp = Symbol_GetSymbol(TypeA);
	if (SybOp)
	{
		auto Syb = SybOp.value();
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
								bool r = Type_CanBeImplicitConverted(TypeA, funcInfo->Pars[0].Type);
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
SystematicAnalysis::IndexOverLoadWith_t SystematicAnalysis::Type_HasIndexedOverLoadWith(const TypeSymbol& TypeA, const TypeSymbol& TypeB)
{

	if (Type_IsUIntType(TypeB)) 
	{
		if (TypeA.IsAddressArray())
		{
			return { true, {} };
		}

		if (Type_IsStaticArray(TypeA))
		{
			return { true,{} };
		}
	}

	auto SybOp = Symbol_GetSymbol(TypeA);
	if (SybOp)
	{
		auto Syb = SybOp.value();
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
						bool r = Type_CanBeImplicitConverted(TypeA, funcInfo->Pars[0].Type)
							&& Type_CanBeImplicitConverted(TypeB, funcInfo->Pars[1].Type);
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
SystematicAnalysis::ForOverLoadWith_t SystematicAnalysis::Type_HasForOverLoadWith(const TypeSymbol& TypeA)
{
	auto SybOp = Symbol_GetSymbol(TypeA);
	if (SybOp)
	{
		auto Syb = SybOp.value();
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
						bool r = Type_CanBeImplicitConverted(TypeA, funcInfo->Pars[0].Type);
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
SystematicAnalysis::UrinaryOverLoadWith_t SystematicAnalysis::Type_HasUrinaryOverLoadWith(const TypeSymbol& TypeA, TokenType Op)
{
	auto SybOp = Symbol_GetSymbol(TypeA);
	if (SybOp)
	{
		auto Syb = SybOp.value();
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
								bool r = Type_CanBeImplicitConverted(TypeA, funcInfo->Pars[0].Type);
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
		r = "umut ";
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
			r += ToString(Syb.VarType,Info->Ex) + ")";
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
TypeSymbolID SystematicAnalysis::Type_GetTypeID(TypesEnum Type, SymbolID SymbolId)
{
	TypeSymbolID R = 0;
	switch (Type)
	{
	case TypesEnum::Void:
		R = 1;
		break;
	case TypesEnum::Bool:
		R = 2;
		break;
	case TypesEnum::Char:
		R = 3;
		break;
	case TypesEnum::uInt8:
		R = 4;
		break;
	case TypesEnum::uInt16:
		R = 5;
		break;
	case TypesEnum::uInt32:
		R = 6;
		break;
	case TypesEnum::uInt64:
		R = 7;
		break;
	case TypesEnum::sInt8:
		R = 8;
		break;
	case TypesEnum::sInt16:
		R = 9;
		break;
	case TypesEnum::sInt32:
		R = 10;
		break;
	case TypesEnum::uIntPtr:
		R = 11;
		break;
	case TypesEnum::sIntPtr:
		R = 12;
		break;
	case TypesEnum::float32:
		R = 13;
		break;
	case TypesEnum::float64:
		R = 14;
		break;
	case TypesEnum::CustomType:
	{
		auto Syb = Symbol_GetSymbol(SymbolId);
		R = (ReflectionCustomTypeID)std::hash<String>()(Syb->FullName);
	}
	break;
	default:
		R = 0;
		break;
	}
	return R;
}
void SystematicAnalysis::Type_Convert(const TypeNode& V, TypeSymbol& Out)
{
	switch (V._name.token->Type)
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
	case TokenType::KeyWord_uft8:
		Out.SetType(TypesEnum::Uft8);
		break;
	case TokenType::KeyWord_uft16:
		Out.SetType(TypesEnum::Uft16);
		break;
	case TokenType::KeyWord_uft32:
		Out.SetType(TypesEnum::Uft32);
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
			Symbol* SybV = _ClassStack.top().Syb;
			if (SybV->Type == SymbolType::Generic_class)
			{
				Out = Type_GetUnMapType();
			}
			else 
			{
				Out = SybV->VarType;
			}
		}
		else
		{
			LogError_CantUseThisHere(NeverNullptr(V._name.token));
			Out.SetType(TypesEnum::Null);
		}
	}
	break;
	case TokenType::Name: 
	{
		if (_PassType == PassType::GetTypes) { return; }
		auto Name = V._name.AsStringView();
		Symbol* SybV;
		_LastLookedAtToken =Nullableptr(V._name.token);
		if (V._generic._Values.size())
		{
			auto Val = Generic_InstantiateOrFindGenericSymbol(NeverNullptr(V._name.token), V._generic, Name);
			if (!Val.has_value()) { return; }
			SybV = Val.value().value();
		}
		else
		{
			SybV = Symbol_GetSymbol(Name, SymbolType::Type).value_unchecked();
		}

		if (SybV == nullptr)
		{
			auto Token = V._name.token;
			
			LogError_CantFindTypeError(NeverNullptr(Token), Name);
		}
		else
		{
			if (Symbol_AccessCheck(SybV, NeverNullptr(V._name.token)))
			{
				Out.SetType(TypesEnum::Null);
				return;
			}


			
			if (SybV->Type == SymbolType::Type_alias || SybV->Type == SymbolType::Func_ptr)
			{
				Symbol_Update_Sym_ToFixedTypes(SybV);
				Out = SybV->VarType;
			}
			else if (SybV->Type == SymbolType::Hard_Type_alias
				|| SybV->Type == SymbolType::Enum
				|| SybV->Type == SymbolType::Type_class
				|| SybV->Type == SymbolType::Hard_Func_ptr)
			{
				Symbol_Update_Sym_ToFixedTypes(SybV);
				Out.SetType(SybV->ID);
			}
			else if (SybV->Type == SymbolType::Trait_class)
			{
				if (!V._IsDynamic)
				{
					auto Token = V._name.token;
					LogError_TraitCantBeAlone(NeverNullptr(Token));
					Out.SetType(TypesEnum::Null);
				}
				else
				{
					Symbol_Update_Sym_ToFixedTypes(SybV);
					Out.SetType(SybV->ID);
				}
			}
			else if (SybV->Type == SymbolType::Unmaped_Generic_Type)
			{
				Out.SetType(SybV->ID);
			}
			else
			{
				LogError_WantedAType(V, SybV);
				Out.SetType(TypesEnum::Null);
			}
		}
	}break;
	case TokenType::internal_Constant_expression:
	{
		//note this can only happen in a generic substitution
		auto* node = V._node.get();
		if (_ConstantExpressionMap.HasValue(node))
		{
			auto& item = _ConstantExpressionMap.at(node);
			Out.SetType(item);
		}
		else
		{
			SymbolID id = Symbol_GetSymbolID(node);
			auto& Syb = Symbol_AddSymbol(SymbolType::ConstantExpression, "?", "?",AccessModifierType::Private);
			_Table.AddSymbolID(Syb, id);

			ConstantExpressionInfo* info = new ConstantExpressionInfo();
			info->Exnode = ExpressionNodeType::As(node);
			info->Conext = Save_SymbolContext();

			_LookingForTypes.push(TypesEnum::Any);

			auto IsOk = Eval_EvaluateToAnyType(*info->Exnode);

			_LookingForTypes.pop();
			
			if (IsOk) 
			{
				info->Ex = IsOk.value().EvaluatedObject;
				Syb.VarType = IsOk.value().Type;
			}

			Syb.Info.reset(info);


			Out.SetType(id);
		}
	}
	break;
	case TokenType::KeyWord_TypeInfo:
	{
		Out.SetType(TypesEnum::Var);
		Out.SetAsTypeInfo();
	}
	break;
	case TokenType::KeyWord_bind: 
	{
		if (_PassType != PassType::GetTypes)
		{
			auto ExNode = ExpressionNodeType::As(V._node.get());

			
			auto IsOk = Eval_EvaluateToAnyType(*ExNode);

			if (IsOk.has_value())
			{
				auto& Object = IsOk.value();

				if (!Object.Type.IsTypeInfo())
				{
					Out.SetType(TypesEnum::Null);

					auto Token = NeverNullptr(V._name.token);
					auto& Type = Object.Type;
					LogError_CantBindTypeItNotTypeInfo(Token, Type);
				}
				else 
				{
					auto* TypeSyb = Eval_Get_ObjectAs<TypeSymbol>(Object);
					Out = *TypeSyb;
					Out.BindType();
				}
			}
			else
			{
				Out.SetType(TypesEnum::Null);
			}
		}
		else
		{
			Out.SetType(TypesEnum::Null);
		}

	}break;
	case TokenType::internal_InlineEnumVariant:
	{
		const auto& TypeList = V._generic._Values;
		Vector<TypeSymbol> outtypelist;
		outtypelist.resize(TypeList.size());

		bool fail = false;
		for (size_t i = 0; i < TypeList.size(); i++)
		{
			auto& typenode = TypeList[i];
			auto& outype = outtypelist[i];
			Type_Convert(typenode, outype);
			if (!Type_ValidateType(outype, NeverNullptr(V._name.token),NodeSyb_t::Any) || outype.IsBadType())
			{
				fail = true;
				break;
			}
		}

		if (fail)
		{
			Out.SetType(TypesEnum::Null);
		}
		else
		{

			bool hassametype = false;
			TypeSymbol* sametype = nullptr;
			for (auto& Item : outtypelist)
			{
				for (auto& Item2 : outtypelist)
				{
					if (&Item2 != &Item)
					{
						if (Type_AreTheSameWithOutimmutable(Item, Item2))
						{
							sametype = &Item;
							hassametype = true;
							break;
						}

					}
				}
				if (hassametype)
				{
					break;
				}
			}

			if (hassametype)
			{
				Out.SetType(TypesEnum::Null);

				String MSG;
				MSG += "has multiple of exact same type of '";
				MSG += ToString(*sametype);
				MSG += "'";
				LogError(ErrorCodes::InValidType, MSG,NeverNullptr(V._name.token));
			}
			else
			{
				std::sort(outtypelist.begin(), outtypelist.end(), [this](TypeSymbol& A, TypeSymbol& B)
					{
						auto aid = Type_GetTypeID(A._Type, A._CustomTypeSymbol);
						auto bid = Type_GetTypeID(B._Type, B._CustomTypeSymbol);
						return aid > bid;
					});
				String SymName = CompilerGeneratedStart;
				SymName += "inlineenum_";
				for (auto& Item : outtypelist)
				{
					SymName += ToString(Item);
					SymName += "_";
				}
				SymName += CompilerGeneratedEnd;
			

				//This may causes type depemdency problems between
				if (auto oldsyb = Symbol_GetSymbol(SymName,SymbolType::Enum))
				{
					Out.SetType(oldsyb.value()->ID);
				}
				else
				{
					auto& Syb = Symbol_AddSymbol(SymbolType::Enum, SymName, SymName, AccessModifierType::Public);
					{
						Syb.PassState = PassType::FixedTypes;
						_Table.AddSymbolID(Syb, Symbol_GetSymbolID(&Syb));

						EnumInfo* info = new EnumInfo();
						Syb.Info.reset(info);

						info->FullName = SymName;
						info->Basetype = TypesEnum::uInt8;
						if (outtypelist.size() > UInt8_MaxSize)
						{
							LogError(ErrorCodes::InValidType, "bad inline enum more then 255 values",NeverNullptr(V._name.token));
						}

						Byte EnumValue = 0;
						for (auto& Item : outtypelist)
						{
							EnumFieldInfo F;

							String valuename;
							switch (Item._Type)
							{
							case TypesEnum::Bool:valuename = "Bool"; break;
							case TypesEnum::Char:valuename = "Char"; break;

							case TypesEnum::Uft8:valuename = "Uft8"; break;
							case TypesEnum::Uft16:valuename = "Uft16"; break;
							case TypesEnum::Uft32:valuename = "Uft32"; break;
							case TypesEnum::sInt8:valuename = "Int8"; break;
							
							case TypesEnum::sInt16:valuename = "Int16"; break;
							case TypesEnum::sInt32:valuename = "Int32"; break;
							case TypesEnum::sInt64:valuename = "Int64"; break;
							case TypesEnum::sIntPtr:valuename = "IntPtr"; break;

							case TypesEnum::uInt8:valuename = "UInt8"; break;
							case TypesEnum::uInt16:valuename = "UInt16"; break;
							case TypesEnum::uInt32:valuename = "UInt32"; break;
							case TypesEnum::uInt64:valuename = "UInt64"; break;
							case TypesEnum::uIntPtr:valuename = "UIntPtr"; break;
							case TypesEnum::float32:valuename = "Float32"; break;
							case TypesEnum::float64:valuename = "Float64"; break;
							case TypesEnum::CustomType:
							{
								auto syb = Symbol_GetSymbol(Item);
								valuename = ScopeHelper::GetNameFromFullName(syb.value()->FullName);
							}
							break;
							
							default:
								UCodeLangUnreachable();
								break;
							}

							F.Ex.ObjectSize =sizeof(EnumValue);
							F.Ex.Object_AsPointer.reset(new Byte[sizeof(EnumValue)]);
							memcpy(F.Ex.Object_AsPointer.get(), &EnumValue, sizeof(EnumValue));
							
							F.Name = valuename;

							info->Fields.push_back(std::move(F));
							EnumValue++;
						}
						
						EnumVariantData Variantdata;
						for (auto& Item : outtypelist)
						{
							EnumVariantFeild F;
							F.Types.push_back(Item);

							Variantdata.Variants.push_back(std::move(F));
						}

						info->VariantData = std::move(Variantdata);

						_InlineEnums.push_back(NeverNullptr(&Syb));
					}
						
					Out.SetType(Syb.ID);
				}
			}
		}

	}break;
	default:
		UCodeLangUnreachable();
		break;
	}
	if (V._IsAddess) {Out._IsAddress = true;}
	if (V._IsAddessArray){Out._IsAddressArray = true;}
	if (V._Isimmutable){Out._Isimmutable = true;}

	if (V._IsTypedMoved) { Out._MoveData = MoveData::Moved; }

	if (V._IsDynamic)
	{
		auto syb = Symbol_GetSymbol(Out);
		bool IsTrait = false;
		if (syb)
		{
			IsTrait = syb.value()->Type==SymbolType::Trait_class;
		}

		if (!IsTrait)
		{
			LogError_DynamicMustBeRrait(V, Out);
			Out.SetType(TypesEnum::Null);
		}
		Out._IsDynamic = true;
	}

	if (V._IsStackArray)
	{
		ExpressionNodeType* node = (ExpressionNodeType*)V._node.get();
		SymbolID id = Symbol_GetSymbolID(node);
		auto BaseTypeName = ToString(Out);
		auto FullName = CompilerGenerated("StaticArray_") + BaseTypeName + std::to_string(id.AsInt());

		if (_PassType == PassType::GetTypes)
		{
			return;
		}

		Symbol* Syb = Symbol_GetSymbol(FullName,SymbolType::Null).value_unchecked();
		if (Syb == nullptr)
		{
			SymbolID id = Symbol_GetSymbolID(node);
			Syb = &Symbol_AddSymbol(SymbolType::Type_StaticArray, FullName, FullName,AccessModifierType::Public);
			_Table.AddSymbolID(*Syb, id);

			StaticArrayInfo* info = new StaticArrayInfo();


			info->Type = Out;
			info->Exnode = node;
			
			Syb->Info.reset(info);

		}
		StaticArrayInfo& Info = *Syb->Get_Info<StaticArrayInfo>();

		if (_PassType == PassType::FixedTypes && node != nullptr && Info.IsCountInitialized == false)
		{
			TypeSymbol UIntType;
			UIntType.SetType(TypesEnum::uIntPtr);

		
			_LookingForTypes.push(UIntType);
			auto IsOk = Eval_Evaluate(UIntType,*node);
			_LookingForTypes.pop();

			if (IsOk)
			{
				void* V = Eval_Get_Object(IsOk.value());

				Info.Count = *(size_t*)V;
				Info.IsCountInitialized = true;
			}
			else
			{
				Info.Count = 0;
				Info.IsCountInitialized = true;
			}
		}
		Out.SetType(Syb->ID);
	}
}

NullablePtr<Symbol> SystematicAnalysis::Generic_InstantiateOrFindGenericSymbol(const NeverNullPtr<Token> Token, const UseGenericsNode& GenericsVals, const String_view& Name)
{
	auto SybVOp = Symbol_GetSymbol(Name, SymbolType::Generic_class);
	if (SybVOp == nullptr)
	{
		LogError_CantFindTypeError(Token, Name);
		return {};
	}

	auto SybV = SybVOp.value();
	if (SybV->Type != SymbolType::Generic_class
		&& SybV->Type != SymbolType::Generic_Alias
		&& SybV->Type != SymbolType::Generic_Enum)
	{
		LogError_ExpectedSymbolToBea(Token, *SybV, SymbolType::Generic_class);
		return {};
	}

	NullablePtr<Symbol> R = nullptr;

	Symbol_Update_Sym_ToFixedTypes(SybV);

	if (SybV->Type == SymbolType::Generic_class)
	{
		auto CInfo = SybV->Get_Info<ClassInfo>();
		auto classnode = ClassNode::As(SybV->Get_NodeInfo<Node>());

		R = Generic_InstantiateOrFindGeneric_Class(Token, SybV, classnode->_generic, CInfo->_GenericData, GenericsVals);
	}
	else if (SybV->Type == SymbolType::Generic_Alias)
	{
		auto CInfo = SybV->Get_Info<Generic_AliasInfo>();
		auto classnode = AliasNode::As(SybV->Get_NodeInfo<Node>());
		R = Generic_InstantiateOrFindGeneric_Alias(Token, SybV, classnode->Generic, CInfo->_GenericData, GenericsVals);
	}
	else if (SybV->Type == SymbolType::Generic_Enum)
	{
		auto CInfo = SybV->Get_Info<EnumInfo>();
		auto classnode = EnumNode::As(SybV->Get_NodeInfo<Node>());
		R = Generic_InstantiateOrFindGeneric_Enum(Token, SybV, classnode->_generic, CInfo->_GenericData, GenericsVals);
	}

	return R;
}


TypeSymbol SystematicAnalysis::Type_GetStaticArrayType(const TypeSymbol& BaseType,size_t Size)
{
	auto BaseTypeName = ToString(BaseType);
	auto FullName = CompilerGenerated("StaticArray_") + BaseTypeName + std::to_string(Size);
	auto Syb = Symbol_GetSymbol(FullName,SymbolType::Type);
	if (Syb == nullptr)
	{
		Syb = &Symbol_AddSymbol(SymbolType::Type_StaticArray, FullName, FullName, AccessModifierType::Public);
		SymbolID id = Symbol_GetSymbolID(Syb.value().value());
		_Table.AddSymbolID(*Syb.value(), id);

		StaticArrayInfo* info = new StaticArrayInfo();


		info->Type = BaseType;
		info->Count = Size;

		Syb.value()->Info.reset(info);
	}
	return TypeSymbol(Syb.value()->ID);
}

inline IRInstruction* SystematicAnalysis::IR_RawObjectDataToCString(const RawEvaluatedObject& EvalObject)
{
	String Str = String(String_view((const char*)EvalObject.Object_AsPointer.get(), EvalObject.ObjectSize));
	String_view Buffer{ Str.data(),Str.size() + 1 };//for null char

	auto BufferIR = _IR_Builder.FindOrAddStaticCString(Buffer);
	return _IR_LookingAtIRBlock->NewLoadPtr(BufferIR);
}



void SystematicAnalysis::Type_ConvertAndValidateType(const TypeNode& V, TypeSymbol& Out,NodeSyb_t Syb)
{
	Type_Convert(V, Out);
	if (Type_ValidateType(Out, NeverNullptr(V._name.token),Syb) == false)
	{
		Out.SetType(TypesEnum::Null);
	}
	if (Syb == NodeSyb_t::Parameter)
	{
		Out.SetAsLocation();
	}
}
bool SystematicAnalysis::Type_ValidateType(const TypeSymbol& V, const NeverNullPtr<Token> Token,NodeSyb_t Syb)
{
	if (V._Type == TypesEnum::CustomType)
	{
		auto Syb2 = Symbol_GetSymbol(V).value();
		if (Syb2->Type == SymbolType::Type_StaticArray)
		{
			StaticArrayInfo* V = Syb2->Get_Info<StaticArrayInfo>();

			if (!V->IsCountInitialized)
			{
				LogError_BeMoreSpecifiicWithStaticArrSize(Token, V->Type);
				return false;
			}
		}
		if (Syb2->Type == SymbolType::ConstantExpression && Syb != NodeSyb_t::Any)
		{
			auto V = Syb2->FullName;
			LogError_CantFindTypeError(Token,V);
		}
	}


	if (V._MoveData == MoveData::Moved && Syb != NodeSyb_t::Parameter)
	{
		LogError_CantUseMoveTypeHere(Token);
	}

	return true;
}



TypeSymbol SystematicAnalysis::Type_ConvertAndValidateType(const TypeNode& V,NodeSyb_t Syb)
{
	TypeSymbol r;
	Type_ConvertAndValidateType(V, r,Syb);
	return r;
}


void SystematicAnalysis::Symbol_RedefinitionCheck(const NeverNullPtr<Symbol> Syb, const NeverNullPtr<Token> Value)
{
	auto other = Symbol_GetSymbol(Syb->FullName,Syb->Type);
	if (other)
	{
		LogError_SymbolRedefinition(Value,Syb);
	}
}
void SystematicAnalysis::Symbol_RedefinitionCheck(const String_view FullName, SymbolType Type, const NeverNullPtr<Token> Value)
{
	auto other = Symbol_GetSymbol(FullName,Type);
	if (other)
	{
		LogError_SymbolRedefinition(Value, other.value());
	}
}
void SystematicAnalysis::Symbol_RedefinitionCheck(const NeverNullPtr<Symbol> Syb, const FuncInfo* Fvalue, const NeverNullPtr<Token> Value)
{
	auto other = Symbol_GetSymbol(Syb->FullName, Syb->Type).value();
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

					if (!Type_AreTheSameWithOutimmutable(V1.Type, V2.Type) || V1.IsOutPar != V2.IsOutPar)
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
			LogError_SymbolRedefinition(Value, Syb);
		}
	}
}

bool SystematicAnalysis::Type_CanDoTypeToTrait(const TypeSymbol& TypeToCheck, const TypeSymbol& Type)
{
	if (Type._IsDynamic)
	{
		auto SybolB = Symbol_GetSymbol(Type).value();
		TraitInfo* Info = SybolB->Get_Info<TraitInfo>();
		auto TypeSybOp = Symbol_GetSymbol(TypeToCheck);

		if (TypeSybOp)
		{
			auto TypeSyb = TypeSybOp.value();
			if (TypeSyb->Type == SymbolType::Type_class)
			{
				ClassInfo* ClassF = TypeSyb->Get_Info<ClassInfo>();

				auto Indexo = ClassF->Get_InheritedTypesIndex(SybolB.value());

				if (Indexo.has_value())
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool SystematicAnalysis::Type_IsValid(TypeSymbol& Out)
{
	return false;
}
bool SystematicAnalysis::Type_CanBeImplicitConverted(const TypeSymbol& TypeToCheck, const TypeSymbol& Type, bool ReassignMode )
{
	if (Type_AreTheSameWithOutimmutable(TypeToCheck, Type)) 
	{ 
		bool V0 =Type_IsimmutableRulesfollowed(TypeToCheck, Type);

		bool V1 = Type_IsAddessAndLValuesRulesfollowed(TypeToCheck, Type, ReassignMode);

		return V0 && V1;
	}
	
	
	if (Type_CanDoTypeToTrait(TypeToCheck, Type))
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
				return { true};
			}
		}

	}

	if (Type_IsIntType(TypeToCheck) && Type_IsIntType(Type)) { return { true }; }

	auto Syb = Symbol_GetSymbol(TypeToCheck);
	if (Syb)
	{
		if (Syb.value()->Type == SymbolType::Type_class)
		{

			String funcName = Syb.value()->FullName;
			ScopeHelper::GetApendedString(funcName, Overload_Cast_Func);

			auto& V = _Table.GetSymbolsWithName(funcName, SymbolType::Func);

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

			auto& V = _Table.GetSymbolsWithName(funcName, SymbolType::Func);

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
	return {nullptr};
}
bool SystematicAnalysis::IR_Build_ImplicitConversion(IRInstruction* Ex, const TypeSymbol ExType, const TypeSymbol& ToType)
{

	if (Type_AreTheSame(ExType, ToType))
	{
		return true;
	}

	if (Type_CanDoTypeToTrait(ExType,ToType))
	{
		auto IRType = IR_ConvertToIRType(ToType);
		IRInstruction* structV = _IR_LookingAtIRBlock->NewLoad(IRType);

		IRStruct* IRStructPtr = _IR_Builder.GetSymbol(IRType._symbol)->Get_ExAs<IRStruct>();

		auto Member = _IR_LookingAtIRBlock->New_Member_Access(structV, IRStructPtr, 0);
		
		if (ExType.IsAddress())
		{
			_IR_LookingAtIRBlock->NewStore(Member, Ex);
		}
		else
		{
			_IR_LookingAtIRBlock->NewStore(Member, _IR_LookingAtIRBlock->NewLoadPtr(Ex));
		}

		auto Member2 = _IR_LookingAtIRBlock->New_Member_Access(structV, IRStructPtr, 1);

		auto IDVTable = _IR_Builder.ToID(Str_GetClassWithTraitVTableName(Symbol_GetSymbol(ExType).value()->FullName, Symbol_GetSymbol(ToType).value()->FullName));

		_IR_LookingAtIRBlock->NewStore(Member2,_IR_LookingAtIRBlock->NewLoadPtr(IDVTable));

		_IR_LastExpressionField = structV;
		return true;
	}

	return false;
}
void SystematicAnalysis::IR_Build_ExplicitConversion(IRInstruction* Ex, const TypeSymbol ExType, const TypeSymbol& ToType, const CastExpressionNode_Data& Data)
{
	if (!IR_Build_ImplicitConversion(Ex, ExType, ToType))
	{
		if (ExType._Type == TypesEnum::CustomType)
		{
			Symbol& syb = *Symbol_GetSymbol(ExType._CustomTypeSymbol);
			if (syb.Type == SymbolType::Hard_Type_alias)
			{
				if (Type_AreTheSameWithOutimmutable(syb.VarType, ExType))
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
			
			_IR_LastExpressionField = Ex;

			IR_Build_FuncCall(v, {}, {});
		}
		else
		{
			if (Type_IsIntType(ToType))
			{
				
				if (Type_IsUIntType(ExType))
				{
					switch (ToType._Type)
					{
					case TypesEnum::sInt8:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_UIntToSInt(_IR_LookingAtIRBlock->New_UIntToUInt8(Ex));break;
					case TypesEnum::sInt16:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_UIntToSInt(_IR_LookingAtIRBlock->New_UIntToUInt16(Ex));break;
					
					ULablesint32:
					case TypesEnum::sInt32:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_UIntToSInt(_IR_LookingAtIRBlock->New_UIntToUInt32(Ex));break;
					
					ULablesint64:
					case TypesEnum::sInt64:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_UIntToSInt(_IR_LookingAtIRBlock->New_UIntToUInt64(Ex));break;


					case TypesEnum::uInt8:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_UIntToUInt8(Ex); break;
					case TypesEnum::uInt16:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_UIntToUInt16(Ex); break;
					
					ULableuint32:
					case TypesEnum::uInt32:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_UIntToUInt32(Ex); break;
					
					ULableuint64:
					case TypesEnum::uInt64:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_UIntToUInt64(Ex); break;


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
					default:UCodeLangUnreachable(); break;
					}
				}
				else if (Type_IsSIntType(ExType))
				{
					switch (ToType._Type)
					{
					case TypesEnum::uInt8:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_SIntToUInt(_IR_LookingAtIRBlock->New_SIntToSInt8(Ex)); break;
					case TypesEnum::uInt16:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_SIntToUInt(_IR_LookingAtIRBlock->New_SIntToSInt16(Ex)); break;
					
					SLableuint32:
					case TypesEnum::uInt32:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_SIntToUInt(_IR_LookingAtIRBlock->New_SIntToSInt32(Ex)); break;
					
					SLableuint64:
					case TypesEnum::uInt64:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_SIntToUInt(_IR_LookingAtIRBlock->New_SIntToSInt64(Ex)); break;


					case TypesEnum::sInt8:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_SIntToSInt8(Ex); break;
					case TypesEnum::sInt16:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_SIntToSInt16(Ex); break;
				
					SLablesint32:
					case TypesEnum::sInt32:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_SIntToSInt32(Ex); break;
					
					SLablesint64:
					case TypesEnum::sInt64:_IR_LastExpressionField = _IR_LookingAtIRBlock->New_SIntToSInt64(Ex); break;

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
					default:UCodeLangUnreachable(); break;
					}
				}
				else
				{
					UCodeLangUnreachable();
				}
			}
			else
			{
				UCodeLangUnreachable();
			}
		}

	}
}
bool SystematicAnalysis::Type_IsSIntType(const TypeSymbol& TypeToCheck) const
{
	return
		TypeToCheck._Type == TypesEnum::sInt8 ||
		TypeToCheck._Type == TypesEnum::sInt16 ||
		TypeToCheck._Type == TypesEnum::sInt32 ||
		TypeToCheck._Type == TypesEnum::sInt64 ||
		TypeToCheck._Type == TypesEnum::sIntPtr;
}
bool SystematicAnalysis::Symbol_IsVarableType(SymbolType type) const
{
	switch (type)
	{
	case SymbolType::ParameterVarable:
	case SymbolType::StackVarable:
	case SymbolType::StaticVarable:
	case SymbolType::ThreadVarable:
	case SymbolType::Unmaped_Varable:
		return true;
	default:
		return false;
		break;
	}
}
bool SystematicAnalysis::Type_IsUIntType(const TypeSymbol& TypeToCheck) const
{
	return
		TypeToCheck._Type == TypesEnum::uInt8 ||
		TypeToCheck._Type == TypesEnum::uInt16 ||
		TypeToCheck._Type == TypesEnum::uInt32 ||
		TypeToCheck._Type == TypesEnum::uInt64 ||
		TypeToCheck._Type == TypesEnum::uIntPtr ;
}
bool SystematicAnalysis::Type_IsfloatType(const TypeSymbol& TypeToCheck) const
{
	return
		TypeToCheck._Type == TypesEnum::float32 ||
		TypeToCheck._Type == TypesEnum::float64;
}
bool SystematicAnalysis::Type_IsCharType(const TypeSymbol& TypeToCheck) const
{
	return
		TypeToCheck._Type == TypesEnum::Char ||
		TypeToCheck._Type == TypesEnum::Uft8 || 
		TypeToCheck._Type == TypesEnum::Uft16 ||
		TypeToCheck._Type == TypesEnum::Uft32;
}
bool SystematicAnalysis::Type_IsPrimitive(const TypeSymbol& TypeToCheck) const
{
	bool r = TypeToCheck.IsAddress() || Type_IsPrimitiveNotIncludingPointers(TypeToCheck);

	if (!r && TypeToCheck.IsAn(TypesEnum::CustomType))
	{
		const Symbol& V = *Symbol_GetSymbol(TypeToCheck).value();
		if (V.Type == SymbolType::Func_ptr)
		{
			return true;
		}
	}

	return r;
}
bool SystematicAnalysis::Type_IsPrimitiveNotIncludingPointers(const TypeSymbol& TypeToCheck) const
{
	bool r = Type_IsIntType(TypeToCheck)
		|| TypeToCheck._Type == TypesEnum::Bool
		|| Type_IsCharType(TypeToCheck)
		|| TypeToCheck._Type == TypesEnum::Void
		|| Type_IsfloatType(TypeToCheck);

	if (r == false && TypeToCheck._Type == TypesEnum::CustomType)
	{
		auto V = Symbol_GetSymbol(TypeToCheck._CustomTypeSymbol).value();
		if (V && (V->Type == SymbolType::Hard_Type_alias
			|| V->Type == SymbolType::Type_alias))
		{
			return Type_IsPrimitiveNotIncludingPointers(V->VarType);
		}
	}

	return r;
}
bool SystematicAnalysis::Type_IsStaticArray(const TypeSymbol& TypeToCheck) const
{
	if (TypeToCheck.IsAn(TypesEnum::CustomType))
	{
		auto Syb = Symbol_GetSymbol(TypeToCheck).value();
		return Syb->Type == SymbolType::Type_StaticArray;
	}


	return false;
}


bool SystematicAnalysis::Type_IsimmutableRulesfollowed(const TypeSymbol& TypeToCheck, const TypeSymbol& Type) const
{

	bool Chechimm = TypeToCheck.Isimmutable();
	bool CmpTypeimm = Type.Isimmutable();


	if (Chechimm == CmpTypeimm || CmpTypeimm == true)
	{
		return true;
	}

	return false;
}
bool SystematicAnalysis::Type_IsAddessAndLValuesRulesfollowed(const TypeSymbol& TypeToCheck, const TypeSymbol& Type, bool ReassignMode) const
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
bool SystematicAnalysis::Symbol_HasDestructor(const TypeSymbol& TypeToCheck)
{
	bool DegBugFlag = (OptimizationFlags_t)_Settings->_Flags & (OptimizationFlags_t)OptimizationFlags::ForDebuging;


	if (!DegBugFlag && Type_IsPrimitive(TypeToCheck))
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
	auto dropfunc = Symbol_GetSymbol(TypeDestructorFuncName, SymbolType::Func);
	if (!dropfunc.has_value())
	{
		auto Sym = Symbol_GetSymbol(TypeToCheck);

		if (Sym && Sym.value()->Type == SymbolType::Type_class)
		{
			auto Classinfo = Sym.value()->Get_Info<ClassInfo>();

			if (!Symbol_IsDependencies(Classinfo))
			{
				Symbol_Update_ClassSym_ToFixedTypes(Sym.value());
			}
			else
			{
				//do nothing so GetSize will make the error.				
			}

			return Classinfo->_WillHaveFielddeInit;
		}
	}
	return dropfunc.has_value();
}
void  SystematicAnalysis::Symbol_Update_ClassSym_ToFixedTypes(NeverNullPtr<Symbol> Sym)
{
	if (Sym->PassState == PassType::GetTypes)
	{
		ClassInfo* Vp = Sym->Get_Info<ClassInfo>();
		auto& classNode = *Sym->Get_NodeInfo<ClassNode>();
		if (!Symbol_IsDependencies(Vp))
		{
			auto Old = SaveAndMove_SymbolContext();
			Set_SymbolConext(Vp->Conext.value());

			OnClassNode(classNode);//update class fields


			Set_SymbolConext(std::move(Old));
		}
		else
		{
			LogError_TypeDependencyCycle(NeverNullptr(classNode._className.token), Vp);
		}

		/*
		ClassInfo* info = Sym->Get_Info<ClassInfo>();
		auto OldConext = SaveAndMove_SymbolContext();
		Set_SymbolConext(info->Conext.value());

		OnClassNode(*Sym->Get_NodeInfo<ClassNode>());

		Set_SymbolConext(std::move(OldConext));
		*/
	}
}
void  SystematicAnalysis::Symbol_Update_FuncSym_ToFixedTypes(NeverNullPtr<Symbol> Sym)
{
	if (Sym->PassState == PassType::GetTypes)
	{
		FuncInfo* info = Sym->Get_Info<FuncInfo>();

		auto OldConext = SaveAndMove_SymbolContext();
		Set_SymbolConext(info->Conext.value());

		OnFuncNode(*Sym->Get_NodeInfo<FuncNode>());

		Set_SymbolConext(std::move(OldConext));
	}
}
void SystematicAnalysis::Symbol_Update_EnumSym_ToFixedTypes(NeverNullPtr<Symbol> Sym)
{
	if (Sym->PassState == PassType::GetTypes)
	{
		EnumInfo* info = Sym->Get_Info<EnumInfo>();

		auto OldConext = SaveAndMove_SymbolContext();
		Set_SymbolConext(info->Conext.value());

		OnEnum(*Sym->Get_NodeInfo<EnumNode>());

		Set_SymbolConext(std::move(OldConext));
	}
}
void SystematicAnalysis::Symbol_Update_TraitSym_ToFixedTypes(NeverNullPtr<Symbol> Sym)
{
	if (Sym->PassState == PassType::GetTypes)
	{
		TraitInfo* info = Sym->Get_Info<TraitInfo>();

		auto OldConext = SaveAndMove_SymbolContext();
		Set_SymbolConext(info->Conext.value());

		OnTrait(*Sym->Get_NodeInfo<TraitNode>());

		Set_SymbolConext(std::move(OldConext));
	}
}
void SystematicAnalysis::Symbol_Update_TagSym_ToFixedTypes(NeverNullPtr<Symbol> Sym)
{
	if (Sym->PassState == PassType::GetTypes)
	{
		TagInfo* info = Sym->Get_Info<TagInfo>();

		auto OldConext = SaveAndMove_SymbolContext();
		Set_SymbolConext(info->Conext.value());

		OnTag(*Sym->Get_NodeInfo<TagTypeNode>());

		Set_SymbolConext(std::move(OldConext));
	}
}
void SystematicAnalysis::Symbol_Update_AliasSym_ToFixedTypes(NeverNullPtr<Symbol> Sym)
{
	if (Sym->PassState == PassType::GetTypes)
	{
		AliasInfo* info = Sym->Get_Info<AliasInfo>();

		auto OldConext = SaveAndMove_SymbolContext();
		Set_SymbolConext(info->Conext.value());

		OnAliasNode(*Sym->Get_NodeInfo<AliasNode>());

		Set_SymbolConext(std::move(OldConext));
	}
}
void SystematicAnalysis::Symbol_Update_EvalSym_ToFixedTypes(NeverNullPtr<Symbol> Sym)
{
	if (Sym->PassState == PassType::GetTypes)
	{
		ConstantExpressionInfo* info = Sym->Get_Info<ConstantExpressionInfo>();

		auto OldConext = SaveAndMove_SymbolContext();
		Set_SymbolConext(info->Conext.value());

		OnDeclareVariablenode(*Sym->Get_NodeInfo<DeclareVariableNode>(),DeclareStaticVariableNode_t::Eval);

		Set_SymbolConext(std::move(OldConext));
	}
}
void SystematicAnalysis::Symbol_Update_ThreadAndStatic_ToFixedTypes(NeverNullPtr<Symbol> Sym)
{
	if (Sym->PassState == PassType::GetTypes)
	{
		UCodeLangAssert(Sym->Type == SymbolType::ThreadVarable || Sym->Type == SymbolType::StaticVarable);
		DeclareVariableInfo* info = Sym->Get_Info<DeclareVariableInfo>();

		auto OldConext = SaveAndMove_SymbolContext();
		Set_SymbolConext(info->Conext.value());

		OnDeclareVariablenode(*Sym->Get_NodeInfo<DeclareVariableNode>(), 
			Sym->Type == SymbolType::ThreadVarable ? DeclareStaticVariableNode_t::Thread : DeclareStaticVariableNode_t::Static);

		Set_SymbolConext(std::move(OldConext));
	}
}
void SystematicAnalysis::Symbol_Update_Sym_ToFixedTypes(NeverNullPtr<Symbol> Sym)
{
	switch (Sym->Type)
	{
	case SymbolType::Generic_class:
	case SymbolType::Type_class:
		Symbol_Update_ClassSym_ToFixedTypes(Sym);
		break;



	case SymbolType::Hard_Type_alias:
	case SymbolType::Type_alias:
	case SymbolType::Func_ptr:
	case SymbolType::Hard_Func_ptr:
	case SymbolType::Generic_Alias:
		Symbol_Update_AliasSym_ToFixedTypes(Sym);
		break;

	case SymbolType::Generic_Enum:
	case SymbolType::Enum:
		Symbol_Update_EnumSym_ToFixedTypes(Sym);
		break;

	case SymbolType::Tag_class:
	case SymbolType::Generic_Tag:
		Symbol_Update_TagSym_ToFixedTypes(Sym);
		break;

	case SymbolType::ConstantExpression:
		Symbol_Update_EvalSym_ToFixedTypes(Sym);
		break;

	case SymbolType::Generic_Trait:
	case SymbolType::Trait_class:
		Symbol_Update_TraitSym_ToFixedTypes(Sym);
		break;

	case SymbolType::GenericFunc:
	case SymbolType::Func:
		Symbol_Update_FuncSym_ToFixedTypes(Sym);
		break;

	case SymbolType::StaticVarable:
	case SymbolType::ThreadVarable:
		Symbol_Update_ThreadAndStatic_ToFixedTypes(Sym);
		break;
	default:
		UCodeLangUnreachable();
		break;
	}
}
bool SystematicAnalysis::Type_GetSize(const TypeSymbol& Type, UAddress& OutSize)
{
	if (Type.IsAddress() || Type.IsAddressArray()){goto IntPtr;}
	
	if (Type.IsTypeInfo())
	{

		if (Type._TypeInfo == TypeInfoPrimitive::ClassFieldInfo)
		{
			OutSize = sizeof(Systematic_BuiltInFunctions::ClassField);
		}
		else if (Type._TypeInfo == TypeInfoPrimitive::EnumFieldInfo)
		{
			OutSize = sizeof(Systematic_BuiltInFunctions::EnumField);
		}
		else if (Type._TypeInfo == TypeInfoPrimitive::EnumVariantInfo)
		{
			OutSize = sizeof(EnumVariantData);
		}
		else
		{
			OutSize = sizeof(TypeSymbol);
		}
		return true;
	}

	switch (Type._Type)
	{
	case TypesEnum::sInt8:
	case TypesEnum::uInt8:
	case TypesEnum::Bool:
	case TypesEnum::Char:
	case TypesEnum::Uft8:
		OutSize = 1;
		return true;
	case TypesEnum::Uft16:
	case TypesEnum::sInt16:
	case TypesEnum::uInt16:
		OutSize = sizeof(UInt16);
		return true;

	case TypesEnum::Uft32:
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
			UCodeLangUnreachable();
			break;
		}
		return true;

	case TypesEnum::CustomType:
	{
		Symbol& V = *Symbol_GetSymbol(Type._CustomTypeSymbol);
	
		if (V.Type == SymbolType::Type_class)
		{

			ClassInfo* Vp = V.Get_Info<ClassInfo>();
			
			if (!Vp->SizeInitialized)
			{
				Vp->Size = 0;
				auto& classNode = *V.Get_NodeInfo<ClassNode>();
				if (!Symbol_IsDependencies(Vp))
				{
					auto Old = SaveAndMove_SymbolContext();
					Set_SymbolConext(Vp->Conext.value());

					OnClassNode(classNode);//update class fields
				
				
					Set_SymbolConext(std::move(Old));
				}
				else
				{
					LogError_TypeDependencyCycle(NeverNullptr(classNode._className.token),Vp);
				}
			}
			
			
			OutSize = Vp->Size;
			return true;
		}
		else if(V.Type == SymbolType::Type_alias
			|| V.Type == SymbolType::Hard_Type_alias)
		{

		

			return Type_GetSize(V.VarType,OutSize);
		}
		else if (V.Type == SymbolType::Enum)
		{
			EnumInfo* Info = V.Get_Info<EnumInfo>();
			if (Info->VariantData.has_value()) 
			{
				auto tagsize = Type_GetSize(Info->Basetype, OutSize);
				auto& Item = Info->VariantData.value();
				
				
				if (!Item.VariantSize.has_value())
				{
					size_t MaxSize = 0;
					
					for (auto& Item2 : Item.Variants)
					{
						size_t ItemSize = 0;
						for (auto& Item3 : Item2.Types)
						{
							UAddress tep = 0;
							Type_GetSize(Item3, tep);
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
				return Type_GetSize(Info->Basetype, OutSize);
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
			bool V = Type_GetSize(Info->Type,OutSize);
			OutSize *= Info->Count;
			return V;
		}
		else if (V.Type == SymbolType::Trait_class && Type._IsDynamic)
		{
			TypeSymbol pointer(TypesEnum::Bool);
			pointer.SetAsAddress();
			return Type_GetSize(pointer).value() * 2;
		}
		else if (V.Type == SymbolType::Unmaped_Generic_Type)
		{
			OutSize = 0;
			return true;
		}
		else
		{
			OutSize = 0;
			return false;
		}
	}
	default:
		OutSize = 0;
		return false;
	}
	


}
bool SystematicAnalysis::Type_GetOffset(const ClassInfo& Type, const FieldInfo* Field, UAddress& OutOffset)
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
		Type_GetSize(Item.Type, FieldSize);
		offset += FieldSize;
	}

	OutOffset = offset;
	return false;
}
SystematicAnalysis::Get_FuncInfo SystematicAnalysis::Type_GetFunc(const TypeSymbol& Name, const ValueParametersNode& Pars)
{
	if (Name.IsBadType())
	{
		return {};
	}
	{
		auto SybOp = Symbol_GetSymbol(Name);
		if (SybOp) 
		{
			auto Syb = SybOp.value();
			if (Syb->Type == SymbolType::Generic_class || Syb->Type == SymbolType::Generic_Trait)
			{

				//most likely Generic Testing
				Get_FuncInfo R;
				R.CantCheckBecauseIsUnMaped = true;
				return R;
			}
			if (Type_IsUnMapType(Name))
			{
				Get_FuncInfo R;
				R.CantCheckBecauseIsUnMaped = true;
				return R;
			}
		}
	}
	String B =ToString(Name);
	Token T;
	T.Type = TokenType::Name;
	T.Value._String = B;


	ScopedNameNode Tep;
	ScopedName V;
	V._token = &T; 

	Tep._ScopedName.push_back(std::move(V));


	return Type_GetFunc(Tep, Pars, Name);
}
void SystematicAnalysis::IR_Build_FuncCall(Get_FuncInfo Func, const ScopedNameNode& Name, const ValueParametersNode& Pars)
{
	if (_PassType != PassType::BuidCode) { return; }
	
	Debug_Add_SetLineNumber(NeverNullptr(Name._ScopedName.begin()->_token), _IR_LookingAtIRBlock->Instructions.size());
	{
		
#define PrimitiveTypeCall(FullName,TypeEnum,DefaultValue) if (ScopedName == FullName) \
		{\
			TypeSymbol iNfo;\
			iNfo.SetType(TypeEnum);\
			if (Pars._Nodes.size())\
			{\
				_LookingForTypes.push(iNfo);\
				auto& Item = Pars._Nodes[0];\
				OnExpressionTypeNode(Item.get(),GetValueMode::Read);\
				IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, iNfo);\
				_LookingForTypes.pop();\
			}\
			else\
			{\
				_LastExpressionType = iNfo;\
				DefaultValue;\
			}\
			return;\
		}\

		auto ScopedName = Str_GetScopedNameAsString(Name);
		auto SymbolsV = Symbol_GetSymbol(ScopedName, SymbolType::Any);
		if (SymbolsV && SymbolsV.value()->Type == SymbolType::Type_alias)
		{
			ScopedName = ToString(SymbolsV.value()->VarType);
		}

		PrimitiveTypeCall(Uint8TypeName, TypesEnum::uInt8, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((UInt8)0);)
		else PrimitiveTypeCall(Uint16TypeName, TypesEnum::uInt16, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((UInt16)0))
		else PrimitiveTypeCall(Uint32TypeName, TypesEnum::uInt32, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((UInt32)0))
		else PrimitiveTypeCall(Uint16TypeName, TypesEnum::uInt64, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad(((UInt64)0)))

		else PrimitiveTypeCall(Sint8TypeName, TypesEnum::sInt8, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((Int8)0);)
		else PrimitiveTypeCall(Sint16TypeName, TypesEnum::sInt16, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((Int16)0))
		else PrimitiveTypeCall(Sint32TypeName, TypesEnum::sInt32, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((Int32)0))
		else PrimitiveTypeCall(Sint16TypeName, TypesEnum::sInt64, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((Int64)0))

		else PrimitiveTypeCall(boolTypeName, TypesEnum::Bool, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad(false))
		else PrimitiveTypeCall(CharTypeName, TypesEnum::Char, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad('\0'))

		else PrimitiveTypeCall(float32TypeName, TypesEnum::float32, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((float32)0))
		else PrimitiveTypeCall(float64TypeName, TypesEnum::float64, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((float64)0))

		else PrimitiveTypeCall(UintPtrTypeName, TypesEnum::uIntPtr, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((UInt64)0))
		else PrimitiveTypeCall(SintPtrTypeName, TypesEnum::sIntPtr, _IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad(((UInt64)0)))


	}
	if (Func.Func == nullptr)
	{

		if (Func.SymFunc)
		{
			if (Func.SymFunc->Type == SymbolType::Enum_Field)
			{
				auto ScopedName = Str_GetScopedNameAsString(Name);
				String EnumClassFullName = ScopedName;
				ScopeHelper::ReMoveScope(EnumClassFullName);



				auto EnumSymbol = Symbol_GetSymbol(EnumClassFullName, SymbolType::Enum);
				if (EnumSymbol)
				{
					EnumInfo* EnumSybInfo = EnumSymbol.value()->Get_Info<EnumInfo>();
					auto& VariantData = EnumSybInfo->VariantData.value();
					size_t EnumIndex = EnumSybInfo->GetFieldIndex(ScopeHelper::GetNameFromFullName(ScopedName)).value();

					EnumFieldInfo& EnumFieldinfo = EnumSybInfo->Fields[EnumIndex];
					EnumVariantFeild& EnumVariantFeildData = VariantData.Variants[EnumIndex];

					if (Func.ThisPar == Get_FuncInfo::ThisPar_t::NoThisPar_GetValue_EnumVariant)
					{
						IRInstruction* ThisObj = nullptr;
						IRInstruction* BoolObj = nullptr;
						{
							TypeSymbol Par =EnumSymbol.value()->ID;
							Par._IsAddress = true;

							auto& Item = Pars._Nodes[0];

							_LookingForTypes.push(Par);

							OnExpressionTypeNode(Item.get(), GetValueMode::Read);
							IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, Par);

							ThisObj = _IR_LastExpressionField;

							_LookingForTypes.pop();
						}

					
						auto Key = LoadEvaluatedEx(EnumFieldinfo.Ex, EnumSybInfo->Basetype);


						
						auto Member = _IR_LookingAtIRBlock->New_Member_Dereference(ThisObj, IR_ConvertToIRType(EnumSymbol.value()->ID), 0);

						auto ObjUnion = _IR_LookingAtIRBlock->New_Member_Dereference(ThisObj, IR_ConvertToIRType(EnumSymbol.value()->ID), 1);


						if (Type_IsPrimitiveNotIncludingPointers(EnumSybInfo->Basetype))
						{
							BoolObj = _IR_LookingAtIRBlock->NewC_Equalto(Member, Key);
						}
						else
						{
							UCodeLangUnreachable();
						}


						IRStruct* UnionStruct = nullptr;
						{
							String UnionName = Str_GetEnumVariantUnionName(EnumSybInfo->FullName);
							IRidentifierID UnionID = _IR_Builder.ToID(UnionName);
							UnionStruct = _IR_Builder.GetSymbol(UnionID)->Get_ExAs<IRStruct>();
						}
						
						IRStruct* VStruct = nullptr;
						if (EnumVariantFeildData.ClassSymbol.has_value())
						{
							TypeSymbol VSyb = TypeSymbol(EnumVariantFeildData.ClassSymbol.value());
							VStruct = _IR_Builder.GetSymbol(IR_ConvertToIRType(VSyb)._symbol)->Get_ExAs<IRStruct>();
						}

						for (size_t i = 1; i < Pars._Nodes.size(); i++)
						{
							auto& Item = Pars._Nodes[i];
							OutExpression* outEx = OutExpression::As(Item.get());
							SymbolID ID = Symbol_GetSymbolID(outEx);
						

							auto& Syb = _Table.GetSymbol(ID);
							IRInstruction* ItemMember;

							if (EnumVariantFeildData.ClassSymbol.has_value())
							{
								auto Struct = _IR_LookingAtIRBlock->New_Member_Access(ObjUnion, UnionStruct, EnumIndex);
								ItemMember = _IR_LookingAtIRBlock->New_Member_Access(Struct, VStruct, i - 1);
							}
							else
							{
								ItemMember = _IR_LookingAtIRBlock->New_Member_Access(ObjUnion, UnionStruct, EnumIndex);

							}

							Syb.IR_Ins = ItemMember;
						}

						_LastExpressionType = TypeSymbol(TypesEnum::Bool);
						_IR_LastExpressionField = BoolObj;
					}
					else
					{
						auto ID = _IR_Builder.ToID(EnumSybInfo->FullName);

						auto Key = LoadEvaluatedEx(EnumFieldinfo.Ex, EnumSybInfo->Basetype);

						auto VariantClass = _IR_LookingAtIRBlock->NewLoad(IRType(ID));
						IRStruct* V = _IR_Builder.GetSymbol(ID)->Get_ExAs<IRStruct>();
						auto Member = _IR_LookingAtIRBlock->New_Member_Access(VariantClass, V, 0);
						_IR_LookingAtIRBlock->NewStore(Member, Key);

						if (EnumVariantFeildData.Types.size()) {
							auto UnionMember = _IR_LookingAtIRBlock->New_Member_Access(VariantClass, V, 1);


							String UnionName = Str_GetEnumVariantUnionName(EnumSybInfo->FullName);
							IRidentifierID UnionID = _IR_Builder.ToID(UnionName);

							auto ObjectMember = _IR_LookingAtIRBlock->New_Member_Access(UnionMember, _IR_Builder.GetSymbol(UnionID)->Get_ExAs<IRStruct>(), EnumIndex);

							IRStruct* VStruct = nullptr;
							if (EnumVariantFeildData.ClassSymbol.has_value())
							{
								TypeSymbol VSyb = TypeSymbol(EnumVariantFeildData.ClassSymbol.value());
								VStruct = _IR_Builder.GetSymbol(IR_ConvertToIRType(VSyb)._symbol)->Get_ExAs<IRStruct>();
							}
							//
							for (size_t i = 0; i < Pars._Nodes.size(); i++)
							{
								auto& Item = Pars._Nodes[i];
								auto& FuncParInfo = EnumVariantFeildData.Types[i];



								_LookingForTypes.push(FuncParInfo);

								OnExpressionTypeNode(Item.get(), GetValueMode::Read);
								IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, FuncParInfo);

								auto ParEx = _IR_LastExpressionField;

								if (EnumVariantFeildData.ClassSymbol.has_value())
								{
									auto VMember = _IR_LookingAtIRBlock->New_Member_Access(ObjectMember, VStruct, i);
									_IR_LookingAtIRBlock->NewStore(VMember, ParEx);
								}
								else
								{
									_IR_LookingAtIRBlock->NewStore(ObjectMember, ParEx);
								}
								_LookingForTypes.pop();
							}
							//


						}
						_LastExpressionType = EnumSymbol.value()->VarType;
						_IR_LastExpressionField = VariantClass;
					}
				}
			}
		}
		else if (Func._BuiltFunc.has_value())
		{
			auto& Value = Func._BuiltFunc.value();

			if (Value.EvalObject.has_value())
			{
				auto& EvalObject = Value.EvalObject.value();
				if (Value.EvalAsCString)
				{
					_IR_LastExpressionField = IR_RawObjectDataToCString(EvalObject);

					_LastExpressionType = TypesEnum::Char;
					_LastExpressionType.SetAsAddressArray();
					_LastExpressionType.SetAsimmutable();
				}
				else
				{
					_IR_LastExpressionField = LoadEvaluatedEx(EvalObject, Value.RetType);
				}
			}
			else
			{

				UCodeLangUnreachable();
			}

			_LastExpressionType = Value.RetType;
		}
		else
		{
		UCodeLangUnreachable();
		}
		return;
	}

	IRInstruction* PushIRStackRet = nullptr;
	bool AutoPushThis = Get_FuncInfo::AddOneToGetParNode(Func.ThisPar);

	Vector< IRInstruction*> IRParsList;

	if (AutoPushThis)
	{
		if (Func.ThisPar == Get_FuncInfo::ThisPar_t::FullScopedName)
		{

			GetMemberTypeSymbolFromVar_t V;
			Symbol_MemberTypeSymbolFromVar(0, Name._ScopedName.size(), Name, V);

			IRParsList.push_back(IR_Build_Member_AsPointer(V));
		}
		else
			if (Func.ThisPar == Get_FuncInfo::ThisPar_t::PushFromScopedName)
			{

				GetMemberTypeSymbolFromVar_t V;
				Symbol_MemberTypeSymbolFromVar(0, Name._ScopedName.size() - 1, Name, V);

				IRParsList.push_back(IR_Build_Member_AsPointer(V));
			}
			else if (Func.ThisPar == Get_FuncInfo::ThisPar_t::PushFromLast)
			{
				IRParsList.push_back(_IR_LastExpressionField);
			}
			else if (Func.ThisPar == Get_FuncInfo::ThisPar_t::OnIRlocationStack)
			{

				bool UseedTopIR = _IR_IRlocations.size() != 0 && _IR_IRlocations.top().UsedlocationIR == false;
				if (!UseedTopIR)
				{
					IRLocation_Cotr tep;
					tep.UsedlocationIR = false;

					auto Type = Func.Func->Pars[0];
					if (Type.Type.IsAddress())
					{
						Type.Type._IsAddress = false;
					}

					PushIRStackRet = tep.Value = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(Type));
					_IR_IRlocations.push(tep);
				}
				else
				{
					PushIRStackRet = _IR_IRlocations.top().Value;

				}



				{
					auto Defe = _IR_LookingAtIRBlock->NewLoadPtr(_IR_IRlocations.top().Value);
					_IR_IRlocations.top().UsedlocationIR = true;
					IRParsList.push_back(Defe);
				}

				if (!UseedTopIR)
				{
					_IR_IRlocations.pop();
				}

			}
			else if (Func.ThisPar == Get_FuncInfo::ThisPar_t::OnIRlocationStackNonedef)
			{
				bool UseedTopIR = _IR_IRlocations.size() != 0 && _IR_IRlocations.top().UsedlocationIR == false;
				if (!UseedTopIR)
				{
					IRLocation_Cotr tep;
					tep.UsedlocationIR = false;

					auto Type = Func.Func->Pars[0];
					if (Type.Type.IsAddress())
					{
						Type.Type._IsAddress = false;
					}


					PushIRStackRet = tep.Value = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(Type.Type));
					_IR_IRlocations.push(tep);
				}

				{
					IRParsList.push_back(_IR_IRlocations.top().Value);
					_IR_IRlocations.top().UsedlocationIR = true;
				}

				if (!UseedTopIR)
				{
					_IR_IRlocations.pop();
				}
			}
			else if (Func.ThisPar == Get_FuncInfo::ThisPar_t::PushWasCalled)
			{
				UCodeLangUnreachable();//just add IRPar list
				//what does  "just add IRPar list" mean
			}
			else if (Func.ThisPar == Get_FuncInfo::ThisPar_t::AutoPushThis)
			{
				IRParsList.push_back(_IR_LookingAtIRBlock->NewLoad(&_IR_LookingAtIRFunc->Pars.front()));
			}
			else if (Func.ThisPar == Get_FuncInfo::ThisPar_t::PushFromScopedNameDynamicTrait)
			{

				GetMemberTypeSymbolFromVar_t V;
				Symbol_MemberTypeSymbolFromVar(0, Name._ScopedName.size() - 1, Name, V);

				IRParsList.push_back(IR_Build_Member_AsPointer(V));
			}
			else
			{
				UCodeLangUnreachable();;
			}
	}


	for (size_t i = 0; i < Pars._Nodes.size(); i++)
	{
		auto& Item = Pars._Nodes[i];

		auto Index = i + (AutoPushThis ? 1 : 0);

		const Vector<ParInfo>& Pars = Func.SymFunc->Type == SymbolType::Func
			? Func.Func->Pars
			: ((FuncPtrInfo*)Func.Func)->Pars;

		

		auto FuncParInfoPtr = Index < Pars.size() ? Pars[Index]
			: Pars.back();

		if (FuncParInfoPtr.Type._CustomTypeSymbol.HasValue())
		{
			const auto Syb = Symbol_GetSymbol(FuncParInfoPtr.Type).value();
			if (Syb->Type == SymbolType::Type_Pack)
			{
				auto* typepack = Syb->Get_Info<TypePackInfo>();
				size_t NewIndex = Index-(Pars.size()-1);


				FuncParInfoPtr.Type = typepack->List[NewIndex];
			} 
		}
		auto& FuncParInfo =FuncParInfoPtr;

		_LookingForTypes.push(FuncParInfo.Type);

		if (Item->Get_Type() == NodeType::OutExpression)
		{
			const OutExpression* OutEx = OutExpression::As(Item.get());
			
			auto OutVal = Symbol_GetSymbolFromExpression(NeverNullptr(OutEx));

			OutVal->IR_Ins = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(OutVal->VarType));

			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoadPtr(OutVal->IR_Ins);
		}
		else 
		{
			OnExpressionTypeNode(Item.get(), GetValueMode::Read);
			IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, FuncParInfo.Type);
		}
		IRParsList.push_back(_IR_LastExpressionField);


		_LookingForTypes.pop();
	}
	auto Syb = Func.SymFunc;

	FileDependency_AddDependencyToCurrentFile(Syb);

	//

	if (Func.ThisPar != Get_FuncInfo::ThisPar_t::PushFromScopedNameDynamicTrait)
	{
		for (auto& Item : IRParsList)
		{
			_IR_LookingAtIRBlock->NewPushParameter(Item);
		}
	}
	//
	if (Func.ThisPar == Get_FuncInfo::ThisPar_t::PushFromScopedNameDynamicTrait) 
	{
		GetMemberTypeSymbolFromVar_t V;
		Symbol_MemberTypeSymbolFromVar(0, Name._ScopedName.size() - 1, Name, V);

		auto TraitType = V.Type;
		auto TraitSyb = Symbol_GetSymbol(TraitType).value();
		auto TraitAsIR = IR_ConvertToIRType(TraitType);

		auto VPtrMember = _IR_LookingAtIRBlock->New_Member_Dereference(IRParsList.front(), TraitAsIR,1);


		TraitInfo* Info = TraitSyb->Get_Info<TraitInfo>();
		size_t FuncIndex = Info->GetIndex_Func(Func.SymFunc).value();

		auto PtrCall = _IR_LookingAtIRBlock->New_Member_Dereference(VPtrMember,IRType(_IR_Builder.ToID(Str_GetTraitVStructTableName(TraitSyb->FullName))), FuncIndex);



		IRParsList.front() = _IR_LookingAtIRBlock->New_Member_Dereference(IRParsList.front(), TraitAsIR, 0);

		//
		for (auto& Item : IRParsList)
		{
			_IR_LookingAtIRBlock->NewPushParameter(Item);
		}
		_IR_LastExpressionField = _IR_LookingAtIRBlock->NewCallFuncPtr(PtrCall);
		//
	}
	else
	if (Syb->Type== SymbolType::Func)
	{
		_IR_LastExpressionField = _IR_LookingAtIRBlock->NewCall(IR_GetIRID(Func.Func));
	}
	else if (Syb->Type == SymbolType::StackVarable)
	{
		_IR_LastExpressionField = _IR_LookingAtIRBlock->NewCallFuncPtr(Syb->IR_Ins);
	}
	else if (Syb->Type == SymbolType::ParameterVarable)
	{
		_IR_LastExpressionField = _IR_LookingAtIRBlock->NewCallFuncPtr(Syb->IR_Par);
	}
	else
	{
		UCodeLangUnreachable();
	}

	{
		auto Tep = _IR_LastExpressionField;
		
		/*
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
		*/
		_IR_LastExpressionField = Tep;
	}

	if (_LookingForTypes.size() && Type_Get_LookingForType().IsnotAn(TypesEnum::Void) && PushIRStackRet)//constructors are just void funcions so just set last as the input this
	{

		_IR_LastExpressionField = PushIRStackRet;
		_LastExpressionType = Func.Func->Pars.front().Type;
		
		
		if (Type_Get_LookingForType().IsAddress())
		{
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoadPtr(_IR_LastExpressionField);
		}
		else
		{
			_LastExpressionType._IsAddress = false;
		}
		

	}
	else
	{
		_LastExpressionType = Func.Func->Ret;
	}
}
void SystematicAnalysis::IR_Build_DestructorCall(const ObjectToDrop& Object)
{
	
	if (Type_IsPrimitive(Object.Type))
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

			Symbol* Sym = Symbol_GetSymbol(TypeDestructorFuncName, SymbolType::Func).value_unchecked();
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
					_IR_IRlocations.push({Object._Object, false});
					break;	
				case ObjectToDropType::IRInstructionNoMod:
					FuncInfo.ThisPar = Get_FuncInfo::ThisPar_t::OnIRlocationStackNonedef;
						_IR_IRlocations.push({ Object._Object, false });
						break;
				case ObjectToDropType::Operator:
					FuncInfo.ThisPar = Get_FuncInfo::ThisPar_t::OnIRlocationStackNonedef;

					if (Object._Operator.Type != IROperatorType::IRInstruction)
					{
						UCodeLangUnreachable();
					}

					_IR_IRlocations.push({_IR_LookingAtIRBlock->NewLoad(Object._Operator.Pointer), false });
					break;
				default:
					UCodeLangUnreachable();
					break;
				}

				//

				ValueParametersNode Vtemp;
				IR_Build_FuncCall(Object.Type, FuncInfo, Vtemp);
			

				_IR_IRlocations.pop();
			}
		}
		
	}
}
SystematicAnalysis::Get_FuncInfo  SystematicAnalysis::Type_GetFunc(const ScopedNameNode& Name,const ValueParametersNode& Pars, TypeSymbol Ret)
{




	TypeSymbol _ThisType;
	Get_FuncInfo::ThisPar_t ThisParType = Get_FuncInfo::ThisPar_t::NoThisPar;
	String ScopedName;
	{

		bool IsThisCall = false;
		for (auto& Item : Name._ScopedName)
		{
			if (Item._operator == ScopedName::Operator_t::Dot)
			{
				IsThisCall = true;
				break;
			}
		}

		if (IsThisCall)
		{
			_GetExpressionMode.push(GetValueMode::Read);
			GetMemberTypeSymbolFromVar_t V;
			bool VBool = Symbol_MemberTypeSymbolFromVar(0, Name._ScopedName.size() - 1, Name, V);
			_GetExpressionMode.pop();
			
			if (VBool)
			{
				_ThisType = V.Type;
				_ThisType.SetAsAddress();

				auto tep_ = V.Type;
				
				if (tep_._IsDynamic)
				{
					tep_._IsDynamic = false;
					ThisParType = Get_FuncInfo::ThisPar_t::PushFromScopedNameDynamicTrait;
				}
				else
				{
					ThisParType = Get_FuncInfo::ThisPar_t::PushFromScopedName;
				}
				Type_RemoveTypeattributes(tep_);


				ScopedName = ToString(tep_) + ScopeHelper::_ScopeSep;
				ScopedName += Name._ScopedName.back()._token->Value._String;
				if (_ThisType.IsBadType())
				{
					Get_FuncInfo V;
					V.CantCheckBecauseIsUnMaped = true;

					return V;//cant check because we are just testing.
				}
			}
			else
			{
				return { Get_FuncInfo::ThisPar_t::NoThisPar, nullptr };
			}
		}
		else 
		{
			ScopedName = "";
			for (auto& Item : Name._ScopedName)
			{
				if (Item._generic.get() && Item._generic->_Values.size())
				{

					ScopedName += Item._token->Value._String;

					auto TypeName = ScopedName;

					bool IsLast = &Item == &Name._ScopedName.back();
					if (!IsLast)
					{
						auto Syb = Generic_InstantiateOrFindGenericSymbol(NeverNullptr(Item._token), *Item._generic, ScopedName);

						if (!Syb.has_value())
						{
							return {};
						}
						else
						{
							ScopedName = Syb.value()->FullName;
						}
					}
					if (Item._operator != ScopedName::Operator_t::Null)
					{
						ScopedName += ScopeHelper::_ScopeSep;
					}
				}
				else
				{
					String Str;
					Item.GetScopedName(Str);
					ScopedName += Str;
					if (Item._operator != ScopedName::Operator_t::Null)
					{
						ScopedName += ScopeHelper::_ScopeSep;
					}
				}
			}
		}
	}
	bool _ThisTypeIsNotNull = !_ThisType.IsNull();
	
	_LastLookedAtToken = Nullableptr(Name._ScopedName.back()._token);
	{

		auto SymbolsV = Symbol_GetSymbol(ScopedName, SymbolType::Any);

		
		if (SymbolsV && SymbolsV.value()->Type == SymbolType::Type_alias)
		{
			ScopedName = ToString(SymbolsV.value()->VarType);
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
				LogError_CanIncorrectParCount(NeverNullptr(Name._ScopedName.back()._token), ScopedName, Pars._Nodes.size(), 1);
			}

			
			
			{
				if (ScopedName == Uint8TypeName){_LastExpressionType = TypesEnum::uInt8;}
				else if (ScopedName == Uint16TypeName) { _LastExpressionType = TypesEnum::uInt16; }
				else if (ScopedName == Uint32TypeName) { _LastExpressionType = TypesEnum::uInt32; }
				else if (ScopedName == Uint64TypeName) { _LastExpressionType = TypesEnum::uInt64; }
				 
				else if (ScopedName == Sint8TypeName) { _LastExpressionType = TypesEnum::sInt8; }
				else if (ScopedName == Sint16TypeName) { _LastExpressionType = TypesEnum::sInt16; }
				else if (ScopedName == Sint32TypeName) { _LastExpressionType = TypesEnum::sInt32; }
				else if (ScopedName == Sint64TypeName) { _LastExpressionType = TypesEnum::sInt64; }

				else if (ScopedName == SintPtrTypeName) { _LastExpressionType = TypesEnum::sIntPtr; }
				else if (ScopedName == UintPtrTypeName) { _LastExpressionType = TypesEnum::uIntPtr; }

				else if (ScopedName == boolTypeName) { _LastExpressionType = TypesEnum::Bool; }
				else if (ScopedName == CharTypeName) { _LastExpressionType = TypesEnum::Char; }

				else if (ScopedName == float32TypeName) { _LastExpressionType = TypesEnum::float32; }
				else if (ScopedName == float64TypeName) { _LastExpressionType = TypesEnum::float64; }

				else
				{
					UCodeLangUnreachable();
				}
			}

			if (Pars._Nodes.size() == 1)
			{
				auto FuncType = _LastExpressionType;
				OnExpressionTypeNode(Pars._Nodes[0].get(), GetValueMode::Read);

				if (!Type_CanBeImplicitConverted(_LastExpressionType, FuncType, true))
				{
					LogError_CantCastImplicitTypes(_LastLookedAtToken.value(), _LastExpressionType, FuncType,true);
				}
				_LastExpressionType = FuncType;
			}

			return { Get_FuncInfo::ThisPar_t::NoThisPar, nullptr };
		}
	
	
	
		
	}
	
	bool AutoThisCall = false;
	
	SymbolType T = SymbolType::Null;
	Symbol* FuncSymbol = nullptr;
	FuncInfo* r = nullptr;

	auto& RetType = Type_Get_LookingForType();
	bool RetIsSet = !(RetType.IsAn(TypesEnum::Var) || RetType.IsAn(TypesEnum::Any));
	

	Vector<ParInfo> ValueTypes;
	ValueTypes.reserve(_ThisTypeIsNotNull ? Pars._Nodes.size() + 1 : Pars._Nodes.size());

	if (_ThisTypeIsNotNull)
	{
		ValueTypes.push_back({ false,_ThisType });
	}

	TypeSymbol NullSymbol;
	NullSymbol.SetType(TypesEnum::Any);

	bool HasOutPar = false;
	for (size_t i = 0; i < Pars._Nodes.size(); i++)
	{
		auto& Item = Pars._Nodes[i];
		auto& ValueItem = ValueTypes.emplace_back();

		_LookingForTypes.push(NullSymbol);


		if (Item->Get_Type() == NodeType::OutExpression)
		{
			const OutExpression* outEx = OutExpression::As(Item.get());
			HasOutPar = true;
			ValueItem.IsOutPar = true;
			ValueItem.Type =Type_ConvertAndValidateType(outEx->_Type,NodeSyb_t::Varable);
		}
		else
		{
			OnExpressionTypeNode(Item.get(), GetValueMode::Read);
			ValueItem.IsOutPar = false;
			ValueItem.Type = _LastExpressionType;
		}

		_LookingForTypes.pop();
		
		
		
	}

	{//unmaped
		for (auto& Item : ValueTypes)
		{
			auto SymbolsV = Symbol_GetSymbol(Item.Type);
			if (SymbolsV.has_value())
			{
				if (SymbolsV.value()->Type == SymbolType::Unmaped_Generic_Type)
				{
					Get_FuncInfo V;
					V.CantCheckBecauseIsUnMaped = true;

					return V;//cant check because we are just testing.
				}
			}
		}

		

	}

	//Out-Par
	{
		if (HasOutPar)
		{
			bool IsControlFlow = false;
			if (_NodeTypeStack.size() > 1) 
			{
				size_t Index = _NodeTypeStack.size() - 1;
				Index--;
				auto& Last = _NodeTypeStack[Index];
				if (Last == NodeType::IfNode || Last == NodeType::WhileNode || Last == NodeType::DoNode
					|| Last == NodeType::RetStatementNode
					|| Last == NodeType::CompileTimeIfNode
					|| Last == NodeType::MatchArm
					)
				{
					IsControlFlow = true;
				}
				else if (_NodeTypeStack.back() == NodeType::CompileTimeIfNode)
				{
					IsControlFlow = true;
				}
			}
			if (_NodeTypeStack.size() > 2)
			{
				size_t Index = _NodeTypeStack.size() - 1;
				Index-=2;
				auto& Last = _NodeTypeStack[Index];
				if (Last == NodeType::MatchArm)
				{
					IsControlFlow = true;
				}
			}
		
			if (!IsControlFlow)
			{
				auto Token = NeverNullptr(Name._ScopedName.back()._token);
				LogError_OutCanOnlyBeInControlFlow(Token); 
				return { };
			}
		}
	}
	//

	//TypeInfo
	{
		bool IsTypeInfo = ValueTypes.size() && ValueTypes.front().Type.IsTypeInfo();

		if (IsTypeInfo)
		{
			bool AutoPassThis = Get_FuncInfo::AddOneToGetParNode(ThisParType);
			Vector< Systematic_BuiltInFunctions::FunctionPar> BuiltInPars;
			BuiltInPars.resize(ValueTypes.size());

			ExpressionNodeType _TepThisPar;
			ValueExpressionNode _TepThisValue;
			ReadVariableNode _TepThisReadNode;
			if (AutoPassThis)
			{
				_TepThisPar._Value.reset(&_TepThisValue);
				_TepThisValue._Value.reset(&_TepThisReadNode);
				_TepThisReadNode._VariableName = Name;
				_TepThisReadNode._VariableName._ScopedName.pop_back();
				_TepThisReadNode._VariableName._ScopedName.back()._operator = ScopedName::Operator_t::Null;
			}


			for (size_t i = 0; i < BuiltInPars.size(); i++)
			{
				auto& ItemFuncPar = BuiltInPars[i];
				auto& ValuePar = ValueTypes[i];

				ItemFuncPar.Type = ValuePar.Type;
				ItemFuncPar.IsOutPar = ValuePar.IsOutPar;
				const Node* ItemNode = nullptr;

				if (AutoPassThis && i == 0)
				{
					ItemNode = _TepThisPar.As();
				}
				else
				{
					ItemNode = Pars._Nodes[AutoPassThis ? i - 1 : i].get();
				}

				ItemFuncPar.ExpressionNode = ItemNode;
			}

			auto FuncData = Systematic_BuiltInFunctions::GetFunction(ScopedName, BuiltInPars,*this);

			if (AutoPassThis)
			{
				auto _ = _TepThisPar._Value.release();//On Stack.
				auto _1 = _TepThisValue._Value.release();//On Stack.
			}

			if (FuncData.has_value())
			{
				auto& FuncDataValue = FuncData.value();
				{//OutPars

					size_t OutParIndex = 0;
					
					for (size_t i = 0; i < BuiltInPars.size(); i++)
					{
						bool IsOutPar = BuiltInPars[i].IsOutPar;

						if (IsOutPar)
						{
							const auto& ItemNode = Pars._Nodes[AutoPassThis ? i - 1 : i];
							auto& FuncDataOutPar = FuncDataValue._OutPars[OutParIndex];

							OutExpression* Ex = OutExpression::As(ItemNode.get());
							if (FuncDataOutPar.EvalObject.has_value())
							{
								EvaluatedEx EvaluatedValue;
								EvaluatedValue.Type = FuncDataOutPar.Type;
								EvaluatedValue.EvaluatedObject =std::move(FuncDataOutPar.EvalObject.value());
								Eval_SetOutExpressionEval(Ex, EvaluatedValue);
							}
							else
							{
								Symbol_SetOutExpression(Ex, FuncDataOutPar.Type);
							}

							OutParIndex++;
						}
					}
					FuncDataValue._OutPars.clear();//Free it's unused. 
				}


				Get_FuncInfo R;
				R.ThisPar = ThisParType;
				R._BuiltFunc = std::move(FuncDataValue);



				return R;
			}
		}
		else
		{

		}
	}
	//

	auto Symbols = _Table.GetSymbolsWithName(ScopedName, SymbolType::Any);
	StartSymbolsLoop:

	Vector<Get_FuncInfo> OkFuncions;

	const UseGenericsNode& Generics = *Name._ScopedName.back()._generic;
	for (auto& Item : Symbols)
	{

		if (Item->Type == SymbolType::Func)
		{
			if (Item->IsInvalid())
			{
				return {};
			}
			FuncInfo* Info = Item->Get_Info<FuncInfo>();
			Symbol_Update_FuncSym_ToFixedTypes(Item);

			IsCompatiblePar CMPPar;
			CMPPar.SetAsFuncInfo(Item);

			if (!Type_IsCompatible(CMPPar, ValueTypes, _ThisTypeIsNotNull, NeverNullptr(Name._ScopedName.back()._token)))
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
			if (Item->IsInvalid())
			{
				return {};
			}
			FuncInfo* Info = Item->Get_Info<FuncInfo>();
			Symbol_Update_FuncSym_ToFixedTypes(Item);

			bool IsParPack = Info->_GenericData.IsPack();
			bool LastParIsPack = IsParPack && Info->Pars.back().Type._CustomTypeSymbol == Info->_GenericData._Generic.back().SybID;

			if (IsParPack)
			{
				if (LastParIsPack && Info->Pars.size()-1 >= ValueTypes.size())
				{
					continue;
				}
			}
			else 
			{
				if (Info->Pars.size() != ValueTypes.size())
				{
					continue;
				}
			}

			Vector<bool> HasBenAdded;
			HasBenAdded.resize(Info->_GenericData._Generic.size());

			if (LastParIsPack)
			{
				HasBenAdded.resize(Generics._Values.size());
			}
			else
			{
				HasBenAdded.resize(Info->_GenericData._Generic.size());
			}

			Vector<TypeSymbol> GenericInput;
			for (size_t i = 0; i < Generics._Values.size(); i++)
			{
				auto& Item = Generics._Values[i];
				Type_Convert(Item, GenericInput.emplace_back());
				
				if (Type_IsUnMapType(GenericInput.back()))
				{
					Get_FuncInfo V;
					V.CantCheckBecauseIsUnMaped = true;
					return V;//cant check because we are just testing.
				}

				HasBenAdded[i] = true;
			}
		


			
			for (size_t i = 0; i < ValueTypes.size(); i++)
			{
				auto& Item = ValueTypes[i];
				if (_ThisTypeIsNotNull && i == 0) { continue; }

				bool Added = false;

				if (i < Info->Pars.size()) 
				{
					auto& Par = Info->Pars[i];
					for (size_t i2 = 0; i2 < Info->_GenericData._Generic.size(); i2++)
					{
						auto& V3 = Info->_GenericData._Generic[i2];
						if (V3.SybID == Par.Type._CustomTypeSymbol)
						{
							if (i2 >= HasBenAdded.size())
							{
								GenericInput.push_back(Item.Type);
								Added = true;
								continue;
							}
							else if (HasBenAdded[i2] == false)
							{
								GenericInput.push_back(Item.Type);
								HasBenAdded[i2] = true;
								Added = true;
								continue;
							}
							break;
						}
					}
				}
				 
				if (Added == false
					&& LastParIsPack
					&& i >= Info->Pars.size() - 1
					)
				{
					auto PackParsIndex = Info->Pars.size() - 1;
					auto PackAddedPar = i - PackParsIndex;
					
					auto PackGenericIndex = Info->_GenericData._Generic.size() - 1;
					auto PackAddedG = i - PackGenericIndex;
					
					//is logically incorrect

					bool CMPBool = true;
					if (CMPBool) {
						GenericInput.push_back(Item.Type);
					}
				}
			}


			{


				auto FuncSym = Symbol_GetSymbol(Info);
				String NewName = Generic_SymbolGenericFullName(FuncSym, GenericInput);
				auto FuncIsMade = Symbol_GetSymbol(NewName, SymbolType::Func);



				if (!FuncIsMade)
				{

					{

						for (size_t i = 0; i < GenericInput.size(); i++)
						{
							const auto& Tnode = GenericInput[i];

							if (i < Info->_GenericData._Generic.size())
							{
								const auto& GenericInfo = Info->_GenericData._Generic[i];

								bool InputTypeIsConstantExpression = false;

								auto TypeSyb = Symbol_GetSymbol(GenericInfo.SybID);


								InputTypeIsConstantExpression = TypeSyb->Type == SymbolType::ConstantExpression;


								if (InputTypeIsConstantExpression != GenericInfo.IsConstantExpression())
								{
									goto ContinueOutloop;
								}
							}
						}
					}
					auto Pointer = std::make_unique<Vector<TypeSymbol>>(std::move(GenericInput));
					//pointer must be unique so it cant be on the stack

					Generic_GenericFuncInstantiate(FuncSym, *Pointer);

					_TepFuncs.push_back({ std::move(Pointer) });//keep pointer 


					FuncSym = Symbol_GetSymbol(NewName, SymbolType::Func).value();
				}
				else
				{
					FuncSym = FuncIsMade.value();
				}
			



				{
					r = FuncSym->Get_Info<FuncInfo>();
					FuncSymbol = FuncSym.value();

					OkFuncions.push_back({ThisParType,r,FuncSymbol });
				}






			}
			break;
		}
		else if (Item->Type == SymbolType::Type_class)
		{
			ClassInfo* V = Item->Get_Info<ClassInfo>();
			Symbol_Update_ClassSym_ToFixedTypes(Item);
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
						ValueTypes.insert(ValueTypes.begin(), { false,V });
					}

					IsCompatiblePar CMPPar;
					CMPPar.SetAsFuncInfo(Item2);

					bool Compatible = Type_IsCompatible(CMPPar, ValueTypes, _ThisTypeIsNotNull, NeverNullptr(Name._ScopedName.back()._token));
					
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
		else if (Item->Type == SymbolType::Generic_class)
		{
			Symbol_Update_ClassSym_ToFixedTypes(Item);
			ClassInfo* V = Item->Get_Info<ClassInfo>();

			const ClassNode& node = *Item->Get_NodeInfo<ClassNode>();
		
			auto classsybOp = Generic_InstantiateOrFindGeneric_Class(
				NeverNullptr(Name._ScopedName.front()._token),
				Item,
				node._generic, V->_GenericData, Generics);
			if (classsybOp.has_value())
			{
				auto classsyb = classsybOp.value();
				String Scope = classsyb->FullName;
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
							V.SetType(classsyb->ID);
							V.SetAsAddress();
							ValueTypes.insert(ValueTypes.begin(), { false,V });
						}

						IsCompatiblePar CMPPar;
						CMPPar.SetAsFuncInfo(Item2);

						bool Compatible = Type_IsCompatible(CMPPar, ValueTypes, _ThisTypeIsNotNull, NeverNullptr(Name._ScopedName.back()._token));

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
		}
		else if (Symbol_IsVarableType(Item->Type))
		{
			Symbol* Type = Symbol_GetSymbol(Item->VarType).value_unchecked();
			if (Type) 
			{

				if (Type->Type == SymbolType::Func_ptr || Type->Type == SymbolType::Hard_Func_ptr)
				{
					FuncPtrInfo* Info = Type->Get_Info<FuncPtrInfo>();//must be the same as Item->Type == SymbolType::Func

					IsCompatiblePar CMPPar;
					CMPPar.SetAsFuncPtrInfo(Type);

					if (!Type_IsCompatible(CMPPar, ValueTypes, _ThisTypeIsNotNull,NeverNullptr(Name._ScopedName.back()._token)))
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
								ValueTypes.insert(ValueTypes.begin(), { false,V });
							}

							IsCompatiblePar CMPPar;
							CMPPar.SetAsFuncInfo(Item2);

							bool Compatible = Type_IsCompatible(CMPPar, ValueTypes, _ThisTypeIsNotNull, NeverNullptr(Name._ScopedName.back()._token));

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

			auto EnumSymbolop = Symbol_GetSymbol(EnumClassFullName, SymbolType::Enum);
			if (EnumSymbolop)
			{
				auto EnumSymbol = EnumSymbolop.value();
				if (EnumSymbol->Type == SymbolType::Enum)
				{
					EnumInfo* Enuminfo = EnumSymbol->Get_Info<EnumInfo>();
					if (Enuminfo->VariantData.has_value())
					{
						size_t FeildIndex = Enuminfo->GetFieldIndex(ScopeHelper::GetNameFromFullName(Item->FullName)).value();

						return Symbol_GetEnumVariantFunc(EnumSymbol, FeildIndex,Item, Pars,NeverNullptr(Name._ScopedName.back()._token),ValueTypes);
					}
				}
			}
		}
		ContinueOutloop:continue;
	}
	if (OkFuncions.size() == 0)
	{
		bool MayBeAutoThisFuncCall = Name._ScopedName.size() == 1 && Context_IsInThisFuncCall();


		if (ThisParType == Get_FuncInfo::ThisPar_t::NoThisPar && MayBeAutoThisFuncCall && AutoThisCall == false)
		{
			AutoThisCall = true;

			ValueTypes.insert(ValueTypes.begin(), { false ,*_FuncStack.back().Pointer->GetObjectForCall() });
			ThisParType = Get_FuncInfo::ThisPar_t::AutoPushThis;
			goto StartSymbolsLoop;
		}
		else 
		{
			_LastExpressionType = TypeSymbol(TypesEnum::Null);
			LogError_CantFindFuncError(NeverNullptr(Name._ScopedName.back()._token), ScopedName, {}, ValueTypes, RetType);
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
			else if (Symbol_IsVarableType(Item.SymFunc->Type))
			{
				Symbol* Type = Symbol_GetSymbol(Item.SymFunc->VarType).value_unchecked();
				if (Type && (Type->Type == SymbolType::Func_ptr || Type->Type == SymbolType::Hard_Func_ptr))
				{
					CMPPar.SetAsFuncPtrInfo(Type);
				}
				else
				{
					UCodeLangUnreachable();
				}	
			}
			else
			{
				UCodeLangUnreachable();
			}


			int Score = Type_GetCompatibleScore(CMPPar, ValueTypes);
			if (!MinScore.has_value() ||  Score > MinScore.value())
			{
				MinScore = Score;
				Ret = &Item;
			}
		}
		if (Ret == nullptr) {
			UCodeLangUnreachable();
		}
		
		auto RValue = *Ret;
		Symbol_AccessCheck(RValue.SymFunc,NeverNullptr(Name._ScopedName.back()._token));


		bool AutoPassThis = Get_FuncInfo::AddOneToGetParNode(ThisParType);
		for (size_t i = 0; i < ValueTypes.size(); i++)
		{
			auto& Item = ValueTypes[i];
			if (AutoPassThis && i == 0) { continue; }

			const auto& ItemNode = Pars._Nodes[AutoPassThis ? i - 1 : i];
			if (Item.IsOutPar)
			{
				const OutExpression* Item = OutExpression::As(ItemNode.get());

				TypeSymbol TypeSyb;

				if (RValue.SymFunc->Type == SymbolType::Func)
				{
					TypeSyb = RValue.SymFunc->Get_Info<FuncInfo>()->Pars[i].Type;
				}
				else if (RValue.SymFunc->Type == SymbolType::Func_ptr)
				{
					TypeSyb = RValue.SymFunc->Get_Info<FuncInfo>()->Pars[i].Type;
				}
				else
				{
					UCodeLangUnreachable();
				}

				Symbol_SetOutExpression(Item, TypeSyb);
			}
		}

		if (!IsInUnSafeBlock())
		{
			auto token =NeverNullptr(Name._ScopedName.back()._token);
			if (RValue.SymFunc->Type == SymbolType::Func)
			{
				auto funcinfo = RValue.SymFunc->Get_Info<FuncInfo>();

				if (funcinfo->IsUnsafe)
				{
					LogError(ErrorCodes::InValidType, "trying to call 'unsafe' funcion but in safe mode", token);
					return { };
				}
			}
		}

		return RValue;
		
		
	}
	return { };
}

void SystematicAnalysis::Type_RemoveTypeattributes(TypeSymbol& tep_)
{
	if (tep_.IsAddress())
	{
		tep_._IsAddress = false;
	}
	if (tep_.IsAddressArray())
	{
		tep_._IsAddressArray = false;
	}
}

SystematicAnalysis::Get_FuncInfo SystematicAnalysis::Symbol_GetEnumVariantFunc(NeverNullPtr<Symbol> EnumSyb, size_t FeildIndex, NeverNullPtr<Symbol> EnumFieldSyb,const ValueParametersNode& Pars, const NeverNullPtr<Token> Token, const Vector<ParInfo>& ValueTypes)
{
	const EnumInfo* Enuminfo = EnumSyb->Get_Info<EnumInfo>();
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
			LogError_CanIncorrectParCount(Token, FullName, Pars._Nodes.size(), Feild_Variant.Types.size()+1);

			_LastExpressionType = TypeSymbol(EnumSyb->ID);
			return {};
		}


		for (size_t i = 0; i < Pars._Nodes.size(); i++)
		{
			auto& Item = Pars._Nodes[i];


			if (i == 0)
			{
				auto& ExItemType = ValueTypes[i];

				TypeSymbol Vthis = EnumSyb->ID;
				if (!Type_CanBeImplicitConverted(ExItemType.Type, Vthis))
				{
					LogError_CantCastImplicitTypes(Token, ExItemType.Type, Vthis, true);
				}
			}
			else
			{
				auto& ItemVariant = Feild_Variant.Types[i - 1];
				if (Item->Get_Type() != NodeType::OutExpression)
				{
					LogError_ParamterMustBeAnOutExpression(Token, i);
				}
				else
				{
					OutExpression* Ex = OutExpression::As(Item.get());


					auto& TypeToSet = ItemVariant;

					Symbol_SetOutExpression(Ex, TypeToSet);
				}

			}




		}
		Get_FuncInfo r;
		r.ThisPar = Get_FuncInfo::ThisPar_t::NoThisPar_GetValue_EnumVariant;
		r.SymFunc = EnumFieldSyb.value();
		r.Func = nullptr;

		_LastExpressionType = TypeSymbol(TypesEnum::Bool);
		return r;
	}
	else 
	{
		if (Feild_Variant.Types.size() == 1)
		{
			TypeSymbol VoidType(TypesEnum::Void);

			if (Type_AreTheSame(VoidType, Feild_Variant.Types.front()))
			{
				if (Pars._Nodes.size() != 0)
				{
					String FullName = Enuminfo->FullName;
					ScopeHelper::GetApendedString(FullName, Feild.Name);
					LogError_CanIncorrectParCount(Token, FullName, Pars._Nodes.size(), 0);

					_LastExpressionType = TypeSymbol(EnumSyb->ID);
					return {};
				}
				else
				{
					Get_FuncInfo r;
					r.ThisPar = Get_FuncInfo::ThisPar_t::NoThisPar;
					r.SymFunc = EnumFieldSyb.value();
					r.Func = nullptr;

					_LastExpressionType = TypeSymbol(EnumSyb->ID);
					return r;
				}
			}
		}

		if (Feild_Variant.Types.size() != Pars._Nodes.size())
		{
			String FullName = Enuminfo->FullName;
			ScopeHelper::GetApendedString(FullName, Feild.Name);
			LogError_CanIncorrectParCount(Token, FullName, Pars._Nodes.size(), Feild_Variant.Types.size());

			_LastExpressionType = TypeSymbol(EnumSyb->ID);
			return {};
		}

		for (size_t i = 0; i < Feild_Variant.Types.size(); i++)
		{
			auto& Item = Feild_Variant.Types[i];
			auto& ExItemType = ValueTypes[i];

			if (!Type_CanBeImplicitConverted(ExItemType.Type, Item))
			{
				LogError_CantCastImplicitTypes(Token, ExItemType.Type, Item, true);
			}
		}

		Get_FuncInfo r;
		r.ThisPar = Get_FuncInfo::ThisPar_t::NoThisPar;
		r.SymFunc = EnumFieldSyb.value();
		r.Func = nullptr;

		_LastExpressionType = TypeSymbol(EnumSyb->ID);
		return r;
	}
}

void SystematicAnalysis::Symbol_SetOutExpression(const OutExpression* Ex, const TypeSymbol& TypeToSet)
{
	auto Str = Ex->_Name.token->Value._String;

	String FullName = _Table._Scope.ThisScope;
	ScopeHelper::GetApendedString(FullName, Str);

	auto Syb = &Symbol_AddSymbol(SymbolType::StackVarable, (String)Str, FullName, AccessModifierType::Public);

	_LookingForTypes.push(TypeToSet);
	Syb->VarType = Type_ConvertAndValidateType(Ex->_Type, NodeSyb_t::Varable);
	_LookingForTypes.pop();

	Type_DeclareVariableTypeCheck(Syb->VarType, TypeToSet, NeverNullptr(Ex->_Name.token));
	_Table.AddSymbolID(*Syb, Symbol_GetSymbolID(*Ex));
}
NeverNullPtr<Symbol> SystematicAnalysis::Symbol_GetSymbolFromExpression(const NeverNullPtr<OutExpression> Ex)
{
	return Symbol_GetSymbol(Symbol_GetSymbolID(*Ex));
}
void SystematicAnalysis::Eval_SetOutExpressionEval(const OutExpression* Ex, const EvaluatedEx& ObjectToSet)
{
	
	auto Str = Ex->_Name.token->Value._String;

	String FullName = _Table._Scope.ThisScope;
	ScopeHelper::GetApendedString(FullName, Str);

	auto Syb = &Symbol_AddSymbol(SymbolType::ConstantExpression, (String)Str, FullName, AccessModifierType::Public);

	_LookingForTypes.push(ObjectToSet.Type);
	Syb->VarType = Type_ConvertAndValidateType(Ex->_Type, NodeSyb_t::Varable);
	_LookingForTypes.pop();

	Type_DeclareVariableTypeCheck(Syb->VarType, ObjectToSet.Type,NeverNullptr(Ex->_Name.token));
	_Table.AddSymbolID(*Syb, Symbol_GetSymbolID(*Ex));

	ConstantExpressionInfo* Info = new ConstantExpressionInfo();
	Syb->Info.reset(Info);

	Info->Ex = ObjectToSet.EvaluatedObject;
}


String SystematicAnalysis::Generic_SymbolGenericFullName(const NeverNullPtr<Symbol> Func, const Vector<TypeSymbol>& Type) const
{
	#if UCodeLangDebug
	for (auto& Item : Type)
	{
		UCodeLangAssert(!Type_IsUnMapType(Item));//trying use UnMaped Type when Generic Instantiate.
	}
	#endif // DEBUG
	UCodeLangAssert(Type.size());//you need input types for Generic Instantiate.

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
String SystematicAnalysis::Generic_SymbolGenericName(const NeverNullPtr<Symbol> Func, const Vector<TypeSymbol>& Type) const
{
	#if UCodeLangDebug
	for (auto& Item : Type)
	{
		UCodeLangAssert(!Type_IsUnMapType(Item));//trying use UnMaped Type when Generic Instantiate.
	}
	#endif // DEBUG
	UCodeLangAssert(Type.size());//you need input types for Generic Instantiate.

	String NewName = ScopeHelper::GetNameFromFullName(Func->FullName) + "<";
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
String SystematicAnalysis::Generic_GetGenericExtendedErrValue(const Generic& Generic, const GenericValuesNode GenericAsNode, const Vector<TypeSymbol>& GenericInput)
{
	String V = "Were ";


	bool IsPack = Generic.IsPack();
	size_t LoopSize = IsPack ? GenericInput.size() - 1 : GenericInput.size();
	for (size_t i = 0; i < LoopSize; i++)
	{
		const auto& Item = GenericInput[i];
		bool IsExpression = Generic._Generic[i].IsConstantExpression();

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

		size_t Index = Generic._Generic.size() - 1;
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
		size_t Index = Generic._Generic.size() - 1;
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
void SystematicAnalysis::Generic_GenericFuncInstantiate(const NeverNullPtr<Symbol> Func, const Vector<TypeSymbol>& GenericInput)
{
	UCodeLangAssert(Func->Type == SymbolType::GenericFunc);
	


	const String NewName = Generic_SymbolGenericName(Func, GenericInput);
	const String FullName = Generic_SymbolGenericFullName(Func, GenericInput);
	const FuncInfo* FInfo = Func->Get_Info<FuncInfo>();

	{
		auto& GenericData = FInfo->_GenericData;
		Push_GenericInfo(NewName, GenericInput, Func, GenericData);
	}
	
	
	const FuncNode& FuncBase = *Func->Get_NodeInfo<FuncNode>();

	



	
	{
		Push_ExtendedErr(Generic_GetGenericExtendedErrValue(FInfo->_GenericData,FuncBase._Signature._generic,GenericInput), NeverNullptr(FuncBase._Signature._Name.token));
	}
	{
		size_t NewSymbolIndex = _Table.Symbols.size();

		auto OldConext = SaveAndMove_SymbolContext();
		auto Oldpasstype = _PassType;
		auto Olderrcount = _ErrorsOutput->Get_ErrorCount();

		Set_SymbolConext(FInfo->Conext.value());
		_PassType = PassType::GetTypes;
		OnFuncNode(FuncBase);

		auto& addedSymbol = *_Table.Symbols[NewSymbolIndex].get();
		UCodeLangAssert(addedSymbol.FullName == NewName);
		UCodeLangAssert(addedSymbol.Type == SymbolType::Func);
		UCodeLangAssert(addedSymbol.PassState == PassType::GetTypes);


		if (_ErrorsOutput->Get_ErrorCount() <= Olderrcount)
		{
			_PassType = PassType::FixedTypes;
			OnFuncNode(FuncBase);
			UCodeLangAssert(addedSymbol.PassState == PassType::FixedTypes);
		}
		Set_SymbolConext(std::move(OldConext));
		_PassType = Oldpasstype;

		Pop_AddToGeneratedGenricSymbol(addedSymbol, GenericInput);
	}
	{
		Pop_ExtendedErr();
	}

	

	FileDependency_AddDependencyToCurrentFile(Func);
}
void SystematicAnalysis::Push_GenericInfo(const String& NewName, const Vector<TypeSymbol>& GenericInput, const 
	NeverNullPtr<Symbol> Func, const Generic& GenericData)
{
	GenericFuncInfo Info;
	Info._IR_GenericFuncName = NewName;
	Info.GenericInput = &GenericInput;
	Info.NodeTarget = Func->NodePtr;
	Info.Pack = Generic_MakeTypePackSymbolIfNeeded(NewName, GenericInput, GenericData);
	_Generic_GenericSymbolStack.push(std::move(Info));
}
void SystematicAnalysis::Pop_AddToGeneratedGenricSymbol(Symbol& addedSymbol, const Vector<TypeSymbol>& GenericInput)
{

	GeneratedGenericSymbolData NewData;
	NewData.ID = addedSymbol.ID;
	NewData.Info = std::move(_Generic_GenericSymbolStack.top());
	_Generic_GenericSymbolStack.pop();

	NewData.Types = GenericInput;
	NewData.Info.GenericInput = &NewData.Types;

	_Generic_GeneratedGenericSybol.push_back(std::move(NewData));

}

void SystematicAnalysis::Generic_TypeInstantiate(const NeverNullPtr<Symbol> Class, const Vector<TypeSymbol>& GenericInput)
{
	UCodeLangAssert(Class->Type == SymbolType::Generic_class);

	const String NewName = Generic_SymbolGenericName(Class, GenericInput);
	const String FullName = Generic_SymbolGenericFullName(Class, GenericInput);
	const ClassNode* node = ClassNode::As(Class->Get_NodeInfo<Node>());

	const ClassInfo* classInfo = Class->Get_Info<ClassInfo>();

	{
		auto& GenericData = classInfo->_GenericData;
		Push_GenericInfo(NewName, GenericInput, Class, GenericData);
	}

	{
		Push_ExtendedErr(Generic_GetGenericExtendedErrValue(classInfo->_GenericData, node->_generic,GenericInput),NeverNullptr(node->_className.token));
	}
	{
		auto OldConext = SaveAndMove_SymbolContext();
		auto Oldpasstype = _PassType;
		auto Olderrcount = _ErrorsOutput->Get_ErrorCount();

		Set_SymbolConext(classInfo->Conext.value());

		size_t NewSymbolIndex = _Table.Symbols.size();
		_PassType = PassType::GetTypes;
		OnClassNode(*node);

		auto& addedSymbol = *_Table.Symbols[NewSymbolIndex].get();
		UCodeLangAssert(addedSymbol.FullName == NewName);
		UCodeLangAssert(addedSymbol.Type == SymbolType::Type_class);
		UCodeLangAssert(addedSymbol.PassState == PassType::GetTypes);

		if (_ErrorsOutput->Get_ErrorCount() <= Olderrcount)
		{
			_PassType = PassType::FixedTypes;
			OnClassNode(*node);

			UCodeLangAssert(addedSymbol.PassState == PassType::FixedTypes);
		}

		Pop_AddToGeneratedGenricSymbol(addedSymbol, GenericInput);
		


	
		//
		Set_SymbolConext(std::move(OldConext));
		_PassType = Oldpasstype;
		//
	}
	{
		Pop_ExtendedErr();
	}

	FileDependency_AddDependencyToCurrentFile(Class);
}

void SystematicAnalysis::Generic_TypeInstantiate_Trait(const NeverNullPtr<Symbol> Trait, const Vector<TypeSymbol>& GenericInput)
{
	UCodeLangAssert(Trait->Type == SymbolType::Trait_class);

	const String NewName = Generic_SymbolGenericName(Trait, GenericInput);
	const String FullName = Generic_SymbolGenericFullName(Trait, GenericInput);
	const TraitNode* node = TraitNode::As(Trait->Get_NodeInfo<Node>());

	const TraitInfo* classInfo = Trait->Get_Info<TraitInfo>();


	{
		auto& GenericData = classInfo->_GenericData;
		Push_GenericInfo(NewName, GenericInput, Trait, GenericData);
	}
	

	
	{
		Push_ExtendedErr(Generic_GetGenericExtendedErrValue(classInfo->_GenericData, node->_generic, GenericInput), NeverNullptr(node->_Name.token));
	}
	{
		auto OldConext = SaveAndMove_SymbolContext();
		auto Oldpasstype = _PassType;
		auto Olderrcount = _ErrorsOutput->Get_ErrorCount();

		Set_SymbolConext(classInfo->Conext.value());

		size_t NewSymbolIndex = _Table.Symbols.size();

		_PassType = PassType::GetTypes;
		OnTrait(*node);


		auto& addedSymbol = *_Table.Symbols[NewSymbolIndex].get();
		UCodeLangAssert(addedSymbol.FullName == NewName);
		UCodeLangAssert(addedSymbol.Type == SymbolType::Trait_class);
		UCodeLangAssert(addedSymbol.PassState == PassType::GetTypes);


		if (_ErrorsOutput->Get_ErrorCount() <= Olderrcount)
		{
			_PassType = PassType::FixedTypes;
			OnTrait(*node);

			UCodeLangAssert(addedSymbol.PassState == PassType::FixedTypes);
		}

		Pop_AddToGeneratedGenricSymbol(addedSymbol, GenericInput);

		//
		Set_SymbolConext(std::move(OldConext));
		_PassType = Oldpasstype;
		//
	}
	{
		Pop_ExtendedErr();
	}

	FileDependency_AddDependencyToCurrentFile(Trait);
}

void SystematicAnalysis::Generic_TypeInstantiate_Alias(const NeverNullPtr<Symbol> Alias, const Vector<TypeSymbol>& GenericInput)
{
	UCodeLangAssert(Alias->Type == SymbolType::Generic_Alias);

	const String NewName = Generic_SymbolGenericName(Alias, GenericInput);
	const String FullName = Generic_SymbolGenericFullName(Alias, GenericInput);
	const AliasNode* node = AliasNode::As(Alias->Get_NodeInfo<Node>());

	const Generic_AliasInfo* classInfo = Alias->Get_Info<Generic_AliasInfo>();

	{
		auto& GenericData = classInfo->_GenericData;
		Push_GenericInfo(NewName, GenericInput, Alias, GenericData);
	}
	{
		Push_ExtendedErr(Generic_GetGenericExtendedErrValue(classInfo->_GenericData, node->Generic, GenericInput),NeverNullptr(node->_AliasName.token));
	}
	{
		auto OldConext = SaveAndMove_SymbolContext();
		auto Oldpasstype = _PassType;
		auto Olderrcount = _ErrorsOutput->Get_ErrorCount();

		Set_SymbolConext(classInfo->Conext.value());

		size_t NewSymbolIndex = _Table.Symbols.size();

		_PassType = PassType::GetTypes;
		OnAliasNode(*node);


		auto& addedSymbol = *_Table.Symbols[NewSymbolIndex].get();

		UCodeLangAssert(addedSymbol.FullName == NewName);
		UCodeLangAssert(addedSymbol.Type == SymbolType::Type_alias 
			|| addedSymbol.Type == SymbolType::Hard_Type_alias 
			|| addedSymbol.Type == SymbolType::Func_ptr 
			|| addedSymbol.Type == SymbolType::Hard_Func_ptr);
		UCodeLangAssert(addedSymbol.PassState == PassType::GetTypes);



		if (_ErrorsOutput->Get_ErrorCount() <= Olderrcount)
		{
			_PassType = PassType::FixedTypes;
			OnAliasNode(*node);

			UCodeLangAssert(addedSymbol.PassState == PassType::FixedTypes);
		}

		Pop_AddToGeneratedGenricSymbol(addedSymbol, GenericInput);
		//
		Set_SymbolConext(std::move(OldConext));
		_PassType = Oldpasstype;
		//
	}
	{
		Pop_ExtendedErr();
	}

	FileDependency_AddDependencyToCurrentFile(Alias);
}

void SystematicAnalysis::Generic_TypeInstantiate_Enum(const NeverNullPtr<Symbol> Enum, const Vector<TypeSymbol>& GenericInput)
{
	UCodeLangAssert(Enum->Type == SymbolType::Generic_Enum);

	const String NewName = Generic_SymbolGenericName(Enum, GenericInput);
	const String FullName = Generic_SymbolGenericFullName(Enum, GenericInput);
	const EnumNode* node = EnumNode::As(Enum->Get_NodeInfo<Node>());

	const EnumInfo* classInfo = Enum->Get_Info<EnumInfo>();

	{
		auto& GenericData = classInfo->_GenericData;
		Push_GenericInfo(NewName, GenericInput, Enum, GenericData);
	}

	{
		Push_ExtendedErr(Generic_GetGenericExtendedErrValue(classInfo->_GenericData, node->_generic, GenericInput),NeverNullptr(node->_EnumName.token));
	}
	{
		auto OldConext = SaveAndMove_SymbolContext();
		auto Oldpasstype = _PassType;
		auto Olderrcount = _ErrorsOutput->Get_ErrorCount();

		Set_SymbolConext(classInfo->Conext.value());

		size_t NewSymbolIndex = _Table.Symbols.size();

		_PassType = PassType::GetTypes;
		OnEnum(*node);


		auto& addedSymbol = *_Table.Symbols[NewSymbolIndex].get();

		UCodeLangAssert(addedSymbol.FullName == NewName);
		UCodeLangAssert(addedSymbol.Type == SymbolType::Enum);
		UCodeLangAssert(addedSymbol.PassState == PassType::GetTypes);


		if (_ErrorsOutput->Get_ErrorCount() <= Olderrcount)
		{
			_PassType = PassType::FixedTypes;
			OnEnum(*node);

			UCodeLangAssert(addedSymbol.PassState == PassType::FixedTypes);
		}


		Pop_AddToGeneratedGenricSymbol(addedSymbol, GenericInput);
		//
		Set_SymbolConext(std::move(OldConext));
		_PassType = Oldpasstype;
		//
	}
	{
		Pop_ExtendedErr();
	}

	FileDependency_AddDependencyToCurrentFile(Enum);
}

void SystematicAnalysis::Generic_TypeInstantiate_Tag(const NeverNullPtr<Symbol> Tag, const Vector<TypeSymbol>& GenericInput)
{
	UCodeLangAssert(Tag->Type == SymbolType::Generic_Tag);

	const String NewName = Generic_SymbolGenericName(Tag, GenericInput);
	const String FullName = Generic_SymbolGenericFullName(Tag, GenericInput);
	const TagTypeNode* node = TagTypeNode::As(Tag->Get_NodeInfo<Node>());

	const TagInfo* classInfo = Tag->Get_Info<TagInfo>();

	{
		auto& GenericData = classInfo->_GenericData;
		Push_GenericInfo(NewName, GenericInput, Tag, GenericData);
	}
	{
		Push_ExtendedErr(Generic_GetGenericExtendedErrValue(classInfo->_GenericData, node->_generic, GenericInput),NeverNullptr(node->_AttributeName.token));
	}
	{
		auto OldConext = SaveAndMove_SymbolContext();
		auto Oldpasstype = _PassType;
		auto Olderrcount = _ErrorsOutput->Get_ErrorCount();

		Set_SymbolConext(classInfo->Conext.value());

		_Table._Scope.ThisScope = ScopeHelper::GetReMoveScope(FullName);

		size_t NewSymbolIndex = _Table.Symbols.size();

		_PassType = PassType::GetTypes;
		OnTag(*node);


		auto& addedSymbol = *_Table.Symbols[NewSymbolIndex].get();

		UCodeLangAssert(addedSymbol.FullName == NewName);
		UCodeLangAssert(addedSymbol.Type == SymbolType::Tag_class);
		UCodeLangAssert(addedSymbol.PassState == PassType::GetTypes);


		if (_ErrorsOutput->Get_ErrorCount() <= Olderrcount)
		{
			_PassType = PassType::FixedTypes;
			OnTag(*node);

			UCodeLangAssert(addedSymbol.PassState == PassType::FixedTypes);
		}


		Pop_AddToGeneratedGenricSymbol(addedSymbol, GenericInput);
		//
		Set_SymbolConext(std::move(OldConext));
		_PassType = Oldpasstype;
		//
	}
	{
		Pop_ExtendedErr();
	}

	FileDependency_AddDependencyToCurrentFile(Tag);
}


//

void* SystematicAnalysis::Eval_Get_Object(const TypeSymbol& Input, const RawEvaluatedObject& Input2)
{
	return Input2.Object_AsPointer.get();
}

void* SystematicAnalysis::Eval_Get_Object(const EvaluatedEx& Input)
{
	return Eval_Get_Object(Input.Type, Input.EvaluatedObject);
}
const void* SystematicAnalysis::Eval_Get_Object(const TypeSymbol& Input, const RawEvaluatedObject& Input2) const
{
	return Input2.Object_AsPointer.get();
}

const void* SystematicAnalysis::Eval_Get_Object(const EvaluatedEx& Input) const
{
	return Eval_Get_Object(Input.Type, Input.EvaluatedObject);
}

SystematicAnalysis::StrExELav SystematicAnalysis::Eval_GetStrEVal(const Node* node)
{
	StrExELav CompilerRet;

	OnExpressionTypeNode(node, GetValueMode::Read);

	if (node->Get_Type() == NodeType::ValueExpressionNode)
	{
		const ValueExpressionNode* V = ValueExpressionNode::As(node);
		if (V->_Value.get()->Get_Type() == NodeType::StringliteralNode)
		{
			StringliteralNode* strnod = StringliteralNode::As(V->_Value.get());

			String Buffer;
			ParseHelper::ParseStringliteralToString(strnod->token->Value._String, Buffer);
			CompilerRet.OwnedStr = std::move(Buffer);
			CompilerRet.Msg = CompilerRet.OwnedStr;
			CompilerRet.IsConstantExpression = true;
		}
	}

	return CompilerRet;
}
bool SystematicAnalysis::Eavl_ConstantExpressionAbleType(const TypeSymbol& Type)
{
	return Type_IsPrimitive(Type) && !Type.IsAddress() && !Type.IsAddressArray();
}

SystematicAnalysis::EvaluatedEx SystematicAnalysis::Eval_MakeEx(const TypeSymbol& Type)
{
	EvaluatedEx r;
	r.Type = Type;

	size_t Size = 0;
	Type_GetSize(Type, Size);

	r.EvaluatedObject.Object_AsPointer = std::make_unique<Byte[]>(Size);
	r.EvaluatedObject.ObjectSize = Size;

	return r;
}

bool SystematicAnalysis::Type_IsCompatible(const IsCompatiblePar& FuncPar,const Vector<ParInfo>& ValueTypes, bool _ThisTypeIsNotNull, const NeverNullPtr<Token> Token)
{

	if (FuncPar.Pars->size() != ValueTypes.size())
	{
		return false;
	}


	//
	if ((PassType_t)FuncPar.Item->PassState < (PassType_t)_PassType)
	{
		if (FuncPar.Item->Type != SymbolType::Func)
		{
			UCodeLangUnreachable();
		}

		FuncInfo* Info = FuncPar.Item->Get_Info<FuncInfo>();
		if (!Symbol_IsDependencies(Info))
		{
			auto OldPass = _PassType;
			auto oldconext = SaveAndMove_SymbolContext();
			
			Set_SymbolConext(Info->Conext.value());

			_RetLoopStack.push_back(Info);

			OnFuncNode(*FuncPar.Item->Get_NodeInfo<FuncNode>());

			_RetLoopStack.pop_back();

			Set_SymbolConext(std::move(oldconext));
		}
		else
		{
			auto V = Symbol_GetDependencies(Info);
			if (V->IsOnRetStatemnt && Info->Ret.IsNull())
			{
				LogError_FuncDependencyCycle(Token, Info);
				Info->Ret.SetType(TypesEnum::Null);//to stop err spam
				return false;
			}
			else
			{
				return true;//ret got fixed
			}
			
		}
	}
	//

	for (size_t i = _ThisTypeIsNotNull ? 1 : 0; i < FuncPar.Pars->size(); i++)
	{
		auto& Item = (*FuncPar.Pars)[i];
		auto& Item2 = ValueTypes[i];

		if (Item.IsOutPar != Item2.IsOutPar)
		{
			return false;
		}
		else if (Item2.IsOutPar && Item2.Type.IsAn(TypesEnum::Var))
		{
			//is ok
			continue;
		}

		if (!Type_CanBeImplicitConverted(Item2.Type, Item.Type, true))
		{
			return false;
		}
	}
	return true;
}

int SystematicAnalysis::Type_GetCompatibleScore(const ParInfo& ParFunc, const ParInfo& Value)
{
	int r = 0;
	
	if (Type_AreTheSameWithOutimmutable(ParFunc.Type,Value.Type))
	{
		r += 5;
	}
	else if (Type_CanBeImplicitConverted(ParFunc.Type, Value.Type, false))
	{
		r += 3;
	}
	else if (Type_CanBeExplicitlyConverted(ParFunc.Type,Value.Type).HasValue)
	{
		r += 1;
	}

	if (ParFunc.IsOutPar == Value.IsOutPar)
	{
		r += 1;
	}
	if (ParFunc.Type.IsAddress() == Value.Type.IsAddress())
	{
		r += 1;
	}
	if (ParFunc.Type.Isimmutable() == Value.Type.Isimmutable())
	{
		r += 1;
	}


	return r;
}

int SystematicAnalysis::Type_GetCompatibleScore(const IsCompatiblePar& Func, const Vector<ParInfo>& ValueTypes)
{
	bool IsNewFunc = false;

	size_t StartIndex = 0;

	if (Func.Item)
	{
		if (Func.Item->Type == SymbolType::Func)
		{
			auto Info = Func.Item->Get_Info<FuncInfo>();
			if (Info->_FuncType == FuncInfo::FuncType::New)
			{
				StartIndex++;
			}
		}
	}
	int r = 0;
	for (size_t i = StartIndex; i < (*Func.Pars).size(); i++)
	{
		size_t ValueTypesIndex = StartIndex == 1 ? i - 1 : i;
		r += Type_GetCompatibleScore((*Func.Pars)[i], ValueTypes[ValueTypesIndex]);
	}


	return (*Func.Pars).size() ? (int)(r / (*Func.Pars).size()) : (int)r;
}
bool SystematicAnalysis::Symbol_AccessCheck(const NeverNullPtr<Symbol> Syb,const NeverNullPtr<Token> Token, const String_view Scope)
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

bool SystematicAnalysis::Type_IsStringSpan8(const TypeSymbol& type)
{
	auto symOp = Symbol_GetSymbol(type);
	if (symOp.has_value())
	{
		auto sym = symOp.value();
		auto& name = sym->FullName;
		return StringHelper::Contains(name, Uft8typeName)
			|| StringHelper::Contains(name, UCodeLangStringSpan8Type);

	}
	return false;
}

bool SystematicAnalysis::Type_IsStringSpan16(const TypeSymbol& type)
{
	auto symOp = Symbol_GetSymbol(type);
	if (symOp.has_value())
	{
		auto sym = symOp.value();
		auto& name = sym->FullName;
		return StringHelper::Contains(name, Uft16typeName)
			|| StringHelper::Contains(name, UCodeLangStringSpan16Type);
	}
	return false;
}

bool SystematicAnalysis::Type_IsStringSpan32(const TypeSymbol& type)
{
	auto symOp = Symbol_GetSymbol(type);
	if (symOp.has_value())
	{
		auto sym = symOp.value();
		const auto& name = sym->FullName;
		return StringHelper::Contains(name, Uft32typeName)
			|| StringHelper::Contains(name, UCodeLangStringSpan32Type);
	}
	return false;
}

bool SystematicAnalysis::Type_IsString8(const TypeSymbol& type)
{
	auto symOp = Symbol_GetSymbol(type);
	if (symOp.has_value())
	{
		auto sym = symOp.value();
		const auto& name = sym->FullName;
		return StringHelper::Contains(name, Uft8typeName)
			|| StringHelper::Contains(name, UCodeLangString8Type);
	}
	return false;
}

bool SystematicAnalysis::Type_IsString16(const TypeSymbol& type)
{
	auto symOp = Symbol_GetSymbol(type);
	if (symOp.has_value())
	{
		auto sym = symOp.value();
		const auto& name = sym->FullName;
		return StringHelper::Contains(name, Uft16typeName)
			|| StringHelper::Contains(name, UCodeLangString16Type);
	}
	return false;
}

bool SystematicAnalysis::Type_IsString32(const TypeSymbol& type)
{
	auto symOp = Symbol_GetSymbol(type);
	if (symOp.has_value())
	{
		auto sym = symOp.value();
		const auto& name = sym->FullName;
		return StringHelper::Contains(name, Uft32typeName)
			|| StringHelper::Contains(name, UCodeLangString32Type);
	}
	return false;
}

Optional<TypeSymbol> SystematicAnalysis::Type_GetStringSpan(const NeverNullPtr<Token> Token)
{
	auto symOp = Symbol_GetSymbol(UCodeLangStringSpanType, SymbolType::Type_class);
	if (symOp)
	{
		auto sym = symOp.value();
		return sym->VarType;
	}
	else
	{
		LogError_CantFindTypeError(Token, UCodeLangStringSpanType);
	}

	return {};
}

Optional<TypeSymbol> SystematicAnalysis::Type_GetStringSpan8(const NeverNullPtr<Token> Token)
{
	auto symOp = Symbol_GetSymbol(UCodeLangStringSpan8Type, SymbolType::Type_class);
	if (symOp)
	{
		auto sym = symOp.value();
		return sym->VarType;
	}
	else
	{
		LogError_CantFindTypeError(Token, UCodeLangStringSpan8Type);
	}

	return {};
}

Optional<TypeSymbol> SystematicAnalysis::Type_GetStringSpan16(const NeverNullPtr<Token> Token)
{
	auto symOp = Symbol_GetSymbol(UCodeLangStringSpan16Type, SymbolType::Type_class);
	if (symOp)
	{
		auto sym = symOp.value();
		return sym->VarType;
	}
	else
	{
		LogError_CantFindTypeError(Token, UCodeLangStringSpan16Type);
	}

	return {};
}

Optional<TypeSymbol> SystematicAnalysis::Type_GetStringSpan32(const NeverNullPtr<Token> Token)
{
	auto symOp = Symbol_GetSymbol(UCodeLangStringSpan32Type, SymbolType::Type_class);
	if (symOp)
	{
		auto sym = symOp.value();
		return sym->VarType;
	}
	else
	{
		LogError_CantFindTypeError(Token, UCodeLangStringSpan32Type);
	}

	return {};
}

RawEvaluatedObject SystematicAnalysis::Eval_MakeExr(const TypeSymbol& Type)
{
	UCodeLangUnreachable();
	return RawEvaluatedObject();
}

bool SystematicAnalysis::Eval_EvaluateDefaultConstructor(EvaluatedEx& Out)
{
	void* Object = Eval_Get_Object(Out);
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
		Type_GetSize(V, PtrSize);
		switch (PtrSize)
		{
		case sizeof(UInt8): *(UInt8*)Object = 0; break;
		case sizeof(UInt16) : *(UInt16*)Object = 0; break;
		case sizeof(UInt32) : *(UInt32*)Object = 0; break;
		case sizeof(UInt64) : *(UInt64*)Object = 0; break;
		default:
			UCodeLangUnreachable();
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
		UCodeLangUnreachable();
		break;
	}
	return false;
}

bool SystematicAnalysis::Eval_Evaluate(EvaluatedEx& Out, const ValueExpressionNode& node)
{
	switch (node._Value->Get_Type())
	{
	case NodeType::NumberliteralNode:
	{
		NumberliteralNode* num = NumberliteralNode::As(node._Value.get());
#define Set_NumberliteralNodeU2(x) \
			UInt##x V; \
			ParseHelper::ParseStringToUInt##x(Str, V); \
			Eval_Set_ObjectAs(Out,V);\

#define Set_NumberliteralNodeS2(x) \
			Int##x V; \
			ParseHelper::ParseStringToInt##x(Str, V); \
			Eval_Set_ObjectAs(Out,V);\

		auto& lookT = Type_Get_LookingForType();
		TypesEnum NewEx;
		if (lookT._Type == TypesEnum::Var)
		{
			NewEx = TypesEnum::sInt32;
		}
		else
		{
			NewEx = (Type_IsfloatType(lookT) || Type_IsIntType(lookT)) ? lookT._Type : TypesEnum::sInt32;
		}
			//if (passtype == PassType::BuidCode)
		{
			auto& Str = num->token->Value._String;



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
				Type_GetSize(V, PtrSize);
				switch (PtrSize)
				{
					case sizeof(UInt8) : goto sint8case;
					case sizeof(UInt16) : goto sint16case;
					case sizeof(UInt32) : goto sint32case;
					case sizeof(UInt64) : goto sint64case;
					default:
						UCodeLangUnreachable();
					break;
				}
			};
			break;

			case TypesEnum::float32:
			{
				Int32 V;
				ParseHelper::ParseStringToInt32(Str, V);
				Eval_Set_ObjectAs(Out, (float32)V);
				break;
			};
			case TypesEnum::float64:
			{
				Int64 V;
				ParseHelper::ParseStringToInt64(Str, V);
				Eval_Set_ObjectAs(Out, (float64)V);
				break;
			};
			default:
				UCodeLangUnreachable();
				break;
			}

		}


		_LastExpressionType.SetType(NewEx);
		_LastLookedAtToken = Nullableptr(num->token);
	}
	break;
	case NodeType::BoolliteralNode:
	{
		BoolliteralNode* num = BoolliteralNode::As(node._Value.get());

		//if (passtype == PassType::BuidCode)
		{
			Eval_Set_ObjectAs(Out, num->Get_Value());
		}
		_LastExpressionType.SetType(TypesEnum::Bool);
		_LastLookedAtToken = Nullableptr(num->token);
	}
	break;
	case NodeType::CharliteralNode:
	{
		CharliteralNode* num = CharliteralNode::As(node._Value.get());

		//if (passtype == PassType::BuidCode)
		{
			String V;
			bool ItWorked = !ParseHelper::ParseCharliteralToChar(num->token->Value._String, V);

			Eval_Set_ObjectAs(Out, (UInt8)V.front());
		}
		_LastExpressionType.SetType(TypesEnum::Char);
		_LastLookedAtToken = Nullableptr(num->token);
	}
	break;
	case NodeType::FloatliteralNode:
	{
		FloatliteralNode* num = FloatliteralNode::As(node._Value.get());
		auto& lookT = Type_Get_LookingForType();
		
		auto& Str = num->token->Value._String;
		

		TypesEnum NewEx;
		if (lookT._Type == TypesEnum::Var)
		{
			NewEx = TypesEnum::float32;
		}
		else
		{
			NewEx = Type_IsfloatType(lookT) ? lookT._Type : TypesEnum::float32;
		}
		//if (passtype == PassType::BuidCode)
		{
			switch (NewEx)
			{
			case TypesEnum::float32:
			{
				float32 V;
				ParseHelper::ParseStringTofloat32(Str, V);
				Eval_Set_ObjectAs(Out, V);
				break;
			}
			case TypesEnum::float64:
			{
				float64 V;
				ParseHelper::ParseStringTofloat64(Str, V);
				Eval_Set_ObjectAs(Out, V);
				break;
			}
			default:
				UCodeLangUnreachable();
				break;
			}
		}
		


		_LastExpressionType.SetType(NewEx);
		_LastLookedAtToken = Nullableptr(num->token);
	}
	case NodeType::StringliteralNode:
	{
		StringliteralNode* nod = StringliteralNode::As(node._Value.get());
		auto& lookT = Type_Get_LookingForType();

		bool IsStaticArr = Type_IsStaticCharArr(lookT);
		if (IsStaticArr)
		{
			auto VSyb = Symbol_GetSymbol(lookT).value();
			StaticArrayInfo* StaticArr = VSyb->Get_Info<StaticArrayInfo>();


			String V;
			bool ItWorked = !ParseHelper::ParseStringliteralToString(nod->token->Value._String, V);
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
					const NeverNullPtr<Token> Token =_LastLookedAtToken.value();
					LogError_CanIncorrectStaticArrCount(Token, lookT, BufferSize, StaticArr->Count);
					_LastExpressionType.SetType(TypesEnum::Null);
					return false;
				}
			}


			Out = Eval_MakeEx(lookT);
			Eval_Set_ObjectAs(Out, V.data(), V.size());
			_LastExpressionType = lookT;
		}
		else
		{
			auto Token = nod->token;
			LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos, "Cant use char[&] in Compile Time.");
			_LastExpressionType.SetType(TypesEnum::Null);
			return false;
		}
	}
	break;
	case NodeType::SizeofExpresionNode:
	{
		SizeofExpresionNode* nod = SizeofExpresionNode::As(node._Value.get());

		auto& lookT = Type_Get_LookingForType();
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
			Type_Convert(nod->_Type, Info);
			UAddress TypeSize;
			Type_GetSize(Info, TypeSize);



			switch (lookT._Type)
			{
			case TypesEnum::sInt8:
				Eval_Set_ObjectAs(Out, (Int8)TypeSize);
				break;
			Int8Case:
			case TypesEnum::uInt8:
				Eval_Set_ObjectAs(Out, (UInt8)TypeSize);
				break;
			case TypesEnum::sInt16:
				Eval_Set_ObjectAs(Out, (Int16)TypeSize);
				break;
			Int16Case:
			
			case TypesEnum::uInt16:
				Eval_Set_ObjectAs(Out, (UInt16)TypeSize);
				break;
			
			case TypesEnum::sInt32:
				Eval_Set_ObjectAs(Out, (Int32)TypeSize);
				break;
			Int32Case:
			case TypesEnum::uInt32:
				Eval_Set_ObjectAs(Out, (UInt32)TypeSize);
				break;
			
			case TypesEnum::sInt64:
				Eval_Set_ObjectAs(Out, (Int64)TypeSize);
				break;
			Int64Case:
			case TypesEnum::uInt64:
				Eval_Set_ObjectAs(Out, (UInt64)TypeSize);
				break;
			default:
			{
				Type.SetType(TypesEnum::uIntPtr);

				UAddress PtrSize;
				Type_GetSize(Info, PtrSize);

				switch (PtrSize)
				{
					case sizeof(UInt8) : goto Int8Case;
					case sizeof(UInt16) : goto Int16Case;
					case sizeof(UInt32) : goto Int32Case;
					case sizeof(UInt64) : goto Int64Case;
				default:
					UCodeLangUnreachable();
					break;
				}

			}break;
			}
		}

		_LastExpressionType = Type;
	}
	break;
	case NodeType::ReadVariableNode:
	{
		ReadVariableNode* nod = ReadVariableNode::As(node._Value.get());
		Eval_Evaluate(Out,*nod);
	}
	break;
	case NodeType::CMPTypesNode:
	{
		CMPTypesNode* nod = CMPTypesNode::As(node._Value.get());
		return Eval_EvalutateCMPTypesNode(Out,*nod);
	}
	break;
	case NodeType::ValidNode:
	{
		ValidNode* nod = ValidNode::As(node._Value.get());
		return Eval_EvalutateValidNode(Out, *nod);
	}
	break;
	case NodeType::TypeToValueNode:
	{
		OnTypeToValueNode(*TypeToValueNode::As(node._Value.get()));
		Eval_Set_ObjectAs(Out, _LastExpressionType);
		return true;
	}
	break;
	case NodeType::ExpressionToTypeValueNode:
	{
		OnExpressionToTypeValueNode(*ExpressionToTypeValueNode::As(node._Value.get()));
		Eval_Set_ObjectAs(Out, _LastExpressionType);
		return true;
	}
	break;
	case NodeType::FuncCallNode:
	{
		return Eval_EvalutateFunc(Out, *FuncCallNode::As(node._Value.get()));
	}
	break;
	default:
		UCodeLangUnreachable();
		break;
	}
	return true;
}
bool SystematicAnalysis::Eval_Evaluate(EvaluatedEx& Out, const ReadVariableNode& nod)
{
	GetMemberTypeSymbolFromVar_t V;
	return Eval_EvalutateScopedName(Out, nod._VariableName, V);
}
bool SystematicAnalysis::Eval_Evaluate(EvaluatedEx& Out, const BinaryExpressionNode& node)
{
	auto Ex0node = node._Value0._Value.get();
	auto Ex1node = node._Value1._Value.get();
	if (_PassType == PassType::BuidCode && 
		(
		Node_SwapForOperatorPrecedence(Ex0node, Ex1node) && Node_SwapForOperatorPrecedence(&node, Ex1node)//i have no clue why this works
		)
		)
	{
		std::swap(Ex0node, Ex1node);
	}

	return false;
}

bool SystematicAnalysis::Eval_Evaluate(EvaluatedEx& Out, const CastNode& node)
{
	TypeSymbol ToTypeAs;
	Type_ConvertAndValidateType(node._ToType, ToTypeAs, NodeSyb_t::Any);

	_LookingForTypes.push(ToTypeAs);

	bool Ex0Bool = Eval_Evaluate_t(Out,node._Expression._Value.get(),GetValueMode::Read);

	_LookingForTypes.pop();


	auto Ex0Type = _LastExpressionType;
	auto HasInfo = Type_CanBeExplicitlyConverted(Ex0Type, ToTypeAs);
	if (!HasInfo.HasValue)
	{
		auto  Token = node._ToType._name.token;

		LogError_CantCastExplicityTypes(NeverNullptr(Token), Ex0Type, ToTypeAs);
		return false;
	}
	else
	{
		if (HasInfo.Value.has_value())
		{
			return false;
		}
		else
		{
			_LastExpressionType = ToTypeAs;
		}

	}
	return true;
}

bool SystematicAnalysis::Eval_Evaluate_t(EvaluatedEx& Out, const Node* node, GetValueMode Mode)
{
	_GetExpressionMode.push(Mode);
	bool R =false;
	switch (node->Get_Type())
	{
	case NodeType::BinaryExpressionNode: R = Eval_Evaluate(Out,*BinaryExpressionNode::As(node)); break;
	case NodeType::ValueExpressionNode: R = Eval_Evaluate(Out, *ValueExpressionNode::As(node)); break;
	case NodeType::CastNode: R = Eval_Evaluate(Out, *CastNode::As(node)); break;
	case NodeType::ExtendedFuncExpression: R = Eval_Evaluate(Out, *ExtendedFuncExpression::As(node)); break;
	case NodeType::ExtendedScopeExpression: R = Eval_Evaluate(Out, *ExtendedScopeExpression::As(node)); break;
	default:
		UCodeLangUnreachable();
		break;
	}
	_GetExpressionMode.pop();

	return R;
}

bool SystematicAnalysis::Eval_Evaluate(EvaluatedEx& Out, const ExpressionNodeType& node, GetValueMode Mode)
{
	return Eval_Evaluate_t(Out, node._Value.get(),Mode);
}

bool SystematicAnalysis::Eval_EvaluatePostfixOperator(EvaluatedEx& Out, TokenType Op)
{
	void* Object = Eval_Get_Object(Out);
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
				(*(UInt8*)Object)--;
			}
			else
			{
				UCodeLangUnreachable();
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
				(*(UInt16*)Object)--;
			}
			else
			{
				UCodeLangUnreachable();
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
				(*(UInt32*)Object)--;
			}
			else
			{
				UCodeLangUnreachable();
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
				(*(UInt64*)Object)--;
			}
			else
			{
				UCodeLangUnreachable();
			}
	}
	break;

	case TypesEnum::uIntPtr:
	case TypesEnum::sIntPtr:
	{
		size_t PtrSize = 0;
		TypeSymbol V;
		V.SetType(TypesEnum::uIntPtr);
		Type_GetSize(V, PtrSize);
		switch (PtrSize)
		{
			case sizeof(UInt8) : goto uint8case;
			case sizeof(UInt16) : goto uint16case;
			case sizeof(UInt32) : goto uint32case;
			case sizeof(UInt64) : goto uint64case;
			default:
				UCodeLangUnreachable();
			break;
		}
	}break;
	default:
		UCodeLangUnreachable();
		break;
	}
	return true;
}

bool SystematicAnalysis::Eval_HasConstantPostfixOperator(const TypeSymbol& Type, TokenType Op)
{
	if ((Op == TokenType::increment || Op == TokenType::decrement) && Type_IsIntType(Type))
	{
		return true;
	}
	return false;
}

bool SystematicAnalysis::Eval_CanEvaluateImplicitConversionConstant(const TypeSymbol& Type, const TypeSymbol& ToType)
{
	if (Type_AreTheSame(Type, ToType))
	{
		return true;
	}
	return false;
}

bool SystematicAnalysis::Eval_EvaluateImplicitConversion(EvaluatedEx& In, const TypeSymbol& ToType, EvaluatedEx& out)
{
	if (Type_AreTheSame(In.Type, ToType))
	{
		out.Type = In.Type;
		out.EvaluatedObject = In.EvaluatedObject;
		return true;
	}
	return false;
}
bool SystematicAnalysis::Eval_EvalutateStepScopedName(EvaluatedEx& Out, const ScopedNameNode& node, size_t Index, ScopedName::Operator_t OpType, GetMemberTypeSymbolFromVar_t& OtherOut)
{
	return false;
}
bool SystematicAnalysis::Eval_CanEvalutateFuncCheck(const Get_FuncInfo& Func)
{
	if (Func._BuiltFunc.has_value())
	{
		return true;
	}

	if (Func.SymFunc && Func.SymFunc->NodePtr)
	{
		const FuncNode* node = FuncNode::As(Func.SymFunc->Get_NodeInfo<Node>());

		if (node->_Signature._HasEvalKeyWord)
		{
			return true;
		}

	}


	return false;
}
bool SystematicAnalysis::Eval_Evaluate(EvaluatedEx& Out, const TypeSymbol& MustBeType, const ExpressionNodeType& node)
{
	OnExpressionTypeNode(node._Value.get(), GetValueMode::Read);//check
	if (!Type_CanBeImplicitConverted(_LastExpressionType, MustBeType, false))
	{
		LogError_CantCastImplicitTypes(_LastLookedAtToken.value(), _LastExpressionType, MustBeType, false);
		return false;
	}
	if (!Eval_CanEvaluateImplicitConversionConstant(_LastExpressionType, MustBeType))
	{
		LogError_Eval_CantCastImplicitTypes(_LastLookedAtToken.value(), _LastExpressionType, MustBeType);
		return false;
	}

	EvaluatedEx ex1 = Eval_MakeEx(_LastExpressionType);
	if (Eval_Evaluate_t(ex1, node._Value.get(),GetValueMode::Read))
	{
		return Eval_EvaluateImplicitConversion(ex1, MustBeType, Out);
	}
	return false;
}
Optional<SystematicAnalysis::EvaluatedEx> SystematicAnalysis::Eval_Evaluate(const TypeSymbol& MustBeType, const ExpressionNodeType& node)
{
	EvaluatedEx Out;
	bool V = Eval_Evaluate(Out, MustBeType, node);

	if (V)
	{
		return { Out };
	}
	else
	{
		return {};
	}
}
bool SystematicAnalysis::Eval_EvaluateToAnyType(EvaluatedEx& Out, const ExpressionNodeType& node)
{
	OnExpressionTypeNode(node._Value.get(), GetValueMode::Read);//check
	

	EvaluatedEx ex1 = Eval_MakeEx(_LastExpressionType);
	bool CompilerRet=  Eval_Evaluate_t(ex1, node._Value.get(),GetValueMode::Read);
	Out = std::move(ex1);
	return CompilerRet;
}
bool SystematicAnalysis::Eval_EvalutateCMPTypesNode(EvaluatedEx& Out, const CMPTypesNode& node)
{
	TypeSymbol Op0 = Type_ConvertAndValidateType(node._TypeOp0, NodeSyb_t::Any);
	TypeSymbol Op1 = Type_ConvertAndValidateType(node._TypeOp1, NodeSyb_t::Any);

	Eval_Set_ObjectAs(Out, Type_Compare(Op0, Op1,NeverNullptr(node._Op)));
	return true;
}
bool SystematicAnalysis::Eval_EvalutateValidNode(EvaluatedEx& Out, const ValidNode& node)
{

	bool IsValid = true;
	auto ErrCount = _ErrorsOutput->Get_Errors().size();

	if (node._IsExpression)
	{
		OnExpressionTypeNode(node._ExpressionToCheck._Value.get(), GetValueMode::Read);
	}
	else
	{
		for (auto& Item : node._StatementToCheck._Nodes)
		{
			OnStatement(*Item);
		}
	}

	if (ErrCount < _ErrorsOutput->Get_Errors().size())
	{
		size_t CountToPop = _ErrorsOutput->Get_Errors().size() - ErrCount;
		for (size_t i = 0; i < CountToPop; i++)
		{
			_ErrorsOutput->Get_Errors().pop_back();
		}
		IsValid = false;
	}

	Eval_Set_ObjectAs(Out, IsValid);

	_LastExpressionType = TypesEnum::Bool;
	return true;
}
bool SystematicAnalysis::Eval_EvalutateFunc(EvaluatedEx& Out, const FuncCallNode& node)
{
	Get_FuncInfo FuncInfo = Type_GetFunc(node._FuncName,node.Parameters, Type_Get_LookingForType());

	if (Eval_CanEvalutateFuncCheck(FuncInfo))
	{
		Vector<EvaluatedEx> ValuePars;
		if (FuncInfo.Func) 
		{
			ValuePars.resize(FuncInfo.Func->Pars.size());

			for (size_t i = 0; i < node.Parameters._Nodes.size(); i++)
			{
				const TypeSymbol& Par = FuncInfo.Func->Pars[i].Type;
				auto& Item = node.Parameters._Nodes[i];

				auto Info = Eval_Evaluate(Par, *Item.get());

				if (!Info.has_value())
				{
					return false;
				}

				ValuePars.push_back(std::move(Info.value()));
			}
		}
		return Eval_EvalutateFunc(Out, FuncInfo, node._FuncName, ValuePars);
	}
	return false;
}

bool SystematicAnalysis::Eval_EvalutateFunc(EvaluatedEx& Out, const Get_FuncInfo& Func, const ScopedNameNode& Name, const Vector<EvaluatedEx>& ValuePars)
{
	
	if (Func.SymFunc)
	{
		FileDependency_AddDependencyToCurrentFile(Func.SymFunc);
	}
	Type_SetFuncRetAsLastEx(Func);


	if (Func._BuiltFunc.has_value())
	{
		auto& BuiltFunc = Func._BuiltFunc.value();
		if (BuiltFunc.EvalObject.has_value())
		{
			auto& EvalObject = BuiltFunc.EvalObject.value();
			Out.Type = _LastExpressionType;
			Out.EvaluatedObject = EvalObject;
			return true;
		}	
	}
	else if (Func.SymFunc)
	{
		const FuncNode* Funcnode = FuncNode::As(Func.SymFunc->Get_NodeInfo<Node>());

	
		EvalFuncData* State =new EvalFuncData();
		_Eval_FuncStackFrames.push_back(Unique_ptr<EvalFuncData>(State));
		constexpr size_t StackSize = 100;
		State->CallFrom = Name._ScopedName.back()._token;
		State->FuncSyb = Func.SymFunc;
		bool Ok = false;
		if (_Eval_FuncStackFrames.size() >= StackSize)
		{
			const NeverNullPtr<Token> token =NeverNullptr(State->CallFrom);
			String Str;
			Str += "Eval func Stack overflow.";
			Str += "--Call Stack \n";
			for (auto& Item : _Eval_FuncStackFrames)
			{
				Str += "On line " + std::to_string(Item->CallFrom->OnLine) + (String)" Called '" + Item->FuncSyb->FullName + "'. \n";
			}

			_ErrorsOutput->AddError(ErrorCodes::InValidType, token->OnLine, token->OnPos,Str);
			Ok = false;
		}
		else 
		{
			Ok = Eval_EvalutateFunc(*State, Func.SymFunc, ValuePars);
		}
		if (Ok)
		{
			Out.Type = Func.SymFunc->Get_Info<FuncInfo>()->Ret;
			Out.EvaluatedObject = State->Ret;
		}
		_Eval_FuncStackFrames.pop_back();
		return Ok;
	}

	return false;
}
bool SystematicAnalysis::Eval_EvalutateFunc(EvaluatedEx& Out, const TypeSymbol& Type, const Get_FuncInfo& Func, const Vector<EvaluatedEx>& ValuePars)
{
	String B = ToString(Type);
	Token T;
	T.Type = TokenType::Name;
	T.Value._String = B;


	ScopedNameNode Tep;
	ScopedName V;
	V._token = &T;

	Tep._ScopedName.push_back(std::move(V));

	return Eval_EvalutateFunc(Out,Func, Tep, ValuePars);
}
bool SystematicAnalysis::Eval_Evaluate(EvaluatedEx& Out, const ExtendedScopeExpression& node)
{
	Optional<EvaluatedEx> Ex = Eval_EvaluateToAnyType(node._Expression);
	
	if (Ex.has_value()) 
	{
		auto ExpressionType = _LastExpressionType;

		GetMemberTypeSymbolFromVar_t V;

		auto ExValue = Ex.value();

		_GetExpressionMode.push(_GetExpressionMode.top());
		{
			V.Type = ExpressionType;
			V._Symbol = Symbol_GetSymbol(ExpressionType).value_unchecked();


			if (Eval_EvalutateStepScopedName(ExValue, node._Extended, 0, node._Operator, V))
			{
				return false;
			}

			for (size_t i = 1; i < node._Extended._ScopedName.size(); i++)
			{
				if (Eval_EvalutateStepScopedName(ExValue, node._Extended, i, node._Extended._ScopedName[i]._operator, V))
				{
					return false;
				}
			}
		}
		_GetExpressionMode.pop();

		_LastExpressionType = V.Type;
		Out = std::move(ExValue);
		return true;
	}
	return false;
}
SystematicAnalysis::EvaluatedEx SystematicAnalysis::Eval_Evaluate_GetPointer()
{
	return {};
}

bool SystematicAnalysis::Eval_Evaluate(EvaluatedEx& Out, const ExtendedFuncExpression& node)
{
	Optional<EvaluatedEx> Ex = Eval_EvaluateToAnyType(node._Expression);

	auto ExpressionType = _LastExpressionType;

	_GetExpressionMode.push(_GetExpressionMode.top());
	{

		TypeSymbol TypeToStart = ExtendedFuncExpressionGetTypeToStart(ExpressionType, node);



		//Boring boiler plate for Tep ScopedNameNode
		const Token& ToGetLinesFrom = *node._Extended._FuncName._ScopedName.begin()->_token;

		ScopedNameNode Name;

		ScopedName TepV;


		Token TepToken;

		TepToken.OnLine = ToGetLinesFrom.OnLine;
		TepToken.OnPos = ToGetLinesFrom.OnPos;
		TepToken.Type = TokenType::Name;

		Type_RemoveTypeattributes(TypeToStart);

		String Buffer = ToString(TypeToStart);

		TepToken.Value._String = Buffer;

		TepV._token = &TepToken;


		TepV._operator = ScopedName::Operator_t::ScopeResolution;
		Name._ScopedName.push_back(std::move(TepV));

		{

			{
				auto Copy = node._Extended._FuncName._ScopedName.back();
				Name._ScopedName.push_back(std::move(Copy));
			}
			for (size_t i = 1; i < node._Extended._FuncName._ScopedName.size(); i++)
			{
				auto& Item = node._Extended._FuncName._ScopedName[i];
				auto Copy = Item;
				Name._ScopedName.push_back(std::move(Copy));
			}
		}
		//

		ValueParametersNode Pars;
		Pars._Nodes.push_back(Unique_ptr<Node>(node._Expression._Value.get()));

		for (size_t i = 0; i < node._Extended.Parameters._Nodes.size(); i++)
		{
			auto& Item = node._Extended.Parameters._Nodes[i];
			Pars._Nodes.push_back(Unique_ptr<Node>(Item.get()));
		}

		auto FuncInfo = Type_GetFunc(Name,Pars, Type_Get_LookingForType());

		
		Vector<EvaluatedEx> ValuePars;
		bool BadPars = false;
		
		if (Eval_CanEvalutateFuncCheck(FuncInfo)) 
		{
			ValuePars.resize(FuncInfo.Func->Pars.size());

			
			for (size_t i = 0; i < Pars._Nodes.size(); i++)
			{
				const TypeSymbol& Par = FuncInfo.Func->Pars[i].Type;
				auto& Item = Pars._Nodes[i];

				auto Info = Eval_Evaluate(Par, *Item.get());

				if (!Info.has_value())
				{
					BadPars = true;
				}

				ValuePars.push_back(std::move(Info.value()));
			}
		}

		for (auto& Item : Pars._Nodes)
		{
			auto Node = Item.release();//is ok it was borrwed.
		}

		if (BadPars)
		{
			return false;
		}

		return Eval_EvalutateFunc(Out, FuncInfo,node._Extended._FuncName, ValuePars);

	}
	_GetExpressionMode.pop();
	return false;
}
bool SystematicAnalysis::Eval_EvalutateFunc(EvalFuncData& State, const NeverNullPtr<Symbol> Func, const Vector<EvaluatedEx>& Pars)
{



	bool Fail = false;
	auto Conxet = SaveAndMove_SymbolContext();

	const FuncInfo* funcInfo = Func->Get_Info<FuncInfo>();
	Set_SymbolConext(funcInfo->Conext.value());
	{
		_Table.AddScope("__EvalFunc");
		
		const FuncNode& Body = *Func->Get_NodeInfo<FuncNode>();
		
		State.Pars.reserve(Pars.size());
		for (size_t i = 0; i < State.Pars.size(); i++)
		{
			auto ID = Symbol_GetSymbolID(Body._Signature._Parameters._Parameters[i]);
			State.Pars.AddValue(ID, Pars[i].EvaluatedObject);
		}
		State.Ret.ObjectSize = Type_GetSize(funcInfo->Ret).value();
		State.Ret.Object_AsPointer.reset(new Byte[State.Ret.ObjectSize]);
		State.FuncSyb = Func.value();
		//

		
		for (auto& Item : Body._Body.value()._Statements._Nodes)
		{
			auto Statement = Item.get();

			if (!Eval_EvalutateStatement(State, Statement))
			{
				Fail = true;
				break;
			}

		}

		_Table.RemoveScope();
	}

	Set_SymbolConext(std::move(Conxet));
	return !Fail;
}
bool SystematicAnalysis::Eval_EvalutateStatement(EvalFuncData& State, const Node* node)
{
	switch (node->Get_Type())
	{
	case NodeType::RetStatementNode:
	{
		const RetStatementNode* Node = RetStatementNode::As(node);
		auto Val = Eval_Evaluate(State.Get_funcInfo()->Ret, Node->_Expression);
		if (Val.has_value()) {
			State.Ret = std::move(Val.value().EvaluatedObject);
		}
		return Val.has_value();
	}
	default:
		return false;
		UCodeLangUnreachable();
		break;
	}
	
}
bool SystematicAnalysis::Eval_EvalutateScopedName(EvaluatedEx& Out, size_t Start, size_t End, const ScopedNameNode& node, GetMemberTypeSymbolFromVar_t& OtherOut)
{
	_GetExpressionMode.push(GetValueMode::Read);
	GetMemberTypeSymbolFromVar_t V;
	auto RetBool = Symbol_MemberTypeSymbolFromVar(Start,End,node, V);
	_GetExpressionMode.pop();

	_LastExpressionType = V.Type;

	if (V._Symbol)
	{
		if (V._Symbol->Type == SymbolType::ConstantExpression)
		{
			ConstantExpressionInfo* Info = V._Symbol->Get_Info<ConstantExpressionInfo>();
			Out.EvaluatedObject = Info->Ex;
			Out.Type = V.Type;
			return true;
		}
	}
	return false;
}
Optional<SystematicAnalysis::EvaluatedEx> SystematicAnalysis::Eval_EvaluateToAnyType(const ExpressionNodeType& node)
{
	EvaluatedEx Out;
	bool V = Eval_EvaluateToAnyType(Out, node);

	if (V)
	{
		return { Out };
	}
	else
	{
		return {};
	}
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
	default:
		break;
	}

	String CompilerRet = "{";
	for (size_t i = 0; i < Data.ObjectSize; i++)
	{
		char V = ((const char*)DataPtr)[i];
		CompilerRet += std::to_string((Byte)V);

		if (i != Data.ObjectSize-1)
		{
			CompilerRet += ",";
		}
	}
	CompilerRet += "}";

	return CompilerRet;
}

IRInstruction* SystematicAnalysis::IR_Load_UIntptr(UAddress Value)
{

	switch (_Settings->PtrSize)
	{
	case IntSizes::Int8:
		return _IR_LookingAtIRBlock->NewLoad((UInt8)Value);
	case IntSizes::Int16:
		return _IR_LookingAtIRBlock->NewLoad((UInt16)Value);
	case IntSizes::Int32:
		return _IR_LookingAtIRBlock->NewLoad((UInt32)Value);
	case IntSizes::Int64:
		return _IR_LookingAtIRBlock->NewLoad((UInt64)Value);
	default:
		UCodeLangUnreachable();
		break;
	}
}
IRInstruction* SystematicAnalysis::IR_Load_SIntptr(SIntNative Value)
{
	return IR_Load_UIntptr(*(UAddress*)&Value);
}

IRInstruction* SystematicAnalysis::IR_Build_Add_uIntPtr(IRInstruction* field, IRInstruction* field2)
{
	return _IR_LookingAtIRBlock->NewAdd(field, field2);
}

IRInstruction* SystematicAnalysis::IR_Build_Sub_uIntPtr(IRInstruction* field, IRInstruction* field2)
{
	return _IR_LookingAtIRBlock->NewSub(field, field2);
}

IRInstruction* SystematicAnalysis::IR_Build_Add_sIntPtr(IRInstruction* field, IRInstruction* field2)
{
	return _IR_LookingAtIRBlock->NewAdd(field, field2);
}

IRInstruction* SystematicAnalysis::IR_Build_Sub_sIntPtr(IRInstruction* field, IRInstruction* field2)
{
	return _IR_LookingAtIRBlock->NewSub(field, field2);
}

IRInstruction* SystematicAnalysis::IR_Build_Mult_uIntPtr(IRInstruction* field, IRInstruction* field2)
{
	return _IR_LookingAtIRBlock->NewUMul(field, field2);
}

IRInstruction* SystematicAnalysis::IR_Build_Mult_sIntPtr(IRInstruction* field, IRInstruction* field2)
{
	return _IR_LookingAtIRBlock->NewSMul(field, field2);
}

IRInstruction* SystematicAnalysis::Build_Div_uIntPtr(IRInstruction* field, IRInstruction* field2)
{
	return _IR_LookingAtIRBlock->NewUDiv(field, field2);
}

IRInstruction* SystematicAnalysis::IR_Build_Div_sIntPtr(IRInstruction* field, IRInstruction* field2)
{
	return _IR_LookingAtIRBlock->NewSDiv(field, field2);
}

void SystematicAnalysis::IR_Build_Increment_uIntPtr(IRInstruction* field, UAddress Value)
{
	switch (_Settings->PtrSize)
	{
	case IntSizes::Int32:
		_IR_LookingAtIRBlock->New_Increment(field,_IR_LookingAtIRBlock->NewLoad((UInt32)Value));
		break;
	case IntSizes::Int64:
		_IR_LookingAtIRBlock->New_Increment(field,_IR_LookingAtIRBlock->NewLoad((UInt64)Value));
		break;
	default:
		UCodeLangUnreachable();
		break;
	}
}

void SystematicAnalysis::IR_Build_Decrement_uIntPtr(IRInstruction* field, UAddress Value)
{
	switch (_Settings->PtrSize)
	{
	case IntSizes::Int32:
		_IR_LookingAtIRBlock->New_Decrement(field, _IR_LookingAtIRBlock->NewLoad((UInt32)Value));
		break;
	case IntSizes::Int64:
		_IR_LookingAtIRBlock->New_Decrement(field, _IR_LookingAtIRBlock->NewLoad((UInt64)Value));
		break;
	default:
		UCodeLangUnreachable();
		break;
	}
}

void SystematicAnalysis::IR_Build_Increment_sIntPtr(IRInstruction* field, SIntNative Value)
{
	return IR_Build_Increment_uIntPtr(field, *(UAddress*)&Value);
}
IRInstruction* SystematicAnalysis::LoadEvaluatedEx(const RawEvaluatedObject& Value, const TypeSymbol& ValueType)
{
	void* ObjectData = Eval_Get_Object(ValueType, Value);
	switch (ValueType._Type)
	{
	case TypesEnum::Bool:
	case TypesEnum::Char:
	case TypesEnum::sInt8:
	case TypesEnum::uInt8:
		return _IR_LookingAtIRBlock->NewLoad(*(UInt8*)ObjectData);
		break;
	case TypesEnum::sInt16:
	case TypesEnum::uInt16:
		return _IR_LookingAtIRBlock->NewLoad(*(UInt16*)ObjectData);
		break;
	case TypesEnum::sInt32:
	case TypesEnum::uInt32:
		return _IR_LookingAtIRBlock->NewLoad(*(UInt32*)ObjectData);
		break;
	case TypesEnum::float32:
		return _IR_LookingAtIRBlock->NewLoad(*(float32*)ObjectData);
		break;
	case TypesEnum::float64:
		return _IR_LookingAtIRBlock->NewLoad(*(float64*)ObjectData);
		break;
	case TypesEnum::sInt64:
	case TypesEnum::uInt64:
		return _IR_LookingAtIRBlock->NewLoad(*(UInt64*)ObjectData);
		break;

	case TypesEnum::sIntPtr:
	case TypesEnum::uIntPtr:
		if (_Settings->PtrSize == IntSizes::Int64)
		{
			return _IR_LookingAtIRBlock->NewLoad(*(UInt64*)ObjectData);
		}
		else
		{
			return _IR_LookingAtIRBlock->NewLoad(*(UInt32*)ObjectData);
		}
	case TypesEnum::CustomType:
	{
		auto Syb = Symbol_GetSymbol(ValueType._CustomTypeSymbol);


		if (Syb->Type == SymbolType::Type_StaticArray)
		{
			StaticArrayInfo* Info = Syb->Get_Info<StaticArrayInfo>();
			auto R = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(ValueType));
			auto Base = Info->Type;
			auto BaseSize = Type_GetSize(Base).value();

			auto Ptr = _IR_LookingAtIRBlock->NewLoadPtr(R);

			RawEvaluatedObject _DataAsIndex;
			_DataAsIndex.ObjectSize = BaseSize;
			_DataAsIndex.Object_AsPointer.reset(new Byte[BaseSize]);
			auto BaseAsIR = _IR_LookingAtIRBlock->NewLoad(BaseSize);
			for (size_t i = 0; i < Info->Count; i++)
			{
				void* ItemOffset = Value.Object_AsPointer.get() + (BaseSize * i);
				Eval_Set_ObjectAs(Base, _DataAsIndex, ItemOffset, BaseSize);

				auto ItemIR = LoadEvaluatedEx(_DataAsIndex, Base);

				_IR_LookingAtIRBlock->New_Index_Vetor(Ptr, _IR_LookingAtIRBlock->NewLoad(i), BaseAsIR);
			}

			return R;
		}
		else
		{
			UCodeLangUnreachable();
		}

	}
	default:
		UCodeLangUnreachable();
		break;
	}
}

void SystematicAnalysis::IR_Build_Decrement_sIntPtr(IRInstruction* field, SIntNative Value)
{
	return IR_Build_Decrement_uIntPtr(field, *(UAddress*)&Value);
}

void SystematicAnalysis::TryLogError_OnWritingVar(NeverNullPtr<Symbol> Symbol, const NeverNullPtr<Token> Token,const String_view Name)
{
	if (Symbol->VarType.Isimmutable())
	{
		LogError_CantModifyiMutableError(Token, Name);
	}
}

String SystematicAnalysis::IR_MangleName(const FuncInfo* Func)
{
	Vector<ClassMethod::Par> Vect;
	for (auto& Item : Func->Pars)
	{
		ClassMethod::Par V;
		V.Type = Assembly_ConvertToType(Item.Type);
		V.IsOutPar = Item.IsOutPar;
		Vect.push_back(V);
	}

	return NameDecoratior::GetDecoratedName(Func->FullName, Vect);
}

IRidentifierID SystematicAnalysis::IR_GetIRID(const FuncInfo* Func)
{
	auto FuncName = IR_MangleName(Func);
	return _IR_Builder.ToID(FuncName);
}

GenericData::Type SystematicAnalysis::Generic_TypeToGenericDataType(GenericValueNode::GenericType type)
{
	switch (type)
	{
	case GenericValueNode::GenericType::Name:
		return GenericData::Type::Name;
		break;
	case GenericValueNode::GenericType::ConstantExpression:
		return GenericData::Type::ConstantExpression;
		break;
	case GenericValueNode::GenericType::Pack:
		return GenericData::Type::Pack;
		break;
	default:
		UCodeLangUnreachable();
		break;
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

void SystematicAnalysis::LogError_CantCastImplicitTypes(const NeverNullPtr<Token> Token, const TypeSymbol& Ex1Type, const TypeSymbol& UintptrType, bool ReassignMode)
{
	if (Ex1Type.IsBadType() || UintptrType.IsBadType()
		|| Type_IsUnMapType(UintptrType) || Type_IsUnMapType(Ex1Type)) { return; }

	bool V1 = Type_IsAddessAndLValuesRulesfollowed(Ex1Type, UintptrType, ReassignMode);
	if (!V1 || Type_CanDoTypeToTrait(Ex1Type, UintptrType))
	{
		LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
			, "The expression is not an Location in memory'");
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
		, "the variable named '" + (String)Str + "'" + " cant be read from you.can not read an invaid variable");
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
		, "Cant find Member Named '" + (String)Str + "' on type '" + ToString(OnType) +"'");
}

void SystematicAnalysis::LogError_CantFindCompoundOpForTypes(const NeverNullPtr<Token> BinaryOp, TypeSymbol& Ex0Type, TypeSymbol& Ex1Type)
{
	if (Ex1Type.IsBadType() || Ex0Type.IsBadType() 
		|| Type_IsUnMapType(Ex0Type) || Type_IsUnMapType(Ex1Type)) { return; }

	LogError(ErrorCodes::InValidType, BinaryOp->OnLine, BinaryOp->OnPos,
		"The type '" + ToString(Ex0Type) + "'" + " cant be '"
		+ ToString(BinaryOp->Type) + "' with '" + ToString(Ex0Type) + "'");
}

void SystematicAnalysis::LogError_CantFindPostfixOpForTypes(const NeverNullPtr<Token> BinaryOp, TypeSymbol& Ex0Type)
{
	if (Ex0Type.IsBadType() || Type_IsUnMapType(Ex0Type)) { return; }

		LogError(ErrorCodes::InValidType, BinaryOp->OnLine, BinaryOp->OnPos,
			"The type '" + ToString(Ex0Type) + "'" + " cant be '"
			+ ToString(BinaryOp->Type) + "'");
}
void SystematicAnalysis::LogError_CantFindBinaryOpForTypes(const NeverNullPtr<Token> BinaryOp, TypeSymbol& Ex0Type, TypeSymbol& Ex1Type)
{
	if (Ex1Type.IsBadType() || Ex0Type.IsBadType()
		|| Type_IsUnMapType(Ex1Type) || Type_IsUnMapType(Ex0Type)) { return; }

	LogError(ErrorCodes::InValidType, BinaryOp->OnLine, BinaryOp->OnPos,
		"The type '" + ToString(Ex0Type) + "'" + " cant be '"
		+ ToString(BinaryOp->Type) + "' with '" + ToString(Ex1Type) + "'");
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
		, "cant guess 'var' type");
}
void SystematicAnalysis::LogError_CantUseThisKeyWordHereError(const NeverNullPtr<Token> NameToken)
{
	LogError(ErrorCodes::InValidType, NameToken->OnLine, NameToken->OnPos, "cant use this here");
}
void SystematicAnalysis::LogError_CantgussTypesTheresnoassignment(const NeverNullPtr<Token> Token)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos
		, "cant guess type theres no '=' [expression]");
}
void SystematicAnalysis::LogError_CantCastExplicityTypes(const NeverNullPtr<Token> Token, TypeSymbol& Ex0Type, TypeSymbol& ToTypeAs)
{
	if (Ex0Type.IsBadType() || ToTypeAs.IsBadType()
		|| Type_IsUnMapType(Ex0Type) || Type_IsUnMapType(ToTypeAs)){return;}

	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Cant Explicity cast Type '" + ToString(Ex0Type) + "' to '" + ToString(ToTypeAs) + "'");
}

SystematicAnalysis::ReadVarErrorCheck_t SystematicAnalysis::TryLogError_OnReadVar(String_view VarName, const NeverNullPtr<Token> Token,const NullablePtr<Symbol> Syb)
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
void SystematicAnalysis::LogError_CantFindPostfixOpForTypes_Constant(const NeverNullPtr<Token> BinaryOp, TypeSymbol& Ex0Type)
{
	LogError_CantFindPostfixOpForTypes(BinaryOp, Ex0Type);
}
void SystematicAnalysis::LogError_Eval_CantDoPostfixOpForTypes(const NeverNullPtr<Token> BinaryOp, TypeSymbol& Ex0Type)
{
	LogError(ErrorCodes::InValidName, BinaryOp->OnLine, BinaryOp->OnPos
		, "The Type operation '" + ToString(Ex0Type) + "' must be an compile time constant.");
}
void SystematicAnalysis::LogError_Eval_CantCastImplicitTypes(const NeverNullPtr<Token> Token,const TypeSymbol& Ex1Type, const TypeSymbol& UintptrType)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Casting Type '" + ToString(Ex1Type) + " to '" + ToString(UintptrType) + "' cant be done at compile time.");
}
void SystematicAnalysis::LogError_CantFindNamespace(const NeverNullPtr<Token> Token, const String_view Namespace)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos
		, "the cant find the Namespace '" + (String)Namespace + "'.");
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

	for (size_t i = _ClassDependencies.size() - 1; i != (Vector<const ClassInfo*>::size_type) - 1;i--)
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
		,"Cant Find Type for '" + (String)TokenStringHelper::ToString(TokenType::KeyWord_This) + "'");
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
		,"The function named " + (String)FuncName + " must return to the type of '" + ToString(TypeSybToBe) + '\'');
}
void SystematicAnalysis::LogError_SymbolRedefinition(const NeverNullPtr<Token> Token, const NeverNullPtr<Symbol> Symbol)
{

	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "the symbol '"+  Symbol->FullName + "\' is already define. try useing a different name");
}
void SystematicAnalysis::LogError_UseingVarableBeforDeclared(const NeverNullPtr<Token> Token)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "trying to use the varable '" + (String)Token->Value._String + "\' before its defined.");
}
void SystematicAnalysis::LogError_BeMoreSpecifiicForRetType(const String_view FuncName, const NeverNullPtr<Token> Token)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "be more Specifiic For return Type. like |" + (String)FuncName + "[...] -> [Type]; or give the funcion a body.");
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
	case SymbolType::StackVarable:return "StackVarable";
	case SymbolType::ParameterVarable:return "ParameterVarable";
	case SymbolType::Type:return "Type";
	case SymbolType::Type_alias:return "Type_alias";
	case SymbolType::Hard_Type_alias:return "Hard_Type_alias";
	case SymbolType::Type_class:return "Type_class";
	case SymbolType::Class_Field:return "Class_Field";
	case SymbolType::Enum:return "Enum";
	case SymbolType::Func:return "Func";
	case SymbolType::ImportedDllFunc:return "ImportedDllFunc";
	case SymbolType::ImportedLibFunc:return "ImportedLibFunc";
	case SymbolType::FuncCall:return "FuncCall";
	case SymbolType::GenericFunc:return "GenericFunc";
	case SymbolType::Generic_class:return"Generic_class";
	case SymbolType::Unmaped_Generic_Type:return "Unmaped_Generic_Type";
	case SymbolType::Unmaped_Varable:return "Unmaped_Varable";
	case SymbolType::Namespace:return "Namespace";
	case SymbolType::Hard_Func_ptr:return "Hard_Func_ptr";
	case SymbolType::Func_ptr:return "Func_ptr";
	case SymbolType::ConstantExpression:return "ConstantExpression";
	case SymbolType::Generic_Alias:return "Generic_Alias";
	case SymbolType::Generic_Tag:return "Generic_Tag";
	case SymbolType::Generic_Enum:return "Generic_Enum";
	default:return "[n/a]";
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
void SystematicAnalysis::LogError_LogWantedAVariable(const NeverNullPtr<Token>& Item,Symbol* TepSyb)
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
void SystematicAnalysis::LogError_MustMakeEnumLikeafuncion(EnumInfo* Einfo, size_t Index, const NeverNullPtr<Token> Token)
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
void SystematicAnalysis::LogError_DynamicMustBeRrait(const TypeNode& V,const TypeSymbol& Out)
{
	LogError(ErrorCodes::InValidType, V._name.token->OnLine, V._name.token->OnPos, "useing a Dynamic type on a none trait the type found '" + ToString(Out) + "'");
}
void SystematicAnalysis::LogError_TraitCantBeAlone(const NeverNullPtr<Token> Token)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "the Type uses a Trait.but Traits cant be use alone.");
}

void SystematicAnalysis::LogError_WantedAType(const TypeNode& V, Symbol* SybV)
{
	LogError(ErrorCodes::InValidType, V._name.token->OnLine, V._name.token->OnPos,
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
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "Parameter  named '" + (String)Token->Value._String + "' is useing a Parameter pact.But Parameter pact must be last Paramter");
}
void SystematicAnalysis::LogError_ParPackTypeIsNotLast(const NeverNullPtr<Token> Token)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "Type Pack named '" + (String)Token->Value._String + "' is not declarded last.");
}
void SystematicAnalysis::LogError_UseingTypeinfoInNonEvalVarable(const NeverNullPtr<Token> Token)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos, "Trying to use typeinfo in a Non-eval Varable");
}
void SystematicAnalysis::LogError_UseingTypeinfoInEvalFuncPar(const NeverNullPtr<Token> Token)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos, "Trying to use typeinfo in a Non-eval Func");
}
void SystematicAnalysis::LogError_CantOutputTypeinfo(const NeverNullPtr<Token> Token)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "Cant Output IR of an typeinfo. place this in an eval funcion or an eval varable");
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
void SystematicAnalysis::LogError(ErrorCodes Err,const String& MSG, const NeverNullPtr<Token> Token)
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

	
	_ErrorsOutput->AddError(Err, Line, Pos,Str);
}

UCodeLangFrontEnd


