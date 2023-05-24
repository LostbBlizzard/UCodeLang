%ULang:
 $Console:
  |Log<T...>[T Pars]:
   $for [var& Item : Pars]:
    var Str = StringConverter::ToString();
    Write(Str.AsSpan());


  
  |Write[umut String& Str] => Write(Str.AsSpan());

  |Write[umut StringView Str]:
   ULang::Native::ConsoleOutSpan(Name.Data(),Str.Size());
  
  |Write[char Value] -> void:
   ULang::Native::ConsoleOutChar(Value);


  |WriteLine[umut String& Str] => WriteLine(Str.AsSpan());

  |WriteLine[umut StringView Str]:
   Write(Str);
   Write('\n');


  |Read[] -> char:
   ret ULang::Native::ConsoleInReadChar();
  |ReadLine[String& Out] -> void:
   ULang::Native::
  
  |ReadLine[] -> String:
   String R = [];
   ReadLine(R);
   ret R;
  
  |Clear[] -> void:
   ULang::Native::ConsoleOutClear();