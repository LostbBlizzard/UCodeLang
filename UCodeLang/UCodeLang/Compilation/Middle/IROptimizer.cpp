#include "IROptimizer.hpp"
#include <iostream>
#include "../Helpers/CompilationErrors.hpp"
#include "../CompilationSettings.hpp"


#if UCodeLangDebug
#include <fstream>
#endif

UCodeLangStart



void IROptimizer::Reset() 
{
	this->~IROptimizer();
	new (this) IROptimizer;
}
void IROptimizer::Optimized(IRBuilder& IRcode)
{
	#define RunlogIRState 1
	Input = &IRcode;

	_IDChecker.Set_ErrorsOutput(_ErrorsOutput);
	_IDChecker.CheckForUB(Input);
	if (_ErrorsOutput->Has_Errors())
	{
		return;
	}


	UpdateOptimizationList();
	
	bool didonepass = false;
	
	do 
	{
		do
		{
			_TypeFixer.Set_ErrorsOutput(_ErrorsOutput);
			_TypeFixer.FixTypes(Input);

			

			#if RunlogIRState
			{//for debuging
				auto S = Input->ToString();

				std::cout << "-----" << std::endl;
				std::cout << S;

				std::ofstream file("OutIR.txt");
				file << S;
				file.close();
			}
			#endif // UCodeLangDebug

			if (_Settings->_Flags == OptimizationFlags::NoOptimization
				|| _Settings->_Flags == OptimizationFlags::Debug) { return; }
			
			if (true) { return; }

			if (_ErrorsOutput->Has_Errors())
			{
				return;
			}
			_UpdatedCode = false;

			for (auto& Func : Input->Funcs)
			{
				if (Func->Linkage == IRFuncLink::StaticExternalLink
					|| Func->Linkage == IRFuncLink::DynamicExternalLink)
				{
					if (Func->StaticPure.Read == PureState::Null)
					{
						Func->StaticPure.Read = PureState::ImPure;
					}
					if (Func->StaticPure.Write == PureState::Null)
					{
						Func->StaticPure.Write = PureState::ImPure;
					}
					if (Func->ThreadPure.Read == PureState::Null)
					{
						Func->ThreadPure.Read = PureState::ImPure;
					}
					if (Func->ThreadPure.Write == PureState::Null)
					{
						Func->ThreadPure.Write = PureState::ImPure;
					}
				}
			}
			for (auto& Func : Input->Funcs)
			{
				if (Func->Linkage == IRFuncLink::StaticExternalLink
					|| Func->Linkage == IRFuncLink::DynamicExternalLink)
				{
					continue;
				}
				IROptimizationFuncData& FuncData = Funcs.GetOrAdd(Func.get(), {});
				size_t InsCount = 0;

				for (auto& Block : Func->Blocks)
				{
					InsCount += Block->Instructions.size();
				}

				if (InsCount < 10)
				{
					FuncData.Inlinestate = InlineState::AutoInline;
				}
				else if (InsCount < 30)
				{
					FuncData.Inlinestate = InlineState::MayBeInlined;
				}
				else
				{
					FuncData.Inlinestate = InlineState::FuncionCantBeInline;
				}

			}
			Funcs.GetOrAdd(&Input->_StaticInit, {});
			Funcs.GetOrAdd(&Input->_threadInit, {});
			Funcs.GetOrAdd(&Input->_threaddeInit, {});
			Funcs.GetOrAdd(&Input->_StaticdeInit, {});



			didonepass = true;
			UpdateCodePass();


			//




		} while (_UpdatedCode);


		if (Optimization_RemoveFuncsWithSameBody)
		{
			BitMaker bits;
			UnorderedMap<size_t,const IRFunc*> Hashs;
			for (auto& Func : Input->Funcs)
			{
				auto& FuncData = Funcs.GetValue(Func.get());
				if (FuncData.BodyWasRemoved) { continue; }

				size_t Hash = 0;
				if (FuncData.FuncBodyWasUpdated || !FuncData.FuncBodyHash.has_value()) 
				{
					for (auto& Block : Func->Blocks)
					{
						for (size_t i = 0; i < Block->Instructions.size(); i++)
						{
							auto& Ins = Block->Instructions[i];
							if (Ins->Type == IRInstructionType::None)
							{
								Block->Instructions.erase(Block->Instructions.begin() + i);
								i--;
							}
						}
					}


					auto Tep = Func->Pars;
					auto TepID = Func->identifier;
					for (auto& Item : Func->Pars)
					{
						Item.identifier = 0;
					}
					Func->identifier = 0;

					Hash = Input->GetImplementationHash(bits, Func.get());

					Func->Pars = Tep;
					Func->identifier = TepID;

					FuncData.FuncBodyWasUpdated = false;
					FuncData.FuncBodyHash = Hash;
				}
				else
				{
					Hash = FuncData.FuncBodyHash.value();
				}


				if (Hashs.HasValue(Hash))
				{
					const auto& SameFunc = Hashs.GetValue(Hash);
					
					Func->Blocks.clear();

					auto Block = new IRBlock();
					Func->Blocks.push_back(Unique_ptr<IRBlock>(Block));

					for (size_t i = 0; i < Func->Pars.size(); i++)
					{
						Block->NewPushParameter(Block->NewLoad(&Func->Pars[i]));
					}
					
					auto V = Block->NewCall(SameFunc->identifier);

					if (Func->ReturnType._Type != IRTypes::Void)
					{
						Block->NewRetValue(V);	
					}
					Block->NewRet();


					//Update Other Calls
					{
						for (auto& Func : Input->Funcs)
						{
							for (auto& Block : Func->Blocks)
							{
								for (size_t i = 0; i < Block->Instructions.size(); i++)
								{
									auto& Ins = Block->Instructions[i];
									if (Ins->Type == IRInstructionType::Call)
									{
										Ins->Target().identifer = SameFunc->identifier;
										UpdatedCodeFor(Func.get());
									}

									if (IsOperatorValueInTarget(Ins->Type))
									{
										auto Op = Ins->Target();
										if (Op.Type == IROperatorType::Get_Func_Pointer)
										{
											Op.identifer = SameFunc->identifier;
											UpdatedCodeFor(Func.get());
										}
									}

									if (IsOperatorValueInInput(Ins->Type))
									{
										auto Op = Ins->Input();
										if (Op.Type == IROperatorType::Get_Func_Pointer)
										{
											Op.identifer = SameFunc->identifier;
											UpdatedCodeFor(Func.get());
										}
									}
								}
							}
						}
					}

					UpdatedCode();
					FuncData.BodyWasRemoved = true;
				}
				else
				{
					Hashs.AddValue(Hash, Func.get());
				}
			}
		}


	} while (_UpdatedCode);
	



	if (didonepass)
	{
		for (auto& Item : Funcs) 
		{
			UndoSSA(Item.first, Item.second.SsA.value());
		}
	}
}

void IROptimizer::UpdateOptimizationList()
{
	auto& Stettings = *_Settings;

	bool ForDebuging = (OptimizationFlags_t)Stettings._Flags & (OptimizationFlags_t)OptimizationFlags::Debug;
	bool ForSize =  (OptimizationFlags_t)Stettings._Flags & (OptimizationFlags_t)OptimizationFlags::ForSize;
	bool ForSpeed = (OptimizationFlags_t)Stettings._Flags & (OptimizationFlags_t)OptimizationFlags::ForSpeed;
	ResetOptimizations();
	if (ForSize)
	{
		if (ForDebuging == false)
		{
			
			Optimization_RemoveUnsedVarables = true;
			Optimization_RemoveUnusePars = true;
			Optimization_RemoveFuncsWithSameBody = true;
			Optimization_LowerToBodysToCFuncions = false;
			Optimization_InlineConditionalJump = false;
			Optimization_RemoveUnreachable = true;
			Optimization_RemoveUnneedMeallocAndFree = false;

			Optimization_FloatFastMath = Stettings.HasFlagArg("ffast-math");
		}

		
		Optimization_ConstantFoldVarables = true;
		Optimization_IndirectMemeberToDirectMemeber = true;
		Optimization_ConstantFuncPtrToDirectCall = true;
	}
	if (ForSpeed)
	{
		if (ForDebuging == false)
		{
			Optimization_DestructureStructMembers = true;
			Optimization_ReorderFuncionsInToHotSpots = true;
			Optimization_ReorderInstructionsBaseOnMemoryAndHotSpots = true;
			Optimization_LoopUnrolling = true;
			Optimization_LoopFrequencyReduction = true;


			if (IsIsolatedandIndependent)
			{
				Optimization_StaticAndThreadRemovealAndReadOnlyToValues = true;
			}
		}
		Optimization_ShortFuncInline = true;
	}
}
void IROptimizer::UpdateCodePass()
{
	{
		UpdateCodePassFunc(&Input->_StaticInit);

		UpdateCodePassFunc(&Input->_StaticdeInit);

		UpdateCodePassFunc(&Input->_threadInit);

		UpdateCodePassFunc(&Input->_threaddeInit);


		for (size_t i = 0; i < Input->Funcs.size(); i++)
		{
			auto& Func = Input->Funcs[i];
			UpdateCodePassFunc(Func.get());
		}
	}
	{
		DoInlines(&Input->_StaticInit);

		DoInlines(&Input->_StaticdeInit);

		DoInlines(&Input->_threadInit);

		DoInlines(&Input->_threaddeInit);


		for (size_t i = 0; i < Input->Funcs.size(); i++)
		{
			auto& Func = Input->Funcs[i];
			DoInlines(Func.get());
		}
	}
}
void IROptimizer::UpdateCodePassFunc(IRFunc* Func)
{
	if (Func->Linkage == IRFuncLink::DynamicExternalLink
		|| Func->Linkage == IRFuncLink::StaticExternalLink)
	{
		return;
	}
	if (!Funcs.GetValue(Func).SsA.has_value()) 
	{
		SSAState _data;
		ToSSA(Func, _data);
		Funcs.GetValue(Func).SsA = std::move(_data);
	}
	
	LookAtfunc = Func;
	

	for (auto& Par : Func->Pars)
	{
		Par._PtrRead = PureState::Null;
		Par._PtrWrite = PureState::Null;
		Par._IsUsed= false;
	}

	for (auto& Block : Func->Blocks)
	{
		for (auto& Ins : Block->Instructions)
		{
			Get_IRData(Ins.get()).IsReferenced = false;
		}

		for (size_t i = 0; i < Block->Instructions.size(); i++)
		{
			auto& Ins = Block->Instructions[i];
			if (Ins->Type == IRInstructionType::PushParameter)
			{
				ConstantFoldOperator(*Ins, Ins->Target(), ReadOrWrite::Read);
			}
			else if (Ins->Type == IRInstructionType::Logical_Not)
			{
				ConstantFoldOperator(*Ins, Ins->Target(), ReadOrWrite::Read);
			}
			else if (Ins->Type == IRInstructionType::Reassign)
			{
				ConstantFoldOperator(*Ins, Ins->Target(), ReadOrWrite::Write);
				ConstantFoldOperator(*Ins, Ins->Input(), ReadOrWrite::Read);
			}
			else if (Ins->Type == IRInstructionType::Load
				|| Ins->Type == IRInstructionType::LoadReturn)
			{
				ConstantFoldOperator(*Ins, Ins->Target(), ReadOrWrite::Read);
			}
			else if (IsBinary(Ins->Type))
			{
				ConstantFoldOperator(*Ins, Ins->A,ReadOrWrite::Read);
				ConstantFoldOperator(*Ins, Ins->B, ReadOrWrite::Read);
					#define	ConstantBinaryFold(bits) \
					switch (Ins->Type) \
					{\
					case IRInstructionType::Add:Ins->Target().Value.AsInt##bits = Ins->A.Value.AsInt##bits  + Ins->B.Value.AsInt##bits ;break;\
					case IRInstructionType::Sub:Ins->Target().Value.AsInt##bits = Ins->A.Value.AsInt##bits  - Ins->B.Value.AsInt##bits ;break;\
					case IRInstructionType::UMult:Ins->Target().Value.AsUInt##bits = Ins->A.Value.AsUInt##bits  * Ins->B.Value.AsUInt##bits ;break;\
					case IRInstructionType::SMult:Ins->Target().Value.AsInt##bits  = Ins->A.Value.AsInt##bits  * Ins->B.Value.AsInt##bits ;break;\
					case IRInstructionType::UDiv:Ins->Target().Value.AsUInt##bits  = Ins->A.Value.AsUInt##bits  / Ins->B.Value.AsUInt##bits ;break;\
					case IRInstructionType::SDiv:Ins->Target().Value.AsInt##bits  = Ins->A.Value.AsInt##bits  / Ins->B.Value.AsInt##bits ;break;\
					case IRInstructionType::EqualTo:Ins->Target().Value.AsInt##bits  = Ins->A.Value.AsInt##bits  == Ins->B.Value.AsInt##bits ;break;\
					case IRInstructionType::NotEqualTo:Ins->Target().Value.AsInt##bits  = Ins->A.Value.AsInt##bits  != Ins->B.Value.AsInt##bits ;break;\
					case IRInstructionType::SGreaterThan:Ins->Target().Value.AsInt##bits = Ins->A.Value.AsInt##bits > Ins->B.Value.AsInt##bits; break; \
					case IRInstructionType::SLessThan:Ins->Target().Value.AsInt##bits = Ins->A.Value.AsInt##bits < Ins->B.Value.AsInt##bits; break; \
					case IRInstructionType::SGreaterThanOrEqual:Ins->Target().Value.AsInt##bits = Ins->A.Value.AsInt##bits >= Ins->B.Value.AsInt##bits; break; \
					case IRInstructionType::SLessThanOrEqual:Ins->Target().Value.AsInt##bits = Ins->A.Value.AsInt##bits <= Ins->B.Value.AsInt##bits; break; \
					case IRInstructionType::UGreaterThan:Ins->Target().Value.AsUInt##bits = Ins->A.Value.AsUInt##bits > Ins->B.Value.AsUInt##bits; break; \
					case IRInstructionType::ULessThan:Ins->Target().Value.AsUInt##bits = Ins->A.Value.AsUInt##bits < Ins->B.Value.AsUInt##bits; break; \
					case IRInstructionType::UGreaterThanOrEqual:Ins->Target().Value.AsUInt##bits = Ins->A.Value.AsUInt##bits >= Ins->B.Value.AsUInt##bits; break; \
					case IRInstructionType::ULessThanOrEqual:Ins->Target().Value.AsUInt##bits = Ins->A.Value.AsUInt##bits <= Ins->B.Value.AsUInt##bits; break; \
					case IRInstructionType::Logical_And:Ins->Target().Value.AsInt##bits  = Ins->A.Value.AsInt##bits && Ins->B.Value.AsInt##bits ;break;\
					case IRInstructionType::Logical_Or:Ins->Target().Value.AsInt##bits  = Ins->A.Value.AsInt##bits || Ins->B.Value.AsInt##bits ;break;\
					default:\
						UCodeLangUnreachable();\
						break;\
					}\

				if (Ins->A.Type == IROperatorType::Value && Ins->B.Type == IROperatorType::Value)
				{
					bool ok = true;
					if (Ins->Type == IRInstructionType::UDiv || Ins->Type == IRInstructionType::SDiv)
					{
						if (Ins->ObjectType.IsType(IRTypes::i8))
						{
							ok = Ins->B.Value.AsInt8 != 0;
						}
						else if (Ins->ObjectType.IsType(IRTypes::i16))
						{
							ok = Ins->B.Value.AsInt16 != 0;
						}
						else if (Ins->ObjectType.IsType(IRTypes::i32))
						{
							ok = Ins->B.Value.AsInt32 != 0;
						}
						else if (Ins->ObjectType.IsType(IRTypes::i64))
						{
							ok = Ins->B.Value.AsInt64 != 0;
						}
						else if (Ins->ObjectType.IsType(IRTypes::f32))
						{
							if (Optimization_FloatFastMath)
							{
								//TODO add check for floats
							}
							else {
								ok = false;
							}
						}
						else if (Ins->ObjectType.IsType(IRTypes::f64))
						{
							if (Optimization_FloatFastMath)
							{
								//TODO add check  for floats
							}
							else {
								ok = false;
							}
						}
						else
						{
							UCodeLangUnreachable();
						}
					}

					if (ok) 
					{
						if (Ins->ObjectType.IsType(IRTypes::i8))
						{
							ConstantBinaryFold(8);
							Ins->Type = IRInstructionType::Load;
							UpdatedCode();
						}
						else if (Ins->ObjectType.IsType(IRTypes::i16))
						{
							ConstantBinaryFold(16);
							Ins->Type = IRInstructionType::Load;
							UpdatedCode();
						}
						else if (Ins->ObjectType.IsType(IRTypes::i32))
						{
							ConstantBinaryFold(32);
							Ins->Type = IRInstructionType::Load;
							UpdatedCode();
						}
						else if (Ins->ObjectType.IsType(IRTypes::i64))
						{
							ConstantBinaryFold(64);
							Ins->Type = IRInstructionType::Load;
							UpdatedCode();
						}
						else if (Ins->ObjectType.IsType(IRTypes::f32))
						{
							if (Optimization_FloatFastMath)
							{
								//TODO ConstantBinaryFold for floats
							}
						}
						else if (Ins->ObjectType.IsType(IRTypes::f64))
						{
							if (Optimization_FloatFastMath)
							{
								//TODO ConstantBinaryFold for floats
							}
						}
						else
						{
							UCodeLangUnreachable();
						}
					}
				}
			}
			else if (Ins->Type == IRInstructionType::Member_Access_Dereference)
			{
				ConstantFoldOperator(*Ins, Ins->Target(), ReadOrWrite::Read);
				if (Optimization_IndirectMemeberToDirectMemeber)
				{
					auto target = Ins->Target();
					
					
					if (target.Type == IROperatorType::Get_PointerOf_IRidentifier)
					{
						Ins->Type = IRInstructionType::Member_Access;
						Ins->Target() = target.identifer;

						UpdatedCode();
					}
					else if (target.Type == IROperatorType::Get_PointerOf_IRParameter)
					{
						Ins->Type = IRInstructionType::Member_Access;
						Ins->Target() = target.Parameter;

						UpdatedCode();
					}
					else if (target.Type == IROperatorType::Get_PointerOf_IRInstruction)
					{
						Ins->Type = IRInstructionType::Member_Access;

						Ins->Target() = target.Pointer;
						
						UpdatedCode();
					}
				}
			}
			else if (Ins->Type == IRInstructionType::Member_Access)
			{
				ConstantFoldOperator(*Ins, Ins->Target(), ReadOrWrite::Read);
			}
			else if (Ins->Type == IRInstructionType::Return)
			{
				
			}
			else if (Ins->Type == IRInstructionType::Call)
			{

			}
			else if (Ins->Type == IRInstructionType::CallFuncPtr)
			{
				ConstantFoldOperator(*Ins, Ins->Target(), ReadOrWrite::Read);
				
				if (Optimization_ConstantFuncPtrToDirectCall) 
				{
					if (Ins->Target().Type == IROperatorType::Get_Func_Pointer)
					{
						auto func = Ins->Target().identifer;
						Ins->Type = IRInstructionType::Call;
						Ins->Target().identifer = func;

						UpdatedCode();
					}
				}
			}
			else if (Ins->Type == IRInstructionType::LoadNone)
			{
				if (Optimization_DestructureStructMembers) 
				{
					bool IsStruct = false;
					IRStruct* structtype = nullptr;
					if (Ins->ObjectType._Type == IRTypes::IRsymbol)
					{
						auto syb = Input->GetSymbol(Ins->ObjectType._symbol);
						IsStruct = syb->SymType == IRSymbolType::Struct;
						
						if (IsStruct) 
						{//Just Check for offsets and not just Union.
							structtype = syb->Get_ExAs<IRStruct>();

							IsStruct = !structtype->IsUnion;
						}
					}

					if (IsStruct)
					{
						bool UsesPointerOf = false;
						for (auto& Block : Func->Blocks)
						{
							for (auto& Insv : Block->Instructions)
							{
								auto OnOp = [&Ins, &Insv,&UsesPointerOf](IROperator& op)
								{
									if (op.Type == IROperatorType::Get_PointerOf_IRInstruction)
									{
										if (op.Pointer == Ins.get())
										{
											return true;
										}
									}
									return false;
								};
								if (IsLoadValueOnlyInTarget(Insv->Type))
								{
									if (OnOp(Insv->Target()))
									{
										UsesPointerOf = true;
										goto Done;
									}
								}
								if (IsLoadValueOnInput(Insv->Type))
								{
									if (OnOp(Insv->Input()))
									{
										UsesPointerOf = true;
										goto Done;
									}
								}
							}
						}
					Done:

						if (!UsesPointerOf)
						{
							DoDestructurStructMembers(Ins, structtype, Func, Block, i);
						}
					}
				}
			}
			else if (Ins->Type == IRInstructionType::None)
			{

			}
			else if (Ins->Type == IRInstructionType::MallocCall)
			{
				ConstantFoldOperator(*Ins, Ins->Target(), ReadOrWrite::Read);
			}
			else if (Ins->Type == IRInstructionType::FreeCall)
			{
				ConstantFoldOperator(*Ins, Ins->Target(), ReadOrWrite::Read);
			}
			else if (Ins->Type == IRInstructionType::Reassign_dereference)
			{
				ConstantFoldOperator(*Ins, Ins->Target(), ReadOrWrite::Write);
				ConstantFoldOperator(*Ins, Ins->Input(), ReadOrWrite::Read);
			}
			else if (Ins->Type == IRInstructionType::ConditionalJump)
			{

			}
			else
			{
				UCodeLangUnreachable();
			}


		}
		if (Optimization_RemoveUnsedVarables)
		{
			for (auto& Ins : Block->Instructions)
			{
				if (Ins->Type == IRInstructionType::Load) //removeing dead Instructions
				{
					auto& Data = Get_IRData(Ins.get());
					if (!Data.IsReferenced)
					{
						Ins->SetAsNone(); UpdatedCode();
					}
				}

				if (Ins->Type == IRInstructionType::LoadNone)//removeing dead Instructions
				{
					auto& Data = Get_IRData(Ins.get());
					if (!Data.IsReferenced)
					{
						Ins->SetAsNone(); UpdatedCode();
					}
				}

				if (Ins->Type == IRInstructionType::Reassign)//removeing dead Instructions
				{
					if (Ins->Target().Pointer->Type == IRInstructionType::None)
					{
						Ins->SetAsNone();
						UpdatedCode();
					}
				}
			}
		}
	}
	
	

	if (Optimization_RemoveUnusePars && Func->Linkage == IRFuncLink::StaticLink)
	{
		for (size_t i = 0; i < Func->Pars.size(); i++)
		{
			auto& Par = Func->Pars[i];

			if (!Par._IsUsed && Par._WasRemoved == false)
			{
				CopyFuncionWithoutUnusedParAndUpdateOtherCalls(Par, Func, i);
				UpdatedCode();
				break;
			}
		}
	}

}
void IROptimizer::DoDestructurStructMembers(UCodeLang::Unique_ptr<UCodeLang::IRInstruction>& Ins, UCodeLang::IRStruct* structtype, UCodeLang::IRFunc* Func, UCodeLang::Unique_ptr<UCodeLang::IRBlock>& Block, const size_t& i)
{
	//std::cout << Input->ToString() << '\n';
	Vector<IRInstruction*> LoadFields;
	Ins->SetAsNone();

	LoadFields.resize(structtype->Fields.size());
	for (size_t i = 0; i < structtype->Fields.size(); i++)
	{
		auto& Item = structtype->Fields[i];

		auto v = new IRInstruction();
		v->Type = IRInstructionType::LoadNone;
		v->ObjectType = Item.Type;

		LoadFields[i] = v;
	}


	//update  for stores
	for (auto& Block : Func->Blocks)
	{
		for (auto& Insv : Block->Instructions)
		{

			//this does not work for stores
			if (Insv->Type == IRInstructionType::Reassign
				|| Insv->Type == IRInstructionType::Reassign_dereference)
			{
				auto& obj = Insv->Target();
				if (obj.Type == IROperatorType::IRInstruction)
				{
					if (obj.Pointer->Type == IRInstructionType::Load)
					{
						auto ptr = obj.Pointer;
						auto& target = ptr->Target();
						if (target.Type == IROperatorType::IRInstruction)
						{
							if (Ins.get() == target.Pointer)
							{
								//Make TargetVec = InputVec2
								//To
								//TargetVec.X = InputVec2.X
								//TargetVec.Y = InputVec2.Y
								UCodeLangToDo();
							}

						}
						
						
					}
					else if (obj.Pointer->Type == IRInstructionType::Member_Access)
					{
						auto ptr = obj.Pointer;
						auto& target = ptr->Target(); 
						if (target.Type == IROperatorType::IRInstruction)
						{
							if (Ins.get() == target.Pointer)
							{

								size_t MemberIndex = obj.Pointer->Input().Value.AsUIntNative;

								IRInstruction* LoadField = LoadFields[MemberIndex];

								obj = LoadField;
							}
						}
						else if (target.Type == IROperatorType::DereferenceOf_IRInstruction)
						{
							if (Ins.get() == target.Pointer)
							{
								UCodeLangToDo();
							}
						}
					}
				}
			}
		}
	}
	//update reads
	for (auto& Block : Func->Blocks)
	{
		for (auto& Insv : Block->Instructions)
		{
			auto OnOp = [&LoadFields,&Ins, &Insv](IROperator& op)
			{
				if (op.Type == IROperatorType::IRInstruction)
				{
					auto target = op.Pointer;
					if (target->Type == IRInstructionType::Member_Access)
					{
						auto& obj = target->Target();

						if (obj.Pointer == Ins.get())
						{
							size_t MemberIndex = target->Input().Value.AsUIntNative;

							IRInstruction* LoadField = LoadFields[MemberIndex];

							op = LoadField;
						}
					}
				}
				else if (op.Type == IROperatorType::IRInstruction)
				{
					auto target = op.Pointer;
					if (target->Type == IRInstructionType::Member_Access)
					{
						auto& obj = target->Target();

						if (obj.Pointer == Ins.get())
						{
							UCodeLangToDo();
						}
					}
				}
			};


			if (IsLoadValueOnlyInTarget(Insv->Type))
			{
				OnOp(Insv->Target());
			}
			if (IsLoadValueOnInput(Insv->Type))
			{
				OnOp(Insv->Input());
			}
		}
	}
	//remove memeber
	for (auto& Block : Func->Blocks)
	{
		for (auto& Insv : Block->Instructions)
		{
			if (Insv->Type == IRInstructionType::Member_Access)
			{
				auto& obj = Insv->Target();
				if (obj.Type == IROperatorType::IRInstruction
					|| obj.Type == IROperatorType::DereferenceOf_IRInstruction)
				{
					if (obj.Pointer == Ins.get())
					{
						Insv->SetAsNone();
					}
				}

			}
		}
	}


	{
		size_t LoadIndex = 0;
		for (auto& Item : LoadFields)//the order of Fields does not doesn't matter
		{
			Block->Instructions.insert(
				Block->Instructions.begin() + i + LoadIndex
				, Unique_ptr<IRInstruction>(Item));

			LoadIndex++;
		}
	}

	UpdatedCode();

	//std::cout << Input->ToString() << '\n';
}
void IROptimizer::CopyFuncionWithoutUnusedParAndUpdateOtherCalls(UCodeLang::IRPar& Par, UCodeLang::IRFunc* Func, const size_t& i)
{
	Par._WasRemoved = true;
	String NewFuncName = Input->FromID(Func->identifier) + "|||-unused[" + Input->FromID(Par.identifier) + "]";
	auto NewID = Input->ToID(NewFuncName);


	IRFunc NewFunc;
	NewFunc.Pars.reserve(Func->Pars.size() - 1);
	for (size_t ix = 0; ix < NewFunc.Pars.size(); ix++)
	{
		if (i == ix)
		{
			continue;
		}
		NewFunc.Pars.push_back(Func->Pars[ix]);
	}
	NewFunc.StaticPure = Func->StaticPure;
	NewFunc.ThreadPure = Func->ThreadPure;
	NewFunc.MustBePrerived = false;
	NewFunc.ReturnType = Func->ReturnType;
	NewFunc.identifier = NewID;
	NewFunc.Blocks = std::move(Func->Blocks);

	for (auto& Item : NewFunc.Blocks)
	{
		for (auto& Ins : Item->Instructions)
		{
			if (IsOperatorValueInTarget(Ins->Type))
			{
				auto& Op = Ins->Target();
				if (Op.Type == IROperatorType::IRParameter ||
					Op.Type == IROperatorType::DereferenceOf_IRParameter ||
					Op.Type == IROperatorType::Get_PointerOf_IRParameter)
				{
					size_t Index = NewFunc.Pars.data() - Op.Parameter;

					if (Index > i)
					{
						Index--;
					}

					Op.Parameter = &NewFunc.Pars[Index];
				}
			}
			if (IsLoadValueOnInput(Ins->Type))
			{
				auto& Op = Ins->Input();
				if (Op.Type == IROperatorType::IRParameter ||
					Op.Type == IROperatorType::DereferenceOf_IRParameter ||
					Op.Type == IROperatorType::Get_PointerOf_IRParameter)
				{
					size_t Index = NewFunc.Pars.data() - Op.Parameter;

					if (Index > i)
					{
						Index--;
					}

					Op.Parameter = &NewFunc.Pars[Index];
				}
			}
		}
	}

	Input->Funcs.push_back(Unique_ptr<IRFunc>(new IRFunc(std::move(NewFunc))));

	{//update all other calls
		UpdateCallWhenParWasRemoved(&Input->_StaticInit, Func, NewFunc, i);
		UpdateCallWhenParWasRemoved(&Input->_StaticdeInit, Func, NewFunc, i);

		UpdateCallWhenParWasRemoved(&Input->_threadInit, Func, NewFunc, i);
		UpdateCallWhenParWasRemoved(&Input->_threaddeInit, Func, NewFunc, i);
		for (auto& Item : Input->Funcs)
		{
			UpdateCallWhenParWasRemoved(Item.get(), Func, NewFunc, i);
		}
	}

	{//build code to call new Func
		IRBlock* Block = new IRBlock();
		Func->Blocks.push_back(Unique_ptr<IRBlock>(Block));
		for (size_t ix = 0; ix < Func->Pars.size(); ix++)
		{
			if (ix == i) { continue; }
			Block->NewPushParameter(Block->NewLoad(&Func->Pars[i]));
		}
		auto R = Block->NewCall(NewID);

		if (Func->ReturnType._Type != IRTypes::Void)
		{
			Block->NewRetValue(R);
		}
		Block->NewRet();
	}
}
void IROptimizer::UpdateCallWhenParWasRemoved(IRFunc* Item, const IRFunc* Func, const IRFunc& NewFunc,size_t i)
{
	for (auto& Block : Item->Blocks)
	{
		for (size_t iy = 0; iy < Block->Instructions.size(); iy++)
		{
			auto& Ins = Block->Instructions[iy];
			if (Ins->Type == IRInstructionType::Call)
			{
				if (Ins->Target().identifer == Func->identifier)
				{
					Ins->Target().identifer = NewFunc.identifier;

					size_t ParIndex = Func->Pars.size() - 1;
					for (int iz = iy - 1; iz >= 0; iz--)
					{
						auto& ParIns = Block->Instructions[iz];

						if (ParIns->Type == IRInstructionType::PushParameter)
						{
							if (ParIndex == i)
							{
								ParIns->Type = IRInstructionType::Load;
								UpdatedCodeFor(Item);
							}

							if (ParIndex == 0)
							{
								break;
							}

							ParIndex--;
						}
					}
				}
			}
		}
		for (auto& Ins : Block->Instructions)
		{

		}

	}
}

void IROptimizer::DoInlines(IRFunc* Func)
{
	auto& FuncData = Funcs.GetValue(Func);
	if (FuncData.BodyWasRemoved) { return; }

	for (auto& Item : Func->Blocks)
	{
		DoInlines(Func,Item.get());
	}
}
void IROptimizer::DoInlines(IRFunc* Func,IRBlock* Block)
{
	for (size_t i = 0; i < Block->Instructions.size(); i++)
	{
		auto& Ins = Block->Instructions[i];

		if (Ins->Type == IRInstructionType::Call)
		{
			auto* FuncToCall = Input->GetFunc(Ins->Target().identifer);
			if (FuncToCall)
			{
				IROptimizationFuncData& FuncData = Funcs.GetValue(FuncToCall);

				if (FuncData.Inlinestate == InlineState::AutoInline)
				{
					InLineData Data;
					Data.Block = Block;
					Data.CallIndex = i;
					Data.Func = Func;

					if (Optimization_ShortFuncInline) {
						InLineFunc(Data);
					}
				}
			}
		}
	}
}
void IROptimizer::ConstantFoldOperator(IRInstruction& I, IROperator& Value,ReadOrWrite OpType)
{

	if (Value.Type == IROperatorType::IRInstruction
		|| Value.Type == IROperatorType::Get_PointerOf_IRInstruction
		|| Value.Type == IROperatorType::DereferenceOf_IRInstruction)
	{
		auto Ptr = Value.Pointer;
		if (Ptr->Type == IRInstructionType::Load)
		{
			if (Ptr->Target().Type == IROperatorType::Value)
			{
				Value = Ptr->Target();
				I.ObjectType = Ptr->ObjectType;
				UpdatedCode();
			}
			else if (Ptr->Target().Type == IROperatorType::IRParameter)
			{
				Value = Ptr->Target();
				UpdatedCode();
			}
			else if (Ptr->Target().Type == IROperatorType::Get_PointerOf_IRInstruction)
			{
				Value = Ptr->Target();
				UpdatedCode();
			}
		}
		else if (Ptr->Type == IRInstructionType::Member_Access_Dereference)
		{
			ConstantFoldOperator(*Ptr,Ptr->Target(), OpType);
		}
		else if (Ptr->Type == IRInstructionType::Member_Access)
		{
			ConstantFoldOperator(*Ptr, Ptr->Target(), OpType);
		}

		Get_IRData(Ptr).IsReferenced = true;
	}

	if (Value.Type == IROperatorType::IRParameter
		|| Value.Type == IROperatorType::Get_PointerOf_IRParameter
		|| Value.Type == IROperatorType::DereferenceOf_IRParameter
		)
	{
		auto Ptr = Value.Parameter;
		Ptr->_IsUsed = true;
	}
	if (Value.Type == IROperatorType::DereferenceOf_IRParameter)
	{
		auto Ptr = Value.Parameter;
		if (OpType == ReadOrWrite::Read)
		{
			Ptr->_PtrRead =PureState::ImPure;
		}
		else
		{
			Ptr->_PtrWrite = PureState::ImPure;
		}
	}

}
void IROptimizer::ToSSA(IRFunc* Func, SSAState& state)
{
	if (Func->Blocks.size() == 0) { return; }
	UCodeLangAssert(Func->Blocks.size());

	auto Block = Func->Blocks.front().get();
	UnorderedMap<size_t,size_t> IndexToBlock;
	for (size_t i = 0; i < Block->Instructions.size(); i++)
	{
		auto& I = Block->Instructions[i];
		switch (I->Type)
		{
		case IRInstructionType::Jump:
		case IRInstructionType::ConditionalJump:
			if (!IndexToBlock.HasValue(I->Target().identifer))
			{
				IRBlock v;

				Func->Blocks.push_back(std::make_unique<IRBlock>(std::move(v)));
				IndexToBlock.AddValue(I->Target().identifer,Func->Blocks.size()-1);


			}
			break;
		}
	}


	IRBlock* OnBlock = Block;
	auto size = Block->Instructions.size();
	for (size_t i = 0; i < size; i++)
	{
		auto& Ins = Block->Instructions[i];


		if (IndexToBlock.HasValue(i))
		{
			OnBlock = Func->Blocks[IndexToBlock.GetValue(i)].get();
		}

		if (Ins->Type == IRInstructionType::Jump)
		{
			Ins->Type = IRInstructionType::JumpBlock;
			Ins->Target().Value = AnyInt64(IndexToBlock.GetValue(Ins->Target().Value.AsUIntNative));
		}
		else if (Ins->Type == IRInstructionType::ConditionalJump)
		{
			Ins->Type = IRInstructionType::JumpBlockIf;
			Ins->Target().Value = AnyInt64(IndexToBlock.GetValue(Ins->Target().Value.AsUIntNative));
		}

		OnBlock->Instructions.push_back(std::move(Ins));

	}

	std::reverse(Func->Blocks.begin() + 1, Func->Blocks.end());
	
	for (auto& Item : Func->Blocks) 
	{
		Item->DebugInfo.DebugInfo.clear();
		auto& myList = Item->Instructions;
		myList.erase(
			std::remove_if(myList.begin(), myList.end(),
				[](const Unique_ptr<IRInstruction>& o) { return o.get() ==nullptr || o->Type ==IRInstructionType::None; }),
			myList.end());

	}
	
	auto S = Input->ToString();

	std::cout << "-----" << std::endl;
	std::cout << S;
	/*
	for (auto& Block : Func->Blocks)
	{
		for (size_t i = 0; i < Block->Instructions.size(); i++)
		{
			auto& Ins = Block->Instructions[i];

			if (Ins->Type == IRInstructionType::Reassign)
			{
				IROperator ToUpdate = Ins->Target();

				if (Ins->Target().Type == IROperatorType::IRInstruction)
				{
					if (Ins->Target().Pointer->Type == IRInstructionType::Member_Access_Dereference) {
						continue;
					}
				}
				Ins->Type = IRInstructionType::Load;
				Ins->Target() =Ins->Input();
				Ins->Input() = IROperator();

				state.Map.AddValue(ToUpdate,Ins.get());
			}
			else
			{
				if (IsOperatorValueInTarget(Ins->Type))
				{
					if (state.Map.HasValue(Ins->Target()))
					{
						state.Updated.AddValue(&Ins->Target(), Ins->Target());
						Ins->Target() = IROperator(state.Map.GetValue(Ins->Target()));
					}
				}	
				
				if (IsOperatorValueInInput(Ins->Type))
				{
					if (state.Map.HasValue(Ins->Input()))
					{
						state.Updated.AddValue(&Ins->Input(), Ins->Input());
						Ins->Target() = IROperator(state.Map.GetValue(Ins->Input()));
					}
				}
			}
		}
	}
	*/
}
void  IROptimizer::UndoSSA(IRFunc* Func, const SSAState& state)
{
	for (auto& Item : state.Updated)
	{
		*Item.first = Item.second;
	}
}
void IROptimizer::InLineFunc(InLineData& Data)
{
	IRInstruction* Call = Data.Block->Instructions[Data.CallIndex].get();

	//IRFunc* CalleFunc = Data.Func;
	const IRFunc* FuncToCall = Input->GetFunc(Call->Target().identifer);


	auto FuncParsCount = FuncToCall->Pars.size();
	Vector<IRInstruction*> PushIns;//its Parameters are backwards

	for (size_t i = Data.CallIndex - 1; i != (size_t)-1; i--)
	{
		IRInstruction* Ins = Data.Block->Instructions[i].get();
		if (Ins->Type != IRInstructionType::PushParameter)
		{
			continue;
		}

		PushIns.push_back(Ins);
		if (PushIns.size() == FuncParsCount) { break; }
	}

	for (size_t i = 0; i < PushIns.size() / 2; i++)
	{
		auto& Item = PushIns[i];
		auto& Item2 = PushIns[PushIns.size() - i];
		std::swap(Item, Item2);
	}


	Vector< IRInstruction*> AddedLoadRets;
	IRInstruction* CalledRetVar = nullptr;
	Vector<IRInstruction*> _MovedIns;
	Data.AddIns = &_MovedIns;



	{//move func at call site
		size_t Offset = 0;
		for (auto& Item : PushIns)
		{
			Item->Type = IRInstructionType::Load;
			Data.InlinedPushedPars.push_back(Item);
		}

		if (FuncToCall->ReturnType._Type != IRTypes::Void)
		{

			auto NewIns = Unique_ptr<IRInstruction>(new IRInstruction());
			NewIns->Type = IRInstructionType::LoadNone;
			NewIns->ObjectType = FuncToCall->ReturnType;
			CalledRetVar = NewIns.get();
			Data.Block->Instructions.insert(Data.Block->Instructions.begin() + Data.CallIndex + Offset, std::move(NewIns));


			Data.CallIndex++;
		}

		for (const auto& Block : FuncToCall->Blocks)
		{
			for (const auto& Item : Block->Instructions)
			{
				if (Item->Type == IRInstructionType::Return)
				{
					continue;
				}
				if (Item->Type == IRInstructionType::LoadReturn)
				{

					auto NewIns = Unique_ptr<IRInstruction>(new IRInstruction(*Item));

					{
						NewIns->Type = IRInstructionType::Reassign;
						NewIns->Input() = NewIns->Target();
						InLineSubOperator(Data, NewIns->Input(), Offset);
						NewIns->Target() = IROperator(CalledRetVar);
					}
					AddedLoadRets.push_back(NewIns.get());
					Data.Block->Instructions.insert(Data.Block->Instructions.begin() + Data.CallIndex +  Offset, std::move(NewIns));
					Offset++;

				}
				else
				{
					auto NewIns = Unique_ptr<IRInstruction>(new IRInstruction(*Item));

					_MovedIns.push_back(NewIns.get());

					if (IsOperatorValueInTarget(NewIns->Type))
					{
						InLineSubOperator(Data, NewIns->Target(), Offset);
					}

					if (IsOperatorValueInInput(NewIns->Type))
					{
						InLineSubOperator(Data, NewIns->Input(), Offset);
					}

					Data.Block->Instructions.insert(Data.Block->Instructions.begin() + Data.CallIndex + Offset, std::move(NewIns));
					Offset++;
				}
			}
		}

	}


	if (FuncToCall->ReturnType._Type == IRTypes::Void)
	{
		Call->Type = IRInstructionType::LoadNone;
	}
	else
	{
		Call->Type = IRInstructionType::Load;
		Call->Target() = IROperator(CalledRetVar);
	}

	UpdatedCode();
	//
	
	auto S = Input->ToString();

	std::cout << "-----" << std::endl;
	std::cout << S;

	{
		SSAState S;
		ToSSA(Data.Func, S);
	}

	auto S2 = Input->ToString();

	std::cout << "-----" << std::endl;
	std::cout << S2;

}
void IROptimizer::InLineSubOperator(InLineData& Data, IROperator& Op, size_t Offset)
{
	IRInstruction* Call = Data.Block->Instructions[Data.CallIndex + Offset].get();
	const IRFunc* CallFunc = Input->GetFunc(Call->Target().identifer);

#if UCodeLangDebug
	if (CallFunc == nullptr)
	{
		UCodeLangUnreachable();
	}
#endif // DEBUG


	if (Op.Type == IROperatorType::IRParameter)
	{
		size_t IndexPar = Op.Parameter - CallFunc->Pars.data();

		Op = IROperator(IROperatorType::IRInstruction, Data.InlinedPushedPars[IndexPar]);
	}
	else if (Op.Type == IROperatorType::DereferenceOf_IRParameter)
	{
		size_t IndexPar = Op.Parameter - CallFunc->Pars.data();

		Op = IROperator(IROperatorType::DereferenceOf_IRInstruction, Data.InlinedPushedPars[IndexPar]);
	}
	else if (Op.Type == IROperatorType::Get_PointerOf_IRParameter)
	{
		size_t IndexPar = Op.Parameter - CallFunc->Pars.data();

		Op = IROperator(IROperatorType::Get_PointerOf_IRInstruction, Data.InlinedPushedPars[IndexPar]);
	}

	else if (Op.Type == IROperatorType::IRInstruction
		|| Op.Type == IROperatorType::Get_PointerOf_IRInstruction
		|| Op.Type == IROperatorType::DereferenceOf_IRInstruction)
	{
		bool Set = false;
		size_t Index = 0;
		for (size_t i = 0; i < CallFunc->Blocks.size(); i++)
		{
			auto& Block = CallFunc->Blocks[i];
			for (auto& BlockIns : Block->Instructions)
			{
				if (BlockIns.get() == Op.Pointer)
				{
					Set = true;
					break;
				}
				Index++;
			}
			if (Set) { break; }
		}

		if (Set == false)
		{
			UCodeLangUnreachable();
		}
		Op = IROperator(Op.Type, (*Data.AddIns)[Index]);
	}

}



UCodeLangEnd

