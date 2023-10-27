#pragma once
#include <UCodeLang/UCodeLang.hpp>
#include "imgui.h"

namespace ECSExample
{
	template<typename T>
	using Unique_ptr = UCodeLang::Unique_ptr<T>;
	
	using String = UCodeLang::String;

	template<typename T>
	using Vector = UCodeLang::Vector<T>;

	using Vec3 = struct
	{
		float X;
		float Y;
		float Z;
	};
	using Vec2 = ImVec2;

	struct Component
	{
		virtual void Start()
		{

		}
		virtual void Update()
		{

		}
		virtual ~Component()
		{

		}
	public:
		Entity* entity()
		{
			return myentity;
		}
		const Entity* entity() const
		{
			return myentity;
		}

		bool calledstart = false;
	private:
		Entity* myentity =nullptr;
	};

	struct Scene;

	struct Entity
	{
		Vec3 Position;
		Vec3 Rotation;
		Vec3 Scale;

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

		String Name;
		Vector<Unique_ptr<Component>> Components;
		Vector<Unique_ptr<Entity>> ChildEntitys;
		Scene* myscenc =nullptr;
		Entity* myparent = nullptr;

		Scene* scenc()
		{
			return  myscenc;
		}
		const Scene* scenc() const
		{
			return  myscenc;
		}


		Entity* AddChildEntity()
		{
			ChildEntitys.push_back(std::make_unique<Entity>());
			auto r = ChildEntitys.back().get();
			r->myscenc = myscenc;
			r->myparent = this;

			return r;
		}

		template<typename T>
		T* AddComponent()
		{
			Components.push_back(std::make_unique<T>());
			auto r = Components.back().get();
			r->myscenc = myscenc;
			r->myentity = this;

			return r;
		}
	};

	struct Scene
	{
		Vector<Unique_ptr<Entity>> Entitys;

		void Update()
		{
			for (size_t i = 0; i < Entitys.size(); i++)
			{
				Entitys[i]->Update();
			}
		}
		Entity* AddEntity()
		{
			Entitys.push_back(std::make_unique<Entity>());
			auto r = Entitys.back().get();
			r->myscenc = this;

			return r;
		}

	};

	static UCodeLang::RunTimeLangState State;
	struct RunTime
	{
		UCodeLang::AnyInterpreter Interpreter;
		Scene scene;

		void ImguiDraw()
		{

		}
	};
	thread_local RunTime _Context;

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