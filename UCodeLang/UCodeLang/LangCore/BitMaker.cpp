#include "BitMaker.hpp"
UCodeLangStart


BytesPtr BitMaker::AsBytePtr()
{
	BytesPtr V;
	V.Resize(_Bytes.size());
	std::memcpy(V.Data(), _Bytes.data(), _Bytes.size());
	return V;
}

BytesPtr BitMaker::AsBytePtrAndMove()
{
	BytesPtr V;
	V.Resize(_Bytes.size());
	std::memcpy(V.Data(), _Bytes.data(), _Bytes.size());
	return V;
}

UCodeLangEnd
