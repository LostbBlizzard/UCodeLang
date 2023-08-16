#pragma once

#include <UCodeLang/LangCore/LangTypes.hpp>


#include "Test.hpp"
UCodeTestStart
	
	//Fuzz testing 
	using RNGValue = UInt64;
constexpr RNGValue MaxRNGValue = UINT64_MAX;
	struct RNG
	{
	public:
		RNGValue SeedValue=1;

		void SetSeed(RNGValue Seed)
		{
			if (Seed == 0)
			{
				Seed = 1;
			}
			SeedValue = Seed;

		}
		template<typename T> T GetIntValue(const T min, const T max)
		{
			T r = SeedValue % max + min;
			UpdateState();

			return r;
		}
		template<typename T> T GetFloatValue(const T min, const T max)
		{
			T r = min + static_cast<T>(SeedValue) / (static_cast<T>(MaxRNGValue / (max - min)));
			UpdateState();
			return r;
		}

		template<typename T> T GetIntEnumValue(const T min, const T max)
		{
			switch (sizeof(T))
			{
			case 1: 
			{
				auto V = GetIntValue(*(UInt8*)&min, *(UInt8*)&max);
				return *(T*)&V;
			}
			case 2:
			{
			 auto V1 = GetIntValue(*(UInt16*)&min, *(UInt16*)&max);
			 return *(T*)&V1;
			}
			case 4:
			{	
				auto V2 = GetIntValue(*(UInt32*)&min, *(UInt32*)&max);
			    return *(T*)&V2;
			}
			case 8:
			{
				auto V3 = GetIntValue(*(UInt64*)&min, *(UInt64*)&max);
				return *(T*)&V3;
			}
			default:
				UCodeLangThrowException("bad path");
				break;
			}
		}
		bool GetBool()
		{
			return GetIntValue<int>(0, 1) == 0;
		}
	private:
		

		//https://en.wikipedia.org/wiki/Xorshift
		void UpdateState()
		{
			uint64_t x = SeedValue;
			x ^= x << 13;
			x ^= x >> 7;
			x ^= x << 17;
			SeedValue = x;
		}
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
	public:
		TestGenerator() {}
		~TestGenerator() {}

		void SetSeed(RNGValue Seed)
		{
			_RNG.SetSeed(Seed);
		}
		void MakeFile();
		void Reset();

		String& Get_OutFile()
		{
			return _OutFile;
		}
	private:
		RNG _RNG;
		String _OutFile;
		size_t TabCount = 0;
		void MakeFuncion();
		void BuildFuncStatments();
		void MakeMain();
		String MakeNewName();
		void AddTabs();
		void AddTabsToFunc();
		TypeInfo GetAnyType();
		size_t NameCounter = 0;
		bool MadeMainFunc = false;

		enum class TypeDataEnum
		{
			Void,

			UInt8,
			UInt16,
			UInt32,
			UInt64,

			SInt8,
			SInt16,
			SInt32,
			SInt64,

			Bool,
			Char,

			Named,

			Start = Void,
			End =Char,
		};
		struct TypeData
		{
			TypeDataEnum Enum;
			String FullName;
		};
		struct NameWithType
		{
			TypeData Type;
			String Name;
		};
		struct FuncData
		{
			String FuncName;
			Vector<NameWithType> Pars;
			TypeData Ret;


			String FuncString;
		};
		Vector < Unique_ptr<FuncData>> Funcs;
		FuncData* CurrentFunc;

		TypeDataEnum GetNewTypePrimitive()
		{
			return _RNG.GetIntEnumValue(TypeDataEnum::Start, TypeDataEnum::End);
		}
		TypeDataEnum GetNewTypePrimitiveNonVoid()
		{
			return _RNG.GetIntEnumValue(TypeDataEnum::Void, TypeDataEnum::End);
		}
		TypeData GetNewType();
		TypeData GetNewNonVoidType();
		String ToString(const TypeData& Value);
	};
			
	UCodeTestEnd