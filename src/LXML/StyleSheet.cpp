#include "stdafx.h"
#include "LXML.h"
//#include "StyleSheet.h"
//#include "MediaList.h"

namespace System
{
namespace Web
{

StyleSheet::StyleSheet(Node* ownerNode)
{
	m_parentStyleSheet = nullptr;
	m_ownerNode = ownerNode;
	m_href = nullptr;
	m_disabled = false;
	m_media = new MediaList;
}

#if 0
int StyleSheetImplImpl::FinalConstruct()
{
	HRESULT hr;

	/*
	hr = CComObject<MediaList>::CreateInstance(&m_media);
	if (FAILED(hr)) return hr;
//	m_media->AddRef();
*/

	return 0;
}

void StyleSheetImplImpl::FinalRelease()
{
	if (m_media)
	{
		m_media->m_pListener = NULL;
//		m_media->Release();
		m_media = NULL;
	}
}
#endif

String StyleSheet::get_type() const
{
	// Must override this
	THROW(-1);
	return nullptr;
}

String StyleSheet::get_title() const
{
	ASSERT(0);
	return nullptr;
}

void StyleSheet::set_title(StringIn newVal)
{
	ASSERT(0);
}

bool StyleSheet::get_disabled() const
{
	return m_disabled;
}

#if 0	// had this
HRESULT StyleSheet::get_ownerNode(ILDOMNode **pVal)
{
	ASSERT(pVal != NULL);
	if (pVal == NULL) return E_POINTER;
	*pVal = m_ownerNode;
	if (*pVal) (*pVal)->AddRef();
	return S_OK;
}
#endif

StyleSheet* StyleSheet::get_parentStyleSheet()
{
	return m_parentStyleSheet;
}

String StyleSheet::get_href() const
{
	return m_href;
}

MediaList* StyleSheet::get_media()
{
	return m_media;
}

}	// Web
}	// System
