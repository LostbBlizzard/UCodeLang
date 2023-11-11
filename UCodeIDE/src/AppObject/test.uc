

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



|main1[int V] => V * 4;
|main2[int V] => V * 2;
|main3[int V] => V * 1;
|main4[int V] => V * 0;

|main5[int V] => V * V;