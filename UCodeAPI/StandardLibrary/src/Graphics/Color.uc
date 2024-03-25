

$ColorRGB export:
 float R = 1;
 float G = 1;
 float B = 1;
 export |new[this&] -> void;

 export |new[this&,float R,float G,float B]:
  this.R = R;
  this.G = G;
  this.B = B;

 export |==[this&,IPar<this> other] -> bool:
  ret this.R == other.R && this.G == other.G && this.B == other.B;

 export |!=[this&,IPar<this> other] -> bool:
  ret !(this == other);

 export |->[imut this&] -> Color24:
  ret [(R * 255) -> int -> byte,(G * 255) -> int -> byte,(B * 255) -> int -> byte];

$Color export:
 float R = 1;
 float G = 1;
 float B = 1;
 float A = 1;
 export |new[this&] -> void;
 
 export |new[this&,float R,float G,float B,float A]:
  this.R = R;
  this.G = G;
  this.B = B;
  this.A = A;
 
 export |new[this&,IPar<ColorRGB> color,float A]:
  this.R = color.R;
  this.G = color.G;
  this.B = color.B;
  this.A = A;
 
 export |RemoveOpacity[this&] -> ColorRGB:
  ret [R,G,B];

 export |==[this&,IPar<this> other] -> bool:
  ret this.R == other.R && this.G == other.G && this.B == other.B && this.A == other.A;

 export |!=[this&,IPar<this> other] -> bool:
  ret !(this == other);

 export |->[imut this&] -> Color32:
  ret [(R * 255) -> int -> byte,(G * 255) -> int -> byte,(B * 255) -> int -> byte,(A * 255) -> int -> byte];