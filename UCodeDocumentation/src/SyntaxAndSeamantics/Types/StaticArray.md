# StaticArray(Internal Type)

 Not to be confused with The [Array Type](/src/Standardlibrary/Types/Generic/Array.md).

 Is a Buffer is Array have their size or length determined at compile time.

 The syntax for Static Array is below:
 the type int be replaced as any valid type.

```

    int[/10] Buffer;
    ^ the base type Type

    int[/10] Buffer;
       ^ left bracket and forward slash
         
    int[/10] Buffer;
         ^constant expression

    int[/10] Buffer;
           ^right bracket

```

The Static Array is able to be indexed to address the base type
or to an [Address Array](AddressArray.md) if [persuaded](../Concepts/TypePersuasion.md).

```

    int[/4] Buffer = [0,0,0,0];
    int& V = Buffer[3];//last item.

    int[&] = Buffer[3];//last Item but as addres Array.

    Buffer[3] = 5;//change last Item.
```

Note the Static Array does not do any bounds checking.

if you need bounds checking and other other features use [Array Type](/src/Standardlibrary/Types/Generic/Array.md).

# when the size is  not specified  and Var(StaticArray)

when the size is not specified for example.

```
   
    int[/] = [5,5];
```

The compiler will use the count the Anonymous object construct fields.

```
   
    int[/] = [5,5];//two field so type is int[/2]
```

if the type is [var](Var.md)  for example.

```
    
    var Hello = [2,2];
```

the compiler will take to take the first Anonymous object construct field as it's base type. and will continue the rules above(when size is not specified).

# StringBuffer(StaticArray)

[String literals](../Concepts/Stringliteral.md) also work with StaticArray just like in the programming language C thay must be inferred to do so.As the [umut](../Concepts/umut.md) [char](char.md)[[&]](../Types.md) is default Behavior.



```

    char[/] = "Hello World";

```

but unlike in C the null char(0x00)
is not appended at the end of the string.

```

    char[/] = "Hello World";
    char[/11] = "Hello World";

    char[/12] = "Hello World";
    //error "Hello World" has 11 Values but char[/12] wants 12.

```

if the null char must be appened you can do something like this.

```

     char[/] = "Hello World\0";

```