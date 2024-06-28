# cpptoulangvm
Example
```
uclang cpptoulangvm [Cppfilepath] [UCodeLangBindingPath] [CppVMBindingPath]
```

Generates bindings for UCodeLang using c++ source code.


exit code is 0 if it worked else 1.

#

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

The argument for the macros is the namespace inside of UCodeLang file.

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

Lets run our UCodeLang UCodeLang command line tool.

Open your command line in your ProjectRoot.
```
ProjectRoot >
```

then run uclang and type cpptoulangvm 

- Then with for c++ file path
- then output ucodelang file(.uc) path
- then c++ output were 'UCodeLangAutoLink' macros is found

```
ProjectRoot > ./uclang cpptoulangvm [CppPath] [UCodeLangOut] [CppVMLinkPath]
```

now the UCodeLangOut will have all funcion and type boilerplate and CppVMLinkPath will have all the Add_CPPCall funcions.

and if you have your API in multiple C++ files you can use [cppdirtoulangvm](./cppdirtoulangvm.md) to search inside directories.it even automatically includes the .h/.hpp files.