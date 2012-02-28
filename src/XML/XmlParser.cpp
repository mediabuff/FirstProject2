#include "stdafx.h"
#include "XML.h"
#include "XmlData.h"
#include "XmlParser.h"

namespace System
{
using namespace IO;

namespace XmlData
{

class Node;
class Element;
class Attr;
class Document;
class Comment;
class Text;
class CDATASection;
class DocumentType;
class ProcessingInstruction;
class NamedNodeMap;
class DOMError;

enum DOMErrorSeverity
{
	DOMErrorSeverity_ERROR,
	DOMErrorSeverity_FATAL_ERROR,
};

using namespace std;

// This version assumes c is a legal hex digit
int Hex2Number(WCHAR c)
{
	ASSERT(iswxdigit(c));

	if (c <= L'9')
		return c - L'0';
	else
		return 10 + towlower(c) - L'a';
}

int strhex2int(const WCHAR* p)
{
	int v = 0;
	while (*p)
	{
		v <<= 4;

		int n = Hex2Number(*p);
		v += n;

		p++;
	}

	return v;
}

WCHAR GetUCode(StringIn ucode)
{
/*
				else if (!tcscmp(name, WSTR("copy")))	// TODO ??? don't replace
				{
					str += L'�';
				}
				else if (!tcscmp(name, WSTR("oslash")))	// TODO ??? don't replace
				{
					str += L'�';
				}
				else if (!tcscmp(name, WSTR("aring")))	// TODO ??? don't replace
				{
					str += L'�';
				}
*/

	if (ucode == L"amp")
		return '&';
	else if (ucode == L"lt")
		return '<';
	else if (ucode == L"gt")
		return '>';
	else if (ucode == L"quot")
		return '"';
	else if (ucode == L"apos")
		return '\'';
	else if ((ucode)[0] == '#')
	{
		ASSERT(0);
		/*
		if (ucode[1] == L'x')
			return strhex2int(&ucode->c_str()[2]);
		else
			return (WCHAR)str2int(&ucode->c_str()[1]);	// decimal
			*/
	}

	return 0;
}

class DOMError
{
public:
	DOMErrorSeverity m_severity;
	String m_message;
	IDOMLocator* m_location;
};

DOMError* CreateDOMError(DOMErrorSeverity severity, StringIn message, IDOMLocator* location)
{
	DOMError* p = new DOMError;
	if (p)
	{
		p->m_severity = severity;
		p->m_message = message;
		p->m_location = location;
	}

	return p;
}

/////////////////////////////////////////////////////////////////////////////
// XMLParser

XMLParser::XMLParser()
{
	m_uri = nullptr;
#if 0
	m_activeDocument = NULL;
#endif
	m_errorHandler = nullptr;
//	m_editMode = FALSE;

//	m_textLen = 0L;
//	m_textData = NULL;

	m_fpos = 0;
	m_line = 0;
	m_column = 0;
	m_oldc = EOF;

	m_utf8 = false;
}

void XMLParser::getcnext()
{
	wchar_t c;
	m_stream >> c;
	m_c = c;

	/*
	if (m_fpos < m_textLen)
	{
		int c = m_textData[m_fpos++];

		if (m_utf8)
		{
			if (c & 80)	// high bit set
			{
			}
		}

		if (c == '\n')
		{
			m_line++;
			m_column = 0;
		}
		else
			m_column++;

		m_oldc = c;
		return c;
	}
	else
	{
		ASSERT(0);
		raise(Exception(L"EOF"));
		return EOF;
	}
	*/
}

/*
void XMLParser::ungetcnext()
{
// TODO
	if (m_oldc == '\n') m_line--;
	m_fpos--;
}
*/

bool XMLParser::handleError(DOMError* error)
{
	if (m_errorHandler)
	{
		bool bContinue = false;
		ASSERT(0);
	//	m_errorHandler->handleError(error, &bContinue);
		return bContinue;
	}
	return false;
}

void XMLParser::CollectSpaces(IO::StringWriter& spaces)
{
	while (!eof())
	{
		if (!isspace(m_c))
		{
			break;
		}

		spaces << m_c;
		getcnext();
	}
}

String XMLParser::GetID()
{
	String str;

	while (!eof())
	{
		if (!(iswalnum(m_c) || m_c == L':' || m_c == L'.' || m_c == L'-' || m_c == L'_'))
		{
			break;
		}

		str += m_c;
		getcnext();
	}

	if (str == nullptr)
	{
	//	StringW* msg = WSTR("Expected ident");
		handleError(CreateDOMError(DOMErrorSeverity_FATAL_ERROR, L"Expected ident", getcnexturrentLocation()));
		raise(Exception(L"Expected ident"));
	}

	return str;
}

void XMLParser::GetNSID(String& ns, String& id)
{
	ns = nullptr;
	id = nullptr;

	IO::StringWriter str;

	while (!eof())
	{
		if (!(iswalnum(m_c) || m_c == ':' || m_c == '.' || m_c == '-' || m_c == '_'))
		{
			break;
		}

		if (m_c == L':')
		{
			ns = str.Detach();
		}
		else
		{
			str << m_c;
		}

		getcnext();
	}

	id = str.str();
}

String XMLParser::GetString()
{
	SkipSpaces();

	int delimiter = m_c;

	if (delimiter != L'\"' && delimiter != L'\'')
	{
		WCHAR msg[64];
		//swprintf(msg, WSTR("Expected \" or ' at offset %d"), m_fpos);
		ASSERT(0);

		handleError(CreateDOMError(DOMErrorSeverity_FATAL_ERROR, msg, getcnexturrentLocation()));
		raise(Exception(msg));
	}
	else
	{
		getcnext();
	}

	String str;

	while (!eof())
	{
		if (m_c == delimiter)
		{
			break;
		}

		str += m_c;
		getcnext();
	}

	EatChar(delimiter);
	SkipSpaces();

	return str;
}

void XMLParser::GetAttrValue(Attr* attr)
{
	DocumentType* doctype = m_document->get_doctype();

#if 0
	NamedNodeMap* entities = NULL;
	if (doctype)
	{
		entities = doctype->get_entities();
	}
#endif

//	SkipSpaces();

	int delimiter = m_c;

	if (delimiter != L'\"' && delimiter != L'\'')
	{
		delimiter = 0;
	}
	else
	{
		getcnext();
	}

	attr->setTextOffset(3, m_fpos);

	if (true)
	{
		if (delimiter == 0)
		{
			WCHAR msg[64];
			ASSERT(0);
			//swprintf(msg, WSTR("Expected \" or '"));

			handleError(CreateDOMError(DOMErrorSeverity_FATAL_ERROR, msg, getcnexturrentLocation()));
			raise(Exception(msg));
		}
	}

	IO::StringWriter strbuilder;

	while (!eof())
	{
		if (delimiter == 0)
		{
			if (iswspace(m_c) || m_c == L'>')
			{
				break;
			}
		}
		else
		{
			if (m_c == delimiter)
			{
				break;
			}
		}

		if (m_c == L'&')
		{
			String name;

			bool bNumber = false;
			getcnext();
			if (m_c == '#')
			{
				bNumber = true;
				getcnext();
			}

			while (!eof())
			{
				if (!iswalnum(m_c) && m_c != L'_') break;
				//if (c == L' ' || c == L'\t' || c == L'\n' || c == L'\r') break;
				//if (c == L'<') break;

				if (m_c == L';')
				{
				//	ungetcnext();
					break;
				}

				ASSERT(0);
#if 0
				*name += c;
#endif
				getcnext();
			}

			if (m_c == L';')
			{
				uint16 uc;

				if (bNumber)
					ASSERT(0)
					;//uc = str2int(name->c_str());
				else
					uc = GetUCode(name);

				if (uc)
				{
					ASSERT(0);
#if 0
					*str += uc;
#endif
				}
				else
				{
					ASSERT(0);
#if 0
					if (entities)
					{
						if (TRUE)	// Use entity references
						{
							if (str.length())	// Create a text node for collected characters
							{
								Text* textNode = m_document->createTextNode(str);
								attr->appendChild(textNode);

								str = WSTR("");
							}

							EntityReference* entityReference = m_document->createEntityReference(name);

							attr->appendChild(entityReference);
						}
						else
						{
							Node* entity = entities->getNamedItem(name);
							if (entity)
							{
								Entity* pEntity = dynamic_cast<Entity*>(entity);

								str += pEntity->m_value;
							}
							else
							{
								OLECHAR msg[256];
								swprintf(msg, WSTR("Undeclared entity '%s'"), name.c_str());

								handleError(CreateDOMError(LSEVERITY_FATAL_ERROR, msg, getcnexturrentLocation()));
								THROW(msg);
							}
						}
					}
#endif
				}
			}
			else
			{
				ASSERT(0);
#if 0
				*str += L'&';
				*str += *name;
				*str += c;
#endif
			}
		}
		else
		{
			//str = str->Append(c);
			strbuilder << m_c;
#if 0
			*str += c;
#endif
		}
		getcnext();
	}

	String str = strbuilder.str();

	if (str.GetLength())	// Create a text node for collected characters
	{
		Text* textNode = m_document->createTextNode(str);
		attr->appendChild(textNode);

		//str = WSTR("");
	}

//	attr->put_value(str);

	attr->setTextOffset(4, m_fpos);

	if (delimiter) EatChar(delimiter);
//	SkipSpaces();
}

void XMLParser::SkipSpaces()
{
	while (!eof())
	{
		if (!iswspace(m_c))
		{
			break;
		}
		getcnext();
	}
}

void XMLParser::EatChar(int c)
{
	if (c != m_c)
	{
		WCHAR msg[64];
		//swprintf(msg, WSTR("Expected %c"), c);
		ASSERT(0);

		handleError(CreateDOMError(DOMErrorSeverity_FATAL_ERROR, msg, getcnexturrentLocation()));
		raise(Exception(msg));
	}
	getcnext();
}

ProcessingInstruction* XMLParser::ParseProcessingInstruction()
{
// <? has been read

	int fpos0 = m_fpos;

	String target = GetID();

	int fpos1 = m_fpos;

	SkipSpaces();

	int fpos2 = m_fpos;

	String data;

//	int c;

	while (!eof())
	{
		if (m_c == L'?')
		{
			break;
		}

		if (m_document)
		{
			data += m_c;
		}
		getcnext();
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

Comment* XMLParser::ParseCommentNode()
{
	int offset1 = m_fpos;
	int offset2;

	String data;

	while (!eof())
	{
		offset2 = m_fpos;

		if (m_c == L'-')
		{
			getcnext();
			if (!eof())
			{
				if (m_c == L'-')
				{
					getcnext();
					break;
				}
			}
		}

		if (m_document)
		{
			data += m_c;
		}

		getcnext();
	}

	EatChar('>');

	int offset3 = m_fpos;

	if (m_document)
	{
		Comment* comment = m_document->createComment(data);

		/*
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
		*/

		return comment;
	}
	else
		return NULL;
}

void XMLParser::ParseText(Node* parentNode, String& data)
{
//	int c;

//	int startoffset = m_fpos-data->Length();

	IO::StringWriter stream;

	while (!eof())
	{
		if (m_c == L'<')
		{
			break;
		}

		if (m_c == '&')
		{
			WCHAR buffer[256];
			unsigned int len = 0;

		//	StringW* t = WSTR("");

			int c;
			while (!eof())
			{
				if (m_c == ';') break;

				if (m_c == ' ' || m_c == '\t' || m_c == '\n' || m_c == '\r') break;
				if (m_c == '<') break;

				if (len > 250) raise(Exception(L"entity too long"));
				buffer[len++] = m_c;
#if 0
				*t += c;
#endif
				getcnext();
			}
			buffer[len] = 0;

		// TODO?, should make sure there's a ';'

			uint16 uc = GetUCode(buffer);
			if (uc)
			{
				stream << (WCHAR)uc;
#if 0
				*data += uc;
#endif
			}
			else
			{
					ASSERT(0);
				if (data.GetLength())	// Create a text node for collected characters
				{
				// TODO: text Offsets
					Text* textNode = NULL;//m_document->createTextNode(buffer.DetachToString()/*data*/);

					/*
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
					*/

					parentNode->appendChild(textNode);

					ASSERT(0);
				//	stream.clear();//data = WSTR("");
			//		startoffset = m_fpos;	// Reset for next text node
				}

				EntityReference* entityRef = NULL;//m_document->createEntityReference(t);

				if (entityRef)
				{
					parentNode->appendChild(entityRef);
				}
			}
		}
		else
		{
			stream << m_c;
		}

		getcnext();
	}

	data = stream.str();

	if (data.GetLength())	// Create a text node for collected characters
	{
		Text* textNode = m_document->createTextNode(data);

		/*
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
		*/

		parentNode->appendChild(textNode);

		data = nullptr;
	}
}

CDATASection* XMLParser::ParseCDATASection()
{
	ASSERT(0);
#if 0
	int offset0 = m_fpos-8;

	SkipSpaces();
	EatChar('[');
	SkipSpaces();

	BufferImp<WCHAR> buffer;
	StringBuilderW stream(&buffer);

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
			stream << m_textData[i];
#if 0
			*data += m_textData[i];//_bstr_t(c);
#endif
		}
	}

	StringW* data = buffer.DetachToString();

	if (data->Length() > 0)
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
#endif
	return NULL;
}

String FindNamespaceURIFromAttributes(NamedNodeMap* attributes, StringIn prefix)
{
	long length = attributes->get_length();
	for (long i = 0; i < length; i++)
	{
		Attr* attr = static_cast<Attr*>(attributes->item(i));

		/*
		BSTR battrName;
		attr->get_nodeName(&battrName);
		_bstr_t attrName = _bstr_t(battrName, false);
		*/

	//	if (!wcsicmp(attrName, WSTR("xmlns", 5))

		String attprefix = attr->get_prefix();

		if (attprefix == L"xmlns")//attprefix.length() && !tcscmp(attprefix.c_str(), WSTR("xmlns")))
		{
			String attlocalname = attr->get_localName();
		//	if ((BSTR)attlocalname == NULL)
		//		attlocalname = WSTR("");

		//	if ((SysStringLen(prefix) == 0 && SysStringLen(attlocalname) == 0) ||
		//		!tcscmp(attlocalname, prefix))
			if (attlocalname == prefix)
			{
				String value = attr->get_value();
				return value;
			}
		}
	}

	return nullptr;
}

String FindNamespaceURI(Node* parent, NamedNodeMap* attributes, StringIn prefix)
{
	if (attributes)
	{
		String namespaceURI = FindNamespaceURIFromAttributes(attributes, prefix);
		if (namespaceURI != nullptr) return namespaceURI;
	}

// Try parent element
	if (parent)
	{
		Node* parentParent = parent->get_parentNode();

		NamedNodeMap* parentAttributes = parent->get_attributes();
		return FindNamespaceURI(parentParent, parentAttributes, prefix);
	}

	return nullptr;
}

void XMLParser::ParseAttributes(Node* parent, NamedNodeMap* attributes)
{
//	int c;

	while (!eof())
	{
		IO::StringWriter strbuilder;
		CollectSpaces(strbuilder);
		String spaces = strbuilder.Detach();

		if (!iswalpha(m_c))
		{
			/*
			for (unsigned int i = 0; i < spaces.GetLength(); i++)
			{
				ungetcnext();
			}
			*/
			break;
		}

		int offset1 = m_fpos;

		String attrPrefix;
		String attrLocalName;
		GetNSID(attrPrefix, attrLocalName);

		if (attrLocalName == nullptr)
		{
			handleError(CreateDOMError(DOMErrorSeverity_FATAL_ERROR, L"Error parsing attributes", getcnexturrentLocation()));
			raise(Exception(L"Error parsing attributes"));
		}

		String qualifiedName;
		if (attrPrefix.GetLength())
		{
			qualifiedName = attrPrefix + ":" + attrLocalName;
		}
		else
		{
			qualifiedName = attrLocalName;
		}

		String namespaceURI;

		if (attrPrefix != nullptr)//->Length())
		{
			if (attrPrefix == L"xml")
			{
	// ??
				namespaceURI = L"http://www.w3.org/XML/1998/namespace";
			}
			else if (attrPrefix == L"xmlns")
			{
				namespaceURI = nullptr;	// no namespace
			}
			else
			{
				namespaceURI = FindNamespaceURI(parent, attributes, attrPrefix);
				if (namespaceURI.GetLength() == 0)
				{
					WCHAR msg[256];
					//swprintf(msg, WSTR("Namespace %s not found for attribute"), attrPrefix.c_str());
					ASSERT(0);

					handleError(CreateDOMError(DOMErrorSeverity_FATAL_ERROR, msg, getcnexturrentLocation()));
					raise(Exception(msg));
				}
			}
		}

		Attr* pAttr = m_document->createAttributeNS(namespaceURI, qualifiedName);
		if (pAttr == NULL)
		{
			WCHAR msg[64];
			//swprintf(msg, WSTR("Invalid attribute name %s"), qualifiedName.c_str());
			ASSERT(0);

			handleError(CreateDOMError(DOMErrorSeverity_FATAL_ERROR, msg, getcnexturrentLocation()));
			raise(Exception(msg));
		}

		/*
		pAttr->setTextOffset(0, offset0);	// space before attr name
		pAttr->setTextOffset(1, offset1);	// start of attr name
		pAttr->setTextOffset(2, m_fpos);		// end of attr name
*/
		SkipSpaces();

		if (m_c == L'=')
		{
			getcnext();

			SkipSpaces();
			GetAttrValue(pAttr);
		}
		else
		{
			if (true)	// xml requires attribute value
			{
				WCHAR msg[64];
				//swprintf(msg, WSTR("Expected ="));
				ASSERT(0);

			// However we only throw an error and not a fatalError
				if (!handleError(CreateDOMError(DOMErrorSeverity_ERROR, msg, getcnexturrentLocation())))
				{
					raise(Exception(msg));
				}
			}

			pAttr->setTextOffset(3, m_fpos);
			pAttr->setTextOffset(4, m_fpos);
		}

		pAttr->setTextOffset(5, m_fpos);
		pAttr->setTextOffset(6, m_fpos);
		pAttr->setTextOffset(7, m_fpos);
		pAttr->setTextOffset(8, m_fpos);
		pAttr->setTextOffset(9, m_fpos);

		attributes->m_items.push_back(pAttr);
	//	attributes->m_items[pAttr->get_nodeName()] = pAttr;
	}

//	SkipSpaces();
}

bool XMLParser::ParseChildren(Node* parentNode, Node* nsResolverParent, int xmlSpacePreserve)
{
	bool bEndTag = false;

	Element* pLastElement = NULL;

	while (!eof())
	{
		int spacesStartOffset = m_fpos;

		IO::StringWriter strbuilder;
		CollectSpaces(strbuilder);
		String spaces = strbuilder.str();

		if (pLastElement)
		{
			pLastElement->setTextOffset(9, m_fpos);
		//	pLastElement->Release();
			pLastElement = NULL;
		}

		if (eof()) break;

	//	if (c == EOF) break;

		if (m_c == L'<')
		{
			getcnext();

			if (spaces.GetLength())
			{
				if (xmlSpacePreserve)	// preserve
				{
					Text* pText = new Text;
					pText->set_ownerDocument(m_document);
					pText->m_data = spaces;

					// TODO pText->m_whiteSpaceInElementContent = TRUE ??

					/*
					pText->setTextOffset(0, spacesStartOffset);
					pText->setTextOffset(1, spacesStartOffset);
					pText->setTextOffset(2, spacesStartOffset);
					pText->setTextOffset(3, spacesStartOffset+spaces->length());
					pText->setTextOffset(4, spacesStartOffset+spaces->length());
					pText->setTextOffset(5, spacesStartOffset+spaces->length());
					pText->setTextOffset(6, spacesStartOffset+spaces->length());
					pText->setTextOffset(7, spacesStartOffset+spaces->length());
					pText->setTextOffset(8, spacesStartOffset+spaces->length());
					pText->setTextOffset(9, spacesStartOffset+spaces->length());
					*/

					parentNode->appendChild(pText);
				}

				spaces = L"";
			}

			if (m_c == L'!')
			{
				getcnext();
				if (m_c == L'[')	// CDATA section
				{
					getcnext();

					String id = GetID();
					if (id == L"CDATA")
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
						String msg = L"Expected CDATA";

						handleError(CreateDOMError(DOMErrorSeverity_FATAL_ERROR, msg, getcnexturrentLocation()));
						raise(Exception(msg));
					}
				}
				else if (m_c == L'-')	// Comment
				{
					EatChar('-');

					Comment* comment = ParseCommentNode();

					parentNode->appendChild(comment);
				}
				else	// DOCTYPE or DTD declaration
				{
					String id = GetID();

					if (id == L"DOCTYPE")
					{
						DocumentType* doctype = ParseDOCTYPE();

						parentNode->appendChild(doctype);
					}
					else if (id == L"ENTITY")
					{
					}
					else if (id == L"ELEMENT")
					{
					}
					else if (id == L"ATTLIST")
					{
					}
					else if (id == L"NOTATION")
					{
					}
					else
					{
						WCHAR msg[512];
						//swprintf(msg, WSTR("Unexpected ident %s"), id.c_str());
						ASSERT(0);

						handleError(CreateDOMError(DOMErrorSeverity_FATAL_ERROR, msg, getcnexturrentLocation()));
						raise(Exception(msg));
					}
				}
			}
			else if (m_c == L'?')	// Processing instruction
			{
				getcnext();
				ProcessingInstruction* pi = ParseProcessingInstruction();
				parentNode->appendChild(pi);
			}
			else if (m_c == L'/')	// End tag of this element
			{
				getcnext();
				bEndTag = true;
				break;
			}
			else // Beginning of new element
			{
				pLastElement = ParseElement(parentNode, nsResolverParent, xmlSpacePreserve);
			}
		}
		else	// Beginning of text element
		{
			ParseText(parentNode, spaces);
		}
	}

	return bEndTag;
}

Element* XMLParser::ParseElement(Node* parentNode, Node* nsResolverParent, int parentXmlSpacePreserve)
{
	int startoffset = m_fpos-1;	// right before the <

	int startoffset1 = m_fpos;

	String taglocalName;
	String tagprefix;
	GetNSID(tagprefix, taglocalName);

//	ATLTRACE("<%S>\n", (WCHAR*)taglocalName);

	if (taglocalName == nullptr)
	{
		String msg = L"Expected tagname after <";

		handleError(CreateDOMError(DOMErrorSeverity_FATAL_ERROR, msg, getcnexturrentLocation()));
		raise(Exception(msg));
	}

	String qualifiedName;
	if (tagprefix.GetLength())
	{
		qualifiedName = tagprefix + ":" + taglocalName;
	}
	else
	{
		qualifiedName = taglocalName;
	}

	long offset1 = m_fpos;
//	_bstr_t whitespace1 = CollectSpaces();
	//SkipSpaces();

// Parse the attributes
	NamedNodeMap* attributes = new NamedNodeMap;

// Add default attributes
	{
		DebugTrace("TODO\n");
#if 0
		CComQIPtr<ILDocumentAS, &IID_ILDocumentAS> documentAS((IUnknown*)m_document);

		<IASModel> asModel;
		documentAS->get_activeASModel(&asModel);

		if (asModel)
		{
			<IASNamedObjectMap> elementDeclarations;
			asModel->get_elementDeclarations(&elementDeclarations);

			StringW tagName = (BSTR)taglocalName;

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

							pAttr->put_nodeName(name);

							pAttr->put_nodeValue(dataValue);

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

	String namespaceURI = FindNamespaceURI(nsResolverParent, attributes, tagprefix);
	if (tagprefix && namespaceURI == nullptr)
	{
		delete attributes;

		WCHAR msg[512];
		//swprintf(msg, WSTR("Undefined namespace %s"), tagprefix.c_str());
		ASSERT(0);

		handleError(CreateDOMError(DOMErrorSeverity_FATAL_ERROR, msg, getcnexturrentLocation()));
		raise(Exception(msg));
	}

	Element* element = m_document->createElementNS(namespaceURI, qualifiedName, taglocalName);
	element->set_attributes(attributes);	// Set the previously parsed attributes onto the element

#if 0
	// TODO, do this here??
	{
		CComQIPtr<CLDOMElementImplImpl>(element)->AddEventListeners(element/*, CComQIPtr<CLDOMDocumentImplImpl>(m_document)->m_pScriptSite*/);
	}
#endif

	String xmlspace = element->getAttribute(L"xml:space");

	int xmlSpacePreserve;

	if (xmlspace != nullptr)
		xmlSpacePreserve = xmlspace == L"preserve";
	else
		xmlSpacePreserve = parentXmlSpacePreserve;

	//attributes->Release();

	DebugTrace("TODO\n");
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

	if (m_c == L'/')	// Immediate End tag of this element
	{
		getcnext();
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

	if (m_c != L'>')
	{
		String msg = L"Expected >";

		handleError(CreateDOMError(DOMErrorSeverity_FATAL_ERROR, msg, getcnexturrentLocation()));
		raise(Exception(msg));

		elementDone(element);
		return element;
	}
	getcnext();

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

		String endtaglocalName;
		String endtagprefix;
		GetNSID(endtagprefix, endtaglocalName);

		element->setTextOffset(7, m_fpos);	// at </element...>

		String endnamespaceURI;

		if (tagprefix == endtagprefix)
		{
			endnamespaceURI = namespaceURI;
		}
		else
		{
			endnamespaceURI = FindNamespaceURI(parentNode, attributes, endtagprefix);
			if (endtagprefix != nullptr && endnamespaceURI == nullptr)
			{
				WCHAR msg[512];
				//swprintf(msg, WSTR("Undefined namespace %s"), endtagprefix.c_str());
				ASSERT(0);

				handleError(CreateDOMError(DOMErrorSeverity_FATAL_ERROR, msg, getcnexturrentLocation()));
				raise(Exception(msg));
			}
		}

		SkipSpaces();

		if ((endtaglocalName != taglocalName) || (namespaceURI != endnamespaceURI))
		{
			WCHAR msg[512];
			//swprintf(msg, WSTR("End tag \"%s\" doesn't match \"%s\""), taglocalName.c_str(), endtaglocalName.c_str());
			ASSERT(0);

			handleError(CreateDOMError(DOMErrorSeverity_FATAL_ERROR, msg, getcnexturrentLocation()));
			raise(Exception(msg));

			elementDone(element);
			return element;
		}

		SkipSpaces();
		EatChar('>');
	}
	else
	{
		WCHAR msg[512];
		ASSERT(0);
		//swprintf(msg, WSTR("Missing endtag for \"%s\""), taglocalName.c_str());

		handleError(CreateDOMError(DOMErrorSeverity_FATAL_ERROR, msg, getcnexturrentLocation()));
		raise(Exception(msg));
	}

	element->setTextOffset(8, m_fpos);	// </element>...
	element->setTextOffset(9, m_fpos);	// TODO

	elementDone(element);

	return element;
}

void XMLParser::elementDone(Element* element)
{
#if 0
	BSTR btagName;
	element->get_tagName(&btagName);
	_bstr_t tagName = _bstr_t(btagName, false);

	if (!wcsicmp(tagName, WSTR("script")))
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
				element->getAttribute(WSTR("src", &bsrc);
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

					VARIANT_BOOL bloaded;
					textfile->load(_bstr_t(abspath), &bloaded);
					if (bloaded)
					{
						BSTR textdata;
						textfile->get_data(&textdata);

						long hr;

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

						long hr;

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
	else if (!wcsicmp(tagName, WSTR("style"))
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
				htmlstylesheet->put_cssText(textdata);	// The stylesheet will parse the text
			}
		}
	}
#endif
//	Fire_ParsedElementDone(element);
}

Document* XMLParser::get_document() const
{
	return m_document;
}

/*
void XMLParser::put_document(Document *newVal)
{
	m_document = newVal;
}
*/

bool XMLParser::parse()
{
	return !loadXML2();
}

long XMLParser::ParseXMLHeader()
{
	String xmlid = GetID();
	if (xmlid != L"xml")
	{
// Not an xml document
		//MessageBox(NULL, "Not an xml document", "XML", MB_OK);
		raise(Exception(L"Not an xml document"));
	}

	//Read <?xml ... ?>
	while (!eof())
	{
		SkipSpaces();

		if (!iswalpha(m_c))
		{
			break;
		}

		String name = GetID();

		SkipSpaces();
		EatChar(L'=');
		SkipSpaces();

		String value = GetString();

		if (name == L"version")
			;//m_version = value;
		else if (name == L"encoding")
			;//m_encoding = value;
		else if (name == L"standalone")
		{
			if (value == L"yes")
				;//m_standalone = TRUE;
			else if (value == L"no")
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

DocumentType* XMLParser::ParseDOCTYPE()
{
// <!DOCTYPE has been read

	DocumentType* doctype = new DocumentType;
	if (doctype)
	{
		doctype->set_ownerDocument(m_document);

		doctype->setTextOffset(0, m_fpos-9);
		doctype->setTextOffset(1, m_fpos-7);
		doctype->setTextOffset(2, m_fpos);

		SkipSpaces();

		doctype->setTextOffset(3, m_fpos);

		doctype->m_name = GetID();
		//doctype->m_name = name;//put_name(name);

		SkipSpaces();

		String more = GetID();
		SkipSpaces();

		if (more == L"PUBLIC")
		{
			doctype->m_publicId = GetString();

			SkipSpaces();

			if (m_c != '>')
			{
				doctype->m_systemId = GetString();
			}
		}
		else if (more == L"SYSTEM")
		{
			doctype->m_systemId = GetString();
		}
		else
		{
			String msg = L"Expected PUBLIC or SYSTEM";

			handleError(CreateDOMError(DOMErrorSeverity_FATAL_ERROR, msg, getcnexturrentLocation()));
			raise(Exception(msg));
		}

		SkipSpaces();

	//	CComObject<CDTDDocument>::CreateInstance(&doctype->m_dtd);
	//	doctype->m_dtd->AddRef();

		doctype->setTextOffset(4, m_fpos);

	// Internal subset
		if (m_c == L'[')
		{
			getcnext();

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

			long hr = dtdParser.Parse2();

			m_line = dtdParser.m_line;
			m_column = dtdParser.m_column;
			m_fpos = dtdParser.m_fpos;

			doctype->setTextOffset(6, m_fpos);
			EatChar(L']');
#endif
		}

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
				long hr = dtdParser.loadURL(absurl);
				if (FAILED(hr))
				{
					OLECHAR msg[512];
					swprintf(msg, WSTR("Failed to load external subset url: %s", absurl);

					if (!handleError(CreateDOMError(LSEVERITY_ERROR, msg, getcnexturrentLocation())))
					{
						THROW(msg);
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

long XMLParser::loadXML2()
{
	DebugTrace("loadXML2()...\n");

	getcnext();

	bool bXML;

	if (m_document == NULL)
	{
// Find out what kind of document this is
		DocumentType* doctype;

		m_line = 0;
		m_column = 0;
		m_oldc = EOF;
		m_fpos = 0;

		//m_bXML = false;

		TRY
		{
			SkipSpaces();

		// Process document children
			while (!eof())
			{
				EatChar('<');

				if (m_c == '?')
				{
					getcnext();
					ParseXMLHeader();
					bXML = true;
				}
				else if (m_c == L'!')
				{
					getcnext();

					if (m_c == L'-')
					{
						getcnext();

						if (m_c == L'-')
						{
							// Comment
							Comment* comment = ParseCommentNode();
						}
					}
					else	// Maybe DOCTYPE
					{
						String id = GetID();
						if (id == L"DOCTYPE")
						{
							if (doctype != NULL)	// Can only have one doctype node
							{
								String msg = L"Can only have one doctype node";

								if (!handleError(CreateDOMError(DOMErrorSeverity_ERROR, msg, getcnexturrentLocation())))
								{
									raise(Exception(msg));
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
							WCHAR* msg = WSTR("Expected DOCTYPE";

							handleError(CreateDOMError(LSEVERITY_FATAL_ERROR, msg, getcnexturrentLocation()));
							THROW(msg);
							*/

						}
					}
				}
				else if (m_c == L'?')
				{
					getcnext();
					// Processing instruction
					ProcessingInstruction* pi = ParseProcessingInstruction();
				}
				else
				{
					SkipSpaces();

					String taglocalName;
					String tagprefix;
					GetNSID(tagprefix, taglocalName);

					if (taglocalName == nullptr)
					{
						String msg = L"Expected tagname after <";

						handleError(CreateDOMError(DOMErrorSeverity_FATAL_ERROR, msg, getcnexturrentLocation()));
						raise(Exception(msg));
					}

#if 0
					if (!tcscmp(taglocalName, WSTR("html"))
					{
						m_document.CoCreateInstance(CLSID_LHTMLDocument);
					}
					else if (!tcscmp(taglocalName, WSTR("svg"))
					{
						//m_document.CoCreateInstance(CLSID_LSVGDocument);
						m_document.CoCreateInstance(WSTR("LSVG.LSVGDocument");
					}
					else if (!tcscmp(taglocalName, WSTR("smil"))
					{
						m_document.CoCreateInstance(CLSID_LSMILDocument);
					}
					else if (!tcscmp(taglocalName, WSTR("lxui"))
					{
						m_document.CoCreateInstance(WSTR("LXUI.LXUIDocument");
					}
					else if (!tcscmp(taglocalName, WSTR("nitf"))
					{
						m_document.CoCreateInstance(CLSID_LNITFDocument);
					}
					else if (!tcscmp(taglocalName, WSTR("NewsML"))
					{
						m_document.CoCreateInstance(CLSID_LNewsMLDocument);
					}
#if 0
					else if (!tcscmp(taglocalName, WSTR("schema"))
					{
						m_document.CoCreateInstance(CLSID_XMLSchemaDocument);
					}
#endif
					else
#endif
						ASSERT(0);
#if 0
					{
						m_document = new LXML::CLDOMDocument;//.CoCreateInstance(CLSID_LDOMDocument);
					}
#endif

					if (m_document == NULL)
					{
						raise(Exception("Couldn't create document"));
					}

#if 0
					CComQIPtr<CLDOMDocumentImplImpl>(m_document)->m_acDocument = m_activeDocument;
#endif
					ASSERT(0);
#if 0
					m_document->put_errorHandler(m_errorHandler);
#endif
				//	m_document->put_isXML(m_bXML);

#if 0
					// hm.. do this here ?
					if (TRUE)
					{
						LCSS::CLDocumentStyleImpl* documentStyle = dynamic_cast<LCSS::CLDocumentStyleImpl*>(m_document);
						if (documentStyle)
						{
							TCHAR appdata[MAX_PATH];
#if 0//_WINDOWS
							SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, appdata);

							CLCSSStyleSheet* stylesheet = new CLCSSStyleSheet;//.CoCreateInstance(CLSID_LCSSStyleSheet);
							if (stylesheet)
							{
								TCHAR pathname[MAX_PATH];
								_makepath(pathname, NULL, appdata, "LXFramework", NULL);

								TCHAR filename[MAX_PATH];

								if (!tcscmp(taglocalName, WSTR("nitf")))
								{
									_makepath(filename, NULL, pathname, "nitf.css", NULL);
								}
								else if (!tcscmp(taglocalName, WSTR("article")))	// DocBook
								{
									_makepath(filename, NULL, pathname, "docbook.css", NULL);
								}
								else if (!tcscmp(taglocalName, WSTR("stylesheet")))
								{
									_makepath(filename, NULL, pathname, "xslt.css", NULL);
								}
								else if (!tcscmp(taglocalName, WSTR("html")) ||
									!tcscmp(taglocalName, WSTR("svg")) ||
									!tcscmp(taglocalName, WSTR("lxui")))
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
#endif

					break;
				}

				SkipSpaces();
			}
		}
		CATCH (Exception* p)
		{
			DebugTrace("\n\nERROR FIRST PASS XML\n\n");
			return -1;
		}
	}
	else
	{
	}

	if (m_document)
	{
		long hr = -1;	// Assume error

		m_line = 0;
		m_column = 0;
		m_oldc = EOF;
		m_fpos = 0;

		m_document->setTextOffset(0, m_fpos);
		m_document->setTextOffset(1, m_fpos);
		m_document->setTextOffset(2, m_fpos);
		m_document->setTextOffset(3, m_fpos);
		m_document->setTextOffset(4, m_fpos);

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

			long hr = dtdParser.Parse();

			static_cast<CDTDDocument*>(m_document.p)->m_pASModel->BuildFromDTDDocument(static_cast<CDTDDocument*>(m_document.p));

			return hr;
		}
#endif

		TRY
		{
			SkipSpaces();

		// Read <?xml ... >
			EatChar('<');
			SkipSpaces();

			if (m_c == '?')
			{
				getcnext();
				ParseXMLHeader();
			}
			/*
			else
			{
				ungetcnext();
				ungetcnext();
			}
			*/

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
					documentElement->getAttribute(WSTR("xsi:noNamespaceSchemaLocation", &bnoNSSchemaLocation);
					_bstr_t noNSSchemaLocation = _bstr_t(bnoNSSchemaLocation, false);

					BSTR bschemaLocation;
					documentElement->getAttribute(WSTR("xsi:schemaLocation", &bschemaLocation);
					_bstr_t schemaLocation = _bstr_t(bschemaLocation, false);

					_bstr_t schemaURL;

					if (schemaLocation.length())
					{
						_bstr_t uri;
						_bstr_t ns;

						StringW p = (BSTR)schemaLocation;

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
						VARIANT_BOOL bloadedSchema;
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
								mergeDocumentElement->getAttribute(WSTR("targetNamespace", &b);
								if (b)
								{
									pSchema->m_targetNamespace = b;
									//pSchema->m_schemaElement->setAttribute(WSTR("targetNamespace", b);
									SysFreeString(b);
								}
							}

							{
								BSTR b;
								mergeDocumentElement->getAttribute(WSTR("elementFormDefault", &b);
								if (b)
								{
									pSchema->m_elementFormDefault = b;
									//pSchema->m_schemaElement->setAttribute(WSTR("elementFormDefault", b);
									SysFreeString(b);
								}
							}

							{
								BSTR b;
								mergeDocumentElement->getAttribute(WSTR("attributeFormDefault", &b);
								if (b)
								{
									pSchema->m_attributeFormDefault = b;
									//pSchema->m_schemaElement->setAttribute(WSTR("attributeFormDefault", b);
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
			catch(StringW p)
			{
				ATLTRACE("\n\nERROR VALIDATING XML: %S\n\n", p);
			}
#endif

			{
#if 0//_WINDOWS
				if (CComQIPtr<CLDOMDocumentImplImpl>(m_document)->m_pScriptSite->m_debugDocumentHelper)
				{
					BSTR textdata;
					m_textDoc->get_data(&textdata);
					// Note, not freeing

					long hr = CComQIPtr<CLDOMDocumentImplImpl>(m_document)->m_pScriptSite->m_debugDocumentHelper->AddUnicodeText(textdata);
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

			DebugTraceLn("Successfully Parsed XML document");

			hr = 0;	// Completely successfull
		}
		CATCH (Exception* e)
		{
		//	TRACE("\n\nPartially failed loading XML: %S\n\n", p);
			DebugTraceLn("\n\nPartially failed loading XML\n");

			hr = 1;
		}

		m_document->setTextOffset(5, m_fpos);
		m_document->setTextOffset(6, m_fpos);
		m_document->setTextOffset(7, m_fpos);
		m_document->setTextOffset(8, m_fpos);
		m_document->setTextOffset(9, m_fpos);

		// Do this even if loading xml partially failed
		{

			if (m_document->get_documentElement())
			{
#if 0	// Had this
				{
					<ILDOMNodeList> extensionDefsElements;
					m_document->getElementsByTagNameNS(WSTR("http://www.w3.org/2000/svg"), WSTR("extensionDefs"), &extensionDefsElements);

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

		DebugTraceLn("loadXML2() = " << hr);

		return hr;
	}
	else
		return -1;
}

bool XMLParser::validateToSchema(Document* schemaDoc)
{
/*
	CComQIPtr<IDOMElement> element;
	m_document->get_documentElement(&element);

	*pVal = ValidateElement(schemaDoc, element);
*/
	return 0;
}

bool XMLParser::load(IO::TextReader* stream)
{
	m_stream = stream;

	bool success = !loadXML2();
	ASSERT(success);

	m_stream = NULL;

//	delete textData;

	return success;
}

bool XMLParser::load(IO::Stream* stream)
{
	IO::StreamReader reader(stream);
	return load(&reader);
}

bool XMLParser::load(StringIn url)
{
	IO::FileStream stream(url, IO::FileMode_Open, IO::FileAccess_Read);

	if (IO::FileStream::GetLastError() != 0)
	{
		ASSERT(0);
		return false;
	}

	return load(&stream);
}

bool XMLParser::loadXML(StringIn sourceText)
{
	IO::StringReader reader(sourceText);
	return load(&reader);
}

#if 0
STDMETHODIMP XMLParser::get_errorHandler(ILDOMErrorHandler **pVal)
{
	*pVal = m_errorHandler;
//	if (*pVal) (*pVal)->AddRef();

	return S_OK;
}

STDMETHODIMP XMLParser::put_errorHandler(ILDOMErrorHandler *newVal)
{
	m_errorHandler = newVal;
	return S_OK;
}
#endif

IDOMLocator* XMLParser::getcnexturrentLocation()
{
	return NULL;
#if 0
	CLDOMLocator* locator = new CLDOMLocator;
	if (locator)
	{
		locator->m_offset = m_fpos;
		locator->m_lineNumber = m_line;
		locator->m_columnNumber = m_column;
		locator->m_uri = m_uri;
	}

	return locator;
#endif
}

}	// XmlData
}
