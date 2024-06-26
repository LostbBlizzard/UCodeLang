
|func[byte v] => v * 2;
 
$for uint8 export:
 |memeberfunc[imut this&]:
  ret func(uint8(this)); 


|main[]:
 uint8 v = 10;
 ret v.memeberfunc();
