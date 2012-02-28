/**
@file GUI include
@defgroup GUI
*/

#ifndef GUI_h
#define GUI_h

#ifndef GUIEXT
#define GUIEXT DECLSPEC_DLLIMPORT
#endif

#ifdef __LERSTAD__
#import "LFC/LFC.h"
#else
#include "LFC/LFC.h"
#endif

#ifdef __LERSTAD__
#import "Networking/Networking.h"
#else
#include "Networking/Networking.h"
#endif

#ifdef __LERSTAD__
#import "Graphics/Graphics.h"
#else
#include "Graphics/Graphics.h"
#endif

#ifdef __LERSTAD__
#import "DShowPlayer/DShowPlayer.h"
#else
#include "DShowPlayer/DShowPlayer.h"
#endif

#ifdef __LERSTAD__
#import "CSS/CSS.h"
#else
#include "CSS/CSS.h"
#endif

#ifdef __LERSTAD__
#import "XML/XML.h"
#else
#include "XML/XML.h"
#endif

#ifdef __LERSTAD__
#import "Expressive/Expressive.h"
#else
#include "Expressive/Expressive.h"
#endif

#ifdef __LERSTAD__
//struct IGraphBuilder : public IUnknown {};
struct ICaptureGraphBuilder2 : public IUnknown {};

struct IDropTargetHelper : public IUnknown {};
struct IDataObject : public IUnknown {};

#else
#include <dshow.h>
#undef GetWindowStyle
#undef GetWindowExStyle
#endif

#ifndef __LERSTAD__
#include <UIAutomationCore.h>
#include <UIAutomationClient.h>
#include <UIAutomationCoreApi.h>
#endif

#undef ChooseColor

typedef IDataObject __IDataObject;

namespace System
{

class GUIEXT DdeInstance
{
public:
	CTOR DdeInstance() : m_id(0), m_lockCount(0)
	{
	}

	~DdeInstance()
	{
		if (m_lockCount != 0)
		{
			ASSERT(0);
		}

		if (m_id)
		{
			DdeUninitialize(m_id);
			m_id = 0;
		}
	}

#ifndef __LERSTAD__

	UINT Initialize(PFNCALLBACK pfnCallback, DWORD afCmd)
	{
		ASSERT(m_id == 0);

		UINT ddeError = DdeInitializeW(&m_id,      // receives instance identifier 
			pfnCallback, // pointer to callback function 
			afCmd,
			0);

		return ddeError;
	}

	UINT GetLastError() const
	{
		return DdeGetLastError(m_id);
	}

	HDDEDATA NameService(HSZ hsz1, UINT afCmd)
	{
		return DdeNameService(m_id, hsz1, 0L, afCmd);
	}

	HCONV Connect(HSZ hszService, HSZ hszTopic, PCONVCONTEXT pCC)
	{
		return DdeConnect(m_id, hszService, hszTopic, pCC);
	}

	HSZ CreateStringHandle(LPTSTR psz, int iCodePage = CP_WINUNICODE)
	{
		return DdeCreateStringHandle(m_id, psz, iCodePage);
	}

	DWORD QueryString(
		HSZ hsz,
		LPTSTR psz,
		DWORD cchMaxChars,
		int iCodePage = CP_WINUNICODE)
	{
		return DdeQueryString(m_id, hsz, psz, cchMaxChars, iCodePage);
	}
#endif

	operator DWORD () const
	{
		return m_id;
	}

	DWORD m_id;
	UINT m_lockCount;
};

}	// System


// TODO separate dll
#include "Expressive.h"

#ifdef _M_X64
#define NX64	// PhysX
#endif

#ifdef __LERSTAD__

typedef GUID KNOWNFOLDERID;

typedef DWORD COLORREF;

struct IXAudio2SourceVoice;
struct IXAudio2SubmixVoice;
struct IXAudio2MasteringVoice;

struct IXAudio2;
struct IXAudio2SourceVoice;

#endif

namespace physx
{
class PxFoundation;
class PxPhysics;
class PxScene;
class PxActor;
class PxRigidActor;
class PxRigidBody;
class PxRigidDynamic;
class PxRigidStatic;
}

/*
template<int place1, int place2, int place3> class Swizzled
{
public:
	float* p;

	__property float3 get_xyz() const
	{
		return float3(x, y, z);
	}

};
*/

#if 0
#include <xmmintrin.h>
#endif

#if 0
class GUIEXT /*__declspec(align(8))*/ float2
{
public:
	CTOR float2() : x(0), y(0)
	{
	}

	CTOR float2(float _x, float _y) : x(_x), y(_y)
	{
	}

	operator float* ()
	{
		return &x;
	}

	float x, y;
};

class GUIEXT /*__declspec(align(16))*/ float3
{
public:
	CTOR float3() : x(0), y(0), z(0)
	{
	}

	CTOR float3(float _x, float _y, float _z = 0.0f) : x(_x), y(_y), z(_z)
	{
	}

	operator float* ()
	{
		return &x;
	}

	float x, y, z;
};

class GUIEXT /*__declspec(align(16))*/ float4
{
public:
	inline CTOR float4() : x(0), y(0), z(0), w(0)
	{
	}

	inline CTOR float4(float _x, float _y, float _z = 0.0f, float _w = 1.0f) : x(_x), y(_y), z(_z), w(_w)
	{
	}

	inline operator float* ()
	{
		return &x;
	}

	inline operator const float* () const
	{
		return &x;
	}

	inline float& operator [] (int index)
	{
		return (&x)[index];
	}

	inline float operator [] (int index) const
	{
		return (&x)[index];
	}

	/*
#ifndef __LERSTAD__

	__property float3 get_xyz() const
	{
		return float3(x, y, z);
	}

	__property float3 get_yxz() const
	{
		return float3(y, x, z);
	}

	__property float3 get_yzx() const
	{
		return float3(y, z, x);
	}

	__property float3 get_xzy() const
	{
		return float3(x, z, y);
	}

	__property float3 get_zyx() const
	{
		return float3(z, y, x);
	}

#endif
*/

	float x, y, z, w;
//	__m128 xyzw;
};

class GUIEXT /*__declspec(align(16))*/ float4x3
{
public:
	float4 row[3];
};
#endif

namespace System
{

typedef gm::vector2f float2;
typedef gm::vector3f float3;
typedef gm::vector4f float4;

interface IObjectChangedNotify
{
	virtual void OnObjectChanged(Object*) abstract;
};

class GUIEXT /*__declspec(align(16))*/ float4x4
{
public:
	CTOR float4x4()
	{
		row[0] = float4(1, 0, 0, 0);
		row[1] = float4(0, 1, 0, 0);
		row[2] = float4(0, 0, 1, 0);
		row[3] = float4(0, 0, 0, 1);
	}

	CTOR float4x4(float4 row0, float4 row1, float4 row2, float4 row3)
	{
		row[0] = row0;
		row[1] = row1;
		row[2] = row2;
		row[3] = row3;
	}

	CTOR float4x4(gm::matrix3f& m)
	{
		row[0] = float4(m[0][0], m[0][1], m[0][2], 0);
		row[1] = float4(m[1][0], m[1][1], m[1][2], 0);
		row[2] = float4(m[2][0], m[2][1], m[2][2], 0);
		row[3] = float4(0, 0, 0, 1);
	}

	float4& operator [] (int index)
	{
		return row[index];
	}

	float4 operator [] (int index) const
	{
		return row[index];
	}

	float operator () (int rowIndex, int colIndex)
	{
		return row[rowIndex][colIndex];
	}

	operator float* ()
	{
		return (float*)this;
	}

	/*
	float4x4 transposed() const
	{
		return float4x4
	}
	*/

	float4 row[4];
};

}

#ifdef __LERSTAD__

struct ID3DX10Mesh;
struct ID3DX10SkinInfo;

// DirectWrite
struct IDWriteTextFormat;
struct IDWriteTextLayout;

typedef HANDLE HTHUMBNAIL;
typedef HTHUMBNAIL** PHTHUMBNAIL;

typedef HICON HCURSOR;

struct ID2D1StrokeStyle;

#else

#include <XAudio2.h>
#include <xaudio2fx.h>
#include <x3daudio.h>

#include <dwmapi.h>

#include <Shlobj.h>
#include <shobjidl.h>

//#undef SEVERITY_WARNING
//#undef SEVERITY_ERROR
//#undef SEVERITY_FATAL_ERROR

#endif

namespace System
{

enum TextChangeKind
{
	TextChangeKind_All,
	TextChangeKind_Insert,
	TextChangeKind_Delete,
	TextChangeKind_Replace,
};

struct TextSourceChangeEventArgs
{
	TextChangeKind type;
	size_t offset;
	size_t len;
};

interface ITextSource
{
	virtual Object* GetObject() abstract;
	virtual Event2<ITextSource*, TextSourceChangeEventArgs>& get_Changed() abstract;
	virtual void Insert(StringIn itext, size_t offset) abstract;
	virtual void Delete(size_t offset, size_t len) abstract;
};

class GUIEXT TextDocument : public IO::StringWriter, public ITextSource, public IHasMimetype
{
public:

	CTOR TextDocument();
	CTOR TextDocument(StringIn str);

	virtual Object* GetObject() override
	{
		return this;
	}

	virtual Event2<ITextSource*, TextSourceChangeEventArgs>& get_Changed() override
	{
		return Changed;
	}

	bool SaveAs(StringIn filename);

	virtual void Insert(StringIn itext, size_t offset) override;
	virtual void Delete(size_t offset, size_t len) override;

	String get_Mimetype()
	{
		return m_Mimetype;
	}

	void set_Mimetype(StringIn mimetype)
	{
		m_Mimetype = mimetype;
	}

	Object* get_Source()
	{
		return m_Source;
	}

	Object* m_Source;
	String m_Mimetype;

	static TextDocument* FromFile(StringIn filename);

protected:

	virtual void OnWritten(size_t len) override
	{
		TextSourceChangeEventArgs args;
		args.type = TextChangeKind_Insert;
		args.offset = str().GetLength()-len;
		args.len = len;

		Changed(this, args);
	}

private:

	Event2<ITextSource*, TextSourceChangeEventArgs> Changed;
};

class GUIEXT TimeDuration
{
public:
	CTOR TimeDuration(uint64 durationNanoSeconds);
	CTOR TimeDuration(double durationSeconds);

	double get_days();
	double get_hours();
	double get_minutes();
	double get_seconds();

	uint64 m_durationNanoSeconds;
};

IO::TextWriter& WriteToStream(IO::TextWriter& stream, const TimeDuration& duration);

interface ICamera
{
	virtual D3DXMATRIX GetProjectionMatrixLH(float width, float height, float zNear, float zFar) = 0;
	virtual D3DXMATRIX GetProjectionMatrixRH(float width, float height, float zNear, float zFar) = 0;
	virtual D3DXMATRIX GetViewMatrix() = 0;
	virtual D3DXMATRIX GetViewMatrixI() = 0;
};

// TODO, in LFC
template<class base, class derived> class offsetofclass
{
public:
	inline operator int ()
	{
		return ((int)static_cast<base*>((derived*)8)) - 8;
	}
};

/*
#define DEPENDENCY_CLASS(classname, baseclassname)	\
	public:	static Gui::DependencyClass* get_Class()	\
{	\
	static Gui::DependencyClass depclass INIT(typeid(classname), baseclassname::get_Class());	\
	return &depclass;	\
}	\
	typedef baseclassname baseClass;
*/

#define DEPENDENCY_CLASS_ABSTRACT(classname, baseclassname)	\
	public:	static Gui::DependencyClass* get_Class();	\
	typedef classname thisClass;	\
	typedef baseclassname baseClass;

#define DEPENDENCY_CLASS(classname, baseclassname)	\
	private: static System::Gui::DependencyClass* pClass;	\
	public:	static System::Gui::DependencyClass* get_Class();	\
	typedef classname thisClass;	\
	typedef baseclassname baseClass;

#define END_DEPENDENCY_CLASS()

#define GET_PROPERTY(name)	get_##name##Property()

#define DEPENDENCY_PROPERTY_RW(type, name)	\
	static System::Gui::TypedDependencyProperty<type>* get_##name##Property();	\
	type get_##name();	\
	System::Gui::PropertyValue* get_##name##Binding();	\
	void set_##name(type value);	\
	void set_##name##Input(System::Gui::typed<type> value);	\
	void set_##name##Expression(System::Expressive::Expression* exp);

#define DEPENDENCY_PROPERTY DEPENDENCY_PROPERTY_RW	// backward

#define DEPENDENCY_PROPERTY_RO(type, name)	\
	type get_##name();	\
	System::Gui::PropertyValue* get_##name##Binding();	\
	protected: void set_##name(type value);	\
	void set_##name##Input(System::Gui::typed<type> value);	\
	void set_##name##Expression(System::Expressive::Expression* exp);	\
	public: static System::Gui::TypedDependencyProperty<type>* get_##name##Property();

#define IMP_DEPENDENCY_PROPERTY(type, classname, name, defvalue)	\
	System::Gui::TypedDependencyProperty<type>* classname::get_##name##Property()	\
	{	\
		static System::Gui::TypedDependencyProperty<type> v(classname::get_Class(), L#name, defvalue);	\
		return &v;	\
	}	\
\
	type classname::get_##name()	\
	{	\
		return GetPropertyValueT<type>(GET_PROPERTY(name));	\
	}	\
\
	Gui::PropertyValue* classname::get_##name##Binding()	\
	{	\
		return GetProperty(GET_PROPERTY(name));	\
	}	\
\
	void classname::set_##name(type value)	\
	{	\
		SetPropertyValue(GET_PROPERTY(name), value);	\
	}	\
	void classname::set_##name##Input(Gui::typed<type> value)	\
	{	\
		Gui::SetBinding(this, GET_PROPERTY(name), value);	\
	}	\
	void classname::set_##name##Expression(Expressive::Expression* exp)	\
	{	\
		SetPropertyBinding(GET_PROPERTY(name), new Gui::ExpressionBinding(this, exp));	\
	}

#define IMP_DEPENDENCY_PROPERTY1(type, classname, name, defvalue, metadata)	\
	System::Gui::TypedDependencyProperty<type>* classname::get_##name##Property()	\
	{	\
		static Gui::TypedDependencyProperty<type> v(classname::get_Class(), L#name, defvalue, metadata);	\
		return &v;	\
	}	\
\
	type classname::get_##name()	\
	{	\
		return GetPropertyValueT<type>(GET_PROPERTY(name));	\
	}	\
\
	System::Gui::PropertyValue* classname::get_##name##Binding()	\
	{	\
		return GetProperty(GET_PROPERTY(name));	\
	}	\
\
	void classname::set_##name(type value)	\
	{	\
		SetPropertyValue(GET_PROPERTY(name), value);	\
	}	\
	void classname::set_##name##Input(Gui::typed<type> value)	\
	{	\
		System::Gui::SetBinding(this, GET_PROPERTY(name), value);	\
	}	\
	void classname::set_##name##Expression(Expressive::Expression* exp)	\
	{	\
		SetPropertyBinding(GET_PROPERTY(name), new Gui::ExpressionBinding(this, exp));	\
	}

#define IMP_DEPENDENCY_PROPERTY_T(T, type, classname, name, defvalue)	\
	template<class T> Gui::TypedDependencyProperty<type>* classname::get_##name##Property()	\
	{	\
		static Gui::TypedDependencyProperty<type> v(classname::get_Class(), L#name, defvalue);	\
		return &v;	\
	}	\
\
	template<class T> type classname::get_##name()	\
	{	\
		return GetPropertyValueT<type>(GET_PROPERTY(name));	\
	}	\
\
	template<class T> void classname::set_##name##Input(Gui::typed<type> value)	\
	{	\
		Gui::SetBinding(this, GET_PROPERTY(name), value);	\
	}	\
	template<class T> Gui::PropertyValue* classname::get_##name##Binding()	\
	{	\
		return GetProperty(GET_PROPERTY(name));	\
	}	\
\
	template<class T> void classname::set_##name(type value)	\
	{	\
		SetPropertyValue(GET_PROPERTY(name), value);	\
	}	\
	template<class T> void classname::set_##name##Expression(Expressive::Expression* exp)	\
	{	\
		SetPropertyBinding(GET_PROPERTY(name), new Gui::ExpressionBinding(this, exp));	\
	}

#define IMP_USE_DEPENDENCY_PROPERTY(type, classname, fromclassname, name)	\
	Gui::TypedDependencyProperty<type>* classname::get_##name##Property()	\
	{	\
		return fromclassname::get_##name##Property();	\
	}	\
\
	type classname::get_##name()	\
	{	\
		return GetPropertyValueT<type>(GET_PROPERTY(name));	\
	}	\
\
	void classname::set_##name(type value)	\
	{	\
		SetPropertyValue(GET_PROPERTY(name), value);	\
	}

#define ROUTEDEVENT_IMP0(cl, args, name)	\
protected:	virtual void On##name(args* _args) {} \
public: Event2<cl*,args*>& get_##name()	\
	{	\
	return *(Event2<cl*,args*>*)get_RoutedEvent(get_##name##Event());	\
	}	\
	static RoutedEvent* get_##name##Event();
//	static RoutedEvent* name##Event;

#define ROUTEDEVENT_IMP1(cl, args, name)	\
protected:	virtual void On##name(args* _args); \
public: Event2<cl*,args*>& get_##name()	\
	{	\
	return *(Event2<cl*,args*>*)get_RoutedEvent(get_##name##Event());	\
	}	\
	static RoutedEvent* get_##name##Event();
//	static RoutedEvent* name##Event;

#define ROUTEDEVENT(args, name)	\
public: Event2<thisClass*,args*>& get_##name()	\
	{	\
	return *(Event2<thisClass*,args*>*)get_RoutedEvent(get_##name##Event());	\
	}	\
	static RoutedEvent* get_##name##Event();\
protected:	virtual void On##name(args* _args)

#define do_once(x)	do {static bool done = false; if (!done) {done=true;x;}} while(0)

#define IMP_ROUTEDEVENT(eventName, className, strategy, type)	\
Gui::RoutedEvent* className::get_##eventName##Event()\
{\
	static Gui::RoutedEvent* v = Gui::EventManager::RegisterRoutedEvent(#eventName, strategy, type, (ClassType*)typeid(className).GetType());\
	do_once(Gui::EventManager::RegisterClassHandler((ClassType*)typeid(className).GetType(), v, mem_fun(&className::On##eventName)));\
	return v;\
}

#ifndef __LERSTAD__
namespace Expressive
{

template<> class typed_expression<StringObject*> : public _Object
{
public:
	CTOR typed_expression<StringObject*>(const expression& exp) : _Object(exp)
	{
	}
};
}

#endif

typedef Expressive::typed_expression<StringObject*> _String;

namespace Gui
{

interface IInputElement;
interface IRootVisual;

class Geometry;
class RoutedEvent;
class RoutedEventArgs;
class DependencyClass;
class DependencyProperty;
class DependencyObject;
class Brush;
class Visual;
class UIElement;
class PropertyValue;
class VisualCollection;
class Transform;
class Visuals;
class Element;
class Window;
class PlatformWindow;
class PlatformWindowSite;
class BitmapSource;
class GroupVisual;
class StrokeStyle;
class Cursor;
class ExpressionBinding;

class geometry;

class OleContainer;

namespace Media3D
{
class Transform3D;
}

class _DependencyObject;
class _Window;
class _Element;
class _UIElement;
class _FrameworkElement;
class _ContentElement;
//class _TextElement;
class _Control;
class _ContentControl;
class _Visual;
class _BitmapVisual;
class _Geometry;
class _RectangleGeometry;
class _EllipseGeometry;
class _Button;
class _GeneralTransform;
class _RotateTransform;
class _TextVisual;
class _WindowVisual;
class _FillGeometryVisual;
class _StrokeGeometryVisual;
class _RenderGeometryVisual;

class RenderContext;
class RetainedRenderContext;

#ifndef __LERSTAD__
D2D1_POINT_2F cnv(gm::PointF point);
gm::PointF cnv(D2D1_POINT_2F point);
#endif

interface IBitmapSource
{
	virtual Graphics::Bitmap* GetBitmap() abstract;

// TODO, changed
};

enum FlowDirection
{
	FlowDirection_LeftToRight,
	FlowDirection_RightToLeft
};

enum HorizontalAlignment
{
	HorizontalAlignment_Left,
	HorizontalAlignment_Center,
	HorizontalAlignment_Right,
	HorizontalAlignment_Stretch
};

class GUIEXT Thickness : public Object
{
public:

	CTOR Thickness() :
		m_left(0),
		m_right(0),
		m_top(0),
		m_bottom(0)
	{
	}

	CTOR Thickness(float value) :
		m_left(value),
		m_right(value),
		m_top(value),
		m_bottom(value)
	{
	}

	CTOR Thickness(float leftright, float topbottom) :
		m_left(leftright),
		m_right(leftright),
		m_top(topbottom),
		m_bottom(topbottom)
	{
	}

	CTOR Thickness(float left, float right, float top, float bottom) :
		m_left(left),
		m_right(right),
		m_top(top),
		m_bottom(bottom)
	{
	}

public:

	float m_left;
	float m_top;
	float m_right;
	float m_bottom;
};

interface IRootVisual
{
	virtual void InvalidateRender() = 0;
	virtual OleContainer* GetOleContainer() = 0;
	virtual Graphics::GraphicsI* GetGI() = 0;
	virtual bool ElementCaptureMouse(UIElement* element) = 0;
	virtual bool ElementReleaseMouseCapture(UIElement* element) = 0;
	virtual void ElementSetMouseCursor(UIElement* element) = 0;
	virtual void ElementSetFocus(UIElement* element) = 0;
};

class RectBorder
{
public:

	class _RectBorder : public Expressive::_object
	{
	public:

		typedef RectBorder type_t;

		virtual int get_sizeof() const
		{
			return sizeof(RectBorder);
		}

		Expressive::typed_expression<float> Left() const
		{
#ifndef __LERSTAD__
			return getMember(offsetof(RectBorder, Left));
#endif
		}

		Expressive::typed_expression<float> Top() const
		{
#ifndef __LERSTAD__
			return getMember(offsetof(RectBorder, Top));
#endif
		}

		Expressive::typed_expression<float> Right() const
		{
#ifndef __LERSTAD__
			return getMember(offsetof(RectBorder, Right));
#endif
		}

		Expressive::typed_expression<float> Bottom() const
		{
#ifndef __LERSTAD__
			return getMember(offsetof(RectBorder, Bottom));
#endif
		}
	};

	inline CTOR RectBorder() : Left(0), Top(0), Right(0), Bottom(0)
	{
	}

	inline CTOR RectBorder(float left, float top, float right, float bottom) : Left(left), Top(top), Right(right), Bottom(bottom)
	{
	}

	float Left;
	float Top;
	float Right;
	float Bottom;
};

/*
struct MATRIX_3X2_F
{
    float _11, _12;
    float _21, _22;
    float _31, _32;
};
*/

typedef RenderContext RenderTarget;

class GUIEXT EventArgs : public Object
{
public:
	CTOR EventArgs();
};

typedef gm::PointF Point;

interface IInputElement
{
//	virtual void AddHandler(RoutedEvent* routedEvent, EventHandler handler) = 0;
//	virtual void RemoveHandler(RoutedEvent* routedEvent, EventHandler handler) = 0;
	virtual void RaiseEvent(RoutedEventArgs* args) = 0;
	virtual bool CaptureMouse() = 0;
	virtual void ReleaseMouseCapture() = 0;
	virtual void SetCaptureMouse(bool b) = 0;
#if 0

	virtual bool Focus() = 0;

	virtual bool get_IsEnabled() = 0;
	virtual bool get_IsKeyboardFocused() = 0;
	virtual bool get_IsKeyboardFocusWithin() = 0;
	virtual bool get_IsMouseCaptured() = 0;
	virtual bool get_IsMouseOver() = 0;
	virtual bool get_IsMouseDirectlyOver() = 0;
#endif
};

interface ICommand
{
	virtual bool CanExecute(Object* parameter) = 0;
	virtual void Execute(Object* parameter) = 0;
	virtual Event1<Object*>& get_CanExecuteChanged() = 0;
};

interface ICommandSource
{
	virtual ICommand* get_Command() = 0;
	virtual Object* get_CommandParameter() = 0;
	virtual IInputElement* get_CommandTarget() = 0;
};

enum RoutingStrategy
{
	RoutingStrategy_Tunnel,
	RoutingStrategy_Bubble,
	RoutingStrategy_Direct,
};

}
}

#include "DataFormats.h"

#include "RoutedEvent.h"
#include "RoutedEventArgs.h"
#include "EventManager.h"

#include "MouseEventArgs.h"
#include "MouseButtonEventArgs.h"

/*
#include <afxwin.h>

CRgn th;
*/

namespace System
{

namespace Gdi
{

class GUIEXT GdiRgn
{
public:

	CTOR GdiRgn() : m_hRgn(nullptr)
	{
	}

	CTOR GdiRgn(decltype(nullptr)) : m_hRgn(nullptr)
	{
	}

	CTOR GdiRgn(const GdiRgn& otherRgn)
	{
		m_hRgn = CreateRectRgn(0,0,0,0);
		::CopyRgn(m_hRgn, otherRgn);
	}

	CTOR GdiRgn(GdiRgn&& other)	// moveable
	{
		m_hRgn = other.m_hRgn;
		other.m_hRgn = nullptr;
	}

	~GdiRgn()
	{
		if (m_hRgn)
		{
			DeleteObject(m_hRgn);
		}
	}

	GdiRgn& operator = (GdiRgn&& other)	// move
	{
		m_hRgn = other.m_hRgn;
		other.m_hRgn = nullptr;
		return *this;
	}

	GdiRgn& operator = (HRGN otherRgn)
	{
		if (m_hRgn == nullptr)
		{
			m_hRgn = CreateRectRgn(0,0,0,0);
		}
		::CopyRgn(m_hRgn, otherRgn);
		return *this;
	}

	bool Equal(HRGN otherRgn) const
	{
		return !!EqualRgn(m_hRgn, otherRgn);
	}

	void Translate(int x, int y)
	{
		OffsetRgn(m_hRgn, x, y);
	}

	void Translate(gm::PointI pt)
	{
		OffsetRgn(m_hRgn, pt.X, pt.Y);
	}

	GdiRgn& operator += (gm::PointI pt)
	{
		ASSERT(m_hRgn);
		OffsetRgn(m_hRgn, pt.X, pt.Y);
		return *this;
	}

	GdiRgn& operator -= (gm::PointI pt)
	{
		ASSERT(m_hRgn);
		OffsetRgn(m_hRgn, -pt.X, -pt.Y);
		return *this;
	}

	int Combine(HRGN hrgnSrc1, HRGN hrgnSrc2, int fnCombineMode	)
	{
		return CombineRgn(m_hRgn, hrgnSrc1, hrgnSrc2, fnCombineMode);
	}

	GdiRgn& operator |= (HRGN other)
	{
		CombineRgn(m_hRgn, m_hRgn, other, RGN_OR);
		return *this;
	}

	GdiRgn& operator &= (HRGN other)
	{
		CombineRgn(m_hRgn, m_hRgn, other, RGN_AND);
		return *this;
	}

	GdiRgn& operator ^= (HRGN other)
	{
		CombineRgn(m_hRgn, m_hRgn, other, RGN_XOR);
		return *this;
	}

	GdiRgn& operator -= (HRGN other)
	{
		CombineRgn(m_hRgn, other, m_hRgn, RGN_DIFF);
		return *this;
	}

	GdiRgn operator | (HRGN other) const
	{
		HRGN hRgn = CreateRectRgn(0,0,0,0);
		CombineRgn(hRgn, m_hRgn, other, RGN_OR);
		return hRgn;
	}

	GdiRgn operator & (HRGN other) const
	{
		HRGN hRgn = CreateRectRgn(0,0,0,0);
		CombineRgn(hRgn, m_hRgn, other, RGN_AND);
		return hRgn;
	}

	GdiRgn operator ^ (HRGN other) const
	{
		HRGN hRgn = CreateRectRgn(0,0,0,0);
		CombineRgn(hRgn, m_hRgn, other, RGN_XOR);
		return hRgn;
	}

	GdiRgn operator - (HRGN other) const
	{
		HRGN hRgn = CreateRectRgn(0,0,0,0);
		CombineRgn(hRgn, m_hRgn, other, RGN_DIFF);
		return hRgn;
	}

	bool Contains(int x, int y) const
	{
		return !!PtInRegion(m_hRgn, x, y);
	}

	bool Contains(gm::PointI pt) const
	{
		return !!PtInRegion(m_hRgn, pt.X, pt.Y);
	}

	bool IntersectsWith(const RECT& rect) const
	{
		return !!RectInRegion(m_hRgn, &rect);
	}

	bool IntersectsWith(HRGN hOther) const
	{
		HRGN hTemp = CreateRectRgn(0,0,0,0);
		int r = CombineRgn(hTemp, m_hRgn, hOther, RGN_AND);
		if (r == ERROR) throw new Exception("GdiRgn error");
		return r != NULLREGION;
	}

	bool operator !() const throw()
	{
		return m_hRgn == nullptr;
	}

	bool operator ==(decltype(nullptr)) const throw()
	{
		return m_hRgn == nullptr;
	}

	bool operator ==(HRGN otherRgn) const throw()
	{
		return Equal(otherRgn);
	}

	bool operator !=(HRGN otherRgn) const throw()
	{
		return !Equal(otherRgn);
	}

	RECT GetBBox() const
	{
		RECT rect;
		GetRgnBox(m_hRgn, &rect);
		return rect;
	}

#ifndef __LERSTAD__
	DWORD GetData(DWORD dwCount, LPRGNDATA lpRgnData)
	{
		return GetRegionData(m_hRgn, dwCount, lpRgnData);
	}
#endif

	HRGN Detach() throw()
	{
		HRGN hRgn = m_hRgn;
		m_hRgn = nullptr;
		return hRgn;
	}

	void Attach(HRGN hRgn)
	{
		if (m_hRgn != hRgn)
		{
			if (m_hRgn) DeleteObject(m_hRgn);
			m_hRgn = hRgn;
		}
	}

	operator HRGN () const throw()
	{
		return m_hRgn;
	}

	static GdiRgn FromHandle(HRGN hRgn)
	{
		return hRgn;
	}

	static GdiRgn FromRect(int x1, int y1, int x2, int y2)
	{
		return CreateRectRgn(x1, y2, x2, y2);
	}

	static GdiRgn FromRect(gm::RectI r)
	{
		return CreateRectRgn(r.GetLeft(), r.GetTop(), r.GetRight(), r.GetBottom());
	}

	static GdiRgn FromRect(gm::BBoxi r)
	{
		return CreateRectRgn(r.GetLeft(), r.GetTop(), r.GetRight(), r.GetBottom());
	}

	static GdiRgn FromEllipse(int x1, int y1, int x2, int y2)
	{
		return CreateEllipticRgn(x1, y2, x2, y2);
	}

	static GdiRgn FromEllipse(gm::RectI r)
	{
		return CreateEllipticRgn(r.GetLeft(), r.GetTop(), r.GetRight(), r.GetBottom());
	}

	static GdiRgn FromEllipse(gm::BBoxi r)
	{
		return CreateEllipticRgn(r.GetLeft(), r.GetTop(), r.GetRight(), r.GetBottom());
	}

#ifndef __LERSTAD__
	static GdiRgn FromData(const XFORM *lpXform, DWORD nCount, const RGNDATA *lpRgnData)
	{
		return ExtCreateRegion(lpXform, nCount, lpRgnData);
	}
#endif

protected:

	HRGN m_hRgn;

	CTOR GdiRgn(HRGN hRgn) : m_hRgn(hRgn)
	{
	}
};

}

namespace Gui
{

enum FontStyle
{
	FontStyle_Normal,
	FontStyle_Oblique,
	FontStyle_Italic
};

}

namespace Gui
{

class GUIEXT ColorF
{
public:

	CTOR ColorF() : r(0), g(0), b(0), a(0)
	{
	}

	CTOR ColorF(float _r, float _g, float _b) : r(_r), g(_g), b(_b), a(1.0f)
	{
	}

	CTOR ColorF(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a)
	{
	}

	operator float* ()
	{
		return &r;
	}

	operator const float* () const
	{
		return &r;
	}

	float& operator [] (uint index)
	{
		if (index >= 4) raise(ArgumentOutOfRangeException());
		return (&r)[index];
	}

	float operator [] (uint index) const
	{
		if (index >= 4) raise(ArgumentOutOfRangeException());
		return (&r)[index];
	}

	static ColorF FromCOLORREF(COLORREF clr)
	{
		return ColorF(GetRValue(clr)/255.0f, GetGValue(clr)/255.0f, GetBValue(clr)/255.0f);
	}

	operator gm::vector4f() const
	{
		return gm::vector4f(r, g, b, a);
	}

	static ColorF Transparent;
	static ColorF Black;
	static ColorF White;
	static ColorF Red;
	static ColorF Green;
	static ColorF Blue;
	static ColorF Yellow;

	float r;
	float g;
	float b;
	float a;
};

enum PopupAnimation
{
	PopupAnimation_None,
	PopupAnimation_Slide,
	PopupAnimation_Fade,
	PopupAnimation_Scroll,
	PopupAnimation_Center,
};

interface IEffect
{
	virtual unsigned int GetInputCount() = 0;
	virtual IEffect* GetInput(unsigned int index) = 0;
	virtual void SetInput(unsigned int index, IEffect* input) = 0;
	virtual void RenderOutput(RenderContext* renderContext) = 0;
//	virtual void RenderOutputNormalMap(RenderTarget* rt) = 0;
	virtual void Render(RenderContext* renderContext, gm::RectF destRect, const gm::matrix4f& modelView, const gm::matrix4f& projection) = 0;
	virtual Graphics::Bitmap* GetOutputBitmap() = 0;
	virtual Graphics::Bitmap* GetOutputNormalMap() = 0;
};

class GUIEXT CEffect : public Object, public IEffect
{
protected:

	CTOR CEffect();	// protected, this class can't be created directly, must derive from it

public:

	virtual ~CEffect();

	/*
	virtual unsigned int GetInputCount() = 0;
	virtual CEffect* GetInput(unsigned int index) = 0;
	virtual void Render(LDraw::GraphicsO* pGraphics, LDraw::RectF destRect, const LDraw::matrix4f& modelView, const LDraw::matrix4f& projection) = 0;
	*/

	virtual void RenderOutput(Gui::RenderContext* rt);
#if 0
	virtual void RenderOutputNormalMap(RenderTarget* rt);
#endif

	virtual Graphics::Bitmap* GetOutputBitmap()
	{
		return m_outImage;
	}

	virtual Graphics::Bitmap* GetOutputNormalMap()
	{
		return m_outNormalMap;
	}

protected:

	void DrawBitmap(RenderContext* renderContext, gm::RectF destRect);

public:
	gm::RectF m_subRegion;
	Graphics::Bitmap* m_outImage;
	Graphics::Bitmap* m_outNormalMap;

	int m_scaledWidth;
	int m_scaledHeight;
};

class GUIEXT AlphaChannelEffect : public CEffect
{
public:
};

class GUIEXT VisualEffect : public CEffect
{
public:
	CTOR VisualEffect()
	{
	}

	CTOR VisualEffect(Visual* visual) : m_visual(visual)
	{
	}

	virtual void RenderOutput(RenderTarget* rt);
	virtual void Render(RenderTarget* rt, gm::RectF destRect, const gm::matrix4f& modelView, const gm::matrix4f& projection)
	{
		ASSERT(0);
	}

	virtual unsigned int GetInputCount()
	{
		return 0;
	}

	virtual IEffect* GetInput(unsigned int index)
	{
		return NULL;
	}

	virtual void SetInput(unsigned int index, IEffect* input)
	{
		raise(Exception("Doesn't take any inputs"));
	}

	Visual* m_visual;
};

}
}

#include "GaussianBlurEffect.h"

namespace System
{

namespace Gui
{

typedef gm::vector2f Vec2f;
typedef gm::vector3f Vec3f;
typedef gm::vector4f Vec4f;

class GUIEXT Effect : public Object
{
public:

	CTOR Effect(IEffect* root) : m_root(root)
	{
	}

	IEffect* m_root;
};

#if 0
class Property
{
public:
};

template<class TYPE> class PropertyT : public Property
{
public:

	TYPE GetValue();
};

class GUIEXT PropertyMetadata : public Object
{
public:
};
#endif

class GUIEXT PropertyMetadata : public Object
{
public:

	CTOR PropertyMetadata() :
		m_notifyOnValueChange(false)
	{
	}

	CTOR PropertyMetadata(bool notifyOnValueChange) :
		m_notifyOnValueChange(notifyOnValueChange)
	{
	}

	bool m_notifyOnValueChange;
};

}
}

#include "DependencyClass.h"
#include "DependencyProperty.h"

namespace System
{
namespace Gui
{

/*
class RoutedEvent : public Object
{
public:
	CTOR RoutedEvent(Type_Info& handlerType, Type_Info& ownerType) : m_handlerType(handlerType), m_ownerType(ownerType)
	{
	}

	const Type_Info& m_handlerType;
	const Type_Info& m_ownerType;

	int m_index;
};
*/

/*
class GUIEXT RoutedEventManager
{
public:

	static RoutedEvent* RegisterRoutedEvent(StringW* name, const Type_Info& handlerType, const Type_Info& ownerType);

	class PerClass
	{
	};

private:
	CTOR RoutedEventManager();
};
*/

/*
class GUIEXT InputEventArgs : public RoutedEventArgs
{
public:
	Window* m_window;
	double m_time;
};

class GUIEXT MouseEventArgs : public InputEventArgs
{
public:
	LDraw::PointF m_screenPos;
	LDraw::PointF m_windowPos;
	//LDraw::PointF m_localPos;

	LDraw::PointF GetLocalPoint(Visual* visual);
};
*/

/*
class ObjectProperty
{
public:
	Object* m_object;
	Property* m_property;
};
*/

interface IBinding;

interface IInputBinding
{
	virtual int SetInput(IBinding* caller, const Variant& value) = 0;
//	virtual Type* GetInputType() = 0;
};

interface IBinding : public IInputBinding
{
	virtual Variant GetOutput(IBinding* caller) = 0;
	virtual int SetOutput(IBinding* caller, const Variant& value) = 0;

	virtual int SetOutput(IBinding* caller, bool value)
	{
		return SetOutput(caller, Variant(value));
	}

	virtual int SetOutput(IBinding* caller, int value)
	{
		return SetOutput(caller, Variant(value));
	}

	virtual int SetOutput(IBinding* caller, float value)
	{
		return SetOutput(caller, Variant(value));
	}

	virtual int SetOutput(IBinding* caller, double value)
	{
		return SetOutput(caller, Variant(value));
	}

	virtual Type* GetOutputType() = 0;
//	virtual Type* GetInputType() = 0;

	virtual void* Connect(IInputBinding* binding) = 0;
};

class GUIEXT PropertyValueConstraint : public Object
{
public:
	virtual Object* Constrain(Object* val) = 0;
};

template<class T> class PropertyValueMinConstraint : public PropertyValueConstraint
{
public:

	CTOR PropertyValueMinConstraint(T minVal) : m_minVal(minVal)
	{
	}

	virtual Object* Constrain(Object* val)
	{
		return NULL;
	}

	T Constrain(T val)
	{
		if (val < m_minVal)
		{
			return m_minVal;
		}
		return val;
	}

	T m_minVal;
};

template<class type_t>
class typed
{
public:

/*
	CTOR typed(bool const_value) : m_const_value(type_t(const_value)), m_exp(NULL), m_binding(NULL)
	{
	}

	CTOR typed(unsigned char const_value) : m_const_value(type_t(const_value)), m_exp(NULL), m_binding(NULL)
	{
	}

	CTOR typed(int const_value) : m_const_value(type_t(const_value)), m_exp(NULL), m_binding(NULL)
	{
	}

	CTOR typed(long const_value) : m_const_value(type_t(const_value)), m_exp(NULL), m_binding(NULL)
	{
	}

	CTOR typed(float const_value) : m_const_value(type_t(const_value)), m_exp(NULL), m_binding(NULL)
	{
	}


	CTOR typed(double const_value) : m_const_value(const_value), m_exp(NULL), m_binding(NULL)
	{
	}
*/

	/*
	CTOR typed(Expressive::Expression* exp) : m_exp(exp), m_binding(NULL)
	{
	}
	*/

#ifndef __LERSTAD__
	template<class T>
	CTOR typed(const Expressive::typed_expression<T>& exp) : m_exp(Expressive::cast<type_t>(exp)), m_binding(NULL)
	{
	}
/*
	CTOR typed(const Expressive::typed_expression<int>& exp) : m_exp(Expressive::cast<type_t>(exp)), m_binding(NULL)
	{
	}

	CTOR typed(const Expressive::typed_expression<float>& exp) : m_exp(Expressive::cast<type_t>(exp)), m_binding(NULL)
	{
	}

	CTOR typed(const Expressive::typed_expression<double>& exp) : m_exp(Expressive::cast<type_t>(exp)), m_binding(NULL)
	{
	}

	CTOR typed(const Expressive::typed_expression<double>& exp) : m_exp(Expressive::cast<type_t>(exp)), m_binding(NULL)
	{
	}
	*/
#endif

	template<class T>
	CTOR typed(T const_value) : m_const_value(type_t(const_value)), m_exp(NULL), m_binding(NULL)
	{
	}
/*
	CTOR typed(type_t const_value) : m_const_value(type_t(const_value)), m_exp(NULL), m_binding(NULL)
	{
	}
*/
	/*
	CTOR typed(int const_value) : m_const_value(type_t(const_value)), m_exp(NULL), m_binding(NULL)
	{
	}

	CTOR typed(float const_value) : m_const_value(type_t(const_value)), m_exp(NULL), m_binding(NULL)
	{
	}

	CTOR typed(Object* const_value) : m_const_value(unbox_cast<type_t>(const_value)), m_exp(NULL), m_binding(NULL)
	{
	}
	*/

	CTOR typed(IBinding* binding) : m_exp(nullptr), m_binding(binding)
	{
	}

	CTOR typed(PropertyValue* binding) : m_exp(nullptr), m_binding(binding)
	{
	}

	bool hasBinding() const
	{
		if (m_exp != nullptr || m_binding != nullptr)
			return true;
		else
			return false;
	}

	IBinding* binding(DependencyObject* _this) const;


	/*
	operator Object* () const
	{
		return box_cast<type_t>(m_const_value);
	}
	*/

	operator type_t () const
	{
		// TODO, evaluate exp if != NULL
		ASSERT(m_exp == nullptr);
		ASSERT(m_binding == nullptr);

		return m_const_value;
	}

	Expressive::Expression* m_exp;
	IBinding* m_binding;
	type_t m_const_value;
};

class GUIEXT BindingExpression : public Expressive::Expression, public IInputBinding
{
public:
	CTOR BindingExpression(IBinding* binding);

	virtual int SetInput(IBinding* caller, const Variant& value) override;	// IInputBinding
//	virtual int SetInput(IBinding* caller, int value) override;	// IInputBinding

	virtual bool Set(Expressive::EvalContext* context, const Variant& value) override
	{
		m_binding->SetOutput(nullptr, value);
		return true;
	}

	virtual bool Set(Expressive::EvalContext* context, bool value) override
	{
		m_binding->SetOutput(nullptr, value);
		return true;
	}

	virtual bool Set(Expressive::EvalContext* context, int value) override
	{
		m_binding->SetOutput(nullptr, value);
		return true;
	}

	virtual bool Set(Expressive::EvalContext* context, float value) override
	{
		m_binding->SetOutput(nullptr, value);
		return true;
	}

	virtual Variant Evaluate(Expressive::EvalContext* context) override;
	virtual void TraceCallback(Expressive::EvalContext* context, Expressive::ExpTrace& trace, Expressive::ITraceCallback* callback) override;

	IBinding* m_binding;
	void* m_ptr;

	ExpressionBinding* m_ownerBinding;
	bool m_setting;
};

}	// Gui
}	// System

#include "PropertyValue.h"

namespace System
{
namespace Gui
{

#if 0
// 1 input, many outputs
class GUIEXT PropertyValueBase : public IInOutBinding
{
public:
};

class GUIEXT PropertyValue : public PropertyValueBase
{
public:

	CTOR PropertyValue();

	StringW* get_Name()
	{
		return GetDependencyProperty()->get_Name();
	}

	virtual const Type_Info& GetPropertyType() const = 0;
	virtual DependencyProperty* GetDependencyProperty() = 0;
	virtual DependencyObject* GetContainer() = 0;

	virtual void SetLocalObjectValue(Object* value) = 0;
	virtual Object* GetLocalObjectValue() = 0;
	virtual Object* GetComputedObjectValue() = 0;

	virtual Object* GetUpStream(IInOutBinding* caller);	// Called on upstream
	virtual int SetUpStream(IInOutBinding* caller, Object* value);	// Called on upstream
	virtual int IncomingObjectValue(IOutBinding* caller, Object* value);	// Called on downstream
	virtual void* Connect(IInOutBinding* binding);

	void UpdateObjectValue(Object* value);

	static Object UnsetValue;

};

class GUIEXT PropertyValue2 : public PropertyValue
{
public:

	virtual void SetLocalObjectValue(Object* value);
	virtual Object* GetLocalObjectValue();
	virtual Object* GetComputedObjectValue();

	void SetIn(IOutBinding* inBinding);

protected:

	IOutBinding* m_inBinding;
	void* m_inBindingPtr;
	list<IInOutBinding*> m_outBindings;

	int SendComputedValueDownStreamExcept(IInOutBinding* exceptThis);

	inline int SendComputedValueDownStream()
	{
		return SendComputedValueDownStreamExcept(NULL);
	}

private:

	Object* m_localValue;
	Object* m_computedValue;
};

class GUIEXT PropertyValueExp : public PropertyValue
{
public:

	virtual void SetLocalObjectValue(Object* value);
	virtual Object* GetLocalObjectValue();
	virtual Object* GetComputedObjectValue();
};

template<class PropertyType> class PropertyValueTBase : public PropertyValue2
{
public:

	CTOR PropertyValueTBase()
	{
	}

	typedef PropertyType propertyType_t;

	static const Type_Info& propertyType()
	{
		return typeid(propertyType_t);
	}

	virtual const Type_Info& GetPropertyType() const
	{
		return typeid(propertyType_t);
	}

	PropertyType GetValue()
	{
		return unbox_cast<PropertyType>(GetComputedObjectValue());
	}

	void SetValue(PropertyType value)
	{
		SetLocalObjectValue(box_cast(value));
	}

	void UpdateValue(PropertyType value)
	{
		UpdateObjectValue(box_cast(value));
	}
};

template<class ContainerType, class PropertyType, unsigned int index = 0> class PropertyValueT : public PropertyValueTBase<PropertyType>
{
public:
	CTOR PropertyValueT()
	{
	}

#ifndef __LERSTAD__
	friend ContainerType;
#endif

	typedef ContainerType t_classType;

	static const Type_Info& GetClassType()
	{
		return typeid(t_classType);
	}

	virtual DependencyProperty* GetDependencyProperty()
	{
		return static_cast<ContainerType*>(this)->s_Class.GetLocalProperty(index);
	}

	virtual DependencyObject* GetContainer()
	{
		return static_cast<ContainerType*>(this);
	}
};
#endif

/*
class GUIEXT TemplatedOwnerExpression : public Expressive::ObjectExpression
{
public:

	static TemplatedOwnerExpression only;

	virtual Object* Evaluate(Expressive::EvalContext* context);
	virtual void TraceCallback(Expressive::EvalContext* context, Expressive::ExpTrace& trace, Expressive::ITraceCallback* callback);

protected:

	CTOR TemplatedOwnerExpression()
	{
	}
};
*/

class GUIEXT GetDPExpression : public Expressive::GetPropertyExpression
{
protected:
	CTOR GetDPExpression(Expressive::Expression* object) : Expressive::GetPropertyExpression(object)
	{
	}
};

class ExpressionBinding;

class GUIEXT GetDPByDPExpression : public GetDPExpression, public IInputBinding
{
public:
	CTOR GetDPByDPExpression(Expressive::Expression* object, DependencyProperty* dp);

	virtual void OnChildChanged(Expressive::Expression* child) override;

	virtual Variant Evaluate(Expressive::EvalContext* context);
	virtual bool Set(Expressive::EvalContext* context, const Variant& value);

	virtual void TraceCallback(Expressive::EvalContext* context, Expressive::ExpTrace& trace, Expressive::ITraceCallback* callback);

	virtual int SetInput(IBinding* caller, const Variant& value) override;	// IInputBinding
	virtual Type* GetInputType();// override;	// IInputBinding

	virtual IO::TextWriter& WriteMLString(IO::TextWriter& stream, map<Object*,int>& defs) override
	{
#ifndef __LERSTAD__
		m_object->WriteMLString(stream, defs);
		stream << ".";
		stream << m_dp->get_Name();
		return stream;
#endif
	}

	DependencyProperty* GetDP() const
	{
		return m_dp;
	}

	IBinding* m_inBinding;
	ExpressionBinding* m_ownerBinding;

protected:

	DependencyProperty* m_dp;
	bool m_setting;
};

class GUIEXT GetDPByExpExpression : public GetDPExpression
{
public:
	CTOR GetDPByExpExpression(Expressive::Expression* object, Expressive::Expression* exp) :
		GetDPExpression(object), m_exp(exp)
	{
	}

	virtual Variant Evaluate(Expressive::EvalContext* context);
	bool Set(Expressive::EvalContext* context, Variant value);

	virtual void TraceCallback(Expressive::EvalContext* context, Expressive::ExpTrace& trace, Expressive::ITraceCallback* callback);

	Expressive::Expression* m_exp;
};

// many inputs, 1 output
class GUIEXT ExpressionBinding : public Object, public IBinding
{
public:

	/*
	CTOR ExpressionBinding(DependencyObject* templatedOwner, Expressive::Expression* exp) : m_templateOwner(templateOwner), m_exp(exp)
	{
		ASSERT(dynamic_cast<Expressive::Expression*>((Object*)exp) != NULL);
	}
	*/

	CTOR ExpressionBinding(DependencyObject* _this, Expressive::Expression* exp);

	virtual Type* GetOutputType() override;
	virtual Variant GetOutput(IBinding* caller) override;

	virtual int SetOutput(IBinding* caller, const Variant& value) override;
	virtual int SetOutput(IBinding* caller, float value) override;

	virtual int SetInput(IBinding* caller, const Variant& value) override;
	virtual void* Connect(IInputBinding* binding) override;

	void ReEval();

	DependencyObject* m_this;	// ???
	Expressive::Expression* m_exp;

	list<IBinding*> m_inBindings;
	IInputBinding* m_outBinding;
};

// many inputs, 1 output
class GUIEXT ExpressionBinding2 : public Object, public IBinding
{
public:
	// vm expression
};

// 1 input, many outputs (e.g: connect an expression to many outputs)
class GUIEXT OneToManyOutputsBinding : public Object, public IBinding
{
public:
	CTOR OneToManyOutputsBinding();
	CTOR OneToManyOutputsBinding(IBinding* inBinding);

	virtual Variant GetOutput(IBinding* caller) override;
	virtual int SetOutput(IBinding* caller, const Variant& value) override;
	virtual int SetInput(IBinding* caller, const Variant& value) override;
	virtual void* Connect(IInputBinding* binding) override;

	void SetInputBinding(IBinding* inBinding);

protected:

	virtual Variant GetValue() = 0;
	virtual void SetValue(const Variant& value) = 0;

	int OutputValue(const Variant& value);

	IBinding* m_inBinding;
	void* m_inBindingPtr;
	list<IInputBinding*> m_outBindings;
};

#if 0
// TODO remove
class GUIEXT PropertyTemplate : public Object
{
public:

	CTOR PropertyTemplate(DependencyProperty* dp, Object* value = NULL) :
		m_dp(dp),
		m_value(value)
	{
	}

	DependencyProperty* m_dp;
	Object* m_value;
};

// TODO remove
class GUIEXT SetProperty : public Object
{
public:
	CTOR SetProperty(DependencyProperty* dp) : m_dp(dp)
	{
	}

	DependencyProperty* m_dp;
};

// TODO remove
class GUIEXT SetPropertyToConst : public SetProperty
{
public:
	CTOR SetPropertyToConst(DependencyProperty* dp, Object* value) : SetProperty(dp), m_value(value)
	{
	}

	/*
	Binding* CreateBinding()
	{
		return new PropertyValueBinding(m_dp, m_value);
	}
*/

	DependencyProperty* m_dp;
	Object* m_value;
};

// TODO remove
class GUIEXT SetPropertyToExpression : public SetProperty
{
public:
	CTOR SetPropertyToExpression(DependencyProperty* dp, Expressive::Expression* exp) : SetProperty(dp), m_exp(exp)
	{
		ASSERT(dynamic_cast<Expressive::Expression*>((Object*)exp) != NULL);
	}

	/*
	Binding* CreateBinding()
	{
		return new ExpressionBinding(m_dp, m_exp);
	}
	*/

	Expressive::Expression* m_exp;
};

// TODO remove
class ObjectTemplateBase;

// TODO remove
class GUIEXT SetPropertyToObjectTemplate : public SetProperty
{
public:

	CTOR SetPropertyToObjectTemplate(DependencyProperty* dp, ObjectTemplateBase* objectTemplate) :
		SetProperty(dp),
		m_objectTemplate(objectTemplate)
	{
	}

	ObjectTemplateBase* m_objectTemplate;
};

/*
class GUIEXT SetPropertyToObjectCollectionTemplate : public SetProperty
{
public:

	CTOR SetPropertyToObjectCollectionTemplate(DependencyProperty* dp, ObjectCollectionTemplate* objectCollectionTemplate) :
		SetProperty(dp),
		m_objectCollectionTemplate(objectCollectionTemplate)
	{
	}

	ObjectCollectionTemplate* m_objectCollectionTemplate;
};
*/

// TODO remove
class GUIEXT SetPropertyGroup : public Object
{
public:
	vector<SetProperty*> m_items;
};

// TODO remove
class Deps
{
public:
	ObjectTemplateBase* m_objTemplate;
	Expressive::StaticObjectExpression* m_exp;
};

// TODO remove
class GUIEXT Refs2
{
public:
//	map<int, Deps*> m_objects;
	map<int, ObjectTemplateBase*> m_objects;
	map<ObjectTemplateBase*, int> m_objects2;
};

// TODO remove
class GUIEXT ObjectTemplateBase : public Object
{
public:
	CTOR ObjectTemplateBase(const Type_Info& classType) : m_classType(classType)
	{
		ASSERT(&classType);
		ASSERT(classType.GetType());
	}

	virtual Object* Create(Refs2* refs, map<int, Expressive::StaticObjectExpression*>& irefs, Window* ownerWindow, UIElement* templatedOwner, IRootVisual* rootVisual) = 0;

	const Type_Info& m_classType;
};

// TODO remove
class GUIEXT ObjectTemplateCollection : public Object
{
public:
	list<ObjectTemplateBase*> m_items;
};

// TODO remove
class GUIEXT ObjectTemplate : public ObjectTemplateBase
{
public:

	CTOR ObjectTemplate(const Type_Info& classType, SetPropertyGroup* setPropertyGroup = NULL) :
		ObjectTemplateBase(classType),
		m_setPropertyGroup(setPropertyGroup)
	{
	}

//	virtual Object* Evaluate(EvalContext* context);	// TODO, only if Expression

	virtual Object* Create(Refs2* refs, map<int, Expressive::StaticObjectExpression*>& irefs, Window* ownerWindow, UIElement* templatedOwner, IRootVisual* rootVisual);

	SetPropertyGroup* m_setPropertyGroup;
};

// TODO remove
class GUIEXT CollectionObjectTemplate : public ObjectTemplateBase
{
public:

	CTOR CollectionObjectTemplate(const Type_Info& classType, ObjectTemplateCollection* items) :
		ObjectTemplateBase(classType),
		m_items(items)
	{
	}

	virtual Object* Create(Refs2* refs, map<int, Expressive::StaticObjectExpression*>& irefs, Window* ownerWindow, UIElement* templatedOwner, IRootVisual* rootVisual);

	ObjectTemplateCollection* m_items;
};
#endif

// TODO remove
class GUIEXT Template : public Object
{
public:
//	Refs2* m_refs;
	Object* m_object;
};

#if 0
class tProperty;
class tSetProperty;
class tSetPropertyGroup;
class _ObjectCollection;
class tCollectionObjectTemplate;
template<class> class tObjectTemplate;

class GUIEXT tSetProperty
{
public:

	CTOR tSetProperty(SetProperty* p) : m_p(p)
	{
	}

	operator SetProperty* () const
	{
		return m_p;
	}

#ifndef __LERSTAD__
	tSetPropertyGroup operator, (tSetProperty prop);
#endif

	SetProperty* m_p;
};

class GUIEXT tProperty
{
public:

	/*
	CTOR tProperty(DependencyProperty& dp) : m_dp(&dp)
	{
	}
	*/

	template<class T>
	CTOR tProperty(TypedDependencyProperty<T>& dp) : m_dp(&dp)
	{
	}

	/*
	operator Expressive::expression () const
	{
		return new WithGetPropertyExpression(m_dp);
	}
	*/

	tSetProperty operator = (const Expressive::expression& exp) const
	{
		return new SetPropertyToExpression(m_dp, exp.m_p);
	}

	/*
	tSetProperty operator = (Expressive::Expression* exp) const
	{
		return new SetPropertyToExpression(m_dp, exp);
	}
	*/

	tSetProperty operator = (Object* obj) const
	{
		return new SetPropertyToConst(m_dp, obj);
	}

	template<class T> tSetProperty operator = (tObjectTemplate<T> obj) const;

	tSetProperty operator = (const tCollectionObjectTemplate& obj) const;

	tSetProperty operator = (int value) const
	{
		if (m_dp->get_Type() == typeid(bool))
			return new SetPropertyToConst(m_dp, BoolObject::GetObject(value != 0));
		else if (m_dp->get_Type() == typeid(int))
			return new SetPropertyToConst(m_dp, IntObject::GetObject((int)value));
		else if (m_dp->get_Type() == typeid(float))
			return new SetPropertyToConst(m_dp, FloatObject::GetObject((float)value));
		else if (m_dp->get_Type() == typeid(double))
			return new SetPropertyToConst(m_dp, DoubleObject::GetObject((double)value));
		else
		{
			ASSERT(0);
			return NULL;
			//return new SetPropertyToConst(m_dp, box_cast(value));
		}
	}

	tSetProperty operator = (float value) const
	{
		if (m_dp->get_Type() == typeid(bool))
			return new SetPropertyToConst(m_dp, BoolObject::GetObject(value != 0));
		else if (m_dp->get_Type() == typeid(int))
			return new SetPropertyToConst(m_dp, IntObject::GetObject((int)value));
		else if (m_dp->get_Type() == typeid(float))
			return new SetPropertyToConst(m_dp, FloatObject::GetObject((float)value));
		else if (m_dp->get_Type() == typeid(double))
			return new SetPropertyToConst(m_dp, DoubleObject::GetObject((double)value));
		else
		{
			ASSERT(0);
			return NULL;
			//return new SetPropertyToConst(m_dp, box_cast(value));
		}
	}

	tSetProperty operator = (double value) const
	{
		if (m_dp->get_Type() == typeid(bool))
			return new SetPropertyToConst(m_dp, BoolObject::GetObject(value != 0));
		else if (m_dp->get_Type() == typeid(int))
			return new SetPropertyToConst(m_dp, IntObject::GetObject((int)value));
		else if (m_dp->get_Type() == typeid(float))
			return new SetPropertyToConst(m_dp, FloatObject::GetObject((float)value));
		else if (m_dp->get_Type() == typeid(double))
			return new SetPropertyToConst(m_dp, DoubleObject::GetObject((double)value));
		else
		{
			ASSERT(0);
			return NULL;
			//return new SetPropertyToConst(m_dp, box_cast(value));
		}
	}

	DependencyProperty* m_dp;
};

class GUIEXT tSetPropertyGroup
{
public:

	CTOR tSetPropertyGroup() : m_p(NULL)
	{
	}

	CTOR tSetPropertyGroup(SetPropertyGroup* p) : m_p(p)
	{
	}

	CTOR tSetPropertyGroup(const tSetProperty& prop)
	{
		m_p = new SetPropertyGroup;
		m_p->m_items.push_back(prop.m_p);
	}

#ifndef __LERSTAD__
	tSetPropertyGroup& operator, (const tSetProperty& prop)
	{
		m_p->m_items.push_back(prop.m_p);
		return *this;
	}
#endif

	SetPropertyGroup* m_p;
};

class GUIEXT Refs
{
public:
	CTOR Refs();

	pair<Refs2*, pair<int, ObjectTemplateBase*> > def(int id);

#if 0
	Expressive::_Object use(int id);

	template<class castTo> castTo useAs(int id)
	{

		Deps* obj = NULL;
		pair<map<int, ObjectTemplateBase*>::iterator,bool> r = m_p->m_objects.insert(map<int, ObjectTemplateBase*>::value_type(id, obj));

		/*
		if (r.first->second == NULL)
		{
			r.first->second = new Deps;
			r.first->second->m_exp = new Expressive::StaticObjectExpression;
		}
		*/

		return castTo(r.first->second->m_exp);
	}
#endif

	Refs2* m_p;
};

template<class T> class tObjectTemplate
{
public:
	CTOR tObjectTemplate()
	{
		m_p = new ObjectTemplate(typeid(T));
	}

	CTOR tObjectTemplate(pair<Refs2*,pair<int, ObjectTemplateBase*> >& defid, tSetPropertyGroup& properties)
	{
		m_p = new ObjectTemplate(typeid(T), properties.m_p);
		defid.second.second = m_p;
#ifndef __LERSTAD__
	//	defidd.second->m_exp->m_pType = typeof(T*);
#endif
	//	ASSERT(defid.second.second->m_exp->m_pType);
		defid.first->m_objects2[m_p] = defid.second.first;
	}

	CTOR tObjectTemplate(const tSetPropertyGroup& properties)
	{
		m_p = new ObjectTemplate(typeid(T), properties.m_p);
	}

	CTOR tObjectTemplate(const tSetProperty& prop)
	{
		SetPropertyGroup* propertyGroup = new SetPropertyGroup();
		propertyGroup->m_items.push_back(prop.m_p);
		m_p = new ObjectTemplate(typeid(T), propertyGroup);
	}

#ifndef __LERSTAD__
	template<class T2> _ObjectCollection operator,(const tObjectTemplate<T2>& other) const;
#endif

	operator ObjectTemplate* ()
	{
		return m_p;
	}

	ObjectTemplate* m_p;
};

class GUIEXT _ObjectCollection
{
public:

	CTOR _ObjectCollection(ObjectTemplateCollection* p = NULL) : m_p(p)
	{
	}

	template<class T2>
	CTOR _ObjectCollection(const tObjectTemplate<T2>& other)
	{
		m_p = new ObjectTemplateCollection;
		m_p->m_items.push_back(other.m_p);
	}

#ifndef __LERSTAD__
	template<class T2>
	_ObjectCollection& operator,(const tObjectTemplate<T2>& other) const
	{
		m_p->m_items.push_back(other.m_p);
		return *this;
	}
#endif

	operator ObjectTemplateCollection* () const
	{
		return m_p;
	}

	ObjectTemplateCollection* m_p;
};

class GUIEXT tCollectionObjectTemplate
{
public:
	CTOR tCollectionObjectTemplate(const _ObjectCollection& item)
	{
		m_p = new CollectionObjectTemplate(typeid(VisualCollection), item.m_p);
	}

	CollectionObjectTemplate* m_p;
};

/*
class GUIEXT _this : public object
{
public:

	CTOR _this() : object(new ThisExpression())
	{
	}
};
*/

//extern object _this;	// TODO remove

/*
class GUIEXT templateOwner : public Expressive::_Object
{
public:

	CTOR templateOwner() : Expressive::_Object(&TemplateOwnerExpression::only)
	{
	}
};
*/

/*
class idref : public object
{
public:

	CTOR idref(int id) : object(IDRefExpression::GetIDREF(id))
	{
	}
};
*/

class GUIEXT tTemplate
{
public:

	CTOR tTemplate(Template* p)
	{
		m_p = p;
	}

	CTOR tTemplate(Refs& refs, ObjectTemplate* obj)
	{
		m_p = new Template;
		m_p->m_refs = refs.m_p;
		m_p->m_object = obj;
	}

	operator Template* ()
	{
		return m_p;
	}

	Template* m_p;
};

class GUIEXT xmlDocument
{
public:
	CTOR xmlDocument(const WCHAR* str);
	CTOR xmlDocument(Expressive::expression& exp);

	XmlData::Document* m_p;
};

/*
class GUIEXT xmlFragment
{
public:
	CTOR xmlFragment(const WCHAR* str);
	CTOR xmlFragment(expression& exp);

	XmlData::Fragment* m_p;
};
*/

template<class T> tSetProperty tProperty::operator = (tObjectTemplate<T> obj) const
{
	return new SetPropertyToObjectTemplate(m_dp, obj.m_p);
}

inline tSetProperty tProperty::operator = (const tCollectionObjectTemplate& obj) const
{
	return new SetPropertyToObjectTemplate(m_dp, obj.m_p);
}

#ifndef __LERSTAD__
template<class T> template<class T2>
_ObjectCollection tObjectTemplate<T>::operator,(const tObjectTemplate<T2>& other) const
{
	ObjectTemplateCollection* p = new ObjectTemplateCollection;
	p->m_items.push_back(m_p);
	p->m_items.push_back(other.m_p);
	return _ObjectCollection(p);
}
#endif

#endif


/*
inline expression operator + (expression& exp, int value)
{
	return new PlusBinaryExpression(exp.m_p, new ConstExpression<int>(value));
}

inline expression operator + (expression& exp, float value)
{
	return new PlusBinaryExpression(exp.m_p, new ConstExpression<float>(value));
}

inline expression operator + (expression& exp, double value)
{
	return new PlusBinaryExpression(exp.m_p, new ConstExpression<double>(value));
}
*/

/*
inline expression operator + (tProperty& left, tProperty& right)
{
	return new PlusBinaryExpression(prop.m_p, new ConstExpression<double>(value));
}
*/

/*
DependencyProperty::operator expression () const
{
	return new WithGetPropertyExpression(this);
}

template<class T> expression DependencyProperty::operator + (T right) const
{
	return new PlusBinaryExpression(new WithGetPropertyExpression(this), right);
}
*/

/*
inline function::function(const expression& exp) : m_p(new ExpressionFunctionT1<float, DependencyObject*>(exp.m_p))
{
}
*/

}
}

#include "DependencyObject.h"
#include "GeneralTransform.h"
#include "Animation.h"

namespace System
{
namespace Gui
{

class GUIEXT Link : public DependencyObject
{
public:

	String m_name;
};

class GUIEXT LinkResolver
{
public:

	virtual void Resolve(Link* link) abstract;
};

class PSDLayerLink : public LinkResolver
{
public:

	void Resolve(Link* link)
	{
	}

	const Type_Info& GetTypeInfo()
	{
		return typeid(Graphics::Bitmap);
	}

	Object* GetData();
};

class GUIEXT BitmapSource : public DependencyObject, public IBitmapSource
{
public:

	CTOR BitmapSource();
	CTOR BitmapSource(Graphics::Bitmap* bitmap);

	virtual Graphics::Bitmap* GetBitmap() override
	{
		return m_bitmap;
	}

	DEPENDENCY_CLASS(BitmapSource, DependencyObject)

	DEPENDENCY_PROPERTY_RO(int, Width)
	DEPENDENCY_PROPERTY_RO(int, Height)

	Graphics::Bitmap* m_bitmap;
};

class GUIEXT TextFormat : public Object
{
public:

	CTOR TextFormat(StringIn fontFamily, float fontSize, int fontWeight = 400);

	String m_fontFamily;
	float m_fontSize;
	int m_fontWeight;

	IDWriteTextFormat* m_pDWriteTextFormat;
};

}	// Gui
}	// System

#include "RenderContext.h"
#include "ImmediateRenderContext.h"
#include "RetainedRenderContext.h"
#include "ManagedRenderContext.h"
#include "ManagedRetainedRenderContext.h"

namespace System
{
namespace Gui
{

class GUIEXT FloatRadians
{
public:

	CTOR FloatRadians(float value) : m_value(value)
	{
	}

	operator float () const
	{
		return m_value;
	}

	float m_value;
};

}
}

#include "Transform.h"
#include "TranslateTransform.h"
#include "RotateTransform.h"
#include "ScaleTransform.h"
#include "MatrixTransform.h"
#include "Color.h"

namespace System
{
namespace Gui
{

/*
class GUIEXT SkewXTransform : public Transform
{
public:
};

class GUIEXT SkewYTransform : public Transform
{
public:
};
*/

/*
class GUIEXT PropertyBinding : public Binding
{
public:

	CTOR PropertyBinding(PropertyValue* depProperty);

	virtual Object* get(PropertyValue* target)
	{
		return m_upStreamProperty->GetComputedObjectValue();
	}

	virtual void set(Object* value)
	{
		ASSERT(0);
		//m_pProperty->SetObjectValue(value);
	}

	virtual void setUpstream(Object* value);

	PropertyValue* m_upStreamProperty;
};
*/

#if 0
template<class TYPE> class ComputedProperty
{
public:

#ifndef __LERSTAD__
	CTOR ComputedProperty(const DependencyObject* object, const DependencyProperty* dp) :
		m_object(object), m_dp(dp)
	{
	}
#endif

	operator TYPE () const
	{
#ifndef __LERSTAD__
		TYPE result = dynamic_cast<IFunction2T<TYPE, DependencyObject*>(m_dp->m_function)->call(m_object);
		return result;
#endif
	}

	const DependencyObject* m_object;
	const DependencyProperty* m_dp;
};
#endif

class GUIEXT ColorRGBA : public Color
{
public:

//	DEPENDENCY_PROPERTY(float, Red)
//	DEPENDENCY_PROPERTY(float, Green)
//	DEPENDENCY_PROPERTY(float, Blue)
	DEPENDENCY_PROPERTY(float, Alpha)
/*
	// Computed
	TypedDependencyProperty<float> HueProperty;
	TypedDependencyProperty<float> LuminanceProperty;
	TypedDependencyProperty<float> SaturationProperty;
	*/
};

}
}

#include "GradientStopCollection.h"

#include "Brush.h"
#include "SolidColorBrush.h"
#include "GradientBrush.h"
#include "LinearGradientBrush.h"
#include "RadialGradientBrush.h"

namespace System
{
namespace Gui
{

class GUIEXT VisualBrush : public Brush
{
public:
};

class GUIEXT Pen : public Object
{
public:
	CTOR Pen(Brush* brush, float width = 1.0f);

	Brush* m_brush;
	float m_width;
};

}
}

/*
class GUIEXT SubPath : public DependencyObject
{
public:
};
*/

#include "PathSegment.h"
#include "PathFigure.h"
#include "LineToPathSegment.h"
#include "CubicBezierToPathSegment.h"

namespace System
{
namespace Gui
{

class GUIEXT PathData : public Object
{
public:
	vector<PathSegment*> m_segments;
	ID2D1PathGeometry* m_pGeometry;
	bool m_closed;
};

#ifdef __LERSTAD__

enum D2D1_COMBINE_MODE
{
	D2D1_COMBINE_MODE_UNION       = 0,
	D2D1_COMBINE_MODE_INTERSECT   = 1,
	D2D1_COMBINE_MODE_XOR         = 2,
	D2D1_COMBINE_MODE_EXCLUDE     = 3,
};

enum D2D1_GEOMETRY_RELATION
{
	D2D1_GEOMETRY_RELATION_UNKNOWN        = 0,
	D2D1_GEOMETRY_RELATION_DISJOINT       = 1,
	D2D1_GEOMETRY_RELATION_IS_CONTAINED   = 2,
	D2D1_GEOMETRY_RELATION_CONTAINS       = 3,
	D2D1_GEOMETRY_RELATION_OVERLAP        = 4 
};

#endif

}
}

#include "Geometry.h"
#include "RectangleGeometry.h"
#include "RoundedRectangleGeometry.h"

namespace System
{
namespace Gui
{

enum FillMode
{
	FillMode_Alternate,
	FillMode_Winding,
};

class GUIEXT PolygonGeometry : public Geometry
{
public:
	CTOR PolygonGeometry();
	CTOR PolygonGeometry(float cx, float cy, float rx, float ry);

	virtual ID2D1PathGeometry* GetD2DGeometry() override;
	virtual HRGN CreateHRGN() override;

	virtual void OnComputedPropertyValueChanged(PropertyValue* pPropertyVal, bool handled) override;

//	DEPENDENCY_CLASS(PolygonGeometry, Geometry)

	static DependencyClass* get_Class();
	static DependencyClass* pClass;

	vector<gm::PointF> m_points;	// TODO

	DEPENDENCY_PROPERTY(FillMode, FillMode)

protected:

	ID2D1PathGeometry* m_D2DGeometry;
};

}
}

#include "EllipseGeometry.h"
#include "PathGeometry.h"
#include "CompoundGeometry.h"

namespace System
{
namespace Gui
{

class GUIEXT StyleSheet : public Object, public Style::IStyleSheet
{
public:

	virtual Style::IStyleDeclaration* CreateDeclaration() override;
	virtual void AddRules(Style::SelectorList* selectors, Style::IStyleDeclaration* declaration) override;

	/*
	virtual void MatchElement(Style::CSSElementMatches* matches, Style::ICSSElementResolver* element)
	{
	}

	virtual void GetStyleSheetMatches(Style::CSSElementMatches* matches, Style::ICSSElementResolver* pPElement)
	{
	}
*/

	virtual size_t GetRuleCount() override
	{
		return m_rules.size();
	}

	virtual Style::ICSSRule* GetRule(size_t index) override
	{
		return m_rules[index];
	}

	vector<Style::ICSSRule*> m_rules;
};

class GUIEXT CSSStyleRule : public Object, public Style::ICSSStyleRule
{
public:

	CTOR CSSStyleRule(Style::SelectorList* selectors, Style::IStyleDeclaration* declaration) :
		m_selectors(selectors),
		m_declaration(declaration)
	{
	}

	Style::CSSType get_type() override
	{
		return Style::STYLE_RULE;
	}

	Style::SelectorList* get_selectorList()
	{
		return m_selectors;
	}

	Style::IStyleDeclaration* get_declaration() override
	{
		return m_declaration;
	}

	Style::SelectorList* m_selectors;
	Style::IStyleDeclaration* m_declaration;
};

class PropertyValuePair
{
public:
	DependencyProperty* m_dp;
	Variant m_value;
};

class GUIEXT CSSDeclaration : public Object, public Style::IStyleDeclaration
{
public:

	virtual void SetProperty(StringIn prefix, StringIn name, StringIn strvalue) override;	// IStyleDeclaration
	virtual Variant GetPropertyValue(Object* pProperty) override	// IStyleDeclaration
	{
		return GetPropertyValue((DependencyProperty*)pProperty);
	}
	virtual bool HasProperty(Object* dp) override;

	Variant GetPropertyValue(DependencyProperty* dp);

	vector<PropertyValuePair> m_props;
};

class GUIEXT ObjectBinding : public Object
{
public:
	virtual Variant GetPropertyValue(DependencyProperty* dp) = 0;
	virtual bool HasProperty(DependencyProperty* dp) = 0;
};

class GUIEXT CSSObjectBinding : public ObjectBinding
{
public:

	virtual Variant GetPropertyValue(DependencyProperty* dp) override;
	virtual bool HasProperty(DependencyProperty* dp) override;

	Style::CSSElementMatches* m_ruleMatches;
};

interface ITreeViewController;

class GUIEXT TreeItem
{
public:
	CTOR TreeItem(ITreeViewController* _controller, Object* _data0)  :
		controller(_controller), data0(_data0), data1(0)
	{
	}

	CTOR TreeItem() : controller(nullptr), data0(nullptr), data1(0)
	{
	}

	CTOR TreeItem(decltype(nullptr)) : controller(nullptr), data0(nullptr), data1(0)
	{
	}

	ITreeViewController* controller;
	Object* data0;
	int data1;
};

interface ITreeViewController
{
	virtual float GetItemHeight(TreeItem item) = 0;
	virtual int GetItemState(TreeItem item) = 0;
	virtual void RenderItem(TreeItem item, ManagedRenderContext renderContext, gm::RectF bounds) = 0;
	virtual int ClickItem(TreeItem item, gm::RectF bounds, gm::PointF pos) = 0;
	virtual size_t GetChildrenCount(TreeItem item) = 0;
	virtual TreeItem GetRoot() = 0;
	virtual TreeItem GetChild(TreeItem item, size_t index) = 0;
};

#if 0
class GUIEXT UIStyle : public DependencyObject
{
public:
//	virtual Visual* CreateVisual(UIElement* element);

	virtual GetPropertyValue(DependencyObject* depobj, DependencyProperty* dp) = 0;
};
#endif

class ElementVisuals;

class GUIEXT Element : public DependencyObject, protected Style::ICSSElementResolver
{
protected:

	CTOR Element(DependencyClass* depClass);

public:

//	virtual Element* GetParent();

	IRootVisual* GetRoot()
	{
		return m_root;
	}

	virtual void SetRoot(IRootVisual* root);

//

	virtual String get_typeName() override;
	virtual String get_namespaceURI() override;
	virtual String get_id() override;
	virtual String get_pseudoElementName() override;	// ??
	virtual String getAttribute(StringIn name) override;
	virtual bool hasAttribute(StringIn name) override;
	virtual Style::ICSSElementResolver* get_rparent() override;
	virtual Style::ICSSElementResolver* get_prevSibling() override;
	virtual Style::ICSSElementResolver* get_nextSibling() override;
	virtual Style::IStyleDeclaration* get_inlineStyle(uint index) override;

	virtual bool IsInherited(Object* pProperty) override;
	virtual bool IsValueInherit(Object* value) override;
//

	DEPENDENCY_CLASS_ABSTRACT(Element, DependencyObject)
	//static DependencyClass* get_Class();

	DEFINE_THIS(_Element)

	DEPENDENCY_PROPERTY_RW(String, ID)

protected:

//	virtual Visual* CreateVisual();	// Called only once
//	virtual void RenderVisual();	// Called when to update contents of m_visual

	virtual void OnSetRoot(IRootVisual* root)
	{
	}

	virtual void RenderRetained(ManagedRetainedRenderContext renderContext);

//private:
public:

	IRootVisual* m_root;
};

class GUIEXT _Element : public _DependencyObject
{
public:

	CTOR _Element(const Expressive::expression& exp) : _DependencyObject(exp)
	{
	}

	/*
	Expressive::typed_expression<float> getOpacity() const
	{
		return getProperty(Element::OpacityProperty);
	}

	Expressive::typed_expression<Geometry*> getClip() const
	{
		return getProperty(Element::ClipProperty);
	}
	*/

//	_UIElement getTemplatedOwner() const;

	_Window getOwnerWindow() const;
};

#if 0
class GUIEXT HIconVisual : public Visual
{
public:

	CTOR HIconVisual();

	virtual void Render(ManagedRenderContext renderContext);

	DEFINE_CLASS()

	HICON m_hIcon;
	//ID2D1Bitmap* m_D2DBitmap;
	LDraw::Bitmap* m_bitmap;
};
#endif

}	// Gui
}	// System

namespace MS
{
namespace Windows
{
namespace Shell
{

class Private
{
public:
	static ITaskbarList* pTaskbarList;
	static ITaskbarList3* pTaskbarList3;
};

enum TaskbarItemProgressState
{
	TaskbarItemProgressState_None, // No progress indicator is displayed in the taskbar button. 
	TaskbarItemProgressState_Indeterminate, // A pulsing green indicator is displayed in the taskbar button. 
	TaskbarItemProgressState_Normal, // A green progress indicator is displayed in the taskbar button. 
	TaskbarItemProgressState_Error, // A red progress indicator is displayed in the taskbar button. 
	TaskbarItemProgressState_Paused, //  
};

class GUIEXT TaskbarItemInfo : public System::Gui::DependencyObject
{
public:
	CTOR TaskbarItemInfo();

	DEPENDENCY_PROPERTY_RW(TaskbarItemProgressState, ProgressState)
	DEPENDENCY_PROPERTY_RW(double, ProgressValue)

	DEPENDENCY_CLASS(TaskbarItemInfo, System::Gui::DependencyObject)
};

}	// Shell
}	// Windows
}	// MS

#include "Visual.h"
#include "VisualTreeController.h"

namespace System
{
namespace Gui
{

#if 0
class _TransformVisual;


class GUIEXT DecoratorTransformVisual : public UIElement
{
public:

	CTOR TransformVisual();

	virtual void Render(ManagedRenderContext renderContext) override;
//	virtual UIElement* HitTest(LDraw::PointF point) override;

	DEFINE_THIS(_TransformVisual)

//	DEPENDENCY_CLASS(TransformVisual, Visual)
	static DependencyClass* get_Class();
	static DependencyClass* baseClass;
	typedef UIElement baseClass;

	DEPENDENCY_PROPERTY(Visual*, Child)
//	DEPENDENCY_PROPERTY(Transform*, Transform)
};

class GUIEXT _TransformVisual : public _Visual
{
public:

	/*
	Expressive::typed_expression<Visual*> getChild() const
	{
		return getProperty(ChildProperty);
	}
	*/

	Expressive::typed_expression<Transform*> getTransform() const
	{
		return getProperty(TransformVisual::get_TransformProperty());
	}
};
#endif

/*
class GUIEXT Mesh : public Object
{
public:
	ID3DX10Mesh* m_d3d10_mesh;
};
*/

/*
class GUIEXT TriangleStrip : public Visual
{
public:

	virtual void Render(RenderTarget* rt);

	vector<float3> m_vertices;
};

class GUIEXT IndexedFaceSet : public Visual
{
public:

	virtual void Render(RenderTarget* rt);

	vector<float3> m_vertices;
	vector<int> m_indices;
};
*/

class GUIEXT VisualInstance : public Visual
{
public:

	CTOR VisualInstance() : Visual(get_Class())
	{
	}

	CTOR VisualInstance(Visual* visual) : Visual(get_Class()), m_visual(visual)
	{
	}

	DEPENDENCY_CLASS(VisualInstance, Visual)

	virtual void Render(ManagedRenderContext renderContext) override;

	Visual* m_visual;
};

class Visuals;

/*
class GUIEXT ElementVisuals : public Visuals
{
public:
	CTOR ElementVisuals();

//	virtual void Add(Visual* visual);

	virtual void Render(ManagedRenderContext renderContext) override;
	virtual UIElement* HitTest(LDraw::PointF point) override;

	Element* m_element;

	static DependencyClass* get_Class();
	static DependencyClass* pClass;

//	DEPENDENCY_CLASS(ElementVisuals, Visuals)
};
*/

}
}

#include "VisualCollection.h"

namespace System
{
namespace Gui
{

	/*
class GUIEXT FontFamily : public DependencyObject
{
public:

	CTOR FontFamily();
	CTOR FontFamily(StringIn name);

	static DependencyClass* get_Class();
	static DependencyClass* pClass;

	DEPENDENCY_PROPERTY(String, Name)
};
*/

GUIEXT void Edit(TextDocument* td);

template<class T>
class Vector2 : public DependencyObject
{
public:

	CTOR Vector2() : DependencyObject(get_Class())
	{
	}

	CTOR Vector2(typed<float> x, typed<float> y) : DependencyObject(get_Class())
	{
		SetBinding(this, get_XProperty(), x);
		SetBinding(this, get_YProperty(), y);
	}

//	DEPENDENCY_CLASS(Vector2<T>, DependencyObject)

	static DependencyClass* get_Class();
	typedef DependencyObject baseClass;
	typedef Vector2<T> thisClass;

	DEPENDENCY_PROPERTY_RW(T, X)
	DEPENDENCY_PROPERTY_RW(T, Y)

private:

	static DependencyClass* pClass;
};

#ifndef GUI_EXPORTS
#ifndef __LERSTAD__

template class DECLSPEC_DLLIMPORT Vector2<unsigned char>;
template class DECLSPEC_DLLIMPORT Vector2<int>;
template class DECLSPEC_DLLIMPORT Vector2<float>;
template class DECLSPEC_DLLIMPORT Vector2<double>;

#endif
#endif

typedef Vector2<unsigned char> Vector2_B;
typedef Vector2<int> Vector2_I;
typedef Vector2<long long> Vector2_L;
typedef Vector2<float> Vector2_F;
typedef Vector2<double> Vector2_D;

template<class T>
class Vector3 : public DependencyObject
{
public:

	CTOR Vector3() : DependencyObject(get_Class())
	{
	}

	CTOR Vector3(gm::vector3<T> v) : DependencyObject(get_Class())
	{
		set_X(v[0]);
		set_Y(v[1]);
		set_Z(v[2]);
	}

	CTOR Vector3(typed<float> x, typed<float> y, typed<float> z) : DependencyObject(get_Class())
	{
		SetBinding(this, get_XProperty(), x);
		SetBinding(this, get_YProperty(), y);
		SetBinding(this, get_ZProperty(), z);
	}

	gm::vector3<T> get_XYZ()
	{
		return gm::vector3<T>(get_X(), get_Y(), get_Z());
	}

//	DEPENDENCY_CLASS(Vector3<T>, DependencyObject)
	static DependencyClass* get_Class();
	static DependencyClass* pClass;
	typedef Vector3<T> thisClass;
	typedef DependencyObject baseClass;

	DEPENDENCY_PROPERTY_RW(T, X)
	DEPENDENCY_PROPERTY_RW(T, Y)
	DEPENDENCY_PROPERTY_RW(T, Z)
};

typedef Vector3<unsigned char> Vector3_B;
typedef Vector3<int> Vector3_I;
typedef Vector3<long long> Vector3_L;
typedef Vector3<float> Vector3_F;
typedef Vector3<double> Vector3_D;

#ifndef GUI_EXPORTS
#ifndef __LERSTAD__

template class DECLSPEC_DLLIMPORT Vector3<unsigned char>;
template class DECLSPEC_DLLIMPORT Vector3<int>;
template class DECLSPEC_DLLIMPORT Vector3<float>;
template class DECLSPEC_DLLIMPORT Vector3<double>;

#endif
#endif

template<class T>
class Vector4 : public DependencyObject
{
public:

	CTOR Vector4() : DependencyObject(get_Class())
	{
	}

	static DependencyClass* get_Class();
	static DependencyClass* pClass;
	typedef Vector4<T> thisClass;
	typedef DependencyObject baseClass;

	DEPENDENCY_PROPERTY_RW(T, X)
	DEPENDENCY_PROPERTY_RW(T, Y)
	DEPENDENCY_PROPERTY_RW(T, Z)
	DEPENDENCY_PROPERTY_RW(T, W)
};

typedef Vector4<unsigned char> Vector4_B;
typedef Vector4<int> Vector4_I;
typedef Vector4<long long> Vector4_L;
typedef Vector4<float> Vector4_F;
typedef Vector4<double> Vector4_D;

}
}

#include "Cursor.h"
#include "SystemColors.h"

namespace System
{
namespace Gui
{

/*
class GUIEXT PolygonVisual : public ShapeVisual
{
public:

	virtual void Render(RenderContext* renderContext);

	vector<LDraw::PointF> m_points;
};

class GUIEXT PathVisual : public ShapeVisual
{
public:
	CTOR PathVisual();

	virtual void Render(RenderContext* renderContext);

	PathData* m_pathData;

	class InitClass
	{
	public:
		CTOR InitClass();
	};

	static InitClass s_initClass;
};
*/

class GUIEXT SystemBrushes
{
private:

	CTOR SystemBrushes();
};

class SystemFonts
{
private:

	CTOR SystemFonts();
};

/*
template<class ContainerClass, class TYPE> class LocalPropertyT
{
public:

	void SetLocal(TYPE value)
	{
	}

	TYPE GetLocal()
	{
	}

	TYPE Get()
	{
	}

	void ClearLocal()
	{
	}

	//property_t& operator = (TYPE
};
*/

class GUIEXT FillEllipseVisual : public Visual
{
public:

	CTOR FillEllipseVisual();

	CTOR FillEllipseVisual(
		typed<float> cx,
		typed<float> cy,
		typed<float> rx,
		typed<float> ry,
		typed<Brush*> brush);

	virtual void Render(ManagedRenderContext renderContext) override;

	DEPENDENCY_CLASS(FillEllipseVisual, Visual)

	DEPENDENCY_PROPERTY(float, Cx)
	DEPENDENCY_PROPERTY(float, Cy)
	DEPENDENCY_PROPERTY(float, Rx)
	DEPENDENCY_PROPERTY(float, Ry)
	DEPENDENCY_PROPERTY(Brush*, Brush)
};

}	// Gui
}	// System

namespace System
{
namespace Gui
{

class Symbol : public Object
{
public:
};

class GUIEXT VisualSymbol : public Symbol
{
public:
	String m_name;
	Visual* m_root;
};

class GUIEXT InstanceVisual : public Visual
{
public:
	CTOR InstanceVisual();
	CTOR InstanceVisual(VisualSymbol* symbol, gm::RectF rect);

	VisualSymbol* m_symbol;
	gm::RectF m_rect;
};

/*
// ?? possible to implement directly on UIElement ??, probably not
interface ILayout
{
	virtual Layout* GetParent() = 0;
	virtual unsigned int GetChildrenCount() = 0;
	virtual Layout* GetChild(unsigned int index) = 0;
	virtual void AddChild(Layout* child) = 0;
};
*/

class GUIEXT Layout : public DependencyObject
{
public:

	CTOR Layout(UIElement* element);

	virtual gm::SizeF PreLayout(gm::SizeD availSize) = 0;
	virtual gm::SizeF LayOut(gm::RectF layoutRect) = 0;

	gm::SizeF get_DesiredSize();

	Layout* GetParent();
	virtual size_t GetChildrenCount();
	virtual Layout* GetChild(size_t index);
	virtual void AddChild(Layout* child);

	Layout* m_parent;

	DEPENDENCY_CLASS_ABSTRACT(Layout, DependencyObject)

	DEPENDENCY_PROPERTY(float, DesiredWidth)
	DEPENDENCY_PROPERTY(float, DesiredHeight)
	DEPENDENCY_PROPERTY(float, ActualWidth)
	DEPENDENCY_PROPERTY(float, ActualHeight)

	UIElement* m_element;
};

class GUIEXT BorderLayout : public Layout
{
public:

};

class GUIEXT PanelLayout : public Layout
{
public:

	CTOR PanelLayout(UIElement* element) : Layout(element)
	{
	}

	virtual size_t GetChildrenCount() override;
	virtual Layout* GetChild(size_t index) override;
	virtual void AddChild(Layout* child) override;

private:

	vector<Layout*> m_children;
};

class GUIEXT StackLayout : public PanelLayout
{
public:

	CTOR StackLayout(UIElement* element);

	enum Direction
	{
		Direction_Horizontal,
		Direction_Vertical,
	};

	virtual gm::SizeF PreLayOut(gm::RectF availRect);
	virtual gm::SizeF LayOut(gm::RectF layoutRect);

	Direction m_direction;
};

template<class T>
class Channel
{
public:

	Channel& operator = (T value)
	{
		return *this;
	}

	operator T ()
	{
		DWORD result = WaitForSingleObject(m_handle, INFINITE);
		ASSERT(result == WAIT_OBJECT_0);
	}

	HANDLE m_handle;
	T m_value;
};

interface IViewable
{
	virtual uint get_ViewCount() abstract;
	virtual Object* getView(uint index) abstract;
	virtual void addView(Object* view) abstract;
};

enum CapStyle
{
	CapStyle_Flat       = 0,
	CapStyle_Square     = 1,
	CapStyle_Round      = 2,
	CapStyle_Triangle   = 3 
};

enum LineJoinStyle
{
	LineJoinStyle_Miter	= 0,
	LineJoinStyle_Bevel	= 1,
	LineJoinStyle_Round	= 2,
	LineJoinStyle_MiterOrBevel	= 3
};

enum DashStyle
{
	DashStyle_solid = 0,
	DashStyle_dash = 1,
	DashStyle_dot = 2,
	DashStyle_dash_dot = 3,
	DashStyle_dash_dot_dot = 4,
//	DashStyle_STYLE_CUSTOM = 5,
};

}

}	// System

#include "StrokeStyle.h"
#include "UIElement.h"
#include "ImageVisual.h"
#include "VideoElement.h"
#include "LineVisual.h"
#include "Group.h"
#include "WindowThumbnailVisual.h"

namespace System
{
namespace Gui
{

class GUIEXT Visuals : public UIElement
{
public:
	CTOR Visuals();
//	CTOR Visuals(DependencyClass* depClass);

	virtual size_t GetChildrenCount() override;
	virtual Visual* GetChild(size_t index) override;

	virtual void AddChild(Visual* child) override;

	void Add(Visual* visual)
	{
		AddChild(visual);
	}

	virtual void Render(ManagedRenderContext renderContext) override;
//	virtual UIElement* HitTest(LDraw::PointF point) override;

	Element* m_element;

	vector<Visual*> m_items;

//	DEPENDENCY_CLASS(Visuals, Visual)
	static DependencyClass* get_Class();
	static DependencyClass* pClass;
};

}	// Gui
}	// System

#include "RenderGeometryVisual.h"
#include "FillGeometryVisual.h"
#include "StrokeGeometryVisual.h"
#include "TextVisual.h"

namespace System
{
namespace Gui
{

class GUIEXT FrameworkElement : public UIElement
{
protected:
	CTOR FrameworkElement(DependencyClass* depClass);
	CTOR FrameworkElement(DependencyClass* depClass, gm::SizeF size);

public:

	virtual bool HasComputedPropertyValue(DependencyProperty* dp) override;
	virtual Variant GetComputedPropertyObjectValue2(DependencyProperty* dp) override;

	DEFINE_THIS(_FrameworkElement)

	DEPENDENCY_CLASS(FrameworkElement, UIElement)

	DEPENDENCY_PROPERTY_RW(ObjectBinding*, Binding)
};

class GUIEXT _FrameworkElement : public _UIElement
{
public:
	CTOR _FrameworkElement(const Expressive::expression& exp) : _UIElement(exp)
	{
	}
};

}
}

#include "Panel.h"
#include "ContentElement.h"

namespace System
{
namespace Gui
{

template<class value_type> class ValueBinding : public OneToManyOutputsBinding
{
public:

	CTOR ValueBinding()
	{
	}

	CTOR ValueBinding(value_type value) : m_value(value)
	{
	}

	virtual void OnSetValue(value_type value)
	{
	}

	virtual Variant GetValue()
	{
		/*
		if (m_objectValue == NULL)
		{
			m_objectValue = box_cast(m_value);
		}
		*/

		return m_value;
	}

	virtual void SetValue(value_type value)
	{
	//	m_objectValue = NULL;
		m_value = value;
		OnSetValue(value);

		if (!m_outBindings.empty())
		{
			OutputValue(GetValue());
		}
	}

	/*
	virtual void OnObjectChanged(Object*) override
	{
		if (!m_outBindings.empty())
		{
			OutputValue(GetValue());
		}
	}
	*/

	virtual void SetValue(const Variant& value) override
	{
		m_value = value;
		OnSetValue(m_value);

		if (!m_outBindings.empty())
		{
			OutputValue(m_value);
		}
	}

	virtual ValueBinding<value_type>& operator = (value_type value)
	{
		SetValue(value);
		return *this;
	}

	virtual operator value_type ()
	{
		return m_value;
	}

	virtual Type* GetOutputType() override
	{
		return typeid(value_type);
	}

	value_type m_value;
};

template<class to_t, class from_t> to_t convert_(from_t value)
{
	return value;
}

#ifndef __LERSTAD__
template<> inline bool convert_<bool>(BOOL value)
{
	return value? true: false;
}
#endif

template<class value_type, class spvalue_type>
class SystemParameter : public ValueBinding<value_type>
{
public:

	virtual void OnSetValue(value_type value) override
	{
		if (!m_set)
		{
			m_set = true;

			spvalue_type spvalue = value;
			SystemParametersInfo(m_set_sp, spvalue, NULL, 0);

			m_set = false;
		}
	}

	CTOR SystemParameter(uint get_sp, uint set_sp) : m_get_sp(get_sp), m_set_sp(set_sp), m_set(false)
	{
		spvalue_type spvalue;
		SystemParametersInfo(m_get_sp, 0, &spvalue, 0);

		m_set = true;
#ifndef __LERSTAD__
		SetValue(convert_<value_type>(spvalue));
#endif
		m_set = false;
	}

	uint m_get_sp;
	uint m_set_sp;
	bool m_set;
};

class GUIEXT SystemInformation
{
public:

#if defined(SPI_GETDROPSHADOW)

	static SystemParameter<bool, BOOL>& get_IsDropShadowEnabled()
	{
#ifndef __LERSTAD__
		static SystemParameter<bool, BOOL> s(SPI_GETDROPSHADOW, SPI_SETDROPSHADOW);
#endif
		return s;
	}

#endif

#if defined(SPI_GETFLATMENU)
	static SystemParameter<bool, BOOL>& get_IsFlatMenuEnabled()
	{
#ifndef __LERSTAD__
		static SystemParameter<bool, BOOL> s(SPI_GETFLATMENU, SPI_SETFLATMENU);
#endif
		return s;
	}

#endif

//	static bool get_IsListBoxSmoothScrollingEnabled();
};

}	// Gui
}	// System

#include "DockPanel.h"

#include "Mouse.h"

namespace System
{
namespace Gui
{

class GUIEXT Stylus
{
public:

//	static ValueBinding<float> s_ScreenXBinding;
//	static ValueBinding<float> s_ScreenYBinding;
	static ValueBinding<float> s_PressureBinding;
//	static ValueBinding<bool> s_DownBinding;

private:
	CTOR Stylus();
};

class GUIEXT _Stylus
{
public:
	static Expressive::typed_expression<float> Pressure();
};

class GUIEXT Date : public DependencyObject
{
public:
	CTOR Date();

	DEPENDENCY_PROPERTY(int, Year)
	DEPENDENCY_PROPERTY(int, TimeOfDay)
	DEPENDENCY_PROPERTY(int, DayOfWeek)
	DEPENDENCY_PROPERTY(int, DayOfMonth)
	DEPENDENCY_PROPERTY(int, HourOfDay)
	DEPENDENCY_PROPERTY(int, Second)
	DEPENDENCY_PROPERTY(int, Minute)
	DEPENDENCY_PROPERTY(int, Milliseconds)

	DEPENDENCY_CLASS(Date, DependencyObject)

	static Date* get_CurrentDate();
};

class GUIEXT Screen
{
public:

	static Expressive::Expression* get_Width();
	static Expressive::Expression* get_Height();

private:

	CTOR Screen();
};

typedef SystemParameter<bool, BOOL> _CursorShadow;
typedef SystemParameter<int, int> _MouseTrails;

class GUIEXT SystemParameters : public DependencyObject
{
public:

//	static ValueBinding<bool>* get_ClientAreaAnimation();
//	static ValueBinding<bool>* get_DisableOverlappedContent();

	static ValueBinding<bool>* get_CursorShadow();
	static ValueBinding<int>* get_MouseTrails();

private:

	CTOR SystemParameters();
};

class GUIEXT VisualElement : public UIElement
{
public:

	CTOR VisualElement();
	CTOR VisualElement(Visual* visual);

	virtual void OnComputedPropertyValueChanged(PropertyValue* pPropertyVal, bool handled) override;

	static DependencyClass* get_Class();

	DEPENDENCY_PROPERTY(Visual*, Visual)

protected:

	virtual Visual* CreateVisual();
};

}	// Gui
}	// System

//#include "TextElement.h"
#include "Control.hpp"

namespace System
{
namespace Gui
{

class GUIEXT ContentControl : public Control
{
protected:

	CTOR ContentControl(DependencyClass* depClass);
	CTOR ContentControl(DependencyClass* depClass, Object* content);
	CTOR ContentControl(DependencyClass* depClass, Object* content, gm::SizeF size);

public:

	Template* get_ContentTemplate();
	void set_ContentTemplate(Template* pTemplate);

//	virtual void OnComputedPropertyValueChanged(PropertyValue* pPropertyVal) override;

	virtual size_t GetLogicalChildrenCount() override;
	virtual Object* GetLogicalChild(size_t position) override;
	virtual Object* ReplaceChild(size_t position, Object* newChild) override;

	virtual void OnComputedPropertyValueChanged(PropertyValue* pPropertyVal, bool handled) override;

	DEFINE_THIS(_ContentControl)

	DEPENDENCY_CLASS_ABSTRACT(ContentControl, Control)

	DEPENDENCY_PROPERTY_RW(Object*, Content)
	DEPENDENCY_PROPERTY_RW(float, ContentWidth)
	DEPENDENCY_PROPERTY_RW(float, ContentHeight)
	DEPENDENCY_PROPERTY_RW(float, MinContentWidth)
	DEPENDENCY_PROPERTY_RW(float, MinContentHeight)
	DEPENDENCY_PROPERTY_RW(float, MaxContentWidth)
	DEPENDENCY_PROPERTY_RW(float, MaxContentHeight)
};

class _ContentControl : public _Control
{
public:
	CTOR _ContentControl(const Expressive::expression& exp) : _Control(exp)
	{
	}

	Expressive::typed_expression<Object*> getContent() const
	{
		return getProperty(ContentControl::get_ContentProperty());
	}

	Expressive::typed_expression<float> getContentWidth() const
	{
		return getProperty(ContentControl::get_ContentWidthProperty());
	}

	Expressive::typed_expression<float> getContentHeight() const
	{
		return getProperty(ContentControl::get_ContentHeightProperty());
	}
};

class GUIEXT Decorator : public FrameworkElement
{
public:

	CTOR Decorator(DependencyClass* depClass);

	virtual size_t GetChildrenCount();
	virtual void AddChild(UIElement* element);
	virtual UIElement* GetChild(size_t index);

	DEPENDENCY_PROPERTY_RW(UIElement*, Child)

	static DependencyClass* get_Class();
};

class GUIEXT Border : public Decorator
{
public:

	CTOR Border();

	DEPENDENCY_CLASS(Border, Decorator)
};

}	// Gui
}	// System

#include "Cursors.h"
#include "Button.h"
#include "PlatformWindow.h"
#include "Window.h"
#include "Popup.h"
#include "ForeignWindow.h"
#include "ActiveXHostVisual.h"
#include "Thumb.h"
#include "Track.h"
#include "Scrollbar.h"
#include "Slider.h"
#include "ScrollViewer.h"
#include "Viewport.h"

namespace System
{
namespace Gui
{

class GUIEXT PrimitiveShape : public Object
{
public:
	virtual bool IsValid(ImmediateRenderContext* renderContext) abstract;
	virtual void Draw(ImmediateRenderContext* renderContext) abstract;
	virtual void Destroy()
	{
	}
};

class GUIEXT BitmapShape : public PrimitiveShape
{
public:

	CTOR BitmapShape();

	virtual bool IsValid(ImmediateRenderContext* renderContext) override;
	virtual void Draw(ImmediateRenderContext* renderContext) override;

	void Create(Graphics::RenderTarget* rt, Graphics::Bitmap* bitmap, gm::RectF dstRect, gm::RectF srcRect);

	ID3D10Buffer* m_d3d10_VertexBuffer;	// TODO, remove
	Graphics::VertexDeclaration m_vertexDeclaration;
	Graphics::VertexBuffer m_vertexBuffer;

	Graphics::Bitmap* m_bitmap;
};

/*
class GUIEXT Texture2D : public Object
{
public:
};
*/

/*
class GUIEXT BlendState : public Object
{
public:
};
*/

class GUIEXT EffectBase : public DependencyObject
{
public:
};

class GUIEXT EffectTechniqueBase : public DependencyObject
{
public:
};

class GUIEXT DXEffect : public Object
{
public:

	CTOR DXEffect();
	CTOR DXEffect(StringIn filename);

	bool Load(StringIn filename);

	ClassType* GenerateClass(StringIn name);

	ID3DX10Effect* m_effect;
	D3DX10_EFFECT_DESC m_desc;
};

}
}

#include "TreeView.h"

namespace System
{
namespace Gui
{
namespace Media3D
{

class Document3D;

class GUIEXT BoxShape : public PrimitiveShape
{
public:

	CTOR BoxShape()
	{
		m_d3d10_VertexBuffer[0] = NULL;
		m_d3d10_VertexBuffer[1] = NULL;
		m_d3d10_IndexBuffer = NULL;
	}

	virtual ~BoxShape();

	virtual bool IsValid(ImmediateRenderContext* renderContext) override;
	virtual void Draw(ImmediateRenderContext* renderContext) override;
	void Create(RenderContext* renderContext, gm::vector3f size);

	ID3D10Buffer* m_d3d10_VertexBuffer[2];
	ID3D10Buffer* m_d3d10_IndexBuffer;

	Graphics::VertexDeclaration m_vertexDeclaration;
	Graphics::VertexBuffer m_vertexBuffer;
	Graphics::IndexBuffer m_indexBuffer;
};

class GUIEXT SphereShape : public PrimitiveShape
{
public:

	CTOR SphereShape();
	virtual ~SphereShape();

	virtual bool IsValid(ImmediateRenderContext* renderContext) override;
	virtual void Draw(ImmediateRenderContext* renderContext) override;
	void Create(RenderContext* renderContext, double radius, int slices, int stacks);

	ID3D10Buffer* m_d3d10_VertexBuffer[2];
	uint m_slices;
	uint m_stacks;
};

}	// Media3D
}	// Gui
}	// System

#include "Object3D.h"
#include "Visual3D.h"
#include "Geometry3D.h"
#include "Material.h"

namespace System
{
namespace Gui
{
namespace Media3D
{

class GUIEXT Mesh : public Geometry3D
{
public:

	CTOR Mesh() : Geometry3D(get_Class())
	{
	}

	void SetIndexData(const void* data, uint count)
	{
		m_pMesh->SetIndexData(data, count);
	}
	void SetVertexData(uint iBuffer, const void* data)
	{
		m_pMesh->SetVertexData(iBuffer, data);
	}

	virtual void Render(ManagedRenderContext renderContext) override;

//	static void Init();

	static DependencyClass* get_Class();
//	DEPENDENCY_CLASS(Mesh, Geometry3D)

	vector<Material*> m_subsetMaterial;
	ID3DX10Mesh* m_pMesh;
};

}	// Media3D
}	// Gui
}	// System

#include "Box.h"
#include "Sphere.h"

namespace System
{
namespace Gui
{
namespace Media3D
{

class GUIEXT Cylinder : public Geometry3D
{
public:

	CTOR Cylinder();

	virtual void Render(ManagedRenderContext renderContext) override;

	DEPENDENCY_CLASS(Cylinder, Geometry3D)

	static TypedDependencyProperty<float> TopRadiusProperty;
	static TypedDependencyProperty<float> BottomRadiusProperty;
	static TypedDependencyProperty<float> HeightProperty;
};

class GUIEXT Torus : public Geometry3D
{
public:
};

class GUIEXT HeightMap : public Geometry3D
{
public:
	CTOR HeightMap() : Geometry3D(get_Class())
	{
	}

	void set_Bitmap(Graphics::Bitmap* bitmap)
	{
		m_bitmap = bitmap;
	}

	virtual void Render(ManagedRenderContext renderContext) override;

	virtual void AddShapeDesc(physx::PxScene* scene, physx::PxRigidActor* actorDesc);

	Graphics::Bitmap* m_bitmap;

	ID3D10InputLayout* m_inputLayout;
	ID3D10Buffer* m_d3d10_VertexBuffer;
	ID3D10Buffer* m_d3d10_IndexBuffer;

	DEPENDENCY_CLASS(HeightMap, Geometry3D)
//	static DependencyClass* get_Class();
//	static DependencyClass* pClass;
//	typedef Geometry3D baseClass;
};

class GUIEXT Light : public Visual3D
{
protected:
	CTOR Light(DependencyClass* depClass);

public:

	virtual void Render(ManagedRenderContext renderContext) override
	{
	}

	DEPENDENCY_PROPERTY(Color*, Color)
	DEPENDENCY_PROPERTY(float, Intensity)
	DEPENDENCY_PROPERTY(float, AmbientIntensity)

	DEPENDENCY_CLASS_ABSTRACT(Light, Visual3D)
};

class GUIEXT DirectionalLight : public Light
{
public:

};

}	// Media3D
}	// Gui
}	// System

#include "PointLight.h"

namespace System
{
namespace Gui
{
namespace Media3D
{

class GUIEXT Visual2D : public Visual3D
{
public:
};

class GUIEXT Particles : public Visual3D
{
public:
};

class ForceField : public Visual3D
{
public:
};

#if 0
struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
{
    LPDIRECT3DTEXTURE9* ppTextures;       // array of textures, entries are NULL if no texture specified    

    // SkinMesh info             
    LPD3DXMESH pOrigMesh;
    LPD3DXATTRIBUTERANGE pAttributeTable;
    DWORD NumAttributeGroups;
    DWORD NumInfl;
    LPD3DXBUFFER pBoneCombinationBuf;
    D3DXMATRIX** ppBoneMatrixPtrs;
    D3DXMATRIX* pBoneOffsetMatrices;
    DWORD NumPaletteEntries;
    bool UseSoftwareVP;
    DWORD iAttributeSW;     // used to denote the split between SW and HW if necessary for non-indexed skinning
};
#endif

class GUIEXT MeshContainer : public Visual3D
{
public:

	CTOR MeshContainer() : Visual3D(get_Class())
	{
	}

	virtual void Render(ManagedRenderContext renderContext) override;

//	Mesh* m_mesh;

	LPSTR Name;

	Mesh* m_mesh;

	ID3DX10Mesh* MeshData;
//	D3DXMESHDATA            MeshData;

	/*
	LPD3DX10MATERIAL          pMaterials;
	LPD3DX10EFFECTINSTANCE    pEffects;
	DWORD                   NumMaterials;
	DWORD                  *pAdjacency;
*/
	ID3DX10SkinInfo* pSkinInfo;

//	struct _D3DXMESHCONTAINER *pNextMeshContainer;

	/////

	// SkinMesh info             
	ID3DX10Mesh* pOrigMesh;
//	D3DX10_ATTRIBUTE_RANGE* pAttributeTable;
	DWORD NumAttributeGroups;
	DWORD NumInfl;
//	ID3DX10Buffer* pBoneCombinationBuf;
	D3DXMATRIX** ppBoneMatrixPtrs;
	D3DXMATRIX* pBoneOffsetMatrices;
	DWORD NumPaletteEntries;
	bool UseSoftwareVP;
	DWORD iAttributeSW;     // used to denote the split between SW and HW if necessary for non-indexed skinning

//	DEPENDENCY_CLASS(MeshContainer, Visual3D)
};

class GUIEXT Shape3D : public Visual3D
{
public:
	CTOR Shape3D();

	virtual void Render(ManagedRenderContext renderContext) override;

	DEPENDENCY_PROPERTY(Geometry3D*, Geometry)
	DEPENDENCY_PROPERTY(Material*, Material)

	DEPENDENCY_CLASS(Shape3D, Visual3D)
};

}	// Media3D
}	// Gui
}	// System

#include "GeneralTransform3D.h"
#include "Transform3D.h"
#include "TranslateTransform3D.h"
#include "ScaleTransform3D.h"

namespace System
{
namespace Gui
{
namespace Media3D
{

/*
class GUIEXT RotateTransform3D : public Transform3D
{
public:

	CTOR RotateTransform3D();

	virtual D3DXMATRIX GetMatrix() override;

	static DependencyClass s_Class;

	static TypedDependencyProperty<float> AxisXProperty;
	static TypedDependencyProperty<float> AxisYProperty;
	static TypedDependencyProperty<float> AxisZProperty;
	static TypedDependencyProperty<float> AngleProperty;
};
*/

class GUIEXT MatrixTransform3D : public Transform3D
{
public:
	CTOR MatrixTransform3D();

	virtual D3DXMATRIX GetMatrix() override;

	DEPENDENCY_CLASS(MatrixTransform3D, Transform3D)

	D3DXMATRIX m_matrix;
};

class GUIEXT Group : public Visual3D
{
public:

	CTOR Group();

	virtual size_t GetChildrenCount() throw () override;
	virtual Visual3D* GetChild(size_t index) throw (ArgumentOutOfRangeException*) override;

	void AddChild(Visual3D* obj) throw (ArgumentNullException*);

	/*
	void SetTransform(const LDraw::matrix4f& transform)
	{
		m_transform = transform;
	}
	*/

	virtual void Render(ManagedRenderContext renderContext) override;
	virtual void SetLight(ImmediateRenderContext*) override;

	DEPENDENCY_CLASS(Group, Visual3D)

	vector<Visual3D*> m_children;
//	LDraw::matrix4f m_transform;
};

class RayMeshIntersect : public DependencyProperty
{
public:

	// in
	static TypedDependencyProperty<Mesh*> MeshProperty;
	static TypedDependencyProperty<Vector3_F*> RayStartProperty;
	static TypedDependencyProperty<Vector3_F*> RayDirectionProperty;

	// out
	static TypedDependencyProperty<int> FaceIndexProperty;
	static TypedDependencyProperty<float> UProperty;
	static TypedDependencyProperty<float> VProperty;
	static TypedDependencyProperty<float> DistProperty;
};

class GUIEXT AnimationKey : public Object
{
public:
	double m_time;
};

class GUIEXT Animation : public Object
{
public:
	Object3D* m_Target;

	vector<AnimationKey*> m_keys;
};

/*
interface Object3DCollection
{
};
*/

class GUIEXT MatrixAnimationKey : public AnimationKey
{
public:
	CTOR MatrixAnimationKey()
	{
	}

	gm::matrix4f m_value;
};

class GUIEXT MatrixAnimation : public Animation
{
public:
	CTOR MatrixAnimation()
	{
		m_Target = NULL;
	}
};

class GUIEXT Document3D : public Object
{
public:
//	virtual Group* CreateGroup(String name) = 0;
//	virtual IMesh* CreateMesh(uint nVertices, uint nFaces) = 0;
	Object3D* FindByName(String name);
	void AddAnimation(Animation* animation);
};

class GUIEXT Camera : public DependencyObject, public ICamera
{
protected:

	CTOR Camera(DependencyClass* depClass) : DependencyObject(depClass)
	{
	}

public:

//	static DependencyClass* get_Class();
//	typedef DependencyObject baseClass;
	DEPENDENCY_CLASS_ABSTRACT(Camera, DependencyObject)
};

}	// Media3D
}	// Gui
}	// System

#include "PerspectiveCamera.h"
#include "Object3DVisual.h"

namespace System
{
namespace Gui
{
namespace Media3D
{

struct VERTEX_XYZ_NORMAL
{
	gm::vector3f position;
	gm::vector3f normal;
};

struct VERTEX_XYZ_COLOR
{
	gm::vector3f position;
	gm::vector4f color;
};

struct VERTEX_NORMAL_TEX
{
	gm::vector3f position;
	gm::vector3f normal;
	gm::vector2f texCoord[1];
};

class GUIEXT SkySphereShape : public PrimitiveShape
{
public:

	CTOR SkySphereShape();
	virtual ~SkySphereShape();

	virtual bool IsValid(ImmediateRenderContext* renderContext) override;
	virtual void Draw(ImmediateRenderContext* renderContext) override;
	virtual void Destroy() override;
	void Create(RenderContext* renderContext, double radius, int slices, vector<float>& angles, vector<gm::vector4f>& colors, int nsign = 1);

	ID3DX10EffectTechnique* m_d3d10_technique;
	ID3D10Buffer* m_d3d10_VertexBuffer[1];
	ID3D10InputLayout* m_inputLayout;
	uint m_slices;
	uint m_stacks;
};

class GUIEXT Lines2 : public PrimitiveShape
{
public:

	CTOR Lines2();
	virtual ~Lines2();

	virtual bool IsValid(ImmediateRenderContext* renderContext) override;
	virtual void Draw(ImmediateRenderContext* renderContext) override;
	void Create(RenderContext* renderContext, gm::vector3f* points, uint nvertices, gm::vector3f* colors, bool colorPerVertex);
	void Create(RenderContext* renderContext, gm::vector3f* points, uint nvertices, gm::vector4f color);

	ID3D10Buffer* m_d3d10_VertexBuffer;
	uint m_nvertices;
};

class GUIEXT IndexedLines : public PrimitiveShape
{
public:

	CTOR IndexedLines();
	virtual ~IndexedLines();

	virtual bool IsValid(ImmediateRenderContext* renderContext) override;
	virtual void Draw(ImmediateRenderContext* renderContext) override;
	void Create(RenderContext* renderContext, gm::vector3f* points, uint nvertices, uint* indices, uint nindices, gm::vector4f color);

	ID3D10Buffer* m_d3d10_VertexBuffer;
	ID3D10Buffer* m_d3d10_IndexBuffer;
	uint m_nvertices;
	uint m_nindices;
};

class GUIEXT CylinderShape : public PrimitiveShape
{
public:

	CTOR CylinderShape()
	{
		m_d3d10_VertexBuffer = NULL;
	}

	virtual ~CylinderShape();

	virtual bool IsValid(ImmediateRenderContext* renderContext) override;
	virtual void Draw(ImmediateRenderContext* renderContext) override;
	void Create(RenderContext* renderContext, float bottomRadius, float topRadius, float height);

	ID3D10Buffer* m_d3d10_VertexBuffer;
	uint m_nvertices;
};

class GUIEXT DiskShape : public PrimitiveShape
{
public:

	CTOR DiskShape();

	virtual bool IsValid(ImmediateRenderContext* renderContext) override;
	virtual void Draw(ImmediateRenderContext* renderContext) override;
	void Create(RenderContext* renderContext, float innerRadius, float outerRadius);

	ID3D10Buffer* m_d3d10_VertexBuffer;
	ID3D10Buffer* m_d3d10_IndexBuffer;
};

class GUIEXT Rectangle2DShape : public PrimitiveShape
{
public:

	CTOR Rectangle2DShape();
	virtual ~Rectangle2DShape();

	virtual bool IsValid(ImmediateRenderContext* renderContext) override;
	virtual void Draw(ImmediateRenderContext* renderContext) override;
	void Create(RenderContext* renderContext, gm::vector2f size);

	ID3D10Buffer* m_d3d10_VertexBuffer[2];
};

}

class GUIEXT Viewbox : public Decorator
{
public:

	CTOR Viewbox();

	gm::RectF m_rect;

	static DependencyClass Class;

protected:

	virtual void RenderRetained(ManagedRetainedRenderContext renderContext) override;
};

class GUIEXT ItemsControl : public Control
{
public:
	CTOR ItemsControl(DependencyClass* depClass);

	virtual void AddItem(Object* item);
	void AddItem(StringIn str);

	DEPENDENCY_PROPERTY(int, SelectedIndex)
	DEPENDENCY_PROPERTY(Object*, SelectedItem)

	DEPENDENCY_CLASS_ABSTRACT(ItemsControl, Control)

protected:

	map<Object*,UIElement*> m_objectToElement;
	map<UIElement*,Object*> m_elementToObject;

	virtual void OnAddElement(UIElement* element) abstract;
};

}
}

#include "ListBox.h"

namespace System
{
namespace Gui
{
/*
class GUIEXT Module
{
};
*/

class GUIEXT TimeManager
{
public:

	void Tick();

	static TimeManager manager;

	static ValueBinding<double>* get_GlobalTime();

	Event1<double> OnTick;

private:

	CTOR TimeManager();

	static ValueBinding<double> s_GlobalTime;

	list<Clock*> m_clocks;	// TODO remove ??
//	double m_globalTime;
};

class GUIEXT ColorDialog
{
public:
	static ColorF ChooseColor();

	COLORREF m_customColors[16];

	static COLORREF customColors[16];
};

}	// Gui
}	// System

#include "Clipboard.h"

#include "Application.h"

namespace System
{
namespace Gui
{

class GUIEXT Directory : public Object
{
public:

	CTOR Directory();

	virtual String ToString() override
	{
		return m_path;
	}

	Event1<int>& get_Changed()
	{
		return Changed;
	}

	Event1<int> Changed;
	String m_path;

	static Directory* get_Current();

private:

	static Directory s_Current;
};

extern GUIEXT IXAudio2* pXAudio2;
extern GUIEXT IXAudio2MasteringVoice* pMasteringVoice;

class WaveformVisual;

}	// Gui

class CoString
{
public:

	CTOR CoString() : m_wstr(nullptr)
	{
	}

	CTOR CoString(CoString&& other)
	{
		m_wstr = other.m_wstr;
		other.m_wstr = nullptr;
	}

	/*
	CTOR CoString(const CoString& other)
	{
		size_t len = wcslen(other.m_wstr);
		CoTaskMemAlloc(len*2+2);

		memcpy(
		m_wstr = other.m_wstr;
		other.m_wstr = nullptr;
	}
	*/

	~CoString()
	{
		CoTaskMemFree(m_wstr);
		m_wstr = nullptr;
	}

	CoString& operator = (CoString&& other)
	{
		m_wstr = other.m_wstr;
		other.m_wstr = nullptr;
	}

	operator LPWSTR () const throw()
	{
		return m_wstr;
	}

	LPWSTR* operator & ()
	{
		ASSERT(m_wstr == nullptr);
		return &m_wstr;
	}

	LPWSTR data() const throw()
	{
		return m_wstr;
	}

	size_t len() const throw()
	{
		return wcslen(m_wstr);
	}

	size_t GetLength() const throw()
	{
		return wcslen(m_wstr);
	}

	void Attach(LPWSTR wstr) throw()
	{
		if (m_wstr != wstr)
		{
			CoTaskMemFree(m_wstr);
			m_wstr = wstr;
		}
	}

	LPWSTR Detach() throw()
	{
		LPWSTR wstr = m_wstr;
		m_wstr = nullptr;
		return wstr;
	}

	LPWSTR m_wstr;
};

class GUIEXT KnownFolders
{
public:

	static String GetPathOf(const KNOWNFOLDERID& folderid);

	static String get_Pictures()
	{
		return GetPathOf(FOLDERID_Pictures);
	}

	static String get_PublicPictures()
	{
		return GetPathOf(FOLDERID_PublicPictures);
	}

	static String get_Videos()
	{
		return GetPathOf(FOLDERID_Videos);
	}

	static String get_PublicVideos()
	{
		return GetPathOf(FOLDERID_PublicVideos);
	}

	static String get_Downloads()
	{
		return GetPathOf(FOLDERID_Downloads);
	}

	static String get_PublicDownloads()
	{
		return GetPathOf(FOLDERID_PublicDownloads);
	}

	static String get_Cookies()
	{
		return GetPathOf(FOLDERID_Cookies);
	}

	static String get_ControlPanelFolder()
	{
		return GetPathOf(FOLDERID_ControlPanelFolder);
	}

	static String get_Documents()
	{
		return GetPathOf(FOLDERID_Documents);
	}

	static String get_PublicDocuments()
	{
		return GetPathOf(FOLDERID_PublicDocuments);
	}

	static String get_Desktop()
	{
		return GetPathOf(FOLDERID_Desktop);
	}

	static String get_PublicDesktop()
	{
		return GetPathOf(FOLDERID_PublicDesktop);
	}

	static String get_History()
	{
		return GetPathOf(FOLDERID_History);
	}

	static String get_SendTo()
	{
		return GetPathOf(FOLDERID_SendTo);
	}

	static String get_StartMenu()
	{
		return GetPathOf(FOLDERID_StartMenu);
	}

	static String get_CommonStartMenu()
	{
		return GetPathOf(FOLDERID_CommonStartMenu);
	}

	static String get_Recent()
	{
		return GetPathOf(FOLDERID_Recent);
	}

	static String get_PrintersFolder()
	{
		return GetPathOf(FOLDERID_PrintersFolder);
	}

	static String get_RecycleBinFolder()
	{
		return GetPathOf(FOLDERID_RecycleBinFolder);
	}

	static String get_Templates()
	{
		return GetPathOf(FOLDERID_Templates);
	}

	static String get_CommonTemplates()
	{
		return GetPathOf(FOLDERID_CommonTemplates);
	}

	static String get_AdminTools()
	{
		return GetPathOf(FOLDERID_AdminTools);
	}

	static String get_CommonAdminTools()
	{
		return GetPathOf(FOLDERID_CommonAdminTools);
	}

	static String get_Startup()
	{
		return GetPathOf(FOLDERID_Startup);
	}

	static String get_CommonStartup()
	{
		return GetPathOf(FOLDERID_CommonStartup);
	}

	static String get_LocalAppData()
	{
		return GetPathOf(FOLDERID_LocalAppData);
	}

	static String get_System()
	{
		return GetPathOf(FOLDERID_System);
	}

	static String get_Windows()
	{
		return GetPathOf(FOLDERID_Windows);
	}

	static String get_ProgramFiles()
	{
		return GetPathOf(FOLDERID_ProgramFiles);
	}

	static String get_ProgramFilesCommon()
	{
		return GetPathOf(FOLDERID_ProgramFilesCommon);
	}

	static String get_ProgramFilesX64()
	{
		return GetPathOf(FOLDERID_ProgramFilesX64);
	}

	static String get_ProgramFilesX86()
	{
		return GetPathOf(FOLDERID_ProgramFilesX86);
	}

private:
	CTOR KnownFolders();
};

}	// System

#include "WaveformData.h"

namespace System
{
namespace Gui
{

class GUIEXT AudioSourceVoice : public DependencyObject
{
public:

	CTOR AudioSourceVoice();
	CTOR AudioSourceVoice(WaveformData* waveformdata);

	void Play();
	void Create();

	DEPENDENCY_PROPERTY_RW(bool, IsPlaying)
	DEPENDENCY_PROPERTY_RW(float, Volume)
	DEPENDENCY_PROPERTY_RW(float, FrequencyRatio)
	DEPENDENCY_PROPERTY_RO(int64, SamplesPlayed)

	DEPENDENCY_CLASS(AudioSourceVoice, DependencyObject)

	IXAudio2SourceVoice* get_Voice()
	{
		return m_pSourceVoice;
	}

	WaveformData* get_Wave()
	{
		return m_wave;
	}

protected:

	friend void CALLBACK AudTimerProc(HWND, UINT, UINT_PTR, DWORD);

	IXAudio2SourceVoice* m_pSourceVoice;
	//WAVEFORMATEX m_wfx;
	WaveformData* m_wave;
};

}	// Gui
}	// System

#include "WaveformVisual.h"

namespace System
{
namespace Gui
{

class GUIEXT GraphAxisVisual : public UIElement
{
public:
	CTOR GraphAxisVisual();

	virtual void Render(ManagedRenderContext renderContext) override;

	virtual void OnMouseLeftButtonDown(MouseButtonEventArgs* args) override;
	virtual void OnMouseLeftButtonUp(MouseButtonEventArgs* args) override;
	virtual void OnMouseMove(MouseEventArgs* args) override;

	/*
	DEPENDENCY_PROPERTY(WaveformData*, Data)
	DEPENDENCY_PROPERTY(int64, AtSample)
	DEPENDENCY_PROPERTY(int64, SelectionStart)
	DEPENDENCY_PROPERTY(int64, SelectionEnd)
	*/

	vector<Color*> m_colors;
	vector<Vector2_F*> m_points;

	Point m_oldpoint;

	static DependencyClass* get_Class();
	static DependencyClass* pClass;
	typedef UIElement baseClass;
};

template<class T>
inline IBinding* typed<T>::binding(DependencyObject* _this) const
{
	if (m_exp)
		return new ExpressionBinding(_this, m_exp);
	else if (m_binding)
		return m_binding;
	else
		return NULL;
}

template<class T>
void SetBinding(DependencyObject* depObj, DependencyProperty* dp, typed<T> value)
{
	if (value.hasBinding())
	{
		depObj->GetProperty(dp)->SetInputBinding(value.binding(depObj));
	}
	else
	{
		depObj->GetProperty(dp)->SetLocalValue(value.m_const_value);
	}
}

#ifndef __LERSTAD__
template class GUIEXT typed<int>;
template class GUIEXT typed<float>;
template class GUIEXT typed<UIElement*>;
#endif

// TODO, make a member of some class
//Object* Create(Refs2* refs, Window* ownerWindow, UIElement* templatedOwner, Object* object);
UIElement* CreateObjectShadowTree(Object* object);

DependencyObject* GetTemplatedOwner(DependencyObject* depObject);

GUIEXT int MessageLoop();

GUIEXT void InitPhysics();
extern GUIEXT physx::PxFoundation* gPhysicsFoundation;
extern GUIEXT physx::PxPhysics* gPhysics;

// TODO: somewhere else
GUIEXT bool SaveBMP(Graphics::Bitmap* bitmap, StringIn filename);
GUIEXT bool SaveBMP(Graphics::Bitmap* bitmap, IO::Stream* stream);

GUIEXT Window* Show(Geometry* pgeometry);
GUIEXT Window* Show(Visual* visual);

GUIEXT String DefaultPrinter();
GUIEXT bool Print(Visual* visual);

class GUIEXT MessageWindow
{
public:

	static void Init();
	static HWND GetHwnd();

private:

	CTOR MessageWindow();

	static HWND m_hWnd;
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

GUIEXT Window* View(List<String>* pList);

GUIEXT void MakeScrollable(UIElement* element);

class GUIEXT MetafileLoader : public Graphics::Loader2
{
public:

	CTOR MetafileLoader();

	virtual void FileType() override;
//	virtual void SetMapMode(int mapmode) override;
//	virtual void SetWindowOrg(int x, int y) override;
//	virtual void SetWindowExt(int cx, int cy) override;
//	virtual void SetViewportOrg(int x, int y) override;
//	virtual void SetViewportExt(int cx, int cy) override;
//	virtual void SetTextColor(COLORREF color) override;
//	virtual void SetTextAlign(uint32 textAlign) override;
//	virtual void SetMiterLimit(float miterLimit) override;
	virtual void MoveTo(int x, int y) override;
	virtual void ExtTextOut(int x, int y, StringIn string, const int* distances) override;
	virtual void SelectObject(int n) override;
	virtual void DeleteObject(int n) override;
	virtual void CreatePenIndirect(int index, const Graphics::LOGPEN& lp) override;
	virtual void CreatePenIndirect(int index, const Graphics::EXTLOGPEN& lp) override;
	virtual void CreateBrushIndirect(int index, const Graphics::LOGBRUSH& lb) override;
	virtual void CreateFontIndirect(int index, const Graphics::LOGFONT_16& lf) override;
	virtual void CreateFontIndirect(int index, const Graphics::LOGFONTA& lf) override;
	virtual void CreateFontIndirect(int index, const Graphics::LOGFONTW& lf) override;
	virtual void StretchDIBits(int destX, int destY, int destWidth, int destHeight, int srcX, int srcY, int srcWidth, int srcHeight, Graphics::Bitmap* bitmap) override;
	virtual void FillPath() override;

	Visuals* pCommands;

protected:

	class ObjectPen : public Graphics::wmfObject
	{
	public:
		CTOR ObjectPen()
		{
			m_type = Graphics::wmfOBJ_PEN;
		}

	//	LDraw::Pen* m_pPen;
		Brush* m_brush;
		float m_width;
	};

	class ObjectBrush : public Graphics::wmfObject
	{
	public:
		CTOR ObjectBrush()
		{
			m_type = Graphics::wmfOBJ_BRUSH;
		}
		Brush* m_pBrush;
	};

	class ObjectFont : public Graphics::wmfObject
	{
	public:
		CTOR ObjectFont()
		{
			m_type = Graphics::wmfOBJ_FONT;
		}
	#if 0
		LDraw::Font* m_pFont;
	#endif
	};

	PathGeometry* m_path;

	ObjectPen* m_pCurPen;
	ObjectBrush* m_pCurBrush;
	ObjectFont* m_pCurFont;
};

GUIEXT void print_(Visual* visual);

}	// Gui

GUIEXT void AddView(Object* obj, Object* view);

GUIEXT Object* GetSource(Object* obj);
GUIEXT Object* GetView(Object* obj);
GUIEXT IEnumerator<Object*>* GetViews(Object* obj);

GUIEXT Object* GetFile(StringIn name);
GUIEXT void SaveAs(Object* obj, StringIn name);

GUIEXT void Show(Graphics::Bitmap* bitmap);

GUIEXT TextDocument* ascpp(ClassType* pType);

GUIEXT Gui::Window* Edit(Gui::Vector2_F* vec);

GUIEXT int OSVersion();

class GUIEXT Math
{
public:

	static float cos(float x)
	{
		return ::cosf(x);
	}

	static float sin(float x)
	{
		return ::sinf(x);
	}

	static float acos(float x)
	{
		return ::acosf(x);
	}

	static float asin(float x)
	{
		return ::asinf(x);
	}

	static float tan(float x)
	{
		return ::tanf(x);
	}

	static float atan(float x)
	{
		return ::atanf(x);
	}

//	const double PI = 3.14159265358979323846;
//	const double SQRT2 = 1.4142135623730950488016887242097;

//	static const float PI = 3.141592653f;
//	static const float SQRT2 = 1.4142135623f;

private:

	CTOR Math();
};

}	// System

namespace System
{
namespace Gui
{

class GUIEXT VidCap : public Object
{
public:

	CTOR VidCap();

	void Capture();

	ComPtr<IGraphBuilder> m_pGraph;
	ComPtr<ICaptureGraphBuilder2> m_pBuild;
};

}
}

#include "MLTranslate.h"

#endif	// GUI_h
