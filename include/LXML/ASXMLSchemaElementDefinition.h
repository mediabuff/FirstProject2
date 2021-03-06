#pragma once

namespace System
{
namespace Web
{

class CElementDefinition :
//	public CDefinition,
	public ASObjectImpl, public IASElementDeclaration
{
public:
	CElementDefinition()
	{
#if 0
		m_pParentModelGroup = NULL;
		m_pAnynomousTypeDef = NULL;
#endif
	}

	String m_nodeName;

	String get_nodeName()
	{
		return m_nodeName;
	}

	CTypeDefinition* m_pType;

	void BuildDefs(Element* element);

#if 0
//	CTypeDefinition* m_pAnynomousTypeDef;
	IASDataTypeDeclaration* m_pAnynomousTypeDef;
	CModelGroup* m_pParentModelGroup;

//	CTypeDefinition* GetTypeDefinition();
	STDMETHOD(get_elementDataType)(/*[out, retval]*/ IASDataTypeDeclaration* *pVal);

	_bstr_t GetElementName();
	bool IsRef();

	int GetMinOccurs();
	DWORD GetMaxOccurs();

	STDMETHODIMP GetDefType(BSTR* pVal)
	{
		*pVal = SysAllocString(L"element");
		return S_OK;
	}

BEGIN_COM_MAP(CElementDefinition)
	COM_INTERFACE_ENTRY(IASObject)
	COM_INTERFACE_ENTRY(IASDeclaration)
	COM_INTERFACE_ENTRY(IASElementDeclaration)
END_COM_MAP()
#endif

// IASObject
	ASObjectType get_asNodeType() const
	{
		return AS_ELEMENT_DECLARATION;
	}

#if 0
	STDMETHOD(cloneASObject)(/*[in]*/ bool deep, /*[out,retval]*/ IASObject** pVal);
#endif
// IASElementDeclaration

	ASDataType m_elementType;
	ASContentModelType m_contentType;

	ASDataType get_elementType() const
	{
		return m_elementType;
	}
	void set_elementType(/*[in]*/ ASDataType newVal)
	{
		//return E_FAIL;
	}
	ASContentModelType get_contentType()
	{
		return m_contentType;
	}
	void set_contentType(/*[in]*/ ASContentModelType newVal)
	{
		//return E_FAIL;
	}
	IASAttributeDeclaration* removeASAttributeDecl(IASAttributeDeclaration* attributeDecl)
	{
		VERIFY(0);
		return NULL;
	}
	void addASAttributeDecl(/*[in]*/ IASAttributeDeclaration* attributeDecl)
	{
		VERIFY(0);
	}
	bool get_strictMixedContent() const
	{
		return false;
	}
	void set_strictMixedContent(/*[in]*/ bool newVal)
	{
		//return E_FAIL;
	}
	String get_systemId()
	{
		return nullptr;
	}
	void set_systemId(StringIn newVal)
	{
		//return E_FAIL;
	}
	IASNamedObjectMap* get_ASAttributeDecls()
	{
		return NULL;
	}
	IASContentModel* get_asCM()
	{
		return NULL;
	}
	void set_asCM(IASContentModel* newVal)
	{
	}
	IASDataTypeDeclaration* get_elementDataType()
	{
		ASSERT(0);
		return NULL;
	}

	IASObjectList* get_attributesList()
	{
		return NULL;
	}

#if 0
	STDMETHOD(get_ASAttributeDecls)(/*[out, retval]*/ IASNamedObjectMap* *pVal)
	{
		if (pVal == NULL) return E_POINTER;
		*pVal = NULL;

		CComPtr<IASDataTypeDeclaration> dataType;
		get_elementDataType(&dataType);
		if (dataType)
		{
			CComQIPtr<IASComplexTypeDeclaration> complexType = dataType;
			if (complexType)
			{
				return complexType->get_ASAttributeDecls(pVal);
			}
		}

		return S_OK;
	}

	STDMETHOD(get_attributesList)(/*[out, retval]*/ IASObjectList* *pVal)
	{
		return S_OK;
	}

	STDMETHOD(get_elementType)(/*[out, retval]*/ ASDataType *pVal);
	STDMETHOD(set_elementType)(/*[in]*/ ASDataType newVal);

	STDMETHOD(get_contentType)(/*[out, retval]*/ ASContentModelType *pVal)
	{
		return S_OK;
	}
	STDMETHOD(set_contentType)(/*[in]*/ ASContentModelType newVal)
	{
		return S_OK;
	}
	STDMETHOD(removeASAttributeDecl)(/*[in]*/ IASAttributeDeclaration* attributeDecl, /*[out,retval]*/ IASAttributeDeclaration** pVal)
	{
		return S_OK;
	}
	STDMETHOD(addASAttributeDecl)(/*[in]*/ IASAttributeDeclaration* attributeDecl)
	{
		return S_OK;
	}
	STDMETHOD(get_strictMixedContent)(/*[out, retval]*/ bool *pVal)
	{
		return S_OK;
	}
	STDMETHOD(set_strictMixedContent)(/*[in]*/ bool newVal)
	{
		return S_OK;
	}
	STDMETHOD(get_isPCDataOnly)(/*[out, retval]*/ bool *pVal)
	{
		return S_OK;
	}
	STDMETHOD(set_isPCDataOnly)(/*[in]*/ bool newVal)
	{
		return S_OK;
	}
	STDMETHOD(get_systemId)(/*[out, retval]*/ BSTR *pVal)
	{
		return S_OK;
	}
	STDMETHOD(set_systemId)(/*[in]*/ BSTR newVal)
	{
		return S_OK;
	}
#endif
};

}	// Web
}
