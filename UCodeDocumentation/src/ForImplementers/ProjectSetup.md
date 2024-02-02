# ForImplementers/CompilerAndVM

After Seting Up the Project lets Compile somehting.

add this to your main file.


```cpp
  #include <UCodeLang/UCodeLang.hpp>

  int main()
  {
    const char* MyUCode = "|main[] => 0;";

    UCodeLang::Compiler myCompiler;
    auto comilerRet = myCompiler.CompileText(MyUCode);


    if (_State == UCodeLang::Compiler::CompilerState::Success)
    {
      //your ucode was Compiled
    }
    else 
    {
      //your ucode some how failed   
    }
  }
  ```



The Code Above will compile your UCodeString.

Now lets try to run it.

let's first set up your runtime state this type can Load your UCode libraries(.ulib)

```cpp
UCodeLang::RunTimeLangState State;
```

and then initialize RunTimeLib useing our Compiled code.

```cpp
UCodeLang::RunTimeLib Lib;
Lib.Init(comilerRet.OutPut);//initialize RunTimeLib useing our Compiled code.
```

add RunTimeLib and
then Link all the Link libraries together.
```cpp
State.AddLib(&Lib);//Add RunTimeLib
State.LinkLibs();//Link libraries
```
then initialize the UCodeLang::Interpreter.

```cpp
UCodeLang::Interpreter interpreter;
interpreter.Init(&State);
```
then call StaticVariablesInitializeFunc to call initialize our Static Varables.

```cpp
interpreter.Call(StaticVariablesInitializeFunc);//initialize our Static Varables.
```

then call ThreadVariablesInitializeFunc to call initialize our Thread local Varables or this case Interpreter local Varables.

```cpp
interpreter.Call(ThreadVariablesInitializeFunc);//initialize thread local/Interpreter local Varables.
```

then call main with Interpreter.RMain<int>
```cpp
int Value = interpreter.RCall<int>("main");//Call main
```

after we call our Thread local Varables destructors.
and call our Static Varables destructors.
```cpp
interpreter.Call(ThreadVariablesUnLoadFunc);//Call  Thread local Varables destructors.
```
not doing so may cause memory leaks or leaving Hardware resources open like files network sockets ect.
```cpp
interpreter.Call(StaticVariablesUnLoadFunc);//Call Static Varables destructors.
```

Here the completed file.
```cpp
#include <UCodeLang/UCodeLang.hpp>

int main()
{
  const char* MyUCode = "|main[] => 1;";

  UCodeLang::Compiler myCompiler;
  auto comilerRet = myCompiler.CompileText(MyUCode);


  if (comilerRet._State == UCodeLang::Compiler::CompilerState::Success)
  { 
    //your ucode was Compiled
    UCodeLang::RunTimeLangState State;

    UCodeLang::RunTimeLib Lib;
	Lib.Init(comilerRet.OutPut);//initialize RunTimeLib useing our Compiled code.


    State.AddLib(&Lib);//Add RunTimeLib
    State.LinkLibs();//Link libraries
   
    UCodeLang::Interpreter interpreter;
    interpreter.Init(&State);

    interpreter.Call(StaticVariablesInitializeFunc);//initialize our Static Varables.
	interpreter.Call(ThreadVariablesInitializeFunc);//initialize thread local/Interpreter local Varables.

    int Value = interpreter.RCall<int>("main");//Call main
    std::cout << " Got Value " << Value;


	interpreter.Call(ThreadVariablesUnLoadFunc);//Call  Thread local Varables destructors.
    interpreter.Call(StaticVariablesUnLoadFunc);//Call Static Varables destructors.
  }
  else 
  {
    //your code some how failed  
    ULangTest::LogErrors(std::cout, myCompiler); 
  }
}
```

But how do we call C++ from UCodeLang.

- [Next Part](./CallingCppfromUCodeLang.md)
