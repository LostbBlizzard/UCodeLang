

<div align="center" style="display:grid;place-items:center;">
<p>
    <a target="_blank"><img width="100" src="Logo.png" alt="Logo"></a>
</p>
<img src="https://img.shields.io/badge/License-MIT-blue.svg" alt="License: MIT">
    
[![CI](https://github.com/LostbBlizzard/UCodeLang/actions/workflows/main.yml/badge.svg?branch=master)](https://github.com/LostbBlizzard/UCodeLang/actions/workflows/main.yml)
<a href="[https:https://github.com/LostbBlizzard/UCodeLang/actions](https://github.com/LostbBlizzard/UCodeLang/actions)">

<h1>The UCode Programming Language</h1>


A high level progaming language designed to be fast to write, to compile and to run.

<div align=left>

UCode is a programming language designed to be embedded into existing software such as game engines while being a low-level language with modern and high-level features.

mainly

- does not require a garbage collector like C#.
- offers more functionality than Lua.
- is easier to use than C++.
- Uses Compile time power to reduce boilerplate.

# Design Principles/Pillars

- Simple is better than complex.
- Zero-overhead abstractions
  - You don't pay for what you don't use.
  - What you do use is just as efficient as what you could reasonably write by hand.
-  Productive
   - Scalable compilation times
   - Reliable Tooling.
   - gradual learning curve
- Reuseable 
  - multi-paradigm
  - Compile time power


# Why use this Programming Language.

This Language was made because the lack of suitable scripting languages
for a GameEngine's that allowed you for low-level control over memory and is easy to analyze
for reflection.

for example 


```
$Vec2:
 float x;
 float y;

$Weapon;

$Player:
 Vec2 pos;
 Weapon[] Weapons;

```

After compiling will become  

```
$Vec2:   //size 8
 float x;//offset 0
 float y;//offset 4

$Weapon;//size 0

$Player: //32
 Vec2 pos;//offset 0
 Vector<Weapon> Weapons;//offset 8

$Vector<Weapon>: //size 24
 Weapon[&] _Data; //offset 0
 uintptr _Size; //offset 8
 uintptr _Capacity; //offset 16

```


We want C# like assembly data but with C++ low level control over memory and without C# garbage collector. 

also since we're makeing a new Language we can learn past languages mistakes with Optional and Result types and no exceptions enum variant types and more.


Despite being made for game engines UCode does not need a VM it can be used by itself and can be compiled directly to C(the only thing that really works the intermediate representation is practically just C).

or directly to native machine code when that gets done.

>[!IMPORTANT]
>UCodeLang Is Not Done!
UCodeLang is not done and  unstable and lacks documentation.
and documentation that does exist may be out of date.
It's nowhere near completion. 

# Supported Progaming Languages

- [C++](https://github.com/LostbBlizzard/UCodeLang) //This Repository

# Downloads & Documentation

[WebSite and PlayGround]()

[Download]()

[Documentation]("UCodeDocumentation/src/SUMMARY.md")
//Goto "UCodeDocumentation/src/SUMMARY.md" if link does not work

[VSCode Extension]()
//not done

[How To Add To Your C++ Project]("UCodeDocumentation/src/ForImplementers.md")
//Goto "UCodeDocumentation/src/ForImplementers.md" if link does not work

# Example(User)


```
using ULang;//include standard Library.

|main[] -> int:
 int a = 10;
 int b = 20;

 var c = a + b;
 
 int[] List;

 List.Push(c);
 
 int&? LastItem = List.Back();

 match LastItem:
  Option(out myvalue):
  
    if myvalue == 30:
       Fmt::Print("myvalue does equal 30");
     
  Null: panic("List size was 0");

```

# Example(Implementers)

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
    //your code some how failed to compile
    ULangTest::LogErrors(std::cout, myCompiler); 
  }
}
```

# Dependencys

UCodeLang has a Dependency on zycore and zydis for debuging reasons and X86 code generation.

both are planned to be removed.

# Project File Structure / github contributions

To build the project just download files or run.

<code>

    git clone https://github.com/LostbBlizzard/UCodeLang.git

</code>

<code>

    cd ./UCodeLang

</code>

After geting the Project Files Run.

- The VS_2019.bat file(If you use VS 2019) to build vs project files and open the project in visual studio.

- The VS_2022.bat file(If you use VS 2022) to build vs project files and open the project in visual studio.

- The GNU_Make.bat file(If you use GCC on windows) and run "make" to build it.

- The GNU_Make.sh file(If you use GCC on linux or Mac) and run "make" to build it. or run

<code>

    ./GNU_Make.sh

</code>


<code>

    make

</code>

- The XCode.sh file(If you use XCode) and and open the project in XCode.




The file Project Structure is lad out like this

- /Dependencies
  - "Contains the premake executable"

- /UC 
  - "Is the command line tool" 
  - "The thing users use to build ucode files and to run ucode."

- /UCApp 
  - "The Testing playground"
  - /src 
    - "use this when testing and debuging features."
    - "I dont care too much for what happends here."
  - /tests
    - "unit test and test helper funcions."

- /UCodeAPI
  - "lots of ucode files"
  - "contains the standard library and other librarys."

- /UCodeLang
  - "the core of ucode-lang if your useing this as a library add this to your include directory"
  - /Dependencies
    - "decompilers for testing"

  - /UCodeAnalyzer 
    - "classes/funcions for the UCodeLanguageSever."
    - "Tools for analyzing UCodeLang"

  - /UCodeLang
     - /Compliation
       - "were ucode get compiled"
     - /LangCore
       - "helpers and other stuff".
     - /RunTime
       - "were your Interpreter jit-Interpreters and runtime live"
       - "the jit-Interpreter compilers is in UCodeLang/Compliation/Back"
    
- /UCodeLanguageSever
  - "the runable language sever"
  - /src
    - "the code"

- /UCodeLanguageSeverLib
  - "the language sever with json and Language Server Protocol"
  - /json-develop
    - "json library"
  - /src
    - "the code"

- /UCodeDocumentation
   - "Were UCodeDocumentation is"
   - /Standardlibrary
     - "Generated from /UCodeAPI/UCodeDocumentation"

- /UCodeIDE
   - "Its use is for testing things that would be hard to test useing a debuger"
   - "it's called the UCodeIDE but it realy just an internal testing tool"

- /UCodeWebsite
   - "Were the UCodeWebsite is im not a web dev so it's probably just the bare minimum"
 


The project is meant too have as few dependencies as possable because it is meant to be easy to added to any c++ pojects.

use the github issue tab for bugs.
use the github discussions to talk about the language and new features and language design.
also keep it civil.


Also don't be shy to Criticize how things are implemented I know the code can be beter in places thing.

Constructive criticism is welcome(dont be too harsh).

Please ask questions about anything Project since I barely documented anything.

the main branche for releases and the feature is where I do actual development.

The final vision of the Project has Backends for 

 - UCodeVM(Interpreter,JitInterpreter,NativeInterpreter)
 
 - C89

 - WebAssembly

 - Windows(native executable),

 - Linux(native executable),

 - MacOs(native executable),

 - X86

 - X86_64

 - Arm

 - Arm64



# Credits
The was Language was designed  and developed by LostBlizzard On [GitHub](https://github.com/LostbBlizzard).

Who currently maintains the 

- The [UCodeLang](https://github.com/LostbBlizzard/UCodeLang) cplusplus implementation of the language.

The UCodeGameEngine(being worked on) the primary reason The Language was made.

# QA

-  Q: UCodeLang is very similar to c++ and rust why not use Rust or C++?

   A: C++ is too compilcaicated of a language and is unanalyzable after compiling(without hacking the compiler).rust memory features is not that usefull for my use cases and c++ Smart Pointer's is all i need.

- Q: Why is it written in c++.

  A: Most Game Engines and specificaly my game engine is written in c++.c++ is stil the best tool for makeing game engines.

- Q:Why not use LLVM for your backend.

  A:LLVM is too big of a dependency and writeing backends my self gives me a good excuse to learn about Computers(The Reason im makeing a game engine).
   Also you just output llvm assembly text.

- Q:Why significant white space?

  A:Just Seemed Interesting also just looks nicer. 


- Q:What is UCodeLang killer feature. 

  A:UCodeLang is not the next big thing it's just a tool made for my specific use case.It takes the best Parts from languages i like and attempts to remove excess.
    Its a refined and streamline c++ in some ways.

- Q:What does UCode mean? 
  
  A:The word UCode means nothing and has no history it just just a name I made up for a progaming language idea.if you realy want a meaning a meaning it when the idea was made it stand for Unsigned Code based on c/c++ unsigned keyword.


- Q:Do you like c++?

  A:I like it enough to spend a year makeing a new language useing it but not enough to make my users for my Game Engine Use it(Sorry unreal).though i do realy like c++ but also im aware of its flaws. 


- Q:what next and when will be Language be done.


- A:When the repository Opens (preferably on 1/2/2023 The one year anniversary of makeing the jit project) most features would be done and dont really plan on adding more unless it Just too Good. I'll be around for an extra year to finsh up the other architectures and backends and stabitly.at that point the project will be done and i can go on working on that game engine and go to work on biger and better things.Also I belive adding too many features can make a language worse.

- Q:what your opinion on new progaming languages.

- A:The more the merrier especially for systems languages.
    Every language experiments the more we experiment the beter languages by we learning from there mistakes.

