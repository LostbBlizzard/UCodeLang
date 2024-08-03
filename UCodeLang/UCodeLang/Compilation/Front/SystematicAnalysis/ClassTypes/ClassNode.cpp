#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

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
		Symbol_RedefinitionCheck((String_view)_Table._Scope.ThisScope, SymbolType::Type_class, NeverNullptr(Node._className.token));
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

	Syb.PassState = _PassType;

	OnAttributesNode(Node._Attributes);

	ClassInfo* ClassInf = nullptr;
	if (_PassType == PassType::GetTypes)
	{
		_Table.AddSymbolID(Syb, SybID);
		Syb.NodePtr = Node.As();



		ClassInf = new ClassInfo();

		ClassInf->Context = Save_SymbolContextRemoveOneScopeName();
		ClassInf->FullName = Syb.FullName;
		Syb.Info.reset(ClassInf);
		Syb.VarType.SetType(Syb.ID);

		ClassInf->_IsExternalC = Node.IsExternCStruct;
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

			AddClassExtraInfo Info;
			Info.IsgenericInstantiation  = IsgenericInstantiation;

			Assembly_AddClass(Node._Attributes, &Syb,Info);
		}

		CaptureErrorContext errorcontext = GetErrorCaptureContext();

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

				const auto& Funcs = GetSymbolsWithName((String)ClassConstructorfunc, SymbolType::Func);
				for (auto& Item : Funcs)
				{
					if (Item->Type == SymbolType::Func)
					{
						FuncInfo* V = Item->Get_Info<FuncInfo>();
						if (V->Pars.size() == 1)
						{
							auto& ThisPar = V->Pars[0];
							bool isthisfunc = ThisPar.Type.IsAddress() && ThisPar.IsOutPar == false && ThisPar.Type._CustomTypeSymbol == SybID;
							if (isthisfunc)
							{
								HasDefaultConstructor = true;
							}
						}
						if (V->Pars.size() == 2)
						{
							auto& ThisPar = V->Pars[0];
							bool isthisfunc = ThisPar.Type.IsAddress() && ThisPar.IsOutPar == false && ThisPar.Type._CustomTypeSymbol == SybID;
							if (isthisfunc) 
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

					DropFunc->PassState = PassType::BuidCode;
				}
				
				if (!HasCopyConstructor && ClassInf->_ClassAutoGenerateCopyConstructor)
				{
					auto DropFunc = &Symbol_AddSymbol(SymbolType::Func, ClassConstructorfunc, _Table._Scope.GetApendedString((String)ClassConstructorfunc), AccessModifierType::Public);
					_Table.AddSymbolID(*DropFunc,Symbol_GetSymbolID(DropFunc));
					FuncInfo* V = new FuncInfo();
					DropFunc->Info.reset(V);

					V->FullName = DropFunc->FullName;
					V->_FuncType = FuncInfo::FuncType::New;
					V->Ret = TypesEnum::Void;
					V->FrontParIsUnNamed = true;

					auto ThisParType = ThisCallType;
					ThisParType._IsAddress = true;
					V->Pars.push_back({ false,ThisParType });
					
					auto ThisParType2 = ThisCallType;
					ThisParType2._Isimmutable = true;
					ThisParType2._IsAddress = true;
					V->Pars.push_back({ false,ThisParType2 });


					ClassInf->_AutoGenerateCopyConstructor = DropFunc->ID;
					DropFunc->PassState = PassType::BuidCode;
				}

				if (!HasMoveConstructor && ClassInf->_ClassAutoGenerateMoveConstructor)
				{
					auto DropFunc = &Symbol_AddSymbol(SymbolType::Func, ClassConstructorfunc, _Table._Scope.GetApendedString((String)ClassConstructorfunc), AccessModifierType::Public);
					_Table.AddSymbolID(*DropFunc, Symbol_GetSymbolID(DropFunc));
					FuncInfo* V = new FuncInfo();
					DropFunc->Info.reset(V);

					V->FullName = DropFunc->FullName;
					V->_FuncType = FuncInfo::FuncType::New;
					V->Ret = TypesEnum::Void;
					V->FrontParIsUnNamed = true;

					auto ThisParType = ThisCallType;
					ThisParType._IsAddress = true;
					V->Pars.push_back({ false,ThisParType });

					auto ThisParType2 = ThisCallType;
					ThisParType2._MoveData = MoveData::Moved;
					V->Pars.push_back({ false,ThisParType2 });

					ClassInf->_AutoGenerateMoveConstructor = DropFunc->ID;
					DropFunc->PassState = PassType::BuidCode;
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
				auto InheritSyb = SybOp.value();
				if (Item._generic._Values.size() == 0)
				{
					if (InheritSyb->Type != SymbolType::Trait_class)
					{
						LogError_ExpectedSymbolToBea(NeverNullptr(Item._Name.token), *InheritSyb, SymbolType::Trait_class);
						continue;
					}
				}
				else
				{
					if (InheritSyb->Type != SymbolType::Generic_Trait)
					{
						LogError_ExpectedSymbolToBea(NeverNullptr(Item._Name.token), *InheritSyb, SymbolType::Generic_Trait);
						continue;
					}

					//if (CheckgenericForErr) { continue; }

					Symbol_Update_TraitSym_ToFixedTypes(InheritSyb);

					if (InheritSyb->ValidState == SymbolValidState::Invalid)
					{
						Syb.ValidState = SymbolValidState::Invalid;
						continue;
					}

					auto CInfo = InheritSyb->Get_Info<TraitInfo>();
					auto classnode = TraitNode::As(InheritSyb->Get_NodeInfo<UCodeLang::Node>());
					auto trait = Generic_InstantiateOrFindGeneric_Trait(NeverNullptr(Item._Name.token), InheritSyb, classnode->_generic, CInfo->_GenericData, Item._generic);
					if (!trait.has_value()) { continue; }
					
					InheritSyb = trait.value();
				}

				/*
				for (auto& Item2 : ClassInf->_InheritedTypes)
				{
					if (InheritSyb == Item2)
					{
						LogError(ErrorCodes::InValidType, Item.Token->OnLine, Item.Token->OnPos,
							"duplicate Inherit Trait");
					}
				}
				*/


				ClassInfo_InheritTypeInfo _Data;
				_Data.Syb = InheritSyb.value();
				ClassInf->_InheritedTypes.push_back(_Data);


			}

			for (auto& Item : ClassInf->_InheritedTypes)
			{
				Symbol_InheritTrait(&Syb, ClassInf, Item.Syb, NeverNullptr(Node._className.token));
			}
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
		if (Isgeneric_t) 
		{
			bool hasanyerrors = ErrorCaptureHasErrors(errorcontext);
			
			if (hasanyerrors)
			{
				Syb.ValidState = SymbolValidState::Invalid;
			}
		}
	}
	else
	{
		Syb.NodePtr = Node.As();

	}




	if (_PassType == PassType::FixedTypes)//auto make funcs
	{
		CaptureErrorContext errorcontext = GetErrorCaptureContext();
		if (Isgeneric_t)
		{
			auto& GenericList = Node._generic;
			Generic_GenericAliasFixTypes(GenericList, IsgenericInstantiation, ClassInf->_GenericData);
		}



		{
			const FieldInfo* bigestoffsetfield = nullptr;
			Optional<size_t> bigestoffset;
			for (auto& cfield : ClassInf->Fields)
			{
				auto offset = Type_GetOffset(*ClassInf, &cfield).value();
				if (offset > bigestoffset || !bigestoffset.has_value())
				{
					bigestoffset = offset;
					bigestoffsetfield = &cfield;
				}
			}

			if (bigestoffset.has_value()) {
				ClassInf->Size = bigestoffset.value();
				ClassInf->Size += Type_GetSize(bigestoffsetfield->Type).value_or(0);
			}
			else
			{
				ClassInf->Size = 0;
			}
		}

		bool ispublic = Node._Access == AccessModifierType::Public;
		if (Node._IsExport && ispublic)
		{
			for (size_t i = 0; i < ClassInf->Fields.size(); i++)
			{
				auto& cfield = ClassInf->Fields[i];

				bool ispublic = false;
				const Token* t = nullptr;
				{
					size_t fieldcount = 0;
					for (auto& Item : Node._Nodes)
					{
						if (Item->Get_Type() == NodeType::DeclareVariableNode)
						{
							auto* declar = DeclareVariableNode::As(Item.get());
							if (fieldcount == i)
							{
								t = declar->_Name.token;
								ispublic = declar->_Access == AccessModifierType::Public;
								break;
							}
							fieldcount++;
						}
					}
				}

				if (ispublic)
				{
					if (!Type_IsTypeExported(cfield.Type))
					{


						LogError_TypeIsNotExport(NeverNullptr(t), cfield.Type, NeverNullptr(&Syb));
					}
				}
			}
		}
		if (!ispublic && Node._IsExport)
		{
			LogError_ExportIsPrivate(NeverNullptr(Node._className.token), NeverNullptr(&Syb));
		}

		if (Isgeneric_t)
		{
			bool hasanyerrors = ErrorCaptureHasErrors(errorcontext);

			if (hasanyerrors)
			{
				Syb.ValidState = SymbolValidState::Invalid;
			}
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

				auto FuncSyb = Symbol_GetSymbol(RemoveSymboolFuncOverloadMangling(_Table._Scope.GetApendedString((String)ClassDestructorFunc)), SymbolType::Func).value();
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
					auto ClassInit = RemoveSymboolFuncOverloadMangling(_Table._Scope.GetApendedString((String)ClassDeInitializefuncName));

					_IR_LookingAtIRBlock->NewPushParameter(_IR_LookingAtIRBlock->NewLoad(&_IR_LookingAtIRFunc->Pars.front()));
					_IR_LookingAtIRBlock->NewCall(_IR_Builder.ToID(ClassInit));


				}_IR_LookingAtIRBlock->NewRet();

				//
				_IR_LookingAtIRFunc = OldFunc;
				_IR_LookingAtIRBlock = OldBlock;

				
				{
					ClassMethod V2;
					V2.FullName = Funcinfo->FullName;
					V2.DecorationName = IR_MangleName(Funcinfo);
					V2.RetType = Assembly_ConvertToType(Funcinfo->Ret);


					V2.IsThisFunction = Funcinfo->FrontParIsUnNamed;
					V2.IsExternC = Funcinfo->IsExternC;
					V2.IsUnsafe = Funcinfo->IsUnsafe;
					V2.IsRemoved = Funcinfo->IsRemoved;
					V2.IsExport = Node._IsExport;

					for (size_t i = 0; i < Funcinfo->Pars.size(); i++)
					{
						auto& Item = Funcinfo->Pars[i];

						auto& F = V2.ParsType.emplace_back();
						F.IsOutPar = Item.IsOutPar;
						F.Type = Assembly_ConvertToType(Item.Type);
					}

					this->_ClassStack.push({});//the push and pop is dumb fix
					Class_Data* Ptr = Assembly_GetAssemblyClass(ScopeHelper::ApendedStrings(Syb.FullName, "n/a"));
					this->_ClassStack.pop();

					Ptr->Methods.push_back(std::move(V2));
				}
				
			}
			

			if (ClassInf->_WillHaveFielddeInit)
			{//de-init func
				auto ThisCallType = TypeSymbol(Syb.ID);

				FuncInfo V = FuncInfo();
				{
					V.FullName = RemoveSymboolFuncOverloadMangling(_Table._Scope.GetApendedString((String)ClassDeInitializefuncName));
					V._FuncType = FuncInfo::FuncType::Drop;
					V.Ret = TypesEnum::Void;
					V.FrontParIsUnNamed = true;

					auto ThisParType = ThisCallType;
					ThisParType._IsAddress = true;
					V.Pars.push_back({ false,ThisParType });
				}


				auto OldFunc = _IR_LookingAtIRFunc;
				auto OldBlock = _IR_LookingAtIRBlock;
				//
				_IR_LookingAtIRFunc = _IR_Builder.NewFunc(V.FullName, IR_ConvertToIRType(V.Ret));
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
				
				{
					auto Funcinfo = &V;
					ClassMethod V2;
					V2.FullName = Funcinfo->FullName;
					V2.DecorationName = Funcinfo->FullName;
					V2.RetType = Assembly_ConvertToType(Funcinfo->Ret);


					V2.IsThisFunction = Funcinfo->FrontParIsUnNamed;
					V2.IsExternC = Funcinfo->IsExternC;
					V2.IsUnsafe = Funcinfo->IsUnsafe;
					V2.IsRemoved = Funcinfo->IsRemoved;
					V2.IsExport = Node._IsExport;

					for (size_t i = 0; i < Funcinfo->Pars.size(); i++)
					{
						auto& Item = Funcinfo->Pars[i];

						auto& F = V2.ParsType.emplace_back();
						F.IsOutPar = Item.IsOutPar;
						F.Type = Assembly_ConvertToType(Item.Type);
					}

					this->_ClassStack.push({});//the push and pop is dumb fix
					Class_Data* Ptr = Assembly_GetAssemblyClass(ScopeHelper::ApendedStrings(Syb.FullName, "n/a"));
					this->_ClassStack.pop();

					Ptr->Methods.push_back(std::move(V2));
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
					V.FullName = RemoveSymboolFuncOverloadMangling(_Table._Scope.GetApendedString((String)ClassConstructorfunc));
					V._FuncType = FuncInfo::FuncType::New;
					V.Ret = TypesEnum::Void;
					V.FrontParIsUnNamed = true;

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
						auto ClassInit = RemoveSymboolFuncOverloadMangling(_Table._Scope.GetApendedString((String)ClassInitializefuncName));

						_IR_LookingAtIRBlock->NewPushParameter(_IR_LookingAtIRBlock->NewLoad(&_IR_LookingAtIRFunc->Pars.front()));
						_IR_LookingAtIRBlock->NewCall(_IR_Builder.ToID(ClassInit));


					}_IR_LookingAtIRBlock->NewRet();
				}

				
				
				{
					ClassMethod V2;
					V2.FullName = RemoveSymboolFuncOverloadMangling(V.FullName);
					V2.DecorationName = RemoveSymboolFuncOverloadMangling(_IR_Builder.FromID(_IR_LookingAtIRFunc->identifier));
					V2.RetType = Assembly_ConvertToType(V.Ret);


					V2.IsThisFunction = V.FrontParIsUnNamed;
					V2.IsExternC = V.IsExternC;
					V2.IsUnsafe = V.IsUnsafe;
					V2.IsRemoved = V.IsRemoved;
					V2.IsExport = Node._IsExport;

					for (size_t i = 0; i < V.Pars.size(); i++)
					{
						auto& Item = V.Pars[i];

						auto& F = V2.ParsType.emplace_back();
						F.IsOutPar = Item.IsOutPar;
						F.Type = Assembly_ConvertToType(Item.Type);
					}

					Class_Data* Ptr = _Lib.Get_Assembly().Find_Class(RemoveSymboolFuncOverloadMangling(Syb.FullName));
					Ptr->Methods.push_back(std::move(V2));
				}
				//
				_IR_LookingAtIRFunc = OldFunc;
				_IR_LookingAtIRBlock = OldBlock;


			}
			

			if (ClassInf->_ClassFieldInit)
			{
				ClassInf->_ClassFieldInit->Blocks[0]->NewRet();

				String funcName = _Table._Scope.GetApendedString((String)ClassInitializefuncName);

				ClassMethod V2;
				V2.FullName = funcName;
				V2.DecorationName = funcName;
				V2.RetType = ReflectionTypes::Void;
				V2.IsThisFunction = true;
				V2.IsExport = Node._IsExport;

				ClassMethod::Par p;
				p.Type = Assembly_ConvertToType(TypeSymbol(Syb.ID));
				p.Type._IsAddress = true;
				V2.ParsType.push_back(std::move(p));

				this->_ClassStack.push({});//the push and pop is dumb fix
				Class_Data* Ptr = Assembly_GetAssemblyClass(ScopeHelper::ApendedStrings(Syb.FullName,"n/a"));
				this->_ClassStack.pop();
				
				Ptr->Methods.push_back(std::move(V2));
			}
	
			if (!ClassInf->_ClassHasCopyConstructor && ClassInf->_ClassAutoGenerateCopyConstructor)
			{
				auto ThisCallType = TypeSymbol(Syb.ID);
				auto sym = Symbol_GetSymbol(ClassInf->_AutoGenerateCopyConstructor.value());
				const FuncInfo& V = *sym->Get_Info<FuncInfo>();
				


				auto OldFunc = _IR_LookingAtIRFunc;
				auto OldBlock = _IR_LookingAtIRBlock;
				//
				auto funcirid = IR_MangleName(&V);
				_IR_LookingAtIRFunc = _IR_Builder.NewFunc(_IR_Builder.ToID(funcirid), IR_ConvertToIRType(V.Ret));
				IRType ThisPar = IR_ConvertToIRType(V.Pars.front());
				{
					IRPar par = IRPar();
					par.identifier = _IR_Builder.ToID(ThisSymbolName);
					par.type = ThisPar;
					_IR_LookingAtIRFunc->Pars.push_back(par);

					IRPar par2 = IRPar();
					par2.identifier = _IR_Builder.ToID("CopyFrom");
					par2.type = ThisPar;
					_IR_LookingAtIRFunc->Pars.push_back(par2);
				}

				_IR_LookingAtIRBlock = _IR_LookingAtIRFunc->NewBlock(".");
				{
					for (size_t i = ClassInf->Fields.size() - 1; i != (Vector<FieldInfo>::size_type) - 1; i--)
					{
						auto& Item = ClassInf->Fields[i];
						auto v = HasCopyContructer(Item.Type);
						auto par1 = _IR_LookingAtIRBlock->New_Member_Dereference(&_IR_LookingAtIRFunc->Pars.front(), ThisPar, i);
						auto par2 = _IR_LookingAtIRBlock->New_Member_Dereference(&_IR_LookingAtIRFunc->Pars[1], ThisPar, i);

						if (v && !Item.Type.IsAddress())
						{
							auto info = v.value()->Get_Info<FuncInfo>();

							_IR_LookingAtIRBlock->NewPushParameter(_IR_LookingAtIRBlock->NewLoadPtr(par1));
							_IR_LookingAtIRBlock->NewPushParameter(_IR_LookingAtIRBlock->NewLoadPtr(par2));
							_IR_LookingAtIRBlock->NewCall(IR_GetIRID(info));
						}
						else
						{
							_IR_LookingAtIRBlock->NewStore(par1,par2);
						}
					}
					_IR_LookingAtIRBlock->NewRet();
				}

				{
					auto Funcinfo = &V;
					ClassMethod V2;
					V2.FullName = Funcinfo->FullName;
					V2.DecorationName = funcirid;
					V2.RetType = Assembly_ConvertToType(Funcinfo->Ret);


					V2.IsThisFunction = Funcinfo->FrontParIsUnNamed;
					V2.IsExternC = Funcinfo->IsExternC;
					V2.IsUnsafe = Funcinfo->IsUnsafe;
					V2.IsRemoved = Funcinfo->IsRemoved;
					V2.IsExport = Node._IsExport;

					for (size_t i = 0; i < Funcinfo->Pars.size(); i++)
					{
						auto& Item = Funcinfo->Pars[i];

						auto& F = V2.ParsType.emplace_back();
						F.IsOutPar = Item.IsOutPar;
						F.Type = Assembly_ConvertToType(Item.Type);
					}

					this->_ClassStack.push({});//the push and pop is dumb fix
					Class_Data* Ptr = Assembly_GetAssemblyClass(ScopeHelper::ApendedStrings(Syb.FullName, "n/a"));
					this->_ClassStack.pop();

					Ptr->Methods.push_back(std::move(V2));
				}
				//
				_IR_LookingAtIRFunc = OldFunc;
				_IR_LookingAtIRBlock = OldBlock;

			}
			
			if (!ClassInf->_ClassHasMoveConstructor && ClassInf->_ClassAutoGenerateMoveConstructor)
			{
				auto ThisCallType = TypeSymbol(Syb.ID);

				auto sym = Symbol_GetSymbol(ClassInf->_AutoGenerateMoveConstructor.value());
				FuncInfo& V = *sym->Get_Info<FuncInfo>(); 

				auto OldFunc = _IR_LookingAtIRFunc;
				auto OldBlock = _IR_LookingAtIRBlock;
				//
				auto funcirid = IR_MangleName(&V);
				_IR_LookingAtIRFunc = _IR_Builder.NewFunc(_IR_Builder.ToID(funcirid), IR_ConvertToIRType(V.Ret));
				IRType ThisPar = IR_ConvertToIRType(V.Pars.front());
				{
					IRPar par = IRPar();
					par.identifier = _IR_Builder.ToID(ThisSymbolName);
					par.type = ThisPar;
					_IR_LookingAtIRFunc->Pars.push_back(par);

					IRPar par2 = IRPar();
					par2.identifier = _IR_Builder.ToID("CopyFrom");
					par2.type = ThisPar;
					_IR_LookingAtIRFunc->Pars.push_back(par2);
				}

				_IR_LookingAtIRBlock = _IR_LookingAtIRFunc->NewBlock(".");
				{
					for (size_t i = ClassInf->Fields.size() - 1; i != (Vector<FieldInfo>::size_type) - 1; i--)
					{
						auto& Item = ClassInf->Fields[i];
						auto v = HasMoveContructer(Item.Type);
						auto par1 = _IR_LookingAtIRBlock->New_Member_Dereference(&_IR_LookingAtIRFunc->Pars.front(), ThisPar, i);
						auto par2 = _IR_LookingAtIRBlock->New_Member_Dereference(&_IR_LookingAtIRFunc->Pars[1], ThisPar, i);

						if (v && !Item.Type.IsAddress())
						{
							auto info = v.value()->Get_Info<FuncInfo>();

							_IR_LookingAtIRBlock->NewPushParameter(_IR_LookingAtIRBlock->NewLoadPtr(par1));
							_IR_LookingAtIRBlock->NewPushParameter(_IR_LookingAtIRBlock->NewLoadPtr(par2));
							_IR_LookingAtIRBlock->NewCall(IR_GetIRID(info));
						}
						else
						{
							_IR_LookingAtIRBlock->NewStore(par1, par2);
						}
					}
					_IR_LookingAtIRBlock->NewRet();
				}

				{
					auto Funcinfo = &V;
					ClassMethod V2;
					V2.FullName = Funcinfo->FullName;
					V2.DecorationName = funcirid;
					V2.RetType = Assembly_ConvertToType(Funcinfo->Ret);


					V2.IsThisFunction = Funcinfo->FrontParIsUnNamed;
					V2.IsExternC = Funcinfo->IsExternC;
					V2.IsUnsafe = Funcinfo->IsUnsafe;
					V2.IsRemoved = Funcinfo->IsRemoved;
					V2.IsExport = Node._IsExport;

					for (size_t i = 0; i < Funcinfo->Pars.size(); i++)
					{
						auto& Item = Funcinfo->Pars[i];

						auto& F = V2.ParsType.emplace_back();
						F.IsOutPar = Item.IsOutPar;
						F.Type = Assembly_ConvertToType(Item.Type);
					}

					this->_ClassStack.push({});//the push and pop is dumb fix
					Class_Data* Ptr = Assembly_GetAssemblyClass(ScopeHelper::ApendedStrings(Syb.FullName, "n/a"));
					this->_ClassStack.pop();

					Ptr->Methods.push_back(std::move(V2));
				}
				//
				_IR_LookingAtIRFunc = OldFunc;
				_IR_LookingAtIRBlock = OldBlock;
			}

			for (auto& Item : ClassInf->_InheritedTypes)
			{
				Symbol_BuildTrait(&Syb, ClassInf, Item.Syb, NeverNullptr(Node._className.token));
			}
		

			bool needstoexportgenricaliases = true;

			if (needstoexportgenricaliases)
			{
				this->_ClassStack.push({});//the push and pop is dumb fix
				Class_Data* Ptr = Assembly_GetAssemblyClass(RemoveSymboolFuncOverloadMangling(ScopeHelper::ApendedStrings(Syb.FullName, "n/a")));
				this->_ClassStack.pop();

				Ptr->GenericAlias.reserve(ClassInf->_GenericAlias.size());
				for (auto& Item : ClassInf->_GenericAlias)
				{
					TraitAlias val;
					
					val.AliasName = Item.Name;
					val.Type = Assembly_ConvertToType(Item.Type);
					if (Item.IsExpression())
					{
						auto& Ex = Item.Expression.value();

						ReflectionRawData data;
						data.Resize(Ex.Size);
						memcpy(data.Get_Data(), Ex.Get_Data(), Ex.Size);

						val.Expression = std::move(data);
					}
					Ptr->GenericAlias.push_back(std::move(val));
				}
			}
		}


		if (Isgeneric_t && IsgenericInstantiation == false)
		{
			String_view Text = _LookingAtFile->FileText;

			String ClassStr = "$";
			ClassStr += Node._className.token->Value._String;

			size_t offset = 0;
			if (Node.EndOfClass->Type == TokenType::Semicolon)
			{
				offset += 1;
			}


			String_view ClassBody =
				String_view(&Text[Node._className.token->OnPos],
					Node.EndOfClass->OnPos - Node._className.token->OnPos + offset);

			GenericClass_Data& VClass = _Lib.Get_Assembly().AddGenericClass((String)ClassInf->Get_Name(), RemoveSymboolFuncOverloadMangling(ClassInf->FullName));

			VClass.Base.Implementation = ClassStr + String(ClassBody);
			VClass.Base.Implementation += "\n\n";
			VClass.AccessModifier = Syb.Access;
			VClass.IsExported = Node._IsExport;
			VClass.UseStatments = Generic_GetCurrentUseStatements();
		}
	}


	_Table.RemoveScope();

}
CapturedUseStatements SystematicAnalysis::Generic_GetCurrentUseStatements()
{
	CapturedUseStatements r;
	r.NameSpaces.resize(_Table.Useings.size());
	for (auto& Item : _Table.Useings)
	{
		r.NameSpaces.push_back(Item);
	}
	return r;
}
UCodeLangFrontEnd

#endif
