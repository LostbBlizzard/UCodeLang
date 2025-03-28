
$Fmt export:
 export |Print[char Str] -> void:
 
  ULang::Native::OS_PrintChar(Str);

 export |Print[imut StringSpan Str] -> void:
  
  ULang::Native::OS_PrintSpan(unsafe Str.Data(),Str.Size());
 
 export |Print[imut String& Str] -> void:
  Print(Str.Str());

 export |Println[char Str] -> void:
  Print(Str);
  Print('\n');

 export |Println[imut StringSpan Str] -> void:
  Print(Str);
  Print('\n');

 export |Println[imut String& Str] -> void:
  Print(Str);
  Print('\n');
