# Standard Library and Modules


After Automatic Binding Generation the last thing we need to is to add the Standard Library and allowing Input And Output.

Let's take the easier one out of the way.

## Allowing Input And Output.

Because CPPCalls and dynamic external funcions is the only for UCodeLang to talk to c++ and by extension the outside world.

Three Classes are made to add all of CPPCalls for you so you can get to work on makeing your framework.

- [IOLink](./Reference/RunTime/IOLink.md)

Adds CppCalls for direct access to the file system,network and other IO.

- [SandBoxedIOLink](./Reference/RunTime/SandBoxedIOLink.md)

Adds CppCalls for simulateing access in memory to the file system,network and other IO allowing the developer selectively allow access to these systems.

like pretending all files don't exist and except for selected few and those are read-only.


- [UtilityLink](./Reference/RunTime/UtilityLink.md)
Adds CppCalls for everything else that doesn't fit in IO mostly relating to the CPU and Memory.
  - Math
  - SIMD
  - Atomic

those classes can be used like so

```cpp
UCodeLang::RunTimeLib Lib;
Lib.Init(comilerRet.OutPut);//initialize RunTimeLib useing our Compiled code.
UtilityLink::Link(Lib);

SandBoxedIOLink::Link(Lib);
//IOLink::Link(Lib);
//You can only pick SandBoxedIOLink or IOLink because thay add the same funcions
```
## Module System
To add the StandardLibrary we need to use the Module System. 




Now you know the basics to get started makeing your framework.

Use the [API Reference](./APIReference.md) to see what other useful funcions and classes are in UCodeLang.