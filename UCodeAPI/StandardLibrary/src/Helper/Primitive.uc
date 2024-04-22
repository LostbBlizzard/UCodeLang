
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
