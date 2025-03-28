
Tests:
 
 [Test]
 |Random1[]:
  var val = Random(2);
  
  var num0 = val.NextInt(1,10);
  var num1 = val.NextInt(1,10);
  var num2 = val.NextInt(1,10);

  ret num0 == 3 && num1 == 9 && num2 == 1;

 [Test]
 |Random2[]:
  var val = Random(123456);

  int[] list = [1,2,3,4,5,6];

  list.Shuffle(val);

  //Assert Numbers gain from writing the code in C++.
  //1,6,3,2,5,4
  Assert(list[0] == 1,"Index 0 is wrong");
  Assert(list[1] == 6,"Index 1 is wrong");
  Assert(list[2] == 3,"Index 2 is wrong");
  Assert(list[3] == 2,"Index 3 is wrong");
  Assert(list[4] == 5,"Index 4 is wrong");
  Assert(list[5] == 4,"Index 5 is wrong");
