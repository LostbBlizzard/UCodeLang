%ULang:
 
 $CompilationMode:
  Debug,
  Release,
  Publish,

 $PlatformType:
  UCodeVM,
  Windows,
  Linux,
  Android,
  Apple,
  Web,
 
 

 $Compiler:
  eval CompilationMode = CompilationMode::Debug;
  eval PlatformType = PlatformType::UCodeVM;
  

  eval bool IsDebugBuild = true;