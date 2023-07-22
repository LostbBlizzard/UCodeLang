#pragma once
#include "../Typedef.hpp"
UCodeAnalyzerStart
class CppHelper
{
public:
	
	struct SummaryTag
	{

	};
	struct CPPExpression
	{
		String Value;
	};
	struct Type
	{
		String Value;
	};
	
	
	struct SymbolData;
	struct ClassType
	{
		struct Field
		{
			Optional<SummaryTag> Summary;
			String Name;
			Type Type;
			Optional<CPPExpression> Value;

			bool Exported = false;
		};
		Vector<Field> Fields;
		Vector<SymbolData> Symbols;
	};
	
	struct EnumType
	{
		struct Field
		{
			String Name;
			Optional<CPPExpression> Value;
			Optional<SummaryTag> Summary;
		};
		Optional<Type> _EnumBaseType;
		Vector<Field> Fields;
		
	};
	struct AliasType
	{
		String _Body;
	};
	struct ConstexprType
	{
		Type _Type;
		CPPExpression _Value;
	};
	struct FuncData 
	{
		struct Par
		{
			bool IsOut = false;
			Type Type;
			String Name;
			Optional<CPPExpression> Default;
		};
		bool IsStatic = true;
		Vector<Par> Pars;
		Type Ret;

		Optional<size_t> OverloadNumber;
	};
	struct SymbolData
	{
		String _NameSpace;


		Optional<SummaryTag> Summary;
		String _Name;
		Variant<ClassType, EnumType, ConstexprType, FuncData> _Type;
	};

	
	
	/// <summary>
	/// Converts Enum,Classes,using,typedefs and funcions with the UCodeLangExportSymbol macro into Cpp calls and ULang types 
	/// also adds summary tag to the made ucode files.
	/// </summary>
	/// <param name="SrcCpp"></param>
	/// <param name="ULangOut"></param>
	/// <returns>if true it worked</returns>
	static bool ParseCppfileAndOutULang(const Path& SrcCpp,const Path& CppLinkFile, const Path& ULangOut);
	static void UpdateCppLinks(UCodeAnalyzer::String& CppLinkText, UCodeAnalyzer::Vector<UCodeAnalyzer::CppHelper::SymbolData>& Symbols);
	static void ParseCppToSybs(UCodeAnalyzer::String& FileText, UCodeAnalyzer::Vector<UCodeAnalyzer::CppHelper::SymbolData>& Symbols);
	static bool ParseULangfileAndUpdateCpp(const Path& SrcLang, const Path& CppOut);

	//
	static void DoConstexprType(size_t& i, String& FileText, SymbolData& Tep, Vector<SymbolData>& Symbols);
	static void DoEnumType(size_t& i, String& FileText, SymbolData& Tep, Vector<SymbolData>& Symbols);
	static void DoClassOrStruct(const String& Keywordlet, size_t& i, String& FileText, SymbolData& Tep, Vector<SymbolData>& Symbols);
	static void DoVarableOrFunc(size_t StartIndex,const String& Keywordlet, size_t& i, String& FileText, SymbolData& Tep, Vector<SymbolData>& Symbols);
	static bool OnDo(size_t StartIndex, const String& Keywordlet, size_t& i, String& Scope, SymbolData& Tep, Vector<SymbolData>& Symbols);
	


	static void GetStringliteral(size_t& i, String& FileText, String& Out);
	static void GetStringScope(size_t& i, String& FileText, String& Out);
	static void GetIndentifier(size_t& i, String& FileText, String& Out);
	static void GetType(size_t& i, String& FileText, Type& Out);
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
		UCodeLang::VectorMap<String, String> InternalNameSpaces;
	};
	static String ToString(CppToULangState& State, const SymbolData& Syb);
	static String ToString(CppToULangState& State, const EnumType& Value, const SymbolData& Syb);

	static String ToString(CppToULangState& State, const ConstexprType& Value, const SymbolData& Syb);
	static String ToString(CppToULangState& State, const ClassType& Value, const SymbolData& Syb);
	static String ToString(CppToULangState& State, const FuncData& Value, const SymbolData& Syb,bool IsInClass = false);

	static String ToString(CppToULangState& State, const Type& Value);
	static String ToString(CppToULangState& State, const CPPExpression& Value);

	static void DoNameSpace(CppToULangState& State, const SymbolData& Syb, String& R);

	static String ToString(const  CppToULangState& State, const Optional<SummaryTag>& Value);
	static String ToString(const  CppToULangState& State, const SummaryTag& Value);
};
UCodeLangEnd
