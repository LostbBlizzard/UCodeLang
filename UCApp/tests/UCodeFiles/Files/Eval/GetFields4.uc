$Cool:
 int64 A;//0 , 0
 int64 B;//8 , 1
 int64 C;//16 , 2

|main[]:
 uintptr R = 0;

 eval typeinfo TypeCool = type(Cool);
 if TypeCool.GetClassInfo(out typeinfo ClassTypeValue):
  $for [uintptr i,var Item : ClassTypeValue.Fields()]:
    $FieldType = bind(Item.Type());
    R += Item.Offset() + i;

 ret R; 