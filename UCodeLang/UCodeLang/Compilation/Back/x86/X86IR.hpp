#pragma once
#include "../NativeCodeBuilder.hpp"

UCodeLangStart

struct X86IR
{
	X86IR()
	{

	}
	~X86IR()
	{

	}
	enum class CleanUpMode
	{
		None,
		CleanUpSpeed,
		RunTimeSpeed,
	};

	using IDType = UInt64;
	using FuncID = IDType;
	using CallConventionID = IDType;
	using RelocationID = IDType;
	struct CallConvention
	{
		/*
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
		*/
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
		RelocationID RelocationId = 0;
		size_t ByteToUpdateOffset = 0;
		RelocationType Type = RelocationType::Size32;
	};
	void CleanUp(CleanUpMode Mode);
};

UCodeLangEnd