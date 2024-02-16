#include "../LangTypes.hpp"
#include "../Version.hpp"

UCodeLangStart

struct Version
{
	UInt32 Major = 0;
	UInt32 Minor = 0;
	UInt32 Patch = 0;

	constexpr Version(UInt32 major,UInt32 minor,UInt32 patch)
		:Major(major),Minor(minor),Patch(patch)
	{
	}
	constexpr Version()
	{

	}

	static constexpr Version CurrentUCodeVersion()
	{
		return Version(UCodeLangMajorVersion, UCodeLangMinorVersion, UCodeLangPatchVersion);
	}


	bool operator==(const Version& other) const
	{
		return Major == other.Major && Minor == other.Minor && Patch == other.Patch;
	}
	bool operator!=(const Version& other) const
	{
		return !this->operator==(other);
	}
	
	bool operator>(const Version& other) const
	{
		if (Major > other.Major)
		{
			return true;
		}
		if (Minor > other.Minor)
		{
			return true;
		}	
		if (Patch > other.Patch)
		{
			return true;
		}
	
		return false;
	}

	bool operator<(const Version& other) const
	{
		if (Major < other.Major)
		{
			return true;
		}
		if (Minor < other.Minor)
		{
			return true;
		}	
		if (Patch < other.Patch)
		{
			return true;
		}
	
		return false;
	}

	bool operator<=(const Version& other) const
	{
		return this->operator==(other) || this->operator<(other);
	}
	bool operator>=(const Version& other) const
	{
		return this->operator==(other) || this->operator>(other);
	}

	String ToString() const
	{
		return std::to_string(Major) + "." + std::to_string(Minor) + "." + std::to_string(Patch);
	}

	static Optional<Version> Parse(const String_view str);
};

UCodeLangEnd
