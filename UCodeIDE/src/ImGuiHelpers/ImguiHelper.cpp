#include "ImguiHelper.hpp"
#include "imgui/misc/cpp/imgui_stdlib.h"
#include "imgui_internal.h"
UCodeIDEStart


UCodeLang::AnyInterpreterPtr ImguiHelper::_Ptr;


//From https://github.com/ocornut/imgui/issues/3469
void ImguiHelper::ItemLabel(StringView title, ItemLabelFlag flags)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	const ImVec2 lineStart = ImGui::GetCursorScreenPos();
	const ImGuiStyle& style = ImGui::GetStyle();
	float fullWidth = ImGui::GetContentRegionAvail().x;
	float itemWidth = ImGui::CalcItemWidth() + style.ItemSpacing.x;
	ImVec2 textSize = ImGui::CalcTextSize(title.data(), title.data() + title.size());
	ImRect textRect;
	textRect.Min = ImGui::GetCursorScreenPos();
	if (flags & ItemLabelFlag::Right)
		textRect.Min.x = textRect.Min.x + itemWidth;
	textRect.Max = textRect.Min;
	textRect.Max.x += fullWidth - itemWidth;
	textRect.Max.y += textSize.y;

	ImGui::SetCursorScreenPos(textRect.Min);

	ImGui::AlignTextToFramePadding();
	// Adjust text rect manually because we render it directly into a drawlist instead of using public functions.
	textRect.Min.y += window->DC.CurrLineTextBaseOffset;
	textRect.Max.y += window->DC.CurrLineTextBaseOffset;

	ImGui::ItemSize(textRect);
	if (ImGui::ItemAdd(textRect, window->GetID(title.data(), title.data() + title.size())))
	{
		ImGui::RenderTextEllipsis(ImGui::GetWindowDrawList(), textRect.Min, textRect.Max, textRect.Max.x,
			textRect.Max.x, title.data(), title.data() + title.size(), &textSize);

		if (textRect.GetWidth() < textSize.x && ImGui::IsItemHovered())
			ImGui::SetTooltip("%.*s", (int)title.size(), title.data());
	}
	if (flags & ItemLabelFlag::Left)
	{
		auto v = ImVec2{ 0, textSize.y + window->DC.CurrLineTextBaseOffset };
		auto n = textRect.Max;
		n.x -= v.x;
		n.x -= v.x;
		ImGui::SetCursorScreenPos(n);
		ImGui::SameLine();
	}
	else if (flags & ItemLabelFlag::Right)
		ImGui::SetCursorScreenPos(lineStart);
};
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
	bool Is32Bit = sizeof(void*) == 4;

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
	case UCodeLang::ReflectionTypes::uIntPtr:
	{
		return InputSize_t(FieldName, (size_t*)Object);
	}
	break;
	case UCodeLang::ReflectionTypes::sIntPtr:
	{
		return InputSize_t(FieldName, (intptr_t*)Object);
	}
	break;
	case UCodeLang::ReflectionTypes::CustomType:
	{
		auto Syb = assembly.Find_Node(type);

		{
			auto vec2info = assembly.IsVec2_t(type);
			if (vec2info.has_value())
			{
				auto& vec2infoVal = vec2info.value();
				if (assembly.IsJust(type))
				{
					switch (vec2infoVal.XAndYType._Type)
					{
					case UCodeLang::ReflectionTypes::sInt8:return Vec2IntField(FieldName, (Int8*)Object);
					case UCodeLang::ReflectionTypes::sInt16:return Vec2IntField(FieldName, (Int16*)Object);
					case UCodeLang::ReflectionTypes::sInt32:return Vec2IntField(FieldName, (Int32*)Object);
					case UCodeLang::ReflectionTypes::sInt64:return Vec2IntField(FieldName, (Int64*)Object);

					case UCodeLang::ReflectionTypes::uInt8:return Vec2IntField(FieldName, (UInt8*)Object);
					case UCodeLang::ReflectionTypes::uInt16:return Vec2IntField(FieldName, (UInt16*)Object);
					case UCodeLang::ReflectionTypes::uInt32:return Vec2IntField(FieldName, (UInt32*)Object);
					case UCodeLang::ReflectionTypes::uInt64:return Vec2IntField(FieldName, (UInt64*)Object);

					case UCodeLang::ReflectionTypes::float32:return Vec2float32Field(FieldName, (float32*)Object);
					case UCodeLang::ReflectionTypes::float64:return Vec2float64Field(FieldName, (float64*)Object);

					case UCodeLang::ReflectionTypes::uIntPtr:
						return Is32Bit ? Vec2IntField(FieldName, (UInt32*)Object) : Vec2IntField(FieldName, (UInt64*)Object);
						break;
					case UCodeLang::ReflectionTypes::sIntPtr:
						return Is32Bit ? Vec2IntField(FieldName, (Int32*)Object) : Vec2IntField(FieldName, (Int64*)Object);
						break;
					default:
						break;
					}
				}
			}
		}
		
		{
			auto vec3info = assembly.IsVec3_t(type);
			if (vec3info.has_value())
			{
				auto& vec3infoVal = vec3info.value();
				if (assembly.IsJust(type))
				{
					switch (vec3infoVal.XAndYType._Type)
					{
					case UCodeLang::ReflectionTypes::sInt8:return Vec3IntField(FieldName, (Int8*)Object);
					case UCodeLang::ReflectionTypes::sInt16:return Vec3IntField(FieldName, (Int16*)Object);
					case UCodeLang::ReflectionTypes::sInt32:return Vec3IntField(FieldName, (Int32*)Object);
					case UCodeLang::ReflectionTypes::sInt64:return Vec3IntField(FieldName, (Int64*)Object);

					case UCodeLang::ReflectionTypes::uInt8:return Vec3IntField(FieldName, (UInt8*)Object);
					case UCodeLang::ReflectionTypes::uInt16:return Vec3IntField(FieldName, (UInt16*)Object);
					case UCodeLang::ReflectionTypes::uInt32:return Vec3IntField(FieldName, (UInt32*)Object);
					case UCodeLang::ReflectionTypes::uInt64:return Vec3IntField(FieldName, (UInt64*)Object);

					case UCodeLang::ReflectionTypes::float32:return Vec3float32Field(FieldName, (float32*)Object);
					case UCodeLang::ReflectionTypes::float64:return Vec3float64Field(FieldName, (float64*)Object);

					case UCodeLang::ReflectionTypes::uIntPtr:
						return Is32Bit ? Vec3IntField(FieldName, (UInt32*)Object) : Vec3IntField(FieldName, (UInt64*)Object);
						break;
					case UCodeLang::ReflectionTypes::sIntPtr:
						return Is32Bit ? Vec3IntField(FieldName, (Int32*)Object) : Vec3IntField(FieldName, (Int64*)Object);
						break;
					default:
						break;
					}
				}
			}
		}

		{
			auto vectorinfo = assembly.IsVector_t(type);
			if (vectorinfo.has_value())
			{
				auto& vectorInfoVal = vectorinfo.value();

				UCodeLang::ReflectionVector vector;
				vector.Set(Object, &vectorInfoVal, _Ptr,assembly,Is32Bit);

				size_t V = vector.size();
				return DrawVector(FieldName, vector, assembly);
			}
		}

		if (Syb)
		{
			if (Syb->Get_Type() == UCodeLang::ClassType::Enum)
			{
				auto& EnumInfo = Syb->Get_EnumData();
				ImguiHelper::ItemLabel(StringView(FieldName), Left);

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
			else if (Syb->Get_Type() == UCodeLang::ClassType::StaticArray)
			{
				auto& ClassInfo = Syb->Get_StaticArray();
				size_t itemsize = assembly.GetSize(ClassInfo.BaseType,Is32Bit).value();

				for (size_t i = 0; i < ClassInfo.Count; i++)
				{
					void* objptr =(void*)( (uintptr_t)Object + (itemsize * i));
					String str = "Item " + std::to_string(i);
					UCodeObjectField(str.c_str(),objptr, ClassInfo.BaseType, assembly);
				}
			}
		}
		return false;
	}
		break;
	case UCodeLang::ReflectionTypes::Void:
			break;
	default:
		UCodeLangUnreachable();
		break;
	}
	return false;
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
	ImguiHelper::ItemLabel(StringView(FieldName), Left);

	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_U32, (void*)&Value);
	ImGui::PopID();

	return V;
}

bool ImguiHelper::uInt16Field(const char* FieldName, UInt16& Value)
{
	ImguiHelper::ItemLabel(StringView(FieldName), Left);

	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_U16, (void*)&Value);
	ImGui::PopID();

	return V;
}

bool ImguiHelper::uInt8Field(const char* FieldName, UInt8& Value)
{
	ImguiHelper::ItemLabel(StringView(FieldName), Left);

	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_U8, (void*)&Value);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::uIntptrField(const char* FieldName, uintptr_t& Value)
{	
	ImguiHelper::ItemLabel(StringView(FieldName), Left);

	ImGui::PushID(&Value);
	auto V = InputSize_t("",&Value);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::IntptrField(const char* FieldName, intptr_t& Value)
{
	ImguiHelper::ItemLabel(StringView(FieldName), Left);

	ImGui::PushID(&Value);
	auto V = InputSize_t("", &Value);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::InputText(const char* label, String& buffer, ImGuiInputTextFlags flags)
{
	ImguiHelper::ItemLabel(StringView(label), Left);

	ImGui::PushID(&buffer);
	auto V = ImGui::InputText("", &buffer, flags);
	ImGui::PopID();

	return V;
}

struct InputTextReflectionCallback_UserData
{
	BytesPtr NewPtr;
	UCodeLang::ReflectionString* Str;
};
int TextReflectionCallBack(ImGuiInputTextCallbackData* data)
{
	InputTextReflectionCallback_UserData* user_data = (InputTextReflectionCallback_UserData*)data->UserData;
	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
	{
		// Resize string callback
		// If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
		UCodeLang::ReflectionString* str = user_data->Str;
		IM_ASSERT(data->Buf == str->data());

		auto v = str->AsCharView();
		str->resize(data->BufTextLen);

		
		user_data->NewPtr.Resize(v.size()+1);
		memcpy(user_data->NewPtr.Data(),v.data(),v.size());
		user_data->NewPtr[v.size()] = '\n';//for intent

		data->Buf = (char*)user_data->NewPtr.Data();
	}
	return 0;
}
bool ImguiHelper::InputText(const char* label, UCodeLang::ReflectionString& buffer, ImGuiInputTextFlags flags)
{
	ImguiHelper::ItemLabel(StringView(label), Left);

	ImGui::PushID(&buffer);


	flags |= ImGuiInputTextFlags_CallbackResize;

	InputTextReflectionCallback_UserData user_data;
	user_data.NewPtr.Resize(buffer.size() + 1);
	memcpy(user_data.NewPtr.Data(),buffer.data(), buffer.size());
	user_data.NewPtr[buffer.size()] = '\n';//for intent

	user_data.Str = &buffer;

	auto V = ImGui::InputText("", (char*)user_data.NewPtr.Data(),buffer.size(), flags, TextReflectionCallBack,&user_data);
	
	ImGui::PopID();

	return V;
}
bool ImguiHelper::MultLineText(const char* label, String& buffer, ImVec2 Size, ImGuiInputTextFlags flags)
{	
	return  ImGui::InputTextMultiline(label, &buffer, Size, flags);
}
bool ImguiHelper::Vec2float32Field(const char* label, float vec2[2])
{
	ImguiHelper::ItemLabel(StringView(label), Left);

	ImGui::PushID(&vec2);
	auto V = ImGui::DragScalarN("", ImGuiDataType_Float, vec2, 2);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::Vec2float64Field(const char* label, double vec2[2])
{
	ImguiHelper::ItemLabel(StringView(label), Left);

	ImGui::PushID(&vec2);
	auto V = ImGui::DragScalarN("", ImGuiDataType_Double, vec2, 2);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::Vec2IntField(const char* label, UInt64 vec2[2])
{
	ImguiHelper::ItemLabel(StringView(label), Left);

	ImGui::PushID(&vec2);
	auto V = ImGui::DragScalarN("", ImGuiDataType_U64, vec2, 2);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::Vec2IntField(const char* label, Int64 vec2[2])
{
	ImguiHelper::ItemLabel(StringView(label), Left);

	ImGui::PushID(&vec2);
	auto V = ImGui::DragScalarN("", ImGuiDataType_S64, vec2, 2);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::Vec2IntField(const char* label, UInt32 vec2[2])
{
	ImguiHelper::ItemLabel(StringView(label), Left);

	ImGui::PushID(&vec2);
	auto V = ImGui::DragScalarN("", ImGuiDataType_U32, vec2, 2);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::Vec2IntField(const char* label, Int32 vec2[2])
{
	ImguiHelper::ItemLabel(StringView(label), Left);

	ImGui::PushID(&vec2);
	auto V = ImGui::DragScalarN("", ImGuiDataType_S32, vec2, 2);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::Vec2IntField(const char* label, UInt16 vec2[2])
{
	ImguiHelper::ItemLabel(StringView(label), Left);

	ImGui::PushID(&vec2);
	auto V = ImGui::DragScalarN("", ImGuiDataType_U16, vec2, 2);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::Vec2IntField(const char* label, Int16 vec2[2])
{
	ImguiHelper::ItemLabel(StringView(label), Left);

	ImGui::PushID(&vec2);
	auto V = ImGui::DragScalarN("", ImGuiDataType_S16, vec2, 2);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::Vec2IntField(const char* label, UInt8 vec2[2])
{	
	ImguiHelper::ItemLabel(StringView(label), Left);

	ImGui::PushID(&vec2);
	auto V = ImGui::DragScalarN("", ImGuiDataType_U8, vec2, 2);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::Vec2IntField(const char* label, Int8 vec2[2])
{
	ImguiHelper::ItemLabel(StringView(label), Left);

	ImGui::PushID(&vec2);
	auto V = ImGui::DragScalarN("", ImGuiDataType_S8, vec2, 2);
	ImGui::PopID();

	return V;
}

bool ImguiHelper::Vec3float32Field(const char* label, float vec2[3])
{
	ImguiHelper::ItemLabel(StringView(label), Left);

	ImGui::PushID(&vec2);
	auto V = ImGui::DragScalarN("", ImGuiDataType_Float, vec2, 3);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::Vec3float64Field(const char* label, double vec2[3])
{
	ImguiHelper::ItemLabel(StringView(label), Left);

	ImGui::PushID(&vec2);
	auto V = ImGui::DragScalarN("", ImGuiDataType_Double, vec2, 3);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::Vec3IntField(const char* label, UInt64 vec2[3])
{
	ImguiHelper::ItemLabel(StringView(label), Left);

	ImGui::PushID(&vec2);
	auto V = ImGui::DragScalarN("", ImGuiDataType_U64, vec2, 3);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::Vec3IntField(const char* label, Int64 vec2[3])
{
	ImguiHelper::ItemLabel(StringView(label), Left);

	ImGui::PushID(&vec2);
	auto V = ImGui::DragScalarN("", ImGuiDataType_S64, vec2, 3);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::Vec3IntField(const char* label, UInt32 vec2[3])
{
	ImguiHelper::ItemLabel(StringView(label), Left);

	ImGui::PushID(&vec2);
	auto V = ImGui::DragScalarN("", ImGuiDataType_U32, vec2, 3);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::Vec3IntField(const char* label, Int32 vec2[3])
{
	ImguiHelper::ItemLabel(StringView(label), Left);

	ImGui::PushID(&vec2);
	auto V = ImGui::DragScalarN("", ImGuiDataType_S32, vec2, 3);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::Vec3IntField(const char* label, UInt16 vec2[3])
{
	ImguiHelper::ItemLabel(StringView(label), Left);

	ImGui::PushID(&vec2);
	auto V = ImGui::DragScalarN("", ImGuiDataType_U16, vec2, 3);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::Vec3IntField(const char* label, Int16 vec2[3])
{
	ImguiHelper::ItemLabel(StringView(label), Left);

	ImGui::PushID(&vec2);
	auto V = ImGui::DragScalarN("", ImGuiDataType_S16, vec2, 3);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::Vec3IntField(const char* label, UInt8 vec2[3])
{
	ImguiHelper::ItemLabel(StringView(label), Left);

	ImGui::PushID(&vec2);
	auto V = ImGui::DragScalarN("", ImGuiDataType_U8, vec2, 3);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::Vec3IntField(const char* label, Int8 vec2[3])
{
	ImguiHelper::ItemLabel(StringView(label), Left);

	ImGui::PushID(&vec2);
	auto V = ImGui::DragScalarN("", ImGuiDataType_S8, vec2, 3);
	ImGui::PopID();

	return V;
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
	ImguiHelper::ItemLabel(StringView(FieldName), Left);

	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_S64, (void*)&Value);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::Int32Field(const char* FieldName, Int32& Value)
{
	ImguiHelper::ItemLabel(StringView(FieldName), Left);

	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_S32, (void*)&Value);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::Int16Field(const char* FieldName, Int16& Value)
{
	ImguiHelper::ItemLabel(StringView(FieldName), Left);

	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_S16, (void*)&Value);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::Int8Field(const char* FieldName, Int8& Value)
{
	ImguiHelper::ItemLabel(StringView(FieldName), Left);

	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_S8, (void*)&Value);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::float32Field(const char* FieldName, float32& Value)
{
	ImguiHelper::ItemLabel(StringView(FieldName), Left);

	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_Float, (void*)&Value);
	ImGui::PopID();

	return V;
}
bool ImguiHelper::float64Field(const char* FieldName, float64& Value)
{
	ImguiHelper::ItemLabel(StringView(FieldName), Left);

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
	ItemLabel(StringView(label),Left);
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
bool ImguiHelper::InputSize_t(const char* label, size_t* v, ImGuiInputTextFlags flags)
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
bool ImguiHelper::InputSize_t(const char* label, intptr_t* v,ImGuiInputTextFlags flags)
{
	if constexpr (sizeof(size_t) == sizeof(UInt64))
	{
		return ImGui::InputScalar(label, ImGuiDataType_::ImGuiDataType_S64, v, nullptr, nullptr, nullptr, flags);
	}
	else
	{
		return ImGui::InputScalar(label, ImGuiDataType_::ImGuiDataType_S32, v, nullptr, nullptr, nullptr, flags);
	}
}
bool ImguiHelper::DrawVector(const char* label, UCodeLang::ReflectionVector& vector, const UCodeLang::ClassAssembly& assembly)
{
	DrawVectorInfo Info;
	Info.ItemSize = vector.GetElementTypeSize();

	Info._OnDrawItem = [vector, &assembly](void* Object, size_t Index) mutable
	{
		UCodeLang::ReflectionVector& Objectbuf = *(UCodeLang::ReflectionVector*)Object;
		auto Item = Objectbuf[Index];

		UCodeObjectField(Item, vector.GetElementType(), assembly);
	};

	Info._AddNewValue = [&assembly](void* Object, size_t Index)
	{
		bool Is32Bit = sizeof(void*) == 4;

		UCodeLang::ReflectionVector& Objectbuf = *(UCodeLang::ReflectionVector*)Object;

		auto objsize = assembly.GetSize(Objectbuf.GetElementType(), Is32Bit);

		if (objsize.has_value()) 
		{
			BytesPtr newobj;
			newobj.Resize(objsize.value());

			void* object = newobj.Data();

			auto itworked = assembly.CallDefaultConstructor(Objectbuf.GetElementType(), object, Is32Bit);
			if (itworked.has_value())
			{
				auto& obj = itworked.value();
				if (obj.has_value())
				{
					for (auto& Item : obj.value())
					{
						_Ptr.ThisCall(Item.MethodToCall, Item.ThisPtr);
					}

				}
				Objectbuf.insert(Index, std::move(object));
			}
		}
		
	};

	Info._AddNewRemove = [](void* Object, size_t Index)
	{
		UCodeLang::ReflectionVector& Objectbuf = *(UCodeLang::ReflectionVector*)Object;
		Objectbuf.remove(Index);
	};

	Info._ResizeVector = [](void* Object, size_t NewIndex)
	{
		UCodeLang::ReflectionVector& Objectbuf = *(UCodeLang::ReflectionVector*)Object;
		Objectbuf.resize(NewIndex);
	};

	return DrawVector(label,&vector, vector.data(), vector.size(), Info);
}
bool ImguiHelper::DrawVector(const char* label, void* Object, void* Buffer, size_t Size, const DrawVectorInfo& Item)
{
	bool WasUpdated = false;
	bool Value = ImGui::TreeNode(label);

	size_t NewSize = Size;

	ImGui::SameLine();

	ImVec2 ButtionSize = { 20,20 };
	ImGui::BeginDisabled(!Item._ResizeVector.has_value());
	{
		ImGui::PushItemWidth(ImGui::CalcItemWidth() - (ButtionSize.x * 2) - 30);
		ImGui::PushID(&Item);
		bool ResizeWasUpdated = InputSize_t("", &NewSize);
		ImGui::PopID();
		ImGui::PopItemWidth();
		if (ResizeWasUpdated && Item._ResizeVector.has_value())
		{
			WasUpdated = true;
			Item._ResizeVector.value()(Object, NewSize);
		}

	}
	ImGui::EndDisabled();
	{
		ImGui::SameLine();

		ImVec2 ButtionSize = { 20,20 };

		ImGui::BeginDisabled(!Item._AddNewValue.has_value());
		if (ImGui::Button("+", ButtionSize))
		{
			WasUpdated = true;
			Item._AddNewValue.value()(Object, Size);
			Size += 1;
		}
		ImGui::EndDisabled();

		ImGui::SameLine();

		ImGui::BeginDisabled(!Item._AddNewRemove.has_value() || Size == 0);
		if (ImGui::Button("-", ButtionSize))
		{
			WasUpdated = true;
			Item._AddNewRemove.value()(Object, Size - 1);
			Size -= 1;
		}
		ImGui::EndDisabled();
	}
	if (Value)
	{

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


		ImGui::TreePop();
	}



	return  WasUpdated;
}
UCodeIDEEnd