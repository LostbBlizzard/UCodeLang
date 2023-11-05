Tests:
 [Test]
 |Color1[]:
  Color white = [];
  Color32 r = white -> Color32;
  ret r == Color32();

 [Test]
 |Color2[]:
  Color32 white = [];
  Color r = white -> Color;
  ret r == Color();