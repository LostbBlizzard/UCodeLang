#pragma once

#ifndef UCodeLangNoCompiler
#include <UCodeLang/LangCore/BitMaker.hpp>
#include <UCodeLang/LangCore/LangTypes.hpp>
UCodeLangStart

//made using https://coinexsmartchain.medium.com/wasm-introduction-part-1-binary-format-57895d851580
class WasmFile
{
public:
	//type represents a 32-bit unsigned integer encoded with LEB128
	using varU32 = UInt32;
	using SectionID = Byte;
	using VectorLength = UInt32;
	using Tag = Byte;


	enum class ValType :Byte
	{
		i32 = 0x7F,
		i64 = 0x7E,
		f32 = 0x7D,
		f64 = 0x7C,
	};

	using WasmType = ValType;

	struct CustomSection
	{
		static constexpr SectionID SectionNumber = 0;

		void ToBytes(BitMaker& bit) const
		{

		}
		void FromBytes(BitReader& bit)
		{

		}
	};
	struct FuncType
	{
		static constexpr Tag tag = 0x60;
		Vector<WasmType> Params;
		Vector<WasmType> Results;

		void ToBytes(BitMaker& bit) const;
		void FromBytes(BitReader& bit);
	};


	struct TypeSection
	{
		static constexpr SectionID SectionNumber = 1;

		Vector<FuncType> Types;

		void ToBytes(BitMaker& bit) const;
		void FromBytes(BitReader& bit);
	};
	struct ImportSection
	{
		static constexpr SectionID SectionNumber = 2;
	};
	struct FuncSection
	{
		static constexpr SectionID SectionNumber = 3;
		Vector<size_t> TypesIndex;

		void ToBytes(BitMaker& bit) const;
		void FromBytes(BitReader& bit);
	};
	struct TableSection
	{
		static constexpr SectionID SectionNumber = 4;
	};
	struct MemSection
	{
		static constexpr SectionID SectionNumber = 5;

		struct Limits
		{
			enum class HasMax : Byte
			{
				min,
				minAmax,
			};
			HasMax hasmax = HasMax::min;
			varU32 min = 0;
			varU32 max = 0;
		};
		Vector< Limits> limits;

		void ToBytes(BitMaker& bit) const;
		void FromBytes(BitReader& bit);
	};
	struct GlobalSection
	{
		static constexpr SectionID SectionNumber = 6;
	};

	enum class ExportTag :Byte
	{
		Func = 0,
		Table = 1,
		mem = 2,
		global = 4,
	};


	struct Export
	{
		String Name;
		ExportTag Tag = ExportTag::Func;
		varU32 Index;
	};
	struct ExportSection
	{
		static constexpr SectionID SectionNumber = 7;
		Vector<Export> Exports;

		void ToBytes(BitMaker& bit) const;
		void FromBytes(BitReader& bit);
	};
	struct StartSection
	{
		static constexpr SectionID SectionNumber = 8;
	};
	struct ElemSection
	{
		static constexpr SectionID SectionNumber = 9;
	};


	struct Expr
	{
		enum class Ins : Byte
		{
			Unreachable = 0x00,
			Return = 0x0f,
			i32const = 0x41,
			i64const = 0x42,
			f32const = 0x43,
			f64const = 0x44,

			i32store = 0x36,
			i64store = 0x37,
			f32store = 0x38,
			f64store = 0x39,

			i32load = 0x28,
			i64load = 0x29,
			f32load = 0x2a,
			f64load = 0x2b,

			i32add = 0x6a,
			i64add = 0x7c,
			f32add = 0x92,
			f64add = 0xa0,


			i32sub = 0x6b,
			i64sub = 0x7d,
			f32sub = 0x93,
			f64sub = 0xa1,

			localget = 0x20,
			localset = 0x21,

			Call = 0x10,

			end = 0x0b,
		};
		Ins InsType = Ins::Unreachable;
		AnyInt64 Const;
		AnyInt64 Const2;

		void Unreachable()
		{
			InsType = Ins::Unreachable;
		}
		void Return()
		{
			InsType = Ins::Return;
		}

		void i32_const(Int32 v)
		{
			InsType = Ins::i32const;
			Const = v;
		}
		void i64_const(Int64 v)
		{
			InsType = Ins::i64const;
			Const = v;
		}
		void f32_const(float32 v)
		{
			InsType = Ins::f32const;
			Const = v;
		}
		void f64_const(float64 v)
		{
			InsType = Ins::f64const;
			Const = v;
		}

		static constexpr int defaultalignment = 0x02;
		void i32_store(int alignment = defaultalignment, int offset = 0)
		{
			InsType = Ins::i32store;
			Const = alignment;
			Const2 = offset;
		}
		void i64_store(int alignment = defaultalignment, int offset = 0)
		{
			InsType = Ins::i64store;
			Const = alignment;
			Const2 = offset;
		}
		void f32_store(int alignment = defaultalignment, int offset = 0)
		{
			InsType = Ins::f32store;
			Const = alignment;
			Const2 = offset;
		}
		void f64_store(int alignment = defaultalignment, int offset = 0)
		{
			InsType = Ins::f64store;
			Const = alignment;
			Const2 = offset;
		}

		void i32_load(int alignment = defaultalignment, int offset = 0)
		{
			InsType = Ins::i32load;
			Const = alignment;
			Const2 = offset;
		}
		void i64_load(int alignment = defaultalignment, int offset = 0)
		{
			InsType = Ins::i64load;
			Const = alignment;
			Const2 = offset;
		}
		void f32_load(int alignment = defaultalignment, int offset = 0)
		{
			InsType = Ins::f32load;
			Const = alignment;
			Const2 = offset;
		}
		void f64_load(int alignment = defaultalignment, int offset = 0)
		{
			InsType = Ins::f64load;
			Const = alignment;
			Const2 = offset;
		}

		void i32_const(UInt32 v) { i32_const(*(Int32*)&v); }
		void i64_const(UInt64 v) { i64_const(*(Int64*)&v); }

		void end()
		{
			InsType = Ins::end;
		}


		void i32_Add()
		{
			InsType = Ins::i32add;
		}
		void i64_Add()
		{
			InsType = Ins::i64add;
		}
		void f32_Add()
		{
			InsType = Ins::f32add;
		}
		void f64_Add()
		{
			InsType = Ins::f64add;
		}

		void i32_Sub()
		{
			InsType = Ins::i32sub;
		}
		void i64_Sub()
		{
			InsType = Ins::i64sub;
		}
		void f32_Sub()
		{
			InsType = Ins::f32sub;
		}
		void f64_Sub()
		{
			InsType = Ins::f64sub;
		}
		void local_get(UInt32 index)
		{
			InsType = Ins::localget;
			Const = index;
		}
		void call(UInt32 index)
		{
			InsType = Ins::Call;
			Const = index;
		}

		void ToBytes(BitMaker& bit) const;
		void FromBytes(BitReader& bit);
	};
	struct Locals
	{
		UInt32 N;
		ValType Type;

		void ToBytes(BitMaker& bit) const
		{
			bit.WriteType(N);
			bit.WriteType((Byte)Type);
		}
		void FromBytes(BitReader& bit)
		{
			bit.ReadType(N);
			bit.ReadType(*(Byte*)&Type);
		}
	};
	struct Code
	{

		Vector<Locals> locals;
		Vector<Expr> Ins;

		void ToBytes(BitMaker& bit) const;
		void FromBytes(BitReader& bit);

		void Push_i32_const(Int32 v)
		{
			Ins.push_back({});
			Ins.back().i32_const(v);
		}
		void Push_i64_const(Int64 v)
		{
			Ins.push_back({});
			Ins.back().i64_const(v);
		}
		void Push_f32_const(float32 v)
		{
			Ins.push_back({});
			Ins.back().f32_const(v);
		}
		void Push_f64_const(float64 v)
		{
			Ins.push_back({});
			Ins.back().f64_const(v);
		}

		void Push_i32_store(int alignment = Expr::defaultalignment, int offset = 0)
		{
			Ins.push_back({});
			Ins.back().i32_store(alignment, offset);
		}
		void Push_i64_store(int alignment = Expr::defaultalignment, int offset = 0)
		{
			Ins.push_back({});
			Ins.back().i64_store(alignment, offset);
		}
		void Push_f32_store(int alignment = Expr::defaultalignment, int offset = 0)
		{
			Ins.push_back({});
			Ins.back().f32_store(alignment, offset);
		}
		void Push_f64_store(int alignment = Expr::defaultalignment, int offset = 0)
		{
			Ins.push_back({});
			Ins.back().f64_store(alignment, offset);
		}

		void Push_i32_load(int alignment = Expr::defaultalignment, int offset = 0)
		{
			Ins.push_back({});
			Ins.back().i32_load(alignment, offset);
		}
		void Push_i64_load(int alignment = Expr::defaultalignment, int offset = 0)
		{
			Ins.push_back({});
			Ins.back().i64_load(alignment, offset);
		}

		void Push_f32_load(int alignment = Expr::defaultalignment, int offset = 0)
		{
			Ins.push_back({});
			Ins.back().f32_load(alignment, offset);
		}
		void Push_f64_load(int alignment = Expr::defaultalignment, int offset = 0)
		{
			Ins.push_back({});
			Ins.back().f64_load(alignment, offset);
		}



		void Push_i32_const(UInt32 v) { Push_i32_const(*(Int32*)&v); }
		void Push_i64_const(UInt64 v) { Push_i64_const(*(Int64*)&v); }

		void Push_Unreachable()
		{
			Ins.push_back({});
			Ins.back().Unreachable();
		}
		void Push_Return()
		{
			Ins.push_back({});
			Ins.back().Return();
		}
		void Push_End()
		{
			Ins.push_back({});
			Ins.back().end();
		}


		void Push_i32_Add()
		{
			Ins.push_back({});
			Ins.back().i32_Add();
		}
		void Push_i64_Add()
		{
			Ins.push_back({});
			Ins.back().i64_Add();
		}

		void Push_f32_Add()
		{
			Ins.push_back({});
			Ins.back().f32_Add();
		}
		void Push_f64_Add()
		{
			Ins.push_back({});
			Ins.back().f64_Add();
		}

		void Push_i32_Sub()
		{
			Ins.push_back({});
			Ins.back().i32_Sub();
		}
		void Push_i64_Sub()
		{
			Ins.push_back({});
			Ins.back().i64_Sub();
		}

		void Push_f32_Sub()
		{
			Ins.push_back({});
			Ins.back().f32_Sub();
		}
		void Push_f64_Sub()
		{
			Ins.push_back({});
			Ins.back().f64_Sub();
		}

		void Push_local_get(UInt32 index)
		{
			Ins.push_back({});
			Ins.back().local_get(index);
		}
		void Push_call(UInt32 funcindex)
		{
			Ins.push_back({});
			Ins.back().call(funcindex);
		}
	};
	struct CodeSection
	{
		static constexpr SectionID SectionNumber = 10;
		Vector<Code> code;
		void ToBytes(BitMaker& bit) const;
		void FromBytes(BitReader& bit);

		//


	};
	struct DataSection
	{
		static constexpr SectionID SectionNumber = 11;
	};
	struct Section
	{
		struct None {};
		Variant<None, TypeSection, FuncSection, ExportSection, CodeSection, MemSection> Type;

		void ToBytes(BitMaker& bit) const;
		void FromBytes(BitReader& bit);
	};


	UInt32 Version = 1;
	Vector<Section> section;

	static bool FromFile(const Path& path, WasmFile& file);
	static bool ToFile(const WasmFile& file, const Path& path);

	static bool FromBytes(WasmFile& file, const BytesView Bytes);

	static BytesPtr ToBytes(const WasmFile& file);

	static void ReadLEB128(BitReader& bit, WasmFile::varU32& out);
	static void WriteLEB128(BitMaker& bit, WasmFile::varU32 value);
	static void Write_String(BitMaker& bit, const String& str)
	{
		bit.WriteType((Byte)str.size());
		for (const auto& c : str) {
			bit.WriteType(c);
		}
	}
	static void Read_String(BitReader& bit, String& str)
	{
		Byte V = 0;
		bit.ReadType(V);

		str.resize(V);

		for (auto& c : str) {
			bit.ReadType(c);
		}
	}
	//For Debuging
	String ToWat() const;

	String ToWat(const ValType& Item) const;
	String ToWat(const FuncType& Item) const;
	String ToWat(const Code& Item) const;
	String ToWat(const Expr& Item) const;
private:
};
UCodeLangEnd

#endif