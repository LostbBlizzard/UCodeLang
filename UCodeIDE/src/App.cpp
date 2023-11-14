#include "App.hpp"
#include "AppObject/AppObject.hpp"
#include "WindowBackend/Window.hpp"

#include <iostream>

#if __EMSCRIPTEN__
#include "emscripten.h"
#endif

UCodeAnalyzer::UCodeIDE::AppObject* appptr =nullptr;
Window* windowptr =nullptr;
void loop()
{
        windowptr->PreDraw();
        appptr->OnDraw();
        windowptr->Draw();
}

int App::main(int argc, char* argv[])
{
    UCodeAnalyzer::UCodeIDE::AppObject app = UCodeAnalyzer::UCodeIDE::AppObject();
    Window window = Window();
    windowptr =&window;
    appptr = &app;

    Window::Init();

    window.InitWindow();
    app.Init();

    #if __EMSCRIPTEN__
    emscripten_set_main_loop(loop, -1, 1);
    #else
    while (window.IsWindowOpen())
    {
       loop();

       if (!app.IsAppRuning()) { window.CloseWindow(); }
    }
    #endif

	return EXIT_SUCCESS;
}
