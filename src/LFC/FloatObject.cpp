#include "stdafx.h"
#include "LFC.h"

namespace System
{

FloatObject FloatObject::s_Zero(0);
FloatObject FloatObject::s_NegativeZero(float_NegativeZero.value);
FloatObject FloatObject::s_One(float_One.value);
FloatObject FloatObject::s_NegativeInfinity(float_NegativeInfinity.value);
FloatObject FloatObject::s_PositiveInfinity(float_PositiveInfinity.value);
FloatObject FloatObject::s_NaN(float_NegativeZero.value);
FloatObject FloatObject::s_Epsilon(FLT_EPSILON);

String FloatObject::ToString()
{
	return String::FromNumber(m_value);
}

FloatObject* FloatObject::GetObject(float value)
{
	if (value == s_Zero.m_value)						return &s_Zero;
	else if (value == s_NegativeZero.m_value)		return &s_NegativeZero;
	else if (value == s_One.m_value)					return &s_One;
	else if (value == s_NegativeInfinity.m_value)	return &s_NegativeInfinity;
	else if (value == s_PositiveInfinity.m_value)	return &s_PositiveInfinity;
	else if (value == s_NaN.m_value)					return &s_NaN;
	else if (value == FLT_EPSILON)					return &s_Epsilon;
	else												return new FloatObject(value);
}

}	// System
