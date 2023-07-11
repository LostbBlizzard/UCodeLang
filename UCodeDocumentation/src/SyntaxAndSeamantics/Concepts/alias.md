# Alias

allows to reference type with an another name.

also see [import Alias](../Keywords/import.md).

for example.
<code>
 
    $CoolInt = int;
    |main[]
     bool AreSame = type(CoolInt == int);//true

     CoolInt Num = 0;//work's like a int.

</code>

a hard alias can be done like this.

<code>
 
    $CoolInt = !int;

</code>
this makes the alias that's a completely unique type.
<code>
 
    $CoolInt = !int;
    |main[]
     bool AreSame = type(CoolInt == int);//false

     CoolInt Num = 0 -> CoolInt;//must be casted. 

</code>

also alias's can be generic.
<code>

    $Vector<T> = T[];

</code>