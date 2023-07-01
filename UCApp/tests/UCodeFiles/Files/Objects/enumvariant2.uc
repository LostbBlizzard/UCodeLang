$Item enum:
 Red[int V],
 Green,
 Blue,

|main[]:

 var V = Item::Red(10);
 int r = 0;
 match V:
  Item::Red(out Num):r = Num;
  Item::Green:r =2;
  Item::Blue:r =3;

 ret r;