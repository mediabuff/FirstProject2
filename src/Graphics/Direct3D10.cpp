#include "stdafx.h"
#include "Graphics.h"

#include "openglext.h"

GfxExt HRESULT MapBuffer(ID3D10Device* device, ID3D10Buffer* buffer, D3D10_MAP mapType, void** p)
{
	ASSERT(device);
	ASSERT(p);

	HRESULT hr = buffer->Map(mapType, 0, p);

	return hr;
}

GfxExt void UnmapBuffer(ID3D10Device* device, ID3D10Buffer* buffer)
{
	device=device;	// Unused
	buffer->Unmap();
}

namespace System
{
namespace gm
{
GfxExt D3DXMATRIX MatrixTranslation(float x, float y, float z)
{
	D3DXMATRIX mt;
	D3DXMatrixTranslation(&mt, x, y, z);
	return mt;
}

GfxExt D3DXMATRIX MatrixTranslation(gm::vector3f vec3)
{
	D3DXMATRIX mt;
	D3DXMatrixTranslation(&mt, vec3[0], vec3[1], vec3[2]);
	return mt;
}

GfxExt D3DXMATRIX MatrixScale(gm::vector3f vec3)
{
	D3DXMATRIX mt;
	D3DXMatrixScaling(&mt, vec3[0], vec3[1], vec3[2]);
	return mt;
}

GfxExt D3DXMATRIX MatrixRotationAxis(gm::vector3f vec3, float angle)
{
	D3DXMATRIX mt;
	D3DXMatrixRotationAxis(&mt, (D3DXVECTOR3*)&vec3, angle);
	return mt;
}
}

namespace Graphics
{

__declspec(thread) ID3D11DeviceContext* s_deviceContext;

void Direct10::ReleaseD3DDevice(ID3D10Device* device)
{
	//
}

/*
ID3D11DeviceContext* Direct10::get_DeviceContext()
{
	return s.m_deviceContext;
}
*/

ID3D10Device* Direct10::get_D3DDevice()
{
	return s.m_device;
}

ID3D10Device1* Direct10::get_D3DDevice1()
{
	return s.m_device1;
}

ID2D1Factory* Direct10::get_D2DFactory()
{
	return s.m_D2DFactory;
}

IDWriteFactory* Direct10::get_DWriteFactory()
{
	return s.m_DWriteFactory;
}

Direct10::Imp::Imp() : m_hr(0)
{
	m_device = NULL;
	m_deviceContext = NULL;
	m_D2DFactory = NULL;
	m_DWriteFactory = NULL;

	m_effect = NULL;
	m_RenderTextureTechnique = NULL;
	m_RenderTextureMaskTechnique = NULL;
	m_modelViewProjectionVariable = NULL;
	m_texture0Variable = NULL;
	m_textureMatrix0Variable = NULL;

	m_vertexLayout0 = NULL;
	m_vertexLayout1 = NULL;
	m_vertexLayout2 = NULL;

	m_DrawImageVertexBuffer = NULL;
}

Direct10::Imp::~Imp()
{
	if (m_DrawImageVertexBuffer)
	{
		m_DrawImageVertexBuffer->Release();
	}

	if (m_effect)
	{
		m_effect->Release();
	}
}

GfxExt bool useD2D = true;

bool Direct10::Imp::Init()
{
	if (m_device == NULL)
	{
		HMODULE h = LoadLibraryW(L"D3D10_1.dll");
		if (h == NULL)
		{
			raise(Exception("Direct3D 10_1 not installed"));
		}

		if (useD2D)
		{
#if 0
			D3D_FEATURE_LEVEL levelAttempts[] =
			{
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_9_3,
				D3D_FEATURE_LEVEL_9_2,
				D3D_FEATURE_LEVEL_9_1,
			};

			UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;	// Needed for Direct2D support
			deviceFlags |= D3D11_CREATE_DEVICE_SINGLETHREADED;

			m_hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE,
				NULL/*LoadLibrary(L"D3DWarp10.dll")*/, deviceFlags,
				levelAttempts, _countof(levelAttempts),
				D3D11_SDK_VERSION,
				&m_device,
				&m_featureLevel,
				&m_deviceContext
				);
#endif
				D3D10_FEATURE_LEVEL1 featureLevels[] =
				{
					D3D10_FEATURE_LEVEL_10_1,
					D3D10_FEATURE_LEVEL_10_0,
					D3D10_FEATURE_LEVEL_9_3,
					D3D10_FEATURE_LEVEL_9_2,
					D3D10_FEATURE_LEVEL_9_1,
				};

				uint featureLevelsCount = _countof(featureLevels);

				UINT deviceFlags = D3D10_CREATE_DEVICE_BGRA_SUPPORT;

				for (size_t i = 0; i < featureLevelsCount; ++i)
				{
					m_hr = D3D10CreateDevice1(NULL, D3D10_DRIVER_TYPE_HARDWARE,
						NULL, deviceFlags, featureLevels[i],
						D3D10_1_SDK_VERSION,
						&m_device1);

					if (m_hr == S_OK) break;
				}

				m_device = m_device1;
			}

		// Get the IDXGIDevice that created the device
		m_hr = m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_pDXGIDevice);
		if (FAILED(m_hr))
		{
			raise_(Exception::FromHResult(m_hr));
		}

		m_hr = m_pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&m_pDXGIAdapter);
		if (FAILED(m_hr))
		{
			raise_(Exception::FromHResult(m_hr));
		}
		m_pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_DXGIFactory);

	//	ID3D11DeviceContext* m_deviceContext = NULL;
		// NOTE: This is so that when D2D restores state it doesn't result in a message DEVICE_IASETPRIMITIVETOPOLOGY_TOPOLOGY_UNDEFINED
		m_device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		/*
		m_device->VSSetShader(NULL);
		m_device->GSSetShader(NULL);
		m_device->PSSetShader(NULL);
		*/

		/*
		HRESULT hr;
		// Create a DXGI factory.
		hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&m_DXGIFactory));
		if (FAILED(hr))
		{
			return false;
		}
		*/

		D3D10_DEPTH_STENCIL_DESC desc;
		desc.DepthFunc = D3D10_COMPARISON_LESS;
		desc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
		desc.StencilEnable = false;
		desc.StencilReadMask = D3D10_DEFAULT_STENCIL_READ_MASK;
		desc.StencilWriteMask = D3D10_DEFAULT_STENCIL_WRITE_MASK;

		desc.FrontFace.StencilDepthFailOp = D3D10_STENCIL_OP_KEEP;
		desc.FrontFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
		desc.FrontFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
		desc.FrontFace.StencilFunc = D3D10_COMPARISON_ALWAYS;

		desc.BackFace.StencilDepthFailOp = D3D10_STENCIL_OP_KEEP;
		desc.BackFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
		desc.BackFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
		desc.BackFace.StencilFunc = D3D10_COMPARISON_ALWAYS;

		desc.DepthEnable = false;
		m_hr = m_device->CreateDepthStencilState(&desc, &m_pDepthStencilState[desc.DepthEnable]);
		if (FAILED(m_hr))
		{
			raise_(Exception::FromHResult(m_hr));
		}

		desc.DepthEnable = true;
		m_hr = m_device->CreateDepthStencilState(&desc, &m_pDepthStencilState[desc.DepthEnable]);
		if (FAILED(m_hr))
		{
			raise_(Exception::FromHResult(m_hr));
		}

		D3D10_RASTERIZER_DESC rasterizerState;
		rasterizerState.CullMode = D3D10_CULL_BACK;
	//	rasterizerState.CullMode = D3D10_CULL_NONE;
		rasterizerState.DepthBias = false;
		rasterizerState.DepthBiasClamp = 0;
		rasterizerState.SlopeScaledDepthBias = 0;
		rasterizerState.DepthClipEnable = true;
		rasterizerState.ScissorEnable = false;
		rasterizerState.AntialiasedLineEnable = false;

		for (int multiSampleEnable = 0; multiSampleEnable < 2; ++multiSampleEnable)
		{
			rasterizerState.MultisampleEnable = multiSampleEnable;

			{
				rasterizerState.FillMode = D3D10_FILL_SOLID;

				rasterizerState.FrontCounterClockwise = false;
				m_hr = m_device->CreateRasterizerState(&rasterizerState, &m_pRasterizerState[rasterizerState.FrontCounterClockwise][1][multiSampleEnable]);
				ASSERT(SUCCEEDED(m_hr));

				rasterizerState.FrontCounterClockwise = true;
				m_hr = m_device->CreateRasterizerState(&rasterizerState, &m_pRasterizerState[rasterizerState.FrontCounterClockwise][1][multiSampleEnable]);
				ASSERT(SUCCEEDED(m_hr));
			}

			{
				rasterizerState.FillMode = D3D10_FILL_WIREFRAME;

				rasterizerState.FrontCounterClockwise = false;
				m_hr = m_device->CreateRasterizerState(&rasterizerState, &m_pRasterizerState[rasterizerState.FrontCounterClockwise][0][multiSampleEnable]);
				ASSERT(SUCCEEDED(m_hr));

				rasterizerState.FrontCounterClockwise = true;
				m_hr = m_device->CreateRasterizerState(&rasterizerState, &m_pRasterizerState[rasterizerState.FrontCounterClockwise][0][multiSampleEnable]);
				ASSERT(SUCCEEDED(m_hr));
			}
		}

		m_device->RSSetState(m_pRasterizerState[true/*frontccw*/][true/*solid*/][true/*multisample*/]);

		// Set default
		m_device->OMSetDepthStencilState(m_pDepthStencilState[false], 0/*stencilRef*/);
	}

	if (m_device1)
	{
		if (m_D2DFactory == NULL)
		{
			D2D1_FACTORY_OPTIONS options;
#ifdef _DEBUG
		//	options.debugLevel = D2D1_DEBUG_LEVEL_WARNING;
		//	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
			options.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#else
			options.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#endif
			m_hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, &m_D2DFactory);
			if (FAILED(m_hr))
			{
				raise_(Exception::FromHResult(m_hr));
			}
		}

		if (m_DWriteFactory == NULL)
		{
		//	HRESULT hr;

			m_hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_DWriteFactory), reinterpret_cast<IUnknown**>(&m_DWriteFactory));
			if (FAILED(m_hr))
			{
				raise_(Exception::FromHResult("DWriteCreateFactory failed", m_hr));
			}
		}
	}

	if (m_effect == NULL)
	{
		HRESULT hr;

		ComPtr<ID3D10Blob> errors;

		UINT hlslFlags = 0;
		hlslFlags |= D3D10_SHADER_ENABLE_STRICTNESS;
#ifdef _DEBUG
		hlslFlags |= D3D10_SHADER_DEBUG;
		hlslFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	//	String dir = CurrentDir();
	//	FilePath path = _Module.GetFilename();
	//	path.

		hr = D3DX10CreateEffectFromFileW(IO::Path::Combine(IO::Path::GetDirectoryName(_Module.GetFullPath()), L"../../../Shaders/Gui.fx").c_strw(), NULL, NULL, "fx_4_0", hlslFlags, 0, m_device, NULL, NULL, &m_effect, &errors, NULL);
		if (FAILED(hr))
		{
			String errormsg;

			switch (hr)
			{
			case D3D11_ERROR_FILE_NOT_FOUND:
				errormsg = L"fx file not found";
				break;
			}

			if (errors)
			{
				SIZE_T size = errors->GetBufferSize();
				errormsg = (char*)errors->GetBufferPointer();
			}

			raise_(Exception::FromHResult("effect err: " + errormsg, hr));
		}

		/*
		m_d3d10_modelViewVariable = m_d3d10_effect->GetVariableByName("modelView")->AsMatrix();
		m_d3d10_projectionVariable = m_d3d10_effect->GetVariableByName("projection")->AsMatrix();
		m_d3d10_modelViewProjectionVariable = m_d3d10_effect->GetVariableByName("modelViewProjection")->AsMatrix();
		m_d3d10_modelViewITVariable = m_d3d10_effect->GetVariableByName("modelViewIT")->AsMatrix();
		m_d3d10_modelViewProjectionITVariable = m_d3d10_effect->GetVariableByName("modelViewProjectionIT")->AsMatrix();
		m_d3d10_nlightsVariable = m_d3d10_effect->GetVariableByName("nlights")->AsScalar();
		m_d3d10_lightsVariable = m_d3d10_effect->GetVariableByName("lights");
		*/

		m_RenderTextureTechnique = m_effect->GetTechniqueByName("RenderTexture");
		m_RenderTextureTechnique->ComputeStateBlockMask(&m_RenderTextureStateBlockMask);

		m_RenderTextureMaskTechnique = m_effect->GetTechniqueByName("RenderTextureMask");
		//	m_res->m_d3d9_technique1 = m_res->m_d3d9_effect->GetTechniqueByName("RenderLinearGradient");
		//	m_res->m_d3d9_technique4 = m_res->m_d3d9_effect->GetTechniqueByName("RenderTextSolidColor");

		m_RenderTextureColorMatrixTechnique = m_effect->GetTechniqueByName("RenderTextureColorTransform");

		m_modelViewVariable = m_effect->GetVariableByName("modelView")->AsMatrix();
		m_projectionVariable = m_effect->GetVariableByName("projection")->AsMatrix();
		m_modelViewProjectionVariable = m_effect->GetVariableByName("modelViewProjection")->AsMatrix();

		m_texture0Variable = m_effect->GetVariableByName("texture0")->AsShaderResource();
		m_textureMatrix0Variable = m_effect->GetVariableByName("textureMatrix0")->AsMatrix();

		m_colorTransformVariable = m_effect->GetVariableByName("colorTransform")->AsMatrix();
		m_colorTranslateVariable = m_effect->GetVariableByName("colorTranslate")->AsVector();
		m_colorMultiplyVariable = m_effect->GetVariableByName("colorMultiply")->AsVector();

		{
			// Define the input layout
			D3D10_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },  
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },  
			};
			UINT numElements = sizeof(layout)/sizeof(layout[0]);
			// Create the input layout
			D3DX10_PASS_DESC PassDesc;
			m_RenderTextureTechnique->GetPassByIndex(0)->GetDesc(&PassDesc);
			hr = m_device->CreateInputLayout(layout, numElements, PassDesc.pIAInputSignature, 
				PassDesc.IAInputSignatureSize, &m_vertexLayout0);
		}
	}

	if (m_DrawImageVertexBuffer == NULL)
	{
		// Create vertex buffer
		VERTEX_XYZ_T2 vertices[] =
		{
			gm::vector3f( 0, 1, 0 ), gm::vector2f(0, 1),
			gm::vector3f( 1, 1, 0 ), gm::vector2f(1, 1),
			gm::vector3f( 0, 0, 0 ), gm::vector2f(0, 0),
			gm::vector3f( 1, 0, 0 ), gm::vector2f(1, 0),

			/*
			LDraw::vector3f( 0.5f, 0.5f, -0.5f ), LDraw::vector2f(0, 0),
			LDraw::vector3f( 0.5f, -0.5f, -0.5f ), LDraw::vector2f(0, 0),
			LDraw::vector3f( -0.5f, -0.5f, -0.5f ), LDraw::vector2f(0, 0),
			LDraw::vector3f( -0.5f, 0.5f, -0.5f ), LDraw::vector2f(0, 0),
	*/
	/*
			D3DXVECTOR3( -0.5f, -0.5f, 0.5f ),
			D3DXVECTOR3( 0.5f, -0.5f, 0.5f ),
			D3DXVECTOR3( 0.5f, 0.5f, 0.5f ),
			D3DXVECTOR3( -0.5f, 0.5f, 0.5f ),
			*/
		};

		/*
		uint32 indices[] =
		{
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4
		};
		*/

		D3D10_BUFFER_DESC bd;
		bd.Usage = D3D10_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_XYZ_T2) * 4;
		bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		D3D10_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = vertices;
		m_device->CreateBuffer(&bd, &InitData, &m_DrawImageVertexBuffer);
	}

	return true;
}

//static DrawImage s_DrawImage;

void Direct10::Imp::Render(RenderTarget* rt, Bitmap* bitmap, gm::RectF dstRect, gm::vector4f colorMultiply, gm::vector4f colorTranslate)
{
	if (bitmap->GetD3D10ShaderResourceView() == NULL) return;

	if (rt->m_spRT)
	{
		rt->m_spRT->Flush();
	}

	ASSERT(bitmap->m_d3d10_shaderResourceView);

	// Store old state
/*
	ID3D10RenderTargetView* oldrt;
	ID3D10DepthStencilView* olddepth;
	rt->m_d3d10->m_device->OMGetRenderTargets(1, &oldrt, &olddepth);
*/
	ID3D10InputLayout* pOldInputLayout;
	rt->m_d3d10->m_device->IAGetInputLayout(&pOldInputLayout);

#if 1
	ID3D10StateBlock* stateBlock;
	D3D10CreateStateBlock(rt->m_d3d10->m_device, &m_RenderTextureStateBlockMask, &stateBlock);

	stateBlock->Capture();
#endif
/*
	ID3D10BlendState* pOldBlendState;
	float oldBlendFactor[4];
	UINT oldSampleMask;
	rt->m_d3d10->m_device->OMGetBlendState(&pOldBlendState, oldBlendFactor, &oldSampleMask);

	ID3D10PixelShader* pOldPixelShader;
	rt->m_d3d10->m_device->PSGetShader(&pOldPixelShader);

	ID3D10VertexShader* pOldVertexShader;
	rt->m_d3d10->m_device->VSGetShader(&pOldVertexShader);

	ID3D10GeometryShader* pOldGeometryShader;
	rt->m_d3d10->m_device->GSGetShader(&pOldGeometryShader);

	rt->m_d3d10->m_device->GSGetGSGetShaderResources(
	*/

	// Set new state
	rt->m_d3d10->m_device->OMSetRenderTargets(1, &rt->m_pRenderTargetView, NULL);

	rt->m_d3d10->m_device->IASetInputLayout(m_vertexLayout0);

	UINT stride = sizeof(VERTEX_XYZ_T2);
	UINT offset = 0;
	rt->m_d3d10->m_device->IASetVertexBuffers(0, 1, &m_DrawImageVertexBuffer, &stride, &offset);
	rt->m_d3d10->m_device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	gm::matrix4f xform =
		gm::matrix4f::getTranslate(dstRect.X, dstRect.Y, 0) *
		gm::matrix4f::getScale(dstRect.Width, dstRect.Height, 1);

	m_modelViewProjectionVariable->SetMatrix((float*)(rt->m_modelViewProjection * xform));

	gm::matrix4f textureMatrix = gm::matrix4f::getIdentity();
	/*
		LDraw::matrix4f::getTranslate(-srcRect.X, -srcRect.Y) *
		LDraw::matrix4f::getScale(srcRect.Width / bitmap->GetWidth(), srcRect.Height / bitmap->GetHeight());
*/

	m_texture0Variable->SetResource(bitmap->GetD3D10ShaderResourceView());
	m_textureMatrix0Variable->SetMatrix((float*)(textureMatrix));

	m_colorMultiplyVariable->SetFloatVector((float*)colorMultiply);
	m_colorTranslateVariable->SetFloatVector((float*)colorTranslate);

	D3DX10_TECHNIQUE_DESC techDesc;
	m_RenderTextureTechnique->GetDesc(&techDesc);

	for (UINT nPass = 0; nPass < techDesc.Passes; ++nPass)
	{
		m_RenderTextureTechnique->GetPassByIndex(nPass)->Apply(0);

		rt->m_d3d10->m_deviceContext->Draw(
			4, // VertexCount
			0 // StartVertexLocation
			);
	}

	m_texture0Variable->SetResource(NULL);

	// Restore old state
/*
	rt->m_d3d10->m_device->PSSetShader(pOldPixelShader);
	rt->m_d3d10->m_device->VSSetShader(pOldVertexShader);
	rt->m_d3d10->m_device->GSSetShader(pOldGeometryShader);
*/

#if 1
	stateBlock->Apply();
	stateBlock->Release();
#endif

	rt->m_d3d10->m_device->IASetInputLayout(pOldInputLayout);
/*	rt->m_d3d10->m_device->OMSetBlendState(pOldBlendState, oldBlendFactor, oldSampleMask);

	rt->m_d3d10->m_device->OMSetRenderTargets(1, &oldrt, olddepth);
	if (oldrt) oldrt->Release();
	if (olddepth) olddepth->Release();
	*/
}

void Direct10::Imp::Render(RenderTarget* rt, Bitmap* bitmap, gm::RectF dstRect, gm::RectF srcRect, DrawBitmapArgs* args)
{
	if (bitmap->m_d3d10_shaderResourceView == NULL)
		return;

	if (rt->m_spRT)
	{
		rt->m_spRT->Flush();
	}

	ASSERT(bitmap->m_d3d10_shaderResourceView);

	rt->m_d3d10->m_device->IASetInputLayout(m_vertexLayout0);

	UINT stride = sizeof(VERTEX_XYZ_T2);
	UINT offset = 0;
	rt->m_d3d10->m_device->IASetVertexBuffers(0, 1, &m_DrawImageVertexBuffer, &stride, &offset);
	rt->m_d3d10->m_device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	gm::matrix4f xform =
		gm::matrix4f::getTranslate(dstRect.X, dstRect.Y, 0) *
		gm::matrix4f::getScale(dstRect.Width, dstRect.Height, 1);

	m_modelViewProjectionVariable->SetMatrix((float*)(rt->m_modelViewProjection * xform));

	gm::matrix4f textureMatrix = gm::matrix4f::getIdentity();
	/*
		LDraw::matrix4f::getTranslate(-srcRect.X, -srcRect.Y) *
		LDraw::matrix4f::getScale(srcRect.Width / bitmap->GetWidth(), srcRect.Height / bitmap->GetHeight());
*/

	m_texture1Variable->SetResource(bitmap->m_d3d10_shaderResourceView);

	m_texture0Variable->SetResource(bitmap->m_d3d10_shaderResourceView);
	m_textureMatrix0Variable->SetMatrix((float*)(textureMatrix));

	if (args)
	{
		m_colorTransformVariable->SetMatrix((float*)args->m_colorTransform.m_transform);
		m_colorTranslateVariable->SetFloatVector((float*)args->m_colorTransform.m_translate);

		D3DX10_TECHNIQUE_DESC techDesc;
		HRESULT hr = m_RenderTextureColorMatrixTechnique->GetDesc(&techDesc);

		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			m_RenderTextureColorMatrixTechnique->GetPassByIndex(p)->Apply(0);

			rt->m_d3d10->m_device->Draw(
				4, // VertexCount
				0 // StartVertexLocation
				);
		}
	}
	else
	{
		D3DX10_TECHNIQUE_DESC techDesc;
		m_RenderTextureTechnique->GetDesc(&techDesc);

		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			m_RenderTextureTechnique->GetPassByIndex(p)->Apply(0);

			rt->m_d3d10->m_device->Draw(
				4, // VertexCount
				0 // StartVertexLocation
				);
		}
	}

	m_texture0Variable->SetResource(NULL);
}

Direct10::Imp Direct10::s;

// static
Direct3D10_I* Direct3D10_I::s_Current;

	// static
Direct3D10_I* Direct3D10_I::GetCurrent()
{
	if (s_Current == NULL)
	{
		s_Current = new Direct3D10_I();
	}
	return s_Current;
}

Direct3D10_I::Direct3D10_I()
{
	m_device = Direct10::get_D3DDevice();
	m_device1 = Direct10::get_D3DDevice1();
//	m_deviceContext = Direct10::get_DeviceContext();

	ASSERT(m_device);
//	ASSERT(m_deviceContext);
}

void Direct3D10_I::Load2()
{
	if (m_d3d10_effect == NULL)
	{
		HRESULT hr;

		ID3D10Blob* errors = NULL;

	// Create the effect
		UINT hlslFlags = 0;
		hlslFlags |= D3D10_SHADER_ENABLE_STRICTNESS;
#ifdef _DEBUG
		hlslFlags |= D3D10_SHADER_DEBUG;
		hlslFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

		hr = D3DX10CreateEffectFromFileW(IO::Path::Combine(IO::Path::GetDirectoryName(_Module.GetFullPath()), L"../../../Shaders/FixedFunction.fx").c_strw(), NULL, NULL, "fx_4_0", hlslFlags, 0, m_device, NULL, NULL, &m_d3d10_effect, &errors, NULL);
		if (FAILED(hr))
		{
			if (errors)
			{
				String errormsg = (char*)errors->GetBufferPointer();
				errors->Release();

				raise(Exception("effect: " + errormsg));
			}
			else
			{
				switch (hr)
				{
				case D3D10_ERROR_FILE_NOT_FOUND:
					raise(IO::FileNotFoundException("file not found"));
				}
				
				raise_(Exception::FromHResult(hr));
			}
		}

		m_d3d10_viewPositionVariable = m_d3d10_effect->GetVariableByName("viewPosition")->AsVector();

		m_d3d10_modelVariable = m_d3d10_effect->GetVariableByName("model")->AsMatrix();
		m_d3d10_viewVariable = m_d3d10_effect->GetVariableByName("view")->AsMatrix();
		m_d3d10_projectionVariable = m_d3d10_effect->GetVariableByName("projection")->AsMatrix();
		m_d3d10_modelViewVariable = m_d3d10_effect->GetVariableByName("modelView")->AsMatrix();
		m_d3d10_modelViewProjectionVariable = m_d3d10_effect->GetVariableByName("modelViewProjection")->AsMatrix();
		m_d3d10_modelViewITVariable = m_d3d10_effect->GetVariableByName("modelViewIT")->AsMatrix();
		m_d3d10_modelViewProjectionITVariable = m_d3d10_effect->GetVariableByName("modelViewProjectionIT")->AsMatrix();

		m_d3d10_materialVariable = m_d3d10_effect->GetVariableByName("material");
		m_d3d10_nlightsVariable = m_d3d10_effect->GetVariableByName("nlights")->AsScalar();
		m_d3d10_lightsVariable = m_d3d10_effect->GetVariableByName("lights");

		m_d3d10_fogTypeVariable = m_d3d10_effect->GetVariableByName("fogType")->AsScalar();
		m_d3d10_fogColorVariable = m_d3d10_effect->GetVariableByName("fogColor")->AsVector();
		m_d3d10_fogDistanceSquaredVariable = m_d3d10_effect->GetVariableByName("fogDistanceSquared")->AsScalar();

//		ASSERT(sizeof(X3DMaterial) == 68);

		m_d3d10_ntexturesVariable = m_d3d10_effect->GetVariableByName("ntextures")->AsScalar();
		m_d3d10_texturesVariable = m_d3d10_effect->GetVariableByName("textures")->AsShaderResource();

	//	d3d10->m_d3d10_effect = m_d3d10_effect;

	//	renderContext.m_p->GetRT()->m_RenderTextureTechnique = m_effect->GetTechniqueByName("SolidColorTechnique");
	//	m_RenderTextureTechnique->ComputeStateBlockMask(&m_RenderTextureStateBlockMask);

		m_d3d10_HeightMapTexture = m_d3d10_effect->GetVariableByName("HeightMapTexture")->AsShaderResource();

		/*
		d3d10->m_d3d10_modelViewVariable = m_d3d10_modelViewVariable;
		renderContext.m_p->GetRT()->m_d3d10->m_d3d10_projectionVariable = m_d3d10_projectionVariable;
		renderContext.m_p->GetRT()->m_d3d10->m_d3d10_modelViewProjectionVariable = m_d3d10_modelViewProjectionVariable;
		renderContext.m_p->GetRT()->m_d3d10->m_d3d10_modelViewITVariable = m_d3d10_modelViewITVariable;
		renderContext.m_p->GetRT()->m_d3d10->m_d3d10_modelViewProjectionITVariable = m_d3d10_modelViewProjectionITVariable;
		renderContext.m_p->GetRT()->m_d3d10->m_d3d10_materialVariable = m_d3d10_materialVariable;
		renderContext.m_p->GetRT()->m_d3d10->m_d3d10_nlightsVariable = m_d3d10_nlightsVariable;
		renderContext.m_p->GetRT()->m_d3d10->m_d3d10_lightsVariable = m_d3d10_lightsVariable;
		*/

		{
			D3D10_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },  
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },  
			};

			m_solidColorTechnique = m_d3d10_effect->GetTechniqueByName("SolidColorTechnique");

			D3DX10_PASS_DESC PassDesc;
			m_solidColorTechnique->GetPassByIndex(0)->GetDesc(&PassDesc);
			hr = m_device->CreateInputLayout(layout, _countof(layout), PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &m_solidColorInputLayout);
			ASSERT(SUCCEEDED(hr));
		}

		{
			D3D10_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },  
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },  
			};

			m_phongLightTechnique = m_d3d10_effect->GetTechniqueByName("PhongLightTechnique");

			D3DX10_PASS_DESC PassDesc;
			m_phongLightTechnique->GetPassByIndex(0)->GetDesc(&PassDesc);
			hr = m_device->CreateInputLayout(layout, _countof(layout), PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &m_phongLightInputLayout);
			ASSERT(SUCCEEDED(hr));
		}

		{
			D3D10_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },  
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },  
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },  
			};

			m_phongLightTexTechnique = m_d3d10_effect->GetTechniqueByName("PhongLightTexTechnique");

			D3DX10_PASS_DESC PassDesc;
			m_phongLightTexTechnique->GetPassByIndex(0)->GetDesc(&PassDesc);
			hr = m_device->CreateInputLayout(layout, _countof(layout), PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &m_phongLightTexInputLayout);
			ASSERT(SUCCEEDED(hr));
		}

		{
			D3D10_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },  
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },  
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D10_INPUT_PER_VERTEX_DATA, 0 },  
			};

			m_colorTexTechnique = m_d3d10_effect->GetTechniqueByName("ColorTexTechnique");

			D3DX10_PASS_DESC PassDesc;
			m_colorTexTechnique->GetPassByIndex(0)->GetDesc(&PassDesc);
			hr = m_device->CreateInputLayout(layout, _countof(layout), PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &m_colorTexInputLayout);
			ASSERT(SUCCEEDED(hr));
		}
	}
}

void Direct3D10_I::Shutdown()
{
	ASSERT(0);
#if 0
	/*
	if (m_spRT)
	{
		m_spRT->Release();
		m_spRT = NULL;
	}
	*/

	if (m_pDepthStencilView)
	{
		m_pDepthStencilView->Release();
		m_pDepthStencilView = NULL;
	}

	if (m_pDepthStencil)
	{
		m_pDepthStencil->Release();
		m_pDepthStencil = NULL;
	}

	if (m_pRenderTargetView)
	{
		m_pRenderTargetView->Release();
		m_pRenderTargetView = NULL;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = NULL;
	}

	GL::Direct10::ReleaseD3DDevice(m_device);
#endif
}

void Direct3D10_I::BeginDraw()
{
	/*
	RECT clientRect;
	GetClientRect(m_hWnd, &clientRect);

	int clientWidth = clientRect.right;
	int clientHeight = clientRect.bottom;

	if (m_pRenderTargetView)
	{
		D3D10_VIEWPORT vp;
		vp.Width = clientWidth;
		vp.Height = clientHeight;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_device->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
		m_device->RSSetViewports(1, &vp);
	}
	*/
}

void Direct3D10_I::EndDraw()
{
}

IDirect3DDevice9* Direct3D10_I::GetD3D9Device()
{
	return NULL;
}

CGcontext Direct3D10_I::GetCGContext()
{
	return NULL;
}

int Direct3D10_I::cgGetLatestProfile(unsigned int type)
{
	return 0;
}

void Direct3D10_I::Viewport(int x, int y, int width, int height)
{
	D3D10_VIEWPORT vp;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = x;
	vp.TopLeftY = y;
	m_device->RSSetViewports(1, &vp);
}

void Direct3D10_I::SetClipPlane(int n, const float equation[4])
{
	VERIFY(0);
}

void Direct3D10_I::EnableClipPlanes(UINT mask)
{
	VERIFY(0);
}

void Direct3D10_I::Draw(UINT first, UINT count)
{
	m_deviceContext->Draw(count, first);
}

void Direct3D10_I::DrawIndexed(/*unsigned int mode,*/ int start, int end, UINT count/*, unsigned int type, int startIndex*/)
{
	// ??
// start / end not used ?

	UINT IndexCount = count;
	UINT StartIndexLocation = start;
	INT BaseVertexLocation = 0;//startIndex;

	m_deviceContext->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
}

UINT Direct3D10_I::Begin(ID3DX10EffectTechnique* technique)
{
	D3DX10_TECHNIQUE_DESC techDesc;
	technique->GetDesc(&techDesc);
	return techDesc.Passes;
}

void Direct3D10_I::Apply(ID3DX10EffectTechnique* technique, UINT nPass)
{
	technique->GetPassByIndex(nPass)->Apply(0);
}

void Direct3D10_I::DrawUP(/*unsigned int mode,*/ UINT count, void* data, int stride)
{
	VERIFY(0);
}

void Direct3D10_I::SetTexture(int stage, Texture texture)
{
}

VertexDeclaration Direct3D10_I::CreateVertexDeclaration(VertexDeclElement* elements, unsigned int count, void* shader)
{
	//ID3D10Blob* blob = (ID3D10Blob*)shader;
	D3DX10_PASS_DESC* passdesc = (D3DX10_PASS_DESC*)shader;

	D3D10_INPUT_ELEMENT_DESC elements10[64];
	for (unsigned int i = 0; i < count; i++)
	{
		elements10[i].SemanticName = elements[i].usage;
		elements10[i].SemanticIndex = elements[i].usageIndex;
		elements10[i].InputSlot = elements[i].stream;
		elements10[i].AlignedByteOffset = elements[i].offset;
		elements10[i].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
		elements10[i].InstanceDataStepRate = 0;

		switch (elements[i].type)
		{
		case DECLTYPE_FLOAT1:
			elements10[i].Format = DXGI_FORMAT_R32_FLOAT;
			break;

		case DECLTYPE_FLOAT2:
			elements10[i].Format = DXGI_FORMAT_R32G32_FLOAT;
			break;

		case DECLTYPE_FLOAT3:
			elements10[i].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			break;

		case DECLTYPE_FLOAT4:
			elements10[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		}
	}

//	void* shaderWithSignature = NULL;
//	uint byteCodeLength = 0;
	ID3D10InputLayout* pInputLayout = NULL;

	HRESULT hr = m_device->CreateInputLayout(elements10, count, passdesc->pIAInputSignature, passdesc->IAInputSignatureSize, &pInputLayout);
	ASSERT(SUCCEEDED(hr));

	return VertexDeclaration(pInputLayout);
}

void Direct3D10_I::SetVertexDeclaration(VertexDeclaration vertexDecl)
{
	m_device->IASetInputLayout(vertexDecl.u.d3d10);
}

void Direct3D10_I::DeleteVertexDeclaration(VertexDeclaration vertexDecl)
{
}

void Direct3D10_I::IdentityTransform()
{
	switch (m_matrixMode)
	{
	case GL_MODELVIEW:
		m_modelviewMatrix = gm::matrix4f::getIdentity();
		break;

	case GL_PROJECTION:
		m_projectionMatrix = gm::matrix4f::getIdentity();
		break;
	}
}

void Direct3D10_I::TranslateTransform(float x, float y, float z)
{
	switch (m_matrixMode)
	{
	case GL_MODELVIEW:
		m_modelviewMatrix = gm::matrix4f::getTranslate(x, y, z) * m_modelviewMatrix;
		break;

	case GL_PROJECTION:
		m_projectionMatrix = gm::matrix4f::getTranslate(x, y, z) * m_projectionMatrix;
		break;
	}
}

void Direct3D10_I::ScaleTransform(float sx, float sy, float sz)
{
	switch (m_matrixMode)
	{
	case GL_MODELVIEW:
		m_modelviewMatrix = gm::matrix4f::getScale(sx, sy, sz) * m_modelviewMatrix;
		break;

	case GL_PROJECTION:
		m_projectionMatrix = gm::matrix4f::getScale(sx, sy, sz) * m_projectionMatrix;
		break;
	}
}

void Direct3D10_I::RotateTransform(float angleDegrees, float x, float y, float z)
{
	switch (m_matrixMode)
	{
	case GL_MODELVIEW:
		m_modelviewMatrix = gm::matrix4f::getRotate(angleDegrees, gm::vector3f(x, y, z)) * m_modelviewMatrix;
		break;

	case GL_PROJECTION:
		m_projectionMatrix = gm::matrix4f::getRotate(angleDegrees, gm::vector3f(x, y, z)) * m_projectionMatrix;
		break;
	}
}

void Direct3D10_I::MultiplyTransform(const float m[16])
{
	switch (m_matrixMode)
	{
	case GL_MODELVIEW:
		m_modelviewMatrix = (*(gm::matrix4f*)m) * m_modelviewMatrix;
		break;

	case GL_PROJECTION:
		m_projectionMatrix = (*(gm::matrix4f*)m) * m_projectionMatrix;
		break;
	}
}

void Direct3D10_I::SetTransform(const float m[16])
{
	switch (m_matrixMode)
	{
	case GL_MODELVIEW:
		m_modelviewMatrix = *(const gm::matrix4f*)m;
		break;

	case GL_PROJECTION:
		m_projectionMatrix = *(const gm::matrix4f*)m;
		break;
	}
}

IndexBuffer Direct3D10_I::CreateIndexBuffer(unsigned int type, unsigned int size, const void* data)
{
	D3D10_BUFFER_DESC buffer_desc;
	buffer_desc.ByteWidth = size;
	buffer_desc.Usage = D3D10_USAGE_DEFAULT;
	buffer_desc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = data;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	ID3D10Buffer* pBuffer;
	HRESULT hr = m_device->CreateBuffer(&buffer_desc, data? &InitData: NULL, &pBuffer);
	ASSERT(SUCCEEDED(hr));

	return IndexBuffer(pBuffer, type);
}

void Direct3D10_I::IndexBufferSubData_(IndexBuffer buffer, unsigned int offset, unsigned int size, const void* data)
{
	D3D10_BOX dstBox;
	dstBox.front = 0;
	dstBox.back = 1;
	dstBox.top = 0;
	dstBox.bottom = 1;
	dstBox.left = offset;
	dstBox.right = offset+size;

	UINT SrcRowPitch = size;
	m_device->UpdateSubresource(buffer.u.d3d10, 0, &dstBox, data, SrcRowPitch, 0);
}

void Direct3D10_I::VertexBufferSubData_(VertexBuffer buffer, unsigned int offset, unsigned int size, const void* data)
{
	D3D10_BOX dstBox;
	dstBox.front = 0;
	dstBox.back = 1;
	dstBox.top = 0;
	dstBox.bottom = 1;
	dstBox.left = offset;
	dstBox.right = offset+size;

	UINT SrcRowPitch = size;
	m_device->UpdateSubresource(buffer.u.d3d10, 0, &dstBox, data, SrcRowPitch, 0);
}

void Direct3D10_I::SetVertexBuffer(UINT stream, VertexBuffer vertexBuffer, UINT byteOffset, UINT stride)
{
	m_device->IASetVertexBuffers(stream, 1, &vertexBuffer.u.d3d10, &stride, &byteOffset);
}

void Direct3D10_I::SetVertexBuffers(UINT start, UINT numBuffers, VertexBuffer* vertexBuffers, const UINT* strides, const UINT* byteOffsets)
{
	ID3D10Buffer** buffers = &vertexBuffers[0].u.d3d10;
	m_device->IASetVertexBuffers(start, numBuffers, buffers, strides, byteOffsets);
}

VertexBuffer Direct3D10_I::CreateVertexBuffer(unsigned int size, const void* data, unsigned int bufferUsage)
{
	D3D10_BUFFER_DESC buffer_desc;
	buffer_desc.ByteWidth = size;
	buffer_desc.Usage = D3D10_USAGE_DEFAULT;
	buffer_desc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = data;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	ID3D10Buffer* pBuffer;
	HRESULT hr = m_device->CreateBuffer(&buffer_desc, data? &InitData: NULL, &pBuffer);
	ASSERT(SUCCEEDED(hr));

	return VertexBuffer(pBuffer);
}

ID3D10ShaderResourceView* Direct3D10_I::CreateShaderResourceView(ID3D10Resource* resource)
{
	ID3D10Texture1D* tex1D = static_cast<ID3D10Texture1D*>(resource);

	D3D10_TEXTURE1D_DESC desc;
	tex1D->GetDesc(&desc);

	// Create the shader-resource view
	D3D10_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = desc.Format;
	srDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE1D;
	srDesc.Texture2D.MostDetailedMip = 0;
	srDesc.Texture2D.MipLevels = 1;

	ID3D10ShaderResourceView* pShaderResView = NULL;
	m_device->CreateShaderResourceView(resource, &srDesc, &pShaderResView);

	return pShaderResView;
}

Texture Direct3D10_I::CreateTexture1D(int level, int internalformat, int width, int border, unsigned int format, unsigned int type, const void *pixels)
{
	D3D10_TEXTURE1D_DESC desc = {0};
	desc.Width = width;
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	desc.SampleDesc.Count = 1;
	desc.Usage = D3D10_USAGE_DYNAMIC;
	desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA initial;
	initial.pSysMem = pixels;
	initial.SysMemPitch = 0;
	initial.SysMemSlicePitch = 0;

	ID3D10Texture1D* pTexture = NULL;
	m_device->CreateTexture1D(&desc, &initial, &pTexture);

#if 0
	void* pMappedData;
	Direct10::get_DeviceContext()->Map(pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &pMappedData);

	//for (int y = 0; y < height; y++)
	{
		uint8* dst = (uint8*)pMappedData;
		uint8* src = (uint8*)pixels;

		for (int x = 0; x < width; ++x)
		{
			if (format == GL_BGRA)
			{
				dst[0] = src[2];
				dst[1] = src[1];
				dst[2] = src[0];
				dst[3] = src[3];
			}
			else
			{
				dst[0] = src[0];
				dst[1] = src[1];
				dst[2] = src[2];
				dst[3] = src[3];
			}
		//	dst[0] = src[2];	// b
		//	dst[1] = src[1];	// g
		//	dst[2] = src[0];	// r

			dst += 4;
			src += 4;
		}
	}

	Direct10::get_DeviceContext()->Unmap(pTexture, 0);
#endif

	return Texture(this, CreateShaderResourceView(pTexture));
}

Texture Direct3D10_I::CreateTexture2D(int level, int internalformat, int width, int height, int border, unsigned int format, unsigned int type, const void *pixels)
{
	D3D10_TEXTURE2D_DESC desc = {0};
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D10_USAGE_DYNAMIC;
	desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

	ID3D10Texture2D* pTexture = NULL;
	m_device->CreateTexture2D(&desc, NULL, &pTexture);

#if 0
	D3D11_MAPPED_SUBRESOURCE mappedTex;
	Direct10::get_DeviceContext()->Map(pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedTex);

	for (int y = 0; y < height; ++y)
	{
		ubyte* dst = (ubyte*)mappedTex.pData + mappedTex.RowPitch * y;
		ubyte* src = (ubyte*)pixels + y * width*3;

		for (int x = 0; x < width; ++x)
		{
			dst[0] = src[0];
			dst[1] = src[1];
			dst[2] = src[2];
			dst[3] = src[3];

		//	dst[0] = src[2];	// b
		//	dst[1] = src[1];	// g
		//	dst[2] = src[0];	// r

			dst += 4;
			src += 4;
		}
	}

	Direct10::get_DeviceContext()->Unmap(pTexture, 0);
#endif

	return Texture(this, CreateShaderResourceView(pTexture));
}

IndexBuffer Direct3D10_I::GetIndexBuffer(UINT* pOffsetInBytes)
{
	ID3D10Buffer* buffer;
	DXGI_FORMAT format;
	m_device->IAGetIndexBuffer(&buffer, &format, pOffsetInBytes);
	return IndexBuffer(buffer, format == DXGI_FORMAT_R16_UINT? GL_UNSIGNED_SHORT: GL_UNSIGNED_INT);
}

void Direct3D10_I::SetIndexBuffer(IndexBuffer indexBuffer, unsigned int offsetInBytes)
{
	DXGI_FORMAT format;

	if (indexBuffer.m_type == GL_UNSIGNED_SHORT)
	{
		format = DXGI_FORMAT_R16_UINT;
	}
	else if (indexBuffer.m_type == GL_UNSIGNED_INT)
	{
		format = DXGI_FORMAT_R32_UINT;
	}
	else
	{
		ASSERT(indexBuffer.m_type == 0);
		ASSERT(indexBuffer.u.d3d10 == NULL);
		format = DXGI_FORMAT_UNKNOWN;
	}

	m_device->IASetIndexBuffer(indexBuffer.u.d3d10, format, offsetInBytes);
}

void Direct3D10_I::SetPrimitiveTopology(unsigned int mode)
{
	D3D10_PRIMITIVE_TOPOLOGY topology;

	switch (mode)
	{
		case GL_TRIANGLES:
			topology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			break;

		case GL_TRIANGLE_STRIP:
			topology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			break;

		default:
			raise(Exception("SetPrimitiveTopology: Invalid mode"));
	}

	m_device->IASetPrimitiveTopology(topology);
}

void Direct3D10_I::SetMatrixMode(unsigned int mode)
{
	m_matrixMode = mode;
}

void Direct3D10_I::PushMatrix()
{
	switch (m_matrixMode)
	{
	case GL_MODELVIEW:
		m_modelviewMatrixStack.push(m_modelviewMatrix);
		break;

	case GL_PROJECTION:
		m_projectionMatrixStack.push(m_projectionMatrix);
		break;

	default:
		VERIFY(0);
	}
}

void Direct3D10_I::PopMatrix()
{
	switch (m_matrixMode)
	{
	case GL_MODELVIEW:
		m_modelviewMatrix = m_modelviewMatrixStack.top();
		m_modelviewMatrixStack.pop();
		break;

	case GL_PROJECTION:
		m_projectionMatrix = m_projectionMatrixStack.top();
		m_projectionMatrixStack.pop();
		break;

	default:
		VERIFY(0);
	}
}

void Direct3D10_I::Enable(unsigned int cap)
{
	switch (cap)
	{
	case GL_DEPTH_TEST:
		{
			D3D10_DEPTH_STENCIL_DESC desc;

			desc.DepthEnable = TRUE;

			desc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
			desc.DepthFunc = D3D10_COMPARISON_LESS;
			desc.StencilEnable = FALSE;
			desc.StencilReadMask = D3D10_DEFAULT_STENCIL_READ_MASK;
			desc.StencilWriteMask = D3D10_DEFAULT_STENCIL_WRITE_MASK;

			desc.FrontFace.StencilFunc = D3D10_COMPARISON_ALWAYS;
			desc.FrontFace.StencilDepthFailOp = D3D10_STENCIL_OP_KEEP;
			desc.FrontFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
			desc.FrontFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;

			desc.BackFace.StencilFunc = D3D10_COMPARISON_ALWAYS;
			desc.BackFace.StencilDepthFailOp = D3D10_STENCIL_OP_KEEP;
			desc.BackFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
			desc.BackFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;

			ID3D10DepthStencilState* pState;
			m_device->CreateDepthStencilState(&desc, &pState);

			m_device->OMSetDepthStencilState(pState, 0/*stencilRef*/);
		}
		break;

	default:
		ASSERT(0);
	}
}

void Direct3D10_I::Disable(unsigned int cap)
{
	switch (cap)
	{
	case GL_DEPTH_TEST:
		{
			D3D10_DEPTH_STENCIL_DESC desc;

			desc.DepthEnable = FALSE;

			desc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
			desc.DepthFunc = D3D10_COMPARISON_LESS;
			desc.StencilEnable = FALSE;
			desc.StencilReadMask = D3D10_DEFAULT_STENCIL_READ_MASK;
			desc.StencilWriteMask = D3D10_DEFAULT_STENCIL_WRITE_MASK;

			desc.FrontFace.StencilFunc = D3D10_COMPARISON_ALWAYS;
			desc.FrontFace.StencilDepthFailOp = D3D10_STENCIL_OP_KEEP;
			desc.FrontFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
			desc.FrontFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;

			desc.BackFace.StencilFunc = D3D10_COMPARISON_ALWAYS;
			desc.BackFace.StencilDepthFailOp = D3D10_STENCIL_OP_KEEP;
			desc.BackFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
			desc.BackFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;

			ID3D10DepthStencilState* pState;
			m_device->CreateDepthStencilState(&desc, &pState);

			m_device->OMSetDepthStencilState(pState, 0/*stencilRef*/);
		}
		break;

	default:
		ASSERT(0);
	}
}

void Direct3D10_I::Clear(uint32 mask)
{
	/*
	if (mask & GL_COLOR_BUFFER_BIT)
	{
		float ClearColor[4] = { 0.0f, 1.0f, 0.0f, 1.0f }; // RGBA
		m_device->ClearRenderTargetView(m_pRenderTargetView, ClearColor);
	}
	*/
}

void Direct3D10_I::DepthMask(bool mask)
{
}

void Direct3D10_I::GetFloatv(unsigned int pname, float* params)
{
	switch (pname)
	{
	case GL_MODELVIEW_MATRIX:
		{
			memcpy(params, (const float*)m_modelviewMatrix, 4*16);
		}
		break;

	case GL_PROJECTION_MATRIX:
		{
			memcpy(params, (const float*)m_projectionMatrix, 4*16);
		//	m_projectionMatrix.copyTo(params);
		}
		break;

	default:
		ASSERT(0);
		throw std::exception("GetFloatv: Invalid pname");
	}
}

// CG
void Direct3D10_I::cgBindProgram(CGprogram cgProgram)
{
}

void Direct3D10_I::cgLoadProgram(CGprogram cgProgram)
{
}

//ID3DMatrixVariable* AsMatrix

#if 0

void GraphicsO::SetupD10_0()
{
	Direct3D10_I* d3d10 = dynamic_cast<Direct3D10_I*>(m_device);

	if (m_res->m_effect10 == NULL)
	{
		UINT dwShaderFlags = 0;
		ID3D10Blob* errors = NULL;
		HRESULT hr = D3DX10CreateEffectFromFile(L"../../Shaders/techniques.fx", NULL, NULL, "fx_4_0", dwShaderFlags, 0, d3d10->m_device, NULL,
																 NULL, &m_res->m_effect10, &errors, NULL);
		if (FAILED(hr))
		{
			if (errors)
			{
				const char* str = (const char*)errors->GetBufferPointer();
				errors->Release();
			}
		}

		m_res->m_technique0 = m_res->m_effect10->GetTechniqueByName("RenderSolidColor");
		m_res->m_technique1 = m_res->m_effect10->GetTechniqueByName("RenderLinearGradient");

		m_res->m_modelViewVariable = m_res->m_effect10->GetVariableByName("modelView")->AsMatrix();
		m_res->m_projectionVariable = m_res->m_effect10->GetVariableByName("projection")->AsMatrix();
		m_res->m_modelViewProjectionVariable = m_res->m_effect10->GetVariableByName("modelViewProjection")->AsMatrix();
		m_res->m_textureMatrix0Variable = m_res->m_effect10->GetVariableByName("textureMatrix0")->AsMatrix();
		m_res->m_texture0Variable = m_res->m_effect10->GetVariableByName("texture0")->AsShaderResource();
		m_res->m_solidColorVariable = m_res->m_effect10->GetVariableByName("solidColor")->AsVector();
	}

	if (m_res->m_vertexDeclaration0 == NULL)
	{
		// Create the input layout
		D3D10_PASS_DESC PassDesc;
		m_res->m_technique0->GetPassByIndex(0)->GetDesc(&PassDesc);

		VertexDeclElement elements[] =
		{
			{"POSITION", 0, 0, DECLTYPE_FLOAT3, 0},
		};

		m_res->m_vertexDeclaration0 = m_device->CreateVertexDeclaration(elements, 1, &PassDesc);
	}

	matrix4f modelView;
	m_device->GetFloatv(GL_MODELVIEW_MATRIX, (float*)modelView);

	matrix4f projection;
	m_device->GetFloatv(GL_PROJECTION_MATRIX, (float*)projection);

	m_res->m_modelViewVariable->SetMatrix(modelView);
	m_res->m_projectionVariable->SetMatrix(projection);
	m_res->m_modelViewProjectionVariable->SetMatrix(projection * modelView);

	m_device->SetVertexDeclaration(m_res->m_vertexDeclaration0);
}

void GraphicsO::SetupD10_1()
{
	Direct3D10_I* d3d10 = dynamic_cast<Direct3D10_I*>(m_device);

	if (m_res->m_effect10 == NULL)
	{
		UINT dwShaderFlags = 0;
		ID3D10Blob* errors = NULL;
		HRESULT hr = D3DX10CreateEffectFromFile(L"C:/mmstudio/techniques.fx", NULL, NULL, "fx_4_0", dwShaderFlags, 0, d3d10->m_device, NULL,
																 NULL, &m_res->m_effect10, &errors, NULL);
		if (FAILED(hr))
		{
			if (errors)
			{
				const char* str = (const char*)errors->GetBufferPointer();
				errors->Release();
			}
		}

		m_res->m_technique0 = m_res->m_effect10->GetTechniqueByName("RenderSolidColor");
		m_res->m_technique1 = m_res->m_effect10->GetTechniqueByName("RenderLinearGradient");

		m_res->m_modelViewVariable = m_res->m_effect10->GetVariableByName("modelView")->AsMatrix();
		m_res->m_projectionVariable = m_res->m_effect10->GetVariableByName("projection")->AsMatrix();
		m_res->m_modelViewProjectionVariable = m_res->m_effect10->GetVariableByName("modelViewProjection")->AsMatrix();
		m_res->m_textureMatrix0Variable = m_res->m_effect10->GetVariableByName("textureMatrix0")->AsMatrix();
		m_res->m_texture0Variable = m_res->m_effect10->GetVariableByName("texture0")->AsShaderResource();
		m_res->m_solidColorVariable = m_res->m_effect10->GetVariableByName("solidColor")->AsVector();
	}

	if (m_res->m_vertexDeclaration1 == NULL)
	{
		// Create the input layout
		D3D10_PASS_DESC PassDesc;
		m_res->m_technique1->GetPassByIndex(0)->GetDesc(&PassDesc);

		VertexDeclElement elements[] =
		{
			{"POSITION", 0, 0, DECLTYPE_FLOAT3, 0},
			{"TEXCOORD", 0, 3*4, DECLTYPE_FLOAT2, 0}

		//	{"vPos", 0, 0, DECLTYPE_FLOAT3, 0},
		//	{"vTexCoord0", 0, 3*4, DECLTYPE_FLOAT2, 0}
		};

		m_res->m_vertexDeclaration1 = m_device->CreateVertexDeclaration(elements, 2, &PassDesc/*pG->m_cgVertexProgram0*/);

		/*
		if( FAILED( g_pd3dDevice->CreateInputLayout( layout, numElements, PassDesc.pIAInputSignature, 
				  PassDesc.IAInputSignatureSize, &g_pVertexLayout ) ) )
			 return FALSE;
		// Set the input layout
		g_pd3dDevice->IASetInputLayout( g_pVertexLayout );
*/
	}

	matrix4f modelView;
	m_device->GetFloatv(GL_MODELVIEW_MATRIX, (float*)modelView);

	matrix4f projection;
	m_device->GetFloatv(GL_PROJECTION_MATRIX, (float*)projection);

	m_res->m_modelViewVariable->SetMatrix(modelView);
	m_res->m_projectionVariable->SetMatrix(projection);
	m_res->m_modelViewProjectionVariable->SetMatrix(projection * modelView);

	m_device->SetVertexDeclaration(m_res->m_vertexDeclaration1);
}
#endif

void Direct3D10_I::VariableSetFloatVector(ID3DX10EffectVectorVariable* variable, const float* v)
{
	HRESULT hr = variable->SetFloatVector((float*)v);
	ASSERT(SUCCEEDED(hr));
}

void Direct3D10_I::VariableSetMatrix(ID3DX10EffectMatrixVariable* variable, const float* m)
{
	HRESULT hr = variable->SetMatrix((float*)m);
	ASSERT(SUCCEEDED(hr));
}

void Direct3D10_I::VariableSetResource(ID3DX10EffectShaderResourceVariable* variable, ID3D10ShaderResourceView* resourceView)
{
	HRESULT hr = variable->SetResource(resourceView);
	ASSERT(SUCCEEDED(hr));
}

ID3D10Blob* Direct3D10_I::CompileShader(LPCWSTR filename, LPCSTR profile)
{
	ID3D10Blob* pShader = NULL;
	ID3D10Blob* pErrorMsgs = NULL;

//	HRESULT hr = D3D10CompileShader(/*NULL, 0,*/ filename, NULL, NULL, "main", "vs_4_0", 0, &pShader, &pErrorMsgs);

	HRESULT hr = D3DX10CompileFromFile(filename, NULL, NULL, "main", profile,
		D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY,
		0,
		NULL, &pShader, &pErrorMsgs, NULL);

	if (FAILED(hr))
	{
		if (pErrorMsgs)
		{
			const char* str = (const char*)pErrorMsgs->GetBufferPointer();
			pErrorMsgs->Release();
		}
	}
	return pShader;
}

HRESULT Direct3D10_I::CreateVertexShader(ID3D10Blob* blob, ID3D10VertexShader** pp)
{
	return m_device->CreateVertexShader((DWORD*)blob->GetBufferPointer(), blob->GetBufferSize(), pp);
}

HRESULT Direct3D10_I::CreatePixelShader(ID3D10Blob* blob, ID3D10PixelShader** pp)
{
	return m_device->CreatePixelShader((DWORD*)blob->GetBufferPointer(), blob->GetBufferSize(), pp);
}

void Direct3D10_I::VSSetShader(ID3D10VertexShader* shader)
{
	m_device->VSSetShader(shader);
}

void Direct3D10_I::PSSetShader(ID3D10PixelShader* shader)
{
	m_device->PSSetShader(shader);
}

void Direct3D10_I::CreateRenderTargetView()
{
	ASSERT(0);
#if 0

	HRESULT hr;

// Create a render target view
	ID3D10Texture2D *pBackBuffer;
	if( FAILED( m_swapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer)))
		return;// NULL;

	hr = m_device->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	pBackBuffer->Release();
	if( FAILED( hr ) )
		return;// NULL;
	m_device->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
#endif
}

}	// Graphics
}	// System
