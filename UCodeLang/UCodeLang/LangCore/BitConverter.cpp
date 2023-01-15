#include "BitConverter.hpp"
UCodeLangStart


#define G_FixEndianess  const Endian ThisEndian = _CPUEndian;\
const Endian OutEndian = InputOutEndian;\
if (ThisEndian != OutEndian) {return R.FlpBytes();}\

#define R_FixEndianess const Endian ThisEndian = _CPUEndian;\
const Endian InputEndian = InputOutEndian;\
if (ThisEndian != InputEndian) {auto v = bits.FlpBytes();bits =v; }\

BitConverter::Byte32 BitConverter::GetBytes(int Value)
{
	Byte32 R;
	UInt8* VPointer = (UInt8*)&Value;

	R.A = VPointer[0];
	R.B = VPointer[1];
	R.C = VPointer[2];
	R.D = VPointer[3];



	G_FixEndianess
	

	return R;
}
int BitConverter::BytesToInt(void* DataBytes, size_t Index)
{
	UInt8* B = ((UInt8*)DataBytes) + Index;
	Byte32 bits = *(Byte32*)B;
	int* R = (int*)&bits;

	R_FixEndianess
	

	return *R;
}

BitConverter::Byte16 BitConverter::GetBytes(Int16 Value)
{
	Byte16 R;
	UInt8* VPointer = (UInt8*)&Value;

	R.A = VPointer[0];
	R.B = VPointer[1];
	
	G_FixEndianess
	return R;
}
Int16 BitConverter::BytesToInt16(void* DataBytes, size_t Index)
{
	UInt8* B = ((UInt8*)DataBytes) + Index;
	Byte16 bits = *(Byte16*)B;
	Int16* R = (Int16*)&bits;

	R_FixEndianess
	return *R;
}

BitConverter::Byte64 BitConverter::GetBytes(Int64 Value)
{
	Byte64 R;
	UInt8* VPointer = (UInt8*)&Value;

	R.A = VPointer[0];
	R.B = VPointer[1];
	R.C = VPointer[2];
	R.D = VPointer[3];

	R.E = VPointer[4];
	R.F = VPointer[5];
	R.G = VPointer[6];
	R.H = VPointer[7];

	G_FixEndianess
	return R;
}
Int64 BitConverter::BytesToInt64(void* DataBytes, size_t Index)
{
	UInt8* B = ((UInt8*)DataBytes) + Index;
	Byte64 bits = *(Byte64*)B;
	Int64* R = (Int64*)&bits;

	R_FixEndianess
	return *R;
}
UCodeLangEnd