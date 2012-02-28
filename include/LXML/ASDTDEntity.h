#ifndef __ASDTDENTITY_H_
#define __ASDTDENTITY_H_

#include "ASDTDObjectImpl.h"

namespace System
{
namespace Web
{

class CASDTDModel;

class WEBEXT CASDTDEntity :
	public CASDTDObject, public IASEntityDeclaration
{
public:
	CASDTDEntity()
	{
//		m_pDTDDocument = NULL;
	}

//	CASDTDModel* m_pDTDDocument;

	int m_c;
	String m_name;
	String m_value;

// IASObject
	String get_entityValue();

	IASModel* get_ownerASModel()
	{
		return m_ownerASModel;
	}
#if 0
	STDMETHOD(set_ownerASModel)(/*[in]*/ IASModel* newVal)
	{
		return E_FAIL;
	}
#endif
	IASObject* cloneASObject(bool deep) const
	{
		ASSERT(0);
		return nullptr;
	}

	String get_localName()
	{
		THROW(-1);
		return nullptr;
	}

	void set_localName(StringIn newVal)
	{
		THROW(-1);
	}

	String get_prefix()
	{
		THROW(-1);
		return nullptr;
	}

	void set_prefix(StringIn newVal)
	{
		THROW(-1);
	}

	String get_nodeName()
	{
		return m_name;
	}

	void set_nodeName(StringIn newVal)
	{
		THROW(-1);
	}

	ASObjectType get_asNodeType() const
	{
		return AS_ENTITY_DECLARATION;
	}
};

}	// Web
}

#endif // __ASDTDENTITY_H_
