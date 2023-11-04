#include "TestGenerator.hpp"
namespace ULangTest
{


	enum NodeTypes
	{
		Function,
		EvalVarable,
		StaticVarable,
		ThreadVarable,
	};

	void TestGenerator::MakeFile()
	{
		int Nodes = _RNG.GetIntValue(2, 10);
		for (size_t i = 0; i < Nodes; i++)
		{
			int NodeType = _RNG.GetIntValue(1,4);

			bool MakeMainV = MadeMainFunc == false ? _RNG.GetIntValue<size_t>(0, (Nodes - i)) == 0 : false;

			if (MakeMainV)
			{
				MadeMainFunc = true;
				MakeMain();
			}
			else
			{
				MakeFunction();
			}

		}
		//make funcs



		{
			for (auto& Item : Funcs)
			{
				_OutFile += Item->FuncString;
			}
		}
	}

	void TestGenerator::Reset()
	{
		this->~TestGenerator();
		new (this) TestGenerator;

	}

	void TestGenerator::MakeFunction()
	{
		
		{
			FuncData* Func =new FuncData();
			Funcs.push_back(Unique_ptr<FuncData>(Func));

			CurrentFunc = Func;
		}

		String NewFuncName = MakeNewName();
		AddTabs();

		CurrentFunc->FuncName = NewFuncName;

		CurrentFunc->FuncString += "|" + NewFuncName + "[";

		int ParCount = _RNG.GetIntValue(1, 4);
		for (size_t i = 0; i < ParCount; i++)
		{
			NameWithType R;
			R.Type = GetNewType();
			R.Name = MakeNewName();

			CurrentFunc->FuncString += ToString(R.Type);
			CurrentFunc->FuncString += " " + R.Name;

			CurrentFunc->Pars.push_back(std::move(R));

			if (i + 1 < ParCount)
			{
				CurrentFunc->FuncString += ",";
			}
		}

		CurrentFunc->FuncString += "]:\n";

		TabCount++;

		BuildFuncStatments();

		TabCount--;
	}

	void TestGenerator::BuildFuncStatments()
	{
		int StatmentCount = _RNG.GetIntValue(1, 8);
		for (size_t i = 0; i < StatmentCount; i++)
		{

		}
	}

	void TestGenerator::MakeMain()
	{
		{
			FuncData* Func = new FuncData();
			Funcs.push_back(Unique_ptr<FuncData>(Func));

			CurrentFunc = Func;
		}


		AddTabs();
		CurrentFunc->FuncString += "|" + (String)"main" + "[";
		CurrentFunc->FuncString += "]:\n";

		TabCount++;

		BuildFuncStatments();

		TabCount--;
	}

	String TestGenerator::MakeNewName()
	{
		NameCounter++;
		return "Name_" + std::to_string(NameCounter);
	}

	void TestGenerator::AddTabs()
	{
		for (size_t i = 0; i < TabCount; i++)
		{
			_OutFile += ' ';
		}
	}
	void TestGenerator::AddTabsToFunc()
	{
		for (size_t i = 0; i < TabCount; i++)
		{
			CurrentFunc->FuncString += ' ';
		}
	}

	TypeInfo TestGenerator::GetAnyType()
	{
		return {};
	}

	TestGenerator::TypeData TestGenerator::GetNewType()
	{
		TypeData R;
		R.Enum = GetNewTypePrimitive();

		return R;
	}

	TestGenerator::TypeData TestGenerator::GetNewNonVoidType()
	{
		TypeData R;
		R.Enum = GetNewTypePrimitiveNonVoid();

		return R;
	}

	String TestGenerator::ToString(const TypeData& Value)
	{
		switch (Value.Enum)
		{
		case TypeDataEnum::Void:"void";
		case TypeDataEnum::SInt8:return "sint8";
		case TypeDataEnum::SInt16:return "sint16";
		case TypeDataEnum::SInt32:return "sint32";
		case TypeDataEnum::SInt64:return "sint64";


		case TypeDataEnum::UInt8:return "uint8";
		case TypeDataEnum::UInt16:return "uint16";
		case TypeDataEnum::UInt32:return "uint32";
		case TypeDataEnum::UInt64:return "uint64";

		case TypeDataEnum::Char:return "char";
		case TypeDataEnum::Bool:return "bool";
		case TypeDataEnum::Named:return Value.FullName;
		default:
			UCodeLangUnreachable();
			break;
		}
	}

}