#ifndef Web_XML_h
#define Web_XML_h

#ifndef WEBEXT
#define WEBEXT DECLSPEC_DLLIMPORT
#endif

// TODO; UICore
#ifdef __LERSTAD__
#import "GUI/GUI.h"
#else
#include "GUI/GUI.h"
#endif

#ifdef __LERSTAD__
#import "CSS/CSS.h"
#else
#include "CSS/CSS.h"
#endif

#ifdef __LERSTAD__
#import "XPath/XPath.h"
#else
#include "XPath/XPath.h"
#endif

#ifdef __LERSTAD__
#import "XML/XML.h"
#else
#include "XML/XML.h"
#endif

#ifdef __LERSTAD__
#import "JSVM/JSVM.h"
#else
#include "JSVM/JSVM.h"
#endif

#ifdef __LERSTAD__
#import "EcmaScript/EcmaScript.h"
#else
#include "EcmaScript/EcmaScript.h"
#endif

#ifdef __LERSTAD__
#import "Networking/Networking.h"
#else
#include "Networking/Networking.h"
#endif

namespace System
{
namespace Web
{

// Forward Declarations

interface IASModel;
interface IASNamedObjectMap;
interface IASDataTypeDeclaration;
interface IASAttributeDeclaration;

interface ISVGShadowElement;	// ??

interface INodeIterator;
interface INodeFilter;
interface IDOMStringList;
interface IDOMNameList;
interface IElementEditVAL;

interface IKeySpline;

interface IAbstractView;
interface IViewCSS;
interface IWindow;

interface ICSSStyleDeclaration;

interface ISVGAnimatedString;

interface IASObject;
interface IASObjectList;
interface IASContentModel;
interface IASDeclaration;
interface IASAttributeGroupDeclaration;

class Node;
class ChildNode;
class Attr;
class Entity;
class Element;
class Document;
class NodeList;
class NamedNodeMap;
class Event;
class EventTarget;
class Range;

class SMILTime;
class SMILTimeList;
class TimeInterval;
class ElementTime;
class ElementAnimation;
class ElementAnimate;
class ElementSet;

class AbstractView;
class CHTMLWindow;	// TODO rename to Window

class MediaList;

class CSSValue;
class CSSPrimitiveValue;
class CSSRect;
class RGBColor;
class CSSValueList;
class SVGColor;
class SVGICCColor;
class SVGPaint;
class CSSStyleDeclaration;

class CATXMLAttr;	// TODO remove

class SVGMatrix;
class SVGPoint;
class SVGPointList;
class SVGPointListMutable;
class SVGNumberList;

class PCharacterData;
class PText;
class PElementBase;
class CXMLDocumentView;
class CHTMPCharacterBox;
class CHTMFlowInlineContext;

WEBEXT Element* GetElementByTagName(Node* parentNode, StringIn tagName, bool bDeep = true);
WEBEXT Element* GetElementByTagNameNS(Node* parentNode, StringIn ns, StringIn tagName, bool bDeep = true);
WEBEXT NodeList* GetElementsByTagNameNS(Node* parentNode, StringIn namespaceURI, StringIn localName);

}	// Web

interface ISingleObjectListener;	// TODO remove

enum NotifyType
{
	NOTIFY_UNKNOWN	= 0,
	NOTIFY_ADD	= 1,
	NOTIFY_REMOVE	= 2,
	NOTIFY_MODIFY	= 3
};

interface INotifyGet
{
	virtual int OnBeforeChange( 
		/* [in] */ NotifyType type,
		/* [in] */ System::Object* targetObject,
		/* [in] */ System::Object* immtargetObject,
		/* [in] */ long dispID) = 0;

	virtual int OnChanged( 
		/* [in] */ NotifyType type,
		/* [in] */ System::Object* targetObject,
		/* [in] */ System::Object* immtargetObject,
		/* [in] */ long dispID) = 0;

};

// TODO remove
interface INotifySend
{
	virtual int Advise( 
		/* [in] */ INotifyGet *get,
		/* [retval][out] */ ULONG_PTR *cookie) = 0;

	virtual int Unadvise( 
		/* [in] */ INotifyGet *get) = 0;

	virtual int LockSend( void) = 0;

	virtual int UnlockSend( void) = 0;

	virtual int FireOnChanged( 
		/* [in] */ NotifyType type,
		/* [in] */ System::Object *targetObject,
		/* [in] */ long dispID) = 0;

};

// TODO remove
class WEBEXT CSingleObject : public System::Object
{
public:
	CTOR CSingleObject()
	{
		m_pListener = NULL;
	}

	~CSingleObject()
	{
		ASSERT(m_pListener == NULL);
	}

	ISingleObjectListener* m_pListener;
};

// TODO remove
interface ISingleObjectListener
{
	virtual void OnChanged(CSingleObject* pObject) = 0;
};

// TODO remove
template <class T> class CNotifyGetImpl/* :
	public INotifyGet*/
{
public:

	char m_test;

	int OnBeforeChange(NotifyType type, System::Object* targetObject, System::Object* immtargetObject, long dispID)
	{
		return Success;
	}

	/*
	int OnChanged(NotifyType type, IUnknown* targetObject, GUID targetObjectIID, DISPID dispID);
	*/

	/*
	int OnChanged(DISPID dispID)
	{
		return S_OK;
	}
	*/
};

// TODO remove
template <class T> class CNotifySendImpl/* :
	public INotifySend*/
{
public:

	vector<INotifyGet*> m_get;
	long m_lockCount;
	int m_pendingNotifies;

	int m_fireReentrance;

	CTOR CNotifySendImpl()
	{
		m_lockCount = 0;
		m_pendingNotifies = 0;

		m_fireReentrance = 0;
	}

	~CNotifySendImpl()
	{
		ASSERT(m_get.GetSize() == 0);
		ASSERT(m_lockCount == 0);
	}

	int Advise(INotifyGet* get, ULONG_PTR* cookie)
	{
	// Check that we're not trying to advise twice
		for (unsigned int i = 0; i < m_get.GetSize(); i++)
		{
			if (m_get[i] == get)
			{
				ASSERT(0);
			}
		}

		*cookie = m_get.Add(get)+1;

		return Success;
	}

	int Unadvise(INotifyGet* get)
	{
		ASSERT(get != NULL);

#if 0
		for (int i = 0; i < m_get.GetSize(); i++)
		{
			if (IsUnknownEqualUnknown(m_get[i], get))
			{
				m_get.RemoveAt(i);
				return S_OK;
			}
		}
#endif

		ASSERT(0);
		return Success;
	}

	int LockSend()
	{
		m_lockCount++;
		return Success;
	}

	int UnlockSend()
	{
		m_lockCount--;
		ASSERT(m_lockCount >= 0);

		if (m_lockCount == 0)
		{
			if (m_pendingNotifies)
			{
				ASSERT(0);
				//FireOnChanged((NotifyType)0, static_cast<T*>(this)->GetUnknown()/*NULL*/, /*static_cast<T*>(this)->GetUnknown(),*/ DISPID_UNKNOWN);
				m_pendingNotifies = 0;
			}
		}

		return Success;
	}

	void FireOnBeforeChange(NotifyType type, System::Object* targetObject, System::Object* immtargetObject, long dispID)
	{
		if (m_lockCount == 0)
		{
			size_t nConnections = m_get.GetSize();

			for (size_t i = 0; i < nConnections; i++)
			{
				m_get[i]->OnBeforeChange(type, targetObject, immtargetObject, dispID);

				ASSERT(m_get.size() == nConnections);
			}
		}
		else
		{
			m_pendingNotifies++;
		}
	}

	int FireOnChanged(NotifyType type, System::Object* targetObject, /*IUnknown* immtargetObject,*/ long dispID)
	{
		//if (m_fireReentrance == 0)
		{
			m_fireReentrance++;

			if (m_lockCount == 0)
			{
				size_t nConnections = m_get.GetSize();

				for (size_t i = 0; i < nConnections; i++)
				{
					ASSERT(0);
				//	m_get[i]->OnChanged(type, targetObject, static_cast<T*>(this)->GetUnknown()/*immtargetObject*/, dispID);

					ASSERT(m_get.GetSize() == nConnections);
				}
			}
			else
			{
				m_pendingNotifies++;
			}

			m_fireReentrance--;
		}

		return Success;
	}
};

} // System

template <class Type> class CreateInstanceT
{
public:

	CTOR CreateInstanceT()
	{
		ASSERT(0);
		//m_value = new Type;
	}

	~CreateInstanceT()
	{
		ASSERT(0);
		//delete m_value;
	}

	Type* m_value;

	operator Type* ()
	{
		return m_value;
	}

	Type* GetValue()
	{
		return m_value;
	}

	Type* operator -> () const
	{
		ASSERT(m_value != NULL);
		return m_value;
	}
};

namespace System
{
namespace Web
{

//typedef String DOMString;	// TODO remove
typedef uint64 DOMTimeStamp;

/*
class WEBEXT SVGPointBase : public Object
{
public:
	virtual double get_x() const = 0;
	virtual void set_x(double x) = 0;

	virtual double get_y() const = 0;
	virtual void set_y(double y) = 0;
};
*/

/*
interface IDocumentStyle
{
	virtual Style::IStyleSheetList* get_styleSheets() = 0;
};
*/

interface IDocumentCSS : public Style::IDocumentStyle
{
	virtual Style::ICSSStyleDeclaration* getOverrideStyle(Element* elt, StringIn pseudoElt) = 0;
};

interface ISVGPoint
{
	virtual double get_x() const = 0;
	virtual void set_x(double x) = 0;

	virtual double get_y() const = 0;
	virtual void set_y(double y) = 0;
};

interface ISVGRect
{
	virtual double get_x() const = 0;
	virtual double get_y() const = 0;
	virtual double get_width() const = 0;
	virtual double get_height() const = 0;
};

enum SVGAngleType
{
	SVG_ANGLETYPE_UNKNOWN = 0,
	SVG_ANGLETYPE_UNSPECIFIED = 1,
	SVG_ANGLETYPE_DEG	= 2,
	SVG_ANGLETYPE_RAD	= 3,
	SVG_ANGLETYPE_GRAD = 4
};

interface ISVGNumber
{
	virtual double get_value() const = 0;
	virtual void set_value(double newVal) = 0;
};

interface ISVGList
{
	virtual unsigned int get_numberOfItems() const = 0;
	virtual void clear() = 0;
};

interface ISVGNumberList : public ISVGList
{
	virtual ISVGNumber* initialize(ISVGNumber* newItem) = 0;
	virtual ISVGNumber* getItem(unsigned int index) = 0;
	virtual ISVGNumber* insertItemBefore(ISVGNumber* newItem, unsigned int index) = 0;
	virtual ISVGNumber* replaceItem(ISVGNumber* newItem, unsigned int index) = 0;
	virtual ISVGNumber* removeItem(unsigned int index) = 0;
	virtual ISVGNumber* appendItem(ISVGNumber* newItem) = 0;
};

interface ISVGPointList : public ISVGList
{
	virtual SVGPoint* appendItem(SVGPoint* newItem) = 0;
	virtual SVGPoint* removeItem(unsigned int index) = 0;
	virtual SVGPoint* replaceItem(SVGPoint* newItem, unsigned int index) = 0;
	virtual SVGPoint* insertItemBefore(SVGPoint* newItem, unsigned int index) = 0;
	virtual SVGPoint* getItem(unsigned int index) const = 0;
	virtual SVGPoint* initialize(SVGPoint* newItem) = 0;
};

interface IEventListener
{
	virtual void handleEvent(Event* evt) abstract;
};

interface IEventListenerGroup
{
	virtual bool isSameEventListenerGroup(IEventListenerGroup* other) = 0;
};

interface IEmbeddingElement
{
	virtual Document* get_contentDocument() = 0;
	virtual IWindow* get_contentWindow() = 0;
};

#if 0
    
    class IStyleSheet
    {
    public:
        virtual StringW get_type() = 0;
        
        virtual /* [helpstring][id][propget] */ ErrorCode  get_disabled( 
            /* [retval][out] */ bool *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ ErrorCode  set_disabled( 
            /* [in] */ bool newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ ErrorCode  get_parentStyleSheet( 
            /* [retval][out] */ IStyleSheet **pVal) = 0;
        
        virtual StringW get_href() = 0;
        
        virtual StringW get_title() = 0;
        
        virtual void set_title(const StringW& newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ ErrorCode  get_media( 
            /* [retval][out] */ MediaList **pVal) = 0;
    };
    
    class IStyleSheetList
    {
    public:
        virtual long get_length() = 0;
        
        virtual /* [helpstring][id] */ ErrorCode  item( 
            /* [in] */ long index,
            /* [retval][out] */ IStyleSheet **pVal) = 0;
        
        virtual /* [helpstring][id] */ ErrorCode  append( 
            /* [in] */ IStyleSheet *item) = 0;
        
    };
#endif

enum CSSValueType
{
	CSS_INHERIT = 0,
	CSS_PRIMITIVE_VALUE = 1,
	CSS_VALUE_LIST = 2,
	CSS_CUSTOM = 3
};

enum CSSUnitType
{
	CSS_UNKNOWN = 0,
	CSS_NUMBER = 1,
	CSS_PERCENTAGE = 2,
	CSS_EMS = 3,
	CSS_EXS = 4,
	CSS_PX = 5,
	CSS_CM = 6,
	CSS_MM = 7,
	CSS_IN = 8,
	CSS_PT = 9,
	CSS_PC = 10,
	CSS_DEG = 11,
	CSS_RAD = 12,
	CSS_GRAD = 13,
	CSS_MS = 14,
	CSS_S = 15,
	CSS_HZ = 16,
	CSS_KHZ = 17,
	CSS_DIMENSION = 18,
	CSS_STRING = 19,
	CSS_URI = 20,
	CSS_IDENT = 21,
	CSS_ATTR = 22,
	CSS_COUNTER = 23,
	CSS_RECT = 24,
	CSS_RGBCOLOR = 25
};

/*
interface ICSSStyleDeclarationHandler
{
	virtual ErrorCode handleCSSEvent(int type, System::StringW* propertyName) = 0;
};

enum CSSObjectType
{
	CSSObject_Rule	= 1,
	CSSObject_Selector	= 2,
	CSSObject_Property	= 3
};

enum CSSEventType
{
	CSSEvent_Insert	= 1,
	CSSEvent_Remove	= 2,
	CSSEvent_Modify	= 3
};

interface ICSSStyleSheetHandler
{
	virtual ErrorCode handleCSSEvent( 
      CSSObjectType objectType,
      CSSEventType eventType,
      Web::CSSStyleSheet* styleSheet,
      Object* pObject) = 0;
		
};
*/
    

#if 0
    __INTERFACE("58BA7BB7-3132-4C5D-84A3-475F6F5C0351")
    ILCSSValue
    {
    public:
        virtual /* [helpstring][propget] */ ErrorCode  get_cssText( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][propput] */ ErrorCode  set_cssText( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][propget] */ ErrorCode  get_cssValueType( 
            /* [retval][out] */ CSSValueType *pVal) = 0;
        
    };
    


    
    __INTERFACE("0955399C-11A6-4c19-BA4A-395211E6368C")
    ILCSSRect
    {
    public:
        virtual /* [helpstring][propget] */ ErrorCode  get_top( 
            /* [retval][out] */ ILCSSPrimitiveValue **pVal) = 0;
        
        virtual /* [helpstring][propget] */ ErrorCode  get_right( 
            /* [retval][out] */ ILCSSPrimitiveValue **pVal) = 0;
        
        virtual /* [helpstring][propget] */ ErrorCode  get_bottom( 
            /* [retval][out] */ ILCSSPrimitiveValue **pVal) = 0;
        
        virtual /* [helpstring][propget] */ ErrorCode  get_left( 
            /* [retval][out] */ ILCSSPrimitiveValue **pVal) = 0;
        
    };
    
#endif

	 /*
    class ICSSCounter
    {
    public:
		 virtual System::StringW* get_identifier() = 0;
        virtual System::StringW* get_listStyle() = 0;
        virtual System::StringW* get_separator() = 0;
    };
	 */

#if 0
    __INTERFACE("FE244BDE-8968-4FD6-9E65-2A371173AD6F")
    ILCSSPrimitiveValue : public ILCSSValue
    {
    public:
        virtual /* [helpstring][propget] */ ErrorCode  get_primitiveType( 
            /* [retval][out] */ CSSUnitType *pVal) = 0;
        
        virtual /* [helpstring] */ ErrorCode  setFloatValue( 
            /* [in] */ CSSUnitType unitType,
            /* [in] */ double floatValue) = 0;
        
        virtual /* [helpstring] */ ErrorCode  getFloatValue( 
            /* [in] */ CSSUnitType unitType,
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring] */ ErrorCode  setStringValue( 
            /* [in] */ CSSUnitType stringType,
            /* [in] */ BSTR stringValue) = 0;
        
        virtual /* [helpstring] */ ErrorCode  getStringValue( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring] */ ErrorCode  getRGBColorValue( 
            /* [retval][out] */ ILRGBColor **pVal) = 0;
        
        virtual /* [helpstring] */ ErrorCode  getRectValue( 
            /* [retval][out] */ ILCSSRect **pVal) = 0;
        
        virtual /* [helpstring] */ ErrorCode  getCounterValue( 
            /* [retval][out] */ ICSSCounter **pVal) = 0;
        
    };

    __INTERFACE("D3463021-23EE-11d7-9444-00E07DC3EF52")
    ILCSSValueList : public ILCSSValue
    {
    public:
        virtual /* [helpstring][propget] */ ErrorCode  get_length( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring] */ ErrorCode  item( 
            /* [in] */ long index,
            /* [retval][out] */ ILCSSValue **pVal) = 0;
        
        virtual /* [helpstring] */ ErrorCode  insertValue( 
            const StringW& value,
            /* [in] */ long index,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring] */ ErrorCode  deleteValue( 
            /* [in] */ long index) = 0;
        
    };
#endif

enum SVGColorType
{
	SVG_COLORTYPE_UNKNOWN = 0,
	SVG_COLORTYPE_RGBCOLOR = 1,
	SVG_COLORTYPE_RGBCOLOR_ICCCOLOR = 2,
	SVG_COLORTYPE_CURRENTCOLOR = 3
};

#if 0
    __INTERFACE("841EDDFD-4BB2-434F-B288-BD3443EB631C")
    ILSVGColor : public ILCSSValue
    {
    public:
        virtual /* [helpstring][propget] */ ErrorCode  get_colorType( 
            /* [retval][out] */ SVGColorType *pVal) = 0;
        
        virtual /* [helpstring][propget] */ ErrorCode  get_rgbColor( 
            /* [retval][out] */ ILRGBColor **pVal) = 0;
        
        virtual /* [helpstring][propget] */ ErrorCode  get_iccColor( 
            /* [retval][out] */ ILSVGICCColor **pVal) = 0;
        
        virtual /* [helpstring] */ ErrorCode  setRGBColor( 
            /* [in] */ BSTR rgbColor) = 0;
        
        virtual /* [helpstring] */ ErrorCode  setRGBColorICCColor( 
            /* [in] */ BSTR rgbColor,
            /* [in] */ BSTR iccColor) = 0;
        
        virtual /* [helpstring] */ ErrorCode  setColor( 
            /* [in] */ LSVGColorType colorType,
            /* [in] */ BSTR rgbColor,
            /* [in] */ BSTR iccColor) = 0;
        
    };
#endif

enum SVGPaintType
{
	SVG_PAINTTYPE_UNKNOWN = 0,
	SVG_PAINTTYPE_RGBCOLOR = 1,
	SVG_PAINTTYPE_RGBCOLOR_ICCCOLOR	= 2,
	SVG_PAINTTYPE_NONE = 101,
	SVG_PAINTTYPE_CURRENTCOLOR = 102,
	SVG_PAINTTYPE_URI_NONE = 103,
	SVG_PAINTTYPE_URI_CURRENTCOLOR = 104,
	SVG_PAINTTYPE_URI_RGBCOLOR	= 105,
	SVG_PAINTTYPE_URI_RGBCOLOR_ICCCOLOR	= 106,
	SVG_PAINTTYPE_URI = 107
};

enum SVGRenderingIntent
{
	RENDERING_INTENT_UNKNOWN = 0,
	RENDERING_INTENT_AUTO = 1,
	RENDERING_INTENT_PERCEPTUAL = 2,
	RENDERING_INTENT_RELATIVE_COLORIMETRIC = 3,
	RENDERING_INTENT_SATURATION = 4,
	RENDERING_INTENT_ABSOLUTE_COLORIMETRIC = 5
};

interface ICSSStyleDeclaration : public Style::IStyleDeclaration
{
#if 0
	virtual unsigned int get_length() const = 0;
	virtual String item(unsigned int index) const = 0;

	virtual String get_cssText() = 0;
	virtual void set_cssText(StringIn cssText) = 0;

	virtual String getPropertyValue(StringIn propertyName) = 0;
	virtual CSSValue* getPropertyCSSValue(StringIn propertyName) = 0;
#endif 

#if 0
        virtual /* [helpstring][propget] */ ErrorCode  get_parentRule( 
            /* [retval][out] */ ILCSSRule **pVal) = 0;
        
        virtual /* [helpstring][propget] */ ErrorCode  get_cssText( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][propput] */ ErrorCode  set_cssText( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring] */ ErrorCode  getPropertyValue( 
            /* [in] */ BSTR propertyName,
            /* [retval][out] */ BSTR *pValue) = 0;
        
        virtual /* [helpstring] */ ErrorCode  removeProperty( 
            /* [in] */ BSTR propertyName,
            /* [retval][out] */ BSTR *pValue) = 0;
        
        virtual /* [helpstring] */ ErrorCode  setProperty( 
            /* [in] */ BSTR propertyName,
            /* [in] */ BSTR value,
            /* [in] */ BSTR priority) = 0;
        
        virtual /* [helpstring] */ ErrorCode  getPropertyCSSValue( 
            /* [in] */ BSTR propertyName,
            /* [retval][out] */ ILCSSValue **ppResult) = 0;
        
        virtual /* [helpstring] */ ErrorCode  getPropertyPriority( 
            /* [in] */ BSTR propertyName,
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring] */ ErrorCode  addHandler( 
            /* [in] */ ICSSStyleDeclarationHandler *handler) = 0;
        
        virtual /* [helpstring] */ ErrorCode  removeHandler( 
            /* [in] */ ICSSStyleDeclarationHandler *handler) = 0;
#endif
};

interface ICSS2Properties
{
#if 0
        virtual /* [helpstring][propget] */ ErrorCode  get_azimuth( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][propput] */ ErrorCode  set_azimuth( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][propget] */ ErrorCode  get_background( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][propput] */ ErrorCode  set_background( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][propget] */ ErrorCode  get_backgroundAttachment( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][propput] */ ErrorCode  set_backgroundAttachment( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][propget] */ ErrorCode  get_backgroundColor( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][propput] */ ErrorCode  set_backgroundColor( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][propget] */ ErrorCode  get_backgroundImage( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][propput] */ ErrorCode  set_backgroundImage( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][propget] */ ErrorCode  get_backgroundPosition( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][propput] */ ErrorCode  set_backgroundPosition( 
            /* [in] */ BSTR newVal) = 0;
#endif
};

#if 0
    class ILCSSSelectorList
    {
    public:
#if 0
        virtual /* [helpstring][propget] */ ErrorCode  get_selectorText( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][propput] */ ErrorCode  set_selectorText( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][propget] */ ErrorCode  get_length( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring] */ ErrorCode  item( 
            /* [in] */ long index,
            /* [retval][out] */ ILCSSSelector **pVal) = 0;
        
        virtual /* [helpstring] */ ErrorCode  deleteItem( 
            /* [in] */ long index) = 0;
        
        virtual /* [helpstring] */ ErrorCode  appendItem( 
            /* [in] */ BSTR selectorText,
            /* [retval][out] */ long *pVal) = 0;
#endif  
};
#endif

interface IElementCSSInlineStyle
{
	virtual ICSSStyleDeclaration* get_style() = 0;
};

#if 0
interface IDocumentStyle
{
  virtual /* [helpstring][propget] */ ErrorCode  get_styleSheets( 
      /* [retval][out] */ IStyleSheetList **pVal) = 0;
  
  virtual /* [helpstring] */ ErrorCode  removeStyleSheet( 
      /* [in] */ IStyleSheet *styleSheet) = 0;
  
};
#endif
    
   
class ISMILAttributeValue
{
public:
#if 0
  virtual /* [helpstring] */ ErrorCode  getStringValue( 
      /* [retval][out] */ BSTR *pVal) = 0;
  
  virtual /* [helpstring] */ ErrorCode  setStringValue( 
      /* [in] */ BSTR newVal) = 0;
  
  virtual /* [helpstring] */ ErrorCode  getObject( 
      /* [retval][out] */ Object **pVal) = 0;
#endif   
};

interface ISMILXMLAttr
{
#if 0
  virtual /* [helpstring][propget] */ ErrorCode  get_name( 
      /* [retval][out] */ BSTR *pVal) = 0;
  
  virtual /* [helpstring][propget] */ ErrorCode  get_namespaceURI( 
      /* [retval][out] */ BSTR *pVal) = 0;
#endif   
};

interface ISVGAnimatedString
{
	virtual String get_animVal() = 0;
	virtual String get_baseVal() = 0;
	virtual void set_baseVal(StringIn newVal) = 0;
};

enum TimeType
{
	SMIL_TIME_INDEFINITE = 0,
	SMIL_TIME_OFFSET = 1,
	SMIL_TIME_SYNC_BASED = 2,
	SMIL_TIME_EVENT_BASED = 3,
	SMIL_TIME_WALLCLOCK = 4,
	SMIL_TIME_MEDIA_MARKER = 5
};

enum FillType
{
	FILL_REMOVE	= 0,
	FILL_FREEZE	= 1,
	FILL_HOLD = 2,
	FILL_TRANSITION = 3,
	FILL_AUTO = 4,
	FILL_DEFAULT = 5
};

enum RestartType
{
	RESTART_ALWAYS	= 0,
	RESTART_NEVER = 1,
	RESTART_WHEN_NOT_ACTIVE	= 2,
	RESTART_DEFAULT = 3
};

#if 0
    class ITimeInstance
    {
    public:
        virtual /* [helpstring][propget] */ ErrorCode  get_time( 
            /* [retval][out] */ ISMILTime **pVal) = 0;
        
    };
    
    class ITimeInterval
    {
    public:
        virtual /* [helpstring][propget] */ ErrorCode  get_beginInstanceTime( 
            /* [retval][out] */ ILTimeInstance **pVal) = 0;
        
        virtual /* [helpstring][propget] */ ErrorCode  get_resolvedBegin( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][propget] */ ErrorCode  get_resolvedEnd( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring] */ ErrorCode  getNextInterval( 
            /* [retval][out] */ ITimeInterval **pVal) = 0;
        
    };
    
#endif

interface IElementTime
{
	virtual FillType get_fill() = 0;
		
	virtual ErrorCode  set_fill(FillType newVal) = 0;
	
	virtual double get_repeatCount() = 0;
	
	virtual ErrorCode  set_repeatCount(double newVal) = 0;
	
	virtual double get_repeatDur() = 0;
	
	virtual ErrorCode  set_repeatDur(double newVal) = 0;
	
	virtual RestartType get_restart() = 0;
	
	virtual ErrorCode  set_restart(RestartType newVal) = 0;
	
	virtual ErrorCode  get_dur(double *pVal) = 0;
	
	virtual ErrorCode  set_dur( 
	/* [in] */ double newVal) = 0;
	
	virtual ErrorCode  get_begin(SMILTimeList **pVal) = 0;
	
	virtual ErrorCode  get_end(SMILTimeList **pVal) = 0;
	
	virtual ErrorCode  get_speed(double *pVal) = 0;
	
	virtual ErrorCode  set_speed(double newVal) = 0;
	
	virtual ErrorCode  get_accelerate(double *pVal) = 0;
	
	virtual /* [helpstring][propput] */ ErrorCode  set_accelerate( 
	/* [in] */ double newVal) = 0;
	
	virtual /* [helpstring][propget] */ ErrorCode  get_decelerate( 
	/* [retval][out] */ double *pVal) = 0;
	
	virtual /* [helpstring][propput] */ ErrorCode  set_decelerate( 
	/* [in] */ double newVal) = 0;
	
	virtual /* [helpstring][propget] */ ErrorCode  get_autoReverse( 
	/* [retval][out] */ bool *pVal) = 0;
	
	virtual /* [helpstring][propput] */ ErrorCode  set_autoReverse( 
	/* [in] */ bool newVal) = 0;
	
	virtual void seekElement(double seekTo) = 0;

	virtual /* [helpstring] */ ErrorCode  beginElement( 
	/* [retval][out] */ bool *pVal) = 0;
	
	virtual /* [helpstring] */ ErrorCode  beginElementAt( 
	/* [in] */ double offset,
	/* [retval][out] */ bool *pVal) = 0;
	
	virtual /* [helpstring] */ ErrorCode  endElement( 
	/* [retval][out] */ bool *pVal) = 0;
	
	virtual /* [helpstring] */ ErrorCode  endElementAt( 
	/* [in] */ double offset,
	/* [retval][out] */ bool *pVal) = 0;
	
	virtual /* [helpstring][propget] */ ErrorCode  get_currentInterval( 
	/* [retval][out] */ TimeInterval **pVal) = 0;
	
	virtual /* [helpstring][propget] */ ErrorCode  get_activeTime( 
	/* [retval][out] */ double *pVal) = 0;
	
	virtual /* [helpstring][propget] */ ErrorCode  get_simpleTime( 
	/* [retval][out] */ double *pVal) = 0;
	
	virtual /* [helpstring] */ ErrorCode  getSimpleDuration( 
	/* [retval][out] */ double *pVal) = 0;
	
	virtual /* [helpstring] */ ErrorCode  getActiveDuration( 
	/* [retval][out] */ double *pVal) = 0;
	
	virtual /* [helpstring] */ ErrorCode  ActiveTimeToSimpleTime( 
	/* [in] */ double tau,
	/* [retval][out] */ double *pVal) = 0;
	
	virtual /* [helpstring] */ ErrorCode  SimpleTimeToActiveTime( 
	/* [in] */ double tsf,
	/* [retval][out] */ double *pVal) = 0;
		
};

interface IElementTimeControl
{
	virtual bool beginElement() = 0;
	virtual bool beginElementAt(double offset) = 0;
	virtual bool endElement() = 0;
	virtual bool endElementAt(double offset) = 0;
};

interface IElementTimeContainer : public IElementTime
{
#if 0
  virtual /* [helpstring][propget] */ ErrorCode  get_timeContainer( 
      /* [retval][out] */ BSTR *pVal) = 0;
  
  virtual /* [helpstring][propput] */ ErrorCode  set_timeContainer( 
      /* [in] */ BSTR newVal) = 0;
  
  virtual /* [helpstring][propget] */ ErrorCode  get_endSync( 
      /* [retval][out] */ BSTR *pVal) = 0;
  
  virtual /* [helpstring][propput] */ ErrorCode  set_endSync( 
      /* [in] */ BSTR newVal) = 0;
#endif   
};

interface IElementSequentialTimeContainer : public IElementTimeContainer
{
};

interface IElementParallelTimeContainer : public IElementTimeContainer
{
};

interface IElementExclusiveTimeContainer : public IElementTimeContainer
{
};

enum CalcModeType
{
	CALCMODE_DISCRETE = 0,
	CALCMODE_LINEAR = 1,
	CALCMODE_PACED	= 2,
	CALCMODE_SPLINE = 3
};

enum AdditiveType
{
	ADDITIVE_REPLACE = 0,
	ADDITIVE_SUM = 1
};

enum AccumulateType
{
	ACCUMULATE_NONE = 0,
	ACCUMULATE_SUM	= 1
};

enum AttributeType
{
	ATTRIBUTE_TYPE_AUTO = 0,
	ATTRIBUTE_TYPE_CSS = 1,
	ATTRIBUTE_TYPE_XML = 2
};

interface ISMILAnimationTarget
{
	virtual NodeList* get_animationElements() = 0;
	virtual uint get_animatableXMLAttrsCount() = 0;
	virtual ISMILXMLAttr* animatableXMLAttrItem(uint index) = 0;
#if 0
  virtual /* [helpstring] */ ErrorCode  animatableXMLAttrItemByName( 
      /* [in] */ BSTR name,
      /* [retval][out] */ ILSMILXMLAttr **pVal) = 0;
#endif    
};

interface IAnimationElement : public IElementTime
{
	virtual Element* get_targetElement() = 0;
#if 0  
  virtual /* [helpstring][propget] */ ErrorCode  get_attributeName( 
      /* [retval][out] */ BSTR *pVal) = 0;
  
  virtual /* [helpstring][propput] */ ErrorCode  set_attributeName( 
      /* [in] */ BSTR newVal) = 0;
#endif   
	virtual AttributeType get_attributeType() = 0;
	virtual void set_attributeType(AttributeType newVal) = 0;
};
    
interface IElementAnimate : public IAnimationElement
{
	virtual AdditiveType get_additive() = 0;
	virtual void set_additive(AdditiveType newVal) = 0;
	virtual AccumulateType get_accumulate() = 0;
	virtual void set_accumulate(AccumulateType newVal) = 0;
	virtual CalcModeType get_calcMode() = 0;
	virtual void set_calcMode(CalcModeType newVal) = 0;
};

interface IAnimateXXX
{
	virtual ISMILAttributeValue* GetCurrentValue() = 0;
	virtual uint GetNValues() = 0;
	virtual ISMILAttributeValue* GetValue(uint index) = 0;

virtual /* [helpstring] */ ErrorCode  GetKeySpline( 
   /* [in] */ long index,
   /* [retval][out] */ IKeySpline **pVal) = 0;
#if 0  
virtual /* [helpstring] */ ErrorCode  InsertKeyTimeValue( 
   /* [in] */ double keyTime,
   /* [in] */ BSTR value,
   /* [retval][out] */ long *pindex) = 0;
#endif   
virtual /* [helpstring] */ ErrorCode  RemoveKeyTime( 
   /* [in] */ long index) = 0;

};

template<class TYPE> interface IAnimatedOwner
{
	virtual void BaseValChanged() = 0;
	virtual void SetAnimVal(typename TYPE::t_baseType* value) = 0;
};

interface IElementTargetAttributes
{
};
    
enum ASObjectType
{
	AS_ELEMENT_DECLARATION	= 1,
	AS_ATTRIBUTE_DECLARATION	= 2,
	AS_NOTATION_DECLARATION	= 3,
	AS_ENTITY_DECLARATION	= 4,
	AS_CONTENTMODEL	= 5,
	AS_MODEL = 6,
	AS_COMPLEXTYPE_DECLARATION	= 7,
	AS_SIMPLETYPE_DECLARATION	= 8,
	AS_INCLUDE	= 9,
	AS_IMPORT	= 10
};

enum ASContentModelType
{
	EMPTY_CONTENTTYPE	= 40,
	SIMPLE_CONTENTTYPE	= 41,
	ELEMENT_CONTENTTYPE	= 42,
	MIXED_CONTENTTYPE	= 43,
	ANY_CONTENTTYPE	= 44
};

enum ASContentModelTypeOp
{
	AS_SEQUENCE	= 50,
	AS_CHOICE	= 51,
	AS_ALL	= 52,
	AS_UNDEFINED_CM	= 53,
	AS_ATOMIC	= 54,
	AS_NONE	= AS_ATOMIC
};

enum ASValueConstraint
{
	NONE_VC	= 0,
	DEFAULT_VC	= 60,
	FIXED_VC	= 61,
	REQUIRED_VC	= 62
};

enum ASDataType
{
	STRING_DATATYPE	= 1,
	NOTATION_DATATYPE	= 10,
	ID_DATATYPE	= 11,
	IDREF_DATATYPE	= 12,
	IDREFS_DATATYPE	= 13,
	ENTITY_DATATYPE	= 14,
	ENTITIES_DATATYPE	= 15,
	NMTOKEN_DATATYPE	= 16,
	NMTOKENS_DATATYPE	= 17,
	BOOLEAN_DATATYPE	= 100,
	FLOAT_DATATYPE	= 101,
	DOUBLE_DATATYPE	= 102,
	DECIMAL_DATATYPE	= 103,
	HEXBINARY_DATATYPE	= 104,
	BASE64BINARY_DATATYPE	= 105,
	ANYURI_DATATYPE	= 106,
	QNAME_DATATYPE	= 107,
	DURATION_DATATYPE	= 108,
	DATETIME_DATATYPE	= 109,
	DATE_DATATYPE	= 110,
	TIME_DATATYPE	= 111,
	GYEARMONTH_DATATYPE	= 112,
	GYEAR_DATATYPE	= 113,
	GMONTHDAY_DATATYPE	= 114,
	GDAY_DATATYPE	= 115,
	GMONTH_DATATYPE	= 116,
	INTEGER	= 117,
	NAME_DATATYPE	= 200,
	NCNAME_DATATYPE	= 201,
	NORMALIZEDSTRING_DATATYPE	= 202,
	TOKEN_DATATYPE	= 203,
	LANGUAGE_DATATYPE	= 204,
	NONPOSITIVEINTEGER_DATATYPE	= 205,
	NEGATIVEINTEGER_DATATYPE	= 206,
	LONG_DATATYPE	= 207,
	INT_DATATYPE	= 208,
	SHORT_DATATYPE	= 209,
	BYTE_DATATYPE	= 210,
	NONNEGATIVEINTEGER_DATATYPE	= 211,
	UNSIGNEDLONG_DATATYPE	= 212,
	UNSIGNEDINT_DATATYPE	= 213,
	UNSIGNEDSHORT_DATATYPE	= 214,
	UNSIGNEDBYTE_DATATYPE	= 215,
	POSITIVEINTEGER_DATATYPE	= 216,
	OTHER_SIMPLE_DATATYPE	= 1000,
	COMPLEX_DATATYPE	= 1001
};

interface ISVGStylable : public IElementCSSInlineStyle
{
	virtual ISVGAnimatedString* get_className() = 0;
	virtual CSSValue* getPresentationAttribute(StringIn name) = 0;	
};

class WEBEXT StyleSheet : public Object, public Style::IStyleSheet
{
public:
	CTOR StyleSheet(Node* ownerNode);

	StyleSheet* get_parentStyleSheet();
	String get_href() const;
	MediaList* get_media();
	virtual String get_type() const;

	String get_title() const;
	void set_title(StringIn newVal);

	bool get_disabled() const;

public:

	StyleSheet* m_parentStyleSheet;
	Node* m_ownerNode;
	MediaList* m_media;
	String m_href;
	bool m_disabled;
};

class WEBEXT CSSStyleSheet : public StyleSheet
{
public:
	CTOR CSSStyleSheet(Node* ownerNode, Style::CSSRule* ownerRule);

	String get_cssText();
	void set_cssText(StringIn cssText);
	virtual String get_type() const override;
	Style::CSSRule* get_ownerRule();
	void set_disabled(bool newVal);

	//
	virtual Style::IStyleDeclaration* CreateDeclaration() override;
	virtual void AddRules(Style::SelectorList* selectors, Style::IStyleDeclaration* declarator) override;
	virtual size_t GetRuleCount() override;
	virtual Style::ICSSRule* GetRule(size_t index) override;
//

	Style::CSSRule* m_ownerRule;
//	CSSRuleList* m_cssRules;

	String m_cssText;

	Object* m_pListener;	// TODO
};

interface ILinkStyle
{
	virtual CSSStyleSheet* get_sheet() = 0;
};

enum ASUserDataHandleType
{
	ASNODE_CLONED	= 1,
	ASNODE_IMPORTED	= 2,
	ASNODE_DELETED	= 3,
	ASNODE_RENAMED	= 4
};

interface IASUserDataHandler
{
virtual /* [helpstring] */ ErrorCode _handle( 
/* [in] */ ASUserDataHandleType operation,
/* [in] */ StringIn key,
/* [in] */ Object *data,
/* [in] */ IASObject *src,
/* [in] */ IASObject *dst) = 0;

};

interface IASObject
{
	virtual ASObjectType get_asNodeType() const = 0;
	virtual IASModel* get_ownerASModel() = 0;
	virtual String get_nodeName() = 0;

//        virtual /* [helpstring][propput] */ ErrorCode  set_nodeName( 
//            /* [in] */ BSTR newVal) = 0;
#if 0
virtual /* [helpstring][propget] */ ErrorCode  get_prefix( 
/* [retval][out] */ BSTR *pVal) = 0;

virtual /* [helpstring][propput] */ ErrorCode  set_prefix( 
/* [in] */ BSTR newVal) = 0;

virtual /* [helpstring][propget] */ ErrorCode  get_localName( 
/* [retval][out] */ BSTR *pVal) = 0;

virtual /* [helpstring][propput] */ ErrorCode  set_localName( 
/* [in] */ BSTR newVal) = 0;
#endif   
	virtual IASObject* cloneASObject(bool deep) const = 0;
	virtual ErrorCode  setUserData(StringIn key,Object *data,IASUserDataHandler *handler, Object **pVal) = 0;
	virtual ErrorCode  getUserData(StringIn key, Object **pVal) = 0;

	#if 0
	virtual /* [helpstring] */ ErrorCode  GetElement( 
	/* [retval][out] */ Element **pVal) = 0;

	virtual /* [helpstring] */ ErrorCode  SetElement( 
	/* [in] */ Element *newVal) = 0;
	#endif   
};

interface IASModel : public IASObject
{
	virtual bool get_isNamespaceAware() const = 0;
	virtual IASNamedObjectMap* get_elementDeclarations() = 0;
	virtual IASNamedObjectMap* get_attributeDeclarations() = 0;
	virtual IASNamedObjectMap* get_notationDeclarations() = 0;
	virtual IASNamedObjectMap* get_entityDeclarations() = 0;
	virtual IASNamedObjectMap* get_contentModelDeclarations() = 0;

#if 0
virtual /* [helpstring] */ ErrorCode  getASModels( 
/* [retval][out] */ IASObjectList **Val) = 0;

virtual /* [helpstring] */ ErrorCode  createASElementDeclaration( 
/* [in] */ BSTR namespaceURI,
/* [in] */ BSTR name,
/* [retval][out] */ IASElementDeclaration **pVal) = 0;

virtual /* [helpstring] */ ErrorCode  createASAttributeDeclaration( 
/* [in] */ BSTR namespaceURI,
/* [in] */ BSTR name,
/* [retval][out] */ IASAttributeDeclaration **pVal) = 0;

virtual /* [helpstring] */ ErrorCode  createASNotationDeclaration( 
/* [in] */ BSTR namespaceURI,
/* [in] */ BSTR name,
/* [in] */ BSTR systemId,
/* [in] */ BSTR publicId,
/* [retval][out] */ IASNotationDeclaration **pVal) = 0;

virtual /* [helpstring] */ ErrorCode  createASEntityDeclaration( 
/* [in] */ BSTR name,
/* [retval][out] */ IASEntityDeclaration **pVal) = 0;

virtual /* [helpstring] */ ErrorCode  createASContentModel( 
/* [in] */ unsigned long minOccurs,
/* [in] */ unsigned long maxOccurs,
/* [in] */ ASContentModelTypeOp Operator,
/* [retval][out] */ IASContentModel **pVal) = 0;

virtual /* [helpstring] */ ErrorCode  FireModelChanged( void) = 0;
#endif   
};

interface IASNamedObjectMap
{
	virtual size_t get_length() const = 0;
	virtual IASObject* getNamedItem(StringIn name) = 0;
	virtual IASObject* getNamedItemNS(StringIn namespaceURI, StringIn localName) = 0;
	virtual IASObject* item(size_t index) = 0;
	virtual IASObject* setNamedItem(IASObject *newASObject) = 0;
	virtual IASObject* setNamedItemNS(IASObject *newASObject) = 0;
};

interface IASObjectList
{
	virtual size_t get_length() const = 0;
	virtual IASObject* item(size_t index) const = 0;        
};

interface IASContentModel : public IASObject
{
	virtual long get_minOccurs() const = 0;
	virtual void set_minOccurs(long newVal) = 0;
	virtual long get_maxOccurs() const = 0;
	virtual void set_maxOccurs(long newVal) = 0;
	virtual ASContentModelTypeOp get_listOperator() const = 0;
	virtual void set_listOperator(ASContentModelTypeOp newVal) = 0;
	virtual IASObjectList * get_subModels() = 0;

#if 0
virtual /* [helpstring][propput] */ ErrorCode  set_subModels( 
/* [in] */ IASObjectList *newVal) = 0;
#endif

virtual void replacesubModel( 
/* [in] */ IASObject *oldNode,
/* [in] */ IASObject *newNode) = 0;

#if 0
virtual /* [helpstring] */ ErrorCode  appendsubModel( 
/* [in] */ IASObject *newNode,
/* [retval][out] */ unsigned long *length) = 0;

virtual /* [helpstring] */ ErrorCode  insertBeforeSubModel( 
/* [in] */ IASObject *newObject,
/* [in] */ IASObject *refObject,
/* [retval][out] */ IASObject **pVal) = 0;

virtual /* [helpstring] */ ErrorCode  removeSubModel( 
/* [in] */ IASObject *oldObject) = 0;

//     virtual /* [helpstring] */ ErrorCode  UpdateElement( void) = 0;
#endif   
};

interface IASDeclaration : public IASObject
{
	virtual IASContentModel* get_asCM() = 0;
	virtual void set_asCM(IASContentModel *newVal) = 0;

#if 0
virtual /* [helpstring] */ ErrorCode  GetDefType( 
/* [retval][out] */ BSTR *pVal) = 0;

virtual /* [helpstring] */ ErrorCode  BuildDefs( void) = 0;
#endif   
};

interface IASElementDeclaration : public IASDeclaration
{
	virtual String get_systemId() = 0;
	virtual void set_systemId(StringIn newVal) = 0;

//  virtual bool get_isPCDataOnly() = 0;

// virtual /* [helpstring][propput] */ ErrorCode  set_isPCDataOnly( 
//   /* [in] */ bool newVal) = 0;

	virtual bool get_strictMixedContent() const = 0;
	virtual void set_strictMixedContent(bool newVal) = 0;
	virtual IASNamedObjectMap* get_ASAttributeDecls() = 0;
	virtual IASObjectList* get_attributesList() = 0;
	virtual ASContentModelType get_contentType() = 0;
	virtual void set_contentType(ASContentModelType newVal) = 0;
	virtual ASDataType get_elementType() const = 0;
	virtual void set_elementType(ASDataType newVal) = 0;
	virtual IASDataTypeDeclaration* get_elementDataType() = 0;
	virtual void addASAttributeDecl(IASAttributeDeclaration *attributeDecl) = 0;
	virtual IASAttributeDeclaration* removeASAttributeDecl(IASAttributeDeclaration *attributeDecl) = 0;
};

interface IASDataTypeDeclaration : public IASDeclaration
{
	virtual String GetName() = 0;
};

interface IASSimpleTypeDeclaration : public IASDataTypeDeclaration
{
};

interface IASAttributeDeclaration : public IASObject
{
	virtual ASDataType get_dataType() const = 0;
	virtual void set_dataType(ASDataType newVal) = 0;
	virtual String get_dataValue() = 0;
	virtual void set_dataValue(StringIn newVal) = 0;
	virtual String get_enumAttr() = 0;
	virtual void set_enumAttr(StringIn newVal) = 0;
	virtual ASValueConstraint get_defaultType() const = 0;
	virtual void set_defaultType(ASValueConstraint newVal) = 0;
};

interface IASAttributeGroupDeclaration : public IASObject
{
	virtual IASObjectList* get_attributesList() = 0;
};

interface IASEntityDeclaration : public IASObject
{
	virtual String get_entityValue() = 0;
};

interface IASNotationDeclaration : public IASObject
{
};
    
interface IDocumentAS
{
#if 0
        virtual /* [helpstring][propget] */ ErrorCode  get_activeASModel( 
            /* [retval][out] */ IASModel **pVal) = 0;
        
        virtual /* [helpstring][propput] */ ErrorCode  set_activeASModel( 
            /* [in] */ IASModel *newVal) = 0;
        
        virtual /* [helpstring] */ ErrorCode  validate( void) = 0;
#endif   
};

interface INodeEditAS
{
};
    
interface IElementEditAS : public INodeEditAS
{
	virtual IASNamedObjectMap* getAttributeList() = 0;
};

#if 0    
    MIDL_INTERFACE("1C278474-584F-4251-B5A1-C899E7677B0F")
    ILSVGShadowElement : public Element
    {
    public:
        virtual /* [helpstring] */ ErrorCode  getShadowOwnerElement( 
            /* [retval][out] */ Element **pVal) = 0;
        
        virtual /* [helpstring] */ ErrorCode  getElementById( 
            /* [in] */ BSTR elementId,
            /* [retval][out] */ Element **pVal) = 0;
        
    };
#endif

// ??
interface ISVGShadowable
{
  virtual ISVGShadowElement* get_shadowTree() = 0;
};

enum CheckTypeVAL
{
	WF_CHECK = 1,
	NS_WF_CHECK	= 2,
	PARTIAL_VALIDITY_CHECK	= 3,
	STRICT_VALIDITY_CHECK	= 4
};

interface INodeEditVAL
{
	virtual String get_defaultValue() = 0;
	virtual IDOMStringList* get_enumeratedValues() = 0;
	virtual bool isNodeValid(bool deep, CheckTypeVAL wFValidityCheckLevel) = 0;
};

interface IElementEditVAL : public INodeEditVAL
{
	virtual IDOMNameList* get_allowedChildren() = 0;
	virtual IDOMNameList* get_allowedParents() = 0;
	virtual IDOMNameList* get_allowedNextSiblings() = 0;
	virtual IDOMNameList* get_allowedPreviousSiblings() = 0;
	virtual IDOMNameList* get_allowedAttributes() = 0;
	virtual IDOMNameList* get_requiredAttributes() = 0;
};

interface IDocumentEditVAL : public INodeEditVAL
{
	virtual bool get_continuousValidityChecking() = 0;
	virtual void set_continuousValidityChecking(bool newVal) = 0;
	virtual IDOMNameList* getDefinedElementTypes() = 0;
};

enum VirtualKeyCode
{
	DOM_VK_UNDEFINED	= 0,
	DOM_VK_RIGHT_ALT	= 0x1,
	DOM_VK_LEFT_ALT	= 0x2,
	DOM_VK_LEFT_CONTROL	= 0x3,
	DOM_VK_RIGHT_CONTROL	= 0x4,
	DOM_VK_LEFT_SHIFT	= 0x5,
	DOM_VK_RIGHT_SHIFT	= 0x6,
	DOM_VK_LEFT_META	= 0x7,
	DOM_VK_RIGHT_META	= 0x8,
	DOM_VK_CAPS_LOCK	= 0x9,
	DOM_VK_DELETE	= 0xa,
	DOM_VK_END	= 0xb,
	DOM_VK_ENTER	= 0xc,
	DOM_VK_ESCAPE	= 0xd,
	DOM_VK_HOME	= 0xe,
	DOM_VK_INSERT	= 0xf,
	DOM_VK_NUM_LOCK	= 0x10,
	DOM_VK_PAUSE	= 0x11,
	DOM_VK_PRINTSCREEN	= 0x12,
	DOM_VK_SCROLL_LOCK	= 0x13,
	DOM_VK_LEFT	= 0x14,
	DOM_VK_RIGHT	= 0x15,
	DOM_VK_UP	= 0x16,
	DOM_VK_DOWN	= 0x17,
	DOM_VK_PAGE_DOWN	= 0x18,
	DOM_VK_PAGE_UP	= 0x19,
	DOM_VK_F1	= 0x1a,
	DOM_VK_F2	= 0x1b,
	DOM_VK_F3	= 0x1c,
	DOM_VK_F4	= 0x1d,
	DOM_VK_F5	= 0x1e,
	DOM_VK_F6	= 0x1f,
	DOM_VK_F7	= 0x20,
	DOM_VK_F8	= 0x21,
	DOM_VK_F9	= 0x22,
	DOM_VK_F10	= 0x23,
	DOM_VK_F11	= 0x24,
	DOM_VK_F12	= 0x25,
	DOM_VK_F13	= 0x26,
	DOM_VK_F14	= 0x27,
	DOM_VK_F15	= 0x28,
	DOM_VK_F16	= 0x29,
	DOM_VK_F17	= 0x2a,
	DOM_VK_F18	= 0x2b,
	DOM_VK_F19	= 0x2c,
	DOM_VK_F20	= 0x2d,
	DOM_VK_F21	= 0x2e,
	DOM_VK_F22	= 0x2f,
	DOM_VK_F23	= 0x30,
	DOM_VK_F24	= 0x31
};

interface IDocumentRange
{
	virtual Range* createRange() = 0;
};

enum FilterReturn
{
	FILTER_ACCEPT	= 1,
	FILTER_REJECT	= 2,
	FILTER_SKIP	= 3
};

enum FilterShow
{
	SHOW_ALL	= 0xffffffff,
	SHOW_ELEMENT	= 0x1,
	SHOW_ATTRIBUTE	= 0x2,
	SHOW_TEXT	= 0x4,
	SHOW_CDATA_SECTION	= 0x8,
	SHOW_ENTITY_REFERENCE	= 0x10,
	SHOW_ENTITY	= 0x20,
	SHOW_PROCESSING_INSTRUCTION	= 0x40,
	SHOW_COMMENT	= 0x80,
	SHOW_DOCUMENT	= 0x100,
	SHOW_DOCUMENT_TYPE	= 0x200,
	SHOW_DOCUMENT_FRAGMENT	= 0x400,
	SHOW_NOTATION	= 0x800
};

interface INodeFilter
{
	virtual FilterReturn acceptNode(Node* node) = 0;
};

interface INodeIterator
{
	virtual Node* get_root() = 0;
	virtual FilterShow get_whatToShow() = 0;
	virtual INodeFilter* get_filter() = 0;
	virtual bool get_expandEntityReferences() = 0;
	virtual Node* nextNode() = 0;
	virtual Node* previousNode() = 0;
	virtual void detach() = 0;
};

interface IDocumentTraversal
{
#if 0
virtual /* [helpstring] */ ErrorCode  createNodeIterator( 
   /* [in] */ Node *root,
   /* [in] */ FilterShow whatToShow,
   /* [in] */ NodeFilter *filter,
   /* [in] */ bool entityReferenceExpansion,
   /* [retval][out] */ INodeIterator **pVal) = 0;
#endif
};

	/*
struct AbstractView;

    MIDL_INTERFACE("5F59C328-7EFC-47fe-B711-176F81796E04")
    DocumentView : public Object
    {
    public:
        virtual AbstractView* get_defaultView() = 0;
    };
	 */

    
enum
{
	AS_ZERO = 0,
	AS_ONE = 1,
	AS_UNBOUNDED = 0xffffffff,
};

#undef SEVERITY_ERROR

enum DOMErrorSeverity
{
	SEVERITY_WARNING	= 0,
	SEVERITY_ERROR	= 1,
	SEVERITY_FATAL_ERROR	= 2
};

interface IDOMLocator
{
	virtual int get_columnNumber() = 0;
	virtual int get_lineNumber() = 0;
	virtual long get_offset() = 0;
	virtual Node* getNode() = 0;
	virtual String get_uri() const = 0;
};

interface IDOMError
{
	virtual DOMErrorSeverity get_severity() = 0;
	virtual String get_message() = 0;
	virtual Object* get_relatedData() = 0;
	virtual Object* get_relatedException() = 0;
	virtual IDOMLocator* get_location() = 0;
};

interface IDOMErrorHandler
{
	virtual bool handleError(IDOMError* pError) = 0;
};

enum UserDataHandleType
{
	NODE_CLONED = 1,
	NODE_IMPORTED = 2,
	NODE_DELETED = 3,
	NODE_RENAMED = 4
};

interface IDOMUserDataHandler
{
	virtual ErrorCode _handle(UserDataHandleType operation, StringIn key, Object* data, Node* src, Node* dst) = 0;
};

enum NodeType
{
	NODE_INVALID	= 0,
	NODE_ELEMENT	= 1,
	NODE_ATTRIBUTE	= 2,
	NODE_TEXT	= 3,
	NODE_CDATA_SECTION	= 4,
	NODE_ENTITY_REFERENCE	= 5,
	NODE_ENTITY	= 6,
	NODE_PROCESSING_INSTRUCTION	= 7,
	NODE_COMMENT	= 8,
	NODE_DOCUMENT	= 9,
	NODE_DOCUMENT_TYPE	= 10,
	NODE_DOCUMENT_FRAGMENT	= 11,
	NODE_NOTATION	= 12,
	NODE_NOT_WELLFORMED	= 201,	// TODO remove this from here
	/*
	DTD_ENTITY	= 202,
	DTD_NOTATION	= 203,
	DTD_ELEMENT	= 204,
	DTD_ATTLIST	= 205
	*/
};

interface ILSParser
{
	virtual IDOMErrorHandler* get_errorHandler() = 0;
	virtual void set_errorHandler(IDOMErrorHandler  *newVal) = 0;

	virtual Document* parseURI(StringIn uri) = 0;
	#if 0
	virtual /* [helpstring] */ ErrorCode  parseTextData( 
		/* [in] */ ITextData  *textData,
		/* [retval][out] */ Document  * *pVal) = 0;
	#endif   
};

interface IRGBColorListener
{
	virtual void OnChangedRGBColor(RGBColor* pRGBColor) = 0;
};

interface ICSSRectListener
{
	virtual void OnChangedRect(CSSRect* pRect) = 0;
};

interface ICSSValueListListener
{
	virtual void OnCSSValueChanged(CSSValueList* pValue) = 0;
};

interface ISVGICCColorListener
{
	virtual void OnChangedICCColor(SVGICCColor* pRGBColor) = 0;
};

interface ISVGColorListener
{
	virtual void OnCSSValueChanged(SVGColor* pValue) = 0;
};

interface ISVGPaintListener
{
	virtual void OnCSSValueChanged(SVGPaint* pValue) = 0;
};

interface ICSSPrimitiveValueListener
{
	virtual void OnCSSValueChanged(CSSPrimitiveValue* pValue) = 0;
};

// ExceptionCode
const unsigned short      INDEX_SIZE_ERR                 = 1;
const unsigned short      DOMSTRING_SIZE_ERR             = 2;
const unsigned short      HIERARCHY_REQUEST_ERR          = 3;
const unsigned short      WRONG_DOCUMENT_ERR             = 4;
const unsigned short      INVALID_CHARACTER_ERR          = 5;
const unsigned short      NO_DATA_ALLOWED_ERR            = 6;
const unsigned short      NO_MODIFICATION_ALLOWED_ERR    = 7;
const unsigned short      NOT_FOUND_ERR                  = 8;
const unsigned short      NOT_SUPPORTED_ERR              = 9;
const unsigned short      INUSE_ATTRIBUTE_ERR            = 10;
// Introduced in DOM Level 2:
const unsigned short      INVALID_STATE_ERR              = 11;
// Introduced in DOM Level 2:
const unsigned short      SYNTAX_ERR                     = 12;
// Introduced in DOM Level 2:
const unsigned short      INVALID_MODIFICATION_ERR       = 13;
// Introduced in DOM Level 2:
const unsigned short      NAMESPACE_ERR                  = 14;
// Introduced in DOM Level 2:
const unsigned short      INVALID_ACCESS_ERR             = 15;
// Introduced in DOM Level 3:
const unsigned short      VALIDATION_ERR                 = 16;
// Introduced in DOM Level 3:
const unsigned short      TYPE_MISMATCH_ERR              = 17;

class WEBEXT DOMException : public Exception
{
public:
	CTOR DOMException(unsigned short code) : m_code(code)
	{
	}

	CTOR DOMException(StringIn str, unsigned short code = 0) : Exception(str), m_code(code)
	{
	}

	unsigned short get_code() const
	{
		return m_code;
	}

protected:

	unsigned short m_code;
};

// DerivationMethods
const unsigned int DERIVATION_RESTRICTION         = 0x00000001;
const unsigned int DERIVATION_EXTENSION           = 0x00000002;
const unsigned int DERIVATION_UNION               = 0x00000004;
const unsigned int DERIVATION_LIST                = 0x00000008;

class WEBEXT TypeInfo : public Object
{
public:
	CTOR TypeInfo();

	String get_typeName()
	{
		return m_typeName;
	}

	String get_typeNamespace()
	{
		return m_typeNamespace;
	}

	bool isDerivedFrom(StringIn typeNamespaceArg, StringIn typeNameArg, unsigned int derivationMethod);

protected:

	String m_typeName;
	String m_typeNamespace;
};

typedef ubyte octet;	// ??

interface ICanvasPixelArray
{
	virtual unsigned int get_length() abstract;
	virtual octet getter(unsigned long index) abstract;
	virtual void setter(unsigned long index, octet value) abstract;
};

interface IImageData
{
	virtual unsigned int get_width() abstract;
	virtual unsigned int get_height() abstract;
	virtual ICanvasPixelArray* get_data() abstract;
};

class WEBEXT ImageData : public Object, IImageData
{
public:
	CTOR ImageData();

	virtual unsigned int get_width() override;
	virtual unsigned int get_height() override;
	virtual ICanvasPixelArray* get_data() override;

// TODO

	Graphics::Bitmap* m_bitmap;
};

interface IImageProviderElement
{
	virtual gm::Sizei GetImageSize() = 0;
};

interface IWebCanvas
{
	virtual Object* getContext(StringIn contextId) abstract;
};

interface IEventTarget
{
	virtual void addEventListener(StringIn type, Object* listener, bool useCapture) abstract;
	virtual void removeEventListener(StringIn type, Object* listener, bool useCapture) abstract;

	// Modified in DOM Level 3:
	virtual bool dispatchEvent(Event* evt)/*raises(EventException)*/ abstract;

	// Introduced in DOM Level 3:
	virtual void addEventListenerNS(StringIn namespaceURI, StringIn type, Object* listener, bool useCapture, IEventListenerGroup* evtGroup) abstract;
	virtual void removeEventListenerNS(StringIn namespaceURI, StringIn type, Object* listener, bool useCapture) abstract;
	virtual bool willTriggerNS(StringIn namespaceURI, StringIn type) abstract;
	virtual bool hasEventListenerNS(StringIn namespaceURI, StringIn type) abstract;
};

interface IAbstractView
{
	virtual Document* get_document() abstract;	// or IDocumentView
	virtual String get_mediaMode() abstract;
};

interface IViewCSS : public IAbstractView
{
	virtual ICSSStyleDeclaration* getComputedStyle(Element* elt, StringIn pseudoElt) abstract;
};

Object* RegisterElement(StringIn namespaceURI, StringIn localName, const Type_Info& typeInfo);

}	// Web
}	// System

#include "cssproperty.h"
#include "cssunits.h"
#include "CSSValue.h"
#include "CSSCounter.h"
#include "CSSPrimitiveValue.h"
#include "RGBColor.h"
#include "CSSRect.h"
#include "SVGPaint.h"
#include "SVGICCColor.h"

#include "ValueType.h"
#include "XMLAttr.h"
#include "CSSPrimitiveValue.h"
#include "CSSValueList.h"
#include "SVGPaint.h"
#include "SVGColor.h"

#include "CSSStyleDeclaration.h"

#include "StyleSheetList.h"
#include "EnumerationCalcModeType.h"

#include "PNode.h"
#include "PElement.h"

#include "DOMEventTargetImpl.h"
#include "DOMImplementation.h"
#include "Node.h"
#include "NodeList.h"
#include "NamedNodeMap.h"
#include "Attr.h"
#include "Element.h"
#include "CharacterData.h"
#include "Text.h"
#include "Comment.h"
#include "CDATASection.h"
#include "EntityReference.h"
#include "DocumentFragment.h"
#include "ProcessingInstruction.h"
#include "ProcessingInstructionStylesheet.h"
#include "DocumentType.h"
#include "DOMNotWellformed.h"
#include "DOMLocator.h"
#include "ElementEditVALImpl.h"
#include "ElementTime.h"
#include "ElementTimeContainer.h"
#include "AnimationTarget.h"
#include "Event.h"
#include "UIEvent.h"
#include "MouseEvent.h"
#include "DOMDocumentEvent.h"
#include "MutationEvent.h"
#include "MutationNameEvent.h"
#include "Document.h"
#include "Entity.h"
#include "DOMRange.h"
#include "DTDDocument.h"
#include "ASDTDModel.h"
//#include "DocumentStyleImpl.h"
#include "GenericDocument.h"
#include "GenericElement.h"

#include "DOMEventTargetImpl.h"
#include "DocumentASImpl.h"
#include "DocumentEditVALImpl.h"
#include "ElementCSSInlineStyleImpl.h"
//#include "AnimationTarget.h"
#include "AnimationElement.h"
#include "PImage.h"
#include "SMILTimeList.h"
#include "LDOMDocumentRangeImpl.h"
#include "LDOMDocumentTraversalImpl.h"
#include "MediaList.h"
#include "LinkStyleImpl.h"
//#include "DocumentStyleImpl.h"

#include "PTextNode.h"
#include "PWord.h"
#include "PBlockBox.h"
#include "PLineBox.h"
#include "PInlineBox.h"
#include "Window.h"
#include "HTMContext.h"

//#include "CSSRuleList.h"
//#include "CSSCommentRule.h"
//#include "CSSImportRule.h"
//#include "CSSStyleRule.h"
//#include "CSSMediaRule.h"
//#include "CSSValueUnknown.h"

#include "SVGNumberValue.h"
#include "BooleanValue.h"
#include "SVGRect.h"
#include "SVGPoint.h"
#include "SVGMatrix.h"
#include "SVGPointList.h"
#include "SVGNumberList.h"

#include "parsecolor.h"

#include "CanvasRenderingContext2D.h"
#include "WebGL.h"

namespace System
{

inline Object* GetAsObject(Object* o)
{
	return o;
}

namespace Web
{
typedef CCSSValueType<CreateInstanceT<CSSValueUnknown> > CSSValueUnknownType;

template</*class ContainerClass,*/ class AnimatedType, int unique = 0> class AnimatedAttributeT : public IAttrOwner, public IAnimatedOwner<AnimatedType>
{
public:

	typedef AnimatedAttributeT</*ContainerClass,*/ AnimatedType, unique> thisClass;

	CTOR AnimatedAttributeT(Element* element, StringIn namespaceURI, StringIn localName, StringIn defaultValueAsString = NULL)
	{
		//static_cast<ContainerClass*>(this)->UnspecifiedAttr(inner_ptr<Attr*>(static_cast<ContainerClass*>(this), &m_attr), namespaceURI, localName);
		element->UnspecifiedAttr(inner_ptr<Attr*>(element, &m_attr), namespaceURI, localName);

		ASSERT(m_attr);
		m_attr->m_owner = this;

		m_animated = new AnimatedType;
		m_animated->m_owner = this;
		if (m_attr->get_specified())
			m_animated->SetBaseValAsString(m_attr->get_value());
		else
			m_animated->SetBaseValAsString(defaultValueAsString);
	}

	virtual Element* GetElement()
	{
		return m_attr->get_ownerElement();
		//return static_cast<ContainerClass*>(this);
	}

	operator AnimatedType* ()
	{
		return m_animated;
	}

	operator Attr* ()
	{
		return m_attr;
	}

	AnimatedType* operator->()
	{
		return m_animated;
	}

	virtual String GetBaseValAsString()
	{
		return GetAsString(m_animated->m_baseVal);
	}
	
	virtual Object* GetBaseValAsObject()
	{
		return GetAsObject(m_animated->m_baseVal);
	}
	virtual void SetAnimValAsObject(Object* value)
	{
		ASSERT(0);
		// TODO, have this
#if 0
		typename AnimatedType::t_baseType* length = static_cast<typename AnimatedType::t_baseType*>(value);
		SetAnimVal(length);
#endif
	}

	virtual IValueHandler* GetValueHandler()
	{
#ifndef __LERSTAD__	// TODO
		return AnimatedType::t_valueHandler::GetValueHandler();
#endif
	}

	virtual void UpdateBaseValString()
	{
	//	SetStringValue(m_animated->m_baseVal, m_attr->get_value());
		m_animated->SetBaseValAsString(m_attr->get_value());
		// static_cast<ContainerClass*>(this)->OnSetBaseValString(m_animated, value);
	}

	virtual void SetAnimVal(typename AnimatedType::t_baseType* value)
	{
		m_animated->SetAnimVal(value);
		// static_cast<ContainerClass*>(this)->OnSetAnimVal(m_animated, value);
	}

	virtual void BaseValChanged()
	{
		m_attr->m_valueIsValid = false;
		// static_cast<ContainerClass*>(this)->OnBaseValChanged(m_attr);
	}

	Attr* m_attr;
	AnimatedType* m_animated;
};

template</*class ContainerClass,*/ class AnimatedType, int unique = 0> class AnimatedAttributeAffectsBoundsT : public AnimatedAttributeT</*ContainerClass,*/ AnimatedType, unique>
{
public:

	typedef AnimatedAttributeT</*ContainerClass,*/ AnimatedType, unique> baseClass;

	CTOR AnimatedAttributeAffectsBoundsT(Element* element, StringIn namespaceURI, StringIn localName, StringIn defaultValueAsString = NULL) : AnimatedAttributeT</*ContainerClass,*/ AnimatedType, unique>(element, namespaceURI, localName, defaultValueAsString)
	{
	}

	virtual void UpdateBaseValString()
	{
		baseClass::UpdateBaseValString();
	//	static_cast<ContainerClass*>(this)->GetPElement()->InvalidateBounds();
		GetElement()->GetPElement()->InvalidateBounds();
		// TODO AttrModifiedEvent
	}

	virtual void BaseValChanged()
	{
		baseClass::BaseValChanged();
		GetElement()->GetPElement()->InvalidateBounds();
		// TODO AttrModifiedEvent
	}

	virtual void SetAnimVal(typename AnimatedType::t_baseType* value)
	{
		baseClass::SetAnimVal(value);
		GetElement()->GetPElement()->InvalidateBounds();
	}
};

#define XMLANIMATEDATTR(classname, impvaluetype, valuetype, name, n)	\
	AnimatedAttributeAffectsBoundsT<impvaluetype, n>& name##_attr()	\
	{	\
		return *static_cast<AnimatedAttributeAffectsBoundsT<impvaluetype, n>*>(this);	\
	}	\
public:	valuetype get_##name() { return name##_attr(); }	\
		Attr* get_##name##Attr() { return name##_attr(); }

//[NoInterfaceObject]
interface XMLHttpRequestEventTarget : EventTarget
{
	// for future use
};

typedef bool boolean;

class WEBEXT XMLHttpRequest : public DispatcherObject, public XMLHttpRequestEventTarget
{
public:

	CTOR XMLHttpRequest();

	// event handler attribute
	IFunction* get_onreadystatechange();
	void set_onreadystatechange(IFunction* function);

	// states
	enum ReadyState
	{
		ReadyState_UNSENT = 0,
		ReadyState_OPENED = 1,
		ReadyState_HEADERS_RECEIVED = 2,
		ReadyState_LOADING = 3,
		ReadyState_DONE = 4,
	};

	ReadyState get_readyState();

	// request
	void open(StringIn method, StringIn url);
	void open(StringIn method, StringIn url, boolean async);
	void open(StringIn method, StringIn url, boolean async, StringIn user);
	void open(StringIn method, StringIn url, boolean async, StringIn user, StringIn password);
	void setRequestHeader(StringIn header, StringIn value);
	void send();
	void send(Document* data);
	void send(StringIn data);
//	void send([AllowAny] DOMString? data);
	void abort();

	// response
	unsigned short get_status();
	String get_statusText();
	String getResponseHeader(StringIn header);
	String getAllResponseHeaders();
	String get_responseText();
	Document* get_responseXML();

private:

	IFunction* m_onreadystatechange;
	String m_responseText;
	Document* m_responseXML;

	ReadyState m_readyState;
	unsigned short m_status;
//	String m_statusText;

	Net::HttpConnection* m_conn;
	Net::HttpRequest* m_request;
};

}	// Web

// This version assumes c is a legal hex digit
int Hex2Number(WCHAR c);
int strhex2int(const WCHAR* p);

uint32 GetUCode(const WCHAR* ucode);

}	// System

#endif	// Web_XML_h
