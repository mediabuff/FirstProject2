#include "StdAfx.h"
#include "Comp2.h"
#include "preprocessor.h"

#include "cpp_compile.h"	// TODO remove ??

namespace System
{
using namespace std;

// TODO, not here
class CCompiler
{
public:
	virtual void OnPragma() = 0;
};

#if 0
double makeflt(long intpart, double frac, int exp)
{
	double val;
	double mod, mod10;//, mod100;

	val = intpart + frac;
	if (exp > 0)
	{
		mod = 1e1;
		mod10 = 1e10;
#if IEEE_FP
		mod100 = 1e100;
#endif
	}
	else if (exp < 0)
	{
		mod = 1e-1;
		mod10 = 1e-10;
#if IEEE_FP
		mod100 = 1e-100;
#endif
		exp = -exp;
	}
#if IEEE_FP
	while (exp >= 100)
	{
		val *= mod100;
		exp -= 100;
	}
#endif
	while (exp >= 10)
	{
		val *= mod10;
		exp -= 10;
	}

	while (exp--)
	{
		val *= mod;		/* slow and dirty */
	}

	return val;
}
#endif

namespace PP
{
enum
{
	PP_IF,
	PP_ELIF,
	PP_ELSE,
	PP_ENDIF,
	PP_IFDEF,
	PP_IFNDEF,
	PP_INCLUDE,
	PP_DEFINE,
	PP_UNDEF,
	PP_LINE,
	PP_ERROR,
	PP_PRAGMA,

	PP_IMPORT,
//
	PP_MAX
};

char* pp_kws[] =
{
	"if",
	"elif",
	"else",
	"endif",
	"ifdef",
	"ifndef",
	"include",
	"define",
	"undef",
	"line",
	"error",
	"pragma",

	"import",
};

int _getnumber(CPreprocessor* pPP)
{
	// TODO

	int c;

	int int32 = 0;

	while (!pPP->_eof())
	{
		c = pPP->_getc();
		if (!(c >= '0' && c <= '9'))
		{
			pPP->_ungetc(c);
			break;
		}

		int32 *= 10;
		int32 += c - '0';
	}

	return 0;
}

double getfrac(CPreprocessor* pPP)
{
	double dig = 0.1;
	double val = 0;

	//OLECHAR* p = *s;

	while (!pPP->_eof())
	{
		int c = pPP->_getc();//WCHAR c = *p;
		if (!iswdigit(c))
		{
			pPP->_ungetc(c);
			break;
		}
		val += dig * (c - L'0');

		dig = 0.1 * dig;

		//p++;
	}

	//*s = p;

	return val;
}

int lookexp(CPreprocessor* pPP/*, long intpart, double frac*/)
{
	int c;
	int exp;

	//OLECHAR *p = *s;

	int minus = 0;

	c = pPP->_getc();//*p++;
	if (towlower(c) != 'e')
	{
		pPP->_ungetc(c);
		return 0;
	}

	c = pPP->_getc();//*p++;
	if (c == '-')
	{
		minus = 1;
		//c = mygetchar();
		c = pPP->_getc();
	}
	else if (c == '+')
	{
		//c = mygetchar();
	}

	//c = *p++;
	if (c >= '0' && c <= '9')
	{
		exp = _getnumber(pPP);
	}
	else
	{
		pPP->_ungetc(c);
		return 0;
		//exp = 0;
		//p--;//myungetc(c);
	}

	if (minus)
		exp = -exp;

//	*s = p;

	return exp;
}

CFile::CFile()
{
	m_pPrev = nullptr;
	m_data = nullptr;

	m_lastc = -1;
	m_fpos = 0;
	m_line = 0;
	m_column = 0;
}

CFile::~CFile()
{
	delete[] m_data;
}

bool CFile::open(StringIn filepath)
{
	m_filepath = filepath;

	try
	{
		IO::FileStream fp(filepath, IO::FileMode_Open, IO::FileAccess_Read);

		FILETIME creation;
		FILETIME lastAccess;
	//	FILETIME lastWrite;
		GetFileTime(fp.GetFileHandle(), &creation, &lastAccess, &m_filetime);
		m_size = fp.GetSize();
	//	m_filetime = IO::File::GetLastModifiedTime(fp);
		if (m_size)
		{
			m_data = new char[m_size];
		//	m_data = (char*)myalloc(m_size, __FILE__, __LINE__);
			fp.Read(m_data, m_size);
		}
		fp.Close();//std::fclose(fp);
		return true;
	}
	_catch (Exception, e)
	{
		return false;//fp;
	}
}

int CFile::_getc2()
{
	int c;
	if (true)
	{
		if (m_fpos < m_size)
		{
			c = m_data[m_fpos];
			++m_fpos;
			++m_column;
			m_lastc = c;
		}
		else
			return EOF;
	}
	/*
	else
	{
		c = fgetc(m_fp);
		m_lastc = c;
	}
	*/

	if (c == '\n')
	{
		m_column = 0;
		++m_line;
	}
	return c;
}

int CFile::_getc()
{
	int c = _getc2();

	// Each instance of a new-line character and an immediately preceding backslash character is deleted,
	// splicing physical source lines to form logical source lines
	while (c == '\\')
	{
		int c2 = _getc2();
		if (c2 == '\r')
		{
			int c3 = _getc2();
			if (c3 == '\n')
			{
				c = _getc2();	// character on the next line
			}
		}
		else if (c2 == '\n')
		{
			c = _getc2();	// character on the next line
		}
		else
		{
			_ungetc(c2);
		//	c = c2;
			return c;
		}
	}
	return c;
}

void CFile::_ungetc(int c)
{
//	ASSERT(c == m_lastc);
	if (c == '\n')
	{
		m_column = 9999;	// TODO
		--m_line;
	}

	if (true)
	{
		--m_column;
		--m_fpos;
		ASSERT(m_fpos >= 0);
	}
	/*
	else
	{
		ungetc(c, m_fp);
	}
	*/
}

CPreprocessor::CPreprocessor(CFile* pFile, cpp::Options* options) :
	m_pFile(pFile),
	m_options(options)
{
	m_pIf = NULL;

	m_pMacroInvocation = NULL;

	curtokpp = NULL;

	m_errors = 0;
	m_warnings = 0;

	m_try = 0;

	m_bNewLine = true;

//	m_pCompiler = NULL;

	// Default
//	m_msgStream.set_BaseStream(IO::NullStreamObject::_out);
}

void CPreprocessor::Define(CDefine* pDefine)
{
	/*
	if (pDefine->m_name == "LFCEXT")
	{
		MessageBeep(-1);
	}
	*/

	pDefine->m_location.m_filepath = m_pFile->m_filepath;
	pDefine->m_location.m_line = m_pFile->m_line;
//	m_defines.push_back(pDefine);
	m_defines.insert(macromap::value_type(pDefine->m_name, pDefine));
//	m_std_defines.insert(std_macromap::value_type(pDefine->m_name, pDefine));
}

CDefine* CPreprocessor::Define(StringIn name)
{
	/*
	if (!strcmp(name, "__RPC_USER"))
	{
		printf("__RPC_USER\n");
	}
	*/
	macromap::iterator pDefine2 = Find(name);
	if (pDefine2 != m_defines.end())
	{
		/*
		char msg[512];
		sprintf_s(msg, "Redefinition of macro '%s' : first seen :\n%s(%d)",
			name, (*pDefine2).second->m_location.m_filepath->c_str(), (*pDefine2).second->m_location.m_line);

		jerror(msg);
		*/

		jerror(serror() << "Redefinition of macro '" << name << "' : first seen :\n" << 
			(*pDefine2).second->m_location);

	//	pDefine->m_replacementList.clear();

		return NULL;
	}
	else
	{
		CDefine* pDefine;
		pDefine = new CDefine;
		pDefine->m_name = name;

		pDefine->m_location.m_filepath = m_pFile->m_filepath;
		pDefine->m_location.m_line = m_pFile->m_line;
	//	m_defines.push_back(pDefine);
#if 0
		m_defines.insert(macromap::value_type(pDefine->m_name, pDefine));
#endif
		m_defines.insert(std_macromap::value_type(pDefine->m_name, pDefine));

		return pDefine;
	}
}

CDefine* CPreprocessor::Define(StringIn name, long int32)
{
	macromap::iterator pDefine2 = Find(name);
	if (pDefine2 != m_defines.end())
	{
		jerror(serror() << "Redefinition of macro '" << name << "' : first seen :\n" <<
			(*pDefine2).second->m_location);

		return NULL;
	}
	else
	{
		CDefine* pDefine;
		pDefine = new CDefine;
		pDefine->m_name = name;

		CToken* tok = new IntegerToken;
	//	tok->type = CToken::tok_integer;
		tok->intval = int32;

		pDefine->m_replacementList.push_back(tok);

		pDefine->m_location.m_filepath = m_pFile->m_filepath;
		pDefine->m_location.m_line = m_pFile->m_line;
	//	m_defines.push_back(pDefine);
		m_defines.insert(macromap::value_type(pDefine->m_name, pDefine));

	//	m_defines.insert(macromap::value_type(pDefine->m_name, pDefine));

		return pDefine;
	}
}

void CPreprocessor::Undef(StringIn name)
{
	if (m_defines.find(name) != m_defines.end())
	{
	//	ASSERT(m_defines.find(name) != m_std_defines.end());

		{
			/*
			std_macromap::iterator it = m_std_defines.begin();
			while (it != m_std_defines.end())
			{
				DebugTraceLn((*it).first);
				++it;
			}
			*/
		}
	/*
		{
			macromap::iterator it = m_defines.begin();
			while (it != m_defines.end())
			{
				TRACE("%s\n", (*it).first->c_str());
				++it;
			}
		}
		*/

		m_defines.erase(name);

	//	m_defines.erase(name);

	//	MessageBeep(-1);
	}
	else
	{
#if 0
		ASSERT(m_std_defines.find(name) == m_std_defines.end());
#endif
	}

	/*
	for (int i = 0; i < m_defines.size(); i++)
	{
		if (!strcmp(m_defines[i]->m_name.c_str(), name))
		{
			m_defines.erase(&m_defines[i]);
		}
	}
	*/
}

bool CPreprocessor::IfDef(StringIn name)
{
	if (m_defines.find(name) != m_defines.end())
	{
		//ASSERT(m_std_defines.find(name) != m_std_defines.end());

		return true;
	}
	else
	{
	//	ASSERT(m_std_defines.find(name) == m_std_defines.end());

		return false;
	}
}

// Argument substitution
int MacroInvocation::SubstituteArguments(vector<vector<CToken*> >& argTokenList)
{
	IO::StringWriter strbuilder;

	//CToken* pprevtok = NULL;

	for (size_t i = 0; i < m_pMacro->m_replacementList.GetSize(); ++i)
	{
		const CToken* ptok = m_pMacro->m_replacementList[i];

		const CToken* pnexttok;
		if (i < m_pMacro->m_replacementList.size()-1)
			pnexttok = m_pMacro->m_replacementList[i+1];
		else
			pnexttok = NULL;

		if (ptok->GetKind() == CToken::tok_symbol && ptok->symbol == '##')
		{
		//	m_string.erase(m_string.end()-1);
			if (i == 0)
				return -1;
			else if (pnexttok == NULL)
				return -2;

			int arg = GetArg(pnexttok->m_astr);
			if (arg >= 0)
			{
				if (argTokenList.GetSize() > arg)
				{
					vector<CToken*>& tokenlist = argTokenList[arg];
					for (int j = 0; j < tokenlist.GetSize(); j++)
					{
						CToken* ptok = tokenlist[j];
					//	str = *str + *ptok->toString();
						ptok->Write(strbuilder);
					}
				}
			}
			else
			{
			//	str = pnexttok->toString();
				pnexttok->Write(strbuilder);
			}

			//str += pnexttok->toString();

#if 0
			m_string = *m_string + *str;
#endif
			i++;
			continue;
		}

		if (ptok->GetKind() == CToken::tok_ident)
		{
			int arg = GetArg(ptok->m_astr);
			if (arg >= 0)
			{
				if (argTokenList.GetSize() > arg)
				{
					vector<CToken*>& tokenlist = argTokenList[arg];
					for (int j = 0; j < tokenlist.GetSize(); j++)
					{
						const CToken* ptok = tokenlist[j];
					//	m_string = *m_string + *ptok->toString();
						ptok->Write(strbuilder);
					}
				}
			}
			else
			{
			//	m_string = *m_string + *ptok->toString();
				ptok->Write(strbuilder);
			}
		}
		else if (ptok->GetKind() == CToken::tok_symbol && ptok->symbol == '#')
		{
			return 1;
			i++;
		}
		else
		{
			//m_string = *m_string + *ptok->toString();
			ptok->Write(strbuilder);
		}

	//	pprevtok = ptok;
	}

	m_string = strbuilder.str();

	return 0;	// success
}

#if 0
void CPreprocessor::skipspaces()
{
	while (!m_pFile->_eof())
	{
		int c = curtokpp->symbol;//m_pFile->_getc();
		if (!(curtokpp->GetKind() == CToken::tok_char && (c == ' ' || c == '\t')))
		{
			//m_pFile->_ungetc(c);
			break;
		}
		GetToken4();
	}

#if 0
// newlines are retained
	while (!m_pFile->_eof())
	{
		int c = m_pFile->_getc();
		if (!(c == ' ' || c == '\t'))
		{
			m_pFile->_ungetc(c);
			break;
		}
	}
#endif
}
#endif

int CPreprocessor::_getc()
{
	if (m_pMacroInvocation)
	{

		// Keep backing out of macro 
		while (m_pMacroInvocation->_eof())
		{

			MacroInvocation* pPrev = m_pMacroInvocation->m_pPrev;
			m_pMacroInvocation = pPrev;

			if (m_pMacroInvocation == NULL)
			{
				break;
			}
		}
	}

	if (m_pMacroInvocation)
	{
		return m_pMacroInvocation->_getc();
	}
	else
	{
		return m_pFile->_getc();
	}
}

void CPreprocessor::_ungetc(int c)
{
	if (m_pMacroInvocation)
	{
		m_pMacroInvocation->_ungetc();
	}
	else
	{
		m_pFile->_ungetc(c);
	}
}

bool CPreprocessor::_eof()
{
	if (m_pMacroInvocation)
	{
		if (!m_pMacroInvocation->_eof())
			return false;
	}

	return m_pFile->_eof();
}

int CPreprocessor::getNextNonSpace(int c)
{
// skip all spaces and comments

	while (/*c == ' ' || c == '\t' || c == '\v' || c == '\f' ||*/ c == '/')
	{
	//	int c = _getc();

		/*
		if (!(c == ' ' || c == '\t' || c == '\v' || c == '\f' || c == '/'))
		{
			break;
			//_ungetc();
		}
		*/

		if (c == '/')	// could be beginning of comment
		{
			int c2 = _getc();
			if (c2 == '*')	// multi-line comment
			{
				bool okay = false;
				while (!_eof())
				{
					c2 = _getc();
					if (c2 == '*')
					{
						int c3 = _getc();
						if (c3 == '/')
						{
							okay = true;
							break;
						}
						else
							_ungetc(c3);
					}
				}

				if (!okay)
				{
					jfatalerror("Unexpected end of file, unterminated comment");
				}

				c = _getc();
			}
			else if (c2 == '/')	// single-line comment
			{
				while (!_eof())
				{
					c2 = _getc();
				//	printf("%c", c2);
					if (c2 == '\r')
					{
						int c3 = _getc();
						ASSERT(c3 == '\n');
						_ungetc(c3);
						break;
					}
					else if (c2 == '\n')
					{
						_ungetc(c2);
						break;
					}
				}
				c = _getc();
				ASSERT(c == '\n' || c == -1);
			}
			else
			{
				_ungetc(c2);
				break;
			}
		}
		else	// white-space
		{
			c = _getc();
		}
	}

	return c;
}

CToken* CPreprocessor::GetToken4()
{

#if 0
	if (m_pMacroInvocation)
	{
		if (!m_pMacroInvocation->m_tokens.empty())
		{
			*curtokpp = m_pMacroInvocation->m_tokens.top();
			m_pMacroInvocation->m_tokens.pop();
			return curtokpp;
		}
	}
	else
	{
		if (!m_pFile->m_tokens.empty())
		{
			*curtokpp = m_pFile->m_tokens.top();
			m_pFile->m_tokens.pop();
			return curtokpp;
		}
	}
#endif

	{
		int c = _getc();
	//	printf("%c", c);

		CToken* token = NULL;

		while (c == EOF)
		{
			if (m_pFile->m_pPrev == NULL)
			{
				break;
			}

			CFile* pPrev = m_pFile->m_pPrev;
			delete m_pFile;
			m_pFile = pPrev;

			c = m_pFile->_getc();
		}

#if 0
	// trigraph sequences
		if (c == '?')
		{
			int c2 = _getc();
			if (c2 == '?')
			{
				int c3 = _getc();
				switch (c3)
				{
				case '=':	c = '#';		break;
				case '/':	c = '\\';	break;
				case '\'':	c = '^';		break;
				case '(':	c = '[';		break;
				case ')':	c = ']';		break;
				case '!':	c = '|';		break;
				case '<':	c = '{';		break;
				case '>':	c = '}';		break;
				case '-':	c = '~';		break;
				default:
					jfatalerror("Unrecognized trigraph sequence");
				}
			}
			else
			{
				_ungetc(c2);
			}
		}
#endif

		c = getNextNonSpace(c);

		if (!((unsigned)(c + 1) <= 256))
		{
			// TODO
			c = 'a';
			//jfatalerror("invalid character");
		}

		if (c == ' ' || c == '\t' || c == '\v' || c == '\f')
		{
			token = new SpaceToken;
		//	token->type = CToken::tok_space;

			while  (!_eof())
			{
				c = _getc();
				if (!(c == ' ' || c == '\t' || c == '\v' || c == '\f'))
				{
					_ungetc(c);
					break;
				}
			}
		}
		else if (isalpha(c) || c == '_')
		{
			IO::StringWriter ident;
			ident << (char)c;
			while (!_eof())
			{
				c = _getc();
				if (!(isalnum(c) || c == '_'))
				{
					_ungetc(c);
					break;
				}

				ident << (char)c;
			}

			String str = ident.str();
			token = new IdentToken;
		//	token->type = CToken::tok_ident;
			token->m_astr = dynamic_cast<StringA*>(str.m_stringObject);
			if (token->m_astr == NULL)
			{
				ASSERT(0);
				IO::StringWriter str;
				str << __FILE__ << paren(__LINE__ + 1) << " : INTERNAL ERROR";
				raise(Exception(str.str()));
			}
			token->m_astr->IncRef();

		//	token->ident.len = ident->Length();
		//	token->ident.ident = strdup(ident->c_str());
		}
		else if (c >= '0' && c <= '9')
		{
			unsigned __int64 intval;
			int c2 = -1;
			if (c == '0')
			{
				c2 = _getc();
				if (c2 == 'x')	// hex number
				{
					intval = 0;

					while (!_eof())
					{
						c = _getc();
						if (!((c >= '0' && c <= '9') ||
							(c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')))
						{
							_ungetc(c);
							break;
						}

						intval *= 16;

						if (c <= '9')
						{
							intval += c - '0';
						}
						else if (c <= 'F')
						{
							intval += 10 + c - 'A';
						}
						else
						{
							intval += 10 + c - 'a';
						}
					}

					token = new IntegerToken;
				//	token->type = CToken::tok_integer;
					token->intval = intval;
				}
				else
				{
					_ungetc(c2);
				}
			}

			if (c2 != 'x')
			{
				intval = c - '0';
				while (!_eof())
				{
					c = _getc();
					if (!(c >= '0' && c <= '9'))
					{
					//	_ungetc(c);
						break;
					}

					intval *= 10;
					intval += c - '0';
				}

			// get fraction
				int exp = 0;
				double fraction;

				//int c = _getc();
				if (c == '.')
				{
					//p++;
					fraction = getfrac(this);

				// get exp
					exp = lookexp(this);

					int c = _getc();
					if (c == 'f' || c == 'F' || c == 'l' || c == 'L')
					{
					}
					else
					{
						_ungetc(c);
					}

					double double64 = makeflt(intval, fraction, exp);

					token = new DoubleToken;
				//	token->type = CToken::tok_double;
					token->double64 = double64;
				}
				else
				{
					fraction = 0;
					_ungetc(c);

					{
						int c = _getc();
						if (c == 'l' || c == 'L')
						{
						}
						else
						{
							_ungetc(c);
						}
					}

					token = new IntegerToken;
				//	token->type = CToken::tok_integer;
					token->intval = intval;
				}
			}
			
		}
		else if (c == '(' || c == ')')
		{
			token = new SymbolToken;
		//	token->type = CToken::tok_symbol;
			token->symbol = c;
		}
		else if (c == '!')
		{
			token = new SymbolToken;

			int c2 = _getc();
			if (c2 == '=')	// !=
			{
			//	token->type = CToken::tok_symbol;
				token->symbol = (c<<8) | c2;
			}
			else
			{
				_ungetc(c2);
			//	token->type = CToken::tok_symbol;
				token->symbol = c;
			}
		}
		else if (c == '<' || c == '>')
		{
			int c2 = _getc();

			token = new SymbolToken;

			if (c2 == c)	// << , >>
			{
			//	token->type = CToken::tok_symbol;
				token->symbol = (c<<8) | c2;

				c2 = _getc();
				if (c2 == '=')
				{
					token->symbol <<= 8;
					token->symbol |= c2;

					ASSERT(token->symbol == '>>=' || token->symbol == '<<=');
				}
				else
				{
					_ungetc(c2);
				}
			}
			else if (c2 == '=')	// <= , >=
			{
			//	token->type = CToken::tok_symbol;
				token->symbol = (c<<8) | c2;
			}
			else
			{
				_ungetc(c2);
			//	token->type = CToken::tok_symbol;
				token->symbol = c;
			}
		}
		else if (c == '+' ||
					c == '-' ||
					c == '=' ||
					c == '&' ||
					c == '|' ||
					c == '^' ||
					c == '*' ||
					c == '/' ||
					c == '%' ||
					c == '.' ||
					c == ':' ||
					c == '#')
		{
			token = new SymbolToken;

			int c2 = _getc();

			if (c == '-' && c2 == '>')	// ->
			{
			//	token->type = CToken::tok_symbol;
				token->symbol = (c<<8) | c2;
				c2 = _getc();
				if (c2 == '*')	// ->*
				{
					token->symbol = '->*';
				}
				else
				{
					_ungetc(c2);
				}
			}
			else if (c == '.' && c2 == '.')
			{
				c2 = _getc();
				if (c2 == '.')
				{
				//	token->type = CToken::tok_symbol;
					token->symbol = '...';
				}
				else
				{
					_ungetc(c2);
					_ungetc(c);

				//	token->type = CToken::tok_symbol;
					token->symbol = '.';
				}
			}
			else if (c == '.' && c2 == '*')
			{
			//	token->type = CToken::tok_symbol;
				token->symbol = (c<<8) | c2;
			}
			else if ((c == '=' || c == '+' || c == '-' || c == '&' || c == '|' || c == ':' || c == '#') && (c2 == c))	// ++ , -- , ==, '&&', '||', '::'
			{
			//	token->type = CToken::tok_symbol;
				token->symbol = (c<<8) | c2;
			}
			else if ((c == '+' || c == '-' || c == '&' || c == '|' || c == '^' || c == '*' || c == '/' || c == '%')
						&& c2 == '=')
			{
			//	token->type = CToken::tok_symbol;
				token->symbol = (c<<8) | c2;
			}
			else
			{
				_ungetc(c2);
			//	token->type = CToken::tok_symbol;
				token->symbol = c;
			}
		}
		else if (c == '\r')
		{
			int c2 = _getc();
			ASSERT(c2 ==  '\n');

			token = new SymbolToken;

		//	token->type = CToken::tok_symbol;
			token->symbol = c2;
		}
		else if (c == '\n')
		{
			token = new SymbolToken;
		//	token->type = CToken::tok_symbol;
			token->symbol = c;
		}
		else
		{
			token = new CharToken;
		//	token->type = CToken::tok_char;
			token->symbol = c;
		}

		curtokpp = token;
	}

	return curtokpp;
}

// Get next token, without preprocessing or macro expansion
CToken* CPreprocessor::GetToken3()
{
	GetToken4();
	while (curtokpp->GetKind() == CToken::tok_space)
	{
		GetToken4();
	}

	return curtokpp;
}

// Get next token with preprocessing
CToken* CPreprocessor::GetToken2()
{
	GetToken3();

// If we've just gotten on a new line, check for preprocessor directive
//	if (m_bNewLine)
	{
		while (curtokpp->GetKind() == CToken::tok_symbol && curtokpp->symbol == '#')
		{
			CToken* hashtok = curtokpp;

			GetToken3();

			if (curtokpp->GetKind() == CToken::tok_ident && String(curtokpp->m_astr) == "pragma")
			{
				//GetToken3();

				//m_stack.push(curtokpp);
				//m_stack.push(hashtok);
				//curtokpp = hashtok;

				IO::StringWriter stream;

				while (!m_pFile->_eof())
				{
					/*
					if (curtokpp->GetKind() == CToken::tok_ident && *curtokpp->m_astr == "once")
					{
						MessageBeep(-1);
					}
					*/

					if (curtokpp->GetKind() == CToken::tok_symbol && curtokpp->symbol == '\n')
					{
						GetToken3();
						break;
					}
					GetToken3();
					stream << curtokpp->toString();
				}

				{
					StringToken* tok = new StringToken;
					tok->m_astr = dynamic_cast<StringA*>(stream.str().m_stringObject);
					tok->m_astr->IncRef();

					m_stack.push(tok);

				ASSERT(m_bNewLine);

				m_bNewLine = true;

				}


				{
					SymbolToken* tok = new SymbolToken;
					tok->symbol = '@';
				//	m_stack.push(tok);

					curtokpp = tok;
					return curtokpp;
				}

				break;
			}
			else if (curtokpp->GetKind() == CToken::tok_ident && String(curtokpp->m_astr) == "once")
			{
				ASSERT(0);
			}
			else
			{
				/*
				if (strstr(m_pFile->m_filepath->c_str(), "Control.h"))
				{
					MessageBeep(-1);
				}
				*/
				directive();
			}
		}

		m_bNewLine = false;
	}

//	ASSERT(!(curtokpp->GetKind() == CToken::tok_symbol && curtokpp->symbol == '#'));

	if (curtokpp->GetKind() == CToken::tok_symbol && curtokpp->symbol == '\n')
	{
		m_bNewLine = true;
	//	return GetToken();
	}

	return curtokpp;
}

#if 0
void CFile::UngetToken(CToken* ptok)
{
	m_tokens.push(*ptok);
}
#endif

CToken* CPreprocessor::TryMacro(MacroInvocation* pParentMacro, macromap::iterator pDefine)
{
//	long pos = ftell(m_pFile->m_fp);

	CToken* mname = curtokpp;

	bool bMacro = false;

	if ((*pDefine).second->m_bFunctionLike)
	{
		_GetToken();
		//int c = _getc();
		if (curtokpp->GetKind() == CToken::tok_symbol && curtokpp->symbol == '(')
		//if (c == '(')//curtokpp->GetKind() == CToken::tok_symbol && curtokpp->symbol == '(')
		{
			bMacro = true;
		}
		else
		{
			m_stack.push(curtokpp);
			curtokpp = mname;
//			printf("");
			return curtokpp;
			//ASSERT(0);
			//_ungetc(c);
		}
	}
	else
	{
		bMacro = true;
	}

	if (bMacro)
	{
		/*
		if (m_pMacroInvocation)
			printindent(m_pMacroInvocation);
		printf("(%s\n", mname.ident);
		*/

		vector<vector<CToken*> > argTokenList;

		if ((*pDefine).second->m_bFunctionLike)
		{
			/*
			if (!strcmp(mname.ident, "__declspec"))
			{
				printf("");
			}
			*/
			int argNamesSize = (*pDefine).second->m_argNames.size();

			/*
			if (argNamesSize == 12)
			{
				printf("");
			}
			*/

#if 0
			GetToken3();	// '('
#endif
			GetToken3();
			int paren = 1;

			if (curtokpp->symbol != ')')
			{
				while (!m_pFile->_eof())
				{
					ASSERT(paren == 1);

					vector<CToken*> tokens;

					while (!m_pFile->_eof())
					{
						if (curtokpp->GetKind() == CToken::tok_symbol && curtokpp->symbol == '(')
						{
							paren++;
						}
						else if (curtokpp->GetKind() == CToken::tok_symbol && curtokpp->symbol == ')')
						{
							paren--;
							if (paren < 1)
							{
								//GetToken4();
								break;
							}
						}
						else if (curtokpp->GetKind() == CToken::tok_char && curtokpp->symbol == ',')
						{
							if (paren == 1)
								break;
						}

						tokens.push_back(curtokpp);
						GetToken4();//GetToken3();
					}
					if (curtokpp->symbol == ',')
						GetToken3();

					argTokenList.push_back(tokens);

					if (argTokenList.GetSize() > argNamesSize+10)	// Break at some sensible overflow
					{
						break;
					}

					if (paren == 0)
						break;
				}
			}

			int argTokenListSize = argTokenList.GetSize();

			if (argTokenListSize < argNamesSize)
			{
				char msg[512];
				sprintf_s(msg, "too few parameters in macro invocation of '%s'", CString(mname->m_astr).c_str());
				jerror(msg);
			}
			else if (argTokenListSize > argNamesSize)
			{
				char msg[512];
				sprintf_s(msg, "too many parameters in macro invocation of '%s'", CString(mname->m_astr).c_str());
				jerror(msg);
			}

			Expect(')');
		//	GetToken3();
		}

		MacroInvocation* pMacroInvocation = new MacroInvocation((*pDefine).second);

		int err = pMacroInvocation->SubstituteArguments(argTokenList);
		if (err != 0)
		{
			if (err == -1)
				jfatalerror("## cannot be the first token in a macro");
			else if (err == -2)
				jfatalerror("## cannot be the last token in a macro");
			else
				jfatalerror("failed to substitute macro arguments");
		}

#if 0
		if (m_pMacroInvocation == NULL)
		{
		//	printf("pushing->'%s'\n", curtokpp->ident);
			m_pFile->UngetToken(curtokpp);
		}
		else
		{
			m_pMacroInvocation->UngetToken(curtokpp);
		}
#endif

		pMacroInvocation->m_pPrev = pParentMacro;//m_pMacroInvocation;
		m_pMacroInvocation = pMacroInvocation;

		return GetToken();	// Descend into macro
	}
	else
	{
	//	printf("pushing->'%s'\n", curtokpp->ident);
#if 0
		m_pFile->UngetToken(curtokpp);
#endif
		curtokpp = mname;
	}

	return curtokpp;
}

// Get next token with preprocessing and macro expansion
CToken* CPreprocessor::GetToken()
{
	MacroInvocation* pParentMacro = m_pMacroInvocation;

	curtokpp = GetToken2();

	if (curtokpp->GetKind() == CToken::tok_ident)
	{
		if (m_pMacroInvocation == NULL || !m_pMacroInvocation->IsInside(curtokpp->m_astr))
		{
			macromap::iterator pDefine = m_defines.find(curtokpp->m_astr);
			if (pDefine != m_defines.end())
			{
				/*
				if (String(curtokpp->m_astr) == "LFCEXT")
				{
					MessageBeep(-1);
				}
				*/
				
				curtokpp = TryMacro(pParentMacro/*m_pMacroInvocation*/, pDefine);
			}
		}
	}

	return curtokpp;
}

void CPreprocessor::GetString(bool bWide/*, CToken& token*/)
{
	int delimiter = '"';

	int line = m_pFile->m_line;

//	StringBuilder strbuilder(bWide? new MutableString<chAR>(L"", 0): new MutableString<char>("", 0));
	IO::StringWriter strbuilder(new MutableString<char>("", 0));

	int c2;
	while (!_eof())
	{
		//PP::CToken* ptok = pPP->GetToken4();
		c2 = _getc();

		if (c2 == '\\')
		{
			int c3 = _getc();
			if (c3 == '\\')
			{
				c2 = '\\';
			}
			else if (c3 == 'n')
			{
				c2 = '\n';
			}
			else if (c3 == 'r')
			{
				c2 = '\r';
			}
			else if (c3 == 't')
			{
				c2 = '\t';
			}
			else if (c3 == '?')
			{
				c2 = '\?';
			}
			else if (c3 == '\'')
			{
				c2 = '\'';
			}
			else if (c3 == '\"')
			{
				c2 = '\"';
			}
			else if (c3 == '0')
			{
				c2 = '\0';
			}
			else
			{
				jwarn(swarn() << "Unrecognized string sequence");
			}
		}
		else if (c2 == delimiter)
		{
		//	ungetc(c2, fpin);
			break;
		}
		strbuilder << (char)c2;
	}

	if (c2 != delimiter)
	{
		char msg[256];
		sprintf_s(msg, "expected end of string, started on line(%d)", line);
		printf(msg);
	}

/*
	if (bWide)
	{
		token.type = CToken::tok_wstring;
	}
	else
	{
		token.type = CToken::tok_string;
	}
	*/

//	token.stringlen = str.length();
	curtokpp = new StringToken;
	//curtokpp->GetKind() = CToken::tok_string_literal;
	curtokpp->m_astr = dynamic_cast<StringA*>(strbuilder.str().m_stringObject);
	curtokpp->m_astr->IncRef();

//	curtokpp->string.string = strdup(str->c_str());
//	curtokpp->string.len = str->Length();

//	*curtok = token;
}

void CPreprocessor::GetCharacter(bool bWide/*, CToken& token*/)
{
	int delimiter = '\'';

	int line = m_pFile->m_line;

	uint32 value = 0;

	int c2;
	while (!_eof())
	{
		//PP::CToken* ptok = pPP->GetToken4();
		c2 = _getc();

		if (c2 == '\\')
		{
			int c3 = _getc();
			if (c3 == '\\')
			{
				c2 = '\\';
			}
			else if (c3 == 'n')
			{
				c2 = '\n';
			}
			else if (c3 == 'r')
			{
				c2 = '\r';
			}
			else if (c3 == 't')
			{
				c2 = '\t';
			}
			else if (c3 == '?')
			{
				c2 = '\?';
			}
			else if (c3 == '\'')
			{
				c2 = '\'';
			}
			else if (c3 == '\"')
			{
				c2 = '\"';
			}
			else if (c3 == '0')
			{
				c2 = '\0';
			}
			else
			{
				jwarn("Unrecognized string sequence\n");
			}
		}
		else if (c2 == delimiter)
		{
		//	ungetc(c2, fpin);
			break;
		}
	//	strbuilder << (char)c2;
		value <<= 8;
		value |= c2;
	}

	if (c2 != delimiter)
	{
		char msg[256];
		sprintf_s(msg, "expected end of character literal, started on line(%d)", line);
		printf(msg);
	}

/*
	if (bWide)
	{
		token.type = CToken::tok_wstring;
	}
	else
	{
		token.type = CToken::tok_string;
	}
	*/

	curtokpp = new CharLiteralToken;
	//curtokpp->GetKind() = CToken::tok_char_literal;
	curtokpp->intval = value;
}

CToken* CPreprocessor::__GetToken()
{
	GetToken();

	while (curtokpp->GetKind() == CToken::tok_space ||
		(curtokpp->GetKind() == CToken::tok_symbol && curtokpp->symbol == '\n'))
	{
		//yylloc.first_line = pPP2->m_pFile->m_line;
		GetToken();
	}

	if (curtokpp->GetKind() == CToken::tok_ident && String(curtokpp->m_astr) == "L")
	{
		m_stack.push(curtokpp);
		GetToken();

		if (	curtokpp->GetKind() == CToken::tok_char ||
				curtokpp->GetKind() == CToken::tok_symbol)
		{
			int c = curtokpp->symbol;

			if (c == '\'')
			{
				m_stack.pop();
				GetCharacter(true);
			}
			else if (c == '\"')
			{
				m_stack.pop();
				GetString(true);
			}
			else
			{
				m_stack.pop();
				curtokpp = m_stack.top();
			}
		}
		else
		{
			m_stack.pop();
			curtokpp = m_stack.top();
		}
	}
	else

	if (	curtokpp->GetKind() == CToken::tok_char ||
			curtokpp->GetKind() == CToken::tok_symbol)
	{
		int c = curtokpp->symbol;

		if (c == '\'')
		{
			GetCharacter(false /*, token*/);
		}
		else if (c == '\"')
		{
			GetString(false /*, token*/);
		}
	}

	return curtokpp;
}

CToken* CPreprocessor::_GetToken()
{
	if (!m_stack.empty())
	{
		curtokpp = m_stack.top();
		m_stack.pop();
		return curtokpp;
	}

	__GetToken();

	if (curtokpp->GetKind() == CToken::tok_string_literal)
	{
		//char* str = 
		while (1)
		{
			CToken* prev = curtokpp;
			__GetToken();
			if (curtokpp->GetKind() == CToken::tok_string_literal)
			{
				ASSERT(0);
#if 0
				char* str = (char*)malloc(prev->string.len + curtokpp->string.len);
#endif
				// TODO
				//memcpy(str, curtokpp->string.string
			}
			else
			{
				m_stack.push(curtokpp);
				curtokpp = prev;
				break;
			}
		}
	}

	return curtokpp;
}

void CPreprocessor::GetHeaderName()
{
	//while (!m_pFile->_eof())
	{
		int c = curtokpp->symbol;

	// Special handling
		if (c == '"' || c == '<')
		{
			GetToken4();	// No special handling while in string

			//CToken token;

		//	__live_object_ptr<StringA> str = new StringA;
		//	char buffer[512];
		//	BufferImp<char, grow_allocator> strbuffer(grow_allocator(buffer, 512));
			IO::StringWriter strbuilder;
			//strbuilder.reserve(512);

			int line = m_pFile->m_line;
			int eos = (c == '"')? '"': '>';	// end of string

			int c2;
			while (!m_pFile->_eof())
			{
				if (curtokpp->GetKind() == CToken::tok_char || curtokpp->GetKind() == CToken::tok_symbol)
				{
					c2 = curtokpp->symbol;//m_pFile->_getc();
					//c2 = m_pFile->_getc();
					if (c2 == eos)
					{
					//	ungetc(c2, fpin);
						break;
					}

					strbuilder << (char)c2;
				}
				else if (curtokpp->GetKind() == CToken::tok_ident)
				{
					strbuilder << String(curtokpp->m_astr);
				}

				GetToken4();	// No special handling while in string
			}

			if (c2 != eos)
			{
				char msg[256];
				sprintf_s(msg, "Expected end of string '%c', started(%d)", eos, line);
				jfatalerror(msg);
			}

			CToken* token;
			if (c == '"')
			{
				token = new StringToken;
			//	token->type = CToken::tok_string_literal;
			}
			else
			{
				token = new LowerThanStringToken;
			//	token->type = CToken::tok_ltstring;
			}

			token->m_astr = dynamic_cast<StringObjectT<char>*>(strbuilder.str().m_stringObject);
			//token->string.string = strdup(str->c_str());

			curtokpp = token;

			//GetToken2();

		//	return str;
		}
		else
			jfatalerror(sfatalerror() << "Expected string");
	}
	//return NULL;
}

void CPreprocessor::Expect(int c)
{
	if (
		(curtokpp->GetKind() != CToken::tok_symbol && curtokpp->GetKind() != CToken::tok_char)
		|| (curtokpp->symbol != c))
	{
		char cur[512];

		if (curtokpp->GetKind() == CToken::tok_ident)
		{
			sprintf_s(cur, "'%s'", CString(curtokpp->m_astr));
		}
		else
		{
			sprintf_s(cur, "'%c'", curtokpp->symbol);
		}

		char msg[256];
		if (c == '\n')
			sprintf_s(msg, "%s expected newline", cur, c);
		else
			sprintf_s(msg, "%s expected '%c'", cur, c);
		jerror(msg);
	}
}

void CPreprocessor::EatSymbol(int c)
{
	Expect(c);
	GetToken3();
}

////////////////////////////////////////////////////////////////////////

void CPreprocessor::newline()
{
	EatSymbol('\n');
}

void CPreprocessor::newline_or_eof()
{
	if (!m_pFile->_eof())
	{
		EatSymbol('\n');
	}
}

void CPreprocessor::skipline()
{
	while (!m_pFile->_eof())
	{
		if (curtokpp->GetKind() == CToken::tok_symbol && curtokpp->symbol == '\n')
		{
			GetToken3();
			break;
		}
		GetToken3();
	}
}

StringA* CPreprocessor::identifier()
{
	if (curtokpp->GetKind() != CToken::tok_ident)
	{
		jfatalerror("Expected identifier");
	}
	StringA* ident = curtokpp->m_astr;
	GetToken2();

	return ident;
}

void CPreprocessor::identifier_list(CDefine* pMacro)
{
	while (!m_pFile->_eof())
	{
		StringA* ident = identifier();
		pMacro->m_argNames.push_back(ident);

		if (curtokpp->GetKind() == CToken::tok_char && curtokpp->symbol == ',')
		{
			GetToken2();
			continue;
		}
		break;
	}
}

long CPreprocessor::literal_expression()
{
	if (curtokpp->GetKind() == CToken::tok_integer)
	{
		long int32 = curtokpp->intval;
		GetToken2();
		return int32;
	}
	else
	{
		jfatalerror("invalid integer constant expression");
		return 0;
	}
}

/*
primary-expression:
	literal
	this
	( expression )
	id-expression
*/
long CPreprocessor::primary_expression()
{
	if (curtokpp->GetKind() == CToken::tok_symbol && curtokpp->symbol == '(')
	{
		GetToken2();

		long pExpr = constant_expression();
		EatSymbol(')');

		return pExpr;
	}
	else if (curtokpp->GetKind() == CToken::tok_ident)
	{
		/*
		CExpression* pexpr = new CExpression;
		pexpr->tok = *curtok;
		GetToken();

		return pexpr;
		*/

		StringA* ident = curtokpp->m_astr;

		/*
		if (String(ident) == "LFCEXT")
		{
			MessageBeep(-1);
		}
		*/

//	GetToken2();

		macromap::iterator macro = m_defines.find(ident);
		if (macro != m_defines.end())
		{
			if (macro->second->m_replacementList.size())
			{
				TryMacro(m_pMacroInvocation, macro);

			/*
			m_pFile->UngetToken(curtokpp);
			GetToken();
			*/
				return primary_expression();
			}
			else
			{
				GetToken2();
				return 1;
			}
		}
		else
			GetToken2();

		return 0;
		//return IfDef(ident.c_str());
	}
	else
	{
		return literal_expression();
	}
}

/*
postfix-expression:
	primary-expression
	postfix-expression [ expression ]
	postfix-expression ( expression-listopt )
	simple-type-specifier ( expression-listopt )
	typename ::opt nested-name-specifier identifier ( expression-listopt )
	typename ::opt nested-name-specifier templateopt template-id ( expression-listopt )
	postfix-expression . templateopt id-expression
	postfix-expression -> templateopt id-expression
	postfix-expression . pseudo-destructor-name
	postfix-expression -> pseudo-destructor-name
	postfix-expression ++
	postfix-expression --
	dynamic_cast < type-id > ( expression )
	static_cast < type-id > ( expression )
	reinterpret_cast < type-id > ( expression )
	const_cast < type-id > ( expression )
	typeid ( expression )
	typeid ( type-id )
*/

long CPreprocessor::postfix_expression()
{
	long pExpr = primary_expression();

#if 0
	if (curtok->type == CToken::tok_keyword &&
		(curtok->pkw->n == KW_DYNAMIC_CAST ||
		curtok->pkw->n == KW_STATIC_CAST ||
		curtok->pkw->n == KW_REINTERPRET_CAST ||
		curtok->pkw->n == KW_CONST_CAST))
	{
		pPP->jfatalerror("xxx_cast not yet supported");
/*
	dynamic_cast < type-id > ( expression )
	static_cast < type-id > ( expression )
	reinterpret_cast < type-id > ( expression )
	const_cast < type-id > ( expression )
	*/
	}
	else if (curtok->symbol == '[')
	{
		while (curtok->symbol == '[')
		{
			CExpression* pExpr2 = new CExpression;
			pExpr2->tok = *curtok;
			GetToken2();

			pExpr2->left = pExpr;
			pExpr2->right = expression(pScope);

			pExpr = pExpr2;

			EatSymbol(']');
		}

		return pExpr;
	}
	else if (curtok->symbol == '(')
	{
		CExpression* pExpr2 = new CExpression;
		pExpr2->tok = *curtok;

		pExpr2->left = pExpr;

		GetToken2();
		if (curtok->type != CToken::tok_symbol || curtok->symbol != ')')
		{
			expression_list(pScope);
		}

		EatSymbol(')');

		pExpr = pExpr2;

		return pExpr;
	}
	else if (curtok->symbol == '.')
	{
		while (curtok->symbol == '.')
		{
			CExpression* pExpr2 = new CExpression;
			pExpr2->tok = *curtok;
			GetToken2();

			pExpr2->left = pExpr;
			//templateopt id-expression
			//pExpr2->right = expression(pScope);
			//pExpr2->right = id_expression(
			if (curtok->type != CToken::tok_ident)
			{
				pPP->jerror("Expected member name");
			}

			pExpr2->right = new CExpression;
			pExpr2->right->tok = *curtok;
			GetToken2();

			pExpr = pExpr2;

			//pExpr->m_pType = ExpressionType(pExpr, pScope);

			// TODO, maybe resolve this later
			{
				/*
				*/

			//	pExpr->left->
			}
		}
	}
#endif

	return pExpr;
}

/*
unary-expression:
	postfix-expression
	++ cast-expression
	-- cast-expression
	unary-operator cast-expression
	sizeof unary-expression
	sizeof ( type-id )
	new-expression
	delete-expression

unary-operator: one of
	* & + - ! �
*/
long CPreprocessor::unary_expression()
{
	if (curtokpp->GetKind() == CToken::tok_ident && String(curtokpp->m_astr) == "defined")
	{
		GetToken3();

		long bDefined;

		if (curtokpp->GetKind() == CToken::tok_symbol && curtokpp->symbol == '(')
		{
			GetToken3();

			StringA* ident = identifier();
			bDefined = IfDef(ident);

			EatSymbol(')');
		}
		else
		{
			StringA* ident = identifier();
			bDefined = IfDef(ident);
		}

		return bDefined;
	}
	else if (curtokpp->GetKind() == CToken::tok_symbol && (
	//	curtok->symbol == '*' ||
	//	curtok->symbol == '&' ||
		curtokpp->symbol == '+' ||
		curtokpp->symbol == '-' ||
		curtokpp->symbol == '!' ||
		curtokpp->symbol == '~'))
	{
		int symbol = curtokpp->symbol;
		GetToken2();
		/*
		longCExpression* pnode = new CExpression;
		pnode->tok = *curtok;
		pnode->tok.symbol |= 'U'<<8;	// Unary
		GetToken();

		pnode->right = cast_expression(pScope);
		return pnode;
		*/
		long right = unary_expression();

		if (symbol == '+')
			return right;
		else if (symbol == '-')
			return -right;
		else if (symbol == '!')
			return !right;
		else// if (symbol == '~')
			return ~right;
	}
#if 0
	else if (curtok->type == CToken::tok_keyword && curtok->pkw->n == KW_SIZEOF)
	{
		ASSERT(0);
		GetToken();

		if (curtok->type == CToken::tok_symbol && curtok->symbol == '(')
		{
			GetToken();

			CExpression* pExpr = new CExpression;
			pExpr->tok.type = CToken::tok_integer;

			CType* pType = type_id(pScope);

			pExpr->tok.int32 = pType->get_sizeof();

			EatSymbol(')');

			return pExpr;
		}
		else
		{
			CExpression* pExpr2 = unary_expression(pScope);
			if (pExpr2 == NULL)
			{
				pPP->jerror("Unexpected");
			}
			else
			{
				ExpressionType(pExpr2, pScope);

				CExpression* pExpr = new CExpression;
				pExpr->tok.type = CToken::tok_integer;
				pExpr->tok.int32 = pExpr2->m_pType->get_sizeof();

				delete pExpr2;

				return pExpr;
			}
		}
	}
#endif
	else
	{
		return postfix_expression();
	}
}

#if 0
/*
cast-expression:
	unary-expression
	( type-id ) cast-expression
*/
CExpression* cast_expression(CScope* pScope)
{
#if 0	// TODO
	if (curtok->type == CToken::tok_symbol && curtok->symbol == '(')
	{
		jfatalerror("cast is currently not supported");
		EatSymbol(')');
		cast_expression(pScope);

		return NULL;
	}
	else
#endif
	{
		return unary_expression(pScope);
	}
}
#endif

/*
pm-expression:
	cast-expression
	pm-expression .* cast-expression
	pm-expression ->* cast-expression
*/
long CPreprocessor::pm_expression()
{
	return unary_expression();
}

/*
multiplicative-expression:
	pm-expression
	multiplicative-expression * pm-expression
	multiplicative-expression / pm-expression
	multiplicative-expression % pm-expression
*/
long CPreprocessor::multiplicative_expression()
{
	long pExpr = pm_expression();
	while (curtokpp->GetKind() == CToken::tok_symbol &&
		(curtokpp->symbol == '*' || curtokpp->symbol == '/' || curtokpp->symbol == '%'))
	{
		GetToken2();

		long pExpr2left = pExpr;
		long pExpr2right = pm_expression();

		if (curtokpp->symbol == '*')
			pExpr = pExpr2left * pExpr2right;
		else if (curtokpp->symbol == '/')
			pExpr = pExpr2left / pExpr2right;
		else
			pExpr = pExpr2left % pExpr2right;
	}

	return pExpr;
}

/*
additive-expression:
	multiplicative-expression
	additive-expression + multiplicative-expression
	additive-expression - multiplicative-expression
*/
long CPreprocessor::additive_expression()
{
	long pExpr = multiplicative_expression();
	while (curtokpp->GetKind() == CToken::tok_symbol &&
		(curtokpp->symbol == '+' || curtokpp->symbol == '-'))
	{
		GetToken2();

		long pExpr2left = pExpr;
		long pExpr2right = multiplicative_expression();

		if (curtokpp->symbol == '+')
			pExpr = pExpr2left + pExpr2right;
		else
			pExpr = pExpr2left - pExpr2right;
	}

	return pExpr;
}

/*
shift-expression:
	additive-expression
	shift-expression << additive-expression
	shift-expression >> additive-expression
*/
long CPreprocessor::shift_expression()
{
	return additive_expression();
}

/*
relational-expression:
	shift-expression
	relational-expression < shift-expression
	relational-expression > shift-expression
	relational-expression <= shift-expression
	relational-expression >= shift-expression
*/
long CPreprocessor::relational_expression()
{
	long pExpr = shift_expression();
	while (curtokpp->GetKind() == CToken::tok_symbol &&
		(curtokpp->symbol == '<' || curtokpp->symbol == '>' || curtokpp->symbol == '<=' || curtokpp->symbol == '>='))
	{
		int sym = curtokpp->symbol;

		GetToken2();

		long pExpr2left = pExpr;
		long pExpr2right = shift_expression();

		if (sym == '<')
			pExpr = pExpr2left < pExpr2right;
		else if (sym == '>')
			pExpr = pExpr2left > pExpr2right;
		else if (sym == '<=')
			pExpr = pExpr2left <= pExpr2right;
		else
			pExpr = pExpr2left >= pExpr2right;
	}

	return pExpr;
}

/*
equality-expression:
	relational-expression
	equality-expression == relational-expression
	equality-expression != relational-expression
*/
long CPreprocessor::equality_expression()
{
	long pExpr = relational_expression();

	while (curtokpp->GetKind() == CToken::tok_symbol &&
		(curtokpp->symbol == '==' || curtokpp->symbol == '!='))
	{
		int sym = curtokpp->symbol;

		GetToken2();

		long pExpr2left = pExpr;
		long pExpr2right = relational_expression();

		if (sym == '==')
			pExpr = pExpr2left == pExpr2right;
		else
			pExpr = pExpr2left != pExpr2right;
	}

	return pExpr;

}

/*
and-expression:
	equality-expression
	and-expression & equality-expression
*/
long CPreprocessor::and_expression()
{
	long pExpr = equality_expression();

	while (curtokpp->GetKind() == CToken::tok_symbol &&
		(curtokpp->symbol == '&'))
	{
		GetToken2();

		long pExpr2left = pExpr;
		long pExpr2right = equality_expression();

		pExpr = pExpr2left & pExpr2right;
	}

	return pExpr;
}

/*
exclusive-or-expression:
	and-expression
	exclusive-or-expression � and-expression
*/
long CPreprocessor::exclusive_or_expression()
{
	return and_expression();
}

/*
inclusive-or-expression:
	exclusive-or-expression
	inclusive-or-expression | exclusive-or-expression
*/
long CPreprocessor::inclusive_or_expression()
{
	long pExpr = exclusive_or_expression();

	while (curtokpp->GetKind() == CToken::tok_symbol &&
		(curtokpp->symbol == '|'))
	{
		GetToken2();

		long pExpr2left = pExpr;
		long pExpr2right = exclusive_or_expression();

		pExpr = pExpr2left | pExpr2right;
	}

	return pExpr;
}

/*
logical-and-expression:
	inclusive-or-expression
	logical-and-expression && inclusive-or-expression
*/
long CPreprocessor::logical_and_expression()
{
	long pExpr = inclusive_or_expression();

	while (curtokpp->GetKind() == CToken::tok_symbol &&
		(curtokpp->symbol == '&&'))
	{
		GetToken2();

		long pExpr2left = pExpr;
		long pExpr2right = inclusive_or_expression();

		pExpr = pExpr2left && pExpr2right;
	}

	return pExpr;
}

/*
logical-or-expression:
	logical-and-expression
	logical-or-expression || logical-and-expression
*/
long CPreprocessor::logical_or_expression()
{
	long pExpr = logical_and_expression();

	while (curtokpp->GetKind() == CToken::tok_symbol &&
		(curtokpp->symbol == '||'))
	{
		GetToken2();

		long pExpr2left = pExpr;
		long pExpr2right = logical_and_expression();

		pExpr = pExpr2left || pExpr2right;
	}

	return pExpr;
}

#if 0
/*
conditional-expression:
	logical-or-expression
	logical-or-expression ? expression : assignment-expression
*/
static CExpression* conditional_expression(CScope* pScope)
{
	return logical_or_expression(pScope);
}

/*
assignment-expression:
	conditional-expression
	logical-or-expression assignment-operator assignment-expression
	throw-expression

assignment-operator: one of
	= *= /= %= += -= >>= <<= &= �= |=
*/

long CPreprocessor::assignment_expression()
{
	CExpression* pExpr = logical_or_expression(pScope);
	if (curtok->type == CToken::tok_symbol && curtok->symbol == '=')
	{
		CExpression* pExpr2 = new CExpression;
		pExpr2->tok = *curtok;
		GetToken();

		pExpr2->left = pExpr;
		pExpr2->right = assignment_expression(pScope);

		pExpr = pExpr2;
	}

	return pExpr;

//	return conditional_expression(pScope);
}
#endif

long CPreprocessor::constant_expression()
{
	return logical_or_expression();
}

void CPreprocessor::replacement_list(CDefine* pDefine)
{
	while (!m_pFile->_eof())
	{
		if (curtokpp->GetKind() == CToken::tok_symbol && curtokpp->symbol == '\n')
		{
			break;
		}
		pDefine->m_replacementList.push_back(curtokpp);
		GetToken4();	// or GetToken3()	// ??
	}
	if (pDefine->m_replacementList.size())
	{
		if (pDefine->m_replacementList[pDefine->m_replacementList.size()-1]->GetKind() == CToken::tok_space)
		{
			pDefine->m_replacementList.RemoveAt(pDefine->m_replacementList.GetSize()-1);
			//pDefine->m_replacementList.erase(pDefine->m_replacementList.end()-1);
		}
	}
}

/*
elif-groups:
	elif-group
	elif-groups elif-group

elif-group:
	# elif constant-expression new-line groupopt

else-group:
	# else new-line groupopt

endif-line:
	# endif new-line
*/

/*
if-section:
	if-group elif-groupsopt else-groupopt endif-line
if-group:
	# if constant-expression new-line groupopt
	# ifdef identifier new-line groupopt
	# ifndef identifier new-line groupopt
*/

void CPreprocessor::if_section(int directive)
{
	CIf* pIf = new CIf;
	pIf->m_pPrev = m_pIf;
	m_pIf = pIf;

	bool bValue;

	if (directive == PP_IF)
	{
		bValue = constant_expression() != 0;
		newline();
	}
	else	// ifdef, ifndef
	{
		String ident = identifier();
		newline();

		bValue = IfDef(ident);

		if (directive == PP_IFNDEF)
		{
			bValue = !bValue;
		}
	}

//	printf("%s\n", bValue?"true":"false");

// We process if/ifdef/indef, elif and else groups, skipping them if necessary until we find the correct one to process

	if (bValue)
	{
		m_pIf->m_bCondition = true;
		return;
	}
	skip_group();

// elif-groups opt
	while (!m_pFile->_eof())
	{
		if (String(curtokpp->m_astr) != "elif")
		{
			break;
		}
		GetToken2();

		bValue = constant_expression() != 0;
		newline();

		if (bValue)
		{
			m_pIf->m_bCondition = true;
			return;
		}

		skip_group();
	}

	if (String(curtokpp->m_astr) == "else")
	{
		GetToken2();

		newline();

		m_pIf->m_bCondition = true;
		return;
	//	skip_group();
	}

	endif();
}

void CPreprocessor::endif()
{
	if (curtokpp->GetKind() != CToken::tok_ident || String(curtokpp->m_astr) != "endif")
	{
		jfatalerror("Expected #endif");
	}
	GetToken();
	newline_or_eof();

	// Back up one level
	CIf* pPrevIf = m_pIf->m_pPrev;
//	delete m_pIf;
	m_pIf = pPrevIf;
}

void CPreprocessor::skip_if_section(int directive)
{
	skipline();
	skip_group();

// elif-groups opt
	while (!m_pFile->_eof())
	{
		if (String(curtokpp->m_astr) != "elif")
		{
			break;
		}

		skipline();
		skip_group();
	}

// else-group opt
	if (curtokpp->GetKind() == CToken::tok_ident && String(curtokpp->m_astr) == "else")
	{
		GetToken();

		newline();
		skip_group();
	}

	if (curtokpp->GetKind() != CToken::tok_ident || String(curtokpp->m_astr) != "endif")
	{
		jfatalerror("Expected #endif");
	}
	GetToken();
}

/*
lparen:
	the left-parenthesis character without preceding white-space
*/

/*
control-line:
# include pp-tokens new-line
# define identifier replacement-list new-line
# define identifier lparen identifier-listopt ) replacement-list new-line
# undef identifier new-line
# line pp-tokens new-line
# error pp-tokensopt new-line
# pragma pp-tokensopt new-line
# new-line
*/

/*
group-part:
	pp-tokensopt new-line
	if-section
	control-line
*/

int CPreprocessor::directive()
{
	if (curtokpp->GetKind() == CToken::tok_ident)
	{
		int i;
		for (i = 0; i < PP_MAX; i++)
		{
			if (String(curtokpp->m_astr) == pp_kws[i])
			{
				break;
			}
		}

		if (i < PP_MAX)
		{
			int directive = i;

			switch (directive)
			{
			case PP_ENDIF:
				{
					//printf("#endif\n");
					if (m_pIf)
					{
						// Back up one level
						CIf* pPrevIf = m_pIf->m_pPrev;
						delete m_pIf;
						m_pIf = pPrevIf;
					}
					else
					{
						jfatalerror("Unexpected #endif");
					}

					GetToken2();
			//		newline_or_eof();
					skipline();

					return false;
				}
				break;

			case PP_ELIF:
				{
					//printf("#elif\n");
					if (m_pIf == NULL)
					{
						jfatalerror("Unexpected #elif");
					}

					ASSERT(m_pIf->m_bCondition);

					while (!m_pFile->_eof())
					{
						if (String(curtokpp->m_astr) != "elif")
						{
							break;
						}
						GetToken2();

						bool bValue = constant_expression() != 0;
						newline();

						skip_group();
					}

					if (String(curtokpp->m_astr) == "else")
					{
						GetToken2();

						newline();

						/*
						m_pIf->m_bCondition = TRUE;
						return;
						*/
						skip_group();
					}

					endif();

/*
					BOOL bValue = constant_expression();
					newline();

					skip_group();

					else_group_opt();
*/

					return false;
				}
				break;

			case PP_ELSE:
				{
					//printf("#else\n");
					if (m_pIf == NULL)
					{
						jfatalerror("Unexpected #else");
					}

					GetToken3();
					newline();

					ASSERT(m_pIf->m_bCondition);
					skip_group();
					endif();

					return false;
				}
				break;
			}

			switch (directive)
			{
			// if-section
			case PP_IF:
			case PP_IFDEF:
			case PP_IFNDEF:
				{
					GetToken3();

					//printf("#if ");
					if_section(directive);
				}
				break;

				// Sigurd Lerstad extension
			case PP_IMPORT:
				{
					GetToken();	// Next token with macro expansion

					GetHeaderName();

					CToken* tok = curtokpp;
					GetToken2();

					FilePath importfile(tok->m_astr);

					String name = importfile.get_Name();

					/*
					_splitpath(CString(importfile), dir, path, NULL, NULL);

				//	CFile* pFile = new CFile;
					char dir[4];
					char path[260];
					_splitpath(CString(m_pFile->m_filepath), dir, path, NULL, NULL);

					char filepath[260];
					_makepath(filepath, dir, path, CString(tok->m_astr), NULL);
					*/

					FilePath filepath;
					if (m_options->m_x64)
						filepath = FilePath("../../bin/x64/Debug/");
					else
						filepath = FilePath("../../bin/Win32/Debug/");

					filepath += Filename(name, ".typeinfo");

					String filename = filepath.ToString();

					if (true)
					{
						IO::FileStream file(filename, IO::FileMode_Open, IO::FileAccess_Read);
						if (!file.IsHandleValid())
						{
							Std::get_Out() << m_pFile->m_filepath << paren(m_pFile->m_line+1) << " : error C00: Couldn't open file '" << filepath << "'" << "\n";
							++m_errors;
							break;
						}

						FILETIME filetime;
						GetFileTime(file.GetFileHandle(), NULL, NULL, &filetime);
						m_includeFiles.push_back(IncludeFile(filename, filetime));

						TypeArchive ar(TypeArchive::Mode_Load, &file, filename);
						ar.m_typestuff = new TypeStuff;

						ar.m_pGlobalNamespace = new Namespace;
						ar.m_bSortedDecls = true;

						ar.m_typelib = new TypeLib;
						ar.ReadHeader();
						ar.MapObject(ar.m_typelib);
						ar.m_typelib->Load(ar);

						/*
						ar >> ar.m_typelib;

						uint ntypelibs;
						ar >> ntypelibs;
						{
							for (uint i = 0; i < ntypelibs; i++)
							{
								TypeLib* typelib;
								ar >> typelib;
							}
						}

						Namespace* globalNamespace;
						ar >> globalNamespace;

						uint ntypes;
						ar >> ntypes;

						for (uint i = 0; i < ntypes; i++)
						{
							NamedType* pType;
							ar >> pType;
							ASSERT(pType->m_typelib == ar.m_typelib);

							// This isn't necessarily true
						//	VERIFY(pType->m_ownerScope->m_decls.find(pType->m_name) != pType->m_ownerScope->m_decls.end());
						}
						*/

						/*
						{
							for (int i = 0; i < ntypes; i++)
							{
								ULONG_PTR address;
								ar >> address;
							}
						}
						*/

						{
							int indefines;
							ar >> indefines;
							ASSERT(indefines == 0);
						}

						{
							int outdefines;
							ar >> outdefines;

							for (int i = 0; i < outdefines; i++)
							{
								CDefine* define;
								ar >> define;

								Define(define);
							}
						}

					//	file.Close();
					}

					GetToken();

					{
						CToken* tok = new ImportToken;
					//	tok->type = CToken::tok_import;
						tok->m_astr = (StringA*)filename.m_stringObject;//new ImmutableString<char>(string_copy(filename));
						tok->m_astr->IncRef();

				//	m_stack.push(tok);
						curtokpp = tok;
					}
				}
				break;

				/*
				control-line:
				# include pp-tokens new-line
				# define identifier replacement-list new-line
				# define identifier lparen identifier-listopt ) replacement-list new-line
				# undef identifier new-line
				# line pp-tokens new-line
				# error pp-tokensopt new-line
				# pragma pp-tokensopt new-line
				# new-line
				*/
			case PP_INCLUDE:
				{
					GetToken();	// Next token with macro expansion

					GetHeaderName();

					CToken* tok = curtokpp;
					GetToken2();
				//	newline_or_eof();

					bool bOk = false;
					if (tok->GetKind() == CToken::tok_string_literal)
					{
						CFile* pFile = new CFile;
						/*
						char drive[4];
						char dir[260];
						_splitpath(CString(m_pFile->m_filepath), drive, dir, NULL, NULL);

						char filepath[260];
						_makepath(filepath, drive, dir, CString(tok->m_astr).c_str(), NULL);
						*/

						String filepath = IO::Path::Combine(IO::Path::GetDirectoryName(m_pFile->m_filepath), tok->m_astr);
						Std::get_Out() << filepath << endl;

						if (pFile->open(filepath))
						{
							m_includeFiles.push_back(IncludeFile(pFile->m_filepath, pFile->m_filetime));

							pFile->m_pPrev = m_pFile;
							m_pFile = pFile;

							GetToken();
							bOk = true;
						}
						else
						{
							delete pFile;
						}
					}

					if (!bOk)
					{
						/*
						vector<String> include_paths;

						
						// TODO
						include_paths.push_back("D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\include\\");
						include_paths.push_back("D:\\Users\\Sigurd\\Documents\\VSProjects\\FirstProject2\\include2\\");

						include_paths.push_back("C:\\Program Files\\Microsoft SDKs\\Windows\\v7.0\\Include\\");
						*/

						//char* includecstr = CString(tok->m_astr).Detach();

						String mpath = IO::Path::GetDirectoryName(_Module.GetFullPath());

						String includecstr = tok->m_astr;

						for (auto it = m_includeDirs.begin(); it != m_includeDirs.end(); ++it)
						{
							CFile* pFile = new CFile;
							/*
							char dir[4];
							char path[260];
							_splitpath_s(CString(*it).c_str(), dir, _countof(dir), path, _countof(path), NULL, 0, NULL, 0);

							f.set_Path();

							char filepath[260];
							_makepath_s(filepath, dir, path, includecstr, NULL);
							*/

						//	FilePath filepath = mpath;
							//filepath += FilePath(*it);

						//	filepath.set_NameExt(includecstr);
							String filepath2 = IO::Path::Combine(mpath, *it);
							String filepath = IO::Path::Combine(filepath2, includecstr);
							Std::get_Out() << filepath << endl;

						//	String filepath = IO::Path::Combine(full, includecstr);
							//Std::get_Out() << filepath << endl;

							if (pFile->open(filepath))
							{
								pFile->m_pPrev = m_pFile;
								m_pFile = pFile;

								GetToken();
								bOk = true;
								break;
							}
							else
							{
								delete pFile;
							}
						}
					}

					if (!bOk)
					{
						jfatalerror(sfatalerror() << "Cannot open include file '" << String(tok->m_astr));
					}
				}
				break;
				
			case PP_DEFINE:
				{
					GetToken3();

					if (curtokpp->GetKind() != CToken::tok_ident)
					{
						jfatalerror("Expected identifier after #define");
					}

				//	printf("#define %s\n", ident.c_str());

					//CDefine* pDefine = Define(curtokpp->ident);
					CDefine* pDefine = new CDefine;
					pDefine->m_name = curtokpp->m_astr;

				//	printf("%s\n", pDefine->m_name.c_str());
				//	if (!strcmp(pDefine->m_name.c_str(), "__RPC_USER"))
				//	{
				//	}

					int c = m_pFile->_getc();
					if (c == '(')	// No intervening spaces between identifer and left parenthesis
					{
						GetToken3();
						pDefine->m_bFunctionLike = true;

						if ((curtokpp->GetKind() != CToken::tok_symbol && curtokpp->GetKind() != CToken::tok_char) ||
							(curtokpp->symbol != ')'))
						{
							identifier_list(pDefine);
						}

						EatSymbol(')');
					}
					else
					{
						pDefine->m_bFunctionLike = false;
						m_pFile->_ungetc(c);
						GetToken3();
					}

#if 0
					skipspaces();
#endif
					replacement_list(pDefine);

					newline_or_eof();

					//CDefine* pDefine2 = Find(pDefine->m_name.c_str());
					macromap::iterator pDefine2 = m_defines.find(pDefine->m_name);
					if (pDefine2 != m_defines.end())
					{
						if (!pDefine->Equals((*pDefine2).second))
						{
							jerror(serror() << "Redefinition of macro '" << pDefine->m_name <<
								"' : first seen :\n" << 
								(*pDefine2).second->m_location);
						}

						delete pDefine;
						pDefine = NULL;
					}

					if (pDefine)
					{
						Define(pDefine);
					}
				}
				break;
				
			case PP_UNDEF:
				{
					GetToken3();

					StringA* ident = identifier();

					Undef(ident);

					newline_or_eof();
				}
				break;
				
			case PP_LINE:
				{
					GetToken3();
					jfatalerror("#line not supported yet");
				}
				break;
				
			case PP_ERROR:
				{
					jfatalerror("#error");
#if 0
					StringA* errorString = new StringA("#error");
					while (!m_pFile->_eof())
					{
						int c = m_pFile->_getc();
						if (c == '\r')
						{
							int c2 = m_pFile->_getc();
							ASSERT(c2 == '\n');
							break;
						}
						else if (c == '\n')
						{
							break;
						}
						errorString = errorString->Append(c);
					}
					jerror(errorString->c_str());
					GetToken2();
#endif
				}
				break;
				
			case PP_PRAGMA:
				{
					//GetToken3();
					CToken* tok = new SymbolToken;
				//	tok->type = CToken::tok_symbol;
					tok->symbol = '#';

				//	m_stack.push(tok);
					curtokpp = tok;

#if 0
					GetToken3();
				//	jwarn("#pragma not supported yet");

					if (m_pCompiler)
						m_pCompiler->OnPragma();

					skipline();
				//	newline_or_eof();
#endif
				}
				break;
				
			default:
				ASSERT(0);
			}
		}
		else
		{
			char msg[1024];
			sprintf_s(msg, "invalid preprocessor command '%s'", CString(curtokpp->m_astr));
			jfatalerror(msg);
		}
	}
	else if (curtokpp->GetKind() == CToken::tok_symbol && curtokpp->symbol == '\n')
	{
		GetToken2();
	}
	else
	{
		jfatalerror("invalid preprocessor command");
	}
	return true;
}

int CPreprocessor::group_part()
{
	GetToken();

	if (curtokpp->symbol == '#')
	{
		GetToken();
		return directive();
	}
	else
	{
		skipline();
	}

	return true;
}

int CPreprocessor::skip_group_part()
{
//	GetToken();

	if (curtokpp->symbol == '#')
	{
		GetToken();

		if (curtokpp->GetKind() == CToken::tok_ident)
		{
			int i;
			for (i = 0; i < PP_MAX; i++)
			{
				if (String(curtokpp->m_astr) == pp_kws[i])
				{
					break;
				}
			}

			if (i < PP_MAX)
			{
				int directive = i;

				switch (directive)
				{
				case PP_ELIF:
					{
						return false;
					}
					break;

				case PP_ELSE:
					{
						return false;
					}
					break;

				case PP_ENDIF:
					{
						return false;
					}
					break;
				}

				GetToken2();

				switch (directive)
				{
				// if-section
				case PP_IF:
				case PP_IFDEF:
				case PP_IFNDEF:
					{
						skip_if_section(directive);
					}
					break;
					
				default:
					skipline();
				}
			}
		}
		else if (curtokpp->GetKind() == CToken::tok_symbol && curtokpp->symbol == '\n')
		{
			GetToken();
		}
	}
	else
	{
		skipline();
	}

	return true;
}

/*
group:
	group-part
	group group-part
*/
void CPreprocessor::group()
{
	ASSERT(0);
	while (!m_pFile->_eof())
	{
		if (!group_part())
			break;
	}
}

void CPreprocessor::skip_group()
{
	while (!m_pFile->_eof())
	{
		if (!skip_group_part())
			break;
	}
}

#if 0
void macro_replace(CDefines* pDefs, std::vector<CToken>& tokens)
{
	for (int i = 0; i < tokens.size(); i++)
	{
		CToken* tok = &tokens[i];

		if (tok->type == CToken::tok_ident)
		{
			CDefine* pDefine = pDefs->Find(tok->ident);
			if (pDefine)
			{
			}
		}
	}
}
#endif

}	// PP
}	// System

/*
preprocessing-file:
	groupopt
*/
#if 0
void preprocessing_file()
{
	PP::CDefines defs;
	PP::group(&defs);
}
#endif
