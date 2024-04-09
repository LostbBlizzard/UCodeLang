
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
 |StringSpan_6[]:
  String Txt = "Hello";
  Txt += " World";
  ret Txt == "Hello World";
 
 [Test]
 |StringSpan_8[]:
  var Txt = "Hello";
  var copy = Txt;
  ret copy == "Hello World";

 [Test]
 |StringSpan_9[]:
  var Txt = "Hello";
  var copy = move Txt;
  ret copy == "Hello World" && Txt.Size() == 0;

 [Test]
 |StringSpan_10[]:
  //var Txt = "Hello" + " World";
  //ret Txt == "Hello World";

 [Test]
 |StringSpan_11[]:
  //String v = " World";
  //var Txt = "Hello" + v;
  //ret Txt == "Hello World";