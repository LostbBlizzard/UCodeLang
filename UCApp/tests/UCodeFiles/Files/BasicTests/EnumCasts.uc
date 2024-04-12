$Color enum:
 Red,
 Blue,
 Green,
 Default = 0,


|main[]:
 Color v = Color::Green;
 ret v -> byte;
 
|main2[]:
 byte v = 2;
 ret (v -> Color) == Color::Green;


