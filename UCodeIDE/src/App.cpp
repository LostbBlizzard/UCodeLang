#include "App.hpp"
#include "AppObject/AppObject.hpp"
#include "WindowBackend/Window.hpp"

#include <iostream>

int App::main(int argc, char* argv[])
{
    AppObject app = AppObject();
    Window window = Window();
    Window::Init();

    window.InitWindow();
    app.Init();

    while (window.IsWindowOpen())
    {
        window.PreDraw();
        app.OnDraw();
        window.Draw();

        if (!app.IsAppRuning()) { window.CloseWindow(); }
    }
	return EXIT_SUCCESS;
}
