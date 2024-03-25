#ifndef UCodeLangNoCompiler
#include "UAssembly.hpp"
#include "CompilerTypes.hpp"
#include "UCodeLang/LangCore/DataType/UnorderedMap.hpp"
#include "UCodeLang/Compilation/Helpers/NameDecoratior.hpp"

#include <fstream>
#include "UCodeLang/Compilation/Middle/IR.hpp"
#define StackName "Stack"
#define StackName_ "[" + StackName + "]"

#include "Zydis/Zydis.h"
#include "UCodeLang/Compilation/Helpers/InstructionBuilder.hpp"
UAssemblyStart
struct OutputIRLineState
{
	IRBuilder::ToStringState State;
	UCodeLang::UnorderedMap<UCodeLang::IRidentifierID, String> Names;
};
void OutputIRLineInfo(IRBuilder* Builder,IRFunc* Func, const UDebugSetLineNumber* Val, OutputIRLineState& State, String& r)
{

	for (auto& Block : Func->Blocks)
	{
		for (auto& Ins : Block->DebugInfo.DebugInfo)
		{
			if (auto DebugIns = Ins.Debug.Get_If<IRDebugSetLineNumber>())
			{
				if (DebugIns->LineNumber == Val->LineNumber)
				{
					r += "   //IR-Data:{\n";
					
					
					{
						for (size_t i = Val->ForIns; i < Block->Instructions.size(); i++)
						{
							{//Stop Adding If not on same Line
								for (auto& Ins2 : Block->DebugInfo.DebugInfo)
								{
									if (&Ins2 != &Ins)
									{
										if (auto DebugIns2 = Ins2.Debug.Get_If<IRDebugSetLineNumber>())
										{
											if (DebugIns2->InsInBlock == i &&
												DebugIns2->LineNumber > Val->LineNumber)
											{
												goto DoneLoop;
											}
										}
									}
								}
							}


							auto& Item = Block->Instructions[i];
							r += "   ";
							r += "   ";
							Builder->ToString(Item.get(), r, State.State, State.Names, i, Block.get());
							r += ';';
							{
								auto DebugInfo = Block->DebugInfo.Get_debugfor(i);
								
								for (auto& Item : DebugInfo)
								{
									if (auto Val = Item->Debug.Get_If<IRDebugSetVarableName>())
									{
										r += "//Variable:" + Val->VarableName;
									}
								}
							}
							r += '\n';
						}
						
					}
					DoneLoop:
					r += "   //}\n";

					return;

				}
			}
		}
	}
}
bool UAssembly::Assemble(const String_view& Text, UClib* Out)
{
	Lexer Lex; 
	Parser Parse; 
	Lex.Set_ErrorsOutput(_ErrorsOutput);
	Lex.Set_Settings(_Settings);
	Parse.Set_ErrorsOutput(_ErrorsOutput);
	Parse.Set_Settings(_Settings);
	
	
	Lex.Lex(Text);
	Parse.Parse(Lex.Get_Output(), Out);

	return _ErrorsOutput->Has_Errors() == false;
}



String UAssembly::ToString(const UClib* Lib, Optional<Path> SourceFiles, bool ShowIR)
{
	auto& InsMapData = Get_InsToInsMapValue();
    String r;
	UnorderedMap<UAddress, String> AddressToName;

	UnorderedMap<String, Vector<String>> OpenedSourceFilesLines;
	String OnFile;

	auto UCodeLayer = Lib->GetLayer(UCode_CodeLayer_UCodeVM_Name);
	
	if (UCodeLayer && UCodeLayer->_Data.Is<CodeLayer::UCodeByteCode>())
	{
		const CodeLayer::UCodeByteCode& Info = UCodeLayer->_Data.Get<CodeLayer::UCodeByteCode>();
		for (const auto& Item2 : Info._NameToPtr)
		{
			AddressToName.AddValue(Item2.second,Item2.first);
		}
	}
	Optional<IRBuilder> IRInfo;
	{
		auto IRLayer = Lib->GetLayer(UCode_CodeLayer_IR_Name);
		if (IRLayer && IRLayer->_Data.Is<CodeLayer::JustData>()) 
		{
			auto& Data = IRLayer->_Data.Get<CodeLayer::JustData>();
			IRBuilder tep;
			if (IRBuilder::FromBytes(tep, BytesView::Make( Data._Data.data(), Data._Data.size() )))
			{
				IRInfo = std::move(tep);
			}
		}
	}
	r += "[ClassData]-- \n";
	
	auto& Assembly = Lib->Get_Assembly();
	for (auto& Item : Assembly.Classes)
	{
		switch (Item->Get_Type())
		{
		case ClassType::NameSpace:
		{
			r += "namespace " + Item->FullName + ";";
			r += "\n\n";
		}
		break;
		case ClassType::Class:
		{
			auto& Class = Item->Get_ClassData();

			for (auto& Item2 : Class.Attributes.Attributes)
			{
				r += ToString(Item2,Assembly,Lib->BitSize);
			}
			if (Class.Attributes.Attributes.size())
			{
				r += '\n';
			}

			bool hasany = Class.Fields.size() || Class.Methods.size();

			r += "$" + Item->FullName;	
			if (!hasany)
			{
				r += ";";
			}
		
			if (hasany)
			{
				if (Class.IsExported)
				{
					r += " export";
				}
				r += ":\n";
				r += ".size:" + std::to_string(Class.Size) + "\n";

				Optional<AccessModifierType> _AccessModifier;

				for (auto& Item2 : Class.Fields)
				{
					if (!_AccessModifier.has_value() || _AccessModifier.value() != Item2.Protection)
					{
						r += "\n";
						r += Item2.Protection == AccessModifierType::Public ?
							"public" :
							"private";

						r += ": \n";

						_AccessModifier = Item2.Protection;
					}

					r += " " + ToString(Item2.Type, Assembly) + " " + Item2.Name + ";//Offset " + std::to_string(Item2.offset) + "\n";
				}
				r += "\n";

				for (auto& Item2 : Class.Methods)
				{
					if (!_AccessModifier.has_value() || _AccessModifier.value() != Item2.Protection)
					{
						r += "\n";
						r += Item2.Protection == AccessModifierType::Public ?
							"public" :
							"private";

						r += ": \n";

						_AccessModifier = Item2.Protection;
					}

					ToString(r, Item2, Lib);
				}
			}
			r += "\n\n";
		}
		break; 
		case ClassType::Alias:
		{
			auto& Class = Item->Get_AliasData();
			r += "$" + Item->FullName;

			if (Class.IsExported)
			{
				r += " export";
			}

			r += " = ";

			if (Class.HardAliasTypeID.has_value()) {
				r += "!";
			}
			r += ToString(Class.Type, Assembly) + ";\n\n";
		}
		break;
		case ClassType::Enum:
		{
			auto& Enum = Item->Get_EnumData();
			r += "$" + Item->FullName + " enum[" + ToString(Enum.BaseType,Assembly) + "]";

			if (Enum.IsExported)
			{
				r += " export";
			}
			bool hasany = Enum.Values.size();
			if (hasany)
			{
				r += ":\n";
			}
			else
			{
				r += ";";
			}
			for (auto& Item : Enum.Values)
			{
				r += " " + Item.Name;
				if (Item.EnumVariantType.has_value())
				{
					auto& Value = Item.EnumVariantType.value();
					r += "[";
					r += ToString(Value, Assembly);
					r += "]";
				}

				r += " = " + ToString(Enum.BaseType, Item._Data, Assembly,Lib->BitSize);

				if (&Item != &Enum.Values.back())
				{
					r += ",\n";
				}
			}
			if (Enum.EnumVariantUnion)
			{
				auto Ptr = Assembly.Find_Node(Enum.EnumVariantUnion.value());
				if (Ptr)
				{
					r += "\n//Union = " + Ptr->FullName;
				}
			}

			r += "\n\n";
		}
		break;
		case ClassType::FuncPtr:
		{
			auto& FuncPtr = Item->Get_FuncPtr();

			r += "$" + Item->FullName + " = |[";

			for (auto& Item : FuncPtr.ParsType)
			{
				r += ToString(Item, Assembly);
				if (&Item != &FuncPtr.ParsType.back())
				{
					r += ",";
				}
			}

			r += "] -> ";
			r += ToString(FuncPtr.RetType, Assembly) + ";\n\n";
		}
		break;
		case ClassType::Tag:
		{
			r += "$" + Item->FullName + " tag";

			auto& TagData = Item->Get_TagData();
			bool hasany = TagData.Fields.size() || TagData.Fields.size();
			
			if (TagData.IsExported)
			{
				r += " export";
			}

			if (hasany)
			{
				r += ":\n";
				for (auto& Item2 : TagData.Fields)
				{
					r += " " + ToString(Item2.Type, Assembly) + " " + Item2.Name + ";//Offset " + std::to_string(Item2.offset) + "\n";
				}
				r += "\n";

				for (auto& Item2 : TagData.Methods)
				{
					r += " ";
					for (auto& Item3 : Item2.Attributes.Attributes)
					{
						r += ToString(Item3, Assembly, Lib->BitSize);
					}

					r += "|" + ScopeHelper::GetNameFromFullName(Item2.FullName) + "[";

					for (auto& Item3 : Item2.ParsType)
					{
						if (&Item3 == &Item2.ParsType.front() && Item2.IsThisFunction)
						{

							if (Item3.Type.Isimmutable())
							{
								r += "umut ";
							}

							r += "this&";
						}
						else
						{
							r += ToString(Item3, Assembly);
						}
						if (&Item3 != &Item2.ParsType.back()) {
							r += ", ";
						}
					}
					r += "] -> " + ToString(Item2.RetType, Assembly) + ";";
					r += "//" + Item2.DecorationName + '\n';

				}
			}
			else
			{
				r += ";";
			}
			r += "\n\n";
		}
		break;
		case ClassType::Trait:
		{
			r += "$" + Item->FullName + " trait:\n";
			auto& TraitData = Item->Get_TraitData();

			if (TraitData.IsExported)
			{
				r += " export";
			}

		}
		break;
		case ClassType::GenericClass:
		{
			r += "---\n\n";
			r += Item->Get_GenericClass().Base.Implementation;
			r += "---\n";
		}
		break;
		case ClassType::GenericFunction:
		{
			r += "---\n\n";
			r += Item->Get_GenericFunctionData().Base.Implementation;
			r += "---\n";
		}
		break;
		case ClassType::StaticArray:
		{
		}
		break;
		case ClassType::ForType:
		{
			auto& TraitData = Item->Get_ForType();
			r += "$for " + ToString(TraitData._TargetType,Assembly);

			if (TraitData.IsExported)
			{
				r += " export";
			}
			if (TraitData._AddedMethods.size())
			{
				r += ": \n";

				for (auto& Item : TraitData._AddedMethods)
				{
					ToString(r, Item,Lib);
				}
			}
			else 
			{
				r += ";";
			}
		}
		break;
		case ClassType::Eval:
		{
			auto& TraitData = Item->Get_EvalData();
			if (TraitData.IsExported)
			{
				r += " export ";
			}
			r += "eval ";
			r += ToString(TraitData.Value._Type, Assembly);
			r += " ";
			r += Item->FullName;
			r += " = ";
			r += ToString(TraitData.Value,Assembly,Lib->BitSize);
			r += ";\n";
		}
		break;
		default:
			UCodeLangUnreachable();
			break;
		} 
	}


	size_t OnFuncFrameStackSize = 0;

	if (UCodeLayer && UCodeLayer->_Data.Is<CodeLayer::UCodeByteCode>())
	{
		const CodeLayer::UCodeByteCode& Info = UCodeLayer->_Data.Get<CodeLayer::UCodeByteCode>();
		r += "\n[Instructions:"  + UCodeLayer->_Name + "]-- \n";

		auto& Insts = Info.Get_Instructions();
		String OnFunc; 
		UnorderedMap<IRidentifierID, OutputIRLineState> IRStringStates;
		BytesView staticbytesview = BytesView::Make(Lib->_StaticBytes.data(), Lib->_StaticBytes.size());
		for (size_t i = 0; i < Insts.size(); i++)
		{
			auto& Item = Insts[i];
			UAddress address = (UAddress)i;
			if (AddressToName.HasValue(address))
			{
				String Name = AddressToName.GetValue(address);
				r += "---" + Name + ": \n";
				OnFunc = Name;
			}

			if (Info.DebugInfo.has_value())
			{
				auto& Value = Info.DebugInfo.value();
				auto List = Value.GetForIns(i);

				if (List.size())
				{
					r += '\n';
				}
				for (auto& Item : List)
				{
					if (auto Val = Item->Debug.Get_If<UDebugSetFile>())
					{
						r += "   //File:";
						r += Val->FileName;
						r += '\n';

						OnFile = Val->FileName;
					}
					else if (auto Val = Item->Debug.Get_If<UDebugSetLineNumber>())
					{
						r += "   //Line:";
						r += std::to_string(Val->LineNumber);
						r += '\n';


						if (SourceFiles.has_value())
						{
							r += "   //Source Line:";
							String LineStr;

							String* ItemValue = nullptr;
							if (OpenedSourceFilesLines.HasValue(OnFile))
							{
								auto& Item = OpenedSourceFilesLines.GetValue(OnFile);

								if (Val->LineNumber - 1 < Item.size()) {
									ItemValue = &Item[Val->LineNumber - 1];
								}
							}
							else
							{
								std::ifstream file;

								file.open(Path(SourceFiles.value() / Path(OnFile).native()));
								if (file.is_open())
								{
									std::string str;
									Vector<String> Lines;
									while (std::getline(file, str))
									{
										Lines.push_back(std::move(str));
									}
									OpenedSourceFilesLines.AddValue(OnFile, std::move(Lines));

									auto& Item = OpenedSourceFilesLines.GetValue(OnFile);

									if (Val->LineNumber - 1 < Item.size()) {
										ItemValue = &Item[Val->LineNumber - 1];
									}
								}
								else
								{
									OpenedSourceFilesLines.AddValue(OnFile, {});
								}

							}


							if (ItemValue)
							{
								LineStr += *ItemValue;
							}
							else
							{
								LineStr += "[Cant Find Line]";
							}

							r += LineStr;
							r += '\n';
						}
						if (IRInfo.has_value() && ShowIR)
						{
							auto& IRInfoVal = IRInfo.value();


							bool HasStackFrame = OnFuncFrameStackSize != 0;
							if (StaticVariablesInitializeFunc == OnFunc)
							{
								auto Id = IRInfoVal._StaticInit.identifier;
								if (!IRStringStates.HasValue(Id))
								{
									OutputIRLineState LineState;
									String Unused;
									IRInfoVal.ToString(LineState.State, &IRInfoVal._StaticInit, Unused);
									IRStringStates.AddValue(Id, std::move(LineState));
								}
								OutputIRLineInfo(&IRInfoVal, &IRInfoVal._StaticInit, Val, IRStringStates.GetValue(Id), r);
							}
							else if (ThreadVariablesInitializeFunc == OnFunc)
							{
								auto Id = IRInfoVal._threadInit.identifier;
								if (!IRStringStates.HasValue(Id))
								{
									OutputIRLineState LineState;
									String Unused;
									IRInfoVal.ToString(LineState.State, &IRInfoVal._threadInit, Unused);
									IRStringStates.AddValue(Id, std::move(LineState));
								}
								OutputIRLineInfo(&IRInfoVal, &IRInfoVal._threadInit, Val, IRStringStates.GetValue(Id), r);
							}
							else if (StaticVariablesUnLoadFunc == OnFunc)
							{
								auto Id = IRInfoVal._StaticdeInit.identifier;
								if (!IRStringStates.HasValue(Id))
								{
									OutputIRLineState LineState;
									String Unused;
									IRInfoVal.ToString(LineState.State, &IRInfoVal._StaticdeInit, Unused);
									IRStringStates.AddValue(Id, std::move(LineState));
								}
								OutputIRLineInfo(&IRInfoVal, &IRInfoVal._StaticdeInit, Val, IRStringStates.GetValue(Id), r);
							}
							else if (ThreadVariablesUnLoadFunc == OnFunc)
							{
								auto Id = IRInfoVal._threaddeInit.identifier;
								if (!IRStringStates.HasValue(Id))
								{
									OutputIRLineState LineState;
									String Unused;
									IRInfoVal.ToString(LineState.State, &IRInfoVal._threaddeInit, Unused);
									IRStringStates.AddValue(Id, std::move(LineState));
								}
								OutputIRLineInfo(&IRInfoVal, &IRInfoVal._threaddeInit, Val, IRStringStates.GetValue(Id), r);
							}
							else
							{
								for (auto& Func : IRInfoVal.Funcs)
								{

									if (IRInfoVal.FromID(Func->identifier) == OnFunc)
									{
										auto Id = Func->identifier;
										if (!IRStringStates.HasValue(Id))
										{
											OutputIRLineState LineState;
											String Unused;
											IRInfoVal.ToString(LineState.State, Func.get(), Unused);
											IRStringStates.AddValue(Id, std::move(LineState));
										}
										OutputIRLineInfo(&IRInfoVal, Func.get(), Val, IRStringStates.GetValue(Id), r);
										break;
									}
								}
							}
						}
					}
					else if (auto Val = Item->Debug.Get_If<UDebugSetFuncStackFrameSize>())
					{
						r += "   //StackFrameSize:" + std::to_string(Val->StackFrameSize);
						r += '\n';
						OnFuncFrameStackSize = Val->StackFrameSize;
					}
					else if (auto Val = Item->Debug.Get_If<UDebugSetVarableLoc>())
					{
						r += "   //";

						if (auto Value = Val->Type.Get_If<RegisterID>())
						{
							r += GetRegisterToString(*Value);
						}
						else
						{
							UCodeLangThrowException("not added");
						}
						r += " = ";
						r += Val->VarableFullName;
						r += '\n';
					}
				}



				if (List.size())
				{
					r += '\n';
				}
			}

			r += "   " + std::to_string(i) + " :";

			i += ParseInstruction(i,Span<Instruction>::Make(Insts.data(), Insts.size()), r, staticbytesview, AddressToName);

			r += '\n';
		}
	}

	for (auto& Item : Lib->_Layers) 
	{
		if (Item->_Name == UCode_CodeLayer_X86_64_UCodeVM_Name)
		{
			
			if (auto Val = Item->_Data.Get_If<CodeLayer::MachineCode>())
			{
				r += "\n[Native-Instructions:" + Item->_Name + "]-- \n";
				
				UnorderedMap<UAddress, String> AddressToName;
				for (const auto& Item2 : Val->_NameToPtr)
				{
					AddressToName.AddValue(Item2.second,Item2.first);
				}
				
				auto MachineMode = ZYDIS_MACHINE_MODE_LONG_64;
				ZyanU64 runtime_address = (ZyanU64)Val->_Code.data();
				size_t InsSize = Val->_Code.size();
				//runtime_address = 0;
				// Loop over the instructions in our buffer.
				ZyanUSize offset = 0;
				ZydisDisassembledInstruction instruction;

				while (ZYAN_SUCCESS(ZydisDisassembleIntel(
					/* machine_mode:    */ MachineMode,
					/* runtime_address: */ runtime_address,
					/* buffer:          */ (void*)((uintptr_t)Val->_Code.data() + offset),
					/* length:          */ InsSize - offset,
					/* instruction:     */ &instruction
				)))
				{
					if (AddressToName.HasValue(offset))
					{
						String Name = AddressToName.GetValue(offset);
						r += "---" + Name + ": \n";
						
					}


					r += "   " + std::to_string(instruction.runtime_address) + " :";
					r += instruction.text;
					r += '\n';
					offset += instruction.info.length;
					runtime_address += instruction.info.length;
				}
			}
		}
	}
    return r;
}

size_t UAssembly::ParseInstruction( size_t I,const Span<Instruction> Data, String& r, const BytesView staticbytesview, UnorderedMap<UAddress, String>& AddressToName
, bool CombineIns)
{
	bool is32mode = UCodeLang_32BitSytem;
	bool is64mode = !is32mode;
	auto& InsMapData = Get_InsToInsMapValue();

	if (CombineIns == false) 
	{
		ToStringInstruction(Data[I], r, staticbytesview, AddressToName);
		return 0;
	}
	else
	{
		auto& Ins = Data[I];
		if (Data[I].OpCode == InstructionSet::Store32v1)
		{
			auto Opt = Instruction::IsLoad32(Data, I);
			if (Opt)
			{
				Int32 V = Opt.value();
				UInt32& VU = *(UInt32*)&V;
				((UInt16*)&V)[0] = Ins.Op_RegUInt16.B;
				r += "Store32 " + GetRegisterToString(Ins.Op_RegUInt16.A);
				r += ", " + std::to_string(VU);
				if (std::to_string(V) != std::to_string(VU))
				{
					r += "|";
					r += std::to_string(V);
				}
				return 1;
			}
		}

	
		if (Data[I].OpCode == InstructionSet::Storef32v1)
		{	
			auto Opt = Instruction::IsLoadf32(Data, I);
			if (Opt)
			{
				float32 V = Opt.value();
				r += "Storef32 " + GetRegisterToString(Ins.Op_RegUInt16.A);
				r += ", " + std::to_string(V);
				return 1;
			}
		}
		if (Data[I].OpCode == InstructionSet::Store64v1) 
		{
			auto Opt = Instruction::IsLoad64(Data, I);
			if (Opt)
			{
				Int64 V = Opt.value();
				UInt64& VU = *(UInt64*)&V;


				r += "Store64 " + GetRegisterToString(Ins.Op_RegUInt16.A);
				r += ", " + std::to_string(VU);
				if (std::to_string(V) != std::to_string(VU))
				{
					r += "|";
					r += std::to_string(V);
				}
				return 3;

			}
		}
		if (Data[I].OpCode == InstructionSet::Storef64v1)
		{
			auto Opt = Instruction::IsLoadf64(Data, I);
			if (Opt)
			{
				float64 V = Opt.value();
				r += "Storef64 " + GetRegisterToString(Ins.Op_RegUInt16.A);
				r += ", " + std::to_string(V);

				return 3;
			}

		}
		if (Data[I].OpCode == InstructionSet::Callv1)
		{

			{
				auto Opt = Instruction::IsCall(Data, I,is32mode);
				if (Opt)
				{
					UAddress V = Opt.value();
					r += "Call ";
					OpValueToString(InsMapData.at(Data[I].OpCode)->Op_A, &V, AddressToName, staticbytesview, r);


					if (is32mode)
					{
						return 1;
					}
					else
					{
						return 3;
					}
				}
			}
			{
				auto Opt = Instruction::IsCallIf(Data, I,is32mode);
				if (Opt)
				{
					UAddress V = Opt.value().Func;
					r += "Callif ";
					OpValueToString(InsMapData.at(Data[I].OpCode)->Op_B, &V, AddressToName, staticbytesview, r);
					r += "," + GetRegisterToString(Opt.value().Reg);
				
					if (is32mode)
					{
						return 1;
					}
					else
					{
						return 3;
					}
				}

			}
		}
		if (Data[I].OpCode == InstructionSet::Jumpv1)
		{
			{
				auto Opt = Instruction::IsJump(Data, I,is32mode);
				if (Opt)
				{
					UAddress V = Opt.value();
					r += "Jump ";
					OpValueToString(InsMapData.at(Data[I].OpCode)->Op_A, &V, AddressToName, staticbytesview, r);


					if (is32mode)
					{
						return 1;
					}
					else 
					{
						return 3;
					}
				}
			}
			{
				auto Opt = Instruction::IsJumpIf(Data, I,is32mode);
				if (Opt)
				{
					UAddress V = Opt.value().Func;
					r += "Jumpif ";
					r += GetRegisterToString(Opt.value().Reg);
					r += ",";
					OpValueToString(InsMapData.at(InstructionSet::Jumpif)->Op_B, &V, AddressToName, staticbytesview, r);

					if (is32mode)
					{
						return 1;
					}
					else 
					{
						return 3;
					}
				}

			}
		}
		if (Data[I].OpCode == InstructionSet::LoadFuncPtrV1)
		{
			auto Opt = Instruction::IsLoadFuncPtr(Data, I,is32mode);
			if (Opt)
			{
				UAddress V = Opt.value();
				r += "LoadFuncPtr " + GetRegisterToString(Ins.Op_RegUInt16.A);
				r += ", "; 
				OpValueToString(InsMapData.at(Data[I].OpCode)->Op_B, &V, AddressToName, staticbytesview, r);

				if (is32mode)
				{
					return 1;
				}
				else 
				{
					return 3;
				}
			}

		}

		ToStringInstruction(Data[I], r, staticbytesview, AddressToName);
		return 0;
	}
}

void UAssembly::ToStringInstruction(const Instruction& Item, String& r, const BytesView staticbytesview, UnorderedMap<UAddress,String>& AddressToName)
{
	auto& InsMapData = Get_InsToInsMapValue();
	if (InsMapData.count(Item.OpCode))
	{
		auto& MapData = InsMapData[Item.OpCode];
		r += (String)MapData->InsName;
		r += " ";

		auto optype = Instruction::GetOpType(Item.OpCode);
		switch (optype)
		{
		case UCodeLang::Instruction::OpType::NoneOp:
			break;
		case UCodeLang::Instruction::OpType::OneReg:
		{
			r += GetRegisterToString(Item.Op_TwoReg.A);
		}
		break;
		case UCodeLang::Instruction::OpType::TwoReg:
		{
			r += GetRegisterToString(Item.Op_TwoReg.A);
			r += ',';
			r += GetRegisterToString(Item.Op_TwoReg.B);
		}
		break;
		case UCodeLang::Instruction::OpType::ThreeReg:
		{
			r += GetRegisterToString(Item.Op_ThreeReg.A);
			r += ',';
			r += GetRegisterToString(Item.Op_ThreeReg.B);
			r += ',';
			r += GetRegisterToString(Item.Op_ThreeReg.C);
		}
		break;
		case UCodeLang::Instruction::OpType::RegUInt8:
		{
			r += GetRegisterToString(Item.Op_RegUInt8.A);
			r += ',';
			OpValueToString(MapData->Op_B, &Item.Op_RegUInt8.B, AddressToName, staticbytesview, r);
		}
		break;
		case UCodeLang::Instruction::OpType::RegUInt16:
		{
			if (MapData->Op_A != OpCodeType::NoOpCode)
			{
				r += GetRegisterToString(Item.Op_RegUInt16.A);
				r += ',';
				OpValueToString(MapData->Op_B, &Item.Op_RegUInt16.B, AddressToName, staticbytesview, r);
			}
		}
		break;
		case UCodeLang::Instruction::OpType::ValUInt8:
		{
			OpValueToString(MapData->Op_A, &Item.Op_ValUInt8.A, AddressToName, staticbytesview, r);
		}
		break;
		case UCodeLang::Instruction::OpType::ValUInt16:
		{
			OpValueToString(MapData->Op_A, &Item.Op_ValUInt16.A, AddressToName, staticbytesview, r);
		}
		break;
		case UCodeLang::Instruction::OpType::TwoRegInt8:
		{
			r += GetRegisterToString(Item.Op_TwoRegInt8.A);
			r += ',';
			r += GetRegisterToString(Item.Op_TwoRegInt8.B);
			r += ',';
			OpValueToString(MapData->Op_C, &Item.Op_TwoRegInt8.C, AddressToName, staticbytesview, r);
		}
		break;
		default:
			UCodeLangUnreachable();
			break;
		}
	}
	else
	{
		r += "Ins " + std::to_string((uintptr_t)Item.OpCode) + ":" + 
			std::to_string((uintptr_t)Item.Op_ThreeUInt8.A) + "," +
			std::to_string((uintptr_t)Item.Op_ThreeUInt8.B) + "," +
			std::to_string((uintptr_t)Item.Op_ThreeUInt8.C);
	}
}

String UAssembly::ToString(const ReflectionTypeInfo& Value, const ClassAssembly& Assembly)
{
	String r;
	if (Value.Isimmutable())
	{
		r = "imut ";
	}

	if (Value._MoveData ==ReflectionMoveData::Moved)
	{
		r += "moved ";
	}

	if (Value._IsDynamic)
	{
		r += "dynamic<";
	}

	switch (Value._Type)
	{
	case ReflectionTypes::Bool:r += "bool";break;
	case ReflectionTypes::Char:r += "char"; break;

	case ReflectionTypes::Uft8:r += "uft8"; break;
	case ReflectionTypes::Uft16:r += "uft16"; break;
	case ReflectionTypes::Uft32:r += "uft32"; break;


	
	case ReflectionTypes::sInt8:r += "sint8"; break;
	case ReflectionTypes::uInt8:r += "uint8"; break;

	case ReflectionTypes::sInt16:r += "sint16"; break;
	case ReflectionTypes::uInt16:r += "uint16"; break;

	case ReflectionTypes::sInt32:r += "sint32"; break;
	case ReflectionTypes::uInt32:r += "uint32"; break;

	case ReflectionTypes::sInt64:r += "sint64"; break;
	case ReflectionTypes::uInt64:r += "uint64"; break;

	case ReflectionTypes::uIntPtr:r += "uIntPtr"; break;
	case ReflectionTypes::sIntPtr:r += "sIntPtr"; break;


	case ReflectionTypes::float32:r += "float32"; break;
	case ReflectionTypes::float64:r += "float64"; break;

	
	case ReflectionTypes::Void:
		r += "void";	break;
	case  ReflectionTypes::Any:
		r += "[any]";
		break;
	case ReflectionTypes::Null:
	default:	
		r += "[badtype]";
		break;
	case ReflectionTypes::CustomType:
	{
		auto Ptr =Assembly.Find_Node(Value._CustomTypeID);
		if (Ptr)
		{
			r += Ptr->FullName;
		}
		else
		{
			r += "[?]";
		}
	}
		break;
	}

	if (Value._IsDynamic)
	{
		r += ">";
	}

	if (Value.IsAddress())
	{
		r += "&";
	}
	if (Value.IsAddressArray())
	{
		r += "[&]";
	}

	return r;
}
String UAssembly::ToString(const ReflectionTypeInfo& Value, const ReflectionRawData& Data, const ClassAssembly& Assembly, UClib::NTypeSize PtrSize)
{
	String r;
	switch (Value._Type)
	{
	case ReflectionTypes::Bool:
		r += *Data.Get_DataAs<bool>() ? "true" : "false"; 
		break;
	case ReflectionTypes::Char:
		r += *Data.Get_DataAs<char>();
		break;
	case ReflectionTypes::sInt8:
		r += std::to_string(*Data.Get_DataAs<Int8>());
		break;
	case ReflectionTypes::uInt8:
		r += std::to_string(*Data.Get_DataAs<UInt8>());
		break;
	case ReflectionTypes::sInt16:
		r += std::to_string(*Data.Get_DataAs<Int16>());
		break;
	case ReflectionTypes::uInt16:
		r += std::to_string(*Data.Get_DataAs<UInt16>());
		break;
	case ReflectionTypes::sInt32:
		r += std::to_string(*Data.Get_DataAs<Int32>());
		break;
	case ReflectionTypes::uInt32:
		r += std::to_string(*Data.Get_DataAs<UInt32>());
		break;
	case ReflectionTypes::sInt64:
		r += std::to_string(*Data.Get_DataAs<Int64>());
		break;
	case ReflectionTypes::uInt64:
		r += std::to_string(*Data.Get_DataAs<UInt64>());
		break;
		r += "uint64"; break;

	case ReflectionTypes::uIntPtr:
		r += PtrSize == UClib::NTypeSize::int32 ? std::to_string(*Data.Get_DataAs<UInt32>()) : std::to_string(*Data.Get_DataAs<UInt64>());
		break;
	case ReflectionTypes::sIntPtr:
		r += PtrSize == UClib::NTypeSize::int32 ? std::to_string(*Data.Get_DataAs<Int32>()) : std::to_string(*Data.Get_DataAs<Int64>());
		break;


	case ReflectionTypes::float32:
		r += std::to_string(*Data.Get_DataAs<float32>());
		break;
	case ReflectionTypes::float64:
		r += std::to_string(*Data.Get_DataAs<float64>());
		break;
	case ReflectionTypes::Void:
		return "void";
	case ReflectionTypes::CustomType:
	{
		auto node = Assembly.Find_Node(Value._CustomTypeID);

		if (node)
		{
				switch (node->Get_Type())
				{
				case ClassType::StaticArray:
				{
					const auto& info = node->Get_StaticArray();

					if (info.BaseType._Type == ReflectionTypes::Char)
					{
						r += "\"";
						for (size_t i = 0; i < info.Count; i++)
						{
							r += (char)Data.Bytes[i];
						}
						r += "\"";
					}
					else 
					{
						auto elmsize =Assembly.GetSize(info.BaseType, PtrSize == UClib::NTypeSize::int32);
						
						if (!elmsize.has_value()) {
							return r;
						}
						r += "[";
						
						
						for (size_t i = 0; i < info.Count; i++)
						{
							ReflectionRawData elm;
							elm.Bytes.reset((Byte*)(((uintptr_t)Data.Bytes.get()) + (i * elmsize.value())));

							elm.Size = elmsize.value();
							r += ToString(info.BaseType, ReflectionRawData(), Assembly, PtrSize);
						
							elm.Bytes.release();

							if (i != info.Count - 1)
							{
								r += ",";
							}
						}
						r += "]";
					}
				}break;
				default:
					break;
				}
		}
	}
	break;
	default:
		break;
	}
	return r;
}
String UAssembly::ToString(const UsedTagValueData& Value, const ClassAssembly& Assembly, UClib::NTypeSize PtrSize)
{
	String R;

	R += "[";

	auto node = Assembly.Find_Node(Value.TypeID);
	if (node)
	{
		R += node->FullName;
	}
	else
	{
		R += "?";
	}

	if (node && node->Get_Type() == ClassType::Tag)
	{
		auto& tagdata = node->Get_TagData();

		if (tagdata.Fields.size())
		{
			R += "(";

			auto& Fields = node->Get_TagData().Fields;
			for (auto& Item : Fields)
			{
				void* p = (void*)((uintptr_t)Item.offset + (uintptr_t)Value._Data.Get_Data());

				TypedRawReflectionData r;
				r._Data.Bytes.reset((Byte*)p);
				r._Data.Size = Assembly.GetSize(Item.Type, PtrSize == UClib::NTypeSize::int32).value_or(0);
				
				r._Type = Item.Type;

				R += ToString(r,Assembly, PtrSize);

				r._Data.Bytes.release();

				if (&Item != &Fields.back())
				{
					R += ",";
				}
			}

			R += ")";
		}
	}

	R += "]";

	return R;
}
String UAssembly::ToString(const ClassMethod::Par& Value, const ClassAssembly& Assembly)
{
	String R;
	if (Value.IsOutPar)
	{
		R += "out ";
	}
	R += ToString(Value.Type, Assembly);
	return R;
}
void UAssembly::ToString(String& r, ClassMethod& Item2, const UClib* Lib)
{
	auto& Assembly = Lib->Get_Assembly();
	r += " ";
	for (auto& Item3 : Item2.Attributes.Attributes)
	{
		r += ToString(Item3, Assembly, Lib->BitSize);
	}
	if (Item2.Attributes.Attributes.size())
	{
		r += '\n';
	}
	if (Item2.IsExport)
	{
		r += "export ";
	}

		

	r += "|" + ScopeHelper::GetNameFromFullName(Item2.FullName) + "[";

	for (auto& Item3 : Item2.ParsType)
	{
		if (&Item3 == &Item2.ParsType.front() && Item2.IsThisFunction)
		{

			if (Item3.Type.Isimmutable())
			{
				r += "umut ";
			}

			r += "this&";
		}
		else
		{
			r += ToString(Item3, Assembly);
		}
		if (&Item3 != &Item2.ParsType.back()) {
			r += ", ";
		}
	}
	r += "] -> " + ToString(Item2.RetType, Assembly) + ";";
	r += "//" + Item2.DecorationName + '\n';
}
void UAssembly::OpValueToString(OpCodeType OpType,const void* In,const UnorderedMap<UAddress, String>& AddressToName,const BytesView StaticVarablesData, String& out)
{

	switch (OpType)
	{
	case OpCodeType::NoOpCode:
		break;
	case OpCodeType::AnyInt8:
	{
		String tepS = std::to_string((UInt64)(*(UInt8*)In));
		String teps2 = std::to_string((Int64)(*(Int8*)In));
		out += (tepS == teps2) ? tepS : tepS + "|" + teps2;
	}	
	break;
	case OpCodeType::AnyInt16:
	{
		String tepS = std::to_string((UInt64)(*(UInt16*)In));
		String teps2 = std::to_string((Int64)(*(Int16*)In));
		out += (tepS == teps2) ? tepS : tepS + "|" + teps2;
	}
	break;
	case OpCodeType::AnyInt32:
	{
		String tepS = std::to_string((UInt64)(*(UInt32*)In));
		String teps2 = std::to_string((Int64)(*(Int32*)In));
		out += (tepS == teps2) ? tepS : tepS + "|" + teps2;
	}	
	break;
	case OpCodeType::AnyInt64:
	{
		String tepS = std::to_string((UInt64)(*(UInt64*)In));
		String teps2 = std::to_string((Int64)(*(Int32*)In));
		out += (tepS == teps2) ? tepS : tepS + "|" + teps2;
	}	
	break;

	
	case OpCodeType::Register:
		out += GetRegisterToString(*(RegisterID*)In);
		break;
	case OpCodeType::StaticCString:
		out += "\"" + (String)(const char*)&StaticVarablesData[(*(UInt16*)In)] + "\"";
		break;

	case OpCodeType::InsAddress:
	{
		auto NewAddress = (*(UAddress*)In) + 1;
		if (AddressToName.HasValue(NewAddress))
		{
			out += "{" + AddressToName.GetValue(NewAddress) + "}";
		}
		else
		{
			out += "{" + std::to_string(NewAddress) + "}";
		}
	}	
	break;
	default:
		UCodeLangUnreachable();
		break;
	}
}
size_t UAssembly::BuildHashForSub(const Instruction* Pointer, size_t BufferSize)
{
	String_view Item((char*)Pointer, BufferSize * sizeof(Instruction));

	return std::hash<String_view>()(Item);
}
UAssembly::StripOutput UAssembly::Strip(UClib& lib,const StripSettings& settings)
{
	UAssembly::StripOutput r;


	if (settings.DebugInfo)
	{
		lib._DebugBytes.clear();
	
		for (auto& Item : lib._Layers)
		{
			if (Item->_Name == settings.TargetCodeLayer)
			{
				if (Item->_Data.Is<CodeLayer::UCodeByteCode>()) {
					auto& v = Item->_Data.Get<CodeLayer::UCodeByteCode>();
					v.DebugInfo = {};
					break;
				}
			}
		}
	}
	if (settings.FuncToAddress)
	{
		for (auto& Item : lib._Layers)
		{
			if (Item->_Name == settings.TargetCodeLayer)
			{
				if (Item->_Data.Is<CodeLayer::UCodeByteCode>()) {
					auto& v = Item->_Data.Get<CodeLayer::UCodeByteCode>();
					r.StripedFuncToAddress = std::move(v._NameToPtr);
				}
			}
		}
	}
	if (settings.TypeAssembly)
	{
		r.StripedAssembly = std::move(lib.Get_Assembly());
	}

	return r;
}
UAssembly::StripFuncs UAssembly::StripFunc(UClib& lib, const StripFuncSettings& setting, TaskManger& tasks)
{
	StripFuncs r;

	auto layer = lib.GetLayer(UCode_CodeLayer_UCodeVM_Name);
	if (layer == nullptr || !layer->_Data.Is<CodeLayer::UCodeByteCode>())
	{
		return r;
	}
	CodeLayer::UCodeByteCode* ByteCodeLayer = &layer->_Data.Get<CodeLayer::UCodeByteCode>();

	bool is32mode = lib.BitSize == UClib::NTypeSize::int32;
	bool is64mode = !is32mode;

	Vector<const ClassMethod*> FuncionsToKeep;
	{
		UnorderedMap<const ClassMethod*, Vector<const ClassMethod*>> DirectReference;
		UnorderedMap<UAddress, String> AddToName;
		{
			DirectReference.reserve(ByteCodeLayer->_NameToPtr.size());

			auto c = tasks.WorkerCount();
			auto sizeperworker = ByteCodeLayer->_NameToPtr.size() / c;
			Vector<std::pair<const String, UAddress>*> ptrs;

			for (auto& Item : ByteCodeLayer->_NameToPtr)
			{
				ptrs.push_back(&Item);
			}
			auto& assembly = lib.Get_Assembly();

			using taskr = Vector<std::pair<const ClassMethod*, Vector<const ClassMethod*>>>;
			Vector<TaskManger::Task<taskr>> _tasks;
			_tasks.reserve(c);

			AddToName.reserve(ByteCodeLayer->_NameToPtr.size());
			for (auto& Item : ByteCodeLayer->_NameToPtr)
			{
				AddToName.AddValue(Item.second, Item.first);
			}

			for (size_t i = 0; i < c; i++)
			{
				std::function<taskr()> f = [is32mode,&AddToName, &assembly, &ptrs, i, ByteCodeLayer, sizeperworker, c]()
					{
						taskr r;

						size_t StartIndex = sizeperworker * i;
						size_t EndIndex = 0;
						bool lastone = i + 1 == c;

						if (lastone)
						{
							EndIndex = ByteCodeLayer->_NameToPtr.size();
						}
						else
						{
							EndIndex = std::min(sizeperworker * (i + 1), ByteCodeLayer->_NameToPtr.size());
						}

						Span<std::pair<const String, UAddress>*> mylist = Span(ptrs.data() + StartIndex, EndIndex - StartIndex);

						for (auto& Item : mylist)
						{
							auto f = assembly.Find_Func(Item->first);
							if (f == nullptr) { continue; }

							auto funcstart = ByteCodeLayer->_NameToPtr.GetValue(Item->first);
							Vector<const ClassMethod*> methods;

							for (size_t i = funcstart; i < ByteCodeLayer->_Instructions.size(); i++)
							{
								auto& Ins = ByteCodeLayer->_Instructions[i];

								Optional<UAddress> address;
								{
									address = Ins.IsCall(Span<Instruction>(ByteCodeLayer->_Instructions.data(), ByteCodeLayer->_Instructions.size()),i,is32mode);
									if (!address.has_value())
									{
										address = Ins.IsLoadFuncPtr(Span<Instruction>(ByteCodeLayer->_Instructions.data(), ByteCodeLayer->_Instructions.size()), i,is32mode);
									}
									if (!address.has_value())
									{
										auto v = Ins.IsCallIf(Span<Instruction>(ByteCodeLayer->_Instructions.data(), ByteCodeLayer->_Instructions.size()), i,is32mode);

										if (v.has_value()) {
											address = v.value().Func;
										}
									}
									if (!address.has_value())
									{
										auto v = Ins.IsJumpIf(Span<Instruction>(ByteCodeLayer->_Instructions.data(), ByteCodeLayer->_Instructions.size()), i,is32mode);

										if (v.has_value()) {
											address = v.value().Func;
										}
									}
									if (!address.has_value())
									{
										address = Ins.IsJump(Span<Instruction>(ByteCodeLayer->_Instructions.data(), ByteCodeLayer->_Instructions.size()), i,is32mode);
									}
								}

								if (address.has_value())
								{
									UAddress add = address.value() + 1;
									NullablePtr<ClassMethod> p;
									auto s = AddToName.TryFindValue(add);
									if (s.has_value())
									{
										auto func = assembly.Find_Func(s.value());
										if (func)
										{
											methods.push_back(func);
										}
									}
								}

								if (Ins.OpCode == InstructionSet::Return)
								{
									break;
								}
							}

							r.push_back(std::make_pair(f, std::move(methods)));

						}

						return r;
					};
				_tasks.push_back(tasks.AddTask(f, {}));
			}

			auto v = tasks.WaitFor(_tasks);
			for (auto& Item : v)
			{
				for (auto& Item2 : Item)
				{
					DirectReference.AddValue(Item2.first, std::move(Item2.second));
				}
			}
		}
		auto t = &lib;
		const auto& lib = *t;

		FuncionsToKeep.reserve(setting.FuncionsToKeep.size());

		Vector<const ClassMethod*> SearchFuncions;

		{
			SearchFuncions.reserve(setting.FuncionsToKeep.size());
			for (auto& Item : setting.FuncionsToKeep)
			{
				SearchFuncions.push_back(Item);
			}
			Array<UCodeLang::String_view, 4> basefuncions = { StaticVariablesInitializeFunc, StaticVariablesUnLoadFunc, ThreadVariablesInitializeFunc, ThreadVariablesUnLoadFunc };

			for (auto& Item : basefuncions)
			{
				if (ByteCodeLayer->_NameToPtr.HasValue((String)Item))
				{
					auto v = ByteCodeLayer->_NameToPtr.GetValue((String)Item);

					UAddress startfunc = v;
					UAddress endfunc = 0;

					auto& NewIns = ByteCodeLayer->_Instructions;
					for (size_t i = startfunc; i < NewIns.size(); i++)
					{
						auto& Item = NewIns[i];
						auto span = Span<Instruction>(NewIns.data(), NewIns.size());
						{
							auto v = Instruction::IsCall(span, i, is32mode);
							if (v.has_value())
							{
								auto tocall = v.value() + 1;

								if (AddToName.HasValue(tocall))
								{
									auto& name = AddToName.GetValue(tocall);

									const ClassMethod* ptr = lib.Get_Assembly().Find_Func(name);

									if (ptr)
									{
										bool isinlist = false;

										for (auto& Item : SearchFuncions)
										{
											if (Item == ptr)
											{
												isinlist = true;
												break;
											}
										}

										if (isinlist == false)
										{
											SearchFuncions.push_back(ptr);
										}
									}
								}
							}
						}

						{
							auto v = Instruction::IsLoadFuncPtr(span, i, is32mode);
							if (v.has_value())
							{
								RegisterID reg = NewIns[0].Op_RegUInt16.A;

								auto tocall = v.value() + 1;

								if (AddToName.HasValue(tocall))
								{
									auto& name = AddToName.GetValue(tocall);

									const ClassMethod* ptr = lib.Get_Assembly().Find_Func(name);

									if (ptr)
									{
										bool isinlist = false;
										
										for (auto& Item : SearchFuncions)
										{
											if (Item == ptr)
											{
												isinlist = true;
												break;
											}
										}

										if (isinlist == false)
										{
											SearchFuncions.push_back(ptr);
										}
									}
								}


							}
						}
						if (Item.OpCode == InstructionSet::Return)
						{
							endfunc = i;
							break;
						}
					}


				}
			}
		}

		size_t funcfoundcount = 0;
		Vector<const ClassMethod*> SearchCopy;
		do
		{
			funcfoundcount = FuncionsToKeep.size();

			SearchCopy = std::move(SearchFuncions);
			for (auto& Item : SearchCopy)
			{
				bool isinfuncionstokeep = false;
				{
					for (auto& Itemp : FuncionsToKeep)
					{
						if (Item == Itemp)
						{
							isinfuncionstokeep = true;
							break;
						}
					}
				}
				if (isinfuncionstokeep == false)
				{
					auto& funclist = DirectReference.GetValue(Item);

					FuncionsToKeep.push_back(Item);
					for (auto& Item : funclist)
					{
						bool isinfuncionstokeep = false;
						{
							for (auto& Itemp : FuncionsToKeep)
							{
								if (Item == Itemp)
								{
									isinfuncionstokeep = true;
									break;
								}
							}
						}

						if (isinfuncionstokeep == false)
						{
							SearchFuncions.push_back(Item);
						}
					}
				}
			}

		} while (FuncionsToKeep.size() != funcfoundcount);
	}


	UnorderedMap<UAddress, UAddress> oldtonew;
	Vector<Instruction> NewIns;
	UnorderedMap<String, UAddress> NewNameToAddress;

	for (auto& Item : ByteCodeLayer->_NameToPtr)
	{
		bool functokeep = false;

		if (Item.first == StaticVariablesInitializeFunc)
		{
			functokeep = true;
		}
		else if (Item.first == StaticVariablesUnLoadFunc)
		{
			functokeep = true;
		}
		else if (Item.first == ThreadVariablesInitializeFunc)
		{
			functokeep = true;
		}
		else if (Item.first == ThreadVariablesUnLoadFunc)
		{
			functokeep = true;
		}
		else 
		{
			for (auto& Itemp : FuncionsToKeep)
			{
				if (Itemp->DecorationName == Item.first)
				{
					functokeep = true;
					break;
				}
			}
		}
		
		if (!functokeep)
		{
			UAddress startfunc = Item.second;
			UAddress endfunc = 0;

			for (size_t i = startfunc; i < ByteCodeLayer->_Instructions.size(); i++)
			{
				auto& Item = ByteCodeLayer->_Instructions[i];

				if (Item.OpCode == InstructionSet::Return)
				{
					endfunc = i;
				}
			}

			UAddress funcsize = endfunc - startfunc;

			if (setting.RemoveFuncions)
			{
				auto p = lib.Get_Assembly().Remove_Func(Item.first);
				if (p.has_value()) 
				{
				
					r.RemovedFuncions.push_back(p.value());
				}
			}
		}
		else
		{
			UAddress startfunc = Item.second;
			UAddress endfunc = 0;

			for (size_t i = startfunc; i < ByteCodeLayer->_Instructions.size(); i++)
			{
				auto& Item = ByteCodeLayer->_Instructions[i];

				if (Item.OpCode == InstructionSet::Return)
				{
					endfunc = i+1;
					break;
				}
			}

			UAddress funcsize = endfunc - startfunc;
			
			size_t oldstartpos = NewIns.size();
			NewIns.resize(NewIns.size() + funcsize);

			oldtonew.AddValue(Item.second,oldstartpos);

			memcpy(NewIns.data() + oldstartpos,ByteCodeLayer->_Instructions.data() + startfunc,funcsize * sizeof(Instruction));
			NewNameToAddress.AddValue(Item.first, oldstartpos);

			auto span = Span(NewIns.data(), NewIns.size());
			for (size_t i = oldstartpos; i < oldstartpos + funcsize; i++)
			{
				auto& Ins = NewIns[i];

				{
					auto p = Instruction::IsJump(span, i, is32mode);
					if (p.has_value())
					{
						auto jumpto = p.value();

						bool ispartofthisfunc = false;
						if (jumpto >= startfunc) 
						{
							ispartofthisfunc  = (jumpto - startfunc) < funcsize;
						}

						if (ispartofthisfunc)
						{
							auto oldindexpos =Item.second + (i - oldstartpos);
							auto jumppos =Instruction::IsJump(Span(ByteCodeLayer->_Instructions.data(), ByteCodeLayer->_Instructions.size()),oldindexpos,is32mode).value();

							size_t newpos;
							{
								size_t diff = jumpto - startfunc;

							
								newpos = oldstartpos + diff;
							}

							InstructionBuilder::Jumpv1(newpos, NewIns[i + 0]);
							InstructionBuilder::Jumpv2(newpos, NewIns[i + 1]);
							
							if (is64mode)
							{
								InstructionBuilder::Jumpv3(newpos, NewIns[i + 2]);
								InstructionBuilder::Jumpv4(newpos, NewIns[i + 3]);
							}
						}
						else
						{
							UCodeLangToDo();
						}

							
					}
				}

				{
					auto p2 = Instruction::IsJumpIf(span, i, is32mode);
					if (p2.has_value())
					{
						auto jumpto = p2.value().Func;
						auto regto = p2.value().Reg;

						bool ispartofthisfunc = false;
						if (jumpto >= startfunc) 
						{
							ispartofthisfunc  = (jumpto - startfunc) < funcsize;
						}


						if (ispartofthisfunc)
						{
							auto oldindexpos =Item.second + (i - oldstartpos);
							auto jumppos = Instruction::IsJumpIf(Span(ByteCodeLayer->_Instructions.data(), ByteCodeLayer->_Instructions.size()), oldindexpos, is32mode).value().Func;

							size_t newpos;
							{
								size_t diff = jumpto - startfunc;

							
								newpos = oldstartpos + diff;
							}

								
							
							InstructionBuilder::Jumpv1(newpos, NewIns[i + 0]);
							
							if (is64mode) {
								InstructionBuilder::Jumpv2(newpos, NewIns[i + 1]);
							}
							else
							{
								InstructionBuilder::Jumpifv2(newpos,regto, NewIns[i + 1]);
							}

							if (is64mode) 
							{
								InstructionBuilder::Jumpv3(newpos, NewIns[i + 2]);
								InstructionBuilder::Jumpifv4(newpos,regto, NewIns[i + 3]);
							}
						}
						else
						{
							UCodeLangToDo();
						}
					}
				}
			}
		}
	}
	
	for (size_t i = 0; i < NewIns.size(); i++)
	{
		auto span = Span<Instruction>(NewIns.data(), NewIns.size());
		{
			auto v = Instruction::IsCall(span,i, is32mode);
			if (v.has_value())
			{
				UAddress tocall = v.value() +1;
				UAddress newcall = oldtonew.GetValue(tocall)-1;

				InstructionBuilder::Callv1(newcall, NewIns[i + 0]);
				InstructionBuilder::Callv2(newcall, NewIns[i + 1]);

				if (is64mode)
				{
					InstructionBuilder::Callv3(newcall, NewIns[i + 2]);
					InstructionBuilder::Callv4(newcall, NewIns[i + 3]);
				}
			}
		}

		{
			auto v = Instruction::IsLoadFuncPtr(span,i, is32mode);
			if (v.has_value())
			{
				RegisterID reg = NewIns[0].Op_RegUInt16.A;

				UAddress tocall = v.value()+1;
				UAddress newcall = oldtonew.GetValue(tocall)-1;

				InstructionBuilder::LoadFuncPtr_V1(newcall,reg, NewIns[i + 0]);
				InstructionBuilder::LoadFuncPtr_V2(newcall,reg, NewIns[i + 1]);

				if (is64mode)
				{
					InstructionBuilder::LoadFuncPtr_V3(newcall,reg, NewIns[i + 2]);
					InstructionBuilder::LoadFuncPtr_V4(newcall,reg, NewIns[i + 3]);
				}
			}
		}

	}


	ByteCodeLayer->_NameToPtr = std::move(NewNameToAddress);
	ByteCodeLayer->_Instructions = std::move(NewIns);

	if (setting.RemoveType)	
	{
		auto& list = lib._Assembly.Classes;

		size_t count = 0;
		do
		{
			count = list.size();
			list.erase(std::remove_if(list.begin(), list.end(), [&FuncionsToKeep,&r,&list](Unique_ptr<AssemblyNode>& node) mutable -> bool
			{
				bool removed = false;

				auto opid = ClassAssembly::GetReflectionTypeID(node.get());
			
				
				bool isused = true;
				if (opid.has_value())
				{
					auto id = opid.value();

					isused = false;
					for (auto& Item : list)
					{
						if (Item.get()) {
							isused = NodeDependsonType(Item.get(), id);
						}
						if (isused)
						{
							break;
						}
					}
				}
				else
				{
					isused = false;
				}

				if (!isused) 
				{
					removed = true;
					OptionalRef<Vector<ClassMethod>> OpMethods;

					switch (node->Get_Type())
					{
					case ClassType::Class:
						OpMethods = Optionalref(node->Get_ClassData().Methods);
						break;
					case ClassType::Tag:
						OpMethods = Optionalref(node->Get_TagData().Methods);
						break;
					default:
						break;
					}		

					if (OpMethods.has_value())
					{
						bool isinlist = false;
					
						for (auto& Item2 : FuncionsToKeep) 
						{
							for (auto& Item : OpMethods.value())
							{
								if (Item2 == &Item)
								{
									isinlist = true;
									break;
								}
							}
							if (isinlist) { break; }
						}

						if (isinlist)
						{
							removed =false;
						}
					}

				}
			
				if (removed)
				{
					r.RemovedTypes.push_back(std::move(node));
				}
				return removed;
			}),list.end());

		} while (count != list.size());

		int a = 0;
	}

	lib.Get_Assembly().Remove_NullFunc();

	return r;
}

bool UAssembly::NodeDependsonType(const AssemblyNode* node, ReflectionCustomTypeID id)
{
	bool isused = false;
	switch (node->Get_Type())
	{
	case ClassType::Class:
	{
		auto& data = node->Get_ClassData();


		for (auto& Item : data.Attributes.Attributes)
		{
			if (Item.TypeID == id)
			{
				isused = true;
				break;
			}
		}

		if (isused) { break; }
		for (auto& Item : data.Fields)
		{
			if (Item.Type._CustomTypeID == id)
			{
				isused = true;
				break;
			}
		}
		if (isused) { break; }

		for (auto& Item : data.InheritedTypes)
		{
			if (Item.TraitID == id)
			{
				isused = true;
				break;
			}
		}
		if (isused) { break; }

		for (auto& Item : data.Methods)
		{
			if (Item.RetType._CustomTypeID == id)
			{
				isused = true;
				break;
			}
			if (isused) { break; }
			for (auto& Par : Item.ParsType)
			{
				if (Par.Type._CustomTypeID == id)
				{
					isused = true;
				}
				break;
			}
			if (isused) { break; }
		}
		if (isused) { break; }
	}
	break;
	case ClassType::Enum:
	{
		auto& data = node->Get_EnumData();
		if (data.BaseType._CustomTypeID == id)
		{
			isused = true;
			break;
		}

		if (data.EnumVariantUnion.has_value())
		{
			auto& unioninfo = data.EnumVariantUnion.value();

			if (unioninfo == id)
			{
				isused =true;
				break;
			}
		}
	}
	break;
	case ClassType::Alias:
	{
		auto& data = node->Get_AliasData();
		if (data.Type._CustomTypeID == id)
		{
			isused = true;
			break;
		}
	}
	break;
	case ClassType::Tag:
	{
		auto& data = node->Get_TagData();

		for (auto& Item : data.Fields)
		{
			if (Item.Type._CustomTypeID == id)
			{
				isused = true;
				break;
			}
		}
		if (isused) { break; }

		for (auto& Item : data.Methods)
		{
			for (auto& Item2 : Item.Attributes.Attributes)
			{
				if (Item2.TypeID == id)
				{
					isused = true;
					break;
				}
			}
			if (Item.RetType._CustomTypeID == id)
			{
				isused = true;
				break;
			}
			if (isused) { break; }
			for (auto& Par : Item.ParsType)
			{
				if (Par.Type._CustomTypeID == id)
				{
					isused = true;
				}
				break;
			}
			if (isused) { break; }
		}
		if (isused) { break; }

	}
	break;
	case ClassType::Trait:
	{
		auto& data = node->Get_TraitData();

		for (auto& Item : data.Fields)
		{
			if (Item.Type._CustomTypeID == id)
			{
				isused = true;
				break;
			}
		}
		if (isused) { break; }

		for (auto& Item : data.Methods)
		{
			for (auto& Item2 : Item.method.Attributes.Attributes)
			{
				if (Item2.TypeID == id)
				{
					isused = true;
					break;
				}
			}
		
			if (isused) { break; }
			if (Item.method.RetType._CustomTypeID == id)
			{
				isused = true;
				break;
			}
			if (isused) { break; }
			for (auto& Par : Item.method.ParsType)
			{
				if (Par.Type._CustomTypeID == id)
				{
					isused = true;
				}
				break;
			}
			if (isused) { break; }
		}
		if (isused) { break; }

	}
	break;
	case ClassType::Eval:
	{
		auto& data = node->Get_EvalData();

		if (data.Value._Type._CustomTypeID == id)
		{
			isused = true;
		}
	}
	break;
	case ClassType::FuncPtr:
	{
		auto& data = node->Get_FuncPtr();

		if (data.RetType._CustomTypeID == id)
		{
			isused = true;
		}

		if (isused)
		{
			break;
		}

		for (auto& Item : data.ParsType)
		{
			if (Item.Type._CustomTypeID == id)
			{
				isused = true;
				break;
			}
		}	
	}
	break;
	case ClassType::StaticArray:
	{
		auto& data = node->Get_StaticArray();

		if (data.BaseType._CustomTypeID == id)
		{
			isused = true;
		}
	}
	break;
	case ClassType::ForType:
	{
		auto& data = node->Get_ForType();

		if (data._TargetType._CustomTypeID == id)
		{
			isused = true;
		}
		else
		{
			for (auto& Item : data._AddedMethods)
			{
				for (auto& Item2 : Item.Attributes.Attributes)
				{
					if (Item2.TypeID == id)
					{
						isused = true;
						break;
					}
				}
				if (isused) { break; }
				if (Item.RetType._CustomTypeID == id)
				{
					isused = true;
					break;
				}
				if (isused) { break; }
				for (auto& Par : Item.ParsType)
				{
					if (Par.Type._CustomTypeID == id)
					{
						isused = true;
					}
					break;
				}
				if (isused) { break; }
			}
		}
	}
	break;
	case ClassType::NameSpace:
	case ClassType::GenericClass:
	case ClassType::GenericFunction:
		break;
	default:
		UCodeLangUnreachable();
		break;
	}
	return isused;
}

UAssemblyEnd


#endif