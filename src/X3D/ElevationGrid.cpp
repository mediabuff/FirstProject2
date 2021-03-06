#include "stdafx.h"
#include "X3D2.h"
#include "ElevationGrid.h"

namespace System
{
namespace x3d
{

void lglNormal(Graphics::Graphics3D* p, const gm::vector3f& c, const gm::vector3f& b, const gm::vector3f& a);

NodeType* ElevationGrid::GetNodeType()
{
	static X3DFieldDefinition height("height", FieldType_MFFloat, SAIFieldAccess_initializeOnly, offsetof(ElevationGrid, m_height));
	static X3DFieldDefinition xDimension("xDimension", FieldType_SFInt32, SAIFieldAccess_initializeOnly, offsetof(ElevationGrid, m_xDimension));
	static X3DFieldDefinition xSpacing("xSpacing", FieldType_SFFloat, SAIFieldAccess_initializeOnly, offsetof(ElevationGrid, m_xSpacing));
	static X3DFieldDefinition zDimension("zDimension", FieldType_SFInt32, SAIFieldAccess_initializeOnly, offsetof(ElevationGrid, m_zDimension));
	static X3DFieldDefinition zSpacing("zSpacing", FieldType_SFFloat, SAIFieldAccess_initializeOnly, offsetof(ElevationGrid, m_zSpacing));
//	static X3DFieldDefinition colorPerVertex(WSTR("colorPerVertex"), FieldType_SFBool, SAIFieldAccess_initializeOnly,-1),
//	static X3DFieldDefinition solid(WSTR("solid"), FieldType_SFBool, SAIFieldAccess_initializeOnly,-1),
//	static X3DFieldDefinition(WSTR("ccw"), FieldType_SFBool, SAIFieldAccess_initializeOnly,-1),

	static X3DFieldDefinition* ElevationGridFields[] =
	{
		&height,
		&xDimension,
		&xSpacing,
		&zDimension,
		&zSpacing,
//		&colorPerVertex"), FieldType_SFBool, SAIFieldAccess_initializeOnly,-1),
//		&solid"), FieldType_SFBool, SAIFieldAccess_initializeOnly,-1),
//		&X3DFieldDefinition(WSTR("ccw"), FieldType_SFBool, SAIFieldAccess_initializeOnly,-1),
		/*
		MFFloat [in]     set_height
		SFNode  [in,out] color           NULL [X3DColorNode]
		SFNode  [in,out] metadata        NULL [X3DMetadataObject]
		SFNode  [in,out] normal          NULL [X3DNormalNode]
		SFNode  [in,out] texCoord        NULL [X3DTextureCoordinateNode]
		SFBool  []       ccw             TRUE  
		SFBool  []       colorPerVertex  TRUE
		SFFloat []       creaseAngle     0    [0,8)
		MFFloat []       height          []   (-8,8)
		SFBool  []       normalPerVertex TRUE
		SFBool  []       solid           TRUE
		SFInt32 []       xDimension      0    [0,8)
		SFFloat []       xSpacing        1.0  (0,8)
		SFInt32 []       zDimension      0    [0,8)
		SFFloat []       zSpacing        1.0  (0,8)
		*/
	};

	static NodeType nodeType("ElevationGrid", typeid(ElevationGrid), ElevationGridFields, _countof(ElevationGridFields), X3DGeometryNode::GetNodeType());
	return &nodeType;
}

NodeType* ElevationGrid::nodeType(GetNodeType());

ElevationGrid::ElevationGrid() : X3DGeometryNode(GetNodeType())
{
// Set defaults
	m_xSpacing->m_value = 1.0;
	m_zSpacing->m_value = 1.0;
}

float ElevationGrid::GetHeightAt(int x, int z)
{
	int cols = m_xDimension->getValue();
	int rows = m_zDimension->getValue();

	if (x < 0) x = 0;
	else if ( x > cols-1) x = cols-1;

	if (z < 0) z = 0;
	else if (z > rows-1) z = rows-1;

	return m_height->get1Value(z*rows + x);
}

Vec3f ElevationGrid::GetVertexAt(int x, int z)
{
	int cols = m_xDimension->getValue();
	int rows = m_zDimension->getValue();

	float xSpacing = m_xSpacing->getValue();
	float zSpacing = m_zSpacing->getValue();

	float height = GetHeightAt(x, z);

	return Vec3f((x-cols) * xSpacing / 2, height, ((z-rows) * zSpacing)/2);
}

class Triangle
{
public:

	CTOR Triangle(Vec3f a, Vec3f b, Vec3f c) : m_a(a), m_b(b), m_c(c)
	{
	}

	Vec3f normal() const
	{
		return cross(m_b-m_a, m_c-m_a);
	}

	Vec3f m_a, m_b, m_c;
};

class TriangleList
{
public:

	CTOR TriangleList(Triangle* triangle, TriangleList* next) : m_triangle(triangle), m_next(next)
	{
	}

	TriangleList* m_next;
	Triangle* m_triangle;
};

void ElevationGrid::Draw(X3DDrawContext* pDC, X3DTextureNodeImplImpl* pTextureNode)
{
	int rows = m_zDimension->getValue();
	int cols = m_xDimension->getValue();

	if (m_texture == nullptr)
	{
		// Create the render target texture
		D3D10_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = cols;
		desc.Height = rows;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R32_FLOAT;
		desc.SampleDesc.Count = 1;
	//	desc.Usage = D3D10_USAGE_DEFAULT;
		desc.Usage = D3D10_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;

		D3D10_SUBRESOURCE_DATA initialData;
		initialData.pSysMem = m_height->m_items.GetData();
		initialData.SysMemPitch = cols*sizeof(float);

		pDC->m_renderContext->GetRT()->m_d3d10->m_device->CreateTexture2D(&desc, &initialData, &m_texture);

		D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;

		pDC->m_renderContext->GetRT()->m_d3d10->m_device->CreateShaderResourceView(m_texture, &srvDesc, &m_textureView);
	}

#if 0
	int rows = m_zDimension->getValue();
	int cols = m_xDimension->getValue();

	if (m_height->m_items.GetSize() >= cols * rows)
	{
		if (m_d3d10_VertexBuffer == NULL)
		{
			float xSpacing = m_xSpacing->getValue();
			float zSpacing = m_zSpacing->getValue();

			LDraw::VERTEX_XYZ_NORMAL* vertices = new LDraw::VERTEX_XYZ_NORMAL[(cols)*(rows)];
			uint32* indices = new uint32[cols*rows*6];

			LDraw::VERTEX_XYZ_NO* pvertex = vertices;
			uint32* pindex = indices;

			float rwidth = 1.0f/(cols);
			float rheight = 1.0f/(rows);

			/*
			LDraw::vector3f* faceNormals = new LDraw::vector3f[cols*rows];

			int i = 0;

			for (int y = 0; y < rows; ++y)
			{
				for (int x = 0; x < cols; ++x)
				{
					LDraw::vector3f p = GetVertexAt(x, y);

					LDraw::vector3f u = GetVertexAt(x, y-1) - p;
					LDraw::vector3f v = GetVertexAt(x-1, y) - p;

					LDraw::vector3f n = normalize(cross(u,v));

					faceNormals[y*rows + x] = n;
				}
			}
			*/

			vector<Triangle> faces;

			for (int y = 1; y < rows; ++y)
			{
				for (int x = 1; x < cols; ++x)
				{
					Vec3f a = GetVertexAt(x-1, y);
					Vec3f b = GetVertexAt(x, y);
					Vec3f c = GetVertexAt(x-1, y-1);
					Vec3f d = GetVertexAt(x, y-1);

					Triangle face0(a,b,c);
					Triangle face1(b,d,c);

					faces.push_back(face0);
					faces.push_back(face1);

					*pindex++ = (y)*(rows) + x-1;
					*pindex++ = (y)*(rows) + x;
					*pindex++ = (y-1)*(rows) + x-1;

					*pindex++ = (y)*(rows) + x;
					*pindex++ = (y-1)*(rows) + x;
					*pindex++ = (y-1)*(rows) + x-1;
				}
			}

			/*
			for (int y = 0; y < rows; ++y)
			{
				for (int x = 0; x < cols; ++x)
				{
					*pindex++ = (y+1)*(rows+1) + x;
					*pindex++ = (y+1)*(rows+1) + x+1;
					*pindex++ = (y+0)*(rows+1) + x;

					*pindex++ = (y+1)*(rows+1) + x+1;
					*pindex++ = (y+0)*(rows+1) + x+1;
					*pindex++ = (y+0)*(rows+1) + x;
				}
			}
			*/

			{
				D3D10_BUFFER_DESC bd;
				bd.Usage = D3D10_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(LDraw::VERTEX_XYZ_T2)*(cols+1)*(rows+1);
				bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
				bd.CPUAccessFlags = 0;
				bd.MiscFlags = 0;
				D3D10_SUBRESOURCE_DATA InitData;
				InitData.pSysMem = vertices;
				renderContext.m_p->GetRT()->m_d3d10->m_device->CreateBuffer(&bd, &InitData, &m_d3d10_VertexBuffer);
			}

			{
				D3D10_BUFFER_DESC bd;
				bd.Usage = D3D10_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(uint32)*cols*rows*6;
				bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
				bd.CPUAccessFlags = 0;
				bd.MiscFlags = 0;
				D3D10_SUBRESOURCE_DATA InitData;
				InitData.pSysMem = indices;
				renderContext.m_p->GetRT()->m_d3d10->m_device->CreateBuffer(&bd, &InitData, &m_d3d10_IndexBuffer);
			}
		}

#if 0
		for (int zn = 1; zn < m_zDimension->m_value; zn++)
		{
			pDC->m_pGraphics3D->glBegin(GL_TRIANGLES);

			for (int xn = 1; xn < m_xDimension->m_value; xn++)
			{
				float x0 = ((xn-1) * m_xSpacing->m_value);
				float z0 = ((zn-1) * m_zSpacing->m_value);
				float x1 = (xn * m_xSpacing->m_value);
				float z1 = (zn * m_zSpacing->m_value);

				int index = zn*m_xDimension->m_value + xn;

				LDraw::vector3f a(x0, m_height->m_items[index-m_xDimension->m_value-1], z0);
				LDraw::vector3f b(x0, m_height->m_items[index-1], z1);
				LDraw::vector3f c(x1, m_height->m_items[index], z1);
				LDraw::vector3f d(x1, m_height->m_items[index-m_xDimension->m_value], z0);

				lglNormal(pDC->m_pGraphics3D, a, b, c);
				pDC->m_pGraphics3D->glVertex(a);
				pDC->m_pGraphics3D->glVertex(b);
				pDC->m_pGraphics3D->glVertex(c);

				lglNormal(pDC->m_pGraphics3D, c, d, a);
				pDC->m_pGraphics3D->glVertex(c);
				pDC->m_pGraphics3D->glVertex(d);
				pDC->m_pGraphics3D->glVertex(a);
			}

			pDC->m_pGraphics3D->glEnd();
		}
	}
	else
	{
		// error
	}
#endif
#endif
}

ElevationGrid* CreateFromBitmap(Graphics::Bitmap* bitmap)
{
	return NULL;
}

}	// x3d
}
