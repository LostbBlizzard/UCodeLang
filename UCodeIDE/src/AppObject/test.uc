

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


|main[]:
 
 while true:
  int a = 0;


//|main1[] => unsafe new int[5];