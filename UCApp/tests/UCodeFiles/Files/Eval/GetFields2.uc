$Cool:
 int32 A;//1
 int8 B;//2
 int16 C;//3
 int64 DVal;//7

|main[]:
 uintptr R = 0;

 eval typeinfo TypeCool = type(Cool);
 if TypeCool.GetClassInfo(out typeinfo ClassTypeValue):
  $for [var Item : ClassTypeValue.Fields()]:
    $FieldType = bind(typeof(Item.Name()));
    R += sizeof(FieldType);

 ret R; 