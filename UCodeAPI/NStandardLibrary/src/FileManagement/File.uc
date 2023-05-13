$ULang::Native:
 $PathChar = char;
 eval PathChar PreferredPathSeparator = '/';

 $FileHandle = uintptr; 
 eval NullFileHandle = bitcast<FileHandle>(0);


 $size_t = uintptr;

 |OpenFile[PathChar[&] Path,size_t PathSize] => NullFileHandle;
 |CloseFile[FileHandle Value] -> void;