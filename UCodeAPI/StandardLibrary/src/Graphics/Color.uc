

$ColorRGB:
 float R = 1;
 float G = 1;
 float B = 1;
 |new[this&] -> void;

 |new[this&,float R,float G,float B]:
  this.R = R;
  this.G = G;
  this.B = B;

 |==[this&,IPar<this> other] -> bool:
  ret this.R == other.R && this.G == other.G && this.B == other.B;

 |!=[this&,IPar<this> other] -> bool:
  ret !(this == other);

 |->[imut this&] -> Color24:
  ret [(R * 255) -> int -> byte,(G * 255) -> int -> byte,(B * 255) -> int -> byte];

$Color:
 float R = 1;
 float G = 1;
 float B = 1;
 float A = 1;
 |new[this&] -> void;
 
 |new[this&,float R,float G,float B,float A]:
  this.R = R;
  this.G = G;
  this.B = B;
  this.A = A;
 
 |new[this&,IPar<ColorRGB> color,float A]:
  this.R = color.R;
  this.G = color.G;
  this.B = color.B;
  this.A = A;
 
 |WithOutOpacity[this&] -> ColorRGB:
  ret [R,G,B];

 |==[this&,IPar<this> other] -> bool:
  ret this.R == other.R && this.G == other.G && this.B == other.B && this.A == other.A;

 |!=[this&,IPar<this> other] -> bool:
  ret !(this == other);

 |->[imut this&] -> Color32:
  ret [(R * 255) -> int -> byte,(G * 255) -> int -> byte,(B * 255) -> int -> byte,(A * 255) -> int -> byte];