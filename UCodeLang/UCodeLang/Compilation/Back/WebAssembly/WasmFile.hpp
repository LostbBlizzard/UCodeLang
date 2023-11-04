#pragma once
#include <UCodeLang/LangCore/BitMaker.hpp>
#include <UCodeLang/LangCore/LangTypes.hpp>
UCodeLangStart

//made useing https://coinexsmartchain.medium.com/wasm-introduction-part-1-binary-format-57895d851580
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

		void ToBytes(BitMaker& bit) const
		{
			bit.WriteType(tag);

			WasmFile::WriteLEB128(bit,(VectorLength)Params.size());
			for (auto& Item : Params)
			{
				bit.WriteType((Byte)Item);
			}

			WasmFile::WriteLEB128(bit, (VectorLength)Results.size());
			for (auto& Item : Results)
			{
				bit.WriteType((Byte)Item);
			}
		}
		void FromBytes(BitReader& bit)
		{
			Tag V =0;
			bit.ReadType(V);

			{
				VectorLength Size = 0;
				WasmFile::ReadLEB128(bit,Size);

				Params.resize(Size);

				for (size_t i = 0; i < Size; i++)
				{
					bit.ReadType(*(Byte*)&Params[i]);
				}
			}
			{
				VectorLength Size = 0;
				WasmFile::ReadLEB128(bit, Size);

				Results.resize(Size);

				for (size_t i = 0; i < Size; i++)
				{
					bit.ReadType(*(Byte*)&Results[i]);
				}
			}
		}
	};


	struct TypeSection
	{
		static constexpr SectionID SectionNumber = 1;

		Vector<FuncType> Types;

		void ToBytes(BitMaker& bit) const
		{
			bit.WriteType(SectionNumber);

			BitMaker tep;
			
			{
				WasmFile::WriteLEB128(tep,(VectorLength)Types.size());
			
				for (auto& Item : Types)
				{
					Item.ToBytes(tep);
				}
			}
			
			auto& bytes = tep.Get_Bytes();
			WasmFile::WriteLEB128(bit, (varU32)bytes.size());
			bit.WriteBytes(bytes.data(), bytes.size());
		}
		void FromBytes(BitReader& bit)
		{
			varU32 L = 0;
			WasmFile::ReadLEB128(bit, L);

			auto bytes = bit.ReadBytesAsSpan(L);
			BitReader v;
			v.SetBytes(bytes.Data(), bytes.Size());

			{
				VectorLength typeslength = 0;
				WasmFile::ReadLEB128(v, typeslength);

				Types.resize(typeslength);

				for (auto& Item : Types)
				{
					Item.FromBytes(v);
				}
			}
		}
	};
	struct ImportSection
	{
		static constexpr SectionID SectionNumber = 2;
	};
	struct FuncSection
	{
		static constexpr SectionID SectionNumber = 3;
		Vector<size_t> TypesIndex;

		void ToBytes(BitMaker& bit) const
		{
			bit.WriteType(SectionNumber);

			BitMaker tep;
			
			{
				WasmFile::WriteLEB128(tep, (VectorLength)TypesIndex.size());

				for (auto& Item : TypesIndex)
				{
					WasmFile::WriteLEB128(tep, (VectorLength)Item);
				}
			}

			auto& bytes = tep.Get_Bytes();
			WasmFile::WriteLEB128(bit, (varU32)bytes.size());
			bit.WriteBytes(bytes.data(), bytes.size());
		}
		void FromBytes(BitReader& bit)
		{
			varU32 L = 0;//SectionNumber
			WasmFile::ReadLEB128(bit, L);

			auto bytes = bit.ReadBytesAsSpan(L);
			BitReader v;
			v.SetBytes(bytes.Data(), bytes.Size());

			{
				VectorLength typeslength = 0;
				WasmFile::ReadLEB128(v, typeslength);

				TypesIndex.resize(typeslength);

				for (auto& Item : TypesIndex)
				{
					WasmFile::ReadLEB128(v, *(varU32*)&Item);
				}
			}
		}
	};
	struct TableSection
	{
		static constexpr SectionID SectionNumber = 4;
	};
	struct MemSection
	{
		static constexpr SectionID SectionNumber = 5;
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

		void ToBytes(BitMaker& bit) const
		{
			bit.WriteType(SectionNumber);

			BitMaker tep;

			{
				WasmFile::WriteLEB128(tep, (VectorLength)Exports.size());

				for (auto& Item : Exports)
				{
					Write_String(tep, Item.Name);
					tep.WriteType((Byte)Item.Tag);
					WasmFile::WriteLEB128(tep, Item.Index);
				}
			}

			auto& bytes = tep.Get_Bytes();
			WasmFile::WriteLEB128(bit, (varU32)bytes.size());
			bit.WriteBytes(bytes.data(), bytes.size());
		}
		void FromBytes(BitReader& bit)
		{
			varU32 L = 0;//SectionNumber
			WasmFile::ReadLEB128(bit, L);

			auto bytes = bit.ReadBytesAsSpan(L);
			BitReader v;
			v.SetBytes(bytes.Data(), bytes.Size());

			{
				VectorLength exportslength = 0;
				WasmFile::ReadLEB128(v, exportslength);

				Exports.resize(exportslength);

				for (auto& Item : Exports)
				{
					Read_String(v,Item.Name);

					v.ReadType(*(Byte*)&Item.Tag);

					WasmFile::ReadLEB128(v, Item.Index);
				}
			}
		}
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

			end = 0x0b,
		};
		Ins InsType= Ins::Unreachable;
		AnyInt64 Const;

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

		void i32_store()
		{
			InsType = Ins::i32store;
		}
		void i64_store()
		{
			InsType = Ins::i64store;
		}
		void f32_store()
		{
			InsType = Ins::f32store;
		}
		void f64_store()
		{
			InsType = Ins::f64store;
		}

		void i32_load()
		{
			InsType = Ins::i32load;
		}
		void i64_load()
		{
			InsType = Ins::i64load;
		}
		void f32_load()
		{
			InsType = Ins::f32load;
		}
		void f64_load()
		{
			InsType = Ins::f64load;
		}

		void i32_const(UInt32 v) { i32_const(*(Int32*)&v); }
		void i64_const(UInt64 v)  { i64_const(*(Int64*)&v); }

		void end()
		{
			InsType = Ins::end;
		}

		void ToBytes(BitMaker& bit) const
		{
			bit.WriteType((Byte)InsType);
			if (InsType == Ins::i32const)
			{
				WriteLEB128(bit, Const.AsUInt32);
			}
			else if (InsType == Ins::f32const)
			{
				bit.WriteType(Const.Asfloat32);
			}
			else if (InsType == Ins::Return
				|| InsType == Ins::Unreachable
				|| InsType == Ins::end
				|| InsType == Ins::i32store
				|| InsType == Ins::i32load)
			{

			}
			else
			{
				UCodeLangUnreachable();
			}
		}
		void FromBytes(BitReader& bit)
		{
			bit.ReadType(*(Byte*)&InsType);
			if (InsType == Ins::i32const)
			{
				ReadLEB128(bit, Const.AsUInt32);
			}
			else if (InsType == Ins::Return
				|| InsType == Ins::Unreachable
				|| InsType == Ins::end
				|| InsType == Ins::i32store)
			{

			}
			else
			{
				UCodeLangUnreachable();
			}
		}
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

		void ToBytes(BitMaker& bit) const
		{
			

			BitMaker tep;

			{
				WasmFile::WriteLEB128(tep, (VectorLength)locals.size());

				for (auto& Item : locals)
				{
					Item.ToBytes(tep);
				}
			}
			{

				for (auto& Item : Ins)
				{
					Item.ToBytes(tep);
				}
			}
			
			varU32 Size = tep.size();
			WriteLEB128(bit, Size);
			bit.WriteBytes(tep.data(), tep.size());
		}
		void FromBytes(BitReader& bit)
		{
			varU32 Size = 0;
			ReadLEB128(bit, Size);
		

			{
				varU32 Count = 0;
				WasmFile::ReadLEB128(bit, Count);
				locals.resize(Count);

				for (size_t i = 0; i < Count; i++)
				{
					locals[i].FromBytes(bit);
				}
			}
			{
				Expr val = Expr();

				do
				{
					val = Expr();

					val.FromBytes(bit);

					Ins.push_back(std::move(val));


				} while (val.InsType != Expr::Ins::end);
				
			}
		}

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

		void Push_i32_store()
		{
			Ins.push_back({});
			Ins.back().i32_store();
		}
		void Push_i64_store()
		{
			Ins.push_back({});
			Ins.back().i64_store();
		}
		void Push_f32_store()
		{
			Ins.push_back({});
			Ins.back().f32_store();
		}
		void Push_f64_store()
		{
			Ins.push_back({});
			Ins.back().f64_store();
		}

		void Push_i32_load()
		{
			Ins.push_back({});
			Ins.back().i32_load();
		}
		void Push_i64_load()
		{
			Ins.push_back({});
			Ins.back().i64_load();
		}

		void Push_f32_load()
		{
			Ins.push_back({});
			Ins.back().f32_load();
		}
		void Add_f64_load()
		{
			Ins.push_back({});
			Ins.back().f64_load();
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
	};
	struct CodeSection
	{
		static constexpr SectionID SectionNumber = 10;
		Vector<Code> code;
		void ToBytes(BitMaker& bit) const
		{
			bit.WriteType(SectionNumber);

			BitMaker tep;

			{
				WasmFile::WriteLEB128(tep, (VectorLength)code.size());

				for (auto& Item : code)
				{
					Item.ToBytes(tep);
				}
			}

			auto& bytes = tep.Get_Bytes();
			WasmFile::WriteLEB128(bit, (varU32)bytes.size());
			bit.WriteBytes(bytes.data(), bytes.size());
		}
		void FromBytes(BitReader& bit)
		{
			varU32 L = 0;
			WasmFile::ReadLEB128(bit, L);

			auto bytes = bit.ReadBytesAsSpan(L);
			BitReader v;
			v.SetBytes(bytes.Data(), bytes.Size());

			{
				VectorLength typeslength = 0;
				WasmFile::ReadLEB128(v, typeslength);

				code.resize(typeslength);

				for (auto& Item : code)
				{
					Item.FromBytes(v);
				}
			}
		}

		//
		

	};
	struct DataSection
	{
		static constexpr SectionID SectionNumber = 11;
	};
	struct Section
	{
		struct None{};
		Variant<None, TypeSection,FuncSection,ExportSection,CodeSection> Type;

		void ToBytes(BitMaker& bit) const
		{
			if (auto V = Type.Get_If<TypeSection>())
			{
				V->ToBytes(bit);
			}
			else if (auto V = Type.Get_If<FuncSection>())
			{
				V->ToBytes(bit);
			}
			else if (auto V = Type.Get_If<CodeSection>())
			{
				V->ToBytes(bit);
			}
			else if (auto V = Type.Get_If<ExportSection>())
			{
				V->ToBytes(bit);
			}
			else
			{
				UCodeLangUnreachable();
			}
		}
		void FromBytes(BitReader& bit)
		{
			SectionID sectionNumber;
			bit.ReadType(sectionNumber);

			if (sectionNumber == TypeSection::SectionNumber)
			{
				TypeSection r;
				r.FromBytes(bit);
				Type = std::move(r);
			}
			else if (sectionNumber == FuncSection::SectionNumber)
			{
				FuncSection r;
				r.FromBytes(bit);

				Type = std::move(r);
			}
			else if (sectionNumber == CodeSection::SectionNumber)
			{
				CodeSection r;
				r.FromBytes(bit);

				Type = std::move(r);
			}
			else if (sectionNumber == ExportSection::SectionNumber)
			{
				ExportSection r;
				r.FromBytes(bit);

				Type = std::move(r);
			}
			else
			{
				UCodeLangUnreachable();
			}
		}
	};


	UInt32 Version = 1;
	Vector<Section> section;

	static bool FromFile(const Path& path, WasmFile& file);
	static bool ToFile(const WasmFile& file, const Path& path);
	
	static bool FromBytes(WasmFile& file, const BytesView Bytes);
	
	static BytesPtr ToBytes(const WasmFile& file);

	static void ReadLEB128(BitReader& bit, WasmFile::varU32& out);
	static void WriteLEB128(BitMaker& bit, WasmFile::varU32 value);
	static void Write_String(BitMaker& bit,const String& str) 
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
