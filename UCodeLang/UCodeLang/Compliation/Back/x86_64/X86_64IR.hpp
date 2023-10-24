#pragma once
#include "x86_64Data.hpp"
#include "X86_64Builder.hpp"
#include "UCodeLang/LangCore/DataType/UnorderedMap.hpp"
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
	
	using IDType = UInt64;
	using FuncID = IDType;
	using CallConventionID = IDType;
	using RelocationID = IDType;
	struct CallConvention
	{
		Vector<GReg> IntegerFuncionArguments;
		Vector<FReg> FloatingPointFuncionArguments;

		GReg IntegerReturnValue = GReg::RAX;
		FReg FloatReturnValue = FReg::xmm0;

		Vector<Byte> FuncionProlog;
		Vector<Byte> FuncionEpilogue;

		//if not here Register will not be used
		Vector<GReg> CallPreservedGRegisters;
		Vector<FReg> CallPreservedFRegisters;

		Vector<GReg> CallClobberedGRegisters;
		Vector<FReg> CallClobberedFRegisters;

		enum class WindowsCallingConvention
		{
			Fast_Call,

			Cdecl = Fast_Call,
			Stdcall,
			Thiscall,
			Vectorcall,

			Default = Fast_Call,
		};

		//https://learn.microsoft.com/en-us/cpp/build/x64-calling-convention?view=msvc-170
		void SetWindows(WindowsCallingConvention convention = WindowsCallingConvention::Default)
		{
			if (convention != WindowsCallingConvention::Fast_Call)
			{
				UCodeLangThrowException("not added");
			}

			IntegerFuncionArguments = { GReg::RCX,GReg::RDX,GReg::r8,GReg::r9 };
			FloatingPointFuncionArguments = { FReg::xmm0,FReg::xmm1,FReg::xmm2,FReg::xmm3 };

			IntegerReturnValue = GReg::RAX;
			FloatReturnValue = FReg::xmm0;

			FuncionProlog = {
				0x55,               // push rbp
				0x48, 0x89, 0xe5,   // mov	rbp, rsp
			};
			FuncionEpilogue = {
				0x5d,   // pop	rbp
				0xc3    // ret
			};

			//https://learn.microsoft.com/en-us/cpp/build/x64-software-conventions?view=msvc-170&source=recommendations
			CallClobberedGRegisters = { GReg::RAX,GReg::RCX,GReg::RDX,GReg::r8,GReg::r9 };
			CallClobberedFRegisters = { FReg::xmm0,FReg::xmm1,FReg::xmm2,FReg::xmm3,FReg::xmm4 };

			CallPreservedGRegisters = { GReg::r12,GReg::r15,GReg::RDI,GReg::RSI,GReg::RBX,GReg::RBP,GReg::RSP };
			CallPreservedFRegisters = {};
		}
		//https://www.ired.team/miscellaneous-reversing-forensics/windows-kernel-internals/linux-x64-calling-convention-stack-frame
		void SetLinux()
		{
			//For testing
			SetWindows();

			IntegerFuncionArguments = { GReg::RDI,GReg::RSI,GReg::RDX,GReg::r8,GReg::r9 };
		}
		//https://github.com/apple/swift/blob/main/docs/ABI/CallConvSummary.rst
		//https://developer.apple.com/library/archive/documentation/DeveloperTools/Conceptual/LowLevelABI/140-x86-64_Function_Calling_Conventions/x86_64.html
		void SetMacOSandIOS()
		{
			//For testing
			SetWindows();
		}

		//The UCode ABI/calling convention is undefined it may change at anytime.
		void SetUCodeLang()
		{
			SetWindows();
		}

		//The calling convention meant to be used when funcion dont need to be exposed to any api
		//is undefined it may change at anytime.
		void SetCallInternal()
		{
			SetUCodeLang();
		}

		static CallConvention Get_UCodeLang()
		{
			CallConvention r;
			r.SetUCodeLang();
			return r;
		}
		static CallConvention Get_CallInternal()
		{
			CallConvention r;
			r.SetCallInternal();
			return r;
		}

		static CallConvention Get_Windows_Default()
		{
			CallConvention r;
			r.SetWindows();
			return r;
		}
		static CallConvention Get_Windows_Cdecl()
		{
			CallConvention r;
			r.SetWindows(WindowsCallingConvention::Cdecl);
			return r;
		}

		static CallConvention Get_Linux_Default()
		{
			CallConvention r;
			r.SetLinux();
			return r;
		}
		static CallConvention Get_Linux_Cdecl()
		{
			CallConvention r;
			r.SetLinux();
			return r;
		}

		static CallConvention Get_MacOSandIOS_Default()
		{
			CallConvention r;
			r.SetMacOSandIOS();
			return r;
		}
		static CallConvention Get_MacOSandIOS_Cdecl()
		{
			CallConvention r;
			r.SetMacOSandIOS();
			return r;
		}
	};
	enum class RelocationType :Byte
	{
		Size8,
		Size16,
		Size32,
		Size64,
	};
	struct Relocation
	{
		RelocationID RelocationId =0;
		size_t ByteToUpdateOffset=0;
		RelocationType Type = RelocationType::Size32;
	};

	struct Relocation8
	{
		RelocationID ID = 0;
		explicit Relocation8(RelocationID Relocation = 0)
			:ID(Relocation)
		{

		}
	};
	struct Relocation16
	{
		RelocationID ID = 0;
		explicit Relocation16(RelocationID Relocation = 0)
			:ID(Relocation)
		{

		}
	};
	struct Relocation32
	{
		
		RelocationID ID = 0;
		explicit Relocation32(RelocationID Relocation = 0)
			:ID(Relocation)
		{

		}
	};
	struct Relocation64
	{
		RelocationID ID = 0;
		explicit Relocation64(RelocationID Relocation = 0)
			:ID(Relocation)
		{

		}
	};
	using NearRelocation32 = X86_64Builder::Near<Relocation32>;
	using NearRelocation64 = X86_64Builder::Near<Relocation64>;
	static String ToString(GReg V)
	{
		switch (V)
		{
		case GReg::RAX:return "RAX";
		case GReg::RBX:return "RBX";
		case GReg::RCX:return "RCX";
		case GReg::RDX:return "RDX";
		case GReg::RSP:return "RSP";
		case GReg::RBP:return "RBP";
		case GReg::RSI:return "RSI";
		case GReg::RDI:return "RDI";
		case GReg::r8:return "r8";
		case GReg::r9:return "r9";
		case GReg::r10:return "r10";
		case GReg::r11:return "r11";
		case GReg::r12:return "r12";
		case GReg::r13:return "r13";
		case GReg::r14:return "r14";
		case GReg::r15:return "r15";
		default:return "n/a";break;
		}
	}
	static String ToString(Near32 V)
	{
		return "Near32" + (String)"(" + std::to_string(V.Value) + ")";
	}
	static String ToString(Near64 V)
	{
		return "Near64" + (String)"(" + std::to_string(V.Value) + ")";
	}

	static String ToString(NearRelocation32 V)
	{
		return "NearRelocation32" + (String)"(" + std::to_string(V.Value.ID) + ")";
	}
	static String ToString(NearRelocation64 V)
	{
		return "NearRelocation64" + (String)"(" + std::to_string(V.Value.ID) + ")";
	}
	static String ToStringFuncID(FuncID V)
	{
		return "FuncID" + (String)"(" + std::to_string(V) + ")";
	}
	
	
	
	
	struct Ins
	{
		//ToString helper
		// Copy $Src => $Out 
		// Swap $Src <> $Out 
		//Varable Names
		// Return value: $Return-Result$ 
		// Par# value: $Par#$ ect 
		// Called Funcion Value: $Result$
		enum class InsRegSize :Int8
		{
			bits8,
			bits16,
			bits32,
			bits64,
		};
		static String ToString(InsRegSize V)
		{
			switch (V)
			{
			case InsRegSize::bits8:return "8";
			case InsRegSize::bits16:return "16";
			case InsRegSize::bits32:return "32";
			case InsRegSize::bits64:return "64";
			default:return "n/a"; break;
			}
		}

		struct CurrintFuncOffset
		{
			size_t offset = 0;
			explicit CurrintFuncOffset(size_t offset = 0):offset(offset) {}

		};
		//Ins
		struct Removed
		{
		};
		struct NoOp
		{
			String ToString() const
			{
				return "noop";
			}
		};
		struct Call
		{
			Variant<GReg,Near32,NearRelocation32> callvariants;
			//explicit Call(FuncID Value) :callvariants(Value) {};
			explicit Call(GReg Value) :callvariants(Value) {};
			explicit Call(Near32 Value) :callvariants(Value) {};
			explicit Call(NearRelocation32 Value) :callvariants(Value) {};
			String ToString() const
			{
				String r;

				r += "call ";
				//if (auto val = callvariants.Get_If<FuncID>())
				//{
					//r += X86_64IR::ToStringFuncID(*val);
				//}
				if (auto val = callvariants.Get_If<GReg>())
				{
					r += X86_64IR::ToString(*val);
				}
				else if (auto val = callvariants.Get_If<Near32>())
				{
					r += X86_64IR::ToString(*val);
				}
				else if (auto val = callvariants.Get_If<NearRelocation32>())
				{
					r += X86_64IR::ToString(*val);
				}


				return r;
			}
		};
		struct Ret 
		{
			String ToString() const
			{
				return "ret";
			}
		};
		struct Jump 
		{ 
			Variant<FuncID, GReg, Near32, NearRelocation32> jumpvariants;
			explicit Jump(FuncID Value) :jumpvariants(Value) {};
			explicit Jump(GReg Value) :jumpvariants(Value) {};
			explicit Jump(Near32 Value) :jumpvariants(Value) {};
			explicit Jump(NearRelocation32 Value) :jumpvariants(Value) {};
			String ToString() const
			{
				String r;

				r += "jump ";
				if (auto val = jumpvariants.Get_If<FuncID>())
				{
					r += X86_64IR::ToStringFuncID(*val);
				}
				else if (auto val = jumpvariants.Get_If<GReg>())
				{
					r += X86_64IR::ToString(*val);
				}
				else if (auto val = jumpvariants.Get_If<Near32>())
				{
					r += X86_64IR::ToString(*val);
				}
				else if (auto val = jumpvariants.Get_If<NearRelocation32>())
				{
					r += X86_64IR::ToString(*val);
				}


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
					r += "mov" + Ins::ToString(RegSize) + " " + X86_64IR::ToString(Src) + " => " + X86_64IR::ToString(Out);
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
					String r;
					r += "mov" + Ins::ToString(RegSize) + " " + std::to_string(Src.AsInt64) + " => " + X86_64IR::ToString(Out);
					return r;
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
			struct RegToFuncReturn
			{
				GReg Src = GReg(GReg::Null);
				InsRegSize RegSize = InsRegSize::bits64;

				explicit RegToFuncReturn() {};
				explicit RegToFuncReturn(GReg Src) :Src(Src) {};
				explicit RegToFuncReturn(InsRegSize RegSize, GReg Src) :Src(Src), RegSize(RegSize) {};

				String ToString() const
				{
					String r;
					r += "mov" + Ins::ToString(RegSize) + " " + X86_64IR::ToString(Src) + " => " + "$Return-Result$";
					return r;
				}
			};
			struct FuncIDToReg
			{
				FuncID func;
				GReg Out = GReg::Null;

				explicit FuncIDToReg(FuncID Value, GReg Out):func(Value), Out(Out) {};
				String ToString() const
				{
					String r;
					r += "mov ";
					r += X86_64IR::ToStringFuncID(func);
					r += " => " + X86_64IR::ToString(Out);
					return r;
				}
			};
			//In
			Variant<RegToReg, ConstToReg, IndRegToReg, RegToRegInd
				,RegToFuncReturn, FuncIDToReg> MoveTypes;
			explicit Move(const RegToReg& Value) :MoveTypes(Value){}
			explicit Move(const ConstToReg& Value) :MoveTypes(Value) {}
			explicit Move(const IndRegToReg& Value) :MoveTypes(Value) {}
			explicit Move(const RegToRegInd& Value) :MoveTypes(Value) {}
			explicit Move(const RegToFuncReturn& Value) :MoveTypes(Value) {}
			String ToString() const
			{
				String r;
				if (auto val = MoveTypes.Get_If<RegToReg>())
				{
					return val->ToString();
				}
				else if (auto val = MoveTypes.Get_If<ConstToReg>())
				{
					return val->ToString();
				}
				else if (auto val = MoveTypes.Get_If<RegToFuncReturn>())
				{
					return val->ToString();
				}
				else if (auto val = MoveTypes.Get_If<FuncIDToReg>())
				{
					return val->ToString();
				}
				else
				{
					UCodeLangThrowException("not added");
				}
				return r;
			}
			inline bool Writes_Reg(Optional<GReg> Reg = {}) const
			{
				if (auto Val2 = MoveTypes.Get_If<Ins::Move::ConstToReg>())
				{
					return !Reg.has_value() || Val2->Out == Reg.value();
				}
				else if (auto Val2 = MoveTypes.Get_If<Ins::Move::RegToReg>())
				{
					return !Reg.has_value() || Val2->Out == Reg.value();
				}
				else if (auto Val2 = MoveTypes.Get_If<Ins::Move::FuncIDToReg>())
				{
					return !Reg.has_value() || Val2->Out == Reg.value();
				}
				else if (auto Val2 = MoveTypes.Get_If<Ins::Move::IndRegToReg>())
				{
					return !Reg.has_value() || Val2->Out == Reg.value();
				}
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
		};

		struct XOR
		{
			//DEST := DEST XOR SRC;
			struct RegToReg
			{
				GReg Src = GReg::Null;
				GReg Out = GReg::Null;
				InsRegSize RegSize = InsRegSize::bits64;

				explicit RegToReg() {};
				explicit RegToReg(GReg Src, GReg Out) :Src(Src), Out(Out) {};
				explicit RegToReg(InsRegSize RegSize, GReg Src, GReg Out) :Src(Src), Out(Out), RegSize(RegSize) {};
				String ToString() const
				{
					String r;
					r += "XOR" + Ins::ToString(RegSize) + " " + X86_64IR::ToString(Src) + " => " + X86_64IR::ToString(Out);
					return r;
				}
			};
			Variant<RegToReg> Xortypes;

			explicit XOR(const RegToReg& Value) :Xortypes(Value) {}
			String ToString() const
			{
				String r;
				if (auto val = Xortypes.Get_If<RegToReg>())
				{
					return val->ToString();
				}
				else
				{
					UCodeLangThrowException("not added");
				}
				return r;
			}
			inline bool Writes_Reg(Optional<GReg> Reg = {}) const
			{
				if (auto Val2 = Xortypes.Get_If<RegToReg>())
				{
					return !Reg.has_value() || Val2->Out == Reg.value();
				}
				else
				{
					return false;
				}
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
		};

		Variant<NoOp, Removed, Call, Ret, Jump,Move, XOR> variant;
		String ToString() const
		{

			if (auto val = variant.Get_If<NoOp>())
			{
				return val->ToString();
			}
			else if (auto val = variant.Get_If<Removed>())
			{
				return "[Remove Ins]";
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
			else if (auto val = variant.Get_If<Move>())
			{
				return val->ToString();
			}
			else if (auto val = variant.Get_If<XOR>())
			{
				return val->ToString();
			}
			else
			{
				UCodeLangThrowException("not added");
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
			if (auto val = variant.Get_If<Call>())
			{
				return true;
			}
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
			if (auto val = variant.Get_If<Move>())
			{
				return val->Writes_Reg(Reg);
			}
			else if (auto val = variant.Get_If<XOR>())
			{
				return val->Writes_Reg(Reg);
			}
			return false;
		}
		inline bool Reads_Reg(Optional<GReg> Reg = {}) const
		{
			if (auto val = variant.Get_If<Move>())
			{
				return val->Reads_Reg(Reg);
			}
			else if (auto val = variant.Get_If<XOR>())
			{
				return val->Reads_Reg(Reg);
			}
			return false;
		}
		inline bool Writes_Reg(Optional<IndrReg> Reg = {}) const
		{
			if (auto val = variant.Get_If<Move>())
			{
				return val->Writes_Reg(Reg);
			}
			else if (auto val = variant.Get_If<XOR>())
			{
				return val->Writes_Reg(Reg);
			}
			return false;
		}
		inline bool Reads_Reg(Optional<IndrReg> Reg = {}) const
		{
			if (auto val = variant.Get_If<Move>())
			{
				return val->Reads_Reg(Reg);
			}
			else if (auto val = variant.Get_If<XOR>())
			{
				return val->Reads_Reg(Reg);
			}
			return false;
		}
		//
		explicit Ins(const Move& Value) :variant(Value){}
		explicit Ins(const NoOp& Value) :variant(Value){}
		explicit Ins(const Call& Value) :variant(Value){}
		explicit Ins(const Ret& Value) :variant(Value) {}
		explicit Ins(const Jump& Value) :variant(Value) {}
		explicit Ins(const Removed& Value) :variant(Value) {}
		explicit Ins(const XOR& Value) :variant(Value) {}
	};
	struct Func
	{
		FuncID funcID = FuncID();
		Vector<Ins> Body;
		CallConventionID CallConvention = CallConventionID();
		Func()
		{
			static constexpr size_t InsSize = sizeof(Ins);
			static constexpr size_t MaxInsSize =32;
			//static_assert(sizeof(Ins) <= MaxInsSize, "Ins Type Is too big,this will cause jit performance problems.");
		}

		

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
			FuncID Func = FuncID();
			Vector<Byte> Bytes;
			Vector<Relocation> Relocations;
		};
		Vector<BuildFunc> Funcs;
	};
	Vector<Func> Funcs;
	UnorderedMap<CallConventionID,CallConvention> CallingConventions;
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

	String ToString() const
	{
		String r;

		for (auto& Item : Funcs)
		{
			r += ToString(Item);
		}

		return r;
	}
	String ToString(const Func& Func) const
	{
		String r;
		r += (String)"F:" + std::to_string(Func.funcID);
		r += '\n';
		for (auto& Item : Func.Body)
		{
			r += ' ' + Item.ToString();
			r += '\n';
		}
		return r;
	}

	//
	enum class CleanUpMode
	{
		None,
		CleanUpSpeed,
		RunTimeSpeed,
	};
	void CleanUp(CleanUpMode Mode);
	BuildInfo Build() const;

	struct BuildState
	{
		X86_64Builder Gen;
	};

	void Build(BuildInfo::BuildFunc& Out,BuildState& State,const Func& Value) const;
	void Build(BuildInfo::BuildFunc& Out,BuildState& State,const Ins& Value) const;

	RelocationID Get_RelocationID()
	{
		return  Next_RelocationID();
	}
private:
	CallConventionID _NextCallID = CallConventionID();
	CallConventionID Next_NextCallID()
	{
		auto r =_NextCallID;
		_NextCallID++;
		return r;
	}
	RelocationID Next_RelocationID()
	{
		auto r = _NextRelocationID;
		_NextRelocationID++;
		return r;
	}
	RelocationID _NextRelocationID = RelocationID();
};

UCodeLangEnd