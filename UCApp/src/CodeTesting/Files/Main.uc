



eval Item = false;

|DoSomeThing[] -> void;

|main[]:
 
 for [var I : 0 => 10]:
  DoSomeThing();

 
 //for [var A : 0 <= 10];
 //for [var A = 10;A > (0-1);A--];
 //for [var A = 10;(A-1) != 0;A--];

 
 for [var A : 0 ,<= 10]:
  DoSomeThing();
 

 ret 1;
