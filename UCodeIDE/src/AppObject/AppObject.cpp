#include "AppObject.hpp"
#include "imgui/imgui.h"
void AppObject::Init()
{
	if (!_IsAppRuning) {
		_IsAppRuning = true;

	}
}

void AppObject::OnDraw()
{
	ImGui::ShowDemoWindow();
}

void AppObject::OnAppEnd()
{
}
