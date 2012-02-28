#ifndef __CODEVIEW_H__
#define __CODEVIEW_H__

// CodeView.h
namespace System
{
namespace CodeView
{

struct OMFDirEntry;

typedef struct OMFDirHeader
{
	 unsigned short  cbDirHeader;
	 unsigned short  cbDirEntry;
	 unsigned long   cDir;
	 union
	 {
		 OMFDirHeader* NextDir;
		 long            lfoNextDir;
	 };
	 unsigned long   flags;
} OMFDirHeader;

struct MemDirHeader
{
	OMFDirHeader dirheader;
	OMFDirEntry* pDirs[32];
};

struct OMFModule;
struct OMFSegMap;
struct OMFSourceModule;

typedef struct OMFDirEntry
{
	 unsigned short  SubSection;
	 unsigned short  iMod;
	 union
	 {
		 long            lfo;	// large file offset of subsection
		 OMFModule* module;
		 OMFSegMap* segmap;
		 OMFSourceModule* sourceModule;
	 };

	 unsigned long   cb;
} OMFDirEntry;

//  Type of subsection entry.

#define sstModule           0x120
#define sstTypes            0x121
#define sstPublic           0x122
#define sstPublicSym        0x123   // publics as symbol (waiting for link)
#define sstSymbols          0x124
#define sstAlignSym         0x125
#define sstSrcLnSeg         0x126   // because link doesn't emit SrcModule
#define sstSrcModule        0x127
#define sstLibraries        0x128
#define sstGlobalSym        0x129
#define sstGlobalPub        0x12a
#define sstGlobalTypes      0x12b
#define sstMPC              0x12c
#define sstSegMap           0x12d
#define sstSegName          0x12e
#define sstPreComp          0x12f   // precompiled types
#define sstPreCompMap       0x130   // map precompiled types in global types
#define sstOffsetMap16      0x131
#define sstOffsetMap32      0x132
#define sstFileIndex        0x133   // Index of file names
#define sstStaticSym        0x134

// information decribing each segment in a module

typedef struct OMFSegDesc {
unsigned short Seg; // segment index
unsigned short pad; // pad to maintain alignment
unsigned long Off; // offset of code in segment
unsigned long cbSeg; // number of bytes in segment
} OMFSegDesc;

typedef unsigned long   CV_uoff32_t;
typedef          long   CV_off32_t;
typedef unsigned short  CV_uoff16_t;
typedef          short  CV_off16_t;
//typedef unsigned short  CV_typ_t;


// per module information
// There is one of these subsection entries for each module
// in the executable. The entry is generated by link/ilink.
// This table will probably require padding because of the
// variable length module name.

typedef struct OMFModule
{
	unsigned short ovlNumber; // overlay number
	unsigned short iLib; // library that the module was linke=d from
	unsigned short cSeg; // count of number of segments in mo=dule
	char Style[2]; // debugging style "CV"
	OMFSegDesc SegInfo[1]; // describes segments in module
	union
	{
		char Name[0]; // length prefixed module name padde=d to
		char* pName;
	};
// long word boundary
} OMFModule;

/*
 * sstSegMap section
 */

typedef struct OMFSegMapDesc
{
	union
	{
		struct
		{
			unsigned short

fRead : 1,
fWrite : 1,
fExecute : 1,
f32Bit : 1,
reserved2 : 4,
fSel : 1,
fAbs : 1,
reserved1 : 2,
fGroup : 1,
reserved0 : 3;
		}
		f;
    unsigned short  flags;
	};

    unsigned short  ovl;
    unsigned short  group;
    unsigned short  frame;
    unsigned short  iSegName;
    unsigned short  iClassName;
    unsigned long   offset;
    unsigned long   cbSeg;
} OMFSegMapDesc;

typedef struct OMFSegMap
{
    unsigned short  cSeg;
    unsigned short  cSegLog;
    OMFSegMapDesc   rgDesc[0];
} OMFSegMap;

//  Symbol hash table format
//  This structure immediately preceeds the global publics table
//  and global symbol tables.

typedef struct OMFSymHash {
    unsigned short  symhash;        // symbol hash function index
    unsigned short  addrhash;       // address hash function index
    unsigned long   cbSymbol;       // length of symbol information
    unsigned long   cbHSym;         // length of symbol hash data
    unsigned long   cbHAddr;        // length of address hashdata
} OMFSymHash;

//  Global types subsection format
 //  This structure immediately preceeds the global types table.
 //  The offsets in the typeOffset array are relative to the address
 //  of ctypes.  Each type entry following the typeOffset array must
 //  begin on a long word boundary.
typedef struct OMFTypeFlags
{ 
	  unsigned long   sig     :8;
     unsigned long   unused  :24;
}
OMFTypeFlags;

typedef struct OMFGlobalTypes
{
	OMFTypeFlags    flags;
	unsigned long   cTypes;
	  // number of types
	unsigned long   typeOffset[];   // array of offsets to types
}
OMFGlobalTypes; 

//  Source line to address mapping table.
//  This table is generated by the link/ilink utility from line number
//  information contained in the object file OMF data.  This table contains
//  only the code contribution for one segment from one source file.


typedef struct OMFSourceLine {
    unsigned short  Seg;            // linker segment index
    unsigned short  cLnOff;         // count of line/offset pairs
    unsigned long   offset[1];      // array of offsets in segment
    unsigned short  lineNbr[1];     // array of line lumber in source
} OMFSourceLine;

typedef OMFSourceLine * LPSL;


//  Source file description
//  This table is generated by the linker


typedef struct OMFSourceFile {
    unsigned short  cSeg;           // number of segments from source file
    unsigned short  reserved;       // reserved
    unsigned long   baseSrcLn[1];   // base of OMFSourceLine tables
                                    // this array is followed by array
                                    // of segment start/end pairs followed by
                                    // an array of linker indices
                                    // for each segment in the file
    unsigned short  cFName;         // length of source file name
    char            Name;           // name of file padded to long boundary
} OMFSourceFile;

typedef OMFSourceFile * LPSF;


//  Source line to address mapping header structure
//  This structure describes the number and location of the
//  OMFAddrLine tables for a module.  The offSrcLine entries are
//  relative to the beginning of this structure.


typedef struct OMFSourceModule {
    unsigned short  cFile;          // number of OMFSourceTables
    unsigned short  cSeg;           // number of segments in module
    unsigned long   baseSrcFile[1]; // base of OMFSourceFile table
                                    // this array is followed by array
                                    // of segment start/end pairs followed
                                    // by an array of linker indices
                                    // for each segment in the module
} OMFSourceModule;

struct OMFFileIndex
{
	uint16 cMod;
	uint16 cRef;
};

////////

#if 0
//      Symbol definitions

typedef enum SYM_ENUM_e {
    S_COMPILE    =  0x0001, // Compile flags symbol
    S_REGISTER   =  0x0002, // Register variable
    S_CONSTANT   =  0x0003, // constant symbol
    S_UDT        =  0x0004, // User defined type
    S_SSEARCH    =  0x0005, // Start Search
    S_END        =  0x0006, // Block, procedure, "with" or thunk end
    S_SKIP       =  0x0007, // Reserve symbol space in $$Symbols table
    S_CVRESERVE  =  0x0008, // Reserved symbol for CV internal use
    S_OBJNAME    =  0x0009, // path to object file name
    S_ENDARG     =  0x000a, // end of argument/return list
    S_COBOLUDT   =  0x000b, // special UDT for cobol that does not symbol pack
    S_MANYREG    =  0x000c, // multiple register variable
    S_RETURN     =  0x000d, // return description symbol
    S_ENTRYTHIS  =  0x000e, // description of this pointer on entry

    S_BPREL16    =  0x0100, // BP-relative
    S_LDATA16    =  0x0101, // Module-local symbol
    S_GDATA16    =  0x0102, // Global data symbol
    S_PUB16      =  0x0103, // a public symbol
    S_LPROC16    =  0x0104, // Local procedure start
    S_GPROC16    =  0x0105, // Global procedure start
    S_THUNK16    =  0x0106, // Thunk Start
    S_BLOCK16    =  0x0107, // block start
    S_WITH16     =  0x0108, // with start
    S_LABEL16    =  0x0109, // code label
    S_CEXMODEL16 =  0x010a, // change execution model
    S_VFTABLE16  =  0x010b, // address of virtual function table
    S_REGREL16   =  0x010c, // register relative address

    S_BPREL32    =  0x0200, // BP-relative
    S_LDATA32    =  0x0201, // Module-local symbol
    S_GDATA32    =  0x0202, // Global data symbol
    S_PUB32      =  0x0203, // a public symbol (CV internal reserved)
    S_LPROC32    =  0x0204, // Local procedure start
    S_GPROC32    =  0x0205, // Global procedure start
    S_THUNK32    =  0x0206, // Thunk Start
    S_BLOCK32    =  0x0207, // block start
    S_WITH32     =  0x0208, // with start
    S_LABEL32    =  0x0209, // code label
    S_CEXMODEL32 =  0x020a, // change execution model
    S_VFTABLE32  =  0x020b, // address of virtual function table
    S_REGREL32   =  0x020c, // register relative address
    S_LTHREAD32  =  0x020d, // local thread storage
    S_GTHREAD32  =  0x020e, // global thread storage
    S_SLINK32    =  0x020f, // static link for MIPS EH implementation

    S_LPROCMIPS  =  0x0300, // Local procedure start
    S_GPROCMIPS  =  0x0301, // Global procedure start

    S_PROCREF    =  0x0400, // Reference to a procedure
    S_DATAREF    =  0x0401, // Reference to data
    S_ALIGN      =  0x0402, // Used for page alignment of symbols
    S_LPROCREF   =  0x0403  // Local Reference to a procedure
} SYM_ENUM_e;

typedef struct CFLAGSYM {
    unsigned short  reclen;         // Record length
    unsigned short  rectyp;         // S_COMPILE
    unsigned char   machine;        // target processor
    struct  {
        unsigned char   language    :8; // language index
        unsigned char   pcode       :1; // true if pcode present
        unsigned char   floatprec   :2; // floating precision
        unsigned char   floatpkg    :2; // float package
        unsigned char   ambdata     :3; // ambiant data model
        unsigned char   ambcode     :3; // ambiant code model
        unsigned char   mode32      :1; // true if compiled 32 bit mode
        unsigned char   pad         :4; // reserved
    } flags;
    unsigned char   ver[1];         // Length-prefixed compiler version string
} CFLAGSYM;

typedef struct OBJNAMESYM {
    unsigned short  reclen;         // Record length
    unsigned short  rectyp;         // S_OBJNAME
    unsigned long   signature;      // signature
    unsigned char   name[1];        // Length-prefixed name
} OBJNAMESYM;

typedef struct DATASYM32 {
    unsigned short  reclen;         // Record length
    unsigned short  rectyp;         // S_LDATA32, S_GDATA32 or S_PUB32
    CV_uoff32_t     off;
    unsigned short  seg;
    CV_typ_t        typind;         // Type index
    unsigned char   name[1];        // Length-prefixed name
} DATASYM32;
typedef DATASYM32 PUBSYM32;

typedef struct LABELSYM32 {
    unsigned short  reclen;         // Record length
    unsigned short  rectyp;         // S_LABEL32
    CV_uoff32_t     off;
    unsigned short  seg;
    CV_PROCFLAGS    flags;          // flags
    unsigned char   name[1];        // Length-prefixed name
} LABELSYM32;
#endif

}
}

#endif // __CODEVIEW_H__
