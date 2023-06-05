$Cool:
 int32 A;//1
 int8 B;//2
 int16 C;//3
 int64 DVal;//7

|main[]:
 int R = 0;

 eval typeinfo TypeCool = type(Cool);
 if TypeCool.GetClassInfo(out typeinfo ClassTypeValue):
  $for [var Item : ClassTypeValue.GetFields()]:
    $FieldType = bind(Item.Type());
    R += sizeof(Item.Type().Name());

 ret R; 