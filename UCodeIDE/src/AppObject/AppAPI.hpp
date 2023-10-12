#pragma once
#include "ImGuiHelpers/ImguiHelper.hpp"

UCodeIDEStart

namespace API 
{
	using Vec2 = ImVec2;
	using Vec4 = ImVec4;
	using TypeInfo = UCodeLang::ReflectionTypeInfo;

	UCodeLangExportSymbol("API") enum class ImKey
	{
		//Me String
		W,
		A,
		S,
		D,
		
		/*
		Me
		String 
		2.0
		*/
		UpArrow,
		DownArrow,
		LeftArrow,
		RightArrow,
	};
	inline ImGuiKey To(ImKey key)
	{
		switch (key)
		{
		case ImKey::W:
			return ImGuiKey::ImGuiKey_W;
		case ImKey::A:
			return ImGuiKey::ImGuiKey_A;
		case ImKey::S:
			return ImGuiKey::ImGuiKey_S;
		case ImKey::D:
			return ImGuiKey::ImGuiKey_D;
		case ImKey::UpArrow:
			return ImGuiKey::ImGuiKey_UpArrow;
		case ImKey::DownArrow:
			return ImGuiKey::ImGuiKey_DownArrow;
		case ImKey::LeftArrow:
			return ImGuiKey::ImGuiKey_LeftArrow;
		case ImKey::RightArrow:
			return ImGuiKey::ImGuiKey_RightArrow;
		default:
			UCodeLangUnreachable();
			break;
		}
	}

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

		UCodeLangExport static bool KeyDown(ImKey key)
		{
			return ImGui::IsKeyDown(To(key));
		}
	};
}
UCodeIDEEnd