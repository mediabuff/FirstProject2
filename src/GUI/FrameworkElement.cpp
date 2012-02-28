#include "stdafx.h"
#include "GUI2.h"

namespace System
{
namespace Gui
{


/*
class StyleDeclaration// : public Web::ICSSStyleDeclaration
{
public:

	virtual void SetProperty(StringIn name, StringIn value);

	bool HasProperty(DependencyProperty* dp);
};
*/

Style::IStyleDeclaration* StyleSheet::CreateDeclaration()
{
	return new CSSDeclaration;
}

void StyleSheet::AddRules(Style::SelectorList* selectors, Style::IStyleDeclaration* declaration)
{
	CSSStyleRule* rule = new CSSStyleRule(selectors, declaration);

	m_rules.push_back(rule);
	/*
	for (uint i = 0; i < selectors->m_items.size(); ++i)
	{
		Rule* rule = new Rule;
		rule->m_selector = selectors->m_items[i];
		rule->m_declaration = declaration;
	}
	*/
}

void CSSDeclaration::SetProperty(StringIn prefix, StringIn name, StringIn strvalue)
{
	DependencyProperty* dp;

	if (!prefix.IsEmpty())
	{
		DependencyClass* depClass = DependencyClass::FindClass(prefix);
		if (depClass == NULL)
		{
			return;
		}

		dp = depClass->GetLocalProperty(name);
	}
	else
	{
		dp = DependencyClass::FindGlobalProperty(name);
	}

	ASSERT(dp);

	PropertyValuePair pair;
	pair.m_dp = dp;

	// TODO

	Type* pType = dp->get_Type().GetType();

	if (pType->get_Kind() == type_bool)
	{
		bool value = (strvalue == L"true" || strvalue == L"1");
		pair.m_value = value;
	}
	else if (pType->get_Kind() == type_int)
	{
		ASSERT(0);
	//	int value = strvalue.ToInt();
	//	pair.m_value = IntObject::GetObject(value);
	}
	else if (pType->get_Kind() == type_float)
	{
		float value = (float)strvalue.ToDouble();
		pair.m_value = value;
	}
	else if (pType->get_Kind() == type_double)
	{
		double value = strvalue.ToDouble();
		pair.m_value = value;
	}
	else
		ASSERT(0);
	//dp->CheckValueType(value);

	m_props.push_back(pair);
}

bool CSSDeclaration::HasProperty(Object* dp)
{
	for (uint i = 0; i < m_props.size(); ++i)
	{
		if (m_props[i].m_dp == dp)
			return true;
	}

	return false;
}

Variant CSSDeclaration::GetPropertyValue(DependencyProperty* dp)
{
	for (uint i = 0; i < m_props.size(); ++i)
	{
		if (m_props[i].m_dp == dp)
			return m_props[i].m_value;
	}

	return nullptr;
}

GUIEXT StyleSheet* g_styleSheet;

Variant CSSObjectBinding::GetPropertyValue(DependencyProperty* dp)
{
	ASSERT(m_ruleMatches);

	Variant value;
	int r = m_ruleMatches->GetSpecifiedPropertyValue2(dp, false, &value);
	if (r == Style::Value_Specified)
	{
		//Style::StyleSheet::GetDeclMatches(),
		return value;
	}
	return nullptr;
}

bool CSSObjectBinding::HasProperty(DependencyProperty* dp)
{
	ASSERT(m_ruleMatches);

	Variant value;
	int r = m_ruleMatches->GetSpecifiedPropertyValue2(dp, false, &value);
	if (r == Style::Value_Specified)
	{
		return true;
	}
	return false;
}

////

DependencyClass* FrameworkElement::get_Class()
{
	static DependencyClass depclass(typeid(thisClass), baseClass::get_Class());

	static DependencyProperty* properties[] =
	{
		get_BindingProperty(),
	};

	return &depclass;
}

IMP_DEPENDENCY_PROPERTY(ObjectBinding*, FrameworkElement, Binding, nullptr)

FrameworkElement::FrameworkElement(DependencyClass* depClass) : UIElement(depClass)
{
}

FrameworkElement::FrameworkElement(DependencyClass* depClass, gm::SizeF size) : UIElement(depClass, size)
{
}

bool FrameworkElement::HasComputedPropertyValue(DependencyProperty* dp)
{
	if (dp == get_BindingProperty())
	{
		return true;
	}

	ObjectBinding* binding = static_cast<ObjectBinding*>(GetPropertyValue(get_BindingProperty()));
	if (binding)
	{
		return binding->HasProperty(dp);
	}

	return false;
}

Variant FrameworkElement::GetComputedPropertyObjectValue2(DependencyProperty* dp)
{
	if (dp == get_BindingProperty())
	{
		Style::CSSElementMatches* ruleMatches = new Style::CSSElementMatches;
		ruleMatches->m_elementResolver = this;

		CSSObjectBinding* objectBinding = new CSSObjectBinding;
		objectBinding->m_ruleMatches = ruleMatches;

		ASSERT(g_styleSheet);
		Style::StyleSheet::GetStyleSheetMatches(g_styleSheet, ruleMatches, this);

		return objectBinding;
	}

	ObjectBinding* binding = static_cast<ObjectBinding*>(GetPropertyValue(get_BindingProperty()));
	if (binding)
	{
		return binding->GetPropertyValue(dp);
	}

	return nullptr;
}

}	// Gui
}	// System
