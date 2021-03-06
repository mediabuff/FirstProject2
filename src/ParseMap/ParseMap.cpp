// ParseMap.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#pragma comment(lib, "LFC")
#pragma comment(lib, "CodeComp")

#include "LFC/LFC.h"
#include "LFC/VCMapFileParser.h"
#include "LFC/VCMangler.h"

#include "CodeComp/CodeComp.h"

#include <cstdlib>

namespace System
{

extern "C" int main(int argc, char* argv[])
{
//	MessageBox(NULL, L"", L"", MB_OK);

	Console::_In->SetFileHandle(GetStdHandle(STD_INPUT_HANDLE));
	Console::_Out->SetFileHandle(GetStdHandle(STD_OUTPUT_HANDLE));
	Console::_Err->SetFileHandle(GetStdHandle(STD_ERROR_HANDLE));

	if (argc < 3)
	{
		return EXIT_FAILURE;
	}

	String mapfilename = argv[1];
	String typeinfodir = argv[2];

	String name = IO::Path::GetFileNameWithoutExtension(mapfilename);

	StringIn typeinfofilename(IO::Path::Combine(typeinfodir, IO::Path::ChangeExtension(name, ".typeinfo")));
	StringIn depfilename(IO::Path::Combine(typeinfodir, IO::Path::ChangeExtension(name, ".dep")));
	StringIn adfilename(IO::Path::Combine(typeinfodir, IO::Path::ChangeExtension(name, "ad")));

	if (!IO::File::Exists(adfilename) ||
		IO::File::GetLastModifiedTime(adfilename) < IO::File::GetLastModifiedTime(typeinfofilename) ||
		IO::File::GetLastModifiedTime(adfilename) < IO::File::GetLastModifiedTime(mapfilename))
	{
		ParseMap(mapfilename, typeinfofilename, adfilename);
	}
	else
	{
		Console::get_Out() << "MakeTypes - no changes to mapfile\n";
	}

	return EXIT_SUCCESS;
}

}	// System
