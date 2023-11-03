#include "ECSExample.hpp"


UCodeIDEStart

namespace ECSExample
{

	void ECSLink(UCodeLang::RunTimeLib& lib)
	{
		UCodeLangAutoLink(lib, ECSExample);
	{//Made by UCodeAutoLink 
		using Time__DeltaTime_ptr = float(*UCodeLangAPI)(); 
		using ComponentAPI__entity_ptr = Entity*(*UCodeLangAPI)(uintptr_t); 
		using ComponentAPI__ientity_ptr = const Entity*(*UCodeLangAPI)(uintptr_t); 
		using Entity__scenc_ptr = Scene*(*UCodeLangAPI)(ECSExample::Entity*); 
		using Entity__iscenc_ptr = const Scene*(*UCodeLangAPI)(ECSExample::Entity*); 
		using Entity__AddChildEntity_ptr = Entity*(*UCodeLangAPI)(ECSExample::Entity*); 
		using Entity__Destroy1_ptr = void(*UCodeLangAPI)(ECSExample::Entity*); 
		using Entity__name_ptr = String*(*UCodeLangAPI)(ECSExample::Entity*); 
		using Entity__iname_ptr = const StringView(*UCodeLangAPI)(ECSExample::Entity*); 
		using Entity__position_ptr = Vec3*(*UCodeLangAPI)(ECSExample::Entity*); 
		using Entity__position2d_ptr = Vec2*(*UCodeLangAPI)(ECSExample::Entity*); 
		using Entity__iposition_ptr = const Vec3*(*UCodeLangAPI)(ECSExample::Entity*); 
		using Entity__iposition2d_ptr = const Vec2*(*UCodeLangAPI)(ECSExample::Entity*); 
		using Entity__scale_ptr = Vec3*(*UCodeLangAPI)(ECSExample::Entity*); 
		using Entity__scale2d_ptr = Vec2*(*UCodeLangAPI)(ECSExample::Entity*); 
		using Entity__iscale_ptr = const Vec3*(*UCodeLangAPI)(ECSExample::Entity*); 
		using Entity__iscale2d_ptr = const Vec2*(*UCodeLangAPI)(ECSExample::Entity*); 
		using Entity__rotation_ptr = Vec3*(*UCodeLangAPI)(ECSExample::Entity*); 
		using Entity__rotation2d_ptr = Vec2*(*UCodeLangAPI)(ECSExample::Entity*); 
		using Entity__irotation_ptr = const Vec3*(*UCodeLangAPI)(ECSExample::Entity*); 
		using Entity__irotation2d7_ptr = const Vec2*(*UCodeLangAPI)(ECSExample::Entity*); 
		using Scene__AddEntity1_ptr = Entity*(*UCodeLangAPI)(ECSExample::Scene*); 
		lib.Add_CPPCall("ECS:Time:DeltaTime",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				float Ret =ECSExample::Time::DeltaTime();
				
				Input.Set_Return<float>(Ret);
			
			},(Time__DeltaTime_ptr)ECSExample::Time::DeltaTime); 
		lib.Add_CPPCall("ECS:ComponentAPI:entity",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				uintptr_t Par0 = Input.GetParameter<uintptr_t>();
				
				Entity* Ret =&ECSExample::ComponentAPI::entity(Par0);
				
				Input.Set_Return<Entity*>(Ret);
			
			},(ComponentAPI__entity_ptr)ECSExample::ComponentAPI::entity); 
		lib.Add_CPPCall("ECS:ComponentAPI:ientity",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				uintptr_t Par0 = Input.GetParameter<uintptr_t>();
				
				const Entity* Ret =&ECSExample::ComponentAPI::ientity(Par0);
				
				Input.Set_Return<const Entity*>(Ret);
			
			},(ComponentAPI__ientity_ptr)ECSExample::ComponentAPI::ientity); 
		lib.Add_CPPCall("ECS:Entity:scenc",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				ECSExample::Entity* thisPar = Input.GetParameter<ECSExample::Entity*>();
				
				Scene* Ret =thisPar->scenc();
				
				Input.Set_Return<Scene*>(Ret);
			
			},(Entity__scenc_ptr)[](ECSExample::Entity* thisPar) ->Scene*
			{
				return thisPar->scenc();
			}); 
		lib.Add_CPPCall("ECS:Entity:iscenc",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				ECSExample::Entity* thisPar = Input.GetParameter<ECSExample::Entity*>();
				
				const Scene* Ret =thisPar->iscenc();
				
				Input.Set_Return<const Scene*>(Ret);
			
			},(Entity__iscenc_ptr)[](ECSExample::Entity* thisPar) ->const Scene*
			{
				return thisPar->iscenc();
			}); 
		lib.Add_CPPCall("ECS:Entity:AddChildEntity",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				ECSExample::Entity* thisPar = Input.GetParameter<ECSExample::Entity*>();
				
				Entity* Ret =thisPar->AddChildEntity();
				
				Input.Set_Return<Entity*>(Ret);
			
			},(Entity__AddChildEntity_ptr)[](ECSExample::Entity* thisPar) ->Entity*
			{
				return thisPar->AddChildEntity();
			}); 
		lib.Add_CPPCall("ECS:Internal:Entity__Destroy1",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				ECSExample::Entity* thisPar = Input.GetParameter<ECSExample::Entity*>();
				
				thisPar->Destroy();
				
				Input.Set_Return();
			
			},(Entity__Destroy1_ptr)[](ECSExample::Entity* thisPar) ->void
			{
				thisPar->Destroy();
			}); 
		lib.Add_CPPCall("ECS:Entity:name",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				ECSExample::Entity* thisPar = Input.GetParameter<ECSExample::Entity*>();
				
				String* Ret =&thisPar->name();
				
				Input.Set_Return<String*>(Ret);
			
			},(Entity__name_ptr)[](ECSExample::Entity* thisPar) ->String*
			{
				return &thisPar->name();
			}); 
		lib.Add_CPPCall("ECS:Entity:iname",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				ECSExample::Entity* thisPar = Input.GetParameter<ECSExample::Entity*>();
				
				const StringView Ret =thisPar->iname();
				
				Input.Set_Return<const StringView>(Ret);
			
			},(Entity__iname_ptr)[](ECSExample::Entity* thisPar) ->const StringView
			{
				return thisPar->iname();
			}); 
		lib.Add_CPPCall("ECS:Entity:position",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				ECSExample::Entity* thisPar = Input.GetParameter<ECSExample::Entity*>();
				
				Vec3* Ret =&thisPar->position();
				
				Input.Set_Return<Vec3*>(Ret);
			
			},(Entity__position_ptr)[](ECSExample::Entity* thisPar) ->Vec3*
			{
				return &thisPar->position();
			}); 
		lib.Add_CPPCall("ECS:Entity:position2d",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				ECSExample::Entity* thisPar = Input.GetParameter<ECSExample::Entity*>();
				
				Vec2* Ret =&thisPar->position2d();
				
				Input.Set_Return<Vec2*>(Ret);
			
			},(Entity__position2d_ptr)[](ECSExample::Entity* thisPar) ->Vec2*
			{
				return &thisPar->position2d();
			}); 
		lib.Add_CPPCall("ECS:Entity:iposition",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				ECSExample::Entity* thisPar = Input.GetParameter<ECSExample::Entity*>();
				
				const Vec3* Ret =&thisPar->iposition();
				
				Input.Set_Return<const Vec3*>(Ret);
			
			},(Entity__iposition_ptr)[](ECSExample::Entity* thisPar) ->const Vec3*
			{
				return &thisPar->iposition();
			}); 
		lib.Add_CPPCall("ECS:Entity:iposition2d",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				ECSExample::Entity* thisPar = Input.GetParameter<ECSExample::Entity*>();
				
				const Vec2* Ret =&thisPar->iposition2d();
				
				Input.Set_Return<const Vec2*>(Ret);
			
			},(Entity__iposition2d_ptr)[](ECSExample::Entity* thisPar) ->const Vec2*
			{
				return &thisPar->iposition2d();
			}); 
		lib.Add_CPPCall("ECS:Entity:scale",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				ECSExample::Entity* thisPar = Input.GetParameter<ECSExample::Entity*>();
				
				Vec3* Ret =&thisPar->scale();
				
				Input.Set_Return<Vec3*>(Ret);
			
			},(Entity__scale_ptr)[](ECSExample::Entity* thisPar) ->Vec3*
			{
				return &thisPar->scale();
			}); 
		lib.Add_CPPCall("ECS:Entity:scale2d",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				ECSExample::Entity* thisPar = Input.GetParameter<ECSExample::Entity*>();
				
				Vec2* Ret =&thisPar->scale2d();
				
				Input.Set_Return<Vec2*>(Ret);
			
			},(Entity__scale2d_ptr)[](ECSExample::Entity* thisPar) ->Vec2*
			{
				return &thisPar->scale2d();
			}); 
		lib.Add_CPPCall("ECS:Entity:iscale",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				ECSExample::Entity* thisPar = Input.GetParameter<ECSExample::Entity*>();
				
				const Vec3* Ret =&thisPar->iscale();
				
				Input.Set_Return<const Vec3*>(Ret);
			
			},(Entity__iscale_ptr)[](ECSExample::Entity* thisPar) ->const Vec3*
			{
				return &thisPar->iscale();
			}); 
		lib.Add_CPPCall("ECS:Entity:iscale2d",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				ECSExample::Entity* thisPar = Input.GetParameter<ECSExample::Entity*>();
				
				const Vec2* Ret =&thisPar->iscale2d();
				
				Input.Set_Return<const Vec2*>(Ret);
			
			},(Entity__iscale2d_ptr)[](ECSExample::Entity* thisPar) ->const Vec2*
			{
				return &thisPar->iscale2d();
			}); 
		lib.Add_CPPCall("ECS:Entity:rotation",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				ECSExample::Entity* thisPar = Input.GetParameter<ECSExample::Entity*>();
				
				Vec3* Ret =&thisPar->rotation();
				
				Input.Set_Return<Vec3*>(Ret);
			
			},(Entity__rotation_ptr)[](ECSExample::Entity* thisPar) ->Vec3*
			{
				return &thisPar->rotation();
			}); 
		lib.Add_CPPCall("ECS:Entity:rotation2d",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				ECSExample::Entity* thisPar = Input.GetParameter<ECSExample::Entity*>();
				
				Vec2* Ret =&thisPar->rotation2d();
				
				Input.Set_Return<Vec2*>(Ret);
			
			},(Entity__rotation2d_ptr)[](ECSExample::Entity* thisPar) ->Vec2*
			{
				return &thisPar->rotation2d();
			}); 
		lib.Add_CPPCall("ECS:Entity:irotation",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				ECSExample::Entity* thisPar = Input.GetParameter<ECSExample::Entity*>();
				
				const Vec3* Ret =&thisPar->irotation();
				
				Input.Set_Return<const Vec3*>(Ret);
			
			},(Entity__irotation_ptr)[](ECSExample::Entity* thisPar) ->const Vec3*
			{
				return &thisPar->irotation();
			}); 
		lib.Add_CPPCall("ECS:Internal:Entity__irotation2d7",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				ECSExample::Entity* thisPar = Input.GetParameter<ECSExample::Entity*>();
				
				const Vec2* Ret =&thisPar->irotation2d();
				
				Input.Set_Return<const Vec2*>(Ret);
			
			},(Entity__irotation2d7_ptr)[](ECSExample::Entity* thisPar) ->const Vec2*
			{
				return &thisPar->irotation2d();
			}); 
		lib.Add_CPPCall("ECS:Internal:Scene__AddEntity1",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				ECSExample::Scene* thisPar = Input.GetParameter<ECSExample::Scene*>();
				
				Entity* Ret =thisPar->AddEntity();
				
				Input.Set_Return<Entity*>(Ret);
			
			},(Scene__AddEntity1_ptr)[](ECSExample::Scene* thisPar) ->Entity*
			{
				return thisPar->AddEntity();
			}); 
	}//Made by UCodeAutoLink End
	}

}


UCodeIDEEnd
