#include "stdafx.h"
#include "LFC.h"

namespace System
{

FunctionObject::FunctionObject(Declarator* decl) : m_decl(decl)
{
}

size_t FunctionObject::get_NumArgs()
{
	return m_decl->m_pType->AsFunction()->m_parameters.size();
}

String FunctionObject::ToString()
{
	return m_decl->ToString();
}

/*
Module* FunctionObject::GetModule()
{
	return Module::FromAddress((const void*)m_decl->m_offset);
}
*/

}	// System
