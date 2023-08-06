

|main[] => 0;


//A simplified standard Library below.

$Vec2:
 int X = 0;
 int Y = 0;

$Vec3:
 int X = 0;
 int Y = 0;
 int Z = 0;

//Syntactic alias: T? = Optional<T>
$Optional<T> enum:
 Value[T val],
 None,

//Syntactic alias: T!E = Result<T,E>
$Result<T,E> enum:
 Value[T val],
 Error[E err],

$OpInt = int?;//make type.
$Opbool = bool?;//make type.
$Opchar = char?;//make type.

$IntVector = int[];

|NullPtr<T>[] => bitcast<T>(0);
|NullPtrArr<T>[] => bitcast<T[&]>(0);

//Syntactic alias: T[] = Vector<T>
$Vector<T>:
 T[&] _Data = NullPtrArr<T>();
 uintptr _Capacity = 0;
 uintptr _Size = 0;
 |Data[umut this&] -> T[&]:ret _Data;
 |Size[umut this&] => _Size;
 |Capacity[umut this&] => _Capacity;

$String:
 Vector<char> Base;

//inlined enum variant: X || Y || Z
//$InlinedEnum = int || bool || char;


            
