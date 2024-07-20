
$for char export:
  export |IsUpper[this&] -> bool: ret IsUpper(char(this));
  export |ToUpper[this&] -> char: ret ToUpper(char(this));

  export |IsLower[this&] -> bool: ret IsLower(char(this));
  export |ToLower[this&] -> char: ret ToLower(char(this));
  
  export |IsDigit[this&] -> bool: ret IsDigit(char(this));


$for bool export:
 export |ToString[imut this&] -> String:
  String r = [];
  ToString(this,r);
  ret r;
  
 export |ToString[imut this&,String& Out]:
  if this:
   Out += "true";
  else:
   Out += "false";
 
 export |Parse[imut StringSpan str] -> bool?:
  if str == "true":
   ret Opt(true);
  else if str == "false": 
   ret Opt(false);

  ret None;
 
 export |Parse[imut String& str]:
  ret Parse(str.Str());

|SIntToString<T>[T val,String& Out] -> void:
 if val == 0:
  Out += "0";
  ret;

 bool isneg = val < 0;
 if isneg: val = val * -1;

 while val > 0:
  var digit = val % 10;
  Out += ( ('0' -> byte) + (digit -> byte) ) -> char;
  val = val / 10;

 if isneg: Out += '-';

 Out.Reverse();

|UIntToString<T>[T val,String& Out] -> void:
 
 if val == 0:
  Out += "0";
  ret;

 while val > 0:
  var digit = val % 10;
  Out += ( ('0' -> byte) + (digit -> byte) ) -> char;
  val = val / 10;

 Out.Reverse();

|ParseSIntToString<T>[imut StringSpan str] -> T?:
 T r = 0;
 bool isok = true;
 bool isneg = false;
    
 if str.Size() != 0: isneg = str[0] == '-';
 
 uintptr digitplace = 0;
 uintptr endindex = 0;
 if isneg: endindex = 1;
 
 for [sintptr ia = str.Size() -> sintptr - 1; ia >= (endindex -> sintptr); ia--]:
   uintptr i = ia -> uintptr;
   var c = str[i];
   if IsDigit(c):
      var digitc =( (c -> byte) - ('0' -> byte)) -> uintptr;
      var numplace = uintptr(1);
            
      for [uintptr i = 0; i < digitplace;i++]:
        numplace = numplace * 10;
      
      var oldr = r;
      r += (digitc * numplace) -> T;
      digitplace++;

      if oldr > r:
         isok = false;
         break;
        
   else:   
     isok = false;
     break;
     
 if isneg: r = r * -1;


 if isok:
  ret Opt(r);
 else: 
  ret None;


|ParseUIntToString<T>[imut StringSpan str] -> T?:
 T r = 0;
 bool isok = true;   
 
 uintptr digitplace = 0;
 uintptr endindex = 0;
 
 for [sintptr ia = str.Size() -> sintptr - 1; ia >= 0; ia--]:
   uintptr i = ia -> uintptr;
   var c = str[i];
   if IsDigit(c):
      var digitc =( (c -> byte) - ('0' -> byte)) -> uintptr;
      var numplace = uintptr(1);
            
      for [uintptr i = 0; i < digitplace;i++]:
        numplace = numplace * 10;
      
      var oldr = r;
      r += (digitc * numplace) -> T;
      digitplace++;

      if oldr > r:
         isok = false;
         break;
        
   else:   
     isok = false;
     break;

 if isok:
  ret Opt(r);
 else: 
  ret None;


$for uint8 export:
 export |ToString[imut this&] -> String:
  String r = [];
  ToString(this,r);
  ret r;
  
 export |ToString[imut this&,String& Out]:UIntToString<uint8>(uint8(this),Out); 
 export |Parse[imut StringSpan str] -> uint8?: ret ParseUIntToString<uint8>(str); 
 export |Parse[imut String& str]: ret Parse(str.Str());
 export |MaxValue[] -> uint8: ret 255;
 export |MinValue[] -> uint8: ret 0;

$for uint16 export:
 export |ToString[imut this&] -> String:
  String r = [];
  ToString(this,r);
  ret r;
  
 export |ToString[imut this&,String& Out]:UIntToString<uint16>(uint16(this),Out); 
 export |Parse[imut StringSpan str] -> uint16?: ret ParseUIntToString<uint16>(str); 
 export |Parse[imut String& str]: ret Parse(str.Str());
 export |MaxValue[] -> uint16: ret 65535;
 export |MinValue[] -> uint16: ret 0;

$for uint32 export:
 export |ToString[imut this&] -> String:
  String r = [];
  ToString(this,r);
  ret r;
  
 export |ToString[imut this&,String& Out]:UIntToString<uint32>(uint32(this),Out); 
 export |Parse[imut StringSpan str] -> uint32?: ret ParseUIntToString<uint32>(str); 
 export |Parse[imut String& str]: ret Parse(str.Str());
 export |MaxValue[] -> uint32: ret 4294967295;
 export |MinValue[] -> uint32: ret 0;

$for uint64 export:
 export |ToString[imut this&] -> String:
  String r = [];
  ToString(this,r);
  ret r;
  
 export |ToString[imut this&,String& Out]:UIntToString<uint64>(uint64(this),Out); 
 export |Parse[imut StringSpan str] -> uint64?: ret ParseUIntToString<uint64>(str); 
 export |Parse[imut String& str]: ret Parse(str.Str());
 export |MaxValue[] -> uint64: ret 18446744073709551615;
 export |MinValue[] -> uint64: ret 0;
 
$for int8 export:
 export |ToString[imut this&] -> String:
  String r = [];
  ToString(this,r);
  ret r;
  
 export |ToString[imut this&,String& Out]:SIntToString<int8>(int8(this),Out); 
 export |Parse[imut StringSpan str] -> int8?: ret ParseUIntToString<int8>(str); 
 export |Parse[imut String& str]: ret Parse(str.Str());
 export |MaxValue[] -> int8: ret 127;
 export |MinValue[] -> int8: ret -128;

$for int16 export:
 export |ToString[imut this&] -> String:
  String r = [];
  ToString(this,r);
  ret r;
  
 export |ToString[imut this&,String& Out]:SIntToString<int16>(int16(this),Out); 
 export |Parse[imut StringSpan str] -> int16?: ret ParseSIntToString<int16>(str); 
 export |Parse[imut String& str]: ret Parse(str.Str());
 export |MaxValue[] -> int16: ret 32767;
 export |MinValue[] -> int16: ret -32768;

$for int32 export:
 export |ToString[imut this&] -> String:
  String r = [];
  ToString(this,r);
  ret r;
  
 export |ToString[imut this&,String& Out]: SIntToString<int32>(int32(this),Out); 
 export |Parse[imut StringSpan str] -> int32?: ret ParseSIntToString<int32>(str); 
 export |Parse[imut String& str]: ret Parse(str.Str());
 export |MaxValue[] -> int32: ret 2147483647;
 export |MinValue[] -> int32: ret -2147483648;

$for int64 export:
 export |ToString[imut this&] -> String:
  String r = [];
  ToString(this,r);
  ret r;
  
 export |ToString[imut this&,String& Out]:SIntToString<int64>(int64(this),Out); 
 export |Parse[imut StringSpan str] -> int64?: ret ParseSIntToString<int64>(str); 
 export |Parse[imut String& str]: ret Parse(str.Str());
 export |MaxValue[] -> int64: ret 9223372036854775807;
 export |MinValue[] -> int64: ret -9223372036854775808;

$for uintptr export:
 export |ToString[imut this&] -> String:
  String r = []; 
  ToString(this,r);
  ret r;
  
 export |ToString[imut this&,String& Out]: 
   $if sizeof(uintptr) == sizeof(uint32):
    UIntToString<uint32>(uint32(this -> uint32),Out); 
   $else:
    UIntToString<uint64>(uint64(this -> uint64),Out); 

 export |Parse[imut StringSpan str] -> uintptr?:ret ParseUIntToString<uintptr>(str); 
 export |Parse[imut String& str]: ret Parse(str.Str());

 export |MaxValue[] -> uintptr:
   $if sizeof(uintptr) == sizeof(uint32):
    ret 4294967295;
   $else:
    ret 18446744073709551615;
 export |MinValue[] -> uintptr: ret 0;

$for sintptr export:
 export |ToString[imut this&] -> String:
  String r = []; 
  ToString(this,r);
  ret r;
  
 export |ToString[imut this&,String& Out]: 
   $if sizeof(sintptr) == sizeof(int32):
    UIntToString<int32>(int32(this -> int32),Out); 
   $else: 
    UIntToString<int64>(int64(this -> int64),Out); 

 export |Parse[imut StringSpan str] -> sintptr?:ret ParseSIntToString<sintptr>(str); 
 export |Parse[imut String& str]: ret Parse(str.Str());

 export |MaxValue[] -> sintptr:
   $if sizeof(sintptr) == sizeof(int32):
    ret 2147483647;
   $else:
    ret 9223372036854775807;
 export |MinValue[] -> sintptr:
   $if sizeof(sintptr) == sizeof(int32):
    ret -2147483648;
   $else:
    ret -9223372036854775808;
