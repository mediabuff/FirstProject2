#include "stdafx.h"
#include "LFC.h"
#include "Debug.h"

namespace System
{

SourceFile::SourceFile()
{
//	startAddress = 0;
//	endAddress = 0;
	m_filename = nullptr;
	m_pParentFile = nullptr;
}

SourceFile::~SourceFile()
{
}

uint SourceFile::GetSegmentCount()
{
	return m_segments.size();
}

void SourceFile::GetSegmentAddress(uint n, ULONG_PTR* pStartAddress, ULONG_PTR* pEndAddress)
{
	ASSERT(n < m_segments.size());

	if (pStartAddress) *pStartAddress = m_segments[n].m_startAddress;
	if (pEndAddress) *pEndAddress = m_segments[n].m_endAddress;
}

DebugSymbol* DebugInfo::LookupSymbol(ULONG_PTR address)
{
	auto it = m_symbols.find(address);
	if (it != m_symbols.end())
	{
		return it->second;
	}
	return NULL;
}

DebugSymbol* DebugInfo::LookupSymbolByName(StringIn name)
{
	auto it = m_symbols_name.find(name);
	if (it != m_symbols_name.end())
	{
		return it->second;
	}
	return NULL;
}

}	// System
