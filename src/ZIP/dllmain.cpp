// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "ZIP.h"

#pragma comment(lib, "LFC")
#pragma comment(lib, "ZLIBDeflate")

namespace System
{
Module _Module;

extern "C"
{

BOOL WINAPI RawDllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		try
		{
			_Module.Init(hModule);
		}
		_catch (Exception, e)
		{
			MessageBox(NULL, CStringw(e.get_Reason()), L"ZIP", MB_OK | MB_ICONERROR);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL (WINAPI *_pRawDllMain)(HMODULE, DWORD, LPVOID) = RawDllMain;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	return _Module.DllMain(hModule, ul_reason_for_call, lpReserved);
}

}	// extern "C"
}
