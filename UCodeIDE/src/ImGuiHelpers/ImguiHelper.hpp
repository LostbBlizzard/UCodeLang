#pragma once
#include "ULanguageNameSpace.hpp"
#include <UCodeAnalyzer/Typedef.hpp>
#include <imgui.h>
#include <functional>
#include <UCodeLang/LangCore/ReflectionData.hpp>
#include <UCodeLang/RunTime/AnyInterpreter.hpp>

#include <UCodeLang/RunTime/ReflectionStl.hpp>
UCodeIDEStart


//Most code here was taken from UcodeGameEngine
class ImguiHelper
{
public:
	static UCodeLang::AnyInterpreterPtr _Ptr;

	static bool UCodeObjectField(const char* FieldName, void* Object, const UCodeLang::ClassMethod::Par& type, const UCodeLang::ClassAssembly& assembly,bool IfClassRemoveFlags=false);
	static bool UCodeObjectField(void* Pointer, const UCodeLang::ReflectionTypeInfo& Type, const UCodeLang::ClassAssembly& Assembly, bool IfClassRemoveFlags = false);

	
	static bool UCodeObjectField(const char* FieldName, void* Object, const UCodeLang::ReflectionTypeInfo& type,const UCodeLang::ClassAssembly& assembly, bool IfClassRemoveFlags = false);

	static bool DrawEnum(void* Pointer, const UCodeLang::Enum_Data& Class, const UCodeLang::ClassAssembly& Assembly);

	static bool uInt64Field(const char* FieldName, UInt64& Value);
	static bool uInt32Field(const char* FieldName, UInt32& Value);
	static bool uInt16Field(const char* FieldName, UInt16& Value);
	static bool uInt8Field(const char* FieldName, UInt8& Value);

	static bool uIntptrField(const char* FieldName, uintptr_t& Value);
	static bool IntptrField(const char* FieldName, intptr_t& Value);

	static bool CharField(const char* FieldName, char& Value);

	static bool Int64Field(const char* FieldName, Int64& Value);
	static bool Int32Field(const char* FieldName, Int32& Value);
	static bool Int16Field(const char* FieldName, Int16& Value);
	static bool Int8Field(const char* FieldName, Int8& Value);

	static bool float32Field(const char* FieldName, float32& Value);
	static bool float64Field(const char* FieldName, float64& Value);
	
	static bool InputText(const char* label, String& buffer, ImGuiInputTextFlags flags = 0);
	
	static bool InputText(const char* label, UCodeLang::ReflectionString& buffer, ImGuiInputTextFlags flags = 0);

	
	static bool MultLineText(const char* label, String& buffer, ImVec2 Size, ImGuiInputTextFlags flags = 0);
	
	static bool Vec2float32Field(const char* label, float vec2[2]);
	static bool Vec2float64Field(const char* label, double vec2[2]);
	static bool Vec2IntField(const char* label, UInt64 vec2[2]);
	static bool Vec2IntField(const char* label, Int64 vec2[2]);
	static bool Vec2IntField(const char* label, UInt32 vec2[2]);
	static bool Vec2IntField(const char* label, Int32 vec2[2]);
	static bool Vec2IntField(const char* label, UInt16 vec2[2]);
	static bool Vec2IntField(const char* label, Int16 vec2[2]);
	static bool Vec2IntField(const char* label, UInt8 vec2[2]);
	static bool Vec2IntField(const char* label, Int8 vec2[2]);

	static bool Vec3float32Field(const char* label, float vec3[3]);
	static bool Vec3float64Field(const char* label, double vec3[3]);
	static bool Vec3IntField(const char* label, UInt64 vec3[3]);
	static bool Vec3IntField(const char* label, Int64 vec3[3]);
	static bool Vec3IntField(const char* label, UInt32 vec3[3]);
	static bool Vec3IntField(const char* label, Int32 vec3[3]);
	static bool Vec3IntField(const char* label, UInt16 vec3[3]);
	static bool Vec3IntField(const char* label, Int16 vec3[3]);
	static bool Vec3IntField(const char* label, UInt8 vec3[3]);
	static bool Vec3IntField(const char* label, Int8 vec3[3]);

	template<typename T>
	struct EnumValue
	{
		const char* label = "null";
		T Value = T();
		EnumValue(const char* Label, T value)
		{
			label = Label;
			Value = value;
		}
	};
	static bool BoolEnumField(const char* label, bool& Value)
	{
		return EnumField<bool>(label, Value, BoolEnumValues, sizeof(BoolEnumValues) / sizeof(BoolEnumValues[1]));
	}



	struct EnumValue2
	{
		const char* label = "null";
		const void* Value = nullptr;
	};

	static bool EnumField(const char* label, void* Value, const EnumValue2* Values, size_t ValuesSize, size_t EnumBaseSize);

	static bool EnumField(const char* label, void* Value, const Vector<EnumValue2>& Values, size_t EnumBaseSize)
	{
		return EnumField(label, Value, Values.data(), Values.size(), EnumBaseSize);
	}

	template<typename T>
	static bool EnumField(const char* label, T& Value, const Vector<EnumValue<T>>& Values)
	{
		return EnumField(label, Value, Values.data(), Values.size());
	}

	template<typename T>
	static bool EnumField(const char* label, T& Value, const EnumValue<T>* Values, size_t Size)
	{
		constexpr size_t MaxStackSize = sizeof(EnumValue2) * 8;
		bool IsEnumValuesBig = (Size * sizeof(EnumValue2)) > MaxStackSize;


		EnumValue2* Ptr = nullptr;
		if (IsEnumValuesBig)
		{
			Ptr = new EnumValue2[Size];
		}
		else
		{
			Ptr = new (alloca(Size * sizeof(EnumValue2))) EnumValue2[Size];
		}

		for (size_t i = 0; i < Size; i++)
		{
			EnumValue2& Enum2Value = Ptr[i];
			const EnumValue<T>& EnumValue = Values[i];

			Enum2Value.label = EnumValue.label;
			Enum2Value.Value = &EnumValue.Value;
		}

		bool V = EnumField(label, &Value, Ptr, Size, sizeof(T));

		if (IsEnumValuesBig)
		{
			delete[] Ptr;
		}
		return V;
	}

	struct EnumVariantFieldUpdate
	{
		bool EnumUpdated = false;
		bool VariantUpdated = false;
	};
	struct VariantInfo
	{
		void* Tag = nullptr;
		void* Union = nullptr;
	};
	static EnumVariantFieldUpdate EnumVariantField(const char* label, VariantInfo Variant, std::function<bool(void* Tag, void* Union, bool UpdatedEnum, bool Draw)> DrawVariant, const EnumValue2* Values, size_t ValuesSize, size_t EnumBaseSize)
	{
		EnumVariantFieldUpdate V;

		bool IsOpen = ImGui::TreeNode((Byte*)Variant.Tag + 1, "");
		ImGui::SameLine();
		V.EnumUpdated = EnumField(label, Variant.Tag, Values, ValuesSize, EnumBaseSize);
		V.VariantUpdated = DrawVariant(Variant.Tag, Variant.Union, V.EnumUpdated, IsOpen);

		if (IsOpen)
		{
			ImGui::TreePop();
		}
		return V;
	}
	
	struct DrawVectorInfo
	{
		size_t ItemSize = 0;

		std::function<void(void* Object, size_t Index)> _OnDrawItem;
		Optional<std::function<void(void* Object, size_t Index)>> _AddNewValue;
		Optional<std::function<void(void* Object, size_t Index)>> _AddNewRemove;
		Optional<std::function<void(void* Object, size_t NewSize)>> _ResizeVector;
	};
	static bool InputSize_t(const char* label, size_t* v,ImGuiInputTextFlags flags = 0);
	static bool InputSize_t(const char* label, intptr_t* v, ImGuiInputTextFlags flags = 0);


	static bool DrawVector(const char* label, UCodeLang::ReflectionVector& vector, const UCodeLang::ClassAssembly& assembly);

	static bool DrawVector(const char* label, void* Object, void* Buffer, size_t Size, const DrawVectorInfo& Item);

	template<typename T>
	static bool DrawVector(const char* label, Vector<T>& Buffer)
	{
		DrawVectorInfo Info;
		Info.ItemSize = sizeof(T);

		Info._OnDrawItem = [](void* Object, size_t Index)
		{
			Vector<T>& Objectbuf = *(Vector<T>*)Object;
			auto& Item = Objectbuf[Index];

			DrawValue<T>(&Item);
		};

		Info._AddNewValue = [](void* Object, size_t Index)
		{
			Vector<T>& Objectbuf = *(Vector<T>*)Object;
			Objectbuf.insert(Objectbuf.begin() + Index, T());
		};

		Info._AddNewRemove = [](void* Object, size_t Index)
		{
			Vector<T>& Objectbuf = *(Vector<T>*)Object;
			Objectbuf.erase(Objectbuf.begin() + Index);
		};

		Info._ResizeVector = [](void* Object, size_t NewIndex)
		{
			Vector<T>& Objectbuf = *(Vector<T>*)Object;
			Objectbuf.resize(NewIndex);
		};

		return DrawVector(label, &Buffer, Buffer.data(), Buffer.size(), Info);
	}


	template<typename T>
	static bool DrawSpan(const char* label, Span<T>& Buffer)
	{
		DrawVectorInfo Info;
		Info.ItemSize = sizeof(T);

		Info._OnDrawItem = [](void* Object, size_t Index)
		{
			UCodeLang::Span<T>& Objectbuf = *(UCodeLang::Span<T>*)Object;
			auto& Item = Objectbuf[Index];

			return DrawValue<T>(&Item);
		};


		return DrawVector(label, &Buffer, Buffer.Data(), Buffer.Size(), Info);
	}

	template<typename T, size_t Size>
	static bool DrawArray(const char* label, Array<T, Size>& Buffer)
	{
		DrawVectorInfo Info;
		Info.ItemSize = sizeof(T);

		Info._OnDrawItem = [](void* Object, size_t Index)
		{
			UCodeLang::Array<T, Size>& Objectbuf = *(UCodeLang::Array<T, Size>*)Object;
			auto& Item = Objectbuf[Index];

			DrawValue<T>(&Item);
		};


		return DrawVector(label, &Buffer, Buffer.data(), Buffer.size(), Info);
	}

	
	template<typename T> static bool DrawValueWithLabel(const char* label, T* Item)
	{
		auto r = DrawValue<T>(Item);
		ImGui::SameLine();
		ImGui::Text(label);
		return r;
	}
	template<typename T> static bool DrawValue(T* Item)
	{
		static_assert(true,"function has no body");
		return false;
	}

	 static bool DrawValue(bool* Item)
	{
		ImGui::PushID(Item);
		auto R = BoolEnumField("", *Item);
		ImGui::PopID();
		return R;
	}
	 static bool DrawValue(char* Item)
	{
		ImGui::PushID(Item);
		auto R = CharField("", *Item);
		ImGui::PopID();
		return R;
	}
	 static bool DrawValue(Int64* Item)
	{
		ImGui::PushID(Item);
		auto R = ImGui::InputScalar("", ImGuiDataType_::ImGuiDataType_S64, Item);
		ImGui::PopID();
		return R;
	}
	 static bool DrawValue(UInt64* Item)
	{
		ImGui::PushID(Item);
		auto R = ImGui::InputScalar("", ImGuiDataType_::ImGuiDataType_U64, Item);
		ImGui::PopID();
		return R;
	}
	 static bool DrawValue(Int32* Item)
	{
		ImGui::PushID(Item);
		auto R = ImGui::InputScalar("", ImGuiDataType_::ImGuiDataType_S32, Item);
		ImGui::PopID();
		return R;
	}
	 static bool DrawValue(UInt32* Item)
	{
		ImGui::PushID(Item);
		auto R = ImGui::InputScalar("", ImGuiDataType_::ImGuiDataType_U32, Item);
		ImGui::PopID();
		return R;
	}
	 static bool DrawValue(Int16* Item)
	{
		ImGui::PushID(Item);
		auto R = ImGui::InputScalar("", ImGuiDataType_::ImGuiDataType_S16, Item);
		ImGui::PopID();
		return R;
	}
	 static bool DrawValue(UInt16* Item)
	{
		ImGui::PushID(Item);
		auto R = ImGui::InputScalar("", ImGuiDataType_::ImGuiDataType_U16, Item);
		ImGui::PopID();
		return R;
	}
	 static bool DrawValue(Int8* Item)
	{
		ImGui::PushID(Item);
		auto R = ImGui::InputScalar("", ImGuiDataType_::ImGuiDataType_S8, Item);
		ImGui::PopID();
		return R;
	}
	 static bool DrawValue(UInt8* Item)
	{
		ImGui::PushID(Item);
		auto R = ImGui::InputScalar("", ImGuiDataType_::ImGuiDataType_U8, Item);
		ImGui::PopID();
		return R;
	}
	 static bool DrawValue(float* Item)
	{
		ImGui::PushID(Item);
		auto R = ImGui::InputScalar("", ImGuiDataType_::ImGuiDataType_Float, Item);
		ImGui::PopID();
		return R;
	}
	 static bool DrawValue(double* Item)
	{
		ImGui::PushID(Item);
		auto R = ImGui::InputScalar("", ImGuiDataType_::ImGuiDataType_Float, Item);
		ImGui::PopID();
		return R;
	}
	 static bool DrawValue(String* Item)
	{
		ImGui::PushID(Item);
		auto R = InputText("", *Item);
		ImGui::PopID();
		return R;
	}
	template<typename T> static bool DrawValue(Vector<T>* Item)
	{
		ImGui::PushID(Item);
		auto R = DrawVector<T>("", *Item);
		ImGui::PopID();
		return R;
	}

	template<typename T, size_t Size> static bool DrawValue(Array<T, Size>* Item)
	{
		ImGui::PushID(Item);
		auto R = DrawArray("", *Item);
		ImGui::PopID();
		return R;
	}
	template<typename T> static bool DrawValue(Span<T>* Item)
	{
		ImGui::PushID(Item);
		auto R = DrawSpan<T>("", *Item);
		ImGui::PopID();
		return R;
	}
private:
	inline static const EnumValue<bool> BoolEnumValues[] =
	{
		EnumValue<bool>("false",false),
		EnumValue<bool>("true",true)
	};
};

UCodeIDEEnd