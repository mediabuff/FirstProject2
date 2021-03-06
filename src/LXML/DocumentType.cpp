#include "stdafx.h"
#include "LXML2.h"
#include "DocumentType.h"

#include "ASDTDModel.h"
#include "DTDDocument.h"

#include "DTDDocument.h"
#include "DTDParser.h"

namespace System
{
namespace Web
{

CLDOMInternalSubsetNode::CLDOMInternalSubsetNode()
{
	m_ownerDoctype = NULL;
}

DocumentType::DocumentType()
{
	m_internalSubsetNode = NULL;

	m_dtd = NULL;
	//m_internalSubset = NULL;

	m_entities = new NamedNodeMap;
}

DocumentType::~DocumentType()
{
}

	/*
int DocumentType::FinalConstruct()
{
	HRESULT hr;

	hr = CLDOMNodeImpl<DocumentType>::FinalConstruct();
	if (FAILED(hr)) return hr;

	hr = CLDOMEventTargetImpl<DocumentType>::FinalConstruct();
	if (FAILED(hr)) return hr;

	hr = CComObject<CLDOMNamedNodeMap>::CreateInstance(&m_entities);
	if (FAILED(hr)) return hr;
	m_entities->AddRef();

	hr = CComObject<CLDOMInternalSubsetNode>::CreateInstance(&m_internalSubsetNode);
	if (FAILED(hr)) return hr;
	m_internalSubsetNode->AddRef();

	hr = CComObject<CDTDDocument>::CreateInstance(&m_dtd);
	if (FAILED(hr)) return hr;
	m_dtd->AddRef();

	return S_OK;
}

void DocumentType::FinalRelease()
{
	if (m_internalSubsetNode)
	{
		m_internalSubsetNode->Release();
		m_internalSubsetNode = NULL;
	}

	if (m_entities)
	{
		m_entities->Release();
		m_entities = NULL;
	}

	if (m_dtd)
	{
		m_dtd->Release();
		m_dtd = NULL;
	}

	CLDOMEventTargetImpl<DocumentType>::FinalRelease();
	CLDOMNodeImpl<DocumentType>::FinalRelease();
}
*/

NodeType DocumentType::get_nodeType() const
{
	return NODE_DOCUMENT_TYPE;
}

Node* DocumentType::cloneNodeExport(Document* ownerDocument, bool deep) const
{
	ASSERT(0);
	return NULL;
}

// virtual
bool DocumentType::dispatchEvent2(Event* evt, bool bCapture)
{
	String type = evt->get_type();

	if (evt->get_eventPhase() == AT_TARGET)
	{
		if (type == L"DOMNodeInsertedIntoDocument")
		{
#if 0
			{
				// External subset
				if (m_systemId.Length())
				{
					sysstring documentUrl = m_ownerDocument->get_url();

					TCHAR result[2048];
					{
						DWORD resultLen = sizeof(result);
						InternetCombineUrl(W2A(documentUrl), W2A(m_systemId), result, &resultLen, 0);
					}

					CDTDParser dtdParser;
					dtdParser.m_parentNode = m_dtd;
					dtdParser.m_dtdDocument = m_dtd;
					m_ownerDocument->get_errorHandler(&dtdParser.m_errorHandler);// = m_errorHandler;
					//dtdParser.m_url = _bstr_t(documentUrl);

					WCHAR* absurl = A2W(result);

					TRACE("loading external subset: %s...", result);
					HRESULT hr = dtdParser.loadURL(absurl);
					if (FAILED(hr))
					{
#if 0	// Have this?
						WCHAR msg[512];
						swprintf(msg, L"Failed to load external subset url: %s", absurl);

						if (!handleError(CreateDOMError(LSEVERITY_ERROR, msg, getCurrentLocation())))
						{
							throw msg;
						}
#endif
					}
				}

				// Build and parse general entities
				{
					for (int i = 0; i < m_dtd->m_entityList.GetSize(); i++)
					{
						CEntity* pDTDEntity = m_dtd->m_entityList[i];

						if (pDTDEntity->m_c == 0)
						{
							CComPtr<ILDOMDocumentFragment> fragment;
							m_ownerDocument->parseFragment(pDTDEntity->m_value, NULL/*hm ??*/, &fragment);

							if (fragment)
							{
								CComObject<CLDOMEntity>* pEntity;
								CComObject<CLDOMEntity>::CreateInstance(&pEntity);
								if (pEntity)
								{
									pEntity->AddRef();

									pDTDEntity->m_domEntity = pEntity;
									pEntity->m_pDTDEntity = pDTDEntity;

									pEntity->m_name = pDTDEntity->m_name;
									pEntity->appendChild(fragment, NULL);

									CComPtr<ILDOMNode> p;
									m_entities->setNamedItem(pEntity, &p);

									pEntity->Release();
								}
							}
						}
					}
				}
			}

			CComObject<CASDTDModel>* pModel;
			CComObject<CASDTDModel>::CreateInstance(&pModel);
			if (pModel)
			{
				pModel->AddRef();

				pModel->BuildFromDTDDocument(m_dtd);

				CComQIPtr<ILDocumentAS> documentAS = m_ownerDocument;
				if (documentAS)
				{
					documentAS->set_activeASModel(pModel);
				}

				pModel->Release();
			}
#endif	// _WINDOWS
		}
		else if (type == L"DOMNodeRemovedFromDocument")
		{
#if 0
			CComQIPtr<ILDocumentAS, &IID_ILDocumentAS> documentAS((IUnknown*)m_ownerDocument);
			if (documentAS)
			{
				documentAS->set_activeASModel(NULL);
			}
#endif
		}
	}

	return CLDOMEventTargetImpl::dispatchEvent2(evt, bCapture);
}

String DocumentType::get_name() const
{
	return m_name;
}

void DocumentType::set_name(StringIn name)
{
	m_name = name;
}

String DocumentType::get_publicId() const
{
	return m_publicId;
}

void DocumentType::set_publicId(StringIn newVal)
{
	m_publicId = newVal;
}

String DocumentType::get_systemId() const
{
	return m_systemId;
}

void DocumentType::set_systemId(StringIn newVal)
{
	m_systemId = newVal;
}

String DocumentType::get_internalSubset()
{
	THROW(-1);
	return nullptr;
}

void DocumentType::set_internalSubset(StringIn newVal)
{
	VERIFY(0);
}

NamedNodeMap* DocumentType::get_entities()
{
	return m_entities;
}

NamedNodeMap* DocumentType::get_notations()
{
	ASSERT(0);
	return NULL;
}

#if 0
STDMETHODIMP DocumentType::get_internalSubsetNode(/*[out, retval]*/ ILDOMInternalSubsetNode* *pVal)
{
	ASSERT(pVal != NULL);
	if (pVal == NULL) return E_POINTER;
	ASSERT(0);
//	*pVal = m_internalSubsetNode;
	//if (*pVal) (*pVal)->AddRef();
	return S_OK;
}

STDMETHODIMP DocumentType::get_internalDTD(ILDTDDocument **pVal)
{
	*pVal = NULL;
	//ASSERT(0);
#if 0
	ASSERT(pVal != NULL);
	if (pVal == NULL) return E_POINTER;
	*pVal = m_internalSubset;
	if (*pVal) (*pVal)->AddRef();
#endif
	return S_OK;
}

#endif

}	// w3c
}
