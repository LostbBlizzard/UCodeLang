$CoolEnum[int] enum: 
 Red,
 Blue,
 Green,


$CString = imut char[&];


|main[]:


 uintptr R = 0;
 eval typeinfo TypeCool = type(CoolEnum);
 $if TypeCool.GetEnumInfo(out typeinfo EnumTypeValue):
  $for [var Item : EnumTypeValue.Fields()]:
   R += sizeof(bind(typeof(Item.Name())));

 ret R;// 3 + 4 + 5 =12 