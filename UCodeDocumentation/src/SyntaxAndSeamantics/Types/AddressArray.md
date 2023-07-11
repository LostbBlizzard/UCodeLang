# AddressArray(Internal Type)

An address array syntax looks like T[&] where T is any type and can be indexable unlike a normal address. 

<code>

  T[&]

</code>



The [] operator is used to index the array. 

For example, if you have an array of integers, you can access the first element of the array using array[0] like this.

<code>
    
    int[\5] Buffer = [0,0,0,0,0];
    
    int[&] = Buffer[0];

    Buffer[0] = 5;
    Console::Log(Buffer[0]);
    //outputs 5 

</code>


An AddressArray is able to be indexed to address the base type
or to an [Address Array](AddressArray.md) if the type is inferred.

<code>

    |Func[ int[&] Pointer]:
     int[&] SubPointer = Pointer[1];//an other Address Array

     int[&] Pointer = Pointer[1]; //just as address


</code>

The Address Array should allways be passed with the size of item in the Array this can be done by using the [Span Type](../../Standardlibrary/Types/Generic/Span.md).

<code>

    |Func[ int[:] Pointer]:
     int[&] SubPointer = Pointer[1];//an other Address Array

     int[&] Pointer = Pointer[1]; //just as address


</code>

Note addresses are unable to converted to address array.

<code>
  
     int a = 10;
     int& aPointer = a; 

     int[&] aAsArray = a;//error. 

</code>