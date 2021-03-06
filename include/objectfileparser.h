#ifndef __OBJECTFILEPARSER_H_
#define __OBJECTFILEPARSER_H_

//#include <strstream>
//#include "Stream.h"

/*
#ifndef CodeCompExt
#define CodeCompExt DECLSPEC_DLLIMPORT
#endif
*/

#include "LFC/Debug.h"

namespace System
{

#define CPU_UNKNOWN		0
#define CPU_M68K		1
#define CPU_IX86		2
#define CPU_PPC			3
#define CPU_SPARC		4
#define CPU_JAVA_IL		(8+1)
#define CPU_NET_IL		(8+2)
#define CPU_FLASH2_IL	(8+3)
#define CPU_FLASH3_IL	(8+4)

#define FORMAT_UNKNOWN	0
#define FORMAT_AOUT		1
#define FORMAT_COFF		2
#define FORMAT_HUNK		3
#define FORMAT_ELF		4

/*
class CSymbol
{
public:
	virtual int Release() = 0;
	virtual char* GetName() = 0;
};
*/

struct ObjectSymbol
{
	char    *n_name;
	unsigned char   n_type;
	char            n_other;
	short           n_desc;
	unsigned long   n_value;
};

struct Relocation
{
	int r_address;
	int r_symbolnum;
	bool r_extern : 1;
	bool r_pcrel : 1;
};

struct ImageSection {
    //BYTE    Name[IMAGE_SIZEOF_SHORT_NAME];
    uint32   VirtualSize;
    uint32   VirtualAddress;
    uint32   SizeOfRawData;
    uint32   PointerToRawData;
    uint32   PointerToRelocations;
 //   uint32   PointerToLinenumbers;
 //   WORD    NumberOfRelocations;
 //   WORD    NumberOfLinenumbers;
 //   uint32   Characteristics;
};

struct SECTION
{
	ImageSection sectionHeader;
	char* name;
};

class LFCEXT SymbolFile : public Object
{
public:
	virtual ~SymbolFile()
	{
	}

	virtual long Read(IO::Stream* stream) = 0;

	virtual uint GetNumberOfSymbols()
	{
		return 0;
	}

	virtual String GetErrorMsg(long nError)
	{
		/*
		if (nError < 0)
		{
			std::strstream string;
			string << "Error " << nError;
			return std::string(string.rdbuf()->str());
		}
		else
		*/
			return nullptr;
	}
};

class LFCEXT ObjectFileParser : public SymbolFile
{
public:
//	virtual int Release() = 0;

	virtual long GetFormat() = 0;
	virtual long GetCPU() = 0;

//	virtual BYTE* GetText() = 0;
//	virtual ULONG GetTextSize() = 0;

	virtual uint GetNumberOfSections()
	{
		return 0;
	}

	virtual int GetSection(uint nSection, SECTION* pSection)
	{
		return -1;
	}

	virtual uint8* GetData(uint nSection)
	{
		return NULL;
	}

	virtual ULONG GetDataSize(uint nSection)
	{
		return 0;
	}

	virtual ObjectSymbol* GetSymbol(uint index)
	{
		return NULL;
	}

	virtual uint GetNumberOfRelocations(uint nSection)
	{
		return 0;
	}

	virtual Relocation* GetRelocation(uint nSection, int index)
	{
		return NULL;
	}

	virtual DebugInfo* GetDebugInfo(System::Scope* pGlobalScope)
	{
		return NULL;
	}
};

}	// System

#endif	// __OBJECTFILEPARSER_H_
