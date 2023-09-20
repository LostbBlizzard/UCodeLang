#include <UCodeLang/LangCore/LangDef.hpp>
#include "App.hpp"

#ifdef PUBLISHED
#ifdef  UCodeLang_Platform_Windows
#include <Windows.h>
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
	LPWSTR* argv;
	int argc;
	argv = CommandLineToArgvW(GetCommandLine(), &argc);
	return App::main(argc, (char**)&argv);
}
#endif 
#else
int main(int argc, char* argv[])
{
	//auto v = _Entry();
	return App::main(argc, argv);
}


#endif 