# type/typeof/bind(keyword)

the type keyword allows for 
- obtaining [type info](./TypeInfo.md)
- comparing types
- binding types


# obtaining type info

to obtain type info can be done by



<code>

        |main[] -> void:
         BoolType B = type(bool);
         var TypeInfo = typeof(B);

</code>

Note typeof is for expressions and will not will never execute the code at runtime.

Use cases can be found on [type info](./TypeInfo.md).

#  comparing types

Comparing types can be done below

<code>

        |main[] -> void:
         var AreTheSameType = type(bool == int);

</code>

when comparing types it will return a constant expression Boolean.

here's the list of valid operators.

- == : will will directly compare the two types and will return true if exactly the same and false otherwise.

- != will will directly compare the two types and will return false if exactly the same and true otherwise.

- ~= will will [approximately compared](../Concepts/approximately%20compared.md) the types works the same as == but will be also true if the following is true.

  -if right side is the base type left of Ex: type(int ~= uint_t) is true.
  including traits.
  
#  binding types

using the bind keyword allows to convert [type info](./TypeInfo.md) to a type declaration.
<code>

        |main[] -> void:
         var BoolType = type(bool);


         bind(BoolType) Item = true;

</code>