#include "stdafx.h"
#include "LSVG2.h"
#include "SVGFEFloodElement.h"
#include "PSVGFEFloodElement.h"

namespace System
{
namespace Web
{

SVGFEFloodElement::SVGFEFloodElement(NamedNodeMap* attributes) : SVGElement(new PSVGFEFloodElement(this), attributes)
{
//	SetAllValues(this);	// ???
}

}	// Web
}	// System
