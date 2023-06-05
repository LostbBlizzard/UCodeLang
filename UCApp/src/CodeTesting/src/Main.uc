
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
 int A;
 int8 B;
 int16 C;

$CString = umut char[&];

|main[]:

 eval typeinfo TypeCool = type(Cool);
 if TypeCool.GetClassInfo(out typeinfo ClassTypeValue):
  $for [var Item : ClassTypeValue.GetFields()]:
    $FieldType = bind(Item.Type());
    FieldType V = Item.Offset() -> FieldType;

 ret 0; 