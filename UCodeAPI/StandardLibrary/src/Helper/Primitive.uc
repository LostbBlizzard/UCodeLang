
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

$for uint16:
 |ToString[imut this&] -> String:
  String r = [];
  ToString(this,r);
  ret r;
  
 |ToString[imut this&,String& Out]:UIntToString<uint16>(uint16(this),Out); 
 |Parse[imut StringSpan str] -> uint16?: ret ParseUIntToString<uint16>(str); 
 |Parse[imut String& str]: ret Parse(str.Str());

$for uint32:
 |ToString[imut this&] -> String:
  String r = [];
  ToString(this,r);
  ret r;
  
 |ToString[imut this&,String& Out]:UIntToString<uint32>(uint32(this),Out); 
 |Parse[imut StringSpan str] -> uint32?: ret ParseUIntToString<uint32>(str); 
 |Parse[imut String& str]: ret Parse(str.Str());

$for uint64:
 |ToString[imut this&] -> String:
  String r = [];
  ToString(this,r);
  ret r;
  
 |ToString[imut this&,String& Out]:UIntToString<uint64>(uint64(this),Out); 
 |Parse[imut StringSpan str] -> uint64?: ret ParseUIntToString<uint64>(str); 
 |Parse[imut String& str]: ret Parse(str.Str());

 
$for int8:
 |ToString[imut this&] -> String:
  String r = [];
  ToString(this,r);
  ret r;
  
 |ToString[imut this&,String& Out]:SIntToString<int8>(int8(this),Out); 
 |Parse[imut StringSpan str] -> int8?: ret ParseUIntToString<int8>(str); 
 |Parse[imut String& str]: ret Parse(str.Str());

$for int16:
 |ToString[imut this&] -> String:
  String r = [];
  ToString(this,r);
  ret r;
  
 |ToString[imut this&,String& Out]:SIntToString<int16>(int16(this),Out); 
 |Parse[imut StringSpan str] -> int16?: ret ParseSIntToString<int16>(str); 
 |Parse[imut String& str]: ret Parse(str.Str());

$for int32:
 |ToString[imut this&] -> String:
  String r = [];
  ToString(this,r);
  ret r;
  
 |ToString[imut this&,String& Out]: SIntToString<int32>(int32(this),Out); 
 |Parse[imut StringSpan str] -> int32?: ret ParseSIntToString<int32>(str); 
 |Parse[imut String& str]: ret Parse(str.Str());

$for int64:
 |ToString[imut this&] -> String:
  String r = [];
  ToString(this,r);
  ret r;
  
 |ToString[imut this&,String& Out]:SIntToString<int64>(int64(this),Out); 
 |Parse[imut StringSpan str] -> int64?: ret ParseSIntToString<int64>(str); 
 |Parse[imut String& str]: ret Parse(str.Str());

$for uintptr:
 |ToString[imut this&] -> String:
  String r = []; 
  ToString(this,r);
  ret r;
  
 |ToString[imut this&,String& Out]: 
   $if sizeof(uintptr) == sizeof(uint32):
    UIntToString<uint32>(uint32(this -> uint32),Out); 
   $else:
    UIntToString<uint64>(uint64(this -> uint64),Out); 

 |Parse[imut StringSpan str] -> uintptr?:ret ParseUIntToString<uintptr>(str); 
 |Parse[imut String& str]: ret Parse(str.Str());


$for sintptr:
 |ToString[imut this&] -> String:
  String r = []; 
  ToString(this,r);
  ret r;
  
 |ToString[imut this&,String& Out]: 
   $if sizeof(sintptr) == sizeof(int32):
    UIntToString<int32>(int32(this -> int32),Out); 
   $else: 
    UIntToString<int64>(int64(this -> int64),Out); 

 |Parse[imut StringSpan str] -> sintptr?:ret ParseSIntToString<sintptr>(str); 
 |Parse[imut String& str]: ret Parse(str.Str());