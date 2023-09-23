$StringSpan_t<T>:
 T[&] _data;
 uintptr _size;
 
 |new[this&]:
  _data = unsafe bitcast<T[&]>(0);
  _size = 0;
 
 unsafe |new[this&,T[&] Data,uintptr Size]:
  _data = Data;
  _size = Size;


$String_t<T>:
 T[&] _data;
 uintptr _size;
 uintptr _capacity;

 $StringSpan = StringSpan_t<T>;
 
 |new[this&]:
  _data = unsafe bitcast<T[&]>(0);
  _size = 0;
  _capacity = 0;
 |new[this&,imut StringSpan string]:
  _data = unsafe new T[string._size];
  _size = string._size;
  _capacity = string._size;

  for [uintptr i = uintptr(0);i < string._size;i++]:
   _data[i] = string._data[i];

 |drop[this&]:
  uintptr ptr =unsafe bitcast<uintptr>(_data);
  if ptr == uintptr(0):
   unsafe drop(_data);

 |realloc[this&,uintptr size]:
  
  if size > _capacity:
   var oldsize = _size;
   var old = _data;

   _capacity = size;
   _data = unsafe new T[size];
   for [uintptr i = uintptr(0);i < oldsize;i++]:
     _data[i] = old[i];


   unsafe drop(old);

 |AsSpan[this&] -> StringSpan:
  ret unsafe StringSpan(_data,_size);


 |+=[this&,imut StringSpan string]:
  var newsize = _size + string._size;
  realloc(newsize);

  var oldsize = _size;
  _size = newsize; 
  
  for [uintptr i = uintptr(0);i < newsize;i++]:
     _data[i + oldsize] = string._data[i];


$StringSpan = StringSpan_t<char>;
$String = String_t<char>;

|main[]:
 imut StringSpan Str = "Hello";//5
 imut StringSpan Str2 = " World";//6

 String Txt = Str;

 Txt += Str2;


 bool sizegood = Txt._size == uintptr(11);
 bool chargood = Txt._data[Txt._size - 1] == 'd';
 
 ret sizegood && chargood;