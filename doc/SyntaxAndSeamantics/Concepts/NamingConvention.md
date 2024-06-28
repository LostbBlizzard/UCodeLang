# Name convention
Pascal case is a naming convention that is preferred by UCode’s standard for naming types, functions, and constants. Pascal case means that every word in the name starts with a capital letter and there are no separators between words. For example:

SomeType is a type name in Pascal case
DoSomething is a function name in Pascal case
PI is a constant name in Pascal case
Pascal case makes the names easy to read and distinguish from other naming conventions. It also helps to avoid conflicts with keywords or built-in types that are usually lowercase. For example:

int is a keyword and a built-in type
Int is a user-defined type in Pascal case
Pascal case should be used consistently throughout the code to maintain clarity and readability. Here is an example of how Pascal case is used in UCode:

```

    //A user-defined type that represents a point
    $Point: 
     int X;
     int Y;


      //A function that takes two points and returns their distance
      $Distance[Point p1, Point p2]:
        return Math::Sqrt(
      (p1.X - p2.X)+(p1.X - p2.X) 
    + (p1.Y - p2.Y)+ (p1.Y - p2.Y)
    );


    //A constant that holds the value of pi
    umut float PI = 3.14159;

    //Creating two points and printing their distance



    Point origin = [0, 0]; //Assigns 0 to both X and Y fields
    Point target = [3, 4]; 
    //Assigns 3 to X and 4 to Y fields

    Console::Log(Distance(origin, target)); //Prints 5

```