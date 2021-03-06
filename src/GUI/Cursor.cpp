#include "stdafx.h"
#include "GUI2.h"

namespace System
{
namespace Gui
{

IMP_DEPENDENCY_PROPERTY(Visual*, Cursor, Image, nullptr)
IMP_DEPENDENCY_PROPERTY(float, Cursor, HotspotX, 0.0f)
IMP_DEPENDENCY_PROPERTY(float, Cursor, HotspotY, 0.0f)

DependencyClass* Cursor::get_Class()
{
	static DependencyClass depclass(typeid(thisClass), baseClass::get_Class());

	static DependencyProperty* properties[] =
	{
		get_HotspotXProperty(),
		get_HotspotYProperty(),
		get_ImageProperty(),
	};

	return &depclass;
}

DependencyClass* Cursor::pClass(get_Class());

Cursor::Cursor() : DependencyObject(get_Class())
{
}

Cursor::Cursor(HCURSOR hCursor) : DependencyObject(get_Class())
{
//	if (Win32API::
#if 0	// Vista
	ICONINFOEX iconinfo;
	iconinfo.cbSize = sizeof(iconinfo);
	GetIconInfoEx(hCursor, &iconinfo);
#else
	ICONINFO iconinfo;
	GetIconInfo(hCursor, &iconinfo);
#endif

	set_HotspotX(iconinfo.xHotspot);
	set_HotspotY(iconinfo.yHotspot);

	DeleteObject(iconinfo.hbmMask);
	DeleteObject(iconinfo.hbmColor);

	m_hCursor = hCursor;
}

void Cursor::OnComputedPropertyValueChanged(PropertyValue* pPropertyVal, bool handled)
{
	if (pPropertyVal->m_dp == get_ImageProperty() ||
		pPropertyVal->m_dp == get_HotspotXProperty() ||
		pPropertyVal->m_dp == get_HotspotYProperty())
	{
#if _WIN32
		if (m_hCursor)
		{
			DestroyIcon(m_hCursor);
			m_hCursor = NULL;
		}
#else
#error Unknown OS
#endif
	}

	baseClass::OnComputedPropertyValueChanged(pPropertyVal, handled);
}

#if _WIN32
HCURSOR Cursor::GetHCursor()
{
	if (m_hCursor == NULL)
	{
		Visual* element = get_Image();
		if (element)
		{
			float hotspotX = get_HotspotX();
			float hotspotY = get_HotspotY();

			m_hCursor = element->CreateHCursor(hotspotX, hotspotY);
		}
	}

	return m_hCursor;
}

HCURSOR Visual::CreateHCursor(float hotspotX, float hotspotY)
{
	BITMAPV5HEADER bi;
	HBITMAP hBitmap;
	void *lpBits;

	HCURSOR hAlphaCursor = NULL;
	
#if 0
	m_Visual->Measure(LDraw::SizeD(0, 0));
	m_Visual->Arrange(LDraw::RectD(LDraw::PointD(0, 0), m_Visual->get_DesiredSize()));
#endif

	int width  = 32;//m_Visual->m_expandedBBox.Width;
	int height = 32;//m_Visual->m_expandedBBox.Height;
	
	ZeroMemory(&bi, sizeof(BITMAPV5HEADER));
	bi.bV5Size = sizeof(BITMAPV5HEADER);
	bi.bV5Width = width;
	bi.bV5Height = height;
	bi.bV5Planes = 1;
	bi.bV5BitCount = 32;
	bi.bV5Compression = BI_BITFIELDS;
	// The following mask specification specifies a supported 32 BPP
	// alpha format for Windows XP.
	bi.bV5RedMask   =  0x00FF0000;
	bi.bV5GreenMask =  0x0000FF00;
	bi.bV5BlueMask  =  0x000000FF;
	bi.bV5AlphaMask =  0xFF000000;
	
	// Create the DIB section with an alpha channel.
	hBitmap = CreateDIBSection(NULL, (BITMAPINFO*)&bi, DIB_RGB_COLORS, 
		(void **)&lpBits, NULL, (DWORD)0);
	
	ManagedRenderContext renderContext(NULL);
	Graphics::Bitmap* bitmap = GetBitmap(renderContext.m_p->GetRT(), width, height, 1, 1);

#if 0
	{
		LDraw::D3D10Texture2D texture(GL::Direct10::get_D3DDevice(), width, height, LDraw::PixelFormat_t::RGBAP_32);
		LDraw::Bitmap bitmap(GL::Direct10::get_D3DDevice(), texture);
		{
			RenderBitmap(&bitmap);

			/*
			Graphics* graphics = new Graphics(new LDraw::GraphicsO(bitmap));
			graphics->TranslateTransform(-m_Visual->m_expandedBBox.X, -m_Visual->m_expandedBBox.Y);
			m_Visual->Render(graphics);
			*/
		}
	}
#endif

	// Create an empty mask bitmap.
	HBITMAP hMonoBitmap = CreateBitmap(width, height,1,1,NULL);

	ICONINFO ii;
	ii.fIcon = false;
	ii.xHotspot = hotspotX - 0;//m_Visual->m_expandedBBox.X;
	ii.yHotspot = hotspotY - 0;//m_Visual->m_expandedBBox.Y;
	ii.hbmMask = hMonoBitmap;
	ii.hbmColor = hBitmap;
	
	// Create the alpha cursor with the alpha DIB section.
	hAlphaCursor = CreateIconIndirect(&ii);

	DeleteObject((void*)hBitmap);
	DeleteObject((void*)hMonoBitmap); 

	return hAlphaCursor;
}

HICON Visual::CreateHIcon()
{
	return CreateHCursor(0, 0);
}

#if 0
HCURSOR Cursor::CreateAlphaCursor()
{
	MSWindows::BITMAPV5HEADER bi;
	HBITMAP hBitmap;
	void *lpBits;

	HCURSOR hAlphaCursor = NULL;
	
	ASSERT(0);
#if 0
	m_Visual->Measure(LDraw::SizeD(0, 0));
	m_Visual->Arrange(LDraw::RectD(LDraw::PointD(0, 0), m_Visual->get_DesiredSize()));
#endif

	int dwWidth  = m_Visual->m_expandedBBox.Width;
	int dwHeight = m_Visual->m_expandedBBox.Height;
	
	std::memset(&bi, 0, sizeof(MSWindows::BITMAPV5HEADER));
	bi.bV5Size = sizeof(MSWindows::BITMAPV5HEADER);
	bi.bV5Width = dwWidth;
	bi.bV5Height = dwHeight;
	bi.bV5Planes = 1;
	bi.bV5BitCount = 32;
	bi.bV5Compression = BI_BITFIELDS;
	// The following mask specification specifies a supported 32 BPP
	// alpha format for Windows XP.
	bi.bV5RedMask   =  0x00FF0000;
	bi.bV5GreenMask =  0x0000FF00;
	bi.bV5BlueMask  =  0x000000FF;
	bi.bV5AlphaMask =  0xFF000000;
	
	// Create the DIB section with an alpha channel.
	hBitmap = MSWindows::CreateDIBSection(NULL, (MSWindows::BITMAPINFO*)&bi, DIB_RGB_COLORS, 
		(void **)&lpBits, NULL, (DWORD)0);
	
	{
		LDraw::Bitmap* bitmap = new LDraw::Bitmap(hBitmap);
		{
			/*
			Graphics* graphics = new Graphics(new LDraw::GraphicsO(bitmap));
			graphics->TranslateTransform(-m_Visual->m_expandedBBox.X, -m_Visual->m_expandedBBox.Y);
			m_Visual->Render(graphics);
			*/
		}
	}
	
	// Create an empty mask bitmap.
	HBITMAP hMonoBitmap = MSWindows::CreateBitmap(dwWidth,dwHeight,1,1,NULL);
	
	ICONINFO ii;
	ii.fIcon = false;
	ii.xHotspot = get_HotspotX() - m_Visual->m_expandedBBox.X;
	ii.yHotspot = get_HotspotY() - m_Visual->m_expandedBBox.Y;
	ii.hbmMask = hMonoBitmap;
	ii.hbmColor = hBitmap;
	
	// Create the alpha cursor with the alpha DIB section.
	hAlphaCursor = CreateIconIndirect(&ii);
	
	MSWindows::DeleteObject((void*)hBitmap);          
	MSWindows::DeleteObject((void*)hMonoBitmap); 
	
	return hAlphaCursor;
}
#endif
#endif

}	// Gui
}	// System
