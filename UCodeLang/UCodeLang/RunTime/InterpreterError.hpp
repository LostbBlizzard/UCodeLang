#include "UCodeLang/LangCore/LangTypes.hpp"
#include "UCodeLang/LangCore/UCodeLangNameSpace.hpp"
#include "UCodeLang/LangCore/ULangDebugInfo.hpp"
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
	struct ModuleID
	{
		String AuthorName;
		String ModuleName;

		UInt64 MajorVersion = 0;
		UInt64 MinorVersion = 0;
		UInt64 RevisionVersion = 0;
	};
	Optional<ModuleID> ModuleName;
	Optional<String> FilePath;
	Optional<size_t> CallerLineNumber;
	String FuncionName;

	String ToString() const
	{
		String r;
		

		r += "at " + FuncionName;

		if (ModuleName.has_value() || FilePath.has_value())
		{
			r += " (";
			if (ModuleName.has_value())
			{
				auto& v = ModuleName.value();

				r += "{";

				r += v.AuthorName;
				r += ":" + v.ModuleName;

				r += "[" + std::to_string(v.MajorVersion) + "." + std::to_string(v.MinorVersion) + ":" + std::to_string(v.RevisionVersion) + "]";

				r += "}";
			}

			if (FilePath.has_value())
			{
				r += FilePath.value();
			}
			if (CallerLineNumber.has_value())
			{
				r += ":";
				r += std::to_string(CallerLineNumber.value());
			}

			r += ")";
		}

		return r;
	}
};
struct GetDetilsCach
{
	Optional<ULangDebugInfo::Cach> cach;
	void Clear()
	{
		cach = {};
	}
};

class RunTimeLangState;
void GetDetils(UAddress Input, StackFrameDetails& Out, const RunTimeLangState* State,GetDetilsCach& cach);
void GetDetils(const StackFrames& Input, const RunTimeLangState* State, Vector<StackFrameDetails>& Out,GetDetilsCach& cach);
Vector<StackFrameDetails> GetDetils(const StackFrames& Input, const RunTimeLangState* State,GetDetilsCach& cach);
UCodeLangEnd
