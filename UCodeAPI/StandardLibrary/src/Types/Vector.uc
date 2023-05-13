%ULang:

 //has built in alias in compiler.Ex: int[]
 $Vector<T>[Buffer<T>]:
  private:
   T[&] _Data;
   size_t _Size;
   size_t _Capacity;

  public:
   |new[this&]:
    _Data &= bitcast<T[&]>(0);
    _Size = 0;
    _Capacity = 0;


   |Size[umut this&] => _Size;
   |Data[umut this&] => _Data;
   |Capacity[umut this&] => _Capacity;

   |AsView[umut this&] -> umut T[:]:ret [_Data,_Size];
   |AsView[this&] -> T[:]:ret [_Data,_Size];


   |Add[this&,umut T& Item] -> void;
   |Add[this&,moved T Item] -> void;
  
   |Add[this&,umut T[:] Items] -> void;
   |Add[this&,moved T[:] Items] -> void;


   |RemoveAtIndex[size_t Index] -> void;
   |RemoveIf<T>[T CallAble]:
    for [var& Item : AsView().AsReverseView()]:
     if CallAble(Item):
      Pop();
  
   |Resize[this&,size_t Offset] -> void;
   |Clear[this&] -> void;
   |Pop[this&] -> void;