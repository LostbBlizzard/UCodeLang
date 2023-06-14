#pragma once

#include "LangTypes.hpp"
UCodeLangStart

using Endian_t = UInt8;
enum class Endian : Endian_t
{
	NaN,
	little,
	Big,
};
class BitConverter
{


public:
	
	inline thread_local static Endian InputOutEndian = Endian::little;

	#if UCodeLang_CPUBIs_BigEndian
	static constexpr Endian _CPUEndian = Endian::Big;
	#else
	static constexpr Endian _CPUEndian = Endian::little;
	#endif // UCodeLang_CPUBIs_BigEndian

	
	static_assert(sizeof(int) == 4, " 'int' is not 4 bytes");
	static_assert(sizeof(UInt8) == 1, " 'Byte' is not 1 bytes");
	static_assert(sizeof(Int16) == 2, " 'Int16' is not 2 bytes");
	static_assert(sizeof(float) == 4, " 'float' is not 4 bytes");
	static_assert(sizeof(Int64) == 8, " 'Int64' is not 8 bytes");
	static_assert(sizeof(float64) == 8, " 'float64' is not 8 bytes");

	struct Byte16
	{
		UInt8 A, B;
		inline void MoveValues(UInt8* P, size_t Indexofset)
		{
			UInt8* outv = &(P)[Indexofset];
			outv[0] = A;
			outv[1] = B;
		}
		inline Byte16 FlpBytes() 
		{
			return { B,A };
		}
	};
	struct Byte32
	{
		UInt8 A, B, C, D;
		inline void MoveValues(UInt8* P, size_t Indexofset)
		{
			UInt8* outv = &(P)[Indexofset];
			outv[0] = A;
			outv[1] = B;
			outv[2] = C;
			outv[3] = D;
		}
		inline Byte32 FlpBytes()
		{
			return { D,C,B,A };
		}
	};
	struct Byte64
	{
		UInt8 A, C, B, D,
			 E, F, G, H;
		inline void MoveValues(UInt8* P, size_t Indexofset)
		{
			UInt8* outv = &(P)[Indexofset];
			outv[0] = A;
			outv[1] = B;
			outv[2] = C;
			outv[3] = D;

			outv[4] = E;
			outv[5] = F;
			outv[6] = G;
			outv[7] = H;
		}
		inline Byte64 FlpBytes()
		{
			return 
			{
				H,G,F,E,
				D,C,B,A
			};
		}
	};
	

	
	inline static UInt8 GetBytes(bool Value)
	{
		return (UInt8)Value;
	}
	inline static bool BytesToBool(const void* DataBytes, size_t Index)
	{
		UInt8 B = ((UInt8*)DataBytes)[Index];
		return B;
	}

	inline static UInt8 GetBytes(signed char Value)
	{
		return GetBytes((unsigned char)Value);
	}
	inline static signed char BytesToSChar(const void* DataBytes, size_t Index)
	{
		return BytesToUChar(DataBytes, Index);
	}

	inline static UInt8 GetBytes(char Value)
	{
		return GetBytes((unsigned char)Value);
	}
	inline static char BytesToChar(const void* DataBytes, size_t Index)
	{
		return BytesToUChar(DataBytes, Index);
	}

	inline static UInt8 GetBytes(unsigned char Value)
	{
		return Value;
	}
	inline static unsigned char BytesToUChar(const void* DataBytes, size_t Index)
	{
		UInt8 B = ((UInt8*)DataBytes)[Index];
		return B;
	}

	static Byte16 GetBytes(Int16 Value);
	static Int16 BytesToInt16(const void* DataBytes, size_t Index);

	inline static Byte16 GetBytes(UInt16 Value) { return GetBytes(*(Int16*)&Value); }
	inline static UInt16 BytesToUInt16(const void* DataBytes, size_t Index)
	{
		auto V = BytesToInt16(DataBytes, Index);
		return *(UInt16*)&V;
	}

	static Byte32 GetBytes(int Value);
	static int BytesToInt(const void* DataBytes, size_t Index);

	inline static Byte32 GetBytes(unsigned int Value) { return GetBytes(*(int*)&Value); }
	inline static unsigned int BytesToUInt(const void* DataBytes, size_t Index){
		auto V = BytesToInt(DataBytes, Index);
		return *(unsigned int*)&V;
	}

	inline static Byte32 GetBytes(float Value){ return GetBytes(*(int*)&Value); }
	inline static float BytesTofloat(const void* DataBytes, size_t Index)
	{
		auto V = BytesToInt(DataBytes, Index);
		return *(float*)&V;
	}

	

	static Byte64 GetBytes(Int64 Value);
	static Int64 BytesToInt64(const void* DataBytes, size_t Index);
	
	inline static Byte64 GetBytes(UInt64 Value) {return GetBytes(*(Int64*)&Value);}
	inline static UInt64 BytesToUInt64(const void* DataBytes, size_t Index) {
		auto V = BytesToInt64(DataBytes, Index);
		return *(UInt64*)&V;
	}

	inline static Byte64 GetBytes(float64 Value){ return GetBytes(*(Int64*)&Value); }
	inline static float64 BytesTofloat64(const void* DataBytes, size_t Index)
	{
		auto V = BytesToInt64(DataBytes, Index);
		return *(float64*)&V;
	}
	//Helpers
	inline static void BytesToBool(const void* DataBytes, size_t Index, bool* OutPut)
	{
		OutPut[0] = BytesToBool(DataBytes, Index);
	}
	inline static void BytesToChar(const void* DataBytes, size_t Index, signed char* OutPut)
	{
		OutPut[0] = BytesToSChar(DataBytes, Index);
	}
	inline static void BytesToChar(const void* DataBytes, size_t Index, char* OutPut)
	{
		OutPut[0] = BytesToChar(DataBytes, Index);
	}
	inline static void BytesToChar(const void* DataBytes, size_t Index, unsigned char* OutPut)
	{
		OutPut[0] = BytesToChar(DataBytes, Index);
	}
	
	inline static void BytesToInt16(const void* DataBytes, size_t Index, Int16* OutPut)
	{
		OutPut[0] = BytesToInt16(DataBytes, Index);
	}
	inline static void BytesToInt16(const void* DataBytes, size_t Index, UInt16* OutPut)
	{
		OutPut[0] = BytesToUInt16(DataBytes, Index);
	}

	inline static void BytesToInt(const void* DataBytes, size_t Index, int* OutPut)
	{
		OutPut[0] = BytesToInt(DataBytes, Index);
	}
	inline static void BytesToInt(const void* DataBytes, size_t Index, unsigned int* OutPut)
	{
		OutPut[0] = BytesToUInt(DataBytes, Index);
	}

	inline static void BytesTofloat(const void* DataBytes, size_t Index, float* OutPut)
	{
		OutPut[0] = BytesTofloat(DataBytes, Index);
	}
	
	inline static void BytesToInt64(const void* DataBytes, size_t Index, Int64* OutPut)
	{
		OutPut[0] = BytesToInt64(DataBytes, Index);
	}
	inline static void BytesToInt64(const void* DataBytes, size_t Index, UInt64* OutPut)
	{
		OutPut[0] = BytesToUInt64(DataBytes, Index);
	}

	inline static void BytesTofloat64(const void* DataBytes, size_t Index, float64* OutPut)
	{
		OutPut[0] = BytesTofloat64(DataBytes, Index);
	}

	inline static void MoveBytes(const bool Value, void* DataBytes, size_t Index)
	{
		((UInt8*)DataBytes)[Index] = GetBytes(Value);
	}
	inline static void MoveBytes(const signed char Value, void* DataBytes, size_t Index)
	{
		((UInt8*)DataBytes)[Index] = GetBytes(Value);
	}
	inline static void MoveBytes(const char Value, void* DataBytes, size_t Index)
	{
		((UInt8*)DataBytes)[Index] = GetBytes(Value);
	}
	inline static void MoveBytes(const unsigned char Value, void* DataBytes, size_t Index)
	{
		((UInt8*)DataBytes)[Index] = GetBytes(Value);
	}

	inline static void MoveBytes(const Int16 Value, void* DataBytes, size_t Index)
	{
		GetBytes(Value).MoveValues((UInt8*)DataBytes, Index);
	}
	inline static void MoveBytes(const UInt16 Value, void* DataBytes, size_t Index)
	{
		GetBytes(Value).MoveValues((UInt8*)DataBytes, Index);
	}

	inline static void MoveBytes(const unsigned int Value, void* DataBytes, size_t Index)
	{
		GetBytes(Value).MoveValues((UInt8*)DataBytes, Index);
	}
	inline static void MoveBytes(const int Value, void* DataBytes, size_t Index)
	{
		GetBytes(Value).MoveValues((UInt8*)DataBytes, Index);
	}
	inline static void MoveBytes(const float Value, void* DataBytes, size_t Index)
	{
		GetBytes(Value).MoveValues((UInt8*)DataBytes, Index);
	}

	inline static void MoveBytes(const Int64 Value, void* DataBytes, size_t Index)
	{
		GetBytes(Value).MoveValues((UInt8*)DataBytes, Index);
	}
	inline static void MoveBytes(const UInt64 Value, void* DataBytes, size_t Index)
	{
		GetBytes(Value).MoveValues((UInt8*)DataBytes, Index);
	}
	
	inline static void MoveBytes(const float64 Value, void* DataBytes, size_t Index)
	{
		GetBytes(Value).MoveValues((UInt8*)DataBytes, Index);
	}
	
};

UCodeLangEnd
