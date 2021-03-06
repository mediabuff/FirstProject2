#ifdef __LERSTAD__
#import "LFC/LFC.h"
#else
#include "LFC/LFC.h"
#endif

#ifdef __LERSTAD__
#import "GUI/GUI.h"
#else
#include "GUI/GUI.h"
#endif

#ifndef __LERSTAD__
#include <wia.h>
#endif

#ifndef __LERSTAD__
#include <boost/dynamic_bitset.hpp>
#endif

#ifndef IMAGEEDITEXT
#define IMAGEEDITEXT DECLSPEC_DLLIMPORT
#endif

namespace System
{
namespace ImageEdit
{

class ImageDocument;

class IMAGEEDITEXT WIADevice : public Object
{
public:

	CTOR WIADevice(WCHAR* deviceID) : m_deviceID(deviceID)
	{
	}

	WCHAR* m_deviceID;
};

interface IBitmapTransferCallback
{
	virtual void OnTransferBitmap(Graphics::Bitmap* bitmap) = 0;
	virtual void OnTransferDone() = 0;
};

class Mask;
class Element;
class Group;
class PathShape;
class BitmapEditor;

struct StrokeSample
{
	gm::PointD pos;
	double angle;
	double arclength;
	int segment;
};

class Path : public Object
{
public:

	class BezierPoint : public Object
	{
	public:

		CTOR BezierPoint()
		{
		}

		CTOR BezierPoint(gm::PointD p0, gm::PointD p1, gm::PointD p2)
		{
			m_points[0] = (p0);
			m_points[1] = (p1);
			m_points[2] = (p2);
		}

		BezierPoint Transform(const gm::matrix3f& transform)
		{
			return BezierPoint(
				transform.transform(m_points[0]),
				transform.transform(m_points[1]),
				transform.transform(m_points[2]));
		}

		gm::PointD m_points[3];
	};

	class SubPath : public Object
	{
	public:
		CTOR SubPath();

#if 0
		int DrawVertices(Graphics::GraphicsI* device, LDraw::BufferData* bufferData);
		void DrawVertices(LDraw::DrawLineStripCommand* pCmd, const gm::matrix3f& transform);
		void Stroke(function1<StrokeSample>& callback);
#endif

		void Reverse();
		void SetEllipse(double x, double y, double width, double height);
		void SetRectangle(double x, double y, double width, double height);

		bool get_IsClosed() const
		{
			return m_closed;
		}

		void set_IsClosed(bool isClosed);

		vector<BezierPoint*> m_data;
		bool m_closed;
	};

	CTOR Path();

	void AddEllipse(double x, double y, double width, double height);
	void AddRectangle(double x, double y, double width, double height);
#if 0
	void DrawOutline(Graphics::GraphicsI* device, LDraw::BufferData* bufferData, LDraw::Commands* pCommands, LDraw::Brush* brush, const gm::matrix3f& transform);
	void DrawFilled(Graphics::GraphicsI* device, LDraw::BufferData* bufferData, LDraw::Commands* pCommands, LDraw::Brush* brush);
	void Stroke(function1<StrokeSample>& callback);
#endif
	vector<SubPath*> m_subPath;
	uint m_refcount;
};

class IMAGEEDITEXT PaintBrush : public Object
{
public:
	void SetCircular();

	Graphics::Bitmap* m_bitmap;
};

class PaintToolSettings
{
public:
	PaintBrush* m_brush;

	double m_spacing;
};

class RenderTo
{
public:
	CTOR RenderTo()
	{
		m_offscreenBitmap[0] = NULL;
		m_offscreenBitmap[1] = NULL;
	}

	int Render(Gui::ManagedRenderContext pO, vector<Element*>& layers);

	Graphics::Bitmap* m_offscreenBitmap[2];
//	IDirect3DSurface9* m_offscreenSurface[2];

	int m_width;
	int m_height;
};

class IMAGEEDITEXT Element : public Object//, public INotifyPropertyChanged
{
public:
	CTOR Element(ImageDocument* document);

//	virtual int Render(IDirect3DSurface9* renderTarget, IDirect3DSurface9* renderTarget2, LDraw::Bitmap* targetBitmap, LDraw::Bitmap* targetBitmap2, UI::Graphics* pGraphics) = 0;
	virtual int Render(ID3D10RenderTargetView* renderTarget, ID3D10RenderTargetView* renderTarget2, Graphics::Bitmap* targetBitmap, Graphics::Bitmap* targetBitmap2, Gui::ManagedRenderContext pGraphics) = 0;
	virtual Graphics::Bitmap* get_Bitmap() = 0;

	double get_Opacity();
	void set_Opacity(double opacity);

	virtual Element* Clone() = 0;

	virtual String get_Name()
	{
		return m_name;
	}

	/*
	virtual signal2<Object*, StringW*>& get_PropertyChanged()
	{
		return PropertyChanged;
	}
	*/

	Graphics::Color get_DisplayColor();

	uint AddRef()
	{
		++m_refcount;
		return m_refcount;
	}

	uint Release()
	{
		--m_refcount;
		return m_refcount;
	}

public:

//	signal2<Object*, StringW*> PropertyChanged;

	ImageDocument* m_document;
	Group* m_parent;
	Mask* m_mask;

	String m_name;
	gm::matrix3f m_transform;

	gm::RectD m_bounds;

	double m_opacity;
	uint32 m_blendMode;
	bool m_visible;
	bool m_boundsValid;

	uint m_refcount;
};

class Adjustment : public Object//, public INotifyPropertyChanged
{
public:

	/*
	virtual signal2<Object*, StringW*>& get_PropertyChanged()
	{
		return PropertyChanged;
	}

	signal2<Object*, StringW*> PropertyChanged;
	*/
};

/*
class HistogramView : public FrameworkElement
{
public:

	void 

	LDraw::Bitmap* get_Bitmap();
	void set_Bitmap(LDraw::Bitmap* bitmap);

	static DependencyProperty* s_BitmapProperty;
};
*/

class BrightnessContrastAdjustment : public Adjustment
{
public:

	CTOR BrightnessContrastAdjustment();

	double get_Brightness();
	void set_Brightness(double brightness);

	double get_Contrast();
	void set_Contrast(double contrast);

	double m_brightness;
	double m_contrast;
};

class LevelsAdjustment : public Adjustment
{
public:

	// 0=rgb, 1=r, 2=g, 3=g, 

	float gamma[5];
	float inputblack[5];
	float inputwhite[5];
	float outputblack[5];
	float outputwhite[5];
};

class Gradient : public Object
{
public:
};

class IMAGEEDITEXT Mask : public Object
{
public:
	Graphics::Bitmap* m_bitmap;
};

class GradientMask : public Mask
{
public:
};

class IMAGEEDITEXT VisualMask : public Mask
{
public:

	CTOR VisualMask();
	CTOR VisualMask(Gui::Visual* visual);

	Gui::Visual* m_visual;
};

class IMAGEEDITEXT ElementMask : public Mask
{
public:

	CTOR ElementMask();
	CTOR ElementMask(Element* element);

	Element* m_element;
};

class IMAGEEDITEXT AdjustmentLayer : public Element
{
public:
	CTOR AdjustmentLayer(ImageDocument* document);

	virtual Element* Clone();

	virtual int Render(ID3D10RenderTargetView* renderTarget, ID3D10RenderTargetView* renderTarget2, Graphics::Bitmap* targetBitmap, Graphics::Bitmap* targetBitmap2, Gui::ManagedRenderContext pGraphics);
	virtual Graphics::Bitmap* get_Bitmap();

	Adjustment* get_Adjustment();

//	void OnAdjustmentPropertyChanged(Object* sender, StringW* propertyName);

	Adjustment* m_adjustment;
};

/*
class Shape : public Object
{
public:
	virtual void Render(UI::Graphics* pGraphics) = 0;
};

class RectangleShape : public Shape
{
public:
};

class EllipseShape : public Shape
{
};
*/


class IMAGEEDITEXT Shape : public Element
{
public:

	virtual Graphics::Bitmap* get_Bitmap()
	{
		return NULL;
	}

protected:
	CTOR Shape(ImageDocument* document);
};

class PathShape : public Shape
{
public:

	CTOR PathShape(ImageDocument* document);

//	virtual int Render(IDirect3DSurface9* renderTarget, IDirect3DSurface9* renderTarget2, LDraw::Bitmap* targetBitmap, LDraw::Bitmap* targetBitmap2, UI::Graphics* pGraphics);
	virtual int Render(ID3D10RenderTargetView* renderTarget, ID3D10RenderTargetView* renderTarget2, Graphics::Bitmap* targetBitmap, Graphics::Bitmap* targetBitmap2, Gui::ManagedRenderContext pGraphics);
//	virtual void Render(UI::Graphics* pGraphics);

	virtual PathShape* Clone();

	void DrawBrush(Gui::ManagedRenderContext pGraphics, StrokeSample sample);

	virtual void CalculateBounds();

	PaintBrush* m_paintBrush;

	Path* m_path;
};

class IMAGEEDITEXT Group : public Element
{
public:

	CTOR Group(ImageDocument* document);

	virtual int Render(ID3D10RenderTargetView* renderTarget, ID3D10RenderTargetView* renderTarget2, Graphics::Bitmap* targetBitmap, Graphics::Bitmap* targetBitmap2, Gui::ManagedRenderContext pGraphics);
//	virtual int Render(IDirect3DSurface9* renderTarget, IDirect3DSurface9* renderTarget2, LDraw::Bitmap* targetBitmap, LDraw::Bitmap* targetBitmap2, UI::Graphics* pGraphics);
	virtual Group* Clone();

	virtual Graphics::Bitmap* get_Bitmap();

	vector<Element*> m_subObjects;

//	RenderTo m_renderTo;
	Graphics::Bitmap* m_offscreenBitmap;
//	IDirect3DSurface9* m_offscreenSurface;
};

class IMAGEEDITEXT Layer : public Group
{
public:

	CTOR Layer(ImageDocument* document);

	virtual Layer* Clone();
	virtual Graphics::Bitmap* get_ThumbnailBitmap() = 0;

	Graphics::Color m_color;
};

/*
class GroupLayer : public Layer
{
public:

	virtual void Render(IDirect3DSurface9* renderTarget, LDraw::Bitmap* targetBitmap2, UI::Graphics* pGraphics);
	virtual LDraw::Bitmap* get_ThumbnailBitmap();
};
*/

/*
class ShapesLayer : public Layer
{
public:
	CTOR ShapesLayer();

	virtual void Render(IDirect3DSurface9* renderTarget, LDraw::Bitmap* targetBitmap2, UI::Graphics* pGraphics);
	virtual LDraw::Bitmap* get_Bitmap();
	virtual LDraw::Bitmap* get_ThumbnailBitmap();

	LDraw::Bitmap* m_bitmap;

	vector<Shape*> m_shapes;
};
*/

class Video : public Object
{
public:
};


class IMAGEEDITEXT Selected : public Object
{
public:

	CTOR Selected(Element* element)
	{
		m_element = element;
	}

	Element* m_element;
};

class PathSelected : public Selected
{
public:
	CTOR PathSelected(PathShape* path);

#if 0
	int Draw(Graphics::GraphicsI* device, LDraw::BufferData* bufferData, LDraw::Commands* pCommands, LDraw::Brush* brush);
#endif
	class SubPath
	{
	public:
		vector<int> m_selectedPoints;
		boost::dynamic_bitset<unsigned long, __gc_allocator> m_isSelected;
	};

	inline PathShape* GetPath()
	{
		return static_cast<PathShape*>(m_element);
	}

	vector<SubPath*> m_subPath;
};

class SelectedItems : public Object
{
public:

	Selected* m_target;
	vector<Selected*> m_items;
};

class IMAGEEDITEXT BElement : public Gui::Visual
{
protected:

	CTOR BElement(Gui::DependencyClass* depClass);

public:

	DEPENDENCY_CLASS_ABSTRACT(BElement, Gui::Visual)

	Mask* m_mask;
	ImageDocument* m_document;
};

}
}

#include "StrokeVisual.h"
#include "MaskOutlineVisual.h"

namespace System
{
namespace ImageEdit
{

class IMAGEEDITEXT ColorPicker : public Gui::UIElement
{
public:

	CTOR ColorPicker();

	virtual void RenderRetained(Gui::ManagedRetainedRenderContext pGraphics) override;
	virtual void OnLeftMouseButtonDown(Gui::MouseButtonEventArgs* args) override;
	virtual void OnLeftMouseButtonUp(Gui::MouseButtonEventArgs* args) override;
	virtual void OnMouseMove(Gui::MouseEventArgs* args) override;

	static Gui::DependencyClass* get_Class();

	DEPENDENCY_PROPERTY(float, Red)
	DEPENDENCY_PROPERTY(float, Green)
	DEPENDENCY_PROPERTY(float, Blue)

private:

	static Gui::DependencyClass* pClass;
};

}
}

#include "ImageTool.h"
#include "HandTool.h"
#include "MagnifyTool.h"
#include "ShapeTool.h"
#include "TransformTool.h"
#include "PaintBrushTool.h"
#include "PencilTool.h"
#include "SelectionTool.h"
#include "Tools.h"

#include "ImageDocument.h"
#include "BitmapEditor.h"
#include "BitmapLayer.h"

namespace System
{
namespace ImageEdit
{

IMAGEEDITEXT void SetMask(BElement* mask);
IMAGEEDITEXT void ReloadEffect();
IMAGEEDITEXT Graphics::Bitmap* CreateBitmap(uint width, uint height, DXGI_FORMAT pixelFormat);
IMAGEEDITEXT Gui::Visual* Ellipse();
IMAGEEDITEXT void Edit(Graphics::Bitmap* bitmap);

}	// ImageEdit
}	// System
