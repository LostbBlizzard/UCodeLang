# Automatic Binding Generation

To Solve this problem [uclang](../UCodeCL/UCodeCl.md) or the UCodeLang command line tool is able to generate these bindings.

First step is geting the uclang command line tool.
Theres two ways of geting by [installing it](../GetingStartedUsers.md) or building from source.

For this example we will just steal it from the [github releases](https://github.com/LostbBlizzard/UCodeLang/releases/latest) and put it in our project root directory.


after that lets mark our c++ files with the funcions and types to export.


Heres our types and funcions we will like to export in our c++ file.
```cpp
enum class Colors
{
    Red,
    Green,
    Blue,
}

void SomeFuncionthatTakeinColor(Colors a)
{
    //
}
```
Next add these c++ macros .The argument for the macros is the namespace inside of UCodeLang file.

```cpp
namespace OurAPI
{

UCodeLangExportSymbol("YourFrameWorkNameSpace") enum class Colors
{
    Red,
    Green,
    Blue,
}


UCodeLangExportSymbol("YourFrameWorkNameSpace") void SomeFuncionthatTakeinColor(Colors a)
{
    //
}

}
```
These will tell uclang command line tool to generate bindings for these funcions and types.

next we need to add 'UCodeLangAutoLink' macros.
when we add our Add_CPPCall.
```cpp
UCodeLang::RunTimeLib Lib;
Lib.Init(comilerRet.OutPut);//initialize RunTimeLib useing our Compiled code.

//Adding Our PrintNumberConsole Funcion.
lib.Add_CPPCall("PrintNumberConsole", []( UCodeLang::InterpreterCPPinterface& Input)
{
	int Par0 = Input.GetParameter<int>();
    UAPI_PrintNumberConsole(Par0);
    Input.Set_Return();
},UAPI_PrintNumberConsole);

//Our UCodeLangAutoLink
UCodeLangAutoLink(Lib, OurAPI);
```

now lets run our UCodeLang UCodeLang command line tool.

Open your command line in your ProjectRoot.
```
ProjectRoot >
```

then run uclang (or uclang.exe if your on windows) and type cpptoulangvm 

- Then with for c++ file path
- then output ucodelang file(.uc) path
- then c++ output were 'UCodeLangAutoLink' macros is found

```
ProjectRoot > ./uclang cpptoulangvm [CppPath] [UCodeLangOut] [CppVMLinkPath]
```

now the UCodeLangOut will have all funcion and type boilerplate and CppVMLinkPath will have all the Add_CPPCall funcions.

and if you have your API in multiple C++ files you can use [cppdirtoulangvm](../UCodeCL/Commands/cppdirtoulangvm.md) to search inside directories.it even automatically includes the .h/.hpp files.
```
ProjectRoot > ./uclang cppdirtoulangvm [CppDir] [UCodeLangOut] [CppVMLinkPath]
```

[The Next Part](./StandardLibAndModules.md) is about using Module System to Add the Standad Library and other Modules(like your framwork API) and also allowing for intellisense for your code editor.