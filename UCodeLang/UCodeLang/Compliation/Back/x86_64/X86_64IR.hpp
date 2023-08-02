#pragma once
#include "x86_64Data.h"
#include "X86_64Gen.hpp"
UCodeLangStart

//were register allcations,stack allcations,Instruction Selection Happens ect. 
struct X86_64IR
{
	X86_64IR()
	{

	}
	~X86_64IR()
	{

	}
	ImportUseing86x64Gen;
	using IDType = int;
	using FuncID = IDType;
	using CallConventionID = IDType;
	struct CallConvention
	{

	};
	struct Relocations
	{
		bool IsStaticRelocation = true;
		size_t RelocationOffset=0;
	};
	struct Ins
	{
		enum class InsRegSize :Int8
		{
			bits8,
			bits16,
			bits32,
			bits64,
		};
		struct CurrintFuncOffset
		{
			size_t offset = 0;
			explicit CurrintFuncOffset(size_t offset = 0):offset(offset) {}

		};
		//Ins
		struct NoOp
		{
			String ToString() const
			{
				String r;
				return r;
			}
		};
		struct Call
		{
			Variant<FuncID, GReg> callvariants;
			explicit Call(FuncID Value) :callvariants(Value) {};
			explicit Call(GReg Value) :callvariants(Value) {};
			String ToString() const
			{
				String r;
				return r;
			}
		};
		struct Ret 
		{
			String ToString() const
			{
				String r;
				return r;
			}
		};
		struct Jump 
		{ 
			Variant<FuncID, GReg, IndrReg, CurrintFuncOffset> jumpvariants; 
			explicit Jump(FuncID Value) :jumpvariants(Value) {};
			explicit Jump(GReg Value) :jumpvariants(Value) {};
			explicit Jump(IndrReg Value) :jumpvariants(Value) {};
			explicit Jump(CurrintFuncOffset Value) :jumpvariants(Value) {};
			String ToString() const
			{
				String r;
				return r;
			}
		};
		struct Move
		{
			struct RegToReg
			{
				GReg Src= GReg::Null;
				GReg Out = GReg::Null;
				InsRegSize RegSize = InsRegSize::bits64;
				
				explicit RegToReg(){};
				explicit RegToReg(GReg Src, GReg Out):Src(Src),Out(Out){};
				explicit RegToReg(InsRegSize RegSize,GReg Src, GReg Out) :Src(Src), Out(Out), RegSize(RegSize){};
				String ToString() const
				{
					String r;
					return r;
				}
			};
			struct ConstToReg
			{
				AnyInt64 Src = AnyInt64();
				GReg Out = GReg::Null;
				InsRegSize RegSize = InsRegSize::bits64;

				explicit ConstToReg() {};
				explicit ConstToReg(AnyInt64 Src, GReg Out):Src(Src), Out(Out) {};
				explicit ConstToReg(InsRegSize RegSize,AnyInt64 Src, GReg Out) :Src(Src), Out(Out), RegSize(RegSize) {};
				String ToString() const
				{
					
				}
			};
			struct IndRegToReg
			{
				IndrReg Src = IndrReg(GReg::Null);
				GReg Out = GReg::Null;
				InsRegSize RegSize = InsRegSize::bits64;

				explicit IndRegToReg() {};
				explicit IndRegToReg(IndrReg Src, GReg Out) :Src(Src), Out(Out) {};
				explicit IndRegToReg(InsRegSize RegSize, IndrReg Src, GReg Out) :Src(Src), Out(Out), RegSize(RegSize) {};
				String ToString() const
				{
					String r;
					return r;
				}
			};
			struct RegToRegInd
			{
				GReg Src = GReg(GReg::Null);
				IndrReg Out = IndrReg(GReg::Null);
				InsRegSize RegSize = InsRegSize::bits64;

				explicit RegToRegInd() {};
				explicit RegToRegInd(GReg Src, IndrReg Out) :Src(Src), Out(Out) {};
				explicit RegToRegInd(InsRegSize RegSize, GReg Src, IndrReg Out) :Src(Src), Out(Out), RegSize(RegSize) {};

				String ToString() const
				{
					String r;
					return r;
				}
			};
			//In
			Variant<RegToReg> MoveTypes;
			explicit Move(const RegToReg& Value) :MoveTypes(Value){}
			explicit Move(const ConstToReg& Value) :MoveTypes(Value) {}
			explicit Move(const IndRegToReg& Value) :MoveTypes(Value) {}
			explicit Move(const RegToRegInd& Value) :MoveTypes(Value) {}
		};

		Variant<NoOp, Call, Ret, Jump> variant;
		String ToString() const
		{

			if (auto val = variant.Get_If<NoOp>())
			{
				return val->ToString();
			}
			else if (auto val = variant.Get_If<Call>())
			{
				return val->ToString();
			}
			else if (auto val = variant.Get_If<Ret>())
			{
				return val->ToString();
			}
			else if (auto val = variant.Get_If<Jump>())
			{
				return val->ToString();
			}
			else
			{
				throw std::exception("not added");
			}
		}
		
		inline bool Reads_Stack() const
		{
			if (variant.Is<Ret>())
			{
				return true;
			}
			return false;
		}
		inline bool Writes_Stack() const
		{
			
			return false;
		}
		inline bool Reads_StackPtr() const
		{
			return Reads_Reg(GReg::RSP);
		}
		inline bool Writes_StackPtr() const
		{
			return Writes_Reg(GReg::RSP);
		}

		inline bool Writes_Reg(Optional<GReg> Reg = {}) const
		{
			return false;
		}
		inline bool Reads_Reg(Optional<GReg> Reg = {}) const
		{
			return false;
		}
		inline bool Writes_Reg(Optional<IndrReg> Reg = {}) const
		{
			return false;
		}
		inline bool Reads_Reg(Optional<IndrReg> Reg = {}) const
		{
			return false;
		}
		//
		explicit Ins(const Move& Value) :variant(Value){}
		explicit Ins(const NoOp& Value) :variant(Value){}
		explicit Ins(const Call& Value) :variant(Value){}
		explicit Ins(const Ret& Value) :variant(Value) {}
		explicit Ins(const Jump& Value) :variant(Value) {}
	};
	struct Func
	{
		FuncID funcID = FuncID();
		Vector<Ins> Body;
		CallConventionID CallConvention = CallConventionID();
		Func()
		{

		}

		static constexpr int v = sizeof(Ins);

		void Add_Ins(const Ins::Move& Item)
		{
			Body.push_back(Ins(Item));
		}
		void Add_Ins(const Ins::NoOp& Item)
		{
			Body.push_back(Ins(Item));
		}
		void Add_Ins(const Ins::Call& Item)
		{
			Body.push_back(Ins(Item));
		}
		void Add_Ins(const Ins::Ret& Item)
		{
			Body.push_back(Ins(Item));
		}
		void Add_Ins(const Ins::Jump& Item)
		{
			Body.push_back(Ins(Item));
		}
	};
	
	struct BuildInfo
	{
		struct BuildFunc
		{
			FuncID Func;
			Vector<Byte> Bytes;
			Vector<Relocations> Relocations;
		};
		Vector<BuildFunc> Funcs;
	};
	Vector<Func> Funcs;
	BinaryVectorMap<CallConventionID,CallConvention> CallingConventions;
	CallConventionID AddCallConvention(CallConvention&& Convention)
	{
		auto R = Next_NextCallID();
		CallingConventions.AddValue(R,std::move(Convention));

		return R;
	}
	Func& AddNewFunc(FuncID funcID)
	{
		Func func;
		func.funcID = funcID;
		Funcs.push_back(std::move(func));
	
		return Funcs.back();
	}
	Func* GetFunc(FuncID funcID)
	{
		for (auto& Item : Funcs)
		{
			if (Item.funcID == funcID)
			{
				return &Item;
			}
		}
		return nullptr;
	}
	const Func* GetFunc(FuncID funcID) const
	{
		for (auto& Item : Funcs)
		{
			if (Item.funcID == funcID)
			{
				return &Item;
			}
		}
		return nullptr;
	}
private:
	CallConventionID _NextCallID = CallConventionID();
	CallConventionID Next_NextCallID()
	{
		auto r =_NextCallID;
		_NextCallID++;
		return r;
	}
};

UCodeLangEnd