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
	//Link
	LinkStart = SyntaxEnd,

	CouldNotFindFunc,
	InValidType,
	Redefinition,
	CantParseTree,
	TreeAnalyerError,

	LinkEnd,

};
class CompliationErrors
{
public:
	struct Error
	{
		ErrorCodes _Code;
		String File;
		size_t Line;
		size_t Pos;
		String _Msg;
		
		
		Error(ErrorCodes Code) :_Code(Code), Line(0), Pos(0)
		{

		};
		Error(ErrorCodes Code, size_t Line, size_t Pos) :_Code(Code), Line(Line), Pos(Pos)
		{

		};
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
		Error R(_Error, Line, Pos);
		R.File = FilePath;
		_Errors.push_back(R);
		return _Errors.back();
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
	String FilePath;
private:
	Vector<Error> _Errors;
};
UCodeLangEnd

