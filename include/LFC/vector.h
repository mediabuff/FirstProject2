#ifndef System_vector_h
#define System_vector_h

#ifndef ASSERT_VALID
#define ASSERT_VALID(x)
#endif

#undef new

namespace System
{

class ArrayBaseData
{
public:
	void* m_pData;
	size_t m_nSize;
};

template<class TYPE> class array
{
protected:

	TYPE* m_pData;
	size_t m_nSize;

public:

	typedef TYPE* iterator;
	typedef const TYPE* const_iterator;

	class const_reverse_iterator
	{
	public:

		CTOR const_reverse_iterator(TYPE* p) : m_p(p)
		{
		}

		const_reverse_iterator& operator ++ ()
		{
			--m_p;
			return *this;
		}

		const_reverse_iterator& operator -- ()
		{
			++m_p;
			return *this;
		}

		const_reverse_iterator operator ++ (int)
		{
			return const_reverse_iterator(m_p--);
		}

		const_reverse_iterator operator -- (int)
		{
			return const_reverse_iterator(m_p++);
		}

		const TYPE& operator * () const
		{
			return m_p[-1];
		}

		const TYPE& operator [] (ptrdiff_t index) const
		{
			return m_p[-index-1];
		}

		bool operator < (const const_reverse_iterator& other) const
		{
			return m_p > other.m_p;
		}

		bool operator > (const const_reverse_iterator& other) const
		{
			return m_p < other.m_p;
		}

		bool operator == (const const_reverse_iterator& other) const
		{
			return m_p == other.m_p;
		}

		bool operator != (const const_reverse_iterator& other) const
		{
			return m_p != other.m_p;
		}

	protected:

		TYPE* m_p;
	};

	class reverse_iterator : public const_reverse_iterator
	{
	public:

		CTOR reverse_iterator(TYPE* p) : const_reverse_iterator(p)
		{
		}

		reverse_iterator& operator ++ ()
		{
			--m_p;
			return *this;
		}

		reverse_iterator& operator -- ()
		{
			++m_p;
			return *this;
		}

		reverse_iterator operator ++ (int)
		{
			return reverse_iterator(m_p--);
		}

		reverse_iterator& operator -- (int)
		{
			return reverse_iterator(m_p++);
		}

		TYPE& operator * ()
		{
			return m_p[-1];
		}

		TYPE& operator [] (ptrdiff_t index)
		{
			return m_p[index-1];
		}
	};

	CTOR array() : m_pData(nullptr), m_nSize(0)
	{
	}

#ifndef __LERSTAD__
	template<size_t size> CTOR array(TYPE (&pData)[size]) : m_pData(pData), m_nSize(size)
	{
	}
#endif

	CTOR array(TYPE* pData, size_t nSize) : m_pData(pData), m_nSize(nSize)
	{
	}

	void assign(TYPE* pData, size_t nSize)
	{
		m_pData = pData;
		m_nSize = nSize;
	}

	inline size_t size() const
	{
		return m_nSize;
	}
	
	inline iterator begin()
	{
		return m_pData;
	}

	iterator end()
	{
		return m_pData + m_nSize;
	}

	reverse_iterator rbegin()
	{
		return reverse_iterator(m_pData + m_nSize);
	}

	reverse_iterator rend()
	{
		return reverse_iterator(m_pData);
	}

	inline const_iterator begin() const
	{
		return m_pData;
	}

	const_iterator end() const
	{
		return m_pData + m_nSize;
	}

	inline const_iterator cbegin()
	{
		return m_pData;
	}

	const_iterator cend()
	{
		return m_pData + m_nSize;
	}

	inline TYPE& operator [] (size_t index)
	{
		ASSERT(index < m_nSize);
		return m_pData[index];
	}

	inline const TYPE& operator [] (size_t index) const
	{
		ASSERT(index < m_nSize);
		return m_pData[index];
	}
};

template<class TYPE>
inline void DestructElements(TYPE* pElements, size_t nCount)
{
//	ASSERT(nCount == 0 ||
//		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));

	// call the destructor(s)
	for (; nCount--; pElements++)
		pElements->~TYPE();
}

template<class TYPE, class A = __gc_allocator>
class vector
{
protected:
	TYPE* m_pData;
	size_t m_nSize;     // number of elements
	size_t m_nMaxSize;  // allocated number of elements
	A m_allocator;

public:

	typedef const TYPE& ARG_TYPE;
	typedef size_t size_type;
	typedef TYPE* iterator;

	class const_reverse_iterator
	{
	public:

		explicit CTOR const_reverse_iterator(TYPE* p) : m_p(p)
		{
		}

		const_reverse_iterator& operator ++ ()
		{
			--m_p;
			return *this;
		}

		const_reverse_iterator& operator -- ()
		{
			++m_p;
			return *this;
		}

		const_reverse_iterator operator ++ (int)
		{
			return const_reverse_iterator(m_p--);
		}

		const_reverse_iterator operator -- (int)
		{
			return const_reverse_iterator(m_p++);
		}

		const TYPE& operator * () const
		{
			return m_p[-1];
		}

		const TYPE& operator [] (ptrdiff_t index) const
		{
			return m_p[index-1];
		}

	protected:

		TYPE* m_p;
	};

	class reverse_iterator : public const_reverse_iterator
	{
	public:

		explicit CTOR reverse_iterator(TYPE* p) : const_reverse_iterator(p)
		{
		}

		reverse_iterator& operator ++ ()
		{
			--m_p;
			return *this;
		}

		reverse_iterator& operator -- ()
		{
			++m_p;
			return *this;
		}

		reverse_iterator operator ++ (int)
		{
			return reverse_iterator(m_p-1);
		}

		reverse_iterator operator -- (int)
		{
			return reverse_iterator(m_p+1);
		}

		TYPE& operator * ()
		{
			return m_p[-1];
		}

		TYPE& operator [] (ptrdiff_t index)
		{
			return m_p[index-1];
		}
	};

// Construction
	CTOR vector();
	explicit CTOR vector(A& al);
	CTOR vector(const vector& other);	// copy constructor

	CTOR vector(vector&& other)	// move constructor
	{
		m_pData = other.m_pData;
		m_nSize = other.m_nSize;
		m_nMaxSize = other.m_nMaxSize;

		other.m_pData = nullptr;	// reset other
	}

	CTOR vector(ARG_TYPE item0) : m_pData(nullptr), m_nSize(0), m_nMaxSize(0)
	{
		push_back(item0);
	}

	CTOR vector(ARG_TYPE item0, ARG_TYPE item1) : m_pData(nullptr), m_nSize(0), m_nMaxSize(0)
	{
		push_back(item0);
		push_back(item1);
	}

	CTOR vector(ARG_TYPE item0, ARG_TYPE item1, ARG_TYPE item2) : m_pData(nullptr), m_nSize(0), m_nMaxSize(0)
	{
		push_back(item0);
		push_back(item1);
		push_back(item2);
	}

	CTOR vector(ARG_TYPE item0, ARG_TYPE item1, ARG_TYPE item2, ARG_TYPE item3) : m_pData(NULL), m_nSize(0), m_nMaxSize(0)
	{
		push_back(item0);
		push_back(item1);
		push_back(item2);
		push_back(item3);
	}

	~vector();

	inline size_type GetSize() const
	{
		return m_nSize;
	}

	inline size_type size() const
	{
		return m_nSize;
	}

	inline size_type capacity() const
	{
		return m_nMaxSize;
	}

	size_type max_size() const
	{
		return SIZE_MAX / sizeof(TYPE);
	}

	template<class T2> TYPE* find(T2 v)
	{
		TYPE* end_ = m_pData + size();
		TYPE* it = m_pData;
		while (it != end_)
		{
			if (v == *it) break;
			++it;
		}
		return it;
	}

	template<class T2> const TYPE* find(T2 v) const
	{
		const TYPE* end_ = m_pData + size();
		const TYPE* it = m_pData;
		while (it != end_)
		{
			if (v == *it) break;
			++it;
		}
		return it;
	}

	template<class T2> reverse_iterator rfind(T2 v)
	{
		TYPE* end_ = m_pData;
		TYPE* it = m_pData + size();

		while (it != end)
		{
			--it;
			if (v == *it) break;
		}
		return it;
	}

// Attributes
	//int GetSize() const;
	//int GetUpperBound() const;
	void SetSize(size_type nNewSize, ARG_TYPE T = TYPE());

	void resize(size_type nNewSize, ARG_TYPE T = TYPE())
	{
		SetSize(nNewSize, T);
	}

	void reserve(size_type nSize);

// Operations
	// Clean up
//	void FreeExtra();
	void RemoveAll();

	// Accessing elements

	TYPE& at(size_type nIndex)
	{
		ASSERT(nIndex < m_nSize);
		return m_pData[nIndex];
	}

	const TYPE& at(size_type nIndex) const
	{
		ASSERT(nIndex < m_nSize);
		return m_pData[nIndex];
	}

	TYPE GetAt(size_type nIndex) const;
	void SetAt(size_type nIndex, ARG_TYPE newElement);
	TYPE& ElementAt(size_type nIndex);
	const TYPE& ElementAt(size_type nIndex) const;

	// Direct Access to the element data (may return nullptr)
	TYPE* GetData();
	const TYPE* GetData() const;

	size_t Add(ARG_TYPE newElement);

	void push_back(const TYPE& newElement)
	{
		size_t nIndex = m_nSize;
		alloc_add(1);
		// copy constructor
		::new (&((TYPE*)m_pData)[nIndex]) TYPE(newElement);
	}

	void push_back(TYPE&& newElement)
	{
		size_t nIndex = m_nSize;
		alloc_add(1);
		// move constructor
		::new (&((TYPE*)m_pData)[nIndex]) TYPE(std::move(newElement));
	}

	// overloaded operator helpers
	TYPE operator [] (size_type nIndex) const;
	TYPE& operator [] (size_type nIndex);

	vector& operator = (const vector& other);

	vector& operator = (vector&& other) throw()
	{
		m_nSize = other.m_nSize;
		m_pData = other.m_pData;

		other.m_nSize = 0;
		other.m_pData = nullptr;

		return *this;
	}

	TYPE* begin() throw()
	{
		return m_pData;
	}

	const TYPE* begin() const throw()
	{
		return (const TYPE*)m_pData;
	}

	TYPE* end() throw()
	{
		return m_pData + m_nSize;
	}

	const TYPE* end() const throw()
	{
		return m_pData + m_nSize;
	}

	const TYPE* cbegin() const throw()
	{
		return m_pData;
	}

	const TYPE* cend() const throw()
	{
		return m_pData + m_nSize;
	}

	reverse_iterator rend() const
	{
		return reverse_iterator(m_pData + m_nSize);
	}

	const_reverse_iterator crend() const
	{
		return const_reverse_iterator(m_pData + m_nSize);
	}

	void clear()
	{
		RemoveAll();
	}

	TYPE& back()
	{
		ASSERT(m_nSize > 0);
		return m_pData[m_nSize-1];
	}

	const TYPE& back() const
	{
		ASSERT(m_nSize > 0);
		return m_pData[m_nSize-1];
	}

	// Operations that move elements around
	void InsertAt(size_t nIndex, ARG_TYPE newElement, size_t nCount = 1);
	void RemoveAt(size_t nIndex, size_t nCount = 1);
//	void InsertAt(size_t nStartIndex, vector* pNewArray);

	void insert(TYPE* it, const TYPE* first, const TYPE* last)
	{
		ptrdiff_t startIndex = it - begin();

		ptrdiff_t nCount = last - first;
		ASSERT(nCount >= 0);
	//	alloc_add((size_t)size);

		size_t nNewSize = m_nSize + nCount;

		if (nNewSize > m_nMaxSize)
		{
			size_t nNewMax = 4;
			while (nNewMax < nNewSize) nNewMax <<= 1;

			TYPE* pNewData = (TYPE*)m_allocator.try_resize_buffer(m_pData, nNewMax * sizeof(TYPE));
			if (pNewData == NULL)
			{
				TYPE* pNewData = (TYPE*)m_allocator.allocate_buffer(nNewMax * sizeof(TYPE));

				memcpy(pNewData, m_pData, startIndex * sizeof(TYPE));
				memcpy(pNewData + startIndex + nCount, m_pData + startIndex, sizeof(TYPE)*(m_nSize - startIndex));

				m_pData = pNewData;
			}

			m_nMaxSize = nNewMax;
		}
		else
		{
			memmove(m_pData + startIndex + nCount, m_pData + startIndex, sizeof(TYPE)*(m_nSize - startIndex));
		}
		m_nSize = nNewSize;

		it = begin() + startIndex;

#ifndef __LERSTAD__
		for (auto from = first; from != last; ++from)
		{
		// copy constructor
			::new (it) TYPE(*from);
			++it;
		}
#endif
	}

	void insert(TYPE* before_position, const TYPE& val)
	{
		insert(before_position, &val, &val+1);
	}

	TYPE* erase(TYPE* it)
	{
		if (it == end()) return end();

		ptrdiff_t index = it - begin();
		RemoveAt(index);
		return begin() + index;
	}

	TYPE* erase(TYPE* first, TYPE* last)
	{
		ptrdiff_t count = last - first;
		ASSERT(count >= 0);
		ptrdiff_t index = first - begin();
		RemoveAt(index, count);
		return begin() + index;
	}

	TYPE* Detach()
	{
		TYPE* pData = m_pData;
		m_pData = NULL;
		return pData;
	}

protected:

	void alloc_reserve(size_type size)
	{
		if (size > m_nMaxSize)
		{
			size_t nNewMax = size;

			TYPE* pNewData = (TYPE*)m_allocator.try_resize_buffer(m_pData, nNewMax * sizeof(TYPE));
			if (pNewData == NULL)
			{
				TYPE* pNewData = (TYPE*)m_allocator.allocate_buffer(nNewMax * sizeof(TYPE));

				// copy new data from old
				memcpy(pNewData, m_pData, m_nSize * sizeof(TYPE));

				// get rid of old stuff (note: no destructors called)
				m_allocator.free_buffer(m_pData);

				m_pData = pNewData;
			}

			m_nMaxSize = nNewMax;
		}
	}

	void alloc_add(size_t AddCount)
	{
		size_t nIndex = m_nSize;
		size_t nNewSize = m_nSize + AddCount;
		if (nNewSize > m_nMaxSize)
		{
			size_t nNewMax = 4;
			while (nNewMax < nNewSize) nNewMax <<= 1;

			ASSERT(nNewMax >= nNewSize);

			TYPE* pNewData = (TYPE*)m_allocator.try_resize_buffer(m_pData, nNewMax * sizeof(TYPE));
			if (pNewData == NULL)
			{
				pNewData = (TYPE*)m_allocator.allocate_buffer(nNewMax * sizeof(TYPE));

				// copy new data from old
				memcpy(pNewData, m_pData, m_nSize * sizeof(TYPE));

				// get rid of old stuff (note: no destructors called)
				m_allocator.free_buffer(m_pData);
				m_pData = pNewData;
			}

			m_nMaxSize = nNewMax;
		}

		m_nSize = nNewSize;
	}
};

/////////////////////////////////////////////////////////////////////////////
// vector<TYPE, ARG_TYPE> inline functions

template<class TYPE, class A>
inline TYPE vector<TYPE,A>::GetAt(size_type nIndex) const
	{ ASSERT(/*nIndex >= 0 &&*/ nIndex < m_nSize);
		return ((TYPE*)m_pData)[nIndex]; }
template<class TYPE, class A>
inline void vector<TYPE,A>::SetAt(size_type nIndex, ARG_TYPE newElement)
	{ ASSERT(/*nIndex >= 0 &&*/ nIndex < m_nSize);
		((TYPE*)m_pData)[nIndex] = newElement; }

template<class TYPE, class A>
inline TYPE& vector<TYPE,A>::ElementAt(size_type nIndex)
{
	ASSERT(/*nIndex >= 0 &&*/ nIndex < m_nSize);
	return ((TYPE*)m_pData)[nIndex];
}

template<class TYPE, class A>
inline const TYPE& vector<TYPE,A>::ElementAt(size_type nIndex) const
{
	ASSERT(/*nIndex >= 0 &&*/ nIndex < m_nSize);
	return ((TYPE*)m_pData)[nIndex];
}

template<class TYPE, class A>
inline const TYPE* vector<TYPE,A>::GetData() const
	{ return (const TYPE*)m_pData; }
template<class TYPE, class A>
inline TYPE* vector<TYPE,A>::GetData()
	{ return (TYPE*)m_pData; }

template<class TYPE, class A>
inline TYPE vector<TYPE,A>::operator[](size_type nIndex) const
	{ return GetAt(nIndex); }
template<class TYPE, class A>
inline TYPE& vector<TYPE,A>::operator[](size_type nIndex)
	{ return ElementAt(nIndex); }

/////////////////////////////////////////////////////////////////////////////
// vector<TYPE, ARG_TYPE> out-of-line functions

template<class TYPE, class A>
inline vector<TYPE,A>::vector() :
	m_pData(NULL),
	m_nSize(0),
	m_nMaxSize(0)
{
}

template<class TYPE, class A>
vector<TYPE, A>::vector(A & al) :
	m_allocator(al),
	m_pData(NULL),
	m_nSize(0),
	m_nMaxSize(0)
{
}

template<class TYPE, class A>
vector<TYPE, A>::vector(const vector<TYPE,A>& other)
{
	m_nSize = m_nMaxSize = other.m_nSize;
	m_pData = (TYPE*)m_allocator.allocate_buffer(m_nSize * sizeof(TYPE));
	for (size_t i = 0; i < m_nSize; i++)
	{
		// copy constructor
		::new (&((TYPE*)m_pData)[i]) TYPE(((TYPE*)other.m_pData)[i]);
	}
}

template<class TYPE, class A>
vector<TYPE, A>::~vector()
{
	//ASSERT_VALID(this);

	if (m_pData != NULL)
	{
		DestructElements((TYPE*)m_pData, m_nSize);
	//	delete[] (uint8*)m_pData;
		m_allocator.free_buffer(m_pData);
	}
}

template<class TYPE, class A>
vector<TYPE,A>& vector<TYPE, A>::operator = (const vector<TYPE, A>& other)
{
	RemoveAll();

	m_nSize = m_nMaxSize = other.m_nSize;
	if (m_nSize)
	{
		m_pData = (TYPE*)m_allocator.allocate_buffer(m_nSize * sizeof(TYPE));
		for (size_t i = 0; i < m_nSize; i++)
		{
			// copy constructor
			::new (&((TYPE*)m_pData)[i]) TYPE(((TYPE*)other.m_pData)[i]);
		}
	}

	return *this;
}

template<class TYPE, class A>
void vector<TYPE,A>::RemoveAll()
{
	// shrink to nothing
	if (m_pData != NULL)
	{
		DestructElements /*<TYPE>*/ ((TYPE*)m_pData, m_nSize);
		//delete[] (uint8*)m_pData;
		m_allocator.free_buffer(m_pData);
		m_pData = NULL;
	}
	m_nSize = m_nMaxSize = 0;

//	SetSize(0);
}

template<class TYPE, class A>
size_t vector<TYPE,A>::Add(ARG_TYPE newElement)
{
	size_t nIndex = m_nSize;

	alloc_add(1);

	// copy constructor
	::new (&((TYPE*)m_pData)[nIndex]) TYPE(newElement);

	return nIndex;
}

template<class TYPE, class A>
void vector<TYPE,A>::reserve(size_type count)
{
	alloc_reserve(count);
}

template<class TYPE, class A>
void vector<TYPE,A>::SetSize(size_type nNewSize, ARG_TYPE newElement)
{
	if (nNewSize == 0)
	{
		if (m_pData != NULL)
		{
			DestructElements/*<TYPE>*/((TYPE*)m_pData, m_nSize);
		//	delete[] (uint8*)m_pData;
		//	m_pData = NULL;
		}

		m_nSize = 0;
	}
	else if (m_pData == NULL)
	{
		// create one with exact size
#ifdef SIZE_T_MAX
		ASSERT(nNewSize <= SIZE_T_MAX/sizeof(TYPE));    // no overflow
#endif
		ASSERT(nNewSize);
		m_pData = (TYPE*)m_allocator.allocate_buffer(nNewSize * sizeof(TYPE));
		//ConstructElements<TYPE>(m_pData, nNewSize);
		/*
		for (int i = 0; i < nNewSize; i++)
		{
			m_pData[m_nSize] = T;
		}
		*/

		for (size_t i = 0; i < nNewSize; ++i)
		{
			::new (&((TYPE*)m_pData)[i]) TYPE(newElement);	// copy constructor
		}

		m_nSize = m_nMaxSize = nNewSize;
	}
	else if (nNewSize <= m_nMaxSize)
	{
		// it fits
		if (nNewSize > m_nSize)
		{
			// initialize the new elements
			//ConstructElements<TYPE>(&m_pData[m_nSize], nNewSize-m_nSize);
			for (size_t i = m_nSize; i < nNewSize; ++i)
			{
				::new (&((TYPE*)m_pData)[i]) TYPE(newElement);	// copy constructor
			}
		}
		else if (m_nSize > nNewSize)
		{
			// destroy the old elements
			DestructElements/*<TYPE>*/(&((TYPE*)m_pData)[nNewSize], m_nSize-nNewSize);
		}
		m_nSize = nNewSize;
	}
	else
	{
		/*
		// otherwise, grow array
		int nGrowBy = m_nGrowBy;
		if (nGrowBy == 0)
		*/
		size_t nGrowBy;
		{
			// heuristically determine growth when nGrowBy == 0
			//  (this avoids heap fragmentation in many situations)
			nGrowBy = m_nSize / 8;
			nGrowBy = (nGrowBy < 4) ? 4 : ((nGrowBy > 1024) ? 1024 : nGrowBy);
		}
		size_t nNewMax;
		if (nNewSize < m_nMaxSize + nGrowBy)
			nNewMax = m_nMaxSize + nGrowBy;  // granularity
		else
			nNewMax = nNewSize;  // no slush

		ASSERT(nNewMax >= m_nMaxSize);  // no wrap around
#ifdef SIZE_T_MAX
		ASSERT(nNewMax <= SIZE_T_MAX/sizeof(TYPE)); // no overflow
#endif
		ASSERT(nNewMax);
		TYPE* pNewData = (TYPE*)m_allocator.allocate_buffer(nNewMax * sizeof(TYPE));

		// copy new data from old
		memcpy(pNewData, m_pData, m_nSize * sizeof(TYPE));

		// construct remaining elements
		ASSERT(nNewSize > m_nSize);
	//	ConstructElements<TYPE>(&pNewData[m_nSize], nNewSize-m_nSize);
		for (size_t i = m_nSize; i < nNewSize; ++i)
		{
			::new (&pNewData[i]) TYPE(newElement);	// copy constructor
		}

		// get rid of old stuff (note: no destructors called)
		//delete[] (uint8*)m_pData;
		m_allocator.free_buffer(m_pData);
		m_pData = pNewData;
		m_nSize = nNewSize;
		m_nMaxSize = nNewMax;
	}
}

template<class TYPE, class A>
void vector<TYPE,A>::InsertAt(size_t nIndex, ARG_TYPE newElement, size_t nCount /*=1*/)
{
	size_t nNewSize = m_nSize + nCount;

	if (nNewSize > m_nMaxSize)
	{
		size_t nNewMax = 4;
		while (nNewMax < nNewSize) nNewMax <<= 1;

		TYPE* pNewData = (TYPE*)m_allocator.try_resize_buffer(m_pData, nNewMax * sizeof(TYPE));
		if (pNewData == NULL)
		{
			TYPE* pNewData = (TYPE*)m_allocator.allocate_buffer(nNewMax * sizeof(TYPE));

			memcpy(pNewData, m_pData, nIndex * sizeof(TYPE));
			memcpy(&pNewData[nIndex + nCount], &((TYPE*)m_pData)[nIndex], sizeof(TYPE)*(m_nSize - nIndex));

			m_pData = pNewData;
		}

		m_nMaxSize = nNewMax;
	}
	else
	{
		memmove(&((TYPE*)m_pData)[nIndex+nCount], &((TYPE*)m_pData)[nIndex], sizeof(TYPE)*(m_nSize - nIndex));
	}
	m_nSize = nNewSize;

	// insert new value in the gap
	ASSERT(nIndex + nCount <= m_nSize);
	while (nCount--)
	{
	// copy constructor
		::new (&((TYPE*)m_pData)[nIndex++]) TYPE(newElement);
	}
}

template<class TYPE, class A>
void vector<TYPE,A>::RemoveAt(size_t nIndex, size_t nCount)
{
	ASSERT(nIndex + nCount <= m_nSize);

	// just remove a range
	size_t nMoveCount = m_nSize - (nIndex + nCount);
	DestructElements/*<TYPE>*/(&((TYPE*)m_pData)[nIndex], nCount);
	if (nMoveCount)
	{
		memmove(&((TYPE*)m_pData)[nIndex], &((TYPE*)m_pData)[nIndex + nCount],
			nMoveCount * sizeof(TYPE));
	}
	m_nSize -= nCount;
}

#define new _new

}

#endif	// System_vector_h
