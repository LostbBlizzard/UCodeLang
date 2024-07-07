# NameSpaces

A allows scope to the Class,types,funcions in a scope to organize code.

A NameSpace can be use like so.


```

    MyNameSpace:
     $VeryColorClass:
      int X;
      int Y;

```

The Class can be accessed by useing '::'.

```

    MyNameSpace:
     $VeryColorClass:
      int X;
      int Y;

    |main[]:
     MyNameSpace::VeryColorClass Myclass = [];

```

NameSpace's can be also be Scoped like so.

```

    MyNameSpace::EvenCoolorNameSpace:
     $VeryColorClass:
      int X;
      int Y;

```

NameSpaces can only be used at the File or NameSpace Scope.