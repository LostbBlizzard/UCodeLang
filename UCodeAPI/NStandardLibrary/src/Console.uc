ULang::Native:
 |ConsoleOutChar[char Value] -> void;
 |ConsoleOutSpan[umut char[&] Buffer,uintptr Size] -> void;
 |ConsoleOutClear[] -> void;

 |ConsoleInReadChar[] -> void;
 |ConsoleInReadLine[uintptr& Size] -> byte[&];


$TypeClass;

$TestEnum enum:
 Red,
 Green[TypeClass V],
 Blue[int a,int b],


|test[] => TestEnum::Red;