
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

 
export |IsUpper[utf8 val] -> bool:panic("not implemented");
export |ToUpper[utf8 val] -> utf8:panic("not implemented");

export |IsLower[utf8 val] -> bool:panic("not implemented");
export |ToLower[utf8 val] -> utf8:panic("not implemented");

export |IsUpper[utf16 val] -> bool:panic("not implemented");
export |ToUpper[utf16 val] -> utf16:panic("not implemented");

export |IsLower[utf16 val] -> bool:panic("not implemented");
export |ToLower[utf16 val] -> utf16:panic("not implemented");

export |IsUpper[utf32 val] -> bool:panic("not implemented");
export |ToUpper[utf32 val] -> utf32:panic("not implemented");

export |IsLower[utf32 val] -> bool:panic("not implemented");
export |ToLower[utf32 val] -> utf32:panic("not implemented");


|Str_StartWith<T>[imut StringSpan_t<T> base,imut StringSpan_t<T> Match] -> bool:
 if Match.Size() > base.Size(): ret false;

 for [uintptr i = 0;i < Match.Size();i++]:
  if base[i] != Match[i]:
   ret false;

 ret true;

|Str_EndWith<T>[imut StringSpan_t<T> base,imut StringSpan_t<T> Match] -> bool: 
 if Match.Size() > base.Size(): ret false;

 for [uintptr i = 0;i < Match.Size();i++]:
  if base[base.Size() - uintptr(1) - i] != Match[Match.Size() - uintptr(1) - i]:
   ret false;

 ret true;

|Str_Contains<T>[imut StringSpan_t<T> base,imut StringSpan_t<T> Match] -> bool: 
 if Match.Size() > base.Size(): ret false;

 uintptr samecount = 0; 
 for [uintptr i = 0;i < base.Size();i++]:
  
  if base[i] == Match[samecount]:
   samecount++;

   if (samecount == Match.Size()):
     ret true;
  else:
     samecount = 0;
 
 ret false;

