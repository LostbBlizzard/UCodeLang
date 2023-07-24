#include "SystematicAnalysis.hpp"
#include "UCodeLang/Compliation/Helpers/KeyWords.hpp"

#include "UCodeLang/Compliation/Helpers/InstructionBuilder.hpp"
#include "UCodeLang/Compliation/Back/UCodeBackEnd/UCodeBackEnd.hpp"
#include "UCodeLang/Compliation/Helpers/ParseHelper.hpp"
#include "UCodeLang/Compliation/Helpers/NameDecoratior.hpp"
#include "UCodeLang/LangCore/DataType/Defer.hpp"
UCodeLangFrontStart

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
			if (This.LookingForTypes.size())
			{
				auto Type = This.LookingForTypes.top();
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
				_Func.RetType = This.GetStaticArrayType(TypesEnum::Char, Value.size());
				auto Ex = This.MakeEx(_Func.RetType);
				This.Set_ObjectAs(Ex, Value.data(), Value.size());


				_Func.EvalObject = std::move(Ex.EvaluatedObject);
				_Func.EvalAsCString = WantsUmutCString;
				return _Func;
			}
			else if (FuncName == "Name" && Type.Type._TypeInfo == TypeInfoPrimitive::ClassFieldInfo && Type.ExpressionNode)
			{
				auto EvalObject = This.EvaluateToAnyType(*ExpressionNodeType::As(Type.ExpressionNode));
				if (EvalObject.has_value())
				{
					auto& EvalObjectAsValue = EvalObject.value();
					const ClassField& Field = *This.Get_ObjectAs<const ClassField>(EvalObjectAsValue);

					const String& Value = Field.Field->Name;

					Func _Func;
					_Func.RetType = This.GetStaticArrayType(TypesEnum::Char, Value.size());
					auto Ex = This.MakeEx(_Func.RetType);
					This.Set_ObjectAs(Ex,Value.data(), Value.size());

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
				auto EvalObject = This.EvaluateToAnyType(*ExpressionNodeType::As(Type.ExpressionNode));
				if (EvalObject.has_value())
				{
					auto& EvalObjectAsValue = EvalObject.value();
					const EnumField& Field = *This.Get_ObjectAs<const EnumField>(EvalObjectAsValue);

					const String& Value = Field.Field->Name;

					Func _Func;
					_Func.RetType = This.GetStaticArrayType(TypesEnum::Char, Value.size());
					auto Ex = This.MakeEx(_Func.RetType);
					This.Set_ObjectAs(Ex, Value.data(), Value.size());

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
				auto EvalObject = This.EvaluateToAnyType(*ExpressionNodeType::As(Type.ExpressionNode));
				if (EvalObject.has_value())
				{
					auto& EvalObjectAsValue = EvalObject.value();
					const EnumField& Field = *This.Get_ObjectAs<const EnumField>(EvalObjectAsValue);

					const auto& Value = Field.Field->Ex;

					Func _Func;
					_Func.RetType = Field._EnumInfo->Basetype;
					auto Ex = This.MakeEx(_Func.RetType);
					This.Set_ObjectAs(Ex, Value.Object_AsPointer.get(),Value.ObjectSize);

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
				auto EvalObject = This.EvaluateToAnyType(*ExpressionNodeType::As(Type.ExpressionNode));
				if (EvalObject.has_value())
				{
					auto& EvalObjectAsValue = EvalObject.value();
					const ClassField& Field = *This.Get_ObjectAs<const ClassField>(EvalObjectAsValue);

					auto OffsetInfo = This.GetOffset(*Field._ClassInfo, Field.Field);

					const size_t& Value = OffsetInfo.value();

					Func _Func;
					_Func.RetType = TypesEnum::uIntPtr;
					auto Ex = This.MakeEx(_Func.RetType);
					This.Set_ObjectAs(Ex, Value);

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
				auto EvalObject = This.EvaluateToAnyType(*ExpressionNodeType::As(Type.ExpressionNode));
				if (EvalObject.has_value())
				{
					auto& EvalObjectAsValue = EvalObject.value();
					const ClassField& Field = *This.Get_ObjectAs<ClassField>(EvalObjectAsValue);

					const TypeSymbol& Value = Field.Field->Type;

					Func _Func;
					_Func.RetType = Value;
					_Func.RetType.SetAsTypeInfo();
					auto Ex = This.MakeEx(_Func.RetType);
					This.Set_ObjectAs(Ex, Value);

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

				auto Value = This.GetTypeID(NewType._Type, NewType._CustomTypeSymbol);

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
				auto Ex = This.MakeEx(_Func.RetType);
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
				auto Ex = This.MakeEx(_Func.RetType);
				This.Set_ObjectAs(Ex, Value.value());

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
					const auto Sym = This.GetSymbol(Par.Type);
					const auto classInfo = Sym->Get_Info<EnumInfo>();
					auto BaseType = classInfo->Basetype;

					Func _Func;
					_Func.RetType = BaseType;
					_Func.RetType.SetAsTypeInfo();
					auto Ex = This.MakeEx(_Func.RetType);
					This.Set_ObjectAs(Ex, BaseType);

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
					const auto Sym = This.GetSymbol(Par.Type);
					const auto classInfo = Sym->Get_Info<EnumInfo>();
					bool IsVariant = classInfo->VariantData.has_value();


					Func _Func;
					_Func.RetType = TypesEnum::Bool;
					_Func.RetType.SetAsTypeInfo();
					auto Ex = This.MakeEx(_Func.RetType);
					This.Set_ObjectAs(Ex, IsVariant);

					_Func.EvalObject = std::move(Ex.EvaluatedObject);

					{	
						Func::OutParData Par;
						if (IsVariant)
						{
							const auto& Variant = classInfo->VariantData.value();
							

							Par.Type = TypesEnum::InternalType;
							Par.Type._TypeInfo = TypeInfoPrimitive::EnumVariantInfo;

							auto Ex = This.MakeEx(Par.Type);
							This.Set_ObjectAs(Ex,&Variant);

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
				const auto Sym = This.GetSymbol(Type.Type);
				bool IsClass = Sym ? Sym->Type == SymbolType::Type_class : nullptr;

				Func _Func;
				_Func.RetType = TypesEnum::Bool;
				auto Ex = This.MakeEx(_Func.RetType);
				This.Set_ObjectAs(Ex, IsClass);

				_Func.EvalObject = std::move(Ex.EvaluatedObject);

				{
					Func::OutParData Par;
					if (IsClass)
					{
						Par.Type = Type.Type;
						Par.Type._IsAddress = false;//If Pass as Ref
						Par.Type._TypeInfo = TypeInfoPrimitive::ClassInfo;

						auto Ex = This.MakeEx(Par.Type);
						This.Set_ObjectAs(Ex, Par.Type);

						Par.EvalObject = std::move(Ex.EvaluatedObject);
					}
					_Func._OutPars.push_back(std::move(Par));
				}
				return _Func;
			}
			else if (FuncName == "GetEnumInfo")
			{
				const auto Sym = This.GetSymbol(Type.Type);
				bool IsEnum = Sym ? Sym->Type == SymbolType::Enum: nullptr;

				Func _Func;
				_Func.RetType = TypesEnum::Bool;
				auto Ex = This.MakeEx(_Func.RetType);
				This.Set_ObjectAs(Ex, IsEnum);

				_Func.EvalObject = std::move(Ex.EvaluatedObject);

				{
					Func::OutParData Par;
					if (IsEnum) 
					{
						Par.Type = Type.Type;
						Par.Type._IsAddress = false;//If Pass as Ref
						Par.Type._TypeInfo = TypeInfoPrimitive::EnumInfo;

						auto Ex = This.MakeEx(Par.Type);
						This.Set_ObjectAs(Ex, Par.Type);

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
			const auto Sym = This.GetSymbol(Type.Type);
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
			_Func.RetType = This.GetStaticArrayType(ArrItemType, FieldsAs.size());

			auto Ex = This.MakeEx(_Func.RetType);
			This.Set_ObjectAs(Ex, FieldsAs.data(), FieldsAs.size() * sizeof(ClassField));
			_Func.EvalObject = std::move(Ex.EvaluatedObject);

			return _Func;
		}
		break;
		case TypeInfoPrimitive::EnumInfo:
		{
			const auto Sym = This.GetSymbol(Type.Type);
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
			_Func.RetType = This.GetStaticArrayType(ArrItemType, FieldsAs.size());

			auto Ex = This.MakeEx(_Func.RetType);
			This.Set_ObjectAs(Ex, FieldsAs.data(), FieldsAs.size() * sizeof(EnumField));
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

	int V = 0;
	int* Ptr = &V;
	GetSymbolID(Ptr);

	{
		for (const auto& File : *_Files)
		{
			_FilesData.AddValue(File,Unique_ptr<FileNodeData>(new FileNodeData()));
		}

	}
	{
		_ForceImportArgWasPassed = _Settings->HasFlagArg("ForceImport");
		_RemoveUnSafeArgWasPassed = _Settings->HasFlagArg("RemoveUnsafe");
		_ImmutabilityIsForced = _Settings->HasFlagArg("ForcedImmutability");

		StartingNameSpace = _Settings->GetArgValueFlag("StartingNameSpac");
	}

	passtype = PassType::GetTypes;
	Pass();

	if (!_ErrorsOutput->Has_Errors()) 
	{
		

		LoadLibSymbols();
		passtype = PassType::FixedTypes;
		Pass();


		if (!_ErrorsOutput->Has_Errors()) {
			BuildLibs();
			if (!_ErrorsOutput->Has_Errors()) {
				BuildCode();
			}
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
		if (!Item->OutputIR){continue;}

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
void SystematicAnalysis::BuildLibs()
{
	if (_Settings->_Type != OutPutType::IRAndSymbols) 
	{
		for (size_t i = 0; i < _Libs->size(); i++)
		{
			auto Item = (*_Libs)[i];

			BuildLib(*Item,(*_LibsNames)[i]);
		}
	}
}
void SystematicAnalysis::BuildLib(const UClib& lib,const Path& LibName)
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
			_Builder.CombineWith(std::move(IRToImport));
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
		auto& Item =_Builder._StaticInit.Blocks.front()->Instructions[Index];
		
		TepIR._StaticInit.Blocks.front()->Instructions.push_back(Unique_ptr<IRInstruction>(Item.get()));
	}
	for (size_t i = 0; i < FileData.IRInitThreadSpan.Count; i++)
	{
		size_t Index = FileData.IRInitThreadSpan.Index + i;
		auto& Item = _Builder._threadInit.Blocks.front()->Instructions[Index];
		
		TepIR._threadInit.Blocks.front()->Instructions.push_back(Unique_ptr<IRInstruction>(Item.get()));
	}
	for (size_t i = 0; i < FileData.IRDeInitStaticSpan.Count; i++)
	{
		size_t Index = FileData.IRDeInitStaticSpan.Index + i;
		auto& Item = _Builder._StaticdeInit.Blocks.front()->Instructions[Index];

		TepIR._StaticdeInit.Blocks.front()->Instructions.push_back(Unique_ptr<IRInstruction>(Item.get()));
	}
	for (size_t i = 0; i < FileData.IRDeInitThreadSpan.Count; i++)
	{
		size_t Index = FileData.IRDeInitThreadSpan.Index + i;
		auto& Item = _Builder._threaddeInit.Blocks.front()->Instructions[Index];

		TepIR._threaddeInit.Blocks.front()->Instructions.push_back(Unique_ptr<IRInstruction>(Item.get()));
	}

	for (size_t i = 0; i < FileData.IRFuncsSpan.Count; i++)
	{
		size_t Index = FileData.IRFuncsSpan.Index + i;
		auto& Item = _Builder.Funcs[Index];

		TepIR.Funcs.push_back(Unique_ptr<IRFunc>(Item.get()));
	}

	for (size_t i = 0; i < FileData.IRSymbolSpan.Count; i++)
	{
		size_t Index = FileData.IRSymbolSpan.Index + i;
		auto& Item = _Builder._Symbols[Index];

		TepIR._Symbols.push_back(Unique_ptr<IRSymbolData>(Item.get()));
	}


	TepIR.ConstStaticStrings = _Builder.ConstStaticStrings;


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
void SystematicAnalysis::Add_SetLineNumber(const Token* token, size_t InsInBlock)
{
	auto ThisFileName = LookingAtFile->FileName.generic_string();
	auto LineNumber = token->OnLine;
	if (LastLineNumber != LineNumber || (ThisFileName != _LastIRFileName) || (LastLookAtDebugBlock != &LookingAtIRBlock->DebugInfo))
	{
		if (LastLookAtDebugBlock != &LookingAtIRBlock->DebugInfo)
		{
			_LastIRFileName = "";
			LastLineNumber = -1;

			LastLookAtDebugBlock = &LookingAtIRBlock->DebugInfo;
		}

		if (ThisFileName != _LastIRFileName)
		{
			_LastIRFileName = ThisFileName;
			LookingAtIRBlock->DebugInfo.Add_SetFile(ThisFileName, InsInBlock);
		}
		LastLineNumber = LineNumber;

		LookingAtIRBlock->DebugInfo.Add_SetLineNumber(token->OnLine, InsInBlock);
	}
}
void SystematicAnalysis::Add_SetVarableInfo(const Symbol& Syb, IRInstruction* Ins)
{
	auto ID = _Builder.ToID(Syb.FullName);
	IRDebugSetVarableName V;
	
	V.VarableName = Syb.FullName;
	
	LookingAtIRBlock->DebugInfo.Add_SetVarableName(std::move(V));

	IRDebugSybol Info;
	Info.VarableName = Syb.FullName;
	
	UCodeLang::ReflectionTypeInfo Type =ConvertToTypeInfo(Syb.VarType);

	Info.LangType = UCode_LangType_UCodeLang;

	BitMaker bitm; 
	UClib::ToBytes(bitm, Type);
	Info.TypeInfo = std::move(bitm.Get_Bytes());

	switch (Syb.Type)
	{	
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
		throw std::exception("bad path");
		break;
	}
	_Builder._Debug.Symbols.AddValue(ID, Info);
}
const FileNode* SystematicAnalysis::Get_FileUseingSybol(const Symbol* Syb)
{
	return Syb->_File;
}
void SystematicAnalysis::AddDependencyToCurrentFile(const Symbol* Syb)
{
	AddDependencyToCurrentFile(Get_FileUseingSybol(Syb));
	
	
	auto CurrentFile = LookingAtFile;
	auto& FileData = GetFileData(CurrentFile);
	auto LookingAtSyb = Syb;
	bool IsAnImport = false;
	while (LookingAtSyb)
	{
	
		

		const String* StringToLookAt = nullptr;
		if (LookingAtSyb->Type == SymbolType::Func)
		{
			StringToLookAt = &LookingAtSyb->Get_Info<FuncInfo>()->FullName;//because of how Import alias works with Funcs
		}
		else
		{
			StringToLookAt = &LookingAtSyb->FullName;

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

		if (LookingAtSyb->Type == SymbolType::Type_alias
			|| LookingAtSyb->Type == SymbolType::Hard_Type_alias)
		{
			LookingAtSyb = GetSymbol(LookingAtSyb->VarType);
		}
		else
		{
			LookingAtSyb = nullptr;
		}
	}

	if (CurrentFile != Syb->_File && !IsAnImport && (FileData._Imports.size() || _ForceImportArgWasPassed))
	{
		auto Token = LastLookedAtToken;
		LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "Trying to use the Symbol '" + Syb->FullName + "[" + ToString(Syb->Type) + "]' but it's not Imported in the file.");
	}
}
void SystematicAnalysis::AddDependencyToCurrentFile(const FileNode* file)
{
	auto CurrentFile = LookingAtFile;
	const FileNode_t* CurrentFile_t =(const FileNode_t*)CurrentFile;

	const FileNode_t* file_t = (const FileNode_t*)file;
	if (CurrentFile != file)
	{
		FileNodeData& Data = *_FilesData[CurrentFile_t];
		
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
Symbol& SystematicAnalysis::AddSybol(SymbolType type, const String& Name, const String& FullName,AccessModifierType Access)
{

	auto& r = _Table.AddSybol(type, Name, FullName,Access);
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
			AddSybol(SymbolType::Namespace, Namespace, _Table._Scope.ThisScope,AccessModifierType::Public);
		}
	}

	
	for (auto& node : node._Nodes)
	{
		PushToNodeScope(*node.get());
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
		PopNodeScope();
	}
	_Table.RemoveScope();

	_Table.RemovePopUseing(UseingIndex);
}
void SystematicAnalysis::OnAttributeNode(const AttributeNode& node)
{


	if (passtype == PassType::GetTypes)
	{

	}
	else if (passtype == PassType::FixedTypes)
	{	
		auto SybID = GetSymbolID(node);
		auto& Syb = AddSybol(SymbolType::UsedTag,"_","_",AccessModifierType::Private);
		_Table.AddSymbolID(Syb, SybID);
		

		String V;
		node.ScopedName.GetScopedName(V);
		auto Att = GetSymbol(V,SymbolType::Tag_class);
		if (Att)
		{
			auto Token = node.ScopedName.ScopedName.back().token;
			if (Att->Type == SymbolType::Tag_class)
			{
				auto& Generic = *node.ScopedName.ScopedName.back().Generic;
				if (Generic.Values.size())
				{
					LogExpectedSymbolToBea(Token, *Att, SymbolType::Generic_Tag);
				}
				
			}
			else if (Att->Type == SymbolType::Generic_Tag)
			{
				auto& Generic = *node.ScopedName.ScopedName.back().Generic;
				if (Generic.Values.size())
				{
					auto Info = Att->Get_Info<TagInfo>();
					auto TagNode =TagTypeNode::As(Att->Get_NodeInfo<Node>());

					Att = InstantiateOrFindGeneric_Tag(Token, Att,TagNode->Generic,Info->_GenericData,Generic);
				}
				else
				{
					LogExpectedSymbolToBea(Token, *Att, SymbolType::Tag_class);
				}
			}
			else
			{
				LogExpectedSymbolToBea(Token, *Att, SymbolType::Tag_class);
			}
			Syb.VarType = TypeSymbol(Att->ID);
		}
		else
		{
			LogCantFindTypeError(node.ScopedName.ScopedName.back().token, V);
		}
	}
	else if (passtype == PassType::BuidCode)
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
void SystematicAnalysis::OnFileNode(const FileNode* File)
{
	LookingAtFile = File;
	_ErrorsOutput->FilePath = File->FileName;
	
	size_t ClassesStart = PassType::BuidCode == passtype ? _Lib._Assembly.Classes.size() : 0;
	size_t GlobalMethodStart = PassType::BuidCode == passtype ? _Lib._Assembly.Get_GlobalObject_Class()->Methods.size() : 0;
	size_t StaticInitStart = PassType::BuidCode == passtype && _Builder._StaticInit.Blocks.size() ? _Builder._StaticInit.Blocks.front()->Instructions.size(): 0;
	size_t ThreadInitStart = PassType::BuidCode == passtype && _Builder._threadInit.Blocks.size() ? _Builder._threadInit.Blocks.front()->Instructions.size() : 0;
	size_t StaticDeInitStart = PassType::BuidCode == passtype && _Builder._StaticdeInit.Blocks.size() ? _Builder._StaticdeInit.Blocks.front()->Instructions.size() : 0;
	size_t ThreadDeInitStart = PassType::BuidCode == passtype && _Builder._threaddeInit.Blocks.size() ? _Builder._threaddeInit.Blocks.front()->Instructions.size() : 0;
	size_t IRFunsStart = PassType::BuidCode == passtype ? _Builder.Funcs.size() : 0;
	size_t IRSybolsStart = PassType::BuidCode == passtype ? _Builder._Symbols.size() : 0;
	size_t IRConstStaticStart = PassType::BuidCode == passtype ? _Builder.ConstStaticStrings.size() : 0;

	bool DoneWithImports = false;

	if (passtype == PassType::BuidCode)
	{
		auto& FileData = GetFileData(File);

		for (auto& Import : FileData._Imports)
		{
			for (size_t i = 0; i < Import._AliasSymbols.size(); i++)
			{
				auto& Item = Import._AliasSymbols[i];
				Item->FullName = std::move(Import._TepSymbolFullNames[i]);//allow import alias to be seen.
			}
		}
	}

	if (StartingNameSpace.has_value())
	{
		_Table.AddScope(StartingNameSpace.value());
	}

	for (auto& node : File->_Nodes)
	{
		PushToNodeScope(*node.get());
		Defer _{ [this](){PopNodeScope(); } };


		if (DoneWithImports == false && node->Get_Type() != NodeType::ImportStatement && passtype == PassType::FixedTypes)
		{
			DoneWithImports = true;


			auto& FileData = GetFileData(LookingAtFile);

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

	if (StartingNameSpace.has_value())
	{
		_Table.RemoveScope();
	}

	OutofLoop:

	if (passtype == PassType::BuidCode)
	{
		auto& FileData = GetFileData(File);


		FileData.AssemblyInfoSpan = FileNodeData::SpanData::NewWithNewIndex(ClassesStart, _Lib._Assembly.Classes.size());
		FileData.GlobalObjectMethodInfoSpan = FileNodeData::SpanData::NewWithNewIndex(GlobalMethodStart, _Lib._Assembly.Get_GlobalObject_Class()->Methods.size());
		FileData.IRInitStaticSpan = FileNodeData::SpanData::NewWithNewIndex(StaticInitStart, _Builder._StaticInit.Blocks.size() ? _Builder._StaticInit.Blocks.front()->Instructions.size() : 0);
		FileData.IRInitThreadSpan = FileNodeData::SpanData::NewWithNewIndex(ThreadInitStart, _Builder._threadInit.Blocks.size() ? _Builder._threadInit.Blocks.front()->Instructions.size() : 0);
		FileData.IRDeInitStaticSpan= FileNodeData::SpanData::NewWithNewIndex(StaticDeInitStart, _Builder._StaticdeInit.Blocks.size() ? _Builder._StaticdeInit.Blocks.front()->Instructions.size() : 0);
		FileData.IRDeInitThreadSpan = FileNodeData::SpanData::NewWithNewIndex(ThreadDeInitStart, _Builder._threaddeInit.Blocks.size() ? _Builder._threaddeInit.Blocks.front()->Instructions.size() : 0);
		FileData.IRFuncsSpan = FileNodeData::SpanData::NewWithNewIndex(IRFunsStart, _Builder.Funcs.size());
		FileData.IRSymbolSpan = FileNodeData::SpanData::NewWithNewIndex(IRSybolsStart, _Builder._Symbols.size());
		FileData.IRConstStringSpan = FileNodeData::SpanData::NewWithNewIndex(IRConstStaticStart, _Builder.ConstStaticStrings.size());
	}

	if (passtype == PassType::FixedTypes || passtype == PassType::BuidCode)
	{
		auto& FileData = GetFileData(File);

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

	bool IsgenericInstantiation = GenericFuncName.size() && GenericFuncName.top().NodeTarget == &Node;
	bool Isgeneric = Node.Generic.Values.size();
	bool Isgeneric_t = Isgeneric && IsgenericInstantiation == false;


	const String ClassName = IsgenericInstantiation ? (String)ScopeHelper::GetNameFromFullName(GenericFuncName.top().GenericFuncName) : (String)Node.ClassName.Token->Value._String;
	_Table.AddScope(ClassName);

	auto SybID = GetSymbolID(Node);


	if (passtype == PassType::GetTypes)
	{
		DoSymbolRedefinitionCheck((String_view)_Table._Scope.ThisScope, SymbolType::Type_class, Node.ClassName.Token);
	}

	auto& Syb = passtype == PassType::GetTypes ?
		AddSybol(Isgeneric_t ? SymbolType::Generic_class : SymbolType::Type_class
			, (String)ClassName, _Table._Scope.ThisScope, Node.Access) :
		*GetSymbol(SybID);

	//we may jump to this node non linearly
	if (Syb.PassState == passtype)
	{
		_Table.RemoveScope();
		return;
	}

	OnAttributesNode(Node.Attributes);

	ClassInfo* ClassInf = nullptr;
	if (passtype == PassType::GetTypes)
	{
		_Table.AddSymbolID(Syb, SybID);
		Syb.NodePtr = Node.As();



		ClassInf = new ClassInfo();
		
		ClassInf->Conext = Get_SymbolConextRemoveOneScopeName();
		ClassInf->FullName = Syb.FullName;
		Syb.Info.reset(ClassInf);
		Syb.VarType.SetType(Syb.ID);

		if (Isgeneric_t)
		{
			_Table.AddScope(GenericTestStr);


			auto& GenericList = Node.Generic;
			InitGenericalias(GenericList, IsgenericInstantiation, ClassInf->_GenericData);

			_Table.RemoveScope();
		}

		ClassInf->_GenericData = {};
		auto& GenericList = Node.Generic;
		InitGenericalias(GenericList, IsgenericInstantiation, ClassInf->_GenericData);
	}
	else
	{
		ClassInf = Syb.Get_Info<ClassInfo>();
	}


	bool CheckgenericForErr = (Isgeneric_t && (passtype == PassType::GetTypes || passtype == PassType::FixedTypes));
	if (!Isgeneric_t || CheckgenericForErr)
	{
		
		ClassStackInfo classStackInfo;
		classStackInfo.Syb = &Syb;
		classStackInfo.Info = ClassInf;
		_ClassStack.push(classStackInfo);
		PushClassDependencie(ClassInf);
		if (CheckgenericForErr)
		{
			_Table.AddScope(GenericTestStr);
			Syb.FullName += GenericTestStr;
		}

		//if (passtype == PassType::GetTypes)
		{
			//PushTepAttributesInTo(Class._Class.Attributes);
		}

		if (passtype == PassType::BuidCode)
		{
			AddClass_tToAssemblyInfo(Node.Attributes, &Syb);
		}

		auto UseingIndex = _Table.GetUseingIndex();



		for (const auto& node : Node._Nodes)
		{
			PushToNodeScope(*node.get());
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
			PopNodeScope();
		}



		if (passtype == PassType::FixedTypes)
		{
			ClassInf->SizeInitialized = true;
		}

		PopClassDependencie();
		_Table.RemovePopUseing(UseingIndex);


		_ClassStack.pop();

		if (CheckgenericForErr)
		{
			_Table.RemoveScope();
			Syb.FullName = Syb.FullName.substr(0,sizeof(GenericTestStr)-1);
		}
	}
	else
	{
		Syb.NodePtr = Node.As();

	}




	if (passtype == PassType::FixedTypes)//auto make funcs
	{
		auto ThisCallType = TypeSymbol(Syb.ID);

		if ((Node.Inherited.Values.size() || ClassInf->_WillHaveFielddeInit) && GetSymbol(_Table._Scope.GetApendedString((String)ClassDestructorFunc), SymbolType::Func) == nullptr)
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
				auto DropFunc = &AddSybol(SymbolType::Func, ClassConstructorfunc, _Table._Scope.GetApendedString((String)ClassConstructorfunc), AccessModifierType::Public);
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
		for (auto& Item : Node.Inherited.Values)
		{
			auto& Str = Item.Name.Token->Value._String;

			auto Syb = GetSymbol(Str, SymbolType::Type);

			if (Syb == nullptr)
			{
				LogCantFindTypeError(Item.Name.Token, Str);
				continue;
			}
			if (Item.Generic.Values.size() == 0)
			{
				if (Syb->Type != SymbolType::Trait_class)
				{
					LogExpectedSymbolToBea(Item.Name.Token, *Syb, SymbolType::Trait_class);
					continue;
				}
			}
			else
			{
				if (Syb->Type != SymbolType::Generic_Trait)
				{
					LogExpectedSymbolToBea(Item.Name.Token, *Syb, SymbolType::Generic_Trait);
					continue;
				}

				auto CInfo = Syb->Get_Info<TraitInfo>();
				auto classnode = TraitNode::As(Syb->Get_NodeInfo<UCodeLang::Node>());
				Syb = InstantiateOrFindGeneric_Trait(Item.Name.Token, Syb, classnode->Generic, CInfo->_GenericData, Item.Generic);
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
			_Data.Syb = Syb;
			ClassInf->_InheritedTypes.push_back(_Data);


		}

		for (auto& Item : ClassInf->_InheritedTypes)
		{
			InheritTrait(&Syb, ClassInf, Item.Syb, Node.ClassName.Token);
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
				V.Pars.push_back({ false,ThisParType });
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
				V.Pars.push_back({ false,ThisParType });
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


	_Table.RemoveScope();






	Syb.PassState = passtype;
}

void SystematicAnalysis::OnAliasNode(const AliasNode& node)
{
	const bool IsgenericInstantiation = GenericFuncName.size() && GenericFuncName.top().NodeTarget == &node;
	const bool Isgeneric = node.Generic.Values.size();
	const bool Isgeneric_t = Isgeneric && IsgenericInstantiation == false;


	const String ClassName = IsgenericInstantiation ? (String)ScopeHelper::GetNameFromFullName(GenericFuncName.top().GenericFuncName) : (String)node.AliasName.Token->Value._String;
	auto SybID = GetSymbolID(node);

	_Table.AddScope(ClassName);

	if (passtype == PassType::GetTypes)
	{
		DoSymbolRedefinitionCheck((String_view)_Table._Scope.ThisScope, node.IsHardAlias ? SymbolType::Hard_Type_alias : SymbolType::Type_alias, node.AliasName.Token);
	}

	SymbolType _Type;
	if (Isgeneric_t) 
	{
		_Type = SymbolType::Generic_Alias;
	}
	else
	{
		_Type =node.IsHardAlias ? SymbolType::Hard_Type_alias : SymbolType::Type_alias;
	}
	auto& Syb = passtype == PassType::GetTypes ?
		AddSybol(_Type, (String)ClassName, _Table._Scope.ThisScope,node.Access) :
		*GetSymbol(SybID);

	//we may jump to this node non linearly
	if (Syb.PassState == passtype)
	{
		_Table.RemoveScope();
		return;
	}

	if (passtype == PassType::GetTypes)
	{
		_Table.AddSymbolID(Syb, SybID);


		Syb.NodePtr = node.As();

		if (!Isgeneric_t)
		{
			if (IsgenericInstantiation)
			{
				auto Ptr = new Generic_AliasInfo();
				Syb.Info.reset(Ptr);
				InitGenericalias(node.Generic, true, Ptr->_GenericData);
				Ptr->Conext = Get_SymbolConextRemoveOneScopeName();
			}
			else
				if (node._Type == AliasType::Type)
				{
					auto V = new AliasInfo();
					Syb.Info.reset(V);

					ConvertAndValidateType(node.Type, Syb.VarType, NodeSyb_t::Any);

					V->Conext = Get_SymbolConextRemoveOneScopeName();
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
						Par.IsOutPar = NodePar.IsOutVarable;
						ConvertAndValidateType(NodePar.Type, Par.Type, NodeSyb_t::Parameter);
					}

					ConvertAndValidateType(node_->ReturnType, V->Ret, NodeSyb_t::Ret);

					Syb.VarType.SetType(SybID);

					Syb.Type = SymbolType::Func_ptr;

					V->Conext = Get_SymbolConextRemoveOneScopeName();
				}
		}
		else
		{
			auto Ptr =new Generic_AliasInfo();
			Syb.Info.reset(Ptr);
			InitGenericalias(node.Generic,false, Ptr->_GenericData);
		}
	}
	if (passtype == PassType::FixedTypes)
	{
		if (!Isgeneric_t) 
		{
			if (node._Type == AliasType::Type)
			{
				ConvertAndValidateType(node.Type, Syb.VarType, NodeSyb_t::Any);
			}
			else
			{
				AliasNode_Func* node_ = (AliasNode_Func*)node._Node.get();
				FuncPtrInfo* nodeinfo_ = (FuncPtrInfo*)Syb.Info.get();

				for (size_t i = 0; i < nodeinfo_->Pars.size(); i++)
				{
					auto& NodePar = node_->Parameters.Parameters[i];
					auto& Par = nodeinfo_->Pars[i];
					Par.IsOutPar = NodePar.IsOutVarable;
					ConvertAndValidateType(NodePar.Type, Par.Type, NodeSyb_t::Parameter);
				}

				ConvertAndValidateType(node_->ReturnType, nodeinfo_->Ret, NodeSyb_t::Ret);
			}
		}
	}

	if (passtype == PassType::BuidCode)
	{
		if (!Isgeneric_t) {
			auto& V = _Lib.Get_Assembly().AddAlias((String)ClassName, _Table._Scope.ThisScope);
			V.Type = ConvertToTypeInfo(Syb.VarType);

			if (node.IsHardAlias)
			{
				V.HardAliasTypeID = GetTypeID(TypesEnum::CustomType, Syb.ID);
			}
			AddDependencyToCurrentFile(Syb.VarType);
		}
	}

	_Table.RemoveScope();

	Syb.PassState = passtype;
}
void SystematicAnalysis::OnUseingNode(const UsingNode& node)
{
	auto& T =node.ScopedName.ScopedName.back().token;
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
void SystematicAnalysis::InitGenericalias(const GenericValuesNode& GenericList, bool IsgenericInstantiation, Generic& Out)
{
	for (size_t i = 0; i < GenericList.Values.size(); i++)
	{
		auto& Item = GenericList.Values[i];

		auto GenericTypeName = Item.AsString();
		auto GenericType = &AddSybol(SymbolType::Type_alias, GenericTypeName,
			_Table._Scope.GetApendedString(GenericTypeName), AccessModifierType::Private);



		if (IsgenericInstantiation)
		{
			GenericType->Type = SymbolType::Type_alias;

			GenericFuncInfo& V2 = GenericFuncName.top();
			GenericType->VarType = (*V2.GenericInput)[i];
		}
		else
		{
			GenericType->Type = SymbolType::Unmaped_Generic_Type;

			SymbolID ID = GetSymbolID(GenericType->NodePtr);
			_Table.AddSymbolID(*GenericType, ID);

			GenericData Info;
			Info.SybID = ID;
			Info.type = GenericTypeToGenericDataType(Item.Generictype);
			Out._Generic.push_back(Info);

			if (Info.type == GenericData::Type::Pack)
			{
				bool IsLast = i == GenericList.Values.size() - 1;
				if (!IsLast)
				{
					LogParPackTypeIsNotLast(Item.Token);
				}
			}
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
	OnAttributesNode(node._Attributes);
	if (passtype == PassType::GetTypes)
	{
		SymbolType Type = IsGenericS && IsgenericInstantiation == false ?
			SymbolType::GenericFunc : SymbolType::Func;



		syb = &AddSybol(Type, (String)FuncName, FullName,node.Signature.Access);
		syb->NodePtr = node.As();
		_Table.AddSymbolID(*syb, sybId);

		FuncInfo* newInfo = new FuncInfo();
		newInfo->Conext = Get_SymbolConextRemoveOneScopeName();
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
			,node.Signature.Access);

			if (IsgenericInstantiation)
			{
				GenericType->Type = SymbolType::Type_alias;

				GenericFuncInfo& V2 = GenericFuncName.top();
			
				auto TepVarable = GenericTypeToGenericDataType(Item.Generictype);
				

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

				
				SymbolID ID = GetSymbolID(&Item);
				_Table.AddSymbolID(*GenericType, ID);

				GenericData Info;
				Info.SybID = ID;
				Info.type = GenericTypeToGenericDataType(Item.Generictype);

				newInfo->_GenericData._Generic.push_back(Info);

				if (Info.type == GenericData::Type::Pack)
				{
					bool IsLast = i == GenericList.Values.size() - 1;
					if (!IsLast)
					{
						LogParPackTypeIsNotLast(Item.Token);
					}
				}
			}

		}


		{
			auto& RetType = node.Signature.ReturnType.node;
			if (RetType && RetType->Get_Type() == NodeType::AnonymousTypeNode)
			{
				auto NewName = GetFuncAnonymousObjectFullName(FullName);


				SymbolID AnonymousSybID = GetSymbolID(RetType.get());
				auto& AnonymousSyb = AddSybol(SymbolType::Type_class, (String)NewName, NewName,node.Signature.Access);

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



		auto ClassSymBool = _ClassStack.size() ? _ClassStack.top().Syb : nullptr;
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
			,node.Signature.Access);
			auto ParSybID = GetSymbolID(Item);

			auto ParInfo = new ParameterInfo();
			ParInfo->IsOutValue = Item.IsOutVarable;
			GenericType->Info.reset(ParInfo);
			_Table.AddSymbolID(*GenericType, ParSybID);

			if (Item.Name.Token == nullptr)
			{
				newInfo->FrontParIsUnNamed = true;
			}
			newInfo->Pars.push_back({ Item.IsOutVarable,ConvertAndValidateType(Item.Type,NodeSyb_t::Parameter) });
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
			SymbolID AnonymousSybID = GetSymbolID(RetType.get());
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


			auto ParSybID = GetSymbolID(Item);
			auto& Sybol = *GetSymbol(ParSybID);
			ConvertAndValidateType(Item.Type, Sybol.VarType,NodeSyb_t::Parameter);
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
								auto Token = Item.Name.Token;
								LogParPackIsNotLast(Token);
							}

							break;
						}
					}
				}
			}

			if (Item2.Type.IsAn(TypesEnum::Void))
			{
				auto Token = Item.Name.Token;
				LogCantUseTypeVoidHere(Token);
			}
			if (Item2.Type.IsTypeInfo())
			{
				auto Token = Item.Name.Token;
				LogUseingTypeinfoInEvalFuncPar(Token);
			}
		}

	}




	bool buidCode = passtype == PassType::BuidCode;
	bool ignoreBody = !IsgenericInstantiation && IsGenericS;
	
	
	if (buidCode && !ignoreBody)
	{
		bool IsBuildingIR = true;
		auto DecName = MangleName(Info);

		if (IsBuildingIR)
		{

			LookingAtIRFunc = _Builder.NewFunc(GetIRID(Info), {});
			LookingAtIRBlock = LookingAtIRFunc->NewBlock("");
			PushNewStackFrame();



			auto& ParNodes = node.Signature.Parameters.Parameters;

			bool IsPackParLast = false;
			if (IsgenericInstantiation && ParNodes.size())
			{
				if (Info->Pars.back().Type._CustomTypeSymbol == GenericFuncName.top().Pack.value())
				{
					IsPackParLast = true;
				}
			}

			size_t ParNodeSize = ParNodes.size();
			if (IsPackParLast)
			{
				size_t ParsCount = ParNodes.size() - 1;

				const TypePackInfo* PackPar = GetSymbol(GenericFuncName.top().Pack.value())->Get_Info<TypePackInfo>();
				ParsCount += PackPar->List.size();
				LookingAtIRFunc->Pars.resize(ParsCount);

				ParNodeSize -= 1;
			}
			else
			{
				LookingAtIRFunc->Pars.resize(ParNodes.size());//becuase we are useing ptrs.
			}

			for (size_t i = 0; i < ParNodeSize; i++)
			{
				auto& Item = ParNodes[i];

				auto ParSybID = GetSymbolID(Item);
				auto& V = *GetSymbol(ParSybID);


				auto& VarType = Info->Pars[i];

				auto& d = LookingAtIRFunc->Pars[i];
				d.identifier = _Builder.ToID(ScopeHelper::GetNameFromFullName(V.FullName));
				
				if (Item.IsOutVarable)
				{
					d.type = IRType(IRTypes::pointer);
				}
				else
				{
					d.type = ConvertToIR(VarType);
				}



				if (HasDestructor(VarType.Type))
				{
					ObjectToDrop V;
					V.DropType = ObjectToDropType::Operator;
					V.ID = ParSybID;
					V._Operator = IROperator(&d);
					V.Type = VarType.Type;

					StackFrames.back().OnEndStackFrame.push_back(V);
				}

				V.IR_Par = &d;
			}

			if (IsPackParLast)
			{
				const TypePackInfo* PackPar = GetSymbol(GenericFuncName.top().Pack.value())->Get_Info<TypePackInfo>();

				size_t V = ParNodeSize;


				auto PackParSybol = GetSymbol(GetSymbolID(ParNodes.back()));
				auto PackParSybolName = ScopeHelper::GetNameFromFullName(PackParSybol->FullName);

				for (size_t i = 0; i < PackPar->List.size(); i++)
				{
					auto& PackType = PackPar->List[i];
					auto& d = LookingAtIRFunc->Pars[V];

					auto ParName = PackParSybolName + std::to_string(i);



					auto& TepPar = AddSybol(SymbolType::ParameterVarable, ParName, PackParSybol->FullName + std::to_string(i), AccessModifierType::Public);
					auto ParSybID = GetSymbolID(&TepPar);
					TepPar.IR_Par = &d;

					_Table.AddSymbolID(TepPar, ParSybID);


					d.identifier = _Builder.ToID(ParName);
					d.type = ConvertToIR(PackType);

					if (HasDestructor(PackType))
					{
						ObjectToDrop V;
						V.DropType = ObjectToDropType::Operator;
						V.ID = ParSybID;
						V._Operator = IROperator(&d);
						V.Type = PackType;

						StackFrames.back().OnEndStackFrame.push_back(V);
					}


					//

					V++;
				}
			}


			LookingAtIRFunc->ReturnType = ConvertToIR(Info->Ret);
		}


		if (node.Signature.HasExternKeyWord)
		{
			bool HasBody = node.Body.has_value();
			if (HasBody)
			{
				if (node.Signature.HasDynamicKeyWord)
				{
					LookingAtIRFunc->Linkage = IRFuncLink::DynamicExport;
				}
				else
				{
					LookingAtIRFunc->Linkage = IRFuncLink::StaticExport;
				}
			}
			else
			{
				if (node.Signature.HasDynamicKeyWord)
				{
					LookingAtIRFunc->Linkage = IRFuncLink::DynamicExternalLink;
				}
				else
				{
					LookingAtIRFunc->Linkage = IRFuncLink::StaticExternalLink;
				}
			}

		}


		Class_Data* Ptr = GetAssemblyClass(FullName);

		ClassMethod V;
		V.FullName = FullName;
		V.DecorationName = DecName;
		V.RetType = ConvertToTypeInfo(Info->Ret);

		V.IsThisFuncion = Info->FrontParIsUnNamed;
		for (size_t i = 0; i < Info->Pars.size(); i++)
		{
			auto& Item = Info->Pars[i];

			auto& F = V.ParsType.emplace_back();
			F.IsOutPar = node.Signature.Parameters.Parameters[i].IsOutVarable;
			F.Type = ConvertToTypeInfo(Item.Type);
		}

		ConvertAttributes(node._Attributes,V.Attributes.Attributes);


		Ptr->Methods.push_back(std::move(V));

		auto& RetType = node.Signature.ReturnType.node;
		if (RetType && RetType->Get_Type() == NodeType::AnonymousTypeNode)
		{
			SymbolID AnonymousSybID = GetSymbolID(RetType.get());
			auto V = GetSymbol(AnonymousSybID);

			AddClass_tToAssemblyInfo({}, V);

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


					LookingAtIRBlock->NewPushParameter(LookingAtIRBlock->NewLoad(&LookingAtIRFunc->Pars.front()));
					LookingAtIRBlock->NewCall(_Builder.ToID(InitFunc));
				}
			}
		}
		//
	}


	bool CheckGeneric = IsGenericS && (passtype == PassType::GetTypes || passtype == PassType::FixedTypes);

	if (CheckGeneric)
	{
		ignoreBody = false;
		_Table.AddScope(GenericTestStr);
	}

	if (node.Body.has_value() && !ignoreBody)
	{
		auto& Body = node.Body.value();
		size_t ErrorCount = _ErrorsOutput->Get_Errors().size();

		OnStatementsWithSetableRet(Body.Statements, Info->Ret, node.Signature.Name.Token);

		bool GotErr =ErrorCount < _ErrorsOutput->Get_Errors().size();
		if (GotErr)
		{
			syb->SetToInvalid();
		}
		syb->VarType = Info->Ret;
	}

	if (CheckGeneric)
	{
		_Table.RemoveScope();
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
Class_Data* SystematicAnalysis::GetAssemblyClass(const String& FullName)
{
	if (_ClassStack.empty())
	{
		auto& Assembly = _Lib.Get_Assembly();

		auto globalAssemblyObjectName = (String_view)ScopeHelper::_globalAssemblyObject;

		auto Ptr =  Assembly.Find_Class(globalAssemblyObjectName);
		if (Ptr == nullptr)
		{
			throw std::exception("bad path");
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
		else if (Func->Pars.size() == 2 && (Func->Pars[0].IsOutPar|| Func->Pars[1].IsOutPar))
		{
			LogCantUseOutInOverloadFunc(Name);
		}
	}
	break;
	case FuncInfo::FuncType::Cast:
	{
		if (Func->Pars.size() != 1)
		{


			LogPostfixOverloadPars(Name, Func);

		}
		else if (Func->Pars.size() == 1 && Func->Pars[0].IsOutPar)
		{
			LogCantUseOutInOverloadFunc(Name);
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
			else if (Func->Pars.size() == 2 && (Func->Pars[0].IsOutPar
				|| Func->Pars[1].IsOutPar))
			{
				LogCantUseOutInOverloadFunc(Name);
			}
		}
		else if (Systematic_PostfixOverloadData::IsPostfixOverload(Func->_FuncType))
		{
			if (Func->Pars.size() != 1)
			{
				LogPostfixOverloadPars(Name, Func);
			}
			else if (Func->Pars.size() == 1 && Func->Pars[0].IsOutPar)
			{
				LogCantUseOutInOverloadFunc(Name);
			}
		}
		else if (Systematic_CompoundOverloadData::IsCompoundOverload(Func->_FuncType))
		{
			if (Func->Pars.size() != 2)
			{
				LogBinaryOverloadPars(Name, Func);

			}
			else if (Func->Pars.size() == 2 && 
				(Func->Pars[0].IsOutPar || Func->Pars[1].IsOutPar))
			{
				LogCantUseOutInOverloadFunc(Name);
			}

		}
		else if (Systematic_UrinaryOverloadData::IsUrinaryOverload(Func->_FuncType))
		{
			if (Func->Pars.size() != 1)
			{
				LogPostfixOverloadPars(Name, Func);
			}
			else if (Func->Pars.size() == 1 && Func->Pars[0].IsOutPar)
			{
				LogCantUseOutInOverloadFunc(Name);
			}
		}
		else if (Systematic_MemberOverloadData::IsMemerOverload(Func->_FuncType))
		{
			if (Func->Pars.size() != 1)
			{
				LogPostfixOverloadPars(Name, Func);
			}
			else if (Func->Pars.size() == 1 && Func->Pars[0].IsOutPar)
			{
				LogCantUseOutInOverloadFunc(Name);
			}
		}
		break;
	}
}
void SystematicAnalysis::LogCantUseOutInOverloadFunc(const UCodeLang::Token& Name)
{
	_ErrorsOutput->AddError(ErrorCodes::InValidType, Name.OnLine, Name.OnPos, "you may not use Out parameters for overload.");
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


		syb = &AddSybol(SymbolType::StackVarable, (String)StrVarName, FullName,AccessModifierType::Public);
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
						const Token* token = node.TypeNode.Name.Token;

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
						For_Datas.AddValue(GetSymbolID(node), g);


						
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
		Add_SetLineNumber(node.Name, LookingAtIRBlock->GetIndex());
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

			const ForExpresion_Data& Data = For_Datas.at(GetSymbolID(node));
			{
				AddDependencyToCurrentFile(Data.FuncGetLoopAble);
				AddDependencyToCurrentFile(Data.FuncToCheck);
				AddDependencyToCurrentFile(Data.FuncToGet);
			}

			{
				auto finfo = Data.FuncGetLoopAble->Get_Info<FuncInfo>();
				auto Ex = node.Modern_List.Value.get();
				LookingForTypes.push(finfo->Pars[0].Type);
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

					
					if (f.Func->Pars[0].Type.IsAddress())
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

					if (f.Func->Pars[0].Type.IsAddress())
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

		auto& LambdaSym = AddSybol(SymbolType::LambdaObject, LambdaName, _Table._Scope.ThisScope, AccessModifierType::Public);
		_Table.AddSymbolID(LambdaSym, GetSymbolID(node));
		LambdaSym.Info =Unique_ptr<LambdaInfo>(new LambdaInfo());


		for (auto& Item : node.Pars.Parameters)
		{
			auto& Sym = AddSybol(SymbolType::ParameterVarable, Item.Name.AsString()
				, _Table._Scope.ScopeHelper::GetApendedString(Item.Name.AsStringView()),AccessModifierType::Public);

			_Table.AddSymbolID(Sym, GetSymbolID(Item));
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

		auto& LambdaSym = *GetSymbol(GetSymbolID(node));
		LambdaInfo* Info = LambdaSym.Get_Info<LambdaInfo>();
		Info->Ret = TypeSymbol(TypesEnum::Var);

		for (size_t i = 0; i < node.Pars.Parameters.size(); i++)
		{
			auto& Item = node.Pars.Parameters[i];
			Symbol& Sym = _Table.GetSymbol(GetSymbolID(Item));

			ConvertAndValidateType(Item.Type,Sym.VarType, NodeSyb_t::Parameter);

			Info->Pars.push_back({ Item.IsOutVarable,Sym.VarType });
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
						SymbolID ParID = GetSymbolID(ParItem);
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
						SymbolID ParID = GetSymbolID(ParItem);
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
			auto& SymClass = AddSybol(SymbolType::Type_class, LambdaClassName, FullName,AccessModifierType::Public);
		
			const SymbolID ClassSymID = GetSymbolID(&node._Capture);

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

				TepFuninfo.Pars.insert(TepFuninfo.Pars.begin(), { false,ClassPtr });

				String funcFullName= SymClass.FullName;
				ScopeHelper::GetApendedString(funcFullName, Overload_Invoke_Func);

				TepFuninfo.FullName = funcFullName;

				auto& SymClass = AddSybol(SymbolType::Func,Overload_Invoke_Func, funcFullName,AccessModifierType::Public);
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

		auto& LambdaSym = _Table.GetSymbol(GetSymbolID(node));
		LambdaInfo* Info = LambdaSym.Get_Info<LambdaInfo>();

		if (Info->UsesOuterScope())
		{
			const SymbolID ClassSymID = GetSymbolID(&node._Capture);
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
					auto& Sym = AddSybol(SymbolType::Class_Field, Item.Name, FullName,AccessModifierType::Public);//to make a this indirection.
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
					Symbol& Sym = _Table.GetSymbol(GetSymbolID(Item));

					IRPar V;
					V.type = ConvertToIR(Info->Pars[i]);
					V.identifier = _Builder.ToID((IRidentifier)Item.Name.Token->Value._String);
					LookingAtIRFunc->Pars[i+1] = V;

					Sym.IR_Par = &LookingAtIRFunc->Pars[i+1];
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
				Symbol& Sym = _Table.GetSymbol(GetSymbolID(Item));

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
	const bool IsgenericInstantiation = GenericFuncName.size() && GenericFuncName.top().NodeTarget == &node;
	const bool Isgeneric = node.Generic.Values.size();
	const bool Isgeneric_t = Isgeneric && IsgenericInstantiation == false;
	const bool CheckgenericForErr = (Isgeneric_t && (passtype == PassType::GetTypes || passtype == PassType::FixedTypes));

	const String ClassName = IsgenericInstantiation ? ScopeHelper::GetNameFromFullName(GenericFuncName.top().GenericFuncName) : node._Name.AsString();
	SymbolID sybId = GetSymbolID(node);
	_Table.AddScope(ClassName);

	auto& Syb = passtype == PassType::GetTypes ?
		AddSybol(Isgeneric_t ? SymbolType::Generic_Trait : SymbolType::Trait_class
			, (String)ClassName, _Table._Scope.ThisScope,node.Access) :
		*GetSymbol(sybId);

	//we may jump to this node non linearly
	if (Syb.PassState == passtype)
	{
		_Table.RemoveScope();
		return;
	}

	if (passtype == PassType::GetTypes)
	{
		_Table.AddSymbolID(Syb, sybId);


		TraitInfo* info = new TraitInfo();
		Syb.Info.reset(info);


		auto& SybClass = AddSybol(SymbolType::Type_class
			, (String)ClassName + "%Class", _Table._Scope.ThisScope + "%Class",node.Access);
		_Table.AddSymbolID(SybClass, GetSymbolID(node._Name));

		Syb.NodePtr = node.As();

		auto TepClass = new ClassInfo();
		TepClass->FullName = SybClass.FullName;

		info->TraitClassInfo = &SybClass;
		SybClass.Info.reset(TepClass);


		SybClass.VarType.SetType(SybClass.ID);
		info->Conext = Get_SymbolConextRemoveOneScopeName();
		InitGenericalias(node.Generic,IsgenericInstantiation, info->_GenericData);

		
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
				_Func.HasBody = funcnode.Body.has_value();

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
			_Func.Syb =&AddSybol(SymbolType::Func,ClassDestructorFunc, FuncDropFunc,AccessModifierType::Public);
			
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
	else if (passtype == PassType::FixedTypes)
	{
		TraitInfo* info = Syb.Get_Info<TraitInfo>();

		auto TepClass = info->TraitClassInfo->Get_Info<ClassInfo>();

		if (CheckgenericForErr)
		{
			_Table.AddScope(GenericTestStr);
		}

		ClassStackInfo classStackInfo;
		classStackInfo.Syb =GetSymbol(GetSymbolID(node._Name));
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
	else if (passtype == PassType::BuidCode && !Isgeneric_t)
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
	

		Trait_Data& TraitData = _Lib.Get_Assembly().AddTrait(ScopeHelper::GetNameFromFullName(Syb.FullName), Syb.FullName);
		TraitData.TypeID = GetTypeID(TypesEnum::CustomType, Syb.ID);
}


	_Table.RemoveScope();
}
void SystematicAnalysis::OnTag(const TagTypeNode& node)
{
	bool IsgenericInstantiation = GenericFuncName.size() && GenericFuncName.top().NodeTarget == &node;
	bool Isgeneric = node.Generic.Values.size();
	bool Isgeneric_t = Isgeneric && IsgenericInstantiation == false;


	SymbolID sybId = GetSymbolID(node);
	const String ClassName = IsgenericInstantiation ? ScopeHelper::GetNameFromFullName(GenericFuncName.top().GenericFuncName) : (String)node.AttributeName.Token->Value._String;
	_Table.AddScope(ClassName);

	auto& Syb = passtype == PassType::GetTypes ?
		AddSybol(Isgeneric_t ? SymbolType::Generic_Tag : SymbolType::Tag_class
			, (String)ClassName, _Table._Scope.ThisScope,node.Access) :
		*GetSymbol(sybId);

	//we may jump to this node non linearly
	if (Syb.PassState == passtype)
	{
		_Table.RemoveScope();
		return;
	}

	if (passtype == PassType::GetTypes)
	{
		_Table.AddSymbolID(Syb, sybId);
		Syb.NodePtr = node.As();

	
		TagInfo* info = new TagInfo();
		Syb.Info.reset(info);
		info->Conext = Get_SymbolConextRemoveOneScopeName();
		InitGenericalias(node.Generic, IsgenericInstantiation, info->_GenericData);
	}
	else if (passtype == PassType::FixedTypes)
	{

	}
	else if (passtype == PassType::BuidCode)
	{
		if (Syb.Type == SymbolType::Tag_class) 
		{
			Tag_Data& TagData = _Lib.Get_Assembly().AddTag(ScopeHelper::GetNameFromFullName(Syb.FullName), Syb.FullName);
			TagData.TypeID = GetTypeID(TypesEnum::CustomType, Syb.ID);
		}
		else
		{

		}
	}


	_Table.RemoveScope();
}
void SystematicAnalysis::OnBitCast(const BitCastExpression& node)
{
	if (passtype == PassType::FixedTypes)
	{
		if (_RemoveUnSafeArgWasPassed)
		{
			auto Token = node.KeywordToken;
			LogError(ErrorCodes::ExpectingSequence, Token->OnLine, Token->OnPos, "Cant do bitcast in safe mode.");
			return;
		}

		auto Token = node.KeywordToken;
		TypeSymbol ToType = ConvertAndValidateType(node._Type,NodeSyb_t::Any);
		
		if (!ToType.IsBadType()) 
		{
			if (!ToType.IsAddress() && (ToType._Type != TypesEnum::uIntPtr))
			{
				LogError(ErrorCodes::ExpectingSequence, Token->OnLine, Token->OnPos, "bitcast<T>. T must be an address or an uintptr and not '" + ToString(ToType) + "'");
			}
		}
		bool IsIntPtr = !ToType.IsAddress();

		if (IsIntPtr) {
			LookingForTypes.push(TypesEnum::uIntPtr);
		}
		else
		{
			TypeSymbol V = TypesEnum::Any;
			V.SetAsAddress();
			LookingForTypes.push(V);
		}
		
		OnExpressionTypeNode(node._Expression.Value.get(), GetValueMode::Read);
		
		LookingForTypes.pop();

		auto ExType = LastExpressionType;
		

		LastExpressionType = ToType;
	}
	else if (passtype == PassType::BuidCode)
	{
		TypeSymbol ToType = ConvertAndValidateType(node._Type, NodeSyb_t::Any);

		bool IsIntPtr = !ToType.IsAddress();

		//!to stop *(_Expression) and do (_Expression)
		if (!IsIntPtr) {
			LookingForTypes.push(TypesEnum::uIntPtr);
		}
		else
		{
			TypeSymbol V = TypesEnum::Any;
			V.SetAsAddress();
			LookingForTypes.push(V);
		}

		OnExpressionTypeNode(node._Expression.Value.get(), GetValueMode::Read);

		LookingForTypes.pop();

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
void SystematicAnalysis::InheritTrait(Symbol* Syb, ClassInfo* ClassInfo,const Symbol* Trait, const Token* ClassNameToken)
{
	const TraitInfo* Traitinfo = Trait->Get_Info<TraitInfo>();

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

							if (TraitPar.Type._CustomTypeSymbol == Traitinfo->TraitClassInfo->ID)
							{
								SubTraitParType.Type._CustomTypeSymbol = Syb->ID;
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
			_ClStack.Syb = Syb;
			_ClStack.Info = ClassInfo;
			_ClassStack.push(_ClStack);
		}
		
		auto oldpass = passtype;
		passtype = PassType::GetTypes;

		{
			AddExtendedErr("Were this = " + Syb->FullName + ".When instantiateing trait " + Trait->FullName, ClassNameToken);
		}

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
		{
			PopExtendedErr();
		}
	}
}
void SystematicAnalysis::BuildTrait(const Symbol* Syb, ClassInfo* ClassInfo,const Symbol* Trait, const Token* ClassNameToken)
{

	auto ID = ClassInfo->Get_InheritedTypesIndex(Trait);
	ClassInfo_InheritTypeInfo& IDSyb = ClassInfo->_InheritedTypes[ID.value()];

	{
		ClassStackInfo _ClStack;
		_ClStack._InStatements = false;
		_ClStack.Info = ClassInfo;
		_ClassStack.push(_ClStack);
	}

	{
		AddExtendedErr("Were this = " + Syb->FullName + ".When instantiateing trait " + Trait->FullName, ClassNameToken);
	}
	for (auto& Item : IDSyb.AddedFuncs)
	{
		const FuncNode& func = *(FuncNode*)Item.FuncNode;
		OnFuncNode(func);
	}
	{
		PopExtendedErr();
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
void SystematicAnalysis::OnCompileTimeIfNode(const CompileTimeIfNode& node)
{
	if (passtype == PassType::GetTypes)
	{
		LookingForTypes.push(TypesEnum::Bool);
		OnExpressionTypeNode(node.Expression.Value.get(),GetValueMode::Read);
		LookingForTypes.pop();
	}

	if (passtype == PassType::FixedTypes)
	{
		
		LookingForTypes.push(TypesEnum::Bool);
		auto BoolValue = Evaluate(TypesEnum::Bool,node.Expression);
		LookingForTypes.pop();

		if (BoolValue.has_value())
		{
			const bool& EvalValue = *(bool*)Get_Object(BoolValue.value());
		

			if (EvalValue)
			{
				auto PassOld = passtype;
				passtype = PassType::GetTypes;
				for (auto& Item : node.Body._Nodes)
				{
					OnStatement(*Item.get());
				}
				passtype = PassType::FixedTypes;
				for (auto& Item : node.Body._Nodes)
				{
					OnStatement(*Item.get());
				}

				passtype = PassOld;
			}
			else
			{
				if (node.Else.get())
				{
					ElseNode* Elsenode = ElseNode::As(node.Else.get());

					auto PassOld = passtype;
					passtype = PassType::GetTypes;
					
					for (const auto& node3 : Elsenode->Body._Nodes)
					{
						OnStatement(*node3);
					}

					passtype = PassType::FixedTypes;
					
					for (const auto& node3 : Elsenode->Body._Nodes)
					{
						OnStatement(*node3);
					}

					passtype = PassOld;
				}
			}

			ValidNodes.AddValue(GetSymbolID(node), EvalValue);

		}

	}


	if (passtype == PassType::BuidCode)
	{
		const bool EvalValue = ValidNodes.at(GetSymbolID(node));

		if (EvalValue)
		{
			for (auto& Item : node.Body._Nodes)
			{
				OnStatement(*Item.get());
			}
		}
		else
		{
			ElseNode* Elsenode = ElseNode::As(node.Else.get());

			for (const auto& node3 : Elsenode->Body._Nodes)
			{
				OnStatement(*node3);
			}
		}
	}
}
TypeSymbol SystematicAnalysis::GetUnMapType()
{
	if (!UnMapTypeSybol.has_value())
	{
		auto& TypeSyb = AddSybol(SymbolType::Unmaped_Generic_Type, CompilerGenerated("UnMapedType"), CompilerGenerated("UnMapedType"), AccessModifierType::Public);
		_Table.AddSymbolID(TypeSyb, GetSymbolID(&TypeSyb));
		UnMapTypeSybol = TypeSyb.ID;
	}	
	return TypeSymbol(UnMapTypeSybol.value());

}
bool SystematicAnalysis::IsUnMapType(const TypeSymbol& Type)
{
	auto Syb = GetSymbol(Type);
	if (Syb)
	{
		return Syb->Type == SymbolType::Unmaped_Generic_Type;
	}
	return {};
}
void SystematicAnalysis::OnCompileTimeforNode(const CompileTimeForNode& node)
{
	if (passtype == PassType::GetTypes)
	{
		if (node.Type == CompileTimeForNode::ForType::Traditional)
		{
			auto Token = node.Name;
			LogError(ErrorCodes::ExpectingSequence, Token->OnLine, Token->OnPos, "Traditional for loops are not yet added to CompileTimeforNode");
		}
		else
		{

		}
	}
	if (passtype == PassType::FixedTypes)
	{
		if (node.Type == CompileTimeForNode::ForType::modern)
		{

			LookingForTypes.push(TypesEnum::Any);
			OnExpressionTypeNode(node.Modern_List.Value.get(), GetValueMode::Read);
			LookingForTypes.pop();

			auto ListType = LastExpressionType;

			bool CanBeLooped = false;
			Symbol* ListTypeSyb;
			if (ListType._CustomTypeSymbol.HasValue())
			{
				ListTypeSyb = GetSymbol(ListType);
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
				auto Token = node.Name;
				LogError(ErrorCodes::ExpectingSequence, Token->OnLine, Token->OnPos, "Cant Loop over type of '" + ToString(ListType) + "'");
			}
			else
			{

				{//test loop for Errs
					size_t OldErrCount = _ErrorsOutput->Get_Errors().size();
					_Table.AddScope("___forTest");
					const String VarableName = (String)node.Name->Value._String;

					auto& ParSyb = AddSybol(SymbolType::Unmaped_Varable, VarableName, _Table._Scope.GetApendedString(VarableName), AccessModifierType::Public);
					ParSyb.VarType = GetUnMapType();
					_Table.AddSymbolID(ParSyb, GetSymbolID(&ParSyb));
					{
						auto Token = node.Name;
						AddExtendedErr("Were '" + VarableName + "' is unmaped for Errors.Before for loop expansion", Token);
					}
					CompileTimeforNodeEvaluateStatements(node);
					{
						PopExtendedErr();
					}
					_Table.RemoveScope();


					bool GotErrs = OldErrCount != _ErrorsOutput->Get_Errors().size();
					if (GotErrs) { return; }
				}


				//
				if (ListTypeSyb->Type == SymbolType::Type_Pack)
				{
					const Node* NodePtr = node.Modern_List.Value.get();
					if (NodePtr->Get_Type() == NodeType::ValueExpressionNode)
					{
						const ValueExpressionNode* ValueNodePtr = ValueExpressionNode::As(NodePtr);
						const auto ExValuePtr = ValueNodePtr->Value.get();
						if (ExValuePtr->Get_Type() == NodeType::ReadVariableNode)
						{
							const auto ReadVarablePtr = ReadVariableNode::As(ExValuePtr);

							GetExpressionMode.push(GetValueMode::Read);
							GetMemberTypeSymbolFromVar_t V;
							bool VBool = GetMemberTypeSymbolFromVar(ReadVarablePtr->VariableName, V);
							GetExpressionMode.pop();
							
							if (VBool)
							{
								auto ParSyb = V.Symbol;
								if (ParSyb->Type == SymbolType::ParameterVarable)
								{
									const TypePackInfo* PackInfo = ListTypeSyb->Get_Info<TypePackInfo>();

									const String ScopeName = std::to_string(GetSymbolID(node).AsInt());
									const String VarableName = (String)node.Name->Value._String;

									CompileTimeforNode TepData;
									TepData.SybToLoopOver = V.Symbol;
									

									for (size_t i = 0; i < PackInfo->List.size(); i++)
									{
										auto& Item = PackInfo->List[i];
									
										_Table.AddScope(ScopeName + std::to_string(i));

										
										auto& ParSyb = AddSybol(SymbolType::ParameterVarable, VarableName, _Table._Scope.GetApendedString(VarableName), AccessModifierType::Public);
										_Table.AddSymbolID(ParSyb, GetSymbolID(&ParSyb));
										ParSyb.VarType = Item;

										size_t OldErrCount = _ErrorsOutput->Get_Errors().size();
										{
											auto Token = node.Name;
											AddExtendedErr("Were '" + VarableName + "' is type of " + ToString(ParSyb.VarType), Token);
										}
										CompileTimeforNodeEvaluateStatements(node);
										{
											PopExtendedErr();
										}

										TepData.SybItems.push_back(&ParSyb);

										
										_Table.RemoveScope();

										bool GotErrs = OldErrCount != _ErrorsOutput->Get_Errors().size();
										if (GotErrs) { continue; }
									}

									ForNodes.AddValue(GetSymbolID(node),std::move(TepData));
								}
							}
						}
					}
				
				}
				else if (ListTypeSyb->Type == SymbolType::Type_StaticArray)
				{
					const StaticArrayInfo* StaticInfo = ListTypeSyb->Get_Info<StaticArrayInfo>();


					const String ScopeName = std::to_string(GetSymbolID(node).AsInt());
					const String VarableName = (String)node.Name->Value._String;

					auto ListArray = Evaluate(ListType,node.Modern_List);
					if (ListArray.has_value())
					{
						size_t ItemSize = GetSize(StaticInfo->Type).value();

						RawEvaluatedObject _DataAsIndex;
						_DataAsIndex.ObjectSize = ItemSize;
						_DataAsIndex.Object_AsPointer.reset(new Byte[ItemSize]);
						
						CompileTimeforNode TepData;
						TepData.SybToLoopOver = ListTypeSyb;

						auto& ListArrayValue = ListArray.value();

						for (size_t i = 0; i < StaticInfo->Count; i++)
						{
							void* ItemOffset = ListArrayValue.EvaluatedObject.Object_AsPointer.get() + (i * ItemSize);
							Set_ObjectAs(StaticInfo->Type,_DataAsIndex,ItemOffset, ItemSize);

							_Table.AddScope(ScopeName + std::to_string(i));
							

							auto& ParSyb = AddSybol(SymbolType::ConstantExpression, VarableName, _Table._Scope.GetApendedString(VarableName), AccessModifierType::Public);
							_Table.AddSymbolID(ParSyb, GetSymbolID(&ParSyb));

							
							ConstantExpressionInfo* ContInfo = new ConstantExpressionInfo();
							ParSyb.Info.reset(ContInfo);

							ContInfo->Ex = _DataAsIndex;
							ParSyb.VarType = StaticInfo->Type;

							size_t OldErrCount = _ErrorsOutput->Get_Errors().size();
							{
								auto Token = node.Name;
								AddExtendedErr("Were '" + VarableName + "' = " + ToString(ParSyb.VarType, ContInfo->Ex), Token);
							}
							CompileTimeforNodeEvaluateStatements(node);
							{
								PopExtendedErr();
							}
							TepData.SybItems.push_back(&ParSyb);

							


							_Table.RemoveScope();

							bool GotErrs = OldErrCount != _ErrorsOutput->Get_Errors().size();
							if (GotErrs){continue;}
						}

						
						ForNodes.AddValue(GetSymbolID(node), std::move(TepData));
					}
				}

				else
				{
					throw std::exception("bad path");
				}
			}
		}
	}
	if (passtype == PassType::BuidCode)
	{
		CompileTimeforNode& Nodes = ForNodes.at(GetSymbolID(node));
		if (Nodes.SybToLoopOver->Type == SymbolType::ParameterVarable)
		{
			const String ScopeName = std::to_string(GetSymbolID(node).AsInt());
			
			for (size_t i = 0; i < Nodes.SybItems.size(); i++)
			{
				auto& Item = Nodes.SybItems[i];

				size_t IRParIndex = LookingAtIRFunc->Pars.size() - Nodes.SybItems.size() + i;
				Item->IR_Par =&LookingAtIRFunc->Pars[IRParIndex];

				{
					auto Token = node.Name;
					AddExtendedErr("Were '" + (String)node.Name->Value._String + "' is type of " + ToString(Item->VarType), Token);
				}
				_Table.AddScope(ScopeName + std::to_string(i));

				for (const auto& node2 : node.Body._Nodes)
				{
					OnStatement(*node2);
				}

				_Table.RemoveScope();
				{
					PopExtendedErr();
				}
			}
		}
		else if (Nodes.SybToLoopOver->Type == SymbolType::Type_StaticArray)
		{
			const String ScopeName = std::to_string(GetSymbolID(node).AsInt());
			

			for (size_t i = 0; i < Nodes.SybItems.size(); i++)
			{
				auto& Item = Nodes.SybItems[i];

				_Table.AddScope(ScopeName + std::to_string(i));
				{
					ConstantExpressionInfo* ConstExpressionInfo = Item->Get_Info< ConstantExpressionInfo>();
					auto Token = node.Name;
					AddExtendedErr("Were '" + ScopeHelper::GetNameFromFullName(Item->FullName) + "' = " + ToString(Item->VarType, ConstExpressionInfo->Ex), Token);
				}
				for (const auto& node2 : node.Body._Nodes)
				{
					OnStatement(*node2);
				}
				{
					PopExtendedErr();
				}
				_Table.RemoveScope();
			}
		}
	}
}
void SystematicAnalysis::CompileTimeforNodeEvaluateStatements(const CompileTimeForNode& node)
{
	auto TepPass = passtype;
	size_t OldErrCount = _ErrorsOutput->Get_Errors().size();

	passtype = PassType::GetTypes;
	for (const auto& node2 : node.Body._Nodes)
	{
		OnStatement(*node2);
	}

	bool GotErrs = OldErrCount != _ErrorsOutput->Get_Errors().size();
	if (!GotErrs)
	{
		passtype = PassType::FixedTypes;
		for (const auto& node2 : node.Body._Nodes)
		{
			OnStatement(*node2);
		}

	}
	passtype = TepPass;
}

void SystematicAnalysis::LogMissingFuncionforTrait(const String_view& FuncName, const FuncInfo* Info, const Symbol* Trait, const Token* ClassNameToken)
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
Symbol* SystematicAnalysis::NewDropFuncSymbol(ClassInfo* ClassInfo, TypeSymbol& ClassAsType)
{

	String FullName = ClassInfo->FullName;
	ScopeHelper::GetApendedString(FullName, (String)ClassDestructorFunc);

	auto DropFunc = &AddSybol(SymbolType::Func, ClassDestructorFunc, FullName,AccessModifierType::Public);
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

				/*
				if (ErrCount == _ErrorsOutput->Get_Errors().size()) {
					OnStatement(*node2);//re do
				}
				*/
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
	SymbolID ImportSymbolID = GetSymbolID(node);
	if (passtype == PassType::GetTypes)
	{
		auto& Syb = AddSybol(SymbolType::Null, CompilerGenerated("importData") + std::to_string(node._Token->OnLine),"",AccessModifierType::Public);
		
		Imports_Info* NewImports = new  Imports_Info();
		Syb.Info.reset(NewImports);
		_Table.AddSymbolID(Syb,ImportSymbolID);

		
		NewImports->NewAliases.resize(node._Imports.size());

		for (auto& Item : node._Imports)
		{
			

			for (auto& Item2 : Item._ImportedSybol.ScopedName)
			{
				if (Item2.Operator != ScopedName::Operator_t::ScopeResolution
					&& Item2.Operator != ScopedName::Operator_t::Null)
				{
					LogError(ErrorCodes::ExpectingSequence, Item2.token->OnLine, Item2.token->OnPos,
						"Must use a '::' as Opetator Here");
				}
			}
			
		}
	}
	else if (passtype == PassType::FixedTypes)
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
				Name += GetScopedNameAsString(node._StartingNameSpace.value());
				Name += ScopeHelper::_ScopeSep;
			}
					
			Name += GetScopedNameAsString(Item._ImportedSybol);
			
			
			auto List = _Table.GetSymbolsWithName(Name);

			if (List.empty())
			{
				auto Token = Item._ImportedSybol.ScopedName.front().token;
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
					auto Token = Item._ImportedSybol.ScopedName.front().token;
					auto Sybol = List.front();
					LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos,
						"Cant Map Symbol '" + Sybol->FullName + "[" + ToString(Sybol->Type) + "]' to Alias");
				}
				else if (!IsOuterfile)
				{
					auto Token = Item._ImportedSybol.ScopedName.front().token;
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
							auto& NewSyb = AddSybol(SymbolType::Type_alias, (String)AliasName->Value._String, (String)AliasName->Value._String, AccessModifierType::Public);
							_Table.AddSymbolID(NewSyb, GetSymbolID(&NewSybInfo));
							
							NewSyb.VarType = SybToBind->VarType;

							NewSybInfo.Sym = &NewSyb;

							NewSyb.PassState = SybToBind->PassState;
							NewSyb.VarType = SybToBind->VarType;
						}
						else if (NewSybInfo.Type == ImportBindType::Func)
						{
							auto& NewSyb = AddSybol(SymbolType::Func, (String)AliasName->Value._String, (String)AliasName->Value._String, AccessModifierType::Public);
							_Table.AddSymbolID(NewSyb, GetSymbolID(&NewSybInfo));
							NewSybInfo.Sym = &NewSyb;
							
							FuncInfo* NewFunc = new FuncInfo();
							NewSyb.Info.reset(NewFunc);

							NewSyb.PassState = SybToBind->PassState;
							*NewFunc = *SybToBind->Get_Info<FuncInfo>();
							
							NewSyb.VarType = SybToBind->VarType;


						}
						_ImportData._AliasSymbols.push_back(NewSybInfo.Sym);

					}
				
					GetFileData(LookingAtFile)._Imports.push_back(std::move(_ImportData));
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
					auto Token = Item._ImportedSybol.ScopedName.front().token;
					auto Sybol = List.front();

					String V = "importing '" + Sybol->FullName + "' but it's Declared in this file.";
					LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos, V);

				}

				FileNodeData::ImportData _ImportData;
				_ImportData.ImportSymbolFullName = Name;
				_ImportData.IsImportUsed = &ImportInfo.IsUsed;
				GetFileData(LookingAtFile)._Imports.push_back(std::move(_ImportData));
			}

		}
	}
	else if (passtype == PassType::BuidCode)
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
				auto Token = Item._ImportedSybol.ScopedName.front().token;
				auto Name = GetScopedNameAsString(Item._ImportedSybol);

				LogError(ErrorCodes::ExpectingSequence, Token->OnLine, Token->OnPos, "'" + Name + "' Import Symbol was not Used");
			}
		}
	}
}
void SystematicAnalysis::TypeDoesNotHaveForOverload(const UCodeLang::Token* Token, UCodeLang::FrontEnd::TypeSymbol& ExType)
{
	if (ExType.IsBadType()) { return; }
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
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

	if (Value.IsTypeInfo())
	{
		//Err
		return {};
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
		else if (syb.Type == SymbolType::ConstantExpression)
		{
			return  ConvertToIR(syb.VarType);
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
SymbolID SystematicAnalysis::GetSymbolID(const void* Item)
{
	#ifdef DEBUG
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
		throw std::exception("Item Is on Stack not on heep");
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
		auto R = SymbolID(IDIndex++);
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
	PushToNodeScope(node2);
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
	const bool IsgenericInstantiation = GenericFuncName.size() && GenericFuncName.top().NodeTarget == &node;
	const bool Isgeneric = node.Generic.Values.size();
	const bool Isgeneric_t = Isgeneric && IsgenericInstantiation == false;


	SymbolID SybID = GetSymbolID(node);

	const String ClassName = IsgenericInstantiation ? (String)ScopeHelper::GetNameFromFullName(GenericFuncName.top().GenericFuncName) : (String)node.EnumName.Token->Value._String;
	_Table.AddScope(ClassName);

	if (passtype == PassType::GetTypes)
	{
		DoSymbolRedefinitionCheck((String_view)_Table._Scope.ThisScope, SymbolType::Type_class, node.EnumName.Token);
	}

	auto& Syb = passtype == PassType::GetTypes ?
		AddSybol(Isgeneric_t ? SymbolType::Generic_Enum : SymbolType::Enum
			, (String)ClassName, _Table._Scope.ThisScope,node.Access) :
		*GetSymbol(SybID);

	//we may jump to this node non linearly
	if (Syb.PassState == passtype)
	{
		_Table.RemoveScope();
		return;
	}

	EnumInfo* ClassInf;
	if (passtype == PassType::GetTypes)
	{
		_Table.AddSymbolID(Syb, SybID);
		Syb.NodePtr = node.As();


		ClassInf = new EnumInfo();
		ClassInf->Conext = Get_SymbolConextRemoveOneScopeName();
		ClassInf->FullName = Syb.FullName;
		Syb.Info.reset(ClassInf);
		Syb.VarType.SetType(Syb.ID);

		InitGenericalias(node.Generic,IsgenericInstantiation, ClassInf->_GenericData);

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
		if (ClassInf->Basetype.IsBadType() || IsUnMapType(ClassInf->Basetype)) { _Table.RemoveScope(); return; }
		if (!ConstantExpressionAbleType(ClassInf->Basetype))
		{
			LogTypeMustBeAnConstantExpressionAble(node.BaseType.Name.Token, ClassInf->Basetype);
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
				auto& EnumFeild = AddSybol(SymbolType::Enum_Field, (String)ItemName, _Table._Scope.ThisScope,AccessModifierType::Public);
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

							SymbolID AnonymousSybID = GetSymbolID(VariantType_.node.get());
							auto& AnonymousSyb = AddSybol(SymbolType::Type_class, (String)NewName, NewName,AccessModifierType::Public);

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

			auto FieldOpt = ClassInf->GetField(ItemName);
			auto& Field = *FieldOpt.value();
			if (Item.Expression.Value)
			{
				auto& Type = ClassInf->Basetype;
				
				auto V = Evaluate(ex,Type, Item.Expression);

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

							_Table.RemoveScope();
							return;
						}
						if (!HasConstantPostfixOperator(Type, TokenType::increment))
						{
							Token temp;
							temp.OnLine = LineDataToken->OnLine;
							temp.OnPos = LineDataToken->OnPos;
							temp.Type = TokenType::increment;


							LogCantDoPostfixOpForTypes_Constant(&temp, Type);

							_Table.RemoveScope();
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

		Enum_Data& EnumData = _Lib.Get_Assembly().AddEnum(ScopeHelper::GetNameFromFullName(Syb.FullName), Syb.FullName);
		EnumData.BaseType = ConvertToTypeInfo(ClassInf->Basetype);
		EnumData.TypeID = GetTypeID(TypesEnum::CustomType,Syb.ID);

		EnumData.Values.resize(ClassInf->Fields.size());
		for (size_t i = 0; i < ClassInf->Fields.size(); i++)
		{
			auto& ClassDataItem = ClassInf->Fields[i];
			auto& EnumDataItem = EnumData.Values[i];
			EnumDataItem.Name = ClassDataItem.Name;
			EnumDataItem._Data.Resize(ClassDataItem.Ex.ObjectSize);
			memcpy(EnumDataItem._Data.Get_Data(), ClassDataItem.Ex.Object_AsPointer.get(),ClassDataItem.Ex.ObjectSize);
		}
		if (ClassInf->VariantData) 
		{
			auto UnionFullName = GetUnrefencedableName(GetEnumVariantUnionName(ClassInf->FullName));
			auto UnionName = GetEnumVariantUnionName(GetUnrefencedableName((String)ClassInf->Get_Name()));

			Class_Data& EnumUnion = _Lib.Get_Assembly().AddClass(UnionName, UnionFullName);
			
			auto& UnionSyb = AddSybol(SymbolType::Type_class, UnionName, UnionFullName, AccessModifierType::Default);
			_Table.AddSymbolID(UnionSyb,GetSymbolID((Node&)UnionSyb));

			EnumUnion.TypeID = GetTypeID(TypesEnum::CustomType, UnionSyb.ID);
			EnumData.EnumVariantUnion = EnumUnion.TypeID;

			auto& List = ClassInf->VariantData.value().Variants;

			size_t MaxSize = 0;
			for (size_t i = 0; i < List.size(); i++)
			{
				auto& Item = List[i];

				if (Item.ClassSymbol.has_value())
				{
					Symbol* Sym = GetSymbol(Item.ClassSymbol.value());

					AddClass_tToAssemblyInfo({}, Sym);//has '!' post fix so its Unrefencedable

					auto Type = TypeSymbol(Sym->ID);

					ClassField V;
					V.offset = 0;
					V.Name = ClassInf->Fields[i].Name;
					V.Type = ConvertToTypeInfo(Type);
					EnumUnion.Fields.push_back(std::move(V));

					auto& EnumDataItem = EnumData.Values[i];
					EnumDataItem.EnumVariantType = ConvertToTypeInfo(Type);




					size_t TypeSize = 0;
					for (auto& Field : Sym->Get_Info<ClassInfo>()->Fields)
					{
						TypeSize += GetSize(Field.Type).value();
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
						V.Type = ConvertToTypeInfo(Type);
						EnumUnion.Fields.push_back(std::move(V));

						auto& EnumDataItem = EnumData.Values[i];
						EnumDataItem.EnumVariantType = V.Type;

						size_t TypeSize = GetSize(Type).value();
						if (TypeSize > MaxSize)
						{
							MaxSize = TypeSize;
						}
					}
				}


				for (auto& Item2 : Item.Types)
				{
					AddDependencyToCurrentFile(Item2);
				}
				
			}
		
			EnumUnion.Size = MaxSize;
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
	bool IsField = InSideClass && DeclareStaticVariableNode_t::ClassField == type;

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
		DoSymbolRedefinitionCheck(FullName, SysType, node.Name.Token);


		syb = &AddSybol(SysType, StrVarName, FullName,node.Access);
		_Table.AddSymbolID(*syb, sybId);

		syb->Set_NodeInfo(node.As());
		if (syb->Type == SymbolType::ConstantExpression)
		{
			ConstantExpressionInfo* info = new ConstantExpressionInfo();
			info->Exnode = &node.Expression;
			info->Conext = Get_SymbolConext();
			syb->Info.reset(info);
		}
		else
		{
			DeclareVariableInfo* info = new DeclareVariableInfo();
			info->LineNumber = node.Name.Token->OnPos;
			info->Conext = Get_SymbolConext();
			syb->Info.reset(info);
		}

		//
		if (type == DeclareStaticVariableNode_t::Static
			|| type == DeclareStaticVariableNode_t::Thread
			|| type == DeclareStaticVariableNode_t::Eval)
		{
			if (!node.Expression.Value)
			{
				auto Token = node.Name.Token;
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
					throw std::exception("not added");
					break;
				}

				LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos, VarType + " Varable must be assigned.missing '='.");
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

	//we may jump to this node non linearly
	if (syb->PassState == passtype)
	{
		_Table.RemoveScope();
		return;
	}

	if (passtype == PassType::FixedTypes)
	{
		auto& VarType = syb->VarType;
		ConvertAndValidateType(node.Type, VarType, NodeSyb_t::ClassFeild);
		VarType.SetAsLocation();


		auto Ex = node.Expression.Value.get();
		ExTypeDeclareVarableCheck(VarType, Ex, node.Name.Token);

		if (VarType.IsTypeInfo() && type != DeclareStaticVariableNode_t::Eval)
		{
			auto Token = node.Name.Token; 
			LogUseingTypeinfoInNonEvalVarable(Token);
		}

		if (VarType.IsAn(TypesEnum::Void))
		{
			auto Token = node.Name.Token;
			LogCantUseTypeVoidHere(Token);
		}
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
			if (syb->Type != SymbolType::ConstantExpression)
			{



				if (syb->Type == SymbolType::StackVarable)
				{
					OnVarable = LookingAtIRBlock->NewLoad(ConvertToIR(syb->VarType));

					Add_SetLineNumber(node.Name.Token, LookingAtIRBlock->GetIndex());
					Add_SetVarableInfo(*syb, OnVarable);
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

					Add_SetLineNumber(node.Name.Token, LookingAtIRBlock->GetIndex());


					Add_SetVarableInfo(*syb,nullptr);
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

					Add_SetLineNumber(node.Name.Token, LookingAtIRBlock->GetIndex());
					Add_SetVarableInfo(*syb, nullptr);

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
							auto Classtype = TypeSymbol(_ClassStack.top().Syb->ID);
							Classtype._IsAddress = true;

							IRPar ThisPar;
							ThisPar.identifier = _Builder.ToID(ThisSymbolName);
							ThisPar.type = ConvertToIR(Classtype);
							Classinfo->_ClassFieldInit->Pars.push_back(ThisPar);
						}
					}

					LookingAtIRFunc = Classinfo->_ClassFieldInit;
					LookingAtIRBlock = LookingAtIRFunc->Blocks.front().get();

					Add_SetLineNumber(node.Name.Token, LookingAtIRBlock->GetIndex());

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

		}
		else
		{
			if (syb->Type == SymbolType::StackVarable)
			{
				Add_SetLineNumber(node.Name.Token, LookingAtIRBlock->GetIndex());

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

			if (syb->Type == SymbolType::ConstantExpression && !VarType.IsNull())
			{
				ConstantExpressionInfo* Info = syb->Get_Info<ConstantExpressionInfo>();

				LookingForTypes.push(VarType);
				auto Item = EvaluateToAnyType(node.Expression);
				LookingForTypes.pop();

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

				Class.Size += GetSize(Item->Type).value_or(0);
				
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
		if (syb->Type != SymbolType::ConstantExpression) 
		{
			Add_SetLineNumber(node.Name.Token, LookingAtIRBlock->GetIndex());


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

	syb->PassState = passtype;
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
		auto Classinfo = _ClassStack.top().Info;
		auto classSb = _ClassStack.top().Syb;
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
void SystematicAnalysis::AddDestructorToStack(const Symbol* syb, const SymbolID& sybId, IRInstruction* OnVarable)
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
void SystematicAnalysis::ExDeclareVariableTypeCheck(TypeSymbol& VarType, const TypeSymbol& Ex, const Token* Token)
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
		OnExpressionTypeNode(node.ToAssign.Value.get(), GetValueMode::Write);
		LookingForTypes.pop();

		auto AssignType = LastExpressionType;

		LookingForTypes.push(AssignType);
		OnExpressionTypeNode(node.Expression.Value.get(), GetValueMode::Read);
		LookingForTypes.pop();


		auto ExpressionType = LastExpressionType;

		

		
		if (!CanBeImplicitConverted(AssignType, ExpressionType,false))
		{
			auto  Token = LastLookedAtToken;
			LogCantCastImplicitTypes(Token,AssignType, ExpressionType, false);

		}
		auto ID = GetSymbolID(node);

		AssignExpression_Data Data;
		Data.Op0 = ExpressionType;
		Data.Op1 = AssignType;

		AssignExpressionDatas.AddValue(ID, Data);
	}
	else if (passtype == PassType::BuidCode)
	{

		auto ID = GetSymbolID(node);
		auto& AssignType = AssignExpressionDatas.at(ID);

		Add_SetLineNumber(node.Token, LookingAtIRBlock->Instructions.size());

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

		if (node.ReassignAddress)
		{
			WriteTo(ExIR,_LastStoreField);
		}
		else 
		{
			WriteTo(ExIR, ExpressionType, _LastStoreField);
		}
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
		BoolCode = IfIndex.logicalNot;
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
void SystematicAnalysis::OnDeclareEvalVariableNode(const DeclareEvalVariableNode& node)
{
	OnDeclareVariablenode(node.Variable, DeclareStaticVariableNode_t::Eval);
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
		return UseOutput ? IROperator(Output) : IROperator(_Builder.ToID(In.Symbol->FullName));
		break;
	default:
		throw std::exception("not added");
		break;
	}
}
IROperator  SystematicAnalysis::BuildMember_DereferencStore(const GetMemberTypeSymbolFromVar_t& In)
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
		return Output;
	case  SymbolType::ParameterVarable:
		return UseOutput ? IROperator(IROperatorType::DereferenceOf_IRInstruction,Output) : IROperator(IROperatorType::DereferenceOf_IRParameter,In.Symbol->IR_Par);
		break;
	case  SymbolType::StaticVarable:
	case  SymbolType::ThreadVarable:
		return UseOutput ? IROperator(IROperatorType::DereferenceOf_IRInstruction, Output) : IROperator(IROperatorType::DereferenceOf_IRParameter, _Builder.ToID(In.Symbol->FullName));
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

	Add_SetLineNumber(In.Start->token, LookingAtIRBlock->Instructions.size());
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
		return UseOutput ? LookingAtIRBlock->NewLoadPtr(Output) : LookingAtIRBlock->NewLoadPtr(_Builder.ToID(In.Symbol->FullName));
	default:
		throw std::exception("not added");
		break;
	}
}
bool SystematicAnalysis::StepGetMemberTypeSymbolFromVar(const ScopedNameNode& node, size_t Index, ScopedName::Operator_t OpType, GetMemberTypeSymbolFromVar_t& Out)
{
	auto& Item = node.ScopedName[Index];


	auto& ItemToken = Item.token;
	LastLookedAtToken = ItemToken;


	String_view ItemTokenString;
	
	if (ItemToken->Type == TokenType::Class)
	{
		if (passtype == PassType::FixedTypes) 
		{
			VarableMemberData Data;
			ExpressionNodeType TepEx;
			ValueExpressionNode TepExValue;
			ReadVariableNode TepExName;
			ScopedName ScopeN;
			Token TepToken = *ItemToken;
			TepToken.Type = TokenType::Name;


			ScopeN.token = &TepToken;
			TepExName.VariableName.ScopedName.push_back(ScopeN);

			TepExValue.Value.reset(&TepExName);
			TepEx.Value.reset(&TepExValue);


			OnExpressionTypeNode(TepEx, GetValueMode::Read);

			auto Type = LastExpressionType;

			TypeSymbol StackCharArr;
			StackCharArr.SetType(TypesEnum::Char);



			bool IsCharArr = IsStaticCharArr(Type);


			if (IsCharArr)
			{
				auto V = Evaluate(Type, TepEx);

				{//stop double free 
					TepEx.Value.release();
					TepExValue.Value.release();
				}

				if (V.has_value())
				{
					auto BuffData = V.value();
					char* Buffer = (char*)Get_Object(BuffData);


					Data.MemberString.resize(BuffData.EvaluatedObject.ObjectSize);
					memcpy(Data.MemberString.data(), BuffData.EvaluatedObject.Object_AsPointer.get(), BuffData.EvaluatedObject.ObjectSize);

					
					VarableMemberDatas.AddValue(GetSymbolID(&Item), std::move(Data));
					ItemTokenString = VarableMemberDatas.at(GetSymbolID(&Item)).MemberString;
				}
				else
				{
					return false;
				}
			}
			else
			{

				{//stop double free 
					TepEx.Value.release();
					TepExValue.Value.release();
				}

				auto Token = Item.token;
				LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos, "Type must be a char[/] and not a '" + ToString(Type) + "' to be used as a identfier.");
				return false;
			}
		}
		else if (passtype == PassType::BuidCode)
		{
			auto Data = VarableMemberDatas.at(GetSymbolID(&Item));
			ItemTokenString = Data.MemberString;
		}

	}
	else
	{
		ItemTokenString = ItemToken->Value._String;
	}

	if (Out.Type._Type != TypesEnum::CustomType
		|| Out.Symbol == nullptr)
	{
		if (passtype == PassType::FixedTypes)
		{
			LogCantFindVarMemberError(ItemToken, ItemTokenString, Out.Type);
		}
		return false;
	}

	if (Out.Symbol->PassState == PassType::GetTypes)
	{
		Update_Sym_ToFixedTypes(Out.Symbol);
	}

	if (OpType == ScopedName::Operator_t::Null
		|| OpType == ScopedName::Operator_t::ScopeResolution
		|| OpType == ScopedName::Operator_t::Dot)
	{

		if (Out.Symbol->Type == SymbolType::Type_class
			 || Out.Symbol->Type == SymbolType::Generic_class)
		{
			ClassInfo* CInfo = Out.Symbol->Get_Info<ClassInfo>();

			auto FeldInfo = CInfo->GetField(ItemTokenString);
			if (!FeldInfo.has_value())
			{
				if (passtype == PassType::FixedTypes)
				{
					LogCantFindVarMemberError(ItemToken, ItemTokenString, Out.Type);
				}
				Out.Symbol = nullptr;
				Out.Type = TypesEnum::Null;
				return false;
			}

			auto FeldFullName = Out.Symbol->FullName;
			ScopeHelper::GetApendedString(FeldFullName, ItemTokenString);
			auto FeldSyb = GetSymbol(FeldFullName, SymbolType::Class_Field);
			{
				AccessCheck(FeldSyb, ItemToken);
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

				if (Index + 1 < node.ScopedName.size())
				{
					const UCodeLang::Token* Token = node.ScopedName.begin()->token;

					auto Token2 = node.ScopedName[Index + 1].token;
					auto& Str2 = Token->Value._String;
					if (passtype == PassType::FixedTypes)
					{
						LogCantFindVarMemberError(Token2, Str2, Out.Type);
					}

					Out.Symbol = nullptr;
					Out.Type = TypesEnum::Null;
					return false;
				}
			}
			if (passtype == PassType::BuidCode)
			{
				AddDependencyToCurrentFile(Out.Symbol);
			}
		}
		else if (Out.Symbol->Type == SymbolType::Enum)
		{
			{
				const UCodeLang::Token* Token = node.ScopedName.begin()->token;
				AccessCheck(Out.Symbol, Token);
			}

			EnumInfo* Einfo = Out.Symbol->Get_Info<EnumInfo>();
			auto& NameString = ItemTokenString;

			auto FeldInfo = Einfo->GetFieldIndex(NameString);
			if (!FeldInfo.has_value())
			{
				if (passtype == PassType::FixedTypes)
				{
					LogCantFindVarMemberError(ItemToken, ItemTokenString, Out.Type);
				}

				Out.Symbol = nullptr;
				Out.Type = TypesEnum::Null;
				return false;
			}
			else if (Einfo->VariantData)
			{
				auto& Item = Einfo->VariantData.value().Variants[FeldInfo.value()];

				bool IsOk = Item.Types.size() == 0;

				if (!IsOk)
				{

					LogMustMakeEnumLikeafuncion(Einfo, FeldInfo.value(), ItemToken);


					Out.Symbol = nullptr;
					Out.Type = TypesEnum::Null;
					return false;
				}
			}

			if (Index + 1 < node.ScopedName.size())
			{
				const UCodeLang::Token* Token = node.ScopedName.begin()->token;

				auto Token2 = node.ScopedName[Index + 1].token;
				auto& Str2 = Token->Value._String;
				if (passtype == PassType::FixedTypes)
				{
					LogCantFindVarMemberError(Token2, Str2, Out.Type);
				}

				Out.Symbol = nullptr;
				Out.Type = TypesEnum::Null;
				return false;
			}


			Out.Type.SetType(Out.Symbol->ID);//set enum type

			{
				String FeildSymFullName = Out.Symbol->FullName;
				ScopeHelper::GetApendedString(FeildSymFullName, NameString);

				Symbol* FeildSym = GetSymbol(FeildSymFullName, SymbolType::Enum_Field);

				Out.Symbol = FeildSym;//set symbol as enum feild


				Out.Set_V1(&Einfo->Fields[*FeldInfo]);
				Out.Set_V2(Einfo);

			}

			if (passtype == PassType::BuidCode)
			{
				AddDependencyToCurrentFile(Out.Symbol);
			}
		}
		else if (Out.Symbol->Type == SymbolType::Func)
		{
			if (Index + 1 < node.ScopedName.size())
			{
				const UCodeLang::Token* Token = node.ScopedName.begin()->token;

				auto Token2 = node.ScopedName[Index + 1].token;
				auto& Str2 = Token->Value._String;
				if (passtype == PassType::FixedTypes)
				{
					LogCantFindVarMemberError(Token2, Str2, Out.Type);
				}

				Out.Symbol = nullptr;
				Out.Type = TypesEnum::Null;
				return false;
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
				
				if (Out.Symbol->Type == SymbolType::Class_Field)
				{
					const UCodeLang::Token* token = node.ScopedName.begin()->token;
					

					AccessCheck(Out.Symbol, token);
				}

				if (TypeAsSybol->Type != SymbolType::Type_class)
				{
					LogCantFindVarMemberError(ItemToken, ItemTokenString, Out.Type);

					Out.Symbol = nullptr;
					Out.Type = TypesEnum::Null;
					return false;
				}
				ClassInfo* CInfo = TypeAsSybol->Get_Info<ClassInfo>();


				auto FeldInfo = CInfo->GetField(ItemTokenString);
				if (!FeldInfo.has_value())
				{
					if (passtype == PassType::FixedTypes)
					{
						LogCantFindVarMemberError(ItemToken, ItemTokenString, Out.Type);
					}

					Out.Symbol = nullptr;
					Out.Type = TypesEnum::Null;
					return false;
				}

				{
					auto FeldFullName = TypeAsSybol->FullName;
					ScopeHelper::GetApendedString(FeldFullName, ItemTokenString);
					auto FeldSyb = GetSymbol(FeldFullName, SymbolType::Class_Field);
					{
						AccessCheck(FeldSyb, ItemToken);
					}
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

					if (Index + 1 < node.ScopedName.size())
					{
						const UCodeLang::Token* Token = node.ScopedName.begin()->token;

						auto Token2 = node.ScopedName[Index + 1].token;
						auto& Str2 = Token->Value._String;
						if (passtype == PassType::FixedTypes)
						{
							LogCantFindVarMemberError(Token2, Str2, Out.Type);
						}

						Out.Symbol = nullptr;
						Out.Type = TypesEnum::Null;
						return false;
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
				if (Index + 1 < node.ScopedName.size())
				{
					const UCodeLang::Token* Token = node.ScopedName.begin()->token;

					auto Token2 = node.ScopedName[Index + 1].token;
					auto& Str2 = Token->Value._String;
					if (passtype == PassType::FixedTypes)
					{
						LogCantFindVarMemberError(Token2, Str2, VarableType);
					}

					Out.Symbol = nullptr;
					Out.Type = TypesEnum::Null;
					return false;
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

	return true;
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

		Add_SetLineNumber(In.Start->token, LookingAtIRBlock->Instructions.size());

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
			return UseOutput ? LookingAtIRBlock->NewLoad(Output) : LookingAtIRBlock->NewLoad_IRID(_Builder.ToID(In.Symbol->FullName));
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
	case  SymbolType::ConstantExpression:
	{
		auto Item = In.Symbol->Get_Info<ConstantExpressionInfo>();
		return LoadEvaluatedEx(Item->Ex, In.Symbol->VarType);
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

	IRInstruction* Output = nullptr;

	BuildMember_Access(In, Output);
	if (Output == nullptr)
	{
		Output = In.Symbol->IR_Ins;
	}
	bool UseOutput = In.Symbol->IR_Ins != Output;

	Add_SetLineNumber(In.Start->token, LookingAtIRBlock->Instructions.size());

	switch (In.Symbol->Type)
	{
	case  SymbolType::Class_Field:
	case  SymbolType::StackVarable:
		return UseOutput ? LookingAtIRBlock->NewLoad_Dereferenc(Output,IRT) : LookingAtIRBlock->NewLoad_Dereferenc(In.Symbol->IR_Ins, IRT);
		break;
	case  SymbolType::ParameterVarable:
		return UseOutput ? LookingAtIRBlock->NewLoad_Dereferenc(Output,IRT) : LookingAtIRBlock->NewLoad_Dereferenc(In.Symbol->IR_Par, IRT);
	default:
		throw std::exception("not added");
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

		StepBuildMember_Access(ITem, Last_Type, OpType, In, Output);
	}

}

void SystematicAnalysis::StepBuildMember_Access(const ScopedName& ITem, TypeSymbol& Last_Type, ScopedName::Operator_t OpType, const GetMemberTypeSymbolFromVar_t& In, IRInstruction*& Output)
{
	Symbol* Sym = GetSymbol(Last_Type);

	Add_SetLineNumber(ITem.token, LookingAtIRBlock->Instructions.size());

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

	String MemberName;
	if (ITem.token->Type == TokenType::Class)
	{
		MemberName = VarableMemberDatas.at(GetSymbolID(&ITem)).MemberString;
	}
	else
	{
		MemberName = ITem.token->Value._String;
	}


	switch (Sym->Type)
	{
	case  SymbolType::Type_class:
	{
		auto* Classinfo = Sym->Get_Info<ClassInfo>();
		size_t MemberIndex = Classinfo->GetFieldIndex(MemberName).value();
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
			case SymbolType::ThreadVarable:
			case SymbolType::StaticVarable:
			{
				TypeSymbol& TypeSys = Last_Type;
				auto id = _Builder.ToID(In.Symbol->FullName);
				if (TypeSys.IsAddress())
				{
					Output = LookingAtIRBlock->New_Member_Dereference(id,ConvertToIR(Sym->VarType), MemberIndex);
				}
				else
				{
					Output = LookingAtIRBlock->New_Member_Access(id, IRstruct, MemberIndex);
				}

			}
			break;
			default:
				throw std::exception("not added");
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

Symbol* SystematicAnalysis::GetTepFuncPtrSyb(const String& TepFuncPtr, const FuncInfo* Finfo)
{
	Symbol* V =GetSymbol(TepFuncPtr, SymbolType::Func_ptr);
	if (V == nullptr)
	{
		V = &AddSybol(SymbolType::Func_ptr, TepFuncPtr, Finfo->FullName,AccessModifierType::Public);
		FuncPtrInfo* V2 = new FuncPtrInfo();
		V->Info.reset(V2);

		SymbolID VID = GetSymbolID(V);

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


	size_t ScopedCount = 0;
	if (Out.Symbol == nullptr && Out.Type.IsBadType())
	{


		if (node.ScopedName[Start].token->Type != TokenType::KeyWord_This)
		{
			auto ScopeName = node.ScopedName[Start];
			auto Token = ScopeName.token;
			auto& Str = Token->Value._String;
			auto SymbolVar = GetSymbol(Str, SymbolType::Varable_t);
			LastLookedAtToken = Token;


			if (SymbolVar == nullptr)
			{
				LogCantFindVarError(Token, Str);
				return false;
			}

			if (SymbolVar->PassState == PassType::GetTypes)
			{
				Update_Sym_ToFixedTypes(SymbolVar);
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
			Out.Symbol = SymbolVar;

			if (SymbolVar->Type == SymbolType::ParameterVarable)
			{
				//because overloaded funcions have the same Symbol FullNames we need to pick the this one and not the first one/  
				FuncInfo* Func = _FuncStack.back().Pointer;
				//throw std::exception("bad");
			}

			if (SymbolVar->Type == SymbolType::Class_Field)
			{
				if (!IsInThisFuncCall())
				{
					LogCantUseThisInStaticFunction(Token);
					Out.Symbol = nullptr;
					Out.Type = TypeSymbol();
					return false;
				}
			}

			if (ScopeName.Generic.get() && ScopeName.Generic->Values.size())
			{
				TypeNode Tep;
				Tep.Name.Token = ScopeName.token;
				auto& Other = *ScopeName.Generic;


				{//cant copy TypeNode but we need anyway.
					Tep.Generic.Values.resize(Other.Values.size());
					memcpy(Tep.Generic.Values.data(), Other.Values.data(), sizeof(TypeNode) * Other.Values.size());

				}

				TypeSymbol Type;
				Convert(Tep, Type);
				if (Type._Type != TypesEnum::Null)
				{
					Out.Type = Type;
					Out.Symbol = GetSymbol(Type);
				}
				
				{// TypeNode has Unique_ptr we do this to not free it.
					new (Tep.Generic.Values.data()) TypeNode[Other.Values.size()];
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
				CantUseThisKeyWordHereError(node.ScopedName[Start].token);
				return false;
			}


			if (!IsInThisFuncCall())
			{
				LogCantUseThisInStaticFunction(node.ScopedName[Start].token);
				return false;
			}



			auto& Func = _FuncStack.back().Pointer;
			auto ObjectType = Func->GetObjectForCall();

			auto objecttypesyb = GetSymbol(*ObjectType);
			ClassInfo* V = objecttypesyb->Get_Info<ClassInfo>();

			Out.Type = *Func->GetObjectForCall();
			Out.Symbol = GetSymbol(*ObjectType);
			//
			Start++;
			End--;
			ScopedCount++;
		}
	}


	bool BadValue = false;
	for (size_t i = Start; i < node.ScopedName.size(); i++)
	{
		if (i > End) { break; }
		ScopedCount++;

		ScopedName::Operator_t OpType = i == 0 ? ScopedName::Operator_t::Null : node.ScopedName[i - 1].Operator;
		if (StepGetMemberTypeSymbolFromVar(node, i, OpType, Out) == false)
		{
			BadValue = true;
			continue;
		}
	}
	if (BadValue == false)
	{
		Out.Start = &node.ScopedName[Start - 1];
		Out.End = ScopedCount;
		if (End == -1)
		{
			Out.End++;//i dont know why this needed its most likely its us useing two diff funcs for the same thing to get the start off syb.
		}

		{
			auto OldTepSyb = Out.Symbol;
			auto ConstExCheck = Out.Symbol;
			while (ConstExCheck &&
				(ConstExCheck->Type == SymbolType::Type_alias || ConstExCheck->Type == SymbolType::Hard_Type_alias))
			{
				ConstExCheck = GetSymbol(ConstExCheck->VarType);
			}
			if (ConstExCheck == nullptr)
			{
				Out.Symbol = OldTepSyb;

			}
			else if (ConstExCheck->Type == SymbolType::ConstantExpression)
			{
				ConstantExpressionInfo* ConstInfo = ConstExCheck->Get_Info<ConstantExpressionInfo>();

				Out.Type = ConstExCheck->VarType;
				Out.Symbol = ConstExCheck;
			}
		}

		if (!(Out.Symbol->Type == SymbolType::Class_Field
			|| Out.Symbol->Type == SymbolType::Enum_Field
			|| Out.Symbol->Type == SymbolType::Func
			|| Out.Symbol->Type == SymbolType::ConstantExpression
			|| IsVarableType(Out.Symbol->Type)))
		{

			if (passtype == PassType::FixedTypes)
			{
				auto& Item = node.ScopedName.back().token;
				LogWantedAVariable(Item, Out.Symbol);
			}
			Out.Type.SetType(TypesEnum::Null);
			Out.Symbol = nullptr;

			return false;
		}

		if (IsWrite(Mod) && !(Out.Symbol->Type == SymbolType::Class_Field || IsVarableType(Out.Symbol->Type)))
		{
			auto& Item = node.ScopedName.back().token;
			LogError(ErrorCodes::InValidType, Item->OnLine, Item->OnPos, "You Cant Write to a " + ToString(Out.Symbol->Type));
		}



		if (_Varable.size())
		{
			auto& Data = _Varable.top();
			Data._UsedSymbols.push_back(Out.Symbol);
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
			t.Op0 = f->Pars[0].Type;
		}
		else
		{
			t.Op0 = LastExpressionType;
		}

		PostFix_Datas.AddValue(GetSymbolID(node),t);
	}
	else
	if (passtype == PassType::BuidCode)
	{
		const PostFixExpressionNode_Data& Data = PostFix_Datas.at(GetSymbolID(node));

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


			Add_SetLineNumber(node.PostfixOp, LookingAtIRBlock->Instructions.size());

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

		Compound_Datas.AddValue(GetSymbolID(node), r);
	}
	else if (passtype == PassType::BuidCode)
	{
		const auto& Data = Compound_Datas.at(GetSymbolID(node));

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
	case NodeType::ExtendedScopeExpression:OnExpressionNode(*ExtendedScopeExpression::As(node)); break;
	case NodeType::ExtendedFuncExpression:OnExpressionNode(*ExtendedFuncExpression::As(node)); break;
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
		const auto Value = node.Value.get();
		switch (node.Value->Get_Type())
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

			OnExpressionTypeNode(nod->Expression.Value.get(), GetValueMode::Read);
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
			OnCMPTypesNode(*nod);
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
		default:
			throw std::exception("not added");
			break;
		}
	}
}

bool SystematicAnalysis::CMPGetValue(const TypeSymbol& Type0, const TypeSymbol& Type1,const Token* Value)
{
	bool CMPValue = false;
	switch (Value->Type)
	{
	case TokenType::equal_Comparison:
		CMPValue = AreTheSame(Type0, Type1);
		break;
	case TokenType::Notequal_Comparison:
		CMPValue = !AreTheSame(Type0, Type1);
		break;
	case TokenType::approximate_Comparison:
		CMPValue = AreTheSameWithOutimmutable(Type0, Type1);
		break;
	default:
		break;
	}
	return CMPValue;
}

void SystematicAnalysis::OnCMPTypesNode(const CMPTypesNode& node)
{
	if (passtype == PassType::GetTypes)
	{

	}
	if (passtype == PassType::FixedTypes)
	{
		TypeSymbol Type0 = ConvertAndValidateType(node.TypeOp0, NodeSyb_t::Any);
		TypeSymbol Type1 = ConvertAndValidateType(node.TypeOp1, NodeSyb_t::Any);


		bool CMPValue =CMPGetValue(Type0,Type1,node.Op);


		ValidNodes.AddValue(GetSymbolID(node), CMPValue);
	}
	if (passtype == PassType::BuidCode)
	{
		_LastExpressionField = LookingAtIRBlock->NewLoad(ValidNodes[GetSymbolID(node)]);
	}

	LastExpressionType.SetType(TypesEnum::Bool);
}

void SystematicAnalysis::OnMovedNode(const MoveNode* nod)
{

	OnExpressionTypeNode(nod->expression.Value.get(), GetValueMode::Read);
	
	auto ExType = LastExpressionType;
	ExType.SetAsMoved();
	LastExpressionType = ExType;
}

void SystematicAnalysis::OnNumberliteralNode(const NumberliteralNode* num)
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

		Add_SetLineNumber(num->Token, LookingAtIRBlock->Instructions.size());

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

void SystematicAnalysis::OnBoolliteralNode(const BoolliteralNode* num)
{
	if (passtype == PassType::BuidCode)
	{
		Add_SetLineNumber(num->Token, LookingAtIRBlock->Instructions.size());
		_LastExpressionField = LookingAtIRBlock->NewLoad(num->Get_Value());
	}
	LastExpressionType.SetType(TypesEnum::Bool);
	LastLookedAtToken = num->Token;
}

void SystematicAnalysis::OnCharliteralNode(const CharliteralNode* num)
{
	if (passtype == PassType::BuidCode)
	{
		String V;
		bool ItWorked = !ParseHelper::ParseCharliteralToChar(num->Token->Value._String, V);

		Add_SetLineNumber(num->Token, LookingAtIRBlock->Instructions.size());


		_LastExpressionField = LookingAtIRBlock->NewLoad((char)V.front());
	}
	LastExpressionType.SetType(TypesEnum::Char);
	LastLookedAtToken = num->Token;
}

void SystematicAnalysis::OnFloatLiteralNode(const FloatliteralNode* num)
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

			Add_SetLineNumber(num->Token, LookingAtIRBlock->Instructions.size());
			_LastExpressionField = LookingAtIRBlock->NewLoad(V);
			break;
		}
		case TypesEnum::float64:
		{
			float64 V;
			bool ItWorked = ParseHelper::ParseStringTofloat64(num->Token->Value._String, V);

			Add_SetLineNumber(num->Token, LookingAtIRBlock->Instructions.size());
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

void SystematicAnalysis::OnStringLiteral(const StringliteralNode* nod, bool& retflag)
{
	retflag = true;
	if (passtype == PassType::GetTypes)
	{//check for ok string

	}


	auto& Type = Get_LookingForType();

	bool IsStaticArr = IsStaticCharArr(Type);



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


			Add_SetLineNumber(nod->Token, LookingAtIRBlock->Instructions.size());

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

bool SystematicAnalysis::IsStaticCharArr(const TypeSymbol& Type)
{
	if (Type._Type == TypesEnum::CustomType)
	{
		auto V = GetSymbol(Type);
		if (V->Type == SymbolType::Type_StaticArray)
		{
			StaticArrayInfo* StaticArr = V->Get_Info< StaticArrayInfo>();

			TypeSymbol CharType;
			CharType.SetType(TypesEnum::Char);

			return AreTheSame(CharType, StaticArr->Type);
		}
	}
	return false;
}

void SystematicAnalysis::OnSizeofNode(const SizeofExpresionNode* nod)
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


void SystematicAnalysis::OnNewNode(const NewExpresionNode* nod)
{
	if (passtype == PassType::FixedTypes)
	{
		if (_RemoveUnSafeArgWasPassed)
		{
			auto Token = nod->KeywordToken;
			LogError(ErrorCodes::ExpectingSequence, Token->OnLine, Token->OnPos, "Cant use 'new' keyword in safe mode.");
			return;
		}
	}

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
		FuncToSyboID[GetSymbolID(nod)] = Func;



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
	

		auto Func = FuncToSyboID.at(GetSymbolID(nod));
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

void SystematicAnalysis::OnAnonymousObjectConstructor(const AnonymousObjectConstructorNode* nod)
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
			FuncToSyboID[GetSymbolID(nod)] = Func;

			SetFuncRetAsLastEx(Func);
			return;
		}
		else if (passtype == PassType::BuidCode)
		{
			const auto& Func = FuncToSyboID.at(GetSymbolID(nod));
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
				SymbolID id = GetSymbolID(nod);
				auto FullName = CompilerGenerated("StaticArray_") + ToString(ArrItemType) + std::to_string(id.AsInt());


				auto Syb = &AddSybol(SymbolType::Type_StaticArray, FullName, FullName,AccessModifierType::Public);
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

void SystematicAnalysis::DoFuncCall(const TypeSymbol& Type, const Get_FuncInfo& Func, const ValueParametersNode& ValuePars)
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
		auto Info = Symbol->Get_Info<DeclareVariableInfo>();
		
		if (Info->LineNumber > FToken->OnPos)
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
				bool DoStore = true;
				if (V.Symbol->Type == SymbolType::ParameterVarable)
				{
					auto ParInfo = V.Symbol->Get_Info<ParameterInfo>();
					if (ParInfo->IsOutValue)
					{
						DoStore = false;
						_LastStoreField = BuildMember_DereferencStore(V);
					}
				}
				

				if (DoStore) {
					_LastStoreField = BuildMember_Store(V, V.Type);
				}
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
		Data = &BinaryExpressionNode_Datas.at(GetSymbolID(node));
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
			V.Op0 = f->Pars[0].Type;
			V.Op1 = f->Pars[1].Type;
			V.FuncToCall = Info.Value.value();
		
			LastExpressionType = f->Ret;
		}
		else
		{
			V.Op0._IsAddress = false;
			V.Op1._IsAddress = false;
			LastExpressionType = BinaryExpressionShouldRurn(Op, Ex0Type);
		}

		BinaryExpressionNode_Datas.AddValue(GetSymbolID(node),V);

		
	}


	BuildCodePart:
	if (passtype == PassType::BuidCode)
	{
		auto& Data = BinaryExpressionNode_Datas.at(GetSymbolID(node));

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

			auto Type = Ex0Type;
			auto Op = node.BinaryOp->Type;
			LastExpressionType = BinaryExpressionShouldRurn(Op, Ex0Type);

			auto TypeSyb = GetSymbol(Type);
			if (TypeSyb)
			{
				if (TypeSyb->Type == SymbolType::Enum)
				{
					Type = TypeSyb->Get_Info<EnumInfo>()->Basetype;
				}
				else
				{
					throw std::exception("bad path");
				}
			}

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

			case TypesEnum::uIntPtr:BindaryBuildU(64); break;
			case TypesEnum::sIntPtr:BindaryBuildS(64); break;

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
				throw std::exception("not added");
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
		auto& Item =CastDatas.at(GetSymbolID(node));
		if (Item.FuncToCall) 
		{
			LookingForTypes.push(Item.FuncToCall->Get_Info<FuncInfo>()->Pars[0].Type);

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
		if (!ToTypeAs.IsNull()) 
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


			CastDatas.AddValue(GetSymbolID(node), data);
		}
	}

	if (passtype == PassType::BuidCode)
	{
		DoExplicitlConversion(_LastExpressionField, LastExpressionType, ToTypeAs,CastDatas.at(GetSymbolID(node)));

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

			V.Op0 = f->Pars[0].Type;
			V.Op1 = f->Pars[1].Type;
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

			

		IndexedExpresion_Datas.AddValue(GetSymbolID(node),V);

	}


	if (passtype == PassType::BuidCode)
	{
		auto& Data = IndexedExpresion_Datas.at(GetSymbolID(node));

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
		if (!FuncToSyboID.HasValue(GetSymbolID(node))) 
		{

			auto Info = GetFunc(node.FuncName, node.Parameters, Get_LookingForType());

			if (Info.SymFunc)
			{
				AddDependencyToCurrentFile(Info.SymFunc);
			}

			FuncToSyboID.AddValue(GetSymbolID(node), std::move(Info));

			SetFuncRetAsLastEx(Info);
		}
	}
	else if (passtype == PassType::BuidCode)
	{
		auto& SybID = FuncToSyboID.at(GetSymbolID(node));
		DoFuncCall(SybID, node.FuncName, node.Parameters);
	}
}
void SystematicAnalysis::SetFuncRetAsLastEx(const Get_FuncInfo& Info)
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
	else if (Info._BuiltFunc.has_value())
	{
		auto& Item = Info._BuiltFunc.value();
		if (Item.EvalAsCString)
		{
			LastExpressionType = TypesEnum::Char;
			LastExpressionType.SetAsAddressArray();
			LastExpressionType.SetAsimmutable();
		}
		else
		{
			LastExpressionType = Info._BuiltFunc.value().RetType;
		}
	}
	else if (Info.CantCheckBecauseIsUnMaped)
	{
		LastExpressionType = GetUnMapType();
	}
}
void SystematicAnalysis::OnDropStatementNode(const DropStatementNode& node)
{
	if (passtype == PassType::GetTypes)
	{
		if (_RemoveUnSafeArgWasPassed)
		{
			auto Token = node.KeywordToken;
			LogError(ErrorCodes::ExpectingSequence, Token->OnLine, Token->OnPos, "Cant use 'drop' keyword in safe mode.");
		}
	}

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
void SystematicAnalysis::ConvertAttributes(const Vector<Unique_ptr<AttributeNode>>& nodes, Vector<UsedTagValueData>& Out)
{
	Out.resize(nodes.size());
	for (size_t i = 0; i < nodes.size(); i++)
	{
		ConvertAttribute(*nodes[i], Out[i]);
	}
}
void SystematicAnalysis::ConvertAttribute(const AttributeNode& nodes, UsedTagValueData Out)
{
	auto Syb = GetSymbol(GetSymbolID(nodes));
	Out.TypeID = GetTypeID(TypesEnum::CustomType,Syb->VarType._CustomTypeSymbol);
}
void SystematicAnalysis::LoadLibSymbols()
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
			this->LookingAtFile = &FileNode;

			if (LoadLibMode::GetTypes == Mode)
			{
				for (auto& LibNode : Item->_Assembly.Classes)
				{
					_Lib._Assembly.Classes.push_back(Unique_ptr<AssemblyNode>(LibNode.get()));//make ref
				}
			}

			LoadLibSymbols(*Item,Mode);
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
			throw std::exception("Bad Path");
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
	for (auto& Item : _TypesToFix)
	{
		*Item.TypeToFix = *Item.ToGetTypeFrom;
	}
	_TypesToFix.clear();//free Mem.


	auto globalAssemblyObjectName = (String_view)ScopeHelper::_globalAssemblyObject;
	_Lib.Get_Assembly().AddClass(String(globalAssemblyObjectName), String(globalAssemblyObjectName));

}
void SystematicAnalysis::LoadLibSymbols(const UClib& lib, LoadLibMode Mode)
{
	
	auto OutputType = OutputTypeAsLibType();
	auto libType = lib._LibType;

	auto GlobalObject = lib.Get_Assembly().Get_GlobalObject_Class();
	if (GlobalObject)
	{
		String Scope;
		LoadClassSymbol(*GlobalObject,Scope, Scope, Mode);
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
			LoadClassSymbol(Item->Get_ClassData(),FullName, Scope, Mode);
		}
		break;
		case ClassType::Alias:
		{

		}
		break;
		case ClassType::Enum:
		{
			LoadEnumSymbol(Item->Get_EnumData(), FullName, Scope, Mode);
		}
		break;
		default:
			break;
		}
	}

}
void SystematicAnalysis::LoadClassSymbol(const Class_Data& Item, const String& FullName, const String& Scope, SystematicAnalysis::LoadLibMode Mode)
{
	auto TepScope = std::move(_Table._Scope);

	_Table._Scope = {};
	_Table._Scope.ThisScope = Scope;

	if (Mode == LoadLibMode::GetTypes)
	{
		auto Name = ScopeHelper::GetNameFromFullName(FullName);
		auto& Syb = AddSybol(SymbolType::Type_class, Name, FullName, AccessModifierType::Public);
		_Table.AddSymbolID(Syb, GetSymbolID(&Item));

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
	 	auto& Syb = _Table.GetSymbol(GetSymbolID(&Item));
		ClassInfo* Info = Syb.Get_Info<ClassInfo>();

		for (size_t i = 0; i < Item.Fields.size(); i++)
		{
			const auto& FieldItem = Item.Fields[i];
			auto& InfoItem = Info->Fields[i];
			LoadType(FieldItem.Type, InfoItem.Type);
		}
	}

	for (auto& Item : Item.Methods)
	{
		LoadSymbol(Item, Mode);
	}

	_Table._Scope = std::move(TepScope);
}
void SystematicAnalysis::LoadEnumSymbol(const Enum_Data& Item, const String& FullName, const String& Scope, SystematicAnalysis::LoadLibMode Mode)
{
	auto TepScope = std::move(_Table._Scope);

	_Table._Scope = {};
	_Table._Scope.ThisScope = Scope;

	if (Mode == LoadLibMode::GetTypes)
	{
		auto Name = ScopeHelper::GetNameFromFullName(FullName);
		auto& Syb = AddSybol(SymbolType::Enum, Name, FullName, AccessModifierType::Public);
		_Table.AddSymbolID(Syb, GetSymbolID(&Item));

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
				auto& FieldSyb = AddSybol(SymbolType::Enum_Field, ValueItem.Name,ScopeHelper::ApendedStrings(FullName,ValueItem.Name), AccessModifierType::Public);
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
		auto& Syb = _Table.GetSymbol(GetSymbolID(&Item));
		auto  enumInfo = Syb.Get_Info<EnumInfo>();

		LoadType(Item.BaseType,enumInfo->Basetype);
		
		
		
		if (Item.EnumVariantUnion.has_value())
		{
			EnumVariantData& Data = enumInfo->VariantData.value();
			for (size_t i = 0; i < Item.Values.size(); i++)
			{
				auto& VariantItem = Data.Variants[i];
				const auto& ValueItem = Item.Values[i];

				if (ValueItem.EnumVariantType.has_value())
				{
					auto Type = LoadType(ValueItem.EnumVariantType.value());
					Symbol* Sym = GetSymbol(Type);
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
									_TypesToFix.push_back(Seter);
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
void SystematicAnalysis::LoadSymbol(const ClassMethod& Item, SystematicAnalysis::LoadLibMode Mode)
{
	if (Mode == LoadLibMode::GetTypes)
	{	
		auto Name =ScopeHelper::GetNameFromFullName(Item.FullName);
		auto& Syb = AddSybol(SymbolType::Func, Name, _Table._Scope.GetApendedString(Name), AccessModifierType::Public);
		_Table.AddSymbolID(Syb, GetSymbolID(&Item));
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
		auto& Syb = _Table.GetSymbol(GetSymbolID(&Item));
		auto Funcinfo = Syb.Get_Info<FuncInfo>();

		LoadType(Item.RetType, Funcinfo->Ret);
		Syb.VarType = Funcinfo->Ret;

		for (size_t i = 0; i < Funcinfo->Pars.size(); i++)
		{
			const ClassMethod::Par& ItemPar = Item.ParsType[i];
			Funcinfo->Pars[i].IsOutPar = ItemPar.IsOutPar;
			LoadType(ItemPar.Type, Funcinfo->Pars[i].Type);
		}
	}
}
Symbol* SystematicAnalysis::GetSymbol(String_view Name, SymbolType Type)
{
	auto& Symbols = _Table.GetSymbolsWithName(Name,Type);
	auto Symbol = Symbols.size() ? Symbols[0] : nullptr;
	return Symbol;
}
void SystematicAnalysis::OnInvalidNode(const InvalidNode& node)
{
	if (passtype == PassType::GetTypes){return;}

	if (node._StringExpression.Value)
	{
		auto StrData = GetStrEVal(node._StringExpression.Value.get());

		if (StrData.IsConstantExpression) 
		{
			auto Token = node.KeyWord;
			LogInvalidNodeError(Token, StrData.Msg);
		}
		else
		{

			TypeSymbol B = TypesEnum::Char;
			B.SetAsAddressArray();

			LogCantCastImplicitTypes_Constant(LastLookedAtToken, LastExpressionType,B);
		}
	}
	else
	{
		auto Token = node.KeyWord;
		LogEmptyInvalidError(Token);
	}
}
void SystematicAnalysis::OnvalidNode(const ValidNode& node)
{
	if (passtype == PassType::GetTypes) 
	{

		if (node.IsExpression)
		{
			OnExpressionTypeNode(node._ExpressionToCheck.Value.get(), GetValueMode::Read);
		}
		else
		{
			for (auto& Item : node._StatementToCheck._Nodes)
			{
				OnStatement(*Item);
			}
		}
		LastExpressionType = TypesEnum::Bool;
	}
	if (passtype == PassType::FixedTypes)
	{
		bool IsValid = true;
	 	auto ErrCount = _ErrorsOutput->Get_Errors().size();

		if (node.IsExpression)
		{
			OnExpressionTypeNode(node._ExpressionToCheck.Value.get(), GetValueMode::Read);
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

	
		LastExpressionType = TypesEnum::Bool;

		ValidNodes.AddValue(GetSymbolID(node), IsValid);
	}
	if (passtype == PassType::BuidCode)
	{
		bool IsValid = ValidNodes.at(GetSymbolID(node));


		_LastExpressionField = LookingAtIRBlock->NewLoad(IsValid);
		LastExpressionType = TypesEnum::Bool;
	}
}
void SystematicAnalysis::OnExpressionNode(const ExtendedScopeExpression& node)
{
	if (passtype == PassType::GetTypes)
	{
		LookingForTypes.push(TypesEnum::Any);
		OnExpressionTypeNode(node.Expression.Value.get(), GetValueMode::Read);
		LookingForTypes.pop();
	}
	if (passtype == PassType::FixedTypes)
	{
		LookingForTypes.push(TypesEnum::Any);
		OnExpressionTypeNode(node.Expression.Value.get(), GetValueMode::Read);
		LookingForTypes.pop();

		auto ExpressionType = LastExpressionType;
	
		GetMemberTypeSymbolFromVar_t V;
		

		GetExpressionMode.push(GetExpressionMode.top());
		{
			V.Type = ExpressionType;
			V.Symbol = GetSymbol(ExpressionType);

		
			StepGetMemberTypeSymbolFromVar(node.Extended, 0, node.Operator, V);
		
			for (size_t i = 1; i < node.Extended.ScopedName.size(); i++)
			{
				StepGetMemberTypeSymbolFromVar(node.Extended, i, node.Extended.ScopedName[i].Operator, V);
			}
		}
		GetExpressionMode.pop();

		LastExpressionType = V.Type;
	}
	if (passtype == PassType::BuidCode)
	{
		LookingForTypes.push(TypesEnum::Any);
		OnExpressionTypeNode(node.Expression.Value.get(), GetValueMode::Read);
		LookingForTypes.pop();


		auto ExpressionType = LastExpressionType;
		auto ExpressionIR = _LastExpressionField;

		GetMemberTypeSymbolFromVar_t V;
		V.Start = node.Extended.ScopedName.data();
		V.End = node.Extended.ScopedName.size();

		GetExpressionMode.push(GetExpressionMode.top());
		{
			V.Type = ExpressionType;
			V.Symbol = GetSymbol(ExpressionType);


			StepGetMemberTypeSymbolFromVar(node.Extended, 0, node.Operator, V);

			for (size_t i = 1; i < node.Extended.ScopedName.size(); i++)
			{
				StepGetMemberTypeSymbolFromVar(node.Extended, i, node.Extended.ScopedName[i].Operator, V);
			}
		}
		GetExpressionMode.pop();

		auto GetValue = GetExpressionMode.top();

		IRInstruction* VPtr= ExpressionIR;


		
		StepBuildMember_Access(node.Extended.ScopedName[0], ExpressionType, node.Operator, V, VPtr);
		for (size_t i = 1; i < node.Extended.ScopedName.size(); i++)
		{
			StepBuildMember_Access(node.Extended.ScopedName[i], ExpressionType, node.Operator, V, VPtr);
		}

		if (IsRead(GetValue)) 
		{
			_LastExpressionField = VPtr;
		}
		
		if (IsWrite(GetValue))
		{
			_LastStoreField = VPtr;
		}


		LastExpressionType = V.Type;
	}
}
void SystematicAnalysis::OnExpressionNode(const ExtendedFuncExpression& node)
{
	if (passtype == PassType::GetTypes)
	{
		LookingForTypes.push(TypesEnum::Any);
		OnExpressionTypeNode(node.Expression.Value.get(), GetValueMode::Read);
		LookingForTypes.pop();
	}
	if (passtype == PassType::FixedTypes)
	{
		LookingForTypes.push(TypesEnum::Any);
		OnExpressionTypeNode(node.Expression.Value.get(), GetValueMode::Read);
		LookingForTypes.pop();

		auto ExpressionType = LastExpressionType;

		GetExpressionMode.push(GetExpressionMode.top());
		{

			TypeSymbol TypeToStart = ExtendedFuncExpressionGetTypeToStart(ExpressionType, node);

			//Boring boiler plate for Tep ScopedNameNode should be the same in  Evaluate(EvaluatedEx& Out, const ExtendedFuncExpression& node)
			const Token& ToGetLinesFrom = *node.Extended.FuncName.ScopedName.begin()->token;

			ScopedNameNode Name;
			
			ScopedName TepV;
			

			Token TepToken; 

			TepToken.OnLine = ToGetLinesFrom.OnLine;
			TepToken.OnPos = ToGetLinesFrom.OnPos;
			TepToken.Type = TokenType::Name;

			RemoveTypeattributes(TypeToStart);

			String Buffer= ToString(TypeToStart);

			TepToken.Value._String= Buffer;
			
			TepV.token = &TepToken;
			

			TepV.Operator = ScopedName::Operator_t::ScopeResolution;
			Name.ScopedName.push_back(std::move(TepV));
			


			
			{
			
				{
					auto Copy = node.Extended.FuncName.ScopedName.back();
					Name.ScopedName.push_back(std::move(Copy));
				}
				for (size_t i = 1; i < node.Extended.FuncName.ScopedName.size(); i++)
				{
					auto& Item = node.Extended.FuncName.ScopedName[i];
					auto Copy = Item;
					Name.ScopedName.push_back(std::move(Copy));
				}
			}
			//

			ValueParametersNode Pars;
			Pars._Nodes.push_back(Unique_ptr<Node>(node.Expression.Value.get()));

			for (size_t i = 0; i < node.Extended.Parameters._Nodes.size(); i++)
			{
				auto& Item = node.Extended.Parameters._Nodes[i];
				Pars._Nodes.push_back(Unique_ptr<Node>(Item.get()));
			}

			auto FuncInfo = GetFunc(Name,Pars, Get_LookingForType());
		
		
			for (auto& Item : Pars._Nodes)
			{
				auto Node = Item.release();//is ok it was borrwed.
			}
		

			FuncToSyboID.AddValue(GetSymbolID(node), FuncInfo);


			SetFuncRetAsLastEx(FuncInfo);
		}
		GetExpressionMode.pop();

	}
	if (passtype == PassType::BuidCode)
	{
		auto& FuncInfo = FuncToSyboID.at(GetSymbolID(node));



		ValueParametersNode Pars;
		Pars._Nodes.push_back(Unique_ptr<Node>(node.Expression.Value.get()));

		for (size_t i = 0; i < node.Extended.Parameters._Nodes.size(); i++)
		{
			auto& Item = node.Extended.Parameters._Nodes[i];
			Pars._Nodes.push_back(Unique_ptr<Node>(Item.get()));
		}
		DoFuncCall(FuncInfo, {}, Pars);


		for (auto& Item : Pars._Nodes)
		{
			auto Node = Item.release();//is ok it was borrwed.
		}
	}
}
TypeSymbol SystematicAnalysis::ExtendedFuncExpressionGetTypeToStart(const TypeSymbol& ExpressionType, const ExtendedFuncExpression& node)
{

	if (node.Operator == ScopedName::Operator_t::Dot)
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

	if (passtype == PassType::FixedTypes)
	{
	
		auto Type = ConvertAndValidateType(node.TypeOp, NodeSyb_t::Any);
		Type.SetAsTypeInfo();


		LastExpressionType = Type;
	}
	if (passtype == PassType::BuidCode)
	{
		
		const Token* Token = node.TypeOp.Name.Token; 
		LogCantOutputTypeinfo(Token);
	}
}

void SystematicAnalysis::OnExpressionToTypeValueNode(const ExpressionToTypeValueNode& node)
{
	if (passtype == PassType::GetTypes)
	{
		LookingForTypes.push(TypesEnum::Any);
		OnExpressionTypeNode(node.TypeEx,GetValueMode::Read);
		LookingForTypes.pop();
	}
	if (passtype == PassType::FixedTypes)
	{
		LookingForTypes.push(TypesEnum::Any);
		OnExpressionTypeNode(node.TypeEx, GetValueMode::Read);
		LookingForTypes.pop();

		auto Type = LastExpressionType;
		Type.SetAsTypeInfo();

		LastExpressionType = Type;
	}
	if (passtype == PassType::BuidCode)
	{
		const Token* Token = LastLookedAtToken;
		LogCantOutputTypeinfo(Token);
	}
}
void SystematicAnalysis::OnMatchStatement(const MatchStatement& node)
{
	if (passtype == PassType::GetTypes)
	{
		OnExpressionTypeNode(node.Expression,GetValueMode::Read);

		size_t ScopeCounter = 0;
		const String ScopeName = std::to_string((uintptr_t)&node);

		for (auto& Item : node.Arms)
		{

			_Table.AddScope(ScopeName + std::to_string(ScopeCounter));

			OnExpressionTypeNode(Item.Expression, GetValueMode::Read);
			
			for (auto& Statement : Item.Statements._Nodes) 
			{
				OnStatement(*Statement);
			}
			_Table.RemoveScope();

			ScopeCounter++;
		}


		if (node.InvaidCase.has_value())
		{
			_Table.AddScope(ScopeName + std::to_string(ScopeCounter));

			for (auto& Statement : node.InvaidCase.value()._Nodes)
			{
				OnStatement(*Statement);
			}

			_Table.RemoveScope();
		}
	}
	if (passtype == PassType::FixedTypes)
	{
		OnExpressionTypeNode(node.Expression, GetValueMode::Read);
		
		size_t ScopeCounter = 0;

		auto ToMatchType = LastExpressionType;

		const String ScopeName = std::to_string((uintptr_t)&node);
	

		MatchStatementData V;

		for (auto& Item : node.Arms)
		{
			_Table.AddScope(ScopeName + std::to_string(ScopeCounter));
			
			CanMatch(ToMatchType,node.Expression, Item.Expression,V.ArmData);

			for (auto& Statement : Item.Statements._Nodes)
			{
				OnStatement(*Statement);
			}
			
			_Table.RemoveScope();

			ScopeCounter++;
		}

		if (node.InvaidCase.has_value())
		{
			_Table.AddScope(ScopeName + std::to_string(ScopeCounter));

			for (auto& Statement : node.InvaidCase.value()._Nodes)
			{
				OnStatement(*Statement);
			}

			_Table.RemoveScope();
		}
		else
		{
			CheckAllValuesAreMatched(ToMatchType, V.ArmData);
		}

		MatchStatementDatas.AddValue(GetSymbolID(node), std::move(V));
	}
	if (passtype == PassType::BuidCode)
	{
		size_t ScopeCounter = 0;
		const String ScopeName = std::to_string((uintptr_t)&node);

		OnExpressionTypeNode(node.Expression, GetValueMode::Read);

		auto Ex = _LastExpressionField;
		auto ToMatchType = LastExpressionType;

		MatchStatementData& V = MatchStatementDatas.at(GetSymbolID(node));
	
		
		BuildMatch_State State;

		for (size_t i = 0; i < node.Arms.size(); i++)
		{
			auto& Item = node.Arms[i];

			auto& VItem = V.ArmData.Arms[i];

			_Table.AddScope(ScopeName + std::to_string(ScopeCounter));

			auto V = BuildMatch(ToMatchType,node.Expression,Ex, State,VItem,Item.Expression);

			for (auto& Statement : Item.Statements._Nodes)
			{
				OnStatement(*Statement);
			}

			EndMatch(V, State);

			State.MatchList.push_back(std::move(V));

			_Table.RemoveScope();

			ScopeCounter++;
		}

		if (node.InvaidCase.has_value())
		{
			auto& Item = node.InvaidCase.value();

			_Table.AddScope(ScopeName + std::to_string(ScopeCounter));

			auto V = BuildInvaildMatch(ToMatchType, Ex, State);

			for (auto& Statement : Item._Nodes)
			{
				OnStatement(*Statement);
			}

			EndMatch(V, State);

			State.MatchList.push_back(std::move(V));

			_Table.RemoveScope();
		}

		EndMatchState(State);
	}
}
bool SystematicAnalysis::MatchShouldOutPassEnumValue(const ExpressionNodeType& node)
{
	ValueExpressionNode* Val = ValueExpressionNode::As(node.Value.get());
	if (Val->Value->Get_Type() == NodeType::FuncCallNode)
	{
		const FuncCallNode* Call = FuncCallNode::As(Val->Value.get());

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
	V.Func.FuncName.ScopedName = Call->FuncName.ScopedName;

	V.Func.Parameters._Nodes.resize(Call->Parameters._Nodes.size() + 1);
	V.Func.Parameters._Nodes[0].reset(node.Value.get());
	memcpy(V.Func.Parameters._Nodes.data() + 1, Call->Parameters._Nodes.data(), Call->Parameters._Nodes.size() * sizeof(Unique_ptr<Node>));

	
	V.NewNode.Value.reset(&V.NewValNode);
	V.NewValNode.Value.reset(&V.Func);
}
void SystematicAnalysis::MatchAutoPassEnd(MatchAutoPassEnum& V)
{
	V.NewNode.Value.release();
	V.NewValNode.Value.release();
	for (auto& Item : V.Func.Parameters._Nodes)
	{
		Item.release();
	}
}
void SystematicAnalysis::CanMatch(const TypeSymbol& MatchItem,const ExpressionNodeType& MatchValueNode, const ExpressionNodeType& node, MatchArmData& Data)
{
	bool IsJust =
		 MatchItem._IsAddressArray == false
		&& MatchItem._IsDynamic == false
		&& MatchItem._TypeInfo == TypeInfoPrimitive::Null;

	bool IsOk = false;
	if (IsJust)
	{
		if (IsIntType(MatchItem) ||
			IsfloatType(MatchItem._Type) ||
			IsCharType(MatchItem._Type) ||
			MatchItem._Type == TypesEnum::Bool)
		{

			if (node.Value.get()->Get_Type() == NodeType::ValueExpressionNode)
			{
				LookingForTypes.push(MatchItem);
				OnExpressionTypeNode(node, GetValueMode::Read);
				LookingForTypes.pop();

				auto Type = LastExpressionType;
				if (!CanBeImplicitConverted(MatchItem, Type, false))
				{
					const Token* token = LastLookedAtToken;
					LogCantCastImplicitTypes(token, MatchItem, Type, false);
				}

				Data.Arms.push_back({});

				IsOk = true;
			}
			else
			{
				const Token* token = LastLookedAtToken;
				LogError(ErrorCodes::InValidType, token->OnLine, token->OnPos, "The Expression cant be Matched use only ValueExpression");
			}

		}
		else
		{
			auto Syb = GetSymbol(MatchItem);
			if (Syb) 
			{
				if (Syb->Type == SymbolType::Enum)
				{
					if (node.Value.get()->Get_Type() == NodeType::ValueExpressionNode)
					{
						
						auto Arm = MatchArm();
						Arm._AutoPassEnum = std::make_shared<MatchAutoPassEnum>();
						if (MatchShouldOutPassEnumValue(node))
						{ 
							
							const ValueExpressionNode* Val = ValueExpressionNode::As(node.Value.get());
							const FuncCallNode* Call = FuncCallNode::As(Val->Value.get());
							
							
							auto& Ptr = Arm.Get_AutoPassEnum();

							MatchAutoPassEnumValueStart(Ptr, MatchValueNode, Val, Call);

							NodeTypeStack.push_back(NodeType::MatchArm);
							LookingForTypes.push(MatchItem);
							OnExpressionTypeNode(Ptr.NewNode, GetValueMode::Read);
							LookingForTypes.pop();
							NodeTypeStack.pop_back();

							//MatchAutoPassEnd(Ptr);

							LastExpressionType = MatchItem;
						}
						else 
						{
							NodeTypeStack.push_back(NodeType::MatchArm);
							LookingForTypes.push(MatchItem);
							OnExpressionTypeNode(node, GetValueMode::Read);
							LookingForTypes.pop();
							NodeTypeStack.pop_back();
						}
						auto Type = LastExpressionType;
						if (!CanBeImplicitConverted(MatchItem, Type, false))
						{
							const Token* token = LastLookedAtToken;
							LogCantCastImplicitTypes(token, MatchItem, Type, false);
						}

						Data.Arms.push_back(std::move(Arm));

						IsOk = true;
					}
					else
					{
						const Token* token = LastLookedAtToken;
						LogError(ErrorCodes::InValidType, token->OnLine, token->OnPos, "The Expression cant be Matched use only ValueExpression");
					}
				}
				else
				{
					if (node.Value.get()->Get_Type() == NodeType::ValueExpressionNode)
					{

						auto Arm = MatchArm();
						Arm._AutoPassEnum = std::make_shared<MatchAutoPassEnum>();
						if (MatchShouldOutPassEnumValue(node))
						{

							const ValueExpressionNode* Val = ValueExpressionNode::As(node.Value.get());
							const FuncCallNode* Call = FuncCallNode::As(Val->Value.get());


							auto& Ptr = Arm.Get_AutoPassEnum();

							MatchAutoPassEnumValueStart(Ptr, MatchValueNode, Val, Call);

							NodeTypeStack.push_back(NodeType::MatchArm);
							LookingForTypes.push(MatchItem);
							OnExpressionTypeNode(Ptr.NewNode, GetValueMode::Read);
							LookingForTypes.pop();
							NodeTypeStack.pop_back();

							//MatchAutoPassEnd(Ptr);

							LastExpressionType = MatchItem;
						}
						else
						{
							NodeTypeStack.push_back(NodeType::MatchArm);
							LookingForTypes.push(MatchItem);
							OnExpressionTypeNode(node, GetValueMode::Read);
							LookingForTypes.pop();
							NodeTypeStack.pop_back();
						}
						auto Type = LastExpressionType;
						if (!CanBeImplicitConverted(MatchItem, Type, false))
						{
							const Token* token = LastLookedAtToken;
							LogCantCastImplicitTypes(token, MatchItem, Type, false);
						}

						Data.Arms.push_back(std::move(Arm));

						IsOk = true;
					}
					else
					{
						const Token* token = LastLookedAtToken;
						LogError(ErrorCodes::InValidType, token->OnLine, token->OnPos, "The Expression cant be Matched use only ValueExpression");
					}
				}
			}
		}
	}
	
	if (IsOk == false)
	{
		const Token* token = LastLookedAtToken;
		LogError(ErrorCodes::InValidType, token->OnLine, token->OnPos, "The type '" + ToString(MatchItem) + "' cant be Matched");
	}
}
void SystematicAnalysis::CheckAllValuesAreMatched(const TypeSymbol& MatchItem, const MatchArmData& Data)
{
	if (MatchItem.IsAn(TypesEnum::Bool))
	{
		bool ValuesSet[2] = { false,false };
		for (auto& Item : Data.Arms)
		{
			
		}
	}
}
SystematicAnalysis::BuildMatch_ret SystematicAnalysis::BuildMatch(const TypeSymbol& MatchItem, const ExpressionNodeType& MatchValueNode,IRInstruction* Item, BuildMatch_State& State, MatchArm& Arm, const ExpressionNodeType& ArmEx)
{
	bool IsJust =
		MatchItem._IsAddressArray == false
		&& MatchItem._IsDynamic == false
		&& MatchItem._TypeInfo == TypeInfoPrimitive::Null;


	size_t EndMatchIndex = LookingAtIRBlock->GetIndex();
	if (State.MatchList.size())
	{
		auto& Last = State.MatchList.back();

		LookingAtIRBlock->UpdateConditionaJump(Last.JumpToUpdateIFMatchTrue.ConditionalJump, Last.JumpToUpdateIFMatchTrue.logicalNot, EndMatchIndex);
	}

	if (IsJust)
	{
		if (IsIntType(MatchItem) ||
			IsfloatType(MatchItem._Type) ||
			IsCharType(MatchItem._Type) ||
			MatchItem._Type == TypesEnum::Bool)
		{


			if (ArmEx.Value.get()->Get_Type() == NodeType::ValueExpressionNode)
			{
				LookingForTypes.push(MatchItem);
				OnExpressionTypeNode(ArmEx, GetValueMode::Read);
				LookingForTypes.pop();

				auto Type = LastExpressionType;
				auto ArmExIR = _LastExpressionField;
				auto IRToTest = LookingAtIRBlock->NewC_Equalto(Item, ArmExIR);


				SystematicAnalysis::BuildMatch_ret R;
				R.JumpToUpdateIFMatchTrue = LookingAtIRBlock->NewConditionalFalseJump(IRToTest, 0);
				return R;
			}
			else
			{
				throw std::exception("bad path");
			}
		}
		else 
		{
			auto Syb = GetSymbol(MatchItem);
			if (Syb->Type == SymbolType::Enum)
			{
				if (MatchShouldOutPassEnumValue(ArmEx))
				{
					const ValueExpressionNode* Val = ValueExpressionNode::As(ArmEx.Value.get());
					const FuncCallNode* Call = FuncCallNode::As(Val->Value.get());

					auto& Ptr = Arm.Get_AutoPassEnum();

					//MatchAutoPassEnumValueStart(Ptr, MatchValueNode, Val, Call);


					LookingForTypes.push(MatchItem);
					OnExpressionTypeNode(Ptr.NewNode, GetValueMode::Read);
					LookingForTypes.pop();

					MatchAutoPassEnd(Ptr);

					auto Type = LastExpressionType;
					auto ArmExIR = _LastExpressionField;


					SystematicAnalysis::BuildMatch_ret R;
					R.JumpToUpdateIFMatchTrue = LookingAtIRBlock->NewConditionalFalseJump(ArmExIR, 0);
					return R;
				}
				else
				{
					LookingForTypes.push(MatchItem);
					OnExpressionTypeNode(ArmEx, GetValueMode::Read);
					LookingForTypes.pop();

					auto Type = LastExpressionType;
					auto ArmExIR = _LastExpressionField;
					auto IRToTest = LookingAtIRBlock->NewC_Equalto(Item, ArmExIR);


					SystematicAnalysis::BuildMatch_ret R;
					R.JumpToUpdateIFMatchTrue = LookingAtIRBlock->NewConditionalFalseJump(IRToTest, 0);
					return R;
				}
			}
			else
			{
				throw std::exception("bad path");
			}
		}
	}
}
SystematicAnalysis::BuildMatch_ret SystematicAnalysis::BuildInvaildMatch(const TypeSymbol& MatchItem, IRInstruction* Item, BuildMatch_State& State)
{
	size_t EndMatchIndex = LookingAtIRBlock->GetIndex();
	if (State.MatchList.size())
	{
		auto& Last = State.MatchList.back();

		LookingAtIRBlock->UpdateConditionaJump(Last.JumpToUpdateIFMatchTrue.ConditionalJump, Last.JumpToUpdateIFMatchTrue.logicalNot, EndMatchIndex);
	}
	return BuildMatch_ret();
}
void SystematicAnalysis::EndMatch(BuildMatch_ret& Value, BuildMatch_State& State)
{
	size_t EndMatchIndex = LookingAtIRBlock->GetIndex();

	Value.JumpToUpdateEndIndex = LookingAtIRBlock->NewJump();
}
void SystematicAnalysis::EndMatchState(BuildMatch_State& State)
{
	size_t EndIndex = LookingAtIRBlock->GetIndex();
	for (auto& Item : State.MatchList)
	{
		LookingAtIRBlock->UpdateJump(Item.JumpToUpdateEndIndex, EndIndex);

	}
}
void SystematicAnalysis::OnMatchExpression(const MatchExpression& node)
{
	if (passtype == PassType::GetTypes)
	{

		OnExpressionTypeNode(node.Expression, GetValueMode::Read);

		size_t ScopeCounter = 0;
		const String ScopeName = std::to_string((uintptr_t)&node);

		for (auto& Item : node.Arms)
		{

			_Table.AddScope(ScopeName + std::to_string(ScopeCounter));

			OnExpressionTypeNode(Item.Expression, GetValueMode::Read);

			OnExpressionTypeNode(Item.AssignmentExpression,GetValueMode::Read);
		}


		if (node.InvaidCase.has_value())
		{
			_Table.AddScope(ScopeName + std::to_string(ScopeCounter));

			OnExpressionTypeNode(node.InvaidCase.value(), GetValueMode::Read);

			_Table.RemoveScope();
		}
	}
	if (passtype == PassType::FixedTypes)
	{
		OnExpressionTypeNode(node.Expression, GetValueMode::Read);

		size_t ScopeCounter = 0;

		auto ToMatchType = LastExpressionType;

		auto MatchAssignmentType = LookingForTypes.top();

		const String ScopeName = std::to_string((uintptr_t)&node);


		MatchExpressionData V;

		for (auto& Item : node.Arms)
		{
			_Table.AddScope(ScopeName + std::to_string(ScopeCounter));

			CanMatch(ToMatchType, node.Expression, Item.Expression, V.ArmData);

			OnExpressionTypeNode(Item.AssignmentExpression, GetValueMode::Read);
			auto AssignmentType = LastExpressionType;
			if (!CanBeImplicitConverted(AssignmentType, MatchAssignmentType))
			{
				const Token* token = LastLookedAtToken;
				LogCantCastImplicitTypes(token, MatchAssignmentType, AssignmentType, false);
			}


			_Table.RemoveScope();

			ScopeCounter++;
		}


		
		if (node.InvaidCase.has_value())
		{
			_Table.AddScope(ScopeName + std::to_string(ScopeCounter));

			OnExpressionTypeNode(node.InvaidCase.value(), GetValueMode::Read);
			auto AssignmentType = LastExpressionType;
			if (!CanBeImplicitConverted(AssignmentType, MatchAssignmentType))
			{
				const Token* token = LastLookedAtToken;
				LogCantCastImplicitTypes(token, MatchAssignmentType, AssignmentType, false);
			}


			_Table.RemoveScope();
		}
		else
		{
			CheckAllValuesAreMatched(ToMatchType,V.ArmData);
		}

		V.MatchAssignmentType = MatchAssignmentType;
		MatchExpressionDatas.AddValue(GetSymbolID(node), std::move(V));
	}
	if (passtype == PassType::BuidCode)
	{
		size_t ScopeCounter = 0;
		const String ScopeName = std::to_string((uintptr_t)&node);

		OnExpressionTypeNode(node.Expression, GetValueMode::Read);

		auto Ex = _LastExpressionField;
		auto ToMatchType = LastExpressionType;

		MatchExpressionData& V = MatchExpressionDatas.at(GetSymbolID(node));

		

		IRInstruction* OutEx = LookingAtIRBlock->NewLoad(ConvertToIR(V.MatchAssignmentType));

		BuildMatch_State State;
		
		LookingForTypes.push(V.MatchAssignmentType);
		for (size_t i = 0; i < node.Arms.size(); i++)
		{
			auto& Item = node.Arms[i];

			auto& VItem = V.ArmData.Arms[i];

			_Table.AddScope(ScopeName + std::to_string(ScopeCounter));

			auto V2 = BuildMatch(ToMatchType, node.Expression, Ex, State, VItem, Item.Expression);

			OnExpressionTypeNode(Item.AssignmentExpression, GetValueMode::Read);

			DoImplicitConversion(_LastExpressionField, LastExpressionType, V.MatchAssignmentType);

			LookingAtIRBlock->NewStore(OutEx, _LastExpressionField);

			EndMatch(V2,State);

			State.MatchList.push_back(std::move(V2));

			_Table.RemoveScope();

			ScopeCounter++;
		}

		if (node.InvaidCase.has_value())
		{
			_Table.AddScope(ScopeName + std::to_string(ScopeCounter));

			auto V2 =BuildInvaildMatch(ToMatchType, Ex, State);

			OnExpressionTypeNode(node.InvaidCase.value(), GetValueMode::Read);

			DoImplicitConversion(_LastExpressionField, LastExpressionType, V.MatchAssignmentType);

			LookingAtIRBlock->NewStore(OutEx, _LastExpressionField);

			EndMatch(V2, State);

			State.MatchList.push_back(std::move(V2));

			_Table.RemoveScope();
		}

		EndMatchState(State);

		LookingForTypes.pop();

		_LastExpressionField = OutEx;
	}
}
String SystematicAnalysis::GetFuncAnonymousObjectFullName(const String& FullFuncName)
{
	return FullFuncName + "!";
}
void SystematicAnalysis::AddClass_tToAssemblyInfo(const Vector<Unique_ptr<AttributeNode>>& attributes, const Symbol* ClassSyb)
{
	const ClassInfo* Class = ClassSyb->Get_Info<ClassInfo>();
	Class_Data& VClass = _Lib.Get_Assembly().AddClass((String)Class->Get_Name(), Class->FullName);
	
	TypeSymbol AsType = TypeSymbol(ClassSyb->ID);

	VClass.Size = 0;
	VClass.TypeID = GetTypeID(AsType._Type, AsType._CustomTypeSymbol);
	
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
	
	for (const auto& Trait : Class->_InheritedTypes)
	{
		auto Typeid = GetTypeID(TypesEnum::CustomType, Trait.Syb->ID);
		
		InheritedTrait_Data Item;
		Item.TraitID = Typeid;
		VClass.InheritedTypes.push_back(std::move(Item));
	}
	ConvertAttributes(attributes,VClass.Attributes.Attributes);
}

ReflectionTypeInfo SystematicAnalysis::ConvertToTypeInfo(const TypeSymbol& Type)
{
	ReflectionTypeInfo r;
	r._Type = Type._Type;
	r._CustomTypeID =GetTypeID(Type._Type, Type._CustomTypeSymbol);
	

	r._IsAddress= Type.IsAddress();
	r._IsAddressArray= Type.IsAddressArray();
	r._Isimmutable = Type.Isimmutable();
	r._IsDynamic = Type.IsDynamicTrait();
	r._MoveData = Type._MoveData;
	return r;
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
bool SystematicAnalysis::AreTheSame(const ParInfo& TypeA, const ParInfo& TypeB)
{
	if (TypeA.IsOutPar != TypeB.IsOutPar)
	{
		return false;
	}

	return AreTheSame(TypeA.Type, TypeB.Type);
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
				if (!AreTheSameWithOutimmutable(Item1.Type, Item2.Type) || Item1.IsOutPar != Item2.IsOutPar)
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
void SystematicAnalysis::LoadType(const ReflectionTypeInfo& Item, TypeSymbol& Out)
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
TypeSymbol SystematicAnalysis::LoadType(const ReflectionTypeInfo& Item)
{
	TypeSymbol R;
	LoadType(Item, R);
	return R;
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

		if (IsCharType(TypeA))
		{
			return { IsSameValueComparisonOp, {} };
		}
		if (TypeA._Type == TypesEnum::Bool)
		{
			return { IsSameValueComparisonOp || IslogicalOperator , {} };
		}

		if (TypeA._Type == TypesEnum::CustomType)
		{
			auto Syb = GetSymbol(TypeA);
			if (Syb->Type == SymbolType::Enum) 
			{
				return { IsSameValueComparisonOp, {} };
			}
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
								bool r = CanBeImplicitConverted(TypeA, funcInfo->Pars[0].Type)
									&& CanBeImplicitConverted(TypeB, funcInfo->Pars[1].Type);
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
								bool r = CanBeImplicitConverted(TypeA, funcInfo->Pars[0].Type)
									 && CanBeImplicitConverted(TypeB, funcInfo->Pars[1].Type);
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
								bool r = CanBeImplicitConverted(TypeA, funcInfo->Pars[0].Type);
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
						bool r = CanBeImplicitConverted(TypeA, funcInfo->Pars[0].Type)
							&& CanBeImplicitConverted(TypeB, funcInfo->Pars[1].Type);
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
						bool r = CanBeImplicitConverted(TypeA, funcInfo->Pars[0].Type);
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
								bool r = CanBeImplicitConverted(TypeA, funcInfo->Pars[0].Type);
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
		else if (Syb.Type == SymbolType::ConstantExpression)
		{
			ConstantExpressionInfo* Info = Syb.Get_Info<ConstantExpressionInfo>();
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
		throw std::exception("bad Type");
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
TypeSymbolID SystematicAnalysis::GetTypeID(TypesEnum Type, SymbolID SymbolId)
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
		auto* Syb = GetSymbol(SymbolId);
		R = (ReflectionCustomTypeID)std::hash<String>()(Syb->FullName);
	}
	break;
	default:
		R = 0;
		break;
	}
	return R;
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
			Symbol* SybV = _ClassStack.top().Syb;
			if (SybV->Type == SymbolType::Generic_class)
			{
				Out = GetUnMapType();
			}
			else 
			{
				Out = SybV->VarType;
			}
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
		LastLookedAtToken = V.Name.Token;
		if (V.Generic.Values.size())
		{
			auto Val = InstantiateOrFindGenericSymbol(V.Name.Token, V.Generic, Name);
			if (!Val.has_value()) { return; }
			SybV = Val.value();
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
			if (AccessCheck(SybV,V.Name.Token))
			{
				Out.SetType(TypesEnum::Null);
				return;
			}

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
			else if (SybV->Type == SymbolType::Unmaped_Generic_Type)
			{
				Out.SetType(SybV->ID);
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
			SymbolID id = GetSymbolID(node);
			auto& Syb = AddSybol(SymbolType::ConstantExpression, "?", "?",AccessModifierType::Private);
			_Table.AddSymbolID(Syb, id);

			ConstantExpressionInfo* info = new ConstantExpressionInfo();
			info->Exnode = ExpressionNodeType::As(node);
			info->Conext = Get_SymbolConext();

			LookingForTypes.push(TypesEnum::Any);

			auto IsOk = EvaluateToAnyType(*info->Exnode);

			LookingForTypes.pop();
			
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
		if (passtype != PassType::GetTypes)
		{
			auto ExNode = ExpressionNodeType::As(V.node.get());

			
			auto IsOk = EvaluateToAnyType(*ExNode);

			if (IsOk.has_value())
			{
				auto& Object = IsOk.value();

				if (!Object.Type.IsTypeInfo())
				{
					Out.SetType(TypesEnum::Null);

					auto Token = V.Name.Token;
					auto& Type = Object.Type;
					LogCantBindTypeItNotTypeInfo(Token, Type);
				}
				else 
				{
					auto* TypeSyb = Get_ObjectAs<TypeSymbol>(Object);
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
		SymbolID id = GetSymbolID(node);
		auto BaseTypeName = ToString(Out);
		auto FullName = CompilerGenerated("StaticArray_") + BaseTypeName + std::to_string(id.AsInt());

		if (passtype == PassType::GetTypes)
		{
			return;
		}

		Symbol* Syb = GetSymbol(FullName,SymbolType::Null);
		if (Syb == nullptr)
		{
			SymbolID id = GetSymbolID(node);
			Syb = &AddSybol(SymbolType::Type_StaticArray, FullName, FullName,AccessModifierType::Public);
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

		
			LookingForTypes.push(UIntType);
			auto IsOk = Evaluate(UIntType,*node);
			LookingForTypes.pop();

			if (IsOk)
			{
				void* V = Get_Object(IsOk.value());

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

Optional<Symbol*> SystematicAnalysis::InstantiateOrFindGenericSymbol(const Token* Token, const UseGenericsNode& GenericsVals, const String_view& Name)
{
	auto SybV = GetSymbol(Name, SymbolType::Generic_class);
	if (SybV == nullptr)
	{
		LogCantFindTypeError(Token, Name);
		return {};
	}
	if (SybV->Type != SymbolType::Generic_class
		&& SybV->Type != SymbolType::Generic_Alias
		&& SybV->Type != SymbolType::Generic_Enum)
	{
		LogExpectedSymbolToBea(Token, *SybV, SymbolType::Generic_class);
		return {};
	}

	Symbol* R = nullptr;
	if (SybV->Type == SymbolType::Generic_class)
	{
		auto CInfo = SybV->Get_Info<ClassInfo>();
		auto classnode = ClassNode::As(SybV->Get_NodeInfo<Node>());
		R = InstantiateOrFindGeneric_Class(Token, SybV, classnode->Generic, CInfo->_GenericData, GenericsVals);
	}
	else if (SybV->Type == SymbolType::Generic_Alias)
	{
		auto CInfo = SybV->Get_Info<Generic_AliasInfo>();
		auto classnode = AliasNode::As(SybV->Get_NodeInfo<Node>());
		R = InstantiateOrFindGeneric_Alias(Token, SybV, classnode->Generic, CInfo->_GenericData, GenericsVals);
	}
	else if (SybV->Type == SymbolType::Generic_Enum)
	{
		auto CInfo = SybV->Get_Info<EnumInfo>();
		auto classnode = EnumNode::As(SybV->Get_NodeInfo<Node>());
		R = InstantiateOrFindGeneric_Enum(Token, SybV, classnode->Generic, CInfo->_GenericData, GenericsVals);
	}

	if (R)
	{
		return R;
	}
	return {};
}


TypeSymbol SystematicAnalysis::GetStaticArrayType(const TypeSymbol& BaseType,size_t Size)
{
	auto BaseTypeName = ToString(BaseType);
	auto FullName = CompilerGenerated("StaticArray_") + BaseTypeName + std::to_string(Size);
	auto Syb = GetSymbol(FullName,SymbolType::Type);
	if (Syb == nullptr)
	{
		Syb = &AddSybol(SymbolType::Type_StaticArray, FullName, FullName, AccessModifierType::Public);
		SymbolID id = GetSymbolID(Syb);
		_Table.AddSymbolID(*Syb, id);

		StaticArrayInfo* info = new StaticArrayInfo();


		info->Type = BaseType;
		info->Count = Size;

		Syb->Info.reset(info);
	}
	return TypeSymbol(Syb->ID);
}

inline IRInstruction* SystematicAnalysis::RawObjectDataToCString(const RawEvaluatedObject& EvalObject)
{
	String Str = String(String_view((const char*)EvalObject.Object_AsPointer.get(), EvalObject.ObjectSize));
	String_view Buffer{ Str.data(),Str.size() + 1 };//for null char

	auto BufferIR = _Builder.FindOrAddConstStrings(Buffer);
	return LookingAtIRBlock->NewLoadPtr(BufferIR);
}

void SystematicAnalysis::LogCantBindTypeItNotTypeInfo(const UCodeLang::Token* Token, UCodeLang::FrontEnd::TypeSymbol& Type)
{

	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "Cant Bind type.Expression is not a typeinfo it is an '" + ToString(Type) + "'");
}



void SystematicAnalysis::ConvertAndValidateType(const TypeNode& V, TypeSymbol& Out,NodeSyb_t Syb)
{
	Convert(V, Out);
	if (ValidateType(Out,V.Name.Token,Syb) == false)
	{
		Out.SetType(TypesEnum::Null);
	}
	if (Syb == NodeSyb_t::Parameter)
	{
		Out.SetAsLocation();
	}
}
bool SystematicAnalysis::ValidateType(const TypeSymbol& V, const Token* Token,NodeSyb_t Syb)
{
	if (V._Type == TypesEnum::CustomType)
	{
		auto Syb2 = GetSymbol(V);
		if (Syb2->Type == SymbolType::Type_StaticArray)
		{
			StaticArrayInfo* V = Syb2->Get_Info<StaticArrayInfo>();

			if (!V->IsCountInitialized)
			{
				LogBeMoreSpecifiicWithStaticArrSize(Token, V->Type);
				return false;
			}
		}
		if (Syb2->Type == SymbolType::ConstantExpression && Syb != NodeSyb_t::Any)
		{
			auto V = Syb2->FullName;
			LogCantFindTypeError(Token,V);
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

					if (!AreTheSameWithOutimmutable(V1.Type, V2.Type) || V1.IsOutPar != V2.IsOutPar)
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
							&& CanBeImplicitConverted(TypeToCheck, funcInfo->Pars[0].Type);
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
						bool r = CanBeImplicitConverted(TypeToCheck, funcInfo->Pars[0].Type);
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
bool SystematicAnalysis::IsVarableType(SymbolType type)
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
bool SystematicAnalysis::IsCharType(const TypeSymbol& TypeToCheck)
{
	return
		TypeToCheck._Type == TypesEnum::Char ||
		TypeToCheck._Type == TypesEnum::Uft8 || 
		TypeToCheck._Type == TypesEnum::Uft16 ||
		TypeToCheck._Type == TypesEnum::Uft32;
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

	if (r == false && TypeToCheck._Type == TypesEnum::CustomType)
	{
		auto V = GetSymbol(TypeToCheck._CustomTypeSymbol);
		if (V && (V->Type == SymbolType::Hard_Type_alias
			|| V->Type == SymbolType::Type_alias))
		{
			return IsPrimitiveNotIncludingPointers(V->VarType);
		}
	}

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

			if (!IsDependencies(Classinfo))
			{
				Update_ClassSym_ToFixedTypes(Sym);
			}
			else
			{
				//do nothing so GetSize will make the error.				
			}

			return Classinfo->_WillHaveFielddeInit;
		}
	}
	return dropfunc;
}
void  SystematicAnalysis::Update_ClassSym_ToFixedTypes(Symbol* Sym)
{
	if (Sym->PassState == PassType::GetTypes)
	{
		ClassInfo* info = Sym->Get_Info<ClassInfo>();
		auto OldConext = Move_SymbolConext();
		Set_SymbolConext(info->Conext.value());

		OnClassNode(*Sym->Get_NodeInfo<ClassNode>());

		Set_SymbolConext(std::move(OldConext));
	}
}
void  SystematicAnalysis::Update_FuncSym_ToFixedTypes(Symbol* Sym)
{
	if (Sym->PassState == PassType::GetTypes)
	{
		FuncInfo* info = Sym->Get_Info<FuncInfo>();

		auto OldConext = Move_SymbolConext();
		Set_SymbolConext(info->Conext.value());

		OnFuncNode(*Sym->Get_NodeInfo<FuncNode>());

		Set_SymbolConext(std::move(OldConext));
	}
}
void SystematicAnalysis::Update_EnumSym_ToFixedTypes(Symbol* Sym)
{
	if (Sym->PassState == PassType::GetTypes)
	{
		EnumInfo* info = Sym->Get_Info<EnumInfo>();

		auto OldConext = Move_SymbolConext();
		Set_SymbolConext(info->Conext.value());

		OnEnum(*Sym->Get_NodeInfo<EnumNode>());

		Set_SymbolConext(std::move(OldConext));
	}
}
void SystematicAnalysis::Update_TraitSym_ToFixedTypes(Symbol* Sym)
{
	if (Sym->PassState == PassType::GetTypes)
	{
		TraitInfo* info = Sym->Get_Info<TraitInfo>();

		auto OldConext = Move_SymbolConext();
		Set_SymbolConext(info->Conext.value());

		OnTrait(*Sym->Get_NodeInfo<TraitNode>());

		Set_SymbolConext(std::move(OldConext));
	}
}
void SystematicAnalysis::Update_TagSym_ToFixedTypes(Symbol* Sym)
{
	if (Sym->PassState == PassType::GetTypes)
	{
		TagInfo* info = Sym->Get_Info<TagInfo>();

		auto OldConext = Move_SymbolConext();
		Set_SymbolConext(info->Conext.value());

		OnTag(*Sym->Get_NodeInfo<TagTypeNode>());

		Set_SymbolConext(std::move(OldConext));
	}
}
void SystematicAnalysis::Update_AliasSym_ToFixedTypes(Symbol* Sym)
{
	if (Sym->PassState == PassType::GetTypes)
	{
		AliasInfo* info = Sym->Get_Info<AliasInfo>();

		auto OldConext = Move_SymbolConext();
		Set_SymbolConext(info->Conext.value());

		OnAliasNode(*Sym->Get_NodeInfo<AliasNode>());

		Set_SymbolConext(std::move(OldConext));
	}
}
void SystematicAnalysis::Update_EvalSym_ToFixedTypes(Symbol* Sym)
{
	if (Sym->PassState == PassType::GetTypes)
	{
		ConstantExpressionInfo* info = Sym->Get_Info<ConstantExpressionInfo>();

		auto OldConext = Move_SymbolConext();
		Set_SymbolConext(info->Conext.value());

		OnDeclareVariablenode(*Sym->Get_NodeInfo<DeclareVariableNode>(),DeclareStaticVariableNode_t::Eval);

		Set_SymbolConext(std::move(OldConext));
	}
}
void SystematicAnalysis::Update_Sym_ToFixedTypes(Symbol* Sym)
{
	switch (Sym->Type)
	{
	case SymbolType::Generic_class:
	case SymbolType::Type_class:
		Update_ClassSym_ToFixedTypes(Sym);
		break;

	case SymbolType::Generic_Enum:
	case SymbolType::Enum:
		Update_EnumSym_ToFixedTypes(Sym);
		break;

	case SymbolType::Tag_class:
	case SymbolType::Generic_Tag:
		Update_TagSym_ToFixedTypes(Sym);
		break;

	case SymbolType::ConstantExpression:
		Update_EvalSym_ToFixedTypes(Sym);
		break;

	case SymbolType::Generic_Trait:
	case SymbolType::Trait_class:
		Update_TraitSym_ToFixedTypes(Sym);
		break;

	case SymbolType::GenericFunc:
	case SymbolType::Func:
		Update_FuncSym_ToFixedTypes(Sym);
		break;
	default:
		throw std::exception("bad path");
		break;
	}
}
bool SystematicAnalysis::GetSize(const TypeSymbol& Type, UAddress& OutSize)
{
	if (Type.IsAddress()){goto IntPtr;}
	
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
				auto& classNode = *V.Get_NodeInfo<ClassNode>();
				if (!IsDependencies(Vp))
				{
					auto Old = Move_SymbolConext();
					Set_SymbolConext(Vp->Conext.value());

					OnClassNode(classNode);//update class fields
				
				
					Set_SymbolConext(std::move(Old));
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
	{
		auto Syb = GetSymbol(Name);
		if (Syb) 
		{
			if (Syb->Type == SymbolType::Generic_class || Syb->Type == SymbolType::Generic_Trait)
			{

				//most likely Generic Testing
				Get_FuncInfo R;
				R.CantCheckBecauseIsUnMaped = true;
				return R;
			}
			if (IsUnMapType(Name))
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
	V.token = &T; 

	Tep.ScopedName.push_back(std::move(V));


	return GetFunc(Tep, Pars, Name);
}
void SystematicAnalysis::DoFuncCall(Get_FuncInfo Func, const ScopedNameNode& Name, const ValueParametersNode& Pars)
{
	if (passtype != PassType::BuidCode) { return; }
	
	Add_SetLineNumber(Name.ScopedName.begin()->token, LookingAtIRBlock->Instructions.size());
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

		else PrimitiveTypeCall(UintPtrTypeName, TypesEnum::uIntPtr, _LastExpressionField = LookingAtIRBlock->NewLoad((UInt64)0))
		else PrimitiveTypeCall(SintPtrTypeName, TypesEnum::sIntPtr, _LastExpressionField = LookingAtIRBlock->NewLoad(((UInt64)0)))


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
							SymbolID ID = GetSymbolID(outEx);
						

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
		else if (Func._BuiltFunc.has_value())
		{
			auto& Value = Func._BuiltFunc.value();

			if (Value.EvalObject.has_value())
			{
				auto& EvalObject = Value.EvalObject.value();
				if (Value.EvalAsCString)
				{
					_LastExpressionField = RawObjectDataToCString(EvalObject);

					LastExpressionType = TypesEnum::Char;
					LastExpressionType.SetAsAddressArray();
					LastExpressionType.SetAsimmutable();
				}
				else
				{
					_LastExpressionField = LoadEvaluatedEx(EvalObject, Value.RetType);
				}
			}
			else
			{

				throw std::exception("bad path");
			}

			LastExpressionType = Value.RetType;
		}
		else
		{
			throw std::exception("bad path");
		}
		return;
	}

	IRInstruction* PushIRStackRet = false;
	bool AutoPushThis = Get_FuncInfo::AddOneToGetParNode(Func.ThisPar);

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
					if (Type.Type.IsAddress())
					{
						Type.Type._IsAddress = false;
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
					if (Type.Type.IsAddress())
					{
						Type.Type._IsAddress = false;
					}


					PushIRStackRet = tep.Value = LookingAtIRBlock->NewLoad(ConvertToIR(Type.Type));
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

		auto Index = i + (AutoPushThis ? 1 : 0);

		const Vector<ParInfo>& Pars = Func.SymFunc->Type == SymbolType::Func
			? Func.Func->Pars
			: ((FuncPtrInfo*)Func.Func)->Pars;

		

		auto FuncParInfoPtr = Index < Pars.size() ? Pars[Index]
			: Pars.back();

		if (FuncParInfoPtr.Type._CustomTypeSymbol.HasValue())
		{
			const auto Syb = GetSymbol(FuncParInfoPtr.Type);
			if (Syb->Type == SymbolType::Type_Pack)
			{
				auto* typepack = Syb->Get_Info<TypePackInfo>();
				size_t NewIndex = Index-(Pars.size()-1);


				FuncParInfoPtr.Type = typepack->List[NewIndex];
			} 
		}
		auto& FuncParInfo =FuncParInfoPtr;

		LookingForTypes.push(FuncParInfo.Type);

		if (Item->Get_Type() == NodeType::OutExpression)
		{
			const OutExpression* OutEx = OutExpression::As(Item.get());
			
			auto OutVal = GetSymbolFromExpression(OutEx);

			OutVal->IR_Ins = LookingAtIRBlock->NewLoad(ConvertToIR(OutVal->VarType));

			_LastExpressionField = LookingAtIRBlock->NewLoadPtr(OutVal->IR_Ins);
		}
		else 
		{
			OnExpressionTypeNode(Item.get(), GetValueMode::Read);
			DoImplicitConversion(_LastExpressionField, LastExpressionType, FuncParInfo.Type);
		}
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
		_LastExpressionField = Tep;
	}

	if (LookingForTypes.size() && Get_LookingForType().IsnotAn(TypesEnum::Void) && PushIRStackRet)//constructors are just void funcions so just set last as the input this
	{

		_LastExpressionField = PushIRStackRet;
		LastExpressionType = Func.Func->Pars.front().Type;
		
		
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
SystematicAnalysis::Get_FuncInfo  SystematicAnalysis::GetFunc(const ScopedNameNode& Name,const ValueParametersNode& Pars, TypeSymbol Ret)
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
			GetExpressionMode.push(GetValueMode::Read);
			GetMemberTypeSymbolFromVar_t V;
			bool VBool = GetMemberTypeSymbolFromVar(0, Name.ScopedName.size() - 1, Name, V);
			GetExpressionMode.pop();
			
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
				RemoveTypeattributes(tep_);


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
			ScopedName = "";
			for (auto& Item : Name.ScopedName)
			{
				if (Item.Generic.get() && Item.Generic->Values.size())
				{

					ScopedName += Item.token->Value._String;

					auto TypeName = ScopedName;

					bool IsLast = &Item == &Name.ScopedName.back();
					if (!IsLast)
					{
						auto Syb = InstantiateOrFindGenericSymbol(Item.token, *Item.Generic, ScopedName);

						if (!Syb.has_value())
						{
							return {};
						}
						else
						{
							ScopedName = Syb.value()->FullName;
						}
					}
					if (Item.Operator != ScopedName::Operator_t::Null)
					{
						ScopedName += ScopeHelper::_ScopeSep;
					}
				}
				else
				{
					String Str;
					Item.GetScopedName(Str);
					ScopedName += Str;
					if (Item.Operator != ScopedName::Operator_t::Null)
					{
						ScopedName += ScopeHelper::_ScopeSep;
					}
				}
			}
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
				LastExpressionType = FuncType;
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

		LookingForTypes.push(NullSymbol);


		if (Item->Get_Type() == NodeType::OutExpression)
		{
			const OutExpression* outEx = OutExpression::As(Item.get());
			HasOutPar = true;
			ValueItem.IsOutPar = true;
			ValueItem.Type =ConvertAndValidateType(outEx->_Type,NodeSyb_t::Varable);
		}
		else
		{
			OnExpressionTypeNode(Item.get(), GetValueMode::Read);
			ValueItem.IsOutPar = false;
			ValueItem.Type = LastExpressionType;
		}

		LookingForTypes.pop();
		
		
		
	}

	{//unmaped
		for (auto& Item : ValueTypes)
		{
			auto SymbolsV = GetSymbol(Item.Type);
			if (SymbolsV)
			{
				if (SymbolsV->Type == SymbolType::Unmaped_Generic_Type)
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
			if (NodeTypeStack.size() > 1) 
			{
				size_t Index = NodeTypeStack.size() - 1;
				Index--;
				auto& Last = NodeTypeStack[Index];
				if (Last == NodeType::IfNode || Last == NodeType::WhileNode || Last == NodeType::DoNode
					|| Last == NodeType::RetStatementNode
					|| Last == NodeType::CompileTimeIfNode
					|| Last == NodeType::MatchArm
					)
				{
					IsControlFlow = true;
				}
				else if (NodeTypeStack.back() == NodeType::CompileTimeIfNode)
				{
					IsControlFlow = true;
				}
			}
			if (NodeTypeStack.size() > 2)
			{
				size_t Index = NodeTypeStack.size() - 1;
				Index-=2;
				auto& Last = NodeTypeStack[Index];
				if (Last == NodeType::MatchArm)
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
				_TepThisPar.Value.reset(&_TepThisValue);
				_TepThisValue.Value.reset(&_TepThisReadNode);
				_TepThisReadNode.VariableName = Name;
				_TepThisReadNode.VariableName.ScopedName.pop_back();
				_TepThisReadNode.VariableName.ScopedName.back().Operator = ScopedName::Operator_t::Null;
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
				auto _ = _TepThisPar.Value.release();//On Stack.
				auto _1 = _TepThisValue.Value.release();//On Stack.
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
								SetOutExpressionEval(Ex, EvaluatedValue);
							}
							else
							{
								SetOutExpression(Ex, FuncDataOutPar.Type);
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

	const UseGenericsNode& Generics = *Name.ScopedName.back().Generic;
	for (auto& Item : Symbols)
	{

		if (Item->Type == SymbolType::Func)
		{
			if (Item->IsInvalid())
			{
				return {};
			}
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
				HasBenAdded.resize(Generics.Values.size());
			}
			else
			{
				HasBenAdded.resize(Info->_GenericData._Generic.size());
			}

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


				auto FuncSym = GetSymbol(Info);
				String NewName = GetGenericFuncFullName(FuncSym, GenericInput);
				auto FuncIsMade = GetSymbol(NewName, SymbolType::Func);



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

								auto TypeSyb = GetSymbol(GenericInfo.SybID);
								if (TypeSyb)
								{
									InputTypeIsConstantExpression = TypeSyb->Type == SymbolType::ConstantExpression;
								}

								if (InputTypeIsConstantExpression != GenericInfo.IsConstantExpression())
								{
									goto ContinueOutloop;
								}
							}
						}
					}
					auto Pointer = std::make_unique<Vector<TypeSymbol>>(std::move(GenericInput));
					//pointer must be unique so it cant be on the stack

					GenericFuncInstantiate(FuncSym, *Pointer);

					TepFuncs.push_back({ std::move(Pointer) });//keep pointer 


					FuncSym = GetSymbol(NewName, SymbolType::Func);
				}
				else
				{
					FuncSym = FuncIsMade;
				}
			



				{
					r = FuncSym->Get_Info<FuncInfo>();
					FuncSymbol = FuncSym;

					OkFuncions.push_back({ThisParType,r,FuncSymbol });
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
						ValueTypes.insert(ValueTypes.begin(), { false,V });
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
								ValueTypes.insert(ValueTypes.begin(), { false,V });
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

			ValueTypes.insert(ValueTypes.begin(), { false ,*_FuncStack.back().Pointer->GetObjectForCall() });
			ThisParType = Get_FuncInfo::ThisPar_t::AutoPushThis;
			goto StartSymbolsLoop;
		}
		else 
		{
			LastExpressionType = TypeSymbol(TypesEnum::Null);
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
		
		auto RValue = *Ret;
		AccessCheck(RValue.SymFunc, Name.ScopedName.back().token);


		bool AutoPassThis = Get_FuncInfo::AddOneToGetParNode(ThisParType);
		for (size_t i = 0; i < ValueTypes.size(); i++)
		{
			auto& Item = ValueTypes[i];

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
					throw std::exception("bad path");
				}

				SetOutExpression(Item, TypeSyb);
			}
		}

		return RValue;
		
		
	}
	return { };
}

void SystematicAnalysis::RemoveTypeattributes(UCodeLang::FrontEnd::TypeSymbol& tep_)
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

SystematicAnalysis::Get_FuncInfo SystematicAnalysis::GetEnumVariantFunc(Symbol* EnumSyb, size_t FeildIndex, Symbol* EnumFieldSyb,const ValueParametersNode& Pars, const Token* Token, const Vector<ParInfo>& ValueTypes)
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
			LogCanIncorrectParCount(Token, FullName, Pars._Nodes.size(), Feild_Variant.Types.size()+1);

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
				if (!CanBeImplicitConverted(ExItemType.Type, Vthis))
				{
					LogCantCastImplicitTypes(Token, ExItemType.Type, Vthis, true);
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


					auto& TypeToSet = ItemVariant;

					SetOutExpression(Ex, TypeToSet);
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

			if (!CanBeImplicitConverted(ExItemType.Type, Item))
			{
				LogCantCastImplicitTypes(Token, ExItemType.Type, Item, true);
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

void SystematicAnalysis::SetOutExpression(const OutExpression* Ex, const TypeSymbol& TypeToSet)
{
	auto Str = Ex->_Name.Token->Value._String;

	String FullName = _Table._Scope.ThisScope;
	ScopeHelper::GetApendedString(FullName, Str);

	auto Syb = &AddSybol(SymbolType::StackVarable, (String)Str, FullName, AccessModifierType::Public);

	LookingForTypes.push(TypeToSet);
	Syb->VarType = ConvertAndValidateType(Ex->_Type, NodeSyb_t::Varable);
	LookingForTypes.pop();

	ExDeclareVariableTypeCheck(Syb->VarType, TypeToSet, Ex->_Name.Token);
	_Table.AddSymbolID(*Syb, GetSymbolID(*Ex));
}
Symbol* SystematicAnalysis::GetSymbolFromExpression(const OutExpression* Ex)
{
	return GetSymbol(GetSymbolID(*Ex));
}
void SystematicAnalysis::SetOutExpressionEval(const OutExpression* Ex, const EvaluatedEx& ObjectToSet)
{
	
	auto Str = Ex->_Name.Token->Value._String;

	String FullName = _Table._Scope.ThisScope;
	ScopeHelper::GetApendedString(FullName, Str);

	auto Syb = &AddSybol(SymbolType::ConstantExpression, (String)Str, FullName, AccessModifierType::Public);

	LookingForTypes.push(ObjectToSet.Type);
	Syb->VarType = ConvertAndValidateType(Ex->_Type, NodeSyb_t::Varable);
	LookingForTypes.pop();

	ExDeclareVariableTypeCheck(Syb->VarType, ObjectToSet.Type, Ex->_Name.Token);
	_Table.AddSymbolID(*Syb, GetSymbolID(*Ex));

	ConstantExpressionInfo* Info = new ConstantExpressionInfo();
	Syb->Info.reset(Info);

	Info->Ex = ObjectToSet.EvaluatedObject;
}


String SystematicAnalysis::GetGenericFuncFullName(const Symbol* Func, const Vector<TypeSymbol>& Type)
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
String SystematicAnalysis::GetGenericFuncName(const Symbol* Func, const Vector<TypeSymbol>& Type)
{
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
String SystematicAnalysis::GetGenericExtendedErrValue(const Generic& Generic, const GenericValuesNode GenericAsNode, const Vector<TypeSymbol>& GenericInput)
{
	String V = "Were ";


	bool IsPack = Generic.IsPack();
	size_t LoopSize = IsPack ? GenericInput.size() - 1 : GenericInput.size();
	for (size_t i = 0; i < LoopSize; i++)
	{
		const auto& Item = GenericInput[i];
		bool IsExpression = Generic._Generic[i].IsConstantExpression();

		const auto& GenericName = GenericAsNode.Values[i].Token->Value._String;
		if (IsExpression)
		{
			Symbol* Syb = GetSymbol(Item);
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
		const auto& PackName = GenericAsNode.Values.back().Token->Value._String;
		V += "[" + (String)PackName + "] = [";

		size_t Index = Generic._Generic.size() - 1;
		for (size_t i = Index; i < GenericInput.size(); i++)
		{
			auto& Item = GenericInput[i];

			auto Syb = GetSymbol(Item);

			if (Syb && Syb->Type == SymbolType::ConstantExpression)
			{
				ConstantExpressionInfo* Info = Syb->Get_Info<ConstantExpressionInfo>();
				V += "(" + ToString(Syb->VarType, Info->Ex) + ")";
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
Optional<SymbolID>  SystematicAnalysis::MakeTypePackSymbolIfNeeded(const String& NewName, const Vector<TypeSymbol>& GenericInput, const Generic& Generic)
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



		auto& PackSyb = AddSybol(SymbolType::Type_Pack, "!Pack", ScopeHelper::ApendedStrings(NewName, "!Pack"), AccessModifierType::Public);
		_Table.AddSymbolID(PackSyb, GetSymbolID(&PackSyb));
		TypePackInfo* PackInfo = new TypePackInfo();
		PackInfo->List = std::move(_PackList);

		PackSyb.Info.reset(PackInfo);

		return PackSyb.ID;
	}
	return {};
}
void SystematicAnalysis::GenericFuncInstantiate(const Symbol* Func, const Vector<TypeSymbol>& GenericInput)
{
	const String NewName = GetGenericFuncName(Func, GenericInput);
	const String FullName = GetGenericFuncFullName(Func, GenericInput);

	GenericFuncInfo Info;
	Info.GenericFuncName = NewName;
	Info.GenericInput = &GenericInput;
	Info.NodeTarget = Func->NodePtr;

	const FuncInfo* FInfo = Func->Get_Info<FuncInfo>();
	Info.Pack = MakeTypePackSymbolIfNeeded(NewName,GenericInput, FInfo->_GenericData);

	const FuncNode& FuncBase = *Func->Get_NodeInfo<FuncNode>();

	
	GenericFuncName.push(std::move(Info));

	auto OldConext = Move_SymbolConext();
	auto Oldpasstype = passtype;

	Set_SymbolConext(FInfo->Conext.value());
	{
		AddExtendedErr(GetGenericExtendedErrValue(FInfo->_GenericData,FuncBase.Signature.Generic,GenericInput),FuncBase.Signature.Name.Token);
	}
	{

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
	}
	{
		PopExtendedErr();
	}

	GenericFuncName.pop();
	
	Set_SymbolConext(std::move(OldConext));
	passtype = Oldpasstype;


	AddDependencyToCurrentFile(Func);
}
void SystematicAnalysis::GenericTypeInstantiate(const Symbol* Class, const Vector<TypeSymbol>& Type)
{
	const String NewName = GetGenericFuncName(Class, Type);
	const String FullName = GetGenericFuncFullName(Class, Type);
	const ClassNode* node = ClassNode::As(Class->Get_NodeInfo<Node>());

	const ClassInfo* classInfo = Class->Get_Info<ClassInfo>();

	GenericFuncInfo Info;
	Info.GenericFuncName = NewName;
	Info.GenericInput = &Type;
	Info.NodeTarget = Class->NodePtr;
	GenericFuncName.push(Info);

	Info.Pack = MakeTypePackSymbolIfNeeded(NewName,Type, classInfo->_GenericData);
	{
		AddExtendedErr(GetGenericExtendedErrValue(classInfo->_GenericData, node->Generic,Type), node->ClassName.Token);
	}
	{
		auto OldConext = Move_SymbolConext();
		auto Oldpasstype = passtype;

		Set_SymbolConext(classInfo->Conext.value());


		passtype = PassType::GetTypes;
		OnClassNode(*node);

		if (!_ErrorsOutput->Has_Errors())
		{
			passtype = PassType::FixedTypes;
			OnClassNode(*node);
		}

		if (!_ErrorsOutput->Has_Errors()) 
		{
			passtype = PassType::BuidCode;
			OnClassNode(*node);
		}

		GenericFuncName.pop();
		//
		Set_SymbolConext(std::move(OldConext));
		passtype = Oldpasstype;
		//
	}
	{
		PopExtendedErr();
	}

	AddDependencyToCurrentFile(Class);
}

void SystematicAnalysis::GenericTypeInstantiate_Trait(const Symbol* Trait, const Vector<TypeSymbol>& Type)
{
	const String NewName = GetGenericFuncName(Trait, Type);
	const String FullName = GetGenericFuncFullName(Trait, Type);
	const TraitNode* node = TraitNode::As(Trait->Get_NodeInfo<Node>());

	const TraitInfo* classInfo = Trait->Get_Info<TraitInfo>();

	GenericFuncInfo Info;
	Info.GenericFuncName = NewName;
	Info.GenericInput = &Type;
	Info.NodeTarget = node;
	GenericFuncName.push(Info);

	Info.Pack = MakeTypePackSymbolIfNeeded(NewName, Type, classInfo->_GenericData);
	{
		AddExtendedErr(GetGenericExtendedErrValue(classInfo->_GenericData, node->Generic, Type), node->_Name.Token);
	}
	{
		auto OldConext = Move_SymbolConext();
		auto Oldpasstype = passtype;

		Set_SymbolConext(classInfo->Conext.value());
		passtype = PassType::GetTypes;
		OnTrait(*node);

		if (!_ErrorsOutput->Has_Errors()) 
		{
			passtype = PassType::FixedTypes;
			OnTrait(*node);
		}

		if (!_ErrorsOutput->Has_Errors())
		{
			passtype = PassType::BuidCode;
			OnTrait(*node);
		}

		GenericFuncName.pop();
		//
		Set_SymbolConext(std::move(OldConext));
		passtype = Oldpasstype;
		//
	}
	{
		PopExtendedErr();
	}

	AddDependencyToCurrentFile(Trait);
}

void SystematicAnalysis::GenericTypeInstantiate_Alias(const Symbol* Alias, const Vector<TypeSymbol>& Type)
{
	const String NewName = GetGenericFuncName(Alias, Type);
	const String FullName = GetGenericFuncFullName(Alias, Type);
	const AliasNode* node = AliasNode::As(Alias->Get_NodeInfo<Node>());

	const Generic_AliasInfo* classInfo =Alias->Get_Info<Generic_AliasInfo>();

	GenericFuncInfo Info;
	Info.GenericFuncName = NewName;
	Info.GenericInput = &Type;
	Info.NodeTarget = node;
	GenericFuncName.push(Info);

	Info.Pack = MakeTypePackSymbolIfNeeded(NewName, Type, classInfo->_GenericData);
	{
		AddExtendedErr(GetGenericExtendedErrValue(classInfo->_GenericData, node->Generic, Type), node->AliasName.Token);
	}
	{
		auto OldConext = Move_SymbolConext();
		auto Oldpasstype = passtype;

		Set_SymbolConext(classInfo->Conext.value());

		passtype = PassType::GetTypes;
		OnAliasNode(*node);

		if (!_ErrorsOutput->Has_Errors())
		{
			passtype = PassType::FixedTypes;
			OnAliasNode(*node);
		}

		if (!_ErrorsOutput->Has_Errors()) 
		{
			passtype = PassType::BuidCode;
			OnAliasNode(*node);
		}

		GenericFuncName.pop();
		//
		Set_SymbolConext(std::move(OldConext));
		passtype = Oldpasstype;
		//
	}
	{
		PopExtendedErr();
	}

	AddDependencyToCurrentFile(Alias);
}

void SystematicAnalysis::GenericTypeInstantiate_Enum(const Symbol* Alias, const Vector<TypeSymbol>& Type)
{
	const String NewName = GetGenericFuncName(Alias, Type);
	const String FullName = GetGenericFuncFullName(Alias, Type);
	const EnumNode* node = EnumNode::As(Alias->Get_NodeInfo<Node>());

	const EnumInfo* classInfo = Alias->Get_Info<EnumInfo>();

	GenericFuncInfo Info;
	Info.GenericFuncName = NewName;
	Info.GenericInput = &Type;
	Info.NodeTarget = node;
	GenericFuncName.push(Info);

	Info.Pack = MakeTypePackSymbolIfNeeded(NewName, Type, classInfo->_GenericData);
	{
		AddExtendedErr(GetGenericExtendedErrValue(classInfo->_GenericData, node->Generic, Type), node->EnumName.Token);
	}
	{
		auto OldConext = Move_SymbolConext();
		auto Oldpasstype = passtype;

		Set_SymbolConext(classInfo->Conext.value());


		passtype = PassType::GetTypes;
		OnEnum(*node);

		if (!_ErrorsOutput->Has_Errors()) 
		{
			passtype = PassType::FixedTypes;
			OnEnum(*node);
		}

		if (!_ErrorsOutput->Has_Errors())
		{
			passtype = PassType::BuidCode;
			OnEnum(*node);
		}

		GenericFuncName.pop();
		//
		Set_SymbolConext(std::move(OldConext));
		passtype = Oldpasstype;
		//
	}
	{
		PopExtendedErr();
	}

	AddDependencyToCurrentFile(Alias);
}

void SystematicAnalysis::GenericTypeInstantiate_Tag(const Symbol* Tag, const Vector<TypeSymbol>& Type)
{
	const String NewName = GetGenericFuncName(Tag, Type);
	const String FullName = GetGenericFuncFullName(Tag, Type);
	const TagTypeNode* node = TagTypeNode::As(Tag->Get_NodeInfo<Node>());

	const TagInfo* classInfo = Tag->Get_Info<TagInfo>();

	GenericFuncInfo Info;
	Info.GenericFuncName = NewName;
	Info.GenericInput = &Type;
	Info.NodeTarget = node;
	GenericFuncName.push(Info);

	Info.Pack = MakeTypePackSymbolIfNeeded(NewName, Type, classInfo->_GenericData);
	{
		AddExtendedErr(GetGenericExtendedErrValue(classInfo->_GenericData, node->Generic, Type), node->AttributeName.Token);
	}
	{
		auto OldConext = Move_SymbolConext();
		auto Oldpasstype = passtype;

		Set_SymbolConext(classInfo->Conext.value());

		_Table._Scope.ThisScope = ScopeHelper::GetReMoveScope(FullName);


		passtype = PassType::GetTypes;
		OnTag(*node);

		if (!_ErrorsOutput->Has_Errors()) 
		{
			passtype = PassType::FixedTypes;
			OnTag(*node);
		}

		if (!_ErrorsOutput->Has_Errors()) 
		{
			passtype = PassType::BuidCode;
			OnTag(*node);
		}

		GenericFuncName.pop();
		//
		Set_SymbolConext(std::move(OldConext));
		passtype = Oldpasstype;
		//
	}
	{
		PopExtendedErr();
	}

	AddDependencyToCurrentFile(Tag);
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


SystematicAnalysis::StrExELav SystematicAnalysis::GetStrEVal(const Node* node)
{
	StrExELav CompilerRet;

	OnExpressionTypeNode(node, GetValueMode::Read);

	if (node->Get_Type() == NodeType::ValueExpressionNode)
	{
		const ValueExpressionNode* V = ValueExpressionNode::As(node);
		if (V->Value.get()->Get_Type() == NodeType::StringliteralNode)
		{
			StringliteralNode* strnod = StringliteralNode::As(V->Value.get());

			String Buffer;
			ParseHelper::ParseStringliteralToString(strnod->Token->Value._String, Buffer);
			CompilerRet.OwnedStr = std::move(Buffer);
			CompilerRet.Msg = CompilerRet.OwnedStr;
			CompilerRet.IsConstantExpression = true;
		}
	}

	return CompilerRet;
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

bool SystematicAnalysis::IsCompatible(const IsCompatiblePar& FuncPar,const Vector<ParInfo>& ValueTypes, bool _ThisTypeIsNotNull, const Token* Token)
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
			auto oldconext = Move_SymbolConext();
			
			Set_SymbolConext(Info->Conext.value());

			_RetLoopStack.push_back(Info);

			OnFuncNode(*FuncPar.Item->Get_NodeInfo<FuncNode>());

			_RetLoopStack.pop_back();

			Set_SymbolConext(std::move(oldconext));
		}
		else
		{
			auto V = GetDependencies(Info);
			if (V->IsOnRetStatemnt && Info->Ret.IsNull())
			{
				LogFuncDependencyCycle(Token, Info);
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

		if (!CanBeImplicitConverted(Item2.Type, Item.Type, true))
		{
			return false;
		}
	}
	return true;
}

int SystematicAnalysis::GetCompatibleScore(const ParInfo& ParFunc, const ParInfo& Value)
{
	int r = 0;
	
	if (AreTheSameWithOutimmutable(ParFunc.Type,Value.Type))
	{
		r += 5;
	}
	else if (CanBeImplicitConverted(ParFunc.Type, Value.Type, false))
	{
		r += 3;
	}
	else if (CanBeExplicitlyConverted(ParFunc.Type,Value.Type).HasValue)
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

int SystematicAnalysis::GetCompatibleScore(const IsCompatiblePar& Func, const Vector<ParInfo>& ValueTypes)
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
		r += GetCompatibleScore((*Func.Pars)[i], ValueTypes[ValueTypesIndex]);
	}


	return (*Func.Pars).size() ? r / (*Func.Pars).size() : r;
}
bool SystematicAnalysis::AccessCheck(const Symbol* Syb,const Token* Token, const String_view Scope)
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
			Set_ObjectAs(Out,V);\

#define Set_NumberliteralNodeS2(x) \
			Int##x V; \
			ParseHelper::ParseStringToInt##x(Str, V); \
			Set_ObjectAs(Out,V);\

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
				Set_ObjectAs(Out, (float32)V);
				break;
			};
			case TypesEnum::float64:
			{
				Int64 V;
				ParseHelper::ParseStringToInt64(Str, V);
				Set_ObjectAs(Out, (float64)V);
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
			Set_ObjectAs(Out, num->Get_Value());
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

			Set_ObjectAs(Out, (UInt8)V.front());
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
				Set_ObjectAs(Out, V);
				break;
			}
			case TypesEnum::float64:
			{
				float64 V;
				ParseHelper::ParseStringTofloat64(Str, V);
				Set_ObjectAs(Out, V);
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
	case NodeType::StringliteralNode:
	{
		StringliteralNode* nod = StringliteralNode::As(node.Value.get());
		auto& lookT = Get_LookingForType();

		bool IsStaticArr = IsStaticCharArr(lookT);
		if (IsStaticArr)
		{
			auto VSyb = GetSymbol(lookT);
			StaticArrayInfo* StaticArr = VSyb->Get_Info<StaticArrayInfo>();


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
					LogCanIncorrectStaticArrCount(Token, lookT, BufferSize, StaticArr->Count);
					LastExpressionType.SetType(TypesEnum::Null);
					return false;
				}
			}


			Out = MakeEx(lookT);
			Set_ObjectAs(Out, V.data(), V.size());
			LastExpressionType = lookT;
		}
		else
		{
			auto Token = nod->Token;
			LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos, "Cant use char[&] in Compile Time.");
			LastExpressionType.SetType(TypesEnum::Null);
			return false;
		}
	}
	break;
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
			case TypesEnum::sInt8:
				Set_ObjectAs(Out, (Int8)TypeSize);
				break;
			Int8Case:
			case TypesEnum::uInt8:
				Set_ObjectAs(Out, (UInt8)TypeSize);
				break;
			case TypesEnum::sInt16:
				Set_ObjectAs(Out, (Int16)TypeSize);
				break;
			Int16Case:
			
			case TypesEnum::uInt16:
				Set_ObjectAs(Out, (UInt16)TypeSize);
				break;
			
			case TypesEnum::sInt32:
				Set_ObjectAs(Out, (Int32)TypeSize);
				break;
			Int32Case:
			case TypesEnum::uInt32:
				Set_ObjectAs(Out, (UInt32)TypeSize);
				break;
			
			case TypesEnum::sInt64:
				Set_ObjectAs(Out, (Int64)TypeSize);
				break;
			Int64Case:
			case TypesEnum::uInt64:
				Set_ObjectAs(Out, (UInt64)TypeSize);
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
	case NodeType::ReadVariableNode:
	{
		ReadVariableNode* nod = ReadVariableNode::As(node.Value.get());
		Evaluate(Out,*nod);
	}
	break;
	case NodeType::CMPTypesNode:
	{
		CMPTypesNode* nod = CMPTypesNode::As(node.Value.get());
		return EvalutateCMPTypesNode(Out,*nod);
	}
	break;
	case NodeType::ValidNode:
	{
		ValidNode* nod = ValidNode::As(node.Value.get());
		return EvalutateValidNode(Out, *nod);
	}
	break;
	case NodeType::TypeToValueNode:
	{
		OnTypeToValueNode(*TypeToValueNode::As(node.Value.get()));
		Set_ObjectAs(Out, LastExpressionType);
		return true;
	}
	break;
	case NodeType::ExpressionToTypeValueNode:
	{
		OnExpressionToTypeValueNode(*ExpressionToTypeValueNode::As(node.Value.get()));
		Set_ObjectAs(Out, LastExpressionType);
		return true;
	}
	break;
	case NodeType::FuncCallNode:
	{
		return EvalutateFunc(Out, *FuncCallNode::As(node.Value.get()));
	}
	break;
	default:
		throw std::exception("not added");
		break;
	}
	return true;
}
bool SystematicAnalysis::Evaluate(EvaluatedEx& Out, const ReadVariableNode& nod)
{
	GetMemberTypeSymbolFromVar_t V;
	return EvalutateScopedName(Out, nod.VariableName, V);
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
	TypeSymbol ToTypeAs;
	ConvertAndValidateType(node.ToType, ToTypeAs, NodeSyb_t::Any);

	LookingForTypes.push(ToTypeAs);

	bool Ex0Bool = Evaluate_t(Out,node.Expression.Value.get(),GetValueMode::Read);

	LookingForTypes.pop();


	auto Ex0Type = LastExpressionType;
	auto HasInfo = CanBeExplicitlyConverted(Ex0Type, ToTypeAs);
	if (!HasInfo.HasValue)
	{
		auto  Token = node.ToType.Name.Token;

		LogCantCastExplicityTypes(Token, Ex0Type, ToTypeAs);
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
			LastExpressionType = ToTypeAs;
		}

	}
	return true;
}

bool SystematicAnalysis::Evaluate_t(EvaluatedEx& Out, const Node* node, GetValueMode Mode)
{
	GetExpressionMode.push(Mode);
	bool R =false;
	switch (node->Get_Type())
	{
	case NodeType::BinaryExpressionNode: R = Evaluate(Out,*BinaryExpressionNode::As(node)); break;
	case NodeType::ValueExpressionNode: R = Evaluate(Out, *ValueExpressionNode::As(node)); break;
	case NodeType::CastNode: R = Evaluate(Out, *CastNode::As(node)); break;
	case NodeType::ExtendedFuncExpression: R = Evaluate(Out, *ExtendedFuncExpression::As(node)); break;
	case NodeType::ExtendedScopeExpression: R = Evaluate(Out, *ExtendedScopeExpression::As(node)); break;
	default:
		throw std::exception("not added");
		break;
	}
	GetExpressionMode.pop();

	return R;
}

bool SystematicAnalysis::Evaluate(EvaluatedEx& Out, const ExpressionNodeType& node, GetValueMode Mode)
{
	return Evaluate_t(Out, node.Value.get(),Mode);
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
bool SystematicAnalysis::EvalutateStepScopedName(EvaluatedEx& Out, const ScopedNameNode& node, size_t Index, ScopedName::Operator_t OpType, GetMemberTypeSymbolFromVar_t& OtherOut)
{
	return false;
}
bool SystematicAnalysis::CanEvalutateFuncCheck(const Get_FuncInfo& Func)
{
	if (Func._BuiltFunc.has_value())
	{
		return true;
	}

	if (Func.SymFunc && Func.SymFunc->NodePtr)
	{
		const FuncNode* node = FuncNode::As(Func.SymFunc->Get_NodeInfo<Node>());

		if (node->Signature.HasEvalKeyWord)
		{
			return true;
		}

	}


	return false;
}
bool SystematicAnalysis::Evaluate(EvaluatedEx& Out, const TypeSymbol& MustBeType, const ExpressionNodeType& node)
{
	OnExpressionTypeNode(node.Value.get(), GetValueMode::Read);//check
	if (!CanBeImplicitConverted(LastExpressionType, MustBeType, false))
	{
		LogCantCastImplicitTypes(LastLookedAtToken, LastExpressionType, MustBeType, false);
		return false;
	}
	if (!CanEvaluateImplicitConversionConstant(LastExpressionType, MustBeType))
	{
		LogCantCastImplicitTypes_Constant(LastLookedAtToken, LastExpressionType, MustBeType);
		return false;
	}

	EvaluatedEx ex1 = MakeEx(LastExpressionType);
	if (Evaluate_t(ex1, node.Value.get(),GetValueMode::Read))
	{
		return EvaluateImplicitConversion(ex1, MustBeType, Out);
	}
	return false;
}
Optional<SystematicAnalysis::EvaluatedEx> SystematicAnalysis::Evaluate(const TypeSymbol& MustBeType, const ExpressionNodeType& node)
{
	EvaluatedEx Out;
	bool V = Evaluate(Out, MustBeType, node);

	if (V)
	{
		return { Out };
	}
	else
	{
		return {};
	}
}
bool SystematicAnalysis::EvaluateToAnyType(EvaluatedEx& Out, const ExpressionNodeType& node)
{
	OnExpressionTypeNode(node.Value.get(), GetValueMode::Read);//check
	

	EvaluatedEx ex1 = MakeEx(LastExpressionType);
	bool CompilerRet=  Evaluate_t(ex1, node.Value.get(),GetValueMode::Read);
	Out = std::move(ex1);
	return CompilerRet;
}
bool SystematicAnalysis::EvalutateCMPTypesNode(EvaluatedEx& Out, const CMPTypesNode& node)
{
	TypeSymbol Op0 = ConvertAndValidateType(node.TypeOp0, NodeSyb_t::Any);
	TypeSymbol Op1 = ConvertAndValidateType(node.TypeOp1, NodeSyb_t::Any);

	Set_ObjectAs(Out, CMPGetValue(Op0, Op1, node.Op));
	return true;
}
bool SystematicAnalysis::EvalutateValidNode(EvaluatedEx& Out, const ValidNode& node)
{

	bool IsValid = true;
	auto ErrCount = _ErrorsOutput->Get_Errors().size();

	if (node.IsExpression)
	{
		OnExpressionTypeNode(node._ExpressionToCheck.Value.get(), GetValueMode::Read);
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

	Set_ObjectAs(Out, IsValid);

	LastExpressionType = TypesEnum::Bool;
	return true;
}
bool SystematicAnalysis::EvalutateFunc(EvaluatedEx& Out, const FuncCallNode& node)
{
	Get_FuncInfo FuncInfo = GetFunc(node.FuncName,node.Parameters, Get_LookingForType());

	if (CanEvalutateFuncCheck(FuncInfo))
	{
		Vector<EvaluatedEx> ValuePars;
		if (FuncInfo.Func) 
		{
			ValuePars.resize(FuncInfo.Func->Pars.size());

			for (size_t i = 0; i < node.Parameters._Nodes.size(); i++)
			{
				const TypeSymbol& Par = FuncInfo.Func->Pars[i].Type;
				auto& Item = node.Parameters._Nodes[i];

				auto Info = Evaluate(Par, *Item.get());

				if (!Info.has_value())
				{
					return false;
				}

				ValuePars.push_back(std::move(Info.value()));
			}
		}
		return EvalutateFunc(Out, FuncInfo, node.FuncName, ValuePars);
	}
	return false;
}

bool SystematicAnalysis::EvalutateFunc(EvaluatedEx& Out, const Get_FuncInfo& Func, const ScopedNameNode& Name, const Vector<EvaluatedEx>& ValuePars)
{
	
	if (Func.SymFunc)
	{
		AddDependencyToCurrentFile(Func.SymFunc);
	}
	SetFuncRetAsLastEx(Func);


	if (Func._BuiltFunc.has_value())
	{
		auto& BuiltFunc = Func._BuiltFunc.value();
		if (BuiltFunc.EvalObject.has_value())
		{
			auto& EvalObject = BuiltFunc.EvalObject.value();
			Out.Type = LastExpressionType;
			Out.EvaluatedObject = EvalObject;
			return true;
		}	
	}
	else if (Func.SymFunc)
	{
		const FuncNode* Funcnode = FuncNode::As(Func.SymFunc->Get_NodeInfo<Node>());

	
		EvalFuncData* State =new EvalFuncData();
		EvalFuncStackFrames.push_back(Unique_ptr<EvalFuncData>(State));
		constexpr size_t StackSize = 100;
		State->CallFrom = Name.ScopedName.back().token;
		State->FuncSyb = Func.SymFunc;
		bool Ok = false;
		if (EvalFuncStackFrames.size() >= StackSize)
		{
			const Token* token = State->CallFrom;
			String Str;
			Str += "Eval func Stack overflow.";
			Str += "--Call Stack \n";
			for (auto& Item : EvalFuncStackFrames)
			{
				Str += "On line " + std::to_string(Item->CallFrom->OnLine) + (String)" Called '" + Item->FuncSyb->FullName + "'. \n";
			}

			_ErrorsOutput->AddError(ErrorCodes::InValidType, token->OnLine, token->OnPos,Str);
			Ok = false;
		}
		else 
		{
			Ok = EvalutateFunc(*State, Func.SymFunc, ValuePars);
		}
		if (Ok)
		{
			Out.Type = Func.SymFunc->Get_Info<FuncInfo>()->Ret;
			Out.EvaluatedObject = State->Ret;
		}
		EvalFuncStackFrames.pop_back();
		return Ok;
	}

	return false;
}
bool SystematicAnalysis::EvalutateFunc(EvaluatedEx& Out, const TypeSymbol& Type, const Get_FuncInfo& Func, const Vector<EvaluatedEx>& ValuePars)
{
	String B = ToString(Type);
	Token T;
	T.Type = TokenType::Name;
	T.Value._String = B;


	ScopedNameNode Tep;
	ScopedName V;
	V.token = &T;

	Tep.ScopedName.push_back(std::move(V));

	return EvalutateFunc(Out,Func, Tep, ValuePars);
}
bool SystematicAnalysis::Evaluate(EvaluatedEx& Out, const ExtendedScopeExpression& node)
{
	Optional<EvaluatedEx> Ex = EvaluateToAnyType(node.Expression);
	
	if (Ex.has_value()) 
	{
		auto ExpressionType = LastExpressionType;

		GetMemberTypeSymbolFromVar_t V;

		auto ExValue = Ex.value();

		GetExpressionMode.push(GetExpressionMode.top());
		{
			V.Type = ExpressionType;
			V.Symbol = GetSymbol(ExpressionType);


			if (EvalutateStepScopedName(ExValue, node.Extended, 0, node.Operator, V))
			{
				return false;
			}

			for (size_t i = 1; i < node.Extended.ScopedName.size(); i++)
			{
				if (EvalutateStepScopedName(ExValue, node.Extended, i, node.Extended.ScopedName[i].Operator, V))
				{
					return false;
				}
			}
		}
		GetExpressionMode.pop();

		LastExpressionType = V.Type;
		Out = std::move(ExValue);
		return true;
	}
	return false;
}
bool SystematicAnalysis::Evaluate(EvaluatedEx& Out, const ExtendedFuncExpression& node)
{
	Optional<EvaluatedEx> Ex = EvaluateToAnyType(node.Expression);

	auto ExpressionType = LastExpressionType;

	GetExpressionMode.push(GetExpressionMode.top());
	{

		TypeSymbol TypeToStart = ExtendedFuncExpressionGetTypeToStart(ExpressionType, node);



		//Boring boiler plate for Tep ScopedNameNode
		const Token& ToGetLinesFrom = *node.Extended.FuncName.ScopedName.begin()->token;

		ScopedNameNode Name;

		ScopedName TepV;


		Token TepToken;

		TepToken.OnLine = ToGetLinesFrom.OnLine;
		TepToken.OnPos = ToGetLinesFrom.OnPos;
		TepToken.Type = TokenType::Name;

		RemoveTypeattributes(TypeToStart);

		String Buffer = ToString(TypeToStart);

		TepToken.Value._String = Buffer;

		TepV.token = &TepToken;


		TepV.Operator = ScopedName::Operator_t::ScopeResolution;
		Name.ScopedName.push_back(std::move(TepV));

		{

			{
				auto Copy = node.Extended.FuncName.ScopedName.back();
				Name.ScopedName.push_back(std::move(Copy));
			}
			for (size_t i = 1; i < node.Extended.FuncName.ScopedName.size(); i++)
			{
				auto& Item = node.Extended.FuncName.ScopedName[i];
				auto Copy = Item;
				Name.ScopedName.push_back(std::move(Copy));
			}
		}
		//

		ValueParametersNode Pars;
		Pars._Nodes.push_back(Unique_ptr<Node>(node.Expression.Value.get()));

		for (size_t i = 0; i < node.Extended.Parameters._Nodes.size(); i++)
		{
			auto& Item = node.Extended.Parameters._Nodes[i];
			Pars._Nodes.push_back(Unique_ptr<Node>(Item.get()));
		}

		auto FuncInfo = GetFunc(Name,Pars, Get_LookingForType());

		
		Vector<EvaluatedEx> ValuePars;
		bool BadPars = false;
		
		if (CanEvalutateFuncCheck(FuncInfo)) 
		{
			ValuePars.resize(FuncInfo.Func->Pars.size());

			
			for (size_t i = 0; i < Pars._Nodes.size(); i++)
			{
				const TypeSymbol& Par = FuncInfo.Func->Pars[i].Type;
				auto& Item = Pars._Nodes[i];

				auto Info = Evaluate(Par, *Item.get());

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

		return EvalutateFunc(Out, FuncInfo,node.Extended.FuncName, ValuePars);

	}
	GetExpressionMode.pop();
	return false;
}
bool SystematicAnalysis::EvalutateFunc(EvalFuncData& State, const Symbol* Func, const Vector<EvaluatedEx>& Pars)
{



	bool Fail = false;
	auto Conxet = Move_SymbolConext();

	const FuncInfo* funcInfo = Func->Get_Info<FuncInfo>();
	Set_SymbolConext(funcInfo->Conext.value());
	{
		_Table.AddScope("__EvalFunc");
		
		const FuncNode& Body = *Func->Get_NodeInfo<FuncNode>();
		
		State.Pars.reserve(Pars.size());
		for (size_t i = 0; i < State.Pars.size(); i++)
		{
			auto ID = GetSymbolID(Body.Signature.Parameters.Parameters[i]);
			State.Pars.AddValue(ID, Pars[i].EvaluatedObject);
		}
		State.Ret.ObjectSize = GetSize(funcInfo->Ret).value();
		State.Ret.Object_AsPointer.reset(new Byte[State.Ret.ObjectSize]);
		State.FuncSyb = Func;
		//

		
		for (auto& Item : Body.Body.value().Statements._Nodes)
		{
			auto Statement = Item.get();

			if (!EvalutateStatement(State, Statement))
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
bool SystematicAnalysis::EvalutateStatement(EvalFuncData& State, const Node* node)
{
	switch (node->Get_Type())
	{
	case NodeType::RetStatementNode:
	{
		const RetStatementNode* Node = RetStatementNode::As(node);
		auto Val = Evaluate(State.Get_funcInfo()->Ret, Node->Expression);
		if (Val.has_value()) {
			State.Ret = std::move(Val.value().EvaluatedObject);
		}
		return Val.has_value();
	}
	default:
		return false;
		throw std::exception("bad path");
		break;
	}
	
}
bool SystematicAnalysis::EvalutateScopedName(EvaluatedEx& Out, size_t Start, size_t End, const ScopedNameNode& node, GetMemberTypeSymbolFromVar_t& OtherOut)
{
	GetExpressionMode.push(GetValueMode::Read);
	GetMemberTypeSymbolFromVar_t V;
	auto RetBool = GetMemberTypeSymbolFromVar(Start,End,node, V);
	GetExpressionMode.pop();

	LastExpressionType = V.Type;

	if (V.Symbol)
	{
		if (V.Symbol->Type == SymbolType::ConstantExpression)
		{
			ConstantExpressionInfo* Info = V.Symbol->Get_Info<ConstantExpressionInfo>();
			Out.EvaluatedObject = Info->Ex;
			Out.Type = V.Type;
			return true;
		}
	}
	return false;
}
Optional<SystematicAnalysis::EvaluatedEx> SystematicAnalysis::EvaluateToAnyType(const ExpressionNodeType& node)
{
	EvaluatedEx Out;
	bool V = EvaluateToAnyType(Out, node);

	if (V)
	{
		return { Out };
	}
	else
	{
		return {};
	}
}
String SystematicAnalysis::ToString(const TypeSymbol& Type, const RawEvaluatedObject& Data)
{
	auto DataPtr = Get_Object(Type, Data);

	

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

	case TypesEnum::sIntPtr:
	case TypesEnum::uIntPtr:
		if (_Settings->PtrSize == IntSizes::Int64)
		{
			return LookingAtIRBlock->NewLoad(*(UInt64*)ObjectData);
		}
		else
		{
			return LookingAtIRBlock->NewLoad(*(UInt32*)ObjectData);
		}
	case TypesEnum::CustomType:
	{
		auto Syb = GetSymbol(ValueType._CustomTypeSymbol);
		if (Syb)
		{
			if (Syb->Type == SymbolType::Type_StaticArray)
			{
				StaticArrayInfo* Info = Syb->Get_Info<StaticArrayInfo>();
				auto R = LookingAtIRBlock->NewLoad(ConvertToIR(ValueType));
				auto Base = Info->Type;
				auto BaseSize = GetSize(Base).value();

				auto Ptr = LookingAtIRBlock->NewLoadPtr(R);

				RawEvaluatedObject _DataAsIndex;
				_DataAsIndex.ObjectSize = BaseSize;
				_DataAsIndex.Object_AsPointer.reset(new Byte[BaseSize]);
				auto BaseAsIR = LookingAtIRBlock->NewLoad(BaseSize);
				for (size_t i = 0; i < Info->Count; i++)
				{
					void* ItemOffset = Value.Object_AsPointer.get() + (BaseSize * i);
					Set_ObjectAs(Base,_DataAsIndex, ItemOffset, BaseSize);

					auto ItemIR = LoadEvaluatedEx(_DataAsIndex, Base);
					
					LookingAtIRBlock->New_Index_Vetor(Ptr, LookingAtIRBlock->NewLoad(i), BaseAsIR);
				}

				return R;
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
	Vector<ClassMethod::Par> Vect;
	for (auto& Item : Func->Pars)
	{
		ClassMethod::Par V;
		V.Type = ConvertToTypeInfo(Item.Type);
		V.IsOutPar = Item.IsOutPar;
		Vect.push_back(V);
	}

	return NameDecoratior::GetDecoratedName(Func->FullName, Vect);
}

IRidentifierID SystematicAnalysis::GetIRID(const FuncInfo* Func)
{
	auto FuncName = MangleName(Func);
	return _Builder.ToID(FuncName);
}

GenericData::Type SystematicAnalysis::GenericTypeToGenericDataType(GenericValueNode::GenericType type)
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
		throw std::exception("bad");
		break;
	}
}

void SystematicAnalysis::LogInvalidNodeError(const Token* Token, String_view ErrStr)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "Invaild:" + (String)ErrStr);
}
void SystematicAnalysis::LogEmptyInvalidError(const Token* Token)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "Reached Invaild Statemet");
}

void SystematicAnalysis::LogCantCastImplicitTypes(const Token* Token, const TypeSymbol& Ex1Type, const TypeSymbol& UintptrType, bool ReassignMode)
{
	if (Ex1Type.IsBadType() || UintptrType.IsBadType()
		|| IsUnMapType(UintptrType) || IsUnMapType(Ex1Type)) { return; }

	bool V1 = IsAddessAndLValuesRulesfollowed(Ex1Type, UintptrType, ReassignMode);
	if (!V1 || CanDoTypeToTrait(Ex1Type, UintptrType))
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
void SystematicAnalysis::LogReadingFromInvaidVariable(const Token* Token, String_view Str)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "the variable named '" + (String)Str + "'" + " cant be read from you.can not read an invaid variable");
}
void SystematicAnalysis::LogCantFindVarError(const Token* Token, String_view Str)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Cant find Variable Named '" + (String)Str + "'");
}
void SystematicAnalysis::LogCantFindVarMemberError(const Token* Token, String_view Str, const TypeSymbol& OnType)
{
	if (OnType.IsBadType() || IsUnMapType(OnType)) { return; }

	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Cant find Member Named '" + (String)Str + "' on type '" + ToString(OnType) +"'");
}

void SystematicAnalysis::LogCantFindCompoundOpForTypes(const Token* BinaryOp, TypeSymbol& Ex0Type, TypeSymbol& Ex1Type)
{
	if (Ex1Type.IsBadType() || Ex0Type.IsBadType() 
		|| IsUnMapType(Ex0Type) || IsUnMapType(Ex1Type)) { return; }

	LogError(ErrorCodes::InValidType, BinaryOp->OnLine, BinaryOp->OnPos,
		"The type '" + ToString(Ex0Type) + "'" + " cant be '"
		+ ToString(BinaryOp->Type) + "' with '" + ToString(Ex0Type) + "'");
}

void SystematicAnalysis::LogCantFindPostfixOpForTypes(const Token* BinaryOp, TypeSymbol& Ex0Type)
{
	if (Ex0Type.IsBadType() || IsUnMapType(Ex0Type)) { return; }

		LogError(ErrorCodes::InValidType, BinaryOp->OnLine, BinaryOp->OnPos,
			"The type '" + ToString(Ex0Type) + "'" + " cant be '"
			+ ToString(BinaryOp->Type) + "'");
}
void SystematicAnalysis::LogCantFindBinaryOpForTypes(const Token* BinaryOp, TypeSymbol& Ex0Type, TypeSymbol& Ex1Type)
{
	if (Ex1Type.IsBadType() || Ex0Type.IsBadType()
		|| IsUnMapType(Ex1Type) || IsUnMapType(Ex0Type)) { return; }

	LogError(ErrorCodes::InValidType, BinaryOp->OnLine, BinaryOp->OnPos,
		"The type '" + ToString(Ex0Type) + "'" + " cant be '"
		+ ToString(BinaryOp->Type) + "' with '" + ToString(Ex1Type) + "'");
}
void SystematicAnalysis::ExpressionMustbeAnLocationValueError(const Token* Token, TypeSymbol& Ex0Type)
{
	if (Ex0Type.IsBadType() || IsUnMapType(Ex0Type)) { return; }
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos,
		"expression must be an Location not an Value'" + ToString(Ex0Type) + "'");
}
void SystematicAnalysis::YouMustReturnSomethingError(const Token* Token)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos
		, "you must return something");
}
void SystematicAnalysis::CantguessVarTypeError(const Token* Token)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos
		, "cant guess 'var' type");
}
void SystematicAnalysis::CantUseThisKeyWordHereError(const Token* NameToken)
{
	LogError(ErrorCodes::InValidType, NameToken->OnLine, NameToken->OnPos, "cant use this here");
}
void SystematicAnalysis::CantgussTypesTheresnoassignment(const Token* Token)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos
		, "cant guess type theres no '=' [expression]");
}
void SystematicAnalysis::LogCantCastExplicityTypes(const Token* Token, TypeSymbol& Ex0Type, TypeSymbol& ToTypeAs)
{
	if (Ex0Type.IsBadType() || ToTypeAs.IsBadType()
		|| IsUnMapType(Ex0Type) || IsUnMapType(ToTypeAs)){return;}

	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
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

	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, Text);
}
void SystematicAnalysis::LogCantFindFuncError(const Token* Token, String_view FuncName,
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
void SystematicAnalysis::LogCantFindPostfixOpForTypes_Constant(const Token* BinaryOp, TypeSymbol& Ex0Type)
{
	LogCantFindPostfixOpForTypes(BinaryOp, Ex0Type);
}
void SystematicAnalysis::LogCantDoPostfixOpForTypes_Constant(const Token* BinaryOp, TypeSymbol& Ex0Type)
{
	LogError(ErrorCodes::InValidName, BinaryOp->OnLine, BinaryOp->OnPos
		, "The Type operation '" + ToString(Ex0Type) + "' must be an compile time constant.");
}
void SystematicAnalysis::LogCantCastImplicitTypes_Constant(const Token* Token,const TypeSymbol& Ex1Type, const TypeSymbol& UintptrType)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Casting Type '" + ToString(Ex1Type) + " to '" + ToString(UintptrType) + "' cant be done at compile time.");
}
void SystematicAnalysis::LogCantFindNamespace(const Token* Token, const String_view Namespace)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos
		, "the cant find the Namespace '" + (String)Namespace + "'.");
}
void SystematicAnalysis::LogTypeMustBeAnConstantExpressionAble(const Token* Token, const TypeSymbol& Type)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos
		, "the type " + ToString(Type) + " must be an Constant Expression able type'");
}
void SystematicAnalysis::LogCantModifyiMutableError(const Token* Token, String_view Name)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Cant modify '" + (String)Name + "' it's immutable");
}
void SystematicAnalysis::LogCantFindTypeError(const Token* Token, String_view Name)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
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
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, Msg);

	
}
void SystematicAnalysis::LogCantUseThisHere(const Token* Token)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
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

	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
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

	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, Msg);
}
void SystematicAnalysis::LogFuncMustBe(const Token* Token, const String_view FuncName, TypeSymbol& TypeSybToBe)
{

	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		,"The function named " + (String)FuncName + " must return to the type of '" + ToString(TypeSybToBe) + '\'');
}
void SystematicAnalysis::LogSymbolRedefinition(const Token* Token, const Symbol* Symbol)
{

	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "the symbol '"+  Symbol->FullName + "\' is already define. try useing a different name");
}
void SystematicAnalysis::LogUseingVarableBeforDeclared(const Token* Token)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "trying to use the varable '" + (String)Token->Value._String + "\' before its defined.");
}
void SystematicAnalysis::LogBeMoreSpecifiicForRetType(const String_view FuncName, const Token* Token)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "be more Specifiic For return Type. like |" + (String)FuncName + "[...] -> [Type]; or give the funcion a body.");
}
void SystematicAnalysis::LogCantBeIndexWithType(const Token* Token, const  TypeSymbol& Ex0Type, const  TypeSymbol& IndexType)
{
	if (Ex0Type.IsBadType() || IndexType.IsBadType()) { return; }

	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "The Type '" + ToString(Ex0Type) + "\' Cant be Index with '" + ToString(IndexType) + "'.");
}
void SystematicAnalysis::LogCantUseThisInStaticFunction(const Token* Token)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
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
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, Msg);

}
void SystematicAnalysis::LogGenericInputWantsaExpressionNotType(const Token* Token, const String_view NameOfPar)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Generic The generic parameter '" + (String)NameOfPar + "' Wants a Expression not a Type.");
}
void SystematicAnalysis::LogGenericInputWantsaTypeNotExpression(const Token* Token, const String_view NameOfPar)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Generic The generic parameter '" + (String)NameOfPar + "'Type Wants a  not a Expression.");

}

void SystematicAnalysis::LogExpectedSymbolToBea(const Token* Token, const Symbol& Syb, SymbolType Value)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Did not expect Symbol the Symbol '" + Syb.FullName + "' to be '" + ToString(Syb.Type) + 
		"' .Expected '" + ToString(Value) + '\'');

}
String SystematicAnalysis::ToString(SymbolType Value)
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

	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, Msg);
}
void SystematicAnalysis::LogBeMoreSpecifiicWithStaticArrSize(const Token* Token, const TypeSymbol& Type)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos
		, "Be More Specifiic with Static Array Size.Ex: " + ToString(Type) + "[/1]");
}
void SystematicAnalysis::LogWantedAVariable(const Token* const& Item,Symbol* TepSyb)
{
	LogError(ErrorCodes::BackEndError, Item->OnLine, Item->OnPos,
		"found a " + ToString(TepSyb->Type) + "(" + TepSyb->FullName + ")" + ".but wanted a Variable or a class field");
}

void SystematicAnalysis::LogBinaryOverloadPars(const Token& Name, const FuncInfo* Func)
{
	LogError(ErrorCodes::InValidType, Name.OnLine, Name.OnPos
		, "The Binary Overload '" + ToString(Name.Type) + "'" + " must have 2 paameters it has " + std::to_string(Func->Pars.size()) + " Pameters");
}
void SystematicAnalysis::LogIndexOverloadPars(const Token& Name, const FuncInfo* Func)
{
	LogError(ErrorCodes::InValidType, Name.OnLine, Name.OnPos
		, "The Index Overload '" + ToString(Name.Type) + "'" + " must have 2 paameters it has " + std::to_string(Func->Pars.size()) + " Pameters");
}
void SystematicAnalysis::LogPostfixOverloadPars(const Token& Name, const FuncInfo* Func)
{
	LogError(ErrorCodes::InValidType, Name.OnLine, Name.OnPos
		, "The Index Overload '" + ToString(Name.Type) + "'" + " must have 1 paameters it has " + std::to_string(Func->Pars.size()) + " Pameters");
}

void SystematicAnalysis::LogCantOverLoadOverload(const UCodeLang::Token* NameToken)
{
	LogError(ErrorCodes::InValidName, NameToken->OnLine, NameToken->OnPos, "You may not Overload '" + ToString(NameToken->Type) + "'.");
}

void SystematicAnalysis::LogCantFindMemberOverloadForType(const Token* Item, TokenType Op, const TypeSymbol& Out)
{
	LogError(ErrorCodes::InValidName, Item->OnLine, Item->OnPos
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
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos,
		Msg);
}
void SystematicAnalysis::LogCantUseMoveTypeHere(const UCodeLang::Token* Token)
{

	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "Cant use moved Type Here.it can only be used in Parameters");
}
void SystematicAnalysis::LogDynamicMustBeRrait(const TypeNode& V,const TypeSymbol& Out)
{
	LogError(ErrorCodes::InValidType, V.Name.Token->OnLine, V.Name.Token->OnPos, "useing a Dynamic type on a none trait the type found '" + ToString(Out) + "'");
}
void SystematicAnalysis::TraitCantBeAlone(const UCodeLang::Token* Token)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "the Type uses a Trait.but Traits cant be use alone.");
}

void SystematicAnalysis::LogWantedAType(const UCodeLang::FrontEnd::TypeNode& V, UCodeLang::FrontEnd::Symbol* SybV)
{
	LogError(ErrorCodes::InValidType, V.Name.Token->OnLine, V.Name.Token->OnPos,
		"found a '" + ToString(SybV->Type) + "' for the Symbol " + SybV->FullName + " but wanted a type");
}
void SystematicAnalysis::LogOutCanOnlyBeInControlFlow(const UCodeLang::Token* Token)
{

	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "The 'out' can only be used in Control flow like if");
}
void SystematicAnalysis::LogParamterMustBeAnOutExpression(const UCodeLang::Token* Token, const size_t& i)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "parameter '" + std::to_string(i) + "' does not use the out keyword");
}
void SystematicAnalysis::LogParPackIsNotLast(const UCodeLang::Token* Token)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "Parameter  named '" + (String)Token->Value._String + "' is useing a Parameter pact.But Parameter pact must be last Paramter");
}
void SystematicAnalysis::LogParPackTypeIsNotLast(const UCodeLang::Token* Token)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "Type Pack named '" + (String)Token->Value._String + "' is not declarded last.");
}
void SystematicAnalysis::LogUseingTypeinfoInNonEvalVarable(const UCodeLang::Token* Token)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos, "Trying to use typeinfo in a Non-eval Varable");
}
void SystematicAnalysis::LogUseingTypeinfoInEvalFuncPar(const UCodeLang::Token* Token)
{
	LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos, "Trying to use typeinfo in a Non-eval Func");
}
void SystematicAnalysis::LogCantOutputTypeinfo(const UCodeLang::Token* Token)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "Cant Output IR of an typeinfo. place this in an eval funcion or an eval varable");
}
void SystematicAnalysis::LogCantUseTypeVoidHere(const UCodeLang::Token* Token)
{
	LogError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "Cant use type void here");
}
void SystematicAnalysis::LogError(ErrorCodes Err,const String& MSG, const Token* Token)
{
	LogError(Err, Token->OnLine, Token->OnPos, MSG);
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


