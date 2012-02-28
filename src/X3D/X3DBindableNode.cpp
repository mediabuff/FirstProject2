#include "StdAfx.h"
#include "X3D2.h"

namespace System
{
namespace x3d
{

IMP_X3DFIELD0(X3DBindableNode, SFBool, set_bind, inputOnly)
IMP_X3DFIELD0(X3DBindableNode, SFBool, isBound, outputOnly)
IMP_X3DFIELD0(X3DBindableNode, SFTime, bindTime, outputOnly)

NodeType* X3DBindableNode::GetNodeType()
{
	static X3DFieldDefinition* fields[] =
	{
		get_set_bindFieldDef(),
		get_isBoundFieldDef(),
		get_bindTimeFieldDef(),
	};

	static NodeType nodeType(nullptr, typeid(X3DBindableNode), fields, _countof(fields), baseClass::GetNodeType());
	return &nodeType;
}

X3DBindableNode::X3DBindableNode(NodeType* nodeType) : X3DChildNode(nodeType),
	m_set_bind(new SFBool(get_set_bindFieldDef(), this)),
	m_isBound(new SFBool(get_isBoundFieldDef(), this)),
	m_bindTime(new SFTime(get_bindTimeFieldDef(), this))
{
}

}
}
