

use ULang;
use API;


|main[]: 
 Fmt::Println("Hello World");


bool Value = false;
int IntValue = 0;
int V = 0;

|OnDraw[] -> void: 
 Imgui::Button("Hello");

 Imgui::ObjectField("My Boolen",Value);
 Imgui::ObjectField("My Int",IntValue);

 V-=5;


