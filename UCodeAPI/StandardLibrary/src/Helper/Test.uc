
$Test tag export;

$SkipedTest tag export;


export  |Assert[bool val]:
 if !val: 
  panic("Assert Failed");

export |Assert[bool val,imut StringSpan panicmsg]:
 if !val: 
  panic(panicmsg);


export |AssertDebug[bool val]:
 
 $if compiler::IsDebug():
   if !val: 
     panic("Assert Failed");
 $else:
  compiler::Assume(val);

export |AssertDebug[bool val,imut StringSpan panicmsg]:
 
 $if compiler::IsDebug():
   if !val: 
     panic("Assert Failed");
 $else:
  compiler::Assume(val);

