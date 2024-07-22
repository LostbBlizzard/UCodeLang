
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
  //TODO add Compiler hint that val is aways going to be true.

|AssertDebug[bool val,StringSpan panicmsg]:
 
 $if compiler::IsDebug():
   if !val: 
     panic("Assert Failed");
 $else:
  //TODO add Compiler hint that val is aways going to be true.

