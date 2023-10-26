#include <UCodeLang/LangCore/LangDef.hpp>
#include "App.hpp"



#if defined(PUBLISHED) && UCodeLang_Platform_Windows
#include <windows.h>
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
	LPWSTR *argv;
	int argc;
	argv = CommandLineToArgvW(GetCommandLine(), &argc);
	return App::main(argc, (char**)&argv);
}
#else
int main(int argc, char* argv[])
{
	//auto v = _Entry();
	return App::main(argc, argv);
}


#endif 