#ifndef UCodeLangNoCompiler
#include "IROptimizer.hpp"
#include <iostream>
#include "../Helpers/CompilationErrors.hpp"
#include "../CompilationSettings.hpp"


#if UCodeLangDebug
#include <fstream>
#endif

UCodeLangStart



#define IsOptimizerStable 0

#define RunlogIRState UCodeLangDebug
//#define RunlogIRState 1

void IROptimizer::Reset() 
{
	this->~IROptimizer();
	new (this) IROptimizer;
}
void IROptimizer::Optimized(IRBuilder& IRcode)
{
	Input = &IRcode;

	_IDChecker.Set_ErrorsOutput(_ErrorsOutput);
	_IDChecker.CheckForUB(Input);
	if (_ErrorsOutput->Has_Errors())
	{
		return;
	}


	UpdateOptimizationList();
	
	bool didonepass = false;
	
	if (Optimization_RemoveIdenticalTypes)
	{
		bool Updated = false;
		Vector<IRSymbolData*> RemovedTypes;
		do
		{
			Updated = false;
			for (auto& Item1 : Input->_Symbols)
			{
				{
					bool isinlist = false;
					for (auto& ItemV : RemovedTypes)
					{
						if (ItemV == Item1.get())
						{
							isinlist = true;
							break;
						}
					}
					if (isinlist)
					{
						continue;
					}
				}
				for (auto& Item2 : Input->_Symbols)
				{
					if (&Item2 != &Item1
						&& Item1->SymType == Item2->SymType)
					{
						if (Item1->SymType == IRSymbolType::Struct)
						{
							IRStruct* Struct1 = Item1->Get_ExAs<IRStruct>();
							IRStruct* Struct2 = Item2->Get_ExAs<IRStruct>();

							if (Struct1->Fields.size() == Struct2->Fields.size())
							{
								if (Struct1->ObjectSize == Struct2->ObjectSize)
								{
									bool isfieldsthesame = true;
									for (size_t i = 0; i < Struct1->Fields.size(); i++)
									{
										auto& Field1 = Struct1->Fields[i];
										auto& Field2 = Struct2->Fields[i];

										if (Field1.Offset != Field2.Offset
											|| Field1.Type != Field2.Type)
										{
											isfieldsthesame = false;
											break;
										}

									}

									if (isfieldsthesame)
									{
										bool isinlist = false;
										for (auto& ItemV : RemovedTypes)
										{
											if (ItemV == Item2.get())
											{
												isinlist = true;
												break;
											}
										}
										if (!isinlist)
										{
											RemovedTypes.push_back(Item2.get());

											auto id1 = Item1->Type._symbol.ID;
											auto id2 = Item2->Type._symbol.ID;
											ReplaceAllTypesTo(id2, id1);
											Updated = true;
										}
									}
								}
							}
							if (Item1->SymType == IRSymbolType::FuncPtr)
							{
								IRFuncPtr* Struct1 = Item1->Get_ExAs<IRFuncPtr>();
								IRFuncPtr* Struct2 = Item2->Get_ExAs<IRFuncPtr>();

								if (Struct1->Pars.size() == Struct2->Pars.size()
									&& Struct1->Ret == Struct2->Ret)
								{
									if (Struct1->Ret == Struct2->Ret)
									{
										bool isfieldsthesame = true;
										for (size_t i = 0; i < Struct1->Pars.size(); i++)
										{
											auto& Field1 = Struct1->Pars[i];
											auto& Field2 = Struct2->Pars[i];

											if (Field1 == Field2)
											{
												isfieldsthesame = false;
												break;
											}
										}
										if (isfieldsthesame)
										{
											bool isinlist = false;
											for (auto& ItemV : RemovedTypes)
											{
												if (ItemV == Item2.get())
												{
													isinlist = true;
													break;
												}
											}
											if (!isinlist)
											{
												RemovedTypes.push_back(Item2.get());

												auto id1 = Item1->Type._symbol.ID;
												auto id2 = Item2->Type._symbol.ID;
												ReplaceAllTypesTo(id2, id1);
												Updated = true;
											}
										}
									}

								}
							}
							if (Item1->SymType == IRSymbolType::StaticArray)
							{
								IRStaticArray* Struct1 = Item1->Get_ExAs<IRStaticArray>();
								IRStaticArray* Struct2 = Item2->Get_ExAs<IRStaticArray>();

								if (Struct1->Count == Struct2->Count &&
									Struct1->Type == Struct2->Type)
								{

									bool isinlist = false;
									for (auto& ItemV : RemovedTypes)
									{
										if (ItemV == Item2.get())
										{
											isinlist = true;
											break;
										}
									}
									if (!isinlist)
									{
										RemovedTypes.push_back(Item2.get());

										auto id1 = Item1->Type._symbol.ID;
										auto id2 = Item2->Type._symbol.ID;
										ReplaceAllTypesTo(id2, id1);
										Updated = true;
									}
								}
							}
						}
					}
				}
			}

		} while (Updated);


		bool removetypesfromIR = _Settings->_Type != OutPutType::IRAndSymbols;
		if (removetypesfromIR)
		{
			Input->_Symbols.erase(std::remove_if(
				Input->_Symbols.begin(),
				Input->_Symbols.end(),
				[RemovedTypes](Unique_ptr<IRSymbolData>& Item)
				{
					bool isinlist = false;
					for (auto& Item2 : RemovedTypes)
					{
						if (Item2 == Item.get())
						{
							isinlist = true;
							break;
						}
					}

					return  isinlist;
				}), Input->_Symbols.end());

		}
	}

	do 
	{
		do
		{
			_TypeFixer.Set_CompilerSetting(_Settings);
			_TypeFixer.Set_ErrorsOutput(_ErrorsOutput);
			_TypeFixer.FixTypes(Input);

			
			#if RunlogIRState
			{//for debuging
				//auto S = Input->ToString();

				//std::cout << "-----" << std::endl;
				//std::cout << S;

				//std::ofstream file("OutIR.txt");
				//file << S;
				//file.close();
			}
			#endif // UCodeLangDebug

			#if !IsOptimizerStable
			return;
			#endif


			if (_Settings->_Flags == OptimizationFlags::NoOptimization
				|| _Settings->_Flags == OptimizationFlags::Debug) { return; }

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
					FuncData.Inlinestate = InlineState::FunctionCantBeInline;
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
			UnorderedMap<size_t,const IRFunc*> Hashes;

			for (auto& Func : Input->Funcs)
			{

				auto& FuncData = Funcs.GetValue(Func.get());
				if (FuncData.BodyWasRemoved) { continue; }

				size_t Hash = 0;
				if (FuncData.FuncBodyWasUpdated || !FuncData.FuncBodyHash.has_value()) 
				{
					for (auto& Block : Func->Blocks)
					{
						Block->Instructions.erase(
							std::remove_if(Block->Instructions.begin(),Block->Instructions.end(),
							[](const Unique_ptr<IRInstruction>& o) { return o.get() == nullptr || o->Type == IRInstructionType::None; })
							,Block->Instructions.end());
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


				if (Hashes.HasValue(Hash))
				{
					const auto& SameFunc = Hashes.GetValue(Hash);
					
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
										if (Ins->Target().identifier == Func->identifier) {
											Ins->Target().identifier = SameFunc->identifier;
											UpdatedCodeFor(Func.get());
										}
									}

									if (IsOperatorValueInTarget(Ins->Type))
									{
										auto Op = Ins->Target();
										if (Op.Type == IROperatorType::Get_Func_Pointer)
										{
											if (Op.identifier == Func->identifier) {
												Op.identifier = SameFunc->identifier;
												UpdatedCodeFor(Func.get());
											}
										}
									}

									if (IsOperatorValueInInput(Ins->Type))
									{
										auto Op = Ins->Input();
										if (Op.Type == IROperatorType::Get_Func_Pointer)
										{
											if (Op.identifier == Func->identifier) {
												Op.identifier = SameFunc->identifier;
												UpdatedCodeFor(Func.get());
											}
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
					Hashes.AddValue(Hash, Func.get());
				}
			}
		}


	} while (_UpdatedCode);
	



	if (didonepass)
	{
		for (auto& Item : Funcs) 
		{
			if (Item.second.SsA.has_value()) {
				UndoSSA(Item.first, Item.second.SsA.value());
			}
		}
	}


	if (Optimization_RemoveUnusePars)
	{//remove unneed funcions
		struct Empty{};
		UnorderedMap<IRidentifierID, Empty> funcstokeep;

		for (auto& Func : Input->Funcs)
		{
			if (Func->MustBePrerived)
			{
				funcstokeep.AddValue(Func->identifier, Empty());
			}
		}

		size_t oldfuncstokeep = funcstokeep.size();
		do
		{
			oldfuncstokeep = funcstokeep.size();
			
			for (auto& Func : Input->Funcs)
			{
				if (funcstokeep.HasValue(Func->identifier)) 
				{
					for (auto& Block : Func->Blocks)
					{
						for (auto& Ins : Block->Instructions)
						{
							Optional<IRidentifierID> v;
							if (Ins->Type == IRInstructionType::Call)
							{
								v = Ins->Target().identifier;
							}
							else if (IsOperatorValueInTarget(Ins->Type))
							{
								auto Op = Ins->Target();
								if (Op.Type == IROperatorType::Get_Func_Pointer)
								{
									v = Op.identifier;
								}
							}
							else if (IsOperatorValueInInput(Ins->Type))
							{
								auto Op = Ins->Input();
								if (Op.Type == IROperatorType::Get_Func_Pointer)
								{
									v = Op.identifier;
								}
							}

							if (v.has_value())
							{
								auto val = v.value();
								
								if (!funcstokeep.HasValue(val))
								{
									funcstokeep.AddValue(val, Empty());
								}
							}

						}
					}
				}
			}

		} while (oldfuncstokeep != funcstokeep.size());

		for (auto& Func : Input->Funcs)
		{
			Input->Funcs.erase(
				std::remove_if(Input->Funcs.begin(), Input->Funcs.end(),
					[&funcstokeep](const Unique_ptr<IRFunc>& o) { return !funcstokeep.HasValue(o->identifier); }),
				Input->Funcs.end());
		}
	}
}
void IROptimizer::ReplaceAllTypesTo(IRidentifierID typetolookfor, IRidentifierID newtype)
{
	auto TestType = [typetolookfor,newtype](IRType& Update)
	{
			if (Update._symbol.ID == typetolookfor)
			{
				Update._symbol.ID = newtype;
			}
	};

	for (auto& OtherItem : Input->_Symbols)
	{
		if (OtherItem->SymType == IRSymbolType::Struct)
		{
			IRStruct* V = OtherItem->Get_ExAs<IRStruct>();
			for (auto& Item : V->Fields)
			{
				TestType(Item.Type);
			}
		}
		else if (OtherItem->SymType == IRSymbolType::FuncPtr)
		{
			IRFuncPtr* V = OtherItem->Get_ExAs<IRFuncPtr>();

			TestType(V->Ret);

			for (auto& Item : V->Pars)
			{
				TestType(Item);
			}
		}
		else if (OtherItem->SymType == IRSymbolType::StaticArray)
		{
			IRStaticArray* V = OtherItem->Get_ExAs<IRStaticArray>();

			TestType(V->Type);
		}
		else if (OtherItem->SymType == IRSymbolType::ThreadLocalVarable
			|| OtherItem->SymType == IRSymbolType::StaticVarable)
		{
			IRBufferData* V = OtherItem->Get_ExAs<IRBufferData>();
			
			TestType(OtherItem->Type);
		}
		else
		{
			UCodeLangUnreachable();
		}
	}
	for (auto& OtherItem : Input->Funcs)
	{
		for (auto& Item : OtherItem->Pars)
		{
			TestType(Item.type);
		}
		for (auto& Block : OtherItem->Blocks)
		{
			for (auto& Item : Block->Instructions)
			{
				TestType(Item->ObjectType);
			}
		}
	}
}

void IROptimizer::UpdateOptimizationList()
{
	auto& Stettings = *_Settings;


	bool IgnoredebugFlag = Stettings.HasArg("IgnoreDebug");
	bool ForDebuging = (OptimizationFlags_t)Stettings._Flags & (OptimizationFlags_t)OptimizationFlags::Debug;
	bool ForSize =  (OptimizationFlags_t)Stettings._Flags & (OptimizationFlags_t)OptimizationFlags::ForSize;
	bool ForSpeed = (OptimizationFlags_t)Stettings._Flags & (OptimizationFlags_t)OptimizationFlags::ForSpeed;

	if (ForSpeed)
	{
		ForSize = true;
	}
	ResetOptimizations();
	
	#if !IsOptimizerStable
	return;
	#endif
	
	bool isdebuging = ForDebuging;
	if (IgnoredebugFlag)
	{
		isdebuging = false;
	}

	if (ForSize)
	{
		if (isdebuging == false)
		{
			Optimization_RemoveUnsedNamedVarables = true;

			Optimization_RemoveUnusePars = true;
			Optimization_RemoveFuncsWithSameBody = true;
			Optimization_LowerToBodysToCFunctions = true;
			Optimization_InlineConditionalJump = true;
			Optimization_RemoveUnreachable = true;
			Optimization_RemoveUnneedMeallocAndFree = true;

			Optimization_FloatFastMath = Stettings.HasFlagArg("ffast-math");
		}
		Optimization_StrengthReduction = true;
		Optimization_RemoveIdenticalTypes = true;
		Optimization_RemoveUnsedVarables = true;
			
		
		Optimization_ConstantFoldVarables = true;
		Optimization_IndirectMemeberToDirectMemeber = true;
		Optimization_ConstantFuncPtrToDirectCall = true;
	}
	if (ForSpeed)
	{
		if (isdebuging == false)
		{
			//Optimization_DestructureStructMembers = true;
			Optimization_ReorderFunctionsInToHotSpots = true;
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

	if (!isdebuging)
	{
		Input->_Debug.Symbols.clear();

		for (auto& Item2 : Input->_StaticInit.Blocks)
		{
			Item2->DebugInfo.DebugInfo.clear();
		}
		for (auto& Item2 : Input->_StaticdeInit.Blocks)
		{
			Item2->DebugInfo.DebugInfo.clear();
		}
		for (auto& Item2 : Input->_threadInit.Blocks)
		{
			Item2->DebugInfo.DebugInfo.clear();
		}
		for (auto& Item2 : Input->_threaddeInit.Blocks)
		{
			Item2->DebugInfo.DebugInfo.clear();
		}

		for (auto& Item : Input->Funcs)
		{
			for (auto& Item2 : Item->Blocks)
			{
				Item2->DebugInfo.DebugInfo.clear();
			}
		}
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
	if (!Funcs.HasValue(Func))
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
		lookatblock = Block.get();
		IRData.clear();

		for (auto& Ins : Block->Instructions)
		{
			IRData.AddValue(Ins.get(), {});
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

					#define	ConstantBinaryFoldfloat(bits) \
					switch (Ins->Type) \
					{\
					case IRInstructionType::Add:Ins->Target().Value.Asfloat##bits = Ins->A.Value.AsInt##bits  + Ins->B.Value.Asfloat##bits ;break;\
					case IRInstructionType::Sub:Ins->Target().Value.Asfloat##bits = Ins->A.Value.AsInt##bits  - Ins->B.Value.Asfloat##bits ;break;\
					case IRInstructionType::UMult:Ins->Target().Value.Asfloat##bits = Ins->A.Value.AsUInt##bits  * Ins->B.Value.Asfloat##bits ;break;\
					case IRInstructionType::SMult:Ins->Target().Value.Asfloat##bits  = Ins->A.Value.AsInt##bits  * Ins->B.Value.Asfloat##bits ;break;\
					case IRInstructionType::UDiv:Ins->Target().Value.Asfloat##bits  = Ins->A.Value.AsUInt##bits  / Ins->B.Value.Asfloat##bits ;break;\
					case IRInstructionType::SDiv:Ins->Target().Value.Asfloat##bits  = Ins->A.Value.AsInt##bits  / Ins->B.Value.Asfloat##bits ;break;\
					case IRInstructionType::EqualTo:Ins->Target().Value.Asfloat##bits  = Ins->A.Value.AsInt##bits  == Ins->B.Value.Asfloat##bits ;break;\
					case IRInstructionType::NotEqualTo:Ins->Target().Value.Asfloat##bits  = Ins->A.Value.AsInt##bits  != Ins->B.Value.Asfloat##bits ;break;\
					case IRInstructionType::SGreaterThan:Ins->Target().Value.Asfloat##bits = Ins->A.Value.AsInt##bits > Ins->B.Value.Asfloat##bits; break; \
					case IRInstructionType::SLessThan:Ins->Target().Value.Asfloat##bits = Ins->A.Value.AsInt##bits < Ins->B.Value.Asfloat##bits; break; \
					case IRInstructionType::SGreaterThanOrEqual:Ins->Target().Value.Asfloat##bits = Ins->A.Value.AsInt##bits >= Ins->B.Value.Asfloat##bits; break; \
					case IRInstructionType::SLessThanOrEqual:Ins->Target().Value.Asfloat##bits = Ins->A.Value.AsInt##bits <= Ins->B.Value.Asfloat##bits; break; \
					case IRInstructionType::UGreaterThan:Ins->Target().Value.Asfloat##bits = Ins->A.Value.AsUInt##bits > Ins->B.Value.Asfloat##bits; break; \
					case IRInstructionType::ULessThan:Ins->Target().Value.Asfloat##bits = Ins->A.Value.AsUInt##bits < Ins->B.Value.Asfloat##bits; break; \
					case IRInstructionType::UGreaterThanOrEqual:Ins->Target().Value.Asfloat##bits = Ins->A.Value.AsUInt##bits >= Ins->B.Value.Asfloat##bits; break; \
					case IRInstructionType::ULessThanOrEqual:Ins->Target().Value.Asfloat##bits = Ins->A.Value.AsUInt##bits <= Ins->B.Value.Asfloat##bits; break; \
					case IRInstructionType::Logical_And:Ins->Target().Value.Asfloat##bits  = Ins->A.Value.AsInt##bits && Ins->B.Value.Asfloat##bits ;break;\
					case IRInstructionType::Logical_Or:Ins->Target().Value.Asfloat##bits  = Ins->A.Value.AsInt##bits || Ins->B.Value.Asfloat##bits ;break;\
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
								//TODO add check  for floats
							}
							else 
							{
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
								ConstantBinaryFoldfloat(32);							
							}
						}
						else if (Ins->ObjectType.IsType(IRTypes::f64))
						{
							if (Optimization_FloatFastMath)
							{
								ConstantBinaryFoldfloat(64);
							}
						}
						else
						{
							UCodeLangUnreachable();
						}
					}
				}
			

				if (Optimization_StrengthReduction) 
				{
					if (
						(Ins->Type == IRInstructionType::SMult
							|| Ins->Type == IRInstructionType::UMult
							)
						&& (Ins->A.Type == IROperatorType::Value
						|| Ins->B.Type == IROperatorType::Value)
						)
					{
						// A * 1 => A
						// A * 2 => A + A or A * 2 => A << 1

						auto& Op = Ins->A.Type == IROperatorType::Value ? Ins->A : Ins->B;
						auto& Other = Ins->A.Type != IROperatorType::Value ? Ins->A : Ins->B;

						enum class mode
						{
							none,
							Mult0,
							Mult1,
							Mult2,
							Mult4,
							Mult8,
							Mult16,
							Mult32,
							Mult64,
						};
						mode ok = mode::none;
						if (Ins->ObjectType.IsType(IRTypes::i8))
						{
							if (Op.Value.AsInt8 == 0)
							{
								ok = mode::Mult0;
							}
							else if (Op.Value.AsInt8 == 1)
							{
								ok = mode::Mult1;
							}
							else if (Op.Value.AsInt8 == 2)
							{
								ok = mode::Mult2;
							}
							else if (Op.Value.AsInt8 == 4)
							{
								ok = mode::Mult4;
							}
							else if (Op.Value.AsInt8 == 8)
							{
								ok = mode::Mult8;
							}
							else if (Op.Value.AsInt8 == 16)
							{
								ok = mode::Mult16;
							}
							else if (Op.Value.AsInt8 == 32)
							{
								ok = mode::Mult32;
							}
							else if (Op.Value.AsInt8 == 64)
							{
								ok = mode::Mult64;
							}
						}
						else if (Ins->ObjectType.IsType(IRTypes::i16))
						{
							if (Op.Value.AsInt16 == 0)
							{
								ok = mode::Mult0;
							}
							else if (Op.Value.AsInt16 == 1)
							{
								ok = mode::Mult1;
							}
							else if (Op.Value.AsInt16 == 2)
							{
								ok = mode::Mult2;
							}
							else if (Op.Value.AsInt16 == 4)
							{
								ok = mode::Mult4;
							}
							else if (Op.Value.AsInt16 == 8)
							{
								ok = mode::Mult8;
							}
							else if (Op.Value.AsInt16 == 16)
							{
								ok = mode::Mult16;
							}
							else if (Op.Value.AsInt16 == 32)
							{
								ok = mode::Mult32;
							}
							else if (Op.Value.AsInt16 == 64)
							{
								ok = mode::Mult64;
							}
						}
						else if (Ins->ObjectType.IsType(IRTypes::i32))
						{
							if (Op.Value.AsInt32 == 0)
							{
								ok = mode::Mult0;
							}
							else if (Op.Value.AsInt32 == 1)
							{
								ok = mode::Mult1;
							}
							else if (Op.Value.AsInt32 == 2)
							{
								ok = mode::Mult2;
							}
							else if (Op.Value.AsInt32 == 4)
							{
								ok = mode::Mult4;
							}
							else if (Op.Value.AsInt32 == 8)
							{
								ok = mode::Mult8;
							}
							else if (Op.Value.AsInt32 == 16)
							{
								ok = mode::Mult16;
							}
							else if (Op.Value.AsInt32 == 32)
							{
								ok = mode::Mult32;
							}
							else if (Op.Value.AsInt32 == 64)
							{
								ok = mode::Mult64;
							}
						}
						else if (Ins->ObjectType.IsType(IRTypes::i64))
						{
							if (Op.Value.AsInt64 == 0)
							{
								ok = mode::Mult0;
							}
							else if (Op.Value.AsInt64 == 1)
							{
								ok = mode::Mult1;
							}
							else if (Op.Value.AsInt64 == 2)
							{
								ok = mode::Mult2;
							}
							else if (Op.Value.AsInt64 == 4)
							{
								ok = mode::Mult4;
							}
							else if (Op.Value.AsInt64 == 8)
							{
								ok = mode::Mult8;
							}
							else if (Op.Value.AsInt64 == 16)
							{
								ok = mode::Mult16;
							}
							else if (Op.Value.AsInt64 == 32)
							{
								ok = mode::Mult32;
							}
							else if (Op.Value.AsInt64 == 64)
							{
								ok = mode::Mult64;
							}
						}
						else if (Ins->ObjectType.IsType(IRTypes::f32))
						{
							if (Optimization_FloatFastMath)
							{
								if (Op.Value.AsInt32 == 0)
								{
									ok = mode::Mult0;
								}
								else if (Op.Value.Asfloat32 == 1)
								{
									ok = mode::Mult1;
								}
								else if (Op.Value.Asfloat32 == 2)
								{
									ok = mode::Mult2;
								}
							}
						}
						else if (Ins->ObjectType.IsType(IRTypes::f64))
						{
							if (Optimization_FloatFastMath)
							{
								if (Op.Value.AsInt8 == 0)
								{
									ok = mode::Mult0;
								}
								else if (Op.Value.Asfloat64 == 1)
								{
									ok = mode::Mult1;
								}
								else if (Op.Value.Asfloat64 == 2)
								{
									ok = mode::Mult2;
								}
							}
						}

						if (ok == mode::Mult1)
						{
							Op = IROperator();
							Ins->Target() = Other;
							Ins->Type = IRInstructionType::Load;
							UpdatedCode();
						}
						else if (ok == mode::Mult2)
						{
							bool DoAdd = Ins->ObjectType.IsType(IRTypes::f64) || Ins->ObjectType.IsType(IRTypes::f32);
							if (DoAdd) 
							{
								Op = IROperator();
								Ins->Target() = Other;
								Ins->Input() = Other;
								Ins->Type = IRInstructionType::Add;
							}
							else
							{
								Op = IROperator();
								Ins->Target() = Other;
								Ins->Input() = AnyInt64(1);
								Ins->Type = IRInstructionType::BitWise_ShiftL;
							}
							UpdatedCode();
						}
						else if (ok == mode::Mult4)
						{
							Op = IROperator();
							Ins->Target() = Other;
							Ins->Input() = AnyInt64(2);
							Ins->Type = IRInstructionType::BitWise_ShiftL;
							UpdatedCode();
						}
						else if (ok == mode::Mult8)
						{
							Op = IROperator();
							Ins->Target() = Other;
							Ins->Input() = AnyInt64(3);
							Ins->Type = IRInstructionType::BitWise_ShiftL;
							UpdatedCode();
						}
						else if (ok == mode::Mult16)
						{
							Op = IROperator();
							Ins->Target() = Other;
							Ins->Input() = AnyInt64(4);
							Ins->Type = IRInstructionType::BitWise_ShiftL;
							UpdatedCode();
						}
						else if (ok == mode::Mult32)
						{
							Op = IROperator();
							Ins->Target() = Other;
							Ins->Input() = AnyInt64(5);
							Ins->Type = IRInstructionType::BitWise_ShiftL;
							UpdatedCode();
						}
						else if (ok == mode::Mult64)
						{
							Op = IROperator();
							Ins->Target() = Other;
							Ins->Input() = AnyInt64(6);
							Ins->Type = IRInstructionType::BitWise_ShiftL;
							UpdatedCode();
						}
						else if (ok == mode::Mult0)
						{
							Op = IROperator();
							
							if (Ins->ObjectType.IsType(IRTypes::f64))
							{
								Ins->Target() = AnyInt64((float64)0);
							}
							else if (Ins->ObjectType.IsType(IRTypes::f32))
							{
								Ins->Target() = AnyInt64((float32)0);
							}
							else
							{
								Ins->Target() = AnyInt64(0);
							}

							Ins->Type = IRInstructionType::Load;
							UpdatedCode();
						}
						else if (ok == mode::none)
						{

						}
						else
						{
							UCodeLangUnreachable();
						}
					}
					
					
					if (
						(Ins->Type == IRInstructionType::SDiv
							|| Ins->Type == IRInstructionType::UDiv
							)
						&& (Ins->B.Type == IROperatorType::Value)
						)
					{
						enum class mode
						{
							none,
							Div0,
							Div1,
							Div2,
							Div4,
							Div8,
							Div16,
							Div32,
							Div64,
						};
						auto& Op = Ins->B;

						mode ok = mode::none;
						if (Ins->ObjectType.IsType(IRTypes::i32))
						{
							if (Op.Value.AsInt32 == 0)
							{
								ok = mode::Div0;
							}
							else if (Op.Value.AsInt32 == 1)
							{
								ok = mode::Div1;
							}
							else if (Op.Value.AsInt32 == 2)
							{
								ok = mode::Div2;
							}
							else if (Op.Value.AsInt32 == 4)
							{
								ok = mode::Div4;
							}
							else if (Op.Value.AsInt32 == 8)
							{
								ok = mode::Div8;
							}
							else if (Op.Value.AsInt32 == 16)
							{
								ok = mode::Div16;
							}
							else if (Op.Value.AsInt32 == 32)
							{
								ok = mode::Div32;
							}
							else if (Op.Value.AsInt32 == 64)
							{
								ok = mode::Div64;
							}
						}
						else if (Ins->ObjectType.IsType(IRTypes::i16))
						{
							if (Op.Value.AsInt16 == 0)
							{
								ok = mode::Div0;
							}
							else if (Op.Value.AsInt16 == 1)
							{
								ok = mode::Div1;
							}
							else if (Op.Value.AsInt16 == 2)
							{
								ok = mode::Div2;
							}
							else if (Op.Value.AsInt16 == 4)
							{
								ok = mode::Div4;
							}
							else if (Op.Value.AsInt16 == 8)
							{
								ok = mode::Div8;
							}
							else if (Op.Value.AsInt16 == 16)
							{
								ok = mode::Div16;
							}
							else if (Op.Value.AsInt16 == 32)
							{
								ok = mode::Div32;
							}
							else if (Op.Value.AsInt16 == 64)
							{
								ok = mode::Div64;
							}
						}
						else if (Ins->ObjectType.IsType(IRTypes::i8))
						{
							if (Op.Value.AsInt8 == 0)
							{
								ok = mode::Div0;
							}
							else if (Op.Value.AsInt8 == 1)
							{
								ok = mode::Div1;
							}
							else if (Op.Value.AsInt8 == 2)
							{
								ok = mode::Div2;
							}
							else if (Op.Value.AsInt8 == 4)
							{
								ok = mode::Div4;
							}
							else if (Op.Value.AsInt8 == 8)
							{
								ok = mode::Div8;
							}
							else if (Op.Value.AsInt8 == 16)
							{
								ok = mode::Div16;
							}
							else if (Op.Value.AsInt8 == 32)
							{
								ok = mode::Div32;
							}
							else if (Op.Value.AsInt8 == 64)
							{
								ok = mode::Div64;
							}
						}
						else if (Ins->ObjectType.IsType(IRTypes::i64))
						{
							if (Op.Value.AsInt64 == 0)
							{
								ok = mode::Div0;
							}
							else if (Op.Value.AsInt64 == 1)
							{
								ok = mode::Div1;
							}
							else if (Op.Value.AsInt64 == 2)
							{
								ok = mode::Div2;
							}
							else if (Op.Value.AsInt64 == 4)
							{
								ok = mode::Div4;
							}
							else if (Op.Value.AsInt64 == 8)
							{
								ok = mode::Div8;
							}
							else if (Op.Value.AsInt64 == 16)
							{
								ok = mode::Div16;
							}
							else if (Op.Value.AsInt64 == 32)
							{
								ok = mode::Div32;
							}
							else if (Op.Value.AsInt64 == 64)
							{
								ok = mode::Div64;
							}
						}
						else if (Ins->ObjectType.IsType(IRTypes::f32))
						{
							if (Optimization_FloatFastMath) {
								if (Op.Value.Asfloat32 == 0)
								{
									ok = mode::Div0;
								}
								else if (Op.Value.Asfloat32 == 1)
								{
									ok = mode::Div1;
								}
								else if (Op.Value.Asfloat32 == 2)
								{
									ok = mode::Div2;
								}
								else if (Op.Value.Asfloat32 == 4)
								{
									ok = mode::Div4;
								}
								else if (Op.Value.Asfloat32 == 8)
								{
									ok = mode::Div8;
								}
								else if (Op.Value.Asfloat32 == 16)
								{
									ok = mode::Div16;
								}
								else if (Op.Value.Asfloat32 == 32)
								{
									ok = mode::Div32;
								}
								else if (Op.Value.Asfloat32 == 64)
								{
									ok = mode::Div64;
								}
							}
						}
						else if (Ins->ObjectType.IsType(IRTypes::f64))
						{
							if (Optimization_FloatFastMath) {
								if (Op.Value.Asfloat64== 0)
								{
									ok = mode::Div0;
								}
								else if (Op.Value.Asfloat64 == 1)
								{
									ok = mode::Div1;
								}
								else if (Op.Value.Asfloat64 == 2)
								{
									ok = mode::Div2;
								}
								else if (Op.Value.Asfloat64 == 4)
								{
									ok = mode::Div4;
								}
								else if (Op.Value.Asfloat64 == 8)
								{
									ok = mode::Div8;
								}
								else if (Op.Value.Asfloat64 == 16)
								{
									ok = mode::Div16;
								}
								else if (Op.Value.Asfloat64 == 32)
								{
									ok = mode::Div32;
								}
								else if (Op.Value.Asfloat64 == 64)
								{
									ok = mode::Div64;
								}
							}
							}

						if (ok == mode::none)
						{

						}
						else if (ok == mode::Div0)
						{
							//Block->Instructions.insert(Block->Instructions.begin() + i, std::make_unique<IRInstruction>(IRInstructionType::Unreachable));
							//UpdatedCode();
						}
						else if (ok == mode::Div1)
						{
							Op = IROperator();
							Ins->Target() = Ins->A;
							Ins->Type = IRInstructionType::Load;
							UpdatedCode();
						}
						else if (ok == mode::Div2)
						{
							Op = IROperator();
							Ins->Target() = Ins->A;
							Ins->Input() = AnyInt64(1);
							Ins->Type = IRInstructionType::BitWise_ShiftR;
							UpdatedCode();
						}
						else if (ok == mode::Div4)
						{
							Op = IROperator();
							Ins->Target() = Ins->A;
							Ins->Input() = AnyInt64(2);
							Ins->Type = IRInstructionType::BitWise_ShiftR;
							UpdatedCode();
						}
						else if (ok == mode::Div8)
						{
							Op = IROperator();
							Ins->Target() = Ins->A;
							Ins->Input() = AnyInt64(3);
							Ins->Type = IRInstructionType::BitWise_ShiftR;
							UpdatedCode();
						}
						else if (ok == mode::Div16)
						{
							Op = IROperator();
							Ins->Target() = Ins->A;
							Ins->Input() = AnyInt64(4);
							Ins->Type = IRInstructionType::BitWise_ShiftR;
							UpdatedCode();
						}
						else if (ok == mode::Div32)
						{
							Op = IROperator();
							Ins->Target() = Ins->A;
							Ins->Input() = AnyInt64(5);
							Ins->Type = IRInstructionType::BitWise_ShiftR;
							UpdatedCode();
						}
						else if (ok == mode::Div64)
						{
							Op = IROperator();
							Ins->Target() = Ins->A;
							Ins->Input() = AnyInt64(6);
							Ins->Type = IRInstructionType::BitWise_ShiftR;
							UpdatedCode();
						}
						else 
						{
							UCodeLangUnreachable();
						}
					}

					if (
						(Ins->Type == IRInstructionType::SDiv
							|| Ins->Type == IRInstructionType::UDiv
							)
						&& (Ins->B == Ins->A)
						)
					{
						if (Ins->ObjectType.IsType(IRTypes::f64))
						{
							if (Optimization_FloatFastMath) 
							{
								Ins->Target() = AnyInt64((float64)1);
								Ins->Type = IRInstructionType::Load;
								UpdatedCode();
							}

						}
						else if (Ins->ObjectType.IsType(IRTypes::f32))
						{
							if (Optimization_FloatFastMath) {
								Ins->Target() = AnyInt64((float32)1);
								Ins->Type = IRInstructionType::Load;
								UpdatedCode();
							}

						}
						else 
						{
							Ins->Target() = AnyInt64(1);
							Ins->Type = IRInstructionType::Load;
							UpdatedCode();
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
						Ins->Target() = target.identifier;

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
						auto func = Ins->Target().identifier;
						Ins->Type = IRInstructionType::Call;
						Ins->Target().identifier = func;
						
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
									if (op.Type == IROperatorType::IRInstruction)
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
			else if (Ins->Type == IRInstructionType::JumpBlock)
			{

			}
			else if (Ins->Type == IRInstructionType::JumpBlockIf)
			{

			}
			else if (Ins->Type == IRInstructionType::f32Toi32)
			{
			}
			else if (Ins->Type == IRInstructionType::f64Toi64)
			{
			}
			else if (Ins->Type == IRInstructionType::SIntToSInt8)
			{
			}
			else if (Ins->Type == IRInstructionType::SIntToSInt16)
			{
			}
			else if (Ins->Type == IRInstructionType::SIntToSInt32)
			{
			}
			else if (Ins->Type == IRInstructionType::SIntToSInt64)
			{
			}
			else if (Ins->Type == IRInstructionType::UIntToUInt8)
			{
			}
			else if (Ins->Type == IRInstructionType::UIntToUInt16)
			{
			}
			else if (Ins->Type == IRInstructionType::UIntToUInt32)
			{
			}
			else if (Ins->Type == IRInstructionType::UIntToUInt64)
			{
			}
			else if (Ins->Type == IRInstructionType::Si32Tof32)
			{

			}
			else if (Ins->Type == IRInstructionType::Si64Tof64)
			{
			}
			else if (Ins->Type == IRInstructionType::SSA_Reassign)
			{
				ConstantFoldOperator(*Ins, Ins->Target(), ReadOrWrite::Write);
				ConstantFoldOperator(*Ins, Ins->Input(), ReadOrWrite::Read);


				if (Optimization_RemoveUnsedVarables)
				{
					if (Ins->Input().Type == IROperatorType::IRInstruction) {
						bool wasused = false;
						for (size_t i2 = 0; i2 < i; i2++)
						{
							auto Ins2 = Block->Instructions[i2].get();

							if (IsOperatorValueInTarget(Ins2->Type))
							{
								if (Ins2->Target() == Ins->Input())
								{
									wasused = true;
									break;

								}
							}

							if (IsOperatorValueInInput(Ins2->Type))
							{
								if (Ins2->Input() == Ins->Input())
								{
									wasused = true;
									break;

								}
							}
						}

						if (!wasused)
						{
							auto& Op = Ins->Input();
							if (Op.Pointer->Type == IRInstructionType::LoadNone)
							{
								Op.Pointer->SetAsNone();
								Ins->Type = IRInstructionType::Load;
								UpdatedCode();
							}
							else if (Op.Pointer->Type == IRInstructionType::Member_Access)
							{

							}
							else
							{
								#if RunlogIRState
								{
									auto S = Input->ToString();
									std::cout << "-----" << std::endl;
									std::cout << S;
								}
								#endif
								UCodeLangUnreachable();
							}
						}
					}
				}
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
				if (Ins->Type == IRInstructionType::Load) //removing dead Instructions
				{
					auto& Data = Get_IRData(Ins.get());
					if (!Data.IsReferenced)
					{
						Ins->SetAsNone(); UpdatedCode();
					}
				}

				if (Ins->Type == IRInstructionType::LoadNone)//removing dead Instructions
				{
					auto& Data = Get_IRData(Ins.get());
					if (!Data.IsReferenced)
					{
						Ins->SetAsNone(); UpdatedCode();
					}
				}

				if (Ins->Type == IRInstructionType::Reassign)//removing dead Instructions
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
				CopyFunctionWithoutUnusedParAndUpdateOtherCalls(Par, Func, i);
				UpdatedCode();
				break;
			}
		}
	}

}
void IROptimizer::DoDestructurStructMembers(UCodeLang::Unique_ptr<UCodeLang::IRInstruction>& Ins, UCodeLang::IRStruct* structtype, UCodeLang::IRFunc* Func, UCodeLang::Unique_ptr<UCodeLang::IRBlock>& Block, const size_t& i)
{
	#if RunlogIRState
	std::cout << Input->ToString() << '\n';
	#endif
	Vector<IRInstruction*> LoadFields;
	auto oldtype = Ins->ObjectType;
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
		for (size_t i = 0; i < Block->Instructions.size(); i++)
		{
			auto& Insv = Block->Instructions[i];

			auto OnOp = [&Block,&LoadFields,&Ins, &Insv, oldtype,&i](IROperator& op)
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
					else if (target == Ins.get())
					{
						Unique_ptr<IRInstruction> base = std::make_unique< IRInstruction>();

						IRInstruction* Basep = base.get();

						base->Type = IRInstructionType::LoadNone;
						base->ObjectType = oldtype;
						Block->Instructions.insert(Block->Instructions.begin() + i, std::move(base));

						i++;

						UCodeLangToDo();

						op = Basep;
					}
					else
					{

					}
				}
			};


			if (IsLoadValueOnlyInTarget(Insv->Type))
			{
				OnOp(Insv->Target());
			}
			auto& V = Block->Instructions[i];//Instructions may realloc
			if (IsLoadValueOnInput(V->Type))
			{
				OnOp(V->Input());
			}
		}
	}
	//remove member
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

	#if RunlogIRState
	std::cout << Input->ToString() << '\n';
	#endif
}
void IROptimizer::CopyFunctionWithoutUnusedParAndUpdateOtherCalls(UCodeLang::IRPar& Par, UCodeLang::IRFunc* Func, const size_t& i)
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

#if RunlogIRState
	_TypeFixer.FixTypes(Input);
	std::cout << Input->ToString() << '\n';
#endif
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
				if (Ins->Target().identifier == Func->identifier)
				{
					Ins->Target().identifier = NewFunc.identifier;

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
	if (!Funcs.HasValue(Func)) { return; }
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
			auto* FuncToCall = Input->GetFunc(Ins->Target().identifier);
			if (FuncToCall)
			{
				IROptimizationFuncData& FuncData = Funcs.GetOrAdd(FuncToCall, {});

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
		if (Ptr->Type == IRInstructionType::Load
			|| Ptr->Type == IRInstructionType::SSA_Reassign)
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
			else if (Ptr->Target().Type == IROperatorType::IRInstruction)
			{
				//if (Ptr->Target().Pointer->Type == IRInstructionType::SSA_Reassign) {
				Value = Ptr->Target();
				UpdatedCode();
				//}
			}
		}
		else if (Ptr->Type == IRInstructionType::Member_Access_Dereference)
		{
			ConstantFoldOperator(*Ptr, Ptr->Target(), OpType);
		}
		else if (Ptr->Type == IRInstructionType::Member_Access)
		{
			ConstantFoldOperator(*Ptr, Ptr->Target(), OpType);
		}


		if (!IRData.HasValue(Ptr))
		{
			IRData.AddValue(Ptr, {});
		}
		IRData.GetValue(Ptr).IsReferenced = true;
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


	if (Value.Type == IROperatorType::IRInstruction)
	{
		auto ins = Value.Pointer;
		if (ins->Type == IRInstructionType::Member_Access_Dereference)
		{
			auto& op = ins->Target();
			if (op.Type == IROperatorType::Get_PointerOf_IRInstruction)
			{
				int a = 0;
				Optional<size_t> newinsIndex;

				for (size_t i = 0; i < lookatblock->Instructions.size(); i++)
				{
					auto& Item = lookatblock->Instructions[i];
					if (Item.get() == ins)
					{
						newinsIndex = i;
						break;
					}
				}

				auto tep = std::make_unique<IRInstruction>();
				IRInstruction* newIns = tep.get();
				lookatblock->Instructions.insert(lookatblock->Instructions.begin() + newinsIndex.value(), std::move(tep));


				newIns->Type = IRInstructionType::Member_Access;
				newIns->A = IROperator(op.Pointer);
				newIns->B = ins->Input();

				Value.Pointer = newIns;
				UpdatedCode();
			}
		}
	}
	if (Value.Type == IROperatorType::DereferenceOf_IRInstruction)
	{
		auto ins = Value.Pointer;
		if (ins->Type == IRInstructionType::Load)
		{
			auto& op = ins->Target();
			if (op.Type == IROperatorType::Get_PointerOf_IRInstruction)
			{
				Value = ins->Input();
				UpdatedCode();
			}
		}
	}
}
void IROptimizer::ToSSA(IRFunc* Func, SSAState& state)
{
	if (Func->Blocks.size() == 0) { return; }
	UCodeLangAssert(Func->Blocks.size());

	ControlFlowToBaseBasicBlocks(Func, state);

#if RunlogIRState 
	{
		auto S = Input->ToString();
		std::cout << "-----" << std::endl;
		std::cout << S;
	}
#endif
	
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
				Ins->Type = IRInstructionType::SSA_Reassign;
				std::swap(Ins->Target(), Ins->Input());

				state.Map.AddValueOrOverWrite(ToUpdate,Ins.get());
			}
			else
			{
				if (IsOperatorValueInTarget(Ins->Type))
				{
					if (state.Map.HasValue(Ins->Target()))
					{
						state.Updated.AddValueOrOverWrite(&Ins->Target(), Ins->Target());
						Ins->Target() = IROperator(state.Map.GetValue(Ins->Target()));
					}
				}	
				
				if (IsOperatorValueInInput(Ins->Type))
				{
					if (state.Map.HasValue(Ins->Input()))
					{
						state.Updated.AddValueOrOverWrite(&Ins->Input(), Ins->Input());
						Ins->Target() = IROperator(state.Map.GetValue(Ins->Input()));
					}
				}
			}
		}
	}
	

	#if RunlogIRState 
	{
		auto S = Input->ToString();
		std::cout << "-----" << std::endl;
		std::cout << S;
	}
	#endif
}
void IROptimizer::UndoSSA(IRFunc* Func, const SSAState& state)
{
	if (Func->Blocks.size()) {
#if RunlogIRState 
		{
			auto S = Input->ToString();
			std::cout << "-----" << std::endl;
			std::cout << S;
		}
#endif

		auto& Block = Func->Blocks.front();
		struct Empty {};

		UnorderedMap<IRInstruction*, Empty> SSAs;
		for (auto& Ins : Block->Instructions)
		{
			if (Ins->Type == IRInstructionType::SSA_Reassign)
			{
				std::swap(Ins->Target(), Ins->Input());
				Ins->Type = IRInstructionType::Reassign;
				SSAs.AddValue(Ins.get(), Empty());
			}
		}

		for (auto& Ins : Block->Instructions)
		{
			if (IsOperatorValueInTarget(Ins->Type))
			{
				auto& Op = Ins->Target();
				if (Op.Type == IROperatorType::IRInstruction)
				{
					if (SSAs.HasValue(Op.Pointer))
					{
						Op = Op.Pointer->Input();
					}
				}
			}
			if (IsOperatorValueInInput(Ins->Type))
			{
				auto& Op = Ins->Input();
				if (Op.Type == IROperatorType::IRInstruction)
				{
					if (SSAs.HasValue(Op.Pointer))
					{
						Op = Op.Pointer->Input();
					}
				}
			}
		}
#if RunlogIRState 
		{
			auto S = Input->ToString();
			std::cout << "-----" << std::endl;
			std::cout << S;
		}
#endif
	}
}

void IROptimizer::ControlFlowToBaseBasicBlocks(IRFunc* Func, SSAState& state)
{
	auto Block = Func->Blocks.front().get();
	UnorderedMap<size_t, size_t> IndexToBlock;
	UnorderedMap<size_t, size_t> JumpLToBlock;

	for (size_t i = 0; i < Block->Instructions.size(); i++)
	{
		auto& I = Block->Instructions[i];
		switch (I->Type)
		{
		case IRInstructionType::Jump:
		{
			if (!IndexToBlock.HasValue(I->Target().Value.AsUIntNative))
			{
				IRBlock v;

				Func->Blocks.push_back(std::make_unique<IRBlock>(std::move(v)));
				IndexToBlock.AddValue(I->Target().Value.AsUIntNative, Func->Blocks.size() - 1);


			}
		}
		break;
		case IRInstructionType::ConditionalJump:
			if (!IndexToBlock.HasValue(I->Target().Value.AsUIntNative))
			{
				IRBlock v;

				Func->Blocks.push_back(std::make_unique<IRBlock>(std::move(v)));
				IndexToBlock.AddValue(I->Target().Value.AsUIntNative, Func->Blocks.size() - 1);


			}
			if (!IndexToBlock.HasValue(i + 1))
			{
				IRBlock v;

				Func->Blocks.push_back(std::make_unique<IRBlock>(std::move(v)));
				IndexToBlock.AddValue(i + 1, Func->Blocks.size() - 1);
			}
			break;
		}
	}


	IRBlock* OnBlock = Block;
	auto size = Block->Instructions.size();
	IRInstructionType lasttype = IRInstructionType::None;
	bool addjumpbecausefallthrough = false;

	for (size_t i = 0; i < size; i++)
	{
		auto* Ins = &Block->Instructions[i];

		if ((*Ins)->Type == IRInstructionType::Jump)
		{
			int a = 0;

		}



		if (lasttype == IRInstructionType::ConditionalJump)
		{
			auto newblockindex = IndexToBlock.GetValue(i);


			OnBlock->Instructions.push_back(
				std::move(std::make_unique<IRInstruction>(IRInstructionType::JumpBlock, IROperator(newblockindex))
				));

			OnBlock = Func->Blocks[newblockindex].get();
			//because of resize
			Ins = &Block->Instructions[i];
		}
		else
		{

			if (IndexToBlock.HasValue(i))
			{
				if (Ins->get()->Type != IRInstructionType::ConditionalJump
					&& Ins->get()->Type != IRInstructionType::Jump)
				{
					int a = 0;

					//for fall through
					auto Index = IndexToBlock.GetValue(i);

					OnBlock->Instructions.push_back(
						std::move(std::make_unique<IRInstruction>(IRInstructionType::JumpBlock, IROperator(Index))
						));
					OnBlock = Func->Blocks[Index].get();
					addjumpbecausefallthrough = true;


					//because of resize
					Ins = &Block->Instructions[i];


				}
			}
			else if (IndexToBlock.HasValue(i))
			{
				auto newblockindex = IndexToBlock.GetValue(i);

				if (newblockindex < Func->Blocks.size()) {
					OnBlock = Func->Blocks[newblockindex].get();
				}
				else
				{
					OnBlock = Func->Blocks.front().get();
				}

			}
		}



		lasttype = (*Ins)->Type;

		if ((*Ins)->Type == IRInstructionType::Jump)
		{
			(*Ins)->Type = IRInstructionType::JumpBlock;
			(*Ins)->Target().Value = AnyInt64(IndexToBlock.GetValue((*Ins)->Target().Value.AsUIntNative));
		}
		else if ((*Ins)->Type == IRInstructionType::ConditionalJump)
		{
			(*Ins)->Type = IRInstructionType::JumpBlockIf;
			(*Ins)->Target().Value = AnyInt64(IndexToBlock.GetValue((*Ins)->Target().Value.AsUIntNative));
		}

		OnBlock->Instructions.push_back(std::move((*Ins)));

	}

	for (auto& Item : Func->Blocks)
	{
		Item->DebugInfo.DebugInfo.clear();
		auto& myList = Item->Instructions;
		myList.erase(
			std::remove_if(myList.begin(), myList.end(),
				[](const Unique_ptr<IRInstruction>& o) { return o.get() == nullptr || o->Type == IRInstructionType::None; }),
			myList.end());

	}
}
void IROptimizer::InLineFunc(InLineData& Data)
{
#if RunlogIRState 
	{
		_TypeFixer.FixTypes(Input);
		auto S = Input->ToString();
		std::cout << "-----" << std::endl;
		std::cout << S;
	}
#endif
	IRInstruction* Call = Data.Block->Instructions[Data.CallIndex].get();

	//IRFunc* CalleFunc = Data.Func;
	const IRFunc* FuncToCall = Input->GetFunc(Call->Target().identifier);


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

	UpdatedCodeFor(Data.Func);
	//
#if RunlogIRState 
	_TypeFixer.FixTypes(Input);
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
#endif

}
void IROptimizer::InLineSubOperator(InLineData& Data, IROperator& Op, size_t Offset)
{
	IRInstruction* Call = Data.Block->Instructions[Data.CallIndex + Offset].get();
	const IRFunc* CallFunc = Input->GetFunc(Call->Target().identifier);

#if UCodeLangDebug
	if (CallFunc == nullptr)
	{
		UCodeLangUnreachable();

		auto S2 = Input->ToString();

		std::cout << "-----" << std::endl;
		std::cout << S2;
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

#endif