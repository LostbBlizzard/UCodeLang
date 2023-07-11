# int(Type)

Int is a data type that represents a 32-bit signed integer. Int can store values from -2,147,483,648 to 2,147,483,647. Int supports the following operations:

Arithmetic operators: +, -, *, /, % (addition, subtraction, multiplication, division, remainder)
Bitwise operators: &, |, ^, ~ (and, or, xor, not)
Shift operators: << , >> (left shift and right shift)
Comparison operators: == , != , < , > , <= , >= (equal to , not equal to , less than , greater than , less than or equal to , greater than or equal to)
Assignment operators: = , += , -= , *= , /= , %= (assigns a value to a variable or modifies it by an expression)
Here is an example of how int is used in UCode:

<code>

    int x = 10; //Assigns 10 to x
    int y = x + 5; //Assigns x + 5 to y
    int z = y / 2; //Assigns y / 2 to z
    Console::Log(x,y,z); //Prints 10 15 7

</code>


int16 is a data type that represents a 16-bit signed integer. Int16 can store values from -32,768 to 32,767. Int16 supports the same operations as int.


    int8 is a data type that represents an 8-bit signed integer. 
    Int8 can store values from -128 to 127. 
    Int8 supports the same operations as int.

int64 is a data type that represents a 64-bit signed integer. int64 can store values from -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807. int64 supports the same operations as int.


<code>

    int16 x = 100; //Assigns 100 to x
    int8 y = x - 50; //Assigns x - 50 to y
    int64 z = y * x; //Assigns y * x to z
    Console::Log(x,y,z); //Prints 100 50 5000

</code>


Unsigned ints are data types that represent non-negative integers. They have a ‘u’ prefix before their size, such as uint16, uint8, and uint64. Unsigned ints can store values from 0 to 2^n - 1, where n is the number of bits in the type. For example, uint16 can store values from 0 to 65,535.

<code>

    int8 y = -128; //Assigns -128 to y
    //y is represented as 10000000 in binary
</code>

# uintptr and intptr

Uintptr is a data type that represents an unsigned integer that can store a pointer value. Uintptr has the same size as the pointer type on the platform where the code is running. For example, on a 32-bit platform, uintptr has 32 bits and can store values from 0 to 4,294,967,295. 

On a 64-bit platform, uintptr has 64 bits and can store values from 0 to 18,446,744,073,709,551,615.

intptr is a data type that represents a signed integer that can store a pointer value. Intptr has the same size as the pointer type on the platform where the code is running. For example, on a 32-bit platform, intptr has 32 bits and can store values from -2,147,483,648 to 2,147,483,647. 

On a 64-bit platform, intptr has 64 bits and can store values from -9,223,372,036,854,775, 808 to 9, 223, 372, 036, 854, 775,


Uintptr and intptr are not directly aliases of other types like size_t in C/C++.
They are their own distinct types.

<code>

    int x = 10; //Assigns 10 to x
    int& p = x; //Assigns the address of x to p

    uintptr u = p -> int& -> uintptr; 
    //Casts p to uintptr and assigns it to u


    //note the bottom code assumes your using a 64-bit machine.

    uint64 A = u;//must be an explicit cast
    they're their own distinct types

    uint64 A = u -> int;//is ok.

</code>


# byte/sbyte
<code>

is a byte keyword that directly Maps maps to keyword uint8.

is a sbyte keyword that directly Maps maps to keyword sint8.

these only exist to make the cold a bit more readable and to express your intent.

</code>