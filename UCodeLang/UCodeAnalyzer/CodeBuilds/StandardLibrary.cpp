#include "StandardLibrary.hpp" 
UCodeAnalyzerStart 
StringView StandardLibraryBuilder::UCodeStandardLibrary = " \
// \
// Packaged FilePath:Types/Optional.uc\
// \
\
%ULang:\
 $Nothing;\
 \
 $Optional<T>:\
  bool _HasValue;\
  T _Value;\
  |new[this&,Nothing Value]:\
   _HasValue = false;\
  |new[this&]:\
   _HasValue = false;\
  \
\
  |HasValue[this&] => _HasValue;\
  |GetValue[this&] => _Value;\
\
// \
// Packaged FilePath:Types/Vec_Types.uc\
// \
\
%ULang:\
 $Vec2i_t<T>:\
  T X;\
  T Y;\
  |new[this&]:\
   X = 0;\
   Y = 0;\
\
 $Vec2f_t<T>:\
  T X;\
  T Y;\
  |new[this&]:\
   X = 0;\
   Y = 0;\
\
\
\
\
 //Typedefs\
 $int2 = Vec2i_t<int>;\
 $uintptr2 = Vec2i_t<uintptr>;\
" ; 
 UCodeAnalyzerEnd