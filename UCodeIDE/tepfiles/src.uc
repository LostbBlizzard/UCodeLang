

|main[] => 0;


//A simplified standard Library below.

int a = 0;
int b = 3;

$Vec2:
 int X = 5;
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

//inlined enum variant: X || Y || Z
/*
$InlinedEnum = int || bool || char;

//Syntactic alias: T^ = Unique_ptr<T> and unq T();
$Unique_ptr<T>:
 T& Data;
 |Make[] => 0;//not added

//Syntactic alias: T$ = Shared_ptr<T> and shr T();
$Shared_ptr<T>:
 T& Data;
 uintptr RefCount = 0;
 |Make[] => 0;//not added

//Syntactic alias: T[^] = Unique_Array<T> and unq T[];
$Unique_Array<T>:
 T[&] Data;
 |Make[] => 0;//not added

//Syntactic alias: T[$] = Shared_Array<T> and shr T[];
$Shared_Array<T>:
 T[&] Data;
 uintptr RefCount = 0;
 |Make[] => 0;//not added

//Syntactic alias: T[] = Vector<T>
$Vector<T>;

$String:
 Vector<char> Base;

//Syntactic alias: T[:] = Span<T>
$Span<T>:
 T[&] Data;
 uintptr Size = 0;
 |new[]:
  Data = bitcast<T[&]>(0);
  Size = 0;
 |new[T[&] data,uintptr size]:
  Data =: data;
  Size =: size;

 |Make[T[&] data,uintptr size] -> this:ret [data,size];
 |Make[umut T[&] data,uintptr size] -> umut this:ret [bitcast<T[&]>(data),size];

 |SubSpan[uintptr offset] => Make(Data[offset],Size);

*/
            
