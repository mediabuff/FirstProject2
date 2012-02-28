#include "stdafx.h"
#include "LXML2.h"

#include "PTextNode.h"

namespace System
{
namespace Web
{

CharacterData::CharacterData(PCharacterData* pNode) : ChildNode(pNode)
{
}

String CharacterData::get_textContent()
{
	return get_data();
}

unsigned int CharacterData::get_length() const
{
	return m_data.GetLength();
}

String CharacterData::get_data()
{
	return m_data;
}

void CharacterData::set_data(StringIn newVal)
{
	String prevValue = m_data;
	//prevValue.Attach(m_data.Detach());

	m_data = newVal;

	if (true)
	{
			ASSERT(0);
#if 0
		for (int i = 0; i < m_pNodes.GetSize(); i++)
		{
			((CPTextNodeBase*)m_pNodes[i])->OnTextContentChanged();
		}
#endif
	}
	else	// TODO remove
	{
#if 0
		CComQIPtr<INotifySend, &IID_INotifySend> cp = thisNode;
		if (cp)
		{
			cp->FireOnChanged(NOTIFY_MODIFY, thisNode, DISPID_UNKNOWN);
		}
#endif
	}

//	CComPtr<ILDOMDocument> ownerDocument;
//	thisNode->get_ownerDocument(&ownerDocument);
//	ASSERT(ownerDocument != NULL);

////////////////////////////////
// create an event notification
	DocumentEvent* ownerDocumentEvent = dynamic_cast<DocumentEvent*>(m_ownerDocument);
	ASSERT(ownerDocumentEvent != nullptr);

	MutationEvent* event = dynamic_cast<MutationEvent*>(ownerDocumentEvent->createEvent(S("MutationEvent")));

	event->initMutationEvent(S("DOMCharacterDataModified"), true, false, nullptr, prevValue, m_data, nullptr, CHANGE_UNKNOWN);

	EventTarget* targetNode = this;

	targetNode->dispatchEvent(event);
}

String CharacterData::substringData(unsigned int offset, unsigned int count) const
{
	ASSERT(0);
	return nullptr;
//	*result = SysAllocStringLen(&((BSTR)m_data)[offset], count);
//	return S_OK;
}

void CharacterData::appendData(StringIn arg)
{
	ASSERT(0);
}

void CharacterData::insertData(unsigned int offset, StringIn arg)
{
//	ASSERT(arg != null);
	if (arg == nullptr) THROW(-1);
	//if ((int)offset < 0) THROW(-1);

	String prevValue = m_data;

	ASSERT(0);
#if 0

	m_data = new StringW(prevValue->c_str(), offset);
	m_data += arg;
	m_data += prevValue.c_str()+offset;
#endif

#if 0

	int insertedLen = wcslen(arg);//SysStringLen(arg);
	BSTR newString = SysAllocStringLen(NULL, m_data.length() + insertedLen);

	CopyMemory(newString, (BSTR)m_data, offset*2);
	CopyMemory(newString + offset, arg, insertedLen*2);
	CopyMemory(newString + (offset+insertedLen), ((BSTR)m_data) + offset, (m_data.Length()-offset)*2);

	CComBSTR prevValue;
	prevValue.Attach(m_data.Detach());

	m_data.Attach(newString);
#endif

	// Update document ranges
	{
		DocumentRangeImplImpl* pDocumentRange = dynamic_cast<DocumentRangeImplImpl*>(m_ownerDocument);
		if (pDocumentRange)
		{
			for (size_t i = 0; i < pDocumentRange->m_ranges.GetSize(); i++)
			{
				Range* pRange = pDocumentRange->m_ranges[i];

			// start boundary point
				if (pRange->m_startContainer == this)
				{
					if (offset < pRange->m_startOffset)
					{
						pRange->m_startOffset += arg.GetLength();
					}
				}

			// end boundary point
				if (pRange->m_endContainer == this)
				{
					if (offset < pRange->m_endOffset)
					{
						pRange->m_endOffset += arg.GetLength();
					}
				}
			}
		}
	}

	if (m_pNode)
	{
		((PText*)m_pNode)->OnTextContentChanged();
		m_ownerDocument->m_pWindow->Flow();
		ASSERT(0);
#if 0
		m_ownerDocument->m_pWindow->Invalidate();
#endif
	}

#if 0	// TODO ??? Had this (TODO, should be removed)
	CComQIPtr<INotifySend, &IID_INotifySend> cp = thisNode;
	if (cp)
	{
		cp->FireOnChanged(NOTIFY_MODIFY, thisNode, DISPID_UNKNOWN);
	}
#endif

////////////////////////////////
// create an event notification
	DocumentEvent* ownerDocumentEvent = dynamic_cast<DocumentEvent*>(m_ownerDocument);
	ASSERT(ownerDocumentEvent != NULL);

	MutationEvent* event = dynamic_cast<MutationEvent*>(ownerDocumentEvent->createEvent(S("MutationEvent")));
	event->initMutationEvent(S("DOMCharacterDataModified"), true, false, nullptr, prevValue, m_data, nullptr, CHANGE_UNKNOWN);
	dispatchEvent(event);

//
//	event->initMutationEvent(OLESTR("DOMSubtreeModified"), TRUE, FALSE, thisNode, NULL, NULL, NULL, CHANGE_UNKNOWN);
//	dispatchEvent(event, &doDefault);
}

void CharacterData::deleteData(unsigned int offset, unsigned int count)
{
	ASSERT(0);
#if 0

//	ASSERT(count >= 0);
//	ASSERT(offset >= 0);
	ASSERT(offset <= m_data.GetLength());

//	if (count < 0) THROW(-1);
//	if (offset < 0) THROW(-1);
	if (offset > m_data.GetLength()) THROW(-1);

	if (count == 0) return;	// No need to do anything

	String prevValue = m_data;
//	prevValue.Attach(m_data.Detach());

	size_t length = m_data.GetLength();



	m_data = new string_alloc<WCHAR>(length-count));

	std::memcpy(m_data->begin(), prevValue->begin(), offset*sizeof(WCHAR));
	std::memcpy(m_data->begin() + offset, prevValue->begin() + offset+count, (length - count - offset)*sizeof(WCHAR));

//	m_data = sysstring(prevValue.c_str(), offset);
//	m_data += prevValue.c_str()+offset+count;

	// Update document ranges
	{
		DocumentRangeImplImpl* pDocumentRange = dynamic_cast<DocumentRangeImplImpl*>(m_ownerDocument);
		if (pDocumentRange)
		{
			for (int i = 0; i < pDocumentRange->m_ranges.GetSize(); i++)
			{
				Range* pRange = pDocumentRange->m_ranges[i];

			// start boundary point
				if (pRange->m_startContainer == this)
				{
					if (pRange->m_startOffset >= offset && pRange->m_startOffset < offset+count)
					{
						pRange->m_startOffset = offset;
					}
					else if (pRange->m_startOffset >= offset+count)
					{
						pRange->m_startOffset -= count;
					}

					ASSERT(pRange->m_startOffset >= 0);
				}

			// end boundary point
				if (pRange->m_endContainer == this)
				{
					if (pRange->m_endOffset >= offset && pRange->m_endOffset < offset+count)
					{
						pRange->m_endOffset = offset;
					}
					else if (pRange->m_endOffset >= offset+count)
					{
						pRange->m_endOffset -= count;
					}

					ASSERT(pRange->m_endOffset >= 0);
				}
			}
		}
	}

	/*
	CComQIPtr<INotifySend, &IID_INotifySend> cp = thisNode;
	if (cp)
	{
		cp->FireOnChanged(NOTIFY_MODIFY, thisNode, DISPID_UNKNOWN);
	}
	*/

#if 0	// TODO
	if (m_pNode)
	{
		((PText*)m_pNode)->OnTextContentChanged();
		m_ownerDocument->m_pWindow->Flow();
		m_ownerDocument->m_pWindow->Invalidate();
	}

////////////////////////////////
// create an event notification
	DocumentEvent* ownerDocumentEvent = dynamic_cast<DocumentEvent*>(m_ownerDocument);
	ASSERT(ownerDocumentEvent != NULL);

	MutationEvent* event = dynamic_cast<MutationEvent*>(ownerDocumentEvent->createEvent(WSTR("MutationEvent")));
	event->initMutationEvent(WSTR("DOMCharacterDataModified"), true, false, NULL, prevValue, m_data, NULL, CHANGE_UNKNOWN);
	dispatchEvent(event);

#endif

//////////////

#if 0
/*
// Fire event
	{
		CComPtr<IDOMDocumentContainer> documentContainer;
		ownerDocument->get_documentContainer(&documentContainer);

		CComPtr<IDOMNode> thisNode;
		QueryInterface(IID_IDOMNode, (void**)&thisNode);

		if (documentContainer)
			((CDOMDocumentContainer*)documentContainer.p)->Fire_changedTextContentNode(thisNode, prevValue);
	}
*/
////////////////////////////////
// create an event notification

	CComQIPtr<ILDOMDocumentEvent, &IID_ILDOMDocumentEvent> ownerDocumentEvent((IUnknown*)ownerDocument);
	ASSERT(ownerDocument != NULL);

	CComPtr<ILDOMMutationEvent> event;
	ownerDocumentEvent->createEvent(OLESTR("MutationEvent"), (ILDOMEvent**)&event);

//
	event->initMutationEvent(OLESTR("DOMCharacterDataModified"), VARIANT_TRUE, VARIANT_FALSE, NULL, prevValue, m_data, NULL, CHANGE_UNKNOWN);

	CComQIPtr<ILDOMEventTarget, &IID_ILDOMEventTarget> targetNode = thisNode;

	bool doDefault;
	targetNode->dispatchEvent(event, &doDefault);

//
//	event->initMutationEvent(OLESTR("DOMSubtreeModified"), TRUE, FALSE, thisNode, NULL, NULL, NULL, CHANGE_UNKNOWN);
//	dispatchEvent(event, &doDefault);
#endif
#endif
}

void CharacterData::replaceData(unsigned int offset, unsigned int count, StringIn arg)
{
	ASSERT(0);
}

}	// Web
}	// System
