Tests:

 |func[] -> int:
  ret 10;

 [Test]
 |Funcion_1[]:
  Funcion<int> func2 = [func];
  ret func2() == 10;

 [Test]
 |Funcion_2[]:
  int number = 10;
  var object = |[int x] => x + number;
  
  Funcion<int,int> func2 = [object];
  ret func2(10) == 20;
