// MakeTypes.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MakeTypes.h"

#pragma comment(lib, "LFC")
#pragma comment(lib, "CodeComp")

namespace System
{

class Init
{
public:
	CTOR Init();
};

Module _Module;

Init::Init()
{
//	MessageBox(NULL, L"", L"", MB_OK);

	HMODULE hModule = GetModuleHandle(nullptr);

	try
	{
		_Module.Init(hModule);
	}
	catch (Exception* e)
	{
		String reason = e->get_Reason();
		AllocConsole();
		Std::_In->SetFileHandle(GetStdHandle(STD_INPUT_HANDLE));
		Std::_Out->SetFileHandle(GetStdHandle(STD_OUTPUT_HANDLE));
		Std::_Err->SetFileHandle(GetStdHandle(STD_ERROR_HANDLE));

		Std::get_Out() << e;
	}
}

// Must have this before other statics that depend on Typeinfo
Init init;

/*
bool GetFileTime(const char* filename, FILETIME* lpCreationTime, FILETIME* lpLastAccessTime, FILETIME* lpLastWriteTime)
{
	HANDLE hFile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		IO::StringWriter str;
		str << "file not found " << filename;
		raise(Exception(str.str()));
	}

	BOOL success = GetFileTime(hFile, lpCreationTime, lpLastAccessTime, lpLastWriteTime);
	CloseHandle(hFile);
	return success != 0;
}
*/

extern "C" int main(int argc, char* argv[])
{
	Std::_In->SetFileHandle(GetStdHandle(STD_INPUT_HANDLE));
	Std::_Out->SetFileHandle(GetStdHandle(STD_OUTPUT_HANDLE));
	Std::_Err->SetFileHandle(GetStdHandle(STD_ERROR_HANDLE));

	try
	{
		Std::Out << "MakeTypeInfo...\n";

		if (argc < 5)
		{
			Std::Out << "Too few arguments\n";
			for (int i = 0; i < argc; ++i)
			{
				Std::Out << argv[i] << "\n";
			}

			Std::Out << "MakeTypes %headerfile %typeinfodir %intdir %outdir\n";

			return EXIT_FAILURE;
		}

		FilePath headerfile = argv[1];
		FilePath typeinfodir = argv[2];
		FilePath intdir = argv[3];
		FilePath outdir = argv[4];
		String name = headerfile.get_Name();

		cpp::CGlobal* pGlobal = new cpp::CGlobal;

		char dirname[260];
		char pathname[260];
		char filename[260];
		char extname[260];
		_splitpath_s(argv[2], dirname, pathname, filename, extname);

		/*
		{
			IO::StreamWriter f(new IO::FileStream(outdir + Filename(name, "result"), IO::FileMode_Create, IO::FileAccess_Write));
			f << "done\n";
		}
		*/

		bool bDo = false;

		StringIn typeinfofilename(typeinfodir + Filename(name, "typeinfo"));
		StringIn depfilename(typeinfodir + Filename(name, "dep"));
		StringIn mapfilename(intdir + Filename(name, "map"));
		StringIn adfilename(outdir + Filename(name, "ad"));

		if (IO::File::Exists(typeinfofilename))
		{
			FILETIME outputfile_time = IO::File::GetLastModifiedTime(typeinfofilename);

			try
			{
				IO::StreamReader f(new IO::FileStream(depfilename, IO::FileMode_Open, IO::FileAccess_Read));
			//	if (f.IsHandleValid())
				{
					while (1)//f.EndOfStream())
					{
						String line = f.ReadLine();
						if (line.GetLength() == 0)
						{
							break;
						}

					//	FILETIME ftime1;

					//	char name[512];
						//SYSTEMTIME systime = {0};
					//	sscanf_s(CString(line), "%s %hu.%hu.%hu.%hu.%hu.%hu", name, 512, &systime.wDay, &systime.wMonth, &systime.wYear, &systime.wHour, &systime.wMinute, &systime.wSecond);

					//	SystemTimeToFileTime(&systime, &ftime1);

						if (IO::File::Exists(line))
						{
							FILETIME inputfile_time = IO::File::GetLastModifiedTime(line);

							if (outputfile_time < inputfile_time)
							{
								bDo = true;
								break;
							}
						}
						else
						{
							Std::Out << "cf " << line << endl;
						}
					}
				}
			}
			_catch (IO::IOException, e)
			{
				UNREFERENCED_PARAMETER(e);
				bDo = true;
			}
		}
		else
			bDo = true;

		if (bDo)
		{
			int err = MakeTypes(headerfile.ToString(), typeinfofilename);
			if (err) return err;
			bDo = true;
		}
		else
		{
			IO::File::Touch(typeinfofilename, IO::FileTime::Now());
			Std::get_Out() << "MakeTypes - no changes to typeinfo\n";
		}

		/*
		if (!IO::File::Exists(adfilename) ||
			IO::File::GetLastModifiedTime(adfilename) < IO::File::GetLastModifiedTime(typeinfofilename) ||
			IO::File::GetLastModifiedTime(adfilename) < IO::File::GetLastModifiedTime(mapfilename))
		{
			ParseMap(mapfilename, typeinfofilename, adfilename);
		}
		else
		{
			Std::get_Out() << "MakeTypes - no changes to mapfile\n";
		}
		*/
	}
	catch (Exception* e)
	{
		Std::get_Err() << e->get_Reason();
		return EXIT_FAILURE;
	}

	/*
#ifdef _DEBUG
	{
		multimap<String, Declarator*> h;
		ClassType* p = h.m_head->GetType();
		if (p)
		{
			size_t sz0 = sizeof(multimap<String, Declarator*>::Node);
			Type* pType = typeid(multimap<String, Declarator*>::Node);
			size_t sz1 = pType->GetClass()->get_sizeof();
			VERIFY(sz0 == sz1);
		}
	}
#endif
	*/

	return EXIT_SUCCESS;
}

}	// System
