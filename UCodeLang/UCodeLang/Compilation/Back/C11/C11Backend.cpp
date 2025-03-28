#ifndef UCodeLangNoCompiler
#include "C11Backend.hpp"
#include "../../Middle/IR.hpp"
#include "../../CompilationSettings.hpp"
#include "../../Helpers/CompilationErrors.hpp"
#include "UCodeLang/LangCore/UClib.hpp"
UCodeLangStart




#define IRFuncEntryPointName "_Entry"


#define IRPrefix "UCIR_"

#define IRMDefinesName IRPrefix "_C89Defs"

#define IRMSVCDefineName IRPrefix "Compiler_MSVC"
#define IRCLangDefineName IRPrefix "Compiler_Clang"
#define IRGNUCDefineName IRPrefix "Compiler_GCC"
#define IREmscriptenDefineName IRPrefix "Compiler_Emscripten"

#define IRWindowsDefineName IRPrefix "Platform_Windows"
#define IRLinuxDefineName IRPrefix "Platform_Linux"
#define IRMacOSDefineName IRPrefix "Platform_MacOS"
#define IRANDROIDDefineName IRPrefix "Platform_ANDROID"
#define IRIPHONEDefineName IRPrefix "Platform_IPHONE"
#define IRWasmDefineName IRPrefix "Platform_Wasm"

#define IRForceinlineDefineName IRPrefix "Forceinline"

#define IRUnreachableDefineName IRPrefix "Unreachable"

#define IRAssumeDefineName IRPrefix "Assume"

#define IRhreadLocal "ThreadLocalKeyWord"

#define IRReturnValue "_ReturnVal"

#define IRReinterpretCastTep "_tep"

C11Backend::C11Backend()
{

}
C11Backend::~C11Backend()
{

}
void C11Backend::Reset()
{

	this->~C11Backend();
	new (this)  C11Backend;
}
void C11Backend::Build(const IRBuilder* Input)
{
	_Input = Input;

	if (Get_Settings()._Type == OutPutType::IRAndSymbols)
	{
		if (HasLibOutput())
		{
			auto _OutLayer = Getliboutput().AddLayer(UCode_CodeLayer_IR_Name);
			CodeLayer::JustData V;
			V._Data = Input->ToBytes().MoveToVector();
			_OutLayer->_Data = std::move(V);

		}
		else
		{
			OutBuffer += "/*Made using UCodeLang C89 Backend.Next Part is an IR Binary this will be used when linking.*/";

			auto Data = Input->ToBytes();
			OutBuffer += String_view((char*)Data.Data(), Data.Size());

			Set_Output(OutBuffer);
		}
	}
	else
	{

		{//Flags 
			Flag_NoExceptions = Get_Settings().HasArg("NoExceptions");
			Flag_CPPCodeAllowed = Get_Settings().HasArg("AllowCpp");

		}
		AddTextSignature();


		if (Flag_CPPCodeAllowed)
		{
			String NameSpace = "CompiledULang";

			OutBuffer += "namespace ";
			OutBuffer += NameSpace;
			OutBuffer += " {";
		}

		//types
		AddBaseTypes();


		//in C89 need to reoder all struct types definitions befor there used

		OutBuffer += ToString();


		if (Flag_CPPCodeAllowed)
		{
			OutBuffer += "\n\n}";
		}

		Set_Output(OutBuffer);
	}
}

void C11Backend::AddTextSignature()
{
	OutBuffer += "/*Made using UCodeLang C89 Backend.\n";
	OutBuffer += "\n";
	OutBuffer += "\n";
	OutBuffer += "*/\n";
}

void C11Backend::AddBaseTypes()
{

	OutBuffer += "/*defs*/\n\n";

	OutBuffer += "#if defined(_MSC_VER)\n";
	OutBuffer += "#define " + (String)IRMSVCDefineName + " 1 \n";
	OutBuffer += "#else\n";
	OutBuffer += "#define " + (String)IRMSVCDefineName + " 0 \n";
	OutBuffer += "#endif\n\n";

	OutBuffer += "#if defined(__GNUC__)\n";
	OutBuffer += "#define " + (String)IRGNUCDefineName + " 1 \n";
	OutBuffer += "#else\n";
	OutBuffer += "#define " + (String)IRGNUCDefineName + " 0 \n";
	OutBuffer += "#endif\n\n";

	OutBuffer += "#if defined(__clang__)\n";
	OutBuffer += "#define " + (String)IRCLangDefineName + " 1 \n";
	OutBuffer += "#else\n";
	OutBuffer += "#define " + (String)IRCLangDefineName + " 0 \n";
	OutBuffer += "#endif\n\n";

	OutBuffer += "#if _WIN64 || _WIN32\n";
	OutBuffer += "#define " + (String)IRWindowsDefineName + " 1 \n";
	OutBuffer += "#else\n";
	OutBuffer += "#define " + (String)IRWindowsDefineName + " 0 \n";
	OutBuffer += "#endif\n\n";


	OutBuffer += "#if __gnu_linux__ || __linux__\n";
	OutBuffer += "#define " + (String)IRLinuxDefineName + " 1 \n";
	OutBuffer += "#else\n";
	OutBuffer += "#define " + (String)IRLinuxDefineName + " 0 \n";
	OutBuffer += "#endif\n\n";

	OutBuffer += "#if __APPLE__ && __MACH__\n";
	OutBuffer += "#define " + (String)IRMacOSDefineName + " 1 \n";
	OutBuffer += "#else\n";
	OutBuffer += "#define " + (String)IRMacOSDefineName + " 0 \n";
	OutBuffer += "#endif\n\n";

	OutBuffer += "#if __APPLE__\n";
	OutBuffer += "#include \"TargetConditionals.h\"\n";

	OutBuffer += "#if TARGET_OS_IPHONE\n";
	OutBuffer += "#define " + (String)IRIPHONEDefineName + " 1 \n";
	OutBuffer += "#else\n";
	OutBuffer += "#define " + (String)IRIPHONEDefineName + " 0 \n";
	OutBuffer += "#endif\n";

	OutBuffer += "#else\n";
	OutBuffer += "#define UCodeLang_Platform_IPHONE 0\n";
	OutBuffer += "#endif\n\n";

	OutBuffer += "#if __ANDROID__\n";
	OutBuffer += "#define " + (String)IRANDROIDDefineName + " 1 \n";
	OutBuffer += "#else\n";
	OutBuffer += "#define " + (String)IRANDROIDDefineName + " 0 \n";
	OutBuffer += "#endif\n\n";

	OutBuffer += "#if defined(__wasm32__) || defined(__wasm64__)\n";
	OutBuffer += "#define " + (String)IRWasmDefineName + " 1 \n";
	OutBuffer += "#else\n";
	OutBuffer += "#define " + (String)IRWasmDefineName + " 0 \n";
	OutBuffer += "#endif\n\n";

	OutBuffer += "#if " + (String)IRMSVCDefineName + "\n";
	OutBuffer += "#define " + (String)IRForceinlineDefineName + " __forceinline\n";
	OutBuffer += "#elif " + (String)IRGNUCDefineName + "\n";
	OutBuffer += "#define " + (String)IRForceinlineDefineName + " __attribute__((always_inline))\n";
	OutBuffer += "#else\n";
	OutBuffer += "#define " + (String)IRForceinlineDefineName + " inline\n";
	OutBuffer += "#endif\n\n";

	OutBuffer += "#ifdef __cplusplus\n";
	OutBuffer += "#define " + (String)IRhreadLocal + " thread_local\n";
	OutBuffer += "#else \n";
	OutBuffer += "#define " + (String)IRhreadLocal + " _Thread_local\n";
	OutBuffer += "#endif\n";
	//OutBuffer += "#elif " + (String)IRGNUCDefineName + "\n";
	//OutBuffer += "#define " + (String)IRThreadLocal  + " __attribute__((always_inline))\n";
	//OutBuffer += "#else\n";
	//OutBuffer += "#define " + (String)IRForceinlineDefineName + " inline\n";
	//OutBuffer += "#endif\n\n";

	OutBuffer += "#if " + (String)IRMSVCDefineName + "\n";
	OutBuffer += "#define " + (String)IRUnreachableDefineName + " __assume(0)\n";
	OutBuffer += "#elif " + (String)IRGNUCDefineName + "\n";
	OutBuffer += "#define " + (String)IRUnreachableDefineName + " __builtin_unreachable()\n";
	OutBuffer += "#else\n";
	OutBuffer += "#define " + (String)IRUnreachableDefineName + " \n";
	OutBuffer += "#endif\n\n";


	OutBuffer += "#if " + (String)IRMSVCDefineName + "\n";
	OutBuffer += "#define " + (String)IRAssumeDefineName + "(X) __assume(X)\n";
	OutBuffer += "#elif " + (String)IRCLangDefineName + "\n";
	OutBuffer += "#define " + (String)IRAssumeDefineName + "(X) __builtin_assume(X)\n";
	OutBuffer += "#else\n";
	OutBuffer += "#define " + (String)IRAssumeDefineName + "(X) \n";
	OutBuffer += "#endif\n\n";

	OutBuffer += "/*includes*/\n\n";

	OutBuffer += "#include <inttypes.h>\n";
	OutBuffer += "#include <stdlib.h>\n";
	OutBuffer += "#include <stdio.h>\n";
	OutBuffer += "#include <string.h>\n";

	if (Flag_CPPCodeAllowed)
	{
		if (Flag_NoExceptions == false)
		{
			bool hasexexceptions = true;

			OutBuffer += "#include <exception>\n";
		}
	}
	OutBuffer += "/*Types*/\n";
	OutBuffer += "typedef float float32_t;\n";
	OutBuffer += "typedef double float64_t;\n";


	OutBuffer += '\n';
	OutBuffer += "void ULangNoOp();";
	OutBuffer += '\n';

}

String C11Backend::ToString(const IRType& Type, bool IsUnsigned)

{
	switch (Type._Type)
	{
	case IRTypes::i8:return IsUnsigned ? "uint8_t" : "int8_t";
	case IRTypes::i16:return IsUnsigned ? "uint16_t" : "int16_t";
	case IRTypes::i32:return IsUnsigned ? "uint32_t" : "int32_t";
	case IRTypes::i64:return IsUnsigned ? "uint64_t" : "int64_t";
	case IRTypes::f32:return "float32_t";
	case IRTypes::f64:return "float64_t";


	case IRTypes::Void:return "void";

	case IRTypes::pointer:
	{
		String r;
		auto Syb = _Input->GetSymbol(Type._symbol);
		if (Syb)
		{
			r += FromIDToCindentifier(Syb->identifier) + "*";
		}
		else {
			r += "void*";
		}
		return r;
	}
	break;
	case IRTypes::IRsymbol:
	{
		auto Syb = _Input->GetSymbol(Type._symbol);
		if (Syb)
		{
			return FromIDToCindentifier(Syb->identifier);
		}
	}
	break;
	default:
		UCodeLangUnreachable();
		break;
	}
	return "void";
}

String C11Backend::ToString()
{
	String r;
	ToStringState State;

	r += "\n\n/*file.h*/\n\n";

	AddSybToString(r);
	r += "\n";


	{
		ToString(r, &_Input->_StaticInit, State);
		ToString(r, &_Input->_StaticdeInit, State);
		ToString(r, &_Input->_threadInit, State);
		ToString(r, &_Input->_threaddeInit, State);

		for (auto& Item : _Input->Funcs)
		{

			_Func = Item.get();
			ToString(r, Item.get(), State);
		}

		if (_Input->EntryPoint.has_value())
		{
			r += "int main(int argc, char** argv);";
		}
	}
	r += "\n\n/*file.cpp*/\n\n";
	{
		{
			r += "void ULangNoOp(){}\n";

			ToString(r, &_Input->_StaticInit, State, true);
			ToString(r, &_Input->_StaticdeInit, State, true);
			ToString(r, &_Input->_threadInit, State, true);
			ToString(r, &_Input->_threaddeInit, State, true);

			for (auto& Item : _Input->Funcs)
			{

				_Func = Item.get();


				if (_Func->Linkage == IRFuncLink::StaticLink) {
					ToString(r, Item.get(), State, true);
				}

			}

			if (_Input->EntryPoint.has_value())
			{
				r += "int main(int argc, char** argv)\n";

				r += "{\n";

				r += FromIDToCindentifier(_Input->_StaticInit.identifier) + "();\n";
				r += FromIDToCindentifier(_Input->_threadInit.identifier) + "();\n\n";

				auto IsVoid = _Input->GetFunc(_Input->EntryPoint.value())->ReturnType._Type == IRTypes::Void;

				if (!IsVoid)
				{
					r += "int exitcode = ";
				}

				r += IRFuncEntryPointName;
				r += "();\n";

				r += +"\n" + FromIDToCindentifier(_Input->_threaddeInit.identifier) + "();\n";
				r += FromIDToCindentifier(_Input->_StaticdeInit.identifier) + "();\n";

				if (!IsVoid)
				{
					r += "\n return exitcode;\n";
				}
				else
				{
					r += "\n return 0;\n";
				}
				r += "}";
			}
		}
	}

	return r;
}

void C11Backend::AddSybToString(UCodeLang::String& r)
{
	UnorderedMap<IRidentifierID, bool> Vals;
	Vals.reserve(_Input->_Symbols.size());

	for (auto& Item : _Input->_Symbols)
	{
		Vals.AddValue(Item->identifier, false);
	}
	size_t Values = _Input->_Symbols.size();


	while (Values != 0)
	{


		for (auto& Item : _Input->_Symbols)
		{
			if (Vals.GetValue(Item->identifier) == false)
			{

				String SybName = FromIDToCindentifier(Item->identifier);
				switch (Item->SymType)
				{
				case IRSymbolType::FuncPtr:
				{
					IRFuncPtr* V = Item->Get_ExAs<IRFuncPtr>();

					for (auto& Item2 : V->Pars)
					{
						if (Item2._Type == IRTypes::IRsymbol || Item2._Type == IRTypes::pointer)
						{
							if (Vals.HasValue(Item2._symbol.ID))
							{
								if (Vals.GetValue(Item2._symbol.ID) == false)
								{

									goto NextMainLoop;
								}
							}
						}
					}

					r += "typedef ";
					r += ToString(V->Ret);
					r += "(*" + SybName + ")";
					r += "(";

					for (auto& Item2 : V->Pars)
					{
						r += ToString(Item2);
						if (&Item2 != &V->Pars.back())
						{
							r += ",";
						}
					}

					r += ")";
					r += ";\n\n";

				}
				break;
				case IRSymbolType::Struct:
				{
					IRStruct* V = Item->Get_ExAs<IRStruct>();

					for (size_t i = 0; i < V->Fields.size(); i++)
					{
						auto& Item2 = V->Fields[i].Type;
						if (Item2._Type == IRTypes::IRsymbol || Item2._Type == IRTypes::pointer)
						{
							if (Vals.HasValue(Item2._symbol.ID))
							{
								if (Vals.GetValue(Item2._symbol.ID) == false)
								{

									goto NextMainLoop;
								}
							}
						}
					}

					r += "typedef ";

					if (V->IsUnion)
					{
						r += "union";
					}
					else
					{
						r += "struct";
					}

					r += " \n{\n";

					for (size_t i = 0; i < V->Fields.size(); i++)
					{
						r += " " + ToString(V->Fields[i].Type) + " __" + std::to_string(i) + "; \n";
					}
					if (V->Fields.size() == 0)
					{
						r += " int __Empty; /*C requires that a struct or union has at least one member*/";
					}

					r += "\n} " + SybName + ";\n\n";
				}
				break;
				case IRSymbolType::StaticArray:
				{
					IRStaticArray* V = Item->Get_ExAs<IRStaticArray>();

					{
						auto& Item2 = V->Type;
						if (Item2._Type == IRTypes::IRsymbol || Item2._Type == IRTypes::pointer)
						{
							if (Vals.HasValue(Item2._symbol.ID))
							{
								if (Vals.GetValue(Item2._symbol.ID) == false)
								{

									goto NextMainLoop;
								}
							}
						}
					}

					r += "typedef struct ";
					r += SybName + " {";
					r += ToString(V->Type);
					r += " base[";
					r += std::to_string(V->Count) + "];";
					r += " } ";
					r += SybName;
					r += ";\n\n";
				}
				break;
				case IRSymbolType::StaticVarable:
				{
					IRBufferData* V = Item->Get_ExAs<IRBufferData>();

					{
						auto& Item2 = Item->Type;
						if (Item2._Type == IRTypes::IRsymbol || Item2._Type == IRTypes::pointer)
						{
							if (Vals.HasValue(Item2._symbol.ID))
							{
								if (Vals.GetValue(Item2._symbol.ID) == false)
								{
									goto NextMainLoop;
								}
							}
						}
					}


					r += "static " + ToString(Item->Type);

					r += " " + SybName;
					if (V->IsInitialized && V->Bytes.size())
					{
						r += "[]";
						r += " = ";

						r += "{";
						for (auto& Item : V->Bytes)
						{
							r += std::to_string((int)Item);
							if (&Item != &V->Bytes.back())
							{
								r += ",";
							}
						}
						r += "}";
					}

					UpdateCppLinks(r, V);
				}
				break;
				case IRSymbolType::ThreadLocalVarable:
				{
					IRBufferData* V = Item->Get_ExAs<IRBufferData>();

					{
						auto& Item2 = Item->Type;
						if (Item2._Type == IRTypes::IRsymbol || Item2._Type == IRTypes::pointer)
						{
							if (Vals.HasValue(Item2._symbol.ID))
							{
								if (Vals.GetValue(Item2._symbol.ID) == false)
								{
									continue;
								}
							}
						}
					}

					r += IRhreadLocal + (String)" " + ToString(Item->Type);

					r += " " + SybName;

					if (V->IsInitialized && V->Bytes.size())
					{
						r += "[]";
						r += " = ";

						r += "{";
						for (auto& Item : V->Bytes)
						{
							r += std::to_string((int)Item);
							if (&Item != &V->Bytes.back())
							{
								r += ",";
							}
						}
						r += "}";
					}

					UpdateCppLinks(r, V);
				}
				break;
				default:
					UCodeLangUnreachable();//Ptr was not set
					break;
				}

				Vals.GetValue(Item->identifier) = true;
				Values--;
			NextMainLoop:
				;
			}
		}
	}
}





void C11Backend::UpdateCppLinks(UCodeLang::String& r, UCodeLang::IRBufferData* V)
{
	//String_view text = String_view((char*)V->Bytes.data(), V->Bytes.size() - 1);
	bool IsCString = false;// V->Bytes.size() && _Input->ConstStaticStrings.HasValue(text);

	r += ";\n";
	return;

	if (IsCString)
	{
		r += ";";
		//r += "=" + (String)text;
	}
	else {
		r += "= {";

		for (auto& Item2 : V->Bytes)
		{
			r += std::to_string(Item2);

			if (&Item2 != &V->Bytes.back()) {
				r += ",";
			}
		}

		r += "};\n";
	}
}

const char* includedfuncions[] = {
 "malloc",
 "free",
 "memcpy",
 "memmove",
 "putchar",
};
constexpr size_t includefuncions_size = sizeof(includedfuncions) / sizeof(includedfuncions[0]);
void C11Backend::ToString(UCodeLang::String& r, const IRFunc* Item, UCodeLang::C11Backend::ToStringState& State, bool OutputBody)
{
	{
		auto str = FromIDToCindentifier(Item->identifier);
		for (size_t i = 0; i < includefuncions_size; i++)
		{
			auto item = includedfuncions[i];
			if (str == item)
			{
				return;
			}
		}
	}

	if (_Input->EntryPoint.has_value() && Item->identifier == _Input->EntryPoint.value())
	{

	}
	r += ToString(Item->ReturnType) + " ";

	if (_Input->EntryPoint.has_value() && Item->identifier == _Input->EntryPoint.value())
	{
		r += IRFuncEntryPointName;
	}
	else
	{
		r += FromIDToCindentifier(Item->identifier);
	}
	r += "(";
	for (auto& Par : Item->Pars)
	{
		r += ToString(Par.type) + " " + FromIDToCindentifier(Par.identifier);
		if (&Par != &Item->Pars.back())
		{
			r += ",";
		}
	}
	r += ")";

	State.PointerToName.clear();
	State.TepPushedParameters.clear();
	State.Num = 0;
	if (OutputBody)
	{

		r += "\n{";
		String Tabs = " ";

		if (Item->ReturnType != IRTypes::Void)
		{
			r += "\n ";
			r += ToString(Item->ReturnType) + " " + (String)IRReturnValue ";";
			r += "\n";
		}

		for (auto& Block : Item->Blocks)
		{

			r += Tabs + "/*Block*/ \n";



			UnorderedMap<size_t, String> Names;
			for (size_t i = 0; i < Block->Instructions.size(); i++)
			{




				auto& I = Block->Instructions[i];
				switch (I->Type)
				{
				case IRInstructionType::Jump:

				case IRInstructionType::ConditionalJump:
					if (!Names.HasValue(I->Target().identifier))
					{
						auto LabelName = "_label" + std::to_string(Names.size());
						Names.AddValue(I->Target().identifier, LabelName);
					}
					break;
				}
			}

			for (size_t i = 0; i < Block->Instructions.size(); i++)
			{
				auto& I = Block->Instructions[i];

				auto DebugInfo = Block->DebugInfo.Get_debugfor(i);
				for (auto& Item : DebugInfo)
				{
					if (auto Val = Item->Debug.Get_If<IRDebugSetFile>())
					{
						r += '\n';
						r += " /*File:" + Val->FileName;
						r += "*/";
					}
					else if (auto Val = Item->Debug.Get_If<IRDebugSetLineNumber>())
					{
						r += '\n';
						r += " /*Line:" + std::to_string(Val->LineNumber);
						r += "*/";
					}
				}
				bool hasname = false;
				for (auto& Item : Names)
				{
					if (Item.first == i)
					{
						r += "\n";
						r += Tabs;
						r += Item.second + ":";
						r += '\n';
						r += "ULangNoOp();";
						hasname = true;
					}
				}
				if (DebugInfo.size() || hasname)
				{
					r += '\n';
					r += '\n';
				}


				if (I->Type == IRInstructionType::None) { continue; }
				if (I->Type != IRInstructionType::PushParameter)
				{
					r += Tabs;
				}


				OutType = &I->ObjectType;
				IRType tep;
				switch (I->Type)
				{
				case IRInstructionType::LoadReturn:
				{

					bool docast = _Func->ReturnType != _Input->GetType(I.get(), I->Target());
					r += (String)IRReturnValue + " = ";
					if (docast)
					{
						r += "*(" + ToString(_Func->ReturnType) + "*)(&";
					}
					r += ToString(State, *I, I->Target());
					if (docast)
					{
						r += ")";
					}
				}break;
				case IRInstructionType::LoadNone:
					r += ToString(I->ObjectType);
					r += " " + State.GetName(I.get());
					break;
				case IRInstructionType::Load:
				{
					r += ToString(I->ObjectType);
					r += " " + State.GetName(I.get());
					r += " = ";

					bool docast = I->ObjectType != _Input->GetType(I.get(), I->Target());

					if (docast)
					{
						r += "*(" + ToString(I->ObjectType) + "*)(&";
					}
					r += ToString(State, *I, I->Target());

					if (docast)
					{
						r += ")";
					}
				}
				break;
				case IRInstructionType::Reassign:
					r += ToString(State, *I, I->Target());
					r += " = " + ToString(State, *I, I->Input());
					break;
				case IRInstructionType::Reassign_dereference:
					r += "*(";
					r += ToString(I->ObjectType);
					r += "*)";
					r += ToString(State, *I, I->Target());
					r += " = " + ToString(State, *I, I->Input());
					break;
				case IRInstructionType::Add:
					tep = *OutType;
					OutType = &tep;
					if (tep._Type == IRTypes::pointer)
					{
						tep = Get_Settings().PtrSize == IntSizes::Int64 ? IRTypes::i64 : IRTypes::i32;
					}

					r += ToStringBinary(State, I.get(), "+");
					break;
				case IRInstructionType::Sub:
					tep = *OutType;
					OutType = &tep;
					if (tep._Type == IRTypes::pointer)
					{
						tep = Get_Settings().PtrSize == IntSizes::Int64 ? IRTypes::i64 : IRTypes::i32;
					}

					r += ToStringBinary(State, I.get(), "-");
					break;
				case IRInstructionType::UMult:
				case IRInstructionType::SMult:
					tep = *OutType;
					OutType = &tep;
					if (tep._Type == IRTypes::pointer)
					{
						tep = Get_Settings().PtrSize == IntSizes::Int64 ? IRTypes::i64 : IRTypes::i32;
					}

					r += ToStringBinary(State, I.get(), "*");
					break;
				case IRInstructionType::SDiv:
				case IRInstructionType::UDiv:
					tep = *OutType;
					OutType = &tep;
					if (tep._Type == IRTypes::pointer)
					{
						tep = Get_Settings().PtrSize == IntSizes::Int64 ? IRTypes::i64 : IRTypes::i32;
					}

					r += ToStringBinary(State, I.get(), "/");
					break;
				case IRInstructionType::EqualTo:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += ToStringBinary(State, I.get(), "==");
					break;
				case IRInstructionType::NotEqualTo:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += ToStringBinary(State, I.get(), "!=");
					break;

				case IRInstructionType::UGreaterThan:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += ToStringBinary(State, I.get(), ">");
					break;
				case IRInstructionType::ULessThan:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += ToStringBinary(State, I.get(), "<");
					break;
				case IRInstructionType::UGreaterThanOrEqual:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += ToStringBinary(State, I.get(), ">=");
					break;
				case IRInstructionType::ULessThanOrEqual:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += ToStringBinary(State, I.get(), "<=");
					break;

				case IRInstructionType::SGreaterThan:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += ToStringBinary(State, I.get(), ">");
					break;
				case IRInstructionType::SLessThan:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += ToStringBinary(State, I.get(), "<");
					break;
				case IRInstructionType::SGreaterThanOrEqual:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += ToStringBinary(State, I.get(), ">=");
					break;
				case IRInstructionType::SLessThanOrEqual:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += ToStringBinary(State, I.get(), "<=");
					break;

				case IRInstructionType::Logical_And:
					r += ToStringBinary(State, I.get(), "&&");
					break;
				case IRInstructionType::Logical_Or:
					r += ToStringBinary(State, I.get(), "||");
					break;
				case IRInstructionType::PushParameter:
					State.TepPushedParameters.push_back(I.get());
					continue;
				case IRInstructionType::Logical_Not:
					r += ToString(I->ObjectType);
					r += " " + State.GetName(I.get());
					r += " = !" + ToString(State, *I, I->Target());
					break;
				case IRInstructionType::Jump:
					r += "goto ";
					r += Names.GetValue(I->Target().identifier);
					break;
				case IRInstructionType::ConditionalJump:
					r += "if (";
					r += ToString(State, *I, I->Input());
					r += "){goto ";
					r += Names.GetValue(I->Target().identifier);
					r += "; }";
					break;
				case IRInstructionType::CleanupFuncCall:
				case IRInstructionType::Call:
				{
					if (I->ObjectType != IRTypes::Void)
					{
						r += ToString(I->ObjectType);
						r += " " + State.GetName(I.get());
						r += " = ";
					}
					r += FromIDToCindentifier(I->Target().identifier) + "(";
					auto func = _Input->GetFunc(I->Target().identifier);
					for (size_t i = 0; i < State.TepPushedParameters.size(); i++)
					{
						auto& Item = State.TepPushedParameters[i];
						auto& par = func->Pars[i];

						if (_Input->GetType(Item) != par.type)
						{
							r += "*(";
							r += ToString(par.type);
							r += "*)&";
						}

						r += State.PointerToName.GetValue(Item->Target().Pointer);
						if (&Item != &State.TepPushedParameters.back())
						{
							r += ",";
						}

					}
					State.TepPushedParameters.clear();
					r += ")";
				}break;
				case IRInstructionType::CallFuncPtr:
				{
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					if (I->ObjectType != IRTypes::Void)
					{
						r += ToString(I->ObjectType);
						r += " " + State.GetName(I.get());
						r += " = ";
					}
					r += ToString(State, *I, I->Target()) + "(";
					
					auto func = _Input->GetSymbol(tep._symbol)->Get_ExAs<IRFuncPtr>();
					for (size_t i = 0; i < State.TepPushedParameters.size(); i++)
					{
						auto& Item = State.TepPushedParameters[i];
						auto& par = func->Pars[i];

						if (_Input->GetType(Item) != par)
						{
							r += "*(";
							r += ToString(par);
							r += "*)&";
						}
						r += State.PointerToName.GetValue(Item->Target().Pointer);
						if (&Item != &State.TepPushedParameters.back())
						{
							r += ",";
						}
					}
					State.TepPushedParameters.clear();
					r += ")";
				}break;
				case IRInstructionType::Return:
					if (Item->ReturnType == IRTypes::Void)
					{
						r += "return";
					}
					else
					{
						r += "return " + (String)IRReturnValue;
					}
					break;
				case IRInstructionType::Member_Access:
					State.PointerToName.AddValue(I.get(), ToString(State, *I, I->Target()) + ".__" + std::to_string(I->Input().Value.AsUIntNative));
					goto GoOver;
					break;
				case IRInstructionType::Member_Access_Dereference:
					State.PointerToName.AddValue(I.get(), ToString(State, *I, I->Target()) + "->__" + std::to_string(I->Input().Value.AsUIntNative));
					goto GoOver;
					break;
				case IRInstructionType::MallocCall:
					OutType = &tep;
					tep = Get_Settings().PtrSize == IntSizes::Int64 ? IRTypes::i64 : IRTypes::i32;
					r += ToString(I->ObjectType);
					r += " " + State.GetName(I.get());
					r += " = malloc(" + ToString(State, *I, I->Target()) + ")";
					break;
				case IRInstructionType::FreeCall:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += "free(" + ToString(State, *I, I->Target()) + ")";
					break;

				case IRInstructionType::SIntToSInt8:
				case IRInstructionType::SIntToSInt16:
				case IRInstructionType::SIntToSInt32:
				case IRInstructionType::SIntToSInt64:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());

					r += ToString(I->ObjectType);
					r += " " + State.GetName(I.get());
					r += " = ";

					r += "(";
					r += ToString(I->ObjectType);
					r += ")";

					r += ToString(State, *I, I->Target());
					break;

				case IRInstructionType::UIntToUInt8:
				case IRInstructionType::UIntToUInt16:
				case IRInstructionType::UIntToUInt32:
				case IRInstructionType::UIntToUInt64:
				{
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());

					auto valname = State.GetName(I.get());


					r += ToString(tep, true);
					r += " " + valname + IRReinterpretCastTep;
					r += " = ";
					r += "*(";
					r += ToString(tep, true);
					r += "*)&";

					r += ToString(State, *I, I->Target());

					r += ";\n ";

					r += ToString(I->ObjectType, true);
					r += " " + valname + (String)IRReinterpretCastTep + (String)"2";
					r += " = ";

					r += "(";
					r += ToString(I->ObjectType, true);
					r += ")";
					r += valname + IRReinterpretCastTep;
					r += ";\n ";

					r += ToString(I->ObjectType);
					r += " " + valname;
					r += " = ";

					r += "*(";
					r += ToString(I->ObjectType);
					r += "*)&";
					r += valname + String(IRReinterpretCastTep) + String("2");
				}
				break;
				case IRInstructionType::Unreachable:
					r += IRUnreachableDefineName;
					break;
				case IRInstructionType::Assume:
					r += IRAssumeDefineName;
					r += "(";
					r += ToString(State, *I, I->Target());
					r += ")";
					break;


				case IRInstructionType::Si32Tof32:
				case IRInstructionType::Si64Tof64:
				case IRInstructionType::f64Tof32:
				case IRInstructionType::f64Toi64:
				case IRInstructionType::f32Tof64:
				case IRInstructionType::f32Toi32:
				{
					r += ToString(I->ObjectType);
					r += " " + State.GetName(I.get());
					r += " = ";

					r += "(";
					r += ToString(I->ObjectType);
					r += ")";

					r += ToString(State, *I, I->Target());
				}
				break;
				case IRInstructionType::BitWise_And:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += ToStringBinary(State, I.get(), "&");
					break;
				case IRInstructionType::BitWise_Or:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += ToStringBinary(State, I.get(), "|");
					break;
				case IRInstructionType::BitWise_ShiftL:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += ToStringBinary(State, I.get(), "<<");
					break;
				case IRInstructionType::BitWise_ShiftR:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += ToStringBinary(State, I.get(), ">>");
					break;
				case IRInstructionType::BitWise_XOr:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += ToStringBinary(State, I.get(), "^");
					break;
				case IRInstructionType::UMod:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += ToStringBinary(State, I.get(), "%");
					break;
				case IRInstructionType::SMod:
					OutType = &tep;
					tep = _Input->GetType(I.get(), I.get()->Target());
					r += ToStringBinary(State, I.get(), "%");
					break;
				case IRInstructionType::BitWise_Not:
					r += ToString(I->ObjectType);
					r += " " + State.GetName(I.get());
					r += " = ~" + ToString(State, *I, I->Target());
					break;
				case IRInstructionType::ThrowException:
				{
					if (Flag_NoExceptions == false)
					{
						if (Flag_CPPCodeAllowed)
						{
							r += "throw std::runtime_error(std::string(";
							r += "(const char*)";
							r += ToString(State, *I, I->Target());
							r += ",";
							r += "(size_t)";
							r += ToString(State, *I, I->Input());
							r += "))";
						}
						else
						{

						}

					}
				}
				break;
				default:
					UCodeLangUnreachable();
					break;
				}
				r += ";\n";
			GoOver:
				;
			}
			State.PointerToName.clear();
		}


		r += "\n}";
	}
	else
	{
		r += ';';
	}
	r += "\n";
}


String C11Backend::ToString(ToStringState& State, IRInstruction& Ins, IROperator& Value)
{
	String r;

	if (Ins.Type == IRInstructionType::Member_Access || Ins.Type == IRInstructionType::Member_Access_Dereference)
	{
		;
	}

	if (Ins.Type != IRInstructionType::Member_Access
		&& Ins.Type != IRInstructionType::Member_Access_Dereference) {

		IRType ThisVal = _Input->GetType(&Ins, Value);

		if (ThisVal != (*OutType))
		{
			bool isok = false;
			if (Ins.Type == IRInstructionType::Reassign
				|| Ins.Type == IRInstructionType::Reassign_dereference)
			{
				isok = &Ins.Target() != &Value;
			}
			else
			{
				isok = true;
			}

			if (isok)
			{
				r += "(";
				r += ToString(*OutType);
				r += ")";
			}
		}
	}
	switch (Value.Type)
	{
	case IROperatorType::Value:
	{
		switch (Ins.ObjectType._Type)
		{
		case IRTypes::i8:r += std::to_string(Value.Value.AsInt8); break;
		case IRTypes::i16:r += std::to_string(Value.Value.AsInt16); break;
		case IRTypes::i32:r += std::to_string(Value.Value.AsInt32); break;
		case IRTypes::i64:r += std::to_string(Value.Value.AsInt64); break;
		case IRTypes::f32:r += std::to_string(Value.Value.Asfloat32); break;
		case IRTypes::f64:r += std::to_string(Value.Value.Asfloat64); break;
		default:UCodeLangUnreachable();
		}
	}
	break;
	case IROperatorType::IRidentifier:
	{
		r += FromIDToCindentifier(Value.identifier);
	}
	break;
	case IROperatorType::Get_PointerOf_IRidentifier:
	{
		r += "&" + FromIDToCindentifier(Value.identifier);
	}
	break;
	case IROperatorType::IRInstruction:
	{
		r += State.PointerToName.GetValue(Value.Pointer);
	}
	break;
	case IROperatorType::Get_PointerOf_IRInstruction:
	{
		r += "&" + State.PointerToName.GetValue(Value.Pointer);
	}
	break;
	case IROperatorType::DereferenceOf_IRInstruction:
	{
		r += "*(" + ToString(*OutType) + "*)" + State.PointerToName.GetValue(Value.Pointer);
	}
	break;
	case IROperatorType::IRParameter:
	{
		r += FromIDToCindentifier(Value.Parameter->identifier);
	}
	break;
	case IROperatorType::DereferenceOf_IRParameter:
	{
		r += "*(" + ToString(*OutType) + "*)" + FromIDToCindentifier(Value.Parameter->identifier);
	}
	break;
	case IROperatorType::Get_PointerOf_IRParameter:
	{
		r += "&" + FromIDToCindentifier(Value.Parameter->identifier);
	}
	break;
	case IROperatorType::Get_Func_Pointer:
	{
		r += "&" + FromIDToCindentifier(Value.identifier);
	}
	break;
	default:UCodeLangUnreachable();
	}

	return r;
}

String C11Backend::ToStringBinary(ToStringState& State, IRInstruction* Ins, const char* V)
{
	String r;
	r += ToString(Ins->ObjectType);
	r += " " + State.GetName(Ins);

	bool docast = Ins->ObjectType != _Input->GetType(Ins->B);
	r += " = ";
	if (docast)
	{
		r += "(" + ToString(Ins->ObjectType) + ")(";
	}
	r += ToString(State, *Ins, Ins->A) + String(V) + ToString(State, *Ins, Ins->B);

	if (docast)
	{
		r += ")";
	}
	return r;
}

String C11Backend::UpdateToCindentifier(const String& Value)
{
	const char* GoodChar = "qwerttyuiopasdfghjklzxcvbnm1234567890QWERTYUIOPASDFGHJKLZXCVBNM_";
	size_t GoodCharSize = strlen(GoodChar);

	String v;
	for (size_t i = 0; i < Value.size(); i++)
	{
		char Char = Value[i];

		bool IsGoodChar = false;
		for (size_t i = 0; i < GoodCharSize; i++)
		{
			if (Char == GoodChar[i])
			{
				IsGoodChar = true;
				break;
			}
		}


		if (IsGoodChar)
		{
			v.push_back(Char);

		}
		else
		{
			v.push_back('_');
		}

	}
	return v;
}

void C11Backend::UpdateBackInfo(CompilationBackEndInfo& BackInfo)
{
	BackInfo.Output = CompilationBackEndInfo::BackEnd::C11;
	BackInfo.OutputSet = CompilationBackEndInfo::InsSet::Other;
}

String C11Backend::FromIDToCindentifier(IRidentifierID Value)
{
	return UpdateToCindentifier(_Input->FromID(Value));
}
String C11Backend::ToStringState::GetName(IRInstruction* Ptr)
{
	if (!PointerToName.HasValue(Ptr))
	{

		auto V = "tep" + std::to_string(Num);
		Num++;
		PointerToName.AddValue(Ptr, V);

	}
	return PointerToName.GetValue(Ptr);
}

UCodeLangEnd

#endif