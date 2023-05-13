%ULang:
 $File:
  private:
   FileHandle Handle;
  public:

   |new[this&]:
    Handle = ULang::Native::NullFileHandle;

   |drop[this&]:
    if IsOpen():
     CloseFile(); 
  
   |OpenFile[this&,umut Path& path] => OpenFile(path.AsView());

   |OpenFile[this&,umut PathChar[:] path] -> void;

  
   |CloseFile[this&] -> void;

   |IsOpen[this&] -> bool;