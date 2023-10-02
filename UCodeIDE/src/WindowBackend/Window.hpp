#pragma once

#include "imgui/backends/imgui_impl_glfw.h"
class Window
{
public:
	static void Init();
	Window(){}
	~Window() { CloseWindow(); }
	void InitWindow();

	void PreDraw();
	void Draw();

	void CloseWindow() 
	{
		if (_IsWindowOpen) {
			_IsWindowOpen = false;
			OnWindowClosed();
		}
	}
	bool  IsWindowOpen() const { return _IsWindowOpen; }
	
	
private:
	void OnWindowClosed();
	bool _IsWindowOpen = true;
	GLFWwindow* window = nullptr;
};

