#include "ImguiHelper.hpp"
#include "Imgui/misc/cpp/imgui_stdlib.h"
UCodeIDEStart


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