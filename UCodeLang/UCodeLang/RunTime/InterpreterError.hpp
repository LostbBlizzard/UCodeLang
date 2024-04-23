#include "UCodeLang/LangCore/LangTypes.hpp"
#include "UCodeLang/LangCore/UCodeLangNameSpace.hpp"

UCodeLangStart

struct StackFrames 
{
	Vector<UAddress> StackOfCallers;
};
struct PanicCalled 
{
  String PanicMsg;
};

struct InterpretorError 
{
  Variant<PanicCalled> ErrorType;
  StackFrames StatckFrames;
};

struct StackFrameDetails
{
	Optional<String> ModuleName;
	Optional<String> FileNumber;
	Optional<size_t> CallerLineNumber;
	String FuncionName;
};

class RunTimeLangState;
void GetDetils(UAddress Input, StackFrameDetails& Out, const RunTimeLangState* State);
void GetDetils(const StackFrames& Input, const RunTimeLangState* State, Vector<StackFrameDetails>& Out);
Vector<StackFrameDetails> GetDetils(const StackFrames& Input, const RunTimeLangState* State);
UCodeLangEnd
