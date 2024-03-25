
$Color24 export:
 byte R = 255;
 byte G = 255;
 byte B = 255;
 export |new[this&] -> void;

 export |new[this&,byte r,byte g,byte b]:
  this.R = r;
  this.G = g;
  this.B = b;

 export |==[this&,IPar<this> other] -> bool:
  ret this.R == other.R && this.G == other.G && this.B == other.B;

 export |!=[this&,IPar<this> other] -> bool:
  ret !(this == other);

 export |->[imut this&] -> ColorRGB:
  ret [R -> int -> float / 255,G -> int -> float / 255,B -> int -> float / 255];

$Color32 export:
 byte R = 255;
 byte G = 255;
 byte B = 255;
 byte A = 255;
 export |new[this&] -> void;

 export |new[this&,byte r,byte g,byte b,byte a]:
  this.R = r;
  this.G = g;
  this.B = b;
  this.A = a;

 export |new[this&,IPar<Color24> color,byte a]:
  this.R = color.R;
  this.G = color.G;
  this.B = color.B;
  this.A = a;

 export |RemoveOpacity[this&] -> Color24:
  ret [R,G,B];

 export |==[this&,IPar<this> other] -> bool:
  ret this.R == other.R && this.G == other.G && this.B == other.B && this.A == other.A;

 export |!=[this&,IPar<this> other] -> bool:
  ret !(this == other);

 export |->[imut this&] -> Color:
  ret [R -> int -> float / 255,G -> int -> float / 255,B -> int -> float / 255,A -> int -> float / 255];

