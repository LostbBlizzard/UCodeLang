# AddressArray(Internal Type)

An address array syntax looks like T[&] where T is any type and can be indexable unlike a normal address. 

```

  T[&]

```



The [] operator is used to index the array. 

For example, if you have an array of integers, you can access the first element of the array using array[0] like this.

```
    
    int[\5] Buffer = [0,0,0,0,0];
    
    int[&] = Buffer[0];

    Buffer[0] = 5;
    Fmt::Print(Buffer[0]);
    //outputs 5 

```


An AddressArray is able to be indexed to address the base type
or to an [Address Array](AddressArray.md) if the type is inferred.

```

    |Func[ int[&] Pointer]:
     int[&] SubPointer = Pointer[1];//an other Address Array

     int[&] Pointer = Pointer[1]; //just as address


```

The Address Array should allways be passed with the size of item in the Array this can be done by using the [Span Type](../../Standardlibrary/Types/Generic/Span.md).

```

    |Func[ int[:] Pointer]:
     int[&] SubPointer = Pointer[1];//an other Address Array

     int[&] Pointer = Pointer[1]; //just as address


```

Note addresses are unable to converted to address array.

```
  
     int a = 10;
     int& aPointer = a; 

     int[&] aAsArray = a;//error. 

```