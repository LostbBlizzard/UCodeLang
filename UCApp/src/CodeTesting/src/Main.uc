
/*

|main[]:
 int a = 10;
 match a:
  1:a = 10;
  2:a = 20;
  3:a = 30;
  invalid:a = 0;
   
 
 int V = match a:
  50 => 500;
  70 => 700;
  invalid => 100;
 ;

*/

//import Hello::{Func = OtherFuncion};

$CoolEnum enum: 
 Red,
 Blue,
 Green,


$CString = umut char[&];


|main[]:


 uintptr R = 0;
 eval typeinfo TypeCool = type(CoolEnum);
 $if TypeCool.GetEnumInfo(out typeinfo EnumTypeValue):
  $for [var Item : EnumTypeValue.Fields()]:
   CString V = Item.Name();
   var ValueOfEnum = Item.Value();

 ret 0; 