
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

import Hello::{Func = OtherFuncion};


$Cool1 enum:
   Red,
   B2,
   B3,

$Cool2 trait:
 |Func[] -> void;

$Cool3 tag:
 |Func[] -> void;

$Cool:
 int X;
 int Y;

|main[]:
 //umut char[&] Item = type(Cool).Name();
 //eval Item2 = type(Cool).FullName();
 //eval BoolValue = compiler::HasFlag("Hello World");
 //ret BoolValue;