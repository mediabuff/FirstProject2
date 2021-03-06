#include "stdafx.h"
#include "GUI2.h"

namespace System
{
namespace Gui
{

IMP_DEPENDENCY_PROPERTY(String, ActiveXHostVisual, CLSID, nullptr)
IMP_DEPENDENCY_PROPERTY(String, ActiveXHostVisual, ProgID, nullptr)

DependencyClass* ActiveXHostVisual::get_Class()
{
	static DependencyClass depclass(typeid(thisClass), baseClass::get_Class());

	static DependencyProperty* properties[] =
	{
		get_CLSIDProperty(),
		get_ProgIDProperty(),
	};

	return &depclass;
}

DependencyClass* ActiveXHostVisual::pClass(get_Class());

ActiveXHostVisual::ActiveXHostVisual() : UIElement(get_Class())
{
}

ActiveXHostVisual::ActiveXHostVisual(const ::IID& rclsid, __in_opt IUnknown* pUnkOuter, __in DWORD dwClsContext) : UIElement(get_Class())
{
	m_site = new OleControlSite;
	m_site->CoCreateInstance(rclsid, pUnkOuter, dwClsContext);
}

ActiveXHostVisual::ActiveXHostVisual(StringIn progId, __in_opt IUnknown* pUnkOuter, __in DWORD dwClsContext) : UIElement(get_Class())
{
	m_site = new OleControlSite;
	m_site->CoCreateInstance(progId, pUnkOuter, dwClsContext);
}

void ActiveXHostVisual::OnSetRoot(IRootVisual* root)
{
	if (m_site)
	{
		m_site->m_container = root->GetOleContainer();
		m_site->m_rcPos.left = 0;
		m_site->m_rcPos.top = 0;
		m_site->m_rcPos.right = 600;
		m_site->m_rcPos.bottom = 500;
		m_site->InPlaceActivate();
	}
}

void ActiveXHostVisual::Render(ManagedRenderContext renderContext)
{
	if (m_site && m_site->m_bWindowless && m_site->m_spViewObject)
	{
		int scaledWidth = 500;
		int scaledHeight = 400;

		if (m_bitmap == nullptr)
		{

			Graphics::D3D10Texture2D texture;

			// Create the render target texture
			D3D10_TEXTURE2D_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Width = scaledWidth;
			desc.Height = scaledHeight;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
		//	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			//desc.Format = DXGI_FORMAT_R8G8B8;
			desc.SampleDesc.Count = 1;
		//	desc.Usage = D3D10_USAGE_DEFAULT;
			desc.Usage = D3D10_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
			desc.BindFlags = /*D3D10_BIND_RENDER_TARGET |*/ D3D10_BIND_SHADER_RESOURCE;
			desc.MiscFlags = 0;//D3D10_RESOURCE_MISC_GDI_COMPATIBLE;

			BITMAPINFOHEADER bi = {0};
			bi.biSize = sizeof(BITMAPINFOHEADER);
			bi.biBitCount = 32;
			bi.biPlanes = 1;
			bi.biWidth = scaledWidth;
			bi.biHeight = scaledHeight;

			m_hBitmap = CreateDIBSection(nullptr, (BITMAPINFO*)&bi, DIB_RGB_COLORS, (void**)&m_bitmapBits, nullptr, 0); 

			//ID3D10Texture2D *pTexture = NULL;
			HRESULT hr = Graphics::Direct10::get_D3DDevice()->CreateTexture2D(&desc, nullptr, &texture.m_d3d10_texture);

			m_bitmap = new Graphics::Bitmap(Graphics::Direct10::get_D3DDevice(), texture);
		}

		if (!m_bDrawValid)
		{
			m_bDrawValid = true;
		
			RECTL rcPos;
			rcPos.left = 0;
			rcPos.top = 0;
			rcPos.right = scaledWidth;
			rcPos.bottom = scaledHeight;

			HDC hDC = CreateCompatibleDC(nullptr);
			HBITMAP hOldBitmap = (HBITMAP)SelectObject(hDC, m_hBitmap);

			m_site->m_spViewObject->Draw(DVASPECT_CONTENT, -1, nullptr, nullptr, nullptr, hDC, &rcPos, &rcPos, nullptr, 0L);
			
			SelectObject(hDC, hOldBitmap);
			DeleteDC(hDC);

		//	HRESULT hr;

			ASSERT(0);
#if 0
			D3D10_MAPPED_TEXTURE2D mapped;
			m_bitmap->Map(D3D10_MAP_WRITE_DISCARD, &mapped);

			int srcRowBytes = ROWBYTES_DWORD(scaledWidth, 32);

			for (int y = 0; y < scaledHeight; y++)
			{
				byte* src = m_bitmapBits + (scaledHeight-y-1)*srcRowBytes;
				byte* dst = (byte*)mapped.pData + y*mapped.RowPitch;

				for (int x = 0; x < scaledWidth; x++)
				{
					dst[0] = src[2];
					dst[1] = src[1];
					dst[2] = src[0];
					dst[3] = 255;

					dst += 4;
					src += 4;
				}
			}

			m_bitmap->Unmap();
#endif
		}

		/*
		IDXGISurface1* dxgiSurface;
		hr = m_bitmap->m_d3d10_texture->QueryInterface(__uuidof(IDXGISurface1), (void**)&dxgiSurface);
		if (SUCCEEDED(hr))
		{
			HDC hdc;

			hr = dxgiSurface->GetDC(TRUE, &hdc);
			if (SUCCEEDED(hr))
			{
				m_site->m_spViewObject->Draw(DVASPECT_CONTENT, -1, NULL, NULL, NULL, hdc, &rcPos, &rcPos, NULL, NULL); 

				dxgiSurface->ReleaseDC(NULL);
			}
		}
		*/

		gm::RectF destRect(60, 20, 300, 200);

		renderContext.DrawBitmap(m_bitmap, destRect, 1.0f);
	}
}

/*
IMP_DEPENDENCY_PROPERTY(ImmutableString<wchar_t>*, ActiveXHostElement, CLSID, NULL)
IMP_DEPENDENCY_PROPERTY(ImmutableString<wchar_t>*, ActiveXHostElement, ProgID, NULL)

ActiveXHostElement::ActiveXHostElement() : UIElement(get_Class())
{
}

Visual* ActiveXHostElement::CreateVisual()
{
	return new ActiveXHostVisual;
}
*/

}	// Gui
}	// System
