#pragma once
#include "AppAPILink.hpp"
UCodeIDEStart

void LinkAppAPICallsTo(UCodeLang::RunTimeLib& lib)
{
	using namespace API;
	UCodeLangAutoLink(lib, API);
	{//Made by UCodeAutoLink 
		using LogTrait__Log_ptr = void(*UCodeLangAPI)(API::LogTrait*); 
		using LogTrait__Log0_ptr = void(*UCodeLangAPI)(API::LogTrait*,StringView); 
		using Imgui__Ping_ptr = int(*UCodeLangAPI)(int); 
		using Imgui__Button_ptr = bool(*UCodeLangAPI)(StringView); 
		using Imgui__Button0_ptr = bool(*UCodeLangAPI)(StringView,const Vec2&); 
		using Imgui__ObjectField_ptr = bool(*UCodeLangAPI)(StringView,Int32&); 
		using Imgui__ObjectField0_ptr = bool(*UCodeLangAPI)(StringView,bool&); 
		lib.Add_CPPCall("API:LogTrait:Log",[](UCodeLang::InterpreterCPPinterface& Input) 
			{
			
				API::LogTrait* thisPar = Input.GetParameter<API::LogTrait*>();
				
				thisPar->Log();
				
				Input.Set_Return();
			
			},(LogTrait__Log_ptr)[](API::LogTrait* thisPar)
			{
				thisPar->Log();
			}); 
		lib.Add_CPPCall("API:Internal:LogTrait__Log0",[](UCodeLang::InterpreterCPPinterface& Input) 
			{
			
				API::LogTrait* thisPar = Input.GetParameter<API::LogTrait*>();
				
				StringView Par0 = Input.GetParameter<StringView>();
				
				thisPar->Log(Par0);
				
				Input.Set_Return();
			
			},(LogTrait__Log0_ptr)[](API::LogTrait* thisPar,StringView Par0)
			{
				thisPar->Log(Par0);
			}); 
		lib.Add_CPPCall("API:Imgui:Ping",[](UCodeLang::InterpreterCPPinterface& Input) 
			{
			
				int Par0 = Input.GetParameter<int>();
				
				int Ret =API::Imgui::Ping(Par0);
				
				Input.Set_Return<int>(Ret);
			
			},(Imgui__Ping_ptr)API::Imgui::Ping); 
		lib.Add_CPPCall("API:Imgui:Button",[](UCodeLang::InterpreterCPPinterface& Input) 
			{
			
				StringView Par0 = Input.GetParameter<StringView>();
				
				bool Ret =API::Imgui::Button(Par0);
				
				Input.Set_Return<bool>(Ret);
			
			},(Imgui__Button_ptr)API::Imgui::Button); 
		lib.Add_CPPCall("API:Internal:Imgui__Button0",[](UCodeLang::InterpreterCPPinterface& Input) 
			{
			
				StringView Par0 = Input.GetParameter<StringView>();
				
				const Vec2* Par1 = Input.GetParameter<const Vec2*>();
				
				bool Ret =API::Imgui::Button(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(Imgui__Button0_ptr)API::Imgui::Button); 
		lib.Add_CPPCall("API:Imgui:ObjectField",[](UCodeLang::InterpreterCPPinterface& Input) 
			{
			
				StringView Par0 = Input.GetParameter<StringView>();
				
				Int32* Par1 = Input.GetParameter<Int32*>();
				
				bool Ret =API::Imgui::ObjectField(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(Imgui__ObjectField_ptr)API::Imgui::ObjectField); 
		lib.Add_CPPCall("API:Internal:Imgui__ObjectField0",[](UCodeLang::InterpreterCPPinterface& Input) 
			{
			
				StringView Par0 = Input.GetParameter<StringView>();
				
				bool* Par1 = Input.GetParameter<bool*>();
				
				bool Ret =API::Imgui::ObjectField(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(Imgui__ObjectField0_ptr)API::Imgui::ObjectField); 
	}//Made by UCodeAutoLink End
}

UCodeIDEEnd
