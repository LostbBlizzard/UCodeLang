

tag DllImport;

trait Object:
 |Update[this&] -> void;
 |AutoAdded[this&,int A] => 0;

[DllImport]
$Player[Object]:
 |Update[this&] -> void;


|main[]:
 Player Obj1 = [];
 dynamic<Object> Hello = Obj1;

 Hello.AutoAdded(5);
 Hello.Update();