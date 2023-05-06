#include "BitConverter.hpp"
UCodeLangStart


#define G_FixEndianess  const Endian ThisEndian = _CPUEndian;\
const Endian OutEndian = InputOutEndian;\
if (ThisEndian != OutEndian) {return CompilerRet.FlpBytes();}\

#define R_FixEndianess const Endian ThisEndian = _CPUEndian;\
const Endian InputEndian = InputOutEndian;\
if (ThisEndian != InputEndian) {auto v = bits.FlpBytes();bits =v; }\

BitConverter::Byte32 BitConverter::GetBytes(int Value)
{
	Byte32 CompilerRet;
	UInt8* VPointer = (UInt8*)&Value;

	CompilerRet.A = VPointer[0];
	CompilerRet.B = VPointer[1];
	CompilerRet.C = VPointer[2];
	CompilerRet.D = VPointer[3];



	G_FixEndianess
	

	return CompilerRet;
}
int BitConverter::BytesToInt(const void* DataBytes, size_t Index)
{
	UInt8* B = ((UInt8*)DataBytes) + Index;
	Byte32 bits = *(Byte32*)B;
	int* CompilerRet = (int*)&bits;

	R_FixEndianess
	

	return *CompilerRet;
}

BitConverter::Byte16 BitConverter::GetBytes(Int16 Value)
{
	Byte16 CompilerRet;
	UInt8* VPointer = (UInt8*)&Value;

	CompilerRet.A = VPointer[0];
	CompilerRet.B = VPointer[1];
	
	G_FixEndianess
	return CompilerRet;
}
Int16 BitConverter::BytesToInt16(const void* DataBytes, size_t Index)
{
	UInt8* B = ((UInt8*)DataBytes) + Index;
	Byte16 bits = *(Byte16*)B;
	Int16* CompilerRet = (Int16*)&bits;

	R_FixEndianess
	return *CompilerRet;
}

BitConverter::Byte64 BitConverter::GetBytes(Int64 Value)
{
	Byte64 CompilerRet;
	UInt8* VPointer = (UInt8*)&Value;

	CompilerRet.A = VPointer[0];
	CompilerRet.B = VPointer[1];
	CompilerRet.C = VPointer[2];
	CompilerRet.D = VPointer[3];

	CompilerRet.E = VPointer[4];
	CompilerRet.F = VPointer[5];
	CompilerRet.G = VPointer[6];
	CompilerRet.H = VPointer[7];

	G_FixEndianess
	return CompilerRet;
}
Int64 BitConverter::BytesToInt64(const void* DataBytes, size_t Index)
{
	UInt8* B = ((UInt8*)DataBytes) + Index;
	Byte64 bits = *(Byte64*)B;
	Int64* CompilerRet = (Int64*)&bits;

	R_FixEndianess
	return *CompilerRet;
}
UCodeLangEnd