#include "stdafx.h"
//nclude "LXML.h"
#include "LXML2.h"

#include "CSSStyleDeclaration.h"

#include "CSSPrimitiveValue.h"
#include "CSSValueList.h"
#include "SVGColor.h"
#include "SVGPaint.h"

//#include "CSSStream.h"

namespace System
{
namespace Web
{

extern uint8 IID_ILCSSPrimitiveValue;
extern uint8 IID_ILCSSValueList;
extern uint8 CLSID_LSVGColor;
extern uint8 CLSID_LSVGPaint;

//extern GUID IID_ILCSSPrimitiveValue;
//extern GUID IID_ILCSSValueList;
//extern CLSID CLSID_LSVGColor;
//extern CLSID CLSID_LSVGPaint;

//#include "LDependencyObjectImpl.h"

#if 0
enum
{
	Display_inline = 0,
	Display_block,
	Display_list_item,
	Display_run_in,
	Display_compact,
	Display_marker,
	Display_table,
	Display_inline_table,
	Display_table_row_group,
	Display_table_header_group,
	Display_table_footer_group,
	Display_table_row,
	Display_table_column_group,
	Display_table_column,
	Display_table_cell,
	Display_table_caption,
	Display_none,

// Mozilla compliant XUL display modes
	Display_inline_box,
	Display_box,
};

enum
{
	DisplayModel_inline_inside,
	DisplayModel_block_inside,
	DisplayModel_table,
	DisplayModel_ruby,

// Mozilla compliant XUL display modes
	DisplayModel_box
};

enum
{
	DisplayRole_none,
	DisplayRole_block,
	DisplayRole_inline,
	DisplayRole_list_item,
	DisplayRole_run_in,
	DisplayRole_compact,
	DisplayRole_table_row,
	DisplayRole_table_cell,
	DisplayRole_table_row_group,
	DisplayRole_table_header_group,
	DisplayRole_table_footer_group,
	DisplayRole_table_column,
	DisplayRole_table_column_group,
	DisplayRole_table_caption,
	DisplayRole_ruby_text,
	DisplayRole_ruby_base,
	DisplayRole_ruby_base_group,
	DisplayRole_ruby_text_group
};

/*
enum
{
	BackgroundRepeat_repeat = 0,
	BackgroundRepeat_repeat_x,
	BackgroundRepeat_repeat_y,
	BackgroundRepeat_no_repeat,
};
*/

enum
{
	Visibility_visible = 0,
	Visibility_hidden,
	Visibility_collapse	// For tables
};

enum
{
	TextAlign_left = 0,
	TextAlign_right,
	TextAlign_center,
	TextAlign_justify,

	TextAlign_String
};

enum
{
	StrokeLineCap_butt,
	StrokeLineCap_round,
	StrokeLineCap_square,
};

enum
{
	StrokeLineJoin_miter,
	StrokeLineJoin_round,
	StrokeLineJoin_bevel,
};

enum
{
	FontStyle_normal = 0,
	FontStyle_italic,
	FontStyle_oblique
};

/*
enum
{
	TextDecoration_none = 0,
	TextDecoration_underline,
	TextDecoration_overline,
	TextDecoration_lineThrough,
	TextDecoration_blink
};
*/

enum
{
	Position_static = 0,
	Position_relative,
	Position_absolute,
	Position_fixed,
};

enum
{
	ListStylePosition_inside,
	ListStylePosition_outside,
};

enum
{
	Float_left = 0,
	Float_right,

	Float_top,
	Float_bottom,

	Float_inside,
	Float_outside,
	Float_start,
	Float_end,
	Float_none
};

enum
{
	Clear_none = 0,
	Clear_left,
	Clear_right,
	Clear_top,
	Clear_bottom,
	Clear_inside,
	Clear_outside,
	Clear_start,
	Clear_end,
	Clear_both
};

enum
{
	Direction_ltr = 0,
	Direction_rtl,
};

enum
{
	TextAnchor_start = 0,
	TextAnchor_middle,
	TextAnchor_end
};

enum
{
	Overflow_visible = 0,
	Overflow_hidden,
	Overflow_scroll,
	Overflow_auto,
};

/*
enum
{
	BorderStyle_none = 0,
	BorderStyle_hidden,			// CSS3
	BorderStyle_dotted,
	BorderStyle_dashed,
	BorderStyle_solid,
	BorderStyle_double,
	BorderStyle_dot_dash,		// CSS3
	BorderStyle_dot_dot_dash,	// CSS3
	BorderStyle_wave,				// CSS3
	BorderStyle_groove,
	BorderStyle_ridge,
	BorderStyle_inset,
	BorderStyle_outset,
};
*/

enum
{
	PointerEvents_visiblePainted = 0,
	PointerEvents_visibleFill,
	PointerEvents_visibleStroke,
	PointerEvents_visible,
	PointerEvents_painted,
	PointerEvents_fill,
	PointerEvents_stroke,
	PointerEvents_all,
	PointerEvents_none 
};

enum
{
	FillRule_nonzero = 0,
	FillRule_evenodd
};

enum
{
	ClipRule_nonzero = 0,
	ClipRule_evenodd
};

enum
{
	ColorInterpolation_auto = 0,
	ColorInterpolation_sRGB,
	ColorInterpolation_linearRGB,
};

enum
{
	Cursor_auto,
	Cursor_crosshair,
	Cursor_default,
	Cursor_pointer,
	Cursor_move,
	Cursor_e_resize,
	Cursor_ne_resize,
	Cursor_nw_resize,
	Cursor_n_resize,
	Cursor_se_resize,
	Cursor_sw_resize,
	Cursor_s_resize,
	Cursor_w_resize,
	Cursor_text,
	Cursor_wait,
	Cursor_help,
	Cursor_progress,
	Cursor_copy,
	Cursor_alias,
	Cursor_context_menu,
	Cursor_cell,
	Cursor_all_scroll,
	Cursor_col_resize,
	Cursor_row_resize,
	Cursor_no_drop,
	Cursor_not_allowed,
	Cursor_vertical_text,
};

enum
{
	RenderingIntent_auto,
	RenderingIntent_perceptual,
	RenderingIntent_relative_colorimetric,
	RenderingIntent_saturation,
	RenderingIntent_absolute_colorimetric,
};

enum
{
	ShapeRendering_auto,
	ShapeRendering_optimizeSpeed,
	ShapeRendering_crispEdges,
	ShapeRendering_geometricPrecision
};

enum
{
	ImageRendering_auto,
	ImageRendering_optimizeSpeed,
	ImageRendering_optimizeQuality
};

enum	// CSS Tables
{
	BorderCollapse_collapse,
	BorderCollapse_separate,
};

enum	// CSS Tables
{
	EmptyCells_show,
	EmptyCells_hide,
};

enum	// Mozilla compliant XUL
{
	BoxOrient_horizontal,
	BoxOrient_vertical,
	BoxOrient_inline_axis,
	BoxOrient_block_axis,
};

enum	// Mozilla compliant XUL
{
	BoxAlign_start,
	BoxAlign_end,
	BoxAlign_center,
	BoxAlign_baseline,
	BoxAlign_stretch,
};

enum
{
	ListStyleType_normal,
	ListStyleType_none,

// glyph
	ListStyleType_box,
	ListStyleType_check,
	ListStyleType_circle,
	ListStyleType_diamond,
	ListStyleType_disc,
	ListStyleType_hyphen,
	ListStyleType_square,

// algorithmic
	ListStyleType_armenian,
	ListStyleType_cjk_ideographic,
	ListStyleType_ethiopic_numeric,
	ListStyleType_georgian,
	ListStyleType_hebrew,
	ListStyleType_japanese_formal,
	ListStyleType_japanese_informal,
	ListStyleType_lower_armenian,
	ListStyleType_lower_roman,
	ListStyleType_simp_chinese_formal,
	ListStyleType_simp_chinese_informal,
	ListStyleType_syriac,
	ListStyleType_tamil,
	ListStyleType_trad_chinese_formal,
	ListStyleType_trad_chinese_informal,
	ListStyleType_upper_armenian,
	ListStyleType_upper_roman,

// numeric
	ListStyleType_arabic_indic,
	ListStyleType_binary,
	ListStyleType_bengali,
	ListStyleType_cambodian,
	ListStyleType_decimal,
	ListStyleType_decimal_leading_zero,
	ListStyleType_devanagari,
	ListStyleType_gujarati,
	ListStyleType_gurmukhi,
	ListStyleType_kannada,
	ListStyleType_khmer,
	ListStyleType_lao,
	ListStyleType_lower_hexadecimal,
	ListStyleType_malayalam,
	ListStyleType_mongolian,
	ListStyleType_myanmar,
	ListStyleType_octal,
	ListStyleType_oriya,
	ListStyleType_persian,
	ListStyleType_telugu,
	ListStyleType_tibetan,
	ListStyleType_thai,
	ListStyleType_upper_hexadecimal,
	ListStyleType_urdu,

// alphabetic
	ListStyleType_afar,
	ListStyleType_amharic,
	ListStyleType_amharic_abegede,
	ListStyleType_cjk_earthly_branch,
	ListStyleType_cjk_heavenly_stem,
	ListStyleType_ethiopic,
	ListStyleType_ethiopic_abegede,
	ListStyleType_ethiopic_abegede_am_et,
	ListStyleType_ethiopic_abegede_gez,
	ListStyleType_ethiopic_abegede_ti_er,
	ListStyleType_ethiopic_abegede_ti_et,
	ListStyleType_ethiopic_halehame_aa_er,
	ListStyleType_ethiopic_halehame_aa_et,
	ListStyleType_ethiopic_halehame_am_et,
	ListStyleType_ethiopic_halehame_gez,
	ListStyleType_ethiopic_halehame_om_et,
	ListStyleType_ethiopic_halehame_sid_et,
	ListStyleType_ethiopic_halehame_so_et,
	ListStyleType_ethiopic_halehame_ti_er,
	ListStyleType_ethiopic_halehame_ti_et,
	ListStyleType_ethiopic_halehame_tig,
	ListStyleType_hangul,
	ListStyleType_hangul_consonant,
	ListStyleType_hiragana,
	ListStyleType_hiragana_iroha,
	ListStyleType_katakana,
	ListStyleType_katakana_iroha,
	ListStyleType_lower_alpha,
	ListStyleType_lower_greek,
	ListStyleType_lower_norwegian,
	ListStyleType_lower_latin,
	ListStyleType_oromo,
	ListStyleType_sidama,
	ListStyleType_somali,
	ListStyleType_tigre,
	ListStyleType_tigrinya_er,
	ListStyleType_tigrinya_er_abegede,
	ListStyleType_tigrinya_et,
	ListStyleType_tigrinya_et_abegede,
	ListStyleType_upper_alpha,
	ListStyleType_upper_greek,
	ListStyleType_upper_norwegian,
	ListStyleType_upper_latin,

// symbolic
	ListStyleType_asterisks,
	ListStyleType_footnotes,
};
#endif
}
}

#include "cssproperty.h"

#include "cssunits.h"
#include "parsecolor.h"

#if 0
#include "AnimationTarget.h"
#endif

//sysstring striptrailspaces(const OLECHAR* bstr);
namespace System
{
namespace Web
{

int IdentNameToIndex(StringIn ident, ImmutableString<WCHAR> idents[]);

extern ImmutableString<char16> BorderStyleIdents[];
extern ImmutableString<char16> DisplayModelIdents[];
extern ImmutableString<char16> DisplayRoleIdents[];

ImmutableString<char16> DisplayStrings[] =
{
	WSTR("inline"),
	WSTR("block"),
	WSTR("list-item"),
	WSTR("run-in"),
	WSTR("compact"),
	WSTR("marker"),
	WSTR("table"),
	WSTR("table-inline"),
	WSTR("table-row-group"),
	WSTR("table-header-group"),
	WSTR("table-footer-group"),
	WSTR("table-row"),
	WSTR("table-column-group"),
	WSTR("table-column"),
	WSTR("table-cell"),
	WSTR("table-caption"),
	WSTR("none"),

// Mozilla compliant XUL display modes
	WSTR("inline_box"),
	WSTR("box"),

	NULL,
};

bool IsColor(StringIn v)
{
	ASSERT(0);
#if 0
	if (v.CompareNoCase(L"transparent") == 0 ||
		v.CompareNoCase(L"currentColor") == 0)
	{
		return true;
	}
	else if (ParseSingleRGBAColor(v->c_str(), NULL) >= 0)
	{
		return true;
	}
#endif
	return false;
}

bool IsBorderWidth(StringIn v)
{
	ASSERT(0);
#if 0
	if (v->CompareNoCase(L"thin") == 0 ||
		v->CompareNoCase(L"medium") == 0 ||
		v->CompareNoCase(L"thick") == 0)
	{
		return true;
	}
	else if (ParseValueUnitString(v->c_str(), NULL, NULL))
	{
		return true;
	}
#endif
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CSSDecl

CSSDecl::~CSSDecl()
{
	if (m_pValue)
	{
#if 0
		m_pValue->m_pListener = NULL; // ????
#endif
	//	int refcount = m_pValue->Release();
		m_pValue = NULL;
	}
}

#if 0
CValueType* NewCSSPropertyValue(int propertyIndex)
{
	CSSProperty* style = &csspropertyList[propertyIndex];

	if (*style->m_cssType == CLSID_LCSSPrimitiveValue)
	{
		CCSSValueType<CreateInstance<CSSPrimitiveValue> >* value;
	//	CCSSValueType<CComObjectCreateInstance<CSSPrimitiveValue> >::CreateInstance(&value);
		value->m_value->m_propertyIndex = m_propertyIndex;
		value->AddRef();

		return value;
	}
	else if (*style->m_cssType == IID_ILCSSValueList)
	{
		CCSSValueType<CComObjectCreateInstance<CSSValueList> > >* value;
		CCSSValueType<CComObjectCreateInstance<CSSValueList> > >::CreateInstance(&value);
		value->AddRef();
		value->m_value->m_propertyIndex = m_propertyIndex;

		return value;
	}
	else if (*style->m_cssType == CLSID_LSVGColor)
	{
		<CCSSValueType<CComObjectCreateInstance<SVGColor> > >* value;
		<CCSSValueType<CComObjectCreateInstance<SVGColor> > >::CreateInstance(&value);
		value->AddRef();
	//	value->m_value->m_propertyIndex = propertyIndex;

		return value;
	}
	else if (*style->m_cssType == CLSID_LSVGPaint)
	{
		<CCSSValueType<CComObjectCreateInstance<SVGPaint> > >* value;
		<CCSSValueType<CComObjectCreateInstance<SVGPaint> > >::CreateInstance(&value);
		value->AddRef();

		return value;
	}

	return NULL;
}
#endif

//virtual
CValueType* CCSSPropertyType::NewVal()
{
	ASSERT(0);
#if 0
	CSSProperty* style = &csspropertyList[m_propertyIndex];

	if (*style->m_cssType == IID_ILCSSPrimitiveValue)
	{
		<CCSSValueType<CComObjectCreateInstance<CSSPrimitiveValue> > >* value;
		<CCSSValueType<CComObjectCreateInstance<CSSPrimitiveValue> > >::CreateInstance(&value);
		value->m_value->m_propertyIndex = m_propertyIndex;
		value->AddRef();

		return value;
	}
	else if (*style->m_cssType == IID_ILCSSValueList)
	{
		<CCSSValueType<CComObjectCreateInstance<CSSValueList> > >* value;
		<CCSSValueType<CComObjectCreateInstance<CSSValueList> > >::CreateInstance(&value);
		value->AddRef();
		value->m_value->m_propertyIndex = m_propertyIndex;

		return value;
	}
	else if (*style->m_cssType == CLSID_LSVGColor)
	{
		<CCSSValueType<CComObjectCreateInstance<SVGColor> > >* value;
		<CCSSValueType<CComObjectCreateInstance<SVGColor> > >::CreateInstance(&value);
		value->AddRef();
	//	value->m_value->m_propertyIndex = propertyIndex;

		return value;
	}
	else if (*style->m_cssType == CLSID_LSVGPaint)
	{
		<CCSSValueType<CComObjectCreateInstance<SVGPaint> > >* value;
		<CCSSValueType<CComObjectCreateInstance<SVGPaint> > >::CreateInstance(&value);
		value->AddRef();

		return value;
	}
#endif
	return NULL;
}

#if 0
//virtual
CValueType* CSSDecl::NewVal()
{
	return NewCSSPropertyValue(m_propertyIndex];

#if 0
//	int propertyIndex = GetCSSStyleIndex(m_propertyName);
	CSSProperty* style = &csspropertyList[m_propertyIndex];

	if (*style->m_cssType == CLSID_LCSSPrimitiveValue)
	{
		CComObject<CCSSValueType<CComObjectCreateInstance<CSSPrimitiveValue> > >* value;
		CComObject<CCSSValueType<CComObjectCreateInstance<CSSPrimitiveValue> > >::CreateInstance(&value);
		value->m_value->m_propertyIndex = m_propertyIndex;
		value->AddRef();

		return value;
	}
	else if (*style->m_cssType == IID_ILCSSValueList)
	{
		<CCSSValueType<CComObjectCreateInstance<CSSValueList> > >* value;
		<CCSSValueType<CComObjectCreateInstance<CSSValueList> > >::CreateInstance(&value);
		value->AddRef();
		value->m_value->m_propertyIndex = m_propertyIndex;

		return value;
	}
	else if (*style->m_cssType == CLSID_LSVGColor)
	{
		<CCSSValueType<CComObjectCreateInstance<SVGColor> > >* value;
		<CCSSValueType<CComObjectCreateInstance<SVGColor> > >::CreateInstance(&value);
		value->AddRef();
	//	value->m_value->m_propertyIndex = propertyIndex;

		return value;
	}
	else if (*style->m_cssType == CLSID_LSVGPaint)
	{
		<CCSSValueType<CComObjectCreateInstance<SVGPaint> > >* value;
		<CCSSValueType<CComObjectCreateInstance<SVGPaint> > >::CreateInstance(&value);
		value->AddRef();

		return value;
	}

	return NULL;
#endif
}
#endif

#if 0
//virtual
void CSSDecl::SetAnimVal(BSTR bstr)
{
	*m_pValue = bstr;
}
#endif

/////////////////////////////////////////////////////////////////////////////
// CSSStyleDeclaration

/*
void CSSStyleDeclaration::FinalRelease()
{
	Clear();
}
*/

void CSSStyleDeclaration::Clear()
{
	for (size_t i = 0; i < m_decls.size(); ++i)
	{
	//	delete m_decls[i];
	}
	m_decls.clear();
}

void CSSStyleDeclaration::SetProperty(StringIn prefix, StringIn name, StringIn strvalue)
{
	internalSetProperty(name, strvalue, nullptr);
}

CSSDecl* CSSStyleDeclaration::FindDecl(StringIn name) const
{
	for (size_t i = 0; i < m_decls.size(); ++i)
	{
		if (name == m_decls[i]->m_propertyName)
			return m_decls[i];
	}

	return nullptr;
}

CSSDecl* CSSStyleDeclaration::FindDecl(int propertyIndex) const
{
	for (size_t i = 0; i < m_decls.size(); ++i)
	{
		if (m_decls[i]->m_propertyIndex == propertyIndex)
			return m_decls[i];
	}

	return nullptr;
}

void CSSStyleDeclaration::ParseCSSText()
{
	++m_bSettingPropertyText;

// Remove all existing properties
	Clear();

	Style::CSSStream stream(m_cssText);
	stream.getnextc();
	stream.ParseDeclaration(this);

	--m_bSettingPropertyText;

#if 0
	m_bSettingPropertyText++;

// Remove all existing properties
	Clear();

// Parse the string and add property declarations
//	const OLECHAR* p = m_cssText.c_str();

	CSSStream stream(m_cssText);
	stream.getnextc();

//	if (p == NULL) return;

	while (!stream.eof())
	{
		//while (*p && iswspace(*p)) p++;	// Skip spaces
		stream.SkipSpacesComments();

		//StringStream strbuilder;
		String propertyName;

	// Get the propertyName
		while (!stream.eof())
		{
			if (stream.m_c == ':')
			{
				//p--;
				break;
			}
			if (stream.m_c == ' ')
			{
				stream.getnextc();
				break;
			}

			propertyName += (WCHAR)stream.m_c;
			stream.getnextc();
		}

	//	String propertyName = strbuilder->str();

		//while (*p && iswspace(*p)) p++;	// Skip spaces
		stream.SkipSpacesComments();

		stream.EatChar(':');	// Error

		//while (*p && iswspace(*p)) p++;	// Skip spaces
		stream.SkipSpacesComments();

	// Get property Value

		String value;

		while (!stream.eof())
		{
			if (stream.m_c == ';')
			{
				stream.getnextc();
				break;
			}

			if (stream.m_c == '!')
			{
				//p--;
				stream.getnextc();
				break;
			}

			/*
			if (c == '}')
			{
				m_CSS->ungetc();
				break;
			}
			*/

			value += (WCHAR)stream.m_c;
			stream.getnextc();
		}

		//value = striptrailspaces(value.c_str());
	//	TODO
	//	ASSERT(0);
#if 0
		value = value.TrimRight();
#endif
	//	while (*p && iswspace(*p)) p++;	// Skip spaces
		stream.SkipSpacesComments();

		String priority;

	//	stream.getnextc();
		if (stream.m_c == '!')
		{
			stream.getnextc();

			//while (*p && iswspace(*p)) p++;	// Skip spaces
			stream.SkipSpacesComments();

		// Get optional !important
			while (!stream.eof())
			{
				if (!isalpha(stream.m_c))
				{
					break;
				}

				priority += (WCHAR)stream.m_c;
				stream.getnextc();
			}
		}
		else
		{
		}

	//	String priority = buffer.DetachToString();

		internalSetProperty(propertyName, value, priority);

		//while (*p && iswspace(*p)) p++;	// Skip spaces
		stream.SkipSpacesComments();
	}

	m_bSettingPropertyText--;
#endif
}

void CSSStyleDeclaration::UpdateCSSText()
{
	ASSERT(0);
#if 0

// TODO, output !important here too
	m_cssText = OLESTR("");

	for (int i = 0; i < m_decls.size(); i++)
	{
		CSSDecl* pdecl = m_decls[i];
		if (pdecl->m_bExcplicitSet)
		{
			m_cssText += pdecl->m_propertyName;
			m_cssText += OLESTR(":");

			//BSTR b;
			//pdecl->m_pValue->m_cssvalue->get_cssText(&b);
		//	sysstring b = pdecl->m_pValue->getStringValue();
			sysstring b = pdecl->m_pValue->get_cssText();

			m_cssText += b;

			m_cssText += OLESTR(";");
		}
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CSSStyleDeclaration

String CSSStyleDeclaration::get_cssText()
{
	if (!m_bCSSTextUpdated)
	{
		UpdateCSSText();
		m_bCSSTextUpdated = true;
	}

	return m_cssText;
}

void CSSStyleDeclaration::set_cssText(StringIn newVal)
{
	m_cssText = newVal;
	m_bCSSTextUpdated = true;

	ParseCSSText();

	if (m_pListener)
	{
		m_pListener->OnStyleDeclChanged(this);
	}

	/*
	for (int i = 0; i < m_handlers.size(); i++)
	{
		m_handlers[i]->handleCSSEvent(0, NULL);
	}
	*/
}

String CSSStyleDeclaration::getPropertyValue(StringIn propertyName)
{
	CSSDecl* pDecl = FindDecl(propertyName);
	if (pDecl)
	{
		return pDecl->m_pValue->get_cssText();//get_cssText(pVal);
	//	return pDecl->m_pValue->/*m_cssvalue->*/getStringValue();//get_cssText(pVal);
	}
	else
	{
		/*
		CSSProperty* pStyle = GetCSSStyle(propertyName);
		if (pStyle)
			*pValue = SysAllocString(pStyle->m_initialCSS);
		else
		*/
		return nullptr;
	}
}

CSSDecl* CSSStyleDeclaration::internalCreateLonghandProperty(StringIn propertyName, int propertyIndex)
{
	CSSDecl* pdecl = new CSSDecl;
	pdecl->m_propertyName = propertyName;
	pdecl->m_propertyIndex = propertyIndex;

//	int propertyIndex = GetCSSStyleIndex(pdecl->m_propertyName);
	if (propertyIndex >= 0)
	{
		CSSProperty* style = CSSProperties::csspropertyList[propertyIndex];

	// Create the value of correct type
		if (style->m_cssType == IID_ILCSSPrimitiveValue)
		{
			CSSPrimitiveValue* value = new CSSPrimitiveValue;
			value->m_propertyIndex = propertyIndex;
			value->m_pListener = this;

			pdecl->m_pValue = value;//new CCSSPrimitiveValue(propertyIndex);

#if 0
			CCSSValueType<CreateInstanceT<CSSPrimitiveValue> >* value = new CCSSValueType<CreateInstanceT<CSSPrimitiveValue> >;
//			CCSSValueType<CComObjectCreateInstance<CSSPrimitiveValue> > >::CreateInstance(&value);
//			value->AddRef();
			value->m_value->m_propertyIndex = propertyIndex;
			value->m_value->m_pListener = this;

		//	pdecl->m_pValue = value;//new CCSSPrimitiveValue(propertyIndex);
			pdecl->m_pValue = value->m_value;//new CCSSPrimitiveValue(propertyIndex);
#endif
		}
		else if (style->m_cssType == IID_ILCSSValueList)
		{
			CSSValueList* value = new CSSValueList;
			value->m_propertyIndex = propertyIndex;
			value->m_pListener = this;

			pdecl->m_pValue = value;

#if 0
			CCSSValueType<CreateInstanceT<CSSValueList> >* value = new CCSSValueType<CreateInstanceT<CSSValueList> >;
		//	CComObject<CCSSValueType<CComObjectCreateInstance<CSSValueList> > >::CreateInstance(&value);
		//	value->AddRef();
			value->m_value->m_propertyIndex = propertyIndex;
			value->m_value->m_pListener = this;

		//	pdecl->m_pValue = value;
			pdecl->m_pValue = value->m_value;
#endif
		}
		else if (style->m_cssType == CLSID_LSVGColor)
		{
			SVGColor* value = new SVGColor;
			value->m_pListener = this;

			pdecl->m_pValue = value;//new CCSSPrimitiveValue(propertyIndex);

			//	pdecl->m_pValue = new CSVGColorValue(propertyIndex);
		}
		else if (style->m_cssType == CLSID_LSVGPaint)
		{
			SVGPaint* value = new SVGPaint;
			value->m_pListener = this;

			pdecl->m_pValue = value;//new CCSSPrimitiveValue(propertyIndex);

			//	pdecl->m_pValue = new CSVGPaintValue(propertyIndex);
		}
		else
		{
			ASSERT(0);
			//pdecl->m_value.CoCreateInstance(*style->m_cssType);
		}
		//pdecl->m_value.CoCreateInstance(*style->m_cssType);
	}
	else	// Unknown property
	{
		CSSValueUnknown* value = new CSSValueUnknown;

		pdecl->m_pValue = value;
	}

	return pdecl;
}

#if 0
void CSSStyleDeclaration::internalAddLonghandProperty(const sysstring& propertyName, int propertyIndex)
{
	ASSERT(FindDecl(propertyName) == NULL);
}
#endif

void/*CSSDecl* */ CSSStyleDeclaration::internalSetLonghandProperty(StringIn propertyName, int propertyIndex, StringIn value, StringIn priority, bool bExcplicit)
{
	CSSDecl* pdecl;
	
	CSSDecl* pprevdecl = FindDecl(propertyName);
	if (pprevdecl == NULL)
	{
		pdecl = internalCreateLonghandProperty(propertyName, propertyIndex);
	}
	else
	{
		pdecl = pprevdecl;
	}

	if (pdecl)
	{
		pdecl->m_bExcplicitSet = bExcplicit;
		pdecl->m_priority = priority;

		TRY
		{
			pdecl->m_pValue->set_cssText(value);
			if (pprevdecl == NULL)
			{
				m_decls.Add(pdecl);
			}
		}
		CATCH(int n)
		{
			if (pprevdecl == NULL)
			{
				delete pdecl;
			}
		//	return NULL;
		}
	}

//	return pdecl;
}

int display_mapping[][2] =
{
	DisplayModel_inline_inside, DisplayRole_inline,	//OLESTR(OLESTR(L"inline",
	DisplayModel_block_inside, DisplayRole_block,//OLESTR(OLESTR(L"block",
	DisplayModel_block_inside, DisplayRole_list_item,//OLESTR(OLESTR(L"list-item",
	DisplayModel_inline_inside, DisplayRole_run_in,//OLESTR(OLESTR(L"run-in",
	DisplayModel_inline_inside, DisplayRole_compact,//OLESTR(OLESTR(L"compact",
	-1,-1,// ??? //"marker",
	DisplayModel_table, DisplayRole_block,//OLESTR(OLESTR(L"table",
	DisplayModel_table, DisplayRole_inline,//OLESTR(OLESTR(L"table-inline",

	DisplayModel_inline_inside, DisplayRole_table_row_group,//OLESTR(OLESTR(L"table-row-group",
	DisplayModel_inline_inside, DisplayRole_table_header_group,//OLESTR(OLESTR(L"table-header-group",
	DisplayModel_inline_inside, DisplayRole_table_footer_group,//OLESTR(OLESTR(L"table-footer-group",
	DisplayModel_inline_inside, DisplayRole_table_row,//OLESTR(OLESTR(L"table-row",
	DisplayModel_inline_inside, DisplayRole_table_column_group,//OLESTR(OLESTR(L"table-column-group",
	DisplayModel_inline_inside, DisplayRole_table_column,//OLESTR(OLESTR(L"table-column",
	DisplayModel_inline_inside, DisplayRole_table_cell,//OLESTR(OLESTR(L"table-celOLESTR(OLESTR(L",
	DisplayModel_inline_inside, DisplayRole_table_caption,//OLESTR(OLESTR(L"table-caption",

	DisplayModel_inline_inside, DisplayRole_none,//OLESTR(OLESTR(L"none",

// Mozilla compliant XUL display modes
	DisplayModel_box, DisplayRole_inline, //OLESTR(OLESTR(L"inline_box",
	DisplayModel_box, DisplayRole_block,//OLESTR(OLESTR(L"box",
};

void CSSStyleDeclaration::internalSetProperty(int propertyIndex, StringIn value, StringIn priority)
{
	CSSProperty* style = CSSProperties::csspropertyList[propertyIndex];

	if (style->m_cssType != 0)
	{
		internalSetLonghandProperty(&style->m_name, propertyIndex, value, priority, true);
	}
	else	// short-hand property
	{
		switch (propertyIndex)
		{
		case CSSProperty_display:
			{
				int index = IdentNameToIndex(value, DisplayStrings);

				internalSetLonghandProperty(CSSProperty_display_model, &DisplayModelIdents[display_mapping[index][0]], priority, false);
				internalSetLonghandProperty(CSSProperty_display_role, &DisplayRoleIdents[display_mapping[index][1]], priority, false);
			}
			break;

		case CSSProperty_margin:
			{
				//String buffer[4];

				vector<String> values;
				values.reserve(4);
				GetSpaceSepStringArray(value, values);
				
				if (values.GetSize() == 0)
				{
				}
				else if (values.GetSize() == 1)
				{
					internalSetLonghandProperty(CSSProperty_margin_top, value, priority, false);
					internalSetLonghandProperty(CSSProperty_margin_right, value, priority, false);
					internalSetLonghandProperty(CSSProperty_margin_bottom, value, priority, false);
					internalSetLonghandProperty(CSSProperty_margin_left, value, priority, false);
				}
				else if (values.GetSize() == 2)
				{
					internalSetLonghandProperty(CSSProperty_margin_top, values[0], priority, false);
					internalSetLonghandProperty(CSSProperty_margin_right, values[1], priority, false);
					internalSetLonghandProperty(CSSProperty_margin_bottom, values[0], priority, false);
					internalSetLonghandProperty(CSSProperty_margin_left, values[1], priority, false);
				}
				else if (values.GetSize() == 3)
				{
					internalSetLonghandProperty(CSSProperty_margin_top, values[0], priority, false);
					internalSetLonghandProperty(CSSProperty_margin_right, values[1], priority, false);
					internalSetLonghandProperty(CSSProperty_margin_bottom, values[2], priority, false);
					internalSetLonghandProperty(CSSProperty_margin_left, values[1], priority, false);
				}
				else // >= 4
				{
					internalSetLonghandProperty(CSSProperty_margin_top, values[0], priority, false);
					internalSetLonghandProperty(CSSProperty_margin_right, values[1], priority, false);
					internalSetLonghandProperty(CSSProperty_margin_bottom, values[2], priority, false);
					internalSetLonghandProperty(CSSProperty_margin_left, values[3], priority, false);
				}
			}
			break;
			
		case CSSProperty_padding:
			{
				//StringW* buffer[4];
				vector<String> values;//(grow_allocator(buffer, sizeof(buffer)));
				values.reserve(4);
				GetSpaceSepStringArray(value, values);
				
				if (values.GetSize() == 0)
				{
				}
				else if (values.GetSize() == 1)
				{
					internalSetLonghandProperty(CSSProperty_padding_top, values[0], priority, false);
					internalSetLonghandProperty(CSSProperty_padding_bottom, values[0], priority, false);
					internalSetLonghandProperty(CSSProperty_padding_left, values[0], priority, false);
					internalSetLonghandProperty(CSSProperty_padding_right, values[0], priority, false);
				}
				else if (values.GetSize() == 2)
				{
					internalSetLonghandProperty(CSSProperty_padding_top, values[0], priority, false);
					internalSetLonghandProperty(CSSProperty_padding_bottom, values[0], priority, false);
					internalSetLonghandProperty(CSSProperty_padding_left, values[1], priority, false);
					internalSetLonghandProperty(CSSProperty_padding_right, values[1], priority, false);
				}
				else if (values.GetSize() == 3)
				{
					internalSetLonghandProperty(CSSProperty_padding_top, values[0], priority, false);
					internalSetLonghandProperty(CSSProperty_padding_left, values[1], priority, false);
					internalSetLonghandProperty(CSSProperty_padding_right, values[1], priority, false);
					internalSetLonghandProperty(CSSProperty_padding_bottom, values[2], priority, false);
				}
				else // >= 4
				{
					internalSetLonghandProperty(CSSProperty_padding_top, values[0], priority, false);
					internalSetLonghandProperty(CSSProperty_padding_bottom, values[1], priority, false);
					internalSetLonghandProperty(CSSProperty_padding_left, values[2], priority, false);
					internalSetLonghandProperty(CSSProperty_padding_right, values[3], priority, false);
				}
			}
			break;

		case CSSProperty_border_color:
			{
				vector<String> values;//(grow_allocator(buffer, sizeof(buffer)));
				values.reserve(4);
				GetSpaceSepStringArray(value, values);
				
				if (values.GetSize() == 0)
				{
				}
				else if (values.GetSize() == 1)
				{
					internalSetLonghandProperty(CSSProperty_border_top_color, values[0], priority, false);
					internalSetLonghandProperty(CSSProperty_border_bottom_color, values[0], priority, false);
					internalSetLonghandProperty(CSSProperty_border_left_color, values[0], priority, false);
					internalSetLonghandProperty(CSSProperty_border_right_color, values[0], priority, false);
				}
				else if (values.GetSize() == 2)
				{
					internalSetLonghandProperty(CSSProperty_border_top_color, values[0], priority, false);
					internalSetLonghandProperty(CSSProperty_border_bottom_color, values[0], priority, false);
					internalSetLonghandProperty(CSSProperty_border_left_color, values[1], priority, false);
					internalSetLonghandProperty(CSSProperty_border_right_color, values[1], priority, false);
				}
				else if (values.GetSize() == 3)
				{
					internalSetLonghandProperty(CSSProperty_border_top_color, values[0], priority, false);
					internalSetLonghandProperty(CSSProperty_border_left_color, values[1], priority, false);
					internalSetLonghandProperty(CSSProperty_border_right_color, values[1], priority, false);
					internalSetLonghandProperty(CSSProperty_border_bottom_color, values[2], priority, false);
				}
				else // >= 4
				{
					internalSetLonghandProperty(CSSProperty_border_top_color, values[0], priority, false);
					internalSetLonghandProperty(CSSProperty_border_bottom_color, values[1], priority, false);
					internalSetLonghandProperty(CSSProperty_border_left_color, values[2], priority, false);
					internalSetLonghandProperty(CSSProperty_border_right_color, values[3], priority, false);
				}
			}
			break;

		case CSSProperty_border_width:
			{
				// TODO
				internalSetLonghandProperty(CSSProperty_border_left_width, value, priority, false);
				internalSetLonghandProperty(CSSProperty_border_right_width, value, priority, false);
				internalSetLonghandProperty(CSSProperty_border_top_width, value, priority, false);
				internalSetLonghandProperty(CSSProperty_border_bottom_width, value, priority, false);
			}
			break;
			
		case CSSProperty_border_style:
			{
				// TODO
				internalSetLonghandProperty(CSSProperty_border_left_style, value, priority, false);
				internalSetLonghandProperty(CSSProperty_border_right_style, value, priority, false);
				internalSetLonghandProperty(CSSProperty_border_top_style, value, priority, false);
				internalSetLonghandProperty(CSSProperty_border_bottom_style, value, priority, false);
			}
			break;

		case CSSProperty_border:
			{
				vector<String> values;//(grow_allocator(buffer, sizeof(buffer)));
				values.reserve(4);
				GetSpaceSepStringArray(value, values);

				for (size_t i = 0; i < values.GetSize(); i++)
				{
					String v = values[i];

					if (IsBorderWidth(v))
					{
						internalSetLonghandProperty(CSSProperty_border_left_width, v, priority, false);
						internalSetLonghandProperty(CSSProperty_border_top_width, v, priority, false);
						internalSetLonghandProperty(CSSProperty_border_right_width, v, priority, false);
						internalSetLonghandProperty(CSSProperty_border_bottom_width, v, priority, false);
					}
					else if (IdentNameToIndex(v, BorderStyleIdents) >= 0)
					{
						internalSetLonghandProperty(CSSProperty_border_left_style, v, priority, false);
						internalSetLonghandProperty(CSSProperty_border_top_style, v, priority, false);
						internalSetLonghandProperty(CSSProperty_border_right_style, v, priority, false);
						internalSetLonghandProperty(CSSProperty_border_bottom_style, v, priority, false);
					}
					else if (IsColor(v))
					{
						internalSetLonghandProperty(CSSProperty_border_left_color, v, priority, false);
						internalSetLonghandProperty(CSSProperty_border_top_color, v, priority, false);
						internalSetLonghandProperty(CSSProperty_border_right_color, v, priority, false);
						internalSetLonghandProperty(CSSProperty_border_bottom_color, v, priority, false);
					}
				}
			}
			break;
			
			// border-xxx shorthands
		case CSSProperty_border_top:
		case CSSProperty_border_left:
		case CSSProperty_border_right:
		case CSSProperty_border_bottom:
			{
				int border_width;
				int border_style;
				int border_color;

				switch (propertyIndex)
				{
					case CSSProperty_border_top:
						{
							border_width = CSSProperty_border_top_width;
							border_style = CSSProperty_border_top_style;
							border_color = CSSProperty_border_top_color;
						}
						break;

					case CSSProperty_border_left:
						{
							border_width = CSSProperty_border_left_width;
							border_style = CSSProperty_border_left_style;
							border_color = CSSProperty_border_left_color;
						}
						break;

					case CSSProperty_border_right:
						{
							border_width = CSSProperty_border_right_width;
							border_style = CSSProperty_border_right_style;
							border_color = CSSProperty_border_right_color;
						}
						break;

					case CSSProperty_border_bottom:
						{
							border_width = CSSProperty_border_bottom_width;
							border_style = CSSProperty_border_bottom_style;
							border_color = CSSProperty_border_bottom_color;
						}
						break;
				}

			//	vector<StringW*> values;
			//	String va[16];
				vector<String> values;///(grow_allocator(buffer, sizeof(buffer)));
				values.reserve(16);
				GetSpaceSepStringArray(value, values);

				String setvalue[3];

				for (size_t i = 0; i < values.GetSize(); i++)
				{
					String v = values[i];

					int unitType;
					ParseValueUnitString(CStringw(v).c_str(), &unitType);
					if (unitType != CSS_UNKNOWN ||
						!v.CompareNoCase(L"thin") ||
						!v.CompareNoCase(L"medium") ||
						!v.CompareNoCase(L"thick"))
					{
						setvalue[0] = v;
					}
					else if (IdentNameToIndex(v, BorderStyleIdents) >= 0)
					{
						setvalue[1] = v;
					}
					else if (IsColor(v))
					{
						setvalue[2] = v;
					}
				}

				if (setvalue[0].GetLength())
					internalSetLonghandProperty(border_width, setvalue[0], priority, false);
				else
					internalSetLonghandProperty(border_width, &CSSProperties::csspropertyList[border_width]->m_initialCSS, priority, false);

				if (setvalue[1].GetLength())
					internalSetLonghandProperty(border_style, setvalue[1], priority, false);
				else
					internalSetLonghandProperty(border_style, &CSSProperties::csspropertyList[border_style]->m_initialCSS, priority, false);

				if (setvalue[2].GetLength())
					internalSetLonghandProperty(border_color, setvalue[2], priority, false);
				else
					internalSetLonghandProperty(border_color, &CSSProperties::csspropertyList[border_color]->m_initialCSS, priority, false);
			}
			break;

		//////////////////////
		case CSSProperty_background:
			{
				// TODO
				internalSetLonghandProperty(CSSProperty_background_color, value, priority, false);
			}
			break;

		case CSSProperty_font:
			{
				ASSERT(0);
#if 0
				/* All font-related properties are first reset to their initial values, including those listed in the preceding paragraph
				plus 'font-stretch' and 'font-size-adjust'.
				Then, those properties that are given explicit values in the 'font' shorthand are set to those values
				*/

				// [ [ <'font-style'> || <'font-variant'> || <'font-weight'> ]? <'font-size'> [ / <'line-height'> ]? <'font-family'> ] | caption | icon | menu | message-box | small-caption | status-bar

			//	CArray<sysstring,sysstring> values;
			//	GetSpaceSepStringArray(value, values);

				String fontStyle = WSTR("normal");
				String fontWeight = WSTR("normal");
				String fontHeight = WSTR("12pt");
				String lineHeight = WSTR("normal");
				String fontFamily = WSTR("Courier New");

				const WCHAR* p = value->c_str();

				while (isspace(*p)) p++;

				while (*p)
				{
					String cssText;

					while (*p)
					{
						if (isspace(*p)) break;
						cssText += *p++;
					}

					if (!std::wcscmp(cssText->c_str(), L"italic") ||
						!std::wcscmp(cssText->c_str(), L"oblique")/* ||
						!tcscmp(cssText.c_str(), OLESTR("normal"))*/)
					{
						fontStyle = cssText;
					}
					else if (!std::wcscmp(cssText->c_str(), L"100") ||
								!std::wcscmp(cssText->c_str(), L"200") ||
								!std::wcscmp(cssText->c_str(), L"300") ||
								!std::wcscmp(cssText->c_str(), L"400") ||
								!std::wcscmp(cssText->c_str(), L"500") ||
								!std::wcscmp(cssText->c_str(), L"600") ||
								!std::wcscmp(cssText->c_str(), L"700") ||
								!std::wcscmp(cssText->c_str(), L"800") ||
								!std::wcscmp(cssText->c_str(), L"900") ||
							/*	!tcscmp(cssText.c_str(), OLESTR("normal")) ||*/
								!std::wcscmp(cssText->c_str(), L"bold") ||
								!std::wcscmp(cssText->c_str(), L"bolder") ||
								!std::wcscmp(cssText->c_str(), L"lighter"))
					{
						fontWeight = cssText;
					}
					else
					{
						const WCHAR* p1 = cssText->c_str();
						const WCHAR* p2;
						if (ParseValueUnitString(p1, &p2, (double*)NULL) != CSS_UNKNOWN)
						{
							if (*p2 == '/')	// line-height
							{
								lineHeight = new StringW(string_copy(p2+1));
								fontHeight = new StringW(string_copy(p1, p2 - p1));
							}
							else if (*p2 == 0)
							{
								fontHeight = cssText;
							}
						}

						while (std::isspace(*p)) p++;

						break;
					}
					while (std::isspace(*p)) p++;
				}

				if (*p)
				{
					fontFamily = new StringW(string_copy(p));
				}

				internalSetLonghandProperty(CSSProperty_font_style, fontStyle, priority, false);
				internalSetLonghandProperty(CSSProperty_font_weight, fontWeight, priority, false);
				internalSetLonghandProperty(CSSProperty_font_size, fontHeight, priority, false);
				internalSetLonghandProperty(CSSProperty_font_family, fontFamily, priority, false);
#endif
			}
			break;

		case CSSProperty_text_decoration:
			{

				ASSERT(0);
#if 0
				//if (tcscmp(value, OLESTR(OLESTR(L"none") != 0)
				{
					uint8 flags = 0;
					int len = value->Length();
					const WCHAR* p = value->c_str();

					int i = 0;
					while (i < len)
					{
						//sysstring str = OLESTR("");

						int start = i;

						while (i < len && !std::isspace(p[i]))
						{
							i++;
						}
						int sublen = i - start;

						if (!_wcsnicmp(p + start, L"underline", sublen))
						{
							flags |= 1;
						}
						else if (!_wcsnicmp(p + start, L"overline", sublen))
						{
							flags |= 2;
						}
						else if (!_wcsnicmp(p + start, L"line-through", sublen))
						{
							flags |= 4;
						}
						else if (!_wcsnicmp(p + start, L"blink", sublen))
						{
							flags |= 8;
						}

						while (i < len && std::isspace(p[i]))
						{
							i++;
						}
					}

				// Initial values
					internalSetLonghandProperty(CSSProperty_text_underline_mode, csspropertyList[CSSProperty_text_underline_mode].m_initialCSS, priority, false);
					internalSetLonghandProperty(CSSProperty_text_underline_color, csspropertyList[CSSProperty_text_underline_color].m_initialCSS, priority, false);
					internalSetLonghandProperty(CSSProperty_text_underline_width, csspropertyList[CSSProperty_text_underline_width].m_initialCSS, priority, false);

					internalSetLonghandProperty(CSSProperty_text_overline_mode, csspropertyList[CSSProperty_text_overline_mode].m_initialCSS, priority, false);
					internalSetLonghandProperty(CSSProperty_text_overline_color, csspropertyList[CSSProperty_text_overline_color].m_initialCSS, priority, false);
					internalSetLonghandProperty(CSSProperty_text_overline_width, csspropertyList[CSSProperty_text_overline_width].m_initialCSS, priority, false);

					internalSetLonghandProperty(CSSProperty_text_line_through_mode, csspropertyList[CSSProperty_text_line_through_mode].m_initialCSS, priority, false);
					internalSetLonghandProperty(CSSProperty_text_line_through_color, csspropertyList[CSSProperty_text_line_through_color].m_initialCSS, priority, false);
					internalSetLonghandProperty(CSSProperty_text_line_through_width, csspropertyList[CSSProperty_text_line_through_width].m_initialCSS, priority, false);

					//internalSetLonghandProperty(CSSProperty_text_blink, OLESTR(OLESTR(L"none", false);

					if (flags & 1)
						internalSetLonghandProperty(CSSProperty_text_underline_style, WSTR("solid"), priority, false);
					else
						internalSetLonghandProperty(CSSProperty_text_underline_style, csspropertyList[CSSProperty_text_underline_style].m_initialCSS, priority, false);

					if (flags & 2)
						internalSetLonghandProperty(CSSProperty_text_overline_style, WSTR("solid"), priority, false);
					else
						internalSetLonghandProperty(CSSProperty_text_overline_style, csspropertyList[CSSProperty_text_overline_style].m_initialCSS, priority, false);

					if (flags & 4)
						internalSetLonghandProperty(CSSProperty_text_line_through_style, WSTR("solid"), priority, false);
					else
						internalSetLonghandProperty(CSSProperty_text_line_through_style, csspropertyList[CSSProperty_text_line_through_style].m_initialCSS, priority, false);

					if (flags & 8)
						;//internalSetLonghandProperty(CSSProperty_text_blink, OLESTR(OLESTR(L"blink", priority, false);
					else
						;//internalSetLonghandProperty(CSSProperty_text_blink, csspropertyList[CSSProperty_text_blink].m_initialCSSpriority, false);
				}
#endif
			}
			break;

		case CSSProperty_border_radius:
			{
				internalSetLonghandProperty(CSSProperty_border_top_left_radius, value, priority, false);
				internalSetLonghandProperty(CSSProperty_border_top_right_radius, value, priority, false);
				internalSetLonghandProperty(CSSProperty_border_bottom_left_radius, value, priority, false);
				internalSetLonghandProperty(CSSProperty_border_bottom_right_radius, value, priority, false);
			}
			break;

		case CSSProperty_white_space:
			{
				ASSERT(0);
#if 0
				if (!_wcsicmp(value->c_str(), L"normal"))
				{
					internalSetLonghandProperty(CSSProperty_wrap_option, WSTR("wrap"), priority, false);
					internalSetLonghandProperty(CSSProperty_linefeed_treatment, WSTR("auto"), priority, false);
					internalSetLonghandProperty(CSSProperty_white_space_treatment, WSTR("ignore-if-surrounding-linefeed"), priority, false);
					internalSetLonghandProperty(CSSProperty_all_space_treatment, WSTR("collapse"), priority, false);
				}
				else if (!_wcsicmp(value->c_str(), L"pre"))
				{
					internalSetLonghandProperty(CSSProperty_wrap_option, WSTR("no-wrap"), priority, false);
					internalSetLonghandProperty(CSSProperty_linefeed_treatment, WSTR("preserve"), priority, false);
					internalSetLonghandProperty(CSSProperty_white_space_treatment, WSTR("preserve"), priority, false);
					internalSetLonghandProperty(CSSProperty_all_space_treatment, WSTR("preserve"), priority, false);
				}
				else if (!_wcsicmp(value->c_str(), L"nowrap"))
				{
					ASSERT(0);
				}
				else if (!_wcsicmp(value->c_str(), L"pre-wrap"))
				{
					ASSERT(0);
				}
				else if (!_wcsicmp(value->c_str(), L"pre-line"))
				{
					ASSERT(0);
				}
#endif
			}
			break;

		default:
			ASSERT(0);
		}
	}
}

void CSSStyleDeclaration::internalSetProperty(StringIn propertyName, StringIn value, StringIn priority)
{
	int propertyIndex = GetCSSPropertyIndex(propertyName);
	if (propertyIndex >= 0)
	{
		internalSetProperty(propertyIndex, value, priority);
	}
	else	// Unknown property
	{
		internalSetLonghandProperty(propertyName, -1, value, priority, true);
	}
}

Object* CSSStyleDeclaration::GetPropertyValue(Object* pProperty)
{
	return getPropertyCSSValue(dynamic_cast<CSSProperty*>(pProperty)->get_Name());
}

bool CSSStyleDeclaration::HasProperty(Object* pProperty)
{
	return getPropertyCSSValue(dynamic_cast<CSSProperty*>(pProperty)->get_Name()) != NULL;
}

//virtual
void CSSStyleDeclaration::OnCSSValueChanged(CSSPrimitiveValue* pValue)
{
	if (m_bSettingPropertyText == 0)
	{
		m_bCSSTextUpdated = false;

		if (m_pListener)
		{
			m_pListener->OnStyleDeclChanged(this);
		}

#if 0
		for (int i = 0; i < m_handlers.size(); i++)
		{
			m_handlers[i]->handleCSSEvent(0, NULL/*sysstring()*/);
		}
#endif
	}
}

//virtual
void CSSStyleDeclaration::OnCSSValueChanged(SVGPaint* pValue)
{
	if (m_bSettingPropertyText == 0)
	{
		m_bCSSTextUpdated = false;

		if (m_pListener)
		{
			m_pListener->OnStyleDeclChanged(this);
		}

#if 0
		for (int i = 0; i < m_handlers.size(); i++)
		{
			m_handlers[i]->handleCSSEvent(0, NULL/*sysstring()*/);
		}
#endif
	}
}

//virtual
void CSSStyleDeclaration::OnCSSValueChanged(SVGColor* pValue)
{
	if (m_bSettingPropertyText == 0)
	{
		m_bCSSTextUpdated = false;

		if (m_pListener)
		{
			m_pListener->OnStyleDeclChanged(this);
		}

#if 0
		for (int i = 0; i < m_handlers.size(); i++)
		{
			m_handlers[i]->handleCSSEvent(0, NULL/*sysstring()*/);
		}
#endif
	}
}

//virtual
void CSSStyleDeclaration::OnCSSValueChanged(CSSValueList* pValue)
{
	if (m_bSettingPropertyText == 0)
	{
		m_bCSSTextUpdated = false;

		if (m_pListener)
		{
			m_pListener->OnStyleDeclChanged(this);
		}

#if 0
		for (int i = 0; i < m_handlers.size(); i++)
		{
			m_handlers[i]->handleCSSEvent(0, NULL/*sysstring()*/);
		}
#endif
	}
}

////////////////////////////////////////////////
// public methods

unsigned int CSSStyleDeclaration::get_length() const
{
	int n = 0;

	for (size_t i = 0; i < m_decls.size(); ++i)
	{
		if (m_decls[i]->m_bExcplicitSet)
		{
			n++;
		}
	}

	return n;
}

String CSSStyleDeclaration::item(unsigned int index) const
{
	for (size_t i = 0; i < m_decls.size(); ++i)
	{
		if (m_decls[i]->m_bExcplicitSet)
		{
			if (index == 0)
			{
				return m_decls[i]->m_propertyName;
			}

			--index;
		}
	}

	return nullptr;
}

void CSSStyleDeclaration::setProperty(StringIn propertyName, StringIn value, StringIn priority)
{
	ASSERT(propertyName != nullptr);
	if (propertyName == nullptr) THROW(-1);

	internalSetProperty(propertyName, value, priority);

/* This will be called indirectly
	m_bCSSTextUpdated = FALSE;

  if (m_pListener)
	{
		m_pListener->OnStyleDeclChanged(this);
	}
*/
}

CSSRuleImpl* CSSStyleDeclaration::get_parentRule()
{
	return m_parentRule;
}

CSSValue* CSSStyleDeclaration::getPropertyCSSValue(StringIn propertyName)
{
	CSSDecl* pDecl = FindDecl(propertyName);
	if (pDecl)
	{
		return pDecl->m_pValue;
	}
	else
	{
		return nullptr;
	}
}

String CSSStyleDeclaration::removeProperty(StringIn propertyName)
{
	for (size_t i = 0; i < m_decls.size(); i++)
	{
		if (propertyName == m_decls[i]->m_propertyName)
		{
		// Return previous value
		//	sysstring prevValue = m_decls[i]->m_pValue->getStringValue();
			String prevValue = m_decls[i]->m_pValue->get_cssText();

			delete m_decls[i];
			ASSERT(0);
			//m_decls.RemoveAt(i);
			m_bCSSTextUpdated = false;

			if (m_pListener)
			{
				m_pListener->OnStyleDeclChanged(this);
			}
#if 0
			for (int i = 0; i < m_handlers.size(); i++)
			{
				m_handlers[i]->handleCSSEvent(0, NULL/*sysstring()*/);
			}
#endif
			return prevValue;
		}
	}

	// Property didn't exist
	return nullptr;
}

String CSSStyleDeclaration::getPropertyPriority(StringIn propertyName)
{
	CSSDecl* pDecl = FindDecl(propertyName);
	if (pDecl)
	{
		return pDecl->m_priority;
	}
	return nullptr;
}

}	// Web
}
