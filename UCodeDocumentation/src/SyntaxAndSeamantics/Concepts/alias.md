# Alias

allows to reference type with an another name.

also see [import Alias](../Keywords/import.md).

for example.
```
 
    $CoolInt = int;
    |main[]
     bool AreSame = type(CoolInt == int);//true

     CoolInt Num = 0;//work's like a int.

```

A Typed alias can be done like this.

```
 
    $CoolInt = !int;

```
this makes the alias that's a completely unique type.
```
 
    $CoolInt = !int;
    |main[]
     bool AreSame = type(CoolInt == int);//false

     CoolInt Num = 0 -> CoolInt;//must be casted. 

```

also alias's can be generic.
```

    $FVector<T> = T[];

```