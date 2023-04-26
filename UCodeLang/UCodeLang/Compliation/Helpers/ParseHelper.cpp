#include "ParseHelper.hpp"

UCodeLangStart 

size_t ParseHelper::ParseStringliteralToString(String_view string, String& out)
{
	for (size_t i = 0; i < string.size();)
	{
		String_view V = String_view(string.data() + i, string.size() - i);

		size_t R = ParseCharliteralToChar(V, out);
		if (R==0)
		{
			return i;
		}
		i += R;
	}
	return 0;
}
size_t ParseHelper::ParseCharliteralToChar(String_view string, String& out)
{
	if (string.size() == 0) { return 0; }

	char Char1 = string.front();
	if (Char1 == '\\')
	{
		if (string.size() >= 1)
		{
			char Char2 = string[1];

			struct MyStruct
			{
				char Input;// '0'
				char Output;// 'x00';
				MyStruct()
				{

				}
				MyStruct(char A, char B)
				{
					Input = A;
					Output = B;
				}
			};
			static const Array<MyStruct, 11> EscapeSequences =
			{
				MyStruct{ '0','\0' },
				MyStruct{ '\\','\\' },
				MyStruct{ 't','\t' },
				MyStruct{ '?','\?' },
				MyStruct{ 'v','\v' },
				MyStruct{ '\'','\'' },
				MyStruct{ 'b','\b' },
				MyStruct{ '\"','\"' },
				MyStruct{ 'r','\r' },
				MyStruct{ 'f','\f' },
			};

			out += Char2;
			return 1;
		}
	}
	else
	{
		if (isascii(Char1))
		{
			out += Char1;
			return 1;//move item
		}
		else
		{
			return 0;
		}
	}
	return 0;
}
UCodeLangEnd