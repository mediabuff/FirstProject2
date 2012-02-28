#include "stdafx.h"
#include "GUI2.h"

namespace System
{
namespace Gui
{

IMP_DEPENDENCY_PROPERTY(float, RotateTransform, AngleRadians, 0.0f)
IMP_DEPENDENCY_PROPERTY(float, RotateTransform, AngleDegrees, _this().getAngleRadians() * float(180.0/M_PI))

DependencyClass* RotateTransform::get_Class()
{
	static DependencyClass depclass(typeid(thisClass), baseClass::get_Class());

	static DependencyProperty* properties[] =
	{
		get_AngleRadiansProperty(),
		get_AngleDegreesProperty(),
	};

	return &depclass;
}

DependencyClass* RotateTransform::pClass(get_Class());

RotateTransform::RotateTransform() : Transform(get_Class())
{
}

RotateTransform::RotateTransform(FloatRadians angle) : Transform(get_Class())
{
	set_AngleRadians(angle.m_value);
}

gm::matrix3f RotateTransform::get_Matrix()
{
	return gm::matrix3f::getRotate(gmDegrees(get_AngleRadians()));
}

}	// Gui
}