
$FuncPointer = |[int a,int b] -> int;

|Add[int a,int b] => a + b;
|Mult[int a,int b] => a * b;

|main[]:
 FuncPointer V = Add;
 int a = 99;
 a = V(5,5);

 V = Mult;

 a = V(a,2);

 ret a;




/*
|main[] -> async<bool>:
 async<int> a = await func();
 yield a;
*/ 

//yield await [] => 0;
//ret 0;

//$Future<T>;
/*
IntVector VectorTest = [];
String StringTest = [];

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
|AreSamePtrArr<T>[umut T[&] A,umut T[&] B] => bitcast<uintptr>(A) == bitcast<uintptr>(B);
|IsNullPtrArr<T>[umut T[&] Ptr] => bitcast<uintptr>(Ptr) == uintptr(0);

//Syntactic alias: T[] = Vector<T>
$Vector<T>:
 private:
  T[&] _Data = NullPtrArr<T>();
  uintptr _Capacity = 0;
  uintptr _Size = 0;
  |Data[umut this&] -> T[&]:ret _Data;//So  ClassAssembly::IsString_t will work.
 public: 
  |Size[umut this&] => _Size;
  |Capacity[umut this&] => _Capacity;
 
  |Resize[this&,uintptr size] -> void:
   Reserve(size);
   _Size = size;
  |Reserve[this&,uintptr size] -> void:

    if IsNullPtrArr(_Data):
      drop(_Data);

    _Data =: new T[size]; 
    _Capacity = size;

  |Clear[this&]:_Size = 0;

  |Push[this&,moved T Item] => Insert(Item,_Size);
  |Push[this&,umut T& Item] => Insert(Item,_Size);
  |Pop[this&] => Remove(_Size - uintptr(1));

  |Remove[this&,uintptr Index] -> T;
 
  |Insert[this&,moved T Item,uintptr Index] -> void:
   //_Size++;
   //if _Capacity < _Size:
   //Resize(_Size);

   //_Data[Index] = Item;
    
  |Insert[this&,umut T& Item,uintptr Index] -> void:
   //_Size++;
   //if _Capacity < _Size:
   //Resize(_Size);

   //_Data[Index] = Item;

$String = String_t<char>;

$String_t<T>:
 private:
  Vector<T> Base = [];
  |Data[umut this&] => Base.Data();
 public: 
  
  |Size[umut this&] => Base.Size();
  |Capacity[umut this&] => Base.Capacity();

  |Resize[this&,uintptr size] => Base.Resize(size);
  |Clear[this&] => Base.Clear();
  |Push[this&, T Item] => Base.Push(Item);
  |Pop[this&] => Base.Pop();
  |Remove[this&,uintptr Index] => Base.Remove(Index);
  |Insert[this&,uintptr Index, T Item] => Base.Insert(Item,Index);

//inlined enum variant: X || Y || Z
//$InlinedEnum = int || bool || char;

*/
            
