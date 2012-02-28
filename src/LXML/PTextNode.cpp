#include "stdafx.h"
#include "LXML2.h"

#include "Window.h"
#include "LXMLDocumentView.h"

#include "PTextNode.h"
#include "PWord.h"
#include "PParentBox.h"

#include "HTMContext.h"

//double ConvertSpecifiedValueToUserUnit(short unitType, double specifiedValue/*, double percentageOf*/);

namespace System
{
namespace Web
{

PCharacterData::PCharacterData(CharacterData* chardata) : PNode(chardata)
{
	m_type = NODE_TEXT;
}

void PCharacterData::OnTextContentChanged()
{
	//CComQIPtr<ILDOMCharacterData> charData(m_pNode);

//	ASSERT(0);
/*
	{
		HDC hDC = CreateCompatibleDC(NULL);
		{
			Gdiplus::Graphics graphics(hDC);

			CHTMCalcContext pC;
			//pC.m_pUI = m_pWindow->m_pUI;
			pC.m_pGraphics = &graphics;

			pC.m_pStringFormat = Gdiplus::StringFormat::GenericTypographic();

			graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintSystemDefault);	// ??
*/
		//	CalculateBoundsAndChildren();//CalculateDimensions(NULL);
/*		}
		DeleteDC(hDC);
	}
*/
// TODO, have common code for both svg and css box model
	if (m_rparent)//m_pWindow->m_pSVGElement == NULL)
	{
		PElementBase* pParent = m_rparent;

		pParent->RemoveBoxes();

		while (pParent)
		{
			if (pParent->m_computedDisplayRole == DisplayRole_table_cell)
			{
				pParent->CalculateMinMaxDimensions();
			}

			pParent = pParent->m_rparent;
		}
	}
#if 0
	else
	{
		CPElement* pParent = pText->m_parent;

		while (pParent)
		{
			CComQIPtr<ILSVGTextElement> textElement = pParent->m_pNode;
			if (textElement)
			{
				CPSVGTextElement* pSVGTextElement = (CPSVGTextElement*)pParent;
				pSVGTextElement->CalculateBounds();

				pSVGTextElement->m_parent->RecalculateParentBounds();

				break;
			}

			pParent = pParent->m_parent;
		}
	}
#endif
//	m_parent->CalculateBoundsAndChildren();
//	m_parent->RecalculateParentBounds();
}

////////////////////////////////////////////////////////////////////////
// PText

PText::PText(Text* textNode) : PCharacterData(textNode)
{
	m_ascentPixel = 0;
	m_realNode = NULL;
	m_realOffset = 0;
	m_bComputedWords = false;

	m_bArrangeValid = false;
}

PText::~PText()
{
	RemoveWords();
}

void PText::RemoveWords()
{
//	CArray<CHTMPCharacterBox*, CHTMPCharacterBox*>::iterator	it = m_wordList.begin();//GetHeadPosition();
//	while (it != m_wordList.end())
	for (int i = 0; i < m_wordList.GetSize(); i++)
	{
		CHTMPCharacterBox* pWord = m_wordList[i];//*it);//(CHTMPCharacterBox*)m_wordList.GetNext(pos);

		if (pWord->m_parentBox)
			pWord->m_parentBox->RemoveChildBox(pWord);

#if 0
		pWord->Release();	// BuildWords() AddRefs, so I must have a matching here?
#endif
		//delete pWord;
		//it++;
	}
	m_wordList.RemoveAll();
}

void PText::OnTextContentChanged()
{
#if 0
	RemoveWords();
	BuildWords();
#endif

	CalculateBoundsAndChildren();
	baseClass::OnTextContentChanged();	// Call base class
}

//void PText::CalculateDimensions(CHTMCalcContext* pC)
void PText::CalculateBoundsAndChildren(/*CHTMCalcContext* pC*/)
{
#if 0
	if (m_bComputedWords)
		return;
#endif
// TODO, somewhere else
	RemoveWords();
	BuildWords();

	m_bComputedWords = true;

#if 1
	if (m_rparent)
	{
		m_ascentPixel = 8;
#if 0
		ASSERT(m_rparent->GetFont());
		LDraw::Font* pFont = m_rparent->GetFont()->m_pFont;
		ASSERT(pFont);
		float ascent = pFont->GetCellAscent();

		m_ascentPixel = ascent;//ConvertSpecifiedValueToUserUnit(CSS_PT, ascent);
#endif

	//	std::vector<CHTMPCharacterBox*>::iterator pos = m_wordList.begin();
	//	while (pos != m_wordList.end())
		for (int i = 0; i < m_wordList.GetSize(); i++)
		{
			CHTMPCharacterBox* pNode = m_wordList[i];//*pos++;
			pNode->CalculateDimensions(NULL/*pC*/);
		}
	}
	else
	{
		m_ascentPixel = 0;
	}

//	m_ascentPixel = 20;	// TODO
#else
	{
	//	Gdiplus::Font font(pBox->m_pElement->m_fontFamily, pBox->m_pElement->m_fontSize);
		Gdiplus::FontFamily fontFamily;
		m_rparent->m_computedFontFamily[0]->GetGdipFont()->GetFamily(&fontFamily);
		float ascent = m_rparent->m_computedFontFamily[0]->GetGdipFont()->GetSize() * fontFamily.GetCellAscent(Gdiplus::FontStyleRegular) / fontFamily.GetEmHeight(Gdiplus::FontStyleRegular);

		m_ascentPixel = ascent;//ConvertSpecifiedValueToUserUnit(CSS_PT, ascent);

	//	ascents.Add(ascentPixel);
	//	m_maxascent = max(m_maxascent, ascentPixel);
	}
#endif

}

#if 0
struct CHARENTITY
{
	WCHAR* ename;
	WCHAR c;
};

CHARENTITY CharEntities[] =
{
	OLESTR("aelig"), L'�',
	OLESTR("AElig"), L'�',
	OLESTR("amp"), L'&',
	OLESTR("apos"),  L'\'',
	OLESTR("aring"), L'�',
	OLESTR("Aring"), L'�',
	OLESTR("auml"),  L'�',
	OLESTR("copy"), L'�',
	OLESTR("gt"), L'>',
	OLESTR("laquo"), L'�',
	OLESTR("lt"), L'<',
	OLESTR("nbsp"), L' ',	// TODO
	OLESTR("oslash"),  L'�',
	OLESTR("Oslash"), L'�',
	OLESTR("quot"), L'\"',
	OLESTR("raquo"), L'�',
	OLESTR("reg"), L'�',
};

#define N_CHARENTITIES	(sizeof(CharEntities)/sizeof(CHARENTITY))

WCHAR FindCharEntity(WCHAR* ename, int elen)
{
	for (int i = 0; i < N_CHARENTITIES; i++)
	{
		if (!wcsncmp(CharEntities[i].ename, ename, elen))
		{
			return CharEntities[i].c;
		}
	}

	return 0;
}
#endif

void PText::BuildWords()
{
	if (m_rparent == NULL) return;
	CharacterData* charData = static_cast<CharacterData*>(m_pNode);

	ASSERT(m_wordList.GetSize() == 0);	// No words

	unsigned int len = charData->get_length();
	String::const_iterator<WCHAR> data = charData->get_data().begin();
	String::const_iterator<WCHAR> p = data;

	int i = 0;
	while (i < len)
	{
		/*
	// Skip spaces
		while ((i < len) && iswspace(p[i]))
		{
			i++;
		}
		*/

		unsigned int n;

		if (p[i] < 0xD800 || p[i] > 0xDFFF)
			n = 1;
		else
			n = 2;

		int startoffset = i;

		bool whitespace = (p[i] == ' ' || p[i] == 0xA);

		if (!whitespace)
		{
			String wordtext;

			bool bEntity = false;
		// Add characters to text item
			while ((i < len) && !((p[i] == ' ' || p[i] == 0xA)))
			{
				if (false && p[i] == L'&')
				{
//					ASSERT(0);
#if 0
					int estart = i;

					i++;

					WCHAR* ename = &data[i];
					int elen = 0;

					WCHAR c;
					if (ename[0] == L'#')
					{
						i++;
						while (i < len)
						{
							c = p[i++];

							if (!iswdigit(c)) break;

							//elen++;
						}
					}
					else
					{
						while (i < len)
						{
							c = p[i++];

							if (!iswalpha(c)) break;

							elen++;
						}
					}

					if (c == ';')
					{
						WCHAR ec;

						if (ename[0] == L'#')
						{
							ec = _wtol(&ename[1]);
						}
						else
						{
							ec = FindCharEntity(ename, elen);
						}

						if (ec)
						{
							if (!bEntity)	// First entity encountered
							{
								wordtext.append(&data[startoffset], estart-startoffset);
							}

							wordtext += ec;

							bEntity = true;
						}
						else
						{
							if (bEntity)
								wordtext.append(&data[estart], i-estart);
						}
					}
					else
					{
						if (bEntity)
							wordtext.append(&data[estart], i-estart);
					}
#endif
				}
				else
				{
					if (bEntity)
						wordtext += p[i];

					i++;
				}
			}

		//	if (i > startoffset)	// ??
			{
				CHTMPWord* pWord = new CHTMPWord(this);
				pWord->m_start = startoffset;

				pWord->m_bFreeText = bEntity;
				if (bEntity)
				{
					ASSERT(0);
#if 0
					pWord->m_len = wordtext.length();
					pWord->m_text = wordtext.copyBSTR();
#endif
				}
				else
				{
					pWord->m_len = i-startoffset;
					//ASSERT(startoffset == 0);
				//	pWord->m_text = &data[startoffset];
				}

				pWord->AddRef();
				m_wordList.Add(pWord);
			}
		}
		else
		{
	//	if ((i < len) && iswspace(p[i]))
			int c = p[i];
			while ((i < len) && (p[i] == c))
			{
				i++;
			}

			if (c != 0xA || m_rparent->m_computedLineFeedTreatment == LineFeedTreatment_preserve)
			{
				CHTMPSpace* pSpace = new CHTMPSpace(this);
				if (c == 0xA) pSpace->m_type = c;

				pSpace->m_start = startoffset;
				pSpace->m_len = i-startoffset;

				pSpace->AddRef();
				m_wordList.Add(pSpace);
			}

			//i++;
		}
	}
}

bool PText::GetCursorPosition(int offset, gm::PointD* pos, double* pHeight, gm::matrix3f* pMat3)
{
	int wordoffset;
	CHTMPCharacterBox* pCharBox = GetWordAtCursorPos(offset, wordoffset);
	if (pCharBox)
	{
		gm::RectF drect = pCharBox->GetAbsOuterRect();

		// TODO, I crashed with overflow here once
		pos->X = pCharBox->m_chars[wordoffset].offset + drect.X;
		pos->Y = drect.Y;
		*pHeight = drect.Height;

		*pMat3 = gm::matrix3f::getIdentity();

		return true;
	}

	return false;
}

CHTMPCharacterBox* PText::GetWordAtCursorPos(int cursorPos, int& wordCursorPos)
{
	ASSERT(cursorPos >= 0);

	for (int pos = m_wordList.GetSize()-1; pos >= 0; pos--)
	{
		CHTMPCharacterBox* pWord = m_wordList[pos];

		if (pWord->m_start <= cursorPos)
		{
			wordCursorPos = cursorPos-pWord->m_start;
			return pWord;
		}
	}
//	ASSERT(0);

	wordCursorPos = 0;
	return NULL;
}

void PText::DistributeInline(CHTMFlowInlineContext* pInlineC)
{
//	std::vector<CHTMPCharacterBox*>::iterator	it = m_wordList.begin();//GetHeadPosition();
//	while (it != m_wordList.end())
	for (int i = 0; i < m_wordList.GetSize(); i++)
	{
		CHTMPCharacterBox* pBox = m_wordList[i];//(*it);//(CHTMPCharacterBox*)m_wordList.GetNext(pos);

		pBox->m_parentBox = NULL;
		pBox->m_outerRect.X = 0;
		pBox->m_outerRect.Y = 0;
		// Width/Height are aldready set

		pInlineC->PlaceInlineBox(pBox);
	//	it++;

		/*
		if (m_pWindow->m_bLayoutComplete == false)
		{
			break;
		}
		*/
	}
}

//virtual
void PText::DistributeInline(CHTMFlowContext* pParentC, PElementBase* nearestPositioned, PElementBase* stackingContext, double oparentwidth)
{
	if (m_bArrangeValid)
		;//return;

	m_bArrangeValid = true;

/*
	NOTE:

	If pParentC is a CHTMFlowBlockContext then this text will be in anonymous box(es)
	If pParentC is a CHTMFlowInlineContext then this text will be in box(es) associated with an inline element
*/

	if (pParentC->m_type == 1)	// block
	{
		PElementBase* pParent = m_rparent;

		CHTMFlowInlineContext pInlineC;// = new CHTMFlowInlineContext;
		pInlineC.m_pParentC = pParentC;
		pInlineC.m_pElement = pParent;
		pInlineC.m_bAnonymous = true;
		pInlineC.m_floatLeft = pParentC->m_floatLeft;	//

		DistributeInline(&pInlineC);

		pParentC->m_floatLeft = pInlineC.m_floatLeft;	//

		//delete pInlineC;
	}
	else	// inline
	{
		DistributeInline((CHTMFlowInlineContext*)pParentC);
	}
}

}	// Web
}	// System
