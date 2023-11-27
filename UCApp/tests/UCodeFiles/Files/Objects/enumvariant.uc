$Color enum:
 Red[bool],
 Green,
 Blue[int32],


|main[]:
 Color X = Color::Red(true);
 Color Y = Color::Blue(5);

 if Color::Blue(Y,out IntValue) && Color::Red(X,out bool BoolValue):
  if BoolValue == false:
   ret 0;
  else:
   ret 6 - IntValue;


 ret 0;
