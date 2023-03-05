

$FuncPtr2 = !|[] -> int;
$FuncPtr = |[] -> int;

|DropData[int V] -> int:
 ret V;

|main[int& V] -> void:
 FuncPtr H = DropData;
