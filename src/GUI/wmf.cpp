#include "stdafx.h"
#include "GUI2.h"
//#include "wmf.h"

namespace System
{
namespace Gui
{

/*
OBJ_PEN Returns the current selected pen. 
OBJ_BRUSH Returns the current selected brush. 
OBJ_PAL Returns the current selected palette. 
OBJ_FONT Returns the current selected font. 
OBJ_BITMAP 
*/


MetafileLoader::MetafileLoader()
{
	// ???
	m_pCurPen = NULL;
	m_pCurBrush = NULL;
	m_pCurFont = NULL;

//	m_textColor = 0;
	//m_bBeginPath = false;
}

void MetafileLoader::FileType()
{
	pCommands = new Visuals;
	m_objects = new Graphics::wmfObject*[m_numOfObjects];
	memset(m_objects, 0, sizeof(Graphics::wmfObject*) * m_numOfObjects);
}

#define OBJ_PEN             1
#define OBJ_BRUSH           2
#define OBJ_DC              3
#define OBJ_METADC          4
#define OBJ_PAL             5
#define OBJ_FONT            6
#define OBJ_BITMAP          7
#define OBJ_REGION          8
#define OBJ_METAFILE        9
#define OBJ_MEMDC           10
#define OBJ_EXTPEN          11
#define OBJ_ENHMETADC       12
#define OBJ_ENHMETAFILE     13
#define OBJ_COLORSPACE      14

void MetafileLoader::SelectObject(int n)
{
	if (n < 0 || n >= m_numOfObjects)
	{
	//	TRACE("SelectObject out of bounds\n");
		ASSERT(0);
		THROW(-1);
	}

	if (m_objects[n] == nullptr)
	{
	//	TRACE("SelectObject NULL object\n");
		ASSERT(0);
		THROW(-1);
	}

	if (m_objects[n]->m_type == OBJ_PEN)
	{
		m_pCurPen = (ObjectPen*)m_objects[n];
	}
	else if (m_objects[n]->m_type == OBJ_BRUSH)
	{
		m_pCurBrush = (ObjectBrush*)m_objects[n];
	}
	else if (m_objects[n]->m_type == OBJ_FONT)
	{
		m_pCurFont = (ObjectFont*)m_objects[n];
	}
	else
		ASSERT(0);
}

void MetafileLoader::DeleteObject(int n)
{
	if (n < 0 || n >= m_numOfObjects)
	{
		raise(Exception("DeleteObject out of bounds\n"));
	}

	if (m_objects[n] == NULL)
	{
		raise(Exception("DeleteObject NULL object\n"));
	}

	delete m_objects[n];
	m_objects[n] = nullptr;
}

void MetafileLoader::CreatePenIndirect(int index, const Graphics::LOGPEN& lp)
{
	ObjectPen* pPen = new ObjectPen;
	pPen->m_brush = new SolidColorBrush(GetRValue(lp.lopnColor)/255.0f, GetGValue(lp.lopnColor)/255.0f, GetBValue(lp.lopnColor)/255.0f);
	pPen->m_width = lp.lopnWidth;

	m_objects[index] = pPen;
}

void MetafileLoader::CreatePenIndirect(int index, const Graphics::EXTLOGPEN& lp)
{
	ObjectPen* pPen = new ObjectPen;
	pPen->m_brush = new SolidColorBrush(GetRValue(lp.elpColor)/255.0f, GetGValue(lp.elpColor)/255.0f, GetBValue(lp.elpColor)/255.0f);
	pPen->m_width = lp.elpWidth;

	m_objects[index] = pPen;
}

void MetafileLoader::CreateBrushIndirect(int index, const Graphics::LOGBRUSH& lb)
{
	ObjectBrush* pBrush = new ObjectBrush;
	pBrush->m_pBrush = new SolidColorBrush(GetRValue(lb.lbColor)/255.0f, GetGValue(lb.lbColor)/255.0f, GetBValue(lb.lbColor)/255.0f);

//	new SolidColorBrush(GetRValue(lb.lbColor)/255.0f, GetGValue(lb.lbColor)/255.0f, GetBValue(lb.lbColor)/255.0f);

	m_objects[index] = pBrush;
}

void MetafileLoader::CreateFontIndirect(int index, const Graphics::LOGFONT_16& lf)
{
	ObjectFont* pObject = new ObjectFont;
	ASSERT(0);
	//pObject->m_pFont = new LDraw::Font(lf.lfFaceName, -lf.lfHeight);

	m_objects[index] = pObject;
}

void MetafileLoader::CreateFontIndirect(int index, const Graphics::LOGFONTA& lf)
{
	ObjectFont* pObject = new ObjectFont;
	ASSERT(0);
//	pObject->m_pFont = new LDraw::Font(lf.lfFaceName, -lf.lfHeight);

	m_objects[index] = pObject;
}

void MetafileLoader::CreateFontIndirect(int index, const Graphics::LOGFONTW& lf)
{
	ObjectFont* pObject = new ObjectFont;

	ASSERT(0);
	// pObject->m_pFont = new LDraw::Font(lf.lfFaceName, -lf.lfHeight);

	m_objects[index] = pObject;
}

void MetafileLoader::MoveTo(int x, int y)
{
	m_curx = x;
	m_cury = y;

	if (m_bBeginPath)
	{
		m_path->LineTo(LPToDP(x, y));
	}
}

#define TA_LEFT                      0
#define TA_RIGHT                     2
#define TA_CENTER                    6

#define TA_TOP                       0
#define TA_BOTTOM                    8
#define TA_BASELINE                  24

void MetafileLoader::ExtTextOut(int x, int y, StringIn string, const int* distances)
{
	ASSERT(0);
#if 0

	LDraw::DrawStringAPointCommand* pCmd = new LDraw::DrawStringAPointCommand;

#if 0
	pCmd->m_string = string;
#endif
	if (m_pCurFont->m_pFont)
		pCmd->m_font = m_pCurFont->m_pFont;

	if (m_textAlign & TA_BASELINE)
	{
		y -= pCmd->m_font->GetCellAscent();
	}
	else if (m_textAlign & TA_BOTTOM)
	{
	}
	else if (m_textAlign & TA_TOP)
	{
	}

	//pCmd->m_point.X += bounds.left;

	if (m_textAlign & TA_CENTER)
	{
	}
	else if (m_textAlign & TA_LEFT)
	{
	}
	else if (m_textAlign & TA_RIGHT)
	{
	}

	pCmd->m_point.X = x;
	pCmd->m_point.Y = y;

	pCmd->m_brush = new LDraw::SolidBrush(LDraw::Color(GetRValue(m_textColor), GetGValue(m_textColor), GetBValue(m_textColor)));

	if (distances)
	{
		pCmd->m_distances = new float[string.GetLength()];
		for (int i = 0; i < string.GetLength(); i++)
		{
			pCmd->m_distances[i] = distances[i];
		}
	}

	pCommands->Add(pCmd);
#endif
}

void MetafileLoader::FillPath()
{
	FillGeometryVisual* pCmd = new FillGeometryVisual;
	if (m_pCurBrush->m_pBrush)
	{
		pCmd->set_Brush(m_pCurBrush->m_pBrush);
	}

	pCmd->set_Geometry(m_path);

	pCommands->Add(pCmd);

	m_path = new PathGeometry;//->Reset();
}

void MetafileLoader::StretchDIBits(int destX, int destY, int destWidth, int destHeight, int srcX, int srcY, int srcWidth, int srcHeight, Graphics::Bitmap* bitmap)
{
	BitmapVisual* pCmd = new BitmapVisual;

	pCmd->set_DestRect(new RectangleGeometry(gm::RectF(float(destX), float(destY), float(destWidth), float(destHeight))));

	/*
	pCmd->m_srcX = srcX;
	pCmd->m_srcY = srcY;
	pCmd->m_srcWidth = srcWidth;
	pCmd->m_srcHeight = srcHeight;
	*/
	BitmapSource* bitmap2 = new BitmapSource(bitmap);
	pCmd->set_Bitmap(bitmap2);

	pCommands->Add(pCmd);
}

}	// Gui
}	// System
