# NameSpaces

A allows scope to the Class,types,funcions in a scope to organize code.

A NameSpace can be use like so.


<code>

    MyNameSpace:
     $VeryColorClass:
      int X;
      int Y;

</code>

The Class can be accessed by useing '::'.

<code>

    MyNameSpace:
     $VeryColorClass:
      int X;
      int Y;

    |main[]:
     MyNameSpace::VeryColorClass Myclass = [];

</code>

NameSpace's can be also be Scoped like so.

<code>

    MyNameSpace::EvenCoolorNameSpace:
     $VeryColorClass:
      int X;
      int Y;

</code>

NameSpaces can only be used at the File or NameSpace Scope.