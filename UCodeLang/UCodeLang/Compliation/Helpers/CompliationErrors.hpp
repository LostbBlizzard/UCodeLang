#pragma once
#include "../../LangCore.hpp"
UCodeLangStart

using ErrorCodes_t = UInt8;
enum class ErrorCodes : ErrorCodes_t
{
	Null,
	//Tips
	TipStart = Null,



	TipEnd,
	//Warnings
	WarningsStart = TipEnd,
	
	TypeCastWarning,


	WarningsEnd,
	//Syntax
	SyntaxStart = WarningsEnd,
	//
	UnknownChar,
	ExpectingSequence,
	ExpectingToken,
	InValidName,
	//
	SyntaxEnd,
	//Middle
	LinkStart = SyntaxEnd,

	CouldNotFindFunc,
	InValidType,
	Redefinition,
	CantParseTree,
	TreeAnalyerError,
	//
	BackEndError,

	InternalCompilerError,
	
	LinkEnd,
};
class CompliationErrors
{
public:
	struct Error
	{
		ErrorCodes _Code;
		Path File;
		size_t Line;
		size_t Pos;
		String _Msg;
		
		
		Error(ErrorCodes Code) :_Code(Code), Line(0), Pos(0)
		{

		};
		Error(ErrorCodes Code, size_t Line, size_t Pos) :_Code(Code), Line(Line), Pos(Pos)
		{

		};
		String ToString()const
		{
			const char* Type;
			if (UCodeLang::CompliationErrors::IsError(_Code))
			{
				Type = "Error";
			}
			else if (UCodeLang::CompliationErrors::IsWarning(_Code))
			{
				Type = "Warning";
			}
			else
			{
				Type = "N/A";
			}
			return (String)Type + " At Line:" + std::to_string(Line) + ":" + _Msg + " In " + File.generic_string();
		}

		bool operator==(const Error& Other) const
		{
			return _Code == Other._Code && 
				Line == Other.Line && 
				Pos == Other.Pos && 
				File == Other.File && 
				_Msg == Other._Msg;
		}
		bool operator!=(const Error& Other) const
		{
			return !this->operator==(Other);
		}
	}; 
	inline static bool IsWarning(ErrorCodes Code)
		{
			return (ErrorCodes_t)Code > (ErrorCodes_t)ErrorCodes::WarningsStart &&
				   (ErrorCodes_t)Code < (ErrorCodes_t)ErrorCodes::WarningsEnd;
		}
	inline static bool IsError(ErrorCodes Code)
	{
		return (ErrorCodes_t)Code > (ErrorCodes_t)ErrorCodes::SyntaxStart &&
			(ErrorCodes_t)Code < (ErrorCodes_t)ErrorCodes::LinkEnd;
	}
	inline static bool IsInfo(ErrorCodes Code)
	{
		return false;
	}
	inline static bool IsHint(ErrorCodes Code)
	{
		return false;
	}
	inline void AddError(const ErrorCodes _Error, size_t Line,size_t Pos,const String& Msg)
	{
		Error& r = AddError(_Error, Line, Pos);
		r._Msg = Msg;
	};
	inline Error& AddError(const ErrorCodes _Error)
	{
		return AddError(_Error,-1 , -1);
	};
	inline Error& AddError(const ErrorCodes _Error, size_t Line, size_t Pos)
	{
		Error CompilerRet(_Error, Line, Pos);
		CompilerRet.File = FilePath;
		return AddError(CompilerRet);
	};
	inline Error& AddError(const Error& _Error)
	{
		_Errors.push_back(_Error);
		return _Errors.back();
	};
	inline auto& Get_Errors() { return  _Errors; };
	
	bool Has_Errors()
	{
		for (const auto& Item : _Errors)
		{
			if (IsError(Item._Code)) { return true; }
		}
		return false;
	}
	bool Has_Warning()
	{
		for (const auto& Item : _Errors)
		{
			if (IsWarning(Item._Code)) { return true; }
		}
		return false;
	}
	inline void Remove_Errors()
	{
		_Errors.clear();
	}

	void FixEndOfLine(size_t FileLineSize, size_t FilePos)
	{
		for (auto& Item : _Errors)
		{
			if (Item.Line == (size_t)-1)//End Token End of File
			{
				Item.Line = FileLineSize;
				Item.Pos = FilePos;
			}
		}
	}
	inline size_t Get_ErrorCount()
	{
		size_t r = 0;
		for (const auto& Item : _Errors)
		{
			if (IsError(Item._Code)) { r++; }
		}

		return r;
	}

	String ToString()const
	{
		String out;
		for (auto& Item : _Errors)
		{
			out += Item.ToString() + '\n';
		}
		return out;
	}

	Path FilePath = "File" + (String)FileExt::SourceFileWithDot;
private:
	Vector<Error> _Errors;
};
UCodeLangEnd

