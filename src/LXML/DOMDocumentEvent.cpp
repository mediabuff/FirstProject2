#include "stdafx.h"
#include "LXML2.h"
#include "DOMDocumentEvent.h"

#include "TimeEvent.h"
#include "SVGZoomEvent.h"

namespace System
{
namespace Web
{

Event* DocumentEvent::createEvent(StringIn eventType)
{
	if (eventType == nullptr) throw new ArgumentNullException();

	if (eventType.Compare(L"MutationEvent") == 0)
	{
		MutationEvent* e = new MutationEvent;
		return e;
	}
	else if (eventType.Compare(L"MutationNameEvent") == 0)
	{
		MutationNameEvent* e = new MutationNameEvent;
		return e;
	}
	else if (eventType.Compare(L"MouseEvents") == 0)
	{
		MouseEvent* e = new MouseEvent;
		return e;
	}
	/*
	else if (!tcsicmp(eventType, OLESTR("TextEvent")))
	{
		CComObject<CLTextEvent>* e;
		CComObject<CLTextEvent>::CreateInstance(&e);
		if (e)
		{
			(*ppVal = e)->AddRef();
		}
	}
	*/
	else if (eventType.Compare(L"TimeEvents") == 0)
	{
		TimeEvent* e = new TimeEvent;
		return e;
	}
	else if (eventType.Compare(L"UIEvents") == 0)
	{
		UIEvent* e = new UIEvent;
		return e;
	}
	else if (eventType.Compare(L"SVGZoomEvents") == 0)
	{
		ZoomEvent* e = new ZoomEvent;
		return e;
	}
	else
	{
		Event* e = new Event;
		return e;
	}

	return nullptr;
}

IEventListenerGroup* DocumentEvent::createEventListenerGroup()
{
	return nullptr;
}

}	// Web
}
