#include "ParseHelper.hpp"

UCodeLangStart
bool ParseHelper::ParseCharliteralToChar(String_view string, char& out)
{
	String v;
	if (ParseCharliteralToChar(string, v) == 0)
	{
		out = v.front();
		return true;
	}
	else
	{
		return false;
	}
}

bool ParseHelper::ParseCharliteralToChar(String_view string, Utf8& out)
{
	String8 v;
	if (ParseCharliteralToChar(string, v) == 0)
	{
		out = v.front();
		return true;
	}
	else
	{
		return false;
	}
}

bool ParseHelper::ParseCharliteralToChar(String_view string, Utf16& out)
{
	String16 v;
	if (ParseCharliteralToChar(string, v) == 0)
	{
		out = v.front();
		return true;
	}
	else
	{
		return false;
	}
}

bool ParseHelper::ParseCharliteralToChar(String_view string, Utf32& out)
{
	String32 v;
	if (ParseCharliteralToChar(string, v) == 0)
	{
		out = v.front();
		return true;
	}
	else
	{
		return false;
	}
}


size_t ParseHelper::ParseStringliteralToString(String_view string, String& out)
{
	for (size_t i = 0; i < string.size();)
	{
		String_view V = String_view(string.data() + i, string.size() - i);

		size_t CompilerRet = ParseCharliteralToChar(V, out);
		if (CompilerRet==0)
		{
			return i;
		}
		i += CompilerRet;
	}
	return 0;
}
size_t ParseHelper::ParseStringliteralToString(String_view string, String8& out)
{
	for (size_t i = 0; i < string.size();)
	{
		String_view V = String_view(string.data() + i, string.size() - i);

		size_t CompilerRet = ParseCharliteralToChar(V, out);
		if (CompilerRet == 0)
		{
			return i;
		}
		i += CompilerRet;
	}
	return 0;
}
size_t ParseHelper::ParseStringliteralToString(String_view string, String16& out)
{
	for (size_t i = 0; i < string.size();)
	{
		String_view V = String_view(string.data() + i, string.size() - i);

		size_t CompilerRet = ParseCharliteralToChar(V, out);
		if (CompilerRet == 0)
		{
			return i;
		}
		i += CompilerRet;
	}
	return 0;
}
size_t ParseHelper::ParseStringliteralToString(String_view string, String32& out)
{
	for (size_t i = 0; i < string.size();)
	{
		String_view V = String_view(string.data() + i, string.size() - i);

		size_t CompilerRet = ParseCharliteralToChar(V, out);
		if (CompilerRet == 0)
		{
			return i;
		}
		i += CompilerRet;
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
			

			struct MyStruct
			{
				char Input;// '0'
				char Output;// 'x00';
				MyStruct()
				{
					Input = 0;
					Input = 0;
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
				MyStruct{ 'n','\n' },
			};
			Optional<char> Char2;
			for (auto& Item : EscapeSequences) 
			{
				if (Item.Input == string[1])
				{
					Char2 = Item.Output;
				}
			}
			if (Char2.has_value()) 
			{
				out += Char2.value();
				return 1;
			}
			else
			{
				return 0;
			}
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
size_t ParseHelper::ParseCharliteralToChar(String_view string, String8& out)
{
	//TODO fully add Utf8 stuport.
	out += Utf8(0);
	return 0;
}
size_t ParseHelper::ParseCharliteralToChar(String_view string, String16& out)
{
	//TODO fully add Utf16 stuport.
	out += Utf16(0);
	return 0;
}
size_t ParseHelper::ParseCharliteralToChar(String_view string, String32& out)
{
	//TODO fully add Utf32 stuport.
	out += Utf32(0);
	return 0;
}
UCodeLangEnd