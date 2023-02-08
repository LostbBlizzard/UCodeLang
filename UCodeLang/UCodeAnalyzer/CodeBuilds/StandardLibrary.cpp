#include "StandardLibrary.hpp" 
UCodeAnalyzerStart 
StringView StandardLibraryBuilder::UCodeStandardLibrary = " \
// \
// Packaged FilePath:Types/Math.uc\
// \
\
%ULang:\
 $Math:\
  |Sqrt<T>[T Value] -> T;\
\
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
  \
  |new[this&,Nothing Value]:SetValue(Value);\
  |new[this&]:SetValue(Nothing());\
  |new[this&,umut T& Value]:SetValue(Value);\
\
\
  |SetValue[umut this&,umut T& Value]:\
   _HasValue = true;\
   _Value =Value;\
\
  |SetValue[umut this&,Nothing Value]:\
   _HasValue = false;\
  \
  |HasValue[umut this&] => _HasValue;\
  |GetValue[umut this&] => _Value;\
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
 $Vec2i = Vec2i_t<int>;\
 //$Vec2 = Vec2f_t<float>;\
" ; 
 UCodeAnalyzerEnd