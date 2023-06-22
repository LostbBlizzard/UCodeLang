$Item:
 char& V;
 |new[this&]:
  V = new char('A');

|main[]:
 Item item = [];

 item.V = '\0';
 char f = item.V;

 drop(item.V);


 ret f;