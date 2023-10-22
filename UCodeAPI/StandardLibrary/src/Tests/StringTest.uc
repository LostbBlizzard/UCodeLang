
Tests:
 
 [Test]
 |StringSpan_1[]:
  ret "Hello".Size() == 5;
 
 [Test]
 |StringSpan_2[]:
  ret "Hello" != "World";

 [Test]
 |StringSpan_3[]:
  ret "Hello" == "Hello";

 //[Test]
 |StringSpan_4[]:
  String Txt = "Hello";
  ret Txt == "Hello";


  