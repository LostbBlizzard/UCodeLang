
Tests:
 
 [Test]
 |Range1[]:
  //lists all the range types 
  var a = 0..10;
  var b = ..;
  var c = ..10;
  var d = 10..;
  var e = 0..=10;
  var f = ..=10;

 [Test]
 |Range2[]:
  int[] nums = [1,2,3,4,5];
  
  var span = nums[3..2];
 
  Assert(span.Size() == 2);
  
  Assert(span[0] == 4);
  Assert(span[1] == 5);

 [Test]
 |Range3[]:
  int[] nums = [1,2,3,4,5];

  var span = nums[..];
 
  Assert(span.Size() == 5);

  Assert(span[0] == 1);
  Assert(span[1] == 2);
  Assert(span[2] == 3);
  Assert(span[3] == 4);
  Assert(span[4] == 5);

 [Test]
 |Range4[]:
  int[] nums = [1,2,3,4,5];
  
  var span = nums[..3];
 
  Assert(span.Size() == 3);
  
  Assert(span[0] == 1);
  Assert(span[1] == 2);
  Assert(span[2] == 3);

 [Test]
 |Range5[]:
   int[] nums = [1,2,3,4,5];
   var span = nums[3..];
  
   Assert(span.Size() == 2);
   
   Assert(span[0] == 4);
   Assert(span[1] == 5);
