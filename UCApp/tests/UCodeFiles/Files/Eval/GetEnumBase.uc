
$MyBase = int;
$CoolEnum[MyBase] enum: 
 Red,
 Blue,
 Green,

|main[]:
 bool R = false;

 eval typeinfo TypeCool = type(CoolEnum);
 $if TypeCool.GetEnumInfo(out typeinfo EnumTypeValue):
  $BaseType = bind(EnumTypeValue.BaseType());
  R = type(MyBase == BaseType); 

 ret R;