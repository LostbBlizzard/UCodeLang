

enum Number:
    Integer[int],
    Float[float],
    Named[float X],
    Complex[float X,float Y],
    Complex2[float,float],

|main[] -> void:

 Number V1 =  Number::BadName::idk;//bad 
 Number V2 = Number::Integer;//bad
 Number V3 = Number::Integer(1);//good