#include "UCodeLang/LangCore/LangTypes.hpp"
#include "UCodeLang/LangCore/UCodeLangNameSpace.hpp"

UCodeLangStart

struct StackFranes {};
struct PanicCalled {

  String PanicMsg;
  Optional<StackFranes> StatckFrames;
};

struct InterpretorError {
  Variant<PanicCalled> ErrorType;
};

UCodeLangEnd
