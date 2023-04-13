

tag DllImport;

trait Object:
 |Update[this&] -> void;


 |Update2[this&,int B] => true;
 |Update3[this&] => Update2(6);
 

[DllImport]
$Player[Object]:
 |Update[this&] -> void;
