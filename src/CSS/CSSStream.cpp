#include "stdafx.h"
//#include "LXML.h"
#include "CSS.h"
#include "CSSStream.h"

namespace System
{
namespace Style
{

bool CSSStream::eof()
{
	return m_c == -1;
	//return m_data->m_position == m_data->str().GetLength();

#if 0
	if (m_data == NULL) return true;
	return (*m_ppos == 0);
#endif
	return 0;
}

void CSSStream::getnextc()
{
	if (m_data->state())
	{
		m_c = -1;
		return;
	}

	wchar_t c;
	m_data >> c;

	if (m_data->state())
	{
		m_c = -1;
		return;
	}

	m_c = c;
}

void CSSStream::EatChar(int c)
{
	if (m_c != c)
	{
		WCHAR msg[64];
		swprintf_s(msg, L"Expected %c", c);

		raise(Exception(msg));
	}
	SkipSpacesComments();

	getnextc();
}

void CSSStream::SkipSpaces()
{
//	int _c = getnextc();

	while (isspace(m_c))
	{
		getnextc();
	//	m_ipos++;
	//	m_ppos++;
	}

	/*
	while (*m_ppos && isspace(*m_ppos))
	{
		m_ipos++;
		m_ppos++;
	}
	*/
}

void CSSStream::SkipSpacesComments()
{
	while (!eof())
	{
		if (m_c == L'/')
		{
			getnextc();
			if (m_c == L'*')
			{
				getnextc();
				while (!eof())
				{
					if (m_c == L'*')
					{
						getnextc();
						if (eof())
						{
							break;
						}
						if (m_c == L'/')
						{
							break;
						}
					}
					getnextc();
				}

				/*
				getnextc();
				EatChar(L'*');

				getnextc();
				*/
				EatChar(L'/');
			}
			else
				;//ungetnextc();
		}
		else
		{
			if (!isspace(m_c))
			{
				break;
			}
		}

		getnextc();
	}
}

String CSSStream::GetID()
{
	String str;

	while (!eof())
	{
		if (m_c == L'\\')	// escaped symbol
		{
			getnextc();	// next character is real character

			if (!(isalnum(m_c) || (m_c == ':') || (m_c == '.') || (m_c == '-')))
			{
				break;
			}
		}
		else
		{
			if (!(isalnum(m_c) || (m_c == '-')))
			{
				break;
			}
		}

		str += (WCHAR)m_c;
		getnextc();
	}

	if (str.GetLength() == 0)
	{
		;//throw OLESTR("Expected an identifier");
	}

	SkipSpacesComments();

	return str;
}

String CSSStream::GetString()
{
	IO::StringWriter strbuilder;

	int delimiter = m_c;

	if (delimiter == L'\'' || L'\"')
	{
		while (!eof())
		{
			if (m_c == delimiter)
			{
				getnextc();
				break;
			}

			strbuilder << (WCHAR)m_c;
			getnextc();
		}

		EatChar(delimiter);
	}
	else
		;//throw OLESTR("Expected string");

	String str = strbuilder;

	SkipSpacesComments();

	return str;
}

}	// Web
}
