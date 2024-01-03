# Calling C++ from UCodeLang

Its Time to Make UCodeLang do something useful.
By default UCodeLang has no Input or Output other than doing stuff with memory.

So UCodeLang has no way Opening Files,Networking or most importantly calling your framework.

To Fix this We have CPPCalls and dynamic external funcions.

Lets Say we have a UCodeLang funcion that we want to make Print a Number to the Console.

```cpp
|PrintNumberConsole[int number] -> void;
```

First we will need to tell the compiler that the funcion will have its implementation be added at runtime by using the dynamic and extern Keywords. 

```cpp
extern dynamic |PrintNumberConsole[int number] -> void;
```

Next In our C++ file. Lets make a funcion that does that.
```cpp
void UAPI_PrintNumberConsole(int num)
{
    std::cout << std::tostring(num) << std::endl;
}
```
Then lets add that funcion to the RunTimeLib using Add_CPPCall.
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
```

Lets me explains whats going in Add_CPPCall.

First give the funcion the a  name "PrintNumberConsole".

Next we make a lambda(funcion without a name) that allow us to talk to the Interpreter that called this funcion. then we ask for for our first function of parameter of type int.

Then we call UAPI_PrintNumberConsole with our parameter.

Then we ask the Interpreter to set Set_Return to nothing.

Lastly we pass a funcion pointer of UAPI_PrintNumberConsole.So the [JitInterpreter](./Reference/RunTime/JitInterpreter.md) and the [NativeInterpreter](./Reference/RunTime/NativeInterpreter.md) will be to call the funcion.

now all you have to do is to call PrintNumberConsole in your UCodeLang Source Code.

```cpp
extern dynamic |PrintNumberConsole[int number] -> void;
const char* MyUCode = "|main[] => PrintNumberConsole(10 + 5);";
```

When you call main
you should should see that 15 was printed to the console.

but this would be tedious to do for every funcion and it doesn't account for.

- Funcion overloading.
- Namespaces.  
- Structs. 
- Enums. 
- Constructors,Destructor copy Constructors.
- Changeing APIs
- Type marshaling
- Just having Hundreds of funcions to expose.

It Would be a architectural and dependency nightmare.

If only theres a way to generate these bindings and types using our c++ source code and all we have to do just mark the funcions and types we would like expose.

We could call it cpptoulangvm.

## [Next Part](./Automaticbindinggeneration.md)
