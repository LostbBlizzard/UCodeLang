# Enum(KeyWord)

Enums are a data type that assigns symbolic names to a set of constant expression values. For example:

<code>

    $PrimaryColors enum:
      Red,
      Green,
      Blue,

</code>

The base type of an enum is an [Padded](../Concepts/Constant%20Expression.md) [byte](../Types/ints.md) by default, unless specified otherwise:

<code>

    $PrimaryColors[int] enum:
      Red,
      Green,
      Blue,

</code>

The value of each enum field is determined by the default value of the base type and the ‘++’ operator is use if the not specifyed.:

<code>

    $PrimaryColors[int] enum:
      Red ,//the default value of int is 0. 
      Green,//Red + 1 = 1
      Blue,//Green + 1 = 2

</code>

You can also specify the value of an enum field with any other constant expression:

<code>

    $PrimaryColors_t = int;
    $PrimaryColors[PrimaryColors_t] enum:
      Red = 5,//5
      Green = 5 / 10,//2
      Blue = 10 * (Green -> PrimaryColors_t),//20

</code>

# Enum Variant

Enum Variants are Keys to maped Data.

<code>
    
    $FireData;
    $IceData;
    $PowerUp enum:
     None,
     Fire[FireData],
     Ice[IceData],
     Damage[int Dmg],
     Speed[int MaxSpeed,int acceleration],

</code>


Then Can be used like this.

<code>

    |main[]:
     PowerUp nonePower =  PowerUp::None;
     PowerUp speedPower = PowerUp::Speed(5,5);
                                       //^set values
    
     if PowerUp::Speed(speedPower,out Speed,out acceleration):
      int NewMaxSpeed =Speed; 
     


</code>

You must the pass the enum with [out](../Keywords/out.md) values to get the value to get the stored value in the Enum.

note it's not possible to get the stored enum without a conditional statement such as a if,else if,while,do,match.

# Enum specification
  Enum Variants are compiled down to classes and Unions showed below in its [IR representation]("IR.md").

<code>
    
    $FireData;
    $IceData;
    $PowerUp enum:
     None,
     Fire[FireData],
     Ice[IceData],
     Damage[int Dmg],
     Speed[int MaxSpeed,int acceleration],
    

    //As IR form
    
    $FireData;
    $IceData;
    $PowerUp_Speed_type:
     int MaxSpeed;
     int acceleration;

    union PowerUp_union:
     //none has no type associated so its uneeded.
     FireData Item_1;//Fire
     IceData Item_2;//Ice
     int Item_3;// Damage
     PowerUp_Speed_type Item_4;// Speed

    $PowerUpClass:
     uint8 EnumKey;
     PowerUp_union union;


</code>

Enum Variants Destructors are a list if chans.

checking its Key With the field Key and calling its Destructor.the same idea applies to 
copy Constructors,
move Constructors,
copy assignments,
and move assignments.
