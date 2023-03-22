


$Vec2i:
 int X;
 int Y;

$Optional<T>:
 T Value;
 bool HasValue;
 |new[this&] -> void;

 |~>[this&] -> T&:ret Value;


|main[] -> var:
 Vec2i? Test = [];
 
 ret Test~>X;