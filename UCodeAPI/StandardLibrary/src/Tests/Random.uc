
Tests:
 
 [Test]
 |Random1[]:
  var val = Random(2);
  
  var num0 = val.NextInt(1,10);
  var num1 = val.NextInt(1,10);
  var num2 = val.NextInt(1,10);

  ret num0 == 3 && num1 == 9 && num2 == 1;
