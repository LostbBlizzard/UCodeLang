

use ULang;
use API;


|main[]: 
 Fmt::Println("Hello World");

//$Vector<T> = int;
//byte[] List = [];
int V = 5;


|OnDraw[] -> void: 
 if Imgui::Button("Say Hello"):
  Fmt::Println("Hello World");
  V++;
