#include "stdafx.h"
#include "LXML2.h"
#include "SMILTime.h"

//#include "LDOMElementImpl.h"
//#include "LDOMDocumentImpl.h"

namespace System
{
namespace Web
{

/////////////////////////////////////////////////////////////////////////////
// SMILTime

/*
Strip any leading, trailing, or intervening white space characters. 
If the value begins with a number or numeric sign indicator (i.e. '+' or '-'), the value should be parsed as an offset value. 
Else if the value begins with the unescaped token "prev", it should be parsed as a syncToPrev-value. 
Else if the value begins with the unescaped token "wallclock", it should be parsed as a wallclock-sync-value. 
Else if the value is the unescaped token "indefinite", it should be parsed as the value "indefinite". 
Else: Build a token substring up to but not including any sign indicator (i.e. strip off any offset, parse that separately, and add it to the result of this step). In the following, any '.' characters preceded by a backslash '\' escape character should not be treated as a separator, but as a normal token character. 
If the token contains no '.' separator character, then the value should be parsed as an event-value with an unspecified (i.e. default) eventbase-element. 
Else if the token ends with the unescaped string ".begin" or ".end", then the value should be parsed as a syncbase-value. 
Else if the token contains the unescaped string ".marker(", then the value should be parsed as a media-marker-value. 
Else, the value should be parsed as an event-value (with a specified eventbase-element).  
*/

double ParseClockValue(const WCHAR* s);
//CWCharString stripspaces(BSTR s);

SMILTime::SMILTime()
{
	m_resolved = false;
	m_resolvedOffset = 0;

	m_offset = 0;
	m_baseBegin = 0;

	m_ownerElement = NULL;
}

TimeType SMILTime::get_timeType() const
{
	return m_type;
}

/*
STDMETHODIMP SMILTime::set_timeType(LTimeType newVal)
{
	m_type = newVal;
	return S_OK;
}
*/

double SMILTime::get_offset()
{
	return m_offset;
}

void SMILTime::set_offset(double newVal)
{
	if (m_offset != newVal)
	{
		m_offset = newVal;

		if (m_type == SMIL_TIME_OFFSET)
		{
			m_resolvedOffset = m_offset;
		}

		if (m_offset <= 0)
		{
			ASSERT(0);
			MessageBeep(-1);
		}

		if (m_pListener)
			m_pListener->OnChanged(this);
	}
}

Element* SMILTime::get_baseElement()
{
	if (m_baseElement == NULL)
	{
		if (m_eventElementId.GetLength())
		{
			Document* ownerDocument = m_ownerElement->get_ownerDocument();

			m_baseElement = ownerDocument->getElementById(m_eventElementId);
		}
	}

	return m_baseElement;
}

void SMILTime::set_baseElement(Element *newVal)
{
	m_baseElement = newVal;
}

bool SMILTime::get_baseBegin()
{
	return m_baseBegin;
}

void SMILTime::set_baseBegin(bool newVal)
{
	m_baseBegin = newVal;
}

bool SMILTime::get_resolved() const
{
	return m_resolved;
}

double SMILTime::get_resolvedOffset() const
{
	return m_resolvedOffset;
}

String SMILTime::get_event() const
{
	return m_event;
}

void SMILTime::set_event(StringIn newVal)
{
	m_event = newVal;
}

String SMILTime::get_marker()
{
	ASSERT(0);

	return nullptr;
}

void SMILTime::set_marker(StringIn newVal)
{
	// TODO: Add your implementation code here
	ASSERT(0);
}

void SMILTime::resolveTimeEvent(double currentTime, Event* evt)
{
	if (m_event.GetLength() && m_eventElementId.GetLength())
	{
		String type = evt->get_type();

		EventTarget* target = evt->get_target();

		Element* element = dynamic_cast<Element*>(target);

		String t1 = element->getAttribute(WSTR("id"));
		if (t1.GetLength())
		{
			if (t1 == m_eventElementId)
			{
				if (type == m_event)
				{
					m_resolvedOffset = currentTime + m_offset;
					m_resolved = true;
				}
			}
		}
	}
}

void SMILTime::setTimeIndefinite()
{
	m_type = SMIL_TIME_INDEFINITE;
	m_resolved = false;	// ??

	if (m_pListener)
		m_pListener->OnChanged(this);
}

void SMILTime::setTimeOffset(double offset)
{
	m_type = SMIL_TIME_OFFSET;
	m_offset = offset;
	m_resolvedOffset = m_offset;
	m_resolved = true;	// ??

	if (m_pListener)
		m_pListener->OnChanged(this);
}

void SMILTime::setTimeSyncbased(StringIn baseElement, bool baseBegin, double offset)
{
	ASSERT(0);
//	return E_NOTIMPL;
}

//

bool SMILTime::parseString(StringIn s)
{
	m_baseElement = NULL;

	m_resolved = false;

	ASSERT(0);
#if 0

	String str = stripspaces(s);

//If the value begins with a number or numeric sign indicator (i.e. '+' or '-'), the value should be parsed as an offset value. 
	const WCHAR* p = str->c_str();

	if (std::isdigit(*p) || (*p == L'-') || (*p == L'+'))	// offset value
	{
		m_resolved = true;	// ??

		/*
		int sign = 1;

		if ((*p == L'-') || (*p == L'+'))
		{
			if (*p == L'-') sign = -1;
			p++;
		}
		*/

		m_type = SMIL_TIME_OFFSET;

		m_offset = ParseClockValue(p);
		m_resolvedOffset = m_offset;
	//	if (sign == -1) m_offset = -m_offset;
	}
	else if (!std::wcsncmp(p, L"prev", 4))	// syncToPrev-value
	{
		m_type = SMIL_TIME_SYNC_BASED;
		//TODO
	}
	else if (!std::wcsncmp(p, L"wallclock", 9))	// wallclock-sync-value
	{
		m_type = SMIL_TIME_WALLCLOCK;
		//TODO
	}
	else if (!std::wcsncmp(p, L"indefinite", 10))	// indefinite
	{
		m_type = SMIL_TIME_INDEFINITE;
	}
	else
	{
		WCHAR* punct;
		WCHAR token[256];

		int i = 0;
		while (*p/* && !iswspace(*p)*/)
		{
			if (*p == L'-' || *p == L'+')
			{
			//	p--;
				break;
			}

			if (*p == L'\\')	// Escape character
			{
				p++;
				if (*p == 0) break;
				//c = *p;
			}
			else
			{
				if (*p == L'.') punct = &token[i];
			}

			token[i] = *p;

			p++;
			i++;
		}

		token[i] = 0;	// null-terminate

		//while (*p && iswspace(*p)) p++;

		if (*p)
			m_offset = ParseClockValue(p);
		else
			m_offset = 0.0;

	// get id
		StringW* id;

		p = token;
		while (*p && *p != L'.')
		{
			ASSERT(0);
#if 0
			id += *p;
#endif
			p++;
		}

		m_eventElementId = id;

	// get event name
		StringW* event;

		p++;
		while (*p)
		{
			ASSERT(0);
#if 0
			event += *p;
#endif
			p++;
		}

		if (*event == L"begin" || *event == L"end")
		{
			if (*event == L"begin")
				m_baseBegin = true;
			else
				m_baseBegin = false;

			m_type = SMIL_TIME_SYNC_BASED;
		}
		else
		{
			m_type = SMIL_TIME_EVENT_BASED;
			m_event = event;
		}

	// TODO, do this later
		/*
		CComPtr<ILDOMElement> element;
		document->getElementById(m_eventElementId, &element);

		m_baseElement = element;
		*/
	}
#endif
	return true;
}

String SMILTime::ToString()
{
	if (m_type == SMIL_TIME_OFFSET)
	{
		WCHAR str[64];
		swprintf_s(str, L"%f", m_offset);
		return string_copy(str);
	}
	else if (m_type == SMIL_TIME_INDEFINITE)
	{
		return S("indefinite");
	}
	else
	{
		ASSERT(0);
		return nullptr;//sysstring();
	}
}

}	// Web
}
