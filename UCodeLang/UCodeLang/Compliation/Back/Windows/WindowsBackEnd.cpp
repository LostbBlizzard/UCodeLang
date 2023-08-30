#include "WindowsBackEnd.hpp"
#include "../x86_64/IRToX86_64IR.hpp"
#include "PE_File.hpp"
UCodeLangStart
void WindowsBackEnd::Reset()
{
	this->~WindowsBackEnd();
	new (this)  WindowsBackEnd;
}

void WindowsBackEnd::Build(const IRBuilder* Input)
{
	/*
	auto x8664_ir = IRToX86_64IR::Into(*Input);
	auto code = x8664_ir.Build();

	
	using namespace enma;
	using namespace std;

	
	enma::pe_image image = enma::pe_image();

	image.set_sub_system(IMAGE_SUBSYSTEM_WINDOWS_CUI);
	image.set_machine(IMAGE_FILE_MACHINE_AMD64);
	image.set_characteristics(IMAGE_FILE_EXECUTABLE_IMAGE);

	image.set_base_of_code(0x1000);
	image.set_image_base(0x140000000);

	image.set_major_linker(UCodeLangMajorVersion);
	image.set_minor_linker(UCodeLangMinorVersion);


	image.set_os_ver_major(6);//Windows Server 2008
	image.set_os_ver_minor(0);

	image.set_subsystem_ver_major(6);
	image.set_subsystem_ver_minor(0);

	image.set_image_ver_major(1);
	image.set_image_ver_minor(0);

	image.set_entry_point(0);

	image.set_file_align(0x200);
	image.set_section_align(0x1000);

	image.set_heap_commit_size(0x1000);
	image.set_heap_reserve_size(0x100000);

	image.set_stack_commit_size(0x1000);
	image.set_stack_reserve_size(0x100000);

	image.set_characteristics_dll(IMAGE_DLLCHARACTERISTICS_NX_COMPAT
		| IMAGE_DLLCHARACTERISTICS_HIGH_ENTROPY_VA
		| IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE
		| IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE
	);

	bool Isuseing_ucrtbasedDll = true;
	bool Isuseing_kernel32Dll = true;

	

	std::vector<uint8_t> bytes_of_entry{ 0xCC };


	image.set_size_of_code(bytes_of_entry.size());
	pe_import_directory imports;
	if (Isuseing_kernel32Dll) 
	{
		#if UCodeLang_Platform_Windows
		Optional<pe_image> nativelib(L"C:\\Windows\\System32\\kernel32.dll");
		#else 
		Optional<pe_image> nativelib;
		#endif
		pe_import_library lib;
		lib.set_library_name("kernel32.dll");

		imports.add_library(lib);
	}
	if (Isuseing_ucrtbasedDll)
	{
		#if UCodeLang_Platform_Windows
		Optional<pe_image> nativelib(L"C:\\Windows\\System32\\ucrtbased.dll");
		pe_export_directory exports;
		if (get_export_directory(image, exports) == pe_directory_code::pe_directory_code_success) 
		{

		}
		#else 
		Optional<pe_image> nativelib;
		pe_export_directory exports;
		#endif
		pe_import_library lib;
		lib.set_library_name("ucrtbased.dll");

		bool isfree_used = true;
		if (isfree_used)
		{
			pe_import_function f;
			f.set_func_name("free");
			lib.add_function(f);
		}

		bool ismalloc_used = true;
		if (ismalloc_used)
		{
			pe_import_function f;
			f.set_func_name("malloc");
			lib.add_function(f);
		}

		bool ismemccpy_used = true;
		if (ismemccpy_used)
		{
			pe_import_function f;
			f.set_func_name("memccpy");
			lib.add_function(f);
		}
	}

	

	auto& section = image.add_section();
	section.set_section_name("text");
	section.set_executable(true);
	section.set_readable(true);
	
	section.set_virtual_address(140);
	section.add_data(bytes_of_entry.data(), bytes_of_entry.size());

	if (imports.size())
	{
		auto& idatasection = image.add_section();
		idatasection.set_section_name("idata");
		idatasection.set_executable(true);
		idatasection.set_readable(true);
		

		idatasection.add_data(bytes_of_entry.data(), bytes_of_entry.size());
		//build_import_directory_only(image, idatasection, imports);
	}
	
	

	Vector<Byte> Out;
	build_pe_image(image, PE_IMAGE_BUILD_ALL_DIRECTORIES, Out);
	


	Set_Output(BytesView::Make(Out.data(), Out.size()));
	*/
}
UCodeLangEnd