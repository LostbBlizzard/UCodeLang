#include "UAssembly.hpp"
#include "CompilerTypes.hpp"
#include "UCodeLang/LangCore/DataType/BinaryVectorMap.hpp"
#include "UCodeLang/Compliation/Helpers/NameDecoratior.hpp"

#include <fstream>
#include "UCodeLang/Compliation/Middle/IR.hpp"
#define StackName "Stack"
#define StackName_ "[" + StackName + "]"

UAssemblyStart
struct OutputIRLineState
{
	IRBuilder::ToStringState State;
	UCodeLang::BinaryVectorMap<UCodeLang::IRidentifierID, String> Names;
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
										r += "//Varable:" + Val->VarableName;
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
void UAssembly::Assemble(const String_view& Text, UClib* Out)
{
	Lexer Lex; 
	Parser Parse; 
	Lex.Set_ErrorsOutput(_ErrorsOutput);
	Lex.Set_Settings(_Settings);
	Parse.Set_ErrorsOutput(_ErrorsOutput);
	Parse.Set_Settings(_Settings);
	
	
	Lex.Lex(Text);
	Parse.Parse(Lex.Get_Output(), Out);
}
String UAssembly::ToString(const UClib* Lib, Optional<Path> SourceFiles, bool ShowIR)
{
	auto& InsMapData = Get_InsToInsMapValue();
    String r;
	BinaryVectorMap<UAddress, String> AddressToName;

	VectorMap<String, Vector<String>> OpenedSourceFilesLines;
	String OnFile;

	auto UCodeLayer = Lib->GetLayer(UCode_CodeLayer_UCodeVM_Name);
	
	if (UCodeLayer && UCodeLayer->_Data.Is<CodeLayer::UCodeByteCode>())
	{
		const CodeLayer::UCodeByteCode& Info = UCodeLayer->_Data.Get<CodeLayer::UCodeByteCode>();
		for (const auto& Item2 : Info._NameToPtr)
		{
			AddressToName[Item2._Value] = Item2._Key;
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
		case ClassType::Class:
		{
			auto& Class = Item->Get_ClassData();

			for (auto Item2 : Class.Attributes.Attributes)
			{
				ToString(Item2,Assembly);
			}
			r += "$" + Item->FullName + ":\n";
			

			r += ".size:" + std::to_string(Class.Size) + "\n";
			
			for (auto Item2 : Class.Fields)
			{
				r += " " + ToString(Item2.Type, Assembly) + " " + Item2.Name + ";//Offset " + std::to_string(Item2.offset) + "\n";
			}
			r += "\n";

			for (auto Item2 : Class.Methods)
			{
				for (auto Item3 : Item2.Attributes.Attributes)
				{
					r += ToString(Item3, Assembly);
				}

				r += " |" + Item2.FullName + "[";
				for (auto& Item3 : Item2.ParsType)
				{
					r += ToString(Item3,Assembly);
				}
				r += "] -> " + ToString(Item2.RetType, Assembly) + ";" ;
				r += "//" + Item2.DecorationName + '\n';
					
			}
			r += "\n\n";
		}
		break; 
		case ClassType::Alias:
		{
			auto& Class = Item->Get_AliasData();
			r += "$" + Item->FullName + " = " + ToString(Class.Type, Assembly) + ";\n\n";
		}
		break;
		case ClassType::Enum:
		{
			auto& Enum = Item->Get_EnumData();
			r += "$" + Item->FullName + " enum[" + ToString(Enum.BaseType,Assembly) + "]:\n";
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
		default:
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
		BinaryVectorMap<IRidentifierID, OutputIRLineState> IRStringStates;
		for (size_t i = 0; i < Insts.size(); i++)
		{
			auto& Item = Insts[i];
			UAddress address = (UAddress)i;
			if (AddressToName.count(address))
			{
				String Name = AddressToName[address];
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

							String* ItemValue=nullptr;
							if (OpenedSourceFilesLines.HasValue(OnFile))
							{
								auto& Item = OpenedSourceFilesLines.at(OnFile);

								if (Val->LineNumber-1 < Item.size()) {
									ItemValue = &Item[Val->LineNumber - 1];
								}
							}
							else
							{
								std::ifstream file(SourceFiles.value().native() + Path(OnFile).native());
								if (file.is_open())
								{
									std::string str;
									Vector<String> Lines;
									while (std::getline(file, str))
									{
										Lines.push_back(std::move(str));
									}
									OpenedSourceFilesLines.AddValue(OnFile,std::move(Lines));
								
									auto& Item = OpenedSourceFilesLines.at(OnFile);

									if (Val->LineNumber-1 < Item.size()) {
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
								OutputIRLineInfo(&IRInfoVal,&IRInfoVal._StaticInit, Val, IRStringStates.at(Id), r);
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
								OutputIRLineInfo(&IRInfoVal, &IRInfoVal._threadInit, Val, IRStringStates.at(Id), r);
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
								OutputIRLineInfo(&IRInfoVal, &IRInfoVal._StaticdeInit, Val, IRStringStates.at(Id), r);
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
								OutputIRLineInfo(&IRInfoVal, &IRInfoVal._threaddeInit, Val, IRStringStates.at(Id), r);
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
										OutputIRLineInfo(&IRInfoVal, Func.get(), Val, IRStringStates.at(Id), r);
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
					else if (Info.DebugInfo.has_value())
					{
						auto& Value = Info.DebugInfo.value();
						auto List = Value.GetForIns(i);
						for (auto& Item : List)
						{
							if (auto Val = Item->Debug.Get_If<UDebugSetVarableLoc>())
							{
								r += "   //";

								if (auto Value = Val->Type.Get_If<RegisterID>())
								{
									r += GetRegisterToString(*Value);
								}
								else
								{
									throw std::exception("not added");
								}
								r += " = ";
								r += Val->VarableFullName;
								r += '\n';
							}
						}
					}
					
				}
				
				

				if (List.size())
				{
					r += '\n';
				}
			}

			r += "   " + std::to_string(i) + " :";

			if (InsMapData.count(Item.OpCode))
			{
				auto& MapData = InsMapData[Item.OpCode];
				r += (String)MapData->InsName;
				r += " ";

				if (MapData->Op_0 != OpCodeType::NoOpCode)
				{
					OpValueToString(MapData->Op_0, Item.Value0, AddressToName, r, Lib);
				}
				if (MapData->Op_1 != OpCodeType::NoOpCode)
				{
					r += ",";
					OpValueToString(MapData->Op_1, Item.Value1, AddressToName, r, Lib);
				}

			}
			else
			{
				r += "Ins " + std::to_string((uintptr_t)Item.OpCode) + ":" + std::to_string((uintptr_t)Item.Value0.AsPtr) + ","
					+ std::to_string((uintptr_t)Item.Value1.AsPtr);
			}



			r += '\n';
		}
	}

    return r;
}

String UAssembly::ToString(const ReflectionTypeInfo& Value, const ClassAssembly& Assembly)
{
	String r;
	if (Value.Isimmutable())
	{
		r = "umut ";
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
	default:
		break;
	}
	return r;
}
String UAssembly::ToString(const UsedTagValueData& Value, const ClassAssembly& Assembly)
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
void UAssembly::OpValueToString(OpCodeType OpType,const AnyInt64& In,const BinaryVectorMap<UAddress, String>& AddressToName, String& out, const UClib* Lib)
{

	switch (OpType)
	{
	case OpCodeType::NoOpCode:
		break;
	case OpCodeType::AnyInt8:
	{
		String tepS = std::to_string((UInt64)In.AsUInt8);
		String teps2 = std::to_string((Int64)In.AsInt8);
		out += (tepS == teps2) ? tepS : tepS + "|" + teps2;
	}	
	break;
	case OpCodeType::AnyInt16:
	{
		String tepS = std::to_string((UInt64)In.AsUInt16);
		String teps2 = std::to_string((Int64)In.AsInt16);
		out += (tepS == teps2) ? tepS : tepS + "|" + teps2;
	}
	break;
	case OpCodeType::AnyInt32:
	{
		String tepS = std::to_string((UInt64)In.AsUInt32);
		String teps2 = std::to_string((Int64)In.AsInt32);
		out += (tepS == teps2) ? tepS : tepS + "|" + teps2;
	}	
	break;
	case OpCodeType::AnyInt64:
	{
		String tepS = std::to_string((UInt64)In.AsUInt64);
		String teps2 = std::to_string((Int64)In.AsInt64);
		out += (tepS == teps2) ? tepS : tepS + "|" + teps2;
	}	
	break;

	case OpCodeType::Anyfloat32:
		out += std::to_string(In.Asfloat32);
		break;
	case OpCodeType::Anyfloat64:
		out += std::to_string(In.Asfloat64);
		break;

	case OpCodeType::Register:
		out += GetRegisterToString(In.AsRegister);
		break;
	case OpCodeType::UIntPtr:
		out += std::to_string(In.AsUInt64);
		break;
		
	case OpCodeType::StaticCString:
		out += "\"" + (String)(const char*)&Lib->Get_StaticBytes()[In.AsUIntNative] + "\"";
		break;

	case OpCodeType::InsAddress:
	{
		auto NewAddress = In.AsAddress + 1;
		if (AddressToName.count(NewAddress))
		{
			out += "{" + AddressToName.at(NewAddress) + "}";
		}
		else
		{
			out += "{" + std::to_string(In.AsUInt64) + "}";
		}
	}	
	break;
	case OpCodeType::RegPtrAndRegOut:
	{
		const RegisterID* ReV = &In.AsRegister;
		RegisterID Ptr = ReV[0];
		RegisterID RegOut = ReV[1];
		out += "[Ptr:" + GetRegisterToString(Ptr) + "]," + "[Target:" + GetRegisterToString(RegOut) + "]";
	}
		
		break;
	default:
		break;
	}
}
size_t UAssembly::BuildHashForSub(const Instruction* Pointer, size_t BufferSize)
{
	String_view Item((char*)Pointer, BufferSize * sizeof(Instruction));

	return std::hash<String_view>()(Item);
}
UAssemblyEnd
