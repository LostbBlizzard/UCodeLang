$CoolEnum[int] enum: 
 Red,
 Blue,
 Green,


$CString = imut char[&];


|main[]:


 int R = 0;
 eval typeinfo TypeCool = type(CoolEnum);
 $if TypeCool.GetEnumInfo(out typeinfo EnumTypeValue):
  $for [var Item : EnumTypeValue.Fields()]:
   R += Item.Value();

 ret R;// 0 + 1 + 2 = 3 