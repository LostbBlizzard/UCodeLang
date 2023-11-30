
#ifndef UCodeLangNoCompiler
#include "UCodeBackEnd.hpp"
#include "UCodeLang/Compilation/Helpers/InstructionBuilder.hpp"
#include "UCodeLang/Compilation/Helpers/NameDecoratior.hpp"
#include "UCodeLang/LangCore/UClib.hpp"
#include "UCodeLang/Compilation/CompilationSettings.hpp"
#include "InsHelper.hpp"
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


			size_t Offset = Item->SymType == IRSymbolType::StaticVarable ? _Output->Get_StaticBytes().size() : _Output->Get_ThreadBytes().size();
			size_t ItemSize = ItemBuf->Bytes.size();


			StaticMemoryManager::StaticMemInfo newinfo;
			newinfo.Offset = Offset;
			

			const IRDebugSymbol* Symbol = nullptr;
			if (IsDebugMode()) {
				for (auto& Item2 : _Input->_Debug.Symbols)
				{
					if (Item2.first == Item->identifier)
					{
						Symbol = &Item2.second;
						break;
					}
				}
			}

			

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


				if (Symbol)
				{
					VarableInfo U;
					U.DeclaredLine = 0;
					U.DeclaredPos = 0;
					U.FileDeclaredIn = "n/a.uc";
					
					
					if (Symbol->LangType == UCode_LangType_UCodeLang) 
					{
						BytesView bits = BytesView::Make(Symbol->TypeInfo.data(), Symbol->TypeInfo.size());
						BitReader r;
						r.SetBytes(bits.Data(), bits.Size());
						UClib::FromBytes(r, U.ReflectionType);
					}
					auto pos = VarableInfo::Static();
					pos.offset = Offset;
					U.TypeLoc = std::move(pos);
					U.VarableType = VarableInfoType::Static;
					this->_DebugInfo.Add_SetVarableName(Symbol->VarableName, std::move(U));
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

				if (Symbol)
				{
					VarableInfo U;
					U.DeclaredLine = 0;
					U.DeclaredPos = 0;
					U.FileDeclaredIn = "n/a.uc";


					if (Symbol->LangType == UCode_LangType_UCodeLang)
					{
						BytesView bits = BytesView::Make(Symbol->TypeInfo.data(), Symbol->TypeInfo.size());
						BitReader r;
						r.SetBytes(bits.Data(), bits.Size());
						UClib::FromBytes(r, U.ReflectionType);
					}
					auto pos = VarableInfo::Thread();
					pos.offset = Offset;
					U.TypeLoc = std::move(pos);
					U.VarableType = VarableInfoType::Thread;
					this->_DebugInfo.Add_SetVarableName(Symbol->VarableName, std::move(U));
				}
			}
		}
	}
}
void UCodeBackEndObject::BuildFuncs()
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

		DoPostOptimizations();
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
		for (size_t i = 0; i < _Registers.Registers.size(); i++)
		{
			auto& Item = _Registers.Registers[i];

			
			if (Item.Types.has_value())
			{
				if (auto op = Item.Types.value().Get_If<IROperator>())
				{
					if (op->Parameter == Value.Par)
					{
						r.Info =(RegisterID)i;
						break;
					}
				}
			}
		}
		UCodeLangAssert(r.Info.Get_If<RegisterID>());
	}
	else if (auto V = Value.Location.Get_If<StackPreCall>())
	{
		IRlocData_StackPre tep;
		tep.offset = V->Offset;
		r.Info = std::move(tep);
	}
	else if (auto V = Value.Location.Get_If<StackPostCall>())
	{
		IRlocData_StackPost tep;
		tep.offset = V->Offset;
		r.Info = std::move(tep);
	}
	else 
	{
		UCodeLangUnreachable();
	}
	return r;
}
UCodeBackEndObject::BinaryOpRet UCodeBackEndObject::DoBinaryOpValues(const IRInstruction* IR)
{
	RegisterID V = GetRegisterOut(IR);
	RegWillBeUsed(V);

	RegisterID A = MakeIntoRegister(IR, IR->A);


	bool ok = !_Registers.GetInfo(A).Types.has_value();
	if (ok)
	{
		SetRegister(A, IR);//So MakeIntoRegister Item->B Does not update it
	}
	//auto OldA = std::move(_Registers.GetInfo(A));
	RegisterID B = MakeIntoRegister(IR, IR->B);


	
	if (ok)
	{
		FreeRegister(A);
	}
	return { A,B,V };
}
void UCodeBackEndObject::UpdateOptimizations()
{
	auto& Settings = Get_Settings();
	bool Debug = (OptimizationFlags_t)Settings._Flags & (OptimizationFlags_t)OptimizationFlags::Debug;
	bool ForSize = (OptimizationFlags_t)Settings._Flags & (OptimizationFlags_t)OptimizationFlags::ForSize;
	bool ForSpeed = (OptimizationFlags_t)Settings._Flags & (OptimizationFlags_t)OptimizationFlags::ForSpeed;
	bool ForMaxSpeed = (OptimizationFlags_t)Settings._Flags & (OptimizationFlags_t)OptimizationFlags::ForMaxSpeed;


	if (Debug == false)
	{
		_Optimizations.InlineFunctionCopys = ForSize;
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

	if (_Optimizations.InlineFunctionCopys)
	{
		UnorderedMap<size_t, UCodeFunc*> FuncsHash;
		for (auto& Item : Funcs)
		{
			UCodeLangUnreachable();
			//Item->_Hash = UAssembly::UAssembly::BuildHashForSub(Item->_Ins.data(), Item->_Ins.size());
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
						UCodeLangUnreachable();//this may not work
						Item2->_AutoJumpTo = Item.get();

						Item2->_Ins.clear();
						Item2->FuncsToLink.clear();

						InstructionBuilder::Jumpv1(NullAddress, _Ins);
						InstructionBuilder::Jumpv2(NullAddress, _Ins);
						InstructionBuilder::Jumpv3(NullAddress, _Ins);
						InstructionBuilder::Jumpv4(NullAddress, _Ins);

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
void UCodeBackEndObject::DoPostOptimizations()
{
	return;

	for (size_t i = 0; i < _OutLayer->_Instructions.size(); i++)
	{
		auto oldindex = i;
		auto& Item = _OutLayer->_Instructions[i];
		size_t FuncSize = 0;
		while (i < _OutLayer->_Instructions.size())
		{
			if (_OutLayer->_Instructions[i].OpCode == InstructionSet::Return)
			{
				FuncSize = i - oldindex;
				break;
			}

			i++;
		}

		Span<Instruction> FuncBody = Span<Instruction>::Make(&Item,FuncSize);
		for (size_t i = 0; i < FuncBody.Size(); i++)
		{
			auto& Item = FuncBody[i];
			if (Item.OpCode == InstructionSet::Store8)
			{
				if (Item.Op_RegUInt8.B == 0) {
					auto reg = Item.Op_RegUInt8.A;
					InstructionBuilder::bitwise_Xor8(Item, reg, reg, reg);
				}
			}
			else if (Item.OpCode == InstructionSet::Store16)
			{
				if (Item.Op_RegUInt16.B == 0) {
					auto reg = Item.Op_RegUInt8.A;
					InstructionBuilder::bitwise_Xor16(Item, reg, reg, reg);
				}
			}
			else if (Item.OpCode == InstructionSet::Store32v1)
			{
				auto& Other = FuncBody[i + 1];

				if (Item.Op_RegUInt16.B == 0
					&& Other.Op_RegUInt16.B == 0) {
					auto reg = Item.Op_RegUInt8.A;
					InstructionBuilder::bitwise_Xor32(Item, reg, reg, reg);

					InstructionBuilder::DoNothing(Other);
					i++;
				}
			}
			else if (Item.OpCode == InstructionSet::Store64v1)
			{
				auto& Other = FuncBody[i + 1];
				auto& Other2 = FuncBody[i + 2];
				auto& Other3 = FuncBody[i + 3];

				if (Item.Op_RegUInt16.B == 0
					&& Other2.Op_RegUInt16.B == 0
					&& Other3.Op_RegUInt16.B == 0) {
					auto reg = Item.Op_RegUInt8.A;
					InstructionBuilder::bitwise_Xor64(Item, reg, reg, reg);

					InstructionBuilder::DoNothing(Other);

					InstructionBuilder::DoNothing(Other2);

					InstructionBuilder::DoNothing(Other3);
					i+=3;
				}
			}

			else if (Item.OpCode == InstructionSet::StoreRegToReg32)
			{
				auto input = Item.Op_TwoReg.A;
				auto output = Item.Op_TwoReg.B;
				for (int i2 = i - 1; i2 >= 0; i2--)
				{
					auto& ItemV = FuncBody[i2];
					if (InsHelper::WriteReg(ItemV, input))
					{
						InsHelper::SetWriteReg(ItemV, output);
						InstructionBuilder::DoNothing(Item);
					}
				}
			}
		}
	
	
		
	}
	for (size_t i = 0; i < _OutLayer->_Instructions.size(); i++)
	{
		auto& Item = _OutLayer->_Instructions[i];

		if (Item.OpCode == InstructionSet::DoNothing) {
			_OutLayer->_Instructions.erase(_OutLayer->_Instructions.begin() + i);
		}
	}
	
}
void UCodeBackEndObject::LinkFuncs()
{
	auto& InsList = _OutLayer->Get_Instructions();
	for (auto& Item : FuncsToLink)
	{
		Instruction& Ins = InsList[Item.Index];


		Optional<UAddress> funcpos;
		for (auto& Item2 : _Funcpos)
		{
			if (Item2._FuncID == Item._FuncID)
			{
				funcpos = Item2.Index;
				break;
			}
		}

		UCodeLangAssert(funcpos.has_value());

		UAddress tocall = funcpos.value();

		if (Ins.OpCode == InstructionSet::Callv1)
		{
			InstructionBuilder::Callv1(tocall, Ins);
			InstructionBuilder::Callv2(tocall, InsList[Item.Index + 1]);
			
			if (Get_Settings().PtrSize == IntSizes::Int64) {
				InstructionBuilder::Callv3(tocall, InsList[Item.Index + 2]);
				InstructionBuilder::Callv4(tocall, InsList[Item.Index + 3]);
			}
		}
		else if (Ins.OpCode == InstructionSet::LoadFuncPtrV1)
		{
			InstructionBuilder::LoadFuncPtr_V1(tocall, Ins.Op_OneReg.A, Ins);
			InstructionBuilder::LoadFuncPtr_V2(tocall, Ins.Op_OneReg.A, InsList[Item.Index + 1]);
			if (Get_Settings().PtrSize == IntSizes::Int64) 
			{
				InstructionBuilder::LoadFuncPtr_V3(tocall, Ins.Op_OneReg.A, InsList[Item.Index + 2]);
				InstructionBuilder::LoadFuncPtr_V4(tocall, Ins.Op_OneReg.A, InsList[Item.Index + 3]);
			}
		}
		else
		{
			UCodeLangUnreachable();
		}


	}
}
void UCodeBackEndObject::RegWillBeUsed(RegisterID Value)
{
	auto& Info = _Registers.GetInfo(Value);

	if (Info.Types.has_value())
	{
		auto& TypesValue = Info.Types.value();


		bool isreferenced = false;
		IRType type;
		if (auto Item = TypesValue.Get_If<const IRInstruction*>())
		{
			if (IsReferencedAfterThisIndex(*Item))
			{
				isreferenced =true;
				auto V = _Registers.GetFreeRegister();
				auto type = GetType(*Item);

				
				if (V.has_value() && V != Value)
				{
					SetRegister(V.value(),*Item);
					RegToReg(type._Type, Value, V.value(), false);
				}
				else 
				{
					CopyValueToStack(*Item, type, Value);
				}

				_Registers.FreeRegister(Value);
			}
		}
		else if (auto Item = TypesValue.Get_If<IROperator>())
		{
			if (IsReferencedAfterThisIndex(*Item))
			{
				isreferenced = true;
				auto V = _Registers.GetFreeRegister();
				auto type = GetType(*Item);


				if (V.has_value() && V != Value)
				{
					SetRegister(V.value(), *Item);
					RegToReg(type._Type, Value, V.value(), false);
				}
				else
				{
					IRInstruction V;
					V.Type = IRInstructionType::Load;
					V.Target() = *Item;

					MoveValueToStack(&V, type, Value);

					_Stack.Has(&V).value()->IR =*Item;
				}

				_Registers.FreeRegister(Value);
			}
		}
		else if (auto Item = TypesValue.Get_If<IRAndOperator>())
		{
			if (IsReferencedAfterThisIndex(Item->GetOp()))
			{
				isreferenced = true;
				auto V = _Registers.GetFreeRegister();
				auto type = GetType(Item->Ins,Item->GetOp());


				if (V.has_value() && V != Value)
				{
					SetRegister(V.value(), Item->Ins);
					RegToReg(type._Type, Value, V.value(), false);
				}
				else
				{
					UCodeLangToDo();
					MoveValueToStack(Item->Ins,type, Value);
				}

				_Registers.FreeRegister(Value);
			}
		}
	}
}
void UCodeBackEndObject::OnFunc(const IRFunc* IR)
{
	lookingatfunc = IR;
	//build code
	UAddress FuncStart = _OutLayer->GetLastInstruction() + 1;


	for (auto& Item : IR->Pars)
	{
		VarableInfo U;
		U.DeclaredLine = 0;
		U.DeclaredPos = 0;
		U.FileDeclaredIn = "n/a.uc";
		
		const IRDebugSymbol* Symbol = nullptr;

		IRidentifier ParId = _Input->FromID(IR->identifier) + ":" + _Input->FromID(Item.identifier);
		
		if (IsDebugMode()) 
		{
			for (auto& Item2 : _Input->_Debug.Symbols)
			{
				if (_Input->FromID(Item2.first) == ParId)
				{
					Symbol = &Item2.second;
					break;
				}
			}
		}
		if (!Symbol)
		{
			continue;
		}

		if (Symbol->LangType == UCode_LangType_UCodeLang)
		{
			BytesView bits = BytesView::Make(Symbol->TypeInfo.data(), Symbol->TypeInfo.size());
			BitReader r;
			r.SetBytes(bits.Data(), bits.Size());
			UClib::FromBytes(r, U.ReflectionType);
		};
		U.VarableType = VarableInfoType::Parameter;
		this->_DebugInfo.Add_SetVarableName(ParId, std::move(U));
	}

	auto FuncName = _Input->FromID(IR->identifier);
	{
		auto V = GetParsLoc(IR->Pars,true);
		CurrentFuncParPos =std::move(V.ParsPos);
	}


	
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

	if (IsDebugMode())
	{
		InstructionBuilder::Debug_FuncStart(_Ins); PushIns();
	}

	if (IR->Linkage != IRFuncLink::StaticLink)
	{
		BuildLink(FuncName,IR->Linkage);
	}

	if (FuncName == "Span<char>:(&Index&)^Span<char>&,Range_t<uintptr>")
	{
		int a = 0;
	}
	
	if (IR->Blocks.size())
	{
		SynchronizePars();
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
			InstructionBuilder::Store32_V2(_Ins, V, (UInt32)_Stack.Size); instr.insert(instr.begin() + FuncStart, _Ins);
			InstructionBuilder::Store32_V1(_Ins, V, (UInt32)_Stack.Size); instr.insert(instr.begin() + FuncStart, _Ins);
		}
		else
		{
			InstructionBuilder::Store64_V4(_Ins, V, (UInt64)_Stack.Size); instr.insert(instr.begin() + FuncStart, _Ins);
			InstructionBuilder::Store64_V3(_Ins, V, (UInt64)_Stack.Size); instr.insert(instr.begin() + FuncStart, _Ins);
			InstructionBuilder::Store64_V2(_Ins, V, (UInt64)_Stack.Size); instr.insert(instr.begin() + FuncStart, _Ins);
			InstructionBuilder::Store64_V1(_Ins, V, (UInt64)_Stack.Size); instr.insert(instr.begin() + FuncStart, _Ins);
		}

		


		for (size_t i = LinkedCallsIndex; i < FuncsToLink.size(); i++)
		{
			auto& Item = FuncsToLink[i];

			if (Ptr == IntSizes::Int32)
			{
				Item.Index += 3;
			}
			else
			{
				Item.Index += 5;
			}
		}
		
		for (size_t i = 0; i < _DebugInfo.DebugInfo.size(); i++)
		{
			auto& Item = _DebugInfo.DebugInfo[i];
			auto Ins = Item.Get_Ins();
			if (Ins.has_value())
			{
				Item.Set_Ins(Ins.value() + 2);
			}
		}

		
		_DebugInfo.Add_UDebugSetFuncStackFrameSize(_Stack.Size,FuncStart);
	}

	_Stack.Reset();
	_Registers.Reset();
	ClearVarableLocs();
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
	UnorderedMap<size_t, UAddress> IRToUCodeInsPost;
	UnorderedMap<size_t, UAddress> IRToUCodeInsPre;

	UnorderedMap<IRidentifierID,Optional<RegistersManager>> Jumps;
	for (size_t i = 0; i < IR->Instructions.size(); i++)
	{
		auto& I = IR->Instructions[i];
		auto jumpos = I->Target().Value.AsUIntNative;
		switch (I->Type)
		{
		case IRInstructionType::Jump:
		{
			auto jumpos = I->Target().Value.AsUIntNative;
			if (!Jumps.HasValue(jumpos))
			{
				Jumps.AddValue(jumpos, {});
			}
		}
		break;
		case IRInstructionType::ConditionalJump:
		{
			auto jumpos = I->Target().Value.AsUIntNative;
			if (!Jumps.HasValue(jumpos))
			{
				Jumps.AddValue(jumpos, {});
			}
		}break;
		}	

	}

	
	for (size_t i = 0; i < IR->Instructions.size(); i++)
	{
		if (i == 13)
		{
			int a = 0;
		}

		auto& Item_ = IR->Instructions[i];
		auto Item = Item_.get();
		Index = i;

		for (auto& JumpItem : Jumps)
		{
			if (JumpItem.first == i)
			{
				if (JumpItem.second.has_value())
				{
					MoveValuesToState(JumpItem.second.value());
				}
				else
				{
					JumpItem.second = SaveState();
				}

				RegWillBeUsed(RegisterID::LinkRegister);
			}
		}


		if (i != 0 && IR->Instructions[i - 1]->Type == IRInstructionType::ConditionalJump)
		{

			for (auto& JumpItem : Jumps)
			{
				if (JumpItem.first == i-2)
				{
					if (JumpItem.second.has_value())
					{
						MoveValuesToState(JumpItem.second.value());
					}
					else
					{
						JumpItem.second = SaveState();
					}
				}
			}


			RegWillBeUsed(RegisterID::LinkRegister);
		}

		IRToUCodeInsPre.AddValue(i,_OutLayer->_Instructions.size());
		
		if (IsDebugMode())
		{
			auto lastIRIndex = i;
			auto DebugInfo = IR->DebugInfo.Get_debugfor(lastIRIndex);
			for (auto& Item : DebugInfo)
			{
				auto InsIndex = (IRToUCodeInsPre.GetValue(i));

				//i == 0 ? _OutLayer->_Instructions.size() : _OutLayer->_Instructions.size() - 1;
				if (auto Val = Item->Debug.Get_If<IRDebugSetFile>())
				{
					Add_SetFile(Val->FileName, InsIndex);
				}
				else if (auto Val = Item->Debug.Get_If<IRDebugSetLineNumber>())
				{
					InstructionBuilder::Debug_LineEnter(_Ins); PushIns();
					Add_SetLineNumber(Val->LineNumber, InsIndex);
				}
			}
		}
		
		switch (Item->Type)
		{
		case IRInstructionType::None:
			break;
		case IRInstructionType::LoadNone:
		{
			/*
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
			*/
		}
		break;
		case IRInstructionType::Load:
		{
			auto V = GetIRLocData(Item, Item->Target());
			

			bool givename = true;
			if (!V.Info.Is<RegisterID>())
			{
				StoreValue(Item,IROperator(Item), Item->Target());
			}
			else
			{
				RegisterID reg = V.Info.Get<RegisterID>();
				if (_Registers.GetInfo(reg).Types.has_value())
				{
					auto& regva = _Registers.GetInfo(reg).Types.value();
					if (auto op = regva.Get_If<IROperator>())
					{
						if (op->Type == IROperatorType::IRParameter)
						{
							//givename = false;
							auto newreg = GetRegisterForTep();
							RegToReg(op->Parameter->type._Type, reg, newreg, false);
							V.Info = newreg;


							//StoreValue(Item, IROperator(Item), Item->Target());
						}
					}
					else 
					{
						RegWillBeUsed(reg);
					}
				}
			}

			if (givename) {
				GiveNameTo(V, Item);
			}
		}
		break;
		case IRInstructionType::LoadReturn:
		{
			auto V = GetIRLocData(Item, Item->Target());
			auto ObjectSize = GetSize(V.ObjectType);
			if (ObjectSize <= sizeof(AnyInt64))
			{
				MakeIntoRegister(V, { RegisterID::OutPutRegister });
			}
			else
			{
				MakeIntoRegister(GetPointerOf(V), { RegisterID::OutPutRegister });
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
			auto BOpVals = DoBinaryOpValues(Item);
			RegisterID A = BOpVals.A;
			RegisterID B = BOpVals.B;
			RegisterID V = BOpVals.V;

			switch (Item->ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::MultS8(_Ins, A, B,V); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::MultS16(_Ins, A, B, V); PushIns(); break;

			case IRTypes::i32:InstructionBuilder::MultS32(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i64:InstructionBuilder::MultS64(_Ins, A, B, V); PushIns(); break;

			case IRTypes::f32:InstructionBuilder::Multf32(_Ins, A, B, V); PushIns(); break;
			case IRTypes::f64:InstructionBuilder::Multf64(_Ins, A, B, V); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					InstructionBuilder::MultS32(_Ins, A, B, V); PushIns(); break;
				}
				else
				{
					InstructionBuilder::MultS64(_Ins, A, B, V); PushIns(); break;
				}
				break;
			default:
				UCodeLangUnreachable();
				break;
			}
			SetRegister(V, Item);
		}
		break;
		case IRInstructionType::UMult:
		{
			auto BOpVals = DoBinaryOpValues(Item);
			RegisterID A = BOpVals.A;
			RegisterID B = BOpVals.B;
			RegisterID V = BOpVals.V;
			auto& Object_t = Item->ObjectType;
			switch (Object_t._Type)
			{
			case IRTypes::i8:InstructionBuilder::MultU8(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::MultU16(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i32:InstructionBuilder::MultU32(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i64:InstructionBuilder::MultU64(_Ins, A, B, V); PushIns(); break;


			case IRTypes::f32:InstructionBuilder::Multf32(_Ins, A, B, V); PushIns(); break;
			case IRTypes::f64:InstructionBuilder::Multf64(_Ins, A, B, V); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					InstructionBuilder::MultU32(_Ins, A, B, V); PushIns(); break;
				}
				else
				{
					InstructionBuilder::MultU64(_Ins, A, B, V); PushIns(); break;
				}
				break;
			default:
				UCodeLangUnreachable();
				break;
			}

			SetRegister(V, Item);
		}
		break;

		case IRInstructionType::SDiv:
		{
			auto BOpVals = DoBinaryOpValues(Item);
			RegisterID A = BOpVals.A;
			RegisterID B = BOpVals.B;
			RegisterID V = BOpVals.V;
			switch (Item->ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::DivS8(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::DivS16(_Ins, A, B, V); PushIns(); break;

			case IRTypes::i32:InstructionBuilder::DivS32(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i64:InstructionBuilder::DivS64(_Ins, A, B, V); PushIns(); break;

			case IRTypes::f32:InstructionBuilder::Divf32(_Ins, A, B, V); PushIns(); break;
			case IRTypes::f64:InstructionBuilder::Divf64(_Ins, A, B, V); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					InstructionBuilder::DivS32(_Ins, A, B, V); PushIns(); break;
				}
				else
				{
					InstructionBuilder::DivS64(_Ins, A, B, V); PushIns(); break;
				}
				break;
			default:
				UCodeLangUnreachable();
				break;
			}

			SetRegister(V, Item);
		}
		break;
		case IRInstructionType::UDiv:
		{
			auto BOpVals = DoBinaryOpValues(Item);
			RegisterID A = BOpVals.A;
			RegisterID B = BOpVals.B;
			RegisterID V = BOpVals.V;
			switch (Item->ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::DivU8(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::DivU16(_Ins, A, B, V); PushIns(); break;

			case IRTypes::i32:InstructionBuilder::DivU32(_Ins, A, B, V); PushIns(); break;

			case IRTypes::i64:InstructionBuilder::DivU64(_Ins, A, B, V); PushIns(); break;

			case IRTypes::f32:InstructionBuilder::Divf32(_Ins, A, B, V); PushIns(); break;
			case IRTypes::f64:InstructionBuilder::Divf64(_Ins, A, B, V); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					InstructionBuilder::DivU32(_Ins, A, B, V); PushIns(); break;
				}
				else
				{
					InstructionBuilder::DivU64(_Ins, A, B, V); PushIns(); break;
				}
				break;
			default:
				UCodeLangUnreachable();
				break;
			}

			SetRegister(V, Item);
		}
		break;

		case IRInstructionType::Add:
		{
			auto BOpVals = DoBinaryOpValues(Item);
			RegisterID A = BOpVals.A;
			RegisterID B = BOpVals.B;
			RegisterID V = BOpVals.V;
			switch (Item->ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::Add8(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::Add16(_Ins, A, B, V); PushIns(); break;

			case IRTypes::i32:InstructionBuilder::Add32(_Ins, A, B, V); PushIns(); break;

			case IRTypes::i64:InstructionBuilder::Add64(_Ins, A, B, V); PushIns(); break;

			case IRTypes::f32:InstructionBuilder::Addf32(_Ins, A, B, V); PushIns(); break;
			case IRTypes::f64:InstructionBuilder::Addf64(_Ins, A, B, V); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					InstructionBuilder::Add32(_Ins, A, B, V); PushIns(); break;
				}
				else
				{
					InstructionBuilder::Add64(_Ins, A, B, V); PushIns(); break;
				}
				break;
			default:
				UCodeLangUnreachable();
				break;
			}
			SetRegister(V, Item);
		}
		break;
		case IRInstructionType::Sub:
		{
			auto BOpVals = DoBinaryOpValues(Item);
			RegisterID A = BOpVals.A;
			RegisterID B = BOpVals.B;
			RegisterID V = BOpVals.V;
			
			switch (Item->ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::Sub8(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::Sub16(_Ins, A, B, V); PushIns(); break;

			case IRTypes::i32:InstructionBuilder::Sub32(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i64:InstructionBuilder::Sub64(_Ins, A, B, V); PushIns(); break;

			case IRTypes::f32:InstructionBuilder::Subf32(_Ins, A, B, V); PushIns(); break;
			case IRTypes::f64:InstructionBuilder::Subf64(_Ins, A, B, V); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					InstructionBuilder::Sub32(_Ins, A, B, V); PushIns(); break;
				}
				else
				{
					InstructionBuilder::Sub64(_Ins, A, B, V); PushIns(); break;
				}
				break;
			default:
				UCodeLangUnreachable();
				break;
			}

			SetRegister(V, Item);
		}
		break;
		case IRInstructionType::PushParameter:
		{
			RegisterID_t& Reg = *(RegisterID_t*)&_InputPar;

			
			if (Reg < (RegisterID_t)RegisterID::EndParameterRegister)
			{
				if (IsPrimitive(GetType(Item)))
				{
					RegWillBeUsed(_InputPar);
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
			auto FuncInfo = _Input->GetFunc(Item->Target().identifier);
			auto FData = FuncCallStart(FuncInfo->Pars, FuncInfo->ReturnType);

			InstructionBuilder::Callv1(NullAddress, _Ins); PushIns();

			FuncInsID Tep;
			Tep.Index = _OutLayer->Get_Instructions().size() - 1;
			Tep._FuncID = Item->Target().identifier;

			FuncsToLink.push_back(Tep);

			InstructionBuilder::Callv2(NullAddress, _Ins); PushIns();

			if (Get_Settings().PtrSize == IntSizes::Int64) {
				InstructionBuilder::Callv3(NullAddress, _Ins); PushIns();
				InstructionBuilder::Callv4(NullAddress, _Ins); PushIns();
			}

			FuncCallEnd(FData);
			GiveFuncReturnName(FuncInfo->ReturnType, Item);
		}
		break;
		case IRInstructionType::MallocCall:
		{
			RegisterID ID = GetRegisterForTep();
			SetRegister(ID, Item);
			auto SizeReg = MakeIntoRegister(Item, Item->Target());
			InstructionBuilder::Malloc(_Ins, SizeReg, ID); PushIns();

			SetRegister(ID, Item);
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
		{
			auto jumpos = Item->Target().Value.AsUIntNative;
			auto& jumpdata = Jumps.GetValue(jumpos);

			RegWillBeUsed(RegisterID::LinkRegister);

			if (jumpdata.has_value())
			{
				MoveValuesToState(jumpdata.value());
			}
			if (!jumpdata.has_value())
			{
				jumpdata = SaveState();
			}

			InstructionBuilder::Jumpv1(NullAddress, _Ins); PushIns();
			InsToUpdate.push_back({ _OutLayer->Get_Instructions().size(),jumpos});

			InstructionBuilder::Jumpv2(NullAddress, _Ins); PushIns();

			if (Get_Settings().PtrSize == IntSizes::Int64) {
				InstructionBuilder::Jumpv3(NullAddress, _Ins); PushIns();
				InstructionBuilder::Jumpv4(NullAddress, _Ins); PushIns();
			}


			
		}
			break;
		case IRInstructionType::ConditionalJump:
		{
			auto jumpos = Item->Target().Value.AsUIntNative;

			RegWillBeUsed(RegisterID::LinkRegister);
			this->_Registers.FreeRegister(RegisterID::LinkRegister);

			auto reg = MakeIntoRegister(Item, Item->Input());
			if (reg == RegisterID::LinkRegister)
			{
				auto otherreg = RegisterID::B;
				RegWillBeUsed(otherreg);
				RegToReg(GetType(Item, Item->Input())._Type, reg, otherreg, false);

				reg = otherreg;
			}

			InstructionBuilder::Jumpv1(NullAddress, _Ins); PushIns();
			InsToUpdate.push_back({ _OutLayer->Get_Instructions().size(),  jumpos });


			if (Get_Settings().PtrSize == IntSizes::Int64)
			{
				InstructionBuilder::Jumpv2(NullAddress, _Ins); PushIns();
				InstructionBuilder::Jumpv3(NullAddress, _Ins); PushIns();
				InstructionBuilder::Jumpifv4(NullAddress, reg, _Ins); PushIns();
			}
			else
			{
				InstructionBuilder::Jumpifv2(NullAddress, reg, _Ins); PushIns();
			}


			auto& jumpdata = Jumps.GetValue(jumpos);
			if (jumpdata.has_value())
			{
				MoveValuesToState(jumpdata.value());
			}
			else
			{
				jumpdata = SaveState();
			}

			this->_Registers.FreeRegister(RegisterID::LinkRegister);
		}	break;
		case IRInstructionType::Logical_Not:
		{	
			RegisterID Out = GetRegisterForTep();
			auto optype = GetType(Item, Item->Target());

			LogicalNot(optype._Type, MakeIntoRegister(Item, Item->Target()),Out);
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
			auto optype = GetType(Item->Target());

			auto BOpVals = DoBinaryOpValues(Item);
			RegisterID A = BOpVals.A;
			RegisterID B = BOpVals.B;
			RegisterID V = BOpVals.V;

			switch (optype._Type)
			{
			case IRTypes::i8:InstructionBuilder::equalto8(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::equalto16(_Ins, A, B,V); PushIns(); break;
			case IRTypes::i32:InstructionBuilder::equalto32(_Ins, A, B,V); PushIns(); break;
			case IRTypes::i64:InstructionBuilder::equalto64(_Ins, A, B,V); PushIns(); break;
			case IRTypes::f32:InstructionBuilder::equaltof32(_Ins, A, B, V); PushIns(); break;
			case IRTypes::f64:InstructionBuilder::equaltof64(_Ins, A, B, V); PushIns(); break;
			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					InstructionBuilder::equalto32(_Ins, A, B, V); PushIns(); break;
				}
				else
				{
					InstructionBuilder::equalto64(_Ins, A, B, V); PushIns(); break;
				}
				break;
			default:
				UCodeLangUnreachable();
				break;
			}

			SetRegister(V,Item);
		}
		break;
		case IRInstructionType::CallFuncPtr:
		{
			auto reg = MakeIntoRegister(Item, Item->Target());

			auto FuncInfo = _Input->GetSymbol(GetType(Item->Target())._symbol)->Get_ExAs<IRFuncPtr>();
			auto FData = FuncCallStart(FuncInfo->Pars, FuncInfo->Ret);
 
			InstructionBuilder::CallReg(reg, _Ins); PushIns();


			FuncCallEnd(FData);
			GiveFuncReturnName(FuncInfo->Ret, Item);
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
			BuildUIntToIntCast(Item, Item->Target(), 1);
		}
		break;
		case IRInstructionType::UIntToUInt16:
		{
			BuildUIntToIntCast(Item, Item->Target(), 2);
		}
		break;
		case IRInstructionType::UIntToUInt32:
		{
			BuildUIntToIntCast(Item, Item->Target(), 4);
		}
		break;
		case IRInstructionType::UIntToUInt64:
		{
			BuildUIntToIntCast(Item, Item->Target(), 8);
		}
		break;

		case IRInstructionType::SGreaterThanOrEqual:
		case IRInstructionType::UGreaterThanOrEqual:	
		{
			auto optype = Item->ObjectType;

			auto BOpVals = DoBinaryOpValues(Item);
			RegisterID A = BOpVals.A;
			RegisterID B = BOpVals.B;
			RegisterID V = BOpVals.V;

			switch (optype._Type)
			{
			case IRTypes::i8:InstructionBuilder::equal_greaterthan8(_Ins, A, B,V); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::equal_greaterthan16(_Ins, A, B,V); PushIns(); break;

			case IRTypes::i32:InstructionBuilder::equal_greaterthan32(_Ins, A, B,V); PushIns(); break;
			case IRTypes::i64:InstructionBuilder::equal_greaterthan64(_Ins, A, B,V); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					InstructionBuilder::equal_greaterthan32(_Ins, A, B,V); PushIns(); 
				}
				else
				{
					InstructionBuilder::equal_greaterthan64(_Ins, A, B,V); PushIns();
				}
				break;
			default:
				UCodeLangUnreachable();
				break;
			}

			SetRegister(V, Item);
		}
		break;
		case IRInstructionType::ULessThan:
		case IRInstructionType::SLessThan:
		{
			auto optype = Item->ObjectType;

			auto BOpVals = DoBinaryOpValues(Item);
			RegisterID A = BOpVals.A;
			RegisterID B = BOpVals.B;
			RegisterID V = BOpVals.V;

			switch (optype._Type)
			{
			case IRTypes::i8:InstructionBuilder::lessthan8(_Ins, A, B,V); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::lessthan16(_Ins, A, B,V); PushIns(); break;

			case IRTypes::i32:InstructionBuilder::lessthan32(_Ins, A, B,V); PushIns(); break;
			
			case IRTypes::i64:InstructionBuilder::lessthan64(_Ins, A, B,V); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					InstructionBuilder::lessthan32(_Ins, A, B,V); PushIns(); break;
				}
				else
				{
					InstructionBuilder::lessthan64(_Ins, A, B,V); PushIns(); break;
				}
				break;
			default:
				UCodeLangUnreachable();
				break;
			}

			SetRegister(V, Item);
		}
		break;
		case IRInstructionType::SGreaterThan:
		case IRInstructionType::UGreaterThan:
		{
			auto optype = Item->ObjectType;

			auto BOpVals = DoBinaryOpValues(Item);
			RegisterID A = BOpVals.A;
			RegisterID B = BOpVals.B;
			RegisterID V = BOpVals.V;

			switch (optype._Type)
			{
			case IRTypes::i8:InstructionBuilder::greaterthan8(_Ins, A, B,V); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::greaterthan16(_Ins, A, B,V); PushIns(); break;

			case IRTypes::i32:InstructionBuilder::greaterthan32(_Ins, A, B,V); PushIns(); break;

			case IRTypes::i64:InstructionBuilder::greaterthan64(_Ins, A, B,V); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					InstructionBuilder::greaterthan32(_Ins, A, B,V); PushIns(); break;
				}
				else
				{
					InstructionBuilder::greaterthan64(_Ins, A, B,V); PushIns(); break;
				}
				break;
			default:
				UCodeLangUnreachable();
				break;
			}

			SetRegister(V, Item);
		}
		break;
		case IRInstructionType::SLessThanOrEqual:
		case IRInstructionType::ULessThanOrEqual:
		{
			auto optype = Item->ObjectType;

			auto BOpVals = DoBinaryOpValues(Item);
			RegisterID A = BOpVals.A;
			RegisterID B = BOpVals.B;
			RegisterID V = BOpVals.V;

			switch (optype._Type)
			{
			case IRTypes::i8:InstructionBuilder::equal_greaterthan8(_Ins, A, B,V); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::equal_greaterthan16(_Ins, A, B,V); PushIns(); break;

			case IRTypes::i32:InstructionBuilder::equal_greaterthan32(_Ins, A, B,V); PushIns(); break;

			case IRTypes::i64:InstructionBuilder::equal_greaterthan64(_Ins, A, B,V); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					InstructionBuilder::equal_greaterthan32(_Ins, A, B,V); PushIns(); break;
				}
				else
				{
					InstructionBuilder::equal_greaterthan64(_Ins, A, B,V); PushIns(); break;
				}
				break;
			default:
				UCodeLangUnreachable();
				break;
			}

			SetRegister(V, Item);
		}
		break;
		case IRInstructionType::NotEqualTo:
		{
			auto optype = Item->ObjectType;

			auto BOpVals = DoBinaryOpValues(Item);
			RegisterID A = BOpVals.A;
			RegisterID B = BOpVals.B;
			RegisterID V = BOpVals.V;

			switch (optype._Type)
			{
			case IRTypes::i8:InstructionBuilder::notequalto8(_Ins, A, B,V); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::notequalto16(_Ins, A, B,V); PushIns(); break;

			case IRTypes::i32:InstructionBuilder::notequalto32(_Ins, A, B,V); PushIns(); break;

			case IRTypes::i64:InstructionBuilder::notequalto64(_Ins, A, B,V); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					InstructionBuilder::notequalto32(_Ins, A, B,V); PushIns(); break;
				}
				else
				{
					InstructionBuilder::notequalto64(_Ins, A, B,V); PushIns(); break;
				}
				break;
			default:
				UCodeLangUnreachable();
				break;
			}

			FreeRegister(A);
			SetRegister(V, Item);
		}
		break;
		case IRInstructionType::Logical_And:
		{
			auto optype = Item->ObjectType;

			auto BOpVals = DoBinaryOpValues(Item);
			RegisterID A = BOpVals.A;
			RegisterID B = BOpVals.B;
			RegisterID V = BOpVals.V;

			switch (optype._Type)
			{
			case IRTypes::i8:InstructionBuilder::LogicalAnd8(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::LogicalAnd16(_Ins, A, B, V); PushIns(); break;

			case IRTypes::i32:InstructionBuilder::LogicalAnd32(_Ins, A, B, V); PushIns(); break;

			case IRTypes::i64:InstructionBuilder::LogicalAnd64(_Ins, A, B, V); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					InstructionBuilder::LogicalAnd32(_Ins, A, B, V); PushIns(); break;
				}
				else
				{
					InstructionBuilder::LogicalAnd64(_Ins, A, B, V); PushIns(); break;
				}
				break;
			default:
				UCodeLangUnreachable();
				break;
			}

			FreeRegister(A);
			SetRegister(V, Item);
		}
		break;
		case IRInstructionType::Unreachable:
		{
			if (IsDebugMode())
			{
				InstructionBuilder::DoNothing(_Ins); PushIns();
			}
		}
		break;
		case IRInstructionType::Ui32Tof32:	
		{
			auto V = MakeIntoRegister(Item, Item->Target());
			auto R = GetRegisterForTep();
			InstructionBuilder::UInt32Tofloat32(_Ins, V, R); PushIns();
			SetRegister(R, Item);
		}
		break;
		case IRInstructionType::Si32Tof32:
		{
			auto V = MakeIntoRegister(Item, Item->Target());
			auto R = GetRegisterForTep();
			InstructionBuilder::SInt32Tofloat32(_Ins, V, R); PushIns();
			SetRegister(R, Item);
		}
		break;
		case IRInstructionType::f32Toi32:
		{
			auto V = MakeIntoRegister(Item, Item->Target());
			auto R = GetRegisterForTep();
			InstructionBuilder::float32ToInt32(_Ins, V, R); PushIns();
			SetRegister(R, Item);
		}
		break;
		case IRInstructionType::Ui64Tof64:
		{
			auto V = MakeIntoRegister(Item, Item->Target());
			auto R = GetRegisterForTep();
			InstructionBuilder::UInt64Tofloat64(_Ins, V, R); PushIns();
			SetRegister(R, Item);
		}
		break;
		case IRInstructionType::Si64Tof64:
		{
			auto V = MakeIntoRegister(Item, Item->Target());
			auto R = GetRegisterForTep();
			InstructionBuilder::SInt64Tofloat64(_Ins, V, R); PushIns();
			SetRegister(R, Item);
		}
		break;
		case IRInstructionType::f64Toi64:
		{
			auto V = MakeIntoRegister(Item, Item->Target());
			auto R = GetRegisterForTep();
			InstructionBuilder::float64ToInt64(_Ins, V, R); PushIns();
			SetRegister(R, Item);
		}
		break;
		case IRInstructionType::f64Tof32:
		{
			auto V = MakeIntoRegister(Item, Item->Target());
			auto R = GetRegisterForTep();
			InstructionBuilder::float64Tofloat32(_Ins, V, R); PushIns();
			SetRegister(R, Item);
		}
		break;
		case IRInstructionType::f32Tof64:
		{
			auto V = MakeIntoRegister(Item, Item->Target());
			auto R = GetRegisterForTep();
			InstructionBuilder::float32Tofloat64(_Ins, V, R); PushIns();
			SetRegister(R, Item);
		}
		break;
		case IRInstructionType::BitWise_ShiftL:
		{
			auto BOpVals = DoBinaryOpValues(Item);
			RegisterID A = BOpVals.A;
			RegisterID B = BOpVals.B;
			RegisterID V = BOpVals.V;
			switch (Item->ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::bitwise_LeftShift8(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::bitwise_LeftShift16(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i32:InstructionBuilder::bitwise_LeftShift32(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i64:InstructionBuilder::bitwise_LeftShift64(_Ins, A, B, V); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					InstructionBuilder::bitwise_LeftShift32(_Ins, A, B, V); PushIns(); break;
				}
				else
				{
					InstructionBuilder::bitwise_LeftShift64(_Ins, A, B, V); PushIns(); break;
				}
				break;
			default:
				UCodeLangUnreachable();
				break;
			}
			SetRegister(V, Item);
		}
		break;
		case IRInstructionType::BitWise_ShiftR:
		{
			auto BOpVals = DoBinaryOpValues(Item);
			RegisterID A = BOpVals.A;
			RegisterID B = BOpVals.B;
			RegisterID V = BOpVals.V;
			switch (Item->ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::bitwise_RightShift8(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::bitwise_RightShift16(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i32:InstructionBuilder::bitwise_RightShift32(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i64:InstructionBuilder::bitwise_RightShift64(_Ins, A, B, V); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					InstructionBuilder::bitwise_RightShift32(_Ins, A, B, V); PushIns(); break;
				}
				else
				{
					InstructionBuilder::bitwise_RightShift64(_Ins, A, B, V); PushIns(); break;
				}
				break;
			default:
				UCodeLangUnreachable();
				break;
			}
			SetRegister(V, Item);
		}
		break;
		case IRInstructionType::BitWise_Or:
		{
			auto BOpVals = DoBinaryOpValues(Item);
			RegisterID A = BOpVals.A;
			RegisterID B = BOpVals.B;
			RegisterID V = BOpVals.V;
			switch (Item->ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::bitwise_or8(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::bitwise_or16(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i32:InstructionBuilder::bitwise_or32(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i64:InstructionBuilder::bitwise_or64(_Ins, A, B, V); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					InstructionBuilder::bitwise_or32(_Ins, A, B, V); PushIns(); break;
				}
				else
				{
					InstructionBuilder::bitwise_or64(_Ins, A, B, V); PushIns(); break;
				}
				break;
			default:
				UCodeLangUnreachable();
				break;
			}
			SetRegister(V, Item);
		}
		break;
		case IRInstructionType::BitWise_And:
		{
			auto BOpVals = DoBinaryOpValues(Item);
			RegisterID A = BOpVals.A;
			RegisterID B = BOpVals.B;
			RegisterID V = BOpVals.V;
			switch (Item->ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::bitwise_and8(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::bitwise_and16(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i32:InstructionBuilder::bitwise_and32(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i64:InstructionBuilder::bitwise_and64(_Ins, A, B, V); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					InstructionBuilder::bitwise_and32(_Ins, A, B, V); PushIns(); break;
				}
				else
				{
					InstructionBuilder::bitwise_and64(_Ins, A, B, V); PushIns(); break;
				}
				break;
			default:
				UCodeLangUnreachable();
				break;
			}
			SetRegister(V, Item);
		}
		break;
		case IRInstructionType::BitWise_XOr:
		{
			auto BOpVals = DoBinaryOpValues(Item);
			RegisterID A = BOpVals.A;
			RegisterID B = BOpVals.B;
			RegisterID V = BOpVals.V;
			switch (Item->ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::bitwise_Xor8(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::bitwise_Xor16(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i32:InstructionBuilder::bitwise_Xor32(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i64:InstructionBuilder::bitwise_Xor64(_Ins, A, B, V); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					InstructionBuilder::bitwise_Xor32(_Ins, A, B, V); PushIns(); break;
				}
				else
				{
					InstructionBuilder::bitwise_Xor64(_Ins, A, B, V); PushIns(); break;
				}
				break;
			default:
				UCodeLangUnreachable();
				break;
			}
			SetRegister(V, Item);
		}
		break;
		case IRInstructionType::BitWise_Not:
		{
			RegisterID Out = GetRegisterForTep();
			auto optype = GetType(Item, Item->Target());

			auto V = MakeIntoRegister(Item, Item->Target());

			switch (optype._Type)
			{
			case IRTypes::i8:InstructionBuilder::bitwise_Not8(_Ins, V, Out); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::bitwise_Not16(_Ins, V, Out); PushIns(); break;

			case IRTypes::i32:InstructionBuilder::bitwise_Not32(_Ins, V, Out); PushIns(); break;

			case IRTypes::i64:InstructionBuilder::bitwise_Not64(_Ins, V, Out); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					InstructionBuilder::bitwise_Not32(_Ins, V, Out); PushIns(); break;
				}
				else
				{
					InstructionBuilder::bitwise_Not64(_Ins, V, Out); PushIns(); break;
				}
				break;
			default:
				UCodeLangUnreachable();
				break;
			}
			SetRegister(Out, Item);

		}
		break;
		case IRInstructionType::SMod:
		{
			auto BOpVals = DoBinaryOpValues(Item);
			RegisterID A = BOpVals.A;
			RegisterID B = BOpVals.B;
			RegisterID V = BOpVals.V;
			switch (Item->ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::ModS8(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::ModS16(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i32:InstructionBuilder::ModS32(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i64:InstructionBuilder::ModS64(_Ins, A, B, V); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					InstructionBuilder::ModS32(_Ins, A, B, V); PushIns(); break;
				}
				else
				{
					InstructionBuilder::ModS64(_Ins, A, B, V); PushIns(); break;
				}
				break;
			default:
				UCodeLangUnreachable();
				break;
			}
			SetRegister(V, Item);
		}
		break;
		case IRInstructionType::UMod:
		{
			auto BOpVals = DoBinaryOpValues(Item);
			RegisterID A = BOpVals.A;
			RegisterID B = BOpVals.B;
			RegisterID V = BOpVals.V;
			switch (Item->ObjectType._Type)
			{
			case IRTypes::i8:InstructionBuilder::ModU8(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i16:InstructionBuilder::ModU16(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i32:InstructionBuilder::ModU32(_Ins, A, B, V); PushIns(); break;
			case IRTypes::i64:InstructionBuilder::ModU64(_Ins, A, B, V); PushIns(); break;

			case IRTypes::pointer:
				if (Get_Settings().PtrSize == IntSizes::Int32)
				{
					InstructionBuilder::ModU32(_Ins, A, B, V); PushIns(); break;
				}
				else
				{
					InstructionBuilder::ModU64(_Ins, A, B, V); PushIns(); break;
				}
				break;
			default:
				UCodeLangUnreachable();
				break;
			}
			SetRegister(V, Item);
		}
		break;
	default:
			UCodeLangUnreachable();
			break;
		}

	
		
		//UpdateVarableLocs();

		
		
		IRToUCodeInsPost.AddValue(i,_OutLayer->Get_Instructions().size()-1);
	}
DoneLoop:
	IRToUCodeInsPre.GetOrAdd(IR->Instructions.size()-1, _OutLayer->Get_Instructions().size());

	DropStack();
	DropPars();

	IRToUCodeInsPost.GetOrAdd(IR->Instructions.size()-1,_OutLayer->Get_Instructions().size());

	if (IsDebugMode())
	{
		InstructionBuilder::Debug_FuncEnd(_Ins); PushIns();
	}
	InstructionBuilder::Return(ExitState::Success, _Ins); PushIns();

	
	UpdateVarableLocs();

	for (auto& Item : InsToUpdate)
	{
		auto& Inst = _OutLayer->Get_Instructions();
		size_t Index = Item.InsToUpdate - 1;

		auto offset = Get_Settings().PtrSize == IntSizes::Int32 ? 1 : 3;
		Instruction& Ins = _OutLayer->Get_Instructions()[Index+ offset];
		UAddress JumpPos = IRToUCodeInsPre.GetValue(Item.Jumpto)-1;	
		UAddress JumpPos2 = IRToUCodeInsPost.GetValue(Item.Jumpto-1);
		
		Instruction& InsToJumpOn = _OutLayer->Get_Instructions()[JumpPos +1];
		

		if (_Stack.Size)
		{
			if (Get_Settings().PtrSize == IntSizes::Int32)
			{
				JumpPos += 3;
			}
			else
			{
				JumpPos += 5;
			}
		}
	
		//Jump on real Ins and not intermediate.
		if (Ins.OpCode == InstructionSet::Store32v2
			|| Ins.OpCode == InstructionSet::Callv2
			|| Ins.OpCode == InstructionSet::Jumpv2
			|| Ins.OpCode == InstructionSet::LoadFuncPtrV2
			|| Ins.OpCode == InstructionSet::Storef32v2)
		{
			JumpPos -= 1;
		}
		else if (Ins.OpCode == InstructionSet::Store64v3
			|| Ins.OpCode == InstructionSet::Storef64v3
			|| Ins.OpCode == InstructionSet::Callv3
			|| Ins.OpCode == InstructionSet::Jumpv3
			|| Ins.OpCode == InstructionSet::LoadFuncPtrV3
			)
		{
			JumpPos -= 3;
		}


		if (Ins.OpCode != InstructionSet::Jumpif) 
		{
			if (Get_Settings().PtrSize == IntSizes::Int32)
			{
				JumpPos += 1;//not sure why this needs to be here
			}

			InstructionBuilder::Jumpv1(JumpPos, Inst[Index]);
			InstructionBuilder::Jumpv2(JumpPos, Inst[Index + 1]);

			if (Get_Settings().PtrSize == IntSizes::Int64) 
			{
				InstructionBuilder::Jumpv3(JumpPos, Inst[Index + 2]);
				InstructionBuilder::Jumpv4(JumpPos, Inst[Index + 3]);
			}
		}
		else
		{
			InstructionBuilder::Jumpv1(JumpPos, Inst[Index]);
			
			if (Get_Settings().PtrSize == IntSizes::Int64)
			{
				InstructionBuilder::Jumpv2(JumpPos, Inst[Index + 1]);
				InstructionBuilder::Jumpv3(JumpPos, Inst[Index + 2]);
				InstructionBuilder::Jumpifv4(JumpPos, Ins.Op_RegUInt16.A, Inst[Index + 3]);
			}
			else
			{

				InstructionBuilder::Jumpifv2(JumpPos, Ins.Op_RegUInt16.A, Inst[Index + 1]);
			}
		}
	}
}
void UCodeBackEndObject::GiveFuncReturnName(const IRType& ReturnType, const IRInstruction* Item)
{
	if (ReturnType._Type != IRTypes::Void) {
		if (GetSize(ReturnType) <= sizeof(AnyInt64))
		{
			SetRegister(RegisterID::OutPutRegister, Item);
		}
		else
		{
			IRlocData CompilerRet;
			CompilerRet.ObjectType = ReturnType;
			CompilerRet.Info = RegisterID::OutPutRegister;

			IRlocData tep = GetFreeStackLoc(ReturnType);

			CopyValues(CompilerRet, tep, true, false);
			CompilerRet = tep;

			_Stack.Get(tep.Info.Get<IRlocData_StackPost>().offset).value()->IR = Item;
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
Optional< RegisterID> UCodeBackEndObject::FindIRInRegister(const IRInstruction* Value, bool GetAddress)
{
	if (GetAddress) { return {}; }
	auto R = _Registers.GetInfo(Value);
	if (R.has_value())
	{
		return R.value();
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
	for (RegisterID_t i = (RegisterID_t)RegisterID::StartRegister; i < (RegisterID_t)RegisterID::EndRegister+1; i++)
	{
		RegisterID CompilerRet = (RegisterID)i;
		auto& Item = _Registers.Registers[i];

		bool WasSet = false;
		if (Item.Types.has_value())
		{
			auto& ItemTypes = Item.Types.value();

			bool isreferenced = false;
			IRType type;

			if (auto IRV = ItemTypes.Get_If<const IRInstruction*>())
			{
				auto& IR = *IRV;
				isreferenced = IsReferencedAfterThisIndex(IR);
				type = GetType(IR);
			}
			else if (auto IRV = ItemTypes.Get_If<IROperator>())
			{
				if (IRV->Type != IROperatorType::Null) {
					isreferenced = IsReferencedAfterThisIndex(*IRV);
					type = GetType(*IRV);
				}
			}

			if (isreferenced)
			{
				WasSet = true;
				auto Size = GetSize(type);
				if (Size <= 1)
				{
					InstructionBuilder::Push8(_Ins, CompilerRet); PushIns();
				}
				else if (Size <= 2)
				{
					InstructionBuilder::Push16(_Ins, CompilerRet); PushIns();
				}
				else if (Size <= 4)
				{
					InstructionBuilder::Push32(_Ins, CompilerRet); PushIns();
				}
				else if (Size <= 8)
				{
					InstructionBuilder::Push64(_Ins, CompilerRet); PushIns();
				}
				else
				{
					UCodeLangUnreachable();
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
		RegWillBeUsed(RegisterID::OutPutRegister);
	}
}
void UCodeBackEndObject::FuncCallEnd(UCodeBackEndObject::FuncCallEndData& Data)
{
	auto& old = _Registers.Registers;
	auto V = GetParsLoc(Data.Pars);

	_Registers.Registers = old;

	size_t PopBufferSize = 0;
	for (size_t i = 0; i < V.OverflowedPars.size(); i++)
	{
		auto& Item = V.ParsPos[V.OverflowedPars[i]];
		PopBufferSize += GetSize(Item.Par->type);
	}


	if (PopBufferSize != 0)
	{
		_Stack.PushedOffset -= PopBufferSize;
		RegisterID PopRegister = GetRegisterForTep();
		bool dopop = IsDebugMode();

		auto loadsize = Get_Settings().PtrSize == IntSizes::Int32 ? 2 : 4;
		if (dopop == false)
		{
			size_t InsCount = 0;
			size_t PopBufferSize = 0;
			while (PopBufferSize != 0)
			{
				if (PopBufferSize >= 8)
				{
					InsCount++;
					PopBufferSize -= 8;
				}
				else if (PopBufferSize >= 4)
				{
					InsCount++;
					PopBufferSize -= 4;
				}
				else if (PopBufferSize >= 2)
				{
					InsCount++;
					PopBufferSize -= 2;
				}
				else if (PopBufferSize >= 1)
				{
					InsCount++;
					PopBufferSize -= 1;
				}
			}
			dopop = InsCount < loadsize;
		}
		if (dopop)
		{


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
				else if (PopBufferSize >= 1)
				{
					InstructionBuilder::Pop8(_Ins, PopRegister);
					PopBufferSize -= 1;
				}
				PushIns();
			}


		}
		else
		{
			
			if (Get_Settings().PtrSize == IntSizes::Int32)
			{
				InstructionBuilder::Store32_V1(_Ins,PopRegister, (UInt32)PopBufferSize); PushIns();
				InstructionBuilder::Store32_V2(_Ins,PopRegister, (UInt32)PopBufferSize); PushIns();
			}
			else
			{
				InstructionBuilder::Store64_V1(_Ins, PopRegister, (UInt64)PopBufferSize); PushIns();
				InstructionBuilder::Store64_V2(_Ins, PopRegister, (UInt64)PopBufferSize); PushIns();
				InstructionBuilder::Store64_V3(_Ins, PopRegister, (UInt64)PopBufferSize); PushIns();
				InstructionBuilder::Store64_V4(_Ins, PopRegister, (UInt64)PopBufferSize); PushIns();
			}
			InstructionBuilder::DecrementStackPointer(_Ins, PopRegister);
			PushIns();
		}
	}
	


	_InputPar = RegisterID::StartParameterRegister;



	for (int i = (int)RegisterID::EndRegister +1 - 1; i >= (int)RegisterID::StartRegister; i--)
	{
		RegisterID CompilerRet = (RegisterID)i;
		auto& Item = _Registers.Registers[i];
		if (Item.Types.has_value())
		{
			auto& ItemTypes = Item.Types.value();

			bool isreferenced = false;
			IRType type;

			if (auto IRV = ItemTypes.Get_If<const IRInstruction*>())
			{
				auto& IR = *IRV;
				isreferenced = IsReferencedAfterThisIndex(IR);
				type = GetType(IR);
			}
			else if (auto IRV = ItemTypes.Get_If<IROperator>())
			{
				isreferenced = IsReferencedAfterThisIndex(*IRV);
				type = GetType(*IRV);
			}

			if (isreferenced)
			{
				auto Size = GetSize(type);
				if (Size <= 1)
				{
					InstructionBuilder::Pop8(_Ins, CompilerRet); PushIns();
				}
				else if (Size <= 2)
				{
					InstructionBuilder::Pop16(_Ins, CompilerRet); PushIns();
				}
				else if (Size <= 4)
				{
					InstructionBuilder::Pop32(_Ins, CompilerRet); PushIns();
				}
				else if (Size <= 8)
				{
					InstructionBuilder::Pop64(_Ins, CompilerRet); PushIns();
				}
				else
				{
					UCodeLangUnreachable();
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
		if (V == RegisterID::OutPutRegister)
		{
			V = RegisterID::LinkRegister;//any  Register but OutPutRegister
		}
		if (Ptr == IntSizes::Int32)
		{
			InstructionBuilder::Store32_V1(_Ins, V, (UInt32)_Stack.Size); PushIns();
			InstructionBuilder::Store32_V2(_Ins, V, (UInt32)_Stack.Size); PushIns();
		}
		else
		{
			InstructionBuilder::Store64_V1(_Ins, V, (UInt64)_Stack.Size); PushIns();
			InstructionBuilder::Store64_V2(_Ins, V, (UInt64)_Stack.Size); PushIns();
			InstructionBuilder::Store64_V3(_Ins, V, (UInt64)_Stack.Size); PushIns();
			InstructionBuilder::Store64_V4(_Ins, V, (UInt64)_Stack.Size); PushIns();
		}
		InstructionBuilder::DecrementStackPointer(_Ins, V); PushIns();
	}

	for (auto& Item: _Stack.Reupdates)
	{
		Instruction& Ins = _OutLayer->Get_Instructions()[Item.InsIndex];
		//if (Item.PostFunc)
		{
			size_t NewOffset = 0;
			size_t PointerSize = (Get_Settings().PtrSize == IntSizes::Int32)
					? 4 : 8;
			if (!Item.PostFunc)
			{
				
				NewOffset = _Stack.Size + PointerSize + Item.StackOffset;
			}
			else
			{
				NewOffset = _Stack.Size - Item.StackOffset;
			}

			UCodeLangAssert(NewOffset <= UInt16_MaxSize);//update this so it can work with all uintptr values

			switch (Ins.OpCode)
			{
			case InstructionSet::GetPointerOfStackSub:
			{
				InstructionBuilder::GetPointerOfStackSub(Ins,Ins.Op_RegUInt16.A, NewOffset);
			}
			break;
			case InstructionSet::GetFromStackSub64:
			{
				InstructionBuilder::GetFromStackSub64(Ins,NewOffset, Ins.Op_RegUInt16.A);
			}
			break;
			case InstructionSet::GetFromStackSub32:
			{
				InstructionBuilder::GetFromStackSub32(Ins, NewOffset, Ins.Op_RegUInt16.A);
			}
			break;
			case InstructionSet::GetFromStackSub16:
			{
				InstructionBuilder::GetFromStackSub16(Ins, NewOffset, Ins.Op_RegUInt16.A);
			}
			break;
			case InstructionSet::GetFromStackSub8:
			{
				InstructionBuilder::GetFromStackSub8(Ins, NewOffset, Ins.Op_RegUInt16.A);
			}
			break;
			case InstructionSet::StoreRegOnStack8:
			{
				InstructionBuilder::StoreRegOnStack8(Ins, Ins.Op_RegUInt16.A, NewOffset);
			}
			break;
			case InstructionSet::StoreRegOnStack16:
			{
				InstructionBuilder::StoreRegOnStack16(Ins, Ins.Op_RegUInt16.A, NewOffset);
			}
			break;
			case InstructionSet::StoreRegOnStack32:
			{
				InstructionBuilder::StoreRegOnStack32(Ins, Ins.Op_RegUInt16.A, NewOffset);
			}
			break;
			case InstructionSet::StoreRegOnStack64:
			{
				InstructionBuilder::StoreRegOnStack64(Ins, Ins.Op_RegUInt16.A, NewOffset);
			}
			break;
			case InstructionSet::StoreRegOnStackSub8:
			{
				InstructionBuilder::StoreRegOnStackSub8(Ins, Ins.Op_RegUInt16.A, NewOffset);
			}
			break;
			case InstructionSet::StoreRegOnStackSub16:
			{
				InstructionBuilder::StoreRegOnStackSub16(Ins, Ins.Op_RegUInt16.A, NewOffset);
			}
			break;
			case InstructionSet::StoreRegOnStackSub32:
			{
				InstructionBuilder::StoreRegOnStackSub32(Ins, Ins.Op_RegUInt16.A, NewOffset);
			}
			break;
			case InstructionSet::StoreRegOnStackSub64:
			{
				InstructionBuilder::StoreRegOnStackSub64(Ins, Ins.Op_RegUInt16.A, NewOffset);
			}
			break;
			default:
				UCodeLangThrowException("bad ins");
				break;
			}
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
		UCodeLangUnreachable();
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
			UCodeLangUnreachable();
			break;
		}
		break;
	default:
		UCodeLangUnreachable();
		break;
	}
	_Registers.FreeRegister(Out);
	return Out;
}
void UCodeBackEndObject::BuildUIntToIntCast(const IRInstruction* Ins, const IROperator& Op, size_t IntSize)
{
	RegisterID V = GetRegisterForTep();
	RegisterID A = MakeIntoRegister(Ins, Op);
	//SetRegister(A,Ins);

	size_t ItemSize = GetSize(GetType(Op));


	while (ItemSize != IntSize)
	{
		if (ItemSize > IntSize)//cast down
		{
			switch (ItemSize)
			{
			case 2:InstructionBuilder::UInt16ToUInt8(_Ins, A, V); PushIns(); ItemSize = 1; break;
			case 4:InstructionBuilder::UInt32ToUInt16(_Ins, A, V); PushIns(); ItemSize = 2; break;
			case 8:InstructionBuilder::UInt64ToUInt32(_Ins, A, V); PushIns(); ItemSize = 4; break;
			default:break;
			}
			A = V;
		}
		else//cast up
		{
			switch (ItemSize)
			{
			case 1:InstructionBuilder::UInt8ToUInt16(_Ins, A, V); PushIns(); ItemSize = 2; break;
			case 2:InstructionBuilder::UInt16ToUInt32(_Ins, A, V); PushIns(); ItemSize = 4; break;
			case 4:InstructionBuilder::UInt32ToUInt64(_Ins, A, V); PushIns(); ItemSize = 8; break;
			default:break;
			}
			A = V;
		}


	}

	//FreeRegister(A);
	SetRegister(V,Ins);
}
RegisterID UCodeBackEndObject::GetRegisterOut(const IRInstruction* Item)
{
	auto r = RegisterID::OutPutRegister;
	return r;
}
void UCodeBackEndObject::BuildSIntToIntCast(const IRInstruction* Ins, const IROperator& Op, size_t IntSize)
{
	RegisterID V = GetRegisterForTep();
	RegisterID A = MakeIntoRegister(Ins, Op);
	//SetRegister(A,Ins);

	size_t ItemSize = GetSize(GetType(Op));


	while (ItemSize != IntSize)
	{
		if (ItemSize > IntSize)//cast down
		{
			switch (ItemSize)
			{
			case 2:InstructionBuilder::SInt16ToSInt8(_Ins, A, V); PushIns(); ItemSize = 1; break;
			case 4:InstructionBuilder::SInt32ToSInt16(_Ins, A, V); PushIns(); ItemSize = 2; break;
			case 8:InstructionBuilder::SInt64ToSInt32(_Ins, A, V); PushIns(); ItemSize = 4; break;
			default:break;
			}
			A = V;
		}
		else//cast up
		{
			switch (ItemSize)
			{
			case 1:InstructionBuilder::SInt8ToSInt16(_Ins, A, V); PushIns(); ItemSize = 2; break;
			case 2:InstructionBuilder::SInt16ToSInt32(_Ins, A, V); PushIns(); ItemSize = 4; break;
			case 4:InstructionBuilder::SInt32ToSInt64(_Ins, A, V); PushIns(); ItemSize = 8; break;
			default:break;
			}
			A = V;
		}


	}

	//FreeRegister(A);
	SetRegister(V, Ins);
}
void UCodeBackEndObject::StoreValueInPointer(RegisterID Pointer,size_t Pointerofset, const IRlocData& Value)
{
	size_t ObjectSize = GetSize(Value.ObjectType);
	size_t Offset = Pointerofset;
	
	
	RegisterID Reg = MakeIntoRegister(Value);
	
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
			RegToReg(Value.ObjectType._Type, V, RegisterToPut.value(),false);
			V = RegisterToPut.value();
		}
		return V;
	}
	else if (Value.Info.Is<IRlocData_StackPost>()
		|| Value.Info.Is<IRlocData_StackPre>())
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

		if (Size <= 1)
		{
			InstructionBuilder::GetFromStackSub8(_Ins, 0, Tep);
		}
		else if (Size <= 2)
		{
			InstructionBuilder::GetFromStackSub16(_Ins, 0, Tep);
		}
		else if (Size <= 4)
		{
			InstructionBuilder::GetFromStackSub32(_Ins, 0, Tep);
		}
		else if (Size <= 8)
		{
			InstructionBuilder::GetFromStackSub64(_Ins, 0, Tep);
		}
		else
		{
			UCodeLangUnreachable();
		}

		if (auto Val = Value.Info.Get_If<IRlocData_StackPost>()) 
		{
			_Stack.AddReUpdatePostFunc(PushIns(), Val->offset- _Stack.PushedOffset);
		}
		else 
		{
			auto Val2 = Value.Info.Get_If<IRlocData_StackPre>();

			
			auto mainobjsize = GetSize(Value.ObjectType);

			auto newpos = mainobjsize + Val2->offset;
			_Stack.AddReUpdatePreFunc(PushIns(), newpos- _Stack.PushedOffset);
		}
		return Tep;
	}
	else if (Value.Info.Is<IRlocData_StaticPos>()
		|| Value.Info.Is<IRlocData_ThreadPos>())
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
		
		if (auto Val = Value.Info.Get_If<IRlocData_StaticPos>()) 
		{
			InstructionBuilder::GetPointerOfStaticMem(_Ins, Tep, Val->offset);
		}
		else  if (auto Val = Value.Info.Get_If<IRlocData_ThreadPos>())
		{
			InstructionBuilder::GetPointerOfThreadMem(_Ins, Tep, Val->offset);
		}
		PushIns();
		
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
			UCodeLangUnreachable();
			break;
		}
		PushIns();

		return Tep;
	}
	UCodeLangUnreachable();
}
void  UCodeBackEndObject::GiveNameTo(const IRlocData& Value, const IRInstruction* Name)
{
	if (Value.Info.Is<RegisterID>())
	{
		SetRegister(Value.Info.Get<RegisterID>(), Name);
	}
	else if (auto Val = Value.Info.Get_If<IRlocData_StackPost>())
	{

		if (auto v = _Stack.Get(Val->offset).value_unchecked())
		{
			//v->IR = Name;
		}
		else 
		{
			_Stack.Add(Name, Val->offset);
		}
	}
	else if (auto Val = Value.Info.Get_If<IRlocData_StaticPos>())
	{

	}
	else if (auto Val = Value.Info.Get_If<IRlocData_ThreadPos>())
	{

	}
	else if (auto Val = Value.Info.Get_If<IRlocData_IRPar>())
	{

	}
	else if (auto Val = Value.Info.Get_If<IRlocData_StackPre>())
	{

	}
	else
	{
		UCodeLangUnreachable();
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
		if (T.Types.has_value()) 
		{
			auto& TV = T.Types.value();
			auto& staicdata = _Stack.Get(stack.offset).value();
			if (auto v = TV.Get_If<const IRInstruction*>())
			{
				staicdata->IR = *v;
			}
			else if (auto v = TV.Get_If<IROperator>())
			{
				staicdata->IR = *v;
			}

		}

		FreeRegister(*Val);
		FreeRegister(R);

		IRlocData Rl;
		Rl.Info = R;
		Rl.ObjectType = IRTypes::pointer;
		return Rl;
	}
	else if (Value.Info.Is<IRlocData_StackPost>()
		|| Value.Info.Is<IRlocData_StackPre>())
	{
		auto R = GetRegisterForTep();
		InstructionBuilder::GetPointerOfStackSub(_Ins, R,0);

		if (auto Val = Value.Info.Get_If<IRlocData_StackPost>()) {
			_Stack.AddReUpdatePostFunc(PushIns(), Val->offset);
		}
		else if (auto Val2 = Value.Info.Get_If<IRlocData_StackPre>())
		{
			auto mainobjsize = GetSize(Value.ObjectType);

			auto newpos = mainobjsize + Val2->offset;
			_Stack.AddReUpdatePreFunc(PushIns(), newpos);
		}
		else
		{
			UCodeLangUnreachable();
		}

		IRlocData V;
		V.Info = R;
		V.ObjectType = IRTypes::pointer;
		return V;
	}
	else if (auto Val = Value.Info.Get_If<IRlocData_StaticPos>())
	{
		auto R = GetRegisterForTep();
		InstructionBuilder::GetPointerOfStaticMem(_Ins, R, Val->offset); PushIns();

		IRlocData V;
		V.Info = R;
		V.ObjectType = IRTypes::pointer;
		return V;
	}
	else if (auto Val = Value.Info.Get_If<IRlocData_ThreadPos>())
	{
		auto R = GetRegisterForTep();
		InstructionBuilder::GetPointerOfThreadMem(_Ins, R, Val->offset); PushIns();

		IRlocData V;
		V.Info = R;
		V.ObjectType = IRTypes::pointer;
		return V;
	}
	else
	{
		UCodeLangUnreachable();
	}
	UCodeLangUnreachable();
}
RegisterID UCodeBackEndObject::LoadOp(const IRInstruction* Ins, const  IROperator& Op)
{
	if (Op.Type == IROperatorType::Value)
	{

		AnyInt64 Value = ToAnyInt(Ins->ObjectType, Op);
		auto CompilerRet = FindValueInRegister(Value);
		

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
			InstructionBuilder::Store32_V1(_Ins, V, Op.Value.AsInt32); PushIns();
			InstructionBuilder::Store32_V2(_Ins, V, Op.Value.AsInt32); PushIns();
			break;
		case IRTypes::f32:
			InstructionBuilder::Storef32_V1(_Ins, V, Op.Value.Asfloat32); PushIns();
			InstructionBuilder::Storef32_V2(_Ins, V, Op.Value.Asfloat32); PushIns();
			break;
		case IRTypes::f64:
			InstructionBuilder::Storef64_V1(_Ins, V, Op.Value.Asfloat64); PushIns();
			InstructionBuilder::Storef64_V2(_Ins, V, Op.Value.Asfloat64); PushIns();
			InstructionBuilder::Storef64_V3(_Ins, V, Op.Value.Asfloat64); PushIns();
			InstructionBuilder::Storef64_V4(_Ins, V, Op.Value.Asfloat64); PushIns();
			break;

		bit64label:
		case IRTypes::i64:
			InstructionBuilder::Store64_V1(_Ins, V, Op.Value.AsInt64); PushIns();
			InstructionBuilder::Store64_V2(_Ins, V, Op.Value.AsInt64); PushIns();
			InstructionBuilder::Store64_V3(_Ins, V, Op.Value.AsInt64); PushIns();
			InstructionBuilder::Store64_V4(_Ins, V, Op.Value.AsInt64); PushIns();
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
			UCodeLangUnreachable();
			break;
		}
		_Registers.SetRegister(V, Value);
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
				UCodeLangUnreachable();
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

		InstructionBuilder::LoadFuncPtr_V1(NullAddress, V, _Ins); PushIns();

		FuncInsID Tep;
		Tep.Index = _OutLayer->Get_Instructions().size() - 1;
		Tep._FuncID = Op.identifier;

		FuncsToLink.push_back(Tep);

		InstructionBuilder::LoadFuncPtr_V2(NullAddress, V, _Ins); PushIns();

		if (Get_Settings().PtrSize != IntSizes::Int32) 
		{
			InstructionBuilder::LoadFuncPtr_V3(NullAddress, V, _Ins); PushIns();
			InstructionBuilder::LoadFuncPtr_V4(NullAddress, V, _Ins); PushIns();
		}
		return V;
	}
	else if (Op.Type == IROperatorType::Get_PointerOf_IRidentifier)
	{
		auto Syb = _Input->GetSymbol(Op.identifier);
		if (Syb)
		{
			if (Syb->SymType == IRSymbolType::StaticVarable)
			{
				StaticMemoryManager::StaticMemInfo& Value = _StaticMemory._List.GetValue(Op.identifier);

				auto V = GetRegisterForTep();
				InstructionBuilder::GetPointerOfStaticMem(_Ins, V, Value.Offset); PushIns();

				return V;
			}
			else if (Syb->SymType == IRSymbolType::ThreadLocalVarable)
			{
				StaticMemoryManager::StaticMemInfo& Value = _ThreadMemory._List.GetValue(Op.identifier);

				auto V = GetRegisterForTep();
				InstructionBuilder::GetPointerOfThreadMem(_Ins, V, Value.Offset); PushIns();

				return V;
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

	UCodeLangUnreachable();
}

void UCodeBackEndObject::StoreValue(const IRInstruction* Ins, const  IROperator& OutputLocationIR, const  IROperator& Input)
{
	if (OutputLocationIR.Type == IROperatorType::IRInstruction)
	{
		auto& Item = OutputLocationIR.Pointer;
		const IRInstruction* VIns = Item;


		while (VIns->Type == IRInstructionType::Member_Access)
		{
			auto Tar = VIns->Target();
			if (Tar.Type == IROperatorType::IRInstruction)
			{
				VIns = Tar.Pointer;
			}
			else
			{
				break;
			}

		}

		if (VIns->Type == IRInstructionType::Member_Access_Dereference)
		{
			VIns = Item;
			size_t Offset = 0;
			while (VIns->Type == IRInstructionType::Member_Access)
			{
				auto V = _Input->GetOffset(
					_Input->GetSymbol(GetType(VIns->Target())._symbol)->Get_ExAs<IRStruct>()
					, VIns->Input().Value.AsUIntNative);

				Offset += V;
				auto Tar = VIns->Target();
				if (Tar.Type == IROperatorType::IRInstruction)
				{
					VIns = Tar.Pointer;
				}
				else
				{
					break;
				}

			}

			auto Type = VIns->ObjectType;

			const IRStruct* VStruct = _Input->GetSymbol(Type._symbol)->Get_ExAs<IRStruct>();

			auto Reg = LoadOp(VIns, VIns->Target());
			size_t FieldOffset = _Input->GetOffset(VStruct, Item->Input().Value.AsUIntNative);

			StoreValueInPointer(Reg, FieldOffset, GetIRLocData(Ins, Input));
			return;
		}
	}


	

	{
		IRInstruction V;
		V.ObjectType = GetType(OutputLocationIR);
		V.Type = IRInstructionType::Load;
		V.Target() = OutputLocationIR;

		auto Out = GetIRLocData(Ins, OutputLocationIR);
		//auto TepInfo = std::move(_Registers.GetInfo(Ins));

		auto Src = GetIRLocData(Ins, Input);
		//auto Tep2Info = std::move(_Registers.GetInfo(Ins));

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
	Pointerl:
	case IRTypes::pointer:
		switch (Get_Settings().PtrSize)
		{
		case IntSizes::Int32:goto Int32L;
		case IntSizes::Int64:goto Int64L;
		default:
			UCodeLangUnreachable();
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
		case IRSymbolType::FuncPtr:goto Pointerl;
		default:
			UCodeLangUnreachable();
			break;
		}
	}
	break;
	default:

		UCodeLangUnreachable();
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
void UCodeBackEndObject::RegToReg(IRTypes Type, RegisterID In, RegisterID Out, bool IsCopy)
{
	if (In != Out)
	{
		switch (Type)
		{
		case IRTypes::i8:
		{
			InstructionBuilder::StoreRegToReg8(_Ins, In, Out); PushIns();
		}break;
		case IRTypes::i16:
		{	
			InstructionBuilder::StoreRegToReg16(_Ins, In, Out); PushIns();
		}break;
		Int32L:
		case IRTypes::f32:
		case IRTypes::i32:
		{
			InstructionBuilder::StoreRegToReg32(_Ins, In, Out); PushIns();
		}	
		break;

		Int64L:
		case IRTypes::f64:
		case IRTypes::i64:
		{
			InstructionBuilder::StoreRegToReg64(_Ins, In, Out); PushIns();
		}	
		break;
			Pointer:
		case IRTypes::pointer:
			switch (Get_Settings().PtrSize)
			{
			case IntSizes::Int32:goto Int32L;
			case IntSizes::Int64:goto Int64L;
			default:
				UCodeLangUnreachable();
				break;
			}
			break;
		case IRTypes::IRsymbol:
		{
			goto Pointer;
		}//FuncPtrs
			break;
		default:
			UCodeLangUnreachable();
			break;
		}
	}
}


//MSVC is unable to use operator= on UCodeLang::Variant so this fixed it.
template<typename T> 
T copy(const T& V)
{
	return V;
}

void UCodeBackEndObject::PushOpStack(const IRInstruction* Ins, const  IROperator& Op)
{
	auto Info =GetIRLocData(Ins,Op);

	auto Size = GetSize(Info.ObjectType);
	size_t Offset = 0;
	RegisterID Tep = GetRegisterForTep();

	while (Size != 0)
	{

		if (Size >= 8)
		{
			{
				IRlocData V;
				V.Info = copy(Info.Info);
				V.ObjectType = IRTypes::i64;
				MoveValueInReg(V, Offset, Tep);
			}

			{
				InstructionBuilder::Push64(_Ins, Tep); PushIns();
				_Stack.PushedOffset += 8;
			}

			Offset += 8;
			Size -= 8;
		}
		else if (Size >= 4)
		{
			{
				IRlocData V;
				V.Info = copy(Info.Info);
				V.ObjectType = IRTypes::i32;
				MoveValueInReg(V, Offset, Tep);
			}

			{
				InstructionBuilder::Push32(_Ins, Tep); PushIns();
				_Stack.PushedOffset += 4;
			}

			Offset += 4;
			Size -= 4;
		}
		else if (Size >= 2)
		{
			{
				IRlocData V;
				V.Info = copy(Info.Info);
				V.ObjectType = IRTypes::i16;
				MoveValueInReg(V, Offset, Tep);
			}

			{
				InstructionBuilder::Push16(_Ins, Tep); PushIns();
				_Stack.PushedOffset += 2;
			}

			Offset += 2;
			Size -= 2;
		}
		else if (Size >= 1)
		{
			{
				IRlocData V;
				V.Info = copy(Info.Info);
				V.ObjectType = IRTypes::i8;
				MoveValueInReg(V, Offset, Tep);
			}

			{
				InstructionBuilder::Push8(_Ins, Tep); PushIns();
				_Stack.PushedOffset += 1;
			}

			Offset += 1;
			Size -= 1;
		}
	}
	
}
UCodeBackEndObject::IRlocData UCodeBackEndObject::GetIRLocData(const IRInstruction* Ins,bool GetAddress)
{
	auto RegInfo = FindIRInRegister(Ins,GetAddress);
	if (RegInfo.has_value())
	{
		UCodeBackEndObject::IRlocData R;
		R.Info = RegInfo.value();
		R.ObjectType = GetType(Ins);
		return R;
	}
	else if (auto stack = _Stack.Has(Ins).value_unchecked())
	{
		UCodeBackEndObject::IRlocData R;
		R.Info = IRlocData_StackPost(stack->Offset);
		R.ObjectType = GetType(Ins);

		if (GetAddress)
		{
			return GetPointerOf(R);
		}

		return R;
	}
	else
	{
		if (Ins->Type == IRInstructionType::LoadNone)
		{

			auto VPtr = _Stack.Has(Ins);
			if (auto V = VPtr.value_unchecked())
			{
				UCodeBackEndObject::IRlocData R;
				R.Info = IRlocData_StackPost(V->Offset);
				R.ObjectType = GetType(Ins);
				if (GetAddress)
				{
					auto old = std::move(R);
					R = GetPointerOf(old);
				}
				return R;
			}
			else
			{
				if (!IsPrimitive(GetType(Ins)))
				{
					UCodeBackEndObject::IRlocData R;
					R.Info = IRlocData_StackPost(_Stack.AddWithSize(Ins, GetSize(Ins))->Offset);
					R.ObjectType = GetType(Ins);


					if (GetAddress)
					{
						auto old = std::move(R);
						R = GetPointerOf(old);
					}
					return R;
				}
				else
				{
					UCodeBackEndObject::IRlocData R;
					if (GetAddress)
					{
						auto old = GetIRLocData(Ins);
						R = GetPointerOf(old);
					}
					else
					{
						auto FreeTep = GetRegisterForTep();
						RegWillBeUsed(FreeTep);
						R.Info = FreeTep;
						R.ObjectType = GetType(Ins);
					} 
					GiveNameTo(R, Ins);
					

					return R;
				}
			}
		}
		else if (Ins->Type == IRInstructionType::Load)
		{
			IRlocData R;
			if (Ins->Target().Type == IROperatorType::Value)
			{
				auto reg = LoadOp(Ins, Ins->Target());
				R.Info = reg;
				R.ObjectType = GetType(Ins, Ins->Target());
			}
			else 
			{
				R = GetIRLocData(Ins->Target());
			}
			
			if (GetAddress)
			{
				auto old = std::move(R);
				R = GetPointerOf(old);
			}
			return R;
		}
		else if (Ins->Type == IRInstructionType::Member_Access_Dereference)
		{
			if (GetAddress) 
			{
				IRlocData CompilerRet;
				CompilerRet.ObjectType = GetType(Ins->Target());
				const IRStruct* VStruct = _Input->GetSymbol(CompilerRet.ObjectType._symbol)->Get_ExAs<IRStruct>();

				const size_t Index = Ins->Input().Value.AsUIntNative;
				auto& Field = VStruct->Fields[Index];

				const size_t FieldOffset = Field.Offset.value();

				CompilerRet.ObjectType._Type = IRTypes::pointer;
				if (FieldOffset == 0)
				{
					auto oldtype = CompilerRet.ObjectType;
					
					auto objptr = GetIRLocData(Ins->Target());
					objptr.ObjectType = oldtype;
					return objptr;
				}
				else
				{
					if (FieldOffset < 256)
					{
						auto pointer = LoadOp(Ins,Ins->Target());
						RegisterID out = GetRegisterForTep();
						InstructionBuilder::LoadEffectiveAddressA(_Ins, pointer, FieldOffset, out); PushIns();


						CompilerRet.Info = out;
						return CompilerRet;
					}
					else 
					{
						RegisterID out = GetRegisterForTep();
						auto pointer = LoadOp(Ins, Ins->Target());
						auto offsetsetleft = FieldOffset;

						RegToReg(IRTypes::pointer, pointer, out,false);

						while (offsetsetleft != 0)
						{
							auto off = std::min<size_t>(UINT8_MAX, offsetsetleft);;

							InstructionBuilder::LoadEffectiveAddressA(_Ins, out, off, out); PushIns();

							offsetsetleft -= off;
						}

						CompilerRet.Info = out;
						return CompilerRet;
					}
				}
			}
			else
			{
				IRlocData CompilerRet;
				CompilerRet.ObjectType = Ins->ObjectType;
				const IRStruct* VStruct = _Input->GetSymbol(CompilerRet.ObjectType._symbol)->Get_ExAs<IRStruct>();

				size_t Index = Ins->Input().Value.AsUIntNative;
				auto& Field = VStruct->Fields[Index];

				size_t FieldOffset = Field.Offset.value();

				auto RegOut = GetRegisterForTep();
				CompilerRet.Info = RegOut;
				CompilerRet.ObjectType = Field.Type;

				ReadValueFromPointer(LoadOp(Ins,Ins->Target()), FieldOffset, CompilerRet);

				return CompilerRet;
			}
		}
		else if (Ins->Type == IRInstructionType::Member_Access)
		{
			auto Pos = GetIRLocData(Ins->Target(),GetAddress);

			if (GetAddress && Pos.ObjectType._Type ==IRTypes::pointer)
			{
				auto& lastIns = _OutLayer->_Instructions.back();
				UCodeLangAssert(lastIns.OpCode == InstructionSet::LoadEffectiveAddressA);
				
				const IRStruct* VStruct = _Input->GetSymbol(GetType(Ins->Target())._symbol)->Get_ExAs<IRStruct>();
				size_t FieldIndex = Ins->Input().Value.AsUIntNative;
				const size_t Offset = _Input->GetOffset(VStruct, FieldIndex);


				auto offsetleft = Offset;
				auto PtrReg = lastIns.Op_TwoRegInt8.A;
				auto OutReg = lastIns.Op_TwoRegInt8.B;
				
				while (offsetleft != 0)
				{
					

					auto& lastIns = _OutLayer->_Instructions.back();
					auto& offsetnumber = lastIns.Op_TwoRegInt8.C;

					auto maxpossableoffsetforins = UINT8_MAX - offsetnumber;

					auto offsettosub = offsetleft;
					if (offsettosub > maxpossableoffsetforins)
					{
						offsettosub = maxpossableoffsetforins;
					}


					if (offsettosub != 0)
					{
						offsetleft -= offsettosub;
						offsetnumber += offsettosub;
					}
					else 
					{
						auto newoffset = std::min<size_t>(UINT8_MAX, offsetleft);
						InstructionBuilder::LoadEffectiveAddressA(_Ins, OutReg, newoffset, OutReg); PushIns();
						offsetleft -= newoffset;
					}
				}
				return Pos;
			}
			else 
			{
				const IRStruct* VStruct = _Input->GetSymbol(Pos.ObjectType._symbol)->Get_ExAs<IRStruct>();
				size_t FieldIndex = Ins->Input().Value.AsUIntNative;

				size_t Offset = _Input->GetOffset(VStruct, FieldIndex);
				AddOffset(Pos, Offset);

				Pos.ObjectType = VStruct->Fields[FieldIndex].Type;

				return Pos;
			}
		}
		else
		{
			UCodeLangUnreachable();
		}
	}
	UCodeLangUnreachable();
}
UCodeBackEndObject::IRlocData UCodeBackEndObject::GetIRLocData(const IRInstruction* Ins, const IROperator& Op,bool GetAddress)
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


			auto StackPtr = _Stack.Has(Item);
			if (auto Stack = StackPtr.value_unchecked())
			{
				CompilerRet.Info = IRlocData_StackPost(Stack->Offset);
			}
			else 
			{
				auto InReg = FindIRInRegister(Item);
				if (InReg.has_value())
				{
					CompilerRet.Info = InReg.value();
				}
				else if (Item->Type == IRInstructionType::Member_Access)
				{
					auto Pos = GetIRLocData(Item->Target());
					const IRStruct* VStruct = _Input->GetSymbol(Pos.ObjectType._symbol)->Get_ExAs<IRStruct>();
					size_t FieldIndex = Item->Input().Value.AsUIntNative;

					size_t Offset = _Input->GetOffset(VStruct, FieldIndex);

					AddOffset(Pos, Offset);

					Pos.ObjectType = VStruct->Fields[FieldIndex].Type;

					CompilerRet = Pos;
				}
				else if (Item->Type == IRInstructionType::Member_Access_Dereference)
				{
					return GetIRLocData(Item, GetAddress);
				}
				else if (Item->Type == IRInstructionType::Load)
				{
					return GetIRLocData(Item, Item->Target(), GetAddress);
				}
				else if (IsLocation(Item->Type))
				{
					return GetIRLocData(Item, GetAddress);
				}
				else
				{
					UCodeLangUnreachable();
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
				UCodeLangUnreachable();
			}
		}
		else if (Op.Type == IROperatorType::Get_PointerOf_IRInstruction)
		{
 			CompilerRet = GetIRLocData(Op.Pointer,true);
		}
		else if (Op.Type == IROperatorType::Get_PointerOf_IRParameter)
		{
			CompilerRet = GetPointerOf(To(*GetParData(Op.Parameter)));
		}
		else if (Op.Type == IROperatorType::Get_PointerOf_IRidentifier)
		{
			if (auto Syb = _Input->GetSymbol(Op.identifier))
			{
				if (Syb->SymType == IRSymbolType::StaticVarable)
				{
					const auto& Mem = _StaticMemory._List.GetValue(Op.identifier);
					CompilerRet.Info = IRlocData_StaticPos(Mem.Offset);
					CompilerRet.ObjectType = Syb->Type;
				}
				else if (Syb->SymType == IRSymbolType::ThreadLocalVarable)
				{
					const auto& Mem = _ThreadMemory._List.GetValue(Op.identifier);
					CompilerRet.Info = IRlocData_ThreadPos(Mem.Offset);
					CompilerRet.ObjectType = Syb->Type;
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
			CompilerRet = GetPointerOf(CompilerRet);
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
			if (auto Syb = _Input->GetSymbol(Op.identifier))
			{
				if (Syb->SymType == IRSymbolType::StaticVarable)
				{
					const auto& Mem = _StaticMemory._List.GetValue(Op.identifier);
					CompilerRet.Info = IRlocData_StaticPos(Mem.Offset);
					CompilerRet.ObjectType = Syb->Type;
				}
				else if (Syb->SymType == IRSymbolType::ThreadLocalVarable)
				{
					const auto& Mem = _ThreadMemory._List.GetValue(Op.identifier);
					CompilerRet.Info = IRlocData_ThreadPos(Mem.Offset);
					CompilerRet.ObjectType = Syb->Type;
				}
				else
				{
					UCodeLangUnreachable();
				}
			}
		}
		else if (Op.Type == IROperatorType::Get_Func_Pointer)
		{
			CompilerRet.Info = LoadOp(Ins, Op);
			CompilerRet.ObjectType = GetType(Ins);
		}
		else
		{
			UCodeLangUnreachable();
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
				auto ValPtr = _Stack.Has(Item);

				if (auto Val = ValPtr.value_unchecked())
				{
					CompilerRet.Info = IRlocData_StackPost(Val->Offset);
				}
				else
				{
					if (Item->Type == IRInstructionType::Load)
					{
						return GetIRLocData(Item,Item->Target());
					}
					else if (Item->Type == IRInstructionType::LoadNone)
					{
						auto t = GetIRLocData(Item);
						GiveNameTo(t,Item);
						return t;
					}
					else if (Item->Type == IRInstructionType::Member_Access)
					{
						auto Pos = GetIRLocData(Item->Target());
						const IRStruct* VStruct = _Input->GetSymbol(Pos.ObjectType._symbol)->Get_ExAs<IRStruct>();
						size_t FieldIndex = Item->Input().Value.AsUIntNative;

						size_t Offset = _Input->GetOffset(VStruct, FieldIndex);
						AddOffset(Pos, Offset);

						Pos.ObjectType = VStruct->Fields[FieldIndex].Type;

						return Pos;
					}
					else
					{
						UCodeLangUnreachable();
					}
				}
			}

			return CompilerRet;
		}
		else if (Op.Type == IROperatorType::IRidentifier)
		{
			if (auto Syb = _Input->GetSymbol(Op.identifier))
			{
				if (Syb->SymType == IRSymbolType::StaticVarable)
				{
					const auto& Mem = _StaticMemory._List.GetValue(Op.identifier);
					CompilerRet.Info = IRlocData_StaticPos(Mem.Offset);
				}
				else if (Syb->SymType == IRSymbolType::ThreadLocalVarable)
				{
					const auto& Mem =_ThreadMemory._List.GetValue(Op.identifier);
					CompilerRet.Info = IRlocData_ThreadPos(Mem.Offset);
				}
				else
				{
					UCodeLangUnreachable();
				}
			}
			return CompilerRet;
		}
		else if (Op.Type == IROperatorType::DereferenceOf_IRInstruction)
		{
			//
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
		else if (Op.Type == IROperatorType::IRParameter)
		{
			const auto Ins = Op.Parameter;
			return To(*GetParData(Ins));
		}
		else if (Op.Type == IROperatorType::Get_PointerOf_IRInstruction)
		{
			return GetIRLocData(Op.Pointer, true);
		}
		else if (Op.Type == IROperatorType::Get_PointerOf_IRParameter)
		{
			return GetPointerOf(To(*GetParData(Op.Parameter)));
		}
		else
		{
			UCodeLangUnreachable();
		}
	}
	UCodeLangUnreachable();
}
UCodeBackEndObject::IRlocData UCodeBackEndObject::GetIRLocData(const IROperator& Op, bool GetAddress)
{
	if (Op.Type == IROperatorType::IRParameter)
	{
		const auto Ins = Op.Parameter;
		auto r = To(*GetParData(Ins));
		if (GetAddress)
		{
			return GetPointerOf(r);
		}
		return r;
	}
	else if (Op.Type == IROperatorType::IRInstruction)
	{
		const auto Ins = Op.Pointer;

		IRlocData CompilerRet;
		auto InReg = FindIRInRegister(Ins);

		CompilerRet.ObjectType = GetType(Ins);
		if (InReg.has_value())
		{

			CompilerRet.Info = InReg.value();

			if (GetAddress)
			{
				CompilerRet = GetPointerOf(CompilerRet);
			}
		}
		else
		{
			
			if (auto Val = _Stack.Has(Ins).value_unchecked())
			{
				CompilerRet.Info = IRlocData_StackPost(Val->Offset);
				
				if (GetAddress)
				{
					CompilerRet = GetPointerOf(CompilerRet);
				}
			}
			else
			{
				CompilerRet = GetIRLocData(Ins,GetAddress);
			}

		
		}
		return CompilerRet;
	}
	else if (Op.Type == IROperatorType::IRidentifier)
	{
		auto Syb = _Input->GetSymbol(Op.identifier);
		if (Syb)
		{
			if (Syb->SymType == IRSymbolType::StaticVarable)
			{
				StaticMemoryManager::StaticMemInfo& Value = _StaticMemory._List.GetValue(Op.identifier);

				IRlocData CompilerRet;
				CompilerRet.ObjectType = Syb->Type;
				CompilerRet.Info = IRlocData_StaticPos(Value.Offset);

				if (GetAddress)
				{
					CompilerRet = GetPointerOf(CompilerRet);
				}

				return CompilerRet;
			}
			else if (Syb->SymType == IRSymbolType::ThreadLocalVarable)
			{
				StaticMemoryManager::StaticMemInfo& Value = _ThreadMemory._List.GetValue(Op.identifier);

				IRlocData CompilerRet;
				CompilerRet.ObjectType = Syb->Type;
				CompilerRet.Info = IRlocData_ThreadPos(Value.Offset);


				if (GetAddress)
				{
					CompilerRet = GetPointerOf(CompilerRet);
				}

				return CompilerRet;
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
	else
	{
		UCodeLangUnreachable();
	}
}
void UCodeBackEndObject::MoveRegInValue(RegisterID Value, const IRlocData& To, size_t Offset)
{
	auto Size = GetSize(To.ObjectType);
	if (auto Val = To.Info.Get_If<RegisterID>())
	{
		if (Offset == 0) {
			RegToReg(To.ObjectType._Type, Value, *Val,false);
		}
		else
		{
			UCodeLangUnreachable();
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
			UCodeLangUnreachable();
			break;
		}
		_Stack.AddReUpdatePostFunc(PushIns(),Val->offset + Offset - _Stack.PushedOffset);
	}
	else if (auto Val = To.Info.Get_If<IRlocData_StackPre>())
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
			InstructionBuilder::StoreRegOnStackSub64(_Ins, Value, 0);
			break;
		default:
			UCodeLangUnreachable();
			break;
		}

		auto mainobjsize = GetSize(To.ObjectType);

		auto newpos = mainobjsize + Val->offset;
		_Stack.AddReUpdatePreFunc(PushIns(), newpos + Offset - _Stack.PushedOffset);
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
			UCodeLangUnreachable();
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
			UCodeLangUnreachable();
			break;
		}
	}
	else
	{
		UCodeLangUnreachable();
	}
}
void UCodeBackEndObject::MoveValueInReg(const IRlocData& Value, size_t Offset, RegisterID To)
{
	auto Size = GetSize(Value.ObjectType);
	if (auto Val = Value.Info.Get_If<RegisterID>())
	{
		if (Offset == 0) {
			RegToReg(Value.ObjectType._Type, *Val, To,true);
		}
		else
		{
			UCodeLangUnreachable();
		}
	}
	else if (Value.Info.Is<IRlocData_StackPost>()
		|| Value.Info.Is<IRlocData_StackPre>())
	{

		switch (Size)
		{
		case 1:
			InstructionBuilder::GetFromStackSub8(_Ins, 0, To);
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
			UCodeLangUnreachable();
			break;
		}
		if (auto Val = Value.Info.Get_If<IRlocData_StackPost>()) {
			_Stack.AddReUpdatePostFunc(PushIns(), Val->offset + Offset- _Stack.PushedOffset);
		}
		else
		{
			auto Val2 = Value.Info.Get_If<IRlocData_StackPre>();

			auto mainobjsize = GetSize(Value.ObjectType);

			auto newpos = mainobjsize + Val2->offset;
			_Stack.AddReUpdatePreFunc(PushIns(), newpos+Offset- _Stack.PushedOffset);
		}
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
			UCodeLangUnreachable();
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
			UCodeLangUnreachable();
			break;
		}
	}
	else
	{
		UCodeLangUnreachable();
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
				UCodeLangUnreachable();
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
				UCodeLangUnreachable();
				break;
			}

		}
	}
	else
	{
		UCodeLangUnreachable();
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

	_Registers.FreeRegister(Tep);
}

void UCodeBackEndObject::LogicalNot(IRTypes Type, RegisterID In, RegisterID Out)
{

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
RegisterID UCodeBackEndObject::GetRegisterForTep()
{
	auto Item = _Registers.GetFreeRegister();
	if (!Item.has_value())
	{
		for (RegisterID_t i = (RegisterID_t)RegisterID::StartRegister; i < (RegisterID_t)RegisterID::EndRegister; i++)
		{
			RegisterID Register = (RegisterID)i;
			auto& Item = _Registers.Registers[i];
			if (Item.Types.has_value())
			{
				auto& ItemValue = Item.Types.value();

				if (auto IR = ItemValue.Get_If<const IRInstruction*>())
				{
					auto& IRV = *IR;
					if (!IsReferencedAfterThisIndex(IRV))
					{
						return Register;
					}
				}
				else if (auto IR = ItemValue.Get_If<AnyInt64>())
				{
					return Register;
				}
			}
			else
			{
				return Register;
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
		WeightType LestToBeat = INT32_MAX;

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
		UCodeLangUnreachable();
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
void UCodeBackEndObject::ClearVarableLocs()
{
	_OldVarableLocRegisters.Reset();
}

enum class TypeRegVal
{
	IRIns,
	IROp,
	IRAndOp,
	Val
};
TypeRegVal Get(Variant<AnyInt64,const IRInstruction*, IROperator,IRAndOperator>& Item)
{
	if (Item.Is< const IRInstruction*>())
	{
		return TypeRegVal::IRIns;
	}
	else if (Item.Is<IROperator>())
	{
		return TypeRegVal::IROp;
	}
	else if (Item.Is<IRAndOperator>())
	{
		return TypeRegVal::IRAndOp;
	}
	else
	{
		return TypeRegVal::Val;
	}
}


void UCodeBackEndObject::UpdateVarableLocs()
{
	if (IsDebugMode())
	{

		for (size_t i = 0; i < _Registers.Registers.size(); i++)
		{
			auto& OldItemInfo = _OldVarableLocRegisters.Registers[i];
			auto& Item2 = _Registers.Registers[i];
			RegisterID RegValue = (RegisterID)i;
			
			bool CanSet = Item2.Types.has_value() != OldItemInfo.Types.has_value();
			if (CanSet == false)
			{
				if (Item2.Types.has_value() && OldItemInfo.Types.has_value())
				{
					enum class TypeEVal
					{
						IRIns,
						IROp,
					};

					TypeRegVal One =Get(Item2.Types.value());
					TypeRegVal Two = Get(OldItemInfo.Types.value());

					if (One != Two)
					{
						CanSet = true;
					}
					else
					{
						if (auto val = Item2.Types.value().Get_If<const IRInstruction*>())
						{
							CanSet = *val != OldItemInfo.Types.value().Get<const IRInstruction*>();
						}
						else if (auto val = Item2.Types.value().Get_If<IROperator>())
						{
							CanSet = *val != OldItemInfo.Types.value().Get<IROperator>();
						}

					}
				}
			}

			if (CanSet) 
			{
				if (Item2.Types.has_value())
				{
					auto& TypesV = Item2.Types.value();
					
					
					if (auto Ir = TypesV.Get_If<const IRInstruction*>())
					{
						auto& IrItem = *Ir;
						auto DebugIns = LookingBlock->Get_debugfor(IrItem);


						for (auto& Item : DebugIns)
						{
							if (auto Irsetvar = Item->Debug.Get_If<IRDebugSetVarableName>())
							{
								UDebugSetVarableLoc Loc;
								Loc.ForIns = _OutLayer->Get_Instructions().size();
								Loc.VarableFullName = Irsetvar->VarableName;
								Loc.Type = RegValue;

								_DebugInfo.Add_SetVarableLoc(std::move(Loc));
								break;
							}
						}
					}
					else if (auto Ir = TypesV.Get_If<IROperator>())
					{

						if (Ir->Type == IROperatorType::IRParameter)
						{
							/*
							IRPar* p = Ir->Parameter;

							UDebugSetVarableLoc Loc;
							Loc.ForIns = _OutLayer->Get_Instructions().size();
							Loc.VarableFullName = _Input->FromID(this->lookingatfunc->identifier) + ":" + _Input->FromID(p->identifier);
							Loc.Type = RegValue;

							_DebugInfo.Add_SetVarableLoc(std::move(Loc));
							*/
						}
						else
						{
							//UCodeLangUnreachable();
						}
					}
					else
					{
						//UCodeLangUnreachable();
					}
				}
				OldItemInfo.Types = Item2.Types;
			}
		}
	}
}
void UCodeBackEndObject::MoveValuesToState(const RegistersManager& state)
{
	
	RegisterID_t reg = (RegisterID_t)-1;
	for (auto& Item : state.Registers)
	{
		reg++;

		if (Item.Types.has_value())
		{
			auto& type = Item.Types.value();

			if (auto val = type.Get_If<const IRInstruction*>())
			{
				if ((*val)->Type == IRInstructionType::Load)
				{
					if ((*val)->A.Type == IROperatorType::IRInstruction)
					{
						if (auto stackitem = _Stack.Has((*val)->A.Pointer).value_unchecked())
						{
							IRlocData V;
							V.Info = IRlocData_StackPost(stackitem->Offset);
							V.ObjectType = GetType(stackitem->IR.Get<const IRInstruction*>());

							IRlocData R;
							R.Info = (RegisterID)reg;
							R.ObjectType = V.ObjectType;

							CopyValues(V,R);
						}
					}
				}
			}
		}
	}
	
	_Registers = state;
}
 
void UCodeBackEndObject::SynchronizePars()
{
	for (auto& Item : CurrentFuncParPos)
	{
		if (Item.Location.Is<RegisterID>())
		{
			this->SynchronizePar(GetParData(Item.Par));
		}
	}
}

RegistersManager UCodeBackEndObject::SaveState()
{

	auto old = _Registers;
	RegisterID_t reg = (RegisterID_t)-1;
	for (auto& Item : _Registers.Registers)
	{
		reg++;
		if (Item.Types.has_value())
		{
			auto& ItemD = Item.Types.value();
			if (auto val = ItemD.Get_If<const IRInstruction*>())
			{
				const IRInstruction* ins = *val;

				if (IsReferencedAfterThisIndex(ins))
				{
					auto type = GetType(ins);
					MoveValueToStack(ins, type, (RegisterID)reg);
				}
				
			}
		}
	}

 	//_Registers.Reset();
	return old; 
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
	if (Test.Type == IROperatorType::Value)
	{
		return false;
	}

	if (Test.Type == Other.Type)
	{
		bool r =false;
		switch (Test.Type)
		{
		case IROperatorType::IRidentifier:
		case IROperatorType::Get_PointerOf_IRidentifier:
			r = Test.identifier == Other.identifier;
			break;
		case IROperatorType::DereferenceOf_IRInstruction:
		case IROperatorType::Get_PointerOf_IRInstruction:
		case IROperatorType::IRInstruction:
			r = Test.Pointer == Other.Pointer;
			break;
		case IROperatorType::DereferenceOf_IRParameter:
		case IROperatorType::IRParameter:
		case IROperatorType::Get_PointerOf_IRParameter:
			r = Test.Parameter == Other.Parameter;
			break;
		default:
			UCodeLangUnreachable();
			break;
		}

		if (r)
		{
			return true;
		}
		else
		{
			if (Other.Type == IROperatorType::IRInstruction)
			{
				auto pointer = Other.Pointer;

				if (pointer->Type == IRInstructionType::Member_Access_Dereference
					|| pointer->Type == IRInstructionType::Member_Access)
				{

					if (IsReferenceingTheSame(Test, pointer->Target()))
					{
						return true;
					}

				}

			}
		}
	}

	return false;
}
bool UCodeBackEndObject::IsReferencedAfterThisIndex(const IROperator& Op)
{
	//size_t i = Index+1 does not work because binaryOp relies on this to not trash first item register
	for (size_t i = Index; i < LookingBlock->Instructions.size(); i++)
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
					//Par->Location = (RegisterID)i;
					//break;
				}
			}
			else if (auto Op = Info.Types.value().Get_If<IROperator>())
			{
				IROperator V = *Op;

				if (V == IROperator((IRPar*)Par->Par))
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

		if (auto ptrOp = Item.IR.Get_If<const IRInstruction*>())
		{
			auto& ptr = *ptrOp;
			if (ptr == nullptr) { continue; }
			if (IsLookingAtPar(ptr, Par->Par))
			{
				Par->Location = StackPostCall(Item.Offset);
			}
		}
		else if (auto ptrOp = Item.IR.Get_If<IROperator>())
		{
			auto& ptr = *ptrOp;
			if (ptr.Type == IROperatorType::IRParameter && ptr.Parameter == Par->Par)
			{
				Par->Location = StackPostCall(Item.Offset);
			}
		}
	}
	
	
	if (auto val = Par->Location.Get_If<RegisterID>())
	{
		_Registers.GetInfo(*val).Types = IROperator((IRPar*)Par->Par);
	}
}
UCodeBackEndObject::FindParsLoc UCodeBackEndObject::GetParsLoc(const Vector<IRType>& Pars, bool SetReg)
{
	Vector<IRPar> Tep;
	Tep.resize(Pars.size());
	for (size_t i = 0; i < Pars.size(); i++)
	{
		auto& Item = Pars[i];
		auto& TepItem = Tep[i];
		TepItem.type = Item;
	}
	return GetParsLoc(Tep,SetReg);
}
UCodeBackEndObject::FindParsLoc UCodeBackEndObject::GetParsLoc(const Vector<IRPar>& Pars,bool SetReg)
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

		auto ParSize = GetSize(Item.type);

		if (IsUseingStack == false && ParSize <= sizeof(AnyInt64))
		{
			Loc.Location = V;

			if (SetReg) {
				SetRegister(V, &Item);
			}

			V2++;
			if (V == RegisterID::EndParameterRegister)
			{
				IsUseingStack = true;
			}
		}
		else
		{
			Loc.Location = StackPreCall(StackOffset);
			StackOffset += GetSize(Item.type);
			CompilerRet.OverflowedPars.push_back(i);
		}
		CompilerRet.ParsPos.push_back(Loc);
	}



	for (size_t i = 0; i < CompilerRet.OverflowedPars.size() / 2; i++)// flip offsets
	{
		auto& Item0 = CompilerRet.ParsPos[CompilerRet.OverflowedPars[i]];
		auto& Item1 = CompilerRet.ParsPos[CompilerRet.OverflowedPars[CompilerRet.OverflowedPars.size() - 1 - i]];

		auto& Item0StackOffSet = Item0.Location.Get<StackPreCall>().Offset;
		auto& Item1StackOffSet = Item1.Location.Get<StackPreCall>().Offset;


		std::swap(Item0StackOffSet, Item1StackOffSet);
	}

	return CompilerRet;
}
void UCodeBackEndObject::BuildLink(const IRidentifier& FuncName, IRFuncLink LinkType)
{
	auto VFuncName = NameDecoratior::GetUnDecoratedName(FuncName);
	if(VFuncName == "__LogChar")
	{
		InstructionBuilder::LogChar(_Ins, RegisterID::StartParameterRegister); PushIns();
	}
	else if (VFuncName == "__LogBuffer")
	{
		InstructionBuilder::LogBuffer(_Ins, RegisterID::Parameter1_Register, RegisterID::Parameter2_Register); PushIns();
	}
	else if (VFuncName == "__ReadChar")
	{
		InstructionBuilder::ReadChar(_Ins,RegisterID::OutPutRegister); PushIns();
	}
	else if (VFuncName == "__ReadBuffer")
	{
		InstructionBuilder::ReadBuffer(_Ins, RegisterID::Parameter1_Register,RegisterID::Parameter2_Register); PushIns();
	}
	else if (VFuncName == "__Malloc")
	{
		InstructionBuilder::Malloc(_Ins, RegisterID::StartParameterRegister, RegisterID::OutPutRegister); PushIns();
	}
	else if (VFuncName == "__Calloc")
	{
		InstructionBuilder::Calloc(_Ins, RegisterID::StartParameterRegister, RegisterID::OutPutRegister); PushIns();
	}
	else if (VFuncName == "__Realloc")
	{
		InstructionBuilder::Realloc(_Ins, RegisterID::Parameter1_Register, RegisterID::Parameter2_Register, RegisterID::OutPutRegister); PushIns();
	}
	else if (VFuncName == "__Strlen")
	{
		InstructionBuilder::Strlen(_Ins, RegisterID::Parameter1_Register,RegisterID::OutPutRegister); PushIns();
	}
	else if (VFuncName == "__Memset")
	{
		InstructionBuilder::Memset(_Ins, RegisterID::Parameter1_Register, RegisterID::Parameter2_Register, RegisterID::Parameter3_Register); PushIns();
	}
	else if (VFuncName == "__Memcmp")
	{
		InstructionBuilder::Memcmp(_Ins, RegisterID::Parameter1_Register, RegisterID::Parameter2_Register, RegisterID::Parameter3_Register); PushIns();
	}
	else if (VFuncName == "__Free")
	{
		InstructionBuilder::Free(_Ins, RegisterID::StartParameterRegister); PushIns();
	}
	else if (VFuncName == "__FileOpen")
	{
		InstructionBuilder::OpenFile(_Ins, RegisterID::Parameter1_Register, RegisterID::Parameter2_Register, RegisterID::OutPutRegister); PushIns();
	}
	else if (VFuncName == "__FilePOpen")
	{
		InstructionBuilder::OpenPCharFile(_Ins, RegisterID::Parameter1_Register, RegisterID::Parameter2_Register, RegisterID::OutPutRegister); PushIns();
	}
	else if (VFuncName == "__FileRead")
	{
		InstructionBuilder::File_Read(_Ins, RegisterID::Parameter1_Register, RegisterID::Parameter2_Register, RegisterID::Parameter3_Register); PushIns();
	}
	else if (VFuncName == "__FileWrite")
	{
		InstructionBuilder::File_Read(_Ins, RegisterID::Parameter1_Register, RegisterID::Parameter2_Register, RegisterID::Parameter3_Register); PushIns();
	}
	else if (VFuncName == "__FileIsOpen")
	{
		InstructionBuilder::FileIsOpen(_Ins, RegisterID::Parameter1_Register,RegisterID::OutPutRegister); PushIns();
	}
	else if (VFuncName == "__FileSetPos")
	{
		InstructionBuilder::File_SetPos(_Ins, RegisterID::Parameter1_Register, RegisterID::Parameter2_Register, RegisterID::Parameter3_Register); PushIns();
	}
	else if (VFuncName == "__FileGetPos")
	{
		InstructionBuilder::File_GetPos(_Ins, RegisterID::Parameter1_Register, RegisterID::OutPutRegister); PushIns();
	}
	else if (VFuncName == "__FileExist")
	{
		InstructionBuilder::File_Exist(_Ins, RegisterID::Parameter1_Register, RegisterID::Parameter2_Register, RegisterID::OutPutRegister); PushIns();
	}
	else if (VFuncName == "__FilePExist")
	{
		InstructionBuilder::FilePChar_Exist(_Ins, RegisterID::Parameter1_Register, RegisterID::Parameter2_Register, RegisterID::OutPutRegister); PushIns();
	}
	else if (VFuncName == "__FileRemove")
	{
		InstructionBuilder::File_Remove(_Ins, RegisterID::Parameter1_Register, RegisterID::Parameter2_Register, RegisterID::OutPutRegister); PushIns();
	}
	else if (VFuncName == "__FilePRemove")
	{
		InstructionBuilder::FilePChar_Remove(_Ins, RegisterID::Parameter1_Register, RegisterID::Parameter2_Register, RegisterID::OutPutRegister); PushIns();
	}
	else
	{
		auto Ptr = _Output->AddStaticBytes((String_view)VFuncName);
		InstructionBuilder::CPPCall(Ptr, _Ins); PushIns();
	}

}
UCodeLangEnd

#endif