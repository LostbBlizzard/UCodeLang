#pragma once
#include "ImGuiHelpers/ImguiHelper.hpp"

UCodeIDEStart

namespace API 
{
	using Vec2 = ImVec2;
	using Vec4 = ImVec4;
	using TypeInfo = UCodeLang::ReflectionTypeInfo;
	UCodeLangExportSymbol("API") UCodeLangExportTrait LogTrait
	{
		UCodeLangExport int SomeVal = 0;

		UCodeLangExport void Log()
		{
			String V =std::to_string(SomeVal);
		}
		UCodeLangExport void Log(StringView Val)
		{
			std::cout << Val;
		}
		UCodeLangEmbed("|Log<T>[this&,T& value] => Log();")
	private:
	};
	

	UCodeLangExportSymbol("API") struct Imgui
	{
		

		static const char* ToCChar(StringView Val)
		{
			static String R;
			R = Val;

			return R.c_str();
		}

		UCodeLangExport static int Ping(int a)
		{
			return a;
		}

		UCodeLangExport static bool Button(StringView Name)
		{
			return Button(Name, {0,0});
		}

		UCodeLangExport static bool Button(StringView Name,const Vec2& Val)
		{
			return ImGui::Button(ToCChar(Name),Val);
		}

		UCodeLangExport static bool ObjectField(StringView Name,Int32& Val)
		{
			return 	ImguiHelper::Int32Field(ToCChar(Name),Val);
		}
		UCodeLangExport static bool ObjectField(StringView Name,bool& Val)
		{
			return 	ImguiHelper::BoolEnumField(ToCChar(Name), Val);
		}

		static bool ObjectField(StringView Name,const TypeInfo& Info,void* Object)
		{
			return 	ImguiHelper::UCodeObjectField(ToCChar(Name),Object, Info,{});
		}

		UCodeLangExport static void Text(StringView Name)
		{
			return  ImGui::Text(ToCChar(Name));
		}
	};
}
UCodeIDEEnd