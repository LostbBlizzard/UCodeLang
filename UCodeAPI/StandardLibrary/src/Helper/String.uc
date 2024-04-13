
export |IsUpper[char val] -> bool: 
 ret (val -> byte >= 'A' -> byte) && (val -> byte <= 'Z' -> byte);
 
export |IsLower[char val] -> bool:
 ret (val -> byte >= 'a' -> byte) && (val -> byte <= 'z' -> byte);

export |ToUpper[char val] -> char:
 if IsLower(val): ret ( (val -> byte) + (('A' -> byte) - ('a' -> byte)) ) -> char;
  
 ret val;

export |ToLower[char val] -> char:
 if IsUpper(val): ret ( (val -> byte) - (('A' -> byte) - ('a' -> byte)) ) -> char;

 ret val;

 