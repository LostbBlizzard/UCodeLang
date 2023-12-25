//Build Script Module Imports 

import UCodeLang::StandardLibrary[0:0:0];
import UCodeLang::BuildSystem[0:0:0];

//Your code starts here 
use ULang;
use ULang::BuildSystem;

|build[BuildSystem& system] => system.Build();