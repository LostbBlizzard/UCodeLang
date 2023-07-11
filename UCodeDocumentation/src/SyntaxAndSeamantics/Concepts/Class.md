# Class(concept)

Classes are a data type that defines a custom structure with fields and methods. Classes work similarly to structs in C, but with some differences. For example:



Classes can inherit from other classes using the ‘[Type]’ operator.
Classes can have constructors and destructors using the [new](./ConstructorsAndDestructors.md) and [drop](./ConstructorsAndDestructors.md) keywords.

<code>

    $Person;//a class with no members.

    $Worker[Person]://inherit from person
    String name;
    int age;


    //constructors
    |new[this&,string name,int age]:
     this.name = name;
     this.age = age;

    //destructors
    |drop[this&]:
     Console::Log("Done Working for the Day.");

</code>


Classes can have access modifiers for their fields and methods using the [public, private](./Access%20Modifiers.md).

<code>

    $Worker[Person]:
     private:
      String name;
     public:
      int age;
</code>