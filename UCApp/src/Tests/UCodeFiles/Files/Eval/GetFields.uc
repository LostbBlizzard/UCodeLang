$Cool:
 int32 A;//4
 int8 B;//1
 int16 C;//2
 int64 D;//8

|main[]:
 int R = 0;
 
 eval typeinfo TypeCool = type(Cool);
 if TypeCool.GetClassInfo(out typeinfo ClassTypeValue):
  $for [var Item : ClassTypeValue.Fields()]:
    $FieldType = bind(Item.Type());
    R += sizeof(FieldType);

 ret R; 