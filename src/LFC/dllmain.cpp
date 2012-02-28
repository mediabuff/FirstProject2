#include "stdafx.h"
#include "LFC.h"

#if WIN32

#include <Psapi.h>
#include <Winternl.h>

#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "imagehlp.lib")

#pragma comment(lib, "Rpcrt4.lib")

typedef LONG    NTSTATUS;
typedef NTSTATUS (WINAPI *pNtQIT)(HANDLE, LONG, PVOID, ULONG, PULONG);

pNtQIT pfNtQueryInformationThread;

#endif

namespace System
{

Module _Module;

//void _AddPersistentLiveRoot(void** pp);

volatile bool loadTypes = true;
//LFCEXT bool loadTypes = true;

void init_gc();

extern "C"
{

	/*
class type_info
{
public:
	virtual ~type_info()
	{
	}

	void*_m_data;
	char _m_d_name[1];
};
*/

BOOL WINAPI RawDllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		Environment::m_hKernel32 = GetModuleHandle(L"kernel32");
		Environment::m_hNtdll = GetModuleHandle(L"Ntdll.dll");

		pfNtQueryInformationThread = (pNtQIT)GetProcAddress(Environment::m_hNtdll, "NtQueryInformationThread");

#ifdef _DEBUG
#if 1
		MessageBoxW(nullptr, L"You can now attach a debugger", L"LFC", MB_OK);
#endif
#endif

		init_gc();

		ASSERT(TypeDatabase::pD == nullptr);
		TypeDatabase::pD = new TypeDatabase;

		TypeDatabase::pD->m_debugInfo = new CVR;
		TypeDatabase::pD->m_typestuff = new TypeStuff;
		TypeDatabase::pD->m_globalNamespace = new Namespace;

		NullType::Static_Init();
		PrimitiveType::Static_Init();

		if (loadTypes)
		{
			try
			{
				_Module.Init(hModule);
			}
			catch (Exception* e)
			{
				MessageBox(nullptr, e->get_Reason().c_strw(), L"LFC", MB_OK | MB_ICONWARNING);
			// NOTE: Fall through, returning TRUE
			}
		}
	}

	return TRUE;
}

BOOL (WINAPI *_pRawDllMain)(HMODULE, DWORD, LPVOID) = RawDllMain;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			_Module.SetHandle(hModule);
		}
		break;

	case DLL_PROCESS_DETACH:
		{
			_Module.Term();
		}
		break;

	case DLL_THREAD_ATTACH:
		{
		}
		break;

	case DLL_THREAD_DETACH:
		{
		}
		break;
	}

	return TRUE;
}

}	// extern "C"
}	// System
