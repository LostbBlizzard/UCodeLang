#include "ImguiHelper.hpp"
#include "Imgui/misc/cpp/imgui_stdlib.h"
UCodeIDEStart


UCodeLang::AnyInterpreterPtr ImguiHelper::_Ptr;


bool ImguiHelper::UCodeObjectField(const char* FieldName, void* Object, const UCodeLang::ClassMethod::Par& type, const UCodeLang::ClassAssembly& assembly, bool IfClassRemoveFlags )
{
	if (type.IsOutPar)
	{
		return false;
	}
	return UCodeObjectField(FieldName,Object,type.Type,assembly, IfClassRemoveFlags);
}

bool ImguiHelper::UCodeObjectField(void* Pointer, const UCodeLang::ReflectionTypeInfo& Type, const UCodeLang::ClassAssembly& Assembly, bool IfClassRemoveFlags)
{
	ImGui::PushID(Pointer);
	auto r = UCodeObjectField("",Pointer,Type,Assembly, IfClassRemoveFlags);
	ImGui::PopID();

	return r;
}

bool ImguiHelper::UCodeObjectField(const char* FieldName, void* Object, const UCodeLang::ReflectionTypeInfo& type, const UCodeLang::ClassAssembly& assembly, bool IfClassRemoveFlags)
{
	switch (type._Type)
	{
	case UCodeLang::ReflectionTypes::Bool:
	{
		return BoolEnumField(FieldName, *(bool*)Object);
	}
	break;
	case UCodeLang::ReflectionTypes::uInt8:
	{
		return uInt8Field(FieldName, *(UInt8*)Object);
	}
	break;
	case UCodeLang::ReflectionTypes::uInt16:
	{
		return uInt16Field(FieldName, *(UInt16*)Object);
	}
	break;
	case UCodeLang::ReflectionTypes::uInt32:
	{
		return uInt32Field(FieldName, *(UInt32*)Object);
	}
	break;
	case UCodeLang::ReflectionTypes::uInt64:
	{
		return uInt64Field(FieldName, *(UInt64*)Object);
	}
	break;
	case UCodeLang::ReflectionTypes::sInt8:
	{
		return Int8Field(FieldName, *(Int8*)Object);
	}
	break;
	case UCodeLang::ReflectionTypes::sInt16:
	{
		return Int16Field(FieldName, *(Int16*)Object);
	}
	break;
	case UCodeLang::ReflectionTypes::sInt32:
	{
		return Int32Field(FieldName, *(Int32*)Object);
	}
	break;
	case UCodeLang::ReflectionTypes::sInt64:
	{
		return Int64Field(FieldName, *(Int64*)Object);
	}
	break;
	case UCodeLang::ReflectionTypes::Char:
	{
		return CharField(FieldName, *(char*)Object);
	}
	break;
	case UCodeLang::ReflectionTypes::float32:
	{
		return float32Field(FieldName, *(float32*)Object);
	}
	break;
	case UCodeLang::ReflectionTypes::float64:
	{
		return float64Field(FieldName, *(float64*)Object);
	}
	break;
	case UCodeLang::ReflectionTypes::CustomType:
	{
		auto Syb = assembly.Find_Node(type);
		if (Syb)
		{
			if (Syb->Get_Type() == UCodeLang::ClassType::Enum)
			{
				auto& EnumInfo = Syb->Get_EnumData();
				ImGui::Text(FieldName);
				ImGui::SameLine();
				return DrawEnum(Object, EnumInfo, assembly);
			}
			else if (Syb->Get_Type() == UCodeLang::ClassType::Class)
			{
				auto& ClassInfo = Syb->Get_ClassData();

				bool IsOpen = true;

				if (!IfClassRemoveFlags) {
				IsOpen = ImGui::TreeNode((Byte*)Object + 1, FieldName);
				}

				bool WasUpdated = false;

				if (IsOpen) 
				{
					for (auto& Field : ClassInfo.Fields)
					{
						void* FieldObject = (void*)((uintptr_t)Object + (uintptr_t)Field.offset);
						if (UCodeObjectField(Field.Name.c_str(), FieldObject, Field.Type, assembly))
						{
							WasUpdated = true;
						}
					}
				}

				if (IsOpen)
				{
					if (!IfClassRemoveFlags) 
					{
						ImGui::TreePop();
					}
				}
				return WasUpdated;
			}
		}
		return false;
	}
		break;
	default:
		break;
	}
}

bool ImguiHelper::DrawEnum(void* Pointer, const UCodeLang::Enum_Data& Class, const UCodeLang::ClassAssembly& Assembly)
{
	const bool Is32Mode = sizeof(void*) == 4;

	bool R = false;
	if (!Class.EnumVariantUnion.has_value())
	{
		ImGui::PushID(Pointer);

		auto Size = Assembly.GetSize(Class.BaseType, Is32Mode);
		if (Size.has_value())
		{
			Vector<EnumValue2> _Eunm;
			_Eunm.resize(Class.Values.size());
			for (size_t i = 0; i < Class.Values.size(); i++)
			{
				auto& ImguiEnum = _Eunm[i];
				const auto& ClassItem = Class.Values[i];

				ImguiEnum.label = ClassItem.Name.c_str();
				ImguiEnum.Value = ClassItem._Data.Get_Data();
			}

			R = EnumField("", Pointer, _Eunm, *Size);
		}
		ImGui::PopID();
	}
	else
	{
		ImGui::PushID(Pointer);

		auto Size = Assembly.GetSize(Class.BaseType, Is32Mode);
		if (Size.has_value())
		{
			size_t TagSize = *Size;

			VariantInfo _Info;
			_Info.Tag = Pointer;
			_Info.Union = (void*)(TagSize + (uintptr_t)Pointer);

			Vector<EnumValue2> _Eunm;
			_Eunm.resize(Class.Values.size());
			for (size_t i = 0; i < Class.Values.size(); i++)
			{
				auto& ImguiEnum = _Eunm[i];
				const auto& ClassItem = Class.Values[i];

				ImguiEnum.label = ClassItem.Name.c_str();
				ImguiEnum.Value = ClassItem._Data.Get_Data();
			}
			size_t UnionTypeID = Class.EnumVariantUnion.value();

			std::shared_ptr<Byte> LastTag = std::shared_ptr<Byte>(new Byte[TagSize]);
			memcpy(LastTag.get(), _Info.Tag, TagSize);

			std::function<bool(void* Tag, void* Union, bool UpdatedEnum, bool Draw)> OnDraw = [Is32Mode,UnionTypeID, &Assembly, Class, TagSize, LastTag](void* Tag, void* Union, bool UpdatedEnum, bool Draw)
			{
				bool Updated = false;

				if (UpdatedEnum)
				{

					{//drop old object.
						const UCodeLang::EnumValues* _UseingValue = nullptr;
						for (size_t i = 0; i < Class.Values.size(); i++)
						{
							auto& Item = Class.Values[i];
							if (!memcmp(LastTag.get(), Item._Data.Get_Data(), TagSize))
							{
								_UseingValue = &Item;
							}
						}
						if (_UseingValue)
						{
							if (_UseingValue->EnumVariantType.has_value())
							{
								auto TypeToInit = _UseingValue->EnumVariantType.value();
								auto Ptr = Union;
								//To-DO drop object
								auto ItWorked = Assembly.CallDestructor(TypeToInit, Ptr, Is32Mode);
								if (ItWorked.has_value())
								{
									if (ItWorked.value().has_value())
									{
										auto& ToCall = ItWorked.value().value();
										for (auto& Item : ToCall) 
										{
											_Ptr.ThisCall(Item.MethodToCall, Item.ThisPtr);
										}
									}

								}
							}
						}
					}

					{//init object
						const UCodeLang::EnumValues* _UseingValue = nullptr;
						for (size_t i = 0; i < Class.Values.size(); i++)
						{
							auto& Item = Class.Values[i];
							if (!memcmp(Tag, Item._Data.Get_Data(), TagSize))
							{
								_UseingValue = &Item;
							}
						}
						if (_UseingValue)
						{
							if (_UseingValue->EnumVariantType.has_value())
							{
								auto& TypeToInit = _UseingValue->EnumVariantType.value();
								auto Ptr = Union;

								//init new object
								auto ItWorked = Assembly.CallDefaultConstructor(TypeToInit, Ptr, Is32Mode);
								if (ItWorked.has_value())
								{
									if (ItWorked.value().has_value())
									{
										auto& Value = ItWorked.value().value();
										for (auto& Item : Value)
										{
											_Ptr.ThisCall(Item.MethodToCall, Item.ThisPtr);
										}
									}

								}
								
							}
						}
					}

				}

				if (Draw)
				{
					const UCodeLang::EnumValues* _UseingValue = nullptr;
					for (size_t i = 0; i < Class.Values.size(); i++)
					{
						auto& Item = Class.Values[i];
						if (!memcmp(Tag, Item._Data.Get_Data(), TagSize))
						{
							_UseingValue = &Item;
						}
					}

					if (_UseingValue)
					{
						if (_UseingValue->EnumVariantType.has_value())
						{
							Updated = UCodeObjectField(Union, _UseingValue->EnumVariantType.value(), Assembly, true);
						}
					}
				}

				return Updated;
			};

			auto Ret = EnumVariantField("", _Info, OnDraw, _Eunm.data(), _Eunm.size(), TagSize);
			R = Ret.EnumUpdated || Ret.VariantUpdated;
		}
		ImGui::PopID();
	}
	return R;
}

bool ImguiHelper::uInt64Field(const char* FieldName, UInt64& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();

	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_U64, (void*)&Value);
	ImGui::PopID();

	return V;
}

bool ImguiHelper::uInt32Field(const char* FieldName, UInt32& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();

	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_U32, (void*)&Value);
	ImGui::PopID();

	return V;
}

bool ImguiHelper::uInt16Field(const char* FieldName, UInt16& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();

	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_U16, (void*)&Value);
	ImGui::PopID();

	return V;
}

bool ImguiHelper::uInt8Field(const char* FieldName, UInt8& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();

	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_U8, (void*)&Value);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::InputText(const char* label, String& buffer, ImGuiInputTextFlags flags)
{
	ImGui::Text(label);
	ImGui::SameLine();

	ImGui::PushID(&buffer);
	auto V = ImGui::InputText("", &buffer, flags);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::MultLineText(const char* label, String& buffer, ImVec2 Size, ImGuiInputTextFlags flags)
{
	return  ImGui::InputTextMultiline(label, &buffer, Size, flags);
}
bool ImguiHelper::CharField(const char* FieldName, char& Value)
{



	thread_local String V;
	V.resize(1);
	V[0] = Value;

	ImGui::PushID(&Value);
	bool R = InputText("", V);
	ImGui::PopID();

	if (R)
	{
		Value = V[0];
	}

	ImGui::SameLine();

	ImGui::BeginDisabled(true);

	ImGui::SetNextItemWidth(ImGui::GetTextLineHeightWithSpacing());

	ImGui::PushID(&Value + 1);

	ImGui::DragScalar("", ImGuiDataType_S8, (void*)&Value);

	ImGui::PopID();

	ImGui::EndDisabled();

	return R;
}
bool ImguiHelper::Int64Field(const char* FieldName, Int64& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();

	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_S64, (void*)&Value);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::Int32Field(const char* FieldName, Int32& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();

	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_S32, (void*)&Value);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::Int16Field(const char* FieldName, Int16& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();

	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_S16, (void*)&Value);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::Int8Field(const char* FieldName, Int8& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();

	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_S8, (void*)&Value);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::float32Field(const char* FieldName, float32& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();

	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_Float, (void*)&Value);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::float64Field(const char* FieldName, float64& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();

	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_Double, (void*)&Value);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::EnumField(const char* label, void* Value, const EnumValue2* Values, size_t ValuesSize, size_t EnumBaseSize)
{
	bool Updated = false;
	const EnumValue2* current_item = nullptr;
	for (size_t i = 0; i < ValuesSize; i++)
	{
		const EnumValue2& Item = Values[i];

		bool IsSame = !memcmp(Item.Value, Value, EnumBaseSize);
		if (IsSame)
		{
			current_item = &Item;
		}
	}

	if (current_item == nullptr)
	{
		if (ValuesSize == 0)
		{
			ImGui::Text("Cant show EnumField because there is no EnumValues");
			return false;
		}
		current_item = &Values[0];

	}

	ImGui::Text(label); ImGui::SameLine();
	ImGui::PushID(Value);
	if (ImGui::BeginCombo("", current_item->label, ImGuiComboFlags_NoArrowButton))
	{
		for (size_t i = 0; i < ValuesSize; i++)
		{
			const EnumValue2& Item = Values[i];

			bool is_selected = (current_item == &Item);
			if (ImGui::Selectable(Item.label, is_selected))
			{
				memcpy(Value, Item.Value, EnumBaseSize);
				Updated = true;
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::PopID();


	return Updated;
}
bool ImguiHelper::InputSize_t(const char* label, size_t* v, int step, int step_fast, ImGuiInputTextFlags flags)
{
	if constexpr (sizeof(size_t) == sizeof(UInt64))
	{
		return ImGui::InputScalar(label, ImGuiDataType_::ImGuiDataType_U64, v, nullptr, nullptr, nullptr, flags);
	}
	else
	{
		return ImGui::InputScalar(label, ImGuiDataType_::ImGuiDataType_U32, v, nullptr, nullptr, nullptr, flags);
	}
}
bool ImguiHelper::DrawVector(const char* label, void* Object, void* Buffer, size_t Size, const DrawVectorInfo& Item)
{
	bool WasUpdated = false;
	bool Value = ImGui::TreeNode(label);

	size_t NewSize = Size;

	ImGui::SameLine();

	ImGui::BeginDisabled(!Item._ResizeVector.has_value());
	{
		ImGui::PushID(&Item);
		bool ResizeWasUpdated = InputSize_t("", &NewSize, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue);
		ImGui::PopID();

		if (ResizeWasUpdated && Item._ResizeVector.has_value())
		{
			WasUpdated = true;
			Item._ResizeVector.value()(Object, NewSize);
		}

	}
	ImGui::EndDisabled();

	if (Value)
	{


		{
			ImGui::SameLine();

			ImVec2 ButtionSize = { 20,20 };

			ImGui::BeginDisabled(!Item._AddNewValue.has_value());
			if (ImGui::Button("+", ButtionSize))
			{
				WasUpdated = true;
				Item._AddNewValue.value()(Object, Size);
			}
			ImGui::EndDisabled();

			ImGui::SameLine();

			ImGui::BeginDisabled(!Item._AddNewRemove.has_value());
			if (ImGui::Button("-", ButtionSize))
			{
				WasUpdated = true;
				Item._AddNewRemove.value()(Object, Size - 1);
			}
			ImGui::EndDisabled();
		}

		//ImGui::Separator();
		if (!WasUpdated) {

			for (size_t i = 0; i < Size; i++)
			{
				String Lable = "Item:" + std::to_string(i);
				ImGui::Text(Lable.c_str());


				bool RemoveItem = false;
				{
					void* ItemPtr = (Byte*)Buffer + (i * Item.ItemSize);
					ImGui::PushID(Lable.c_str());
					if (ImGui::BeginPopupContextItem("????"))
					{
						if (ImGui::MenuItem("Remove Item"))
						{
							RemoveItem = true;
						}
						ImGui::EndPopup();
					}
					ImGui::PopID();
				}
				ImGui::SameLine();


				Item._OnDrawItem(Object, i);





				if (RemoveItem)
				{
					WasUpdated = true;
					Item._AddNewRemove.value()(Object, i);
					break;
				}
			}
		}

		ImGui::TreePop();
	}



	return  WasUpdated;
}
UCodeIDEEnd