#include "BitMaker.hpp"
UCodeLangStart


BytesPtr BitMaker::AsBytePtr()
{
	BytesPtr V;
	V.Bytes.reset(new Byte[_Bytes.size()]);
	std::memcpy(V.Bytes.get(), _Bytes.data(), _Bytes.size());
	V.Size = _Bytes.size();
	return V;
}

BytesPtr BitMaker::AsBytePtrAndMove()
{
	BytesPtr V;
	V.Bytes.reset(new Byte[_Bytes.size()]);
	std::memcpy(V.Bytes.get(), _Bytes.data(), _Bytes.size());
	V.Size = _Bytes.size();
	return V;
}

UCodeLangEnd
