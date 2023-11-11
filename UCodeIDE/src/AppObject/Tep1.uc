$StringSpan_t<T>:
 private:
  T[&] _data;
  uintptr _size;
 public: 
  //$MyString = String_t<T>;
  //$MySpan = Span<T>;
  
  |new[this&]:
   _data = unsafe bitcast<T[&]>(0);
   _size = 0;
 
  unsafe |new[this&,T[&] data,uintptr size]:
   _data = data;
   _size = size;

  |Size[imut this&] => _size;
  unsafe |iData[imut this&] -> imut T[&]:ret _data;
  unsafe |Data[this&] -> T[&]:ret _data; 

  |==[imut this&,imut this& Other] -> bool:
   if this.Size() != Other.Size():ret false;

   for [uintptr i = 0;i < this.Size();i++]:

    if this[i] != Other[i]:ret false;

   ret true;

  |!=[imut this&,imut this& Other] => !(this == Other);

  |[][this&,uintptr Index] -> T&:ret _data[Index];
  |[][imut this&,uintptr Index] -> imut T&:ret _data[Index];


$StringSpan = StringSpan_t<char>;

|main[] => "Hello" == "World ";
