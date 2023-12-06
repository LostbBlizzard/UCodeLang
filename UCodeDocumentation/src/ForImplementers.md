# ForImplementers

This section is about for People who are Implementing the UCodeLang in there C++ Projects such as GameEngines or Developer frameworks.


## Requirements

- C++17 Compiler.
- Basic knowledge of linking and Include C++ Projects

## Download

Simply Git Clone The project or download it inside your external librarys or dependency folder.

```
git clone https://github.com/LostbBlizzard/UCodeLang.git
```


## Building Project

We will be using premake here but this will work with any build system.

To Build the Project we need to set up the local Project includes Directory. 

- The UCodeLang/UCodeLang is is intentional.It Just how the project is Structured.

- \[YourDependencyFolder\] is the Directory you git clone to.
 - \[GitDownload\] is the Directory git clone produced containing UCodeLang Repository.

```lua
includedirs
{
    "[YourDependencyFolder]/[GitDownload]/UCodeLang",
    "[YourDependencyFolder]/[GitDownload]/UCodeLang/UCodeLang",
    "[YourDependencyFolder]/[GitDownload]/UCodeLang/Dependencies/zydis/include",
    "[YourDependencyFolder]/[GitDownload]/UCodeLang/Dependencies/zycore/include",
    "[YourDependencyFolder]/[GitDownload]/UCodeLang/Dependencies/zydis/src",
}
```

Next we pick what files are part of the Library.

UCodeLang mainly uses .hpp and .cpp but with some exceptions so we also have to include them.

```
files 
{
    "[YourDependencyFolder]/[GitDownload]/UCodeLang/UCodeLang/**.hpp",
    "[YourDependencyFolder]/[GitDownload]/UCodeLang/UCodeLang/**.cpp",
    "[YourDependencyFolder]/[GitDownload]/UCodeLang/UCodeLang/**.h", 
    "[YourDependencyFolder]/[GitDownload]/UCodeLang/UCodeLang/**.c",
}
```

We will also need to include zydis and zycore because UCodeLang needs them.


```lua
files { 
"[YourDependencyFolder]/[GitDownload]/UCodeLang/Dependencies/zydis/src/**.c",
"[YourDependencyFolder]/[GitDownload]/UCodeLang/Dependencies/zycore/src/**.c",

"[YourDependencyFolder]/[GitDownload]/UCodeLang/Dependencies/zydis/src/**.inc",
"[YourDependencyFolder]/[GitDownload]/UCodeLang/Dependencies/zycore/src/**.inc",

"[YourDependencyFolder]/[GitDownload]/UCodeLang/Dependencies/zydis/include/**.h",
"[YourDependencyFolder]/[GitDownload]/UCodeLang/Dependencies/zycore/include/**.h",
}
```

Try building this as a Static library.
It may take 2-8 minutes to fully build. 

90% of the Project is the Compiler.


If your haveing any problems try asking for help on the [github discussions tab](https://github.com/LostbBlizzard/UCodeLang/discussions).


# Using it

Now the hard part is over its now time to use this library.

Now in your Main project now add these two include Directorys and make sure you link your static library.

```lua
includedirs
{
"[YourDependencyFolder]/[GitDownload]/UCodeLang",
"[YourDependencyFolder]/[GitDownload]/UCodeLang/UCodeLang"
}
```
If every thing is done correctly.
your should be able to use these segment of c++ in your project.

```cpp
#include <UCodeLang/UCodeLang.hpp>

int main()
{
  const char* MyUCode = "|main[] => 1;";

  UCodeLang::Compiler myCompiler;
  auto comilerRet = myCompiler.CompileText(MyUCode);


  if (!comilerRet.IsError())
  { 
    //your ucode was Compiled
    UCodeLang::RunTimeLangState State;

    UCodeLang::RunTimeLib Lib;
    Lib.Init(comilerRet.GetValue().OutPut.value());//initialize RunTimeLib using our Compiled code.


    State.AddLib(&Lib);//Add RunTimeLib
    State.LinkLibs();//Link libraries
   
    UCodeLang::Interpreter interpreter;
    interpreter.Init(&State);

    interpreter.Call(StaticVariablesInitializeFunc);//initialize our Static Variables.
    interpreter.Call(ThreadVariablesInitializeFunc);//initialize thread local/Interpreter local Variables.

    int Value = interpreter.RCall<int>("main");//Call main
    std::cout << " Got Value " << Value;


    interpreter.Call(ThreadVariablesUnLoadFunc);//Call Thread local Variables destructors.
    interpreter.Call(StaticVariablesUnLoadFunc);//Call Static Variables destructors.
  }
  else 
  {
    //your code failed to compile
  }
}
```

Here the [C++ reference](./ForImplementers/APIReference.md) use this find out what usefull full classes,funcion and types are in UCodeLang.

Also see the [Language Reference](./SyntaxAndSemantics.md) if you alread know C++ you can learn the Whole  Language in a day.