use ULang;


enum Item[int]:
 Red,
 Green,
 Blue,


|Func[int a]:
 ret 1 + (2 * 3);

|main[] -> var:
 ret Func(1) * 2;