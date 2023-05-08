

$Vec2:
 int X;
 int Y;



|CoolFunc4<T...>[int B,bool X,T ParList] -> void;

|CoolFunc3<X,Y,T...>[X B,Y X,T ParList] -> void;

|CoolFunc2<X,T...>[X B,T ParList] -> void;

|CoolFunc<T...>[int B,T ParList]-> void;


|main[]:

 //Have 1 Generics

 //CoolFunc4<int,bool,char>(0,true,'H');
 //CoolFunc4(0,true,'H');
 //CoolFunc4<int,bool>(0,true,'H');

 //Have 3 Generics

 //CoolFunc3<int,bool,char>(0,true,'H');
 //CoolFunc3(0,true,'H');
 //CoolFunc3<int,bool>(0,true,'H');

 //Have 3 Generics

 //CoolFunc2<int,bool,char>(0,true,'H');
 //CoolFunc2(0,true,'H');
 //CoolFunc2<int,bool>(0,true,'H');


 //Have 2 Generics

 //CoolFunc<bool,char>(0,true,'H');
 //CoolFunc(0,true,'H');
 //CoolFunc<bool>(0,true,'H');


