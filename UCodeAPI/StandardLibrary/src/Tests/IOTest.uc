

Tests:

 [Test]
 |Print1[] -> bool:
  Fmt::Println("Hello World");
  ret true;

 [Test]
 |Print2[] -> bool:
  Fmt::Print('H');
  Fmt::Println('i');
  ret true;