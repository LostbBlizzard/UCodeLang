

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

|main[] => Color(0.1,0.2,0.3,0.4);

