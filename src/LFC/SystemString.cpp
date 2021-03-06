#include "stdafx.h"
#include "LFC.h"

namespace System
{

char* StringBuilder::BuildString(char* p) const
{
	p -= m_str.GetLength();
	m_str.CopyTo(p, m_str.GetLength());

	ASSERT(m_prev);
	return m_prev->BuildString(p);
}

WCHAR* StringBuilder::BuildString(WCHAR* pw) const
{
	pw -= m_str.GetLength();
	m_str.CopyTo(pw, m_str.GetLength());

	ASSERT(m_prev);
	return m_prev->BuildString(pw);
}

String StringBuilderBase::ToString()
{
	return BuildString();
}

StringObject* StringBuilderBase::BuildString() const
{
	size_t totallen = GetLength();

	if (m_wide)
	{
		StringObject* stringObject = new ImmutableString<WCHAR>(new WCHAR[totallen], totallen);
		WCHAR* pw = (WCHAR*)stringObject->GetData() + stringObject->GetLength();

		BuildString(pw);

		return stringObject;
	}
	else
	{
		StringObject* stringObject = new ImmutableString<char>(new char[totallen], totallen);
		char* p = (char*)stringObject->GetData() + stringObject->GetLength();

		BuildString(p);

		return stringObject;
	}
}

StringObject* StringBuilderBase::BuildMutableString() const
{
	size_t totallen = GetLength();

	if (m_wide)
	{
		StringObject* stringObject = new MutableString<WCHAR>(totallen);
		WCHAR* pw = (WCHAR*)stringObject->GetData() + stringObject->GetLength();

		BuildString(pw);

		return stringObject;
	}
	else
	{
		StringObject* stringObject = new MutableString<char>(totallen);
		char* p = (char*)stringObject->GetData() + stringObject->GetLength();

		BuildString(p);

		return stringObject;
	}
}

char* StringBuilderTwo::BuildString(char* p) const
{
	p -= m_str2.GetLength();
	m_str2.CopyTo(p, m_str2.GetLength());

	p -= m_str1.GetLength();
	m_str1.CopyTo(p, m_str1.GetLength());

	return p;
}

WCHAR* StringBuilderTwo::BuildString(WCHAR* pw) const
{
	pw -= m_str2.GetLength();
	m_str2.CopyTo(pw, m_str2.GetLength());

	pw -= m_str1.GetLength();
	m_str1.CopyTo(pw, m_str1.GetLength());

	return pw;
}

/*
StringObject* StringBuilderTwo::BuildString() const
{
	size_t totallen = GetTotalLength();

	StringObject* stringObject = new ImmutableString<WCHAR>(new WCHAR[totallen], totallen);
	WCHAR* pw = (WCHAR*)stringObject->GetData() + stringObject->GetLength();

	BuildString(pw);

	return stringObject;
}
*/

String::String(const StringBuilderBase& builder)
{
	m_stringObject = builder.BuildString();
	if (m_stringObject)
	{
		m_stringObject->IncRef();
	}
}

int StringObject::GetLength31()
{
	size_t len = GetLength();
	if (len > INT_MAX) raise(SystemException(L"String is larger than INT_MAX"));
	return (int)len;
}

uint32 StringObject::GetLength32()
{
	size_t len = GetLength();
#ifdef _M_X64
	if (len > UINT_MAX) raise(SystemException(L"String is larger than UINT_MAX"));
#endif
	return (uint32)len;
}

StringObject* StringObject::Insert(StringObject* str, size_t offset)
{
	switch (str->GetCharSize())
	{
	case 1:	return Insert((char8*)str->GetData(), offset, str->GetLength());
	case 2:	return Insert((char16*)str->GetData(), offset, str->GetLength());
//	case 4:	return Insert((char32_t*)str->GetData(), offset, str->GetLength());
	}
	raise(SystemException("Invalid charsize"));
}

// ImmutableString

template<class T>
uint32 ImmutableString<T>::GetHashCode()
{
	uint32 hashCode = 0;
	for (size_t i = 0; i < m_data.size(); ++i)
	{
		hashCode += hashCode*31 + m_data[i];
	}

	return hashCode;
}

template<class T>
size_t ImmutableString<T>::Find(uint32 ch, size_t startoffset)
{
	for (size_t i = startoffset; i < m_data.size(); ++i)
	{
		if (m_data[i] == ch) return i;
	}

	return ~0;
}

template<class T>
size_t ImmutableString<T>::Count(uint32 ch)
{
	size_t count = 0;

	for (size_t i = 0; i < m_data.size(); ++i)
	{
		if (m_data[i] == ch) ++count;
	}

	return count;
}

#if 0
template<class T>
template<class T2>
int ImmutableString<T>::_Compare(const T2* otherstr, unsigned int otherlen) const
{
	/*
	if (this == str) return 0;	// Equal
	if (this == NULL) return -1;
	if (otherstr == NULL) return 1;
*/

	ASSERT(this);
	ASSERT(otherstr);

	uint i;
	for (i = 0; i < m_data.size()/*m_len*/; i++)
	{
		T c = m_data[i];

		if (i == otherlen) return 1;	// return > 0

		T2 c2 = otherstr[i];

		int diff = c - c2;
		if (diff)
			return diff;
	}

	if (m_data.size() == otherlen)
		return 0;
	else
		return -1;
}
#endif

template<class T>
int ImmutableString<T>::SubCompare(ptrdiff_t offset, size_t len, const char* otherstr, size_t otherlen)
{
	if (this == nullptr && otherstr == nullptr) return 0;
	if (this == nullptr) return -1;
	if (otherstr == nullptr) return 1;

	return _Compare(m_data.begin() + offset, len, otherstr, otherlen);
}

template<class T>
int ImmutableString<T>::SubCompare(ptrdiff_t offset, size_t len,  const char16* otherstr, size_t otherlen)
{
	if (this == nullptr && otherstr == nullptr) return 0;
	if (this == nullptr) return -1;
	if (otherstr == nullptr) return 1;

	return _Compare(m_data.begin() + offset, len, otherstr, otherlen);
}

template<class T>
int ImmutableString<T>::SubCompare(ptrdiff_t offset, size_t len,  const char32_t* otherstr, size_t otherlen)
{
	if (this == nullptr && otherstr == nullptr) return 0;
	if (this == nullptr) return -1;
	if (otherstr == nullptr) return 1;

	return _Compare(m_data.begin() + offset, len, otherstr, otherlen);
}

template<class char_type>
int ImmutableString<char_type>::SubCompare(ptrdiff_t offset, size_t len,  StringObject* otherstr)
{
	if (this == otherstr) return 0;
	if (this == nullptr) return -1;
	if (otherstr == nullptr) return 1;

	size_t otherlen = otherstr->GetLength();
	switch (otherstr->GetCharSize())
	{
		case 1: return _Compare(m_data.begin(), m_data.size(), (char8*)otherstr->GetData(), otherlen);
		case 2: return _Compare(m_data.begin(), m_data.size(), (char16*)otherstr->GetData(), otherlen);
		case 4: return _Compare(m_data.begin(), m_data.size(), (char32_t*)otherstr->GetData(), otherlen);
	}

	ASSERT(0);
	return -1;
}

/*
template<class T>
ImmutableString<T>* ImmutableString<T>::ReplaceChar(T c, T c2)
{
	__gc_allocator allocator;

	T* buffer = (T*)allocator.allocate_buffer((m_data.size()+1)*sizeof(T));
	buffer[m_data.size()] = 0;
	for (int i = 0; i < m_data.size(); i++)
	{
		if (m_data[i] == c)
			buffer[i] = c2;
		else
			buffer[i] = m_data[i];
	}

	return new StringT<T>(true, buffer, m_data.size());
}
*/

//////////////////////////////////////////
// MutableString

template<class T>
size_t MutableString<T>::Find(uint32 ch, size_t startoffset)
{
	for (size_t i = startoffset; i < m_data.size(); ++i)
	{
		if (m_data[i] == ch) return i;
	}

	return ~0;
}

template<class T>
size_t MutableString<T>::Count(uint32 ch)
{
	size_t count = 0;

	for (size_t i = 0; i < m_data.size(); ++i)
	{
		if (m_data[i] == ch) ++count;
	}

	return count;
}

template<class char_type> StringObject* MutableString<char_type>::Append(const char* cstr, size_t len)
{
	if (m_refcount > 1)
	{
		size_t offset = m_data.size();
		return new MutableString<char_type>(m_data.begin(), m_data.size(), cstr, len, offset);
	}
	else
	{
		size_t oldlen = m_data.size();
		m_data.resize(oldlen + len);
		char_type* p = m_data.begin() + oldlen;
		while (len--)
		{
			*p++ = char_type(*cstr++);
		}

		return this;
	}
}

template<class char_type> StringObject* MutableString<char_type>::Append(const wchar_t* cstr, size_t len)
{
	if (m_refcount > 1)
	{
		size_t offset = m_data.size();

		return new MutableString<char_type>(m_data.begin(), m_data.size(), cstr, len, offset);
	}
	else
	{
		size_t oldlen = m_data.size();
		m_data.resize(oldlen + len);
		char_type* p = m_data.begin() + oldlen;
		while (len--)
		{
			*p++ = char_type(*cstr++);
		}

		return this;
	}
}

template<> LFCEXT StringObject* MutableString<char>::Append(const char* cstr, size_t len)
{
	if (m_refcount > 1)
	{
		size_t offset = m_data.size();	// At end
		return new MutableString<char>(m_data.begin(), m_data.size(), cstr, len, offset);
	}
	else
	{
		m_data.insert(m_data.end(), cstr, cstr + len);
		return this;
	}
}

///////

template<> StringObject* MutableString<wchar_t>::Append(const wchar_t* cstr, size_t len)
{
	if (m_refcount > 1)
	{
		size_t offset = m_data.size();	// At end
		return new MutableString<wchar_t>(m_data.begin(), m_data.size(), cstr, len, offset);
	}
	else
	{
		m_data.insert(m_data.end(), cstr, cstr + len);
		return this;
	}
}

template<class char_type> StringObject* MutableString<char_type>::Append(StringObject* stringData)
{
	if (stringData == NULL) return this;

	size_t otherlen = stringData->GetLength();

	if (m_refcount > 1)
	{
		size_t offset = m_data.size();	// At end

		switch (stringData->GetCharSize())
		{
		case 1:
			return new MutableString<char_type>(m_data.begin(), m_data.size(), (char*)stringData->GetData(), otherlen, offset);
		case 2:
			return new MutableString<char_type>(m_data.begin(), m_data.size(), (char16*)stringData->GetData(), otherlen, offset);

		default:
			return new MutableString<char_type>(m_data.begin(), m_data.size(), (char32_t*)stringData->GetData(), otherlen, offset);
		}
	}
	else
	{
		size_t offset = m_data.size();
		m_data.resize(m_data.size()+otherlen);
		char_type* dstdata = m_data.begin() + offset;

		switch (stringData->GetCharSize())
		{
		case 1:
			{
				char* srcdata = (char*)stringData->GetData();

				for (size_t i = 0; i < otherlen; i++)
				{
					*dstdata++ = char_type(*srcdata++);
				}
			}
			break;

		case 2:
			{
				char16* srcdata = (char16*)stringData->GetData();

				for (size_t i = 0; i < otherlen; i++)
				{
					*dstdata++ = char_type(*srcdata++);
				}
			}
			break;

		default:
			{
				char32_t* srcdata = (char32_t*)stringData->GetData();

				for (size_t i = 0; i < otherlen; i++)
				{
					*dstdata++ = char_type(*srcdata++);
				}
			}
		}

		return this;
	}
}

template<class T> StringObject* MutableString<T>::Insert(const char* cstr, size_t offset, size_t len)
{
	if (m_refcount > 1)
	{
	//	size_t offset = m_data.size();
		return new MutableString<char>(m_data.begin(), m_data.size(), cstr, len, offset);
//		return new MutableString<char>(cstr, len, m_data.begin(), m_data.size(), offset);
	}
	else
	{
		size_t oldlen = m_data.size();
		m_data.resize(oldlen + len);
		memmove(m_data.begin()+offset+len, m_data.begin()+offset, (oldlen-offset)*sizeof(T));

		T* p = m_data.begin()+offset;
		while (len--)
		{
			*p++ = T(*cstr++);
		}

		return this;
	}
}

template<class T> StringObject* MutableString<T>::Insert(const wchar_t* cstr, size_t offset, size_t len)
{
	if (m_refcount > 1)
	{
	//	size_t offset = m_data.size();
		return new MutableString<char>(m_data.begin(), m_data.size(), cstr, len, offset);
	}
	else
	{
		size_t oldlen = m_data.size();
		m_data.resize(oldlen + len);
		memmove(m_data.begin()+offset+len, m_data.begin()+offset, (oldlen-offset)*sizeof(T));

		T* p = m_data.begin()+offset;
		while (len--)
		{
			*p++ = T(*cstr++);
		}

		return this;
	}
}

template<class T> StringObject* MutableString<T>::Delete(size_t offset, size_t len)
{
	if (m_refcount > 1)
	{
		return new MutableString<T>(m_data.begin(), offset, m_data.begin() + offset + len, m_data.size()-len-offset);
	}
	else
	{
		m_data.erase(m_data.begin() + offset, m_data.begin() + offset+len);
		return this;
	}
}

template<class T> StringObject* ImmutableString<T>::Delete(size_t offset, size_t len)
{
	return new MutableString<T>(m_data.begin(), offset, m_data.begin() + offset + len, m_data.size()-len-offset);
}

//template<> StringObject* MutableString<wchar_t>::Insert(const wchar_t* cstr, unsigned int offset, unsigned int len)

/*
template<> LFCEXT StringObject* MutableString<char>::Insert(const char* cstr, unsigned int offset, unsigned int len)
{
	if (m_refCount > 1)
	{
		return new MutableString<char>(m_data.begin(), m_data.size(), cstr, len, offset);
	}
	else
	{
		m_data.insert(m_data.begin()+offset, cstr, cstr+len);
		return this;
	}
}

template<> StringObject* MutableString<wchar_t>::Insert(const wchar_t* cstr, unsigned int offset, unsigned int len)
{
	if (m_refCount > 1)
	{
		return new MutableString<char>(m_data.begin(), m_data.size(), cstr, len, offset);
	}
	else
	{
		m_data.insert(m_data.begin()+offset, cstr, cstr+len);
		return this;
	}
}
*/

template<class T, class T2>
int _Compare(const T* str, size_t len, const T2* otherstr, size_t otherlen)
{
//	if (str == otherstr) return 0;	// Equal
//	if (str == NULL) return -1;
//	if (otherstr == NULL) return 1;

	ASSERT(str);
	ASSERT(otherstr);

	uint i;
	for (i = 0; i < len; i++)
	{
		T c = str[i];

		if (i == otherlen) return 1;	// return > 0

		T2 c2 = otherstr[i];

		int diff = c - c2;
		if (diff)
			return diff;
	}

	if (len == otherlen)
		return 0;
	else
		return -1;
}

template<class T> int MutableString<T>::SubCompare(ptrdiff_t offset, size_t sublen, const char* cstr, size_t len)
{
	return _Compare(m_data.begin() + offset, sublen, cstr, len);
}

template<class T> int MutableString<T>::SubCompare(ptrdiff_t offset, size_t sublen, const wchar_t* cstr, size_t len)
{
	return _Compare(m_data.begin() + offset, sublen, cstr, len);
}

template<class T> int MutableString<T>::SubCompare(ptrdiff_t offset, size_t sublen, const char32_t* cstr, size_t len)
{
	return _Compare(m_data.begin() + offset, sublen, cstr, len);
}

template<class T> int MutableString<T>::SubCompare(ptrdiff_t offset, size_t sublen, StringObject* stringData)
{
	// NOTE: Negate the result
	if (stringData)
		return -stringData->Compare(m_data.begin() + offset, sublen);
	else
		return -1;
}

template<class T> int MutableString<T>::CompareNoCase(const char* cstr, size_t len)
{
	ASSERT(0);
	return -1;
}

template<class T> int MutableString<T>::CompareNoCase(const wchar_t* cstr, size_t len)
{
	ASSERT(0);
	return -1;
}

template<class T> int MutableString<T>::CompareNoCase(StringObject* stringData)
{
	ASSERT(0);
	return -1;
}

template<> LFCEXT int MutableString<char>::CompareNoCase(StringObject* stringData)
{
	if (stringData->GetCharSize() == 1)
		return CompareStringA(LOCALE_USER_DEFAULT, LINGUISTIC_IGNORECASE, m_data.begin(), GetLength32(), (char*)stringData->GetData(), stringData->GetLength32()) - 2;
	else
	{
		VERIFY(0);
		return -1;
	}
}

template<> LFCEXT int MutableString<wchar_t>::CompareNoCase(StringObject* stringData)
{
	if (stringData->GetCharSize() == 2)
		return CompareStringW(LOCALE_USER_DEFAULT, LINGUISTIC_IGNORECASE, m_data.begin(), GetLength32(), (wchar_t*)stringData->GetData(), stringData->GetLength32()) - 2;
	else
	{
		VERIFY(0);
		return -1;
	}
}

template<> LFCEXT int MutableString<char>::CompareNoCase(const char* cstr, size_t len)
{
	if (len > INT_MAX) raise(SystemException(L"String too long"));
	return CompareStringA(LOCALE_USER_DEFAULT, LINGUISTIC_IGNORECASE, m_data.begin(), GetLength32(), cstr, (int)len) - 2;
}

template<> LFCEXT int MutableString<wchar_t>::CompareNoCase(const wchar_t* cstr, size_t len)
{
	if (len > INT_MAX) raise(SystemException(L"String too long"));
	return CompareStringW(LOCALE_USER_DEFAULT, LINGUISTIC_IGNORECASE, m_data.begin(), GetLength32(), cstr, (int)len) - 2;
}

/////////////

#if 0
LFCEXT StringA* StringT<WCHAR>::ToStringA() const
{
#if WIN32

	__gc_allocator allocator;
	string_data<char> strdata(string_alloc<char, __gc_allocator>(Length()*3, allocator));

	uint len = Length();
//	astr->MakeRoom(len*3);

	int count = MSWindows::WideCharToMultiByte(CP_ACP, 0, c_str(), len, strdata.data, len*3, NULL, NULL);
	strdata.data = (char*)allocator.reallocate_buffer(strdata.data, count+1);
	strdata.len = count;
	strdata.data[count] = 0;

	StringA* astr = new StringA(strdata);

#else

	StringA* astr = new StringA(NULL, Length());
	int len = Length();
//	astr->MakeRoom(len);
	for (int i = 0; i < len; i++)
	{
		(astr->begin())[i] = (char)(c_str()[i]);
	}

#endif

	return astr;
}

LFCEXT StringW* StringT<char>::ToStringW() const
{
#if WIN32
	StringW* str = new StringW(string_alloc<WCHAR>(Length()));

	MSWindows::MultiByteToWideChar(CP_ACP, 0, c_str(), Length(), str->begin(), Length());
#else
	uint len = Length();

	StringW* str = new StringW((WCHAR*)NULL, len);

	for (uint i = 0; i < len; i++)
	{
		(str->begin())[i] = (WCHAR)c_str()[i];
	}
#endif

	return str;
}
#endif

/*
template<class char_type1, class char_type2> int cstricmp(const char_type* s1, unsigned int len1, const char_type* s2, unsigned int len2);

template<> int cstricmp<char>(const char* s1, unsigned int len1, const char* s2, unsigned int len2)
{
//	return _stricmp(m_data.begin(), str->m_data.begin());
	ASSERT(0);
	return 0;
}

template<> int cstricmp<char16>(const char16* s1, unsigned int len1, const char16* s2, unsigned int len2)
{
//	return _wcsicmp(m_data.begin(), str->m_data.begin());
	ASSERT(0);
	return 0;
}

template<> int cstricmp<char32_t>(const char32_t* s1, unsigned int len1, const char32_t* s2, unsigned int len2)
{
	ASSERT(0);
	return 0;
}
*/

template<class T>
template<class T2>
int ImmutableString<T>::_CompareNoCase(const T2* otherstr, size_t otherlen) const
{
	/*
	if (this == NULL && otherstr == NULL) return 0;
	if (this == NULL) return -1;
	if (str == NULL) return 1;
*/

	ASSERT(this);
	ASSERT(otherstr);

	ASSERT(0);
	return 0;
//	return cstricmp(m_data.begin(), m_data.size(), otherstr, otherlen);
}

/*
int ImmutableString<char>::_CompareNoCase(const char* otherstr, size_t otherlen) const
{
	ASSERT(this);
	ASSERT(otherstr);
	return CompareStringA(LOCALE_USER_DEFAULT, LINGUISTIC_IGNORECASE, m_data.begin(), m_data.size(), otherstr, otherlen) - 2;
}
*/

template<class char_type>
int ImmutableString<char_type>::CompareNoCase(const char8* str, size_t len)
{
	if (this == nullptr && str == nullptr) return 0;
	if (this == nullptr) return -1;
	if (str == nullptr) return 1;

//	return _stricmp(m_data.begin(), str);
	ASSERT(0);
	return 0;
}

template<class char_type>
int ImmutableString<char_type>::CompareNoCase(const char16* str, size_t len)
{
	if (this == nullptr && str == nullptr) return 0;
	if (this == nullptr) return -1;
	if (str == nullptr) return 1;

	ASSERT(0);
	return 0;
}

template<class char_type>
int ImmutableString<char_type>::CompareNoCase(StringObject* str)
{
	if (this == str) return 0;
	if (this == NULL) return -1;
	if (str == NULL) return 1;

	size_t otherlen = str->GetLength();

	switch (str->GetCharSize())
	{
	case 1:
		return _CompareNoCase((char8*)str->GetData(), otherlen);

	case 2:
		return _CompareNoCase((char16*)str->GetData(), otherlen);

	case 4:
		return _CompareNoCase((char32_t*)str->GetData(), otherlen);
	}

	ASSERT(0);
	return -1;
}

// Insert

template<class char_type>
StringObject* ImmutableString<char_type>::Insert(const char8* cstr, size_t offset, size_t len)
{
	if (len == 0) return this;
	return new MutableString<char_type>(m_data.begin(), m_data.size(), cstr, len, offset);
}

template<class char_type>
StringObject* ImmutableString<char_type>::Insert(const char16* cstr, size_t offset, size_t len)
{
	if (len == 0) return this;
	return new MutableString<char_type>(m_data.begin(), m_data.size(), cstr, len, offset);
}

// Append

template<class char_type>
StringObject* ImmutableString<char_type>::Append(const char8* cstr, size_t len)
{
	size_t offset = m_data.size();	// At end
	return new MutableString<char_type>(m_data.begin(), m_data.size(), cstr, len, offset);
}

template<class char_type>
StringObject* ImmutableString<char_type>::Append(const char16* cstr, size_t len)
{
	size_t offset = m_data.size();	// At end
	return new MutableString<char_type>(m_data.begin(), m_data.size(), cstr, len, offset);
}

template<class char_type>
StringObject* ImmutableString<char_type>::Append(const char32_t* cstr, size_t len)
{
	size_t offset = m_data.size();	// At end
	return new MutableString<char_type>(m_data.begin(), m_data.size(), cstr, len, offset);
}

template<class char_type>
StringObject* ImmutableString<char_type>::Append(StringObject* stringData)
{
	if (stringData == nullptr) return this;

	size_t offset = m_data.size();	// At end

	switch (stringData->GetCharSize())
	{
	case 1:
		return new MutableString<char_type>(m_data.begin(), m_data.size(), (const char*)stringData->GetData(), stringData->GetLength(), offset);

	case 2:
		return new MutableString<char_type>(m_data.begin(), m_data.size(), (const char16*)stringData->GetData(), stringData->GetLength(), offset);

//	case 4:
//		return new MutableString<char_type>(m_data.begin(), m_data.size(), (const char32_t*)stringData->GetData(), stringData->GetLength(), offset);

	default:
		ASSERT(0);
		return NULL;
	}
}

template<class char_type>
StringObject* ImmutableString<char_type>::SubString(size_t offset, size_t len)
{
	// TODO, SubStringObject

	return new ImmutableString<char_type>(string_copy(m_data.begin() + offset, len));
}

static char32_t nullchar[] = {0};

ImmutableString<char> ImmutableString<char>::blank("");
ImmutableString<wchar_t> ImmutableString<wchar_t>::blank(L"");
ImmutableString<char32_t> ImmutableString<char32_t>::blank(nullchar);

/////////////////
// String

String::String(const char* cstr)
{
	if (cstr == nullptr)
		m_stringObject = nullptr;
	else if (*cstr == 0)
		m_stringObject = &ImmutableString<char>::blank;
	else
		m_stringObject = new ImmutableString<char>(string_copy(cstr));

	if (m_stringObject)
	{
		m_stringObject->IncRef();
	}
}

String::String(const wchar_t* cstr)
{
	if (cstr == nullptr)
		m_stringObject = nullptr;
	else if (*cstr == 0)
		m_stringObject = &ImmutableString<wchar_t>::blank;
	else
		m_stringObject = new ImmutableString<wchar_t>(string_copy(cstr));

	if (m_stringObject)
	{
		m_stringObject->IncRef();
	}
}

int String::Compare(StringObject* str) const
{
	if (m_stringObject == str) return 0;
	if (m_stringObject == nullptr) return -1;
	if (str == nullptr) return 1;

	return m_stringObject->Compare(str);
}

int String::Compare(const char8* cstr) const
{
	if (m_stringObject == nullptr)
	{
		if (cstr == nullptr) return 0;
		else return -1;
	}
	else if (cstr == nullptr)
	{
		return 1;
	}

	return m_stringObject->Compare(cstr);
}

int String::Compare(const char16* cstr) const
{
	if (m_stringObject == nullptr)
	{
		if (cstr == nullptr) return 0;
		else return -1;
	}
	else if (cstr == nullptr)
	{
		return 1;
	}

	return m_stringObject->Compare(cstr);
}

int String::CompareNoCase(const char8* cstr) const
{
	return m_stringObject->CompareNoCase(cstr);
}

int String::CompareNoCase(const char16* cstr) const
{
	return m_stringObject->CompareNoCase(cstr);
}

int String::CompareNoCase(String str) const
{
	return m_stringObject->CompareNoCase(str);
}

int String::Compare(const char32_t* cstr) const
{
	ASSERT(0);
	return 0;
}

int String::CompareNoCase(const char32_t* cstr) const
{
	ASSERT(0);
	return 0;
}

String& String::operator = (const char* cstr)
{
	if (m_stringObject) m_stringObject->DecRef();

	if (*cstr == 0)
		m_stringObject = &ImmutableString<char>::blank;
	else
		m_stringObject = new ImmutableString<char>(string_copy(cstr));

	m_stringObject->IncRef();
	return *this;
}

String& String::operator = (const wchar_t* cstr)
{
	if (m_stringObject) m_stringObject->DecRef();

	if (*cstr == 0)
		m_stringObject = &ImmutableString<wchar_t>::blank;
	else
		m_stringObject = new ImmutableString<wchar_t>(string_copy(cstr));

	m_stringObject->IncRef();
	return *this;
}

String& String::operator = (const String& string)
{
	StringObject* stringObject = string.m_stringObject;

	if (stringObject)
	{
		stringObject->IncRef();

		if (m_stringObject)
		{
			m_stringObject->DecRef();
		}

		m_stringObject = stringObject;
	}
	else
	{
		if (m_stringObject)
		{
			m_stringObject->DecRef();
			m_stringObject = nullptr;
		}
	}

	return *this;
}

String& String::operator = (const StringBuilderBase& builder)
{
	if (m_stringObject) m_stringObject->DecRef();
	m_stringObject = builder.BuildString();
	m_stringObject->IncRef();
	return *this;
}

String& String::operator = (const string_data<char>& str)
{
	if (m_stringObject) m_stringObject->DecRef();
	m_stringObject = new ImmutableString<char>(string_copy(str.data, str.len));
	m_stringObject->IncRef();
	return *this;
}

String& String::operator = (const string_data<wchar_t>& str)
{
	if (m_stringObject) m_stringObject->DecRef();
	m_stringObject = new ImmutableString<wchar_t>(string_copy(str.data, str.len));
	m_stringObject->IncRef();
	return *this;
}

String& String::Insert(const char* cstr, size_t offset)
{
	if (m_stringObject)
	{
		StringObject* p = m_stringObject->Insert(cstr, offset);
		p->IncRef();
		m_stringObject->DecRef();
		m_stringObject = p;
	}
	else
	{
		if (offset != 0) raise(ArgumentOutOfRangeException());

		m_stringObject = new ImmutableString<char>(string_copy(cstr));
		m_stringObject->IncRef();
	}

	return *this;
}

String& String::Insert(const wchar_t* cstr, size_t offset)
{
	if (m_stringObject)
	{
		StringObject* p = m_stringObject->Insert(cstr, offset);
		p->IncRef();
		m_stringObject->DecRef();
		m_stringObject = p;
	}
	else
	{
		if (offset != 0) raise(ArgumentOutOfRangeException());

		m_stringObject = new ImmutableString<wchar_t>(string_copy(cstr));
		m_stringObject->IncRef();
	}

	return *this;
}

String& String::Insert(const String& str, size_t offset)
{
	if (m_stringObject == NULL)
	{
		if (offset != 0) raise(SystemException(L"String offset error"));
		*this = str;
	}
	else
	{
		StringObject* p = m_stringObject->Insert(str, offset);
		p->IncRef();
		m_stringObject->DecRef();
		m_stringObject = p;
	}

	return *this;
}

String& String::Append(const char* cstr, size_t len)
{
	if (m_stringObject == NULL)
	{
		m_stringObject = new MutableString<char>(cstr, len);
		m_stringObject->IncRef();
	}
	else
	{
		StringObject* p = m_stringObject->Append(cstr, len);
		if (p != m_stringObject)
		{
			p->IncRef();
			m_stringObject->DecRef();
			m_stringObject = p;
		}
	}
	return *this;
}

String& String::Append(const wchar_t* cstr, size_t len)
{
	if (m_stringObject == NULL)
	{
		m_stringObject = new MutableString<wchar_t>(cstr, len);
		m_stringObject->IncRef();
	}
	else
	{
		StringObject* p = m_stringObject->Append(cstr, len);
		if (p != m_stringObject)
		{
			p->IncRef();
			m_stringObject->DecRef();
			m_stringObject = p;
		}
	}
	return *this;
}

String& String::operator += (const String& str)
{
	if (m_stringObject)
	{
		if (str.m_stringObject)
		{
			StringObject* p = m_stringObject->Append(str.m_stringObject);
			if (p != m_stringObject)
			{
				p->IncRef();
				m_stringObject->DecRef();
				m_stringObject = p;
			}
		}
	}
	else
	{
		*this = str;
	}

	return *this;
}

String& String::operator += (const StringBuilderBase& builder)
{
	if (m_stringObject)
	{
		size_t totallen = GetLength() + builder.GetLength();

		bool wide = (m_stringObject->GetCharSize() == 2) || builder.m_wide;
		StringObject* stringObject;

		if (wide)
		{
			stringObject = new ImmutableString<WCHAR>(new WCHAR[totallen], totallen);
			stringObject->IncRef();
			WCHAR* pw = (WCHAR*)stringObject->GetData() + stringObject->GetLength();
			pw = builder.BuildString(pw);

			m_stringObject->CopyTo(pw, m_stringObject->GetLength());
		}
		else
		{
			stringObject = new ImmutableString<char>(new char[totallen], totallen);
			stringObject->IncRef();
			char* p = (char*)stringObject->GetData() + stringObject->GetLength();
			p = builder.BuildString(p);

			m_stringObject->CopyTo(p, m_stringObject->GetLength());
		}

		m_stringObject->DecRef();

		m_stringObject = stringObject;
	}
	else
	{
		m_stringObject = builder.BuildString();
		m_stringObject->IncRef();
	}

	return *this;
}

String& String::Delete(size_t offset, size_t len)
{
	if (len == 0) return *this;

	StringObject* p = m_stringObject->Delete(offset, len);
	p->IncRef();
	m_stringObject->DecRef();
	m_stringObject = p;

	return *this;
}

String String::FromRaw(const char* cstr)
{
	return FromRaw(cstr, cstrlen(cstr));
}

String String::FromRaw(const char* cstr, size_t len)
{
	return new ImmutableString<char>(cstr, len);
}

String String::FromRaw(const wchar_t* cstr)
{
	return FromRaw(cstr, cstrlen(cstr));
}

String String::FromRaw(const wchar_t* cstr, size_t len)
{
	return new ImmutableString<wchar_t>(cstr, len);
}

size_t String::CopyTo(char* cstrdst, size_t maxbufferchars, size_t offset) const
{
	if (m_stringObject)
	{
		return m_stringObject->CopyTo(cstrdst, maxbufferchars, offset);
	}
	else
	{
		return 0;
	}
}

size_t String::CopyTo(wchar_t* cstrdst, size_t maxbufferchars, size_t offset) const
{
	if (m_stringObject)
	{
		return m_stringObject->CopyTo(cstrdst, maxbufferchars, offset);
	}
	else
	{
		return 0;
	}
}

std::string String::ToStdString() const
{
	if (m_stringObject)
	{
		size_t len = m_stringObject->GetLength();
		std::string str(len, ' ');
		m_stringObject->CopyTo(const_cast<char*>(str.data()), len);
		return str;
	}
	else
		return std::string();
}

std::wstring String::ToStdWString() const
{
	if (m_stringObject)
	{
		size_t len = m_stringObject->GetLength();
		std::wstring str(len, ' ');
		m_stringObject->CopyTo(const_cast<wchar_t*>(str.data()), len);
		return str;
	}
	else
		return std::wstring();
}

/////////////////
// SubString

int String::_SubString::Compare(const char8* cstr) const
{
	return m_stringObject->SubCompare(m_offset, m_len, cstr, strlen(cstr));
}

int String::_SubString::Compare(const char16* cstr) const
{
	return m_stringObject->SubCompare(m_offset, m_len, cstr, wcslen(cstr));
}

int String::_SubString::CompareNoCase(const char8* cstr) const
{
	ASSERT(0);
	return 0;
}

int String::_SubString::CompareNoCase(const char16* cstr) const
{
	ASSERT(0);
	return 0;
}

int String::_SubString::Compare(const char32_t* cstr) const
{
	ASSERT(0);
	return 0;
}

int String::_SubString::CompareNoCase(const char32_t* cstr) const
{
	ASSERT(0);
	return 0;
}

StringBuilderTwo String::operator + (const String& other) const
{
	return StringBuilderTwo(other.m_stringObject, m_stringObject);
}

StringBuilderTwo String::operator + (const char* cstr) const
{
	return StringBuilderTwo(cstr, m_stringObject);
}

StringBuilderTwo String::operator + (const wchar_t* cstr) const
{
	return StringBuilderTwo(cstr, m_stringObject);
}

template<class T> StringObject* MutableString<T>::SubString(size_t offset, size_t len)
{
	return new ImmutableString<T>(string_copy(m_data.begin() + offset, len));
}

template<class T> size_t MutableString<T>::CopyTo(char* cstrdst, size_t maxbufferchars, size_t offset)
{
	size_t len = m_data.size() - offset;
	if (len > maxbufferchars) len = maxbufferchars;

	for (size_t i = 0; i < len; ++i)
	{
		cstrdst[i] = (char)m_data[offset+i];	// TODO
	}
	return len;
}

template<class T> size_t MutableString<T>::CopyTo(wchar_t* cstrdst, size_t maxbufferchars, size_t offset)
{
	size_t len = m_data.size() - offset;
	if (len > maxbufferchars) len = maxbufferchars;

	for (uint i = 0; i < len; ++i)
	{
		cstrdst[i] = m_data[offset+i];
	}
	return len;
}

template<class T> size_t ImmutableString<T>::CopyTo(char* cstrdst, size_t maxbufferchars, size_t offset)
{
	size_t len = m_data.size() - offset;
	if (len > maxbufferchars) len = maxbufferchars;

	for (size_t i = 0; i < len; ++i)
	{
		cstrdst[i] = (char)m_data[offset+i];	// TODO
	}
	return len;
}

template<class T> size_t ImmutableString<T>::CopyTo(wchar_t* cstrdst, size_t maxbufferchars, size_t offset)
{
	size_t len = m_data.size();

	if (len > maxbufferchars) len = maxbufferchars;

	for (size_t i = 0; i < len; i++)
	{
		cstrdst[i] = m_data[offset+i];
	}
	return len;
}

template<class T>
void ImmutableString<T>::Write(IO::TextWriter& stream)
{
	stream.WriteChars(m_data.begin(), m_data.size());
}

template<class T>
void MutableString<T>::Write(IO::TextWriter& stream)
{
	stream.WriteChars(m_data.begin(), m_data.size());
}

LFCEXT IO::TextWriter& operator << (IO::TextWriter& stream, const String& str)
{
	if (str.m_stringObject)
	{
		str.m_stringObject->Write(stream);
	}

	return stream;
}

LFCEXT IO::TextWriter& operator << (IO::TextWriter& stream, const StringVariant& str)
{
	if (str.m_stringObject)	str.m_stringObject->Write(stream);
	else if (str.m_cstr)		stream << str.m_cstr;
	else if (str.m_cstrw)	stream << str.m_cstrw;

	return stream;
}

IO::TextReader& ReadFromStream(IO::TextReader& stream, CString& cbuffer)
{
	size_t nchars = stream.ReadChars(cbuffer.buffer(), cbuffer.bufferlen()-1);
	cbuffer.m_p[nchars] = 0;

	return stream;
}

IO::TextReader& ReadFromStream(IO::TextReader& stream, String& str)
{
	wchar_t buffer[4096];
	size_t nchars = stream.ReadChars(buffer, _countof(buffer));
	str = new ImmutableString<wchar_t>(string_copy(buffer, nchars));

	return stream;
}

LFCEXT IO::TextReader& operator >> (IO::TextReader& stream, String& str)
{
	return ReadFromStream(stream, str);
}

IO::TextReader& ReadFromStream(IO::TextReader& stream, char& ch)
{
	stream.ReadChars(&ch, 1);
	return stream;
}

String StringObject::ToString()
{
	return this;
}

bool String::BeginsWith(StringIn str) const
{
	return LeftOf(str.GetLength()) == str;
}

bool String::EndsWith(StringIn str) const
{
	return RightOf(str.GetLength()) == str;
}

String String::ToNormalized(NormalizationForm form) const
{
	if (m_stringObject == nullptr)
	{
		return String();
	}

	switch (m_stringObject->GetCharSize())
	{
	case 1:
	case 4:
		{
			raise(Exception("not charsize 2"));
		}
		break;
	}

	ASSERT(0);
	return nullptr;
#if 0
	int dstlen = ::NormalizeString((NORM_FORM)form, (char16*)m_stringObject->GetData(), m_stringObject->GetLength31(), NULL, 0);
	wchar_t* dst = new wchar_t[dstlen];
	::NormalizeString((NORM_FORM)form, (char16*)m_stringObject->GetData(), m_stringObject->GetLength31(), dst, dstlen);

	return String::FromRaw(dst);
#endif
}

bool String::IsNormalized(NormalizationForm form) const
{
	if (m_stringObject == nullptr)
	{
		return true;
	}

	switch (m_stringObject->GetCharSize())
	{
	case 1:
	case 4:
		{
			raise(Exception("not charsize 2"));
		}
		break;
	}

	ASSERT(0);
	return false;
//	return ::IsNormalizedString((NORM_FORM)form, (char16*)m_stringObject->GetData(), m_stringObject->GetLength31()) != FALSE;
}

String String::ToUpperCase() const
{
	if (m_stringObject == nullptr) return nullptr;

	int len = m_stringObject->GetLength31();
	if (len == 0) return nullptr;

	switch (m_stringObject->GetCharSize())
	{
	case 1:
		{
			//CharUpperBuff(

			int dstlen = LCMapStringA(LOCALE_USER_DEFAULT, LCMAP_UPPERCASE,
				(const char*)m_stringObject->GetData(), len,
				nullptr, 0);

			char* dst = new char[dstlen];
			LCMapStringA(LOCALE_USER_DEFAULT, LCMAP_UPPERCASE,
				(const char*)m_stringObject->GetData(), len,
				dst, dstlen);

			return String::FromRaw(dst);
		}
		break;

	case 2:
		{
			int dstlen = LCMapStringW(LOCALE_USER_DEFAULT, LCMAP_UPPERCASE,
				(const wchar_t*)m_stringObject->GetData(), len,
				nullptr, 0);

			wchar_t* dst = new wchar_t[dstlen];
			LCMapStringW(LOCALE_USER_DEFAULT, LCMAP_UPPERCASE,
				(const wchar_t*)m_stringObject->GetData(), len,
				dst, dstlen);

			return String::FromRaw(dst);
		}
		break;

	case 4:
		{
			raise(Exception("not charsize 2"));
		}
		break;

	default:
		__assume(0);
	}

	ASSERT(0);
	return String();
#if 1
	int dstlen = LCMapStringEx(LOCALE_NAME_USER_DEFAULT, LCMAP_UPPERCASE, (char16*)m_stringObject->GetData(), m_stringObject->GetLength31(), NULL, 0, NULL, NULL, NULL);
	wchar_t* dst = new wchar_t[dstlen];

	LCMapStringEx(LOCALE_NAME_USER_DEFAULT, LCMAP_UPPERCASE, (char16*)m_stringObject->GetData(), m_stringObject->GetLength31(), dst, dstlen, NULL, NULL, NULL);

	return String::FromRaw(dst);
#endif
}

String String::ToLowerCase() const
{
	if (m_stringObject == nullptr) return nullptr;

	int len = m_stringObject->GetLength31();
	if (len == 0) return nullptr;

	switch (m_stringObject->GetCharSize())
	{
	case 1:
		{
			//CharUpperBuff(

			int dstlen = LCMapStringA(LOCALE_USER_DEFAULT, LCMAP_UPPERCASE,
				(const char*)m_stringObject->GetData(), len,
				nullptr, 0);

			char* dst = new char[dstlen];
			LCMapStringA(LOCALE_USER_DEFAULT, LCMAP_UPPERCASE,
				(const char*)m_stringObject->GetData(), len,
				dst, dstlen);

			return String::FromRaw(dst);
		}
		break;

	case 2:
		{
			int dstlen = LCMapStringW(LOCALE_USER_DEFAULT, LCMAP_UPPERCASE,
				(const wchar_t*)m_stringObject->GetData(), len,
				nullptr, 0);

			wchar_t* dst = new wchar_t[dstlen];
			LCMapStringW(LOCALE_USER_DEFAULT, LCMAP_UPPERCASE,
				(const wchar_t*)m_stringObject->GetData(), len,
				dst, dstlen);

			return String::FromRaw(dst);
		}
		break;

	case 4:
		{
			raise(Exception("not charsize 2"));
		}
		break;

	default:
		__assume(0);
	}

	ASSERT(0);
	return String();
}

int String::ToLowerCase(WCHAR* dst, size_t dstlen) const
{
	return -1;
/*
	if (m_stringObject == nullptr) return nullptr;

	int len = m_stringObject->GetLength31();
	if (len == 0) return nullptr;

	switch (m_stringObject->GetCharSize())
	{
	case 1:
		{
			//CharUpperBuff(

			int dstlen = LCMapStringA(LOCALE_USER_DEFAULT, LCMAP_UPPERCASE,
				(const char*)m_stringObject->GetData(), len,
				nullptr, 0);

			return LCMapStringA(LOCALE_USER_DEFAULT, LCMAP_UPPERCASE,
				(const char*)m_stringObject->GetData(), len,
				dst, dstlen);
		}
		break;

	case 2:
		{
			int dstlen = LCMapStringW(LOCALE_USER_DEFAULT, LCMAP_UPPERCASE,
				(const wchar_t*)m_stringObject->GetData(), len,
				nullptr, 0);

			wchar_t* dst = new wchar_t[dstlen];
			LCMapStringW(LOCALE_USER_DEFAULT, LCMAP_UPPERCASE,
				(const wchar_t*)m_stringObject->GetData(), len,
				dst, dstlen);

			return String::FromRaw(dst);
		}
		break;

	case 4:
		{
			raise(Exception("not charsize 2"));
		}
		break;

	default:
		__assume(0);
	}

	ASSERT(0);
	return String();
	*/
}

bool String::IsLowerCase() const
{
	return ToLowerCase() == *this;
}

bool String::IsUpperCase() const
{
	return ToUpperCase() == *this;
}

ptrdiff_t String::const_iterator_base::operator - (const const_iterator_base& it) const
{
	ASSERT(m_p == it.m_p);
//	if (m_p != it.m_p) raise(SystemException("Not pointing to same string"));

	return m_offset - it.m_offset;
}

// pointer comparison
bool String::const_iterator_base::operator > (const const_iterator_base& it) const
{
	ASSERT(m_p == it.m_p);
//	if (m_p != it.m_p) raise(SystemException("Not pointing to same string"));

	return m_offset > it.m_offset;
}

bool String::const_iterator_base::operator < (const const_iterator_base& it) const
{
	ASSERT(m_p == it.m_p);
//	if (m_p != it.m_p) raise(SystemException("Not pointing to same string"));

	return m_offset < it.m_offset;
}

bool String::const_iterator_base::operator >= (const const_iterator_base& it) const
{
	ASSERT(m_p == it.m_p);
//	if (m_p != it.m_p) raise(SystemException("Not pointing to same string"));

	return m_offset >= it.m_offset;
}

bool String::const_iterator_base::operator <= (const const_iterator_base& it) const
{
	ASSERT(m_p == it.m_p);
//	if (m_p != it.m_p) raise(SystemException("Not pointing to same string"));

	return m_offset <= it.m_offset;
}

bool String::const_iterator_base::atend() const
{
	if (m_p == nullptr)
	{
		if (m_offset != 0) raise(SystemException(L"referencing null pointer"));
		return true;
	}
	else
	{
		//if (m_offset < 0) throw new Exception(L"offset < 0");
		if ((unsigned int )m_offset > m_p->GetLength()) raise(SystemException(L"offset < 0 or >= length"));

		return m_offset == m_p->GetLength();
	}
}

char* WriteNumber(char* dst, unsigned int num, uint base);

char* WriteNumber(char* dst, int num, uint base)
{
	if (num < 0)
	{
		*dst++ = '-';
		num = -num;
	}

	return WriteNumber(dst, (unsigned int)num, base);
}

char* WriteNumber(char* dst, unsigned long num, uint base);

char* WriteNumber(char* dst, long num, uint base)
{
	if (num < 0)
	{
		*dst++ = '-';
		num = -num;
	}

	return WriteNumber(dst, (unsigned long)num, base);
}

char* WriteNumber(char* dst, uint64 num, uint base);

char* WriteNumber(char* dst, int64 num, uint base)
{
	if (base == 10 && num < 0)
	{
		*dst++ = '-';
		num = -num;
	}

	return WriteNumber(dst, (uint64)num, base);
}

/*
void WriteNumberAndNull(char* dst, char num, uint base)
{
	char* p = WriteNumber(dst, (int)num, base);
	*p = 0;
}
*/

void WriteNumberAndNull(char* dst, signed char num, uint base)
{
	if (base == 10 && num < 0)
	{
		*dst++ = '-';
		num = -num;
	}

	char* p = WriteNumber(dst, (int)num, base);
	*p = 0;
}

void WriteNumberAndNull(char* dst, unsigned char num, uint base)
{
	char* p = WriteNumber(dst, (unsigned int)num, base);
	*p = 0;
}

void WriteNumberAndNull(char* dst, int num, uint base)
{
	if (base == 10 && num < 0)
	{
		*dst++ = '-';
		num = -num;
	}

	char* p = WriteNumber(dst, num, base);
	*p = 0;
}

void WriteNumberAndNull(char* dst, unsigned int num, uint base)
{
	char* p = WriteNumber(dst, num, base);
	*p = 0;
}

void WriteNumberAndNull(char* dst, long num, uint base)
{
	if (base == 10 && num < 0)
	{
		*dst++ = '-';
		num = -num;
	}

	char* p = WriteNumber(dst, num, base);
	*p = 0;
}

void WriteNumberAndNull(char* dst, unsigned long num, uint base)
{
	char* p = WriteNumber(dst, num, base);
	*p = 0;
}

void WriteNumberAndNull(char* dst, int64 num, uint base)
{
	if (base == 10 && num < 0)
	{
		*dst++ = '-';
		num = -num;
	}

	char* p = WriteNumber(dst, num, base);
	*p = 0;
}

void WriteNumberAndNull(char* dst, uint64 num, uint base)
{
	char* p = WriteNumber(dst, num, base);
	*p = 0;
}

// static
String String::FromNumber(signed char value, int base)
{
	char buffer[8+2];
	WriteNumberAndNull(buffer, value, base);
	return buffer;
}

// static
String String::FromNumber(unsigned char value, int base)
{
	/*
	static ImmutableString<char> str[256];

	if (base == 10)
	{
		if (str[value].m_data == NULL)
		{
			str[value] = 
		}
	}
	*/

	char buffer[8+2];
	WriteNumberAndNull(buffer, value, base);
	return buffer;
}

// static
String String::FromNumber(short value, int base)
{
	char buffer[16+2];
	WriteNumberAndNull(buffer, value, base);
	return buffer;
}

// static
String String::FromNumber(unsigned short value, int base)
{
	char buffer[16+2];
	WriteNumberAndNull(buffer, value, base);
	return buffer;
}

// static
String String::FromNumber(int value, int base)
{
	char buffer[32+2];
	WriteNumberAndNull(buffer, value, base);
	return buffer;
}

// static
String String::FromNumber(unsigned int value, int base)
{
	char buffer[32+2];
	WriteNumberAndNull(buffer, value, base);
	return buffer;
}

// static
String String::FromNumber(long value, int base)
{
	char buffer[64+2];
	WriteNumberAndNull(buffer, value, base);
	return buffer;
}

// static
String String::FromNumber(unsigned long value, int base)
{
	char buffer[64+2];
	WriteNumberAndNull(buffer, value, base);
	return buffer;
}

// static
String String::FromNumber(int64 value, int base)
{
	char buffer[64+2];
	WriteNumberAndNull(buffer, value, base);
	return buffer;
}

// static
String String::FromNumber(uint64 value, int base)
{
	char buffer[64+2];
	WriteNumberAndNull(buffer, value, base);
	return buffer;
}

// static
String String::FromNumber(float value)
{
	IO::StringWriter str;

	IO::TextWriter& str2 = (IO::TextWriter&)str;

	str2 << value;
	return str;
}

// static
String String::FromNumber(double value)
{
	IO::StringWriter str;
	str << value;
	return str;
}

// static
String String::FromNumber(long double value)
{
	IO::StringWriter str;
	str << value;
	return str;
}

// explicit instantiation

template class LFCEXT ImmutableString<char8>;
template class LFCEXT ImmutableString<char16>;
template class LFCEXT ImmutableString<char32_t>;

template class LFCEXT MutableString<char8>;
template class LFCEXT MutableString<char16>;
//template class MutableString<char32_t>;

}	// System
