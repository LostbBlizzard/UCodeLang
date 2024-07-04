

#include "App.hpp"

#if (PUBLISHED || RELASE) && UCodeLang_Platform_Windows
#include <Windows.h>
#include <iostream>
void get_command_line_args(int* argc, char*** argv)
{
	// Get the command line arguments as wchar_t strings
	wchar_t** wargv = CommandLineToArgvW(GetCommandLineW(), argc);
	if (!wargv) { *argc = 0; *argv = NULL; return; }

	// Count the number of bytes necessary to store the UTF-8 versions of those strings
	int n = 0;
	for (int i = 0; i < *argc; i++)
		n += WideCharToMultiByte(CP_UTF8, 0, wargv[i], -1, NULL, 0, NULL, NULL) + 1;

	// Allocate the argv[] array + all the UTF-8 strings
	*argv = (char**)malloc((*argc + 1) * sizeof(char*) + n);
	if (!*argv) { *argc = 0; return; }

	// Convert all wargv[] --> argv[]
	char* arg = (char*)&((*argv)[*argc + 1]);
	for (int i = 0; i < *argc; i++)
	{
		(*argv)[i] = arg;
		arg += WideCharToMultiByte(CP_UTF8, 0, wargv[i], -1, arg, n, NULL, NULL) + 1;
	}
	(*argv)[*argc] = NULL;
}


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
	int     argc;
	char** argv;
	get_command_line_args(&argc, &argv);



	if (argc <= 1)
	{
		if (AllocConsole())
		{
			freopen("CONOUT$", "wt", stdout);
			freopen("CONIN$", "rt", stdin);
			SetConsoleTitle(L"UCodeLang");
			std::ios::sync_with_stdio(1);
		}
	}

	auto exit = App::main(argc, argv);


	free(argv);
	return exit;
}
#else
int main(int argc, char* argv[])
{
	return App::main(argc, argv);
}
#endif 
