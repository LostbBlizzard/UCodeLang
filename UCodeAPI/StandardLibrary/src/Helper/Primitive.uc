
$for bool:
 |ToString[imut this&] -> String:
  String r = [];
  ToString(this,r);
  ret r;
  
 |ToString[imut this&,String& Out]:
  if this:
   Out += "true";
  else:
   Out += "false";
 
 |Parse[imut StringSpan str] -> bool?:
  if str == "true":
   ret Opt(true);
  else if str == "false": 
   ret Opt(false);

  ret None;
 
 |Parse[imut String& str]:
  ret Parse(str.Str());

|SIntToString<T>[T val,String& Out] -> void;
|UIntToString<T>[T val,String& Out] -> void;


|ParseSIntToString<T>[imut StringSpan str] -> T?;
|ParseUIntToString<T>[imut StringSpan str] -> T?;


$for uint8:
 |ToString[imut this&] -> String:
  String r = [];
  ToString(this,r);
  ret r;
  
 |ToString[imut this&,String& Out]:UIntToString<uint8>(uint8(this),Out); 
 |Parse[imut StringSpan str] -> uint8?: ret ParseUIntToString<uint8>(str); 
 |Parse[imut String& str]: ret Parse(str.Str());

 
