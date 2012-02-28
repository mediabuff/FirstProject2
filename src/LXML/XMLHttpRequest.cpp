#include "stdafx.h"
#include "LXML2.h"

namespace System
{
namespace Web
{

XMLHttpRequest::XMLHttpRequest() :
	m_readyState(ReadyState_UNSENT)
{
}

IFunction* XMLHttpRequest::get_onreadystatechange()
{
	return m_onreadystatechange;
}

void XMLHttpRequest::set_onreadystatechange(IFunction* function)
{
	m_onreadystatechange = function;
//	m_request->m_stateChanged.Connect(function);
//	onreadystatechange
}

XMLHttpRequest::ReadyState XMLHttpRequest::get_readyState()
{
	return m_readyState;
}

// request
void XMLHttpRequest::open(StringIn method, StringIn url)
{
	if (m_conn == NULL)
	{
		m_conn = new Net::HttpConnection("www.microsoft.com");
	}

	m_request = m_conn->OpenRequest(method, url);
	m_readyState = ReadyState_OPENED;
}

void XMLHttpRequest::open(StringIn method, StringIn url, boolean async)
{
}

void XMLHttpRequest::open(StringIn method, StringIn url, boolean async, StringIn user)
{
}

void XMLHttpRequest::open(StringIn method, StringIn url, boolean async, StringIn user, StringIn password)
{
}

void XMLHttpRequest::setRequestHeader(StringIn header, StringIn value)
{
	if (m_readyState != ReadyState_OPENED)
	{
		raise(DOMException(INVALID_STATE_ERR));
	}

	m_request->AddHeader(header, value);
}

void XMLHttpRequest::send()
{
	if (m_readyState != ReadyState_OPENED)
	{
		raise(DOMException(INVALID_STATE_ERR));
	}

	m_request->Send();
/*
	uint8 buf[512];
	while (1)
	{
		ULONG nRead = m_request->GetContentStream()->Read(buf, 512);
		if (nRead > 0)
		{
			//fwrite(buf, 1, nRead, tempfile);
		}
		if (nRead < 512)
			break;
	}
*/
}

void XMLHttpRequest::send(Document* data)
{
}

void XMLHttpRequest::send(StringIn data)
{
}

//	void send([AllowAny] DOMString? data);
void XMLHttpRequest::abort()
{
}

// response
unsigned short XMLHttpRequest::get_status()
{
	return m_status;
}

String XMLHttpRequest::get_statusText()
{
	if (m_readyState == ReadyState_UNSENT || m_readyState == ReadyState_OPENED)
	{
		return nullptr;
	}

	return m_request->m_status;
}

String XMLHttpRequest::getResponseHeader(StringIn header)
{
	if (m_readyState == ReadyState_UNSENT || m_readyState == ReadyState_OPENED)
	{
		return nullptr;
	}

	return m_request->m_responseHeaders.GetHeaderValue(header);
}

String XMLHttpRequest::getAllResponseHeaders()
{
	if (m_readyState == ReadyState_UNSENT || m_readyState == ReadyState_OPENED)
	{
		return nullptr;
	}

	return m_request->m_responseHeaders.GetAllHeaders();
}

String XMLHttpRequest::get_responseText()
{
	return m_responseText;
}

Document* XMLHttpRequest::get_responseXML()
{
	return m_responseXML;
}

}	// Web
}	// System
