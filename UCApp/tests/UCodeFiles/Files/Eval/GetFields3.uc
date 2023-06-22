$Cool:
 int64 A;//0
 int64 B;//8
 int64 C;//16

|main[]:
 int R = 0;

 eval typeinfo TypeCool = type(Cool);
 if TypeCool.GetClassInfo(out typeinfo ClassTypeValue):
  $for [var Item : ClassTypeValue.Fields()]:
    $FieldType = bind(Item.Type());
    R += Item.Type().Offset();

 ret R; 