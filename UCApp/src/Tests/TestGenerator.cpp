#include "TestGenerator.hpp"
namespace ULangTest
{


	enum NodeTypes
	{
		Funcion,
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

			switch (NodeType)
			{
			case 1:
				break;
			default:
				break;
			}
		}
	}

	void TestGenerator::MakeFuncion()
	{
		String NewFuncName = MakeNewName();

		AddTabs();
		_OutFile += "|" + NewFuncName + "[";

		int ParCount = _RNG.GetIntValue(1, 8);
		for (size_t i = 0; i < ParCount; i++)
		{

		}

		_OutFile += "]:\n";

		TabCount++;

		int StatmentCount = _RNG.GetIntValue(1, 8);
		for (size_t i = 0; i < StatmentCount; i++)
		{

		}

		TabCount--;
	}

	String TestGenerator::MakeNewName()
	{
		return String();
	}

	void TestGenerator::AddTabs()
	{
		for (size_t i = 0; i < TabCount; i++)
		{
			_OutFile += ' ';
		}
	}

	TypeInfo TestGenerator::GetAnyType()
	{
		return {};
	}

}