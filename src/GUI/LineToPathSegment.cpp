#include "stdafx.h"
#include "GUI2.h"

namespace System
{
namespace Gui
{

IMP_DEPENDENCY_PROPERTY(float, LineToPathSegment, X, 0.0f)
IMP_DEPENDENCY_PROPERTY(float, LineToPathSegment, Y, 0.0f)

DependencyClass* LineToPathSegment::get_Class()
{
	static DependencyClass depclass(typeid(thisClass), baseClass::get_Class());

	static DependencyProperty* properties[] =
	{
		get_XProperty(),
		get_YProperty(),
	};

	return &depclass;
}

DependencyClass* LineToPathSegment::pClass(get_Class());

LineToPathSegment::LineToPathSegment() : PathSegment(get_Class())
{
}

LineToPathSegment::LineToPathSegment(gm::PointF point) : PathSegment(get_Class())
{
	SetPropertyValue(get_XProperty(), point.X);
	SetPropertyValue(get_YProperty(), point.Y);
}

void LineToPathSegment::Sink(ID2D1GeometrySink* pGeometrySink)
{
	pGeometrySink->AddLine(cnv(get_Point()));
}

}	// Gui
}	// System
