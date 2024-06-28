# Conditional compilation

conditional compilation In UCode is allows a way of including or omitting selected lines of source code depending on constant expressions.

unlike languages like C UCode uses constant expressions to determine what to include and what to remove useing special keywords.

the special keywords are:
- $if
- $else
- $for
- Obj.$Varable/Obj.$Func()



The [eval keyword](../Keywords/eval.md) is used to make constant expressions.

# $if and $else
the special variant of the if keyword allows forces the check comparison do be an constant expression and
does not generate symbols in the if block until evaluated.also the newly added symbols are in the same scope as the if.

for example:

```

    eval bool Mode = true; //constant expression varable 
  
    |Main[]:
     $if Mode:
      int A = 1;
     $else if true://constant else if
      int A = 2;
     $else:
      int A = 3;

     Console::Log(A);
   
```

after evaluating the code gets simplified into.

```

    |Main[]:
     int A = 1;
     Console::Log(A);

```

also works in file scope and namespace scope.


```

    eval bool Mode = true; //constant expression varable

    $if Mode:
     $CoolInt = uint32;
    $else
     $CoolInt = uint64;

```

after evaluating the code gets simplified into.

```
 
    $CoolInt = uint64;

```

# $for

the special variant of the for keyword will unwap the list to their own scopes and is mostly used with [parameter packs](./parameter%20packs.md) but can work with any object with constant expression for loop overload implementation.

note the list must also be a constant expression.

```

    |Func<X:...>[ParameterPack X]:
     $for [Item : X]:
      var ItemT = typeof(Item);
      bind(ItemT) Buffer = Item;
 
    |main[]:
     Func<int,uint32,uint64>(0,1,2);

```

after evaluating the code gets simplified into.

```
 
    |Func<int,uint32,int64>[int X1,uint32 X2,int64 X3]:

     |
      var ItemT = typeof(X1);
      bind(ItemT) Buffer = X1;
     |

     |
      var ItemT = typeof(X2);
      bind(ItemT) Buffer = X2;
     |

     |
      var ItemT = typeof(X3);
      bind(ItemT) Buffer = X3;
     |

```

# Obj.$Varable/Obj.$Func()

The special variant of the object member access allows to use Varable instead of a constant literal.

for example:

```

    eval char[\] MemberName = "X";
    $Vec2:
     int X;
     int Y;
  
    |Main[]:
     Vec2 Vec = Vec2();
     Vec.$MemberName = 1;
   
```


after evaluating the code gets simplified into.

```

    $Vec2:
     int X;
     int Y;
  
    |Main[]:
     Vec.X = 1;
   
```

this alows for simpler serialization and Deserialization and gives more tools for reduceing boilerplate.

-Note the special object member access must be a static char Array constant expression.

-Also using this allows for accessing operator overloading Funcions pointers or Funcions with keywords in them like new or drop.

for example:

```

    
    $Vec2:
     int X;
     int Y;
     |+[this&,this& Other] -> Vec2;

    eval char[\] MemberName = "+";
    $ItemFuncPointer = [Vec2&,Vec2&] -> Vec2;

    |Main[]:
     Vec2 Vec = Vec2();
     Vec2 OtherVec = Vec2();
     ItemFuncPointer Pointer = Vec2::$MemberName;

     //Calls Vec2::+[Vec2&,Vec2& Other] -> Vec2;
     Vec2 Value = Pointer(Vec,OtherVec);
     
   
```