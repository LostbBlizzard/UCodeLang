
$Fmt:
 |Print[char Str] -> void:
 
  ULang::Native::OS_PrintChar(Str);

 |Print[imut StringSpan Str] -> void:
  
  ULang::Native::OS_PrintSpan(unsafe Str.Data(),Str.Size());
 
 |Print[imut String& Str] -> void;

 |Println[char Str] -> void:
  Print(Str);
  Print('\n');

 |Println[imut StringSpan Str] -> void:
  Print(Str);
  Print('\n');

 |Println[imut String& Str] -> void:
  Print(Str);
  Print('\n');
