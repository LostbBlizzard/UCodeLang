#pragma once

#ifndef UCodeLangNoCompiler
#include "UCodeLang/LangCore/UClib.hpp"
#include "UCodeLang/Compilation/CompilationSettings.hpp"
#include "UCodeLang/Compilation/Helpers/CompilationErrors.hpp"
#include "UCodeLang/LangCore/UClib.hpp"
#include "UAssembly_NameSpace.hpp"

#include "UAssembly_Lexer.hpp"
#include "UAssembly_Parser.hpp"
#include "UCodeLang/LangCore/TaskManger.hpp"
UAssemblyStart
class UAssembly
{
public:
	UAssembly() {}
	~UAssembly() {}
	bool Assemble(const String_view& Text, UClib* Out);
	UCodeLangForceinline void Set_ErrorsOutput(CompilationErrors* V) { _ErrorsOutput = V; }
	UCodeLangForceinline void Set_Settings(CompilationSettings* V) { _Settings = V; }


	static String ToString(const UClib* Lib, Optional<Path> SourceFiles = {}, bool ShowIR = false);



	//return is added 
	static size_t ParseInstruction(size_t I, const Span<Instruction> Data, String& r, const BytesView staticbytesview, UnorderedMap<UAddress, String>& AddressToName, bool CombineIns = true);
	static void ToStringInstruction(const Instruction& Item, String& r, const BytesView staticbytesview, UnorderedMap<UAddress, String>& AddressToName);

	static String ToString(const ReflectionTypeInfo& Value, const ClassAssembly& Assembly);
	static String ToString(const ReflectionTypeInfo& Value, const ReflectionRawData& Data, const ClassAssembly& Assembly, UClib::NTypeSize PtrSize);
	static String ToString(const TypedRawReflectionData& Value, const ClassAssembly& Assembly, UClib::NTypeSize PtrSize)
	{
		return ToString(Value._Type, Value._Data, Assembly, PtrSize);
	}
	static String ToString(const UsedTagValueData& Value, const ClassAssembly& Assembly, UClib::NTypeSize PtrSize);
	static String ToString(const ClassMethod::Par& Value, const ClassAssembly& Assembly);
	static void ToString(String& r, ClassMethod& Item2, const UClib* Lib);

	static String GetRegisterToString(RegisterID reg)
	{
		switch (reg)
		{
		case UCodeLang::RegisterID::A:return "[A]";
		case UCodeLang::RegisterID::B:return "[B]";
		case UCodeLang::RegisterID::C:return "[C]";
		case UCodeLang::RegisterID::D:return "[D]";
		case UCodeLang::RegisterID::E:return "[E]";
		case UCodeLang::RegisterID::F:return "[F]";
		default:return "[bad]";
		}
	}
	static String GetRegisterToString(UInt64 reg) { return GetRegisterToString((RegisterID)reg); }
	static String GetInsName(const UClib* Lib, UAddress Pos)
	{
		return "[&" + std::to_string(Pos) + "]";
	}
	static void OpValueToString(OpCodeType OpType, const void* In, const UnorderedMap<UAddress, String>& AddressToName, const BytesView StaticVarablesData, String& out);

	static size_t BuildHashForSub(const Instruction* Pointer, size_t BufferSize);

	struct StripSettings
	{
		bool DebugInfo = true;
		bool TypeAssembly = true;
		bool FuncToAddress = true;
		String TargetCodeLayer = UCode_CodeLayer_UCodeVM_Name;
	};
	struct StripOutput
	{
		Optional<ULangDebugInfo> StripedDebugInfo;
		Optional<ClassAssembly> StripedAssembly;
		Optional<UnorderedMap<String, UAddress>> StripedFuncToAddress;
	};
	//Currntly only works UCodeVM CodeLayer
	static StripOutput Strip(UClib& lib, const StripSettings& settings);

	struct StripFuncSettings
	{
		Vector<const ClassMethod*> FuncionsToKeep;
		bool RemoveFuncions = true;
		bool RemoveType = false;
	};
	struct StripFuncs
	{
		Vector<ClassMethod> RemovedFuncions;
		Vector<Unique_ptr<AssemblyNode>> RemovedTypes;
	};
	static StripFuncs StripFunc(UClib& lib, const StripFuncSettings& setting)
	{
		TaskManger tasks;
		tasks.Init();

		return StripFunc(lib, setting, tasks);
	}
	static bool NodeDependsonType(const AssemblyNode* node, ReflectionCustomTypeID id);
	static StripFuncs StripFunc(UClib& lib, const StripFuncSettings& setting, TaskManger& tasks);
private:
	CompilationErrors* _ErrorsOutput = nullptr;
	CompilationSettings* _Settings = nullptr;
};
UAssemblyEnd

#endif