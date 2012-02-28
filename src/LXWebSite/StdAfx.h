// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__98D1B466_0F58_4710_B3A8_A448E5F9AE60__INCLUDED_)
#define AFX_STDAFX_H__98D1B466_0F58_4710_B3A8_A448E5F9AE60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _WIN32_IE	0x0500

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif
#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

#include <atlapp.h>

#include <comdef.h>

#include <atlwin.h>
#include <atlctl.h>
#include <atlhost.h>

#include <AtlDlgs.h>
#include <AtlCtrls.h>
#include <AtlMisc.h>

#include <direct.h>

#pragma comment(lib, "Shlwapi.lib")

#include <wininet.h>
#pragma comment(lib, "wininet.lib")

#import "C:\Program Files\Common Files\System\ado\msadox.dll"
#import "C:\Program Files\Common Files\system\ado\msado15.dll" rename("EOF", "EndOfFile")

#include "LXWebSite.h"

#import "..\LHTML\LHTML.tlb" no_namespace, named_guids, raw_interfaces_only, exclude("IServiceProvider", "IDispatchEx")

#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

#define ASSERT(x)	ATLASSERT(x)
#define TRACE	ATLTRACE

#include "..\Utils\UTempl.h"
#include "..\Utils\UPtrList.h"
#include "..\Utils\UPtrArray.h"
#include "..\Utils\UString.h"
#include "..\Utils\UArray.h"
//#include "..\Utils\Draw.h"
#include "..\Utils\IEnumfe.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Utils\\Debug\\Utils.lib")
#else
#pragma comment(lib, "..\\Utils\\Release\\Utils.lib")
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__98D1B466_0F58_4710_B3A8_A448E5F9AE60__INCLUDED)
