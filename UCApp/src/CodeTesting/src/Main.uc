
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

$Cool:
 int32 A;//4
 int8 B;//1
 int16 C;//2
 int64 D;//8

$CString = umut char[&];


|main[]:


 uintptr R = 0;
 eval typeinfo TypeCool = type(Cool);
 if TypeCool.GetClassInfo(out typeinfo ClassTypeValue):
  $for [var Item : ClassTypeValue.GetFields()]:
    $FieldType = bind(Item.Type());
    R += sizeof(FieldType);

 ret 0; 