

// Ref API
//https://learn.microsoft.com/en-us/windows/win32/winprog/windows-data-types

%NWin32:
 //$APIENTRY = WINAPI;
 $ATOM = !WORD;

 $BYTE = char;

 $BOOL = !int;
 $BOOLEAN = !BYTE;
 $CCHAR = !char;
 $CHAR = !char;
 
 $TCHAR = !char;
 $LPTSTR = !TCHAR[&];

 $WORD = !uint64;

 $HANDLE = !uint64;
 $HINSTANCE = !HANDLE;
 $HMODULE = !HANDLE;

 $HFILE = !int;
 $UINT = int;
 $LPCSTR = umut char[&];
