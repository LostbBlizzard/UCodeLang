# var(Keyword)

In UCode, the auto keyword is used to specify that the variable’s data type will automatically be deducted from its initializer. 

The compiler decides the data type for a variable by looking at its initialization.

<code>

    var num = 10;//The data type is int32

    var Str = "Hello World";//The data type is umut char[&]

</code>

This can also be done partially.

<code>

    int A = 10;
    var& B = A;//type will be come int&
</code>




lastly be used as function return.


<code>

    |Func[] -> var:
     ret 10;

    //Func will be int32
 
</code>

note the compiler will the first return statement it finds as the function return.

<code>

    |Func[] -> var:
     if true:
      ret true;
     else:
      ret 0;

    //Func return type will be bool can and throw an error because int32 cant be implicitly convertible as bool.
 
</code>

note the compiler prefers returning by Value and not by address.


the two Funcs may look the same but
have very different results.may cause subtle bugs if not observant.

<code>

    |Func[] -> var:
     static V;
     ret V;
 
</code>

<code>

    |Func[] -> var&:
     static V;
     ret V;
 
</code>
