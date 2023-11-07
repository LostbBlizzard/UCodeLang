



|Reverse<T>[ StringSpan_t<T> str ]:
 uintptr start = 0;
 uintptr end = str.Size() - 1;
 
 while start < end:
    Swap(str[start],str[end]);
    end--;
    start++;


//|Reverse<T>[ String_t<T> string ] => Reverse<T>(string.Str());
 

|StartWith<T>[StringSpan_t<T> string,StringSpan_t<T> with] -> bool:
 if string.Size() < with.Size():ret false; 
 
 for [uintptr i = 0; i < with.Size(); i++]:

  if string[i] != with[i]:ret false;

  ret true;

|ToString<T,Char_t>[imut T& Object] -> String_t<Char_t>:
 ret [];

