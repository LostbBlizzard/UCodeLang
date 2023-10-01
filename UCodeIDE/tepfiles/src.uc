

use ULang;
use API;


|main[]: 
 Fmt::Println("Hello World");


bool Value = false;
int IntValue = 0;
int V = 0;



|OnDraw[] -> void: 
 Imgui::Button("Say Hello");
 
 Imgui::ObjectField("Val 1",IntValue);
 Imgui::ObjectField("Val 2",Value);
 V++;
 //Fmt::Println("Hello World");
 //V++;

