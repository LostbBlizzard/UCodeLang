#include "StandardLibraryBuilder.hpp"

#include "UCodeLang/Compliation/UAssembly/UAssembly.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>
#include "UCodeAnalyzer/TextBuilder/UCodeTextBuilder.hpp"

#define Template_VarName0 "T"
#define Template_VarName1 "U"
#define Template_VarName2 "V"

namespace fs = std::filesystem;
void StandardLibraryBuilder::Get_BasicTypeFile(UCodeLang::String& Out)
{
	UCodeAnalyzer::UCodeTextBuilder Builder;
	auto& Name = Builder.AddNameSpace(StandardLibrarynamespace);

	auto V2 = Name.AddAlias(VoidPtrTypeName,"!");
	V2.OldName += UintPtrTypeName;

	auto V =Name.AddAlias(CStringTypeName, CharTypeName);
	V.OldName += "[&]";

	auto& HashClass = Name.AddClass("Hash");
	HashClass.AddGeneric(Template_VarName0);


	auto& BitsClass = Name.AddClass("Bits");
	BitsClass.AddGeneric(Template_VarName0);
}
void StandardLibraryBuilder::Get_VecFile(UCodeLang::String& Out)
{
	UCodeAnalyzer::UCodeTextBuilder Builder;
	auto& Name = Builder.AddNameSpace(StandardLibrarynamespace);

	Name.AddUseing("TestMath");

	auto& Vec2_tClass = Name.AddClass("Vec2_t");
	Vec2_tClass.AddGeneric(Template_VarName0);
	Vec2_tClass.AddMember("X", Template_VarName0, "0");
	Vec2_tClass.AddMember("Y", Template_VarName0, "0");

	auto& Vec3_tClass = Name.AddClass("Vec3_t");
	Vec3_tClass.AddGeneric(Template_VarName0);
	Vec3_tClass.AddMember("X", Template_VarName0, "0");
	Vec3_tClass.AddMember("Y", Template_VarName0, "0");
	Vec3_tClass.AddMember("Z", Template_VarName0, "0");

	auto& Vec2i_tClass = Name.AddClass("Vec2i_t");
	Vec2i_tClass.AddGeneric(Template_VarName0);
	Vec2i_tClass.AddMember("X", Template_VarName0, "0");
	Vec2i_tClass.AddMember("Y", Template_VarName0, "0");

	auto& Vec3i_tClass = Name.AddClass("Vec3i_t");
	Vec3i_tClass.AddMember("X", Template_VarName0, "0");
	Vec3i_tClass.AddMember("Y", Template_VarName0, "0");
	Vec3i_tClass.AddMember("Z", Template_VarName0, "0");
	//

	auto& Vec2_32Class = Name.AddAlias("Vec2_32", Vec2_tClass.Name);
	Vec2_32Class.AddGeneric(Uint8TypeName);//float32

	auto& Vec2_64Class = Name.AddAlias("Vec2_64", Vec2_tClass.Name);
	Vec2_64Class.AddGeneric(Uint8TypeName);//float64

	auto& Vec2i_32Class = Name.AddAlias("Vec2i_32", Vec2i_tClass.Name);
	Vec2i_32Class.AddGeneric(Uint8TypeName);//int32

	auto& Vec2i_64Class = Name.AddAlias("Vec2i_64", Vec2i_tClass.Name);
	Vec2i_64Class.AddGeneric(Uint8TypeName);//int64

	//
	auto& Vec3_32Class = Name.AddAlias("Vec3_32", Vec3_tClass.Name);
	Vec3_32Class.AddGeneric(Uint8TypeName);//float32

	auto& Vec3_64Class = Name.AddAlias("Vec3_64", Vec3_tClass.Name);
	Vec3_64Class.AddGeneric(Uint8TypeName);//float64

	auto& Vec3i_32Class = Name.AddAlias("Vec3i_32", Vec3i_tClass.Name);
	Vec3i_32Class.AddGeneric(Uint8TypeName);//int32

	auto& Vec3i_64Class = Name.AddAlias("Vec3i_64", Vec3i_tClass.Name);
	Vec3i_64Class.AddGeneric(Uint8TypeName);//int64

	//Aliass
	Name.AddAlias("Vec2", Vec2_32Class.NewName);
	Name.AddAlias("Vec2i", Vec2i_32Class.NewName);

	Name.AddAlias("Vec3", Vec3_32Class.NewName);
	Name.AddAlias("Vec3i", Vec3i_32Class.NewName);

	Builder.ToUCodeFile(Out);
}

void StandardLibraryBuilder::Get_LibFile(UCodeLang::String& Out)
{
	UCodeAnalyzer::UCodeTextBuilder Builder;
	auto& Name = Builder.AddNameSpace(StandardLibrarynamespace);

	{//DLLAttribute
		auto& DLLAttribute = Name.AddAttributeClass(DLLCallAttributeName);


	}
	Builder.ToUCodeFile(Out);
}

void StandardLibraryBuilder::Get_LowLevelFile(UCodeLang::String& Out)
{
	UCodeAnalyzer::UCodeTextBuilder Builder;
	auto& Name = Builder.AddNameSpace(StandardLibrarynamespace);

	auto& LowLevelClass = Name.AddClass("LowLevel");
	{
		auto& Method = LowLevelClass.AddMethod("Cast");
		Method.AddGeneric(Template_VarName0);
		Method.AddGeneric(Template_VarName1);
		Method.AddParameter("Value", Template_VarName0);
		Method.RetType = Template_VarName1;
	}

	{
		auto& Method = LowLevelClass.AddMethod("Malloc");
		Method.AddParameter("Size", UintPtrTypeName);
		Method.RetType = VoidPtrTypeName;
	}

	{
		auto& Method = LowLevelClass.AddMethod("Calloc");
		Method.AddParameter("Size", UintPtrTypeName);
		Method.RetType = VoidPtrTypeName;
	}

	{
		auto& Method = LowLevelClass.AddMethod("Realloc");
		Method.AddParameter("oldptr", VoidPtrTypeName);
		Method.AddParameter("Size", UintPtrTypeName);
		Method.RetType = VoidPtrTypeName;
	}

	{
		auto& Method = LowLevelClass.AddMethod("Free");
		Method.AddParameter("ptr", VoidPtrTypeName);
	}
	Builder.ToUCodeFile(Out);
}

void StandardLibraryBuilder::Get_ConsoleFile(UCodeLang::String& Out)
{
	UCodeAnalyzer::UCodeTextBuilder Builder;
	auto& Name = Builder.AddNameSpace(StandardLibrarynamespace);

	auto& Console = Name.AddClass("Console");
	
	{
		auto& Method = Console.AddMethod("Log");
		Method.AddParameter("string", CStringTypeName);
	}
	{
		//auto& Method = Console.AddMethod("Log_");
		//Method.AddParameter("string", "String&");
	}

	auto& Console_ReadLine = Console.AddMethod("ReadLine");

	Builder.ToUCodeFile(Out);
}

void StandardLibraryBuilder::Get_MathFile(UCodeLang::String& Out)
{
	UCodeAnalyzer::UCodeTextBuilder Builder;
	auto& Name = Builder.AddNameSpace(StandardLibrarynamespace);

	auto& Console = Name.AddClass("Math");
	auto& Console_Log = Console.AddMethod("PI");

	Builder.ToUCodeFile(Out);
}

void StandardLibraryBuilder::Get_GenericContainersFile(UCodeLang::String& Out)
{
	UCodeAnalyzer::UCodeTextBuilder Builder;
	auto& Name = Builder.AddNameSpace(StandardLibrarynamespace);

	{
		auto& Class = Name.AddClass("Array");
		Class.AddGeneric(Template_VarName0);
	}
	{
		auto& Class = Name.AddClass("String");
	}

	{
		auto& Class = Name.AddClass("Stack");
		Class.AddGeneric(Template_VarName0);
	}

	{
		auto& Class = Name.AddClass("Optional");
		Class.AddGeneric(Template_VarName0);
		Class.AddMember("Item", Template_VarName0);
		Class.AddMember("HasValue", boolTypeName, UCodeAnalyzer::KeyWordAndTokens::FalseKeyWord);
	}
	Builder.ToUCodeFile(Out);
}

void StandardLibraryBuilder::Get_MemoryFile(UCodeLang::String& Out)
{
	UCodeAnalyzer::UCodeTextBuilder Builder;
	auto& Name = Builder.AddNameSpace(StandardLibrarynamespace);

	{
		auto& Class = Name.AddClass("Ref");
		Class.AddGeneric(Template_VarName0);
	}

	Builder.ToUCodeFile(Out);
}

void StandardLibraryBuilder::BuildLibraryToDir(const UCodeLang::String& Dir)
{
	UCodeLang::String TepString;
	UCodeLang::String TepPath;
	
	{//Math
		Get_MathFile(TepString); TepPath = Dir + "Math/Math" + UCodeLang::FileExt::SourceFileWithDot;
		WirteToFile(TepPath, TepString); TepString.clear();
	}

	{//LowLevel
		Get_LibFile(TepString); TepPath = Dir + "LowLevel/Dll" + UCodeLang::FileExt::SourceFileWithDot;
		WirteToFile(TepPath, TepString); TepString.clear();

		Get_LowLevelFile(TepString); TepPath = Dir + "LowLevel/LowLevel" + UCodeLang::FileExt::SourceFileWithDot;
		WirteToFile(TepPath, TepString); TepString.clear();
	}

	{//IO
		Get_ConsoleFile(TepString); TepPath = Dir + "IO/Console" + UCodeLang::FileExt::SourceFileWithDot;
		WirteToFile(TepPath, TepString); TepString.clear();
	}

	{//Types
		Get_GenericContainersFile(TepString); TepPath = Dir + "Types/Containers" + UCodeLang::FileExt::SourceFileWithDot;
		WirteToFile(TepPath, TepString); TepString.clear();
		
		Get_VecFile(TepString); TepPath = Dir + "Types/Vectors" + UCodeLang::FileExt::SourceFileWithDot;
		WirteToFile(TepPath, TepString); TepString.clear();

		Get_MemoryFile(TepString); TepPath = Dir + "Types/Memory" + UCodeLang::FileExt::SourceFileWithDot;
		WirteToFile(TepPath, TepString); TepString.clear();
	}
}

void StandardLibraryBuilder::WirteToFile(const UCodeLang::String& Path, const UCodeLang::String& String)
{
	fs::path PathD = Path;
	fs::path PathDir = PathD.parent_path();
	fs::create_directories(PathDir);

	std::ofstream out(Path);
	out << String;
	out.close();
}
