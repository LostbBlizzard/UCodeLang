#include "AppObject.hpp"
#include "imgui/imgui.h"
#include "ImGuiHelpers/TextEditor/TextEditor.h"
#include "LanguageSever.hpp"

UCodeIDEStart
TextEditor _Editor;
UCodeLanguageSever::LanguageSever _Sever;

void UCodeIDEStyle(ImGuiStyle* dst)
{
	ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	style->FrameBorderSize = 1.0f;
	style->FrameRounding = 0.0f;
}

void AppObject::Init()
{
	if (!_IsAppRuning) {
		_IsAppRuning = true;


		UCodeIDEStyle(nullptr);
	}
}
void BeginDockSpace(bool* p_open)
{
    // If you strip some features of, this demo is pretty much equivalent to calling DockSpaceOverViewport()!
    // In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
    // In this specific demo, we are not using DockSpaceOverViewport() because:
    // - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
    // - we allow the host window to have padding (when opt_padding == true)
    // - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
    // TL;DR; this demo is more complicated than what you would normally use.
    // If we removed all the options we are showcasing, this demo would become:
    //     void ShowExampleAppDockSpace()
    //     {
    //         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    //     }

    bool opt_fullscreen = true;
    bool opt_padding =false;
    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_::ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    //ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("IDE", p_open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("IDE");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
}
void EndDockSpace()
{
    ImGui::End();//EndDockSpace
}

void AppObject::OnDraw()
{
    bool Doc = true;
    BeginDockSpace(&Doc);

    if (ImGui::Begin("ShowStyleEditor")) 
    {
        ImGui::ShowStyleEditor();
        ImGui::End();
    }


    if (ImGui::Begin("Files"))
    {
        ImGui::End();
    }
    if (ImGui::Begin("File.uc"))
    {
        TextEditor::LanguageDefinition Def;
        Def.mName = "UCodeLang";
        Def.mTokenize = [](const char* in_begin, const char* in_end, const char*& out_begin, const char*& out_end, TextEditor::PaletteIndex& paletteIndex)
        {


            return false;
        };


        _Editor.SetShowWhitespaces(false);
        _Editor.SetLanguageDefinition(Def);
        _Editor.Render("File.uc");
        ImGui::End();
    }

    if (ImGui::Begin("Error List"))
    {
        ImGui::End();
    }
    if (ImGui::Begin("Output"))
    {
        ImGui::End();
    }

	if (ImGui::BeginMainMenuBar())
	{

		if (ImGui::BeginMenu("File"))
		{


			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{


			ImGui::EndMenu();
		}


		if (ImGui::BeginMenu("Git"))
		{


			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Build"))
		{


			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Settings"))
		{


			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{


			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

    EndDockSpace();
}

void AppObject::OnAppEnd()
{
}
UCodeIDEEnd