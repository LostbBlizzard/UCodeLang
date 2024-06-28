# This(KeyWord)

The 'this' is a keyword that refers to the this parameter. 

It is a address that points to the object of the class of which the member function is called.

```
$Item:
   int Hp;
     
   |DoDamage[this&]:
    this.Hp = 10;
   
    Hp = 10;//does the same as above

    int V = this.Hp;
    
    int V2 = this.Hp;//does the same as above

```

 The this keyword can also be used to be an alias current class type.

 ```
$BigTree:
   int Hp;
   |AddBoth[umut this&,umut this& Other] -> BigTree:
      ret [this.Hp + Other.Hp];

 ```

 becomes

 ```
   $BigTree:
     int Hp;
     |AddBoth[umut BigTree,umut BigTree& Other] -> Item:
      ret [this.Hp + Other.Hp];

 ```