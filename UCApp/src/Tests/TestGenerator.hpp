#pragma once

#include <UCodeLang/LangCore/LangTypes.hpp>
namespace ULangTest
{
	using namespace UCodeLang;

	using RNGValue = size_t;
	struct RNG
	{
	public:
		RNGValue SeedValue;

		void SetSeed(RNGValue Seed)
		{
			SeedValue = Seed;
		}
		template<typename T> T GetIntValue(const T min, const T max)
		{
			return {};
		}
		template<typename T> T GetIntEnumValue(const T min, const T max)
		{
			return {};
		}
		bool GetBool()
		{
			return GetIntValue<int>(1, 2) == 1;
		}
	private:
	};

	using Mode_t = int;
	enum class Mode :Mode_t
	{
		New,
		EvalVarable,
		StaticVarable,
		ThreadVarable,
		Funcion,

		Max,
	};
	struct TestInput
	{
		bool Mode[(Mode_t)Mode::Max];
	};
	struct Sybol
	{

	};
	struct TypeInfo
	{
		String TypeFullName;
	};

	class TestGenerator
	{
		TestGenerator() {}
		~TestGenerator() {}

		void SetSeed(RNGValue Seed)
		{
			_RNG.SetSeed(Seed);
		}
		void MakeFile();

	private:
		RNG _RNG;
		String _OutFile;
		size_t TabCount =0 ;
		void MakeFuncion();
		String MakeNewName();
		void AddTabs();
		TypeInfo GetAnyType();
	};
}