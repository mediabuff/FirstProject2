#include "stdafx.h"
#include "GUI2.h"

namespace System
{
namespace Gui
{

IMP_DEPENDENCY_PROPERTY(double, Timeline, Duration, 0.0);

Timeline::Timeline(DependencyClass* depClass) : DependencyObject(depClass)
{
}


}
}
