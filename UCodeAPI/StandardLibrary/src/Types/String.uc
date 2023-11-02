

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
  unsafe |Data[imut this&] -> imut T[&]:ret _data;
  unsafe |Data[this&] -> T[&]:ret _data; 

  |==[imut this&,imut this& Other] -> bool:
   if this.Size() != Other.Size():ret false;

   for [uintptr i = 0;i < this.Size();i++]:

    if this[i] != Other[i]:ret false;

   ret true;

  |!=[imut this&,imut this& Other] => !(this == Other);

  |AsSpan[this&] -> T[:]:ret unsafe [_data,_size];
  |AsSpan[imut this&] -> imut T[:]:ret unsafe [_data,_size];

  
  |ToStr[MySpan& span] -> this: ret unsafe [span.Data(),span.Size()];
  |ToStr[imut MySpan& span] -> this:ret unsafe [span.Data(),span.Size()];

  |[][this&,uintptr Index] -> T&:ret _data[Index];
  |[][imut this&,uintptr Index] -> imut T&:ret _data[Index];

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
   for [uintptr i = 0;i < span.Size();i++];//this[i] = span[i];

  unsafe |Data[imut this&] -> imut T[&]:ret unsafe _base.Data();
  unsafe |Data[this&] -> T[&]:ret unsafe _base.Data();
  

  |Size[imut this&] => _base.Size();
  |Capacity[imut this&] => _base.Capacity();

  |Resize[this&,uintptr Size] -> void:_base.Resize(Size);
  |Reserve[this&,uintptr Size] -> void:_base.Reserve(Size);
  |Clear[this&] -> void:_base.Clear();

  |Pop[this&] -> T:ret _base.Pop();
  |Remove[this&,uintptr Index] -> T:ret _base.Remove(Index);

  |Push[this&,imut T& Val] -> void:_base.Push(Val);
  //|Push[this&,moved T Val] -> void:_base.Push(Val);

  |Insert[this&,uintptr Index,imut T& Item] -> void:_base.Insert(Index,Item);
  //|Insert[this&,uintptr Index,moved T Item] -> void:_base.Insert(Index,Item);
  
  //Not required Funcions 
  |[][this&,uintptr Index] -> T&:ret _base[Index];
  |[][imut this&,uintptr Index] -> imut T&:ret _base[Index];

  |Append[this&,imut T[:] Val] -> void:_base.Append(Val);
  //|Append[this&,moved T[:] Val] -> void:_base.Append(Val);

  |==[imut this&,imut this& Other] -> bool:
   ret this.AsStrSpan() == Other.AsStrSpan();

  |!=[imut this&,imut this& Other]:ret !(this == Other);

  |+[imut this&,imut this& Other] -> this;

  |+=[this&,imut this& Other] -> this;


  |==[imut this&, IPar<MyStringSpan> Other] -> bool:
   ret this.AsStrSpan() == Other;

  |!=[imut this&, IPar<MyStringSpan> Other]:ret !(this == Other);

  //|+[imut this&, IPar<MyStringSpan> Other] -> this;

  |+=[this&, IPar<MyStringSpan> Other] -> void;

  |AsSpan[this&] -> T[:]:ret unsafe [];
  |AsSpan[imut this&] -> imut T[:]:ret unsafe [];

  |AsStrSpan[this&] -> MyStringSpan:ret unsafe [];
  |AsStrSpan[imut this&] -> imut MyStringSpan:ret unsafe [];

  |[][this&,Range_t<uintptr> Range] -> MyStringSpan:ret AsStrSpan()[Range];
  |[][imut this&,Range_t<uintptr> Range] -> imut MyStringSpan:ret AsStrSpan()[Range];

$String = String_t<char>;
$StringSpan = StringSpan_t<char>;

$String8 = String_t<utf8>;
$String8Span8 = StringSpan_t<utf8>;

$String16 = String_t<utf16>;
$String16Span = StringSpan_t<utf16>;

$String32 = String_t<utf32>;
$String32Span = StringSpan_t<utf32>;
