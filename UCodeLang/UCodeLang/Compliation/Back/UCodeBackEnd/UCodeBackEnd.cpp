#include "UCodeBackEnd.hpp"
#include "UCodeLang/Compliation/Helpers/InstructionBuilder.hpp"
#include "UCodeLang/Compliation/Helpers/NameDecoratior.hpp"
UCodeLangStart

#define GenIns(x) ResetIns(); x;
#define GenInsPush(x) ResetIns(); x;	ULib.Add_Instruction(_Ins);



UCodeBackEndObject::UCodeBackEndObject()
{
	
}

UCodeBackEndObject::~UCodeBackEndObject()
{
}


void UCodeBackEndObject::Reset()
{
	this->~UCodeBackEndObject();
	new (this)  UCodeBackEndObject;
}
void UCodeBackEndObject::BuildSymbols()
{
	for (auto& Item : _Input->_Symbols)
	{
		if (Item->SymType == IRSymbolType::StaticVarable
			|| Item->SymType == IRSymbolType::ThreadLocalVarable)
		{
			const IRBufferData* ItemBuf = Item->Get_ExAs<IRBufferData>();
			size_t Offset = _Output->Get_StaticBytes().size();
			size_t ItemSize = ItemBuf->Bytes.size();


			StaticMemoryManager::StaticMemInfo newinfo;
			newinfo.Offset = Offset;
			

			if (Item->SymType == IRSymbolType::StaticVarable)
			{
				_StaticMemory._List.AddValue(Item->identifier, newinfo);

				if (ItemBuf->IsInitialized)
				{
					_Output->AddStaticBytes(ItemBuf->Bytes.data(), ItemSize);
				}
				else
				{
					_Output->AddStaticBytes(ItemBuf->Bytes.data(), ItemSize);
				}

			}
			else
			{
				_ThreadMemory._List.AddValue(Item->identifier, newinfo);

				if (ItemBuf->IsInitialized)
				{
					_Output->AddThreadBytes(ItemBuf->Bytes.data(), ItemSize);
				}
				else
				{
					_Output->AddThreadBytes(ItemBuf->Bytes.data(), ItemSize);
				}
			}
		}
	}
}
void  UCodeBackEndObject::BuildFuncs()
{
	if (_Input->_StaticInit.HasInstructions()) {
		OnFunc(&_Input->_StaticInit);
	}

	if (_Input->_StaticdeInit.HasInstructions()) {
		OnFunc(&_Input->_StaticdeInit);
	}

	if (_Input->_threadInit.HasInstructions()) {
		OnFunc(&_Input->_threadInit);
	}

	if (_Input->_threaddeInit.HasInstructions()) {
		OnFunc(&_Input->_threaddeInit);
	}
	for (auto& Item : _Input->Funcs)
	{
		OnFunc(Item.get());
	}
}
void UCodeBackEndObject::Build(const IRBuilder* Input)
{
	_Input = Input;
	_Output = &Get_Output();

	if (Get_Settings()._Type == OutPutType::IRAndSymbols)
	{
		auto V = _Output->AddLayer(UCode_CodeLayer_IR_Name);
		auto V2 = CodeLayer::JustData();
		V2._Data = Input->ToBytes().MoveToVector();

		V->_Data = std::move(V2);
	}
	else
	{
		auto V = _Output->AddLayer(UCode_CodeLayer_UCodeVM_Name);
		V->_Data = CodeLayer::UCodeByteCode();
		_OutLayer = V->_Data.Get_If<CodeLayer::UCodeByteCode>();

		UpdateOptimizations();

		BuildSymbols();

		BuildFuncs();

		DoOptimizations();

		LinkFuncs();

		AddDebugInfo();

		
	}
}
void UCodeBackEndObject::AddDebugInfo()
{
	if (IsDebugMode())
	{
		_OutLayer->DebugInfo = ULangDebugInfo(std::move(_DebugInfo));

		//For Showing IR When using UAssembly::ToString
		auto V2 = CodeLayer::JustData();
		V2._Data = _Input->ToBytes().MoveToVector();
		auto V = _Output->AddLayer(UCode_CodeLayer_IR_Name);
		V->_Data = std::move(V2);
	}
}
UCodeBackEndObject::IRlocData UCodeBackEndObject::To(const ParlocData& Value)
{
	IRlocData r;
	r.ObjectType = Value.Par->type;

	if (auto V = Value.Location.Get_If<RegisterID>())
	{
		r.Info = *V;
	}
	else 
	{
		throw std::exception("bad path");
	}
	return r;
}
void UCodeBackEndObject::UpdateOptimizations()
{
	auto& Setings = Get_Settings();
	bool Debug = (OptimizationFlags_t)Setings._Flags & (OptimizationFlags_t)OptimizationFlags::Debug;
	bool ForSize = (OptimizationFlags_t)Setings._Flags & (OptimizationFlags_t)OptimizationFlags::ForSize;
	bool ForSpeed = (OptimizationFlags_t)Setings._Flags & (OptimizationFlags_t)OptimizationFlags::ForSpeed;
	bool ForMaxSpeed = (OptimizationFlags_t)Setings._Flags & (OptimizationFlags_t)OptimizationFlags::ForMaxSpeed;


	if (Debug == false)
	{
		_Optimizations.InlineFuncionCopys = ForSize;
	}
	else
	{
		if (ForSpeed)
		{
			_Optimizations.ReOderForCacheHit = true;
		}
	}
}
void UCodeBackEndObject::DoOptimizations()
{

	if (_Optimizations.InlineFuncionCopys)
	{
		BinaryVectorMap<size_t, UCodeFunc*> FuncsHash;
		for (auto& Item : Funcs)
		{
			Item->_Hash = UAssembly::UAssembly::BuildHashForSub(Item->_Ins.data(), Item->_Ins.size());
		}

		for (auto& Item : Funcs)
		{
			if (Item->_AutoJumpTo) { continue; }

			size_t Hash = Item->_Hash.value();
			for (auto& Item2 : Funcs)
			{
				if (Item2->_AutoJumpTo) { continue; }
				if (Item.get() != Item2.get())
				{
					size_t Hash2 = Item2->_Hash.value();
					if (Hash == Hash2)
					{
						Item2->_AutoJumpTo = Item.get();

						Item2->_Ins.clear();
						Item2->FuncsToLink.clear();

						InstructionBuilder::Jump(NullAddress, _Ins);

						FuncInsID Jump;
						Jump.Index = 0;
						Jump._FuncID = Item->IRName;
						Item->FuncsToLink.push_back(Jump);
					}
					break;
				}
			}


		}
	}
}
void UCodeBackEndObject::LinkFuncs()
{
	for (auto& Item : FuncsToLink)
	{
		Instruction& Ins =_OutLayer->Get_Instructions()[Item.Index];


		UAddress funcpos = NullAddress;
		for (auto& Item2 : _Funcpos)
		{
			if (Item2._FuncID == Item._FuncID)
			{
				funcpos = Item2.Index;
				break;
			}
		}

		if (Ins.OpCode == InstructionSet::Call)
		{
			InstructionBuilder::Call(funcpos, Ins);
		}
		else if (Ins.OpCode == InstructionSet::LoadFuncPtr)
		{
			InstructionBuilder::LoadFuncPtr(funcpos, Ins.Value1.AsRegister, Ins);
		}
		else
		{
			throw std::exception("not added");
		}


	}
}
void UCodeBackEndObject::RegWillBeUsed(RegisterID Value)
{
	auto& Info = _Registers.GetInfo(Value);

	if (Info.Types.has_value())
	{
		auto& TypesValue = Info.Types.value();
		if (auto Item = TypesValue.Get_If<const IRInstruction*>())
		{
			if (IsReferencedAfterThisIndex(*Item)) 
			{
				_Registers.FreeRegister(Value);
				CopyValueToStack(*Item, GetType(*Item), Value);
			}
		}
	}
}
void UCodeBackEndObject::OnFunc(const IRFunc* IR)
{

	//build code
	UAddress FuncStart = _OutLayer->GetLastInstruction() + 1;


	{
		auto V = GetParsLoc(IR->Pars);
		CurrentFuncParPos =std::move(V.ParsPos);
	}


	auto FuncName = _Input->FromID(IR->identifier);
	if (&_Input->_StaticInit == IR)
	{
		FuncName = StaticVariablesInitializeFunc;
	}
	else if(&_Input->_StaticdeInit == IR)
	{
		FuncName = StaticVariablesUnLoadFunc;
	}
	else if (&_Input->_threadInit == IR)
	{
		FuncName = ThreadVariablesInitializeFunc;
	}
	else if (&_Input->_threaddeInit == IR)
	{
		FuncName = ThreadVariablesUnLoadFunc;
	}
		

	size_t LinkedCallsIndex = FuncsToLink.size();

	if (IR->Linkage != IRFuncLink::StaticLink)
	{
		BuildLink(FuncName,IR->Linkage);
	}



	if (IR->Blocks.size())
	{
		OnBlock(IR->Blocks.front().get());
	}

	_InputPar = RegisterID::StartParameterRegister;

	if (_Stack.Size)
	{
		RegisterID V = RegisterID::A;

		auto Ptr = Get_Settings().PtrSize;
		auto& instr = _OutLayer->Get_Instructions();
		
		InstructionBuilder::IncrementStackPointer(_Ins, V);
		instr.insert(instr.begin() + FuncStart, _Ins);
		
		if (Ptr == IntSizes::Int32)
		{
			InstructionBuilder::Store32(_Ins, V, (UInt32)_Stack.Size);
		}
		else
		{
			InstructionBuilder::Store64(_Ins, V, (UInt64)_Stack.Size);
		}

		instr.insert(instr.begin() + FuncStart, _Ins);


		for (size_t i = LinkedCallsIndex; i < FuncsToLink.size(); i++)
		{
			auto& Item = FuncsToLink[i];
			Item.Index += 2;
		}
	}

	_Stack.Reset();
	_Registers.Reset();
	_OutLayer->Add_NameToInstruction(FuncStart, FuncName);


	Funcpos V;
	V.Index = FuncStart-1;
	V._FuncID = IR->identifier;
	_Funcpos.push_back(V);
}
void UCodeBackEndObject::OnBlock(const IRBlock* IR)
{
	LookingBlock = IR;
	OnBlockBuildCode(IR);
	
}
void UCodeBackEndObject::OnBlockBuildCode(const IRBlock* IR)
{
	struct  InsToUpdate_t
	{
		size_t InsToUpdate;
		size_t Jumpto;
	};
	Vector<InsToUpdate_t> InsToUpdate;
	VectorMap<size_t, UAddress> IRToUCodeIns;

	for (size_t i = 0; i < IR->Instructions.size(); i++)
	{
		auto& Item_ = IR->Instructions[i];
		auto Item = Item_.get();
		Index = i;



		if (IsDebugMode()) {
			auto DebugInfo = IR->DebugInfo.Get_debugfor(i);
			for (auto& Item : DebugInfo)
			{
				if (auto Val = Item->Debug.Get_If<IRDebugSetFile>())
				{
					Add_SetFile(Val->FileName, i);
				}
				else if (auto Val = Item->Debug.Get_If<IRDebugSetLineNumber>())
				{
					Add_SetLineNumber(Val->LineNumber, Val->InsInBlock);
				}
			}
		}

		switch (Item->Type)
		{
		case IRInstructionType::None:
			break;
		case IRInstructionType::LoadNone:
		{
			if (IsPrimitive(Item->ObjectType))
			{
				auto R = GetRegisterForTep();
				RegWillBeUsed(R);
				SetRegister(R,Item);
			}
			else 
			{
				if (!_Stack.Has(Item))
				{

					UCodeBackEndObject::IRlocData R;
					_Stack.AddWithSize(Item, GetSize(Item));
				}
			}
		}
			break;
		case IRInstructionType::Load:
		{
			auto V = GetIRLocData(Item, Item->Target());
			GiveNameTo(V,Item);
		}
		break;
		case IRInstructionType::LoadReturn:
		{
			auto V = GetIRLocData(Item, Item->Target());
			auto ObjectSize = _Input->GetSize(GetType(Item));
			if (ObjectSize <= sizeof(AnyInt64))
			{
				MakeIntoRegister(V, {RegisterID::OuPutRegister});
			}
			else
			{
				MakeIntoRegister(GetPointerOf(V), {RegisterID::OuPutRegister});
			}
		}
		break;
		case IRInstructionType::Reassign:
		{
			StoreValue(Item, Item->Target(), Item->Input());
		}
		break;

		case IRInstructionType::SMult:
		{
			RegisterID V = RegisterID::MathOuPutRegister;
			RegisterID A = MakeIntoRegister(Item, Item->A);
			SetRegister(A,Item);
			RegisterID B = MakeIntoRegister(Item, Item->B);

			RegWillBeUsed(V);
			switch (Item->ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::MultS8(_Ins, A, B); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::MultS16(_Ins, A, B); PushIns(); break;

			bit32label:
			case IRTypes::i32:InstructionBuilder::MultS32(_Ins, A, B); PushIns(); break;

			bit64label:
			case IRTypes::i64:InstructionBuilder::MultS64(_Ins, A, B); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					goto bit32label;
				}
				else
				{
					goto bit64label;
				}
				break;
			default:
				throw std::exception("not added");
				break;
			}

			FreeRegister(A);
			SetRegister(V,Item);
		}
		break;
		case IRInstructionType::UMult:
		{
			RegisterID V = RegisterID::MathOuPutRegister;
			RegisterID A = MakeIntoRegister(Item, Item->A);
			SetRegister(A,Item);
			RegisterID B = MakeIntoRegister(Item, Item->B);


			RegWillBeUsed(V);
			auto& Object_t = Item->ObjectType;
			switch (Object_t._Type)
			{
			case IRTypes::i8:InstructionBuilder::MultU8(_Ins, A, B); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::MultU16(_Ins, A, B); PushIns(); break;

			bit32label1:
			case IRTypes::i32:InstructionBuilder::MultU32(_Ins, A, B); PushIns(); break;

			bit64label1:
			case IRTypes::i64:InstructionBuilder::MultU64(_Ins, A, B); PushIns(); break;
			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					goto bit32label1;
				}
				else
				{
					goto bit64label1;
				}
				break;
			default:
				throw std::exception("not added");
				break;
			}

			FreeRegister(A);
			SetRegister(V, Item);
		}
		break;

		case IRInstructionType::SDiv:
		{
			RegisterID V = RegisterID::MathOuPutRegister;
			RegisterID A = MakeIntoRegister(Item, Item->A);
			SetRegister(A,Item);
			RegisterID B = MakeIntoRegister(Item, Item->B);


			RegWillBeUsed(V);
			switch (Item->ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::DivS8(_Ins, A, B); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::DivS16(_Ins, A, B); PushIns(); break;

			bit32label2:
			case IRTypes::i32:InstructionBuilder::DivS32(_Ins, A, B); PushIns(); break;

			bit64label2:
			case IRTypes::i64:InstructionBuilder::DivS64(_Ins, A, B); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					goto bit32label2;
				}
				else
				{
					goto bit64label2;
				}
				break;
			default:
				throw std::exception("not added");
				break;
			}

			FreeRegister(A);
			SetRegister(V, Item);
		}
		break;
		case IRInstructionType::UDiv:
		{
			RegisterID V = RegisterID::MathOuPutRegister;
			RegisterID A = MakeIntoRegister(Item, Item->A);
			SetRegister(A,Item);
			RegisterID B = MakeIntoRegister(Item, Item->B);


			RegWillBeUsed(V);
			switch (Item->ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::DivU8(_Ins, A, B); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::DivU16(_Ins, A, B); PushIns(); break;

			bit32label5:
			case IRTypes::i32:InstructionBuilder::DivU32(_Ins, A, B); PushIns(); break;

			bit64label5:
			case IRTypes::i64:InstructionBuilder::DivU64(_Ins, A, B); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					goto bit32label5;
				}
				else
				{
					goto bit64label5;
				}
				break;
			default:
				throw std::exception("not added");
				break;
			}

			FreeRegister(A);
			SetRegister(V,Item);
		}
		break;

		case IRInstructionType::Add:
		{
			RegisterID V = RegisterID::MathOuPutRegister;
			RegisterID A = MakeIntoRegister(Item, Item->A);
			SetRegister(A,Item);
			RegisterID B = MakeIntoRegister(Item, Item->B);


			RegWillBeUsed(V);
			switch (Item->ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::Add8(_Ins, A, B); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::Add16(_Ins, A, B); PushIns(); break;

			bit32label6:
			case IRTypes::i32:InstructionBuilder::Add32(_Ins, A, B); PushIns(); break;

			bit64label6:
			case IRTypes::i64:InstructionBuilder::Add64(_Ins, A, B); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					goto bit32label6;
				}
				else
				{
					goto bit64label6;
				}
				break;
			default:
				throw std::exception("not added");
				break;
			}

			FreeRegister(A);
			SetRegister(V,Item);
		}
		break;
		case IRInstructionType::Sub:
		{
			RegisterID V = RegisterID::MathOuPutRegister;
			RegisterID A = MakeIntoRegister(Item, Item->A);
			SetRegister(A,Item);
			RegisterID B = MakeIntoRegister(Item, Item->B);


			RegWillBeUsed(V);
			switch (Item->ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::Sub8(_Ins, A, B); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::Sub16(_Ins, A, B); PushIns(); break;
			bit32label4:
			case IRTypes::i32:InstructionBuilder::Sub32(_Ins, A, B); PushIns(); break;

			bit64label4:
			case IRTypes::i64:InstructionBuilder::Sub64(_Ins, A, B); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					goto bit32label4;
				}
				else
				{
					goto bit64label4;
				}
				break;
			default:
				throw std::exception("not added");
				break;
			}

			FreeRegister(A);
			SetRegister(V,Item);
		}
		break;
		case IRInstructionType::PushParameter:
		{
			RegisterID_t& Reg = *(RegisterID_t*)&_InputPar;

			
			if (Reg < (RegisterID_t)RegisterID::EndParameterRegister)
			{
				if (IsPrimitive(GetType(Item)))
				{
					LoadOpToReg(Item, Item->Target(), _InputPar);
					SetRegister(_InputPar, Item->Target());
					Reg++;
				}
				else
				{
					PushOpStack(Item, Item->Target());
				}
			}
			else
			{
				PushOpStack(Item, Item->Target());
			}
		}
		break;
		case IRInstructionType::Call:
		{
			auto FuncInfo = _Input->GetFunc(Item->Target().identifer);
			auto FData = FuncCallStart(FuncInfo->Pars, FuncInfo->ReturnType);
			
			InstructionBuilder::Call(NullAddress, _Ins); PushIns();

			FuncInsID Tep;
			Tep.Index = _OutLayer->Get_Instructions().size() - 1;
			Tep._FuncID = Item->Target().identifer;

			FuncsToLink.push_back(Tep);

		

			FuncCallEnd(FData);
		}
		break;
		case IRInstructionType::MallocCall:
		{
			RegisterID ID = GetRegisterForTep();
			SetRegister(ID,Item);
			auto SizeReg = MakeIntoRegister(Item, Item->Target());
			InstructionBuilder::Malloc(_Ins, SizeReg, ID); PushIns();

			SetRegister(ID,Item);
		}
		break;
		case IRInstructionType::FreeCall:
		{
			InstructionBuilder::Free(_Ins, MakeIntoRegister(Item, Item->Target())); PushIns();
		}break;
		case IRInstructionType::Return:
			goto DoneLoop;
			break;
		case IRInstructionType::Jump:
			InstructionBuilder::Jump(NullAddress, _Ins); PushIns();
			InsToUpdate.push_back({ _OutLayer->Get_Instructions().size(), Item->Target().Value.AsUIntNative });
			break;
		case IRInstructionType::ConditionalJump:
			InstructionBuilder::Jumpif(NullAddress, MakeIntoRegister(Item, Item->Input()), _Ins); PushIns();
			InsToUpdate.push_back({ _OutLayer->Get_Instructions().size(),Item->Target().Value.AsUIntNative });
			break;
		case IRInstructionType::Logical_Not:
		{	
			RegisterID Out = GetRegisterForTep();
			LogicalNot(Item->ObjectType._Type, MakeIntoRegister(Item, Item->Target()),Out);
			SetRegister(Out,Item);

		}
		break;
		case IRInstructionType::Reassign_dereference:
		{
			RegisterID Pointer = MakeIntoRegister(Item, Item->Target());
			StoreValueInPointer(Pointer, GetIRLocData(Item, Item->Input()));
		}
		break; 
		case IRInstructionType::EqualTo:
		{
			RegisterID V = RegisterID::BoolRegister;
			RegisterID A = MakeIntoRegister(Item, Item->A);
			SetRegister(A,Item);
			RegisterID B = MakeIntoRegister(Item, Item->B);

			RegWillBeUsed(V);
			switch (Item->ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::equalto8(_Ins, A, B); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::equalto16(_Ins, A, B); PushIns(); break;

			bit32label3:
			case IRTypes::i32:InstructionBuilder::equalto32(_Ins, A, B); PushIns(); break;
			bit64label3:
			case IRTypes::i64:InstructionBuilder::equalto64(_Ins, A, B); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					goto bit32label3;
				}
				else
				{
					goto bit64label3;
				}
				break;
			default:
				throw std::exception("not added");
				break;
			}

			FreeRegister(A);
			SetRegister(V,Item);
		}
		break;
		case IRInstructionType::CallFuncPtr:
		{
			auto FuncInfo = _Input->GetSymbol(Item->Target().identifer)->Get_ExAs<IRFuncPtr>();
			auto& FData = FuncCallStart(FuncInfo->Pars, FuncInfo->Ret);
 
			InstructionBuilder::CallReg(MakeIntoRegister(Item,Item->Target()), _Ins); PushIns();


			FuncCallEnd(FData);
		}
		break;
		

		case IRInstructionType::Member_Access://Vec2.X
		{
		}
		break;
		case IRInstructionType::Member_Access_Dereference://Vec2->X
		{
		
		}
		break;
		case IRInstructionType::SIntToUInt:
		{
			RegisterID V = GetRegisterForTep();
			RegisterID A = MakeIntoRegister(Item, Item->Target());
			SetRegister(A,Item);

			switch (GetType(Item->Target())._Type)
			{
			case IRTypes::i8:InstructionBuilder::SInt8ToUInt8(_Ins, A, V); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::SInt16ToUInt16(_Ins, A, V); PushIns(); break;
			bit32label44:
			case IRTypes::i32:InstructionBuilder::SInt32ToUInt32(_Ins, A, V); PushIns(); break;

			bit64label44:
			case IRTypes::i64:InstructionBuilder::SInt64ToUInt64(_Ins, A, V); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					goto bit32label44;
				}
				else
				{
					goto bit64label44;
				}
				break;
			default:
				throw std::exception("not added");
				break;
			}

			FreeRegister(A);
			SetRegister(V,Item);
		}
		break;
		case IRInstructionType::UIntToSInt:
		{
			RegisterID V = GetRegisterForTep();
			RegisterID A = MakeIntoRegister(Item, Item->Target());
			SetRegister(A,Item);

			switch (GetType(Item->Target())._Type)
			{
			case IRTypes::i8:InstructionBuilder::UInt8ToSInt8(_Ins, A, V); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::UInt16ToSInt16(_Ins, A, V); PushIns(); break;
			bit32label9:
			case IRTypes::i32:InstructionBuilder::UInt32ToSInt32(_Ins, A, V); PushIns(); break;

			bit64label9:
			case IRTypes::i64:InstructionBuilder::UInt64ToSInt64(_Ins, A, V); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					goto bit32label9;
				}
				else
				{
					goto bit64label9;
				}
				break;
			default:
				throw std::exception("not added");
				break;
			}

			FreeRegister(A);
			SetRegister(V,Item);
		}
		break;
		case IRInstructionType::SIntToSInt8:
		{
			BuildSIntToIntCast(Item, Item->Target(), 1);
		}
		break;
		case IRInstructionType::SIntToSInt16:
		{
			BuildSIntToIntCast(Item, Item->Target(), 2);
		}
		break;
		case IRInstructionType::SIntToSInt32:
		{
			BuildSIntToIntCast(Item, Item->Target(), 4);
		}
		break;
		case IRInstructionType::SIntToSInt64:
		{
			BuildSIntToIntCast(Item, Item->Target(), 8);
		}
		break;
		case IRInstructionType::UIntToUInt8:
		{
			BuildSIntToIntCast(Item, Item->Target(), 1);
		}
		break;
		case IRInstructionType::UIntToUInt16:
		{
			BuildSIntToIntCast(Item, Item->Target(), 2);
		}
		break;
		case IRInstructionType::UIntToUInt32:
		{
			BuildSIntToIntCast(Item, Item->Target(), 4);
		}
		break;
		case IRInstructionType::UIntToUInt64:
		{
			BuildSIntToIntCast(Item, Item->Target(), 8);
		}
		break;
		default:
			throw std::exception("not added");
			break;
		}


		IRToUCodeIns[i] = _OutLayer->Get_Instructions().size() - 1;
	}
DoneLoop:
	DropStack();
	DropPars();
	InstructionBuilder::Return(ExitState::Success, _Ins); PushIns();
	IRToUCodeIns[Index] = _OutLayer->Get_Instructions().size() - 1;

	for (auto& Item : InsToUpdate)
	{
		Instruction& Ins = _OutLayer->Get_Instructions()[Item.InsToUpdate - 1];
		UAddress JumpPos = IRToUCodeIns[Item.Jumpto];

		if (Ins.OpCode == InstructionSet::Jump) 
		{
			InstructionBuilder::Jump(JumpPos, Ins);
		}
		else
		{
			InstructionBuilder::Jumpif(JumpPos,Ins.Value1.AsRegister, Ins);
		}
	}
}
UCodeBackEndObject::FuncCallEndData UCodeBackEndObject::FuncCallStart(const Vector<IRType>& Pars, const IRType& RetType)
{
	Vector<IRPar> Tep;
	Tep.resize(Pars.size());
	for (size_t i = 0; i < Pars.size(); i++)
	{
		auto& Item = Pars[i];
		auto& TepItem = Tep[i];
		TepItem.type = Item;
	}
	return FuncCallStart(Tep,RetType);
}
Optional< RegisterID> UCodeBackEndObject::FindIRInRegister(const IRInstruction* Value)
{
	auto R = _Registers.GetInfo(Value);
	if (R.has_value())
	{
		return R.value();
	}

	for (size_t i = 0; i < _Registers.Registers.size(); i++)
	{
		auto& Item = _Registers.Registers[i];

		if (Item.Types.has_value())
		{
			auto& ItemValue = Item.Types.value();
			if (auto IR = ItemValue.Get_If<const IRInstruction*>())
			{
				auto IRV = *IR;
				if (IRV->Type == IRInstructionType::Load)
				{
					if (IRV->Target().Pointer == Value)
					{
						return (RegisterID)i;
					}
				}
			}
		}
	}
	return {};
}
Optional<RegisterID>  UCodeBackEndObject::FindValueInRegister(AnyInt64 Value)
{
	auto CompilerRet = _Registers.GetValue(Value);
	if (CompilerRet.has_value())
	{
		return CompilerRet.value();
	}

	for (size_t i = 0; i < _Registers.Registers.size(); i++)
	{
		auto& Item = _Registers.Registers[i];

		if (Item.Types.has_value()) 
		{
			auto& ItemValue = Item.Types.value();
			if (auto IR = ItemValue.Get_If<const IRInstruction*>())
			{
				auto IRV = *IR;
				if (IRV->Type == IRInstructionType::Load)
				{
					AnyInt64 ItemValue = IRV->Target().Value;
					if (ItemValue.Value == Value.AsUInt64)
					{
						return (RegisterID)i;
					}
				}
			}
		}
	}
	return {};
}

UCodeBackEndObject::FuncCallEndData  UCodeBackEndObject::FuncCallStart(const Vector<IRPar>& Pars, const IRType& RetType)
{
	FuncCallRet(RetType);
	for (RegisterID_t i = (RegisterID_t)RegisterID::StartRegister; i < (RegisterID_t)RegisterID::EndRegister; i++)
	{
		RegisterID CompilerRet = (RegisterID)i;
		auto& Item = _Registers.Registers[i];

		bool WasSet = false;
		if (Item.Types.has_value())
		{
			auto& ItemTypes = Item.Types.value();
			if (auto IRV = ItemTypes.Get_If<const IRInstruction*>())
			{
				auto& IR = *IRV;
				if (IsReferencedAfterThisIndex(IR))
				{
					WasSet = true;
					auto VType = GetType(IR);
					switch (VType._Type)
					{
					case IRTypes::i8:
						InstructionBuilder::Push8(_Ins, CompilerRet); PushIns();
						break;
					case IRTypes::i16:
						InstructionBuilder::Push16(_Ins, CompilerRet); PushIns();
						break;
					bit32labelGG:
					case IRTypes::f32:
					case IRTypes::i32:
						InstructionBuilder::Push32(_Ins, CompilerRet); PushIns();
						break;
					bit64labelGG:
					case IRTypes::f64:
					case IRTypes::i64:
						InstructionBuilder::Push64(_Ins, CompilerRet); PushIns();
						break;

					case IRTypes::pointer:
						if (Get_Settings().PtrSize == IntSizes::Int32)
						{
							goto bit32labelGG;
						}
						else
						{
							goto bit64labelGG;
						}
						break;
					default:
						throw std::exception("not added");
						break;
					}
				}
				
			}
			

		}
		
		if (WasSet == false)
		{
			Item.Types = {};
		}
	}

	FuncCallEndData r;
	r.Pars = Pars;
	return r;
}
void UCodeBackEndObject::FuncCallRet(const IRType& RetType)
{
	if (RetType._Type != IRTypes::Void)
	{
		RegWillBeUsed(RegisterID::OuPutRegister);
	}
}
void UCodeBackEndObject::FuncCallEnd(UCodeBackEndObject::FuncCallEndData& Data)
{
	auto V = GetParsLoc(Data.Pars);


	size_t PopBufferSize = 0;
	for (size_t i = 0; i < V.OverflowedPars.size(); i++)
	{
		auto& Item = V.ParsPos[V.OverflowedPars[i]];
		PopBufferSize += _Input->GetSize(Item.Par->type);
	}


	if (PopBufferSize != 0)
	{
		RegisterID PopRegister = GetRegisterForTep();
		while (PopBufferSize != 0)
		{
			if (PopBufferSize >= 8)
			{
				InstructionBuilder::Pop64(_Ins, PopRegister);
				PopBufferSize -= 8;
			}
			else if (PopBufferSize >= 4)
			{
				InstructionBuilder::Pop32(_Ins, PopRegister);
				PopBufferSize -= 4;
			}
			else if (PopBufferSize >= 2)
			{
				InstructionBuilder::Pop16(_Ins, PopRegister);
				PopBufferSize -= 2;
			}
			else if (PopBufferSize >= 8)
			{
				InstructionBuilder::Pop8(_Ins, PopRegister);
				PopBufferSize -= 1;
			}
			PushIns();
		}

	}
	_InputPar = RegisterID::StartParameterRegister;




	for (RegisterID_t i = (RegisterID_t)RegisterID::StartRegister; i < (RegisterID_t)RegisterID::EndRegister; i++)
	{
		RegisterID CompilerRet = (RegisterID)i;
		auto& Item = _Registers.Registers[i];
		if (Item.Types.has_value())
		{
			auto& ItemTypes = Item.Types.value();
			if (auto IRV = ItemTypes.Get_If<const IRInstruction*>())
			{
				auto IR = *IRV;
				if (IsReferencedAfterThisIndex(IR))
				{
					auto VType = GetType(IR);
					switch (VType._Type)
					{
					case IRTypes::i8:
						InstructionBuilder::Pop8(_Ins, CompilerRet); PushIns();
						break;
					case IRTypes::i16:
						InstructionBuilder::Pop16(_Ins, CompilerRet); PushIns();
						break;
					bit32labelGG2:
					case IRTypes::f32:
					case IRTypes::i32:
						InstructionBuilder::Pop32(_Ins, CompilerRet); PushIns();
						break;
					bit64labelGG2:
					case IRTypes::f64:
					case IRTypes::i64:
						InstructionBuilder::Pop64(_Ins, CompilerRet); PushIns();
						break;

					case IRTypes::pointer:
						if (Get_Settings().PtrSize == IntSizes::Int32)
						{
							goto bit32labelGG2;
						}
						else
						{
							goto bit64labelGG2;
						}
						break;
					default:
						throw std::exception("not added");
						break;
					}
				}

			}
		}
	}
}
void UCodeBackEndObject::AddOffset(IRlocData& Pos, size_t Offset)
{
	if (auto Val = Pos.Info.Get_If<IRlocData_StackPost>())
	{
		Val->offset += Offset;
	}
	else if (auto Val = Pos.Info.Get_If<IRlocData_StackPre>())
	{
		Val->offset += Offset;
	}
	else if (auto Val = Pos.Info.Get_If<IRlocData_StaticPos>())
	{
		Val->offset += Offset;
	}
	else if (auto Val = Pos.Info.Get_If<IRlocData_ThreadPos>())
	{
		Val->offset += Offset;
	}
}
void UCodeBackEndObject::DropStack()
{
	if (_Stack.Size)
	{
		auto Ptr = Get_Settings().PtrSize;
		auto V = GetRegisterForTep();
		if (Ptr == IntSizes::Int32)
		{
			InstructionBuilder::Store32(_Ins, V, (UInt32)_Stack.Size); PushIns();
		}
		else
		{
			InstructionBuilder::Store64(_Ins, V, (UInt64)_Stack.Size); PushIns();
		}
		InstructionBuilder::DecrementStackPointer(_Ins, V); PushIns();
	}

	for (auto& Item: _Stack.Reupdates)
	{
		Instruction& Ins = _OutLayer->Get_Instructions()[Item.InsIndex];
		if (Item.PostFunc)
		{
			size_t NewOffset = _Stack.Size - Item.StackOffset;


			switch (Ins.OpCode)
			{
			case InstructionSet::GetPointerOfStackSub:
			{
				InstructionBuilder::GetPointerOfStackSub(Ins,Ins.Value0.AsRegister, NewOffset);
			}
			break;
			case InstructionSet::GetFromStackSub64:
			{
				InstructionBuilder::GetFromStackSub64(Ins,NewOffset, Ins.Value1.AsRegister);
			}
			break;
			case InstructionSet::GetFromStackSub32:
			{
				InstructionBuilder::GetFromStackSub32(Ins, NewOffset, Ins.Value1.AsRegister);
			}
			break;
			case InstructionSet::GetFromStackSub16:
			{
				InstructionBuilder::GetFromStackSub16(Ins, NewOffset, Ins.Value1.AsRegister);
			}
			break;
			case InstructionSet::GetFromStackSub8:
			{
				InstructionBuilder::GetFromStackSub8(Ins, NewOffset, Ins.Value1.AsRegister);
			}
			break;
			case InstructionSet::StoreRegOnStack8:
			{
				InstructionBuilder::StoreRegOnStack8(Ins, Ins.Value0.AsRegister, NewOffset);
			}
			break;
			case InstructionSet::StoreRegOnStack16:
			{
				InstructionBuilder::StoreRegOnStack16(Ins, Ins.Value0.AsRegister, NewOffset);
			}
			break;
			case InstructionSet::StoreRegOnStack32:
			{
				InstructionBuilder::StoreRegOnStack32(Ins, Ins.Value0.AsRegister, NewOffset);
			}
			break;
			case InstructionSet::StoreRegOnStack64:
			{
				InstructionBuilder::StoreRegOnStack64(Ins, Ins.Value0.AsRegister, NewOffset);
			}
			break;
			case InstructionSet::StoreRegOnStackSub8:
			{
				InstructionBuilder::StoreRegOnStackSub8(Ins, Ins.Value0.AsRegister, NewOffset);
			}
			break;
			case InstructionSet::StoreRegOnStackSub16:
			{
				InstructionBuilder::StoreRegOnStackSub16(Ins, Ins.Value0.AsRegister, NewOffset);
			}
			break;
			case InstructionSet::StoreRegOnStackSub32:
			{
				InstructionBuilder::StoreRegOnStackSub32(Ins, Ins.Value0.AsRegister, NewOffset);
			}
			break;
			case InstructionSet::StoreRegOnStackSub64:
			{
				InstructionBuilder::StoreRegOnStackSub64(Ins, Ins.Value0.AsRegister, NewOffset);
			}
			break;
			default:
				throw std::exception("bad ins");
				break;
			}
		}
		else
		{

		}
	}
}
void UCodeBackEndObject::DropPars()
{

}
void UCodeBackEndObject::StoreValueInPointer(const IRType& ObjectType, RegisterID Pointer, const  IROperator& Value,IRInstruction* Ins)
{
	StoreValueInPointer(ObjectType, Pointer, LoadOp(Ins, Value));
}
void UCodeBackEndObject::StoreValueInPointer(const IRType& ObjectType, RegisterID Pointer, RegisterID Value)
{
	switch (ObjectType._Type)
	{
	case IRTypes::i8:InstructionBuilder::StoreRegToPtr8(_Ins, Value, Pointer); PushIns(); break;
	case IRTypes::i16:InstructionBuilder::StoreRegToPtr16(_Ins, Value, Pointer); PushIns(); break;
	bit32label9:
	case IRTypes::i32:InstructionBuilder::StoreRegToPtr32(_Ins, Value, Pointer); PushIns(); break;

	bit64label9:
	case IRTypes::i64:InstructionBuilder::StoreRegToPtr64(_Ins, Value, Pointer); PushIns(); break;

	case IRTypes::pointer:
		if (Get_Settings().PtrSize == IntSizes::Int32)
		{
			goto bit32label9;
		}
		else
		{
			goto bit64label9;
		}
		break;
	default:
		throw std::exception("not added");
		break;
	}
	FreeRegister(Value);
}
RegisterID  UCodeBackEndObject::ReadValueFromPointer(const IRType& ObjectType, RegisterID Pointer)
{
	auto Out = GetRegisterForTep();
	switch (ObjectType._Type)
	{
	case IRTypes::i8:
		InstructionBuilder::StoreFromPtrToReg8(_Ins, Pointer, Out); PushIns();
		break;
	case IRTypes::i16:
		InstructionBuilder::StoreFromPtrToReg16(_Ins, Pointer, Out); PushIns();
		break;
	Int32L5:
	case IRTypes::f32:
	case IRTypes::i32:
		InstructionBuilder::StoreFromPtrToReg32(_Ins, Pointer, Out); PushIns();
		break;

	Int64L5:
	case IRTypes::f64:
	case IRTypes::i64:
		InstructionBuilder::StoreFromPtrToReg64(_Ins, Pointer, Out); PushIns();
		break;
	case IRTypes::pointer:
		switch (Get_Settings().PtrSize)
		{
		case IntSizes::Int32:goto Int32L5;
		case IntSizes::Int64:goto Int64L5;
		default:
			throw std::exception("not added");
			break;
		}
		break;
	default:
		throw std::exception("not added");
		break;
	}
	return Out;
}
void UCodeBackEndObject::BuildUIntToIntCast(const IRInstruction* Ins, const IROperator& Op, size_t IntSize)
{
	RegisterID V = GetRegisterForTep();
	RegisterID A = MakeIntoRegister(Ins, Op);
	SetRegister(A,Ins);

	size_t ItemSize = _Input->GetSize(GetType(Op));


	while (ItemSize != IntSize)
	{
		if (ItemSize > IntSize)//cast down
		{
			switch (ItemSize)
			{
			case 2:InstructionBuilder::Int16ToInt8(_Ins, A, V); PushIns(); ItemSize = 1; break;
			case 4:InstructionBuilder::Int32ToInt16(_Ins, A, V); PushIns(); ItemSize = 2; break;
			case 8:InstructionBuilder::Int64ToInt32(_Ins, A, V); PushIns(); ItemSize = 4; break;
			default:break;
			}
			A = V;
		}
		else//cast up
		{
			switch (ItemSize)
			{
			case 1:InstructionBuilder::Int8ToInt16(_Ins, A, V); PushIns(); ItemSize = 2; break;
			case 2:InstructionBuilder::Int16ToInt32(_Ins, A, V); PushIns(); ItemSize = 4; break;
			case 4:InstructionBuilder::Int32ToInt64(_Ins, A, V); PushIns(); ItemSize = 8; break;
			default:break;
			}
			A = V;
		}


	}

	FreeRegister(A);
	SetRegister(V,Ins);
}
void UCodeBackEndObject::BuildSIntToIntCast(const IRInstruction* Ins, const IROperator& Op, size_t IntSize)
{
	BuildUIntToIntCast(Ins, Op, IntSize);
}
void UCodeBackEndObject::StoreValueInPointer(RegisterID Pointer,size_t Pointerofset, const IRlocData& Value)
{
	size_t ObjectSize = GetSize(Value.ObjectType);
	size_t Offset = Pointerofset;
	
	RegisterID Reg = Value.Info.Get<RegisterID>();
	
	while (ObjectSize != 0)
	{
		
		if (ObjectSize >= 8)
		{
			

			if (Offset == 0)
			{
				InstructionBuilder::StoreRegToPtr64(_Ins, Reg, Pointer); PushIns();
			}
			else
			{
				InstructionBuilder::PointerMemberLoad64(_Ins, Pointer,Reg, Offset); PushIns();
			}
			ObjectSize -= 8;
			Offset += 8;
		}
		else if (ObjectSize >= 4)
		{
			if (Offset == 0)
			{
				InstructionBuilder::StoreRegToPtr32(_Ins, Reg, Pointer); PushIns();
			}
			else
			{
				InstructionBuilder::PointerMemberLoad32(_Ins, Pointer,Reg, Offset); PushIns();
			}
			ObjectSize -= 4;
			Offset += 4;
		}
		else if (ObjectSize >= 2)
		{
			if (Offset == 0)
			{
				InstructionBuilder::StoreRegToPtr16(_Ins, Reg, Pointer); PushIns();
			}
			else
			{
				InstructionBuilder::PointerMemberLoad16(_Ins, Pointer,Reg, Offset); PushIns();
			}
			ObjectSize -= 2;
			Offset += 2;
		}
		else if (ObjectSize >= 1)
		{
			if (Offset == 0)
			{
				InstructionBuilder::StoreRegToPtr8(_Ins, Reg, Pointer); PushIns();
			}
			else
			{
				InstructionBuilder::PointerMemberLoad8(_Ins, Pointer,Reg, Offset); PushIns();
			}
			ObjectSize -= 1;
			Offset += 1;
		}
	}

	
}
RegisterID UCodeBackEndObject::MakeIntoRegister(const IRlocData& Value, Optional<RegisterID> RegisterToPut)
{
	if (Value.Info.Is<RegisterID>())
	{
		auto V = Value.Info.Get<RegisterID>();
		if (RegisterToPut.has_value())
		{
			RegToReg(Value.ObjectType._Type, V, RegisterToPut.value());
			V = RegisterToPut.value();
		}
		return V;
	}
	else if (auto Val = Value.Info.Get_If<IRlocData_StackPost>())
	{
		RegisterID Tep;
		if (RegisterToPut.has_value())
		{
			Tep = RegisterToPut.value();
		}
		else
		{
			Tep = GetRegisterForTep();
		}

		auto Size = GetSize(Value.ObjectType);

	
		switch (Size)
		{
		case 1:
			InstructionBuilder::GetFromStackSub8(_Ins, 0, Tep);
			break;
		case 2:
			InstructionBuilder::GetFromStackSub16(_Ins, 0, Tep);
			break;
		case 4:
			InstructionBuilder::GetFromStackSub32(_Ins, 0, Tep);
			break;
		case 8:
			InstructionBuilder::GetFromStackSub64(_Ins, 0, Tep);
			break;
		default:
			throw std::exception("not added");
			break;
		}
		_Stack.AddReUpdatePostFunc(PushIns(),Val->offset);

		return Tep;
	}
	else if (auto Val = Value.Info.Get_If<IRlocData_StaticPos>())
	{
		RegisterID Tep;
		if (RegisterToPut.has_value())
		{
			Tep = RegisterToPut.value();
		}
		else
		{
			Tep = GetRegisterForTep();
		}

		auto Size = GetSize(Value.ObjectType);

		InstructionBuilder::GetPointerOfStaticMem(_Ins, Tep, Val->offset); PushIns();
		
		switch (Size)
		{
		case 1:
			InstructionBuilder::StoreFromPtrToReg8(_Ins, Tep, Tep);
			break;
		case 2:
			InstructionBuilder::StoreFromPtrToReg16(_Ins, Tep, Tep);
			break;
		case 4:
			InstructionBuilder::StoreFromPtrToReg32(_Ins, Tep, Tep);
			break;
		case 8:
			InstructionBuilder::StoreFromPtrToReg64(_Ins,Tep, Tep);
			break;
		default:
			throw std::exception("not added");
			break;
		}
		PushIns();

		return Tep;
	}
	throw std::exception("not added");
}
void  UCodeBackEndObject::GiveNameTo(const IRlocData& Value, const IRInstruction* Name)
{
	if (Value.Info.Is<RegisterID>())
	{
		SetRegister(Value.Info.Get<RegisterID>(), Name);
	}
	else if (auto Val = Value.Info.Get_If<IRlocData_StackPost>())
	{
		_Stack.Add(Name, Val->offset);
	}
	else if (auto Val = Value.Info.Get_If<IRlocData_StaticPos>())
	{

	}
	else if (auto Val = Value.Info.Get_If<IRlocData_ThreadPos>())
	{

	}
	else
	{
		throw std::exception("not added");
	}
}
UCodeBackEndObject::IRlocData UCodeBackEndObject::GetPointerOf(const IRlocData& Value)
{
	if (auto Val = Value.Info.Get_If<RegisterID>())
	{
		auto stack = GetFreeStackPos(Value.ObjectType);
		auto R = GetRegisterForTep();

		
		//move value to stack
		IRlocData Src;
		Src.ObjectType = Value.ObjectType;
		Src.Info = stack;
		IRlocData V;
		V.Info = *Val;
		V.ObjectType = Value.ObjectType;

		CopyValues(V,Src);

		InstructionBuilder::GetPointerOfStackSub(_Ins, R, 0);
		_Stack.AddReUpdatePostFunc(PushIns(), stack.offset);
		
		//make all read/writes point to stack.
		auto T = _Registers.GetInfo(*Val);
		_Stack.Get(stack.offset)->IR = T.Types.value().Get<const IRInstruction*>();

		FreeRegister(*Val);
		FreeRegister(R);

		IRlocData Rl;
		Rl.Info = R;
		Rl.ObjectType = IRTypes::pointer;
		return Rl;
	}
	else if (auto Val = Value.Info.Get_If<IRlocData_StackPost>())
	{
		auto R = GetRegisterForTep();
		InstructionBuilder::GetPointerOfStackSub(_Ins, R,0);

		_Stack.AddReUpdatePostFunc(PushIns(),Val->offset);

		IRlocData V;
		V.Info = R;
		V.ObjectType = IRTypes::pointer;
		return V;
	}
	else if (auto Val = Value.Info.Get_If<IRlocData_StaticPos>())
	{
		auto R = GetRegisterForTep();
		InstructionBuilder::GetPointerOfStaticMem(_Ins, R, Val->offset);

		IRlocData V;
		V.Info = R;
		V.ObjectType = IRTypes::pointer;
		return V;
	}
	else if (auto Val = Value.Info.Get_If<IRlocData_ThreadPos>())
	{
		auto R = GetRegisterForTep();
		InstructionBuilder::GetPointerOfThreadMem(_Ins, R, Val->offset);

		IRlocData V;
		V.Info = R;
		V.ObjectType = IRTypes::pointer;
		return V;
	}
	else
	{
		throw std::exception("not added");
	}
	throw std::exception("not added");
}
RegisterID UCodeBackEndObject::LoadOp(const IRInstruction* Ins, const  IROperator& Op)
{
	if (Op.Type == IROperatorType::Value)
	{

		AnyInt64 Value = ToAnyInt(Ins->ObjectType, Op);
		auto CompilerRet = FindValueInRegister(Value);
		if (CompilerRet.has_value())
		{
			return CompilerRet.value();
		}

		auto V = GetRegisterForTep();
		switch (Ins->ObjectType._Type)
		{
		case IRTypes::i8:
			InstructionBuilder::Store8(_Ins, V, Op.Value.AsInt8); PushIns();
			break;
		case IRTypes::i16:
			InstructionBuilder::Store16(_Ins, V, Op.Value.AsInt16); PushIns();
			break;

		bit32label:
		case IRTypes::i32:
			InstructionBuilder::Store32(_Ins, V, Op.Value.AsInt32); PushIns();
			break;
		case IRTypes::f32:
			InstructionBuilder::Storef32(_Ins, V, Op.Value.Asfloat32); PushIns();
			break;
		case IRTypes::f64:
			InstructionBuilder::Storef64(_Ins, V, Op.Value.Asfloat64); PushIns();
			break;

		bit64label:
		case IRTypes::i64:
			InstructionBuilder::Store64(_Ins, V, Op.Value.AsInt64); PushIns();
			break;

		case IRTypes::pointer:
			if (Get_Settings().PtrSize == IntSizes::Int32)
			{
				goto bit32label;
			}
			else
			{
				goto bit64label;
			}
			break;
		default:
			throw std::exception("not added");
			break;
		}
		return V;
	}
	else if (Op.Type == IROperatorType::IRParameter)
	{
		auto V = GetParData(Op.Parameter);

		if (V->Location.Is<RegisterID>())
		{
			return V->Location.Get<RegisterID>();
		}
		else
		{
			auto Type = Op.Parameter->type;

			RegisterID CompilerRet = GetRegisterForTep();
			size_t Offset = GetStackOffset(*V);

			switch (Type._Type)
			{
			case IRTypes::i8:
				InstructionBuilder::GetFromStackSub8(_Ins, 0, CompilerRet);
				break;
			case IRTypes::i16:
				InstructionBuilder::GetFromStackSub16(_Ins, 0, CompilerRet);
				break;
			Case32Bit2:
			case IRTypes::f32:
			case IRTypes::i32:
				InstructionBuilder::GetFromStackSub32(_Ins, 0, CompilerRet);
				break;
			Case64Bit2:
			case IRTypes::f64:
			case IRTypes::i64:
				InstructionBuilder::GetFromStackSub64(_Ins, 0, CompilerRet);
				break;
			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int64)
				{
					goto Case64Bit2;
				}
				else
				{
					goto Case32Bit2;
				}
				break;
			default:
				throw std::exception("not added");
				break;

			}
			_Stack.AddReUpdatePostFunc(PushIns(), Offset);

			SetRegister(CompilerRet, Ins);

			V->Location = CompilerRet;
			return CompilerRet;
		}
	}
	else if (Op.Type == IROperatorType::IRInstruction)
	{
		auto Item = Op.Pointer;

		auto InReg = FindIRInRegister(Item);

		if (InReg)
		{
			return InReg.value();

		}
		else
		{
			auto V = GetIRLocData(Op.Pointer);

			return MakeIntoRegister(V);
		}
	}
	else if (Op.Type == IROperatorType::Get_Func_Pointer)
	{
		auto V = GetRegisterForTep();

		InstructionBuilder::LoadFuncPtr(NullAddress, V, _Ins); PushIns();

		FuncInsID Tep;
		Tep.Index = _OutLayer->Get_Instructions().size() - 1;
		Tep._FuncID = Op.identifer;

		FuncsToLink.push_back(Tep);

		return V;
	}
	else if (Op.Type == IROperatorType::Get_PointerOf_IRidentifier)
	{
		auto Syb = _Input->GetSymbol(Op.identifer);
		if (Syb)
		{
			if (Syb->SymType == IRSymbolType::StaticVarable)
			{
				StaticMemoryManager::StaticMemInfo& Value = _StaticMemory._List[Op.identifer];

				auto V = GetRegisterForTep();
				InstructionBuilder::GetPointerOfStaticMem(_Ins, V, Value.Offset); PushIns();

				return V;
			}
			else if (Syb->SymType == IRSymbolType::ThreadLocalVarable)
			{
				StaticMemoryManager::StaticMemInfo& Value = _ThreadMemory._List[Op.identifer];

				auto V = GetRegisterForTep();
				InstructionBuilder::GetPointerOfStaticMem(_Ins, V, Value.Offset); PushIns();

				return V;
			}

		}
	}
	else if (Op.Type == IROperatorType::Get_PointerOf_IRInstruction)
	{
		auto Ins = Op.Pointer;
		auto V = GetIRLocData(Ins);
		auto Item = GetPointerOf(V);
		return MakeIntoRegister(Item);
	}
	else if (Op.Type == IROperatorType::DereferenceOf_IRInstruction)
	{
		auto Ins = Op.Pointer;
		auto InsData = GetIRLocData(Ins);

		return ReadValueFromPointer(InsData.ObjectType, MakeIntoRegister(InsData));
	}
	else if (Op.Type == IROperatorType::DereferenceOf_IRParameter)
	{
		const auto Ins = Op.Parameter;
		auto InsData = To(*GetParData(Ins));
		return ReadValueFromPointer(InsData.ObjectType, MakeIntoRegister(InsData));
	}

	throw std::exception("not added");
}

void UCodeBackEndObject::StoreValue(const IRInstruction* Ins, const  IROperator& OutputLocationIR, const  IROperator& Input)
{
	bool IRusedereference = false;
	if (OutputLocationIR.Type ==IROperatorType::IRInstruction)
	{
		auto& Item = OutputLocationIR.Pointer;
		if (Item->Type == IRInstructionType::Member_Access_Dereference)
		{
			IRusedereference = true;
		}
	}

	
	if (IRusedereference)
	{
		auto& Item = OutputLocationIR.Pointer;

		auto Type = Item->ObjectType;//GetType(Item, Item->Target());

		const IRStruct* VStruct = _Input->GetSymbol(Type._symbol)->Get_ExAs<IRStruct>();

		auto Reg = LoadOp(Item,Item->Target());
		size_t FieldOffset = _Input->GetOffset(VStruct, Item->Input().Value.AsUIntNative);

		StoreValueInPointer(Reg,FieldOffset,GetIRLocData(Ins, Input));
	}
	else
	{
		IRInstruction V;
		V.ObjectType = GetType(OutputLocationIR);
		V.Type = IRInstructionType::Load;
		V.Target() = OutputLocationIR;

		auto Out = GetIRLocData(Ins, OutputLocationIR);
		auto TepInfo = std::move(_Registers.GetInfo(Ins));

		auto Src = GetIRLocData(Ins, Input);
		auto Tep2Info = std::move(_Registers.GetInfo(Ins));

		CopyValues(Src, Out);

	}
	
}
void  UCodeBackEndObject::CopyValueToStack(const IRInstruction* IRName, const IRType& ObjectType, RegisterID Item)
{
	size_t StackPos = _Stack.Size;
	switch (ObjectType._Type)
	{
	case IRTypes::i8://move value to stack
		_Stack.Size += 1;
		InstructionBuilder::StoreRegOnStackSub8(_Ins, Item, _Stack.Size);
		break;
	case IRTypes::i16:
		_Stack.Size += 2;
		InstructionBuilder::StoreRegOnStackSub16(_Ins, Item, _Stack.Size); 
		break;
	Int32L:
	case IRTypes::f32:
	case IRTypes::i32:
		_Stack.Size += 4;
		InstructionBuilder::StoreRegOnStackSub32(_Ins, Item, _Stack.Size);
		break;
	Int64L:
	case IRTypes::f64:
	case IRTypes::i64:
		_Stack.Size += 8;
		InstructionBuilder::StoreRegOnStackSub64(_Ins, Item, _Stack.Size);
		break;
	case IRTypes::pointer:
		switch (Get_Settings().PtrSize)
		{
		case IntSizes::Int32:goto Int32L;
		case IntSizes::Int64:goto Int64L;
		default:
			throw std::exception("not added");
			break;
		}
		break;
	case IRTypes::IRsymbol:
	{
		auto Syb = _Input->GetSymbol(ObjectType._symbol);
		switch (Syb->SymType)
		{
		case IRSymbolType::Struct:
		{
			const IRStruct* V = Syb->Get_ExAs<IRStruct>();
			_Stack.Size += _Input->GetSize(V);
		}
		break;
		default:
			throw std::exception("not added");
			break;
		}
	}
	break;
	default:

		throw std::exception("not added");
		break;
	}
	_Stack.Add(IRName,StackPos);
	_Stack.AddReUpdatePostFunc(PushIns(), StackPos);
}
void UCodeBackEndObject::MoveValueToStack(const IRInstruction* IRName, const IRType& ObjectType, RegisterID Item)
{
	CopyValueToStack(IRName, ObjectType, Item);
	_Registers.FreeRegister(Item);
}
void UCodeBackEndObject::LoadOpToReg(const IRInstruction* Ins, const  IROperator& Op, RegisterID Out)
{
	auto Pos = GetIRLocData(Ins, Op);
	MakeIntoRegister(Pos, Out);
}
void UCodeBackEndObject::RegToReg(IRTypes Type, RegisterID In, RegisterID Out)
{
	if (In != Out)
	{
		switch (Type)
		{
		case IRTypes::i8:
			InstructionBuilder::StoreRegToReg8(_Ins,In,Out); PushIns();
			break;
		case IRTypes::i16:
			InstructionBuilder::StoreRegToReg16(_Ins, In, Out); PushIns();
			break;
		Int32L:
		case IRTypes::f32:
		case IRTypes::i32:
			InstructionBuilder::StoreRegToReg32(_Ins, In, Out); PushIns();
			break;

		Int64L:
		case IRTypes::f64:
		case IRTypes::i64:
			InstructionBuilder::StoreRegToReg64(_Ins, In, Out); PushIns();
			break;
			Pointer:
		case IRTypes::pointer:
			switch (Get_Settings().PtrSize)
			{
			case IntSizes::Int32:goto Int32L;
			case IntSizes::Int64:goto Int64L;
			default:
				throw std::exception("not added");
				break;
			}
			break;
		case IRTypes::IRsymbol:
		{
			goto Pointer;
		}//FuncPtrs
			break;
		default:
			throw std::exception("not added");
			break;
		}
	}
}
void UCodeBackEndObject::PushOpStack(const IRInstruction* Ins, const  IROperator& Op)
{
	auto CompilerRet =LoadOp(Ins,Op);
	switch (Ins->ObjectType._Type)
	{
	case IRTypes::i8:
		InstructionBuilder::Push8(_Ins, CompilerRet); PushIns();
		_Stack.PushedOffset += 1;
		break;
	case IRTypes::i16:
		InstructionBuilder::Push16(_Ins, CompilerRet); PushIns();
		_Stack.PushedOffset += 2;
		break;

	bit32label:
	case IRTypes::f32:
	case IRTypes::i32:
		InstructionBuilder::Push32(_Ins, CompilerRet); PushIns();
		_Stack.PushedOffset += 4;
		break;

	bit64label:
	case IRTypes::f64:
	case IRTypes::i64:
		InstructionBuilder::Push64(_Ins, CompilerRet); PushIns();
		_Stack.PushedOffset += 8;
		break;

	case IRTypes::pointer:
		if (Get_Settings().PtrSize == IntSizes::Int32)
		{
			goto bit32label;
		}
		else
		{
			goto bit64label;
		}
		break;
	default:
		throw std::exception("not added");
		break;
	}
}
UCodeBackEndObject::IRlocData UCodeBackEndObject::GetIRLocData(const IRInstruction* Ins)
{
	auto RegInfo = FindIRInRegister(Ins);
	if (RegInfo.has_value())
	{
		UCodeBackEndObject::IRlocData R;
		R.Info = RegInfo.value();
		R.ObjectType = GetType(Ins);
		return R;
	}
	else
	{
		if (Ins->Type == IRInstructionType::LoadNone)
		{

			auto V = _Stack.Has(Ins);
			if (V)
			{
				UCodeBackEndObject::IRlocData R;
				R.Info = IRlocData_StackPost(V->Offset);
				return R;
			}
			else
			{
				UCodeBackEndObject::IRlocData R;
				_Stack.AddWithSize(Ins, GetSize(Ins));
				return R;
			}
		}
		else if (Ins->Type == IRInstructionType::Load)
		{
			return GetIRLocData(Ins->Target().Pointer);
		}
		else
		{
			throw std::exception("not added");
		}
	}
}
UCodeBackEndObject::IRlocData UCodeBackEndObject::GetIRLocData(const IRInstruction* Ins, const IROperator& Op)
{
	bool IsPrimitive = _Input->IsPrimitive(GetType(Ins,Op));
	if (IsPrimitive)
	{
		IRlocData CompilerRet;
		CompilerRet.ObjectType = GetType(Ins,Op);

		if (Op.Type == IROperatorType::IRInstruction)
		{
			auto Item = Op.Pointer;
			CompilerRet.ObjectType = GetType(Item);
			auto InReg = FindIRInRegister(Item);
			if (InReg.has_value())
			{
				CompilerRet.Info = InReg.value();
			}
			else
			{
				auto Stack = _Stack.Has(Item);
				if (Stack)
				{
					CompilerRet.Info = IRlocData_StackPost(Stack->Offset);
				}
				else if (Item->Type == IRInstructionType::Member_Access)
				{
					auto Pos = GetIRLocData(Item, Item->Target());
					const IRStruct* VStruct = _Input->GetSymbol(Pos.ObjectType._symbol)->Get_ExAs<IRStruct>();
					size_t FieldIndex = Item->Input().Value.AsUIntNative;

					size_t Offset = _Input->GetOffset(VStruct,FieldIndex);
					AddOffset(Pos, Offset);

					Pos.ObjectType = VStruct->Fields[FieldIndex].Type;
					
					CompilerRet = Pos;
				}
				else if (Item->Type == IRInstructionType::Member_Access_Dereference)
				{
					const IRStruct* VStruct = _Input->GetSymbol(GetType(Item, Item->Target())._symbol)->Get_ExAs<IRStruct>();

					auto Reg = LoadOp(Item, Item->Target());
					size_t FieldOffset = _Input->GetOffset(VStruct, Item->Input().Value.AsUIntNative);

					size_t ObjectSize = 4;

					auto RegOut = GetRegisterForTep();

					if (FieldOffset == 0)
					{
						InstructionBuilder::StoreFromPtrToReg32(_Ins, Reg, RegOut); PushIns();
					}
					else
					{
						InstructionBuilder::PointerMemberRead32(_Ins, Reg, RegOut, FieldOffset); PushIns();
					}

					CompilerRet.Info = RegOut;
				}
				else if (Item->Type == IRInstructionType::Call)
				{
					CompilerRet.Info = RegisterID::OuPutRegister;
				}
				else if (Item->Type == IRInstructionType::Load)
				{
					return GetIRLocData(Item, Item->Target());
				}
				else
				{
					throw std::exception("bad path");
				}
			}
		}
		else if (Op.Type == IROperatorType::IRParameter)
		{
			auto V = GetParData(Op.Parameter);

			if (V) 
			{
				return To(*V);
			}
			else
			{
				throw std::exception("bad path");
			}
		}
		else if (Op.Type == IROperatorType::Get_PointerOf_IRInstruction)
		{
			CompilerRet = GetPointerOf(GetIRLocData(Op.Pointer));
		}
		else if (Op.Type == IROperatorType::Get_PointerOf_IRParameter)
		{
			CompilerRet = GetPointerOf(To(*GetParData(Op.Parameter)));
		}
		else if (Op.Type == IROperatorType::Value)
		{
			CompilerRet.Info = LoadOp(Ins,Op);
		}
		else if (Op.Type == IROperatorType::DereferenceOf_IRInstruction
			|| Op.Type == IROperatorType::DereferenceOf_IRParameter)
		{
			CompilerRet.Info = LoadOp(Ins, Op);
		}
		else if (Op.Type == IROperatorType::IRidentifier)
		{
			if (auto Syb = _Input->GetSymbol(Op.identifer))
			{
				if (Syb->SymType == IRSymbolType::StaticVarable)
				{
					const auto& Mem = _StaticMemory._List.at(Op.identifer);
					CompilerRet.Info = IRlocData_StaticPos(Mem.Offset);
					CompilerRet.ObjectType = Syb->Type;
				}
				else if (Syb->SymType == IRSymbolType::ThreadLocalVarable)
				{
					const auto& Mem = _ThreadMemory._List.at(Op.identifer);
					CompilerRet.Info = IRlocData_ThreadPos(Mem.Offset);
					CompilerRet.ObjectType = Syb->Type;
				}
				else
				{
					throw std::exception("not added");
				}
			}
		}
		else
		{
			throw std::exception("bad path");
		}
		return CompilerRet;
	}
	else
	{
		IRlocData CompilerRet;
		CompilerRet.ObjectType = GetType(Ins, Op);
		if (Op.Type == IROperatorType::IRInstruction)
		{
			auto Item = Op.Pointer;

			auto InReg = FindIRInRegister(Item);
			
			CompilerRet.ObjectType = GetType(Item);
			if (InReg.has_value())
			{
				
				CompilerRet.Info = InReg.value();
			}
			else
			{
				auto Val = _Stack.Has(Item);

				if (Val)
				{
					CompilerRet.Info = IRlocData_StackPost(Val->Offset);
				}
				else
				{
					if (Item->Type == IRInstructionType::Call)
					{
						size_t ObjectSize = GetSize(Item);
						if (ObjectSize > sizeof(AnyInt64))
						{
							CompilerRet.Info = RegisterID::OuPutRegister;

							IRlocData tep = GetFreeStackLoc(GetType(Item));

							CopyValues(CompilerRet, tep, true, false);
							CompilerRet = tep;
						}
						else
						{
							CompilerRet.Info = RegisterID::OuPutRegister;
						}
					}
					else if (Item->Type == IRInstructionType::Load)
					{
						return GetIRLocData(Item,Item->Target());
					}
					else 
					{
						throw std::exception("not added");
					}
				}
			}

			return CompilerRet;
		}
		else if (Op.Type == IROperatorType::IRidentifier)
		{
			if (auto Syb = _Input->GetSymbol(Op.identifer))
			{
				if (Syb->SymType == IRSymbolType::StaticVarable)
				{
					const auto& Mem = _StaticMemory._List.at(Op.identifer);
					CompilerRet.Info = IRlocData_StaticPos(Mem.Offset);
				}
				else if (Syb->SymType == IRSymbolType::ThreadLocalVarable)
				{
					const auto& Mem =_ThreadMemory._List.at(Op.identifer);
					CompilerRet.Info = IRlocData_ThreadPos(Mem.Offset);
				}
				else
				{
					throw std::exception("not added");
				}
			}
			return CompilerRet;
		}
		else if (Op.Type == IROperatorType::DereferenceOf_IRInstruction)
		{
			auto Ins = Op.Pointer;
			auto V = GetIRLocData(Ins);
	
			IRlocData tep = GetFreeStackLoc(Ins->ObjectType);

			CopyValues(V, tep, true, false);
			return tep;
		}
		else if (Op.Type == IROperatorType::DereferenceOf_IRParameter)
		{
			const auto Ins = Op.Parameter;
			auto V = To(*GetParData(Ins));

			IRlocData tep = GetFreeStackLoc(Ins->type);

			CopyValues(V, tep, true, false);
			return tep;
		}
		else
		{
			throw std::exception("not added");
		}
	}
	throw std::exception("not added");
}
void UCodeBackEndObject::MoveRegInValue(RegisterID Value, const IRlocData& To, size_t Offset)
{
	auto Size = GetSize(To.ObjectType);
	if (auto Val = To.Info.Get_If<RegisterID>())
	{
		if (Offset == 0) {
			RegToReg(To.ObjectType._Type, Value, *Val);
		}
		else
		{
			throw std::exception("not added");
		}
	}
	else if (auto Val = To.Info.Get_If<IRlocData_StackPost>())
	{
		switch (Size)
		{
		case 1:
			InstructionBuilder::StoreRegOnStackSub8(_Ins, Value, 0); 
			break;
		case 2:
			InstructionBuilder::StoreRegOnStackSub16(_Ins, Value, 0);
			break;
		case 4:
			InstructionBuilder::StoreRegOnStackSub32(_Ins, Value, 0);
			break;
		case 8:
			InstructionBuilder::StoreRegOnStackSub64(_Ins, Value,0);
			break;
		default:
			throw std::exception("bad path");
			break;
		}
		_Stack.AddReUpdatePostFunc(PushIns(), Val->offset + Offset);
	}
	else if (auto Val = To.Info.Get_If<IRlocData_StaticPos>())
	{
		auto TepReg = GetRegisterForTep();
		InstructionBuilder::GetPointerOfStaticMem(_Ins, TepReg, Val->offset + Offset); PushIns();

		switch (Size)
		{
		case 1:
			InstructionBuilder::StoreRegToPtr8(_Ins, Value, TepReg); PushIns();
			break;
		case 2:
			InstructionBuilder::StoreRegToPtr16(_Ins, Value, TepReg); PushIns();
			break;
		case 4:
			InstructionBuilder::StoreRegToPtr32(_Ins, Value, TepReg); PushIns();
			break;
		case 8:
			InstructionBuilder::StoreRegToPtr64(_Ins, Value, TepReg); PushIns();
			break;
		default:
			throw std::exception("bad path");
			break;
		}
	}
	else if (auto Val = To.Info.Get_If<IRlocData_ThreadPos>())
	{
		auto TepReg = GetRegisterForTep();
		InstructionBuilder::GetPointerOfThreadMem(_Ins, TepReg, Val->offset + Offset); PushIns();

		switch (Size)
		{
		case 1:
			InstructionBuilder::StoreRegToPtr8(_Ins, Value, TepReg); PushIns();
			break;
		case 2:
			InstructionBuilder::StoreRegToPtr16(_Ins, Value, TepReg); PushIns();
			break;
		case 4:
			InstructionBuilder::StoreRegToPtr32(_Ins, Value, TepReg); PushIns();
			break;
		case 8:
			InstructionBuilder::StoreRegToPtr64(_Ins, Value, TepReg); PushIns();
			break;
		default:
			throw std::exception("bad path");
			break;
		}
	}
	else
	{
		throw std::exception("bad path");
	}
}
void UCodeBackEndObject::MoveValueInReg(const IRlocData& Value, size_t Offset, RegisterID To)
{
	auto Size = GetSize(Value.ObjectType);
	if (auto Val = Value.Info.Get_If<RegisterID>())
	{
		if (Offset == 0) {
			RegToReg(Value.ObjectType._Type, *Val, To);
		}
		else
		{
			throw std::exception("not added");
		}
	}
	else if (auto Val = Value.Info.Get_If<IRlocData_StackPost>())
	{
		
		switch (Size)
		{
		case 1:
			InstructionBuilder::GetFromStackSub8(_Ins,0, To); 
			break;
		case 2:
			InstructionBuilder::GetFromStackSub16(_Ins, 0, To); 
			break;
		case 4:
			InstructionBuilder::GetFromStackSub32(_Ins, 0, To);
			break;
		case 8:
			InstructionBuilder::GetFromStackSub64(_Ins, 0, To);
			break;
		default:
			throw std::exception("bad path");
			break;
		}
		_Stack.AddReUpdatePostFunc(PushIns(), Val->offset + Offset);
	}
	else if (auto Val = Value.Info.Get_If<IRlocData_StaticPos>())
	{
		InstructionBuilder::GetPointerOfStaticMem(_Ins, To, Val->offset + Offset); PushIns();

		switch (Size)
		{
		case 1:
			InstructionBuilder::StoreFromPtrToReg8(_Ins, To, To); PushIns();
			break;
		case 2:
			InstructionBuilder::StoreFromPtrToReg16(_Ins, To, To); PushIns();
			break;
		case 4:
			InstructionBuilder::StoreFromPtrToReg32(_Ins, To, To); PushIns();
			break;
		case 8:
			InstructionBuilder::StoreFromPtrToReg64(_Ins, To, To); PushIns();
			break;
		default:
			throw std::exception("bad path");
			break;
		}
	}
	else if (auto Val = Value.Info.Get_If<IRlocData_ThreadPos>())
	{
		InstructionBuilder::GetPointerOfThreadMem(_Ins, To, Val->offset + Offset); PushIns();

		switch (Size)
		{
		case 1:
			InstructionBuilder::StoreFromPtrToReg8(_Ins, To, To); PushIns();
			break;
		case 2:
			InstructionBuilder::StoreFromPtrToReg16(_Ins, To, To); PushIns();
			break;
		case 4:
			InstructionBuilder::StoreFromPtrToReg32(_Ins, To, To); PushIns();
			break;
		case 8:
			InstructionBuilder::StoreFromPtrToReg64(_Ins, To, To); PushIns();
			break;
		default:
			throw std::exception("bad path");
			break;
		}
	}
	else
	{
		throw std::exception("bad path");
	}
}
void UCodeBackEndObject::ReadValueFromPointer(RegisterID Pointer, size_t Pointerofset, const IRlocData& Out)
{
	auto Size = GetSize(Out.ObjectType);
	if (auto Val = Out.Info.Get_If<RegisterID>())
	{
		if (Pointerofset == 0)
		{
			switch (Size)
			{
			case 1:
				InstructionBuilder::StoreFromPtrToReg8(_Ins, Pointer, *Val); PushIns();
				break;
			case 2:
				InstructionBuilder::StoreFromPtrToReg16(_Ins, Pointer, *Val); PushIns();
				break;
			case 4:
				InstructionBuilder::StoreFromPtrToReg32(_Ins, Pointer, *Val); PushIns();
				break;
			case 8:
				InstructionBuilder::StoreFromPtrToReg64(_Ins, Pointer, *Val); PushIns();
				break;
			default:
				throw std::exception("bad path");
				break;
			}
		}
		else
		{
			switch (Size)
			{
			case 1:
				InstructionBuilder::PointerMemberRead8(_Ins, Pointer, *Val, Pointerofset); PushIns();
				break;
			case 2:
				InstructionBuilder::PointerMemberRead16(_Ins, Pointer, *Val, Pointerofset); PushIns();
				break;
			case 4:
				InstructionBuilder::PointerMemberRead32(_Ins, Pointer, *Val, Pointerofset); PushIns();
				break;
			case 8:
				InstructionBuilder::PointerMemberRead64(_Ins, Pointer, *Val, Pointerofset); PushIns();
				break;
			default:
				throw std::exception("bad path");
				break;
			}
		}
	}
	else
	{
		throw std::exception("bad path");
	}
}
void UCodeBackEndObject::CopyValues(const IRlocData& Src, const IRlocData& Out, bool DerefSrc, bool DerefOut)
{
	auto Size = GetSize(Src.ObjectType);
	size_t Offset = 0;
	auto Tep = GetRegisterForTep(Src);

	RegisterID SrcPointer = RegisterID::A;
	RegisterID OutPointer = RegisterID::A;
	if (DerefSrc)
	{
		SrcPointer = MakeIntoRegister(Src);
		
	}
	if (DerefOut)
	{
		OutPointer = MakeIntoRegister(Out);
	}
	while (Size != 0)
	{
		
		if (Size >= 8)
		{
			if (DerefSrc)
			{
				IRlocData V;
				V.Info = Tep;
				V.ObjectType = IRTypes::i64;
				ReadValueFromPointer(SrcPointer,Offset,V);
			}
			else 
			{
				IRlocData V;
				V.Info = Src.Info;
				V.ObjectType = IRTypes::i64;
				MoveValueInReg(V, Offset, Tep);
			}

			if (DerefOut) 
			{
				IRlocData V;
				V.Info = Tep;
				V.ObjectType = IRTypes::i64;
				StoreValueInPointer(OutPointer, Offset, V);
			}
			else
			{
				IRlocData V;
				V.Info = Out.Info;
				V.ObjectType = IRTypes::i64;
				MoveRegInValue(Tep, V, Offset);
			}

			Offset += 8;
			Size -= 8;
		}
		else if (Size >= 4)
		{
			if (DerefSrc)
			{
				IRlocData V;
				V.Info = Tep;
				V.ObjectType = IRTypes::i32;
				ReadValueFromPointer(SrcPointer, Offset, V);
			}
			else
			{

				IRlocData V;
				V.Info = Src.Info;
				V.ObjectType = IRTypes::i32;
				MoveValueInReg(V, Offset, Tep);
			}

			if (DerefOut)
			{
				IRlocData V;
				V.Info = Tep;
				V.ObjectType = IRTypes::i32;
				ReadValueFromPointer(SrcPointer, Offset, V);
			}
			else 
			{
				IRlocData V;
				V.Info =Out.Info;
				V.ObjectType = IRTypes::i32;
				MoveRegInValue(Tep, V, Offset);
			}

			Offset += 4;
			Size -= 4;
		}
		else if (Size >= 2)
		{
			if (DerefSrc)
			{
				IRlocData V;
				V.Info = Tep;
				V.ObjectType = IRTypes::i16;
				ReadValueFromPointer(SrcPointer, Offset,V);
			}
			else
			{

				IRlocData V;
				V.Info = Src.Info;
				V.ObjectType = IRTypes::i16;
				MoveValueInReg(V, Offset,Tep);
			}

			if (DerefOut)
			{
				IRlocData V;
				V.Info = Tep;
				V.ObjectType = IRTypes::i16;
				ReadValueFromPointer(SrcPointer, Offset, V);
			}
			else
			{
				IRlocData V;
				V.Info = Out.Info;
				V.ObjectType = IRTypes::i32;
				MoveRegInValue(Tep, V, Offset);
			}

			Offset += 2;
			Size -= 2;
		}
		else if (Size >= 1)
		{

			if (DerefSrc)
			{
				IRlocData V;
				V.Info = Tep;
				V.ObjectType = IRTypes::i8;
				ReadValueFromPointer(SrcPointer, Offset, V);
			}
			else
			{
				IRlocData V;
				V.Info = Src.Info;
				V.ObjectType = IRTypes::i8;
				MoveValueInReg(V, Offset,Tep);
			}

			if (DerefOut)
			{
				IRlocData V;
				V.Info = Tep;
				V.ObjectType = IRTypes::i8;
				ReadValueFromPointer(SrcPointer, Offset, V);
			}
			else
			{
				IRlocData V;
				V.Info = Out.Info;
				V.ObjectType = IRTypes::i8;
				MoveRegInValue(Tep, V, Offset);
			}

			Offset += 1;
			Size -= 1;
		}
	}
}

void UCodeBackEndObject::LogicalNot(IRTypes Type, RegisterID In, RegisterID Out)
{
	if (In != Out)
	{
		switch (Type)
		{
		case IRTypes::i8:
			InstructionBuilder::LogicalNot8(_Ins, In, Out); PushIns();
			break;
		case IRTypes::i16:
			InstructionBuilder::LogicalNot16(_Ins, In, Out); PushIns();
			break;
		Int32L:
		case IRTypes::f32:
		case IRTypes::i32:
			InstructionBuilder::LogicalNot32(_Ins, In, Out); PushIns();
			break;

		Int64L:
		case IRTypes::f64:
		case IRTypes::i64:
			InstructionBuilder::LogicalNot64(_Ins, In, Out); PushIns();
			break;
		case IRTypes::pointer:
			switch (Get_Settings().PtrSize)
			{
			case IntSizes::Int32:goto Int32L;
			case IntSizes::Int64:goto Int64L;
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
RegisterID UCodeBackEndObject::GetRegisterForTep()
{
	auto Item = _Registers.GetFreeRegister();
	if (!Item.has_value())
	{
		for (RegisterID_t i = (RegisterID_t)RegisterID::StartRegister; i < (RegisterID_t)RegisterID::EndRegister; i++)
		{
			RegisterID CompilerRet = (RegisterID)i;
			auto& Item = _Registers.Registers[i];
			if (Item.Types.has_value())
			{
				auto& ItemValue = Item.Types.value();

				if (auto IR = ItemValue.Get_If<const IRInstruction*>()) 
				{
					auto& IRV = *IR;
					if (!IsReferencedAfterThisIndex(IRV))
					{
						return CompilerRet;
					}
				}
			}
		}


		constexpr size_t BuffSize = (RegisterID_t)RegisterID::EndParameterRegister;
		Array<WeightType, BuffSize> RegisterWeights;
		for (RegisterID_t i = 0; i < BuffSize; i++)
		{
			RegisterID CompilerRet = (RegisterID)i;
			auto& Item = _Registers.Registers[i];

			if (Item.Types.has_value())
			{
				auto& ItemValue = Item.Types.value();

				if (auto IR = ItemValue.Get_If<const IRInstruction*>())
				{
					RegisterWeights[i] = IsReferencedAfterThisIndexWeighted(*IR);
				}
			}

		}


		size_t BestToOverLoad = 0;
		WeightType LestToBeat = SIZE_MAX;

		for (size_t i = 0; i < RegisterWeights.size(); i++)
		{
			auto Item = RegisterWeights[i];

			if (Item < LestToBeat)
			{
				BestToOverLoad = i;
				LestToBeat = Item;
			}

		}

		auto r = (RegisterID)BestToOverLoad;
		RegWillBeUsed(r);
		return r;
	}
	return Item.value();
}
AnyInt64 UCodeBackEndObject::ToAnyInt(const IRType& ObjectType,const IROperator& Op)
{
	AnyInt64 Value;
	switch (ObjectType._Type)
	{
	case IRTypes::i8:
		Value = Op.Value.AsInt8;
		break;
	case IRTypes::i16:
		Value = Op.Value.AsInt16;
		break;

	bit32label:
	case IRTypes::i32:
		Value = Op.Value.AsInt32;
		break;
	case IRTypes::f32:
		Value = Op.Value.Asfloat32;
		break;
	case IRTypes::f64:
		Value = Op.Value.Asfloat64;
		break;

	bit64label:
	case IRTypes::i64:
		Value = Op.Value.AsInt64;
		break;

	case IRTypes::pointer:
		if (Get_Settings().PtrSize == IntSizes::Int32)
		{
			goto bit32label;
		}
		else
		{
			goto bit64label;
		}
		break;
	default:
		throw std::exception("not added");
		break;
	}
	return Value;
}
UCodeBackEndObject::IRlocData_StackPost UCodeBackEndObject::GetFreeStackPos(IRType V)
{
	return  IRlocData_StackPost(_Stack.AddWithSize(nullptr, GetSize(V))->Offset);
}
UCodeBackEndObject::IRlocData UCodeBackEndObject::GetFreeStackLoc(IRType V)
{
	IRlocData tep;
	tep.ObjectType = V;
	tep.Info = IRlocData_StackPost(GetFreeStackPos(V));
	return  tep;
}
UCodeBackEndObject::WeightType UCodeBackEndObject::IsReferencedAfterThisIndexWeighted(const IROperator& Op)
{
	WeightType CompilerRet = 0;
	WeightType NextUseOffset = 0;

	WeightType Count = 0;
	for (size_t i = Index + 1; i < LookingBlock->Instructions.size(); i++)
	{
		auto Item = LookingBlock->Instructions[i].get();
		Count++;
		if (IsOperatorValueInTarget(Item->Type))
		{
			if (IsReferenceingTheSame(Op, Item->Target()))
			{
				CompilerRet++;

				if (NextUseOffset == 0)
				{
					NextUseOffset = Count;
				}
			}
		}

		if (IsOperatorValueInInput(Item->Type))
		{
			if (IsReferenceingTheSame(Op, Item->Input()))
			{
				CompilerRet++;

				if (NextUseOffset == 0)
				{
					NextUseOffset = Count;
				}
			}
		}
	}
	return CompilerRet - NextUseOffset;
}
bool  UCodeBackEndObject::IsReferenceingTheSame(const IROperator& Test, const IROperator& Other)
{
	if (Test.Type == Other.Type)
	{
		if (Test.identifer == Other.identifer)
		{
			return true;
		}
	}

	return false;
}
bool UCodeBackEndObject::IsReferencedAfterThisIndex(const IROperator& Op)
{
	for (size_t i = Index + 1; i < LookingBlock->Instructions.size(); i++)
	{
		auto Item = LookingBlock->Instructions[i].get();

		if (IsOperatorValueInTarget(Item->Type))
		{
			if (IsReferenceingTheSame(Op, Item->Target()))
			{
				return true;
			}
		}

		if (IsOperatorValueInInput(Item->Type))
		{
			if (IsReferenceingTheSame(Op, Item->Input()))
			{
				return true;
			}
		}
	}
	return false;
}
void  UCodeBackEndObject::SynchronizePar(ParlocData* Par)
{
	for (size_t i = 0; i < RegistersManager::RegisterSize; i++)
	{
		auto& Info = _Registers.Registers[i];
		if (Info.Types.has_value())
		{
			if (auto IR = Info.Types.value().Get_If<const IRInstruction*>())
			{
				const IRInstruction* IRV = *IR;
				if (IsLookingAtPar(IRV, Par->Par))
				{
					Par->Location = (RegisterID)i;
					break;
				}
			}
		}
	}
	for (size_t i = 0; i < _Stack.Items.size(); i++)
	{
		auto& Item = *_Stack.Items[i];

		if (IsLookingAtPar(Item.IR, Par->Par))
		{
			Par->Location = StackPostCall(Item.Offset);
		}
	}
}
UCodeBackEndObject::FindParsLoc UCodeBackEndObject::GetParsLoc(const Vector<IRType>& Pars)
{
	Vector<IRPar> Tep;
	Tep.resize(Pars.size());
	for (size_t i = 0; i < Pars.size(); i++)
	{
		auto& Item = Pars[i];
		auto& TepItem = Tep[i];
		TepItem.type = Item;
	}
	return GetParsLoc(Tep);
}
UCodeBackEndObject::FindParsLoc UCodeBackEndObject::GetParsLoc(const Vector<IRPar>& Pars)
{
	FindParsLoc CompilerRet;

	auto V = RegisterID::StartParameterRegister;
	RegisterID_t& V2 = (RegisterID_t&)V;
	size_t StackOffset = 0;
	bool IsUseingStack = false;

	
	for (size_t i = 0; i < Pars.size(); i++)
	{
		auto& Item = Pars[i];

		ParlocData Loc;
		Loc.Par = &Item;

		auto ParSize = _Input->GetSize(Item.type);

		if (IsUseingStack == false && ParSize <= sizeof(AnyInt64))
		{
			Loc.Location = V;

			SetRegister(V,&Item);

			V2++;
			if (V == RegisterID::EndParameterRegister)
			{
				IsUseingStack = true;
			}
		}
		else
		{
			Loc.Location = StackPreCall(StackOffset);
			StackOffset += _Input->GetSize(Item.type);
			CompilerRet.OverflowedPars.push_back(i);
		}
		CompilerRet.ParsPos.push_back(Loc);
	}



	for (size_t i = 0; i < CompilerRet.OverflowedPars.size() / 2; i++)// flip offsets
	{
		auto& Item0 = CompilerRet.ParsPos[CompilerRet.OverflowedPars[i]];
		auto& Item1 = CompilerRet.ParsPos[CompilerRet.OverflowedPars[CompilerRet.OverflowedPars.size() - 1 - i]];

		throw std::exception("bad path");
		//std::swap(Item0.StackOffset, Item1.StackOffset);
	}

	return CompilerRet;
}
void UCodeBackEndObject::BuildLink(const IRidentifier& FuncName, IRFuncLink LinkType)
{
	auto VFuncName = NameDecoratior::GetUnDecoratedName(FuncName);
	if (VFuncName == "__Log")
	{
		InstructionBuilder::Log(_Ins, RegisterID::StartParameterRegister); PushIns();
	}
	else if(VFuncName == "__LogChar")
	{
		InstructionBuilder::LogChar(_Ins, RegisterID::StartParameterRegister); PushIns();
	}
	else if (VFuncName == "__LogBuffer")
	{
		//move next Par to input
		RegToReg(IRTypes::i8, RegisterID((RegisterID_t)RegisterID::StartParameterRegister + 1), RegisterID::InPutRegister);
		
		InstructionBuilder::LogBuffer(_Ins, RegisterID::StartParameterRegister); PushIns();
	}
	else if (VFuncName == "__ReadChar")
	{
		InstructionBuilder::ReadChar(_Ins,RegisterID::OuPutRegister); PushIns();
	}
	else if (VFuncName == "__Malloc")
	{
		InstructionBuilder::Malloc(_Ins, RegisterID::StartParameterRegister, RegisterID::OuPutRegister); PushIns();
	}
	else if (VFuncName == "__Free")
	{
		InstructionBuilder::Free(_Ins, RegisterID::StartParameterRegister); PushIns();
	}
	else
	{
		auto Ptr = _Output->AddStaticBytes((String_view)VFuncName);
		InstructionBuilder::CPPCall(Ptr, _Ins); PushIns();
	}

}
UCodeLangEnd

