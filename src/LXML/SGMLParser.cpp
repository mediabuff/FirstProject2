#include "stdafx.h"
#include "LXML.h"
#include "LXML2.h"

#if 0

#include "../LXUI/http.h"

#include "SGMLParser.h"

#include "LDOMDocumentImpl.h"
#include "LDOMElementImpl.h"
#include "LDOMDocument.h"
#include "ScriptSite.h"
#include "LDocumentStyleImpl.h"

#include "LDOMDocumentType.h"
#include "LDOMNamedNodeMap.h"
#include "LDOMEntity.h"

//#include "HTMControlFrameBase.h"
//#include "LHTMLWindow.h"
#include "LBoxWindow.h"
#include "LXMLDocumentView.h"

#include "LDOMError.h"
#include "LDOMLocator.h"

#include "DTDParser.h"
#include "DTDDocument.h"
#include "ASDTDModel.h"

namespace LXML
{

CLDOMError* CreateDOMError(DOMErrorSeverity severity, const sysstring& message, IDOMLocator* location);

/////////////////////////////////////////////////////////////////////////////
// SGMLParser

SGMLParser::SGMLParser()
{
	m_errorHandler = NULL;

//	m_textLen = 0L;
//	m_textData = NULL;

//	m_fpos = 0;
	m_line = 0;
	m_column = 0;
//	m_oldc = EOF;

	m_utf8 = false;
}

void SGMLParser::CollectSpaces(sysstring& spaces)
{
	spaces = OLESTR("");

	while (!eof())
	{
		int c = getcnext();

		if (!iswspace(c))
		{
			ungetcnext();
			break;
		}

		spaces += c;
	}
}

sysstring SGMLParser::GetID()
{
	sysstring str = OLESTR("");

	while (!eof())
	{
		WCHAR c = getcnext();

		if (!(iswalnum(c) || c == L':' || c == L'.' || c == L'-' || c == L'_'))
		{
			ungetcnext();
			break;
		}

		str += c;
	}

	if (str.length() == 0)
	{
		OLECHAR* msg = OLESTR("Expected ident");
		handleError(CreateDOMError(LSEVERITY_FATAL_ERROR, msg, getcnexturrentLocation()));
		throw msg;
	}

	return str;
}

void SGMLParser::GetNSID(sysstring& ns, sysstring& id)
{
	ns = OLESTR("");
	id = OLESTR("");

	sysstring str = OLESTR("");

	while (!eof())
	{
		OLECHAR c = getcnext();
		if (!(iswalnum(c) || c == ':' || c == '.' || c == '-' || c == '_'))
		{
			ungetcnext();
			break;
		}

		if (c == L':')
		{
			ns = str;
			str = OLESTR("");
		}
		else
		{
			str += c;
		}
	}

	id = str;

//	ungetcnext();
}

sysstring SGMLParser::GetString()
{
	SkipSpaces();

	int delimiter = getcnext();

	if (delimiter != L'\"' && delimiter != L'\'')
	{
		OLECHAR msg[64];
		swprintf(msg, OLESTR("Expected \" or ' at offset %d"), m_fpos);

		handleError(CreateDOMError(LSEVERITY_FATAL_ERROR, msg, getcnexturrentLocation()));
		throw msg;
	}

	sysstring str = OLESTR("");
	OLECHAR c;

	while (!eof())
	{
		c = getcnext();
		if (c == delimiter)
		{
			ungetcnext();
			break;
		}

		str += c;
	}

	EatChar(delimiter);
	SkipSpaces();

	return str;
}

void SGMLParser::GetAttrValue(Attr* attr)
{
	DocumentType* doctype = m_document->get_doctype();

	NamedNodeMap* entities = NULL;
	if (doctype)
	{
		entities = doctype->get_entities();
	}

//	SkipSpaces();

	int delimiter = getcnext();

	if (delimiter != L'\"' && delimiter != L'\'')
	{
		delimiter = 0;
		ungetcnext();
	}

	attr->setTextOffset(3, m_fpos);

	if (true)
	{
		if (delimiter == 0)
		{
			OLECHAR msg[64];
			swprintf(msg, OLESTR("Expected \" or '"));

			handleError(CreateDOMError(LSEVERITY_FATAL_ERROR, msg, getcnexturrentLocation()));
			throw msg;
		}
	}

	//CWCharString str = OLESTR("";
	OLECHAR c;

	sysstring str;

	while (!eof())
	{
		c = getcnext();

		if (delimiter == 0)
		{
			if (iswspace(c) || c == L'>')
			{
				ungetcnext();
				break;
			}
		}
		else
		{
			if (c == delimiter)
			{
				ungetcnext();
				break;
			}
		}

		if (c == L'&')
		{
			sysstring name = OLESTR("");

			bool bNumber = false;
			int c = getcnext();
			if (c == '#')
			{
				bNumber = true;
			}
			else
				ungetcnext();

			while (!eof())
			{
				c = getcnext();

				if (!iswalnum(c) && c != L'_') break;
				//if (c == L' ' || c == L'\t' || c == L'\n' || c == L'\r') break;
				//if (c == L'<') break;

				if (c == L';')
				{
				//	ungetcnext();
					break;
				}

				name += c;
			}

			if (c == L';')
			{
				WCHAR uc;

				if (bNumber)
					uc = str2int(name.c_str());
				else
					uc = GetUCode(name.c_str());

				if (uc)
				{
					str += uc;
				}
				else
				{
					if (entities)
					{
						if (true)	// Use entity references
						{
							if (str.length())	// Create a text node for collected characters
							{
								Text* textNode = m_document->createTextNode(str);
								attr->appendChild(textNode);

								str = OLESTR("");
							}

							EntityReference* entityReference = m_document->createEntityReference(name);

							attr->appendChild(entityReference);
						}
						else
						{
							Node* entity = entities->getNamedItem(name.c_str());
							if (entity)
							{
								Entity* pEntity = dynamic_cast<Entity*>(entity);

								str += pEntity->m_value;
							}
							else
							{
								OLECHAR msg[256];
								swprintf(msg, OLESTR("Undeclared entity '%s'"), name.c_str());

								handleError(CreateDOMError(LSEVERITY_FATAL_ERROR, msg, getcnexturrentLocation()));
								throw msg;
							}
						}
					}
				}
			}
			else
			{
				str += L'&';
				str += name;
				str += c;
			}
		}
		else
		{
			str += c;
		}
	}

	if (str.length())	// Create a text node for collected characters
	{
		Text* textNode = m_document->createTextNode(str);
		attr->appendChild(textNode);

		str = OLESTR("");
	}

//	attr->set_value(str);

	attr->setTextOffset(4, m_fpos);

	if (delimiter) EatChar(delimiter);
//	SkipSpaces();
}

void SGMLParser::SkipSpaces()
{
	while (!eof())
	{
		int c = getcnext();

		if (!iswspace(c))
		{
			ungetcnext();
			break;
		}
	}
}

void SGMLParser::EatChar(int c)
{
	int c2 = getcnext();
	if (c2 != c)
	{
		static OLECHAR msg[64];
		swprintf(msg, OLESTR("Expected %c"), c);

		handleError(CreateDOMError(LSEVERITY_FATAL_ERROR, msg, getcnexturrentLocation()));
		throw msg;
	}
}

ProcessingInstruction* SGMLParser::ParseProcessingInstruction()
{
// <? has been read

	int fpos0 = m_fpos;

	sysstring target = GetID();

	int fpos1 = m_fpos;

	SkipSpaces();

	int fpos2 = m_fpos;

	sysstring data;

	int c;

	while (!eof())
	{
		c = getcnext();
		if (c == L'?')
		{
			ungetcnext();
			break;
		}

		if (m_document)
		{
			data += c;
		}
	}

	int fpos3 = m_fpos;

	EatChar('?');
	EatChar('>');

	if (m_document)
	{
		ProcessingInstruction* pi = m_document->createProcessingInstruction(target, data);
		if (pi)
		{
			pi->setTextOffset(0, fpos0-2);
			pi->setTextOffset(1, fpos0);	// start target
			pi->setTextOffset(2, fpos1);	// end of target

			pi->setTextOffset(3, fpos2);	// start of data
			pi->setTextOffset(4, fpos2);
			pi->setTextOffset(5, fpos3);
			pi->setTextOffset(6, fpos3);	// end of data

			pi->setTextOffset(7, m_fpos-2);
			pi->setTextOffset(8, m_fpos);		// after ?>

			pi->setTextOffset(9, m_fpos);	// TODO
		}

		return pi;
	}
	else
		return NULL;
}

Comment* SGMLParser::ParseCommentNode()
{
	int offset1 = m_fpos;
	int offset2;

	sysstring data;

	int c;

	while (!eof())
	{
		offset2 = m_fpos;

		c = getcnext();
		if (c == L'-')
		{
			if (!eof())
			{
				int c2 = getcnext();
				if (c2 == L'-')
				{
					break;
				}
				ungetcnext();
			}
		}

		if (m_document)
		{
			data += c;
		}
	}

	EatChar('>');

	int offset3 = m_fpos;

	if (m_document)
	{
		Comment* comment = m_document->createComment(data);

		comment->setTextOffset(0, offset1-4);	// <!--

		comment->setTextOffset(1, offset1);
		comment->setTextOffset(2, offset1);

		comment->setTextOffset(3, offset2);
		comment->setTextOffset(4, offset2);

		comment->setTextOffset(5, offset3);
		comment->setTextOffset(6, offset3);
		comment->setTextOffset(7, offset3);
		comment->setTextOffset(8, offset3);
		comment->setTextOffset(9, offset3);

		return comment;
	}
	else
		return NULL;
}

void SGMLParser::ParseText(Node* parentNode, sysstring& data)
{
	int c;

	int startoffset = m_fpos-data.length();

	while (!eof())
	{
		c = getcnext();
		if (c == L'<')
		{
			ungetcnext();
			break;
		}

		if (c == '&')
		{
			sysstring t = OLESTR("");

			int c;
			while (!eof())
			{
				c = getcnext();
				if (c == ';') break;

				if (c == ' ' || c == '\t' || c == '\n' || c == '\r') break;
				if (c == '<') break;

				t += c;
			}

		// TODO?, should make sure there's a ';'

			WCHAR uc = GetUCode(t.c_str());
			if (uc)
			{
				data += uc;
			}
			else
			{
				if (data.length())	// Create a text node for collected characters
				{
				// TODO: text Offsets
					Text* textNode = m_document->createTextNode(data);

					textNode->setTextOffset(0, startoffset);
					textNode->setTextOffset(1, startoffset);
					textNode->setTextOffset(2, startoffset);
					textNode->setTextOffset(3, startoffset+data.length());
					textNode->setTextOffset(4, startoffset+data.length());
					textNode->setTextOffset(5, startoffset+data.length());
					textNode->setTextOffset(6, startoffset+data.length());
					textNode->setTextOffset(7, startoffset+data.length());
					textNode->setTextOffset(8, startoffset+data.length());
					textNode->setTextOffset(9, startoffset+data.length());

					parentNode->appendChild(textNode);

					data = OLESTR("");
					startoffset = m_fpos;	// Reset for next text node
				}

				EntityReference* entityRef = m_document->createEntityReference(t);

				if (entityRef)
				{
					parentNode->appendChild(entityRef);
				}
			}
		}
		else
		{
			data += c;
		}
	}

	if (data.length())	// Create a text node for collected characters
	{
		Text* textNode = m_document->createTextNode(data);

		textNode->setTextOffset(0, startoffset);
		textNode->setTextOffset(1, startoffset);
		textNode->setTextOffset(2, startoffset);
		textNode->setTextOffset(3, startoffset+data.length());
		textNode->setTextOffset(4, startoffset+data.length());
		textNode->setTextOffset(5, startoffset+data.length());
		textNode->setTextOffset(6, startoffset+data.length());
		textNode->setTextOffset(7, startoffset+data.length());
		textNode->setTextOffset(8, startoffset+data.length());
		textNode->setTextOffset(9, startoffset+data.length());

		parentNode->appendChild(textNode);

		data = OLESTR("");
	}
}

CDATASection* SGMLParser::ParseCDATASection()
{
	int offset0 = m_fpos-8;

	SkipSpaces();
	EatChar('[');
	SkipSpaces();

	sysstring data;

	int offset1 = m_fpos;

	while (!eof())
	{
		int startpos = m_fpos;

		if (getcnext() == L']' &&
			getcnext() == L']' &&
			getcnext() == L'>')
		{
		// End of cdata
			break;
		}

		int endpos = m_fpos;

		for (int i = startpos; i < endpos; i++)
		{
			//char c[2] = {0};
			//c[0] = textData[i];
			ASSERT(0);
#if 0
			data += m_textData[i];//_bstr_t(c);
#endif
		}
	}

	if (data.length() > 0)
	{
		CDATASection* node = m_document->createCDATASection(data);

	// <![CDATA[
		node->setTextOffset(0, offset0);
		node->setTextOffset(1, offset0);
		node->setTextOffset(2, offset0);

	// Text content
		node->setTextOffset(3, offset1);
		node->setTextOffset(4, offset1);
		node->setTextOffset(5, m_fpos-3);
		node->setTextOffset(6, m_fpos-3);

	// ]]>
		node->setTextOffset(7, m_fpos);
		node->setTextOffset(8, m_fpos);
		node->setTextOffset(9, m_fpos);

		return node;
	}

	return NULL;
}

void SGMLParser::ParseAttributes(Node* parent, NamedNodeMap* attributes)
{
	int c;

	while (!eof())
	{
		int offset0 = m_fpos;

		sysstring spaces;
		CollectSpaces(spaces);

		c = getcnext();

		if (!iswalpha(c))
		{
			ungetcnext();
			for (int i = 0; i < spaces.length(); i++)
			{
				ungetcnext();
			}
			break;
		}

		ungetcnext();

		int offset1 = m_fpos;

	//	sysstring attrPrefix;
		sysstring attrLocalName = attrLocalName = GetID();

		if (attrLocalName.length() == 0)
		{
			OLECHAR* msg = OLESTR("Error parsing attributes");
			handleError(CreateDOMError(LSEVERITY_FATAL_ERROR, msg, getcnexturrentLocation()));
			throw msg;
		}

	//	sysstring qualifiedName = attrPrefix.length()? attrPrefix + OLESTR(":") + attrLocalName: attrLocalName;

#if 0
		sysstring namespaceURI;

		if (attrPrefix.length())
		{
			if (!wcscmp(attrPrefix.c_str(), OLESTR("xml")))
			{
	// ??
				namespaceURI = OLESTR("http://www.w3.org/XML/1998/namespace");
			}
			else if (!wcscmp(attrPrefix.c_str(), OLESTR("xmlns")))
			{
				namespaceURI = OLESTR("");	// no namespace
			}
			else
			{
				namespaceURI = FindNamespaceURI(parent, attributes, attrPrefix.c_str());
				if (namespaceURI.length() == 0)
				{
					OLECHAR msg[256];
					swprintf(msg, OLESTR("Namespace %s not found for attribute"), attrPrefix.c_str());

					handleError(CreateDOMError(LSEVERITY_FATAL_ERROR, msg, getcnexturrentLocation()));
					throw msg;
				}
			}
		}
#endif

		Attr* pAttr = m_document->createAttribute(attrLocalName);
		if (pAttr == NULL)
		{
			OLECHAR msg[64];
			swprintf(msg, OLESTR("Invalid attribute name %s"), attrLocalName.c_str());

			handleError(CreateDOMError(LSEVERITY_FATAL_ERROR, msg, getcnexturrentLocation()));
			throw msg;
		}

		pAttr->setTextOffset(0, offset0);	// space before attr name
		pAttr->setTextOffset(1, offset1);	// start of attr name
		pAttr->setTextOffset(2, m_fpos);		// end of attr name

		SkipSpaces();

		c = getcnext();

		if (c == L'=')
		{
			SkipSpaces();
			GetAttrValue(pAttr);
		}
		else
		{
			if (true)	// xml requires attribute value
			{
				OLECHAR msg[64];
				swprintf(msg, OLESTR("Expected ="));

			// However we only throw an error and not a fatalError
				if (!handleError(CreateDOMError(LSEVERITY_ERROR, msg, getcnexturrentLocation())))
				{
					throw msg;
				}
			}
			ungetcnext();

			pAttr->setTextOffset(3, m_fpos);
			pAttr->setTextOffset(4, m_fpos);
		}

		pAttr->setTextOffset(5, m_fpos);
		pAttr->setTextOffset(6, m_fpos);
		pAttr->setTextOffset(7, m_fpos);
		pAttr->setTextOffset(8, m_fpos);
		pAttr->setTextOffset(9, m_fpos);

		attributes->setNamedItemNS(pAttr);
	}

//	SkipSpaces();
}

bool SGMLParser::ParseChildren(Node* parentNode, Node* nsResolverParent, int xmlSpacePreserve)
{
	bool bEndTag = false;

	Element* pLastElement = NULL;

	while (!eof())
	{
		int spacesStartOffset = m_fpos;
		sysstring spaces;
		CollectSpaces(spaces);

		if (pLastElement)
		{
			pLastElement->setTextOffset(9, m_fpos);
		//	pLastElement->Release();
			pLastElement = NULL;
		}

		if (eof()) break;

		int c = getcnext();

	//	if (c == EOF) break;

		if (c == L'<')
		{
			if (spaces.length())
			{
#if 0
				if (xmlSpacePreserve)	// preserve
				{
					Text* pText = new Text;
					pText->m_ownerDocument = m_document;
					pText->m_data = spaces;

					// TODO pText->m_whiteSpaceInElementContent = TRUE ??

					pText->setTextOffset(0, spacesStartOffset);
					pText->setTextOffset(1, spacesStartOffset);
					pText->setTextOffset(2, spacesStartOffset);
					pText->setTextOffset(3, spacesStartOffset+spaces.length());
					pText->setTextOffset(4, spacesStartOffset+spaces.length());
					pText->setTextOffset(5, spacesStartOffset+spaces.length());
					pText->setTextOffset(6, spacesStartOffset+spaces.length());
					pText->setTextOffset(7, spacesStartOffset+spaces.length());
					pText->setTextOffset(8, spacesStartOffset+spaces.length());
					pText->setTextOffset(9, spacesStartOffset+spaces.length());

					parentNode->appendChild(pText);
				}
#endif

				spaces = OLESTR("");
			}

			c = getcnext();

			if (c == L'!')
			{
				c = getcnext();
				if (c == L'[')	// CDATA section
				{
					sysstring id = GetID();
					if (!wcscmp(id.c_str(), OLESTR("CDATA")))
					{
						CDATASection* pCDATA = ParseCDATASection();
						if (pCDATA)
						{
							parentNode->appendChild(pCDATA);
						//	pCDATA->Release();
						}

					//	continue;
					}
					else
					{
						OLECHAR* msg = OLESTR("Expected CDATA");

						handleError(CreateDOMError(LSEVERITY_FATAL_ERROR, msg, getcnexturrentLocation()));
						throw msg;
					}
				}
				else if (c == L'-')	// Comment
				{
					EatChar('-');

					Comment* comment = ParseCommentNode();

					parentNode->appendChild(comment);
				}
				else	// DOCTYPE or DTD declaration
				{
					ungetcnext();

					sysstring id = GetID();

					if (!wcscmp(id.c_str(), OLESTR("DOCTYPE")))
					{
						DocumentType* doctype = ParseDOCTYPE();

						parentNode->appendChild(doctype);
					}
					else if (!wcscmp(id.c_str(), OLESTR("ENTITY")))
					{
					}
					else if (!wcscmp(id.c_str(), OLESTR("ELEMENT")))
					{
					}
					else if (!wcscmp(id.c_str(), OLESTR("ATTLIST")))
					{
					}
					else if (!wcscmp(id.c_str(), OLESTR("NOTATION")))
					{
					}
					else
					{
						OLECHAR msg[512];
						swprintf(msg, OLESTR("Unexpected ident %s"), id.c_str());

						handleError(CreateDOMError(LSEVERITY_FATAL_ERROR, msg, getcnexturrentLocation()));
						throw msg;
					}
				}
			}
			else if (c == L'?')	// Processing instruction
			{
				ProcessingInstruction* pi = ParseProcessingInstruction();
				parentNode->appendChild(pi);
			}
			else if (c == L'/')	// End tag of this element
			{
				bEndTag = true;
				break;
			}
			else // Beginning of new element
			{
				ungetcnext();

				pLastElement = ParseElement(parentNode, nsResolverParent, xmlSpacePreserve);
			}
		}
		else	// Beginning of text element
		{
			ungetcnext();

			ParseText(parentNode, spaces);
		}
	}

	return bEndTag;
}

Element* SGMLParser::ParseElement(Node* parentNode, Node* nsResolverParent, int parentXmlSpacePreserve)
{
	long startoffset = m_fpos-1;	// right before the <

	long startoffset1 = m_fpos;

	sysstring taglocalName = GetID();

	if (taglocalName.length() == 0)
	{
		OLECHAR* msg = OLESTR("Expected tagname after <");

		handleError(CreateDOMError(LSEVERITY_FATAL_ERROR, msg, getcnexturrentLocation()));
		throw msg;
	}

//	sysstring qualifiedName = tagprefix.length()? tagprefix + OLESTR(":") + taglocalName: taglocalName;

	long offset1 = m_fpos;
//	_bstr_t whitespace1 = CollectSpaces();
	//SkipSpaces();

// Parse the attributes
	NamedNodeMap* attributes = new NamedNodeMap;

// Add default attributes
	{
//		ATLTRACE("TODO\n");
#if 0
		CComQIPtr<ILDocumentAS, &IID_ILDocumentAS> documentAS((IUnknown*)m_document);

		<IASModel> asModel;
		documentAS->get_activeASModel(&asModel);

		if (asModel)
		{
			<IASNamedObjectMap> elementDeclarations;
			asModel->get_elementDeclarations(&elementDeclarations);

			sysstring tagName = (BSTR)taglocalName;

			<IASElementDeclaration> elementDecl;

			elementDeclarations->getNamedItem(tagName, (IASObject**)&elementDecl);
			if (elementDecl)
			{
				<IASNamedObjectMap> attributeDecls;
				elementDecl->get_ASAttributeDecls(&attributeDecls);

				long length;
				attributeDecls->get_length(&length);

				for (int i = 0; i < length; i++)
				{
					<IASObject> object;
					attributeDecls->item(i, &object);

					CComQIPtr<IASAttributeDeclaration, &IID_IASAttributeDeclaration> attrDecl((IUnknown*)object);

					ASValueConstraint defaultType;
					attrDecl->get_defaultType(&defaultType);

					if (defaultType == DEFAULT_VC || defaultType == FIXED_VC)
					{
						CComBSTR name;
						attrDecl->get_nodeName(&name);

						CComBSTR dataValue;
						attrDecl->get_dataValue(&dataValue);

						if (dataValue.Length())
						{
							/*
							WCHAR* localName;
							{
								WCHAR* colon;
								if (colon = wcschr(qualifiedName, L':'))
									localName = colon+1;
								else
									localName = qualifiedName;
							}
							*/

						//	ATLTRACE("default attribute: %S=%S\n", (BSTR)name, (BSTR)dataValue);

							CComObject<CLDOMAttr>* pAttr;
							CComObject<CLDOMAttr>::CreateInstance(&pAttr);
							pAttr->AddRef();
							pAttr->m_ownerDocument = m_document;
						//	pAttr->m_ownerElement =
							pAttr->m_specified = VARIANT_FALSE;

							pAttr->set_nodeName(name);

							pAttr->set_nodeValue(dataValue);

							//m_document->createAttributeNS(NULL, localName, &attr);

							<ILDOMNode> p;
							attributes->setNamedItemNS(pAttr, &p);

							pAttr->Release();
						}
					}
				}
			}
		}
#endif
	}

	ParseAttributes(nsResolverParent, attributes);

#if 0
	sysstring namespaceURI = FindNamespaceURI(nsResolverParent, attributes, tagprefix.c_str());
	if (tagprefix.length() > 0 && namespaceURI.length() == 0)
	{
		delete attributes;

		OLECHAR msg[512];
		swprintf(msg, OLESTR("Undefined namespace %s"), tagprefix.c_str());

		handleError(CreateDOMError(LSEVERITY_FATAL_ERROR, msg, getcnexturrentLocation()));
		throw msg;
	}
#endif

	Element* element = m_document->createElement(taglocalName);
	element->put_attributes(attributes);	// Set the previously parsed attributes onto the element

#if 0
	// TODO, do this here??
	{
		CComQIPtr<CLDOMElementImplImpl>(element)->AddEventListeners(element/*, CComQIPtr<CLDOMDocumentImplImpl>(m_document)->m_pScriptSite*/);
	}
#endif

	sysstring xmlspace = element->getAttribute(OLESTR("xml:space"));

	int xmlSpacePreserve;

	if (xmlspace.length())
		xmlSpacePreserve = xmlspace == OLESTR("preserve");
	else
		xmlSpacePreserve = parentXmlSpacePreserve;

	//attributes->Release();

//	ATLTRACE("TODO\n");
#if 0
	// Do this here this way?
	if (TRUE)
	{
		//CComQIPtr<CLDOMDocumentImplImpl> pDocument = m_document;
		CComQIPtr<CLDOMElementImplImpl>(element)->UpdateAnimationElements(element);
	}
#endif

	element->setTextOffset(0, startoffset);	// at the '...<'
	element->setTextOffset(1, startoffset1);	// at the '<...element'
	element->setTextOffset(2, offset1);	// <element...
	element->setTextOffset(3, m_fpos);	// <element attributes...>

	SkipSpaces();

	int c = getcnext();
	if (c == L'/')	// Immediate End tag of this element
	{
		SkipSpaces();
		EatChar('>');

		element->setTextOffset(4, m_fpos);
		element->setTextOffset(5, m_fpos);
		element->setTextOffset(6, m_fpos);
		element->setTextOffset(7, m_fpos);
		element->setTextOffset(8, m_fpos);
		element->setTextOffset(9, m_fpos);	// TODO

		parentNode->appendChild(element);

		elementDone(element);
		return element;
	}

	if (c != L'>')
	{
		OLECHAR msg[64];
		swprintf(msg, OLESTR("Expected >"));

		handleError(CreateDOMError(LSEVERITY_FATAL_ERROR, msg, getcnexturrentLocation()));
		throw msg;

		elementDone(element);
		return element;
	}

	element->setTextOffset(4, m_fpos);	// <tagName attributes>...

	parentNode->appendChild(element);

	bool bEndTag = ParseChildren(element, element/*NOTE, using element instead of nsResolverParent*/, xmlSpacePreserve);

// Required end tag

// ??
	element->setTextOffset(5, m_fpos-2);	// at </

	if (bEndTag)
	{
		SkipSpaces();	// TODO, remove this??

		element->setTextOffset(6, m_fpos);	// at </...element

		sysstring endtaglocalName = GetID();

		element->setTextOffset(7, m_fpos);	// at </element...>

#if 0
		sysstring endnamespaceURI;

		if (!wcscmp(tagprefix.c_str(), endtagprefix.c_str()))
		{
			endnamespaceURI = namespaceURI;
		}
		else
		{
			endnamespaceURI = FindNamespaceURI(parentNode, attributes, endtagprefix.c_str());
			if (endtagprefix.length() > 0 && endnamespaceURI.length() == 0)
			{
				OLECHAR msg[512];
				swprintf(msg, OLESTR("Undefined namespace %s"), endtagprefix.c_str());

				handleError(CreateDOMError(LSEVERITY_FATAL_ERROR, msg, getcnexturrentLocation()));
				throw msg;
			}
		}
#endif

		SkipSpaces();

		if (wcscmp(endtaglocalName.c_str(), taglocalName.c_str()))
		{
			OLECHAR msg[512];
			swprintf(msg, OLESTR("End tag \"%s\" doesn't match \"%s\""), taglocalName.c_str(), endtaglocalName.c_str());

			handleError(CreateDOMError(LSEVERITY_FATAL_ERROR, msg, getcnexturrentLocation()));
			throw msg;

			elementDone(element);
			return element;
		}

		SkipSpaces();
		EatChar('>');
	}
	else
	{
		OLECHAR msg[512];
		swprintf(msg, OLESTR("Missing endtag for \"%s\""), taglocalName.c_str());

		handleError(CreateDOMError(LSEVERITY_FATAL_ERROR, msg, getcnexturrentLocation()));
		throw msg;
	}

	element->setTextOffset(8, m_fpos);	// </element>...
	element->setTextOffset(9, m_fpos);	// TODO

	elementDone(element);

	return element;
}

void SGMLParser::elementDone(Element* element)
{
#if 0
	BSTR btagName;
	element->get_tagName(&btagName);
	_bstr_t tagName = _bstr_t(btagName, false);

	if (!wcsicmp(tagName, OLESTR("script")))
	{
		if (!m_editMode)
		{
			CComQIPtr<IDispatch> scriptDisp;

			CComQIPtr<IHTMLDocument2> htmlDocument;
			CComQIPtr<ISVGDocument> svgDocument;

			if (htmlDocument = m_document)
			{
				htmlDocument->get_Script(&scriptDisp);
			}
			else if (svgDocument = m_document)
			{
				svgDocument->get_Script(&scriptDisp);
			}

			if (scriptDisp != NULL)
			{
				CComQIPtr<IActiveScript> activeScript = scriptDisp;
				CComQIPtr<IActiveScriptParse> activeScriptParse = scriptDisp;

				BSTR bsrc;
				element->getAttribute(OLESTR("src", &bsrc);
				_bstr_t src = _bstr_t(bsrc, false);
				if (src.length())
				{
					BSTR burl;
					m_textDoc->get_url(&burl);
					_bstr_t url = _bstr_t(burl, false);

					char dir[260] = {0};
					char path[260] = {0};
					if (url.length())
					{
						_splitpath(url, dir, path, NULL, NULL);
					}

					char abspath[260];
					_makepath(abspath, dir, path, src, NULL);

					CComQIPtr<ITextData> textfile;
					textfile.CoCreateInstance(CLSID_TextData);

					bool bloaded;
					textfile->load(_bstr_t(abspath), &bloaded);
					if (bloaded)
					{
						BSTR textdata;
						textfile->get_data(&textdata);

						HRESULT hr;

						EXCEPINFO	excepinfo;
						hr = activeScriptParse->ParseScriptText(

							textdata,

							NULL,
							NULL,
							NULL,
							0,
							0,	// start line number
							SCRIPTTEXT_ISVISIBLE | SCRIPTTEXT_ISPERSISTENT,	// flags
							NULL,
							&excepinfo);
					}

					/*
					URL_COMPONENTS	url = {0};
					url.dwStructSize = sizeof(url);
					InternetCrackUrl(m_url, 0, 0, &url);
					InternetCombineUrl(
					*/
				}

				CComQIPtr<IDOMNode> node;
				element->get_firstChild(&node);
				if (node != NULL)
				{
					CComQIPtr<IDOMCharacterData> text = node;
					if (text)
					{
						BSTR textdata;
						text->get_data(&textdata);

						HRESULT hr;

						EXCEPINFO	excepinfo;
						hr = activeScriptParse->ParseScriptText(

							textdata,

							NULL,
							NULL,
							NULL,
							0,
							0,	// start line number
							SCRIPTTEXT_ISVISIBLE | SCRIPTTEXT_ISPERSISTENT,	// flags
							NULL,
							&excepinfo);

						SysFreeString(textdata);

						//htmlDocument->close();
					}
				}
			}
		}
	}
	else if (!wcsicmp(tagName, OLESTR("style"))
	{
		CComQIPtr<IDOMNode> child;
		element->get_firstChild(&child);

		CComQIPtr<IDOMText> text = child;
		if (text)
		{
			BSTR btextdata;
			text->get_data(&btextdata);
			_bstr_t textdata = _bstr_t(btextdata, false);

			CComQIPtr<IDOMCSSStyleSheet> stylesheet;
			stylesheet.CoCreateInstance(CLSID_DOMCSSStyleSheet);

			CComQIPtr<IDocumentStyle> documentStyle = m_document;//getcnextontrollingUnknown();
			if (documentStyle)
			{
				CComQIPtr<IStyleSheetList> stylesheetList;
				documentStyle->get_styleSheets(&stylesheetList);

				stylesheetList->append(stylesheet);

				CComQIPtr<IHTMLStyleSheet> htmlstylesheet = stylesheet;
				htmlstylesheet->set_cssText(textdata);	// The stylesheet will parse the text
			}
		}
	}
#endif
//	Fire_ParsedElementDone(element);
}

Document* SGMLParser::get_document() const
{
	return m_document;
}

void SGMLParser::set_document(Document *newVal)
{
	m_document = newVal;
}

bool SGMLParser::parse()
{
	return !loadXML2();
}

long SGMLParser::ParseXMLHeader()
{
	StringW* xmlid = GetID();
	if (wcscmp(xmlid.c_str(), L"xml"))
	{
// Not an xml document
		//MessageBox(NULL, "Not an xml document", "XML", MB_OK);
		throw OLESTR("Not an xml document");
	}

	//Read <?xml ... ?>
	while (!eof())
	{
		SkipSpaces();

		int c = getcnext();

		if (!iswalpha(c))
		{
			ungetcnext();
			break;
		}

		ungetcnext();

		sysstring name = GetID();

		SkipSpaces();
		EatChar(L'=');
		SkipSpaces();

		sysstring value = GetString();

		if (!wcscmp(name.c_str(), OLESTR("version")))
			;//m_version = value;
		else if (!wcscmp(name.c_str(), OLESTR("encoding")))
			;//m_encoding = value;
		else if (!wcscmp(name.c_str(), OLESTR("standalone")))
		{
			if (!wcscmp(value.c_str(), OLESTR("yes")))
				;//m_standalone = TRUE;
			else if (!wcscmp(value.c_str(), OLESTR("no")))
				;//m_standalone = FALSE;
			else
			{
				// TODO, create warning
				;//m_standalone = FALSE;
			}
		}

		SkipSpaces();
	}

	EatChar('?');
	SkipSpaces();
	EatChar('>');

	return 0;
}

DocumentType* SGMLParser::ParseDOCTYPE()
{
// <!DOCTYPE has been read

#if 0
	USES_CONVERSION;
#endif

	DocumentType* doctype = new DocumentType;
	if (doctype)
	{
		doctype->m_ownerDocument = m_document;

		doctype->setTextOffset(0, m_fpos-9);
		doctype->setTextOffset(1, m_fpos-7);
		doctype->setTextOffset(2, m_fpos);

		SkipSpaces();

		doctype->setTextOffset(3, m_fpos);

		doctype->m_name = GetID();
		//doctype->m_name = name;//put_name(name);

		SkipSpaces();

		sysstring more = GetID();
		SkipSpaces();

		if (!wcscmp(more.c_str(), OLESTR("PUBLIC")))
		{
			doctype->m_publicId = GetString();

			SkipSpaces();

			int c = getcnext();
			ungetcnext();
			if (c != '>')
			{
				doctype->m_systemId = GetString();
			}
		}
		else if (!wcscmp(more.c_str(), OLESTR("SYSTEM")))
		{
			doctype->m_systemId = GetString();
		}
		else
		{
			OLECHAR* msg = OLESTR("Expected PUBLIC or SYSTEM");

			handleError(CreateDOMError(LSEVERITY_FATAL_ERROR, msg, getcnexturrentLocation()));
			throw msg;
		}

		SkipSpaces();

	//	CComObject<CDTDDocument>::CreateInstance(&doctype->m_dtd);
	//	doctype->m_dtd->AddRef();

		doctype->setTextOffset(4, m_fpos);

	// Internal subset
		int c = getcnext();
		if (c == L'[')
		{
			ASSERT(0);
#if 0
			doctype->setTextOffset(5, m_fpos);

			CDTDParser dtdParser;
			dtdParser.m_document = m_document;//doctype->m_dtd;
			dtdParser.m_dtdDocument = doctype->m_dtd;
			dtdParser.m_parentNode = doctype->m_internalSubsetNode;

			dtdParser.m_eof = L']';
			dtdParser.m_textData = m_textData;
			dtdParser.m_errorHandler = m_errorHandler;
			dtdParser.m_line = m_line;
			dtdParser.m_column = m_column;
			dtdParser.m_fpos = m_fpos;

			HRESULT hr = dtdParser.Parse2();

			m_line = dtdParser.m_line;
			m_column = dtdParser.m_column;
			m_fpos = dtdParser.m_fpos;

			doctype->setTextOffset(6, m_fpos);
			EatChar(L']');
#endif
		}
		else
			ungetcnext();	// ungetcnext [

		SkipSpaces();

		doctype->setTextOffset(7, m_fpos);

		EatChar(L'>');

		doctype->setTextOffset(8, m_fpos);
		doctype->setTextOffset(9, m_fpos);

		if (m_document)
		{
#if 0	// Done when node is inserted into document
			// External subset
			if (doctype->m_systemId.Length())
			{
				CComBSTR documentUrl;
				m_document->get_url(&documentUrl);

				TCHAR result[2048];
				{
					DWORD resultLen = sizeof(result);
					InternetCombineUrl(W2A(documentUrl), W2A(doctype->m_systemId), result, &resultLen, 0);
				}

				CDTDParser dtdParser;
				dtdParser.m_parentNode = doctype->m_dtd;
				dtdParser.m_dtdDocument = doctype->m_dtd;
				dtdParser.m_errorHandler = m_errorHandler;
				//dtdParser.m_url = _bstr_t(documentUrl);

				WCHAR* absurl = A2W(result);

				ATLTRACE("loading external subset: %s...", result);
				HRESULT hr = dtdParser.loadURL(absurl);
				if (FAILED(hr))
				{
					OLECHAR msg[512];
					swprintf(msg, OLESTR("Failed to load external subset url: %s", absurl);

					if (!handleError(CreateDOMError(LSEVERITY_ERROR, msg, getcnexturrentLocation())))
					{
						throw msg;
					}
				}

				ATLTRACE("done\n", result);
			}

			// Build and parse general entities
			{
				for (int i = 0; i < doctype->m_dtd->m_entityList.GetSize(); i++)
				{
					CEntity* pDTDEntity = doctype->m_dtd->m_entityList[i];

					if (pDTDEntity->m_c == 0)
					{
						<ILDOMDocumentFragment> fragment;
						m_document->parseFragment(pDTDEntity->m_value, NULL/*hm ??*/, &fragment);

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

								<ILDOMNode> p;
								doctype->m_entities->setNamedItem(pEntity, &p);

								pEntity->Release();
							}
						}
					}
				}
			}

			/*
		// Finally convert the internal and external subset to AS model
			CComObject<CASDTDModel>* pModel;
			CComObject<CASDTDModel>::CreateInstance(&pModel);
			if (pModel)
			{
				pModel->AddRef();

				pModel->BuildFromDTDDocument(doctype->m_dtd);

				CComQIPtr<ILDocumentAS> documentAS = m_document;
				if (documentAS)
				{
					documentAS->put_activeASModel(pModel);
				}

				pModel->Release();
			}
			*/
#endif
		}
	}

	return doctype;
}

#if 0
#include "XMLSchema.h"
#endif

HRESULT SGMLParser::loadXML2()
{
//	USES_CONVERSION;

	TRACE("loadXML2()...\n");

//	m_document.Release();

#if 0
	m_textDoc->get_len(&m_textLen);
	m_textDoc->get_data(&m_textData);
#endif

	if (m_document == NULL)
	{
// Find out what kind of document this is
		DocumentType* doctype;

		m_line = 0;
		m_column = 0;
	//	m_oldc = EOF;
		m_fpos = 0;

		//m_bXML = false;

		try
		{
			SkipSpaces();

		// Read <?xml ... >
			EatChar('<');

			int c = getcnext();
			if (c == '?')
			{
				ParseXMLHeader();
			//	m_bXML = true;
			}
			else
			{
				ungetcnext();
				ungetcnext();
			}

			SkipSpaces();

		// Process document children
			while (!eof())
			{
				EatChar('<');
			//	SkipSpaces();

				int c = getcnext();
				if (c == L'!')
				{
					int c2 = getcnext();
					if (c2 == L'-')
					{
						int c3 = getcnext();
						if (c3 == L'-')
						{
							// Comment
							Comment* comment = ParseCommentNode();
						}
						else
						{
							ungetcnext();
							ungetcnext();
						}
					}
					else	// Maybe DOCTYPE
					{
						ungetcnext();

						sysstring id = GetID();
						if (id.length() && !wcscmp(id.c_str(), OLESTR("DOCTYPE")))
						{
							if (doctype != NULL)	// Can only have one doctype node
							{
								OLECHAR* msg = OLESTR("Can only have one doctype node");

								if (!handleError(CreateDOMError(LSEVERITY_ERROR, msg, getcnexturrentLocation())))
								{
									throw msg;
								}
							}

							doctype = ParseDOCTYPE();
						}
						else
						{
							// Assume it's a DTD document

							ASSERT(0);
#if 0
							m_document.CoCreateInstance(CLSID_LDTDDocument);
#endif
							break;
							/*
							WCHAR* msg = OLESTR("Expected DOCTYPE";

							handleError(CreateDOMError(LSEVERITY_FATAL_ERROR, msg, getcnexturrentLocation()));
							throw msg;
							*/

						}
					}
				}
				else if (c == L'?')
				{
					// Processing instruction
					ProcessingInstruction* pi = ParseProcessingInstruction();
				}
				else
				{
					ungetcnext();	// back up the character that wasn't a '!'

					SkipSpaces();

					sysstring taglocalName;
					sysstring tagprefix;
					GetNSID(tagprefix, taglocalName);

					if (taglocalName.length() == 0)
					{
						OLECHAR* msg = OLESTR("Expected tagname after <");

						handleError(CreateDOMError(LSEVERITY_FATAL_ERROR, msg, getcnexturrentLocation()));
						throw msg;
					}

#if 0
					if (!wcscmp(taglocalName, OLESTR("html"))
					{
						m_document.CoCreateInstance(CLSID_LHTMLDocument);
					}
					else if (!wcscmp(taglocalName, OLESTR("svg"))
					{
						//m_document.CoCreateInstance(CLSID_LSVGDocument);
						m_document.CoCreateInstance(OLESTR("LSVG.LSVGDocument");
					}
					else if (!wcscmp(taglocalName, OLESTR("smil"))
					{
						m_document.CoCreateInstance(CLSID_LSMILDocument);
					}
					else if (!wcscmp(taglocalName, OLESTR("lxui"))
					{
						m_document.CoCreateInstance(OLESTR("LXUI.LXUIDocument");
					}
					else if (!wcscmp(taglocalName, OLESTR("nitf"))
					{
						m_document.CoCreateInstance(CLSID_LNITFDocument);
					}
					else if (!wcscmp(taglocalName, OLESTR("NewsML"))
					{
						m_document.CoCreateInstance(CLSID_LNewsMLDocument);
					}
#if 0
					else if (!wcscmp(taglocalName, OLESTR("schema"))
					{
						m_document.CoCreateInstance(CLSID_XMLSchemaDocument);
					}
#endif
					else
#endif
					{
						m_document = new CLDOMDocument;//.CoCreateInstance(CLSID_LDOMDocument);
					}

					if (m_document == NULL)
					{
						throw OLESTR("Couldn't create document");
					}

#if 0
					CComQIPtr<CLDOMDocumentImplImpl>(m_document)->m_acDocument = m_activeDocument;
#endif
					m_document->put_errorHandler(m_errorHandler);
				//	m_document->put_isXML(m_bXML);

					// hm.. do this here ?
					if (true)
					{
						LCSS::DocumentStyleImpl* documentStyle = dynamic_cast<LCSS::DocumentStyleImpl*>(m_document);
						if (documentStyle)
						{
#if 0//_WINDOWS
							TCHAR appdata[MAX_PATH];
							SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, appdata);

							CSSStyleSheet* stylesheet = new CSSStyleSheet;//.CoCreateInstance(CLSID_LCSSStyleSheet);
							if (stylesheet)
							{
								TCHAR pathname[MAX_PATH];
								_makepath(pathname, NULL, appdata, "LXFramework", NULL);

								TCHAR filename[MAX_PATH];

								if (!wcscmp(taglocalName, OLESTR("nitf")))
								{
									_makepath(filename, NULL, pathname, "nitf.css", NULL);
								}
								else if (!wcscmp(taglocalName, OLESTR("article")))	// DocBook
								{
									_makepath(filename, NULL, pathname, "docbook.css", NULL);
								}
								else if (!wcscmp(taglocalName, OLESTR("stylesheet")))
								{
									_makepath(filename, NULL, pathname, "xslt.css", NULL);
								}
								else if (!wcscmp(taglocalName, OLESTR("html")) ||
									!wcscmp(taglocalName, OLESTR("svg")) ||
									!wcscmp(taglocalName, OLESTR("lxui")))
								{
									_makepath(filename, NULL, pathname, "html.css", NULL);
								}

								//MessageBox(NULL, filename, "", MB_OK);

								stylesheet->Load(A2W(filename));

								<ILStyleSheetList> stylesheets;
								documentStyle->get_styleSheets(&stylesheets);
								stylesheets->append(stylesheet);
							}
#endif	// _WINDOWS
						}
					}

					break;
				}

				SkipSpaces();
			}
		}
		catch(OLECHAR* p)
		{
		// hm.. create document even if failed (NO????)
#if 0
			m_document.CoCreateInstance(OLESTR("LHTML.LSVGDocument");
#endif

			TRACE("\n\nERROR FIRST PASS XML: %S\n\n", p);
			return E_FAIL;
		}
	}
	else
	{
		// TODO, remove all children
		Element* documentElement = m_document->get_documentElement();
		if (documentElement)
		{
			m_document->put_documentElement(NULL);
			m_document->removeChild(documentElement);
		}

		DocumentType* doctype = m_document->get_doctype();
		if (doctype)
		{
			//m_document->put_doctype(NULL);
			m_document->removeChild(doctype);
		}
	}

	if (m_document)
	{
#if 0
		if (m_textDoc)
		{
			CComBSTR url;
			m_textDoc->get_url(&url);
			m_document->put_url(url);
		}
#endif

		HRESULT hr = E_FAIL;	// Assume error

		m_line = 0;
		m_column = 0;
	//	m_oldc = EOF;
		m_fpos = 0;

		/*
		m_document->setTextOffset(0, m_fpos);
		m_document->setTextOffset(1, m_fpos);
		m_document->setTextOffset(2, m_fpos);
		m_document->setTextOffset(3, m_fpos);
		m_document->setTextOffset(4, m_fpos);
		*/

#if 0
		CComQIPtr<ILDTDDocument, &IID_ILDTDDocument> dtdDocument((IUnknown*)m_document);
		if (dtdDocument)	// ???
		{
			CDTDParser dtdParser;
			dtdParser.m_document = m_document;//doctype->m_dtd;
			dtdParser.m_dtdDocument = static_cast<CDTDDocument*>(m_document.p);
			dtdParser.m_parentNode = m_document;//doctype->m_internalSubsetNode;

			//dtdParser.m_eof = L']';
			dtdParser.m_url = m_uri;
			dtdParser.m_textData = m_textData;
			dtdParser.m_errorHandler = m_errorHandler;
			dtdParser.m_line = m_line;
			dtdParser.m_column = m_column;
			dtdParser.m_fpos = m_fpos;

			CComObject<CASDTDModel>::CreateInstance(&static_cast<CDTDDocument*>(m_document.p)->m_pASModel);
			static_cast<CDTDDocument*>(m_document.p)->m_pASModel->AddRef();

			HRESULT hr = dtdParser.Parse();

			static_cast<CDTDDocument*>(m_document.p)->m_pASModel->BuildFromDTDDocument(static_cast<CDTDDocument*>(m_document.p));

			return hr;
		}
#endif

#if 0
		CComQIPtr<CLDOMDocumentImplImpl>(m_document)->InitScripting();
#endif

		try
		{
			SkipSpaces();

		// Read <?xml ... >
			EatChar('<');
			SkipSpaces();

			int c = getcnext();
			if (c == '?')
			{
				ParseXMLHeader();
			}
			else
			{
				ungetcnext();
				ungetcnext();
			}

			SkipSpaces();

			ParseChildren(m_document, NULL, false/*xmlSpacePreserve*/);

	// Validate document

#if 0
			try
			{
				<ILDOMElement> documentElement;
				m_document->get_documentElement(&documentElement);
				if (documentElement)
				{
					BSTR bnoNSSchemaLocation;
					documentElement->getAttribute(OLESTR("xsi:noNamespaceSchemaLocation", &bnoNSSchemaLocation);
					_bstr_t noNSSchemaLocation = _bstr_t(bnoNSSchemaLocation, false);

					BSTR bschemaLocation;
					documentElement->getAttribute(OLESTR("xsi:schemaLocation", &bschemaLocation);
					_bstr_t schemaLocation = _bstr_t(bschemaLocation, false);

					_bstr_t schemaURL;

					if (schemaLocation.length())
					{
						_bstr_t uri;
						_bstr_t ns;

						OLECHAR* p = (BSTR)schemaLocation;

						// Get namespace part
						while (*p)
						{
							if (iswspace(*p)) break;
							WCHAR c2[2] = {*p, 0};
							ns += c2;
							p++;
						}

						while (iswspace(*p)) p++;

						// get uri part
						while (*p)
						{
						//	if (iswspace(*p)) break;
							WCHAR c2[2] = {*p, 0};
							uri += c2;
							p++;
						}

						schemaURL = uri;
					}

					if (schemaURL.length() == 0)
					{
						schemaURL = noNSSchemaLocation;
					}

					if (schemaURL.length())
					{
						char abspath[260];

						{
							BSTR burl;
							m_textDoc->get_url(&burl);
							_bstr_t url = _bstr_t(burl, false);

							char dir[260] = {0};
							char path[260] = {0};
							if (url.length())
							{
								_splitpath(url, dir, path, NULL, NULL);
							}

							_makepath(abspath, dir, path, schemaURL, NULL);
						}

						CComQIPtr<IXMLSchemaDocument> schemadoc;
						schemadoc.CoCreateInstance(CLSID_XMLSchemaDocument);
						bool bloadedSchema;
						schemadoc->load(_bstr_t(abspath), &bloadedSchema);
						if (bloadedSchema)
						{
							CComQIPtr<IDOMElement> mergeDocumentElement;
							schemadoc->get_documentElement(&mergeDocumentElement);

							CSchema* pSchema = new CSchema;
							pSchema->m_errorHandler = m_errorHandler;
						//	pSchema->m_schemaElement.CoCreateInstance(CLSID_DOMElement);

						// Copy the targetNamespace attribute (hm..)
							{
								BSTR b;
								mergeDocumentElement->getAttribute(OLESTR("targetNamespace", &b);
								if (b)
								{
									pSchema->m_targetNamespace = b;
									//pSchema->m_schemaElement->setAttribute(OLESTR("targetNamespace", b);
									SysFreeString(b);
								}
							}

							{
								BSTR b;
								mergeDocumentElement->getAttribute(OLESTR("elementFormDefault", &b);
								if (b)
								{
									pSchema->m_elementFormDefault = b;
									//pSchema->m_schemaElement->setAttribute(OLESTR("elementFormDefault", b);
									SysFreeString(b);
								}
							}

							{
								BSTR b;
								mergeDocumentElement->getAttribute(OLESTR("attributeFormDefault", &b);
								if (b)
								{
									pSchema->m_attributeFormDefault = b;
									//pSchema->m_schemaElement->setAttribute(OLESTR("attributeFormDefault", b);
									SysFreeString(b);
								}
							}

							pSchema->IncludeSchema(mergeDocumentElement);

						//
							pSchema->ValidateDocument(m_document);

							delete pSchema;
						}
					}
				}
			}
			catch(OLECHAR* p)
			{
				TRACE("\n\nERROR VALIDATING XML: %S\n\n", p);
			}
#endif

			{
#if 0//_WINDOWS
				if (CComQIPtr<CLDOMDocumentImplImpl>(m_document)->m_pScriptSite->m_debugDocumentHelper)
				{
					BSTR textdata;
					m_textDoc->get_data(&textdata);
					// Note, not freeing

					HRESULT hr = CComQIPtr<CLDOMDocumentImplImpl>(m_document)->m_pScriptSite->m_debugDocumentHelper->AddUnicodeText(textdata);
					ASSERT(SUCCEEDED(hr));
				}

			//	SCRIPTSTATE state;
			//	m_pScriptSite->m_activeScript->GetScriptState(&state);

			//	CComQIPtr<IActiveScriptParse> activeScriptParse = CComQIPtr<CLDOMDocumentImplImpl>(m_document)->m_pScriptSite->m_activeScript;

			//	CComQIPtr<IActiveScriptParseProcedure> activeScriptParseProcedure = CComQIPtr<CLDOMDocumentImplImpl>(m_document)->m_pScriptSite->m_activeScript;

				CComQIPtr<CLDOMDocumentImplImpl/*, &IID_CLDOMDocumentImplImpl*/>((IUnknown*)m_document)->AddScripts(m_document);
			//	AddEventListeners(m_document);	// Already done
#endif	// _WINDOWS
			}

			TRACE("Successfully Parsed XML document\n");

			hr = S_OK;	// Completely successfull
		}
		catch(OLECHAR* p)
		{
			TRACE("\n\nPartially failed loading XML: %S\n\n", p);

			hr = S_FALSE;
		}

		m_document->setTextOffset(5, m_fpos);
		m_document->setTextOffset(6, m_fpos);
		m_document->setTextOffset(7, m_fpos);
		m_document->setTextOffset(8, m_fpos);
		m_document->setTextOffset(9, m_fpos);

		// Do this even if loading xml partially failed
		{

			if (m_document->m_documentElement)
			{
#if 0	// Had this
				{
					<ILDOMNodeList> extensionDefsElements;
					m_document->getElementsByTagNameNS(OLESTR("http://www.w3.org/2000/svg"), OLESTR("extensionDefs"), &extensionDefsElements);

					pDocument->BindElements(extensionDefsElements, pDocument->m_documentElement);
				}
#endif

#if 0	// TODO:Remove this
			// SMIL Animation (connect animate/set etc. elements to the elements they target)
				pDocument->BuildAnimationListForAllElements(pDocument->m_documentElement, pDocument->m_documentElement);
#endif
#if 0	// Had this
				pDocument->SetValuesOnAllAnimations(pDocument->m_documentElement);
#endif

#if 0	// Done during parsing
			// Set baseVal/animVal from attributes and parse 'style' attributes
				pDocument->UpdateAnimationElements(pDocument->m_documentElement);
#endif

#if 0	// Done automatically while document is being built ?
				pDocument->GetFirstIntervals(pDocument->m_documentElement);
#endif
			}

#if 0	// Had this
			{
				for (int i = 0; i < pDocument->m_pViews.GetSize(); i++)
				{
					ASSERT(0);
#if 0
					pDocument->m_pViews[i]->DoneParsing();
#endif
				}
			}
#endif
		}

		TRACE("loadXML2() hr=%d\n", hr);

		return hr;
	}
	else
		return E_FAIL;
}

bool SGMLParser::validateToSchema(Document *schemaDoc)
{
/*
	CComQIPtr<IDOMElement> element;
	m_document->get_documentElement(&element);

	*pVal = ValidateElement(schemaDoc, element);
*/
	return S_OK;
}

bool SGMLParser::load(const sysstring& xmlSource)
{
	ASSERT(0);
	return false;
#if 0
	USES_CONVERSION;

	std::string filename = DownloadFile(W2A(xmlSource.c_str()));

	HANDLE hFile = CreateFile(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	DWORD nRead;
	uint8* buffer = NULL;// = new uint8[512];
	int len = 0;
	do
	{
		buffer = (uint8*)realloc(buffer, len+512);
		ReadFile(hFile, buffer+len, 512, &nRead, NULL);
		len += nRead;
	}
	while (nRead == 512);

	CloseHandle(hFile);

	WCHAR* textData = new OLECHAR[len];
	m_textLen = len;
	for (int i = 0; i < len; i++)
	{
		textData[i] = buffer[i];
	}

	delete [] buffer;

	m_textData = textData;

	bool success = !loadXML2();

	delete textData;

	return success;
#if 0
	if (success == NULL) return E_POINTER;
	*success = VARIANT_FALSE;

	if (m_textDoc == NULL)
	{
		CComQIPtr<ITextData, &IID_ITextData> textDoc;
		textDoc.CoCreateInstance(CLSID_TextData);

		m_textDoc = textDoc;
	}

	if (m_textDoc)
	{
		HRESULT hr = E_FAIL;	// Assume failure

		if (V_VT(&xmlSource) == VT_BSTR)
		{
			m_uri = V_BSTR(&xmlSource);
			hr = m_textDoc->load(m_uri);
		}
		else if (V_VT(&xmlSource) == VT_UNKNOWN || V_VT(&xmlSource) == VT_DISPATCH)
		{
			CComQIPtr<IStream, &IID_IStream> stream = V_UNKNOWN(&xmlSource);

			if (stream)
			{
				CComQIPtr<IPersistStream> persistStream = m_textDoc;
				hr = persistStream->Load(stream);
			}
		}

		if (SUCCEEDED(hr))
		{
			hr = loadXML2();
		// S_FALSE means partial success, but we only mark this variable on a complete success
			if (hr == S_OK)
				*success = VARIANT_TRUE;
			else
				*success = VARIANT_FALSE;
		}
	}

// Should I release the textDoc if it hasn't been explicitly set?

	return S_OK;
#endif
#endif
}

bool SGMLParser::loadXML(const sysstring& sourceText)
{
#if 0

	if (m_textDoc == NULL)
	{
		CComQIPtr<ITextData, &IID_ITextData> textDoc;
		textDoc.CoCreateInstance(CLSID_TextData);

		m_textDoc = textDoc;
	}

	m_textDoc->put_data(SysAllocString(sourceText));
#endif
	ASSERT(0);
#if 0
	m_textLen = sourceText.length();
	m_textData = sourceText.c_str();
#endif

	return !loadXML2();

// Should I release the textDoc if it hasn't been explicitly set?
}

STDMETHODIMP SGMLParser::get_errorHandler(ILDOMErrorHandler **pVal)
{
	*pVal = m_errorHandler;
//	if (*pVal) (*pVal)->AddRef();

	return S_OK;
}

STDMETHODIMP SGMLParser::put_errorHandler(ILDOMErrorHandler *newVal)
{
	m_errorHandler = newVal;
	return S_OK;
}

ILDOMLocator* SGMLParser::getcnexturrentLocation()
{
	CLDOMLocator* locator = new CLDOMLocator;
	if (locator)
	{
		locator->m_offset = m_fpos;
		locator->m_lineNumber = m_line;
		locator->m_columnNumber = m_column;
		locator->m_uri = m_uri;
	}

	return locator;
}

}	// LXML

#endif