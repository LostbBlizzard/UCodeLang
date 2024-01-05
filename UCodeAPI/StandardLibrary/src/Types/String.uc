

$StringSpan_t<T>:
 private:
  T[&] _data;
  uintptr _size;
 public: 
  //$MyString = String_t<T>;
  $MySpan = Span<T>;
  
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

  |AsSpan[this&] -> T[:]:ret unsafe [_data,_size];
  |iAsSpan[imut this&] -> imut T[:]:ret unsafe [_data,_size];

  
  |ToStr[MySpan& span] -> this: ret unsafe [span.Data(),span.Size()];
  |ToStr[imut MySpan& span] -> this:ret unsafe [span.Data(),span.Size()];

  |[][this&,uintptr Index] -> T&:
    $if compiler::IsDebug():
      if Index >= _size:panic("Index is out of bounds");

    ret _data[Index];
  |[][imut this&,uintptr Index] -> imut T&:
    $if compiler::IsDebug():
      if Index >= _size:panic("Index is out of bounds");

    ret _data[Index];

  |[][this&,Range_t<uintptr> Range] -> this:ret ToStr(AsSpan()[Range]);
  |[][imut this&,Range_t<uintptr> Range] -> this:ret ToStr(AsSpan()[Range]);

$String_t<T>:
 private: 
  Vector<T> _base;//there's some optimizations we could do but I just need something working
 public:
  $MyStringSpan = StringSpan_t<T>;
  $MySpan = Span<T>;

  |new[this&] -> void;

  
  |new[this&,IPar<MyStringSpan> span] -> void:
   Resize(span.Size());
   for [uintptr i = 0;i < span.Size();i++]:this[i] = span[i];

  unsafe |iData[imut this&] -> imut T[&]:ret unsafe _base.iData();
  unsafe |Data[this&] -> T[&]:ret unsafe _base.Data();
  

  |Size[imut this&] => _base.Size();
  |Capacity[imut this&] => _base.Capacity();

  |Resize[this&,uintptr Size] -> void:_base.Resize(Size);
  |Reserve[this&,uintptr Size] -> void:_base.Reserve(Size);
  |Clear[this&] -> void:_base.Clear();

  |Pop[this&] -> T:ret _base.Pop();
  |Remove[this&,uintptr Index] -> T:ret _base.Remove(Index);

  |Push[this&,imut T& Val] -> void:_base.Push(Val);
  |Push[this&,moved T Val] -> void:_base.Push(Val);

  |Insert[this&,uintptr Index,imut T& Item] -> void:_base.Insert(Index,Item);
  |Insert[this&,uintptr Index,moved T Item] -> void:_base.Insert(Index,Item);
  
  //Not required Functions 
  |[][this&,uintptr Index] -> T&:ret _base[Index];
  |[][imut this&,uintptr Index] -> imut T&:ret _base[Index];

  |Append[this&,imut T[:] Val] -> void:_base.Append(Val);
  |Append[this&,moved Span<T> Val] -> void:_base.Append(Val);

  |==[imut this&,imut this& Other] -> bool:
   ret this.iStr() == Other.iStr();

  |!=[imut this&,imut this& Other]:ret !(this == Other);

  |+[imut this&,imut this& Other] -> this:
   ret this + Other.iStr(); 

  |+=[this&,imut this& Other] -> void:
   this += Other.iStr();


  |==[imut this&, IPar<MyStringSpan> Other] -> bool:
   ret this.iStr() == Other;

  |!=[imut this&, IPar<MyStringSpan> Other]:ret !(this == Other);

  |+[imut this&, IPar<MyStringSpan> Other] -> this:
   this copy = this;
   copy += Other;
   ret copy;

  |+=[this&, IPar<MyStringSpan> Other] -> void:
   _base.Append(Other.iAsSpan());

  |AsSpan[this&] -> T[:]:ret unsafe [];
  |iAsSpan[imut this&] -> imut T[:]:ret unsafe [];

  |Str[this&] -> MyStringSpan:ret unsafe [];
  |iStr[imut this&] -> imut MyStringSpan:ret unsafe [];

  |[][this&,Range_t<uintptr> Range] -> MyStringSpan:ret Str()[Range];
  |[][imut this&,Range_t<uintptr> Range] -> imut MyStringSpan:ret Str()[Range];

$String = String_t<char>;
$StringSpan = StringSpan_t<char>;

$String8 = String_t<utf8>;
$String8Span = StringSpan_t<utf8>;

$String16 = String_t<utf16>;
$String16Span = StringSpan_t<utf16>;

$String32 = String_t<utf32>;
$String32Span = StringSpan_t<utf32>;
