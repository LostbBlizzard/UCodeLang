# Syntax

UCode uses [Sugnificant Whitespace](./Concepts/SignificantWhitespace.md) like python.

The syntax for declaring a [function](./Concepts/function.md) in UCode is as follows:

<code>

    |function_name[parameter_list];//not being explicit with the return

    |function_name[parameter_list] -> void;


</code>


For example, the following code declares a function named add that takes two integer parameters and returns an integer:


<code>

    |add[int x, int y] -> int;


</code>


Variables are declared with a type and a name. For example, the following code declares an integer variable named x:
<code>

    int x;

</code>


Member functions are just functions
with a [this](./Keywords/this.md) parameter
<code>

    use UCode;

    $MyClass:
     |SayHello[this&]:
      Console::Log("Hello, World!");

    |main[]:
     var object = MyClass();
     object.SayHello();

</code>


To access a [static](./Keywords/static.md) variable or function in UCode, you can use the class name and the scope resolution operator (::).


<code>

    //no use of the UCode namespace;

    |main[]:
     UCode::Console::SayHello();

</code>


functions can be expressed in different ways.


<code>


    |add[int x, int y] => x + y;
    
    |add2[int x, int y] -> int:
     ret x + y;

    |add3[int x, int y]:
     ret x + y;

</code>

[enums](./Keywords/Enum.md) are is as follows:
<code>
 
    $Color enum:
     Red,
     Green,
     Blue,

</code>

[namespaces](./Concepts/namespaces.md) are is as follows:
<code>
 
    MyNamespaces:
    
     static int MyStaticValue = 10;
     thread int MythreadLocalValue =10; 
     int Value = 10;//also my threadLocalValue 

</code>

[using namespaces](./Keywords/use.md)  is as follows:
<code>

    use MyNamespaces;

</code>

[alias](./Concepts/alias.md) is as follows:
<code>

    $i64 = int64;
    $u32 = int32;

</code>

[inheritance](./Concepts/inheritance.md) look like this
<code>

    $Worker;//no members

    $FasterWorker[Worker];//no members

</code>

[Object protections](./Concepts/inheritance.md) look like this
<code>

    $Worker;

    $FasterWorker[Worker]:
     public:
      int Hp = 10;
     private:
      int Money = 10;
      int Happiness = 10;

</code>

 [Standard Library](../Standardlibrary.md) Compiler alias 

 - [Optional](../Standardlibrary/Types/Generic/optional.md)
  <code>
 
       int? MaybeAnInt;//an optional int

  </code>

 - [Vector](../Standardlibrary/Types/Generic/Vector.md)
  <code>
 
       int[] Ints;//an Vector<int>

  </code>

 - [Array](../Standardlibrary/Types/Generic/Array.md.md)
  <code>
 
       int[5] MoreInts;//an Array<int,5>

  </code>

 - [Span](../Standardlibrary/Types/Generic/Span.md)

   <code>
 
       int[:] SpanOfInts;//an Span<int>

   </code>
  
  - [unique_ptr](../Standardlibrary/Types/Generic/Unique_ptr.md)

    <code>
 
        int^ UniqueInt;//an Unique_ptr<int>

    </code>

  - [Shared_ptr](../Standardlibrary/Types/Generic/Shared_ptr.md)

    <code>
 
        int$ SharedInt;//an Shared_ptr<int>

    </code>

  - [unique_Array](../Standardlibrary/Types/Generic/Unique_ptr.md)

    <code>
 
        int[^] UniqueIntsArray;//an Unique_Array<int>

    </code>

  - [Shared_Array](../Standardlibrary/Types/Generic/Shared_ptr.md)

    <code>
 
        int[$] SharedIntsArray;//an Shared_Array<int>

    </code>


 [immutability](./Concepts/umut.md)
  <code>

    umut int ConstInt = 5;
    umut ConstInt2 = 4;// or like this.
    
    $MyClass:
     |SayHello[umut this&]:
      Console::Log("Hello, World!");

  </code>

 [references](./Concepts/Address.md)
  <code>

    int x = 10;
    int& r = x;

    |Update_NumToChange[int& V]:
      V = 10;


  </code>


Thats the basics heres. 
Some links Explore More:
 - [Standardlibrary](../Standardlibrary.md)
 - [Modules](./Modules.md)
 - [MemoryManagement](./MemoryManagement.md)
 - [Keywords](./Keywords.md)
 - [traits](./Concepts/traits.md)
 - [generics](./Concepts/generic.md)