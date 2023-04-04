

enum Number:
    Integer[int],
    Float[float],
    Named[float X],
    Complex[float X,float Y],
    Complex2[float,float],
    None,

|main[]:

 |
  Number V1 = Number::None;

  Number V2 = Number::Integer(1);
 |

 if V2 == Number::Integer(Out):


 ret sizeof(Number);