#pragma once
#include <UCodeLang/UCodeLang.hpp>
#include "imgui.h"
#include <ImGuiHelpers/ImguiHelper.hpp>

UCodeIDEStart

namespace ECSExample
{

	void ECSLink(UCodeLang::RunTimeLib& lib);

	template<typename T>
	using Unique_ptr = UCodeLang::Unique_ptr<T>;
	
	using String = UCodeLang::String;

	template<typename T>
	using Vector = UCodeLang::Vector<T>;

	struct Vec3
	{
		float X = 0;
		float Y = 0;
		float Z = 0;
	};
	using Vec2 = ImVec2;
	
	template<typename T>
	using NullablePtr = UCodeLang::NullablePtr<T>;

	using String_view = UCodeLang::String_view;
	
	template<typename T>
	struct Object
	{
	public:
		Object()
		{

		}
		Object(T* val)
		{
			base = std::make_shared<T*>(val);
		}

		void OnDestroy()
		{
			*base.get() = nullptr;
		}
		bool IsDestroyed()
		{
			if (base.get())
			{
				return (*base.get()) == nullptr;
			}
			return false;
		}
		bool Exist() {return !IsDestroyed();}

		T* Get() { return *(base.get()); }

		NullablePtr<T> GetPtr()
		{
			if (Exist())
			{
				return UCodeLang::Nullableptr(Get());
			}
			return {};
		}

		Object<const T> AsReadOnly() const
		{
			Object<const T> r;
			r.base = *(UCodeLang::Shared_ptr<const T*>*)&base;
			return r;
		}
	//private:
		UCodeLang::Shared_ptr<T*> base;
	};

	template<typename T>
	Object<T> obj(T* val)
	{
		return Object<T>(val);
	}



	struct Entity;
	UCodeLangExportSymbol("ECS") UCodeLangExportTrait Component
	{
	public:
		Component()
		{
			myobj = obj(this);
		}
		virtual void Start()
		{

		}
		virtual void Update()
		{

		}
		virtual ~Component()
		{
			myobj.OnDestroy();
		}


		UCodeLangExport Entity* entity()
		{
			return myentity;
		}
		UCodeLangExport const Entity* ientity() const
		{
			return myentity;
		}

		UCodeLangExport void Destroy()
		{
			isdestroyed = true;
		}

		UCodeLangExport Object<Component> object() { return myobj; }
		UCodeLangExport Object<const Component> iobject() const { return myobj.AsReadOnly(); }

		

		bool calledstart = false;
		bool isdestroyed = false;
	private:
		Entity* myentity =nullptr;
		Object<Component> myobj;
	};

	struct Scene;

	UCodeLangExportSymbol("ECS") struct Entity
	{
		Vec3 Position;
		Vec3 Rotation;
		Vec3 Scale;

		Entity()
		{
			myobj = obj(this);
		}
		~Entity()
		{
			myobj.OnDestroy();
		}

		void Update()
		{
			for (size_t i = 0; i < Components.size(); i++)
			{
				auto Item = Components[i].get();

				if (Item->calledstart)
				{
					Item->Update();
				}
				else
				{
					Item->Start();
					Item->calledstart = true;
				}
			}
			for (size_t i = 0; i < ChildEntitys.size(); i++)
			{
				auto Item = ChildEntitys[i].get();
				Item->Update();
			}

		}

		String Name = "New Entity";
		Vector<Unique_ptr<Component>> Components;
		Vector<Unique_ptr<Entity>> ChildEntitys;
		Scene* myscenc = nullptr;
		Entity* myparent = nullptr;
		Object<Entity> myobj;
		bool isdestroyed = false;

		UCodeLangExport  Scene* scenc()
		{
			return  myscenc;
		}
		UCodeLangExport  const Scene* scenc() const
		{
			return  myscenc;
		}


		UCodeLangExport Entity* AddChildEntity()
		{
			ChildEntitys.push_back(std::make_unique<Entity>());
			auto r = ChildEntitys.back().get();
			r->myscenc = myscenc;
			r->myparent = this;

			return r;
		}
		UCodeLangExport void Destroy()
		{
			isdestroyed = true;
		}

		template<typename T>
		T* AddComponent()
		{
			Components.push_back(std::make_unique<T>());
			auto r = Components.back().get();
			r->myentity = this;

			return r;
		}

		UCodeLangExport Object<Entity> object() { return myobj; }
		UCodeLangExport Object<const Entity> iobject() const { return myobj.AsReadOnly(); }

		//for this we allow geting the actual pointer of the member. you may not want this
		UCodeLangExport String& name()
		{
			return Name;
		}
		UCodeLangExport const StringView iname() const
		{
			return Name;
		}

		UCodeLangExport Vec3& position()
		{
			return Position;
		}
		UCodeLangExport Vec2& position2d()
		{
			return (Vec2&)Position;
		}

		UCodeLangExport const Vec3& iposition() const
		{
			return Position;
		}
		UCodeLangExport const Vec2& iposition2d() const
		{
			return (const Vec2&)Position;
		}

		UCodeLangExport Vec3& scale()
		{
			return Scale;
		}
		UCodeLangExport Vec2& scale2d()
		{
			return (Vec2&)Scale;
		}

		UCodeLangExport const Vec3& iscale() const
		{
			return Scale;
		}
		UCodeLangExport const Vec2& iscale2d() const
		{
			return (Vec2&)Scale;
		}

		UCodeLangExport Vec3& rotation()
		{
			return Rotation;
		}
		UCodeLangExport Vec2& rotation2d()
		{
			return (Vec2&)Rotation;
		}

		UCodeLangExport const Vec3& irotation() const
		{
			return Rotation;
		}
		UCodeLangExport const Vec2& irotation2d() const
		{
			return (Vec2&)Rotation;
		}

		UCodeLangExport NullablePtr<Entity> parent()
		{
			return UCodeLang::Nullableptr(myparent);
		}
		UCodeLangExport const NullablePtr<Entity> iparent() const
		{
			return UCodeLang::Nullableptr(myparent);
		}

		UCodeLangExport Object<Entity> parentobj()
		{
			if (myparent)
			{
				return myparent->object();
			}
			return {};
		}
		UCodeLangExport Object<const Entity> iparentobj() const
		{
			if (myparent)
			{
				return myparent->iobject();
			}
			return {};
		}

		UCodeLangExport Vector<Entity*> childentitys()
		{
			Vector<Entity*> r;
			r.resize(ChildEntitys.size());

			for (size_t i = 0; i < r.size(); i++)
			{
				auto& Item = ChildEntitys[i];
				r.push_back(Item.get());
			}

			return r;
		}
		UCodeLangExport Vector<const Entity*> ichildentitys() const
		{
			Vector<const Entity*> r;
			r.resize(ChildEntitys.size());

			for (size_t i = 0; i < r.size(); i++)
			{
				auto& Item = ChildEntitys[i];
				r.push_back(Item.get());
			}

			return r;
		}

		UCodeLangExport Vector<Object<Entity>> childentitysobj()
		{
			Vector<Object<Entity>> r;
			r.resize(ChildEntitys.size());

			for (size_t i = 0; i < r.size(); i++)
			{
				auto& Item = ChildEntitys[i];
				r.push_back(Item->object());
			}

			return r;
		}
		UCodeLangExport Vector<Object<const Entity>> ichildentitysobj() const
		{
			Vector<Object<const Entity>> r;
			r.resize(ChildEntitys.size());

			for (size_t i = 0; i < r.size(); i++)
			{
				auto& Item = ChildEntitys[i];
				r.push_back(Item->iobject());
			}

			return r;
		}
	};

	struct Scene
	{
		Vector<Unique_ptr<Entity>> Entitys;
		Object<Scene> myobj;


		void Update()
		{
			for (size_t i = 0; i < Entitys.size(); i++)
			{
				Entitys[i]->Update();
			}


			//remove all destroyed entitys
			std::remove_if(Entitys.begin(), Entitys.end(),
				[](Unique_ptr<Entity>& val) {
					if (val->isdestroyed)
					{
						return true;
					}
					return false;
				}
			);
		}
		UCodeLangExport Entity* AddEntity()
		{
			Entitys.push_back(std::make_unique<Entity>());
			auto r = Entitys.back().get();
			r->myscenc = this;

			return r;
		}

		UCodeLangExport Object<Scene> object() { return myobj; }
		UCodeLangExport Object<const Scene> iobject() const { return myobj.AsReadOnly(); }
	};

	static UCodeLang::RunTimeLangState State;
	struct RunTime
	{
		UCodeLang::AnyInterpreter Interpreter;
		Scene scene;

		void ImguiDraw(Entity* entity)
		{
			if (ImGui::TreeNode(entity, entity->Name.c_str()))
			{
				if (ImGui::Button("New ChildEntity"))
				{
					entity->AddChildEntity();
				}
				ImGui::SameLine();
				if (ImGui::Button("Destroy Entity"))
				{
					entity->Destroy();
				}


				ImguiHelper::InputText("Name", entity->Name);
				ImguiHelper::Vec3float32Field("Position", &entity->Position.X);
				ImguiHelper::Vec3float32Field("Rotation", &entity->Rotation.X);
				ImguiHelper::Vec3float32Field("Scale", &entity->Scale.X);

				for (auto& Item : entity->ChildEntitys)
				{
					ImguiDraw(Item.get());
				}

				ImGui::TreePop();
			}
		}

		void ImguiDraw()
		{
			if (ImGui::Button("New Entity"))
			{
				scene.AddEntity();
			}
			ImGui::Separator();
			
			for (auto& Item : scene.Entitys)
			{
				ImguiDraw(Item.get());
			}
		}
	};
	inline thread_local RunTime _Context;

	static UCodeLang::RunTimeLangState& Get_State()
	{
		return State;
	}
	static UCodeLang::AnyInterpreter& Get_Interpreter()
	{
		return _Context.Interpreter;
	}

	struct UCodeComponent :public Component
	{
		void SetClass(const UCodeLang::Class_Data* Class)
		{

			if (_Class)
			{
				UnloadClass();
			}
			_Class = Class;

			_Start = Class->Get_ClassMethod("Start");
			_Update = Class->Get_ClassMethod("Update");

			_Contructer = Class->Get_ClassMethod("Start");
			_Destructor = Class->Get_ClassMethod("Update");

			UCodeLang::ReflectionTypeInfo type;
			type._Type = UCodeLang::ReflectionTypes::CustomType;
			type._CustomTypeID = Class->TypeID;

			auto classsize = Get_State().Get_Assembly().GetSize(type,sizeof(void*) == 4).value();


			ULangObject = Get_State().Malloc(classsize);
			if (_Contructer)
			{
				Get_Interpreter().ThisCall(_Contructer, ULangObject);
			}
			_CalledULangObjectStart = false;
		}
		void UnloadClass()
		{
			if (_Destructor)
			{
				Get_Interpreter().ThisCall(_Destructor, ULangObject);
			}
			Get_State().Free(ULangObject);
			ULangObject = nullptr;
			
			_CalledULangObjectStart = false;
			_Class = nullptr;
			_Start = nullptr;
			_Update = nullptr;

			_Contructer = nullptr;
			_Destructor = nullptr;
		}

		
		void Update() override
		{
			if (_Class)
			{
				if (_CalledULangObjectStart)
				{
					if (_Update) {
						Get_Interpreter().ThisCall(_Update, ULangObject);
					}
				}
				else
				{
					_CalledULangObjectStart = true;
					if (_Start) {
						Get_Interpreter().ThisCall(_Start, ULangObject);
					}
				}
			}
		}
		~UCodeComponent() override
		{
			if (_Class)
			{
				UnloadClass();
			}
		}

		const UCodeLang::Class_Data*  Get_ClassData()
		{
			return _Class;
		}
	private:
		const UCodeLang::Class_Data* _Class = nullptr;

		const UCodeLang::ClassMethod* _Start = nullptr;
		const UCodeLang::ClassMethod* _Update =nullptr;


		const UCodeLang::ClassMethod* _Contructer = nullptr;
		const UCodeLang::ClassMethod* _Destructor = nullptr;
		
		void* ULangObject =nullptr;

		bool _CalledULangObjectStart = false;
	};
}

UCodeIDEEnd