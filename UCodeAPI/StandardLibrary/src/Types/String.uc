

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

$String_t<T>:
 private: 
  Vector<T> _base;//there's some optimizations we could do but I just need something working
 public:
  //$StringSpan = StringSpan_t<T>;
  $StringSpan = int;

  unsafe |Data[imut this&] -> imut T&:ret unsafe _base.Data();
  unsafe |Data[this&] -> T&:ret unsafe _base.Data();
  

  |Size[imut this&] => _base.Size();
  |Capacity[imut this&] => _base.Capacity();

  |Resize[this&,uintptr Size] -> void:_base.Resize(Size);
  |Reserve[this&,uintptr Size] -> void:_base.Reserve(Size);
  |Clear[this&] -> void:_base.Clear(Size);

  |Pop[this&] -> T:ret _base.Pop(Size);
  |Remove[this&,uintptr Index] -> T:ret _base.Remove(Index);

  |Push[this&,imut T& Val] -> void:_base.Push(Val);
  //|Push[this&,moved T Val] -> void:_base.Push(Val);

  |Insert[this&,uintptr Index,imut T& Item] -> void:_base.Insert(Index,Item);
  //|Insert[this&,uintptr Index,moved T Item] -> void:_base.Insert(Index,Item);
  
  //Not required Funcions 
  |[][this&,uintptr Index] -> T&:ret _base[Index];
  |[][imut this&,uintptr Index] -> imut T&:ret _base[Index];

  |Append[this&,imut T[:] Val] -> void:_base.Append(Val);
  |Append[this&,moved T[:] Val] -> void:_base.Append(Val);

  |==[imut this&,imut this& Other] -> bool;

  |!=[imut this&,imut this& Other]:ret !(this == Other);

  |+[imut this&,imut this& Other] -> this;

  |+=[this&,imut this& Other] -> this;


  |==[imut this&, IPar<StringSpan> Other] -> bool;

  |!=[imut this&, IPar<StringSpan> Other]:ret !(this == Other);

  //|+[imut this&, IPar<StringSpan> Other] -> this;

  |+=[this&, IPar<StringSpan> Other] -> void;

$String = String_t<char>;
$StringSpan = StringSpan_t<char>;

$String8 = String_t<utf8>;
$StringSpan8 = StringSpan_t<utf8>;

$String16 = String_t<utf16>;
$StringSpan16 = StringSpan_t<utf16>;

$String32 = String_t<utf32>;
$StringSpan32 = StringSpan_t<utf32>;
