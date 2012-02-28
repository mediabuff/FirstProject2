//--------------------------------------------------------------------------------------
// File: DXUT.h
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma once
#ifndef DXUT_H
#define DXUT_H

#ifndef DECLSPEC_DLLEXPORT
#define DECLSPEC_DLLEXPORT __declspec(dllexport)
#define DECLSPEC_DLLIMPORT __declspec(dllimport)
#endif

#ifdef GUI_EXPORTS
#define GUIEXT DECLSPEC_DLLEXPORT
#else
#define GUIEXT DECLSPEC_DLLIMPORT
#endif

#ifndef UNICODE
#error "DXUT requires a Unicode build. See the nearby comments for details"
//
// If you are using Microsoft Visual C++ .NET, under the General tab of the project 
// properties change the Character Set to 'Use Unicode Character Set'.  
//
// Windows XP and later are native Unicode so Unicode applications will perform better.  
// For Windows 98 and Windows Me support, consider using the Microsoft Layer for Unicode (MSLU).  
//
// To use MSLU, link against a set of libraries similar to this
//      /nod:kernel32.lib /nod:advapi32.lib /nod:user32.lib /nod:gdi32.lib /nod:shell32.lib /nod:comdlg32.lib /nod:version.lib /nod:mpr.lib /nod:rasapi32.lib /nod:winmm.lib /nod:winspool.lib /nod:vfw32.lib /nod:secur32.lib /nod:oleacc.lib /nod:oledlg.lib /nod:sensapi.lib UnicoWS.lib kernel32.lib advapi32.lib user32.lib gdi32.lib shell32.lib comdlg32.lib version.lib mpr.lib rasapi32.lib winmm.lib winspool.lib vfw32.lib secur32.lib oleacc.lib oledlg.lib sensapi.lib dxerr.lib dxguid.lib d3dx9d.lib d3d9.lib comctl32.lib
// and put the unicows.dll (available for download from msdn.microsoft.com) in the exe's folder.
// 
// For more details see the MSDN article titled:
// "MSLU: Develop Unicode Applications for Windows 9x Platforms with the Microsoft Layer for Unicode"
// at http://msdn.microsoft.com/msdnmag/issues/01/10/MSLU/default.aspx 
//
#endif

#ifndef STRICT
#define STRICT
#endif

// If app hasn't choosen, set to work with Windows 98, Windows Me, Windows 2000, Windows XP and beyond
#ifndef WINVER
#define WINVER         0x0500
#endif
#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0500 
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT   0x0600
#endif

// #define DXUT_AUTOLIB to automatically include the libs needed for DXUT 
#ifdef DXUT_AUTOLIB
#pragma comment( lib, "dxerr.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3d10.lib" )
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment( lib, "d3dx9d.lib" )
#pragma comment( lib, "d3dx10d.lib" )
#else
#pragma comment( lib, "d3dx9.lib" )
#pragma comment( lib, "d3dx10.lib" )
#endif
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "comctl32.lib" )
#endif

#pragma warning( disable : 4100 ) // disable unreference formal parameter warnings for /W4 builds

// Enable extra D3D debugging in debug builds if using the debug DirectX runtime.  
// This makes D3D objects work well in the debugger watch window, but slows down 
// performance slightly.
#if defined(DEBUG) || defined(_DEBUG)
#ifndef D3D_DEBUG_INFO
#define D3D_DEBUG_INFO
#endif
#endif

// Standard Windows includes
#include <windows.h>
#include <initguid.h>
#include <assert.h>
#include <wchar.h>
#include <mmsystem.h>
#include <commctrl.h> // for InitCommonControls() 
#include <shellapi.h> // for ExtractIcon()
#include <new.h>      // for placement new
#include <shlobj.h>
#include <math.h>      
#include <limits.h>      
#include <stdio.h>

// CRT's memory leak detection
#if defined(DEBUG) || defined(_DEBUG)
#include <crtdbg.h>
#endif

// Direct3D9 includes
#include <d3d9.h>
#include <d3dx9.h>

// Direct3D10 includes
#include <dxgi.h>
#include <d3d10_1.h>
#include <d3d10.h>
#include <d3dx10.h>

// XInput includes
#include <xinput.h>

// HRESULT translation for Direct3D10 and other APIs 
#include <dxerr.h>

// strsafe.h deprecates old unsecure string functions.  If you 
// really do not want to it to (not recommended), then uncomment the next line 
//#define STRSAFE_NO_DEPRECATE

#ifndef STRSAFE_NO_DEPRECATE
#pragma deprecated("strncpy")
#pragma deprecated("wcsncpy")
#pragma deprecated("_tcsncpy")
#pragma deprecated("wcsncat")
#pragma deprecated("strncat")
#pragma deprecated("_tcsncat")
#endif

#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 ) 

#if defined(DEBUG) || defined(_DEBUG)
#ifndef V
#define V(x)           { hr = (x); if( FAILED(hr) ) { DXUTTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = (x); if( FAILED(hr) ) { return DXUTTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
#endif
#else
#ifndef V
#define V(x)           { hr = (x); }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = (x); if( FAILED(hr) ) { return hr; } }
#endif
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif



//--------------------------------------------------------------------------------------
// Structs
//--------------------------------------------------------------------------------------
struct DXUTD3D9DeviceSettings
{
    UINT AdapterOrdinal;
    D3DDEVTYPE DeviceType;
    D3DFORMAT AdapterFormat;
    DWORD BehaviorFlags;
    D3DPRESENT_PARAMETERS pp;
};

struct DXUTD3D10DeviceSettings
{
    UINT AdapterOrdinal;
    D3D10_DRIVER_TYPE DriverType;
    UINT Output;
    DXGI_SWAP_CHAIN_DESC sd;
    UINT32 CreateFlags;
    UINT32 SyncInterval;
    DWORD PresentFlags;
    bool AutoCreateDepthStencil; // DXUT will create the a depth stencil resource and view if true
    DXGI_FORMAT AutoDepthStencilFormat;
};

enum DXUTDeviceVersion { DXUT_D3D9_DEVICE, DXUT_D3D10_DEVICE };
struct DXUTDeviceSettings
{
    DXUTDeviceVersion ver;
    union
    {
        DXUTD3D9DeviceSettings d3d9; // only valid if ver == DXUT_D3D9_DEVICE
        DXUTD3D10DeviceSettings d3d10; // only valid if ver == DXUT_D3D10_DEVICE
    };
};


//--------------------------------------------------------------------------------------
// Error codes
//--------------------------------------------------------------------------------------
#define DXUTERR_NODIRECT3D              MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0901)
#define DXUTERR_NOCOMPATIBLEDEVICES     MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0902)
#define DXUTERR_MEDIANOTFOUND           MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0903)
#define DXUTERR_NONZEROREFCOUNT         MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0904)
#define DXUTERR_CREATINGDEVICE          MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0905)
#define DXUTERR_RESETTINGDEVICE         MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0906)
#define DXUTERR_CREATINGDEVICEOBJECTS   MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0907)
#define DXUTERR_RESETTINGDEVICEOBJECTS  MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0908)
#define DXUTERR_DEVICEREMOVED           MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x090A)

//--------------------------------------------------------------------------------------
// Callback registration 
//--------------------------------------------------------------------------------------

// General callbacks
typedef void    (CALLBACK *LPDXUTCALLBACKFRAMEMOVE)( double fTime, float fElapsedTime, void* pUserContext );
typedef void    (CALLBACK *LPDXUTCALLBACKKEYBOARD)( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
typedef void    (CALLBACK *LPDXUTCALLBACKMOUSE)( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos, void* pUserContext );
typedef LRESULT (CALLBACK *LPDXUTCALLBACKMSGPROC)( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext );
typedef void    (CALLBACK *LPDXUTCALLBACKTIMER)( UINT idEvent, void* pUserContext );
typedef bool    (CALLBACK *LPDXUTCALLBACKMODIFYDEVICESETTINGS)( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );
typedef bool    (CALLBACK *LPDXUTCALLBACKDEVICEREMOVED)( void* pUserContext );

// Direct3D 9 callbacks
typedef bool    (CALLBACK *LPDXUTCALLBACKISD3D9DEVICEACCEPTABLE)( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
typedef HRESULT (CALLBACK *LPDXUTCALLBACKD3D9DEVICECREATED)( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
typedef HRESULT (CALLBACK *LPDXUTCALLBACKD3D9DEVICERESET)( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
typedef void    (CALLBACK *LPDXUTCALLBACKD3D9FRAMERENDER)( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
typedef void    (CALLBACK *LPDXUTCALLBACKD3D9DEVICELOST)( void* pUserContext );
typedef void    (CALLBACK *LPDXUTCALLBACKD3D9DEVICEDESTROYED)( void* pUserContext );

// Direct3D 10 callbacks
typedef bool    (CALLBACK *LPDXUTCALLBACKISD3D10DEVICEACCEPTABLE)( UINT Adapter, UINT Output, D3D10_DRIVER_TYPE DeviceType, DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
typedef HRESULT (CALLBACK *LPDXUTCALLBACKD3D10DEVICECREATED)( ID3D10Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
typedef HRESULT (CALLBACK *LPDXUTCALLBACKD3D10SWAPCHAINRESIZED)( ID3D10Device* pd3dDevice, IDXGISwapChain *pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
typedef void    (CALLBACK *LPDXUTCALLBACKD3D10FRAMERENDER)( ID3D10Device* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
typedef void    (CALLBACK *LPDXUTCALLBACKD3D10SWAPCHAINRELEASING)( void* pUserContext );
typedef void    (CALLBACK *LPDXUTCALLBACKD3D10DEVICEDESTROYED)( void* pUserContext );

// General callbacks
GUIEXT void WINAPI DXUTSetCallbackFrameMove( LPDXUTCALLBACKFRAMEMOVE pCallback, void* pUserContext = NULL );
GUIEXT void WINAPI DXUTSetCallbackKeyboard( LPDXUTCALLBACKKEYBOARD pCallback, void* pUserContext = NULL );
GUIEXT void WINAPI DXUTSetCallbackMouse( LPDXUTCALLBACKMOUSE pCallback, bool bIncludeMouseMove = false, void* pUserContext = NULL );
GUIEXT void WINAPI DXUTSetCallbackMsgProc( LPDXUTCALLBACKMSGPROC pCallback, void* pUserContext = NULL );
GUIEXT void WINAPI DXUTSetCallbackDeviceChanging( LPDXUTCALLBACKMODIFYDEVICESETTINGS pCallback, void* pUserContext = NULL );
GUIEXT void WINAPI DXUTSetCallbackDeviceRemoved( LPDXUTCALLBACKDEVICEREMOVED pCallback, void* pUserContext = NULL );

// Direct3D 9 callbacks
GUIEXT void WINAPI DXUTSetCallbackD3D9DeviceAcceptable( LPDXUTCALLBACKISD3D9DEVICEACCEPTABLE pCallback, void* pUserContext = NULL );
GUIEXT void WINAPI DXUTSetCallbackD3D9DeviceCreated( LPDXUTCALLBACKD3D9DEVICECREATED pCallback, void* pUserContext = NULL );
GUIEXT void WINAPI DXUTSetCallbackD3D9DeviceReset( LPDXUTCALLBACKD3D9DEVICERESET pCallback, void* pUserContext = NULL );
GUIEXT void WINAPI DXUTSetCallbackD3D9FrameRender( LPDXUTCALLBACKD3D9FRAMERENDER pCallback, void* pUserContext = NULL );
GUIEXT void WINAPI DXUTSetCallbackD3D9DeviceLost( LPDXUTCALLBACKD3D9DEVICELOST pCallback, void* pUserContext = NULL );
GUIEXT void WINAPI DXUTSetCallbackD3D9DeviceDestroyed( LPDXUTCALLBACKD3D9DEVICEDESTROYED pCallback, void* pUserContext = NULL );

// Direct3D 10 callbacks
GUIEXT void WINAPI DXUTSetCallbackD3D10DeviceAcceptable( LPDXUTCALLBACKISD3D10DEVICEACCEPTABLE pCallback, void* pUserContext = NULL );
GUIEXT void WINAPI DXUTSetCallbackD3D10DeviceCreated( LPDXUTCALLBACKD3D10DEVICECREATED pCallback, void* pUserContext = NULL );
GUIEXT void WINAPI DXUTSetCallbackD3D10SwapChainResized( LPDXUTCALLBACKD3D10SWAPCHAINRESIZED pCallback, void* pUserContext = NULL );
GUIEXT void WINAPI DXUTSetCallbackD3D10FrameRender( LPDXUTCALLBACKD3D10FRAMERENDER pCallback, void* pUserContext = NULL );
GUIEXT void WINAPI DXUTSetCallbackD3D10SwapChainReleasing( LPDXUTCALLBACKD3D10SWAPCHAINRELEASING pCallback, void* pUserContext = NULL );
GUIEXT void WINAPI DXUTSetCallbackD3D10DeviceDestroyed( LPDXUTCALLBACKD3D10DEVICEDESTROYED pCallback, void* pUserContext = NULL );


//--------------------------------------------------------------------------------------
// Initialization
//--------------------------------------------------------------------------------------
GUIEXT HRESULT WINAPI DXUTInit( bool bParseCommandLine = true, bool bShowMsgBoxOnError = true, WCHAR* strExtraCommandLineParams = NULL, bool bThreadSafeDXUT = false );

// Choose either DXUTCreateWindow or DXUTSetWindow.  If using DXUTSetWindow, consider using DXUTStaticWndProc
GUIEXT HRESULT WINAPI DXUTCreateWindow( const WCHAR* strWindowTitle = L"Direct3D Window", 
                          HINSTANCE hInstance = NULL, HICON hIcon = NULL, HMENU hMenu = NULL,
                          int x = CW_USEDEFAULT, int y = CW_USEDEFAULT );
GUIEXT HRESULT WINAPI DXUTSetWindow( HWND hWndFocus, HWND hWndDeviceFullScreen, HWND hWndDeviceWindowed, bool bHandleMessages = true );
GUIEXT LRESULT CALLBACK DXUTStaticWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

// Choose either DXUTCreateDevice or DXUTSetD3D*Device or DXUTCreateD3DDeviceFromSettings
GUIEXT HRESULT WINAPI DXUTCreateDevice( bool bWindowed = true, int nSuggestedWidth = 0, int nSuggestedHeight = 0 );
GUIEXT HRESULT WINAPI DXUTCreateDeviceFromSettings( DXUTDeviceSettings* pDeviceSettings, bool bPreserveInput = false, bool bClipWindowToSingleAdapter = true );
GUIEXT HRESULT WINAPI DXUTSetD3D9Device( IDirect3DDevice9* pd3dDevice );
GUIEXT HRESULT WINAPI DXUTSetD3D10Device( ID3D10Device* pd3dDevice, IDXGISwapChain* pSwapChain );

// Choose either DXUTMainLoop or implement your own main loop 
GUIEXT HRESULT WINAPI DXUTMainLoop( HACCEL hAccel = NULL );

// If not using DXUTMainLoop consider using DXUTRender3DEnvironment
GUIEXT void WINAPI DXUTRender3DEnvironment(); 


//--------------------------------------------------------------------------------------
// Common Tasks 
//--------------------------------------------------------------------------------------
GUIEXT HRESULT WINAPI DXUTToggleFullScreen();
GUIEXT HRESULT WINAPI DXUTToggleREF();
GUIEXT void    WINAPI DXUTPause( bool bPauseTime, bool bPauseRendering );
GUIEXT void    WINAPI DXUTSetConstantFrameTime( bool bConstantFrameTime, float fTimePerFrame = 0.0333f );
GUIEXT void    WINAPI DXUTSetCursorSettings( bool bShowCursorWhenFullScreen = false, bool bClipCursorWhenFullScreen = false );
GUIEXT void    WINAPI DXUTSetD3DVersionSupport( bool bAppCanUseD3D9 = true, bool bAppCanUseD3D10 = true );
GUIEXT void    WINAPI DXUTSetHotkeyHandling( bool bAltEnterToToggleFullscreen = true, bool bEscapeToQuit = true, bool bPauseToToggleTimePause = true );
GUIEXT void    WINAPI DXUTSetMultimonSettings( bool bAutoChangeAdapter = true );
GUIEXT void    WINAPI DXUTSetShortcutKeySettings( bool bAllowWhenFullscreen = false, bool bAllowWhenWindowed = true ); // Controls the Windows key, and accessibility shortcut keys
GUIEXT void    WINAPI DXUTSetWindowSettings( bool bCallDefWindowProc = true );
GUIEXT HRESULT WINAPI DXUTSetTimer( LPDXUTCALLBACKTIMER pCallbackTimer, float fTimeoutInSecs = 1.0f, UINT* pnIDEvent = NULL, void* pCallbackUserContext = NULL );
GUIEXT HRESULT WINAPI DXUTKillTimer( UINT nIDEvent );
GUIEXT void    WINAPI DXUTResetFrameworkState();
GUIEXT void    WINAPI DXUTShutdown( int nExitCode = 0 );
GUIEXT void    WINAPI DXUTSetIsInGammaCorrectMode( bool bGammaCorrect );


//--------------------------------------------------------------------------------------
// State Retrieval  
//--------------------------------------------------------------------------------------

// Direct3D 9
GUIEXT IDirect3D9*              WINAPI DXUTGetD3D9Object(); // Does not addref unlike typical Get* APIs
GUIEXT IDirect3DDevice9*        WINAPI DXUTGetD3D9Device(); // Does not addref unlike typical Get* APIs
GUIEXT D3DPRESENT_PARAMETERS    WINAPI DXUTGetD3D9PresentParameters();
GUIEXT const D3DSURFACE_DESC*   WINAPI DXUTGetD3D9BackBufferSurfaceDesc();
GUIEXT const D3DCAPS9*          WINAPI DXUTGetD3D9DeviceCaps();
GUIEXT HRESULT                  WINAPI DXUTGetD3D9DeviceCaps( DXUTDeviceSettings* pDeviceSettings, D3DCAPS9* pCaps );
GUIEXT bool                     WINAPI DXUTDoesAppSupportD3D9();
GUIEXT bool                     WINAPI DXUTIsAppRenderingWithD3D9();

// Direct3D 10
GUIEXT bool                     WINAPI DXUTIsD3D10Available(); // If D3D10 APIs are availible
GUIEXT IDXGIFactory*            WINAPI DXUTGetDXGIFactory(); // Does not addref unlike typical Get* APIs
GUIEXT ID3D10Device*            WINAPI DXUTGetD3D10Device(); // Does not addref unlike typical Get* APIs
GUIEXT ID3D10Device1*           WINAPI DXUTGetD3D10Device1(); // Does not addref unlike typical Get* APIs
GUIEXT IDXGISwapChain*          WINAPI DXUTGetDXGISwapChain(); // Does not addref unlike typical Get* APIs
GUIEXT ID3D10RenderTargetView*  WINAPI DXUTGetD3D10RenderTargetView(); // Does not addref unlike typical Get* APIs
GUIEXT ID3D10DepthStencilView*  WINAPI DXUTGetD3D10DepthStencilView(); // Does not addref unlike typical Get* APIs
GUIEXT const DXGI_SURFACE_DESC* WINAPI DXUTGetDXGIBackBufferSurfaceDesc();
GUIEXT bool                     WINAPI DXUTDoesAppSupportD3D10();
GUIEXT bool                     WINAPI DXUTIsAppRenderingWithD3D10();

// General
GUIEXT DXUTDeviceSettings WINAPI DXUTGetDeviceSettings(); 
GUIEXT HINSTANCE WINAPI DXUTGetHINSTANCE();
GUIEXT HWND      WINAPI DXUTGetHWND();
GUIEXT HWND      WINAPI DXUTGetHWNDFocus();
GUIEXT HWND      WINAPI DXUTGetHWNDDeviceFullScreen();
GUIEXT HWND      WINAPI DXUTGetHWNDDeviceWindowed();
GUIEXT RECT      WINAPI DXUTGetWindowClientRect();
GUIEXT LONG      WINAPI DXUTGetWindowWidth();
GUIEXT LONG      WINAPI DXUTGetWindowHeight();
GUIEXT RECT      WINAPI DXUTGetWindowClientRectAtModeChange(); // Useful for returning to windowed mode with the same resolution as before toggle to full screen mode
GUIEXT RECT      WINAPI DXUTGetFullsceenClientRectAtModeChange(); // Useful for returning to full screen mode with the same resolution as before toggle to windowed mode
GUIEXT double    WINAPI DXUTGetTime();
GUIEXT float     WINAPI DXUTGetElapsedTime();
GUIEXT bool      WINAPI DXUTIsWindowed();
GUIEXT bool	  WINAPI DXUTIsInGammaCorrectMode();
GUIEXT float     WINAPI DXUTGetFPS();
GUIEXT LPCWSTR   WINAPI DXUTGetWindowTitle();
GUIEXT LPCWSTR   WINAPI DXUTGetFrameStats( bool bIncludeFPS = false );
GUIEXT LPCWSTR   WINAPI DXUTGetDeviceStats();
GUIEXT LPCWSTR   WINAPI DXUTGetD3D10CounterStats();
GUIEXT bool      WINAPI DXUTIsVsyncEnabled();
GUIEXT bool      WINAPI DXUTIsRenderingPaused();
GUIEXT bool      WINAPI DXUTIsTimePaused();
GUIEXT bool      WINAPI DXUTIsActive();
GUIEXT int       WINAPI DXUTGetExitCode();
GUIEXT bool      WINAPI DXUTGetShowMsgBoxOnError();
GUIEXT bool      WINAPI DXUTGetAutomation();  // Returns true if -automation parameter is used to launch the app
GUIEXT bool      WINAPI DXUTIsKeyDown( BYTE vKey ); // Pass a virtual-key code, ex. VK_F1, 'A', VK_RETURN, VK_LSHIFT, etc
GUIEXT bool      WINAPI DXUTWasKeyPressed( BYTE vKey );  // Like DXUTIsKeyDown() but return true only if the key was just pressed
GUIEXT bool      WINAPI DXUTIsMouseButtonDown( BYTE vButton ); // Pass a virtual-key code: VK_LBUTTON, VK_RBUTTON, VK_MBUTTON, VK_XBUTTON1, VK_XBUTTON2
GUIEXT HRESULT   WINAPI DXUTCreateState(); // Optional method to create DXUT's memory.  If its not called by the application it will be automatically called when needed
GUIEXT void      WINAPI DXUTDestroyState(); // Optional method to destroy DXUT's memory.  If its not called by the application it will be automatically called after the application exits WinMain 

//--------------------------------------------------------------------------------------
// DXUT core layer includes
//--------------------------------------------------------------------------------------
#include "DXUTmisc.h"
#include "DXUTenum.h"


#endif



