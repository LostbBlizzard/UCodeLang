# Optional\<T>(Standardlibrary/Type/Generic)

Optional is a generic type that is part of the standard library. Optional<T> is a wrapper type that can hold either a value of type T or nothing (null). 

Optional types can be used to represent values that may or may not exist, such as return values from functions that can fail, parameters that are optional, or variables that are uninitialized.

The syntax for creating optional types is Type?, where Type is any valid type. For example:

int? creates an optional int

string? creates an optional string

float[]? creates an optional array of floats

Optional types can be assigned with either a value of the same type or it's self. 

For example:

int? n = 10 assigns 10 to n

string? s = “hello” assigns “hello” to s.

float[]? f = null assigns null to f.

<code>

    //A function that takes an int and 
    returns an int?

    //It returns the square root of the input if it is positive
    //It returns null otherwise

    int? sqrt[int x]:
        if x > 0:
         ret Math::sqrt(x);
        else:
         ret [];
    
</code>

# Optional\<T> Class/Methods: 

<code>

    $Optional<T>:

     //makes an Optional with no assigned value.
     |new[this&] -> void;
     
     //makes an Optional with the assigned value.
     |new[this&,umut T& Value] -> void;
 
     //makes an Optional with the moved value.
     |new[this&,moved T Value] -> void;

     |drop[this&] -> void;

     //Returns if the Value is assigned.
     |IsEmpty[this&] -> bool;

     //Returns The Value if assigned else panic.
     |Value[this&] =>  T;

</code>

# Optional\<T> : Implentation

The Implementation of Optinoal type is a class with two members.One for the 'T' Type Called Value other is a bool for if T is assigned In that order.
For example:
<code>

    %UCode:
     $Optional<T>:
        public:
         |IsEmpty[this&] => _HasValue;

         |Value[this&] =>  _Value;

        private:
         T _Value;
         bool _HasValue;


</code>

if T is an [Address](../../../SyntaxAndSeamantics/Concepts/Address.md) The Optional will use a nullptr as its Empty value.

For example:
<code>

    $Optional<T>:
     private:
      T _Value;
      eval uintptr Nullptr = 0;

     public:
      |SetValue[this&,umut T& Value]:
       _Value &= Value;

      |SetValue[this&,moved T Value]:
       _Value &= Value;

      |SetEmpty[this&]:bitcast<uintptr&>(_Value) &= bitcast<uintptr&>(Nullptr);

      |IsEmpty[umut this&] => bitcast<uintptr>(_Value) == Nullptr;


</code>

the same concept applies to [dynamic traits](../../../SyntaxAndSeamantics/Keywords/dynamic.md).

lastly if T implements the EmptyVariant trait and is not an [Address](../../../SyntaxAndSeamantics/Concepts/Address.md) it will use EmptyVariant Funcions to check and set the Object as showed below.


<code>


    
    trait EmptyVariant:
     
     |Opt_IsEmpty[this&] -> bool;
     |Opt_SetEmpty[this&] -> void;
     |Opt_SetValue[this&,umut this& Value] -> void;
     |Opt_SetValue[this&,moved this Value] -> void;


    $Optional<T>:
     private:
      T _Value;
      eval uintptr Nullptr = 0;

     public:
      |SetValue[this&,umut T& Value]:
       _Value.Opt_SetValue(Value);

      |SetValue[this&,moved T Value]:
       _Value.Opt_SetValue(Value);

      |SetEmpty[this&]:
       _Value.Opt_IsEmpty();
    
      |IsEmpty[umut this&] => _Value.Opt_IsEmpty();


</code>

this may be used for types like unique pointer or any object that could have a null state for optimization.

how the Optional variant is chosen is by [condiional compilation](../../../SyntaxAndSeamantics/Concepts/conditional%20compliation.md) with the use of [type(TypeA == TypeB)](../../../SyntaxAndSeamantics/Keywords/type.md).