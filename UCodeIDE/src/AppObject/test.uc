

/*

$Player[Component]:
 float Speed = 1;
 |Start[this&]:
  entity().position2d() = [2,5];



 |Update[this&] -> void:
  
  var& pos = entity().position2d();//get rereference to entity position.
  float deltatime = Time::DeltaTime();
  
  //pos.Y += Speed * deltatime;

  
  if Imgui::KeyDown(ImKey::W):
    pos.Y += Speed * deltatime;
    
  if Imgui::KeyDown(ImKey::S):
    pos.Y -= Speed * deltatime;
  
  if Imgui::KeyDown(ImKey::A):
    pos.X -= Speed * deltatime;
  
  if Imgui::KeyDown(ImKey::D):
    pos.X += Speed * deltatime;
*/



$IPar<T> = imut T&;

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

$Color32:
 byte R = 255;
 byte G = 255;
 byte B = 255;
 byte A = 255;
 |new[this&] -> void;

 |new[this&,byte r,byte g,byte b,byte a]:
  this.R = r;
  this.G = g;
  this.B = b;
  this.A = a;

 |new[this&,IPar<Color24> color,byte a]:
  this.R = color.R;
  this.G = color.G;
  this.B = color.B;
  this.A = a;

 |WithOutOpacity[this&] -> Color24:
  ret [R,G,B];

 |==[this&,IPar<this> other] -> bool:
  ret this.R == other.R && this.G == other.G && this.B == other.B && this.A == other.A;

 |!=[this&,IPar<this> other] -> bool:
  ret !(this == other);

 |->[imut this&] -> Color:
  ret [R -> int -> float / 255,G -> int -> float / 255,B -> int -> float / 255,A -> int -> float / 255];



$Color24:
 byte R = 255;
 byte G = 255;
 byte B = 255;
 |new[this&] -> void;

 |new[this&,byte r,byte g,byte b]:
  this.R = r;
  this.G = g;
  this.B = b;

 |==[this&,IPar<this> other] -> bool:
  ret this.R == other.R && this.G == other.G && this.B == other.B;

 |!=[this&,IPar<this> other] -> bool:
  ret !(this == other);

 |->[imut this&] -> ColorRGB:
  ret [R -> int -> float / 255,G -> int -> float / 255,B -> int -> float / 255];


//[Test]
|Color2[]:
 Color32 white = [];
 Color r = white -> Color;
 ret r == Color();




|main[byte R] =>  float(R -> uint32 -> float) / 255.0;

