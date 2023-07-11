# Address/pointers

The syntax for addressing is &Value, where Value is the expression that needs to be pointed to. For example:

-&x obtains a pointer to the variable x

-&a[i] obtains a pointer to the i-th element of the array a

-&f() obtains a pointer to the return value of the function f

References can be used like normal variables, except they cannot be nullified.

The syntax for creating references is Type& Name = Value, where Type is the type of the value, Name is the name of the reference, and Value is the expression that needs to be referred to. For example:

int& r = x creates an int reference named r that refers to x.

char& c = s[0] creates a char reference 
named c that refers to the first character of the string.

float& f = g() creates a float reference named f that refers to the return value of the function.

Here is an example of how addressing and references are used in UCode:

<code>

    int x = 10;

    int& r = x; //Creates an int reference named r that refers to x
    Console::Log(r); //Prints 10

    r = 30; //Modifies the value at r
    Console::Log(r); //Prints 30

</code>

# Reassigning references

References can be reassigned by using an Address assignment statement. This changes what value the reference points to. For example:

<code>

    int v = 10; //Assigns 10 to v

    int& r = v; //Creates an int reference named r that refers to v
    
    Console::Log(r); //Prints 10

    int w = 5; //Assigns 5 to w
    
    r &= w; //Reassigns r to refer to w
    
    Console::Log(r); //Prints 5
</code>

Reassigning references should be done with caution, as it may cause unexpected behavior or errors if done incorrectly. For example:

<code>

    r &= bitcast<int&>(0); //Reassigns r to refer to a null pointer.
    
    Console::Log(r); //Prints garbage or causes an error
</code>

Reassigning references to nullptr or invalid memory addresses is not intended by UCode’s standard, as references are always assumed to be pointing to something valid.

If you need a nullable reference type, you can use Type? instead of Type&. Type? maps to [optional<Type>](../../Standardlibrary/Types/Generic/optional.md), which is a wrapper type that can hold either a valid value or nothing (null). For example:

<code>

    int? n = []; //Creates an optional int named n

    Console::Log(n); //Prints nothing if null.

    n = 10; //Assigns 10 to n
    
    Console::Log(n); //Prints 10

    n = []; //Assigns null (nothing) back again 
    
    Console::Log(n); //Prints nothing