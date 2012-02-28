#include "stdafx.h"
#include "LSVG2.h"

#include "SVGDefsElement.h"

namespace System
{
namespace Web
{

SVGDefsElement::SVGDefsElement(NamedNodeMap* attributes) : SVGElement(new PSVGElement(this), attributes)
{
}

}	// Web
}	// System
