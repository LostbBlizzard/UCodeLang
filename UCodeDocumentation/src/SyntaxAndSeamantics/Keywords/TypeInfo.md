# reflection types(keywords)

Reflection types are the showed below all are keywords that can not be used at runtime.
 
 - typeinfo : has Data about types
 - classinfo : has Data about Classes
 - enuminfo : has Data about Enums
 - aliasinfo : has Data about alias
 - traitinfo : has Data about traits


Note all functions and members are implemented internally by the compiler.

# typeinfo

Can be accessed by useing [type](./type.md) or [typeof](./type.md) keywords.

it can be used also be used as a type by useing the [bind](./type.md) keyword.


It has has the following functions.

<code>

    $typeinfo:
     
    //Get the Name of the type.
    |Name[this&] -> char[\];

    //Get the Full Name of the type.
    |FullName[this&] -> char[\];
   
    //Get the TypeID
    |TypeID[this&] -> uintptr;
      
    //if it's an Address. Ex:T&
    |IsAddress[this&] -> bool;

    //if it's an AddressArray.  Ex:T[&]
    |IsAddressArray[this&] -> bool;

    //if it's an immutable.  Ex: umut T
    |IsImmutable[this&] -> bool;

    //if it's an Dynamic.  Ex: dynamic<T>
    |IsDynamic[this&] -> bool;

    //if it's an Moved.  Ex: moved T
    |IsMoved[this&] -> bool;

    // try get the Class info.
    |GetClassInfo[this&,out classinfo Info] -> bool;

    // try get the Enum info.
    |GetEnumInfo[this&,out enuminfo Info] -> bool;
  

</code>

# classinfo

It has has the following functions.

<code>

    

    $classinfo:
     $fieldinfo:
      
      //Get the Name of Field.
      |Name[this&] -> umut char[&];
      
      //Get the type of Field.
      |GetType[this&] -> typeinfo;
      
      //Get the byte offset.
      |Offset[this&] -> uintptr;

    //Get the Fields of the Class.
    |Fields[this&] -> fieldinfo[\];


</code>

# enuminfo

It has has the following functions.

<code>

    

    $enuminfo:
     $fieldinfo:
      
      //Get the Name of EnumField.
      |Name[this&] -> char[/];
      
      //Get the Value of the EnumField.
      //The Type is the Enum basetype.
      |Value[this&] -> var;
      
      //Gets the type of the Variant if it is one.
      |TryGetVariant[this&,out typeinfo info] -> bool;
     $variantinfo:


    //Get the Base Type of the enum.
    |BaseType[this&] -> typeinfo;

    //Get the Fields of the enum.
    |Fields[this&] -> fieldinfo[\];


    //Gets the type of the Union of the Variant if it is one.
    |TryGetVariantUnion[this&,out variantinfo info] -> bool;

</code>