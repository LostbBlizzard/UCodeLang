# Syntax

Here a basic run down of UCodeLang Syntax for People who already know how a programming language.

UCode uses [Sugnificant Whitespace](./Concepts/SignificantWhitespace.md) like python.

The syntax for declaring a [function](./Concepts/function.md) in UCode is as follows:

```cpp
|function_name[parameter_list];//not being explicit with the return
|function_name[parameter_list] -> void;
```


For example, the following code declares a function named add that takes two integer parameters and returns an integer:

```cpp
|add[int x, int y] -> int;
```


Variables are declared with a type and a name. For example, the following code declares an integer variable named x:
```cpp
int x;
```


Member functions are just functions
with a [this](./Keywords/this.md) parameter
```cpp
use ULang;

$MyClass:
  |SayHello[this&]:
    Fmt::Print("Hello, World!");

|main[]:
  var object = MyClass();
  object.SayHello();
```


To access a [static](./Keywords/static.md) variable or function in UCode, you can use the class name and the scope resolution operator (::).


```
//no use of the ULang namespace;
|main[]:
  ULang::Fmt::Print("Hello");
```


functions can be expressed in different ways.


```cpp
|add[int x, int y] => x + y;
    
|add2[int x, int y] -> int:
  ret x + y;

|add3[int x, int y]:
  ret x + y;
```

[enums](./Keywords/Enum.md) are is as follows:
```cpp
$Color enum:
  Red,
  Green,
  Blue,

```

[namespaces](./Concepts/namespaces.md) are is as follows:
``` 
MyNamespaces: 
  static int MyStaticValue = 10;
  thread int MythreadLocalValue =10; 
  int Value = 10;//also my threadLocalValue 
```

[using namespaces](./Keywords/use.md)  is as follows:
```
use MyNamespaces;
```

[alias](./Concepts/alias.md) is as follows:

```cpp
$i64 = int64;
$u32 = uint32;
```

[inheritance](./Concepts/inheritance.md) look like this
```
$Worker trait;//no members

$FasterWorker[Worker];//no members
```

[Object protections](./Concepts/inheritance.md) look like this
```cpp
$Worker;

$FasterWorker[Worker]:
  public:
    int Hp = 10;
  private:
    int Money = 10;
    int Happiness = 10;
```

 [Standard Library](../Standardlibrary.md) Compiler alias 

 - [Optional](../Standardlibrary/Types/Generic/optional.md)
```cpp
int? MaybeAnInt;//an optional int
```

 - [Vector](../Standardlibrary/Types/Generic/Vector.md)
```cpp
int[] Ints;//an Vector<int>

```
 - [Array](../Standardlibrary/Types/Generic/Array.md)

```cpp
int[5] MoreInts;//an Array<int,5> //an Array<int,(somevarable)>
int[(somevarable)] MoreInts; //an Array<int,(somevarable)>  Map short hand takes precedent over array in this case so you need parentheses. 
```
- [Map](../Standardlibrary/Types/Generic/Map.md)
```cpp
int[String] StringToIntMap;//an Map<String,int>
int[sometype] SomeTypeToIntMap;//an Map<sometype,int>
```
 - [Span](../Standardlibrary/Types/Generic/Span.md)

```cpp
int[:] SpanOfInts;//an Span<int>
```
  
  - [unique_ptr](../Standardlibrary/Types/Generic/Unique_ptr.md)

```cpp
int^ UniqueInt;//an Unique_ptr<int>
```

- [Shared_ptr](../Standardlibrary/Types/Generic/Shared_ptr.md)

```cpp
int$ SharedInt;//an Shared_ptr<int>
```

- [unique_Array](../Standardlibrary/Types/Generic/Unique_ptr.md)

```cpp
int[^] UniqueIntsArray;//an Unique_Array<int>
```

- [Shared_Array](../Standardlibrary/Types/Generic/Shared_ptr.md)

```cpp
int[$] SharedIntsArray;//an Shared_Array<int>
```
[immutability](./Concepts/umut.md)
```cpp
imut int ConstInt = 5;
imut ConstInt2 = 4;// or like this.    
$MyClass:
  |SayHello[umut this&]:
    Fmt::Print("Hello, World!");
```

 [references](./Concepts/Address.md)

```cpp
|main[]:  
  int x = 10;
  int& r = x;

|Update_NumToChange[int& V]:
  V = 10;
```


Thats the basics heres. 
Some links Explore More:
 - [Standardlibrary](../Standardlibrary.md)
 - [Modules](./Modules.md)
 - [MemoryManagement](./MemoryManagement.md)
 - [Keywords](./Keywords.md)
 - [traits](./Concepts/traits.md)
 - [generics](./Concepts/generic.md)
