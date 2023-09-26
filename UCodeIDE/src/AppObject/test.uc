/*
$Range_t<T>:
  T _start;
  T _end;
  |new[this&]:
   _start = [];
   _end = [];
  |new[this&,T start,T end]:
   _start = start;
   _end = end;

|Range<T>[T start,T end] -> Range_t<T>:
 ret [start,end];
 

$Span<T>:
 T[&] _data;
 uintptr _size;
 |new[this&]:
  _data = unsafe bitcast<T[&]>(0);
  _size = 0;

 |new[this&,T[&] data,uintptr size]:
  _data = data;
  _size = size;

 $FindFunc = |[T& Ptr] -> bool;
 |Find[this&,FindFunc Func] -> int&?:
  ret Null;

$Vector<T>:
 T[&] _data;
 uintptr _size;
 uintptr _capacity;

 |new[this&]:
  _data = unsafe bitcast<T[&]>(0);
  _size = 0;
  _capacity = 0;
 
 |new<X...>[this&,X Elems] -> void:
  _data = unsafe bitcast<T[&]>(0);
  _size = 0;
  _capacity = 0;

  Resize(Elems.Count());
  
  uintptr Index = 0;
  $for [var& Item : Elems]:
   _data[Index] = Item;
   Index++;

 |Resize[this&,uintptr size]:
  if size > _capacity:
   var oldsize = _size;
   var old = _data;

   _capacity = size;
   _size = size;
   _data = unsafe new T[size];
   for [uintptr i = uintptr(0);i < oldsize;i++]:
     _data[i] = old[i];

   uintptr ptr =unsafe bitcast<uintptr>(old);
   if ptr != uintptr(0):
      unsafe drop(old);

 |[][this&,Range_t<uintptr> V] -> int[:]:
  ret [_data[V._start],V._end - V._start];

 |drop[this&]:
  uintptr ptr =unsafe bitcast<uintptr>(_data);
  if ptr != uintptr(0):
   unsafe drop(_data);


$Optional<T> enum:
 Option[T Value],
 Null,
 


|main[] -> int:
 
 int[] list = [0,1,2,3,4,5,6,7,8,9,10];
 
 int[:] span = list[5..10];

 int&? numOp = span.Find([x] => x == 7);
 
 

 //int&? numOp = Null;
 //ret 0;
 
 match numOp:
  Null:ret 0;
  Option(out Value):ret Value;

*/

/*
$StringSpan_t<T>:
 //$String = String_t<T>;
 T[&] _data;
 uintptr _size;
 |new[this&]:
  _data = unsafe bitcast<T[&]>(0);
  _size = 0;

 unsafe |new[this&,T[&] data,uintptr size]:
  _data = data;
  _size = size;

 |Size[imut this&] => _size;
 unsafe |Data[imut this&] => _data;

$StringSpan =StringSpan_t<char>;
*/


$Vec4_t<T>:
 T X = 1;
 T Y = 2;
 T Z = 3;
 T W = 4;

$Vec4 = Vec4_t<int64>;

|CopyV[imut Vec4 Val]:
 ret Val;

|main2[Vec4 Val]: 
 ret CopyV(Val);

|Get_X[Vec4 Val]: 
 ret Val.X;

|Get_Y[Vec4 Val]: 
 ret Val.Y;

|Get_Z[Vec4 Val]: 
 ret Val.Z;

|Get_W[Vec4 Val]: 
 ret Val.W;

/*
|Print[imut StringSpan Val]:
 ret Val.Size() + uintptr(5);

|Copy[imut StringSpan Val]:
 ret Val;
|main[]: 
 //var V = "Hello World";
 ret Copy("Hello World");// V.Size();

*/