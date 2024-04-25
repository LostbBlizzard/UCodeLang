
|IsDigit[char val] -> bool: 
 ret (val -> byte >= '0' -> byte) && (val -> byte <= '9' -> byte);

|IsUpper[char val] -> bool: 
 ret (val -> byte >= 'A' -> byte) && (val -> byte <= 'Z' -> byte);
 
|IsLower[char val] -> bool:
 ret (val -> byte >= 'a' -> byte) && (val -> byte <= 'z' -> byte);

|ToUpper[char val] -> char:
 if IsLower(val): ret ( (val -> byte) + (('A' -> byte) - ('a' -> byte)) ) -> char;
  
 ret val;

|ToLower[char val] -> char:
 if IsUpper(val): ret ( (val -> byte) - (('A' -> byte) - ('a' -> byte)) ) -> char;

 ret val;

 
|IsUpper[utf8 val] -> bool:panic("not implemented");
|ToUpper[utf8 val] -> utf8:panic("not implemented");

|IsLower[utf8 val] -> bool:panic("not implemented");
|ToLower[utf8 val] -> utf8:panic("not implemented");

|IsUpper[utf16 val] -> bool:panic("not implemented");
|ToUpper[utf16 val] -> utf16:panic("not implemented");

|IsLower[utf16 val] -> bool:panic("not implemented");
|ToLower[utf16 val] -> utf16:panic("not implemented");

|IsUpper[utf32 val] -> bool:panic("not implemented");
|ToUpper[utf32 val] -> utf32:panic("not implemented");

|IsLower[utf32 val] -> bool:panic("not implemented");
|ToLower[utf32 val] -> utf32:panic("not implemented");


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

