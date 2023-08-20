# ForImplementers/CompilerAndVM

After Seting Up the Project lets Compile somehting.

add this to your main file.


<details>
  <summary>Cpp</summary>

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

</details>

<details>

  <summary>C89</summary>
    
  ```c
  #include <UCodeLang/UCodeLangCAPI.h>

  int main()
  {//TODO upate is file when 
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

</details>

<details>

  <summary>rust</summary>
    
  ```rust
  use UCodeLang;

  i32 main()
  {
    let MyUCode = "|main[] => 0;";

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

</details>

<details>

  <summary>zig</summary>
    
  ```rust
  #include <UCodeLang/UCodeLang.hpp>

  i32 main()
  {
    const MyUCode = "|main[] => 0;";

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

</details>


The Code Above will compile your UCodeString.

now lets try to run it.

let's first set up your state this type can Upload your UCode libraries(.ulib)

and then initialize RunTimeLib useing our Compiled code.

add RunTimeLib and
then Link all the Link libraries together.

then initialize the UCodeLang::Interpreter.

then call StaticVariablesInitializeFunc to call initialize our Static Varables.


then call ThreadVariablesInitializeFunc to call initialize our Thread local Varables or this case Interpreter local Varables.

then call main with Interpreter.RMain<int>

after we call our Thread local Varables destructors.
and call our Static Varables destructors.

not doing so may cause memory leaks or leaving Hardware resources open like files network sockets ect.

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
    //your ucode some how failed  
    ULangTest::LogErrors(std::cout, myCompiler); 
  }
}
```

See not too hard.
but what if we want to add our c++ functionality so UCode can Call our C++ Code.

it's also pretty easy all we have to do add our C++ Funcion before we link our code and add our ULang signature funcion before we compile.

```cpp
#include <UCodeLang/UCodeLang.hpp>

//Our Native Funcion
int MyCppFunc(int A)
{
    return A + 5;
}

//Our Interpreter Funcion Wapper.
void UCodeLangAPI ULang_MyCppFunc(InterpreterCPPinterface& Input)
{
	int A = Input.GetParameter<int>();
	Input.Set_Return(MyCppFunc(A));
}

int main()
{

  UCodeLang::String MyUCode = "|main[] => MyCppFunc(5);\n";
  MyUCode += "extern dynamic |MyCppFunc[int A] -> int;\n";//our ULang signature that maps to Cpp Code Funcion.
  UCodeLang::Compiler myCompiler;
  auto comilerRet = myCompiler.CompileText(MyUCode);


  if (comilerRet._State == UCodeLang::Compiler::CompilerState::Success)
  { 
    //your ucode was Compiled
    UCodeLang::RunTimeLangState State;

    UCodeLang::RunTimeLib Lib;
	Lib.Init(comilerRet.OutPut);//initialize RunTimeLib useing our Compiled code.

    UCodeLang::RunTimeLib CppLib;
    Lib.Add_CPPCall("MyCppFunc", ULang_MyCppFunc, MyCppFunc);//Add Our Interpreter Funcion Wapper.
    //Also Add the Native CppCall this is for JiT compilation. this is optional but makes the CPPCall a bit slower if not added.


    State.AddLib(&CppLib);//Add our Cpp funcions.
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
    //your ucode some how failed  
    ULangTest::LogErrors(std::cout, myCompiler); 
  }
}
```

and that's the basics now your ready to explore the rest of this Project Library.

Note this tutorial is just small example you should look at the [Project Library](./Project.md) on you should properly set it up and the UCodeLang Type life.

Try looking these:

 - [Proper Setup]

 - Debuging(Steping,inspecting Varables).

 - Backends

 - Jit-interpreter and Native-interpreter.
