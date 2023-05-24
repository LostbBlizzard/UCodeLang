
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

/*

eval char[/1] VarableName = "Y";

$Cool:
 int X;
 int Y;

|main[]:
 
 eval Item = type(Cool).Name();
 eval Item2 = type(Cool).FullName();

 //umut char[&] Str = Item;

*/


|main[]:
 
 //int^ Buffer = unq int();
 //int$ Buffer2 = shr int();

 int[^] Buffer5 = unq int[1];
 //int[$] Buffer6 = shr int[1];
 

 //umut char[&] Str = Item;