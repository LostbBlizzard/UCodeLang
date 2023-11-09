
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

 [Test]
 |StringSpan_4[]:
  String Txt = "Hello";
  ret Txt == "Hello";

 [Test]
 |StringSpan_5[]:
  String Txt = "World";
  ret Txt != "Hello";

 [Test]
 |StringSpan_Reverse[]:
  String Txt = "World";
  Reverse(Txt);
  ret Txt == "dlrow";

 [Test]
 |StringSpan_Reverse1[]:
  String Txt = "World";
  Reverse(Txt.Str());
  ret Txt == "dlrow";


 [Test]
 |StringSpan_StartWith[]:
  String Txt = "Hello World";
  ret StartWith(Txt,"Hello");

 [Test]
 |StringSpan_StartWith2[]:
  String Txt = "Bello World";
  ret StartWith(Txt,"Hello") == false;

  