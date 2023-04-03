

enum Number:
    Integer[int],
    Float[float],
    Named[float X],
    Complex[float X,float Y],
    Complex2[float,float],
    None,

|main[] -> void:

 //Number V1 =  BadName::Number::Integer;//bad 
 Number V2 = Number::Integer;//bad
 Number V4 =  Number::None;
 //Number V3 = Number::Integer(1);//good