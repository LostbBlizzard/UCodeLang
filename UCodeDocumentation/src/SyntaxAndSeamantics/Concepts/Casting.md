Casting is a way of converting a value from one type to another. Casting can be used to change the representation or interpretation of a value, such as converting a float to an int or an int to a pointer. Casting can also be used to perform explicit conversions that are not allowed implicitly, such as converting an int64 to an int8 or an uint16 to an int16.

The syntax for casting is Value -> Type, where Value is the expression that needs to be converted and Type is the desired type. For example:

3.14 -> int converts the float 3.14 to the int 3
65 -> char converts the int 65 to the char ‘A’
x -> uint8 converts the variable x to an uint8
Casting can have different effects depending on the types involved. For example:

Casting between numeric types can cause truncation, rounding, or overflow if the value does not fit in the target type.
Casting between pointer types can change the address or size of the pointed object.
Casting between signed and unsigned types can change the sign bit or the value range of the integer.
Here is an example of how casting is used in UCode:

<code>

    float x = 1.23; //Assigns 1.23 to x
    int y = x -> int; //Casts x to int and assigns it to y
    Console::Log(y); //Prints 1

    char c = 'a'; //Assigns 'a' to c
    int z = c -> int; //Casts c to int and assigns it to z
    Console::Log(z); //Prints 97

    uintptr u = (uintptr) (x -> int&) -> uintptr; //Casts the address of x to uintptr and assigns it to u
    Console::Log(u);

</code>