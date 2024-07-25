
$Test tag export;

$SkipedTest tag export;


|Assert[bool val]:
 if !val: 
  panic("Assert Failed");

|Assert[bool val,StringSpan panicmsg]:
 if !val: 
  panic(panicmsg);


|AssertDebug[bool val]:
 
 $if compiler::IsDebug():
   if !val: 
     panic("Assert Failed");
 $else:
  compiler::Assume(val);

|AssertDebug[bool val,StringSpan panicmsg]:
 
 $if compiler::IsDebug():
   if !val: 
     panic("Assert Failed");
 $else:
  compiler::Assume(val);

