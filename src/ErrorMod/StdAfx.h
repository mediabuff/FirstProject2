// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__05C29070_8F27_4DD8_98CB_E9C7937771BD__INCLUDED_)
#define AFX_STDAFX_H__05C29070_8F27_4DD8_98CB_E9C7937771BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include <atlctl.h>

#include <atlapp.h>
#include <atlmisc.h>

#import "..\UI\UI.tlb" raw_interfaces_only, raw_native_types, no_namespace, named_guids
//#import "..\WebEditor\WebEditor.tlb" raw_interfaces_only, raw_native_types, no_namespace, named_guids

#define ASSERT(x)	ATLASSERT(x)
#define TRACE	ATLTRACE

#include "..\Utils\UPtrList.h"
#include "..\Utils\UPtrArray.h"
#include "..\Utils\UString.h"
#include "..\Utils\UArray.h"
#include "..\Utils\Draw.h"
#include "..\Utils\DblPoint.h"
#include "..\Utils\Rect.h"
#include "..\Utils\IEnumfe.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Utils\\Debug\\Utils.lib")
#else
#pragma comment(lib, "..\\Utils\\Release\\Utils.lib")
#endif

#pragma comment(lib, "atl.lib")

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__05C29070_8F27_4DD8_98CB_E9C7937771BD__INCLUDED)
