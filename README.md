<div align="center" style="display:grid;place-items:center;">
<p>
    <a target="_blank"><img width="100" src="Logo.png" alt="Logo"></a>
</p>
<img src="https://img.shields.io/badge/License-MIT-blue.svg" alt="License: MIT">
    
[![CI](https://github.com/LostbBlizzard/UCodeLang/actions/workflows/main.yml/badge.svg?branch=master)](https://github.com/LostbBlizzard/UCodeLang/actions/workflows/main.yml)
<a href="[https:https://github.com/LostbBlizzard/UCodeLang/actions](https://github.com/LostbBlizzard/UCodeLang/actions)">

<h1>The UCode Programming Language</h1>


A high level programming language designed to be fast to write, to compile and to run.

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
- Reusable 
  - multi-paradigm
  - Compile time power


# Supported programming Languages

- [C++](https://github.com/LostbBlizzard/UCodeLang) //This Repository
- [C](https://github.com/LostbBlizzard/UCodeLang/releases/latest) //Just Copy UCodeLangCAPI.h and the steal a static library from the [releases](https://github.com/LostbBlizzard/UCodeLang/releases/latest)

# Features

Completely focused and specialized on Game Development and Game Engines.

## Langue Features
- No implicit conversions
- No Macros
- [Panic and value base errors](https://lostbblizzard.github.io/UCodeLang/Standardlibrary/ErrorHandleing/Result.html?highlight=result#resultstandardlibraryerrorhandleing)
- [Attributing tag system like C#](https://lostbblizzard.github.io/UCodeLang/SyntaxAndSeamantics/Types/tag.html)
- [Constructors,Destructor](https://lostbblizzard.github.io/UCodeLang/SyntaxAndSeamantics/Concepts/ConstructorsAndDestructors.html?highlight=construct#constructors-and-destructors)
- [Copy Constructors(missing docs)]()
- [Move semantics(missing docs)]()
- [Defer](https://lostbblizzard.github.io/UCodeLang/SyntaxAndSeamantics/Miscellaneous/defer.html?highlight=defer#deferkeyword)
- [Unsafe operations are behind like memory allocation are behind the unsafe keyword]()  
- [Rust like traits(working on)](https://lostbblizzard.github.io/UCodeLang/SyntaxAndSeamantics/Concepts/traits.html?highlight=trait#traits)
- [Go like concurrency(working on)]()
- [Easy to use binary(working on) and Json(working on) serialization in Standard Library]()
- [Operator overloading(missing docs)]()
- [Function overloading(missing docs)]()
- [Generics(missing docs)]()
- [Good compiled time analysis](https://lostbblizzard.github.io/UCodeLang/SyntaxAndSeamantics/Builtin/TypeInfo.html?highlight=typeinfo#typeinfo)
- [A Standard Library designed for game development](https://lostbblizzard.github.io/UCodeLang/Standardlibrary/Standardlibrary.html)
- [Module System](https://lostbblizzard.github.io/UCodeLang/ForImplementers/StandardLibAndModules.html)


And More.

## Library/Runtime Freatures
- Optional Runtime SandBoxing
- Flexible runtime
  - Code can be [jited](https://lostbblizzard.github.io/UCodeLang/ForImplementers/Reference/RunTime/JitInterpreter.html), [interpreted](https://lostbblizzard.github.io/UCodeLang/ForImplementers/Reference/RunTime/Interpreter.html) or [precompiled to native code](https://lostbblizzard.github.io/UCodeLang/ForImplementers/Reference/RunTime/NativeInterpreter.html) all working on the same runtime.
  - Runtime threading model can made for any threading Style (task based, event loop,etcetera)
- [A Wasm BackEnd](https://lostbblizzard.github.io/UCodeLang/ForImplementers/Reference/BackEnds/WasmBackEnd.html?highlight=wasmba#definition)
- [A C11 BackEnd](https://lostbblizzard.github.io/UCodeLang/ForImplementers/Reference/BackEnds/C11BackEnd.html?highlight=c11#definition)
- [Code be compiled to Native Executables for Linux(working on),Windows(working on) and Mac(working on)]()
- [Able to view all classes,funcions,trait after compileing(missing docs)]()
- [Able to strip output unneeded classes(missing docs)]()
- [Able to catch Panic if you like(missing docs and buggy)]()
- [Debuging(working on)]()
- [Module System]()


## Tooling Freatures
- [Automatic Binding Generation](https://lostbblizzard.github.io/UCodeLang/ForImplementers/Automaticbindinggeneration.html?highlight=aut#automatic-binding-generation)
- [Language server(working on)]()

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

After compiling we will get assembly data that can be used to see all classes ,funcions and Fields.
So it can be used in your inspector like most popular game engines.

the text below assume you compiled for a 64 CPU
The VM is CPU agnostic except for CPU pointer sizes.
it needs to be compiled for 64-bit or 32-bit which is trivial to do.

```
$Vec2:   //class size 8
 float x;//field offset 0
 float y;//field offset 4

$Weapon;//size 0

$Player: //class size 32
 Vec2 pos;//field offset 0
 Vector<Weapon> Weapons;//field offset [8-12 depending if the compiler added padding]

$Vector<Weapon>: //class size 24
 Weapon[&] _Data; //field offset 0
 uintptr _Size; //field offset 8
 uintptr _Capacity; //field offset 16

```


We want C# like assembly data but with C++ low level control over memory and without C# garbage collector. 

also since we're making a new Language we can learn past languages mistakes with using Optional,Result types and enum variant types and 
removing exceptions and more.


Despite being made for game engines UCode does not need a VM it can be used by itself and can be compiled directly to C,Wasm and Native platform librarys.

or directly to native machine code when that gets done.

# Why you should not use this Programming Language.

If your use case only needs a small language and will not be spending most of its time in the scripting language
you shold use [lua](https://github.com/lua/lua).

If you need static typing and C++ low-level ness but ok to have a Garbage Collector use [Mono C#](https://github.com/mono/mono) or [Terra](https://github.com/terralang/terra).



# Downloads & Documentation

[Website](https://lostbblizzard.github.io/UCodeLang/)

[PlayGround](https://lostbblizzard.github.io/UCodeLang/UCodeIDE/UCodeIDE.html)

[Documentation](https://lostbblizzard.github.io/UCodeLang/UCodeDocumentation/index.html)

[VSCode Extension](https://marketplace.visualstudio.com/items?itemName=Lostblizzard.ucode-language)

[Geting Started for Users](https://lostbblizzard.github.io/UCodeLang/UCodeDocumentation/GetingStartedUsers.html)

[How To Add To Your C++ Project](https://lostbblizzard.github.io/UCodeLang/UCodeDocumentation/ForImplementers.html)

## Download

Run The following in your terminal to install on linux/MacOs.
```
curl -L https://github.com/LostbBlizzard/UCodeLang/releases/latest/download/install.sh | sh
```

Or Download and run the [UCodeLangSetup.exe](https://github.com/LostbBlizzard/UCodeLang/releases/latest/download/UCodeLangSetup.exe) on the [latest release](https://github.com/LostbBlizzard/UCodeLang/releases/latest) for Windows.

# Example(User)


```
use ULang;//include standard Library.

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
# Example Entity component System

This Can be Fuound in UCodeIDE/src/AppObject/ECSExample.hpp
and UCodeIDE/src/AppObject/test.uc.

```
use API;//Imgui,Vec2,Time
use ECS;//Component,Entity

$Player[Component]:
 float Speed = 1;
 |Start[this&]:
  entity().position2d() = [2,5];



 |Update[this&] -> void:
  
  var& pos = entity().position2d();//get rereference to entity position.
  float deltatime = Time::DeltaTime();
  
  if Imgui::KeyDown(ImKey::W):
    pos.Y += Speed * deltatime;
    
  if Imgui::KeyDown(ImKey::S):
    pos.Y -= Speed * deltatime;
  
  if Imgui::KeyDown(ImKey::A):
    pos.X -= Speed * deltatime;
  
  if Imgui::KeyDown(ImKey::D):
    pos.X += Speed * deltatime;

```


# Example(Implementers)

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
    //your code some how failed to compile
  }
}
```

# Dependencies

UCodeLang has a Dependency on zycore and zydis for debugging reasons and X86 code generation.

both are planned to be removed.

# Project File Structure / github contributions

To build the project just download files or run.

```
git clone https://github.com/LostbBlizzard/UCodeLang.git
```

```
cd ./UCodeLang
```

```
use [premake](https://premake.github.io/) to generate Visual Studio solution,Makefiles+[CompileCommands](https://github.com/tarruda/premake-export-compile-commands),Xcode projects.
```


The file Project Structure is lad out like this

- /UC 
  - "Is the command line tool" 
  - "The thing users use to build ucode files and to run ucode."

- /UCApp 
  - "The Testing playground"
  - /src 
    - "use this when testing and debugging features."
    - "I dont care too much for what happends here."
  - /tests
    - "unit test and test helper functions."

- /UCodeAPI
  - "lots of ucode files"
  - "contains the standard library and other librarys."

- /UCodeLang
  - "the core of ucode-lang if your using this as a library add this to your include directory"
  - /Dependencies
    - "decompilers for testing"

  - /UCodeAnalyzer 
    - "classes/functions for the UCodeLanguageSever."
    - "Tools for analyzing UCodeLang"

  - /UCodeLang
     - /Compilation
       - "were ucode get compiled"
     - /LangCore
       - "helpers and other stuff".
     - /RunTime
       - "were your Interpreter jit-Interpreters and runtime live"
       - "the jit-Interpreter compilers is in UCodeLang/Compilation/Back"
    
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

- /doc
   - "Were UCodeDocumentation is"

- /UCodeIDE
   - "Its use is for testing things that would be hard to test using a debugger"
   - "it's called the UCodeIDE but it really just an internal testing tool"

- /website
   - "Were the UCodeWebsite is im not a web dev so it's probably just the bare minimum"
 


The project is meant too have as few dependencies as possible because it is meant to be easy to added to any c++ pojects.

use the github issue tab for bugs.
use the github discussions to talk about the language and new features and language design.
also keep it civil.


Also don't be shy to Criticize how things are implemented I know the code can be better in places thing.

Constructive criticism is welcome(dont be too harsh).

Please ask questions about anything Project since I barely documented anything.

the main branch for releases and the feature is where I do actual development.

The final vision of the Project has Backends for 

 - UCodeVM(Interpreter,JitInterpreter,NativeInterpreter)
 
 - C11

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

   A: C++ is too compilcaicated of a language and is unanalyzable after compiling(without hacking the compiler).rust memory features is not that useful for my use cases and c++ Smart Pointer's is all i need.

- Q: Why is it written in c++.

  A: Most Game Engines and specifically my game engine is written in c++.c++ is still the best tool for making game engines.

- Q:Why not use LLVM for your backend.

  A:LLVM is too big of a dependency and writing backends my self gives me a good excuse to learn about Computers(The Reason im making a game engine).
   Also you just output llvm assembly text.

- Q:Why significant white space?

  A:Just Seemed Interesting also just looks nicer. 


- Q:What is UCodeLang killer feature. 

  A:UCodeLang is not the next big thing it's just a tool made for my specific use case.It takes the best Parts from languages i like and attempts to remove excess.
    Its a refined and streamline c++ in some ways.

- Q:What does UCode mean? 
  
  A:The word UCode means nothing and has no history it just just a name I made up for a programming language idea.if you really want a meaning a meaning it when the idea was made it stand for Unsigned Code based on c/c++ unsigned keyword.


- Q:Do you like c++?

  A:I like it enough to spend a year making a new language using it but not enough to make my users for my Game Engine Use it(Sorry unreal).though i do really like c++ but also im aware of its flaws. 


- Q:what next and when will be Language be done.


- A:When the repository Opens (preferably on 1/2/2023 The one year anniversary of making the jit project) most features would be done and dont really plan on adding more unless it Just too Good. I'll be around for an extra year to finish up the other architectures and backends and stabitly.at that point the project will be done and i can go on working on that game engine and go to work on biger and better things.Also I believe adding too many features can make a language worse.

- Q:what your opinion on new programming languages.

- A:The more the merrier especially for systems languages.
    Every language experiments the more we experiment the better languages by we learning from there mistakes.

