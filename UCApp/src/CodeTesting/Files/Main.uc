

enum Number:
    Integer[int],
    Float[float],
    Named[float X],
    Complex[float X,float Y],
    Complex2[float,float],
    None,

|main[] -> void:


 Number V1 =  Number::None;
 Number V2 = Number::Integer(1);
 Number V3 = Number::Complex(1.0,1.0);