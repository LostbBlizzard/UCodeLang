#pragma once
#include "../Typedef.hpp"
UCodeAnalyzerStart
class CppHelper
{
public:
	
	struct SummaryTag
	{
		String text;
	};
	struct CPPExpression
	{
		String Value;
	};
	struct CPPType
	{
		enum class Mode
		{
			None,
			Ptr,
			Address
		};
		bool IsConst = false;
		Mode mode = Mode::None;
		String Value;
	};
	
	
	struct SymbolData;
	struct ClassType
	{
		struct Field
		{
			Optional<SummaryTag> Summary;
			String Name;
			CPPType Type;
			Optional<CPPExpression> Value;

			bool Exported = false;
		};
		Vector<Field> Fields;
		Vector<SymbolData> Symbols;
		bool IsTrait = false;
	};
	
	struct EnumType
	{
		struct Field
		{
			String Name;
			Optional<CPPExpression> Value;
			Optional<SummaryTag> Summary;
		};
		Optional<CPPType> _EnumBaseType;
		Vector<Field> Fields;
		
	};
	struct AliasType
	{
		String _Body;
	};
	struct ConstexprType
	{
		CPPType _Type;
		CPPExpression _Value;
	};
	struct FuncData 
	{
		struct Par
		{
			bool IsOut = false;
			CPPType Type;
			String Name;
			Optional<CPPExpression> Default;
		};
		Optional<String> MemberClassName;
		Vector<Par> Pars;
		CPPType Ret;

		Optional<size_t> OverloadNumber;
		bool IsStatic = false;
		bool IsThisConst = false;
	};
	struct SymbolData
	{
		String _NameSpace;


		Optional<SummaryTag> Summary;
		String _FullName;
		String _Name;

		//int becuase gcc is unable use ClassType in Variant as default
		Variant<int,ClassType, EnumType, ConstexprType, FuncData> _Type;
	};


	struct FuncInfo
	{
		String Ulangnamespace;
		String FuncFullName;
		String FuncName;
		Vector<String> Pars;
		String Ret;
		Optional<size_t> OverloadValue;
		Optional<String> MemberFuncClass;
		FuncData* MyData = nullptr;
	};
	
	/// <summary>
	/// Converts Enum,Classes,using,typedefs and funcions with the UCodeLangExportSymbol macro into Cpp calls and ULang types 
	/// also adds summary tag to the made ucode files.
	/// </summary>
	/// <param name="SrcCpp"></param>
	/// <param name="ULangOut"></param>
	/// <returns>if true it worked</returns>
	static bool ParseCppfileAndOutULang(const Path& SrcCpp,const Path& CppLinkFile, const Path& ULangOut);
	static bool ParseCppfileAndOutULangLink(const Path& SrcCpp, const Path& CppLinkFile, const Path& ULangOut);
	static bool ParseULangToCppStaticLink(const Path& SrcCpp, const Path& CppLinkFile, const Path& ULangOut);

	static void UpdateCppLinks(String& CppLinkText, Vector<CppHelper::SymbolData>& Symbols);
	static void OutputIRLineInfo(CppHelper::SymbolData& Item, Vector<FuncInfo>& V);
	static void ParseCppToSybs(String& FileText, Vector<CppHelper::SymbolData>& Symbols);
	static void DoOverLoadOnFunc(UCodeLang::UnorderedMap<String, size_t>& Overloads, CppHelper::SymbolData& Last, CppHelper::FuncData* Val);
	static bool ParseULangfileAndUpdateCpp(const Path& SrcLang, const Path& CppOut);
	//
	struct ParseCppState
	{
		Vector<String> Scopes;
		String ScopesAsString() const
		{
			String R;
			for (size_t i = 0; i < Scopes.size(); i++)
			{
				R += Scopes[i];
				if (i + 1 < Scopes.size())
				{
					R += "::";
				}
			}
			if (Scopes.size())
			{
				R += "::";
			}

			return R;
		}
	};
	static void DoConstexprType(size_t& i, String& FileText, SymbolData& Tep, Vector<SymbolData>& Symbols, ParseCppState& State);
	static void DoEnumType(size_t& i, String& FileText, SymbolData& Tep, Vector<SymbolData>& Symbols, ParseCppState& State);
	static void DoClassOrStruct(const String& Keywordlet, size_t& i, String& FileText, SymbolData& Tep, Vector<SymbolData>& Symbols, ParseCppState& State);
	static void DoVarableOrFunc(size_t StartIndex,const String& Keywordlet, size_t& i, String& FileText, SymbolData& Tep, Vector<SymbolData>& Symbols, ParseCppState& State);
	static bool OnDo(size_t StartIndex, const String& Keywordlet, size_t& i, String& Scope, SymbolData& Tep, Vector<SymbolData>& Symbols, ParseCppState& State);
	


	static void GetStringliteral(size_t& i, String& FileText, String& Out);
	static void GetStringScope(size_t& i, String& FileText, String& Out);
	static void GetIndentifier(size_t& i, String& FileText, String& Out);
	static void GetType(size_t& i, String& FileText, CPPType& Out);
	static void MovePass(size_t& i, String& FileText, const char& passChar);
	static void MovePassSpace(size_t& i, String& FileText);

	static void GetCPPExpression(size_t& i, String& FileText, CPPExpression& Out);
	static void GetSummaryTag(size_t& i, String& FileText, SummaryTag& Out);
	static void GetSummaryTag(size_t& i, String& FileText,Optional<SummaryTag>& Out);

	struct CppToULangState
	{
		size_t TabSize = 1;
		size_t ScopeCount = 2;

		void AddScope(String& Out) const
		{
			for (size_t i = 0; i < TabSize; i++)
			{
				Out += ' ';
			}
		}
		void AddScopesUseingScopeCount(String& Out) const
		{
			for (size_t i = 0; i < ScopeCount; i++)
			{
				AddScope(Out);
			}
		}
		String LastNameSpace;
		UCodeLang::UnorderedMap<String, String> InternalNameSpaces;
	};
	static String ToString(CppToULangState& State, const SymbolData& Syb);
	static String ToString(CppToULangState& State, const EnumType& Value, const SymbolData& Syb);

	static String ToString(CppToULangState& State, const ConstexprType& Value, const SymbolData& Syb);
	static String ToString(CppToULangState& State, const ClassType& Value, const SymbolData& Syb);
	static String ToString(CppToULangState& State, const FuncData& Value, const SymbolData& Syb,bool IsInClass = false);

	static String ToString(CppToULangState& State, const CPPType& Value);
	static String ToString(CppToULangState& State, const CPPExpression& Value);

	static void DoNameSpace(CppToULangState& State, const SymbolData& Syb, String& R);

	static String ToString(const  CppToULangState& State, const Optional<SummaryTag>& Value);
	static String ToString(const  CppToULangState& State, const SummaryTag& Value);
};
UCodeLangEnd
