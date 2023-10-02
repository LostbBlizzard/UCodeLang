$Color enum:
 Red[bool],
 Green,
 Blue[int32],


|main[]:
 Color X = Color::Red(5);
 Color Y = Color::Blue(true);

 if Color::Blue(Y,out BoolValue) && Color::Red(X,out int IntValue):
  if BoolValue == false:
   ret 0;
  else:
   ret 6 - IntValue;


 ret 0;

