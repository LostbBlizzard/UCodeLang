#include "AppAPILink.hpp"
UCodeIDEStart

void LinkAppAPICallsTo(UCodeLang::RunTimeLib& lib)
{
	using namespace API;
	UCodeLangAutoLink(lib, API);
	{//Made by UCodeAutoLink 
		using Imgui__Ping_ptr = int(*UCodeLangAPI)(int); 
		using Imgui__Button_ptr = bool(*UCodeLangAPI)(StringView); 
		using Imgui__Button0_ptr = bool(*UCodeLangAPI)(StringView,const Vec2&); 
		using Imgui__ObjectField_ptr = bool(*UCodeLangAPI)(StringView,Int32&); 
		using Imgui__ObjectField0_ptr = bool(*UCodeLangAPI)(StringView,bool&); 
		using Imgui__Text_ptr = void(*UCodeLangAPI)(StringView); 
		using Imgui__KeyDown_ptr = bool(*UCodeLangAPI)(ImKey); 
		lib.Add_CPPCall("API:Imgui:Ping",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				int Par0 = Input.GetParameter<int>();
				
				int Ret =API::Imgui::Ping(Par0);
				
				Input.Set_Return<int>(Ret);
			
			},(Imgui__Ping_ptr)API::Imgui::Ping); 
		lib.Add_CPPCall("API:Imgui:Button",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				StringView Par0 = Input.GetParameter<StringView>();
				
				bool Ret =API::Imgui::Button(Par0);
				
				Input.Set_Return<bool>(Ret);
			
			},(Imgui__Button_ptr)API::Imgui::Button); 
		lib.Add_CPPCall("API:Internal:Imgui__Button0",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				StringView Par0 = Input.GetParameter<StringView>();
				
				const Vec2* Par1 = Input.GetParameter<const Vec2*>();
				
				bool Ret =API::Imgui::Button(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(Imgui__Button0_ptr)API::Imgui::Button); 
		lib.Add_CPPCall("API:Imgui:ObjectField",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				StringView Par0 = Input.GetParameter<StringView>();
				
				Int32* Par1 = Input.GetParameter<Int32*>();
				
				bool Ret =API::Imgui::ObjectField(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(Imgui__ObjectField_ptr)API::Imgui::ObjectField); 
		lib.Add_CPPCall("API:Internal:Imgui__ObjectField0",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				StringView Par0 = Input.GetParameter<StringView>();
				
				bool* Par1 = Input.GetParameter<bool*>();
				
				bool Ret =API::Imgui::ObjectField(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(Imgui__ObjectField0_ptr)API::Imgui::ObjectField); 
		lib.Add_CPPCall("API:Imgui:Text",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				StringView Par0 = Input.GetParameter<StringView>();
				
				API::Imgui::Text(Par0);
				
				Input.Set_Return();
			
			},(Imgui__Text_ptr)API::Imgui::Text); 
		lib.Add_CPPCall("API:Imgui:KeyDown",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				ImKey Par0 = Input.GetParameter<ImKey>();
				
				bool Ret =API::Imgui::KeyDown(Par0);
				
				Input.Set_Return<bool>(Ret);
			
			},(Imgui__KeyDown_ptr)API::Imgui::KeyDown); 
	}//Made by UCodeAutoLink End
}

UCodeIDEEnd
