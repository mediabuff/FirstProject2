#include "stdafx.h"
#include "X3D2.h"

namespace System
{
namespace x3d
{

NodeType* X3DInfoNode::GetNodeType()
{
	/*
	X3DFieldDefinition* fields[] =
	{
		get_infoFieldDef(),
		get_titleFieldDef(),
	};
	NodeType nodeType(null, typeid(X3DInfoNode), fields, _countof(fields), &X3DChildNode::GetNodeType());
	*/

	static NodeType nodeType(nullptr, typeid(X3DInfoNode), nullptr, 0, baseClass::GetNodeType());
	return &nodeType;
}

X3DInfoNode::X3DInfoNode(NodeType* nodeType) : X3DChildNode(nodeType)
{
}

}	// x3d
}	// System
